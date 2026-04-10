// RDnpNSdlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"

#include "DenpRec.h"
#include "RDnpNSdlg.h"


////////////////////////////////////////////////////////////////////////////
// CRDnpNSdlg ダイアログ
//	定型伝票 → 入金・出金


// 入金・出金伝票 項目ポジション

static DNP_POS RD_NSHeadPos[] = {
	// ヘッド部分
/*	{ 0, 0, DP_DENP_PN },
	{ 0, 1, DP_MM_PN },
	{ 0, 2, DP_DD_PN },
*/
	{ 0, 0, DP_BMN_PN },
	{ 0, 1, DP_KOJI_PN },
//	{ 0, 5, DP_KESYU_PN },
	{ -1, -1, -1 }
};

// 入金伝票
static DNP_POS RD_NDataPos[] = {
	// 仕訳部分
	{ 1, 0, -1/*FD_CBMN_PN*/,	0},
	{ 1, 1, -1/*FD_CKOJI_PN*/,	0},
	{ 1, 2, FD_CRED_PN,		1},
	{ 1, 3, FD_TKY_PN,		1},
	{ 1, 5, FD_CRVAL_PN,	1},
	{ 1, 6, FD_CRZEI_PN,	1},
	{ 1, 4, FD_CSKBN_PN,	1},

	{ 1, 7, FD_TGNUM_PN	,	1},
	{ 1, 8, FD_TGDATE_PN,	1},
	{ 1, 9, FD_HJIYU_PN	,	1},
	{ 1,10, FD_HUSEN_PN	,	1},

	{ -1,-1, -1,	1},
};

static DNP_POS RD_NKeiPos[] = {
	//合計部分
	{ 2, 0, FD_CRVAL_PN,	0},
	{ 2, 1, FD_CRZEI_PN,	0},

	{ -1,-1, -1,	1},
};

// 出金伝票
static DNP_POS RD_SDataPos[] = {
	// 仕訳部分
	{ 1, 0, -1/*FD_DBMN_PN*/,	0},
	{ 1, 1, -1/*FD_DKOJI_PN*/,	0},
	{ 1, 2, FD_DEBT_PN,		1},
	{ 1, 3, FD_TKY_PN,		1},
	{ 1, 5, FD_DBVAL_PN,	1},
	{ 1, 6, FD_DBZEI_PN,	1},
	{ 1, 4, FD_DSKBN_PN,	1},

	{ 1, 7, FD_TGNUM_PN	,	1},
	{ 1, 8, FD_TGDATE_PN,	1},
	{ 1, 9, FD_HJIYU_PN	,	1},
	{ 1,10, FD_HUSEN_PN	,	1},

	{ -1,-1, -1,	1},
};

static DNP_POS RD_SKeiPos[] = {
	//合計部分
	{ 2, 0, FD_DBVAL_PN,	0},
	{ 2, 1, FD_DBZEI_PN,	0},

	{ -1,-1, -1,	1},
};


const int RDNS_ICNT	=	81;


IMPLEMENT_DYNAMIC(CRDnpNSdlg, CRDdnpBaseDlg)

CRDnpNSdlg::CRDnpNSdlg(CWnd* pParent /*=NULL*/)
	: CRDdnpBaseDlg(CRDnpNSdlg::IDD, pParent)
{
	SetDataLineCount(6);
	InitDnpBase( RD_NSHeadPos, RD_NDataPos, RD_NKeiPos, RDNS_ICNT );
	m_Type = KIND_NYUKIN;

	m_pParent = pParent;
}

CRDnpNSdlg::~CRDnpNSdlg()
{
}


void CRDnpNSdlg::SetDenpyoType( int type )
{
	m_Type = type;

	SetDenpyoKind( type );
}


void CRDnpNSdlg::DoDataExchange(CDataExchange* pDX)
{
	CRDdnpBaseDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_diag[0]);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_diag[1]);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL3, m_diag[2]);
}


BEGIN_MESSAGE_MAP(CRDnpNSdlg, CRDdnpBaseDlg)
END_MESSAGE_MAP()


// CRDnpNSdlg メッセージ ハンドラ

BOOL CRDnpNSdlg::OnInitDialog()
{
	CRDdnpBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	InitBaseDiag( m_diag, sizeof m_diag / sizeof m_diag[0] );
	diag_inz();

	ChangeDenpType( m_Type );

	inpdata_dsp();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}



void	CRDnpNSdlg::diag_inz()
{
	// ヘッド部の部門・工事 状態セット
	DIAGRAM_ATTRIBUTE da;

	if( m_BmonLNG ) {
		m_diag[0].ModifyItem( 10, DIAG_MDFY_OPAQUE );
		m_diag[0].GetAttribute( 0, (LPUNKNOWN)&da );
		da.attr_inpmax = m_BmonLNG;
		m_diag[0].SetAttribute( 0, (LPUNKNOWN)&da, TRUE );
	}
	else {
		m_diag[0].ModifyItem( 10, DIAG_MDFY_TRANSPARENT );
	}

	if( m_KojiLNG ) {
		m_diag[0].ModifyItem( 11, DIAG_MDFY_OPAQUE );
		m_diag[0].GetAttribute( 1, (LPUNKNOWN)&da );
		da.attr_inpmax = m_KojiLNG;
		m_diag[0].SetAttribute( 1, (LPUNKNOWN)&da, TRUE );
	}
	else {
		m_diag[0].ModifyItem( 11, DIAG_MDFY_TRANSPARENT );
	}

	CRDdnpBaseDlg::diag_inz();
}


//--------------------------------------------------
// 入出金の切り替え
//		int type	KIND_NYUKIN(入金)
//					KIND_SYUKIN(出金)
//
//--------------------------------------------------
void CRDnpNSdlg::ChangeDenpType( int type )
{
	DIAGRAM_ATTRIBUTE	da;
	DIAGRAM_DATA		dd;

	int idx1[] = {2,3,4,5,6,7,8,9,10,11};
	int idx2[] = {2};
	int i, max, n;
	COLORREF	cl;

	cl = (type == KIND_NYUKIN) ? RGB_RED : RGB_BLUE;

	max = sizeof idx1 / sizeof idx1[0];

	//項目名色変更
	for( i = 0; i < max; i++ ) {
		m_diag[0].GetAttribute( idx1[i], (LPUNKNOWN)&da );
		da.attr_fcolor = cl;
		m_diag[0].SetAttribute( idx1[i], (LPUNKNOWN)&da, FALSE );
		n = idx1[i];
	}
	n++;
	//線の色変更
	for( i = n; i <= (n+5); i++ ) {
		m_diag[0].GetAttribute( i, (LPUNKNOWN)&da );
		da.attr_lfcolor = cl;
		m_diag[0].SetAttribute( i, (LPUNKNOWN)&da, FALSE );
	}

	if( type == KIND_NYUKIN )
			dd.data_disp = "入 金 伝 票";
	else	dd.data_disp = "出 金 伝 票";

	m_diag[0].SetData( 2, (LPUNKNOWN)&dd );
	m_diag[0].Refresh();

	int pgMax, ln_start, ln_end;
	pgMax		= 15;
	ln_start	= 72;
	ln_end		= 81;

	//線の色変更
	for( int pg = 0; pg < pgMax; pg++ ) {
		int start, end;

		start	= (pg * m_ItemCount) * ln_start;
		end		= (pg * m_ItemCount) * ln_end;

		for( i = start; i <= end; i++ ) {
			m_diag[1].GetAttribute( i, (LPUNKNOWN)&da );
			da.attr_lfcolor = cl;
			m_diag[1].SetAttribute( i, (LPUNKNOWN)&da, FALSE );
		}
	}

	//項目名色変更
	max = sizeof idx2 / sizeof idx2[0];
	for( i = 0; i < max; i++ ) {
		m_diag[2].GetAttribute( idx2[i], (LPUNKNOWN)&da );
		da.attr_fcolor = cl;
		m_diag[2].SetAttribute( idx2[i], (LPUNKNOWN)&da, FALSE );

		n = idx2[i];
	}
	n++;
	//線の色変更
	for( i = 4; i <= 9; i++ ) {
		m_diag[2].GetAttribute( i, (LPUNKNOWN)&da );
		da.attr_lfcolor = cl;
		m_diag[2].SetAttribute( i, (LPUNKNOWN)&da, FALSE );
	}
	m_diag[2].Refresh();

	if( type == KIND_NYUKIN ) {
		InitDnpBase( RD_NSHeadPos, RD_NDataPos, RD_NKeiPos, RDNS_ICNT );
	}
	else {
		InitDnpBase( RD_NSHeadPos, RD_SDataPos, RD_SKeiPos, RDNS_ICNT );
	}

	m_Type = type;
}
