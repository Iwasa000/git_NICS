#include "stdafx.h"
#include "TaxationListView.h"

#include "ShinJigyo.h"
#include "ShinNougyou.h"
#include "ShinFudousan.h"
#include "ShinBuy.h"
#include "ShinSell.h"
#include "ShinKniinpDlg.h"

#include "ShinBuyKeika.h"
#include "ShinJigyoKeika.h"//'14.08.11
#include "ShinFudousanKeika.h"//'14.08.11
#include "ShinNougyouKeika.h"//'14.08.11
#include "ShinSellKeika.h"//'14.08.11
#include "ShinKniinpKeika.h"

//2016.10.20 INSERT START
#include "ShinKniinp6syuDlg.h"
#include "ShinKniinpKeika6syu.h"
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
int CTaxationListView::SetPrtPar( CVPrint *pVprn, NPRT_PAR *pPrtPar , SH_PRINT_CMINFO *pPrintCmInfo )
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
int CTaxationListView::PrintSellList()
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
int CTaxationListView::PrintSellList( int pno, int fno, CDC *pDC )
{
	int	st = -1;

	char	*pBp = NULL;
	if( (pBp = new char[Msiz]) == NULL ){
		return st;
	}
	memset( pBp, '\0', (sizeof(char))*Msiz );

	char	*pSvBp = pBp;

	for( int dp=1; dp<=2; ++dp ){
		if( dp == 1 ){
			if( m_pShinSell ){
				m_pShinSell->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinSell->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if( m_pShinSell ){
				st = m_pShinSell->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		if( st ){
			break;
		}
	}

	delete [] pSvBp;

	return st;
}

//-----------------------------------------------------------------------------
// 印刷処理（課税売上高）
//-----------------------------------------------------------------------------
// 引数	pno		：	ページ番号
//		fno		：	
//		pDC		：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CTaxationListView::PrintSellKeikaList( int pno, int fno, CDC *pDC )
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
		if( dp == 1 ){
			if( m_pShinSellKeika ){
				m_pShinSellKeika->SetPrintInfo(m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinSellKeika->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if( m_pShinSellKeika ){
				st = m_pShinSellKeika->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 3 ){
			if( m_pShinSellKeika ){
				st = m_pShinSellKeika->PrintHyo3( pno, dp, pDC, pBp );
			}
		}
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
int CTaxationListView::PrintBuyList( int pno, int fno, CDC *pDC )
{
	int	st = -1;

	char	*pBp = NULL;
	if( (pBp = new char[Msiz]) == NULL ){
		return st;
	}
	memset( pBp, '\0', (sizeof(char))*Msiz );

	char	*pSvBp = pBp;

	for( int dp=1; dp<=2; ++dp ){
		if( dp == 1 ){
			if( m_pShinBuy ){
				m_pShinBuy->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinBuy->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if( m_pShinSell ){
				st = m_pShinBuy->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		if( st ){
			break;
		}
	}

	delete [] pSvBp;

	return st;
}

//-----------------------------------------------------------------------------
// 印刷（課税仕入高）
//-----------------------------------------------------------------------------
// 引数	
// 
// 
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int CTaxationListView::PrintBuyKeikaList( int pno, int fno, CDC *pDC )
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
		if( dp == 1 ){
			if( m_pShinBuyKeika ){
				m_pShinBuyKeika->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinBuyKeika->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if( m_pShinBuyKeika ){
				st = m_pShinBuyKeika->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 3 ){
			if( m_pShinBuyKeika ){
				st = m_pShinBuyKeika->PrintHyo3( pno, dp, pDC, pBp );
			}
		}
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
int CTaxationListView::PrintJigyoList( int pno, int fno, CDC *pDC )
{
	int	st = 0;
	
	char	*pBp = NULL;
	if( (pBp = (char *) malloc( Msiz )) == NULL ){
		return -1;
	}
	m_pBpSv = pBp;
	st = 0;

	for( int dp=1; dp<=2; ++dp ){
		if( dp == 1 ){
			if( m_pShinJigyo ){
				m_pShinJigyo->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinJigyo->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if( m_pShinJigyo ){
				st = m_pShinJigyo->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		if( st ){
			break;
		}
	}

	free( pBp );

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
int CTaxationListView::PrintJigyoKeikaList( int pno, int fno, CDC *pDC )
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
		if( dp == 1 ){
			if( m_pShinJigyoKeika ){
				m_pShinJigyoKeika->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinJigyoKeika->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if( m_pShinJigyoKeika ){
				st = m_pShinJigyoKeika->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 3 ){
			if( m_pShinJigyoKeika ){
				st = m_pShinJigyoKeika->PrintHyo3( pno, dp, pDC, pBp );
			}
		}
		if( st ){
			break;
		}
	}

	free( pBp );

	return st;
}

//-----------------------------------------------------------------------------
// 印刷（課税取引金額計算表 不動産所得）
//-----------------------------------------------------------------------------
// 引数	
// 
// 
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int CTaxationListView::PrintFudousanList( int pno, int fno, CDC *pDC )
{
	int	st = 0;
	
	char	*pBp = NULL;
	if( (pBp = (char *) malloc( Msiz )) == NULL )	return( -1 );
	m_pBpSv = pBp;
	st = 0;

	for( int dp=1; dp<=2; ++dp ){
		if( dp == 1 ){
			if( m_pShinFudousan ){
				m_pShinFudousan->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinFudousan->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if( m_pShinFudousan ){
				st = m_pShinFudousan->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		if( st ){
			break;
		}
	}

	free( pBp );

	return st;
}

//-----------------------------------------------------------------------------
// 印刷（課税取引金額計算表 不動産所得）
//-----------------------------------------------------------------------------
// 引数	
// 
// 
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int CTaxationListView::PrintFudousanKeikaList( int pno, int fno, CDC *pDC )
{
	int	st = 0;
	
	char	*pBp = NULL;
	if( (pBp = (char *) malloc( Msiz )) == NULL )	return( -1 );
	m_pBpSv = pBp;
	st = 0;

//	for( int dp=1; dp<=2; ++dp ){
	for( int dp=1; dp<=3; ++dp ){
		if( dp == 1 ){
			if( m_pShinFudousanKeika ){
				m_pShinFudousanKeika->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinFudousanKeika->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if( m_pShinFudousanKeika ){
				st = m_pShinFudousanKeika->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 3 ){
			if( m_pShinFudousanKeika ){
				st = m_pShinFudousanKeika->PrintHyo3( pno, dp, pDC, pBp );
			}
		}
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
int CTaxationListView::PrintNougyouList( int pno, int fno, CDC *pDC )
{
	int	st = 0;
	
	char	*pBp = NULL;
	if( (pBp = (char *) malloc( Msiz )) == NULL ){
		return -1;
	}
	m_pBpSv = pBp;

	for( int dp=1; dp<=3; ++dp ){
	if( dp == 1 ){
		if( m_pShinNougyou ){
				m_pShinNougyou->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinNougyou->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if( m_pShinNougyou ){
				st = m_pShinNougyou->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 3 ){
			if( m_pShinNougyou ){
				st = m_pShinNougyou->PrintHyo3( pno, dp, pDC, pBp );
			}
		}
		if( st ){
			break;
		}
	}

	free( pBp );

	return( st );
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
int CTaxationListView::PrintNougyouKeikaList( int pno, int fno, CDC *pDC )
{
	int	st = 0;
	
	char	*pBp = NULL;
	if( (pBp = (char *) malloc( Msiz )) == NULL ){
		return -1;
	}
	m_pBpSv = pBp;

//	for( int dp=1; dp<=3; ++dp ){
	for( int dp=1; dp<=4; ++dp ){
	if( dp == 1 ){
		if( m_pShinNougyouKeika ){
				m_pShinNougyouKeika->SetPrintInfo( m_pVprn, m_pPset , m_PrintCmInfo);
				st = m_pShinNougyouKeika->PrintHyo1( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 2 ){
			if( m_pShinNougyouKeika ){
				st = m_pShinNougyouKeika->PrintHyo2( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 3 ){
			if( m_pShinNougyouKeika ){
				st = m_pShinNougyouKeika->PrintHyo3( pno, dp, pDC, pBp );
			}
		}
		else if( dp == 4 ){
			if( m_pShinNougyouKeika ){
				st = m_pShinNougyouKeika->PrintHyo4( pno, dp, pDC, pBp );
			}
		}
		if( st ){
			break;
		}
	}

	free( pBp );

	return( st );
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
int CTaxationListView::PrintKniinpList( int pno, int fno, CDC *pDC )
{
	int	st=0;

	long	dotx, doty;
	dotx = doty = 0L;
	//	単位：1/100mm にする。
//	doty = (dat1 * 10L);
//	dotx = (dat2 * 10L);
	doty = 0;
	dotx = 0;

	int hList = 0;

	Mk_List( &hList );
	Title_Print( hList );
//2016.10.20 INSERT START
	if( m_SixKindFlg == TRUE ){
		GyoshuTitle_6syu( hList );
	}else{
//2016.10.20 INSERT END
	GyoshuTitle( hList );
//2016.10.20 INSERT START
	}
//2016.10.20 INSERT END

	st = PrtKniInpData( hList );

	m_pVprn->DrawLwframe( hList );
	m_pVprn->DeleteList( hList );

	return 0;
}

int CTaxationListView::Mk_List( int* plist )
{
	long	dotx, doty;
	dotx = doty = 0L;
	//	単位：1/100mm にする。
//	doty = (dat1 * 10L);
//	dotx = (dat2 * 10L);
	doty = 0;
	dotx = 0;

	int i;
	memset( &m_ListPar, '\0', sizeof( LIST_PAR ));

	m_ListPar.lsSx = 900+dotx;
	m_ListPar.lsSy = 5500+doty;
//2016.10.20 UPDATE START
//	m_ListPar.lsLins = 13;
	if( m_SixKindFlg == TRUE ){
		m_ListPar.lsLins = 15;
	}else{
		m_ListPar.lsLins = 13;
	}
//2016.10.20 UPDATE END
	m_ListPar.lsLinWidth = 600;
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

void CTaxationListView::Title_Print( int hlist )
{
	SF_PAR	sfpar;
	CString	str;

	long	dotx, doty, x, y;
	dotx = doty = 0L;
	//	単位：1/100mm にする。
//	doty = (dat1 * 10L);
//	dotx = (dat2 * 10L);
	doty = 0;
	dotx = 0;

	int	i;
	for( i = 0; i < 4; i++ ){
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		str.Empty();

		switch( i ){
		case 0:
			sfpar.sf_ksiz = 180;
			sfpar.sf_deco = MAKELONG( LKnrWd, 0);	//アンダーライン
			str = "事業別売上高等の内訳（簡易課税用）";
			x = 4600 + dotx;	y = 2800 + doty;
			break;
		case 1:
			sfpar.sf_ksiz = FNT120z;

//			if( m_PrintCmInfo->OPL )	str.Format( "           %-40.40s %s ", COPNM, DMME );
//			else		str.Format( "%s %-40.40s %s", COPNO, COPNM, DMME );
			x = m_ListPar.lsSx+300;	y = 4500 + doty;
			break;
		case 2:
			sfpar.sf_fcsg = SFp_xl | SFp_yc;
			sfpar.sf_xfld = m_ListPar.lsTrowWidth;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			sfpar.sf_ksiz = FNT108z;
			sfpar.sf_mrgn[0] = 200;
			if( IsJcContinuePrint() && m_pTaxListData->YEAR && (m_pTaxListData->Y_GENGO==0x05) ){
				str.Format("（平成%2x年分）", m_pTaxListData->YEAR+R_TO_H_ADNEN);
			}
			else{
				str.Format("（平成%2x年分）", m_pTaxListData->YEAR);
			}
			x = m_ListPar.lsSx;
			y = m_ListPar.lsSy;
			break;
		case 3:
			sfpar.sf_fcsg = SFp_xr | SFp_yc;
			sfpar.sf_xfld = m_ListPar.lsRowWidth[0]*2;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			sfpar.sf_ksiz = FNT108z;
			if( m_pTaxListData->Head.KHstax&0x01 )		str = "（経理方式：税抜き）";
			else						str = "（経理方式：税込み）";
			x = m_ListPar.lsSx+m_ListPar.lsTrowWidth+(m_ListPar.lsRowWidth[0]*2);
			y = m_ListPar.lsSy;
			break;
		}
		m_pVprn->DrawString( 0, x, y, str, &sfpar );
	}

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
		sfpar.sf_ksiz = FNT084z;
		sfpar.sf_mrgn[1] = 20;
		sfpar.sf_mrgn[2] = 50;
		m_pVprn->DrawString( hlist, i+1, 1, "円", &sfpar );
	}
}

void CTaxationListView::GyoshuTitle( int hlist )
{
	SF_PAR	sfpar;
	CString	str;

	long	ltype, x, y;
	int		i, lin;

	for( i = 0; i < 9; i++ ){
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		sfpar.sf_fcsg = SFp_xc | SFp_yc | SFcitv;
		sfpar.sf_xfld = m_ListPar.lsTrowWidth;
		sfpar.sf_yfld = m_ListPar.lsLinWidth*2;
		sfpar.sf_mrgn[0] = 200;
		sfpar.sf_mrgn[1] = 200;
		sfpar.sf_ksiz = FNT108z;

		str.Empty();

		lin = 0;
		x = m_ListPar.lsSx;
		y = m_ListPar.lsSy+m_ListPar.lsTlinWidth;
		ltype = MAKELONG(LKnrW0, 4);

		switch( i ){
		case 0:
			str.Format( "第一種事業（卸売業）" );
			lin = 2;
			y += (m_ListPar.lsLinWidth*2);
			break;
		case 1:
			str.Format( "第二種事業（小売業）" );
			lin = 4;
			y += (m_ListPar.lsLinWidth*4);
			break;
		case 2:
			sfpar.sf_fcsg = SFp_xc | SFp_yb | SFcitv;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			str.Format( "第三種事業（製造業等）" );
			lin = 6;
			y += (m_ListPar.lsLinWidth*5);
			break;
		case 3:
			sfpar.sf_fcsg = SFp_xc | SFp_yt | SFcitv;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			sfpar.sf_mrgn[2] = 50;
			str.Format( "　【農業事業者】　" );
			y += (m_ListPar.lsLinWidth*6);
			break;
		case 4:
			sfpar.sf_fcsg = SFp_xc | SFp_yb | SFcitv;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			str.Format( "第四種事業（その他）" );
			lin = 8;
			y += (m_ListPar.lsLinWidth*7);
			break;
		case 5:		// 10/04/06 追加
			sfpar.sf_fcsg = SFp_xc | SFp_yt | SFcitv;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			sfpar.sf_mrgn[2] = 50;
			str.Format( " 【固定資産等の譲渡】 " );
			y += (m_ListPar.lsLinWidth*8);
			break;
		case 6:
			sfpar.sf_fcsg = SFp_xc | SFp_yb | SFcitv;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			str.Format( "第五種事業（サービス業）" );
			lin = 10;
			y += (m_ListPar.lsLinWidth*9);
			break;
		case 7:
			sfpar.sf_fcsg = SFp_xc | SFp_yt | SFcitv;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			sfpar.sf_mrgn[2] = 50;
			str.Format( "　【不動産事業者】　" );
			y += (m_ListPar.lsLinWidth*10);
			break;
		case 8:
			str.Format( "　合　計　" );
			y += (m_ListPar.lsLinWidth*12);
			break;
		}

		m_pVprn->DrawString( 0, x, y, str, &sfpar );
		if( lin )
			m_pVprn->DrawSline(hlist, ltype, 0, lin, 4, lin);
	}
}

//2016.10.20 INSERT START
void CTaxationListView::GyoshuTitle_6syu( int hlist )
{
	SF_PAR	sfpar;
	CString	str;

	long	ltype, x, y;
	int		i, lin;

	for( i = 0; i < 10; i++ ){
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		sfpar.sf_fcsg = SFp_xc | SFp_yc | SFcitv;
		sfpar.sf_xfld = m_ListPar.lsTrowWidth;
		sfpar.sf_yfld = m_ListPar.lsLinWidth*2;
		sfpar.sf_mrgn[0] = 200;
		sfpar.sf_mrgn[1] = 200;
		sfpar.sf_ksiz = FNT108z;

		str.Empty();

		lin = 0;
		x = m_ListPar.lsSx;
		y = m_ListPar.lsSy+m_ListPar.lsTlinWidth;
		ltype = MAKELONG(LKnrW0, 4);

		switch( i ){
		case 0:
			str.Format( "第一種事業（卸売業）" );
			lin = 2;
			y += (m_ListPar.lsLinWidth*2);
			break;
		case 1:
			str.Format( "第二種事業（小売業）" );
			lin = 4;
			y += (m_ListPar.lsLinWidth*4);
			break;
		case 2:
			sfpar.sf_fcsg = SFp_xc | SFp_yb | SFcitv;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			str.Format( "第三種事業（製造業等）" );
			lin = 6;
			y += (m_ListPar.lsLinWidth*5);
			break;
		case 3:
			sfpar.sf_fcsg = SFp_xc | SFp_yt | SFcitv;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			sfpar.sf_mrgn[2] = 50;
			str.Format( "　【農業事業者】　" );
			y += (m_ListPar.lsLinWidth*6);
			break;
		case 4:
			sfpar.sf_fcsg = SFp_xc | SFp_yb | SFcitv;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			str.Format( "第四種事業（その他）" );
			lin = 8;
			y += (m_ListPar.lsLinWidth*7);
			break;
		case 5:		// 10/04/06 追加
			sfpar.sf_fcsg = SFp_xc | SFp_yt | SFcitv;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			sfpar.sf_mrgn[2] = 50;
			str.Format( " 【固定資産等の譲渡】 " );
			y += (m_ListPar.lsLinWidth*8);
			break;
		case 6:
			str.Format( "第五種事業（サービス業）" );
			lin = 10;
			y += (m_ListPar.lsLinWidth*10);
			break;
		case 7:
			lin = 12;
			str.Format( "第六種事業（不動産業）" );
			y += (m_ListPar.lsLinWidth*12);
			break;
		case 8:
			str.Format( "　合　計　" );
			y += (m_ListPar.lsLinWidth*14);
			break;
		}

		m_pVprn->DrawString( 0, x, y, str, &sfpar );
		if( lin )
			m_pVprn->DrawSline(hlist, ltype, 0, lin, 4, lin);
	}
}
//2016.10.20 INSERT END

int CTaxationListView::PrtKniInpData( int hlist )
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
				st = m_pShinKniinp6syu->DispJudge( j, i );
			}else{
				st = m_pShinKniinp->DispJudge( j, i );
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
int CTaxationListView::PrintKniinpKeikaList( int pno, int fno, CDC *pDC )
{
	int	st=0;
	long	dotx, doty;
	dotx = doty = 0L;
	//	単位：1/100mm にする。
//	doty = (dat1 * 10L);
//	dotx = (dat2 * 10L);
	doty = 0;
	dotx = 0;

	int hList = 0;

	Mk_List_Keika( &hList );
	Title_Print_Keika( hList );
//2016.10.20 UPDATE START
//	GyoshuTitle_Keika( hList );
	if( m_SixKindFlg == TRUE ){
		GyoshuTitle_Keika_6syu( hList );
	}else{
		GyoshuTitle_Keika( hList );
	}
//2016.10.20 UPDATE END

	st = PrtKniInpData_Keika( hList );

	m_pVprn->DrawLwframe( hList );
	m_pVprn->DeleteList( hList );

	return 0;
}

int CTaxationListView::Mk_List_Keika( int* plist )
{
	long	dotx, doty;
	dotx = doty = 0L;
	//	単位：1/100mm にする。
//	doty = (dat1 * 10L);
//	dotx = (dat2 * 10L);
	doty = 0;
	dotx = 0;

	int i;
	memset( &m_ListPar, '\0', sizeof( LIST_PAR ));

	m_ListPar.lsSx = 900+dotx;
	m_ListPar.lsSy = 5500+doty;
//	m_ListPar.lsLins = 13;
//2016.10.20 UPDATE START
//	m_ListPar.lsLins = 25;
////	m_ListPar.lsLinWidth = 600;
//	m_ListPar.lsLinWidth = 900;
	m_ListPar.lsLins = 28;
	m_ListPar.lsLinWidth = 750;
//2016.10.20 UPDATE END
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

void CTaxationListView::Title_Print_Keika( int hlist )
{
	SF_PAR	sfpar;
	CString	str;

	long	dotx, doty, x, y;
	dotx = doty = 0L;
	//	単位：1/100mm にする。
//	doty = (dat1 * 10L);
//	dotx = (dat2 * 10L);
	doty = 0;
	dotx = 0;

	int	i;
	for( i = 0; i < 4; i++ ){
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		str.Empty();

		switch( i ){
		case 0:
			sfpar.sf_ksiz = 180;
			sfpar.sf_deco = MAKELONG( LKnrWd, 0);	//アンダーライン
			str = "事業別売上高等の内訳（簡易課税用）";
			x = 4600 + dotx;	y = 2800 + doty;
			break;
		case 1:
			sfpar.sf_ksiz = FNT120z;

//			if( m_PrintCmInfo->OPL )	str.Format( "           %-40.40s %s ", COPNM, DMME );
//			else		str.Format( "%s %-40.40s %s", COPNO, COPNM, DMME );
			x = m_ListPar.lsSx+300;	y = 4500 + doty;
			break;
		case 2:
			sfpar.sf_fcsg = SFp_xl | SFp_yc;
			sfpar.sf_xfld = m_ListPar.lsTrowWidth;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			sfpar.sf_ksiz = FNT108z;
			sfpar.sf_mrgn[0] = 200;
			if( IsJcContinuePrint() && m_pTaxListData->YEAR && (m_pTaxListData->Y_GENGO==0x05) ){
				str.Format("（平成%2x年分）", m_pTaxListData->YEAR+R_TO_H_ADNEN);
			}
			else{
				str.Format("（平成%2x年分）", m_pTaxListData->YEAR);
			}
			x = m_ListPar.lsSx;
			y = m_ListPar.lsSy;
			break;
		case 3:
			sfpar.sf_fcsg = SFp_xr | SFp_yc;
			sfpar.sf_xfld = m_ListPar.lsRowWidth[0]*2;
			sfpar.sf_yfld = m_ListPar.lsLinWidth;
			sfpar.sf_ksiz = FNT108z;
			if( m_pTaxListData->Head.KHstax&0x01 )		str = "（経理方式：税抜き）";
			else						str = "（経理方式：税込み）";
			x = m_ListPar.lsSx+m_ListPar.lsTrowWidth+(m_ListPar.lsRowWidth[0]*2);
			y = m_ListPar.lsSy;
			break;
		}
		m_pVprn->DrawString( 0, x, y, str, &sfpar );
	}

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
		sfpar.sf_ksiz = FNT084z;
		sfpar.sf_mrgn[1] = 20;
		sfpar.sf_mrgn[2] = 50;
		m_pVprn->DrawString( hlist, i+1, 1, "円", &sfpar );
	}
}

void CTaxationListView::GyoshuTitle_Keika( int hlist )
{
	SF_PAR	sfpar;
	CString	str;

	long	ltype, x, y;
	long	ltype2, x2, y2;
	int		i, lin;

//	for( i = 0; i < 9; i++ ){
//2016.10.20 UPDATE START
	for( i = 0; i < 27; i++ ){
//2016.10.20 UPDATE END
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		sfpar.sf_fcsg = SFp_xc | SFp_yc | SFcitv;
//		sfpar.sf_xfld = m_ListPar.lsTrowWidth;
//		sfpar.sf_yfld = m_ListPar.lsLinWidth*2;
		sfpar.sf_yfld = m_ListPar.lsLinWidth;
		sfpar.sf_mrgn[0] = 200;
		sfpar.sf_mrgn[1] = 200;
		sfpar.sf_ksiz = FNT108z;

		str.Empty();

		lin = 0;
//		x = m_ListPar.lsSx;
		y = m_ListPar.lsSy+m_ListPar.lsTlinWidth;
		ltype = MAKELONG(LKnrW0, 4);
		ltype2 = MAKELONG(LKnrW0, 3); //上辺

		switch( i ){
			case 0:
			case 3:
			case 6:
//2016.10.20 UPDATE START
			//case 9:
			////case 12:
			//case 15:
			////case 18:
			//case 21:
			case 12:
			case 18:
			case 24:
//2016.10.20 UPDATE END
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
//2016.10.20 UPDATE START
		//case 9:
		//	str.Format( "第四種事業（その他）" );
		//	lin = 10;
		//	y += (m_ListPar.lsLinWidth*10);
		//	break;
		//case 10:
		//	str.Format( "うち税率６．３％適用分" );
		//	lin = 11;
		//	y += (m_ListPar.lsLinWidth*11);
		//	break;
		//case 11:
		//	str.Format( "うち税率４％適用分" );
		//	lin = 12;
		//	y += (m_ListPar.lsLinWidth*12);
		//	break;
		//case 12:
		//	str.Format( " 【固定資産等の譲渡】 " );
		//	lin = 13;
		//	y += (m_ListPar.lsLinWidth*13);
		//	break;
		//case 13:
		//	str.Format( "うち税率６．３％適用分" );
		//	lin = 14;
		//	y += (m_ListPar.lsLinWidth*14);
		//	break;
		//case 14:
		//	str.Format( "うち税率４％適用分" );
		//	lin = 15;
		//	y += (m_ListPar.lsLinWidth*15);
		//	break;
		//case 15:
		//	str.Format( "第五種事業（サービス業）" );
		//	lin = 16;
		//	y += (m_ListPar.lsLinWidth*16);
		//	break;
		//case 16:
		//	str.Format( "うち税率６．３％適用分" );
		//	lin = 17;
		//	y += (m_ListPar.lsLinWidth*17);
		//	break;
		//case 17:
		//	str.Format( "うち税率４％適用分" );
		//	lin = 18;
		//	y += (m_ListPar.lsLinWidth*18);
		//	break;
		//case 18:
		//	str.Format( "　【不動産事業者】　" );
		//	lin = 19;
		//	y += (m_ListPar.lsLinWidth*19);
		//	break;
		//case 19:
		//	str.Format( "うち税率６．３％適用分" );
		//	lin = 20;
		//	y += (m_ListPar.lsLinWidth*20);
		//	break;
		//case 20:
		//	str.Format( "うち税率４％適用分" );
		//	lin = 21;
		//	y += (m_ListPar.lsLinWidth*21);
		//	break;
		//case 21:
		//	str.Format( "　合　計　" );
		//	lin = 22;
		//	y += (m_ListPar.lsLinWidth*22);
		//	break;
		//case 22:
		//	str.Format( "うち税率６．３％適用分" );
		//	lin = 23;
		//	y += (m_ListPar.lsLinWidth*23);
		//	break;
		//case 23:
		//	str.Format( "うち税率４％適用分" );
		//	lin = 24;
		//	y += (m_ListPar.lsLinWidth*24);
		//	break;
		//}

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
			str.Format( "　【不動産事業者】　" );
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
//2016.10.20 UPDATE END

		m_pVprn->DrawString( 0, x, y, str, &sfpar );


		x2 = m_ListPar.lsSx + m_ListPar.lsTrowWidth;
		y2 =  y - m_ListPar.lsLinWidth;

		if( lin ){
			switch( i ){ //枠線・斜線描画
				case 0:
				case 3:
				case 6:
//2016.10.20 UPDATE START
				//case 9:
				//case 15:
				//case 21:
				case 12:
				case 18:
				case 24:
//2016.10.20 UPDATE END
					m_pVprn->DrawSline(hlist, ltype2, 0, lin, 4, lin);
					break;
//2016.10.20 UPDATE START
				//case 12:
				//case 18:
				case 9:
				case 15:
				case 21:
//2016.10.20 UPDATE ENDT
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
//2016.10.20 UPDATE START
				//case 12:
				//case 13:
				//case 14:
				case 15:
				case 16:
				case 17:
//2016.10.20 UPDATE END
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
void CTaxationListView::GyoshuTitle_Keika_6syu( int hlist )
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

int CTaxationListView::PrtKniInpData_Keika( int hlist )
{
	int		i, j, x, y;
	BYTE	st, flg;
//	KKVAL*	pK = &NRec.KUval[40];
	KKVAL*	pK = &m_pTaxListData->NRec.KUval[40];
	SF_PAR	sfpar;
	char	buf[64];
	char	val[6];

	for( i = flg = 0; i < 9; i++ ){
//	for( i = flg = 0; i < 24; i++ ){
		::ZeroMemory( &sfpar, sizeof( SF_PAR ));
		sfpar.sf_fcsg = SFp_xr | SFp_yb | SFcitv;
		sfpar.sf_xfld = m_ListPar.lsTrowWidth;
		sfpar.sf_yfld = m_ListPar.lsLinWidth;
		sfpar.sf_mrgn[0] = 120;
		sfpar.sf_mrgn[1] = 50;
		sfpar.sf_mrgn[3] = 40;
		sfpar.sf_ksiz = FNT108z;

/*		switch( i ){
		case 0:		x = 2;	break;
		case 1:		x = 4;	break;
		case 2:		x = 5;	break;
		case 3:		x = 6;	break;
		case 4:		x = 7;	break;
		case 5:		x = 9;	break;
		case 6:		x = 10;	break;
		case 7:		x = 12;	break;
		case 8:		x = 8;	break;		// 10/04/06 追加
		}*/
	
		switch( i ){
		case 0:		y = 1;		break; //第一種
		case 1:		y = 4;		break; //第二種
		case 2:		y = 7;		break; //第三種
//2016.10.20 UPDATE START
		//case 3:		continue;	break; //農業事業者
		//case 4:		y = 10;		break; //第四種
		//case 5:		y = 16;		break; //第五種
		//case 6:		y = 19;		break; //不動産事業者
		//case 7:		y = 22;		break; //合計
		//case 8:		y = 13;		break; //固定資産等の譲渡

		case 3:		y = 10;		break; //農業事業者
		case 4:		y = 13;		break; //第四種
		case 5:		y = 19;		break; //第五種
		case 6:		y = 22;		break; //不動産事業者
		case 7:		y = 25;		break; //合計
		case 8:		y = 16;		break; //固定資産等の譲渡
//2016.10.20 UPDATE END
		}


//		for( j = 0; j < 4; j++ ){
//		for( j = 0; j < 8; j++ ){
//		for( j = 0; j < JSKWIDECNT; j++ ){
		for( j = 0; j < 12; j++ ){

			::ZeroMemory( buf, sizeof( buf ));
			::ZeroMemory( val, sizeof( val ));
			//x = j+1;
			x = (j%4)+1;
			st = 0;

/*			switch( j){
			case 0: //決算額
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
			case 1: //課税取引外
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
			case 2: //課税取引金額
				if( i == 7 ){
					if( flg&0x03 )	st = 1;
				}
				else{
					if( l_test( (pK+i)->KKval1 ) || l_test( (pK+i)->KKval2 ))	st = 1;
				}
				memcpy( val, (pK+i)->KKval3, 6 );
				break;
			case 3: //返還等課税取引
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
			case 4: //課税取引金額　うち６．３％適用分
				x = 3;
				y += 1;
				if( l_test( (pK+i)->KKval5 )){
					st = 1;
				}
				memcpy( val, (pK+i)->KKval5, 6 );
				break;
			case 5: //返還等課税取引　うち６．３％適用分
				x = 4;
				if( l_test( (pK+i)->KKval6 )){
					st = 1;
				}
				memcpy( val, (pK+i)->KKval6, 6 );
				break;
			case 6: //課税取引金額　うち４％適用分
				x = 3;
				y += 1;
				if( l_test( (pK+i)->KKval7 )){
					st = 1;
				}
				memcpy( val, (pK+i)->KKval7, 6 );
				break;
			case 7: //返還等課税取引　うち４％適用分
				x = 4;
				if( l_test( (pK+i)->KKval8 )){
					st = 1;
				}
				memcpy( val, (pK+i)->KKval8, 6 );
				break;
			}*/
//2016.10.20 UPDATE START
			//st = m_pShinKniinpKeika->DispJudge( j, i );

			if( m_SixKindFlg == TRUE ){
				st = m_pShinKniinpKeika6syu->DispJudge( j, i );
			}else{
				st = m_pShinKniinpKeika->DispJudge( j, i );
			}
//2016.10.20 UPDATE END

			switch( j ){
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
			case 4: //決算額		うち６．３％適用分
				continue;
				break;
			case 5: //課税取引外	うち６．３％適用分
				continue;
				break;
			case 6: //課税取引金額	うち６．３％適用分
				memcpy( val, (pK+i)->KKval5, 6 );
				break;
			case 7: //返還等課税取引　うち６．３％適用分
				memcpy( val, (pK+i)->KKval6, 6 );
				break;
			case 8: //決算額		うち４％適用分
				continue;
				break;
			case 9: //課税取引外	うち４％適用分
				continue;
				break;
			case 10: //課税取引金額　うち４％適用分
				memcpy( val, (pK+i)->KKval7, 6 );
				break;
			case 11: //返還等課税取引　うち４％適用分
				memcpy( val, (pK+i)->KKval8, 6 );
				break;
			}

			if( st )
//				l_print( buf, val, "s,sss,sss,sss,ss9" );
				m_Arith.l_print( buf, val, "s,sss,sss,sss,ss9" );

			m_pVprn->DrawString( hlist, x, y+(j/4), buf, &sfpar );
		}
	}
	return 0;
}