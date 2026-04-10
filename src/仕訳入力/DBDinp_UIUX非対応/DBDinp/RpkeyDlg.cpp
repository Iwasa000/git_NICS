// RpkeyDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dbdinp.h"

#include "scansydef.h"

#include "DBDinpRec.h"
#include "input1.h"
#include "external.h"

#include "RpkeyDlg.h"

#include "DataListCtrl.h"
#include "DinpSub.h"
#include "SubFnc.h"
#include "MessageDef.h"

#include "icsdbedt.h"
#include "mstrw.h"

#include "DBDinpDoc.h"
#include "DBDinpView.h"

#include "MainFrm.h"
#include "SelImgDlg.h"
#include "BmnChk.h"

extern CBmnHaniCheck	BmChk;

//消費税モジュール
extern CDBSyohi* pDBsy;

extern CDBDinpView *p_DBDinpView;

extern struct _AUTOSEL *pAUTOSEL;

extern CBrnTky m_BrTek;

extern
BOOL IsKaribarai( DWORD kncd );
extern
BOOL IsKariuke( DWORD kncd );
extern
int fncKanaCheckVK_BACK(MSG* pMsg, HWND hWnd);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRpkeyDlg ダイアログ

// 入力項目の初期化
struct _RPL_LINE CRpkeyDlg::m_Rpline = {0};
short CRpkeyDlg::m_TEK_MODE = 0;

BYTE	CRpkeyDlg::m_MNTHbcd1[4] = {0};
BYTE	CRpkeyDlg::m_MNTHbcd2[4] = {0};
BYTE	CRpkeyDlg::m_MMDDbcd1[4] = {0};
BYTE	CRpkeyDlg::m_MMDDbcd2[4] = {0};


// input control table
static struct _InpCtl RPINP_CTL[RP_MAX_PN+1] = {
	{IDC_SCNDATE1,0,0,0,-1},	{IDC_SCNDATE2,0,0,0,-1},	//0
	{IDC_SCNDENP1,0,0,0,-1},	{IDC_SCNDENP2,0,0,0,-1},
	{IDC_SCNSNO1,0,0,0,-1},		{IDC_SCNSNO2,0,0,0,-1},

	{IDC_SCNBMON1,0,0,0,-1},	{IDC_SCNBMON2,0,0,0,-1},
	{IDC_SCNCBMN1,0,0,0,-1},	{IDC_SCNCBMN2,0,0,0,-1},
	{IDC_SCNKOJI1,0,0,0,-1},	{IDC_SCNKOJI2,0,0,0,-1},	//5
	{IDC_SCNCKJI1,0,0,0,-1},	{IDC_SCNCKJI2,0,0,0,-1},	

	{IDC_SCNDEBT,0,0,0,-1},		{IDC_SCNDEBT2,0,0,0,-1},

	// 登録番号
	{ IDC_SCN_INVNO,IDC_SCN_INVNODISP,0,0,-1},	{ IDC_SCN_INVNO2,IDC_SCN_INVNO2DISP,0,0,-1},

	{IDC_SCNCRED,0,0,0,-1},		{IDC_SCNCRED2,0,0,0,-1},
	{IDC_SCNVAL1,0,0,0,-1},		{IDC_SCNVAL2,0,0,0,-1},		//10

	// 免税事業者からの課税仕入れ, 課税事業者からの課税仕入れ
	{ IDC_CHK_MENZEI,0,0,0,-1},	
	{ IDC_SCNMENWARI,0,0,0,-1},{ IDC_SCNMENWARI2,0,0,0,-1},
	{ IDC_CHK_KAZEI,0,0,0,-1},

	{IDC_SCNSYZSW,0,0,0,-1},	{IDC_SCNSYZSW2,0,0,0,-1},
	{IDC_SCNZRITU,0,0,0,-1},	{IDC_SCNZRITU2,0,0,0,-1},
	{IDC_SCNSKBN,0,0,0,-1},		{IDC_SCNSKBN2,0,0,0,-1},
	{IDC_SCNURISIRE,0,0,0,-1},	{IDC_SCNURISIRE2,0,0,0,-1},	//15
	{IDC_SCNTOKUS,0,0,0,-1},	{IDC_SCNTOKUS2,0,0,0,-1},

	{IDC_SCNTKY,0,0,0,-1},		{IDC_SCNTKY2,0,0,0,-1},
	{IDC_SCNHJIYU,0,0,0,-1},	{IDC_SCNHJIYU2,0,0,0,-1},
	{IDC_SCNSNUM1,0,0,0,-1},	{IDC_SCNSNUM2,0,0,0,-1},
	{IDC_SCNTDAT1,0,0,0,-1},	{IDC_SCNTDAT2,0,0,0,-1},	//20
	{IDC_SCNHUSEN,0,0,0,-1},	{IDC_SCNHUSEN2,0,0,0,-1},

	// チェックボックス
	{IDC_CHK_HUSEN, 0,0,0,-1 },
	{IDC_CHECK_DENP,0,0,0,-1},	{IDC_CHECK_SNO,0,0,0,-1},
	{IDC_CHECK_DBM,0,0,0,-1},	{IDC_CHECK_CBM,0,0,0,-1},
	{IDC_CHECK_DKJ,0,0,0,-1},	{IDC_CHECK_CKJ,0,0,0,-1},	//25
	{IDC_CHECK_TKY,0,0,0,-1},
	{IDC_CHK_CONFIRM,0,0,0,-1},
	{IDC_CHK_CONMDFY,0,0,0,-1},

	{ 0, 0,0,0,-1 },
};

// キーの移動先 テーブル
struct _KeyMoveTbl RpMoveTbl[] = 
{
	{RP_DATE1_PN,	-1, RP_DENP1_PN, -1,			RP_DATE2_PN },
	{RP_DATE2_PN,	-1, RP_DENP2_PN, RP_DATE1_PN,	RP_DENP1_PN },
	{RP_MNONLY_PN,	-1, RP_NODENP_PN, RP_DATE2_PN,	RP_DENP1_PN },

	{RP_DENP1_PN,	RP_DATE1_PN, RP_BMON1_PN, RP_MNONLY_PN,	RP_DENP2_PN },
	{RP_DENP2_PN,	RP_DATE2_PN, RP_BMON2_PN, RP_DENP1_PN,	RP_NODENP_PN },
	{RP_NODENP_PN,	RP_MNONLY_PN,RP_NOBMON_PN,RP_DENP2_PN,	RP_DOCEVI1_PN },

	//証憑番号	[11.26 /12]
	{RP_DOCEVI1_PN,		RP_DATE1_PN, RP_BMON1_PN, RP_NODENP_PN,		RP_DOCEVI2_PN },
	{RP_DOCEVI2_PN,		RP_DATE2_PN, RP_BMON2_PN, RP_DOCEVI1_PN,	RP_NODOCEVI_PN },
	{RP_NODOCEVI_PN,	RP_MNONLY_PN,RP_NOBMON_PN,RP_DOCEVI2_PN,	RP_BMON1_PN },

	{RP_BMON1_PN,	RP_DENP1_PN, RP_CBMN1_PN, RP_NODOCEVI_PN,	RP_BMON2_PN },
	{RP_BMON2_PN,	RP_DENP2_PN, RP_CBMN2_PN, RP_BMON1_PN,		RP_NOBMON_PN },
	{RP_NOBMON_PN,	RP_NODENP_PN,RP_NOCBMN_PN, RP_BMON2_PN,		RP_KOJI1_PN },

	{RP_CBMN1_PN,	RP_BMON1_PN, RP_DEBT1_PN,	RP_NOBMON_PN,	RP_CBMN2_PN },
	{RP_CBMN2_PN,	RP_BMON2_PN, RP_DEBT2_PN,	RP_CBMN1_PN,	RP_NOCBMN_PN },
	{RP_NOCBMN_PN,	RP_NOBMON_PN,RP_DEBT2_PN,	RP_CBMN2_PN,	RP_CKJI1_PN },

	{RP_KOJI1_PN,	RP_DENP2_PN, RP_CKJI1_PN, RP_NOCBMN_PN,	RP_KOJI2_PN },
	//{RP_KOJI2_PN,	RP_DENP2_PN, RP_CKJI2_PN, RP_KOJI1_PN,	RP_NOKOJI_PN },
	{RP_KOJI2_PN,	RP_DENP2_PN, RP_INVNO_PN, RP_KOJI1_PN,	RP_NOKOJI_PN },
	{RP_NOKOJI_PN,	RP_DENP2_PN, RP_NOCKJI_PN,RP_KOJI2_PN,	RP_CBMN1_PN },

	{RP_CKJI1_PN,	RP_KOJI1_PN, RP_DEBT1_PN,	RP_NOKOJI_PN,	RP_CKJI2_PN },
	{RP_CKJI2_PN,	RP_KOJI2_PN, RP_DEBT1_PN,	RP_CKJI1_PN,	RP_NOCKJI_PN },
	{RP_NOCKJI_PN,	RP_NOCKJI_PN,RP_DEBT1_PN,	RP_CKJI2_PN,	RP_DEBT1_PN },

	{RP_DEBT1_PN,	RP_CBMN1_PN, RP_CRED1_PN,	RP_NOCKJI_PN,	RP_DEBT2_PN },
	//{RP_DEBT2_PN,	RP_CBMN2_PN, RP_CRED2_PN,	RP_DEBT1_PN,	RP_CRED1_PN },
	{RP_DEBT2_PN,	RP_CBMN2_PN, RP_CRED2_PN,	RP_DEBT1_PN,	RP_INVNO_PN },

	// 登録番号
	{RP_INVNO_PN, RP_KOJI2_PN, RP_MENZEI_PN, RP_DEBT2_PN, RP_INVNO2_PN },
	{RP_INVNO2_PN, RP_KOJI2_PN, RP_MENZEI_PN, RP_INVNO_PN, RP_CRED1_PN },

	{RP_CRED1_PN,	RP_DEBT1_PN, RP_VAL1_PN,	RP_INVNO2_PN,	RP_CRED2_PN },
	{RP_CRED2_PN,	RP_DEBT2_PN, RP_VAL2_PN,	RP_CRED1_PN,	RP_VAL1_PN },

	{RP_VAL1_PN,	RP_CRED1_PN, RP_SYZSW1_PN,	RP_CRED2_PN,	RP_VAL2_PN },
	{RP_VAL2_PN,	RP_CRED2_PN, RP_SYZSW2_PN,	RP_VAL1_PN,		RP_MENZEI_PN },

	// 免税事業者 課税仕入れ
	{RP_MENZEI_PN, RP_INVNO_PN, RP_MENWARI1_PN, RP_VAL2_PN, -1 },

	{RP_MENWARI1_PN,  RP_MENZEI_PN, RP_KAZEI_PN, RP_MENZEI_PN, -1 },
	{RP_MENWARI2_PN, RP_MENZEI_PN, RP_KAZEI_PN, RP_MENWARI1_PN, -1 },

	{RP_KAZEI_PN, RP_MENWARI1_PN, -1, RP_VAL2_PN, -1 },

	{RP_SYZSW1_PN,	-1,-1,	RP_KAZEI_PN,	RP_SYZSW2_PN },
	{RP_SYZSW2_PN,	-1,-1,	RP_SYZSW1_PN,	RP_ZRITU1_PN },
	{RP_ZRITU1_PN,	-1,-1,	RP_SYZSW2_PN,	RP_ZRITU2_PN },
	{RP_ZRITU2_PN,	-1,-1,	RP_ZRITU1_PN,	RP_SKBN1_PN },
	{RP_SKBN1_PN,	-1,-1,	RP_ZRITU2_PN,	RP_SKBN2_PN },
	{RP_SKBN2_PN,	-1,-1,	RP_SKBN1_PN,	RP_URISIRE1_PN },
	{RP_URISIRE1_PN, -1,-1,	RP_SKBN2_PN,	RP_URISIRE2_PN },
	{RP_URISIRE2_PN, -1,-1,	RP_URISIRE1_PN,	RP_TOKUS1_PN },
	//特定収入
	{RP_TOKUS1_PN, -1,-1,	RP_URISIRE1_PN,	RP_TOKUS2_PN },
	{RP_TOKUS2_PN, -1,-1,	RP_TOKUS1_PN,	RP_TKY1_PN },

	{RP_TKY1_PN,	RP_URISIRE1_PN,	RP_TKY2_PN,		RP_TOKUS2_PN,	RP_TKY2_PN},
	{RP_TKY2_PN,	RP_TKY1_PN,		RP_HJIYU1_PN,	RP_TKY1_PN,		RP_NOTKY_PN},
	{RP_NOTKY_PN,	RP_TKY2_PN,		RP_HJIYU1_PN,	RP_TKY2_PN,		RP_HJIYU1_PN},

	{RP_HJIYU1_PN,	-1,-1,	RP_NOTKY_PN,	RP_HJIYU2_PN },
	{RP_HJIYU2_PN,	-1,-1,	RP_HJIYU1_PN,	RP_SNUM1_PN},

	{RP_SNUM1_PN,	RP_HJIYU1_PN,	RP_TDAT1_PN,	RP_HJIYU2_PN,	RP_SNUM2_PN },
	{RP_SNUM2_PN,	RP_HJIYU2_PN,	RP_TDAT2_PN,	RP_SNUM1_PN,	RP_TDAT1_PN},
	{RP_TDAT1_PN,	RP_SNUM1_PN,	RP_HSEN1_PN,	RP_SNUM2_PN,	RP_TDAT2_PN },
	{RP_TDAT2_PN,	RP_SNUM2_PN,	RP_HSEN1_PN,	RP_TDAT1_PN,	RP_HSEN1_PN},

	{RP_HSEN1_PN,	-1, -1,		RP_TDAT2_PN,	RP_HSEN2_PN},
	{RP_HSEN2_PN,	-1, -1,		RP_HSEN1_PN,	RP_HSENCHK_PN},
	{RP_HSENCHK_PN,	RP_TDAT2_PN, RP_CONFIRM_PN,	RP_HSEN2_PN,	RP_CONFIRM_PN},

	{RP_CONFIRM_PN,	RP_HSEN1_PN, RP_CONMDFY_PN,	RP_HSENCHK_PN,	-1},
	{RP_CONMDFY_PN, RP_CONFIRM_PN, -1, RP_CONFIRM_PN, -1},
	{RP_IDOK,		-1, -1, -1, -1},

	{-1,-1,-1,-1,-1}

};


typedef struct _WARNING_TBL
{
	int		b_pn;	// 修正前ｺﾝﾄﾛｰﾙﾎﾟｼﾞｼｮﾝ
	int		a_pn;	// 修正後ｺﾝﾄﾛｰﾙﾎﾟｼﾞｼｮﾝ
	int		pnsw;	// 修正前未入力サイン
} WARNING_TBL;

// 警告ﾁｪｯｸﾃｰﾌﾞﾙ
static WARNING_TBL CHK_PNTBL[] = {
	{RP_DATE1_PN, RP_DATE2_PN, 0}, {RP_DENP1_PN, RP_DENP2_PN, 0},
	{RP_DOCEVI1_PN, RP_DOCEVI2_PN, 0},
	{RP_BMON1_PN, RP_BMON2_PN, 0}, {RP_CBMN1_PN, RP_CBMN2_PN, 0}, 
	{RP_KOJI1_PN, RP_KOJI2_PN, 0}, {RP_CKJI1_PN, RP_CKJI2_PN, 0}, 

	{RP_DEBT1_PN, RP_DEBT2_PN, 0},
	{RP_CRED1_PN, RP_CRED2_PN, 0}, {RP_VAL1_PN, RP_VAL2_PN, 0},
	
	{RP_SYZSW1_PN, RP_SYZSW2_PN, 0},{RP_ZRITU1_PN, RP_ZRITU2_PN, 0}, 
	{RP_SKBN1_PN, RP_SKBN2_PN, 0}, 	{RP_URISIRE1_PN, RP_URISIRE2_PN, 0},
	{RP_TOKUS1_PN, RP_TOKUS2_PN, 0},

	{RP_TKY1_PN, RP_TKY2_PN, 0},
	{RP_HJIYU1_PN, RP_HJIYU2_PN, 0},

	{RP_SNUM1_PN, RP_SNUM2_PN, 0},
	{RP_TDAT1_PN, RP_TDAT2_PN, 0},

	{RP_HSEN1_PN, RP_HSEN2_PN, 0},
	{-1, RP_CONFIRM_PN, 0}, {-1, RP_CONMDFY_PN, 0},

	// インボイス対応
	{ RP_INVNO_PN, RP_INVNO2_PN, 0 },
	{ -1, RP_MENZEI_PN, 0 },
	{ -1, RP_KAZEI_PN, 0 },

};


//仕訳データに仮受・仮払消費税のデータが含まれているか？
//
BOOL CheckSyzKariCode(CDBDATA* data, CString syz1, CString syz2)
{
	CString cmp1, sycd1, sycd2;

	sycd1 = syz1.Left(6);
	sycd2 = syz2.Left(6);
	//借方
	cmp1 = data->dbt.Left(6);
	if( cmp1.CompareNoCase(sycd1) == 0 )
		return TRUE;
	if( cmp1.CompareNoCase(sycd2) == 0 )
		return TRUE;

	//貸方
	cmp1 = data->cre.Left(6);
	if( cmp1.CompareNoCase(sycd1) == 0 )
		return TRUE;
	if( cmp1.CompareNoCase(sycd2) == 0 )
		return TRUE;

	return FALSE;
}



CRpkeyDlg::CRpkeyDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CRpkeyDlg::IDD, pParent)
	, m_NoDenp(FALSE)
	, m_NoDbmn(FALSE)
	, m_NoCbmn(FALSE)
	, m_NoDkoji(FALSE)
	, m_NoCkoji(FALSE)
	, m_Confirm(FALSE)
	, m_ConMdfy(FALSE)
	, m_ChkHusen(FALSE)
	, m_NoTky(FALSE)
	, m_ChkMnonly(FALSE)
	, m_NoDocEvi(FALSE)
{
	//{{AFX_DATA_INIT(CRpkeyDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

#ifdef	_ICSPEN_	// 01.20 /01
	flgPen	=	FALSE;
#endif

	m_PGSW = 0;

	pZmSel = NULL;

	m_bSwkDel = FALSE;
	m_bIgnoreFocus = FALSE;
}


CRpkeyDlg::~CRpkeyDlg()
{
	if( pZmSel != NULL ) {
		delete pZmSel;
		pZmSel = NULL;
	}
}

#undef TRACE
#define TRACE	MyTrace


void CALLBACK CRpkeyDlg::callback(UINT n,void* p,CWnd* pwnd )
{
	CRpkeyDlg* pDlg;
	pDlg = (CRpkeyDlg*)pwnd;

	if( n == IDOK ) {
		ZmselDBData* zd = (ZmselDBData*)p;

TRACE( "*** callback IDOK mode = %d, errflg %d\n", zd->selmode, zd->errflg );

		if( pDlg ) {
			pDlg->ZmSelSet( zd );
		}
	}
	else {
		if( pDlg ) {
			pDlg->ZmSelBackFocus();
		}
		TRACE( "--- callback not IDOK \n" );
	}

}

// 財務選択より
void CRpkeyDlg::ZmSelSet( ZmselDBData* zd )
{
	char tmp[128];
	int ID, pn, pos;
	VARIANT var;

	pn = get_nowpn();

	switch( pn ) {
	case RP_TKY1_PN:
	case RP_TKY2_PN:
		ID = getINP_CTL( pn )->IDC_X;

		strcpy_s( tmp, sizeof tmp, zd->tkstr );
		var.pbVal = (BYTE*)tmp;
		pos = ((CICSDBEDT*)GetDlgItem(ID))->GetCaretPosition();
		((CICSDBEDT*)GetDlgItem(ID))->InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );

		break;

	case RP_DEBT1_PN:
	case RP_DEBT2_PN:
	case RP_CRED1_PN:
	case RP_CRED2_PN:
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
		KamokuSelDataJob( pn, zd );
		break;

	case RP_BMON1_PN:
	case RP_BMON2_PN:
	case RP_CBMN1_PN:
	case RP_CBMN2_PN:
	case RP_KOJI1_PN:
	case RP_KOJI2_PN:
	case RP_CKJI1_PN:
	case RP_CKJI2_PN:
		BumonKojiSelDataJob( pn, zd );
		break;

	case RP_INVNO_PN:
	case RP_INVNO2_PN:
		InvNoSelDataJob(pn, zd);
		break;

	}
}

// 選択画面ダイアログにフォーカスがある場合で ESCAPE などが押された
void CRpkeyDlg::ZmSelBackFocus()
{
	set_focus( get_nowpn() );
}


void CRpkeyDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRpkeyDlg)
	// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	DDX_Control(pDX, IDC_SCNZRITU, m_Zritu);
	DDX_Control(pDX, IDC_SCNURISIRE, m_Urisire);
	DDX_Control(pDX, IDC_SCNSYZSW, m_Syzsw);
	DDX_Control(pDX, IDC_SCNSKBN, m_Skbn);
	DDX_Control(pDX, IDC_SCNZRITU2, m_Zritu2);
	DDX_Control(pDX, IDC_SCNURISIRE2, m_Urisire2);
	DDX_Control(pDX, IDC_SCNSYZSW2, m_Syzsw2);
	DDX_Control(pDX, IDC_SCNSKBN2, m_Skbn2);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDC_BTN_DEL, m_BtnDel);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_DENP, m_NoDenp);
	DDX_Check(pDX, IDC_CHECK_DBM, m_NoDbmn);
	DDX_Check(pDX, IDC_CHECK_CBM, m_NoCbmn);
	DDX_Check(pDX, IDC_CHECK_DKJ, m_NoDkoji);
	DDX_Check(pDX, IDC_CHECK_CKJ, m_NoCkoji);
	DDX_Check(pDX, IDC_CHECK_TKY, m_NoTky);
	DDX_Check(pDX, IDC_CHK_CONFIRM, m_Confirm);
	DDX_Check(pDX, IDC_CHK_CONMDFY, m_ConMdfy);
	DDX_Control(pDX, IDC_SCNHJIYU, m_Hjiyu1);
	DDX_Control(pDX, IDC_SCNHJIYU2, m_Hjiyu2);

	DDX_Control(pDX, IDC_STATIC_TKY1, m_image1);
	DDX_Control(pDX, IDC_STATIC_TKY2, m_image2);
	DDX_Check(pDX, IDC_CHK_HUSEN, m_ChkHusen);

	DDX_Control(pDX, IDC_SCNTOKUS, m_Tokus);
	DDX_Control(pDX, IDC_SCNTOKUS2, m_Tokus2);

	DDX_Control(pDX, IDC_SCNHUSEN,  m_Hcbo);
	DDX_Control(pDX, IDC_SCNHUSEN2, m_Hcbo2);

	DDX_Check(pDX, IDC_CHECK_MNONLY, m_ChkMnonly);

	DDX_Check(pDX, IDC_CHECK_SNO, m_NoDocEvi);

	DDX_Check( pDX, IDC_CHK_MENZEI, m_MenzeiChk );
	DDX_Check( pDX, IDC_CHK_KAZEI, m_KazeiChk );
	DDX_Control(pDX, IDC_SCNMENWARI, m_MenWari);
	DDX_Control(pDX, IDC_SCNMENWARI2, m_MenWari2);
}


BEGIN_MESSAGE_MAP(CRpkeyDlg, ICSDialog)
	//{{AFX_MSG_MAP(CRpkeyDlg)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_SCNSYZSW, OnSelchangeScnsyzsw)
	ON_CBN_SELCHANGE(IDC_SCNSYZSW2, OnSelchangeScnsyzsw2)
	ON_CBN_SELCHANGE(IDC_SCNZRITU, OnSelchangeScnzritu)
	ON_CBN_SELCHANGE(IDC_SCNZRITU2, OnSelchangeScnzritu2)
	ON_CBN_SELCHANGE(IDC_SCNSKBN, OnSelchangeScnskbn)
	ON_CBN_SELCHANGE(IDC_SCNSKBN2, OnSelchangeScnskbn2)
	ON_CBN_SELCHANGE(IDC_SCNURISIRE, OnSelchangeScnurisire)
	ON_CBN_SELCHANGE(IDC_SCNURISIRE2, OnSelchangeScnurisire2)
	ON_CBN_SELCHANGE(IDC_SCNTOKUS, OnSelchangeScnTokus)
	ON_CBN_SELCHANGE(IDC_SCNTOKUS2, OnSelchangeScnTokus2)

	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_RETROFOCUS, RetroFocus )
	ON_BN_CLICKED(IDC_CHECK_DENP, &CRpkeyDlg::OnBnClickedCheckDenp)
	ON_BN_CLICKED(IDC_CHECK_DBM, &CRpkeyDlg::OnBnClickedCheckDbm)
	ON_BN_CLICKED(IDC_CHECK_CBM, &CRpkeyDlg::OnBnClickedCheckCbm)
	ON_BN_CLICKED(IDC_CHECK_DKJ, &CRpkeyDlg::OnBnClickedCheckDkj)
	ON_BN_CLICKED(IDC_CHECK_CKJ, &CRpkeyDlg::OnBnClickedCheckCkj)
	ON_CBN_SELCHANGE(IDC_SCNHJIYU, &CRpkeyDlg::OnCbnSelchangeScnhjiyu)
	ON_CBN_SELCHANGE(IDC_SCNHJIYU2, &CRpkeyDlg::OnCbnSelchangeScnhjiyu2)
	ON_BN_CLICKED(IDC_CHK_CONFIRM, &CRpkeyDlg::OnBnClickedChkConfirm)
	ON_BN_CLICKED(IDC_CHK_CONMDFY, &CRpkeyDlg::OnBnClickedChkConMdfy)

	ON_MESSAGE( MESSAGE_IMGST_KEY, OnImageStaticKey)
	ON_MESSAGE( MESSAGE_IMGST_FOCUS, OnImageStaticFocus)

	ON_BN_CLICKED(IDC_CHK_HUSEN, &CRpkeyDlg::OnBnClickedChkHusen)
	ON_CBN_SELCHANGE(IDC_SCNHUSEN, &CRpkeyDlg::OnCbnSelchangeScnhusen)
	ON_CBN_SELCHANGE(IDC_SCNHUSEN2, &CRpkeyDlg::OnCbnSelchangeScnhusen2)
	ON_BN_CLICKED(IDC_SWKDEL, &CRpkeyDlg::OnBnClickedSwkdel)
	ON_BN_CLICKED(IDC_CHECK_TKY, &CRpkeyDlg::OnBnClickedCheckTky)
	ON_BN_CLICKED(IDC_CHECK_MNONLY, &CRpkeyDlg::OnBnClickedCheckMnonly)
	ON_BN_CLICKED(IDC_CHECK_SNO, &CRpkeyDlg::OnBnClickedCheckSno)
	ON_BN_CLICKED(IDC_CHK_MENZEI, &CRpkeyDlg::OnBnClickedChkMenzei)
	ON_BN_CLICKED(IDC_CHK_KAZEI, &CRpkeyDlg::OnBnClickedChkKazei)
	ON_CBN_SELCHANGE(IDC_SCNMENWARI, &CRpkeyDlg::OnCbnSelchangeScnMenWari)
	ON_CBN_SELCHANGE(IDC_SCNMENWARI2, &CRpkeyDlg::OnCbnSelchangeScnMenWari2)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRpkeyDlg メッセージ ハンドラ

BOOL CRpkeyDlg::OnInitDialog() 
{
	// TODO: この位置に初期化の補足処理を追加してください
//	ControlInit(448,226,-1);

	ICSDialog::OnInitDialog();
	
	if( pZmSel == NULL ) {
		if( (pZmSel = new CZmselDB) == NULL ) {
			EndDialog(-1);
//			ICSExit( ERROR_MEMORY, "選択クラスオブジェクトを作成できません！" );
			return FALSE;
		}
	}

	// 摘要文字数
	char pbuf[128];
	sprintf_s( pbuf, sizeof pbuf, "摘要(%d)：", Voln1->tk_ln );
	GetDlgItem( IDC_ST_TKY )->SetWindowText( pbuf );

	// 選択初期化
	pZmSel->SetPara( pDBzm, this, pDBsy, callback );

	((CICSDBEDT*)GetDlgItem(IDC_SCNDEBT))->SetKanaTerm(TRUE, KanaSearchKeta(), ICSDBEDT_KTYPE_KANA );
	((CICSDBEDT*)GetDlgItem(IDC_SCNDEBT2))->SetKanaTerm(TRUE, KanaSearchKeta(), ICSDBEDT_KTYPE_KANA );
	((CICSDBEDT*)GetDlgItem(IDC_SCNCRED))->SetKanaTerm(TRUE, KanaSearchKeta(), ICSDBEDT_KTYPE_KANA );
	((CICSDBEDT*)GetDlgItem(IDC_SCNCRED2))->SetKanaTerm(TRUE, KanaSearchKeta(), ICSDBEDT_KTYPE_KANA );

	((CICSDBEDT*)GetDlgItem(IDC_SCNTKY))->ImeEndMode( TRUE );
	((CICSDBEDT*)GetDlgItem(IDC_SCNTKY2))->ImeEndMode( TRUE );

	((CICSDBEDT*)GetDlgItem(IDC_SCNSNUM1))->SetKana( TRUE );
	((CICSDBEDT*)GetDlgItem(IDC_SCNSNUM2))->SetKana( TRUE );


	//
	int nID[] = { IDC_SCN_INVNO, IDC_SCN_INVNO2, 0 };
	int dispID[] = { IDC_SCN_INVNODISP, IDC_SCN_INVNO2DISP, 0 };
	for( int n = 0; nID[n] != 0; n++ ) {
		CICSDBEDT* pDBedt;
		pDBedt = (CICSDBEDT*)GetDlgItem(nID[n]);
		pDBedt->ImeEndMode(TRUE);
		pDBedt->EnableDelete(0);
		pDBedt->EnableHome(1);
		pDBedt->SetKanaTerm(TRUE, INVKANA_MAX, ICSDBEDT_KTYPE_KANA);
	}

	// 変動事由
	set_hjiyuinz();
	// 消費税セット
	inpctlline_inz( RPINP_CTL );
	def_syselinz();
	set_syselinz();

	m_Hcbo.InitHusenCombo();
	m_Hcbo2.InitHusenCombo();
/*
	m_Hcbo.SetCurSel(3);
	m_Hcbo.EnableWindow(FALSE);
	m_Hcbo2.EnableWindow(FALSE);
*/
	keyline_dsp( &m_Rpline );

	::ZeroMemory( m_saveKana, sizeof m_saveKana );

	syzInvoice.SetDBZmSub( pDBzm );

	ICSDialog::OnInitDialogEX();

	kazeichkbox_move();

	if( bInvDisp ) {
		for( int n = 0; nID[n] != 0; n++ ) {
			CRect rect;
			GetDlgItem(nID[n])->GetWindowRect(&rect);
			ScreenToClient(rect);
			GetDlgItem(dispID[n])->SetWindowPos(&CWnd::wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
		}
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


void CRpkeyDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	ICSDialog::OnShowWindow(bShow, nStatus);
	
	if( m_PGSW == 0 && bShow == TRUE ) {
		m_PGSW = 1;

		// イメージスタティック位置調整
		CRect rect;
		GetDlgItem( getINP_CTL( RP_TKY1_PN )->IDC_X )->GetWindowRect( &rect );
		ScreenToClient( rect );
		m_image1.MoveWindow( rect.left, rect.top, rect.Width(), rect.Height() );

		GetDlgItem( getINP_CTL( RP_TKY2_PN )->IDC_X )->GetWindowRect( &rect );
		ScreenToClient( rect );
		m_image2.MoveWindow( rect.left, rect.top, rect.Width(), rect.Height() );

	//	m_image1.SetBackColor(TRUE);
	//	m_image1.SetImage(0,0,0,NULL);
	//	m_image2.SetBackColor(TRUE);
	//	m_image2.SetImage(0,0,0,NULL);
	}	
}


// 摘要イメージスタティックにイメージをセット
void CRpkeyDlg::SetImage( int pn, IMREC* imp )
{
	CWnd*	pWnd;

	if( pn == RP_TKY1_PN )	pWnd = &m_image1;
	else					pWnd = &m_image2;

	pWnd->SetWindowPos( NULL, 0,0,0,0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
	if( imp ) {
		((ImageStatic*)pWnd)->SetImage( imp->dotxs, imp->dotys, imp->img_l, imp->imdata );
	}
	else {
		((ImageStatic*)pWnd)->SetImage( 0, 0, 0, NULL );
	}
}

// 摘要イメージスタティックを非表示
void CRpkeyDlg::OffImage( int pn )
{
	CWnd*	pWnd;
	if( pn == RP_TKY1_PN )	pWnd = &m_image1;
	else					pWnd = &m_image2;

	pWnd->SetWindowPos( NULL, 0,0,0,0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
}

//イメージスタティックにフォーカスをセット
void CRpkeyDlg::SetFocusToImage( int pn )
{
	CWnd*	pWnd;
	if( pn == RP_TKY1_PN )	pWnd = &m_image1;
	else					pWnd = &m_image2;

//	GotoDlgCtrl( pWnd );
	pWnd->SetFocus();
}


//イメージスタティックからのキー処理
LRESULT CRpkeyDlg::OnImageStaticKey( WPARAM wParam, LPARAM lParam )
{
	UINT	id	=	wParam;
	UINT	nChar	=	lParam;

	int		pn;
	pn = (id == IDC_STATIC_TKY1) ? RP_TKY1_PN : RP_TKY2_PN;

	switch( nChar ) {
	case VK_ESCAPE:
		EndDialog(IDCANCEL);
		break;
	case VK_F11:
		PostMessage(WM_COMMAND, MAKELONG(IDC_BTN_DEL, BN_CLICKED), 
								(LPARAM)GetDlgItem(IDC_BTN_DEL)->m_hWnd);
		break;
	case VK_F4:
	//	PostMessage(WM_COMMAND, MAKELONG(IDOK, BN_CLICKED), 
	//							(LPARAM)GetDlgItem(IDOK)->m_hWnd);

		// ﾌｫｰｶｽをﾎﾞﾀﾝに移して入力直後でOCXのﾃﾞｰﾀﾁｪｯｸ
		GetDlgItem(IDOK)->PostMessage(WM_LBUTTONDOWN,0,0);
		GetDlgItem(IDOK)->PostMessage(WM_LBUTTONUP,0,0);
		break;
	case VK_DELETE:
		// イメージ取り消し
		m_Rpline.LINE_CND[ pn ].IMG_sg = FALSE;
		m_Rpline._RPDTA[ pn].IMG_SQ = -1;
		OffImage( pn );
		// 摘要エディットにフォーカスをセット
		set_focus( pn );
		break;

	case VK_TAB:
		if( ! is_SHIFT() ) {
			// 入力ずみ 摘要ダイアログを表示
			if( DispTekiyoImage( pn ) != IDOK )
				break;
			// IDOK のときも ↓
		}
		else {
			nChar = VK_F2;
		}
		// else は ↓
	default:
		term_focus( pn, nChar );
		break;
	}

	return 1;
}

//イメージスタティックのフォーカス処理
LRESULT CRpkeyDlg::OnImageStaticFocus( WPARAM wParam, LPARAM lParam )
{
	return 1;
}


BOOL CRpkeyDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	set_nonICSpn( pMsg );

	int pn = get_nowpn();
	int posflg = -1;

	switch( pn ) {
	case RP_DEBT1_PN:
	case RP_DEBT2_PN:
	case RP_CRED1_PN:
	case RP_CRED2_PN:
		posflg = 0;
		break;
	case RP_INVNO_PN:
	case RP_INVNO2_PN:
		posflg = 1;
		break;
	}

	if( pMsg->message == WM_KEYDOWN && m_PGSW ) {

		if( pMsg->wParam == VK_ESCAPE ) {
			EndDialog(IDCANCEL);
			return TRUE;
		}

		if( posflg != -1 ) {
			if( KanaCheckVK_BACK(pMsg, posflg) ) {
				if( posflg == 0 ) {
				return TRUE;
		}
			}
		}

		if( pMsg->wParam == VK_F11 ) {
			PostMessage(WM_COMMAND, MAKELONG(IDC_BTN_DEL, BN_CLICKED), 
									(LPARAM)GetDlgItem(IDC_BTN_DEL)->m_hWnd);
			return TRUE;
		}
#ifdef BTNCHKKEEP // 04.23 /01
		else if( pMsg->wParam == VK_F8 && m_bMODIFY ) {
#else
		else if( pMsg->wParam == VK_F4 ) {
#endif
		//	PostMessage(WM_COMMAND, MAKELONG(IDOK, BN_CLICKED), 
		//							(LPARAM)GetDlgItem(IDOK)->m_hWnd);

			// ﾌｫｰｶｽをﾎﾞﾀﾝに移して入力直後でOCXのﾃﾞｰﾀﾁｪｯｸ
			GetDlgItem(IDOK)->PostMessage(WM_LBUTTONDOWN,0,0);
			GetDlgItem(IDOK)->PostMessage(WM_LBUTTONUP,0,0);
			return TRUE;			
		}
		else if( pMsg->wParam == VK_F9 ) {
			PostMessage(WM_COMMAND, MAKELONG(IDC_SWKDEL, BN_CLICKED), 
									(LPARAM)GetDlgItem(IDC_SWKDEL)->m_hWnd);
			return TRUE;
		}

		if( GetDlgItem( getINP_CTL(RP_SYZSW1_PN)->IDC_X )->m_hWnd == pMsg->hwnd ){
			if( ! OnTerminationRpSyzsw(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_SYZSW2_PN)->IDC_X )->m_hWnd == pMsg->hwnd ){
			if( ! OnTerminationRpSyzsw2(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_ZRITU1_PN)->IDC_X )->m_hWnd == pMsg->hwnd ){
			if( ! OnTerminationRpZritu(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_ZRITU2_PN)->IDC_X )->m_hWnd == pMsg->hwnd ){
			if( ! OnTerminationRpZritu2(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_SKBN1_PN)->IDC_X )->m_hWnd == pMsg->hwnd ){
			if( ! OnTerminationRpSkbn(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_SKBN2_PN)->IDC_X )->m_hWnd == pMsg->hwnd ){
			if( ! OnTerminationRpSkbn2(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_URISIRE1_PN)->IDC_X )->m_hWnd == pMsg->hwnd ){
			if( ! OnTerminationRpUrisire(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_URISIRE2_PN)->IDC_X )->m_hWnd == pMsg->hwnd ){
			if( ! OnTerminationRpUrisire2(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_TOKUS1_PN)->IDC_X )->m_hWnd == pMsg->hwnd ){
			if( ! OnTerminationRpTokus(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_TOKUS2_PN)->IDC_X )->m_hWnd == pMsg->hwnd ){
			if( ! OnTerminationRpTokus2(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_HJIYU1_PN)->IDC_X )->m_hWnd == pMsg->hwnd ){
			if( ! OnTerminationRpHjiyu(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_HJIYU2_PN)->IDC_X )->m_hWnd == pMsg->hwnd ){
			if( ! OnTerminationRpHjiyu2(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_NODENP_PN)->IDC_X )->m_hWnd == pMsg->hwnd )
		{
			if( !OnTerminationNoDenp(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_NOBMON_PN)->IDC_X )->m_hWnd == pMsg->hwnd )
		{
			if( !OnTerminationNoBmn(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_NOCBMN_PN)->IDC_X )->m_hWnd == pMsg->hwnd )
		{
			if( !OnTerminationNoCreBmn(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_NOKOJI_PN)->IDC_X )->m_hWnd == pMsg->hwnd )
		{
			if( !OnTerminationNoKoji(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_NOCKJI_PN)->IDC_X )->m_hWnd == pMsg->hwnd )
		{
			if( !OnTerminationNoCreKoji(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_NOTKY_PN)->IDC_X )->m_hWnd == pMsg->hwnd )
		{
			if( !OnTerminationNoTky(pMsg->wParam) )		//摘要取消
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_CONFIRM_PN)->IDC_X )->m_hWnd == pMsg->hwnd )
		{
			if( !OnTerminationConfirm(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_CONMDFY_PN)->IDC_X )->m_hWnd == pMsg->hwnd )
		{
			if( !OnTerminationConMdfy(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_HSEN1_PN)->IDC_X )->m_hWnd == pMsg->hwnd )
		{
			if( !OnTerminationScnhusen(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_HSEN2_PN)->IDC_X )->m_hWnd == pMsg->hwnd )
		{
			if( !OnTerminationScnhusen2(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( getINP_CTL(RP_HSENCHK_PN)->IDC_X )->m_hWnd == pMsg->hwnd )
		{
			if( !OnTerminationChkHusen(pMsg->wParam) )
				return TRUE;
		}
		//証憑番号	[11.26 /12]
		else if( GetDlgItem( getINP_CTL(RP_NODOCEVI_PN)->IDC_X )->m_hWnd == pMsg->hwnd )
		{
			if( !OnTerminationNoDocEvi(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( IDC_CHECK_MNONLY )->m_hWnd == pMsg->hwnd )
		{
			if( !OnTerminationChkMnonly(pMsg->wParam) )
				return TRUE;
		}
		else if( GetDlgItem( IDOK )->m_hWnd == pMsg->hwnd )
		{
			if( !OnTerminateOnOK(pMsg->wParam) )
				return TRUE;
		}
		// インボイス対応
		else if( GetDlgItem( IDC_CHK_MENZEI )->m_hWnd == pMsg->hwnd ) {
			// 免税事業者からの課税仕入れ
			if( !OnTerminationMenzei( pMsg->wParam )) {
				return TRUE;
			}
		}
		else if( GetDlgItem( IDC_CHK_KAZEI )->m_hWnd == pMsg->hwnd ) {
			// 課税事業者からの課税仕入れ
			if( !OnTerminationKazei( pMsg->wParam )) {
				return TRUE;
			}
		}
		else if( GetDlgItem(IDC_SCNMENWARI2)->m_hWnd == pMsg->hwnd ) {
			// 免税事業者控除割合
			if( !OnTerminationMenWari(pMsg->wParam) ) {
				return TRUE;
			}
		}
		else if( GetDlgItem( IDCANCEL )->m_hWnd == pMsg->hwnd || GetDlgItem( IDC_BTN_DEL )->m_hWnd == pMsg->hwnd ||
				 GetDlgItem( IDC_SWKDEL )->m_hWnd == pMsg->hwnd ) {
			if( pMsg->wParam == VK_TAB ) {
				if( is_SHIFT() ) {
					PrevDlgCtrl();
				}
				else {
					NextDlgCtrl();
				}
				return TRUE;
			}
			else if( pMsg->wParam == VK_F2 ) {
				PrevDlgCtrl();
				return TRUE;
			}
		}

		if( pMsg->wParam == VK_F8 ) {
			short s1, s2;
			s1 = GetKeyState( VK_SHIFT );
			s2 = GetKeyState( VK_CONTROL );

			if( (s1 & 0x80) && (s2 & 0x80) ) {
				mikaku_disp( 1 );
			}
		}
	}
	if( pMsg->message == WM_KEYUP && m_PGSW ) {
		if( posflg != -1 ) {
			FukuKanaSearch(pMsg, 0, posflg);
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}


// IDC get
struct _InpCtl *CRpkeyDlg::getINP_CTL( int pn )
{
	if( pn >= 0 && pn < RP_MAX_PN )
		return( &RPINP_CTL[pn] );
	else
		return( &RPINP_CTL[0] );	//ERROR
}

//　その他項目コントロールデータレコードをセット
void CRpkeyDlg::set_etcdsp( int pn )
{
//struct _ItemData data, *pdata;
int code_pn;

	if( pn == RP_DEBT1_PN || pn == RP_DEBT2_PN ||
		pn == RP_CRED1_PN || pn == RP_CRED2_PN  )
	{
		code_pn = pn;

	/*	
		data = *_getDATA( code_pn );	// 科目
		pdata = _getDATA( pn );
		data.IP_BRN = *((unsigned short*)pdata->IP_cBRN);	// 枝番
		_set_etcdsp( &data, &m_Rpline.LINE_CND[pn], FALSE );
		sprintf( pdata->ETC_TXT, "%s", data.ETC_TXT );
	*/
	}
	else if( pn == RP_BMON1_PN || pn == RP_BMON2_PN ) {	//#* 08.19 /02
/*		pdata = _getDATA( pn );
		_set_bmndsp( pdata, &m_Rpline.LINE_CND[pn] );
*/
	}
}


// その他項目　表示
void CRpkeyDlg::dsp_etc( int pn, BOOL dspsw )
{

#ifdef LATER_CLOSE /////////////////////////////////////

struct _ItemData *pdata;
int hide_idc1, hide_idc2;
BOOL dsp;

	switch( pn )
	{
	case RP_DBR1_PN:
	case RP_DBR2_PN:
	case RP_CBR1_PN:
	case RP_CBR2_PN:
	case RP_DCBR1_PN:
	case RP_DCBR2_PN:
		// 項目表示
		pdata = _getDATA( pn );
		dsp = _dsp_etc( this, pdata, &m_Rpline.LINE_CND[pn], scGetINP_CTL(pn), BRNTKY_X_RATE, dspsw, FALSE );

		// 背景のコントロールの表示／非表示
		switch( pn )
		{
		case RP_CBR1_PN:	hide_idc1 = IDC_STATIC8;
							hide_idc2 = 0;
							break;
		case RP_DCBR1_PN:	hide_idc1 = IDC_STATIC8;
							hide_idc2 = IDC_STATIC9;
							break;
		case RP_DBR1_PN:	hide_idc1 = IDC_CHECK_DBM;
							hide_idc2 = 0;
							break;

		case RP_CBR2_PN:	hide_idc1 = IDC_CHECK_DBR;
							hide_idc2 = 0;
							break;
		case RP_DCBR2_PN:	hide_idc1 = IDC_CHECK_CBR;
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

	case RP_BMON1_PN:	//#* 08.19 /02 
	case RP_BMON2_PN:
		// 項目表示
		pdata = _getDATA( pn );
		dsp = _dsp_etc( this, pdata, &m_Rpline.LINE_CND[pn], scGetINP_CTL(pn), BUMON_X_RATE, dspsw, FALSE );

		// 背景のコントロールの表示／非表示
		if( pn == RP_BMON1_PN )
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
#endif /////////////// LATER 

}


//
LRESULT CRpkeyDlg::RetroFocus( WPARAM wParam, LPARAM lParam ) 
{
#ifdef _ICSPEN_	// 01.20 /01
	_set_focus( wParam );
#else
	set_focus( wParam );
#endif
	return 1;
}

// カーソルセット
#ifdef _ICSPEN_	// 01.20 /01
void CRpkeyDlg::set_focus( int pn )
{
	PostMessage( WM_RETROFOCUS, pn );
}

void CRpkeyDlg::_set_focus( int pn )
{
#else
void CRpkeyDlg::set_focus( int pn )
{
#endif

	// インボイス対応
	if( pn <= RP_HSENCHK_PN )
		c_set( getINP_CTL( pn )->IDC_X );
	else if( pn == RP_IDOK )
	{
		GotoDlgCtrl( GetDlgItem( IDOK ) );
	}
	else if( pn == RP_IDCANCEL )
	{
		GotoDlgCtrl( GetDlgItem( IDCANCEL ) );
	}
	else if( pn == RP_NODENP_PN ) {
		GotoDlgCtrl( GetDlgItem( IDC_CHECK_DENP ) );
	}
	else if( pn == RP_NOBMON_PN ) {
		GotoDlgCtrl( GetDlgItem( IDC_CHECK_DBM ) );
	}
	else if( pn == RP_NOCBMN_PN ) {
		GotoDlgCtrl( GetDlgItem( IDC_CHECK_CBM ) );
	}
	else if( pn == RP_NOKOJI_PN ) {
		GotoDlgCtrl( GetDlgItem( IDC_CHECK_DKJ ) );
	}
	else if( pn == RP_NOCKJI_PN ) {
		GotoDlgCtrl( GetDlgItem( IDC_CHECK_CKJ ) );
	}
	else if( pn == RP_NOTKY_PN ) {
		GotoDlgCtrl( GetDlgItem( IDC_CHECK_TKY ) );
	}
	else if( pn == RP_CONFIRM_PN ) {	// 仕訳確定
		GotoDlgCtrl( GetDlgItem( IDC_CHK_CONFIRM ) );
	}
	else if( pn == RP_CONMDFY_PN ) {	// 確定仕訳 を修正可に
		GotoDlgCtrl( GetDlgItem( IDC_CHK_CONMDFY ) );
	}
	else if( pn == RP_MNONLY_PN ) {		// 月のみ修正
		GotoDlgCtrl( GetDlgItem( IDC_CHECK_MNONLY ) );
	}
	else if( pn == RP_NODOCEVI_PN ) {	// 証憑[11.26 /12]
		GotoDlgCtrl( GetDlgItem( IDC_CHECK_SNO ) );
	}
	else if( pn == RP_INVNO_PN ) {
		GotoDlgCtrl( GetDlgItem( IDC_SCN_INVNO ) );
	}
	else if( pn == RP_INVNO2_PN ) {
		GotoDlgCtrl( GetDlgItem( IDC_SCN_INVNO2 ) );
	}
	else if( pn == RP_MENZEI_PN ) {
		GotoDlgCtrl( GetDlgItem( IDC_CHK_MENZEI ) );
	}
	else if( pn == RP_KAZEI_PN ) {
		GotoDlgCtrl( GetDlgItem( IDC_CHK_KAZEI ) );
	}
	else if( pn == RP_MENWARI1_PN ) {
		GotoDlgCtrl(GetDlgItem(IDC_SCNMENWARI));
	}
	else if( pn == RP_MENWARI2_PN ) {
		GotoDlgCtrl(GetDlgItem(IDC_SCNMENWARI2));
	}

//	else if( pn == RP_HSEN2_PN ) {		// 付箋取消
//		GotoDlgCtrl( GetDlgItem( IDC_CHK_HUSEN ) );
//	}
}


/**************************************
		keyline_inz()
	入力ラインバッファイニシャライズ
***************************************/
void CRpkeyDlg::keyline_inz()
{
	int i;

	memset( m_MNTHbcd1, '\xff', sizeof m_MNTHbcd1 );
	memset( m_MNTHbcd2, '\xff', sizeof m_MNTHbcd2 );
	memset( m_MMDDbcd1, '\xff', sizeof m_MMDDbcd1 );
	memset( m_MMDDbcd2, '\xff', sizeof m_MMDDbcd2 );

	for( i = RP_DATE1_PN ; i < RP_MAX_PN; ++i )
	{
		switch( i )
		{
		case RP_DATE1_PN:
			m_Rpline.RP_DATE1_DT.IP_DAY = m_Rpline.RP_DATE2_DT.IP_DAY = 0;
			memset( (char *)&m_Rpline.RP_DATE1_DT.IP_BCDDAY, (char)0, sizeof(m_Rpline.RP_DATE1_DT.IP_BCDDAY) );
			memset( (char *)&m_Rpline.RP_DATE2_DT.IP_BCDDAY, (char)0, sizeof(m_Rpline.RP_DATE2_DT.IP_BCDDAY) );
			break;
		case RP_DENP1_PN:
			m_Rpline.RP_DENP1_DT.IP_DENP = m_Rpline.RP_DENP2_DT.IP_DENP = -1;
			break;
		case RP_BMON1_PN:
			m_Rpline.RP_BMON1_DT.IP_BMN = m_Rpline.RP_BMON2_DT.IP_BMN = -1;
			break;
		case RP_CBMN1_PN:
			m_Rpline.RP_CBMN1_DT.IP_BMN = m_Rpline.RP_CBMN2_DT.IP_BMN = -1;
			break;
		case RP_KOJI1_PN:
			::ZeroMemory( &m_Rpline.RP_KOJI1_DT.IP_KOJI, sizeof m_Rpline.RP_KOJI1_DT.IP_KOJI );
			::ZeroMemory( &m_Rpline.RP_KOJI2_DT.IP_KOJI, sizeof m_Rpline.RP_KOJI2_DT.IP_KOJI );
			break;
		case RP_CKJI1_PN:
			::ZeroMemory( &m_Rpline.RP_CKJI1_DT.IP_KOJI, sizeof m_Rpline.RP_CKJI1_DT.IP_KOJI );
			::ZeroMemory( &m_Rpline.RP_CKJI2_DT.IP_KOJI, sizeof m_Rpline.RP_CKJI2_DT.IP_KOJI );
			break;

		case RP_DEBT1_PN:
			memset( &m_Rpline.RP_DEBT1_DT, '\0', sizeof m_Rpline.RP_DEBT1_DT );
			m_Rpline.RP_DEBT1_DT.IP_BRN = -1;
			break;
		case RP_DEBT2_PN:
			memset( &m_Rpline.RP_DEBT2_DT, '\0', sizeof m_Rpline.RP_DEBT2_DT );
			m_Rpline.RP_DEBT2_DT.IP_BRN = -1;
			break;
		case RP_CRED1_PN:
			memset( &m_Rpline.RP_CRED1_DT, '\0', sizeof m_Rpline.RP_CRED1_DT );
			m_Rpline.RP_CRED1_DT.IP_BRN = -1;
			break;
		case RP_CRED2_PN:
			memset( &m_Rpline.RP_CRED2_DT, '\0', sizeof m_Rpline.RP_CRED2_DT );
			m_Rpline.RP_CRED2_DT.IP_BRN = -1;
			break;

		case RP_VAL1_PN:
			memset( (char *)&m_Rpline.RP_VAL1_DT, (char)0, sizeof(m_Rpline.RP_VAL1_DT) );
			memset( (char *)&m_Rpline.RP_VAL2_DT, (char)0, sizeof(m_Rpline.RP_VAL1_DT) );
			break;
		case RP_SYZSW1_PN:
			memset( (char *)&m_Rpline.RP_SYZSW1_DT._udta._SYDTA, 0, sizeof( m_Rpline.RP_SYZSW1_DT._udta._SYDTA ) );
			memset( (char *)&m_Rpline.RP_SYZSW2_DT._udta._SYDTA, 0, sizeof( m_Rpline.RP_SYZSW2_DT._udta._SYDTA ) );
			break;
		case RP_ZRITU1_PN:
			memset( (char *)&m_Rpline.RP_ZRITU1_DT._udta._SYDTA, 0, sizeof( m_Rpline.RP_ZRITU1_DT._udta._SYDTA ) );
			memset( (char *)&m_Rpline.RP_ZRITU2_DT._udta._SYDTA, 0, sizeof( m_Rpline.RP_ZRITU2_DT._udta._SYDTA ) );
			break;
		case RP_SKBN1_PN:
			memset( (char *)&m_Rpline.RP_SKBN1_DT._udta._SYDTA, 0, sizeof( m_Rpline.RP_SKBN1_DT._udta._SYDTA ) );
			memset( (char *)&m_Rpline.RP_SKBN2_DT._udta._SYDTA, 0, sizeof( m_Rpline.RP_SKBN2_DT._udta._SYDTA ) );
			break;
		case RP_URISIRE1_PN:
			memset( (char *)&m_Rpline.RP_URISIRE1_DT._udta._SYDTA, 0, sizeof( m_Rpline.RP_URISIRE1_DT._udta._SYDTA ) );
			memset( (char *)&m_Rpline.RP_URISIRE2_DT._udta._SYDTA, 0, sizeof( m_Rpline.RP_URISIRE2_DT._udta._SYDTA ) );
			break;
		//特定収入
		case RP_TOKUS1_PN:
			memset( (char *)&m_Rpline.RP_TOKUS1_DT._udta._SYDTA, 0, sizeof( m_Rpline.RP_TOKUS1_DT._udta._SYDTA ) );
			memset( (char *)&m_Rpline.RP_TOKUS2_DT._udta._SYDTA, 0, sizeof( m_Rpline.RP_TOKUS2_DT._udta._SYDTA ) );
			break;

		case RP_TKY1_PN:
			m_Rpline.RP_TKY1_DT.IP_TKY[0] = '\0';
			m_Rpline.RP_TKY1_DT.IMG_SQ = 0;
			m_Rpline.RP_TKY2_DT.IP_TKY[0] = '\0';
			m_Rpline.RP_TKY2_DT.IMG_SQ = 0;
			break;

		case RP_HJIYU1_PN:
			memset( (char *)&m_Rpline.RP_HJIYU1_DT._udta._SYDTA, 0, sizeof( m_Rpline.RP_HJIYU1_DT._udta._SYDTA ) );
			memset( (char *)&m_Rpline.RP_HJIYU2_DT._udta._SYDTA, 0, sizeof( m_Rpline.RP_HJIYU2_DT._udta._SYDTA ) );
			break;

		case RP_HSEN1_PN:
			memset( (char *)&m_Rpline.RP_HSEN1_DT, 0, sizeof( m_Rpline.RP_HSEN1_DT) );
			memset( (char *)&m_Rpline.RP_HSEN2_DT, 0, sizeof( m_Rpline.RP_HSEN2_DT) );
			break;

		case RP_SNUM1_PN:
			memset( (char *)&m_Rpline.RP_SNUM1_DT.IP_SNUM, (char)0, sizeof(m_Rpline.RP_SNUM1_DT.IP_SNUM) );
			memset( (char *)&m_Rpline.RP_SNUM2_DT.IP_SNUM, (char)0, sizeof(m_Rpline.RP_SNUM2_DT.IP_SNUM) );
			break;

		case RP_TDAT1_PN:
			m_Rpline.RP_TDAT1_DT.IP_TGDAY = m_Rpline.RP_TDAT2_DT.IP_TGDAY = 0;
			memset( (char *)&m_Rpline.RP_TDAT1_DT.IP_TGBCD, (char)0, sizeof(m_Rpline.RP_TDAT1_DT.IP_TGBCD) );
			memset( (char *)&m_Rpline.RP_TDAT2_DT.IP_TGBCD, (char)0, sizeof(m_Rpline.RP_TDAT2_DT.IP_TGBCD) );
			break;
		//証憑番号	[11.26 /12]
		case RP_DOCEVI1_PN:
			memset( (char *)&m_Rpline.RP_DOCEVI1_DT.IP_SNUM, (char)0, sizeof(m_Rpline.RP_DOCEVI1_DT.IP_SNUM) );
			memset( (char *)&m_Rpline.RP_DOCEVI2_DT.IP_SNUM, (char)0, sizeof(m_Rpline.RP_DOCEVI2_DT.IP_SNUM) );
			break;

			// インボイス対応
		case RP_INVNO_PN:	// 登録番号
			memset(( char* )&m_Rpline.RP_INVNO_DT, '\0', sizeof( m_Rpline.RP_INVNO_DT ));
			break;
		case RP_INVNO2_PN:	// 登録番号
			memset((char* )&m_Rpline.RP_INVNO2_DT, '\0', sizeof( m_Rpline.RP_INVNO2_DT ));
			break;

		case RP_MENZEI_PN:	// 免税事業者からの課税仕入れ
			memset((char* )&m_Rpline.RP_MENZEI_DT, '\0', sizeof( m_Rpline.RP_MENZEI_DT ));
			break;


		default:
			break;
		}
		//コンディションサイン OFF
		memset( (char *)&m_Rpline.LINE_CND[i], (char)0, sizeof(m_Rpline.LINE_CND[i]) );
	}

//	keyline_dsp( &m_Rpline );
//	set_nowpn( RP_SEQ1_PN );	// 選択項目背景色消去のため

}


//データをセット
void CRpkeyDlg::keyline_dsp( struct _RPL_LINE *dtline )
{
	int i, sel;
	VARIANT var;
	char	buf[128], tmp[64];
	CString str;
	struct _InpCtl *ctl = &RPINP_CTL[0];

	// なしのチェックボックス
	if( dtline->LINE_CND[RP_DENP2_PN].INP_type )
		m_NoDenp = 1;
	else
		m_NoDenp = 0;

	if( dtline->LINE_CND[RP_BMON2_PN].INP_type )
		m_NoDbmn = 1;
	else
		m_NoDbmn = 0;
		
	if( dtline->LINE_CND[RP_CBMN2_PN].INP_type )
		m_NoCbmn = 1;
	else
		m_NoCbmn = 0;

	if( dtline->LINE_CND[RP_KOJI2_PN].INP_type )
		m_NoDkoji = 1;
	else
		m_NoDkoji = 0;
		
	if( dtline->LINE_CND[RP_CKJI2_PN].INP_type )
		m_NoCkoji = 1;
	else
		m_NoCkoji = 0;
	//摘要取消
	if( dtline->LINE_CND[RP_TKY2_PN].INP_type )
		m_NoTky = 1;
	else
		m_NoTky = 0;

	// 仕訳確定チェックボックス
	if( dtline->LINE_CND[RP_CONFIRM_PN].INP_type )
		m_Confirm = 1;
	else
		m_Confirm = 0;
	// 確定仕訳 を修正可能に
	if( dtline->LINE_CND[RP_CONMDFY_PN].INP_type )
		m_ConMdfy = 1;
	else
		m_ConMdfy = 0;

	// 付箋取消
	if( dtline->LINE_CND[RP_HSEN2_PN].INP_type )
		m_ChkHusen = TRUE;
	else
		m_ChkHusen = FALSE;

	// 月のみ修正
	if( dtline->LINE_CND[ RP_DATE2_PN ].INP_type )
		m_ChkMnonly = TRUE;
	else
		m_ChkMnonly = FALSE;

	// 免税事業者からの課税仕入れ
	if( dtline->LINE_CND[RP_MENZEI_PN].INP_type ) {
		m_MenzeiChk = TRUE;
	}
	else {
		m_MenzeiChk = FALSE;
	}
	// 課税事業者からの課税仕入
	if( dtline->LINE_CND[RP_KAZEI_PN].INP_type ) {
		m_KazeiChk = TRUE;
	}
	else {
		m_KazeiChk = FALSE;
	}

	UpdateData(FALSE);
	// 伝票番号２の Enable/Disableを ﾎﾞﾀﾝｸﾘｯｸの関数で処理
	PostMessage(WM_COMMAND, MAKELONG(IDC_CHECK_DENP, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_DENP)->m_hWnd );
	// 部門２の Enable/Disableを ﾎﾞﾀﾝｸﾘｯｸの関数で処理	
	if( GetDlgItem(IDC_CHECK_DBM)->IsWindowEnabled() ) {
		PostMessage(WM_COMMAND, MAKELONG(IDC_CHECK_DBM, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_DBM)->m_hWnd );
	}
	if( GetDlgItem(IDC_CHECK_CBM)->IsWindowEnabled() ) {
		PostMessage(WM_COMMAND, MAKELONG(IDC_CHECK_CBM, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_CBM)->m_hWnd );
	}
	if( GetDlgItem(IDC_CHECK_DKJ)->IsWindowEnabled() ) {
		PostMessage(WM_COMMAND, MAKELONG(IDC_CHECK_DKJ, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_DKJ)->m_hWnd );
	}
	if( GetDlgItem(IDC_CHECK_CKJ)->IsWindowEnabled() ) {
		PostMessage(WM_COMMAND, MAKELONG(IDC_CHECK_CKJ, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_CKJ)->m_hWnd );
	}

	PostMessage(WM_COMMAND, MAKELONG(IDC_CHK_HUSEN, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHK_HUSEN)->m_hWnd );
	//摘要取消
	PostMessage(WM_COMMAND, MAKELONG(IDC_CHECK_TKY, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_TKY)->m_hWnd );
	//月のみ修正
	PostMessage(WM_COMMAND, MAKELONG(IDC_CHECK_MNONLY, BN_CLICKED), (LPARAM)GetDlgItem(IDC_CHECK_MNONLY)->m_hWnd );

	// インボイス対応
	for( i = RP_DATE1_PN ; i <= RP_HSEN2_PN ; ++i )
	{
	//	if( i > RP_CBMN2_PN ) break;

		var.pbVal = NULL;
		switch( i )
		{
#ifdef CLOSE // 03.15 /11
		case RP_DATE1_PN:
			::ZeroMemory( bcd, sizeof bcd );
			memcpy( bcd, dtline->RP_DATE1_DT.IP_BCDDAY, sizeof dtline->RP_DATE1_DT.IP_BCDDAY );
			var.pbVal = bcd;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
			pDBzm->BcdDateToString( str, dtline->RP_DATE1_DT.IP_BCDDAY );
			DBSetDispString( this, (ctl+i)->IDC_X, str );
			break;
		case RP_DATE2_PN:
			::ZeroMemory( bcd, sizeof bcd );
			memcpy( bcd, dtline->RP_DATE2_DT.IP_BCDDAY, sizeof dtline->RP_DATE2_DT.IP_BCDDAY );
			var.pbVal = bcd;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
			pDBzm->BcdDateToString( str, dtline->RP_DATE2_DT.IP_BCDDAY );
			DBSetDispString( this, (ctl+i)->IDC_X, str );
			break;
#endif
		case RP_DENP1_PN:
			if( dtline->RP_DENP1_DT.IP_DENP != -1 ) {
				sprintf_s( buf, sizeof buf, "%d", dtline->RP_DENP1_DT.IP_DENP );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case RP_DENP2_PN:
			if( dtline->RP_DENP2_DT.IP_DENP != -1 ) {
				sprintf_s( buf, sizeof buf, "%d", dtline->RP_DENP2_DT.IP_DENP );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case RP_BMON1_PN:
			if( dtline->RP_BMON1_DT.IP_BMN != -1 ) {
				pDBzm->BumonCodeToStr( buf, sizeof buf, dtline->RP_BMON1_DT.IP_BMN );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case RP_BMON2_PN:
			if( dtline->RP_BMON2_DT.IP_BMN != -1 ) {
				pDBzm->BumonCodeToStr( buf, sizeof buf, dtline->RP_BMON2_DT.IP_BMN );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case RP_CBMN1_PN:
			if( dtline->RP_CBMN1_DT.IP_BMN != -1 ) {
				pDBzm->BumonCodeToStr( buf, sizeof buf, dtline->RP_CBMN1_DT.IP_BMN );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case RP_CBMN2_PN:
			if( dtline->RP_CBMN2_DT.IP_BMN != -1 ) {
				pDBzm->BumonCodeToStr( buf, sizeof buf, dtline->RP_CBMN2_DT.IP_BMN );
				var.pbVal = (BYTE*)buf;
			}
			else	var.pbVal = NULL;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case RP_KOJI1_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->RP_KOJI1_DT.IP_KOJI );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case RP_KOJI2_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->RP_KOJI2_DT.IP_KOJI );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case RP_CKJI1_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->RP_CKJI1_DT.IP_KOJI );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case RP_CKJI2_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->RP_CKJI2_DT.IP_KOJI );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;

		case RP_DEBT1_PN:
			if( dtline->LINE_CND[i].INP_sg )	{
				//科目名称再表示
				knam_set( &dtline->RP_DEBT1_DT );
			}
			DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->RP_DEBT1_DT.IP_stKMK, pDBzm );
			break;

		case RP_DEBT2_PN:
			if( dtline->LINE_CND[i].INP_sg ) {
				//科目名称再表示
				knam_set( &dtline->RP_DEBT2_DT );
			}
			DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->RP_DEBT2_DT.IP_stKMK, pDBzm );
			break;

		case RP_CRED1_PN:
			if( dtline->LINE_CND[i].INP_sg ) {
				//科目名称再表示
				knam_set( &dtline->RP_CRED1_DT );
			}
			DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->RP_CRED1_DT.IP_stKMK, pDBzm );
			break;

		case RP_CRED2_PN:
			if( dtline->LINE_CND[i].INP_sg ) {
				//科目名称再表示
				knam_set( &dtline->RP_CRED2_DT );
			}
			DBkamokudata_set( this, (ctl+i)->IDC_X, &dtline->RP_CRED2_DT.IP_stKMK, pDBzm );
			break;

		case RP_VAL1_PN:
			if( dtline->LINE_CND[ i ].INP_sg == TRUE )
			{
				var.pbVal = dtline->RP_VAL1_DT.IP_VAL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}
			else
			{
				var.pbVal = NULL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}
			break;
		case RP_VAL2_PN:
			if( dtline->LINE_CND[ i ].INP_sg == TRUE )
			{
				var.pbVal = dtline->RP_VAL2_DT.IP_VAL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}
			else
			{
				var.pbVal = NULL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			}
			break;

		case RP_SYZSW1_PN:
			sel = dtline->RP_SYZSW1_DT.SKBN_SGN2;
			((CComboBox*)GetDlgItem(IDC_SCNSYZSW))->SetCurSel( sel );
			break;
		case RP_SYZSW2_PN:
			sel = dtline->RP_SYZSW2_DT.SKBN_SGN2;
			((CComboBox*)GetDlgItem(IDC_SCNSYZSW2))->SetCurSel( sel );
			break;
		case RP_ZRITU1_PN:
			sel = dtline->RP_ZRITU1_DT.SKBN_SGN2;
			((CComboBox*)GetDlgItem(IDC_SCNZRITU))->SetCurSel( sel );
			break;
		case RP_ZRITU2_PN:
			sel = dtline->RP_ZRITU2_DT.SKBN_SGN2;
			((CComboBox*)GetDlgItem(IDC_SCNZRITU2))->SetCurSel( sel );
			break;
		case RP_SKBN1_PN:
			sel = dtline->RP_SKBN1_DT.SKBN_SGN2;
			((CComboBox*)GetDlgItem(IDC_SCNSKBN))->SetCurSel( sel );
			break;
		case RP_SKBN2_PN:
			sel = dtline->RP_SKBN2_DT.SKBN_SGN2;
			((CComboBox*)GetDlgItem(IDC_SCNSKBN2))->SetCurSel( sel );
			break;
		case RP_URISIRE1_PN:
			sel = dtline->RP_URISIRE1_DT.SKBN_SGN2;
			((CComboBox*)GetDlgItem(IDC_SCNURISIRE))->SetCurSel( sel );
			break;
		case RP_URISIRE2_PN:
			sel = dtline->RP_URISIRE2_DT.SKBN_SGN2;
			((CComboBox*)GetDlgItem(IDC_SCNURISIRE2))->SetCurSel( sel );
			break;

		case RP_TOKUS1_PN:
			sel = dtline->RP_TOKUS1_DT.SKBN_SGN2;
			((CComboBox*)GetDlgItem(IDC_SCNTOKUS))->SetCurSel( sel );
			break;
		case RP_TOKUS2_PN:
			sel = dtline->RP_TOKUS2_DT.SKBN_SGN2;
			((CComboBox*)GetDlgItem(IDC_SCNTOKUS2))->SetCurSel( sel );
			break;

		case RP_TKY1_PN:
			// イメージ摘要
			if( dtline->LINE_CND[i].IMG_sg ) {
				pDBzm->image->SetAbsolutePosition( dtline->RP_TKY1_DT.IMG_SQ );
				IMREC* pi;
				pi = pDBzm->image->DecodeImage();

				SetImage( i, pi );

				var.pbVal = NULL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			else {
				OffImage( i );
				strcpy_s( buf, sizeof buf, (char*)dtline->RP_TKY1_DT.IP_TKY );
				var.pbVal = (BYTE*)buf;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			break;
		case RP_TKY2_PN:
			// イメージ摘要
			if( dtline->LINE_CND[i].IMG_sg ) {
				pDBzm->image->SetAbsolutePosition( dtline->RP_TKY2_DT.IMG_SQ );
				IMREC* pi;
				pi = pDBzm->image->DecodeImage();

				SetImage( i, pi );

				var.pbVal = NULL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			else {
				OffImage( i );
				strcpy_s( buf, sizeof buf, (char*)dtline->RP_TKY2_DT.IP_TKY );
				var.pbVal = (BYTE*)buf;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			break;

		case RP_HJIYU1_PN:
			sel = dtline->RP_HJIYU1_DT.SKBN_SGN2;
			((CComboBox*)GetDlgItem(IDC_SCNHJIYU))->SetCurSel( sel );
			break;
		case RP_HJIYU2_PN:
			sel = dtline->RP_HJIYU2_DT.SKBN_SGN2;
			((CComboBox*)GetDlgItem(IDC_SCNHJIYU2))->SetCurSel( sel );
			break;

		case RP_SNUM1_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->RP_SNUM1_DT.IP_SNUM );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case RP_SNUM2_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->RP_SNUM2_DT.IP_SNUM );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case RP_TDAT1_PN:
			//データセット
			IntsrekitoGenstr( buf, sizeof buf, dtline->RP_TDAT1_DT.IP_TGDAY );

			if( buf[0] ) {
				sprintf_s( tmp, sizeof tmp, "%08d", dtline->RP_TDAT1_DT.IP_TGDAY );
				var.pbVal = (BYTE*)tmp;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			else {
				var.pbVal = NULL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			DBSetDispString( this, (ctl+i)->IDC_X, buf );
			break;
		case RP_TDAT2_PN:
			//データセット
			IntsrekitoGenstr( buf, sizeof buf, dtline->RP_TDAT2_DT.IP_TGDAY );

			if( buf[0] ) {
				sprintf_s( tmp, sizeof tmp, "%08d", dtline->RP_TDAT2_DT.IP_TGDAY );
				var.pbVal = (BYTE*)tmp;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			else {
				var.pbVal = NULL;
				DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			DBSetDispString( this, (ctl+i)->IDC_X, buf );
			break;

		case RP_HSEN1_PN:
			sel = dtline->RP_HSEN1_DT.IP_HUSEN;
			((CComboBox*)GetDlgItem(IDC_SCNHUSEN))->SetCurSel( sel );
			break;
		case RP_HSEN2_PN:
			sel = dtline->RP_HSEN2_DT.IP_HUSEN;
			((CComboBox*)GetDlgItem(IDC_SCNHUSEN2))->SetCurSel( sel );
			break;

		//証憑番号	[11.26 /12]
		case RP_DOCEVI1_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->RP_DOCEVI1_DT.IP_SNUM );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;
		case RP_DOCEVI2_PN:
			strcpy_s( buf, sizeof buf, (char*)dtline->RP_DOCEVI2_DT.IP_SNUM );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, (ctl+i)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			break;

		// インボイス対応
		case RP_INVNO_PN:
			strcpy_s( buf, sizeof( buf ), dtline->RP_INVNO_DT.INVNO );
			var.pbVal = ( BYTE* )buf;
			DBdata_set( this, ( ctl + i )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			DBSetDispString( this, ( ctl + i )->IDC_X2, dtline->RP_INVNO_DT.INVNO );
			break;
		case RP_INVNO2_PN:
			strcpy_s( buf, sizeof( buf ), dtline->RP_INVNO2_DT.INVNO );
			var.pbVal = ( BYTE* )buf;
			DBdata_set( this, ( ctl + i )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			DBSetDispString( this, ( ctl + i )->IDC_X2, dtline->RP_INVNO2_DT.INVNO );
			break;

		case RP_MENWARI1_PN:
			sel = dtline->RP_MENWARI1_DT.SKBN_SGN2;
			((CComboBox*)GetDlgItem(IDC_SCNMENWARI))->SetCurSel(sel);
			break;
		case RP_MENWARI2_PN:
			sel = dtline->RP_MENWARI2_DT.SKBN_SGN2;
			((CComboBox*)GetDlgItem(IDC_SCNMENWARI2))->SetCurSel(sel);
			break;

		default:
			break;
		}
	}
	EnableMenWariCombo(m_MenzeiChk);

}

/*
void CRpkeyDlg::fnc_set( ctl )
{


}
*/

//入力コントロール，１行イニシャライズ
void CRpkeyDlg::inpctlline_inz( struct _InpCtl *ctl )
{
//	fnc_set( ctl );
	int bm_sgn, kj_sgn, kmklen, doc_sgn;
	char tmp[20];
	kmklen = KMKCD_LN + 1 + pDBzm->zvol->edcol;

	bm_sgn = kj_sgn = doc_sgn = 0;

	BOOL bEntry;
#ifdef _DEBUG
	bEntry = TRUE;
#else
	bEntry = GET_EntrySign();
#endif
	BOOL bWari = FALSE;

	for( int i = RP_DATE1_PN ; i < RP_MAX_PN; ++i )
	{
		// Delete キー処理
		if( i < RP_NODENP_PN ) {
			switch( i ) {
			case RP_SYZSW1_PN:
			case RP_SYZSW2_PN:
			case RP_ZRITU1_PN:
			case RP_ZRITU2_PN:
			case RP_SKBN1_PN:
			case RP_SKBN2_PN:
			case RP_URISIRE1_PN:
			case RP_URISIRE2_PN:
			case RP_TOKUS1_PN:	//特定収入
			case RP_TOKUS2_PN:
			case RP_HJIYU1_PN:
			case RP_HJIYU2_PN:
			case RP_HSEN1_PN:
			case RP_HSEN2_PN:
			case RP_HSENCHK_PN:
			case RP_MENZEI_PN:	// 免税事業者からの課税仕入れ
			case RP_KAZEI_PN:	// 課税事業者からの課税仕入れ
			case RP_MENWARI1_PN:
			case RP_MENWARI2_PN:
				break;
			default:
				// ICSDBEDT
				((CICSDBEDT*)GetDlgItem( (ctl+i)->IDC_X ))->EnableDelete(0);;
				break;
			}
		}

		switch( i ) {
		case RP_BMON1_PN:	// 部門
		case RP_BMON2_PN:
		case RP_CBMN1_PN:
		case RP_CBMN2_PN:
		case RP_NOBMON_PN:
		case RP_NOCBMN_PN:
			if( (Voln1->sub_sw&0x02) )
			{
				(ctl+i)->INP_ena = TRUE;
				if( i != RP_NOBMON_PN && i != RP_NOCBMN_PN ) {
					sprintf_s( tmp, sizeof tmp, "%d", pDBzm->GetBumonMaxDecimal() );
					((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->SetMinval( "0" );
					((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->SetMaxval( tmp );
					((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->SetMaxLen(Voln1->bmcol);//桁数設定(数字のときは 0)
					((CICSDBEDT*)GetDlgItem((ctl + i)->IDC_X))->ImeEndMode(TRUE);
				}
			}
			else
			{
				(ctl+i)->INP_ena = FALSE;
				if( i != RP_NOBMON_PN && i != RP_NOCBMN_PN ) {
					sprintf_s( tmp, sizeof tmp, "%d", pDBzm->GetBumonMaxDecimal() );
					((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->SetMinval( "0" );
					((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->SetMaxval( tmp );
					((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->SetMaxLen( 0 );	
					((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->EnableWindow( FALSE );
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
		case RP_KOJI1_PN:	// 工事
		case RP_KOJI2_PN:
		case RP_CKJI1_PN:
		case RP_CKJI2_PN:
		case RP_NOKOJI_PN:
		case RP_NOCKJI_PN:

			if( Voln1->apno == 0x20 && (Voln1->sub_sw&0x04)  )
			{
				(ctl+i)->INP_ena = TRUE;
				if( i != RP_NOKOJI_PN && i != RP_NOCKJI_PN ) {
					((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->SetMaxLen( Voln1->kjcol );
				//	sprintf_s( tmp, sizeof tmp, "%d", pDBzm->GetKojiMaxDecimal() );
				//	((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->SetMinval( "0" );
				//	((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->SetMaxval( tmp );
				}
			}
			else
			{
				(ctl+i)->INP_ena = FALSE;
				if( i != RP_NOKOJI_PN && i != RP_NOCKJI_PN ) {
					((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->SetMaxLen( Voln1->kjcol );
				//	sprintf_s( tmp, sizeof tmp, "%d", pDBzm->GetKojiMaxDecimal() );
				//	((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->SetMinval( "0" );
				//	((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->SetMaxval( tmp );
					((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->EnableWindow( FALSE );
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

		case RP_DEBT1_PN:
		case RP_DEBT2_PN:
		case RP_CRED1_PN:
		case RP_CRED2_PN:
			(ctl+i)->INP_ena = TRUE;
			((CICSDBEDT*)GetDlgItem( (ctl+i)->IDC_X ))->ImeEndMode( TRUE );
			((CICSDBEDT*)GetDlgItem( (ctl+i)->IDC_X ))->SetMaxLen( kmklen );
			break;		
		case RP_TKY1_PN:	// 摘要
		case RP_TKY2_PN:	// 摘要
			(ctl+i)->INP_ena = TRUE;
			((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ) )->SetMaxLen( (pDBzm->zvol->tk_ln*2) );	//文字数設定
			break;
		case RP_HJIYU1_PN:
		case RP_HJIYU2_PN:	// 変動事由は、セット済み
			break;
		case RP_CONFIRM_PN:
			if( ! (Voln1->tl_cor & 0x10) || IsJzSansyo() ) {	//確定マスター修正不可 or 参照モード
				(ctl+i)->INP_ena = FALSE;
				GetDlgItem((ctl+i)->IDC_X)->EnableWindow( FALSE );
			}
			else if( /*IsModifyConfirmData() ||*/ IsEnkakuUser() || bEntry || IsJozuEnvCloudMaster() )
			{	// 確定仕訳修正可能・遠隔ユーザ・上手君dbNL の場合、確定させない
				(ctl+i)->INP_ena = FALSE;
				GetDlgItem((ctl+i)->IDC_X)->EnableWindow( FALSE );

				if( bEntry ) {
					GetDlgItem(IDC_ST_CONFIRM)->ShowWindow( SW_HIDE);
					GetDlgItem((ctl+i)->IDC_X)->ShowWindow( SW_HIDE);
				}
			}
			else {
				(ctl+i)->INP_ena = TRUE;
				GetDlgItem((ctl+i)->IDC_X)->EnableWindow( TRUE );
			}
			break;
		case RP_CONMDFY_PN:
			if( ! (Voln1->tl_cor & 0x10) || bEntry || IsJzSansyo() ) {	//確定マスター修正不可 or 上手君dbNL or 参照モード
				(ctl+i)->INP_ena = FALSE;
				GetDlgItem((ctl+i)->IDC_X)->EnableWindow( FALSE );

				if( bEntry ) {
					GetDlgItem(IDC_ST_CONFIRM)->ShowWindow( SW_HIDE);
					GetDlgItem((ctl+i)->IDC_X)->ShowWindow( SW_HIDE);
				}
			}
			else if( ! IsModifyConfirmData() || IsEnkakuUser() ) {	// 確定仕訳修正不可・遠隔ユーザの場合、未確定にさせない。
				(ctl+i)->INP_ena = FALSE;
				GetDlgItem((ctl+i)->IDC_X)->EnableWindow( FALSE );
			}
			else {
				(ctl+i)->INP_ena = TRUE;
				GetDlgItem((ctl+i)->IDC_X)->EnableWindow( TRUE );
			}
			break;

		case RP_DOCEVI1_PN:	// 証憑番号[11.26 /12]
		case RP_DOCEVI2_PN:
		case RP_NODOCEVI_PN:

			if( bDOCEVI_Master )
			{
				(ctl+i)->INP_ena = TRUE;
				if( i != RP_NODOCEVI_PN ) {
					((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->SetMaxLen( Voln1->snocol );
				}
			}
			else
			{
				(ctl+i)->INP_ena = FALSE;
				if( i != RP_NODOCEVI_PN ) {
					((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->SetMaxLen( Voln1->snocol );
					((CICSDBEDT *)GetDlgItem( (ctl+i)->IDC_X ))->ShowWindow( SW_HIDE );
				}
				else {
					GetDlgItem((ctl+i)->IDC_X)->ShowWindow( SW_HIDE );
				}
				if( ! doc_sgn ) {
					doc_sgn = 1;
					GetDlgItem(IDC_ST_SNO)->ShowWindow( SW_HIDE );
					GetDlgItem(IDC_STATIC26)->ShowWindow( SW_HIDE );
				}
			}
			break;

		// インボイス対応
		case RP_INVNO_PN:	// 登録番号
		case RP_INVNO2_PN:
			if( !bInvMaster || !bInvUse || !bInvDisp ) {
				GetDlgItem(( ctl + i )->IDC_X )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_ST_DEBT_INVNO )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_STATIC32 )->ShowWindow( SW_HIDE );
				( ctl + i )->INP_ena = FALSE;

				GetDlgItem((ctl + i)->IDC_X2)->ShowWindow(SW_HIDE);
				GetDlgItem((ctl + i)->IDC_X2)->ShowWindow(SW_HIDE);
			}
			else {
				( ctl + i )->INP_ena = TRUE;
				CRect rect;
				GetDlgItem((ctl + i)->IDC_X)->GetWindowRect(rect);
				ScreenToClient(rect);
				GetDlgItem((ctl + i)->IDC_X2)->SetWindowPos(&CWnd::wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
			}
			break;

		case RP_MENZEI_PN:	// 免税事業者からの課税仕入れ
		case RP_KAZEI_PN:	// 課税事業者からの課税仕入れ
			if( !bInvMaster || !bInvUse ) {
				GetDlgItem(( ctl + i )->IDC_X )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_ST_CONFIRM2 )->ShowWindow( SW_HIDE );
				( ctl + i )->INP_ena = FALSE;
			}
			else {
				( ctl + i )->INP_ena = TRUE;
			}
			break;
		//免税事業者　控除割合
		case RP_MENWARI1_PN:
		case RP_MENWARI2_PN:
		{
			if( !bInvMaster || !bInvUse ) {
			}
			else {
				int mentype = GetMenzeiWariai(Voln1);

				if( mentype == SYZ_MEN_NOT || mentype == SYZ_MEN_80 ) {
				}
				else {
					bWari = TRUE;
				}
			}
			if( bWari ) {
				(ctl + i)->INP_ena = TRUE;
			}
			else {
				GetDlgItem((ctl + i)->IDC_X)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC25)->ShowWindow(SW_HIDE);
				(ctl + i)->INP_ena = FALSE;
			}
			break;
		}	

		default:
			(ctl+i)->INP_ena = TRUE;
			break;
		}
	}
}

//課税事業者のチェックボックス
void CRpkeyDlg::kazeichkbox_move()
{
	//控除割合は、変更後のみのため
	return;

	BOOL bWari = FALSE;

	if( !bInvMaster || !bInvUse ) {
	}
	else {
		int mentype = GetMenzeiWariai(Voln1);

		if( mentype == SYZ_MEN_NOT || mentype == SYZ_MEN_80 ) {
		}
		else {
			bWari = TRUE;
		}
	}
	struct _InpCtl* ctl;
	ctl = RPINP_CTL;

	if( bWari ) {
	}
	else {
		int mentype = GetMenzeiWariai(Voln1);

		if( mentype == SYZ_MEN_NOT || mentype == SYZ_MEN_80 ) {

			int nID = (ctl + RP_KAZEI_PN)->IDC_X;

			CRect rect, rc2;
			GetDlgItem(nID)->GetWindowRect(rect);
			ScreenToClient(&rect);

			int baseID = (ctl + RP_MENZEI_PN)->IDC_X;
			GetDlgItem(baseID)->GetWindowRect(rc2);
			ScreenToClient(&rc2);

			int ht = rc2.Height();
			GetDlgItem(nID)->MoveWindow(rect.left, rc2.bottom + (ht / 3), rect.Width(), rect.Height());
		}
	}
}


// 変動・振替科目[公益]コンボボックスイニシャライズ
void CRpkeyDlg::set_hjiyuinz()
{
	int cnt, i, index;
	char buf[128];
	BOOL bHjiyu = FALSE;

	bHjiyu = (pDBzm->zvol->apno < 0x50) ? TRUE : FALSE;

	sprintf_s( buf, sizeof buf, "%s：", pDBzm->get_hjiyutitle(2) );
	GetDlgItem( IDC_ST_HJIYU )->SetWindowText( buf );

	if( bHjiyu ) {
		cnt = p_DBDinpView->m_Seldata.GetHjiyuCount(0);
	}
	else {
		cnt = p_DBDinpView->m_Seldata.GetNPfuriCount();
	}

	if( ! IsHjiyuItemGyosyu() ) {
		GetDlgItem( IDC_ST_HJIYU )->SetWindowText( "" );
		cnt = -1;
	}

	// 変動事由・振替科目[公益]マスターでない
	if( cnt < 0 ) {
		m_Hjiyu1.EnableWindow(FALSE);
		m_Hjiyu2.EnableWindow(FALSE);
		GetDlgItem(IDC_ST_HJIYU)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC21)->EnableWindow(FALSE);

		getINP_CTL( RP_HJIYU1_PN )->INP_ena = FALSE;
		getINP_CTL( RP_HJIYU2_PN )->INP_ena = FALSE;
		return;
	}
	else {
		m_Hjiyu1.EnableWindow(TRUE);
		m_Hjiyu2.EnableWindow(TRUE);
		GetDlgItem(IDC_ST_HJIYU)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC21)->EnableWindow(TRUE);

		getINP_CTL( RP_HJIYU1_PN )->INP_ena = TRUE;
		getINP_CTL( RP_HJIYU2_PN )->INP_ena = TRUE;
	}

	m_Hjiyu1.ResetContent();
	m_Hjiyu2.ResetContent();

	m_Hjiyu1.AddString("");
	m_Hjiyu1.SetItemData( 0, NON_SELECT );
	m_Hjiyu2.AddString("");
	m_Hjiyu2.SetItemData( 0, NON_SELECT );

	const KSH_NAMDTA*	pKshNam;
	const NP_FURIDTA*	pNPfuri;
	index = 1;

	//遡及事由
	CDWordArray sokyu;

	if( bHjiyu ) {
		for( i = 0; i < cnt; i++ ) {
			pKshNam = p_DBDinpView->m_Seldata.GetHjiyuData( 0, i );

			sprintf_s( buf, sizeof buf, "%2d.%.40s", pKshNam->hn_cod, pKshNam->hn_name );
			m_Hjiyu1.AddString(buf);
			m_Hjiyu1.SetItemData( index, pKshNam->hn_cod );
			m_Hjiyu2.AddString(buf);
			m_Hjiyu2.SetItemData( index, pKshNam->hn_cod );
			index++;

#ifdef SOKYU_CLOSE
			if( (pKshNam->hn_cod >= 86 && pKshNam->hn_cod <= 90) ) {
				sokyu.Add( pKshNam->hn_cod );
			}
#endif
		}
		// 株主資本以外
		cnt = p_DBDinpView->m_Seldata.GetHjiyuCount(1);

		for( i = 0; i < cnt; i++ ) {
			pKshNam = p_DBDinpView->m_Seldata.GetHjiyuData( 1, i );

#ifdef SOKYU_CLOSE
			int add_flg = 0;
			if( (pKshNam->hn_cod >= 86 && pKshNam->hn_cod <= 90) ) {
				for( int n = 0; n < sokyu.GetCount(); n++ ) {
					if( sokyu[n] == pKshNam->hn_cod ) {
						add_flg = 1;
						break;
					}
				}
			}
			if( add_flg )	continue;
#endif

			sprintf_s( buf, sizeof buf, "%2d.%.40s", pKshNam->hn_cod, pKshNam->hn_name );
			m_Hjiyu1.AddString(buf);
			m_Hjiyu1.SetItemData( index, pKshNam->hn_cod );
			m_Hjiyu2.AddString(buf);
			m_Hjiyu2.SetItemData( index, pKshNam->hn_cod );
			index++;
		}
	}
	else {
		if( IsSyafuKaisei( pDBzm->zvol ) ) { 	//内部取引
			KSH_SCAN ksh;
			int ret;

			i = 0;
			do {
				if( (ret = p_DBDinpView->m_Seldata.GetScanKHKbn( i, &ksh )) == 0 ) {
					if( ksh.item == 0 || ksh.item == HJIYU_TAISYO ) {
						i++;
						continue;
					}

					int nbcd;
					if( ksh.item == HJIYU_MITOROK ) {
						strcpy_s( ksh.ks_name, sizeof ksh.ks_name, "（なし）" ); 
						nbcd = HJIYU_MITOROK;
					}
					else {
						nbcd =  ksh.item-110;
					}

					m_Hjiyu1.AddString( ksh.ks_name );
					m_Hjiyu1.SetItemData( index, nbcd );
					m_Hjiyu2.AddString( ksh.ks_name );
					m_Hjiyu2.SetItemData( index, nbcd );
					i++;
					index++;
				}
			} while( ret == 0 );
		}
		else {
			for( i = 0; i < cnt; i++ ) {
				pNPfuri = p_DBDinpView->m_Seldata.GetNPfuriData( i );

				sprintf_s( buf, sizeof buf, "%2d.%.32s", pNPfuri->nf_swno, pNPfuri->nf_name );
				m_Hjiyu1.AddString(buf);
				m_Hjiyu1.SetItemData( index, pNPfuri->nf_swno );
				m_Hjiyu2.AddString(buf);
				m_Hjiyu2.SetItemData( index, pNPfuri->nf_swno );
				index++;
			}
		}
	}
}


// 科目名称再セット
void CRpkeyDlg::knam_set( struct _ItemData *data )
{
	struct _DBKNREC	*pKn;

	if( pKn = pDBzm->DB_PjisToKnrec( data->IP_CdNum ) ) {
		memcpy( data->IP_KNAM, pKn->knnam, KMNLEN );
	}
}


// ｺﾝﾄﾛｰﾙがﾌｫｰｶｽを受け取った時
int CRpkeyDlg::set_nowpn(int pn)
{
	if( !m_PGSW ) return 1;

	int prev_pn /*, ret, mode, id*/;

	prev_pn = m_NOWPN;
	m_NOWPN = pn;

	// 選択ダイアログの表示
	if( prev_pn != m_NOWPN ) {
		if( pZmSel->IsDisplay() )
			pZmSel->DialogOFF();
	}

#ifdef OLD_KEEP
	switch( pn ) {
	case RP_TKY1_PN:
	case RP_TKY2_PN:
		ZmselDataSet(pn, &data);

		if( prev_pn != m_NOWPN ) mode = ZSEL_MODE_TEKIYO;
		else {
			// 保存ﾓｰﾄﾞﾞがある場合
			if( m_svMode != -1 ) mode = m_svMode;
			else				 mode = ZSEL_MODE_TEKIYO;
		}

	//	if( pn == RP_TKY2_PN ) {
	//		if( m_svMode == ZSEL_MODE_TEKIYO || m_svMode == ZSEL_MODE_TEKIYO50 )
	//			mode = m_svMode;
	//	}
		ret = ZmselDlgON(pn, mode, &data, NULL);
		break;
	default:
		if( m_pZmsel->IsDisplay() ) {
			m_pZmsel->DialogOFF();
		}
		break;
	}

	// ｽﾀﾃｨｯｸの色更新
	id = GetInptbl(prev_pn)->STC_id;
	if( id )	GetDlgItem(id)->Invalidate();
	id = GetInptbl(m_NOWPN)->STC_id;
	if( id )	GetDlgItem(id)->Invalidate();
#endif


	// 07.01 /03
	switch( m_NOWPN ) {
	case RP_NODENP_PN:
		pn = RP_DENP2_PN;	break;
	case RP_NOBMON_PN:
		pn = RP_BMON2_PN;	break;
	case RP_NOCBMN_PN:
		pn = RP_CBMN2_PN;	break;
	case RP_NOKOJI_PN:
		pn = RP_KOJI2_PN;	break;
	case RP_NOCKJI_PN:
		pn = RP_CKJI2_PN;	break;
	case RP_NOTKY_PN:
		pn = RP_TKY2_PN;	break;
	}

#ifdef OLD_CLOSE
	// 色をつけた 10.25 /01
	select_back_col( pn );
	inp_bkcol( pn );
#endif

	return 0;
}


int CRpkeyDlg::get_nowpn()
{
	return m_NOWPN;
}


int CRpkeyDlg::set_nonICSpn( tagMSG* pMsg )
{
	CWnd *pwnd;

	if( pwnd = GetFocus() )
	{
		if( get_nowpn() != RP_IDOK && pwnd->m_hWnd == GetDlgItem( IDOK )->m_hWnd )
		{
			return set_nowpn( RP_IDOK );
		}
		else if( get_nowpn() != RP_IDCANCEL && pwnd->m_hWnd == GetDlgItem( IDCANCEL )->m_hWnd )
		{
			return set_nowpn( RP_IDCANCEL );
		}
		else if( get_nowpn() != RP_IDCBTNDEL && pwnd->m_hWnd == GetDlgItem( IDC_BTN_DEL )->m_hWnd )
		{
			return set_nowpn( IDC_BTN_DEL );
		}
		// 部門・枝番チェックボタン
		else if( get_nowpn() != RP_NODENP_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHECK_DENP )->m_hWnd )
		{
			return set_nowpn( RP_NODENP_PN );
		}
		else if( get_nowpn() != RP_NOBMON_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHECK_DBM )->m_hWnd )
		{
			return set_nowpn( RP_NOBMON_PN );
		}
		else if( get_nowpn() != RP_NOCBMN_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHECK_CBM )->m_hWnd )
		{
			return set_nowpn( RP_NOCBMN_PN );
		}
		else if( get_nowpn() != RP_NOKOJI_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHECK_DKJ )->m_hWnd )
		{
			return set_nowpn( RP_NOKOJI_PN );
		}
		else if( get_nowpn() != RP_NOCKJI_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHECK_CKJ )->m_hWnd )
		{
			return set_nowpn( RP_NOCKJI_PN );
		}
		else if( get_nowpn() != RP_NOTKY_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHECK_TKY )->m_hWnd )
		{
			return set_nowpn( RP_NOTKY_PN );
		}
		// 消費税関係
		else if( get_nowpn() != RP_SYZSW1_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNSYZSW )->m_hWnd )
		{
			return set_nowpn( RP_SYZSW1_PN );
		}
		else if( get_nowpn() != RP_ZRITU1_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNZRITU )->m_hWnd )
		{
			return set_nowpn( RP_ZRITU1_PN );
		}
		else if( get_nowpn() != RP_SKBN1_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNSKBN )->m_hWnd )
		{
			return set_nowpn( RP_SKBN1_PN );
		}
		else if( get_nowpn() != RP_URISIRE1_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNURISIRE )->m_hWnd )
		{
			return set_nowpn( RP_URISIRE1_PN );
		}
		else if( get_nowpn() != RP_SYZSW2_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNSYZSW2 )->m_hWnd )
		{
			return set_nowpn( RP_SYZSW2_PN );
		}
		else if( get_nowpn() != RP_ZRITU2_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNZRITU2 )->m_hWnd )
		{
			return set_nowpn( RP_ZRITU2_PN );
		}
		else if( get_nowpn() != RP_SKBN2_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNSKBN2 )->m_hWnd )
		{
			return set_nowpn( RP_SKBN2_PN );
		}
		else if( get_nowpn() != RP_URISIRE2_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNURISIRE2 )->m_hWnd )
		{
			return set_nowpn( RP_URISIRE2_PN );
		}
		else if( get_nowpn() != RP_TOKUS1_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNTOKUS )->m_hWnd )	//特定収入[06.03 /08]
		{
			return set_nowpn( RP_TOKUS1_PN );
		}
		else if( get_nowpn() != RP_TOKUS2_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNTOKUS2 )->m_hWnd )
		{
			return set_nowpn( RP_TOKUS2_PN );
		}
		// 変動事由
		else if( get_nowpn() != RP_HJIYU1_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNHJIYU )->m_hWnd )
		{
			return set_nowpn( RP_HJIYU1_PN );
		}
		else if( get_nowpn() != RP_HJIYU2_PN && pwnd->m_hWnd == GetDlgItem( IDC_SCNHJIYU2 )->m_hWnd )
		{
			return set_nowpn( RP_HJIYU2_PN );
		}
		// 仕訳確定
		else if( get_nowpn() != RP_CONFIRM_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHK_CONFIRM )->m_hWnd )
		{
			return set_nowpn( RP_CONFIRM_PN );
		}
		else if( get_nowpn() != RP_CONMDFY_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHK_CONMDFY )->m_hWnd )
		{
			return set_nowpn( RP_CONMDFY_PN );
		}
		// 付箋取消
		else if( get_nowpn() != RP_HSEN2_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHK_HUSEN )->m_hWnd )
		{
			return set_nowpn( RP_HSEN2_PN );
		}
		// 証憑番号取消
		else if( get_nowpn() != RP_NODOCEVI_PN && pwnd->m_hWnd == GetDlgItem( IDC_CHECK_SNO )->m_hWnd )
		{
			return set_nowpn( RP_NODOCEVI_PN );
		}

	}
	return get_nowpn();
}


// 消費税 常に同じ設定の表示
void CRpkeyDlg::def_syselinz()
{
	struct _ZMSGBIND* zmb;
	int idx, i;

	zmb = &ZMSGBND[SWKKB_PG];
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

	idx = 0;
	m_Syzsw2.AddString( zmb->zmsg[0].zeitem );
	m_Syzsw2.SetItemData( idx++, zmb->zmsg[0].zeifnc );
	m_Syzsw2.AddString( "*仕訳区分 取消" );
	m_Syzsw2.SetItemData( idx++, SWKSW_DEL );

	for( i = 1; i < zmb->n_zmsg; i++ ) {
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

		m_Syzsw2.AddString( zmb->zmsg[i].zeitem );
		m_Syzsw2.SetItemData( idx++, zmb->zmsg[i].zeifnc );
	}

	zmb = &ZMSGBND[ZRITU_PG];
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
		m_Zritu2.AddString( zmb->zmsg[i].zeitem );
		m_Zritu2.SetItemData( idx, zmb->zmsg[i].zeifnc );
		idx++;
	}

	zmb = &ZMSGBND[SKBN_PG];
	for( i = 0; i < zmb->n_zmsg; i++ ) {
		m_Skbn.AddString( zmb->zmsg[i].zeitem );
		m_Skbn.SetItemData( i, zmb->zmsg[i].zeifnc );
		m_Skbn2.AddString( zmb->zmsg[i].zeitem );
		m_Skbn2.SetItemData( i, zmb->zmsg[i].zeifnc );
	}

	if( !( !bInvMaster || !bInvUse ) ) {
		m_MenWari.AddString("80% 控除");
		m_MenWari.AddString("50% 控除");
		m_MenWari2.AddString("80% 控除");
		m_MenWari2.AddString("50% 控除");
	}
}



BOOL KanriUrikbnCheck( long urisgn )
{
	BOOL bRet = FALSE;

	BYTE sgn2 = pDBzm->zvol->s_sgn2;
	BYTE sgn3 = pDBzm->zvol->s_sgn3;

	if( urisgn == NON_SELECT )
		return TRUE;

	//単一でもその他はある
	if( ! (sgn2&0x80) ) {
		if( urisgn == GY_ETC )
			bRet = TRUE;

		return bRet;
	}

	switch( urisgn ) {
	case GY_OROSI:
		if( (sgn3&0x10) )	bRet = TRUE;
		break;
	case GY_KOURI:
		if( (sgn3&0x20) )	bRet = TRUE;
		break;
	case GY_SEIZO:
		if( (sgn3&0x40) )	bRet = TRUE;
		break;
	case GY_SRVCE:
		if( (sgn3&0x80) )	bRet = TRUE;
		break;
	case GY_ETC	:
		bRet = TRUE;
		break;
	case GY_FDOSN:
		if( (sgn3&0x08) )	bRet = TRUE;
		break;
	}

	return bRet;
}


// 消費税選択関係イニシャライズ
void CRpkeyDlg::set_syselinz()
{
	struct _ZMSGBIND* zmb;
	int i, n;
	int tp = is_symstyp();

	m_Urisire.ResetContent();
	m_Urisire2.ResetContent();
	switch( tp ) {
	case KANI_SYMST:
		GetDlgItem(IDC_ST_URISIRE)->SetWindowText( "売上区分：" );
		zmb = &ZMSGBND[GYOSYU_PG];
		for( i = n = 0; i < zmb->n_zmsg; i++ ) {
			if( (i+1) == zmb->n_zmsg ) { //第六種対応
				if( Voln1->ee_ymd < KANI6_YMD ) {
					break;
				}
			}
			m_Urisire.AddString( zmb->zmsg[i].zeitem );
			m_Urisire.SetItemData( i, zmb->zmsg[i].zeifnc );

			//ボリュームのサインがあるもののみとする
			if( KanriUrikbnCheck( zmb->zmsg[i].zeifnc ) ) {
				m_Urisire2.AddString( zmb->zmsg[i].zeitem );
				m_Urisire2.SetItemData( n, zmb->zmsg[i].zeifnc );
				n++;
			}
		}	
		getINP_CTL( RP_URISIRE1_PN )->INP_ena = TRUE;
		getINP_CTL( RP_URISIRE2_PN )->INP_ena = TRUE;
		break;
	case KOBET_SYMST:
		GetDlgItem(IDC_ST_URISIRE)->SetWindowText( "仕入区分：" );
		zmb = &ZMSGBND[SIRE_PG];
		for( i = 0; i < zmb->n_zmsg; i++ ) {
			m_Urisire.AddString( zmb->zmsg[i].zeitem );
			m_Urisire.SetItemData( i, zmb->zmsg[i].zeifnc );
			m_Urisire2.AddString( zmb->zmsg[i].zeitem );
			m_Urisire2.SetItemData( i, zmb->zmsg[i].zeifnc );
		}
		n = i;
		// 02.22 /12
		m_Urisire.AddString( "基本設定" );
		m_Urisire.SetItemData( n++, URI_KIHON );
		m_Urisire.AddString( "仕入区分対象" );
		m_Urisire.SetItemData( n, URI_TAISYO );
		// 12.14 /11
		n = i;
		m_Urisire2.AddString( "科目設定に従う" );
		m_Urisire2.SetItemData( n, URI_KAMOKU );

		getINP_CTL( RP_URISIRE1_PN )->INP_ena = TRUE;
		getINP_CTL( RP_URISIRE2_PN )->INP_ena = TRUE;
		break;
	case NORMAL_MST:
	case NORMAL_SYMST:
	default:
		GetDlgItem(IDC_ST_URISIRE)->SetWindowText( "－－－－：" );
		GetDlgItem(IDC_ST_URISIRE)->EnableWindow(FALSE);
		m_Urisire.EnableWindow(FALSE);
		m_Urisire2.EnableWindow(FALSE);

		getINP_CTL( RP_URISIRE1_PN )->INP_ena = FALSE;
		getINP_CTL( RP_URISIRE2_PN )->INP_ena = FALSE;
		break;
	}

	// 特定収入
	if( pDBzm->zvol->apno >= 0x50 ) {
		if( tp == KOBET_SYMST ) {
			zmb = &ZMSGBND[KBTOKUS_PG];
		}
		else {
			zmb = &ZMSGBND[TOKUS_PG];
		}
		int idx = 0;
		for( i = 0; i < zmb->n_zmsg; i++ ) {
			if( ! IsMasterType(MST_SYZ5_8|MST_SYZ8) ) {
				if( zmb->zmsg[i].zeifnc == TK_SYNYU8 || zmb->zmsg[i].zeifnc == TK_FUTOK8 || 
					zmb->zmsg[i].zeifnc == TK_KATOK8 ||	zmb->zmsg[i].zeifnc == TK_HKATK8 ||
					zmb->zmsg[i].zeifnc == TK_KYOTK8 ) {
					continue;
				}
			}
			if( ! IsMasterType(MST_SYZ8_10|MST_SYZ10) ) {
				if( zmb->zmsg[i].zeifnc >= TK_SYNYU10 && zmb->zmsg[i].zeifnc <= TK_FUTOK8K ) {
					continue;
				}
			}

			if( zmb->zmsg[i].zeifnc != TK_GAI ) {
				m_Tokus.AddString( zmb->zmsg[i].zeitem );
				m_Tokus.SetItemData( idx, zmb->zmsg[i].zeifnc );
			}
			m_Tokus2.AddString( zmb->zmsg[i].zeitem );
			m_Tokus2.SetItemData( idx, zmb->zmsg[i].zeifnc );
			idx++;
		}
		getINP_CTL( RP_TOKUS1_PN )->INP_ena = TRUE;
		getINP_CTL( RP_TOKUS2_PN )->INP_ena = TRUE;
	}
	else {
		GetDlgItem(IDC_ST_TOKUS)->EnableWindow(FALSE);
		m_Tokus.EnableWindow(FALSE);
		m_Tokus2.EnableWindow(FALSE);

		GetDlgItem(IDC_ST_TOKUS)->ShowWindow(SW_HIDE);
		m_Tokus.ShowWindow(SW_HIDE);
		m_Tokus2.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC22)->ShowWindow(SW_HIDE);

		getINP_CTL( RP_TOKUS1_PN )->INP_ena = FALSE;
		getINP_CTL( RP_TOKUS2_PN )->INP_ena = FALSE;
	}
}

BEGIN_EVENTSINK_MAP(CRpkeyDlg, ICSDialog)
    //{{AFX_EVENTSINK_MAP(CRpkeyDlg)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT_RANGE(CRpkeyDlg, IDC_SCNDATE1, IDC_SCNTDAT2, 3 /* SetFocus */, OnSetFocusRpDBedt, VTS_I4 )
	ON_EVENT_RANGE(CRpkeyDlg, IDC_SCNDATE1, IDC_SCNTDAT2, 2 /* KillFocus */, OnKillFocusRpDBedt, VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CRpkeyDlg, IDC_SCNDATE1, IDC_SCNTDAT2, 1 /* Termination */, OnTerminationRpDBedt, VTS_I4 VTS_I4 VTS_I4 VTS_I4)	
	ON_EVENT_RANGE(CRpkeyDlg, IDC_SCNDATE1, IDC_SCNTDAT2, 5 /* ImeEndComposition */, OnImeEndCompositionRpDBedt, VTS_I4 VTS_I2 VTS_BSTR VTS_BSTR)
	ON_EVENT_RANGE(CRpkeyDlg, IDC_SCNDATE1, IDC_SCNTDAT2, 4 /* KanaTermination */, OnKanaTerminationRpDBedt, VTS_I4 VTS_BSTR)
END_EVENTSINK_MAP()

void CRpkeyDlg::OnSetFocusRpDBedt( UINT ID )
{
	if( ! m_PGSW )	return;

	switch( ID ) {
	case IDC_SCNDATE1:	OnSetFocusRpdate1();	break;
	case IDC_SCNDATE2:	OnSetFocusRpdate2();	break;
	case IDC_SCNDENP1:	OnSetFocusRpdenp1();	break;
	case IDC_SCNDENP2:	OnSetFocusRpdenp2();	break;
	case IDC_SCNBMON1:	OnSetFocusRpbmon1();	break;
	case IDC_SCNBMON2:	OnSetFocusRpbmon2();	break;
	case IDC_SCNCBMN1:	OnSetFocusRpcbmn1();	break;
	case IDC_SCNCBMN2:	OnSetFocusRpcbmn2();	break;
	case IDC_SCNKOJI1:	OnSetFocusRpkoji1();	break;
	case IDC_SCNKOJI2:	OnSetFocusRpkoji2();	break;
	case IDC_SCNCKJI1:	OnSetFocusRpckji1();	break;
	case IDC_SCNCKJI2:	OnSetFocusRpckji2();	break;
	case IDC_SCNDEBT:	OnSetFocusRpDebt1();	break;
	case IDC_SCNDEBT2: 	OnSetFocusRpDebt2();	break;
	case IDC_SCNCRED: 	OnSetFocusRpCred1();	break;
	case IDC_SCNCRED2:	OnSetFocusRpCred2();	break; 
	case IDC_SCNVAL1:	OnSetFocusRpval1();	break;
	case IDC_SCNVAL2:	OnSetFocusRpval2();	break;
	case IDC_SCNTKY: 	OnSetFocusRptky1();	break; 
	case IDC_SCNTKY2: 	OnSetFocusRptky2();	break; 
	case IDC_SCNSNUM1:	OnSetFocusRpSnum1();	break;
	case IDC_SCNSNUM2:	OnSetFocusRpSnum2();	break;
	case IDC_SCNTDAT1:	OnSetFocusRpTgdate1();	break;
	case IDC_SCNTDAT2:	OnSetFocusRpTgdate2();	break;

	case IDC_SCNSNO1:	OnSetFocusRpDocEvi1();	break;
	case IDC_SCNSNO2:	OnSetFocusRpDocEvi2();	break;
	case IDC_SCN_INVNO:	
		OnSetFocusRpInvno();
		GetDlgItem(IDC_SCN_INVNODISP)->ShowWindow(SW_HIDE);
		break;
	case IDC_SCN_INVNO2:
		OnSetFocusRpInvno2();
		GetDlgItem(IDC_SCN_INVNO2DISP)->ShowWindow(SW_HIDE);
		break;

	case IDC_SCN_INVNODISP:
		GetDlgItem(IDC_SCN_INVNODISP)->ShowWindow(SW_HIDE);
		set_focus(RP_INVNO_PN);
		break;

	case IDC_SCN_INVNO2DISP:
		GetDlgItem(IDC_SCN_INVNO2DISP)->ShowWindow(SW_HIDE);
		set_focus(RP_INVNO2_PN);
		break;
	default:
		ASSERT( FALSE );
		break;
	}
}

void CRpkeyDlg::OnKillFocusRpDBedt( UINT ID, long inplen )
{
	if( ! m_PGSW )	return;

	switch( ID ) {
	case IDC_SCNDATE1:	OnKillFocusRpdate1(inplen);	break;
	case IDC_SCNDATE2:	OnKillFocusRpdate2(inplen);	break;
	case IDC_SCNDENP1:	OnKillFocusRpdenp1(inplen);	break;
	case IDC_SCNDENP2:	OnKillFocusRpdenp2(inplen);	break;
	case IDC_SCNBMON1:	OnKillFocusRpbmon1(inplen);	break;
	case IDC_SCNBMON2:	OnKillFocusRpbmon2(inplen);	break;
	case IDC_SCNCBMN1:	OnKillFocusRpcbmn1(inplen);	break;
	case IDC_SCNCBMN2:	OnKillFocusRpcbmn2(inplen);	break;
	case IDC_SCNKOJI1:	OnKillFocusRpkoji1(inplen);	break;
	case IDC_SCNKOJI2:	OnKillFocusRpkoji2(inplen);	break;
	case IDC_SCNCKJI1:	OnKillFocusRpckji1(inplen);	break;
	case IDC_SCNCKJI2:	OnKillFocusRpckji2(inplen);	break;
	case IDC_SCNDEBT:	OnKillFocusRpDebt1(inplen);	break;
	case IDC_SCNDEBT2: 	OnKillFocusRpDebt2(inplen);	break;
	case IDC_SCNCRED: 	OnKillFocusRpCred1(inplen);	break;
	case IDC_SCNCRED2:	OnKillFocusRpCred2(inplen);	break; 
	case IDC_SCNVAL1:	OnKillFocusRpval1(inplen);	break;
	case IDC_SCNVAL2:	OnKillFocusRpval2(inplen);	break;
	case IDC_SCNTKY: 	OnKillFocusRptky1(inplen);	break; 
	case IDC_SCNTKY2: 	OnKillFocusRptky2(inplen);	break; 
	case IDC_SCNSNUM1:	OnKillFocusRpSnum1(inplen);	break;
	case IDC_SCNSNUM2:	OnKillFocusRpSnum2(inplen);	break;
	case IDC_SCNTDAT1:	OnKillFocusRpTgdate1(inplen);	break;
	case IDC_SCNTDAT2:	OnKillFocusRpTgdate2(inplen);	break;

	case IDC_SCNSNO1:	OnKillFocusRpDocEvi1(inplen);	break;
	case IDC_SCNSNO2:	OnKillFocusRpDocEvi2(inplen);	break;
	case IDC_SCN_INVNO:	
		OnKillFocusRpInvno( inplen );	
		if( !m_bIgnoreFocus && bInvDisp) {
			GetDlgItem(IDC_SCN_INVNODISP)->ShowWindow(SW_SHOW);
		}
		break;

	case IDC_SCN_INVNO2:
		OnKillFocusRpInvno2( inplen );
		if( !m_bIgnoreFocus && bInvDisp ) {
			GetDlgItem(IDC_SCN_INVNO2DISP)->ShowWindow(SW_SHOW);
		}
		break;
	case IDC_SCN_INVNODISP:
	case IDC_SCN_INVNO2DISP:
		break;
	default:
		ASSERT( FALSE );
		break;
	}
}

void CRpkeyDlg::OnTerminationRpDBedt( UINT ID, long nChar, long inplen, long kst )
{
	if( ! m_PGSW )	return;

	switch( ID ) {
	case IDC_SCNDATE1:	OnTerminationRpdate1(nChar,inplen,kst);	break;
	case IDC_SCNDATE2:	OnTerminationRpdate2(nChar,inplen,kst);	break;
	case IDC_SCNDENP1:	OnTerminationRpdenp1(nChar,inplen,kst);	break;
	case IDC_SCNDENP2:	OnTerminationRpdenp2(nChar,inplen,kst);	break;
	case IDC_SCNBMON1:	OnTerminationRpbmon1(nChar,inplen,kst);	break;
	case IDC_SCNBMON2:	OnTerminationRpbmon2(nChar,inplen,kst);	break;
	case IDC_SCNCBMN1:	OnTerminationRpcbmn1(nChar,inplen,kst);	break;
	case IDC_SCNCBMN2:	OnTerminationRpcbmn2(nChar,inplen,kst);	break;
	case IDC_SCNKOJI1:	OnTerminationRpkoji1(nChar,inplen,kst);	break;
	case IDC_SCNKOJI2:	OnTerminationRpkoji2(nChar,inplen,kst);	break;
	case IDC_SCNCKJI1:	OnTerminationRpckji1(nChar,inplen,kst);	break;
	case IDC_SCNCKJI2:	OnTerminationRpckji2(nChar,inplen,kst);	break;
	case IDC_SCNDEBT:	OnTerminationRpDebt1(nChar,inplen,kst);	break;
	case IDC_SCNDEBT2:	OnTerminationRpDebt2(nChar,inplen,kst);	break;
	case IDC_SCNCRED: 	OnTerminationRpCred1(nChar,inplen,kst);	break;
	case IDC_SCNCRED2: 	OnTerminationRpCred2(nChar,inplen,kst);	break;
	case IDC_SCNVAL1:	OnTerminationRpval1(nChar,inplen,kst);	break;
	case IDC_SCNVAL2:	OnTerminationRpval2(nChar,inplen,kst);	break;
	case IDC_SCNTKY:	OnTerminationRptky1(nChar,inplen,kst);	break;
	case IDC_SCNTKY2:	OnTerminationRptky2(nChar,inplen,kst);	break;
	case IDC_SCNSNUM1:	OnTerminationRpSnum1(nChar,inplen,kst);	break;
	case IDC_SCNSNUM2:	OnTerminationRpSnum2(nChar,inplen,kst);	break;
	case IDC_SCNTDAT1:	OnTerminationRpTgdate1(nChar,inplen,kst);	break;
	case IDC_SCNTDAT2:	OnTerminationRpTgdate2(nChar,inplen,kst);	break;

	case IDC_SCNSNO1:	OnTerminationRpDocEvi1(nChar,inplen,kst);	break;
	case IDC_SCNSNO2:	OnTerminationRpDocEvi2(nChar,inplen,kst);	break;
	case IDC_SCN_INVNO:	OnTerminationRpInvno( nChar, inplen, kst );	break;
	case IDC_SCN_INVNO2:	OnTerminationRpInvno2( nChar, inplen, kst );	break;
		break;
		break;
	default:
		ASSERT( FALSE );
		break;
	}
}

void CRpkeyDlg::OnImeEndCompositionRpDBedt( UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring)
{
	if( ! m_PGSW )	return;

	char	tmp[128];
	char	ystr[128];
	int		term = 0;
	int		st, pos, len;
	VARIANT	var;

	switch( ID ) {
	case IDC_SCNDEBT:
		if( !lastBrnInpStr((char*)string, tmp, sizeof tmp) )
			return;

		if( ItemDataCheck( RP_DEBT1_PN, nChar, tmp, 0 ) <= 0 )
			return;
		OnTerminationRpDebt1( nChar, 0, 0 );
		break;
	case IDC_SCNDEBT2:
		if( !lastBrnInpStr((char*)string, tmp, sizeof tmp) )
			return;
		if( ItemDataCheck( RP_DEBT2_PN, nChar, tmp, 0 ) <= 0 )
			return;
		OnTerminationRpDebt2( nChar, 0, 0 );
		break;
	case IDC_SCNCRED:
		if( !lastBrnInpStr((char*)string, tmp, sizeof tmp) )
			return;
		if( ItemDataCheck( RP_CRED1_PN, nChar, tmp, 1 ) <= 0 )
			return;
		OnTerminationRpCred1( nChar, 0, 0 );
		break;
	case IDC_SCNCRED2:
		if( !lastBrnInpStr((char*)string, tmp, sizeof tmp) )
			return;
		if( ItemDataCheck( RP_CRED2_PN, nChar, tmp, 1 ) <= 0 )
			return;
		OnTerminationRpCred2( nChar, 0, 0 );
		break;

	case IDC_SCNTKY:
	case IDC_SCNTKY2:
		term = 1;

		len = get_strcnt( (char*)string, Voln1->tk_ln * 2 );
		memcpy( tmp, string, len );
		tmp[len] = '\0';

		memcpy( ystr, ystring, len );
		ystr[len] = '\0';

		if( (st = TekiyoStr_Chk( string )) > 0 ) {

			if( isdigitstr( (char*)ystring ) ) {
				if( strlen( ystring ) <= 2 ) {
					if( ! pZmSel->IsDisplay() ) {
						term = 100;
					}
				}
			}

			if( term != 100 ) {
				if( TekiyoKeyMode() ) {	// 摘要コード＋Enter で 摘要をセット
					if(	st == 1 ) {
						if( nChar == VK_RETURN ) {
							_KanaTermTekiyo( ID, ystring, FALSE );
							term = 0;
						}
					}
					if( nChar == VK_TAB ) {
						if( ! isdigitstr( (char*)ystring ) ) {
							_KanaTermTekiyo( ID, ystring, FALSE );
							term = 0;
						}
					//	else
					//		_KanaTermTekiyo( ID, ystring, TRUE );	
					//	term = 0;
					}
					else if( nChar == VK_F12 ) {
						if( ! isdigitstr( (char*)ystr ) ) {
							_KanaTermTekiyo( ID, ystring, FALSE );
							term = 0;
						}
					}
				}
				else {
					if( nChar == VK_TAB || nChar == VK_F12 ) {
						_KanaTermTekiyo( ID, ystring, FALSE );
						term = 0;
					}
				}
			}
		}
		if( term ) {
			var.pbVal = (BYTE*)tmp;
			pos = ((CICSDBEDT*)GetDlgItem(ID))->GetCaretPosition();
			((CICSDBEDT*)GetDlgItem(ID))->InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );
		}

		break;


	case IDC_SCNBMON1:
	case IDC_SCNBMON2:
	case IDC_SCNCBMN1:
	case IDC_SCNCBMN2:
		if( CheckImeEndBmon( ID, nChar, string ) != 0 )
			return;

		switch( ID ) {
		case IDC_SCNBMON1:	OnTerminationRpbmon1( nChar, 1, 0 );	break;
		case IDC_SCNBMON2:	OnTerminationRpbmon2( nChar, 1, 0 );	break;
		case IDC_SCNCBMN1:	OnTerminationRpcbmn1( nChar, 1, 0 );	break;
		case IDC_SCNCBMN2:	OnTerminationRpcbmn2( nChar, 1, 0 );	break;
		}
		break;

	case IDC_SCNKOJI1:
	case IDC_SCNKOJI2:
	case IDC_SCNCKJI1:
	case IDC_SCNCKJI2:
		if( CheckImeEndKoji( ID, nChar, string ) != 0 )
			return;

		switch( ID ) {
		case IDC_SCNKOJI1:	OnTerminationRpkoji1( nChar, 1, 0 );	break;
		case IDC_SCNKOJI2:	OnTerminationRpkoji2( nChar, 1, 0 );	break;
		case IDC_SCNCKJI1:	OnTerminationRpckji1( nChar, 1, 0 );	break;
		case IDC_SCNCKJI2:	OnTerminationRpckji2( nChar, 1, 0 );	break;
		}
		break;

	case IDC_SCN_INVNO:
		if( !lastBrnInpStr((char*)string, tmp, sizeof tmp) )
			return;

		if( CheckDataDBeditInv(ID, nChar, tmp) <= 0 )
			return;

		OnTerminationRpInvno(nChar, 1, 0);
		break;
	case IDC_SCN_INVNO2:
		if( !lastBrnInpStr((char*)string, tmp, sizeof tmp) )
			return;

		if( CheckDataDBeditInv(ID, nChar, tmp) <= 0 )
			return;

		OnTerminationRpInvno2(nChar, 1, 0);
		break;

	default:
		break;
	}
}


// 科目欄での カナターミネーション
void CRpkeyDlg::OnKanaTerminationRpDBedt(UINT ID, LPCTSTR Kana)
{
	if( ! m_PGSW )	return;

	switch( ID ) {
	case IDC_SCNDEBT:
	case IDC_SCNDEBT2:
	case IDC_SCNCRED:
	case IDC_SCNCRED2:
		KanaTerminationKamoku(ID, Kana);
		break;
	}
}


// 摘要欄 でのカナターミネーション
void CRpkeyDlg::_KanaTermTekiyo( UINT ID, LPCTSTR string, BOOL bTkcut/*= FALSE*/ )
{
	CRect rect, dlgrc, dlgrect;
	CSize size, *scrnsize;

	scrnsize = GetScreenSize();

	GetWindowRect( dlgrect );
	GetClientRect( dlgrc );
	GetDlgItem( ID )->GetWindowRect( rect );
	ScreenToClient( rect );
	int ht, tmp = 0;
	ht = rect.Height();

	pZmSel->GetCrntRect( size );

	if( (dlgrect.left + size.cx) > scrnsize->cx ) {
		tmp = (dlgrect.left + size.cx) - scrnsize->cx;
	}

	rect.left = dlgrc.left - tmp;
	rect.top -= (ht/2);

	if( ! isdigitstr( (char*)string ) ) {	// カナ
		pZmSel->SetTekiyoKana( string );
		pZmSel->DialogON( ZSEL_TKYKANA, ZSEL_TOPLEFT, &rect );
		m_BrTek.set_tkquery( TRUE );
	}
	else {
		DWORD code;
		short sgn;
		ZmselDBData	zd;
		VARIANT var;
		
		if( strlen( string ) > 2 ) 
			sgn = 0;
		else
			sgn = 1;

		code = atoi( string );
		pZmSel->InputData( sgn, code, -1, ZSEL_TKY );
		pZmSel->ResultData( &zd );

		// 番号から、摘要選択
		if( zd.errflg != 0 ) {
			Buzzer();
		}
		else {
			var.pbVal = (BYTE*)zd.tkstr;
			int pos = ((CICSDBEDT*)GetDlgItem(ID))->GetCaretPosition();
			((CICSDBEDT*)GetDlgItem(ID))->InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );
		}
	}
}


//////////////////////////////////////////////////////////////////////
// 日付
void CRpkeyDlg::OnSetFocusRpdate1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	try
	{
		set_nowpn( RP_DATE1_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_DATE1_PN ); 
}

void CRpkeyDlg::OnKillFocusRpdate1( long inplen ) 
{
	DateInp( RP_DATE1_PN, &m_Rpline.RP_DATE1_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpdate1(long nChar, long inplen, long kst) 
{
	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = DateInp( RP_DATE1_PN, &m_Rpline.RP_DATE1_DT, nChar, inplen, kst )) )
			return;

		//カーソル移動
		if( term_focus( RP_DATE1_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

void CRpkeyDlg::OnSetFocusRpdate2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	try
	{
		set_nowpn( RP_DATE2_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_DATE2_PN ); 	
}

void CRpkeyDlg::OnKillFocusRpdate2( long inplen ) 
{
	DateInp( RP_DATE2_PN, &m_Rpline.RP_DATE2_DT, 0, inplen, 0 );	
}

void CRpkeyDlg::OnTerminationRpdate2(long nChar, long inplen, long kst) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = DateInp( RP_DATE2_PN, &m_Rpline.RP_DATE2_DT, nChar, inplen, kst )) )
			return;

		//カーソル移動
		if( term_focus( RP_DATE2_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

// 日付チェックデータ
long CRpkeyDlg::CheckDataIcsinputDate( int pn, long nChar ) 
{
	char check[3] = {0};
	char ymd[3] = {0};
	char data[20] = {0};
	BYTE bcd[10] = {0};
	VARIANT var;
	CString str;
	int len;

	UpdateData();
	if( m_ChkMnonly ) { 
		return CheckDataIcsinputMnthOnly( pn, nChar );
	}

	try
	{
	//	DBdata_get( pwnd, getINP_CTL(DATE_PN)->IDC_X, &var, TYPE_BCD, 4 );
		DBdata_get( this, getINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		
		if( var.pbVal != NULL )	{
			len = strlen( (char*)var.pbVal );
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
					int sgn = KESY_SG_GET( m_Rpline._RPDTA[pn].IP_BCDDAY[0] );
					switch( sgn ) {
					case 1 :
						strcpy_s( data, sizeof data, "61" );	break;
					case 2 :
						strcpy_s( data, sizeof data, "62" );	break;
					case 3 :
						strcpy_s( data, sizeof data, "63" );	break;
					case 4 :
						strcpy_s( data, sizeof data, "00" );	break;
					default:
						sprintf_s( data, sizeof data, "%02x", m_Rpline._RPDTA[pn].IP_BCDDAY[0] );
						break;
					}
					if( len == 1 ) {
						data[2] = '0';
						data[3] = var.pbVal[0];
					}
					else	strncpy_s( &data[2], sizeof(data)-2, (char*)var.pbVal, 2 );
					data[4] = 0;
				}
			}
		}

		if( DateInputCheck( data, bcd ) != 0 ) {
			// 不正データが入力された
			var.pbVal = m_Rpline._RPDTA[pn].IP_BCDDAY;
			DBdata_set( this, getINP_CTL( pn)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
			pDBzm->BcdDateToString( str, m_Rpline._RPDTA[pn].IP_BCDDAY );
			DBSetDispString( this, getINP_CTL(pn)->IDC_X, str );
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

		BOOL bCheck;
		if( pn == RP_DATE1_PN ) {
			bCheck = !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd );

			if( bCheck ) {
				// 期間のチェック
				int st = vd.db_vd_check( (BYTE*)check, pDBzm );
				if( st != 0 && st != 2 )
					bCheck = FALSE;
			}
		}
		else {
			// 変更後は、入力可能月を指定
			bCheck = !pDBzm->Myvd_chek( (unsigned char*)check ) && !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd );
		}

		if( bCheck )
//		if( /* !pDBzm->vd_chek( (unsigned char*)check ) && !pDBzm->vd_sign( (unsigned char*)check, (unsigned char*)ymd ) */ )
//		if( !pDBzm->Myvd_chek( (unsigned char*)check ) && !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd ) )
//		if( !vd.db_vd_check( (BYTE*)check, pDBzm ) && !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd ) )
		{
			memmove( bcd, &ymd[0], 2 );
		//	sprintf( data, "%02x%02x%02x", bcd[0], bcd[1] );
		//	var.pbVal = (BYTE*)data;
		//	DBdata_set( pwnd, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			var.pbVal = bcd;
			DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
		}
		else
		{
			Buzzer();
			memset( bcd, 0, sizeof bcd );
			var.pbVal = bcd;
			DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
			memset( m_Rpline._RPDTA[pn].IP_BCDDAY, 0, sizeof m_Rpline._RPDTA[pn].IP_BCDDAY );
			pDBzm->BcdDateToString( str, m_Rpline._RPDTA[pn].IP_BCDDAY );
			DBSetDispString( this, getINP_CTL( pn )->IDC_X, str );
			return -1;
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}

	return nChar;
}


int MonthInputCheck( char* check, BYTE* bcd )
{
	size_t	len	= strlen( check );

	char	tmp[16];
	short	mnth;

	memset( tmp, 0, sizeof tmp );

	if( len <= 0 ) {
		return	-1;
	}

	for( int i = 0; i < len; i++ ) {
		if( IsCharAlphaNumeric( check[i] ) && ! IsCharAlpha( check[i] ) );
		else 
			return -1;
	}

	switch( len ) {
	case 1:
		mnth = -1;
		tmp[0] = '0';
		tmp[1] = check[0];

		break;
	case 2:
		if( check[0] == '6' && check[1] == '1' ) {
			mnth = 61;
		}
		else if( check[0] == '6' && check[1] == '2' ) {
			mnth = 62;
		}
		else if( check[0] == '6' && check[1] == '3' ) {
			mnth = 63;
		}
		else {
			tmp[0] = check[0];
			tmp[1] = check[1];
			mnth = atoi( tmp );
		}	
		break;
	default:
		ASSERT( FALSE );
		return -1;
		break;
	}

	::ZeroMemory( tmp, sizeof tmp );
	
	if( mnth == -1 ) {
		tmp[0] = '0';
		tmp[1] = '0';
	}
	else if( mnth == 0 ) {
		tmp[0] = '0';
		tmp[1] = '0';	
	}
	else if( mnth >= 61 && mnth <= 63 ) {
		sprintf_s( tmp, sizeof tmp, "%02d", mnth );
	}
	else {
		sprintf_s( &tmp[0], sizeof tmp, "%02d", mnth );
	}

	tmp[2] = 'F';
	tmp[3] = 'F';

	l_pack( bcd, tmp, 4 );

	return	0;
}


void BcdMonthToString( char* str, int strsize, BYTE* bcd )
{
	int kes_sgn;

	if( bcd[0] != 0 ) {
		kes_sgn = KESY_SG_GET( bcd[0] );

		if( kes_sgn == 0 )
			sprintf_s( str, strsize, "%02x月", (bcd[0]&0x1f) );
		else {
			switch( kes_sgn ) {
			case 1:
				sprintf_s( str, strsize, "%02x月(Q1)", (bcd[0]&0x1f) );
				break;
			case 2:
				sprintf_s( str, strsize, "%02x月(Q2)", (bcd[0]&0x1f) );
				break;
			case 3:
				sprintf_s( str, strsize, "%02x月(Q3)", (bcd[0]&0x1f) );
				break;
			case 4:
				sprintf_s( str, strsize, "%02x月(決)", (bcd[0]&0x1f) );
				break;
			}
		}
	}
	else	strcpy_s( str, strsize, "      " );
}



//
//	月のみ入力
//
long CRpkeyDlg::CheckDataIcsinputMnthOnly( int pn, long nChar ) 
{
	char check[3] = {0};
	char ymd[3] = {0};
	char data[20] = {0};
	BYTE bcd[10] = {0};
	VARIANT var;
	CString str;
	int len;

	try
	{
		DBdata_get( this, getINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		
		if( var.pbVal != NULL )	{
			len = strlen( (char*)var.pbVal );
			int sgn = 0;
			if( len == 1 ) {
				data[0] = '0';
				data[1] = var.pbVal[0];
				sgn = 1;
			}
			else if( len == 2 ) {
				strncpy_s( data, sizeof data, (char*)var.pbVal, len );
				sgn = 1;
			}

			if( !sgn ) {
				int sgn = KESY_SG_GET( m_Rpline._RPDTA[pn].IP_BCDDAY[0] );
				switch( sgn ) {
				case 1 :
					strcpy_s( data, sizeof data, "61" );	break;
				case 2 :
					strcpy_s( data, sizeof data, "62" );	break;
				case 3 :
					strcpy_s( data, sizeof data, "63" );	break;
				case 4 :
					strcpy_s( data, sizeof data, "00" );	break;
				default:
					sprintf_s( data, sizeof data, "%02x", m_Rpline._RPDTA[pn].IP_BCDDAY[0] );
					break;
				}
				data[2] = 0;
			}
		}

		if( MonthInputCheck( data, bcd ) != 0 ) {
			// 不正データが入力された
			var.pbVal = m_Rpline._RPDTA[pn].IP_BCDDAY;
			DBdata_set( this, getINP_CTL( pn)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 2 );
			BcdMonthToString( str.GetBuffer(128),128, m_Rpline._RPDTA[pn].IP_BCDDAY );
			str.ReleaseBuffer();
			DBSetDispString( this, getINP_CTL(pn)->IDC_X, str );
			return -1;
		}
		else {
			memcpy( check, bcd, 3 );
		}

		CVolDateDB vd;

		BOOL bCheck;
		if( pn == RP_DATE1_PN ) {
			bCheck = !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd );

			if( bCheck ) {
				// 期間のチェック
				int st = vd.db_vd_check( (BYTE*)check, pDBzm );
				if( st != 0 && st != 2 )
					bCheck = FALSE;
			}
		}
		else {
			// 変更後は、入力可能月を指定
			bCheck = !pDBzm->Myvd_chek( (unsigned char*)check ) && !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd );
		}

		if( bCheck )
		{
			memmove( bcd, &ymd[0], 2 );
			var.pbVal = bcd;
			DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_BCD, 2 );
		}
		else
		{
			Buzzer();
			memset( bcd, 0, sizeof bcd );
			var.pbVal = bcd;
			DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_BCD, 2 );
			memset( m_Rpline._RPDTA[pn].IP_BCDDAY, 0, sizeof m_Rpline._RPDTA[pn].IP_BCDDAY );

			BcdMonthToString( str.GetBuffer(128),128, m_Rpline._RPDTA[pn].IP_BCDDAY );
			str.ReleaseBuffer();
			DBSetDispString( this, getINP_CTL(pn)->IDC_X, str );
			return -1;
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}

	return nChar;

}

//
// 日付　入力＆オフフォーカス
long CRpkeyDlg::DateInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check = {0};
	VARIANT var;
	CString str;
	int bcd_len;

	UpdateData();

	bcd_len = m_ChkMnonly ? 2 : 4;

	// 日付文字列チェック＆セット( 貼り付け時も )
	if( inplen > 0 || nChar == 0x16 ) {
		if( ! m_ChkMnonly ) {
			CheckDataIcsinputDate( pn, nChar );
		}
		else {
			CheckDataIcsinputMnthOnly( pn, nChar );
		}
	}
	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_BCD, bcd_len );
		memcpy( check.IP_BCDDAY, var.pbVal, 3 );
		
		if( check.IP_BCDDAY[0] )
		{
			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
			memmove( keybuf->IP_BCDDAY, check.IP_BCDDAY, sizeof(keybuf->IP_BCDDAY) );
		}
		else
			m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
		//データセット

		if( ! m_ChkMnonly ) {
			pDBzm->BcdDateToString( str, check.IP_BCDDAY );
		}
		else {
			BcdMonthToString( str.GetBuffer(64),64, check.IP_BCDDAY );
			str.ReleaseBuffer();
		}
		DBSetDispString( this, getINP_CTL( pn )->IDC_X, str );
	//	DBdata_set( getINP_CTL( pn )->IDC_X, keybuf, CICSInput );

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
//			data_get( getINP_CTL( pn )->IDC_X, &check, CICSInput );
//			if( check.IP_DAY[0] )
//			{
//				m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
//
//				memmove( keybuf->IP_DAY, check.IP_DAY, sizeof(keybuf->IP_DAY) );
//			}
	}
	else if( nChar == VK_DELETE )
	{
		memset( (char *)keybuf, (char)0, sizeof(struct _ItemData) );
//		memset( (char*)keybuf->IP_DAY, (char)0xff, sizeof(keybuf->IP_DAY));
		var.pbVal = (BYTE*)keybuf->IP_BCDDAY;
		DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_BCD, bcd_len );

		m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}


///////////////////////////////////////////////////////////////////////////////
// 伝票番号
void CRpkeyDlg::OnSetFocusRpdenp1() 
{
	try
	{
		set_nowpn( RP_DENP1_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_DENP1_PN ); 	
}

void CRpkeyDlg::OnKillFocusRpdenp1( long inplen ) 
{
	DenpInp( RP_DENP1_PN, &m_Rpline.RP_DENP1_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpdenp1(long nChar, long inplen, long kst) 
{
	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = DenpInp( RP_DENP1_PN, &m_Rpline.RP_DENP1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_DENP1_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

void CRpkeyDlg::OnSetFocusRpdenp2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	try
	{
		set_nowpn( RP_DENP2_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_DENP2_PN );	
}

void CRpkeyDlg::OnKillFocusRpdenp2( long inplen ) 
{
	DenpInp( RP_DENP2_PN, &m_Rpline.RP_DENP2_DT, 0, inplen, 0 );	
}

void CRpkeyDlg::OnTerminationRpdenp2(long nChar, long inplen, long kst) 
{
	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = DenpInp( RP_DENP2_PN, &m_Rpline.RP_DENP2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_DENP2_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}

// 伝票番号　入力＆オフフォーカス
long CRpkeyDlg::DenpInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	VARIANT var;

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		if( var.pbVal[0] != 0 )	check.IP_DENP = atoi( (LPCTSTR)var.pbVal );
		else					check.IP_DENP = -1;

		if( check.IP_DENP != -1 )
		{
			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
			keybuf->IP_DENP = check.IP_DENP;
		}
		else
			m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;

		//データセット
	//	data_set( getINP_CTL( pn )->IDC_X, keybuf, CICSInput );
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
//			data_get( getINP_CTL( pn )->IDC_X, &check, CICSInput );
//			if( check.IP_DENP != 0xffff )
//			{
//				m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
//	
//				keybuf->IP_DENP = check.IP_DENP;
//			}
	}
	else if( nChar == VK_DELETE )
	{
		check.IP_DENP = -1;
		var.pbVal = NULL;
		DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	
		keybuf->IP_DENP = check.IP_DENP;
		m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}


//////////////////////////////////////////////////////////////////////////////////////
// 借方部門

void CRpkeyDlg::RpbmonFocusSub( int pn )
{
	try
	{
		// 部門名称セット
		set_etcdsp( pn );
		// 部門名称表示
		dsp_etc( pn, TRUE );	// 今回ポジションを表示

		set_nowpn( pn );

		BmonKojiSelDisp( pn );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}

void CRpkeyDlg::OnSetFocusRpbmon1() 
{
	RpbmonFocusSub( RP_BMON1_PN );
}
void CRpkeyDlg::OnKillFocusRpbmon1( long inplen ) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	BmonInp( RP_BMON1_PN, &m_Rpline.RP_BMON1_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpbmon1(long nChar, long inplen, long kst) 
{
	try {
		// 入力＆オフフォーカス
		if( !(nChar = BmonInp( RP_BMON1_PN, &m_Rpline.RP_BMON1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_BMON1_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}

void CRpkeyDlg::OnSetFocusRpbmon2() 
{
	RpbmonFocusSub( RP_BMON2_PN );
}
void CRpkeyDlg::OnKillFocusRpbmon2( long inplen ) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	BmonInp( RP_BMON2_PN, &m_Rpline.RP_BMON2_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpbmon2(long nChar, long inplen, long kst) 
{
	try {
		// 入力＆オフフォーカス
		if( !(nChar = BmonInp( RP_BMON2_PN, &m_Rpline.RP_BMON2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_BMON2_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}

//////////////////////////////////////////////////////////////////////////////////////
// 貸方部門
void CRpkeyDlg::OnSetFocusRpcbmn1() 
{
	RpbmonFocusSub( RP_CBMN1_PN );
}

void CRpkeyDlg::OnKillFocusRpcbmn1( long inplen ) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	BmonInp( RP_CBMN1_PN, &m_Rpline.RP_CBMN1_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpcbmn1(long nChar, long inplen, long kst) 
{
	try {
		// 入力＆オフフォーカス
		if( !(nChar = BmonInp( RP_CBMN1_PN, &m_Rpline.RP_CBMN1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_CBMN1_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}

void CRpkeyDlg::OnSetFocusRpcbmn2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	RpbmonFocusSub( RP_CBMN2_PN );
}

void CRpkeyDlg::OnKillFocusRpcbmn2( long inplen ) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	BmonInp( RP_CBMN2_PN, &m_Rpline.RP_CBMN2_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpcbmn2(long nChar, long inplen, long kst) 
{
	try {
		// 入力＆オフフォーカス
		if( !(nChar = BmonInp( RP_CBMN2_PN, &m_Rpline.RP_CBMN2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_CBMN2_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}

// 部門　入力＆オフフォーカス
long CRpkeyDlg::BmonInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	CDBINPDataRec temp;	// 部門コード取得用
	int number = 0;
	int inp = 0;
	int ret = 0;
	char str[60];
	long nKey;
	VARIANT var;
	ZmselDBData	zd;

	if( inplen ) {
		//データリード
		DBdata_get( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		inp = strlen( (char*)var.pbVal );
		number = atoi( (char*)var.pbVal );
		check.IP_BMN = -1;

		// 画面番号優先のときのみ 2桁 で 画面番号選択
		nKey = (pAUTOSEL->BMNSEL_OPT == OPT_BMNSEL) ? VK_RETURN : VK_TAB;

		if( inp && inp < 3 && nKey == VK_RETURN ) {
			pZmSel->InputData( 1, number, -1, ZSEL_BUMON );
			pZmSel->ResultData( &zd );

			if( zd.errflg != 0 ) {
				// 選択ミス//データセット(再表示)
				check.IP_BMN = keybuf->IP_BMN;
				Buzzer();
			}
			else {
				check.IP_BMN = zd.bmn;
			}
		}
		else {
			if( strlen( (char*)var.pbVal ) )
			{
				check.IP_BMN = atoi( (char*)var.pbVal );

				if( pDBzm->IsGokeiBumon( check.IP_BMN ) ) {
					ICSMessageBox(_T("合計部門は入力できません！"), 0,0,0,this);
					nChar = 0;
					check.IP_BMN = keybuf->IP_BMN;
				}
			}
		}

		if( check.IP_BMN != -1 ) {
			pDBzm->BumonCodeToStr( str, sizeof str, check.IP_BMN );
			var.pbVal = (BYTE*)str;
		}
		else	var.pbVal = NULL;
		DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		check.IP_BMN = -1;
		if( strlen( (char*)var.pbVal ) ) {
			check.IP_BMN = atoi( (char*)var.pbVal );
		}

		if( check.IP_BMN != -1)
		{
			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
			keybuf->IP_BMN = check.IP_BMN;
		}
		else {
			keybuf->IP_BMN = -1;
			m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
		}
			
		//データセット
		if( keybuf->IP_BMN != -1 ) {
			pDBzm->BumonCodeToStr( str, sizeof str, keybuf->IP_BMN );
			var.pbVal = (BYTE*)str;
		}
		else	var.pbVal = NULL;

		DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		dsp_etc( pn, FALSE );	// 今回ポジションを非表示

		return( ret );
	}
	else
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return VK_F2;

		// ページアップ／ダウン
		if( nChar == VK_PRIOR ) {
			pZmSel->PageUp();
		}
		else if( nChar == VK_NEXT ) {
			pZmSel->PageDown();
		}
		else if( nChar == VK_HOME ) {
			pZmSel->PageChange();
		}
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{
		if( nChar == VK_TAB ) {		// 選択画面より番号選択
			if( inp > 0 ) {

				pZmSel->InputData( 1, number, -1, ZSEL_BUMON );
				pZmSel->ResultData( &zd );

				// 番号から、部門選択
				if( zd.errflg != 0 ) {
					// 選択ミス//データセット(再表示)
					check.IP_BMN = keybuf->IP_BMN;
					Buzzer();
				}
				else {
					keybuf->IP_BMN = zd.bmn;
					check.IP_BMN = zd.bmn;
				}

				if( check.IP_BMN != -1 ) {
					pDBzm->BumonCodeToStr( str, sizeof str, check.IP_BMN );
					var.pbVal = (BYTE*)str;
				}
				else	var.pbVal = NULL;
				DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
		}
	}
	else if( nChar == VK_DELETE )
	{
		var.pbVal = NULL;
		keybuf->IP_BMN = -1;

		m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
		DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	return( nChar );
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 借方工事

void CRpkeyDlg::RpKojiFocusSub( int pn )
{
	try
	{
		// 工事名称セット
		set_etcdsp( pn );
		// 工事名称表示
		dsp_etc( pn, TRUE );	// 今回ポジションを表示

		set_nowpn( pn );		

		BmonKojiSelDisp( pn );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


void CRpkeyDlg::OnSetFocusRpkoji1() 
{
	RpKojiFocusSub( RP_KOJI1_PN );
}
void CRpkeyDlg::OnKillFocusRpkoji1( long inplen ) 
{
	KojiInp( RP_KOJI1_PN, &m_Rpline.RP_KOJI1_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpkoji1(long nChar, long inplen, long kst) 
{
	try {
		// 入力＆オフフォーカス
		if( !(nChar = KojiInp( RP_KOJI1_PN, &m_Rpline.RP_KOJI1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_KOJI1_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}

void CRpkeyDlg::OnSetFocusRpkoji2() 
{
	RpKojiFocusSub( RP_KOJI2_PN );
}
void CRpkeyDlg::OnKillFocusRpkoji2( long inplen ) 
{
	KojiInp( RP_KOJI2_PN, &m_Rpline.RP_KOJI2_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpkoji2(long nChar, long inplen, long kst) 
{
	try {
		// 入力＆オフフォーカス
		if( !(nChar = KojiInp( RP_KOJI2_PN, &m_Rpline.RP_KOJI2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_KOJI2_PN, nChar ) != 0 )
			Buzzer();


	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 貸方工事
void CRpkeyDlg::OnSetFocusRpckji1() 
{
	RpKojiFocusSub( RP_CKJI1_PN );
}
void CRpkeyDlg::OnKillFocusRpckji1( long inplen ) 
{
	KojiInp( RP_CKJI1_PN, &m_Rpline.RP_CKJI1_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpckji1(long nChar, long inplen, long kst) 
{
	try {
		// 入力＆オフフォーカス
		if( !(nChar = KojiInp( RP_CKJI1_PN, &m_Rpline.RP_CKJI1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_CKJI1_PN, nChar ) != 0 )
			Buzzer();
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}

void CRpkeyDlg::OnSetFocusRpckji2() 
{
	RpKojiFocusSub( RP_CKJI2_PN );
}
void CRpkeyDlg::OnKillFocusRpckji2( long inplen ) 
{
	KojiInp( RP_CKJI2_PN, &m_Rpline.RP_CKJI2_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpckji2(long nChar, long inplen, long kst) 
{
	try {
		// 入力＆オフフォーカス
		if( !(nChar = KojiInp( RP_CKJI2_PN, &m_Rpline.RP_CKJI2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_CKJI2_PN, nChar ) != 0 )
			Buzzer();


	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}

// 工事　入力＆オフフォーカス
long CRpkeyDlg::KojiInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	CDBINPDataRec temp;	// 部門コード取得用
	int number = 0;
	int inp = 0;
	int ret = 0;
	char str[60];
	long nKey;
	VARIANT var;
	ZmselDBData	zd;


	if( inplen ) {
		DBdata_get( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		inp = strlen( (char*)var.pbVal );
		number = atoi( (char*)var.pbVal );
		::ZeroMemory( check.IP_KOJI, sizeof check.IP_KOJI );

		// 画面番号優先のときのみ 2桁 で 画面番号選択
		nKey = (pAUTOSEL->BMNSEL_OPT == OPT_BMNSEL) ? VK_RETURN : VK_TAB;

		if( inp && inp < 3 && nKey == VK_RETURN ) {

			pZmSel->InputData( 1, number, -1, ZSEL_KOJI );
			pZmSel->ResultData( &zd );

			if( zd.errflg != 0 ) {
				// 選択ミス//データセット(再表示)
				memcpy( check.IP_KOJI, keybuf->IP_KOJI, sizeof check.IP_KOJI );
				Buzzer();
			}
			else {
				memcpy( check.IP_KOJI, zd.koji, sizeof check.IP_KOJI );
			}
		}
		else {
			if( inp > 0 ) {
				strcpy_s( (char*)check.IP_KOJI, sizeof check.IP_KOJI, (char*)var.pbVal );
			}
		}

		if( check.IP_KOJI[0] != '\0' ) {
			pDBzm->KojiCodeToStr( str, sizeof str, CString(check.IP_KOJI) );
			var.pbVal = (BYTE*)str;
		}
		else	var.pbVal = NULL;
		DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		::ZeroMemory( &check.IP_KOJI, sizeof check.IP_KOJI );
		if( strlen( (char*)var.pbVal ) ) {
			strcpy_s( (char*)&check.IP_KOJI, sizeof check.IP_KOJI, (char*)var.pbVal );
		}

		if( check.IP_KOJI[0] != 0 )
		{
			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
			strcpy_s( (char*)keybuf->IP_KOJI, sizeof keybuf->IP_KOJI, (char*) check.IP_KOJI );
		}
		else {
			m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
			::ZeroMemory( keybuf->IP_KOJI, sizeof keybuf->IP_KOJI );
		}
			
		//データセット
		if( keybuf->IP_KOJI[0] != 0 ) {
			var.pbVal = (BYTE*)keybuf->IP_KOJI;
		}
		else	var.pbVal = NULL;

		DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		dsp_etc( pn, FALSE );	// 今回ポジションを非表示

		return( ret );
	}
	else
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return VK_F2;

		// ページアップ／ダウン
		if( nChar == VK_PRIOR ) {
			pZmSel->PageUp();
		}
		else if( nChar == VK_NEXT ) {
			pZmSel->PageDown();
		}
		else if( nChar == VK_HOME ) {
			pZmSel->PageChange();
		}
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{
		if( nChar == VK_TAB ) {		// 選択画面より番号選択
			if( inp > 0 ) {

				pZmSel->InputData( 1, number, -1, ZSEL_KOJI );
				pZmSel->ResultData( &zd );

				if( zd.errflg != 0 ) {
					// 選択ミス//データセット(再表示)
					memcpy( check.IP_KOJI, keybuf->IP_KOJI, sizeof check.IP_KOJI );
					Buzzer();
				}
				else {
					memcpy( keybuf->IP_KOJI, zd.koji, sizeof keybuf->IP_KOJI );
					memcpy( check.IP_KOJI, zd.koji, sizeof check.IP_KOJI );
				}

				if( check.IP_KOJI[0] != 0 ) {
					pDBzm->KojiCodeToStr( str, sizeof str, CString(check.IP_KOJI) );
					var.pbVal = (BYTE*)str;
				}
				else	var.pbVal = NULL;
				DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
		}
	}
	else if( nChar == VK_DELETE )
	{
		var.pbVal = NULL;
		::ZeroMemory( keybuf->IP_KOJI, sizeof keybuf->IP_KOJI );

		m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
		DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	return( nChar );
}


///////////////////////////////////////////////////////////////////////////////////////
// 借方科目
void CRpkeyDlg::OnSetFocusRpDebt1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	try
	{
		set_nowpn( RP_DEBT1_PN );

		KamokuSelDisp( RP_DEBT1_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_DEBT1_PN );
}

void CRpkeyDlg::OnKillFocusRpDebt1( long inplen ) 
{
	ItemInp( RP_DEBT1_PN, &m_Rpline.RP_DEBT1_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpDebt1(long nChar, long inplen, long kst )
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	try
	{
		// 入力＆オフフォーカス
		nChar = ItemInp( RP_DEBT1_PN, &m_Rpline.RP_DEBT1_DT, nChar, inplen, kst );
		if( nChar == -1 )
		{
			Buzzer();
			return;
		}
		else if( !nChar )
			return;

		// 財務科目選択用ターミネーション
		nChar = KamokuSelTermination( RP_DEBT1_PN, nChar, kst );

		//カーソル移動
		if( term_focus( RP_DEBT1_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


void CRpkeyDlg::OnSetFocusRpDebt2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	try
	{
		set_nowpn( RP_DEBT2_PN );

		KamokuSelDisp( RP_DEBT2_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_DEBT2_PN );
}

void CRpkeyDlg::OnKillFocusRpDebt2( long inplen ) 
{
	ItemInp( RP_DEBT2_PN, &m_Rpline.RP_DEBT2_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpDebt2(long nChar, long inplen, long kst )
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	try
	{
		// 入力＆オフフォーカス
		nChar = ItemInp( RP_DEBT2_PN, &m_Rpline.RP_DEBT2_DT, nChar, inplen, kst );
		if( nChar == -1 )
		{
			Buzzer();
			return;
		}
		else if( !nChar )
			return;

		// 財務科目選択用ターミネーション
		nChar = KamokuSelTermination( RP_DEBT2_PN, nChar, kst );

		//カーソル移動
		if( term_focus( RP_DEBT2_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// 貸方科目
void CRpkeyDlg::OnSetFocusRpCred1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	try
	{
		set_nowpn( RP_CRED1_PN );

		KamokuSelDisp( RP_DEBT1_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_CRED1_PN );
}

void CRpkeyDlg::OnKillFocusRpCred1( long inplen ) 
{
	ItemInp( RP_CRED1_PN, &m_Rpline.RP_CRED1_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpCred1(long nChar, long inplen, long kst )
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	try
	{
		// 入力＆オフフォーカス
		nChar = ItemInp( RP_CRED1_PN, &m_Rpline.RP_CRED1_DT, nChar, inplen, kst );
		if( nChar == -1 )
		{
			Buzzer();
			return;
		}
		else if( !nChar )
			return;

		// 財務科目選択用ターミネーション
		nChar = KamokuSelTermination( RP_CRED1_PN, nChar, kst );

		//カーソル移動
		if( term_focus( RP_CRED1_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

// 貸方科目 後
void CRpkeyDlg::OnSetFocusRpCred2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	try
	{
		set_nowpn( RP_CRED2_PN );

		KamokuSelDisp( RP_DEBT2_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_CRED2_PN );
}

void CRpkeyDlg::OnKillFocusRpCred2( long inplen ) 
{
	ItemInp( RP_CRED2_PN, &m_Rpline.RP_CRED2_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpCred2( long nChar, long inplen, long kst )
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	try
	{
		// 入力＆オフフォーカス
		nChar = ItemInp( RP_CRED2_PN, &m_Rpline.RP_CRED2_DT, nChar, inplen, kst );
		if( nChar == -1 )
		{
			Buzzer();
			return;
		}
		else if( !nChar )
			return;

		// 財務科目選択用ターミネーション
		nChar = KamokuSelTermination( RP_CRED2_PN, nChar, kst );

		//カーソル移動
		if( term_focus( RP_CRED2_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

//
// 科目カナ検索
BOOL CRpkeyDlg::ItemKanaSearch(int pn, long data) 
{

	return FALSE;
}

//
// 科目チェック
long CRpkeyDlg::ItemDataCheck(int pn, long nChar, LPCTSTR string, int dcsw ) 
{
	struct _KamokuData check = {0}, ans = {0};
	struct _DBKNREC* pKn;
	// 科目コード選択
	int sgn = 0;
	int set_ok = 0;

	check.kd_eda = -1;
	KamokuStr_ChkLength( string, &check, Voln1->edcol );

	// ピリオド＋枝番の対応
	if( check.kd_sgn == 2 ) {
		DBkamokudata_get( this, getINP_CTL( pn )->IDC_X, &ans, pDBzm);

		if( ans.kd_code[0] ) {
			ans.kd_eda = check.kd_eda;
			DBkamokudata_set( this, getINP_CTL( pn )->IDC_X, &ans, pDBzm);
			set_ok = 1;
		}
		else {
			if( BRmst ) {
				ans.kd_eda = check.kd_eda;
				strcpy_s( ans.kd_name, sizeof ans.kd_name, "枝　番" );
				DBkamokudata_set( this, getINP_CTL( pn )->IDC_X, &ans, pDBzm);
				set_ok = 1;
			}
		}
	}
	else {
/*- '09.03.11 -*/
//		ZmselDBData zsel;
//
//		pZmSel->InputData( sgn, atoi( check.kd_code ), check.kd_eda, ZSEL_KAMOKU );
//
//		pZmSel->ResultData( &zsel );
//
//		if( zsel.errflg != -1 ) {
//			pKn = pDBzm->PjisToKnrec( zsel.dwkcode );
//			if( pKn && !(pKn->knvoid & 0x01) ) {
//				strcpy_s( ans.kd_code, sizeof ans.kd_code, zsel.kcode );
//				ans.kd_eda = zsel.edaban;
//				memcpy( ans.kd_name, pKn->knnam, sizeof pKn->knnam );
//
//				DBkamokudata_set( this, getINP_CTL( pn )->IDC_X, &ans, pDBzm);
//				set_ok = 1;
//			}
//		}
/*-------------*/
		ZmselDBData zsel;

		short sgn;
		short mode = ZSEL_KMINPCD;
		BOOL bKcdDisp = FALSE;

		if( pZmSel->GetCrntMode() == ZSEL_BRNTKY )	mode = ZSEL_BRNTKY;

		if( pAUTOSEL->KMNO_OPT == OPT_KMNO ) {
			bKcdDisp = TRUE;
		}

		if( mode ==  ZSEL_KMINPCD ) {
			if( bKcdDisp )	sgn = 0;
			else {
				if( check.kd_sgn == 1 || check.kd_sgn == 5 )
						sgn = 1;
				else	sgn = 0;
			}
		}
		else {
			// 枝番選択
			sgn = 1;
		}

		pZmSel->InputData( sgn, atoi( check.kd_code ), check.kd_eda, mode );

		pZmSel->ResultData( &zsel );

		if( zsel.errflg == 0 ) {
			pKn = pDBzm->PjisToKnrec( zsel.dwkcode );
			if( pKn && !(pKn->knvoid & 0x01) ) {
				strcpy_s( ans.kd_code, sizeof ans.kd_code, zsel.kcode );
				ans.kd_eda = zsel.edaban;
				memcpy( ans.kd_name, pKn->knnam, sizeof pKn->knnam );

				DBkamokudata_set( this, getINP_CTL( pn )->IDC_X, &ans, pDBzm);
				set_ok = 1;
			}
		}
		else{
#ifdef CLOSE //Zmsel で検索するので不要(09.20 /22)
			// 科目５０音選択による処理が必要！
			// 枝番入力ありの時には？？？
			KSEL_PAR kpar;
			CRect rc;
			GetDlgItem(getINP_CTL( pn )->IDC_X)->GetWindowRect( rc );

			pDBzm->knrec->Requery("", 0);

			kpar.ksPsgn	= KSP_LEFTTOP;
			kpar.ksFsgn = 0;
			kpar.ksLnum = 10;

			::ZeroMemory( kpar.ksKana, sizeof kpar.ksKana );
			strncpy_s( kpar.ksKana, sizeof kpar.ksKana, check.kd_code, sizeof kpar.ksKana -1 );

			kpar.ksKrng = 0;
			kpar.ksPos.x	= rc.right;
			kpar.ksPos.y	= rc.top;
			kpar.ksCwnd = this;
			kpar.ksStat = 0;
			kpar.ksFhwd = /*this->m_hWnd*/m_Ok.m_hWnd;

			if( Kmk50onSelect(pDBzm, &kpar, this) ) {
				memmove( (char *)ans.kd_name, (LPCTSTR)pDBzm->knrec->knnam, KMNLEN );
				strcpy_s( ans.kd_code, sizeof ans.kd_code, pDBzm->knrec->kncod );
				ans.kd_eda = -1;

				DBkamokudata_set( this, getINP_CTL( pn )->IDC_X, &ans, pDBzm);
				set_ok = 1;
			}
#endif
		}
/*-------------*/
	}

	if( !set_ok ) {
		Buzzer();

		memset( (char *)&ans, (char)0, sizeof(ans) );
		ans.kd_eda = -1;
		DBkamokudata_set( this, getINP_CTL( pn )->IDC_X, &ans, pDBzm );
		return 0;
	}
	return nChar;
}

//
// 科目　入力＆オフフォーカス
long CRpkeyDlg::ItemInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;

	// オフフォーカス
//	if( !nChar )
//	{
		//データリード
		if( nChar == 0x16 ) {
			DBkamokudata_set( this, getINP_CTL( pn )->IDC_X, &keybuf->IP_stKMK, pDBzm );
		}

		DBkamokudata_get( this, getINP_CTL( pn )->IDC_X, &check.IP_stKMK, pDBzm);

		if( check.IP_CdNum[0] == 0 && check.IP_BRN == -1 )
		{
			DBkamokudata_set( this, getINP_CTL( pn )->IDC_X, &keybuf->IP_stKMK, pDBzm );
			// Shift + Tab
			if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
				nChar = VK_F2;

			return (inplen == 0) ? nChar : (-1);
		}
		else if( check.IP_CdNum || check.IP_BRN != -1 )
		{
			if( check.IP_CdNum[0] == 0 && check.IP_BRN != -1 ) {
				sprintf_s( check.IP_KNAM, sizeof check.IP_KNAM, "%.14s", "枝　番");
			}

			memmove( keybuf, &check, sizeof(struct _ItemData));
			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
		}
		else
			m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;

		//データセット
		DBkamokudata_set( this, getINP_CTL( pn )->IDC_X, &keybuf->IP_stKMK, pDBzm );
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
	if( nChar )
	{
	//	nChar = ((CDBDinpView*)p_DBDinpView)->SelItemDsp( nChar );
	//	if( !nChar )
	//		return(0);
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{

	}
	else if( nChar == VK_DELETE )
	{
		memset( (char*)keybuf, (char)0, sizeof(struct _ItemData) );
		keybuf->IP_BRN = -1;
		DBkamokudata_set( this, getINP_CTL( pn )->IDC_X, &keybuf->IP_stKMK, pDBzm );		

		m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}

///////////////////////////////////////////////////////////////////////////////////////////
///	金額
// 開始
void CRpkeyDlg::OnSetFocusRpval1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	try
	{
		set_nowpn( RP_VAL1_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_VAL1_PN );
}

void CRpkeyDlg::OnKillFocusRpval1( long inplen ) 
{
	 ValInp( RP_VAL1_PN, &m_Rpline.RP_VAL1_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpval1(long nChar, long inplen, long kst ) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = ValInp( RP_VAL1_PN, &m_Rpline.RP_VAL1_DT, nChar, inplen, kst )) )
			return;

		//カーソル移動
		if( term_focus( RP_VAL1_PN, nChar ) != 0 )
			Buzzer();
		
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

// 終了
void CRpkeyDlg::OnSetFocusRpval2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	try
	{
		set_nowpn( RP_VAL2_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_VAL2_PN );
}

void CRpkeyDlg::OnKillFocusRpval2( long inplen ) 
{
	 ValInp( RP_VAL2_PN, &m_Rpline.RP_VAL2_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpval2(long nChar, long inplen, long kst ) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = ValInp( RP_VAL2_PN, &m_Rpline.RP_VAL2_DT, nChar, inplen, kst )) )
			return;

		//カーソル移動
		if( term_focus( RP_VAL2_PN, nChar ) != 0 )
			Buzzer();


	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

//
// 金額　入力＆オフフォーカス
long CRpkeyDlg::ValInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst )
{
	struct _ItemData check = {0};
	VARIANT var;
	char	chkbf[20] = {0};

	if( inplen || nChar == 0x16 ) {
		l_defn( 0x16 );
		DBdata_get( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
		memcpy( check.IP_VAL, var.pbVal, sizeof check.IP_VAL );
		memmove( keybuf, &check, sizeof(check) );

		if( inplen < 0 && memcmp( check.IP_VAL, chkbf, 6 ) == 0 ) {
			// BacsSpace による削除
			var.pbVal = NULL;
			DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
		}
		else {
			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
		}
	}
	else if (inplen == 0) { //電卓からの場合、inplen == 0 で来るので金額をチェックする。
		l_defn(0x16);
		DBdata_get(this, getINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0);
		memcpy(check.IP_VAL, var.pbVal, sizeof check.IP_VAL);

		if (memcmp(keybuf->IP_VAL, check.IP_VAL, 6) != 0) {
			memcpy(keybuf, &check, sizeof(check));
			m_Rpline.LINE_CND[pn].INP_sg = TRUE;
		}
	}

	// オフフォーカス
	if( !nChar )
	{

		//データリード
//		if( inplen > 0 )
//		{
//			DBdata_get( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
//			memcpy( check.IP_VAL, var.pbVal, sizeof check.IP_VAL );
//			memmove( keybuf, &check, sizeof(check) );
//			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
//		}
//		else
//			m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;

		//データセット
		if( m_Rpline.LINE_CND[ pn ].INP_sg == TRUE )
		{
			var.pbVal = (BYTE*)keybuf->IP_VAL;
			DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
		}
		else
		{
			var.pbVal = NULL;
			DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
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
//		if( ((CEdit*)GetDlgItem( getINP_CTL( pn )->IDC_X ))->LineLength() )
//		{
//			l_defn( 0x16 );
//			data_get( getINP_CTL( pn )->IDC_X, &check, CICSInput );
//			memmove( keybuf->IP_VAL, check.IP_VAL, sizeof(keybuf->IP_VAL) );
//			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
//		}
	}
	else if( nChar == VK_DELETE )
	{
		var.pbVal = NULL;
		DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
		memset( (char *)keybuf->IP_VAL, (char)0, sizeof(keybuf->IP_VAL) );
		m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}

/////////////////////////////////////////////////////////////////////////////////////
// 摘要

void CRpkeyDlg::OnSetFocusRptky1() 
{
	CString debt, cred;

	try
	{
		if( get_nowpn() != RP_TKY1_PN )
		{
			set_nowpn( RP_TKY1_PN );

			if( m_Rpline.LINE_CND[RP_TKY1_PN].IMG_sg )
				SetFocusToImage( RP_TKY1_PN );
			else {
/*
				// 連想摘要検索＆表示
				debt_inp_sg = m_Rpline.LINE_CND[RP_DEBT1_PN].INP_sg;
				cred_inp_sg = m_Rpline.LINE_CND[RP_CRED1_PN].INP_sg;
				if( debt_inp_sg || cred_inp_sg )
				{
					if( debt_inp_sg )
						debt = m_Rpline.RP_DEBT1_DT.IP_CdNum;
					if( cred_inp_sg )
						cred = m_Rpline.RP_CRED1_DT.IP_CdNum;
				}
				// 連想摘要を表示
*/
			}
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_TKY1_PN );
}


void CRpkeyDlg::OnKillFocusRptky1( long inplen ) 
{
	OnTerminationRptky1( 0, inplen, 0 ); 
}

void CRpkeyDlg::OnKanaTermRptky1(LPCSTR data) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	struct _ItemData dmy_data = {0};
	CDBipTKREC tkrec;

	try
	{
#ifdef CLOSE
		//５０音摘要表示
		if( ((CDBDinpView*)p_DBDinpView)->tky_select50_dsp( (char*)data ) )
		{
		//	data_reply( getINP_CTL( RP_TKY_PN )->IDC_X, 0, &dmy_data, CICSInput );
			return;
		}

		//摘要選択
		if( ((CDBDinpView*)p_DBDinpView)->tky_select( &tkrec, (char *)data ) != -1 )
		{
			tky_select_dsp( &tkrec, TRUE );
		}
		else
		{
			Buzzer();
		//	data_reply( getINP_CTL( RP_TKY_PN )->IDC_X, -1, &dmy_data, CICSInput );
		}
#endif

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

// 摘要２
void CRpkeyDlg::OnSetFocusRptky2() 
{
	try
	{
		if( get_nowpn() != RP_TKY2_PN )
		{
			set_nowpn( RP_TKY2_PN );
		}

		if( m_Rpline.LINE_CND[RP_TKY2_PN].IMG_sg )
			SetFocusToImage( RP_TKY2_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_TKY2_PN );
}


void CRpkeyDlg::OnKillFocusRptky2( long inplen ) 
{
	OnTerminationRptky2( 0, inplen, 0 ); 
}

void CRpkeyDlg::OnKanaTermRptky2(LPCSTR data) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	struct _ItemData dmy_data = {0};
	CDBipTKREC tkrec;

	try
	{
#ifdef CUT_CLOSE
		//５０音摘要表示
		if( ((CDBDinpView*)p_DBDinpView)->tky_select50_dsp( (char*)data ) )
		{
		//	data_reply( getINP_CTL( RP_TKY_PN )->IDC_X, 0, &dmy_data, CICSInput );
			return;
		}

		//摘要選択
		if( ((CDBDinpView*)p_DBDinpView)->tky_select( &tkrec, (char *)data ) != -1 )
		{
			tky_select_dsp( &tkrec, TRUE );
		}
		else
		{
			Buzzer();
		//	data_reply( getINP_CTL( RP_TKY_PN )->IDC_X, -1, &dmy_data, CICSInput );
		}
#endif
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


/*==========================
	摘要セットデータ表示
===========================*/
void CRpkeyDlg::tky_select_dsp( CDBipTKREC *ptk, BOOL F12_KEY )
{
	struct _ItemData check={0};
	struct _ItemData ins_data={0};
	char fmt[10];

	//仕訳摘要表示
//	sprintf( fmt, "%%.%ds", sizeof( ptk->TKnam ) );
	strcpy_s( fmt, sizeof fmt, "%s" );
	sprintf_s( (char *)ins_data.IP_TKY, sizeof ins_data.IP_TKY, fmt, ptk->m_tkname );
	if( F12_KEY ) {
/*--*///	data_reply( getINP_CTL( RP_TKY_PN )->IDC_X, 1, &ins_data, CICSInput );
	}
	else
	{
		//データセットのためのダミーフォーカスＯＦＦ
		OnTerminationRptky1( 0, 0, 0 );
/*--*///	((CICSDBEDT*)GetDlgItem( getINP_CTL( RP_TKY_PN )->IDC_X ))->TekiInsert( (char*)ins_data.IP_TKY );
	}
}

void CRpkeyDlg::OnTerminationRptky1(long nChar, long inplen, long kst) 
{
	char pbuf[128];
	VARIANT var;
	struct _ItemData check={0};
	int i, tkyID, del_pos;

	tkyID = getINP_CTL( RP_TKY1_PN )->IDC_X;

	try
	{
		// オフフォーカス
		if( !nChar )
		{
			//データリード
			if( m_Rpline.LINE_CND[ RP_TKY1_PN ].IMG_sg )
				return;

		//	pos = ((CICSDBEDT*)GetDlgItem( tkyID )->GetCaretPosition();

			DBdata_get( this, tkyID, &var, ICSDBEDT_TYPE_STRING, 0 );
		//	DBdata_set( this, tkyID, &var, ICSDBEDT_TYPE_STRING, 0 );
		//	GetDlgItem( tkyID  )->GetWindowText( (char*)check.IP_TKY, (Voln1->TK_LN * 2 + 1) );
		//	((CICSDBEDT*)GetDlgItem( tkyID )->SetCaretPosition( pos );

			//データセット
			i = sprintf_s( pbuf, sizeof pbuf, "%.80s", var.pbVal /*check.IP_TKY*/ );
		//	i = get_inpl( (unsigned char*)pbuf, i/2 );
			i = get_inpl( (unsigned char*)pbuf, Voln1->tk_ln );	// 02.09 /02 -ANKINP-
			if( i )
			{
				memmove( m_Rpline.RP_TKY1_DT.IP_TKY, pbuf, i );
				m_Rpline.RP_TKY1_DT.IP_TKY[i] = '\0';
				m_Rpline.LINE_CND[ RP_TKY1_PN ].INP_sg = TRUE;
			}
			else
			{
				var.pbVal = NULL;
				DBdata_set( this, getINP_CTL( RP_TKY1_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				::ZeroMemory( m_Rpline.RP_TKY1_DT.IP_TKY, sizeof m_Rpline.RP_TKY1_DT.IP_TKY );
				m_Rpline.LINE_CND[ RP_TKY1_PN ].INP_sg = FALSE;
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

		/*---VER2----*/
		// ページアップ／ダウン
		if( nChar == VK_PRIOR ) {
			pZmSel->PageUp();
		}
		else if( nChar == VK_NEXT ) {
			pZmSel->PageDown();
		}
		else if( nChar == VK_HOME ) {
			pZmSel->PageChange();
		}
		/*---VER2----*/
		//
		else if( nChar == VK_DELETE )
		{
			//カーソルポジション以降の文字列取り消し
			DBdata_get( this, tkyID, &var, ICSDBEDT_TYPE_STRING, 0 );

			del_pos = ((CICSDBEDT*)GetDlgItem( tkyID ))->GetCaretPosition();
			if( del_pos > (Voln1->tk_ln*2) || del_pos < 0 )	del_pos = 0;

			strcpy_s( pbuf, sizeof pbuf, (const char*)var.pbVal );
			pbuf[del_pos] = '\0';

			var.pbVal = (BYTE*)pbuf;
			DBdata_set( this, tkyID, &var, ICSDBEDT_TYPE_STRING, 0 );

			::ZeroMemory( m_Rpline.RP_TKY1_DT.IP_TKY, sizeof m_Rpline.RP_TKY1_DT.IP_TKY );
			memcpy( m_Rpline.RP_TKY1_DT.IP_TKY, pbuf, del_pos );

			int len = strlen( pbuf );
			m_Rpline.LINE_CND[ RP_TKY1_PN ].INP_sg = len > 0 ? TRUE : FALSE;
			nChar = 0;
		}
		else if( nChar == VK_TAB ) {
			// 入力ずみ 摘要ダイアログを表示
			if( DispTekiyoImage( RP_TKY1_PN ) != IDOK )
				nChar = 0;
		}

		//カーソル移動
		if( term_focus( RP_TKY1_PN, nChar ) != 0 ) {
			if( nChar != VK_F12 )
				Buzzer();
		}

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

// 摘要２
void CRpkeyDlg::OnTerminationRptky2(long nChar, long inplen, long kst) 
{
	char pbuf[128];
	VARIANT var;
	struct _ItemData check={0};
	int i, tkyID, del_pos;
	tkyID = getINP_CTL( RP_TKY2_PN )->IDC_X;

	try
	{
		// オフフォーカス
		if( !nChar )
		{
			//データリード
			if( m_Rpline.LINE_CND[ RP_TKY2_PN ].IMG_sg )
				return;

		//	pos = ((CICSDBEDT*)GetDlgItem( tkyID )->GetCaretPosition();

			DBdata_get( this, tkyID, &var, ICSDBEDT_TYPE_STRING, 0 );
		//	DBdata_set( this, tkyID, &var, ICSDBEDT_TYPE_STRING, 0 );
		//	GetDlgItem( tkyID  )->GetWindowText( (char*)check.IP_TKY, (Voln1->TK_LN * 2 + 1) );

		//	((CICSDBEDT*)GetDlgItem( tkyID )->SetCaretPosition( pos );

			//データセット
			i = sprintf_s( pbuf, sizeof pbuf, "%.80s", var.pbVal /*check.IP_TKY*/ );
		//	i = get_inpl( (unsigned char*)pbuf, i/2 );
			i = get_inpl( (unsigned char*)pbuf, Voln1->tk_ln );	// 02.09 /02 -ANKINP-
			if( i )
			{
				memmove( m_Rpline.RP_TKY2_DT.IP_TKY, pbuf, i );
				m_Rpline.RP_TKY2_DT.IP_TKY[i] = '\0';
				m_Rpline.LINE_CND[ RP_TKY2_PN ].INP_sg = TRUE;
			}
			else
			{
				var.pbVal = NULL;
				DBdata_set( this, getINP_CTL( RP_TKY2_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				::ZeroMemory( m_Rpline.RP_TKY2_DT.IP_TKY, sizeof m_Rpline.RP_TKY2_DT.IP_TKY );
				m_Rpline.LINE_CND[ RP_TKY2_PN ].INP_sg = FALSE;
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

		/*---VER2----*/
		// ページアップ／ダウン
		if( nChar == VK_PRIOR ) {
			pZmSel->PageUp();
		}
		else if( nChar == VK_NEXT ) {
			pZmSel->PageDown();
		}
		else if( nChar == VK_HOME ) {
			pZmSel->PageChange();
		}
		/*---VER2----*/
		//
		else if( nChar == VK_DELETE )
		{
			//カーソルポジション以降の文字列取り消し
			DBdata_get( this, tkyID, &var, ICSDBEDT_TYPE_STRING, 0 );

			del_pos = ((CICSDBEDT*)GetDlgItem( tkyID ))->GetCaretPosition();
			if( del_pos > (Voln1->tk_ln*2) || del_pos < 0 )	del_pos = 0;

			strcpy_s( pbuf, sizeof pbuf, (const char*)var.pbVal );
			pbuf[del_pos] = '\0';

			var.pbVal = (BYTE*)pbuf;
			DBdata_set( this, tkyID, &var, ICSDBEDT_TYPE_STRING, 0 );

			::ZeroMemory( m_Rpline.RP_TKY2_DT.IP_TKY, sizeof m_Rpline.RP_TKY2_DT.IP_TKY );
			memcpy( m_Rpline.RP_TKY2_DT.IP_TKY, pbuf, del_pos );

			int len = strlen( pbuf );
			m_Rpline.LINE_CND[ RP_TKY2_PN ].INP_sg = len > 0 ? TRUE : FALSE;

			nChar = 0;
		}
		else if( nChar == VK_TAB ) {
			// 入力ずみ 摘要ダイアログを表示
			if( DispTekiyoImage( RP_TKY2_PN ) != IDOK )
				nChar = 0;
		}
		
		//カーソル移動
		if( term_focus( RP_TKY2_PN, nChar ) != 0 ) {
			if( nChar != VK_F12 )
				Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


int CRpkeyDlg::DispTekiyoImage( int pn ) 
{
	// 表示位置は、選択部分の下座標を基準に
	if( pZmSel->IsDisplay() )
		pZmSel->DialogOFF();

	RECT rect, rc = {0};
	RECT dlgrect;

	GetDlgItem( getINP_CTL( pn )->IDC_X )->GetWindowRect( &rect );
	GetWindowRect( &dlgrect );

	int img_ret;
	VARIANT		var;
	CSelImgDlg imgdlg(this);

	rc = rect;
	rc.left = dlgrect.left - 70;
	rc.bottom = rc.top;

	imgdlg.Initial( pDBzm, &m_TkyData, rc );

	((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(2);

	img_ret = imgdlg.DoModal();

	((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(0);

	if( img_ret == IDOK ) {
		if( imgdlg.m_bSelTekImage ) {
			// イメージ摘要
			IMREC* pi;
			pDBzm->image->SetAbsolutePosition( imgdlg.m_CImgSeq );
			pi = pDBzm->image->DecodeImage();

			::ZeroMemory( m_Rpline._RPDTA[ pn ].IP_TKY, sizeof m_Rpline._RPDTA[ pn ].IP_TKY );
			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
			m_Rpline.LINE_CND[ pn ].IMG_sg = TRUE;
			m_Rpline._RPDTA[ pn ].IMG_SQ = imgdlg.m_CImgSeq;

			var.pbVal = NULL;
			DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			SetImage( pn, pi );
		}
		else {
			// 文字摘要
			::ZeroMemory( m_Rpline._RPDTA[ pn ].IP_TKY, sizeof m_Rpline._RPDTA[ pn ].IP_TKY );

			strncpy_s( (char*)m_Rpline._RPDTA[ pn ].IP_TKY, sizeof m_Rpline._RPDTA[ pn ].IP_TKY, 
													imgdlg.m_Tekiyo, Voln1->tk_ln*2 );

			var.pbVal = (BYTE*)m_Rpline._RPDTA[ pn ].IP_TKY;
			DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			m_Rpline.LINE_CND[ pn ].INP_sg = m_Rpline._RPDTA[ pn ].IP_TKY[0] ? TRUE : FALSE;
			m_Rpline.LINE_CND[ pn ].IMG_sg = FALSE;
			m_Rpline._RPDTA[ pn ].IMG_SQ = -1;

			OffImage( pn );
		}
	}

	return img_ret;
}


///////////////////////////////////////////////////////////////////////////////
// 手形番号
void CRpkeyDlg::OnSetFocusRpSnum1() 
{

	try
	{
		set_nowpn( RP_SNUM1_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_SNUM1_PN ); 	
}

void CRpkeyDlg::OnKillFocusRpSnum1( long inplen ) 
{
	SnumInp( RP_SNUM1_PN, &m_Rpline.RP_SNUM1_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpSnum1(long nChar, long inplen, long kst) 
{
	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = SnumInp( RP_SNUM1_PN, &m_Rpline.RP_SNUM1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_SNUM1_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

void CRpkeyDlg::OnSetFocusRpSnum2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	try
	{
		set_nowpn( RP_SNUM2_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_SNUM2_PN );	
}

void CRpkeyDlg::OnKillFocusRpSnum2( long inplen ) 
{
	SnumInp( RP_SNUM2_PN, &m_Rpline.RP_SNUM2_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpSnum2(long nChar, long inplen, long kst) 
{

	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = SnumInp( RP_SNUM2_PN, &m_Rpline.RP_SNUM2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_SNUM2_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}

// 証憑番号 入力＆オフフォーカス
long CRpkeyDlg::SnumInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	char	buf[30] = {0};
	VARIANT var;

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

		if( var.pbVal[0] != 0 )	strcpy_s( (char*)check.IP_SNUM, sizeof check.IP_SNUM, (char*)var.pbVal );
		else					::ZeroMemory( check.IP_SNUM, sizeof check.IP_SNUM );

		if( check.IP_SNUM[0] != 0 )
		{
			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
			strcpy_s( (char*)keybuf->IP_SNUM, sizeof keybuf->IP_SNUM, (char*)check.IP_SNUM );
		}
		else
			m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;

		//データセット
	//	data_set( getINP_CTL( pn )->IDC_X, keybuf, CICSInput );
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
//		data_get( getINP_CTL( pn )->IDC_X, &check, CICSInput );
//		if( check.IP_DENP != 0xffff )
//		{
//			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
//
//			keybuf->IP_DENP = check.IP_DENP;
//		}
	}
	else if( nChar == VK_DELETE )
	{
		check.IP_TGDAY = -1;
		var.pbVal = NULL;
		DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	
		::ZeroMemory( keybuf->IP_SNUM, sizeof keybuf->IP_SNUM );
		m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}


///////////////////////////////////////////////////////////////////////////////
// 手形期日
void CRpkeyDlg::OnSetFocusRpTgdate1() 
{

	try
	{
		set_nowpn( RP_TDAT1_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_TDAT1_PN ); 	
}

void CRpkeyDlg::OnKillFocusRpTgdate1( long inplen ) 
{
	TgdateInp( RP_TDAT1_PN, &m_Rpline.RP_TDAT1_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpTgdate1(long nChar, long inplen, long kst) 
{

	try
	{
		if( inplen > 0 || nChar == 0x16 ) CheckDataIcsinputTgDate( RP_TDAT1_PN, nChar );
		// 入力＆オフフォーカス
		if( !(nChar = TgdateInp( RP_TDAT1_PN, &m_Rpline.RP_TDAT1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_TDAT1_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

void CRpkeyDlg::OnSetFocusRpTgdate2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	try
	{
		set_nowpn( RP_TDAT2_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_TDAT2_PN );	
}

void CRpkeyDlg::OnKillFocusRpTgdate2( long inplen ) 
{
	TgdateInp( RP_TDAT2_PN, &m_Rpline.RP_TDAT2_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpTgdate2(long nChar, long inplen, long kst) 
{
	try
	{
		if( inplen > 0 || nChar == 0x16 ) CheckDataIcsinputTgDate( RP_TDAT2_PN, nChar );

		// 入力＆オフフォーカス
		if( !(nChar = TgdateInp( RP_TDAT2_PN, &m_Rpline.RP_TDAT2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		switch( nChar ) {
//		case VK_RETURN:
//		case VK_TAB:
		case VK_DELETE:
		case VK_RIGHT:
			set_focus( RP_IDOK );
			break;
		default:
			if( term_focus( RP_TDAT2_PN, nChar ) != 0 )
				Buzzer();

			break;
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}


/**************************
	手形期日チェックデータ
***************************/
long CRpkeyDlg::CheckDataIcsinputTgDate( int pn, long nChar ) 
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
	try
	{
		DBdata_get( this, getINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		strcpy_s( data, sizeof data, (char*)var.pbVal );

		if( (ret = TgDateInputCheck( data, result, sizeof result, Voln1->ss_ymd, Voln1->ee_ymd, -1 )) <= 0 ) {
			// 不正データが入力された
			if( m_Rpline._RPDTA[pn].IP_TGDAY > 0 ) {
				int gendate, gengo;
				vd.db_datecnvGen( 0, m_Rpline._RPDTA[pn].IP_TGDAY, &gengo, &gendate, 0, 0 );

				sprintf_s( result, sizeof result, "%d", gendate );
				var.pbVal = (BYTE*)result;
			}
			else	var.pbVal = NULL;

			DBdata_set( this, getINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			IntsrekitoGenstr( buf, sizeof buf, m_Rpline._RPDTA[pn].IP_TGDAY );
			DBSetDispString( this, getINP_CTL(pn)->IDC_X, buf );
			return -1;
		}
		else {
//			memcpy( check, bcd, 3 );
		}

		var.pbVal = (BYTE*)result;
		DBdata_set( this, getINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
//		DBSetDispString( this, getINP_CTL(this, DATE_PN)->IDC_X, str );
//		DBdata_reply( this, getINP_CTL(this, DATE_PN)->IDC_X, 1, &ipdata );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}

	return nChar;
}

// 手形期日　入力＆オフフォーカス
long CRpkeyDlg::TgdateInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	char	buf[30] = {0};
	VARIANT var;
	CVolDateDB vd;

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		if( var.pbVal[0] != 0 ) {
			strcpy_s( buf, sizeof buf, (LPCTSTR)var.pbVal );
			BYTE bcddate[10] = {0};
			l_pack( bcddate, buf, 6 );
			check.IP_TGDAY = TgDateBcdToSeireki( bcddate, NULL );
		//	int gendate;
		//	gendate = atoi( (LPCTSTR)var.pbVal );
		//	vd.db_datecnv( gendate, &check.IP_TGDAY, 1, 0 );
		}
		else	check.IP_TGDAY = -1;

		if( check.IP_TGDAY != -1 )
		{
			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
			keybuf->IP_TGDAY = check.IP_TGDAY;
		}
		else {
			m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
			keybuf->IP_TGDAY = -1;
		}

		//データセット
		IntsrekitoGenstr( buf, sizeof buf, m_Rpline._RPDTA[pn].IP_TGDAY );
		DBSetDispString( this, getINP_CTL( pn )->IDC_X, buf );
	//	data_set( getINP_CTL( pn )->IDC_X, keybuf, CICSInput );
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
//		data_get( getINP_CTL( pn )->IDC_X, &check, CICSInput );
//		if( check.IP_DENP != 0xffff )
//		{
//			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
//
//			keybuf->IP_DENP = check.IP_DENP;
//		}
	}
	else if( nChar == VK_DELETE )
	{
		check.IP_TGDAY = -1;
		var.pbVal = NULL;
		DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	
		keybuf->IP_TGDAY = check.IP_TGDAY;
		m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}


long CRpkeyDlg::OnTerminationRpSyzsw(long nChar) 
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_SYZSW1_PN, nChar ) != 0 ) {
		if( nChar != VK_DOWN && nChar != VK_UP && nChar == VK_ESCAPE ) {
			Buzzer();
		}
		return nChar;
	}
	return 0;
}

long CRpkeyDlg::OnTerminationRpSyzsw2(long nChar) 
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_SYZSW2_PN, nChar ) != 0 ) {
		if( nChar != VK_DOWN && nChar != VK_UP && nChar == VK_ESCAPE ) {
			Buzzer();
		}
		return nChar;
	}
	return 0;
}

long CRpkeyDlg::OnTerminationRpZritu( long nChar)
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_ZRITU1_PN, nChar ) != 0 ) {
		if( nChar != VK_DOWN && nChar != VK_UP && nChar == VK_ESCAPE ) {
			Buzzer();
		}
		return nChar;
	}
	return 0;
}

long CRpkeyDlg::OnTerminationRpZritu2( long nChar)
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_ZRITU2_PN, nChar ) != 0 ) {
		if( nChar != VK_DOWN && nChar != VK_UP && nChar == VK_ESCAPE ) {
			Buzzer();
		}
		return nChar;
	}
	return 0;
}
long CRpkeyDlg::OnTerminationRpSkbn(long nChar) 
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_SKBN1_PN, nChar ) != 0 ) {
		if( nChar != VK_DOWN && nChar != VK_UP && nChar == VK_ESCAPE ) {
			Buzzer();
		}
		return nChar;
	}
	return 0;
}

long CRpkeyDlg::OnTerminationRpSkbn2(long nChar) 
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_SKBN2_PN, nChar ) != 0 ) {
		if( nChar != VK_DOWN && nChar != VK_UP && nChar == VK_ESCAPE ) {
			Buzzer();
		}
		return nChar;
	}
	return 0;
}

long CRpkeyDlg::OnTerminationRpUrisire(long nChar) 
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_URISIRE1_PN, nChar ) != 0 ) {
		if( nChar != VK_DOWN && nChar != VK_UP && nChar == VK_ESCAPE ) {
			Buzzer();
		}
		return nChar;
	}
	return 0;
}

long CRpkeyDlg::OnTerminationRpUrisire2(long nChar) 
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_URISIRE2_PN, nChar ) != 0 ) {
		if( nChar != VK_DOWN && nChar != VK_UP && nChar == VK_ESCAPE ) {
			Buzzer();
		}
		return nChar;
	}
	return 0;
}

//特定収入
long CRpkeyDlg::OnTerminationRpTokus(long nChar) 
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_TOKUS1_PN, nChar ) != 0 ) {
		if( nChar != VK_DOWN && nChar != VK_UP && nChar == VK_ESCAPE ) {
			Buzzer();
		}
		return nChar;
	}
	return 0;
}

long CRpkeyDlg::OnTerminationRpTokus2(long nChar) 
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_TOKUS2_PN, nChar ) != 0 ) {
		if( nChar != VK_DOWN && nChar != VK_UP && nChar == VK_ESCAPE ) {
			Buzzer();
		}
		return nChar;
	}
	return 0;
}

long CRpkeyDlg::OnTerminationRpHjiyu(long nChar) 
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_HJIYU1_PN, nChar ) != 0 ) {
		if( nChar != VK_DOWN && nChar != VK_UP && nChar == VK_ESCAPE ) {
			Buzzer();
		}
		return nChar;
	}
	return 0;
}


long CRpkeyDlg::OnTerminationRpHjiyu2(long nChar) 
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_HJIYU2_PN, nChar ) != 0 ) {
		if( nChar != VK_DOWN && nChar != VK_UP && nChar == VK_ESCAPE ) {
			Buzzer();
		}
		return nChar;
	}
	return 0;
}



// 伝票番号取消
long CRpkeyDlg::OnTerminationNoDenp(long nChar)
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_NODENP_PN, nChar ) != 0 ) {
	//	Buzzer();
		return nChar;
	}
	return 0;
}
//借方部門取消
long CRpkeyDlg::OnTerminationNoBmn(long nChar)
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_NOBMON_PN, nChar ) != 0 ) {
	//	Buzzer();
		return nChar;
	}
	return 0;
}
//貸方部門取消
long CRpkeyDlg::OnTerminationNoCreBmn(long nChar)
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_NOCBMN_PN, nChar ) != 0 ) {
	//	Buzzer();
		return nChar;
	}
	return 0;
}

//借方工事取消
long CRpkeyDlg::OnTerminationNoKoji(long nChar)
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_NOKOJI_PN, nChar ) != 0 ) {
	//	Buzzer();
		return nChar;
	}
	return 0;
}

//貸方工事取消
long CRpkeyDlg::OnTerminationNoCreKoji(long nChar)
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_NOCKJI_PN, nChar ) != 0 ) {
	//	Buzzer();
		return nChar;
	}
	return 0;
}


//
long CRpkeyDlg::OnTerminateOnOK( long nChar )
{
	switch( nChar )
	{
	case VK_RIGHT:
	case VK_DOWN:
//	case VK_F3:
		set_focus( RP_IDCANCEL );
		return 0;
		break;
	case VK_UP:
	case VK_LEFT:
	case VK_F2:
		if( getINP_CTL( RP_CONMDFY_PN )->INP_ena )
			set_focus( RP_CONMDFY_PN );
		else if( getINP_CTL( RP_CONFIRM_PN )->INP_ena )
			set_focus( RP_CONFIRM_PN );
		else
			set_focus( RP_HSENCHK_PN );
		return 0;
		break;
	case VK_TAB:
		if( is_SHIFT() ) {
			if( getINP_CTL( RP_CONMDFY_PN )->INP_ena )
				set_focus( RP_CONMDFY_PN );
			else if( getINP_CTL( RP_CONFIRM_PN )->INP_ena )
				set_focus( RP_CONFIRM_PN );
			else
				set_focus( RP_HSENCHK_PN );
		}
		else {
			set_focus( RP_IDCANCEL );
		}
		return 0;
		break;
	}
	return nChar;
}


struct _KeyMoveTbl* CRpkeyDlg::Get_KeyMoveTbl( int nowpn )
{
	struct _KeyMoveTbl* km, *ret;
	km = RpMoveTbl;
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
long CRpkeyDlg::term_focus( int nowpn, long nChar )
{
	struct _KeyMoveTbl* km, *first, *tmp;
	int pn, ret;
	km = RpMoveTbl;
	first = RpMoveTbl;

	km = Get_KeyMoveTbl( nowpn );
	if( km == NULL )
		return nChar;

	tmp = km;
	ret = nChar;

	CComboBox* pCmb = NULL;

	switch( nowpn ) {
	case RP_SYZSW1_PN:		pCmb = &m_Syzsw;	break;
	case RP_ZRITU1_PN:		pCmb = &m_Zritu;	break;
	case RP_SKBN1_PN:		pCmb = &m_Skbn;		break;
	case RP_URISIRE1_PN:	pCmb = &m_Urisire;	break;
	case RP_SYZSW2_PN:		pCmb = &m_Syzsw2;	break;
	case RP_ZRITU2_PN:		pCmb = &m_Zritu2;	break;
	case RP_SKBN2_PN:		pCmb = &m_Skbn2;	break;
	case RP_URISIRE2_PN:	pCmb = &m_Urisire2;	break;
	// 特定収入
	case RP_TOKUS1_PN:	pCmb = &m_Tokus;	break;
	case RP_TOKUS2_PN:	pCmb = &m_Tokus2;	break;

	case RP_HJIYU1_PN:	pCmb = &m_Hjiyu1;	break;
	case RP_HJIYU2_PN:	pCmb = &m_Hjiyu2;	break;
	case RP_HSEN1_PN:	pCmb = &m_Hcbo;		break;
	case RP_HSEN2_PN:	pCmb = &m_Hcbo2;	break;
	case RP_MENWARI2_PN:pCmb = &m_MenWari2;	break;
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
	else {
		if( nChar == VK_ESCAPE ) {
			EndDialog( IDCANCEL );
			return 0;
		}
	}
		
	switch( nChar ) {
	case VK_DELETE:
	case VK_RETURN:
	case VK_TAB:
		km++;
		if( km->nowpn != -1 ) {
			do {
				if( getINP_CTL( km->nowpn )->INP_ena ) {
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
				if( getINP_CTL( tmp->right_pn )->INP_ena ) {
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
				if( getINP_CTL( tmp->dn_pn )->INP_ena ) {
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
				if( getINP_CTL( tmp->left_pn )->INP_ena ) {
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
				if( getINP_CTL( tmp->up_pn )->INP_ena ) {
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

//
// 一括修正用のデータ検索
int CRpkeyDlg::datacmp( CDBDATA* data )
{
	struct _RPL_LINE *key = &m_Rpline;
	int date, ksgn, n, i, k, st, len;
	CString str, str2;
	char	val[6];
	BYTE	dsgn;

	FILETrace( "datacmp [%d] %d, %02x%02x%02x(7)%02x\n", data->seq, data->ddate, 
				data->dsign[0],data->dsign[1],data->dsign[2],data->dsign[7] );

	if( key->LINE_CND[ RP_CONMDFY_PN ].INP_sg ) {
		// 確定仕訳 を修正可能にしない場合、
		if( ! key->LINE_CND[ RP_CONMDFY_PN ].INP_type ) {
			// 確定仕訳は修正不可
			if( _tline_chk( &data->dsign ) || IsJzSansyo() )
				return FALSE;
		}
	}

	// 取消仕訳
	if( (data->dsign[0] & 0x01) )
		return FALSE;

	// 仕訳 一括取消
	if( m_bSwkDel ) {
		if( ! IsModifyConfirmData() || IsEnkakuUser() ) {	// 確定仕訳修正不可・遠隔ユーザの場合
			// 確定仕訳は修正不可
			if( _tline_chk( &data->dsign ) || IsJzSansyo() )
				return FALSE;
		}
	}
	else {
		//仕訳確定サイン
		if( key->LINE_CND[ RP_CONFIRM_PN ].INP_sg )
		{
			if( key->LINE_CND[ RP_CONFIRM_PN ].INP_type ) {
				// 確定仕訳は修正不可
				if( _tline_chk( &data->dsign ) || IsJzSansyo() )
					return FALSE;
			}
		}
	}

	//
	// キー項目入力ありチェック
	if( !isScanKeyInp() ) {
		// 確定仕訳 を 未確定 にしない場合、
		if( ! key->LINE_CND[ RP_CONMDFY_PN ].INP_sg && ! key->LINE_CND[ RP_CONMDFY_PN ].INP_type ) {
			if( ! IsModifyConfirmData() || IsEnkakuUser() ) {	// 確定仕訳修正不可・遠隔ユーザの場合
				// 確定仕訳は修正不可
				if( _tline_chk( &data->dsign ) || IsJzSansyo() )
					return FALSE;
			}
		}
		return TRUE;
	}

	//
	// 日付
	if( key->LINE_CND[ RP_DATE1_PN ].INP_sg )
	{
		if( ! m_ChkMnonly ) {
			pDBzm->BcdDateToYmd( key->RP_DATE1_DT.IP_BCDDAY, &date );
			ksgn = KESY_SG_GET( key->RP_DATE1_DT.IP_BCDDAY[0] );

			if( data->ddate != date )
				return FALSE;

			// 決修サインもチェック
			if( (ksgn && ! data->ksign) || (! ksgn && data->ksign ) )
				return FALSE;
		}
		else {
			// 月のみ
			int mnth, data_mnth;
			data_mnth = (data->ddate % 10000) / 100;

			str.Format( "%02x", (key->RP_DATE1_DT.IP_BCDDAY[0]&0x1f) );
			mnth = atoi( str );

			ksgn = KESY_SG_GET( key->RP_DATE1_DT.IP_BCDDAY[0] );

			if( data_mnth != mnth )
				return FALSE;

			// 決修サインもチェック
			if( (ksgn && ! data->ksign) || (! ksgn && data->ksign ) )
				return FALSE;
		}
	}
	//
	// 伝票番号
	if( key->LINE_CND[ RP_DENP1_PN ].INP_sg )
	{
		if( data->cno == -1 )
			return( FALSE );

		if( data->cno !=  key->RP_DENP1_DT.IP_DENP )
			return( FALSE );
	}
	//
	// 借方部門番号
	if( key->LINE_CND[ RP_BMON1_PN ].INP_sg  )
	{
		if( data->dbmn == -1 )
			return FALSE;

		if( data->dbmn != key->RP_BMON1_DT.IP_BMN )
			return FALSE;
	}
	//
	// 貸方部門番号
	if( key->LINE_CND[ RP_CBMN1_PN ].INP_sg  )
	{
		if( data->cbmn == -1 )
			return FALSE;

		if( data->cbmn != key->RP_CBMN1_DT.IP_BMN )
			return FALSE;
	}
	//
	// 借方工事番号
	if( key->LINE_CND[ RP_KOJI1_PN ].INP_sg  )
	{
		if( data->dkno.IsEmpty() )
			return FALSE;

		str = key->RP_KOJI1_DT.IP_KOJI;
		len = str.GetLength();
		trim_zero( str.GetBuffer(128), 128, len );

		if( data->dkno != str )
			return FALSE;
	}
	//
	// 貸方工事番号
	if( key->LINE_CND[ RP_CKJI1_PN ].INP_sg  )
	{
		if( data->ckno.IsEmpty() )
			return FALSE;

		str = key->RP_CKJI1_DT.IP_KOJI;
		len = str.GetLength();
		trim_zero( str.GetBuffer(128), 128, len );

		if( data->ckno != str )
			return FALSE;
	}
	//
	// 借方
	if( key->LINE_CND[RP_DEBT1_PN].INP_sg ) 
	{
		if( key->RP_DEBT1_DT.IP_CdNum[0] ) {
			str = data->dbt;
			str2 = key->RP_DEBT1_DT.IP_CdNum;

			str.TrimRight();
			str2.TrimRight();

			if( str != str2 )
				return FALSE;
		}

		// 枝番
		if( key->RP_DEBT1_DT.IP_BRN != -1 && data->dbr != key->RP_DEBT1_DT.IP_BRN )
			return FALSE;
	}
	//
	// 貸方
	if( key->LINE_CND[RP_CRED1_PN].INP_sg ) 
	{
		if( key->RP_CRED1_DT.IP_CdNum[0] ) {
			str = data->cre;
			str2 = key->RP_CRED1_DT.IP_CdNum;

			str.TrimRight();
			str2.TrimRight();

			if( str != str2 )
				return FALSE;
		}
		
		// 枝番
		if( key->RP_CRED1_DT.IP_BRN != -1 && data->cbr != key->RP_CRED1_DT.IP_BRN )
			return FALSE;
	}

	//
	// 金額
	if( key->LINE_CND[ RP_VAL1_PN ].INP_sg )
	{
		l_defn( 0x16 );

		l_input( val, (void*)(LPCTSTR)data->val );
		if( l_cmp( val, (char *)key->RP_VAL1_DT.IP_VAL ) != 0 )
			return( FALSE );
	}
	//
	// 消費税仕訳区分
	if( key->LINE_CND[ RP_SYZSW1_PN ].INP_sg )
	{
		dsgn = key->RP_SYZSW1_DT.SKBN_SGN1;

		if( (data->dsign[4]&0xff) != dsgn )
			return FALSE;
	}
	//
	// 税率
	if( key->LINE_CND[ RP_ZRITU1_PN ].INP_sg )
	{
		switch( key->RP_ZRITU1_DT.SKBN_SGN1 )
		{
		case ZR_10:	dsgn = 0x05;	break;
		case ZR_8K:	dsgn = 0x04;	break;
		case ZR_8:	dsgn = 0x03;	break;
		case ZR_5:	dsgn = 0x00;	break;
		case ZR_3:	dsgn = 0x01;	break;
		default:
			dsgn = 0x02;
			break;
		}

		CString syz1, syz2;
		pDBzm->SyzKariCode( syz1, syz2 );

		// 仮払消費税か仮受消費税
//		if( data->dbt == "01071101" || data->cre == "01071101" || data->dbt == "04051601" || data->cre == "04051601" ) {
//		if( data->dbt == syz1 || data->cre == syz1 || data->dbt == syz2 || data->cre == syz2 ) {
		if( CheckSyzKariCode(data, syz1, syz2) ) {
			if( (data->dsign[1]&0x0f) != dsgn )
				return FALSE;
		}
		else {
			// 課税仕訳のみ
			if( !( (data->dsign[2]&0x0f) == 0x01 && (data->dsign[1]&0x0f) == dsgn) )
				return FALSE;
		}
	}
	//
	// 消費税区分
	if( key->LINE_CND[ RP_SKBN1_PN ].INP_sg )
	{
		if( ! (data->dsign[0] & 0x40) )
			return FALSE;

		switch( key->RP_SKBN1_DT.SKBN_SGN1 )
		{
		case SK_SOTO:	dsgn = 0x00;	break;
		case SK_UCHI:	dsgn = 0x10;	break;
		case SK_BETU:	dsgn = 0x20;	break;
		case SK_HIKZ:	dsgn = 0x00;	break;
		case SK_HUKA:
		default:
						dsgn = 0x02;	break;
		}

		if( key->RP_SKBN1_DT.SKBN_SGN1 == SK_HIKZ || key->RP_SKBN1_DT.SKBN_SGN1 == SK_HUKA ) {
			if( (data->dsign[2]&0x0f) != dsgn )
				return FALSE;
		}
		else {
			// 外税・内税・別記は、課税サインもチェック
			if( (data->dsign[2]&0x0f) != 0x01 || (data->dsign[1]&0xf0) != dsgn )
				return FALSE;
		}
	}
	//
	// 売上／仕入 区分
	if( key->LINE_CND[ RP_URISIRE1_PN ].INP_sg )
	{
		int tp = is_symstyp();
		if( tp == KANI_SYMST ) {

			if( ! kani_datacmp( data, key->RP_URISIRE1_DT.SKBN_SGN1 ) )
				return FALSE;

#ifdef OLD_CLOSE
			switch( key->RP_URISIRE1_DT.SKBN_SGN1 )
			{
			case GY_OROSI:	dsgn = 0x01;	break;
			case GY_KOURI:	dsgn = 0x02;	break;
			case GY_SEIZO:	dsgn = 0x03;	break;
			case GY_SRVCE:	dsgn = 0x05;	break;
			case GY_ETC:
			default:
				dsgn = 0x04;
				break;
			}
			if( (data->dsign[3]&0x0f) != dsgn )
				return FALSE;
#endif
		}
		else if( tp == KOBET_SYMST ) {

			if( ! kobetsu_datacmp( data, key->RP_URISIRE1_DT.SKBN_SGN1 ) )
				return FALSE;

#ifdef OLD_CLOSE
			switch( key->RP_URISIRE1_DT.SKBN_SGN1 )
			{
			case URI_KAZEI:		dsgn = 0x10;	break;
			case URI_HIKAZEI:	dsgn = 0x20;	break;
			case URI_KYOTU:
			default:
				dsgn = 0x30;
				break;
			}
			if( (data->dsign[3]&0xf0) != dsgn )
				return FALSE;
#endif
		}
		else {
			return FALSE;
		}
	}

	//
	// 特定収入
	if( pDBzm->zvol->apno >= 0x50 ) {
		if( key->LINE_CND[ RP_TOKUS1_PN ].INP_sg )
		{
			switch( key->RP_TOKUS1_DT.SKBN_SGN1 )
			{
			case TK_SYNYU:	dsgn = 0x01;	break;
			case TK_KATOK:	dsgn = 0x11;	break;
			case TK_HKATK:	dsgn = 0x21;	break;
			case TK_KYOTK:	dsgn = 0x31;	break;

			case TK_FUTOK:	dsgn = 0x02;	break;
			case TK_GAITK:	dsgn = 0x03;	break;

			case TK_SYNYU5:	dsgn = 0x04;	break;
			case TK_KATOK5:	dsgn = 0x14;	break;
			case TK_HKATK5:	dsgn = 0x24;	break;
			case TK_KYOTK5:	dsgn = 0x34;	break;

			case TK_FUTOK5:	dsgn = 0x05;	break;
			case TK_SYNYU8:	dsgn = 0x06;	break;
			case TK_KATOK8:	dsgn = 0x16;	break;
			case TK_HKATK8:	dsgn = 0x26;	break;
			case TK_KYOTK8:	dsgn = 0x36;	break;
			case TK_FUTOK8:	dsgn = 0x07;	break;

			case TK_SYNYU10:	dsgn = 0x08;	break;
			case TK_KATOK10:	dsgn = 0x18;	break;
			case TK_HKATK10:	dsgn = 0x28;	break;
			case TK_KYOTK10:	dsgn = 0x38;	break;
			case TK_FUTOK10:	dsgn = 0x09;	break;

			case TK_SYNYU8K:	dsgn = 0x0A;	break;
			case TK_KATOK8K:	dsgn = 0x1A;	break;
			case TK_HKATK8K:	dsgn = 0x2A;	break;
			case TK_KYOTK8K:	dsgn = 0x3A;	break;
			case TK_FUTOK8K:	dsgn = 0x0B;	break;

			default:
				return FALSE;
				break;
			}

			int sytyp = is_symstyp();
			if( sytyp == KOBET_SYMST ) {
				if( key->RP_TOKUS1_DT.SKBN_SGN1 == TK_SYNYU ) {
					if( (data->dsign[5]&0x0f) != dsgn )
						return FALSE;
				}
				else {
					if( (data->dsign[5]&0x3f) != dsgn )
						return FALSE;
				}
			}
			else {
				if( (data->dsign[5]&0x0f) != dsgn )
					return FALSE;
			}
#ifdef CLOSE
			if( (data->dsign[5]&0xff) != dsgn )
				return FALSE;
#endif
		}
	}

	//
	// 摘要
	if( key->LINE_CND[ RP_TKY1_PN ].INP_sg )
	{
		if( key->LINE_CND[ RP_TKY1_PN ].IMG_sg ) {	// イメージ摘要 検索
			if( !(data->dsign[0] & 0x80) )
				return FALSE;

			if( data->imgsq != key->RP_TKY1_DT.IMG_SQ )
				return FALSE;
		}
		else {
			if( data->dsign[0] & 0x80 )	// イメージ摘要
				return FALSE;

			char* pcmp;
			pcmp = data->tekiyo.GetBuffer(128);
			n = strlen( (char *)key->RP_TKY1_DT.IP_TKY );

			CString strChk2;
			int chksgn = HaihunCheck( (LPCTSTR)key->RP_TKY1_DT.IP_TKY, strChk2 );

			for( i = pDBzm->zvol->tk_ln * 2, k = 0; i >= n ; )
			{
				if( !strncmp( pcmp, (char *)key->RP_TKY1_DT.IP_TKY, n ) )
					break;	//Found

				if( chksgn ) { //半角長音でも検索
					if( !strncmp( pcmp, (char *)(LPCTSTR)strChk2, n ) )
						break;	//Found
				}

				st = IsKindOfLetter( (LPCTSTR)data->tekiyo, k );
				switch( st ) {
				case 1:
					i--;	pcmp++;	k++;	break;
				case 2:
					i -= 2;	pcmp += 2;	k += 2;	break;
				default:
					data->tekiyo.ReleaseBuffer();
					return FALSE;
				}
			}
			data->tekiyo.ReleaseBuffer();

			if( i < n )
				return( FALSE );
		}
	}
	//変動事由
	if( key->LINE_CND[ RP_HJIYU1_PN ].INP_sg )
	{
		if( IsSyafuKaisei( pDBzm->zvol ) ) {
			if( key->RP_HJIYU1_DT.SKBN_SGN1 == HJIYU_MITOROK ) { //内部取引なし
				if( data->hjiyu != 0 )
					return FALSE;
			}
			else {
				if( data->hjiyu != key->RP_HJIYU1_DT.SKBN_SGN1 )
					return FALSE;
			}
		}
		else {
			if( data->hjiyu != key->RP_HJIYU1_DT.SKBN_SGN1 )
				return FALSE;
		}
	}

	//手形番号
	if( key->LINE_CND[ RP_SNUM1_PN ].INP_sg )
	{
		str = key->RP_SNUM1_DT.IP_SNUM;

		if( data->rnumber != str )
			return FALSE;
	}
	//手形期日
	if( key->LINE_CND[ RP_TDAT1_PN ].INP_sg )
	{
		if( data->tdate != key->RP_TDAT1_DT.IP_TGDAY )
			return FALSE;
	}	
	//付箋
	if( key->LINE_CND[ RP_HSEN1_PN ].INP_sg )
	{
		if( (data->dsign[7]&0x0f) != key->RP_HSEN1_DT.IP_HUSEN )
			return FALSE;
	}
	//証憑番号	[11.26 /12]
	if( key->LINE_CND[ RP_DOCEVI1_PN ].INP_sg )
	{
		str = key->RP_DOCEVI1_DT.IP_SNUM;

		if( data->skn != str )
			return FALSE;
	}


	// 確定仕訳・転送済み仕訳を未確定にする
	if( key->LINE_CND[RP_CONMDFY_PN].INP_type && ! m_bSwkDel )
	{
		if( ! (data->dsign.GetAt(7) & 0x10) && ! (data->dsign.GetAt(7) & 0x20) ) {
			return FALSE;
		}
	}
	else {
		if( ! IsModifyConfirmData() || IsEnkakuUser() ) {	// 確定仕訳修正不可・遠隔ユーザの場合
			// 確定仕訳は修正不可
			if( _tline_chk( &data->dsign ) || IsJzSansyo() )
				return FALSE;
		}
	}

	// インボイス対応
	// 登録番号
	if( key->LINE_CND[RP_INVNO_PN].INP_sg ) {
		if( data->invno.CompareNoCase( key->RP_INVNO_DT._invno ) != 0 ) {
			return FALSE;
		}
	}

	DBKNREC* pKn;
	int sire = 0;
	pKn = pDBzm->DB_PjisToKnrec(data->dbt);
	if( pKn ) {
		if( pDBzm->km_siire(pKn) > 0 ) {
			sire++;
		}
	}
	pKn = pDBzm->DB_PjisToKnrec(data->cre);
	if( pKn ) {
		if( pDBzm->km_siire(pKn) > 0 ) {
			sire++;
		}
	}

	int tmp = 0;
	tmp = _atoh((char*)(LPCTSTR)data->dbt);
	int karibarai = 0;

	if( IsKaribarai(tmp) ) {
		karibarai++;
	}

	tmp = _atoh((char*)(LPCTSTR)data->cre);
	if( IsKaribarai(tmp) ) {
		karibarai++;
	}

	//完成工事振替も対象
	if( !sire ) {
		if( pDBzm->zvol->apno == 0x20 ) {
			if( data->dsign[4] == 31 )
				sire++;
		}
	}
	
	if( m_MenzeiChk ) { //免税事業者チェックON
		if( !karibarai ) {
			if( !(data->dsign[0] & 0x40) )
				return FALSE;

			if( !sire )		return FALSE;
		}

		if( key->LINE_CND[RP_MENZEI_PN].INP_type ) {
			// すでに 免税事業者からの課税仕入れ chkON
		//	if( data->dsign.GetAt(6) & 0x20 ) {
		//		return FALSE;
		//	}
		}
	}
	else if( m_KazeiChk ) {	// 課税事業者の仕入 = 免税事業者チェックOFF
		if( !karibarai ) {
			if( !(data->dsign[0] & 0x40) )
				return FALSE;

			if( !sire )		return FALSE;
		}

		if( key->LINE_CND[RP_KAZEI_PN].INP_type ) {
		//	if( !(data->dsign.GetAt(6) & 0x20) ) {
		//		return FALSE;
		//	}
		}
	}

	return( TRUE );

}


//
//	個別対応の設定を 途中で変更されたときの対応
//
BOOL CRpkeyDlg::kobetsu_datacmp( CDBDATA* data, short sign )
{
	int tp = is_symstyp();

	if( tp != KOBET_SYMST )
		return FALSE;

	BYTE defKbn, tmp, chkKbn, dsgn;

	switch( (Voln1->s_sgn6 & 0x0f) ) {
	case 1:	defKbn = 0x10;	break;
	case 2:	defKbn = 0x20;	break;
	case 3:	defKbn = 0x30;	break;
	default:
		defKbn = 0;
		break;
	}

	chkKbn = 0;

	//課税仕訳か？
	BOOL bChkOK = FALSE;
	DWORD dwDebt, dwCred;

	dwDebt = _atoh( (char*)(LPCTSTR)data->dbt );
	dwCred = _atoh( (char*)(LPCTSTR)data->cre );

	if( IsKaribarai(dwDebt) || IsKariuke(dwCred) ||
		IsKaribarai(dwCred) || IsKariuke(dwDebt) ) {
		bChkOK = TRUE;
	}

	if( ! bChkOK ) {
		if( (data->dsign[0] & 0x41) != 0x40 ) 
			return FALSE;
		if( (data->dsign[2] & 0x0f) != 0x01 )
			return FALSE;

		bChkOK = TRUE;
	}

	DWORD dwKbaraiYunyu = 0;

	if( data->dsign[4] ) {
		tmp = data->dsign[4];

		if( tmp == 4 || tmp == 6 || tmp == 8 || tmp == 10 || tmp == 11 || tmp == 14 ||
			tmp == 16 || (tmp >= 21 && tmp <= 25) ) {

			if( !(data->dsign[3] & 0xf0) ) {
				chkKbn = defKbn;
			}

			//輸入 仮払
			if( tmp == 4 ) {
				if( IsKaribarai(dwDebt) )	dwKbaraiYunyu |= 0x01;
				if( IsKaribarai(dwCred) )	dwKbaraiYunyu |= 0x02;
			}
		}
	}
	
	if( chkKbn == 0 ) {
		chkKbn = (data->dsign[3] & 0xf0);
	}

	// 今回の検索対象サイン
	switch( sign )
	{
	case URI_KAZEI:		dsgn = 0x10;	break;
	case URI_HIKAZEI:	dsgn = 0x20;	break;
	case URI_KYOTU:		dsgn = 0x30;	break;
	default:
		dsgn = 0;
		break;
	}

	if( sign != URI_KIHON && sign != URI_TAISYO ) {
		if( chkKbn != 0 ) {
			return ( chkKbn == dsgn );
		}
	}

	// 途中で変更のため サインがついていない
	DBKNREC* pKn1, *pKn2;

	pKn1 = pDBzm->DB_PjisToKnrec( data->dbt, TRUE );
	pKn2 = pDBzm->DB_PjisToKnrec( data->cre, TRUE );

	// 基本設定／仕入区分対象
	if( sign == URI_KIHON || sign == URI_TAISYO ) {
	//	if( ! IsKobetsuKamoku( pKn1, data->dsign[2] ) && ! IsKobetsuKamoku( pKn2, data->dsign[2] ) )
	//		return FALSE;

		BOOL bDeb, bCre;
		bDeb = IsKobetsuKamoku( pKn1, data->dsign[2] );
		bCre = IsKobetsuKamoku( pKn2, data->dsign[2] );

		//仮払消費税で輸入
		if( dwKbaraiYunyu & 0x01 )	bDeb = TRUE;
		if( dwKbaraiYunyu & 0x02 )	bCre = TRUE;

		if( ! bDeb && ! bCre )
			return FALSE;

		if( sign == URI_KIHON ) {
			if( (data->dsign[3] & 0xf0) == 0 ) {
				//科目に設定があった場合は、基本設定とはしない。
				if( bDeb ) {
					if( (pKn1->knsgn[4] & 0x0f) )	return FALSE;
				}
				if( bCre ) {
					if( (pKn2->knsgn[4] & 0x0f) )	return FALSE;
				}
				return TRUE;
			}
		}
		else if( sign == URI_TAISYO )	return TRUE;

		return FALSE;
	}

	BOOL bKmChk = FALSE;

	if( pKn1 ) {
		bKmChk = IsKobetsuKamoku( pKn1, data->dsign[2] );

		if( bKmChk ) {
			switch( (pKn1->knsgn[4] & 0x0f) ) {
			case 0x01:	chkKbn = 0x10;	break;
			case 0x02:	chkKbn = 0x20;	break;
			case 0x03:	chkKbn = 0x30;	break;
			default:
				break;
			}
			if( chkKbn != 0 ) {
				return ( chkKbn == dsgn );
			}
			else {
				return ( defKbn == dsgn );
			}
		}
	}

	if( pKn2 ) {
		bKmChk = FALSE;

		bKmChk = IsKobetsuKamoku( pKn2, data->dsign[2] );

		if( bKmChk ) {
			switch( (pKn2->knsgn[4] & 0x0f) ) {
			case 0x01:	chkKbn = 0x10;	break;
			case 0x02:	chkKbn = 0x20;	break;
			case 0x03:	chkKbn = 0x30;	break;
			default:
				break;
			}
			if( chkKbn != 0 ) {
				return ( chkKbn == dsgn );
			}
			else {
				return ( defKbn == dsgn );
			}
		}
	}

	return FALSE;
}


//
//	簡易課税の設定に 途中で変更されたときの対応
//
BOOL CRpkeyDlg::kani_datacmp( CDBDATA* data, short sign )
{
	int tp = is_symstyp();

	if( tp != KANI_SYMST )
		return FALSE;

	BYTE defKbn, tmp, chkKbn, dsgn;

	defKbn = KaniDefaultKubun( Voln1 );

	chkKbn = 0;

	//仮受消費税チェックにも売上区分がつく
	CString syz1, syz2;
	pDBzm->SyzKariCode(syz1, syz2);
	CDBDATA chkd;
	chkd.dbt = data->dbt;
	chkd.cre = data->cre;

	BOOL bKariuke = FALSE;
	if( CheckSyzKariCode(&chkd, syz2, syz2) ) {
		bKariuke = TRUE;
	}

	if( !bKariuke ) {
		//課税仕訳か？
		if( (data->dsign[0] & 0x41) != 0x40 )
			return FALSE;

		if( (data->dsign[2] & 0x3f) != 0x01 && (data->dsign[2] & 0x3f) != 0x31 )
			return FALSE;

		if( data->dsign[4] ) {
			tmp = data->dsign[4];
			if( (tmp >= 1 && tmp <= 3) || tmp == 5 || tmp == 7 || tmp == 9 || tmp == 13 ||
				tmp == 15 || tmp == 16 || (tmp >= 17 && tmp <= 20) || tmp == 26 || tmp == 27 ) {
				chkKbn = defKbn;
			}
		}
	}

	if( chkKbn == 0 ) {
		chkKbn = (data->dsign[3] & 0x0f);
	}

	// 今回の検索対象サイン
	switch( sign )
	{
	case GY_OROSI:	dsgn = 0x01;	break;
	case GY_KOURI:	dsgn = 0x02;	break;
	case GY_SEIZO:	dsgn = 0x03;	break;
	case GY_SRVCE:	dsgn = 0x05;	break;
	case GY_FDOSN:	dsgn = 0x06;	break;
	case GY_ETC:
	default:
		dsgn = 0x04;
	}

	if( chkKbn != 0 ) {
		return ( chkKbn == dsgn );
	}

	// 途中で変更のため サインがついていない
	DBKNREC* pKn1, *pKn2;

	pKn1 = pDBzm->DB_PjisToKnrec( data->dbt, TRUE );
	pKn2 = pDBzm->DB_PjisToKnrec( data->cre, TRUE );

	BOOL bKmChk = FALSE;

	if( pKn1 ) {
		/* 売上、資産譲渡 */
		if( (pKn1->knsgn[0]&0xff) == (BYTE)0x02 || (pKn1->knsgn[0]&0xff) == (BYTE)0x11  ) {
			bKmChk = TRUE;
		}
		/* 貸倒損失 */
	//	if( (pKn1->knsgn[0]&0xff) == (BYTE)0x08 && (pKn1->knsgn[1]&0xff) == 0x03 ) {
	//		bKmChk = TRUE;
	//	}
		/* 資産譲渡 */
		if( (pKn1->knsgn[0]&0xff) == (BYTE)0x01 && (pKn1->knsgn[1]&0xff) == 0x01 ) {
			bKmChk = TRUE;
		}

		if( bKmChk || bKariuke ) {
			chkKbn = (pKn1->knsgn[3] & 0x0f);

			if( chkKbn != 0 ) {
				return ( chkKbn == dsgn );
			}
			else {
				return ( defKbn == dsgn );
			}
		}
	}

	if( pKn2 ) {
		bKmChk = FALSE;

		/* 売上、資産譲渡 */
		if( (pKn2->knsgn[0]&0xff) == (BYTE)0x02 || (pKn2->knsgn[0]&0xff) == (BYTE)0x11  ) {
			bKmChk = TRUE;
		}
		/* 貸倒損失 */
	//	if( (pKn2->knsgn[0]&0xff) == (BYTE)0x08 && (pKn2->knsgn[1]&0xff) == 0x03 ) {
	//		bKmChk = TRUE;
	//	}
		/* 資産譲渡 */
		if( (pKn2->knsgn[0]&0xff) == (BYTE)0x01 && (pKn2->knsgn[1]&0xff) == 0x01 ) {
			bKmChk = TRUE;
		}

		if( bKmChk || bKariuke ) {
			chkKbn = (pKn2->knsgn[3] & 0x0f);

			if( chkKbn != 0 ) {
				return ( chkKbn == dsgn );
			}
			else {
				return ( defKbn == dsgn );
			}
		}
	}

	return FALSE;
}

//
// キー項目入力ありチェック
BOOL CRpkeyDlg::isScanKeyInp()
{
	struct _RPL_LINE *key = &m_Rpline;
	
		// 日付
	if( key->LINE_CND[ RP_DATE1_PN ].INP_sg ||
		// 伝票番号
		key->LINE_CND[ RP_DENP1_PN ].INP_sg ||
		// 部門番号
		key->LINE_CND[ RP_BMON1_PN ].INP_sg || key->LINE_CND[ RP_CBMN1_PN ].INP_sg ||
		// 工事
		key->LINE_CND[ RP_KOJI1_PN ].INP_sg || key->LINE_CND[ RP_CKJI1_PN ].INP_sg ||
		// 借方、貸方科目, 枝番
		key->LINE_CND[ RP_DEBT1_PN ].INP_sg || key->LINE_CND[RP_CRED1_PN].INP_sg || 
		// 金額
		key->LINE_CND[ RP_VAL1_PN ].INP_sg ||
		// 消費税区分
		key->LINE_CND[ RP_SYZSW1_PN ].INP_sg ||
		key->LINE_CND[ RP_ZRITU1_PN ].INP_sg ||
		key->LINE_CND[ RP_SKBN1_PN ].INP_sg ||
		key->LINE_CND[ RP_URISIRE1_PN ].INP_sg ||
		key->LINE_CND[ RP_TOKUS1_PN ].INP_sg ||
		// 摘要 等
		key->LINE_CND[ RP_TKY1_PN ].INP_sg ||
		key->LINE_CND[ RP_HJIYU1_PN ].INP_sg ||
		key->LINE_CND[ RP_SNUM1_PN ].INP_sg ||
		key->LINE_CND[ RP_TDAT1_PN ].INP_sg ||
		key->LINE_CND[ RP_HSEN1_PN ].INP_sg ||
		// 証憑番号	[11.26 /12]
		key->LINE_CND[ RP_DOCEVI1_PN ].INP_sg ||
		// 確定仕訳を未確定に
		key->LINE_CND[ RP_CONMDFY_PN ].INP_sg ||
		// 登録番号
		key->LINE_CND[RP_INVNO_PN].INP_sg || key->LINE_CND[RP_INVNO2_PN].INP_sg || m_MenzeiChk || m_KazeiChk )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// 修正箇所のチェック
BOOL CRpkeyDlg::ReplaceChk(int data_pn)
{
	int chk_pn = -1;
	BOOL bRval;

	switch( data_pn ) {
	case DATA_DATE:
		chk_pn = RP_DATE2_PN;
		break;
	case DATA_DENP:
		chk_pn = RP_DENP2_PN;
		break;
	case DATA_DEB_BMON:
		chk_pn = RP_BMON2_PN;
		break;
	case DATA_CRE_BMON:
		chk_pn = RP_CBMN2_PN;
		break;
	case DATA_DEB_KNO:
		chk_pn = RP_KOJI2_PN;
		break;
	case DATA_CRE_KNO:
		chk_pn = RP_CKJI2_PN;
		break;
	case DATA_DEBT:
		chk_pn = RP_DEBT2_PN;
		break;
	case DATA_CRED:
		chk_pn = RP_CRED2_PN;
		break;
	case DATA_VAL:
		chk_pn = RP_VAL2_PN;
		break;
	case DATA_SYZ:
		chk_pn = RP_SKBN2_PN;
		break;
	case DATA_TEKIYO:
		chk_pn = RP_TKY2_PN;
		break;
	case DATA_HJIYU:
		chk_pn = RP_HJIYU2_PN;
		break;
	case DATA_SYONO:
		chk_pn = RP_SNUM2_PN;
		break;
	case DATA_TGDATE:
		chk_pn = RP_TDAT2_PN;
		break;
	case DATA_HUSEN:
		chk_pn = RP_HSEN2_PN;
		break;
	case DATA_DOCEVI:	//証憑番号
		chk_pn = RP_DOCEVI2_PN;
		break;
	case DATA_SKIN: 
	default:
		chk_pn = -1;
		break;
	}

	if( chk_pn >= 0 ){
		if( data_pn == DATA_SYZ ) {
			bRval = (m_Rpline.LINE_CND[RP_SYZSW2_PN].INP_sg || m_Rpline.LINE_CND[RP_ZRITU2_PN].INP_sg ||
					 m_Rpline.LINE_CND[RP_SKBN2_PN].INP_sg || m_Rpline.LINE_CND[RP_URISIRE2_PN].INP_sg );

			if( pDBzm->zvol->apno >= 0x50 ) {
				if( m_Rpline.LINE_CND[RP_TOKUS2_PN].INP_sg )	bRval = TRUE;
			}
		}
		else {
			if( chk_pn == RP_DENP2_PN || chk_pn == RP_BMON2_PN || chk_pn == RP_CBMN2_PN ||
				chk_pn == RP_KOJI2_PN || chk_pn == RP_CKJI2_PN || chk_pn == RP_DOCEVI2_PN )
				bRval = (m_Rpline.LINE_CND[chk_pn].INP_sg || m_Rpline.LINE_CND[chk_pn].INP_type);
			else
				bRval = m_Rpline.LINE_CND[chk_pn].INP_sg;
		}
	} 
	else {
		bRval = FALSE;
	}

	return bRval;
}

//
// データの修正後の仕訳を作成する
void CRpkeyDlg::MakeMdfySiwake(CDBINPDataRec* pDt)
{
	struct _RPL_LINE *key = &m_Rpline;
	struct _RPL_LINE *fncarg;
	CDBINPDataRec orgrec, tmprec;
	int st;
	struct _SY_MSG_PACK2 smpk;

	//仕訳の一括取消
	if( m_bSwkDel ) {
		pDt->m_dsign[0] |= 0x01;
		pDBzm->dbdata->dsign[0] |= 0x01;
		return;
	}

	// 修正前のﾃﾞｰﾀを保存(枝番戻しのため)
	orgrec = *pDt;
	// ダミーでイニシャライズ
	tmprec.Reset();
	tmprec.m_seq = orgrec.m_seq+1;
	tmprec.m_ddate = orgrec.m_ddate;
	tmprec.m_ksign = orgrec.m_ksign;
	tmprec.m_mofs  = orgrec.m_mofs;
	
	DB_SyIniz( &tmprec );

	//特定収入仕訳を変更する場合。
	Set_DsignCheck( key, pDt );

	// CDBDATA にセットしている
	DB_SyIniz( pDt );

	// 元の消費税をｾｯﾄ
	SavePrevSyz(&orgrec);

	// 元の税額が自動計算か？
	COLORREF back;
	int defzsg = isnot_defzeigaku(pDt, back);
	// 元仕訳 外税 免税
	int org_soto = is_sotomensiwake(pDt);
	int chg50_80 = 0;

	// 日付
	if( key->LINE_CND[ RP_DATE2_PN ].INP_sg )
	{
		int ksgn;
		CVolDateDB vd;
		int offset;
		BYTE tmp[10] = {0};

		//リバースチャージ等、期間判定がある場合のため、再度イニシャライズ
//		DB_SyIniz( &tmprec );   下に移動

		if( ! m_ChkMnonly ) {
			ksgn = KESY_SG_GET( key->RP_DATE2_DT.IP_BCDDAY[0] );
			pDBzm->BcdDateToYmd( key->RP_DATE2_DT.IP_BCDDAY, &key->RP_DATE2_DT.IP_DAY );

			memcpy( tmp, key->RP_DATE2_DT.IP_BCDDAY, 2 );

			switch( ksgn ) {
			case 1:	tmp[0] = 0x61;	break;
			case 2:	tmp[0] = 0x62;	break;
			case 3:	tmp[0] = 0x63;	break;
			case 4:	tmp[0] = 0x00;	break;
			default:
				break;
			}

			if( vd.db_vd_offset( tmp, &offset, pDBzm ) != 0 ) {
				ICSMessageBox( "月のオフセットを取得できません！ ",0,0,0,this);
				return;
			}

			pDBzm->dbdata->ddate = key->RP_DATE2_DT.IP_DAY;
			pDBzm->dbdata->ksign = (ksgn > 0) ? 1 : 0;
			pDBzm->dbdata->mofs = offset;
		}
		else {
			// 月のみ
			ksgn = KESY_SG_GET( key->RP_DATE2_DT.IP_BCDDAY[0] );
			tmp[0] = key->RP_DATE2_DT.IP_BCDDAY[0];

			int mm_num, ym_num, ysgn, gengo;
			BYTE yymm[10] = {0};
			char ybf[32];

			switch( ksgn ) {
			case 1:	mm_num = 61;	break;
			case 2:	mm_num = 62;	break;
			case 3:	mm_num = 63;	break;
			case 4:	mm_num = 0;		break;
			default:
				sprintf_s( ybf, sizeof ybf, "%02x", tmp[0] );
				mm_num = atoi( ybf );
				ksgn = 0;
				break;
			}

			vd.db_vd_yearGen( 1, mm_num, &gengo, &ym_num, &ysgn, pDBzm );
			sprintf_s( ybf, sizeof ybf, "%d", ym_num );

			if( ym_num >= 100 ) {
				int s_year, d_year, mnth, day;
				int chkdate, newdate, d_gengo;
				s_year = atoi( ybf );

				vd.db_datecnvGen( gengo, s_year, &d_gengo, &d_year, 1, 1 );

				mnth = (s_year %100);
				day = (pDBzm->dbdata->ddate % 100);
				
				if( ksgn != 0 ) {
					int tmp_mn;

					switch( ksgn ) {
					case 1:	tmp_mn = 61;	break;
					case 2:	tmp_mn = 62;	break;
					case 3:	tmp_mn = 63;	break;
					case 4:
					default:
						tmp_mn = 0x00;	break;
					}
					chkdate = (tmp_mn * 100)+ day;
				}
				else {
					chkdate = (mnth * 100)+ day;
				}

				if( vd.db_vd_check( 0, chkdate, pDBzm ) == 0 ) {
					if( vd.db_vd_offset( 0, chkdate, &offset, pDBzm ) != 0 ) {
						ICSMessageBox( "月のオフセットを取得できません！ ",0,0,0,this);
						return;
					}
					newdate = ((d_year/100) * 10000) + (mnth * 100)+ day;

					pDBzm->dbdata->ddate	= newdate;
					pDBzm->dbdata->ksign	= (ksgn > 0) ? 1 : 0;
					pDBzm->dbdata->mofs		= offset;
				}
			}
		}

		// DB_SyIniz で CDBDATA の値が変更されるので保存
		int new_ddate = pDBzm->dbdata->ddate;
		int new_ksign = pDBzm->dbdata->ksign;
		int new_mofs = pDBzm->dbdata->mofs;

		int inv_date_check = 0;
		// 163461
		if( (orgrec.m_ddate < SYZMEN1_YMD) && (new_ddate >= SYZMEN1_YMD) ||
			(new_ddate < SYZMEN1_YMD) && (orgrec.m_ddate >= SYZMEN1_YMD) ) {
			inv_date_check++;
		}
		// 163411 50% 控除期間をまたぐ場合、内税の消費税計算 されない
		if( (orgrec.m_ddate < SYZMEN3_YMD) && (new_ddate >= SYZMEN3_YMD) ||
			(new_ddate < SYZMEN3_YMD) && (orgrec.m_ddate >= SYZMEN3_YMD) ) {
			inv_date_check++;
		}

		if( inv_date_check || org_soto ) {
			//科目も含めて Iniz
			tmprec.m_dbt = pDt->m_dbt;
			tmprec.m_dbr = pDt->m_dbr;
			tmprec.m_cre = pDt->m_cre;
			tmprec.m_cbr = pDt->m_cbr;

			memcpy(tmprec.m_val, pDt->m_val, sizeof tmprec.m_val);
			memcpy(tmprec.m_zei, pDt->m_zei, sizeof tmprec.m_zei);
			tmprec.m_dsign.Copy(pDt->m_dsign);

			if( (new_ddate < SYZMEN3_YMD) && (orgrec.m_ddate >= SYZMEN3_YMD) ) {
				chg50_80 = 1;
			}
		}
		//リバースチャージ等、期間判定がある場合のため、再度イニシャライズ
		DB_SyIniz(&tmprec);
			   		 
		pDBzm->dbdata->ddate = new_ddate;
		pDBzm->dbdata->ksign = new_ksign;
		pDBzm->dbdata->mofs  = new_mofs;
		pDt->m_ddate = new_ddate;
		pDt->m_ksign = new_ksign;
		pDt->m_mofs  = new_mofs;

		//免時事業者 税額 自動変更を生かす
		if( m_svMenzei != NON_SELECT && ( (!org_soto && !defzsg) || org_soto ) ) {
			pDBzm->SetCDBData(pDt);
			pDBsy->SyMsg(&smpk);
			pDBzm->GetCDBData(pDt);
		}
		else {
			DB_SyIniz(pDt);
		}
	}

	// 伝票番号
	if( key->LINE_CND[ RP_DENP2_PN ].INP_type ) { // 伝票番号なし 07.01 /03
		pDBzm->dbdata->cno = -1;
	}
	else {
		if( key->LINE_CND[ RP_DENP2_PN ].INP_sg )
		{
			pDBzm->dbdata->cno = key->RP_DENP2_DT.IP_DENP;
		}
	}
	// 借方 部門番号
	if( key->LINE_CND[ RP_BMON2_PN ].INP_type ) { // 部門なし 07.01 /03
		pDBzm->dbdata->dbmn = -1;
	}
	else {
		if( key->LINE_CND[ RP_BMON2_PN ].INP_sg )
		{
			pDBzm->dbdata->dbmn = key->RP_BMON2_DT.IP_BMN;
		}
	}
	// 貸方 部門番号
	if( key->LINE_CND[ RP_CBMN2_PN ].INP_type ) { // 部門なし 07.01 /03
		pDBzm->dbdata->cbmn = -1;
	}
	else {
		if( key->LINE_CND[ RP_CBMN2_PN ].INP_sg )
		{
			pDBzm->dbdata->cbmn = key->RP_CBMN2_DT.IP_BMN;
		}
	}

	// 借方 工事番号
	if( key->LINE_CND[ RP_KOJI2_PN ].INP_type ) { // 工事番号なし
		pDBzm->dbdata->dkno.Empty();
	}
	else {
		if( key->LINE_CND[ RP_KOJI2_PN ].INP_sg )
		{
			pDBzm->dbdata->dkno = key->RP_KOJI2_DT.IP_KOJI;
			trim_zero( pDBzm->dbdata->dkno.GetBuffer(64), 64, pDBzm->dbdata->dkno.GetLength() );
			pDBzm->dbdata->dkno.ReleaseBuffer();
		}
	}
	// 貸方 工事番号
	if( key->LINE_CND[ RP_CKJI2_PN ].INP_type ) { // 工事番号なし
		pDBzm->dbdata->ckno.Empty();
	}
	else {
		if( key->LINE_CND[ RP_CKJI2_PN ].INP_sg )
		{
			pDBzm->dbdata->ckno = key->RP_CKJI2_DT.IP_KOJI;
			trim_zero( pDBzm->dbdata->ckno.GetBuffer(64), 64, pDBzm->dbdata->ckno.GetLength() );
			pDBzm->dbdata->ckno.ReleaseBuffer();
		}
	}
	//科目・枝番の変更
	int kmk_chg = 0;

	// 借方
	if( key->LINE_CND[RP_DEBT2_PN].INP_sg ) 
	{
		if( key->RP_DEBT2_DT.IP_CdNum[0] ) {
			pDBzm->dbdata->dbt = key->RP_DEBT2_DT.IP_CdNum;
		}
		pDBzm->dbdata->dbr = key->RP_DEBT2_DT.IP_BRN;

		if( pDBzm->dbdata->dbt.CompareNoCase(orgrec.m_dbt) != 0 || orgrec.m_dbr != pDBzm->dbdata->dbr ) {
			kmk_chg |= 0x01;
		}
		pDBsy->SyMsg( &smpk );
	}
	// 借方枝番の登録のﾁｪｯｸ
//	Brchk1(0, &pDt->darec, &orgrec.darec, 1);

	// 貸方
	if( key->LINE_CND[RP_CRED2_PN].INP_sg ) 
	{
		if( key->RP_CRED2_DT.IP_CdNum[0] ) {
			pDBzm->dbdata->cre = key->RP_CRED2_DT.IP_CdNum;
		}
		pDBzm->dbdata->cbr = key->RP_CRED2_DT.IP_BRN;

		if( pDBzm->dbdata->cre.CompareNoCase(orgrec.m_cre) != 0 || orgrec.m_cbr != pDBzm->dbdata->cbr ) {
			kmk_chg |= 0x02;
		}
		pDBsy->SyMsg( &smpk );
	}
	// 貸方枝番の登録のﾁｪｯｸ
//	Brchk1(1, &pDt->darec, &orgrec.darec, 1);

	// 部門範囲チェック
	if( ! BmChk.BmnHaniCheck( pDBzm->dbdata->dbmn, pDBzm->dbdata->dbt ) ) {
		pDBzm->dbdata->dbmn = -1;
	}
	if( ! BmChk.BmnHaniCheck( pDBzm->dbdata->cbmn, pDBzm->dbdata->cre ) ) {
		pDBzm->dbdata->cbmn = -1;
	}

	// 金額
	if( key->LINE_CND[ RP_VAL2_PN ].INP_sg )
	{
		l_print( pDBzm->dbdata->val.GetBuffer(100), key->RP_VAL2_DT.IP_VAL, "sssssssssss9" );
		pDBzm->dbdata->val.ReleaseBuffer();
		pDBsy->SyMsg( &smpk );
	}

	// 消費税区分仕訳区分
	if( key->LINE_CND[ RP_SYZSW2_PN ].INP_sg )
			fncarg = key;
	else	fncarg = NULL;	// 元の仕訳の消費税をｾｯﾄ

	Set_Syzsw(fncarg, pDt);

	// 03.08 /17 税区分 → 税率 の順で 変更する。非課税の科目を 内税 5% に変更した場合
	// 税率 5% を先にセットしようにも、科目のデフォルト消費税が非課税でセットできない。[15-904]

	// 税区分
	if( key->LINE_CND[ RP_SKBN2_PN ].INP_sg )
			fncarg = key;
	else	fncarg = NULL;	// 元の仕訳の消費税をｾｯﾄ

	Set_Taxsgn(fncarg, pDt);

	// 税率
	if( key->LINE_CND[ RP_ZRITU2_PN ].INP_sg )
			fncarg = key;
	else	fncarg = NULL;	// 元の仕訳の消費税をｾｯﾄ

	Set_Zritu( fncarg, pDt);

	//
	// 業種・個別
	if( key->LINE_CND[ RP_URISIRE2_PN ].INP_sg )
			fncarg = key;
	else	fncarg = NULL;

	int zei_kbn = -1;
	if(  key->LINE_CND[ RP_SKBN2_PN ].INP_sg  ) {
		zei_kbn = key->RP_SKBN2_DT.SKBN_SGN1;
	}
	// 税区分を 非課税にする場合は、それを優先する
	if( fncarg == NULL ) {
		if( zei_kbn != SK_HIKZ && zei_kbn != SK_HUKA ) {
			Set_UriSire(fncarg, pDt);
		}
	}
	else {
		if( (zei_kbn == SK_HIKZ || zei_kbn == SK_HUKA) ) {
			// 税区分を非課税・不課税に変更する
		}
		else if( (orgrec.m_dsign[0]&0x40) && (orgrec.m_dsign[2]&0x0f) != 1 && zei_kbn == -1 ) {
			// 元の仕訳の税区分が非課税・不課税 で、税区分の変更なし
		}
		else {
			Set_UriSire(fncarg, pDt);
		}
	}

	// 特定収入
	// 税区分を 内税・外税・別記・非課税にする場合は、それを優先する
	if( pDBzm->zvol->apno >= 0x50 ) {
		if( key->LINE_CND[ RP_TOKUS2_PN ].INP_sg )
				fncarg = key;
		else	fncarg = NULL;

		if( fncarg == NULL ) {
			if( zei_kbn != SK_HIKZ && zei_kbn != SK_SOTO && zei_kbn != SK_UCHI && zei_kbn != SK_BETU ) {
				Set_Tokus(fncarg, pDt);
			}
		}
		else {
			Set_Tokus(fncarg, pDt);
		}
	}

	// 対価
	Set_Taika( NULL, pDt );

	//
	// 摘要
	if( key->LINE_CND[ RP_TKY2_PN ].INP_type ) { // 摘要取消
		if( (pDt->m_dsign[0] & 0x80) ) {
			// イメージ摘要につきサインを落とす
			pDBzm->dbdata->dsign[0] &= ~0x80;
			pDBzm->dbdata->imgsq = -1;
		}
		pDBzm->dbdata->tekiyo.Empty();
	}
	else {
		if( key->LINE_CND[ RP_TKY2_PN ].INP_sg )
		{
			// イメージ摘要にする
			if( key->LINE_CND[ RP_TKY2_PN ].IMG_sg ) {
				pDBzm->dbdata->dsign[0] |= 0x80;
				pDBzm->dbdata->tekiyo.Empty();
				pDBzm->dbdata->imgsq = key->RP_TKY2_DT.IMG_SQ;
			}
			else {
				// イメージ摘要でない
				if(!(pDt->m_dsign[0] & 0x80) ) {
					LPSTR p;
					p = pDBzm->dbdata->tekiyo.GetBuffer(128);
					st = TkyoIns((char*)p, (char*)key->RP_TKY1_DT.IP_TKY, (char*)key->RP_TKY2_DT.IP_TKY );
					pDBzm->dbdata->tekiyo.ReleaseBuffer();
				}
				else {
					// イメージ摘要につきサインを落とす
					pDBzm->dbdata->dsign[0] &= ~0x80;
					pDBzm->dbdata->tekiyo = key->RP_TKY2_DT.IP_TKY;
					pDBzm->dbdata->imgsq = -1;
				}
			}
		}
	}
	//
	// 変動事由
	if( key->LINE_CND[ RP_HJIYU2_PN ].INP_sg )
	{
		if( IsSyafuKaisei( pDBzm->zvol ) ) {
			if( key->RP_HJIYU2_DT.SKBN_SGN1 == HJIYU_MITOROK )
				pDBzm->dbdata->hjiyu = 0;
			else {
				pDBzm->dbdata->hjiyu = key->RP_HJIYU2_DT.SKBN_SGN1;
			}
		}
		else {
			pDBzm->dbdata->hjiyu = key->RP_HJIYU2_DT.SKBN_SGN1;
		}
	}
	//
	// 手形関係
	if( IsBillCodeEx(pDBzm->dbdata->dbt, pDBzm) || IsBillCodeEx(pDBzm->dbdata->cre, pDBzm) ) {
		//
		//証憑番号
		if( key->LINE_CND[RP_SNUM2_PN].INP_sg )
		{
			pDBzm->dbdata->rnumber = key->RP_SNUM2_DT.IP_SNUM;
		}
	}
	//手形期日-- 科目チェックやめる 240301_0902
	if( key->LINE_CND[ RP_TDAT2_PN ].INP_sg )
	{
		pDBzm->dbdata->tdate = key->RP_TDAT2_DT.IP_TGDAY;
	}
	//
	//付箋
	if( key->LINE_CND[ RP_HSEN2_PN ].INP_sg )
	{
		if( key->LINE_CND[ RP_HSEN2_PN ].INP_type ) {
			pDBzm->dbdata->dsign[7]  &= (~0x0f);	// 取消
			pDBzm->dbdata->tag.Empty();
		}
		else {
			// 付箋を設定
			pDBzm->dbdata->dsign[7] &= (~0x0f);
			pDBzm->dbdata->dsign[7]	|= key->RP_HSEN2_DT.IP_HUSEN;
		}
	}

	//仕訳確定サイン
	if( key->LINE_CND[ RP_CONFIRM_PN ].INP_sg )
	{
		if( key->LINE_CND[ RP_CONFIRM_PN ].INP_type )
			pDBzm->dbdata->dsign[7] |= 0x10;
	}
	//確定仕訳・転送済み仕訳を を 未確定仕訳にする
	if( key->LINE_CND[ RP_CONMDFY_PN ].INP_sg )
	{
		if( key->LINE_CND[ RP_CONMDFY_PN ].INP_type ) {
			pDBzm->dbdata->dsign[7] &= (~0x10);
			pDBzm->dbdata->dsign[7] &= (~0x20);
		}
	}

	//証憑番号
	if( bDOCEVI_Master ) {
		if( key->LINE_CND[ RP_DOCEVI2_PN ].INP_type ) { // 証憑番号なし
			pDBzm->dbdata->skn.Empty();
		}
		else if( key->LINE_CND[ RP_DOCEVI2_PN ].INP_sg )
		{
			pDBzm->dbdata->skn = key->RP_DOCEVI2_DT.IP_SNUM;
		}
	}

	pDBzm->GetCDBData( pDt );
	//変動事由チェック
	if( pDt->m_hjiyu > 0 ) {
		if( ! IsSyafuKaisei( pDBzm->zvol ) ) {
			if( ! p_DBDinpView->IsEnableHjiyu( pDt ) ) {
				pDt->m_hjiyu = 0;
				pDBzm->dbdata->hjiyu = 0;
			}
		}
		else {
			//社会福祉の内部取引は、全仕訳に設定可能
			//科目が変更されている場合
			if( orgrec.m_dbt.CompareNoCase( pDBzm->dbdata->dbt ) != 0 ||
				orgrec.m_cre.CompareNoCase( pDBzm->dbdata->cre ) != 0 ) {

				BOOL bDoClear = TRUE;

				if( key->LINE_CND[ RP_HJIYU2_PN ].INP_sg )
				{
					if( key->RP_HJIYU2_DT.SKBN_SGN1 != 100 )	//(なし) 以外
						bDoClear = FALSE;
				}

				if( bDoClear ) {
					pDt->m_hjiyu = 0;
					pDBzm->dbdata->hjiyu = 0;
				}
			}
		}
	}
	//仕入科目か？
	DBKNREC* pKn;
	int sire = 0;
	int uri_igai[2] = { -1,-1 };

	pKn = pDBzm->DB_PjisToKnrec(pDt->m_dbt);
	if( pKn ) {
		if( pDBzm->km_siire(pKn) > 0 ) {
			sire++;
		}
		else {
			//売上科目かをチェック
			if( issykamok(pKn) ) {
				uri_igai[0] = uri_attr_igai(pKn);
			}
		}
	}
	pKn = pDBzm->DB_PjisToKnrec(pDt->m_cre);
	if( pKn ) {
		if( pDBzm->km_siire(pKn) > 0 ) {
			sire++;
		}
		else {
			//売上科目かをチェック
			if( issykamok(pKn) ) {
				uri_igai[1] = uri_attr_igai(pKn);
			}
		}
	}
	//完成工事振替も対象
	int syzok = 0;
	int kanfuri = 0;
	if( !sire ) {
		if( pDBzm->zvol->apno == 0x20 ) {
			if( pDt->m_dsign[4] == 31 ) {
				//税区分チェック 課税
				if( (pDt->m_dsign[0] & 0x40) && (pDt->m_dsign[2] & 0x0f) == 0x01 ) {
					sire++;
					kanfuri = 1;
				}
			}
		}
	}
	if( sire ) {
		if( !uri_igai[0] || !uri_igai[1] )
			sire = 0;
	}

	int tmp = 0;
	tmp = _atoh((char*)(LPCTSTR)pDt->m_dbt);
	if( IsKaribarai(tmp) ) {
		if( is_karibarai_menzei(&pDt->m_dsign[0]) ) {
			syzok = 1;
			sire++;
		}
	}

	tmp = _atoh((char*)(LPCTSTR)pDt->m_cre);
	if( IsKaribarai(tmp) ) {
		if( is_karibarai_menzei(&pDt->m_dsign[0]) ) {
			syzok = 1;
			sire++;
		}
	}
	// 登録番号の対象は仕入 (非課税、不課税、資産譲渡、仕訳区分ありは除く)
	if( sire ) {
		if( (pDt->m_dsign[0] & 0x40) && (pDt->m_dsign[2] & 0x0f) == 0x01 ) {
			if( (pDt->m_dsign[2] & 0x30) != 0x30 && (pDt->m_dsign[4]) == 0 ) {
				syzok = 1;
			}
		}
	}
	int men_change = 0;

	// 免税事業者からの課税仕入れ
	if( key->LINE_CND[RP_MENZEI_PN].INP_sg && sire && (syzok || kanfuri)) {
		if( key->LINE_CND[RP_MENZEI_PN].INP_type ) {
			int mentype = GetMenzeiWariai(Voln1);
			DWORD fncCode = CD_SP_EXEMPT;
			//控除割合 50% ?
			if( mentype == SYZ_MEN_8050 || mentype == SYZ_MEN_50 ) {
				if( key->RP_MENWARI2_DT.SKBN_SGN2 ) {
					fncCode = CD_SP_EXEMPT_50;
				}
			}
			else if( mentype == SYZ_MEN_NOT ) {
				fncCode = 0;
			}
			if( fncCode ) {
				if( pDBsy->SyFnc(fncCode) == 0 ) {
					struct _SY_MSG_PACK2 smpk;
					pDBsy->SyMsg(&smpk);
					pDBzm->GetCDBData(pDt);
				}
				men_change++;
			}
		}
	}

	// 課税事業者からの課税仕入れ
	if( key->LINE_CND[RP_KAZEI_PN].INP_sg && sire ) {
		if( key->LINE_CND[RP_KAZEI_PN].INP_type ) {
			if( pDBsy->SyFnc( CD_SP_NO_EXEMPT ) == 0 ) {
				struct _SY_MSG_PACK2 smpk;
				pDBsy->SyMsg( &smpk );
				pDBzm->GetCDBData( pDt );
			}
			men_change++;
		}
	}

	// インボイス対応
	// 登録番号
	if( key->LINE_CND[RP_INVNO2_PN].INP_sg ) {
		if( syzok ) {
			sprintf_s(pDt->m_invno, sizeof(pDt->m_invno), _T("%s"), key->_RPDTA[RP_INVNO2_PN]._invno);
		}
	}
	//直前の免税事業者からのサインを戻す
	if( !men_change ) {
		if( sire && syzok ) {
			SetPrevMenzei(pDt);
			int new_soto = is_sotomensiwake(pDt);
			//外税免税で、科目・枝番変更 か 期間の変更(50%->80%) は税額が変わってしまう
			if( org_soto && new_soto && (kmk_chg || chg50_80) ) {
				memcpy(pDt->m_zei, orgrec.m_zei, 6);
			}
		}
	}
}


void CRpkeyDlg::SavePrevSyz( CDBINPDataRec* pDt )
{
	struct _SY_MSG_PACK2 smpk;
	pDBsy->SyMsg( &smpk );

	int ritu_sw, zeikbn_sw, uri_sw, zei_sw, swk_sw, taika_sw, toku_sw, menzei_sw;
	ritu_sw = zeikbn_sw = uri_sw = zei_sw = swk_sw = toku_sw = menzei_sw = 0;

	swk_sw	= smpk.SY_MSG_SWKBN->sel_no;
	ritu_sw = smpk.SY_MSG_RITU->sel_no;
	zeikbn_sw = smpk.SY_MSG_ZEI->sel_no;
	uri_sw	= smpk.SY_MSG_URISIRE->sel_no;
	//特定収入[06.03 /08]
	toku_sw	= smpk.SY_MSG_TOKTEI->sel_no;
	// 対価・取得区分
	taika_sw = smpk.SY_MSG_TAIKAKBN->sel_no;
	// 免税
	menzei_sw = smpk.SY_MSG_EXEMPT->sel_no;

	// 消費税仕訳区分
	if( swk_sw ) {
		m_svSyzsw = pDt->m_dsign[4];
#ifdef CLOSE
		switch( pDt->m_dsign[4] & 0xff ) {
		case 1:		m_svSyzsw = KH_URI;		break;
		case 2:		m_svSyzsw = KASISON;		break;
		case 3:		m_svSyzsw = KASISYU;		break;
		case 4:		m_svSyzsw = S_IMPORT;		break;
		case 5:		m_svSyzsw = H_EXPORT;		break;
		case 6:		m_svSyzsw = HAB_SYUN;		break;
		case 7:		m_svSyzsw = HAB_HIYO;		break;
		case 8:		m_svSyzsw = SHIWARI;		break;
		case 9:		m_svSyzsw = URIWARI;		break;
		case 10:	m_svSyzsw = TANA_PL;		break;
		case 11:	m_svSyzsw = TANA_MN;		break;
		case 12:	m_svSyzsw = TOKT_SYU;		break;
		default:
			m_svSyzsw = NON_SELECT;
			break;
		}
#endif
	}
	else	m_svSyzsw = NON_SELECT;

	// 税率
	if( ritu_sw ) {
		CString syz1, syz2;
		pDBzm->SyzKariCode(syz1, syz2);
		CDBDATA chkd;
		chkd.dbt = pDt->m_dbt;
		chkd.cre = pDt->m_cre;

		BOOL bSvRitu = FALSE;
		if( (pDt->m_dsign[2] & 0x0f) == 0x01 || CheckSyzKariCode(&chkd, syz1, syz2) ) {
			bSvRitu = TRUE;
		}
		if( bSvRitu ) {
			switch( pDt->m_dsign[1] & 0x0f ) {
			case 0:		m_svSyzRitu = ZR_5;		break;
			case 1:		m_svSyzRitu = ZR_3;		break;
			case 2:		m_svSyzRitu = ZR_0;		break;
			case 3:		m_svSyzRitu = ZR_8;		break;
			case 4:		m_svSyzRitu = ZR_8K;	break;
			case 5:		m_svSyzRitu = ZR_10;	break;

			default:
				m_svSyzRitu = NON_SELECT;
				break;
			}
		}
		else {
			m_svSyzRitu = NON_SELECT;
		}
	}
	else	m_svSyzRitu = NON_SELECT;

	// 税区分
	if( zeikbn_sw ) {
		if( (pDt->m_dsign[2] & 0x0f) == 0x01 ) {
			switch( pDt->m_dsign[1] & 0xf0 ) {
			case 0x00:		m_svSyzSkbn = SK_SOTO;		break;
			case 0x10:		m_svSyzSkbn = SK_UCHI;		break;
			case 0x20:		m_svSyzSkbn = SK_BETU;		break;
			default:
				m_svSyzSkbn = NON_SELECT;
				break;
			}
		}
		else {
			switch( pDt->m_dsign[2] & 0x0f ) {
			case 0:		m_svSyzSkbn = SK_HIKZ;		break;
			case 2:		m_svSyzSkbn = SK_HUKA;		break;
			default:
				m_svSyzSkbn = NON_SELECT;
				break;
			}		
		}
	}
	else {
		m_svSyzSkbn = NON_SELECT;
	}

	// 売上 ／ 仕入
	int sytyp;
	
	// 変更前仕訳の売上／仕入区分初期化
	m_svUriSire = NON_SELECT;
	m_svTokus = NON_SELECT;
	m_svTaika = NON_SELECT;

	sytyp = is_symstyp();
	// 消費税マスターでない
	if( sytyp == NORMAL_MST )
		return;
	
//	if( !pDBsy->IsSyohizei( pDBzm->dbdata ) )
//		return;
	if( ! IsSyohizeiData( pDBzm->dbdata ) )
		return;
	
	if( sytyp == KANI_SYMST ) {
		// 業種・簡易		
		switch( pDt->m_dsign[3] & 0x0f ) {
		case 1:	m_svUriSire = GY_OROSI;	break;
		case 2:	m_svUriSire = GY_KOURI;	break;
		case 3:	m_svUriSire = GY_SEIZO;	break;
		case 4:	m_svUriSire = GY_ETC;		break;
		case 5:	m_svUriSire = GY_SRVCE;		break;
		case 6:	m_svUriSire = GY_FDOSN;		break;
		default:
			break;
		}
	}
	else if( sytyp == KOBET_SYMST )
	{
		// 個別
		switch( pDt->m_dsign[3] & 0xf0 ) {
		case 0x10:	m_svUriSire = URI_KAZEI;	break;	// 課税売上に係る
		case 0x20:	m_svUriSire = URI_HIKAZEI;	break;	// 非課税売上に係る
		case 0x30:	m_svUriSire = URI_KYOTU;	break;	// 共通に係る
		default:
			break;
		}
	}

	// 特定収入
	m_svTokus = NON_SELECT;
	if( pDBzm->zvol->apno >= 0x50 ) {

		BYTE	dsign5;
		if (sytyp == KOBET_SYMST) {
			dsign5 = pDt->m_dsign[5] & 0xff;
		}
		else {
			dsign5 = pDt->m_dsign[5] & 0x0f;
		}

		switch(dsign5) {
		case 0x01:	m_svTokus = TK_SYNYU;	break;

		case 0x11:	
			if( sytyp == KOBET_SYMST )	m_svTokus = TK_KATOK;
			break;
		case 0x21:
			if( sytyp == KOBET_SYMST )	m_svTokus = TK_HKATK;
			break;
		case 0x31:	
			if( sytyp == KOBET_SYMST )	m_svTokus = TK_KYOTK;
			break;

		case 0x02:	m_svTokus = TK_FUTOK;	break;
		case 0x03:	m_svTokus = TK_GAITK;	break;

		case 0x04:	m_svTokus = TK_SYNYU5;	break;
		case 0x14:	
			if( sytyp == KOBET_SYMST )	m_svTokus = TK_KATOK5;
			break;
		case 0x24:
			if( sytyp == KOBET_SYMST )	m_svTokus = TK_HKATK5;
			break;
		case 0x34:	
			if( sytyp == KOBET_SYMST )	m_svTokus = TK_KYOTK5;
			break;

		case 0x05:	m_svTokus = TK_FUTOK5;	break;
		case 0x06:	m_svTokus = TK_SYNYU8;	break;
		case 0x16:	
			if( sytyp == KOBET_SYMST )	m_svTokus = TK_KATOK8;
			break;
		case 0x26:
			if( sytyp == KOBET_SYMST )	m_svTokus = TK_HKATK8;
			break;
		case 0x36:	
			if( sytyp == KOBET_SYMST )	m_svTokus = TK_KYOTK8;
			break;

		case 0x07:	m_svTokus = TK_FUTOK8;	break;
		// 10% 対応
		case 0x08:	m_svTokus = TK_SYNYU10;	break;
		case 0x18:	
			if( sytyp == KOBET_SYMST )	m_svTokus = TK_KATOK10;
			break;
		case 0x28:
			if( sytyp == KOBET_SYMST )	m_svTokus = TK_HKATK10;
			break;
		case 0x38:	
			if( sytyp == KOBET_SYMST )	m_svTokus = TK_KYOTK10;
			break;

		case 0x09:	m_svTokus = TK_FUTOK10;	break;

		// 8%軽減対応
		case 0x0A:	m_svTokus = TK_SYNYU8K;	break;
		case 0x1A:
			if (sytyp == KOBET_SYMST)	m_svTokus = TK_KATOK8K;
			break;
		case 0x2A:
			if (sytyp == KOBET_SYMST)	m_svTokus = TK_HKATK8K;
			break;
		case 0x3A:
			if (sytyp == KOBET_SYMST)	m_svTokus = TK_KYOTK8K;
			break;

		case 0x0B:	m_svTokus = TK_FUTOK8K;	break;

		default:
			break;
		}
	}

	m_svTaika = NON_SELECT;
	// 譲渡仕訳
	if( _joto_data(&pDt->m_dsign[0]) ) {
		if( taika_sw ) {
			m_svTaika = smpk.SY_MSG_TAIKAKBN->msg_tbl[taika_sw - 1]->code;
		}
	}
	//if( taika_sw ) {
	//	m_svTaika = smpk.SY_MSG_TAIKAKBN->msg_tbl[ taika_sw-1]->code;
	//}
	//else {
	//	m_svTaika = NON_SELECT;
	//}
	m_svMenzei = NON_SELECT;
	if( pDt->m_dsign[6] & 0x20 ) {
		int tmp = (pDt->m_dsign[6] & 0xc0);

		if( tmp == 0 ) {
			//80%
			m_svMenzei = INV_MEN80;
		}
		tmp >>= 6;

		if( tmp == 1 ) {
			//50%
			m_svMenzei = INV_MEN50;
		}
		else if( tmp == 2 ) { //0%
		}
	}
}

// 消費税仕訳区分の修正
int CRpkeyDlg::Set_Syzsw( struct _RPL_LINE* rpline, CDBINPDataRec* pDt)
{
	short syzsw;

	if( rpline ) {
		syzsw = rpline->RP_SYZSW2_DT.SKBN_SGN1;
	}
	else {
		syzsw = m_svSyzsw;
	}

	BOOL  bNone = FALSE;
	DWORD code = 0;

	switch( syzsw ) {
	case KH_URI:	code = CD_URIAGE;		break;
	case KASISON:	code = CD_KASISON;		break;
	case KASISYU:	code = CD_KASIKAI;		break;
	case S_IMPORT:	code = CD_YUNYU;		break;
	case H_EXPORT:	code = CD_HKYUSYUTU;	break;
	case HAB_HIYO:	code = CD_HANSYOHI;		break;
	case HAB_SYUN:	code = CD_HANSYONYU;	break;
	case URIWARI:	code = CD_URIWARI;		break;
	case SHIWARI:	code = CD_SIREWARI;		break;
	case TANA_PL:	code = CD_TANAKASAN;	break;
	case TANA_MN:	code = CD_TANAGENSAN;	break;
	case TOKT_SYU:	code = CD_TOKUTEI;		break;
	case MEN_EXPORT:code = CD_YUSYUTU;		break;	// 輸出売上

	case UK_RISOK:	code = CD_UKERI;		break;	// 受取利息
	case SIIRE:		code = CD_SIRE;			break;	// 仕入
	case KEIHI:		code = CD_KEIHI;		break;	// 経費
	case SEISAN_SWK:code = CD_ZEISEISAN;	break;	// 消費税精算

	case RVCHARGE:	code = CD_TKKZ_SIRE_SUB;	break;	// 特定課税仕入（本体）		
	case RVCHAR_HEN:code = CD_TKKZ_SIREHEN_SUB;	break;	// 特定課税仕入の返還（本体）

	case YUNYU_KOKU:	code = CD_YUNYU_KUNI;	break;	// 輸入消費税
	case YUNYU_CHIHO:	code = CD_YUNYU_TIHOU;	break;
	case YUNYU_GASAN:	code = CD_YUNYU_GASSAN;	break;
	case KOJI_KANFURI:	code = CD_KANSEI_FURI;	break;	// 完成時振替

	//仮受仮払の仕訳区分
	case SI_SYUTOK:	code = CD_SISNSYTK;	break;	// 資産取得
	case SI_JOTO:	code = CD_SISNJYOT;	break;	// 資産譲渡
	case URI_HEN:	code = CD_URIHEN;	break;	// 売上返品
	case SIIRE_HEN:	code = CD_SIREHEN;	break;	// 仕入返品

	// 仕訳区分 削除
	case SWKSW_DEL: code = CD_SWKBN_NON;	break;
	default:
		code	= CD_SWKBN_NON;
		bNone	= TRUE;
		break;
	}

//	if( syzsw != SWKSW_DEL && code == CD_SWKBN_NON ) {
//		if( m_svSyzRitu == ZR_0 )
//			return 0;
//	}

	// 科目が輸出免税売上で、仕訳区分がなしのとき [05.28 /08]
	if( bNone && chk_yumenz( pDt ) )
		return 0;

	// 科目が貸倒損失で、仕訳区分がなしのとき
	if( bNone && chk_kasitaore(pDt) )
		return 0;

	if( syzsw == RVCHARGE || syzsw == RVCHAR_HEN ) {
		if( pDt->m_ddate < RVCHAR_YMD ) {
			code	= CD_SWKBN_NON;
		}
	}
	if( code > 0 ) {
		if( pDBsy->SyFnc(code) == 0 ) {
			struct _SY_MSG_PACK2 smpk;
			pDBsy->SyMsg(&smpk);
			pDBzm->GetCDBData(pDt);
		}
	}

	return 0;
}

// [05.28 /08]
BOOL CRpkeyDlg::chk_yumenz( CDBINPDataRec* pDt )
{
	DBKNREC* pKn;

	pKn = pDBzm->DB_PjisToKnrec( pDt->m_dbt );
	if( pKn ) {
		if( (pKn->knsgn[0]&0x0f) == 0x02 && (pKn->knsgn[1]&0x0f) == 0x09 )
			return TRUE;
	}

	pKn = pDBzm->DB_PjisToKnrec( pDt->m_cre );
	if( pKn ) {
		if( (pKn->knsgn[0]&0x0f) == 0x02 && (pKn->knsgn[1]&0x0f) == 0x09 )
			return TRUE;
	}
	
	return FALSE;
}

//貸倒損失？ 
BOOL CRpkeyDlg::chk_kasitaore(CDBINPDataRec* pDt)
{
	DBKNREC* pKn;

	pKn = pDBzm->DB_PjisToKnrec(pDt->m_dbt);
	if( pKn ) {
		if( is_kasitaore(pKn) )
			return TRUE;
	}

	pKn = pDBzm->DB_PjisToKnrec(pDt->m_cre);
	if( pKn ) {
		if( is_kasitaore(pKn) )
			return TRUE;
	}

	return FALSE;
}


// 消費税率の セット
int CRpkeyDlg::Set_Zritu( struct _RPL_LINE* rpline, CDBINPDataRec* pDt )
{
	short syzsw;

	if( rpline ) {
		syzsw = rpline->RP_ZRITU2_DT.SKBN_SGN1;
	}
	else {
		syzsw = m_svSyzRitu;
	}

	DWORD code = 0;

	switch( syzsw ) {
	case ZR_10:	code = CD_RITU10;	break;
	case ZR_8K:	code = CD_RD_RITU8;	break;
	case ZR_8:	code = CD_RITU8;	break;
	case ZR_5:	code = CD_RITU5;	break;
	case ZR_3:	code = CD_RITU3;	break;
	case ZR_0:	code = CD_RITU0;	break;
	default:
		break;
	}

	if( code > 0 ) {
		if( pDBsy->SyFnc(code) == 0 ) {
			struct _SY_MSG_PACK2 smpk;
			pDBsy->SyMsg(&smpk);
			pDBzm->GetCDBData(pDt);
		}
	}

	return 0;
}



int CRpkeyDlg::Set_DsignCheck( struct _RPL_LINE* rpline, CDBINPDataRec* pDt )
{
	// 税区分を変更するか？
	if( ! rpline->LINE_CND[ RP_SKBN2_PN ].INP_sg )
		return 0;

	short syzsw;
	syzsw = rpline->RP_SKBN2_DT.SKBN_SGN1;

//BYTE* p1 = &pDt->m_dsign[0];
//MyTrace( "Set_DsignCheck(1) [%02x%02x%02x%02x%02x%02x]\n", p1[0],p1[1],p1[2],p1[3],p1[4],p1[5]);
	//税区分の変更をするときは、特定収入サインを変える
	if( pDBzm->zvol->apno >= 0x50 ) {
		if( syzsw != SK_HUKA ) {
			if( (pDt->m_dsign[0]&0x40) && (pDt->m_dsign[2]&0x0f) == 0x02 && (pDt->m_dsign[4]&0x0f)== 0x0c) {
				pDt->m_dsign[4] = 0; //仕訳区分 0x0c を落とす
			}
		}
	}
//MyTrace( "Set_DsignCheck(2) [%02x%02x%02x%02x%02x%02x]\n", p1[0],p1[1],p1[2],p1[3],p1[4],p1[5]);

	return 0;
}


// 税区分のセット
int CRpkeyDlg::Set_Taxsgn( struct _RPL_LINE* rpline, CDBINPDataRec* pDt )
{
	short syzsw;

	if( rpline ) {
		syzsw = rpline->RP_SKBN2_DT.SKBN_SGN1;
	}
	else {
		syzsw = m_svSyzSkbn;
	}

	DWORD code = 0;

	switch( syzsw ) {
	case SK_SOTO:	code = CD_SOTOZ;	break;
	case SK_UCHI:	code = CD_UTIZ;		break;
	case SK_BETU:	code = CD_BEKKI;	break;	
	case SK_HIKZ:	code = CD_HIKAZ;	break;
	case SK_HUKA:	code = CD_FUKAZ;	break;
	default:
		break;
	}

	if( code > 0 ) {
		if( pDBsy->SyFnc(code) == 0 ) {
			struct _SY_MSG_PACK2 smpk;
			pDBsy->SyMsg(&smpk);
			pDBzm->GetCDBData(pDt);
		}
	}

	return 0;
}

// 売上・仕入 区分
int CRpkeyDlg::Set_UriSire( struct _RPL_LINE* rpline, CDBINPDataRec* pDt )
{
	short syzsw;

	if( rpline ) {
		syzsw = rpline->RP_URISIRE2_DT.SKBN_SGN1;
	}
	else {
		syzsw = m_svUriSire;
	}

	DWORD code = 0;

	int sytyp = is_symstyp();
	// 消費税マスターでない
	if( sytyp == NORMAL_MST )
		return 0;
	
//	if( !pDBsy->IsSyohizei( pDBzm->dbdata ) ) 
//		return 0;
	if( ! IsSyohizeiData( pDBzm->dbdata ) )
		return 0;
	
	if( sytyp == KANI_SYMST ) {
		// 業種・簡易		
		switch( syzsw ) {
		case GY_OROSI:	code = CD_1OROSI;	break;
		case GY_KOURI:	code = CD_2KOURI;	break;
		case GY_SEIZO:	code = CD_3SEIZO;	break;
		case GY_ETC:	code = CD_4SONOTA;	break;
		case GY_SRVCE:	code = CD_5SRVIC;	break;
		case GY_FDOSN:	code = CD_6FUDOSAN;	break;
		default:
			break;
		}
	}
	else if( sytyp == KOBET_SYMST )
	{
		DBKNREC* pKn;
		int	ksgn = 0;

		// 個別
		switch( syzsw ) {
		case URI_KAZEI:		code = CD_KAURI;	break;	// 課税売上に係る
		case URI_HIKAZEI:	code = CD_HIURI;	break;	// 非課税売上に係る
		case URI_KYOTU:		code = CD_KYOURI;	break;	// 共通に係る
		case URI_KAMOKU:
			//科目設定に従う
			ksgn = 0;
			pKn = pDBzm->DB_PjisToKnrec( pDt->m_dbt );
			if( pKn ) {
				if( pDBzm->km_syattr( pKn ) > 0 ) {	//03.26 /12
					ksgn = (pKn->knsgn[4]&0x0f);
				}
			}
			pKn = pDBzm->DB_PjisToKnrec( pDt->m_cre );
			if( pKn ) {
				if( pDBzm->km_syattr( pKn ) > 0 ) {	//03.26 /12
					if( ksgn == 0 ) {
						ksgn = (pKn->knsgn[4]&0x0f);
					}
					else {
						if( (pKn->knsgn[4]&0x0f) != 0 )
							ksgn = (pKn->knsgn[4]&0x0f);
					}
				}
			}
			if( ksgn != 0 ) {
				switch( ksgn ) {
				case 1:	code = CD_KAURI;	break;	// 課税売上に係る
				case 2:	code = CD_HIURI;	break;	// 非課税売上に係る
				case 3:	code = CD_KYOURI;	break;	// 共通に係る
				default:
					break;
				}
			}
			break;
		default:
			break;
		}
	}

	if( code > 0 ) {
		if( pDBsy->SyFnc( code ) == 0 ) {
			struct _SY_MSG_PACK2 smpk;
			pDBsy->SyMsg( &smpk );
			pDBzm->GetCDBData( pDt );
		}
	}
	return 0;
}

// 特定収入[06.03 /08]
int CRpkeyDlg::Set_Tokus( struct _RPL_LINE* rpline, CDBINPDataRec* pDt )
{
	short syzsw;

	if( rpline ) {
		syzsw = rpline->RP_TOKUS2_DT.SKBN_SGN1;
	}
	else {
		syzsw = m_svTokus;
	}

	DWORD code = 0;

	int sytyp = is_symstyp();
	
//	if( !pDBsy->IsSyohizei( pDBzm->dbdata ) )
//		return 0;
	if( ! IsSyohizeiData( pDBzm->dbdata ) )
		return 0;
	
	// 個別
	switch( syzsw ) {
	case TK_SYNYU:	code = CD_TK_SYNYU;	break;

	case TK_KATOK:	
		if( sytyp == KOBET_SYMST )	code = CD_TK_KAURI|CD_TK_SYNYU;	// 課税売上に係る
		break;
	case TK_HKATK:	
		if( sytyp == KOBET_SYMST )	code = CD_TK_HIURI|CD_TK_SYNYU;	// 非課税売上に係る
		break;
	case TK_KYOTK:	
		if( sytyp == KOBET_SYMST )	code = CD_TK_KYOURI|CD_TK_SYNYU;	// 共通に係る
		break;

	case TK_FUTOK:	code = CD_TK_FUTOK;	break;
	case TK_GAITK:	code = CD_TK_GAITK;	break;
	// 5% or 8%
	case TK_SYNYU5:	code = CD_TK_SYNYU_5PER;	break;
	case TK_KATOK5:	
		if( sytyp == KOBET_SYMST )	code = CD_TK_KAURI|CD_TK_SYNYU_5PER;	// 課税売上に係る
		break;
	case TK_HKATK5:	
		if( sytyp == KOBET_SYMST )	code = CD_TK_HIURI|CD_TK_SYNYU_5PER;	// 非課税売上に係る
		break;
	case TK_KYOTK5:	
		if( sytyp == KOBET_SYMST )	code = CD_TK_KYOURI|CD_TK_SYNYU_5PER;	// 共通に係る
		break;
	case TK_FUTOK5:	code = CD_FK_SYNYU_5PER;	break;

	case TK_SYNYU8:	code = CD_TK_SYNYU_8PER;	break;
	case TK_KATOK8:	
		if( sytyp == KOBET_SYMST )	code = CD_TK_KAURI|CD_TK_SYNYU_8PER;	// 課税売上に係る
		break;
	case TK_HKATK8:	
		if( sytyp == KOBET_SYMST )	code = CD_TK_HIURI|CD_TK_SYNYU_8PER;	// 非課税売上に係る
		break;
	case TK_KYOTK8:	
		if( sytyp == KOBET_SYMST )	code = CD_TK_KYOURI|CD_TK_SYNYU_8PER;	// 共通に係る
		break;
	case TK_FUTOK8:	code = CD_FK_SYNYU_8PER;	break;

	// 10%
	case TK_SYNYU10:	code = CD_TK_SYNYU_10PER;	break;
	case TK_KATOK10:	
		if( sytyp == KOBET_SYMST )	code = CD_TK_KAURI|CD_TK_SYNYU_10PER;	// 課税売上に係る
		break;
	case TK_HKATK10:	
		if( sytyp == KOBET_SYMST )	code = CD_TK_HIURI|CD_TK_SYNYU_10PER;	// 非課税売上に係る
		break;
	case TK_KYOTK10:	
		if( sytyp == KOBET_SYMST )	code = CD_TK_KYOURI|CD_TK_SYNYU_10PER;	// 共通に係る
		break;
	case TK_FUTOK10:	code = CD_FK_SYNYU_10PER;	break;

	//8% 軽減
	case TK_SYNYU8K:	code = CD_TK_SYNYU_RD_8PER;	break;
	case TK_KATOK8K:
		if (sytyp == KOBET_SYMST)	code = CD_TK_KAURI | CD_TK_SYNYU_RD_8PER;	// 課税売上に係る
		break;
	case TK_HKATK8K:
		if (sytyp == KOBET_SYMST)	code = CD_TK_HIURI | CD_TK_SYNYU_RD_8PER;	// 非課税売上に係る
		break;
	case TK_KYOTK8K:
		if (sytyp == KOBET_SYMST)	code = CD_TK_KYOURI | CD_TK_SYNYU_RD_8PER;	// 共通に係る
		break;
	case TK_FUTOK8K:	code = CD_FK_SYNYU_RD_8PER;	break;

	default:
		break;
	}

	//個別対応
	if( sytyp == KOBET_SYMST ) {
		short uri_sw;
		if( rpline ) {
			uri_sw = rpline->RP_URISIRE2_DT.SKBN_SGN1;
		}
		else {
			uri_sw = m_svUriSire;
		}
		switch( uri_sw ) {
		case URI_KAZEI:		code |= CD_TK_KAURI;	break;	// 課税売上に係る
		case URI_HIKAZEI:	code |= CD_TK_HIURI;	break;	// 非課税売上に係る
		case URI_KYOTU:		code |= CD_TK_KYOURI;	break;	// 共通に係る
		}

		code |= CD_TK_SFT;
	}

	if( code > 0 ) {
		if( pDBsy->SyFnc( code ) == 0 ) {
			struct _SY_MSG_PACK2 smpk;
			pDBsy->SyMsg( &smpk );
			pDBzm->GetCDBData( pDt );
		}
	}
	else if( syzsw == TK_GAI ) {
		if( pDt->m_dsign[4] == 0x0c ) {
			pDBzm->dbdata->dsign[4] = 0;
			pDBzm->dbdata->dsign[5] = 0;
			pDt->m_dsign[4] = 0;
			pDt->m_dsign[5] = 0;
		}
	}

	return 0;
}

// 対価区分
int CRpkeyDlg::Set_Taika( struct _RPL_LINE* rpline, CDBINPDataRec* pDt )
{
	DWORD code = 0;

	code = m_svTaika;

	if( code > 0 ) {
		if( pDBsy->SyFnc( code ) == 0 ) {
			struct _SY_MSG_PACK2 smpk;
			pDBsy->SyMsg( &smpk );
			pDBzm->GetCDBData( pDt );
		}
	}

	return 0;
}

// 02.22 /02
/*================================================
		char* p ....... 置換を行う文字列開始位置
		char* pBfo .... 置換前の文字列
		char* pRp ..... 置換後の文字列
	---------------------------------------------
	返送値 0 = 置換していない, 1 = 置換した
  ================================================*/
int CRpkeyDlg::TkyoIns(char* p, char* pBfo, char* pRp)
{
	char wkbf[128] = {0};
	char temp[128] = {0};

	size_t bf_len = strlen((char*)pBfo);// 置換前文字列の長さ
	size_t rp_len = strlen((char*)pRp);	// 置換後文字列の長さ
	short j;
	int st;

	j = 0;

	short sp, nxtsp, max, cnt;
	char *pwkbf, *pnxt, *org;

	max = (pDBzm->zvol->tk_ln*2);
	j = kjlen( p, max );
	memcpy( wkbf, p, j );	// スペースを除く

	nxtsp = sp = 0;
	pwkbf = &wkbf[0];
	org = &wkbf[0];
	pnxt = pwkbf;

	CString before2;
	//半角ハイフンがある場合は、半角長音も調べる
	int chksgn = HaihunCheck( pBfo, before2 );
	int isReplace;

	// 置換してから再度検索文字があるかどうかを調べる
	for( sp = 0; sp < max /*&& *pwkbf != 0*/;  )
	{
		if( (max - sp) >= bf_len ) {
			isReplace = 0;

			if( ! chksgn ) {
				// Found or 置換前 入力なし
				if( !strncmp( pwkbf, pBfo, bf_len ) || !bf_len ) {
					isReplace = 1;
				}
			}
			else {
				if( !strncmp( pwkbf, pBfo, bf_len ) || !strncmp( pwkbf, (LPCTSTR)before2, bf_len ) || !bf_len ) {
					isReplace = 1;
				}
			}

			if( isReplace ) {
				// 次回検索開始位置を保存
				nxtsp = (sp + rp_len);

				// 置換文字列の後の文字列を一時ﾊﾞｯﾌｧへ
				if( ((max - sp)-bf_len) > 0 ) {
					memmove( temp, pwkbf+bf_len, (max - sp)-bf_len );
					temp[((max-sp)-bf_len)] = '\0';
				}
				else {
					::ZeroMemory( temp, sizeof(temp));
				}

//TRACE( "\nbf %d, nxtsp %d, aft %d, max %d", bf_len, nxtsp, ((max - sp) - bf_len), max );

				// 置換後も文字列セット可
				if( nxtsp < max) {
					memcpy( pwkbf, pRp, rp_len );

					*(pwkbf + rp_len) = '\0';

					// 置換前文字列がない時は、置換後の摘要に置き換えのみ
					if( !bf_len )
						break;

					// 置換後は元の文字列をｾｯﾄ(あれば)
					cnt = get_strcnt( temp, (max - nxtsp) );
					if( cnt ) {
						memcpy( (pwkbf + rp_len), temp, cnt );
						*(pwkbf + rp_len + cnt) = '\0';
					}

					// 次の検索開始位置をセット
					pnxt = (pwkbf + rp_len);
					pwkbf = pnxt;
					sp = nxtsp;

					continue;
				}
				else {
					// 今回置換分で 最大数を超えた -最大数まで文字列セット-
					pnxt = 0;

					cnt = get_strcnt( pRp, (max - sp) );
					if( cnt ) memcpy( pwkbf, pRp, cnt );

					if( (max - sp - cnt) > 0 ) {
						j = get_strcnt( temp, (max-sp-cnt));
						if( j ) memcpy( pwkbf + cnt, temp, j );
						else	*(pwkbf+cnt) = '\0';
					}
				
					break;
				}
			}
			else {
				if( *pwkbf == '\0' )	// 最終位置まできた。
					break;

				// 文字タイプ サーチ
				st = IsKindOfLetter( (LPCTSTR)org, sp );
				switch( st ) {
				case 1:		// ASCII 
					pwkbf++;	sp++;	break;
				case 2:		// MB文字１バイト目
					pwkbf += 2;	sp += 2;	break;
				default:
					return 0;
				}
			}
		}
		else	break;		// 置換前文字列なし
	}

	// 漢字スペース等のセット
	strcpy_s( p, max+1, wkbf );

	return 0;

}

// 消費税仕訳 区分 選択
void CRpkeyDlg::OnSelchangeScnsyzsw() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Syzsw.GetCurSel();
	DWORD itd = m_Syzsw.GetItemData( sel );

	m_Rpline.RP_SYZSW1_DT.SKBN_SGN1 = itd;	// 固有の情報
	m_Rpline.RP_SYZSW1_DT.SKBN_SGN2 = sel;	// 選択部分
	if( itd == NON_SELECT ) {
		strcpy_s( m_Rpline.RP_SYZSW1_DT.SKBN_TXT, sizeof m_Rpline.RP_SYZSW1_DT.SKBN_TXT, "" );
		m_Rpline.LINE_CND[ RP_SYZSW1_PN ].INP_sg = FALSE;
	}
	else {
		m_Syzsw.GetLBText( sel, m_Rpline.RP_SYZSW1_DT.SKBN_TXT );
		m_Rpline.LINE_CND[ RP_SYZSW1_PN ].INP_sg = TRUE;
	}	
}

void CRpkeyDlg::OnSelchangeScnsyzsw2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Syzsw2.GetCurSel();
	DWORD itd = m_Syzsw2.GetItemData( sel );

	m_Rpline.RP_SYZSW2_DT.SKBN_SGN1 = itd;
	m_Rpline.RP_SYZSW2_DT.SKBN_SGN2 = sel;
	if( itd == NON_SELECT ) {
		strcpy_s( m_Rpline.RP_SYZSW2_DT.SKBN_TXT, sizeof m_Rpline.RP_SYZSW2_DT.SKBN_TXT, "" );
		m_Rpline.LINE_CND[ RP_SYZSW2_PN ].INP_sg = FALSE;
	}
	else {
		m_Syzsw2.GetLBText( sel, m_Rpline.RP_SYZSW2_DT.SKBN_TXT );
		m_Rpline.LINE_CND[ RP_SYZSW2_PN ].INP_sg = TRUE;
	}		
}

// 税率選択
void CRpkeyDlg::OnSelchangeScnzritu() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Zritu.GetCurSel();
	DWORD itd = m_Zritu.GetItemData( sel );

	m_Rpline.RP_ZRITU1_DT.SKBN_SGN1 = itd;
	m_Rpline.RP_ZRITU1_DT.SKBN_SGN2 = sel;

	if( itd == NON_SELECT ) {
		strcpy_s( m_Rpline.RP_ZRITU1_DT.SKBN_TXT, sizeof m_Rpline.RP_ZRITU1_DT.SKBN_TXT, "" );
		m_Rpline.LINE_CND[ RP_ZRITU1_PN ].INP_sg = FALSE;
	}
	else {
		m_Zritu.GetLBText( sel, m_Rpline.RP_ZRITU1_DT.SKBN_TXT );
		m_Rpline.LINE_CND[ RP_ZRITU1_PN ].INP_sg = TRUE;
	}		
}

void CRpkeyDlg::OnSelchangeScnzritu2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Zritu2.GetCurSel();
	DWORD itd = m_Zritu2.GetItemData( sel );

	m_Rpline.RP_ZRITU2_DT.SKBN_SGN1 = itd;
	m_Rpline.RP_ZRITU2_DT.SKBN_SGN2 = sel;

	if( itd == NON_SELECT ) {
		strcpy_s( m_Rpline.RP_ZRITU2_DT.SKBN_TXT, sizeof m_Rpline.RP_ZRITU2_DT.SKBN_TXT, "" );
		m_Rpline.LINE_CND[ RP_ZRITU2_PN ].INP_sg = FALSE;
	}
	else {
		m_Zritu2.GetLBText( sel, m_Rpline.RP_ZRITU2_DT.SKBN_TXT );
		m_Rpline.LINE_CND[ RP_ZRITU2_PN ].INP_sg = TRUE;
	}			
}

// 税区分
void CRpkeyDlg::OnSelchangeScnskbn() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Skbn.GetCurSel();
	DWORD itd = m_Skbn.GetItemData( sel );

	m_Rpline.RP_SKBN1_DT.SKBN_SGN1 = itd;
	m_Rpline.RP_SKBN1_DT.SKBN_SGN2 = sel;

	if( itd == NON_SELECT ) {
		strcpy_s( m_Rpline.RP_SKBN1_DT.SKBN_TXT, sizeof m_Rpline.RP_SKBN1_DT.SKBN_TXT, "" );
		m_Rpline.LINE_CND[ RP_SKBN1_PN ].INP_sg = FALSE;
	}
	else {
		m_Skbn.GetLBText( sel, m_Rpline.RP_SKBN1_DT.SKBN_TXT );
		m_Rpline.LINE_CND[ RP_SKBN1_PN ].INP_sg = TRUE;
	}			
}

void CRpkeyDlg::OnSelchangeScnskbn2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Skbn2.GetCurSel();
	DWORD itd = m_Skbn2.GetItemData( sel );

	m_Rpline.RP_SKBN2_DT.SKBN_SGN1 = itd;
	m_Rpline.RP_SKBN2_DT.SKBN_SGN2 = sel;

	if( itd == NON_SELECT ) {
		strcpy_s( m_Rpline.RP_SKBN2_DT.SKBN_TXT, sizeof m_Rpline.RP_SKBN2_DT.SKBN_TXT, "" );
		m_Rpline.LINE_CND[ RP_SKBN2_PN ].INP_sg = FALSE;
	}
	else {
		m_Skbn2.GetLBText( sel, m_Rpline.RP_SKBN2_DT.SKBN_TXT );
		m_Rpline.LINE_CND[ RP_SKBN2_PN ].INP_sg = TRUE;
	}
}

// 売上・仕入区分
void CRpkeyDlg::OnSelchangeScnurisire() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Urisire.GetCurSel();
	DWORD itd = m_Urisire.GetItemData( sel );

	m_Rpline.RP_URISIRE1_DT.SKBN_SGN1 = itd;
	m_Rpline.RP_URISIRE1_DT.SKBN_SGN2 = sel;

	if( itd == NON_SELECT ) {
		strcpy_s( m_Rpline.RP_URISIRE1_DT.SKBN_TXT, sizeof m_Rpline.RP_URISIRE1_DT.SKBN_TXT, "" );
		m_Rpline.LINE_CND[RP_URISIRE1_PN ].INP_sg = FALSE;
	}
	else {
		m_Urisire.GetLBText( sel, m_Rpline.RP_URISIRE1_DT.SKBN_TXT );
		m_Rpline.LINE_CND[ RP_URISIRE1_PN ].INP_sg = TRUE;
	}		
}

void CRpkeyDlg::OnSelchangeScnurisire2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Urisire2.GetCurSel();
	DWORD itd = m_Urisire2.GetItemData( sel );

	m_Rpline.RP_URISIRE2_DT.SKBN_SGN1 = itd;
	m_Rpline.RP_URISIRE2_DT.SKBN_SGN2 = sel;

	if( itd == NON_SELECT ) {
		strcpy_s( m_Rpline.RP_URISIRE2_DT.SKBN_TXT, sizeof m_Rpline.RP_URISIRE2_DT.SKBN_TXT, "" );
		m_Rpline.LINE_CND[RP_URISIRE2_PN ].INP_sg = FALSE;
	}
	else {
		m_Urisire2.GetLBText( sel, m_Rpline.RP_URISIRE2_DT.SKBN_TXT );
		m_Rpline.LINE_CND[ RP_URISIRE2_PN ].INP_sg = TRUE;
	}	
}


// 特定収入
void CRpkeyDlg::OnSelchangeScnTokus() 
{
	int sel = m_Tokus.GetCurSel();
	DWORD itd = m_Tokus.GetItemData( sel );

	m_Rpline.RP_TOKUS1_DT.SKBN_SGN1 = itd;
	m_Rpline.RP_TOKUS1_DT.SKBN_SGN2 = sel;

	if( itd == NON_SELECT ) {
		strcpy_s( m_Rpline.RP_TOKUS1_DT.SKBN_TXT, sizeof m_Rpline.RP_TOKUS1_DT.SKBN_TXT, "" );
		m_Rpline.LINE_CND[RP_TOKUS1_PN ].INP_sg = FALSE;
	}
	else {
		m_Urisire.GetLBText( sel, m_Rpline.RP_TOKUS1_DT.SKBN_TXT );
		m_Rpline.LINE_CND[ RP_TOKUS1_PN ].INP_sg = TRUE;
	}		
}

void CRpkeyDlg::OnSelchangeScnTokus2() 
{
	int sel = m_Tokus2.GetCurSel();
	DWORD itd = m_Tokus2.GetItemData( sel );

//MyTrace( "OnSelchangeScnTokus2 sel = %d, itd = %d", sel, itd );

	m_Rpline.RP_TOKUS2_DT.SKBN_SGN1 = itd;
	m_Rpline.RP_TOKUS2_DT.SKBN_SGN2 = sel;

	if( itd == NON_SELECT ) {
		strcpy_s( m_Rpline.RP_TOKUS2_DT.SKBN_TXT, sizeof m_Rpline.RP_TOKUS2_DT.SKBN_TXT, "" );
		m_Rpline.LINE_CND[RP_TOKUS2_PN ].INP_sg = FALSE;
	}
	else {
		m_Tokus2.GetLBText( sel, m_Rpline.RP_TOKUS2_DT.SKBN_TXT );
		m_Rpline.LINE_CND[ RP_TOKUS2_PN ].INP_sg = TRUE;
	}	
}

//////////////////////////////////////////////////////////////
// PEN
//
#ifdef	_ICSPEN_	// 01.20 /01
//ペンDLL初期化
/*
short CRpkeyDlg::SetPen( BOOL flg, ICSPen* ppen )
{
	if( flg == FALSE )	return	0;

	if( flgPen != FALSE )	return	0;

	icspen	=	ppen;

	icspen->RegisterControl( this );

	flgPen	=	TRUE;

	return	0;
}
*/

//ペン表示ウィンドウ位置、サイズ
void CRpkeyDlg::SetPenRect( CRect& rect )
{
	rectPen	=	rect;
}

//消費税選択後の移動
void CRpkeyDlg::set_nextpos()
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

void CRpkeyDlg::OnICSInputFocus( int pn )
{
/*
	if( flgPen != FALSE ) {
		
		CRect	rect = ((CDBDinpView*)p_DBDinpView)->GetPenRect();

		switch( pn )
		{
		case RP_DENP1_PN:	//伝票番号（開始）
		case RP_DENP2_PN:	//伝票番号（終了）
		case RP_BMON1_PN:	//借方部門（開始）
		case RP_BMON2_PN:	//　　部門（終了）
		case RP_CBMN1_PN:	//貸方部門（開始）
		case RP_CBMN2_PN:	//　　部門（終了）
			icspen->ShowWindow( TRUE, PWINDOW_NUM );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_NUM );
			break;
		case RP_DATE1_PN:	//日付（開始）
		case RP_DATE2_PN:	//日付（終了）
			icspen->ShowWindow( TRUE, PWINDOW_MD );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_MD );
			break;
		case RP_DEBT1_PN:	//借方
		case RP_CRED1_PN:	//貸方
		case RP_DEBT2_PN:	//借方
		case RP_CRED2_PN:	//貸方
			icspen->ShowWindow( TRUE, PWINDOW_KANAK );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_KANAK );
			break;

		case RP_VAL1_PN:	//金額（開始）
		case RP_VAL2_PN:	//金額（終了）
			icspen->ShowWindow( TRUE, PWINDOW_VAL );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_VAL );
			break;
		case RP_SYZSW1_PN:	case RP_SYZSW2_PN:	
		case RP_ZRITU1_PN:	case RP_ZRITU2_PN:	
		case RP_SKBN1_PN:	case RP_SKBN2_PN:	
		case RP_URISIRE1_PN:	case RP_URISIRE2_PN:	
			icspen->ShowWindow( FALSE );
//			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_VAL );
			break;
		case RP_TKY1_PN:	//摘要
		case RP_TKY2_PN:	//摘要
			icspen->ShowWindow( TRUE, PWINDOW_KANAT );
			icspen->ShowWindow( TRUE, PWINDOW_GAIJI );
			icspen->ShowWindow( TRUE, PWINDOW_PEN );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_KANAT );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_GAIJI );
			icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_PEN );
			break;

		}
	}
*/
}
#endif


void CRpkeyDlg::OnBtnDel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int st = ICSMessageBox("全項目の削除を行います。\r\n\r\nよろしいですか？", MB_YESNO, 0,0,this);

	if( st == IDYES ) {
		keyline_inz();
		keyline_dsp(&m_Rpline);

		set_focus(RP_DATE1_PN);
	}	
}

// backSpaceで消されたときのため
void CRpkeyDlg::last_dataget()
{
	//摘要取得
	struct _RPL_LINE* dtline;
	dtline = &m_Rpline;

	if( ! dtline->LINE_CND[RP_TKY1_PN].IMG_sg ) {
		OnTerminationRptky1(0,0,0);
	}
	if( ! dtline->LINE_CND[RP_TKY2_PN].IMG_sg ) {
		OnTerminationRptky2(0,0,0);
	}
}


void CRpkeyDlg::OnOK() 
{
//MyTrace( "[IKTError] CRpkeyDlg::OnOK TOP\n" );

	//最終データ取得
	last_dataget();

	// 修正用の警告メッセージ
	if( SyuseiWarning() != 0 )
		return;

//MyTrace( "[IKTError] CRpkeyDlg::OnOK(2)\n" );

	// 科目の消費税のﾁｪｯｸ
	if( KmkSyzChk() != 0 )	return;

//MyTrace( "[IKTError] CRpkeyDlg::OnOK(3)\n" );

	ICSDialog::OnOK();
}


// 10.26 /01	修正前の項目が指定されていない時
//	return  1 = ﾒｯｾｰｼﾞﾎﾞｯｸｽでいいえが押された
//			0 = ﾁｪｯｸOK
//
int CRpkeyDlg::SyuseiWarning()
{
	// 修正前の項目が入力されているかのﾁｪｯｸ
	int st, k, rt_pn;
	CString msg, tmp;
	short chkflg = 0, bf_chk = 0;

	rt_pn = -1;

	for( k = 0; k < sizeof(CHK_PNTBL)/sizeof(CHK_PNTBL[0]); k++ ) {
		// 修正後項目チェック
		if( IsInputSyuseigo( CHK_PNTBL[k].a_pn ) ) {
			chkflg = 1;

			// 仕訳 確定 チェック
			if( CHK_PNTBL[k].a_pn == RP_HSEN2_PN || CHK_PNTBL[k].a_pn == RP_CONFIRM_PN || CHK_PNTBL[k].a_pn == RP_CONMDFY_PN ) {
				CHK_PNTBL[k].pnsw = 0;
				continue;
			}

			// インボイス対応
			// 免税事業者からの課税仕入れチェック
			if(( CHK_PNTBL[k].a_pn == RP_MENZEI_PN ) || ( CHK_PNTBL[k].a_pn == RP_KAZEI_PN )) {
				CHK_PNTBL[k].pnsw = 0;
				continue;
			}

			// 修正前の項目チェック <入力可の項目のみ>
			if( !m_Rpline.LINE_CND[ CHK_PNTBL[k].b_pn ].INP_sg && getINP_CTL(CHK_PNTBL[k].b_pn)->INP_ena ) {
				// 修正後の入力があって修正前が未入力
				CHK_PNTBL[k].pnsw = 1;
				bf_chk = 1;
				// フォーカス戻し用
				if( rt_pn == -1 )	rt_pn = CHK_PNTBL[k].b_pn;
			}
			else {
				CHK_PNTBL[k].pnsw = 0;
			}
		}
		else {
			CHK_PNTBL[k].pnsw = 0;
		}
	}
	if( !chkflg ) {
		ICSMessageBox( _T( "修正後の項目が入力されていません！" ),0,0,0,this);
		set_focus(RP_DATE1_PN);
		return -1;
	}

	if( bf_chk ) {
		msg = _T( "以下の修正前項目が入力されていません！\r\n\r\n" );

		GetNotInputStr(tmp);

		msg += tmp;

		msg += _T( "\r\n\r\nこのまま修正を行うと、検索条件に一致するすべての仕訳について\r\n上記の項目が修正されます。" );
		msg += _T( "\r\n修正処理を開始しますか？" );

		st = ICSMessageBox( msg, MB_YESNO|MB_DEFBUTTON2, 0,0,this);

		if( st != IDYES ) {
			set_focus( rt_pn );
			return -1;
		}
	}

	return 0;
}

// 修正後データを入力されているか
// 07.01 /03
BOOL CRpkeyDlg::IsInputSyuseigo( int chk_pn )
{
	BOOL bret = FALSE;

	if( chk_pn == RP_DENP2_PN || chk_pn == RP_BMON2_PN || chk_pn == RP_CBMN2_PN ||
		chk_pn == RP_KOJI2_PN || chk_pn == RP_CKJI2_PN || chk_pn == RP_TKY2_PN || chk_pn == RP_DOCEVI2_PN ) {
		// 伝票番号＆部門なし・摘要取消チェック
		if( m_Rpline.LINE_CND[ chk_pn ].INP_sg || m_Rpline.LINE_CND[ chk_pn ].INP_type )
			bret = TRUE;
	}
	else {
		if( m_Rpline.LINE_CND[ chk_pn ].INP_sg )
			bret = TRUE;
	}

	return bret;
}

// 修正前未入力の文字列をセット
void CRpkeyDlg::GetNotInputStr(CString& tmp)
{
	int sw, cnt, id;
	char buf[64];
	CString work;

	tmp.Empty();
	cnt = 0;

	for(int k = 0; k < sizeof(CHK_PNTBL)/sizeof(CHK_PNTBL[0]); k++ ) {
		sw = CHK_PNTBL[k].pnsw;		// 未入力サイン

		switch( CHK_PNTBL[k].b_pn )
		{
		case RP_DATE1_PN:
			id = IDC_ST_DATE;	break;
		case RP_DENP1_PN:
			id = IDC_ST_DENP;	break;
		case RP_BMON1_PN:
			id = IDC_ST_DBMN;	break;
		case RP_CBMN1_PN:
			id = IDC_ST_CBMN;	break;
		case RP_KOJI1_PN:
			id = IDC_ST_DKJ;	break;
		case RP_CKJI1_PN:
			id = IDC_ST_CKJ;	break;

		case RP_DEBT1_PN:
			id = IDC_ST_DEBT;	break;
		case RP_CRED1_PN:
			id = IDC_ST_CRED;	break;
		case RP_VAL1_PN:
			id = IDC_ST_VAL;	break;

		case RP_SYZSW1_PN:
			id = IDC_ST_SYZSW;	break;
		case RP_ZRITU1_PN:
			id = IDC_ST_ZRITU;	break;
		case RP_SKBN1_PN:
			id = IDC_ST_SKBN;	break;
			break;
		case RP_URISIRE1_PN:
			id = IDC_ST_URISIRE;	break;
			break;
		// 特定収入
		case RP_TOKUS1_PN:
			id = IDC_ST_TOKUS;	break;
			break;

		case RP_TKY1_PN:
			id = IDC_ST_TKY;	break;
			break;
		case RP_HJIYU1_PN:
			id = IDC_ST_HJIYU;	break;
			break;

		case RP_SNUM1_PN:
			id = IDC_ST_SNUM;	break;
			break;
		case RP_TDAT1_PN:
			id = IDC_ST_TDAT;	break;
			break;

		case RP_DOCEVI1_PN:
			id = IDC_ST_SNO;	break;

		case RP_INVNO_PN:
			id = IDC_ST_DEBT_INVNO;	break;

		default:	break;
		}

		if( sw ) {
			GetDlgItem( id )->GetWindowText( work );
			wsprintf( buf, _T( "『%s』　"), work.SpanExcluding( _T("：") ) );
			tmp += buf;

			if( !(++cnt % 3) ) {
				tmp += _T( "\r\n" );
			}
		}
	}
}


// 科目名称から消費税属性文字列取得
char* GetKNsz3str( DBKNREC* pKN )
{
static char* pAtt[] = {"対象外", "不課税", "課税", "非課税"};
static char* pNULL = "";

	short pos = (pKN->knsgn[2]&0x0f);

	if( pos >= (sizeof(pAtt) / sizeof(pAtt[0]) ) )
		return pNULL;
	else
		return pAtt[pos];
}


// 科目の消費税のﾁｪｯｸ
//	return  1 = ﾒｯｾｰｼﾞﾎﾞｯｸｽでいいえが押された
//			0 = ﾁｪｯｸOK
//
int CRpkeyDlg::KmkSyzChk()
{
	DBKNREC *pDEB1, *pDEB2, *pCRE1, *pCRE2;
	pDEB1 = pDEB2 = pCRE1 = pCRE2 = NULL;

	if( m_Rpline.LINE_CND[ RP_DEBT1_PN ].INP_sg )
		pDEB1 = pDBzm->DB_PjisToKnrec( m_Rpline.RP_DEBT1_DT.IP_CdNum );

	if( m_Rpline.LINE_CND[ RP_DEBT2_PN ].INP_sg )
		pDEB2 = pDBzm->DB_PjisToKnrec( m_Rpline.RP_DEBT2_DT.IP_CdNum );

	if( m_Rpline.LINE_CND[ RP_CRED1_PN ].INP_sg )
		pCRE1 = pDBzm->DB_PjisToKnrec( m_Rpline.RP_CRED1_DT.IP_CdNum );

	if( m_Rpline.LINE_CND[ RP_CRED2_PN ].INP_sg )
		pCRE2 = pDBzm->DB_PjisToKnrec( m_Rpline.RP_CRED2_DT.IP_CdNum );

	CString msg, tmp;
	int chkflg, rval;
	chkflg = rval = 0;

	// 科目が修正されない時
	if( pDEB2 == NULL && pCRE2 == NULL ) return 0;

	if( !m_Rpline.LINE_CND[ RP_SKBN2_PN ].INP_sg ) {
		msg = "修正後の消費税区分が入力されていません！\r\n\r\n";

		int dbt_syz, cre_syz;
		dbt_syz = cre_syz = -1;

		int diff_syz = 0;
		int mae_msg = 0;
		int mae_msg2 = 0;

		if( pDEB2 != NULL ) {
			dbt_syz = 0;
			if( pDEB1 != NULL ) {
				if( (pDEB1->knsgn[2]&0x0f) != (pDEB2->knsgn[2]&0x0f) ) {
					msg += "消費税区分に相違があります。\r\n";
					diff_syz |= 0x01;

					tmp.Format( "\r\n借方修正前『%.14s』[%s]\r\n借方修正後『%.14s』[%s]\r\n" ,pDEB1->knnam, GetKNsz3str(pDEB1), pDEB2->knnam, GetKNsz3str(pDEB2) );
					chkflg++;
					msg += tmp;
					//対象外なので修正後の科目の消費税になる。
					if( (pDEB1->knsgn[2]&0x0f) == 0 )	dbt_syz = 1;
	
					// 課税等→対象外
					if( (pDEB2->knsgn[2]&0x0f) == 0 ) {
						diff_syz |= 0x10;
						mae_msg++;
					}

					// 対象外→課税等
					if( (pDEB1->knsgn[2]&0x0f) == 0 )	mae_msg2++;
				}
			}
		}
		if( pCRE2 != NULL ) {
			cre_syz = 0;
			if( pCRE1 != NULL ) {
				if( (pCRE1->knsgn[2]&0x0f) != (pCRE2->knsgn[2]&0x0f) ) {
					if( ! diff_syz ) {
						msg += "消費税区分に相違があります。\r\n";
					}
					diff_syz |= 0x02;

					tmp.Format( "\r\n貸方修正前『%.14s』[%s]\r\n貸方修正後『%.14s』[%s]\r\n" ,pCRE1->knnam, GetKNsz3str(pCRE1), pCRE2->knnam, GetKNsz3str(pCRE2) );
					chkflg++;
					msg += tmp;
					//対象外なので修正後の科目の消費税になる。
					if( (pCRE1->knsgn[2]&0x0f) == 0 )	cre_syz = 1;

					// 対象外 → 課税等
					if( (pCRE1->knsgn[2]&0x0f) == 0 )	mae_msg++;

					// 課税等 → 対象外
					if( (pCRE2->knsgn[2]&0x0f) == 0 ) {
						diff_syz |= 0x20;
						mae_msg2++;
					}
				}
			}
		}

		if( chkflg ) {
			int ato_chk = 0;
			// 借・貸 いずれか一方に消費税区分の相違があるとき、かつ修正前が課税・不課税・非課税で修正後が対象外のとき
			if( (diff_syz&0x0f) == 0x01 ) {
				if( diff_syz & 0x10 )	ato_chk++;
			}
			else if( (diff_syz&0x0f) == 0x02 ) {
				if( diff_syz & 0x20 )	ato_chk++;
			}

//MyTrace( "dbt_syz(%d), cre_syz(%d), ato_chk(%d), mae_msg(%d), mae_msg2(%d)\n", dbt_syz, cre_syz, ato_chk, mae_msg, mae_msg2 );

			if( (dbt_syz > 0 || cre_syz > 0 || ato_chk > 0 ) && mae_msg != 2 && mae_msg2 != 2 ) {
				msg += "\r\nこのまま処理を進めると、修正後の科目の消費税区分になります。";
			}
			else {
				msg += "\r\nこのまま処理を進めると、修正前の仕訳の消費税区分が\r\n修正後の仕訳に反映されます。\
\r\n反映したくない場合は「いいえ」を押して一旦戻り、\r\n修正後の消費税区分を設定してください。";
			}

			msg += "\r\n\r\n修正処理を行いますか？";

			if( ICSMessageBox(msg, MB_YESNO,0,0,this) == IDYES )
				rval = 0;
			else
				rval = 1;
		}
	}

	return rval;
}


// 科目カナターミネーション
void CRpkeyDlg::KanaTerminationKamoku( UINT ID, LPCTSTR kana )
{
#ifdef OLD_CLOSE
	KSEL_PAR kpar;
	CRect rc;
	GetDlgItem(ID)->GetWindowRect( rc );

	struct _KamokuData ans = {0};

	pDBzm->knrec->Requery("", 0);

	kpar.ksPsgn	= KSP_LEFTBOTTOM;
	kpar.ksFsgn = 0;
	kpar.ksLnum = 10;
	::ZeroMemory( kpar.ksKana, sizeof kpar.ksKana );
	strncpy_s( kpar.ksKana, sizeof kpar.ksKana, kana, sizeof kpar.ksKana-1 );
	kpar.ksKrng = 0;
	kpar.ksPos.x	= rc.left;
	kpar.ksPos.y	= rc.top;
	kpar.ksCwnd = this;
	kpar.ksStat = 0;
	kpar.ksFhwd = m_Ok.m_hWnd;

	if( Kmk50onSelect(pDBzm, &kpar, this) ) {
		memmove( (char *)ans.kd_name, (LPCTSTR)pDBzm->knrec->knnam, KMNLEN );
		strcpy_s( ans.kd_code, sizeof ans.kd_code, pDBzm->knrec->kncod );
		ans.kd_eda = -1;

		struct _ItemData* keybuf;

		switch( ID ) {
		case IDC_SCNDEBT:	keybuf = &m_Rpline._RPDTA[ RP_DEBT1_PN ];	break;
		case IDC_SCNDEBT2:	keybuf = &m_Rpline._RPDTA[ RP_DEBT2_PN ];	break;
		case IDC_SCNCRED:	keybuf = &m_Rpline._RPDTA[ RP_CRED1_PN ];	break;
		case IDC_SCNCRED2:	keybuf = &m_Rpline._RPDTA[ RP_CRED2_PN ];	break;
		}

		::ZeroMemory( &keybuf->IP_stKMK, sizeof keybuf->IP_stKMK );
		memcpy( &keybuf->IP_stKMK, &ans, sizeof keybuf->IP_stKMK );

		DBkamokudata_set( this, ID, &keybuf->IP_stKMK, pDBzm );
	}
#endif

	int nowmode, mode;
	char tmp[64] = {0};
	char kanabf[64] = {0};
	nowmode = pZmSel->GetCrntMode();
	mode = -1;
	int firstkana = 0;

	if( KanaSearchType() == 0 ) {	//１文字検索
	kana_copy( kanabf, kana );
	}
	else {
		kana_check(kanabf, kana);
	}

	// 濁点等は除く
	if( (BYTE)kanabf[0] == (BYTE)0xDE || (BYTE)kanabf[0] == (BYTE)0xDF ) {
		return;
	}

	if( nowmode == ZSEL_KAMOKU	) {
		mode = ZSEL_KMKANA;
		strcpy_s( tmp, sizeof tmp, kanabf );
		::ZeroMemory( m_saveKana, sizeof m_saveKana );
		strcpy_s( m_saveKana, sizeof m_saveKana, tmp );
		pZmSel->SetKamokuKana( tmp );
		pZmSel->SetBrnTkyKana("");
		firstkana = 1;
	}
	else if( nowmode == ZSEL_KMKANA ) {
		mode = ZSEL_KMKANA;
		strcpy_s( tmp, sizeof tmp, kanabf );
#ifdef CLOSE // 07.17 /07
		int n = sprintf_s( tmp, sizeof tmp, "%s%s", m_saveKana, kanabf );
		if( n >= 0 ) {
			if( n >= sizeof m_saveKana )	n = sizeof m_saveKana - 1;
			memmove( m_saveKana, tmp, n );
			m_saveKana[n] = '\0';
		}
		else	::ZeroMemory( m_saveKana, sizeof m_saveKana );
#endif
		pZmSel->SetKamokuKana( tmp );
TRACE("SetKamokuKana tmp = %s\n", tmp);
		pZmSel->SetBrnTkyKana("");
	}
	else if( nowmode == ZSEL_BRNTKY ) {
		mode = ZSEL_BRNTKY;
		strcpy_s(tmp, sizeof tmp, kanabf);
	//	sprintf_s( tmp, sizeof tmp, "%s%s", m_saveKana, kanabf );
	//	strcpy_s( m_saveKana, sizeof m_saveKana, tmp );
		pZmSel->SetBrnTkyKana( tmp );

		if( m_saveKana[0] == 0 )
			firstkana = 1;
	}

	if( mode != -1 ) {
		int pn;
		switch( ID ) {
		case IDC_SCNDEBT:	pn = RP_DEBT1_PN;	break;
		case IDC_SCNDEBT2:	pn = RP_DEBT2_PN;	break;
		case IDC_SCNCRED:	pn = RP_CRED1_PN;	break;
		case IDC_SCNCRED2:	pn = RP_CRED2_PN;	break;
		}

		KamokuSelDisp( pn, mode );

		if( KanaSearchType() ) {	//複数検索
			int newmd = pZmSel->GetCrntMode();

			//カナ検索がなかった場合、前のカナで検索
			if( newmd == ZSEL_KAMOKU && ! firstkana ) {
				strcpy_s(tmp, sizeof tmp, m_saveKana);
				if( mode == ZSEL_BRNTKY ) {
					pZmSel->SetBrnTkyKana(tmp);
				}
				else {
					pZmSel->SetKamokuKana(tmp);
				}
				KamokuSelDisp(pn, mode);
			}
			else {
				strcpy_s(m_saveKana, sizeof m_saveKana, tmp);
			}
		}
	}
}


//伝票番号なし
void CRpkeyDlg::OnBnClickedCheckDenp()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	UpdateData();
	m_Rpline.LINE_CND[ RP_DENP2_PN ].INP_type = m_NoDenp ? 1 : 0;

	CWnd* pwnd;
	pwnd = GetDlgItem( getINP_CTL(RP_DENP2_PN)->IDC_X );
	((CICSDBEDT*)pwnd)->SetEnabled( m_NoDenp ? FALSE : TRUE );

	getINP_CTL(RP_DENP2_PN)->INP_ena = m_NoDenp ? FALSE : TRUE;
}

// 借方部門なし
void CRpkeyDlg::OnBnClickedCheckDbm()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	UpdateData();
	m_Rpline.LINE_CND[ RP_BMON2_PN ].INP_type = m_NoDbmn ? 1 : 0;

	CWnd* pwnd;
	pwnd = GetDlgItem( getINP_CTL(RP_BMON2_PN)->IDC_X );
	((CICSDBEDT*)pwnd)->SetEnabled( m_NoDbmn ? FALSE : TRUE );

	getINP_CTL(RP_BMON2_PN)->INP_ena = m_NoDbmn ? FALSE : TRUE;
}

// 貸方部門なし
void CRpkeyDlg::OnBnClickedCheckCbm()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	UpdateData();
	m_Rpline.LINE_CND[ RP_CBMN2_PN ].INP_type = m_NoCbmn ? 1 : 0;

	CWnd* pwnd;
	pwnd = GetDlgItem( getINP_CTL(RP_CBMN2_PN)->IDC_X );
	((CICSDBEDT*)pwnd)->SetEnabled( m_NoCbmn ? FALSE : TRUE );

	getINP_CTL(RP_CBMN2_PN)->INP_ena = m_NoCbmn ? FALSE : TRUE;
}

// 借方工事なし
void CRpkeyDlg::OnBnClickedCheckDkj()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	UpdateData();
	m_Rpline.LINE_CND[ RP_KOJI2_PN ].INP_type = m_NoDkoji ? 1 : 0;

	CWnd* pwnd;
	pwnd = GetDlgItem( getINP_CTL(RP_KOJI2_PN)->IDC_X );
	((CICSDBEDT*)pwnd)->SetEnabled( m_NoDkoji ? FALSE : TRUE );

	getINP_CTL(RP_KOJI2_PN)->INP_ena = m_NoDkoji ? FALSE : TRUE;
}

// 貸方工事なし
void CRpkeyDlg::OnBnClickedCheckCkj()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	UpdateData();
	m_Rpline.LINE_CND[ RP_CKJI2_PN ].INP_type = m_NoCkoji ? 1 : 0;

	CWnd* pwnd;
	pwnd = GetDlgItem( getINP_CTL(RP_CKJI2_PN)->IDC_X );
	((CICSDBEDT*)pwnd)->SetEnabled( m_NoCkoji ? FALSE : TRUE );

	getINP_CTL(RP_CKJI2_PN)->INP_ena = m_NoCkoji ? FALSE : TRUE;
}


// 変動事由
void CRpkeyDlg::OnCbnSelchangeScnhjiyu()
{
	int sel = m_Hjiyu1.GetCurSel();
	DWORD itd = m_Hjiyu1.GetItemData( sel );

	m_Rpline.RP_HJIYU1_DT.SKBN_SGN1 = itd;
	m_Rpline.RP_HJIYU1_DT.SKBN_SGN2 = sel;

	if( itd == NON_SELECT ) {
		strcpy_s( m_Rpline.RP_HJIYU1_DT.SKBN_TXT, sizeof m_Rpline.RP_HJIYU1_DT.SKBN_TXT, "" );
		m_Rpline.LINE_CND[ RP_HJIYU1_PN ].INP_sg = FALSE;
	}
	else {
		m_Hjiyu1.GetLBText( sel, m_Rpline.RP_HJIYU1_DT.SKBN_TXT );
		m_Rpline.LINE_CND[ RP_HJIYU1_PN ].INP_sg = TRUE;
	}
}

void CRpkeyDlg::OnCbnSelchangeScnhjiyu2()
{
	int sel = m_Hjiyu2.GetCurSel();
	DWORD itd = m_Hjiyu2.GetItemData( sel );

	m_Rpline.RP_HJIYU2_DT.SKBN_SGN1 = itd;
	m_Rpline.RP_HJIYU2_DT.SKBN_SGN2 = sel;

	if( itd == NON_SELECT ) {
		strcpy_s( m_Rpline.RP_HJIYU2_DT.SKBN_TXT, sizeof m_Rpline.RP_HJIYU2_DT.SKBN_TXT, "" );
		m_Rpline.LINE_CND[ RP_HJIYU2_PN ].INP_sg = FALSE;
	}
	else {
		m_Hjiyu2.GetLBText( sel, m_Rpline.RP_HJIYU2_DT.SKBN_TXT );
		m_Rpline.LINE_CND[ RP_HJIYU2_PN ].INP_sg = TRUE;
	}
}

// 付箋
void CRpkeyDlg::OnCbnSelchangeScnhusen()
{
	int st = m_Hcbo.GetCurSel();
	m_Rpline.RP_HSEN1_DT.IP_HUSEN = st;
	m_Rpline.LINE_CND[ RP_HSEN1_PN ].INP_sg = (st > 0) ? TRUE : FALSE;
}


void CRpkeyDlg::OnCbnSelchangeScnhusen2()
{
	UpdateData();

	if( m_ChkHusen ) {
		m_Rpline.RP_HSEN2_DT.IP_HUSEN = 0;
	}
	else {
		int st = m_Hcbo2.GetCurSel();
		m_Rpline.RP_HSEN2_DT.IP_HUSEN = st;
		m_Rpline.LINE_CND[ RP_HSEN2_PN ].INP_sg = (st > 0) ? TRUE : FALSE;
	}
}

long CRpkeyDlg::OnTerminationScnhusen(long nChar) 
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_HSEN1_PN, nChar ) != 0 ) {
		if( nChar != VK_DOWN && nChar != VK_UP && nChar == VK_ESCAPE ) {
			Buzzer();
		}
		return nChar;
	}
	return 0;
}


long CRpkeyDlg::OnTerminationScnhusen2(long nChar) 
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_HSEN2_PN, nChar ) != 0 ) {
		if( nChar != VK_DOWN && nChar != VK_UP && nChar == VK_ESCAPE ) {
			Buzzer();
		}
		return nChar;
	}
	return 0;
}

// 付箋取消
void CRpkeyDlg::OnBnClickedChkHusen()
{
	UpdateData();
	m_Rpline.LINE_CND[ RP_HSEN2_PN ].INP_sg = m_ChkHusen ? TRUE : FALSE;
	m_Rpline.LINE_CND[ RP_HSEN2_PN ].INP_type = m_ChkHusen ? 1 : 0;

	if( m_ChkHusen ) {
		m_Hcbo2.EnableWindow(FALSE);
	}
	else {
		m_Hcbo.EnableWindow(TRUE);
		m_Hcbo2.EnableWindow(TRUE);

		int st = m_Hcbo.GetCurSel();
		m_Rpline.RP_HSEN1_DT.IP_HUSEN = st;
		m_Rpline.LINE_CND[ RP_HSEN1_PN ].INP_sg = (st > 0) ? TRUE : FALSE;

		st = m_Hcbo2.GetCurSel();
		m_Rpline.RP_HSEN2_DT.IP_HUSEN = st;
		m_Rpline.LINE_CND[ RP_HSEN2_PN ].INP_sg = (st > 0) ? TRUE : FALSE;
	}
}

//付箋取消 キー移動
long CRpkeyDlg::OnTerminationChkHusen(long nChar)
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	switch( nChar ) {
	case VK_RIGHT:
		set_focus( RP_IDOK );
		break;
	default:
		if( term_focus( RP_HSENCHK_PN, nChar ) != 0 ) {
			return nChar;
		}
		break;
	}

	return 0;
}

//月のみ修正 キー移動
long CRpkeyDlg::OnTerminationChkMnonly(long nChar)
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_MNONLY_PN, nChar ) != 0 ) {
		return nChar;
	}

	return 0;
}

// 仕訳確定
void CRpkeyDlg::OnBnClickedChkConfirm()
{
	UpdateData();
	m_Rpline.LINE_CND[ RP_CONFIRM_PN ].INP_sg = m_Confirm ? TRUE : FALSE;
	m_Rpline.LINE_CND[ RP_CONFIRM_PN ].INP_type = m_Confirm ? 1 : 0;
	// 仕訳を確定する場合は、確定仕訳を修正可能にはできない
	if( m_Confirm ) {
		if( m_ConMdfy ) {
			m_ConMdfy = FALSE;
			m_Rpline.LINE_CND[ RP_CONMDFY_PN ].INP_type = 0;
			UpdateData( FALSE );
		}
	}
}

//仕訳確定 キー移動
long CRpkeyDlg::OnTerminationConfirm(long nChar)
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	switch( nChar ) {
	case VK_RIGHT:
		set_focus( RP_IDOK );
		break;
	default:
		if( term_focus( RP_CONFIRM_PN, nChar ) != 0 ) {
			return nChar;
		}
		break;
	}

	return 0;
}


// 仕訳確定
void CRpkeyDlg::OnBnClickedChkConMdfy()
{
	UpdateData();
	m_Rpline.LINE_CND[ RP_CONMDFY_PN ].INP_sg = m_ConMdfy ? TRUE : FALSE;
	m_Rpline.LINE_CND[ RP_CONMDFY_PN ].INP_type = m_ConMdfy ? 1 : 0;
	// 確定仕訳を修正可能する場合は、仕訳を確定できない
	if( m_ConMdfy ) {
		if( m_Confirm ) {
			m_Confirm = FALSE;
			m_Rpline.LINE_CND[ RP_CONFIRM_PN ].INP_type = 0;
			UpdateData( FALSE );
		}
	}
}

//仕訳確定 キー移動
long CRpkeyDlg::OnTerminationConMdfy(long nChar)
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	switch( nChar ) {
	case VK_RETURN:
	case VK_TAB:
	case VK_RIGHT:
		set_focus( RP_IDOK );
		break;
	default:
		if( term_focus( RP_CONMDFY_PN, nChar ) != 0 ) {
			return nChar;
		}
		break;
	}

	return 0;
}


//
//	摘要欄のイメージダイアログを表示用のレコードをセット
//
void CRpkeyDlg::SetTekiyoDlgRecord( CDBINPDataRec* data )
{
	m_TkyData = *data;
}


//仕訳の取消
void CRpkeyDlg::OnBnClickedSwkdel()
{
	int st = ICSMessageBox("仕訳の取消を行います。\r\n\r\nよろしいですか？", MB_YESNO|MB_DEFBUTTON2, 0,0,this);

	if( st == IDYES ) {
		m_bSwkDel = TRUE;
		EndDialog(IDOK);
	}
}

// 確定仕訳 を未確定にするのチェック
BOOL CRpkeyDlg::IsConmdfyCheck()
{
	struct _RPL_LINE *key = &m_Rpline;

	if( key->LINE_CND[ RP_CONMDFY_PN ].INP_sg ) {
		// 確定仕訳 を未確定に
		if( key->LINE_CND[ RP_CONMDFY_PN ].INP_type ) {
			return TRUE;
		}
	}

	return FALSE;
}

// 摘要取消
void CRpkeyDlg::OnBnClickedCheckTky()
{
	UpdateData();
	m_Rpline.LINE_CND[ RP_TKY2_PN ].INP_type = m_NoTky ? 1 : 0;

	CWnd* pwnd;
	pwnd = GetDlgItem( getINP_CTL(RP_TKY2_PN)->IDC_X );
	((CICSDBEDT*)pwnd)->SetEnabled( m_NoTky ? FALSE : TRUE );

	getINP_CTL(RP_TKY2_PN)->INP_ena = m_NoTky ? FALSE : TRUE;
}


//摘要取消 ターミネーション
long CRpkeyDlg::OnTerminationNoTky(long nChar)
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_NOTKY_PN, nChar ) != 0 ) {
	//	Buzzer();
		return nChar;
	}
	return 0;
}


//	未確定チェックボックスの表示
void CRpkeyDlg::mikaku_disp( int disp )
{
	BOOL bEntry;
#ifdef _DEBUG
	bEntry = TRUE;
#else
	bEntry = GET_EntrySign();
#endif

	if( ! bEntry )	return;

	struct _InpCtl*	ctl;
	ctl = RPINP_CTL + RP_CONMDFY_PN;

	if( ! (Voln1->tl_cor & 0x10) || disp == 0 || IsJzSansyo() ) {	//確定マスター修正不可 or 上手君dbNL
		ctl->INP_ena = FALSE;
		GetDlgItem( ctl->IDC_X)->EnableWindow( FALSE );

		GetDlgItem(IDC_ST_CONFIRM)->ShowWindow( SW_HIDE);
		GetDlgItem( ctl->IDC_X)->ShowWindow( SW_HIDE);
	}
	else if( ! IsModifyConfirmData() || IsEnkakuUser() ) {	// 確定仕訳修正不可・遠隔ユーザの場合、未確定にさせない。
		ctl->INP_ena = FALSE;
		GetDlgItem( ctl->IDC_X)->EnableWindow( FALSE );
	}
	else {
		ctl->INP_ena = TRUE;
		GetDlgItem( ctl->IDC_X)->EnableWindow( TRUE );
	}

	if( disp ) {
		GetDlgItem(IDC_ST_CONFIRM)->ShowWindow( SW_SHOW );
		GetDlgItem( ctl->IDC_X)->ShowWindow( SW_SHOW );
	}
}


// 月日 or 月のみ
void CRpkeyDlg::OnBnClickedCheckMnonly()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	// 月・月日の情報を保存する。
	if( m_Rpline.LINE_CND[ RP_DATE1_PN ].INP_sg ) {
		if( m_ChkMnonly ){
			memcpy( m_MNTHbcd1, m_Rpline.RP_DATE1_DT.IP_BCDDAY, 4 );
		}
		else {
			memcpy( m_MMDDbcd1, m_Rpline.RP_DATE1_DT.IP_BCDDAY, 4 );
		}
	}
	else {
		if( m_ChkMnonly )	memset( m_MNTHbcd1, '\xff', sizeof m_MNTHbcd1 );
		else				memset( m_MMDDbcd1, '\xff', sizeof m_MMDDbcd1 );
	}

	if( m_Rpline.LINE_CND[ RP_DATE2_PN ].INP_sg ) {
		if( m_ChkMnonly ){
			memcpy( m_MNTHbcd2, m_Rpline.RP_DATE2_DT.IP_BCDDAY, 4 );
		}
		else {
			memcpy( m_MMDDbcd2, m_Rpline.RP_DATE2_DT.IP_BCDDAY, 4 );
		}
	}
	else {
		if( m_ChkMnonly )	memset( m_MNTHbcd2, '\xff', sizeof m_MNTHbcd2 );
		else				memset( m_MMDDbcd2, '\xff', sizeof m_MMDDbcd2 );
	}

	UpdateData();
	m_Rpline.LINE_CND[ RP_DATE2_PN ].INP_type = m_ChkMnonly ? 1 : 0;

	_InpCtl* ctl;

	BYTE bcd[4] = {0};
	VARIANT var;
	char	str[24];
	int		pn;

	m_Rpline.RP_DATE1_DT.IP_DAY =0;
	memset( (char *)&m_Rpline.RP_DATE1_DT.IP_BCDDAY, (char)0, sizeof(m_Rpline.RP_DATE1_DT.IP_BCDDAY) );
	m_Rpline.LINE_CND[ RP_DATE1_PN ].INP_sg = FALSE;

	m_Rpline.RP_DATE2_DT.IP_DAY = 0;
	memset( (char *)&m_Rpline.RP_DATE2_DT.IP_BCDDAY, (char)0, sizeof(m_Rpline.RP_DATE2_DT.IP_BCDDAY) );
	m_Rpline.LINE_CND[ RP_DATE2_PN ].INP_sg = FALSE;

	if( m_ChkMnonly ) {
		if( m_MNTHbcd1[0] != (BYTE)0xff ) {
			m_Rpline.RP_DATE1_DT.IP_BCDDAY[0] = m_MNTHbcd1[0];
			m_Rpline.LINE_CND[ RP_DATE1_PN ].INP_sg = TRUE;
		}
		if( m_MNTHbcd2[0] != (BYTE)0xff ) {
			m_Rpline.RP_DATE2_DT.IP_BCDDAY[0] = m_MNTHbcd2[0];
			m_Rpline.LINE_CND[ RP_DATE2_PN ].INP_sg = TRUE;
		}
	}
	else {
		if( m_MMDDbcd1[0] != (BYTE)0xff ) {
			m_Rpline.RP_DATE1_DT.IP_BCDDAY[0] = m_MMDDbcd1[0];
			m_Rpline.RP_DATE1_DT.IP_BCDDAY[1] = m_MMDDbcd1[1];
			m_Rpline.LINE_CND[ RP_DATE1_PN ].INP_sg = TRUE;
		}
		if( m_MMDDbcd2[0] != (BYTE)0xff ) {
			m_Rpline.RP_DATE2_DT.IP_BCDDAY[0] = m_MMDDbcd2[0];
			m_Rpline.RP_DATE2_DT.IP_BCDDAY[1] = m_MMDDbcd2[1];
			m_Rpline.LINE_CND[ RP_DATE2_PN ].INP_sg = TRUE;
		}
	}

	for( int i = 0; i < 2; i++ ) {
		pn = (i==0) ? RP_DATE1_PN : RP_DATE2_PN;
		ctl = getINP_CTL( pn );

		if( m_ChkMnonly ) {
			((CICSDBEDT*)GetDlgItem(ctl->IDC_X))->SetMinval("00");
			((CICSDBEDT*)GetDlgItem(ctl->IDC_X))->SetMaxval("99");

			bcd[0] = m_Rpline._RPDTA[pn].IP_BCDDAY[0];
			var.pbVal = bcd;
			DBdata_set( this, ctl->IDC_X, &var, ICSDBEDT_TYPE_BCD, 2 );
			
			BcdMonthToString( str, sizeof str, bcd );
			DBSetDispString( this, ctl->IDC_X, str );
		}
		else {
			((CICSDBEDT*)GetDlgItem(ctl->IDC_X))->SetMinval("0000");
			((CICSDBEDT*)GetDlgItem(ctl->IDC_X))->SetMaxval("9931");

			memcpy( bcd, m_Rpline._RPDTA[pn].IP_BCDDAY, 4 );
			var.pbVal = bcd;
			DBdata_set( this, ctl->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
			
			pDBzm->BcdDateToString( str, sizeof str, bcd );
			DBSetDispString( this, ctl->IDC_X, str );
		}
	}
}


// 科目欄での選択画面の表示
void CRpkeyDlg::KamokuSelDisp( int pn, int mode/*=-1*/ )
{
	CRect rect, dlgrc, dlgrect;
	CSize size, *scrnsize;

//	UINT ID = getINP_CTL( pn )->IDC_X;
	UINT ID = getINP_CTL( RP_DEBT1_PN )->IDC_X;

	scrnsize = GetScreenSize();

	GetWindowRect( dlgrect );
	GetClientRect( dlgrc );
	GetDlgItem( ID )->GetWindowRect( rect );
	ScreenToClient( rect );
	int ht, tmp = 0;
	ht = rect.Height();

	pZmSel->GetCrntRect( size );

	if( (dlgrect.left + size.cx) > scrnsize->cx ) {
		tmp = (dlgrect.left + size.cx) - scrnsize->cx;
	}

	rect.left = dlgrc.left - tmp;
	rect.top -= (ht/2);

	int zs_mode;
	if( mode == -1 )	zs_mode = ZSEL_KAMOKU;
	else				zs_mode = mode;

	if( mode != ZSEL_BRNTKY ) {
		pZmSel->SetBrnTkyKana("");
	}

	pZmSel->DialogON( zs_mode, ZSEL_TOPLEFT, &rect );

}


// 財務科目選択用ターミネーション
long CRpkeyDlg::KamokuSelTermination( int pn, long nChar, long kst )
{
	long ret;
	ret = nChar;
	BOOL bShift = FALSE;

	if( kst & ICSDBEDT_KST_SHIFT ) {
		bShift	=	TRUE;
		// Shit + Tab
		if( nChar == VK_TAB )
			nChar = VK_F2;
	}

	struct _ItemData *keybuf = NULL;
	switch( pn ) {
	case RP_DEBT1_PN:	keybuf = &m_Rpline.RP_DEBT1_DT;		break;
	case RP_DEBT2_PN:	keybuf = &m_Rpline.RP_DEBT2_DT;		break;
	case RP_CRED1_PN:	keybuf = &m_Rpline.RP_CRED1_DT;		break;
	case RP_CRED2_PN:	keybuf = &m_Rpline.RP_CRED2_DT;		break;
	}

	if( nChar == VK_RETURN ) {
		CDBDATA tmp;
		tmp.seq = 1;
		tmp.ddate = -1;
		tmp.mofs = -1;
		tmp.cno = -1;
		tmp.dbmn = -1;
		tmp.dbt = keybuf->IP_CdNum;
		tmp.dbr = keybuf->IP_BRN;
		tmp.cbmn = -1;
		tmp.cre.Empty();
		tmp.cbr = -1;

		// 選択モジュールセット
		pZmSel->SetBrnTkyDCsgn( 0 );
		pZmSel->SetRecord( &tmp );

		if( keybuf->IP_BRN == -1 ) {
			if( pZmSel->GetCrntMode() != ZSEL_BRNTKY ) {
				::ZeroMemory( m_saveKana, sizeof m_saveKana );
				pZmSel->SetBrnTkyKana("");
				if( pZmSel->DialogON( ZSEL_BRNTKY, ZSEL_BTMLEFT, NULL ) == 0 ) {
					ret = 0;
				}
			}
		}
	}
	else if( (nChar == VK_TAB && ! bShift) ) {
		CDBDATA tmp;
		tmp.seq = 1;
		tmp.ddate = -1;
		tmp.mofs = -1;
		tmp.cno = -1;
		tmp.dbmn = -1;
		tmp.dbt = keybuf->IP_CdNum;
		tmp.dbr = keybuf->IP_BRN;
		tmp.cbmn = -1;
		tmp.cre.Empty();
		tmp.cbr = -1;

		// 選択モジュールセット
		pZmSel->SetBrnTkyDCsgn( 0 );
		pZmSel->SetRecord( &tmp );

		::ZeroMemory( m_saveKana, sizeof m_saveKana );
		pZmSel->SetBrnTkyKana("");

		if( pZmSel->GetCrntMode() != ZSEL_BRNTKY ) {
			if( pZmSel->DialogON( ZSEL_BRNTKY, ZSEL_BTMLEFT, NULL ) == 0 ) {
				ret = 0;
			}
		}
	}
	else if( nChar == VK_PRIOR ) {
		pZmSel->PageUp();
		ret = 0;
	}
	else if( nChar == VK_NEXT ) {
		pZmSel->PageDown();
		ret = 0;
	}
	else if( nChar == VK_HOME ) {
		::ZeroMemory( m_saveKana, sizeof m_saveKana );
		pZmSel->SetBrnTkyKana("");
		KamokuSelDisp( pn );
		ret = 0;
	}

	return ret;
}

// 科目選択よりデータセット
long CRpkeyDlg::KamokuSelDataJob( int pn, ZmselDBData* zd )
{
	UINT ID;
	ID = getINP_CTL( pn )->IDC_X;

	struct _KamokuData check = {0}, ans = {0};

	memcpy( check.kd_code, zd->kcode, KMKCD_LN );
	check.kd_eda = zd->edaban;

	DBKNREC* pKn = pDBzm->DB_PjisToKnrec( check.kd_code );
	if( pKn ) {
		memcpy( check.kd_name, pKn->knnam, sizeof pKn->knnam );
	}
	DBkamokudata_set( this, ID, &check, pDBzm );

	return 0;
}



///////////////////////////////////////////////////////////////////////////////
//証憑番号	[11.26 /12]
void CRpkeyDlg::OnSetFocusRpDocEvi1() 
{
	try
	{
		set_nowpn( RP_DOCEVI1_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_DOCEVI1_PN ); 
}

void CRpkeyDlg::OnKillFocusRpDocEvi1( long inplen ) 
{
	DocEviInp( RP_DOCEVI1_PN, &m_Rpline.RP_DOCEVI1_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpDocEvi1(long nChar, long inplen, long kst) 
{
	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = DocEviInp( RP_DOCEVI1_PN, &m_Rpline.RP_DOCEVI1_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_DOCEVI1_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

void CRpkeyDlg::OnSetFocusRpDocEvi2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	try
	{
		set_nowpn( RP_DOCEVI2_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	OnICSInputFocus( RP_DOCEVI2_PN );	
}

void CRpkeyDlg::OnKillFocusRpDocEvi2( long inplen ) 
{
	DocEviInp( RP_DOCEVI2_PN, &m_Rpline.RP_DOCEVI2_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpDocEvi2(long nChar, long inplen, long kst) 
{

	try
	{
		// 入力＆オフフォーカス
		if( !(nChar = DocEviInp( RP_DOCEVI2_PN, &m_Rpline.RP_DOCEVI2_DT, nChar, inplen, kst)) )
			return;

		//カーソル移動
		if( term_focus( RP_DOCEVI2_PN, nChar ) != 0 )
			Buzzer();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}

// 証憑番号 入力＆オフフォーカス
long CRpkeyDlg::DocEviInp(int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	char	buf[30] = {0};
	VARIANT var;

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

		if( var.pbVal[0] != 0 )	strcpy_s( (char*)check.IP_SNUM, sizeof check.IP_SNUM, (char*)var.pbVal );
		else					::ZeroMemory( check.IP_SNUM, sizeof check.IP_SNUM );

		if( check.IP_SNUM[0] != 0 )
		{
			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
			strcpy_s( (char*)keybuf->IP_SNUM, sizeof keybuf->IP_SNUM, (char*)check.IP_SNUM );
		}
		else
			m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;

		//データセット
	//	data_set( getINP_CTL( pn )->IDC_X, keybuf, CICSInput );
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
//		data_get( getINP_CTL( pn )->IDC_X, &check, CICSInput );
//		if( check.IP_DENP != 0xffff )
//		{
//			m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
//
//			keybuf->IP_DENP = check.IP_DENP;
//		}
	}
	else if( nChar == VK_DELETE )
	{
		check.IP_TGDAY = -1;
		var.pbVal = NULL;
		DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	
		::ZeroMemory( keybuf->IP_SNUM, sizeof keybuf->IP_SNUM );
		m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
	}

	return( nChar );
}


//証憑番号取消
long CRpkeyDlg::OnTerminationNoDocEvi(long nChar)
{
	if( ! m_PGSW ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	//カーソル移動
	if( term_focus( RP_NODOCEVI_PN, nChar ) != 0 ) {
	//	Buzzer();
		return nChar;
	}
	return 0;
}

//証憑番号なし
void CRpkeyDlg::OnBnClickedCheckSno()
{
	UpdateData();
	m_Rpline.LINE_CND[ RP_DOCEVI2_PN ].INP_type = m_NoDocEvi ? 1 : 0;

	CWnd* pwnd;
	pwnd = GetDlgItem( getINP_CTL(RP_DOCEVI2_PN)->IDC_X );
	((CICSDBEDT*)pwnd)->SetEnabled( m_NoDocEvi ? FALSE : TRUE );

	getINP_CTL(RP_DOCEVI2_PN)->INP_ena = m_NoDocEvi ? FALSE : TRUE;
}


// 消費税仕訳かどうか？ 仮受・仮払消費税仕訳を含む
BOOL CRpkeyDlg::IsSyohizeiData( CDBDATA* dbdata )
{
	BOOL bRet = FALSE;

	if( !pDBsy->IsSyohizei( dbdata ) ) {
		DBKNREC* pKn;

		pKn = pDBzm->DB_PjisToKnrec( dbdata->dbt );
		if( pKn ) {
			if( pDBzm->km_syattr( pKn ) > 0 ) {
				bRet = TRUE;
			}
		}
		pKn = pDBzm->DB_PjisToKnrec( dbdata->cre );
		if( pKn ) {
			if( pDBzm->km_syattr( pKn ) > 0 ) {
				bRet = TRUE;
			}
		}
	}
	else {
		bRet = TRUE;
	}

	return bRet;
}


// 部門・工事での選択画面の表示
void CRpkeyDlg::BmonKojiSelDisp( int pn, LPCTSTR kana/*=NULL*/ )
{
	CRect rect, dlgrc, dlgrect;
	CSize size, *scrnsize;

	UINT ID = getINP_CTL( pn )->IDC_X;

	int bmkj_type = -1;

	switch( ID ) {
	case IDC_SCNBMON1:
	case IDC_SCNCBMN1:
	case IDC_SCNBMON2:
	case IDC_SCNCBMN2:
		bmkj_type = 0;
		break;
	case IDC_SCNKOJI1:
	case IDC_SCNCKJI1:
	case IDC_SCNKOJI2:
	case IDC_SCNCKJI2:
		bmkj_type = 1;
		break;
	default:
		break;
	}

	if( bmkj_type < 0 )
		return;

	scrnsize = GetScreenSize();

	GetWindowRect( dlgrect );
	GetClientRect( dlgrc );
	GetDlgItem( IDC_SCNCBMN1 )->GetWindowRect( rect );
	ScreenToClient( rect );
	int ht, tmp = 0;
	ht = rect.Height();

	pZmSel->GetCrntRect( size );

	if( (dlgrect.left + size.cx) > scrnsize->cx ) {
		tmp = (dlgrect.left + size.cx) - scrnsize->cx;
	}

	rect.left = dlgrc.left - tmp;
	rect.top += (ht + (ht/2));

	int zs_mode;
	if( kana == NULL ) {
		if( bmkj_type == 0 ) {
			pZmSel->SetBumonKana( "" );
			zs_mode = ZSEL_BUMON;
		}
		else {
			pZmSel->SetKojiKana( "" );
			zs_mode = ZSEL_KOJI;
		}
	}
	else {
		if( bmkj_type == 0 ) {
			pZmSel->SetBumonKana( kana );
			zs_mode = ZSEL_BUMON_KANA;
		}
		else {
			pZmSel->SetKojiKana( kana );
			zs_mode = ZSEL_KOJI_KANA;
		}
	}

	pZmSel->DialogON( zs_mode, ZSEL_BTMLEFT, &rect );

}


// 科目選択よりデータセット
long CRpkeyDlg::BumonKojiSelDataJob( int pn, ZmselDBData* zd )
{
	UINT ID;
	ID = getINP_CTL( pn )->IDC_X;

	VARIANT var;
	char str[64] = {0};

	switch( pn ) {
	case RP_BMON1_PN:
	case RP_BMON2_PN:
	case RP_CBMN1_PN:
	case RP_CBMN2_PN:
		if( zd->bmn != -1 ) {
			pDBzm->BumonCodeToStr( str, sizeof str, zd->bmn );
			var.pbVal = (BYTE*)str;
		}
		else	var.pbVal = NULL;
		DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
		break;
	case RP_KOJI1_PN:
	case RP_KOJI2_PN:
	case RP_CKJI1_PN:
	case RP_CKJI2_PN:
		if( zd->koji[0] != 0 ) {
			pDBzm->KojiCodeToStr( str, sizeof str, CString(zd->koji) );
			var.pbVal = (BYTE*)str;
		}
		else	var.pbVal = NULL;
		DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
		break;
	}

	return 0;
}


// 部門番号IMEのEND処理
int CRpkeyDlg::CheckImeEndBmon( UINT ID, long nChar, LPCTSTR string ) 
{
	VARIANT var;

	CString buf = string;
	int		ret = 0;

	int pn = 0;
	switch( ID ) {
	case IDC_SCNBMON1:	pn = RP_BMON1_PN;
	case IDC_SCNBMON2:	pn = RP_BMON2_PN;
	case IDC_SCNCBMN1:	pn = RP_CBMN1_PN;
	case IDC_SCNCBMN2:	pn = RP_CBMN2_PN;
	}
	if( pn == 0 )	return ret;

	if( nChar == VK_TAB || nChar == VK_F12 ) {
		// カナ検索
		if( ! isdigitstr( (char*)string ) ) {
			BmonKojiSelDisp( pn, string );
			ret = 1;
		}
		else {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();

			DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else {
		if( isdigitstr((char*)(LPCTSTR)buf) ) {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();

			DBdata_set(this, ID, &var, ICSDBEDT_TYPE_STRING, 0);
		}
		else {
			ret = 1;
		}
	}

	return ret;
}


// 工事番号IMEのEND処理
int CRpkeyDlg::CheckImeEndKoji( UINT ID, long nChar, LPCTSTR string ) 
{
	VARIANT var;
	int		ret = 0;

	int pn = 0;
	switch( ID ) {
	case IDC_SCNKOJI1:	pn = RP_KOJI1_PN;
	case IDC_SCNKOJI2:	pn = RP_KOJI2_PN;
	case IDC_SCNCKJI1:	pn = RP_CKJI1_PN;
	case IDC_SCNCKJI2:	pn = RP_CKJI2_PN;
	}
	if( pn == 0 )	return ret;

	if( nChar == VK_TAB || nChar == VK_F12 ) {
		// カナ検索
		if( ! isdigitstr( (char*)string ) ) {
			BmonKojiSelDisp( pn, string );
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


HWND CRpkeyDlg::GetKamokuCtrlHWND()
{
	int pn = get_nowpn();
	HWND hwnd;

	struct _InpCtl* pctl = RPINP_CTL;
	switch( pn ) {
	case RP_DEBT1_PN:
	case RP_DEBT2_PN:
	case RP_CRED1_PN:
	case RP_CRED2_PN:
	case RP_INVNO_PN:
	case RP_INVNO2_PN:
		hwnd = GetDlgItem((pctl + pn)->IDC_X)->m_hWnd;
		break;
	default:
		hwnd = NULL;
		break;
	}
	return hwnd;
}

//科目欄 複数文字でカナ検索
int CRpkeyDlg::FukuKanaSearch(MSG* pMsg, int edaonly, int possgn)
{
	HIMC	hImc;
	char	buf[128] = { 0 };
	char	yomi[128] = { 0 };
	char	yomi_org[128] = { 0 };

	HWND	hWnd;
	if( !possgn ) {
		if( KanaSearchType() == 0 )
			return 0;
		//コントロールのHWND を取得する
		hWnd = GetKamokuCtrlHWND();
	}
	else {
		hWnd = GetKamokuCtrlHWND();
	}

	if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_TAB ) {
		return 0;
	}

	if( pMsg->wParam == VK_NEXT || pMsg->wParam == VK_PRIOR ) {
		return 0;
	}

	hImc = ImmGetContext(hWnd);

	if( pMsg->wParam == VK_END ) {
		ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		ImmReleaseContext(hWnd, hImc);
		return 0;
	}

	ImmGetCompositionString(hImc, GCS_COMPSTR, buf, sizeof buf);
	ImmGetCompositionString(hImc, GCS_COMPREADSTR, yomi_org, sizeof yomi_org);

//	MyTrace("●CRpkeyDlg::FukuKanaSearch buf, = %s, yomi_org = %s●\n", buf, yomi_org);

	//読みにアルファベットがあるか？
	CString stmp(yomi);
	kana_check(yomi, yomi_org);

	int orglen = strlen(yomi_org);
	int searchsgn = 1;
	if( orglen > 0 ) {
		int len = strlen(yomi);
		if( len < orglen ) {
			searchsgn = 0;
		}
	}

	int rval = 0;

	int ID, pn;
	pn = get_nowpn();

	struct _InpCtl* pctl = RPINP_CTL;
	ID = (pctl + pn)->IDC_X;

	if( buf[0] && yomi[0] && searchsgn ) {
		if( possgn == 0 ) {
			KanaTerminationKamoku(ID, yomi);
		}
		else {
			KanaTerminationInvno(ID, yomi);
		}
	}
	else if( (buf[0] == '\0') && (yomi[0] == '\0') ) {
		if( pMsg->wParam == VK_BACK || pMsg->wParam == VK_DELETE ) {
			if( possgn == 0 ) {
				if( pZmSel->GetCrntMode() == ZSEL_BRNTKY || pZmSel->GetCrntMode() == ZSEL_KMKANA ) {

					int setmode = pZmSel->GetCrntMode();
					if( m_saveKana[0] != 0 ) {
						KanaTerminationKamoku(ID, yomi);
					}
					else {
						KamokuSelDisp(pn);
					}
				}
			}
			else {
				if( m_saveKana[0] != 0 ) {
					KanaTerminationInvno(ID, yomi);
				}
				else {
				}
			}
		}
		if( pMsg->wParam == VK_BACK ) {
			rval = 1;
		}
	}
	ImmReleaseContext(hWnd, hImc);

	return rval;
}

//科目欄 バックスペース対応
int CRpkeyDlg::KanaCheckVK_BACK(MSG* pMsg, int possgn)
{
	HIMC	hImc;
	char	buf[128] = { 0 };
	char	yomi[128] = { 0 };
	char	yomi_org[128] = { 0 };

	//コントロールのHWND を取得する
	HWND	hWnd;

	if( possgn == 0 ) {
		if( KanaSearchType() == 0 )
			return 0;
		hWnd = GetKamokuCtrlHWND();
	}
	else {
		//登録番号
		hWnd = GetKamokuCtrlHWND();
	}

	return fncKanaCheckVK_BACK(pMsg, hWnd);

#ifdef CLOSE
	hImc = ImmGetContext(hWnd);
	if( pMsg->wParam == VK_F2 ) {
		ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		ImmReleaseContext(hWnd, hImc);
		return 0;
	}

	ImmGetCompositionString(hImc, GCS_COMPSTR, buf, sizeof buf);
	ImmGetCompositionString(hImc, GCS_COMPREADSTR, yomi_org, sizeof yomi_org);

	int rval = 0;

	if( (buf[0] == '\0') && (yomi[0] == '\0') ) {
		if( pMsg->wParam == VK_BACK ) {
			rval = 1;
		}
	}

	ImmReleaseContext(hWnd, hImc);

	return rval;
#endif
}

void CRpkeyDlg::OnSetFocusRpInvno() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	try {
		set_nowpn( RP_INVNO_PN );
		InvNoSelDisp(RP_INVNO_PN);
		DispInvno( RP_INVNO_PN, m_Rpline.RP_INVNO_DT._invno );
	}
	catch( CErrBlk E ) {
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}

	OnICSInputFocus( RP_INVNO_PN );	
}

void CRpkeyDlg::OnKillFocusRpInvno( long inplen ) 
{
	InvnoInp( RP_INVNO_PN, &m_Rpline.RP_INVNO_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpInvno( long nChar, long inplen, long kst )
{
	if( !m_PGSW ) {
		return;
	}

	if(( nChar == VK_TAB ) && ( is_SHIFT())) {
		nChar = VK_F2;
	}

	try {
		nChar = InvnoInp( RP_INVNO_PN, &m_Rpline.RP_INVNO_DT, nChar, inplen, kst );
		//カーソル移動
		if( term_focus( RP_INVNO_PN, nChar ) != 0 ) {
			Buzzer();
		}
	}
	catch( CErrBlk E ) {
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

void CRpkeyDlg::OnSetFocusRpInvno2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	try {
		set_nowpn( RP_INVNO2_PN );
		InvNoSelDisp(RP_INVNO2_PN);
		DispInvno( RP_INVNO2_PN, m_Rpline.RP_INVNO2_DT._invno );
	}
	catch( CErrBlk E ) {
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}

	OnICSInputFocus( RP_INVNO2_PN );	
}

void CRpkeyDlg::OnKillFocusRpInvno2( long inplen ) 
{
	InvnoInp( RP_INVNO2_PN, &m_Rpline.RP_INVNO2_DT, 0, inplen, 0 );
}

void CRpkeyDlg::OnTerminationRpInvno2( long nChar, long inplen, long kst )
{
	if( !m_PGSW ) {
		return;
	}

	if(( nChar == VK_TAB ) && ( is_SHIFT())) {
		nChar = VK_F2;
	}

	try {
		nChar = InvnoInp( RP_INVNO2_PN, &m_Rpline.RP_INVNO2_DT, nChar, inplen, kst );
		//カーソル移動
		if( term_focus( RP_INVNO2_PN, nChar ) != 0 ) {
			Buzzer();
		}
	}
	catch( CErrBlk E ) {
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}

}

void CRpkeyDlg::DispInvno( int pn, CString invno )
{
	VARIANT var;

	if( !invno.IsEmpty() ) {
	char tmpInvno[16] = { 0 };
	invno.Replace( _T( "T" ), _T( "" ));
	sprintf_s( tmpInvno, sizeof( tmpInvno ), _T( "%s" ), invno );
	var.pbVal = ( BYTE* )tmpInvno;
	}
	else {
		var.pbVal = NULL;
	}
//	DBSetDispString( this, getINP_CTL( pn )->IDC_X2, ( char* )var.pbVal );
	DBdata_set(this, getINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
}


long CRpkeyDlg::CheckDataDBeditInv(UINT ID, long nChar, LPCTSTR string)
{
	if( m_PGSW )
	{
		VARIANT var;
		int num = atoi(string);
		int len = strlen(string);
		//選択欄より
		if( num <= 32 && len <= 2 ) {
			var.pbVal = (BYTE*)string;
			DBdata_set(this, ID, &var, ICSDBEDT_TYPE_STRING, 0);
		}
		else {
			//登録番号チェック は Terminationで
			CICSDBEDT* pDedt;
			pDedt = (CICSDBEDT*)(GetDlgItem(ID));
			int pos = pDedt->GetCaretPosition();
			var.pbVal = (BYTE*)string;

			if( len == 13 || pos == 13 ) {
			DBdata_set(this, ID, &var, ICSDBEDT_TYPE_STRING, 0);
		}
			else {
				pDedt->InsertData(&var, ICSDBEDT_TYPE_STRING, 0, pos);
			}
		}
	}
	else {
		return 0;
	}

	return nChar;
}


long CRpkeyDlg::InvnoInp( int pn, struct _ItemData *keybuf, long nChar, long inplen, long kst ) 
{
	ASSERT( keybuf );
	if( keybuf == NULL ) {
		return -1;
	}

	VARIANT var;
	UpdateData();
	char buf[64];

	// オフフォーカス
	if( !nChar ) {
		DBdata_get( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		if( strlen(( char* )var.pbVal ) > 0 ) {
			CString checkInvno;
			checkInvno.Format( _T( "T%s" ), ( char* )var.pbVal );
			if( syzInvoice.CheckInvoiceNumber( checkInvno ) == -1 ) {
				//memset( keybuf->_invno, '\0', sizeof( keybuf->_invno ));
				//var.pbVal = NULL;
				//DBdata_set(this, getINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
				m_bIgnoreFocus = TRUE;
				sprintf_s(buf, sizeof buf, "%s", (char*)var.pbVal);

				DBSetDispString(this, getINP_CTL(pn)->IDC_X2/*scGetINP_CTL(pn)->IDC_X*/, buf);
				ICSMessageBox( syzInvoice.GetErrorMessage(), MB_ICONSTOP );
				m_bIgnoreFocus = FALSE;

				checkInvno.Replace("T", "");
				sprintf_s(keybuf->_invno, sizeof(keybuf->_invno), _T("%s"), checkInvno);
				var.pbVal = (BYTE*)(LPCTSTR)checkInvno;
				DBdata_set(this, getINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);

				set_focus( pn );
				set_nowpn( pn );
				m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
			}
			else {
				sprintf_s( keybuf->_invno, sizeof( keybuf->_invno ), _T( "%s" ), checkInvno );
				DBSetDispString( this, getINP_CTL( pn )->IDC_X2, keybuf->_invno );
				m_Rpline.LINE_CND[ pn ].INP_sg = TRUE;
			}
		}
		else {
			memset( keybuf->_invno, '\0', sizeof( keybuf->_invno ));
			var.pbVal = NULL;
			DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			DBSetDispString( this, getINP_CTL( pn )->IDC_X2, keybuf->_invno );
			m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
		}
		return 0;
	}
	else {


		if( inplen ) {
			UINT ID = getINP_CTL(pn)->IDC_X;
			UINT dispID = getINP_CTL(pn)->IDC_X2;
			//データリード
			DBdata_get(this, ID, &var, ICSDBEDT_TYPE_STRING, 0);
			int n = strlen((char*)var.pbVal);

			//2ケタ未満は選択欄から
			if( n && n < 3 ) {
				int number = atoi((char*)var.pbVal);
				char buf[64];
				sprintf_s(buf, sizeof buf, "%d", number);

				ZmselDBData	zd;
				CString stmp;
				pZmSel->InputData(1, number, -1, ZSEL_INVNO);
				pZmSel->ResultData(&zd);
				char disp[64] = { 0 };

				if( zd.errflg != 0 ) {
					// 選択ミス//データセット(再表示)
					stmp = keybuf->_invno;
					stmp.Replace("T", "");
					var.pbVal = (BYTE*)(LPCTSTR)stmp;
					sprintf_s(disp, sizeof disp, "T%s", stmp);
					Buzzer();
				}
				else {
					stmp = zd.sel_name;
					stmp.Replace("T", "");
					var.pbVal = (BYTE*)(LPCTSTR)stmp;
					sprintf_s(disp, sizeof disp, "T%s", stmp);
				}
				DBdata_set(this, ID, &var, ICSDBEDT_TYPE_STRING, 0);
				DBSetDispString(this, dispID, disp);
			}
		}

		if(( nChar == VK_TAB ) && ( kst & ICSDBEDT_KST_SHIFT )) {
			return VK_F2;
		}
		else if( nChar == VK_DELETE ) {
			memset( keybuf->INVNO, '\0', sizeof( keybuf->INVNO ));
			var.pbVal = NULL;
			DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			DBSetDispString( this, getINP_CTL( pn )->IDC_X2, keybuf->_invno );
			m_Rpline.LINE_CND[ pn ].INP_sg = FALSE;
		}
		else {
			// ページアップ／ダウン
			if( nChar == VK_PRIOR ) {
				pZmSel->PageUp();
			}
			else if( nChar == VK_NEXT ) {
				pZmSel->PageDown();
			}
			else if( nChar == VK_HOME ) {
				::ZeroMemory(m_saveKana, sizeof m_saveKana);
				pZmSel->SetInvnoKana("");
				pZmSel->PageChange();
			}
		}
	}

	return nChar;
}


void CRpkeyDlg::OnBnClickedChkMenzei()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	UpdateData();
	m_Rpline.LINE_CND[RP_MENZEI_PN].INP_sg = m_MenzeiChk ? TRUE : FALSE;
	m_Rpline.LINE_CND[RP_MENZEI_PN].INP_type = m_MenzeiChk ? 1 : 0;
	if( m_MenzeiChk ) {
		if( m_KazeiChk ) {
			m_KazeiChk = FALSE;
			m_Rpline.LINE_CND[RP_KAZEI_PN].INP_type = 0;
			UpdateData( FALSE );
		}
	}
	EnableMenWariCombo(m_MenzeiChk);
}


void CRpkeyDlg::OnBnClickedChkKazei()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	UpdateData();
	m_Rpline.LINE_CND[RP_KAZEI_PN].INP_sg = m_KazeiChk ? TRUE : FALSE;
	m_Rpline.LINE_CND[RP_KAZEI_PN].INP_type = m_KazeiChk ? 1 : 0;
	if( m_KazeiChk ) {
		if( m_MenzeiChk ) {
			m_MenzeiChk = FALSE;
			m_Rpline.LINE_CND[RP_MENZEI_PN].INP_type = 0;
			UpdateData( FALSE );
		}
	}
	EnableMenWariCombo(m_MenzeiChk);
}

long CRpkeyDlg::OnTerminationMenzei( long nChar) 
{
	if( !m_PGSW ) {
		return nChar;
	}

	if(( nChar == VK_TAB ) && ( is_SHIFT() )) {
		nChar = VK_F2;
	}

	//カーソル移動
	if( term_focus( RP_MENZEI_PN, nChar ) != 0 ) {
		if(( nChar != VK_DOWN ) && ( nChar != VK_UP ) && ( nChar == VK_ESCAPE )) {
			Buzzer();
		}

		return nChar;
	}

	return 0;
}

long CRpkeyDlg::OnTerminationKazei( long nChar) 
{
	if( !m_PGSW ) {
		return nChar;
	}

	if(( nChar == VK_TAB ) && ( is_SHIFT() )) {
		nChar = VK_F2;
	}

	//カーソル移動
	if( term_focus( RP_KAZEI_PN, nChar ) != 0 ) {
		if(( nChar != VK_DOWN ) && ( nChar != VK_UP ) && ( nChar == VK_ESCAPE )) {
			Buzzer();
		}

		return nChar;
	}

	return 0;
}


//免税事業者控除割合
long CRpkeyDlg::OnTerminationMenWari(long nChar)
{
	if( !m_PGSW ) {
		return nChar;
	}

	if( (nChar == VK_TAB) && (is_SHIFT()) ) {
		nChar = VK_F2;
	}

	//カーソル移動
	if( term_focus(RP_MENWARI2_PN, nChar) != 0 ) {
		if( (nChar != VK_DOWN) && (nChar != VK_UP) && (nChar == VK_ESCAPE) ) {
			Buzzer();
		}

		return nChar;
	}

	return 0;
}



// インボイス登録番号での選択画面の表示
void CRpkeyDlg::InvNoSelDisp(int pn, LPCTSTR kana/*=NULL*/)
{
	CRect rect, dlgrc, dlgrect;
	CSize size, *scrnsize;

	UINT ID = getINP_CTL(pn)->IDC_X;

	scrnsize = GetScreenSize();

	GetWindowRect(dlgrect);
	GetClientRect(dlgrc);
	GetDlgItem(IDC_SCN_INVNO)->GetWindowRect(rect);
	ScreenToClient(rect);
	int ht, tmp = 0;
	ht = rect.Height();

	pZmSel->GetCrntRect(size);

	if( (dlgrect.left + size.cx) > scrnsize->cx ) {
		tmp = (dlgrect.left + size.cx) - scrnsize->cx;
	}

	rect.left = dlgrc.left - tmp;
	rect.top -= (ht / 2);
	int mode = ZSEL_INVNO;

	if( kana == NULL ) {
		pZmSel->SetInvnoKana("");
	}
	else {
		pZmSel->SetInvnoKana(kana);
		mode = ZSEL_INVKANA;
	}

	pZmSel->DialogON(mode, ZSEL_TOPLEFT, &rect);

}



void CRpkeyDlg::InvNoSelDataJob(int pn, ZmselDBData* zd)
{
	UINT ID, dispID;
	ID = getINP_CTL(pn)->IDC_X;
	dispID = getINP_CTL(pn)->IDC_X2;

	VARIANT var;
	char str[64] = { 0 };
	char disp[64] = { 0 };

	MyTrace("InvNoSelDataJob TOP\n");

	//登録番号
	CString stmp;
	if( zd->sel_name[0] ) {
		stmp = zd->sel_name;
		stmp.Replace("T", "");

		MyTrace("InvNoSelDataJob stmp = %s\n", stmp);

		var.pbVal = (BYTE*)(LPCTSTR)stmp;
		sprintf_s(disp, sizeof disp, "T%s", stmp);
	}
	else {
		var.pbVal = NULL;
	}
	DBdata_set(this, ID, &var, ICSDBEDT_TYPE_STRING, 0);
	DBSetDispString(this, dispID, disp);
}


void CRpkeyDlg::EnableMenWariCombo(BOOL bEnable)
{
	struct _InpCtl* ctl;

	//変更前は、非表示
	ctl = getINP_CTL(RP_MENWARI1_PN);
	ctl->INP_ena = FALSE;

	for( int pn = RP_MENWARI2_PN; pn <= RP_MENWARI2_PN; pn++ ) {

		ctl = getINP_CTL( pn );
		int nID = ctl->IDC_X;
		CWnd* pwnd = GetDlgItem(nID);

		ctl->INP_ena = bEnable;
		pwnd->EnableWindow(bEnable);
	}
//	GetDlgItem(IDC_STATIC25)->EnableWindow(bEnable);
}


void CRpkeyDlg::OnCbnSelchangeScnMenWari()
{
	int sel = m_MenWari.GetCurSel();

	m_Rpline.RP_MENWARI1_DT.SKBN_SGN1 = 0;
	m_Rpline.RP_MENWARI1_DT.SKBN_SGN2 = sel;

	m_MenWari.GetLBText(sel, m_Rpline.RP_MENWARI1_DT.SKBN_TXT);
	m_Rpline.LINE_CND[RP_MENWARI1_PN].INP_sg = TRUE;
}

void CRpkeyDlg::OnCbnSelchangeScnMenWari2()
{
	int sel = m_MenWari2.GetCurSel();

	m_Rpline.RP_MENWARI2_DT.SKBN_SGN1 = 0;
	m_Rpline.RP_MENWARI2_DT.SKBN_SGN2 = sel;

	m_MenWari2.GetLBText(sel, m_Rpline.RP_MENWARI2_DT.SKBN_TXT);
	m_Rpline.LINE_CND[RP_MENWARI2_PN].INP_sg = TRUE;
}


//直前の免税サインをセット
int CRpkeyDlg::SetPrevMenzei(CDBINPDataRec* pDt)
{
	DWORD code = 0;

	if( m_svMenzei == NON_SELECT )
		return 0;

	int sytyp = is_symstyp();
	// 消費税マスターでない
	if( sytyp == NORMAL_MST )
		return 0;

	if( !IsSyohizeiData(pDBzm->dbdata) )
		return 0;

	// 業種・簡易		
	switch( m_svMenzei ) {
	case INV_MEN80:	code = CD_SP_EXEMPT;	break;
	case INV_MEN50:	code = CD_SP_EXEMPT_50;	break;
	default:
		break;
	}
	if( code > 0 ) {
		if( pDBsy->SyFnc(code) == 0 ) {
			struct _SY_MSG_PACK2 smpk;
			pDBsy->SyMsg(&smpk);
			pDBzm->GetCDBData(pDt);
		}
	}
	return 0;
}


// 登録番号ターミネーション
void CRpkeyDlg::KanaTerminationInvno(UINT ID, LPCTSTR kana)
{

	int nowmode, mode;
	char tmp[64] = { 0 };
	char kanabf[64] = { 0 };
	nowmode = pZmSel->GetCrntMode();
	mode = -1;
	int firstkana = 0;

	kana_check(kanabf, kana);

	// 濁点等は除く
	if( (BYTE)kanabf[0] == (BYTE)0xDE || (BYTE)kanabf[0] == (BYTE)0xDF ) {
		return;
	}
	if( strlen(kanabf) > INVKANA_MAX ) {
		kanabf[INVKANA_MAX] = '\0';
	}

	if( nowmode == ZSEL_INVNO ) {
		mode = ZSEL_INVKANA;
		strcpy_s(tmp, sizeof tmp, kanabf);
		::ZeroMemory(m_saveKana, sizeof m_saveKana);
		strcpy_s(m_saveKana, sizeof m_saveKana, tmp);
		pZmSel->SetInvnoKana(tmp);
		firstkana = 1;
	}
	else if( nowmode == ZSEL_INVKANA ) {
		mode = ZSEL_INVKANA;
		strcpy_s(tmp, sizeof tmp, kanabf);
		pZmSel->SetInvnoKana(tmp);
	}

	if( mode != -1 ) {
		int pn;
		switch( ID ) {
		case IDC_SCN_INVNO:		pn = RP_INVNO_PN;	break;
		case IDC_SCN_INVNO2:	pn = RP_INVNO2_PN;	break;
		}

		int newmd = pZmSel->GetCrntMode();

		//カナ検索がなかった場合、前のカナで検索
		if( newmd == ZSEL_INVNO && !firstkana ) {
			strcpy_s(tmp, sizeof tmp, m_saveKana);
			pZmSel->SetInvnoKana(tmp);
			InvNoSelDisp(pn, tmp);
		}
		else {
			strcpy_s(m_saveKana, sizeof m_saveKana, tmp);
			InvNoSelDisp(pn, tmp);
		}
	}
}
