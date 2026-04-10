// ScanKeyView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"

#include "icsdisp.h"

#include "input1.h"
#include "external.h"

#include "ScanKeyView.h"

#include "scansydef.h"
#include "DataListCtrl.h"
#include "DinpSub.h"
#include "MstRw.h"
#include "MainFrm.h"

//#include "perform_chk.h"


extern CDBSyohi*	pDBsy;
extern SCAN_ITEMEX	SC_Item;
extern CStringArray	SC_Tky;

extern BOOL PROGRAM_OK;
extern BOOL bIKAT_SCAN;

extern struct _AUTOSEL* pAUTOSEL;

extern CBrnTky m_BrTek;

#ifdef CLOSE

// control 表示順序 table
static int ThisCtrl[] = {
	IDC_LIST1, IDC_ST_ORDER, IDC_ST_DUSER, IDC_ST_SEQ, IDC_ST_DATE, IDC_ST_DENP, IDC_ST_DBMN, IDC_ST_CBMN, 
	IDC_ST_DKJ, IDC_ST_CKJ, IDC_ST_DEBT, IDC_ST_DBR, IDC_ST_CRED, IDC_ST_CBR, IDC_ST_DBCR, IDC_ST_DCBR,
	IDC_ST_VAL, IDC_ST_ZEI, IDC_ST_SYZSW, IDC_ST_SKBN, IDC_ST_URISIRE, IDC_ST_TKY, IDC_ST_SNUM, IDC_ST_TDAT, 
	IDC_STATIC1, IDC_STATIC2, IDC_STATIC3, IDC_STATIC4, IDC_STATIC5, IDC_STATIC6, IDC_STATIC7, IDC_STATIC8, 
	IDC_STATIC9, IDC_STATIC10, IDC_STATIC11, IDC_STATIC12, IDC_STATIC13, IDC_STATIC14, IDOK, IDCANCEL, 
	IDC_STATIC_BOX1, IDC_STATIC_BOX2, IDC_STATIC_BOX3,
	IDC_ICSDISPCTRL1, IDC_DISPORDER, IDC_DATA_USER,
	IDC_SCNSEQ1, IDC_SCNSEQ2, /*IDC_SCNDATE1, IDC_SCNDATE2,*/IDC_DATE_SEL, IDC_SCNDENP1, IDC_SCNDENP2, 
	IDC_SCNBMON1, IDC_SCNBMON2, IDC_SCNCBMN1, IDC_SCNCBMN2, IDC_SCNKOJI1, IDC_SCNKOJI2, IDC_SCNCBMN1, IDC_SCNCBMN2,
	IDC_SCNDEBT, IDC_SCNDBR1, IDC_SCNDBR2, IDC_SCNCRED, IDC_SCNCBR1, IDC_SCNCBR2, IDC_SCNDBCR, IDC_SCNDCBR1, IDC_SCNDCBR2, 
	IDC_SCNVAL1, IDC_SCNVAL2, IDC_SCNZEI1, IDC_SCNZEI2, IDC_SCNSYZSW, IDC_SCNSKBN, IDC_SCNZRITU, IDC_SCNURISIRE, 
	IDC_SCNTKY, IDC_SCNSNUM1, IDC_SCNSNUM2, IDC_SCNTDAT1, IDC_SCNTDAT2, IDC_SCNHUSEN,
	IDC_TKYONLY, IDC_IMGTKYEDIT, IDC_TKYCOMBO
};

#endif


// input control table
static struct _InpCtl INP_CTL[SNOT_LAST_PN +1] = {
	{IDC_DISPORDER,0,0,0,-1},{IDC_DATA_USER,0,0,0,-1},
	{IDC_COMBO_STYPE,0,0,0,-1},		//3

	{IDC_SCNSEQ1,0,0,0,-1},{IDC_SCNSEQ2,0,0,0,-1},

//	{IDC_SCNDATE1,0,0,0,-1},{IDC_SCNDATE2,0,0,0,-1},
	{IDC_SCN_DATESEL,0,0,0,-1},{0,0,0,0,-1},
	{IDC_SCNDENP1,0,0,0,-1},{IDC_SCNDENP2,0,0,0,-1},
	{IDC_SCNBMON1,IDC_DISPETC,0,0,-1},{IDC_SCNBMON2,IDC_DISPETC,0,0,-1},
	{IDC_SCNCBMN1,IDC_DISPETC,0,0,-1},{IDC_SCNCBMN2,IDC_DISPETC,0,0,-1},
	{IDC_SCNDCBMN1,IDC_DISPETC,0,0,-1},{IDC_SCNDCBMN2,IDC_DISPETC,0,0,-1},//15

	{IDC_SCNKOJI1,IDC_DISPETC,0,0,-1},{IDC_SCNKOJI2,IDC_DISPETC,0,0,-1},
	{IDC_SCNCKJI1,IDC_DISPETC,0,0,-1},{IDC_SCNCKJI2,IDC_DISPETC,0,0,-1},
	{IDC_SCNDCKJI1,IDC_DISPETC,0,0,-1},{IDC_SCNDCKJI2,IDC_DISPETC,0,0,-1},//21

	{IDC_SCNDEBT,0,0,0,-1},{IDC_SCNDEBT2,0,0,0,-1},{IDC_SCNDBR1,IDC_DISPETC,0,0,-1},{IDC_SCNDBR2,IDC_DISPETC,0,0,-1},
	{IDC_SCNCRED,0,0,0,-1},{IDC_SCNCRED2,0,0,0,-1},{IDC_SCNCBR1,IDC_DISPETC,0,0,-1},{IDC_SCNCBR2,IDC_DISPETC,0,0,-1},
	{IDC_SCNDBCR,0,0,0,-1},{IDC_SCNDBCR2,0,0,0,-1},{IDC_SCNDCBR1,IDC_DISPETC,0,0,-1},{IDC_SCNDCBR2,IDC_DISPETC,0,0,-1},
	{IDC_SCNVAL1,0,0,0,-1},{IDC_SCNVAL2,0,0,0,-1},
	{IDC_SCNZEI1,0,0,0,-1},{IDC_SCNZEI2,0,0,0,-1},
	{IDC_SCNTAIKA1,0,0,0,-1},{IDC_SCNTAIKA2,0,0,0,-1},	//39
	{IDC_SCNSYZSW,0,0,0,-1},{IDC_SCNZRITU,0,0,0,-1},{IDC_SCNSKBN,0,0,0,-1},{IDC_SCNURISIRE,0,0,0,-1},
	{IDC_SCNTOKUS,0,0,0,-1},
	{IDC_SCNTKY,0,0,0,-1},
	{IDC_SCNINVNO,0,0,0,-1},		// 登録番号
	{IDC_SCNSNUM1,0,0,0,-1},
	{IDC_SCNSNUM2,0,0,0,-1},
	{IDC_SCNTDAT1,0,0,0,-1},
	{IDC_SCNTDAT2,0,0,0,-1},
	{IDC_SCNHUSEN,0,0,0,-1},
	{IDC_SCNHUCMT,0,0,0,-1},
	{IDC_CHECK_MENZEI,0,0,0,-1},	// 免税事業者からの課税仕入れ
	{IDC_SCNMENWARI,0,0,0,-1},		// 免税事業者からの課税仕入れ控除割合
	{IDC_SCNNOINPBRN,0,0,0,-1},
	{IDC_SCNGYOKBN,0,0,0,-1},
	{IDC_SCNHJIYU,0,0,-1}, //56

	{IDC_SCNSNO1,0,0,0,-1},{IDC_SCNSNO2,0,0,0,-1},	//証憑番号
	{IDC_SCNDOCSEQ1,0,0,0,-1},{IDC_SCNDOCSEQ2,0,0,0,-1},//文書番号
	{IDC_SCNSHTTYP,0,0,0,-1},
	{IDC_SCNSYZKTSN,0,0,0,-1},	//62 資産
	{IDC_COMBO_MIKAKUD,0,0,0,-1},

	// チェックボックス
	{IDC_FURI_CHK,0,0,0,-1},	
	{IDC_NYUK_CHK,0,0,0,-1},	{IDC_SYUK_CHK,0,0,0,-1},

	{IDC_CHECK_DBM,0,0,0,-1},	{IDC_CHECK_CBM,0,0,0,-1},
	{IDC_CHECK_DKJ,0,0,0,-1},	{IDC_CHECK_CKJ,0,0,0,-1}, //70
	{IDC_CHECK_DBR,0,0,0,-1},
	{IDC_CHECK_CBR,0,0,0,-1},
	{IDC_CHECK_KMSYZ,0,0,0,-1},

	{IDC_CHK_INPDATA,0,0,0,-1},
	{IDC_CHK_MDFDATA,0,0,0,-1},

	// NOT ボタン
	{IDC_NCHK_SEQ, 0,0,0,-1},	{IDC_NCHK_DATE, 0,0,0,-1},	{IDC_NCHK_DENP, 0,0,0,-1},
	{IDC_NCHK_DBMN, 0,0,0,-1},	{IDC_NCHK_CBMN, 0,0,0,-1},	{IDC_NCHK_DCBMN, 0,0,0,-1},
	{IDC_NCHK_DKJI, 0,0,0,-1},	{IDC_NCHK_CKJI, 0,0,0,-1},	{IDC_NCHK_DCKJI, 0,0,0,-1},
	{IDC_NCHK_DBT, 0,0,0,-1},	{IDC_NCHK_DBRN, 0,0,0,-1},
	{IDC_NCHK_CRE, 0,0,0,-1},	{IDC_NCHK_CBRN, 0,0,0,-1},
	{IDC_NCHK_DBCR, 0,0,0,-1},	{IDC_NCHK_DCBRN, 0,0,0,-1},

	{IDC_NCHK_VAL, 0,0,0,-1},	{IDC_NCHK_ZEI, 0,0,0,-1},	{IDC_NCHK_TAI, 0,0,0,-1},
	{IDC_NCHK_SYZSW, 0,0,0,-1},	{IDC_NCHK_ZRITU, 0,0,0,-1},
	{IDC_NCHK_SKBN, 0,0,0,-1},	{IDC_NCHK_URISIRE, 0,0,0,-1},{IDC_NCHK_TOKUS, 0,0,0,-1},

	{IDC_NCHK_TKY,0,0,0,-1},
	{IDC_NCHK_INVNO,0,0,0,-1},	//100
	{IDC_NCHK_SNUMBER,0,0,0,-1},{IDC_NCHK_TGDATE,0,0,0,-1},	{IDC_NCHK_HUSEN,0,0,0,-1},

	{IDC_NCHK_SNO,0,0,0,-1},
	{IDC_NCHK_SEQ2,0,0,0,-1},
	{IDC_NCHK_MENZEI,0,0,0,-1}
};

// キー移動テーブル
//	{ 現在位置, 上, 下, 左, 右} (-1は 移動できない)
//
struct _KeyMoveTbl SkeyMvTbl[] = 
{
	{SCN_ORDER_PN,	-1,-1,	-1,				SCN_DUSER_PN },
	{SCN_DUSER_PN,	-1,-1,	SCN_ORDER_PN,	SCN_STYPE_PN},

	{SCN_CHKINPDT_PN,	SCN_DUSER_PN,	 SCN_CHKMDFDT_PN,	SCN_DUSER_PN,		SCN_CHKMDFDT_PN},
	{SCN_CHKMDFDT_PN,	SCN_CHKINPDT_PN, SCN_STYPE_PN,		SCN_CHKINPDT_PN,	SCN_STYPE_PN},

	{SCN_STYPE_PN,	-1,-1,	SCN_DUSER_PN,	SCN_DTTYPE_PN},
	{SCN_DTTYPE_PN,	-1,-1,	SCN_STYPE_PN,	SNOT_SEQ_PN},

	{SNOT_SEQ_PN,	-1, SNOT_DATE_PN, SCN_DTTYPE_PN,	SCN_SEQ1_PN },

	{SCN_SEQ1_PN,	-1, SCN_DATE1_PN, SNOT_SEQ_PN,	SCN_SEQ2_PN },
	{SCN_SEQ2_PN,	-1, SCN_DATE1_PN, SCN_SEQ1_PN,	SNOT_DOCSEQ_PN },

	{SNOT_DOCSEQ_PN,	-1, SNOT_DATE_PN, SCN_SEQ2_PN,		SCN_DOCSEQ1_PN },
	{SCN_DOCSEQ1_PN,	-1, SNOT_DATE_PN, SNOT_DOCSEQ_PN,	SCN_DOCSEQ2_PN },
	{SCN_DOCSEQ2_PN,	-1, SNOT_DATE_PN, SCN_DOCSEQ1_PN,	SCN_DOCSHTP_PN },

	{SCN_DOCSHTP_PN,	-1,-1,	SCN_DOCSEQ2_PN,	SNOT_DATE_PN},

	{SNOT_DATE_PN,	SNOT_SEQ_PN, SNOT_DENP_PN, SCN_DOCSHTP_PN,	SCN_DATE1_PN },
	{SCN_DATE1_PN,	SCN_SEQ1_PN, SCN_DENP1_PN, SNOT_DATE_PN,	SNOT_DENP_PN },
//	{SCN_DATE2_PN,	SCN_SEQ2_PN, SCN_DENP2_PN, SCN_DATE1_PN,	SNOT_DENP_PN },

	{SNOT_DENP_PN,	SNOT_DATE_PN, SNOT_DOCEVI_PN, SCN_DATE1_PN,	SCN_DENP1_PN },
	{SCN_DENP1_PN,	SCN_DATE1_PN, SCN_DOCEVI1_PN, SNOT_DENP_PN,	SCN_DENP2_PN },
	{SCN_DENP2_PN,	SCN_DATE1_PN, SCN_DOCEVI2_PN, SCN_DENP1_PN,	SCN_GYOKBN_PN },

	{SCN_GYOKBN_PN,	-1,-1,	SCN_DENP2_PN,	SCN_CHKFURI_PN},

	{SNOT_DOCEVI_PN,	SNOT_DENP_PN, SNOT_DBMN_PN,	  SCN_DENP2_PN,		SCN_DOCEVI1_PN },
	{SCN_DOCEVI1_PN,	SCN_DENP1_PN, SCN_CHKFURI_PN, SNOT_DOCEVI_PN,	SCN_DOCEVI2_PN },
	{SCN_DOCEVI2_PN,	SCN_DENP2_PN, SCN_CHKFURI_PN, SCN_DOCEVI1_PN,	SCN_CHKFURI_PN },

	{SCN_CHKFURI_PN,	SCN_DOCEVI1_PN, SCN_KOJI1_PN, SCN_DOCEVI2_PN,	SCN_CHKNYUK_PN },
	{SCN_CHKNYUK_PN,	SCN_DOCEVI1_PN, SCN_KOJI1_PN, SCN_CHKFURI_PN,	SCN_CHKSYUK_PN },
	{SCN_CHKSYUK_PN,	SCN_DOCEVI1_PN, SCN_KOJI1_PN, SCN_CHKNYUK_PN,	SNOT_DBMN_PN },

	{SNOT_DBMN_PN,	SNOT_DENP_PN, SNOT_CBMN_PN, SCN_CHKSYUK_PN,	SCN_BMON1_PN },
	{SCN_BMON1_PN,	SCN_DATE1_PN, SCN_CBMN1_PN, SNOT_DBMN_PN,	SCN_BMON2_PN },
	{SCN_BMON2_PN,	SCN_DATE1_PN, SCN_CBMN2_PN, SCN_BMON1_PN,	SCN_CHKDBM_PN },
	{SCN_CHKDBM_PN, SCN_DENP2_PN, SCN_CHKCBM_PN,SCN_BMON2_PN,	SNOT_DKJI_PN },

	{SNOT_DKJI_PN,	SCN_CHKFURI_PN, SNOT_CKJI_PN,	SCN_CHKDBM_PN,	SCN_KOJI1_PN },
	{SCN_KOJI1_PN,	SCN_CHKFURI_PN, SCN_CKJI1_PN,	SNOT_DKJI_PN,	SCN_KOJI2_PN },
	{SCN_KOJI2_PN,	SCN_CHKFURI_PN, SCN_CKJI2_PN,	SCN_KOJI1_PN,	SCN_CHKDKJ_PN },
	{SCN_CHKDKJ_PN, SCN_CHKFURI_PN, SCN_CHKCKJ_PN,	SCN_KOJI2_PN,	SNOT_CBMN_PN },

	{SNOT_CBMN_PN,	SNOT_DBMN_PN, SNOT_DCBMN_PN,	SCN_CHKDKJ_PN,	SCN_CBMN1_PN },
	{SCN_CBMN1_PN,	SCN_BMON1_PN, SCN_DCBMN1_PN,	SNOT_CBMN_PN,	SCN_CBMN2_PN },
	{SCN_CBMN2_PN,	SCN_BMON2_PN, SCN_DCBMN2_PN,	SCN_CBMN1_PN,	SCN_CHKCBM_PN },
	{SCN_CHKCBM_PN, SCN_CHKDBM_PN,SCN_DCBMN2_PN,	SCN_CBMN2_PN,	SNOT_CKJI_PN },

	{SNOT_CKJI_PN,	SNOT_DKJI_PN, SNOT_DCKJI_PN,	SCN_CHKCBM_PN,	SCN_CKJI1_PN },
	{SCN_CKJI1_PN,	SCN_KOJI1_PN, SCN_DCKJI1_PN,	SNOT_CKJI_PN,	SCN_CKJI2_PN },
	{SCN_CKJI2_PN,	SCN_KOJI2_PN, SCN_DCKJI2_PN,	SCN_CKJI1_PN,	SCN_CHKCKJ_PN },
	{SCN_CHKCKJ_PN, SCN_CHKCKJ_PN,SCN_DCKJI2_PN,	SCN_CKJI2_PN,	SNOT_DCBMN_PN },

	//貸借部門
	{SNOT_DCBMN_PN,	SNOT_CBMN_PN, SNOT_DEB_PN,  SCN_CHKCKJ_PN,	SCN_DCBMN1_PN },
	{SCN_DCBMN1_PN,	SCN_CBMN1_PN, SCN_DEBT_PN,	SNOT_DCBMN_PN,	SCN_DCBMN2_PN },
	{SCN_DCBMN2_PN,	SCN_CBMN2_PN, SCN_DEBT_PN,	SCN_DCBMN1_PN,	SNOT_DCKJI_PN },
	//貸借工事
	{SNOT_DCKJI_PN,	SNOT_CKJI_PN, SNOT_DBRN_PN,SCN_DCBMN2_PN,	SCN_DCKJI1_PN },
	{SCN_DCKJI1_PN,	SCN_CKJI1_PN, SCN_DEBT_PN,	SNOT_DCKJI_PN,	SCN_DCKJI2_PN },
	{SCN_DCKJI2_PN,	SCN_CKJI2_PN, SCN_DEBT_PN,	SCN_DCKJI1_PN,	SNOT_DEB_PN },

	{SNOT_DEB_PN,	SNOT_DCBMN_PN, SNOT_CRE_PN,	SCN_DCKJI2_PN,	SCN_DEBT_PN },
	{SCN_DEBT_PN,	SCN_DCBMN1_PN, SCN_CRED_PN,	SNOT_DEB_PN,	SCN_DEBT2_PN },
	{SCN_DEBT2_PN,	SCN_DCBMN2_PN, SCN_CRED2_PN,SCN_DEBT_PN,	SNOT_DBRN_PN },
	{SNOT_DBRN_PN,	SNOT_DCKJI_PN,SNOT_CBRN_PN,	SCN_DEBT2_PN,	SCN_DBR1_PN },
	{SCN_DBR1_PN,	SCN_DCKJI1_PN, SCN_CBR1_PN,	SNOT_DBRN_PN,	SCN_DBR2_PN },
	{SCN_DBR2_PN,	SCN_DCKJI2_PN, SCN_CBR2_PN,	SCN_DBR1_PN,	SCN_CHKDBR_PN},
	{SCN_CHKDBR_PN,	SCN_CHKCBM_PN,SCN_CHKCBR_PN,SCN_DBR2_PN,	SNOT_CRE_PN },

	{SNOT_CRE_PN,	SNOT_DEB_PN, SNOT_DBCR_PN,	SCN_CHKDBR_PN,	SCN_CRED_PN },
	{SCN_CRED_PN,	SCN_DEBT_PN, SCN_DBCR_PN,	SNOT_CRE_PN,	SCN_CRED2_PN },
	{SCN_CRED2_PN,	SCN_DEBT2_PN, SCN_DBCR2_PN,	SCN_CRED_PN,	SNOT_CBRN_PN },
	{SNOT_CBRN_PN,	SNOT_DBRN_PN, SNOT_DCBRN_PN,SCN_CRED2_PN,	SCN_CBR1_PN },
	{SCN_CBR1_PN,	SCN_DBR1_PN, SCN_DCBR1_PN,	SNOT_CBRN_PN,	SCN_CBR2_PN },
	{SCN_CBR2_PN,	SCN_DBR2_PN, SCN_DCBR2_PN,	SCN_CBR1_PN,	SCN_CHKCBR_PN},
	{SCN_CHKCBR_PN,	SCN_CHKCBM_PN,SCN_CHKCBR_PN,SCN_CBR2_PN,	SNOT_DBCR_PN },

	{SNOT_DBCR_PN,	SNOT_CRE_PN, SNOT_VAL_PN,	SCN_CHKCBR_PN,	SCN_DBCR_PN },
	{SCN_DBCR_PN,	SCN_CRED_PN, SCN_VAL1_PN,	SNOT_DBCR_PN,	SCN_DBCR2_PN },
	{SCN_DBCR2_PN,	SCN_CRED2_PN, SCN_VAL2_PN,	SCN_DBCR_PN,	SNOT_DCBRN_PN },
	{SNOT_DCBRN_PN,	SNOT_CBRN_PN, SNOT_ZEI_PN,	SCN_DBCR2_PN,	SCN_DCBR1_PN },
	{SCN_DCBR1_PN,	SCN_CBR1_PN, SCN_VAL1_PN,	SNOT_DCBRN_PN,	SCN_DCBR2_PN },
	{SCN_DCBR2_PN,	SCN_CBR2_PN, SCN_VAL2_PN,	SCN_DCBR1_PN,	SNOT_VAL_PN},

	{SNOT_VAL_PN,	SNOT_DBCR_PN,SNOT_TAI_PN,	SCN_DCBR2_PN,	SCN_VAL1_PN },
	{SCN_VAL1_PN,	SCN_DBCR_PN, SCN_ZEI1_PN,	SNOT_VAL_PN,	SCN_VAL2_PN },
	{SCN_VAL2_PN,	SCN_DBCR2_PN,SCN_ZEI2_PN,	SCN_VAL1_PN,	SNOT_ZEI_PN },
	
	{SNOT_ZEI_PN,	SNOT_DCBRN_PN,SNOT_ZRITU_PN,	SCN_VAL2_PN,	SCN_ZEI1_PN },
	{SCN_ZEI1_PN,	SCN_VAL1_PN, SCN_TAI1_PN,	SNOT_ZEI_PN,	SCN_ZEI2_PN },
	{SCN_ZEI2_PN,	SCN_VAL2_PN, SCN_TAI2_PN,	SCN_ZEI1_PN,	SNOT_TAI_PN },

	{SNOT_TAI_PN,	SNOT_VAL_PN, SNOT_SYZSW_PN,	SCN_ZEI2_PN,	SCN_TAI1_PN },
	{SCN_TAI1_PN,	SCN_VAL1_PN, SCN_SYZSW_PN,	SNOT_TAI_PN,	SCN_TAI2_PN },
	{SCN_TAI2_PN,	SCN_VAL2_PN, SCN_SYZSW_PN,	SCN_TAI1_PN,	SCN_CHKKMSYZ_PN },

	{SCN_CHKKMSYZ_PN, SNOT_ZEI_PN, SCN_HJIYU_PN, SCN_TAI2_PN,	SCN_SYZKTSN_PN },
	{SCN_SYZKTSN_PN,	-1,-1,	SCN_CHKKMSYZ_PN,	SNOT_SYZSW_PN},

	{SNOT_SYZSW_PN,	SNOT_TAI_PN,	SNOT_SKBN_PN,	SCN_SYZKTSN_PN,	SCN_SYZSW_PN },
	{SCN_SYZSW_PN,	-1,-1,	SNOT_SYZSW_PN,	SNOT_ZRITU_PN },

	{SNOT_ZRITU_PN,	SCN_TAI2_PN,	SNOT_URISIRE_PN,SCN_SYZSW_PN,	SCN_ZRITU_PN },
	{SCN_ZRITU_PN,	-1,-1,	SNOT_ZRITU_PN,	SCN_HJIYU_PN},
	
	{SCN_HJIYU_PN,	-1,-1,	SCN_ZRITU_PN,	SNOT_SKBN_PN},

	{SNOT_SKBN_PN,	SNOT_SYZSW_PN,	SNOT_INVNO_PN,	SCN_HJIYU_PN,	SCN_SKBN_PN },
	{SCN_SKBN_PN,	-1,	-1,	SNOT_SKBN_PN,	SNOT_URISIRE_PN },
	
	{SNOT_URISIRE_PN,SNOT_ZRITU_PN,	SCN_TKY_PN,	SCN_SKBN_PN,	SCN_URISIRE_PN },
	{SCN_URISIRE_PN,-1,-1,	SNOT_URISIRE_PN,	SCN_TOKUS_PN},

	{SNOT_TOKUS_PN,SNOT_ZRITU_PN,	SCN_TKY_PN,	SCN_URISIRE_PN,	SCN_TOKUS_PN },
	{SCN_TOKUS_PN,-1,-1,	SNOT_TOKUS_PN,	SNOT_INVNO_PN },

	// 登録番号
	{ SNOT_INVNO_PN, SNOT_SKBN_PN, SNOT_TKY_PN, SCN_TOKUS_PN,	SCN_INVNO_PN },
	{ SCN_INVNO_PN, SCN_SKBN_PN, SCN_TKYONLY, SNOT_INVNO_PN, SNOT_MENZEI_PN },
	{ SNOT_MENZEI_PN, SNOT_SKBN_PN, SNOT_TKY_PN, SCN_INVNO_PN,	SCN_MENZEI_PN },
	{ SCN_MENZEI_PN, SCN_SKBN_PN, SNOT_TKY_PN, SNOT_MENZEI_PN, SCN_MENWARI_PN },
	{ SCN_MENWARI_PN, -1, -1, SCN_MENZEI_PN, SCN_SELTKY_PN },

	{SCN_SELTKY_PN,	-1,-1,	SCN_MENZEI_PN, SNOT_TKY_PN },
	{SNOT_TKY_PN,	SNOT_INVNO_PN,SNOT_SNUM_PN,	SCN_SELTKY_PN, SCN_TKY_PN },
	//{SCN_TKY_PN,	SCN_SYZSW_PN,SCN_SNUM1_PN,	SNOT_TKY_PN,	SNOT_SNUM_PN},
	{SCN_TKY_PN,	SCN_INVNO_PN,SCN_SNUM1_PN,	SNOT_TKY_PN,	SNOT_SNUM_PN },

	{SNOT_SNUM_PN,	SNOT_TKY_PN, SNOT_HUSEN_PN,SCN_TKY_PN,	SCN_SNUM1_PN },
	{SCN_SNUM1_PN,	SCN_TKY_PN,	SCN_HUSEN_PN,	SNOT_SNUM_PN,	SCN_SNUM2_PN },
	//{SCN_SNUM2_PN,	SCN_TKY_PN,	SCN_TDAT2_PN,	SCN_SNUM1_PN,	SNOT_TGDATE_PN },
	{SCN_SNUM2_PN,	SCN_TKY_PN,	SCN_HUSEN_PN,	SCN_SNUM1_PN,	SNOT_TGDATE_PN },
	{SNOT_TGDATE_PN,SNOT_SNUM_PN, SNOT_HUSEN_PN,SCN_SNUM2_PN,	SCN_TDAT1_PN },
	{SCN_TDAT1_PN,	SCN_SNUM1_PN, SCN_NOINP_PN,	SNOT_TGDATE_PN,	SCN_TDAT2_PN },
	{SCN_TDAT2_PN,	SCN_SNUM2_PN, SCN_NOINP_PN,	SCN_TDAT1_PN,	SNOT_HUSEN_PN },

	{SNOT_HUSEN_PN,	SNOT_SNUM_PN, -1,	SCN_TDAT2_PN,	SCN_HUSEN_PN },
	{SCN_HUSEN_PN,	-1,-1,	SNOT_HUSEN_PN,	SCN_HUCMT_PN },
	{SCN_HUCMT_PN,	SCN_HUSEN_PN,-1,	SCN_HUSEN_PN,	SCN_NOINP_PN},

	{SCN_NOINP_PN,	-1,-1,	SCN_HUCMT_PN,	-1},

	{-1,-1,-1,-1,-1}

};


// Now ポジション
static int NOW_PN = -1;

// 枝番摘要表示コントロールＸ軸拡大比率
#define BRNTKY_X_RATE 3.2

// 部門名称表示コントロールＸ軸拡大比率 08.19 /02
//#define BUMON_X_RATE	(Voln1 && Voln1->APNO == 0x20) ? 3.5 : 2.9
#define BUMON_X_RATE	3.5


// CScanKeyView


int GetPositionFromSkeyID( int ID )
{
	int pn = 0;
	switch( ID ) {
	case IDC_SCNDBR1:	pn = SCN_DBR1_PN;	break;
	case IDC_SCNDBR2:	pn = SCN_DBR2_PN;	break;
	case IDC_SCNCBR1:	pn = SCN_CBR1_PN;	break;
	case IDC_SCNCBR2:	pn = SCN_CBR2_PN;	break;
	case IDC_SCNDCBR1:	pn = SCN_DCBR1_PN;	break;
	case IDC_SCNDCBR2:	pn = SCN_DCBR2_PN;	break;
	}

	return pn;
}


IMPLEMENT_DYNCREATE(CScanKeyView, CDBbaseView)

CScanKeyView::CScanKeyView()
	: CDBbaseView(CScanKeyView::IDD)
{
	m_EditColor[0] = ::CreateSolidBrush( RGB_WHITE );
	m_EditColor[1] = ::CreateSolidBrush( RGB_YELLOW );
	m_EditColor[2] = ::CreateSolidBrush( SELECT_BACK_COL );
	m_ChkColor	= ::CreateSolidBrush( RGB_SKY );

	m_DIALOG_OK	= FALSE;
	m_nDtOwner	= -2;	
	m_nDtMode	= 0;
	m_HenCnt	= 0;

	m_idxTkyReport = -1;

	ASSERT( INP_CTL[SNOT_HUSEN_PN].IDC_X == IDC_NCHK_HUSEN );
	ASSERT( INP_CTL[SCN_DOCSHTP_PN].IDC_X == IDC_SCNSHTTYP );
	ASSERT( INP_CTL[SNOT_DOCSEQ_PN].IDC_X == IDC_NCHK_SEQ2 );

	flgPen = TRUE;

	caution_dispcnd() = FALSE;

	m_bDataTypeCombo = TRUE;
	m_bIMEendBrn = FALSE;

	// インボイス対応
	m_InpMode = INPMODE_NORMMAL;	// 通常入力

	m_bIgnoreFocus = FALSE;
}

CScanKeyView::~CScanKeyView()
{
}

void CScanKeyView::DoDataExchange(CDataExchange* pDX)
{
	CDBbaseView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SCNNOINPBRN, m_NipBrn);
	DDX_Control(pDX, IDC_SCNGYOKBN, m_GyoKbn);
	DDX_Control(pDX, IDC_ALLKEYDEL, m_AllDel);
	DDX_Control(pDX, IDC_COMBO_STYPE, m_SType);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_TKYONLY, m_TKonly);
	DDX_Control(pDX, IDC_DISPORDER, m_Order);
	DDX_Control(pDX, IDC_DATA_USER, m_DUser);
	DDX_Control(pDX, IDC_TKYCOMBO, m_Tkycb);
	DDX_Control(pDX, IDC_SCNZRITU, m_Zritu);
	DDX_Control(pDX, IDC_SCNURISIRE, m_Urisire);
	DDX_Control(pDX, IDC_SCNSYZSW, m_Syzsw);
	DDX_Control(pDX, IDC_SCNSKBN, m_Skbn);
	DDX_Control(pDX, IDC_SCNBMON1, m_Bmon1);
	DDX_Control(pDX, IDC_SCNBMON2, m_Bmon2);
	DDX_Control(pDX, IDC_SCNCBMN1, m_Cbmn1);
	DDX_Control(pDX, IDC_SCNCBMN2, m_Cbmn2);
	DDX_Control(pDX, IDC_SCNDCBMN1, m_DCbmn1);
	DDX_Control(pDX, IDC_SCNDCBMN2, m_DCbmn2);

	DDX_Control(pDX, IDC_SCNCBR1, m_Cbr1);
	DDX_Control(pDX, IDC_SCNCBR2, m_Cbr2);
	DDX_Control(pDX, IDC_SCNCKJI1, m_Ckji1);
	DDX_Control(pDX, IDC_SCNCKJI2, m_Ckji2);
	DDX_Control(pDX, IDC_SCNDCKJI1, m_DCkji1);
	DDX_Control(pDX, IDC_SCNDCKJI2, m_DCkji2);

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
	DDX_Control(pDX, IDC_SCNTAIKA1, m_Tai1);
	DDX_Control(pDX, IDC_SCNTAIKA2, m_Tai2);
	DDX_Control(pDX, IDC_SCNHUSEN, m_Hcbo);

	DDX_Control(pDX, IDC_SCNDEBT2, m_Debt2);
	DDX_Control(pDX, IDC_SCNCRED2, m_Cred2);
	DDX_Control(pDX, IDC_SCNDBCR2, m_DBcr2);

	DDX_Control(pDX, IDC_SCN_DATESEL, m_selDate);
	DDX_Control(pDX, IDC_SCNHJIYU, m_KSHcombo);

	DDX_Control(pDX, IDC_NCHK_SEQ, m_NotSeq);

	DDX_Control(pDX, IDC_NCHK_DATE, m_NotDate);
	DDX_Control(pDX, IDC_NCHK_DENP, m_NotDenp);

	DDX_Control(pDX, IDC_NCHK_DBMN, m_NotDbmn);
	DDX_Control(pDX, IDC_NCHK_CBMN, m_NotCbmn);
	DDX_Control(pDX, IDC_NCHK_DKJI, m_NotDkji);
	DDX_Control(pDX, IDC_NCHK_CKJI, m_NotCkji);

	DDX_Control(pDX, IDC_NCHK_DBT,	m_NotDbt);
	DDX_Control(pDX, IDC_NCHK_DBRN,	m_NotDbrn);
	DDX_Control(pDX, IDC_NCHK_CRE,	m_NotCre);
	DDX_Control(pDX, IDC_NCHK_CBRN,	m_NotCbrn);
	DDX_Control(pDX, IDC_NCHK_DBCR,	m_NotDBCR);
	DDX_Control(pDX, IDC_NCHK_DCBRN,m_NotDCbrn);

	DDX_Control(pDX, IDC_NCHK_VAL, m_NotVal);
	DDX_Control(pDX, IDC_NCHK_ZEI, m_NotZei);
	DDX_Control(pDX, IDC_NCHK_TAI, m_NotTai);
	DDX_Control(pDX, IDC_NCHK_SYZSW,	m_NotSyzsw);
	DDX_Control(pDX, IDC_NCHK_ZRITU,	m_NotZritu);
	DDX_Control(pDX, IDC_NCHK_SKBN,		m_NotSkbn);
	DDX_Control(pDX, IDC_NCHK_URISIRE,	m_NotUrisire);

	DDX_Control(pDX, IDC_NCHK_SNUMBER,	m_NotSnum);
	DDX_Control(pDX, IDC_NCHK_TGDATE,	m_NotTgdate);
	DDX_Control(pDX, IDC_NCHK_HUSEN,	m_NotHusen);

	DDX_Control(pDX, IDC_NCHK_TKY,	m_NotTky);
	DDX_Control(pDX, IDC_TKY_REPORT, m_Report);

	DDX_Control(pDX, IDC_SCNTOKUS,	m_Tokus);
	DDX_Control(pDX, IDC_NCHK_TOKUS,m_NotTokus);

	DDX_Control(pDX, IDC_NCHK_SNO,	m_NotDocEvi);
	DDX_Control(pDX, IDC_SCNSNO1, m_DocEvi1);
	DDX_Control(pDX, IDC_SCNSNO2, m_DocEvi2);
	DDX_Control(pDX, IDC_NCHK_DCBMN,	m_NotDCBmn);
	DDX_Control(pDX, IDC_NCHK_DCKJI,	m_NotDCKji);

	DDX_Control(pDX, IDC_SCNDOCSEQ1, m_DocSeq1);
	DDX_Control(pDX, IDC_SCNDOCSEQ2, m_DocSeq2);
	DDX_Control(pDX, IDC_NCHK_SEQ2,	m_NotDocSeq);
	DDX_Control(pDX, IDC_SCNSHTTYP, m_DShtCombo);
	DDX_Control(pDX, IDC_SCNSYZKTSN, m_SyzKtsn);
	DDX_Control(pDX, IDC_COMBO_MIKAKUD, m_DTtypeCmb);

	DDX_Control(pDX, IDC_SCNINVNO, m_InvnoEdt);
	DDX_Control(pDX, IDC_SCNINVNODISP, m_InvnoDisp);
	DDX_Control(pDX, IDC_NCHK_INVNO, m_NotInvno );
	DDX_Control(pDX, IDC_NCHK_MENZEI, m_NotMenzei);
	DDX_Control(pDX, IDC_SCNMENWARI, m_MenWariCmb);
}


// DISP のオリジナルサイズ
void CScanKeyView::get_disp2_originalsize( double& w, double& h )
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
	case SCN_DEBT2_PN:
	case SCN_DBR1_PN:
	case SCN_DBR2_PN:
	case SCN_CHKDBR_PN:	now_blk = 9;	break;

	case SCN_CRED_PN:
	case SCN_CRED2_PN:
	case SCN_CBR1_PN:
	case SCN_CBR2_PN:	
	case SCN_CHKCBR_PN:	now_blk = 10;	break;

	case SCN_DBCR_PN:
	case SCN_DBCR2_PN:
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
void CScanKeyView::inp_bkcol( int pn )
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
		img_edit = ( pn == SCN_TKY_PN && is_mdfy_imgtky() );

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

//
// 選択項目背景色セット
//	DB ではこの関数使用していない
//
void CScanKeyView::select_back_col( int pn )
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
					pnt[max++] = SCN_DEBT2_PN;
					pnt[max++] = SCN_DBR1_PN;
					pnt[max++] = SCN_DBR2_PN;
					break;
		case 10:	pnt[max++] = SCN_CRED_PN;
					pnt[max++] = SCN_CRED2_PN;
					pnt[max++] = SCN_CBR1_PN;
					pnt[max++] = SCN_CBR2_PN;
					break;
		case 11:	pnt[max++] = SCN_DBCR_PN;
					pnt[max++] = SCN_DBCR2_PN;
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

//
// コメントの表示
void CScanKeyView::cmntbox( int pn )
{
// DB から表示しない
//#ifdef DB_OLD_CLOSE
	int idc, idc_box = IDC_STATIC_BOX3;

	if( !BRNTKY_sel )
		idc = -1;
	else
	{
		switch( pn )
		{
		case SCN_DBR1_PN:
		case SCN_DBR2_PN://	idc = scGetINP_CTL( SCN_DBR2_PN )->IDC_X; break;
							idc = IDC_CHECK_DBR;		break; //02.26 /03
		case SCN_CBR1_PN:
		case SCN_CBR2_PN://	idc = scGetINP_CTL( SCN_CBR2_PN )->IDC_X; break;
							idc = IDC_CHECK_CBR;	break; //02.26 /03
		case SCN_DCBR1_PN:
		case SCN_DCBR2_PN://idc = scGetINP_CTL( SCN_DCBR2_PN )->IDC_X; break;
							idc = IDC_STATIC_BOX2;	break; //02.26 /03

		default:			idc = -1; break;
		}
	}
//	_cmnt_disp( idc, idc_box );

	CWnd *wnd_idc_box;
	wnd_idc_box = GetDlgItem( idc_box );
	if( idc == -1 )
	{
		wnd_idc_box->ShowWindow( SW_HIDE );
		return;
	}
	else {
		WINDOWPLACEMENT idc_place;
		RECT	newrect;
		CWnd* wnd_idc = GetDlgItem( IDC_SCNDCBR2 );
		wnd_idc->GetWindowPlacement( &idc_place );

		newrect.top = idc_place.rcNormalPosition.top;

		int wd = idc_place.rcNormalPosition.right - idc_place.rcNormalPosition.left;
		newrect.left = idc_place.rcNormalPosition.right + (wd/3);

		wnd_idc_box->SetWindowPos( 0, newrect.left, newrect.top, 0, 0, SWP_SHOWWINDOW|SWP_NOZORDER|SWP_NOSIZE );
	}
//#endif
}

void CScanKeyView::_cmnt_disp( int idc, int idc_box )
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

	newrect.top = idc_place.rcNormalPosition.top;
	newrect.left = idc_place.rcNormalPosition.right;
	
	wnd_idc_box->SetWindowPos( 0, newrect.left, newrect.top, 0, 0, SWP_SHOWWINDOW|SWP_NOZORDER|SWP_NOSIZE );
}

//
// 表題を窪み表示に
void CScanKeyView::selbox( int pn )
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
	case SCN_DEBT2_PN:
	case SCN_CHKDBR_PN:	idc1 = IDC_ST_DEBT; idc2 = -1; break;	//#* 02.26 /03

	case SNOT_DBRN_PN:
	case SCN_DBR1_PN:
	case SCN_DBR2_PN:	idc1 = IDC_ST_DBR; idc2 = IDC_STATIC8; break;

	case SNOT_CRE_PN:
	case SCN_CRED_PN:
	case SCN_CRED2_PN:
	case SCN_CHKCBR_PN:	idc1 = IDC_ST_CRED; idc2 = -1; break;	//#* 02.26 /03

	case SNOT_CBRN_PN:
	case SCN_CBR1_PN:
	case SCN_CBR2_PN:	idc1 = IDC_ST_CBR; idc2 = IDC_STATIC9; break;

	case SNOT_DBCR_PN:
	case SCN_DBCR_PN:
	case SCN_DBCR2_PN:
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
void CScanKeyView::box_disp( int idc1, int idc2 )
{
	_box_disp( idc1, IDC_STATIC_BOX1 );
	_box_disp( idc2, IDC_STATIC_BOX2 );
}

void CScanKeyView::_box_disp( int idc, int idc_box )
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
	wnd2->UpdateWindow();
}

//コントロールの表示順序
void CScanKeyView::ctrldspjun( int *tbl, int n )
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


BEGIN_MESSAGE_MAP(CScanKeyView, CDBbaseView)
	ON_CBN_SELCHANGE(IDC_TKYCOMBO, OnSelchangeTkycombo)
	ON_CBN_SELCHANGE(IDC_SCNSKBN, OnSelchangeScnskbn)
	ON_CBN_SELCHANGE(IDC_SCNZRITU, OnSelchangeScnzritu)
	ON_CBN_SELCHANGE(IDC_SCNSYZSW, OnSelchangeScnsyzsw)
	ON_CBN_SELCHANGE(IDC_SCNURISIRE, OnSelchangeScnurisire)
	ON_CBN_SELCHANGE(IDC_SCNTOKUS, OnSelchangeScnTokus)
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
	ON_CBN_SELCHANGE(IDC_SCNGYOKBN, OnSelchangeScnGyoKbn)
	ON_MESSAGE( WM_RETROFOCUS, RetroFocus )
	ON_BN_CLICKED(IDC_NCHK_SYZSW, &CScanKeyView::OnBnClickedNchkSyzsw)
	ON_COMMAND(ID_BUTTON_F8, &CScanKeyView::OnButtonF8)
	ON_COMMAND(ID_BUTTON_F11, &CScanKeyView::OnButtonF11)
	ON_COMMAND(ID_BUTTON_END, &CScanKeyView::OnButtonEnd)
	ON_BN_CLICKED(IDOK, &CScanKeyView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CScanKeyView::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_SCNHJIYU, &CScanKeyView::OnCbnSelchangeScnhjiyu)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F4, &CScanKeyView::OnUpdateButtonF4)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_DEL, &CScanKeyView::OnBnClickedBtnDel)
	ON_COMMAND(ID_BUTTON_F9, &CScanKeyView::OnButtonF9)
	ON_COMMAND(ID_BUTTON_INSERT, &CScanKeyView::OnButtonInsert)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_INSERT, &CScanKeyView::OnUpdateButtonInsert)
//	ON_COMMAND(ID_BUTTON_F2, &CScanKeyView::OnButtonF2)
ON_COMMAND(ID_BUTTON_F6, &CScanKeyView::OnButtonF6)
ON_NOTIFY(ICS_NM_REPORT_SELCHANGED, IDC_TKY_REPORT, OnNMReportSelchangedTKY_REPORT)
ON_NOTIFY(ICS_NM_REPORT_EDITKEYDOWN, IDC_TKY_REPORT, OnNMReportEditkeydownTKY_REPORT)
ON_NOTIFY(ICS_NM_REPORT_EDITSETFOCUS, IDC_TKY_REPORT, OnNMReportEditsetfocusTKY_REPORT)
ON_NOTIFY(ICS_NM_REPORT_EDITKILLFOCUS, IDC_TKY_REPORT, OnNMReportEditkillfocusTKY_REPORT)
ON_UPDATE_COMMAND_UI(ID_BUTTON_F6, &CScanKeyView::OnUpdateButtonF6)
ON_UPDATE_COMMAND_UI(ID_BUTTON_F9, &CScanKeyView::OnUpdateButtonF9)
ON_COMMAND(ID_BUTTON_F5, &CScanKeyView::OnButtonF5)
ON_UPDATE_COMMAND_UI(ID_BUTTON_F5, &CScanKeyView::OnUpdateButtonF5)

	ON_BN_CLICKED(IDC_CHECK_KMSYZ, OnCheckKMSYZ)
	ON_BN_CLICKED(IDC_NCHK_SNO, &CScanKeyView::OnBnClickedNchkSno)
	ON_CBN_SELCHANGE(IDC_SCNSHTTYP, OnSelchangeScnShtTyp)
	ON_CBN_SELCHANGE(IDC_SCNSYZKTSN, OnSelchangeScnSyzKtsn)
	ON_CBN_SELCHANGE(IDC_COMBO_MIKAKUD, OnSelchangeDataType)

	ON_MESSAGE( WM_SELGUIDEMSG, OnSelGuideMsg)
	ON_MESSAGE(WM_USER_SELCHG_MSG, OnSelChgMsg)
	ON_BN_CLICKED(IDC_CHECK_MENZEI, &CScanKeyView::OnBnClickedCheckMenzei)
	ON_BN_CLICKED(IDC_NCHK_INVNO, &CScanKeyView::OnBnClickedNchkInvno)
	ON_BN_CLICKED(IDC_NCHK_MENZEI, &CScanKeyView::OnBnClickedNchkMenzei)
	ON_CBN_SELCHANGE(IDC_SCNMENWARI, OnSelchangeMenWari)
	ON_BN_CLICKED(IDC_BTN_DUPLI, &CScanKeyView::OnBnClickedBtnDupli)
END_MESSAGE_MAP()


// CScanKeyView 診断

#ifdef _DEBUG
void CScanKeyView::AssertValid() const
{
	CDBbaseView::AssertValid();
}

#ifndef _WIN32_WCE
void CScanKeyView::Dump(CDumpContext& dc) const
{
	CDBbaseView::Dump(dc);
}
#endif
#endif //_DEBUG


// CScanKeyView メッセージ ハンドラ

void CScanKeyView::OnInitialUpdate()
{
	CDBbaseView::SelectDispAdjust();

	CDBbaseView::OnInitialUpdate();
	ResizeParentToFit();

#ifdef OLD_CLOSE
	// 兄弟ウィンドウをクリップ
	int i;
	int clipID[] = {
		IDC_SCN_DATESEL,
		IDC_SCNBMON1, IDC_STATIC4, IDC_SCNBMON2,
		IDC_SCNCBMN1, IDC_STATIC5, IDC_SCNCBMN2,
		IDC_ST_DENP2, IDC_FURI_CHK, IDC_NYUK_CHK, IDC_SYUK_CHK,
		IDC_CHECK_DBM, IDC_CHECK_CBM,
		IDC_ST_DKJ, IDC_NCHK_DKJI, IDC_ST_CKJ, IDC_NCHK_CKJI, 
		IDC_CHECK_DKJ, IDC_CHECK_CKJ,
		IDC_SCNKOJI1, IDC_STATIC6, IDC_SCNKOJI2,
		IDC_SCNCKJI1, IDC_STATIC7, IDC_SCNCKJI2,
		IDC_ST_DBR, IDC_NCHK_DBRN, IDC_SCNDBR1, IDC_STATIC8, IDC_SCNDBR2, IDC_CHECK_DBR,
		IDC_ST_CBR, IDC_NCHK_CBRN, IDC_SCNCBR1, IDC_STATIC9, IDC_SCNCBR2, IDC_CHECK_CBR
	};
	for( i = 0; i < (sizeof clipID/ sizeof clipID[0]); i++ ) {
		GetDlgItem( clipID[i] )->ModifyStyle(0, WS_CLIPSIBLINGS);
	}
#endif

	CWnd*	pwnd	=	GetWindow( GW_CHILD );

	while( pwnd ) {
		pwnd->ModifyStyle(0, WS_CLIPSIBLINGS);
		pwnd	=	pwnd->GetWindow( GW_HWNDNEXT );
	}

	((CICSDisp*)GetDlgItem( IDC_DISPETC ))->SetBackColor( BRTEK_BackCOL );

#ifdef _DBEDT_FRAME_
	m_Bmon1.SetFrame( -1, RGB_DARK_GRAY );
	m_Bmon2.SetFrame( -1, RGB_DARK_GRAY );
	m_Cbmn1.SetFrame( -1, RGB_DARK_GRAY );
	m_Cbmn2.SetFrame( -1, RGB_DARK_GRAY );
	m_Cbr1.SetFrame( -1, RGB_DARK_GRAY );
	m_Cbr2.SetFrame( -1, RGB_DARK_GRAY );
	m_Ckji1.SetFrame( -1, RGB_DARK_GRAY );
	m_Ckji2.SetFrame( -1, RGB_DARK_GRAY );
	m_Cred.SetFrame( -1, RGB_DARK_GRAY );
	m_Dbr1.SetFrame( -1, RGB_DARK_GRAY );
	m_Dbr2.SetFrame( -1, RGB_DARK_GRAY );
	m_Debt.SetFrame( -1, RGB_DARK_GRAY );
	m_DBcr.SetFrame( -1, RGB_DARK_GRAY );
	m_DCbr1.SetFrame( -1, RGB_DARK_GRAY );
	m_DCbr2.SetFrame( -1, RGB_DARK_GRAY );
	m_Denp1.SetFrame( -1, RGB_DARK_GRAY );
	m_Denp2.SetFrame( -1, RGB_DARK_GRAY );
	m_Koji1.SetFrame( -1, RGB_DARK_GRAY );
	m_Koji2.SetFrame( -1, RGB_DARK_GRAY );
	m_Seq1.SetFrame( -1, RGB_DARK_GRAY );
	m_Seq2.SetFrame( -1, RGB_DARK_GRAY );
	m_Snum1.SetFrame( -1, RGB_DARK_GRAY );
	m_Snum2.SetFrame( -1, RGB_DARK_GRAY );
	m_Tdat1.SetFrame( -1, RGB_DARK_GRAY );
	m_Tdat2.SetFrame( -1, RGB_DARK_GRAY );
	m_Tky.SetFrame( -1, RGB_DARK_GRAY );
	m_Val1.SetFrame( -1, RGB_DARK_GRAY );
	m_Val2.SetFrame( -1, RGB_DARK_GRAY );
	m_Zei1.SetFrame( -1, RGB_DARK_GRAY );
	m_Zei2.SetFrame( -1, RGB_DARK_GRAY );
	m_Tai1.SetFrame( -1, RGB_DARK_GRAY );
	m_Tai2.SetFrame( -1, RGB_DARK_GRAY );
#endif

	m_Bmon1.EnableDelete(0);	m_Bmon2.EnableDelete(0);
	m_Cbmn1.EnableDelete(0);	m_Cbmn2.EnableDelete(0);
	m_Koji1.EnableDelete(0);	m_Koji2.EnableDelete(0);
	m_Ckji1.EnableDelete(0);	m_Ckji2.EnableDelete(0);
	m_DCbmn1.EnableDelete(0);	m_DCbmn2.EnableDelete(0);
	m_DCkji1.EnableDelete(0);	m_DCkji2.EnableDelete(0);

	m_Cred.EnableDelete(0);		m_Cred2.EnableDelete(0);
	m_Cbr1.EnableDelete(0);		m_Cbr2.EnableDelete(0);
	m_Debt.EnableDelete(0);		m_Debt2.EnableDelete(0);
	m_Dbr1.EnableDelete(0);		m_Dbr2.EnableDelete(0);
	m_DBcr.EnableDelete(0);		m_DBcr2.EnableDelete(0);
	m_DCbr1.EnableDelete(0);	m_DCbr2.EnableDelete(0);
	m_Denp1.EnableDelete(0);	m_Denp2.EnableDelete(0);
	m_Seq1.EnableDelete(0);		m_Seq2.EnableDelete(0);
	m_Snum1.EnableDelete(0);	m_Snum2.EnableDelete(0);
	m_Tdat1.EnableDelete(0);	m_Tdat2.EnableDelete(0);
	m_Tky.EnableDelete(0); 
	m_Tky.EnableHome(1); 
	m_Val1.EnableDelete(0);		m_Val2.EnableDelete(0);
	m_Zei1.EnableDelete(0);		m_Zei2.EnableDelete(0);
	m_Tai1.EnableDelete(0);		m_Tai2.EnableDelete(0);

	m_DocSeq1.EnableDelete(0);	m_DocSeq2.EnableDelete(0);

	// インボイス対応
	m_InvnoEdt.ImeEndMode(TRUE);
	m_InvnoEdt.EnableDelete( 0 );
	m_InvnoEdt.EnableHome(1);
	m_InvnoEdt.SetKanaTerm(TRUE, 20, ICSDBEDT_KTYPE_KANA);

	InitTkyReport();
}



// 消費税 常に同じ設定の表示
void CScanKeyView::def_syselinz()
{
	struct _ZMSGBIND* zmb;

	zmb = &ZMSGBND[SWKKB_PG];
	int i, idx;
	m_Syzsw.ResetContent();
	idx = 0;

	for( i = 0; i < zmb->n_zmsg; i++ ) {
		if( Voln1->ee_ymd < RVCHAR_YMD ) {
			if( zmb->zmsg[i].zeifnc >= RVCHARGE ) {
				continue;
			}
		}
		if( !IsMasterType( MST_SYZ8_10 | MST_SYZ10 ) ) {
			if( zmb->zmsg[i].zeifnc >= KOJI_KANFURI ) {
				continue;
			}
		}
		else {
			if( !M_MISEIKOJI_TYPE ) {
				if( zmb->zmsg[i].zeifnc == KOJI_KANFURI ) {
					continue;
				}
			}
		}
		//仮受仮払の仕訳区分
		if( !(Voln1->s_sgn9 & 0x01) ) {
			switch( zmb->zmsg[i].zeifnc ) {
			case SI_SYUTOK:
			case SI_JOTO:
			case URI_HEN:
			case SIIRE_HEN:
				continue;
			}
		}
		m_Syzsw.AddString( zmb->zmsg[i].zeitem );
		m_Syzsw.SetItemData( idx++, zmb->zmsg[i].zeifnc );
	}

	zmb = &ZMSGBND[ZRITU_PG];
	m_Zritu.ResetContent();
	idx = 0;

	for( i = 0; i < zmb->n_zmsg; i++ ) {
		if( zmb->zmsg[i].zeifnc == ZR_8 ) {
			if( ! IsMasterType( MST_SYZ5_8|MST_SYZ8 ) )
				continue;
		}
		if( zmb->zmsg[i].zeifnc == ZR_10 || zmb->zmsg[i].zeifnc == ZR_8K ) {
			if( ! IsMasterType( MST_SYZ8_10|MST_SYZ10 ) )
				continue;
		}

		m_Zritu.AddString( zmb->zmsg[i].zeitem );
		m_Zritu.SetItemData( idx, zmb->zmsg[i].zeifnc );
		idx++;
	}

	zmb = &ZMSGBND[SKBN_PG];
	m_Skbn.ResetContent();
	for( i = 0; i < zmb->n_zmsg; i++ ) {
		m_Skbn.AddString( zmb->zmsg[i].zeitem );
		m_Skbn.SetItemData( i, zmb->zmsg[i].zeifnc );
	}
}


// 消費税選択関係イニシャライズ
void CScanKeyView::set_syselinz()
{
	struct _ZMSGBIND* zmb;
	int i, tp;

	if( pDBsy->IsSyohizeiMaster() )
	{
		if( pDBsy->IsKaniMaster() )
			tp = KANI_SYMST;
		else if( pDBsy->IsKobetsuMaster() )
			tp = KOBET_SYMST;
		else
			tp = NORMAL_SYMST;
	}
	else
		tp = NORMAL_MST;

	m_Urisire.ResetContent();
	switch( tp ) {
	case KANI_SYMST:
		GetDlgItem(IDC_ST_URISIRE)->SetWindowText( "売上区分:" );
		GetDlgItem(IDC_ST_URISIRE)->EnableWindow(TRUE);
		zmb = &ZMSGBND[GYOSYU_PG];

		for( i = 0; i < zmb->n_zmsg; i++ ) {
			if( (i+1) == zmb->n_zmsg ) { //第六種対応
				if( Voln1->ee_ymd < KANI6_YMD ) {
					break;
				}
			}
			m_Urisire.AddString( zmb->zmsg[i].zeitem );
			m_Urisire.SetItemData( i, zmb->zmsg[i].zeifnc );
		}
		m_Urisire.EnableWindow(TRUE);
		m_Urisire.SetCurSel(0);
		m_NotUrisire.EnableWindow( TRUE );
		scGetINP_CTL( SNOT_URISIRE_PN )->INP_ena = TRUE;
		scGetINP_CTL( SCN_URISIRE_PN )->INP_ena = TRUE;
		break;
	case KOBET_SYMST:
		GetDlgItem(IDC_ST_URISIRE)->SetWindowText( "仕入区分:" );
		GetDlgItem(IDC_ST_URISIRE)->EnableWindow(TRUE);
		zmb = &ZMSGBND[SIRE_PG];
		for( i = 0; i < zmb->n_zmsg; i++ ) {
			m_Urisire.AddString( zmb->zmsg[i].zeitem );
			m_Urisire.SetItemData( i, zmb->zmsg[i].zeifnc );
		}
#ifndef KOBETSU_SEQ_RET
		// 03.05 /12
		m_Urisire.AddString( "基本設定" );
		m_Urisire.SetItemData( i++, URI_KIHON );
		m_Urisire.AddString( "仕入区分対象" );
		m_Urisire.SetItemData( i, URI_TAISYO );
#endif
		m_Urisire.EnableWindow(TRUE);
		m_Urisire.SetCurSel(0);
		m_NotUrisire.EnableWindow( TRUE );
		scGetINP_CTL( SNOT_URISIRE_PN )->INP_ena = TRUE;
		scGetINP_CTL( SCN_URISIRE_PN )->INP_ena = TRUE;
		break;
	case NORMAL_MST:
	case NORMAL_SYMST:
	default:
		GetDlgItem(IDC_ST_URISIRE)->SetWindowText( "－－－－:" );
		GetDlgItem(IDC_ST_URISIRE)->EnableWindow(FALSE);
		m_Urisire.EnableWindow(FALSE);
		scGetINP_CTL( SNOT_URISIRE_PN )->INP_ena = FALSE;
		scGetINP_CTL( SCN_URISIRE_PN )->INP_ena = FALSE;
		m_Urisire.EnableWindow(FALSE);
		m_Urisire.SetCurSel(-1);
		m_NotUrisire.EnableWindow( FALSE );
		break;
	}

	// 特定収入
	m_Tokus.ResetContent();
	if( pDBzm->zvol->apno >= 0x50 ) {

		if( tp == KOBET_SYMST )	zmb = &ZMSGBND[KBTOKUS_PG];
		else					zmb = &ZMSGBND[TOKUS_PG];
		for( i = 0; i < zmb->n_zmsg; i++ ) {
			if( ! IsMasterType(MST_SYZ5_8|MST_SYZ8) ) {
				if( zmb->zmsg[i].zeifnc == TK_SYNYU8 || zmb->zmsg[i].zeifnc == TK_FUTOK8 || 
					zmb->zmsg[i].zeifnc == TK_KATOK8 ||	zmb->zmsg[i].zeifnc == TK_HKATK8 ||
					zmb->zmsg[i].zeifnc == TK_KYOTK8 ) {
					continue;
				}
			}
			if( ! IsMasterType(MST_SYZ8_10|MST_SYZ10) ) {
				if( zmb->zmsg[i].zeifnc >= TK_SYNYU10 && zmb->zmsg[i].zeifnc <= TK_FUTOK8K) {
					continue;
				}
			}
			m_Tokus.AddString( zmb->zmsg[i].zeitem );
			m_Tokus.SetItemData( i, zmb->zmsg[i].zeifnc );
		}

		GetDlgItem(IDC_ST_TOKUS)->ShowWindow( SW_SHOW );
		GetDlgItem(IDC_ST_TOKUS)->EnableWindow(TRUE);
		m_NotTokus.ShowWindow( SW_SHOW );
		m_NotTokus.ShowWindow( SW_SHOW );

		m_Tokus.SetCurSel(0);

		DWORD itd = SCAN_KEY.SCN_SKBN_DT.SKBN_SGN1;/*m_Skbn.GetItemData( sel );*/
	//	SetTokusWnd( itd == SK_HUKA ? TRUE : FALSE );
		SetTokusWnd( (tp == KANI_SYMST) ? FALSE : TRUE );
	}
	else {
		GetDlgItem(IDC_ST_TOKUS)->ShowWindow( SW_HIDE );

		m_Tokus.ShowWindow( SW_HIDE );
		m_NotTokus.ShowWindow( SW_HIDE );
		m_Tokus.SetCurSel(-1);
		SetTokusWnd( FALSE );
	}

	i = 0;
	m_SyzKtsn.ResetContent();
	m_SyzKtsn.AddString( "なし" );
	m_SyzKtsn.SetItemData( i++, NON_SELECT );
	m_SyzKtsn.AddString( "取得" );
	m_SyzKtsn.SetItemData( i++, KTS_SYUTOK );
	m_SyzKtsn.AddString( "譲渡" );
	m_SyzKtsn.SetItemData( i++, KTS_JOTO );
}


void CScanKeyView::SetTokusWnd( BOOL bEnable )
{
	GetDlgItem(IDC_ST_TOKUS)->EnableWindow( bEnable );
	scGetINP_CTL( SNOT_TOKUS_PN )->INP_ena = bEnable;
	scGetINP_CTL( SCN_TOKUS_PN )->INP_ena = bEnable;
	m_Tokus.EnableWindow(bEnable);
	m_NotTokus.EnableWindow( bEnable );
}

/*=======================
	FIX 8科目選択
	科目・摘要32項目選択
		本体
========================*/
void CScanKeyView::SelectedIcsselctrl(short position, long Number, int place ) 
{
	int pn;
	CDBipTKREC tkrec;
	CDBINPDataRec tmpdt;	// 部門コード取得用
	struct _ItemData dmy_data = {0};
	int dcsw;
	struct _ItemData check={0}, ans={0}, *pKey;
	char temp[64];
#ifdef _ICSPEN_	// 01.20 /01
	CWnd *nextkey = 0;
#endif

	pn = get_nowpn();

	if( pn == SCN_DEBT_PN || pn == SCN_CRED_PN || pn == SCN_DBCR_PN ||
		pn == SCN_DEBT2_PN || pn == SCN_CRED2_PN || pn == SCN_DBCR2_PN )
	{
		switch( pn )
		{
		case SCN_DEBT_PN:
			dcsw = 0;
			check = SCAN_KEY.SCN_DEBT_DT;
			pKey = &SCAN_KEY.SCN_DEBT_DT;
			break;
		case SCN_DEBT2_PN:
			dcsw = 0;
			check = SCAN_KEY.SCN_DEBT2_DT;
			pKey = &SCAN_KEY.SCN_DEBT2_DT;
			break;

		case SCN_CRED_PN:
			dcsw = 1;
			check = SCAN_KEY.SCN_CRED_DT;
			pKey = &SCAN_KEY.SCN_CRED_DT;
			break;
		case SCN_CRED2_PN:
			dcsw = 1;
			check = SCAN_KEY.SCN_CRED2_DT;
			pKey = &SCAN_KEY.SCN_CRED2_DT;
			break;

		case SCN_DBCR_PN:
			dcsw = 0;
			check = SCAN_KEY.SCN_DBCR_DT;
			pKey = &SCAN_KEY.SCN_DBCR_DT;
			break;
		case SCN_DBCR2_PN:
			dcsw = 0;
			check = SCAN_KEY.SCN_DBCR2_DT;
			pKey = &SCAN_KEY.SCN_DBCR2_DT;
			break;
		}
	//	check.IP_CHKCdNum = (int)Number;
	//	check.IP_CHKCdNumSg = 1;
		// 科目ｺｰﾄﾞ or 番号 02.20/02
		if( bKamokuCode ) {
			sprintf_s( check.IP_stKMK.kd_code, sizeof check.IP_stKMK.kd_code, "%d", Number );
			check.IP_stKMK.kd_sgn = 1;
			check.IP_BRN = -1;
		}
		else {
			sprintf_s( check.IP_stKMK.kd_code, sizeof check.IP_stKMK.kd_code, "%d", Number );	// 番号
			check.IP_stKMK.kd_sgn = 1;
			check.IP_BRN = -1;
		}

		if( InputItemCheck( ans.IP_stKMK, check.IP_stKMK, dcsw, TRUE ) == TRUE )
		{
			if( KanaSearchType() ) {
				//複数カナ
				HWND	hWnd;
				HIMC	hImc;
				hWnd = GetKamokuCtrlHWND();
				if( hWnd != NULL ) {
					hImc = ImmGetContext(hWnd);
					ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
					ImmReleaseContext(hWnd, hImc);
				}
			}

			DBkamokudata_set( this, scGetINP_CTL(pn)->IDC_X, &ans.IP_stKMK, pDBzm );
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
	else if( pn == SCN_TKY_PN && place )
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
			 pn == SCN_CBMN1_PN || pn == SCN_CBMN2_PN ||
			 pn == SCN_DCBMN1_PN || pn == SCN_DCBMN2_PN ) {
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
		case SCN_DCBMN1_PN:
			pKey = &SCAN_KEY.SCN_DCBMN1_DT;
			break;
		case SCN_DCBMN2_PN:
			pKey = &SCAN_KEY.SCN_DCBMN2_DT;
			break;
		}

		if( bmncode_todata( &tmpdt, Number, 0 ) != FALSE ) {
			pKey->IP_BMN = tmpdt.m_dbmn;
			bmon_select_dsp( pn, pKey, TRUE );
		}
	}
	else if( pn == SCN_KOJI1_PN || pn == SCN_KOJI2_PN || 
			 pn == SCN_CKJI1_PN || pn == SCN_CKJI2_PN ||
			 pn == SCN_DCKJI1_PN || pn == SCN_DCKJI2_PN ) {
		// 工事選択処理
		switch( pn ) {
		case SCN_KOJI1_PN:
			pKey = &SCAN_KEY.SCN_KOJI1_DT;
			break;
		case SCN_KOJI2_PN:
			pKey = &SCAN_KEY.SCN_KOJI2_DT;
			break;
		case SCN_CKJI1_PN:
			pKey = &SCAN_KEY.SCN_CKJI1_DT;
			break;
		case SCN_CKJI2_PN:
			pKey = &SCAN_KEY.SCN_CKJI2_DT;
			break;
		case SCN_DCKJI1_PN:
			pKey = &SCAN_KEY.SCN_DCKJI1_DT;
			break;
		case SCN_DCKJI2_PN:
			pKey = &SCAN_KEY.SCN_DCKJI2_DT;
			break;
		}
		CString kno;

		if( kojicode_todata( &tmpdt, NULL, Number, 0 ) != FALSE ) {
			tmpdt.m_dkno.TrimRight();

			strcpy_s( (char*)pKey->IP_KOJI, sizeof pKey->IP_KOJI, tmpdt.m_dkno );
			koji_select_dsp( pn, pKey, TRUE );
		}
	}
	else if( pn == SCN_INVNO_PN && place )
	{
		//登録番号選択
		sprintf_s(temp, sizeof temp, "%d", Number);
		SelResult srt;
		srt.sel_pn = SL_INVNO_PN;
		if( m_Seldata.GetSelResult(&srt, temp) ) {
			pKey = &SCAN_KEY.SCN_INVNO_DT;
			sprintf_s(pKey->_invno, sizeof(pKey->_invno), _T("%s"), srt.selname);
			VARIANT var;
			var.pbVal = (BYTE*)pKey->_invno;
			DBdata_set(this, scGetINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
			
			sprintf_s(temp, sizeof(temp), _T("T%s"), srt.selname);
			DBSetDispString(this, IDC_SCNINVNODISP/*scGetINP_CTL(pn)->IDC_X*/, temp);
			SCAN_KEY.LINE_CND[pn].INP_sg = TRUE;
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
void CScanKeyView::bmon_select_dsp( int pn, struct _ItemData *keybuf, BOOL bTerm )
{
	struct _ItemData check={0};
	char	tmp[30] = {0};
	VARIANT var;

	check.IP_BMN = keybuf->IP_BMN;

	pDBzm->BumonCodeToStr( tmp, sizeof tmp, check.IP_BMN );

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


// 工事を表示
void CScanKeyView::koji_select_dsp( int pn, struct _ItemData *keybuf, BOOL bTerm )
{
	struct _ItemData check={0};
	char	tmp[128] = {0};
	VARIANT var;

	memcpy( check.IP_KOJI, keybuf->IP_KOJI, sizeof check.IP_KOJI );
	pDBzm->KojiCodeToStr( tmp, sizeof tmp, CString(check.IP_KOJI) );

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
			KojiInp( pn, keybuf, 0, 0, 0 );
		}
#else
		KojiInp( pn, keybuf, 0, 0, 0 );
#endif
	}	
}


/**************************************************
	keyline_dataset( SCAN_ITEMEX* pSI )
	入力ラインバッファ にデータセット
***************************************************/
void CScanKeyView::keyline_dataset( SCAN_ITEMEX* pSI )
{
	int i;

	for( i = SCN_ORDER_PN ; i <= SCN_DTTYPE_PN ; ++i ) {
		//コンディションサイン OFF
		memset( (char *)&SCAN_KEY.LINE_CND[i], (char)0, sizeof(SCAN_KEY.LINE_CND[i]) );
	}

	for( i = SCN_ORDER_PN ; i <= SCN_DTTYPE_PN ; ++i )
	{

		switch( i )
		{
		case SCN_ORDER_PN:
			memset( (char *)SCAN_KEY.SCN_ORDER_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_ORDER_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_ORDER_DT.SKBN_SGN1 = 0;
			break;
		case SCN_DUSER_PN:
			memset( (char *)SCAN_KEY.SCN_DUSER_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_DUSER_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_DUSER_DT.SKBN_SGN1 = pDBzm->data_authority() > 0 ? 0 : 1;
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

		case SCN_DCBMN1_PN:
			SCAN_KEY.SCN_DCBMN1_DT.IP_BMN = pSI->sc_dcbmn1;
			SCAN_KEY.SCN_DCBMN2_DT.IP_BMN = pSI->sc_dcbmn2;

			if( pSI->sc_dcbmn1 >= 0 )	SCAN_KEY.LINE_CND[SCN_DCBMN1_PN].INP_sg = 1;
			if( pSI->sc_dcbmn2 >= 0 )	SCAN_KEY.LINE_CND[SCN_DCBMN2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_DCBMN1_PN].KEY_inp = (char)pSI->sc_dcbmnopt;
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

		case SCN_DCKJI1_PN:
			::ZeroMemory( &SCAN_KEY.SCN_DCKJI1_DT.IP_KOJI, sizeof SCAN_KEY.SCN_DCKJI1_DT.IP_KOJI );
			::ZeroMemory( &SCAN_KEY.SCN_DCKJI2_DT.IP_KOJI, sizeof SCAN_KEY.SCN_DCKJI2_DT.IP_KOJI );
			memcpy( &SCAN_KEY.SCN_DCKJI1_DT.IP_KOJI, pSI->sc_dckji1, sizeof pSI->sc_dckji1 );
			memcpy( &SCAN_KEY.SCN_DCKJI2_DT.IP_KOJI, pSI->sc_dckji2, sizeof pSI->sc_dckji2 );

			if( pSI->sc_dckji1[0] )	SCAN_KEY.LINE_CND[SCN_DCKJI1_PN].INP_sg = 1;
			if( pSI->sc_dckji2[0] )	SCAN_KEY.LINE_CND[SCN_DCKJI2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_DCKJI1_PN].KEY_inp = (char)pSI->sc_dckjiopt;
			break;

		case SCN_DEBT_PN:
			memset( &SCAN_KEY.SCN_DEBT_DT, '\0', sizeof SCAN_KEY.SCN_DEBT_DT );
			memcpy( SCAN_KEY.SCN_DEBT_DT.IP_CdNum, pSI->sc_dbt, sizeof pSI->sc_dbt );
			SCAN_KEY.SCN_DEBT_DT.IP_BRN = -1;

			if( pSI->sc_dbt[0] )	SCAN_KEY.LINE_CND[SCN_DEBT_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_DEBT_PN].KEY_inp = (char)pSI->sc_dbtopt;
			break;
		case SCN_DEBT2_PN:
			memset( &SCAN_KEY.SCN_DEBT2_DT, '\0', sizeof SCAN_KEY.SCN_DEBT2_DT );
			memcpy( SCAN_KEY.SCN_DEBT2_DT.IP_CdNum, pSI->sc_dbt2, sizeof pSI->sc_dbt2 );
			SCAN_KEY.SCN_DEBT2_DT.IP_BRN = -1;

			if( pSI->sc_dbt2[0] )	SCAN_KEY.LINE_CND[SCN_DEBT2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_DEBT2_PN].KEY_inp = (char)pSI->sc_dbtopt;
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
		case SCN_CRED2_PN:
			memset( &SCAN_KEY.SCN_CRED2_DT, '\0', sizeof SCAN_KEY.SCN_CRED2_DT );
			memcpy( SCAN_KEY.SCN_CRED2_DT.IP_CdNum, pSI->sc_cre2, sizeof pSI->sc_cre2 );
			SCAN_KEY.SCN_CRED2_DT.IP_BRN = -1;
			if( pSI->sc_cre2[0] )	SCAN_KEY.LINE_CND[SCN_CRED2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_CRED2_PN].KEY_inp = (char)pSI->sc_creopt;
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
		case SCN_DBCR2_PN:
			memset( &SCAN_KEY.SCN_DBCR2_DT, '\0', sizeof SCAN_KEY.SCN_DBCR2_DT );
			memcpy( SCAN_KEY.SCN_DBCR2_DT.IP_CdNum, pSI->sc_dbcr2, sizeof pSI->sc_dbcr2 );
			SCAN_KEY.SCN_DBCR2_DT.IP_BRN = -1;

			if( pSI->sc_dbcr2[0] )	SCAN_KEY.LINE_CND[SCN_DBCR2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_DBCR2_PN].KEY_inp = (char)pSI->sc_dbcropt;
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
			if( pSI->sc_syzsw > NON_SELECT ) SCAN_KEY.LINE_CND[SCN_SYZSW_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_SYZSW_PN].KEY_inp = (char)pSI->sc_syzswopt;
			break;
		case SCN_ZRITU_PN:
			memset( (char *)SCAN_KEY.SCN_ZRITU_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_ZRITU_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_ZRITU_DT.SKBN_SGN1 = pSI->sc_zritu;
			if( pSI->sc_zritu > NON_SELECT ) SCAN_KEY.LINE_CND[SCN_ZRITU_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_ZRITU_PN].KEY_inp = (char)pSI->sc_zrituopt;
			break;
		case SCN_SKBN_PN:
			memset( (char *)SCAN_KEY.SCN_SKBN_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_SKBN_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_SKBN_DT.SKBN_SGN1 = pSI->sc_skbn;
			if( pSI->sc_skbn > NON_SELECT ) SCAN_KEY.LINE_CND[SCN_SKBN_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_SKBN_PN].KEY_inp = (char)pSI->sc_skbnopt;
			break;
		case SCN_URISIRE_PN:
			memset( (char *)SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_URISIRE_DT.SKBN_SGN1 = pSI->sc_urisire;
			if( pSI->sc_urisire > NON_SELECT ) SCAN_KEY.LINE_CND[SCN_URISIRE_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_URISIRE_PN].KEY_inp = (char)pSI->sc_urisireopt;
			break;
		// 特定収入[06.03 /08]
		case SCN_TOKUS_PN:
			memset( (char *)SCAN_KEY.SCN_TOKUS_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_TOKUS_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_TOKUS_DT.SKBN_SGN1 = pSI->sc_tokus;
			if( pSI->sc_tokus > NON_SELECT ) SCAN_KEY.LINE_CND[SCN_TOKUS_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_TOKUS_PN].KEY_inp = (char)pSI->sc_tokusopt;
			break;

		case SCN_TKY_PN:
			memcpy( SCAN_KEY.SCN_TKY_DT.IP_TKY, pSI->sc_tky, sizeof pSI->sc_tky );
			if( pSI->sc_tky[0] ) SCAN_KEY.LINE_CND[SCN_TKY_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_TKY_PN].KEY_inp = (char)pSI->sc_tkyopt;
			break;
		case SCN_SNUM1_PN:
			memset( SCAN_KEY.SCN_SNUM1_DT.IP_SNUM, 0, sizeof SCAN_KEY.SCN_SNUM1_DT.IP_SNUM );
			memset( SCAN_KEY.SCN_SNUM2_DT.IP_SNUM, 0, sizeof SCAN_KEY.SCN_SNUM2_DT.IP_SNUM );			
			memcpy( SCAN_KEY.SCN_SNUM1_DT.IP_SNUM, pSI->sc_rnum1, sizeof pSI->sc_rnum1 );
			memcpy( SCAN_KEY.SCN_SNUM2_DT.IP_SNUM, pSI->sc_rnum2, sizeof pSI->sc_rnum2 );

			if( pSI->sc_rnum1[0] ) SCAN_KEY.LINE_CND[SCN_SNUM1_PN].INP_sg = 1;
			if( pSI->sc_rnum2[0] ) SCAN_KEY.LINE_CND[SCN_SNUM2_PN].INP_sg = 1;
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

			if( pSI->sc_husen > NON_SELECT ) SCAN_KEY.LINE_CND[SCN_HUSEN_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_HUSEN_PN].KEY_inp = (char)pSI->sc_husenopt;
			break;

		case SCN_NOINP_PN:
			memset( (char *)SCAN_KEY.SCN_NOINP_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_NOINP_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_NOINP_DT.SKBN_SGN1 = pSI->sc_noinp;

			if( pSI->sc_noinp > 0 ) SCAN_KEY.LINE_CND[SCN_NOINP_PN].INP_sg = 1;
			break;

		case SCN_GYOKBN_PN:	// 業種の区分
			memset( (char *)SCAN_KEY.SCN_GYOKBN_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_GYOKBN_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_GYOKBN_DT.SKBN_SGN1 = pSI->sc_gyokbn;

			if( pSI->sc_gyokbn > 0 ) SCAN_KEY.LINE_CND[SCN_GYOKBN_PN].INP_sg = 1;
			break;

		case SCN_HJIYU_PN:
			memset( (char *)SCAN_KEY.SCN_HJIYU_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_HJIYU_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_HJIYU_DT.SKBN_SGN1 = pSI->sc_hjiyu;

			if( pSI->sc_hjiyu > 0 ) SCAN_KEY.LINE_CND[SCN_HJIYU_PN].INP_sg = 1;
			break;

		case SCN_DOCEVI1_PN:
			memset( SCAN_KEY.SCN_DOCEVI1_DT.IP_SNUM, 0, sizeof SCAN_KEY.SCN_DOCEVI1_DT.IP_SNUM );
			memset( SCAN_KEY.SCN_DOCEVI2_DT.IP_SNUM, 0, sizeof SCAN_KEY.SCN_DOCEVI2_DT.IP_SNUM );
			memcpy( SCAN_KEY.SCN_DOCEVI1_DT.IP_SNUM, pSI->sc_docev1, sizeof pSI->sc_docev1 );
			memcpy( SCAN_KEY.SCN_DOCEVI2_DT.IP_SNUM, pSI->sc_docev2, sizeof pSI->sc_docev2 );

			if( pSI->sc_docev1[0] ) SCAN_KEY.LINE_CND[SCN_DOCEVI1_PN].INP_sg = 1;
			if( pSI->sc_docev2[0] ) SCAN_KEY.LINE_CND[SCN_DOCEVI2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_DOCEVI2_PN].KEY_inp = (char)pSI->sc_docevopt;
			break;


		case SCN_DOCSEQ1_PN:
			SCAN_KEY.SCN_DOCSEQ1_DT.IP_SEQ = pSI->sc_docseq1;
			SCAN_KEY.SCN_DOCSEQ2_DT.IP_SEQ = pSI->sc_docseq2;

			if( pSI->sc_docseq1 > 0 )	SCAN_KEY.LINE_CND[SCN_DOCSEQ1_PN].INP_sg = 1;
			if( pSI->sc_docseq2 > 0 )	SCAN_KEY.LINE_CND[SCN_DOCSEQ2_PN].INP_sg = 1;
			SCAN_KEY.LINE_CND[SCN_DOCSEQ1_PN].KEY_inp = (char)pSI->sc_docseqopt;

			memset( (char *)SCAN_KEY.SCN_DOCSHTP_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_DOCSHTP_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_DOCSHTP_DT.SKBN_SGN1 = pSI->sc_docshtyp;

			if( pSI->sc_docshtyp > 0 ) SCAN_KEY.LINE_CND[SCN_DOCSHTP_PN].INP_sg = 1;
			break;

		case SCN_SYZKTSN_PN:
			memset( (char *)SCAN_KEY.SCN_SYZKTSN_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_SYZKTSN_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_SYZKTSN_DT.SKBN_SGN1 = pSI->sc_syzktsn;
			if( pSI->sc_syzktsn > 0 ) SCAN_KEY.LINE_CND[SCN_SYZKTSN_PN].INP_sg = 1;
			break;

		case SCN_DTTYPE_PN:
			memset( (char *)SCAN_KEY.SCN_DTTYPE_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_DTTYPE_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_DTTYPE_DT.SKBN_SGN1 = pSI->sc_mikad;
			if( pSI->sc_mikad > 0 ) SCAN_KEY.LINE_CND[SCN_DTTYPE_PN].INP_sg = 1;
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
void CScanKeyView::keyline_inz( BOOL bDisp/*=TRUE*/)
{
	int i;

	// スキャンオンリー時
	if( bSCAN_ONLY_MODE )
		GetDlgItem( IDCANCEL )->SetWindowText( "終了(End)" );

TRACE( "@F11 keyline_inz 01\n" );

	// インボイス対応
	for( i = SCN_ORDER_PN ; i <= SCN_DTTYPE_PN ; ++i )
	{
		switch( i )
		{
		case SCN_ORDER_PN:
			memset( (char *)SCAN_KEY.SCN_ORDER_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_ORDER_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_ORDER_DT.SKBN_SGN1 = 0;
			break;
		case SCN_DUSER_PN:
			memset( (char *)SCAN_KEY.SCN_DUSER_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_DUSER_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_DUSER_DT.SKBN_SGN1 = pDBzm->data_authority() > 0 ? 0 : 1;
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
		case SCN_DCBMN1_PN:
			SCAN_KEY.SCN_DCBMN1_DT.IP_BMN = SCAN_KEY.SCN_DCBMN2_DT.IP_BMN = -1;
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

		case SCN_DCKJI1_PN:
			::ZeroMemory( &SCAN_KEY.SCN_DCKJI1_DT.IP_KOJI, sizeof SCAN_KEY.SCN_DCKJI1_DT.IP_KOJI );
			::ZeroMemory( &SCAN_KEY.SCN_DCKJI2_DT.IP_KOJI, sizeof SCAN_KEY.SCN_DCKJI2_DT.IP_KOJI );
			break;

		case SCN_DEBT_PN:
			memset( &SCAN_KEY.SCN_DEBT_DT, '\0', sizeof SCAN_KEY.SCN_DEBT_DT );
			memset( &SCAN_KEY.SCN_DEBT2_DT, '\0', sizeof SCAN_KEY.SCN_DEBT2_DT );
			break;
		case SCN_DBR1_PN:
			SCAN_KEY.SCN_DBR1_DT.IP_cBRN = SCAN_KEY.SCN_DBR2_DT.IP_cBRN = -1;
			((CButton*)GetDlgItem(IDC_CHECK_DBR))->SetCheck(0);
			SendMessage(WM_COMMAND, MAKELONG(IDC_CHECK_DBR, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_DBR)->m_hWnd);
			break;
		case SCN_CRED_PN:
			memset( &SCAN_KEY.SCN_CRED_DT, '\0', sizeof SCAN_KEY.SCN_CRED_DT );
			memset( &SCAN_KEY.SCN_CRED2_DT, '\0', sizeof SCAN_KEY.SCN_CRED2_DT );
			break;
		case SCN_CBR1_PN:
			SCAN_KEY.SCN_CBR1_DT.IP_cBRN = SCAN_KEY.SCN_CBR2_DT.IP_cBRN = -1;
			//#* 02.26 /03
			((CButton*)GetDlgItem(IDC_CHECK_CBR))->SetCheck(0);
			SendMessage(WM_COMMAND, MAKELONG(IDC_CHECK_CBR, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_CBR)->m_hWnd);
			break;
		case SCN_DBCR_PN:
			memset( &SCAN_KEY.SCN_DBCR_DT, '\0', sizeof SCAN_KEY.SCN_DBCR_DT );
			memset( &SCAN_KEY.SCN_DBCR2_DT, '\0', sizeof SCAN_KEY.SCN_DBCR2_DT );
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
		case SCN_TAI1_PN:
			memset( (char *)&SCAN_KEY.SCN_TAI1_DT, (char)0, sizeof(SCAN_KEY.SCN_TAI1_DT) );
			memset( (char *)&SCAN_KEY.SCN_TAI2_DT, (char)0, sizeof(SCAN_KEY.SCN_TAI2_DT) );
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
		// 特定収入
		case SCN_TOKUS_PN:
			memset( (char *)SCAN_KEY.SCN_TOKUS_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_TOKUS_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_TOKUS_DT.SKBN_SGN1 = NON_SELECT;
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
		case SCN_HUCMT_PN:
			SCAN_KEY.SCN_HUCMT_DT.IP_HUSEN = 0;
			memset( (char *)SCAN_KEY.SCN_HUCMT_DT.IP_HUCMT, 0, sizeof( SCAN_KEY.SCN_HUCMT_DT.IP_HUCMT ) );
			SCAN_KEY.SCN_HUCMT_DT.SKBN_SGN1 = 0;
			break;
		case SCN_NOINP_PN:
			memset( (char *)SCAN_KEY.SCN_NOINP_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_NOINP_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_NOINP_DT.SKBN_SGN1 = 0;
			break;
		case SCN_GYOKBN_PN:	// 業種の区分
			memset( (char *)SCAN_KEY.SCN_GYOKBN_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_GYOKBN_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_GYOKBN_DT.SKBN_SGN1 = 0;
			break;

		case SCN_HJIYU_PN:
			memset( (char *)SCAN_KEY.SCN_HJIYU_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_HJIYU_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_HJIYU_DT.SKBN_SGN1 = 0;
			break;

		case SCN_DOCEVI1_PN:
			memset( SCAN_KEY.SCN_DOCEVI1_DT.IP_SNUM, 0, sizeof SCAN_KEY.SCN_DOCEVI1_DT.IP_SNUM );
			memset( SCAN_KEY.SCN_DOCEVI2_DT.IP_SNUM, 0, sizeof SCAN_KEY.SCN_DOCEVI2_DT.IP_SNUM );
			break;

		case SCN_DOCSEQ1_PN:
			SCAN_KEY.SCN_DOCSEQ1_DT.IP_SEQ = SCAN_KEY.SCN_DOCSEQ2_DT.IP_SEQ = 0;
			break;

		case SCN_DOCSHTP_PN:
			memset( (char *)SCAN_KEY.SCN_DOCSHTP_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_DOCSHTP_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_DOCSHTP_DT.SKBN_SGN1 = 0;
			break;

		case SCN_SYZKTSN_PN:
			memset((char *)SCAN_KEY.SCN_SYZKTSN_DT.SKBN_TXT, 0, sizeof(SCAN_KEY.SCN_SYZKTSN_DT.SKBN_TXT));
			SCAN_KEY.SCN_SYZKTSN_DT.SKBN_SGN1 = 0;
			break;

		case SCN_DTTYPE_PN:
			memset( (char *)SCAN_KEY.SCN_DTTYPE_DT.SKBN_TXT, 0, sizeof( SCAN_KEY.SCN_DTTYPE_DT.SKBN_TXT ) );
			SCAN_KEY.SCN_DTTYPE_DT.SKBN_SGN1 = 0;
			break;

			// インボイス対応
		case SCN_INVNO_PN:	// 登録番号
			memset((char* )&SCAN_KEY.SCN_INVNO_DT, '\0', sizeof( SCAN_KEY.SCN_INVNO_DT ));
			(( CButton* )GetDlgItem( IDC_NCHK_INVNO ))->SetCheck( BST_UNCHECKED );
			break;

		case SCN_MENZEI_PN:	// 免税事業者からの課税仕入れ
			memset((char* )&SCAN_KEY.SCN_MENZEI_DT, '\0', sizeof( SCAN_KEY.SCN_MENZEI_DT ));
			(( CButton* )GetDlgItem( IDC_CHECK_MENZEI ))->SetCheck( BST_UNCHECKED );
			break;

		case SCN_MENWARI_PN:	// 免税事業者からの課税仕入れ 控除割合
			memset((char*)&SCAN_KEY.SCN_MENWARI_DT, '\0', sizeof(SCAN_KEY.SCN_MENWARI_DT));
			((CComboBox*)GetDlgItem(IDC_SCNMENWARI))->SetCurSel(0);
			break;

		default:
			break;
		}
		//コンディションサイン OFF
		memset( (char *)&SCAN_KEY.LINE_CND[i], (char)0, sizeof(SCAN_KEY.LINE_CND[i]) );
	}

	if( bDisp )	keyline_dsp( INP_CTL, &SCAN_KEY );

	set_nowpn( SCN_ORDER_PN );	// 選択項目背景色消去のため

	// イメージ摘要エディットOFF
	disp_imgtkyedit( SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_type );
//	selimg_btntxt( SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_type );

TRACE( "@F11 keyline_inz END\n" );

	set_topfocus();	// initial focus 
}

//１行表示
void CScanKeyView::keyline_dsp( struct _InpCtl *ctl, struct _SKEY_LINE *dtline )
{
	int i, chk;
	VARIANT var;
	char	buf[128];
//	BYTE	bcd[30];
	CString str;
	int sel;

TRACE( "@F11 keyline_dsp 01\n" );

	for( i = SCN_ORDER_PN ; i <= SCN_DTTYPE_PN ; ++i )
	{
	//	if( i > SCN_CBMN2_PN ) break;

		var.pbVal = NULL;
		switch( i )
		{
		case SCN_ORDER_PN:
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( 0 );
			break;
		case SCN_DUSER_PN:
			sel = pDBzm->data_authority() > 0 ? 0 : 1;
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( sel );
			SelchangeDuserJob(sel);
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
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DATE_PN)->IDC_X ))->SetCheck(chk);

			// ICSDateSelectにデータセット
			set_dateselect();

#ifdef OLD_CLOSE
			::ZeroMemory( bcd, sizeof bcd );
			memcpy( bcd, dtline->SCN_DATE1_DT.IP_BCDDAY, sizeof dtline->SCN_DATE1_DT.IP_BCDDAY );
			var.pbVal = bcd;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 0 );
			BcdDateToString( str, dtline->SCN_DATE1_DT.IP_BCDDAY );
			DBSetDispString( this, (ctl+i)->IDC_X, str );
//			break;
//		case SCN_DATE2_PN:
			::ZeroMemory( bcd, sizeof bcd );
			memcpy( bcd, dtline->SCN_DATE2_DT.IP_BCDDAY, sizeof dtline->SCN_DATE2_DT.IP_BCDDAY );
			var.pbVal = bcd;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 0 );
			BcdDateToString( str, dtline->SCN_DATE2_DT.IP_BCDDAY );
			DBSetDispString( this, (ctl+i)->IDC_X, str );
#endif
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
				pDBzm->BumonCodeToStr( buf, sizeof buf, dtline->SCN_BMON1_DT.IP_BMN );
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
				pDBzm->BumonCodeToStr( buf, sizeof buf, dtline->SCN_BMON2_DT.IP_BMN );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case SCN_CBMN1_PN:
			if( dtline->SCN_CBMN1_DT.IP_BMN != -1 ) {
				pDBzm->BumonCodeToStr( buf, sizeof buf, dtline->SCN_CBMN1_DT.IP_BMN );
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
				pDBzm->BumonCodeToStr( buf, sizeof buf, dtline->SCN_CBMN2_DT.IP_BMN );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;

		case SCN_DCBMN1_PN:
			if( dtline->SCN_DCBMN1_DT.IP_BMN != -1 ) {
				pDBzm->BumonCodeToStr( buf, sizeof buf, dtline->SCN_DCBMN1_DT.IP_BMN );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DCBMN_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_DCBMN2_PN:
			if( dtline->SCN_DCBMN2_DT.IP_BMN != -1 ) {
				pDBzm->BumonCodeToStr( buf, sizeof buf, dtline->SCN_DCBMN2_DT.IP_BMN );
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

		case SCN_DCKJI1_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->SCN_DCKJI1_DT.IP_KOJI );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DCKJI_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_DCKJI2_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->SCN_DCKJI2_DT.IP_KOJI );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;

		case SCN_DEBT_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//科目名称再表示
				knam_set( &dtline->SCN_DEBT_DT );
			}
			DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_DEBT_DT.IP_stKMK, pDBzm );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DEB_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_DEBT2_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//科目名称再表示
				knam_set( &dtline->SCN_DEBT2_DT );
			}
			DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_DEBT2_DT.IP_stKMK, pDBzm );
			break;

		case SCN_DBR1_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//枝番摘要再セット
				set_etcdsp( i );
			}
			if( dtline->SCN_DBR1_DT.IP_cBRN != -1 ) {
				pDBzm->EdabanToStr( buf, sizeof buf, dtline->SCN_DBR1_DT.IP_cBRN );
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
				pDBzm->EdabanToStr( buf, sizeof buf, dtline->SCN_DBR2_DT.IP_cBRN );
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
			DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_CRED_DT.IP_stKMK, pDBzm );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_CRE_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_CRED2_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//科目名称再表示
				knam_set( &dtline->SCN_CRED2_DT );
			}
			DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_CRED2_DT.IP_stKMK, pDBzm );
			break;

		case SCN_CBR1_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//枝番摘要再セット
				set_etcdsp( i );
			}
			if( dtline->SCN_CBR1_DT.IP_cBRN != -1 ) {
				pDBzm->EdabanToStr( buf, sizeof buf, dtline->SCN_CBR1_DT.IP_cBRN );
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
				pDBzm->EdabanToStr( buf, sizeof buf, dtline->SCN_CBR2_DT.IP_cBRN );
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
			DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_DBCR_DT.IP_stKMK, pDBzm );

			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DBCR_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_DBCR2_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//科目名称再表示
				knam_set( &dtline->SCN_DBCR2_DT );
			}
			DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_DBCR2_DT.IP_stKMK, pDBzm );
			break;

		case SCN_DCBR1_PN:
			if( dtline->LINE_CND[i].INP_sg )
			{
				//枝番摘要再セット
				set_etcdsp( i );
			}
			if( dtline->SCN_DCBR1_DT.IP_cBRN != -1 ) {
				pDBzm->EdabanToStr( buf, sizeof buf, dtline->SCN_DCBR1_DT.IP_cBRN );
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
				pDBzm->EdabanToStr( buf, sizeof buf, dtline->SCN_DCBR2_DT.IP_cBRN );
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

		case SCN_TAI1_PN:	//対価
			if( dtline->LINE_CND[ i ].INP_sg == TRUE )
			{
				var.pbVal = dtline->SCN_TAI1_DT.IP_VAL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}
			else
			{
				var.pbVal = NULL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_TAI_PN)->IDC_X ))->SetCheck(chk);

			break;
		case SCN_TAI2_PN:
			if( dtline->LINE_CND[ i ].INP_sg == TRUE )
			{
				var.pbVal = dtline->SCN_TAI2_DT.IP_VAL;
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
			chk = (dtline->LINE_CND[i].KEY_inp & SC_KMKHANI) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SCN_CHKKMSYZ_PN)->IDC_X ))->SetCheck(chk);	//属性検索
			break;
		case SCN_URISIRE_PN:
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( SCAN_KEY.SCN_URISIRE_DT.SKBN_SGN1 );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_URISIRE_PN)->IDC_X ))->SetCheck(chk);
			break;
		// 特定収入[06.03 /08]
		case SCN_TOKUS_PN:
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( SCAN_KEY.SCN_TOKUS_DT.SKBN_SGN1 );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_TOKUS_PN)->IDC_X ))->SetCheck(chk);
			break;

		case SCN_TKY_PN:
			((CComboBox*)GetDlgItem(IDC_TKYCOMBO))->SetCurSel( 0 );
			strcpy_s( buf, sizeof buf, (char*)dtline->SCN_TKY_DT.IP_TKY );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			scGetINP_CTL(SNOT_TKY_PN)->INP_ena = TRUE;
			m_NotTky.EnableWindow( TRUE );
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			m_NotTky.SetCheck(chk);
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

		case SCN_HUCMT_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->SCN_HUCMT_DT.IP_HUCMT );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;

		case SCN_NOINP_PN:
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( SCAN_KEY.SCN_NOINP_DT.SKBN_SGN1 );
			break;

		case SCN_GYOKBN_PN:	// 業種の区分
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( SCAN_KEY.SCN_GYOKBN_DT.SKBN_SGN1 );
			break;

		case SCN_HJIYU_PN:
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( SCAN_KEY.SCN_HJIYU_DT.SKBN_SGN1 );
			break;

		//証憑番号 [11.26 /12]
		case SCN_DOCEVI1_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->SCN_DOCEVI1_DT.IP_SNUM );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			
			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DOCEVI_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_DOCEVI2_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->SCN_DOCEVI2_DT.IP_SNUM );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		//文書番号
		case SCN_DOCSEQ1_PN:
			if( dtline->SCN_DOCSEQ1_DT.IP_SEQ > 0 ) {
				sprintf_s( buf, sizeof buf, "%d", dtline->SCN_DOCSEQ1_DT.IP_SEQ );
				var.pbVal = (BYTE*)buf;
			}
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem( (ctl+SNOT_DOCSEQ_PN)->IDC_X ))->SetCheck(chk);
			break;
		case SCN_DOCSEQ2_PN:
			if( dtline->SCN_DOCSEQ2_DT.IP_SEQ > 0 ) {
				sprintf_s( buf, sizeof buf, "%d", dtline->SCN_DOCSEQ2_DT.IP_SEQ );
				var.pbVal = (BYTE*)buf;
			}
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;

		case SCN_DOCSHTP_PN:	// 書類種別
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( SCAN_KEY.SCN_DOCSHTP_DT.SKBN_SGN1 );
			break;

		case SCN_SYZKTSN_PN:	// 資産
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( SCAN_KEY.SCN_SYZKTSN_DT.SKBN_SGN1 );
			break;

		case SCN_DTTYPE_PN:	// 資産
			((CComboBox*)GetDlgItem((ctl+i)->IDC_X))->SetCurSel( SCAN_KEY.SCN_DTTYPE_DT.SKBN_SGN1 );
			break;

			// インボイス対応
		case SCN_INVNO_PN:	// 登録番号
			sprintf_s( buf, sizeof( buf ), _T( "%s" ), dtline->SCN_INVNO_DT.INVNO );
			char temp[64];
			::ZeroMemory(temp, sizeof temp);
			if( strlen( buf ) > 0 ) {
				var.pbVal = ( BYTE* )buf;
				sprintf_s(temp, sizeof(temp), _T("T%s"), buf);
			}
			else {
				var.pbVal = NULL;
			}
			DBdata_set( this, ( ctl + i )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			DBSetDispString( this, IDC_SCNINVNODISP/*( ctl + i )->IDC_X*/, ( char* )temp );
			break;

		case SCN_MENZEI_PN:
			if( dtline->SCN_MENZEI_DT.MENZEI ) {
				(( CButton* )GetDlgItem( IDC_CHECK_MENZEI ))->SetCheck( BST_CHECKED );
			}
			else {
				(( CButton* )GetDlgItem( IDC_CHECK_MENZEI ))->SetCheck( BST_UNCHECKED );
			}

			chk = (dtline->LINE_CND[i].KEY_inp & SC_NOTSEARCH) ? 1 : 0;
			((CButton*)GetDlgItem((ctl + SNOT_MENZEI_PN)->IDC_X))->SetCheck(chk);
			break;

		case SCN_MENWARI_PN:
			((CComboBox*)GetDlgItem((ctl + i)->IDC_X))->SetCurSel(SCAN_KEY.SCN_MENWARI_DT.SKBN_SGN1);
			break;

		default:
			break;
		}
	}

}


// ICSDateSelectにデータセット
void CScanKeyView::set_dateselect()
{
	int sy, ey,sofs, eofs;
	struct _SKEY_LINE *dtline;

	sy = ey = -1;

	dtline = &SCAN_KEY;

	// 開始日付
	if( dtline->SCN_DATE1_DT.IP_BCDDAY[0] != 0 ) {
		skey_dateconv( dtline->SCN_DATE1_DT.IP_BCDDAY, &sy, &sofs );
	}

	if( sy == -1 ) {
		sofs = -1;
		pDBzm->minfo->MoveFirst();
		do {
			if( pDBzm->minfo->insw & 0x01 ) {
				sofs = pDBzm->minfo->ofset;
			//	sy = pDBzm->minfo->smd;
				break;
			}
		} while( pDBzm->minfo->MoveNext() == 0 );
	}
	// 終了日付
	if( dtline->SCN_DATE2_DT.IP_BCDDAY[0] != 0 ) {
		skey_dateconv( dtline->SCN_DATE2_DT.IP_BCDDAY, &ey, &eofs );
	}

	if( ey == -1 ) {
		eofs = -1;
		pDBzm->minfo->MoveFirst();
		do {
			if( pDBzm->minfo->insw & 0x01 ) {
				eofs = pDBzm->minfo->ofset;
			//	ey = pDBzm->minfo->emd;
			}
			if( pDBzm->minfo->kes_sgn == 4 )
				break;
		} while( pDBzm->minfo->MoveNext() == 0 );
	}

	m_selDate.SetMonthOffset( sofs, eofs, sy, ey );
}


// 日付データを作成
int CScanKeyView::skey_dateconv( BYTE* bcddate, int* pYear, int* pOfs )
{
	int sgn = KESY_SG_GET( bcddate[0] );

	CVolDateDB vd;
	BYTE tmp[4] = {0};
	char buf[64];

	switch( sgn ) {
	case 0: tmp[0] = 0x61;	break;
	case 1: tmp[0] = 0x62;	break;
	case 2: tmp[0] = 0x63;	break;
	case 3: tmp[0] = 0x00;	break;
	default:
			tmp[0] = bcddate[0];	break;
	}

	tmp[1] = bcddate[1];

	int md, genyy, ysgn, year, ofs, gengo, d_gengo;
	sprintf_s( buf, sizeof buf, "%02x%02x", tmp[0], tmp[1] );
	md = atoi( buf );

	if( vd.db_vd_yearGen( 0, md, &gengo, &genyy, &ysgn, pDBzm ) != 0 )
		return -1;

	if( vd.db_datecnvGen( gengo, genyy, &d_gengo, &year, 1, 2 ) != 0 )
		return -1;

	sprintf_s( buf, sizeof buf, "%04d%02x%02x", year, tmp[0], tmp[1] );
	year = atoi( buf );

	vd.db_vd_offset( tmp, &ofs, pDBzm );

	*pYear	= year;
	*pOfs	= ofs;

	return 0;
}


// 科目名称再セット
void CScanKeyView::knam_set( struct _ItemData *data )
{
	struct _DBKNREC	*pKn;

	if( pKn = pDBzm->DB_PjisToKnrec( data->IP_CdNum ) ) {
		memcpy( data->IP_KNAM, pKn->knnam, KMNLEN );
	}
}


//オリジナルダイアログサイズ
void CScanKeyView::get_originalsize( double& w, double& h )
{
	w = (double)(m_original_rect.right - m_original_rect.left);
	h = (double)(m_original_rect.bottom - m_original_rect.top);
}




/*
#ifdef	_ICSPEN_	// 01.20 /01
//ペンDLL初期化
short CScanKeyView::SetPen( BOOL flg, ICSPen* ppen )
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
void CScanKeyView::SetPenRect( CRect& rect )
{
	rectPen	=	rect;
}

//消費税選択後の移動
void CScanKeyView::set_nextpos()
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


void CScanKeyView::OnICSInputFocus( int pn )
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
void CScanKeyView::last_data_set()
{
int pn = get_nowpn();

	if( get_nowpn() == SCN_SEQ1_PN )
		_set_focus( SCN_SEQ2_PN );
	else
		_set_focus( SCN_SEQ1_PN );

}

// IDC get
struct _InpCtl *CScanKeyView::scGetINP_CTL( int pn )
{
	return( &INP_CTL[pn] );
}


//
LRESULT CScanKeyView::RetroFocus( WPARAM wParam, LPARAM lParam ) 
{
#ifdef _ICSPEN_	// 01.20 /01
	_set_focus( wParam );
#else
	set_focus( wParam );
#endif
	return 1;
}

// 現在フォーカス位置にカーソルセット
void CScanKeyView::set_nowfocus()
{
	PostMessage( WM_RETROFOCUS, get_nowpn() );
}

// 先頭項目にカーソルセット
void CScanKeyView::set_topfocus()
{
	PostMessage( WM_RETROFOCUS, SCN_ORDER_PN );
}


// カーソルセット
#ifdef _ICSPEN_	// 01.20 /01
void CScanKeyView::set_focus( int pn )
{
	PostMessage( WM_RETROFOCUS, pn );
}
void CScanKeyView::_set_focus( int pn )
{
#else
void CScanKeyView::set_focus( int pn )
{
#endif

	if( pn == SCN_TKY_PN ) {
		if( is_mdfy_imgtky() )
			pn = SCN_IMGEDT_PN;
	}
	/*---VER2----*/
	if( pn != SCN_TKY_PN ) {
		m_Report.ShowWindow( SW_HIDE );
	}

	// 日付コントロール
	if( pn == SCN_DATE1_PN ) {
		CWnd* pwnd;
		pwnd = m_selDate.GetWindow( GW_CHILD );
		m_selDate.SetFocus();
		return;
	}

	// インボイス対応
	if( pn <= SCN_DTTYPE_PN )
		c_set( scGetINP_CTL( pn )->IDC_X );
	else if( pn == SCN_IDOK )
	{
		c_set( IDOK );
	}
	else if( pn == SCN_IDCANCEL )
	{
		c_set( IDCANCEL );
	}
	else if( pn == SCN_TKYONLY ) {//#* 05.16 /01 摘要のみ修正
		c_set( IDC_TKYONLY );
	}
	else if( pn == SCN_IMGEDT_PN ) {//#* 06.19 /01 ｲﾒｰｼﾞ摘要のみ検索
		c_set( IDC_IMGTKYEDIT );
	}
	else if( pn == SCN_ALLDEL_PN ) {
		c_set(IDC_ALLKEYDEL);
	}
	else if( pn == SCN_DUPLI_PN ) {
		c_set(IDC_BTN_DUPLI);
	}
	else if( pn == SCN_DELSWK_PN ) {
		c_set(IDC_BTN_DEL);
	}
	else if( pn == SCN_SELTKY_PN ) {
		c_set( IDC_TKYCOMBO );
	}
	else if( pn >= SCN_CHKFURI_PN  && pn <= SNOT_LAST_PN ) { // チェック ＆ NOT
		c_set( scGetINP_CTL( pn )->IDC_X );
	}

}

void CScanKeyView::FocusBhnd(  )
{
	c_set( IDC_ICSDISPCTRL1 );	// タイトルに 
}


// 部門入力表示ＯＮ／ＯＦＦ ... 08.28 /02
void CScanKeyView::disp_bmoninp( int sw )
{
	if( sw != bmon_dispcnd() )
	{
		bmon_dispcnd() = sw;

	//	_disp_brinp( sw, IDC_ICSHEDITCTRL2 );
	}
}


// 注意表示ＯＮ／ＯＦＦ
void CScanKeyView::disp_caution( int sw )
{
	if( sw != caution_dispcnd() )
	{
		caution_dispcnd() = sw;

		_disp_caution( sw );
	}
}

void CScanKeyView::_disp_caution( int sw )
{
	CWnd *disporg;

	VERIFY(disporg = GetDlgItem( IDC_ICSDISPCTRL2 ) );

	if( ! sw ) {
		disporg->ShowWindow(SW_HIDE);
		return;
	}

	disporg->ShowWindow(SW_SHOW);

	CWnd* base;
	CRect rect, dispRect;

	disporg->GetWindowRect(&dispRect);
	int x, y;

	if( bSCANSAVE_Master ) {
		base = GetDlgItem(IDC_BTN_DEL);
	}
	else {
		base = GetDlgItem(IDC_SELGUIDE);
	}
	base->GetWindowRect(&rect);
	ScreenToClient( &rect );
	x = rect.right - dispRect.Width();
	y = rect.bottom;
	disporg->MoveWindow( x, y, dispRect.Width(), dispRect.Height() );
}



static BOOL isfixvalcnd( int pn )
{	
	return TRUE;
	
}

//ポジションによる表示切替
void CScanKeyView::disp_change()
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
	case SCN_BMON1_PN:	case SCN_BMON2_PN :
	case SCN_CBMN1_PN:	case SCN_CBMN2_PN :
	case SCN_DCBMN1_PN:	case SCN_DCBMN2_PN :
	case SCN_KOJI1_PN:	case SCN_KOJI2_PN :
	case SCN_CKJI1_PN:	case SCN_CKJI2_PN :
	case SCN_DCKJI1_PN:	case SCN_DCKJI2_PN :
		sg = FALSE;	
		break;
	case SCN_TKY_PN:
	case SCN_INVNO_PN:
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
void CScanKeyView::_disp_change()
{
int pn = get_nowpn();
BOOL sg = FALSE;

	if (pn == SCN_HJIYU_PN)	sg = TRUE;
	else {
		if (pn >= SCN_BMON1_PN && pn <= SCN_DCKJI2_PN )
			sg = FALSE;
		else if(pn == SCN_TKY_PN || pn == SCN_INVNO_PN )
			sg = FALSE;
		else {
			sg = TRUE;
		}
	}

	// 固定科目選択のディセーブル／イネーブル
	fix8disp( sg );
}


BOOL CScanKeyView::disp_brnselect( int pn )
{
	BOOL sg, inpsg;
	CString code;

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
		break;
	}
	// 枝番摘要
	if( sg == TRUE )
	{
		::ZeroMemory( saveITEM50kana, sizeof saveITEM50kana );
		BRNTKY_select( -1, code, NULL, 1 );
	}

	return sg;
}


// 現在ポジションセット
int CScanKeyView::set_nowpn( int pn )
{
	BOOL sg;
	CString code;
	int prev_pn;

	prev_pn = NOW_PN;
	NOW_PN = pn;

	//科目名称選択画面を直前のポジションに従いリセット
	SelItemReset( NOW_PN, prev_pn );

#ifdef DB_OLD_CLOSE
	//表題を窪み表示
	selbox( pn );
#endif

	disp_change();

	if( prev_pn != pn )
	{

		sg = disp_brnselect( pn );

		if( sg == FALSE ) {
			SelPar par;
			par.sel_pn = SL_RESET;
			::ZeroMemory(par.tky_kana, sizeof par.tky_kana);

			fixvalcnd( !isfixvalcnd( pn ));
			disp_change();

			m_Seldata.SelectJob( &par, -1 );
		}

		if( pn != SCN_TKY_PN ) {
			m_Report.ShowWindow( SW_HIDE );
		}

		// ツールバー切替
		switch( pn )
		{
		case SCN_DEBT_PN:	case SCN_DEBT2_PN:
		case SCN_CRED_PN:	case SCN_CRED2_PN:
		case SCN_DBCR_PN:	case SCN_DBCR2_PN:
			switch( prev_pn ) {
				case SCN_DEBT_PN:	case SCN_DEBT2_PN:
				case SCN_CRED_PN:	case SCN_CRED2_PN:
				case SCN_DBCR_PN:	case SCN_DBCR2_PN:
					break;
				default:
					((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(2,1);
					break;
			}
			break;
		default:
			switch( prev_pn ) {
				case SCN_DEBT_PN:	case SCN_DEBT2_PN:
				case SCN_CRED_PN:	case SCN_CRED2_PN:
				case SCN_DBCR_PN:	case SCN_DBCR2_PN:
					((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(2);
					break;
				default:
					break;
			}
			break;
		}
	}

	// コメント表示
	cmntbox( pn );

#ifdef DB_OLD_CLOSE
	// 選択背景色
	select_back_col( pn );
	// 入力項目背景色
	inp_bkcol( pn );
#endif

	if( prev_pn == SCN_TKY_PN ) {
		ICSColorInfo ci;
		((ICSWinApp*)AfxGetApp())->GetStanderdColor( ci );

		struct _InpCtl* ctl;
		COLORREF	set_col;

		if( SC_Tky.GetCount() > 0 )
		{
			set_col = RGB_YELLOW1;	// 入力有りカラー
		}
		else
		{
			set_col = ci.BackColor();
		}

		ctl = scGetINP_CTL( prev_pn );

		CWnd* pwnd = GetDlgItem( ctl->IDC_X );

		if( ! is_mdfy_imgtky() ) {
			((CICSDBEDT*)pwnd)->SetBackColor( set_col );
		}
	}
	TGnumWndMove();
/*
	switch( pn )
	{
	case SCN_DBR1_PN:
	case SCN_DBR2_PN:
	case SCN_CBR1_PN:
	case SCN_CBR2_PN:
	case SCN_DCBR1_PN:
	case SCN_DCBR2_PN:
	case SCN_BMON1_PN:
	case SCN_BMON2_PN:
	case SCN_CBMN1_PN:
	case SCN_CBMN2_PN:
	case SCN_KOJI1_PN:
	case SCN_KOJI2_PN:
	case SCN_CKJI1_PN:
	case SCN_CKJI2_PN:
		break;
	default:
		dsp_etc( pn, FALSE );
		break;
	}
*/

	return( prev_pn );	//直前のポジション
}


// 現在ポジションゲット
int CScanKeyView::get_nowpn( void )
{
	return( NOW_PN );
}



// イメージ摘要のみ か 文字摘要のみ のスキャンか？
// 
int CScanKeyView::is_mdfy_imgtky()
{
	if( IMG_master == ERR )
		return FALSE;

	if( SCAN_KEY.LINE_CND[SCN_TKY_PN].INP_sg && SCAN_KEY.LINE_CND[SCN_TKY_PN].INP_type )
		return TRUE;
	else
		return FALSE;
}


// イメージ摘要のエディットの表示/非表示
void CScanKeyView::disp_imgtkyedit( int sw )
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

/*
		if( now_pn == SCN_SELTKY_PN )
			set_focus( SCN_TKY_PN );
		else
			set_focus( now_pn );
*/
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
/*
		set_focus( SCN_IMGEDT_PN );
		if( now_pn != SCN_TKY_PN )
			set_nowpn( SCN_TKY_PN );
*/
	}
}


// データアドレスリード
struct _ItemData *CScanKeyView::_getDATA( int pn )
{
	return &SCAN_KEY._XXDTA2[pn];
}

struct _ItemData *CScanKeyView::getDATA()
{
	return _getDATA( get_nowpn() );
}


//　その他項目コントロールデータレコードをセット
void CScanKeyView::set_etcdsp( int pn )
{
int code_pn, bmn1, bmn2;
struct _SET_KMKETC sk;
struct _ItemData *pdata; /*data*/

	if( pn == SCN_DBR1_PN || pn == SCN_DBR2_PN ||
		pn == SCN_CBR1_PN || pn == SCN_CBR2_PN ||
		pn == SCN_DCBR1_PN || pn == SCN_DCBR2_PN )
	{
		switch( pn )
		{
		case SCN_DBR1_PN:
		case SCN_DBR2_PN:
			code_pn = SCN_DEBT_PN;
			bmn1 = _getDATA( SCN_BMON1_PN )->IP_BMN;
			bmn2 = _getDATA( SCN_BMON2_PN )->IP_BMN;
			break;
		case SCN_CBR1_PN:
		case SCN_CBR2_PN:
			code_pn = SCN_CRED_PN;
			bmn1 = _getDATA( SCN_CBMN1_PN )->IP_BMN;
			bmn2 = _getDATA( SCN_CBMN2_PN )->IP_BMN;
			break;
		case SCN_DCBR1_PN:
		case SCN_DCBR2_PN:
			code_pn = SCN_DBCR_PN;
			bmn1 = bmn2 = -1;
			break;
		}
		// 部門枝番は、検索しない
		if( bmn1 != -1 )		sk.bmn = bmn1;
		else if( bmn2 != -1)	sk.bmn = bmn2;
		else					sk.bmn = -1;

		sk.code = _getDATA( code_pn )->IP_CdNum;
		sk.brn = _getDATA( pn )->IP_cBRN;

		pdata = _getDATA( pn );

		// その他名称を取得
		if( _set_kmketc( &sk, FALSE ) ) {
			SCAN_KEY.LINE_CND[pn].ETC_TXT_sg = TRUE;
			SCAN_KEY.LINE_CND[pn].ETC_TXT_ForeCOL = sk.etc_col;

			strcpy_s( pdata->ETC_TXT, sizeof pdata->ETC_TXT, sk.etc );
		}
		else {
			SCAN_KEY.LINE_CND[pn].ETC_TXT_sg = FALSE;
			SCAN_KEY.LINE_CND[pn].ETC_TXT_ForeCOL = 0;

			::ZeroMemory( pdata->ETC_TXT, sizeof pdata->ETC_TXT );
		}
	}
	else if( pn == SCN_BMON1_PN || pn == SCN_BMON2_PN || 
			 pn == SCN_CBMN1_PN || pn == SCN_CBMN2_PN ||
			 pn == SCN_DCBMN1_PN || pn == SCN_DCBMN2_PN  ) {
		struct _DATA_LINE line;
		inpline_inz( &line );
		pdata = _getDATA( pn );
		// 名称を取得
		line._XXDTA.dd_dbmn = pdata->IP_BMN;
		_set_bmndsp( &line, BMON_PN );

		SCAN_KEY.LINE_CND[pn].ETC_TXT_sg		=	line.LINE_CND[BMON_PN].ETC_TXT_sg;
		SCAN_KEY.LINE_CND[pn].ETC_TXT_ForeCOL	=	line.LINE_CND[BMON_PN].ETC_TXT_ForeCOL;

		if( line._XXDTA.dd_dbmn_etc.IsEmpty() )
			::ZeroMemory( pdata->ETC_TXT, sizeof pdata->ETC_TXT );
		else
			strcpy_s( pdata->ETC_TXT, sizeof pdata->ETC_TXT, line._XXDTA.dd_dbmn_etc );

	}
	else if( pn == SCN_KOJI1_PN || pn == SCN_KOJI2_PN || 
			 pn == SCN_CKJI1_PN || pn == SCN_CKJI2_PN ||
			 pn == SCN_DCKJI1_PN || pn == SCN_DCKJI2_PN ) {
		struct _DATA_LINE line;
		inpline_inz( &line );
		pdata = _getDATA( pn );

		// 名称を取得
		line._XXDTA.dd_dkno = pdata->IP_KOJI;

		LPSTR p = line._XXDTA.dd_dkno.GetBuffer(64);
		trim_zero( p, 64, line._XXDTA.dd_dkno.GetLength() );
		line._XXDTA.dd_dkno.ReleaseBuffer();
		_set_bmndsp( &line, KOJI_PN );

		SCAN_KEY.LINE_CND[pn].ETC_TXT_sg		=	line.LINE_CND[KOJI_PN].ETC_TXT_sg;
		SCAN_KEY.LINE_CND[pn].ETC_TXT_ForeCOL	=	line.LINE_CND[KOJI_PN].ETC_TXT_ForeCOL;

		if( line._XXDTA.dd_dkno_etc.IsEmpty() )
			::ZeroMemory( pdata->ETC_TXT, sizeof pdata->ETC_TXT );
		else
			strcpy_s( pdata->ETC_TXT, sizeof pdata->ETC_TXT, line._XXDTA.dd_dkno_etc );
	}
}


void CScanKeyView::CtrlShow( int idc, BOOL sw )
{
	GetDlgItem( idc )->ShowWindow( sw ? SW_SHOW : SW_HIDE );
}

// その他項目　表示
void CScanKeyView::dsp_etc( int pn, BOOL dspsw )
{
struct _ItemData *pdata;
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

		break;

	case SCN_BMON1_PN:	case SCN_BMON2_PN:
	case SCN_CBMN1_PN:	case SCN_CBMN2_PN:
	case SCN_DCBMN1_PN:	case SCN_DCBMN2_PN:
		// 項目表示
		pdata = _getDATA( pn );
		dsp = _dsp_etc( this, pdata, &SCAN_KEY.LINE_CND[pn], scGetINP_CTL(pn), BUMON_X_RATE, dspsw, FALSE );
		break;

	case SCN_KOJI1_PN:	case SCN_KOJI2_PN:
	case SCN_CKJI1_PN:	case SCN_CKJI2_PN:
	case SCN_DCKJI1_PN:	case SCN_DCKJI2_PN:
		// 項目表示
		pdata = _getDATA( pn );
		dsp = _dsp_etc( this, pdata, &SCAN_KEY.LINE_CND[pn], scGetINP_CTL(pn), BUMON_X_RATE, dspsw, FALSE );
		break;

	default:
		GetDlgItem(IDC_DISPETC)->SetWindowPos( this, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE );
		break;
	}
}

//アクセス用クラスセット
void CScanKeyView::fnc_set( struct _InpCtl *ctl)
{
	int i;

	for( i = SCN_SEQ1_PN ; i <= SNOT_LAST_PN ; ++i )
	{
		switch( (ctl+i)->IDC_X )
		{
		case IDC_SCNSEQ1:	(ctl+i)->p_obj = (long)&m_Seq1;	break;
		case IDC_SCNSEQ2: 	(ctl+i)->p_obj = (long)&m_Seq2;	break;
	//	case IDC_SCNDATE1:	/*(ctl+i)->p_obj = (long)&m_Date1;*/	break;	
	//	case IDC_SCNDATE2:	/*(ctl+i)->p_obj = (long)&m_Date2;*/	break;	
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
		// 科目(終了分)
		case IDC_SCNDEBT2:	(ctl+i)->p_obj = (long)&m_Debt2;	break;
		case IDC_SCNCRED2:	(ctl+i)->p_obj = (long)&m_Cred2;	break;
		case IDC_SCNDBCR2:	(ctl+i)->p_obj = (long)&m_DBcr2;	break;

		case IDC_SCNVAL1:	(ctl+i)->p_obj = (long)&m_Val1;	break;
		case IDC_SCNVAL2:	(ctl+i)->p_obj = (long)&m_Val2;	break; 
		case IDC_SCNZEI1:	(ctl+i)->p_obj = (long)&m_Zei1;	break; 
		case IDC_SCNZEI2:	(ctl+i)->p_obj = (long)&m_Zei2;	break; 
		case IDC_SCNTKY:	(ctl+i)->p_obj = (long)&m_Tky;	break; 
		case IDC_SCNSNUM1: 	(ctl+i)->p_obj = (long)&m_Snum1;	break;
		case IDC_SCNSNUM2:	(ctl+i)->p_obj = (long)&m_Snum2;	break;
		case IDC_SCNTDAT1:	(ctl+i)->p_obj = (long)&m_Tdat1;	break;
		case IDC_SCNTDAT2:	(ctl+i)->p_obj = (long)&m_Tdat2;	break;
		// 貸借部門・工事
		case IDC_SCNDCBMN1:	(ctl+i)->p_obj = (long)&m_DCbmn1;	break;
		case IDC_SCNDCBMN2:	(ctl+i)->p_obj = (long)&m_DCbmn2;	break;
		case IDC_SCNDCKJI1:	(ctl+i)->p_obj = (long)&m_DCkji1;	break;
		case IDC_SCNDCKJI2:	(ctl+i)->p_obj = (long)&m_DCkji2;	break;
		// 文書番号
		case IDC_SCNDOCSEQ1:	(ctl+i)->p_obj = (long)&m_DocSeq1;	break;
		case IDC_SCNDOCSEQ2:	(ctl+i)->p_obj = (long)&m_DocSeq2;	break;
		case IDC_SCNINVNO:		(ctl + i)->p_obj = (long)&m_InvnoEdt;	break;
		default:
			break;	
		}	
	}
}

//入力コントロール，１行イニシャライズ
void CScanKeyView::inpctlline_inz( struct _InpCtl *ctl )
{
	fnc_set( ctl );
	int bm_sgn, kj_sgn, ed_sgn;
	char tmp[20];
	bm_sgn = kj_sgn = ed_sgn = 0;

	for( int i = SCN_ORDER_PN ; i <= SNOT_LAST_PN ; ++i )
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

		case SCN_DCBMN1_PN:
		case SCN_DCBMN2_PN:
		case SNOT_DCBMN_PN:

			if( (Voln1->sub_sw&0x02) )
			{
				(ctl+i)->INP_ena = TRUE;
				if( (ctl+i)->p_obj ) {
					sprintf_s( tmp, sizeof tmp, "%d", _op_max_calq( Voln1->bmcol ) );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMinval( "0" );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxval( tmp );
				//	((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxLen( 0 );	//桁数設定(数字のときは 0)
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxLen( Voln1->bmcol );	//桁数設定(カナ検索に変更のため)

					((CICSDBEDT *)(ctl+i)->p_obj)->EnableWindow( TRUE );
					((CICSDBEDT *)(ctl+i)->p_obj)->ImeEndMode( TRUE );
				}
				else {
					GetDlgItem((ctl+i)->IDC_X)->EnableWindow( TRUE );
					GetDlgItem((ctl+i)->IDC_X)->EnableWindow( TRUE );
				}
				if( ! bm_sgn ) {
					bm_sgn = 1;
					GetDlgItem(IDC_ST_DBMN)->EnableWindow( TRUE );
					GetDlgItem(IDC_ST_CBMN)->EnableWindow( TRUE );
					GetDlgItem(IDC_STATIC4)->EnableWindow( TRUE );
					GetDlgItem(IDC_STATIC5)->EnableWindow( TRUE );
					//貸借部門
					GetDlgItem(IDC_ST_DCBMN)->EnableWindow( TRUE );
					GetDlgItem(IDC_STATIC27)->EnableWindow( TRUE );
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
					GetDlgItem(IDC_ST_DCBMN)->EnableWindow( FALSE );
					GetDlgItem(IDC_STATIC4)->EnableWindow( FALSE );
					GetDlgItem(IDC_STATIC5)->EnableWindow( FALSE );
					GetDlgItem(IDC_STATIC27)->EnableWindow( FALSE );
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

		case SCN_DCKJI1_PN:
		case SCN_DCKJI2_PN:
		case SNOT_DCKJI_PN:

//TRACE( "KOJI i = %d\n", i );
			if( Voln1->apno == 0x20 && (Voln1->sub_sw&0x04) )
			{
				(ctl+i)->INP_ena = TRUE;
				if( (ctl+i)->p_obj ) {
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxLen( pDBzm->zvol->kjcol  );
					sprintf_s( tmp, sizeof tmp, "%d", _op_max_calq( Voln1->kjcol ) );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMinval( "0" );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxval( tmp );
					((CICSDBEDT *)(ctl+i)->p_obj)->EnableWindow( TRUE );
					((CICSDBEDT *)(ctl+i)->p_obj)->ImeEndMode( TRUE );
				}
				else {
					GetDlgItem((ctl+i)->IDC_X)->EnableWindow( TRUE );
					GetDlgItem((ctl+i)->IDC_X)->EnableWindow( TRUE );
				}
				if( ! kj_sgn ) {
					kj_sgn = 1;
					GetDlgItem(IDC_ST_DKJ)->EnableWindow( TRUE );
					GetDlgItem(IDC_ST_CKJ)->EnableWindow( TRUE );
					GetDlgItem(IDC_STATIC6)->EnableWindow( TRUE );
					GetDlgItem(IDC_STATIC7)->EnableWindow( TRUE );
					//貸借工事
					GetDlgItem(IDC_ST_DCKJ)->EnableWindow( TRUE );
					GetDlgItem(IDC_STATIC26)->EnableWindow( TRUE );
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
					GetDlgItem(IDC_ST_DCKJ)->EnableWindow( FALSE );
					GetDlgItem(IDC_STATIC6)->EnableWindow( FALSE );
					GetDlgItem(IDC_STATIC7)->EnableWindow( FALSE );
					GetDlgItem(IDC_STATIC26)->EnableWindow( FALSE );
				}
			}
			break;

		case SCN_DEBT_PN:
		case SCN_CRED_PN:
		case SCN_DBCR_PN:
			(ctl+i)->INP_ena = TRUE;
			if( (ctl+i)->p_obj ) {
				((CICSDBEDT*)(ctl+i)->p_obj )->ImeEndMode( TRUE );
				((CICSDBEDT*)(ctl+i)->p_obj )->SetKanaTerm(TRUE, KanaSearchKeta(), ICSDBEDT_KTYPE_KANA );
			}
			break;
		// 科目(終了)
		case SCN_DEBT2_PN:
		case SCN_CRED2_PN:
		case SCN_DBCR2_PN:
			if( m_KmkType == 0 ) {
				(ctl+i)->INP_ena = FALSE;
			}
			else {
				(ctl+i)->INP_ena = TRUE;
			}
			if( (ctl+i)->p_obj ) {
				((CICSDBEDT*)(ctl+i)->p_obj )->ImeEndMode( TRUE );
				((CICSDBEDT*)(ctl+i)->p_obj )->SetKanaTerm(TRUE, KanaSearchKeta(), ICSDBEDT_KTYPE_KANA );

				((CICSDBEDT *)(ctl+i)->p_obj)->EnableWindow( (ctl+i)->INP_ena );
			}
			GetDlgItem(IDC_STATIC15)->EnableWindow( (ctl+i)->INP_ena );
			GetDlgItem(IDC_STATIC17)->EnableWindow( (ctl+i)->INP_ena );
			GetDlgItem(IDC_STATIC18)->EnableWindow( (ctl+i)->INP_ena );
			break;

		case SCN_DBR1_PN:
		case SCN_DBR2_PN:
		case SCN_CBR1_PN:
		case SCN_CBR2_PN:
		case SCN_DCBR1_PN:
		case SCN_DCBR2_PN:

		case SCN_CHKDBR_PN:
		case SCN_CHKCBR_PN:
		case SNOT_DBRN_PN:
		case SNOT_CBRN_PN:
		case SNOT_DCBRN_PN:

			if( (Voln1->br_sw&0x01) ) {
				(ctl+i)->INP_ena = TRUE;
				if( (ctl+i)->p_obj ) {
					sprintf_s( tmp, sizeof tmp, "%d", _op_max_calq( Voln1->edcol ) );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMinval( "0" );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxval( tmp );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxLen( 0 );	//桁数設定(数字のときは 0)

					((CICSDBEDT *)(ctl+i)->p_obj )->ImeEndMode( TRUE );
					((CICSDBEDT *)(ctl+i)->p_obj )->SetKana( TRUE );
					((CICSDBEDT *)(ctl+i)->p_obj )->SetKanaTerm(TRUE, KanaSearchKeta(), ICSDBEDT_KTYPE_KANA );
					((CICSDBEDT *)(ctl+i)->p_obj)->EnableWindow( TRUE );
				}
				else {
					GetDlgItem((ctl+i)->IDC_X)->EnableWindow( TRUE );
					GetDlgItem((ctl+i)->IDC_X)->EnableWindow( TRUE );
				}
				if( ! ed_sgn ) {
					ed_sgn = 1;
					GetDlgItem(IDC_ST_DBR)->EnableWindow( TRUE );
					GetDlgItem(IDC_ST_CBR)->EnableWindow( TRUE );
					GetDlgItem(IDC_ST_DCBR)->EnableWindow( TRUE );
					GetDlgItem(IDC_STATIC8)->EnableWindow( TRUE );
					GetDlgItem(IDC_STATIC9)->EnableWindow( TRUE );
					GetDlgItem(IDC_STATIC10)->EnableWindow( TRUE );
				}
			}
			else
			{
				(ctl+i)->INP_ena = FALSE;
				if( (ctl+i)->p_obj ) {
					sprintf_s( tmp, sizeof tmp, "%d", _op_max_calq( Voln1->edcol ) );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMinval( "0" );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxval( tmp );
					((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxLen( 0 );

					((CICSDBEDT *)(ctl+i)->p_obj)->EnableWindow( FALSE );
				}
				else {
					GetDlgItem((ctl+i)->IDC_X)->EnableWindow( FALSE );
					GetDlgItem((ctl+i)->IDC_X)->EnableWindow( FALSE );
				}
				if( ! ed_sgn ) {
					ed_sgn = 1;
					GetDlgItem(IDC_ST_DBR)->EnableWindow( FALSE );
					GetDlgItem(IDC_ST_CBR)->EnableWindow( FALSE );
					GetDlgItem(IDC_ST_DCBR)->EnableWindow( FALSE );
					GetDlgItem(IDC_STATIC8)->EnableWindow( FALSE );
					GetDlgItem(IDC_STATIC9)->EnableWindow( FALSE );
					GetDlgItem(IDC_STATIC10)->EnableWindow( FALSE );
				}
			}
			break;

		case SCN_TKY_PN:	// 摘要
			(ctl+i)->INP_ena = TRUE;
			if( (ctl+i)->p_obj )
			{
				((CICSDBEDT *)(ctl+i)->p_obj)->ImeEndMode( TRUE );
				((CICSDBEDT *)(ctl+i)->p_obj)->SetMaxLen( (pDBzm->zvol->tk_ln*2) );	//文字数設定
			}
			break;

		case SNOT_DOCSEQ_PN:
		case SCN_DOCSEQ1_PN:
		case SCN_DOCSEQ2_PN:
		case SCN_DOCSHTP_PN:
			(ctl+i)->INP_ena = bSCANSAVE_Master ? TRUE : FALSE;

			if( (ctl+i)->p_obj ) {
				((CICSDBEDT *)(ctl+i)->p_obj)->EnableWindow( (ctl+i)->INP_ena );
			}
			else {
				GetDlgItem((ctl+i)->IDC_X)->EnableWindow( (ctl+i)->INP_ena );
				GetDlgItem((ctl+i)->IDC_X)->EnableWindow( (ctl+i)->INP_ena );
			}
			break;
		default:
			(ctl+i)->INP_ena = TRUE;
			break;
		}
	}

}



//科目名称選択画面を直前のポジションに従いリセット
void CScanKeyView::SelItemReset( int now_pn, int prev_pn )
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
	case SCN_DCBMN1_PN:	//	開始貸借部門
	case SCN_DCBMN2_PN:	//	終了貸借部門
		switch( now_pn ) {
		case SCN_BMON1_PN:	//	開始借方部門
		case SCN_BMON2_PN:	//	終了借方部門
		case SCN_CBMN1_PN:	//	開始貸方部門
		case SCN_CBMN2_PN:	//	終了貸方部門
		case SCN_DCBMN1_PN:	//	開始貸借部門
		case SCN_DCBMN2_PN:	//	終了貸借部門
			return;		
		}
		break;
	}

	// 工事番号
	switch( prev_pn ) {	
	case SCN_KOJI1_PN:	//	開始借方工事
	case SCN_KOJI2_PN:	//	終了借方工事
	case SCN_CKJI1_PN:	//	開始貸方工事
	case SCN_CKJI2_PN:	//	終了貸方工事
	case SCN_DCKJI1_PN:	//	開始貸借工事
	case SCN_DCKJI2_PN:	//	終了貸借工事
		switch( now_pn ) {
		case SCN_KOJI1_PN:	//	開始借方工事
		case SCN_KOJI2_PN:	//	終了借方工事
		case SCN_CKJI1_PN:	//	開始貸方工事
		case SCN_CKJI2_PN:	//	終了貸方工事
		case SCN_DCKJI1_PN:	//	開始貸借工事
		case SCN_DCKJI2_PN:	//	終了貸借工事
			return;		
		}
		break;
	}

	//ホームページにリセット
	SelPar par;

	if( now_pn == SCN_BMON1_PN || now_pn == SCN_BMON2_PN ||
		now_pn == SCN_CBMN1_PN || now_pn == SCN_CBMN2_PN ||
		now_pn == SCN_DCBMN1_PN || now_pn == SCN_DCBMN2_PN  ) {
		par.sel_pn = SL_BUMON_PN;
		m_Seldata.SelectJob( &par, VK_HOME );
	}
	else if( now_pn == SCN_KOJI1_PN || now_pn == SCN_KOJI2_PN ||
			now_pn == SCN_CKJI1_PN  || now_pn == SCN_CKJI2_PN ||
			now_pn == SCN_DCKJI1_PN || now_pn == SCN_DCKJI2_PN ) {
		par.sel_pn = SL_KOJI_PN;
		m_Seldata.SelectJob( &par, VK_HOME );
	}
	else if( now_pn != SCN_TKY_PN && now_pn != SCN_INVNO_PN )
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


void CScanKeyView::TermBuzzer()
{
}

BEGIN_EVENTSINK_MAP(CScanKeyView, CDBbaseView)
    //{{AFX_EVENTSINK_MAP(CScanKeyView)
	ON_EVENT(CScanKeyView, IDC_ICSSELCTRL1, 1 /* Selected */, OnSelectedIcsselctrl1, VTS_I2 VTS_I4)
	ON_EVENT(CScanKeyView, IDC_ICSSELCTRL2, 1 /* Selected */, OnSelectedIcsselctrl2, VTS_I2 VTS_I4)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT_RANGE(CScanKeyView, IDC_SCNSEQ1, IDC_SCNHUCMT, 3 /* SetFocus */, OnSetFocusScnDBedt, VTS_I4 )
	ON_EVENT_RANGE(CScanKeyView, IDC_SCNSEQ1, IDC_SCNHUCMT, 2 /* KillFocus */, OnKillFocusScnDBedt, VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CScanKeyView, IDC_SCNSEQ1, IDC_SCNHUCMT, 1 /* Termination */, OnTerminationScnDBedt, VTS_I4 VTS_I4 VTS_I4 VTS_I4)	
	ON_EVENT_RANGE(CScanKeyView, IDC_SCNSEQ1, IDC_SCNHUCMT, 5 /* ImeEndComposition */, OnImeEndCompositionScnDBedt, VTS_I4 VTS_I2 VTS_BSTR VTS_BSTR)
	ON_EVENT_RANGE(CScanKeyView, IDC_SCNSEQ1, IDC_SCNHUCMT, 4 /* KanaTermination */, OnKanaTerminationScnDBedt, VTS_I4 VTS_BSTR)
END_EVENTSINK_MAP()


void CScanKeyView::OnSetFocusScnDBedt( UINT ID )
{
	if( ! PROGRAM_OK )	return;

	switch( ID ) {
	case IDC_SCNSEQ1:	OnSetFocusScnseq1();	break;
	case IDC_SCNSEQ2: 	OnSetFocusScnseq2();	break;
//	case IDC_SCNDATE1:	OnSetFocusScndate1();	break;
//	case IDC_SCNDATE2:	OnSetFocusScndate2();	break;
	case IDC_SCNDENP1:	OnSetFocusScndenp1();	break;
	case IDC_SCNDENP2:	OnSetFocusScndenp2();	break;
	case IDC_SCNBMON1:	OnSetFocusScnbmon1();	break;
	case IDC_SCNBMON2:	OnSetFocusScnbmon2();	break;
	case IDC_SCNCBMN1:	OnSetFocusScncbmn1();	break;
	case IDC_SCNCBMN2:	OnSetFocusScncbmn2();	break;
	case IDC_SCNDCBMN1:	OnSetFocusScnDCbmn1();	break;
	case IDC_SCNDCBMN2:	OnSetFocusScnDCbmn2();	break;
	case IDC_SCNKOJI1:	OnSetFocusScnkoji1();	break;
	case IDC_SCNKOJI2:	OnSetFocusScnkoji2();	break;
	case IDC_SCNCKJI1:	OnSetFocusScnckji1();	break;
	case IDC_SCNCKJI2:	OnSetFocusScnckji2();	break;
	case IDC_SCNDCKJI1:	OnSetFocusScnDCkji1();	break;
	case IDC_SCNDCKJI2:	OnSetFocusScnDCkji2();	break;

	case IDC_SCNDEBT:	OnSetFocusScnDebt();	break;
	case IDC_SCNDEBT2:	OnSetFocusScnDebt2();	break;
	case IDC_SCNDBR1: 	OnSetFocusScndbr1();	break;
	case IDC_SCNDBR2: 	OnSetFocusScndbr2();	break;
	case IDC_SCNCRED: 	OnSetFocusScnCred();	break;
	case IDC_SCNCRED2: 	OnSetFocusScnCred2();	break;
	case IDC_SCNCBR1:	OnSetFocusScncbr1();	break; 
	case IDC_SCNCBR2:	OnSetFocusScncbr2();	break; 
	case IDC_SCNDBCR:  	OnSetFocusScnDBcr();	break;
	case IDC_SCNDBCR2:	OnSetFocusScnDBcr2();	break;
	case IDC_SCNDCBR1:	OnSetFocusScnDCbr1();	break; 
	case IDC_SCNDCBR2:	OnSetFocusScnDCbr2();	break;
	case IDC_SCNVAL1:	OnSetFocusScnval1();	break;
	case IDC_SCNVAL2:	OnSetFocusScnval2();	break;
	case IDC_SCNZEI1:	OnSetFocusScnzei1();	break; 
	case IDC_SCNZEI2:	OnSetFocusScnzei2();	break; 
	case IDC_SCNTAIKA1:	OnSetFocusScntai1();	break; 
	case IDC_SCNTAIKA2:	OnSetFocusScntai2();	break; 
	case IDC_SCNTKY: 	OnSetFocusScntky();		break; 
	case IDC_SCNSNUM1:	OnSetFocusScnSnum1();	 break;
	case IDC_SCNSNUM2:	OnSetFocusScnSnum2();	break;
	case IDC_SCNTDAT1:	OnSetFocusScnTgdate1();	break;
	case IDC_SCNTDAT2:	OnSetFocusScnTgdate2();	break;
	case IDC_SCNHUCMT:	OnSetFocusScnHUcmt();	break;

	case IDC_SCNSNO1:	OnSetFocusScnDocEvi1();	break;
	case IDC_SCNSNO2:	OnSetFocusScnDocEvi2();	break;
	case IDC_SCNDOCSEQ1:	OnSetFocusScnDocSeq1();	break;
	case IDC_SCNDOCSEQ2: 	OnSetFocusScnDocSeq2();	break;

		// インボイス対応
	case IDC_SCNINVNO:		// 登録番号
		OnSetFocusScnInvno();	
		m_InvnoDisp.ShowWindow(SW_HIDE);
		break;
	case IDC_SCNINVNODISP:
		set_focus(SCN_INVNO_PN);
		m_InvnoDisp.ShowWindow(SW_HIDE);
		break;
	default:
		ASSERT( FALSE );
		break;
	}
}

void CScanKeyView::OnKillFocusScnDBedt( UINT ID, long inplen )
{
	if( ! PROGRAM_OK )	return;

	switch( ID ) {
	case IDC_SCNSEQ1:	OnKillFocusScnseq1( inplen);	break;
	case IDC_SCNSEQ2: 	OnKillFocusScnseq2( inplen);	break;
//	case IDC_SCNDATE1:	OnKillFocusScndate1(inplen);	break;
//	case IDC_SCNDATE2:	OnKillFocusScndate2(inplen);	break;
	case IDC_SCNDENP1:	OnKillFocusScndenp1(inplen);	break;
	case IDC_SCNDENP2:	OnKillFocusScndenp2(inplen);	break;
	case IDC_SCNBMON1:	OnKillFocusScnbmon1(inplen);	break;
	case IDC_SCNBMON2:	OnKillFocusScnbmon2(inplen);	break;
	case IDC_SCNCBMN1:	OnKillFocusScncbmn1(inplen);	break;
	case IDC_SCNCBMN2:	OnKillFocusScncbmn2(inplen);	break;
	case IDC_SCNDCBMN1:	OnKillFocusScnDCbmn1(inplen);	break;
	case IDC_SCNDCBMN2:	OnKillFocusScnDCbmn2(inplen);	break;

	case IDC_SCNKOJI1:	OnKillFocusScnkoji1(inplen);	break;
	case IDC_SCNKOJI2:	OnKillFocusScnkoji2(inplen);	break;
	case IDC_SCNCKJI1:	OnKillFocusScnckji1(inplen);	break;
	case IDC_SCNCKJI2:	OnKillFocusScnckji2(inplen);	break;
	case IDC_SCNDCKJI1:	OnKillFocusScnDCkji1(inplen);	break;
	case IDC_SCNDCKJI2:	OnKillFocusScnDCkji2(inplen);	break;

	case IDC_SCNDEBT:	OnKillFocusScnDebt(inplen);	break;
	case IDC_SCNDEBT2:	OnKillFocusScnDebt2(inplen);	break;
	case IDC_SCNDBR1:	OnKillFocusScndbr1(inplen);	break;
	case IDC_SCNDBR2:	OnKillFocusScncbr2(inplen);	break;
	case IDC_SCNCRED:	OnKillFocusScnCred(inplen);		break;
	case IDC_SCNCRED2:	OnKillFocusScnCred2(inplen);	break;
	case IDC_SCNCBR1: 	OnKillFocusScncbr1(inplen);	break;
	case IDC_SCNCBR2: 	OnKillFocusScncbr2(inplen);	break;
	case IDC_SCNDBCR:	OnKillFocusScnDBcr(inplen);		break;
	case IDC_SCNDBCR2:	OnKillFocusScnDBcr2(inplen);	break;
	case IDC_SCNDCBR1:	OnKillFocusScnDCbr1(inplen);	break;
	case IDC_SCNDCBR2:	OnKillFocusScnDCbr2(inplen);	break;
	case IDC_SCNVAL1:	OnKillFocusScnval1(inplen);	break;
	case IDC_SCNVAL2:	OnKillFocusScnval2(inplen);	break;
	case IDC_SCNZEI1:	OnKillFocusScnzei1(inplen);	break; 
	case IDC_SCNZEI2:	OnKillFocusScnzei2(inplen);	break; 
	case IDC_SCNTAIKA1:	OnKillFocusScntai1(inplen);	break; 
	case IDC_SCNTAIKA2:	OnKillFocusScntai2(inplen);	break; 
	case IDC_SCNTKY: 	OnKillFocusScntky(inplen);	break; 
	case IDC_SCNSNUM1:	OnKillFocusScnSnum1(inplen);	break;
	case IDC_SCNSNUM2:	OnKillFocusScnSnum2(inplen);	break;	
	case IDC_SCNTDAT1:	OnKillFocusScnTgdate1(inplen);	break;
	case IDC_SCNTDAT2:	OnKillFocusScnTgdate2(inplen);	break;
	case IDC_SCNHUCMT:	OnKillFocusScnHUcmt(inplen);	break;

	case IDC_SCNSNO1:	OnKillFocusScnDocEvi1(inplen);	break;
	case IDC_SCNSNO2:	OnKillFocusScnDocEvi2(inplen);	break;
	case IDC_SCNDOCSEQ1:	OnKillFocusScnDocSeq1( inplen);	break;
	case IDC_SCNDOCSEQ2: 	OnKillFocusScnDocSeq2( inplen);	break;
		
		// インボイス対応
	case IDC_SCNINVNO:		// 登録番号
		OnKillFocusScnInvno( inplen );	
		if( !m_bIgnoreFocus ) {
			m_InvnoDisp.ShowWindow(SW_SHOW);
		}
		break;
	case IDC_SCNINVNODISP:
		break;
	default:
		ASSERT( FALSE );
		break;
	}
}

void CScanKeyView::OnTerminationScnDBedt( UINT ID, long nChar, long inplen, long kst )
{
	if( ! PROGRAM_OK )	return;

	switch( ID ) {
	case IDC_SCNSEQ1:	OnTerminationScnseq1(nChar,inplen,kst);	break;
	case IDC_SCNSEQ2: 	OnTerminationScnseq2(nChar,inplen,kst);	break;
//	case IDC_SCNDATE1:	OnTerminationScndate1(nChar,inplen,kst);	break;
//	case IDC_SCNDATE2:	OnTerminationScndate2(nChar,inplen,kst);	break;
	case IDC_SCNDENP1:	OnTerminationScndenp1(nChar,inplen,kst);	break;
	case IDC_SCNDENP2:	OnTerminationScndenp2(nChar,inplen,kst);	break;
	case IDC_SCNBMON1:	OnTerminationScnbmon1(nChar,inplen,kst);	break;
	case IDC_SCNBMON2:	OnTerminationScnbmon2(nChar,inplen,kst);	break;
	case IDC_SCNCBMN1:	OnTerminationScncbmn1(nChar,inplen,kst);	break;
	case IDC_SCNCBMN2:	OnTerminationScncbmn2(nChar,inplen,kst);	break;
	case IDC_SCNDCBMN1:	OnTerminationScnDCbmn1(nChar,inplen,kst);	break;
	case IDC_SCNDCBMN2:	OnTerminationScnDCbmn2(nChar,inplen,kst);	break;

	case IDC_SCNKOJI1:	OnTerminationScnkoji1(nChar,inplen,kst);	break;
	case IDC_SCNKOJI2:	OnTerminationScnkoji2(nChar,inplen,kst);	break;
	case IDC_SCNCKJI1:	OnTerminationScnckji1(nChar,inplen,kst);	break;
	case IDC_SCNCKJI2:	OnTerminationScnckji2(nChar,inplen,kst);	break;
	case IDC_SCNDCKJI1:	OnTerminationScnDCkji1(nChar,inplen,kst);	break;
	case IDC_SCNDCKJI2:	OnTerminationScnDCkji2(nChar,inplen,kst);	break;

	case IDC_SCNDEBT:	OnTerminationScnDebt(nChar,inplen,kst);		break;
	case IDC_SCNDEBT2:	OnTerminationScnDebt2(nChar,inplen,kst);	break;
	case IDC_SCNDBR1:	TerminationBrn(SCN_DBR1_PN,nChar,inplen,kst);	break;
	case IDC_SCNDBR2:	TerminationBrn(SCN_DBR2_PN,nChar,inplen,kst);	break;
	case IDC_SCNCRED: 	OnTerminationScnCred(nChar,inplen,kst);		break;
	case IDC_SCNCRED2: 	OnTerminationScnCred2(nChar,inplen,kst);	break;
	case IDC_SCNCBR1:	TerminationBrn(SCN_CBR1_PN,nChar,inplen,kst);	break;
	case IDC_SCNCBR2:	TerminationBrn(SCN_CBR2_PN,nChar,inplen,kst);	break;
	case IDC_SCNDBCR: 	OnTerminationScnDBcr(nChar,inplen,kst);		break;
	case IDC_SCNDBCR2: 	OnTerminationScnDBcr2(nChar,inplen,kst);	break;
	case IDC_SCNDCBR1: 	TerminationBrn(SCN_DCBR1_PN,nChar,inplen,kst);	break;
	case IDC_SCNDCBR2:	TerminationBrn(SCN_DCBR2_PN,nChar,inplen,kst);	break;
	case IDC_SCNVAL1:	OnTerminationScnval1(nChar,inplen,kst);	break;
	case IDC_SCNVAL2:	OnTerminationScnval2(nChar,inplen,kst);	break;
	case IDC_SCNZEI1: 	OnTerminationScnzei1(nChar,inplen,kst);	break;
	case IDC_SCNZEI2: 	OnTerminationScnzei2(nChar,inplen,kst);	break;
	case IDC_SCNTAIKA1: OnTerminationScntai1(nChar,inplen,kst);	break;
	case IDC_SCNTAIKA2: OnTerminationScntai2(nChar,inplen,kst);	break;
	case IDC_SCNTKY:	OnTerminationScntky(nChar,inplen,kst);	break;
	case IDC_SCNSNUM1:	OnTerminationScnSnum1(nChar,inplen,kst);	break;
	case IDC_SCNSNUM2:	OnTerminationScnSnum2(nChar,inplen,kst);	break;
	case IDC_SCNTDAT1:	OnTerminationScnTgdate1(nChar,inplen,kst);	break;
	case IDC_SCNTDAT2:	OnTerminationScnTgdate2(nChar,inplen,kst);	break;
	case IDC_SCNHUCMT:	OnTerminationScnHUcmt(nChar,inplen,kst);	break;

	case IDC_SCNSNO1:	OnTerminationScnDocEvi1(nChar,inplen,kst);	break;
	case IDC_SCNSNO2:	OnTerminationScnDocEvi2(nChar,inplen,kst);	break;
	case IDC_SCNDOCSEQ1:	OnTerminationScnDocSeq1(nChar,inplen,kst);	break;
	case IDC_SCNDOCSEQ2: 	OnTerminationScnDocSeq2(nChar,inplen,kst);	break;

		// インボイス対応
	case IDC_SCNINVNO:	OnTerminationScnInvno( nChar, inplen, kst );	break;	// 登録番号
	case IDC_SCNINVNODISP:

		break;
	default:
		ASSERT( FALSE );
		break;
	}
}


void CScanKeyView::OnImeEndCompositionScnDBedt(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring) 
{
	if( ! PROGRAM_OK )	return;
	
	int term, pos;
	int len, st, tky_sgn;
	char tmp[128] = {0}, ystr[128] = {0};
	VARIANT var;

	switch( ID ) {
	case IDC_SCNDEBT:
		if( !lastBrnInpStr((char*)string, tmp, sizeof tmp ) )
			return;
		if( ItemDataCheck( SCN_DEBT_PN, nChar, tmp, 0 ) <= 0 )
			return;
		OnTerminationScnDebt( nChar, 0, 0 );
		break;
	case IDC_SCNDEBT2:
		if( !lastBrnInpStr((char*)string, tmp, sizeof tmp) )
			return;
		if( ItemDataCheck( SCN_DEBT2_PN, nChar, tmp, 0 ) <= 0 )
			return;
		OnTerminationScnDebt2( nChar, 0, 0 );
		break;

	case IDC_SCNCRED:
		if( !lastBrnInpStr((char*)string, tmp, sizeof tmp) )
			return;
		if( ItemDataCheck( SCN_CRED_PN, nChar, tmp, 1 ) <= 0 )
			return;
		OnTerminationScnCred( nChar, 0, 0 );
		break;
	case IDC_SCNCRED2:
		if( !lastBrnInpStr((char*)string, tmp, sizeof tmp) )
			return;
		if( ItemDataCheck( SCN_CRED2_PN, nChar, tmp, 1 ) <= 0 )
			return;
		OnTerminationScnCred2( nChar, 0, 0 );
		break;

	case IDC_SCNDBCR:
		if( !lastBrnInpStr((char*)string, tmp, sizeof tmp) )
			return;
		if( ItemDataCheck( SCN_DBCR_PN, nChar, tmp, 1 ) <= 0 )
			return;
		OnTerminationScnDBcr( nChar, 0, 0 );
		break;
	case IDC_SCNDBCR2:
		if( !lastBrnInpStr((char*)string, tmp, sizeof tmp) )
			return;
		if( ItemDataCheck( SCN_DBCR2_PN, nChar, tmp, 1 ) <= 0 )
			return;
		OnTerminationScnDBcr2( nChar, 0, 0 );
		break;

	case IDC_SCNBMON1:
	case IDC_SCNBMON2:
	case IDC_SCNCBMN1:
	case IDC_SCNCBMN2:
	case IDC_SCNDCBMN1:
	case IDC_SCNDCBMN2:
		if( CheckImeEndBmon( ID, nChar, string ) != 0 )
			return;

		switch( ID ) {
		case IDC_SCNBMON1:	OnTerminationScnbmon1( nChar, 1, 0 );	break;
		case IDC_SCNBMON2:	OnTerminationScnbmon2( nChar, 1, 0 );	break;
		case IDC_SCNCBMN1:	OnTerminationScncbmn1( nChar, 1, 0 );	break;
		case IDC_SCNCBMN2:	OnTerminationScncbmn2( nChar, 1, 0 );	break;
		case IDC_SCNDCBMN1:	OnTerminationScnDCbmn1( nChar, 1, 0 );	break;
		case IDC_SCNDCBMN2:	OnTerminationScnDCbmn2( nChar, 1, 0 );	break;
		}
		break;


	case IDC_SCNKOJI1:
	case IDC_SCNKOJI2:
	case IDC_SCNCKJI1:
	case IDC_SCNCKJI2:
	case IDC_SCNDCKJI1:
	case IDC_SCNDCKJI2:
		if( CheckImeEndKoji( ID, nChar, string ) != 0 )
			return;

		switch( ID ) {
		case IDC_SCNKOJI1:	OnTerminationScnkoji1( nChar, 1, 0 );	break;
		case IDC_SCNKOJI2:	OnTerminationScnkoji2( nChar, 1, 0 );	break;
		case IDC_SCNCKJI1:	OnTerminationScnckji1( nChar, 1, 0 );	break;
		case IDC_SCNCKJI2:	OnTerminationScnckji2( nChar, 1, 0 );	break;
		case IDC_SCNDCKJI1:	OnTerminationScnDCkji1( nChar, 1, 0 );	break;
		case IDC_SCNDCKJI2:	OnTerminationScnDCkji2( nChar, 1, 0 );	break;
		}
		break;

	case IDC_SCNDBR1:
	case IDC_SCNDBR2:
	case IDC_SCNCBR1:
	case IDC_SCNCBR2:
	case IDC_SCNDCBR1:
	case IDC_SCNDCBR2:
		if( !lastBrnInpStr((char*)string, tmp, sizeof tmp) )
			return;

		CheckImeEndBranch( ID, nChar, tmp);
		{
			int pn = GetPositionFromSkeyID(ID);
			TerminationBrn( pn, nChar, 1, 0 );
		}
		break;

	case IDC_SCNINVNO:
		if( !lastBrnInpStr(string, tmp, sizeof tmp) )
			return;

		if( CheckDataDBeditInv(nChar, tmp) <= 0 )
			return;

		OnTerminationScnInvno(nChar, 1, 0);
		break;

	case IDC_SCNTKY:
		term	= 1;
		tky_sgn	= 0;
		len = get_strcnt( (char*)string, Voln1->tk_ln * 2 );
		memcpy( tmp, string, len );
		tmp[len] = '\0';

		memcpy( ystr, ystring, len );
		ystr[len] = '\0';

		if( (st = TekiyoStr_Chk( tmp )) > 0 ) {
			if( TekiyoKeyMode() ) {	
				// 摘要コード＋Enter で 摘要をセット
				if(	st == 1 ) {
					if( nChar == VK_RETURN ) {
						tky_sgn = 1;
					}
				}
				if( nChar == VK_TAB || nChar == VK_F12 ) {
					// カナ検索
					if( st == 2 ) {
						tky_sgn = 1;
					}
				}
			}
			else {
				// 番号摘要セット ＋ カナ検索
				if( nChar == VK_TAB || nChar == VK_F12 ) {
					tky_sgn = 1;
				}
			}
			if( tky_sgn ) {
				OnKanaTermScntky( ystring );
				term = 0;
			}
		}
		// 文字列をコントロールにセットする
		if( term ) {
			var.pbVal = (BYTE*)tmp;
			pos = ((CICSDBEDT*)GetDlgItem(ID))->GetCaretPosition();
			((CICSDBEDT*)GetDlgItem(ID))->InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );
		}

		break;
	default:
		break;
	}
}


void CScanKeyView::OnKanaTerminationScnDBedt(UINT ID, LPCTSTR Kana) 
{
	if( ! PROGRAM_OK )	return;

	if( KanaSearchType() )
		return;

	if( EdabanKanaSearch(ID, Kana) )
		return;

	ItemKanaSearch( Kana );
}


int CScanKeyView::EdabanKanaSearch(UINT ID, LPCTSTR data)
{
	int eda_inp = 0;

	switch( ID ) {
	case IDC_SCNDBR1: 
	case IDC_SCNDBR2:
	case IDC_SCNCBR1:
	case IDC_SCNCBR2:
	case IDC_SCNDCBR1:
	case IDC_SCNDCBR2:
		eda_inp = 1;
		break;
	}

	if( ! eda_inp )		return 0;

	if( !BRNTKY_sel )	return 1;

	char kana[10] = {0}, tmp[36] = {0};
	kana_copy( kana, data );

	SelPar par;

	par.sel_pn = SL_BRNTKY_PN;
	par.sel_flg = 1;

	BRNTKY_PAR brp;
	brp = m_Seldata.GetBrnTkyPar();

	par.tkybrn	= brp.bt_code;
	par.brn_bmn = brp.bt_bmn;
	// 絞込み
	if( BRNTKY_sel ) {
		sprintf_s( par.brn_kana, sizeof par.brn_kana, "%s%s", saveITEM50kana, kana );
	}
	else {
		sprintf_s( par.brn_kana, sizeof par.brn_kana, "%s", kana );
	}

	int st = m_Seldata.SelectJob( &par, -1 );

	::ZeroMemory( saveITEM50kana, sizeof saveITEM50kana );
	if( ! BRNTKY_sel ) {
		Buzzer();
		::ZeroMemory( par.brn_kana, sizeof par.brn_kana );
		int st = m_Seldata.SelectJob( &par, -1 );
	}

	return 1;
}


// 部門番号IMEのEND処理
int CScanKeyView::CheckImeEndBmon( UINT ID, long nChar, LPCTSTR string ) 
{
	VARIANT var;

	CString buf = string;
	int		ret = 0;

	if( nChar == VK_TAB || nChar == VK_F12 ) {
		// カナ検索
		if( ! isdigitstr( (char*)string ) ) {
			SelPar par;
			::ZeroMemory( par.kmk_kana, sizeof par.kmk_kana );
			par.sel_pn = SL_BMNKANA_PN;
			memcpy( par.kmk_kana, string, sizeof par.kmk_kana );

			m_Seldata.SelectJob( &par, -1 );
			ret = 1;
		}
		else {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();

			DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else {
		var.pbVal = (BYTE*)buf.GetBuffer(64);
		buf.ReleaseBuffer();

		DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	return ret;
}



// 工事番号IMEのEND処理
int CScanKeyView::CheckImeEndKoji( UINT ID, long nChar, LPCTSTR string ) 
{
	VARIANT var;
	int		ret = 0;

	if( nChar == VK_TAB || nChar == VK_F12 ) {
		// カナ検索
		if( ! isdigitstr( (char*)string ) ) {
			SelPar par;
			::ZeroMemory( par.kmk_kana, sizeof par.kmk_kana );
			par.sel_pn = SL_KOJIKANA_PN;
			memcpy( par.kmk_kana, string, sizeof par.kmk_kana );

			m_Seldata.SelectJob( &par, -1 );
			ret = 1;
		}
	}
	else {
		CString buf = string;

		kana_chk( &buf, buf.GetLength() );

		var.pbVal = (BYTE*)buf.GetBuffer(64);
		buf.ReleaseBuffer();

		DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	return ret;
}


//////////////////////////////////////////////////////////////////////
// seq
void CScanKeyView::OnSetFocusScnseq1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_SEQ1_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_SEQ1_PN ); 
}

void CScanKeyView::OnKillFocusScnseq1( long inplen ) 
{
	if( m_DIALOG_OK == FALSE )
		return;

	SeqInp( SCN_SEQ1_PN, &SCAN_KEY.SCN_SEQ1_DT, 0, inplen, 0);
}

void CScanKeyView::OnTerminationScnseq1(long nChar, long inplen, long kst) 
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
			TermBuzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

void CScanKeyView::OnSetFocusScnseq2() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_SEQ2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_SEQ2_PN ); 
}

void CScanKeyView::OnKillFocusScnseq2( long inplen ) 
{
	if( m_DIALOG_OK == FALSE )
		return;

	SeqInp( SCN_SEQ2_PN, &SCAN_KEY.SCN_SEQ2_DT, 0, inplen, 0);
}

void CScanKeyView::OnTerminationScnseq2(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = SeqInp( SCN_SEQ2_PN, &SCAN_KEY.SCN_SEQ2_DT, nChar, inplen, kst )) )
			return;

		if( term_focus( SCN_SEQ2_PN, nChar ) != 0 )
			TermBuzzer();
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}


//
// スキャンキーＡｌｌクリア
void CScanKeyView::ScanKeyAllDel()
{
	PROGRAM_OK = FALSE;	// KillFocus させない

	int st = myICSMessageBox( "全検索項目の消去を行います。\n\nよろしいですか。", MB_YESNO );

	if( st == IDYES ) {
		CButton* btn;
		btn = (CButton*)GetDlgItem( IDC_FURI_CHK );
		btn->SetCheck(0);
		btn = (CButton*)GetDlgItem( IDC_NYUK_CHK );
		btn->SetCheck(0);
		btn = (CButton*)GetDlgItem( IDC_SYUK_CHK );
		btn->SetCheck(0);

TRACE( "@F11 ScanKeyAllDel \n" );
		SC_Tky.RemoveAll();
		m_Report.RemoveAllRecords();

		m_Tky.SetBackColor( RGB_WHITE );

		keyline_inz();	// コントロールテーブルの初期化＆表示
	}

	PROGRAM_OK = TRUE;
}

//
//	スキャンキー項目のクリア
//
void CScanKeyView::ScanKeyClear( BOOL bMust/*= FALSE */)
{
	if( ! bMust ) {
		if( pAUTOSEL == NULL )
			return;

		if( pAUTOSEL->SCNCLR_OPT != OPT_SCNCLR )
			return;
	}

	CButton* btn;
	btn = (CButton*)GetDlgItem( IDC_FURI_CHK );
	btn->SetCheck(0);
	btn = (CButton*)GetDlgItem( IDC_NYUK_CHK );
	btn->SetCheck(0);
	btn = (CButton*)GetDlgItem( IDC_SYUK_CHK );
	btn->SetCheck(0);

	SC_Tky.RemoveAll();
	m_Tky.SetBackColor( RGB_WHITE );

	keyline_inz();	// コントロールテーブルの初期化＆表示
}

//
// ＳＥＱ番号　入力＆オフフォーカス
long CScanKeyView::SeqInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
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
		else {
			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
			keybuf->IP_SEQ = 0;
		}

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
void CScanKeyView::OnSetFocusScndate1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_DATE1_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_DATE1_PN ); 
}

void CScanKeyView::OnKillFocusScndate1( long inplen ) 
{
	if( m_DIALOG_OK == FALSE )
		return;

	DateInp( SCN_DATE1_PN, &SCAN_KEY.SCN_DATE1_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScndate1(long nChar, long inplen, long kst) 
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
			TermBuzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

void CScanKeyView::OnSetFocusScndate2() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_DATE2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_DATE2_PN ); 	
}

void CScanKeyView::OnKillFocusScndate2( long inplen ) 
{
	if( m_DIALOG_OK == FALSE )
		return;

	DateInp( SCN_DATE2_PN, &SCAN_KEY.SCN_DATE2_DT, 0, inplen, 0 );	
}

void CScanKeyView::OnTerminationScndate2(long nChar, long inplen, long kst) 
{
//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = DateInp( SCN_DATE2_PN, &SCAN_KEY.SCN_DATE2_DT, nChar, inplen, kst )) )
			return;

		//カーソル移動
		if( term_focus( SCN_DATE2_PN, nChar ) != 0 )
			TermBuzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

// 日付チェックデータ
long CScanKeyView::CheckDataIcsinputDate( int pn, long nChar ) 
{
	char check[3] = {0};
	char ymd[3] = {0};
	char data[20] = {0};
	BYTE bcd[10] = {0};
	VARIANT var;
	CString str;

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
//		if( /* !pDBzm->vd_chek( (unsigned char*)check ) && !pDBzm->vd_sign( (unsigned char*)check, (unsigned char*)ymd ) */ )
//		if( !pDBzm->Myvd_chek( (unsigned char*)check ) && !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd ) )
		if( /*!vd.db_vd_check( (BYTE*)check, pDBzm ) &&*/ ! Myvd_sign( (unsigned char*)check, (unsigned char*)ymd, pDBzm ) )
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
long CScanKeyView::DateInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check = {0};
	VARIANT var;
	CString str;

	// 日付文字列チェック＆セット
	if( inplen > 0 || nChar == 0x16 )	CheckDataIcsinputDate( pn, nChar );
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
void CScanKeyView::OnSetFocusScndenp1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_DENP1_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_DENP1_PN ); 	
}

void CScanKeyView::OnKillFocusScndenp1( long inplen ) 
{
	DenpInp( SCN_DENP1_PN, &SCAN_KEY.SCN_DENP1_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScndenp1(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

TRACE( "@F11 OnTerminationScndenp1 nChar = %d\n", nChar );

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = DenpInp( SCN_DENP1_PN, &SCAN_KEY.SCN_DENP1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_DENP1_PN, nChar ) != 0 )
			TermBuzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

void CScanKeyView::OnSetFocusScndenp2() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_DENP2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_DENP2_PN );	
}

void CScanKeyView::OnKillFocusScndenp2( long inplen ) 
{
	DenpInp( SCN_DENP2_PN, &SCAN_KEY.SCN_DENP2_DT, 0, inplen, 0 );	
}

void CScanKeyView::OnTerminationScndenp2(long nChar, long inplen, long kst) 
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
			TermBuzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}	
}

// 伝票番号　入力＆オフフォーカス
long CScanKeyView::DenpInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
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

void CScanKeyView::ScnbmonFocusSub( int pn )
{
	if( m_DIALOG_OK == FALSE )
		return;

	SelPar par;

//	try
	{
		// 選択部分に 部門名称を表示
		switch( get_nowpn() ) {
		case SCN_BMON1_PN:
		case SCN_BMON2_PN:
		case SCN_CBMN1_PN:
		case SCN_CBMN2_PN:
		case SCN_DCBMN1_PN:
		case SCN_DCBMN2_PN:
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

void CScanKeyView::OnSetFocusScnbmon1() 
{
	ScnbmonFocusSub( SCN_BMON1_PN );
}
void CScanKeyView::OnKillFocusScnbmon1( long inplen ) 
{
	BmonInp( SCN_BMON1_PN, &SCAN_KEY.SCN_BMON1_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnbmon1(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = BmonInp( SCN_BMON1_PN, &SCAN_KEY.SCN_BMON1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_BMON1_PN, nChar ) != 0 )
			TermBuzzer();

//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

void CScanKeyView::OnSetFocusScnbmon2() 
{
	ScnbmonFocusSub( SCN_BMON2_PN );
}
void CScanKeyView::OnKillFocusScnbmon2( long inplen ) 
{
	BmonInp( SCN_BMON2_PN, &SCAN_KEY.SCN_BMON2_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnbmon2(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = BmonInp( SCN_BMON2_PN, &SCAN_KEY.SCN_BMON2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_BMON2_PN, nChar ) != 0 )
			TermBuzzer();

//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

//////////////////////////////////////////////////////////////////////////////////////
// 貸方部門
void CScanKeyView::OnSetFocusScncbmn1() 
{
	ScnbmonFocusSub( SCN_CBMN1_PN );
}

void CScanKeyView::OnKillFocusScncbmn1( long inplen ) 
{
	BmonInp( SCN_CBMN1_PN, &SCAN_KEY.SCN_CBMN1_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScncbmn1(long nChar, long inplen, long kst) 
{
//	try 
	{
		// 入力＆オフフォーカス
		if( !(nChar = BmonInp( SCN_CBMN1_PN, &SCAN_KEY.SCN_CBMN1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_CBMN1_PN, nChar ) != 0 )
			TermBuzzer();

	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

void CScanKeyView::OnSetFocusScncbmn2() 
{
	ScnbmonFocusSub( SCN_CBMN2_PN );

}

void CScanKeyView::OnKillFocusScncbmn2( long inplen ) 
{
	BmonInp( SCN_CBMN2_PN, &SCAN_KEY.SCN_CBMN2_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScncbmn2(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = BmonInp( SCN_CBMN2_PN, &SCAN_KEY.SCN_CBMN2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_CBMN2_PN, nChar ) != 0 )
			TermBuzzer();

//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

//////////////////////////////////////////////////////////////////////////////////////
// 貸借部門
void CScanKeyView::OnSetFocusScnDCbmn1() 
{
	ScnbmonFocusSub( SCN_DCBMN1_PN );
}

void CScanKeyView::OnKillFocusScnDCbmn1( long inplen ) 
{
	BmonInp( SCN_DCBMN1_PN, &SCAN_KEY.SCN_DCBMN1_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnDCbmn1(long nChar, long inplen, long kst) 
{
//	try 
	{
		// 入力＆オフフォーカス
		if( !(nChar = BmonInp( SCN_DCBMN1_PN, &SCAN_KEY.SCN_DCBMN1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_DCBMN1_PN, nChar ) != 0 )
			TermBuzzer();
	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

void CScanKeyView::OnSetFocusScnDCbmn2() 
{
	ScnbmonFocusSub( SCN_DCBMN2_PN );

}

void CScanKeyView::OnKillFocusScnDCbmn2( long inplen ) 
{
	BmonInp( SCN_DCBMN2_PN, &SCAN_KEY.SCN_DCBMN2_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnDCbmn2(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = BmonInp( SCN_DCBMN2_PN, &SCAN_KEY.SCN_DCBMN2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_DCBMN2_PN, nChar ) != 0 )
			TermBuzzer();

//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}



// 部門　入力＆オフフォーカス
long CScanKeyView::BmonInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	CDBINPDataRec temp;	// 部門コード取得用
	int number;
	int n = 0;
	int ret = 0;
	char str[60];
	VARIANT var;
	BOOL bScrnNo;	//:TRUE 部門選択 画面番号優先

	bScrnNo = (pAUTOSEL->BMNSEL_OPT == OPT_BMNSEL);

	if( inplen ) {
		//データリード
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		n = strlen( (char*)var.pbVal );
		check.IP_BMN = -1;

#ifdef _2KT_BSEL	// 10.01 /02
		if( n && n < 3 && bScrnNo ) {
			number = atoi( (char*)var.pbVal );

			if( bmncode_todata( &temp, number, 0 ) != FALSE ) {
				check.IP_BMN = temp.m_dbmn;
			}
			else {
				// 選択ミス---データセット(再表示)
				check.IP_BMN = keybuf->IP_BMN;
				ret = -1;
			}
		}
		else {
#endif
			if( strlen( (char*)var.pbVal ) )
			{
				if( ! bScrnNo && ( nChar == VK_TAB && !(kst & ICSDBEDT_KST_SHIFT)) ) {
					number = atoi( (char*)var.pbVal );
					// 部門番号優先時は、Tab で選択画面より取得
					if( bmncode_todata( &temp, number, 0 ) != FALSE ) {
						check.IP_BMN = temp.m_dbmn;
					}
					else {
						// 選択ミス---データセット(再表示)
						check.IP_BMN = keybuf->IP_BMN;
						ret = -1;
					}
				}
				else {
					check.IP_BMN = atoi( (char*)var.pbVal );
				}
			}
#ifdef _2KT_BSEL	// 10.01 /02
		}
#endif

		if( check.IP_BMN != -1 ) {
			pDBzm->BumonCodeToStr( str, sizeof str, check.IP_BMN );
			var.pbVal = (BYTE*)str;
		}
		else {
			var.pbVal = NULL;
		}
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
		else {
			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
			keybuf->IP_BMN = -1;
		}
			
		//データセット
		if( keybuf->IP_BMN != -1 ) {
			pDBzm->BumonCodeToStr( str, sizeof str, keybuf->IP_BMN );
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

		if( ! (nChar = SelBmonDsp( nChar )) ) 
			return 0;
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
						pDBzm->BumonCodeToStr( str, keybuf->IP_BMN );
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

void CScanKeyView::ScnKojiFocusSub( int pn )
{
	if( m_DIALOG_OK == FALSE )
		return;

	SelPar	par;

//	try
	{
		// 選択部分に 工事名称を表示
		switch( get_nowpn() ) {
		case SCN_KOJI1_PN:
		case SCN_KOJI2_PN:
		case SCN_CKJI1_PN:
		case SCN_CKJI2_PN:
		case SCN_DCKJI1_PN:
		case SCN_DCKJI2_PN:
			break;
		default:
			par.sel_pn = SL_KOJI_PN;
			m_Seldata.SelectJob( &par, -1 );
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


void CScanKeyView::OnSetFocusScnkoji1() 
{
	ScnKojiFocusSub( SCN_KOJI1_PN );
}
void CScanKeyView::OnKillFocusScnkoji1( long inplen ) 
{
	KojiInp( SCN_KOJI1_PN, &SCAN_KEY.SCN_KOJI1_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnkoji1(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = KojiInp( SCN_KOJI1_PN, &SCAN_KEY.SCN_KOJI1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_KOJI1_PN, nChar ) != 0 )
			TermBuzzer();

//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

void CScanKeyView::OnSetFocusScnkoji2() 
{
	ScnKojiFocusSub( SCN_KOJI2_PN );
}
void CScanKeyView::OnKillFocusScnkoji2( long inplen ) 
{
	KojiInp( SCN_KOJI2_PN, &SCAN_KEY.SCN_KOJI2_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnkoji2(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = KojiInp( SCN_KOJI2_PN, &SCAN_KEY.SCN_KOJI2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_KOJI2_PN, nChar ) != 0 )
			TermBuzzer();


//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 貸方工事
void CScanKeyView::OnSetFocusScnckji1() 
{
	ScnKojiFocusSub( SCN_CKJI1_PN );
}
void CScanKeyView::OnKillFocusScnckji1( long inplen ) 
{
	KojiInp( SCN_CKJI1_PN, &SCAN_KEY.SCN_CKJI1_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnckji1(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = KojiInp( SCN_CKJI1_PN, &SCAN_KEY.SCN_CKJI1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_CKJI1_PN, nChar ) != 0 )
			TermBuzzer();
//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

void CScanKeyView::OnSetFocusScnckji2() 
{
	ScnKojiFocusSub( SCN_CKJI2_PN );
}
void CScanKeyView::OnKillFocusScnckji2( long inplen ) 
{
	KojiInp( SCN_CKJI2_PN, &SCAN_KEY.SCN_CKJI2_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnckji2(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = KojiInp( SCN_CKJI2_PN, &SCAN_KEY.SCN_CKJI2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_CKJI2_PN, nChar ) != 0 )
			TermBuzzer();


//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}


////////////////////////////////////////////////////////////////
// 貸借工事
void CScanKeyView::OnSetFocusScnDCkji1() 
{
	ScnKojiFocusSub( SCN_DCKJI1_PN );
}
void CScanKeyView::OnKillFocusScnDCkji1( long inplen ) 
{
	KojiInp( SCN_DCKJI1_PN, &SCAN_KEY.SCN_DCKJI1_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnDCkji1(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = KojiInp( SCN_DCKJI1_PN, &SCAN_KEY.SCN_DCKJI1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_DCKJI1_PN, nChar ) != 0 )
			TermBuzzer();
//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}

void CScanKeyView::OnSetFocusScnDCkji2() 
{
	ScnKojiFocusSub( SCN_DCKJI2_PN );
}
void CScanKeyView::OnKillFocusScnDCkji2( long inplen ) 
{
	KojiInp( SCN_DCKJI2_PN, &SCAN_KEY.SCN_DCKJI2_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnDCkji2(long nChar, long inplen, long kst) 
{
//	try {
		// 入力＆オフフォーカス
		if( !(nChar = KojiInp( SCN_DCKJI2_PN, &SCAN_KEY.SCN_DCKJI2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_DCKJI2_PN, nChar ) != 0 )
			TermBuzzer();
//	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}	
}


// 工事　入力＆オフフォーカス
long CScanKeyView::KojiInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	CDBINPDataRec temp;	// 工事コード取得用
	int number;
	int n = 0;
	int ret = 0;
	char str[60];
	VARIANT var;
	CString kjcd;
	BOOL bScrnNo;	//:TRUE 工事画面番号優先

	bScrnNo = (pAUTOSEL->BMNSEL_OPT == OPT_BMNSEL);

	if( inplen ) {
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		n = strlen( (char*)var.pbVal );
		::ZeroMemory( check.IP_KOJI, sizeof check.IP_KOJI );

#ifdef _2KT_BSEL	// 10.01 /02
		if( n && n < 3 && bScrnNo ) {
			number = atoi( (char*)var.pbVal );

			if( kojicode_todata( &temp, NULL, number, 0 ) != FALSE ) {
				temp.m_dkno.TrimRight();
				strcpy_s( (char*)check.IP_KOJI, sizeof check.IP_KOJI, temp.m_dkno );
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
				if( ! bScrnNo && (nChar == VK_TAB && !(kst & ICSDBEDT_KST_SHIFT)) ) {
					number = atoi( (char*)var.pbVal );

					if( kojicode_todata( &temp, NULL, number, 0 ) != FALSE ) {
						temp.m_dkno.TrimRight();
						strcpy_s( (char*)check.IP_KOJI, sizeof check.IP_KOJI, temp.m_dkno );
					}
					else {
						// 選択ミス//データセット(再表示)		
						strcpy_s( (char*)check.IP_KOJI, sizeof check.IP_KOJI, (char*)keybuf->IP_KOJI );
						ret = -1;
					}
				}
				else {
					strcpy_s( (char*)check.IP_KOJI, sizeof check.IP_KOJI, (char*)var.pbVal );
				}
			}
#ifdef _2KT_BSEL	// 10.01 /02
		}
#endif

		if( check.IP_KOJI[0] != '\0' ) {
			pDBzm->KojiCodeToStr( str, sizeof str, (char*)check.IP_KOJI );
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

		SelKojiDsp( nChar );
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{
#ifndef _2KT_BSEL	// 10.01 /02
		if( nChar == VK_TAB ) {		// 選択画面より番号選択
			if( strlen( data ) ) {
				number = atoi( data );

				if( kojicode_todata( &temp, number ) != FALSE ) {
					tmpdt.m_dkno.TrimRight();
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
void CScanKeyView::OnSetFocusScnDebt() 
{
	if( m_DIALOG_OK == FALSE )
		return;
//	try
	{

		set_nowpn( SCN_DEBT_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_DEBT_PN );
}

void CScanKeyView::OnKillFocusScnDebt( long inplen ) 
{
	ItemInp( SCN_DEBT_PN, &SCAN_KEY.SCN_DEBT_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnDebt(long nChar, long inplen, long kst )
{
	if( m_DIALOG_OK == FALSE )
		return;

	KamokuTermJob( SCN_DEBT_PN, nChar, inplen, kst );
}

// 終了 -------------------
void CScanKeyView::OnSetFocusScnDebt2() 
{
	if( m_DIALOG_OK == FALSE )
		return;
//	try
	{

		set_nowpn( SCN_DEBT2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_DEBT2_PN );
}

void CScanKeyView::OnKillFocusScnDebt2( long inplen ) 
{
	ItemInp( SCN_DEBT2_PN, &SCAN_KEY.SCN_DEBT2_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnDebt2(long nChar, long inplen, long kst )
{
	if( m_DIALOG_OK == FALSE )
		return;

	KamokuTermJob( SCN_DEBT2_PN, nChar, inplen, kst );
}


/////////////////////////////////////////////////////////////////////////////////////
// 貸方科目
void CScanKeyView::OnSetFocusScnCred() 
{
	if( m_DIALOG_OK == FALSE )
		return;
//	try
	{

		set_nowpn( SCN_CRED_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_CRED_PN );
}

void CScanKeyView::OnKillFocusScnCred( long inplen ) 
{
	ItemInp( SCN_CRED_PN, &SCAN_KEY.SCN_CRED_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnCred(long nChar, long inplen, long kst )
{
	if( m_DIALOG_OK == FALSE )
		return;

	KamokuTermJob( SCN_CRED_PN, nChar, inplen, kst );
}

// 終了 -------------------
void CScanKeyView::OnSetFocusScnCred2() 
{
	if( m_DIALOG_OK == FALSE )
		return;
//	try
	{

		set_nowpn( SCN_CRED2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_CRED2_PN );
}

void CScanKeyView::OnKillFocusScnCred2( long inplen ) 
{
	ItemInp( SCN_CRED2_PN, &SCAN_KEY.SCN_CRED2_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnCred2(long nChar, long inplen, long kst )
{
	if( m_DIALOG_OK == FALSE )
		return;

	KamokuTermJob( SCN_CRED2_PN, nChar, inplen, kst );
}

//////////////////////////////////////////////////////////////////////////////////
// 借 ／ 貸
void CScanKeyView::OnSetFocusScnDBcr() 
{
	if( m_DIALOG_OK == FALSE )
		return;
//	try
	{
		set_nowpn( SCN_DBCR_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_DBCR_PN );
}

void CScanKeyView::OnKillFocusScnDBcr( long inplen ) 
{
	ItemInp( SCN_DBCR_PN, &SCAN_KEY.SCN_DBCR_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnDBcr(long nChar, long inplen, long kst )
{
	if( m_DIALOG_OK == FALSE )
		return;

	KamokuTermJob( SCN_DBCR_PN, nChar, inplen, kst );
}

// 終了 -------------------
void CScanKeyView::OnSetFocusScnDBcr2() 
{
	if( m_DIALOG_OK == FALSE )
		return;
//	try
	{
		set_nowpn( SCN_DBCR2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_DBCR2_PN );
}

void CScanKeyView::OnKillFocusScnDBcr2( long inplen ) 
{
	ItemInp( SCN_DBCR2_PN, &SCAN_KEY.SCN_DBCR2_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnDBcr2(long nChar, long inplen, long kst )
{
	if( m_DIALOG_OK == FALSE )
		return;

	KamokuTermJob( SCN_DBCR2_PN, nChar, inplen, kst );
}


// 科目名称ターミネーションジョブ
void CScanKeyView::KamokuTermJob( int pn, long nChar, long inplen, long kst )
{
	struct _ItemData *keybuf;

	switch( pn ) {
	case SCN_DEBT_PN:	keybuf	=	&SCAN_KEY.SCN_DEBT_DT;	break;
	case SCN_DEBT2_PN:	keybuf	=	&SCAN_KEY.SCN_DEBT2_DT;	break;
	case SCN_CRED_PN:	keybuf	=	&SCAN_KEY.SCN_CRED_DT;	break;
	case SCN_CRED2_PN:	keybuf	=	&SCAN_KEY.SCN_CRED2_DT;	break;
	case SCN_DBCR_PN:	keybuf	=	&SCAN_KEY.SCN_DBCR_DT;	break;
	case SCN_DBCR2_PN:	keybuf	=	&SCAN_KEY.SCN_DBCR2_DT;	break;
	default:
		return;
	}

	// 入力＆オフフォーカス
	nChar = ItemInp( pn, keybuf, nChar, inplen, kst );
	if( nChar == -1 )
	{
		Buzzer();
		return;
	}
	else if( !nChar )
		return;

	//カーソル移動
	if( term_focus( pn, nChar ) != 0 )
		TermBuzzer();

}


//
// 科目カナ検索
BOOL CScanKeyView::ItemKanaSearch( LPCTSTR data) 
{
	char kana[10] = {0}, tmp[36] = {0};

	kana_copy( tmp, data );
	kana_copy( kana, data );

	SelPar par;

	if( m_SelDispPN == SL_KMKANA_PN ) {
		par.sel_pn = SL_KMKANA_PN;
#ifdef CLOSE // 07.17 /07
		sprintf_s( kana, sizeof kana, "%s%s", saveITEM50kana, tmp );
#endif
		memcpy( par.kmk_kana, kana, sizeof par.kmk_kana );
	}
	else {
		par.sel_pn = SL_KMKANA_PN;
		memcpy( par.kmk_kana, kana, sizeof par.kmk_kana );
	}

	int st = m_Seldata.SelectJob( &par, -1 );

	if( m_SelDispPN == SL_KMKANA_PN ) {
		if( ! st )
			strcpy_s( saveITEM50kana, sizeof saveITEM50kana, kana);	//save
	}
	else {
		::ZeroMemory( saveITEM50kana, sizeof saveITEM50kana );
	}

	return TRUE;

}

//
// 科目チェック
long CScanKeyView::ItemDataCheck(int pn, long nChar, LPCTSTR string, int dcsw ) 
{
	struct _ItemData check,ans;

//	memmove( (char *)&check, (char *)data, sizeof(check) );
	::ZeroMemory(&check.IP_stKMK, sizeof check.IP_stKMK);
	check.IP_stKMK.kd_eda = -1;
	KamokuStr_ChkLength( string, &check.IP_stKMK, 0 );

	if( InputItemCheck( ans.IP_stKMK, check.IP_stKMK, dcsw ) == TRUE )
	{
		DBkamokudata_set( this, scGetINP_CTL( pn )->IDC_X, &ans.IP_stKMK, pDBzm);
//		data_reply( scGetINP_CTL( pn )->IDC_X, 1, &ans, CICSInput );
	}
	else
	{
		Buzzer();

		memset( (char *)&ans, (char)0, sizeof(ans) );
		ans.IP_stKMK.kd_eda = -1;
		DBkamokudata_set( this, scGetINP_CTL( pn )->IDC_X, &ans.IP_stKMK, pDBzm );

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
long CScanKeyView::ItemInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;

	// オフフォーカス
//	if( !nChar )
//	{
		//データリード
		if( nChar == 0x16 ) {
			DBkamokudata_set( this, scGetINP_CTL( pn )->IDC_X, &keybuf->IP_stKMK, pDBzm );
		}

		DBkamokudata_get( this, scGetINP_CTL( pn )->IDC_X, &check.IP_stKMK, pDBzm);

		if( check.IP_CdNum[0] == 0 )
		{
			DBkamokudata_set( this, scGetINP_CTL( pn )->IDC_X, &keybuf->IP_stKMK, pDBzm );
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
		DBkamokudata_set( this, scGetINP_CTL( pn )->IDC_X, &keybuf->IP_stKMK, pDBzm );
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
		DBkamokudata_set( this, scGetINP_CTL( pn )->IDC_X, &keybuf->IP_stKMK, pDBzm );		

		SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}


//////////////////////////////////////////////////////////////////////////////////////
// 枝番関係
//

// 枝番フォーカス処理
void CScanKeyView::FocusScnBrnSub( int pn )
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
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
void CScanKeyView::OnSetFocusScndbr1() 
{
	FocusScnBrnSub( SCN_DBR1_PN );
}

void CScanKeyView::OnKillFocusScndbr1( long inplen ) 
{
	TerminationBrn( SCN_DBR1_PN, 0, inplen, 0 );
}
// 借方 ２
void CScanKeyView::OnSetFocusScndbr2()
{
	FocusScnBrnSub( SCN_DBR2_PN );
}

void CScanKeyView::OnKillFocusScndbr2( long inplen )
{
	TerminationBrn( SCN_DBR2_PN, 0, inplen, 0 );
}
// 貸方 １
void CScanKeyView::OnSetFocusScncbr1()
{
	FocusScnBrnSub( SCN_CBR1_PN );
}

void CScanKeyView::OnKillFocusScncbr1( long inplen )
{
	TerminationBrn( SCN_CBR1_PN, 0, inplen, 0 );
}
// 貸方 ２
void CScanKeyView::OnSetFocusScncbr2()
{
	FocusScnBrnSub( SCN_CBR2_PN );
}

void CScanKeyView::OnKillFocusScncbr2( long inplen )
{
	TerminationBrn( SCN_CBR2_PN, 0, inplen, 0 );
}
// 借／貸 １
void CScanKeyView::OnSetFocusScnDCbr1()
{
	FocusScnBrnSub( SCN_DCBR1_PN );
}

void CScanKeyView::OnKillFocusScnDCbr1( long inplen )
{
	TerminationBrn( SCN_DCBR1_PN, 0, inplen, 0 );
}
// 借／貸 ２
void CScanKeyView::OnSetFocusScnDCbr2()
{
	FocusScnBrnSub( SCN_DCBR2_PN );
}

void CScanKeyView::OnKillFocusScnDCbr2( long inplen )
{
	TerminationBrn( SCN_DCBR2_PN, 0, inplen, 0 );
}

// 枝番データ取得
struct _ItemData *CScanKeyView::get_brndata( int pn )
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



int CScanKeyView::CheckImeEndBranch( UINT ID, long nChar, LPCTSTR string )
{
	struct _ItemData check;
	VARIANT var;
	char edabuf[64] = {0};

	check.IP_stKMK.kd_eda = -1;
	KamokuStr_ChkLength( string, &check.IP_stKMK, Voln1->edcol );

	int pn = 0;

	pn = GetPositionFromSkeyID(ID);

	if( pn == 0 )	return 0;
	int eda = -1;
	CString	swork;
	int rval = 1;

	switch( check.IP_stKMK.kd_sgn ) {
	case 0:
		swork = check.IP_stKMK.kd_code;
		if( swork.GetLength() > Voln1->edcol ) {
			swork.SetAt( Voln1->edcol, '\0' );
		}
		eda = atoi( swork );
		pDBzm->EdabanToStr( edabuf, sizeof edabuf, eda );
		var.pbVal = (BYTE*)edabuf;
		DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
		break;
	case 2:	// .(ピリオド) での枝番入力
		pDBzm->EdabanToStr( edabuf, sizeof edabuf, check.IP_stKMK.kd_eda );
		var.pbVal = (BYTE*)edabuf;
		DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
		m_bIMEendBrn = TRUE;
		rval = 2;
		break;
	case 1:
		// 枝番摘要選択
		if( ! brnsel_set( pn, atoi( check.IP_stKMK.kd_code ) ) )
			nChar = 0;
		else {
			m_bIMEendBrn = TRUE;
		}
		break;
	}

	return rval;

}



// ターミネーション枝番
void CScanKeyView::TerminationBrn(int pn, long nChar, long inplen, long kst)
{
	struct _ItemData *keybuf;

	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		if( keybuf = get_brndata( pn ) )
		{
			// 入力＆オフフォーカス
			if( !(nChar = BrInp( pn, keybuf, nChar, inplen, kst)) )
			{
				return;
			}
			// カーソルセット
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
long CScanKeyView::BrInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	char str[128];
	int n = 0;
	int ret = 0;
	VARIANT var;

	if( inplen )
	{
		//データリード
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		n = strlen( (char*)var.pbVal );

		if( BRNTKY_sel && n && n < 3 && !m_bIMEendBrn )
		{
			// 枝番摘要選択
			if( brnsel_set( pn, atoi( (char*)var.pbVal ) ) )
				ret = 0;
			else
			{
				// 選択ミス
				//データセット(再表示)
				if( keybuf->IP_cBRN != -1 ) {
					pDBzm->EdabanToStr( str, sizeof str, keybuf->IP_cBRN );
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
			m_bIMEendBrn = FALSE;

			if( n )
			{
				SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
				
				keybuf->IP_cBRN = atoi( (char*)var.pbVal );
				pDBzm->EdabanToStr( str, sizeof str, keybuf->IP_cBRN );
				var.pbVal = (BYTE*)str;
			}
			else {
				keybuf->IP_cBRN = -1;
				SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
				var.pbVal = NULL;
			}
			//データセット
			DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			ret = 0;
		}
	}

	if( ! nChar ) {
		// 枝番摘要表示
		dsp_etc( pn, FALSE );	// 今回ポジションを非表示
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
	if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_HOME ) 
	{
		if( nChar == VK_TAB || nChar == VK_HOME ) {
			if( ! BRNTKY_sel || nChar == VK_HOME ) {
				disp_brnselect( pn );
				if( BRNTKY_sel )	nChar = 0;
			}
		}
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
BOOL CScanKeyView::brnsel_set( int pn, int num )
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

			pKey = _getDATA( pn );

			pKey->IP_cBRN = srt.brn_no;
			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;

			//データセット
			pDBzm->EdabanToStr( str, sizeof str, pKey->IP_cBRN );
			var.pbVal = (BYTE*)str;
			DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			return TRUE;
		}
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////////////////
///	金額
// 開始
void CScanKeyView::OnSetFocusScnval1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_VAL1_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_VAL1_PN );
}

void CScanKeyView::OnKillFocusScnval1( long inplen ) 
{
	 ValInp( SCN_VAL1_PN, &SCAN_KEY.SCN_VAL1_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnval1(long nChar, long inplen, long kst ) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = ValInp( SCN_VAL1_PN, &SCAN_KEY.SCN_VAL1_DT, nChar, inplen, kst )) )
			return;

		//カーソル移動
		if( term_focus( SCN_VAL1_PN, nChar ) != 0 )
			TermBuzzer();
		
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

// 終了
void CScanKeyView::OnSetFocusScnval2() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_VAL2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_VAL2_PN );
}

void CScanKeyView::OnKillFocusScnval2( long inplen ) 
{
	 ValInp( SCN_VAL2_PN, &SCAN_KEY.SCN_VAL2_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnval2(long nChar, long inplen, long kst ) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = ValInp( SCN_VAL2_PN, &SCAN_KEY.SCN_VAL2_DT, nChar, inplen, kst )) )
			return;

		//カーソル移動
		if( term_focus( SCN_VAL2_PN, nChar ) != 0 )
			TermBuzzer();


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
void CScanKeyView::OnSetFocusScnzei1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_ZEI1_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_ZEI1_PN );
}

void CScanKeyView::OnKillFocusScnzei1( long inplen )
{
	if( m_DIALOG_OK == FALSE )
		return;

	ValInp( SCN_ZEI1_PN, &SCAN_KEY.SCN_ZEI1_DT, 0, inplen, 0);
}


void CScanKeyView::OnTerminationScnzei1(long nChar, long inplen, long kst ) 
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
			TermBuzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

//終了
void CScanKeyView::OnSetFocusScnzei2() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_ZEI2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_ZEI2_PN );
}

void CScanKeyView::OnKillFocusScnzei2( long inplen )
{
	if( m_DIALOG_OK == FALSE )
		return;

	ValInp( SCN_ZEI2_PN, &SCAN_KEY.SCN_ZEI2_DT, 0, inplen, 0);
}

void CScanKeyView::OnTerminationScnzei2(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = ValInp( SCN_ZEI2_PN, &SCAN_KEY.SCN_ZEI2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_ZEI2_PN, nChar ) != 0 )
			TermBuzzer();
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}


////////////////////////////////////////////////////////////////////////////////////////
// 対価
//開始
void CScanKeyView::OnSetFocusScntai1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

	set_nowpn( SCN_TAI1_PN );

	OnICSInputFocus( SCN_TAI1_PN );
}

void CScanKeyView::OnKillFocusScntai1( long inplen )
{
	if( m_DIALOG_OK == FALSE )
		return;

	ValInp( SCN_TAI1_PN, &SCAN_KEY.SCN_TAI1_DT, 0, inplen, 0);
}


void CScanKeyView::OnTerminationScntai1(long nChar, long inplen, long kst ) 
{
	if( m_DIALOG_OK == FALSE )
		return;

	// 入力＆オフフォーカス
	if( !(nChar = ValInp( SCN_TAI1_PN, &SCAN_KEY.SCN_TAI1_DT, nChar, inplen, kst)) )
		return;

	//カーソル移動
	if( term_focus( SCN_TAI1_PN, nChar ) != 0 )
		TermBuzzer();
}

//終了
void CScanKeyView::OnSetFocusScntai2() 
{
	if( m_DIALOG_OK == FALSE )
		return;

	set_nowpn( SCN_TAI2_PN );

	OnICSInputFocus( SCN_TAI2_PN );
}

void CScanKeyView::OnKillFocusScntai2( long inplen )
{
	if( m_DIALOG_OK == FALSE )
		return;

	ValInp( SCN_TAI2_PN, &SCAN_KEY.SCN_TAI2_DT, 0, inplen, 0);
}

void CScanKeyView::OnTerminationScntai2(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

	// 入力＆オフフォーカス
	if( !(nChar = ValInp( SCN_TAI2_PN, &SCAN_KEY.SCN_TAI2_DT, nChar, inplen, kst)) )
		return;

	//カーソル移動
	if( term_focus( SCN_TAI2_PN, nChar ) != 0 )
		TermBuzzer();
}

//
// 金額　入力＆オフフォーカス
long CScanKeyView::ValInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst )
{
	struct _ItemData check = {0};
	VARIANT var;
	char	chkbf[20] = {0};

	if( inplen || nChar == 0x16 ) {
		l_defn( 0x16 );
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
		memcpy( check.IP_VAL, var.pbVal, sizeof check.IP_VAL );
		memmove( keybuf, &check, sizeof(check) );

		if( inplen < 0 && memcmp( check.IP_VAL, chkbf, 6 ) == 0 ) {
			// BacsSpace による削除
			var.pbVal = NULL;
			DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
		}
		else {
			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
		}
	}
	else if( inplen == 0 ) { //電卓からの場合、inplen == 0 で来るので金額をチェックする。
		l_defn( 0x16 );
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );

		memcpy( check.IP_VAL, var.pbVal, sizeof check.IP_VAL );

		if( memcmp( keybuf->IP_VAL, check.IP_VAL, 6 ) != 0 ) {
			memcpy( keybuf, &check, sizeof(check) );
			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
		}
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

// 複数検索摘要 の表示
void CScanKeyView::TkyReportDisp() 
{
	CRect rect;
	m_Tky.GetWindowRect( rect );
	ScreenToClient( rect );

	int top, head_ht, repo_ht;

	head_ht = m_Report.GetHeaderHeight();
	repo_ht = head_ht + (head_ht*5);

	top = rect.top - repo_ht;

	m_Report.ShowWindow(SW_SHOW);
	m_Report.MoveWindow( rect.left, top, rect.Width(), repo_ht );

	m_Tky.SetBackColor( RGB_YELLOW1 );
}




void CScanKeyView::OnSetFocusScntky() 
{
	BOOL debt_inp_sg;
	BOOL cred_inp_sg;
	BOOL dbcr_inp_sg;
	CString debt, cred;

	if( m_DIALOG_OK == FALSE )
		return;

	/*---VER2----*/
	if( SC_Tky.GetCount() > 0 ) {
		// 複数検索あり
		TkyReportDisp();
	}

//	try
	{
		if( get_nowpn() != SCN_TKY_PN )
		{
			set_nowpn( SCN_TKY_PN );

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


void CScanKeyView::OnKillFocusScntky( long inplen ) 
{
	OnTerminationScntky( 0, inplen, 0 ); 
}

void CScanKeyView::OnKanaTermScntky(LPCSTR data) 
{
	struct _ItemData dmy_data = {0};
	CDBipTKREC tkrec;

//	try
	{
		//５０音摘要表示

		if( ! isdigitstr( (char*)data ) ) {

			SelPar par;
			par.sel_pn = SL_TKYKANA_PN;
			strcpy_s( par.tky_kana, sizeof par.tky_kana, data );
			if( strlen( par.tky_kana ) > 4 ) {
				 par.tky_kana[4] = '\0';
			}

			m_Seldata.SelectJob( &par, -1 );
			// 摘要 カナ検索中
			pDBzm->tkrec_kanamode() = TRUE;
			m_BrTek.set_tkquery( TRUE );
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
void CScanKeyView::tky_select_dsp( CDBipTKREC *ptk, BOOL F12_KEY )
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
		//データセットのためのダミーフォーカスＯＦＦ
		OnTerminationScntky( 0, 0, 0 );

/*DB_OLD*///	((CICSDBEDT*)GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X ))->TekiInsert( (char*)ins_data.IP_TKY );

		var.pbVal = ins_data.IP_TKY;
		CWnd* pwnd = GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X );
		short pos = ((CICSDBEDT*)pwnd)->GetCaretPosition();
		((CICSDBEDT*)pwnd)->InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );	
	}
}

void CScanKeyView::OnTerminationScntky(long nChar, long inplen, long kst) 
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
		//	pos = ((CICSDBEDT*)GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X))->GetCaretPosition();

			DBdata_get( this, scGetINP_CTL( SCN_TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		//	DBdata_set( this, scGetINP_CTL( SCN_TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

		//	((CICSDBEDT*)GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X))->SetCaretPosition( pos );

			//データセット
			i = sprintf_s( pbuf, sizeof pbuf, "%.80s", var.pbVal /*check.IP_TKY*/ );
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

		if( nChar == VK_UP ) {
			if( SC_Tky.GetCount() > 0 ) {
				m_Report.SetFocus();
				ICSReportRows* pRow = m_Report.GetRows();
				int cnt = pRow->GetCount();
				// リストの最後の項目に
				m_Report.SetFocusedRow(cnt-1);
				m_idxTkyReport	= (cnt-1);
				return;
			}
		}

		//カーソル移動
		if( term_focus( SCN_TKY_PN, nChar ) != 0 )
			TermBuzzer();

	}
//	catch ( CErrBlk E )
//	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
//	}
}

///////////////////////////////////////////////////////////////////////////////
// 手形番号
void CScanKeyView::OnSetFocusScnSnum1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_SNUM1_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_SNUM1_PN ); 	
}

void CScanKeyView::OnKillFocusScnSnum1( long inplen ) 
{
	SnumInp( SCN_SNUM1_PN, &SCAN_KEY.SCN_SNUM1_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnSnum1(long nChar, long inplen, long kst) 
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
			TermBuzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

void CScanKeyView::OnSetFocusScnSnum2() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_SNUM2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_SNUM2_PN );	
}

void CScanKeyView::OnKillFocusScnSnum2( long inplen ) 
{
	SnumInp( SCN_SNUM2_PN, &SCAN_KEY.SCN_SNUM2_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnSnum2(long nChar, long inplen, long kst) 
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
			TermBuzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}	
}

// 証憑番号 入力＆オフフォーカス
long CScanKeyView::SnumInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
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
void CScanKeyView::OnSetFocusScnTgdate1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_TDAT1_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_TDAT1_PN ); 	
}

void CScanKeyView::OnKillFocusScnTgdate1( long inplen ) 
{
	TgdateInp( SCN_TDAT1_PN, &SCAN_KEY.SCN_TDAT1_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnTgdate1(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		if( inplen > 0 || nChar == 0x16 ) CheckDataIcsinputTgDate( SCN_TDAT1_PN, nChar );
		// 入力＆オフフォーカス
		if( !(nChar = TgdateInp( SCN_TDAT1_PN, &SCAN_KEY.SCN_TDAT1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_TDAT1_PN, nChar ) != 0 )
			TermBuzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

void CScanKeyView::OnSetFocusScnTgdate2() 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_TDAT2_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_TDAT2_PN );	
}

void CScanKeyView::OnKillFocusScnTgdate2( long inplen ) 
{
	TgdateInp( SCN_TDAT2_PN, &SCAN_KEY.SCN_TDAT2_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnTgdate2(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		if( inplen > 0 || nChar == 0x16 ) CheckDataIcsinputTgDate( SCN_TDAT2_PN, nChar );

		// 入力＆オフフォーカス
		if( !(nChar = TgdateInp( SCN_TDAT2_PN, &SCAN_KEY.SCN_TDAT2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_TDAT2_PN, nChar ) != 0 )
			TermBuzzer();
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
long CScanKeyView::CheckDataIcsinputTgDate( int pn, long nChar ) 
{
	char check[3] = {0};
	char ymd[3] = {0};
	char data[20] = {0};
	char result[30];
	char buf[40];
	VARIANT var;
	CString str;
	int ret;
	CVolDateDB vd;

	//
//	try
	{
		DBdata_get( this, scGetINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		strcpy_s( data, sizeof data, (char*)var.pbVal );

		if( (ret = TgDateInputCheck( data, result, sizeof result, Voln1->ss_ymd, Voln1->ee_ymd, -1 )) <= 0 ) {
			// 不正データが入力された
			if( SCAN_KEY._XXDTA2[pn].IP_TGDAY > 0 ) {
				int gendate, d_gengo;
				vd.db_datecnvGen( 0, SCAN_KEY._XXDTA2[pn].IP_TGDAY, &d_gengo, &gendate, 0, 0 );

				sprintf_s( result, sizeof result, "%d", gendate );
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
long CScanKeyView::TgdateInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	char	buf[30] = {0};
	VARIANT var;
	CVolDateDB vd;

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		if( var.pbVal[0] != 0 )	{
			int gendate;
			gendate = atoi( (LPCTSTR)var.pbVal );
			sprintf_s( buf, sizeof buf, "%06d", gendate );
			BYTE bcddate[10] = {0};
			l_pack( bcddate, buf, 6 );
			check.IP_TGDAY = TgDateBcdToSeireki( bcddate, NULL );

		//	vd.db_datecnv( gendate, &check.IP_TGDAY, 1, 0 );
		}
		else {
			check.IP_TGDAY = -1;
		}

		if( check.IP_TGDAY != -1 )
		{
			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
			keybuf->IP_TGDAY = check.IP_TGDAY;
		}
		else {
			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
			keybuf->IP_TGDAY = -1;
		}

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
BOOL CScanKeyView::isScanKeyInp()
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



void CScanKeyView::OnSelchangeTkycombo() 
{
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

		scGetINP_CTL(SNOT_TKY_PN)->INP_ena = TRUE;
		m_NotTky.EnableWindow( TRUE );
	}
	else {
		SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_sg = TRUE;

		scGetINP_CTL(SNOT_TKY_PN)->INP_ena = FALSE;
		m_NotTky.SetCheck( BST_UNCHECKED );
		m_NotTky.EnableWindow( FALSE );
	}
	// エディット表示設定
	disp_imgtkyedit( SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_type );

}


void CScanKeyView::OnSelchangeScnsyzsw() 
{
	int sel = m_Syzsw.GetCurSel();
	DWORD itd = m_Syzsw.GetItemData( sel );

	SCAN_KEY.SCN_SYZSW_DT.SKBN_SGN1 = itd;
	if( itd == NON_SELECT ) {
		strcpy_s( SCAN_KEY.SCN_SYZSW_DT.SKBN_TXT, sizeof SCAN_KEY.SCN_SYZSW_DT.SKBN_TXT, "" );
		SCAN_KEY.LINE_CND[ SCN_SYZSW_PN ].INP_sg = FALSE;
	}
	else {
		m_Syzsw.GetLBText( sel, SCAN_KEY.SCN_SYZSW_DT.SKBN_TXT );
		SCAN_KEY.LINE_CND[ SCN_SYZSW_PN ].INP_sg = TRUE;
	}
}


void CScanKeyView::OnSelchangeScnskbn() 
{
	int sel = m_Skbn.GetCurSel();
	DWORD itd = m_Skbn.GetItemData( sel );
	SCAN_KEY.SCN_SKBN_DT.SKBN_SGN1 = itd;

	if( itd == NON_SELECT ) {
		strcpy_s( SCAN_KEY.SCN_SKBN_DT.SKBN_TXT, sizeof SCAN_KEY.SCN_SKBN_DT.SKBN_TXT, "" );
		SCAN_KEY.LINE_CND[ SCN_SKBN_PN ].INP_sg = FALSE;
	}
	else {
		m_Syzsw.GetLBText( sel, SCAN_KEY.SCN_SKBN_DT.SKBN_TXT );
		SCAN_KEY.LINE_CND[ SCN_SKBN_PN ].INP_sg = TRUE;
	}
	if( Voln1->apno >= 0x50 ) {
	//	SetTokusWnd( itd == SK_HUKA ? TRUE : FALSE );
		BOOL bEnable = TRUE;
		if( pDBsy->IsSyohizeiMaster() ) {
			if( pDBsy->IsKaniMaster() )
				bEnable = FALSE;
		}
		SetTokusWnd( bEnable );
	}
}


void CScanKeyView::OnSelchangeScnzritu() 
{
	int sel = m_Zritu.GetCurSel();
	DWORD itd = m_Zritu.GetItemData( sel );
	SCAN_KEY.SCN_ZRITU_DT.SKBN_SGN1 = itd;

	if( itd == NON_SELECT ) {
		strcpy_s( SCAN_KEY.SCN_ZRITU_DT.SKBN_TXT, sizeof SCAN_KEY.SCN_ZRITU_DT.SKBN_TXT, "" );
		SCAN_KEY.LINE_CND[ SCN_ZRITU_PN ].INP_sg = FALSE;
	}
	else {
		m_Syzsw.GetLBText( sel, SCAN_KEY.SCN_ZRITU_DT.SKBN_TXT );
		SCAN_KEY.LINE_CND[ SCN_ZRITU_PN ].INP_sg = TRUE;
	}
}


void CScanKeyView::OnSelchangeScnurisire() 
{
	int sel = 	m_Urisire.GetCurSel();
	DWORD itd = m_Urisire.GetItemData( sel );
	SCAN_KEY.SCN_URISIRE_DT.SKBN_SGN1 = itd;
	m_Urisire.GetLBText( sel, SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT );

	if( itd == NON_SELECT ) {
		strcpy_s( SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT, sizeof SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT, "" );
		SCAN_KEY.LINE_CND[ SCN_URISIRE_PN ].INP_sg = FALSE;
	}
	else {
		m_Urisire.GetLBText( sel, SCAN_KEY.SCN_URISIRE_DT.SKBN_TXT );
		SCAN_KEY.LINE_CND[ SCN_URISIRE_PN ].INP_sg = TRUE;
	}
}

// 特定収入
void CScanKeyView::OnSelchangeScnTokus() 
{
	int sel = 	m_Tokus.GetCurSel();
	DWORD itd = m_Tokus.GetItemData( sel );
	SCAN_KEY.SCN_TOKUS_DT.SKBN_SGN1 = itd;
	m_Tokus.GetLBText( sel, SCAN_KEY.SCN_TOKUS_DT.SKBN_TXT );

	if( itd == NON_SELECT ) {
		strcpy_s( SCAN_KEY.SCN_TOKUS_DT.SKBN_TXT, sizeof SCAN_KEY.SCN_TOKUS_DT.SKBN_TXT, "" );
		SCAN_KEY.LINE_CND[ SCN_TOKUS_PN ].INP_sg = FALSE;
	}
	else {
		m_Tokus.GetLBText( sel, SCAN_KEY.SCN_TOKUS_DT.SKBN_TXT );
		SCAN_KEY.LINE_CND[ SCN_TOKUS_PN ].INP_sg = TRUE;
	}
}


int CScanKeyView::set_nonICSpn( tagMSG* pMsg )
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
		// 日付コントロール
		else if( get_nowpn() != SCN_DATE1_PN && (pwnd->m_hWnd == GetDlgItem( IDC_SCN_DATESEL )->m_hWnd || 
			::IsChild( GetDlgItem( IDC_SCN_DATESEL )->m_hWnd, pwnd->m_hWnd )) ) {
			return set_nowpn( SCN_DATE1_PN );
		}
		// 伝票種別チェックボタン
		else if( get_nowpn() != SCN_CHKFURI_PN && pwnd->m_hWnd == GetDlgItem( IDC_FURI_CHK )->m_hWnd )
		{
			return set_nowpn( SCN_CHKFURI_PN );
		}
		else if( get_nowpn() != SCN_CHKNYUK_PN && pwnd->m_hWnd == GetDlgItem( IDC_NYUK_CHK )->m_hWnd )
		{
			return set_nowpn( SCN_CHKNYUK_PN );
		}
		else if( get_nowpn() != SCN_CHKSYUK_PN && pwnd->m_hWnd == GetDlgItem( IDC_SYUK_CHK )->m_hWnd )
		{
			return set_nowpn( SCN_CHKSYUK_PN );
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
		// 入力漏れ
		else if( get_nowpn() != SCN_NOINP_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNNOINPBRN )->m_hWnd )
		{
			return set_nowpn( SCN_NOINP_PN );
		}
		// 業種の選択
		else if( get_nowpn() != SCN_GYOKBN_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNGYOKBN )->m_hWnd )
		{
			return set_nowpn( SCN_GYOKBN_PN );
		}
		// 変動事由
		else if( get_nowpn() != SCN_HJIYU_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNHJIYU )->m_hWnd )
		{
			return set_nowpn( SCN_HJIYU_PN );
		}
		// 資産
		else if( get_nowpn() != SCN_SYZKTSN_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNSYZKTSN )->m_hWnd )
		{
			return set_nowpn( SCN_SYZKTSN_PN );
		}
		// 仕訳データタイプ
		else if( get_nowpn() != SCN_DTTYPE_PN && pwnd->m_hWnd == GetDlgItem( IDC_COMBO_MIKAKUD )->m_hWnd )
		{
			return set_nowpn( SCN_DTTYPE_PN );
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
		else if( get_nowpn() != SNOT_SNUM_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_SNUMBER )->m_hWnd ) {
			return set_nowpn( SNOT_SNUM_PN );
		}
		else if( get_nowpn() != SNOT_TGDATE_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_TGDATE )->m_hWnd ) {
			return set_nowpn( SNOT_TGDATE_PN );
		}
		else if( get_nowpn() != SNOT_HUSEN_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_HUSEN )->m_hWnd ) {
			return set_nowpn( SNOT_HUSEN_PN );
		}
		else if( get_nowpn() != SNOT_DOCEVI_PN && pwnd->m_hWnd == GetDlgItem( IDC_NCHK_SNO )->m_hWnd ) {
			return set_nowpn( SNOT_DOCEVI_PN );
		}

	}
	return get_nowpn();
}

	

int CScanKeyView::LbuttonDownJob( struct tagMSG * pMsg )
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

BOOL CScanKeyView::PreTranslateMessage(MSG* pMsg) 
{
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
		pn = get_nowpn();
		if( pn == SCN_TKY_PN )
		{
		//	if( pMsg->wParam == VK_BACK )
		//	{
		//		if( GetDlgItem( scGetINP_CTL( SCN_TKY_PN )->IDC_X )->m_hWnd == pMsg->hwnd )
		//		{
		//			OnTerminationScntky(pMsg->wParam);
		//			return TRUE;
		//		}
		//	}
		}

		switch( pn ) {
		case SCN_DEBT_PN:
		case SCN_DEBT2_PN:
		case SCN_CRED_PN:
		case SCN_CRED2_PN:
		case SCN_DBCR_PN:
		case SCN_DBCR2_PN:
			if( KanaCheckVK_BACK(pMsg) )
				return 1;
		case SCN_INVNO_PN:
		//	if( KanaCheckVK_BACK(pMsg, 1) )
		//		return 1;
			KanaCheckVK_BACK(pMsg, 1);
			break;
		}
		int nChar = -1;
		if( is_SHIFT() && pMsg->wParam == VK_TAB )
			nChar = VK_F2;
		else if( ! is_SHIFT() )
			nChar = pMsg->wParam;

		if( nChar != -1 )
		{
			CWnd* fwnd = GetFocus();
			if (fwnd == NULL) {
				return TRUE;
			}
			int id = fwnd->GetDlgCtrlID();

			if( nChar == VK_INSERT ) {
				if( ! bCONFIRM_MASTER )
					PostMessage(WM_COMMAND, MAKELONG(IDC_TKYONLY, BN_CLICKED), (LPARAM)GetDlgItem(IDC_TKYONLY)->m_hWnd);
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
			else if( GetDlgItem(IDC_ALLKEYDEL)->m_hWnd == pMsg->hwnd )
			{
				if( !OnTerminateOnAllKeyDel(nChar) )
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
			else if( GetDlgItem( IDC_SCNTOKUS )->m_hWnd == pMsg->hwnd ) {
				if( !OnTerminationScnTokus( nChar ) )
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
			else if( GetDlgItem( IDC_SCNGYOKBN )->m_hWnd == pMsg->hwnd ) {
				if( !OnTermComboGyoKbn( nChar ) )
					return TRUE;
			}
			else if( (pn = IsNotChkbox( pMsg->hwnd )) != -1 ) {
				if( !OnTermNotChkbox( pn, nChar ) )
					return TRUE;
			}
			else if( (pn = IsDenpChkbox( pMsg->hwnd )) != -1 ) {
				if( !OnTermDenpChkbox( pn, nChar ) )
					return TRUE;
			}
			else if( GetDlgItem( IDC_SCNHJIYU )->m_hWnd == pMsg->hwnd ) {
				if( !OnTermComboHjiyu( nChar ) )
					return TRUE;
			}
			// ｲﾒｰｼﾞ摘要表示ｴﾃﾞｨｯﾄ
			else if( GetDlgItem( IDC_IMGTKYEDIT )->m_hWnd == pMsg->hwnd ) {
				if( !OnTerminateOnImgTkyEdit( nChar ) )
					return TRUE;
			}
			// 複数摘要のレポート
			else if( GetDlgItem( IDC_TKY_REPORT )->m_hWnd == pMsg->hwnd ) {
				if( !OnReportKeyJob( nChar ) )
					return TRUE;
			}
			else if( GetDlgItem( IDC_CHECK_KMSYZ )->m_hWnd == pMsg->hwnd ) {
				if( !OnTermCheckKMSYZ( nChar ) )
					return TRUE;
			}
	//		else if( GetDlgItem( IDC_SCNSHTTYP )->m_hWnd == pMsg->hwnd ) {
			if( id == IDC_SCNSHTTYP || id == 1001 ) {
				if( !OnTermComboDocSheetTyp( nChar ) )
					return TRUE;
			}
			else if( GetDlgItem( IDC_SCNSYZKTSN )->m_hWnd == pMsg->hwnd ) {
				if( !OnTermComboSyzKtsisan( nChar ) )
					return TRUE;
			}
			else if( GetDlgItem( IDC_COMBO_MIKAKUD )->m_hWnd == pMsg->hwnd ) {
				if( !OnTermComboDataType( nChar ) )
					return TRUE;
			}
			else if( GetDlgItem( IDC_CHECK_MENZEI )->m_hWnd == pMsg->hwnd ) {
				if( !OnTerminationMenzei( nChar )) {
					return TRUE;
				}
			}
			else if( GetDlgItem(IDC_SCNMENWARI)->m_hWnd == pMsg->hwnd ) {
				if( !OnTerminationMenWari(nChar) ) {
					return TRUE;
				}
			}
		}
	}
	else if( pMsg->message == WM_KEYUP )
	{
		pn = get_nowpn();
		int kmbrn_flg = -1;
		int invno_flg = -1;

		switch( pn ) {
		case SCN_DEBT_PN:
		case SCN_DEBT2_PN:
		case SCN_CRED_PN:
		case SCN_CRED2_PN:
		case SCN_DBCR_PN:
		case SCN_DBCR2_PN:
			kmbrn_flg = 0;
			break;
		case SCN_DBR1_PN:
		case SCN_DBR2_PN:
		case SCN_CBR1_PN:
		case SCN_CBR2_PN:
		case SCN_DCBR1_PN:
		case SCN_DCBR2_PN:
			kmbrn_flg = 1;
			break;
		case SCN_INVNO_PN:
			invno_flg = 1;
			break;
		}
		if( kmbrn_flg != -1 ) {
			FukuKanaSearch(pMsg, kmbrn_flg);
		}
		else if( invno_flg != -1 ) {
			FukuKanaSearch(pMsg, 0, invno_flg);
		}
	}

	return CDBbaseView::PreTranslateMessage(pMsg);
}


long CScanKeyView::OnTerminationScnSyzsw(long nChar) 
{
	if( m_DIALOG_OK == FALSE )
		return nChar;

//	try
	{
		//カーソル移動
		if( term_focus( SCN_SYZSW_PN, nChar ) != 0 ) {
			TermBuzzer();
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

long CScanKeyView::OnTerminationScnZritu( long nChar)
{
	if( m_DIALOG_OK == FALSE )
		return nChar;

//	try
	{
		//カーソル移動
		if( term_focus( SCN_ZRITU_PN, nChar ) != 0 ) {
			TermBuzzer();
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


long CScanKeyView::OnTerminationScnskbn(long nChar) 
{
	if( m_DIALOG_OK == FALSE )
		return nChar;

//	try
	{
		//カーソル移動
		if( term_focus( SCN_SKBN_PN, nChar ) != 0 ) {
			TermBuzzer();
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

long CScanKeyView::OnTerminationScnurisire(long nChar) 
{
	if( m_DIALOG_OK == FALSE )
		return nChar;

//	try
	{
		//カーソル移動
		if( term_focus( SCN_URISIRE_PN, nChar ) != 0 ) {
			TermBuzzer();
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


// 特定収入
long CScanKeyView::OnTerminationScnTokus(long nChar) 
{
	if( m_DIALOG_OK == FALSE )
		return nChar;

	//カーソル移動
	if( term_focus( SCN_TOKUS_PN, nChar ) != 0 ) {
		TermBuzzer();
		return nChar;
	}
	return 0;
}

//
long CScanKeyView::OnTerminateOnOK( long nChar )
{
	switch( nChar )
	{
	case VK_RIGHT:
	case VK_DOWN:
//	case VK_F3:
		set_focus( SCN_IDCANCEL );
		return 0;
	case VK_UP:
		if( ! bCONFIRM_MASTER ) {
			set_focus( SCN_TKYONLY );
			return 0;
		}
		// else go through
	case VK_LEFT:
	case VK_F2:
		set_focus( SCN_NOINP_PN );
		return 0;
	}
	return nChar;
}


// 摘要のみ修正
void CScanKeyView::OnTkyonly() 
{
	//	摘要のみ修正
	((CMainFrame*)GetDBDinpMainFrame())->IsTkyOnly() = TRUE;	// サインの初期化

	ButtonF8Job();

//	EndDialog( IDOK );
}

// Insert キー
void CScanKeyView::OnButtonInsert()
{
	OnTkyonly();
}

void CScanKeyView::OnUpdateButtonInsert(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( bCONFIRM_MASTER ? FALSE : TRUE );
}

// 普通の検索
void CScanKeyView::OnBnClickedOk()
{
	//	摘要のみ修正
	((CMainFrame*)GetDBDinpMainFrame())->IsTkyOnly() = FALSE;

	ButtonF8Job();
}

void CScanKeyView::OnBnClickedCancel()
{
	OnButtonEnd();
}


void CScanKeyView::NotCheckSet( int pn, int sgn )
{
	struct _LINE_CND* lcnd;

	switch( pn ) {
	case SNOT_SEQ_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_SEQ1_PN ];		break;
	case SNOT_DATE_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DATE1_PN ];		break;
	case SNOT_DENP_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DENP1_PN ];		break;
	case SNOT_DBMN_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_BMON1_PN ];		break;
	case SNOT_CBMN_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_CBMN1_PN ];		break;
	case SNOT_DCBMN_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DCBMN1_PN ];		break;
	case SNOT_DKJI_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_KOJI1_PN ];		break;
	case SNOT_CKJI_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_CKJI1_PN ];		break;
	case SNOT_DCKJI_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DCKJI1_PN ];		break;
	case SNOT_DEB_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DEBT_PN ];		break;
	case SNOT_DBRN_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DBR1_PN ];		break;
	case SNOT_CRE_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_CRED_PN ];		break;
	case SNOT_CBRN_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_CBR1_PN ];		break;
	case SNOT_DBCR_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DBCR_PN ];		break;
	case SNOT_DCBRN_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DCBR1_PN ];		break;
	case SNOT_VAL_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_VAL1_PN ];		break;
	case SNOT_ZEI_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_ZEI1_PN ];		break;
	case SNOT_TAI_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_TAI1_PN ];		break;
	case SNOT_SYZSW_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_SYZSW_PN ];		break;
	case SNOT_ZRITU_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_ZRITU_PN ];		break;
	case SNOT_SKBN_PN:		lcnd = &SCAN_KEY.LINE_CND[ SCN_SKBN_PN ];		break;
	case SNOT_URISIRE_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_URISIRE_PN ];	break;
	case SNOT_TOKUS_PN:		lcnd = &SCAN_KEY.LINE_CND[ SCN_TOKUS_PN ];		break;	// 特定収入[06.03 /08]
	case SNOT_SNUM_PN:		lcnd = &SCAN_KEY.LINE_CND[ SCN_SNUM1_PN ];		break;
	case SNOT_TGDATE_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_TDAT1_PN ];		break;
	case SNOT_HUSEN_PN:		lcnd = &SCAN_KEY.LINE_CND[ SCN_HUSEN_PN ];		break;
	case SNOT_TKY_PN:		lcnd = &SCAN_KEY.LINE_CND[ SCN_TKY_PN ];		break;
	case SNOT_DOCEVI_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DOCEVI1_PN ];	break;
	case SNOT_DOCSEQ_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_DOCSEQ1_PN ];	break;
	case SNOT_INVNO_PN:		lcnd = &SCAN_KEY.LINE_CND[ SCN_INVNO_PN];	break;
	case SNOT_MENZEI_PN:	lcnd = &SCAN_KEY.LINE_CND[ SCN_MENZEI_PN];	break;
	default:
		lcnd = NULL;
		break;
	}
	if( lcnd ) {
		if( sgn )	lcnd->KEY_inp |= SC_NOTSEARCH;
		else		lcnd->KEY_inp &= ~SC_NOTSEARCH;
	}
}


//
long CScanKeyView::OnTerminateOnCancel( long nChar )
{
	switch( nChar )
	{
	case VK_LEFT:
	case VK_UP:
	case VK_F2:
		set_focus( SCN_IDOK );
		return 0;
	case VK_DOWN:
	case VK_RIGHT:
		return 0;
	}
	return nChar;
}

long CScanKeyView::OnTerminateOnAllKeyDel(long nChar)
{
	switch( nChar )
	{
	case VK_LEFT:
	case VK_UP:
	case VK_F2:
	case VK_TAB:
		set_focus(SCN_IDOK);
	//	set_focus(SCN_DUPLI_PN);
		return 0;
	case VK_DOWN:
	case VK_RIGHT:
		return 0;
	}
	return nChar;
}


long CScanKeyView::OnTerminateOnTkyOnly( long nChar )
{
//	return term_focus( SCN_CHKDBM_PN, nChar );

	switch( nChar )
	{
	case VK_RIGHT:
	case VK_DOWN:
	case VK_F3:
		set_focus( SCN_IDOK );
	//	GotoDlgCtrl( GetDlgItem( IDOK ) );
		return 0;
	case VK_LEFT:
	case VK_UP:
	case VK_F2:
		set_focus( SCN_TKY_PN );
		return 0;
	}
	return nChar;

}


// 借方部門 のﾁｪｯｸﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKeyView::OnTermCheckDBm( long nChar )
{
	return term_focus( SCN_CHKDBM_PN, nChar );
}

// 部門 のﾁｪｯｸﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKeyView::OnTermCheckCBm( long nChar )
{
	return term_focus( SCN_CHKCBM_PN, nChar );
}

// 工事 のﾁｪｯｸﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKeyView::OnTermCheckDkoji( long nChar )
{
	return term_focus( SCN_CHKDKJ_PN, nChar );
}

// 貸方工事 のﾁｪｯｸﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKeyView::OnTermCheckCkoji( long nChar )
{
	return term_focus( SCN_CHKCKJ_PN, nChar );
}


// 借方 のﾁｪｯｸﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKeyView::OnTermCheckDBR( long nChar )
{
	return term_focus( SCN_CHKDBR_PN, nChar );
}


// 貸方 のﾁｪｯｸﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKeyView::OnTermCheckCBR( long nChar )
{
	return term_focus( SCN_CHKCBR_PN, nChar );
}

// ターミネーション 摘要選択コンボ
long CScanKeyView::OnTerminateOnSelTky( long nChar )
{
	return term_focus( SCN_SELTKY_PN, nChar );
}

// ターミネーション イメージ摘要エディット
long CScanKeyView::OnTerminateOnImgTkyEdit( long nChar )
{
	if( nChar ==  VK_TAB ) {
		if( is_SHIFT() ) nChar = VK_F2;
	}

	return term_focus( SCN_TKY_PN, nChar );

}


// 表示順のｺﾝﾎﾞﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKeyView::OnTermComboOrder( long nChar )
{
	return term_focus( SCN_ORDER_PN, nChar );
}

// データユーザのｺﾝﾎﾞﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKeyView::OnTermComboDUser( long nChar )
{
	return term_focus( SCN_DUSER_PN, nChar );
}

// 検索タイプのｺﾝﾎﾞﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKeyView::OnTermComboSType( long nChar )
{
	return term_focus( SCN_STYPE_PN, nChar );
}

// 付箋のｺﾝﾎﾞﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKeyView::OnTermComboHusen( long nChar )
{
/*
	switch( nChar ) {
	case VK_RIGHT:
		if( ! BRmst ) {
			set_focus( SCN_IDOK );
			return 0;
		}
		break;
	default:
		break;
	}
*/
	return term_focus( SCN_HUSEN_PN, nChar );
}


long CScanKeyView::OnTermComboNoInp( long nChar )
{

	switch( nChar ) {
	case VK_RETURN:
	case VK_RIGHT:
//	case VK_F3:
	case VK_TAB:
		set_focus( SCN_IDOK );
		return 0;
		break;
	default:
		break;
	}

	return term_focus( SCN_NOINP_PN, nChar );
}

// 業種区分 ターミネーション
long CScanKeyView::OnTermComboGyoKbn( long nChar )
{

	return term_focus( SCN_GYOKBN_PN, nChar );
}

// 変動事由ターミネーション
long CScanKeyView::OnTermComboHjiyu( long nChar )
{
	return term_focus( SCN_HJIYU_PN, nChar );
}

// NOT ﾁｪｯｸﾎﾞｯｸｽ か？
int CScanKeyView::IsNotChkbox( HWND hwnd )
{
	int ret = -1;
	int pn, id;

	for( pn = SNOT_SEQ_PN; pn <= SNOT_LAST_PN; pn++ ) {
		id = scGetINP_CTL( pn )->IDC_X;
		if( GetDlgItem( id )->m_hWnd == hwnd ) {
			ret = pn;
			break;
		}
	}

	return ret;
}

// NOT ﾁｪｯｸﾎﾞｯｸｽ ﾀｰﾐﾈｰｼｮﾝ
long CScanKeyView::OnTermNotChkbox( int pn, long nChar )
{
	return term_focus( pn, nChar );
}

// 伝票種別チェックボックスか？
int CScanKeyView::IsDenpChkbox( HWND hwnd )
{
	int ret = -1;
	int pn, id;

	for( pn = SCN_CHKFURI_PN; pn <= SCN_CHKSYUK_PN; pn++ ) {
		id = scGetINP_CTL( pn )->IDC_X;
		if( GetDlgItem( id )->m_hWnd == hwnd ) {
			ret = pn;
			break;
		}
	}

	return ret;
}

// 伝票種別チェックボックス キーターミネーション
long CScanKeyView::OnTermDenpChkbox( int pn, long nChar )
{
	return term_focus( pn, nChar );
}

struct _KeyMoveTbl* CScanKeyView::Get_KeyMoveTbl( int nowpn )
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
long CScanKeyView::term_focus( int nowpn, long nChar )
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
	case SCN_GYOKBN_PN:		pCmb = &m_GyoKbn;	break;
	case SCN_HJIYU_PN:		pCmb = &m_KSHcombo;	break;
	case SCN_DOCSHTP_PN:	pCmb = &m_DShtCombo;break;
	case SCN_SYZKTSN_PN:	pCmb = &m_SyzKtsn;break;
	case SCN_DTTYPE_PN:		pCmb = &m_DTtypeCmb;	break;
	case SCN_MENWARI_PN:	pCmb = &m_MenWariCmb;	break;
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


void CScanKeyView::OnSelchangeDisporder() 
{
	int sel = m_Order.GetCurSel();

	SCAN_KEY.SCN_ORDER_DT.SKBN_SGN1 = sel;
	SCAN_KEY.LINE_CND[ SCN_ORDER_PN ].INP_sg = TRUE;
}

void CScanKeyView::OnSelchangeDataUser() 
{
	int sel = m_DUser.GetCurSel();

	SCAN_KEY.SCN_DUSER_DT.SKBN_SGN1 = sel;
	SCAN_KEY.LINE_CND[ SCN_DUSER_PN ].INP_sg = TRUE;

	SelchangeDuserJob( sel );
}


void CScanKeyView::OnSelchangeComboStype() 
{
	int sel = m_SType.GetCurSel();

	SCAN_KEY.SCN_STYPE_DT.SKBN_SGN1 = sel;
	SCAN_KEY.LINE_CND[ SCN_STYPE_PN ].INP_sg = TRUE;
}

void CScanKeyView::OnSelchangeScnhusen() 
{
	int sel = m_Hcbo.GetCurSel();

	if( sel == 0 ) {
		SCAN_KEY.SCN_HUSEN_DT.SKBN_SGN1 = 0;
		strcpy_s( SCAN_KEY.SCN_HUSEN_DT.SKBN_TXT, sizeof SCAN_KEY.SCN_HUSEN_DT.SKBN_TXT, "" );
		SCAN_KEY.LINE_CND[ SCN_HUSEN_PN ].INP_sg = FALSE;
	}
	else {
		SCAN_KEY.SCN_HUSEN_DT.SKBN_SGN1 = sel;
		SCAN_KEY.LINE_CND[ SCN_HUSEN_PN ].INP_sg = TRUE;	
	}
}


// 付箋コメント
void CScanKeyView::OnSetFocusScnHUcmt()
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		set_nowpn( SCN_HUCMT_PN );
	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
	OnICSInputFocus( SCN_HUCMT_PN ); 
}

void CScanKeyView::OnKillFocusScnHUcmt( long inplen )
{
	HUcmtInp( SCN_HUCMT_PN, &SCAN_KEY.SCN_HUCMT_DT, 0, inplen, 0);
}

void CScanKeyView::OnTerminationScnHUcmt(long nChar, long inplen, long kst)
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = HUcmtInp( SCN_HUCMT_PN, &SCAN_KEY.SCN_HUCMT_DT, nChar, inplen, kst)) )
			return;

		switch( nChar ) {
		case VK_RIGHT:
			if( ! BRmst && ! BMON_MST ) {
				set_focus( SCN_IDOK );
				return;
			}
			break;
		default:
			break;
		}

		//カーソル移動
		if( term_focus( SCN_HUCMT_PN, nChar ) != 0 )
			TermBuzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}	
}

long CScanKeyView::HUcmtInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst )
{
	struct _ItemData check;
	char	buf[30] = {0};
	VARIANT var;

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

		if( var.pbVal[0] != 0 )	strcpy_s( (char*)check.IP_HUCMT, sizeof check.IP_HUCMT, (char*)var.pbVal );
		else					::ZeroMemory( check.IP_HUCMT, sizeof check.IP_HUCMT );

		if( check.IP_HUCMT[0] != 0 )
		{
			SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
			strcpy_s( (char*)keybuf->IP_HUCMT, sizeof keybuf->IP_HUCMT, (char*)check.IP_HUCMT );
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
		var.pbVal = NULL;
		DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	
		::ZeroMemory( keybuf->IP_HUCMT, sizeof keybuf->IP_HUCMT );
		SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );

}


// 入力漏れ
void CScanKeyView::OnSelchangeScnnoinpbrn() 
{
	int sel = m_NipBrn.GetCurSel();
	/*---VER2---*/
	DWORD dwItem = m_NipBrn.GetItemData( sel );

	SCAN_KEY.SCN_NOINP_DT.SKBN_SGN1 = (short)dwItem;
	if( sel )	SCAN_KEY.LINE_CND[ SCN_NOINP_PN ].INP_sg = TRUE;
	else		SCAN_KEY.LINE_CND[ SCN_NOINP_PN ].INP_sg = FALSE;
}

// 業種の区分
void CScanKeyView::OnSelchangeScnGyoKbn() 
{
	int sel = m_GyoKbn.GetCurSel();

	DWORD dwItem = m_GyoKbn.GetItemData( sel );

	SCAN_KEY.SCN_GYOKBN_DT.SKBN_SGN1 = (short)dwItem;
	if( sel )	SCAN_KEY.LINE_CND[ SCN_GYOKBN_PN ].INP_sg = TRUE;
	else		SCAN_KEY.LINE_CND[ SCN_GYOKBN_PN ].INP_sg = FALSE;
}

// 変動事由
void CScanKeyView::OnCbnSelchangeScnhjiyu()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int sel		= m_KSHcombo.GetCurSel();
	DWORD item	= m_KSHcombo.GetItemData( sel );

	if( sel > 0 ) {
		SCAN_KEY.LINE_CND[SCN_HJIYU_PN].INP_sg = TRUE;
		SCAN_KEY.SCN_HJIYU_DT.SKBN_SGN1 = (BYTE)sel;	
		SCAN_KEY.SCN_HJIYU_DT.SKBN_SGN2 = (BYTE)item;	//変動事由コード
	}
	else {
		SCAN_KEY.LINE_CND[SCN_HJIYU_PN].INP_sg = FALSE;
		SCAN_KEY.SCN_HJIYU_DT.SKBN_SGN1 = 0;	
		SCAN_KEY.SCN_HJIYU_DT.SKBN_SGN2 = 0;
	}
}

void CScanKeyView::OnSize(UINT nType, int cx, int cy) 
{
	if( ((CMainFrame*)GetDBDinpMainFrame())->IgnoreOnSizeFlag() )
		return;

	CDBbaseView::OnSize(nType, cx, cy);
	
	// TODO: この位置にOnSizeメッセージ ハンドラ用のコードを追加してください
	if( m_DIALOG_OK )
	{
#ifdef DB_OLD_CLOSE
		//表題を窪み表示
		selbox( get_nowpn() );
#endif
		InvoiceCtrlMove();

		// コメント表示
		cmntbox( get_nowpn() );

		if( is_mdfy_imgtky() == TRUE )	// イメージ摘要のｴﾃﾞｨｯﾄﾎﾞｯｸｽON
			disp_imgtkyedit( SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_type );
		else
			disp_imgtkyedit( STK_NORMAL );

		//付箋コメント20文字対応
		set_husencmt();

		PostMessage( WM_RETROFOCUS, get_nowpn() );	// 現在行枝番摘要表示のため
//		dsp_etc( get_nowpn(), TRUE );
		_disp_change();
		dsp_etc( get_nowpn(), TRUE );

		set_docevi();
		set_DataType();

		_disp_caution(TRUE);
	}
}

#ifdef CLOSE

//--------------------------------
// その他項目　表示
//--------------------------------
BOOL CScanKeyView::_dsp_etc( CWnd *pwnd, struct _ItemData *data, struct _LINE_CND *cnd, struct _InpCtl *ctl, double x_rate, BOOL dspsw, BOOL focusoff_disp )
{
CWnd *itemorg;
CWnd *etcorg;
CWnd *prevorg;
CWnd *kmkorg;
WINDOWPLACEMENT itemplace, kmp;
RECT newrect, kmrect;
int	wd, height;
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
	VERIFY(itemorg = pwnd->GetDlgItem( ctl->IDC_X ));

	struct _InpCtl *kmctl;
	kmctl = scGetINP_CTL( SCN_DEBT_PN );
	VERIFY(kmkorg = pwnd->GetDlgItem( kmctl->IDC_X ));

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
			kmkorg->GetWindowPlacement( &kmp );
			kmrect = kmp.rcNormalPosition;
			wd = kmrect.right - kmrect.left;
			wd *= 2;
			height = newrect.bottom - newrect.top;
			height += (newrect.bottom - newrect.top) / 4;
			etcorg->SetWindowPos( &pwnd->wndTop, newrect.left, newrect.top - height, wd, height, SWP_NOACTIVATE );

			dispdata.backcl = BRTEK_BackCOL;
			dispdata.forecl = cnd->ETC_TXT_ForeCOL;		// 文字色;

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
BOOL CScanKeyView::fix8cnd()
{
	return fix8sg;
}

void CScanKeyView::fix8cnd( BOOL sg )
{
	fix8sg = sg;
}

// 選択画面表示色状態
BOOL CScanKeyView::fixvalcnd()
{
	return fixvalsg;
}

void CScanKeyView::fixvalcnd( BOOL sg )
{
	fixvalsg = sg;
}


//
// 固定項目の表示
void CScanKeyView::fix8disp( BOOL sg )
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
void CScanKeyView::enable_fix8( BOOL sg )
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
void CScanKeyView::enable_fixval( BOOL sg )
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
void CScanKeyView::set_valtitle( char *msg )
{
	puts( IDC_VALTITLE, msg, CICSDisp );
}

// 選択タイトル表示のディセーブル／イネーブル
void CScanKeyView::valtitle( BOOL sg )
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

#endif


/////////////////////////////////////////////////////////////////////////////////////

void CScanKeyView::OnCheckDbm() 
{
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

	ChangeEnableDCbmn();
//	SCAN_KEY.LINE_CND[SCN_BMON1_PN].KEY_inp = chk;
}

void CScanKeyView::OnCheckCbm() 
{
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

	ChangeEnableDCbmn();
//	SCAN_KEY.LINE_CND[SCN_CBMN1_PN].KEY_inp = chk;
}

void CScanKeyView::OnCheckDkj() 
{
	int chk = ((CButton*)GetDlgItem(IDC_CHECK_DKJ))->GetCheck();
	if( Voln1->apno == 0x20 && (Voln1->sub_sw&0x04) ) {
		m_Koji1.EnableWindow( !chk );
		scGetINP_CTL(SCN_KOJI1_PN)->INP_ena = !chk;
		m_Koji2.EnableWindow( !chk );
		scGetINP_CTL(SCN_KOJI2_PN)->INP_ena = !chk;

		if( chk )	SCAN_KEY.LINE_CND[SCN_KOJI1_PN].KEY_inp |= SC_ITEMNONE;
		else		SCAN_KEY.LINE_CND[SCN_KOJI1_PN].KEY_inp	&= ~SC_ITEMNONE;

		ChangeEnableDCkji();
	}
//	SCAN_KEY.LINE_CND[SCN_KOJI1_PN].KEY_inp = chk;
}


void CScanKeyView::OnCheckCkj() 
{
	int chk = ((CButton*)GetDlgItem(IDC_CHECK_CKJ))->GetCheck();
	if( Voln1->apno == 0x20 && (Voln1->sub_sw&0x04) ) {
		m_Ckji1.EnableWindow( !chk );
		scGetINP_CTL(SCN_CKJI1_PN)->INP_ena = !chk;
		m_Ckji2.EnableWindow( !chk );
		scGetINP_CTL(SCN_CKJI2_PN)->INP_ena = !chk;

		if( chk )	SCAN_KEY.LINE_CND[SCN_CKJI1_PN].KEY_inp |= SC_ITEMNONE;
		else		SCAN_KEY.LINE_CND[SCN_CKJI1_PN].KEY_inp	&= ~SC_ITEMNONE;

		ChangeEnableDCkji();
	}
//	SCAN_KEY.LINE_CND[SCN_CKJI1_PN].KEY_inp = chk;
}

void CScanKeyView::OnCheckDbr() 
{
	int chk = ((CButton*)GetDlgItem(IDC_CHECK_DBR))->GetCheck();

	if( (Voln1->br_sw & 0x01) ) {
		m_Dbr1.EnableWindow( !chk );
		scGetINP_CTL(SCN_DBR1_PN)->INP_ena = !chk;
		m_Dbr2.EnableWindow( !chk );
		scGetINP_CTL(SCN_DBR2_PN)->INP_ena = !chk;

		if( chk )	SCAN_KEY.LINE_CND[SCN_DBR1_PN].KEY_inp |= SC_ITEMNONE;
		else		SCAN_KEY.LINE_CND[SCN_DBR1_PN].KEY_inp	&= ~SC_ITEMNONE;
		ChangeEnableDcbr();
	}
//	SCAN_KEY.LINE_CND[SCN_DBR1_PN].KEY_inp = chk;
}

void CScanKeyView::OnCheckCbr() 
{
	int chk = ((CButton*)GetDlgItem(IDC_CHECK_CBR))->GetCheck();

	if( (Voln1->br_sw & 0x01) ) {
		m_Cbr1.EnableWindow( !chk );
		scGetINP_CTL(SCN_CBR1_PN)->INP_ena = !chk;
		m_Cbr2.EnableWindow( !chk );
		scGetINP_CTL(SCN_CBR2_PN)->INP_ena = !chk;

		if( chk )	SCAN_KEY.LINE_CND[SCN_CBR1_PN].KEY_inp |= SC_ITEMNONE;
		else		SCAN_KEY.LINE_CND[SCN_CBR1_PN].KEY_inp	&= ~SC_ITEMNONE;

		ChangeEnableDcbr();
	}
//	SCAN_KEY.LINE_CND[SCN_CBR1_PN].KEY_inp = chk;
}


// 貸／借枝番 入力可否の切り替え
void CScanKeyView::ChangeEnableDcbr() 
{
	int deb_chk = ((CButton*)GetDlgItem(IDC_CHECK_DBR))->GetCheck();
	int cre_chk = ((CButton*)GetDlgItem(IDC_CHECK_CBR))->GetCheck();

	BOOL bEnable;
	bEnable = !(deb_chk || cre_chk);

	m_DCbr1.EnableWindow( bEnable );
	scGetINP_CTL(SCN_DCBR1_PN)->INP_ena = bEnable;
	m_DCbr2.EnableWindow( bEnable );
	scGetINP_CTL(SCN_DCBR2_PN)->INP_ena = bEnable;

}


// 貸借 部門 入力可否の切り替え
void CScanKeyView::ChangeEnableDCbmn() 
{
	int deb_chk = ((CButton*)GetDlgItem(IDC_CHECK_DBM))->GetCheck();
	int cre_chk = ((CButton*)GetDlgItem(IDC_CHECK_CBM))->GetCheck();

	BOOL bEnable;
	bEnable = !(deb_chk || cre_chk);

	m_DCbmn1.EnableWindow( bEnable );
	scGetINP_CTL(SCN_DCBMN1_PN)->INP_ena = bEnable;
	m_DCbmn2.EnableWindow( bEnable );
	scGetINP_CTL(SCN_DCBMN2_PN)->INP_ena = bEnable;
}


// 貸借 工事 入力可否の切り替え
void CScanKeyView::ChangeEnableDCkji() 
{
	int deb_chk = ((CButton*)GetDlgItem(IDC_CHECK_DKJ))->GetCheck();
	int cre_chk = ((CButton*)GetDlgItem(IDC_CHECK_CKJ))->GetCheck();

	BOOL bEnable;
	bEnable = !(deb_chk || cre_chk);

	m_DCkji1.EnableWindow( bEnable );
	scGetINP_CTL(SCN_DCKJI1_PN)->INP_ena = bEnable;
	m_DCkji2.EnableWindow( bEnable );
	scGetINP_CTL(SCN_DCKJI2_PN)->INP_ena = bEnable;
}



void CScanKeyView::OnSelectedIcsselctrl1(short position, long Number) 
{
	SelectedIcsselctrl( position, Number, 0 );
}

void CScanKeyView::OnSelectedIcsselctrl2(short position, long Number) 
{
	SelectedIcsselctrl( position, Number, 1 );	
}

void CScanKeyView::OnAllkeydel() 
{
	ScanKeyAllDel();
}




void CScanKeyView::OnBnClickedNchkSyzsw()
{
	// TODO: ここにOnBnClickedNchkSyzswコントロール通知ハンドラ コードを追加します。
}


void CScanKeyView::OnDraw(CDC* /*pDC*/)
{
	if( !FirstTime() )	return;
	FirstTime()	=	FALSE;

//	BeginApp();
}


void CScanKeyView::BeginApp()
{
	// 枝番摘要表示コントロールのサイズをゼロ（見えなくする）に
	CWnd* org;
	VERIFY(org = GetDlgItem( IDC_DISPETC ));
	org->SetWindowPos( 0, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOMOVE );

	CDBbaseView::InitWork(FALSE);

	scanitem_initEX( &SC_Item );
	if( pDBzm->data_authority() ) {
		SC_Item.sc_duser = SCD_ALLDATA;
	}

	// 日付設定
	// スキャン部分は保存しない
	m_selDate.SetMode(ICS_DATESELECT_MODE_NONE);
	m_selDate.Iniz( pDBzm, ICS_DATESELECT_RANGE_PERIOD|ICS_DATESELECT_EDIT_ON|ICS_DATESELECT_EDIT_MD|ICS_DATESELECT_INPOK_ONLY, 0, 0 );
	
	//科目範囲
//	m_KmkType = 0;
	m_KmkType = ((CMainFrame*)GetDBDinpMainFrame())->SkeyKmkType();

	keyline_inz( FALSE );

	char pbuf[128];

	// コントロールテーブルの初期化
	inpctlline_inz( INP_CTL );

	// 検索対象データ
	if( ! pDBzm->data_authority() ) {
		m_DUser.SetCurSel(1);
		m_DUser.EnableWindow( FALSE );
		GetDlgItem(IDC_ST_DUSER)->EnableWindow(FALSE);
		scGetINP_CTL( SCN_DUSER_PN )->INP_ena = FALSE;

		SelchangeDuserJob( 1, 1 );
	}
	else {
		m_DUser.SetCurSel(0);
		m_DUser.EnableWindow( TRUE );
		GetDlgItem(IDC_ST_DUSER)->EnableWindow(TRUE);
		scGetINP_CTL( SCN_DUSER_PN )->INP_ena = TRUE;

		SelchangeDuserJob( 0, 1 );
	}

	// 変動事由の選択 03.23 /06
	int i, n;
	KSH_SCAN ksh;
	m_HenCnt = m_Seldata.ScanKHKbnMake();
	m_KSHcombo.ResetContent();
	if( m_HenCnt > 0 ) {
		m_KSHcombo.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ST_HJIYU)->ShowWindow(SW_SHOW);
		for( i = 0; i < m_HenCnt; i++ ) {
			if( m_Seldata.GetScanKHKbn( i, &ksh ) == 0 ) {
				m_KSHcombo.AddString( ksh.ks_name );
				m_KSHcombo.SetItemData( i, ksh.item );
			}
		}
		scGetINP_CTL( SCN_HJIYU_PN )->INP_ena = TRUE;

		sprintf_s( pbuf, "%s:", pDBzm->get_hjiyutitle(2) );
		GetDlgItem( IDC_ST_HJIYU )->SetWindowText( pbuf );
	}
	else {
		m_KSHcombo.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ST_HJIYU)->ShowWindow(SW_HIDE);

		scGetINP_CTL( SCN_HJIYU_PN )->INP_ena = FALSE;
	}

	// 消費税セット
	def_syselinz();

	// 摘要文字数
//	sprintf_s( pbuf, sizeof pbuf, "摘要(%d):", Voln1->tk_ln );
//	GetDlgItem( IDC_ST_TKY )->SetWindowText( pbuf );

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

	m_Tkycb.ResetContent();
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
	
	m_Hcbo.SetMode( HUMD_SCAN );
	m_Hcbo.InitHusenComboEX();
//	m_DIALOG_OK = TRUE;

	SelPar par;
	par.sel_pn = SL_KAMOKU_PN;
	m_Seldata.SelectJob( &par, -1 );

	// 消費税マスターごとの設定部分
	set_syselinz();
	// 証憑番号 11.26 /12
	set_docevi();
	//インボイス対応
	InvoiceCtrlMove(1);
	// 付箋コメント２０文字対応
	set_husencmt();

	set_DocSeqAndShtyp();

	BOOL bEntry = GET_EntrySign();
//	BOOL bEntry = TRUE;
	if( IsEnkakuUser() || bEntry || IsJozuEnvCloudMaster() ) {
		m_bDataTypeCombo = FALSE;
		scGetINP_CTL( SCN_DTTYPE_PN )->INP_ena = FALSE;
	}
	else {
		m_bDataTypeCombo = TRUE;
		scGetINP_CTL( SCN_DTTYPE_PN )->INP_ena = TRUE;
	}
	set_DataType();

	keyline_dsp( INP_CTL, &SCAN_KEY );

	/*---VER2---*/
	// 入力漏れ
	i = 0;
	m_NipBrn.ResetContent();
	m_NipBrn.AddString("");
	m_NipBrn.SetItemData( i, 0 );
	i++;
	
	if( BRmst ) {
		m_NipBrn.AddString("枝番");
		m_NipBrn.SetItemData( i, SC_NOINP_BRN );
		i++;
	}
	if( (pDBzm->zvol->sub_sw&0x02) ) {
		m_NipBrn.AddString("部門");
		m_NipBrn.SetItemData( i, SC_NOINP_BMON );
		i++;
	}

	if( BRmst && (pDBzm->zvol->sub_sw&0x02) ) {
		m_NipBrn.AddString("部門(+枝番)");
		m_NipBrn.SetItemData( i, SC_NOINP_BMNBRN );
		i++;

		m_NipBrn.AddString("部門or枝番");
		m_NipBrn.SetItemData( i, SC_NOINP_BMORBR );
		i++;
	}
	if( (pDBzm->zvol->sub_sw&0x04) ) {
		m_NipBrn.AddString("工事番号");
		m_NipBrn.SetItemData( i, SC_NOINP_KOJI );
		i++;
	}

	// 01.28 /14
	m_NipBrn.AddString("伝票番号");
	m_NipBrn.SetItemData( i, SC_NOINP_DENP );
	i++;

	// インボイス対応
	if( bInvMaster && bInvUse && bInvDisp ) {
		m_NipBrn.AddString( _T( "登録番号" ));
		m_NipBrn.SetItemData( i, SC_NOINP_INVNO );
		i++;
	}

	// 業種の区分　08.19 /08
	BOOL bComboShow = FALSE;
	int type =  IsKojinGyosyuMaster( pDBzm );
	if( type ) {
		if( IsKojinKengyoMaster( type ) ) {
			bComboShow = TRUE;
			GetDlgItem(IDC_ST_GYOKBN)->ShowWindow( SW_SHOW );
			m_GyoKbn.ShowWindow( SW_SHOW );

			m_GyoKbn.ResetContent();
			m_GyoKbn.AddString("全業種");
			i = 0;
			m_GyoKbn.SetItemData( i, 0 );
			i++;

			if( type& 0x01) {
				m_GyoKbn.AddString("一般");
				m_GyoKbn.SetItemData( i, SC_GYOKBN_IPPAN );
				i++;
			}
			if( type& 0x02) {
				m_GyoKbn.AddString("不動産");
				m_GyoKbn.SetItemData( i, SC_GYOKBN_HUDOSAN );
				i++;
			}
			if( type& 0x04) {
				m_GyoKbn.AddString("農業");
				m_GyoKbn.SetItemData( i, SC_GYOKBN_NOGYO );
				i++;
			}

			m_GyoKbn.SetCurSel(0);
			scGetINP_CTL( SCN_GYOKBN_PN )->INP_ena = TRUE;
		}
	}
	if( ! bComboShow ) {
		GetDlgItem(IDC_ST_GYOKBN)->ShowWindow( SW_HIDE );
		m_GyoKbn.ShowWindow( SW_HIDE );

		scGetINP_CTL( SCN_GYOKBN_PN )->INP_ena = FALSE;
	}

	set_nowpn( SCN_ORDER_PN );	// 選択項目背景色消去のため
	// イメージ摘要エディットOFF
	disp_imgtkyedit( SCAN_KEY.LINE_CND[ SCN_TKY_PN ].INP_type );

	// 摘要のみ[確定マスターは無効]
	m_TKonly.EnableWindow( bCONFIRM_MASTER ? FALSE : TRUE );

	/*---VER2---*/
	InitTkyReport();
	m_idxTkyReport = -1;

	//伝票選択クリア
	CButton* btn;
	btn = (CButton*)GetDlgItem( IDC_FURI_CHK );
	btn->SetCheck(0);
	btn = (CButton*)GetDlgItem( IDC_NYUK_CHK );
	btn->SetCheck(0);
	btn = (CButton*)GetDlgItem( IDC_SYUK_CHK );
	btn->SetCheck(0);
	
	syzInvoice.SetDBZmSub( pDBzm );
	InvoiceKeyMoveChange();

	m_DIALOG_OK = TRUE;

	_disp_caution( TRUE );
}

/*---VER2---*/
// リポートコントロール設定
void CScanKeyView::InitTkyReport()
{
	//リサイズ設定はカラム設定より前に行う必要がある
	m_Report.Resize( TRUE, ICSResizeDefault );

	m_Report.RemoveAllRecords();
	m_Report.RemoveColumn(-1);

	m_Report.SetDraggable(FALSE);
	m_Report.SetRemovable(FALSE);

	CRect rect;

	m_Tky.GetWindowRect( &rect );

	//カラムの設定
	m_Report.AddColumn( new ICSReportColumn( 0, _T("複数摘要の検索指定 [Del: 摘要削除]"), rect.Width(), TRUE, ICS_REPORT_NOICON, FALSE, TRUE, DT_RIGHT ) );
	
/*
	ICSReportColumns* clms = m_Report.GetColumns();
	for( int n = 0; n < 3; n++ ) {
		ICSReportColumn* clm = clms->GetAt(n);
		clm->SetAlignment( DT_CENTER );
	}
*/
	m_Report.SetColumnBackColor( 0,  RGB_PAPER_WHITE, FALSE);

	m_Report.SetGridColor( REPORT_GRID_VERTICAL,	RGB_GRAY );
	m_Report.SetGridColor( REPORT_GRID_HORIZONTAL,	RGB_GRAY );
	m_Report.SetGridStyle( REPORT_GRID_VERTICAL, RGS_SOLID );

	m_Report.SetGridPerCount( FALSE, 5, RGB_BLUE, RGS_SOLID );

	CFont* pFont;
//	pFont = m_Tab.GetFont();
	pFont = GetDlgItem(IDC_ST_ORDER)->GetFont();
	LOGFONT lf;
	pFont->GetLogFont(&lf);

	m_Report.SetTextFont( lf );
	m_Report.SetHeaderFont( lf );

	int ht = abs( lf.lfHeight );
	ht += (ht / 3);

	m_Report.SetHeaderHeight(ht);
	m_Report.SetItemHeight(ht);

	m_Report.AllowEdit(TRUE);
}

void CScanKeyView::to_scankey()
{
	INP_mode = _RETREV;

	// 複数摘要検索
	if( SC_Tky.GetCount() == 0 ) {
		m_Report.RemoveAllRecords();
		m_Tky.SetBackColor( RGB_WHITE );
	}

	// 取消仕訳ボタンの表示
	if( pAUTOSEL && pAUTOSEL->DELSWK_OPT == OPT_DELSWK )
		GetDlgItem( IDC_BTN_DEL )->ShowWindow( SW_HIDE );
	else
		GetDlgItem( IDC_BTN_DEL )->ShowWindow( TRUE );

	//科目カナ検索
	KanaSearchUpdate();

	set_focus( SCN_ORDER_PN );
}


// 検索開始
void CScanKeyView::OnButtonF8()
{
	if( get_nowpn() == SCN_TKY_PN )
		c_set( IDOK );

	ButtonF8Job();
}

//重複仕訳の検索サインOFF
void CScanKeyView::ButtonF8Job(int duplioff/*=1*/)
{
	if( INP_mode != _RETREV ) {
		ASSERT( FALSE );
		return;
	}
	if( duplioff ) {
		pDBzm->IsDupliScan() = FALSE;
	}

	CalcMouseEvent();

	// 最終データ取得
	last_data_set();

	PROGRAM_OK = FALSE;	// View切替が終わるまで、このビューでのキーを棄てる。
	// 一括修正スキャン中のサインOFF
	if( bIKAT_SCAN ) {
		bIKAT_SCAN = FALSE;
	}				

	gotokeyscan( /*TRUE*/);
}


//電卓ウィンドウの消去
void CScanKeyView::CalcMouseEvent()
{
	int pn = get_nowpn();

	BOOL bCalcWndC = FALSE;

	switch( pn ) {
	case SCN_VAL1_PN:
	case SCN_VAL2_PN:
	case SCN_ZEI1_PN:
	case SCN_ZEI2_PN:
	case SCN_TAI1_PN:
	case SCN_TAI2_PN:

		CWnd* pwnd;
		pwnd = GetFocus();

		CRect rct;
		if( pwnd ) {
			if( pn == SCN_VAL1_PN && m_Val1.m_hWnd != pwnd->m_hWnd )
				bCalcWndC = TRUE;
			else if( pn == SCN_VAL2_PN && m_Val2.m_hWnd != pwnd->m_hWnd )
				bCalcWndC = TRUE;
			else if( pn == SCN_ZEI1_PN && m_Zei1.m_hWnd != pwnd->m_hWnd )
				bCalcWndC = TRUE;
			else if( pn == SCN_ZEI2_PN && m_Zei2.m_hWnd != pwnd->m_hWnd )
				bCalcWndC = TRUE;
			else if( pn == SCN_TAI1_PN && m_Tai1.m_hWnd != pwnd->m_hWnd )
				bCalcWndC = TRUE;
			else if( pn == SCN_TAI2_PN && m_Tai2.m_hWnd != pwnd->m_hWnd )
				bCalcWndC = TRUE;

			if( bCalcWndC ) {
				::SendMessage( pwnd->m_hWnd, WM_KEYDOWN, VK_ESCAPE, 0 );
			}
		}
	}
}


// スキャン開始
void CScanKeyView::gotokeyscan()
{
	int rno;

	// 入力項目取得
	if( get_skeyitem( 0 ) != 0 ) {
		PROGRAM_OK = TRUE;
		return;
	}

	inpmdisp( _KEYSCAN );

	SCAN_ITEMEX* pSI;
	pSI = &SC_Item;

	if( pSI->sc_seq1 != -1 )
		rno = pSI->sc_seq1 - 1;
	else
		rno = 0;

	if( pDBzm->IsDupliScan() ) {
		((CMainFrame*)GetDBDinpMainFrame())->ChgToScanView(0, rno, 0);
		return;
	}

	// 伝票検索へ
	if( pSI->sc_denpopt & (SC_FURIDEN | SC_NYUKIN | SC_SYUKKIN) ) {
		if( ! ((CMainFrame*)GetDBDinpMainFrame())->ScanDenpCheck() ) {
			PROGRAM_OK = FALSE;
			//伝票形式に戻る
			((CMainFrame*)GetDBDinpMainFrame())->ChgToDenpView(FALSE);
			PROGRAM_OK = TRUE;
			return;
		}
		((CMainFrame*)GetDBDinpMainFrame())->ChgToDenpView();
	}
	else {
		((CMainFrame*)GetDBDinpMainFrame())->ChgToScanView( 0, rno, 0 );
	}
}

// 取消仕訳検索 のツールバー状態
void CScanKeyView::OnUpdateButtonF9(CCmdUI *pCmdUI)
{
	if( pAUTOSEL ) {
		pCmdUI->Enable( !(pAUTOSEL->DELSWK_OPT == OPT_DELSWK) );
	}
	else {
		pCmdUI->Enable( TRUE );
	}
}


// 取消仕訳検索
void CScanKeyView::OnButtonF9()
{
	if( ! PROGRAM_OK )	return;

	CalcMouseEvent();

	last_data_set();

	// 入力項目取得
	if( get_skeyitem( 1 ) != 0 )
		return;

	inpmdisp( _KEYSCAN );

	SCAN_ITEMEX* pSI;
	pSI = &SC_Item;
	int rno;

	if( pSI->sc_seq1 != -1 )
		rno = pSI->sc_seq1 - 1;
	else
		rno = 0;

	// 伝票検索へ
	((CMainFrame*)GetDBDinpMainFrame())->ChgToScanView( 0, rno, 1 );

}


// 取消仕訳 を検索
void CScanKeyView::OnBnClickedBtnDel()
{
	OnButtonF9();
}


void CScanKeyView::skeyitem_init()
{
	scanitem_initEX( &SC_Item );
}

// 検索用 入力項目を取得
//	int scan_sgn	0: 通常の検索
//					1: 取消仕訳 検索
//
int CScanKeyView::get_skeyitem( int scan_sgn )
{
	// NOT ﾁｪｯｸﾎﾞｯｸｽ
	CButton* pbtn;
	struct _InpCtl *pIctl;
	int sgn, id;

	for( int pn = SNOT_SEQ_PN; pn <= SNOT_LAST_PN; pn++ ) {
		pIctl = scGetINP_CTL( pn );
		id = pIctl->IDC_X;
		if( pIctl->INP_ena ) {
			pbtn = (CButton*)GetDlgItem( id );
			sgn = pbtn->GetCheck();

			NotCheckSet( pn, sgn );
		}
	}

	struct _SKEY_LINE* ps;
	SCAN_ITEMEX* pSI;
	ps = &SCAN_KEY;
	pSI = &SC_Item;

	// 表示順
	switch( ps->SCN_ORDER_DT.SKBN_SGN1 ) {
	case 0:		pSI->sc_type = SCT_ORDERSEQ;	break;
	default:	pSI->sc_type = SCT_ORDERDATE;	break;
	}
#ifdef OLD_CLOSE
	if( m_bTKYONLY )
		pSI->sc_type |= SCT_TKYONLY;
	else
		pSI->sc_type &= ~SCT_TKYONLY;
#endif

	// 対象データ
	switch( ps->SCN_DUSER_DT.SKBN_SGN1 ) {
	case 0:		pSI->sc_duser = SCD_ALLDATA;	break;
	default:	pSI->sc_duser = SCD_CURUESR;	break;
	}

	pSI->sc_dtype = 0;

	if( pSI->sc_duser == SCD_CURUESR ) {
		pbtn = (CButton*)GetDlgItem( IDC_CHK_INPDATA );
		sgn = pbtn->GetCheck();
		if( sgn == BST_CHECKED )
			pSI->sc_dtype |= 0x01;

		pbtn = (CButton*)GetDlgItem( IDC_CHK_MDFDATA );
		sgn = pbtn->GetCheck();
		if( sgn == BST_CHECKED )
			pSI->sc_dtype |= 0x02;

		if( pSI->sc_dtype == 0 ) {
			ICSMessageBox( "検索する対象データを指定してください！\r\n（入力仕訳・修正仕訳）" );
			return 1;
		}
	}

	// 仕訳データタイプ
	if( m_DTtypeCmb.IsWindowVisible() ) {
		if( ps->LINE_CND[ SCN_DTTYPE_PN ].INP_sg )
			pSI->sc_mikad = ps->SCN_DTTYPE_DT.SKBN_SGN1;
		else
			pSI->sc_mikad = 0;
	}
	else {
		pSI->sc_mikad = 0;
	}

	// 対象データ
	switch( ps->SCN_STYPE_DT.SKBN_SGN1 ) {
	case 0:		pSI->sc_stype = SCS_AND;		break;
	default:	pSI->sc_stype = SCS_OR;			break;
	}

	// SEQ番号
	if( ps->LINE_CND[ SCN_SEQ1_PN ].INP_sg )
		pSI->sc_seq1 = ps->SCN_SEQ1_DT.IP_SEQ;
	else
		pSI->sc_seq1 = -1;
	if( ps->LINE_CND[ SCN_SEQ2_PN ].INP_sg )
		pSI->sc_seq2 = ps->SCN_SEQ2_DT.IP_SEQ;
	else
		pSI->sc_seq2 = -1;

	pSI->sc_seqopt = 0;
	if( ps->LINE_CND[ SCN_SEQ1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_seqopt |= SC_NOTSEARCH;
	}
	// 日付
	DateSelectToSkeyLine();

/*	if( ps->LINE_CND[ SCN_DATE1_PN ].INP_sg )
		memcpy( pSI->sc_date1, ps->SCN_DATE1_DT.IP_BCDDAY, 2 );
	else
		::ZeroMemory( pSI->sc_date1, sizeof pSI->sc_date1 );
	if( ps->LINE_CND[ SCN_DATE2_PN ].INP_sg )
		memcpy( pSI->sc_date2, ps->SCN_DATE2_DT.IP_BCDDAY, 2 );
	else
		::ZeroMemory( pSI->sc_date2, sizeof pSI->sc_date2 );
	pSI->sc_dateopt = 0;
	if( ps->LINE_CND[ SCN_DATE1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_dateopt |= SC_NOTSEARCH;
	}
*/
	// 伝票番号
	if( ps->LINE_CND[ SCN_DENP1_PN ].INP_sg )
		pSI->sc_denp1 = ps->SCN_DENP1_DT.IP_DENP;
	else
		pSI->sc_denp1 = -1;
	if( ps->LINE_CND[ SCN_DENP2_PN ].INP_sg )
		pSI->sc_denp2 = ps->SCN_DENP2_DT.IP_DENP;
	else
		pSI->sc_denp2 = -1;
	pSI->sc_denpopt = 0;
	if( ps->LINE_CND[ SCN_DENP1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_denpopt |= SC_NOTSEARCH;
	}
	// 伝票種別
	CButton* btn;
	btn = (CButton*)GetDlgItem( IDC_FURI_CHK );
	int st = btn->GetCheck();
	if( st == BST_CHECKED )
		pSI->sc_denpopt |= SC_FURIDEN;
	btn = (CButton*)GetDlgItem( IDC_NYUK_CHK );
	st = btn->GetCheck();
	if( st == BST_CHECKED )
		pSI->sc_denpopt |= SC_NYUKIN;
	btn = (CButton*)GetDlgItem( IDC_SYUK_CHK );
	st = btn->GetCheck();
	if( st == BST_CHECKED )
		pSI->sc_denpopt |= SC_SYUKKIN;

	//借方部門
	if( ps->LINE_CND[ SCN_BMON1_PN ].INP_sg )
		pSI->sc_dbmn1 = ps->SCN_BMON1_DT.IP_BMN;
	else
		pSI->sc_dbmn1 = -1;
	if( ps->LINE_CND[ SCN_BMON2_PN ].INP_sg )
		pSI->sc_dbmn2 = ps->SCN_BMON2_DT.IP_BMN;
	else
		pSI->sc_dbmn2 = -1;
	pSI->sc_dbmnopt = 0;
	if( ps->LINE_CND[ SCN_BMON1_PN ].KEY_inp & SC_ITEMNONE ) {
		pSI->sc_dbmnopt |= SC_ITEMNONE;
	}
	if( ps->LINE_CND[ SCN_BMON1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_dbmnopt |= SC_NOTSEARCH;
	}

	//貸方部門
	if( ps->LINE_CND[ SCN_CBMN1_PN ].INP_sg )
		pSI->sc_cbmn1 = ps->SCN_CBMN1_DT.IP_BMN;
	else
		pSI->sc_cbmn1 = -1;
	if( ps->LINE_CND[ SCN_CBMN2_PN ].INP_sg )
		pSI->sc_cbmn2 = ps->SCN_CBMN2_DT.IP_BMN;
	else
		pSI->sc_cbmn2 = -1;
	pSI->sc_cbmnopt = 0;
	if( ps->LINE_CND[ SCN_CBMN1_PN ].KEY_inp & SC_ITEMNONE ) {
		pSI->sc_cbmnopt |= SC_ITEMNONE;
	}
	if( ps->LINE_CND[ SCN_CBMN1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_cbmnopt |= SC_NOTSEARCH;
	}

	//貸借部門
	if( ps->LINE_CND[ SCN_DCBMN1_PN ].INP_sg )
		pSI->sc_dcbmn1 = ps->SCN_DCBMN1_DT.IP_BMN;
	else
		pSI->sc_dcbmn1 = -1;
	if( ps->LINE_CND[ SCN_DCBMN2_PN ].INP_sg )
		pSI->sc_dcbmn2 = ps->SCN_DCBMN2_DT.IP_BMN;
	else
		pSI->sc_dcbmn2 = -1;
	pSI->sc_dcbmnopt = 0;
	if( ps->LINE_CND[ SCN_DCBMN1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_dcbmnopt |= SC_NOTSEARCH;
	}

	// 借方工事
	if( ps->LINE_CND[ SCN_KOJI1_PN ].INP_sg ) {
		memcpy( pSI->sc_dkji1, ps->SCN_KOJI1_DT.IP_KOJI, sizeof pSI->sc_dkji1  );
		trim_zero( pSI->sc_dkji1, sizeof pSI->sc_dkji1, strlen( pSI->sc_dkji1 ));
	}
	else
		::ZeroMemory( pSI->sc_dkji1, sizeof pSI->sc_dkji1 );
	if( ps->LINE_CND[ SCN_KOJI2_PN ].INP_sg ) {
		memcpy( pSI->sc_dkji2, ps->SCN_KOJI2_DT.IP_KOJI, sizeof pSI->sc_dkji2  );
		trim_zero( pSI->sc_dkji2, sizeof pSI->sc_dkji2, strlen( pSI->sc_dkji2 ));
	}
	else
		::ZeroMemory( pSI->sc_dkji2, sizeof pSI->sc_dkji2 );
	pSI->sc_dkjiopt = 0;
	if( ps->LINE_CND[ SCN_KOJI1_PN ].KEY_inp & SC_ITEMNONE ) {
		pSI->sc_dkjiopt |= SC_ITEMNONE;
	}
	if( ps->LINE_CND[ SCN_KOJI1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_dkjiopt |= SC_NOTSEARCH;
	}

	// 貸方工事
	if( ps->LINE_CND[ SCN_CKJI1_PN ].INP_sg ) {
		memcpy( pSI->sc_ckji1, ps->SCN_CKJI1_DT.IP_KOJI, sizeof pSI->sc_ckji1 );
		trim_zero( pSI->sc_ckji1, sizeof pSI->sc_ckji1, strlen( pSI->sc_ckji1 ));
	}
	else
		::ZeroMemory( pSI->sc_ckji1, sizeof pSI->sc_ckji1 );
	if( ps->LINE_CND[ SCN_CKJI2_PN ].INP_sg ) {
		memcpy( pSI->sc_ckji2, ps->SCN_CKJI2_DT.IP_KOJI, sizeof pSI->sc_ckji2 );
		trim_zero( pSI->sc_ckji2, sizeof pSI->sc_ckji2, strlen( pSI->sc_ckji2 ));
	}
	else
		::ZeroMemory( pSI->sc_ckji2, sizeof pSI->sc_ckji2 );
	pSI->sc_ckjiopt = 0;
	if( ps->LINE_CND[ SCN_CKJI1_PN ].KEY_inp & SC_ITEMNONE ) {
		pSI->sc_ckjiopt |= SC_ITEMNONE;
	}
	if( ps->LINE_CND[ SCN_CKJI1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_ckjiopt |= SC_NOTSEARCH;
	}

	// 貸借工事
	if( ps->LINE_CND[ SCN_DCKJI1_PN ].INP_sg ) {
		memcpy( pSI->sc_dckji1, ps->SCN_DCKJI1_DT.IP_KOJI, sizeof pSI->sc_dckji1 );
		trim_zero( pSI->sc_dckji1, sizeof pSI->sc_dckji1, strlen( pSI->sc_dckji1 ));
	}
	else
		::ZeroMemory( pSI->sc_dckji1, sizeof pSI->sc_dckji1 );
	if( ps->LINE_CND[ SCN_DCKJI2_PN ].INP_sg ) {
		memcpy( pSI->sc_dckji2, ps->SCN_DCKJI2_DT.IP_KOJI, sizeof pSI->sc_dckji2 );
		trim_zero( pSI->sc_dckji2, sizeof pSI->sc_dckji2, strlen( pSI->sc_dckji2 ));
	}
	else
		::ZeroMemory( pSI->sc_dckji2, sizeof pSI->sc_dckji2 );
	pSI->sc_dckjiopt = 0;
	if( ps->LINE_CND[ SCN_DCKJI1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_dckjiopt |= SC_NOTSEARCH;
	}

	// 借方
	if( ps->LINE_CND[ SCN_DEBT_PN ].INP_sg )
		memcpy( pSI->sc_dbt, ps->SCN_DEBT_DT.IP_CdNum, sizeof pSI->sc_dbt );
	else
		::ZeroMemory( pSI->sc_dbt, sizeof pSI->sc_dbt );
	// 借方（終了）
	if( ps->LINE_CND[ SCN_DEBT2_PN ].INP_sg )
		memcpy( pSI->sc_dbt2, ps->SCN_DEBT2_DT.IP_CdNum, sizeof pSI->sc_dbt2 );
	else
		::ZeroMemory( pSI->sc_dbt2, sizeof pSI->sc_dbt2 );

	pSI->sc_dbtopt = 0;
	if( ps->LINE_CND[ SCN_DEBT_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_dbtopt |= SC_NOTSEARCH;
	}
	if( m_KmkType )	pSI->sc_dbtopt |= SC_KMKHANI;	// 科目範囲

	// 借方枝番
	if( ps->LINE_CND[ SCN_DBR1_PN ].INP_sg )
		pSI->sc_dbr1 = ps->SCN_DBR1_DT.IP_cBRN;
	else
		pSI->sc_dbr1 = -1;

	if( ps->LINE_CND[ SCN_DBR2_PN ].INP_sg )
		pSI->sc_dbr2 = ps->SCN_DBR2_DT.IP_cBRN;
	else
		pSI->sc_dbr2 = -1;
	pSI->sc_dbropt = 0;
	if( ps->LINE_CND[ SCN_DBR1_PN ].KEY_inp & SC_ITEMNONE ) {
		pSI->sc_dbropt |= SC_ITEMNONE;
	}
	if( ps->LINE_CND[ SCN_DBR1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_dbropt |= SC_NOTSEARCH;
	}

	// 貸方
	if( ps->LINE_CND[ SCN_CRED_PN ].INP_sg )
		memcpy( pSI->sc_cre, ps->SCN_CRED_DT.IP_CdNum, sizeof pSI->sc_cre );
	else
		::ZeroMemory( pSI->sc_cre, sizeof pSI->sc_cre );
	// 貸方（終了）
	if( ps->LINE_CND[ SCN_CRED2_PN ].INP_sg )
		memcpy( pSI->sc_cre2, ps->SCN_CRED2_DT.IP_CdNum, sizeof pSI->sc_cre2 );
	else
		::ZeroMemory( pSI->sc_cre2, sizeof pSI->sc_cre2 );

	pSI->sc_creopt = 0;
	if( ps->LINE_CND[ SCN_CRED_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_creopt |= SC_NOTSEARCH;
	}
	if( m_KmkType )	pSI->sc_creopt |= SC_KMKHANI;	// 科目範囲

	// 貸方枝番
	if( ps->LINE_CND[ SCN_CBR1_PN ].INP_sg )
		pSI->sc_cbr1 = ps->SCN_CBR1_DT.IP_cBRN;
	else
		pSI->sc_cbr1 = -1;

	if( ps->LINE_CND[ SCN_CBR2_PN ].INP_sg )
		pSI->sc_cbr2 = ps->SCN_CBR2_DT.IP_cBRN;
	else
		pSI->sc_cbr2 = -1;
	pSI->sc_cbropt = 0;
	if( ps->LINE_CND[ SCN_CBR1_PN ].KEY_inp & SC_ITEMNONE ) {
		pSI->sc_cbropt |= SC_ITEMNONE;
	}
	if( ps->LINE_CND[ SCN_CBR1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_cbropt |= SC_NOTSEARCH;
	}

	// 借／貸
	if( ps->LINE_CND[ SCN_DBCR_PN ].INP_sg )
		memcpy( pSI->sc_dbcr, ps->SCN_DBCR_DT.IP_CdNum, sizeof pSI->sc_dbcr );
	else
		::ZeroMemory( pSI->sc_dbcr, sizeof pSI->sc_dbcr );
	// 借／貸(終了)
	if( ps->LINE_CND[ SCN_DBCR2_PN ].INP_sg )
		memcpy( pSI->sc_dbcr2, ps->SCN_DBCR2_DT.IP_CdNum, sizeof pSI->sc_dbcr2 );
	else
		::ZeroMemory( pSI->sc_dbcr2, sizeof pSI->sc_dbcr2 );

	pSI->sc_dbcropt = 0;
	if( ps->LINE_CND[ SCN_DBCR_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_dbcropt |= SC_NOTSEARCH;
	}		
	if( m_KmkType )	pSI->sc_dbcropt |= SC_KMKHANI;	// 科目範囲

	// 借／貸枝番
	if( ps->LINE_CND[ SCN_DCBR1_PN ].INP_sg )
		pSI->sc_dcbrn1 = ps->SCN_DCBR1_DT.IP_cBRN;
	else
		pSI->sc_dcbrn1 = -1;

	if( ps->LINE_CND[ SCN_DCBR2_PN ].INP_sg )
		pSI->sc_dcbrn2 = ps->SCN_DCBR2_DT.IP_cBRN;
	else
		pSI->sc_dcbrn2 = -1;

	if( (pSI->sc_dbropt & SC_ITEMNONE) || (pSI->sc_cbropt & SC_ITEMNONE) ) {
		pSI->sc_dcbropt |= SC_ITEMNONE;
	}
	pSI->sc_dcbropt = 0;
	if( ps->LINE_CND[ SCN_DCBR1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_dcbropt |= SC_NOTSEARCH;
	}		

	// 金額
	if( ps->LINE_CND[ SCN_VAL1_PN ].INP_sg ) {
		memcpy( pSI->sc_val1, ps->SCN_VAL1_DT.IP_VAL, sizeof pSI->sc_val1 );
		pSI->sc_valsgn[0] = 1;
	}
	else {
		::ZeroMemory( pSI->sc_val1, sizeof pSI->sc_val1 );
		pSI->sc_valsgn[0] = 0;
	}

	if( ps->LINE_CND[ SCN_VAL2_PN ].INP_sg ) {
		memcpy( pSI->sc_val2, ps->SCN_VAL2_DT.IP_VAL, sizeof pSI->sc_val2 );
		pSI->sc_valsgn[1] = 1;
	}
	else {
		::ZeroMemory( pSI->sc_val2, sizeof pSI->sc_val2 );
		pSI->sc_valsgn[1] = 0;
	}
	pSI->sc_valopt = 0;
	if( ps->LINE_CND[ SCN_VAL1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_valopt |= SC_NOTSEARCH;
	}				
	// 税額
	if( ps->LINE_CND[ SCN_ZEI1_PN ].INP_sg ) {
		memcpy( pSI->sc_zei1, ps->SCN_ZEI1_DT.IP_VAL, sizeof pSI->sc_zei1 );
		pSI->sc_zeisgn[0] = 1;
	}
	else {
		::ZeroMemory( pSI->sc_zei1, sizeof pSI->sc_zei1 );
		pSI->sc_zeisgn[0] = 0;
	}

	if( ps->LINE_CND[ SCN_ZEI2_PN ].INP_sg ) {
		memcpy( pSI->sc_zei2, ps->SCN_ZEI2_DT.IP_VAL, sizeof pSI->sc_zei2 );
		pSI->sc_zeisgn[1] = 1;
	}
	else {
		::ZeroMemory( pSI->sc_zei2, sizeof pSI->sc_zei2 );
		pSI->sc_zeisgn[1] = 0;
	}
	pSI->sc_zeiopt = 0;
	if( ps->LINE_CND[ SCN_ZEI1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_zeiopt |= SC_NOTSEARCH;
	}				

	// 対価
	if( ps->LINE_CND[ SCN_TAI1_PN ].INP_sg ) {
		memcpy( pSI->sc_tai1, ps->SCN_TAI1_DT.IP_VAL, sizeof pSI->sc_tai1 );
		pSI->sc_taisgn[0] = 1;
	}
	else {
		::ZeroMemory( pSI->sc_tai1, sizeof pSI->sc_tai1 );
		pSI->sc_taisgn[0] = 0;
	}

	if( ps->LINE_CND[ SCN_TAI2_PN ].INP_sg ) {
		memcpy( pSI->sc_tai2, ps->SCN_TAI2_DT.IP_VAL, sizeof pSI->sc_tai2 );
		pSI->sc_taisgn[1] = 1;
	}
	else {
		::ZeroMemory( pSI->sc_tai2, sizeof pSI->sc_tai2 );
		pSI->sc_taisgn[1] = 0;
	}
	pSI->sc_taiopt = 0;
	if( ps->LINE_CND[ SCN_TAI1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_taiopt |= SC_NOTSEARCH;
	}

	// 消費税仕訳区分サイン
	if( ps->LINE_CND[ SCN_SYZSW_PN ].INP_sg )
		pSI->sc_syzsw = ps->SCN_SYZSW_DT.SKBN_SGN1;
	else
		pSI->sc_syzsw = NON_SELECT;
	pSI->sc_syzswopt = 0;
	if( ps->LINE_CND[ SCN_SYZSW_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_syzswopt |= SC_NOTSEARCH;
	}				

	// 消費税税率サイン
	if( ps->LINE_CND[ SCN_ZRITU_PN ].INP_sg )
		pSI->sc_zritu = ps->SCN_ZRITU_DT.SKBN_SGN1;
	else
		pSI->sc_zritu = NON_SELECT;
	pSI->sc_zrituopt = 0;
	if( ps->LINE_CND[ SCN_ZRITU_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_zrituopt |= SC_NOTSEARCH;
	}				

	// 消費税区分サイン
	if( ps->LINE_CND[ SCN_SKBN_PN ].INP_sg )
		pSI->sc_skbn = ps->SCN_SKBN_DT.SKBN_SGN1;
	else
		pSI->sc_skbn = NON_SELECT;
	pSI->sc_skbnopt = 0;
	if( ps->LINE_CND[ SCN_SKBN_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_skbnopt |= SC_NOTSEARCH;
	}
	// 消費税属性検索
	if( ps->LINE_CND[ SCN_SKBN_PN ].KEY_inp & SC_KMKHANI ) {
		pSI->sc_skbnopt |= SC_KMKHANI;
	}

	// 売上／仕入サイン
	if( ps->LINE_CND[ SCN_URISIRE_PN ].INP_sg )
		pSI->sc_urisire = ps->SCN_URISIRE_DT.SKBN_SGN1;
	else
		pSI->sc_urisire = NON_SELECT;
	pSI->sc_urisireopt = 0;
	if( ps->LINE_CND[ SCN_URISIRE_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_urisireopt |= SC_NOTSEARCH;
	}

	// 特定収入[06.03 /08]
	if( ps->LINE_CND[ SCN_TOKUS_PN ].INP_sg )
		pSI->sc_tokus = ps->SCN_TOKUS_DT.SKBN_SGN1;
	else
		pSI->sc_tokus = NON_SELECT;
	pSI->sc_tokusopt = 0;
	if( ps->LINE_CND[ SCN_TOKUS_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_tokusopt |= SC_NOTSEARCH;
	}

	// 摘要
	if( ps->LINE_CND[ SCN_TKY_PN ].INP_sg )
		memcpy( pSI->sc_tky, ps->SCN_TKY_DT.IP_TKY, sizeof pSI->sc_tky );
	else
		::ZeroMemory( pSI->sc_tky, sizeof pSI->sc_tky );

	pSI->sc_tkytyp = ps->LINE_CND[ SCN_TKY_PN ].INP_type;
	// 摘要 NOT
	pSI->sc_tkyopt = 0;
	if( ps->LINE_CND[ SCN_TKY_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_tkyopt |= SC_NOTSEARCH;
	}

	// 手形番号
	if( ps->LINE_CND[ SCN_SNUM1_PN ].INP_sg )
		memcpy( pSI->sc_rnum1, ps->SCN_SNUM1_DT.IP_SNUM, sizeof pSI->sc_rnum1 );
	else
		::ZeroMemory( pSI->sc_rnum1, sizeof pSI->sc_rnum1 );

	if( ps->LINE_CND[ SCN_SNUM2_PN ].INP_sg )
		memcpy( pSI->sc_rnum2, ps->SCN_SNUM2_DT.IP_SNUM, sizeof pSI->sc_rnum2 );
	else
		::ZeroMemory( pSI->sc_rnum2, sizeof pSI->sc_rnum2 );
	pSI->sc_snumopt = 0;
	if( ps->LINE_CND[ SCN_SNUM1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_snumopt |= SC_NOTSEARCH;
	}

	// 手形期日
	if( ps->LINE_CND[ SCN_TDAT1_PN ].INP_sg )
		pSI->sc_tgdate1 = ps->SCN_TDAT1_DT.IP_TGDAY;
	else
		pSI->sc_tgdate1 = 0;

	if( ps->LINE_CND[ SCN_TDAT2_PN ].INP_sg )
		pSI->sc_tgdate2 = ps->SCN_TDAT2_DT.IP_TGDAY;
	else
		pSI->sc_tgdate2 = 0;
	pSI->sc_tgdtopt = 0;
	if( ps->LINE_CND[ SCN_TDAT1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_tgdtopt |= SC_NOTSEARCH;
	}

	// 付箋
	if( ps->LINE_CND[ SCN_HUSEN_PN ].INP_sg ) {
		pSI->sc_husen = ps->SCN_HUSEN_DT.SKBN_SGN1;
	}
	else {	
		pSI->sc_husen = NON_SELECT;
	}
	pSI->sc_husenopt = 0;
	if( ps->LINE_CND[ SCN_HUSEN_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_husenopt |= SC_NOTSEARCH;
	}

	//付箋コメント
//	::ZeroMemory( pSI->sc_hucmnt, sizeof pSI->sc_hucmnt );
	::ZeroMemory( pSI->sc_hucmntEX, sizeof pSI->sc_hucmntEX );

	if( ps->LINE_CND[ SCN_HUCMT_PN ].INP_sg ) {
//		memcpy( pSI->sc_hucmnt, ps->SCN_HUCMT_DT.IP_HUCMT, 10 );
		memcpy( pSI->sc_hucmntEX, ps->SCN_HUCMT_DT.IP_HUCMT, 40 );
	}

	// 入力漏れ
	if( ps->LINE_CND[ SCN_NOINP_PN ].INP_sg ) {
			pSI->sc_noinp = ps->SCN_NOINP_DT.SKBN_SGN1;
	}
	else	pSI->sc_noinp = 0;
	// 業種の区分
	if( ps->LINE_CND[ SCN_GYOKBN_PN ].INP_sg ) {
			pSI->sc_gyokbn = ps->SCN_GYOKBN_DT.SKBN_SGN1;
	}
	else	pSI->sc_gyokbn = 0;

	// 変動事由
	if( ps->LINE_CND[ SCN_HJIYU_PN ].INP_sg ) {
			pSI->sc_hjiyu = ps->SCN_HJIYU_DT.SKBN_SGN2;
	}
	else	pSI->sc_hjiyu = 0;

	// 証憑番号
	if( ps->LINE_CND[ SCN_DOCEVI1_PN ].INP_sg )
		memcpy( pSI->sc_docev1, ps->SCN_DOCEVI1_DT.IP_SNUM, sizeof pSI->sc_docev1 );
	else
		::ZeroMemory( pSI->sc_docev1, sizeof pSI->sc_docev1 );

	if( ps->LINE_CND[ SCN_DOCEVI2_PN ].INP_sg )
		memcpy( pSI->sc_docev2, ps->SCN_DOCEVI2_DT.IP_SNUM, sizeof pSI->sc_docev2 );
	else
		::ZeroMemory( pSI->sc_docev2, sizeof pSI->sc_docev2 );
	pSI->sc_docevopt = 0;
	if( ps->LINE_CND[ SCN_DOCEVI1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_docevopt |= SC_NOTSEARCH;
	}

	// 文書番号
	if( ps->LINE_CND[ SCN_DOCSEQ1_PN ].INP_sg )
		pSI->sc_docseq1 = ps->SCN_DOCSEQ1_DT.IP_SEQ;
	else
		pSI->sc_docseq1 = -1;
	if( ps->LINE_CND[ SCN_DOCSEQ2_PN ].INP_sg )
		pSI->sc_docseq2 = ps->SCN_DOCSEQ2_DT.IP_SEQ;
	else
		pSI->sc_docseq2 = -1;

	pSI->sc_docseqopt = 0;
	if( ps->LINE_CND[ SCN_DOCSEQ1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_docseqopt |= SC_NOTSEARCH;
	}
	//書類種別
	if( ps->LINE_CND[ SCN_DOCSHTP_PN ].INP_sg ) {
			pSI->sc_docshtyp = ps->SCN_DOCSHTP_DT.SKBN_SGN1;
	}
	else	pSI->sc_docshtyp = 0;

	//資産
	if( ps->LINE_CND[ SCN_SYZKTSN_PN ].INP_sg ) {
			pSI->sc_syzktsn = ps->SCN_SYZKTSN_DT.SKBN_SGN1;
	}
	else	pSI->sc_syzktsn = 0;

	// インボイス対応
	// 登録番号
	if( ps->LINE_CND[SCN_INVNO_PN].INP_sg ) {
		pSI->sc_invsgn = 1;
		memmove( pSI->sc_invno, &ps->SCN_INVNO_DT._invno, sizeof( pSI->sc_invno ));
	}
	else {
		pSI->sc_invsgn = 0;
	}
	pSI->sc_invnoopt = 0;
	if( ps->LINE_CND[SCN_INVNO_PN].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_invnoopt |= SC_NOTSEARCH;
	}

	// 免税事業者からの課税仕入
	if( ps->LINE_CND[SCN_MENZEI_PN].INP_sg ) {
		pSI->sc_menzei = 1;
	}
	else {
		pSI->sc_menzei = 0;
	}
	//NOT
	pSI->sc_menzeiopt = 0;
	if( ps->LINE_CND[SCN_MENZEI_PN].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_menzeiopt |= SC_NOTSEARCH;
	}

	// 免税事業者からの課税仕入 の控除割合
	if( ps->LINE_CND[SCN_MENZEI_PN].INP_sg ) {
		if( ps->LINE_CND[SCN_MENWARI_PN].INP_sg ) {
			pSI->sc_menwari = ps->SCN_MENWARI_DT.SKBN_SGN1;
		}
		else {
			pSI->sc_menwari = 0;
		}
	}
	else {
		pSI->sc_menwari = 0;
	}

	if( scan_sgn == 0 ) {
		pDBzm->IsDelScan() = FALSE;
	}
	else {
		pDBzm->IsDelScan() = TRUE;
		pDBzm->IsDupliScan() = FALSE;
	}

	return 0;
}


void CScanKeyView::DateSelectToSkeyLine()
{
	int sofs, eofs, sy, ey;
	int sksgn, eksgn;
	char buf[64];

	struct _SKEY_LINE* ps;
	SCAN_ITEMEX* pSI;
	ps = &SCAN_KEY;
	pSI = &SC_Item;

	m_selDate.GetMonthOffset(sofs, eofs, sy, ey);

	pDBzm->minfo->MoveFirst();
	do {
		if( pDBzm->minfo->ofset == sofs ) {
			sksgn = pDBzm->minfo->kes_sgn;
		}
		if( pDBzm->minfo->ofset == eofs ) {
			eksgn = pDBzm->minfo->kes_sgn;
		}
	} while( pDBzm->minfo->MoveNext() == 0 );

	sy %= 10000;
	ey %= 10000;

	ps->LINE_CND[ SCN_DATE1_PN ].INP_sg = 1;
	sprintf_s( buf, sizeof buf, "%04d", sy );
	l_pack( ps->SCN_DATE1_DT.IP_BCDDAY, buf, 4 );
	KESY_SG_SET( ps->SCN_DATE1_DT.IP_BCDDAY[0], sksgn );
	memcpy( pSI->sc_date1, ps->SCN_DATE1_DT.IP_BCDDAY, 2 );
	pSI->sc_sofs = sofs;

	ps->LINE_CND[ SCN_DATE2_PN ].INP_sg = 1;
	sprintf_s( buf, sizeof buf, "%04d", ey );
	l_pack( ps->SCN_DATE2_DT.IP_BCDDAY, buf, 4 );
	KESY_SG_SET( ps->SCN_DATE2_DT.IP_BCDDAY[0], eksgn );
	memcpy( pSI->sc_date2, ps->SCN_DATE2_DT.IP_BCDDAY, 2 );
	pSI->sc_eofs = eofs;

	pSI->sc_dateopt = 0;
	if( ps->LINE_CND[ SCN_DATE1_PN ].KEY_inp & SC_NOTSEARCH ) {
		pSI->sc_dateopt |= SC_NOTSEARCH;
	}
}


// End 押下し
void CScanKeyView::OnButtonEnd()
{
	ScanKeyClear();
	CalcMouseEvent();

	if( bSCAN_ONLY_MODE )
		((CMainFrame*)GetDBDinpMainFrame())->PostMessage( WM_CLOSE );
	else {
		// 検索項目を開く直前のビューに変更
		((CMainFrame*)GetDBDinpMainFrame())->SkeyOnButtonEnd();
	}
}


BOOL CScanKeyView::EndApp( WPARAM wParam, BOOL bCosel )
{
	if( (wParam == 1) ) {
		//ICSExit()でエラー終了した場合はこちらに記述します。
		return TRUE;
	}
	else {
		if( ! bCosel ) {
			int st = IDYES;
			if( ! ((CMainFrame*)GetDBDinpMainFrame())->CloudEndChainJob() ) {
				st	=	ICSJobEndBox( _T("仕訳入力を終了してもよろしいですか？") );
			}
			if( st == IDYES ) 
				return TRUE;
			else
				return FALSE;
		}
		//		((CMainFrame*)GetDBDinpMainFrame())->JobEndOperation(bCosel);
		return TRUE;	
	}
}

// F11 全項目削除
void CScanKeyView::OnButtonF11()
{
	ScanKeyAllDel();
/*
	// ICSDateSelectにデータセット
	// test code
	set_dateselect();
*/
}


int CScanKeyView::ReloadView()
{
//FILETrace( "@CScanKeyView::ReloadView\n" );
	if( pDBzm == NULL ) {
		return -1;
	}

	try {
		// マスター再読み込み
		mst_reload();
	}
	catch( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;	
	}

	struct _InpCtl *ctl;
	struct _SKEY_LINE *dtline;
	int i;

	ctl = INP_CTL;
	dtline = &SCAN_KEY;

	//科目名称再表示
	i = SCN_DEBT_PN;
	if( dtline->LINE_CND[i].INP_sg )
	{
		knam_set( &dtline->SCN_DEBT_DT );
		DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_DEBT_DT.IP_stKMK, pDBzm );
	}

	i = SCN_DEBT2_PN;
	if( dtline->LINE_CND[i].INP_sg )
	{
		knam_set( &dtline->SCN_DEBT2_DT );
		DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_DEBT2_DT.IP_stKMK, pDBzm );
	}

	i = SCN_CRED_PN;
	if( dtline->LINE_CND[i].INP_sg )
	{
		knam_set( &dtline->SCN_CRED_DT );
		DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_CRED_DT.IP_stKMK, pDBzm );
	}

	i = SCN_CRED2_PN;
	if( dtline->LINE_CND[i].INP_sg )
	{
		knam_set( &dtline->SCN_CRED2_DT );
		DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_CRED2_DT.IP_stKMK, pDBzm );
	}

	i = SCN_DBCR_PN;
	if( dtline->LINE_CND[i].INP_sg )
	{
		knam_set( &dtline->SCN_DBCR_DT );
		DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_DBCR_DT.IP_stKMK, pDBzm );
	}

	i = SCN_DBCR2_PN;
	if( dtline->LINE_CND[i].INP_sg )
	{
		knam_set( &dtline->SCN_DBCR2_DT );
		DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->SCN_DBCR2_DT.IP_stKMK, pDBzm );
	}

	m_Seldata.SelectUpdate();

	// 諸口貸借 を更新
	g_stotal( SYOGT, (char *)SYOG_d, (char *)SYOG_c, (char *)SG_def );
	// 現金貸借 を更新
	g_stotal( /*GNKIN*/pAUTOSEL->DCKM_CODE, (char *)GENK_d, (char *)GENK_c, (char *)GK_def );

	return 0;
}


void CScanKeyView::OnUpdateButtonF4(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( FALSE );
}

HBRUSH CScanKeyView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDBbaseView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
/*
	if (pWnd->GetDlgCtrlID() == IDC_NCHK_SEQ)
	{
		pDC->SetTextColor( RGB_WHITE );

		// Set the background mode for text to transparent 
		// so background will show thru.
		pDC->SetBkMode(TRANSPARENT);

		// Return handle to our CBrush object
		hbr = m_ChkColor;
	}
*/
	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


//
//	複数摘要で検索
//
void CScanKeyView::OnButtonF6()
{
	INT_PTR cnt;
	VARIANT var;
	CString str;

	if( get_nowpn() == SCN_TKY_PN ) {
		cnt = SC_Tky.GetCount();

		if( cnt < 5 ) {
			DBdata_get( this, scGetINP_CTL( SCN_TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			if( var.pbVal[0] != '\0' ) {
				ICSReportRecordItem*  pItem;
				ICSReportRecord* pRecord;

				pRecord = new ICSReportRecord();
			//	m_Report.InsertRecordAt( idx, pRecord );
				m_Report.AddRecord( pRecord );

				str.Format(_T("%s"), var.pbVal );
				SC_Tky.Add( str );

				ICS_REPORT_EDITPARA rp;
				rp.kind = ICS_EDS_NUMERIC|ICS_EDS_ALPHABET|ICS_EDS_KANA|ICS_EDS_ZENKAKU|ICS_EDS_ASCII;
				rp.inverse = FALSE;			//フォーカス時インバース
				rp.cmove = TRUE;			//カーソル移動許可/不許可
				rp.lenMax = 80;				//入力最大バイト数
				rp.vector = ICS_EDS_LEFT;	//入力方向
				rp.conma = FALSE;			//入力時カンマ編集
				rp.format = "";	//書式フォーマット(倍長のみ有効)
				rp.hitem = 0;				//コンボボックスリストアイテムの高さ

				pItem  =  pRecord->AddItem( new ICSReportRecordItemText( str, &rp ) );
				((ICSReportRecordItemText*)pItem)->EnableDelete(0);
				
				pItem->SetBackgroundColor( RGB_PAPER_WHITE );
				m_Report.AllowEditColumn(0,TRUE);

				m_Report.Populate();
				m_Report.RedrawControl();

				// レポートを表示
				if( cnt == 0 ) {
					// 複数検索あり
					TkyReportDisp();
				}

				// 摘要欄クリア
				var.pbVal = NULL;
				DBdata_set( this, scGetINP_CTL( SCN_TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
		}
	}
	else {
		// toggle
		if( m_KmkType == 0 ) {
			m_KmkType = 1;
		}
		else {
			m_KmkType = 0;

			// カーソルの表示キープ
			int nowpn = get_nowpn();
			if( nowpn == SCN_DEBT2_PN || nowpn == SCN_CRED2_PN || nowpn == SCN_DBCR2_PN ) {
				set_focus( nowpn - 1 );
			}
		}

		struct _InpCtl *ctl;
		ctl = INP_CTL;

		for( int i = SCN_DEBT_PN ; i <= SCN_DBCR2_PN ; ++i )
		{
			switch( i ) {
			case SCN_DEBT2_PN:
			case SCN_CRED2_PN:
			case SCN_DBCR2_PN:
				if( m_KmkType == 0 ) {
					(ctl+i)->INP_ena = FALSE;
					// 最終のデータはクリアする。
					memset( &SCAN_KEY._XXDTA2[i], '\0', sizeof SCAN_KEY._XXDTA2[i] );
					memset( (char *)&SCAN_KEY.LINE_CND[i], (char)0, sizeof(SCAN_KEY.LINE_CND[i]) );
					DBkamokudata_set( this, (ctl+i)->IDC_X, &SCAN_KEY._XXDTA2[i].IP_stKMK, pDBzm );
				}
				else {
					(ctl+i)->INP_ena = TRUE;
				}

				((CICSDBEDT *)(ctl+i)->p_obj)->EnableWindow( (ctl+i)->INP_ena );
				GetDlgItem(IDC_STATIC15)->EnableWindow( (ctl+i)->INP_ena );
				GetDlgItem(IDC_STATIC17)->EnableWindow( (ctl+i)->INP_ena );
				GetDlgItem(IDC_STATIC18)->EnableWindow( (ctl+i)->INP_ena );
				break;
			}
		}
	}
}


// F6 状態
void CScanKeyView::OnUpdateButtonF6(CCmdUI *pCmdUI)
{
	int pn;
	pn = get_nowpn();

	switch( pn ) {
	case SCN_DEBT_PN:	case SCN_DEBT2_PN:
	case SCN_CRED_PN:	case SCN_CRED2_PN:
	case SCN_DBCR_PN:	case SCN_DBCR2_PN:
	case SCN_TKY_PN:
		pCmdUI->Enable( TRUE );
		return;
		break;
	}

	pCmdUI->Enable( FALSE );
}


//
//	Edit 状態でないときの 摘要リポートキー処理
//
long CScanKeyView::OnReportKeyJob( long nChar )
{
	int count, index;
	ICSReportRows*	pRows;
	index = -1;

	switch( nChar ) {
	case VK_RETURN:
		if( m_idxTkyReport != -1 ) {
			m_Report.EditItem( m_idxTkyReport, 0, TRUE );
			nChar = 0;
		}
		break;
	case VK_DELETE:
		if( m_idxTkyReport != -1 ) {
			index = m_idxTkyReport;
		}
		else {
			pRows = m_Report.GetRows();
			count = pRows->GetCount();
			if( count > 0 )	index = 0;
		}

		if( index != -1 ) {

			m_Report.RemoveRecordAt( index );
			m_Report.RedrawControl();
			SC_Tky.RemoveAt( index );
			ICSReportRows*	pRows	=	m_Report.GetRows();

			count		=	pRows->GetCount();
			if( count == 0 ) {
				m_Report.ShowWindow( SW_HIDE );
				set_focus( SCN_TKY_PN );
				m_Tky.SetBackColor( RGB_WHITE );
			}
			else {
				if( index >= count ) {
					m_Report.EditItem( count-1, 0, TRUE );
				}
				else {
					m_Report.EditItem( index, 0, TRUE );
				}
			}
			nChar = 0;
		}
		break;

	case VK_TAB:
		if( is_SHIFT() ) {
			term_focus( SCN_TKY_PN, VK_LEFT );
		}
		else {
			term_focus( SCN_TKY_PN, VK_TAB );
		}
		nChar = 0;
		break;
	case VK_F2:
		term_focus( SCN_TKY_PN, VK_LEFT );
		nChar = 0;
		break;
	}

	return nChar;
}


void CScanKeyView::OnNMReportSelchangedTKY_REPORT(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更後の行
	ICSReportRow*	pRow	=	pItemNotify->pRow;
	int	indexRow	=	pRow->GetIndex();
	//クリックされた位置
	POINT pt	=	pItemNotify->pt;

	m_idxTkyReport = indexRow;
}

void CScanKeyView::OnNMReportEditkeydownTKY_REPORT(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更のあったカラム
	ICSReportColumn*	pColumn	=	pItemNotify->pColumn;
	int	indexColumn	=	pColumn->GetIndex();
	//変更のあった行
	ICSReportRow*	pRow	=	pItemNotify->pRow;
	int	indexRow	=	pRow->GetIndex();
	//変更のあったアイテム
	ICSReportRecordItem*	pItem	=	pItemNotify->pItem;
	int	indexItem	=	pItem->GetIndex();
	//ターミネーションキーコード
	UINT	nChar	=	pItemNotify->nChar;

	ICSReportRows*	pRows	=	m_Report.GetRows();
	int count		=	pRows->GetCount();

	CString	str;
	// 複数摘要 キー処理
	switch( nChar ) {
	case VK_RETURN:
	case VK_DOWN:
		if( (indexRow+1) < count ) {
			indexRow++;
			m_Report.EditItem( indexRow, 0, TRUE );
		}
		else {
			str = pItem->GetCaption(NULL);
			SC_Tky.SetAt( indexRow, str );
			set_focus( SCN_TKY_PN );
		}
		break;

	case VK_UP:
		if( indexRow > 0 ) {
			indexRow--;
			m_Report.EditItem( indexRow, 0, TRUE );
		}
		else {
			str = pItem->GetCaption(NULL);
			SC_Tky.SetAt( indexRow, str );
		}
		break;
	// 摘要を削除する
	case VK_DELETE:
		m_Report.RemoveRecordAt( indexRow );
		m_Report.RedrawControl();
		SC_Tky.RemoveAt( indexRow );

		count		=	pRows->GetCount();
		if( count == 0 ) {
			m_Report.ShowWindow( SW_HIDE );
			set_focus( SCN_TKY_PN );
			m_Tky.SetBackColor( RGB_WHITE );
		}
		else {
			if( indexRow >= count ) {
				m_Report.EditItem( count-1, 0, TRUE );
			}
			else {
				m_Report.EditItem( indexRow, 0, TRUE );
			}
		}
		break;

	case VK_TAB:
		set_focus( SCN_TKY_PN );
		break;
	case VK_LEFT:
		set_focus( SCN_SELTKY_PN );
		break;
	case VK_RIGHT:
		set_focus( SNOT_SNUM_PN );
		break;
	}
}

void CScanKeyView::OnNMReportEditsetfocusTKY_REPORT(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更のあったカラム
	ICSReportColumn*	pColumn	=	pItemNotify->pColumn;
	int	indexColumn	=	pColumn->GetIndex();
	//変更のあった行
	ICSReportRow*	pRow	=	pItemNotify->pRow;
	int	indexRow	=	pRow->GetIndex();
	//変更のあったアイテム
	ICSReportRecordItem*	pItem	=	pItemNotify->pItem;
	int	indexItem	=	pItem->GetIndex();

	m_idxTkyReport = indexRow;
}

void CScanKeyView::OnNMReportEditkillfocusTKY_REPORT(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更のあったカラム
	ICSReportColumn*	pColumn	=	pItemNotify->pColumn;
	int	indexColumn	=	pColumn->GetIndex();
	//変更のあった行
	ICSReportRow*	pRow	=	pItemNotify->pRow;
	int	indexRow	=	pRow->GetIndex();
	//変更のあったアイテム
	ICSReportRecordItem*	pItem	=	pItemNotify->pItem;
	int	indexItem	=	pItem->GetIndex();

	m_idxTkyReport = -1;

	CString	str;
	str = pItem->GetCaption(NULL);

	SC_Tky.SetAt( indexRow, str );
}

//
//	損益のみ
//
void CScanKeyView::OnButtonF5()
{
	int i, pn, s_pn, e_pn;
	pn = get_nowpn();

	struct	_KamokuData check1 = {0}, check2 = {0};
	struct _ItemData *key1, *key2;

	DBKNREC* pKn;
	DWORD	dwCode, scod, ecod;

	// 損益開始終了
	scod = ecod = 0;

	for( i = 0; i < pDBzm->knm_info->reccount; i++ ) {
		pKn = (pDBzm->pKnrec + i);

		dwCode = pKn->kncod;
		hl_rev( &dwCode, 4 ); 

		if( dwCode >= SONEKICODE ) {
			if( scod == 0 ) {
				scod = dwCode;
				sprintf_s( check1.kd_code, sizeof check1.kd_code, "%08x", scod );
				check1.kd_eda = -1;
				sprintf_s( check1.kd_name, sizeof check1.kd_name, "%.14s", pKn->knnam );
			}
			break;
		}
	}

	i = pDBzm->knm_info->reccount - 1;
	pKn = (pDBzm->pKnrec + i);
	dwCode = pKn->kncod;
	hl_rev( &dwCode, 4 ); 

	ecod = dwCode;
	sprintf_s( check2.kd_code, sizeof check2.kd_code, "%08x", ecod );
	check1.kd_eda = -1;
	sprintf_s( check2.kd_name, sizeof check2.kd_name, "%.14s", pKn->knnam );

	switch( pn ) {
	case SCN_DEBT_PN:	case SCN_DEBT2_PN:
		s_pn	=	SCN_DEBT_PN;
		e_pn	=	SCN_DEBT2_PN;
		key1	=	&SCAN_KEY.SCN_DEBT_DT;
		key2	=	&SCAN_KEY.SCN_DEBT2_DT;
		break;
	case SCN_CRED_PN:	case SCN_CRED2_PN:
		s_pn	=	SCN_CRED_PN;
		e_pn	=	SCN_CRED2_PN;
		key1	=	&SCAN_KEY.SCN_CRED_DT;
		key2	=	&SCAN_KEY.SCN_CRED2_DT;
		break;
	case SCN_DBCR_PN:	case SCN_DBCR2_PN:
		s_pn	=	SCN_DBCR_PN;
		e_pn	=	SCN_DBCR2_PN;
		key1	=	&SCAN_KEY.SCN_DBCR_DT;
		key2	=	&SCAN_KEY.SCN_DBCR2_DT;
		break;
	}

	// toggle
	if( m_KmkType == 0 ) {
		m_KmkType = 1;
	}

	struct _InpCtl *ctl;
	ctl = INP_CTL;

	for( int i = SCN_DEBT_PN ; i <= SCN_DBCR2_PN ; ++i )
	{
		switch( i ) {
		case SCN_DEBT2_PN:
		case SCN_CRED2_PN:
		case SCN_DBCR2_PN:
			(ctl+i)->INP_ena = TRUE;

			((CICSDBEDT *)(ctl+i)->p_obj)->EnableWindow( (ctl+i)->INP_ena );
			GetDlgItem(IDC_STATIC15)->EnableWindow( (ctl+i)->INP_ena );
			GetDlgItem(IDC_STATIC17)->EnableWindow( (ctl+i)->INP_ena );
			GetDlgItem(IDC_STATIC18)->EnableWindow( (ctl+i)->INP_ena );
			break;
		}
	}

	DBkamokudata_set( this, (ctl+s_pn)->IDC_X, &check1, pDBzm );
	DBkamokudata_set( this, (ctl+e_pn)->IDC_X, &check2, pDBzm );
	ItemInp( s_pn, key1, 0,0,0 );
	ItemInp( e_pn, key2, 0,0,0 ); 
}


// F5 状態
void CScanKeyView::OnUpdateButtonF5(CCmdUI *pCmdUI)
{
	int pn;
	pn = get_nowpn();

	switch( pn ) {
	case SCN_DEBT_PN:	case SCN_DEBT2_PN:
	case SCN_CRED_PN:	case SCN_CRED2_PN:
	case SCN_DBCR_PN:	case SCN_DBCR2_PN:
		pCmdUI->Enable( TRUE );
		return;
		break;
	}

	pCmdUI->Enable( FALSE );
}



// 消費税チェック のﾁｪｯｸﾎﾞｯｸｽﾀｰﾐﾈｰｼｮﾝ
long CScanKeyView::OnTermCheckKMSYZ( long nChar )
{
	return term_focus( SCN_CHKKMSYZ_PN, nChar );
}

// 消費税チェック
void CScanKeyView::OnCheckKMSYZ()
{
	int chk = ((CButton*)GetDlgItem(IDC_CHECK_KMSYZ))->GetCheck();

	// 消費税区分のエリアを使う
	if( chk )	SCAN_KEY.LINE_CND[SCN_SKBN_PN].KEY_inp |= SC_KMKHANI;
	else		SCAN_KEY.LINE_CND[SCN_SKBN_PN].KEY_inp	&= ~SC_KMKHANI;
}


// 検索項目、科目検索タイプ保存
int CScanKeyView::GetSkeyKmkType()
{
	return m_KmkType;
}


LRESULT CScanKeyView::OnSelGuideMsg(WPARAM w, LPARAM l )
{
	BOOL bPos = FALSE;

	int pn = get_nowpn();

	if( pn == SCN_DEBT_PN || pn == SCN_CRED_PN || pn == SCN_DBCR_PN ||
		pn == SCN_DEBT2_PN || pn == SCN_CRED2_PN || pn == SCN_DBCR2_PN )
	{
		bPos = TRUE;
	}
	else if( BRNTKY_sel && (pn == SCN_DBR1_PN || pn == SCN_DBR2_PN || pn == SCN_CBR1_PN || pn == SCN_CBR2_PN || pn == SCN_DCBR1_PN || pn == SCN_DCBR2_PN) )
	{
		bPos = TRUE;
	}
	else if( pn == SCN_TKY_PN )
	{
		bPos = TRUE;
	}
	else if( pn == SCN_BMON1_PN || pn == SCN_BMON2_PN || 
			 pn == SCN_CBMN1_PN || pn == SCN_CBMN2_PN || 
			 pn == SCN_DCBMN1_PN || pn == SCN_DCBMN2_PN	 ) {
		// 部門選択処理
		bPos = TRUE;
	}
	else if( pn == SCN_KOJI1_PN || pn == SCN_KOJI2_PN || 
			 pn == SCN_CKJI1_PN || pn == SCN_CKJI2_PN ||
			 pn == SCN_DCKJI1_PN || pn == SCN_DCKJI2_PN	 ) { 
		// 工事選択処理
		bPos = TRUE;
	}

	if( bPos ) {
		if( w == VK_NEXT || w == VK_PRIOR ) {
			keybd_event( w, 0, 0, 0 );
			keybd_event( w, 0, KEYEVENTF_KEYUP, 0 );
		}
	}

	return 1;
}


///////////////////////////////////////////////////////////////////////////////
// 証憑番号
void CScanKeyView::OnSetFocusScnDocEvi1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

	set_nowpn( SCN_DOCEVI1_PN );
}

void CScanKeyView::OnKillFocusScnDocEvi1( long inplen ) 
{
	DocEviInp( SCN_DOCEVI1_PN, &SCAN_KEY.SCN_DOCEVI1_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnDocEvi1(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = DocEviInp( SCN_DOCEVI1_PN, &SCAN_KEY.SCN_DOCEVI1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_DOCEVI1_PN, nChar ) != 0 )
			TermBuzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}
}

void CScanKeyView::OnSetFocusScnDocEvi2() 
{
	if( m_DIALOG_OK == FALSE )
		return;

	set_nowpn( SCN_DOCEVI2_PN );
}

void CScanKeyView::OnKillFocusScnDocEvi2( long inplen ) 
{
	DocEviInp( SCN_DOCEVI2_PN, &SCAN_KEY.SCN_DOCEVI2_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnDocEvi2(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

//	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = SnumInp( SCN_DOCEVI2_PN, &SCAN_KEY.SCN_DOCEVI2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( SCN_DOCEVI2_PN, nChar ) != 0 )
			TermBuzzer();

	}
//	catch ( CErrBlk E )
	{
//		ErrExit( E.ErrCode, E.ErrMsg );
//		return;
	}	
}

// 証憑番号 入力＆オフフォーカス
long CScanKeyView::DocEviInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
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

// 証憑番号 コントロール設定
void CScanKeyView::enable_docevi( BOOL bEnable )
{
	scGetINP_CTL( SNOT_DOCEVI_PN )->INP_ena = bEnable;
	scGetINP_CTL( SCN_DOCEVI1_PN )->INP_ena = bEnable;
	scGetINP_CTL( SCN_DOCEVI2_PN )->INP_ena = bEnable;

	GetDlgItem(IDC_ST_SNO)->EnableWindow( bEnable );
	GetDlgItem(IDC_STATIC25)->EnableWindow( bEnable );
	
	m_DocEvi1.EnableWindow(bEnable);
	m_DocEvi2.EnableWindow(bEnable);
	m_NotDocEvi.EnableWindow( bEnable );

	int nShow;
	nShow = bEnable ? SW_SHOW : SW_HIDE;

	if( pDBzm != NULL ) {
		m_DocEvi1.SetMaxLen(Voln1->snocol);
		m_DocEvi2.SetMaxLen(Voln1->snocol);
	}

	GetDlgItem(IDC_ST_SNO)->ShowWindow( nShow );
	GetDlgItem(IDC_STATIC25)->ShowWindow( nShow );
	m_DocEvi1.ShowWindow( nShow );
	m_DocEvi2.ShowWindow( nShow );
	m_NotDocEvi.ShowWindow( nShow );

	//コントロール位置調整
	CRect rect, base;

	GetDlgItem(IDC_SCNSHTTYP)->GetWindowRect( &base );
	ScreenToClient(base);

	int h;
	if( bEnable ) {
		GetDlgItem(IDC_ST_DENP)->GetWindowRect( &rect );
		ScreenToClient(&rect);
		h = rect.Height();
		rect.top = base.bottom;
		rect.bottom = rect.top + h;
		GetDlgItem(IDC_ST_DENP)->MoveWindow( &rect );

		GetDlgItem(IDC_STATIC3)->GetWindowRect( &rect );
		ScreenToClient(&rect);
		h = rect.Height();
		rect.top	= base.bottom;
		rect.bottom = rect.top + h;
		GetDlgItem(IDC_STATIC3)->MoveWindow( &rect );

		m_Denp1.GetWindowRect( &rect );
		ScreenToClient(&rect);
		h = rect.Height();
		rect.top	= base.bottom;
		rect.bottom = rect.top + h;
		m_Denp1.MoveWindow(&rect);

		m_Denp2.GetWindowRect( &rect );
		ScreenToClient(&rect);
		h = rect.Height();
		rect.top	= base.bottom;
		rect.bottom = rect.top + h;
		m_Denp2.MoveWindow(&rect);

		// NOT ボタンを基準にする。
		m_NotDocEvi.GetWindowRect( &base );
		ScreenToClient(base);

		m_NotDenp.GetWindowRect( &rect );
		ScreenToClient(&rect);
		h = rect.Height();

		rect.bottom = base.top;
		rect.top	= rect.bottom - h;
		m_NotDenp.MoveWindow(&rect);
	}
	else {
		GetDlgItem(IDC_ST_DENP2)->GetWindowRect( &base );
		ScreenToClient(base);

		int btmpos = base.top - (base.Height()/2);

		GetDlgItem(IDC_ST_DENP)->GetWindowRect( &rect );
		ScreenToClient(&rect);
		h = rect.Height();
		rect.bottom = btmpos;
		rect.top	= rect.bottom - h; 
		GetDlgItem(IDC_ST_DENP)->MoveWindow( &rect );

		GetDlgItem(IDC_STATIC3)->GetWindowRect( &rect );
		ScreenToClient(&rect);
		h = rect.Height();
		rect.bottom = btmpos;
		rect.top	= rect.bottom - h; 
		GetDlgItem(IDC_STATIC3)->MoveWindow( &rect );

		m_Denp1.GetWindowRect( &rect );
		ScreenToClient(&rect);
		h = rect.Height();
		rect.bottom = btmpos;
		rect.top	= rect.bottom - h; 
		m_Denp1.MoveWindow(&rect);

		m_Denp2.GetWindowRect( &rect );
		ScreenToClient(&rect);
		h = rect.Height();
		rect.bottom = btmpos;
		rect.top	= rect.bottom - h; 
		m_Denp2.MoveWindow(&rect);

		m_NotDenp.GetWindowRect( &rect );
		ScreenToClient(&rect);
		h = rect.Height();
		rect.bottom = btmpos;
		rect.top	= rect.bottom - h; 
		m_NotDenp.MoveWindow(&rect);
	}
}


// 証憑番号 設定
void CScanKeyView::set_docevi()
{
	if( bDOCEVI_Master ) {
		enable_docevi( TRUE );
	}
	else {
		enable_docevi( FALSE );
	}
}


// 証憑番号 NOT
void CScanKeyView::OnBnClickedNchkSno()
{
	int chk = ((CButton*)GetDlgItem(IDC_NCHK_SNO))->GetCheck();

	// 消費税区分のエリアを使う
	if( chk )	SCAN_KEY.LINE_CND[SCN_DOCEVI1_PN].KEY_inp	|= SC_NOTSEARCH;
	else		SCAN_KEY.LINE_CND[SCN_DOCEVI1_PN].KEY_inp	&= ~SC_NOTSEARCH;
}


// 付箋コメント２０文字対応
void CScanKeyView::set_husencmt()
{
	CRect rect, rc2;
	// 付箋コンボボックス座標
	GetDlgItem( IDC_SCNHUSEN )->GetWindowRect( rect );
	ScreenToClient( rect );

	int sy = rect.bottom;
	int sx = rect.left;

	CWnd* pwnd = GetDlgItem( IDC_SCNHUCMT );

	CRect cmtRect;
	pwnd->GetWindowRect( cmtRect );
	ScreenToClient( cmtRect );

	GetDlgItem( IDC_SCNNOINPBRN )->GetWindowRect( rc2 );
	ScreenToClient( rc2 );

	CRect vwRect;
	GetClientRect( &vwRect );

	//付箋２０文字と摘要２０文字は同時期なので
	if( IsMasterType(MST_TKY20) ) {
		// 入力漏れ項目名の底座標 
		GetDlgItem( IDC_SCNNOINPBRN )->GetWindowRect( rc2 );
		ScreenToClient( rc2 );

//MyTrace( "付箋コンボBTM->sy = %d, rc2.bottom = %d, vwBtm = %d\n", sy, rc2.bottom, vwRect.bottom );

		// ビューの底座標より余裕があるか？
		if( rc2.bottom > sy ) {
			if( (rc2.bottom + cmtRect.Height()) < vwRect.bottom ) {
				sy = rc2.bottom;
			}
		}
		pwnd->MoveWindow( sx, sy, rc2.right - sx, cmtRect.Height() );

		((CICSDBEDT *)pwnd)->SetMaxLen( 40 );
	}
	else {
		sy = rect.top;
		sx = rect.right;

		GetDlgItem( IDC_ST_NOIPBRN )->GetWindowRect( rc2 );
		ScreenToClient( rc2 );

		pwnd->MoveWindow( sx, sy, rc2.left - sx, cmtRect.Height() );
		((CICSDBEDT *)pwnd)->SetMaxLen( 10 );
	}
}



//////////////////////////////////////////////////////////////////////
// 文書番号seq
void CScanKeyView::OnSetFocusScnDocSeq1() 
{
	if( m_DIALOG_OK == FALSE )
		return;

	set_nowpn( SCN_DOCSEQ1_PN );
}

void CScanKeyView::OnKillFocusScnDocSeq1( long inplen ) 
{
	if( m_DIALOG_OK == FALSE )
		return;

	SeqInp( SCN_DOCSEQ1_PN, &SCAN_KEY.SCN_DOCSEQ1_DT, 0, inplen, 0);
}

void CScanKeyView::OnTerminationScnDocSeq1(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

	// 入力＆オフフォーカス
	if( !(nChar = SeqInp( SCN_DOCSEQ1_PN, &SCAN_KEY.SCN_DOCSEQ1_DT, nChar, inplen, kst )) )
		return;

	//カーソル移動
	if( term_focus( SCN_DOCSEQ1_PN, nChar ) != 0 )
		TermBuzzer();
}

void CScanKeyView::OnSetFocusScnDocSeq2() 
{
	if( m_DIALOG_OK == FALSE )
		return;

	set_nowpn( SCN_DOCSEQ2_PN );
}

void CScanKeyView::OnKillFocusScnDocSeq2( long inplen ) 
{
	if( m_DIALOG_OK == FALSE )
		return;

	SeqInp( SCN_DOCSEQ2_PN, &SCAN_KEY.SCN_DOCSEQ2_DT, 0, inplen, 0);
}

void CScanKeyView::OnTerminationScnDocSeq2(long nChar, long inplen, long kst) 
{
	if( m_DIALOG_OK == FALSE )
		return;

	// 入力＆オフフォーカス
	if( !(nChar = SeqInp( SCN_DOCSEQ2_PN, &SCAN_KEY.SCN_DOCSEQ2_DT, nChar, inplen, kst )) )
		return;

	if( term_focus( SCN_DOCSEQ2_PN, nChar ) != 0 )
		TermBuzzer();
}


// 書類種別 ターミネーション
long CScanKeyView::OnTermComboDocSheetTyp( long nChar )
{

	return term_focus( SCN_DOCSHTP_PN, nChar );
}


void CScanKeyView::set_DocShtypCombo()
{
	m_DShtCombo.ResetContent();

	if( bSCANSAVE_Master ) {
		//データセット
		pDCsb->sheet_type->MoveFirst();

		int idx = 0;

#ifdef OLD_CLOSE
		m_DShtCombo.AddString("----------------");
		m_DShtCombo.SetItemData(idx, NON_SELECT);
		idx++;
		m_DShtCombo.AddString("-未設定-");
		m_DShtCombo.SetItemData(idx, (DWORD)DOCSHT_NONSUP);

		if( pDCsb->sheet_type->st != -1 ) {
			do {
				idx++;
				m_DShtCombo.AddString(pDCsb->sheet_type->shname);
				m_DShtCombo.SetItemData(idx, pDCsb->sheet_type->srtype);
			} while( pDCsb->sheet_type->MoveNext() == 0 );
		}
#endif
		CStringArray text;
		CDWordArray	data;

		text.Add("----------------");
		data.Add(NON_SELECT);
		idx;

		text.Add("-未設定-");
		data.Add(DOCSHT_NONSUP);
		idx;

		if( pDCsb->sheet_type->st != -1 ) {
			do {
				text.Add(pDCsb->sheet_type->shname);
				data.Add(pDCsb->sheet_type->srtype);
				idx++;
			} while( pDCsb->sheet_type->MoveNext() == 0 );
		}
		m_DShtCombo.MakeComboList(text, data);
	}
}

// 
void CScanKeyView::set_DocSeqAndShtyp()
{
	m_DShtCombo.ResetContent();

	if( bSCANSAVE_Master ) {
		m_DocSeq1.ShowWindow( SW_SHOW );
		m_DocSeq1.EnableWindow( TRUE );
		m_DocSeq2.ShowWindow( SW_SHOW );
		m_DocSeq2.EnableWindow( TRUE );
		m_NotDocSeq.ShowWindow( SW_SHOW );
		m_NotDocSeq.EnableWindow( TRUE );
		GetDlgItem(IDC_ST_SEQ2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ST_SEQ2)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC29)->ShowWindow(SW_SHOW);

		m_DShtCombo.ShowWindow( SW_SHOW );
		m_DShtCombo.EnableWindow( TRUE );
		GetDlgItem(IDC_ST_SHTTYP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ST_SHTTYP)->EnableWindow(TRUE);

		set_DocShtypCombo();
	}
	else {
		m_DocSeq1.ShowWindow( SW_HIDE );
		m_DocSeq1.EnableWindow( FALSE );
		m_DocSeq2.ShowWindow( SW_HIDE );
		m_DocSeq2.EnableWindow( FALSE );
		m_NotDocSeq.ShowWindow( SW_HIDE );
		m_NotDocSeq.EnableWindow( FALSE );
		GetDlgItem(IDC_ST_SEQ2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ST_SEQ2)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC29)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ST_SHTTYP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ST_SHTTYP)->EnableWindow(FALSE);
		m_DShtCombo.ShowWindow( SW_HIDE );
		m_DShtCombo.EnableWindow( TRUE );
	}
}


// 
void CScanKeyView::OnSelchangeScnShtTyp() 
{
	int sel = 	m_DShtCombo.GetCurSel();
	DWORD itd = m_DShtCombo.GetItemData( sel );

	SCAN_KEY.SCN_DOCSHTP_DT.SKBN_SGN1 = (short)itd;

	if( itd == NON_SELECT ) {
		SCAN_KEY.LINE_CND[ SCN_DOCSHTP_PN ].INP_sg = FALSE;
	}
	else {
		SCAN_KEY.LINE_CND[ SCN_DOCSHTP_PN ].INP_sg = TRUE;
	}
}


// 資産 ターミネーション
long CScanKeyView::OnTermComboSyzKtsisan( long nChar )
{
	return term_focus( SCN_SYZKTSN_PN, nChar );
}


void CScanKeyView::OnSelchangeScnSyzKtsn()
{
	int sel = 	m_SyzKtsn.GetCurSel();
	DWORD itd = m_SyzKtsn.GetItemData( sel );

	SCAN_KEY.SCN_SYZKTSN_DT.SKBN_SGN1 = (short)itd;

	if( itd == NON_SELECT ) {
		SCAN_KEY.LINE_CND[ SCN_SYZKTSN_PN ].INP_sg = FALSE;
	}
	else {
		SCAN_KEY.LINE_CND[ SCN_SYZKTSN_PN ].INP_sg = TRUE;
	}
}


void CScanKeyView::SelchangeDuserJob( int sel, int init/*=0*/ )
{
	BOOL bEnable;

	if( sel == 0 )	bEnable = FALSE;
	else			bEnable = TRUE;

	CButton* pbtn;
	pbtn = ((CButton*)GetDlgItem(IDC_CHK_INPDATA));
	if( init )	pbtn->SetCheck( BST_CHECKED );
	pbtn->EnableWindow(bEnable);
	scGetINP_CTL( SCN_CHKINPDT_PN )->INP_ena = bEnable;

	pbtn = ((CButton*)GetDlgItem(IDC_CHK_MDFDATA));
	if( init )	pbtn->SetCheck( BST_UNCHECKED );
	pbtn->EnableWindow(bEnable);
	scGetINP_CTL( SCN_CHKMDFDT_PN )->INP_ena = bEnable;
}


long CScanKeyView::OnTermComboDataType( long nChar )
{
	return term_focus( SCN_DTTYPE_PN, nChar );
}

void CScanKeyView::OnSelchangeDataType()
{
	int sel = 	m_DTtypeCmb.GetCurSel();

	SCAN_KEY.SCN_DTTYPE_DT.SKBN_SGN1 = (short)sel;

	if( sel == NON_SELECT ) {
		SCAN_KEY.LINE_CND[ SCN_DTTYPE_PN ].INP_sg = FALSE;
	}
	else {
		SCAN_KEY.LINE_CND[ SCN_DTTYPE_PN ].INP_sg = TRUE;
	}
}

// 検索データタイプ
void CScanKeyView::set_DataType()
{
	CRect rct, rct2;
	GetDlgItem(IDC_STATIC_GRP)->GetWindowRect(&rct);

	if( ! m_bDataTypeCombo ) {
		m_SType.GetWindowRect( rct2 );
		rct.bottom = rct2.bottom +2;
		ScreenToClient( &rct );
		GetDlgItem(IDC_STATIC_GRP)->MoveWindow(&rct);

		m_DTtypeCmb.ShowWindow( SW_HIDE );
		GetDlgItem(IDC_ST_DATMKAKU)->ShowWindow( SW_HIDE );
	}
	else {
		m_DTtypeCmb.GetWindowRect( rct2 );
		rct.bottom = rct2.bottom + 2;
		ScreenToClient( &rct );
		GetDlgItem(IDC_STATIC_GRP)->MoveWindow(&rct);

		m_DTtypeCmb.ShowWindow( SW_SHOW );
		GetDlgItem(IDC_ST_DATMKAKU)->ShowWindow( SW_SHOW );
	}
}


//-----------------------------------------------------------------------------
// タブクリックによる切替時の チェック
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CScanKeyView::TabLbuttonDownJob()
{
	last_data_set();
	return 0;
}


LRESULT CScanKeyView::OnSelChgMsg(WPARAM wParam, LPARAM lParam)
{
	OnSelchangeScnShtTyp();
	return 1;
}


HWND CScanKeyView::GetKamokuCtrlHWND()
{
	int pn = get_nowpn();
	struct _InpCtl* ctl = INP_CTL;

	HWND hwnd = ((CICSDBEDT *)(ctl + pn)->p_obj)->m_hWnd;

	return hwnd;
}


//入力コントロール，カナ検索文字数更新
void CScanKeyView::KanaSearchUpdate()
{
	struct _InpCtl* ctl = INP_CTL;

	for( int i = SCN_DEBT_PN; i <= SCN_DCBR2_PN; ++i )
	{
		if( (ctl + i)->p_obj ) {
			((CICSDBEDT *)(ctl + i)->p_obj)->SetKanaTerm(TRUE, KanaSearchKeta(), ICSDBEDT_KTYPE_KANA);
		}
	}
}


void CScanKeyView::OnBnClickedCheckMenzei()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int chk = (( CButton* )GetDlgItem( IDC_CHECK_MENZEI ))->GetCheck();
	if( chk ) {
		SCAN_KEY.LINE_CND[SCN_MENZEI_PN].INP_sg = TRUE;
	}
	else {
		SCAN_KEY.LINE_CND[SCN_MENZEI_PN].INP_sg = FALSE;
	}
}

void CScanKeyView::OnSetFocusScnInvno()
{
	if( m_DIALOG_OK == FALSE ) {
		return;
	}

	SelPar par;
	::ZeroMemory(par.tky_kana, sizeof par.tky_kana);
	par.sel_pn = SL_INVNO_PN;
	m_Seldata.SelectJob(&par, -1);

	set_nowpn( SCN_INVNO_PN );
	DispInvno( SCN_INVNO_PN, SCAN_KEY.SCN_INVNO_DT._invno );

}

void CScanKeyView::OnKillFocusScnInvno( long inplen ) 
{
	if( m_DIALOG_OK == FALSE ) {
		return;
	}

	InvnoInp( SCN_INVNO_PN, &SCAN_KEY.SCN_INVNO_DT, 0, inplen, 0 );
}

void CScanKeyView::OnTerminationScnInvno( long nChar, long inplen, long kst )
{
	if( m_DIALOG_OK == FALSE ) {
		return;
	}

	nChar = InvnoInp( SCN_INVNO_PN, &SCAN_KEY.SCN_INVNO_DT, nChar, inplen, kst );
	if( term_focus( SCN_INVNO_PN, nChar ) != 0 ) {
		TermBuzzer();
	}
}

void CScanKeyView::DispInvno( int pn, CString invno )
{
	VARIANT var;
	char tmpInvno[16] = { 0 };

	if( !invno.IsEmpty() ) {
		invno.Replace( _T( "T" ), _T( "" ));
		sprintf_s( tmpInvno, sizeof( tmpInvno ), _T( "%s" ), invno );
		var.pbVal = ( BYTE* )tmpInvno;
	}
	else {
		var.pbVal = NULL;
	}
//	DBSetDispString( this, scGetINP_CTL( pn )->IDC_X, ( char* )var.pbVal );
	DBdata_set(this, scGetINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
}

long CScanKeyView::InvnoInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	ASSERT( keybuf );
	if( keybuf == NULL ) {
		return -1;
	}

	VARIANT var;
	UpdateData();
	char buf[64];

	if( inplen ) {
		//データリード
		DBdata_get(this, scGetINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
		int n = strlen((char*)var.pbVal);

		//2ケタ未満は選択欄から
		if( n && n < 3 ) {
			int number = atoi((char*)var.pbVal);
			sprintf_s(buf, sizeof buf, "%d", number);
			SelResult srt;
			srt.sel_pn = SL_INVNO_PN;

			if( m_Seldata.GetSelResult(&srt, buf) ) {
				sprintf_s(keybuf->_invno, sizeof(keybuf->_invno), _T("%s"), srt.selname );
				var.pbVal = (BYTE*)keybuf->_invno;
				DBdata_set(this, scGetINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
				sprintf_s(buf, sizeof(buf), _T("T%s"), keybuf->_invno);
				DBSetDispString(this, IDC_SCNINVNODISP/*scGetINP_CTL(pn)->IDC_X*/, buf);
				SCAN_KEY.LINE_CND[pn].INP_sg = TRUE;
			}
			else {
				memset(keybuf->_invno, '\0', sizeof(keybuf->_invno));
				var.pbVal = NULL;
				DBdata_set(this, scGetINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
				::ZeroMemory(buf, sizeof buf);
				DBSetDispString(this, IDC_SCNINVNODISP/*scGetINP_CTL(pn)->IDC_X*/, buf);
				SCAN_KEY.LINE_CND[pn].INP_sg = FALSE;
			}
		}
	}

	// オフフォーカス
	if( !nChar ) {
		DBdata_get( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		if( strlen(( char* )var.pbVal ) > 0 ) {
			CString checkInvno;
			checkInvno.Format( _T( "T%s" ), ( char* )var.pbVal );
			if( syzInvoice.CheckInvoiceNumber( checkInvno ) == -1 ) {
			//	memset( keybuf->_invno, '\0', sizeof( keybuf->_invno ));
			//	var.pbVal = NULL;
			//	DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				m_bIgnoreFocus = TRUE;
				sprintf_s( buf, sizeof buf, "%s", (char*)var.pbVal );

				DBSetDispString(this, IDC_SCNINVNODISP/*scGetINP_CTL(pn)->IDC_X*/, buf);
				ICSMessageBox( syzInvoice.GetErrorMessage(), MB_ICONSTOP );
				m_bIgnoreFocus = FALSE;

				checkInvno.Replace("T", "");
				sprintf_s(keybuf->_invno, sizeof(keybuf->_invno), _T("%s"), checkInvno);
				var.pbVal = (BYTE*)(LPCTSTR)checkInvno;
				DBdata_set(this, scGetINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);

				set_focus( pn );
				set_nowpn( pn );
				SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
			}
			else {
				sprintf_s( keybuf->_invno, sizeof( keybuf->_invno ), _T( "%s" ),var.pbVal );
				DBSetDispString(this, IDC_SCNINVNODISP/*scGetINP_CTL(pn)->IDC_X*/, checkInvno);
				SCAN_KEY.LINE_CND[ pn ].INP_sg = TRUE;
			}
		}
		else {
			memset( keybuf->_invno, '\0', sizeof( keybuf->_invno ));
			var.pbVal = NULL;
			DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			DBSetDispString(this, IDC_SCNINVNODISP/*scGetINP_CTL(pn)->IDC_X*/, keybuf->_invno );
			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
		}
		return 0;
	}
	else {
		if( (nChar == VK_TAB) && (kst & ICSDBEDT_KST_SHIFT) ) {
			return VK_F2;
		}
		if( nChar == VK_PRIOR || nChar == VK_NEXT || nChar == VK_HOME ) {
			SelPar par;
			::ZeroMemory(par.tky_kana, sizeof par.tky_kana);
			par.sel_pn = SL_INVNO_PN;
			m_Seldata.SelectJob(&par, nChar);
		}
		else if( nChar == VK_DELETE ) {
			memset( keybuf->_invno, '\0', sizeof( keybuf->_invno ));
			var.pbVal = NULL;
			DBdata_set( this, scGetINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			DBSetDispString(this, IDC_SCNINVNODISP/*scGetINP_CTL(pn)->IDC_X*/, keybuf->_invno );
			SCAN_KEY.LINE_CND[ pn ].INP_sg = FALSE;
		}
	}

	return nChar;
}

void CScanKeyView::OnBnClickedNchkInvno()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int chk = (( CButton* )GetDlgItem( IDC_NCHK_INVNO ))->GetCheck();

	if( chk ) {
		SCAN_KEY.LINE_CND[SCN_INVNO_PN].KEY_inp |= SC_NOTSEARCH;
	}
	else {
		SCAN_KEY.LINE_CND[SCN_INVNO_PN].KEY_inp &= ~SC_NOTSEARCH;
	}
}

long CScanKeyView::OnTerminationMenzei( long nChar )
{
	return term_focus( SCN_MENZEI_PN, nChar );
}

void CScanKeyView::OnBnClickedNchkMenzei()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int chk = ((CButton*)GetDlgItem(IDC_NCHK_MENZEI))->GetCheck();

	if( chk ) {
		SCAN_KEY.LINE_CND[SCN_MENZEI_PN].KEY_inp |= SC_NOTSEARCH;
	}
	else {
		SCAN_KEY.LINE_CND[SCN_MENZEI_PN].KEY_inp &= ~SC_NOTSEARCH;
	}
}


//インボイス関連コントロールを挿入できるか？
void CScanKeyView::InvoiceCtrlMove(int begin/*= 0*/)
{
	CRect rect, ctrRect;
	//ビューの底位置
	GetClientRect(rect);

	BOOL bDownSpace = FALSE;

	CWnd* pwnd = GetDlgItem(IDCANCEL);
	if( !pwnd )	return;

	if( !::IsWindow(pwnd->m_hWnd) )
		return;

	pwnd->GetWindowRect(ctrRect);
	ScreenToClient(ctrRect);
	//下側に余白ない場合は、スクロールバーを表示する
	int aki = (rect.bottom - ctrRect.bottom);

	if( aki >= (ctrRect.Height() * 2) ) {
		bDownSpace = TRUE;
	}
	BOOL bMove = TRUE;

	// インボイス対応
	if( !bInvMaster || !bInvUse || !bInvDisp ) {
		scGetINP_CTL(SNOT_INVNO_PN)->INP_ena = FALSE;
		scGetINP_CTL(SCN_INVNO_PN)->INP_ena = FALSE;
		scGetINP_CTL(SNOT_MENZEI_PN)->INP_ena = FALSE;

		// 登録番号
		GetDlgItem(IDC_ST_INVNO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_NCHK_INVNO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCNINVNO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCNINVNODISP)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_SCNMENWARI)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCNMENWARI)->ShowWindow(SW_HIDE);
	}
	else {
		GetDlgItem(IDC_ST_INVNO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_NCHK_INVNO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCNINVNO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCNINVNODISP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCNMENWARI)->EnableWindow(TRUE);
		GetDlgItem(IDC_SCNMENWARI)->ShowWindow(SW_SHOW);
	}

	if( !bInvMaster || !bInvUse ) {
		// 免税事業者からの課税仕入れ
		GetDlgItem(IDC_CHECK_MENZEI)->ShowWindow(SW_HIDE);
		scGetINP_CTL(SCN_MENZEI_PN)->INP_ena = FALSE;
		GetDlgItem(IDC_NCHK_MENZEI)->ShowWindow(SW_HIDE);
		scGetINP_CTL(SNOT_MENZEI_PN)->INP_ena = FALSE;
	}
	else {
		GetDlgItem(IDC_CHECK_MENZEI)->ShowWindow(SW_SHOW);
		scGetINP_CTL(SCN_MENZEI_PN)->INP_ena = TRUE;
		GetDlgItem(IDC_NCHK_MENZEI)->ShowWindow(SW_SHOW);
		scGetINP_CTL(SNOT_MENZEI_PN)->INP_ena = TRUE;
	}

	// インボイス対応なし
	if( !bInvMaster || !bInvUse /*|| !bInvDisp*/ ) {
		bMove = FALSE;
	}

	int downID1[] = {
		IDC_TKYCOMBO, IDC_ST_TKY, IDC_NCHK_TKY, IDC_SCNTKY, -1
	};
	int downID2[] = {
		IDC_ST_SNUM, IDC_NCHK_SNUMBER, IDC_SCNSNUM1, IDC_STATIC13, IDC_SCNSNUM2, IDC_ST_TDAT, IDC_NCHK_TGDATE, IDC_SCNTDAT1, IDC_STATIC14, IDC_SCNTDAT2, -1
	};
	int downID3[] = {
		IDC_ST_HUSEN, IDC_NCHK_HUSEN, IDC_SCNHUSEN, IDC_SCNHUCMT, IDC_ST_NOIPBRN, IDC_SCNNOINPBRN, -1
	};

	//税区分: NOT
	CRect baseRect;
	GetDlgItem(IDC_NCHK_SKBN)->GetWindowRect(baseRect);
	ScreenToClient(baseRect);

	int topPos = baseRect.bottom + 1;
	int ht, newYpos;

	newYpos = topPos + 1;

	//インボイス関連の検索項目を挿入
	int insID[] = {
		IDC_ST_INVNO, IDC_NCHK_INVNO, IDC_SCNINVNO, IDC_NCHK_MENZEI, IDC_CHECK_MENZEI, IDC_SCNMENWARI, -1
	};

	int nextLeft = -1;

	if( bMove ) {
		int mentype = GetMenzeiWariai(Voln1);

		for( int i = 0; insID[i] != -1; i++ ) {
			GetDlgItem(insID[i])->GetWindowRect(ctrRect);
			ScreenToClient(ctrRect);
			int wd, ht;
			wd = ctrRect.Width();
			ht = ctrRect.Height();

			if( i == 4 ) {
				CWnd* pwnd = GetDBDinpMainFrame();

				if( pwnd->IsZoomed() ) {
					CFont* pFont, *pOld;
					CWnd* pwnd = GetDlgItem(insID[i]);
					pFont = pwnd->GetFont();

					// 免税事業者チェックボックス幅を調整
					CClientDC dc(pwnd);
					pOld = dc.SelectObject(pFont);
					CString wtxt;
					pwnd->GetWindowText(wtxt);
					wtxt += "あA";
					CSize sz;
					sz = dc.GetTextExtent(wtxt, wtxt.GetLength());

					wd = sz.cx;
				}
			}
			else if( i==5) {
				if( mentype == SYZ_MEN_NOT || mentype == SYZ_MEN_80 ) {
					GetDlgItem(insID[i])->EnableWindow(FALSE);
					GetDlgItem(insID[i])->ShowWindow(SW_HIDE);
				}
			}
			if( !bInvDisp ) { //登録番号は表示しないとき
				if( i <= 2 )	continue;

				if( nextLeft == -1 ) {
					ctrRect.top = newYpos + 1;
					ctrRect.left = baseRect.left;
					ctrRect.right = ctrRect.left + wd;
					ctrRect.bottom = ctrRect.top + ht;
					nextLeft = ctrRect.right + 5;
				}
				else {
					ctrRect.top = newYpos + 1;
					ctrRect.left = nextLeft;
					ctrRect.right = ctrRect.left + wd;
					ctrRect.bottom = ctrRect.top + ht;
				}
				GetDlgItem(insID[i])->MoveWindow(ctrRect);

				if( i == 3 ) {
					topPos = ctrRect.bottom + 1;
				}
				if( i == 4 ) {
					nextLeft = ctrRect.right;
				}
			}
			else {
				if( i == 0 ) {
					ctrRect.top = newYpos + 1;
					ctrRect.left = baseRect.left - wd;
					ctrRect.right = baseRect.left;
					ctrRect.bottom = ctrRect.top + ht;
				}
				else {
					ctrRect.top = newYpos;
					ctrRect.left = nextLeft;
					ctrRect.right = nextLeft + wd;
					ctrRect.bottom = ctrRect.top + ht;
				}
				GetDlgItem(insID[i])->MoveWindow(ctrRect);
				nextLeft = ctrRect.right;

				if( i == 2 || i == 3 ) {
					nextLeft += 5;
				}
				else if( i == 1 ) {
					topPos = ctrRect.bottom + 1;
				}
				else if( i == 4 ) {
					nextLeft = ctrRect.right;
				}
			}
		}
		newYpos = topPos;
	}
	
	if( bInvDisp ) { //登録番号 表示用Edit 表示
		GetDlgItem(IDC_SCNINVNO)->GetWindowRect(rect);
		ScreenToClient(rect);
		GetDlgItem(IDC_SCNINVNODISP)->SetWindowPos(&CWnd::wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	}
	
	//一行分下げる
	for( int i = 0; downID1[i] != -1; i++ ) {
		GetDlgItem(downID1[i])->GetWindowRect(ctrRect);
		ScreenToClient(ctrRect);
		ht = ctrRect.Height();

		ctrRect.top = newYpos;
		ctrRect.bottom = ctrRect.top + ht;
		GetDlgItem(downID1[i])->MoveWindow(ctrRect);
		// NOT の底座標を 次の基準
		if( i == 2 ) {
			topPos = ctrRect.bottom + 1;
		}
	}
	newYpos = topPos;

	for( int i = 0; downID2[i] != -1; i++ ) {
		GetDlgItem(downID2[i])->GetWindowRect(ctrRect);
		ScreenToClient(ctrRect);
		ht = ctrRect.Height();

		ctrRect.top = newYpos;
		ctrRect.bottom = ctrRect.top + ht;
		GetDlgItem(downID2[i])->MoveWindow(ctrRect);
		// NOT の底座標を 次の基準
		if( i == 1 ) {
			topPos = ctrRect.bottom + 1;
		}
	}
	newYpos = topPos;

	for( int i = 0; downID3[i] != -1; i++ ) {
		GetDlgItem(downID3[i])->GetWindowRect(ctrRect);
		ScreenToClient(ctrRect);
		ht = ctrRect.Height();

		ctrRect.top = newYpos;
		ctrRect.bottom = ctrRect.top + ht;
		GetDlgItem(downID3[i])->MoveWindow(ctrRect);
	}
	//ボタンの移動
	//手形期日２を基本位置
	CRect btnBase;
	GetDlgItem(IDC_SCNTDAT2)->GetWindowRect(btnBase);
	ScreenToClient(btnBase);
	newYpos = btnBase.top + (btnBase.Height() / 2);

	int btnID[] = { IDC_TKYONLY,IDC_ALLKEYDEL, IDOK, IDC_BTN_DUPLI, IDCANCEL, IDC_BTN_DEL, -1 };

	for( int i = 0; btnID[i] != -1; i++ ) {
		GetDlgItem(btnID[i])->GetWindowRect(ctrRect);
		ScreenToClient(ctrRect);
		ht = ctrRect.Height();
		ctrRect.top = newYpos;
		ctrRect.bottom = ctrRect.top + ht;
		GetDlgItem(btnID[i])->MoveWindow(ctrRect);

		if( btnID[i] == IDC_ALLKEYDEL || btnID[i] == IDC_BTN_DUPLI ) {
			newYpos = ctrRect.bottom + 1;
		}
	}

	//
	if( begin ) {
	//	if( !bDownSpace ) {
			GetDlgItem(IDC_BTN_DEL)->GetWindowRect(ctrRect);
			ScreenToClient(ctrRect);
			ht = ctrRect.Height();
			this->ScrollToPosition(CPoint(0, 0));
			rect.bottom = ctrRect.bottom + ht;
			rect.right = ctrRect.right + 5;

			CSize szTotal;
			szTotal.cx = rect.Width();
			szTotal.cy = rect.Height();

			SetScrollSizes(MM_TEXT, szTotal);
	//	}
	}
}


//インボイス キー移動テーブルの変更
void CScanKeyView::InvoiceKeyMoveChange()
{
	// 登録番号
	static struct _KeyMoveTbl  normalInv[5] = {
		{ SNOT_INVNO_PN, SNOT_SKBN_PN, SNOT_TKY_PN, SCN_TOKUS_PN, SCN_INVNO_PN },
		{ SCN_INVNO_PN, SCN_SKBN_PN, SCN_TKYONLY, SNOT_INVNO_PN, SNOT_MENZEI_PN },
		{ SNOT_MENZEI_PN, SNOT_SKBN_PN, SNOT_TKY_PN, SCN_INVNO_PN,	SCN_MENZEI_PN },
		{ SCN_MENZEI_PN, SCN_SKBN_PN, SNOT_TKY_PN, SNOT_MENZEI_PN, SCN_MENWARI_PN },
		{ SCN_MENWARI_PN, -1, -1, SCN_MENZEI_PN, SCN_SELTKY_PN },
	};
	// 免税のみ
	static struct _KeyMoveTbl  onlyMenz[5] = {
		{ SNOT_INVNO_PN, SNOT_MENZEI_PN, SNOT_MENZEI_PN, SNOT_MENZEI_PN, SNOT_MENZEI_PN },
		{ SCN_INVNO_PN, SCN_MENZEI_PN, SCN_MENZEI_PN, SCN_MENZEI_PN, SCN_MENZEI_PN},
		{ SNOT_MENZEI_PN, SNOT_SKBN_PN, SNOT_TKY_PN, SCN_TOKUS_PN, SCN_MENZEI_PN},
		{ SCN_MENZEI_PN,  SCN_SKBN_PN, SCN_TKY_PN, SNOT_MENZEI_PN, SNOT_TKY_PN},
		{ SCN_MENWARI_PN, -1, -1, SCN_MENZEI_PN, SNOT_TKY_PN },
	};
	struct _KeyMoveTbl* km, *chgsrc;
	km = SkeyMvTbl;

	//免税のチェックボックスのみ 登録番号表示なし
	if( bInvMaster && bInvUse && !bInvDisp ) {
		chgsrc = onlyMenz;
	}
	else {
		chgsrc = normalInv;
	}

	for( int i = 0; (km + i)->nowpn != -1; i++ ) {
		switch( (km + i)->nowpn ) {
		case SNOT_INVNO_PN:
			memcpy((km + i), &chgsrc[0], sizeof(struct _KeyMoveTbl) );
			break;
		case SCN_INVNO_PN:
			memcpy((km + i), &chgsrc[1], sizeof(struct _KeyMoveTbl));
			break;
		case SNOT_MENZEI_PN:
			memcpy((km + i), &chgsrc[2], sizeof(struct _KeyMoveTbl));
			break;
		case SCN_MENZEI_PN:
			memcpy((km + i), &chgsrc[3], sizeof(struct _KeyMoveTbl));
			break;
		case SCN_MENWARI_PN:
			memcpy((km + i), &chgsrc[4], sizeof(struct _KeyMoveTbl));
			break;
		}
	}

	int mentype = GetMenzeiWariai(Voln1);
	if( mentype == SYZ_MEN_NOT || mentype == SYZ_MEN_80 ) {
		scGetINP_CTL(SCN_MENWARI_PN)->INP_ena = FALSE;
	}
	else {
		scGetINP_CTL(SCN_MENWARI_PN)->INP_ena = TRUE;
	}
}


long CScanKeyView::OnTerminationMenWari(long nChar)
{
	if( m_DIALOG_OK == FALSE )
		return nChar;

	//カーソル移動
	if( term_focus(SCN_MENWARI_PN, nChar) != 0 ) {
		TermBuzzer();
		return nChar;
	}
	return 0;
}

// 免税事業者　控除割合
void CScanKeyView::OnSelchangeMenWari()
{
	int sel = m_MenWariCmb.GetCurSel();
	SCAN_KEY.SCN_MENWARI_DT.SKBN_SGN1 = sel;
	m_MenWariCmb.GetLBText(sel, SCAN_KEY.SCN_MENWARI_DT.SKBN_TXT);

	SCAN_KEY.LINE_CND[SCN_MENWARI_PN].INP_sg = TRUE;
}


long CScanKeyView::CheckDataDBeditInv(long nChar, LPCTSTR string)
{
	VARIANT var;
	int num = atoi(string);

	int idc = scGetINP_CTL(SCN_INVNO_PN)->IDC_X;
	int len = strlen(string);

	//選択欄より
	if( num <= 32 && len <= 2) {
		var.pbVal = (BYTE*)string;
		DBdata_set(this, idc, &var, ICSDBEDT_TYPE_STRING, 0);
	}
	else {
		//登録番号チェック は Terminationで
		CICSDBEDT* pDedt;
		pDedt = (CICSDBEDT*)(GetDlgItem(idc));
		int pos = pDedt->GetCaretPosition();
		var.pbVal = (BYTE*)string;

		if( len == 13 || pos == 13 ) {
		DBdata_set(this, idc, &var, ICSDBEDT_TYPE_STRING, 0);
	}
		else {
			pDedt->InsertData(&var, ICSDBEDT_TYPE_STRING, 0, pos);
		}
	}
	return nChar;
}


void CScanKeyView::ReCoselSetFocus(BOOL bNewCo)
{
	if( bNewCo ) {
	}
	else {
		int pn;
		pn = get_nowpn();
		set_focus(pn);
	}
}


int CScanKeyView::GetToolSub()
{
	int ret = 0;
	int pn;

	pn = get_nowpn();

	// ツールバー切替
	switch( pn )
	{
	case SCN_DEBT_PN:	case SCN_DEBT2_PN:
	case SCN_CRED_PN:	case SCN_CRED2_PN:
	case SCN_DBCR_PN:	case SCN_DBCR2_PN:
		ret = 1;
		break;
	default:
		break;
	}

	return ret;
}


#include "CDupliDlg.h"

// 重複仕訳検索
void CScanKeyView::OnBnClickedBtnDupli()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
#ifdef CLOSE
	CDupliDlg	dlg(this);

	dlg.m_dwChkBit = ((CMainFrame*)GetDBDinpMainFrame())->DupliSwk();
	dlg.bmon_mast = BMON_MST;
	dlg.koji_mast = M_KOJI;

	int st = dlg.DoModal();

	if( st == IDOK ) {
		((CMainFrame*)GetDBDinpMainFrame())->DupliSwk() = dlg.m_dwChkBit;
		pDBzm->IsDupliScan() = TRUE;
		ButtonF8Job(0);
	}
	else {
		pDBzm->IsDupliScan() = FALSE;
	}
#endif
}



// 手形番号 20文字拡張でフォーカスがONのときは幅を広げる
void CScanKeyView::TGnumWndMove()
{
	int pn = get_nowpn();

	//IDC_NCHK_SNUMBER
	//IDC_SCNSNUM1
	//IDC_STATIC13
	//IDC_SCNSNUM2

	CRect	rect, edt1, edt2;
	CRect	base, base2;
	int xpos1, xpos2;
	int xend1, xend2;

	GetDlgItem(IDC_NCHK_SNUMBER)->GetWindowRect(&base);
	ScreenToClient(base);
	GetDlgItem(IDC_STATIC13)->GetWindowRect(&base2);
	ScreenToClient(base2);

	GetDlgItem(IDC_SCNSNUM1)->GetWindowRect(&edt1);
	ScreenToClient(edt1);

	GetDlgItem(IDC_SCNSNUM2)->GetWindowRect(&edt2);
	ScreenToClient(edt2);

	if( pn == SCN_SNUM1_PN ) {
		CSize sz = DBEditTextSize(IDC_SCNSNUM1, this, 20);

		xpos1 = base.right;
		xend1 = xpos1 + sz.cx;
	}
	else {
		xpos1 = base.right;
		xend1 = base2.left;
		xend1 -= (base2.Width() / 2);
	}

	xpos2 = base2.right;
	xpos2 += (base2.Width() / 2);

	if( pn == SCN_SNUM2_PN ) {
		CSize sz = DBEditTextSize(IDC_SCNSNUM2, this, 20);

		xend2 = xpos2 + sz.cx;
	}
	else {
		CRect tgdate;
		GetDlgItem(IDC_ST_TDAT)->GetWindowRect(&tgdate);
		ScreenToClient(tgdate);
		xend2 = tgdate.left;;
		xend2 -= (base2.Width() / 2);
	}
	int ht = edt1.Height();


	GetDlgItem(IDC_SCNSNUM1)->MoveWindow(xpos1, edt1.top, xend1-xpos1, ht );
	GetDlgItem(IDC_SCNSNUM2)->MoveWindow(xpos2, edt2.top, xend2-xpos2, ht);
}