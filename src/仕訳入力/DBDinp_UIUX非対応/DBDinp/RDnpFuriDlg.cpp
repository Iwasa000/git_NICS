// RDnpFuriDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"

#include "DenpRec.h"
#include "RDnpFuriDlg.h"

#include "DnpInView.h"

extern CDnpInView* p_DnpView;



// CRDnpFuriDlg ダイアログ

// 振替伝票 項目ポジション

static DNP_POS RDFuriHeadPos[] = {
	// ヘッド部分
//	{ 0, 0, DP_DENP_PN,	1 },
//	{ 0, 1, DP_MM_PN,	1 },
//	{ 0, 2, DP_DD_PN,	1 },
//	{ 0, 3, DP_BMN_PN,	1 },
//	{ 0, 4, DP_KOJI_PN,	1 },

//	{ 0, 9, DP_KESYU_PN },
//	{ 0,11, DP_SYOGT_PN },

	{ -1, -1, -1, 0 }
};


static DNP_POS RDFuriDataPos[] = {
	// 仕訳部分
	{ 1, 0, FD_DBVAL_PN,	1},
	{ 1, 1, FD_DBZEI_PN,	1},
	{ 1, 2, FD_DSKBN_PN,	1},
	{ 1, 3, FD_DBMN_PN,		1},
	{ 1, 4, FD_DKOJI_PN,	1},
	{ 1, 5, FD_DEBT_PN,		1},
	{ 1, 6, FD_DSYOG_PN,	0},
	{ 1, 7, FD_TKY_PN,		1},
	{ 1, 8, FD_CBMN_PN,		1},
	{ 1, 9, FD_CKOJI_PN,	1},
	{ 1,10, FD_CRED_PN,		1},
	{ 1,11, FD_CSYOG_PN,	0},
	{ 1,12, FD_CSKBN_PN,	1},
	{ 1,13, FD_CRVAL_PN,	1},
	{ 1,14, FD_CRZEI_PN,	1},

	{ 1,15, FD_TGNUM_PN,	0},
	{ 1,16, FD_TGDATE_PN,	0},
	{ 1,17, FD_HJIYU_PN,	0},
	{ 1,18, FD_HUSEN_PN,	0},

	{ -1,-1, -1,	0},

};

static DNP_POS RDFuriKeiPos[] = {

	{ 2, 0, FD_DBVAL_PN,	0},
	{ 2, 1, FD_DBZEI_PN,	0},
	{ 2, 2, FD_CRVAL_PN,	0},
	{ 2, 3, FD_CRZEI_PN,	0},

	{ -1,-1, -1,	0},

};


IMPLEMENT_DYNAMIC(CRDnpFuriDlg, CRDdnpBaseDlg)

CRDnpFuriDlg::CRDnpFuriDlg(CWnd* pParent /*=NULL*/)
	: CRDdnpBaseDlg(CRDnpFuriDlg::IDD, pParent)
{
	SetDataLineCount(7);
	InitDnpBase( RDFuriHeadPos, RDFuriDataPos, RDFuriKeiPos, 145 );

	m_pParent = pParent;

	SetDenpyoKind( KIND_FURIKAE );
}

CRDnpFuriDlg::~CRDnpFuriDlg()
{
}

void CRDnpFuriDlg::DoDataExchange(CDataExchange* pDX)
{
	CRDdnpBaseDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_diag[0]);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_diag[1]);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL3, m_diag[2]);
}


BEGIN_MESSAGE_MAP(CRDnpFuriDlg, CRDdnpBaseDlg)
END_MESSAGE_MAP()


// CRDnpFuriDlg メッセージ ハンドラ

BOOL CRDnpFuriDlg::OnInitDialog()
{
	// TODO:  ここに初期化を追加してください

	CRDdnpBaseDlg::OnInitDialog();

	InitBaseDiag( m_diag, sizeof m_diag / sizeof m_diag[0] );
	diag_inz();

	inpdata_dsp();

//	CRDdnpBaseDlg::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}



void CRDnpFuriDlg::diag_inz()
{
	CRDdnpBaseDlg::diag_inz();

	CString str, kmk, buf;
	DIAGRAM_DATA		dd;

	if( m_BmonLNG && m_KojiLNG ) {
		str = _T( "部門　工事番号" );
	}
	else if( m_BmonLNG ) {
		str = _T( "部門　　　　　" );
	}
	else if( m_KojiLNG ) {
		str = _T( "　　　工事番号" );
	}
	else {
		str = _T( "　　　　　　　" );
	}

	dd.data_imgdata = NULL;
	dd.data_disp = buf;
	buf.Format( "%s\n借　　　　　方", str );

	dd.data_disp = buf;
	m_diag[0].SetData( 3, (LPUNKNOWN)&dd );

	buf.Format( "%s\n貸　　　　　方", str );
	dd.data_disp = buf;
	m_diag[0].SetData( 6, (LPUNKNOWN)&dd );

	// 諸口枝番
	long flg = DIAG_MDFY_READONLY;
	if( ! p_DnpView->IsFuriSyogBrn() )	flg = DIAG_MDFY_TRANSPARENT;

	m_diag[0].ModifyItem( 11, flg );
	m_diag[0].ModifyItem( 12, flg );

	syog_iniz();
}


//-------------------------------------------------------
//	諸口枝番 部分 入力サイン
//
//-------------------------------------------------------
void CRDnpFuriDlg::syog_iniz()
{
	int i;
	DNP_POS* pos;
	BOOL ena;

	// 仕訳データ部
	pos = RDFuriDataPos;

	for( i = 0; pos[i].dp_pn != -1; i++ ) {
		switch( pos[i].dp_pn ) {
		case FD_DSYOG_PN:
		case FD_CSYOG_PN:
			if( p_DnpView->IsFuriSyogBrn() ) {
				ena = TRUE;
			}
			else {
				ena = FALSE;
			}
			break;
		default:
			continue;
			break;
		}

		pos[i].dp_enable = ena;
	}
}




