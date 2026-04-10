// DataListLine.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "MemDC.h"

#include "DataListCtrl.h"

#include "DtClmItem.h"
#include "DataListLine.h"

#include "resource.h"

#include "DBDinpRec.h"
#include "Input1.h"
#include "external.h"

#include "AutoInp.h"
extern struct _AUTOSEL *pAUTOSEL;

#include "mstrw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDataListLine

// 仕訳パターン１
DT_CLMITEM LN1[] = {
	{ 0, "", DATA_SEQ,	0, "", -1,-1,	7, 7,	0,	0, 0, -1 },
	{ 0, "", DATA_DATE,	0, "", -1,-1,	7, 7,	0, -1, 0, -1 },
	{ 2, "", DATA_DENP,	0, "", -1,-1,	7, 7,	0,  0, 0,  0 },

	{ 0, "", DATA_DEB_BMON,	0, "", -1,-1, 12, 12, 0, -1, 0,  1 },
	{ 2, "", DATA_DEB_KNO,	0, "", -1,-1, 12, 12, 0, -1, 0,  2 },

	{ 0, "", DATA_DEBT,	0, "", -1,-1,	0,	0,	0, -1, 0,  3 },
	{ 0, "", DATA_CRED,	0, "", -1,-1,	0,	0,	0, -1, 0, -1 },
	{ 0, "", DATA_VAL,	0, "", -1,-1,	15,	0,	0, -1, 0, -1 },
	{ 0, "", DATA_ZEI,	0, "", -1,-1,	15,	0,	0, -1, 0, -1 },
	{ 0, "", DATA_SYZ,	0, "", -1,-1,	8,	8,	0, -1, 0, -1 },
	{ 0, "", DATA_HJIYU,DT_INP_BOTTOMRIGHT, "", -1,-1,	40,	4,	0, -1, 0, -1 },

	{ 2, "", DATA_TEKIYO,	0, "", -1,-1,	80,	80,	4, -1, 0,  4 },

#ifdef CLOSE	// 手形関係のプロがまだないため
	{ 0, "", DATA_SYONO,	0, "", -1,-1,	10,	10,	0, -1, 0, -1 },
	{ 2, "", DATA_TGDATE,	0, "", -1,-1,	10,	10,	0, -1, 0, 11 },
#endif

	{ 0, "", DATA_HUSEN,	0, "", -1,-1,	6,	6,	0, -1, 0, 10 },
};

// 証憑番号対応
//	仕訳パターン１
DT_CLMITEM snoLN1[] = {
	{ 0, "", DATA_SEQ,	0, "", -1,-1,	7, 7,	0,	0, 0, -1 },
	{ 0, "", DATA_DATE,	0, "", -1,-1,	8, 8,	0, -1, 0, -1 },
	{ 2, "", DATA_DENP,	0, "", -1,-1,	7, 7,	0,  0, 0, -1 },

	{ 2, "", DATA_DOCEVI, 0, "", -1,-1,	8, 8,	0,  0, 0, 2 },

	{ 0, "", DATA_DEB_BMON,	0, "", -1,-1, 12, 12, 0, -1, 0,  1 },
	{ 2, "", DATA_DEB_KNO,	0, "", -1,-1, 12, 12, 0, -1, 0,  3 },

	{ 0, "", DATA_DEBT,	0, "", -1,-1,	0,	0,	0, -1, 0,  4 },
	{ 0, "", DATA_CRED,	0, "", -1,-1,	0,	0,	0, -1, 0, -1 },
	{ 0, "", DATA_VAL,	0, "", -1,-1,	15,	0,	0, -1, 0, -1 },
	{ 0, "", DATA_ZEI,	0, "", -1,-1,	15,	0,	0, -1, 0, -1 },
	{ 0, "", DATA_SYZ,	0, "", -1,-1,	8,	8,	0, -1, 0, -1 },
	{ 0, "", DATA_HJIYU,DT_INP_BOTTOMRIGHT, "", -1,-1,	40,	4,	0, -1, 0, -1 },

	{ 2, "", DATA_TEKIYO,	0, "", -1,-1,	80,	80,	4, -1, 0,  5 },

#ifdef CLOSE	// 手形関係のプロがまだないため
	{ 0, "", DATA_SYONO,	0, "", -1,-1,	10,	10,	0, -1, 0, -1 },
	{ 2, "", DATA_TGDATE,	0, "", -1,-1,	10,	10,	0, -1, 0, 11 },
#endif

	{ 0, "", DATA_HUSEN,	0, "", -1,-1,	6,	6,	0, -1, 0, 11 },
};

// 仕訳パターン２ タイトル部分
DT_CLMITEM LN2_t[] = {
	{ 0, "", DATA_SEQ,	0, "", -1,-1,	8,	8,  0,  0, 0, -1 },
	{ 0, "", DATA_DATE,	0, "", -1,-1,	7,	7,  0, -1, 0, -1 },
	{ 2, "", DATA_DENP,	0, "", -1,-1,	7,	7,  0,	0, 0, 0 },

	{ 0, "", DATA_DEB_BMON,	0, "", -1,-1,	8,	8,  0, -1, 0,  2 },
	{ 0, "", DATA_DEB_KNO,	0, "", -1,-1,  13, 13,  0, -1, 0, -1 },
	{ 2, "", DATA_DEBT,		0, "", -1,-1,14+7,14+7, 1, -1, 0,  1 },

	{ 0, "", DATA_CRE_BMON,	0, "", -1,-1,   8,	8,  0, -1, 0,  4 },
	{ 0, "", DATA_CRE_KNO,	0, "", -1,-1,  13, 13,  0, -1, 0, -1 },
	{ 2, "", DATA_CRED,   	0, "", -1,-1,14+7,14+7, 1, -1, 0,  5 },

	{ 0, "", DATA_VAL,	0, "", -1,-1,	15,	0,  0, -1, 0,  7 },
	{ 0, "", DATA_ZEI,	0, "", -1,-1,	15,	0,  0, -1, 0, -1 },
	{ 0, "", DATA_SYZ,	0, "", -1,-1,	8,	8,  0, -1, 0, -1 },
	{ 0, "変動事由", DATA_HJIYU,DT_INP_BOTTOMRIGHT, "", -1,-1,	40,	4,	0, -1, 0, -1 },

	{ 4, "", DATA_TEKIYO,	0, "", -1,-1,	8,	8,  0,	0, 0, -1 },

	{ 3, "", DATA_SYONO,	0, "", -1,-1,	10,	10,  0, 88, 1, -1 },
	{ 3, "", DATA_TGDATE,	0, "", -1,-1,	10,	10,  0, -1, 0, -1 },

	{ 3, "", DATA_HUSEN,	0, "", -1,-1,	6,	6,  0, -1, 0, -1 },
};

// 仕訳パターン２ データ部分
DT_CLMITEM LN2_d[] = {
	{ 1, "", DATA_SEQ,	0, "", -1,-1,		8,	8,  0,  0, 0, -1 },
	{ 1, "", DATA_DATE,	0, "", -1,-1,		7,	7,  0, -1, 0, -1 },
	{ 3, "", DATA_DENP,	0, "", -1,-1,		7,	7,  0,	0, 0,  0 },

	{ 1, "", DATA_DEB_BMON,	0, "", -1,-1,	8,	8,  0, -1, 0,  1 },
	{ 1, "", DATA_DEB_KNO,	0, "", -1,-1,	13,13,  0, -1, 0, -1 },
	{ 3, "", DATA_DEBT,		0, "", -1,-1,14+7,14+7, 1, -1, 0,  2 },

	{ 1, "", DATA_CRE_BMON,	0, "", -1,-1,	8,	8, 	0, -1, 0,  4 },
	{ 1, "", DATA_CRE_KNO,	0, "", -1,-1,	13,13, 	0, -1, 0, -1 },
	{ 3, "", DATA_CRED, 	0, "", -1,-1,14+7,14+7, 1, -1, 0,  5 },

	{ 1, "", DATA_VAL,	0, "", -1,-1,		15,	0, 	0, -1, 0,  7 },
	{ 1, "", DATA_ZEI,	0, "", -1,-1,		15,	0, 	0, -1, 0, -1 },
	{ 1, "", DATA_SYZ,	0, "", -1,-1,		8,	8, 	0, -1, 0, -1 },
	{ 1, "", DATA_HJIYU,DT_INP_BOTTOMRIGHT, "", -1,-1,		40,	4,	0, -1, 0, -1 },

	{ 4, "", DATA_TEKIYO,	0, "", -1,-1,	80,	0, 	0,	8, 0, -1 },
	{ 4, "", DATA_SYONO,	0, "", -1,-1,	20,	10, 	0, -1, 0, -1 },
	{ 4, "", DATA_TGDATE,	0, "", -1,-1,	10,	10, 	0, -1, 0, -1 },

	{ 4, "", DATA_HUSEN,	0, "", -1,-1,	6,	6, 	0, -1, 0, -1 },
};



// 仕訳パターン２[証憑あり] タイトル部分
DT_CLMITEM snoLN2_t[] = {
	{ 0, "", DATA_SEQ,		0, "", -1,-1,	8,	8,  0,  0, 0, -1 },
	{ 0, "", DATA_DATE,		0, "", -1,-1,	8,	8,  0, -1, 0, -1 },

	{ 2, "", DATA_DENP,		0, "", -1,-1,	8,	8,  0,  0, 0, -1 },
	{ 2, "", DATA_DOCEVI,	0, "", -1,-1,	8,	8,  0,	0, 0, 0 },

	{ 0, "", DATA_DEB_BMON,	0, "", -1,-1,	8,	8,  0, -1, 0,  1 },
	{ 0, "", DATA_DEB_KNO,	0, "", -1,-1,  13, 13,  0, -1, 0, -1 },
	{ 2, "", DATA_DEBT,		0, "", -1,-1,14+7,14+7, 1, -1, 0,  3 },

	{ 0, "", DATA_CRE_BMON,	0, "", -1,-1,   8,	8,  0, -1, 0,  5 },
	{ 0, "", DATA_CRE_KNO,	0, "", -1,-1,  13, 13,  0, -1, 0, -1 },
	{ 2, "", DATA_CRED,   	0, "", -1,-1,14+7,14+7, 1, -1, 0,  6 },

	{ 0, "", DATA_VAL,	0, "", -1,-1,	15,	0,  0, -1, 0,  8 },
	{ 0, "", DATA_ZEI,	0, "", -1,-1,	15,	0,  0, -1, 0, -1 },
	{ 0, "", DATA_SYZ,	0, "", -1,-1,	8,	8,  0, -1, 0, -1 },
	{ 0, "変動事由", DATA_HJIYU,DT_INP_BOTTOMRIGHT, "", -1,-1,	40,	4,	0, -1, 0, -1 },

	{ 4, "", DATA_TEKIYO,	0, "", -1,-1,	8,	8,  0,	0, 0, -1 },

	{ 3, "", DATA_SYONO,	0, "", -1,-1,	10,	10,  0, 88, 1, -1 },
	{ 3, "", DATA_TGDATE,	0, "", -1,-1,	10,	10,  0, -1, 0, -1 },

	{ 3, "", DATA_HUSEN,	0, "", -1,-1,	6,	6,  0, -1, 0, -1 },
};

// 仕訳パターン２[証憑あり] データ部分
DT_CLMITEM snoLN2_d[] = {
	{ 1, "", DATA_SEQ,		0, "", -1,-1,	8,	8,  0,  0, 0, -1 },
	{ 1, "", DATA_DATE,		0, "", -1,-1,	8,	8,  0, -1, 0, -1 },

	{ 3, "", DATA_DENP,		0, "", -1,-1,	8,	8,  0,  0, 0, -1 },
	{ 3, "", DATA_DOCEVI,	0, "", -1,-1,	8,	8,  0,	0, 0,  0 },

	{ 1, "", DATA_DEB_BMON,	0, "", -1,-1,	8,	8,  0, -1, 0,  1 },
	{ 1, "", DATA_DEB_KNO,	0, "", -1,-1,	13,13,  0, -1, 0, -1 },
	{ 3, "", DATA_DEBT,		0, "", -1,-1,14+7,14+7, 1, -1, 0,  3 },

	{ 1, "", DATA_CRE_BMON,	0, "", -1,-1,	8,	8, 	0, -1, 0,  5 },
	{ 1, "", DATA_CRE_KNO,	0, "", -1,-1,	13,13, 	0, -1, 0, -1 },
	{ 3, "", DATA_CRED, 	0, "", -1,-1,14+7,14+7, 1, -1, 0,  6 },

	{ 1, "", DATA_VAL,	0, "", -1,-1,		15,	0, 	0, -1, 0,  8 },
	{ 1, "", DATA_ZEI,	0, "", -1,-1,		15,	0, 	0, -1, 0, -1 },
	{ 1, "", DATA_SYZ,	0, "", -1,-1,		8,	8, 	0, -1, 0, -1 },
	{ 1, "", DATA_HJIYU,DT_INP_BOTTOMRIGHT, "", -1,-1,		40,	4,	0, -1, 0, -1 },

	{ 4, "", DATA_TEKIYO,	0, "", -1,-1,	80,	0, 		0,	8, 0, -1 },
	{ 4, "", DATA_SYONO,	0, "", -1,-1,	20,	10, 	0, -1, 0, -1 },
	{ 4, "", DATA_TGDATE,	0, "", -1,-1,	10,	10, 	0, -1, 0, -1 },

	{ 4, "", DATA_HUSEN,	0, "", -1,-1,	6,	6, 	0, -1, 0, -1 },
};





CDataListLine::CDataListLine()
{
	EnableTitleTips( FALSE );
	SetColumnResize( FALSE );

//	m_crTitleBkColor = RGB_BLUE_01;
	m_dwStyle |= DS_FRAME3D;
	m_dwStyle &= ~DS_BK_DEBCRE;
	m_dwStyle &= ~DS_HUSEN_SEQ;

	//部門・工事が借／貸で異なる場合の表示が
	//おかしくなるため
	m_Lines = 2;
}

CDataListLine::CDataListLine( int point )
			: CDataListCtrl( point )
{
	EnableTitleTips( FALSE );
	SetColumnResize( FALSE );

//	m_crTitleBkColor = RGB_BLUE_01;
	m_dwStyle |= DS_FRAME3D;
	m_dwStyle &= ~DS_BK_DEBCRE;
	m_dwStyle &= ~DS_HUSEN_SEQ;

	//部門・工事が借／貸で異なる場合の表示が
	//おかしくなるため
	m_Lines = 2;
}

CDataListLine::~CDataListLine()
{
}

/*
BEGIN_MESSAGE_MAP(CDataListLine, CDataListCtrl)
	//{{AFX_MSG_MAP(CDataListLine)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CDataListLine メッセージ ハンドラ 

void CDataListLine::OnDraw(CDC* pDC)
{
	int saveDC = pDC->SaveDC();

	pDC->SetBkMode(TRANSPARENT);
//	if( m_bEraseBkgnd ) {
		pDC->FillSolidRect( m_rectCtrl, m_crBackground );
//	}
	
	if( m_RectArray.GetSize() == 0 ) {
		pDC->RestoreDC( saveDC );
		return;
	}

	CRect rect;
	int nid, i;
	DT_COLUMN dtc;

	for( i = 0; i < m_RectArray[0].Dtcnt; i++ ) {
		nid = m_RectArray[0].DtID[i];
		rect = m_RectArray[0].DtRect[i];
		rect.left++;

		pDC->FillSolidRect( rect, m_crTitleBkColor );
		if( GetItemColumn( nid, &dtc ) ) {
			if( dtc.clm_back != (DWORD)-1 ) {
				pDC->FillSolidRect( rect, dtc.clm_back );
			}
		}
	}
//	pDC->SetBkColor( m_crTitleBkColor );

	int n, right_pos = 0;
	if( (m_RectArray[0].LineRect.right+leftMargin) < m_rectCtrl.right ) {
		rect.left = m_RectArray[0].LineRect.right+leftMargin;
		rect.right = m_rectCtrl.right;
		rect.top = m_rectCtrl.top;
		rect.bottom = m_rectCtrl.bottom;	
		pDC->FillSolidRect( rect, m_crBackground );

		right_pos = m_RectArray[0].LineRect.right+leftMargin+1;
	}

#ifdef CLOSE
	CPen pen1(PS_SOLID, 2, RGB(0,0,0) );
	pDC->SelectObject( &pen1 );

	rect = m_rectCtrl;
	rect.left++;
	rect.bottom--;
	if( right_pos )	rect.right = right_pos;
	rect.right--;
	pDC->MoveTo( rect.top, rect.left );
	pDC->LineTo( rect.right, rect.top );
	pDC->LineTo( rect.right, rect.bottom );
	pDC->LineTo( rect.left, rect.bottom );
	pDC->LineTo( rect.left, rect.top );
#endif
	rect = m_rectCtrl;
	if( right_pos )	rect.right = right_pos;

//	pDC->DrawEdge( rect, EDGE_ETCHED , BF_RECT );

	CPen pen2( PS_SOLID, 1, RGB_BLACK );
	CPen penwk( PS_SOLID, 1, RGB_DARKER_GRAY );
	pDC->SelectObject( &pen2 );
	DT_RECT  dtrect;
	dtrect = m_RectArray.GetAt( 0 );

	rect = m_rectCtrl;
	rect.DeflateRect( topMargin, leftMargin );

	// タイトル部分
	for( i = 0; i < dtrect.Dtcnt; i++ ) {
		CRect& rc = dtrect.DtRect[i];
		// 上の線
		if( 0 /*dtrect.DtID[i] == DATA_NONE*/ ) {
			if( rc.right > rect.right )
				rc.right = rect.right;

			if( rect.top < rc.top ) {
				pDC->MoveTo( rc.left, rc.top );
				pDC->LineTo( rc.right, rc.top );
			}
			else {
				pDC->MoveTo( rc.right, rc.top );
			}
			pDC->LineTo( rc.right, rc.bottom );
			pDC->LineTo( rc.left, rc.bottom );

			if( rect.left < rc.left ) {
				pDC->LineTo( rc.left, rc.top );
			}
		}
		else {
			if( 0 /*m_dwStyle & DS_FRAME3D*/ ) {
				CPen penHigh( PS_SOLID, 1, ::GetSysColor( COLOR_BTNHIGHLIGHT ) );
				CPen penShadow( PS_SOLID, 1, ::GetSysColor( COLOR_3DDKSHADOW ) );

				if( rc.right > rect.right )
					rc.right = rect.right;

				pDC->SelectObject( &penHigh );

				pDC->MoveTo( rc.left+1, rc.bottom-1 );
				pDC->LineTo( rc.left+1, rc.top );
				pDC->LineTo( rc.right, rc.top );
				
				pDC->SelectObject( &penShadow );
			
				pDC->LineTo( rc.right, rc.bottom-1 );
				pDC->LineTo( rc.left+1, rc.bottom-1 );		
			}
			else {
				CPen pen_twk( PS_SOLID, 1, RGB_PAPER_WHITE );
				pDC->SelectObject( &pen_twk );

				if( rc.right > rect.right )
					rc.right = rect.right;

				pDC->MoveTo( rc.left, rc.bottom );
				pDC->LineTo( rc.left, rc.top );
				pDC->LineTo( rc.right, rc.top );
				pDC->LineTo( rc.right, rc.bottom );
				pDC->LineTo( rc.left, rc.bottom );
			}
		}

		nid = dtrect.DtID[i];

		pDC->SetBkColor( m_crTitleBkColor );
		if( GetItemColumn( nid, &dtc ) ) {
			if( dtc.clm_back != (DWORD)-1 ) {
				pDC->SetBkColor( dtc.clm_back );
			}
		}

		Dt_DrawTitleText( pDC, dtrect, i );
	}

	BOOL bFocused = FALSE;
	bFocused = IsFocused();
	CWnd* pwnd = GetFocus();
//TRACE( "bFocused %d, %d, %x IsChild %d\n", bFocused, m_bInputBoxDisp, pwnd->m_hWnd, IsChild( pwnd ) );

	for( i = 0; i < m_LineCount; i++ ) {
		dtrect = m_RectArray.GetAt( i+1 );
		COLORREF back;

		for( n = 0; n < dtrect.Dtcnt; n++ ) {
			CRect& rc = dtrect.DtRect[n];
			if( dtrect.DtID[n] == DATA_NONE )	back = m_crBackground;
			else {
				if( bFocused ) {
					back = ((i+1) == m_CurrentLine) ? m_crCurBkColor : m_crDataBkColor[0];
				}
				else {
					back = m_crDataBkColor[0];
				}
			}
			pDC->FillSolidRect( rc, back );
			pDC->SetTextColor( m_crDataTextColor[0] );
		}

		Dt_DrawData( pDC, dtrect, i, TRUE );
//		DrawWaku( pDC, dtrect );	
		for( n = 0; n < dtrect.Dtcnt; n++ ) {
			if( m_dwStyle & DS_FRAME3D ) {
				CPen penHigh( PS_SOLID, 1, ::GetSysColor( COLOR_BTNHIGHLIGHT ) );
				CPen penShadow( PS_SOLID, 1, ::GetSysColor( COLOR_3DDKSHADOW ) );

				CRect& rc = dtrect.DtRect[n ];
				if( rc.right > rect.right )
					rc.right = rect.right;

				pDC->SelectObject( &penShadow );

				pDC->MoveTo( rc.left+1, rc.bottom-1 );
				pDC->LineTo( rc.left+1, rc.top );
				pDC->LineTo( rc.right, rc.top );

				pDC->SelectObject( &penHigh );
				
				pDC->LineTo( rc.right, rc.bottom-1 );
				pDC->LineTo( rc.left+1, rc.bottom-1 );
			}
			else {
				pDC->SelectObject( &penwk );

				CRect& rc = dtrect.DtRect[n ];
				if( rc.right > rect.right )
					rc.right = rect.right;

				pDC->MoveTo( rc.left, rc.bottom-1 );
				pDC->LineTo( rc.left, rc.top );
				pDC->LineTo( rc.right, rc.top );
				pDC->LineTo( rc.right, rc.bottom-1 );
				pDC->LineTo( rc.left, rc.bottom-1 );
			}
		}
	}

#ifdef _XX_DEBUG
    int log_sy = pDC->GetDeviceCaps( LOGPIXELSY );
	LOGFONT lf;
	m_pFont->GetLogFont( &lf );
	
	double tmp = ( abs(lf.lfHeight) * 72);
	double point = tmp / (double)log_sy;

	CString str;
	str.Format( "point = %f", point );
	pDC->TextOut( 3, m_rectCtrl.bottom-20, str );
#endif

	pDC->RestoreDC( saveDC );

}


// カラムアイテムの配列を作成する。
int CDataListLine::ClmItemArrayMake( int displine )
{
	DT_CLMITEM *pMk;
	m_clmItem.RemoveAll();
	int max, i;

	if( !(m_dwStyle & DS_BK_DEBCRE) ) {	//部門・工事が １仕訳につき１つずつ
		if( bDOCEVI_Master ) {
			max = sizeof( snoLN1 ) / sizeof( snoLN1[0] );
			pMk = snoLN1;
		}
		else {
			max = sizeof( LN1 ) / sizeof( LN1[0] );
			pMk = LN1;
		}
	}
	else {		//借方／貸方の両方に 部門・工事がある。
		if( displine == 0 ) {
			if( bDOCEVI_Master ) {
				max = sizeof( snoLN2_t ) / sizeof( snoLN2_t[0] );
				pMk = snoLN2_t;
			}
			else {
				max = sizeof( LN2_t ) / sizeof( LN2_t[0] );
				pMk = LN2_t;
			}
		}
		else {
			if( bDOCEVI_Master ) {
				max = sizeof( snoLN2_d ) / sizeof( snoLN2_d[0] );
				pMk = snoLN2_d;
			}
			else {
				max = sizeof( LN2_d ) / sizeof( LN2_d[0] );
				pMk = LN2_d;
			}
		}
	}

	int h_idx = -1;
	int idx = 0;

	for( i = 0; i < max; i++ ) {
		if( pAUTOSEL->DSPZEI_OPT == OPT_NODSP ) {
			if( (pMk+i)->id == DATA_ZEI ) {
				continue;
			}
		}
		if (pAUTOSEL->DSPSYZ_OPT == OPT_NODSP) {
			if ((pMk + i)->id == DATA_SYZ) {
				continue;
			}
		}

		//変動事由の index
		if( (pMk+i)->id == DATA_HJIYU ) {
			h_idx = idx;
		}
		m_clmItem.Add( *(pMk+i ) );
		idx++;
	}

	if( !(m_dwStyle & DS_BK_DEBCRE) ) {	//部門・工事が １仕訳につき１つずつ
		for( i = 0; i < m_clmItem.GetCount(); i++ ) {

			if( m_clmItem[i].id == DATA_HUSEN ) {
				m_clmItem[i].clm_sidx = h_idx;
			}
			else if( m_clmItem[i].id == DATA_HJIYU ) {
				if( pAUTOSEL->DSPZEI_OPT == OPT_NODSP ) {
					m_clmItem[i].dispsize	= 8;
				}
				else {
					m_clmItem[i].dispsize	= 4;
				}
			}
		}
	}

	return m_clmItem.GetCount();
}


int CDataListLine::MakeDataRect( CDC* pDC, int displine )
{
	CSize sz;
	CRect rc(0,0,0,0), rectbuf;
	DT_RECT  dtrect;
	int i, n, end_sw = 0, basic_top, lnofs, max, dtid;
	int	title = 0;
	short	line_pos;

	dtrect.Dtcnt = 0;
	line_pos = displine;

	DT_CLMITEM *pMk;

	//クラウド共有のENDで仕訳確定に行くとき、OnSizeが呼ばれて不正終了していた対応
	if( pDBzm == NULL ) {
		CDataListCtrl::MakeDataRect(pDC, displine);
		return (displine == 1) ? 1 : 0;;
	}

	DT_ITEMMAX im;
	GetImax( &im );

	max = ClmItemArrayMake(displine);
	pMk = &m_clmItem[0];

	if( !(m_dwStyle & DS_BK_DEBCRE) ) {
		title = 1;

		if( Voln1 ) {
			for( i = 0; i < max; i++ ) {
				int bmsz = 8;
				if( (pMk+i)->id == DATA_DEB_BMON || (pMk+i)->id == DATA_DEB_KNO ) {
					if( Voln1->apno != 0x20 ) {
						(pMk+i)->inpsize	= bmsz;
						(pMk+i)->dispsize	= bmsz;
					}
					else {
						(pMk+i)->inpsize	= 12;
						(pMk+i)->dispsize	= 12;
					}
				}

				// 枝番6桁のときは変動事由が手形に重なるのを防ぐ
				if( (pMk+i)->id == DATA_TEKIYO ) {
					if( Voln1->edcol >= 6 ) {
						(pMk+i)->dispsize	= 81;
					}
				}
			}
		}
	}
	else {
		if( displine == 0 ) {
			title = 2;
		}
		else {
		}
	}

	// 1行のみ作成のため
	m_LineCount = 1;
	dtrect.SetSize( max );
	DT_COLUMN dtcol;
	if( displine == 0 ) {
		m_ClmArray.RemoveAll();
		m_ClmArray.SetSize(max);
	}
	for( i = 0; i < max; i++ ) {
		// 09.02 /08
		if( (pMk+i)->id == DATA_SYZ ) {
			if( CheckWidthSyohi() )
				(pMk+i)->dispsize = 8+2;
			else
				(pMk+i)->dispsize = 8;
		}

		if( displine == 0 ) {
			dtcol.ln_ofs	= (pMk+i)->ln_ofs;
			if( (pMk+i)->id == DATA_HJIYU ) {
				if( title ) {
					// 変動事由・振替科目区分[公益] タイトル
					int s_type;
					if( title == 1 ) {
						if( pAUTOSEL->DSPZEI_OPT == OPT_NODSP ) {
							s_type = 2;
						}
						else {
							s_type = 0;
						}
					}
					else {
						s_type = 2;
					}
					(pMk+i)->title = (char*)pDBzm->get_hjiyutitle( s_type );

				//	if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO)/*== 0x52*/ )	(pMk+i)->title = " ";
					if( ! IsHjiyuItemGyosyu() )		(pMk+i)->title = " ";
				}
			}
			dtcol.clm_title	= (pMk+i)->title;
			dtcol.clm_id	= (pMk+i)->id;
			dtcol.clm_type	= (pMk+i)->type;
			dtcol.clm_fmt	= (pMk+i)->format;
			dtcol.clm_back	= (pMk+i)->back;
			dtcol.clm_text	= (pMk+i)->text;
			dtcol.clm_inpsz	= (pMk+i)->inpsize;
			dtcol.clm_size	= (pMk+i)->dispsize;
			dtcol.clm_szadj	= (pMk+i)->size_adj;
			dtcol.clm_spos	= (pMk+i)->spos;
			dtcol.clm_adj	= (pMk+i)->spos_adj;
			dtcol.clm_sidx	= (pMk+i)->clm_sidx;
			m_ClmArray.SetAt( i, dtcol );
		}

		if( ! (pMk+i)->dispsize )
			sz = GetWidth( pDC, (pMk+i)->id, 0 );
		else
			sz = GetWidth( pDC, (pMk+i)->dispsize );
		// 横幅に少し余裕を持たせるための調整
		if( (pMk+i)->size_adj > 0 ) {
			sz.cx += ( CX_ADJ * (pMk+i)->size_adj );
		}

		dtid = (pMk+i)->id;

		if( !(m_dwStyle & DS_KOJI_USE) ) {
			if( (pMk+i)->id == DATA_DEB_KNO || (pMk+i)->id == DATA_CRE_KNO ) {
				dtid = DATA_NONE;
			}
		}

		basic_top = topMargin;
		if( !(m_dwStyle & DS_BK_DEBCRE) ) {	// 借貸部門工事なし
			lnofs = (pMk+i)->ln_ofs;
			if( displine == 1 ) lnofs++;
		}
		else	lnofs = (pMk+i)->ln_ofs;

		n = lnofs * sz.cy;
		
		basic_top += n;
		rc.top = basic_top;
		// 開始左座標の設定
		if( (pMk+i)->clm_sidx == -1 ) {
			if( (pMk+i)->spos == -1 ) {
				ASSERT( i > 0 );
				rc.left = dtrect.DtRect[i-1].right;
			}
			else {
				rc.left = leftMargin;
				if( (pMk+i)->spos > 0 ) {
					CSize tmp = GetWidth( pDC, (pMk+i)->spos );
					rc.left += tmp.cx;
					rc.left += CX_ADJ * (pMk+i)->spos_adj;
				}
			}
		}
		else {
//			ASSERT( i > (pMk+i)->clm_sidx );
			rc.left = dtrect.DtRect[(pMk+i)->clm_sidx].right;
		}

		rc.bottom = rc.top + sz.cy;

		if( rc.bottom >= m_rectCtrl.bottom ) {
			rc.bottom = m_rectCtrl.bottom;
			end_sw = 1;
		}
		rc.right = rc.left + sz.cx;
		if( rc.right >= m_rectCtrl.right )
			rc.right = m_rectCtrl.right-1;

	//	rc.OffsetRect(0,1);

		dtrect.DtRect[i] = rc;
		dtrect.DtID[i] = dtid;
		dtrect.DtInpRect[i] = rc;
		if( (pMk + i)->id == DATA_SYONO ) {
			CSize tmp = GetWidth(pDC, (pMk + i)->inpsize);
			CRect syon;
			syon = rc;
			syon.right = rc.left + tmp.cx;
			dtrect.DtInpRect[i] = syon;
		}

		if( i == 0 ) {
			dtrect.LineRect.top = 0;	// タイトルとデータが混ざるので
			dtrect.LineRect.bottom = 0;	// DtRect[] で処理をする
			dtrect.LineRect.left = rc.left;
			dtrect.LineRect.right = rc.right;
		}
		if( dtrect.LineRect.right < rc.right )
			dtrect.LineRect.right = rc.right;

#ifdef CLOSE
		if( dtrect.DtID[i] == DATA_TEKIYO ) {
			dtrect.DtTekRect.top	= rc.bottom;
			dtrect.DtTekRect.bottom = rc.bottom + sz.cy;
			dtrect.DtTekRect.right	= rc.right;
			dtrect.DtTekRect.left	= rc.right - sz.cx;
		}
#endif
		dtrect.DtEnable[i] = (dtid == DATA_NONE) ? 0 : 1;

		if( dtid == DATA_HJIYU ) {
			//	if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ ) {
			if( IsHjiyuItemGyosyu() == 0 ) {
				dtrect.DtEnable[i] = 0;
			}
		}
	}

	dtrect.Dtcnt = max;

	CRect* hjrc = NULL;
	CRect* hjinp = NULL;
	int		right;

	if( !(m_dwStyle & DS_BK_DEBCRE) ) {
		// 借方／貸方 １部門入力 -> 変動事由の右側隙間をなくす
		for( i = 0; i < max; i++ ) {
		//	if( dtrect.DtID[i] == DATA_SYONO ) {	// 手形は 見せなくした。
			if( dtrect.DtID[i] == DATA_HUSEN ) {
				right = dtrect.DtRect[i].left;
			}
			if( dtrect.DtID[i] == DATA_HJIYU ) {
				hjrc = &dtrect.DtRect[i];
				hjinp = &dtrect.DtInpRect[i];
			}
		}
		if( pAUTOSEL->DSPZEI_OPT != OPT_NODSP ) {
			if( hjrc ) {
				hjrc->right = right;
				hjinp->right = right;
			}
		}
	}
	else {
		// 借方／貸方 それぞれ部門入力 -> 変動事由の右側を付箋とあわせる
		for( i = 0; i < max; i++ ) {
			if( dtrect.DtID[i] == DATA_HUSEN ) {
				right = dtrect.DtRect[i].right;
			}
			if( dtrect.DtID[i] == DATA_HJIYU ) {
				hjrc = &dtrect.DtRect[i];
				hjinp = &dtrect.DtInpRect[i];
			}
		}
		if( hjrc ) {
			hjrc->right = right;
			hjinp->right = right;
		}
	}


#ifdef KEEP//-------------old---------------------------

	// 下がはみ出ないか？
	if( line_pos == 0 ) {
		sz = GetWidth( pDC, ids[0], line_pos );
		n = (sz.cy *((line_pos+1)*m_Lines) ) + topMargin;
		basic_top = topMargin;
	}
	else {
		sz = GetWidth( pDC, ids[0], 0 );
		n = (sz.cy * m_Lines ) + topMargin;	// タイトル分の行幅
		basic_top = n;
		sz = GetWidth( pDC, ids[0], 1 );
		// bottom を計算 ウィンドウに入るか？
		n += (sz.cy *( line_pos*m_Lines) );	//データ部の行幅		
		// top を計算
		basic_top += (sz.cy *( (line_pos-1)*m_Lines) );		
	}
//TRACE( "n %d, bottom %d\n", n, m_rectCtrl.bottom );

	if( m_rectCtrl.bottom < n )	{
		if( m_LineCount > (line_pos-1) )	// 表示可能行数再セット
			m_LineCount = (line_pos-1);
		return 1;
	}
#endif

//TRACE( "dtrect.LineRect %d,%d,%d,%d\n", dtrect.LineRect.top, dtrect.LineRect.left,
//										dtrect.LineRect.bottom, dtrect.LineRect.right );

	if( m_RectArray.GetSize() < (line_pos+1) ) {
		m_RectArray.SetSize( (line_pos+5) );
	}
	m_RectArray.SetAt( line_pos, dtrect );

	return 	(displine == 1) ? 1 : 0;
}


CSize CDataListLine::GetWidth( CDC* pDC, int count )
{
	CSize strsize( 0, 0 );
	char buf[200] = {0};
	if( count > sizeof buf ) {
ASSERT( FALSE );
		count = (sizeof buf)-1;
	}

	for( int i = 0; i < count; i++ ) {
		buf[i] = 'A';
	}

	strsize = pDC->GetTextExtent( buf, count );
	strsize.cx += CX_ADJ;
//	strsize.cy = (strsize.cy * 3) / 2;
	strsize.cy += CY_ADJ;

ASSERT( strsize.cx != 0 && strsize.cy != 0 );
//if( id == DATA_SEQ ) TRACE( "strsize.cx %d\n", strsize.cx );

	return strsize;
}

CSize CDataListLine::GetWidth( CDC* pDC, int id, int line )
{
	char* str = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
	CSize strsize( 0, 0 );
	CSize syohi_size( 0,0 );
	CFont* pOldFont;

//TRACE( "strlen %d\n", strlen( str ) );

	switch( id ) {
	case DATA_SEQ:
		strsize = pDC->GetTextExtent( str, m_SeqKeta );
		break;
	case DATA_DATE:
		strsize = pDC->GetTextExtent( str, 7 );
		break;
	case DATA_DENP:
		strsize = pDC->GetTextExtent( str, m_DenpKeta );
		break;
	case DATA_DEB_BMON:
	case DATA_CRE_BMON:
		strsize = pDC->GetTextExtent( str, m_BmonKeta );
		break;
	case DATA_DEBT:
	case DATA_CRED:
		strsize = pDC->GetTextExtent( str, 14+m_EdaKeta );
		break;
	case DATA_DEB_KNO:
	case DATA_CRE_KNO:
		strsize = pDC->GetTextExtent( str, m_KojiKeta );
		strsize.cx += 2;
		break;

	case DATA_SYZ:
		strsize = pDC->GetTextExtent( str, 6 );
		// 消費税のみ 横幅 を小さいフォントで計算する
		pOldFont = pDC->SelectObject( m_pSyzFont );
		syohi_size = pDC->GetTextExtent( str, 9 );
		pDC->SelectObject( pOldFont );
		if( strsize.cx < syohi_size.cx )
			strsize.cx = syohi_size.cx;
		break;

	case DATA_VAL:
	case DATA_ZEI:
		strsize = pDC->GetTextExtent( str, 15 );
		break;
	case DATA_TEKIYO:
		strsize = pDC->GetTextExtent( str, strlen(str) );
		break;
	case DATA_SYONO:
		strsize = pDC->GetTextExtent( str, 8 );
		break;
	case DATA_TGDATE:
		strsize = pDC->GetTextExtent( str, 9 );
		break;
	}

	strsize.cx += CX_ADJ;
	strsize.cy += CY_ADJ;

ASSERT( strsize.cx != 0 && strsize.cy != 0 );
//if( id == DATA_SEQ ) TRACE( "strsize.cx %d\n", strsize.cx );

	return strsize;
}


void CDataListLine::EnforceLayoutEX( BOOL bMsgSend /* = FALSE*/ )
{
	EnforceLayout(bMsgSend);
	
/*	if( ! BMON_MST ) {
		SetTitleClr( DATA_DEB_BMON, RGB_DARKER_GRAY, RGB_GRAY );
		SetTitleClr( DATA_CRE_BMON, RGB_DARKER_GRAY, RGB_GRAY );
	}
*/
	SetListItemEnable(1, DATA_DEB_BMON, BMON_MST);
	SetListItemEnable(1, DATA_CRE_BMON, BMON_MST);
/*
	if( ! M_KOJI ) {
		SetTitleClr( DATA_DEB_KNO, RGB_DARKER_GRAY, RGB_GRAY );
		SetTitleClr( DATA_CRE_KNO, RGB_DARKER_GRAY, RGB_GRAY );
	}
*/
	SetListItemEnable(1, DATA_DEB_KNO, M_KOJI);
	SetListItemEnable(1, DATA_CRE_KNO, M_KOJI);

/*	if( pAUTOSEL->BILL_OPT == OPT_NOINP ) {
		SetTitleClr( DATA_SYONO, RGB_DARKER_GRAY, RGB_GRAY );
		SetTitleClr( DATA_TGDATE, RGB_DARKER_GRAY, RGB_GRAY );

		SetListItemEnable(1, DATA_SYONO, FALSE);
		SetListItemEnable(1, DATA_TGDATE, FALSE);
	}
	if( pAUTOSEL->HUSEN_OPT == OPT_NOINP ) {
		SetTitleClr( DATA_HUSEN, RGB_DARKER_GRAY, RGB_GRAY );
		SetListItemEnable(1, DATA_HUSEN, FALSE);
	}
*/
}

