//////////////////////////////////////////////////////////////////////////////
// DBInpSub DLL project
// ScanKey.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"

#include "DBDinpRec.h"

#include "DBInpSubFunc.h"
//#include "DinpSub.h"

#include "ScanKey.h"

#include "../DBDinp/scansydef.h"

#include "icsdisp.h"

#include "BrnTkyDef.h"

//#include "mainfrm.h"

//#include "DinpINI.h"
//extern struct _AUTOSEL *pAUTOSEL;	// 02.20 /02 科目コード表示のため

// データページ管理
//#include "scan.h"
//extern DB_SCANINFO		SCAN_PGTBL;


#define Voln1		m_pZm->zvol


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////
extern int get_strcnt(char* str, int byte);


/////////////////////////////////////////////////////////////////////////////
// CScanKey ダイアログ

// control 表示順序 table
static int ThisCtrl[] = {
	IDC_LIST1, IDC_ST_ORDER, IDC_ST_DUSER, IDC_ST_SEQ, IDC_ST_DATE, IDC_ST_DENP, IDC_ST_DBMN, IDC_ST_CBMN, 
	IDC_ST_DKJ, IDC_ST_CKJ, IDC_ST_DEBT, IDC_ST_DBR, IDC_ST_CRED, IDC_ST_CBR, IDC_ST_DBCR, IDC_ST_DCBR,
	IDC_ST_VAL, IDC_ST_ZEI, IDC_ST_SYZSW, IDC_ST_SKBN, IDC_ST_URISIRE, IDC_ST_TKY, IDC_ST_SNUM, IDC_ST_TDAT, 
	IDC_STATIC1, IDC_STATIC2, IDC_STATIC3, IDC_STATIC4, IDC_STATIC5, IDC_STATIC6, IDC_STATIC7, IDC_STATIC8, 
	IDC_STATIC9, IDC_STATIC10, IDC_STATIC11, IDC_STATIC12, IDC_STATIC13, IDC_STATIC14, IDOK, IDCANCEL, 
	IDC_STATIC_BOX1, IDC_STATIC_BOX2, IDC_STATIC_BOX3,
	IDC_ICSDISPCTRL1, IDC_DISPORDER, IDC_DATA_USER,
	IDC_SCNSEQ1, IDC_SCNSEQ2, IDC_SCNDATE1, IDC_SCNDATE2, IDC_SCNDENP1, IDC_SCNDENP2, 
	IDC_SCNBMON1, IDC_SCNBMON2, IDC_SCNCBMN1, IDC_SCNCBMN2, IDC_SCNKOJI1, IDC_SCNKOJI2, IDC_SCNCBMN1, IDC_SCNCBMN2,
	IDC_SCNDEBT, IDC_SCNDBR1, IDC_SCNDBR2, IDC_SCNCRED, IDC_SCNCBR1, IDC_SCNCBR2, IDC_SCNDBCR, IDC_SCNDCBR1, IDC_SCNDCBR2, 
	IDC_SCNVAL1, IDC_SCNVAL2, IDC_SCNZEI1, IDC_SCNZEI2, IDC_SCNSYZSW, IDC_SCNSKBN, IDC_SCNZRITU, IDC_SCNURISIRE, 
	IDC_SCNTKY, IDC_SCNSNUM1, IDC_SCNSNUM2, IDC_SCNTDAT1, IDC_SCNTDAT2, IDC_SCNHUSEN,
	IDC_TKYONLY, IDC_IMGTKYEDIT, IDC_TKYCOMBO
};

//static int ShowCtrlTbl[] = {
//	IDC_STATIC13, IDC_STATIC14, IDC_STATIC15, IDC_STATIC20, IDC_STATIC21, IDC_STATIC22
//};
//static BOOL ShowCtrlSW = TRUE;

// フォーク サイン
//extern int IN_FORK;
//------------------------------
// プログラム準備ＯＫサイン
//extern int m_DIALOG_OK;
//------------------------------

// Select (in DinpView.c)
//extern struct _DataTable SelFix8[2*4];
//extern struct _DataTable SelVal32[8*4];

// input control table
static struct _InpCtl INP_CTL[SNOT_HUSEN_PN+1] = {
	{IDC_DISPORDER,0,0,0,-1},{IDC_DATA_USER,0,0,0,-1},
	{IDC_COMBO_STYPE,0,0,0,-1},

	{IDC_SCNSEQ1,0,0,0,-1},{IDC_SCNSEQ2,0,0,0,-1},
	{IDC_SCNDATE1,0,0,0,-1},{IDC_SCNDATE2,0,0,0,-1},
	{IDC_SCNDENP1,0,0,0,-1},{IDC_SCNDENP2,0,0,0,-1},
	{IDC_SCNBMON1,IDC_DISPETC,0,0,-1},{IDC_SCNBMON2,IDC_DISPETC,0,0,-1},
	{IDC_SCNCBMN1,IDC_DISPETC,0,0,-1},{IDC_SCNCBMN2,IDC_DISPETC,0,0,-1},
	{IDC_SCNKOJI1,IDC_DISPETC,0,0,-1},{IDC_SCNKOJI2,IDC_DISPETC,0,0,-1},
	{IDC_SCNCKJI1,IDC_DISPETC,0,0,-1},{IDC_SCNCKJI2,IDC_DISPETC,0,0,-1},

	{IDC_SCNDEBT,0,0,0,-1},{IDC_SCNDBR1,IDC_DISPETC,0,0,-1},{IDC_SCNDBR2,IDC_DISPETC,0,0,-1},
	{IDC_SCNCRED,0,0,0,-1},{IDC_SCNCBR1,IDC_DISPETC,0,0,-1},{IDC_SCNCBR2,IDC_DISPETC,0,0,-1},
	{IDC_SCNDBCR,0,0,0,-1},{IDC_SCNDCBR1,IDC_DISPETC,0,0,-1},{IDC_SCNDCBR2,IDC_DISPETC,0,0,-1},
	{IDC_SCNVAL1,0,0,0,-1},{IDC_SCNVAL2,0,0,0,-1},
	{IDC_SCNZEI1,0,0,0,-1},{IDC_SCNZEI2,0,0,0,-1},
	{IDC_SCNSYZSW,0,0,0,-1},{IDC_SCNZRITU,0,0,0,-1},{IDC_SCNSKBN,0,0,0,-1},{IDC_SCNURISIRE,0,0,0,-1},
	{IDC_SCNTKY,0,0,0,-1},
	{IDC_SCNSNUM1,0,0,0,-1},
	{IDC_SCNSNUM2,0,0,0,-1},
	{IDC_SCNTDAT1,0,0,0,-1},
	{IDC_SCNTDAT2,0,0,0,-1},
	{IDC_SCNHUSEN,0,0,0,-1},
	{IDC_SCNNOINPBRN,0,0,0,-1},
	// チェックボックス
	{IDC_CHECK_DBM,0,0,0,-1},	{IDC_CHECK_CBM,0,0,0,-1},
	{IDC_CHECK_DKJ,0,0,0,-1},	{IDC_CHECK_CKJ,0,0,0,-1},
	{IDC_CHECK_DBR,0,0,0,-1},
	{IDC_CHECK_CBR,0,0,0,-1},
	// NOT ボタン
	{IDC_NCHK_SEQ, 0,0,0,-1},	{IDC_NCHK_DATE, 0,0,0,-1},	{IDC_NCHK_DENP, 0,0,0,-1},	
	{IDC_NCHK_DBMN, 0,0,0,-1},	{IDC_NCHK_CBMN, 0,0,0,-1},
	{IDC_NCHK_DKJI, 0,0,0,-1},	{IDC_NCHK_CKJI, 0,0,0,-1},
	{IDC_NCHK_DBT, 0,0,0,-1},	{IDC_NCHK_DBRN, 0,0,0,-1},
	{IDC_NCHK_CRE, 0,0,0,-1},	{IDC_NCHK_CBRN, 0,0,0,-1},
	{IDC_NCHK_DBCR, 0,0,0,-1},	{IDC_NCHK_DCBRN, 0,0,0,-1},

	{IDC_NCHK_VAL, 0,0,0,-1},	{IDC_NCHK_ZEI, 0,0,0,-1},
	{IDC_NCHK_SYKBN, 0,0,0,-1},	{IDC_NCHK_ZRITU, 0,0,0,-1},
	{IDC_NCHK_ZKBN, 0,0,0,-1},	{IDC_NCHK_URISIRE, 0,0,0,-1},

	{IDC_NCHK_SNUMBER,0,0,0,-1},{IDC_NCHK_TGDATE,0,0,0,-1},	{IDC_NCHK_HUSEN,0,0,0,-1}

};


struct _KeyMoveTbl SkeyMvTbl[] = 
{
	{SCN_ORDER_PN,	-1,-1,	-1,				SCN_DUSER_PN },
	{SCN_DUSER_PN,	-1,-1,	SCN_ORDER_PN,	SCN_STYPE_PN},
	{SCN_STYPE_PN,	-1,-1,	SCN_ORDER_PN,	SNOT_SEQ_PN},

	{SNOT_SEQ_PN,	-1, SNOT_DATE_PN, SCN_STYPE_PN,	SCN_SEQ1_PN },
	{SCN_SEQ1_PN,	-1, SCN_DATE1_PN, SNOT_SEQ_PN,	SCN_SEQ2_PN },
	{SCN_SEQ2_PN,	-1, SCN_DATE2_PN, SCN_SEQ1_PN,	SNOT_DATE_PN },

	{SNOT_DATE_PN,	SNOT_SEQ_PN, SNOT_DENP_PN, SCN_SEQ2_PN,		SCN_DATE1_PN },
	{SCN_DATE1_PN,	SCN_SEQ1_PN, SCN_DENP1_PN, SNOT_DATE_PN,	SCN_DATE2_PN },
	{SCN_DATE2_PN,	SCN_SEQ2_PN, SCN_DENP2_PN, SCN_DATE1_PN,	SNOT_DENP_PN },

	{SNOT_DENP_PN,	SNOT_DATE_PN, SNOT_DBMN_PN, SCN_DATE2_PN,	SCN_DENP1_PN },
	{SCN_DENP1_PN,	SCN_DATE1_PN, SCN_BMON1_PN, SNOT_DENP_PN,	SCN_DENP2_PN },
	{SCN_DENP2_PN,	SCN_DATE2_PN, SCN_BMON2_PN, SCN_DENP1_PN,	SNOT_DBMN_PN },

	{SNOT_DBMN_PN,	SNOT_DENP_PN, SNOT_CBMN_PN, SCN_DENP2_PN,	SCN_BMON1_PN },
	{SCN_BMON1_PN,	SCN_DENP1_PN, SCN_CBMN1_PN, SNOT_DBMN_PN,	SCN_BMON2_PN },
	{SCN_BMON2_PN,	SCN_DENP2_PN, SCN_CBMN2_PN, SCN_BMON1_PN,	SCN_CHKDBM_PN },
	{SCN_CHKDBM_PN, SCN_DENP2_PN, SCN_CHKCBM_PN,SCN_BMON2_PN,	SNOT_DKJI_PN },

	{SNOT_CBMN_PN,	SNOT_DBMN_PN, SNOT_DEB_PN,	SCN_CHKDKJ_PN,	SCN_CBMN1_PN },
	{SCN_CBMN1_PN,	SCN_BMON1_PN, SCN_DEBT_PN,	SNOT_CBMN_PN,	SCN_CBMN2_PN },
	{SCN_CBMN2_PN,	SCN_BMON2_PN, SCN_DEBT_PN,	SCN_CBMN1_PN,	SCN_CHKCBM_PN },
	{SCN_CHKCBM_PN, SCN_CHKDBM_PN,SCN_DEBT_PN,	SCN_CBMN2_PN,	SNOT_CKJI_PN },

	{SNOT_DKJI_PN,	SNOT_CBMN_PN, SNOT_CKJI_PN,	SCN_CHKDBM_PN,	SNOT_CBMN_PN },
	{SCN_KOJI1_PN,	SCN_DENP2_PN, SCN_CKJI1_PN, SNOT_DKJI_PN,	SCN_KOJI2_PN },
	{SCN_KOJI2_PN,	SCN_DENP2_PN, SCN_CKJI2_PN, SCN_KOJI1_PN,	SCN_CHKDKJ_PN },
	{SCN_CHKDKJ_PN, SCN_DENP2_PN, SCN_CHKCKJ_PN,SCN_KOJI2_PN,	SCN_CBMN1_PN },

	{SNOT_CKJI_PN,	SNOT_DKJI_PN, SCN_CHKDBR_PN,SCN_CHKCBM_PN,	SCN_CKJI1_PN },
	{SCN_CKJI1_PN,	SCN_KOJI1_PN, SCN_DEBT_PN,	SNOT_CKJI_PN,	SCN_CKJI2_PN },
	{SCN_CKJI2_PN,	SCN_KOJI2_PN, SCN_DEBT_PN,	SCN_CKJI1_PN,	SCN_CHKCKJ_PN },
	{SCN_CHKCKJ_PN, SCN_CHKCKJ_PN,SCN_DEBT_PN,	SCN_CKJI2_PN,	SNOT_DEB_PN },

	{SNOT_DEB_PN,	SNOT_CBMN_PN, SNOT_CRE_PN,	SCN_CHKCKJ_PN,	SCN_DEBT_PN },
	{SCN_DEBT_PN,	SCN_CBMN1_PN, SCN_CRED_PN,	SNOT_DEB_PN,	SNOT_DBRN_PN },
	{SNOT_DBRN_PN,	SCN_CHKCBM_PN,SNOT_CBRN_PN,	SCN_DEBT_PN,	SCN_DBR1_PN },
	{SCN_DBR1_PN,	SCN_CBMN1_PN, SCN_CBR1_PN,	SNOT_DBRN_PN,	SCN_DBR2_PN },
	{SCN_DBR2_PN,	SCN_CBMN1_PN, SCN_CBR2_PN,	SCN_DBR1_PN,	SCN_CHKDBR_PN},
	{SCN_CHKDBR_PN,	SCN_CHKCBM_PN,SCN_CHKCBR_PN,SCN_DBR2_PN,	SNOT_CRE_PN },

	{SNOT_CRE_PN,	SNOT_DEB_PN, SNOT_DBCR_PN,	SCN_CHKDBR_PN,	SCN_CRED_PN },
	{SCN_CRED_PN,	SCN_DEBT_PN, SCN_DBCR_PN,	SNOT_CRE_PN,	SNOT_CBRN_PN },
	{SNOT_CBRN_PN,	SNOT_DBRN_PN, SNOT_DCBRN_PN,SCN_CRED_PN,	SCN_CBR1_PN },
	{SCN_CBR1_PN,	SCN_DBR1_PN, SCN_DCBR1_PN,	SNOT_CBRN_PN,	SCN_CBR2_PN },
	{SCN_CBR2_PN,	SCN_DBR2_PN, SCN_DCBR2_PN,	SCN_CBR1_PN,	SCN_CHKCBR_PN},
	{SCN_CHKCBR_PN,	SCN_CHKCBM_PN,SCN_CHKCBR_PN,SCN_CBR2_PN,	SNOT_DBCR_PN },

	{SNOT_DBCR_PN,	SNOT_CRE_PN, SNOT_VAL_PN,	SCN_CHKCBR_PN,	SCN_DBCR_PN },
	{SCN_DBCR_PN,	SCN_CRED_PN, SCN_VAL1_PN,	SNOT_DBCR_PN,	SNOT_DCBRN_PN },
	{SNOT_DCBRN_PN,	SNOT_CBRN_PN, SNOT_VAL_PN,	SCN_DBCR_PN,	SCN_DCBR1_PN },
	{SCN_DCBR1_PN,	SCN_CBR1_PN, SCN_VAL1_PN,	SNOT_DCBRN_PN,	SCN_DCBR2_PN },
	{SCN_DCBR2_PN,	SCN_CBR2_PN, SCN_VAL2_PN,	SCN_DCBR1_PN,	SNOT_VAL_PN},

	{SNOT_VAL_PN,	SNOT_DBCR_PN,SNOT_ZEI_PN,	SCN_DCBR2_PN,	SCN_VAL1_PN },
	{SCN_VAL1_PN,	SCN_DBCR_PN, SCN_ZEI1_PN,	SNOT_VAL_PN,	SCN_VAL2_PN },
	{SCN_VAL2_PN,	SCN_DCBR1_PN,SCN_ZEI2_PN,	SCN_VAL1_PN,	SNOT_ZEI_PN },
	
	{SNOT_ZEI_PN,	SNOT_DBCR_PN,SNOT_SYZSW_PN,	SCN_VAL2_PN,	SCN_ZEI1_PN },
	{SCN_ZEI1_PN,	SCN_VAL1_PN, SCN_SYZSW_PN,	SCN_VAL2_PN,	SCN_ZEI2_PN },
	{SCN_ZEI2_PN,	SCN_VAL2_PN, SCN_SYZSW_PN,	SCN_ZEI1_PN,	SNOT_SYZSW_PN},

	{SNOT_SYZSW_PN,	SNOT_ZEI_PN,	SNOT_SKBN_PN,	SCN_ZEI2_PN,	SCN_SYZSW_PN },
	{SCN_SYZSW_PN,	-1,-1,	SNOT_SYZSW_PN,	SNOT_ZRITU_PN },

	{SNOT_ZRITU_PN,	SCN_ZEI2_PN,	SNOT_URISIRE_PN,SCN_SYZSW_PN,	SCN_ZRITU_PN },
	{SCN_ZRITU_PN,	-1,-1,	SNOT_ZRITU_PN,	SNOT_SKBN_PN},
	
	{SNOT_SKBN_PN,	SNOT_SYZSW_PN,	SNOT_SNUM_PN,SCN_ZRITU_PN,	SCN_SKBN_PN },
	{SCN_SKBN_PN,	-1,-1,	SNOT_SKBN_PN,	SNOT_URISIRE_PN },
	
	{SNOT_URISIRE_PN,SNOT_ZRITU_PN,	SCN_TKY_PN,	SCN_SKBN_PN,	SCN_URISIRE_PN },
	{SCN_URISIRE_PN,-1,-1,	SNOT_URISIRE_PN,	SCN_TKY_PN},
	
	{SCN_TKY_PN,	SCN_SYZSW_PN,SCN_SNUM1_PN,	SCN_URISIRE_PN,	SNOT_SNUM_PN},

	{SNOT_SNUM_PN,	SNOT_SKBN_PN, SNOT_TGDATE_PN,SCN_TKY_PN,	SCN_SNUM1_PN },
	{SCN_SNUM1_PN,	SCN_TKY_PN,	SCN_TDAT1_PN,	SNOT_SNUM_PN,	SCN_SNUM2_PN },
	{SCN_SNUM2_PN,	SCN_TKY_PN,	SCN_TDAT2_PN,	SCN_SNUM1_PN,	SNOT_TGDATE_PN },	
	{SNOT_TGDATE_PN,SNOT_SNUM_PN, SNOT_HUSEN_PN,SCN_SNUM2_PN,	SCN_TDAT1_PN },
	{SCN_TDAT1_PN,	SCN_SNUM1_PN, -1,	SNOT_TGDATE_PN,	SCN_TDAT2_PN },	
	{SCN_TDAT2_PN,	SCN_SNUM2_PN, -1,	SCN_TDAT1_PN,	SNOT_HUSEN_PN },	

	{SNOT_HUSEN_PN,	SNOT_TGDATE_PN, -1,	SCN_TDAT2_PN,	SCN_HUSEN_PN },
	{SCN_HUSEN_PN,	-1,-1,	SNOT_HUSEN_PN,	SCN_NOINP_PN},
	{SCN_NOINP_PN,	-1,-1,	SCN_HUSEN_PN,	-1},

	{-1,-1,-1,-1,-1}

};


// Now ポジション
static int NOW_PN = -1;

// 枝番摘要表示コントロールＸ軸拡大比率
#ifndef CLOSE	// 10.01 /99
#define BRNTKY_X_RATE 3.2
#else
#define BRNTKY_X_RATE 2.8
#endif

// 部門名称表示コントロールＸ軸拡大比率 08.19 /02
//#define BUMON_X_RATE	(Voln1 && Voln1->APNO == 0x20) ? 3.5 : 2.9
#define BUMON_X_RATE	3.5

#define _2KT_BSEL	// 2桁部門選択


CScanKey::CScanKey(CWnd* pParent /*=NULL*/)
	: ICSDialog(CScanKey::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScanKey)
	//}}AFX_DATA_INIT

	m_pParent = pParent;
	m_nID = CScanKey::IDD;

#ifdef	_ICSPEN_	// 01.20 /01
	flgPen	=	FALSE;
#endif

	m_bTKYONLY = FALSE;
	//アクセラレータテーブルロード
//	m_hAccelTable = ::LoadAccelerators( AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

#ifndef CLOSE //#* 06.19 /01 ｴﾃﾞｨｯﾄｺﾝﾄﾛｰﾙの色
	m_EditColor[0] = ::CreateSolidBrush( RGB_WHITE );
	m_EditColor[1] = ::CreateSolidBrush( RGB_YELLOW );
	m_EditColor[2] = ::CreateSolidBrush( SELECT_BACK_COL );
#endif

	m_DIALOG_OK	= FALSE;
	m_nDtOwner	= -2;	
	m_nDtMode	= 0;

	ASSERT( INP_CTL[SNOT_HUSEN_PN].IDC_X == IDC_NCHK_HUSEN );

}


void CScanKey::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScanKey)
	DDX_Control(pDX, IDC_SCNNOINPBRN, m_NipBrn);
	DDX_Control(pDX, IDC_ALLKEYDEL, m_AllDel);
	DDX_Control(pDX, IDC_COMBO_STYPE, m_SType);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_TKYONLY, m_TKonly);
	DDX_Control(pDX, IDC_CHECK_DBM, m_ChkDBM );
	DDX_Control(pDX, IDC_CHECK_CBM, m_ChkCBM );
	DDX_Control(pDX, IDC_CHECK_DKJ, m_ChkDKJ );
	DDX_Control(pDX, IDC_CHECK_CKJ, m_ChkCKJ );
	DDX_Control(pDX, IDC_CHECK_DBR, m_ChkDbr );
	DDX_Control(pDX, IDC_CHECK_CBR, m_ChkCbr );
	DDX_Control(pDX, IDC_DISPORDER, m_Order);
	DDX_Control(pDX, IDC_DATA_USER, m_DUser);
	DDX_Control(pDX, IDC_TKYCOMBO, m_Tkycb);
	DDX_Control(pDX, IDC_SCNZRITU, m_Zritu);
	DDX_Control(pDX, IDC_SCNURISIRE, m_Urisire);
	DDX_Control(pDX, IDC_SCNSYZSW, m_Syzsw);
	DDX_Control(pDX, IDC_SCNSKBN, m_Skbn);
	DDX_Control(pDX, IDC_LIST1, m_LIST1);
	DDX_Control(pDX, IDC_SCNBMON1, m_Bmon1);
	DDX_Control(pDX, IDC_SCNBMON2, m_Bmon2);
	DDX_Control(pDX, IDC_SCNCBMN1, m_Cbmn1);
	DDX_Control(pDX, IDC_SCNCBMN2, m_Cbmn2);
	DDX_Control(pDX, IDC_SCNCBR1, m_Cbr1);
	DDX_Control(pDX, IDC_SCNCBR2, m_Cbr2);
	DDX_Control(pDX, IDC_SCNCKJI1, m_Ckji1);
	DDX_Control(pDX, IDC_SCNCKJI2, m_Ckji2);
	DDX_Control(pDX, IDC_SCNCRED, m_Cred);
	DDX_Control(pDX, IDC_SCNDBR1, m_Dbr1);
	DDX_Control(pDX, IDC_SCNDBR2, m_Dbr2);
	DDX_Control(pDX, IDC_SCNDEBT, m_Debt);
	DDX_Control(pDX, IDC_SCNDBCR, m_DBcr);
	DDX_Control(pDX, IDC_SCNDCBR1, m_DCbr1);
	DDX_Control(pDX, IDC_SCNDCBR2, m_DCbr2);
	DDX_Control(pDX, IDC_SCNDENP1, m_Denp1);
	DDX_Control(pDX, IDC_SCNDENP2, m_Denp2);
	DDX_Control(pDX, IDC_SCNKOJI1, m_Koji1);
	DDX_Control(pDX, IDC_SCNKOJI2, m_Koji2);
	DDX_Control(pDX, IDC_SCNSEQ1, m_Seq1);
	DDX_Control(pDX, IDC_SCNSEQ2, m_Seq2);
	DDX_Control(pDX, IDC_SCNSNUM1, m_Snum1);
	DDX_Control(pDX, IDC_SCNSNUM2, m_Snum2);
	DDX_Control(pDX, IDC_SCNTDAT1, m_Tdat1);
	DDX_Control(pDX, IDC_SCNTDAT2, m_Tdat2);
	DDX_Control(pDX, IDC_SCNTKY, m_Tky);
	DDX_Control(pDX, IDC_SCNVAL1, m_Val1);
	DDX_Control(pDX, IDC_SCNVAL2, m_Val2);
	DDX_Control(pDX, IDC_SCNZEI1, m_Zei1);
	DDX_Control(pDX, IDC_SCNZEI2, m_Zei2);
	DDX_Control(pDX, IDC_ICSSELCTRL1, m_selfix8);
	DDX_Control(pDX, IDC_ICSSELCTRL2, m_selval32);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SCNHUSEN, m_Hcbo);
}

BOOL CScanKey::Create()
{
	return ICSDialog::Create(m_nID, m_pParent);
}

void CScanKey::PostNcDestroy() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	ICSDialog::PostNcDestroy();
//	delete this;
}

// 初期化処理
int CScanKey::InitWork( SCAN_ITEM* pSI, DWORD nFlag, CDBZmSub* pZm ) 
{
	m_pZm = pZm;

	if( Voln1->ocr_idev == 3 )
		IMG_master = 0;
	else
		IMG_master = ERR;

	if( m_Sy.SyOpen( m_pZm ) == ERR ) {
		return -1;
	}

	m_nFlag = nFlag;

	m_Seldata.Init( m_pZm, this, (SELECTDATAPROC)SetSelectFix8, (SELECTDATAPROC)SetSelectSel32 );
	m_Seldata.SetKmDispMode( (nFlag & SCF_KMKCODE) ? SL_KMK_CODE : SL_KMK_NUM );
	// 選択画面に科目コード表示
	bKamokuCode = (nFlag & SCF_KMKCODE) ? TRUE : FALSE;

	int	sw;

	if( (nFlag & (SCF_BRNTKYO|SCF_SYOGBRN)) == (SCF_BRNTKYO|SCF_SYOGBRN) )
		sw = (BT_NOMALBRN_TEK|BT_SYOGTBRN_TEK);
	else if( nFlag & SCF_BRNTKYO )
		sw = BT_NOMALBRN_TEK;
	else if( nFlag & SCF_SYOGBRN )
		sw = BT_SYOGTBRN_TEK;

	sw |= SL_BRNTKY_ONLY;
	m_Seldata.SetBrnTkyMode( sw );
	m_Seldata.SetTekiyoMode( (nFlag & SCF_TKYSWK) ? SL_TKY_SIWAKE : SL_TKY_RENSO );

	keyline_dataset( pSI );

	return 0;
}


int CALLBACK CScanKey::SetSelectFix8( _SelTable* pSel, SelSetProcPar* pSetProc, int cnt, CWnd* lp )
{
	BOOL bNumber;
	CScanKey* pSkey;
	pSkey = (CScanKey*)lp;

	bNumber = (pSkey->m_nFlag & SCF_KMKCODE) ? FALSE : TRUE;

	if( pSkey->m_selfix8.GetNumber() != bNumber )
		pSkey->m_selfix8.SetNumber( bNumber );

	pSkey->m_selfix8.SetData( (long)pSel );

	return 0;
}


int CALLBACK CScanKey::SetSelectSel32( _SelTable* pSel, SelSetProcPar* pSetProc, int cnt, CWnd* lp )
{	
	BOOL bNumber;
	CScanKey* pSkey;
	pSkey = (CScanKey*)lp;

	switch( pSetProc->sp_nowpn ) {
	case SL_KAMOKU_PN:
	case SL_KMKANA_PN:
		bNumber = (pSkey->m_nFlag & SCF_KMKCODE) ? FALSE : TRUE;
		break;
	default:
		bNumber = TRUE;
	}
	if( pSkey->m_selval32.GetNumber() != bNumber )
		pSkey->m_selval32.SetNumber( bNumber );

	if( pSetProc->sp_nowpn == SL_BRNTKY_PN ) {
		// 選択画面の色
		COLORREF txt = pSkey->m_Seldata.GetSel32TextColor();
		COLORREF back = pSkey->m_Seldata.GetSel32BackColor();

		pSkey->m_selval32.SetForeColor( txt );	// 文字色
		pSkey->m_selval32.SetBackColor( back );	// 背景色
		pSkey->BRNTKY_sel = 1;
	}
	else if( pSetProc->sp_prevpn == SL_BRNTKY_PN ) {
		// 選択画面の色を元に戻す
		pSkey->enable_fixval( pSkey->fixvalcnd() );	// 元背景色強制描画のため
		pSkey->BRNTKY_sel = 0;
	}

	pSkey->set_valtitle( pSetProc->seljob );

	pSkey->m_selval32.SetData( (long)pSel );

	return 0;
}


// DISP のオリジナルサイズ
void CScanKey::get_disp2_originalsize( double& w, double& h )
{
	w = (double)(m_disp2_rect.right - m_disp2_rect.left);
	h = (double)(m_disp2_rect.bottom - m_disp2_rect.top);
}

//
// ポジション位置のブロックラインゲット
static int get_blockno( int pn )
{
	int now_blk;
	
	switch( pn )
	{
	case SCN_ORDER_PN:	now_blk = 0;	break;
	case SCN_DUSER_PN:	now_blk = 1;	break;
//	case SCN_STYPE_PN:	now_blk = 1;	break;

	case SCN_SEQ1_PN:
	case SCN_SEQ2_PN:	now_blk = 2;	break;

	case SCN_DATE1_PN:
	case SCN_DATE2_PN:	now_blk = 3;	break;

	case SCN_DENP1_PN:
	case SCN_DENP2_PN:	now_blk = 4;	break;

	case SCN_BMON1_PN:
	case SCN_BMON2_PN:
	case SCN_CHKDBM_PN:	now_blk = 5;	break;

	case SCN_CBMN1_PN:
	case SCN_CBMN2_PN:
	case SCN_CHKCBM_PN:	now_blk = 6;	break;

	case SCN_KOJI1_PN:
	case SCN_KOJI2_PN:
	case SCN_CHKDKJ_PN:	now_blk = 7;	break;

	case SCN_CKJI1_PN:
	case SCN_CKJI2_PN:
	case SCN_CHKCKJ_PN:	now_blk = 8;	break;

	case SCN_DEBT_PN:
	case SCN_DBR1_PN:
	case SCN_DBR2_PN:
	case SCN_CHKDBR_PN:	now_blk = 9;	break;

	case SCN_CRED_PN:
	case SCN_CBR1_PN:
	case SCN_CBR2_PN:	
	case SCN_CHKCBR_PN:	now_blk = 10;	break;

	case SCN_DBCR_PN:
	case SCN_DCBR1_PN:
	case SCN_DCBR2_PN:  now_blk = 11;	break;
		
	case SCN_VAL1_PN:
	case SCN_VAL2_PN:	now_blk = 12;	break;

	case SCN_ZEI1_PN:
	case SCN_ZEI2_PN:	now_blk = 13;	break;

	case SCN_SYZSW_PN:	now_blk = 14;	break;

	case SCN_SKBN_PN:	now_blk = 15;	break;

	case SCN_ZRITU_PN:	now_blk = 16;	break;

	case SCN_URISIRE_PN:	now_blk = 17;	break;

	case SCN_TKY_PN:	now_blk = 18;	break;

	case SCN_SNUM1_PN:	
	case SCN_SNUM2_PN:	now_blk = 19;	break;

	case SCN_TDAT1_PN:
	case SCN_TDAT2_PN:	now_blk = 20;	break;

	default:			now_blk = -1;	break;
	}

	return now_blk;
}

//
// 入力項目背景色セット
// select_back_col() 後に実行すること
void CScanKey::inp_bkcol( int pn )
{
	int blk;
	COLORREF set_col;
	BOOL img_edit;
	struct _InpCtl *ctl;
	CWnd *pwnd;
	int now_blk = get_blockno( pn );	// 現在カーソルのあるブロック

	for( pn = SCN_SEQ1_PN ; pn < SCN_TDAT2_PN + 1 ; ++pn )
	{
		if( SCAN_KEY.LINE_CND[ pn ].INP_sg )
		{
			set_col = RGB_YELLOW;	// 入力有りカラー
		}
		else
		{
			set_col = RGB_WHITE;
		}

		ctl = scGetINP_CTL( pn );
#ifndef CLOSE //#* 06.19 /01
		img_edit = ( pn == SCN_TKY_PN && is_mdfy_imgtky() );
#endif
		blk = get_blockno( pn );
		// 入力背景色描画orセット
		if( ctl->BACK_COLOR != -1 && ctl->BACK_COLOR != set_col )
		{
			// セット
			ctl->BACK_COLOR = set_col;

			// 描画
			if( blk != now_blk )
			{
				VERIFY( pwnd = GetDlgItem( ctl->IDC_X ) );
				if( img_edit )
					GetDlgItem(IDC_IMGTKYEDIT)->Invalidate();
				else
					((CICSDBEDT*)pwnd)->SetBackColor( set_col );
			}
		}
	}
}

#ifndef CLOSE	// 12.14 /98
//
// 選択項目背景色セット
void CScanKey::select_back_col( int pn )
{
static int prev_blk = -1;	// 直前の位置のセーブ
int blk,now_blk;
BOOL sel, img_edit;
int i,n,max;
int pnt[4];
struct _InpCtl *ctl;
CWnd *pwnd;

	now_blk = get_blockno( pn );

	if( now_blk == prev_blk )
		return;

	for( n = 0 ; n < 2 ; ++n )
	{
		if( !n )
		{
			blk = now_blk;
			sel = TRUE;
		}
		else
		{
			blk = prev_blk;
			sel = FALSE;
		}

		max = 0;
		switch( blk )
		{
		case 0:		pnt[max++] = SCN_SEQ1_PN;	break;
		case 1:		pnt[max++] = SCN_SEQ2_PN;	break;

		case 2:		pnt[max++] = SCN_SEQ1_PN;
					pnt[max++] = SCN_SEQ2_PN;
					break;

		case 3:		pnt[max++] = SCN_DATE1_PN;
					pnt[max++] = SCN_DATE2_PN;
					break;

		case 4:		pnt[max++] = SCN_DENP1_PN;
					pnt[max++] = SCN_DENP2_PN;
					break;

		case 5:		pnt[max++] = SCN_BMON1_PN;
					pnt[max++] = SCN_BMON2_PN;
					break;
		case 6:		pnt[max++] = SCN_CBMN1_PN;
					pnt[max++] = SCN_CBMN2_PN;
					break;
		case 7:		pnt[max++] = SCN_KOJI1_PN;
					pnt[max++] = SCN_KOJI2_PN;
					break;
		case 8:		pnt[max++] = SCN_CKJI1_PN;
					pnt[max++] = SCN_CKJI2_PN;
					break;

		case 9:		pnt[max++] = SCN_DEBT_PN;
					pnt[max++] = SCN_DBR1_PN;
					pnt[max++] = SCN_DBR2_PN;
					break;
		case 10:	pnt[max++] = SCN_CRED_PN;
					pnt[max++] = SCN_CBR1_PN;
					pnt[max++] = SCN_CBR2_PN;
					break;
		case 11:	pnt[max++] = SCN_DBCR_PN;
					pnt[max++] = SCN_DCBR1_PN;
					pnt[max++] = SCN_DCBR2_PN;
					break;
			
		case 12:	pnt[max++] = SCN_VAL1_PN;
					pnt[max++] = SCN_VAL2_PN;
					break;

		case 13:	pnt[max++] = SCN_ZEI1_PN;
					pnt[max++] = SCN_ZEI2_PN;
					break;

		case 14:	pnt[max++] = SCN_SYZSW_PN;
					break;
		case 15:	pnt[max++] = SCN_ZRITU_PN;
					break;
		case 16:	pnt[max++] = SCN_SKBN_PN;
					break;
		case 17:	pnt[max++] = SCN_URISIRE_PN;
					break;

		case 18:	pnt[max++] = SCN_TKY_PN;
					break;

		case 19:	pnt[max++] = SCN_SNUM1_PN;
					pnt[max++] = SCN_SNUM2_PN;
					break;
		case 20:	pnt[max++] = SCN_TDAT1_PN;
					pnt[max++] = SCN_TDAT2_PN;
					break;

		default:	continue;
		}

		for( i = 0 ; i < max ; ++i )
		{
			ctl = scGetINP_CTL( pnt[i] );
			VERIFY( pwnd = GetDlgItem( ctl->IDC_X ) );

			img_edit = ( pnt[i] == SCN_TKY_PN && is_mdfy_imgtky() );

			if( pnt[i] == SCN_ORDER_PN || pnt[i] == SCN_DUSER_PN ||
				pnt[i] == SCN_SYZSW_PN || pnt[i] == SCN_ZRITU_PN ||
				pnt[i] == SCN_SKBN_PN || pnt[i] == SCN_URISIRE_PN )
				continue;

			if( sel == TRUE )
			{
				// 選択背景色描画
				ctl->BACK_COLOR = ((CICSDBEDT*)pwnd)->GetBackColor();
				((CICSDBEDT*)pwnd)->SetBackColor( SELECT_BACK_COL );
				if( img_edit ) {
					GetDlgItem(IDC_IMGTKYEDIT)->Invalidate();
					GetDlgItem(IDC_IMGTKYEDIT)->UpdateWindow();
				}
			}
			else
			{
				// 選択背景色ＯＦＦ
				((CICSDBEDT*)pwnd)->SetBackColor( ctl->BACK_COLOR );
				if( img_edit ) {
					GetDlgItem(IDC_IMGTKYEDIT)->Invalidate();
					GetDlgItem(IDC_IMGTKYEDIT)->UpdateWindow();
				}
			}
			pwnd->UpdateWindow();
		}
	}
	prev_blk = now_blk;	// 今回位置のセーブ
}
#endif

//
// コメントの表示
void CScanKey::cmntbox( int pn )
{
	int idc, idc_box = IDC_STATIC_BOX3;

	if( !BRNTKY_sel )
		idc = -1;
	else
	{
		switch( pn )
		{
		case SCN_DBR1_PN:
		case SCN_DBR2_PN://	idc = scGetINP_CTL( SCN_DBR2_PN )->IDC_X; break;
							idc = IDC_CHECK_DBM;		break; //02.26 /03
		case SCN_CBR1_PN:
		case SCN_CBR2_PN://	idc = scGetINP_CTL( SCN_CBR2_PN )->IDC_X; break;
							idc = IDC_SCNZEI2;	break; //02.26 /03
		case SCN_DCBR1_PN:
		case SCN_DCBR2_PN://idc = scGetINP_CTL( SCN_DCBR2_PN )->IDC_X; break;
							idc = IDC_SCNZEI2;	break; //02.26 /03

		default:			idc = -1; break;
		}
	}
	_cmnt_disp( idc, idc_box );
}

void CScanKey::_cmnt_disp( int idc, int idc_box )
{
	CWnd *wnd_idc;
	CWnd *wnd_idc_box;
	WINDOWPLACEMENT idc_place;
	WINDOWPLACEMENT idc_box_place;
	RECT	newrect;
//	double 	rate_w, rate_h;

//	get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット

	wnd_idc_box = GetDlgItem( idc_box );

	if( idc == -1 )
	{
		wnd_idc_box->ShowWindow( SW_HIDE );
		return;
	}

	wnd_idc = GetDlgItem( idc );
	wnd_idc->GetWindowPlacement( &idc_place );
	
	wnd_idc_box = GetDlgItem( idc_box );
	wnd_idc_box->GetWindowPlacement( &idc_box_place );

	//Ｎｅｗポジション
#ifndef CLOSE //02.26 /03
	newrect.top = idc_place.rcNormalPosition.top - (idc_box_place.rcNormalPosition.bottom - idc_box_place.rcNormalPosition.top);
#else
	newrect.top = idc_place.rcNormalPosition.bottom - (idc_box_place.rcNormalPosition.bottom - idc_box_place.rcNormalPosition.top);
#endif
	newrect.left = idc_place.rcNormalPosition.right;
	
	wnd_idc_box->SetWindowPos( 0, newrect.left, newrect.top, 0, 0, SWP_SHOWWINDOW|SWP_NOZORDER|SWP_NOSIZE );
}

//
// 表題を窪み表示に
void CScanKey::selbox( int pn )
{
	int idc1, idc2;

	switch( pn )
	{
	case SCN_ORDER_PN:	idc1 = IDC_ST_ORDER; idc2 = -1; break;
	case SCN_DUSER_PN:	idc1 = IDC_ST_DUSER; idc2 = -1; break;
	case SCN_STYPE_PN:	idc1 = IDC_ST_STYPE; idc2 = -1; break;

	case SNOT_SEQ_PN:
	case SCN_SEQ1_PN:
	case SCN_SEQ2_PN:	idc1 = IDC_ST_SEQ; idc2 = -1; break;

	case SNOT_DATE_PN:
	case SCN_DATE1_PN:
	case SCN_DATE2_PN:	idc1 = IDC_ST_DATE; idc2 = -1; break;

	case SNOT_DENP_PN:
	case SCN_DENP1_PN:
	case SCN_DENP2_PN:	idc1 = IDC_ST_DENP; idc2 = -1; break;

	case SNOT_DBMN_PN:
	case SCN_BMON1_PN:
	case SCN_BMON2_PN:
	case SCN_CHKDBM_PN:	idc1 = IDC_ST_DBMN; idc2 = -1; break;

	case SNOT_CBMN_PN:
	case SCN_CBMN1_PN:
	case SCN_CBMN2_PN:
	case SCN_CHKCBM_PN:	idc1 = IDC_ST_CBMN; idc2 = -1; break;

	case SNOT_DKJI_PN:
	case SCN_KOJI1_PN:
	case SCN_KOJI2_PN:
	case SCN_CHKDKJ_PN:	idc1 = IDC_ST_DKJ; idc2 = -1; break;

	case SNOT_CKJI_PN:
	case SCN_CKJI1_PN:
	case SCN_CKJI2_PN:
	case SCN_CHKCKJ_PN:	idc1 = IDC_ST_CKJ; idc2 = -1; break;

	case SNOT_DEB_PN:
	case SCN_DEBT_PN:
	case SCN_CHKDBR_PN:	idc1 = IDC_ST_DEBT; idc2 = -1; break;	//#* 02.26 /03

	case SNOT_DBRN_PN:
	case SCN_DBR1_PN:
	case SCN_DBR2_PN:	idc1 = IDC_ST_DBR; idc2 = IDC_STATIC8; break;

	case SNOT_CRE_PN:
	case SCN_CRED_PN:
	case SCN_CHKCBR_PN:	idc1 = IDC_ST_CRED; idc2 = -1; break;	//#* 02.26 /03

	case SNOT_CBRN_PN:
	case SCN_CBR1_PN:
	case SCN_CBR2_PN:	idc1 = IDC_ST_CBR; idc2 = IDC_STATIC9; break;

	case SNOT_DBCR_PN:
	case SCN_DBCR_PN:
						idc1 = IDC_ST_DBCR; idc2 = -1; break;
	case SNOT_DCBRN_PN:
	case SCN_DCBR1_PN:
	case SCN_DCBR2_PN:	idc1 = IDC_ST_DCBR; idc2 = IDC_STATIC10; break;
		
	case SNOT_VAL_PN:
	case SCN_VAL1_PN:
	case SCN_VAL2_PN:	idc1 = IDC_ST_VAL; idc2 = -1; break;

	case SNOT_ZEI_PN:
	case SCN_ZEI1_PN:
	case SCN_ZEI2_PN:	idc1 = IDC_ST_ZEI; idc2 = -1; break;

	case SNOT_SYZSW_PN:
	case SCN_SYZSW_PN:		idc1 = IDC_ST_SYZSW;	idc2 = -1; break;
	case SNOT_ZRITU_PN:	
	case SCN_ZRITU_PN:		idc1 = IDC_ST_ZRITU;	idc2 = -1; break;
	case SNOT_SKBN_PN:
	case SCN_SKBN_PN:		idc1 = IDC_ST_SKBN;		idc2 = -1; break;
	case SNOT_URISIRE_PN:	
	case SCN_URISIRE_PN:	idc1 = IDC_ST_URISIRE;	idc2 = -1; break;

	case SNOT_SNUM_PN:
	case SCN_SNUM1_PN:
	case SCN_SNUM2_PN:	idc1 = IDC_ST_SNUM;		idc2 = -1; break;
	
	case SNOT_TGDATE_PN:
	case SCN_TDAT1_PN:
	case SCN_TDAT2_PN:	idc1 = IDC_ST_TDAT;		idc2 = -1; break;
	
	case SNOT_HUSEN_PN:
	case SCN_HUSEN_PN:	idc1 = IDC_ST_HUSEN;	idc2 = -1; break;

	default:
		box_disp( -1, -1 ); return;
	}
	box_disp( idc1, idc2 );
}

//
// スタティックを窪み表示に
void CScanKey::box_disp( int idc1, int idc2 )
{
	_box_disp( idc1, IDC_STATIC_BOX1 );
	_box_disp( idc2, IDC_STATIC_BOX2 );
}

void CScanKey::_box_disp( int idc, int idc_box )
{
	CWnd *wnd1;
	CWnd *wnd2;
	WINDOWPLACEMENT place1;
	RECT	newrect;
	CString txt;

	wnd2 = GetDlgItem( idc_box );

	if( idc == -1 )
	{
		wnd2->ShowWindow( SW_HIDE );
		return;
	}

	wnd1 = GetDlgItem( idc );
	
	//テキストコピー
	wnd1->GetWindowText( txt );

	//Ｎｅｗポジション
	wnd1->GetWindowPlacement( &place1 );
	newrect.top = place1.rcNormalPosition.top;
	newrect.bottom = place1.rcNormalPosition.bottom;
	newrect.left = place1.rcNormalPosition.left;
	newrect.right = place1.rcNormalPosition.right;
	
	wnd2->SetWindowPos( wnd1, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_SHOWWINDOW );
	wnd2->SetWindowText( txt );
}

//コントロールの表示順序
void CScanKey::ctrldspjun( int *tbl, int n )
{
	int i;
#ifdef CLOSE	// 08.26 /98
	int nflag = 0;
#else
	int nflag = SWP_NOACTIVATE;
#endif
	CWnd *prevorg;
	CWnd *noworg;

	for( i = 0 ; i < n ; ++i )
	{
		VERIFY( noworg = GetDlgItem( *(tbl + i) ) );
		if( i )
			noworg->SetWindowPos( prevorg, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|nflag );
		prevorg = noworg;
	}
}


BEGIN_MESSAGE_MAP(CScanKey, ICSDialog)
	//{{AFX_MSG_MAP(CScanKey)
	ON_CBN_SELCHANGE(IDC_TKYCOMBO, OnSelchangeTkycombo)
	ON_CBN_SELCHANGE(IDC_SCNSKBN, OnSelchangeScnskbn)
	ON_CBN_SELCHANGE(IDC_SCNZRITU, OnSelchangeScnzritu)
	ON_CBN_SELCHANGE(IDC_SCNSYZSW, OnSelchangeScnsyzsw)
	ON_CBN_SELCHANGE(IDC_SCNURISIRE, OnSelchangeScnurisire)
	ON_CBN_SELCHANGE(IDC_DATA_USER, OnSelchangeDataUser)
	ON_CBN_SELCHANGE(IDC_DISPORDER, OnSelchangeDisporder)
	ON_CBN_SELCHANGE(IDC_SCNHUSEN, OnSelchangeScnhusen)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_TKYONLY, OnTkyonly)
	ON_BN_CLICKED(IDC_CHECK_CBM, OnCheckCbm)
	ON_BN_CLICKED(IDC_CHECK_CBR, OnCheckCbr)
	ON_BN_CLICKED(IDC_CHECK_CKJ, OnCheckCkj)
	ON_BN_CLICKED(IDC_CHECK_DBM, OnCheckDbm)
	ON_BN_CLICKED(IDC_CHECK_DBR, OnCheckDbr)
	ON_BN_CLICKED(IDC_CHECK_DKJ, OnCheckDkj)
	ON_CBN_SELCHANGE(IDC_COMBO_STYPE, OnSelchangeComboStype)
	ON_BN_CLICKED(IDC_ALLKEYDEL, OnAllkeydel)
	ON_CBN_SELCHANGE(IDC_SCNNOINPBRN, OnSelchangeScnnoinpbrn)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_RETROFOCUS, RetroFocus )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanKey メッセージ ハンドラ

BOOL CScanKey::OnInitDialog() 
{
	CWnd* org;
	char pbuf[50];

	GetWindowRect( &m_original_rect );	//オリジナルのサイズゲット

	GetDlgItem( IDC_ICSDISPCTRL2 )->GetWindowRect( &m_disp2_rect );	//オリジナルのサイズゲット
	disp_caution( FALSE );	// 注意メッセージのサイズをゼロに。

	GetDlgItem( IDC_ICSSELCTRL1 )->GetClientRect( &m_selfix8_rect );	//オリジナルのサイズゲット

	// 枝番摘要表示コントロールのサイズをゼロ（見えなくする）に
	VERIFY(org = GetDlgItem( IDC_DISPETC ));
	org->SetWindowPos( 0, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOMOVE );

	// 選択タイトル背景色、文字色
	((CICSDisp*)GetDlgItem( IDC_VALTITLEBASE ))->SetBackColor( RGB_FIX8_BACK );
	((CICSDisp*)GetDlgItem( IDC_VALTITLE ))->SetBackColor( RGB_FIX8_BACK );
	((CICSDisp*)GetDlgItem( IDC_VALTITLE ))->SetForeColor( RGB_VAL32_FORE );

	// 選択タイトル用ＤＩＳＰコントロールのサイズをゼロ（見えなくする）に
	WINDOWPLACEMENT place;
	RECT rct;
	VERIFY(org = GetDlgItem( IDC_ICSSELCTRL1 ));
	org->GetWindowPlacement( &place );
	rct = place.rcNormalPosition;

	VERIFY(org = GetDlgItem( IDC_VALTITLEBASE));
# ifndef CLOSE //#* 08.06 /02
	org->SetWindowPos( 0, rct.left, rct.top, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE );
# else
	org->SetWindowPos( 0, rct.left, rct.top, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER );
# endif

	VERIFY(org = GetDlgItem( IDC_VALTITLE));
# ifndef CLOSE //#* 08.06 /02
	org->SetWindowPos( 0, rct.left, rct.top, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE );
# else
	org->SetWindowPos( 0, rct.left, rct.top, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER );
# endif


//	ControlInit( 510, 292, -1 );

	ICSDialog::OnInitDialog();
	
	//コントロールの表示順序
//	ctrldspjun( ThisCtrl, sizeof(ThisCtrl)/sizeof(ThisCtrl[0]) );

	// コントロールテーブルの初期化
	inpctlline_inz( INP_CTL );

	// 消費税セット
	def_syselinz();

	// 摘要文字数
	sprintf_s( pbuf, sizeof pbuf, "摘要(%d):", Voln1->tk_ln );
	GetDlgItem( IDC_ST_TKY )->SetWindowText( pbuf );

	// ｲﾒｰｼﾞか通常の文字かの選択
	struct _SCNTKY_SEL {
		char*	string;
		long	sign;
	} scntky_sel[] = {

		{ "文字入力",		STK_NORMAL },
		{ "イメージ摘要",	STK_IMG },
		{ "文字摘要のみ",	STK_STR },
		{ "摘要未入力",		STK_STRNONE }
	};

	int i, n;
	for( i = n = 0; i < (sizeof scntky_sel / sizeof scntky_sel[0]); i++ ) {
		if( IMG_master == ERR ) {
			if( i == 1 || i == 2 )
				continue;
		}
		m_Tkycb.AddString( scntky_sel[i].string );
		m_Tkycb.SetItemData( n, scntky_sel[i].sign );
		n++;
	}
	GetDlgItem( IDC_IMGTKYEDIT )->SetWindowText( "＜イメージ摘要＞" );
	// ｲﾒｰｼﾞ摘要のｴﾃﾞｨｯﾄﾎﾞｯｸｽを見えなくする
	disp_imgtkyedit( STK_NORMAL );
	
	m_Hcbo.InitHusenCombo();

	m_DIALOG_OK = TRUE;

	SelPar par;
	par.sel_pn = SL_KAMOKU_PN;
	m_Seldata.SelectJob( &par, -1 );

	// 消費税マスターごとの設定部分
	set_syselinz();
	keyline_dsp( INP_CTL, &SCAN_KEY );

	// 入力漏れ
	m_NipBrn.AddString("");
	m_NipBrn.AddString("枝番");
	if( (m_pZm->zvol->sub_sw&0x02) )	m_NipBrn.AddString("部門");

	set_nowpn( SCN_ORDER_PN );	// 選択項目背景色消去のため
	// イメージ摘要エディットOFF
	disp_imgtkyedit( SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_type );

	ICSDialog::OnInitDialogEX();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// 消費税 常に同じ設定の表示
void CScanKey::def_syselinz()
{
	struct _ZMSGBIND* zmb;

	zmb = &ZMSGBND[SWKKB_PG];
	int i;
	for( i = 0; i < zmb->n_zmsg; i++ ) {
		m_Syzsw.AddString( zmb->zmsg[i].zeitem );
		m_Syzsw.SetItemData( i, zmb->zmsg[i].zeifnc );
	}

	zmb = &ZMSGBND[ZRITU_PG];
	for( i = 0; i < zmb->n_zmsg; i++ ) {
		m_Zritu.AddString( zmb->zmsg[i].zeitem );
		m_Zritu.SetItemData( i, zmb->zmsg[i].zeifnc );
	}

	zmb = &ZMSGBND[SKBN_PG];
	for( i = 0; i < zmb->n_zmsg; i++ ) {
		m_Skbn.AddString( zmb->zmsg[i].zeitem );
		m_Skbn.SetItemData( i, zmb->zmsg[i].zeifnc );
	}
}


/*
#define NORMAL_MST		0
#define NORMAL_SYMST	1
#define KANI_SYMST		2
#define KOBET_SYMST		3
*/

// 消費税選択関係イニシャライズ
void CScanKey::set_syselinz()
{
	struct _ZMSGBIND* zmb;
	int i, tp;

	if( m_Sy.IsSyohizeiMaster() )
	{
		if( m_Sy.IsKaniMaster() )
			tp = KANI_SYMST;
		else if( m_Sy.IsKobetsuMaster() )
			tp = KOBET_SYMST;
		else
			tp = NORMAL_SYMST;
	}
	else
		tp = NORMAL_MST;

	m_Urisire.ResetContent();
	switch( tp ) {
	case KANI_SYMST:
		GetDlgItem(IDC_ST_URISIRE)->SetWindowText( "売上区分：" );
		zmb = &ZMSGBND[GYOSYU_PG];
		for( i = 0; i < zmb->n_zmsg; i++ ) {
			m_Urisire.AddString( zmb->zmsg[i].zeitem );
			m_Urisire.SetItemData( i, zmb->zmsg[i].zeifnc );
		}
		m_Urisire.SetCurSel(0);
		scGetINP_CTL( SCN_URISIRE_PN )->INP_ena = TRUE;
		break;
	case KOBET_SYMST:
		GetDlgItem(IDC_ST_URISIRE)->SetWindowText( "仕入区分：" );
		zmb = &ZMSGBND[SIRE_PG];
		for( i = 0; i < zmb->n_zmsg; i++ ) {
			m_Urisire.AddString( zmb->zmsg[i].zeitem );
			m_Urisire.SetItemData( i, zmb->zmsg[i].zeifnc );
		}
		m_Urisire.SetCurSel(0);
		scGetINP_CTL( SCN_URISIRE_PN )->INP_ena = TRUE;
		break;
	case NORMAL_MST:
	case NORMAL_SYMST:
	default:
		GetDlgItem(IDC_ST_URISIRE)->SetWindowText( "－－－－：" );
		GetDlgItem(IDC_ST_URISIRE)->EnableWindow(FALSE);
		m_Urisire.EnableWindow(FALSE);
		scGetINP_CTL( SCN_URISIRE_PN )->INP_ena = FALSE;
		m_Urisire.SetCurSel(-1);
		break;
	}
}


/*=======================
	FIX 8科目選択
	科目・摘要32項目選択
		本体
========================*/
void CScanKey::SelectedIcsselctrl(short position, long Number, int place ) 
{
	int pn;
	CDBipTKREC tkrec;
	CDBipDataRec tmpdt;	// 部門コード取得用
	struct _ItemData dmy_data = {0};
	int dcsw;
	struct _ItemData check={0}, ans={0}, *pKey;
	char temp[10];
#ifdef _ICSPEN_	// 01.20 /01
	CWnd *nextkey = 0;
#endif

	pn = get_nowpn();

	if( pn == SCN_DEBT_PN || pn == SCN_CRED_PN || pn == SCN_DBCR_PN )
	{
		switch( pn )
		{
		case SCN_DEBT_PN:
			dcsw = 0;
			check = SCAN_KEY.SCN_DEBT_DT;
			pKey = &SCAN_KEY.SCN_DEBT_DT;
			break;
		case SCN_CRED_PN:
			dcsw = 1;
			check = SCAN_KEY.SCN_CRED_DT;
			pKey = &SCAN_KEY.SCN_CRED_DT;
			break;
		case SCN_DBCR_PN:
			dcsw = 0;
			check = SCAN_KEY.SCN_DBCR_DT;
			pKey = &SCAN_KEY.SCN_DBCR_DT;
			break;
		}
	//	check.IP_CHKCdNum = (int)Number;
	//	check.IP_CHKCdNumSg = 1;
		// 科目ｺｰﾄﾞ or 番号 02.20/02
		if( bKamokuCode ) {
			sprintf_s( check.IP_stKMK.kd_code, sizeof check.IP_stKMK.kd_code, "%d", Number );
			check.IP_stKMK.kd_sgn = 0;
		}
		else {
			sprintf_s( check.IP_stKMK.kd_code, sizeof check.IP_stKMK.kd_code, "%d", Number );	// 番号
			check.IP_stKMK.kd_sgn = 1;
		}

		if( InputItemCheck( ans.IP_stKMK, check.IP_stKMK, dcsw, TRUE ) == TRUE )
		{

			DBkamokudata_set( this, scGetINP_CTL(pn)->IDC_X, &ans.IP_stKMK, m_pZm );
			//データセット
			memmove( pKey, &ans, sizeof(struct _ItemData));
			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;

#ifdef _ICSPEN_	// 01.20 /01
			// 選択後次へ移動
			if( flgPen != FALSE )
			{
				nextkey = GetDlgItem( scGetINP_CTL( pn )->IDC_X );
			}
#endif
		}
	}
	else if( BRNTKY_sel && (pn == SCN_DBR1_PN || pn == SCN_DBR2_PN || pn == SCN_CBR1_PN || pn == SCN_CBR2_PN || pn == SCN_DCBR1_PN || pn == SCN_DCBR2_PN) )
	{
#ifdef _ICSPEN_	// 01.20 /01
		
		BOOL sw = brnsel_set( pn, Number );

		if( flgPen != FALSE )
			if( sw )
			{
				// 選択後次へ移動
				nextkey = GetDlgItem( scGetINP_CTL( pn )->IDC_X );
			}
#else
		brnsel_set( pn, Number );
#endif
	}
#ifndef CLOSE	// 10.21 /98
	else if( pn == SCN_TKY_PN && place )
#else
	else if( pn == SCN_TKY_PN )
#endif
	{
		if( !is_mdfy_imgtky() ) //#* 06.19 /01  ｲﾒｰｼﾞ摘要検索でない時
		{
			//摘要選択
			sprintf_s( temp, sizeof temp, "%d", Number );
			if( /*((CDBDinpView*)m_pParent)->*/tky_select( &tkrec, (char *)temp ) != -1 )
			{
				//表示
				tky_select_dsp( &tkrec, FALSE );
			}
		}
	}
	else if( pn == SCN_BMON1_PN || pn == SCN_BMON2_PN || 
			 pn == SCN_CBMN1_PN || pn == SCN_CBMN2_PN	) {	//#* 02.27 /02
		// 部門選択処理
		switch( pn ) {
		case SCN_BMON1_PN:
			pKey = &SCAN_KEY.SCN_BMON1_DT;
			break;
		case SCN_BMON2_PN:
			pKey = &SCAN_KEY.SCN_BMON2_DT;
			break;
		case SCN_CBMN1_PN:
			pKey = &SCAN_KEY.SCN_CBMN1_DT;
			break;
		case SCN_CBMN2_PN:
			pKey = &SCAN_KEY.SCN_CBMN2_DT;
			break;
		}
		int bmn;
		if( bmncode_todata( &bmn, Number, 0 ) != FALSE ) {
			pKey->IP_BMN = bmn;
			bmon_select_dsp( pn, pKey, TRUE );			
		}
	}

	set_focus( pn );

#ifdef _ICSPEN_	// 01.20 /01
	if( nextkey )
	{
		// 選択後次へ移動
		nextkey->PostMessage( WM_KEYDOWN, VK_TAB );
	}
#endif

}


//#* 部門を表示 ... 02.27 /02
void CScanKey::bmon_select_dsp( int pn, struct _ItemData *keybuf, BOOL bTerm )
{
	struct _ItemData check={0};
	char	tmp[30] = {0};
	VARIANT var;

	check.IP_BMN = keybuf->IP_BMN;

	set_codestr( tmp, sizeof tmp, check.IP_BMN, Voln1->bmcol );

	var.pbVal = (BYTE*)tmp;
	DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

	if( bTerm ) {
#ifdef	_ICSPEN_
		if( flgPen != FALSE )
		{
			//選択後次へ移動
			GetDlgItem( scGetINP_CTL( pn )->IDC_X )->PostMessage( WM_KEYDOWN, VK_TAB );
		}
		else
		{
			//データセットのためのダミーフォーカスＯＦＦ
			BmonInp( pn, keybuf, 0, 0, 0 );
		}
#else
		BmonInp( pn, keybuf, 0, 0, 0 );
#endif
	}	
}




/**************************************************
	keyline_dataset( SCAN_ITEM* pSI )
	入力ラインバッファ にデータセット
***************************************************/
void CScanKey::keyline_dataset( SCAN_ITEM* pSI )
{
	int i;

	for( i = SCN_ORDER_PN ; i <= SCN_NOINP_PN ; ++i ) {
		//コンディションサイン OFF
		memset( (char *)&SCAN_KEY.LINE_CND[i], (char)0, sizeof(SCAN_KEY.LINE_CND[i]) );
	}

	for( i = SCN_ORDER_PN ; i <= SCN_NOINP_PN ; ++i )
	{

		switch( i )
		{
		case SCN_ORDER_PN:
			memset( (char *)SCAN_KEY.SCN_ORDER_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_ORDER_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_ORDER_DT.SKBN_SGN1 = 0;
			break;
		case SCN_DUSER_PN:
			memset( (char *)SCAN_KEY.SCN_DUSER_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_DUSER_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_DUSER_DT.SKBN_SGN1 = 0;
			break;
		case SCN_STYPE_PN:
			memset( (char *)SCAN_KEY.SCN_STYPE_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_STYPE_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_STYPE_DT.SKBN_SGN1 = 0;
			break;
		
		case SCN_SEQ1_PN:
			SCAN_KEY.SCN_SEQ1_DT.IP_SEQ = pSI->sc_seq1;
			SCAN_KEY.SCN_SEQ2_DT.IP_SEQ = pSI->sc_seq2;

			if( pSI->sc_seq1 > 0 )	SCAN_KEY.LINE_CND[SCN_SEQ1_PN].INP_sg = 1;
			if( pSI->sc_seq2 > 0 )	SCAN_KEY.LINE_CND[SCN_SEQ2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_SEQ1_PN].KEY_inp = (char)pSI->sc_seqopt;
			break;
		case SCN_DATE1_PN:
			SCAN_KEY.SCN_DATE1_DT.IP_DAY = SCAN_KEY.SCN_DATE2_DT.IP_DAY = 0;
			memset( (char *)&SCAN_KEY.SCN_DATE1_DT.IP_BCDDAY, (char)0, sizeof(SCAN_KEY.SCN_DATE1_DT.IP_BCDDAY) );
			memset( (char *)&SCAN_KEY.SCN_DATE2_DT.IP_BCDDAY, (char)0, sizeof(SCAN_KEY.SCN_DATE2_DT.IP_BCDDAY) );
			memcpy( (char *)&SCAN_KEY.SCN_DATE1_DT.IP_BCDDAY, pSI->sc_date1, 2);
			memcpy( (char *)&SCAN_KEY.SCN_DATE2_DT.IP_BCDDAY, pSI->sc_date2, 2);

			if( pSI->sc_date1[0] )	SCAN_KEY.LINE_CND[SCN_DATE1_PN].INP_sg = 1;
			if( pSI->sc_date2[0] )	SCAN_KEY.LINE_CND[SCN_DATE2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_DATE1_PN].KEY_inp = (char)pSI->sc_dateopt;
			break;
		case SCN_DENP1_PN:
			SCAN_KEY.SCN_DENP1_DT.IP_DENP = pSI->sc_denp1;
			SCAN_KEY.SCN_DENP2_DT.IP_DENP = pSI->sc_denp2;

			if( pSI->sc_denp1 >= 0 )	SCAN_KEY.LINE_CND[SCN_DENP1_PN].INP_sg = 1;
			if( pSI->sc_denp2 >= 0 )	SCAN_KEY.LINE_CND[SCN_DENP2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_DENP1_PN].KEY_inp = (char)pSI->sc_denpopt;
			break;
		case SCN_BMON1_PN:
			SCAN_KEY.SCN_BMON1_DT.IP_BMN = pSI->sc_dbmn1;
			SCAN_KEY.SCN_BMON2_DT.IP_BMN = pSI->sc_dbmn2;

			if( pSI->sc_dbmn1 >= 0 )	SCAN_KEY.LINE_CND[SCN_BMON1_PN].INP_sg = 1;
			if( pSI->sc_dbmn2 >= 0 )	SCAN_KEY.LINE_CND[SCN_BMON2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_BMON1_PN].KEY_inp = (char)pSI->sc_dbmnopt;
			break;
		case SCN_CBMN1_PN:
			SCAN_KEY.SCN_CBMN1_DT.IP_BMN = pSI->sc_cbmn1;
			SCAN_KEY.SCN_CBMN2_DT.IP_BMN = pSI->sc_cbmn2;

			if( pSI->sc_cbmn1 >= 0 )	SCAN_KEY.LINE_CND[SCN_CBMN1_PN].INP_sg = 1;
			if( pSI->sc_cbmn2 >= 0 )	SCAN_KEY.LINE_CND[SCN_CBMN2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_CBMN1_PN].KEY_inp = (char)pSI->sc_cbmnopt;
			break;
		case SCN_KOJI1_PN:
			::ZeroMemory( &SCAN_KEY.SCN_KOJI1_DT.IP_KOJI, sizeof SCAN_KEY.SCN_KOJI1_DT.IP_KOJI );
			::ZeroMemory( &SCAN_KEY.SCN_KOJI2_DT.IP_KOJI, sizeof SCAN_KEY.SCN_KOJI2_DT.IP_KOJI );
			memcpy( &SCAN_KEY.SCN_KOJI1_DT.IP_KOJI, pSI->sc_dkji1, sizeof pSI->sc_dkji1 );
			memcpy( &SCAN_KEY.SCN_KOJI2_DT.IP_KOJI, pSI->sc_dkji2, sizeof pSI->sc_dkji2 );

			if( pSI->sc_dkji1[0] )	SCAN_KEY.LINE_CND[SCN_KOJI1_PN].INP_sg = 1;
			if( pSI->sc_dkji2[0] )	SCAN_KEY.LINE_CND[SCN_KOJI2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_KOJI1_PN].KEY_inp = (char)pSI->sc_dkjiopt;
			break;
		case SCN_CKJI1_PN:
			::ZeroMemory( &SCAN_KEY.SCN_CKJI1_DT.IP_KOJI, sizeof SCAN_KEY.SCN_CKJI1_DT.IP_KOJI );
			::ZeroMemory( &SCAN_KEY.SCN_CKJI2_DT.IP_KOJI, sizeof SCAN_KEY.SCN_CKJI2_DT.IP_KOJI );
			memcpy( &SCAN_KEY.SCN_CKJI1_DT.IP_KOJI, pSI->sc_ckji1, sizeof pSI->sc_ckji1 );
			memcpy( &SCAN_KEY.SCN_CKJI2_DT.IP_KOJI, pSI->sc_ckji2, sizeof pSI->sc_ckji2 );

			if( pSI->sc_ckji1[0] )	SCAN_KEY.LINE_CND[SCN_CKJI1_PN].INP_sg = 1;
			if( pSI->sc_ckji2[0] )	SCAN_KEY.LINE_CND[SCN_CKJI2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_CKJI1_PN].KEY_inp = (char)pSI->sc_ckjiopt;
			break;
		case SCN_DEBT_PN:
			memset( &SCAN_KEY.SCN_DEBT_DT, '\0', sizeof SCAN_KEY.SCN_DEBT_DT );
			memcpy( SCAN_KEY.SCN_DEBT_DT.IP_CdNum, pSI->sc_dbt, sizeof pSI->sc_dbt );
			SCAN_KEY.SCN_DEBT_DT.IP_BRN = -1;

			if( pSI->sc_dbt[0] )	SCAN_KEY.LINE_CND[SCN_DEBT_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_DEBT_PN].KEY_inp = (char)pSI->sc_dbtopt;
			break;
		case SCN_DBR1_PN:
			SCAN_KEY.SCN_DBR1_DT.IP_cBRN = pSI->sc_dbr1;
			SCAN_KEY.SCN_DBR2_DT.IP_cBRN = pSI->sc_dbr2;

			if( pSI->sc_dbr1 >= 0 )	SCAN_KEY.LINE_CND[SCN_DBR1_PN].INP_sg = 1;
			if( pSI->sc_dbr2 >= 0 )	SCAN_KEY.LINE_CND[SCN_DBR2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_DBR1_PN].KEY_inp = (char)pSI->sc_dbropt;
			break;
		case SCN_CRED_PN:
			memset( &SCAN_KEY.SCN_CRED_DT, '\0', sizeof SCAN_KEY.SCN_CRED_DT );
			memcpy( SCAN_KEY.SCN_CRED_DT.IP_CdNum, pSI->sc_cre, sizeof pSI->sc_cre );
			SCAN_KEY.SCN_CRED_DT.IP_BRN = -1;
			if( pSI->sc_cre[0] )	SCAN_KEY.LINE_CND[SCN_CRED_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_CRED_PN].KEY_inp = (char)pSI->sc_creopt;
			break;
		case SCN_CBR1_PN:
			SCAN_KEY.SCN_CBR1_DT.IP_cBRN = pSI->sc_cbr1;
			SCAN_KEY.SCN_CBR2_DT.IP_cBRN = pSI->sc_cbr2;

			if( pSI->sc_cbr1 >= 0 )	SCAN_KEY.LINE_CND[SCN_CBR1_PN].INP_sg = 1;
			if( pSI->sc_cbr2 >= 0 )	SCAN_KEY.LINE_CND[SCN_CBR2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_CBR1_PN].KEY_inp = (char)pSI->sc_cbropt;
			break;
		case SCN_DBCR_PN:
			memset( &SCAN_KEY.SCN_DBCR_DT, '\0', sizeof SCAN_KEY.SCN_DBCR_DT );
			memcpy( SCAN_KEY.SCN_DBCR_DT.IP_CdNum, pSI->sc_dbcr, sizeof pSI->sc_dbcr );
			SCAN_KEY.SCN_DBCR_DT.IP_BRN = -1;

			if( pSI->sc_dbcr[0] )	SCAN_KEY.LINE_CND[SCN_DBCR_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_DBCR_PN].KEY_inp = (char)pSI->sc_dbcropt;
			break;
		case SCN_DCBR1_PN:
			SCAN_KEY.SCN_DCBR1_DT.IP_cBRN = pSI->sc_dcbrn1;
			SCAN_KEY.SCN_DCBR2_DT.IP_cBRN = pSI->sc_dcbrn2;
	
			if( pSI->sc_dcbrn1 >= 0 )	SCAN_KEY.LINE_CND[SCN_DCBR1_PN].INP_sg = 1;
			if( pSI->sc_dcbrn2 >= 0 )	SCAN_KEY.LINE_CND[SCN_DCBR2_PN].INP_sg = 1;			
			SCAN_KEY.LINE_CND[SCN_DCBR1_PN].KEY_inp = (char)pSI->sc_dcbropt;
			break;
		case SCN_VAL1_PN:
			memcpy( (char *)&SCAN_KEY.SCN_VAL1_DT, pSI->sc_val1, sizeof pSI->sc_val1 );
			memcpy( (char *)&SCAN_KEY.SCN_VAL2_DT, pSI->sc_val2, sizeof pSI->sc_val2 );
			SCAN_KEY.LINE_CND[SCN_VAL1_PN].INP_sg = pSI->sc_valsgn[0];
			SCAN_KEY.LINE_CND[SCN_VAL2_PN].INP_sg = pSI->sc_valsgn[1];
			SCAN_KEY.LINE_CND[SCN_VAL1_PN].KEY_inp = (char)pSI->sc_valopt;
			break;
		case SCN_ZEI1_PN:
			memcpy( (char *)&SCAN_KEY.SCN_ZEI1_DT, pSI->sc_zei1, sizeof pSI->sc_zei1 );
			memcpy( (char *)&SCAN_KEY.SCN_ZEI2_DT, pSI->sc_zei2, sizeof pSI->sc_zei2 );
			SCAN_KEY.LINE_CND[SCN_ZEI1_PN].INP_sg = pSI->sc_zeisgn[0];
			SCAN_KEY.LINE_CND[SCN_ZEI2_PN].INP_sg = pSI->sc_zeisgn[1];
			SCAN_KEY.LINE_CND[SCN_ZEI1_PN].KEY_inp = (char)pSI->sc_zeiopt;
			break;
		case SCN_SYZSW_PN:
			memset( (char *)SCAN_KEY.SCN_SYZSW_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_SYZSW_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_SYZSW_DT.SKBN_SGN1 = pSI->sc_syzsw;
			if( pSI->sc_syzsw > SCZ_NONSELECT ) SCAN_KEY.LINE_CND[SCN_SYZSW_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_SYZSW_PN].KEY_inp = (char)pSI->sc_syzswopt;
			break;
		case SCN_ZRITU_PN:
			memset( (char *)SCAN_KEY.SCN_ZRITU_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_ZRITU_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_ZRITU_DT.SKBN_SGN1 = pSI->sc_zritu;
			if( pSI->sc_zritu > SCZ_NONSELECT ) SCAN_KEY.LINE_CND[SCN_ZRITU_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_ZRITU_PN].KEY_inp = (char)pSI->sc_zrituopt;
			break;
		case SCN_SKBN_PN:
			memset( (char *)SCAN_KEY.SCN_SKBN_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_SKBN_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_SKBN_DT.SKBN_SGN1 = pSI->sc_skbn;
			if( pSI->sc_skbn > SCZ_NONSELECT ) SCAN_KEY.LINE_CND[SCN_SKBN_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_SKBN_PN].KEY_inp = (char)pSI->sc_skbnopt;
			break;
		case SCN_URISIRE_PN:
			memset( (char *)SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_URISIRE_DT.SKBN_SGN1 = pSI->sc_urisire;
			if( pSI->sc_urisire > SCZ_NONSELECT ) SCAN_KEY.LINE_CND[SCN_URISIRE_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_URISIRE_PN].KEY_inp = (char)pSI->sc_urisireopt;
			break;
		case SCN_TKY_PN:
			memcpy( SCAN_KEY.SCN_TKY_DT.IP_TKY, pSI->sc_tky, sizeof pSI->sc_tky );
			if( pSI->sc_tky[0] ) SCAN_KEY.LINE_CND[SCN_TKY_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_TKY_PN].KEY_inp = (char)pSI->sc_tkyopt;
			break;
		case SCN_SNUM1_PN:
			memset( SCAN_KEY.SCN_SNUM1_DT.IP_SNUM, 0, sizeof SCAN_KEY.SCN_SNUM1_DT.IP_SNUM );
			memset( SCAN_KEY.SCN_SNUM2_DT.IP_SNUM, 0, sizeof SCAN_KEY.SCN_SNUM2_DT.IP_SNUM );
			memcpy( SCAN_KEY.SCN_SNUM1_DT.IP_SNUM, pSI->sc_snum1, sizeof pSI->sc_snum1 );
			memcpy( SCAN_KEY.SCN_SNUM2_DT.IP_SNUM, pSI->sc_snum2, sizeof pSI->sc_snum2 );

			if( pSI->sc_snum1[0] ) SCAN_KEY.LINE_CND[SCN_SNUM1_PN].INP_sg = 1;
			if( pSI->sc_snum2[0] ) SCAN_KEY.LINE_CND[SCN_SNUM2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_SNUM2_PN].KEY_inp = (char)pSI->sc_snumopt;
			break;
		case SCN_TDAT1_PN:
			SCAN_KEY.SCN_TDAT1_DT.IP_TGDAY = pSI->sc_tgdate1;
			SCAN_KEY.SCN_TDAT2_DT.IP_TGDAY = pSI->sc_tgdate2;
			memset( (char *)&SCAN_KEY.SCN_TDAT1_DT.IP_TGBCD, (char)0, sizeof(SCAN_KEY.SCN_TDAT1_DT.IP_TGBCD) );
			memset( (char *)&SCAN_KEY.SCN_TDAT2_DT.IP_TGBCD, (char)0, sizeof(SCAN_KEY.SCN_TDAT2_DT.IP_TGBCD) );

			if( pSI->sc_tgdate1 > 0 ) SCAN_KEY.LINE_CND[SCN_TDAT1_PN].INP_sg = 1;
			if( pSI->sc_tgdate2 > 0 ) SCAN_KEY.LINE_CND[SCN_TDAT2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_TDAT1_PN].KEY_inp = (char)pSI->sc_tgdtopt;
			break;
		case SCN_HUSEN_PN:
			memset( (char *)SCAN_KEY.SCN_HUSEN_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_HUSEN_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_HUSEN_DT.SKBN_SGN1 = pSI->sc_husen;

			if( pSI->sc_husen > SCZ_NONSELECT ) SCAN_KEY.LINE_CND[SCN_HUSEN_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_HUSEN_PN].KEY_inp = (char)pSI->sc_husenopt;
			break;

		case SCN_NOINP_PN:
			memset( (char *)SCAN_KEY.SCN_NOINP_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_NOINP_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_NOINP_DT.SKBN_SGN1 = pSI->sc_noinp;

			if( pSI->sc_noinp > 0 ) SCAN_KEY.LINE_CND[SCN_NOINP_PN].INP_sg = 1;
			break;

		default:
			break;
		}
	}

}



/**************************************
		keyline_inz()
	入力ラインバッファイニシャライズ
***************************************/
void CScanKey::keyline_inz()
{
	int i;

#ifdef SPEECH	// 06.06 /00
	ppclr_tky( _RETREV );
#endif

	// スキャンオンリー時
	if( bSCAN_ONLY_MODE )
		GetDlgItem( IDCANCEL )->SetWindowText( "終了(End)" );

//	set_focus( SCN_SEQ1_PN );	// initial focus 
	set_topfocus();	// initial focus 

	for( i = SCN_ORDER_PN ; i <= SCN_NOINP_PN ; ++i )
	{
		switch( i )
		{
		case SCN_ORDER_PN:
			memset( (char *)SCAN_KEY.SCN_ORDER_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_ORDER_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_ORDER_DT.SKBN_SGN1 = 0;
			break;
		case SCN_DUSER_PN:
			memset( (char *)SCAN_KEY.SCN_DUSER_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_DUSER_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_DUSER_DT.SKBN_SGN1 = 0;
			break;
		case SCN_STYPE_PN:
			memset( (char *)SCAN_KEY.SCN_STYPE_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_STYPE_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_STYPE_DT.SKBN_SGN1 = 0;
			break;

		case SCN_SEQ1_PN:
			SCAN_KEY.SCN_SEQ1_DT.IP_SEQ = SCAN_KEY.SCN_SEQ2_DT.IP_SEQ = 0;
			break;
		case SCN_DATE1_PN:
			SCAN_KEY.SCN_DATE1_DT.IP_DAY = SCAN_KEY.SCN_DATE2_DT.IP_DAY = 0;
			memset( (char *)&SCAN_KEY.SCN_DATE1_DT.IP_BCDDAY, (char)0, sizeof(SCAN_KEY.SCN_DATE1_DT.IP_BCDDAY) );
			memset( (char *)&SCAN_KEY.SCN_DATE2_DT.IP_BCDDAY, (char)0, sizeof(SCAN_KEY.SCN_DATE2_DT.IP_BCDDAY) );
			break;
		case SCN_DENP1_PN:
			SCAN_KEY.SCN_DENP1_DT.IP_DENP = SCAN_KEY.SCN_DENP2_DT.IP_DENP = -1;
			break;
		case SCN_BMON1_PN:
			SCAN_KEY.SCN_BMON1_DT.IP_BMN = SCAN_KEY.SCN_BMON2_DT.IP_BMN = -1;
			((CButton*)GetDlgItem(IDC_CHECK_DBM))->SetCheck(0);
			SendMessage(WM_COMMAND, MAKELONG(IDC_CHECK_DBM, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_DBM)->m_hWnd);
			break;
		case SCN_CBMN1_PN:
			SCAN_KEY.SCN_CBMN1_DT.IP_BMN = SCAN_KEY.SCN_CBMN2_DT.IP_BMN = -1;
			((CButton*)GetDlgItem(IDC_CHECK_CBM))->SetCheck(0);
			SendMessage(WM_COMMAND, MAKELONG(IDC_CHECK_CBM, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_CBM)->m_hWnd);
			break;
		case SCN_KOJI1_PN:
			::ZeroMemory( &SCAN_KEY.SCN_KOJI1_DT.IP_KOJI, sizeof SCAN_KEY.SCN_KOJI1_DT.IP_KOJI );
			::ZeroMemory( &SCAN_KEY.SCN_KOJI2_DT.IP_KOJI, sizeof SCAN_KEY.SCN_KOJI2_DT.IP_KOJI );
			((CButton*)GetDlgItem(IDC_CHECK_DKJ))->SetCheck(0);
			SendMessage(WM_COMMAND, MAKELONG(IDC_CHECK_DKJ, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_DKJ)->m_hWnd);
			break;
		case SCN_CKJI1_PN:
			::ZeroMemory( &SCAN_KEY.SCN_CKJI1_DT.IP_KOJI, sizeof SCAN_KEY.SCN_CKJI1_DT.IP_KOJI );
			::ZeroMemory( &SCAN_KEY.SCN_CKJI2_DT.IP_KOJI, sizeof SCAN_KEY.SCN_CKJI2_DT.IP_KOJI );
			((CButton*)GetDlgItem(IDC_CHECK_CKJ))->SetCheck(0);
			SendMessage(WM_COMMAND, MAKELONG(IDC_CHECK_CKJ, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_CKJ)->m_hWnd);
			break;

		case SCN_DEBT_PN:
			memset( &SCAN_KEY.SCN_DEBT_DT, '\0', sizeof SCAN_KEY.SCN_DEBT_DT );
			break;
		case SCN_DBR1_PN:
			SCAN_KEY.SCN_DBR1_DT.IP_cBRN = SCAN_KEY.SCN_DBR2_DT.IP_cBRN = -1;
			((CButton*)GetDlgItem(IDC_CHECK_DBR))->SetCheck(0);
			SendMessage(WM_COMMAND, MAKELONG(IDC_CHECK_DBR, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_DBR)->m_hWnd);
			break;
		case SCN_CRED_PN:
			memset( &SCAN_KEY.SCN_CRED_DT, '\0', sizeof SCAN_KEY.SCN_CRED_DT );
			break;
		case SCN_CBR1_PN:
			SCAN_KEY.SCN_CBR1_DT.IP_cBRN = SCAN_KEY.SCN_CBR2_DT.IP_cBRN = -1;
			//#* 02.26 /03
			((CButton*)GetDlgItem(IDC_CHECK_CBR))->SetCheck(0);
			SendMessage(WM_COMMAND, MAKELONG(IDC_CHECK_CBR, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_CBR)->m_hWnd);
			break;
		case SCN_DBCR_PN:
			memset( &SCAN_KEY.SCN_DBCR_DT, '\0', sizeof SCAN_KEY.SCN_DBCR_DT );
			break;
		case SCN_DCBR1_PN:
			SCAN_KEY.SCN_DCBR1_DT.IP_cBRN = SCAN_KEY.SCN_DCBR2_DT.IP_cBRN = -1;
			break;
		case SCN_VAL1_PN:
			memset( (char *)&SCAN_KEY.SCN_VAL1_DT, (char)0, sizeof(SCAN_KEY.SCN_VAL1_DT) );
			memset( (char *)&SCAN_KEY.SCN_VAL2_DT, (char)0, sizeof(SCAN_KEY.SCN_VAL1_DT) );
			break;
		case SCN_ZEI1_PN:
			memset( (char *)&SCAN_KEY.SCN_ZEI1_DT, (char)0, sizeof(SCAN_KEY.SCN_ZEI1_DT) );
			memset( (char *)&SCAN_KEY.SCN_ZEI2_DT, (char)0, sizeof(SCAN_KEY.SCN_ZEI2_DT) );
			break;
		case SCN_SYZSW_PN:
			memset( (char *)SCAN_KEY.SCN_SYZSW_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_SYZSW_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_SYZSW_DT.SKBN_SGN1 = NON_SELECT;
			break;
		case SCN_ZRITU_PN:
			memset( (char *)SCAN_KEY.SCN_ZRITU_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_ZRITU_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_ZRITU_DT.SKBN_SGN1 = NON_SELECT;
			break;
		case SCN_SKBN_PN:
			memset( (char *)SCAN_KEY.SCN_SKBN_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_SKBN_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_SKBN_DT.SKBN_SGN1 = NON_SELECT;
			break;
		case SCN_URISIRE_PN:
			memset( (char *)SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_URISIRE_DT.SKBN_SGN1 = NON_SELECT;
			break;
		case SCN_TKY_PN:
			SCAN_KEY.SCN_TKY_DT.IP_TKY[0] = '\0';
			break;
		case SCN_SNUM1_PN:
			memset( SCAN_KEY.SCN_SNUM1_DT.IP_SNUM, 0, sizeof SCAN_KEY.SCN_SNUM1_DT.IP_SNUM );
			memset( SCAN_KEY.SCN_SNUM2_DT.IP_SNUM, 0, sizeof SCAN_KEY.SCN_SNUM2_DT.IP_SNUM );
			break;
		case SCN_TDAT1_PN:
			SCAN_KEY.SCN_TDAT1_DT.IP_TGDAY = SCAN_KEY.SCN_TDAT2_DT.IP_TGDAY = 0;
			memset( (char *)&SCAN_KEY.SCN_TDAT1_DT.IP_TGBCD, (char)0, sizeof(SCAN_KEY.SCN_TDAT1_DT.IP_TGBCD) );
			memset( (char *)&SCAN_KEY.SCN_TDAT2_DT.IP_TGBCD, (char)0, sizeof(SCAN_KEY.SCN_TDAT2_DT.IP_TGBCD) );
			break;
		case SCN_HUSEN_PN:
			memset( (char *)SCAN_KEY.SCN_HUSEN_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_HUSEN_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_HUSEN_DT.SKBN_SGN1 = 0;
			break;
		case SCN_NOINP_PN:
			memset( (char *)SCAN_KEY.SCN_NOINP_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_NOINP_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_NOINP_DT.SKBN_SGN1 = 0;
			break;

		default:
			break;
		}
		//コンディションサイン OFF
		memset( (char *)&SCAN_KEY.LINE_CND[i], (char)0, sizeof(SCAN_KEY.LINE_CND[i]) );
	}

	keyline_dsp( INP_CTL, &SCAN_KEY );

	set_nowpn( SCN_ORDER_PN );	// 選択項目背景色消去のため

#ifndef CLOSE //#* 06.19 /01
	// イメージ摘要エディットOFF
	disp_imgtkyedit( SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_type );
//	selimg_btntxt( SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_type );
#endif
}

//１行表示
void CScanKey::keyline_dsp( struct _InpCtl *ctl, struct _SKEY_LINE *dtline )
{
	int i, chk;
	VARIANT var;
	char	buf[128];
	BYTE	bcd[30];
	CString str;

	for( i = SCN_ORDER_PN ; i <= SCN_HUSEN_PN ; ++i )
	{
	//	if( i > SCN_CBMN2_PN ) break;

		var.pbVal = NULL;
		switch( i )
		{
		case SCN_ORDER_PN:
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( 0 );
			break;
		case SCN_DUSER_PN:
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( 0 );
			break;
		case SCN_STYPE_PN:
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( 0 );
			break;

		case SCN_SEQ1_PN:
			if( dtline->SCN_SEQ1_DT.IP_SEQ > 0 ) {
				sprintf_s( buf, sizeof buf, "%d", dtline->SCN_SEQ1_DT.IP_SEQ );
				var.pbVal = (BYTE*)buf;
			}
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_SEQ_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_SEQ2_PN:
			if( dtline->SCN_SEQ2_DT.IP_SEQ > 0 ) {
				sprintf_s( buf, sizeof buf, "%d", dtline->SCN_SEQ2_DT.IP_SEQ );
				var.pbVal = (BYTE*)buf;
			}
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case SCN_DATE1_PN:
			::ZeroMemory( bcd, sizeof bcd );
			memcpy( bcd, dtline->SCN_DATE1_DT.IP_BCDDAY, sizeof dtline->SCN_DATE1_DT.IP_BCDDAY );
			var.pbVal = bcd;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 0 );
			BcdDateToString( str, dtline->SCN_DATE1_DT.IP_BCDDAY );
			DBSetDispString( this, (ctl+i)->IDC_X, str );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DATE_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_DATE2_PN:
			::ZeroMemory( bcd, sizeof bcd );
			memcpy( bcd, dtline->SCN_DATE2_DT.IP_BCDDAY, sizeof dtline->SCN_DATE2_DT.IP_BCDDAY );
			var.pbVal = bcd;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 0 );
			BcdDateToString( str, dtline->SCN_DATE2_DT.IP_BCDDAY );
			DBSetDispString( this, (ctl+i)->IDC_X, str );
			break;
		case SCN_DENP1_PN:
			if( dtline->SCN_DENP1_DT.IP_DENP != -1 ) {
				sprintf_s( buf, sizeof buf, "%d", dtline->SCN_DENP1_DT.IP_DENP );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DENP_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_DENP2_PN:
			if( dtline->SCN_DENP2_DT.IP_DENP != -1 ) {
				sprintf_s( buf, sizeof buf, "%d", dtline->SCN_DENP2_DT.IP_DENP );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case SCN_BMON1_PN:
			if( dtline->SCN_BMON1_DT.IP_BMN != -1 ) {
				set_codestr( buf, sizeof buf, dtline->SCN_BMON1_DT.IP_BMN, Voln1->bmcol );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DBMN_PN)->IDC_X ))->SetCheck(chk);
			chk = (dtline->LINE_CND[i].KEY_inp & SC_ITEMNONE) ? 1 : 0;
			((CButton*)GetDlgItem(IDC_CHECK_DBM))->SetCheck(chk);
			SendMessage(WM_COMMAND, MAKELONG(IDC_CHECK_DBM, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_DBM)->m_hWnd);
			break;
		case SCN_BMON2_PN:
			if( dtline->SCN_BMON2_DT.IP_BMN != -1 ) {
				set_codestr( buf, sizeof buf, dtline->SCN_BMON2_DT.IP_BMN, Voln1->bmcol );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case SCN_CBMN1_PN:
			if( dtline->SCN_CBMN1_DT.IP_BMN != -1 ) {
				set_codestr( buf, sizeof buf, dtline->SCN_CBMN1_DT.IP_BMN, Voln1->bmcol );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_CBMN_PN)->IDC_X ))->SetCheck(chk);
			chk = (dtline->LINE_CND[i].KEY_inp & SC_ITEMNONE) ? 1 : 0;
			((CButton*)GetDlgItem(IDC_CHECK_CBM))->SetCheck(chk);
			SendMessage(WM_COMMAND, MAKELONG(IDC_CHECK_CBM, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_CBM)->m_hWnd);
			break;
		case SCN_CBMN2_PN:
			if( dtline->SCN_CBMN2_DT.IP_BMN != -1 ) {
				set_codestr( buf, sizeof buf, dtline->SCN_CBMN2_DT.IP_BMN, Voln1->bmcol );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case SCN_KOJI1_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->SCN_KOJI1_DT.IP_KOJI );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DKJI_PN)->IDC_X ))->SetCheck(chk);
			chk = (dtline->LINE_CND[i].KEY_inp & SC_ITEMNONE) ? 1 : 0;
			((CButton*)GetDlgItem(IDC_CHECK_DKJ))->SetCheck(chk);
			SendMessage(WM_COMMAND, MAKELONG(IDC_CHECK_DKJ, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_DKJ)->m_hWnd);
			break;
		case SCN_KOJI2_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->SCN_KOJI2_DT.IP_KOJI );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case SCN_CKJI1_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->SCN_CKJI1_DT.IP_KOJI );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_CKJI_PN)->IDC_X ))->SetCheck(chk);
			chk = (dtline->LINE_CND[i].KEY_inp & SC_ITEMNONE) ? 1 : 0;
			((CButton*)GetDlgItem(IDC_CHECK_CKJ))->SetCheck(chk);
			SendMessage(WM_COMMAND, MAKELONG(IDC_CHECK_CKJ, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_CKJ)->m_hWnd);
			break;
		case SCN_CKJI2_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->SCN_CKJI2_DT.IP_KOJI );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;

		case SCN_DEBT_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//科目名称再表示
				knam_set( &dtline->SCN_DEBT_DT );
			}
			DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_DEBT_DT.IP_stKMK, m_pZm );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DEB_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_DBR1_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//枝番摘要再セット
				set_etcdsp( i );
			}
			if( dtline->SCN_DBR1_DT.IP_cBRN != -1 ) {
				set_codestr( buf, sizeof buf, dtline->SCN_DBR1_DT.IP_cBRN, Voln1->edcol );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DBRN_PN)->IDC_X ))->SetCheck(chk);
			chk = (dtline->LINE_CND[i].KEY_inp & SC_ITEMNONE) ? 1 : 0;
			((CButton*)GetDlgItem(IDC_CHECK_DBR))->SetCheck(chk);
			SendMessage(WM_COMMAND, MAKELONG(IDC_CHECK_DBR, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_DBR)->m_hWnd);
			break;
		case SCN_DBR2_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//枝番摘要再セット
				set_etcdsp( i );
			}
			if( dtline->SCN_DBR2_DT.IP_cBRN != -1 ) {
				set_codestr( buf, sizeof buf, dtline->SCN_DBR2_DT.IP_cBRN, Voln1->edcol );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			break;
		case SCN_CRED_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//科目名称再表示
				knam_set( &dtline->SCN_CRED_DT );
			}
			DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_CRED_DT.IP_stKMK, m_pZm );
			break;
		case SCN_CBR1_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//枝番摘要再セット
				set_etcdsp( i );
			}
			if( dtline->SCN_CBR1_DT.IP_cBRN != -1 ) {
				set_codestr( buf, sizeof buf, dtline->SCN_CBR1_DT.IP_cBRN, Voln1->edcol );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_CBRN_PN)->IDC_X ))->SetCheck(chk);
			chk = (dtline->LINE_CND[i].KEY_inp & SC_ITEMNONE) ? 1 : 0;
			((CButton*)GetDlgItem(IDC_CHECK_CBR))->SetCheck(chk);
			SendMessage(WM_COMMAND, MAKELONG(IDC_CHECK_CBR, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_CBR)->m_hWnd);
			break;
		case SCN_CBR2_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//枝番摘要再セット
				set_etcdsp( i );
			}
			if( dtline->SCN_CBR2_DT.IP_cBRN != -1 ) {
				set_codestr( buf, sizeof buf, dtline->SCN_CBR2_DT.IP_cBRN, Voln1->edcol );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;

		case SCN_DBCR_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//科目名称再表示
				knam_set( &dtline->SCN_DBCR_DT );
			}
			DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_DBCR_DT.IP_stKMK, m_pZm );

			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DBCR_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_DCBR1_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//枝番摘要再セット
				set_etcdsp( i );
			}
			if( dtline->SCN_DCBR1_DT.IP_cBRN != -1 ) {
				set_codestr( buf, sizeof buf, dtline->SCN_DCBR1_DT.IP_cBRN, Voln1->edcol );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DCBRN_PN)->IDC_X ))->SetCheck(chk);
			
			break;
		case SCN_DCBR2_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//枝番摘要再セット
				set_etcdsp( i );
			}
			if( dtline->SCN_DCBR2_DT.IP_cBRN != -1 ) {
				set_codestr( buf, sizeof buf, dtline->SCN_DCBR2_DT.IP_cBRN, Voln1->edcol );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;

		case SCN_VAL1_PN:
			if( dtline->LINE_CND[ i ].INP_sg == TRUE )
			{
				var.pbVal = dtline->SCN_VAL1_DT.IP_VAL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}
			else
			{
				var.pbVal = NULL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}

			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_VAL_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_VAL2_PN:
			if( dtline->LINE_CND[ i ].INP_sg == TRUE )
			{
				var.pbVal = dtline->SCN_VAL2_DT.IP_VAL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}
			else
			{
				var.pbVal = NULL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}
			break;

		case SCN_ZEI1_PN:
			if( dtline->LINE_CND[ i ].INP_sg == TRUE )
			{
				var.pbVal = dtline->SCN_ZEI1_DT.IP_VAL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}
			else
			{
				var.pbVal = NULL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_ZEI_PN)->IDC_X ))->SetCheck(chk);

			break;
		case SCN_ZEI2_PN:
			if( dtline->LINE_CND[ i ].INP_sg == TRUE )
			{
				var.pbVal = dtline->SCN_ZEI2_DT.IP_VAL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}
			else
			{
				var.pbVal = NULL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}
			break;

		case SCN_SYZSW_PN:
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( SCAN_KEY.SCN_SYZSW_DT.SKBN_SGN1 );

			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_SYZSW_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_ZRITU_PN:
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( SCAN_KEY.SCN_ZRITU_DT.SKBN_SGN1 );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_ZRITU_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_SKBN_PN:
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( SCAN_KEY.SCN_SKBN_DT.SKBN_SGN1 );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_SKBN_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_URISIRE_PN:
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( SCAN_KEY.SCN_URISIRE_DT.SKBN_SGN1 );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_URISIRE_PN)->IDC_X ))->SetCheck(chk);
			break;

		case SCN_TKY_PN:
			((CComboBox*)GetDlgItem(IDC_TKYCOMBO))->SetCurSel( 0 );
			strcpy_s( buf, sizeof buf, (char*)dtline->SCN_TKY_DT.IP_TKY );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;

		case SCN_SNUM1_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->SCN_SNUM1_DT.IP_SNUM );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_SNUM_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_SNUM2_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->SCN_SNUM2_DT.IP_SNUM );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case SCN_TDAT1_PN:
			if( SCAN_KEY.SCN_TDAT1_DT.IP_TGDAY > 0 ) {
				sprintf_s( buf, sizeof buf, "%d", SCAN_KEY.SCN_TDAT1_DT.IP_TGDAY );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;

			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			IntsrekitoGenstr( buf, sizeof buf, SCAN_KEY.SCN_TDAT1_DT.IP_TGDAY );
			DBSetDispString( this, (ctl+i)->IDC_X, buf );

			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_TGDATE_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_TDAT2_PN:
			if( SCAN_KEY.SCN_TDAT2_DT.IP_TGDAY > 0 ) {
				sprintf_s( buf, sizeof buf, "%d", SCAN_KEY.SCN_TDAT2_DT.IP_TGDAY );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;

			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			IntsrekitoGenstr( buf, sizeof buf, SCAN_KEY.SCN_TDAT2_DT.IP_TGDAY );
			DBSetDispString( this, (ctl+i)->IDC_X, buf );
			break;
		case SCN_HUSEN_PN:
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( SCAN_KEY.SCN_HUSEN_DT.SKBN_SGN1 );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_HUSEN_PN)->IDC_X ))->SetCheck(chk);
			break;

		default:
			break;
		}
	}

}


// 科目名称再セット
void CScanKey::knam_set( struct _ItemData *data )
{
	struct _DBKNREC	*pKn;

	if( pKn = DB_PjisToKnrec( data->IP_CdNum, m_pZm ) ) {
		memcpy( data->IP_KNAM, pKn->knnam, KMNLEN );
	}
}


//オリジナルダイアログサイズ
void CScanKey::get_originalsize( double& w, double& h )
{
	w = (double)(m_original_rect.right - m_original_rect.left);
	h = (double)(m_original_rect.bottom - m_original_rect.top);
}




/*
#ifdef	_ICSPEN_	// 01.20 /01
//ペンDLL初期化
short CScanKey::SetPen( BOOL flg, ICSPen* ppen )
{
	if( flg == FALSE )	return	0;

	if( flgPen != FALSE )	return	0;

	icspen	=	ppen;

#ifdef DB_KEEP
	icspen->RegisterControl( this );
#endif

	flgPen	=	TRUE;

	return	0;
}
*/

//ペン表示ウィンドウ位置、サイズ
void CScanKey::SetPenRect( CRect& rect )
{
	rectPen	=	rect;
}

//消費税選択後の移動
void CScanKey::set_nextpos()
{
/*
	switch( get_nowpn() ) {
		//消費税区分
		case SY_SKBN_PN:
			OnTerminationEditctrl1( VK_RETURN );
			break;
		//売上/仕入区分
		case SY_URISIRE_PN:
			OnTerminationEditctrl2( VK_RETURN );
			break;
	}
*/
}


void CScanKey::OnICSInputFocus( int pn )
{

#ifdef DB_OLD_CLOSE

	if( flgPen != FALSE ) {
		
		CRect	rect = ((CDBDinpView*)m_pParent)->GetPenRect();

		switch( pn )
		{
		case SCN_SEQ1_PN:	//番号（開始）
		case SCN_SEQ2_PN:	//番号（終了）
		case SCN_DENP1_PN:	//伝票番号（開始）
		case SCN_DENP2_PN:	//伝票番号（終了）
		case SCN_BMON1_PN:	//借方部門（開始）
		case SCN_BMON2_PN:	//　　部門（終了）
		case SCN_CBMN1_PN:	//貸方部門（開始）
		case SCN_CBMN2_PN:	//　　部門（終了）
			icspen->ShowWindow( TRUE, PWINDOW_NUM );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_NUM );
			break;
		case SCN_DATE1_PN:	//日付（開始）
		case SCN_DATE2_PN:	//日付（終了）
			icspen->ShowWindow( TRUE, PWINDOW_MD );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_MD );
			break;
		case SCN_DEBT_PN:	//借方
		case SCN_CRED_PN:	//貸方
		case SCN_DBCR_PN:	//貸借
			icspen->ShowWindow( TRUE, PWINDOW_KANAK );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_KANAK );
			break;

		case SCN_DBR1_PN:	//借方枝番（開始）
		case SCN_DBR2_PN:	//借方枝番（終了）
		case SCN_DCBR1_PN:	//貸借枝番（開始）
		case SCN_DCBR2_PN:	//貸借枝番（終了）
		case SCN_CBR1_PN:	//貸方枝番（開始）
		case SCN_CBR2_PN:	//貸方枝番（終了）
//		case SCN_BRNINP_PN:
//		case SCN_BMNINP_PN: //#* 08.28 /02
			icspen->ShowWindow( TRUE, PWINDOW_PEN );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_PEN );
			break;

		case SCN_VAL1_PN:	//金額（開始）
		case SCN_VAL2_PN:	//金額（終了）
		case SCN_ZEI1_PN:	//税額（開始）
		case SCN_ZEI2_PN:	//税額（終了）
			icspen->ShowWindow( TRUE, PWINDOW_VAL );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_VAL );
			break;
		case SCN_SKBN_PN:	//消費税区分
		case SCN_URISIRE_PN://業種・売り仕入区分
			icspen->ShowWindow( FALSE );
//			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_VAL );
			break;
		case SCN_TKY_PN:	//摘要
			icspen->ShowWindow( TRUE, PWINDOW_KANAT );
			icspen->ShowWindow( TRUE, PWINDOW_GAIJI );
			icspen->ShowWindow( TRUE, PWINDOW_PEN );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_KANAT );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_GAIJI );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_PEN );
			break;

		}
	}

#endif 
}


// 最終データセット
void CScanKey::last_data_set()
{
int pn = get_nowpn();

	if( get_nowpn() == SCN_SEQ1_PN )
		_set_focus( SCN_SEQ2_PN );
	else
		_set_focus( SCN_SEQ1_PN );
#ifdef LATER_CLOSE
	if( pn == SCN_SKBN_PN || pn == SCN_URISIRE_PN )
		m_LIST1.ShowWindow( SW_HIDE );
	else
#endif
		_set_focus( pn );
}

// IDC get
struct _InpCtl *CScanKey::scGetINP_CTL( int pn )
{
	return( &INP_CTL[pn] );
}


//
LRESULT CScanKey::RetroFocus( WPARAM wParam, LPARAM lParam ) 
{
#ifdef _ICSPEN_	// 01.20 /01
	_set_focus( wParam );
#else
	set_focus( wParam );
#endif
	return 1;
}

// 現在フォーカス位置にカーソルセット
void CScanKey::set_nowfocus()
{
	PostMessage( WM_RETROFOCUS, get_nowpn() );
}

// 先頭項目にカーソルセット
void CScanKey::set_topfocus()
{
	PostMessage( WM_RETROFOCUS, SCN_ORDER_PN );
}


// カーソルセット
#ifdef _ICSPEN_	// 01.20 /01
void CScanKey::set_focus( int pn )
{
	PostMessage( WM_RETROFOCUS, pn );
}
void CScanKey::_set_focus( int pn )
{
#else
void CScanKey::set_focus( int pn )
{
#endif

#ifndef CLOSE //#* 06.19 /01 ｲﾒｰｼﾞ摘要のみ検索
	if( pn == SCN_TKY_PN ) {
		if( is_mdfy_imgtky() )
			pn = SCN_IMGEDT_PN;
	}
#endif

#ifdef SPEECH	// 05.12 /00
	int idc = get_idc( pn );

	if( idc )
	{
		if( pn <= SCN_TKY_PN )
			c_set( idc );
		else if( pn == SCN_BRNINP_PN || pn == SCN_BMNINP_PN )
			c_set( idc );
		else if( pn == SCN_IDOK )
		{
			GotoDlgCtrl( GetDlgItem( idc ) );
		}
		else if( pn == SCN_IDCANCEL )
		{
			GotoDlgCtrl( GetDlgItem( idc ) );
		}
		else if( pn == SCN_TKYONLY ) {//#* 05.16 /01 摘要のみ修正
			GotoDlgCtrl( GetDlgItem( idc ) );
		}
		else if( pn == SCN_IMGEDT_PN ) {//#* 06.19 /01 ｲﾒｰｼﾞ摘要のみ検索
			GotoDlgCtrl( GetDlgItem( idc ) );
		}
	}
#else
	if( pn <= SCN_NOINP_PN )
		c_set( scGetINP_CTL( pn )->IDC_X );
	else if( pn == SCN_IDOK )
	{
		GotoDlgCtrl( GetDlgItem( IDOK ) );
	}
	else if( pn == SCN_IDCANCEL )
	{
		GotoDlgCtrl( GetDlgItem( IDCANCEL ) );
	}
	else if( pn == SCN_TKYONLY ) {//#* 05.16 /01 摘要のみ修正
		GotoDlgCtrl( GetDlgItem( IDC_TKYONLY ) );
	}
	else if( pn == SCN_IMGEDT_PN ) {//#* 06.19 /01 ｲﾒｰｼﾞ摘要のみ検索
		GotoDlgCtrl( GetDlgItem( IDC_IMGTKYEDIT ) );
	}
	else if( pn >= SCN_CHKDBM_PN  && pn <= SNOT_HUSEN_PN ) { // チェック ＆ NOT
		GotoDlgCtrl( GetDlgItem( scGetINP_CTL( pn )->IDC_X ) );
	}
#endif

}

#ifndef CLOSE	// 04.28 /00
void CScanKey::FocusBhnd(  )
{
	c_set( IDC_ICSDISPCTRL1 );	// タイトルに 
}
#endif

void CScanKey::EndView()
{
	OnCancel();
}


// 部門入力表示ＯＮ／ＯＦＦ ... 08.28 /02
void CScanKey::disp_bmoninp( int sw )
{
	if( sw != bmon_dispcnd() )
	{
		bmon_dispcnd() = sw;

	//	_disp_brinp( sw, IDC_ICSHEDITCTRL2 );
	}
}


// 注意表示ＯＮ／ＯＦＦ
void CScanKey::disp_caution( int sw )
{
	if( sw != caution_dispcnd() )
	{
		caution_dispcnd() = sw;

		_disp_caution( sw );
	}
}

void CScanKey::_disp_caution( int sw )
{
	double w, h;
	double rate_w, rate_h;
	CWnd *disporg;

	rate_w = rate_h = 1.0;
//	get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット

	VERIFY(disporg = GetDlgItem( IDC_ICSDISPCTRL2 ) );
	if( sw )
		get_disp2_originalsize( w, h );	// 元サイズをゲット
	else
		w = h = 0;

	disporg->SetWindowPos( 0, 0, 0, w * rate_w, h * rate_h, SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE );

}



static BOOL isfixvalcnd( int pn )
{
#ifndef CLOSE	// 04.04 /00
	
	return TRUE;
	
#else
	
	BOOL sg;
	
	switch( pn )
	{
	case SCN_DEBT_PN:
	case SCN_CRED_PN:
	case SCN_DBCR_PN:
	case SCN_TKY_PN:
		sg = TRUE;
		break;
	default:
		sg = FALSE;
		break;
	}
	return sg;
#endif
}

//ポジションによる表示切替
void CScanKey::disp_change()
{
BOOL sg;
int pn = get_nowpn();

	// 選択画面の表示色
	sg = isfixvalcnd( pn );

	if( sg != fixvalcnd() )
	{
		enable_fixval( sg );
	}

	// 固定科目選択のディセーブル／イネーブル
//	sg = pn == SCN_TKY_PN ? FALSE : TRUE;	... 02.27 /02

	switch( pn ) {
	case SCN_BMON1_PN: case SCN_BMON2_PN :
	case SCN_CBMN1_PN: case SCN_CBMN2_PN :
		sg = FALSE;	
		break;
	case SCN_TKY_PN:
		sg = FALSE;	
		break;
	default:
		sg = TRUE;
		break;
	}

	if( sg != fix8cnd() )
	{
		fix8disp( sg );
	}
}

//ポジションによる表示切替（強制表示）
void CScanKey::_disp_change()
{
int pn = get_nowpn();
BOOL sg;

//	sg = pn >= SCN_TKY_PN ? FALSE : TRUE; ... 08.06 /02
	sg = (pn >= SCN_TKY_PN || pn == SCN_BMON1_PN || pn == SCN_BMON2_PN ) ? FALSE : TRUE;

	// 固定科目選択のディセーブル／イネーブル
	fix8disp( sg );
}


// 現在ポジションセット
int CScanKey::set_nowpn( int pn )
{
	BOOL sg, inpsg;
	CString code;
	int prev_pn;

	prev_pn = NOW_PN;
	NOW_PN = pn;

	//科目名称選択画面を直前のポジションに従いリセット
	SelItemReset( NOW_PN, prev_pn );

	//表題を窪み表示
	selbox( pn );

#ifdef LATER_CLOSE ///////////////////////////
	// 消費税選択ボックス
#ifndef CLOSE	// 03.30 /99
	if( (prev_pn == SCN_SKBN_PN || prev_pn == SCN_URISIRE_PN) && (pn != prev_pn) )
		kbn_select_dsp( FALSE );
#else
	kbn_select_dsp( /*pn == SCN_SKBN_PN || pn == SCN_URISIRE_PN ? TRUE : */FALSE );
#endif
#endif	//////////////////////////////////////


#ifndef CLOSE	// 10.20 /98
	disp_change();
#endif

	if( prev_pn != pn )
	{
		switch( pn )
		{
		case SCN_DBR1_PN:
		case SCN_DBR2_PN:
		case SCN_CBR1_PN:
		case SCN_CBR2_PN:
		case SCN_DCBR1_PN:
		case SCN_DCBR2_PN:
			switch( pn )
			{
			case SCN_DBR1_PN:
			case SCN_DBR2_PN:
				code = _getDATA( SCN_DEBT_PN )->IP_CdNum;
				inpsg = SCAN_KEY.LINE_CND[SCN_DEBT_PN].INP_sg;
				break;
			case SCN_CBR1_PN:
			case SCN_CBR2_PN:
				code = _getDATA( SCN_CRED_PN )->IP_CdNum;
				inpsg = SCAN_KEY.LINE_CND[SCN_CRED_PN].INP_sg;
				break;
			default:
				code = _getDATA( SCN_DBCR_PN )->IP_CdNum;
				inpsg = SCAN_KEY.LINE_CND[SCN_DBCR_PN].INP_sg;
				break;
			}
			sg = inpsg ? TRUE : FALSE;
			break;
		default:
			sg = FALSE;
		}

		// 枝番摘要
		if( sg == TRUE )
		{
			SelPar par;
			par.sel_pn = SL_BRNTKY_PN;
			par.tkybrn = code;

			m_Seldata.SelectJob( &par, -1 );

#ifdef SEL_KEEP
			if( BRNTKY_sel && TKYBRN_SEL.BRN_KCOD == code )
				ret = brntky_page( &TKYBRN_SEL, SelVal32, &((CDBDinpView*)m_pParent)->m_selval32, TKYBRN_SEL.BRN_PAGE );
			else
				ret = brntky_sch( &TKYBRN_SEL, SelVal32, &((CDBDinpView*)m_pParent)->m_selval32, code );
			if( ret == FALSE )
				sg = FALSE;
			else
				((CDBDinpView*)m_pParent)->BRNTKY_sel_set( ret );
#endif
		}

		if( sg == FALSE ) {
			SelPar par;
			par.sel_pn = SL_RESET;

			fixvalcnd( !isfixvalcnd( pn ));
			disp_change();

			m_Seldata.SelectJob( &par, -1 );
		}

#ifdef SEL_KEEP
		if( sg == FALSE && BRNTKY_sel )
		{
			((CDBDinpView*)m_pParent)->BRNTKY_sel_set( 0 );

			fixvalcnd( !isfixvalcnd( pn ));
			disp_change();

			//科目・摘要選択画面リセット
			((CDBDinpView*)m_pParent)->SelDspRst( INP_mode );
		}
#endif

#ifndef CLOSE //#* 06.19 /01
//		if( is_mdfy_imgtky() == TRUE && pn == SCN_TKY_PN )
//			set_focus( SCN_IMGEDT_PN );
#endif
	}

	// コメント表示
	cmntbox( pn );

#ifndef CLOSE	// 12.15 /98
	// 選択背景色
	select_back_col( pn );
# ifdef LATER_CLOSE /////////
	((CICSHedit*)GetDlgItem( IDC_DISPETC ))->SetBackColor( BRTEK_BackCOL );	// 枝番摘要表示背景色（常に選択背景色）
# endif
#endif

	// 入力項目背景色
	inp_bkcol( pn );

#ifdef DB_CUT
	//
	// 枝番入力へフォーカス
	switch( pn )
	{
	case SCN_DBR1_PN:
	case SCN_DBR2_PN:
	case SCN_CBR1_PN:
	case SCN_CBR2_PN:
	case SCN_DCBR1_PN:
	case SCN_DCBR2_PN:
		set_focus( SCN_BRNINP_PN );
		break;
#ifndef CLOSE //#* 08.28 /02
	case SCN_BMON1_PN:	
	case SCN_BMON2_PN:
		set_focus( SCN_BMNINP_PN );
		break;
#endif
	default:
		break;
	}
#endif

	return( prev_pn );	//直前のポジション
}


// 現在ポジションゲット
int CScanKey::get_nowpn( void )
{
	return( NOW_PN );
}



// イメージ摘要のみ か 文字摘要のみ のスキャンか？
// 
int CScanKey::is_mdfy_imgtky()
{
	if( IMG_master == ERR )
		return FALSE;

	if( SCAN_KEY.LINE_CND[SCN_TKY_PN].INP_sg && SCAN_KEY.LINE_CND[SCN_TKY_PN].INP_type )
		return TRUE;
	else
		return FALSE;
}


// イメージ摘要のエディットの表示/非表示
void CScanKey::disp_imgtkyedit( int sw )
{
	RECT newrect = {0};
	CWnd *dispwnd, *orgwnd, *prevwnd;
	WINDOWPLACEMENT orgwpt;
	int now_pn, prev_pn, i;

	VERIFY( orgwnd= GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X ) );
	VERIFY( dispwnd = GetDlgItem( IDC_IMGTKYEDIT ) );

	prev_pn = SCN_ORDER_PN;

	for( i = SCN_URISIRE_PN; i >= SCN_ORDER_PN; i-- ) {
		if( scGetINP_CTL( i )->INP_ena == TRUE ) {
			prev_pn = i;
			break;
		}
	}
	VERIFY( prevwnd= GetDlgItem( scGetINP_CTL( prev_pn )->IDC_X ) );

	now_pn = get_nowpn();

	if( sw == STK_NORMAL ) {
	//	dispwnd->ShowWindow(SW_HIDE);
		dispwnd->HideCaret();
		dispwnd->SetWindowPos( prevwnd, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOACTIVATE );

		if( now_pn == SCN_SELTKY_PN )
			set_focus( SCN_TKY_PN );
		else
			set_focus( now_pn );
	}
	else {
		char* txt;
		switch( sw ) {
		case STK_IMG:
			txt = "＜イメージ摘要仕訳を検索します＞";	break;
		case STK_STR:
			txt = "＜文字摘要仕訳を検索します＞";	break;
		case STK_STRNONE:
			txt = "＜摘要未入力仕訳を検索します＞";	break;
		default:
			txt = "エラー";
			break;
		}
		dispwnd->SetWindowText( txt );

	//	dispwnd->ShowWindow( SW_SHOW );
		dispwnd->ShowCaret();
		orgwnd->GetWindowPlacement( &orgwpt );
		newrect = orgwpt.rcNormalPosition;
		dispwnd->SetWindowPos( prevwnd, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOACTIVATE );

		set_focus( SCN_IMGEDT_PN );
		if( now_pn != SCN_TKY_PN )
			set_nowpn( SCN_TKY_PN );

	}
}


// データアドレスリード
struct _ItemData *CScanKey::_getDATA( int pn )
{
	return &SCAN_KEY._XXDTA2[pn];
}

struct _ItemData *CScanKey::getDATA()
{
	return _getDATA( get_nowpn() );
}


//　その他項目コントロールデータレコードをセット
void CScanKey::set_etcdsp( int pn )
{
//struct _ItemData data, *pdata;
int code_pn;

	if( pn == SCN_DBR1_PN || pn == SCN_DBR2_PN ||
		pn == SCN_CBR1_PN || pn == SCN_CBR2_PN ||
		pn == SCN_DCBR1_PN || pn == SCN_DCBR2_PN )
	{
		switch( pn )
		{
		case SCN_DBR1_PN:
		case SCN_DBR2_PN:
			code_pn = SCN_DEBT_PN;
			break;
		case SCN_CBR1_PN:
		case SCN_CBR2_PN:
			code_pn = SCN_CRED_PN;
			break;
		case SCN_DCBR1_PN:
		case SCN_DCBR2_PN:
			code_pn = SCN_DBCR_PN;
			break;
		}
	/*	
		data = *_getDATA( code_pn );	// 科目
		pdata = _getDATA( pn );
		data.IP_BRN = *((unsigned short*)pdata->IP_cBRN);	// 枝番
		_set_etcdsp( &data, &SCAN_KEY.LINE_CND[pn], FALSE );
		sprintf( pdata->ETC_TXT, "%s", data.ETC_TXT );
	*/
	}
	else if( pn == SCN_BMON1_PN || pn == SCN_BMON2_PN ) {	//#* 08.19 /02
/*		pdata = _getDATA( pn );
		_set_bmndsp( pdata, &SCAN_KEY.LINE_CND[pn] );
*/
	}
}


void CScanKey::CtrlShow( int idc, BOOL sw )
{
	GetDlgItem( idc )->ShowWindow( sw ? SW_SHOW : SW_HIDE );
}

// その他項目　表示
void CScanKey::dsp_etc( int pn, BOOL dspsw )
{
struct _ItemData *pdata;
int hide_idc1, hide_idc2;
BOOL dsp;

	switch( pn )
	{
	case SCN_DBR1_PN:
	case SCN_DBR2_PN:
	case SCN_CBR1_PN:
	case SCN_CBR2_PN:
	case SCN_DCBR1_PN:
	case SCN_DCBR2_PN:
		// 項目表示
		pdata = _getDATA( pn );
		dsp = _dsp_etc( this, pdata, &SCAN_KEY.LINE_CND[pn], scGetINP_CTL(pn), BRNTKY_X_RATE, dspsw, FALSE );

		// 背景のコントロールの表示／非表示
		switch( pn )
		{
		case SCN_CBR1_PN:	hide_idc1 = IDC_STATIC8;
							hide_idc2 = 0;
							break;
		case SCN_DCBR1_PN:	hide_idc1 = IDC_STATIC8;
							hide_idc2 = IDC_STATIC9;
							break;
		case SCN_DBR1_PN:	hide_idc1 = IDC_CHECK_DBM;
							hide_idc2 = 0;
							break;

		case SCN_CBR2_PN:	hide_idc1 = IDC_CHECK_DBR;
							hide_idc2 = 0;
							break;
		case SCN_DCBR2_PN:	hide_idc1 = IDC_CHECK_CBR;
							hide_idc2 = 0;
							break;

		default:			hide_idc1 = 0;
							hide_idc2 = 0;
							break;
		}
		if( hide_idc1 || hide_idc2 )
		{
			if( dsp )
			{
				if( hide_idc1 )
					CtrlShow( hide_idc1, FALSE );
				if( hide_idc2 )
					CtrlShow( hide_idc2, FALSE );
			}
			else
			{
				this->UpdateWindow();
				if( hide_idc1 )
					CtrlShow( hide_idc1, TRUE );
				if( hide_idc2 )
					CtrlShow( hide_idc2, TRUE );
			}
		}

		break;

	case SCN_BMON1_PN:	//#* 08.19 /02 
	case SCN_BMON2_PN:
		// 項目表示
		pdata = _getDATA( pn );
		dsp = _dsp_etc( this, pdata, &SCAN_KEY.LINE_CND[pn], scGetINP_CTL(pn), BUMON_X_RATE, dspsw, FALSE );

		// 背景のコントロールの表示／非表示
		if( pn == SCN_BMON1_PN )
		{
			hide_idc1 = IDC_STATIC3;
			if( dsp )
			{
				CtrlShow( hide_idc1, FALSE );
			}
			else
			{
				this->UpdateWindow();
				CtrlShow( hide_idc1, TRUE );
			}
		}
		break;
	}
}

//アクセス用クラスセット
void CScanKey::fnc_set( struct _InpCtl *ctl)
{
	int i;

	for( i = SCN_SEQ1_PN ; i <= SNOT_HUSEN_PN ; ++i )
	{
		switch( (ctl+i)->IDC_X )
		{
		case IDC_SCNSEQ1:	(ctl+i)->p_obj = (long)&m_Seq1;	break;
		case IDC_SCNSEQ2: 	(ctl+i)->p_obj = (long)&m_Seq2;	break;
		case IDC_SCNDATE1:	/*(ctl+i)->p_obj = (long)&m_Date1;*/	break;	
		case IDC_SCNDATE2:	/*(ctl+i)->p_obj = (long)&m_Date2;*/	break;	
		case IDC_SCNDENP1:	(ctl+i)->p_obj = (long)&m_Denp1;	break;	
		case IDC_SCNDENP2:	(ctl+i)->p_obj = (long)&m_Denp2;	break;	
		case IDC_SCNBMON1:	(ctl+i)->p_obj = (long)&m_Bmon1;	break;
		case IDC_SCNBMON2:	(ctl+i)->p_obj = (long)&m_Bmon2;	break;
		case IDC_SCNCBMN1:	(ctl+i)->p_obj = (long)&m_Cbmn1;	break;
		case IDC_SCNCBMN2:	(ctl+i)->p_obj = (long)&m_Cbmn2;	break;
		case IDC_SCNKOJI1:	(ctl+i)->p_obj = (long)&m_Koji1;	break;
		case IDC_SCNKOJI2:	(ctl+i)->p_obj = (long)&m_Koji2;	break;
		case IDC_SCNCKJI1:	(ctl+i)->p_obj = (long)&m_Ckji1;	break;
		case IDC_SCNCKJI2:	(ctl+i)->p_obj = (long)&m_Ckji2;	break;
		case IDC_SCNDEBT:	(ctl+i)->p_obj = (long)&m_Debt;	break;
		case IDC_SCNDBR1:	(ctl+i)->p_obj = (long)&m_Dbr1;	break; 
		case IDC_SCNDBR2:	(ctl+i)->p_obj = (long)&m_Dbr2;	break; 
		case IDC_SCNCRED:	(ctl+i)->p_obj = (long)&m_Cred;	break; 
		case IDC_SCNCBR1:	(ctl+i)->p_obj = (long)&m_Cbr1;	break; 
		case IDC_SCNCBR2:	(ctl+i)->p_obj = (long)&m_Cbr2;	break; 
		case IDC_SCNDBCR: 	(ctl+i)->p_obj = (long)&m_DBcr;	break;
		case IDC_SCNDCBR1:	(ctl+i)->p_obj = (long)&m_DCbr1;	break; 
		case IDC_SCNDCBR2:	(ctl+i)->p_obj = (long)&m_DCbr2;	break;
		case IDC_SCNVAL1:	(ctl+i)->p_obj = (long)&m_Val1;	break;
		case IDC_SCNVAL2:	(ctl+i)->p_obj = (long)&m_Val2;	break; 
		case IDC_SCNZEI1:	(ctl+i)->p_obj = (long)&m_Zei1;	break; 
		case IDC_SCNZEI2:	(ctl+i)->p_obj = (long)&m_Zei2;	break; 
		case IDC_SCNTKY:	(ctl+i)->p_obj = (long)&m_Tky;	break; 
		case IDC_SCNSNUM1: 	(ctl+i)->p_obj = (long)&m_Snum1;	break;
		case IDC_SCNSNUM2:	(ctl+i)->p_obj = (long)&m_Snum2;	break;
		case IDC_SCNTDAT1:	(ctl+i)->p_obj = (long)&m_Tdat1;	break;
		case IDC_SCNTDAT2:	(ctl+i)->p_obj = (long)&m_Tdat2;	break;
		default:
			break;	
		}	
	}
}

//入力コントロール，１行イニシャライズ
void CScanKey::inpctlline_inz( struct _InpCtl *ctl )
{
	fnc_set( ctl );
	int bm_sgn, kj_sgn;
	char tmp[20];
	bm_sgn = kj_sgn = 0;

	for( int i = SCN_ORDER_PN ; i <= SNOT_HUSEN_PN ; ++i )
	{
		switch( i ) {
		case SCN_BMON1_PN:	// 部門
		case SCN_BMON2_PN:
		case SCN_CBMN1_PN:
		case SCN_CBMN2_PN:
		case SCN_CHKDBM_PN:
		case SCN_CHKCBM_PN:
		case SNOT_DBMN_PN:
		case SNOT_CBMN_PN:

			if( (Voln1->sub_sw&0x02) )
			{
				(ctl+i)->INP_ena = TRUE;
				if( (ctl+i)->p_obj ) {
					sprintf_s( tmp, sizeof tmp, "%d", _op_max_calq( Voln1->bmcol ) );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMinval( "0" );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxval( tmp );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxLen( 0 );	//桁数設定(数字のときは 0)
				}
			}
			else
			{
				(ctl+i)->INP_ena = FALSE;
				if( (ctl+i)->p_obj ) {
					sprintf_s( tmp, sizeof tmp, "%d", _op_max_calq( Voln1->bmcol ) );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMinval( "0" );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxval( tmp );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxLen( 0 );

					((CICSDBEDT *)(ctl+i)->p_obj)->EnableWindow( FALSE );	
				}
				else {
					GetDlgItem((ctl+i)->IDC_X)->EnableWindow( FALSE );
					GetDlgItem((ctl+i)->IDC_X)->EnableWindow( FALSE );				
				}
				if( ! bm_sgn ) {
					bm_sgn = 1;
					GetDlgItem(IDC_ST_DBMN)->EnableWindow( FALSE );
					GetDlgItem(IDC_ST_CBMN)->EnableWindow( FALSE );
					GetDlgItem(IDC_STATIC4)->EnableWindow( FALSE );
					GetDlgItem(IDC_STATIC5)->EnableWindow( FALSE );
				}
			}
			break;
		case SCN_KOJI1_PN:	// 工事
		case SCN_KOJI2_PN:
		case SCN_CKJI1_PN:
		case SCN_CKJI2_PN:
		case SCN_CHKDKJ_PN:
		case SCN_CHKCKJ_PN:
		case SNOT_DKJI_PN:
		case SNOT_CKJI_PN:

			if( Voln1->apno == 0x20 && (Voln1->sub_sw&0x04) )
			{
				(ctl+i)->INP_ena = TRUE;
				if( (ctl+i)->p_obj ) {
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxLen( 0 );
					sprintf_s( tmp, sizeof tmp, "%d", _op_max_calq( Voln1->kjcol ) );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMinval( "0" );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxval( tmp );
				}
			}
			else
			{
				(ctl+i)->INP_ena = FALSE;
				if( (ctl+i)->p_obj ) {
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxLen( 0 );
					sprintf_s( tmp, sizeof tmp, "%d", _op_max_calq( Voln1->kjcol ) );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMinval( "0" );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxval( tmp );
					((CICSDBEDT *)(ctl+i)->p_obj)->EnableWindow( FALSE );
				}
				else {
					GetDlgItem((ctl+i)->IDC_X)->EnableWindow( FALSE );
					GetDlgItem((ctl+i)->IDC_X)->EnableWindow( FALSE );				
				}
				if( ! kj_sgn ) {
					kj_sgn = 1;
					GetDlgItem(IDC_ST_DKJ)->EnableWindow( FALSE );
					GetDlgItem(IDC_ST_CKJ)->EnableWindow( FALSE );
					GetDlgItem(IDC_STATIC6)->EnableWindow( FALSE );
					GetDlgItem(IDC_STATIC7)->EnableWindow( FALSE );
				}
			}
			break;

		case SCN_DEBT_PN:
		case SCN_CRED_PN:
		case SCN_DBCR_PN:
			(ctl+i)->INP_ena = TRUE;
			if( (ctl+i)->p_obj ) {
				((CICSDBEDT*)(ctl+i)->p_obj )->ImeEndMode( TRUE );
				((CICSDBEDT*)(ctl+i)->p_obj )->SetKanaTerm(TRUE, 1, ICSDBEDT_KTYPE_KANA );
			}
			break;		
		case SCN_TKY_PN:	// 摘要
			(ctl+i)->INP_ena = TRUE;
			if( (ctl+i)->p_obj )
			{
				((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxLen( (m_pZm->zvol->tk_ln*2) );	//文字数設定
			}
			break;
		default:
			(ctl+i)->INP_ena = TRUE;
			break;
		}
	}

}



//科目名称選択画面を直前のポジションに従いリセット
void CScanKey::SelItemReset( int now_pn, int prev_pn )
{
	switch( prev_pn )
	{
	case SCN_DEBT_PN:	//	8	//借方
	case SCN_DBR1_PN:	//	9	//借方枝番（開始）
	case SCN_DBR2_PN:	//	10	//借方枝番（終了）
	case SCN_CRED_PN:	//	11	//貸方
	case SCN_CBR1_PN:	//	12	//貸方枝番（開始）
	case SCN_CBR2_PN:	//	13	//貸方枝番（終了）
	case SCN_DBCR_PN:	//	14	//貸借
	case SCN_DCBR1_PN:	//	15	//貸借枝番（開始）
	case SCN_DCBR2_PN:	//	16	//貸借枝番（終了）
		switch( now_pn )
		{
		case SCN_DEBT_PN:	//	8	//借方
		case SCN_DBR1_PN:	//	9	//借方枝番（開始）
		case SCN_DBR2_PN:	//	10	//借方枝番（終了）
		case SCN_CRED_PN:	//	11	//貸方
		case SCN_CBR1_PN:	//	12	//貸方枝番（開始）
		case SCN_CBR2_PN:	//	13	//貸方枝番（終了）
		case SCN_DBCR_PN:	//	14	//貸借
		case SCN_DCBR1_PN:	//	15	//貸借枝番（開始）
		case SCN_DCBR2_PN:	//	16	//貸借枝番（終了）
			return;
		}
		break;
	}

	//#* 部門関係 02.27 /02
	switch( prev_pn ) {	
	case SCN_BMON1_PN:	//	開始借方部門
	case SCN_BMON2_PN:	//	終了借方部門
	case SCN_CBMN1_PN:	//	開始貸方部門
	case SCN_CBMN2_PN:	//	終了貸方部門
		switch( now_pn ) {
		case SCN_BMON1_PN:	//	開始借方部門
		case SCN_BMON2_PN:	//	終了借方部門
		case SCN_CBMN1_PN:	//	開始貸方部門
		case SCN_CBMN2_PN:	//	終了貸方部門
			return;		
		}
		break;
	}

	//ホームページにリセット
	SelPar par;

	if( now_pn == SCN_BMON1_PN || now_pn == SCN_BMON2_PN ||
		now_pn == SCN_CBMN1_PN || now_pn == SCN_CBMN2_PN ) {
		par.sel_pn = SL_BUMON_PN;
		m_Seldata.SelectJob( &par, VK_HOME );
	}
	else if( now_pn != SCN_TKY_PN )
	{
		par.sel_pn = SL_KAMOKU_PN;
		m_Seldata.SelectJob( &par, -1 );

#ifdef SEL_KEEP
	//	if( TKDsw || AC_sel )  ... 02.27 /02
		if( TKDsw || AC_sel || BMNdsw )	
		{
			CDPG = 0;
			KPGcnt = resetpag( CDPG );
		//!	((CDBDinpView*)m_pParent)->m_selval32.SetData( (long)SelVal32 );
			((CDBDinpView*)m_pParent)->SetICSSel32( (long)SelVal32, ! bKamokuCode );
		}
		else
			((CDBDinpView*)m_pParent)->SelItemDsp( VK_HOME );
#endif
	}
}

BEGIN_EVENTSINK_MAP(CScanKey, ICSDialog)
    //{{AFX_EVENTSINK_MAP(CScanKey)
	ON_EVENT(CScanKey, IDC_ICSSELCTRL1, 1 /* Selected */, OnSelectedIcsselctrl1, VTS_I2 VTS_I4)
	ON_EVENT(CScanKey, IDC_ICSSELCTRL2, 1 /* Selected */, OnSelectedIcsselctrl2, VTS_I2 VTS_I4)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT_RANGE(CScanKey, IDC_SCNSEQ1, IDC_SCNTDAT2, 3 /* SetFocus */, OnSetFocusScnDBedt, VTS_I4 )
	ON_EVENT_RANGE(CScanKey, IDC_SCNSEQ1, IDC_SCNTDAT2, 2 /* KillFocus */, OnKillFocusScnDBedt, VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CScanKey, IDC_SCNSEQ1, IDC_SCNTDAT2, 1 /* Termination */, OnTerminationScnDBedt, VTS_I4 VTS_I4 VTS_I4 VTS_I4)	
	ON_EVENT_RANGE(CScanKey, IDC_SCNSEQ1, IDC_SCNTDAT2, 5 /* ImeEndComposition */, OnImeEndCompositionScnDBedt, VTS_I4 VTS_I2 VTS_BSTR VTS_BSTR)
	ON_EVENT_RANGE(CScanKey, IDC_SCNSEQ1, IDC_SCNTDAT2, 4 /* KanaTermination */, OnKanaTerminationScnDBedt, VTS_I4 VTS_BSTR)
END_EVENTSINK_MAP()


void CScanKey::OnSetFocusScnDBedt( UINT ID )
{
	switch( ID ) {
	case IDC_SCNSEQ1:	OnSetFocusScnseq1();	break;
	case IDC_SCNSEQ2: 	OnSetFocusScnseq2();	break;
	case IDC_SCNDATE1:	OnSetFocusScndate1();	break;
	case IDC_SCNDATE2:	OnSetFocusScndate2();	break;
	case IDC_SCNDENP1:	OnSetFocusScndenp1();	break;
	case IDC_SCNDENP2:	OnSetFocusScndenp2();	break;
	case IDC_SCNBMON1:	OnSetFocusScnbmon1();	break;
	case IDC_SCNBMON2:	OnSetFocusScnbmon2();	break;
	case IDC_SCNCBMN1:	OnSetFocusScncbmn1();	break;
	case IDC_SCNCBMN2:	OnSetFocusScncbmn2();	break;
	case IDC_SCNKOJI1:	OnSetFocusScnkoji1();	break;
	case IDC_SCNKOJI2:	OnSetFocusScnkoji2();	break;
	case IDC_SCNCKJI1:	OnSetFocusScnckji1();	break;
	case IDC_SCNCKJI2:	OnSetFocusScnckji2();	break;
	case IDC_SCNDEBT:	OnSetFocusScnDebt();	break;
	case IDC_SCNDBR1: 	OnSetFocusScndbr1();	break;
	case IDC_SCNDBR2: 	OnSetFocusScndbr2();	break;
	case IDC_SCNCRED: 	OnSetFocusScnCred();	break;
	case IDC_SCNCBR1:	OnSetFocusScncbr1();	break; 
	case IDC_SCNCBR2:	OnSetFocusScncbr2();	break; 
	case IDC_SCNDBCR:  	OnSetFocusScnDBcr();	break;
	case IDC_SCNDCBR1:	OnSetFocusScnDCbr1();	break; 
	case IDC_SCNDCBR2:	OnSetFocusScnDCbr2();	break;
	case IDC_SCNVAL1:	OnSetFocusScnval1();	break;
	case IDC_SCNVAL2:	OnSetFocusScnval2();	break;
	case IDC_SCNZEI1:	OnSetFocusScnzei1();	break; 
	case IDC_SCNZEI2:	OnSetFocusScnzei2();	break; 
	case IDC_SCNTKY: 	OnSetFocusScntky();		break; 
	case IDC_SCNSNUM1:	OnSetFocusScnSnum1();	 break;
	case IDC_SCNSNUM2:	OnSetFocusScnSnum2();	break;
	case IDC_SCNTDAT1:	OnSetFocusScnTgdate1();	break;
	case IDC_SCNTDAT2:	OnSetFocusScnTgdate2();	break;
		break;
	default:
		ASSERT( FALSE );
		break;
	}
}

void CScanKey::OnKillFocusScnDBedt( UINT ID, long inplen )
{
	switch( ID ) {
	case IDC_SCNSEQ1:	OnKillFocusScnseq1( inplen);	break;
	case IDC_SCNSEQ2: 	OnKillFocusScnseq2( inplen);	break;
	case IDC_SCNDATE1:	OnKillFocusScndate1(inplen);	break;
	case IDC_SCNDATE2:	OnKillFocusScndate2(inplen);	break;
	case IDC_SCNDENP1:	OnKillFocusScndenp1(inplen);	break;
	case IDC_SCNDENP2:	OnKillFocusScndenp2(inplen);	break;
	case IDC_SCNBMON1:	OnKillFocusScnbmon1(inplen);	break;
	case IDC_SCNBMON2:	OnKillFocusScnbmon2(inplen);	break;
	case IDC_SCNCBMN1:	OnKillFocusScncbmn1(inplen);	break;
	case IDC_SCNCBMN2:	OnKillFocusScncbmn2(inplen);	break;
	case IDC_SCNKOJI1:	OnKillFocusScnkoji1(inplen);	break;
	case IDC_SCNKOJI2:	OnKillFocusScnkoji2(inplen);	break;
	case IDC_SCNCKJI1:	OnKillFocusScnckji1(inplen);	break;
	case IDC_SCNCKJI2:	OnKillFocusScnckji2(inplen);	break;
	case IDC_SCNDEBT:	OnKillFocusScnDebt(inplen);	break;
	case IDC_SCNDBR1:	OnKillFocusScndbr1(inplen);	break;
	case IDC_SCNDBR2:	OnKillFocusScncbr2(inplen);	break;
	case IDC_SCNCRED:	OnKillFocusScnCred(inplen);	break;
	case IDC_SCNCBR1: 	OnKillFocusScncbr1(inplen);	break;
	case IDC_SCNCBR2: 	OnKillFocusScncbr2(inplen);	break;
	case IDC_SCNDBCR:	OnKillFocusScnDBcr(inplen);	break;
	case IDC_SCNDCBR1:	OnKillFocusScnDCbr1(inplen);	break;
	case IDC_SCNDCBR2:	OnKillFocusScnDCbr2(inplen);	break;
	case IDC_SCNVAL1:	OnKillFocusScnval1(inplen);	break;
	case IDC_SCNVAL2:	OnKillFocusScnval2(inplen);	break;
	case IDC_SCNZEI1:	OnKillFocusScnzei1(inplen);	break; 
	case IDC_SCNZEI2:	OnKillFocusScnzei2(inplen);	break; 
	case IDC_SCNTKY: 	OnKillFocusScntky(inplen);	break; 
	case IDC_SCNSNUM1:	OnKillFocusScnSnum1(inplen);	break;
	case IDC_SCNSNUM2:	OnKillFocusScnSnum2(inplen);	break;	
	case IDC_SCNTDAT1:	OnKillFocusScnTgdate1(inplen);	break;
	case IDC_SCNTDAT2:	OnKillFocusScnTgdate2(inplen);	break;
		break;
	default:
		ASSERT( FALSE );
		break;
	}
}

void CScanKey::OnTerminationScnDBedt( UINT ID, long nChar, long inplen, long kst )
{
	switch( ID ) {
	case IDC_SCNSEQ1:	OnTerminationScnseq1(nChar,inplen,kst);	break;
	case IDC_SCNSEQ2: 	OnTerminationScnseq2(nChar,inplen,kst);	break;
	case IDC_SCNDATE1:	OnTerminationScndate1(nChar,inplen,kst);	break;
	case IDC_SCNDATE2:	OnTerminationScndate2(nChar,inplen,kst);	break;
	case IDC_SCNDENP1:	OnTerminationScndenp1(nChar,inplen,kst);	break;
	case IDC_SCNDENP2:	OnTerminationScndenp2(nChar,inplen,kst);	break;
	case IDC_SCNBMON1:	OnTerminationScnbmon1(nChar,inplen,kst);	break;
	case IDC_SCNBMON2:	OnTerminationScnbmon2(nChar,inplen,kst);	break;
	case IDC_SCNCBMN1:	OnTerminationScncbmn1(nChar,inplen,kst);	break;
	case IDC_SCNCBMN2:	OnTerminationScncbmn2(nChar,inplen,kst);	break;
	case IDC_SCNKOJI1:	OnTerminationScnkoji1(nChar,inplen,kst);	break;
	case IDC_SCNKOJI2:	OnTerminationScnkoji2(nChar,inplen,kst);	break;
	case IDC_SCNCKJI1:	OnTerminationScnckji1(nChar,inplen,kst);	break;
	case IDC_SCNCKJI2:	OnTerminationScnckji2(nChar,inplen,kst);	break;
	case IDC_SCNDEBT:	OnTerminationScnDebt(nChar,inplen,kst);	break;
	case IDC_SCNDBR1:	TerminationBrn(SCN_DBR1_PN,nChar,inplen,kst);	break;
	case IDC_SCNDBR2:	TerminationBrn(SCN_DBR2_PN,nChar,inplen,kst);	break;
	case IDC_SCNCRED: 	OnTerminationScnCred(nChar,inplen,kst);	break;
	case IDC_SCNCBR1:	TerminationBrn(SCN_CBR1_PN,nChar,inplen,kst);	break;
	case IDC_SCNCBR2:	TerminationBrn(SCN_CBR2_PN,nChar,inplen,kst);	break;
	case IDC_SCNDBCR: 	OnTerminationScnDBcr(nChar,inplen,kst);	break;
	case IDC_SCNDCBR1: 	TerminationBrn(SCN_DCBR1_PN,nChar,inplen,kst);	break;
	case IDC_SCNDCBR2:	TerminationBrn(SCN_DCBR2_PN,nChar,inplen,kst);	break;
	case IDC_SCNVAL1:	OnTerminationScnval1(nChar,inplen,kst);	break;
	case IDC_SCNVAL2:	OnTerminationScnval2(nChar,inplen,kst);	break;
	case IDC_SCNZEI1: 	OnTerminationScnzei1(nChar,inplen,kst);	break;
	case IDC_SCNZEI2: 	OnTerminationScnzei2(nChar,inplen,kst);	break;
	case IDC_SCNTKY:	OnTerminationScntky(nChar,inplen,kst);	break;
	case IDC_SCNSNUM1:	OnTerminationScnSnum1(nChar,inplen,kst);	break;
	case IDC_SCNSNUM2:	OnTerminationScnSnum2(nChar,inplen,kst);	break;
	case IDC_SCNTDAT1:	OnTerminationScnTgdate1(nChar,inplen,kst);	break;
	case IDC_SCNTDAT2:	OnTerminationScnTgdate2(nChar,inplen,kst);	break;
		break;
	default:
		ASSERT( FALSE );
		break;
	}
}


void CScanKey::OnImeEndCompositionScnDBedt(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring) 
{
	switch( ID ) {
	case IDC_SCNDEBT:
		if( ItemDataCheck( SCN_DEBT_PN, nChar, string, 0 ) <= 0 )
			return;
		OnTerminationScnDebt( nChar, 0, 0 );
		break;
	case IDC_SCNCRED:
		if( ItemDataCheck( SCN_CRED_PN, nChar, string, 1 ) <= 0 )
			return;
		OnTerminationScnCred( nChar, 0, 0 );
		break;
	case IDC_SCNDBCR:
		if( ItemDataCheck( SCN_DBCR_PN, nChar, string, 1 ) <= 0 )
			return;
		OnTerminationScnDBcr( nChar, 0, 0 );
		break;

	case IDC_SCNTKY:
		if( nChar == VK_TAB ) {
			OnKanaTermScntky( ystring );
		}
		break;
	default:
		break;
	}
}


void CScanKey::OnKanaTerminationScnDBedt(UINT ID, LPCTSTR Kana) 
{
	ItemKanaSearch( (long)Kana );
}


//////////////////////////////////////////////////////////////////////
// seq
void CScanKey::OnSetFocusScnseq1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.15 /00
		MyRecognizeON( SPCID_DENPYO );	// MyRecognizeON( SPCID_BANGO );
#endif
		set_nowpn( SCN_SEQ1_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_SEQ1_PN ); 
}

void CScanKey::OnKillFocusScnseq1( long inplen ) 
{
	if( m_DIALOG_OK == FALSE )
		return;

	SeqInp( SCN_SEQ1_PN, &SCAN_KEY.SCN_SEQ1_DT, 0, inplen, 0);
}

void CScanKey::OnTerminationScnseq1(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = SeqInp( SCN_SEQ1_PN, &SCAN_KEY.SCN_SEQ1_DT, nChar, inplen, kst )) )
			return;

		//カーソル移動
		if( term_focus( SCN_SEQ1_PN, nChar ) != 0 )
			Buzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

void CScanKey::OnSetFocusScnseq2() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.15 /00
		MyRecognizeON( SPCID_DENPYO );	// MyRecognizeON( SPCID_BANGO );
#endif
		set_nowpn( SCN_SEQ2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_SEQ2_PN ); 
}

void CScanKey::OnKillFocusScnseq2( long inplen ) 
{
	if( m_DIALOG_OK == FALSE )
		return;

	SeqInp( SCN_SEQ2_PN, &SCAN_KEY.SCN_SEQ2_DT, 0, inplen, 0);
}

void CScanKey::OnTerminationScnseq2(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = SeqInp( SCN_SEQ2_PN, &SCAN_KEY.SCN_SEQ2_DT, nChar, inplen, kst )) )
			return;

		if( term_focus( SCN_SEQ2_PN, nChar ) != 0 )
			Buzzer();
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}


//
// スキャンキーＡｌｌクリア
void CScanKey::ScanKeyAllDel()
{
#ifndef SPEECH	// 05.19 /00
	switch( ICSMessageBox( "全検索項目の消去を行います。\n\nよろしいですか。", MB_YESNO,0,0,this ) )
	{
	case IDNO:
		return;
	}
#endif

	keyline_inz();	// コントロールテーブルの初期化＆表示

//	set_topfocus();
}

//
// ＳＥＱ番号　入力＆オフフォーカス
long CScanKey::SeqInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	VARIANT var;
	char tmp[64];

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		check.IP_SEQ = atoi( (char*)var.pbVal );

		if( check.IP_SEQ > 0 )
		{
			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
			keybuf->IP_SEQ = check.IP_SEQ;
		}
		else
			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;

		//データセット
		if( keybuf->IP_SEQ > 0 ) {
			sprintf_s( tmp, sizeof tmp, "%d", keybuf->IP_SEQ );
			var.pbVal = (BYTE*)tmp;
		}
		else {
			var.pbVal = NULL;
		}
		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		return(0);
	}
	else
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return VK_F2;
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{
//		data_get( scGetINP_CTL( pn )->IDC_X, &check, CICSInput );
//		if( check.IP_SEQ )
//		{
//			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
//			keybuf->IP_SEQ = check.IP_SEQ;
//		}
	}
	else if( nChar == VK_DELETE )
	{
		check.IP_SEQ = 0;
		var.pbVal = NULL;

		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		keybuf->IP_SEQ = check.IP_SEQ;
		SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}

//////////////////////////////////////////////////////////////////////
// 日付
void CScanKey::OnSetFocusScndate1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.15 /00
		MyRecognizeON( SPCID_DENPYO );	// MyRecognizeON( SPCID_BANGO );
#endif
		set_nowpn( SCN_DATE1_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_DATE1_PN ); 
}

void CScanKey::OnKillFocusScndate1( long inplen ) 
{
	if( m_DIALOG_OK == FALSE )
		return;

	DateInp( SCN_DATE1_PN, &SCAN_KEY.SCN_DATE1_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScndate1(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = DateInp( SCN_DATE1_PN, &SCAN_KEY.SCN_DATE1_DT, nChar, inplen, kst )) )
			return;

		//カーソル移動
		if( term_focus( SCN_DATE1_PN, nChar ) != 0 )
			Buzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

void CScanKey::OnSetFocusScndate2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.15 /00
		MyRecognizeON( SPCID_DENPYO );	// MyRecognizeON( SPCID_BANGO );
#endif
		set_nowpn( SCN_DATE2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_DATE2_PN ); 	
}

void CScanKey::OnKillFocusScndate2( long inplen ) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

	DateInp( SCN_DATE2_PN, &SCAN_KEY.SCN_DATE2_DT, 0, inplen, 0 );	
}

void CScanKey::OnTerminationScndate2(long nChar, long inplen, long kst) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = DateInp( SCN_DATE2_PN, &SCAN_KEY.SCN_DATE2_DT, nChar, inplen, kst )) )
			return;

		//カーソル移動
		if( term_focus( SCN_DATE2_PN, nChar ) != 0 )
			Buzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

// 日付チェックデータ
long CScanKey::CheckDataIcsinputDate( int pn, long nChar ) 
{
	char check[3] = {0};
	char ymd[3] = {0};
	char data[20] = {0};
	BYTE bcd[10] = {0};
	VARIANT var;
	CString str;

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	//
//	try
	{
	//	DBdata_get( pwnd, scGetINP_CTL(DATE_PN)->IDC_X, &var, TYPE_BCD, 4 );
		DBdata_get( this, scGetINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

		if( var.pbVal != NULL )	{
			int len = strlen( (char*)var.pbVal );
			if( len > 2 ) {
				strncpy_s( data, sizeof data, (char*)var.pbVal, 4 );
			}
			else {
				int sgn = 0;
				if( len == 2 ) {
					if( var.pbVal[0] == '6' || (var.pbVal[0] == '0' && var.pbVal[1] == '0') ) {
						strncpy_s( data, sizeof data, (char*)var.pbVal, len );
						sgn = 1;
					}
				}

				if( !sgn ) {
					strcpy_s( data, sizeof data, "00" );
					if( len == 1 ) {
						data[2] = '0';
						data[3] = var.pbVal[0];
					}
					else	strncpy_s( &data[2], sizeof(data)-2, (char*)var.pbVal, 2 );
				}
			}
		}

		if( DateInputCheck( data, bcd ) != 0 ) {
			// 不正データが入力された
			var.pbVal = SCAN_KEY._XXDTA2[pn].IP_BCDDAY;
			DBdata_set( this, scGetINP_CTL( pn)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
			BcdDateToString( str, SCAN_KEY._XXDTA2[pn].IP_BCDDAY );
			DBSetDispString( this, scGetINP_CTL(pn)->IDC_X, str );
			return -1;
		}
		else {
			memcpy( check, bcd, 3 );
		}

/*		if( !check[1] )
		{
			check[0] = 0;
			check[1] = (char)0xff;
		}
*/
		CVolDateDB vd;
//		if( /* !m_pZm->vd_chek( (unsigned char*)check ) && !m_pZm->vd_sign( (unsigned char*)check, (unsigned char*)ymd ) */ )
//		if( !m_pZm->Myvd_chek( (unsigned char*)check ) && !m_pZm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd ) )
		if( /*!vd.db_vd_check( (BYTE*)check, m_pZm ) &&*/ ! Myvd_sign( (unsigned char*)check, (unsigned char*)ymd, m_pZm ) )
		{
			memmove( bcd, &ymd[0], 2 );
		//	sprintf( data, "%02x%02x%02x", bcd[0], bcd[1] );
		//	var.pbVal = (BYTE*)data;
		//	DBdata_set( pwnd, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			var.pbVal = bcd;
			DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
		}
		else
		{
			Buzzer();
			memset( bcd, 0, sizeof bcd );
			var.pbVal = bcd;
			DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
			BcdDateToString( str, SCAN_KEY._XXDTA2[pn].IP_BCDDAY );
			DBSetDispString( this, scGetINP_CTL( pn )->IDC_X, str );
			return -1;
		}
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return -1;
	}

	return nChar;
}

//
// 日付　入力＆オフフォーカス
long CScanKey::DateInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check = {0};
	VARIANT var;
	CString str;

	// 日付文字列チェック＆セット
	if( inplen > 0 )	CheckDataIcsinputDate( pn, nChar );
	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
		memcpy( check.IP_BCDDAY, var.pbVal, 3 );
		
		if( check.IP_BCDDAY[0] )
		{
			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
			memmove( keybuf->IP_BCDDAY, check.IP_BCDDAY, sizeof(keybuf->IP_BCDDAY) );
		}
		else
			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
		//データセット

		BcdDateToString( str, check.IP_BCDDAY );
		DBSetDispString( this, scGetINP_CTL( pn )->IDC_X, str );
	//	DBdata_set( scGetINP_CTL( pn )->IDC_X, keybuf, CICSInput );

		return(0);
	}
	else
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return VK_F2;
	}
	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB )
	{
//			data_get( scGetINP_CTL( pn )->IDC_X, &check, CICSInput );
//			if( check.IP_DAY[0] )
//			{
//				SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
//
//				memmove( keybuf->IP_DAY, check.IP_DAY, sizeof(keybuf->IP_DAY) );
//			}
	}
	else if( nChar == VK_DELETE )
	{
		memset( (char *)keybuf, (char)0, sizeof(struct _ItemData) );
//		memset( (char*)keybuf->IP_DAY, (char)0xff, sizeof(keybuf->IP_DAY));
		var.pbVal = (BYTE*)keybuf->IP_BCDDAY;
		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );

		SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}

///////////////////////////////////////////////////////////////////////////////
// 伝票番号
void CScanKey::OnSetFocusScndenp1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.15 /00
		MyRecognizeON( SPCID_DENPYO );	// MyRecognizeON( SPCID_BANGO );
#endif
		set_nowpn( SCN_DENP1_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_DENP1_PN ); 	
}

void CScanKey::OnKillFocusScndenp1( long inplen ) 
{
	DenpInp( SCN_DENP1_PN, &SCAN_KEY.SCN_DENP1_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScndenp1(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = DenpInp( SCN_DENP1_PN, &SCAN_KEY.SCN_DENP1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_DENP1_PN, nChar ) != 0 )
			Buzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

void CScanKey::OnSetFocusScndenp2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.15 /00
		MyRecognizeON( SPCID_DENPYO );	// MyRecognizeON( SPCID_BANGO );
#endif
		set_nowpn( SCN_DENP2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_DENP2_PN );	
}

void CScanKey::OnKillFocusScndenp2( long inplen ) 
{
	DenpInp( SCN_DENP2_PN, &SCAN_KEY.SCN_DENP2_DT, 0, inplen, 0 );	
}

void CScanKey::OnTerminationScndenp2(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = DenpInp( SCN_DENP2_PN, &SCAN_KEY.SCN_DENP2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_DENP2_PN, nChar ) != 0 )
			Buzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}	
}

// 伝票番号　入力＆オフフォーカス
long CScanKey::DenpInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	VARIANT var;

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		if( var.pbVal[0] != 0 )	check.IP_DENP = atoi( (LPCTSTR)var.pbVal );
		else					check.IP_DENP = -1;

		if( check.IP_DENP != -1 )
		{
			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
			keybuf->IP_DENP = check.IP_DENP;
		}
		else
			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;

		//データセット
	//	data_set( scGetINP_CTL( pn )->IDC_X, keybuf, CICSInput );
		return( 0 );
	}
	else
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return VK_F2;
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{
//			data_get( scGetINP_CTL( pn )->IDC_X, &check, CICSInput );
//			if( check.IP_DENP != 0xffff )
//			{
//				SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
//	
//				keybuf->IP_DENP = check.IP_DENP;
//			}
	}
	else if( nChar == VK_DELETE )
	{
		check.IP_DENP = -1;
		var.pbVal = NULL;
		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	
		keybuf->IP_DENP = check.IP_DENP;
		SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}

//////////////////////////////////////////////////////////////////////////////////////
// 借方部門

void CScanKey::ScnbmonFocusSub( int pn )
{
	if( m_DIALOG_OK == FALSE )
		return;

	SelPar par;

//	try
	{
#ifdef SPEECH	// 05.12 /00
		MyRecognizeON( SPCID_BUMON );
#endif
		// 選択部分に 部門名称を表示
		switch( get_nowpn() ) {
		case SCN_BMON1_PN:
		case SCN_BMON2_PN:
		case SCN_CBMN1_PN:
		case SCN_CBMN2_PN:
			break;
		default:
			par.sel_pn = SL_BUMON_PN;
			m_Seldata.SelectJob( &par, -1 );
			break;
		}
		// 部門名称セット
		set_etcdsp( pn );
		// 部門名称表示
		dsp_etc( pn, TRUE );	// 今回ポジションを表示

		set_nowpn( pn );		
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}	
}

void CScanKey::OnSetFocusScnbmon1() 
{
	ScnbmonFocusSub( SCN_BMON1_PN );
}
void CScanKey::OnKillFocusScnbmon1( long inplen ) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	BmonInp( SCN_BMON1_PN, &SCAN_KEY.SCN_BMON1_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnbmon1(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = BmonInp( SCN_BMON1_PN, &SCAN_KEY.SCN_BMON1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_BMON1_PN, nChar ) != 0 )
			Buzzer();

//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

void CScanKey::OnSetFocusScnbmon2() 
{
	ScnbmonFocusSub( SCN_BMON2_PN );
}
void CScanKey::OnKillFocusScnbmon2( long inplen ) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	BmonInp( SCN_BMON2_PN, &SCAN_KEY.SCN_BMON2_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnbmon2(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = BmonInp( SCN_BMON2_PN, &SCAN_KEY.SCN_BMON2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_BMON2_PN, nChar ) != 0 )
			Buzzer();

//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

//////////////////////////////////////////////////////////////////////////////////////
// 貸方部門
void CScanKey::OnSetFocusScncbmn1() 
{
	ScnbmonFocusSub( SCN_CBMN1_PN );
}

void CScanKey::OnKillFocusScncbmn1( long inplen ) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	BmonInp( SCN_CBMN1_PN, &SCAN_KEY.SCN_CBMN1_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScncbmn1(long nChar, long inplen, long kst) 
{
//	try 
	{
		// 入力＆オフフォーカス
		if( !(nChar = BmonInp( SCN_CBMN1_PN, &SCAN_KEY.SCN_CBMN1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_CBMN1_PN, nChar ) != 0 )
			Buzzer();

	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

void CScanKey::OnSetFocusScncbmn2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	ScnbmonFocusSub( SCN_CBMN2_PN );

}

void CScanKey::OnKillFocusScncbmn2( long inplen ) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	BmonInp( SCN_CBMN2_PN, &SCAN_KEY.SCN_CBMN2_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScncbmn2(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = BmonInp( SCN_CBMN2_PN, &SCAN_KEY.SCN_CBMN2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_CBMN2_PN, nChar ) != 0 )
			Buzzer();

//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

// 部門　入力＆オフフォーカス
long CScanKey::BmonInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	CDBipDataRec temp;	// 部門コード取得用
	int number;
	int n = 0;
	int ret = 0;
	char str[60];
	VARIANT var;

	if( inplen > 0 ) {
		//データリード
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		n = strlen( (char*)var.pbVal );
		check.IP_BMN = -1;

#ifdef _2KT_BSEL	// 10.01 /02
		if( n && n < 3 ) {
			number = atoi( (char*)var.pbVal );

			int bmcode;
			if( bmncode_todata( &bmcode, number, 0 ) != FALSE ) {
				check.IP_BMN = bmcode;
			}
			else {
				// 選択ミス//データセット(再表示)		
				check.IP_BMN = keybuf->IP_BMN;
				ret = -1;			
			}
		}
		else {
#endif
			if( strlen( (char*)var.pbVal ) )
			{
				check.IP_BMN = atoi( (char*)var.pbVal );
			}
#ifdef _2KT_BSEL	// 10.01 /02
		}
#endif

		if( check.IP_BMN != -1 ) {
			set_codestr( str, sizeof str, check.IP_BMN, Voln1->bmcol );
			var.pbVal = (BYTE*)str;
		}
		else	var.pbVal = NULL;
		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		check.IP_BMN = -1;
		if( strlen( (char*)var.pbVal ) ) {
			check.IP_BMN = atoi( (char*)var.pbVal );
		}

		if( check.IP_BMN != -1)
		{
			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
			keybuf->IP_BMN = check.IP_BMN;
		}
		else
			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
			
		//データセット
		if( keybuf->IP_BMN != -1 ) {
			set_codestr( str, sizeof str, keybuf->IP_BMN, Voln1->bmcol );
			var.pbVal = (BYTE*)str;
		}
		else	var.pbVal = NULL;

		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		dsp_etc( pn, FALSE );	// 今回ポジションを非表示

		return( ret );
	}
	else
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return VK_F2;

		SelPar par;
		par.sel_pn = SL_BUMON_PN;
		m_Seldata.SelectJob( &par, nChar );

	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{
#ifndef _2KT_BSEL	// 10.01 /02
		if( nChar == VK_TAB ) {		// 選択画面より番号選択
			if( strlen( data ) ) {
				number = atoi( data );

				if( bmncode_todata( &temp, number ) != FALSE ) {
					check.IP_BMN = temp.m_dbmn;
					keybuf->IP_BMN = check.IP_BMN;
				//	::ZeroMemory( str, sizeof(str));
					if( keybuf->IP_BMN != -1 ) {
						m_pZm->BumonCodeToStr( str, keybuf->IP_BMN );
						var.pbVal = (BYTE*)str;
					}
					else	var.pbVal = NULL;

					DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				}
			}
		}
#endif
	}
	else if( nChar == VK_DELETE )
	{
		var.pbVal = NULL;
		keybuf->IP_BMN = -1;

		SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	return( nChar );
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 借方工事

void CScanKey::ScnKojiFocusSub( int pn )
{
	if( m_DIALOG_OK == FALSE )
		return;
//	try
	{
#ifdef SPEECH	// 05.12 /00
		MyRecognizeON( SPCID_BUMON );
#endif
		// 選択部分に 工事名称を表示
		switch( get_nowpn() ) {
		case SCN_KOJI1_PN:
		case SCN_KOJI2_PN:
		case SCN_CKJI1_PN:
		case SCN_CKJI2_PN:
			break;
		default:
			break;
		}
		// 工事名称セット
		set_etcdsp( pn );
		// 工事名称表示
		dsp_etc( pn, TRUE );	// 今回ポジションを表示

		set_nowpn( pn );		
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}


void CScanKey::OnSetFocusScnkoji1() 
{
	ScnKojiFocusSub( SCN_KOJI1_PN );
}
void CScanKey::OnKillFocusScnkoji1( long inplen ) 
{
	KojiInp( SCN_KOJI1_PN, &SCAN_KEY.SCN_KOJI1_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnkoji1(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = KojiInp( SCN_KOJI1_PN, &SCAN_KEY.SCN_KOJI1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_KOJI1_PN, nChar ) != 0 )
			Buzzer();

//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

void CScanKey::OnSetFocusScnkoji2() 
{
	ScnKojiFocusSub( SCN_KOJI2_PN );
}
void CScanKey::OnKillFocusScnkoji2( long inplen ) 
{
	KojiInp( SCN_KOJI2_PN, &SCAN_KEY.SCN_KOJI2_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnkoji2(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = KojiInp( SCN_KOJI2_PN, &SCAN_KEY.SCN_KOJI2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_KOJI2_PN, nChar ) != 0 )
			Buzzer();


//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 貸方工事
void CScanKey::OnSetFocusScnckji1() 
{
	ScnKojiFocusSub( SCN_CKJI1_PN );
}
void CScanKey::OnKillFocusScnckji1( long inplen ) 
{
	KojiInp( SCN_CKJI1_PN, &SCAN_KEY.SCN_CKJI1_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnckji1(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = KojiInp( SCN_CKJI1_PN, &SCAN_KEY.SCN_CKJI1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_CKJI1_PN, nChar ) != 0 )
			Buzzer();
//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

void CScanKey::OnSetFocusScnckji2() 
{
	ScnKojiFocusSub( SCN_CKJI2_PN );
}
void CScanKey::OnKillFocusScnckji2( long inplen ) 
{
	KojiInp( SCN_CKJI2_PN, &SCAN_KEY.SCN_CKJI2_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnckji2(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = KojiInp( SCN_CKJI2_PN, &SCAN_KEY.SCN_CKJI2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_CKJI2_PN, nChar ) != 0 )
			Buzzer();


//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

// 工事　入力＆オフフォーカス
long CScanKey::KojiInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	CDBipDataRec temp;	// 部門コード取得用
	int number;
	int n = 0;
	int ret = 0;
	char str[60];
	VARIANT var;

	if( inplen > 0 ) {
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		n = strlen( (char*)var.pbVal );
		check.IP_BMN = -1;

#ifdef _2KT_BSEL	// 10.01 /02
		if( n && n < 3 ) {
			number = atoi( (char*)var.pbVal );

			CString kjcd;
			if( kojicode_todata( kjcd, number, 0 ) != FALSE ) {
				strcpy_s( (char*)check.IP_KOJI, sizeof check.IP_KOJI, kjcd );
			}
			else {
				// 選択ミス//データセット(再表示)		
				strcpy_s( (char*)check.IP_KOJI, sizeof check.IP_KOJI, (char*)keybuf->IP_KOJI );
				ret = -1;			
			}
		}
		else {
#endif
			if( strlen( (char*)var.pbVal ) )
			{
				strcpy_s( (char*)check.IP_KOJI, sizeof check.IP_KOJI, (char*)var.pbVal );
			}
#ifdef _2KT_BSEL	// 10.01 /02
		}
#endif

		if( check.IP_KOJI[0] != '\0' ) {
			set_codestring( str, sizeof str, (char*)check.IP_KOJI, Voln1->kjcol );
			var.pbVal = (BYTE*)str;
		}
		else	var.pbVal = NULL;
		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		::ZeroMemory( &check.IP_KOJI, sizeof check.IP_KOJI );
		if( strlen( (char*)var.pbVal ) ) {
			strcpy_s( (char*)&check.IP_KOJI, sizeof check.IP_KOJI, (char*)var.pbVal );
		}

		if( check.IP_KOJI[0] != 0 )
		{
			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
			strcpy_s( (char*)keybuf->IP_KOJI, sizeof keybuf->IP_KOJI, (char*) check.IP_KOJI );
		}
		else {
			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
			::ZeroMemory( keybuf->IP_KOJI, sizeof keybuf->IP_KOJI );
		}
			
		//データセット
		if( keybuf->IP_KOJI[0] != 0 ) {
			var.pbVal = (BYTE*)keybuf->IP_KOJI;
		}
		else	var.pbVal = NULL;

		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		dsp_etc( pn, FALSE );	// 今回ポジションを非表示

		return( ret );
	}
	else
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return VK_F2;

		SelPar par;
		par.sel_pn = SL_KOJI_PN;
		m_Seldata.SelectJob( &par, nChar );

	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{
#ifndef _2KT_BSEL	// 10.01 /02
		if( nChar == VK_TAB ) {		// 選択画面より番号選択
			if( strlen( data ) ) {
				number = atoi( data );

				if( kojicode_todata( &temp, number ) != FALSE ) {
					strcpy( check.IP_KOJI, temp.m_dkno );
					strcpy( keybuf->IP_KOJI, check.IP_KOJI );
				//	::ZeroMemory( str, sizeof(str));
					if( keybuf->IP_KOJI[0] != '\0' ) {
						set_codestring( str, (char*)keybuf->IP_KOJI, Voln1->kjcol );
						var.pbVal = (BYTE*)str;
					}
					else	var.pbVal = NULL;

					DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				}
			}
		}
#endif
	}
	else if( nChar == VK_DELETE )
	{
		var.pbVal = NULL;
		::ZeroMemory( keybuf->IP_KOJI, sizeof keybuf->IP_KOJI );

		SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	return( nChar );
}

///////////////////////////////////////////////////////////////////////////////////////
// 借方科目
void CScanKey::OnSetFocusScnDebt() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;
//	try
	{
#ifdef SPEECH	// 05.12 /00
		MyRecognizeON( SPCID_KAMOKU );
#endif

#ifdef SEL_KEEP
		((CDBDinpView*)m_pParent)->Dsp_TkyToItem();	// 摘要関係表示から科目関係表示に
#endif
		set_nowpn( SCN_DEBT_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_DEBT_PN );
}

void CScanKey::OnKillFocusScnDebt( long inplen ) 
{
	ItemInp( SCN_DEBT_PN, &SCAN_KEY.SCN_DEBT_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnDebt(long nChar, long inplen, long kst )
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		nChar = ItemInp( SCN_DEBT_PN, &SCAN_KEY.SCN_DEBT_DT, nChar, inplen, kst );
		if( nChar == -1 )
		{
			Buzzer();
			return;
		}
		else if( !nChar )
			return;

		//カーソル移動
		if( term_focus( SCN_DEBT_PN, nChar ) != 0 )
			Buzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// 貸方科目
void CScanKey::OnSetFocusScnCred() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;
//	try
	{
#ifdef SPEECH	// 05.12 /00
		MyRecognizeON( SPCID_KAMOKU );
#endif
#ifdef SEL_KEEP
		((CDBDinpView*)m_pParent)->Dsp_TkyToItem();	// 摘要関係表示から科目関係表示に
#endif

		set_nowpn( SCN_CRED_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_CRED_PN );
}

void CScanKey::OnKillFocusScnCred( long inplen ) 
{
	ItemInp( SCN_CRED_PN, &SCAN_KEY.SCN_CRED_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnCred(long nChar, long inplen, long kst )
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		nChar = ItemInp( SCN_CRED_PN, &SCAN_KEY.SCN_CRED_DT, nChar, inplen, kst );
		if( nChar == -1 )
		{
			Buzzer();
			return;
		}
		else if( !nChar )
			return;

		//カーソル移動
		if( term_focus( SCN_CRED_PN, nChar ) != 0 )
			Buzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

//////////////////////////////////////////////////////////////////////////////////
// 借 ／ 貸
void CScanKey::OnSetFocusScnDBcr() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;
//	try
	{
#ifdef SPEECH	// 05.12 /00
		MyRecognizeON( SPCID_KAMOKU );
#endif
#ifdef SEL_KEEP
		((CDBDinpView*)m_pParent)->Dsp_TkyToItem();	// 摘要関係表示から科目関係表示に
#endif
		set_nowpn( SCN_DBCR_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_DBCR_PN );
}

void CScanKey::OnKillFocusScnDBcr( long inplen ) 
{
	ItemInp( SCN_DBCR_PN, &SCAN_KEY.SCN_DBCR_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnDBcr(long nChar, long inplen, long kst )
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		nChar = ItemInp( SCN_DBCR_PN, &SCAN_KEY.SCN_DBCR_DT, nChar, inplen, kst );
		if( nChar == -1 )
		{
			Buzzer();
			return;
		}
		else if( !nChar )
			return;

		//カーソル移動
		if( term_focus( SCN_DBCR_PN, nChar ) != 0 )
			Buzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

//
// 科目カナ検索
BOOL CScanKey::ItemKanaSearch( long data) 
{
	char kana[10];

	strcpy_s( kana, sizeof kana, (char *)data );

	SelPar par;
	par.sel_pn = SL_KMKANA_PN;
	strcpy_s( par.kmk_kana, sizeof par.kmk_kana, (char*)data );
	
	m_Seldata.SelectJob( &par, -1 );

	return TRUE;

}

//
// 科目チェック
long CScanKey::ItemDataCheck(int pn, long nChar, LPCTSTR string, int dcsw ) 
{
	struct _ItemData check,ans;

//	memmove( (char *)&check, (char *)data, sizeof(check) );
	KamokuStr_ChkLength( string, &check.IP_stKMK, m_pZm->zvol->edcol );

	if( InputItemCheck( ans.IP_stKMK, check.IP_stKMK, dcsw ) == TRUE )
	{
		DBkamokudata_set( this, scGetINP_CTL( pn )->IDC_X, &ans.IP_stKMK, m_pZm);
//		data_reply( scGetINP_CTL( pn )->IDC_X, 1, &ans, CICSInput );
	}
	else
	{
		Buzzer();

		memset( (char *)&ans, (char)0, sizeof(ans) );
		ans.IP_stKMK.kd_eda = -1;
		DBkamokudata_set( this, scGetINP_CTL( pn )->IDC_X, &ans.IP_stKMK, m_pZm );

//		if( SCAN_KEY.LINE_CND[ pn ].INP_sg )
//			data_reply( scGetINP_CTL( pn )->IDC_X, 1, &ans, CICSInput );
//		else
//			data_reply( scGetINP_CTL( pn )->IDC_X, -1, &ans, CICSInput );
//		set_focus( pn );

		return 0;
	}
	return nChar;
}

//
// 科目　入力＆オフフォーカス
long CScanKey::ItemInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;

	// オフフォーカス
//	if( !nChar )
//	{
		//データリード
		DBkamokudata_get( this, scGetINP_CTL( pn )->IDC_X, &check.IP_stKMK, m_pZm);

		if( check.IP_CdNum[0] == 0 )
		{
			DBkamokudata_set( this, scGetINP_CTL( pn )->IDC_X, &keybuf->IP_stKMK, m_pZm );
			if( nChar == VK_PRIOR || nChar == VK_NEXT || nChar == VK_HOME )
				goto NEXTPOS;
			
			if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
				nChar = VK_F2;

			return (inplen == 0) ? nChar : (-1);
		}
		else if( check.IP_CdNum )
		{
			memmove( keybuf, &check, sizeof(struct _ItemData));

			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
		}
		else
			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;

		//データセット
		DBkamokudata_set( this, scGetINP_CTL( pn )->IDC_X, &keybuf->IP_stKMK, m_pZm );
//		return(0);
//	}
//	else
//	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return VK_F2;
//	}

	if( !nChar )
		return 0;

	// ページアップ／ダウン
NEXTPOS:
	if( nChar )
	{
		SelPar par;
		par.sel_pn = SL_KAMOKU_PN;
		m_Seldata.SelectJob( &par, nChar );

#ifdef SEL_KEEP
		nChar = ((CDBDinpView*)m_pParent)->SelItemDsp( nChar );
#endif
		if( !nChar )
			return(0);
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{

	}
	else if( nChar == VK_DELETE )
	{
		memset( (char*)keybuf, (char)0, sizeof(struct _ItemData) );
		keybuf->IP_BRN = -1;
		DBkamokudata_set( this, scGetINP_CTL( pn )->IDC_X, &keybuf->IP_stKMK, m_pZm );		

		SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}


//////////////////////////////////////////////////////////////////////////////////////
// 枝番関係
//

// 枝番フォーカス処理
void CScanKey::FocusScnBrnSub( int pn )
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.15 /00
		MyRecognizeON( SPCID_DENPYO );	// MyRecognizeON( SPCID_BANGO );
#endif
		// 資金繰諸口枝番等セット
		set_etcdsp( pn );
		// 枝番摘要表示
		dsp_etc( pn, TRUE );	// 今回ポジションを表示
		set_nowpn( pn );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}
// 借方 １
void CScanKey::OnSetFocusScndbr1() 
{
	FocusScnBrnSub( SCN_DBR1_PN );
}

void CScanKey::OnKillFocusScndbr1( long inplen ) 
{
	TerminationBrn( SCN_DBR1_PN, 0, inplen, 0 );
}
// 借方 ２
void CScanKey::OnSetFocusScndbr2()
{
	FocusScnBrnSub( SCN_DBR2_PN );
}

void CScanKey::OnKillFocusScndbr2( long inplen )
{
	TerminationBrn( SCN_DBR2_PN, 0, inplen, 0 );
}
// 貸方 １
void CScanKey::OnSetFocusScncbr1()
{
	FocusScnBrnSub( SCN_CBR1_PN );
}

void CScanKey::OnKillFocusScncbr1( long inplen )
{
	TerminationBrn( SCN_CBR1_PN, 0, inplen, 0 );
}
// 貸方 ２
void CScanKey::OnSetFocusScncbr2()
{
	FocusScnBrnSub( SCN_CBR2_PN );
}

void CScanKey::OnKillFocusScncbr2( long inplen )
{
	TerminationBrn( SCN_CBR2_PN, 0, inplen, 0 );
}
// 借／貸 １
void CScanKey::OnSetFocusScnDCbr1()
{
	FocusScnBrnSub( SCN_DCBR1_PN );
}

void CScanKey::OnKillFocusScnDCbr1( long inplen )
{
	TerminationBrn( SCN_DCBR1_PN, 0, inplen, 0 );
}
// 借／貸 ２
void CScanKey::OnSetFocusScnDCbr2()
{
	FocusScnBrnSub( SCN_DCBR2_PN );
}

void CScanKey::OnKillFocusScnDCbr2( long inplen )
{
	TerminationBrn( SCN_DCBR2_PN, 0, inplen, 0 );
}

// 枝番データ取得
struct _ItemData *CScanKey::get_brndata( int pn )
{
	struct _ItemData *brn;

	switch( pn )
	{
	case SCN_DBR1_PN:
	case SCN_DBR2_PN:
	case SCN_CBR1_PN:
	case SCN_CBR2_PN:
	case SCN_DCBR1_PN:
	case SCN_DCBR2_PN:
		brn = _getDATA( pn );
		break;
	default:
		return 0;
	}
	return brn;
}

// ターミネーション枝番
void CScanKey::TerminationBrn(int pn, long nChar, long inplen, long kst ) 
{
	struct _ItemData *keybuf;

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		if( keybuf = get_brndata( pn ) )
		{
			// 入力＆オフフォーカス
			if( !(nChar = BrInp( pn, keybuf, nChar, inplen, kst )) )
			{
				return;
			}
			// カーソルセット
		//	brinp_to_next( pn, nChar );
			term_focus( pn, nChar );
		}
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

// 枝番　入力＆オフフォーカス
long CScanKey::BrInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	char str[128];
	int n = 0;
	int ret = 0;
	VARIANT var;

	if( inplen > 0 )
	{
		//データリード
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		n = strlen( (char*)var.pbVal );

		if( BRNTKY_sel && n && n < 3 )
		{
			// 枝番摘要選択
			if( brnsel_set( pn, atoi( (char*)var.pbVal ) ) )
				ret = 0;
			else
			{
				// 選択ミス
				//データセット(再表示)
				if( keybuf->IP_cBRN != -1 ) {
					set_codestr( str, sizeof str, keybuf->IP_cBRN, Voln1->edcol );
					var.pbVal = (BYTE*)str;
				}
				else	var.pbVal = NULL;

				DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			//	PostMessage( WM_RETROFOCUS, pn );
				ret = -1;
			}
		}
		else
		{
			if( n )
			{
				SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
				
				keybuf->IP_cBRN = atoi( (char*)var.pbVal );
				set_codestr( str, sizeof str, keybuf->IP_cBRN, Voln1->edcol );
				var.pbVal = (BYTE*)str;
			}
			else {
				SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
				var.pbVal = NULL;
			}
			//データセット
			DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			ret = 0;
		}
		// 枝番摘要表示
		dsp_etc( pn, FALSE );	// 今回ポジションを非表示
	}

	if( ! nChar ) {
		return ret;
	}
	else {
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return VK_F2;
	}

	// ページアップ／ダウン
	if( nChar && BRNTKY_sel )
	{
		SelPar par;
		par.sel_pn = SL_BRNTKY_PN;
		m_Seldata.SelectJob( &par, nChar );

#ifdef SEL_KEEP
		nChar = ((CDBDinpView*)m_pParent)->SelItemDsp( nChar );
#endif
		if( !nChar )
			return(0);
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{
	}
	else if( nChar == VK_DELETE )
	{
		check.IP_cBRN = -1;
		var.pbVal = NULL;

		keybuf->IP_cBRN = check.IP_cBRN;
		SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;

		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
		// 資金繰諸口枝番等セット
		set_etcdsp( pn );
	}

	return( nChar );
}

//
// 枝番摘要選択toデータセット＆表示
BOOL CScanKey::brnsel_set( int pn, int num )
{
	struct _ItemData *pKey;
	char str[128];
	VARIANT var;

	if( num > 8 )
	{

		SelResult srt;
		sprintf_s( str, sizeof str, "%d", num );

		srt.sel_pn = SL_BRNTKY_PN;
		if( m_Seldata.GetSelResult( &srt, str ) ) {
#ifdef SEL_KEEP
		if( brntky_get( &code, &brn, &TKYBRN_SEL, num - 8 ) ) {
#endif
			pKey = _getDATA( pn );

			pKey->IP_cBRN = srt.brn_no;
			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;

			//データセット
			set_codestr( str, sizeof str, pKey->IP_cBRN, Voln1->edcol );
			var.pbVal = (BYTE*)str;
			DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			return TRUE;
		}
	}
	return FALSE;
}

//
// 枝番入力後カーソルセット
BOOL CScanKey::brinp_to_next( int pn, short nChar )
{
	// カーソルセット
	switch( pn )
	{
	case SCN_DBR1_PN:
		switch( nChar )
		{
		case VK_RETURN:
		case VK_TAB:
		case VK_RIGHT:
//		case VK_F3:
		case VK_DELETE:
			set_focus( SCN_DBR2_PN );
			break;
		case VK_LEFT:
		case VK_F2:
			set_focus( SCN_DEBT_PN );
			break;
		case VK_UP:
			if( !scGetINP_CTL( SCN_BMON1_PN )->INP_ena )	//#* 02.26 /03
				set_focus( SCN_DENP1_PN );
			else
				set_focus( SCN_BMON1_PN );
			break;
		case VK_DOWN:
			if( !scGetINP_CTL( SCN_CBR1_PN )->INP_ena )	//#* 02.26 /03
				set_focus( SCN_CRED_PN );
			else
				set_focus( SCN_CBR1_PN );
			break;
		default:
			Buzzer();
			break;
		}
		break;

	case SCN_DBR2_PN:
		switch( nChar )
		{
		case VK_RETURN:
		case VK_TAB:
		case VK_RIGHT:
//		case VK_F3:
		case VK_DELETE:
			set_focus( SCN_CHKDBR_PN );		//#* 02.26 /03
			break;
		case VK_LEFT:
		case VK_F2:
			set_focus( SCN_DBR1_PN );
			break;

		case VK_UP:
			if( !scGetINP_CTL( SCN_BMON2_PN )->INP_ena )	//#* 02.26 /03
				set_focus( SCN_DENP2_PN );
			else
				set_focus( SCN_BMON2_PN );
			break;
		case VK_DOWN:
			if( !scGetINP_CTL( SCN_CBR2_PN )->INP_ena )	//#* 02.26 /03
				set_focus( SCN_CRED_PN );
			else
				set_focus( SCN_CBR2_PN );
			break;

		default:
			Buzzer();
			break;
		}
		break;

	case SCN_CBR1_PN:
		switch( nChar )
		{
		case VK_RETURN:
		case VK_TAB:
		case VK_RIGHT:
//		case VK_F3:
		case VK_DELETE:
			set_focus( SCN_CBR2_PN );
			break;
		case VK_LEFT:
		case VK_F2:
			set_focus( SCN_CRED_PN );
			break;
		case VK_UP:
			if( !scGetINP_CTL( SCN_DBR1_PN )->INP_ena )	//#* 02.26 /03
				set_focus( SCN_DEBT_PN );
			else
				set_focus( SCN_DBR1_PN );
			break;
		case VK_DOWN:
			if( !scGetINP_CTL( SCN_DCBR1_PN )->INP_ena )	//#* 02.26 /03
				set_focus( SCN_DBCR_PN );
			else
				set_focus( SCN_DCBR1_PN );
			break;
		default:
			Buzzer();
			break;
		}
		break;

	case SCN_CBR2_PN:
		switch( nChar )
		{
		case VK_RETURN:
		case VK_TAB:
		case VK_RIGHT:
//		case VK_F3:
		case VK_DELETE:
			set_focus( SCN_CHKCBR_PN );		//#* 02.26 /03
			break;
		case VK_LEFT:
		case VK_F2:
			set_focus( SCN_CBR1_PN );
			break;
		case VK_UP:
			if( !scGetINP_CTL( SCN_DBR2_PN )->INP_ena )	//#* 02.26 /03
				set_focus( SCN_DEBT_PN );
			else
				set_focus( SCN_DBR2_PN );
			break;
		case VK_DOWN:
			if( !scGetINP_CTL( SCN_DCBR2_PN )->INP_ena )	//#* 02.26 /03
				set_focus( SCN_DBCR_PN );
			else
				set_focus( SCN_DCBR2_PN );
			break;

		default:
			Buzzer();
			break;
		}
		break;

	case SCN_DCBR1_PN:
		switch( nChar )
		{
		case VK_RETURN:
		case VK_TAB:
		case VK_RIGHT:
//		case VK_F3:
		case VK_DELETE:
			set_focus( SCN_DCBR2_PN );
			break;
		case VK_LEFT:
		case VK_F2:
			set_focus( SCN_DBCR_PN );
			break;
		case VK_UP:
			if( !scGetINP_CTL(SCN_CBR1_PN)->INP_ena )
				set_focus( SCN_CRED_PN );
			else
				set_focus( SCN_CBR1_PN );
			break;
		case VK_DOWN:
			set_focus( SCN_VAL2_PN );
			break;
		default:
			Buzzer();
			break;
		}
		break;

	case SCN_DCBR2_PN:
		switch( nChar )
		{
		case VK_RETURN:
		case VK_TAB:
		case VK_RIGHT:
//		case VK_F3:
		case VK_DELETE:
			set_focus( SCN_VAL1_PN );
			break;
		case VK_LEFT:
		case VK_F2:
			set_focus( SCN_DCBR1_PN );
			break;
		case VK_UP:
			if( !scGetINP_CTL(SCN_CBR2_PN)->INP_ena )
				set_focus( SCN_CRED_PN );
			else
				set_focus( SCN_CBR2_PN );
			break;
		case VK_DOWN:
			set_focus( SCN_VAL2_PN );
			break;
		default:
			Buzzer();
			break;
		}
		break;

	default:
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////
///	金額
// 開始
void CScanKey::OnSetFocusScnval1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.12 /00
		MyRecognizeON( SPCID_KINGAKU );
#endif
		set_nowpn( SCN_VAL1_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_VAL1_PN );
}

void CScanKey::OnKillFocusScnval1( long inplen ) 
{
	 ValInp( SCN_VAL1_PN, &SCAN_KEY.SCN_VAL1_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnval1(long nChar, long inplen, long kst ) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = ValInp( SCN_VAL1_PN, &SCAN_KEY.SCN_VAL1_DT, nChar, inplen, kst )) )
			return;

		//カーソル移動
		if( term_focus( SCN_VAL1_PN, nChar ) != 0 )
			Buzzer();
		
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

// 終了
void CScanKey::OnSetFocusScnval2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.12 /00
		MyRecognizeON( SPCID_KINGAKU );
#endif
		set_nowpn( SCN_VAL2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_VAL2_PN );
}

void CScanKey::OnKillFocusScnval2( long inplen ) 
{
	 ValInp( SCN_VAL2_PN, &SCAN_KEY.SCN_VAL2_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnval2(long nChar, long inplen, long kst ) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = ValInp( SCN_VAL2_PN, &SCAN_KEY.SCN_VAL2_DT, nChar, inplen, kst )) )
			return;

		//カーソル移動
		if( term_focus( SCN_VAL2_PN, nChar ) != 0 )
			Buzzer();


	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// 税額
//開始
void CScanKey::OnSetFocusScnzei1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.12 /00
		MyRecognizeON( SPCID_KINGAKU );
#endif
		set_nowpn( SCN_ZEI1_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_ZEI1_PN );
}

void CScanKey::OnKillFocusScnzei1( long inplen )
{
	if( m_DIALOG_OK == FALSE )
		return;

	ValInp( SCN_ZEI1_PN, &SCAN_KEY.SCN_ZEI1_DT, 0, inplen, 0);
}


void CScanKey::OnTerminationScnzei1(long nChar, long inplen, long kst ) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = ValInp( SCN_ZEI1_PN, &SCAN_KEY.SCN_ZEI1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_ZEI1_PN, nChar ) != 0 )
			Buzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

//終了
void CScanKey::OnSetFocusScnzei2() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.12 /00
		MyRecognizeON( SPCID_KINGAKU );
#endif
		set_nowpn( SCN_ZEI2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_ZEI2_PN );
}

void CScanKey::OnKillFocusScnzei2( long inplen )
{
	if( m_DIALOG_OK == FALSE )
		return;

	ValInp( SCN_ZEI2_PN, &SCAN_KEY.SCN_ZEI2_DT, 0, inplen, 0);
}

void CScanKey::OnTerminationScnzei2(long nChar, long inplen, long kst) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = ValInp( SCN_ZEI2_PN, &SCAN_KEY.SCN_ZEI2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_ZEI2_PN, nChar ) != 0 )
			Buzzer();
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

//
// 金額　入力＆オフフォーカス
long CScanKey::ValInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst )
{
	struct _ItemData check = {0};
	VARIANT var;

	if( inplen > 0 ) {
		l_defn( 0x16 );
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
		memcpy( check.IP_VAL, var.pbVal, sizeof check.IP_VAL );
		memmove( keybuf, &check, sizeof(check) );

		SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
	}
	
	// オフフォーカス
	if( !nChar )
	{

		//データリード
//		if( inplen > 0 )
//		{
//			DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
//			memcpy( check.IP_VAL, var.pbVal, sizeof check.IP_VAL );
//			memmove( keybuf, &check, sizeof(check) );
//			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
//		}
//		else
//			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;

		//データセット
		if( SCAN_KEY.LINE_CND[ pn ].INP_sg == TRUE )
		{
			var.pbVal = (BYTE*)keybuf->IP_VAL;
			DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
		}
		else
		{
			var.pbVal = NULL;
			DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
		}
		return(0);
	}
	else
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return VK_F2;
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{
//		if( ((CEdit*)GetDlgItem( scGetINP_CTL( pn )->IDC_X ))->LineLength() )
//		{
//			l_defn( 0x16 );
//			data_get( scGetINP_CTL( pn )->IDC_X, &check, CICSInput );
//			memmove( keybuf->IP_VAL, check.IP_VAL, sizeof(keybuf->IP_VAL) );
//			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
//		}
	}
	else if( nChar == VK_DELETE )
	{
		var.pbVal = NULL;
		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
		memset( (char *)keybuf->IP_VAL, (char)0, sizeof(keybuf->IP_VAL) );
		SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}

/////////////////////////////////////////////////////////////////////////////////////
// 摘要

void CScanKey::OnSetFocusScntky() 
{
	BOOL debt_inp_sg;
	BOOL cred_inp_sg;
	BOOL dbcr_inp_sg;
	CString debt, cred;

	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.12 /00
		MyRecognizeON( SPCID_TEKIYO );
#endif
		if( get_nowpn() != SCN_TKY_PN )
		{
#ifndef CLOSE	// 02.07 /01
			set_nowpn( SCN_TKY_PN );
#endif
			// 連想摘要検索＆表示
			debt_inp_sg = SCAN_KEY.LINE_CND[SCN_DEBT_PN].INP_sg;
			cred_inp_sg = SCAN_KEY.LINE_CND[SCN_CRED_PN].INP_sg;
			dbcr_inp_sg = SCAN_KEY.LINE_CND[SCN_DBCR_PN].INP_sg;
			if( debt_inp_sg || cred_inp_sg )
			{
				if( debt_inp_sg )
					debt = SCAN_KEY.SCN_DEBT_DT.IP_CdNum;
				if( cred_inp_sg )
					cred = SCAN_KEY.SCN_CRED_DT.IP_CdNum;
			}
			else if( dbcr_inp_sg )
			{
				debt = SCAN_KEY.SCN_DBCR_DT.IP_CdNum;
				cred = "";
			}

			SelPar par;
			par.sel_pn = SL_TKY_PN;
			par.dbt = debt;
			par.cre = cred;
			::ZeroMemory( par.tky_kana, sizeof par.tky_kana );

			m_Seldata.SelectJob( &par, -1 );

#ifdef SEL_KEEP
			if( ((CDBDinpView*)m_pParent)->rentky_srdsp(debt,cred) == -1 )
			{
				// 連想摘要が無かっ場合には５０音摘要を表示させる
				GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X )->PostMessage( WM_KEYDOWN, VK_HOME );
				GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X )->PostMessage( WM_KEYUP, VK_HOME );
			}
#endif
		}
	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}
	OnICSInputFocus( SCN_TKY_PN );
}


void CScanKey::OnKillFocusScntky( long inplen ) 
{
	OnTerminationScntky( 0, inplen, 0 ); 
}

void CScanKey::OnKanaTermScntky(LPCSTR data) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	struct _ItemData dmy_data = {0};
	CDBipTKREC tkrec;

//	try
	{
		//５０音摘要表示

		if( ! isdigitstr( (char*)data ) ) {

			SelPar par;
			par.sel_pn = SL_TKYKANA_PN;
			strcpy_s( par.tky_kana, sizeof par.tky_kana, data );

			m_Seldata.SelectJob( &par, -1 );
		}
		else {
			if( tky_select( &tkrec, (char *)data ) != -1 ) {
				tky_select_dsp( &tkrec, TRUE );
			}
		}


#ifdef SEL_KEEP
		if( ((CDBDinpView*)m_pParent)->tky_select50_dsp( (char*)data ) )
		{
		//	data_reply( scGetINP_CTL( SCN_TKY_PN )->IDC_X, 0, &dmy_data, CICSInput );
			return;
		}

		//摘要選択
		if( ((CDBDinpView*)m_pParent)->tky_select( &tkrec, (char *)data ) != -1 )
		{
			tky_select_dsp( &tkrec, TRUE );
		}
		else
		{
			Buzzer();
		//	data_reply( scGetINP_CTL( SCN_TKY_PN )->IDC_X, -1, &dmy_data, CICSInput );
		}
#endif
	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}
}

/*==========================
	摘要セットデータ表示
===========================*/
void CScanKey::tky_select_dsp( CDBipTKREC *ptk, BOOL F12_KEY )
{
	struct _ItemData check={0};
	struct _ItemData ins_data={0};
	char fmt[10];

	//仕訳摘要表示
//	sprintf( fmt, "%%.%ds", sizeof( ptk->TKnam ) );
	strcpy_s( fmt, sizeof fmt, "%s" );
	sprintf_s( (char *)ins_data.IP_TKY, sizeof ins_data.IP_TKY, fmt, ptk->m_tkname );
	VARIANT var;

	if( F12_KEY ) {
/*DB_OLD*///	data_reply( scGetINP_CTL( SCN_TKY_PN )->IDC_X, 1, &ins_data, CICSInput );
		var.pbVal = ins_data.IP_TKY;
		CWnd* pwnd = GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X );
		short pos = ((CICSDBEDT*)pwnd)->GetCaretPosition();
		((CICSDBEDT*)pwnd)->InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );
	}
	else
	{
#ifndef CLOSE	// 04.08 /99
		//データセットのためのダミーフォーカスＯＦＦ
		OnTerminationScntky( 0, 0, 0 );
#endif
/*DB_OLD*///	((CICSDBEDT*)GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X ))->TekiInsert( (char*)ins_data.IP_TKY );

		var.pbVal = ins_data.IP_TKY;
		CWnd* pwnd = GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X );
		short pos = ((CICSDBEDT*)pwnd)->GetCaretPosition();
		((CICSDBEDT*)pwnd)->InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );	
	}
}

void CScanKey::OnTerminationScntky(long nChar, long inplen, long kst) 
{
	char pbuf[128];
	VARIANT var;
	struct _ItemData check={0};
	int i;

	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// オフフォーカス
		if( !nChar )
		{
			//データリード
#ifndef CLOSE	// 04.08 /99
		//	pos = ((CICSDBEDT*)GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X))->GetCaretPosition();

			DBdata_get( this, scGetINP_CTL( SCN_TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		//	DBdata_set( this, scGetINP_CTL( SCN_TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		//	GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X  )->GetWindowText( (char*)check.IP_TKY, (Voln1->TK_LN * 2 + 1) );
		//	data_set( scGetINP_CTL( SCN_TKY_PN )->IDC_X, &check, CICSInput );

		//	((CICSDBEDT*)GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X))->SetCaretPosition( pos );
#else
			data_get( scGetINP_CTL( SCN_TKY_PN )->IDC_X, &check, CICSInput );
#endif
			//データセット
			i = sprintf_s( pbuf, sizeof pbuf, "%.80s", var.pbVal /*check.IP_TKY*/ );
		//	i = get_inpl( (unsigned char*)pbuf, i/2 );
		//	i = get_inpl( (unsigned char*)pbuf, Voln1->tk_ln );	// 02.09 /02 -ANKINP-
			i = kjlen( pbuf, Voln1->tk_ln * 2 );
			if( i )
			{
				memmove( SCAN_KEY.SCN_TKY_DT.IP_TKY, pbuf, i );
				SCAN_KEY.SCN_TKY_DT.IP_TKY[i] = '\0';
				SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_sg = TRUE;
			}
			else
			{
				var.pbVal = NULL;
				DBdata_set( this, scGetINP_CTL( SCN_TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				// ｲﾒｰｼﾞ摘要時は 文字列ｸﾘｱ (入力ｻｲﾝは使用中のため)
				if( is_mdfy_imgtky() ) 
					SCAN_KEY.SCN_TKY_DT.IP_TKY[0] = '\0';
				else
					SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_sg = FALSE;
			}
			return;
		}
		else
		{
			if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			{
				nChar = VK_F2;
			}
		}

		// ページアップ／ダウン
		if( nChar )
		{
			//#* 06.19 /01 ｲﾒｰｼﾞ摘要時も ICSSelect の表示のため この関数を通す ( 連想摘要がないときに 50音 表示に VK_HOME が Postされるため ) 
			//適要選択欄ページめくり
#ifdef SEL_KEEP
			nChar = ((CDBDinpView*)m_pParent)->SelTkyDsp( nChar );
#endif
			SelPar par;
			par.sel_pn = SL_TKY_PN;

			m_Seldata.SelectJob( &par, nChar );

			if( !nChar )
				return;
		}
		//
		if( nChar == VK_DELETE )
		{
			var.pbVal = NULL;
			DBdata_set( this, scGetINP_CTL( SCN_TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		//	data_set( scGetINP_CTL( SCN_TKY_PN )->IDC_X, &check, CICSInput );
			SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_sg = FALSE;
		}

		//カーソル移動
		if( term_focus( SCN_TKY_PN, nChar ) != 0 )
			Buzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
// 証憑番号
void CScanKey::OnSetFocusScnSnum1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.15 /00
		MyRecognizeON( SPCID_SNUM );	// MyRecognizeON( SPCID_BANGO );
#endif
		set_nowpn( SCN_SNUM1_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_SNUM1_PN ); 	
}

void CScanKey::OnKillFocusScnSnum1( long inplen ) 
{
	SnumInp( SCN_SNUM1_PN, &SCAN_KEY.SCN_SNUM1_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnSnum1(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = SnumInp( SCN_SNUM1_PN, &SCAN_KEY.SCN_SNUM1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_SNUM1_PN, nChar ) != 0 )
			Buzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

void CScanKey::OnSetFocusScnSnum2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.15 /00
		MyRecognizeON( SPCID_SNUM );	// MyRecognizeON( SPCID_BANGO );
#endif
		set_nowpn( SCN_SNUM2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_SNUM2_PN );	
}

void CScanKey::OnKillFocusScnSnum2( long inplen ) 
{
	SnumInp( SCN_SNUM2_PN, &SCAN_KEY.SCN_SNUM2_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnSnum2(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = SnumInp( SCN_SNUM2_PN, &SCAN_KEY.SCN_SNUM2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_SNUM2_PN, nChar ) != 0 )
			Buzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}	
}

// 証憑番号 入力＆オフフォーカス
long CScanKey::SnumInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	char	buf[30] = {0};
	VARIANT var;

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

		if( var.pbVal[0] != 0 )	strcpy_s( (char*)check.IP_SNUM, sizeof check.IP_SNUM, (char*)var.pbVal );
		else					::ZeroMemory( check.IP_SNUM, sizeof check.IP_SNUM );

		if( check.IP_SNUM[0] != 0 )
		{
			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
			strcpy_s( (char*)keybuf->IP_SNUM, sizeof keybuf->IP_SNUM, (char*)check.IP_SNUM );
		}
		else
			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;

		//データセット
	//	data_set( scGetINP_CTL( pn )->IDC_X, keybuf, CICSInput );
		return( 0 );
	}
	else
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return VK_F2;
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{
//		data_get( scGetINP_CTL( pn )->IDC_X, &check, CICSInput );
//		if( check.IP_DENP != 0xffff )
//		{
//			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
//
//			keybuf->IP_DENP = check.IP_DENP;
//		}
	}
	else if( nChar == VK_DELETE )
	{
		check.IP_TGDAY = -1;
		var.pbVal = NULL;
		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	
		::ZeroMemory( keybuf->IP_SNUM, sizeof keybuf->IP_SNUM );
		SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}





///////////////////////////////////////////////////////////////////////////////
// 手形期日
void CScanKey::OnSetFocusScnTgdate1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.15 /00
		MyRecognizeON( SPCID_TGDATE );	// MyRecognizeON( SPCID_BANGO );
#endif
		set_nowpn( SCN_TDAT1_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_TDAT1_PN ); 	
}

void CScanKey::OnKillFocusScnTgdate1( long inplen ) 
{
	TgdateInp( SCN_TDAT1_PN, &SCAN_KEY.SCN_TDAT1_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnTgdate1(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		if( inplen > 0 ) CheckDataIcsinputTgDate( SCN_TDAT1_PN, nChar );
		// 入力＆オフフォーカス
		if( !(nChar = TgdateInp( SCN_TDAT1_PN, &SCAN_KEY.SCN_TDAT1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_TDAT1_PN, nChar ) != 0 )
			Buzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

void CScanKey::OnSetFocusScnTgdate2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
#ifdef SPEECH	// 05.15 /00
		MyRecognizeON( SPCID_DENPYO );	// MyRecognizeON( SPCID_BANGO );
#endif
		set_nowpn( SCN_TDAT2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_TDAT2_PN );	
}

void CScanKey::OnKillFocusScnTgdate2( long inplen ) 
{
	TgdateInp( SCN_TDAT2_PN, &SCAN_KEY.SCN_TDAT2_DT, 0, inplen, 0 );
}

void CScanKey::OnTerminationScnTgdate2(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		if( inplen > 0 ) CheckDataIcsinputTgDate( SCN_TDAT2_PN, nChar );

		// 入力＆オフフォーカス
		if( !(nChar = TgdateInp( SCN_TDAT2_PN, &SCAN_KEY.SCN_TDAT2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_TDAT2_PN, nChar ) != 0 )
			Buzzer();
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}	
}


/**************************
	手形期日チェックデータ
***************************/
long CScanKey::CheckDataIcsinputTgDate( int pn, long nChar ) 
{
	char check[3] = {0};
	char ymd[3] = {0};
	char data[20] = {0};
	char result[30];
	char buf[40];
	VARIANT var;
	CString str;
	int ret;

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	//
//	try
	{
		DBdata_get( this, scGetINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		strcpy_s( data, sizeof data, (char*)var.pbVal );

		if( (ret = TgDateInputCheck( data, result, sizeof result, m_pZm->zvol->ss_ymd, m_pZm->zvol->ee_ymd, -1 )) <= 0 ) {
			// 不正データが入力された
			if( SCAN_KEY._XXDTA2[pn].IP_TGDAY > 0 ) {
				sprintf_s( result, sizeof result, "%d", SCAN_KEY._XXDTA2[pn].IP_TGDAY );
				var.pbVal = (BYTE*)result;
			}
			else	var.pbVal = NULL;

			DBdata_set( this, scGetINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			IntsrekitoGenstr( buf, sizeof buf, SCAN_KEY._XXDTA2[pn].IP_TGDAY );
			DBSetDispString( this, scGetINP_CTL(pn)->IDC_X, buf );
			return -1;
		}
		else {
//			memcpy( check, bcd, 3 );
		}

		var.pbVal = (BYTE*)result;
		DBdata_set( this, scGetINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
//		DBSetDispString( this, scGetINP_CTL(this, DATE_PN)->IDC_X, str );
//		DBdata_reply( this, scGetINP_CTL(this, DATE_PN)->IDC_X, 1, &ipdata );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return -1;
	}

	return nChar;
}

// 手形期日　入力＆オフフォーカス
long CScanKey::TgdateInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	char	buf[30] = {0};
	VARIANT var;

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		if( var.pbVal[0] != 0 )	check.IP_TGDAY = atoi( (LPCTSTR)var.pbVal );
		else					check.IP_TGDAY = -1;

		if( check.IP_TGDAY != -1 )
		{
			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
			keybuf->IP_TGDAY = check.IP_TGDAY;
		}
		else
			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;

		//データセット
		IntsrekitoGenstr( buf, sizeof buf, SCAN_KEY._XXDTA2[pn].IP_TGDAY );
		DBSetDispString( this, scGetINP_CTL( pn )->IDC_X, buf );
	//	data_set( scGetINP_CTL( pn )->IDC_X, keybuf, CICSInput );
		return( 0 );
	}
	else
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return VK_F2;
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{
//		data_get( scGetINP_CTL( pn )->IDC_X, &check, CICSInput );
//		if( check.IP_DENP != 0xffff )
//		{
//			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
//
//			keybuf->IP_DENP = check.IP_DENP;
//		}
	}
	else if( nChar == VK_DELETE )
	{
		check.IP_TGDAY = -1;
		var.pbVal = NULL;
		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	
		keybuf->IP_TGDAY = check.IP_TGDAY;
		SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}



//
// キー項目入力ありチェック
BOOL CScanKey::isScanKeyInp()
{
	struct _SKEY_LINE *key = &SCAN_KEY;

	// ＳＥＱ番号
	if( key->LINE_CND[ SCN_SEQ1_PN ].INP_sg || key->LINE_CND[ SCN_SEQ2_PN ].INP_sg ||
	// 日付
		key->LINE_CND[ SCN_DATE1_PN ].INP_sg || key->LINE_CND[ SCN_DATE2_PN ].INP_sg ||
	// 伝票番号
		key->LINE_CND[ SCN_DENP1_PN ].INP_sg || key->LINE_CND[ SCN_DENP2_PN ].INP_sg ||
	// 部門番号
		key->LINE_CND[ SCN_BMON1_PN ].INP_sg || key->LINE_CND[ SCN_BMON2_PN ].INP_sg ||
		key->LINE_CND[ SCN_CBMN1_PN ].INP_sg || key->LINE_CND[ SCN_CBMN2_PN ].INP_sg ||
	// 工事番号
		key->LINE_CND[ SCN_KOJI1_PN ].INP_sg || key->LINE_CND[ SCN_KOJI2_PN ].INP_sg ||
		key->LINE_CND[ SCN_CKJI1_PN ].INP_sg || key->LINE_CND[ SCN_CKJI2_PN ].INP_sg ||
	// 借方、貸方、貸借　科目
		key->LINE_CND[SCN_DEBT_PN].INP_sg || key->LINE_CND[SCN_CRED_PN].INP_sg || key->LINE_CND[SCN_DBCR_PN].INP_sg ||
	// 借方、貸方、貸借　枝番
		key->LINE_CND[SCN_DBR1_PN].INP_sg || key->LINE_CND[SCN_CBR1_PN].INP_sg || key->LINE_CND[SCN_DCBR1_PN].INP_sg ||
		key->LINE_CND[SCN_DBR2_PN].INP_sg || key->LINE_CND[SCN_CBR2_PN].INP_sg || key->LINE_CND[SCN_DCBR2_PN].INP_sg ||
	// 部門、枝番なし check OFF か 02.26 /03
		key->LINE_CND[SCN_BMON1_PN].KEY_inp ||
		key->LINE_CND[SCN_DBR1_PN].KEY_inp || key->LINE_CND[SCN_CBR1_PN].KEY_inp ||
	// 金額
		key->LINE_CND[ SCN_VAL1_PN ].INP_sg || key->LINE_CND[ SCN_VAL2_PN ].INP_sg ||
	// 対価
		key->LINE_CND[ SCN_ZEI1_PN ].INP_sg || key->LINE_CND[ SCN_ZEI2_PN ].INP_sg ||
	// 消費税
		key->LINE_CND[ SCN_SYZSW_PN ].INP_sg ||  key->LINE_CND[ SCN_ZRITU_PN ].INP_sg ||
		key->LINE_CND[ SCN_SKBN_PN ].INP_sg || key->LINE_CND[ SCN_URISIRE_PN ].INP_sg ||		
	// 摘要
		key->LINE_CND[ SCN_TKY_PN ].INP_sg || 
	// 証憑番号
		key->LINE_CND[ SCN_SNUM1_PN ].INP_sg || key->LINE_CND[ SCN_SNUM2_PN ].INP_sg ||
	// 手形期日	
		key->LINE_CND[ SCN_TDAT1_PN ].INP_sg || key->LINE_CND[ SCN_TDAT2_PN ].INP_sg ||
	// 付箋
		key->LINE_CND[ SCN_HUSEN_PN ].INP_sg
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



void CScanKey::OnSelchangeTkycombo() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Tkycb.GetCurSel();
	DWORD sign = m_Tkycb.GetItemData( sel );
	int i;
	char pbuf[128];

	SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_type = (char)sign;
	if( sign == STK_NORMAL ) {
		// 文字摘要があればサインのみセット
		i = sprintf_s( pbuf, sizeof pbuf, "%.80s", SCAN_KEY.SCN_TKY_DT.IP_TKY );
	//	i = get_inpl( (unsigned char*)pbuf, i/2 );
	//	i = get_inpl( (unsigned char*)pbuf, Voln1->tk_ln ); // 02.09 /02 -ANKINP-
		i = kjlen( pbuf, Voln1->tk_ln*2 );
		if( i )
		{
			SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_sg = TRUE;
		}
		else
		{
			SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_sg = FALSE;
		}
	}
	else {
		SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_sg = TRUE;
	}
	// エディット表示設定
	disp_imgtkyedit( SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_type );

}


void CScanKey::OnSelchangeScnsyzsw() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Syzsw.GetCurSel();
	DWORD itd = m_Syzsw.GetItemData( sel );

	SCAN_KEY.SCN_SYZSW_DT.SKBN_SGN1 = (short)itd;
	if( itd == NON_SELECT ) {
		strcpy_s( SCAN_KEY.SCN_SYZSW_DT.SKBN_TXT, sizeof SCAN_KEY.SCN_SYZSW_DT.SKBN_TXT, "" );
		SCAN_KEY.LINE_CND[ SCN_SYZSW_PN ].INP_sg = FALSE;
	}
	else {
		m_Syzsw.GetLBText( sel, SCAN_KEY.SCN_SYZSW_DT.SKBN_TXT );
		SCAN_KEY.LINE_CND[ SCN_SYZSW_PN ].INP_sg = TRUE;
	}
}


void CScanKey::OnSelchangeScnskbn() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Skbn.GetCurSel();
	DWORD itd = m_Skbn.GetItemData( sel );
	SCAN_KEY.SCN_SKBN_DT.SKBN_SGN1 = (short)itd;

	if( itd == NON_SELECT ) {
		strcpy_s( SCAN_KEY.SCN_SKBN_DT.SKBN_TXT, sizeof SCAN_KEY.SCN_SKBN_DT.SKBN_TXT, "" );
		SCAN_KEY.LINE_CND[ SCN_SKBN_PN ].INP_sg = FALSE;
	}
	else {
		m_Syzsw.GetLBText( sel, SCAN_KEY.SCN_SKBN_DT.SKBN_TXT );
		SCAN_KEY.LINE_CND[ SCN_SKBN_PN ].INP_sg = TRUE;
	}
}


void CScanKey::OnSelchangeScnzritu() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Zritu.GetCurSel();
	DWORD itd = m_Zritu.GetItemData( sel );
	SCAN_KEY.SCN_ZRITU_DT.SKBN_SGN1 = (short)itd;

	if( itd == NON_SELECT ) {
		strcpy_s( SCAN_KEY.SCN_ZRITU_DT.SKBN_TXT, sizeof SCAN_KEY.SCN_ZRITU_DT.SKBN_TXT, "" );
		SCAN_KEY.LINE_CND[ SCN_ZRITU_PN ].INP_sg = FALSE;
	}
	else {
		m_Syzsw.GetLBText( sel, SCAN_KEY.SCN_ZRITU_DT.SKBN_TXT );
		SCAN_KEY.LINE_CND[ SCN_ZRITU_PN ].INP_sg = TRUE;
	}
}


void CScanKey::OnSelchangeScnurisire() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = 	m_Urisire.GetCurSel();
	DWORD itd = m_Urisire.GetItemData( sel );
	SCAN_KEY.SCN_URISIRE_DT.SKBN_SGN1 = sel;
	m_Urisire.GetLBText( sel, SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT );

	if( itd == NON_SELECT ) {
		strcpy_s( SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT, sizeof SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT, "" );
		SCAN_KEY.LINE_CND[ SCN_URISIRE_PN ].INP_sg = FALSE;
	}
	else {
		m_Syzsw.GetLBText( sel, SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT );
		SCAN_KEY.LINE_CND[ SCN_URISIRE_PN ].INP_sg = TRUE;
	}
}


int CScanKey::set_nonICSpn( tagMSG* pMsg )
{
	CWnd *pwnd;

	if( pwnd = GetFocus() )
	{
		if( get_nowpn() != SCN_IDOK && pwnd->m_hWnd == GetDlgItem( IDOK )->m_hWnd )
		{
			return set_nowpn( SCN_IDOK );
		}
		else if( get_nowpn() != SCN_IDCANCEL && pwnd->m_hWnd == GetDlgItem( IDCANCEL )->m_hWnd )
		{
			return set_nowpn( SCN_IDCANCEL );
		}
		//#* 05.16 /01 摘要のみ修正
		else if( get_nowpn() != SCN_TKYONLY && pwnd->m_hWnd == GetDlgItem( IDC_TKYONLY )->m_hWnd )
		{
			return set_nowpn( SCN_TKYONLY );
		}
		//#* 06.19 /01 ｲﾒｰｼﾞ摘要選択ﾎﾞﾀﾝ
		else if( get_nowpn() != SCN_SELTKY_PN && pwnd->m_hWnd == GetDlgItem( IDC_TKYCOMBO )->m_hWnd )
		{
			return set_nowpn( SCN_SELTKY_PN );
		}
		// 部門・枝番チェックボタン
		else if( get_nowpn() != SCN_CHKDBM_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHECK_DBM )->m_hWnd )
		{
			return set_nowpn( SCN_CHKDBM_PN );
		}
		else if( get_nowpn() != SCN_CHKCBM_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHECK_CBM )->m_hWnd )
		{
			return set_nowpn( SCN_CHKCBM_PN );
		}
		else if( get_nowpn() != SCN_CHKDKJ_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHECK_DKJ )->m_hWnd )
		{
			return set_nowpn( SCN_CHKDKJ_PN );
		}
		else if( get_nowpn() != SCN_CHKCKJ_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHECK_CKJ )->m_hWnd )
		{
			return set_nowpn( SCN_CHKCKJ_PN );
		}
		else if( get_nowpn() != SCN_CHKDBR_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHECK_DBR )->m_hWnd )
		{
			return set_nowpn( SCN_CHKDBR_PN );
		}
		else if( get_nowpn() != SCN_CHKCBR_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHECK_CBR )->m_hWnd )
		{
			return set_nowpn( SCN_CHKCBR_PN );
		}
		// 消費税関係
		else if( get_nowpn() != SCN_SYZSW_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNSYZSW )->m_hWnd )
		{
			return set_nowpn( SCN_SYZSW_PN );
		}
		else if( get_nowpn() != SCN_ZRITU_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNZRITU )->m_hWnd )
		{
			return set_nowpn( SCN_ZRITU_PN );
		}
		else if( get_nowpn() != SCN_SKBN_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNSKBN )->m_hWnd )
		{
			return set_nowpn( SCN_SKBN_PN );
		}
		else if( get_nowpn() != SCN_URISIRE_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNURISIRE )->m_hWnd )
		{
			return set_nowpn( SCN_URISIRE_PN );
		}
		// データ関係コンボボックス
		else if( get_nowpn() != SCN_ORDER_PN && pwnd->m_hWnd == GetDlgItem( IDC_DISPORDER )->m_hWnd )
		{
			return set_nowpn( SCN_ORDER_PN );
		}
		else if( get_nowpn() != SCN_DUSER_PN && pwnd->m_hWnd == GetDlgItem( IDC_DATA_USER )->m_hWnd )
		{
			return set_nowpn( SCN_DUSER_PN );
		}
		else if( get_nowpn() != SCN_STYPE_PN && pwnd->m_hWnd == GetDlgItem( IDC_COMBO_STYPE )->m_hWnd )
		{
			return set_nowpn( SCN_STYPE_PN );
		}
		// 付箋
		else if( get_nowpn() != SCN_HUSEN_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNHUSEN )->m_hWnd )
		{
			return set_nowpn( SCN_HUSEN_PN );
		}
		// NOT チェックボックス
		else if( get_nowpn() != SNOT_SEQ_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_SEQ )->m_hWnd ) {
			return set_nowpn( SNOT_SEQ_PN );
		}
		else if( get_nowpn() != SNOT_DATE_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_DATE )->m_hWnd ) {
			return set_nowpn( SNOT_DATE_PN );
		}
		else if( get_nowpn() != SNOT_DENP_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_DENP )->m_hWnd ) {
			return set_nowpn( SNOT_DENP_PN );
		}
		else if( get_nowpn() != SNOT_DBMN_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_DBMN )->m_hWnd ) {
			return set_nowpn( SNOT_DBMN_PN );
		}
		else if( get_nowpn() != SNOT_CBMN_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_CBMN )->m_hWnd ) {
			return set_nowpn( SNOT_CBMN_PN );
		}
		else if( get_nowpn() != SNOT_DKJI_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_DKJI )->m_hWnd ) {
			return set_nowpn( SNOT_DKJI_PN );
		}
		else if( get_nowpn() != SNOT_CKJI_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_CKJI )->m_hWnd ) {
			return set_nowpn( SNOT_CKJI_PN );
		}
		else if( get_nowpn() != SNOT_DEB_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_DBT )->m_hWnd ) {
			return set_nowpn( SNOT_DEB_PN );
		}
		else if( get_nowpn() != SNOT_DBRN_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_DBRN )->m_hWnd ) {
			return set_nowpn( SNOT_DBRN_PN );
		}
		else if( get_nowpn() != SNOT_CRE_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_CRE )->m_hWnd ) {
			return set_nowpn( SNOT_CRE_PN );
		}
		else if( get_nowpn() != SNOT_CBRN_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_CBRN )->m_hWnd ) {
			return set_nowpn( SNOT_CBRN_PN );
		}
		else if( get_nowpn() != SNOT_DBCR_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_DBCR )->m_hWnd ) {
			return set_nowpn( SNOT_DBCR_PN );
		}
		else if( get_nowpn() != SNOT_DCBRN_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_DCBRN )->m_hWnd ) {
			return set_nowpn( SNOT_DCBRN_PN );
		}
		else if( get_nowpn() != SNOT_VAL_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_VAL )->m_hWnd ) {
			return set_nowpn( SNOT_VAL_PN );
		}
		else if( get_nowpn() != SNOT_ZEI_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_ZEI )->m_hWnd ) {
			return set_nowpn( SNOT_ZEI_PN );
		}
		else if( get_nowpn() != SNOT_SYZSW_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_SYZSW )->m_hWnd ) {
			return set_nowpn( SNOT_SYZSW_PN );
		}
		else if( get_nowpn() != SNOT_ZRITU_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_ZRITU )->m_hWnd ) {
			return set_nowpn( SNOT_ZRITU_PN );
		}
		else if( get_nowpn() != SNOT_SKBN_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_SKBN )->m_hWnd ) {
			return set_nowpn( SNOT_SKBN_PN );
		}
		else if( get_nowpn() != SNOT_URISIRE_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_URISIRE )->m_hWnd ) {
			return set_nowpn( SNOT_URISIRE_PN );
		}
		else if( get_nowpn() != SNOT_SNUM_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_SNUM )->m_hWnd ) {
			return set_nowpn( SNOT_SNUM_PN );
		}
		else if( get_nowpn() != SNOT_TGDATE_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_TGDATE )->m_hWnd ) {
			return set_nowpn( SNOT_TGDATE_PN );
		}
		else if( get_nowpn() != SNOT_HUSEN_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_HUSEN )->m_hWnd ) {
			return set_nowpn( SNOT_HUSEN_PN );
		}

	}
	return get_nowpn();
}

	

int CScanKey::LbuttonDownJob( struct tagMSG * pMsg )
{
	int pn;
	
	// F11 のガイドがクリックされた
	if( GetDlgItem( IDC_ICSDISPCTRL2 )->m_hWnd == pMsg->hwnd )
	{
		PostMessage( WM_KEYDOWN, VK_F11 );
		PostMessage( WM_KEYUP, VK_F11 );
		return 0;
	}
	// 摘要枝番コントロールがクリックされた
	else if( GetDlgItem( IDC_DISPETC )->m_hWnd == pMsg->hwnd )
	{
		switch( get_nowpn() )
		{
		case SCN_CBR1_PN: pn = SCN_DBR1_PN; break;
		case SCN_CBR2_PN: pn = SCN_DBR2_PN; break;
		case SCN_DCBR1_PN: pn = SCN_CBR1_PN; break;
		case SCN_DCBR2_PN: pn = SCN_CBR2_PN; break;
		default:
			return 1;
		}
		SendDlgItemMessage( scGetINP_CTL(pn)->IDC_X ,WM_LBUTTONDOWN );
		return 0;
	}
	return 1;
}

BOOL CScanKey::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
//	if( (m_hAccelTable != NULL) && ::TranslateAccelerator( m_hWnd, m_hAccelTable, pMsg))
//	{
//		return TRUE;
//	}

	int pn;
	set_nonICSpn( pMsg );

	if( pMsg->message == WM_LBUTTONDOWN )
	{
		if( LbuttonDownJob( pMsg ) == 0 )
			return TRUE;
	}
	else if( pMsg->message == WM_KEYDOWN )
	{
		if( get_nowpn() == SCN_TKY_PN )
		{
# ifndef CLOSE //#* 06.19 /01 ｲﾒｰｼﾞ摘要表示ｴﾃﾞｨｯﾄ
			if( GetDlgItem( IDC_IMGTKYEDIT )->m_hWnd == pMsg->hwnd ) {
				if( !OnTerminateOnImgTkyEdit( pMsg->wParam ) )
					return TRUE;
			}
# endif
		//	if( pMsg->wParam == VK_BACK )
		//	{
		//		if( GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X )->m_hWnd == pMsg->hwnd )
		//		{
		//			OnTerminationScntky(pMsg->wParam);
		//			return TRUE;
		//		}
		//	}
		}
		else {
			int nChar = -1;
			if( is_SHIFT() && pMsg->wParam == VK_TAB )
				nChar = VK_F2;
			else if( ! is_SHIFT() )
				nChar = pMsg->wParam;

			if( nChar != -1 )
			{
				if( nChar == VK_F8 ) {
					PostMessage(WM_COMMAND, MAKELONG(IDOK, BN_CLICKED), (LPARAM)GetDlgItem(IDOK)->m_hWnd);
					return TRUE;
				}
				else if( nChar == VK_INSERT ) {
					PostMessage(WM_COMMAND, MAKELONG(IDC_TKYONLY, BN_CLICKED), (LPARAM)GetDlgItem(IDC_TKYONLY)->m_hWnd);
					return TRUE;
				}
				else if( nChar == VK_END ) {
					PostMessage(WM_COMMAND, MAKELONG(IDCANCEL, BN_CLICKED), (LPARAM)GetDlgItem(IDCANCEL)->m_hWnd);
					return TRUE;
				}
				else if( nChar == VK_F11 ) {
					PostMessage(WM_COMMAND, MAKELONG(IDC_ALLKEYDEL, BN_CLICKED), (LPARAM)GetDlgItem(IDC_ALLKEYDEL)->m_hWnd);
					return TRUE;
				}

				if( GetDlgItem( IDOK )->m_hWnd == pMsg->hwnd )
				{
					if( !OnTerminateOnOK( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDCANCEL )->m_hWnd == pMsg->hwnd )
				{
					if( !OnTerminateOnCancel( nChar ) )
						return TRUE;
				}
				//#* 03.29 /01 摘要のみ
				else if( GetDlgItem( IDC_TKYONLY )->m_hWnd == pMsg->hwnd ) {
					if( !OnTerminateOnTkyOnly( nChar ) )
						return TRUE;
				}
				//#* 06.19 /01 摘要の選択コンボボックス
				else if( GetDlgItem( IDC_TKYCOMBO )->m_hWnd == pMsg->hwnd ) {
					if( !OnTerminateOnSelTky( nChar ) )
						return TRUE;
				}
				//#* 02.26 /03 部門・枝番 チェックボックス
				else if( GetDlgItem( IDC_CHECK_DBM )->m_hWnd == pMsg->hwnd ) {
					if( !OnTermCheckDBm( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDC_CHECK_CBM )->m_hWnd == pMsg->hwnd ) {
					if( !OnTermCheckCBm( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDC_CHECK_DKJ )->m_hWnd == pMsg->hwnd ) {
					if( !OnTermCheckDkoji( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDC_CHECK_CKJ )->m_hWnd == pMsg->hwnd ) {
					if( !OnTermCheckCkoji( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDC_CHECK_DBR )->m_hWnd == pMsg->hwnd ) {
					if( !OnTermCheckDBR( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDC_CHECK_CBR )->m_hWnd == pMsg->hwnd ) {
					if( !OnTermCheckCBR( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDC_SCNSYZSW )->m_hWnd == pMsg->hwnd ) {
					if( !OnTerminationScnSyzsw( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDC_SCNZRITU )->m_hWnd == pMsg->hwnd ) {
					if( !OnTerminationScnZritu( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDC_SCNSKBN )->m_hWnd == pMsg->hwnd ) {
					if( !OnTerminationScnskbn( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDC_SCNURISIRE )->m_hWnd == pMsg->hwnd ) {
					if( !OnTerminationScnurisire( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDC_DISPORDER )->m_hWnd == pMsg->hwnd ) {
					if( !OnTermComboOrder( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDC_DATA_USER )->m_hWnd == pMsg->hwnd ) {
					if( !OnTermComboDUser( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDC_COMBO_STYPE )->m_hWnd == pMsg->hwnd ) {
					if( !OnTermComboSType( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDC_SCNHUSEN )->m_hWnd == pMsg->hwnd ) {
					if( !OnTermComboHusen( nChar ) )
						return TRUE;
				}
				else if( GetDlgItem( IDC_SCNNOINPBRN )->m_hWnd == pMsg->hwnd ) {
					if( !OnTermComboNoInp( nChar ) )
						return TRUE;
				}
				else if( (pn = IsNotChkbox( pMsg->hwnd )) != -1 ) {
					if( !OnTermNotChkbox( pn, nChar ) )
						return TRUE;
				}
			}
		}
	}
	
	return ICSDialog::PreTranslateMessage(pMsg);
}


long CScanKey::OnTerminationScnSyzsw(long nChar) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return nChar;

//	try
	{
		//カーソル移動
		if( term_focus( SCN_SYZSW_PN, nChar ) != 0 ) {
			Buzzer();
			return nChar;
		}
		return 0;
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return nChar;;
	}	
}

long CScanKey::OnTerminationScnZritu( long nChar)
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return nChar;

//	try
	{
		//カーソル移動
		if( term_focus( SCN_ZRITU_PN, nChar ) != 0 ) {
			Buzzer();
			return nChar;
		}
		return 0;
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return nChar;
	}	
}


long CScanKey::OnTerminationScnskbn(long nChar) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return nChar;

//	try
	{
		//カーソル移動
		if( term_focus( SCN_SKBN_PN, nChar ) != 0 ) {
			Buzzer();
			return nChar;
		}
		return 0;
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return nChar;
	}	
}

long CScanKey::OnTerminationScnurisire(long nChar) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_DIALOG_OK == FALSE )
		return nChar;

//	try
	{
		//カーソル移動
		if( term_focus( SCN_URISIRE_PN, nChar ) != 0 ) {
			Buzzer();
			return nChar;
		}
		return 0;
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return nChar;
	}	
}



//
long CScanKey::OnTerminateOnOK( long nChar )
{
	switch( nChar )
	{
	case VK_RIGHT:
	case VK_DOWN:
//	case VK_F3:
#ifndef CLOSE	// 08.03 /98
		set_focus( SCN_IDCANCEL );
#else
		GotoDlgCtrl( GetDlgItem( IDCANCEL ) );
#endif
		return 0;
	case VK_UP:
		set_focus( SCN_TKYONLY );
		return 0;
	case VK_LEFT:
	case VK_F2:
		set_focus( SCN_NOINP_PN );
		return 0;
	}
	return nChar;
}


void CScanKey::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
#ifdef OLD_CLOSE	////////////////
	if( IN_FORK )
		return;
	
	try
	{
#ifndef CLOSE	// 05.16 /01
		// キー捨ての判定を１個所で行うため。
		PostMessage( WM_KEYDOWN, VK_F8 );
#else
		((CDBDinpView*)m_pParent)->gotokeyscan();
#endif
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
#endif		////////////////////////

	// NOT ﾁｪｯｸﾎﾞｯｸｽ
	CButton* pbtn;
	struct _InpCtl *pIctl;
	int sgn, id;

	for( int pn = SNOT_SEQ_PN; pn <= SNOT_HUSEN_PN; pn++ ) {
		pIctl = scGetINP_CTL( pn );
		id = pIctl->IDC_X;
		if( pIctl->INP_ena ) {
			pbtn = (CButton*)GetDlgItem( id );
			sgn = pbtn->GetCheck();

			NotCheckSet( pn, sgn );
		}
	}
	m_bTKYONLY = FALSE;

	ICSDialog::OnOK();
}

// 摘要のみ修正
void CScanKey::OnTkyonly() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	// NOT ﾁｪｯｸﾎﾞｯｸｽ
	CButton* pbtn;
	struct _InpCtl *pIctl;
	int sgn, id;

	for( int pn = SNOT_SEQ_PN; pn <= SNOT_HUSEN_PN; pn++ ) {
		pIctl = scGetINP_CTL( pn );
		id = pIctl->IDC_X;
		if( pIctl->INP_ena ) {
			pbtn = (CButton*)GetDlgItem( id );
			sgn = pbtn->GetCheck();

			NotCheckSet( pn, sgn );
		}
	}

	m_bTKYONLY = TRUE;

	EndDialog( IDOK );
}


void CScanKey::NotCheckSet( int pn, int sgn )
{
	struct _LINE_CND* lcnd;

	switch( pn ) {
	case SNOT_SEQ_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_SEQ1_PN ];		break;
	case SNOT_DATE_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DATE1_PN ];		break;
	case SNOT_DENP_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DENP1_PN ];		break;
	case SNOT_DBMN_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_BMON1_PN ];		break;
	case SNOT_CBMN_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_CBMN1_PN ];		break;
	case SNOT_DKJI_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_KOJI1_PN ];		break;
	case SNOT_CKJI_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_CKJI1_PN ];		break;
	case SNOT_DEB_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DEBT_PN ];		break;
	case SNOT_DBRN_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DBR1_PN ];		break;
	case SNOT_CRE_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_CRED_PN ];		break;
	case SNOT_CBRN_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_CBR1_PN ];		break;
	case SNOT_DBCR_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DBCR_PN ];		break;
	case SNOT_DCBRN_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DCBR1_PN ];		break;
	case SNOT_VAL_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_VAL1_PN ];		break;
	case SNOT_ZEI_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_ZEI1_PN ];		break;
	case SNOT_SYZSW_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_SYZSW_PN ];		break;
	case SNOT_ZRITU_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_ZRITU_PN ];		break;
	case SNOT_SKBN_PN:		lcnd = &SCAN_KEY.LINE_CND[ SCN_SKBN_PN ];		break;
	case SNOT_URISIRE_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_URISIRE_PN ];	break;
	case SNOT_SNUM_PN:		lcnd = &SCAN_KEY.LINE_CND[ SCN_SNUM1_PN ];		break;
	case SNOT_TGDATE_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_TDAT1_PN ];		break;
	case SNOT_HUSEN_PN:		lcnd = &SCAN_KEY.LINE_CND[ SCN_HUSEN_PN ];		break;
	default:
		lcnd = NULL;
		break;
	}
	if( lcnd ) {
		if( sgn )	lcnd->KEY_inp |= SC_NOTSEARCH;
		else		lcnd->KEY_inp &= ~SC_NOTSEARCH;
	}
}

void CScanKey::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
//	if( IN_FORK )
//		return;

/*	try
	{
#ifndef CLOSE	// 08.04 /98
# ifndef CLOSE	// 11.19 /99
		// スキャンオンリー時
		if( !bSCAN_ONLY_MODE )
# else
		// 仕訳ＦＵＬＬ時
		if( !scheck( 0 ) )
# endif
			((CDBDinpView*)m_pParent)->endkeyscan();	
		else
			GetParentFrame()->PostMessage( WM_CLOSE );
#else
		((CDBDinpView*)m_pParent)->endkeyscan();	
#endif
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
//	ICSDialog::OnCancel();

  */

	ICSDialog::OnCancel();

}

//
long CScanKey::OnTerminateOnCancel( long nChar )
{
	switch( nChar )
	{
	case VK_LEFT:
	case VK_UP:
	case VK_F2:
#ifndef CLOSE	// 08.03 /98
		set_focus( SCN_IDOK );
#else
		GotoDlgCtrl( GetDlgItem( IDOK ) );
#endif
		return 0;
	case VK_DOWN:
	case VK_RIGHT:
		return 0;
	}
	return nChar;
}

long CScanKey::OnTerminateOnTkyOnly( long nChar )
{
	return term_focus( SCN_CHKDBM_PN, nChar );
/*
	switch( nChar )
	{
	case VK_RIGHT:
	case VK_DOWN:
	case VK_F3:
		GotoDlgCtrl( GetDlgItem( IDOK ) );
		return 0;
	case VK_LEFT:
	case VK_UP:
	case VK_F2:
		set_focus( SCN_TKY_PN );
		return 0;
	}
	return nChar;
*/
}


// 借方部門 のﾁｪｯｸﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKey::OnTermCheckDBm( long nChar )
{
	return term_focus( SCN_CHKDBM_PN, nChar );
}

// 部門 のﾁｪｯｸﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKey::OnTermCheckCBm( long nChar )
{
	return term_focus( SCN_CHKCBM_PN, nChar );
}

// 工事 のﾁｪｯｸﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKey::OnTermCheckDkoji( long nChar )
{
	return term_focus( SCN_CHKDKJ_PN, nChar );
}

// 貸方工事 のﾁｪｯｸﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKey::OnTermCheckCkoji( long nChar )
{
	return term_focus( SCN_CHKCKJ_PN, nChar );
}


// 借方 のﾁｪｯｸﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKey::OnTermCheckDBR( long nChar )
{
	return term_focus( SCN_CHKDBR_PN, nChar );
}


// 貸方 のﾁｪｯｸﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKey::OnTermCheckCBR( long nChar )
{
	return term_focus( SCN_CHKCBR_PN, nChar );
}

// ターミネーション 摘要選択コンボ
long CScanKey::OnTerminateOnSelTky( long nChar )
{
	return term_focus( SCN_SELTKY_PN, nChar );
}

// ターミネーション イメージ摘要エディット
long CScanKey::OnTerminateOnImgTkyEdit( long nChar )
{
	if( nChar ==  VK_TAB ) {
		if( !is_SHIFT() ) nChar = VK_DOWN;
		else		      nChar = VK_F2;
	}

	return term_focus( SCN_TKY_PN, nChar );

}


// 表示順のｺﾝﾎﾞﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKey::OnTermComboOrder( long nChar )
{
	return term_focus( SCN_ORDER_PN, nChar );
}

// データユーザのｺﾝﾎﾞﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKey::OnTermComboDUser( long nChar )
{
	return term_focus( SCN_DUSER_PN, nChar );
}

// 検索タイプのｺﾝﾎﾞﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKey::OnTermComboSType( long nChar )
{
	return term_focus( SCN_STYPE_PN, nChar );
}

// 付箋のｺﾝﾎﾞﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKey::OnTermComboHusen( long nChar )
{
	switch( nChar ) {
	case VK_RIGHT:
		set_focus( SCN_IDOK );
		return 0;
		break;
	default:
		break;
	}

	return term_focus( SCN_HUSEN_PN, nChar );
}


long CScanKey::OnTermComboNoInp( long nChar )
{
	switch( nChar ) {
	case VK_RETURN:
	case VK_RIGHT:
//	case VK_F3:
		set_focus( SCN_IDOK );
		return 0;
		break;
	default:
		break;
	}

	return term_focus( SCN_NOINP_PN, nChar );
}

// NOT ﾁｪｯｸﾎﾞｯｸｽ か？
int CScanKey::IsNotChkbox( HWND hwnd )
{
	int ret = -1;
	int pn, id;

	for( pn = SNOT_SEQ_PN; pn <= SNOT_HUSEN_PN; pn++ ) {
		id = scGetINP_CTL( pn )->IDC_X;
		if( GetDlgItem( id )->m_hWnd == hwnd ) {
			ret = pn;
			break;
		}
	}

	return ret;
}

// NOT ﾁｪｯｸﾎﾞｯｸｽ ﾀｰﾐﾈｰｼｮﾝ
long CScanKey::OnTermNotChkbox( int pn, long nChar )
{
	return term_focus( pn, nChar );
}


struct _KeyMoveTbl* CScanKey::Get_KeyMoveTbl( int nowpn )
{
	struct _KeyMoveTbl* km, *ret;
	km = SkeyMvTbl;
	ret = NULL;

	for( int i = 0; (km+i)->nowpn != -1; i++ ) {
		if( (km+i)->nowpn == nowpn ) {
			ret = (km+i);
			break;
		}
	}

	return ret;
}

// ターミネーション フォーカスセット
long CScanKey::term_focus( int nowpn, long nChar )
{
	struct _KeyMoveTbl* km, *first, *tmp;
	int pn, ret;
	km = SkeyMvTbl;
	first = SkeyMvTbl;

	km = Get_KeyMoveTbl( nowpn );
	if( km == NULL )
		return nChar;

	tmp = km;
	ret = nChar;

	CComboBox* pCmb = NULL;

	switch( nowpn ) {
	case SCN_ORDER_PN:	pCmb = &m_Order;	break;
	case SCN_DUSER_PN:	pCmb = &m_DUser;	break;	
	case SCN_STYPE_PN:	pCmb = &m_SType;	break;	
	case SCN_SYZSW_PN:	pCmb = &m_Syzsw;	break;
	case SCN_ZRITU_PN:	pCmb = &m_Zritu;	break;
	case SCN_SKBN_PN:	pCmb = &m_Skbn;		break;
	case SCN_URISIRE_PN:	pCmb = &m_Urisire;	break;
	case SCN_SELTKY_PN:		pCmb = &m_Tkycb;	break;
	case SCN_HUSEN_PN:		pCmb = &m_Hcbo;		break;
	case SCN_NOINP_PN:		pCmb = &m_NipBrn;	break;
	}
	
	if( pCmb ) {
		switch( nChar ) {
		case VK_DOWN:
			if( !pCmb->GetDroppedState() ) {
				pCmb->ShowDropDown(TRUE);
				ret = 0;
			}
			nChar = 0;
			break;
		case VK_DELETE:
		case VK_RETURN:
		case VK_RIGHT:
//		case VK_F3:
		case VK_F2:
		case VK_LEFT:
			if( pCmb->GetDroppedState() )
				pCmb->ShowDropDown(FALSE);
			break;
		}
	}

	switch( nChar ) {
	case VK_DELETE:
	case VK_RETURN:
	case VK_TAB:
		km++;
		if( km->nowpn != -1 ) {
			do {
				if( scGetINP_CTL( km->nowpn )->INP_ena ) {
					set_focus( km->nowpn );
					ret = 0;
					break;
				}
				km++;
			} while( km->nowpn != -1 );
		}
		break;

	case VK_RIGHT:
//	case VK_F3:
		if( tmp->right_pn != -1 ) {
			while( 1 ) {
				if( scGetINP_CTL( tmp->right_pn )->INP_ena ) {
					set_focus( tmp->right_pn );
					ret = 0;
					break;
				}
				else {
					pn = tmp->right_pn;
					tmp = Get_KeyMoveTbl( pn );
					if( tmp == NULL )	break;
					else if( tmp == km )	break;
					else if( tmp->right_pn == -1 )	break;
				}
			}
		}
		break;

	case VK_DOWN:
		if( tmp->dn_pn != -1 ) {
			while( 1 ) {
				if( scGetINP_CTL( tmp->dn_pn )->INP_ena ) {
					set_focus( tmp->dn_pn );
					ret = 0;
					break;
				}
				else {
					pn = tmp->dn_pn;
					tmp = Get_KeyMoveTbl( pn );
					if( tmp == NULL )	break;
					else if( tmp == km )	break;
					else if( tmp->dn_pn == -1 )	break;
				}
			}
		}
		break;
	case VK_F2:
	case VK_LEFT:
		if( tmp->left_pn != -1 ) {
			while( 1 ) {
				if( scGetINP_CTL( tmp->left_pn )->INP_ena ) {
					set_focus( tmp->left_pn );
					ret = 0;
					break;
				}
				else {
					pn = tmp->left_pn;
					tmp = Get_KeyMoveTbl( pn );
					if( tmp == NULL )	break;
					else if( tmp == km )	break;
					else if( tmp->left_pn == -1 )	break;
				}
			}
		}
		break;
	case VK_UP:
		if( tmp->up_pn != - 1 ) {
			while( 1 ) {
				if( scGetINP_CTL( tmp->up_pn )->INP_ena ) {
					set_focus( tmp->up_pn );
					ret = 0;
					break;
				}
				else {
					pn = tmp->up_pn;
					tmp = Get_KeyMoveTbl( pn );
					if( tmp == NULL )	break;
					else if( tmp == km )	break;
					else if( tmp->up_pn == -1 )	break;
				}
			}
		}
		break;
	}

	return ret;
}


void CScanKey::OnSelchangeDisporder() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Order.GetCurSel();

	SCAN_KEY.SCN_ORDER_DT.SKBN_SGN1 = sel;
	SCAN_KEY.LINE_CND[ SCN_ORDER_PN ].INP_sg = TRUE;
}

void CScanKey::OnSelchangeDataUser() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_DUser.GetCurSel();

	SCAN_KEY.SCN_DUSER_DT.SKBN_SGN1 = sel;
	SCAN_KEY.LINE_CND[ SCN_DUSER_PN ].INP_sg = TRUE;	
}


void CScanKey::OnSelchangeComboStype() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_SType.GetCurSel();

	SCAN_KEY.SCN_STYPE_DT.SKBN_SGN1 = sel;
	SCAN_KEY.LINE_CND[ SCN_STYPE_PN ].INP_sg = TRUE;		
}

void CScanKey::OnSelchangeScnhusen() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Hcbo.GetCurSel();

	if( sel == 0 ) {
		SCAN_KEY.SCN_HUSEN_DT.SKBN_SGN1 = 0;
		strcpy_s( SCAN_KEY.SCN_HUSEN_DT.SKBN_TXT, sizeof SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT, "" );
		SCAN_KEY.LINE_CND[ SCN_HUSEN_PN ].INP_sg = FALSE;
	}
	else {
		SCAN_KEY.SCN_HUSEN_DT.SKBN_SGN1 = sel;
		SCAN_KEY.LINE_CND[ SCN_HUSEN_PN ].INP_sg = TRUE;	
	}
}

// 入力漏れ
void CScanKey::OnSelchangeScnnoinpbrn() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_NipBrn.GetCurSel();

	SCAN_KEY.SCN_NOINP_DT.SKBN_SGN1 = sel;
	if( sel )	SCAN_KEY.LINE_CND[ SCN_NOINP_PN ].INP_sg = TRUE;		
	else		SCAN_KEY.LINE_CND[ SCN_NOINP_PN ].INP_sg = FALSE;
}



void CScanKey::OnSize(UINT nType, int cx, int cy) 
{
#ifdef _ICSPEN_	// 03.05 /01
//	if( IN_FORK )
//		return;
#endif

	ICSDialog::OnSize(nType, cx, cy);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if( m_DIALOG_OK )
	{
		//表題を窪み表示
		selbox( get_nowpn() );
		// コメント表示
		cmntbox( get_nowpn() );

		if( caution_dispcnd() == TRUE )
			_disp_caution( TRUE );
		if( is_mdfy_imgtky() == TRUE )	// イメージ摘要のｴﾃﾞｨｯﾄﾎﾞｯｸｽON
			disp_imgtkyedit( SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_type );

		PostMessage( WM_RETROFOCUS, get_nowpn() );	// 現在行枝番摘要表示のため
//		dsp_etc( get_nowpn(), TRUE );
	}	
}



/***************************
	科目選択チェック
****************************/
int CScanKey::InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick/*=FALSE*/ )
{

	return m_Seldata.InputItemCheck( ans, check, _dcsw, bSelClick );
}

/*========================
	摘要選択
=========================*/
int CScanKey::tky_select( CDBipTKREC* ptkrec, char *data )
{

	SelResult srt;
	srt.sel_pn = SL_TKY_PN;

	if( ! isdigitstr( data ) )	//数字文字列
		return -1;

	if( m_Seldata.GetSelResult( &srt, data ) ) {
		ptkrec->m_tkcod = srt.tky_code;
		ptkrec->m_tkname = srt.tky;
		return 0;
	}

	return -1;

#ifdef SEL_KEEP

	unsigned char *p;
	char str[50];
	int /*i,*/sel_no;
//0	TKattr = 0;	//適用レコード消費税コード
	CDBipTKREC dbtkrec;

	p = (unsigned char *)data;
	strcpy( str, (char *)data );
#ifndef CLOSE // 05.28 /01
	if( isdigitstr( (char*)p ) )	//数字文字列
#else
	if( isdigit( *p ) )	//数字
#endif
	{
		//摘要コード
		if( strlen( str ) > 2 )
		{
			if( (sel_no = atoi( str )) <= 0 )
			{
				goto ERR_;
			}

TK_CODIN:

			// データベース 摘要読み込み
			if( m_pZm->DB_TekiyoNoRead( &dbtkrec, sel_no ) == ERR )
				goto ERR_;

			TkNamTrimRight( &dbtkrec );	// 摘要名称後ろスペースカット

#ifdef DB_CLOSE
			if( IDXENT( Ntkid ) < sel_no )
				goto ERR_;
			if( m_pZm->Tekiyo50onRead( &TKRECD, sel_no ) == ERR )
				goto ERR_;
# ifndef CLOSE	// 09.07 /99
			TkNamTrimRight( &TKRECD );	// 摘要名称後ろスペースカット
# endif

			ptk = &TKRECD;
#endif
		}
		else	//番号選択
		{
#ifndef CLOSE	// 10.21 /9
# ifndef CLOSE	// 09.06 /99
			if( (sel_no = atoi( str ) - (isEntrySys() ? 9 : 1) ) < 0 )
# else
			if( (sel_no = atoi( str ) - 1 ) < 0 )
# endif
#else
			if( (sel_no = atoi( str ) - 9) < 0 )
#endif
			{
				goto ERR_;
			}

			if( TKDsw == RENTKNO ) { /* ﾚﾝｿｳ ﾃｷﾖｳ ﾋｮｳｼﾞ */
			/*	if( sel_no >= DP_MAX ) {
					if( (sel_no - DP_MAX) >= DP_MAX ) goto ERR_;
					if( RTpage ) sel_no += DP_MAX;
					if( sel_no - DP_MAX >= RTKct[1] ) goto ERR_;
					sel_no += (RENT_MAX - DP_MAX);
				}
				else {
					if( RTpage ) sel_no += DP_MAX;
					if( sel_no >= RTKct[0] ) goto ERR_;
				}
			*/
  				sel_no = TKSEL_PG[sel_no];
				goto	TK_CODIN;
			}
			else if( TKDsw == SWKTKNO ) {	 /* ｼﾞﾄﾞｳｼﾜｹﾃｷﾖｳ */
				if( SW_AutoTK == 0 || sel_no >= TATdcnt ) goto ERR_;			
			//	sel_no = m_ASTKCX[AC_MAX * (TATpage-1) + sel_no];
				sel_no = TKSEL_PG[sel_no];
				goto	TK_CODIN;
			}
			else if( TKDsw == KANATKNO ) {  /* 50ｵﾝ ﾃｷﾖｳ ﾋｮｳｼﾞ */
				if( SW_TK50 == 0 || sel_no >= T50dcnt ) goto ERR_;
			//	sel_no = TKRECX[AC_MAX * (T50pag-1) + sel_no];
				sel_no = TKSEL_PG[sel_no];
				goto	TK_CODIN;
			}	
			else
				goto ERR_;
		}
	}
	else
	{
ERR_:
		return -1;
	}

	*tkrec = dbtkrec;
//	memmove( tkrec, ptk, sizeof( TKREC ) );

#endif

	return 0;

}



//
// 部門コードをｾｯﾄ
int CScanKey::bmncode_todata( int* bmn, int number, int dc_sw )
{
	BOOL ret;
	SelResult srt;

	srt.sel_pn = SL_BUMON_PN;
	srt.bmn = -1;
	srt.selname.Empty();

	char buf[36];
	sprintf_s( buf, sizeof buf, "%d", number );
	ret = m_Seldata.GetSelResult( &srt, buf );
	if( ret ) *bmn = srt.bmn;

	return ret;
}

//
// 工事コードをｾｯﾄ
int CScanKey::kojicode_todata( CString& kjcd, int number, int dc_sw )
{
	int ret = 0;

	return ret;
}



//--------------------------------
// その他項目　表示
//--------------------------------
BOOL CScanKey::_dsp_etc( CWnd *pwnd, struct _ItemData *data, struct _LINE_CND *cnd, struct _InpCtl *ctl, double x_rate, BOOL dspsw, BOOL focusoff_disp )
{
CWnd *itemorg;
CWnd *etcorg;
CWnd *prevorg;
WINDOWPLACEMENT itemplace;
RECT newrect;
LONG hight,vh;
double rate_w, rate_h;
char tmp[30], fmt[30];
DISPDATA dispdata = {0};

#ifdef _DEBUG
if( ctl == NULL )	return FALSE;
#endif

	VERIFY(etcorg = pwnd->GetDlgItem( ctl->IDC_X2 ));
	for( int i = 1; ;i++ ) {
		if( (ctl-i)->IDC_X != -1 ) {
			VERIFY(prevorg = pwnd->GetDlgItem( (ctl-i)->IDC_X ));
			break;
		}
	}
//	VERIFY(prevorg = pwnd->GetDlgItem( (ctl-1)->IDC_X ));
	VERIFY(itemorg = pwnd->GetDlgItem( ctl->IDC_X ));

	dispdata.xpos = 3;
	dispdata.ypos = 3;
	dispdata.str = data->ETC_TXT;
	((CICSDisp*)etcorg)->ClearDisp();

	if( cnd->ETC_TXT_sg && dspsw )
	{
		// 枝番摘要表示
		itemorg->GetWindowPlacement( &itemplace );

		// その他表示欄を科目表示欄の上に
		itemorg->GetWindowPlacement( &itemplace );
		newrect = itemplace.rcNormalPosition;
		if( pwnd->GetFocus() == itemorg )
		{
			// 現在フォーカスがある
		//	get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット
			rate_w = m_Xrate;
			rate_h = m_Yrate;
			vh = 2.0 * rate_h;
			hight = (newrect.bottom - newrect.top);
			newrect.bottom = newrect.top + /*hight*1/3 +*/ vh;
			newrect.top = newrect.bottom - hight;

			etcorg->SetWindowPos( &pwnd->wndTop, newrect.left, newrect.top, (newrect.right - newrect.left) * x_rate /*1.7*/, newrect.bottom - newrect.top, SWP_NOACTIVATE );

//			dispdata.forecl = ((CICSInput*)itemorg)->GetForeColor();		// 文字色
			dispdata.forecl = cnd->ETC_TXT_ForeCOL;		// 文字色
			
			((CICSDisp*)etcorg)->DotDisp( (LPUNKNOWN)(&dispdata) );

			return TRUE;
		}
		else if( focusoff_disp == TRUE )
		{
			// 現在フォーカスが無い
			switch( ctl->IDC_X2 ) {
#ifdef LATER_CLOSE	///////////////////
			case IDC_DISPBM1:	case IDC_DISPBM2:	// 部門名称
			case IDC_DISPBM3:	case IDC_DISPBM4:
			case IDC_DISPBM5:	case IDC_DISPBM6:
			case IDC_DISPBM7:
				sprintf( fmt, "%%.%ds", get_strcnt( data->ETC_TXT, 8 ) );
				sprintf( tmp, fmt, data->ETC_TXT );			
				break;
#endif	///////////////////////////////
			case 1:	break;
			default:
			// 枝番名称
				sprintf_s( fmt, sizeof fmt, "%%.%ds", get_strcnt( data->ETC_TXT, 17 ) );
				sprintf_s( tmp, sizeof tmp, fmt, data->ETC_TXT );
				break;
			}

			dispdata.str = tmp;

			etcorg->SetWindowPos( prevorg, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOACTIVATE );

			dispdata.forecl = cnd->ETC_TXT_ForeCOL; //sg SYOGBR_ForeCOL;		// 文字色
			((CICSDisp*)etcorg)->DotDisp( (LPUNKNOWN)(&dispdata) );
			return TRUE;
		}
		else
		{
			// その他表示欄を最小に
			etcorg->SetWindowPos( prevorg, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE );
			return FALSE;
		}
	}
	else
	{
		// その他表示欄を最小に
		etcorg->SetWindowPos( prevorg, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE );
		return FALSE;
	}
}



// 固定科目選択画面状態ＳＷ
BOOL CScanKey::fix8cnd()
{
	return fix8sg;
}

void CScanKey::fix8cnd( BOOL sg )
{
	fix8sg = sg;
}

// 選択画面表示色状態
BOOL CScanKey::fixvalcnd()
{
	return fixvalsg;
}

void CScanKey::fixvalcnd( BOOL sg )
{
	fixvalsg = sg;
}


//
// 固定項目の表示
void CScanKey::fix8disp( BOOL sg )
{
	if( sg )
	{
		// 固定科目選択のディセーブル／イネーブル
		enable_fix8( sg );

		// 選択タイトル表示のディセーブル／イネーブル
		valtitle( !sg );
	}
	else
	{
		// 選択タイトル表示のディセーブル／イネーブル
		valtitle( !sg );

		// 固定科目選択のディセーブル／イネーブル
		enable_fix8( sg );
	}

	fix8cnd( sg );

}

// 固定科目選択のディセーブル／イネーブル
void CScanKey::enable_fix8( BOOL sg )
{
	if( flgPen )
	{
		RECT rect1,rect2;
		CWnd *pfix8,*pSel32;

		VERIFY(pfix8 = GetDlgItem( IDC_ICSSELCTRL1 ));
		VERIFY(pSel32 = GetDlgItem( IDC_ICSSELCTRL2 ));
		pfix8->GetWindowRect( &rect1 );
		ScreenToClient( &rect1 );
		pSel32->GetWindowRect( &rect2 );
		ScreenToClient( &rect2 );

		// New Size
		if( sg == TRUE )
			rect2.top = rect1.bottom/* + 1*/;
		else
			rect2.top = rect1.top;

		pSel32->SetWindowPos( 0, rect2.left, rect2.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER );

	}
	else
	{
		RECT newrect;
		double rate_w, rate_h;

	//	get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット
	//	GetRate( rate_w, rate_h );
		rate_w = rate_h = 1.0;

		CWnd *pfix8;
		VERIFY(pfix8 = GetDlgItem( IDC_ICSSELCTRL1 ));

		// New Size
		if( sg == TRUE )
		{
			newrect.top = 0;
			newrect.left = 0;
			newrect.bottom = m_selfix8_rect.bottom * rate_h;
			newrect.right = m_selfix8_rect.right * rate_w;
		}
		else
		{
			newrect.top = 0;
			newrect.left = 0;
			newrect.bottom = 0;
			newrect.right = 0;
		}
		pfix8->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE );
	//	pfix8->UpdateWindow();
	}

}

//
// 選択画面の表示色
void CScanKey::enable_fixval( BOOL sg )
{
	CICSSel *pfix8,*pSel32;
	CICSDisp *ttlbaseorg,*ttlorg;

	VERIFY(pfix8 = (CICSSel*)GetDlgItem( IDC_ICSSELCTRL1 ));
	VERIFY(pSel32 = (CICSSel*)GetDlgItem( IDC_ICSSELCTRL2 ));

	VERIFY(ttlbaseorg = (CICSDisp*)GetDlgItem( IDC_VALTITLEBASE ));
	VERIFY(ttlorg = (CICSDisp*)GetDlgItem( IDC_VALTITLE ));

	// New Size
	if( sg )
	{
		// 科目選択画面背景色の設定
		pfix8->SetBackColor( RGB_FIX8_BACK );
		pSel32->SetBackColor( RGB_VAL32_BACK );

		// 科目選択画面文字色の設定
		pfix8->SetForeColor( RGB_FIX8_FORE );
		pSel32->SetForeColor( RGB_VAL32_FORE );
		// 科目選択画面枠の色
		pfix8->SetLineColor( RGB_GRAY );
		pSel32->SetLineColor( RGB_GRAY );

		// 選択タイトル背景色、文字色
		ttlbaseorg->SetBackColor( RGB_VAL32_BACK );
		ttlorg->SetBackColor( RGB_VAL32_BACK );
		ttlorg->SetForeColor( RGB_VAL32_FORE );
	}
	else
	{
		// 科目選択画面背景色の設定
		pfix8->SetBackColor( RGB_FIX8_BACK_UNSEL );
		pSel32->SetBackColor( RGB_VAL32_BACK_UNSEL );
		// 科目選択画面文字色の設定
		pfix8->SetForeColor( RGB_FIX8_FORE_UNSEL );
		pSel32->SetForeColor( RGB_VAL32_FORE_UNSEL );
		// 科目選択画面枠の色
		pfix8->SetLineColor( RGB_GRAY );
		pSel32->SetLineColor( RGB_GRAY );

		// 選択タイトル背景色、文字色
		ttlbaseorg->SetBackColor( RGB_VAL32_BACK_UNSEL );
		ttlorg->SetBackColor( RGB_VAL32_BACK_UNSEL );
		ttlorg->SetForeColor( RGB_VAL32_FORE_UNSEL );

	}

//	// 表示
//	pfix8->UpdateWindow();
//	pSel32->UpdateWindow();
//	ttlbaseorg->UpdateWindow();
//	ttlorg->UpdateWindow();

	fixvalcnd( sg );
}

// 選択タイトルテキストセット
void CScanKey::set_valtitle( char *msg )
{
	puts( IDC_VALTITLE, msg, CICSDisp );
}

// 選択タイトル表示のディセーブル／イネーブル
void CScanKey::valtitle( BOOL sg )
{
	CWnd *ttlbaseorg,*ttlorg;
	CICSSel *pfix8,*pSel32;
	RECT rect8,rect32,newrect,newrect2;
	LONG v_h, v_w;
	double rate_w, rate_h;
	WINDOWPLACEMENT place;

//	get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット
//	GetRate( rate_w, rate_h );
	rate_w = rate_h = 1.0;

	VERIFY(ttlbaseorg = (CICSDisp*)GetDlgItem( IDC_VALTITLEBASE ));
	VERIFY(ttlorg = (CICSDisp*)GetDlgItem( IDC_VALTITLE ));

/*
#ifdef	_ICSPEN_	// 01.20 /01
	//Penの時は表示無し
	if( IsPen() ) {
		ttlbaseorg->MoveWindow( 0,0,0,0 );
		ttlorg->MoveWindow( 0,0,0,0 );
		return;
	}
#endif	//end of _ICSPEN_
*/
	// New Size
	if( sg == TRUE )
	{
		VERIFY(pfix8 = (CICSSel*)GetDlgItem( IDC_ICSSELCTRL1));
		VERIFY(pSel32 = (CICSSel*)GetDlgItem( IDC_ICSSELCTRL2));
		pfix8->GetWindowRect( &rect8 );
		pSel32->GetWindowRect( &rect32 );

		newrect.top = rect8.top;
		newrect.left = rect8.left;
		newrect.bottom = rect8.bottom;	// m_selfix8_rect.bottom * rate_h;
		newrect.right = rect8.right;		// m_selfix8_rect.right * rate_w;
		ttlbaseorg->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE );

		newrect2 = m_valtitle_rect;

		newrect2.bottom = newrect2.bottom * rate_h;
		newrect2.right = newrect2.right * rate_w;

		v_h = (newrect.bottom - newrect.top) / 3;
		v_w = ((newrect.right - newrect.left) / 12);
		ttlbaseorg->GetWindowPlacement( &place );
		newrect2.top = place.rcNormalPosition.top + v_h;
		newrect2.left = place.rcNormalPosition.left + v_w;
		newrect2.bottom = newrect2.top + v_h;
		newrect2.right = newrect2.left + (v_w*10);

		ttlorg->SetWindowPos( 0, newrect2.left, newrect2.top, newrect2.right - newrect2.left, newrect2.bottom - newrect2.top, SWP_NOZORDER|SWP_NOACTIVATE );
	}
	else
	{
		newrect.top = 0;
		newrect.left = 0;
		newrect.bottom = 0;
		newrect.right = 0;

		ttlbaseorg->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE );
		ttlorg->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOZORDER|SWP_NOACTIVATE );
	}

//	ttlbaseorg->UpdateWindow();
//	ttlorg->UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////////////

void CScanKey::OnCheckDbm() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int chk = ((CButton*)GetDlgItem(IDC_CHECK_DBM))->GetCheck();

	if( ! (Voln1->sub_sw&0x02) ) {
		SCAN_KEY.LINE_CND[SCN_BMON1_PN].KEY_inp	&= ~SC_ITEMNONE;
		return;
	}

	m_Bmon1.EnableWindow( !chk );
	scGetINP_CTL(SCN_BMON1_PN)->INP_ena = !chk;
	m_Bmon2.EnableWindow( !chk );
	scGetINP_CTL(SCN_BMON2_PN)->INP_ena = !chk;
	
	if( chk )	SCAN_KEY.LINE_CND[SCN_BMON1_PN].KEY_inp |= SC_ITEMNONE;
	else		SCAN_KEY.LINE_CND[SCN_BMON1_PN].KEY_inp	&= ~SC_ITEMNONE;

//	SCAN_KEY.LINE_CND[SCN_BMON1_PN].KEY_inp = chk;
}

void CScanKey::OnCheckCbm() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int chk = ((CButton*)GetDlgItem(IDC_CHECK_CBM))->GetCheck();

	if( ! (Voln1->sub_sw&0x02) ) {
		SCAN_KEY.LINE_CND[SCN_CBMN1_PN].KEY_inp	&= ~SC_ITEMNONE;
		return;
	}

	m_Cbmn1.EnableWindow( !chk );
	scGetINP_CTL(SCN_CBMN1_PN)->INP_ena = !chk;
	m_Cbmn2.EnableWindow( !chk );
	scGetINP_CTL(SCN_CBMN2_PN)->INP_ena = !chk;

	if( chk )	SCAN_KEY.LINE_CND[SCN_CBMN1_PN].KEY_inp |= SC_ITEMNONE;
	else		SCAN_KEY.LINE_CND[SCN_CBMN1_PN].KEY_inp	&= ~SC_ITEMNONE;

//	SCAN_KEY.LINE_CND[SCN_CBMN1_PN].KEY_inp = chk;
}

void CScanKey::OnCheckDkj() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int chk = ((CButton*)GetDlgItem(IDC_CHECK_DKJ))->GetCheck();
	if( Voln1->apno == 0x20 && (Voln1->sub_sw&0x04) ) {
		m_Koji1.EnableWindow( !chk );
		scGetINP_CTL(SCN_KOJI1_PN)->INP_ena = !chk;
		m_Koji2.EnableWindow( !chk );
		scGetINP_CTL(SCN_KOJI2_PN)->INP_ena = !chk;

		if( chk )	SCAN_KEY.LINE_CND[SCN_KOJI1_PN].KEY_inp |= SC_ITEMNONE;
		else		SCAN_KEY.LINE_CND[SCN_KOJI1_PN].KEY_inp	&= ~SC_ITEMNONE;
	}
//	SCAN_KEY.LINE_CND[SCN_KOJI1_PN].KEY_inp = chk;
}


void CScanKey::OnCheckCkj() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int chk = ((CButton*)GetDlgItem(IDC_CHECK_CKJ))->GetCheck();
	if( Voln1->apno == 0x20 && (Voln1->sub_sw&0x04) ) {
		m_Ckji1.EnableWindow( !chk );
		scGetINP_CTL(SCN_CKJI1_PN)->INP_ena = !chk;
		m_Ckji2.EnableWindow( !chk );
		scGetINP_CTL(SCN_CKJI2_PN)->INP_ena = !chk;

		if( chk )	SCAN_KEY.LINE_CND[SCN_CKJI1_PN].KEY_inp |= SC_ITEMNONE;
		else		SCAN_KEY.LINE_CND[SCN_CKJI1_PN].KEY_inp	&= ~SC_ITEMNONE;
	}
//	SCAN_KEY.LINE_CND[SCN_CKJI1_PN].KEY_inp = chk;
}

void CScanKey::OnCheckDbr() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int chk = ((CButton*)GetDlgItem(IDC_CHECK_DBR))->GetCheck();

	m_Dbr1.EnableWindow( !chk );
	scGetINP_CTL(SCN_DBR1_PN)->INP_ena = !chk;
	m_Dbr2.EnableWindow( !chk );
	scGetINP_CTL(SCN_DBR2_PN)->INP_ena = !chk;

	if( chk )	SCAN_KEY.LINE_CND[SCN_DBR1_PN].KEY_inp |= SC_ITEMNONE;
	else		SCAN_KEY.LINE_CND[SCN_DBR1_PN].KEY_inp	&= ~SC_ITEMNONE;

	ChangeEnableDcbr();
//	SCAN_KEY.LINE_CND[SCN_DBR1_PN].KEY_inp = chk;
}

void CScanKey::OnCheckCbr() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int chk = ((CButton*)GetDlgItem(IDC_CHECK_CBR))->GetCheck();

	m_Cbr1.EnableWindow( !chk );
	scGetINP_CTL(SCN_CBR1_PN)->INP_ena = !chk;
	m_Cbr2.EnableWindow( !chk );
	scGetINP_CTL(SCN_CBR2_PN)->INP_ena = !chk;

	if( chk )	SCAN_KEY.LINE_CND[SCN_CBR1_PN].KEY_inp |= SC_ITEMNONE;
	else		SCAN_KEY.LINE_CND[SCN_CBR1_PN].KEY_inp	&= ~SC_ITEMNONE;

	ChangeEnableDcbr();
//	SCAN_KEY.LINE_CND[SCN_CBR1_PN].KEY_inp = chk;
}


// 貸／借枝番 入力可否の切り替え
void CScanKey::ChangeEnableDcbr() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int deb_chk = ((CButton*)GetDlgItem(IDC_CHECK_DBR))->GetCheck();
	int cre_chk = ((CButton*)GetDlgItem(IDC_CHECK_CBR))->GetCheck();

	BOOL bEnable;
	bEnable = !(deb_chk || cre_chk);

	m_DCbr1.EnableWindow( bEnable );
	scGetINP_CTL(SCN_DCBR1_PN)->INP_ena = bEnable;
	m_DCbr2.EnableWindow( bEnable );
	scGetINP_CTL(SCN_DCBR2_PN)->INP_ena = bEnable;

}




void CScanKey::OnSelectedIcsselctrl1(short position, long Number) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SelectedIcsselctrl( position, Number, 0 );
}

void CScanKey::OnSelectedIcsselctrl2(short position, long Number) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SelectedIcsselctrl( position, Number, 1 );	
}

void CScanKey::OnAllkeydel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	ScanKeyAllDel();
}

