#include "stdafx.h"
#include "TaxationList31View.h"

#include "ShinBuy31.h"
#include "ShinJigyo31.h"//'14.08.11
// kasai add -->
#include "ShinJigyo31Ex.h"
// kasai add <--
// 2023/07/11 h.wada add -> 
#include "ShinJigyo31Ex2.h"
// h.wada <--
// 2023/07/14 tanaka add -> 
#include "ShinNougyou31Ex2.h"
// tanaka add <--
#include "ShinFudousan31.h"//'14.08.11
#include "ShinNougyou31.h"//'14.08.11
// 211129_h add -->
#include "ShinFudousan31Ex.h"
#include "ShinNougyou31Ex.h"
// 211129_h add <--
#include "ShinFudousan31Ex2.h"
#include "ShinSell31.h"//'14.08.11
// kasai add -->
#include "ShinSell31Ex.h"
#include "ShinBuy31Ex.h"
// kasai add <--
// tanaka add -->
#include "ShinSell31Ex2.h"
#include "ShinBuy31Ex2.h"
// tanaka add <--
#include "ShinKniinp31.h"
// 211201_h add -->
#include "ShinKniinp31Ex.h"
// 211201_h add <--

//2016.10.20 INSERT START
//#include "ShinKniinp6.h"
//#include "ShinKniinpKeika6syu.h"
//2016.10.20 INSERT END

//-----------------------------------------------------------------------------
// 印刷情報のセット
//-----------------------------------------------------------------------------
// 引数	pVprn	：	出力クラス
//		pPrtPar	：	印刷情報
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTaxationList31View::SetPrtPar( CVPrint *pVprn, NPRT_PAR *pPrtPar , SH_PRINT_CMINFO *pPrintCmInfo )
{
	// 情報セット
	ASSERT( pVprn );
	if( pVprn == NULL ){
		return -1;
	}
	m_pVprn = pVprn;
	ASSERT( pPrtPar );
	if( pPrtPar == NULL ){
		return -1;
	}
	m_pPset = pPrtPar;

	ASSERT( pPrintCmInfo );
	if( pPrintCmInfo == NULL ){
		return -1;
	}
	m_PrintCmInfo = pPrintCmInfo;
/*	memset( pPrtPar, '\0', sizeof(NPRT_PAR) );
	pPrtPar->psPaperSize	= DMPAPER_A4;			// 用紙サイズ
	pPrtPar->psOrientation	= DMORIENT_PORTRAIT;	// 用紙方向
	pPrtPar->psPrevFlg		= 0x0100;				// プレビュー可
	pPrtPar->psPageNum		= 0;					// ページ数
//	pPrtPar->psColorSet       = &m_Cset;

    GetIcsofnDirectory( (LPTSTR)pPrtPar->psSigCopath );
	char prnam[80] = {0};
	memset( prnam, '\0', sizeof(prnam) );
	if( PCOM_GetString( "SelectPrinter", prnam) != -1) {
		strcpy_s( (char*)pPrtPar->psPrinterName, sizeof(pPrtPar->psPrinterName), prnam );
	}
	else{
		pPrtPar->psPrinterName[0] = 0;
	}*/

	return 0;
}

#ifdef _TEST_
//-----------------------------------------------------------------------------
// 印刷処理
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CTaxationList31View::PrintSellList()
{
	SF_PAR	PrtFont;
	memset( &PrtFont, '\0', sizeof(SF_PAR) );
	PrtFont.sf_ksiz = FNT200z;

	// 印刷テスト
	m_pVprn->DrawString( 0, 300, 1000, _T("DEFGH"), &PrtFont );

	return 0;
}
#else

#define		Msiz	512*20
//-----------------------------------------------------------------------------
// 印刷処理（課税売上高）
//-----------------------------------------------------------------------------
// 引数	pno		：	ページ番号
//		fno		：	
//		pDC		：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CTaxationList31View::PrintSell31List( int pno, int fno, CDC *pDC )
{
	int	st = -1;

	char	*pBp = NULL;
	if( (pBp = new char[Msiz]) == NULL ){
		return st;
	}
	memset( pBp, '\0', (sizeof(char))*Msiz );

	char	*pSvBp = pBp;

//	for( int dp=1; dp<=2; ++dp ){
	for( int dp=1; dp<=3; ++dp ){

// kasai del -->
		/*if( dp == 1 ){
			if( m_pShinSell31 ){
				m_pShinSell31->SetPrintInfo(m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinSell31->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if( m_pShinSell31 ){
				st = m_pShinSell31->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 3 ){
			if( m_pShinSell31 ){
				st = m_pShinSell31->PrintHyo3( pno, dp, pDC, pBp );
			}
		}*/
// kasai del <--
// kasai add -->
		if( dp == 1 ){
			if ( m_pShinSell31Ex2 ) {
				m_pShinSell31Ex2->SetPrintInfo(m_pVprn, m_pPset, m_PrintCmInfo);
				st = m_pShinSell31Ex2->PrintHyo1( pno, dp, pDC, pBp );
			}
			else if( m_pShinSell31Ex ){
				m_pShinSell31Ex->SetPrintInfo(m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinSell31Ex->PrintHyo1( pno, dp, pDC, pBp );
			}
			else if( m_pShinSell31 ){
				m_pShinSell31->SetPrintInfo(m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinSell31->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if ( m_pShinSell31Ex2 ) {
				m_pShinSell31Ex2->SetPrintInfo(m_pVprn, m_pPset, m_PrintCmInfo);
				st = m_pShinSell31Ex2->PrintHyo2( pno, dp, pDC, pBp );
			}
			else if( m_pShinSell31Ex ){
				st = m_pShinSell31Ex->PrintHyo2( pno, dp, pDC, pBp );
			}
			else if( m_pShinSell31 ){
				st = m_pShinSell31->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 3 ){
			if ( m_pShinSell31Ex2 ) {
				m_pShinSell31Ex2->SetPrintInfo( m_pVprn, m_pPset, m_PrintCmInfo );
				st = m_pShinSell31Ex2->PrintHyo3( pno, dp, pDC, pBp );
			}
			if( m_pShinSell31Ex ){
				st = m_pShinSell31Ex->PrintHyo3( pno, dp, pDC, pBp );
			}
			else if( m_pShinSell31 ){
				st = m_pShinSell31->PrintHyo3( pno, dp, pDC, pBp );
			}
		}
// kasai add <--
		if( st ){
			break;
		}
	}

	delete [] pSvBp;

	return st;
}

#endif

//-----------------------------------------------------------------------------
// 印刷（課税仕入高）
//-----------------------------------------------------------------------------
// 引数	
// 
// 
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int CTaxationList31View::PrintBuy31List( int pno, int fno, CDC *pDC )
{
	int	st = -1;

	char	*pBp = NULL;
	if( (pBp = new char[Msiz]) == NULL ){
		return st;
	}
	memset( pBp, '\0', (sizeof(char))*Msiz );

	char	*pSvBp = pBp;

//	for( int dp=1; dp<=2; ++dp ){
	for( int dp=1; dp<=3; ++dp ){

// kasai del -->
		/*if( dp == 1 ){
			if( m_pShinBuy31 ){
				m_pShinBuy31->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinBuy31->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if( m_pShinBuy31 ){
				st = m_pShinBuy31->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 3 ){
			if( m_pShinBuy31 ){
				st = m_pShinBuy31->PrintHyo3( pno, dp, pDC, pBp );
			}
		}*/
// kasai del <--
// kasai add -->
		if( dp == 1 ){
			if ( m_pShinBuy31Ex2 ) {
				m_pShinBuy31Ex2->SetPrintInfo(m_pVprn, m_pPset, m_PrintCmInfo);
				st = m_pShinBuy31Ex2->PrintHyo1(pno, dp, pDC, pBp);
			}
			else if( m_pShinBuy31Ex ){
				m_pShinBuy31Ex->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinBuy31Ex->PrintHyo1( pno, dp, pDC, pBp );
			}
			else if( m_pShinBuy31 ){
				m_pShinBuy31->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinBuy31->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if ( m_pShinBuy31Ex2 ) {
				st = m_pShinBuy31Ex2->PrintHyo2(pno, dp, pDC, pBp);
			}
			else if( m_pShinBuy31Ex ){
				st = m_pShinBuy31Ex->PrintHyo2( pno, dp, pDC, pBp );
			}
			else if( m_pShinBuy31 ){
				st = m_pShinBuy31->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 3 ){
			if ( m_pShinBuy31Ex2 ) {
				st = m_pShinBuy31Ex2->PrintHyo3(pno, dp, pDC, pBp);
			}
			if( m_pShinBuy31Ex ){
				st = m_pShinBuy31Ex->PrintHyo3( pno, dp, pDC, pBp );
			}
			else if( m_pShinBuy31 ){
				st = m_pShinBuy31->PrintHyo3( pno, dp, pDC, pBp );
			}
		}
// kasai add <--
		if( st ){
			break;
		}
	}

	delete [] pSvBp;

	return st;
}
//-----------------------------------------------------------------------------
// 印刷（課税取引金額計算表 事業所得）
//-----------------------------------------------------------------------------
// 引数	
// 
// 
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int CTaxationList31View::PrintJigyo31List( int pno, int fno, CDC *pDC )
{
	int	st = 0;
	
	char	*pBp = NULL;
	if( (pBp = (char *) malloc( Msiz )) == NULL ){
		return -1;
	}
	m_pBpSv = pBp;
	st = 0;

//	for( int dp=1; dp<=2; ++dp ){
	for( int dp=1; dp<=3; ++dp ){

// kasai del -->
		/*if( dp == 1 ){
			if( m_pShinJigyo31 ){
				m_pShinJigyo31->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinJigyo31->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if( m_pShinJigyo31 ){
				st = m_pShinJigyo31->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 3 ){
			if( m_pShinJigyo31 ){
				st = m_pShinJigyo31->PrintHyo3( pno, dp, pDC, pBp );
			}
		}*/
// kasai del <--
// 2023/07/11 h.wada change -> //m_pShinJigyo31Ex2からの関数呼び出し追加 
// kasai add -->
		if( dp == 1 ){
			if (m_pShinJigyo31Ex2) {
				m_pShinJigyo31Ex2->SetPrintInfo(m_pVprn, m_pPset, m_PrintCmInfo);
				st = m_pShinJigyo31Ex2->PrintHyo1(pno, dp, pDC, pBp);
			}
			else if( m_pShinJigyo31Ex ){
				m_pShinJigyo31Ex->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinJigyo31Ex->PrintHyo1( pno, dp, pDC, pBp );
			}
			else if( m_pShinJigyo31 ){
				m_pShinJigyo31->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinJigyo31->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if (m_pShinJigyo31Ex2) {
				st = m_pShinJigyo31Ex2->PrintHyo2(pno, dp, pDC, pBp);
			}
			else if( m_pShinJigyo31Ex ){
				st = m_pShinJigyo31Ex->PrintHyo2( pno, dp, pDC, pBp );
			}
			else if( m_pShinJigyo31 ){
				st = m_pShinJigyo31->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 3 ){
			if (m_pShinJigyo31Ex2) {
				st = m_pShinJigyo31Ex2->PrintHyo3(pno, dp, pDC, pBp);
			}
			else if( m_pShinJigyo31Ex ){
				st = m_pShinJigyo31Ex->PrintHyo3( pno, dp, pDC, pBp );
			}
			else if( m_pShinJigyo31 ){
				st = m_pShinJigyo31->PrintHyo3( pno, dp, pDC, pBp );
			}
		}
// kasai add <--
// h,wada change <--
		if( st ){
			break;
		}
	}

	free( pBp );

	return st;
}

//----------------
//-----------------------------------------------------------------------------
// 印刷（課税取引金額計算表 不動産所得）
//-----------------------------------------------------------------------------
// 引数	
// 
// 
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int CTaxationList31View::PrintFudousan31List( int pno, int fno, CDC *pDC )
{
	int	st = 0;
	
	char	*pBp = NULL;
	if( (pBp = (char *) malloc( Msiz )) == NULL )	return( -1 );
	m_pBpSv = pBp;
	st = 0;

//	for( int dp=1; dp<=2; ++dp ){
	for( int dp=1; dp<=3; ++dp ){
// 211129_h del -->
		//if( dp == 1 ){
		//	if( m_pShinFudousan31 ){
		//		m_pShinFudousan31->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
		//		st = m_pShinFudousan31->PrintHyo1( pno, dp, pDC, pBp );
		//	}
		//}
		//else if( dp == 2 ){
		//	if( m_pShinFudousan31 ){
		//		st = m_pShinFudousan31->PrintHyo2( pno, dp, pDC, pBp );
		//	}
		//}
		//else if( dp == 3 ){
		//	if( m_pShinFudousan31 ){
		//		st = m_pShinFudousan31->PrintHyo3( pno, dp, pDC, pBp );
		//	}
		//}
// 211129_h del <--
// 211129_h add -->
		if( dp == 1 ){
			if (m_pShinFudousan31Ex2) {
				m_pShinFudousan31Ex2->SetPrintInfo(m_pVprn, m_pPset, m_PrintCmInfo);
				st = m_pShinFudousan31Ex2->PrintHyo1(pno, dp, pDC, pBp);
			}
			if( m_pShinFudousan31Ex ){
				m_pShinFudousan31Ex->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinFudousan31Ex->PrintHyo1( pno, dp, pDC, pBp );
			}
			else if( m_pShinFudousan31 )	{
				m_pShinFudousan31->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinFudousan31->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if (m_pShinFudousan31Ex2) {
				st = m_pShinFudousan31Ex2->PrintHyo2(pno, dp, pDC, pBp);
			}
			if( m_pShinFudousan31Ex ){
				st = m_pShinFudousan31Ex->PrintHyo2( pno, dp, pDC, pBp );
			}
			else if( m_pShinFudousan31 )	{
				st = m_pShinFudousan31->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 3 ){
			if (m_pShinFudousan31Ex2) {
				st = m_pShinFudousan31Ex2->PrintHyo3(pno, dp, pDC, pBp);
			}
			if( m_pShinFudousan31Ex ){
				st = m_pShinFudousan31Ex->PrintHyo3( pno, dp, pDC, pBp );
			}
			else if( m_pShinFudousan31 )	{
				st = m_pShinFudousan31->PrintHyo3( pno, dp, pDC, pBp );
			}
		}
// 211129_h add <--
		if( st ){
			break;
		}
	}

	free( pBp );

	return st;
}
//-----------------------------------------------------------------------------
// 印刷（課税取引金額計算表 農業所得）
//-----------------------------------------------------------------------------
// 引数	
// 
// 
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int CTaxationList31View::PrintNougyou31List( int pno, int fno, CDC *pDC )
{
	int	st = 0;
	
	char	*pBp = NULL;
	if( (pBp = (char *) malloc( Msiz )) == NULL ){
		return -1;
	}
	m_pBpSv = pBp;

//	for( int dp=1; dp<=3; ++dp ){
	for( int dp=1; dp<=4; ++dp ){
// 211129_h del -->
		//if( dp == 1 ){
		//	if( m_pShinNougyou31 ){
		//		m_pShinNougyou31->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
		//		st = m_pShinNougyou31->PrintHyo1( pno, dp, pDC, pBp );
		//	}
		//}
		//else if( dp == 2 ){
		//	if( m_pShinNougyou31 ){
		//		st = m_pShinNougyou31->PrintHyo2( pno, dp, pDC, pBp );
		//	}
		//}
		//else if( dp == 3 ){
		//	if( m_pShinNougyou31 ){
		//		st = m_pShinNougyou31->PrintHyo3( pno, dp, pDC, pBp );
		//	}
		//}
		//else if( dp == 4 ){
		//	if( m_pShinNougyou31 ){
		//		st = m_pShinNougyou31->PrintHyo4( pno, dp, pDC, pBp );
		//	}
		//}
// 211129_h del <--
// 211129_h add -->
		if( dp == 1 ){
			if ( m_pShinNougyou31Ex2 ) {
				m_pShinNougyou31Ex2->SetPrintInfo(m_pVprn, m_pPset, m_PrintCmInfo);
				st = m_pShinNougyou31Ex2->PrintHyo1(pno, dp, pDC, pBp);
			}
			else if( m_pShinNougyou31Ex ){
				m_pShinNougyou31Ex->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinNougyou31Ex->PrintHyo1( pno, dp, pDC, pBp );
			}
			else	{
				m_pShinNougyou31->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinNougyou31->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if ( m_pShinNougyou31Ex2 ) {
				st = m_pShinNougyou31Ex2->PrintHyo2(pno, dp, pDC, pBp);
			}
			else if( m_pShinNougyou31Ex ){
				st = m_pShinNougyou31Ex->PrintHyo2( pno, dp, pDC, pBp );
			}
			else	{
				st = m_pShinNougyou31->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 3 ){
			if ( m_pShinNougyou31Ex2 ) {
				st = m_pShinNougyou31Ex2->PrintHyo3(pno, dp, pDC, pBp);
			}
			else if( m_pShinNougyou31Ex ){
				st = m_pShinNougyou31Ex->PrintHyo3( pno, dp, pDC, pBp );
			}
			else	{
				st = m_pShinNougyou31->PrintHyo3( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 4 ){
			if ( m_pShinNougyou31Ex2 ) {
				st = m_pShinNougyou31Ex2->PrintHyo4(pno, dp, pDC, pBp);
			}
			else if( m_pShinNougyou31Ex ){
				st = m_pShinNougyou31Ex->PrintHyo4( pno, dp, pDC, pBp );
			}
			else	{
				st = m_pShinNougyou31->PrintHyo4( pno, dp, pDC, pBp );
			}
		}

// 211129_h add <--
		if( st ){
			break;
		}
	}

	free( pBp );

	return( st );
}

int CTaxationList31View::PrtKniInpData( int hlist )
{
	int		i, j, x, y;
	BYTE	st, flg;
//	KKVAL*	pK = &NRec.KUval[40];
	KKVAL*	pK = &m_pTaxListData->NRec.KUval[40];
	SF_PAR	sfpar;
	char	buf[64];
	char	val[6];

	for( i = flg = 0; i < 9; i++ ){
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		sfpar.sf_fcsg = SFp_xr | SFp_yb | SFcitv;
		sfpar.sf_xfld = m_ListPar.lsTrowWidth;
		sfpar.sf_yfld = m_ListPar.lsLinWidth;
		sfpar.sf_mrgn[0] = 120;
		sfpar.sf_mrgn[1] = 50;
		sfpar.sf_mrgn[3] = 40;
		sfpar.sf_ksiz = FNT108z;

		switch( i ){
		case 0:		x = 2;	break;
		case 1:		x = 4;	break;
		case 2:		x = 5;	break;
		case 3:		x = 6;	break;
		case 4:		x = 7;	break;
//2016.10.20 UPDATE START
		//case 5:		x = 9;	break;
		//case 6:		x = 10;	break;
		//case 7:		x = 12;	break;

		case 5:
			if( m_SixKindFlg == TRUE ){
				x = 10;
			}else{
				x = 9;
			}
			break;
		case 6:
			if( m_SixKindFlg == TRUE ){
				x = 12;
			}else{
				x = 10;
			}
			break;
		case 7:
			if( m_SixKindFlg == TRUE ){
				x = 14;
			}else{
				x = 12;
			}
			break;
//2016.10.20 UPDATE END
		case 8:		x = 8;	break;		// 10/04/06 追加
		}

		for( j = 0; j < 4; j++ ){
			::ZeroMemory( buf, sizeof( buf ));
			::ZeroMemory( val, sizeof( val ));
			y = j+1;
			st = 0;
/*			switch( j){
			case 0:
				if( i == 7 ){
					if( flg&0x01 )	st = 1;
				}
				else{
					if( l_test( (pK+i)->KKval1 )){
						st = 1;		flg |= 0x01;
					}
				}
				memcpy( val, (pK+i)->KKval1, 6 );
				break;
			case 1:
				if( i == 7 ){
					if( flg&0x02 )	st = 1;
				}
				else{
					if( l_test( (pK+i)->KKval2 )){
						st = 1;		flg |= 0x02;
					}
				}
				memcpy( val, (pK+i)->KKval2, 6 );
				break;
			case 2:
				if( i == 7 ){
					if( flg&0x03 )	st = 1;
				}
				else{
					if( l_test( (pK+i)->KKval1 ) || l_test( (pK+i)->KKval2 ))	st = 1;
				}
				memcpy( val, (pK+i)->KKval3, 6 );
				break;
			case 3:
				if( i == 7 ){
					if( flg&0x04 )	st = 1;
				}
				else{
					if( l_test( (pK+i)->KKval4 )){
						st = 1;		flg |= 0x04;
					}
				}
				memcpy( val, (pK+i)->KKval4, 6 );
				break;
			}*/

//2016.10.20 UPDATE START
			//st = m_pShinKniinp->DispJudge( j, i );

			if( m_SixKindFlg == TRUE ){
///				st = m_pShinKniinp6syu->DispJudge( j, i );
			}else{
//				st = m_pShinKniinp->DispJudge( j, i );
			}
//2016.10.20 UPDATE END

			switch( j ){
				case 0:
					memcpy( val, (pK+i)->KKval1, 6 );
					break;
				case 1:
					memcpy( val, (pK+i)->KKval2, 6 );
					break;
				case 2:
					memcpy( val, (pK+i)->KKval3, 6 );
					break;
				case 3:
					memcpy( val, (pK+i)->KKval4, 6 );
					break;
			}


			if( st )
//				l_print( buf, val, "s,sss,sss,sss,ss9" );
				m_Arith.l_print( buf, val, "s,sss,sss,sss,ss9" );

			m_pVprn->DrawString( hlist, y, x, buf, &sfpar );
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 印刷（課税取引金額計算表 事業別売上高）
//-----------------------------------------------------------------------------
// 引数	
// 
// 
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int CTaxationList31View::PrintKniinp31List( int pno, int fno, CDC *pDC )
{
	int	st=0;
	long	dotx, doty;
	dotx = doty = 0L;
	doty = 0;
	dotx = 0;

	int hList = 0;

// 211203h del -->
	//Mk_List_31( &hList );				// 大枠作成
	//Title_Print_31( hList );			// 上部タイトル作成
	//GyoshuTitle_31( hList );			// 業種タイトル作成
	//st = PrtKniInpData_31( hList );		// 金額出力
// 211203h del <--
// 211203h add -->
	if(m_pShinKniinp31Ex)	{
		Mk_List_31_Ex( &hList );			// 大枠作成
		Title_Print_31( hList );			// 上部タイトル作成
		GyoshuTitle_31_Ex( hList );			// 業種タイトル作成
		st = PrtKniInpData_31_Ex( hList );	// 金額出力
	}
	else	{
		Mk_List_31( &hList );				// 大枠作成
		Title_Print_31( hList );			// 上部タイトル作成
		GyoshuTitle_31( hList );			// 業種タイトル作成
		st = PrtKniInpData_31( hList );		// 金額出力
	}
// 211203h add <--

	m_pVprn->DrawLwframe( hList );
	m_pVprn->DeleteList( hList );

	return 0;
}

int CTaxationList31View::Mk_List_31( int* plist )
{
	long	dotx, doty;
	dotx = doty = 0L;
	doty = 0;
	dotx = 0;

	int i;
	memset( &m_ListPar, '\0', sizeof( LIST_PAR ));

	m_ListPar.lsSx = 980+dotx;
	m_ListPar.lsSy = 4500+doty;
	m_ListPar.lsLins = 38;
	m_ListPar.lsLinWidth = 620;
	m_ListPar.lsTlinWidth = 600;		
	m_ListPar.lsTrowWidth = 5200;	

	m_ListPar.lsRows = 5;
	for( i = 0; i < m_ListPar.lsRows-1; i++ ) {
		m_ListPar.lsRowWidth[i] = 3250;
	}
	m_ListPar.lsLwfpar.lwOtyp  = LKnrW0 | FKrect;
	m_ListPar.lsLwfpar.lwThtyp = LKnrW0;	m_ListPar.lsLwfpar.lwTvtyp = LKnrW0;
	m_ListPar.lsLwfpar.lwDhtyp = 0;		m_ListPar.lsLwfpar.lwDvtyp = LKnrW0;

	if( ( *plist = m_pVprn->MakeList( &m_ListPar ) ) == -1 )		return -1;

	return 0;
}

void CTaxationList31View::Title_Print_31( int hlist )
{

	SF_PAR	sfpar;
	CString	str;

	char	cnm[128] = { 0 }, buf[128] = { 0 };
	long	dotx, doty, x, y, j = 0;
	dotx = doty = 0L;
	doty = 0;
	dotx = 0;

	::ZeroMemory( buf, sizeof(buf) );
	::ZeroMemory( cnm, sizeof(cnm) );

	if( m_pPset->psSigCoprn&0x2 ){
		if( m_PrintCmInfo->OPL ){
			sprintf_s( buf, sizeof( buf ), _T(" %s"), m_PrintCmInfo->CopNam );
		}
		else{
			sprintf_s( buf, sizeof( buf ), _T("[%08d] %s"), m_PrintCmInfo->CopNum, m_PrintCmInfo->CopNam );
		}
	}
	else{
		// マスター
		if( m_pSnHeadData->IsSoleProprietor() ) {
			j = sizeof( m_pSnHeadData->Sn_DIHYO );
			memmove( cnm, m_pSnHeadData->Sn_DIHYO, j );
		}
		else {			// 法人名
			j = sizeof( m_pSnHeadData->Sn_CONAM );
			memmove( cnm, m_pSnHeadData->Sn_CONAM, j );
		}

		if( m_PrintCmInfo->OPL ) {
			sprintf_s( buf, sizeof(buf), _T( " %s" ), cnm );
		}
		else {
			sprintf_s( buf, sizeof( buf ), _T( "[%08d] %s" ), m_pZmsub->zvol->v_cod, cnm );
		}
	}

// 230802Aw add -->
	int		m_FNT180z;
	m_FNT180z	= m_pVprn->RegisterFont(180, 0, 0, _T("ＭＳ 明朝"));
// 230802Aw add <--

	int	i;
	for( i = 0; i < 4; i++ ){
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		str.Empty();

		switch( i ){
		case 0:
// 230802Aw del -->
			//sfpar.sf_ksiz = 180;
// 230802Aw del <--
// 230802Aw add -->
			sfpar.sf_ksiz = m_FNT180z;
// 230802Aw add <--
			sfpar.sf_deco = MAKELONG( LKnrWd, 0);	//アンダーライン
			str = "事業別売上高等の内訳（簡易課税用）";
			x = 4600 + dotx;	y = 2500 + doty;
			break;
		case 1:	// これ何かわからん
			// 氏名
			sfpar.sf_ksiz = FNT108z;
			x = m_ListPar.lsSx+300;	y = 1000 + doty;
			if( strlen( buf ) > 0 ) {
				str = buf;
			}
			break;
		case 2:
			sfpar.sf_fcsg = SFp_xl | SFp_yc;
			sfpar.sf_xfld = m_ListPar.lsTrowWidth;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			sfpar.sf_ksiz = FNT108z;
			sfpar.sf_mrgn[0] = 200;
			str.Format( "（令和%2x年分）", m_pTaxListData->YEAR );
			x = m_ListPar.lsSx;
			y = m_ListPar.lsSy;
			break;
		case 3:
			sfpar.sf_fcsg = SFp_xr | SFp_yc;
			sfpar.sf_xfld = m_ListPar.lsRowWidth[0]*2;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			sfpar.sf_ksiz = FNT108z;
			if( m_pTaxListData->Head.KHstax&0x01 )	str = "（経理方式：税抜き）";
			else									str = "（経理方式：税込み）";
			x = m_ListPar.lsSx+m_ListPar.lsTrowWidth+(m_ListPar.lsRowWidth[0]*2);
			y = m_ListPar.lsSy;
			break;
		}
		m_pVprn->DrawString( 0, x, y, str, &sfpar );
	}

// 230802Aw add -->
	m_pVprn->DeleteRegFont(m_FNT180z);
// 230802Aw add <--

	for( i = 0; i < 5; i++ ){
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		sfpar.sf_fcsg = SFp_xc | SFp_yc;
		sfpar.sf_ksiz = FNT108z;

		str.Empty();
		switch( i ){
		case 0:	str = "区　　　　　分";	break;
		case 1:	str = "決　算　額";	break;
		case 2:	str = "課税取引外";	break;
		case 3:	str = "課税取引金額";	break;
		case 4:	str = "返還等課税取引";	break;
		}
		m_pVprn->DrawString( hlist, i, 0, str, &sfpar );
	}

	for( i = 0; i < 4; i++ ){
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		sfpar.sf_fcsg = SFp_xr | SFp_yt;
		sfpar.sf_ksiz = FNT072z;
		sfpar.sf_mrgn[1] = 20;
		sfpar.sf_mrgn[2] = 50;
		m_pVprn->DrawString( hlist, i+1, 1, "円", &sfpar );
	}

}

void CTaxationList31View::GyoshuTitle_31( int hlist )
{
	SF_PAR	sfpar;
	CString	str;

	long	ltype, x, y;
	long	ltype2, x2, y2;
	int		i, lin;

	for (i = 0; i < 38; i++) {
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		sfpar.sf_fcsg = SFp_xc | SFp_yc | SFcitv;
		sfpar.sf_yfld = m_ListPar.lsLinWidth;
		sfpar.sf_mrgn[0] = 200;
		sfpar.sf_mrgn[1] = 200;
		sfpar.sf_ksiz = FNT108z;

		str.Empty();

		lin = 0;
		y = m_ListPar.lsSy+m_ListPar.lsTlinWidth;
		ltype = MAKELONG(LKnrW0, 4);
		ltype2 = MAKELONG(LKnrW0, 3); //上辺

		switch( i ){
			case 0:
			case 4:
			case 10:
			case 17:
			case 25:
			case 29:
			case 33:
				x = m_ListPar.lsSx;
				sfpar.sf_xfld = m_ListPar.lsTrowWidth;
				break;
			default:
				x = m_ListPar.lsSx + 500;
				sfpar.sf_xfld = m_ListPar.lsTrowWidth - 500;
				break;
		}


		switch( i ){
		case 0:
			str.Format( "第一種事業（卸売業）" );
			lin = 1;
			y += (m_ListPar.lsLinWidth*1);
			break;
		case 1:
			str.Format( "うち税率７．８％適用分" );
			lin = 2;
			y += (m_ListPar.lsLinWidth*2);
			break;
		case 2:
			str.Format( "うち税率６．２４％適用分" );
			lin = 3;
			y += (m_ListPar.lsLinWidth*3);
			break;
		case 3:
			str.Format( "うち税率６．３％適用分" );
			lin = 4;
			y += (m_ListPar.lsLinWidth*4);
			break;
		case 4:
			str.Format( "第二種事業（小売業）" );
			lin = 5;
			y += (m_ListPar.lsLinWidth*5);
			break;
		case 5:
			str.Format( "うち税率７．８％適用分" );
			lin = 6;
			y += (m_ListPar.lsLinWidth*6);
			break;
		case 6:
			str.Format( "うち税率６．２４％適用分" );
			lin = 7;
			y += (m_ListPar.lsLinWidth*7);
			break;
		case 7:
			str.Format( "うち税率６．３％適用分" );
			lin = 8;
			y += (m_ListPar.lsLinWidth*8);
			break;
		case 8:
			str.Format("　【農業事業者】　");
			lin = 9;
			y += (m_ListPar.lsLinWidth * 9);
			break;
		case 9:
			str.Format("うち税率６．２４％適用分");
			lin = 10;
			y += (m_ListPar.lsLinWidth * 10);
			break;
		case 10:
			str.Format( "第三種事業（製造業等）" );
			lin = 11;
			y += (m_ListPar.lsLinWidth*11);
			break;
		case 11:
			str.Format( "うち税率７．８％適用分" );
			lin = 12;
			y += (m_ListPar.lsLinWidth*12);
			break;
		case 12:
			str.Format( "うち税率６．２４％適用分" );
			lin = 13;
			y += (m_ListPar.lsLinWidth*13);
			break;
		case 13:
			str.Format( "うち税率６．３％適用分" );
			lin = 14;
			y += (m_ListPar.lsLinWidth*14);
			break;
		case 14:
			str.Format( "　【農業事業者】　" );
			lin = 15;
			y += (m_ListPar.lsLinWidth*15);
			break;
		case 15:
			str.Format( "うち税率７．８％適用分" );
			lin = 16;
			y += (m_ListPar.lsLinWidth*16);
			break;
		case 16:
			str.Format( "うち税率６．３％適用分" );
			lin = 17;
			y += (m_ListPar.lsLinWidth*17);
			break;
		case 17:
			str.Format( "第四種事業（その他）" );
			lin = 18;
			y += (m_ListPar.lsLinWidth*18);
			break;
		case 18:
			str.Format( "うち税率７．８％適用分" );
			lin = 19;
			y += (m_ListPar.lsLinWidth*19);
			break;
		case 19:
			str.Format( "うち税率６．２４％適用分" );
			lin = 20;
			y += (m_ListPar.lsLinWidth*20);
			break;
		case 20:
			str.Format( "うち税率６．３％適用分" );
			lin = 21;
			y += (m_ListPar.lsLinWidth*21);
			break;
		case 21:  
			str.Format( " 【固定資産等の譲渡】 " );
			lin = 22;
			y += (m_ListPar.lsLinWidth*22);
			break;
		case 22:
			str.Format( "うち税率７．８％適用分" );
			lin = 23;
			y += (m_ListPar.lsLinWidth*23);
			break;
		case 23:
			str.Format( "うち税率６．２４％適用分" );
			lin = 24;
			y += (m_ListPar.lsLinWidth*24);
			break;
		case 24:
			str.Format( "うち税率６．３％適用分" );
			lin = 25;
			y += (m_ListPar.lsLinWidth*25);
			break;
		case 25:  
			str.Format( "第五種事業（サービス業）" );
			lin = 26;
			y += (m_ListPar.lsLinWidth*26);
			break;
		case 26:
			str.Format( "うち税率７．８％適用分" );
			lin = 27;
			y += (m_ListPar.lsLinWidth*27);
			break;
		case 27:
			str.Format( "うち税率６．２４％適用分" );
			lin = 28;
			y += (m_ListPar.lsLinWidth*28);
			break;
		case 28:
			str.Format( "うち税率６．３％適用分" );
			lin = 29;
			y += (m_ListPar.lsLinWidth*29);
			break;
		case 29:  
			//str.Format( "　【不動産事業者】　" );
			str.Format( "第六種事業（不動産業）" );
			lin = 30;
			y += (m_ListPar.lsLinWidth*30);
			break;
		case 30:
			str.Format( "うち税率７．８％適用分" );
			lin = 31;
			y += (m_ListPar.lsLinWidth*31);
			break;
		case 31:
			str.Format( "うち税率６．２４％適用分" );
			lin = 32;
			y += (m_ListPar.lsLinWidth*32);
			break;
		case 32:
			str.Format( "うち税率６．３％適用分" );
			lin = 33;
			y += (m_ListPar.lsLinWidth*33);
			break;
		case 33:
			str.Format( "　合　計　" );
			lin = 34;
			y += (m_ListPar.lsLinWidth*34);
			break;
		case 34:
			str.Format( "うち税率７．８％適用分" );
			lin = 35;
			y += (m_ListPar.lsLinWidth*35);
			break;
		case 35:
			str.Format( "うち税率６．２４％適用分" );
			lin = 36;
			y += (m_ListPar.lsLinWidth*36);
			break;
		case 36:
			str.Format( "うち税率６．３％適用分" );
			lin = 37;
			y += (m_ListPar.lsLinWidth*37);
			break;
		}


		m_pVprn->DrawString( 0, x, y, str, &sfpar );


		x2 = m_ListPar.lsSx + m_ListPar.lsTrowWidth;
		y2 =  y - m_ListPar.lsLinWidth;

		if( lin ){
			switch( i ){ //枠線・斜線描画
				case 0:		// 第一種
				case 4: 	// 第二種
				case 10: 	// 第三種
				case 17:	// 第四種
				case 25:	// 第五種
				case 29:	// 第六種
				case 33:	// 合計
					m_pVprn->DrawSline(hlist, ltype2, 0, lin, 4, lin);
					break;

				case 8:		// 農業(第２種)
				case 14:	// 農業（第３種）
				case 21:	// 固定
					m_pVprn->DrawSline(  0	, ltype, x, y2, x2 , y2 ); //横
					m_pVprn->DrawSline(hlist, ltype2, 1, lin, 4, lin);
					break;
				default:
					m_pVprn->DrawSline(  0	, ltype, x, y2, x, y ); //縦
					m_pVprn->DrawSline(  0	, ltype, x, y2, x2 , y2 ); //横
					m_pVprn->DrawSline(hlist, ltype2, 1, lin, 4, lin);

					m_pVprn->DrawSline(  0	, ltype, x2 , y, x2 + m_ListPar.lsRowWidth[0], y2 ); //斜線
					m_pVprn->DrawSline(  0	, ltype, x2 + m_ListPar.lsRowWidth[0] , y, x2 + m_ListPar.lsRowWidth[1] + m_ListPar.lsRowWidth[1], y2 ); //斜線
					break;
			}

			int tmp = 0;
			switch( i ){ //固定資産　返還等課税取引斜線
				case 21:
				case 22:
				case 23:
				case 24:
					tmp = m_ListPar.lsRowWidth[0] + m_ListPar.lsRowWidth[1] + m_ListPar.lsRowWidth[2];
					m_pVprn->DrawSline(  0	, ltype, x2 + tmp , y, x2 + tmp + m_ListPar.lsRowWidth[3], y2 ); //斜線
					break;
				default:
					break;
			}
		}
	}
}

//2016.10.20 INSERT START
void CTaxationList31View::GyoshuTitle_31_6syu( int hlist )
{
	SF_PAR	sfpar;
	CString	str;

	long	ltype, x, y;
	long	ltype2, x2, y2;
	int		i, lin;

	for( i = 0; i < 27; i++ ){
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		sfpar.sf_fcsg = SFp_xc | SFp_yc | SFcitv;
		sfpar.sf_yfld = m_ListPar.lsLinWidth;
		sfpar.sf_mrgn[0] = 200;
		sfpar.sf_mrgn[1] = 200;
		sfpar.sf_ksiz = FNT108z;

		str.Empty();

		lin = 0;
		y = m_ListPar.lsSy+m_ListPar.lsTlinWidth;
		ltype = MAKELONG(LKnrW0, 4);
		ltype2 = MAKELONG(LKnrW0, 3); //上辺

		switch( i ){
			case 0:
			case 3:
			case 6:
			case 12:
			case 18:
			case 21:
			case 24:
				x = m_ListPar.lsSx;
				sfpar.sf_xfld = m_ListPar.lsTrowWidth;
				break;
			default:
				x = m_ListPar.lsSx + 500;
				sfpar.sf_xfld = m_ListPar.lsTrowWidth - 500;
				break;
		}



		switch( i ){
		case 0:
			str.Format( "第一種事業（卸売業）" );
			lin = 1;
			y += (m_ListPar.lsLinWidth*1);
			break;
		case 1:
			str.Format( "うち税率６．３％適用分" );
			lin = 2;
			y += (m_ListPar.lsLinWidth*2);
			break;
		case 2:
			str.Format( "うち税率４％適用分" );
			lin = 3;
			y += (m_ListPar.lsLinWidth*3);
			break;
		case 3:
			str.Format( "第二種事業（小売業）" );
			lin = 4;
			y += (m_ListPar.lsLinWidth*4);
			break;
		case 4:
			str.Format( "うち税率６．３％適用分" );
			lin = 5;
			y += (m_ListPar.lsLinWidth*5);
			break;
		case 5:
			str.Format( "うち税率４％適用分" );
			lin = 6;
			y += (m_ListPar.lsLinWidth*6);
			break;
		case 6:
			str.Format( "第三種事業（製造業等）" );
			lin = 7;
			y += (m_ListPar.lsLinWidth*7);
			break;
		case 7:
			str.Format( "うち税率６．３％適用分" );
			lin = 8;
			y += (m_ListPar.lsLinWidth*8);
			break;
		case 8:
			str.Format( "うち税率４％適用分" );
			lin = 9;
			y += (m_ListPar.lsLinWidth*9);
			break;
		case 9:
			str.Format( "　【農業事業者】　" );
			lin = 10;
			y += (m_ListPar.lsLinWidth*10);
			break;
		case 10:
			str.Format( "うち税率６．３％適用分" );
			lin = 11;
			y += (m_ListPar.lsLinWidth*11);
			break;
		case 11:
			str.Format( "うち税率４％適用分" );
			lin = 12;
			y += (m_ListPar.lsLinWidth*12);
			break;
		case 12:
			str.Format( "第四種事業（その他）" );
			lin = 13;
			y += (m_ListPar.lsLinWidth*13);
			break;
		case 13:  
			str.Format( "うち税率６．３％適用分" );
			lin = 14;
			y += (m_ListPar.lsLinWidth*14);
			break;
		case 14:  
			str.Format( "うち税率４％適用分" );
			lin = 15;
			y += (m_ListPar.lsLinWidth*15);
			break;
		case 15:  
			str.Format( " 【固定資産等の譲渡】 " );
			lin = 16;
			y += (m_ListPar.lsLinWidth*16);
			break;
		case 16:  
			str.Format( "うち税率６．３％適用分" );
			lin = 17;
			y += (m_ListPar.lsLinWidth*17);
			break;
		case 17:  
			str.Format( "うち税率４％適用分" );
			lin = 18;
			y += (m_ListPar.lsLinWidth*18);
			break;
		case 18:  
			str.Format( "第五種事業（サービス業）" );
			lin = 19;
			y += (m_ListPar.lsLinWidth*19);
			break;
		case 19:  
			str.Format( "うち税率６．３％適用分" );
			lin = 20;
			y += (m_ListPar.lsLinWidth*20);
			break;
		case 20:  
			str.Format( "うち税率４％適用分" );
			lin = 21;
			y += (m_ListPar.lsLinWidth*21);
			break;
		case 21:  
			str.Format( "第六種事業（不動産業）" );
			lin = 22;
			y += (m_ListPar.lsLinWidth*22);
			break;
		case 22:  
			str.Format( "うち税率６．３％適用分" );
			lin = 23;
			y += (m_ListPar.lsLinWidth*23);
			break;
		case 23:  
			str.Format( "うち税率４％適用分" );
			lin = 24;
			y += (m_ListPar.lsLinWidth*24);
			break;
		case 24:
			str.Format( "　合　計　" );
			lin = 25;
			y += (m_ListPar.lsLinWidth*25);
			break;
		case 25:
			str.Format( "うち税率６．３％適用分" );
			lin = 26;
			y += (m_ListPar.lsLinWidth*26);
			break;
		case 26:
			str.Format( "うち税率４％適用分" );
			lin = 27;
			y += (m_ListPar.lsLinWidth*27);
			break;
		}

		m_pVprn->DrawString( 0, x, y, str, &sfpar );


		x2 = m_ListPar.lsSx + m_ListPar.lsTrowWidth;
		y2 =  y - m_ListPar.lsLinWidth;

		if( lin ){
			switch( i ){ //枠線・斜線描画
				case 0:
				case 3:
				case 6:
				case 12:
				case 18:
				case 21:
				case 24:
					m_pVprn->DrawSline(hlist, ltype2, 0, lin, 4, lin);
					break;
				case 9:
				case 15:
					m_pVprn->DrawSline(  0	, ltype, x, y2, x2 , y2 ); //横
					m_pVprn->DrawSline(hlist, ltype2, 1, lin, 4, lin);
					break;
				default:
					m_pVprn->DrawSline(  0	, ltype, x, y2, x, y ); //縦
					m_pVprn->DrawSline(  0	, ltype, x, y2, x2 , y2 ); //横
					m_pVprn->DrawSline(hlist, ltype2, 1, lin, 4, lin);

					m_pVprn->DrawSline(  0	, ltype, x2 , y, x2 + m_ListPar.lsRowWidth[0], y2 ); //斜線
					m_pVprn->DrawSline(  0	, ltype, x2 + m_ListPar.lsRowWidth[0] , y, x2 + m_ListPar.lsRowWidth[1] + m_ListPar.lsRowWidth[1], y2 ); //斜線
					break;
			}

			int tmp = 0;
			switch( i ){ //固定資産　返還等課税取引斜線
				case 15:
				case 16:
				case 17:
					tmp = m_ListPar.lsRowWidth[0] + m_ListPar.lsRowWidth[1] + m_ListPar.lsRowWidth[2];
					m_pVprn->DrawSline(  0	, ltype, x2 + tmp , y, x2 + tmp + m_ListPar.lsRowWidth[3], y2 ); //斜線
					break;
				default:
					break;
			}
		}
	}
}

//2016.10.20 INSERT END

int CTaxationList31View::PrtKniInpData_31( int hlist )
{
	int		i, j, x, y;
	BYTE	st, flg;
	KKVAL*	pK = &m_pTaxListData->NRec.KUval[40];
	SF_PAR	sfpar;
	char	buf[64];
	char	val[6];

	for( i = flg = 0; i < 10; i++ ){
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		sfpar.sf_fcsg = SFp_xr | SFp_yb | SFcitv;
		sfpar.sf_xfld = m_ListPar.lsTrowWidth;
		sfpar.sf_yfld = m_ListPar.lsLinWidth;
		sfpar.sf_mrgn[0] = 120;
		sfpar.sf_mrgn[1] = 50;
		sfpar.sf_mrgn[3] = 40;
		sfpar.sf_ksiz = FNT084z;
	
		switch( i ){	// y 上から数えて何行目か
			case 0:		y = 1;		break; //第一種
			case 1:		y = 5;		break; //第二種
			case 2:		y = 11;		break; //第三種
			case 3:		y = 15;		break; //第三種　農業事業者
			case 4:		y = 18;		break; //第四種
			case 5:		y = 26;		break; //第五種
			case 6:		y = 30;		break; //第六種
			case 7:		y = 34;		break; //合計
			case 8:		y = 22;		break; //固定資産等の譲渡
			case 9:		y = 9;		break; //第二種　農業事業者
		}

		int loopcnt = 16;//デフォルト

		if (i == 9) { //第二種　農業事業者
			loopcnt = 8;
		}
		else if (i == 3) { //第三種　農業事業者
			loopcnt = 12;
		}

		static long Dispjudgepos1[] = { 0 , 1 , 2 , 3 , 8 , 9 , 10 , 11 };
		static long Dispjudgepos2[] = { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 12 , 13 , 14 , 15 };
		static long Dispjudgeposdef[] = { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 };

		int pos = 0;

		for( j = 0; j < loopcnt; j++ ){

			::ZeroMemory( buf, sizeof( buf ));
			::ZeroMemory( val, sizeof( val ));

			x = (j%4)+1;
			st = 0;

			if (i == 9) { //第二種　農業事業者
				pos = Dispjudgepos1[j];
			}
			else if (i == 3) { //第三種　農業事業者
				pos = Dispjudgepos2[j];
			}
			else {
				pos = Dispjudgeposdef[j];
			}


// 211201_h del -->
			//st = m_pShinKniinp31->DispJudge(pos, i );
// 211201_h del <--
// 211201_h add -->
			if(m_pShinKniinp31Ex != 0)	{
				st = m_pShinKniinp31Ex->DispJudge(pos, i );
			}
			else	{
				st = m_pShinKniinp31->DispJudge(pos, i );
			}
// 211201_h add <--

			switch(pos){
			case 0: //決算額
				memcpy( val, (pK+i)->KKval1, 6 );
				break;
			case 1: //課税取引外
				memcpy( val, (pK+i)->KKval2, 6 );
				break;
			case 2: //課税取引金額
				memcpy( val, (pK+i)->KKval3, 6 );
				break;
			case 3: //返還等課税取引
				memcpy( val, (pK+i)->KKval4, 6 );
				break;
			case 4: //決算額		うち7.8％適用分
				continue;
				break;
			case 5: //課税取引外	うち7.8％適用分
				continue;
				break;
			case 6: //課税取引金額	うち7.8％適用分
				memcpy( val, (pK+i)->KKval5, 6 );
				break;
			case 7: //返還等課税取引　うち7.8％適用分
				memcpy( val, (pK+i)->KKval6, 6 );
				break;
			case 8: //決算額		うち6.24％適用分
				continue;
				break;
			case 9: //課税取引外	うち6.24％適用分
				continue;
				break;
			case 10: //課税取引金額　うち6.24％適用分
				memcpy( val, (pK+i)->KKval7, 6 );
				break;
			case 11: //返還等課税取引　うち6.24％適用分
				memcpy( val, (pK+i)->KKval8, 6 );
				break;
			case 12: //決算額		うち6.3％適用分
				continue;
				break;
			case 13: //課税取引外	うち6.3％適用分
				continue;
				break;
			case 14: //課税取引金額　うち6.3％適用分
				memcpy( val, (pK+i)->KKval9, 6 );
				break;
			case 15: //返還等課税取引　うち6.3％適用分
				memcpy( val, (pK+i)->KKval10, 6 );
				break;
			}

			if( st ){
				m_Arith.l_print( buf, val, "s,sss,sss,sss,ss9" );
			}

			m_pVprn->DrawString( hlist, x, y+(j/4), buf, &sfpar );
		}
	}
	return 0;
}

// 211203h add -->
int CTaxationList31View::Mk_List_31_Ex( int* plist )
{
	long	dotx, doty;
	dotx = doty = 0L;
	doty = 0;
	dotx = 0;

	int i;
	memset( &m_ListPar, '\0', sizeof( LIST_PAR ));

	m_ListPar.lsSx = 980+dotx;
	m_ListPar.lsSy = 4500+doty;
//	m_ListPar.lsLins = 38;
	m_ListPar.lsLins = 29;
	m_ListPar.lsLinWidth = 620;
	m_ListPar.lsTlinWidth = 600;		
	m_ListPar.lsTrowWidth = 5200;	

	m_ListPar.lsRows = 5;
	for( i = 0; i < m_ListPar.lsRows-1; i++ ) {
		m_ListPar.lsRowWidth[i] = 3250;
	}
	m_ListPar.lsLwfpar.lwOtyp  = LKnrW0 | FKrect;
	m_ListPar.lsLwfpar.lwThtyp = LKnrW0;	m_ListPar.lsLwfpar.lwTvtyp = LKnrW0;
	m_ListPar.lsLwfpar.lwDhtyp = 0;		m_ListPar.lsLwfpar.lwDvtyp = LKnrW0;

	if( ( *plist = m_pVprn->MakeList( &m_ListPar ) ) == -1 )		return -1;

	return 0;
}

void CTaxationList31View::GyoshuTitle_31_Ex( int hlist )
{
	SF_PAR	sfpar;
	CString	str;

	long	ltype, x, y;
	long	ltype2, x2, y2;
	int		i, lin;

//	for (i = 0; i < 38; i++) {
	for (i = 0; i < 28; i++) {
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		sfpar.sf_fcsg = SFp_xc | SFp_yc | SFcitv;
		sfpar.sf_yfld = m_ListPar.lsLinWidth;
		sfpar.sf_mrgn[0] = 200;
		sfpar.sf_mrgn[1] = 200;
		sfpar.sf_ksiz = FNT108z;

		str.Empty();

		lin = 0;
		y = m_ListPar.lsSy+m_ListPar.lsTlinWidth;
		ltype = MAKELONG(LKnrW0, 4);
		ltype2 = MAKELONG(LKnrW0, 3); //上辺

		switch( i ){
			case 0:
			case 3:
			case 8:
			case 13:
			case 19:
			case 22:
			case 25:
				x = m_ListPar.lsSx;
				sfpar.sf_xfld = m_ListPar.lsTrowWidth;
				break;
			default:
				x = m_ListPar.lsSx + 500;
				sfpar.sf_xfld = m_ListPar.lsTrowWidth - 500;
				break;
		}


		switch( i ){
		case 0:
			str.Format( "第一種事業（卸売業）" );
			break;
		case 1:
			str.Format( "うち税率７．８％適用分" );
			break;
		case 2:
			str.Format( "うち税率６．２４％適用分" );
			break;
		case 3:
			str.Format( "第二種事業（小売業）" );
			break;
		case 4:
			str.Format( "うち税率７．８％適用分" );
			break;
		case 5:
			str.Format( "うち税率６．２４％適用分" );
			break;
		case 6:
			str.Format("　【農業事業者】　");
			break;
		case 7:
			str.Format("うち税率６．２４％適用分");
			break;
		case 8:
			str.Format( "第三種事業（製造業等）" );
			break;
		case 9:
			str.Format( "うち税率７．８％適用分" );
			break;
		case 10:
			str.Format( "うち税率６．２４％適用分" );
			break;
		case 11:
			str.Format( "　【農業事業者】　" );
			break;
		case 12:
			str.Format( "うち税率７．８％適用分" );
			break;
		case 13:
			str.Format( "第四種事業（その他）" );
			break;
		case 14:
			str.Format( "うち税率７．８％適用分" );
			break;
		case 15:
			str.Format( "うち税率６．２４％適用分" );
			break;
		case 16:
			str.Format( " 【固定資産等の譲渡】 " );
			break;
		case 17:
			str.Format( "うち税率７．８％適用分" );
			break;
		case 18:
			str.Format( "うち税率６．２４％適用分" );
			break;
		case 19:  
			str.Format( "第五種事業（サービス業）" );
			break;
		case 20:
			str.Format( "うち税率７．８％適用分" );
			break;
		case 21:
			str.Format( "うち税率６．２４％適用分" );
			break;
		case 22:  
			str.Format( "第六種事業（不動産業）" );
			break;
		case 23:
			str.Format( "うち税率７．８％適用分" );
			break;
		case 24:
			str.Format( "うち税率６．２４％適用分" );
			break;
		case 25:
			str.Format( "　合　計　" );
			break;
		case 26:
			str.Format( "うち税率７．８％適用分" );
			break;
		case 27:
			str.Format( "うち税率６．２４％適用分" );
			break;
		}
		lin = i + 1;
		y += (m_ListPar.lsLinWidth*lin);

		m_pVprn->DrawString( 0, x, y, str, &sfpar );


		x2 = m_ListPar.lsSx + m_ListPar.lsTrowWidth;
		y2 =  y - m_ListPar.lsLinWidth;

		if( lin ){
			switch( i ){ //枠線・斜線描画
				case 0:		// 第一種
				case 3: 	// 第二種
				case 8: 	// 第三種
				case 13:	// 第四種
				case 19:	// 第五種
				case 22:	// 第六種
				case 25:	// 合計
					m_pVprn->DrawSline(hlist, ltype2, 0, lin, 4, lin);
					break;

				case 6:		// 農業(第２種)
				case 11:	// 農業（第３種）
				case 16:	// 固定
					m_pVprn->DrawSline(  0	, ltype, x, y2, x2 , y2 ); //横
					m_pVprn->DrawSline(hlist, ltype2, 1, lin, 4, lin);
					break;
				default:
					m_pVprn->DrawSline(  0	, ltype, x, y2, x, y ); //縦
					m_pVprn->DrawSline(  0	, ltype, x, y2, x2 , y2 ); //横
					m_pVprn->DrawSline(hlist, ltype2, 1, lin, 4, lin);

					m_pVprn->DrawSline(  0	, ltype, x2 , y, x2 + m_ListPar.lsRowWidth[0], y2 ); //斜線
					m_pVprn->DrawSline(  0	, ltype, x2 + m_ListPar.lsRowWidth[0] , y, x2 + m_ListPar.lsRowWidth[1] + m_ListPar.lsRowWidth[1], y2 ); //斜線
					break;
			}

			int tmp = 0;
			switch( i ){ //固定資産　返還等課税取引斜線
				case 16:
				case 17:
				case 18:
					tmp = m_ListPar.lsRowWidth[0] + m_ListPar.lsRowWidth[1] + m_ListPar.lsRowWidth[2];
					m_pVprn->DrawSline(  0	, ltype, x2 + tmp , y, x2 + tmp + m_ListPar.lsRowWidth[3], y2 ); //斜線
					break;
				default:
					break;
			}
		}
	}
}

int CTaxationList31View::PrtKniInpData_31_Ex( int hlist )
{
	int		i, j, x, y;
	BYTE	st, flg;
	KKVAL*	pK = &m_pTaxListData->NRec.KUval[40];
	SF_PAR	sfpar;
	char	buf[64];
	char	val[6];

	for( i = flg = 0; i < 10; i++ ){
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		sfpar.sf_fcsg = SFp_xr | SFp_yb | SFcitv;
		sfpar.sf_xfld = m_ListPar.lsTrowWidth;
		sfpar.sf_yfld = m_ListPar.lsLinWidth;
		sfpar.sf_mrgn[0] = 120;
		sfpar.sf_mrgn[1] = 50;
		sfpar.sf_mrgn[3] = 40;
		sfpar.sf_ksiz = FNT084z;
	
		switch( i ){	// y 上から数えて何行目か
			case 0:		y = 1;		break; //第一種
			case 1:		y = 4;		break; //第二種
			case 2:		y = 9;		break; //第三種
			case 3:		y = 12;		break; //第三種　農業事業者
			case 4:		y = 14;		break; //第四種
			case 5:		y = 20;		break; //第五種
			case 6:		y = 23;		break; //第六種
			case 7:		y = 26;		break; //合計
			case 8:		y = 17;		break; //固定資産等の譲渡
			case 9:		y = 7;		break; //第二種　農業事業者
		}

		int loopcnt = 12;//デフォルト

		if(i == 9 || i == 3) { // 農業事業者
			loopcnt = 8;
		}

		static long Dispjudgepos1[] = { 0 , 1 , 2 , 3 , 8 , 9 , 10 , 11 };
		//static long Dispjudgepos2[] = { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 12 , 13 , 14 , 15 };
		static long Dispjudgeposdef[] = { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 };

		int pos = 0;

		for( j = 0; j < loopcnt; j++ ){

			::ZeroMemory( buf, sizeof( buf ));
			::ZeroMemory( val, sizeof( val ));

			x = (j%4)+1;
			st = 0;

			if (i == 9) { //第二種　農業事業者
				pos = Dispjudgepos1[j];
			}
			//else if (i == 3) { //第三種　農業事業者
			//	pos = Dispjudgepos2[j];
			//}
			else {
				pos = Dispjudgeposdef[j];
			}

			st = m_pShinKniinp31Ex->DispJudge(pos, i );

			switch(pos){
			case 0: //決算額
				memcpy( val, (pK+i)->KKval1, 6 );
				break;
			case 1: //課税取引外
				memcpy( val, (pK+i)->KKval2, 6 );
				break;
			case 2: //課税取引金額
				memcpy( val, (pK+i)->KKval3, 6 );
				break;
			case 3: //返還等課税取引
				memcpy( val, (pK+i)->KKval4, 6 );
				break;
			case 4: //決算額		うち7.8％適用分
				continue;
				break;
			case 5: //課税取引外	うち7.8％適用分
				continue;
				break;
			case 6: //課税取引金額	うち7.8％適用分
				memcpy( val, (pK+i)->KKval5, 6 );
				break;
			case 7: //返還等課税取引　うち7.8％適用分
				memcpy( val, (pK+i)->KKval6, 6 );
				break;
			case 8: //決算額		うち6.24％適用分
				continue;
				break;
			case 9: //課税取引外	うち6.24％適用分
				continue;
				break;
			case 10: //課税取引金額　うち6.24％適用分
				memcpy( val, (pK+i)->KKval7, 6 );
				break;
			case 11: //返還等課税取引　うち6.24％適用分
				memcpy( val, (pK+i)->KKval8, 6 );
				break;
			//case 12: //決算額		うち6.3％適用分
			//	continue;
			//	break;
			//case 13: //課税取引外	うち6.3％適用分
			//	continue;
			//	break;
			//case 14: //課税取引金額　うち6.3％適用分
			//	memcpy( val, (pK+i)->KKval9, 6 );
			//	break;
			//case 15: //返還等課税取引　うち6.3％適用分
			//	memcpy( val, (pK+i)->KKval10, 6 );
			//	break;
			}

			if( st ){
				m_Arith.l_print( buf, val, "s,sss,sss,sss,ss9" );
			}

			m_pVprn->DrawString( hlist, x, y+(j/4), buf, &sfpar );
		}
	}
	return 0;
}
// 211203h add <--
