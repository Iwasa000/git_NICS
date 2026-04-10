// DBDinpView.cpp : CDBDinpView クラスの実装
//

#include "stdafx.h"
#include "DBDinp.h"

#include "MainFrm.h"
#include "DBDinpDoc.h"
#include "DBDinpView.h"

#include "MainFrm.h"

#include "DBDinpDoc.h"
#include "DBDinpView.h"
class CDBDinpView *p_DBDinpView = NULL;

#include "input1.h"
#include "input2.h"
#include "mstrw.h"

#include "scan.h"
#include "SubFnc.h"
#include "DinpSub.h"
#include "DispOptionDlg.h"

#define EXTERN_DECLARE_DATALINE
#include "DataLine.h"
#include "GenpScanDialog.h"


// 定型仕訳
CMKRDSub Mkrd;

#include "RdregDlg.h"

//#define DLINETrace	MyTrace
//#define DLINETrace	TRACE
#define DLINETrace		__noop

//#undef	FILETrace
//#define	FILETrace	MyTrace

//#define ONSIZETrace	MyTrace
//#define ONSIZETrace	TRACE
#define ONSIZETrace		__noop


//#define CRECTrace	{	MyTrace( "[%d] CREC zei = %02x%02x%02x%02x%02x%02x\n", __LINE__, CREC.m_zei[0],CREC.m_zei[1],CREC.m_zei[2],CREC.m_zei[3],CREC.m_zei[4],CREC.m_zei[5] ); }
#define IDC_DISPSLIP_CHK_RECT  {\
CRect	dspRect;\
GetDlgItem( IDC_DISPSLIP_CHK )->GetWindowRect( &dspRect );\
ScreenToClient( &dspRect );\
MyTrace( "DispSlip[%d] = (%d,%d,%d,%d)\n", __LINE__, dspRect.left, dspRect.top, dspRect.right,  dspRect.bottom );\
}

#define	LNTrace(x)		{	MyTrace( "%s[%d]", #x, __LINE__);	}

// 部門入力
#include "BkInpDlg.h"
static CBkInpDlg* m_pBkInp = NULL;
static CBkInpDlg* m_pBkInpX = NULL;

//スキャン画面
#include "scanview.h"
static CScanView* m_pScanView = NULL;

#include "RpkeyDlg.h"
CRpkeyDlg*	pRpkeyDlg = NULL;

// 入力選択項目Get/Set
extern CDinpINI DinpSW;

// 入力設定項目選択
#include "AutoInp.h"
struct _AUTOSEL *pAUTOSEL = 0;

// 合計問い合わせ
#include "ttldsp.h"

// 貸借合計問合わせ
#include "TaiTtl.h"
CTaiTtl m_TaiTtl;
#include "TaiDsp.h"
CTaiDsp* m_pTaiDsp = 0;

#ifdef _CPYIMG //#* 05.10 /01
#include "SelImgDlg.h"
#endif

#include "MnthDlg.h"
//#include "perform_chk.h"

#include "SwkJob.h"
static CSwkJob* m_pSjb = NULL;

#include "BmnChk.h"

CBmnHaniCheck	BmChk;
CKojiDataCheck	KjChk;

#include "SwkCpyJob.h"
#include "CNPautoDlg.h"

static CSWKCpySrc* m_pSCSrc = NULL;

static RECT BIGIMG_RECT;	// イメージ拡大元サイズ


#if (FOCUS_DEBUG == 1)
  #define set_focus(a)		set_focusTrace(a, 0, __LINE__, __FUNCTION__, __FILE__)
  #define _set_focus(a,b)	_set_focusTrace(a, b, 0, __LINE__, __FUNCTION__, __FILE__)
#else
  #undef set_focus
  #undef _set_focus
#endif

#if (INPLINE_DEBUG == 1)
#define inpline_dsp( data, line )	inpline_dspTrace(data, line, __LINE__, __FUNCTION__, __FILE__)
#else
#undef inpline_dsp
#endif


#ifdef DBG_SYINIZ
void DB_SyInizTrace(CDBINPDataRec* rec, int line/* = 0*/, char* func/*= NULL*/, char* fname/* = NULL*/)
{
	MyTrace("◆◆◆ DB_SyInizTrace L:%d, Func(= %s), File = %s\n", line, func, fname);
	DB_SyIniz(rec);
}

#define DB_SyIniz( data )	DB_SyInizTrace(data, __LINE__, __FUNCTION__, __FILE__)
#endif


#ifdef DBG_SY_CHK_TXT
LPCTSTR sy_chk_txtTrace(CDBINPDataRec* rec, struct _SY_MSG_PACK2* smp, int mode, int line/* = 0*/, char* func/*= NULL*/, char* fname/* = NULL*/)
{
	MyTrace("◆◆◆ sy_chk_txtTrace L:%d, Func(= %s), File = %s\n", line, func, fname);
	return sy_chk_txt(rec, smp, mode);
}

#define sy_chk_txt( data, smp, mode )	sy_chk_txtTrace(data, smp, mode, __LINE__, __FUNCTION__, __FILE__)
#endif





// 枝番摘要モジュール
CBrnTky m_BrTek;


int BRTORK_ITEM;
struct _BRTORK_DATA {
	_BRTORK_DATA() {
		jobend = 0;
		rewupdat_sg = 0;
		svINP_mode = 0;
		CREC.Reset();
		SCAN_rec.Reset();
	}
	int jobend;
	int rewupdat_sg;
	int svINP_mode;
	CDBINPDataRec CREC, SCAN_rec;
};
static struct _BRTORK_DATA BRTORK_DATA;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBDinpView

struct _DataTable SelFix8[2*4];
struct _DataTable SelVal32[8*5];

static int SKBN_N = 0;
static struct _DataTable SV_SelVal32[8*4];

//Value
int		BOTOM_SEQ = 0;

int PROGRAM_OK = FALSE;
//------------------------------
int INITIAL_SW = TRUE;

int MST_OPEN_OK = FALSE;	// マスター準備ＯＫサイン

BOOL bIKAT_SCAN = FALSE;	// 一括修正中サイン

// DataListCtrl の入力窓
struct _InpCtl DLIST_INPCTRL[] = 
{
	{ IDC_ICSDBEDTCTRL1,	0,	0,	0,	0,	0	},
	{ IDC_ICSDBEDTCTRL2,	0,	0,	0,	0,	0	}
};

// Now ポジション
//static int NOW_PN;
//static int SAVE_PN = -1;
//static int DDATA_NOW_PN[ 20 ];
// 修正データ１ラインリードＳＷ
static int DATA_1RD = 0;

// 枝番チェックポジション
int BRNCHK_PN = -1;

BOOL FNC_BUTTON = FALSE;	// ファンクション実行中

// 仕訳状態ビットテーブル
#include "bit.h"
CBit	MODEFY_BIT;		//修正仕訳ビットテーブル
CBit	SYZMDFY_BIT;	//消費税サイン修正ビットテーブル


#include "SEQ.h"
CSEQ	APPEND_SEQ;		//追加仕訳ＳＥＱ登録テーブル
CSEQ	SEQ_APPEND;		//追加仕訳ＳＥＱ登録テーブル管理テーブル
//CSEQ	SCAN_SEQ;		//スキャン仕訳ＳＥＱ登録テーブル
//CSEQ	SEQ_SCAN;		//スキャン仕訳ＳＥＱ登録テーブル管理テーブル

CSEQ	INSPOS_SEQ;		//挿入仕訳対応ＳＥＱ ポジション格納テーブル
CSEQ	SEQ_INSPOS;		//挿入仕訳対応ＳＥＱ 格納テーブル

CSEQ	SCMDFY_SEQ;		//カラムソート用 データＳＥＱテーブル
vector<DWORD>	SEQ_SCMDFY;		//カラムソート用 データＳＥＱ管理テーブル
//CSEQ	SEQ_SCMDFY;		//カラムソート用 データＳＥＱ管理テーブル
CSEQ	MDFYDATA_SEQ;	//カラムソート用 データＳＥＱ管理テーブル(dbdata)


DB_SCANINFO		SCAN_PGTBL;
SCAN_ITEMEX		SC_Item;
CStringArray	SC_Tky;		// 複数検索摘要


#ifdef _SLIP_ACCOUNTS_
extern	int _IS_SLIP_MASTER;	// 原票保存マスター？
#endif

#define BRNTKY_X_RATE 1.9	// 枝番摘要表示コントロールＸ軸比率
// 部門名称表示コントロールＸ軸比率 08.19 /02
//#define BUMON_X_RATE	(Voln1 && Voln1->APNO == 0x20) ? 3.5 : 2.8
#define BUMON_X_RATE	3.6

#define RENTKNO		get_rentk_sw()
#define SWKTKNO		get_swktky_sw()
#define KANATKNO	3

const int nTimerID = 100;


int DB_DataScan( CString& filter, WORD* dnpsgn = NULL, BOOL mkonly = FALSE );

extern
BOOL IsKaribarai( DWORD kncd );

void InitScanPgTBL( DB_SCANINFO* pSCpg )
{
	pSCpg->data_owner	= 0;
	pSCpg->data_mode	= 0;

	pSCpg->tbl_end		= 0;
	pSCpg->mk_datetbl	= 0;

	pSCpg->sort_id		= -1;
	pSCpg->sort_type	= 0;
	pSCpg->sort_cnt		= 0;
	pSCpg->mk_dbdata	= 0;

	pSCpg->ins_data		= 0;
	pSCpg->insSeq.RemoveAll();
	pSCpg->scanInsSeq.RemoveAll();
	pSCpg->ins_query	= 0;

	pSCpg->filter.Empty();

	pSCpg->sortOrder.Empty();
}

int GetDtListColumn(int nLine, CArray<DT_COLUMN, DT_COLUMN&>& clm, int inv = 0 );

///////////////////////////////////////////////////////////////////////////////////
// CDBDinpView

IMPLEMENT_DYNCREATE(CDBDinpView, CDBbaseView)

BEGIN_MESSAGE_MAP(CDBDinpView, CDBbaseView)
	ON_COMMAND(ID_BUTTON_END, OnButtonEnd)
	ON_COMMAND(ID_BUTTON_F12, OnButtonF12)
	ON_COMMAND(ID_BUTTON_F8, OnButtonF8)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F8, OnUpdateButtonF8)
	ON_COMMAND(ID_BUTTON_F11, OnButtonF11)
	ON_COMMAND(ID_BUTTON_F3, OnButtonF3)
	ON_COMMAND(ID_BUTTON_F4, OnButtonF4)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F4, OnUpdateButtonF4)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F12, OnUpdateButtonF12)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F3, OnUpdateButtonF3)
	ON_COMMAND(ID_BUTTON_F9, OnButtonF9)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F9, OnUpdateButtonF9)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F11, OnUpdateButtonF11)
	ON_COMMAND(ID_BUTTON_F7, OnButtonF7)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F7, OnUpdateButtonF7)

	ON_MESSAGE( WM_RETROFOCUS2, RetroFocus2 )
	ON_MESSAGE( WM_RETROFOCUS, RetroFocus )
	ON_MESSAGE( WM_SENDSEQ, OnSendSeq )

	ON_NOTIFY_RANGE( DLN_BEGINLABELEDIT, IDC_DTLIST, IDC_DTLINE, OnBeginLabelEdit )
	ON_NOTIFY_RANGE( DLN_ENDLABELEDIT,   IDC_DTLIST, IDC_DTLINE, OnEndLabelEdit )
	ON_NOTIFY_RANGE( DLN_SETDISPINFO,	 IDC_DTLIST, IDC_DTLINE, OnSetDispInfo )
	ON_NOTIFY( DLN_SELCHANGED, IDC_DTLIST, OnLineChanged )

	ON_MESSAGE( WM_SKBN_INEND, OnSkbnEnd )
	ON_MESSAGE( WM_SKBN_ICHIRN, DspSkbnIchiran)
	ON_MESSAGE( WM_SKBN_LINEDISP, SetSkbnLineDisp )

	ON_MESSAGE( WM_BKINP_END, OnBkinpEnd )
	ON_MESSAGE( WM_HSINP_INEND, OnHusenInpMsg )
	ON_MESSAGE( WM_SUBDATA_INEND, OnSubDataInEnd)

	ON_MESSAGE( WM_HJIYU_INEND, OnHjiyuInEnd )
	ON_MESSAGE( WM_HJIYU_SEL, OnHjiyuSel )

#ifdef REGDATA	// 07.28 /00
	ON_MESSAGE( WM_REGDATA, OnRGDTsel )
#endif
	ON_MESSAGE( WM_CORRECTMSG, OnCorrectMsg )
	ON_MESSAGE( WM_DBDINPMSG,  OnDBDinpMsg)
	ON_MESSAGE( WM_SELGUIDEMSG, OnSelGuideMsg)

	ON_WM_SIZE()
	ON_MESSAGE(ICS_SELDATE_MESSAGE_CHANGE, OnDateSelectMessageChangeMNTHSEL)
	ON_COMMAND(ID_BUTTON_F2, &CDBDinpView::OnButtonF2)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F2, &CDBDinpView::OnUpdateButtonF2)
	ON_COMMAND(ID_BUTTON_INSERT, &CDBDinpView::OnButtonInsert)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_INSERT, &CDBDinpView::OnUpdateButtonInsert)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_BUTTON_F6, &CDBDinpView::OnButtonF6)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F6, &CDBDinpView::OnUpdateButtonF6)
	ON_COMMAND(ID_BUTTON_F5, &CDBDinpView::OnButtonF5)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F5, &CDBDinpView::OnUpdateButtonF5)
	ON_COMMAND(ID_MENU_KYUCSV, &CDBDinpView::OnMenuKyucsv)
	ON_UPDATE_COMMAND_UI(ID_MENU_KYUCSV, &CDBDinpView::OnUpdateMenuKyucsv)

	ON_COMMAND(ID_MENU_GENCSV, &CDBDinpView::OnMenuGencsv)
	ON_UPDATE_COMMAND_UI(ID_MENU_GENCSV, &CDBDinpView::OnUpdateMenuGencsv)

//	ON_WM_RBUTTONDOWN()
//ON_COMMAND(IDM_SWK_COPY, &CDBDinpView::OnSwkCopy)
//ON_COMMAND(IDM_SWK_PASTE, &CDBDinpView::OnSwkPaste)
//ON_UPDATE_COMMAND_UI(IDM_SWK_COPY, &CDBDinpView::OnUpdateSwkCopy)
//ON_UPDATE_COMMAND_UI(IDM_SWK_PASTE, &CDBDinpView::OnUpdateSwkPaste)
#ifdef _SLIP_ACCOUNTS_
ON_BN_CLICKED(IDC_DISPSLIP_BTN, &CDBDinpView::OnBnClickedDispslipBtn)
ON_BN_CLICKED(IDC_LINKSLIP_BTN, &CDBDinpView::OnBnClickedLinkslipBtn)
ON_BN_CLICKED(IDC_DISPSLIP_CHK, &CDBDinpView::OnBnClickedDispslipChk)	//原票表示ボタン
ON_COMMAND(ID_MENU_SLIP, &CDBDinpView::OnMenuSlip)
ON_UPDATE_COMMAND_UI(ID_MENU_SLIP, &CDBDinpView::OnUpdateMenuSlip)
ON_BN_CLICKED(IDC_GNPSCAN_BTN, &CDBDinpView::OnBnClickedGnpScanBtn)
#endif
	ON_WM_TIMER()

END_MESSAGE_MAP()


// CDBDinpView コンストラクション/デストラクション

CDBDinpView::CDBDinpView()
	: CDBbaseView(CDBDinpView::IDD), fPen(FALSE), m_DtLine(120), m_DtList(120)
{
	// TODO: 構築コードをここに追加します。

	//注意！この関数を変更してはいけません
	FirstTime()	=	TRUE;

	// TODO: この場所に構築用のコードを追加してください。
	flgPen = FALSE;

	m_AutoInp_use = FALSE;

	m_ASTKCX = NULL;

	m_TEK_MODE = 0;

	m_ToSkbnDlg = FALSE;
	m_bHusenFocus = TRUE;

	NOW_PN	= -1;
	EDIT_PN	= -1;
	SAVE_PN	= -1;
	for( int i = 0; i < 20; i++ )
		DDATA_NOW_PN[ i ] = -1;

	m_bMnthFix = FALSE;
	m_bIgnoreDateTerm = FALSE;

#ifdef REGDATA
	m_bRGDTA_BUTTON_DISP = FALSE;
	m_CmnId = -1;

	RdataTblIniz();
#endif

#ifdef _SLIP_ACCOUNTS_
	m_bSLACC_BUTTON_DISP = FALSE;
	m_pImgmng = NULL;
	m_Imgcnt = -1;
	m_SlipBtnState = ID_LINKOFF_BSTATE;
	m_ImgDtseq = -1;
	m_NowImgSeq = -1;
	m_DImgdlgType = ID_DBDINP_NODISPDLG_TYPE;
	m_bSLIP_MASTER	= FALSE;
#endif

	m_OneHusen = 0;
	m_bSyzYunyu = m_bSyzSotoMen = FALSE;
	m_pnZeiSotoMen = 0;
	m_NOBRNCHK_CND	= 0;
	m_BMKJICHK_CND	= 0;
	m_TabKeyStop	= 0;

	m_BKINP_PN = -1;

	m_bSwkCopy = FALSE;
	m_bSwkPaste = FALSE;

	::ZeroMemory( m_blkIdx, sizeof m_blkIdx );
	m_blkIdx[0] = 1;
	m_blkIdx[1] = 2;
	m_blkIdx[2] = 3;
	m_blkIdx[3] = 4;
	m_blkIdx[4] = 5;

#ifdef _SLIP_ACCOUNTS_
	m_SlipPakageCd = -1;
#endif

	m_sdbtBmn = -1;
	m_screBmn = -1;
	m_bAppendUpkey = FALSE;

	m_bScanMode = FALSE;
	m_reCoDispType = 0;
	m_bDscroll = FALSE;

	prevImgSeq = -1;
	m_DenpSeq = m_DenpICNO = 0;

	m_bDWP_LINECOUNT = FALSE;
	ddata_dspFlg = FALSE;

	m_bForkUCOM = FALSE;

	m_InpMode = INPMODE_NORMMAL;

	m_CalcFlg = FALSE;
	m_CalcOFF = 0;
	m_bActiveIgnore = FALSE;
//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
TRACE( "sizeof struct _AUTOSEL %d\n", sizeof (struct _AUTOSEL) ); 
}

CDBDinpView::~CDBDinpView()
{
	SubDataInpDelete();
	if( m_pSjb != NULL ) {
		delete m_pSjb;
	}
	if( m_pSCSrc != NULL ) {
		delete m_pSCSrc;
	}
}


void CDBDinpView::CloseWork()
{
#ifdef _SLIP_ACCOUNTS_
	if( m_pImgmng ){
		delete m_pImgmng;
		m_pImgmng = NULL;
	}
#endif
	CDBbaseView::CloseWork();
}



#ifdef _SLIP_ACCOUNTS_
//-----------------------------------------------------------------------------
// イメージ表示モジュール CALLBACK 関数
//-----------------------------------------------------------------------------
void ImgDlgFunction( UINT msg, void *p, CWnd *pWnd  )
{
	IMG_CBMSGINFO	*pCbmsgInfo = (IMG_CBMSGINFO*)p;

//MyTrace( "@ImgDlgFunction callback msg = %d, docseq = %d\n", msg, pCbmsgInfo->docseq );
//MyTrace( "@ImgDlgFunction callback srcOrder = %d, leftLinkCnt = %d\n", pCbmsgInfo->srcOrder, pCbmsgInfo->leftLinkCnt );

	if( msg == ID_IMGDLG_KEYDOWN_MSG ){
		if( pWnd ){
			pWnd->SetActiveWindow();
			pWnd->SetFocus();

			CDBDinpView	*pView = (CDBDinpView*)pWnd;
			if( pView ){
				pView->ResetFocusFormImg();
			}
		}
	}
	else if( msg == ID_IMGDLG_ENDDIALOG_MSG ){

		// 摘要を元に戻す
		((CDBDinpView*)pWnd)->ResetTekiyoFromImg();

		if( INP_mode == _APPEND ){
			if( pCbmsgInfo ){
				if( pCbmsgInfo->srcOrder == 0 ){
//--> '12.09.28 INS START
					((CDBDinpView*)pWnd)->SaveDocseq( pCbmsgInfo->docseq );
//<-- '12.09.28 INS END
					// リンクをクリアする
					((CDBDinpView*)pWnd)->DelAllDocseq();

					((CDBDinpView*)pWnd)->ChgLinkBtnState( ID_DISPSLIP_BSTATE );
				}
				else if( pCbmsgInfo->srcOrder > 0 ){
					((CDBDinpView*)pWnd)->ChgLinkBtnState( ID_LINKOFF_BSTATE );
				}
			}
		}
		else{
			if( pCbmsgInfo ){
				if( pCbmsgInfo->srcOrder == 0 ){
					// リンクをクリアする
					((CDBDinpView*)pWnd)->DelAllDocseq();

					((CDBDinpView*)pWnd)->ChgLinkBtnState( ID_DISPSLIP_BSTATE );
					((CDBDinpView*)pWnd)->ChangeImgdlgType( ID_DBDINP_NODISPDLG_TYPE );

					((CDBDinpView*)pWnd)->IsChangeLinkDocSeq();
				}
				else{
					((CDBDinpView*)pWnd)->ChgLinkBtnState( ID_LINKOFF_BSTATE );
				}
			}
		}
	}
	else if( msg == ID_IMGDLG_PGCHANGED_MSG ){
		// 摘要を元に戻す
		((CDBDinpView*)pWnd)->ResetTekiyoFromImg();

		if( INP_mode == _APPEND ){
//--> '12.09.28 INS START
			if( pCbmsgInfo ){
				((CDBDinpView*)pWnd)->SaveDocseq( pCbmsgInfo->docseq );
			}
//<-- '12.09.28 INS END
/*- '12.09.12_CUT -*/
//			if( ((CDBDinpView*)pWnd)->IsConnectedDoc() ){
//				((CDBDinpView*)pWnd)->ChgLinkBtnState( ID_LINKOFF_BSTATE );
//			}
//			else{
//				((CDBDinpView*)pWnd)->ChgLinkBtnState( ID_LINKON_BSTATE );
//			}
/*-----------------*/
		}
	}
	else if( msg == ID_IMGDLG_ADDIMG_ON_MSG ){
		// 摘要を元に戻す
		((CDBDinpView*)pWnd)->ResetTekiyoFromImg();

		if( INP_mode == _APPEND ){
/*- '12.09.27 -*/
//			if( ((CDBDinpView*)pWnd)->IsConnectedDoc() ){
//				((CDBDinpView*)pWnd)->ChgLinkBtnState( ID_LINKOFF_BSTATE );
//			}
//			else{
//				((CDBDinpView*)pWnd)->ChgLinkBtnState( ID_LINKON_BSTATE );
//			}
/*-------------*/
			((CDBDinpView*)pWnd)->ChgLinkBtnState( ID_LINKON_BSTATE );
/*-------------*/
		}
//--> '12.09.18 INS START
		else{
			((CDBDinpView*)pWnd)->ChangeImgdlgType( ID_DBDINP_ADDLINKDLG_TYPE );
			((CDBDinpView*)pWnd)->ChgLinkBtnState( ID_LINKON_BSTATE );
		}
//<-- '12.09.18 INS END

//--> '12.09.25 INS START
		// フォーカス再セット
		CDBDinpView	*pView = (CDBDinpView*)pWnd;
		if( pView ){
			pView->ResetFocusFormImg();
		}
//<-- '12.09.25 INS END
	}
#ifndef _SCANNER_SV_
	else if( msg == ID_IMGDLG_LINKON_MSG ){
		if( ((CDBDinpView*)pWnd)->GetLinkBtnState() == ID_LINKON_BSTATE ){
//			((CDBDinpView*)pWnd)->PostMessage( WM_COMMAND, IDC_LINKSLIP_BTN );
			((CDBDinpView*)pWnd)->SendMessage( WM_COMMAND, IDC_LINKSLIP_BTN );
		}
	}
	else if( msg == ID_IMGDLG_LINKOFF_MSG ){
		if( INP_mode == _APPEND ){
			if( ((CDBDinpView*)pWnd)->GetLinkBtnState() == ID_LINKOFF_BSTATE ){
//				((CDBDinpView*)pWnd)->PostMessage( WM_COMMAND, IDC_LINKSLIP_BTN );
				((CDBDinpView*)pWnd)->SendMessage( WM_COMMAND, IDC_LINKSLIP_BTN );
			}
		}
		else{
			if( ((CDBDinpView*)pWnd)->GetLinkBtnState() == ID_LINKOFF_BSTATE ){
//				((CDBDinpView*)pWnd)->PostMessage( WM_COMMAND, IDC_LINKSLIP_BTN );
				((CDBDinpView*)pWnd)->SendMessage( WM_COMMAND, IDC_LINKSLIP_BTN );
			}
		}
	}
#else
	else if( msg == ID_IMGDLG_LINKON_MSG ){
		((CDBDinpView*)pWnd)->DoAfterLinkCall( msg );
	}
	else if( msg == ID_IMGDLG_LINKOFF_MSG ){
		((CDBDinpView*)pWnd)->DoAfterLinkCall( msg );
	}
	else if( msg == ID_IMGDLG_LINKOFF_FIX_MSG ){
		((CDBDinpView*)pWnd)->DoAfterDropLinkCall( msg, pCbmsgInfo->leftLinkCnt );
	}
//-- '17.05.11 INS START
	else if( msg == ID_IMGDLG_ALL_LINKON_MSG ){
		((CDBDinpView*)pWnd)->DoAfterLinkCall( msg );
	}
	else if( msg == ID_IMGDLG_ALL_LINKOFF_MSG ){
		((CDBDinpView*)pWnd)->DoAfterLinkCall( msg );
	}
//-- '17.05.11 INS END
	else if( msg == ID_IMGDLG_BATCH_F5_MSG ){
		CDBbaseView::m_ImgPrintFlg = PRINT_VERIFY;
		pWnd->PostMessage(WM_COMMAND, ID_BUTTON_F5);
	}
	else if( msg == ID_IMGDLG_PRINTIMG_F5_MSG ){
		CDBbaseView::m_ImgPrintFlg = PRINT_DOCIMG;
		pWnd->PostMessage(WM_COMMAND, ID_BUTTON_F5);
	}
#endif
}
#endif

#ifdef _SCANNER_SV_
//-----------------------------------------------------------------------------
// 原票表示ダイアログでリンクが解除された時の後処理
//-----------------------------------------------------------------------------
void CDBDinpView::DoAfterDropLinkCall( int msg, int leftCnt )
{
	if( ((CREC.m_dsign[7]&0x30)&&!(pDBzm->zvol->tl_cor&0x01)) || !(pDBzm->zvol->tl_cor&0x10) || IsJzSansyo() ){
		return;
	}

	if( leftCnt == 0 ){
		CREC.m_dsign[6] &= ~0x01;
		getLINE_DATA()->_XXDTA.dd_dsign &= ~0x0800;

//		ChgLinkBtnState( ID_DISPSLIP_BSTATE );
		MoveSubWindow( 0 );
	}
}


// DocSeq 比較
int __cdecl SeqCompare(void *context, const void *ele1, const void *ele2 )
{
	int ret;

	int* dw1, *dw2;

	dw1 = (int*)ele1;
	dw2 = (int*)ele2;

	if( *dw1 > *dw2 )		ret = 1;
	else if( *dw1 < *dw2 )	ret = -1;
	else					ret = 0;

	return ret;
}


//-----------------------------------------------------------------------------
// リンクが変更されたかのチェック
//-----------------------------------------------------------------------------
BOOL CDBDinpView::IsChangeLinkDocSeq()
{
	if( ! m_bSLIP_MASTER )
		return FALSE;
	
	if( !(CREC.m_dsign[6] & 0x01) )
		return FALSE;

	int dcnt1 = m_SRECdseqAry.GetCount();
	CArray<int >	now_dseq;
	BOOL change = FALSE;
	CString Sql, strvalue;
	int docseq, i;
	int dcnt2 = 0;

	//変更時
	CRecordset	rset(pDBzm->m_database);
	Sql.Format( "select docseq from lnkseq where dtseq = %d order by docseq", CREC.m_seq );
	try{
		rset.Open( CRecordset::forwardOnly, Sql, CRecordset::readOnly );
	}
	catch( CDBException* dbe ){	// SQL自体のエラー
		dbe->m_strError;
		dbe->Delete();
		return FALSE;
	}

	for (i = 0; ; i++) {
		if (i)
			rset.MoveNext();
		if (rset.IsEOF() == TRUE)
			break;
		rset.GetFieldValue( (SHORT)0, strvalue );
		docseq = atoi( (LPCTSTR)strvalue );

		now_dseq.Add(docseq);
		dcnt2++;
	}
 
//MyTrace( "IsChangeLinkDocSeq(%d) dcnt1 = %d, dcnt2 = %d\n", m_ImgDtseq, dcnt1, dcnt2 );

	if( dcnt1 == 0 && dcnt2 == 0 )
		return FALSE;

	if( dcnt1 != dcnt2 ) 
		change = TRUE;
	else {
		if( dcnt1 > 0 )
			qsort_s( &m_SRECdseqAry[0], dcnt1, sizeof(int), SeqCompare, this );

		for( int n = 0; n < dcnt1; n++ ) {
//MyTrace( "IsChangeLinkDocSeq[%d], [%d]\n", m_SRECdseqAry[n], now_dseq[n] );
			if( m_SRECdseqAry[n] != now_dseq[n] ) {
				change = TRUE;
				break;
			}
		}
	}

	if( change ) {
		MODEFY_BIT.onbitn( CREC.m_seq - 1);
	}

	return change;

}



//-----------------------------------------------------------------------------
// 原票表示ダイアログでリンクが切り替えられた時の後処理
//-----------------------------------------------------------------------------
void CDBDinpView::DoAfterLinkCall( int msg )
{
	// リンク処理
	if( IsDispSlip() ){
//MyTrace("DoAfterLinkCall msg = %d, CREC.m_seq  %d, m_NowImgSeq(%d)\n", msg, CREC.m_seq, m_NowImgSeq);

		if( ((CREC.m_dsign[7]&0x30)&&!(pDBzm->zvol->tl_cor&0x01)) || !(pDBzm->zvol->tl_cor&0x10) || IsJzSansyo() ){
			return;
		}
//-- '17.05.11 INS START
		if( msg == ID_IMGDLG_ALL_LINKON_MSG ){
			int st = 0;
			if( INP_mode == _APPEND ){
				CArray<int>	tmpLinkDocSeqAry;
				m_pImgmng->GetLinkDocseqOnDlg( tmpLinkDocSeqAry );
				if( tmpLinkDocSeqAry.GetCount() ){
					m_ContDocseqAry.Copy(tmpLinkDocSeqAry);
					CREC.m_dsign[6] |= 0x01;
				}
			}
			else{
				if( (m_DImgdlgType==ID_DBDINP_INPUTDLG_TYPE) || (m_DImgdlgType==ID_DBDINP_ADDLINKDLG_TYPE) ){
					CArray<int>	tmpLinkDocSeqAry;
					m_pImgmng->GetLinkDocseqOnDlg( tmpLinkDocSeqAry );
					if( tmpLinkDocSeqAry.GetCount() ){
						m_ContDocseqAry.Copy(tmpLinkDocSeqAry);
						CREC.m_dsign[6] |= 0x01;
					}
				}
			}
		}
		else if( msg == ID_IMGDLG_ALL_LINKOFF_MSG ){
			if( INP_mode == _APPEND ){
				m_ContDocseqAry.RemoveAll();
				CREC.m_dsign[6] &= ~0x01;
			}
			else{
				if( (m_DImgdlgType==ID_DBDINP_INPUTDLG_TYPE) || (m_DImgdlgType==ID_DBDINP_ADDLINKDLG_TYPE) ){
					m_ContDocseqAry.RemoveAll();
					CREC.m_dsign[6] &= ~0x01;
				}
			}
		}
//		if( msg == ID_IMGDLG_LINKON_MSG ){			// 関連追加
		else if( msg == ID_IMGDLG_LINKON_MSG ){			// 関連追加
//-- '17.05.11 INS END
			int st = 0;
			if( INP_mode == _APPEND ){
				int docseq;
				if( (docseq=m_pImgmng->GetDocseqOnImgDlg()) != -1 ){
					m_ContDocseqAry.Add( docseq );
					CREC.m_dsign[6] |= 0x01;

// 表示ダイアログでのリンク切替はフォーカスを戻さない
//					set_focus( get_nowpn() );
				}
			}
			else{
				if( (m_DImgdlgType==ID_DBDINP_INPUTDLG_TYPE) || (m_DImgdlgType==ID_DBDINP_ADDLINKDLG_TYPE) ){
					int docseq;

					if( (docseq=m_pImgmng->GetDocseqOnImgDlg()) != -1 ){
						m_ContDocseqAry.Add( docseq );
						CREC.m_dsign[6] |= 0x01;

// 表示ダイアログでのリンク切替はフォーカスを戻さない
//						set_focus( get_nowpn() );
					}
				}
				else if( m_DImgdlgType==ID_DBDINP_CONFIRMDLG_TYPE ) {
				}
			}
		}
		else if( msg == ID_IMGDLG_LINKOFF_MSG ){	// 関連解除
			if( INP_mode == _APPEND ){
				DelDocseq();
				int cnt = (int)m_ContDocseqAry.GetCount();
				if( cnt == 0 ){
					CREC.m_dsign[6] &= ~0x01;
				}

// 表示ダイアログでのリンク切替はフォーカスを戻さない
//				set_focus( get_nowpn() );
			}
			else{
//MyTrace( "m_DImgdlgType = %d\n", m_DImgdlgType );

//--> '16.02.16 INS START
				if( (m_DImgdlgType==ID_DBDINP_INPUTDLG_TYPE) || (m_DImgdlgType==ID_DBDINP_ADDLINKDLG_TYPE) ){

					int st =  m_pImgmng->GetLinkDocseqOnDlg(m_ContDocseqAry);
//MyTrace( "GetLinkDocseqOnDlg(st=%d), m_ContDocseqAry = %d\n", st, m_ContDocseqAry.GetCount() );
					if( (int)m_ContDocseqAry.GetCount() == 0 ){
						CREC.m_dsign[6] &= ~0x01;
					}
				}
//<-- '16.02.16 INS END

/*				int nowCnt = m_pImgmng->GetCountRelImg( pDBzm, getLINE_DATA()->_XXDTA.dd_seq );
				if( nowCnt > 0 ){
					BOOL	nowAddflg = FALSE;
					int		nowDocseq = m_pImgmng->GetDocseqOnImgDlg();
					int		max = (int)m_ContDocseqAry.GetCount();
					for( int i=0; i<max; i++ ){
						if( m_ContDocseqAry[i] == nowDocseq ){
							nowAddflg = TRUE;
							break;
						}
					}

					if( nowAddflg == FALSE ){
						if( ICSMessageBox(_T("表示中の原票と仕訳の関連を解除してよろしいですか？"), (MB_YESNO|MB_DEFBUTTON2), 0, 0, this) == IDYES ){
							m_pImgmng->DisConnectDataToImgFile( pDBzm, getLINE_DATA()->_XXDTA.dd_seq );
							int newCnt = m_pImgmng->GetCountRelImg( pDBzm, getLINE_DATA()->_XXDTA.dd_seq );
							if( (nowCnt==-1) || (newCnt==-1) ){
								// エラー
								;
							}
							else if( newCnt == 0 ){
								CREC.m_dsign[6] &= ~0x01;
								getLINE_DATA()->_XXDTA.dd_dsign &= ~0x0800;

								ChgLinkBtnState( ID_DISPSLIP_BSTATE );
								MoveSubWindow( 0 );
							}
							else if( newCnt && (nowCnt>newCnt) ){
								MoveSubWindow( 0 );
								DispImgDlg( FALSE );
							}
						}
						else{
							m_pImgmng->ChgLinkStateInImgDlg( 1 );
						}
					}
					else{
						DelDocseq();
						ChgLinkBtnState( ID_LINKON_BSTATE );
					}

					// フォーカス再セット
					set_focus( get_nowpn() );
				}
				else{
					DelDocseq();
					int cnt = (int)m_ContDocseqAry.GetCount();
					if( cnt == 0 ){
						CREC.m_dsign[6] &= ~0x01;
					}
					ChgLinkBtnState( ID_LINKON_BSTATE );

//					set_focus( get_nowpn() );
				}*/
			}
		}
	}
	else if( m_bSLACC_BUTTON_DISP ){
		DispImgDlg( FALSE );
	}
}
#endif

void CDBDinpView::DoDataExchange(CDataExchange* pDX)
{
	CDBbaseView::DoDataExchange(pDX);

	//	DDX_Control(pDX, IDC_ICSSELCTRL2, m_selval32);
	//	DDX_Control(pDX, IDC_ICSSELCTRL1, m_selfix8);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL1, m_Input);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL2, m_Inp2);
	DDX_Control(pDX, IDC_ICSDISPCTRL12, m_GEN1);
	DDX_Control(pDX, IDC_ICSDISPGENKIN, m_GEN2);
	DDX_Control(pDX, IDC_DTLIST, m_DtList);
	DDX_Control(pDX, IDC_DTLINE, m_DtLine);
	DDX_Control(pDX, IDC_MNTHSEL, m_selDate);
	DDX_Control(pDX, IDC_DISPSLIP_BTN, m_DispSlipBtn);
	DDX_Control(pDX, IDC_LINKSLIP_BTN, m_LinkSlipBtn);
	DDX_Control(pDX, IDC_DISPSLIP_CHK, m_DispSlipChk);
	DDX_Control(pDX, IDC_GNPSCAN_BTN, m_GnpScanBtn);
}

BOOL CDBDinpView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CDBbaseView::PreCreateWindow(cs);
}


// CDBDinpView 診断

#ifdef _DEBUG
void CDBDinpView::AssertValid() const
{
	CDBbaseView::AssertValid();
}

void CDBDinpView::Dump(CDumpContext& dc) const
{
	CDBbaseView::Dump(dc);
}

CDBDinpDoc* CDBDinpView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDBDinpDoc)));
	return (CDBDinpDoc*)m_pDocument;
}
#endif //_DEBUG


// CDBDinpView メッセージ ハンドラ


void CDBDinpView::OnDraw(CDC* pDC)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	BeginApp();
}


int CDBDinpView::BeginApp( int recosel/*=0*/ )
{
	//二回目以降は処理を行いません。注意！これより以下の二行は変更してはいけません。
//	if( !FirstTime() )	return -1;
	FirstTime()	=	FALSE;

//TRACE( "DTLIST_DATA sz = %d\n", sizeof(DTLIST_DATA) );
//TRACE( "CString sz = %d\n", sizeof(CString));

	//この位置にアプリケーションの初期化処理を記述して下さい。

	try {
		//extern *CDBDinpView
		p_DBDinpView = this;
		// ツールバー変更
		((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(0);
		enable_fixval(TRUE);

		// 選択ウィンドウ
		if( MakeSelWnd() != 0 ) {
			return -1;
		}
		m_Input.ShowWindow(SW_HIDE);
		m_Inp2.ShowWindow(SW_HIDE);

		//data display initialize
		for( int i = 0 ; i < DDATA_LNMAX ; ++i ) {
			//データイニシャライズ
			inpline_inz( &DDATA_LINE[i] );
		}
#ifdef _SLIP_ACCOUNTS_
		m_SlipPakageCd = IsBuyScannerSave() ? 0 : -1;
//MyTrace("m_SlipPakageCd = %d\n", m_SlipPakageCd );
#endif

		SwkListSortClear();

		InitScanPgTBL( &SCAN_PGTBL );
		// マルチウィンドウ 行数増やす
#ifdef BLK_CLOSE
		OnSizeJob();
#else
		BlkMoveOnSizeJob(1);
#endif

		// 定型仕訳の 表示欄
		RDRemainDisp( FALSE );

		StartBlock();

		//ビューサイズ取得
		if( ! recosel ) {
		//	GetClientRect( &m_ChkViewRect );
		}

		m_bBmpMake = FALSE;
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}

#ifdef _DEBUG
#define TRACE_SIZEOF( x )	 \
	{ CString str; str.Format( "sizeof %s, %d\n", #x, sizeof x ); OutputDebugString( (LPCTSTR)str ); }
TRACE_SIZEOF( CString );
TRACE_SIZEOF( CDBINPDataRec );
TRACE_SIZEOF( CDBipEZREC );
TRACE_SIZEOF( CDBipTKREC );
TRACE_SIZEOF( CDBZmSub );
TRACE_SIZEOF( CDipZmSub );
TRACE_SIZEOF( DTLIST_DATA );
TRACE_SIZEOF( _DATA_LINE );
#endif

	return 0;

}


BOOL CDBDinpView::EndApp( WPARAM wParam, BOOL bCosel )
{
int sg;

	//通常の終了処理を記述して下さい。
	//終了を許可する場合はTRUE、許可しない場合はFALSEを返します。
	PROGRAM_OK = FALSE;
	if( (wParam == 1) || wParam == 100 ) {
		//ICSExit()でエラー終了した場合はこちらに記述します。

		if( INP_mode != _RETREV ) 
		{
			if( get_nowln() != -1 )
			{
				int pn = GetNowLineSetPN();;
				__set_focus( get_nowln(), pn );

				//別画面入力消去
				Destroy_ModeLessDlg();
			}
		}

		EndBlock(-1);
	}
	else {
		//それ以外はこちらに記述します。

		// ツールバーのボタンで会社選択時
		if( bCosel )	goto CONFIRM_TAG;

		switch( ((CMainFrame*)GetDBDinpMainFrame())->NowView() )
		{
		case INP_VIEW:
			if( INP_mode == _RETREV )
			{
				ASSERT( FALSE );
			}
			else if( INP_mode == _MODEFY )
			{
				if( ! bCONFIRM_MASTER ) {	// 未確定マスター
					PROGRAM_OK = TRUE;
	
					if( m_pSyzInp && get_nowpn() == SKBN_PN )
					{
					//	m_pSyzInp->PostMessage( WM_KEYDOWN, VK_END );
					//	m_pSyzInp->PostMessage( WM_KEYUP, VK_END );
						PostMessage( WM_SKBN_INEND, VK_END );
					}
					else
					{
						PostMessage( WM_KEYDOWN, VK_END );
						PostMessage( WM_KEYUP, VK_END );
					}
					return FALSE;
				}
			}
			else if( INP_mode == _APPEND )
			{
				PROGRAM_OK = TRUE;
				TermDistribute( 0 );
				PROGRAM_OK = FALSE;
			}

			break;
		}

#ifdef REGDATA	// 11.06 /00
		// 定型仕訳処理チェック
		if( RemainRdata() )
		{
			int	st;

			st	=	myICSMessageBox( _T("定型仕訳処理中です。\n\n処理を終わりますか？"), MB_YESNO|MB_DEFBUTTON2 );

			if( st == IDYES )
			{
				EndRdata();	// 定型仕訳テーブルの削除
				
				PROGRAM_OK = TRUE;
			}
			else
				PROGRAM_OK = TRUE;

			set_focus( get_nowpn() );
			return	FALSE;
		}
#endif

CONFIRM_TAG:	// ツールバーのボタンで会社選択時
		int	st = IDYES;

		if( ! bCosel ) {
			if( ((CMainFrame*)GetDBDinpMainFrame())->IsCloudEndMsg() ) {
				//クラウド共有マスターでかつ事務所(仕訳確定への移行メッセージを後で出す)
			}
			else {
				st	=	ICSJobEndBox( _T("仕訳入力を終了してもよろしいですか？") );
			}
		}

		if( st == IDYES ) {

			try
			{
				// 最前面に表示する 09.11 /07
				ICSSetForegroundWindow( GetDBDinpMainFrame()->m_hWnd );

				// 最終データチェック
				if( INP_mode == _APPEND )
				{
					switch( Wdata_ok() )
					{
					case 1:	// 最後のＷｒｉｔｅが残っている
						st	=	myICSMessageBox( _T("最後の仕訳が書き込まれていません。書き込みを行いますか？\n\n「いいえ」の場合この仕訳は破棄されます。"), MB_YESNO|MB_DEFBUTTON2 );

						if( st == IDYES )
						{
							if( WandNextData(1) == -1 )
							{
								scheck( 3 );

								ICSMessage( 0, get_Dfullmsg() );
							}
							else {
								if( IsDispSlip() ) {
									int cnt = (int)m_ContDocseqAry.GetCount();
									if( cnt ) {
										m_pImgmng->ConnectDataToImgFile(pDBzm, REAL_SEQ_NO, m_ContDocseqAry);
										m_ContDocseqAry.RemoveAll();
										if( m_ImgDispOption.nextpgType == ID_AUTO_NEXTPAGE ) {
											m_pImgmng->DispImgDlgUnlinkNextPage();
										}
									}
								}
							}
							ddata_dsp(-1);

							//データラインイニシャライズ
							inpline_inz( &INP_LINE );
							INP_LINE._XXDTA.dd_ddate = SREC.m_ddate;//初期日付
							inpline_dsp( &INP_LINE, APPEND_LINE );

					//!		m_selval32.SetData( (long)SelVal32 );
					//		SetICSSel32( (long)SelVal32, (pAUTOSEL->KMNO_OPT != OPT_KMNO) );
							SelPar par;
							par.sel_pn = SL_KAMOKU_PN;
							m_Seldata.SelectJob( &par, -1 );

							DB_SyIniz( &CREC );

							set_nowpn( DATE_PN );
							__set_focus( get_nowln(), DATE_PN );

							//別画面入力消去
							Destroy_ModeLessDlg();
						}
						else {
							inpline_inz( &INP_LINE );
							INP_LINE._XXDTA.dd_ddate = SREC.m_ddate;//初期日付
							inpline_dsp( &INP_LINE, APPEND_LINE );
						}
						break;
					case 2:	// 最後のデータが不完全
						st	=	myICSMessageBox( _T("最後の仕訳が不完全です。仕訳を完成させますか？\n\n「いいえ」の場合この仕訳は破棄されます。"), MB_YESNO|MB_DEFBUTTON2 );

						if( st == IDYES )
						{
							PROGRAM_OK = TRUE;
							set_focus( get_nowpn() );
							return FALSE;
						}
						else {
							inpline_inz( &INP_LINE );
							INP_LINE._XXDTA.dd_ddate = SREC.m_ddate;//初期日付
							inpline_dsp( &INP_LINE, APPEND_LINE );
						}
						break;
					default:	// 最後のデータも書き込まれている
						break;
					}
				}

				//諸口貸借　イニシャライズ&表示
				if( totaldisp( bCosel ? MODE_COSEL : MODE_END ) == IDCANCEL )
				{
					PROGRAM_OK = TRUE;
					int pn = GetNowLineSetPN();
					set_focus( pn );
					return FALSE;
				}

				if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() == INP_VIEW && INP_mode != _RETREV ) 
				{
					int pn = GetNowLineSetPN();
					__set_focus( get_nowln(), pn );

					//別画面入力消去
					Destroy_ModeLessDlg();
				}

				if( DATA_1RD )
				{
					//CREC to 残高ADD.
					sg = rewupdat( &CREC, &SCAN_rec, INP_mode, TRUE, TRUE );
					DATA_1RD = 0;

					if( sg == 99 )
					{
						BRTORK_DATA.jobend = TRUE;
						return FALSE;
					}

					if( rewupdat_job( sg ) ) {
						return FALSE;
					}
				}

				if( ((CMainFrame*)GetDBDinpMainFrame())->IsCloudEndMsg() ) {
					if( ! ((CMainFrame*)GetDBDinpMainFrame())->CloudEndChainJob() ) {
						st	=	ICSJobEndBox( _T("仕訳入力を終了してもよろしいですか？") );
						if( st != IDYES ) {
							PROGRAM_OK = TRUE;
							int pn = GetNowLineSetPN();
							set_focus( pn );
							return FALSE;
						}
					}
				}

//				if( ! bCosel )	EndBlock(0);
			}
			catch( CErrBlk E )
			{
				ErrExit( E.ErrCode, E.ErrMsg );
				return FALSE;
			}
#ifdef _SLIP_ACCOUNTS_
			if( m_pImgmng ){
				if( pAUTOSEL != NULL ){
/*- '12.10.03 -*/
//					if( m_bSLIP_MASTER && (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_NOT_LINKIMG) ){
/*-------------*/
/*-- '12.10.26 --*/
//					if( m_bSLIP_MASTER && (m_ImgDispOption.autoLkType==ID_AUTOLINK_NOLINK) ){
/*---------------*/
					if( m_bSLIP_MASTER ){
/*---------------*/
/*-------------*/
						pAUTOSEL->SLIP_SEQ = m_NowImgSeq;
					}
				}
				ChgLinkBtnState( ID_DISPSLIP_BSTATE );
				m_pImgmng->DispImgDlgDelete();
				delete m_pImgmng;
				m_pImgmng = NULL;
			}
#endif	// _SLIP_ACCOUNTS_
			return	TRUE;
		}
		else if( st == IDNO ) {
			PROGRAM_OK = TRUE;
			__set_focus( get_nowln(), get_nowpn() );
			return	FALSE;
		}
		else if( st == IDCANCEL ) {
			return	TRUE;
		}

	}

	return	TRUE;
}



void CDBDinpView::ReCoselCheckOtherPro()
{
	if( RemainRdata() )
	{
		EndRdata();	// 定型仕訳テーブルの削除
	}
}

//定型仕訳処理中か？
BOOL CDBDinpView::EndAppChkRDdata( WPARAM wParam )
{
	if( (wParam == 1) || wParam == 100 ) {

	}
	else {
		if( INP_mode == _APPEND || ((CMainFrame*)GetDBDinpMainFrame())->IsReCOSEL() ) {
			// 定型仕訳処理チェック
			if( RemainRdata() )
			{
				int	st;
				PROGRAM_OK = FALSE;

				st	=	myICSMessageBox( _T("定型仕訳処理中です。\n\n処理を終わりますか？"), MB_YESNO|MB_DEFBUTTON2 );

				if( st == IDYES )
				{
					EndRdata();	// 定型仕訳テーブルの削除
				}

				PROGRAM_OK = TRUE;

				if( ((CMainFrame*)GetDBDinpMainFrame())->IsReCOSEL() ) {
					if( st == IDYES ) {
						return TRUE;
					}
				}
				set_focus( get_nowpn() );
				return	FALSE;
			}
		}
	}

	return TRUE;
}


BOOL CDBDinpView::EndChk()
{
	int	st;

#ifdef REGDATA	// 11.06 /00
	// 定型仕訳処理チェック
	if( RemainRdata() )
	{
		st	=	myICSMessageBox( _T("通常入力で、定型仕訳処理中です。\n\n処理を終わりますか？"), MB_YESNO|MB_DEFBUTTON2 );
		if( st == IDYES )
		{
			EndRdata();	// 定型仕訳テーブルの削除
			return TRUE;
		}
		else {
			return	FALSE;
		}
	}
#endif

	// 最終データチェック
	switch( AppendWdata_ok() ) 
	{
		case 1:	// 最後のＷｒｉｔｅが残っている
			st	=	myICSMessageBox( _T("通常入力で、書き込まれていない仕訳があります。\n通常入力に戻りますか？"), MB_YESNO|MB_DEFBUTTON2 );
			if( st == IDYES ) {
				return FALSE;
			}
			break;
		case 2:
			st	=	myICSMessageBox( _T("通常入力で、未完成の仕訳があります。\n通常入力に戻りますか？"), MB_YESNO|MB_DEFBUTTON2 );
			if( st == IDYES ) {
				return FALSE;
			}
			break;
	}

	return TRUE;
}



// 摘要のみ修正か？
BOOL CDBDinpView::IsSCAN_TKYONLY()
{
	return ((CMainFrame*)GetDBDinpMainFrame())->IsTkyOnly();
}


//消費税入力画面表示
int CDBDinpView::SkbnInDisp(long idc)
{
	if (m_pSyzInp == NULL)
	{
		ermset( ERROR_ENV, "消費税入力オブジェクトがありません。");
		return -1;
	}
	else
	{
#ifdef _ICSPEN_	// 01.20 /01
		CWnd *org;
		int sw;

		CSize size;
		m_pSyzInp->GetCrntRect( size );
//		m_pSyzInp->get_originalsize( w, h );
		CRect	irect;

//		SelWndShow( TRUE );

		int ln = get_nowln();
		if( ln == APPEND_LINE ) {
			m_DtLine.GetItemScreenRect( 0, DATA_SYZ, irect );
		}
		else {
			m_DtList.GetItemScreenRect( ln, DATA_SYZ, irect );
		}
		int inpHt = irect.Height();

		org	=	GetDlgItem( idc );
		org->GetWindowRect( irect );
		this->ScreenToClient( &irect );
		irect.left = irect.right;

MyTrace("m_pSyzInp->DialogON irect(1)= %d,%d,%d,%d\n", irect.left, irect.top, irect.right, irect.bottom);

		BOOL bOK = dinp_modefy_ok( getLINE_DATA() );

		CRect vwRect;
		GetClientRect( vwRect );

		if( (irect.bottom + size.cy) >= vwRect.bottom ) {
			int tmp = (irect.bottom + size.cy) - vwRect.bottom;

			if( INP_mode != _KEYSCAN ) {
				irect.top -= tmp;
				irect.bottom -= tmp;
			}
		}

		int pos_select, pos_dtnew, pos_list;
		int pos_flg;

		pos_select = pos_dtnew = pos_list = -1;

		//入力タイプ・現金欄 の位置をチェック
		for( int n = 0; n < DINP_BLKMAX; n++ ) {
			if( m_blkIdx[n] == BLK_DTNEW ) {
				pos_dtnew = n;
			}
			if( m_blkIdx[n] == BLK_SELECT ) {
				pos_select = n;
			}
			if( m_blkIdx[n] == BLK_DTLIST ) {
				pos_list = n;
			}
		}
		pos_flg = SYP_TOPLEFT;

		BOOL btm_sgn = FALSE;

		if( pos_select < pos_list ) {
			CRect selRect;
			if( pAUTOSEL->BLKSEL_OPT == OPT_NONOPT ) {
				m_selval32.GetWindowRect( selRect );
				this->ScreenToClient( &selRect );
MyTrace("m_pSyzInp->DialogON selRect(1)= %d,%d,%d,%d\n", selRect.left, selRect.top, selRect.right, selRect.bottom);

				if( selRect.bottom > (irect.top - size.cy) )
					btm_sgn = TRUE;
			}
		}

		if( (irect.top - size.cy) < 0 || btm_sgn ) {
MyTrace("m_pSyzInp->DialogON caoll (SYP_BOTTOMLEFT)= %d,%d,%d,%d\n", irect.left, irect.top, irect.right, irect.bottom);

			sw = m_pSyzInp->DialogON( bOK, irect, SYP_BOTTOMLEFT, m_SET_SYZINP_PN );
		}
		else {
			if( pAUTOSEL->BLKSEL_OPT == OPT_NONOPT ) {
				if( pos_dtnew >= (pos_select+1) && pos_list < pos_select ) {
					if( ln == APPEND_LINE ) {
						pos_flg = SYP_BOTTOMRIGHT;
					}
				}
			}
			else {
				if( ln == APPEND_LINE ) {
					CRect listRect;
					m_DtList.GetWindowRect(&listRect);
					ScreenToClient(listRect);
					if( m_DtList.GetLines() == 1 ) {
						irect.top = listRect.bottom - (inpHt * 2) - (inpHt / 2);
					}
					else {
						irect.top = listRect.bottom - (inpHt * 4) - (inpHt / 2);
					}
				}
			}
		//	if( pos_select < pos_list )	pos_flg = SYP_TOPRIGHT;
MyTrace("m_pSyzInp->DialogON(2) = %d,%d,%d,%d, pos_flg(%d)\n", irect.left, irect.top, irect.right, irect.bottom, pos_flg);

			sw = m_pSyzInp->DialogON( bOK, irect, pos_flg/*SYP_TOPLEFT*/, m_SET_SYZINP_PN );
		}

		SelWndShow(TRUE);
#else
		CWnd *org;
		WINDOWPLACEMENT orgplace,dinpplace,skbninplace;
		RECT	newrect;
		int sw;
		double rate_w, rate_h;
		double w, h;

		get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット
		m_pSyzInp->get_originalsize( w, h );

		org = GetDlgItem( idc );
		org->GetWindowPlacement( &orgplace );
		this->GetWindowPlacement( &dinpplace );
		m_pSyzInp->GetWindowPlacement( &skbninplace );

		newrect.bottom = orgplace.rcNormalPosition.top;
		newrect.top = newrect.bottom - ( h * rate_h );
		newrect.right = orgplace.rcNormalPosition.left + (rate_w * 10);
		newrect.left = newrect.right - ( w * rate_w ) + (rate_w * 10);

		sw = m_pSyzInp->SetWindowPos( &wndTop, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_SHOWWINDOW );
#endif

		return sw;
	}
}


//部門／工事入力画面作成＆表示
void CDBDinpView::BkInpMakeX()
{
	if (m_pBkInpX == NULL)
	{
		if( !(m_pBkInpX = new CBkInpDlg(this)) )
		{
			ermset( ERROR_MEMORY, "部門／工事入力オブジェクトが作成できません。");
			return;
		}
	}

	if( m_pBkInpX->GetSafeHwnd() == NULL )
	{
		if( m_pBkInpX->Create() == FALSE )
		{
			ermset( ERROR_ENV, "部門／工事入力オブジェクトが作成できません。");
			return;
		}
	}
}

void CDBDinpView::BkInpMake()
{
	m_pBkInp = m_pBkInpX;
}

//部門／工事画面表示
void CDBDinpView::BkInpDisp( int pn )
{
	if (m_pBkInp == NULL)
	{
		ermset( ERROR_ENV, "部門／工事入力オブジェクトがありません。");
		return;
	}
	else
	{
#ifdef _ICSPEN_	// 01.20 /01
		int sw;
		RECT	newrect;
		CRect	irect, rc;

		int ln = get_nowln();
		int item;
		item = (pn == BMON_PN) ? DATA_DEB_BMON : DATA_DEB_KNO;

		if( ln == APPEND_LINE ) {
			m_DtLine.GetItemScreenRect( 0, item, irect );
		}
		else {
			m_DtList.GetItemScreenRect( ln, item, irect );
		}
		this->ScreenToClient( &irect );

		m_pBkInp->ShowWindow( SW_SHOW );
		m_pBkInp->GetWindowRect( rc );

		newrect.bottom	=	irect.top;
		newrect.top		=	newrect.bottom - rc.Height();
		newrect.left	=	irect.left;
		newrect.right	=	irect.left + rc.Width();

		CWnd*	pWnd	=	(CWnd*)this;
		CRect	rect;
		pWnd->GetClientRect( &rect );

		sw = m_pBkInp->SetWindowPos( &wndTop, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_SHOWWINDOW|SWP_NOACTIVATE );
		// 消費税エリアの再表示
		InvalidateRect( &newrect, TRUE );
#else
		CWnd *org;
		WINDOWPLACEMENT orgplace,dinpplace,skbninplace;
		RECT	newrect;
		int sw;
		double rate_w, rate_h;
		double w, h;

		get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット
		m_pBkInp->get_originalsize( w, h );

		org = GetDlgItem( idc );
		org->GetWindowPlacement( &orgplace );
		this->GetWindowPlacement( &dinpplace );
		m_pBkInp->GetWindowPlacement( &skbninplace );

		newrect.bottom = orgplace.rcNormalPosition.top;
		newrect.top = newrect.bottom - ( h * rate_h );
		newrect.right = orgplace.rcNormalPosition.left + (rate_w * 10);
		newrect.left = newrect.right - ( w * rate_w ) + (rate_w * 10);

		sw = m_pBkInp->SetWindowPos( &wndTop, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_SHOWWINDOW );
#endif

		m_pBkInp->UpdateWindow();
	}
}


//部門入力 画面消去
void CDBDinpView::Destroy_BkInp()
{

	if( m_pBkInp )
	{
		// 消費税入力ダイアログ非表示
//		m_pBkInp->SetWindowPos( 0, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE );
		m_pBkInp->ShowWindow(SW_HIDE);
		m_pBkInp->UpdateWindow();
		int pn;

		if( m_pBkInp->GetMode() == 0 )
			pn = BMON_PN;
		else
			pn = KOJI_PN;

		// データ表示
		TerminationDataSet( pn );

		m_pBkInp = NULL;
	}
}


//付箋入力表示
void CDBDinpView::HusenInpDisp( BOOL bOneHusen/*=FALSE*/ )
{
	if (m_pHusenInp == NULL)
	{
		ermset( ERROR_ENV, "付箋入力オブジェクトがありません。");
		return;
	}
	else
	{
#ifdef _ICSPEN_	// 01.20 /01
		int sw;

		CSize size;
		m_pHusenInp->GetCrntRect( size );
		CRect	irect;

		int ln = get_nowln();
		int item;

		if( bOneHusen ) {
			// 1行 時の 摘要欄
			item = DATA_TEKIYO;
			m_DtList.GetItemScreenRect( ln, item, irect );
		}
		else {
			item = DATA_HUSEN;

			if( ln == APPEND_LINE ) {
				m_DtLine.GetItemScreenRect( 0, item, irect );
			}
			else {
				m_DtList.GetItemScreenRect( ln, item, irect );
			}
		}

		this->ScreenToClient( &irect );
		irect.left = irect.right;

		BOOL bOK = dinp_modefy_ok( getLINE_DATA() );
		if( bCONFIRM_MASTER )	bOK = FALSE;

		if( (irect.top - size.cy) < 0 ) {
			sw = m_pHusenInp->DialogON( bOK, irect, HSN_BOTTOMLEFT, HSN_HSEL_PN );
		}
		else {
			sw = m_pHusenInp->DialogON( bOK, irect, HSN_TOPLEFT, HSN_HSEL_PN );
		}

		if( INP_mode == _APPEND )
			m_selDate.ShowWindow( SW_HIDE );

		if( pAUTOSEL->BLKSEL_OPT ) {
			SelWndMoveJob( TRUE );
		}

#else
		CWnd *org;
		WINDOWPLACEMENT orgplace,dinpplace,skbninplace;
		RECT	newrect;
		int sw;
		double rate_w, rate_h;
		double w, h;

		get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット
		m_pHuInp->get_originalsize( w, h );

		org = GetDlgItem( idc );
		org->GetWindowPlacement( &orgplace );
		this->GetWindowPlacement( &dinpplace );
		m_pHuInp->GetWindowPlacement( &skbninplace );

		newrect.bottom = orgplace.rcNormalPosition.top;
		newrect.top = newrect.bottom - ( h * rate_h );
		newrect.right = orgplace.rcNormalPosition.left + (rate_w * 10);
		newrect.left = newrect.right - ( w * rate_w ) + (rate_w * 10);

		sw = m_pHuInp->SetWindowPos( &wndTop, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_SHOWWINDOW );
#endif
	}
}

//付箋入力 画面消去
void CDBDinpView::Destroy_HusenInp()
{
	if (m_pHusenInp ) {
		if( m_pHusenInp->IsDisplay() ) {
			CDBbaseView::Destroy_HusenInp();
			// データ表示
			TerminationDataSet( DATA_HUSEN );
			// 1行 時の付箋OFF
			m_OneHusen = 0;
		}
	}
}


//変動事由入力表示
void CDBDinpView::HjiyuInpDisp()
{
	if (m_pHjiyuInp == NULL)
	{
		ermset( ERROR_ENV, "変動事由入力オブジェクトがありません。");
		return;
	}
	else
	{
		int sw;

		CSize size;
		m_pHjiyuInp->GetCrntRect( size );
		CRect	irect;

		int ln = get_nowln();
		int item;
		item = DATA_HUSEN;

		SelWndShow( TRUE );

#ifndef _SLIP_ACCOUNTS_
		if( ln == APPEND_LINE ) {
			m_DtLine.GetItemScreenRect( 0, item, irect );
		}
		else {
			if( m_DtList.GetLines() == 1 )
				item = DATA_HJIYU;

			m_DtList.GetItemScreenRect( ln, item, irect );
		}

		this->ScreenToClient( &irect );
		irect.left = irect.right;

		BOOL bOK = dinp_modefy_ok( getLINE_DATA() );
		sw = m_pHjiyuInp->DialogON( bOK, irect, HJY_TOPLEFT );
#else
		CRect	brect, selRect;
		m_selval32.GetWindowRect( &selRect );
		this->ScreenToClient( &selRect );
		int pos = HJY_TOPLEFT;

		if( ln == APPEND_LINE ) {
			if( m_bSLACC_BUTTON_DISP ){
				m_DtLine.GetItemScreenRect( 0, item, irect );
				m_LinkSlipBtn.GetWindowRect( &brect );

				this->ScreenToClient( &irect );
				this->ScreenToClient( &brect );
				irect.left = brect.right + 5;
				pos = HJY_TOPRIGHT;

				if( (irect.top - size.cy) < selRect.bottom ) {
					int ht = irect.Height();
					irect.InflateRect( 0, ht );
					pos = HJY_BOTTOMRIGHT;
				}

				BOOL bOK = dinp_modefy_ok( getLINE_DATA() );
				sw = m_pHjiyuInp->DialogON( bOK, irect, pos );
			}
			else{
				m_DtLine.GetItemScreenRect( 0, item, irect );
				this->ScreenToClient( &irect );
				irect.left = irect.right;

				if( (irect.top - size.cy) < selRect.bottom ) {
					int ht = irect.Height();
					irect.InflateRect( 0, ht );
					pos = HJY_BOTTOMLEFT;
				}

//MyTrace( "(B)selRect %d, %d, %d, %d\n", selRect.left, selRect.top, selRect.right, selRect.bottom );
				BOOL bOK = dinp_modefy_ok( getLINE_DATA() );
				sw = m_pHjiyuInp->DialogON( bOK, irect, pos );
			}
		}
		else {
			if( m_DtList.GetLines() == 1 )
				item = DATA_HJIYU;

			m_DtList.GetItemScreenRect( ln, item, irect );

			this->ScreenToClient( &irect );
			irect.left = irect.right;
			//はみ出た分調整
			if( (irect.right - size.cx) < 0 ) {
				int sa = (irect.right - size.cx);
				irect.left += abs(sa);
			}
			BOOL bOK = dinp_modefy_ok( getLINE_DATA() );

			if( (irect.top - size.cy) < 0 ) {
				pos = HJY_BOTTOMLEFT;
			}
			else {
				if( selRect.top > irect.top ) {	//選択画面が下側にある
					if( (irect.top + size.cy) > selRect.top ) {
						pos = HJY_TOPLEFT;
					}
				}
				else {
					if( (irect.top - size.cy) < selRect.bottom ) {
						pos = HJY_BOTTOMLEFT;
					}
				}
			}
			sw = m_pHjiyuInp->DialogON( bOK, irect, pos );
		}

		if( pAUTOSEL->BLKSEL_OPT ) {
			SelWndMoveJob( TRUE );
		}
#endif
	}
}

//付箋入力 画面消去
void CDBDinpView::Destroy_HjiyuInp()
{
	if (m_pHjiyuInp ) {
		if( m_pHjiyuInp->IsDisplay() ) {
			CDBbaseView::Destroy_HjiyuInp();
			// データ表示
			TerminationDataSet( DATA_HJIYU );
		}
	}
}


//サブ入力画面 表示
void CDBDinpView::SubDataInpDisp( int pn )
{
	if (m_pSubDtInp == NULL)
	{
		ermset( ERROR_ENV, "サブ入力ブジェクトがありません。");
		return;
	}
	else
	{
		int sw;

		CSize size;
		m_pSubDtInp->GetCrntRect( size );
		CRect	irect, seqRect;

		int ln = get_nowln();
		int item;
		item = DATA_TEKIYO;

		if( ln == APPEND_LINE ) {
			return;
		}
		else {
			if( m_DtList.GetLines() != 1 )
				return;

			m_DtList.GetItemScreenRect( ln, item, irect );

			m_DtList.GetItemScreenRect(ln, DATA_SEQ, seqRect);
		}

		this->ScreenToClient( &irect );
		irect.left = irect.right;

		this->ScreenToClient(&seqRect);

		//サブ入力画面の左端が隠れていないか？
		if ((irect.left - size.cx) < seqRect.left) {
			irect.left = seqRect.left;
			if ((irect.top - size.cy) < 0) {
				sw = m_pSubDtInp->DialogON(irect, SDT_BOTTOMRIGHT, pn);
			}
			else {
				sw = m_pSubDtInp->DialogON(irect, SDT_TOPRIGHT, pn);
			}
		}
		else {
			if ((irect.top - size.cy) < 0) {
				sw = m_pSubDtInp->DialogON(irect, SDT_BOTTOMLEFT, pn);
			}
			else {
				sw = m_pSubDtInp->DialogON(irect, SDT_TOPLEFT, pn);
			}
		}

		if( pAUTOSEL->BLKSEL_OPT ) {
			SelWndMoveJob( TRUE );
		}

		((CMainFrame*)GetDBDinpMainFrame())->StatusGuideDisp(AFX_IDS_IDLEMESSAGE);
	}
}

//サブ入力 画面消去
void CDBDinpView::Destroy_SubDataInp()
{
	if (m_pSubDtInp ) {
		if( m_pSubDtInp->IsDisplay() ) {
			CDBbaseView::Destroy_SubDataInp();
		}
	}
}

//移動するコントロール
// 下側より位置をセットする。
#ifndef _SLIP_ACCOUNTS_
static int gCtrlID[] = {
	IDC_STATIC_POS2,IDC_ICSDISPCTRL12, IDC_ICSDISPGENKIN, IDC_ICSDISPCTRL11, IDC_ICSDISPSYOGT,IDC_DISPBIGIMG,
	IDC_DTLINE,
	IDC_STATIC_POS, IDC_ICSDISPCTRL10, IDC_ICSDISPRDREMAIN, IDC_MNTHSEL
};
#else
static int gCtrlID[] = {
	IDC_STATIC_POS2,IDC_ICSDISPCTRL12, IDC_ICSDISPGENKIN, IDC_ICSDISPCTRL11, IDC_ICSDISPSYOGT,IDC_DISPBIGIMG,
	IDC_DTLINE,
	IDC_STATIC_POS, IDC_ICSDISPCTRL10, IDC_ICSDISPRDREMAIN, IDC_DISPSLIP_CHK, IDC_LINKSLIP_BTN, IDC_MNTHSEL
};
#endif

//ポジションテーブル
#ifndef FIX_CLOSE // 11.21 /12
	int gPNtbl[] = {
		SEQ_PN,DATE_PN,
		DENP_PN,DOCEVI_PN,
		BMON_PN,KOJI_PN,DEBT_PN,
		CBMN_PN,CKJI_PN,CRED_PN,
		VAL_PN,ZEI_PN,SKBN_PN,TKY_PN,
		SYON_PN,TGDT_PN,HJIYU_PN,
		-1,
	};
#endif


void CDBDinpView::OnInitialUpdate() 
{
CWnd *org;
//int i;
//RECT rct;
//WINDOWPLACEMENT place;

/*---------------------------*/

	int rsl = ResolutionRatio();

	DWORD wver = Version_Platform();
	if( wver <= WINVISTA_PFM ) {
		CRect rc( 0, 0, 509, 307 );
		CRect rc_w7( 0, 0, 509, 300 );

		((CDialog*)this)->MapDialogRect( &rc );
		((CDialog*)this)->MapDialogRect( &rc_w7 );

		int y = rc.bottom - rc_w7.bottom;

		CRect vrc;
		GetClientRect( &vrc );
		vrc.bottom += y;

		MoveWindow( vrc.top, vrc.left, vrc.Width(), vrc.Height() );

		GetDlgItem( IDC_DTLIST )->GetWindowRect( rc );
		ScreenToClient( rc );
		GetDlgItem( IDC_DTLIST )->MoveWindow( rc.left, rc.top, rc.Width(), rc.Height()+y );
		// オフセット分移動
		for( int i = 0; i < (sizeof gCtrlID / sizeof gCtrlID[0]); i++ ) {
			GetDlgItem( gCtrlID[i] )->GetWindowRect( rc );
			ScreenToClient( rc );
			rc.top		+= y;
			rc.bottom	+= y;
			GetDlgItem( gCtrlID[i] )->MoveWindow( rc.left, rc.top, rc.Width(), rc.Height() );
		}
	}

	if( rsl == RSL_LONGERHANWIDE /*psz->cy >= 1024*/ ) {
		int h_plus = 18;
		CRect rc, rect2;
		GetDlgItem( IDC_ICSSELCTRL1 )->GetWindowRect( rc );
		ScreenToClient( rc );
		GetDlgItem( IDC_ICSSELCTRL1 )->MoveWindow( rc.left, rc.top, rc.Width(), rc.Height()+h_plus );

		GetDlgItem( IDC_ICSSELCTRL2 )->GetWindowRect( rc );
		ScreenToClient( rc );
		GetDlgItem( IDC_ICSSELCTRL2 )->MoveWindow( rc.left, rc.top, rc.Width(), rc.Height()+h_plus );

		m_DtList.GetWindowRect( rect2 );
		ScreenToClient( rect2 );
		rect2.top = rc.top + rc.Height()+h_plus;
		m_DtList.MoveWindow( rect2.left, rect2.top, rect2.Width(), rect2.Height() );
	}
/*---------------------------*/

#ifdef _SLIP_ACCOUNTS_
	// CToolTipCtrl型オブジェクトを作成する
	m_tooltip.Create( this );
	ChgLinkBtnState( ID_DISPSLIP_BSTATE );
#endif

	CDBbaseView::OnInitialUpdate();
	ResizeParentToFit();

	//フォームの大きさにウィンドウの大きさを合わせます。
	//この機能が不要な場合は、以下の二行を削除するか
	//ResizeParentToFit()のパラメータをTRUEにして下さい。
//	GetDBDinpMainFrame()->RecalcLayout();
//	ResizeParentToFit( FALSE );
//	((CMainFrame*)GetDBDinpMainFrame())->MinMaxFlg()	=	TRUE;

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	// 縦の長さ変更

	VERIFY(org = GetDlgItem( IDC_DISPBIGIMG ));
	org->GetClientRect( &BIGIMG_RECT );
	org->SetWindowPos( 0, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOMOVE );
	// イメージ表示（大コントロール）の背景色を穏やかな青に
//	((CICSDisp*)org)->SetBackColor( SELECT_BACK_COL );
	((CICSDisp*)org)->SetCaption( "" );

	// 定型残り仕訳消去
	RDRemainIniz();

	m_Input.EnableDelete(0);
	m_Inp2.EnableDelete(0);

	m_Input.EnableHome(1);
	m_Inp2.EnableHome(1);

	m_DtLine.SetInputHwnd(0, m_Input.m_hWnd );
	m_DtList.SetInputHwnd(0, m_Inp2.m_hWnd );

#ifndef _DEBUG
	m_DtList.SetDoubleBuffering( TRUE );
#else
	m_DtList.SetDoubleBuffering( FALSE );
#endif
}


void CDBDinpView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
//MyTrace( "@CDBDinpView::OnActivateView TOP bActivate = %d, POK = %d\n", bActivate, PROGRAM_OK );	
//TRACE( "**activate view \n" );	
//	CDBbaseView::OnActivateView(bActivate, pActivateView, pDeactiveView);
//	return;

	if( PROGRAM_OK && m_ImgPrintFlg == PRINT_NONE ) {

#ifdef CLOSE/*--FILETrace-------------------------------*/
		// 表示行数が変わった
		int old_lncnt = SCROLL_NL();
		m_DtList.GetDispDataCount();
		int now_lncnt = m_DtList.GetDispDataCount();
MyTrace( "@OnDBDinpMsg old_lncnt = %d, now_lncnt = %d", old_lncnt, now_lncnt );
#endif		/*--FILETrace-------------------------------*/

		CWnd* pwnd;
		pwnd = GetDBDinpMainFrame();

		if( pwnd && ((CMainFrame*)pwnd)->NowView() == INP_VIEW && ! ((CMainFrame*)pwnd)->IsReCOSEL() ) {
 			if( bActivate && INP_mode != _RETREV ) {
				if( IsWindow( m_Input.m_hWnd ) && IsWindow( m_Inp2.m_hWnd ) ) {
					int pn;
					pn = get_nowpn();

#ifndef _SLIP_ACCOUNTS_
					if( !(pn == TKY_PN && m_pSubDtInp->IsDisplay() ) && pn != SKBN_PN && pn != HSEN_PN ) {
#else
//					if( !(pn == TKY_PN && m_pSubDtInp->IsDisplay() ) && pn != SKBN_PN && pn != HSEN_PN && (IsDispSlip()==FALSE) ) {
					BOOL bSet = FALSE;
					if( !(pn == TKY_PN && m_pSubDtInp->IsDisplay() ) && pn != SKBN_PN && pn != HSEN_PN && (IsDispSlip()==FALSE) )
						bSet= TRUE;
					if( bSet && m_pSelWnd != NULL ) {
						if( m_pSelWnd->IsWindowVisible() ) {
							if( pDeactiveView->m_hWnd == pActivateView->m_hWnd && pActivateView->m_hWnd == this->m_hWnd ) {
								bSet = FALSE;
							}
						}
					}
					if( bSet ) {
#endif
						if( m_bSLIP_MASTER && m_DispSlipChk.GetCheck() ) {
						//	SendMessage( WM_RETROFOCUS, pn );
						}
						else {
FILETrace("@OnActivateView set_focus pn = %d\n", pn );
							if( !m_bActiveIgnore ) {
								set_focus(pn);
							}
						}
					}
					if( m_bActiveIgnore ) {
						m_bActiveIgnore = FALSE;
					}

				}
			}
		}
	}

	CDBbaseView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}



BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lp) {
//	SendMessage(hWnd, WM_CLOSE, 0, 0);

	CWnd* pwnd = CWnd::FromHandle(hWnd);
	CRect rect;
	pwnd->GetWindowRect(&rect);
//	MyTrace("●●hWnd(%08x), rect(%d,%d,%d,%d)\n", hWnd, rect.left, rect.top, rect.right, rect.bottom);

	return TRUE;
}



// MainFrame より呼ばれる
void CDBDinpView::OnActivateJob(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
//	MyTrace("%s nState = %d PROGRAM_OK(%d)\n", __FUNCTION__, nState, PROGRAM_OK);

	if( ! PROGRAM_OK )	return;

	if( nState == WA_INACTIVE ) {
		m_DtLine.HideTitleTipWindow();
		m_DtList.HideTitleTipWindow();
		GetDtListItemWidth();

		if( get_nowln() == APPEND_LINE ) {
		//	::EnumChildWindows(this->m_hWnd, EnumChildProc, 0);
			CRect rect;
			m_Input.GetWindowRect(&rect);

			if( pWndOther ) {
			//	MyTrace(">>>>CDBDinpView::OnActivateJob pWndOther = %08x\n", pWndOther->m_hWnd);
				CWnd* pwnd = CWnd::FromHandle(pWndOther->m_hWnd);
				CRect rc2;
				pwnd->GetWindowRect(&rc2);

			//	MyTrace(">>>>CDBDinpView::OnActivateJob rect(%d,%d,%d,%d), rc2 = (%d,%d,%d,%d)\n", rect.left, rect.top, rect.right, rect.bottom, rc2.left, rc2.top, rc2.right, rc2.bottom);

				//新規入力の DBEdit に隣接しているか？
				if( rect.left == rc2.left && (rect.bottom == rc2.top ||rect.top == rc2.bottom)) {
					m_CalcFlg = TRUE;	//電卓ウィンドウサインON
				}
			}
		}
		else {
			//消費税ダイアログに月指定が重なって描画が残るので月指定けす。
			if( get_nowpn() == SKBN_PN && m_pSyzInp->IsDisplay() ) {
				m_selDate.ShowWindow(SW_HIDE);
			}
		}
	}
	else {
		if( m_CalcFlg ) {
			m_CalcOFF = GetTickCount();
			m_CalcFlg = FALSE;
		}
	}
}

void CDBDinpView::OnButtonEnd()
{
	//この関数は、キーボードからEndキーを押した時と
	//ツールバーのEndボタンを押した時に呼び出されます。

//	m_DtList.HideTitleTipWindow();
	int ret;

	try
	{
		switch( INP_mode )
		{
		case _MODEFY:
			// 確定マスター
			if( IsMAsterKAKUTEI() )	goto CLOSE_TAG;

			if( m_pSyzInp->IsDisplay() )
				SendMessage( WM_SKBN_INEND, VK_END, 0 );
			else
			{
				TermDistribute( VK_END );
			}
			return;

		case _KEYSCAN:
			if( m_pSyzInp->IsDisplay() )
				SendMessage( WM_SKBN_INEND, VK_END, 0 );
			else
			{
				CalcMouseEvent();
				TermDistribute( 0 );
			}
			//別画面入力消去
			Destroy_ModeLessDlg();

			SelWndShow( FALSE );

			if( DATA_1RD )
			{
				//データの戻し
				m_pScanView->scan_zrec_set( &CREC, SCROLL_NL() - get_nowln() );
				//CREC to 残高ADD.
				ret = rewupdat( &CREC, &SCAN_rec, INP_mode );

				if( rewupdat_job( ret ) ) {
					return;
				}
				DATA_1RD = 0;
			}
#ifdef _SLIP_ACCOUNTS_
			HideImgDlg();
#endif
			// 02.15 /13
			if( IsSCAN_TKYONLY() ) {
				VARIANT var;
				var.pbVal = NULL;
				int pn = get_nowpn();
				exDBdata_set( pn, getINP_CTL(pn)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				EDIT_PN = -1;
			}
			((CMainFrame*)GetDBDinpMainFrame())->ChgToScanView( 1, SCROLL_NL() - get_nowln(), 0 );
			return;

			break;

		case _APPEND:

			if( m_InsLine != -1 ) {
				int svInsLine = m_InsLine;
				// 新規入力行のデータ・seq更新
				SaveDinpPara( INP_mode );
				SaveDinpData( _APPEND );
				SaveDinpData_SEQ( _APPEND, pDBzm->m_lastdata.m_seq );

				m_InsLine = -1;

				if( m_InsMode == _MODEFY ) {
					ins_back_modefy();
					insert_clear();
				}
				else if( m_InsMode == _KEYSCAN ) {
					INP_mode = _KEYSCAN;
					//別画面入力消去
					Destroy_ModeLessDlg();

#ifdef _SLIP_ACCOUNTS_
					HideImgDlg();
#endif
				//	((CMainFrame*)GetDBDinpMainFrame())->ChgToScanView( 1, svInsLine, 0 );
					//検索項目入力
				//	gotoscankey( /*TRUE*/ );
					ins_back_keyscan();
				}
				return;
			}
			break;

		case _RETREV:			//検索項目入力は
			ASSERT( FALSE );
		}

CLOSE_TAG:

		((CMainFrame*)GetDBDinpMainFrame())->PostMessage( WM_CLOSE );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


//タブクリックで変更された場合の情報保存
void CDBDinpView::tabchg_saveinfo( BOOL bZanUp )
{
	if( bZanUp ) {
		if( DATA_1RD )
		{
			//データの戻し
			if( INP_mode == _KEYSCAN )
				m_pScanView->scan_zrec_set( &CREC, SCROLL_NL() - get_nowln() );

			//摘要欄のデータ取得 04.10 /20
			if (get_nowpn() == TKY_PN)
				TerminationIcsinputTky(this, get_nowln(), 0, 0, 0);

			//CREC to 残高ADD.
			rewupdat( &CREC, &SCAN_rec, INP_mode, FALSE, FALSE );
			DATA_1RD = 0;
		}
		else {
			//摘要欄のデータ取得 04.10 /20
			if (INP_mode == _APPEND) {
				if (get_nowpn() == TKY_PN)
					TerminationIcsinputTky(this, get_nowln(), 0, 0, 0);
			}
		}
	}

	//消費税入力消去
	Destroy_ModeLessDlg();

	m_DtList.HideTitleTipWindow();

	//入力または行戻りの修正はモードをセーブしておく
    if( INP_mode == _APPEND || INP_mode == _MODEFY )
	{
		_save_inpcondition();
	}

	SaveDinpPara( INP_mode );	//選択科目等表示属性セーブ

	c_set( IDC_ICSDISPCTRL10 );

	SAVE_PN = -1;
}

//-------------------------------------------------------------
//	現在のモード等 情報を保存
//
//-------------------------------------------------------------
void CDBDinpView::_save_inpcondition()
{
	SV_INP_mode = INP_mode;		//現在モードセーブ

	int zrec_idx;
	zrec_idx = DDATA_LINE[ SCROLL_NL()-1 ].ZREC_NO - 1;

	// 取消仕訳の復帰があるかもしれないので、底データのSEQ ではなく、開始時の、SEQ をセットする
//	if( zrec_idx >= 0 )
//		SV_BOTOM_SEQ_NO = ZREC[ zrec_idx ].m_seq; //スクロール行の底データのＳＥＱ番号	
//	else
		SV_BOTOM_SEQ_NO = START_SEQ;

	SaveDinpData( INP_mode );	//データおよびコントロールテーブルセーブ

	// 入力タイプセーブ 
	SaveInpType( INP_mode, get_nowln() );
}


//検索項目入力
void CDBDinpView::to_scankey()
{
	gotoscankey();
}

//スキャンモードから入力・行戻り修正モードへ
void CDBDinpView::to_inpmode()
{
	//data display initialize
	endkeyscan();
}


// 検索終了
void CDBDinpView::endkeyscan()
{
FILETrace( "@Dinp::endkeyscan TOP CREC.m_seq = %d, m_ddate = %d\n", CREC.m_seq, CREC.m_ddate );

int save_brn_sel;
struct _TKYBRN_SEL save_sel;

	//ソートクリア
	SwkListSortClear();

	InitScanPgTBL( &SCAN_PGTBL );

	bIKAT_SCAN = FALSE;		//  一括修正スキャン中サインの初期化
	//	摘要のみ修正
	((CMainFrame*)GetDBDinpMainFrame())->IsTkyOnly() = FALSE;	// サインの初期化

	// 他の入力で 仕訳が 追加されているか？
	int btm_seq = APPEND_SEQ.ngetseq( SEQ_NO - 1);
	int add_sgn, mode, n;
	add_sgn = 0;
	mode = SV_INP_mode;
	if( btm_seq > 0 ) {
		if( btm_seq > SV_BOTOM_SEQ_NO )
			add_sgn = 1;
	}
FILETrace("@Dinp::endkeyscan(1) btm_seq = %d, add_sgn = %d, SV_BOTOM_SEQ_NO = %d\n", btm_seq, add_sgn, SV_BOTOM_SEQ_NO);

		// 追加仕訳なし、前のモードへ
		inpmdisp( SV_INP_mode );	//モードリセット
#ifdef OLD_CLOSE // 10.11 /12
		SetDinpData( INP_mode );	//データおよびコントロールテーブルリセット
		ReSetDinpPara( INP_mode );	//選択科目等表示属性リセット
#endif
		ReSetDinpPara( INP_mode );	//選択科目等表示属性リセット
		SetDinpData( INP_mode );	//データおよびコントロールテーブルリセット

		btm_seq = SV_BOTOM_SEQ_NO;
		if( USERS_SEQ < btm_seq )	btm_seq = USERS_SEQ;

		if( IsMAsterKAKUTEI() ) {
			CREC.m_seq = btm_seq;
		}
//	}

FILETrace( "@Dinp::endkeyscan CREC.m_seq = %d, m_ddate = %d, mode = %04x\n", CREC.m_seq, CREC.m_ddate, INP_mode );
FILETrace( "@Dinp::endkeyscan(2) btm_seq = %d, \n", btm_seq );

	// 摘要枝番関係セーブ
	save_brn_sel = BRNTKY_sel;
	save_sel = TKYBRN_SEL;

	//科目・摘要選択画面リセット
	SelDspRst( INP_mode );

	// 枝番摘要画面リセット
//	BRNTKY_sel_set( save_brn_sel );
	TKYBRN_SEL = save_sel;
	brntky_rst();

	// 追加入力用に画面変更
	if( !IsMAsterKAKUTEI() ) {
		ScanMode_WndMove( FALSE );
	}
	else {
		ScanMode_WndMove( TRUE );
	}

	//スクロールデータリセット
	for( int i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
		ZREC[i].Reset();
	}

//FILETrace( "@Dinp::endkeyscan (1)\n" );

	CDBINPDataRec key;
	key.m_seq = btm_seq;

	if( INP_mode == _MODEFY ) {
		key.m_seq = CREC.m_seq;
		//_KEYSCAN時に1行 → 2行に変更した場合。
		int lnchk;
		lnchk = get_nowln();
		if( lnchk > SCROLL_NL()	) {
			lnchk = SCROLL_NL();
			set_nowln( lnchk );
		}
		int seq = key.m_seq;
		if( IsMAsterKAKUTEI() )	seq = 0;

		n = endkeyscan_getn(&ZREC[0], &key, SCROLL_NL(), lnchk, seq );
	}
	else {
	//	n = scandat_getn( &ZREC[0], &key, SCROLL_NL(), CREC.m_seq );
		n = scaninsdat_getn( &ZREC[0], -1, SCROLL_NL(), CREC.m_seq );
	}

//FILETrace( "@Dinp::endkeyscan (2)\n" );

	scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );
	D_SEQN = SCROLL_NL()-1;
	ddata_dsp(-1);

	if( (((CMainFrame*)GetDBDinpMainFrame())->IsKeiriJzEBEnv() || IsBuyScannerSave() ) 
		&& m_bSLACC_BUTTON_DISP && !IsJzSansyo() && !FromReadOCR() ) {
		m_GnpScanBtn.EnableWindow(TRUE);
		m_GnpScanBtn.ShowWindow(SW_SHOW);
	}
//FILETrace( "@Dinp::endkeyscan (3)\n" );

	// 検索項目入力へ、
	if( n <= 0 ) {
		if( bSCAN_ONLY_MODE || bCONFIRM_MASTER ) {
			CString msg, str;
			if( bCONFIRM_MASTER ) {
				GetConfirmString(str);
				str += "\r\n\r\n";
			}
			else	str.Format( "『検索・修正モード』\r\n\r\n" );

			msg.Format( "%s現ユーザで入力された仕訳がありません！\r\n検索項目画面に移動します。", str );
			myICSMessageBox( msg );

			((CMainFrame*)GetDBDinpMainFrame())->PostMessage( WM_BUNKI, 0, 0 );
			return;
		}
	}

//FILETrace( "@Dinp::endkeyscan (4)\n" );

	//入力行再表示
	if( !IsMAsterKAKUTEI() ) {
		INP_LINE._XXDTA.dd_seq = REAL_SEQ_NO + 1;	// 他で追加があるため SEQ更新
		if( add_sgn ) {
			CREC.m_seq = INP_LINE._XXDTA.dd_seq;
		}
		inpline_dsp( &INP_LINE, APPEND_LINE);
	}
	// 現在行 取得
	int line = get_nowln();
	if( line != APPEND_LINE ) {
		// 取消復帰で、カーソル行の CREC,SREC がずれる 場合の対応
		int zno = _getLINE_DATA( line )->ZREC_NO;
		if( zno > 0 ) {
			if( SREC.m_seq != ZREC[ zno-1 ].m_seq ) {
				SREC = ZREC[ zno-1 ];
			}
		}
	}

	//直前のサンプル仕訳 SREC の修正	//04.07 /98
	set_SREC( &SREC, &ZREC[0], DDATA_LINE, SCROLL_NL(), getLINE_DATA(), INP_mode );
#ifdef IMPORT	// 10.14 /98
	if( NXTDTA_sg )
		set_SREC( &NXT_rec2, &ZREC[0], DDATA_LINE, SCROLL_NL(), getLINE_DATA(), INP_mode );
#endif


	if( INP_mode != _APPEND )
	{
		CREC = SREC;		//修正行のリセット
		SCAN_rec = SREC;	//修正元仕訳リセット
		DATA_1RD = TRUE;
	}
	else {
		DATA_1RD = FALSE;

		if( SREC.m_dbmn == -1 ) {
			if( m_sdbtBmn != -1 ) {
				SREC.m_dbmn = m_sdbtBmn;
			}
		}
		if( SREC.m_cbmn == -1 ) {
			if( m_screBmn != -1 ) {
				SREC.m_cbmn = m_screBmn;
			}
		}
	}
//FILETrace( "@Dinp::endkeyscan CREC.m_seq = %d, m_ddate = %d, dbt'%s', cre'%s'\n", CREC.m_seq, CREC.m_ddate, CREC.m_dbt, CREC.m_cre );
//FILETrace( "@Dinp::endkeyscan ---> DATA_1RD = %d\n", DATA_1RD );

	//消費税
//	pDBsy->Sy_iniz( &CREC, INP_mode == _APPEND ? 0 : 1 );
	DB_SyIniz( &CREC );

	// 諸口・枝番 再表示
	syogdsp( (char *)SG_def );
	genkdsp( (char *)GK_def );

//	m_selDate.Iniz( pDBzm, ICS_DATESELECT_RANGE_PERIOD|ICS_DATESELECT_EDIT_OFF|ICS_DATESELECT_INPOK_ONLY, 0, 0 );

	int sofs, eofs, sdate, edate;
	pDBzm->get_datelimit( sofs, eofs, sdate, edate );
	m_selDate.SetMonthOffset( sofs, eofs, sdate, edate );

	OnDateSelectMessageChangeMNTHSEL(0,0);

	int set_pn;
	UpdateDataLineDisp( &set_pn );
	if( set_pn == -1 )	set_pn = get_nowpn();
	if( set_pn == -1 )	set_pn = DATE_PN;

	PostMessage( WM_RETROFOCUS, set_pn );

	// 入力タイプリセット 
	SetInpType( INP_mode );

	RDRemainDispJOB();

	this->UpdateWindow();

FILETrace( "@Dinp::endkeyscan END CREC.m_seq = %d, m_ddate = %d, dbt = %s, cre = %s\n", CREC.m_seq, CREC.m_ddate, CREC.m_dbt, CREC.m_cre );

//FILETrace( "@Dinp::endkeyscan END\n" );
}


// ビュー直接切り替え時など 新規入力モードに戻す
void CDBDinpView::to_appendmode()
{	
	int save_brn_sel;
	struct _TKYBRN_SEL save_sel;
	int i, n;

//FILETrace( ">>>>>>>>>>>>CDBDinpView::to_appendmode() TOP\n" );
//FILETrace( "  check CREC seq = %d", CREC.m_seq );

	//ソートクリア
	SwkListSortClear();
	BOOL bWndMove = FALSE;
	if( m_reCoDispType != 0 )	bWndMove = TRUE;

	m_reCoDispType = 0;

	if( DATA_1RD )
	{
	//	memmove( (char *)&ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1], (char *)&CREC, sizeof( struct _DATAREC ) );
	//	ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1] = CREC;
		CRECtoZREC();
		//CREC to 残高ADD.
		rewupdat( &CREC, &SCAN_rec, INP_mode, FALSE, FALSE );
		DATA_1RD = 0;
	}

	if( INP_mode == _KEYSCAN && !IsMAsterKAKUTEI() ) {
		// 追加入力用に画面変更
		ScanMode_WndMove( FALSE );
	}
	else {
		if( IsMAsterKAKUTEI() )	ScanMode_WndMove( TRUE );
		else {
			if( bWndMove ) {
				ScanMode_WndMove( FALSE );
			}
		}
	}

	// 確定マスターは、[修正]のみ
	int mode;
	mode = IsMAsterKAKUTEI() ? _MODEFY : _APPEND;

	SetDinpDataLite( SV_INP_mode );
	inpmdisp( mode );
	// Dinp Para _APPEND mode リセット
	ReSetDinpPara( mode );

//FILETrace( "  check[ReSetDinpPara] CREC seq = %d", CREC.m_seq );

	ONSIZETrace("to_appendmode SCROLL_NL = %d\n", SCROLL_NL());

	// 摘要枝番関係セーブ
	save_brn_sel = BRNTKY_sel;
	save_sel = TKYBRN_SEL;

	//ZREC ReSet
	for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
		ZREC[i].Reset();
	}

	//仕訳表示用
	pDBzm->dbdata_Requery() = TRUE;

	CDBINPDataRec key;

	key.m_seq = APPEND_SEQ.ngetseq( SEQ_NO - 1);
	if( key.m_seq <= 0 ) key.m_seq = USERS_SEQ;
//	n = scandat_getn( &ZREC[0], &key, SCROLL_NL() );
	n = scaninsdat_getn( &ZREC[0], -1, SCROLL_NL() );

	scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );
	D_SEQN = SCROLL_NL()-1;

	// 検索項目入力へ、
	if( n <= 0 ) {
		if( bSCAN_ONLY_MODE || bCONFIRM_MASTER ) {
			CString msg, str;
			if( bCONFIRM_MASTER ) {
				GetConfirmString(str);
				str += "\r\n\r\n";
			}
			else	str.Format( "『検索・修正モード』\r\n\r\n" );

			msg.Format( "%s現ユーザで入力された仕訳がありません！\r\n検索項目画面に移動します。", str );
			myICSMessageBox( msg );

			((CMainFrame*)GetDBDinpMainFrame())->PostMessage( WM_BUNKI, 0, 0 );
			return;
		}
	}

	if( !IsMAsterKAKUTEI() ) {
		set_nowln( APPEND_LINE );
		ddata_dsp(-1);	

		//入力行再表示
		INP_LINE._XXDTA.dd_seq = REAL_SEQ_NO + 1;	// 他で追加があるため SEQ更新
		CREC.m_seq = INP_LINE._XXDTA.dd_seq;
		inpline_dsp( &INP_LINE, APPEND_LINE);
	}
	else {
		set_nowln( SCROLL_NL() );
		ddata_dsp(-1);

		if( get_nowln() == -1 ) {
			set_nowln(SCROLL_NL());
		}
	}
	if( (((CMainFrame*)GetDBDinpMainFrame())->IsKeiriJzEBEnv() || IsBuyScannerSave() ) 
		&& m_bSLACC_BUTTON_DISP && !IsJzSansyo() && !FromReadOCR() ) {
		m_GnpScanBtn.EnableWindow(TRUE);
		m_GnpScanBtn.ShowWindow(SW_SHOW);
	}

	//消費税
//	pDBsy->Sy_iniz( &CREC, 0 );
	DB_SyIniz( &CREC );

	//科目・摘要選択画面リセット
	//#* 03.06 /02 ... 修正行の摘要をそのまま表示していた ==========
	SelDspParaRst();  
	// 表示している反対側の摘要のデータを再セット
	//===============================================================
	SelDspRst( INP_mode );

	// 枝番摘要画面リセット
//	BRNTKY_sel_set( save_brn_sel );
	TKYBRN_SEL = save_sel;
	brntky_rst();

	//直前のサンプル仕訳 SREC の修正	//04.07 /98
	set_SREC( &SREC, &ZREC[0], DDATA_LINE, SCROLL_NL(), getLINE_DATA(), INP_mode );
#ifdef IMPORT	// 10.14 /98
	if( NXTDTA_sg )
		set_SREC( &NXT_rec2, &ZREC[0], DDATA_LINE, SCROLL_NL(), getLINE_DATA(), INP_mode );
#endif

	if( SREC.m_dbmn == -1 ) {
		if( m_sdbtBmn != -1 ) {
			SREC.m_dbmn = m_sdbtBmn;
		}
	}
	if( SREC.m_cbmn == -1 ) {
		if( m_screBmn != -1 ) {
			SREC.m_cbmn = m_screBmn;
		}
	}

	/* 諸口合計	*/
	g_stotal( SYOGT, (char *)SYOG_d, (char *)SYOG_c, (char *)SG_def );
	syogdsp( (char *)SG_def );

	/* 現金合計	*/
	g_stotal( /*GNKIN*/pAUTOSEL->DCKM_CODE, (char *)GENK_d, (char *)GENK_c, (char *)GK_def );
	genkdsp( (char *)GK_def );

//	m_selDate.Iniz( pDBzm, ICS_DATESELECT_RANGE_PERIOD|ICS_DATESELECT_EDIT_OFF|ICS_DATESELECT_INPOK_ONLY, 0, 0 );
	int sofs, eofs, sdate, edate;
	pDBzm->get_datelimit( sofs, eofs, sdate, edate );
	m_selDate.SetMonthOffset( sofs, eofs, sdate, edate );

	OnDateSelectMessageChangeMNTHSEL(0,0);

	int set_pn;
	UpdateDataLineDisp( &set_pn );
	if( set_pn == -1 )	set_pn = get_nowpn();
	if( set_pn == -1 )	set_pn = DATE_PN;

	set_focus( set_pn );
}


// 会社再選択時の画面処理等
void CDBDinpView::Recosel_Job()
{
	if( INP_mode == _KEYSCAN ) {
		// 追加入力用に画面変更
		ScanMode_WndMove( FALSE );
	}

	// Dinp Para リセット
//	ReSetDinpPara( _APPEND );
//	ReSetDinpPara( _RETREV );
//	ReSetDinpPara( _KEYSCAN );
//	ReSetDinpPara( _MODEFY );

	InizDinpPara( _APPEND );
	InizDinpPara( _RETREV );
	InizDinpPara( _KEYSCAN );
	InizDinpPara( _MODEFY );

	// 仕訳選択状態をクリア
	click_seljob( -1, 0 );

	//会社選択時の画面を保存
	m_reCoDispType = ((CMainFrame*)GetDBDinpMainFrame())->NowView();

	DATA_1RD = 0;
//FILETrace( "@Recosel_Job End DATA1_READ = %d\n", DATA_1RD );

}



void CDBDinpView::OnUpdateButtonF3(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	if( PROGRAM_OK ) {
		if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() == INP_VIEW && INP_mode != _RETREV )
		{
#ifndef _SLIP_ACCOUNTS_
			pCmdUI->Enable( TRUE );
			return;
#else
			if( get_nowpn() == TKY_PN ){
				pCmdUI->Enable( TRUE );
				return;
			}
			else if( IsDispSlip() == FALSE && m_InsLine == -1 ){
				pCmdUI->Enable( TRUE );
				return;
			}
#endif
		}
	}

	pCmdUI->Enable( FALSE );
}


// F3 摘要欄の場合は、借方枝番消去
//
void CDBDinpView::OnButtonF3() 
{
	if( !PROGRAM_OK )
		return;

	if( get_nowpn() == TKY_PN )
			remove_edabandsp( &CREC, DEBT_PN );
	else {
		// 電卓ウィンドウ消去
		CalcMouseEvent();
		dtlist_dispopt();
	}
}


// 仕訳表示行の設定
void CDBDinpView::dtlist_dispopt()
{
	CDispOptionDlg dlg;


	BlockInfo	blk;
	blk.mode	= 0;

	// 新規入力行 なし
	if( IsMAsterKAKUTEI() )			blk.mode = 1;
	else {
		if( INP_mode == _KEYSCAN )	blk.mode = 1;
	}

	if( ! blk.mode ) {
		dlg.m_bEnaMove = TRUE;
		DispBmpMake(blk.mode);
	}
	else {
		dlg.m_bEnaMove = FALSE;
	}

	CString	str[5];
	str[0] = "科目等選択欄エリア";
	str[1] = "仕訳表示リストエリア";
	str[2] = "入力タイプ表示エリア";
	str[3] = "新規仕訳入力行エリア";
	str[4] = "現金貸借表示エリア";
	blk.pBlkStr = str;

	blk.blkCnt	= 5;
	int blkHt[10] = {162,275,33,110,33};

	if( blk.mode ) {
		CRect rcTmp[10];
		for( int i = 0; i < blk.blkCnt; i++ ) {
			if( i == 0 ) {
				rcTmp[i].top	= 0;
				rcTmp[i].bottom = blkHt[i];
			}
			else {
				rcTmp[i].top	= rcTmp[i-1].bottom;
				rcTmp[i].bottom = rcTmp[i].top + blkHt[i];
			}
		}

		blk.pRect	= rcTmp;
	}
	else {
		blk.pRect	= m_blkRect;
	}

	blk.pBmp	= &m_ScrnBmp;
	blk.pBlkIdx	= m_blkIdx;
	blk.selDisp = (pAUTOSEL->BLKSEL_OPT == OPT_NONOPT) ? TRUE : FALSE;
	blk.bBmpMake	= m_bBmpMake;

	dlg.SetBlockInfo( &blk );

//	DWORD st, stchg;
//	BOOL	bOldUseDefColor;
	short line, layout_chg = 0, stchg = 0;
	int	chk_swk_radio, chk_lh_radio, ret;
	int focus_sgn = 0;
	short bKeepSgn[10] = {0};

//	st = m_DtList.GetStyle();
	line = pAUTOSEL->DSPLINE_OPT;

	dlg.m_Sw1 = (line==1) ? 0 : 1;
	chk_swk_radio = dlg.m_Sw1;

/*
	dlg.m_DenpYes	= (st & DS_DENP_USE) ? 0 : 1;
	dlg.m_BmnYes	= (st & DS_BMON_USE) ? 0 : 1;
	dlg.m_KojiYes	= (st & DS_KOJI_USE) ? 0 : 1;
	dlg.m_SyoYes	= (st & DS_SYONO_USE) ? 0 : 1;
	dlg.m_TgdtYes	= (st & DS_TGDT_USE) ? 0 : 1;
	dlg.m_LnHt1		= (m_DtList.GetLineHeight() == -1) ? 0 : 1;
*/
	dlg.m_DenpYes	= (pAUTOSEL->DSPDENP_OPT == OPT_NONOPT) ? 0 : 1;
	dlg.m_BmnYes	= (pAUTOSEL->DSPBMON_OPT == OPT_NONOPT) ? 0 : 1;
	dlg.m_KojiYes	= (pAUTOSEL->DSPKOJI_OPT == OPT_NONOPT) ? 0 : 1;
	dlg.m_SyoYes	= (pAUTOSEL->DSPTGNO_OPT == OPT_NONOPT) ? 0 : 1;
	dlg.m_TgdtYes	= (pAUTOSEL->DSPTGDT_OPT == OPT_NONOPT) ? 0 : 1;
//	if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ )	dlg.m_Hjiyu		= 1;
	if( IsHjiyuItemGyosyu() == 0 )		dlg.m_Hjiyu = 1;
	else {
		dlg.m_Hjiyu		= (pAUTOSEL->DSPHJIYU_OPT == OPT_NONOPT) ? 0 : 1;
	}
	dlg.m_Zei		= (pAUTOSEL->DSPZEI_OPT == OPT_NONOPT) ? 0 : 1;
	dlg.m_Syz		= (pAUTOSEL->DSPSYZ_OPT == OPT_NONOPT) ? 0 : 1;

	bKeepSgn[0] = dlg.m_DenpYes;
	bKeepSgn[1] = dlg.m_BmnYes;
	bKeepSgn[2] = dlg.m_KojiYes;
	bKeepSgn[3] = dlg.m_SyoYes;
	bKeepSgn[4] = dlg.m_TgdtYes;
	bKeepSgn[5] = dlg.m_Hjiyu;
	bKeepSgn[6] = dlg.m_Zei;
	bKeepSgn[7] = dlg.m_Syz;

	dlg.m_LnHt1		= (pAUTOSEL->DSPLNWD_OPT == OPT_NARROW) ? 0 : 1;	
	chk_lh_radio = dlg.m_LnHt1;
#ifdef TEST_CUT
	dlg.m_ChkDfCol	= m_DtList.GetUseDefaultColor();
	bOldUseDefColor = dlg.m_ChkDfCol;
#endif

PROGRAM_OK = FALSE;

	ret = dlg.DoModal();

PROGRAM_OK = TRUE;
	
	if( ret == IDOK ) {
		dlg.GetBlockIdx( m_blkIdx );

		if( dlg.GetSelDispShow() ) {
			pAUTOSEL->BLKSEL_OPT = OPT_NONOPT;
		}
		else {
			pAUTOSEL->BLKSEL_OPT = 1;
		}

		BOOL bLcntChg = FALSE;
		if( blk.selDisp ) {	//選択欄をビューに表示していた。
			if( pAUTOSEL->BLKSEL_OPT != OPT_NONOPT ) {
				bLcntChg = TRUE;
			}
		}
		else {
			if( pAUTOSEL->BLKSEL_OPT == OPT_NONOPT ) {
				//選択欄をビューに表示していなかったのを再表示した。
				SelWndShow( FALSE);
				bLcntChg = TRUE;
			}
		}
		int old_lncnt = m_DtList.GetDispDataCount();

		BOOL bKeyScan = FALSE;
		if( IsMAsterKAKUTEI() )	bKeyScan = TRUE;
		else {
			if( INP_mode == _KEYSCAN )	bKeyScan = TRUE;
		}

		BlkWndMove();
		SlipWndMove(bKeyScan);

		RDRemainDispJOB();

		pAUTOSEL->BLK_OPT[0] = 	m_blkIdx[0];
		pAUTOSEL->BLK_OPT[1] = 	m_blkIdx[1];
		pAUTOSEL->BLK_OPT[2] = 	m_blkIdx[2];
		pAUTOSEL->BLK_OPT[3] = 	m_blkIdx[3];
		pAUTOSEL->BLK_OPT[4] = 	m_blkIdx[4];

		if( ! dlg.m_DenpYes ) {
			pAUTOSEL->DSPDENP_OPT = OPT_NONOPT;
		}
		else {					
			pAUTOSEL->DSPDENP_OPT = OPT_NODSP;
		}
		if( bKeepSgn[0] != dlg.m_DenpYes )	stchg++;

		if( ! dlg.m_BmnYes ) {
			pAUTOSEL->DSPBMON_OPT = OPT_NONOPT;
		}
		else {
			pAUTOSEL->DSPBMON_OPT = OPT_NODSP;
		}
		if( bKeepSgn[1] != dlg.m_BmnYes )	stchg++;
	
		if( Voln1->apno == 0x20 ) {
			if( ! dlg.m_KojiYes ) {
				pAUTOSEL->DSPKOJI_OPT = OPT_NONOPT;
			}
			else {
				pAUTOSEL->DSPKOJI_OPT = OPT_NODSP;
			}
		}
		else {
			pAUTOSEL->DSPKOJI_OPT = OPT_NODSP;
		}
		if( bKeepSgn[2] != dlg.m_KojiYes )	stchg++;
		
		if( ! dlg.m_SyoYes ) {
			pAUTOSEL->DSPTGNO_OPT = OPT_NONOPT;
		}
		else {
			pAUTOSEL->DSPTGNO_OPT = OPT_NODSP;
		}
		if( bKeepSgn[3] != dlg.m_SyoYes )	stchg++;

		if( ! dlg.m_TgdtYes ) {
			pAUTOSEL->DSPTGDT_OPT = OPT_NONOPT;
		}
		else {
			pAUTOSEL->DSPTGDT_OPT = OPT_NODSP;
		}
		if( bKeepSgn[4] != dlg.m_TgdtYes )	stchg++;

		// 変動事由
		if( ! dlg.m_Hjiyu ) {
			pAUTOSEL->DSPHJIYU_OPT = OPT_NONOPT;
		}
		else {
			pAUTOSEL->DSPHJIYU_OPT = OPT_NODSP;
		}
		if( bKeepSgn[5] != dlg.m_Hjiyu  )	stchg++;

		// 税額		09.20 /07
		if( ! dlg.m_Zei ) {
			pAUTOSEL->DSPZEI_OPT = OPT_NONOPT;
		}
		else {
			pAUTOSEL->DSPZEI_OPT = OPT_NODSP;
		}
		if( bKeepSgn[6] != dlg.m_Zei  ) {
			stchg++;
		}
		// 消費税	11.05 /20
		if (!dlg.m_Syz) {
			pAUTOSEL->DSPSYZ_OPT = OPT_NONOPT;
		}
		else {
			pAUTOSEL->DSPSYZ_OPT = OPT_NODSP;
		}
		if (bKeepSgn[7] != dlg.m_Syz) {
			stchg++;
		}
		if( stchg ) {
//			m_DtList.SetStyle( stchg );
			SetListColumn( ! dlg.m_Sw1 ? 1 : 2 );
			SetDtListItemWidth();
			layout_chg = 1;
		}

		if( chk_swk_radio != dlg.m_Sw1 || chk_lh_radio != dlg.m_LnHt1 || bLcntChg ) {
			pAUTOSEL->DSPLINE_OPT = ! dlg.m_Sw1 ? 1 : 2;
			pAUTOSEL->DSPLNWD_OPT = ! dlg.m_LnHt1 ? OPT_NARROW : OPT_WIDE;

//			int old_lncnt = m_DtList.GetDispDataCount();
			if( ! dlg.m_Sw1 ) {
				m_DtList.SetLineHeight( !dlg.m_LnHt1 ? -1 : 0 );
			}
//			m_DtList.SetLines( dlg.m_Sw1 ? 2 : 1 );		// この中で表示項目変更も行っています。

			if( ! layout_chg ) {
				SetListColumn( ! dlg.m_Sw1 ? 1 : 2 );
				SetDtListItemWidth();
			}
			m_DtList.EnforceLayout(); // 項目変更の矩形をセット

			int now_lncnt = m_DtList.GetDispDataCount();
			if( now_lncnt > DDATA_LNMAX )	now_lncnt = DDATA_LNMAX;

			if( INP_mode != _KEYSCAN ) {
				if( swkline_dispchg( old_lncnt, now_lncnt, &focus_sgn ) != 0 ) {
					ErrExit( 0, "仕訳表示行の変更ができませんでした。" );
				}
			}
			else {
				if( swkline_keyscan_dispchg( old_lncnt, now_lncnt, &focus_sgn ) != 0 ) {
					ErrExit( 0, "仕訳表示行の変更ができませんでした。" );
				}
			}
		}

		if( bLcntChg ) {
			m_SelDispPN = -1;
			disp_change( -1, get_nowpn() );
		}

		if( layout_chg )	m_DtList.EnforceLayout(); // 項目変更の矩形をセット

		if( IMG_master != ERR ) { //OCR
			big_img(getLINE_DATA(), getINP_CTL( TKY_PN ));
		}

		//税額欄 表示変更
		if( bKeepSgn[6] != dlg.m_Zei || bKeepSgn[7] != dlg.m_Syz) {
			m_DtLine.EnforceLayoutEX( FALSE );
			AutoSelIndicator( pAUTOSEL, INP_mode );
		}

		//ソートサイン
		SwkListSortSign( m_sortID, m_sortType );

		m_selDate.UpdateWindow();
		m_DtLine.RedrawDataListCtrl();

#ifdef TEST_CUT
		if( dlg.m_ChkDfCol != bOldUseDefColor ) {
			m_DtList.SetUseDefaultColor(dlg.m_ChkDfCol);
			m_DtLine.SetUseDefaultColor(dlg.m_ChkDfCol);
			m_DtLine.RedrawDataListCtrl();
			m_pScanView->SetScanListUseDefColor( dlg.m_ChkDfCol );
		}
#endif
		m_DtList.RedrawDataListCtrl();

//TRACE( "OnbuttonF3 End\n" );
	}

	if( ! focus_sgn ) {
		int set_focus_sgn = 0;
		// 今フォーカスがあったところが 非表示になる場合
		if( stchg ) {
			switch( get_nowpn() ) {
			case DENP_PN:
				if( dlg.m_DenpYes ) {
					set_focus( DATE_PN );
					set_focus_sgn = 1;
				}	
				break;
			case BMON_PN:
			case CBMN_PN:
				if( dlg.m_BmnYes ) {
					set_focus( DATE_PN );
					set_focus_sgn = 1;
				}	
				break;
			case KOJI_PN:
			case CKJI_PN:
				if( dlg.m_KojiYes ) {
					set_focus( DATE_PN );
					set_focus_sgn = 1;
				}	
				break;
			case SYON_PN:
				if( dlg.m_SyoYes ) {
					set_focus( DATE_PN );
					set_focus_sgn = 1;
				}	
				break;
			case TGDT_PN:
				if( dlg.m_TgdtYes ) {
					set_focus( DATE_PN );
					set_focus_sgn = 1;
				}	
				break;
			case HJIYU_PN:
				if( dlg.m_Hjiyu ) {
					set_focus( DATE_PN );
					set_focus_sgn = 1;
				}	
				break;
			// 税額		09.20 /07
			case ZEI_PN:
				if( dlg.m_Zei ) {
					set_focus( DATE_PN );
					set_focus_sgn = 1;
				}	
				break;
			case SKBN_PN:
				if (dlg.m_Syz) {
					set_focus(DATE_PN);
					set_focus_sgn = 1;
				}
				break;
			}
		}

		if( ! set_focus_sgn ) {
			set_focus( get_nowpn() );
		}
	}

}


// 仕訳表示ライン数の変更( スキャン以外 ）
//	int* pFocusSgn		この関数で、フォーカスセットしたか？
//
int CDBDinpView::swkline_dispchg( int oldln, int nowln, int* pFocusSgn )
{
//MyTrace( "@swkline_dispchg top old = %d, now = %d\n", oldln, nowln );
//MyTrace("@DATA_1RD (%d)\n", DATA_1RD);
//MyTrace("L:(%d)  CREC.m_invno = %s\n", __LINE__, CREC.m_invno);

ASSERT( INP_mode != _RETREV );

	int ln, i, setln, index, pn;
	CDBINPDataRec key;
	ln = get_nowln();
	pn = get_nowpn();

	//修正データの戻し
	if( DATA_1RD )
	{
	//	memmove( (char *)&ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1], (char *)&CREC, sizeof( struct _DATAREC ) );
	//	ZREC[DDATA_LINE[ ln - 1 ].ZREC_NO - 1] = CREC;
		CRECtoZREC();
	}
	
	for( i = 0; i < oldln; i++ ) {
		if( DDATA_LINE[i].ZREC_NO )
			break;
	}
	// リストにデータなし
	if( i == oldln ) {
		SCROLL_NL_ = nowln;
		if( !IsMAsterKAKUTEI() )
			SCROLL_NL_ += 1;
		return 0;
	}
	
	if( oldln < nowln || INP_mode == _APPEND ) { // 表示行数が 増えた 

		if( INP_mode != _APPEND ) {	// 修正データリライト
			if( DATA_1RD )
			{
				//CREC to 残高ADD.
				rewupdat( &CREC, &SCAN_rec, INP_mode );
				DATA_1RD = 0;
//				PROGRAM_OK = FALSE;
			}
		}

		// 現在 底に表示しているデータからからセット	
		key.m_seq = DDATA_LINE[oldln-1]._XXDTA.dd_seq;
		key.m_ddate = DDATA_LINE[oldln-1]._XXDTA.dd_ddate;

		//データディスプレイニシャライズ
		for( i = 0 ; i < DDATA_LNMAX; ++i )
		{
			inpline_inz( &DDATA_LINE[i] );
		}
		//データリード＆テーブル作成
	//	_fillbuf( (char *)&ZREC[0], sizeof( ZREC ), 0 );
		for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
			ZREC[i].Reset();
		}
		SCROLL_NL_ = nowln;
		if( !IsMAsterKAKUTEI() )
			SCROLL_NL_ += 1;

//		scandat_getn( &ZREC[0], &key, SCROLL_NL() );
		scaninsdat_getn( &ZREC[0], key.m_seq, SCROLL_NL() );
		scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );

		//データテーブル表示
		_ddata_dsp( -1 );

		setln = -1;
		if( ln != APPEND_LINE ) {	// 今回セットする行
			setln = nowln - oldln;
			setln += ln;
		}
		D_SEQN = SCROLL_NL()-1;

		if( setln > 0 ) {
			set_nowln( setln );
			set_nowpn( pn );
		}

		if( INP_mode != _APPEND ) {
			//修正データ読み出し
			CREC = ZREC[DDATA_LINE[ setln - 1 ].ZREC_NO - 1];
			//残高 SUB. from CREC
			reupdat(&CREC);
			DATA_1RD = 1;
		}
		if( setln > 0 ) {
			_set_focus( setln, SEQ_PN );
			*pFocusSgn = 1;
		}
	}
	else {	 // 表示行数が 減った
ASSERT( nowln >= 4 );
		if( DATA_1RD )
		{
			//CREC to 残高ADD.
			rewupdat( &CREC, &SCAN_rec, INP_mode );
			DATA_1RD = 0;
//				PROGRAM_OK = FALSE;
		}
		// 先頭行の時
		if( ln == 1 || ! DDATA_LINE[ln-2].ZREC_NO ) {
			if( oldln - ln >= nowln ) {		// 底のデータが変わる
				index = (ln+nowln-1)-1;
				ASSERT( index >= 0 );

				key.m_seq = DDATA_LINE[ index ]._XXDTA.dd_seq;
				key.m_ddate = DDATA_LINE[ index ]._XXDTA.dd_ddate;
			//	rno = DDATA_LINE[ index ]._XXDTA.dd_seq;;	
				setln = 1;
			}
			else {			// 今回も前のままで表示できる
				key.m_seq = DDATA_LINE[oldln-1]._XXDTA.dd_seq;
				key.m_ddate = DDATA_LINE[oldln-1]._XXDTA.dd_ddate;
			//	rno = DDATA_LINE[oldln-1]._XXDTA.dd_seq;
				setln = nowln - (oldln - ln);
			}
		}
		else if( ln == oldln ) {// 最下行の時
			// 現在 底に表示しているデータからセット
			key.m_seq = DDATA_LINE[oldln-1]._XXDTA.dd_seq;
			key.m_ddate = DDATA_LINE[oldln-1]._XXDTA.dd_ddate;
		//	rno = DDATA_LINE[oldln-1]._XXDTA.dd_seq;
			setln = nowln;
		}
		else {
			// 現在表示している状態をチェック
			int up, down, dcnt;
			up = down = dcnt = 0;

			for( i = oldln-1; i >= 0; i-- ) {
				if( DDATA_LINE[i].ZREC_NO )
					dcnt++;
				else	break;
			}

			if( dcnt <= nowln ) {
				// 底のデータはそのまま
			//	rno = DDATA_LINE[oldln-1]._XXDTA.dd_seq;
				key.m_seq = DDATA_LINE[oldln-1]._XXDTA.dd_seq;
				key.m_ddate = DDATA_LINE[oldln-1]._XXDTA.dd_ddate;
				setln = nowln - (oldln-ln);
			}
			else {

				for( i = (ln-1); i >= 0; i-- ) {
					if( DDATA_LINE[i].ZREC_NO )
						up++;
					else	break;
				}
				for( i = (ln-1); i < oldln; i++ ) {
					if( DDATA_LINE[i].ZREC_NO )
						down++;
					else	break;
				}
				ASSERT( up && down ); 
				if( ! ( up && down ) )	return -1;

				if( nowln == 1 ) {
					key.m_seq = DDATA_LINE[ln-1]._XXDTA.dd_seq;
					key.m_ddate = DDATA_LINE[ln-1]._XXDTA.dd_ddate;
				//	rno = DDATA_LINE[ln-1]._XXDTA.dd_seq;
					setln = 1;
				}
				if( nowln == 2 || nowln == 3 ) {
					key.m_seq = DDATA_LINE[ln]._XXDTA.dd_seq;
					key.m_ddate = DDATA_LINE[ln]._XXDTA.dd_ddate;
				//	rno = DDATA_LINE[ln]._XXDTA.dd_seq;
					setln = (nowln == 2) ? 1 : 2;
				}
				else {
					if( down > up ) {
						// 現在行を 上から2行目にする
						index = (ln-1)+ (nowln - 2);	// 最終行
						setln = 2;
					}
					else {
						// 現在行を 下から2行目にする
						index = ln;		// 最終行	
						setln = (nowln-1);
					}
					key.m_seq = -1;
					// 最終行データチェック
					for( i = index; i >= 0; i-- ) {
						if( DDATA_LINE[i].ZREC_NO ) {
							key.m_seq = DDATA_LINE[i]._XXDTA.dd_seq;
							key.m_ddate = DDATA_LINE[i]._XXDTA.dd_ddate;
						//	rno = DDATA_LINE[i]._XXDTA.dd_seq;
							break;
						}
					}
					if( -1 == key.m_seq ) return -1;
				}
			}
		}
		//データディスプレイニシャライズ
		for( i = 0 ; i < DDATA_LNMAX; ++i )
		{
			inpline_inz( &DDATA_LINE[i] );
		}
		//データリード＆テーブル作成
	//	_fillbuf( (char *)&ZREC[0], sizeof( ZREC ), 0 );
		for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
			ZREC[i].Reset();
		}
		SCROLL_NL_ = nowln;
		if( !IsMAsterKAKUTEI() )
			SCROLL_NL_ += 1;
	
	//	scandat_getn( &ZREC[0], &key, SCROLL_NL() );
		scaninsdat_getn( &ZREC[0], key.m_seq, SCROLL_NL() );
		scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );

		//データテーブル表示
		_ddata_dsp( -1 );

		D_SEQN = SCROLL_NL()-1;
		set_nowln( setln );
		set_nowpn( pn );

		if( INP_mode != _APPEND ) {
			//修正データ読み出し
			CREC = ZREC[DDATA_LINE[ setln - 1 ].ZREC_NO - 1];
			//残高 SUB. from CREC
			reupdat(&CREC);
			DATA_1RD = 1;
		}
		if( setln > 0 ) {
			_set_focus( setln, SEQ_PN );
			*pFocusSgn = 1;
		}
	}

	return 0;
}


// スキャン中からの行数変更

// 仕訳表示ライン数の変更
int CDBDinpView::swkline_keyscan_dispchg( int oldln, int nowln, int* pFocusSgn )
{
//MyTrace( "@swkline_keyscan_dispchg top" );
ASSERT( INP_mode != _RETREV );

	int ln, i, setln, pn;
	ln = get_nowln();
	pn = get_nowpn();
	setln = -1;

	//修正データの戻し
	if( DATA_1RD )
	{
	//	memmove( (char *)&ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1], (char *)&CREC, sizeof( struct _DATAREC ) );
	//	ZREC[DDATA_LINE[ ln - 1 ].ZREC_NO - 1] = CREC;
		CRECtoZREC();

		if( INP_mode == _KEYSCAN )
		{
			if( m_pScanView->scan_zrec_set( &CREC, oldln - ln ) == -1 )
				return -1;
		}
	}
	
	for( i = 0; i < oldln; i++ ) {
		if( DDATA_LINE[i].ZREC_NO )
			break;
	}
	// リストにデータなし
	if( i == oldln )	return 0;
	
	if( oldln < nowln ) { // 表示行数が 増えた 	

		// 修正データリライト
		if( DATA_1RD )
		{
			//CREC to 残高ADD.
			rewupdat( &CREC, &SCAN_rec, INP_mode );
			DATA_1RD = 0;
//				PROGRAM_OK = FALSE;
		}

		// 現在 底に表示しているデータからセット	

		//データディスプレイニシャライズ
		for( i = 0 ; i < DDATA_LNMAX; ++i )
		{
			inpline_inz( &DDATA_LINE[i] );
		}
		//データリード＆テーブル作成
	//	_fillbuf( (char *)&ZREC[0], sizeof( ZREC ), 0 );
		for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
			ZREC[i].Reset();
		}
		SCROLL_NL_ = nowln;

		m_pScanView->scan_data_rd( &ZREC[ SCROLL_NL() - 1 ], SCROLL_NL(), 0 );
		scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );

		//データテーブル表示
		_ddata_dsp( -1 );

		setln = nowln - oldln;
		setln += ln;

		D_SEQN = SCROLL_NL()-1;

		if( setln > 0 ) {
			set_nowln( setln );
			set_nowpn( pn );
		}

		//修正データ読み出し
		CREC = ZREC[DDATA_LINE[ setln - 1 ].ZREC_NO - 1];
		//残高 SUB. from CREC
		reupdat(&CREC);
		DATA_1RD = 1;

		if( setln > 0 ) {
			_set_focus( setln, SEQ_PN );
			*pFocusSgn = 1;
		}
	}
	else {	 // 表示行数が 減った
ASSERT( nowln >= 4 );
		if( DATA_1RD )
		{
			//CREC to 残高ADD.
			rewupdat( &CREC, &SCAN_rec, INP_mode );
			DATA_1RD = 0;
//			PROGRAM_OK = FALSE;
		}
		int sht;	// SCAN_RD_BOTOM からの オフセット

		// 先頭行
		if( ln == 1 || ! DDATA_LINE[ln-2].ZREC_NO ) {
			
			if( oldln - ln >= nowln ) {		// 底のデータが変わる
				sht = oldln - (ln+nowln-1);
				setln = 1;
			}
			else {			// 今回も前のままで表示できる
				sht = 0;
				setln = nowln - (oldln - ln);
			}
		}
		else if( ln == oldln ) {// 最下行の時
			sht = 0;
			setln = nowln;
		}
		else {
			// 現在表示している状態をチェック
			int up, down, dcnt;
			up = down = dcnt = 0;

			for( i = oldln-1; i >= 0; i-- ) {
				if( DDATA_LINE[i].ZREC_NO )
					dcnt++;
				else	break;
			}

			if( dcnt <= nowln ) {
				// 底のデータはそのまま
				sht = 0;
				setln = nowln - (oldln-ln);
			}
			else {
				// 底のデータが変わる
				for( i = (ln-1); i >= 0; i-- ) {
					if( DDATA_LINE[i].ZREC_NO )
						up++;
					else	break;
				}
				for( i = (ln-1); i < oldln; i++ ) {
					if( DDATA_LINE[i].ZREC_NO )
						down++;
					else	break;
				}
				ASSERT( up && down ); 
				if( ! ( up && down ) )	return -1;

				if( nowln == 1 ) {
					sht = oldln - ln;
					setln = 1;
				}
				if( nowln == 2 || nowln == 3 ) {
					sht = (oldln - ln ) + 1;
					setln = (nowln == 2) ? 1 : 2;
				}
				else {
					if( down > up ) {
						// 現在行を 上から2行目にする
						sht = oldln - ( ln+(nowln-2) );
						setln = 2;
					}
					else {
						// 現在行を 下から2行目にする
						sht = oldln - ( ln+1 );
						setln = (nowln-1);
					}
				}			
			}
		}
		sht *= -1;

		//データディスプレイニシャライズ
		for( i = 0 ; i < DDATA_LNMAX; ++i )
		{
			inpline_inz( &DDATA_LINE[i] );
		}
		//データリード＆テーブル作成
	//	_fillbuf( (char *)&ZREC[0], sizeof( ZREC ), 0 );
		for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
			ZREC[i].Reset();
		}
		SCROLL_NL_ = nowln;

		m_pScanView->scan_data_rd( &ZREC[ SCROLL_NL() - 1 ], SCROLL_NL(), sht );
	
		scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );

		//データテーブル表示
		_ddata_dsp( -1 );

		D_SEQN = SCROLL_NL()-1;
		set_nowln( setln );
		set_nowpn( pn );

		//修正データ読み出し
		CREC = ZREC[DDATA_LINE[ setln - 1 ].ZREC_NO - 1];
		//残高 SUB. from CREC
		reupdat(&CREC);
		DATA_1RD = 1;

		if( setln > 0 ) {
			_set_focus( setln, SEQ_PN );
			*pFocusSgn = 1;
		}
	}

	return 0;
}

//マスターオープン＆リード
void CDBDinpView::StartBlock()
{
int line;
int i, st;

	CWaitCursor wait;

FILETrace( "@CDBDinpView::StartBlock now %d, %08x\n", get_nowln(), this );

	//コントロールの NextControl() セット
//	NextControl( FALSE );
	c_set( IDC_ICSDISPCTRL10 );	// 入力モード表示にフォーカス

	m_bForkUCOM = TRUE;

	if( DATA_1RD )	DATA_1RD = 0;
	// 1行 付箋 OFF
	m_OneHusen = 0;
	// 輸入仕入れサインを クリア
	m_bSyzYunyu = FALSE;
	m_bSyzSotoMen = FALSE;
	// 部門・工事チェック時の フォーカス位置
	m_BKINP_PN = -1;

	//挿入行位置・モード
	m_InsLine = -1;
	m_InsMode = _APPEND;

	SetListTitle();
	SetListColumn( pAUTOSEL->DSPLINE_OPT );

	DWORD st1, st2;
	st1 = m_DtList.GetStyle();
	st2 = m_DtLine.GetStyle();

	DT_ITEMMAX im;
	im.im_seq = 8;
	im.im_denp = 7;
	im.im_eda = Voln1->edcol;
	im.im_bmn = Voln1->bmcol;
	im.im_koji= Voln1->kjcol;
	im.im_tky = (Voln1->tk_ln * 2);

	im.im_sno= Voln1->snocol;

//TRACE( "sizeof struct _AUTOSEL %d\n", sizeof (struct _AUTOSEL) ); 

	m_DtList.SetImax( &im );
	m_DtList.SetConfirmMode( Confirm_Mode() );
	m_DtList.SetDispDataLineMax(DDATA_LNMAX);

	m_DtLine.SetImax( &im );

	if( Voln1->apno == 0x20 && Voln1->kjcol ) {
		if( ! (pAUTOSEL->DSPKOJI_OPT != OPT_NODSP) )	st1 |= DS_KOJI_USE;
		else											st1 &= ~DS_KOJI_USE;
	//	m_DtList.SetStyle( st1 | DS_KOJI_USE );
		m_DtLine.SetStyle( st2 | DS_KOJI_USE );
	}
	else {
		st1 &= ~DS_KOJI_USE;
	//	m_DtList.SetStyle( (st1 & ~DS_KOJI_USE) );
		m_DtLine.SetStyle( (st2 & ~DS_KOJI_USE) );
	}
	// 設定情報よりセットする
	if(	pAUTOSEL->DSPBMON_OPT != OPT_NODSP && Voln1->bmcol )	
		st1 |= DS_BMON_USE;
	else
		st1 &= ~DS_BMON_USE;
	
	if( pAUTOSEL->DSPDENP_OPT != OPT_NODSP )	st1 |= DS_DENP_USE;
	else										st1 &= ~DS_DENP_USE;
	if( pAUTOSEL->DSPTGNO_OPT != OPT_NODSP )	st1 |= DS_SYONO_USE;
	else										st1 &= ~DS_SYONO_USE;
	if( pAUTOSEL->DSPTGDT_OPT != OPT_NODSP)		st1 |= DS_TGDT_USE;
	else										st1 &= ~DS_TGDT_USE;

	m_DtList.SetStyle( st1 );
	m_DtList.SetLineHeight( pAUTOSEL->DSPLNWD_OPT == OPT_NARROW ? -1 : 0 );
//	m_DtList.SetLines( pAUTOSEL->DSPLINE_OPT );

	SetDtListItemWidth();

	m_DtList.EnforceLayout();

	// 入力行の前回情報
	st1 = m_DtLine.GetStyle();
	if( pAUTOSEL->INP2BK_OPT == OPT_INP2BK )
		st1 |= DS_BK_DEBCRE;
	else
		st1 &= ~DS_BK_DEBCRE;

	m_DtLine.SetStyle(st1);
	m_DtLine.EnforceLayoutEX();

/*
	if( ! BMON_MST ) {
		m_DtLine.SetTitleClr( DATA_DEB_BMON, RGB_DARKER_GRAY, RGB_GRAY );
		m_DtLine.SetTitleClr( DATA_CRE_BMON, RGB_DARKER_GRAY, RGB_GRAY );

		m_DtLine.SetListItemEnable(1, DATA_DEB_BMON, FALSE);
		m_DtLine.SetListItemEnable(1, DATA_CRE_BMON, FALSE);
	}
	if( ! M_KOJI ) {
		m_DtLine.SetTitleClr( DATA_DEB_KNO, RGB_DARKER_GRAY, RGB_GRAY );
		m_DtLine.SetTitleClr( DATA_CRE_KNO, RGB_DARKER_GRAY, RGB_GRAY );

		m_DtLine.SetListItemEnable(1, DATA_DEB_KNO, FALSE);
		m_DtLine.SetListItemEnable(1, DATA_CRE_KNO, FALSE);
	}
	if( pAUTOSEL->BILL_OPT == OPT_NOINP ) {
		m_DtLine.SetTitleClr( DATA_SYONO, RGB_DARKER_GRAY, RGB_GRAY );
		m_DtLine.SetTitleClr( DATA_TGDATE, RGB_DARKER_GRAY, RGB_GRAY );

		m_DtLine.SetListItemEnable(1, DATA_SYONO, FALSE);
		m_DtLine.SetListItemEnable(1, DATA_TGDATE, FALSE);
	}
	if( pAUTOSEL->HUSEN_OPT == OPT_NOINP ) {
		m_DtLine.SetTitleClr( DATA_HUSEN, RGB_DARKER_GRAY, RGB_GRAY );
		m_DtLine.SetListItemEnable(1, DATA_HUSEN, FALSE);
	}
*/

	SCROLL_NL_ = m_DtList.GetDispDataCount();
FILETrace( "@CDBDinpView::StartBlock(L=%d), SCROLL_NL_ = %d\n", __LINE__, SCROLL_NL_ );

	if( SCROLL_NL_ > DDATA_LNMAX )	SCROLL_NL_ = DDATA_LNMAX;
	if( !IsMAsterKAKUTEI() )	SCROLL_NL_ += 1;	// 新規入力分

	/* 諸口合計	*/
	g_stotal( SYOGT, (char *)SYOG_d, (char *)SYOG_c, (char *)SG_def );
	syogdsp( (char *)SG_def );

	/* 現金合計	*/
	genk_strchg( pAUTOSEL->DCKM_CODE );
	g_stotal( /*GNKIN*/pAUTOSEL->DCKM_CODE, (char *)GENK_d, (char *)GENK_c, (char *)GK_def );
	genkdsp( (char *)GK_def );

	/* 現金合計の表示 */
	genk_ctrlshow( (pAUTOSEL->DCKM_OPT == OPT_NODSP) ? FALSE : TRUE );

	// ビットテーブル設定
	int da_max = pDBzm->m_damax;	// MAX仕訳数分メモリ確保

	if( MODEFY_BIT.mk_bitbl( da_max, OFF) == -1 )	//修正仕訳ビットテーブル
		ermset( ERROR_MEMORY, "仕訳修正ビットテーブルが作成できませんでした。");

	if( SYZMDFY_BIT.mk_bitbl( da_max, OFF) == -1 )		//スキャンビットテーブル
		ermset( ERROR_MEMORY, "消費税ビットテーブルが作成できませんでした。");

	// ＳＥＱテーブル作成
	if( APPEND_SEQ.mk_seqtbl( da_max ) == -1 )		//追加仕訳ＳＥＱ登録テーブル
		ermset( ERROR_MEMORY, "追加仕訳ＳＥＱ登録テーブルが作成できませんでした。");
	if( SEQ_APPEND.mk_seqtbl( da_max ) == -1 )		//追加仕訳ＳＥＱテーブル管理テーブル
		ermset( ERROR_MEMORY, "追加仕訳ＳＥＱテーブル管理テーブルが作成できませんでした。");
	if( SCMDFY_SEQ.mk_seqtbl( da_max ) == -1 )		//追加仕訳ＳＥＱテーブル管理テーブル
		ermset( ERROR_MEMORY, "カラムソート用ＳＥＱテーブルが作成できませんでした。");

	SEQ_SCMDFY.clear();

	if( MDFYDATA_SEQ.mk_seqtbl( da_max ) == -1 )	//カラムソート用dbdata SEQテーブル管理テーブル
		ermset( ERROR_MEMORY, "カラムソート用ＳＥＱテーブル管理テーブルが作成できませんでした。");

	CString filter;

#ifdef REGDATA // 07.31 /00
	InitRdata();
#endif

#ifdef _SLIP_ACCOUNTS_
	if( InitSldata() != 0 ) {
	}
#endif

	if( m_BrTek.brntky_iniz(pDBzm) != 0 ) {
		ermset( ERROR_MEMORY, "枝番摘要処理の初期化ができませんでした。");
	}

	// 枝番文字列に科目コード追加 サインをセット
	m_BrTek.brntky_kmcode( pAUTOSEL->SCNCD_OPT == OPT_SCNCD );

	// 選択初期化 & 消費税画面作成
	CDBbaseView::InitWork(TRUE);
	// サブ入力画面
	SubDataInpMake();

	// 月指定のイニシャライズ
	// 仕訳入力の日付選択データがあるか？
	st = CheckSelectMonthData();
	if( st == -1 ) {
		ermset( 0, "月指定のデータを取得できませんでした。");
		return;
	}
	m_selDate.SetMode( ICS_DATESELECT_MODE_ONESELF );
	m_selDate.Iniz( pDBzm, ICS_DATESELECT_RANGE_PERIOD|ICS_DATESELECT_EDIT_OFF|ICS_DATESELECT_INPOK_ONLY, 0, 0 );
	int sofs, eofs, sy, ey;
	sofs = eofs = sy = ey = -1;
	if( st ) {
		// 仕訳入力の日付選択データなし -> 全月指定
		pDBzm->minfo->MoveFirst();
		do {
			if( (pDBzm->minfo->insw & 0x01) ) {
				if( sofs == -1 )	sofs = pDBzm->minfo->ofset;

				eofs = pDBzm->minfo->ofset;
			}
		} while( pDBzm->minfo->MoveNext() == 0 );

		m_selDate.SetMonthOffset( sofs, eofs, sy, ey );
	}
	m_selDate.GetMonthOffset(sofs, eofs, sy, ey);
	if( sofs == -1 && eofs == -1 ) {
		ermset( ERROR_STOP, "日付の設定を取得できません！" );
		return;
	}

	pDBzm->set_datelimit( sofs, eofs, sy, ey );

	// 確定マスター新規入力窓を表示しない
	ScanMode_WndMove(IsMAsterKAKUTEI());

	if( sofs == eofs ) {
		m_bMnthFix = TRUE;
	}
	else {
		m_bMnthFix = FALSE;
	}

	// 部門チェック
	if( BmChk.SetDBZmSub(pDBzm) != 0 ) {
		ermset(0, _T("部門チェッククラスの作成ができません！"));
	}
	if( BmChk.BmnCheckInit() != 0 ) {
		ermset(0, _T("部門チェッククラスの初期化ができません！"));
	}

	// 工事番号 チェック
	if( KjChk.SetDBZmSub(pDBzm) != 0 ) {
		ermset(0, _T("工事番号チェッククラスの作成ができません！"));
	}
	if( KjChk.KojiCheckInit() != 0 ) {
		ermset(0, _T("工事番号チェッククラスの初期化ができません！"));
	}
	//data display initialize
	for( i = 0 ; i < DDATA_LNMAX ; ++i )
	{
		//データイニシャライズ
		inpline_inz( &DDATA_LINE[i] );
	}

	int mode;
	mode = IsMAsterKAKUTEI() ? _MODEFY : _APPEND;

	inpmdisp( mode );
	// 自動項目色
	AutoSelIndicator( pAUTOSEL, INP_mode );

	/* ｻｲｼｭｳ ﾃﾞｰﾀ ﾖﾋﾞﾀﾞｼ */
	SEQ_NO		= 0;
	USERS_SEQ	= 0;
	line = ldata_dsp();

	//サンプルレコード保存[06.27 /13]
//	SaveDinpDataSREC( _APPEND );
	SaveDinpParaSREC( _APPEND );

FILETrace( "@CDBDinpView::StartBlock(L=%d)\n", __LINE__ );
	//データスクロール行表示
	ddata_dsp(-1);

FILETrace( "@CDBDinpView::StartBlock(L=%d)\n", __LINE__);

	//データスクロール行底SEQ
	if( DDATA_LINE[ SCROLL_NL()-1 ].ZREC_NO ) {
		BOTOM_SEQ = SEQ_APPEND.ngetseq( ZREC[ DDATA_LINE[ SCROLL_NL()-1 ].ZREC_NO - 1].m_seq) - 1;
	}
	else
		BOTOM_SEQ = 0;

	//新規入力データイニシャライズ
	inpline_inz( &INP_LINE );

	if( !IsMAsterKAKUTEI() ) {
		//初期日付
		INP_LINE._XXDTA.dd_ddate = CREC.m_ddate;
		INP_LINE._XXDTA.dd_mofs = CREC.m_mofs;
		INP_LINE._XXDTA.dd_ksign = CREC.m_ksign;

		pDBzm->DateConv( INP_LINE.BcdDate, &CREC, CV2_1 );

		inpline_dsp( &INP_LINE, APPEND_LINE );

		set_nowln( APPEND_LINE );	//追加入力行
		set_prevln( APPEND_LINE );
	}
	else {
		set_nowln( SCROLL_NL() );	//最終行
		set_prevln( SCROLL_NL() );
	}
FILETrace( "@CDBDinpView::StartBlock(L=%d)\n", __LINE__ );

	//部門入力画面作成
	BkInpMakeX();

	// 一括修正の項目をｲﾆｼｬﾗｲｽﾞ(static)
	CRpkeyDlg::keyline_inz();
	CRpkeyDlg::set_tekmode( m_TEK_MODE );

	SelPar par;
	par.sel_pn = SL_KAMOKU_PN;
	m_Seldata.SelectJob( &par, -1 );
	ResetSelect();


	// 仕訳がすべてのユーザで、入力されていない場合は、
	// ここに来る前に、EXIT している
	// スキャンオンリー時
	if( bSCAN_ONLY_MODE )
	{
//		scheck( 0 );	// 残り仕訳数表示のため
		//現在のユーザの権限が、他の仕訳を見られないときは、終了する
		if( !line && ! pDBzm->data_authority() )
			ermset( 0, _T("検索する仕訳がありません。") );
	}
	// 確定マスター
	if( bCONFIRM_MASTER ) {
		//現在のユーザの権限が、他の仕訳を見られないときは、終了する
		if( !line && ! pDBzm->data_authority() )
			ermset( 0, _T("検索する仕訳がありません。") );
	}

	//仕訳フルチェック
	if( ! bCONFIRM_MASTER ) {
		if( !bSCAN_ONLY_MODE && scheck( 1 ) ) 
		{
			ermset( 0, get_Dfullmsg() );
		
			EndBlock( 0 );
			return;
		}
	}
	else {
		if( ! line ) {
			// 確定マスターで、現ユーザには仕訳がない。(他ユーザの仕訳はある)
			bSCAN_ONLY_MODE = TRUE;	
		}
	}
FILETrace( "@CDBDinpView::StartBlock(L=%d)\n", __LINE__ );

//	pDBsy->Sy_iniz( &CREC, 0 );
	DB_SyIniz( &CREC );

	// 自動仕訳 (社会福祉のみ)
	if( IsOKGyosyu( pDBzm->zvol->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ ) {

		if( m_pSjb == NULL ) {
			m_pSjb = new CSwkJob;
			if( m_pSjb == NULL ) {
				ermset( 0, _T("自動仕訳クラスの作成ができません！") );
			}
		}

		if( m_pSjb->Initial() != 0 ) {
			ermset( 0, _T("自動仕訳クラスの作成ができません！") );
		}
	}

	if( m_pSCSrc == NULL ) {
		m_pSCSrc = new CSWKCpySrc;
		if( m_pSCSrc == NULL ) {
			ermset( 0, _T("仕訳コピークラスの作成ができません！") );
		}
	}

FILETrace( "@CDBDinpView::StartBlock END\n" );
};


// 仕訳入力の月情報があるかをチェックする。
//		返送値	-1:	error
//				 1:	月情報なし
//				 0:	あり
//
int CDBDinpView::CheckSelectMonthData()
{
	CString filter;
	int own_sgn = 0;
	int ret = 0;

	if( pDBzm->owntb == NULL ) {
		if( pDBzm->OwnTblOpen() != 0 )
			return -1;
		own_sgn = 1;
	}

	ICSMCTL	mctrl;
	mctl_usertbl	utbl;

	if( mctrl.SetDatabase() == 0 ) {
		mctrl.get_usertbl( utbl );
	}
	else {
		return -1;
	}

	filter.Format( "apl_name = 'DBDinp' AND itm_name = 'select_month' AND u_id = %d", utbl.user_id );

	pDBzm->owntb->Requery( filter );

	if( pDBzm->owntb->st == -1 ) {
		// 仕訳入力の月情報がない
		ret = 1;
	}

	if( own_sgn ) {
		pDBzm->OwnTblClose();
	}

	return ret;
}


// 最初のフォーカスをセット
// 画面をすべて作成してから
void CDBDinpView::set_firstfocus( int ReCosel/*=0*/)
{

	PROGRAM_OK = TRUE;	
	int set_pn = DATE_PN;

	if( !IsMAsterKAKUTEI() ) {
		set_nowln(APPEND_LINE);
	}
	else {
		set_nowln( SCROLL_NL() );
		//伝票入力タイプ形式で表示の場合
		if( DenpTypeMode() )	set_pn = SEQ_PN;
	}

	set_nowpn(set_pn);
	PostMessage( WM_RETROFOCUS, set_pn );

}

// 会社再選択時
void CDBDinpView::ReCoselSetFocus( BOOL bNewCo )
{
//FILETrace( "---> ReCoselSetFocus\n" );

	if( bNewCo ) {
		// 新しく会社を選んだ
		set_firstfocus( 1 );

	}
	else {
		// 会社確認でキャンセル
		int ln, pn;
		ln = get_nowln();
		pn = get_nowpn();

		_set_focus( ln, pn );
	}
}

//リスト項目幅セット
void CDBDinpView::SetDtListItemWidth()
{
	if( m_DtList.GetLines() != 1 )
		return;

	int scrX = GetSystemMetrics(SM_CXFULLSCREEN);
	if( pAUTOSEL->RESOL_OPT == scrX ) {
		m_DtList.EnforceLayout();

		int itm_wd;
		int save_wd_use = 1;

		DT_RECT* pdt;
		pdt = m_DtList.GetDtRect();

		int clq_wd = 0;

		for( int i = 0; i < pdt->Dtcnt; i++ ) {
			itm_wd = pdt->DtRect[i].Width();

			//MyTrace("[%d] itm_wd = %d\n", pdt->DtID[i], itm_wd);
			//MyTrace("----clq_wd = %d\n", clq_wd);

			switch( pdt->DtID[i] ) {
			case DATA_SEQ:
				if( pAUTOSEL->WDSEQ_OPT != 0 ) {
					clq_wd += pAUTOSEL->WDSEQ_OPT;
				}
				else {
					clq_wd += itm_wd;
				}
				break;
			case DATA_DATE:
				if( pAUTOSEL->WDDATE_OPT != 0 ) {
					clq_wd += pAUTOSEL->WDDATE_OPT;
				}
				else {
					clq_wd += itm_wd;
				}
				break;
			case DATA_DENP:
				if( pAUTOSEL->WDDENP_OPT != 0 ) {
					clq_wd += pAUTOSEL->WDDENP_OPT;
				}
				else {
					clq_wd += itm_wd;
				}
				break;

			case DATA_DEB_BMON:
				if( pAUTOSEL->WDBMON_OPT != 0 ) {
					clq_wd += pAUTOSEL->WDBMON_OPT;
				}
				else {
					clq_wd += itm_wd;
				}
				break;
			case DATA_DEB_KNO:
				if( pAUTOSEL->WDKOJI_OPT != 0 ) {
					clq_wd += pAUTOSEL->WDKOJI_OPT;
				}
				else {
					clq_wd += itm_wd;
				}
				break;
			case DATA_DEBT:
				if( pAUTOSEL->WDDEBT_OPT != 0 ) {
					clq_wd += pAUTOSEL->WDDEBT_OPT;
				}
				else {
					clq_wd += itm_wd;
				}
				break;
			case DATA_CRED:
				if( pAUTOSEL->WDCRED_OPT != 0 ) {
					clq_wd += pAUTOSEL->WDCRED_OPT;
				}
				else {
					clq_wd += itm_wd;
				}
				break;
			case DATA_VAL:
				if( pAUTOSEL->WDVAL_OPT != 0 ) {
					clq_wd += pAUTOSEL->WDVAL_OPT;
				}
				else {
					clq_wd += itm_wd;
				}
				break;

			case DATA_ZEI:
				if( pAUTOSEL->WDZEI_OPT != 0 ) {
					clq_wd += pAUTOSEL->WDZEI_OPT;
				}
				else {
					clq_wd += itm_wd;
				}
				break;
			case DATA_SYZ:
				if( pAUTOSEL->WDSYZ_OPT != 0 ) {
					clq_wd += pAUTOSEL->WDSYZ_OPT;
				}
				else {
					clq_wd += itm_wd;
				}
				break;
			case DATA_TEKIYO:
				if( pAUTOSEL->WDTKY_OPT != 0 ) {
					clq_wd += pAUTOSEL->WDTKY_OPT;
				}
				else {
					//右端なので、最低限分 16ピクセル
					clq_wd += 16;
				}
				break;

			case DATA_DOCEVI:
				if( pAUTOSEL->WDSYON_OPT != 0 ) {
					clq_wd += pAUTOSEL->WDSYON_OPT;
				}
				else {
					clq_wd += itm_wd;
				}
				break;

			default:
				clq_wd += itm_wd;
				break;
			}
		}
//		MyTrace("clq_wd = %d, line Width = %d\n", clq_wd, pdt->LineRect.Width());

		if( clq_wd > pdt->LineRect.Width() ) {
			save_wd_use = 0;
		}
		if( save_wd_use ) {
			m_DtList.SetListItemWidth(DATA_SEQ, pAUTOSEL->WDSEQ_OPT);
			m_DtList.SetListItemWidth(DATA_DATE, pAUTOSEL->WDDATE_OPT);
			m_DtList.SetListItemWidth(DATA_DENP, pAUTOSEL->WDDENP_OPT);
			m_DtList.SetListItemWidth(DATA_DEB_BMON, pAUTOSEL->WDBMON_OPT);
			m_DtList.SetListItemWidth(DATA_DEB_KNO, pAUTOSEL->WDKOJI_OPT);
			m_DtList.SetListItemWidth(DATA_DEBT, pAUTOSEL->WDDEBT_OPT);
			m_DtList.SetListItemWidth(DATA_CRED, pAUTOSEL->WDCRED_OPT);
			m_DtList.SetListItemWidth(DATA_VAL, pAUTOSEL->WDVAL_OPT);
			m_DtList.SetListItemWidth(DATA_ZEI, pAUTOSEL->WDZEI_OPT);
			m_DtList.SetListItemWidth(DATA_SYZ, pAUTOSEL->WDSYZ_OPT);
			m_DtList.SetListItemWidth(DATA_TEKIYO, pAUTOSEL->WDTKY_OPT);
			m_DtList.SetListItemWidth(DATA_HJIYU, pAUTOSEL->WDHJYU_OPT);
			m_DtList.SetListItemWidth(DATA_DOCEVI, pAUTOSEL->WDSYON_OPT);
		}
	}
}

//項目幅保存
void CDBDinpView::GetDtListItemWidth()
{
	if( !::IsWindow(m_DtList.m_hWnd) )	return;

	if( m_DtList.GetLines() == 1 ) {
		if( pAUTOSEL != NULL ) {
			pAUTOSEL->WDSEQ_OPT = m_DtList.GetListItemWidth(DATA_SEQ);
			pAUTOSEL->WDDATE_OPT = m_DtList.GetListItemWidth(DATA_DATE);
			pAUTOSEL->WDDENP_OPT = m_DtList.GetListItemWidth(DATA_DENP);
			pAUTOSEL->WDBMON_OPT = m_DtList.GetListItemWidth(DATA_DEB_BMON);
			pAUTOSEL->WDKOJI_OPT = m_DtList.GetListItemWidth(DATA_DEB_KNO);
			pAUTOSEL->WDDEBT_OPT = m_DtList.GetListItemWidth(DATA_DEBT);
			pAUTOSEL->WDCRED_OPT = m_DtList.GetListItemWidth(DATA_CRED);
			pAUTOSEL->WDVAL_OPT = m_DtList.GetListItemWidth(DATA_VAL);
			pAUTOSEL->WDZEI_OPT = m_DtList.GetListItemWidth(DATA_ZEI);
			pAUTOSEL->WDSYZ_OPT = m_DtList.GetListItemWidth(DATA_SYZ);
			pAUTOSEL->WDTKY_OPT = m_DtList.GetListItemWidth(DATA_TEKIYO);
			pAUTOSEL->WDHJYU_OPT = m_DtList.GetListItemWidth(DATA_HJIYU);
			pAUTOSEL->WDSYON_OPT = m_DtList.GetListItemWidth(DATA_DOCEVI);
		}
	}
}


//END Write
void CDBDinpView::EndBlock( int mode )
{
	// プログラム準備ＯＫサインＯＦＦ
	PROGRAM_OK = FALSE;

	// 画面の幅等を保存する
	int scrX = GetSystemMetrics(SM_CXFULLSCREEN);
	if( pAUTOSEL != NULL ) {
		pAUTOSEL->RESOL_OPT = scrX;
	}
	GetDtListItemWidth();

#ifdef _SLIP_ACCOUNTS_
	HideImgDlg();
#endif
	// 04.07 /14
	Destroy_ModeLessDlg();

	jb_end( mode );

	// 選択クラスへのデータをセットするため
	CDBbaseView::m_bSelDataInit = FALSE;
}




//#* 08.05 /02 --------------------------------------------------------------------------
void CDBDinpView::SetICSSel8( long selval, BOOL bNumber /*=TRUE*/ )
{
	if( m_selfix8.GetNumber() != bNumber )
		m_selfix8.SetNumber( bNumber );

ASSERT( FALSE );

	m_selfix8.SetData( selval );
}

void CDBDinpView::SetICSSel32( long selval, BOOL bNumber /*=TRUE*/ )
{
	if( m_selval32.GetNumber() != bNumber )
		m_selval32.SetNumber( bNumber );

ASSERT( FALSE );

	m_selval32.SetData( selval );
}

#ifndef _SLIP_ACCOUNTS_
void CDBDinpView::OnButtonF11() 
{
	if( ! PROGRAM_OK )
		return;

	if( INP_mode != _KEYSCAN ) {
		if( get_nowpn() != TKY_PN ) {
			DWORD stl = m_DtLine.GetStyle();
			if( (stl & DS_BK_DEBCRE) ) {
				stl &= ~DS_BK_DEBCRE;
				m_DtLine.SetStyle(stl);
				m_DtLine.EnforceLayoutEX( TRUE );

				pAUTOSEL->INP2BK_OPT = OPT_NONOPT;
			}
			else {
				stl |= DS_BK_DEBCRE;
				m_DtLine.SetStyle(stl);
				m_DtLine.EnforceLayoutEX( TRUE );

				pAUTOSEL->INP2BK_OPT = OPT_INP2BK;
			}
			// 自動項目色
			AutoSelIndicator( pAUTOSEL, INP_mode );
		}
	}
	else {
		// 仕訳検索修正中は、摘要複写キー
		if( get_nowpn() != TKY_PN )
			return;

		if( ! IsSCAN_TKYONLY() || /*IMG_master == ERR ||*/ INP_mode == _APPEND )
			return;

		int ln = get_nowln();
		CString str;
		VARIANT var;
		char	buf[128] = {0};

		if( ln > 1 && _getLINE_DATA( ln-1 )->ZREC_NO > 0 ) {
			if( _getLINE_DATA( ln-1 )->LINE_CND[ TKY_PN ].IMG_sg == 0 ) {
				str = _getLINE_DATA( ln-1 )->_XXDTA.dd_tek.tekiyo;
				var.pbVal = (BYTE*)(LPCTSTR)str;
				exDBdata_set( TKY_PN, _getINP_CTL( ln, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

				_getLINE_DATA( ln )->_XXDTA.dd_tek.tekiyo = str;
				_getLINE_DATA( ln )->_XXDTA.dd_tek.height = 0;
				_getLINE_DATA( ln )->_XXDTA.dd_tek.width = 0;
				_getLINE_DATA( ln )->_XXDTA.dd_tek.image = NULL;
				_getLINE_DATA( ln )->ImgSEQ = 0;

				if( getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg ) {
					getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg = 0;
					TerminationDataSet( TKY_PN );

					CREC.m_dsign[0] &= ~0x80;
					CREC.m_imgsq = -1;
				}
			}
			else if( _getLINE_DATA( ln-1 )->LINE_CND[ TKY_PN ].IMG_sg == 1 ) {
				// イメージ摘要
				CREC.m_tekiyo.Empty();
				CREC.m_dsign[0] |= 0x80;
				CREC.m_imgsq = _getLINE_DATA( ln-1 )->ImgSEQ;

				_getLINE_DATA( ln )->_XXDTA.dd_tek.tekiyo.Empty();
				_getLINE_DATA( ln )->_XXDTA.dd_tek.height = 0;
				_getLINE_DATA( ln )->_XXDTA.dd_tek.width = 0;
				_getLINE_DATA( ln )->_XXDTA.dd_tek.image = NULL;
				_getLINE_DATA( ln )->ImgSEQ = _getLINE_DATA( ln-1 )->ImgSEQ;

				getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg = 1;

				var.pbVal = NULL;
				exDBdata_set( TKY_PN, _getINP_CTL( ln, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				// イメージ再表示
				TerminationDataSet( TKY_PN );
				// 欄外大きなイメージ
				big_img( getLINE_DATA(), getINP_CTL( TKY_PN ));
			}
		}
	}
}
#else
void CDBDinpView::OnButtonF11() 
{
	// 電卓ウィンドウ消去
	CalcMouseEvent();

//--> '12.02.15 INS START
	if( IsDispSlip() ){
		// 転記
		CString	str, tmpStr;
		BYTE	setday[10]= {0};
		if( get_nowpn() == DATE_PN ){
			// 日付
#ifndef _SCANNER_SV_
			m_pImgmng->GetSelectTextInDlg( str );
			IMG_ANALYZEDREC	analyzedRec;
			int	mm=0, dd=0;
			if( m_pImgmng->AnalyzeStrForData(str, ID_DATE_SEARCH, analyzedRec) == 0 ){
				mm = analyzedRec.mmdd / 100;
				dd = analyzedRec.mmdd % 100;

				char	data[16] = {0};
				BYTE	bcd[10] = {0};
				wsprintf( data, _T("%02d%02d"), mm, dd );
				if( DateInputCheck( data, bcd ) == 0 ) {

					char ymd[3] = {0};
					char check[3] = {0};
					memcpy( check, bcd, 3 );
					if( dinp_modefy_ok( getLINE_DATA() ) && pDBzm->check_datelimit( (BYTE*)check ) && 
						!pDBzm->Myvd_chek( (BYTE*)check ) && !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd ) ){

						CArith ar;
						ar.int_bcd( &setday[0], mm, 1 );
						ar.int_bcd( &setday[1], dd, 1 );

						BYTE bcd[10] = {0};
						bcd[0] = setday[0];
						bcd[1] = setday[1];
						bcd[2] = 0x00;

						VARIANT var;
						if( ! m_bMnthFix ) {
							var.pbVal = bcd;
							exDBdata_set( DATE_PN, getINP_CTL(this, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
						}
						else  {
							var.pbVal = &bcd[1];
							exDBdata_set( DATE_PN, getINP_CTL(this, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 2 );
						}
				//		exDBdata_set( DATE_PN, getINP_CTL(this, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );

						m_DtLine.SetPosition( get_nowln(), DATE_PN, 1 );
					}
				}
			}
#else
			IMGEDIT_INFO	editInfo={0};
			if( m_pImgmng->GetExtInfoInDlg(editInfo)==0 ){
				int mmdd = editInfo.ymd % 10000;
				int mm = mmdd / 100;
				int dd = mmdd % 100;

				char	data[16] = {0};
				BYTE	bcd[10] = {0};
				wsprintf( data, _T("%02d%02d"), mm, dd );
				if( DateInputCheck( data, bcd ) == 0 ) {

					char ymd[3] = {0};
					char check[3] = {0};
					memcpy( check, bcd, 3 );
					if( dinp_modefy_ok( getLINE_DATA() ) && pDBzm->check_datelimit( (BYTE*)check ) && 
						!pDBzm->Myvd_chek( (BYTE*)check ) && !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd ) ){

						CArith ar;
						ar.int_bcd( &setday[0], mm, 1 );
						ar.int_bcd( &setday[1], dd, 1 );

						BYTE bcd[10] = {0};
						bcd[0] = setday[0];
						bcd[1] = setday[1];
						bcd[2] = 0x00;

						VARIANT var;
						if( ! m_bMnthFix ) {
							var.pbVal = bcd;
							exDBdata_set( DATE_PN, getINP_CTL(this, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
						}
						else  {
							var.pbVal = &bcd[1];
							exDBdata_set( DATE_PN, getINP_CTL(this, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 2 );
						}
				//		exDBdata_set( DATE_PN, getINP_CTL(this, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );

						m_DtLine.SetPosition( get_nowln(), DATE_PN, 1 );
					}
				}
			}
#endif
		}
		else if( get_nowpn() == VAL_PN ){
			// 金額
#ifndef _SCANNER_SV_
			m_pImgmng->GetSelectTextInDlg( str );
			IMG_ANALYZEDREC	analyzedRec;
			if( m_pImgmng->AnalyzeStrForData(str, ID_MONY_SEARCH, analyzedRec) == 0 ){
				char	smony[64] = {0};
				sprintf_s( smony, sizeof(smony), _T("%.0f"), analyzedRec.mony );

				VARIANT var;
				char	ip_val[6];
				memset( ip_val, '\0', sizeof(ip_val) );

				CArith ar;
				ar.l_defn( 0x16 );
				ar.l_input( ip_val, smony );
				var.pbVal = (BYTE*)ip_val;
				exDBdata_set( VAL_PN, getINP_CTL( this, VAL_PN )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
				TerminationDataSet( VAL_PN );
			}
#else
			IMGEDIT_INFO	editInfo={0};
			if( m_pImgmng->GetExtInfoInDlg(editInfo)==0 ){
				char	smony[64] = {0};
				sprintf_s( smony, sizeof(smony), _T("%I64d"), editInfo.price );

				VARIANT var;
				char	ip_val[6];
				memset( ip_val, '\0', sizeof(ip_val) );

				CArith ar;
				ar.l_defn( 0x16 );
				ar.l_input( ip_val, smony );
				var.pbVal = (BYTE*)ip_val;
				exDBdata_set( VAL_PN, getINP_CTL( this, VAL_PN )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
				TerminationDataSet( VAL_PN );
			}
#endif
		}
		else if( get_nowpn() == TKY_PN ){
			// 選択中の文字列
			char	check[128] = {0};

#ifndef _SCANNER_SV_
			m_pImgmng->GetSelectTextInDlg( str );
#else
			IMGEDIT_INFO	editInfo={0};
			if( m_pImgmng->GetExtInfoInDlg(editInfo)==0 ){
				str.Format( _T("%s"), editInfo.tekiyo );
			}
#endif
			VARIANT var;

			exDBdata_get( TKY_PN, getINP_CTL( this, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			if( var.pbVal != NULL )	strcpy_s( check, sizeof check, (char*)var.pbVal );
			else					memset( &check, '\0', sizeof(check));
			if( strlen(check) ){
				str = check + str;
			}
			int pos = str.GetLength();
			var.pbVal = (BYTE*)(LPCTSTR)str;
			exDBdata_set( TKY_PN, getINP_CTL( this, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			((CICSDBEDT*)this->GetDlgItem( getINP_CTL( this, TKY_PN )->IDC_X))->SetCaretPosition( pos );
		}

		return;
	}
//<-- '12.02.15 INS END

	if( ! PROGRAM_OK )
		return;

	if( INP_mode != _KEYSCAN ) {
		if( get_nowpn() != TKY_PN ) {
			DWORD stl = m_DtLine.GetStyle();
			if( (stl & DS_BK_DEBCRE) ) {
				stl &= ~DS_BK_DEBCRE;
				m_DtLine.SetStyle(stl);
				m_DtLine.EnforceLayoutEX( TRUE );

				pAUTOSEL->INP2BK_OPT = OPT_NONOPT;
			}
			else {
				stl |= DS_BK_DEBCRE;
				m_DtLine.SetStyle(stl);
				m_DtLine.EnforceLayoutEX( TRUE );

				pAUTOSEL->INP2BK_OPT = OPT_INP2BK;
			}
			// 自動項目色
			AutoSelIndicator( pAUTOSEL, INP_mode );
		}
	}
	else {
		// 仕訳検索修正中は、摘要複写キー
		if( get_nowpn() != TKY_PN )
			return;

		if( ! IsSCAN_TKYONLY() || /*IMG_master == ERR ||*/ INP_mode == _APPEND )
			return;

		int ln = get_nowln();
		CString str;
		VARIANT var;
		char	buf[128] = {0};

		if( ln > 1 && _getLINE_DATA( ln-1 )->ZREC_NO > 0 ) {
			if( _getLINE_DATA( ln-1 )->LINE_CND[ TKY_PN ].IMG_sg == 0 ) {
				str = _getLINE_DATA( ln-1 )->_XXDTA.dd_tek.tekiyo;
				var.pbVal = (BYTE*)(LPCTSTR)str;
				exDBdata_set( TKY_PN, _getINP_CTL( ln, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

				_getLINE_DATA( ln )->_XXDTA.dd_tek.tekiyo = str;
				_getLINE_DATA( ln )->_XXDTA.dd_tek.height = 0;
				_getLINE_DATA( ln )->_XXDTA.dd_tek.width = 0;
				_getLINE_DATA( ln )->_XXDTA.dd_tek.image = NULL;
				_getLINE_DATA( ln )->ImgSEQ = 0;

				if( getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg ) {
					getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg = 0;
					TerminationDataSet( TKY_PN );

					CREC.m_dsign[0] &= ~0x80;
					CREC.m_imgsq = -1;
				}
			}
			else if( _getLINE_DATA( ln-1 )->LINE_CND[ TKY_PN ].IMG_sg == 1 ) {
				// イメージ摘要
				CREC.m_tekiyo.Empty();
				CREC.m_dsign[0] |= 0x80;
				CREC.m_imgsq = _getLINE_DATA( ln-1 )->ImgSEQ;

				_getLINE_DATA( ln )->_XXDTA.dd_tek.tekiyo.Empty();
				_getLINE_DATA( ln )->_XXDTA.dd_tek.height = 0;
				_getLINE_DATA( ln )->_XXDTA.dd_tek.width = 0;
				_getLINE_DATA( ln )->_XXDTA.dd_tek.image = NULL;
				_getLINE_DATA( ln )->ImgSEQ = _getLINE_DATA( ln-1 )->ImgSEQ;

				getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg = 1;

				var.pbVal = NULL;
				exDBdata_set( TKY_PN, _getINP_CTL( ln, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				// イメージ再表示
				TerminationDataSet( TKY_PN );
				// 欄外大きなイメージ
				big_img( getLINE_DATA(), getINP_CTL( TKY_PN ));
			}
		}
	}
}
#endif

#ifndef _SLIP_ACCOUNTS_
void CDBDinpView::OnUpdateButtonF11(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください

	if( bIKAT_SCAN || ! PROGRAM_OK || bCONFIRM_MASTER )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
	if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() == INP_VIEW )
	{
		if( get_nowpn() == TKY_PN ) {
			// 摘要のみ修正時は、摘要複写可能
			if( INP_mode == _KEYSCAN && IsSCAN_TKYONLY() ) {
				pCmdUI->Enable( TRUE );
				return;
			}
		}
		else {
			if( (INP_mode != _RETREV && INP_mode != _KEYSCAN) ) {
				pCmdUI->Enable( TRUE );
				return;
			}
		}
	}
	pCmdUI->Enable( FALSE );
}
#else
void CDBDinpView::OnUpdateButtonF11(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください

	if( bIKAT_SCAN || ! PROGRAM_OK || bCONFIRM_MASTER )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
	if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() == INP_VIEW )
	{
		if( get_nowpn() == TKY_PN ) {
/*- '12.10.05 -*/
//			// 摘要のみ修正時は、摘要複写可能
//			if( INP_mode == _KEYSCAN && IsSCAN_TKYONLY() ) {
//				pCmdUI->Enable( TRUE );
//				return;
//			}
/*-------------*/
			if( IsDispSlip() == FALSE ){
				// 摘要のみ修正時は、摘要複写可能
				if( INP_mode == _KEYSCAN && IsSCAN_TKYONLY() ) {
					pCmdUI->Enable( TRUE );
					return;
				}
			}
			else{
				pCmdUI->Enable( TRUE );
				return;
			}
/*-------------*/
		}
		else {
			if( (INP_mode != _RETREV && INP_mode != _KEYSCAN) ) {
				pCmdUI->Enable( TRUE );
				return;
			}
		}
	}
	pCmdUI->Enable( FALSE );
}
#endif

// 摘要辞書
void CDBDinpView::OnButtonF12() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
#ifdef _TEST_CRASH
	char buf[10];
	char* p = NULL;

//	memcpy( &buf[10], buf, 10000000 );
	DBKNREC* pKn = NULL;
	sprintf_s( buf, sizeof buf, "%d", pKn->knicod );

//	strcpy_s( &buf[128], sizeof buf, "aiueokakikukeko" );
#endif

#ifdef _SLIP_ACCOUNTS_
	static	int	svSelDispPN = 0;
#endif

	const MSG* pMsg = CWnd::GetCurrentMessage();

#ifndef _SLIP_ACCOUNTS_
	// アクセラレータで来た分は処理しない [10.22 /08]
	if( HIWORD(pMsg->wParam) != 0 )
		return;
#else
	// 原票の時のみ処理する
	if( IsDispSlip() && (get_nowpn()==TKY_PN) ){
		;
	}
	else{
		// アクセラレータで来た分は処理しない [10.22 /08]
		if( HIWORD(pMsg->wParam) != 0 )
			return;
	}
#endif

	if( get_nowpn() == TKY_PN ) {
		HIMC	hImc;
		char	buf[128] = {0};
		char	yomi[128] = {0};

		UINT ID;
		ID = getINP_CTL( TKY_PN )->IDC_X;

		hImc = ImmGetContext( GetDlgItem( ID )->m_hWnd );
		ImmGetCompositionString( hImc, GCS_COMPSTR, buf, sizeof buf );
		ImmGetCompositionString( hImc, GCS_COMPREADSTR, yomi, sizeof yomi );

		if( buf[0] && yomi[0] ) {
#ifdef _SLIP_ACCOUNTS_
			svSelDispPN = m_SelDispPN;		// 保存
#endif
			OnImeEndCompositionIcsdbedtctrl( ID, VK_F12, buf, yomi );
			ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		}
#ifdef _SLIP_ACCOUNTS_
		else if( (buf[0]=='\0') && (yomi[0]=='\0') ){
//-- '16.03.26 --
//			if( IsDispSlip() == FALSE ){
//				svSelDispPN = m_SelDispPN;		// 保存
//				if( m_SelDispPN == SL_SLIP_TKY_PN ){
//					SelPar	par;
//					par.sel_pn = SL_TKY_PN;
//					par.dbt	= CREC.m_dbt;
//					par.cre = CREC.m_cre;
//					::ZeroMemory( par.tky_kana, sizeof par.tky_kana );
//
//					m_Seldata.SelectJob( &par, -1 );
//				}
//			}
//			else{
//				if( m_SelDispPN != SL_SLIP_TKY_PN ){
//					svSelDispPN = m_SelDispPN;	// 保存
//					SelPar	par;
//					par.sel_pn = SL_SLIP_TKY_PN;
//					par.dbt	= CREC.m_dbt;
//					par.cre = CREC.m_cre;
//					::ZeroMemory( par.tky_kana, sizeof par.tky_kana );
//
//					m_Seldata.SelectJob( &par, -1 );
//				}
//				else{
//					SelPar	par;
//					par.dbt	= CREC.m_dbt;
//					par.cre = CREC.m_cre;
//
//					if( svSelDispPN == SL_TKYKANA_PN ){
//						par.sel_pn = SL_TKYKANA_PN;
//						strncpy_s( par.tky_kana, sizeof par.tky_kana, m_tky_50kana, sizeof par.tky_kana );
//					}
//					else{
//						par.sel_pn = SL_TKY_PN;
//						::ZeroMemory( par.tky_kana, sizeof par.tky_kana );
//					}
//
//					m_Seldata.SelectJob( &par, -1 );
//				}
//			}
//----------------
			svSelDispPN = m_SelDispPN;		// 保存
			if( m_SelDispPN == SL_SLIP_TKY_PN ){
				SelPar	par;
				par.sel_pn = SL_TKY_PN;
				par.dbt	= CREC.m_dbt;
				par.cre = CREC.m_cre;
				::ZeroMemory( par.tky_kana, sizeof par.tky_kana );

				m_Seldata.SelectJob( &par, -1 );
			}
//----------------
		}
#endif
		ImmReleaseContext( GetDlgItem( ID )->m_hWnd, hImc );
	}
}


void CDBDinpView::OnUpdateButtonF12(CCmdUI* pCmdUI) 
{

	if( !PROGRAM_OK )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
	if( get_nowpn() == TKY_PN /*&& ! TekiyoKeyMode()*/ )
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

/*
static BOOL isfixvalcnd( int pn )
{	
	return TRUE;
}
*/



//ポジションによる表示切替
void CDBDinpView::disp_change( int oldpn, int pn )
{
	int sel_pn = -1;

	BOOL	sg = TRUE;
	// 選択画面の表示色
	if( ! fixvalcnd() )
	{
		enable_fixval( sg );
	}
	SelPar par;
	par.brn_bmn = -1;
	par.brn_cd = -1;
	::ZeroMemory( par.brn_kana, sizeof par.brn_kana );
	::ZeroMemory( par.kmk_kana, sizeof par.kmk_kana );
	::ZeroMemory( par.tky_kana, sizeof par.tky_kana );

	int sgn;

	switch( pn ) {
	case BMON_PN:	case CBMN_PN:
		if( m_SelDispPN != SL_BUMON_PN && m_SelDispPN != SL_BMNKANA_PN)
			sel_pn = SL_BUMON_PN;
		break;
	case KOJI_PN:	case CKJI_PN:
		if( m_SelDispPN != SL_KOJI_PN && m_SelDispPN != SL_KOJIKANA_PN )
			sel_pn = SL_KOJI_PN;
		break;
	case TKY_PN:
#ifndef _SLIP_ACCOUNTS_
		if( m_SelDispPN != SL_TKY_PN && m_SelDispPN != SL_TKYKANA_PN ) {
			sel_pn = SL_TKY_PN;
			par.dbt = CREC.m_dbt;
			par.cre = CREC.m_cre;
		}
#else
		if( (m_SelDispPN!=SL_TKY_PN) && (m_SelDispPN!=SL_TKYKANA_PN) && (m_SelDispPN!=SL_SLIP_TKY_PN) ){
			// 原票からのイメージ摘要抽出を一時停止
			sel_pn = SL_TKY_PN;
			par.dbt = CREC.m_dbt;
			par.cre = CREC.m_cre;
		}
#endif
		break;
	case HJIYU_PN:
		if( pDBzm->zvol->apno < 0x50 )
			sel_pn = SL_HJIYU_PN;
		else {
			if( IsSyafuKaisei( pDBzm->zvol ) ) {
				if( m_SelDispPN != SL_NAIBU_PN )
					sel_pn = SL_NAIBU_PN;
			}
			else {
				sel_pn = SL_NPFURI_PN;
			}
		}
		par.dbt = getLINE_DATA()->_XXDTA.dd_dbt;
		par.cre = getLINE_DATA()->_XXDTA.dd_cre;
		break;
	case DEBT_PN: case CRED_PN:
		sgn = 0;
		if( oldpn == pn ) {
			if( get_nowln() == get_prevln() ) {
				sgn = 1;
			}
		}
		if( ! sgn ) {
			if( m_SelDispPN != SL_KAMOKU_PN && m_SelDispPN != SL_KMKANA_PN /*&& m_SelDispPN != SL_BRNTKY_PN*/ )
				sel_pn = SL_KAMOKU_PN;
		}
		break;

	default:
		if( m_SelDispPN != SL_KAMOKU_PN && m_SelDispPN != SL_KMKANA_PN )
			sel_pn = SL_KAMOKU_PN;
	
		break;
	}

	if( sel_pn != -1 ) {
		par.sel_pn = sel_pn;
		m_Seldata.SelectJob( &par, -1 );
	}

	// 固定科目選択のディセーブル／イネーブル
	switch( pn ) {
	case BMON_PN:	case CBMN_PN:
	case KOJI_PN:	case CKJI_PN:
	case SKBN_PN:	case HJIYU_PN:
	case TKY_PN:
		sg = FALSE;	
		break;
	default:
		sg = TRUE;
		break;
	}

	if( pAUTOSEL->BLKSEL_OPT && m_pSelWnd != NULL ) {
		if( sg != m_pSelWnd->fix8cnd() )
		{
			m_pSelWnd->fix8disp( sg );
		}
	}
	else {
		if( sg != fix8cnd() )
		{
			fix8disp( sg );
		}
	}

}


void CDBDinpView::fix8disp( BOOL sg )
{
	if( pAUTOSEL->BLKSEL_OPT ) {
		valtitle( FALSE );
		enable_fix8( FALSE );
	}
	else {
		CDBbaseView::fix8disp( sg );
	}
}


//ポジションによる表示切替（強制表示）
void CDBDinpView::_disp_change()
{
int pn = get_nowpn();
BOOL sg;

	// 固定項目の表示

//	sg = pn > VAL_PN ? FALSE : TRUE;
//	sg = pn > VAL_PN ? FALSE : (pn == BMON_PN) ? FALSE : TRUE;	//#* 02.27 /02
	switch( pn ) {
	case BMON_PN:	case CBMN_PN:
	case KOJI_PN:	case CKJI_PN:
	case SKBN_PN:	case HJIYU_PN:
	case TKY_PN:
		sg = FALSE;
		break;
	default:
		sg = TRUE;
		break;
	}

	if( pAUTOSEL->BLKSEL_OPT && m_pSelWnd != NULL ) {
		m_pSelWnd->fix8disp( sg );
	}
	else {
		fix8disp( sg );
	}
}


//諸口貸借表示
void CDBDinpView::syog_dsp( char *buf )
{
	// 元文字色セーブ
	static OLE_COLOR sv_fore = ((CICSDisp*)GetDlgItem( IDC_ICSDISPSYOGT ))->GetForeColor();

	val_disp( GetDlgItem( IDC_ICSDISPSYOGT ), buf, sv_fore );
}

//現金貸借表示
void CDBDinpView::genk_dsp( char *buf )
{
	// 元文字色セーブ
	static OLE_COLOR sv_fore = ((CICSDisp*)GetDlgItem( IDC_ICSDISPGENKIN ))->GetForeColor();

	val_disp( GetDlgItem( IDC_ICSDISPGENKIN ), buf, sv_fore );
}


//現金貸借表示
void CDBDinpView::genk_ctrlshow( BOOL bShow )
{
	// 元文字色セーブ
	int nCmdShow;
	nCmdShow = bShow ? SW_SHOW : SW_HIDE;

	m_GEN1.ShowWindow( nCmdShow );
	m_GEN2.ShowWindow( nCmdShow );
}

// 現金貸借表示文字列変更
void CDBDinpView::genk_strchg( char* kmcd )
{
	CString code, caption;
	code = kmcd;

	DBKNREC* pKn;
	pKn = pDBzm->DB_PjisToKnrec( code );
	char buf[64] = {0}, des[64] = {0};

	if( pKn ) {
		memcpy( buf, pKn->knnam, sizeof pKn->knnam );
		ksp_cut( des, buf, strlen( buf ));
		int n = get_strcnt( des, 8 );

		::ZeroMemory( buf, sizeof buf );
		memcpy( buf, des, n );
		caption.Format( _T( "%s貸借"), buf ) ;
	}
	else {
		caption = _T( "　　貸借" );
	}

	m_GEN1.SetCaption( caption );
}


// 入力モードセット＆表示
void CDBDinpView::set_inpmsg( int mode, char *msg )
{
	CWnd *pwd;
	pwd = GetDlgItem(IDC_ICSDISPCTRL10);

	switch( INP_mode = mode )
	{
	case _APPEND:
	case _MODEFY:
	case _KEYSCAN:
		pwd->SetWindowText(msg);
	//	if( IsJzSansyo() ) {
	//		((CICSDisp*)pwd)->SetForeColor(RGB_GRAY);
	//	}
	//	else {
			((CICSDisp*)pwd)->SetForeColor(mode == _APPEND ? RGB_YELLOW : RGB_SKY);
	//	}
		break;
	}

}

// 入力可能仕訳数表示
void CDBDinpView::inpn_dsp( int nline )
{
}


//#undef	FILETrace
//#define	FILETrace	MyTrace


void CDBDinpView::TraceWndRect(int num)
{
	CRect rc;

	GetDlgItem(IDC_ICSDISPGENKIN)->GetWindowRect( rc );
	ScreenToClient( &rc );

MyTrace( "(%d)TraceWndRect DISPGENKIN => [%4d,%4d,%4d,%4d]\n", num, rc.left, rc.top, rc.right, rc.bottom);

	m_DtLine.GetWindowRect( rc );
	ScreenToClient( &rc );

MyTrace( "(%d)TraceWndRect m_DtLine ===> [%4d,%4d,%4d,%4d]\n", num, rc.left, rc.top, rc.right, rc.bottom);
}


// 10.27 /10 他の入力画面ビューからの戻りで、表示の調整
void CDBDinpView::CtrlDispJob()
{
	// ダイアログ開始Y座標用
	if( IsNotMultiWindow() ) {
	}
	else {
		if( INP_mode == _APPEND ){
			// 定型仕訳残り数表示
			RDRemainDispJOB( );
		}
		else {
			RDRemainDisp( FALSE );
		}
		// 欄外大きなイメージ
		big_img( getLINE_DATA(), getINP_CTL( TKY_PN ));
		_disp_change();
	}
}

void CDBDinpView::ddata_dsp( int vect )	//vect == -1 : 底から上へ
{
	if (ddata_dspFlg) {
		ddata_dspFlg = FALSE;
		return;
	}
	//仕訳表示エリア表示
	_ddata_dsp( vect );
}

//仕訳表示エリア表示
void CDBDinpView::_ddata_dsp( int vect )	//vect == -1 : 底から上へ
{
	int i,n;
	m_DtList.RemoveAllDtListData();

	for( i = 0 ; i < SCROLL_NL() ; ++i )
	{
		n = vect < 0 ? SCROLL_NL() - (i + 1) : i;
		inpline_dsp( &DDATA_LINE[n], n + 1 );

FILETrace( "DDATA_LINE[%d]._XXDTA.dd_seq = %d\n", n, DDATA_LINE[n]._XXDTA.dd_seq );
//FILETrace( "DDATA_LINE[n]._XXDTA.dd_syzstr %s[%d]\n", DDATA_LINE[n]._XXDTA.dd_syz_str, n );
	}
	m_DtList.RedrawDataListCtrl();

	// 入力ラインの 表示更新
	if( ! IsMAsterKAKUTEI() ) {
		if( INP_mode == _APPEND || INP_mode == _MODEFY ) {
			inpline_update();
		}
	}
}


// 入力ラインの 表示更新
void CDBDinpView::inpline_update()
{
	// 科目名称再セット
	struct _DATA_LINE *data;
	data = _getLINE_DATA( APPEND_LINE );
	DBKNREC* pKn;

	// 借方
	if( ! data->_XXDTA.dd_dbt.IsEmpty() ) {
		pKn = pDBzm->DB_PjisToKnrec( data->_XXDTA.dd_dbt );
		if( pKn ) {
			KamokuString( pKn, data->_XXDTA.dd_deb_name );
		}
	}
	// 貸方
	if( ! data->_XXDTA.dd_cre.IsEmpty() ) {
		pKn = pDBzm->DB_PjisToKnrec( data->_XXDTA.dd_cre );
		if( pKn ) {
			KamokuString( pKn, data->_XXDTA.dd_cre_name );
		}
	}

	// 部門
	if( BMON_MST ) {
		set_etcdsp( APPEND_LINE, BMON_PN );
		set_etcdsp( APPEND_LINE, CBMN_PN );
	}
	// 工事
	if( M_KOJI ) {
		set_etcdsp( APPEND_LINE, KOJI_PN );
		set_etcdsp( APPEND_LINE, CKJI_PN );
	}
	// 借方／貸方
	set_etcdsp( APPEND_LINE, DEBT_PN );
	set_etcdsp( APPEND_LINE, CRED_PN );

	// 金額の表示フラグをセット
	data->_XXDTA.val_flag = data->LINE_CND[VAL_PN].INP_sg;
	data->_XXDTA.zei_flag = data->LINE_CND[ZEI_PN].INP_sg;

	// イメージ再表示
//	dsp_tky( _getLINE_DATA(APPEND_LINE), _getINP_CTL( APPEND_LINE, TKY_PN ));

	m_DtLine.SetDtListData(0, &data->_XXDTA);
}


#if (INPLINE_DEBUG ==1)
void CDBDinpView::inpline_dspTrace(struct _DATA_LINE *dtline, int ln, int line/* = 0*/, char* func/*= NULL*/, char* fname/* = NULL*/)
{
	MyTrace("◆◆◆ inpline_dspTrace ln = %d, L:%d, Func(= %s), File = %s\n", ln, line, func, fname);
#else
void CDBDinpView::inpline_dsp(struct _DATA_LINE *dtline, int ln)
{
#endif

	DTLIST_DATA data;
	data = dtline->_XXDTA;
/*
#ifdef _DEBUG
	data.dd_dbmn_etc = "ABCDEFGHIJKLMNOPQRSTU";
	data.dd_dkno_etc = "あいうえおかきくけこさし";
	data.dd_deb_etc = "ほほほAあいうえおかきくこ";
#endif
*/
	if( ln == APPEND_LINE ) {
		data.dd_ctbl[DATA_SEQ].back = RGB_DARK_GREEN;
		data.dd_ctbl[DATA_SEQ].text = RGB_WHITE;
	}
	else {
		if( ! BmonKoji_DebCre_Inp(ln) ) {
			if( ! data.dd_cbmn_etc.IsEmpty() && data.dd_dbmn_etc.IsEmpty() )
				data.dd_ctbl[BMON_PN].text = data.dd_ctbl[CBMN_PN].text;

			if( ! data.dd_ckno_etc.IsEmpty() && data.dd_dkno_etc.IsEmpty() )
				data.dd_ctbl[KOJI_PN].text = data.dd_ctbl[CKJI_PN].text;
		}

		int rno = dtline->ZREC_NO - 1;

		CString stmp;
		if( rno >= 0 ) {
			if( ZREC[rno].m_icno > 0 ) {
				DenpTypeString( ZREC[rno].m_dtype, stmp );
			}
		}
		if( stmp.IsEmpty() ) {
			::ZeroMemory( (dtline)->Denp, sizeof (dtline)->Denp );
			data.dd_udtbl[0].userdata = NULL;
		}
		else {
			strcpy_s( (dtline)->Denp, sizeof (dtline)->Denp, stmp );
			data.dd_udtbl[0].userdata = (BYTE*)(dtline)->Denp;
		}
	}

	data.val_flag = dtline->LINE_CND[VAL_PN].INP_sg;
	data.zei_flag = dtline->LINE_CND[ZEI_PN].INP_sg;

	/* 非課税有価証券譲渡仕訳 */
	if( (dtline->_XXDTA.zei_flag & 0x10) )
			data.zei_flag |= 0x10;
	else	data.zei_flag &= ~0x10;

	data.dd_tek.image = 0;
	data.dd_tek.width = 0;
	data.dd_tek.height = 0;

//	MyTrace( "dtline->ImgSEQ = %d\n", dtline->ImgSEQ );

	if( dtline->LINE_CND[ TKY_PN ].IMG_sg == 1 && dtline->ImgSEQ ) {
		// イメージファイル
		CString filter;
		filter.Format( "iseq = '%d'", dtline->ImgSEQ );

//		if( pDBzm->image->Requery( filter ) == 0 ) {
		if( pDBzm->image && pDBzm->image->SetAbsolutePosition( dtline->ImgSEQ ) == 0 ) {
			IMREC* pIMREC = pDBzm->image->DecodeImage();

			if( pIMREC != NULL ) {
				data.dd_tek.width = pIMREC->dotxs;
				data.dd_tek.height = pIMREC->dotys;
				data.dd_tek.image = (char*)pIMREC->imdata;
			}
			else {
				data.dd_tek.tekiyo = Image;
				data.dd_tek.width	= 0;
				data.dd_tek.height	= 0;
				data.dd_tek.image	= NULL;
			}
		}
		else {
			data.dd_tek.tekiyo = Image;
		}
	}

	if( ln != APPEND_LINE ) {
		if( m_InsLine != -1 ) {
			if( ln == m_InsLine ) {
				data.dd_seq = pDBzm->m_lastdata.m_seq;

				data.dd_ctbl[DATA_SEQ].back	= RGB_YELLOW;
				data.dd_ctbl[DATA_SEQ].text	= RGB_BLACK;
				data.dd_dsign |= 0x1000;

				m_DtList.SetCurrentLine( ln );
			}
		}
//MyTrace( "inpline_dsp ln-1(%d), Denp = %s(%08x), dd_udtbl(%s, %08x)\n", ln-1, dtline->Denp, dtline->Denp, data.dd_udtbl[0].userdata, data.dd_udtbl[0].userdata );

		m_DtList.SetDtListData( ln-1, &data, FALSE );
		// 別ウィンドウの入力にもセット

	}
	else {
		m_DtLine.SetDtListData( 0, &data );
	}

}




//
//	新規入力時に、前行の枝番取得
//
int CDBDinpView::GetZengyoBrcode( int dc_sw )
{
	int ln = SCROLL_NL();	//最終行を取得
	int pn;
	int brn = -1;
	
	struct _DATA_LINE *dline = _getLINE_DATA( ln );
	CDBINPDataRec* dt;

	pn = !dc_sw ? DEBT_PN : CRED_PN;

	if( dline->ZREC_NO > 0 ) {
		dt = &ZREC[ dline->ZREC_NO - 1 ];

		brn = !dc_sw ? dt->m_dbr : dt->m_cbr;
	}

	return brn;
}


//フォーカスポイントまで自動入力
BOOL CDBDinpView::is_AutoInp_use()
{
	return m_AutoInp_use;
}


// 同一行チェック
int CDBDinpView::is_linechk( int ln )
{	
	return( ln == get_nowln() ? 0 : 1 );
}


//*************************************************
//	データ行の移動
//	返送値	-1 ... 正常処理ができなかった
//			 0 ... 正常終了(行の変更の有無は関係なく)
//*************************************************
int CDBDinpView::_dline_chg( CWnd* pwnd, int ln, int pn, int focs_sw/* = 0 */ )
{
static unsigned seq = 0;
int ret;
struct _DATA_LINE *dtline;
int i;
int prev_ln = get_nowln();
BOOL	bLineChange = FALSE;

DLINETrace(	"_dline_chg TOP, prev_ln = %d, ln = %d\n", prev_ln, ln );
DLINETrace("@_dline_chg CREC.m_dsign[6] = %02x\n", CREC.m_dsign[6] );

	if( prev_ln != APPEND_LINE )
	{
		// 入力タイプセーブ
		SaveInpType( INP_mode, prev_ln );
		// デフォルト枝番入力タイプセット
		dtline = getLINE_DATA();
		dtline->LINE_CND[ DEBT_PN ].INP_type = brn_dfltinptype( dtline->_XXDTA.dd_dbr );
		dtline->LINE_CND[ CRED_PN ].INP_type = brn_dfltinptype( dtline->_XXDTA.dd_cbr );
	}

#ifdef _SLIP_ACCOUNTS_
	if( ln != prev_ln ){
		if( IsDispSlip() ){

			if( prev_ln != APPEND_LINE ){
				// 関連対象有り？
				int cnt = (int)m_ContDocseqAry.GetCount();

				if( cnt ){
					if( m_ImgDtseq == -1 )	m_ImgDtseq = CREC.m_seq;

					CREC.m_dsign[6] |= 0x01;
					dtline->_XXDTA.dd_dsign |= 0x800;
					m_pImgmng->ConnectDataToImgFile( pDBzm, dtline->_XXDTA.dd_seq, m_ContDocseqAry );
					m_ContDocseqAry.RemoveAll();
					m_pImgmng->DeleteAllChildImgDlg();
					if( m_DImgdlgType == ID_DBDINP_ADDLINKDLG_TYPE ){
						ChangeImgdlgType( ID_DBDINP_CONFIRMDLG_TYPE );
						if( m_DispSlipChk.GetCheck() == 0 ){
							HideImgDlg();
						}
					}
					else{
						if( m_DispSlipChk.GetCheck() == 0 ){
							ChangeImgdlgType( ID_DBDINP_CONFIRMDLG_TYPE );
							HideImgDlg();
						}
					}
				}
				else{
					if( m_DispSlipChk.GetCheck() == 0 ){
						ChangeImgdlgType( ID_DBDINP_CONFIRMDLG_TYPE );
						HideImgDlg();
					}
				}
			}
			else if( prev_ln == APPEND_LINE ){
				// 確定前にフォーカスが外れた時には、リンク予定情報はクリアする
				m_ContDocseqAry.RemoveAll();
			}
		}
		dtline = getLINE_DATA();
		dtline->LINE_CND[ZEI_PN].KEY_inp = 0;
	}
#endif

	if( prev_ln == APPEND_LINE && ln != prev_ln ){
		m_InsLine = -1;
		//修正行へ
		SaveDinpDataNOW_PN( _APPEND );
	}
DLINETrace(	"_dline_chg ln = %d, pn = %d, focs_sw = %d\n", ln, pn, focs_sw );

	switch( ret = __dline_chg( pwnd, ln, pn, focs_sw ) )
	{
	case 1:	// データ行の移動有り
//		// 欄外イメージ表示
//		big_img( dtline, getINP_CTL( TKY_PN ));
		ret = 0;
		bLineChange = TRUE;
		break;
	}

	// 欄外イメージ表示
	dtline = getLINE_DATA();
	if( dtline->_XXDTA.dd_seq == seq )
	{
		// データ行の移動無
		if( (i = get_nowln()) != APPEND_LINE )
		{
			// 入力タイプリセット
			SetInpType( INP_mode );
		}
	}
	else
	{
//		_getLINE_DATA( prev_ln )->LINE_CND[ TKY_PN ].CSLpos = 0;	// 摘要ポジションリセット

		seq = dtline->_XXDTA.dd_seq;
		big_img( dtline, getINP_CTL( TKY_PN ));

		m_SET_SYZINP_PN = -1;	// 消費税ダイアログフォーカスポジション
	}

	// 選択背景色
	// 仕訳属性表示
	OnLineattr_dsp();

	//消費税
	if( bLineChange ) {
		m_bSyzYunyu = FALSE;
		DB_SyIniz( &CREC );

		if( pn == DEBT_PN || pn == CRED_PN ) {
			SelWndShow( FALSE );
		}
	}

	if( ln != prev_ln ) {
		if( pn == DEBT_PN || pn == CRED_PN ) {
			Dsp_TkyToItem(TRUE);
		}
		m_bSyzSotoMen = FALSE;
		m_pnZeiSotoMen = 0;
	}


#ifdef _SLIP_ACCOUNTS_	// '12.10.03
	if( ln != prev_ln ){
		if( ln == APPEND_LINE ){
			if( m_DispSlipChk.GetCheck() ){
#ifndef _SCANNER_SV_
				if( IsDispSlip()==FALSE ){
					if( m_pImgmng->GetImgDlgType() == 0 ){
						m_NowImgSeq = m_pImgmng->GetDocseqOnImgDlg();
					}
					m_pImgmng->DispImgDlgOff(); 

					IMG_DLGOPTION	dlgOption;
					memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
					if( m_NowImgSeq != -1 ){
						dlgOption.imgDispSw |= 0x01;
						dlgOption.DispImgSeq = m_NowImgSeq;
					}
					dlgOption.btnDispSw |= (0x01|0x02|0x04|0x08);
					if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK ){
						dlgOption.btnDispSw |= (0x10|0x20);
					}
					m_pImgmng->DispImgDlgOn( this, dlgOption );
					m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
					m_DImgdlgType = ID_DBDINP_INPUTDLG_TYPE;
					ChgLinkBtnState( ID_LINKON_BSTATE );
					m_Imgcnt = 0;
					m_ImgDtseq = -1;

				}
				else if( IsDispSlip()==TRUE ){
					ChgLinkBtnState( ID_LINKON_BSTATE );
				}
#else
				if( m_pImgmng->GetImgDlgType() == 0 ){
					m_NowImgSeq = m_pImgmng->GetDocseqOnImgDlg();
				}

				IMG_DLGOPTION	dlgOption;
				memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
				if( m_NowImgSeq != -1 ){
					dlgOption.imgDispSw |= 0x01;
					dlgOption.DispImgSeq = m_NowImgSeq;
				}
				dlgOption.btnDispSw |= (0x01|0x02|0x04|0x08);
				if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && ! IsJzSansyo() ){
					dlgOption.btnDispSw |= (0x10|0x20);
				}
		//		dlgOption.btnHideSw |= 0x01;
				m_pImgmng->DispImgDlgOn( this, dlgOption );
				m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
				m_DImgdlgType = ID_DBDINP_INPUTDLG_TYPE;
				ChgLinkBtnState( ID_LINKON_BSTATE );
				m_Imgcnt = 0;
				m_ImgDtseq = -1;

//				set_focus( get_nowpn() );
#endif
			}
		}
		else{
			if( m_DispSlipChk.GetCheck() ){
				if( IsDispSlip()==TRUE ){
					ChgLinkBtnState( ID_LINKON_BSTATE );
				}
			}
		}
	}
#endif

	return ret;
}

//
//	focs_sw -- マウスクリック時は 0
//
// 返送値	-1 ... 正常処理ができなかった
//			 1 ... 行の変更をした
//			 0 ... 行変更はなかった
int CDBDinpView::__dline_chg( CWnd* pwnd, int ln, int pn, int focs_sw )
{
	int save_brn_sel;
	struct _TKYBRN_SEL save_sel;
	int ret = 0;
	int mv_rval = -1;	// 行移動をしなかった場合は、0にする
	int i, st;
	int mode_chg = FALSE;

DLINETrace( "__dline_chg now= %d, ln = %d, DATA_1RD = %d, focs_sw(%d)\n", get_nowln(), ln, DATA_1RD, focs_sw );
	// ビューが異なる
	if( ((CMainFrame*)GetDBDinpMainFrame())->IsSCANMDFY() && ((CMainFrame*)GetDBDinpMainFrame())->NowView() != INP_VIEW ) 
		return 0;

	//修正データの戻し
	if( DATA_1RD )
	{
	//	memmove( (char *)&ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1], (char *)&CREC, sizeof( struct _DATAREC ) );
	//	ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1] = CREC;
		CRECtoZREC();

		if( INP_mode == _KEYSCAN )
		{
			if( m_pScanView->scan_zrec_set( &CREC, SCROLL_NL() - get_nowln() ) == -1 )
				return -1;
		}
	}

	if( ln != 1 && ln != SCROLL_NL() )	//先頭行でも最下行でもなく
		if( !is_linechk( ln ) )			//前回行と同じ行の場合は
			return(0);					//行チェンジは行わない

LOOP:
	//to 入力ライン[確定マスターでない]
	if( ln == APPEND_LINE && ! IsMAsterKAKUTEI() )
	{
		struct _InpCtl* pCtl = _getINP_CTL( ln, pn );

		if( pCtl->INP_ena == FALSE )
		{
			set_focus( get_nowpn() );
			return(-1);
		}
		if( DATA_1RD )
		{
			//CREC to 残高ADD.
			st = rewupdat( &CREC, &SCAN_rec, INP_mode );
			if( rewupdat_job( st ) ) {
				return -1;
			}
		//	memmove( (char *)&ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1], (char *)&CREC, sizeof( struct _DATAREC ) );
		//	ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1] = CREC;
			CRECtoZREC();
			DATA_1RD = 0;
		}

		// Dinp Para INP_mode 分 save
		SaveDinpPara( INP_mode );

		inpmdisp( _APPEND );

		// Dinp Para _APPEND mode リセット
		ReSetDinpPara( _APPEND );

		// 摘要枝番関係セーブ
		save_brn_sel = BRNTKY_sel;
		save_sel = TKYBRN_SEL;

		//
		endscan();
		// 摘要かな検索 をしていたら、元に戻す
		if( pDBzm->tkrec_kanamode() ) {
		//	pDBzm->tkrec->Requery( "", 0 );
		//	pDBzm->tkrec_kanamode() = FALSE;
			tkrec_queryAll();
		}

		if( m_InsLine == -1 ) {
			//ZREC ReSet
		//	_fillbuf( (char *)&ZREC[0], sizeof( ZREC ), 0 );
			for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
				ZREC[i].Reset();
			}

			CDBINPDataRec key;

			key.m_seq = APPEND_SEQ.ngetseq( SEQ_NO - 1);
			if( key.m_seq <= 0 ) key.m_seq = USERS_SEQ;
	//		scandat_getn( &ZREC[0], &key, SCROLL_NL() );
			scaninsdat_getn( &ZREC[0], -1, SCROLL_NL() );

			scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );
			D_SEQN = SCROLL_NL()-1;
		}
		else {
			SREC = m_InsSREC;
		}
		m_bDscroll = TRUE;
		set_nowln( ln );

		if( m_InsLine == -1 ) {
			ddata_dsp(-1);	
		}

		//消費税
//		pDBsy->Sy_iniz( &CREC, 0 );
		DB_SyIniz( &CREC );

		if( get_nowpn() == TKY_PN )	{
			// 07.04 /08
			rentky_srdsp(CREC.m_dbt,CREC.m_cre);
		}
		else {
			SelDspRst( INP_mode );
		}

		// 枝番摘要画面リセット
//		BRNTKY_sel_set( save_brn_sel );
		TKYBRN_SEL = save_sel;
		brntky_rst();

		//直前のサンプル仕訳 SREC の修正	//04.07 /98
		set_SREC( &SREC, &ZREC[0], DDATA_LINE, SCROLL_NL(), getLINE_DATA(), INP_mode );
#ifdef IMPORT	// 10.14 /98
		if( NXTDTA_sg )
			set_SREC( &NXT_rec2, &ZREC[0], DDATA_LINE, SCROLL_NL(), getLINE_DATA(), INP_mode );
#endif
		sel_shiftjob(ln, 0, FALSE);
//		set_nowpn( pn );
		RDRemainDispJOB();

		if( focs_sw )	// click 時 は除く
		{
			set_focus( get_nowpn() );
			return(-1);
		}
	}
	else if( ln != APPEND_LINE )	//修正ライン
	{

		if( ln > SCROLL_NL() )	//オーバーフロー
		{
			// 03.11 /10 移動
			if( DATA_1RD )
			{
				st = rewupdat( &CREC, &SCAN_rec, INP_mode );
				if( rewupdat_job( st ) ) {
					return -1;
				}
				//修正データの戻し
				ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1] = CREC;
				if( INP_mode == _KEYSCAN )
				{
					if( m_pScanView->scan_zrec_set( &CREC, SCROLL_NL() - get_nowln() ) == -1 )
						return -1;
				}
				DATA_1RD = 0;
			}

			i = dscroll_up(2);

			if( !i )
			{
				if( INP_mode == _KEYSCAN || IsMAsterKAKUTEI() ) // 確定マスターは、入力ラインなし
					return 0;

				ln = APPEND_LINE;
				goto LOOP;
			}
			else if( i == 1 )
			{
				set_nowln( SCROLL_NL() );
			}
			else	//i == 2
			{
				set_nowln( SCROLL_NL() - 1);
			}

//			set_nowpn( pn );
		}
		else if( ln < 0 )	//アンダーフロー
		{
			// 03.11 /10 移動
			if( DATA_1RD )
			{
				st = rewupdat( &CREC, &SCAN_rec, INP_mode );
				if( rewupdat_job( st ) ) {
					return -1;
				}
				//修正データの戻し
				ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1] = CREC;
				if( INP_mode == _KEYSCAN )
				{
					if( m_pScanView->scan_zrec_set( &CREC, SCROLL_NL() - get_nowln() ) == -1 )
						return -1;
				}

				DATA_1RD = 0;
			}

			i = dscroll_down(2);

			if( !i )
			{
				return(0);
			}
			else if( i == 1 )
			{
				set_nowln( 1 );
			}
			else	//i == 2
			{
				set_nowln( 2 );
			}

//			set_nowpn( pn );
		}
		else if( DDATA_LINE[ln - 1].ZREC_NO )
		{
		//	struct _InpCtl* pCtl = _getINP_CTL( ln, pn );
			struct _InpCtl* pCtl = _getINP_CTL( pwnd, ln, pn );

		//	if( DDATA_CTL[ln-1][pn].INP_ena == FALSE )
			if( pCtl->INP_ena == FALSE )
			{
				set_focus( get_nowpn() );
				return(-1);
			}

			BOOL	bBmnChg = FALSE;

			if( INP_mode == _APPEND )	//to 修正ライン
			{
				intscan();

				inpmdisp( _MODEFY );

				mode_chg = TRUE;

				//Dinp Para _APPEND mode save	03.17 /98
				SaveDinpPara( _APPEND );

				//Dinp Para _MODEFY mode initial & set;
				InizDinpPara( _MODEFY );
				ReSetDinpPara( _MODEFY );

				// 標準科目名称
//				TKDsw = 1;	//ダミー
			}
			else if( DATA_1RD && is_linechk( ln ) )
			{
				int bmn1, bmn2;
				bmn1 = CREC.m_dbmn;
				bmn2 = CREC.m_cbmn;
				CString kno1, kno2;
				kno1 = CREC.m_dkno;
				kno2 = CREC.m_ckno;

				st = rewupdat( &CREC, &SCAN_rec, INP_mode );
				if( rewupdat_job( st ) ) {
					return -1;
				}
				if( bmn1 != CREC.m_dbmn || bmn2 != CREC.m_cbmn ) {
					bBmnChg = TRUE;
					rewupdat_disp();
				}

				if( kno1 != CREC.m_dkno || kno2 != CREC.m_ckno ) {
					bBmnChg = TRUE;
					rewupdat_kojidisp();
				}
				rewupdat_syzdisp();
				//修正データの戻し
				ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1] = CREC;
				if( INP_mode == _KEYSCAN )
				{
					if( m_pScanView->scan_zrec_set( &CREC, SCROLL_NL() - get_nowln() ) == -1 )
						return -1;
				}
				DATA_1RD = 0;
			}

			if( ln == 1 /*&& !focs_sw*/ )
			{
				if( dscroll_down(1) ) {
					set_nowln( 2 );
					set_focus( pn );
				}
				else {
					set_nowln( ln );
					mv_rval = 0;

					// dataupd() でメッセージボックス表示
					if( ((CMainFrame*)GetDBDinpMainFrame())->MsgBoxFlag() ) {
						((CMainFrame*)GetDBDinpMainFrame())->MsgBoxFlag() = FALSE;
						if( ! focs_sw ) {
							set_focus( pn );
						}
					}
				}
			}
			else if( ln == SCROLL_NL() /*&& !focs_sw*/ )
			{
				if( dscroll_up(1) ) {
					m_bDscroll = TRUE;
					set_nowln( SCROLL_NL() - 1 );

					if( !(IsDispSlip() && pn==TKY_PN) )
						set_focus( pn );
				}
				else {
					set_nowln( ln );
					mv_rval = 0;

					// dataupd() でメッセージボックス表示
					if( ((CMainFrame*)GetDBDinpMainFrame())->MsgBoxFlag() ) {
						((CMainFrame*)GetDBDinpMainFrame())->MsgBoxFlag() = FALSE;
						if( ! focs_sw ) {
							set_focus( pn );
						}
					}
				}
			}
			else
			{
				set_nowln( ln );
				if( bBmnChg && ! focs_sw ) {
					set_focus( pn );
				}
				else {
					// dataupd() でメッセージボックス表示
					if( ((CMainFrame*)GetDBDinpMainFrame())->MsgBoxFlag() ) {
						((CMainFrame*)GetDBDinpMainFrame())->MsgBoxFlag() = FALSE;
						if( ! focs_sw ) {
							set_focus( pn );
						}
					}
				}
				//ここは行変更なので間違いと思う 2018年から有効
				//mv_rval = 0;
			}
//			set_nowpn( pn );

		}
		else
		{
			set_focus( get_nowpn() );
			return(-1);
		}

		if( !DATA_1RD )
		{
			//修正データ to CREC
		//	memmove( (char *)&CREC, (char *)&ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1], sizeof( struct _DATAREC ) );
			CREC = ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1];
			//残高 SUB. from CREC
			reupdat( &CREC );

			DATA_1RD = 1;

//			RTpage = 1;	//#* 03.14 /02 ページリセット

			if( get_nowpn() == TKY_PN )
			{
				//連想摘要検索＆表示
				rentky_srdsp(CREC.m_dbt,CREC.m_cre);
			}
		}
	}

	sel_lineupdate();
DLINETrace( "__dline_chg [END] ln = %d, mv_rval = %d\n", get_nowln(), mv_rval );

	return (mv_rval == 0) ? 0 : 1;	//	return(0);
}


/***********************************
	データ行１行移動
************************************/
long CDBDinpView::dline_chg( CWnd* pwnd, long nChar )
{
	int ln;

	switch( nChar )
	{
		case VK_END:	//修正の終了
			if( IsMAsterKAKUTEI() )	return( nChar );
			else {
				if( INP_mode != _KEYSCAN && get_nowln() != APPEND_LINE )
					ln = APPEND_LINE;	//入力行に戻る
				else
					return( nChar );
			}
			break;

		case VK_UP:	//１行戻る
			if( get_nowln() == APPEND_LINE )	//入力行
			{
				if( m_InsLine != -1 )
					return -1;

				ln = SCROLL_NL();
				m_bAppendUpkey = TRUE;
			}
			else if( get_nowln() > 1 )
			{
				ln = get_nowln() - 1;
			}
			else
				ln = -1;
			break;

		case VK_DOWN:	//１行進む
			if( get_nowln() == APPEND_LINE )
				return(-1);	//入力行以上先へは行けない
			else
				ln = get_nowln() + 1;
			break;

		default:
			return( nChar );
			break;
	}
	// ここで現在の表示行の ターミネーション0を呼び出す。
/**/m_DtList.SetFocus();
//	c_set( IDC_ICSDISPCTRL10 );

	int ret = _dline_chg( pwnd, ln, get_nowpn(), 1 );

	return( ret );
}

/*****************************************
	data table scrol_nl 分 scroll down
******************************************/
int CDBDinpView::dscroll_down( int scrol_nl, int scrol_bar )
{
	int i,j,sw;
	int ln_out = 0;
	int loop;
	int ln,skip = 0;
	int seq,seq2,n;
	CDBINPDataRec dta, *rec1, *rec2, key;

//FILETrace("@dscroll_down %d,%d\n", scrol_nl, scrol_bar );

	if( !DDATA_LINE[0].ZREC_NO )
	{
		if( !scrol_bar )
			return(0);
		else if( !DDATA_LINE[SCROLL_NL()-2].ZREC_NO )
			return(0);
		else
			skip = 1;
	}

	ln = get_nowln();

	//スキャンモード
	if( INP_mode == _KEYSCAN )
	{
		for( sw = 1 ; sw ; sw = 0 )	//１度のみの実行
		{
			if( skip )
			{
				if( !DDATA_LINE[SCROLL_NL()-2].ZREC_NO )	//最下行に１行しかない場合
					break;								//このループ内の処理はスキップする。
			}

			loop = m_pScanView->scan_data_rd( &ZREC[ SCROLL_NL() - 1], SCROLL_NL(), -scrol_nl );
			if( !loop )
				break;
			else if( m_pScanView->scan_get_rdatan() < SCROLL_NL() )
			{
				if( !scrol_bar )
				{
					//データテーブルリセット
					m_pScanView->scan_data_rd( &ZREC[ SCROLL_NL() - 1], SCROLL_NL(), loop );
					loop = 0;
					break;
				}
				skip = 1;
			}

			//仕訳表示エリアN行ダウン
			scroll_down( DDATA_LINE, SCROLL_NL(), loop );

			//N行表示
			for( i = 0, j = SCROLL_NL() - 1 ; i < m_pScanView->scan_get_rdatan() ; ++i, --j )
			{
				//既に表示されているデータはデータレコードポインタのみ付け替え
				if( DDATA_LINE[j].ZREC_NO )
					DDATA_LINE[j].ZREC_NO = j + 1;
				else if( !skip ) 
				{
					recdsp( &ZREC[0], j, DDATA_LINE, j );
				}
			}

			if( ln + loop <= SCROLL_NL() )
				ln += loop;
			else
			{
				ln = SCROLL_NL();

				ln_out = 1;
			}
			break;
		}
	}
	else
	{
		for( loop = 0 ; loop < scrol_nl ; ++loop )
		{
			if( skip )
			{
				if( !DDATA_LINE[SCROLL_NL()-2].ZREC_NO )
					break;
			}
			else	//skip == 0
			{
				n = DDATA_LINE[0].ZREC_NO - 1;	//ZREC[n]

				seq = ZREC[n].m_seq;	//そのシーク番号
				rec1 = &ZREC[n];

				if( n > 0 )
					n--;
				else	// n == 0
					n = RECMAX - 1;
				seq2 = ZREC[n].m_seq;	//一つ前のシーク番号
				rec2 = &ZREC[n];
				//まだ読み込まれていないZREC[n]か，リングバッファの底か
				if( seq2 <= 0 || dbip_data_cmp( 0, rec1, rec2 ) < 0 /*seq2 > seq*/ )
				{
					key = *rec1;
					int ins_pos		= INSPOS_SEQ.ngetseq( key.m_seq-1 );

			//		if( ( key.m_seq -1 ) <= 0 )
					if( (ins_pos - 1) <= 0 )
						if( !scrol_bar )
							break;	//return(0);
						else
						{
							skip = 1;
							goto SCRL_DWN;
						}
					if( !data_nxt_back( &key, &dta, 1 ) ) {
						if( !scrol_bar )
							break;	//return(0);
						else
						{
							skip = 1;
							goto SCRL_DWN;
						}
					}
					ZREC[n] = dta;
				}
			}
SCRL_DWN:
			//仕訳表示エリア１行ダウン
			scroll_down( DDATA_LINE, SCROLL_NL(), 1 );

			//１行表示
			if( !skip )
			{
				recdsp( &ZREC[0], n, DDATA_LINE, 0 );
			}

			if( ln != APPEND_LINE )	//入力ライン以外
				if( ln < SCROLL_NL() )
					++ln;
				else if( !ln_out )
				{
					ln_out = 1;
				}
		}
	}
	if( !loop )
		return(0);

	//修正データの戻し
	if( !ln_out && DATA_1RD )
	{
	//	memmove( (char *)&ZREC[DDATA_LINE[ln-1].ZREC_NO - 1], (char *)&CREC, sizeof( struct _DATAREC ) );
		ZREC[DDATA_LINE[ln-1].ZREC_NO - 1] = CREC;
		if( INP_mode == _KEYSCAN )
		{
			if( m_pScanView->scan_zrec_set( &CREC, SCROLL_NL() - ln ) == -1 )
				return 0;
		}
	}

	//仕訳表示エリア表示
	ddata_dsp(1);

	//計算等
	if( ln_out )
	{
		if( DATA_1RD )
		{
			rewupdat( &CREC, &SCAN_rec, INP_mode );
			DATA_1RD = 0;
		}
		
		if( !DATA_1RD )
		{

			//連想摘要検索＆表示
			if( get_nowpn() == TKY_PN )
				rentky_srdsp(CREC.m_dbt, CREC.m_cre);
		}
	}

	//フォーカス位置
	i = get_nowpn();
	set_nowln( ln );
	set_nowpn(i);
//	set_focus( get_nowpn() );

	return(loop);
}

/***********************************
	data table 1line scroll up
************************************/
int CDBDinpView::dscroll_up( int scrol_nl )
{
	int i,j,sw;
	int ln_out = 0;
	int loop;
	int ln,nn;
	int seq,seq2,n;
	CDBINPDataRec dta, *rec1, *rec2, key;

//FILETrace("@dscroll_up = %d\n", scrol_nl );

	if( !DDATA_LINE[SCROLL_NL()-1].ZREC_NO )
		return(0);

	ln = get_nowln();
	if( INP_mode == _KEYSCAN )
	{
		for( sw = 1 ; sw ; sw = 0 )
		{
			loop = m_pScanView->scan_data_rd( &ZREC[ SCROLL_NL() - 1], SCROLL_NL(), scrol_nl );
			if( !loop )
				break;

			//仕訳表示エリアN行アップ
			scroll_up( DDATA_LINE, SCROLL_NL(), loop );

			//N行表示
			for( i = 0, j = SCROLL_NL() - 1 ; i < m_pScanView->scan_get_rdatan() ; ++i, --j )
			{
				//既に表示されているデータはデータレコードポインタのみ付け替え
				if( DDATA_LINE[j].ZREC_NO ) {
					DDATA_LINE[j].ZREC_NO = j + 1;
				}
				else
				{
					recdsp( &ZREC[0], j, DDATA_LINE, j );
				}
			}

			if( ln - loop > 0 )
				ln -= loop;
			else
			{
				ln = 1;
				ln_out = 1;
			}
			break;
		}
	}
	else
	{
		for ( loop = 0 ; loop < scrol_nl ; ++loop )
		{
			n = DDATA_LINE[SCROLL_NL()-1].ZREC_NO - 1;	//ZREC[n]

			seq = ZREC[n].m_seq;	//そのシーク番号
			rec1 = &ZREC[n];
			if( n < RECMAX - 1 )
				n++;
			else	// n == RECMAX - 1
				n = 0;
			seq2 = ZREC[n].m_seq;	//一つ先のシーク番号
			rec2 = &ZREC[n];
			//まだ読み込まれていないZREC[n]か，リングバッファのTOPか
			if( seq2 <= 0 || dbip_data_cmp( 0, rec1, rec2 ) > 0 /*seq2 < seq*/ )
			{
				key = *rec1;
				nn = data_nxt_for( &key, &dta, 1 );
			//	key.m_seq++;
			//	nn = data_for( &key, &dta, 1 );

				if( !nn )
				{
					break;
//							ln = 0;
//							pn = get_nowpn();
//							return(_dline_chg( pwnd, ln, pn) );
				}
		//		memmove( &ZREC[n], &dta, sizeof( struct _DATAREC ) );
				ZREC[n] = dta;
			}
			//仕訳表示エリア１行アップ
			scroll_up( DDATA_LINE, SCROLL_NL(), 1 );

			//１行表示
			recdsp( &ZREC[0], n, DDATA_LINE, SCROLL_NL() - 1 );

			if( ln != APPEND_LINE )	//入力ライン以外
				if( ln > 1 )
					--ln;
				else if( !ln_out )
				{
					ln_out = 1;
				}
		}
	}
	if( !loop )
		return(0);

	//修正データの戻し
	if( !ln_out && DATA_1RD )
	{
//		memmove( (char *)&ZREC[DDATA_LINE[ln-1].ZREC_NO - 1], (char *)&CREC, sizeof( struct _DATAREC ) );
		ZREC[DDATA_LINE[ln-1].ZREC_NO - 1] = CREC;
		if( INP_mode == _KEYSCAN )
		{
			if( m_pScanView->scan_zrec_set( &CREC, SCROLL_NL() - ln ) == -1 )
				return 0;
		}
	}

	//仕訳表示エリア表示
	ddata_dsp(-1);

	//計算等
	if( ln_out )
	{
		if( DATA_1RD )
		{
			rewupdat( &CREC, &SCAN_rec, INP_mode );
			DATA_1RD = 0;
		}
		
		if( !DATA_1RD )
		{
			//連想摘要検索＆表示
			if( get_nowpn() == TKY_PN )
				rentky_srdsp(CREC.m_dbt, CREC.m_cre);

		}
	}

	//フォーカス位置
	i = get_nowpn();
	set_nowln( ln );
	set_nowpn(i);
//	set_focus( get_nowpn() );


	return(loop);
}


/***********************************
	write data check
************************************/
int CDBDinpView::Wdata_ok()
{
	if( /*getLINE_DATA()->LINE_CND[DATE_PN].INP_sg &&*/
		getLINE_DATA()->LINE_CND[DEBT_PN].INP_sg &&
		getLINE_DATA()->LINE_CND[CRED_PN].INP_sg &&
		getLINE_DATA()->LINE_CND[VAL_PN].INP_sg )
		return 1;
	else if( /*!getLINE_DATA()->LINE_CND[DATE_PN].INP_sg &&*/
		!getLINE_DATA()->LINE_CND[DEBT_PN].INP_sg &&
		!getLINE_DATA()->LINE_CND[CRED_PN].INP_sg &&
		!getLINE_DATA()->LINE_CND[VAL_PN].INP_sg )
		return 0;
	else
		return 2;
}


//---------------------------------------
//	他入力画面からの 追加仕訳チェック
//
//---------------------------------------
int CDBDinpView::AppendWdata_ok()
{
	struct _DATA_LINE* dtline;

	dtline = _getLINE_DATA( APPEND_LINE );

	if( /*dtline->LINE_CND[DATE_PN].INP_sg &&*/
		dtline->LINE_CND[DEBT_PN].INP_sg &&
		dtline->LINE_CND[CRED_PN].INP_sg &&
		dtline->LINE_CND[VAL_PN].INP_sg )
		return 1;
	else if( /*!getLINE_DATA()->LINE_CND[DATE_PN].INP_sg &&*/
		!dtline->LINE_CND[DEBT_PN].INP_sg &&
		!dtline->LINE_CND[CRED_PN].INP_sg &&
		!dtline->LINE_CND[VAL_PN].INP_sg )
		return 0;
	else
		return 2;
}


/****************************************************************
概要：	write & next data set
引数：
		endapp = 1	EndApp() からの最終仕訳書き込み呼び出し
返送値：
	-2 ... MAXまで登録済み
	-1 ... Error,
	 1 ... 次仕訳あり
	 0 ... OK
*****************************************************************/
int CDBDinpView::WandNextData( int endapp /*=0*/ )
{

	static int		_2Write;

	BOOL fstwrt = FALSE;
	int ret;
	int wcnt;

	int nxt = 0;
	short i = 0;

	m_NOBRNCHK_CND	= 0;	// 枝番未入力チェック状態サインイニシャライズ
	m_BMKJICHK_CND	= 0;

	m_TabKeyStop = 0;

	//自動仕訳用
	CDBINPDataRec atrec, tmpcrec;
	tmpcrec = CREC;

	/* 1 ﾚｺｰﾄﾞ ｴﾝﾄﾞ */
	_2Write = 1;

	if( INP_mode == _APPEND ) {
//		PRtlen = TKJUK.kj_inpl;

		CREC.m_seq = REAL_SEQ_NO + 1;

		SREC = CREC;
	}

	for( wcnt = 0 ; _2Write-- ; wcnt++ ) {

		if( INP_mode == _APPEND ) {	/* ﾆｭｳﾘｮｸ */
			CREC.m_dsign[0] &= 0xfe;

 			BOTOM_SEQ = ++SEQ_NO;
			CREC.m_seq = ++REAL_SEQ_NO;

			/* 伝票番号 自動加算	*/
			if( pAUTOSEL->DENP_OPT == OPT_AUTO/*1*/ && CREC.m_cno != -1 )	{
				CREC.m_cno = DENP_NO;
				DENP_NO++;
			}
			else if( pAUTOSEL->DENP_OPT == OPT_NON ) {
//				CREC.m_cno = -1;
			}
			else CREC.m_cno = SREC.m_cno;
		}

		/*　枝番登録有り？	*/
		if( INP_mode ) {
			if( brchk1( &CREC, 0 ) || brchk1( &CREC, 1 ) ) {
				inpmdisp( INP_mode );

				set_focus( DATE_PN );	//goto	DATE10;
#ifdef IMPORT	// 10.05 /98
				return 0;
#else
				return;
#endif
			}
		}

		ret = dataupd( &CREC, &SCAN_rec, INP_mode, 0, this );

		if( INP_mode == _APPEND )
		{
			if( ret == 1 ) {
				// MAXまで登録
				return -2;
			}
			else if( ret != -1 )
			{
				/* 伝票番号 = SEQ No.	*/
				if( pAUTOSEL->DENP_OPT == OPT_EQU /*2*/ )
				{
					// リライトデータのセーブ
					RWREC[wcnt] = CREC;
				}
			}
		}

		if( fstwrt == FALSE )
		{
			if( INP_mode == _APPEND )
				SREC.m_seq = CREC.m_seq;	// 直前のＳＥＱ番号セット

			fstwrt = TRUE;
		}

		//諸口・現金貸借
		if( INP_mode == _APPEND )
		{
			syog_genk( &CREC, _ADD );
		}

		if( INP_mode ) {	/* Scan mode */
#ifdef IMPORT	// 10.05 /98
			return 0;
#endif
		}

		if( INP_mode == _APPEND )
		{
			/* 伝票番号 == SEQ No.	*/
			if( pAUTOSEL->DENP_OPT == OPT_EQU )
				;	// 伝票番号 == SEQ No. の場合はあとで表示を行う。
			else
			{
				/* ﾃﾞｰﾀ ｾｰﾌﾞ & ﾋｮｳｼﾞ */
				if( ++D_SEQN >= RECMAX ) D_SEQN = 0;
				ZREC[D_SEQN] = CREC;
				datadsp( D_SEQN, 1, m_InsLine );
			}
		}
	}

	if( INP_mode == _APPEND )
	{
		/* 伝票番号 = SEQ No.	*/
		if( pAUTOSEL->DENP_OPT == OPT_EQU )
		{
			int n;
			int scan_requery = 0;

			for( n = 0 ; n < wcnt ; ++n )
			{
				SCAN_rec = RWREC[n];
				RWREC[n].m_cno = RWREC[n].m_seq;

				// データリライト
				if( SCAN_rec != RWREC[n] ) {
					if( m_InsLine != -1 && m_InsMode == _KEYSCAN ) {
						scan_requery++;
					}

					if( scan_requery ) {
						pDBzm->dbdata_scan_insert() = TRUE;
					}

					int c_ret;
					c_ret = pDBzm->DB_DataCorrect( &RWREC[n] );
					if( c_ret == ERR )
					{
						ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
						ret = -1;
					}
					else if( c_ret == -2 ) {
						PROGRAM_OK = FALSE;
						
						int st = myICSMessageBox( (LPCTSTR)pDBzm->GetZmErrMsg() );
					
						PROGRAM_OK = TRUE;
					}

					if( scan_requery ) {
						pDBzm->dbdata_scan_insert() = FALSE;
					}
				}

				if( scan_requery ) {
					CString stmp;
					DB_DataScan( stmp );
				}

				/* ﾃﾞｰﾀ  ｾｰﾌﾞ & ﾋｮｳｼﾞ */
				if( ++D_SEQN >= RECMAX ) D_SEQN = 0;
				ZREC[D_SEQN] = RWREC[n];
				datadsp( D_SEQN, 1, m_InsLine );
			}
		}
	}


	if( INP_mode == _APPEND && !RemainRdata() && !endapp ) {
		//自動仕訳（社会福祉・宗教)
		if (KiriSwk(&atrec, &tmpcrec)) {
			if (!nxt) {
				nxt = 1;
				atrec.m_seq = REAL_SEQ_NO + 1;

				NXT_rec = atrec;
				SetKriMode(FALSE);
			}
		}
	}

	// 04.03 /03
	if( INP_mode == _APPEND ) {
		// 諸口ブザーの元仕訳チェック
		set_syobz_data( &CREC );

		// 定型仕訳 最後の書き込み
		datetekiyo_check_tkrec(&CREC);
	}

#ifdef IMPORT	// 10.02 /98
	if( !nxt )
	{
		if( NXTDTA_sg )
		{
			if( NXT_rec.m_dbt == SREC.m_dbt && NXT_rec.m_cre == SREC.m_cre )
			{
				SREC.m_seq = NXT_rec2.m_seq;

				SREC.m_dbt = NXT_rec2.m_dbt;
				SREC.m_dbr = NXT_rec2.m_dbr;
				SREC.m_cre = NXT_rec2.m_cre;
				SREC.m_cbr = NXT_rec2.m_cbr;
				memmove( SREC.m_val, NXT_rec2.m_val, sizeof(SREC.m_val));
				SREC.m_tekiyo = NXT_rec2.m_tekiyo;
			}

			NXTDTA_sg = FALSE;
		}
#endif
	/* ｽｲｯﾁ & ﾊﾞｯﾌｧ ｲﾆｼｬﾗｲｽﾞ */
	int		ddate, cno;
	BYTE	ksign, mofs;
	ddate = CREC.m_ddate;
	cno = CREC.m_cno;
	ksign =	CREC.m_ksign;
	mofs  =	CREC.m_mofs;

	CREC.Reset();
	CREC.m_ddate = ddate;
	CREC.m_cno = cno;
	CREC.m_ksign = ksign;
	CREC.m_mofs = mofs;

	REAL_SEQ_NO = pDBzm->m_lastdata.m_seq -1 ;
	CREC.m_seq = REAL_SEQ_NO + 1;

#ifdef IMPORT	// 10.02 /98
	}
	else
	{
		NXT_rec2 = SREC;		// SREC save

		CREC = NXT_rec;
		SREC = NXT_rec;
		//#* 05.08 /02 ... 金額コピーのため
		memmove( SREC.m_val, NXT_rec2.m_val, sizeof(SREC.m_val));
	}
#endif

	TKattr2 = TKattr;

	TKattr = 0;

	if( SREC.m_dbmn == -1 ) {
		if( m_sdbtBmn != -1 ) {
			SREC.m_dbmn = m_sdbtBmn;
		}
	}
	if( SREC.m_cbmn == -1 ) {
		if( m_screBmn != -1 ) {
			SREC.m_cbmn = m_screBmn;
		}
	}

//	SaveDinpDataSREC( _APPEND );
	SaveDinpParaSREC( _APPEND );

//	pDBsy->Sy_iniz( &CREC, 0 );
	DB_SyIniz( &CREC );

#ifdef IMPORT	// 10.05 /98
	if( ret )
		return ret;
	else
		return nxt;
#endif
}


/*-------------------------------------------------------------------
概要：仕訳データのDBへの追加・更新
		実際の追加更新は、rewupdat2()内でdataupd()を呼ぶ

引数：	CDBINPDataRec *dta	追加・更新する仕訳
		CDBINPDataRec *moto	更新前の仕訳
		int mode			入力モード
		BOOL brtorok		TRUE：枝番残高登録処理をする
		BOOL bmchk			TRUE：部門チェックを行うか？
	
  -------------------------------------------------------------------*/
int CDBDinpView::rewupdat( CDBINPDataRec *dta, CDBINPDataRec *moto, int mode, BOOL brtorok/*= FALSE*/, BOOL bmchk/*=TRUE*/ )
{
int st;

	p_DBDinpView->m_NOBRNCHK_CND = 0;	// 枝番未入力チェック状態サインイニシャライズ
	m_BMKJICHK_CND	= 0;

	m_TabKeyStop	= 0;

	if( ! modefy_ok( mode, dta ) )
		return 0;

	PROGRAM_OK = FALSE;	// TRUEには、rewupdat2()で戻す。

	if( brtorok )
	{
		BRTORK_DATA.rewupdat_sg = TRUE;
		BRTORK_DATA.svINP_mode = mode;
		BRTORK_DATA.CREC = *dta;
		BRTORK_DATA.SCAN_rec = *moto;

		CString kmkcd;

		if( !(dta->m_dsign[0] & 0x01) )
		{
			// 借方
			st = brchk1( dta, 0 );
			if( st == 99 )
			{
				st = brzan_torok( this, dta->m_dbmn, dta->m_dbt, dta->m_dbr, 0 );		//枝番登録選択
				if( st == -1 )					//キャンセル
					del_dtaeda( dta, 0 );		//仕訳枝番消去
				else if( st == 0 ) {
					if (pDBzm->IsDiffEdabanSyzSgn((char*)(LPCTSTR)dta->m_dbt, dta->m_dbr)) {
						pDBsy->SyReOpen(pDBzm);
					}
					m_TaiTtl.init_taiclq();
				}
			}

			// 貸方
			st = brchk1( dta, 1 );
			if( st == 99 )
			{
				st = brzan_torok( this, dta->m_cbmn, dta->m_cre, dta->m_cbr, 1 );		//枝番登録選択
				
				if( st == -1 )					//キャンセル
					del_dtaeda( dta, 1 );		//仕訳枝番消去
				else if (st == 0) {
					if (pDBzm->IsDiffEdabanSyzSgn((char*)(LPCTSTR)dta->m_dbt, dta->m_dbr)) {
						pDBsy->SyReOpen(pDBzm);
					}
					m_TaiTtl.init_taiclq();
				}
			}
		}
	}

	int ret = rewupdat2( dta, moto, mode, brtorok, bmchk );

	return( ret );
}

/*-------------------------------------------------------------------
概要：	仕訳データのDBへの追加・更新
引数：	rewupdat と同じ
返送値：０＝OK
  -------------------------------------------------------------------*/
int CDBDinpView::rewupdat2( CDBINPDataRec *dta, CDBINPDataRec *moto, int mode, BOOL brtorok, BOOL bmchk/*=TRUE*/ )
{
#ifdef _SLIP_ACCOUNTS_
	if( IsDispSlip() ){
		// 関連対象有り？
		int cnt = (int)m_ContDocseqAry.GetCount();

		if( cnt ){
			CREC.m_dsign[6] |= 0x01;

			m_pImgmng->ConnectDataToImgFile( pDBzm, CREC.m_seq, m_ContDocseqAry );
			m_ContDocseqAry.RemoveAll();
			m_pImgmng->DeleteAllChildImgDlg();
			if( m_DImgdlgType == ID_DBDINP_ADDLINKDLG_TYPE ){
				ChangeImgdlgType( ID_DBDINP_CONFIRMDLG_TYPE );
				if( m_DispSlipChk.GetCheck() == 0 ){
					HideImgDlg();
				}
			}
			else{
				if( m_DispSlipChk.GetCheck() == 0 ){
					ChangeImgdlgType( ID_DBDINP_CONFIRMDLG_TYPE );
					HideImgDlg();
				}
			}
		}
		else{
			if( m_DispSlipChk.GetCheck() == 0 ){
				ChangeImgdlgType( ID_DBDINP_CONFIRMDLG_TYPE );
				HideImgDlg();
			}
		}
	}
#endif

	int ret = 0;
	//表示行数変更処理中でない
	if( !m_bDWP_LINECOUNT ) {
		ret = dataupd( dta, moto, mode, bmchk, this );
		if( ret == -2 ) {
			m_DCrct = *dta;
			PostMessage( WM_CORRECTMSG, 0, 0 );
		}
	}

	BRTORK_DATA.rewupdat_sg = FALSE;

	PROGRAM_OK = TRUE;

	return ret;
}


// データが変更可能か？
BOOL CDBDinpView::dinp_modefy_ok( struct _DATA_LINE *dtline )
{
//FILETrace( "dinp_modefy_ok TOP dtline->_XXDTA.dd_seq(%d), CREC.m_seq(%d)\n", 
//			dtline->_XXDTA.dd_seq, CREC.m_seq );

	if( dtline->_XXDTA.dd_seq == CREC.m_seq ) {
		return modefy_ok( INP_mode, &CREC );
	}
	else {
ASSERT( dtline->_XXDTA.dd_seq == CREC.m_seq );
		// レコードテーブルより
		if( dtline->ZREC_NO > 0 ) {
			int idx = dtline->ZREC_NO - 1;
			
			return modefy_ok( INP_mode, &ZREC[ idx ] );
		}
	}

	return FALSE;
}


// 取消データチェック
BOOL CDBDinpView::isdeldata()
{
	return _isdeldata( get_nowln() );
}

BOOL CDBDinpView::_isdeldata( int ln )
{
	if( ln && DDATA_LINE[ ln - 1 ].ZREC_NO )
		return ZREC[DDATA_LINE[ ln - 1 ].ZREC_NO - 1].m_dsign[0] & 0x01;
	return FALSE;
}


/***********************************
	摘要関係表示から科目関係表示に
************************************/
void CDBDinpView::Dsp_TkyToItem( BOOL bUpdate/*=FALSE*/ )
{
	if( m_SelDispPN == SL_TKY_PN || m_SelDispPN == SL_TKYKANA_PN || bUpdate ) {
		SelPar par;
		par.sel_pn = SL_KAMOKU_PN;
		m_Seldata.SelectJob( &par, -1 );
	}
}



//連想摘要検索＆表示
int CDBDinpView::rentky_srdsp( CString debt, CString cred )
{
	SelPar par;

#ifndef _SLIP_ACCOUNTS_
	par.sel_pn = SL_TKY_PN;
	par.dbt	= debt;
	par.cre = cred;
	::ZeroMemory( par.tky_kana, sizeof par.tky_kana );

	m_Seldata.SelectJob( &par, -1 );
#else
	if( IsDispSlip() == FALSE ){
		par.sel_pn = SL_TKY_PN;
		par.dbt	= debt;
		par.cre = cred;
		::ZeroMemory( par.tky_kana, sizeof par.tky_kana );

		m_Seldata.SelectJob( &par, -1 );
	}
	else{
		// 原票からのイメージ摘要抽出を一時停止
		par.sel_pn = SL_TKY_PN;
		par.dbt	= debt;
		par.cre = cred;
		::ZeroMemory( par.tky_kana, sizeof par.tky_kana );

		m_Seldata.SelectJob( &par, -1 );
	}
#endif

	return 0;
}




// 連想摘要マックスページを取得
int CDBDinpView::RTpmax_Get()
{
ASSERT( FALSE );
	return 0;
}

/*===============================
	枝番摘要toデータ摘要
================================*/
BOOL CDBDinpView::brnitm_to_tky( int ln, int bmn, CString& code, int brn )
{
	int dc_sw, tno;
	CDBipTKREC tkrec;

	if( brn == -1 )	return FALSE;

	// 枝番摘要を検索
	if( BRmst )
	{
		// 枝番to摘要レコードリード
		int bmncd = -1;
		if( pAUTOSEL->BMNTKBR_OPT == OPT_CHKON )
			bmncd = bmn;

		if( tno = m_BrTek.get_brtek( bmncd/*bmn*/, code, brn ) )
		{
			if( tno > 0 ) {
				if( pDBzm->DB_TekiyoNoRead( &tkrec, tno ) == ERR )
					return TRUE;

				//摘要がすでに含まれているか？
				if( IsIncludeString( CREC.m_tekiyo, tkrec.m_tkname ) )
					return TRUE;

				//摘要toデータ
				dc_sw = tky_select_to_drec( &CREC, &tkrec );
			}
			else {
				tkrec.Reset();
				if( bmncd != -1 ) {
					// 部門枝番残高本体に登録された枝番名称
					CDBINPDataRec tmpRec;
					tmpRec.m_dbmn = bmncd;
					tmpRec.m_dbt = code;
					tmpRec.m_dbr = brn;

					if( pDBzm->DB_BmnEdabanMasterSearch(&tmpRec, 0) != 0 )
						return TRUE;
					tkrec.m_tkname = pDBzm->bmezrec->ezname;
				}
				else {
					// 枝番残高本体に登録された枝番名称
					if( pDBzm->DB_EdabanZanSearch(code, brn) != 0 )
						return TRUE;
					tkrec.m_tkname = pDBzm->ezrec->ezname;
				}
				dc_sw = 0;

				//摘要がすでに含まれているか？
				if( IsIncludeString( CREC.m_tekiyo, tkrec.m_tkname ) )
					return TRUE;
			}
			//表示
			tky_select_dsp( &CREC, dc_sw, &tkrec, FALSE );
			//データセットのためのダミーフォーカスＯＦＦ
			TerminationIcsinputTky( this, ln, 0, 0, 0 );

			return TRUE;
		}
	}
	return FALSE;
}


//　その他項目コントロールデータレコードをセット
void CDBDinpView::set_etcdsp( int ln, int pn )
{
struct _DATA_LINE *data;

	if( pn == DEBT_PN ) 
	{
		data = _getLINE_DATA( ln );
		_set_etcdsp( data, pn, data->LINE_CND );
	}
	else if( pn == CRED_PN ) 
	{
		data = _getLINE_DATA( ln );
		_set_etcdsp( data, pn, data->LINE_CND );
	}
	else if( pn == BMON_PN || pn == CBMN_PN || pn == KOJI_PN || pn == CKJI_PN ) 
	{
		// 部門名称 or 工事名称
		data = _getLINE_DATA( ln );

		_set_bmndsp( data, pn );	

		if( (pn == BMON_PN || pn == KOJI_PN) && ! BmonKoji_DebCre_Inp( ln ) ) {
			int pn2;
			pn2 = pn == BMON_PN ? CBMN_PN : CKJI_PN;
			_set_bmndsp( data, pn2 );	
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////
// ICSDBEdt に セットする

long CDBDinpView::exDBdata_set( int pn, int id, VARIANT* data, int type, int bcdlen )
{
	if( get_nowpn() != pn ) {
		data->pbVal = NULL;
		return 0;
	}

	return DBdata_set( this, id, data, type, bcdlen );
}

long CDBDinpView::exDBdata_get( int pn, int id, VARIANT* data, int type, int bcdlen )
{
	if( get_nowpn() != pn ) {
		data->pbVal = NULL;
		return 0;
	}

	return DBdata_get( this, id, data, type, bcdlen );
}

void CDBDinpView::exDBSetDispString( int pn, int id, LPCTSTR string )
{
	if( get_nowpn() != pn )
		return;

	DBSetDispString( this, id, string );
}

int CDBDinpView::exDBkamokudata_get( int pn, int id, struct _KamokuData* data )
{
	if( get_nowpn() != pn )
		return 0;

	return DBkamokudata_get( this, id, data, pDBzm );
}

int CDBDinpView::exDBkamokudata_set( int pn, int id, struct _KamokuData* data )
{
	if( get_nowpn() != pn )
		return 0;

	return DBkamokudata_set( this, id, data, pDBzm );
}


//
// ＳＥＱ番号Ｆｏｃｕｓメイン
#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsSeq( CWnd* pwnd, int ln, short click )	
{
	_FocusIcsSeq( pwnd, ln, click );	

	OnICSInputFocus( SEQ_PN );
}
void CDBDinpView::_FocusIcsSeq( CWnd* pwnd, int ln, short click )	
{
#else
void CDBDinpView::FocusIcsSeq( CWnd* pwnd, int ln, short click )	
{
#endif

	if( PROGRAM_OK == FALSE )
		return;

	try
	{

#ifdef ONFORK	// 02.28 /01
		if( INITIAL_SW )
		{
			INITIAL_SW = FALSE;
		}
#endif

		//別画面入力消去
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			if( click & 0x10 )	nChar = VK_SHIFT;
			if( click & 0x20 )	nChar = VK_CONTROL;

			click_seljob( ln, nChar );
		}

		if( _dline_chg( this, ln, SEQ_PN, click ? 0 : 1 ) == -1 )
			return;

		if( ln == APPEND_LINE )
		{
//			GetDlgItem( _getINP_CTL( SCROLL_NL, SEQ_PN )->IDC_X )->PostMessage( WM_LBUTTONDOWN );
//			GetDlgItem( _getINP_CTL( SCROLL_NL, SEQ_PN )->IDC_X )->PostMessage( WM_LBUTTONUP );
			return;
		}

		if( SAVE_PN == -1 )
			SAVE_PN = SEQ_PN;

		SelWndShow( FALSE );

		set_nowpn( SEQ_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

// seq ターミネーション
//
// ＳＥＱ番号Ｔｅｒｍｉｎａｔｉｏｎメイン
void CDBDinpView::TerminationIcsinputSeq( CWnd* pwnd, long nChar, long inplen, long kst )
{
	if( PROGRAM_OK == FALSE )
		return;

//	VARIANT var;
//	char buf[60];
	int		zrec_no;
	BYTE	sign;

	try
	{
		int ln = get_nowln();
		if( ln == APPEND_LINE )
			return;

		// ＳＥＱ指定によるデータ呼出し
		if( seq_to_data( ln, nChar ) )
			return;

	//	DBdata_set( getINP_CTL( SEQ_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	//	getLINE_DATA()->_XXDTA.dd_seq = atoi( var.pbVal );
		
		if( !nChar )
			return;

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return;

		if( nChar )
		{
			if( nChar == VK_UP || nChar == VK_DOWN ) {
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			nChar = dline_chg( this, nChar );
			if(	nChar == -1 )
			{
				return;
			}
			else if( !nChar )
			{
				if( !isdeldata() && SAVE_PN != SEQ_PN && SAVE_PN != -1 ) {
					set_focus( SAVE_PN );
				}
				else {
					set_focus( SEQ_PN );
				}
				return;
			}
		}

		if( nChar == VK_DELETE && dinp_modefy_ok( getLINE_DATA() ) ) 
		{
			if( CREC.m_dsign[0] & 0x01 )
			{
				if( !isDataDelOK( 1 ) )
					return;
				CREC.m_dsign[0] &= 0xfe;	// 取消サインＯＦＦ
			}
			else
			{
				if( !isDataDelOK( 0 ) )
					return;

				CREC.m_dsign[0] |= 0x01;	// 取消サインＯＮ
			}
			SREC.m_dsign[0] = (SREC.m_dsign[0] & 0xfe) | (CREC.m_dsign[0] & 0x01);

			//諸口・現金貸借
			if( CREC.m_dsign[0] & 0x01 )
			{
				getLINE_DATA()->_XXDTA.dd_dsign |= 0x01;
				syog_genk( &CREC, _SUB );
			}
			else
			{
				getLINE_DATA()->_XXDTA.dd_dsign &= ~0x01;
				syog_genk( &CREC, _ADD );
			}

			// サインセット
			zrec_no = DDATA_LINE[ get_nowln() - 1 ].ZREC_NO;
			ZREC[ zrec_no - 1].m_dsign[0] = (ZREC[zrec_no - 1].m_dsign[0] & 0xfe) | (CREC.m_dsign[0] & 0x01);

			getLINE_DATA()->LINE_CND[SEQ_PN].INP_sg = TRUE;

#ifdef LATER_CLOSE
			// 選択背景色
			select_line_col( get_nowln() );
			// 仕訳属性表示
			OnLineattr_dsp();
#endif
			dataupd( &CREC, &SCAN_rec, INP_mode, 0, this );;
			SCAN_rec = CREC;

			inpline_dsp( getLINE_DATA(), get_nowln() );
			m_DtList.RedrawDataListCtrl();

			//デフォルトポジションにセット
			if( ! DenpTypeMode() ) {
				if( !(CREC.m_dsign[0] & 0x01) && SAVE_PN != SEQ_PN && SAVE_PN != -1 )
					set_focus( SAVE_PN );
			}
		}
		else if( nChar == VK_HOME ) {
			// 仕訳を確定する --- VER2 ---
			BOOL bUpdate = FALSE;
			int st;

			if( (Voln1->tl_cor & 0x10) && GET_EntrySign() == 0 && !IsJzSansyo() ) {	// マスター全体 未確定・上手君でない・参照モードでもない

				// 確定仕訳が修正可 のとき
				if( IsModifyConfirmData() ) {
					// 確定・転送済みサインOFF

					PROGRAM_OK = FALSE;

					if( CREC.m_dsign[7] & 0x30 ) {
						st = myICSMessageBox("確定仕訳を未確定にしますか？",MB_YESNO|MB_DEFBUTTON2,0,0,this);
						if( st == IDYES ) {
							CREC.m_dsign[7] &= ~0x30;
							getLINE_DATA()->_XXDTA.dd_dsign &= ~0x0A;

							bUpdate = TRUE;
						}

						set_focus( SEQ_PN );
					}
				//	else {
				//		st = myICSMessageBox("現在の仕訳を確定しますか？",MB_YESNO,0,0,this);
				//		if( st == IDYES ) {
				//			CREC.m_dsign[7] |= 0x10;
				//			getLINE_DATA()->_XXDTA.dd_dsign |= 0x02;
				//			bUpdate = TRUE;
				//		}
				//	}

					PROGRAM_OK = TRUE;
				}
				else {
					if( ! IsEnkakuUser() && ! IsJozuEnvCloudMaster() ) {

						if( !(CREC.m_dsign[7] & 0x30) ) {
							PROGRAM_OK = FALSE;

							st = myICSMessageBox("現在の仕訳を確定しますか？",MB_YESNO|MB_DEFBUTTON2,0,0,this);
							if( st == IDYES ) {
								CREC.m_dsign[7] |= 0x10;
								getLINE_DATA()->_XXDTA.dd_dsign |= 0x02;

								bUpdate = TRUE;
							}
							PROGRAM_OK = TRUE;

							set_focus( SEQ_PN );
						}
					}
				}

				if( bUpdate ) {
					zrec_no = DDATA_LINE[ get_nowln() - 1 ].ZREC_NO;
					sign = (ZREC[zrec_no - 1].m_dsign[7] & ~0x10) | (CREC.m_dsign[7] & 0x10);
					ZREC[ zrec_no - 1].m_dsign[7] = sign;
				
					getLINE_DATA()->LINE_CND[SEQ_PN].INP_sg = TRUE;
					dataupd( &CREC, &SCAN_rec, INP_mode, 0, this );

					inpline_dsp( getLINE_DATA(), get_nowln() );
					m_DtList.RedrawDataListCtrl();

					//仕訳確定したので、原票変更不可
					if( IsDispSlip() ) {
						if( (CREC.m_dsign[6]&0x01) ) {
							if( m_pImgmng->GetImgDlgType() == 0 ){
								m_NowImgSeq = m_pImgmng->GetDocseqOnImgDlg();
							}

							IMG_DLGOPTION	dlgOption;
							memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
							if( m_NowImgSeq != -1 ){
								dlgOption.imgDispSw |= 0x01;
								dlgOption.DispImgSeq = m_NowImgSeq;
							}
							dlgOption.btnDispSw |= (0x01|0x02|0x04|0x08);
							if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
								dlgOption.btnDispSw |= (0x10|0x20);
							}
					//		dlgOption.btnHideSw |= 0x01;
							m_pImgmng->DispImgDlgOn( this, dlgOption );
							m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
							m_DImgdlgType = ID_DBDINP_INPUTDLG_TYPE;
							ChgLinkBtnState( ID_LINKON_BSTATE );
							m_Imgcnt = 0;
							m_ImgDtseq = -1;
						}
						else {
							m_pImgmng->DispImgDlgOff();
							ChgLinkBtnState( ID_DISPSLIP_BSTATE );
						}
					}
				}
			}
		}
		else if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
		{
			if( isdeldata() )
			{
				nChar = dline_chg( this, VK_DOWN );
				if(	nChar == -1 )
				{
					return;
				}
				else if( !nChar )
				{
					if( !isdeldata() && SAVE_PN != SEQ_PN && SAVE_PN != -1 ) {
						set_focus( SAVE_PN );
					}
					else {
						set_focus( SEQ_PN );
					}
					return;
				}
			}
			else {
			//	set_focus( DATE_PN );
				set_termfocus( ln, SEQ_PN, 1 );
			}
		}
		else if( nChar == VK_INSERT ) {
			// 仕訳を挿入する
			if( ! isInsertMaster() ) {
				set_focus( SEQ_PN );
				return;
			}

			if( (Voln1->tl_cor & 0x10) && IsMasterType(MST_INSERT) && !IsJzSansyo() ) {	// マスター全体 未確定
				int li, idx;
				m_InsSeq = -1;

				for( li = 1; li <= ln; li++ ) {

					if( li == ln ) {
						idx = DDATA_LINE[li-1].ZREC_NO -1;

						if( idx >= 0 ) {
							m_InsSeq = ZREC[idx].m_seq;
						}
						break;
					}
				}

				if( m_InsSeq != -1 ) {
					m_InsLine = ln;
					if( INP_mode == _KEYSCAN ) {
						m_InsMode = _KEYSCAN;

						KeyscanInsertJob();
					}
					else {
						m_InsMode = _MODEFY;

						modefy_insline_dispchg();
					}
				}
			}
		}
		else {
			if( nChar != 0x16 )
				Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}

}

//
// ＳＥＱ指定によるデータ呼出し
BOOL CDBDinpView::seq_to_data( int ln, long nChar )
{
	VARIANT var;
	char	buf[64];
	int	seq;
	PROGRAM_OK = FALSE;

	if( nChar == VK_TAB || nChar == VK_RETURN )
	{
		// 入力データ取出し
		exDBdata_get( SEQ_PN, _getINP_CTL( ln, SEQ_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		seq = atoi( (char*)var.pbVal );

		// データ呼出し
		if( seq_to_data_move( ln, seq, _getLINE_DATA(ln)->_XXDTA.dd_seq ) )
		{
			PROGRAM_OK = TRUE;
			return TRUE;
		}
	}
	// 表示データのリセット
//	data_set( _getINP_CTL( ln, SEQ_PN )->IDC_X, &_getLINE_DATA(ln)->SEQ_DATA, CICSInput );
	sprintf_s( buf, sizeof buf, "%d", _getLINE_DATA(ln)->_XXDTA.dd_seq );
	var.pbVal = (BYTE*)buf;
	exDBdata_set( SEQ_PN, _getINP_CTL( ln, SEQ_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

	PROGRAM_OK = TRUE;
	return FALSE;
}

//
// 検索画面よりのＳＥＱ指定
void CDBDinpView::SeqDataJob( int seq, int now_seq )
{
	PROGRAM_OK = FALSE;

	seq_to_data_move( get_nowln(), seq, now_seq );
	
	PROGRAM_OK = TRUE;

	m_bScanMode = FALSE;
}

//
// ＳＥＱ指定によるデータ呼出し(スレーブ)
BOOL CDBDinpView::seq_to_data_move( int ln, int seq, int now_seq )
{
//	int vect_n = seq - now_seq;
	int ins_pos		= INSPOS_SEQ.ngetseq( seq-1 );
	int ins_pos2	= INSPOS_SEQ.ngetseq( now_seq-1 );

	if( INP_mode == _KEYSCAN )
	{
		check_scan_insertquery();
		if( SCAN_PGTBL.sort_id != -1 ) {
			ins_pos = SCMDFY_SEQ.ngetseq( seq-1 );
			ins_pos2 = SCMDFY_SEQ.ngetseq( now_seq-1 );
		}
	}

	int vect_n = ins_pos - ins_pos2;
	int vect = vect_n < 0 ? -1 : 1;

	CWaitCursor wait;

//FILETrace( "seq_to_data_move\n" );

	if( vect_n )
	{
		//
		//修正データの戻し
		if( DATA_1RD )
		{
		//	memmove( (char *)&ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1], (char *)&CREC, sizeof( struct _DATAREC ) );
		//	ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1] = CREC;
			CRECtoZREC();

			if( INP_mode == _KEYSCAN )
			{
				if( m_pScanView->scan_zrec_set( &CREC, SCROLL_NL() - get_nowln() ) == -1 )
					return -1;
			}
		}

		//
		// 通常の修正
		if( INP_mode == _MODEFY )
		{
			return seq_to_modefy_data( ln, seq, now_seq, vect );
		}
		//
		// スキャンの修正
		else if( INP_mode == _KEYSCAN )
		{
			return seq_to_keyscan_data( ln, seq, now_seq, vect );
		}
		else
			return FALSE;
	}
	return FALSE;
}

//
//ＳＥＱ指定によるデータ呼出し(スレーブ:通常の修正)
BOOL CDBDinpView::seq_to_modefy_data( int ln, int seq, int now_seq, int vect )
{
	CDBINPDataRec data, key;
	int i,j, keyseq;
	int n;
//	int rno;

	key.m_seq = seq;;

	// 該当SEQ位置のデータをリード
	n = vect < 0 ? data_back( &key, &data, 1 ) : data_for( &key, &data, 1 );
	
//	MyTrace("%s, ln = %d, data取得ret = %d, data.m_seq = %d\n", __FUNCTION__, ln, n, data.m_seq);

	// オーバーｱｸｾｽの場合は先頭又は最終のデータをリード
	if( !n )
	{
		if( vect < 0 ) {
		//	key.m_seq = 1;
			key.m_seq = SEQ_INSPOS.ngetseq( 0 );	//最初のSEQ取得

			n = data_for( &key, &data, 1 );
		}
		else {
		//	key.m_seq = pDBzm->m_lastdata.m_seq-1;
			key.m_seq = SEQ_INSPOS.ngetseq( pDBzm->m_insCnt-1 );	//最初のSEQ取得

			n = data_back( &key, &data, 1 );
		}

		if( !n )
			return FALSE;
	}

	// 現在と同じデータの場合は何もしない
	if( now_seq == data.m_seq )
	{
		// １クリック
		return FALSE;
	}

	// 修正データリライト
	if( DATA_1RD )
	{
		//CREC to 残高ADD.
		rewupdat( &CREC, &SCAN_rec, INP_mode );
		DATA_1RD = 0;

		PROGRAM_OK = FALSE;
	}

	// 同じ表示位置とするために差分行分データをリード
	keyseq = data.m_seq;
	for( j = ln + 1; j <= SCROLL_NL() ; ++j )
	{
//		key.m_seq = keyseq+1;
		int pos = INSPOS_SEQ.ngetseq( keyseq -1 );
		pos++;
		key.m_seq = SEQ_INSPOS.ngetseq( pos-1 );	//次のSEQ取得

		n = data_for( &key, &data, 1 );

		if( !n ) {
			key.m_seq = keyseq;
			break;
		}
		keyseq = data.m_seq;
	}
	key.m_seq = keyseq;
	if( j < SCROLL_NL() )
		ln = ln + (SCROLL_NL() - (j-1));

	int btmseq = DDATA_LINE[SCROLL_NL() - 1]._XXDTA.dd_seq;

	//データディスプレイニシャライズ
	for( i = 0 ; i < DDATA_LNMAX; ++i )
	{
		inpline_inz( &DDATA_LINE[i] );
	}
	//データリード＆テーブル作成
	for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
		ZREC[i].Reset();
	}

//	scandat_getn( &ZREC[0], &key, SCROLL_NL() );
	scaninsdat_getn( &ZREC[0], key.m_seq, SCROLL_NL() );
	scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );
	D_SEQN = SCROLL_NL()-1;

	int btmln = SCROLL_NL();
	if( DDATA_LINE[btmln - 1]._XXDTA.dd_seq == seq ) {
		ln = btmln;
	}
	//行位置にデータがない場合は、次の行を探す
	int ln_chk = 0;

	if (DDATA_LINE[ln - 1]._XXDTA.dd_seq == 0) {

		for (int nxtln = ln + 1; nxtln <= SCROLL_NL(); nxtln++) {
			if (DDATA_LINE[nxtln -1]._XXDTA.dd_seq != 0) {
				ln = nxtln;
				ln_chk++;
				break;
			}
		}
	}
	else {
		ln_chk = 1;
	}
	//表示データがないので、もとの表示に戻す
	if (!ln_chk) {
		scaninsdat_getn(&ZREC[0], btmseq, SCROLL_NL());
		scandat_dspn(&ZREC[0], DDATA_LINE, SCROLL_NL());
		D_SEQN = SCROLL_NL() - 1;
		return FALSE;
	}

	set_nowln( ln );
	set_nowpn( SEQ_PN );

	//データテーブル表示
	_ddata_dsp( -1 );
#ifdef LATER_CLOSE
	//　入力ライン属性表示
	OnLineattr_dsp();
#endif

	//修正データ読み出し
	CREC = ZREC[DDATA_LINE[ ln - 1 ].ZREC_NO - 1];
	//残高 SUB. from CREC
	reupdat(&CREC);
	DATA_1RD = 1;

	_set_focus( ln, SEQ_PN );
	// １クリック
//	GetDlgItem( _getINP_CTL( ln, SEQ_PN )->IDC_X )->PostMessage( WM_LBUTTONDOWN );
//	GetDlgItem( _getINP_CTL( ln, SEQ_PN )->IDC_X )->PostMessage( WM_LBUTTONUP );

	return TRUE;
}

//
//ＳＥＱ指定によるデータ呼出し(スレーブ:スキャンの修正)
BOOL CDBDinpView::seq_to_keyscan_data( int ln, int seq, int now_seq, int vect )
{
	CDBINPDataRec data;
	int i;
	int n;
	int sft;

	while( m_pScanView->seq_data_rd( &data, seq, SCROLL_NL() - get_nowln(), vect ) )
	{
		if( data.m_seq == now_seq )	// 現在行と同じ場合はエラーとする
			break;

		// 修正データリライト
		if( DATA_1RD )
		{
			//CREC to 残高ADD.
			rewupdat( &CREC, &SCAN_rec, INP_mode );
			DATA_1RD = 0;

			PROGRAM_OK = FALSE;
		}

		//データディスプレイニシャライズ
		for( i = 0 ; i < DDATA_LNMAX ; ++i )
		{
			inpline_inz( &DDATA_LINE[i] );
		}

		//初期データリード＆テーブル作成
//		_fillbuf( (char *)&ZREC[0], sizeof( ZREC ), 0 );
		for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
			ZREC[i].Reset();
		}
		sft = SCROLL_NL() - ln;
		
		if( n = m_pScanView->scan_data_rd( &ZREC[ SCROLL_NL() - 1 ], SCROLL_NL(), sft ) )
		{
//			if( n != sft )
//				ln += n;
			if( n != sft ) {	//#* 03.08 /02
				for( i = SCROLL_NL()-1; i >= 0 && ZREC[i].m_seq ; --i ) {
					if( seq == ZREC[i].m_seq ) {
						ln = i+1;	break;
					}
				}
			}
		}
		else
		{
			m_pScanView->scan_data_rd( &ZREC[ SCROLL_NL() - 1 ], SCROLL_NL(), 0 );
			ln = SCROLL_NL();		//最下行にポジションセット
		}

		scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );
		//データテーブル表示
		_ddata_dsp( -1 );

		//修正データ初期読み出し
		CREC = ZREC[DDATA_LINE[ ln - 1 ].ZREC_NO - 1];
		//残高 SUB. from CREC
		reupdat(&CREC);
		DATA_1RD = 1;

		set_nowln( ln );
		set_nowpn( SEQ_PN );

		_set_focus( ln, SEQ_PN );
		// １クリック
//		GetDlgItem( _getINP_CTL( ln, SEQ_PN )->IDC_X )->PostMessage( WM_LBUTTONDOWN );
//		GetDlgItem( _getINP_CTL( ln, SEQ_PN )->IDC_X )->PostMessage( WM_LBUTTONUP );

		return TRUE;
	}
	// １クリック
//	GetDlgItem( _getINP_CTL( get_nowln(), SEQ_PN )->IDC_X )->PostMessage( WM_LBUTTONDOWN );
//	GetDlgItem( _getINP_CTL( get_nowln(), SEQ_PN )->IDC_X )->PostMessage( WM_LBUTTONUP );
	return FALSE;
}

////////////////////////////////////////////////////////////////////
// 日付

#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsDate( CWnd* pwnd, int ln, short click )
{
	_FocusIcsDate( pwnd, ln, click );

	OnICSInputFocus( DATE_PN );
}
void CDBDinpView::_FocusIcsDate( CWnd* pwnd, int ln, short click )
{
#else
void CDBDinpView::FocusIcsDate( CWnd* pwnd, int ln, short click )
{
#endif
	int nextpn;

	if( PROGRAM_OK == FALSE )
		return;

	try
	{

		//別画面入力消去
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}

		if( _dline_chg( pwnd, ln, DATE_PN, click ? 0 : 1  ) == -1 )
			return;

		// 取消データの場合はＳＥＱに
		if( isdeldata() )
		{
			SAVE_PN = DATE_PN;
			set_focus( SEQ_PN );
			return;
		}
		else {
			SAVE_PN = -1;
			if( data_denpdialog(ln) != 0 )
				return;
		}

		if( INITIAL_SW )
		{
			INITIAL_SW = FALSE;
			ddata_dsp( 1 );		// イメージ摘要のため再表示
		}

//		MyTrace("FocusICsDate = 1 INP_sg = %d\n", getLINE_DATA()->LINE_CND[DATE_PN].INP_sg);

		// 日付固定
		if( INP_mode == _APPEND && !IsJzSansyo() ) {
			if( getLINE_DATA()->LINE_CND[DATE_PN].INP_sg == FALSE ) {
				if( (nextpn = getAUTOSELpn(get_nowln(), DATE_PN)) != DATE_PN )
				{
					set_nowpn(DATE_PN);
					AutoInp(nextpn);
					set_focus(nextpn);
					return;
				}
			}
		}

		SelWndShow( FALSE );

		set_nowpn( DATE_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}

}


// 日付チェックデータ
long CDBDinpView::CheckDataIcsinputDate( CWnd* pwnd, long nChar ) 
{
	char check[3] = {0};
	char ymd[3] = {0};
	char data[20] = {0};
	BYTE bcd[10] = {0};
	VARIANT var;
	CString str;

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	//
	try
	{
	//	DBdata_get( pwnd, getINP_CTL(pwnd, DATE_PN)->IDC_X, &var, TYPE_BCD, 4 );
		exDBdata_get( DATE_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

		struct _DATA_LINE *ldata;
		int len;

		if( ! m_bMnthFix ) {
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
						ldata = getLINE_DATA();
						int sgn = KESY_SG_GET( ldata->BcdDate[0] );
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
							sprintf_s( data, sizeof data, "%02x", ldata->BcdDate[0] );
							break;
						}
						if( len == 1 ) {
							data[2] = '0';
							data[3] = var.pbVal[0];
						}
						else	strncpy_s( &data[2], sizeof(data)-2, (char*)var.pbVal, 2 );
						data[4] = 0;

						// 09.06 /07
						if( pDBzm->inpdate_shimechk( ldata->BcdDate, data ) != 0 )
							return -1;
					}
				}
			}
		}
		else {
			ldata = getLINE_DATA();
			int sgn = KESY_SG_GET( ldata->BcdDate[0] );
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
				sprintf_s( data, sizeof data, "%02x", ldata->BcdDate[0] );
				break;
			}
			if( var.pbVal != NULL ) {
				len = strlen( (char*)var.pbVal );
				if( len == 1 ) {
					data[2] = '0';
					data[3] = var.pbVal[0];
				}
				else	strncpy_s( &data[2], sizeof(data)-2, (char*)var.pbVal, 2 );
			}
			else {
				data[2] = data[3] = '0';
			}
			data[4] = 0;

			// 09.06 /07
			if( pDBzm->inpdate_shimechk( ldata->BcdDate, data ) != 0 )
				return -1;
		}

		if( DateInputCheck( data, bcd ) != 0 ) {
			// 不正データが入力された
			ldata = getLINE_DATA();
			if( ! m_bMnthFix ) {
				var.pbVal = ldata->BcdDate;
				exDBdata_set( DATE_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
			}
			else  {
				var.pbVal = &ldata->BcdDate[1];
				exDBdata_set( DATE_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 2 );
			}
			pDBzm->BcdDateToString( str, ldata->BcdDate, m_bMnthFix );
			exDBSetDispString( DATE_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, str );

			return -1;
		}
		else {
			memcpy( check, bcd, 3 );
		}

/*		if( !check[1] )
		{
			check[0] = 0;21
			check[1] = (char)0xff;
		}
*/
		CVolDateDB vd;
//		if( dinp_modefy_ok( getLINE_DATA() ) && !pDBzm->vd_chek( (unsigned char*)check ) && !pDBzm->vd_sign( (unsigned char*)check, (unsigned char*)ymd ) )
//		if( dinp_modefy_ok( getLINE_DATA() ) && !vd.db_vd_check( (BYTE*)check, pDBzm ) && !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd ) )
		if( dinp_modefy_ok( getLINE_DATA() ) && pDBzm->check_datelimit( (BYTE*)check ) && 
			!pDBzm->Myvd_chek( (BYTE*)check ) && !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd ) )
		{
			memmove( bcd, &ymd[0], 2 );

			if( ! m_bMnthFix ) {
				var.pbVal = bcd;
				exDBdata_set( DATE_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
			}
			else  {
				var.pbVal = &bcd[1];
				exDBdata_set( DATE_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 2 );
				// 09.06 /07
				ldata = getLINE_DATA();
				ldata->BcdDate[0] = bcd[0];
			}

//*DEBUG
//VARIANT dbgvar;
//exDBdata_get( DATE_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, &dbgvar, ICSDBEDT_TYPE_BCD, 4 );
//TRACE( "date %02x%02x", dbgvar.pbVal[0], dbgvar.pbVal[1] );
//			data_reply( pwnd, getINP_CTL(pwnd, DATE_PN)->IDC_X, 1, &ipdata );
		}
		else
		{
			Buzzer();
			memset( bcd, 0, sizeof bcd );
			var.pbVal = bcd;
			ldata = getLINE_DATA();

			exDBdata_set( DATE_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, m_bMnthFix ? 2 : 4 );
			pDBzm->BcdDateToString( str, ldata->BcdDate, m_bMnthFix );
			exDBSetDispString( DATE_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, str );
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

void CDBDinpView::TerminationIcsinputDate( CWnd* pwnd, long nChar, long inplen, long kst ) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
//	struct _ItemData check = {0};
//	INPUTDATA data;
	BYTE bcd[10] = {0};
	BYTE tmp[10] = {0};
	int date, ksgn, offset;
	VARIANT var;
	CString str;
	CVolDateDB vd;

	if( PROGRAM_OK == FALSE )
		return;

	if( ! nChar && m_bIgnoreDateTerm ) {
		m_bIgnoreDateTerm = FALSE;
		return;
	}

	try
	{
//		if( !nChar )	//フォーカスＯＦＦ
//		{
			if( !dinp_modefy_ok( getLINE_DATA() ) )
			{
				goto NEXT;
			}

			if( ! m_bMnthFix ) {
				exDBdata_get( DATE_PN, getINP_CTL( pwnd, DATE_PN )->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
				if( var.pbVal != NULL )	memcpy( bcd, var.pbVal, 3 );
			}
			else {
				exDBdata_get( DATE_PN, getINP_CTL( pwnd, DATE_PN )->IDC_X, &var, ICSDBEDT_TYPE_BCD, 2 );
				
				if( var.pbVal != NULL )	memcpy( &bcd[1], var.pbVal, 2 );
				bcd[0] = getLINE_DATA()->BcdDate[0];
			}

			// 月日 不正チェック用
			struct _DATA_LINE *ldata;
			ldata = getLINE_DATA();
			BYTE oldbcd[10] = {0};

			int old_date, old_ksign, old_mofs;
			old_date	= SREC.m_ddate;
			old_ksign	= SREC.m_ksign;
			old_mofs	= SREC.m_mofs;

			memcpy( oldbcd, ldata->BcdDate, sizeof ldata->BcdDate );

//FILETrace( "   bcd ---> %02x%02x%02x\n", bcd[0], bcd[1], bcd[2] );
//FILETrace( "oldbcd ---> %02x%02x%02x\n", oldbcd[0], oldbcd[1], oldbcd[2] );
//FILETrace( "srec date = %d", SREC.m_ddate );

			if( ! bcd[0] || ! bcd[1] ) {
				pDBzm->DateConv( bcd, &SREC, CV2_1 );
			}
			else pDBzm->DateConv( bcd, &SREC, CV1_2 );

//FILETrace( "srec date2 = %d", SREC.m_ddate );
		
			// CREC に SREC から 日付をセット
			if( !date_chk( nChar ) )
			{
				//消費税
				getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt( &CREC, NULL, INP_mode);
			}
			else
			{
				Buzzer();
				getLINE_DATA()->_XXDTA.dd_ddate = 0;
				getLINE_DATA()->_XXDTA.dd_mofs = 0;
				getLINE_DATA()->_XXDTA.dd_ksign = 0;
				memset( getLINE_DATA()->BcdDate, '\0', sizeof getLINE_DATA()->BcdDate );

				TerminationDataSet( DATE_PN );
			//	_fillbuf( (char *)&getLINE_DATA()->DATE_DATA, sizeof(getLINE_DATA()->DATE_DATA), 0 );
			//	pDBzm->DateConv( bcd, &getLINE_DATA()->DATE_DATA.IP_DAY, CV2_1 );
			//	exDBdata_set( pwnd, getINP_CTL( pwnd, DATE_PN )->IDC_X, &data );
				return;
			}

			// ８％を日付で判断して変更する
			memcpy( getLINE_DATA()->_XXDTA.dd_zei, CREC.m_zei, 6 );

			pDBzm->BcdDateToYmd( bcd, &date );
			ksgn = KESY_SG_GET (bcd[0]);
			memcpy( getLINE_DATA()->BcdDate, bcd, 2 );
			memcpy( tmp, bcd, 2 );

			switch( ksgn ) {
			case 1: tmp[0] = 0x61;	break;
			case 2: tmp[0] = 0x62;	break;
			case 3: tmp[0] = 0x63;	break;
			case 4: tmp[0] = 0x00;	break;
			default:
				break;
			}

			if( vd.db_vd_offset(tmp, &offset, pDBzm) == -1 ) {
//FILETrace( "tmp %02x%02x\n", tmp[0], tmp[1] );
//				PROGRAM_OK = FALSE;
//				ICSMessageBox( "月のオフセットを取得できません！ ");
//				PROGRAM_OK = TRUE;
//				pDBzm->DateConv( bcd, &SREC, CV2_1 );

// 月日 不正チェック用
FILETrace( "月のオフセットを取得できません！ tmp = %02x%02x%02x", tmp[0], tmp[1], tmp[2] );

				if( ! m_bMnthFix ) {
					var.pbVal = oldbcd;
					exDBdata_set( DATE_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
				}
				else  {
					var.pbVal = &oldbcd[1];
					exDBdata_set( DATE_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 2 );
				}

				memcpy( getLINE_DATA()->BcdDate, oldbcd, 2 );
				SREC.m_ddate = old_date;
				SREC.m_ksign = old_ksign;
				SREC.m_mofs = old_mofs;
				date_chk(0);

				set_focus( DATE_PN );
				return;
			}

			getLINE_DATA()->_XXDTA.dd_ddate = date;
			getLINE_DATA()->_XXDTA.dd_mofs = offset;
			getLINE_DATA()->_XXDTA.dd_ksign = (KESY_SG_GET (bcd[0]) == 0) ? 0 : 1;

			getLINE_DATA()->LINE_CND[DATE_PN].INP_sg = TRUE;
			if( ! m_bMnthFix ) {
				var.pbVal = bcd;
				exDBdata_set( DATE_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
			}
			else  {
				var.pbVal = &bcd[1];
				exDBdata_set( DATE_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 2 );
			}

			int pntbl[] = { DATE_PN, VAL_PN, ZEI_PN, SKBN_PN };

			pDBzm->BcdDateToString( str, getLINE_DATA()->BcdDate, m_bMnthFix );
			exDBSetDispString( DATE_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, str );
			//消費税も表示
			TerminationDataSet( pntbl, (sizeof pntbl / sizeof pntbl[0]) );

//			return;
//		}

NEXT:

		if( !nChar ) {
			// 10.16 /03 ... 諸口ブザー
			if( INP_mode == _APPEND ) {
				chk_syobz( &CREC, DATE_PN );
				if( pAUTOSEL->SYOBUZ_OPT == OPT_SYOBUZ && is_syobuzzer_on(DATE_PN) )
					Buzzer();
			}
			return;
		}

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return;

		if( nChar )
		{
			if( nChar == VK_UP || nChar == VK_DOWN ) {
				int ln = get_nowln();
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			nChar = dline_chg( pwnd, nChar );
			if(	nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合はＳＥＱに
				if( isdeldata() )
				{
					SAVE_PN = DATE_PN;
					set_focus( SEQ_PN );
					return;
				}
				else
					SAVE_PN = -1;

				set_focus( DATE_PN );
				return;
			}
		}
		long mnthFixKey = VK_TAB;
		if( !DateTabKeyMode() ) {
			mnthFixKey = VK_HOME;
			if( nChar == VK_TAB )	nChar = VK_RETURN;
		}

		if( nChar == VK_RETURN || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
		{
			if( !is_AutoInp_use() )
			{
				if( CheckAppendSansyo() )
					return;

#ifdef LATER_CLOSE	///////////////////////////////////

				// 伝票番号「なし」モードで、/*伝票番号が無い場合*/
				if( (pAUTOSEL->DENP_OPT == OPT_NON) /*&& (getLINE_DATA()->_XXDTA.dd_cno == (WORD)0xffff)*/ )
				{
					if( getINP_CTL( pwnd, BMON_PN )->INP_ena )	// 部門入力が許可されていれば
						set_focus( BMON_PN );				// 部門に、
					else									// そうでなければ	
						set_focus( DEBT_PN );				// 借方科目に、すすむ。
				}
				else
					set_focus( DENP_PN );

#else
				set_focus( DENP_PN );

#endif	///////////////////////////////////////////////
				set_termfocus( get_nowln(), DATE_PN, 1 );
			}
		}
		else if( nChar == mnthFixKey ) {	// 日付固定切り替え
			DateInputChange();
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 )
		{
			if( INP_mode != _APPEND )
				set_termfocus( get_nowln(), DATE_PN, -1 );
			else {
				_set_focus( SCROLL_NL(), SEQ_PN );
			}
		}
		else {
			if( nChar != 0x16 )
				Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	

}

// 日付入力の変更
void CDBDinpView::DateInputChange()
{
	// つぎのkillfocus は無視する
	m_bIgnoreDateTerm = TRUE;

	// モードの変更
	m_bMnthFix = ! m_bMnthFix;
	// 日付表示エリアを更新
	c_set( IDC_ICSDISPCTRL10 );
	
	set_focus( DATE_PN );
}

// 伝票フォーカス
#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsDenp( CWnd* pwnd, int ln, short click )
{
	_FocusIcsDenp( pwnd, ln, click );

	OnICSInputFocus( DENP_PN );
}
void CDBDinpView::_FocusIcsDenp( CWnd* pwnd, int ln, short click )
{
#else
void CDBDinpView::FocusIcsDenp( CWnd* pwnd, int ln, short click )
{
#endif
	int nextpn;

	if( PROGRAM_OK == FALSE )
		return;

	try
	{

		//別画面入力消去
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}

		if( _dline_chg( pwnd, ln, DENP_PN, click ? 0 : 1  ) == -1 )
			return;

		if( CheckAppendSansyo() )
			return;

		// 取消データの場合はＳＥＱに
		if( isdeldata() )
		{
			SAVE_PN = DENP_PN;
			set_focus( SEQ_PN );
			return;
		}
		else {
			SAVE_PN = -1;
			if( data_denpdialog(ln) != 0 )
				return;
		}

		// 伝票番号自動入力
		if( INP_mode == _APPEND )
			if( getLINE_DATA()->LINE_CND[DENP_PN].INP_sg == FALSE )
				if( (nextpn = getAUTOSELpn( get_nowln(), DENP_PN )) != DENP_PN )
				{
					set_nowpn( DENP_PN );
					AutoInp( nextpn );
					set_focus( nextpn );
					return;
				}

		SelWndShow( FALSE );

		set_nowpn( DENP_PN );
	
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


/***********************
	伝票番号入力メイン
************************/
void CDBDinpView::TerminationIcsinputDenp( CWnd* pwnd, int ln, long nChar, long inplen, long kst  )
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
//struct _ItemData check = {0};
int ip_denp;
char buf[30];
size_t length;
VARIANT var;

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
//		if( !nChar )	//フォーカスＯＦＦ
//		{
			exDBdata_get( DENP_PN, getINP_CTL( pwnd, DENP_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			if( var.pbVal )	length = strlen( (LPCTSTR)var.pbVal );
			else			length = 0;

			if( length > 0 ) {
				ip_denp = atoi( (const char*)var.pbVal );
			}
			else {
				ip_denp = -1;
			}

			if( !dinp_modefy_ok( getLINE_DATA() ) )
			{
				if( getLINE_DATA()->_XXDTA.dd_cno != ip_denp )
				{
					Buzzer();
				}
				if( getLINE_DATA()->_XXDTA.dd_cno != -1 ) {
					sprintf_s( buf, sizeof buf, "%d", getLINE_DATA()->_XXDTA.dd_cno );
				}
				else {
					::ZeroMemory( buf, sizeof buf );
				}
				var.pbVal = (BYTE*)buf;
				exDBdata_set( DENP_PN, getINP_CTL( pwnd, DENP_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			else
			{
				if( getLINE_DATA()->LINE_CND[DENP_PN].INP_sg == FALSE && ip_denp == -1 /*&& !((CEdit*)pwnd->GetDlgItem( getINP_CTL( pwnd, DENP_PN )->IDC_X ))->LineLength() */ )
				{
					if( getLINE_DATA()->LINE_CND[DENP_PN].NEXT_MOVE_KEY == FALSE )
						goto NEXT;
				}

				if( INP_mode == _APPEND && getLINE_DATA()->LINE_CND[DENP_PN].INP_sg == FALSE )
				{
					// 伝票番号自動入力
					if( auto_cno( &ip_denp, &CREC, pAUTOSEL ) == FALSE )
					{
						if( ip_denp == -1 )
							ip_denp = SREC.m_cno;
					}
				}

				CREC.m_cno = ip_denp;
				SREC.m_cno = ip_denp;

				getLINE_DATA()->_XXDTA.dd_cno = ip_denp;
				getLINE_DATA()->LINE_CND[DENP_PN].INP_sg = TRUE;

				if( getLINE_DATA()->_XXDTA.dd_cno != -1 ) {
					sprintf_s( buf, sizeof buf, "%d", getLINE_DATA()->_XXDTA.dd_cno );
				}
				else {
					::ZeroMemory( buf, sizeof buf );
				}
				var.pbVal = (BYTE*)buf;
				exDBdata_set( DENP_PN, getINP_CTL( pwnd, DENP_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		//		data_set( getINP_CTL( pwnd, DENP_PN )->IDC_X, &getLINE_DATA()->DENP_DATA, CICSInput );
				TerminationDataSet( DENP_PN );

				// 自動加算
				if( INP_mode == _APPEND && pAUTOSEL->DENP_OPT == OPT_AUTO/*1*/ ) {
					DENP_NO = int_seetno( CREC.m_cno );
				}
			}
//			return;
//		}

NEXT:
		getLINE_DATA()->LINE_CND[DENP_PN].NEXT_MOVE_KEY = FALSE;

		if( !nChar ) {
			// 10.16 /03 ... 諸口ブザー
			if( INP_mode == _APPEND ) {
				chk_syobz( &CREC, DENP_PN );
				if( pAUTOSEL->SYOBUZ_OPT == OPT_SYOBUZ && is_syobuzzer_on(DENP_PN) )
					Buzzer();
			}
			return;
		}

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return;

		if( nChar )
		{
			if( nChar == VK_UP || nChar == VK_DOWN ) {
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			nChar = dline_chg( pwnd, nChar );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合はＳＥＱに
				if( isdeldata() )
				{
					SAVE_PN = DENP_PN;
					set_focus( SEQ_PN );
					return;
				}
				else
					SAVE_PN = -1;

				int set_pn = pn_movecheck( DENP_PN );
				set_focus( set_pn );
				return;
			}
		}

		if( dinp_modefy_ok( getLINE_DATA() ) && nChar == VK_DELETE )
		{
			ip_denp = -1;
			::ZeroMemory( buf, sizeof buf );
			var.pbVal = (BYTE*)buf;
			exDBdata_set( DENP_PN, getINP_CTL( pwnd, DENP_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		//	data_set( getINP_CTL( pwnd, DENP_PN )->IDC_X, &check, CICSInput );

			CREC.m_cno = ip_denp;
			SREC.m_cno = ip_denp;

			getLINE_DATA()->_XXDTA.dd_cno = ip_denp;
			getLINE_DATA()->LINE_CND[DENP_PN].INP_sg = TRUE;

			TerminationDataSet( DENP_PN );

			if( INP_mode == _APPEND )
				DENP_NO = 0;
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
		{
			getLINE_DATA()->LINE_CND[DENP_PN].NEXT_MOVE_KEY = TRUE;

			if( !is_AutoInp_use() )
			{
			//	if( getINP_CTL( pwnd, BMON_PN )->INP_ena )
			//		set_focus( BMON_PN );
			//	else
			//		set_focus( DEBT_PN );
				set_termfocus( get_nowln(), DENP_PN, 1 );
			}
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 ) {
		//	set_focus( DATE_PN );
			set_termfocus( get_nowln(), DENP_PN, -1 );
		}
		else if( nChar == VK_DELETE )
			;
		else {
			if( nChar != 0x16 )
				Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}

}

/*==========================
	自動伝票番号取り出し
===========================*/
BOOL CDBDinpView::auto_cno( int *next_cno, CDBINPDataRec *data, struct _AUTOSEL *mode )
{
	switch( mode->DENP_OPT )
	{
	case OPT_AUTO/*1*/:	// 自動加算
		if( DENP_NO )
		{
			*next_cno = DENP_NO;
			return TRUE;	// セットした
		}
		break;

	case OPT_EQU/*2*/:	// ＝ＳＥＱ番号
		*next_cno = data->m_seq;
		return TRUE;	// セットした

	case OPT_NON/*3*/: // 無し
		*next_cno = -1;
		return TRUE;	// セットした

	default: // 毎回入力
		break;
	}
	return FALSE;	// セットせず
}


// 部門フォーカス処理
#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsBmon( CWnd* pwnd, int ln, int dc_sw, short click )
{
	_FocusIcsBmon( pwnd, ln, dc_sw, click );

	int pn;
	pn = dc_sw ? CBMN_PN : BMON_PN;
	OnICSInputFocus( pn );
}
void CDBDinpView::_FocusIcsBmon( CWnd* pwnd, int ln, int dc_sw, short click )
{
#else
void CDBDinpView::FocusIcsBmon( CWnd* pwnd, int ln, int dc_sw, short click )
{
#endif
	int nextpn;
	int pn;
	pn = dc_sw ? CBMN_PN : BMON_PN;

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
		//別画面入力消去
		Destroy_ModeLessDlg();

		if( getINP_CTL( pwnd, pn )->INP_ena == TRUE )
		{
			if( click ) {
				long nChar = 0;
				click_seljob( ln, nChar );
			}

			if( _dline_chg( pwnd, ln, pn, click ? 0 : 1 ) == -1 )
				return;

			if( CheckAppendSansyo() )
				return;

			// 取消データの場合はＳＥＱに
			if( isdeldata() )
			{
				SAVE_PN = pn;
				set_focus( SEQ_PN );
				return;
			}
			else {
				SAVE_PN = -1;
				if( data_denpdialog(ln) != 0 )
					return;
			}

		}
		else
		{
			set_focus( get_nowpn() );	//フォーカス戻し
			return;
		}

		int chkpn;
		chkpn = !dc_sw ? BMON_PN : CBMN_PN;
		int set_pn;
		if( inp_ok( get_nowpn(), chkpn, &set_pn ) == FALSE )
		{
			set_nowpn( set_pn );
			set_focus( set_pn );
			return;
		}

		// 部門固定
		if( INP_mode == _APPEND ) {
			if( getLINE_DATA()->LINE_CND[pn].INP_sg == FALSE ) {

				if( (nextpn = getAUTOSELpn( get_nowln(), pn )) != chkpn )
				{
					set_nowpn( chkpn );	// ターミネーションOFF での正しい項目位置をセット
					AutoInp( nextpn );
					set_focus( nextpn );
					return;
				}
				else if( click ) {
					//当項目まで自動入力
					if( !is_AutoInp_use() && chkpn == CBMN_PN )
						AutoInp( chkpn );
				}
			}
			else if( chkpn == CBMN_PN ) {
				if( getLINE_DATA()->LINE_CND[DEBT_PN].INP_sg == FALSE ) {
					if( !is_AutoInp_use() )
						AutoInp( chkpn );
				}
			}
		}
		
		set_nowpn( pn );

		SelWndShow( TRUE );

		if( INP_mode != _APPEND ) {
			if( ! BmonKoji_DebCre_Inp( get_nowln() ) ) {	// 部門が 一つのみ表示されている
				if( CREC.m_dbmn != -1 || CREC.m_cbmn != -1 ) {
					if( CREC.m_dbmn != CREC.m_cbmn ) {
						if( !m_pBkInp )	BkInpMake();
						m_pBkInp->Bkinp_Init( 0, &CREC, getLINE_DATA() );
						BkInpDisp( pn );

						if( m_BKINP_PN == CBMN_PN ) {
							m_BKINP_PN = -1;
							m_pBkInp->set_fastfocus( dinp_modefy_ok( getLINE_DATA()), 1 );
						}
						else {
							m_pBkInp->set_fastfocus( dinp_modefy_ok( getLINE_DATA()), 0 );
						}
					}
				}
			}
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

//----------------------------------------------------
// 概要：借／貸 個別に 部門・工事入力するのか？
// 引数：ln		行番号
// 返送値： TRUE = 借／貸 個別に部門・工事入力する
//----------------------------------------------------
BOOL CDBDinpView::BmonKoji_DebCre_Inp( int ln )
{
	BOOL bRet = FALSE;
	DWORD st;

	if( ln == APPEND_LINE ) {
		st = m_DtLine.GetStyle();
		if( st & DS_BK_DEBCRE )
			bRet = TRUE;
	}
	else {
		switch( m_DtList.GetLines() ) {
		case 1:	bRet = FALSE;	break;
		case 2:	bRet = TRUE;	break;
		default:
			st = m_DtLine.GetStyle();
			if( st & DS_BK_DEBCRE )
				bRet = TRUE;
			break;
		}	
	}

	return bRet;
}



// 部門番号IMEのEND処理 カナ検索
int CDBDinpView::CheckImeEndBumon( CWnd* pwnd, int pn, long nChar, LPCTSTR string ) 
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

			SelWndShow( TRUE );

			m_Seldata.SelectJob( &par, -1 );
			ret = 1;
		}
		else {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();

			exDBdata_set( pn, getINP_CTL( pwnd, pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else {
		if( isdigitstr( (char*)(LPCTSTR)buf ) ) {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();

			exDBdata_set( pn, getINP_CTL( pwnd, pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
		else {
			ret = 1;
		}
	}

	return ret;
}



// 部門のターミネーション
void CDBDinpView::TerminationIcsinputBumon( CWnd* pwnd, int dc_sw, int ln, long nChar, long inplen, long kst ) 
{
int check = -1, prev = -1;
int pntbl[] = { BMON_PN, CBMN_PN };
int number, sel_sw, bm_dcset = 0, bmnno_input;
size_t len;
int bmon, pn, n;
VARIANT var;
long nKey;
static int	do_bmncheck = 0;
int	set_bmntky = 0;

pn = dc_sw ? CBMN_PN : BMON_PN;

char	tmp[30] = {0}, bf[30] = {0};
int	chk_dbt, chk_cre;
chk_dbt = chk_cre = -1;


	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( PROGRAM_OK == FALSE )
		return;

	// 借方部門のとき
	
	if( ! dc_sw && ! BmonKoji_DebCre_Inp( ln ) ) {
		// 借方・貸方 同じ部門をセットするか？
		int deb, cre;
		deb = getLINE_BumonData( 0 );
		cre = getLINE_BumonData( 1 );

		if( deb == cre || (deb == -1 && cre == -1) ) {
			bm_dcset = 1;
		}
	}
	int dc_sgn;
	if( bm_dcset )	dc_sgn = -1;
	else			dc_sgn = dc_sw;

	try
	{
//		if( !nChar )	//フォーカスＯＦＦ
//		{
			// 部門選択 画面番号優先( VK_RETURN )
			nKey = (pAUTOSEL->BMNSEL_OPT == OPT_BMNSEL) ? VK_RETURN : VK_TAB;
			bmnno_input = 0;

			sel_sw = 1;
			// 直前のデータを保存
			prev = getLINE_BumonData(dc_sw);

			exDBdata_get( pn, getINP_CTL( pwnd, pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			if( var.pbVal != NULL )
				strcpy_s( bf, sizeof bf, (const char*)var.pbVal );
			len = 0;

			len = strlen( (const char*)bf );
			bmon = -1;

			if( len > 0 ) {
				bmon = atoi( (LPCTSTR)bf );
			}
			check = bmon;

#ifdef _2KT_BSEL	// 10.01 /02 .......................................
			// 画面番号優先のときのみ 2桁 で 画面番号選択
			if( inplen > 0 && (len  > 0 && len <= 2) && nKey == VK_RETURN ) {
				// 選択画面より番号選択
				if( dinp_modefy_ok( getLINE_DATA() ) ) {
					number = atoi( bf );

					CString bmnname;

					if( bmncode_todata2( &CREC, number, dc_sgn, &bmnname ) != FALSE ) {
						bmon_select_dsp( pwnd, dc_sw, &CREC, FALSE );

						if( bm_dcset ) {
							if( CREC.m_dbmn == -1 )	check = CREC.m_cbmn;
							else					check = CREC.m_dbmn;
						}
						else {
							check = (dc_sw) ? CREC.m_cbmn : CREC.m_dbmn;
						}
						bmnkoji_to_tekiyo( &CREC, &bmnname );
						bmneda_update(&CREC, dc_sgn);
					}
					else {
						check = getLINE_BumonData(dc_sw);
						Buzzer();
						sel_sw = 0;	// 不正な選択番号を入力
					}
				}
			}
			else if( inplen > 0 ) {
#else
			else {
#endif	// ...........................................................

				// 部門を手入力
				n = BMON_LNG-len;
					
				if( n >= 0 ) {
					bmon = atoi( bf );

					if( pDBzm->IsGokeiBumon( bmon ) ) {
						// 合計部門は入力不可
						PROGRAM_OK = FALSE;
						myICSMessageBox("合計部門は入力できません！");
						PROGRAM_OK = TRUE;

						nChar	= -1;
						bmon	= prev;
					}
					else {
						if( nChar == VK_RETURN && nKey == VK_RETURN ) {
							bmnno_input = 1;	// 部門No 手入力サインON
							do_bmncheck = 1;
						}
						if( !(nChar == VK_TAB && nKey == VK_TAB) )
							set_bmntky = 1;
					}
				}
				else {
					bmon = -1;
				}
				check = bmon;
			}
			else {
				if( ! inplen )
					bmnno_input = 1;
			}

			if( !dinp_modefy_ok( getLINE_DATA() ) )
			{
				if( check != getLINE_BumonData(dc_sw) )
				{
					Buzzer();
				}

				pDBzm->BumonCodeToStr( (char*)tmp, sizeof tmp, getLINE_BumonData(dc_sw) );
				var.pbVal = (BYTE*)tmp;
				exDBdata_set( pn, getINP_CTL( pwnd, pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			}
			else
			{
				if( getLINE_DATA()->LINE_CND[pn].INP_sg == FALSE && len <= 0 && !inplen  )
				{
					if( getLINE_DATA()->LINE_CND[pn].NEXT_MOVE_KEY == FALSE )
						goto NEXT;
				}

				if( INP_mode == _APPEND && getLINE_DATA()->LINE_CND[pn].INP_sg == FALSE )
				{
					if( check == -1 && sel_sw ) {

						if( ! dc_sw && ! BmonKoji_DebCre_Inp( ln ) ) {
							if( dc_sw ) {
								check = SREC.m_cbmn;
								if( check == -1 && SREC.m_dbmn != -1 )
									check = SREC.m_dbmn;
							}
							else {
								check = SREC.m_dbmn;
								if( check == -1 && SREC.m_cbmn != -1 )
									check = SREC.m_cbmn;
							}
						}
						else {
							//各行入力
							check = (dc_sw) ? SREC.m_cbmn : SREC.m_dbmn;
						}
						if( check != -1 )	set_bmntky = 1;

					//	check = (dc_sw) ? SREC.m_cbmn : SREC.m_dbmn;
					}
				}

				if( bm_dcset ) {	// 借貸 同じ部門をセット
					chk_dbt = chk_cre = check;

					SREC.m_dbmn = chk_dbt;
					SREC.m_cbmn = chk_cre;
				//	SREC.m_cbmn = SREC.m_dbmn = check;

					CREC.m_cbmn = SREC.m_cbmn;
					CREC.m_dbmn = SREC.m_dbmn;
				
					getLINE_DATA()->_XXDTA.dd_cbmn = chk_cre;
					getLINE_DATA()->_XXDTA.dd_dbmn = chk_dbt;
					getLINE_DATA()->LINE_CND[CBMN_PN].INP_sg = TRUE;
				}
				else {
#ifdef CLOSE
					if( INP_mode != _APPEND ) {
						if( dc_sw ) {
							if( ! BmChk.BmnHaniCheck( check, CREC.m_cre ) ) {
								check = -1;
							}
						}
						else {
							if( ! BmChk.BmnHaniCheck( check, CREC.m_dbt ) ) {
								check = -1;
							}
						}
					}
#endif

					if( dc_sw )	SREC.m_cbmn = check;
					else		SREC.m_dbmn = check;

					if( dc_sw ){
						CREC.m_cbmn = SREC.m_cbmn;
						getLINE_DATA()->_XXDTA.dd_cbmn = check;
					}
					else {
						CREC.m_dbmn = SREC.m_dbmn;
						getLINE_DATA()->_XXDTA.dd_dbmn = check;
					}
				}
				set_etcdsp( ln, pn );
				if( set_bmntky ) {
					struct _SET_KMKETC sk;
					// 部門
					sk.bmn = check;
					_set_bmnetc( &sk );
					bmnkoji_to_tekiyo( &CREC, &sk.etc );
					bmneda_update(&CREC, dc_sgn);
				}
				//個別対応で部門の消費税あり
				if (pDBzm->IsKobetsuBmnSyz() && set_bmntky && INP_mode == _APPEND ) {
					DWORD dwKobe = pDBzm->KobetsuSiwakeCheck(&CREC, check);

					if (dwKobe) {
						pDBzm->SetCDBData(&CREC);
						pDBsy->SyFnc(dwKobe);
						getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt(&CREC, NULL, INP_mode);
						memcpy(getLINE_DATA()->_XXDTA.dd_zei, CREC.m_zei, 6);
						//消費税再表示
						int pntbl[] = { ZEI_PN, SKBN_PN };
						if (is_syohizeisiwake(&CREC)) {
							getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = TRUE;
						}
						else {
							getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = FALSE;
						}
						TerminationDataSet(pntbl, 2);
					}
				}

				getLINE_DATA()->LINE_CND[pn].INP_sg = TRUE;

				pDBzm->BumonCodeToStr( tmp, sizeof tmp, getLINE_BumonData(dc_sw) );
				var.pbVal = (BYTE*)tmp;
				exDBdata_set( pn, getINP_CTL( pwnd, pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		
				if( bm_dcset )	TerminationDataSet( pntbl, 2 );
				else			TerminationDataSet( pn );
			}
//			return;
//		}
NEXT:
		getLINE_DATA()->LINE_CND[pn].NEXT_MOVE_KEY = FALSE;

		if( !nChar ) {
			// 08.30 /04 ... 画面番号優先機能をつけたため、チェックする
			if( do_bmncheck ) {
				do_bmncheck = 0;
				if( BmnCodeChk( pwnd, check, sel_sw, pn ) )
					return;
			}
			return;
		}
		// 合計部門入力は、カーソル再セット
		if( nChar == -1 ) {
			set_focus( pn );
			return;
		}

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return;

		if( nChar )
		{
			//#* 部門選択
			nChar = SelBmonDsp( nChar );

			//#* 部門チェック ... 08.28 /02
			if( INP_mode != _APPEND && (nChar == VK_UP || nChar == VK_DOWN) ) {
				if( BmnCodeChk( pwnd, check, sel_sw, pn ) )
					return;
			}

			if( nChar == VK_UP || nChar == VK_DOWN ) {
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			nChar = dline_chg( pwnd, nChar );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合はＳＥＱに
				if( isdeldata() )
				{
					SAVE_PN = pn;
					set_focus( SEQ_PN );
					return;
				}
				else
					SAVE_PN = -1;

				int set_pn = pn_movecheck( pn );
				set_focus( set_pn );
				return;
			}
		}

		if( dinp_modefy_ok( getLINE_DATA() ) && nChar == VK_DELETE )
		{
			check = -1;
			::ZeroMemory( tmp, sizeof tmp );
			var.pbVal = (BYTE*)tmp;
			exDBdata_set( pn, getINP_CTL( pwnd, pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			if( bm_dcset ) {	// 借貸 同じ部門をセット
				SREC.m_dbmn = -1;
				SREC.m_cbmn = -1;
				CREC.m_dbmn = -1;
				CREC.m_cbmn = -1;
				getLINE_DATA()->_XXDTA.dd_dbmn = check;
				getLINE_DATA()->_XXDTA.dd_cbmn = check;
				getLINE_DATA()->LINE_CND[CBMN_PN].INP_sg = TRUE;

				TerminationDataSet( pntbl, 2 );
				bmneda_update(&CREC, dc_sgn);
			}
			else {
				if( dc_sw ) {
					SREC.m_cbmn = -1;
					CREC.m_cbmn = -1;
				}
				else {
					SREC.m_dbmn = -1;
					CREC.m_dbmn = -1;
				}			
				if( dc_sw ) getLINE_DATA()->_XXDTA.dd_cbmn = check;
				else		getLINE_DATA()->_XXDTA.dd_dbmn = check;
				getLINE_DATA()->LINE_CND[pn].INP_sg = TRUE;
				TerminationDataSet( pn );	
				bmneda_update(&CREC, dc_sgn);
			}
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
		{
			//#* 部門チェック ... 08.28 /02
			// 画面番号優先のときはチェックしない [08.30 /04]
			if( nChar == VK_RETURN && nKey == VK_TAB ) {
				do_bmncheck = 0;
				if( BmnCodeChk( pwnd, check, sel_sw, pn ) )
					return;
			}

			getLINE_DATA()->LINE_CND[pn].NEXT_MOVE_KEY = TRUE;

			if( !is_AutoInp_use() ) {

				if( dinp_modefy_ok( getLINE_DATA() ) ) {
					// 部門番号優先時は、ここで 画面番号より選択
					if( nKey == VK_TAB && nChar == nKey && ! bmnno_input ) {
						if( check != -1 ) {
							number = check;

							CString	bmnname;
	
							// 番号が不正なときはブザー
							if( !bmncode_todata2( &CREC, number, dc_sw, &bmnname ) ) {
								if( dc_sw ) {
									CREC.m_cbmn = prev;
								}
								else {
									CREC.m_dbmn = prev;
								}
								Buzzer();
							}
							// 部門再表示
							bmon_select_dsp( pwnd, dc_sw, &CREC, FALSE );
							bmnkoji_to_tekiyo( &CREC, &bmnname );
							bmneda_update(&CREC, dc_sgn);
						}
					}
				}

			//	set_focus( DEBT_PN );
				set_termfocus( get_nowln(), pn, 1 );
			}
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 )
		{
			set_termfocus( get_nowln(), pn, -1 );
		}
		else if( nChar == VK_DELETE )
			;
		else {
			if( nChar != 0x16 )
				Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


LRESULT CDBDinpView::OnBkinpEnd( WPARAM wParam, LPARAM lParam )
{
	long nChar;
	int bm_pn[] = {BMON_PN, CBMN_PN};
	int kj_pn[] = {KOJI_PN, CKJI_PN};

	if( m_pBkInp != NULL ) {
		if( ! m_pBkInp->GetMode() ) {

			if( wParam )
			{
				TerminationDataSet( bm_pn, 2 );

				nChar = dline_chg( this, wParam );
				if( nChar == -1 )
				{
	//				MessageBeep( 0xffffffff );
					return 1;
				}
				else if( !nChar )
				{
					// 取消データの場合はＳＥＱに
					if( isdeldata() )
					{
						SAVE_PN = BMON_PN;
						set_focus( SEQ_PN );
						return 1;
					}
					else
						SAVE_PN = -1;

					set_focus( BMON_PN );
					return 1;
				}

				switch( nChar ) {
				case VK_RETURN : case VK_RIGHT : case VK_TAB : 
				case VK_DELETE:
					set_termfocus( get_nowln(), BMON_PN, 1 );
					break;
				case VK_LEFT: case VK_F2:
					set_termfocus( get_nowln(), BMON_PN, -1 );
					break;
				}

			}
		}
		else {
			TerminationDataSet( kj_pn, 2 );

			nChar = dline_chg( this, wParam );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return 1;
			}
			else if( !nChar )
			{
				// 取消データの場合はＳＥＱに
				if( isdeldata() )
				{
					SAVE_PN = KOJI_PN;
					set_focus( SEQ_PN );
					return 1;
				}
				else
					SAVE_PN = -1;

				set_focus( KOJI_PN );
				return 1;
			}

			switch( nChar ) {
			case VK_RETURN : case VK_RIGHT : case VK_TAB : 
			case VK_DELETE:
				set_termfocus( get_nowln(), KOJI_PN, 1 );
				break;
			case VK_LEFT: case VK_F2:
				set_termfocus( get_nowln(), KOJI_PN, -1 );
				break;
			}
		}
	}

	return 1;
}


//#* 部門を表示 ... 02.27 /02
void CDBDinpView::bmon_select_dsp( CWnd* pwnd, int dc_sw, CDBINPDataRec *pCrec, BOOL bTerm )
{
	int pn;
	pn = (dc_sw) ? CBMN_PN : BMON_PN;

	char	tmp[40] = {0};
	VARIANT var;
	pDBzm->BumonCodeToStr( tmp, sizeof tmp, dc_sw ? pCrec->m_cbmn : pCrec->m_dbmn );
	var.pbVal = (BYTE*)tmp;
	exDBdata_set( pn, getINP_CTL( pwnd, pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	TerminationDataSet(pn);

	if( bTerm ) {
		getLINE_DATA()->LINE_CND[get_nowpn()].NEXT_MOVE_KEY = TRUE;

	#ifdef	_ICSPEN_
		if( IsPen() != FALSE )
		{
			//選択後次へ移動
			pwnd->GetDlgItem( getINP_CTL( pwnd, get_nowpn() )->IDC_X )->PostMessage( WM_KEYDOWN, VK_TAB );
			return;
		}
		else
		{
			//データセットのためのダミーフォーカスＯＦＦ
			TerminationIcsinputBumon( pwnd, dc_sw, get_nowln(), 0, 0, 0 );
		}
	#else
		//データセットのためのダミーフォーカスＯＦＦ
		TerminationIcsinputBumon( pwnd, dc_sw, get_nowln(), 0, 0, 0 );
	#endif
	}
}

// 08.28 /02
// 部門のチェック
BOOL CDBDinpView::BmnCodeChk( CWnd* pwnd, int bmn, int sw, int pn )
{
	BOOL bRet = FALSE;
	int st;
	CString msg;
	char tmp[20] = {0};

	if( !FNC_BUTTON )
	{
		if( pAUTOSEL->BMNCHK_OPT != OPT_BMNCHK )
			return FALSE;

		if( !dinp_modefy_ok( getLINE_DATA() ) )
			return FALSE;

		// 摘要のみ修正 か 部門入力不可
		if( IsSCAN_TKYONLY() || !BMON_LNG ) return FALSE;
		// 部門未入力
		if( bmn == -1 )	return FALSE;

		// 不正な選択番号を入力したのでチェックせずにもどる
		if( !sw )	return TRUE;

		PROGRAM_OK = FALSE;

		if( !pDBzm->bmncode_ok( bmn ) ) {
			pDBzm->BumonCodeToStr( tmp, sizeof tmp, bmn );

		//	msg.Format( " %s[%s]は登録されていません！\n\n 修正しますか？", d_bmn, tmp );
			msg.Format( " %s[%s]は登録されていません！\n\n 登録しますか？\r\n\r\n はい…自動登録、いいえ…部門入力に戻る", d_bmn, tmp );

			st = myICSMessageBox( msg, MB_YESNO|MB_DEFBUTTON2 );
		//	if( st == IDYES ) {
			if( st == IDNO ) {
				if( pwnd == this ) {
					pwnd->PostMessage( WM_RETROFOCUS, pn );
					set_nowpn( pn );
					bRet = TRUE;
				}
				else {	// 別ダイアログからの呼び出し
					pwnd->PostMessage( WM_RETROFOCUS, pn );
					bRet = TRUE;
				}
			}		
		}
		
		PROGRAM_OK = TRUE;
	}	

	return bRet;
}



// 工事フォーカス処理
#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsKoji( CWnd* pwnd, int ln, int dc_sw, short click )
{
	_FocusIcsKoji( pwnd, ln, dc_sw, click );

	int pn;
	pn = dc_sw ? CKJI_PN : KOJI_PN;
	OnICSInputFocus( pn );
}
void CDBDinpView::_FocusIcsKoji( CWnd* pwnd, int ln, int dc_sw, short click )
{
#else
void CDBDinpView::FocusIcsKoji( CWnd* pwnd, int ln, int dc_sw, short click )
{
#endif
	int nextpn;
	int pn;
	pn = dc_sw ? CKJI_PN : KOJI_PN;

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
		//別画面入力消去
		Destroy_ModeLessDlg();

		if( getINP_CTL( pwnd, pn )->INP_ena == TRUE )
		{
			if( click ) {
				long nChar = 0;
				click_seljob( ln, nChar );
			}

			if( _dline_chg( pwnd, ln, pn, click ? 0 : 1 ) == -1 )
				return;

			if( CheckAppendSansyo() )
				return;

			// 取消データの場合はＳＥＱに
			if( isdeldata() )
			{
				SAVE_PN = pn;
				set_focus( SEQ_PN );
				return;
			}
			else {
				SAVE_PN = -1;
				if( data_denpdialog(ln) != 0 )
					return;
			}
		}
		else
		{
			set_focus( get_nowpn() );	//フォーカス戻し
			return;
		}

		int set_pn;
		if( inp_ok( get_nowpn(), pn, &set_pn ) == FALSE )
		{
			set_nowpn( set_pn );
			set_focus( set_pn );
			return;
		}

		// 工事固定
		if( INP_mode == _APPEND ) {
			if( getLINE_DATA()->LINE_CND[pn].INP_sg == FALSE ) {
				if( (nextpn = getAUTOSELpn( get_nowln(), pn )) != pn )
				{
					AutoInp( nextpn );
					set_focus( nextpn );
					return;
				}
				else if( click ) {
					//当項目まで自動入力
					if( !is_AutoInp_use() && pn == CKJI_PN )
						AutoInp( pn );
				}
			}
			else if( pn == CKJI_PN ) {
				if( getLINE_DATA()->LINE_CND[DEBT_PN].INP_sg == FALSE ) {
					if( !is_AutoInp_use() )
						AutoInp( pn );
				}
			}
		}

		if( INP_mode != _APPEND ) {
			if( ! BmonKoji_DebCre_Inp( get_nowln() ) ) {	// 工事が 一つのみ表示されている
				if( ! CREC.m_dkno.IsEmpty() || !CREC.m_ckno.IsEmpty() ) {
					if( CREC.m_dkno != CREC.m_ckno ) {
						if( !m_pBkInp )	BkInpMake();
						m_pBkInp->Bkinp_Init( 1, &CREC, getLINE_DATA() );
						BkInpDisp( pn );

						if( m_BKINP_PN == CKJI_PN ) {
							m_BKINP_PN = -1;
							m_pBkInp->set_fastfocus( dinp_modefy_ok( getLINE_DATA() ), 1 );
						}
						else {
							m_pBkInp->set_fastfocus( dinp_modefy_ok( getLINE_DATA() ), 0 );
						}
					}
				}
			}
		}

		set_nowpn( pn );

		SelWndShow( TRUE );

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

// 工事番号IMEのEND処理
int CDBDinpView::CheckImeEndKoji( CWnd* pwnd, int pn, long nChar, LPCTSTR string ) 
{
	VARIANT var;

	CString buf = string;
	int		ret = 0;

	if( nChar == VK_TAB || nChar == VK_F12 ) {
		// カナ検索
		if( ! isdigitstr( (char*)string ) ) {
			SelPar par;
			::ZeroMemory( par.kmk_kana, sizeof par.kmk_kana );
			par.sel_pn = SL_KOJIKANA_PN;
			memcpy( par.kmk_kana, string, sizeof par.kmk_kana );

			SelWndShow( TRUE );

			m_Seldata.SelectJob( &par, -1 );
			ret = 1;
		}
		else {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();

			exDBdata_set( pn, getINP_CTL( pwnd, pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else {
		kana_chk( &buf, buf.GetLength() );

		var.pbVal = (BYTE*)buf.GetBuffer(64);
		buf.ReleaseBuffer();

		exDBdata_set( pn, getINP_CTL( pwnd, pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	return ret;
}


// 工事のターミネーション
void CDBDinpView::TerminationIcsinputKoji( CWnd* pwnd, int dc_sw, int ln, long nChar, long inplen, long kst ) 
{
CString check, prev, koji, zerochk, kjnam;
int pntbl[] = { KOJI_PN, CKJI_PN };
int number, sel_sw, kj_dcset = 0, kno_input;
int set_kjtky = 0;
int digit_sgn = 0;
size_t len;
int pn, n;
VARIANT var;
long nKey;
static int	do_knocheck = 0;

pn = dc_sw ? CKJI_PN : KOJI_PN;

char	tmp[30] = {0}, bf[30] = {0};

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( PROGRAM_OK == FALSE )
		return;

	// 借方工事のとき
	if( ! dc_sw && ! BmonKoji_DebCre_Inp( ln ) ) {
		// 借方・貸方 同じ工事をセットするか？
		CString deb, cre;
		deb = getLINE_KojiData( 0 );
		cre = getLINE_KojiData( 1 );

		if( deb == cre || (deb.IsEmpty() && cre.IsEmpty() ) ) {
			kj_dcset = 1;
		}
	}

	try
	{
//		if( !nChar )	//フォーカスＯＦＦ
//		{
			// 工事選択 画面番号優先( VK_RETURN )
			nKey = (pAUTOSEL->BMNSEL_OPT == OPT_BMNSEL) ? VK_RETURN : VK_TAB;
			kno_input = 0;

			sel_sw = 1;
			// 直前のデータを保存
			prev = getLINE_KojiData(dc_sw);

			exDBdata_get( pn, getINP_CTL( pwnd, pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			len = 0;
			if( var.pbVal != NULL )	{
				strcpy_s( bf, sizeof bf, (const char*)var.pbVal );
				if( inplen > 0 ) {
					len = strlen( (const char*)bf );	// ゼロをつめるまえの長さ
				}
				trim_zero( bf, sizeof bf, strlen( bf ));
			}
			koji.Empty();

			if( len > 0 ) {
				pDBzm->KojiCodeToStr( koji.GetBuffer(48), 48, bf );
				koji.ReleaseBuffer();
			}
			zerochk	= koji;
			check	= bf;

#ifdef _2KT_BSEL	// 10.01 /02 .......................................
			digit_sgn = isdigitstr( bf );
			// 画面番号優先のときのみ 2桁 で 画面番号選択
			if( inplen != 0 && (len  > 0 && len <= 2) && digit_sgn && nKey == VK_RETURN ) {
				// 選択画面より番号選択
				if( dinp_modefy_ok( getLINE_DATA() ) ) {
					number = atoi( bf );
					if( kojicode_todata( &CREC, &kjnam, number, dc_sw ) != FALSE ) {
						koji_select_dsp( pwnd, dc_sw, &CREC, FALSE );
						check = (dc_sw) ? CREC.m_ckno : CREC.m_dkno;

						// 工事名称から、仕訳摘要へ
						bmnkoji_to_tekiyo( &CREC, &kjnam, 1 );
					}
					else {
						check = getLINE_KojiData(dc_sw);
						Buzzer();
						sel_sw = 0;	// 不正な選択番号を入力
					}
				}
			}
			else if( inplen > 0 && (len > 2 || !digit_sgn) ) {
#else
			else {
#endif	// ...........................................................

				// 工事を手入力
				n = pDBzm->zvol->kjcol-len;

				if( n >= 0 ) {
					pDBzm->KojiCodeToStr( koji.GetBuffer(48), 48, bf );
					koji.ReleaseBuffer();
					// 選択画面番号を優先時
					if( nChar == VK_RETURN && nKey == VK_RETURN ) {
						kno_input = 1;	// 工事No 手入力サインON
						do_knocheck = 1;
					}

					trim_zero( bf, sizeof bf, strlen( bf ));
					set_kjtky = 1;
				}
				else {
					koji.Empty();
					bf[0] = '\0';
				}
				zerochk	= koji;
				check	= bf;
			}
			else {
				// 入力されていない場合、TABでの検索はしない
				if( ! inplen )
					kno_input = 1;
				else {
					set_kjtky = 1;	// 09.29 /10

					//工事番号優先で タブキーで画面より選択　[04.18 /12]
					if( (nKey == VK_TAB) && nChar == VK_TAB )
						set_kjtky = 0;
				}
			}

			if( !dinp_modefy_ok( getLINE_DATA() ) )
			{
				if( check != getLINE_KojiData(dc_sw) )
				{
					Buzzer();
				}

				pDBzm->KojiCodeToStr( (char*)tmp, sizeof tmp, getLINE_KojiData(dc_sw) );
				var.pbVal = (BYTE*)tmp;
				exDBdata_set( pn, getINP_CTL( pwnd, pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			}
			else
			{
				if( getLINE_DATA()->LINE_CND[pn].INP_sg == FALSE && len <= 0 && !inplen )
				{
					if( getLINE_DATA()->LINE_CND[pn].NEXT_MOVE_KEY == FALSE )
						goto NEXT;
				}

				if( INP_mode == _APPEND && getLINE_DATA()->LINE_CND[pn].INP_sg == FALSE )
				{
					if( check.IsEmpty() && sel_sw ) {

						if( ! dc_sw && ! BmonKoji_DebCre_Inp( ln ) ) {
							if( dc_sw ) {
								check = SREC.m_ckno;
								if( check.IsEmpty() && ! SREC.m_dkno.IsEmpty() )
									check = SREC.m_dkno;
							}
							else {
								check = SREC.m_dkno;
								if( check.IsEmpty() && ! SREC.m_ckno.IsEmpty() )
									check = SREC.m_ckno;
							}
							if( ! check.IsEmpty() )	set_kjtky = 1;
						}
						else {
							check = (dc_sw) ? SREC.m_ckno : SREC.m_dkno;
							set_kjtky = 1;
						}
					}
				}

				if( kj_dcset ) {	// 借貸 同じ工事番号をセット
					SREC.m_ckno = SREC.m_dkno = check;

					CREC.m_ckno = SREC.m_ckno;
					CREC.m_dkno = SREC.m_dkno;
				
					getLINE_DATA()->_XXDTA.dd_ckno = check;
					getLINE_DATA()->_XXDTA.dd_dkno = check;
					getLINE_DATA()->LINE_CND[CKJI_PN].INP_sg = TRUE;
				}
				else {
					if( dc_sw ) SREC.m_ckno = check;
					else		SREC.m_dkno = check;

					if( dc_sw ){
						CREC.m_ckno = SREC.m_ckno;
						getLINE_DATA()->_XXDTA.dd_ckno = check;
					}
					else {
						CREC.m_dkno = SREC.m_dkno;
						getLINE_DATA()->_XXDTA.dd_dkno = check;
					}
				}
				// 名称表示
				set_etcdsp( ln, pn );
				if( set_kjtky ) {
					m_Seldata.GetKojiName( check, kjnam, FALSE );
					bmnkoji_to_tekiyo( &CREC, &kjnam, 1 );
				}

				getLINE_DATA()->LINE_CND[pn].INP_sg = TRUE;

				pDBzm->KojiCodeToStr( tmp, sizeof tmp, getLINE_KojiData(dc_sw) );
				var.pbVal = (BYTE*)tmp;
				exDBdata_set( pn, getINP_CTL( pwnd, pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		
				if( kj_dcset )	TerminationDataSet( pntbl, 2 );
				else			TerminationDataSet( pn );	
			}
//			return;
//		}

NEXT:
		getLINE_DATA()->LINE_CND[pn].NEXT_MOVE_KEY = FALSE;

		if( !nChar ) {
			// 08.30 /04 ... 画面番号優先機能をつけたため、チェックする
			if( do_knocheck ) {
				do_knocheck = 0;
				if( KojiCodeChk( pwnd, check, sel_sw, pn ) )
					return;
			}
			return;
		}

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return;

		if( nChar )
		{
			// 工事選択 
			nChar = SelKojiDsp( nChar );

			// 工事チェック
			if( INP_mode != _APPEND && (nChar == VK_UP || nChar == VK_DOWN) ) {
				if( KojiCodeChk( pwnd, check, sel_sw, pn ) )
					return;
			}

			if( nChar == VK_UP || nChar == VK_DOWN ) {
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			nChar = dline_chg( pwnd, nChar );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合はＳＥＱに
				if( isdeldata() )
				{
					SAVE_PN = pn;
					set_focus( SEQ_PN );
					return;
				}
				else
					SAVE_PN = -1;

				int set_pn = pn_movecheck( pn );
				set_focus( set_pn );
				return;
			}
		}

		if( dinp_modefy_ok( getLINE_DATA() ) && nChar == VK_DELETE )
		{
			check.Empty();
			::ZeroMemory( tmp, sizeof tmp );
			var.pbVal = NULL;
			exDBdata_set( pn, getINP_CTL( pwnd, pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			if( kj_dcset ) {	// 借貸 同じ工事をセット
				SREC.m_dkno.Empty();
				SREC.m_ckno.Empty();
				CREC.m_dkno.Empty();
				CREC.m_ckno.Empty();
				getLINE_DATA()->_XXDTA.dd_dkno = check;
				getLINE_DATA()->_XXDTA.dd_ckno = check;
				getLINE_DATA()->LINE_CND[KOJI_PN].INP_sg = TRUE;
				getLINE_DATA()->LINE_CND[CKJI_PN].INP_sg = TRUE;

				TerminationDataSet( pntbl, 2 );
			}
			else {
				if( dc_sw ) {
					SREC.m_ckno.Empty();
					CREC.m_ckno.Empty();
				}
				else {
					SREC.m_dkno.Empty();
					CREC.m_dkno.Empty();
				}			
				if( dc_sw ) getLINE_DATA()->_XXDTA.dd_ckno = check;
				else		getLINE_DATA()->_XXDTA.dd_dkno = check;
				getLINE_DATA()->LINE_CND[pn].INP_sg = TRUE;
				TerminationDataSet( pn );	
			}
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
		{
			// 画面番号優先のときはチェックしない [08.30 /04]
			if( nChar == VK_RETURN && nKey == VK_TAB ) {
				do_knocheck = 0;
				if( KojiCodeChk( pwnd, check, sel_sw, pn ) )
					return;
			}

			getLINE_DATA()->LINE_CND[pn].NEXT_MOVE_KEY = TRUE;

			if( !is_AutoInp_use() ) {

				if( dinp_modefy_ok( getLINE_DATA() ) ) {
					// 工事番号優先時は、ここで 画面番号より選択( TABキー押された場合 )
					if( nKey == VK_TAB && nChar == nKey && ! kno_input ) {
						if( ! check.IsEmpty() ) {
							digit_sgn = isdigitstr( (char*)(LPCTSTR)check );
							if( digit_sgn ) {
								number = atoi( check );

								// 番号が不正なときはブザー
								if( !kojicode_todata( &CREC, &kjnam, number, dc_sw ) ) {
									if( dc_sw ) {
										CREC.m_ckno = prev;
									}
									else {
										CREC.m_dkno = prev;
									}
									Buzzer();
								}
								else {
									// 工事名称から、仕訳摘要へ
									bmnkoji_to_tekiyo( &CREC, &kjnam, 1 );
								}
								// 部門再表示
								koji_select_dsp( this, dc_sw, &CREC, FALSE );
							}
						}
					}
				}

			//	set_focus( DEBT_PN );
				set_termfocus( get_nowln(), pn, 1 );
			}
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 )
		{
			set_termfocus( get_nowln(), pn, -1 );

		}
		else if( nChar == VK_DELETE )
			;
		else {
			if( nChar != 0x16 )
				Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

// 工事を表示
void CDBDinpView::koji_select_dsp( CWnd* pwnd, int dc_sw, CDBINPDataRec *pCrec, BOOL bTerm )
{
	int pn;
	pn = (dc_sw) ? CKJI_PN : KOJI_PN;

	char	tmp[128] = {0};
	VARIANT var;
	if( dc_sw )
		pDBzm->KojiCodeToStr( tmp, sizeof tmp, pCrec->m_ckno );
	else
		pDBzm->KojiCodeToStr( tmp, sizeof tmp, pCrec->m_dkno );

	var.pbVal = (BYTE*)tmp;
	exDBdata_set( pn, getINP_CTL( pwnd, pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	TerminationDataSet(pn);

	if( bTerm ) {
		getLINE_DATA()->LINE_CND[get_nowpn()].NEXT_MOVE_KEY = TRUE;

	#ifdef	_ICSPEN_
		if( IsPen() != FALSE )
		{
			//選択後次へ移動
			pwnd->GetDlgItem( getINP_CTL( pwnd, get_nowpn() )->IDC_X )->PostMessage( WM_KEYDOWN, VK_TAB );
			return;
		}
		else
		{
			//データセットのためのダミーフォーカスＯＦＦ
			TerminationIcsinputKoji( pwnd, dc_sw, get_nowln(), 0, 0, 0 );
		}
	#else
		//データセットのためのダミーフォーカスＯＦＦ
		TerminationIcsinputKoji( pwnd, dc_sw, get_nowln(), 0, 0, 0 );
	#endif
	}
}

// 08.28 /02
// 工事番号のチェック
BOOL CDBDinpView::KojiCodeChk( CWnd* pwnd, CString koji, int sw, int pn )
{
	BOOL bRet = FALSE;
	int st;
	CString msg;
	char tmp[20] = {0};

	if( !FNC_BUTTON )
	{
		if( pAUTOSEL->BMNCHK_OPT != OPT_BMNCHK )
			return FALSE;

		if( !dinp_modefy_ok( getLINE_DATA() ) )
			return FALSE;

		// 部門入力不可
		if( ! M_KOJI ) return FALSE;
		// 部門未入力
		if( koji.IsEmpty() )	return FALSE;

		// 不正な選択番号を入力したのでチェックせずにもどる
		if( !sw )	return TRUE;

		PROGRAM_OK = FALSE;

		if( ! Kojicode_Ok( koji ) ) {
			pDBzm->KojiCodeToStr( tmp, sizeof tmp, koji );

//			msg.Format( " %s[%s]は登録されていません！\n\n 修正しますか？", d_kjno, tmp );
			msg.Format( " %s[%s]は登録されていません！\n\n 処理を進めますか？\r\n\r\n はい…処理続行、いいえ…工事入力に戻る", d_kjno, tmp );

			st = myICSMessageBox( msg, MB_YESNO|MB_DEFBUTTON2 );
		//	if( st == IDYES ) {
			if( st == IDNO ) {

				if( pwnd == this ) {
					pwnd->PostMessage( WM_RETROFOCUS, pn );
					set_nowpn( pn );
					bRet = TRUE;
				}
				else {	// 別ダイアログからの呼び出し
					pwnd->PostMessage( WM_RETROFOCUS, pn );
					bRet = TRUE;
				}
			}		
		}
		
		PROGRAM_OK = TRUE;
	}	

	return bRet;
}

 
// 借方フォーカス処理
#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsDebt( CWnd* pwnd, int ln, short click )
{
	_FocusIcsDebt( pwnd, ln, click );

	OnICSInputFocus( DEBT_PN );
}
void CDBDinpView::_FocusIcsDebt( CWnd* pwnd, int ln, short click )
{
#else
void CDBDinpView::FocusIcsDebt( CWnd* pwnd, int ln, short click )
{
#endif
	int nextpn;

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
		if( BRNCHK_PN == DEBT_PN )
		{
//			if( !BRNTKY_sel || getLINE_DATA()->_XXDTA.dd_dbt != TKYBRN_SEL.BRN_KCOD ) {
//				CString tmp;
//				tmp = getLINE_DATA()->_XXDTA.dd_dbt;
//				int bmn;
//				bmn = getLINE_DATA()->_XXDTA.dd_dbmn;
//				BRNTKY_sel_set( brntky_sch( &TKYBRN_SEL, SelVal32, &m_selval32, bmn, tmp ));
//			}
			BRNCHK_PN = -1;
			return;
		}

		//別画面入力消去
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}




		if( _dline_chg( pwnd, ln, DEBT_PN, click ? 0 : 1  ) == -1 )
			return;






		if( CheckAppendSansyo() )
			return;

		// 取消データの場合はＳＥＱに
		if( isdeldata() )
		{
			SAVE_PN = DEBT_PN;
			set_focus( SEQ_PN );
			return;
		}
		else {
			SAVE_PN = -1;
			if( data_denpdialog(ln) != 0 )
				return;
		}

		Dsp_TkyToItem();	// 摘要関係表示から科目関係表示に

		// 借方科目固定
		if( INP_mode == _APPEND )
			if( getLINE_DATA()->LINE_CND[DEBT_PN].INP_sg == FALSE )

				if( (nextpn = getAUTOSELpn( get_nowln(), DEBT_PN )) != DEBT_PN )
				{
					if( KojinKamokuAutoCheck(0) ) {
						set_nowpn( DEBT_PN );
						AutoInp( nextpn );

						if( nextpn > CRED_PN ) {
							if( ! KojinKamokuAutoCheck(1) ) {
								nextpn = CRED_PN;
							}
						}
						set_focus( nextpn );
						return;
					}
				}
				else if( click ) {
					//当項目まで自動入力
					if( !is_AutoInp_use() )
						AutoInp( DEBT_PN );
				}
//				if( AUTOSEL.DEBT_FIX )
//					if( !is_AutoInp_use() )
//					{
//						OnTerminationIcsinputctrl4( VK_RETURN );
//						OnTerminationIcsinputctrl4( 0 );
//					}

		set_nowpn( DEBT_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;

	}
}

// 借方チェックデータ
long CDBDinpView::CheckDataIcsinputDebt( CWnd* pwnd, long nChar, LPCTSTR string ) 
{
	CString kmkstr;
	int edaban = -1;
	struct _KamokuData check = {0}, ans = {0};
	check.kd_eda = -1;

	// 06.30 /04 -- コード表示時は 2桁 入力でも コード入力として扱う
	BOOL bCodeInput = FALSE;

	m_TabKeyStop &= ~0x01;

	try
	{
		KamokuStr_ChkLength( string, &check, Voln1->edcol );

		int kamoku = atoi( check.kd_code );
		::ZeroMemory( &ans, sizeof(struct _KamokuData) );

		if( pAUTOSEL->KMNO_OPT == OPT_KMNO ) {
			if( ! BRNTKY_sel || ( BRNTKY_sel && kamoku < 9 ) ) {
				if( check.kd_sgn == 1 || check.kd_sgn == 5 ) {
					bCodeInput = TRUE;
				}
			}
		}

		if( (check.kd_sgn == 2 || check.kd_sgn == 3) ) {
			memcpy( check.kd_code, getLINE_DATA()->_XXDTA.dd_dbt, KMKCD_LN );
			if( check.kd_code[0] == '\0' ) {
				memcpy( check.kd_code, SREC.m_dbt, KMKCD_LN );
			}
		}

		if( dinp_modefy_ok( getLINE_DATA() ) && m_Seldata.InputItemCheck( ans, check, 0 ) == TRUE &&
			CheckKamokuInput( 0, &ans ) == 0 )
		{
			// 06.17 /03 [コード + .] 前行枝番コピー
			if( INP_mode == _APPEND ) {
				if( check.kd_sgn == 4 || check.kd_sgn == 5 ) {
					ans.kd_eda = GetZengyoBrcode( 0 );
				}
			}

			if( BRNTKY_sel && ans.kd_eda != -1 ) {
				set_brn( ans.kd_eda );
			}
			exDBkamokudata_set( DEBT_PN, getINP_CTL( pwnd, DEBT_PN )->IDC_X, &ans );
	//		data_reply( pwnd, getINP_CTL( pwnd, DEBT_PN )->IDC_X, 1, &ans);


			struct _DATA_LINE *ldata = getLINE_DATA();

			ldata->LINE_CND[ DEBT_PN ].KEY_inp = TRUE;

			if( INP_mode == _APPEND )
			{
				if( ldata->LINE_CND[ DEBT_PN ].INP_sg && ldata->LINE_CND[ CRED_PN ].INP_sg )
					TKattr = 0;
				kmkstr = ans.kd_code;
				edaban = ans.kd_eda;
				if( is_mdfybrnitem( ldata->_XXDTA.dd_dbt, ldata->_XXDTA.dd_dbr, kmkstr, edaban ) )
					TKattr2 = 0;
			}

			if( nChar == VK_TAB ) {
				m_TabKeyStop |= 0x01;
			}
		}
		else
		{
			Buzzer();

			_fillbuf( (char *)&ans, sizeof(ans), 0 );
			ans.kd_eda = -1;

			if( dinp_modefy_ok( getLINE_DATA() ) )
			{
				if( getLINE_DATA()->LINE_CND[DEBT_PN].INP_sg ) {
					struct _DATA_LINE *ldata = getLINE_DATA();

					strcpy_s( ans.kd_code, sizeof ans.kd_code, ldata->_XXDTA.dd_dbt );
					ans.kd_eda = ldata->_XXDTA.dd_dbr;
					sprintf_s( ans.kd_name, sizeof ans.kd_name, "%.14s", (LPCTSTR)ldata->_XXDTA.dd_deb_name );
					exDBkamokudata_set( DEBT_PN, getINP_CTL( pwnd, DEBT_PN )->IDC_X, &ans );
				}
				else {
					Buzzer();
					exDBkamokudata_set( DEBT_PN, getINP_CTL( pwnd, DEBT_PN )->IDC_X, &ans );
				}

			//	set_focus( DEBT_PN );
			}
			else
			{
				struct _DATA_LINE *ldata = getLINE_DATA();

				strcpy_s( ans.kd_code, sizeof ans.kd_code, ldata->_XXDTA.dd_dbt );
				ans.kd_eda = ldata->_XXDTA.dd_dbr;
				sprintf_s( ans.kd_name, sizeof ans.kd_name, "%.14s", (LPCTSTR)ldata->_XXDTA.dd_deb_name );
				exDBkamokudata_set( DEBT_PN, getINP_CTL( pwnd, DEBT_PN )->IDC_X, &ans );
			}
			return 0;
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}	

	return nChar;
}

// 08.25 /09 --- IMEを手動でＯＦＦにした時 ---
BOOL CDBDinpView::TerminationCodeCheckDebt( CString& kmkcd )
{
	int len = kmkcd.GetLength();
	if( len == 0 )	return FALSE;

	if( len > 4 ) {
		DBKNREC* pKn;
		pKn = pDBzm->DB_PjisToKnrec( kmkcd );

		if( ! pKn ) return FALSE;
	}
	else {
		long n = CheckDataIcsinputDebt( this, VK_RETURN, kmkcd );
		if( n <= 0 ) return FALSE;
		else {
			struct _KamokuData check = {0};
			exDBkamokudata_get( DEBT_PN, getINP_CTL( this, DEBT_PN )->IDC_X, &check );
			kmkcd = check.kd_code;
		}
	}

	return TRUE;
}


// 借方ターミネーション
void CDBDinpView::TerminationIcsinputDebt( CWnd* pwnd, int ln, long nChar, long inplen, long kst )
{
int st;
struct _DATA_LINE *ldata;
BOOL set_brtek	= FALSE;
BOOL bEdaCopy	= FALSE;
CString kmkcd_str;
int eda_code;
struct _KamokuData check = {0};

TRACE("TerminationIcsinputDebt top\n");

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( PROGRAM_OK == FALSE )
		return;

	try
	{
		if( nChar == 0x16 ) {
			::ZeroMemory( &check, sizeof check );
			ldata = getLINE_DATA();
			memcpy( check.kd_code, ldata->_XXDTA.dd_dbt, KMKCD_LN );
			check.kd_eda = ldata->_XXDTA.dd_dbr;
			strcpy_s( check.kd_name, sizeof check.kd_name, ldata->_XXDTA.dd_deb_name );
			
			exDBkamokudata_set( DEBT_PN, getINP_CTL( pwnd, DEBT_PN )->IDC_X, &check );
		}

//		if( !nChar )	//フォーカスＯＦＦ
//		{
			if( !nChar )
				getLINE_DATA()->LINE_CND[ DEBT_PN ].KEY_inp = FALSE;

			if( !dinp_modefy_ok( getLINE_DATA() ) )
				goto NEXT;

			exDBkamokudata_get( DEBT_PN, getINP_CTL( pwnd, DEBT_PN )->IDC_X, &check );

			// 枝番セット
			if( BRNTKY_sel && get_brn() != -1 )
			{
				eda_code = get_brn();
				check.kd_eda = eda_code;
				set_brn( -1 );	// リセット
			}

			if( !check.kd_code[0] /*check.CdNum == 0xffff*/ && getLINE_DATA()->LINE_CND[DEBT_PN].INP_sg == FALSE )
			{
				if( getLINE_DATA()->LINE_CND[DEBT_PN].NEXT_MOVE_KEY == FALSE )
//					return;
					goto NEXT;

				if( pAUTOSEL->BRCPY_OPT == OPT_NONOPT ) {	// 08.02 /02
					memcpy( check.kd_code, SREC.m_dbt, KMKCD_LN );
					check.kd_eda = -1;
				}
				else {
					memcpy( check.kd_code, SREC.m_dbt, KMKCD_LN );
					// 選択欄での枝番選択時は、セットしない
					if( !(nChar == VK_NEXT || nChar == VK_PRIOR) ) {
						check.kd_eda = SREC.m_dbr;
						// 枝番摘要を 仕訳摘要に 08.27 /04
						if( pAUTOSEL->BRTOTKY_OPT == OPT_EXEC )
							bEdaCopy = TRUE;
					}
				}

				struct	_DBKNREC* pKn;
				if( (pKn = pDBzm->DB_PjisToKnrec( SREC.m_dbt ) ) != NULL ) {
					KamokuString( pKn, check.kd_name, sizeof check.kd_name );
				}

				set_brtek = bEdaCopy ? TRUE : FALSE;

				// 入力枝番タイプセット
				ldata = getLINE_DATA();
				kmkcd_str = check.kd_code;
				eda_code = check.kd_eda;

				ldata->LINE_CND[ DEBT_PN ].INP_type = brn_inptype( ldata->LINE_CND[ DEBT_PN ].INP_type, ldata->_XXDTA.dd_dbt, ldata->_XXDTA.dd_dbr, kmkcd_str, eda_code );
			}
			else
			{
				set_brtek = FALSE;

				// 入力枝番タイプセット
				ldata = getLINE_DATA();
				kmkcd_str = check.kd_code;

				if( ! TerminationCodeCheckDebt( kmkcd_str ) ) {
					::ZeroMemory( &check, sizeof check );
					ldata = getLINE_DATA();
					kmkcd_str = ldata->_XXDTA.dd_dbt;
					memcpy( check.kd_code, ldata->_XXDTA.dd_dbt, KMKCD_LN );
					check.kd_eda = ldata->_XXDTA.dd_dbr;
					strcpy_s( check.kd_name, sizeof check.kd_name, ldata->_XXDTA.dd_deb_name );

					exDBkamokudata_set( DEBT_PN, getINP_CTL( pwnd, DEBT_PN )->IDC_X, &check );
					// 資金繰諸口枝番等セット
					set_etcdsp( ln, DEBT_PN );
					TerminationDataSet( DEBT_PN );
					return;
				}
				else {
					strcpy_s( check.kd_code, sizeof check.kd_code, kmkcd_str );
				}

				eda_code = check.kd_eda;
				ldata->LINE_CND[ DEBT_PN ].INP_type = brn_inptype( ldata->LINE_CND[ DEBT_PN ].INP_type,  ldata->_XXDTA.dd_dbt, ldata->_XXDTA.dd_dbr, kmkcd_str, eda_code );

				// 枝番摘要セット
				if( ldata->LINE_CND[ DEBT_PN ].INP_type == 1 )
				{
					// 科目.枝番 に変更があったか？
					if( pAUTOSEL->BRTOTKY_OPT == OPT_EXEC ) {
					
						kmkcd_str = check.kd_code;
						eda_code = check.kd_eda;		
						if( ldata->LINE_CND[ DEBT_PN ].KEY_inp || is_mdfybrnitem( ldata->_XXDTA.dd_dbt, ldata->_XXDTA.dd_dbr, kmkcd_str, eda_code ) )
							set_brtek = TRUE;
					}
				}
			}

			ldata->LINE_CND[ DEBT_PN ].KEY_inp = FALSE;
			//諸口・現金貸借
			if( INP_mode != _APPEND && check.kd_code[0] != '\0'  ) //#* 03.07 /02
			{
				syog_genk( &CREC, _SUB );
			}

			int clear = 0;
			int kmk_chg = 0;

			if( check.kd_code[0] != '\0' )
			{
				SREC.m_dbt = check.kd_code;
				SREC.m_dbr = check.kd_eda;

				if( CREC.m_dbt != SREC.m_dbt ) {
					m_NOBRNCHK_CND &= ~0x01;	// 借方枝番未入力状況サインＯＦＦ
					m_BMKJICHK_CND  &= ~0x010101;
					if( IsSyafuKaisei( Voln1 ) )
						clear = 1;

					kmk_chg++;
				}
				if(CREC.m_dbr != SREC.m_dbr)
					kmk_chg++;

				kmkbmn_change( 0, &CREC, SREC.m_dbt );
				CREC.m_dbt = SREC.m_dbt;
				CREC.m_dbr = SREC.m_dbr;

				ldata = getLINE_DATA();
				ldata->_XXDTA.dd_deb_name = check.kd_name;
				ldata->_XXDTA.dd_dbt = check.kd_code;
				ldata->_XXDTA.dd_dbr = check.kd_eda;
				getLINE_DATA()->LINE_CND[DEBT_PN].INP_sg = TRUE;
			}
			else
			{
				ldata = getLINE_DATA();
				kmkcd_str = ldata->_XXDTA.dd_dbt;
				memcpy( check.kd_code, ldata->_XXDTA.dd_dbt, KMKCD_LN );
				check.kd_eda = ldata->_XXDTA.dd_dbr;
				strcpy_s( check.kd_name, sizeof check.kd_name, ldata->_XXDTA.dd_deb_name );

				exDBkamokudata_set( DEBT_PN, getINP_CTL( pwnd, DEBT_PN )->IDC_X, &check );
				// 資金繰諸口枝番等セット
				set_etcdsp( ln, DEBT_PN );

//				MessageBeep( 0xffffffff );
				TerminationDataSet( DEBT_PN );
				return;
			}

			//消費税
			getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt( &CREC, NULL, INP_mode);
			if (kmk_chg && INP_mode == _APPEND && pDBzm->IsKobetsuBmnSyz()) {
				DWORD dwKobe = pDBzm->KobetsuSiwakeCheck(&CREC, CREC.m_dbmn);
				if (dwKobe) {
					pDBzm->SetCDBData(&CREC);
					int st = pDBsy->SyFnc(dwKobe);
					getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt(&CREC, NULL, INP_mode);
				}
			}
			memcpy( getLINE_DATA()->_XXDTA.dd_zei, CREC.m_zei, 6 );

			//諸口・現金貸借
			if( INP_mode != _APPEND && check.kd_code[0] != '\0' ) //#* 03.07 /02
			{
				syog_genk( &CREC, _ADD );
			}

			//消費税再表示
			int pntbl[] = {ZEI_PN, SKBN_PN};
			if( is_syohizeisiwake( &CREC ) ) {
				getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = TRUE;
			}
			else {
				getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = FALSE;
			}
			TerminationDataSet( pntbl, 2 );
			//変動事由・振替科目[公益]チェック
			HendJiyuCheck( &CREC, clear );
			InvnoCheck(&CREC);

			if( set_brtek )
			{
				// 枝番摘要を摘要に
				if( ! brnitm_to_tky( ln, CREC.m_dbmn, CREC.m_dbt, CREC.m_dbr /*check.ip_kmk.kamoku, check.IP_BRN*/ ) )
					ldata->LINE_CND[ DEBT_PN ].INP_type = 3;	// 枝番登録に期待
			}

			if( !nChar )
			{
				PROGRAM_OK = FALSE;
				{
					if( !FNC_BUTTON )
					{

#ifdef SWKJOB	//#* 01.15 /02 仕訳のチェック
						if( swk_check( DEBT_PN, &CREC ) == 0 ) 
						{
#endif						

							//枝番チェック
							st = brchk1( &CREC, 0 );
//BEGIN_PERFORMANCE
							if( st == 99 )
							{
								st = brzan_torok( this, CREC.m_dbmn, CREC.m_dbt, CREC.m_dbr, 0 );		//枝番登録選択

								if( st == 0 )
								{
									// 枝番摘要 -> 仕訳摘要
									if( pAUTOSEL->BRTOTKY_OPT == OPT_EXEC )
										brnitm_to_tky( ln, CREC.m_dbmn, CREC.m_dbt, CREC.m_dbr );
									pDBzm->IsModify() = TRUE;

									if (pDBzm->IsDiffEdabanSyzSgn((char*)(LPCTSTR)CREC.m_dbt, CREC.m_dbr)) {
										if (pDBsy->SyReOpen(pDBzm) == -1) {
											ermset(0, "消費税モジュール再オープンに失敗しました。");
											return;
										}
										//枝番なしでイニシャライズしてから、枝番をセットして消費税属性を得る
										int neweda = CREC.m_dbr;
										CREC.m_dbr = -1;
										DB_SyIniz(&CREC);
										CREC.m_dbr = neweda;
										getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt(&CREC, NULL, INP_mode);
										memcpy(getLINE_DATA()->_XXDTA.dd_zei, CREC.m_zei, 6);
										//消費税再表示
										int pntbl[] = { ZEI_PN, SKBN_PN };
										if (is_syohizeisiwake(&CREC)) {
											getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = TRUE;
										}
										else {
											getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = FALSE;
										}
										TerminationDataSet(pntbl, 2);
									}
									m_TaiTtl.init_taiclq();
								}
								else if( st == -1 )				//キャンセル
								{
									del_dtaeda( &CREC, 0 );		//仕訳枝番消去
								}
								else {
									return;						//Fork登録
								}
							}
							else if( st == 0 ) {
								// 枝番すでに登録済み か、ゼロ登録
							}
							else if( st == 2 || st == 3 ) {	// 枝番無しか登録MAXなので枝番名称を表示しない
							}

							if( st )
							{
								if( getLINE_DATA()->_XXDTA.dd_dbr != CREC.m_dbr )
								{
									// 入力枝番タイプOFFセット
									getLINE_DATA()->LINE_CND[ DEBT_PN ].INP_type = brn_dfltinptype( CREC.m_dbr );
								}
								getLINE_DATA()->_XXDTA.dd_dbr = SREC.m_dbr = CREC.m_dbr;	// 0xffff

							}

#ifdef SWKJOB	//#* 01.15 /02 仕訳のチェック
						}
#endif
					}
				}
				PROGRAM_OK = TRUE;
			}

			::ZeroMemory( &check, sizeof check );
			ldata = getLINE_DATA();
			memcpy( check.kd_code, ldata->_XXDTA.dd_dbt, KMKCD_LN );
			check.kd_eda = ldata->_XXDTA.dd_dbr;
			strcpy_s( check.kd_name, sizeof check.kd_name, ldata->_XXDTA.dd_deb_name );
			
			exDBkamokudata_set( DEBT_PN, getINP_CTL( pwnd, DEBT_PN )->IDC_X, &check );

			// 資金繰諸口枝番等セット
			set_etcdsp( ln, DEBT_PN );
			TerminationDataSet( DEBT_PN );

//			return;
//		}

NEXT:
		getLINE_DATA()->LINE_CND[DEBT_PN].NEXT_MOVE_KEY = FALSE;

		if( !nChar )
		{
			return;
		}

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return;

		if( nChar )
		{
			nChar = SelItemDsp( nChar );
			if( !nChar )
				return;

			if( nChar == VK_UP || nChar == VK_DOWN ) {
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			nChar = dline_chg( pwnd, nChar );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合はＳＥＱに
				if( isdeldata() )
				{
					SAVE_PN = DEBT_PN;
					set_focus( SEQ_PN );
					return;
				}
				else
					SAVE_PN = -1;

				set_focus( DEBT_PN );
				return;
			}
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
		{
			exDBkamokudata_get( DEBT_PN, getINP_CTL( pwnd, DEBT_PN )->IDC_X, &check );

			// 枝番摘要検索＆表示
			if( (nChar == VK_TAB || (nChar == VK_RETURN && !is_AutoInp_use() && getLINE_DATA()->_XXDTA.dd_dbr == -1 )) 
									&& dinp_modefy_ok( getLINE_DATA() ) )
			{
				int dspstr = 0;

				if( check.kd_code[0] != 0 ) {
					kmkcd_str = check.kd_code;
				}
				else {
					kmkcd_str = SREC.m_dbt;
					check.kd_eda = SREC.m_dbr;

					int type =  IsKojinGyosyuMaster( pDBzm );
					if( type ) {
						if( IsKojinKengyoMaster( type ) ) {
							struct _KamokuData  akd = {0}, ckd = {0};

							strcpy_s( ckd.kd_code, sizeof ckd.kd_code, kmkcd_str );
							ckd.kd_sgn = 2;	// 科目コードチェック用に 枝番のみのサインセット
							if( ! m_Seldata.InputItemCheck( akd, ckd, 0 ) ) {
								Buzzer();
								return;
							}
						}
					}
					dspstr++;
				//	struct	_DBKNREC* pKn;
				//	if( (pKn = pDBzm->DB_PjisToKnrec( kmkcd_str ) ) != NULL ) {
				//		memmove( check.kd_name, pKn->knnam, 14 );
				//	}
				}
				int bmn = getLINE_DATA()->_XXDTA.dd_dbmn;

		//		BRNTKY_PAR bpar;
		//		bpar = m_Seldata.GetBrnTkyPar();
		//		if( /*!BRNTKY_sel ||*/ kmkcd_str.CompareNoCase( bpar.bt_code ) != 0 ) {
				if( EdabanDispReload( kmkcd_str ) ) {
					if( BRNTKY_select( bmn, kmkcd_str, NULL, nChar == VK_TAB ? TRUE : FALSE ) )
					{
						::ZeroMemory( saveITEM50kana, sizeof saveITEM50kana );
						if( nChar == VK_RETURN )	nChar = VK_TAB;

						if( dspstr ) {	//科目未入力時のみ
							struct	_DBKNREC* pKn;
							if( (pKn = pDBzm->DB_PjisToKnrec( kmkcd_str ) ) != NULL ) {
								KamokuString( pKn, check.kd_name, sizeof check.kd_name );
								exDBSetDispString( DEBT_PN, getINP_CTL(pwnd, DEBT_PN)->IDC_X, check.kd_name );
							}
						}
					}
					else
					{
						//枝番摘要なし
						if( (m_TabKeyStop & 0x01) && nChar == VK_TAB ) {
							m_TabKeyStop &= ~0x01;
						}
						else {
							nChar = VK_RETURN;
						}
					}
				}
				else	nChar = VK_RETURN;
			}
#ifdef DB_KEEP
			if( check.kd_code[0] == 0/*0xffff*/ )
			{
				::ZeroMemory( &check, sizeof check );
				ldata = getLINE_DATA();
				memcpy( check.kd_code, ldata->_XXDTA.dd_dbt, KMKCD_LN );
				check.kd_eda = ldata->_XXDTA.dd_dbr;
				strcpy( check.kd_name, ldata->_XXDTA.dd_deb_name );

				exDBkamokudata_set( DEBT_PN, getINP_CTL( pwnd, DEBT_PN )->IDC_X, &check );

				// 資金繰諸口枝番等セット
				set_etcdsp( ln, DEBT_PN );
				TerminationDataSet( DEBT_PN );
				return;
			}
			else if( !check.kd_code[0] && !BOTOM_SEQ )
#endif

			if( nChar == VK_TAB && ! dinp_modefy_ok( getLINE_DATA() ) ) {
				nChar = VK_RETURN;
			}

			if( ! check.kd_code[0] && !BOTOM_SEQ )
			{
				Buzzer();
				return;
			}
			// 現在の業種でない科目の 空うちEnter での 入力をチェックする。
			int type =  IsKojinGyosyuMaster( pDBzm );
			if( type ) {
				if( IsKojinKengyoMaster( type ) ) {
					if( ! check.kd_code[0] ) {
						struct _KamokuData  akd = {0}, ckd = {0};

						strcpy_s( ckd.kd_code, sizeof ckd.kd_code,  SREC.m_dbt );
						ckd.kd_sgn = 2;	// 科目コードチェック用に 枝番のみのサインセット
						if( ! m_Seldata.InputItemCheck( akd, ckd, 0 ) ) {
							Buzzer();
							return;
						}
					}
				}
			}


			if( getLINE_DATA()->_XXDTA.dd_dbr == -1 ) {
				if( pAUTOSEL->BRNKAK_OPT == OPT_BRNKAK ) {
					// 枝番残高があるか？
					if( pDBzm->DB_EdabanZanSearch( CString(check.kd_code), -1 ) == 0 )
						return;
				}
			}

			getLINE_DATA()->LINE_CND[DEBT_PN].NEXT_MOVE_KEY = TRUE;

			if( !is_AutoInp_use() && nChar != VK_TAB ) {
				set_termfocus( get_nowln(), DEBT_PN, 1 );
			//	set_focus( CRED_PN );
			}
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 )
		{
			set_termfocus( get_nowln(), DEBT_PN, -1 );

#ifdef LATER_CLOSE
			if( getINP_CTL( pwnd, BMON_PN )->INP_ena )
				set_focus( BMON_PN );
			// 伝票番号「なし」モードで、/*伝票番号が無い場合*/
			else if( (pAUTOSEL->DENP_OPT == OPT_NON) /*&& (getLINE_DATA()->_XXDTA.dd_cno == (WORD)0xffff)*/ )
				set_focus( DATE_PN );	// 日付に戻る。
			else
				set_focus( DENP_PN );
#endif
		}
		else {
			if( nChar != 0x16 )
				Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


/***************************
	借方カナチェック
****************************/
void CDBDinpView::KanaTermIcsinputDebt(LPCTSTR data) 
{
	char k_data[20] = {0};
	char kana[20] = {0};

	if( KanaSearchType() )
		return;

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	try
	{

	//	strcpy_s( kana, sizeof kana, (char *)data );
		kana_copy( k_data, data );
		kana_copy( kana, data );

		SelPar par;
		if( BRNTKY_sel ) {
			par.sel_pn = SL_BRNTKY_PN;
			par.sel_flg = 1;

			BRNTKY_PAR brp;
			brp = m_Seldata.GetBrnTkyPar();

			par.tkybrn	= brp.bt_code;
			par.brn_bmn = brp.bt_bmn;
			// 絞込み
			sprintf_s( par.brn_kana, sizeof par.brn_kana, "%s%s", saveITEM50kana, data );
		}
		else if( m_SelDispPN == SL_KMKANA_PN ) {
			par.sel_pn = SL_KMKANA_PN;
#ifdef CLOSE // 07.17 /07
			sprintf_s( kana, sizeof kana, "%s%s", saveITEM50kana, k_data );
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
			if( ! BRNTKY_sel )	Buzzer();
		}

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


// 貸方フォーカス処理
#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsCred( CWnd* pwnd, int ln, short click )
{
	_FocusIcsCred( pwnd, ln, click );

	OnICSInputFocus( CRED_PN );
}
void CDBDinpView::_FocusIcsCred( CWnd* pwnd, int ln, short click )
{
#else
void CDBDinpView::FocusIcsCred( CWnd* pwnd, int ln, short click )
{
#endif
	int nextpn;

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
		if( BRNCHK_PN != -1 )
		{
			if( BRNCHK_PN == CRED_PN ) {
//				if( !BRNTKY_sel || getLINE_DATA()->_XXDTA.dd_cre != TKYBRN_SEL.BRN_KCOD ) {
//					CString tmp;
//					tmp = getLINE_DATA()->_XXDTA.dd_cre;
//					int bmn;
//					bmn = getLINE_DATA()->_XXDTA.dd_cbmn;
//					BRNTKY_sel_set( brntky_sch( &TKYBRN_SEL, SelVal32, &m_selval32, bmn, tmp));
//				}
				BRNCHK_PN = -1;
			}
			return;
		}

		//別画面入力消去
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}

		if( _dline_chg( pwnd, ln, CRED_PN, click ? 0 : 1  ) == -1 )
			return;

		if( CheckAppendSansyo() )
			return;

		// 取消データの場合はＳＥＱに
		if( isdeldata() )
		{
			SAVE_PN = CRED_PN;
			set_focus( SEQ_PN );
			return;
		}
		else {
			SAVE_PN = -1;
			if( data_denpdialog(ln) != 0 )
				return;
		}

		//直前のポジションが当ポジションより前で、借方科目が入力されていない場合は直前のポジションに返す。
		int set_pn;
		if( inp_ok( get_nowpn(), CRED_PN, &set_pn ) == FALSE )
		{
			set_nowpn( set_pn );
			set_focus( /*get_nowpn()*/ set_pn );
			return;
		}

		Dsp_TkyToItem();	// 摘要関係表示から科目関係表示に

		// 貸方科目固定
		if (INP_mode == _APPEND) {
			if (click) {
				if (getLINE_DATA()->LINE_CND[DEBT_PN].INP_sg == FALSE) {
					if (!KojinKamokuAutoCheck(0)) {
						set_focus(DEBT_PN);
						return;
					}
					else {
						AutoInp(CRED_PN);
					}
				}
			}

			if (getLINE_DATA()->LINE_CND[CRED_PN].INP_sg == FALSE) {

				if ((nextpn = getAUTOSELpn(get_nowln(), CRED_PN)) != CRED_PN)
				{
					if (getLINE_DATA()->LINE_CND[DEBT_PN].INP_sg == FALSE) {
						if (!KojinKamokuAutoCheck(0)) {
							set_focus(DEBT_PN);
							return;
						}
					}

					if (KojinKamokuAutoCheck(1)) {
						set_nowpn(CRED_PN);	// ターミネーションOFF での正しい項目位置をセット
						AutoInp(nextpn);
						set_focus(nextpn);
						return;
					}
				}
				else if (click) {
					//当項目まで自動入力
					if (!is_AutoInp_use())
						AutoInp(CRED_PN);
				}
			}
		}
		set_nowpn( CRED_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


// 貸方チェックデータ
long CDBDinpView::CheckDataIcsinputCred( CWnd* pwnd, long nChar, LPCTSTR string )
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString kmkstr;
	int	edaban = -1;
	struct _KamokuData check = {0}, ans = {0};
	check.kd_eda = -1;
	// 06.30 /04 -- コード表示時は 2桁 入力でも コード入力として扱う
	BOOL bCodeInput = FALSE;

	m_TabKeyStop &= ~0x02;

	try
	{
		KamokuStr_ChkLength( string, &check, Voln1->edcol );

		int kamoku = atoi( check.kd_code );
		::ZeroMemory( &ans, sizeof(struct _KamokuData) );

		if( pAUTOSEL->KMNO_OPT == OPT_KMNO ) {
			if( ! BRNTKY_sel || ( BRNTKY_sel && kamoku < 9 ) ) {
				if( check.kd_sgn == 1 || check.kd_sgn == 5 ) {
					bCodeInput = TRUE;
				}
			}
		}

		if( (check.kd_sgn == 2 || check.kd_sgn == 3) ) {
			memcpy( check.kd_code, getLINE_DATA()->_XXDTA.dd_cre, KMKCD_LN );
			if( check.kd_code[0] == '\0' ) {
				memcpy( check.kd_code, SREC.m_cre, KMKCD_LN );
			}
		}

		if( dinp_modefy_ok( getLINE_DATA() ) && m_Seldata.InputItemCheck( ans, check, 1 ) == TRUE &&
			CheckKamokuInput( 1, &ans ) == 0 )
		{
			// 06.17 /03 [コード + .] 前行枝番コピー
			if( INP_mode == _APPEND ) {
				if( check.kd_sgn == 4 || check.kd_sgn == 5 ) {
					ans.kd_eda = GetZengyoBrcode( 1 );
				}
			}

			if( BRNTKY_sel && ans.kd_eda != -1 ) {
				set_brn( ans.kd_eda );
			}
			exDBkamokudata_set( CRED_PN, getINP_CTL( pwnd, CRED_PN )->IDC_X, &ans);

			struct _DATA_LINE *ldata = getLINE_DATA();

			ldata->LINE_CND[ CRED_PN ].KEY_inp = TRUE;

			if( INP_mode == _APPEND )
			{
				if( ldata->LINE_CND[ DEBT_PN ].INP_sg && ldata->LINE_CND[ CRED_PN ].INP_sg )
					TKattr = 0;

				kmkstr = ans.kd_code;
				edaban = ans.kd_eda;
				if( is_mdfybrnitem( ldata->_XXDTA.dd_cre, ldata->_XXDTA.dd_cbr, kmkstr, edaban ) )
					TKattr2 = 0;
			}

			if( nChar == VK_TAB ) {
				m_TabKeyStop |= 0x02;
			}
		}
		else
		{
			Buzzer();

			_fillbuf( (char *)&ans, sizeof(ans), 0 );
			ans.kd_eda = -1;

			if( dinp_modefy_ok( getLINE_DATA() ) )
			{
				if( getLINE_DATA()->LINE_CND[CRED_PN].INP_sg ) {
					struct _DATA_LINE *ldata = getLINE_DATA();
					memcpy( ans.kd_code, ldata->_XXDTA.dd_cre, KMKCD_LN );
					ans.kd_eda = ldata->_XXDTA.dd_cbr;
					memcpy( ans.kd_name, ldata->_XXDTA.dd_cre_name, sizeof ans.kd_name );
					exDBkamokudata_set( CRED_PN, getINP_CTL( pwnd, CRED_PN )->IDC_X, &ans);
				}
				else {
					Buzzer();
					exDBkamokudata_set( CRED_PN, getINP_CTL( pwnd, CRED_PN )->IDC_X, &ans);
				}

		//		set_focus( CRED_PN );
			}
			else
			{
				struct _DATA_LINE *ldata = getLINE_DATA();
				memcpy( ans.kd_code, ldata->_XXDTA.dd_cre, KMKCD_LN );
				ans.kd_eda = ldata->_XXDTA.dd_cbr;
				memcpy( ans.kd_name, ldata->_XXDTA.dd_cre_name, sizeof ans.kd_name );
				exDBkamokudata_set( CRED_PN, getINP_CTL( pwnd, CRED_PN )->IDC_X, &ans);
			}

			return 0;
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}	
	return nChar;
}


// 08.25 /09 --- IMEを手動でＯＦＦにした時 ---
BOOL CDBDinpView::TerminationCodeCheckCred( CString& kmkcd )
{
	int len = kmkcd.GetLength();
	if( len == 0 )	return FALSE;

	if( len > 4 ) {
		DBKNREC* pKn;
		pKn = pDBzm->DB_PjisToKnrec( kmkcd );

		if( ! pKn ) return FALSE;
	}
	else {
		long n = CheckDataIcsinputCred( this, VK_RETURN, kmkcd );
		if( n <= 0 ) return FALSE;
		else {
			struct _KamokuData check = {0};
			exDBkamokudata_get( CRED_PN, getINP_CTL( this, CRED_PN )->IDC_X, &check );
			kmkcd = check.kd_code;
		}
	}

	return TRUE;
}

// 貸方 ターミネーション
void CDBDinpView::TerminationIcsinputCred( CWnd* pwnd, int ln, long nChar, long inplen, long kst )
{
int st;
//INPUTDATA check;
struct _DATA_LINE *ldata;
BOOL set_brtek	= FALSE;
BOOL bEdaCopy	= FALSE;
CString kmkcd_str;
int eda_code;
struct _KamokuData check = {0};

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( PROGRAM_OK == FALSE )
		return;

//MyTrace("TermCred TOP ln = %d, nChar = %d, inplen = %d\n", ln, nChar, inplen);
	try
	{

		// コピーデータは無効にする
		if( nChar == 0x16 ) {
			::ZeroMemory( &check, sizeof check );
			ldata = getLINE_DATA();
			memcpy( check.kd_code, ldata->_XXDTA.dd_cre, KMKCD_LN );
			check.kd_eda = ldata->_XXDTA.dd_cbr;
			strcpy_s( check.kd_name, sizeof check.kd_name, ldata->_XXDTA.dd_cre_name );
			
			exDBkamokudata_set( CRED_PN, getINP_CTL( pwnd, CRED_PN )->IDC_X, &check );
		}

//		if( !nChar )
//		{

			if( !nChar )
				getLINE_DATA()->LINE_CND[ CRED_PN ].KEY_inp = FALSE;

			if( !dinp_modefy_ok( getLINE_DATA() ) )
				goto NEXT;

			exDBkamokudata_get( CRED_PN, getINP_CTL( pwnd, CRED_PN )->IDC_X, &check );

			// 枝番セット
			if( BRNTKY_sel && get_brn() != -1 )
			{
				check.kd_eda = get_brn();
				set_brn( -1 );	// リセット
			}

			if( !check.kd_code[0] /*check.CdNum == 0xffff*/ && getLINE_DATA()->LINE_CND[CRED_PN].INP_sg == FALSE )
			{
//#ifdef DBCUT
				if( getLINE_DATA()->LINE_CND[CRED_PN].NEXT_MOVE_KEY == FALSE )
//					return;
					goto NEXT;
//#endif

				if( pAUTOSEL->BRCPY_OPT == OPT_NONOPT ) {	// 08.02 /02
					memcpy( check.kd_code, SREC.m_cre, KMKCD_LN );
					check.kd_eda = -1;
				}
				else {
					memcpy( check.kd_code, SREC.m_cre, KMKCD_LN );
					if( !(nChar == VK_NEXT || nChar == VK_PRIOR) ) {
						check.kd_eda = SREC.m_cbr;
						// 枝番摘要を 仕訳摘要に 08.27 /04
						if( pAUTOSEL->BRTOTKY_OPT == OPT_EXEC )
							bEdaCopy = TRUE;
					}
				}

				struct	_DBKNREC* pKn;
				if( (pKn = pDBzm->DB_PjisToKnrec( SREC.m_cre )) != NULL ) {
					KamokuString( pKn, check.kd_name, sizeof check.kd_name );
				}

				set_brtek = bEdaCopy ? TRUE : FALSE;

				// 入力枝番タイプセット
				ldata = getLINE_DATA();
				kmkcd_str = check.kd_code;
				eda_code = check.kd_eda;

				ldata->LINE_CND[ CRED_PN ].INP_type = brn_inptype( ldata->LINE_CND[ CRED_PN ].INP_type, CString(ldata->_XXDTA.dd_cre), ldata->_XXDTA.dd_cbr, kmkcd_str, eda_code );
			}
			else
			{
				set_brtek = FALSE;
				// 入力枝番タイプセット
				ldata = getLINE_DATA();
				kmkcd_str = check.kd_code;

				if( ! TerminationCodeCheckCred( kmkcd_str ) ) {
					::ZeroMemory( &check, sizeof check );
					ldata = getLINE_DATA();
					kmkcd_str = ldata->_XXDTA.dd_cre;
					memcpy( check.kd_code, ldata->_XXDTA.dd_cre, KMKCD_LN );
					check.kd_eda = ldata->_XXDTA.dd_cbr;
					strcpy_s( check.kd_name, sizeof check.kd_name, ldata->_XXDTA.dd_cre_name );

					exDBkamokudata_set( CRED_PN, getINP_CTL( pwnd, CRED_PN )->IDC_X, &check );
					// 資金繰諸口枝番等セット
					set_etcdsp( ln, CRED_PN );
					TerminationDataSet( CRED_PN );

					return;
				}
				else {
					strcpy_s( check.kd_code, sizeof check.kd_code, kmkcd_str );
				}
				eda_code = check.kd_eda;

				ldata->LINE_CND[ CRED_PN ].INP_type = brn_inptype( ldata->LINE_CND[ CRED_PN ].INP_type, CString(ldata->_XXDTA.dd_cre), ldata->_XXDTA.dd_cbr, kmkcd_str, eda_code );

				// 枝番摘要セット
				if( ldata->LINE_CND[ CRED_PN ].INP_type == 1 )
				{
					// 科目.枝番 に変更があったか？
					if( pAUTOSEL->BRTOTKY_OPT == OPT_EXEC ) {
						kmkcd_str = check.kd_code;
						eda_code = check.kd_eda;
						if( ldata->LINE_CND[ CRED_PN ].KEY_inp || is_mdfybrnitem( CString(ldata->_XXDTA.dd_cre), ldata->_XXDTA.dd_cbr, kmkcd_str, eda_code ) )
							set_brtek = TRUE;
					}
				}
			}

			ldata->LINE_CND[ CRED_PN ].KEY_inp = FALSE;
			//諸口・現金貸借
			if( INP_mode != _APPEND && check.kd_code[0] != '\0'/*0xffff*/ ) //#* 03.07 /02
			{
				syog_genk( &CREC, _SUB );
			}

			int clear = 0;
			int kmk_chg = 0;

			if( check.kd_code[0] != '\0'/*0xffff*/ )
			{
			//	memmove( &SREC.m_cre, &check.IP_CdNum, 4 );
				SREC.m_cre = check.kd_code;
				SREC.m_cbr = check.kd_eda;

				if( CREC.m_cre != SREC.m_cre ) {
					m_NOBRNCHK_CND &= ~0x02;	// 貸方枝番未入力状況サインＯＦＦ
					m_BMKJICHK_CND  &= ~0x020202;
					if( IsSyafuKaisei( Voln1 ) )
						clear = 1;

					kmk_chg++;
				}
				if (CREC.m_cbr != SREC.m_cbr)
					kmk_chg++;

				kmkbmn_change( 1, &CREC, SREC.m_cre );

				CREC.m_cre = SREC.m_cre;
				CREC.m_cbr = SREC.m_cbr;

				ldata->_XXDTA.dd_cre_name = check.kd_name;
				ldata->_XXDTA.dd_cre = check.kd_code;
				ldata->_XXDTA.dd_cbr = check.kd_eda;
				getLINE_DATA()->LINE_CND[CRED_PN].INP_sg = TRUE;
			}
			else
			{
				ldata = getLINE_DATA();
				memcpy( check.kd_code, ldata->_XXDTA.dd_cre, KMKCD_LN );
				check.kd_eda = ldata->_XXDTA.dd_cbr;
				memcpy( check.kd_name, ldata->_XXDTA.dd_cre_name, sizeof check.kd_name );

				exDBkamokudata_set( CRED_PN, getINP_CTL( pwnd, CRED_PN )->IDC_X, &check );

				// 資金繰諸口枝番等セット
				set_etcdsp( ln, CRED_PN );
				TerminationDataSet( CRED_PN );
//				MessageBeep( 0xffff );
				return;
			}

			getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt( &CREC, NULL, INP_mode);
			//消費税
			if (kmk_chg && INP_mode == _APPEND && pDBzm->IsKobetsuBmnSyz()) {
				DWORD dwKobe = pDBzm->KobetsuSiwakeCheck(&CREC, CREC.m_cbmn);
				if (dwKobe) {
					pDBzm->SetCDBData(&CREC);
					int st = pDBsy->SyFnc(dwKobe);
					getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt(&CREC, NULL, INP_mode);
				}
			}
			memcpy( getLINE_DATA()->_XXDTA.dd_zei, CREC.m_zei, 6 );

			//諸口・現金貸借
			if( INP_mode != _APPEND && check.kd_code[0] != '\0' ) //#* 03.07 /02
			{
				syog_genk( &CREC, _ADD );
			}

			int pntbl[] = {ZEI_PN, SKBN_PN};
			if (is_syohizeisiwake( &CREC )) {
				getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = TRUE;
			}
			else {
				getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = FALSE;
			}
			TerminationDataSet( pntbl, 2 );
			//変動事由・振替科目[公益]チェック
			HendJiyuCheck( &CREC, clear );
			InvnoCheck(&CREC);

			if( set_brtek )
			{
				// 枝番摘要を摘要に
				if( !brnitm_to_tky( ln, CREC.m_cbmn, CREC.m_cre, CREC.m_cbr /*check.IP_CdNum, check.IP_BRN*/ ) )
					ldata->LINE_CND[ CRED_PN ].INP_type = 3;	// 枝番登録に期待
			}

			if( !nChar )
			{
				PROGRAM_OK = FALSE;
				{
					if( !FNC_BUTTON )
					{
#ifdef SWKJOB	//#* 01.15 /02 仕訳のチェック
						if( swk_check( CRED_PN, &CREC ) == 0 ) 
						{
#endif
							//枝番チェック
							st = brchk1( &CREC, 1 );
							if( st == 99 )
							{
								st = brzan_torok( this, CREC.m_cbmn, CREC.m_cre, CREC.m_cbr, 1 );		//枝番登録選択
								
								if( st == 0 )
								{
									// 枝番摘要 -> 仕訳摘要
									if( pAUTOSEL->BRTOTKY_OPT == OPT_EXEC )
										brnitm_to_tky( ln, CREC.m_cbmn, CREC.m_cre, CREC.m_cbr );

									pDBzm->IsModify() = TRUE;

									if (pDBzm->IsDiffEdabanSyzSgn((char*)(LPCTSTR)CREC.m_cre, CREC.m_cbr)) {
										if (pDBsy->SyReOpen(pDBzm) == -1) {
											ermset(0, "消費税モジュール再オープンに失敗しました。");
											return;
										}
										//枝番なしでイニシャライズしてから、枝番をセットして消費税属性を得る
										int neweda = CREC.m_cbr;
										CREC.m_cbr = -1;
										DB_SyIniz(&CREC);
										CREC.m_cbr = neweda;
										getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt(&CREC, NULL, INP_mode);
										memcpy(getLINE_DATA()->_XXDTA.dd_zei, CREC.m_zei, 6);
										//消費税再表示
										int pntbl[] = { ZEI_PN, SKBN_PN };
										if (is_syohizeisiwake(&CREC)) {
											getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = TRUE;
										}
										else {
											getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = FALSE;
										}
										TerminationDataSet(pntbl, 2);
									}
									m_TaiTtl.init_taiclq();
								}
								else if( st == -1 )				//キャンセル
								{
									del_dtaeda( &CREC, 1 );		//仕訳枝番消去
								}
								else {
									return;						//Fork登録
								}
							}
							else if( st == 0 ) {
								// 枝番登録済みか、ゼロ登録
							}
							else if( st == 2 || st == 3 ) {	// 枝番無しか登録MAXなので枝番名称を表示しない
							}

							if( st )
							{
								if( getLINE_DATA()->_XXDTA.dd_cbr != CREC.m_cbr )
								{
									// 入力枝番タイプOFFセット
									getLINE_DATA()->LINE_CND[ CRED_PN ].INP_type = brn_dfltinptype( CREC.m_cbr );
								}
								getLINE_DATA()->_XXDTA.dd_cbr = SREC.m_cbr = CREC.m_cbr;	// 0xffff
							}
#ifdef SWKJOB	//#* 01.15 /02 仕訳のチェック
						}
#endif
					}
				}
				PROGRAM_OK = TRUE;
			}

			::ZeroMemory( &check, sizeof check );
			ldata = getLINE_DATA();
			memcpy( check.kd_code, ldata->_XXDTA.dd_cre, KMKCD_LN );
			check.kd_eda = ldata->_XXDTA.dd_cbr;
		//	memcpy( check.kd_name, ldata->_XXDTA.dd_cre_name, sizeof check.kd_name );
			strcpy_s( check.kd_name, sizeof check.kd_name, ldata->_XXDTA.dd_cre_name );
			
			exDBkamokudata_set( CRED_PN, getINP_CTL( pwnd, CRED_PN )->IDC_X, &check );

			// 資金繰諸口枝番等セット
			set_etcdsp( ln, CRED_PN );
			TerminationDataSet( CRED_PN );

//			return;
//		}

NEXT:
		getLINE_DATA()->LINE_CND[CRED_PN].NEXT_MOVE_KEY	= FALSE;

		if( !nChar )
		{
			return;
		}
		
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return;

		if( nChar )
		{
			nChar = SelItemDsp( nChar );
			if( !nChar )
				return;

			if( nChar == VK_UP || nChar == VK_DOWN ) {
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			nChar = dline_chg( pwnd, nChar );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合はＳＥＱに
				if( isdeldata() )
				{
					SAVE_PN = CRED_PN;
					set_focus( SEQ_PN );
					return;
				}
				else
					SAVE_PN = -1;

				set_focus( CRED_PN );
				return;
			}
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
		{
			exDBkamokudata_get( CRED_PN, getINP_CTL( pwnd, CRED_PN )->IDC_X, &check );

			// 枝番摘要検索＆表示
			if( (nChar == VK_TAB || (nChar == VK_RETURN && !is_AutoInp_use() && getLINE_DATA()->_XXDTA.dd_cbr == -1 )) 
									&& dinp_modefy_ok( getLINE_DATA() ) )
			{
		//		unsigned short code;
		//		code = (check.IP_CdNum != 0 ? (unsigned short)check.IP_CdNum : (unsigned short)SREC.m_cre);

				int dspstr = 0;

				if( check.kd_code[0] != 0 ) {
					kmkcd_str = check.kd_code;
			//		pDBzm->TV_kamokucodeConv( check.ip_kmk.kamoku, kmkcd_str, 1 );
				}
				else {
					kmkcd_str = SREC.m_cre;
					int type =  IsKojinGyosyuMaster( pDBzm );
					if( type ) {
						if( IsKojinKengyoMaster( type ) ) {
							struct _KamokuData  akd = {0}, ckd = {0};

							strcpy_s( ckd.kd_code, sizeof ckd.kd_code, kmkcd_str );
							ckd.kd_sgn = 2;	// 科目コードチェック用に 枝番のみのサインセット
							if( ! m_Seldata.InputItemCheck( akd, ckd, 0 ) ) {
								Buzzer();
								return;
							}
						}
					}
					dspstr++;
				}
				int bmn;
				bmn = getLINE_DATA()->_XXDTA.dd_cbmn;

			//	BRNTKY_PAR bpar;
			//	bpar = m_Seldata.GetBrnTkyPar();
			//	if( /*!BRNTKY_sel ||*/ kmkcd_str.CompareNoCase( bpar.bt_code ) != 0 ) {
				if( EdabanDispReload( kmkcd_str ) ) {
					if( BRNTKY_select( bmn, kmkcd_str, NULL, nChar == VK_TAB ? TRUE : FALSE ) )
					{
						::ZeroMemory( saveITEM50kana, sizeof saveITEM50kana );
						if( nChar == VK_RETURN )	nChar = VK_TAB;

						if( dspstr ) {
							struct	_DBKNREC* pKn;
							if( (pKn = pDBzm->DB_PjisToKnrec( kmkcd_str ) ) != NULL ) {
								KamokuString( pKn, check.kd_name, sizeof check.kd_name );
								exDBSetDispString( CRED_PN, getINP_CTL(pwnd, CRED_PN)->IDC_X, check.kd_name );
							}
						}
					}
					else
					{
						//枝番摘要なし
						if( (m_TabKeyStop & 0x02) && nChar == VK_TAB ) {
							m_TabKeyStop &= ~0x02;
						}
						else {
							nChar = VK_RETURN;
						}
					}
				}
				else	nChar = VK_RETURN;
			}

#ifdef DB_KEEP
			if( check.kd_code[0] == '\0'/*0xffff*/ )
			{
				::ZeroMemory( &check, sizeof check );
				ldata = getLINE_DATA();

				memcpy( check.kd_code, ldata->_XXDTA.dd_cre, KMKCD_LN );
				check.kd_eda = ldata->_XXDTA.dd_cbr;
				memcpy( check.kd_name, ldata->_XXDTA.dd_cre_name, sizeof check.kd_name );

				exDBkamokudata_set( CRED_PN, getINP_CTL( pwnd, CRED_PN )->IDC_X, &check );
				// 資金繰諸口枝番等セット
				set_etcdsp( ln, CRED_PN );
				TerminationDataSet( CRED_PN );
				return;
			}
			else if( !check.kd_code[0] && !BOTOM_SEQ )
#endif

			if( nChar == VK_TAB && ! dinp_modefy_ok( getLINE_DATA() ) ) {
				nChar = VK_RETURN;
			}

			if( ! check.kd_code[0] && !BOTOM_SEQ )
			{
				Buzzer();
				return;
			}

			// 現在の業種でない科目の 空うちEnter での 入力をチェックする。
			int type =  IsKojinGyosyuMaster( pDBzm );
			if( type ) {
				if( IsKojinKengyoMaster( type ) ) {
					if( ! check.kd_code[0] ) {
						struct _KamokuData  akd = {0}, ckd = {0};

						strcpy_s( ckd.kd_code, sizeof ckd.kd_code,  SREC.m_cre );
						ckd.kd_sgn = 2;	// 科目コードチェック用に 枝番のみのサインセット
						if( ! m_Seldata.InputItemCheck( akd, ckd, 0 ) ) {
							Buzzer();
							return;
						}
					}
				}
			}

			if( getLINE_DATA()->_XXDTA.dd_cbr == -1 ) {
				if( pAUTOSEL->BRNKAK_OPT == OPT_BRNKAK ) {
					// 枝番残高があるか？
					if( pDBzm->DB_EdabanZanSearch( CString(check.kd_code), -1 ) == 0 )
						return;
				}
			}

			getLINE_DATA()->LINE_CND[CRED_PN].NEXT_MOVE_KEY = TRUE;

			if( !is_AutoInp_use() && nChar != VK_TAB ) {
				set_termfocus( get_nowln(), CRED_PN, 1 );
			}
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 ) {
			set_termfocus( get_nowln(), CRED_PN, -1 );
		}
		else {
			if( nChar != 0x16 )
				Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}

/***************************
	貸方カナチェック
****************************/
void CDBDinpView::KanaTermIcsinputCred(LPCTSTR data) 
{
	char k_data[10] = {0};
	char kana[10] = {0};

	if( KanaSearchType() )
		return;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	try
	{
		int st;
	//	strcpy_s( kana, sizeof kana, (char *)data );
		kana_copy( k_data, data );
		kana_copy( kana, data );

		SelPar par;
		if( BRNTKY_sel ) {
			par.sel_pn = SL_BRNTKY_PN;
			par.sel_flg = 1;

			BRNTKY_PAR brp;
			brp = m_Seldata.GetBrnTkyPar();

			par.tkybrn	= brp.bt_code;
			par.brn_bmn = brp.bt_bmn;

			// 絞込み
			sprintf_s( par.brn_kana, sizeof par.brn_kana, "%s%s", saveITEM50kana, data );
		}
		else if( m_SelDispPN == SL_KMKANA_PN ) {
			par.sel_pn = SL_KMKANA_PN;
#ifdef CLOSE // 07.17 /07
			sprintf_s( kana, sizeof kana, "%s%s", saveITEM50kana, k_data );
#endif
			memcpy( par.kmk_kana, kana, sizeof par.kmk_kana );
		}
		else {
			par.sel_pn = SL_KMKANA_PN;
			memcpy( par.kmk_kana, kana, sizeof par.kmk_kana );
		}

		st = m_Seldata.SelectJob( &par, -1 );

		if( m_SelDispPN == SL_KMKANA_PN ) {
			if( ! st )
				strcpy_s( saveITEM50kana, sizeof saveITEM50kana, kana);	//save
		}
		else {
			::ZeroMemory( saveITEM50kana, sizeof saveITEM50kana );
			if( ! BRNTKY_sel )	Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


// 金額フォーカス処理
#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsVal( CWnd* pwnd, int ln, short click )
{
	_FocusIcsVal( pwnd, ln, click );

	OnICSInputFocus( VAL_PN );
}
void CDBDinpView::_FocusIcsVal( CWnd* pwnd, int ln, short click )
{
#else
void CDBDinpView::FocusIcsVal( CWnd* pwnd, int ln, short click )
{
#endif
	if( PROGRAM_OK == FALSE )
		return;

	try
	{
		if( BRNCHK_PN != -1 )
			return;

		//別画面入力消去
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}

		if( _dline_chg( pwnd, ln, VAL_PN, click ? 0 : 1 ) == -1 )
			return;

		if( CheckAppendSansyo() )
			return;

		if( click ) {
			//当項目まで自動入力
			if( INP_mode == _APPEND && !is_AutoInp_use() )
				AutoInp( VAL_PN );
		}

		// 取消データの場合はＳＥＱに
		if( isdeldata() )
		{
			SAVE_PN = VAL_PN;
			set_focus( SEQ_PN );
			return;
		}
		else { 
			SAVE_PN = -1;
			if( data_denpdialog(ln) != 0 )
				return;
		}

		//直前のポジションが当ポジションより前で、貸方科目が入力されていない場合は直前のポジションに返す。
		int set_pn;
		if( inp_ok( get_nowpn(), VAL_PN, &set_pn ) == FALSE )
		{
			set_nowpn( set_pn );
			set_focus( /*get_nowpn()*/set_pn );
			return;
		}

		set_nowpn( VAL_PN );

		SelWndShow( FALSE );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

// 金額 ターミネーション
void CDBDinpView::TerminationIcsinputVal( CWnd* pwnd, int ln, long nChar, long inplen, long kst  ) 
{
	VARIANT var;
	char	ip_val[6];
//	struct _DATA_LINE *ldata;

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
//		if( !nChar )
//		{
			l_defn( _BIN );

			exDBdata_get( VAL_PN, getINP_CTL( pwnd, VAL_PN )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			if( var.pbVal != NULL )		memcpy( ip_val, var.pbVal, 6 );
			else						ZeroMemory( ip_val, sizeof ip_val );
		
			if( !dinp_modefy_ok( getLINE_DATA() ) )
			{
				if( l_jsgn( getLINE_DATA()->_XXDTA.dd_val, ip_val, _BIN & 0x0f ) )
				{
					Buzzer();
				}
				memcpy( ip_val, getLINE_DATA()->_XXDTA.dd_val, 6 );
				var.pbVal = (BYTE*)ip_val;
				exDBdata_set( VAL_PN, getINP_CTL( pwnd, VAL_PN )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
				TerminationDataSet( VAL_PN );
			}
			else
			{
				if( getLINE_DATA()->LINE_CND[VAL_PN].INP_sg == FALSE && !l_test( ip_val ) && ! inplen )
				{
					if( getLINE_DATA()->LINE_CND[VAL_PN].NEXT_MOVE_KEY == FALSE )
//						return;
						goto NEXT;

					memmove( ip_val, SREC.m_val, 6 );
				}
				//諸口・現金貸借
				if( INP_mode != _APPEND )
				{
					syog_genk( &CREC, _SUB );
					//貸借合計を再計算する
					m_TaiTtl.init_taiclq();
				}
				//金額入力時は、税額F10 サインを落とす
				if( l_jsgn(CREC.m_val, ip_val, _BIN & 0x0f) )
				{
					getLINE_DATA()->LINE_CND[ZEI_PN].KEY_inp = 0;
				}

				memmove( SREC.m_val, ip_val, sizeof(SREC.m_val) );
				memmove( CREC.m_val, SREC.m_val, sizeof(SREC.m_val) );

				memmove( getLINE_DATA()->_XXDTA.dd_val, ip_val, sizeof(ip_val) );

				getLINE_DATA()->LINE_CND[VAL_PN].INP_sg = TRUE;

				//消費税
				struct _SY_MSG_PACK2 smp;
				getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt( &CREC, &smp, INP_mode);
#ifndef KEEP
				char zei[6] = {0};
				l_input( zei, (char*)(LPCTSTR)smp.SY_MSG_SYZEI );	// 税額をセット
				memcpy( getLINE_DATA()->_XXDTA.dd_zei, zei, 6 );
				memmove( SREC.m_zei, zei, sizeof(SREC.m_zei) );
				memmove( CREC.m_zei, SREC.m_zei, sizeof(SREC.m_zei) );

				if (is_syohizeisiwake( &CREC )) {
					getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = TRUE;
				}
				else {
					getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = FALSE;
				}
#endif
				//諸口・現金貸借
				if( INP_mode != _APPEND )
				{
					syog_genk( &CREC, _ADD );
				}

				::ZeroMemory( ip_val, sizeof ip_val );
				memcpy( ip_val, getLINE_DATA()->_XXDTA.dd_val, 6 );
				var.pbVal = (BYTE*)ip_val;
				exDBdata_set( VAL_PN, getINP_CTL( pwnd, VAL_PN )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );

				int pntbl[] = { VAL_PN, ZEI_PN, SKBN_PN };
				//消費税再表示
				TerminationDataSet( pntbl, 3 );
			}
//			return;
//		}
NEXT:
		getLINE_DATA()->LINE_CND[VAL_PN].NEXT_MOVE_KEY = FALSE;
			
		if( !nChar )
			return;

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return;

		if( nChar )
		{
			if( nChar == VK_UP || nChar == VK_DOWN ) {
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			nChar = dline_chg( pwnd, nChar );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合はＳＥＱに
				if( isdeldata() )
				{
					SAVE_PN = VAL_PN;
					set_focus( SEQ_PN );
					return;
				}
				else
					SAVE_PN = -1;

				set_focus( VAL_PN );
				return;
			}
		}
		// 消費税区分セット位置
		m_SET_SYZINP_PN = -1;

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
		{
			// 09.08 /08
			if( !is_AutoInp_use() ) {
				if( ValEnterCopyMode() && INP_mode == _APPEND ) {
					if( nChar == VK_RETURN ) {
						if( getLINE_DATA()->LINE_CND[VAL_PN].INP_sg == FALSE && 
							!l_test( ip_val ) && ! inplen ) {
							return;
						}
					}
				}
			}

			getLINE_DATA()->LINE_CND[VAL_PN].NEXT_MOVE_KEY = TRUE;
			
			if( !is_AutoInp_use() )
			{
				int vect;
				vect = (nChar == VK_RETURN ) ? 1 : 2;

				//インボイス外税免税は、税額常に修正
				if( is_sotomensiwake(&CREC) && pAUTOSEL->DSPZEI_OPT == OPT_NONOPT ) {
					m_pnZeiSotoMen = VAL_PN;
					set_focus(ZEI_PN);
				}
				else {
					//消費税項目カーソル位置チェック
					if( pAUTOSEL->DSPSYZ_OPT == OPT_NODSP ) {
						m_SET_SYZINP_PN = -1;
					}
					else {
						m_SET_SYZINP_PN = m_pSyzInp->CheckSyzPn(vect);
						if( m_SET_SYZINP_PN == -1 ) {
							if( IsMustStopSyz(&CREC) ) {
								m_SET_SYZINP_PN = SY_SZKBN_PN;
							}
						}
					}
					if( m_SET_SYZINP_PN >= 0 )
						set_termfocus(get_nowln(), VAL_PN, 1);
					//変動事由・振替科目[公益]チェック
					else if( IsHjiyuOrNPfuri(CREC.m_dbt, CREC.m_cre) ) {
						int pn = SKBN_PN;
						if( pAUTOSEL->DSPSYZ_OPT == OPT_NODSP ) {
							if( pAUTOSEL->DSPZEI_OPT == OPT_NODSP ) {
								pn = VAL_PN;
							}
							else {
								pn = ZEI_PN;
							}
						}
						set_termfocus(get_nowln(), pn, 1);
					}
					else {
						//	set_termfocus( get_nowln(), SKBN_PN, 1 );
						set_focus(TKY_PN);
					}
				}
			}
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 ) {
			set_termfocus( get_nowln(), VAL_PN, -1 );
		}
		else {
			if( nChar != 0x16 )
				Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
// 税額

// vフォーカス処理
#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsZei( CWnd* pwnd, int ln, short click )
{
	_FocusIcsZei( pwnd, ln, click );

	OnICSInputFocus( ZEI_PN );
}
void CDBDinpView::_FocusIcsZei( CWnd* pwnd, int ln, short click )
{
#else
void CDBDinpView::FocusIcsZei( CWnd* pwnd, int ln, short click )
{
#endif
	if( PROGRAM_OK == FALSE )
		return;

	try
	{
		if( BRNCHK_PN != -1 )
			return;

		//別画面入力消去
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}

		if( _dline_chg( pwnd, ln, ZEI_PN, click ? 0 : 1  ) == -1 )
			return;

		if( CheckAppendSansyo() )
			return;

		if( click ) {
			//当項目まで自動入力
			if( INP_mode == _APPEND && !is_AutoInp_use() )
				AutoInp( ZEI_PN );
		}

		// 取消データの場合はＳＥＱに
		if( isdeldata() )
		{
			SAVE_PN = ZEI_PN;
			set_focus( SEQ_PN );
			return;
		}
		else {
			SAVE_PN = -1;
			if( data_denpdialog(ln) != 0 )
				return;
		}

		//直前のポジションが当ポジションより前で、貸方科目が入力されていない場合は直前のポジションに返す。
		int set_pn;
		if( inp_ok( get_nowpn(), ZEI_PN, &set_pn ) == FALSE )
		{
			set_nowpn( set_pn );
			set_focus( /*get_nowpn()*/set_pn );
			return;
		}

		set_nowpn( ZEI_PN );

		SelWndShow( FALSE );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

// 税額 ターミネーション
void CDBDinpView::TerminationIcsinputZei( CWnd* pwnd, int ln, long nChar, long inplen, long kst  ) 
{
	VARIANT var;
	char	ip_val[6];
	char	buf[64];
//	struct _DATA_LINE *ldata;

	if( PROGRAM_OK == FALSE )
		return;

	// 税額修正不可 -- 05.21 /10
	BOOL bModify = TRUE;

	if( !is_sotomensiwake(&CREC) ) {
		if( !is_syohizeisiwake(&CREC) || pAUTOSEL->ZEIMDFY_OPT != OPT_ZEIMDFY || is_yusyutsusiwake(&CREC) ) {
			bModify = FALSE;
		}
	}

	try
	{
//		if( !nChar )
//		{
			l_defn( _BIN );

			exDBdata_get( ZEI_PN, getINP_CTL( pwnd, ZEI_PN )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
			if( var.pbVal != NULL )	memcpy( ip_val, var.pbVal, 6 );
			else					memset( ip_val, '\0', sizeof ip_val );
		
			if( !dinp_modefy_ok( getLINE_DATA() ) || ! bModify )
			{
				if( l_jsgn( getLINE_DATA()->_XXDTA.dd_zei, ip_val, _BIN & 0x0f ) )
				{
					Buzzer();
				}
				memcpy( ip_val, getLINE_DATA()->_XXDTA.dd_zei, 6 );
				var.pbVal = (BYTE*)ip_val;
				exDBdata_set( ZEI_PN, getINP_CTL( pwnd, ZEI_PN )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
				TerminationDataSet( ZEI_PN );
			}
			else
			{
				if( getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg == FALSE && !l_test( ip_val ) && ! inplen  )
				{
					if( getLINE_DATA()->LINE_CND[VAL_PN].NEXT_MOVE_KEY == FALSE )
//						return;
						goto NEXT;

//					memmove( ip_val, SREC.m_zei, 6 );
				}
				//諸口・現金貸借
				if( INP_mode != _APPEND )
				{
					syog_genk( &CREC, _SUB );
				}

				memmove( getLINE_DATA()->_XXDTA.dd_zei, ip_val, sizeof(ip_val) );
				getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = TRUE;

				//消費税
			//	getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt( &CREC, INP_mode);
				//消費税再表示
			//	TerminationDataSet( SKBN_PN );
			//	::ZeroMemory( ip_val, sizeof ip_val );
				
				l_print( buf, ip_val, FMT15 );

				pDBzm->SetCDBData( &CREC );
				if( pDBsy->SyZei( buf ) == 0 ) {
					struct _SY_MSG_PACK2 smpk;
					pDBsy->SyMsg( &smpk );
					CDBINPDataRec rec;
					pDBzm->GetCDBData( &rec );

					memmove( SREC.m_zei, ip_val, sizeof(SREC.m_zei) );
					memmove( CREC.m_zei, SREC.m_zei, sizeof(SREC.m_zei) );

					memcpy( getLINE_DATA()->_XXDTA.dd_zei, rec.m_zei, 6 );
					memcpy( ip_val, rec.m_zei, 6 );
					memcpy( getLINE_DATA()->_XXDTA.dd_val, rec.m_val, 6 );

					var.pbVal = (BYTE*)ip_val;
					exDBdata_set( ZEI_PN, getINP_CTL( pwnd, ZEI_PN )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
					int pntbl[] = {ZEI_PN, VAL_PN};
					TerminationDataSet( pntbl, 2 );
				}
				else {
					memmove( ip_val, CREC.m_zei, sizeof(CREC.m_zei) );
					var.pbVal = (BYTE*)ip_val;
					exDBdata_set( ZEI_PN, getINP_CTL( pwnd, ZEI_PN )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
				}

				//諸口・現金貸借
				if( INP_mode != _APPEND )
				{
					syog_genk( &CREC, _ADD );
				}
			}
//			return;
//		}
NEXT:
		getLINE_DATA()->LINE_CND[ZEI_PN].NEXT_MOVE_KEY = FALSE;
			
		if( !nChar )
			return;

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return;

		if( nChar )
		{
			if( nChar == VK_UP || nChar == VK_DOWN ) {
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			nChar = dline_chg( pwnd, nChar );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合はＳＥＱに
				if( isdeldata() )
				{
					SAVE_PN = ZEI_PN;
					set_focus( SEQ_PN );
					return;
				}
				else
					SAVE_PN = -1;

				set_focus( ZEI_PN );
				return;
			}
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
		{
			getLINE_DATA()->LINE_CND[ZEI_PN].NEXT_MOVE_KEY = TRUE;
			
			if( !is_AutoInp_use() )
			{
				if( m_bSyzYunyu ) {
					set_focus( TKY_PN );
				}
				else if( m_bSyzSotoMen  ) {
					int vect = (nChar == VK_RETURN) ? 1 : 2;

					//消費税項目カーソル位置チェック
					if( pAUTOSEL->DSPSYZ_OPT == OPT_NODSP ) {
						m_SET_SYZINP_PN = -1;
					}
					else {
						m_SET_SYZINP_PN = m_pSyzInp->CheckSyzPn(vect);
						if( m_SET_SYZINP_PN == -1 ) {
							if( IsMustStopSyz(&CREC) ) {
								m_SET_SYZINP_PN = SY_SZKBN_PN;
							}
						}
					}
					if( m_pnZeiSotoMen == SKBN_PN ) {
						m_SET_SYZINP_PN = -1;
					}

					if( m_SET_SYZINP_PN >= 0 )
						set_termfocus(get_nowln(), VAL_PN, 1);
					else {
						set_focus(TKY_PN);
					}
				}
				else {
					set_termfocus( get_nowln(), ZEI_PN, 1 );
					if( is_sotomensiwake(&CREC) ) {
						m_bSyzSotoMen = TRUE;
						m_pnZeiSotoMen = ZEI_PN;
					}
				}
			}
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 ) {
			set_termfocus( get_nowln(), ZEI_PN, -1 );
		}
		else {
			if( nChar != 0x16 )
				Buzzer();
		}
		// 輸入仕入れサインを クリア
		if( nChar != 0x16 ) {
			m_bSyzYunyu = FALSE;
		}

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}



///////////////////////////////////////////////////////////////////////////////
// 消費税

// GoTo 消費税選択画面
#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsSyz( CWnd* pwnd, int ln, short click, int focs_sw ) 
{
	_FocusIcsSyz( pwnd, ln, click, focs_sw );
	OnICSAnkFocus(  );
	OnICSInputFocus( SKBN_PN );
}
void CDBDinpView::_FocusIcsSyz( CWnd* pwnd, int ln, short click, int focs_sw ) 
{
#else
void CDBDinpView::FocusIcsSyz( CWnd* pwnd, int ln, short click, int focs_sw ) 
{
#endif

	int sv_ln = get_nowln();

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
		if( BRNCHK_PN != -1 )
			return;


TRACE( "FocusIcsSyz: m_ToSkbnDlg %d\n", m_ToSkbnDlg );
		Destroy_BkInp();
		Destroy_HusenInp();
		Destroy_HjiyuInp();
		Destroy_SubDataInp();

		if( !m_ToSkbnDlg )
		{
			if( click ) {
				long nChar = 0;
				click_seljob( ln, nChar );
			}

			if( _dline_chg( this, ln, SKBN_PN, focs_sw ) == -1 )
			{
				if( sv_ln != APPEND_LINE && get_nowln() == APPEND_LINE )
				{
					//消費税画面消去＆現仕訳消費税区分表示
					Destroy_SkbnIn();

					if( get_nowpn() == SKBN_PN )
						;
					else
						return;
				}
				else
					return;
			}

			if( CheckAppendSansyo() )
				return;

			// 取消データの場合はＳＥＱに
			if( isdeldata() )
			{
				SAVE_PN = SKBN_PN;
				set_focus( SEQ_PN );
				return;
			}
			else {
				SAVE_PN = -1;
				if( data_denpdialog(ln) != 0 )
					return;
			}

			//直前のポジションが当ポジションより前で、金額が入力されていない場合は直前のポジションに返す。
			int set_pn;
			if( inp_ok( get_nowpn(), SKBN_PN, &set_pn ) == FALSE )
			{
				set_nowpn( set_pn );
				set_focus( /*get_nowpn()*/set_pn );
				return;
			}

//			if( BRNTKY_sel && get_nowpn() != SKBN_PN  )
//			{
//				BRNTKY_sel_set( 0 );
//				//科目・摘要選択画面リセット
//				SelDspRst( INP_mode );
//				fixvalcnd( !isfixvalcnd( SKBN_PN ) );	// 元背景色強制描画のため
//			}

			if( click ) {
				//当項目まで自動入力
				if( INP_mode == _APPEND && !is_AutoInp_use() )
					AutoInp( SKBN_PN );
			}
			getLINE_DATA()->LINE_CND[SKBN_PN].INP_sg = TRUE;

			//消費税画面作成＆入力
			m_ToSkbnDlg = TRUE;

			set_nowpn( SKBN_PN );

			//消費税
			getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt( &CREC, NULL, INP_mode);

			//消費税再表示
			TerminationDataSet( SKBN_PN );

#ifdef LATER_CLOSE
			SkbnSelectColor( get_nowln() );
#endif

			if( SkbnInDisp( getINP_CTL( SKBN_PN )->IDC_X ) == 0 )
			{
				if( INP_mode == _APPEND )
					m_selDate.ShowWindow( SW_HIDE );
			}
			else
			{
				PROGRAM_OK = FALSE;
				Destroy_SkbnIn( TRUE );
				PROGRAM_OK = TRUE;

				// 空の消費税区分欄表示
				PostMessage( WM_SKBN_ICHIRN, (WPARAM)0, (LPARAM)0 );

				if( INP_mode == _APPEND )
					m_selDate.ShowWindow( SW_SHOW );

				SelWndShow( FALSE );
			}
			m_ToSkbnDlg = FALSE;
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

// 消費税 ターミネーション
int CDBDinpView::TerminationIcsinputSyz( CWnd* pwnd, int ln, long nChar, long inplen, long kst  )
{
	int sv_ln = get_nowln();
//MSG msg = {0};

	if( PROGRAM_OK == FALSE )
		return -1;

	try
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return 0;

		if( nChar )
		{
			if( nChar == VK_UP || nChar == VK_DOWN ) {
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			nChar = dline_chg( this, nChar );
			if( nChar == -1 )
			{
				return -1;
			}
			else if( !nChar )
			{
				if( sv_ln == get_nowln() )
					FocusIcsSyz( this, get_nowln(), 0, 0 );
				else
				{
# ifdef _NEWFOCUS_	// 01.27 /01
					__set_focus( get_nowln(), SKBN_PN );
# else
					set_focus( SKBN_PN );
# endif
				}

				return -1;
			}
		}

		if( !nChar )
		{
			return 0;
		}

		switch( nChar )
		{
		case VK_END:
			break;
		case VK_TAB:
		case VK_RETURN:
		case VK_RIGHT:
//		case VK_F3:
			if( IsYunyuMove() ) {
				set_focus( ZEI_PN );
			}
			else if( is_sotomensiwake(&CREC) && pAUTOSEL->DSPZEI_OPT == OPT_NONOPT && !m_bSyzSotoMen ) {
				m_bSyzSotoMen = TRUE;
				m_pnZeiSotoMen = SKBN_PN;
				set_focus(ZEI_PN);
			}
			//変動事由・振替科目[公益]チェック [10.22 /08]
			else if( IsHjiyuOrNPfuri( CREC.m_dbt, CREC.m_cre ) ) {
				set_termfocus( get_nowln(), SKBN_PN, 1 );
			}
			else {
				set_focus( TKY_PN );
			}
			break;
		case VK_LEFT:
		case VK_F2:
			set_focus( VAL_PN );
			break;
		default:
			Buzzer();
			return -1;
			break;

		}
		return 0;
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}
}

// 輸入仕入で、税額欄に移動するか？
BOOL CDBDinpView::IsYunyuMove()
{
	BOOL ret = FALSE;

	if( INP_mode == _APPEND ) {
		if( pAUTOSEL->ZEIMDFY_OPT == OPT_ZEIMDFY ) {	// 税額修正可能のみ
			if( m_bSyzYunyu )	ret = TRUE;
		}
	}
	else {
		// 税額欄を表示している場合で、税額修正可能のみ
		if( pAUTOSEL->DSPZEI_OPT == OPT_NONOPT && pAUTOSEL->ZEIMDFY_OPT == OPT_ZEIMDFY ) {
			if( m_bSyzYunyu )	ret = TRUE;
		}
	}
	return ret;
}

//　消費税区分選択番号
LRESULT CDBDinpView::SetSkbnLineDisp( WPARAM wParam, LPARAM lParam ) 
{
TRACE( "selno = %d\n", wParam );

	struct _SY_MSG_PACK2 smpk;
	pDBsy->SyMsg( &smpk );
//	getLINE_DATA()->_XXDTA.dd_syz_str = smpk.SY_LIN_MSG;

//MyTrace("L:(%d)  smpk.SY_MSG_URISIRE->sel_no = %d\n", __LINE__, smpk.SY_MSG_URISIRE->sel_no );

	//特定収入の仕訳で、特定収入に変更したか？
	DWORD dwKobe = 0;

	if (INP_mode == _APPEND && pDBzm->IsKobetsuBmnSyz()) {
		CDBINPDataRec	tmprec;
		pDBzm->GetCDBData(&tmprec);
		if (pDBzm->IsTokuteiSyunyuData(&tmprec)) {
			//特定収入区分が異なる場合
			if( (tmprec.m_dsign[5]&0x0f) != (CREC.m_dsign[5] & 0x0f) ) {
				int bmn = tmprec.m_dbmn;
				if (bmn == -1)	tmprec.m_cbmn;
				dwKobe = pDBzm->KobetsuSiwakeCheck(&tmprec, bmn);
			}
		}
	}
	//諸口・現金貸借
	if( INP_mode != _APPEND ) {
		syog_genk( &CREC, _SUB );
	}

	if (dwKobe) {
		int st = pDBsy->SyFnc(dwKobe);
	}
	pDBzm->GetCDBData( &CREC );
//	MyTrace("L:(%d)  CREC.m_invno = %s\n", __LINE__, CREC.m_invno);
	DSIGN_TRACE(CREC.m_dsign);

	getLINE_DATA()->_XXDTA.dd_syz_str = sy_line_txt( &CREC );

//	MyTrace("L:(%d) getLINE_DATA()->_XXDTA.dd_syz_str = %s\n", __LINE__, getLINE_DATA()->_XXDTA.dd_syz_str);

	memmove( getLINE_DATA()->_XXDTA.dd_zei, CREC.m_zei, 6 );

	if( is_syohizeisiwake( &CREC ) ) {
		getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = TRUE;
	}
	else {
		getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = FALSE;
	}

	int pntbl[] = {
		SKBN_PN, ZEI_PN
	};
	
	TerminationDataSet( pntbl, 2 );
	// 消費税が、輸入仕入 に変更になったか？
	if( m_pSyzInp->GetSyzswkKbn() != CD_YUNYU ) {
		if( (CREC.m_dsign[0]&0x40) && (CREC.m_dsign[2]&0x0f) == 0x01 && 
			CREC.m_dsign[4] == 4 ) {
			m_bSyzYunyu = (pAUTOSEL->ZEIMDFY_OPT == OPT_ZEIMDFY) ? TRUE : FALSE;
		}
		else {
			m_bSyzYunyu = FALSE;
		}
	}

	if( INP_mode != _APPEND ) {
		syog_genk( &CREC, _ADD );
	}
	else {	// 12.10 /12
		if( TKattr ) {	//1:課税, 2:非課税, 3:不課税, 4:経過(3%)
			//摘要消費税属性から 変更されているか？
			if( (CREC.m_dsign[0]&0x40) ) {

				if( TKattr == 1 ) {
					if( (CREC.m_dsign[2]&0x0f) != 1 ) {
						TKattr = 0;
					}
				}
				else if( TKattr == 2 ) {
					if( (CREC.m_dsign[2]&0x0f) != 0 ) {
						TKattr = 0;
					}
				}
				else if( TKattr == 3 ) {
					if( (CREC.m_dsign[2]&0x0f) != 2 ) {
						TKattr = 0;
					}
				}
				else {
					if( !( (CREC.m_dsign[2]&0x0f) == 1 && (CREC.m_dsign[1]&0x0f) == 1) ) {
						TKattr = 0;
					}
				}
			}
		}
	}

	return 1;
}

//キーで終わった場合 CSkbnIn からのメッセージ
LRESULT CDBDinpView::OnSkbnEnd( WPARAM wParam, LPARAM lParam ) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
//	if( m_pSyzInp )
//	{
//		if( m_pSyzInp->m_LastKey )
//		{

		PROGRAM_OK = FALSE;

#ifdef DB_CLOSE
#ifdef _NEWFOCUS_	// 01.27 /01
		__set_focus( get_nowln(), SKBN_PN );
#else
		set_focus( SKBN_PN );
#endif
#endif
		PROGRAM_OK = TRUE;

//		PROGRAM_OK = FALSE;
//		Destroy_SkbnIn();
//		PROGRAM_OK = TRUE;

		int kst = 0;

		if( is_SHIFT() )	kst = ICSDBEDT_KST_SHIFT;

		if( TerminationIcsinputSyz( this, get_nowln(), wParam, 0, kst ) != -1 )
		{
			TerminationIcsinputSyz( this, get_nowln(), 0, 0, 0 );
		}
		else if( m_pSyzInp )
		{
			m_pSyzInp->set_nowfocus();
		}

//		}
//	}
	return 1;
}

// 消費税区分一覧表示
LRESULT CDBDinpView::DspSkbnIchiran( WPARAM wParam, LPARAM lParam )
{
	BOOL bMake = FALSE;

	if( m_pSyzInp && m_pSyzInp->IsDisplay() ) {
		if( m_pSelWnd == NULL ) { //他プログラムからの戻り時は、選択ウィンドウができていない。
			if( pAUTOSEL->BLKSEL_OPT ) {
				MakeSelWnd( TRUE );
				bMake = TRUE;

				_disp_change();
			//	Destroy_SkbnIn();
			//	set_focus( get_nowpn() );
			//	return 1;
			}
		}
	}

	DispSyzIchiran( wParam, lParam );

	if( m_pSyzInp && m_pSyzInp->IsDisplay() ) {
		if( pAUTOSEL->BLKSEL_OPT ) {
			SelWndMoveJob( TRUE );
		}
	}

/*
	DWORD i;
	CString *str = (CString*)lParam;

#define SYZSEL_MAX	8*4

static struct _SelTable SelVal32[SYZSEL_MAX];
static SelSetProcPar	spar;

	spar.sp_prevpn = m_SelDispPN;
	spar.sp_nowpn = BVW_SYZ_PN;
	strcpy_s( spar.seljob, sizeof spar.seljob, "《消費税》" );

	for( i = 0 ; i < SYZSEL_MAX ; ++i )
	{
		// 表示
		SelVal32[i].Data = i < wParam ? *(str+i) : "";
	//	SelVal32[i].number = i + (isEntrySys() ? 9 : 1);
		SelVal32[i].number = i + 1;
	}

	SetSelectSel32( SelVal32, &spar, SYZSEL_MAX, this );
*/
	return TRUE;
}


//消費税選択画面消去＆修正消費税区分表示
void CDBDinpView::Destroy_SkbnIn(int back_color )
{
	if( m_pSyzInp == NULL )
		return;

	if( m_pSyzInp->IsDisplay() )
	{
		// 消費税入力ダイアログ非表示
		m_pSyzInp->DialogOFF();

		// インボイス対応
		// 登録番号取得
		//if( bInvMaster && bInvUse && bInvDisp ) {
		//	sprintf_s( CREC.m_invno, sizeof( CREC.m_invno ), _T( "%s" ), pDBzm->dbdata->invno );
		//}
		//消費税

		DSIGN_TRACE(CREC.m_dsign);

		getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt( &CREC, NULL, INP_mode);
		//消費税再表示
		TerminationDataSet( SKBN_PN );

		DSIGN_TRACE(CREC.m_dsign);
	}
}

// 現状では、実質この関数は呼ばれない(コメントアウトされている)
//
void CDBDinpView::Destroy_SkbnInX()
{
#ifdef DB_CLOSE
	if( m_pSyzInp )
		Destroy_SkbnIn();	// 非表示
	if( m_pSkbnInX )
	{
		//消費税入力ダイアログ削除
		m_pSkbnInX->SkbnDestroy();
	}
#endif
}

// モードレスダイアログの非表示
void CDBDinpView::Destroy_ModeLessDlg()
{
	Destroy_SkbnIn();
	Destroy_BkInp();
	Destroy_HusenInp();
	Destroy_HjiyuInp();
	Destroy_SubDataInp();

	m_selDate.ShowWindow(SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////////////
// 変動事由
// 部門フォーカス処理
#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsHjiyu( CWnd* pwnd, int ln, short click )
{
	_FocusIcsHjiyu( pwnd, ln, click );

	OnICSInputFocus( HJIYU_PN );
}
void CDBDinpView::_FocusIcsHjiyu( CWnd* pwnd, int ln, short click )
{
#else
void CDBDinpView::FocusIcsHjiyu( CWnd* pwnd, int ln, short click )
{
#endif
	if( PROGRAM_OK == FALSE )
		return;

TRACE( "**FocusIcsHjiyu %d\n", ln );	

	try
	{
		//別画面入力消去
		Destroy_SkbnIn();
		Destroy_BkInp();
		Destroy_HusenInp();
		Destroy_SubDataInp();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}

		if( _dline_chg( pwnd, ln, HJIYU_PN, click ? 0 : 1  ) == -1 )
			return;

		if( CheckAppendSansyo() )
			return;

		// 取消データの場合はＳＥＱに
		if( isdeldata() )
		{
			SAVE_PN = HJIYU_PN;
			set_focus( SEQ_PN );
			return;
		}
		else {
			SAVE_PN = -1;
			if( data_denpdialog(ln) != 0 )
				return;
		}

		//入力項目チェック
		int set_pn;
		if( inp_ok( get_nowpn(), HJIYU_PN, &set_pn ) == FALSE )
		{
			set_nowpn( set_pn );
			set_focus( /*get_nowpn()*/set_pn );
			return;
		}

		if( click ) {
			//当項目まで自動入力
			if( INP_mode == _APPEND && !is_AutoInp_use() )
				AutoInp( HJIYU_PN );
		}

		getLINE_DATA()->LINE_CND[HJIYU_PN].INP_sg = TRUE;

		SelWndShow( FALSE );

		set_nowpn( HJIYU_PN );

		HJIYU_DATA hd = {0};
		CString str;

		if( pDBzm->zvol->apno < 0x50 ) {
			// 変動事由
			if( get_khkbntype( CREC.m_dbt, CREC.m_cre, pDBzm ) >= 0 ) {
				hd.hjy_code = CREC.m_hjiyu;
				pDBzm->get_hjiyustr( hd.hjy_code, str );
				strcpy_s( hd.hjy_name, sizeof hd.hjy_name, str );
				m_pHjiyuInp->SetHjiyuData( &hd, FALSE );
				m_pHjiyuInp->set_mode(0);
				HjiyuInpDisp();
				if( INP_mode == _APPEND )
					m_selDate.ShowWindow( SW_HIDE );
			}
			else {
				Destroy_HjiyuInp();
			}
		}
		else {
			if( IsSyafuKaisei( pDBzm->zvol ) ) {
				//内部取引消去
				hd.hjy_code = CREC.m_hjiyu;
#ifndef NAIBU_VER2
				hd.hjy_nbcd = -1;
#else
				hd.hjy_nbcd = CREC.m_nbcd;
#endif

				m_Seldata.GetNaibuString( hd.hjy_code, hd.hjy_nbcd, str );
				strcpy_s( hd.hjy_name, sizeof hd.hjy_name, str );
				m_pHjiyuInp->SetHjiyuData( &hd, FALSE );

				m_pHjiyuInp->set_mode(2);
				HjiyuInpDisp();
				if( INP_mode == _APPEND )
					m_selDate.ShowWindow( SW_HIDE );
			}
			else {
				// 振替区分
				if( m_Seldata.IsNPfuridata( CREC.m_dbt, CREC.m_cre ) ) {
					hd.hjy_code = CREC.m_hjiyu;
					m_Seldata.GetNPfuriStr( hd.hjy_code, str );
					strcpy_s( hd.hjy_name, sizeof hd.hjy_name, str );
					m_pHjiyuInp->SetHjiyuData( &hd, FALSE );
					int sign = 1;
					if( IsGakkouH27Master( pDBzm ) )	sign = 4;
					m_pHjiyuInp->set_mode(sign);
					HjiyuInpDisp();
					if( INP_mode == _APPEND )
						m_selDate.ShowWindow( SW_HIDE );
				}
				else {
					Destroy_HjiyuInp();
				}
			}
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}

}


LRESULT CDBDinpView::OnHjiyuInEnd( WPARAM wParam, LPARAM lParam )
{
	long nChar;

	try {
		if( wParam )
		{
			TerminationDataSet( HJIYU_PN );

			wParam = SelNaibuDsp( wParam );

			nChar = dline_chg( this, wParam );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return 1;
			}
			else if( !nChar )
			{
				// 取消データの場合はＳＥＱに
				if( isdeldata() )
				{
					SAVE_PN = HJIYU_PN;
					set_focus( SEQ_PN );
					return 1;
				}
				else
					SAVE_PN = -1;

				int set_pn = pn_movecheck( HJIYU_PN );
				set_focus( set_pn );
				return 1;
			}

			switch( nChar ) {
			case VK_RETURN : case VK_RIGHT : case VK_TAB : 
				set_termfocus( get_nowln(), HJIYU_PN, 1 );
				break;
			case VK_LEFT:
				set_termfocus( get_nowln(), HJIYU_PN, -1 );
				break;
			}
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}

	return 1;
}

//変動事由選択
LRESULT CDBDinpView::OnHjiyuSel( WPARAM wParam, LPARAM lParam )
{
	if( wParam > 0 ) {

		_hjiyu_select( wParam );

	}
	else {
		getLINE_DATA()->_XXDTA.dd_hjiyu = "";
		CREC.m_hjiyu = 0;
		CREC.m_nbcd = -1;
	}

	TerminationDataSet( HJIYU_PN );

	return 1;
}


// 変動事由 セレクト サブ
void CDBDinpView::_hjiyu_select( int selno )
{
	char buf[128];
	SelResult	srt;
	BOOL bHendo, bNaibu;

	if (pDBzm->zvol->apno < 0x50) {
		bHendo = TRUE;
		bNaibu = FALSE;
	}
	else {
		bHendo = FALSE;

		if( IsSyafuKaisei( pDBzm->zvol ) )	bNaibu = TRUE;
		else								bNaibu = FALSE;
	}

	sprintf_s( buf, sizeof buf, "%d", selno );
	
	if( bHendo )	srt.sel_pn = SL_HJIYU_PN;
	else {
		srt.sel_pn = bNaibu ? SL_NAIBU_PN : SL_NPFURI_PN;
	}

	if( m_Seldata.GetSelResult( &srt, buf ) ) {
		// 変動事由 or 振替科目 選択OK
		getLINE_DATA()->_XXDTA.dd_hjiyu = srt.selname;
		HJIYU_DATA hd = {0};
		hd.hjy_code = srt.code;
		strcpy_s( hd.hjy_name, sizeof hd.hjy_name, srt.selname );

		if( bNaibu ) {
#ifndef NAIBU_VER2
			hd.hjy_nbcd = -1;
			CREC.m_nbcd = -1;
#else
			hd.hjy_nbcd = srt.bmn;
			CREC.m_nbcd = srt.bmn;
#endif
		}
		else {
			CREC.m_nbcd = -1;
		}
		m_pHjiyuInp->SetHjiyuData( &hd, TRUE );
		CREC.m_hjiyu = srt.code;

		if( bHendo && getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg == 0 ) {
			if( pAUTOSEL->HJIYU_OPT == OPT_HJIYU ) {
				CString tmp;
				tmp = CREC.m_tekiyo;
				// 変動事由を仕訳摘要へ[ すでに同じ名称がある場合は除く ]
				if( ! IsIncludeString( tmp, srt.selname ) ) {
					tmp += srt.selname;
					int len;
					len = tmp.GetLength();

					if( len > (Voln1->tk_ln * 2) ) {
						LPSTR p = tmp.GetBuffer(256);
						int n = get_strcnt( p, (Voln1->tk_ln * 2) );

						*(p + n) = '\0';
						tmp.ReleaseBuffer();
					}

					CREC.m_tekiyo = tmp;
					getLINE_DATA()->_XXDTA.dd_tek.tekiyo = tmp;
					TerminationDataSet( TKY_PN );
				}
			}
		}
	}
	else {
		Buzzer();
	}

}

// 科目が変更になったときの変動事由・振替科目[公益]チェック
void CDBDinpView::HendJiyuCheck( CDBINPDataRec *pcrec, int clear/*=0*/ )
{
	if( pcrec->m_hjiyu != 0 ) {
		if( ! IsEnableHjiyu( pcrec ) || clear ) {
			// 変動事由コードが正しくないので 0 にする
			 pcrec->m_hjiyu = 0;

			 getLINE_DATA()->_XXDTA.dd_hjiyu = "";
			 TerminationDataSet( HJIYU_PN );
		}
	}
}



///////////////////////////////////////////////////////////////////////////////
// 摘要

#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsTky( CWnd* pwnd, int ln, short click) 
{
	_FocusIcsTky( pwnd, ln, click ) ;

	OnICSInputFocus( TKY_PN );
}
void CDBDinpView::_FocusIcsTky( CWnd* pwnd, int ln, short click) 
{
#else
void CDBDinpView::FocusIcsTky( CWnd* pwnd, int ln, short click) 
{
#endif

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
		if( BRNCHK_PN != -1 )
			return;

		//別画面入力消去
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}

		if( _dline_chg( pwnd, ln, TKY_PN, click ? 0 : 1 ) == -1 )
			return;

		if( CheckAppendSansyo() )
			return;

		//当項目まで自動入力
		if( click ) {
			if( INP_mode == _APPEND && !is_AutoInp_use() )
				AutoInp( TKY_PN );
		}
		// 取消データの場合はＳＥＱに
		if( isdeldata() )
		{
			SAVE_PN = TKY_PN;
			set_focus( SEQ_PN );
			return;
		}
		else {
			SAVE_PN = -1;
			if( data_denpdialog(ln) != 0 )
				return;
		}

		//直前のポジションが当ポジションより前で、金額が入力されていない場合は直前のポジションに返す。
		int set_pn;
		if( inp_ok( get_nowpn(), TKY_PN, &set_pn ) == FALSE )
		{
			set_nowpn( set_pn );
			set_focus( /*get_nowpn()*/set_pn );
			return;
		}

		if( get_nowpn() != TKY_PN || IsSCAN_TKYONLY() ) //#* 06.06 /03
		{
			// 連想摘要検索＆表示
			if( rentky_srdsp(CREC.m_dbt, CREC.m_cre) == -1 )
			{
				// 連想摘要が無かっ場合には５０音摘要を表示させる
				pwnd->GetDlgItem( _getINP_CTL( ln, TKY_PN )->IDC_X )->PostMessage( WM_KEYDOWN, VK_HOME );
# ifndef CLOSE	// 10.02 /99
				pwnd->GetDlgItem( _getINP_CTL( ln, TKY_PN )->IDC_X )->PostMessage( WM_KEYUP, VK_HOME );
# endif
			}
		}
		// カーソルポジションセット
//		set_cursorpos( getINP_CTL( pwnd, TKY_PN )->IDC_X, getLINE_DATA()->LINE_CND[ TKY_PN ].CSLpos, CICSInput );

		set_nowpn( TKY_PN );

		SelWndShow( TRUE );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}



// 摘要ターミネーション
void CDBDinpView::TerminationIcsinputTky( CWnd* pwnd, int ln, long nChar, long inplen, long kst ) 
{
	CString string;
	char srec_tks[128] = {0};
	int first = FALSE;
	int i;
//	struct _ItemData check={0};
	char	check[128] = {0};
	VARIANT var;
	short del_pos,pos;
	int nxt = 0;
# ifdef REGDATA	// 11.06 /00
	BOOL rdt = FALSE;
# endif


//FILETrace( "CREC %d, ldata %d\n", CREC.m_seq, _getLINE_DATA( ln )->_XXDTA.dd_seq );
	try
	{
		// 違うデータの場合は以下の処理をスキップ
		if( CREC.m_seq != _getLINE_DATA( ln )->_XXDTA.dd_seq ) {
//FILETrace( "CREC %d, ldata %d\n", CREC.m_seq, _getLINE_DATA( ln )->_XXDTA.dd_seq );
			return;
		}

		// 漢字摘要の取出し（イメージはオール漢字スペース）
		if( SREC.m_imgsq > 0 ) {
//			kspclr( srec_tks, Voln1->tk_ln );
		}
		else {
			strcpy_s( srec_tks, sizeof srec_tks, SREC.m_tekiyo );
		}

		//#* 05.24 /01	摘要のみ修正時のイメージ修正はDELETE なしでも可能
		if( getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg == 1 && ! IsSCAN_TKYONLY() )
		{
			// イメージ摘要
			memset( &check, '\0', sizeof(check));
			var.pbVal = (BYTE*)check;

			exDBdata_set( TKY_PN, _getINP_CTL( ln, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			goto NEXT_POS0;
		}
		else
		{
			if( get_nowpn() == TKY_PN && ! is_AutoInp_use() ) {
				pos = ((CICSDBEDT*)pwnd->GetDlgItem( _getINP_CTL( ln, TKY_PN )->IDC_X))->GetCaretPosition();
//MyTrace( "摘要 Caret = %d\n", pos );

				exDBdata_get( TKY_PN, _getINP_CTL( ln, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				if( var.pbVal != NULL )	strcpy_s( check, sizeof check, (char*)var.pbVal );
				else					memset( &check, '\0', sizeof(check));

				teki_stringcheck( check, Voln1->tk_ln * 2 );

				var.pbVal = (BYTE*)check;
				exDBdata_set( TKY_PN, _getINP_CTL( ln, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				((CICSDBEDT*)pwnd->GetDlgItem( _getINP_CTL( ln, TKY_PN )->IDC_X))->SetCaretPosition( pos );
			}
			else {
				strncpy_s( check, sizeof check, CREC.m_tekiyo, (Voln1->tk_ln*2) );
			}

			if( dinp_modefy_ok( getLINE_DATA() ) )
			{
				_getLINE_DATA(ln)->_XXDTA.dd_tek.tekiyo = check;
			}
			else
			{
				if( strcmp( check, _getLINE_DATA(ln)->_XXDTA.dd_tek.tekiyo ) )
				{
					Buzzer();
				}
				::ZeroMemory( &check, sizeof (check));

				strncpy_s( check, sizeof check, _getLINE_DATA(ln)->_XXDTA.dd_tek.tekiyo, Voln1->tk_ln*2 );
				var.pbVal = (BYTE*)check;
				exDBdata_set( TKY_PN, _getINP_CTL( ln, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			string.Format( _T("%.80s"), check );
			i = string.GetLength();
			//#* 05.24 /01	摘要のみ修正でイメージ摘要の時
			if( getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg == 1 && IsSCAN_TKYONLY() ) {
				if( i == 0 ) {
					// イメージ摘要で、文字入力がない
					memset( &check, '\0', sizeof(check));
					var.pbVal = (BYTE*)check;
					exDBdata_set( TKY_PN, _getINP_CTL( ln, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
					goto NEXT_POS0;
				}
				else {
					// 文字を入力した場合は文字の摘要に
					SREC.m_dsign[0] &= ~0x80;
					CREC.m_dsign[0] &= ~0x80;

					getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg = 0;
					getLINE_DATA()->ImgSEQ = 0;
					TerminationDataSet( TKY_PN );
				}
			}

	//		kjmove( CREC.m_tekiyo.GetBuffer(100), (LPSTR)(LPCTSTR)string, (Voln1->tk_ln*2));	// 02.09 /02 -ANKINP-
	//		CREC.m_tekiyo.ReleaseBuffer();
			CREC.m_tekiyo = string;
			_getLINE_DATA(ln)->_XXDTA.dd_tek.tekiyo = string;
			TerminationDataSet( TKY_PN );

		//	if( i < sizeof( _getLINE_DATA(ln)->_XXDTA.dd_tek.tekiyo ) )
		//		_getLINE_DATA(ln)->_XXDTA.dd_tek.tekiyo[ i ] = '\0';

			// カーソルポジションセーブ
//				get_cursorpos( getINP_CTL( TKY_PN )->IDC_X, getLINE_DATA()->LINE_CND[ TKY_PN ].CSLpos, CICSInput );

		}

NEXT_POS0:
		if( !nChar )
		{
			return;
		}

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		{
			return;
		}

		if( nChar )
		{
			//適要選択欄ページめくり
			nChar = SelTkyDsp( nChar );
			if( !nChar )
				return;

			// 枝番未入力チェックＪＯＢ
			if( (INP_mode != _APPEND && nChar == VK_UP) || nChar == VK_DOWN )
			//	if( isNoBRNInpChk() )
				if( pAUTOSEL->BRNKAK_OPT != OPT_BRNKAK && isNoBRNInpChk() )
					return;

			if( nChar == VK_UP || nChar == VK_DOWN ) {
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			nChar = dline_chg( pwnd, nChar );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合はＳＥＱに
				if( isdeldata() )
				{
					SAVE_PN = TKY_PN;
					set_focus( SEQ_PN );
					return;
				}
				else
					SAVE_PN = -1;

				set_focus( TKY_PN );
				return;
			}
		}

		// ＴＡＢ　１文字コピー
		if( nChar == VK_TAB )
		{
			if( INP_mode == _APPEND )
			{
				tab_copy( pwnd, getINP_CTL( pwnd, TKY_PN )->IDC_X, (char*)srec_tks );
			}
#ifdef _CPYIMG //#* 05.10 /01 イメージ複写
			else if( INP_mode == _MODEFY || INP_mode == _KEYSCAN ) {
				if( dinp_modefy_ok( getLINE_DATA() ) )
				{
					// 表示位置は、選択部分の下座標を基準に
					RECT rect;
					m_selfix8.GetWindowRect( &rect );

					PROGRAM_OK = FALSE;
			
					int img_ret;
					CSelImgDlg imgdlg;
					imgdlg.Initial( pDBzm, &CREC, rect );

					((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(2);

					img_ret = imgdlg.DoModal();

					((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(0);

					PROGRAM_OK = TRUE;

					if( img_ret == IDOK ) {
						if( imgdlg.m_bSelTekImage ) {
							// イメージ摘要

							SREC.m_tekiyo.Empty();
							CREC.m_tekiyo.Empty();
							SREC.m_imgsq = imgdlg.m_CImgSeq;
							CREC.m_imgsq = imgdlg.m_CImgSeq;
							SREC.m_dsign[0] |= 0x80;
							CREC.m_dsign[0] |= 0x80;

							getLINE_DATA()->_XXDTA.dd_tek.tekiyo.Empty();
							getLINE_DATA()->ImgSEQ = imgdlg.m_CImgSeq;
							getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg = 1;

							// イメージ再表示
							TerminationDataSet( TKY_PN );

							::ZeroMemory( &check, sizeof (check));
							var.pbVal = (BYTE*)check;
							exDBdata_set( TKY_PN, _getINP_CTL( ln, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

							// 欄外大きなイメージ
							big_img( getLINE_DATA(), getINP_CTL( pwnd, TKY_PN ));
						}
						else {
							// 文字摘要
							SREC.m_tekiyo.Empty();
							CREC.m_tekiyo.Empty();
							SREC.m_tekiyo = imgdlg.m_Tekiyo;
							CREC.m_tekiyo = imgdlg.m_Tekiyo;
							SREC.m_dsign[0] &= ~0x80;
							CREC.m_dsign[0] &= ~0x80;
						
							getLINE_DATA()->_XXDTA.dd_tek.tekiyo = CREC.m_tekiyo;
							getLINE_DATA()->ImgSEQ = 0;
							getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg = 0;
							// イメージ再表示
							TerminationDataSet( TKY_PN );
		
							::ZeroMemory( &check, sizeof (check));
							strncpy_s( check, sizeof check, _getLINE_DATA(ln)->_XXDTA.dd_tek.tekiyo, Voln1->tk_ln*2 );
							var.pbVal = (BYTE*)check;
							exDBdata_set( TKY_PN, _getINP_CTL( ln, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

						}
						goto NEXT_POS2;

					}
					else {
						// フォーカスが消えてしまうので
						set_focus( TKY_PN );
					}
				}
			}
# endif
			return;
		}
	
		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_DELETE )
		{
			exDBdata_get( TKY_PN, getINP_CTL( pwnd, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			int linelength = 0;
			if( var.pbVal != NULL )	{
				linelength = strlen( (const char*)var.pbVal );
			}

			if( nChar == VK_DELETE )
			{
				if(dinp_modefy_ok( getLINE_DATA() ) )
				{
					switch( getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg )
					{
					case 1:	// イメージ摘要の取消
						memset( &check, '\0', sizeof(check));
						var.pbVal = (BYTE*)check;
						exDBdata_set( TKY_PN, getINP_CTL( pwnd, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

						SREC.m_dsign[0] &= ~0x80;
						CREC.m_dsign[0] &= ~0x80;
						SREC.m_imgsq = 0;
						CREC.m_imgsq = 0;

						getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg = 2;
						getLINE_DATA()->_XXDTA.dd_tek.image = NULL;
						getLINE_DATA()->_XXDTA.dd_tek.height = 0;
						getLINE_DATA()->_XXDTA.dd_tek.width = 0;

						TerminationDataSet( TKY_PN );
						
						goto NEXT_POS1;
						break;

					case 2: // イメージ摘要の取消の取消
						if( ! linelength )
						{
							SREC.m_tekiyo.Empty();
							CREC.m_tekiyo.Empty();
							SREC.m_imgsq = getLINE_DATA()->ImgSEQ;
							CREC.m_imgsq = getLINE_DATA()->ImgSEQ;
							SREC.m_dsign[0] |= 0x80;
							CREC.m_dsign[0] |= 0x80;

							getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg = 1;
							TerminationDataSet( TKY_PN );

							goto NEXT_POS2;
						}
					default:	// 通常の摘要の取消
						//カーソルポジション以降の文字列取り消し
						del_pos = ((CICSDBEDT*)pwnd->GetDlgItem( getINP_CTL( pwnd, TKY_PN )->IDC_X))->GetCaretPosition();
						check[del_pos] = '\0';
						var.pbVal = (BYTE*)check;
						exDBdata_set( TKY_PN, getINP_CTL( pwnd, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
						break;
					}
				}
				else
				{
					Buzzer();
					return;
				}
			}
			else if( !dinp_modefy_ok( getLINE_DATA() ) )
			{
				if( getLINE_DATA()->_XXDTA.dd_tek.tekiyo != check )
				{
					Buzzer();
					::ZeroMemory( &check, sizeof (check));
					strcpy_s( check, sizeof check, _getLINE_DATA(ln)->_XXDTA.dd_tek.tekiyo );
					var.pbVal = (BYTE*)check;
					exDBdata_set( TKY_PN, _getINP_CTL( ln, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				}
			
				goto NEXT_POS2;

			}

			if( getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg == 1 )
				goto NEXT_POS2;

NEXT_POS1:
//			exDBdata_get( TKY_PN, getINP_CTL( pwnd, TKY_PN )->IDC_X, &check, CICSInput );

			string.Format( _T("%.80s"), check );
			i = string.GetLength();


			if( nChar != VK_DELETE && !i && getLINE_DATA()->LINE_CND[TKY_PN].INP_sg == FALSE )
			{
				i = get_inpl( (unsigned char*)srec_tks, Voln1->tk_ln );
			//	i *= 2;	--ANKINP--

				CString fmt;
				fmt.Format( _T("%%.%us"), i );
				string.Format( fmt, srec_tks );

				if( INP_mode == _APPEND )
				{
					TKattr = TKattr2;
					// 空打ち複写の時、摘要消費税のチェックをする。文字列が一致しない場合はクリア [12.10 /12]
					//if( gTKattrec.m_tksgn[1] == TKattr ) {
					//	if( string.Compare( gTKattrec.m_tkname ) != 0 ) {
					//	if( string.Find( gTKattrec.m_tkname ) < 0 ) {
					//		TKattr = 0;
					//	}
					//}
					if( string.Find( gTKattrec.m_tkname ) < 0 ) {
						if( gTKattrec.m_tksgn[1] == TKattr ) {
							TKattr = 0;
						}
						gTKattrec.Reset();
					}

					if( i )
						first = TRUE;

					// インボイス対応
					// 消費税属性もしくは、免税事業者からの課税仕入れサイン、登録番号セットされているか
					// 摘要で 消費税属性セット
					//if( TKattr && pDBsy->IsSyohizeiMaster() )
					if(( TKattr || pDBzm->CheckTkrec( &gTKattrec )) && pDBsy->IsSyohizeiMaster() )
					{
						pDBzm->SetCDBData( &CREC );

						// インボイス対応
						//pDBsy->Sy_tkyo_inp( TKattr );
						_SY_TKREC_ syTkrec = { 0 };
						pDBzm->SetSyTkrec( &syTkrec, &gTKattrec, &CREC );
						pDBsy->Sy_tkyo_inp( &syTkrec );

						CREC.m_dsign.Copy( pDBzm->dbdata->dsign );

						//DBKNREC* pKn1, *pKn2;
						//pKn1 = pDBzm->DB_PjisToKnrec(CREC.m_dbt);
						//pKn2 = pDBzm->DB_PjisToKnrec(CREC.m_cre);
						//int siire = 0;
						//if( pKn1 && pDBzm->km_siire(pKn1) ) {
						//	siire++;
						//}
						//if( pKn2 && pDBzm->km_siire(pKn2) ) {
						//	siire++;
						//}
						//if( siire ) {
						//	sprintf_s(CREC.m_invno, sizeof(CREC.m_invno), _T("%s"), pDBzm->dbdata->invno);
						//}
						//else {
						//	::ZeroMemory(CREC.m_invno, sizeof CREC.m_invno);
						//}
						if( pDBzm->is_invnosiwake(&CREC) ) {
							//未入力の場合のみ
							if( CREC.m_invno[0] == 0 ) {
								sprintf_s(CREC.m_invno, sizeof(CREC.m_invno), _T("%s"), pDBzm->dbdata->invno);
							}
						}

						l_input( CREC.m_zei, (char*)(LPCTSTR)pDBzm->dbdata->zei );
						//消費税
						getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt( &CREC, NULL, INP_mode);
						//税額
						memcpy( getLINE_DATA()->_XXDTA.dd_zei, CREC.m_zei, 6 );
						//消費税再表示
						int pntbl[] = {SKBN_PN, ZEI_PN};
						TerminationDataSet( pntbl, 2 );

						getLINE_DATA()->LINE_CND[ TKY_PN ].TKattr = TKattr;	// コードセーブ
					}
				}
			}
			else if( INP_mode == _APPEND )
				TKattr2 = 0;

			CREC.m_tekiyo = string;

			getLINE_DATA()->_XXDTA.dd_tek.tekiyo = string;
			getLINE_DATA()->LINE_CND[TKY_PN].INP_sg = TRUE;
			TerminationDataSet( TKY_PN );

			if( first )
			{
				::ZeroMemory( &check, sizeof (check));
				strncpy_s( check, sizeof check, getLINE_DATA()->_XXDTA.dd_tek.tekiyo, Voln1->tk_ln*2 );
				var.pbVal = (BYTE*)check;
				exDBdata_set( TKY_PN, getINP_CTL( pwnd, TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				if( !is_AutoInp_use() ) {
					set_focus( get_nowpn() );
				}
				return;
			}
		}
		else if( getLINE_DATA()->LINE_CND[TKY_PN].INP_sg == TRUE )
		{
//			DBdata_set( pwnd, IDC_ICSINPUTCTRL8, TKY_DATA );
		}

NEXT_POS2:
		if( nChar == VK_LEFT || nChar == VK_F2)
		{
			//変動事由・振替科目[公益]チェック
			if( IsHjiyuOrNPfuri( CREC.m_dbt, CREC.m_cre ) ) {
				set_termfocus( get_nowln(), TKY_PN, -1 );
			}
			else {
				//消費税入力ポジションチェック
				pDBzm->SetCDBData( &CREC );
				int pn = SKBN_PN;
				if (pAUTOSEL->DSPSYZ_OPT == OPT_NODSP) {
					m_SET_SYZINP_PN = -1;
					pn = TKY_PN;
				}
				else {
					m_SET_SYZINP_PN = m_pSyzInp->CheckSyzPn(-1);
				}
				if( m_SET_SYZINP_PN >= 0 ) {
					set_focus( SKBN_PN );
				}
				else {
					set_termfocus( get_nowln(), pn, -1 );
				}
			}
		}
		else if( nChar == VK_RETURN || nChar == VK_TAB )
		{
			if( /*1*/ check_lastitem( get_nowln(), TKY_PN, nChar ) && !is_AutoInp_use() ) {
				// 枝番未入力チェックＪＯＢ
				if( isNoBRNInpChk() )
					return;
				//工事番号チェック
				if( isKojiChk() )
					return;
				//部門範囲チェック
				if( isBmnHaniChk() ) 
					return;
				//工事番号 科目チェック
				if( isKojiDataChk() )
					return;
				//部門未入力チェック
				if( isNoBmnInpChk() )
					return;

				if( INP_mode == _APPEND ) {
#ifdef _SLIP_ACCOUNTS_
					if( IsDispSlip() ){
/*- '12.09.22 -*/
//						if( (m_ImgDispOption.autoLkType==ID_AUTOLINK_ON) && (m_SlipBtnState==ID_LINKON_BSTATE) ){
/*-------------*/
/*-- '12.10.03 --*/
//						if( (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_DO_LINKIMG) && (m_ImgDispOption.autoLkType==ID_AUTOLINK_ON) && (m_SlipBtnState==ID_LINKON_BSTATE) ){
/*---------------*/
						if( (m_ImgDispOption.autoLkType==ID_AUTOLINK_ON) && (m_SlipBtnState==ID_LINKON_BSTATE) ){
/*---------------*/
/*-------------*/
/*- '12.07.02 -*/
//							if( m_pImgmng->ConnectDataToImgFile(pDBzm, INP_LINE._XXDTA.dd_seq) == 0 ){
//								CREC.m_dsign[6] |= 0x01;
//							}
/*-------------*/
#ifndef _SCANNER_SV_
							int docseq;
							if( (docseq=m_pImgmng->GetDocseqOnImgDlg()) != -1 ){
								CREC.m_dsign[6] |= 0x01;
								m_ContDocseqAry.Add( docseq );
							}
#endif
							CArray<int> tmpDocseqAry;
							if( m_pImgmng->GetLinkDocseqOnDlg(tmpDocseqAry) != -1 ){
								CREC.m_dsign[6] |= 0x01;
								m_ContDocseqAry.RemoveAll();
								m_ContDocseqAry.Copy( tmpDocseqAry );
							}
//------------------
/*-------------*/
						}
					}
#endif
					DB_AppendOperation( pwnd, TKY_PN );

#ifdef _SLIP_ACCOUNTS_
					if( IsDispSlip() ){
						// 関連対象有り？ ('12.11.01)
						int cnt = (int)m_ContDocseqAry.GetCount();
						if( cnt ){
							m_pImgmng->ConnectDataToImgFile( pDBzm, REAL_SEQ_NO, m_ContDocseqAry );
							m_ContDocseqAry.RemoveAll();
#ifndef _SCANNER_SV_
							if( cnt == 1 ){
								m_pImgmng->DeleteAllChildImgDlg();
								if( m_ImgDispOption.nextpgType == ID_AUTO_NEXTPAGE ){
									m_pImgmng->DispImgDlgNextPage();
								}
							}
							else{
								if( m_ImgDispOption.nextpgType == ID_AUTO_NEXTPAGE ){
									m_pImgmng->DispImgDlgUnlinkNextPage();
								}
							}
#endif
							if( m_ImgDispOption.nextpgType == ID_AUTO_NEXTPAGE ){
								m_pImgmng->DispImgDlgUnlinkNextPage();
							}
//------------------
						}
						else{
							if( m_ImgDispOption.nextpgType == ID_AUTO_NEXTPAGE ){
								m_pImgmng->DispImgDlgNextPage();
							}
						}

						if( m_ImgDispOption.nextpgType == ID_AUTO_NEXTPAGE ){
							ChgLinkBtnState( ID_LINKON_BSTATE );
						}
						else{
							ChgLinkBtnState( ID_LINKON_BSTATE );
						}
						// 摘要にフォーカスが移動してしまう為の対処
						set_nowpn( DATE_PN );
						set_focus( DATE_PN );
					}
#endif
				}
				else {
					i = dline_chg( pwnd, VK_DOWN );

					if( !i ) {
						if( IsSCAN_TKYONLY() ) {
							set_focus( TKY_PN );
						}
						else {
							set_focus( DATE_PN );// SCAN 時など 次の行がない
						}
					}
					else {
						set_focus( get_nowpn() );
					}
				}
			}
			else {
				// 次の移動できる項目がなければ、
				if( !is_AutoInp_use() )
				{
					set_termfocus( get_nowln(), TKY_PN, 1 );
				}
			}
		}
		else if( nChar == VK_RIGHT ) {
			if( INP_mode != _APPEND && get_nowln() != APPEND_LINE ) {
				int lcnt = m_DtList.GetLines();
				if( lcnt == 1 ) {
					// 1行モードの仕訳修正時の 右矢印 は 付箋
					m_OneHusen = ONEHUSEN_PN;

					int sub_pn;
					BOOL bOK = dinp_modefy_ok( getLINE_DATA() );
					if( bCONFIRM_MASTER )	bOK = FALSE;
					sub_pn = m_pSubDtInp->SetCurData( bOK, &CREC );
					SubDataInpDisp( sub_pn );
#ifdef VER2_CLOSE
					getLINE_DATA()->LINE_CND[HSEN_PN].INP_sg = TRUE;

					HSEN_DATA crec = {0}, srec = {0};
					crec.hsen_inpsg = getLINE_DATA()->LINE_CND[HSEN_PN].INP_sg;
					crec.hsen_sel = (CREC.m_dsign[7]&0x0f);
					strcpy_s( crec.hsen_cmt, sizeof crec.hsen_cmt, CREC.m_tag );

					srec.hsen_sel = (SREC.m_dsign[7]&0x0f);
					strcpy_s( srec.hsen_cmt, sizeof srec.hsen_cmt, SREC.m_tag );

					m_pHusenInp->SetHusenData( &crec, &srec );
					HusenInpDisp( TRUE );
#endif
				}
				else {
					m_OneHusen = 0;
				}
			}
			else {
				m_OneHusen = 0;
			}

			if( ! m_OneHusen ) {
				if( ! check_lastitem( get_nowln(), TKY_PN, nChar ) ) {
					set_termfocus( get_nowln(), TKY_PN, 1 );
				}
			}
		}
		else if( nChar == VK_DELETE || nChar == VK_F12 || nChar == 0x16 )
			return;
		else
		{
			Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// 新規仕訳ラインの書き込み 作業
void CDBDinpView::DB_AppendOperation( CWnd* pwnd, int pn )
{
//	BeginWaitCursor();
//	CWaitCursor wait;
	int i, rdt, nxt = 0;
	int no;

	//スクロールデータリセット
	if( m_InsLine == -1 && (no = DDATA_LINE[ SCROLL_NL()-1 ].ZREC_NO) > 0 ) {
//	if( (no = DDATA_LINE[ SCROLL_NL()-1 ].ZREC_NO) > 0 ) {

		if( BOTOM_SEQ != SEQ_APPEND.ngetseq( ZREC[ no - 1].m_seq - 1) )
		{
			for( i = 0; i < RECMAX; i++ )
				ZREC[i].Reset();

			CDBINPDataRec key;
			key.m_seq = APPEND_SEQ.ngetseq( SEQ_NO - 1);
			if( key.m_seq <= 0 ) key.m_seq = USERS_SEQ;

	//		scandat_getn( &ZREC[0], &key, SCROLL_NL() );
			scaninsdat_getn( &ZREC[0], -1, SCROLL_NL() );

			scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );
			D_SEQN = SCROLL_NL()-1;

			set_nowln( APPEND_LINE );

			ddata_dsp(-1);

			//消費税 -- 元のソースもコメントアウト
//			pDBsy->Sy_iniz( &CREC, 0 );
			//ddata_dsp で 上の仕訳に対して SyIniz()をしているので、再度行う
			DB_SyIniz( &CREC );
		}
	}
	TKattr = getLINE_DATA()->LINE_CND[ TKY_PN ].TKattr;	// コードリセット
	m_bSyzSotoMen = FALSE;
	m_pnZeiSotoMen == 0;

#ifdef IMPORT	// 10.05 /98
	nxt = WandNextData();
#else
	WandNextData();
#endif

	ddata_dsp(-1);

	//データラインイニシャライズ
	inpline_inz( &INP_LINE );
	INP_LINE._XXDTA.dd_ddate = SREC.m_ddate;	//初期日付
	INP_LINE._XXDTA.dd_ksign = SREC.m_ksign;
	INP_LINE._XXDTA.dd_mofs = SREC.m_mofs;
	pDBzm->DateConv( INP_LINE.BcdDate, &SREC, CV2_1 );
	inpline_dsp( &INP_LINE, APPEND_LINE );

	// 空データをOCX にセット( kill focus にデータを取得するので)
	VARIANT var;
	var.pbVal = NULL;
	m_Input.SetData( &var, ICSDBEDT_TYPE_STRING, 0 );

	// 04.03 /03 --- 新規書き込み時
	if( pAUTOSEL->SYOBUZ_OPT == OPT_SYOBUZ && is_syobuzzer_on(pn) )
		Buzzer();


#ifdef REGDATA	// 11.06 /00
	if( !nxt )
	{
		// 定型仕訳を入力ラインにセット
		// 内部で、DB_SyIniz を表示用で呼び出す。
		rdt = RdataToInpData( &CREC, &SREC );
		// データを OCXにセット
		if( rdt ) {
			if( pn == TKY_PN ) {
				var.pbVal = (BYTE*)(LPCTSTR)CREC.m_tekiyo;
				m_Input.SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
			}
		}
	}
	else
		rdt = FALSE;
#endif
		i = 0;

//	EndWaitCursor();

	// 02.20 /02
	if(pAUTOSEL->KMPG_OPT == OPT_NONOPT ) {
		// 先頭ページに戻る
		if( INP_mode == _APPEND ) {
			SelPar par;
			par.sel_pn = SL_KAMOKU_PN;
			m_Seldata.SelectJob( &par, -1 );
		}
	}

	m_DtLine.HideTitleTipWindow();

	if( !rdt )	DB_SyIniz( &CREC );


	if( !i )	//////// ここは新規書き込み＆そうでない場合もいる
	{
#ifdef IMPORT	// 10.05 /98
		if( INP_mode == _APPEND )
		{
			if( !nxt )
			{
				NXTDTA_sg = FALSE;

# ifdef REGDATA	// 11.06 /00
				if( rdt == FALSE )
					set_focus( DATE_PN );
# else
				set_focus( DATE_PN );
# endif
			}
			else
			{
				NXTDTA_sg = TRUE;

				//金額まで自動入力
				if( INP_mode == _APPEND && !is_AutoInp_use() )
					AutoInp( VAL_PN, TRUE ); // 03.09 /02 引数追加
				set_focus( VAL_PN );
			}
		}
		else
			set_focus( DATE_PN );
#else
		set_focus( DATE_PN );
#endif
	}
	else {
		set_focus( get_nowpn() );
	}

	RDRemainDispJOB();

	if( INP_mode == _APPEND )
	{
		if( scheck( nxt == -1 ? 2 : 1 ) ) 
		{
			ErrExit( 0, get_Dfullmsg() );
			EndBlock( 0 );
			return;
		}
		else if( bSCAN_ONLY_MODE )
		{
			// Go To 検索項目登録
			OnButtonF8();
			return;
		}
	}


}

//////////////////////////////////////////////////////////////////////////////////////
// 手形番号

// フォーカス
#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsSyono( CWnd* pwnd, int ln, short click )
{
	_FocusIcsSyono( pwnd, ln, click );

	OnICSInputFocus( SYON_PN );
}
void CDBDinpView::_FocusIcsSyono( CWnd* pwnd, int ln, short click )
{
#else
void CDBDinpView::FocusIcsSyono( CWnd* pwnd, int ln, short click )
{
#endif

	if( PROGRAM_OK == FALSE )
		return;

	try
	{

		//別画面入力消去
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}

		if( _dline_chg( pwnd, ln, SYON_PN, click ? 0 : 1  ) == -1 )
			return;

		if( CheckAppendSansyo() )
			return;

		//当項目まで自動入力
		if( click ) {
			if( INP_mode == _APPEND && !is_AutoInp_use() )
				AutoInp( SYON_PN );
		}

		// 取消データの場合はＳＥＱに
		if( isdeldata() )
		{
			SAVE_PN = SYON_PN;
			set_focus( SEQ_PN );
			return;
		}
		else {
			SAVE_PN = -1;
			if( data_denpdialog(ln) != 0 )
			return;
		}
#ifdef KEEP_CLOSE	// 入力できないからといってマウスでのクリックをはじかない
		else if( ! IsBillData(getLINE_DATA()->_XXDTA.dd_dbt, getLINE_DATA()->_XXDTA.dd_cre ) ) {
					// その処理によるバグを防ぐ
			int pn = get_nowpn();
			if( pn == SYON_PN || pn == TGDT_PN || pn < 0 )
				pn = VAL_PN;
			set_nowpn( SYON_PN );
			set_focus( pn );
			return;
		}
		else {
			SAVE_PN = -1;
		}
#endif

		//直前のポジションが当ポジションより前で、借方科目が入力されていない場合は直前のポジションに返す。
		int set_pn;
		if( inp_ok( get_nowpn(), SYON_PN, &set_pn ) == FALSE )
		{
			set_nowpn( set_pn );
			set_focus( /*get_nowpn()*/set_pn );
			return;
		}

		set_nowpn( SYON_PN );

		SelWndShow( FALSE );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

/***********************
	手形番号入力メイン
************************/
void CDBDinpView::TerminationIcsinputSyono( CWnd* pwnd, int ln, long nChar, long inplen, long kst  )
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
//struct _ItemData check = {0};
CString ip_snumber;
char buf[30];
size_t length;
VARIANT var;

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
//		if( !nChar )	//フォーカスＯＦＦ
//		{
			exDBdata_get( SYON_PN, getINP_CTL( pwnd, SYON_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			if( var.pbVal )	length = strlen( (LPCTSTR)var.pbVal );
			else			length = 0;
			
			if( length > 0 ) {
				ip_snumber = var.pbVal;
			}
			else {
				ip_snumber.Empty();
			}

			if( !dinp_modefy_ok( getLINE_DATA() ) /*|| !IsBillData(CREC.m_dbt, CREC.m_cre)*/ )
			{
				if( getLINE_DATA()->_XXDTA.dd_snumber != ip_snumber )
				{
					Buzzer();
				}
				if( ! getLINE_DATA()->_XXDTA.dd_snumber.IsEmpty() ) {
					strcpy_s( buf, sizeof buf, getLINE_DATA()->_XXDTA.dd_snumber );
				}
				else {
					::ZeroMemory( buf, sizeof buf );
				}
				var.pbVal = (BYTE*)buf;
				exDBdata_set( SYON_PN, getINP_CTL( pwnd, SYON_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			else
			{
				if( getLINE_DATA()->LINE_CND[SYON_PN].INP_sg == FALSE && ip_snumber.IsEmpty() && ! inplen )
				{
					if( getLINE_DATA()->LINE_CND[SYON_PN].NEXT_MOVE_KEY == FALSE )
						goto NEXT;
				}

				if( INP_mode == _APPEND && getLINE_DATA()->LINE_CND[SYON_PN].INP_sg == FALSE )
				{
					// 手形番号自動入力
					if( ip_snumber.IsEmpty() )
						ip_snumber = SREC.m_snumber;
				}

				CREC.m_rnumber = ip_snumber;
				SREC.m_rnumber = ip_snumber;

				getLINE_DATA()->_XXDTA.dd_snumber = ip_snumber;
				getLINE_DATA()->LINE_CND[SYON_PN].INP_sg = TRUE;

				if( ! getLINE_DATA()->_XXDTA.dd_snumber.IsEmpty()  ) {
					strcpy_s( buf, sizeof buf, getLINE_DATA()->_XXDTA.dd_snumber );
				}
				else {
					::ZeroMemory( buf, sizeof buf );
				}
				var.pbVal = (BYTE*)buf;
				exDBdata_set( SYON_PN, getINP_CTL( pwnd, SYON_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		//		data_set( getINP_CTL( pwnd, SYON_PN )->IDC_X, &getLINE_DATA()->DENP_DATA, CICSInput );
				TerminationDataSet( SYON_PN );

			}
//			return;
//		}

NEXT:
		getLINE_DATA()->LINE_CND[SYON_PN].NEXT_MOVE_KEY = FALSE;

		if( !nChar ) {
			return;
		}

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return;

		if( nChar )
		{
			if( nChar == VK_UP || nChar == VK_DOWN ) {
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			nChar = dline_chg( pwnd, nChar );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合はＳＥＱに
				if( isdeldata() )
				{
					SAVE_PN = SYON_PN;
					set_focus( SEQ_PN );
					return;
				}
				else
					SAVE_PN = -1;

				int set_pn = pn_movecheck( SYON_PN );
				set_focus( set_pn );
				return;
			}
		}

		if( dinp_modefy_ok( getLINE_DATA() ) && nChar == VK_DELETE )
		{
			ip_snumber.Empty();
			::ZeroMemory( buf, sizeof buf );
			var.pbVal = (BYTE*)buf;
			exDBdata_set( SYON_PN, getINP_CTL( pwnd, SYON_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			CREC.m_rnumber = ip_snumber;
			SREC.m_rnumber = ip_snumber;

			getLINE_DATA()->_XXDTA.dd_snumber = ip_snumber;
			getLINE_DATA()->LINE_CND[SYON_PN].INP_sg = TRUE;

			TerminationDataSet( SYON_PN );
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
		{
			getLINE_DATA()->LINE_CND[SYON_PN].NEXT_MOVE_KEY = TRUE;

			if( !is_AutoInp_use() )
			{
				set_termfocus( get_nowln(), SYON_PN, 1 );
			}
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 )
			set_termfocus( get_nowln(), SYON_PN, -1 );
		else if( nChar == VK_DELETE )
			;
		else {
			if( nChar != 0x16 )
				Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}



//////////////////////////////////////////////////////////////////////////////////////
// 手形期日

// フォーカス
#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsTgDate( CWnd* pwnd, int ln, short click )
{
	_FocusIcsTgDate( pwnd, ln, click );

	OnICSInputFocus( TGDT_PN );
}
void CDBDinpView::_FocusIcsTgDate( CWnd* pwnd, int ln, short click )
{
#else
void CDBDinpView::FocusIcsTgDate( CWnd* pwnd, int ln, short click )
{
#endif

	if( PROGRAM_OK == FALSE )
		return;

	try
	{

		//別画面入力消去
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}

		if( _dline_chg( pwnd, ln, TGDT_PN, click ? 0 : 1  ) == -1 )
			return;

		if( CheckAppendSansyo() )
			return;
		
//		if( pAUTOSEL->BILL_OPT == OPT_NOINP ) {
//			set_focus( get_nowpn() );		
//		}

		//当項目まで自動入力
		if( click ) {
			if( INP_mode == _APPEND && !is_AutoInp_use() )
				AutoInp( TGDT_PN );
		}

		// 取消データの場合はＳＥＱに
		if( isdeldata() )
		{
			SAVE_PN = TGDT_PN;
			set_focus( SEQ_PN );
			return;
		}
		else {
			SAVE_PN = -1;
			if( data_denpdialog(ln) != 0 )
				return;
		}
#ifdef KEEP_CLOSE
		else if( ! IsBillData(getLINE_DATA()->_XXDTA.dd_dbt, getLINE_DATA()->_XXDTA.dd_cre ) ) {
			int pn = get_nowpn();
			if( pn == SYON_PN || pn == TGDT_PN || pn < 0 )
				pn = VAL_PN;
			set_nowpn( TGDT_PN );
			set_focus( pn );
			return;
		}
		else
			SAVE_PN = -1;
#endif

		//直前のポジションが当ポジションより前で、借方科目が入力されていない場合は直前のポジションに返す。
		int set_pn;
		if( inp_ok( get_nowpn(), TGDT_PN, &set_pn ) == FALSE )
		{
			set_nowpn( set_pn );
			set_focus( /*get_nowpn()*/set_pn );
			return;
		}

		set_nowpn( TGDT_PN );

		SelWndShow( FALSE );
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
long CDBDinpView::CheckDataIcsinputTgDate( CWnd* pwnd, long nChar ) 
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
	try
	{
		exDBdata_get( TGDT_PN, getINP_CTL(pwnd, TGDT_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		if( var.pbVal != NULL )	strcpy_s( data, sizeof data, (char*)var.pbVal );
		else					memset( data, '\0', sizeof data );

		struct _DATA_LINE *ldata;
		CVolDateDB vd;
		int tmp, yy, gengo;

		ldata = getLINE_DATA();
		if( (ret = TgDateInputCheck( data, result, sizeof result, Voln1->ss_ymd, Voln1->ee_ymd, SREC.m_tdate )) <= 0 ) {
			// 不正データが入力された
			if( ldata->_XXDTA.dd_tdate != -1 ) {

				// 西暦から 元号へ変換
				tmp = ldata->_XXDTA.dd_tdate;
				if( vd.db_datecnvGen( 0, tmp, &gengo, &yy, 0, 0 ) != 0 )
					return -1;

				sprintf_s( result, sizeof result, "%d", yy );
				var.pbVal = (BYTE*)result;
			}
			else	var.pbVal = NULL;

			exDBdata_set( TGDT_PN, getINP_CTL(pwnd, TGDT_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			IntsrekitoGenstr( buf, sizeof buf, ldata->_XXDTA.dd_tdate );
			exDBSetDispString( TGDT_PN, getINP_CTL(pwnd, TGDT_PN)->IDC_X, buf );
			return -1;
		}
		else {
//			memcpy( check, bcd, 3 );
		}

		if( dinp_modefy_ok( getLINE_DATA() )  )
		{
			var.pbVal = (BYTE*)result;
			exDBdata_set( TGDT_PN, getINP_CTL(pwnd, TGDT_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
//			exDBSetDispString( TGDT_PN, getINP_CTL(pwnd, DATE_PN)->IDC_X, str );
//			data_reply( pwnd, getINP_CTL(pwnd, DATE_PN)->IDC_X, 1, &ipdata );
		}
		else
		{
			Buzzer();
			var.pbVal = NULL;
			exDBdata_set( TGDT_PN, getINP_CTL(pwnd, TGDT_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			
			IntsrekitoGenstr( str.GetBuffer(30), 30, ldata->_XXDTA.dd_tdate );
			str.ReleaseBuffer( -1 );
			exDBSetDispString( TGDT_PN, getINP_CTL(pwnd, TGDT_PN)->IDC_X, str );
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

/***********************
	手形期日入力メイン
************************/
void CDBDinpView::TerminationIcsinputTgDate( CWnd* pwnd, int ln, long nChar, long inplen, long kst  )
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
int i, gen_tgdate, ip_tgdate = -1;
char buf[30];
VARIANT var;
CVolDateDB vd;

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
//		if( !nChar )	//フォーカスＯＦＦ
//		{
			exDBdata_get( TGDT_PN, getINP_CTL( pwnd, TGDT_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			if( var.pbVal != NULL )	gen_tgdate = atoi( (const char*)var.pbVal );
			else					gen_tgdate = 0;

			int gengo;

			if( gen_tgdate == 0 ) {
				gen_tgdate = ip_tgdate = -1;
			}

			if( gen_tgdate > 0 ) {	// 元号 -> 西暦
				sprintf_s( buf, sizeof buf, "%06d", gen_tgdate );
				BYTE bcddate[10] = {0};
				l_pack( bcddate, buf, 6 );
				ip_tgdate = TgDateBcdToSeireki( bcddate, NULL );
			//	vd.db_datecnvGen( gen_tgdate, &gengo, &ip_tgdate, 1, 0 );
			}

			if( !dinp_modefy_ok( getLINE_DATA() ) /*|| !IsBillData(CREC.m_dbt, CREC.m_cre)*/ )
			{
				if( getLINE_DATA()->_XXDTA.dd_tdate != ip_tgdate )
				{
					Buzzer();
				}
				if( getLINE_DATA()->_XXDTA.dd_tdate > 0 ) {

					vd.db_datecnvGen( 0, getLINE_DATA()->_XXDTA.dd_tdate, &gengo, &gen_tgdate, 1, 0 );
					sprintf_s( buf, sizeof buf, "%d", gen_tgdate );
				}
				else {
					::ZeroMemory( buf, sizeof buf );
				}
				var.pbVal = (BYTE*)buf;
				exDBdata_set( TGDT_PN, getINP_CTL( pwnd, TGDT_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			else
			{
				if( getLINE_DATA()->LINE_CND[TGDT_PN].INP_sg == FALSE && ip_tgdate <= 0 && ! inplen )
				{
					if( getLINE_DATA()->LINE_CND[TGDT_PN].NEXT_MOVE_KEY == FALSE )
						goto NEXT;
				}

				if( INP_mode == _APPEND && getLINE_DATA()->LINE_CND[TGDT_PN].INP_sg == FALSE )
				{
					if( ip_tgdate == -1 )
						ip_tgdate = SREC.m_tdate;
				}

				CREC.m_tdate = ip_tgdate;
				SREC.m_tdate = ip_tgdate;

				getLINE_DATA()->_XXDTA.dd_tdate = ip_tgdate;
				IntsrekitoGenstr( buf, sizeof buf, ip_tgdate );
				getLINE_DATA()->_XXDTA.dd_tdstr = buf;
				getLINE_DATA()->LINE_CND[TGDT_PN].INP_sg = TRUE;

				if( getLINE_DATA()->_XXDTA.dd_tdate > 0 ) {
					sprintf_s( buf, sizeof buf, "%d", gen_tgdate );
				}
				else {
					::ZeroMemory( buf, sizeof buf );
				}
				var.pbVal = (BYTE*)buf;
				exDBdata_set( TGDT_PN, getINP_CTL( pwnd, TGDT_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		//		data_set( getINP_CTL( pwnd, TGDT_PN )->IDC_X, &getLINE_DATA()->DENP_DATA, CICSInput );
				TerminationDataSet( TGDT_PN );

			}
//			return;
//		}

NEXT:
		getLINE_DATA()->LINE_CND[TGDT_PN].NEXT_MOVE_KEY = FALSE;

		if( !nChar ) {
			return;
		}

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return;

		if( nChar )
		{
			if( nChar == VK_UP || nChar == VK_DOWN ) {
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			nChar = dline_chg( pwnd, nChar );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合はＳＥＱに
				if( isdeldata() )
				{
					SAVE_PN = TGDT_PN;
					set_focus( SEQ_PN );
					return;
				}
				else
					SAVE_PN = -1;

				int set_pn = pn_movecheck( TGDT_PN );
				set_focus( set_pn );
				return;
			}
		}

		if( dinp_modefy_ok( getLINE_DATA() ) && nChar == VK_DELETE )
		{
			gen_tgdate = ip_tgdate = -1;
			::ZeroMemory( buf, sizeof buf );
			var.pbVal = (BYTE*)NULL;
			exDBdata_set( TGDT_PN, getINP_CTL( pwnd, TGDT_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			CREC.m_tdate = ip_tgdate;
			SREC.m_tdate = ip_tgdate;

			getLINE_DATA()->_XXDTA.dd_tdate = ip_tgdate;
			getLINE_DATA()->_XXDTA.dd_tdstr.Empty();
			getLINE_DATA()->LINE_CND[TGDT_PN].INP_sg = TRUE;

			TerminationDataSet( TGDT_PN );
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
		{
			getLINE_DATA()->LINE_CND[SYON_PN].NEXT_MOVE_KEY = TRUE;

			if( nChar == VK_RETURN || nChar == VK_TAB ) {
				if( check_lastitem( get_nowln(), TGDT_PN, nChar ) && !is_AutoInp_use() ) {
					if( INP_mode == _APPEND ) {
						DATAandDOC_AppendOperation( TGDT_PN );
					}
					else {
						i = dline_chg( this, VK_DOWN );

						if( !i ) {
							if( IsSCAN_TKYONLY() ) {
								set_focus( TKY_PN );
							}
							else {
								set_focus( DATE_PN );// SCAN 時など 次の行がない
							}
						}
						else {
							set_focus( get_nowpn() );
						}
					}
				}
				else {
					if( !is_AutoInp_use() )
					{
						set_termfocus( get_nowln(), TGDT_PN, 1 );
					}
				}
			}
			else {
				if( !is_AutoInp_use() )
				{
					set_termfocus( get_nowln(), TGDT_PN, 1 );
				}
			}
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 )
			set_termfocus( get_nowln(), TGDT_PN, -1 );
		else if( nChar == VK_DELETE )
			;
		else {
			if( nChar != 0x16 )
				Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


/*
void CDBDinpView::set_focus( CWnd* pwnd, int pn )
{
	_set_focus( get_nowln(), pn );

}
*/


#if (FOCUS_DEBUG == 1)
#undef FILETrace
#define FILETrace			MyTrace
#endif


#if (FOCUS_DEBUG == 1)
void CDBDinpView::set_focusTrace( int pn, int iClick/*=0*/, int line/*=0*/, char* func/*=0*/, char* fname/*=0*/ )
{
	FILETrace( ">>>>>> set_focus pn = %d, iClick = %d, L:%d, Func(= %s), File = %s\n", pn, iClick, line, func, fname );
	_set_focusTrace( get_nowln(), pn, iClick, line, func, fname );
#else
void CDBDinpView::set_focus( int pn, int iClick/*=0*/ )
{
FILETrace( ">>>>>> set_focus pn = %d\n", pn );

	_set_focus( get_nowln(), pn, iClick );
#endif
}

#if (FOCUS_DEBUG == 1)
void CDBDinpView::_set_focusTrace( int ln, int pn, int iClick/*=0*/,int line/*=0*/, char* func/*=0*/, char* fname/*=0*/ )
{
	FILETrace( ">>>>>> _set_focus ln = %d, pn = %d, L:%d, Func(= %s), File = %s\n", ln, pn,line, func, fname );
#else
void CDBDinpView::_set_focus( int ln, int pn, int iClick/*=0*/ )
{
FILETrace( ">>>>>> _set_focus ln = %d, pn = %d\n", ln, pn );
#endif

	PostMessage( WM_RETROFOCUS2, ln, MAKELONG( pn, iClick ) );
}

LRESULT CDBDinpView::RetroFocus( WPARAM wParam, LPARAM lParam ) 
{
FILETrace( ">>>>>> RetroFocus ln = %d, wParam = %d, m_DenpSeq = %d\n", get_nowln(), wParam, m_DenpSeq );
	__set_focus( get_nowln(), wParam, 0 );
	return 1;
}

LRESULT CDBDinpView::RetroFocus2( WPARAM wParam, LPARAM lParam ) 
{
FILETrace( ">>>>>>>>> @RetroFocus2 wParam = %d, LoLP = %d, HiLP = %d, m_DenpSeq = %d\n", wParam, LOWORD(lParam), HIWORD(lParam), m_DenpSeq );

	__set_focus( wParam, LOWORD(lParam), HIWORD(lParam) );

	return 1;
}

void CDBDinpView::__set_focus( int ln, int pn, int iClick/*=0*/  )
{
FILETrace( ">>>>>>>>> @__set_focus ln = %d, wParam = %d\n", ln, pn );

	if( ln == APPEND_LINE ) {
		m_DtLine.SetFocus();
		m_DtLine.SetPosition( 1, pn, iClick );
	}
	else {
		m_DtList.SetFocus();
		m_DtList.SetPosition( ln, pn, iClick );
	}
}


#if (FOCUS_DEBUG == 1)
#undef FILETrace
#define FILETrace		ATLTRACE
#endif

// ターミネーション時のフォーカス処理
void CDBDinpView::set_termfocus( int ln, int pn, int vect_n )
{
	DT_RECT* pdt;
	CDataListCtrl* pCtrl;

	if( ln == APPEND_LINE ) {
		pdt = m_DtLine.GetDtRect();
		pCtrl = &m_DtLine;
	}
	else {
		pdt = m_DtList.GetDtRect();
		pCtrl = &m_DtList;
	}

	if( pdt == NULL )
		return;

	int pos, nxt;

LOOP:
	pos = -1;
	nxt = 0;

	for( int i = 0; i < pdt->Dtcnt; i++ ) {
		if( pn == pdt->DtID[i] ) {
			if( vect_n < 0 ) break;
			else	nxt = 1;
		}
		else if( nxt ) {
			if( pdt->DtID[i] != DATA_NONE && pCtrl->GetListItemEnable( 0, pdt->DtID[i] ) ) {
				pos = pdt->DtID[i];
				break;
			}
		}

		if( pdt->DtID[i] != DATA_NONE && pCtrl->GetListItemEnable( 0, pdt->DtID[i] ) )
			pos = pdt->DtID[i];
	}

	if( pos != -1 ) {
		switch( pos ) {
		case DENP_PN:
			if( pAUTOSEL->DENP_OPT == OPT_NON ) {
				pn = DENP_PN;
				goto LOOP;
			}
			break;

		case DNPTYPE_PN:	//伝票タイプへは移動しない。
			pn = DNPTYPE_PN;
			goto LOOP;
			break;

		case ZEI_PN:
			if( vect_n > 0 ) {	// 税額へは、ターミネーションで行かせない
				pn = ZEI_PN;
				goto LOOP;
			}
			else {
				if( !is_syohizeisiwake( &CREC )) {
					pn = ZEI_PN;
					goto LOOP;
				}
			}
			break;
		case SKBN_PN:
			if( vect_n < 0 ) {	
				if( !is_syohizeisiwake( &CREC )) {
					pn = SKBN_PN;
					goto LOOP;
				}
			}
			break;

		case HJIYU_PN:
			if( vect_n < 0 ) {	
				if( CREC.m_hjiyu == 0 ) {
					pn = HJIYU_PN;
					goto LOOP;
				}
			}
			break;

		case SYON_PN:	// 手形科目でない仕訳は、FOCUS なし
		case TGDT_PN:
			if( ! IsBillData(getLINE_DATA()->_XXDTA.dd_dbt, getLINE_DATA()->_XXDTA.dd_cre ) ) {
				pn = pos;
				goto LOOP;
			}
			break;
		}

		_set_focus( ln, pos );
	}
}

//---------------------------------------------------------------
// 現在の項目が、仕訳の 最後の項目か？
//
//---------------------------------------------------------------
BOOL CDBDinpView::check_lastitem( int ln, int pn, long nChar )
{
	int last, nxtbill, i, sgn;
	DT_RECT* pdt;

	if( ln == APPEND_LINE ) {
		last = m_DtLine.GetLastItem();
	}
	else {
		last = m_DtList.GetLastItem();
		pdt = m_DtList.GetDtRect();
	}

	if( last < 0 )
		return FALSE;
	else {
		if( pn == TKY_PN ) {
			if( ln == APPEND_LINE ) {
				DWORD stl = m_DtLine.GetStyle();
				if( (stl & DS_BK_DEBCRE) ) {
					if( IsBillData(getLINE_DATA()->_XXDTA.dd_dbt, getLINE_DATA()->_XXDTA.dd_cre ) )
						return FALSE;
				}
				else {
					// １部門 入力は、手形項目を当面表示しない
					if( nChar != VK_RIGHT )
						return TRUE;
				}
			}
			else {

				nxtbill = 0;
				// 摘要より後の項目に手形があるか？
				for( i = sgn = 0; i < pdt->DtID.GetCount(); i++ ) {
					if( pdt->DtID[i] == TKY_PN ) {
						sgn = 1;
					}
					if( sgn ) {
						if( pdt->DtID[i] == SYON_PN || pdt->DtID[i] == TGDT_PN ) {
							nxtbill = 1;
							break;
						}
					}
				}
				// 手形項目があり、
				if( nxtbill ) {
					//手形のデータ。
					if( IsBillData(getLINE_DATA()->_XXDTA.dd_dbt, getLINE_DATA()->_XXDTA.dd_cre ) )
						return FALSE;
				}

				if( last == SYON_PN || last == TGDT_PN ) {
					if( ! IsBillData(getLINE_DATA()->_XXDTA.dd_dbt, getLINE_DATA()->_XXDTA.dd_cre ) )
						return TRUE;
				}
			}
//			if( pAUTOSEL->BILL_OPT == OPT_NOINP && pAUTOSEL->HUSEN_OPT == OPT_NOINP )
//				return TRUE;
		}
		else if( pn == TGDT_PN ) {
//			if( pAUTOSEL->HUSEN_OPT == OPT_NOINP )
				return TRUE;
		}

		// 付箋は → 矢印より
		if( last == HSEN_PN && pn != HSEN_PN ) {
			if( nChar != VK_RIGHT )
				last = pn;
		}

		return (last == pn);
	}
}

#ifndef _SLIP_ACCOUNTS_
// 現在ポジションセット
int CDBDinpView::set_nowpn( int pn )
{
	int i, ln;

	ln = get_nowln();
	if( ln == APPEND_LINE )
	{
		i = NOW_PN;
		NOW_PN = pn;
	}
	else
	{
		i = DDATA_NOW_PN[ln-1];
		DDATA_NOW_PN[ln-1] = pn;
	}

	if( BRNTKY_sel && i != pn  )
	{
		//科目・摘要選択画面リセット
		SelDspRst( INP_mode );
	}

	disp_change( i, pn );

	//#* 03.15 /02
	if( pn == TKY_PN ) {
		// OnBeginLabelEdit で行う
//		CWnd* pwnd = GetDlgItem( getINP_CTL( TKY_PN )->IDC_X );
//		short pos = ((CICSDBEDT*)pwnd)->GetCaretPosition();
//		((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(1, pos == 0 ? 1 : 0);
	}
	else {
		if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() == INP_VIEW ) {
			if( pn != VAL_PN ) {
				int subtool = 0;
				switch( pn ) {
				case DATE_PN:
					if( pAUTOSEL->DATE_OPT == OPT_FIX )
						subtool = 1;
					break;
				case BMON_PN:
					if( pAUTOSEL->BMON_OPT == OPT_FIX )
						subtool = 1;
					break;
				case KOJI_PN:
					if( pAUTOSEL->KOJI_OPT == OPT_FIX )
						subtool = 1;
					break;
				case DEBT_PN:
					if( pAUTOSEL->DEBT_OPT == OPT_FIX )
						subtool = 1;
					break;
				case CBMN_PN:
					if( pAUTOSEL->BMON_OPT == OPT_FIX )
						subtool = 1;
					break;
				case CKJI_PN:
					if( pAUTOSEL->KOJI_OPT == OPT_FIX )
						subtool = 1;
					break;
				case CRED_PN:
					if( pAUTOSEL->CRED_OPT == OPT_FIX )
						subtool = 1;
				}
				((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(0, subtool);
			}
		}
	}

	return( i );	//直前のポジション
}
#else
// 現在ポジションセット
int CDBDinpView::set_nowpn( int pn )
{
	int i, ln;

	ln = get_nowln();
	if( ln == APPEND_LINE )
	{
		i = NOW_PN;
		NOW_PN = pn;
	}
	else
	{
		i = DDATA_NOW_PN[ln-1];
		DDATA_NOW_PN[ln-1] = pn;
	}

	if( BRNTKY_sel && i != pn  )
	{
		//科目・摘要選択画面リセット
		SelDspRst( INP_mode );
	}

	if( i != pn ) {
		if( pn == DEBT_PN || pn == CRED_PN ) {
			SelWndShow( FALSE );
		}
	}

	disp_change( i, pn );

	//#* 03.15 /02
	if( pn == TKY_PN ) {
		// OnBeginLabelEdit で行う
//		CWnd* pwnd = GetDlgItem( getINP_CTL( TKY_PN )->IDC_X );
//		short pos = ((CICSDBEDT*)pwnd)->GetCaretPosition();
//		((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(1, pos == 0 ? 1 : 0);
	}
	else {
		if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() == INP_VIEW ) {
			ChangeToolbarSub( pn );
		}
	}

	return( i );	//直前のポジション
}
#endif

#ifdef _SLIP_ACCOUNTS_
//-----------------------------------------------------------------------------
// 現在位置に従ってツールバーを変更する ('12.10.02)
//-----------------------------------------------------------------------------
// 引数		pn	：	該当ペイン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBDinpView::ChangeToolbarSub( int pn )
{
	if( pn != VAL_PN ) {
		int subtool = 0;
		switch( pn ) {
		case DATE_PN:
			if( pAUTOSEL->DATE_OPT == OPT_FIX )
				subtool = 1;
			break;
		case BMON_PN:
			if( pAUTOSEL->BMON_OPT == OPT_FIX )
				subtool = 1;
			break;
		case KOJI_PN:
			if( pAUTOSEL->KOJI_OPT == OPT_FIX )
				subtool = 1;
			break;
		case DEBT_PN:
			if( pAUTOSEL->DEBT_OPT == OPT_FIX )
				subtool = 1;
			break;
		case CBMN_PN:
			if( pAUTOSEL->BMON_OPT == OPT_FIX )
				subtool = 1;
			break;
		case CKJI_PN:
			if( pAUTOSEL->KOJI_OPT == OPT_FIX )
				subtool = 1;
			break;
		case CRED_PN:
			if( pAUTOSEL->CRED_OPT == OPT_FIX )
				subtool = 1;
			break;

		case SEQ_PN:
			if( INP_mode == _MODEFY || INP_mode == _KEYSCAN )
				subtool = 2;
		}
		int tooltype = 0;

/*- '12.10.06 -*/
//		ChangeImgDlgDisp( pn, tooltype );
/*-------------*/
		if( IsDispSlip() ){
			tooltype = 6;
		}
/*-------------*/
		if( tooltype != -1 ){
			if( tooltype == 6 ){
				subtool = 0;
				if( pn == SEQ_PN ) {
					if( INP_mode == _MODEFY || INP_mode == _KEYSCAN )
						subtool = 2;
				}
			}
			((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar( tooltype, subtool );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 現在位置に従ってツールバーを変更する[摘要欄] ('12.10.03)
//-----------------------------------------------------------------------------
// 引数		pn	：	該当ペイン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBDinpView::ChangeToolbarSubInTkypn( int pn )
{
	if( pn == TKY_PN ){
		if( IsDispSlip() == FALSE ){
			char	buf[256];
			::ZeroMemory( buf, sizeof buf );
			strcpy_s( (char*)buf, sizeof buf, getLINE_DATA()->_XXDTA.dd_tek.tekiyo );
			int pos = strlen( (char*)buf );
			((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar( 1, pos == 0 ? 1 : 0 );
		}
		else{
			char	buf[256];
			::ZeroMemory( buf, sizeof buf );
			strcpy_s( (char*)buf, sizeof buf, getLINE_DATA()->_XXDTA.dd_tek.tekiyo );
			int pos = strlen( (char*)buf );
			((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar( 7, pos == 0 ? 1 : 0 );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// イメージダイアログ表示切替 ('12.10.06)
//   ツールバー切替を必要としないバージョン
//-----------------------------------------------------------------------------
// 引数	pn			：	フォーカス領域
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CDBDinpView::ChangeImgDlgDisp( int pn )
{
	if( m_bSLIP_MASTER == FALSE ){
		return 0;
	}
	else{
		int dmytooltype = 0;
		return ChangeImgDlgDisp( pn, dmytooltype );
	}
}

//-----------------------------------------------------------------------------
// イメージダイアログ表示切替
//-----------------------------------------------------------------------------
// 引数	pn			：	フォーカス領域
//		tooltype	：	ツールバー切替コード
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CDBDinpView::ChangeImgDlgDisp( int pn, int &tooltype )
{
	// 初期化
	tooltype = -1;

	// 現行：仕訳データシーケンス
	int	setDtseq = getLINE_DATA()->_XXDTA.dd_seq;

//MyTrace( "┏━━━━━ ChangeImgDlgDisp TOP pn = %d, m_bDscroll = %d ━━━━━━┓\n", pn, m_bDscroll );
//MyTrace("---> [%d]m_ImgDispOption.unlinkDispInAdd = %d, IsDispSlip() %d\n", setDtseq, m_ImgDispOption.unlinkDispInAdd, IsDispSlip());
//MyTrace("---> m_ImgDispOption.prevImgSeq = %d, setDtseq %d\n", prevImgSeq, setDtseq );
//	if( m_ImgDispOption.unlinkDispInAdd == ID_HIDE_UNLINK_IN_DATAADD ) {
//		CArray<int> aryDseq;
//		m_pImgmng->GetRelImgAry(setDtseq, aryDseq);
//
//		if( INP_mode != _APPEND ) {
//			if( !(getLINE_DATA()->_XXDTA.dd_dsign & 0x800) ) {
////				MyTrace("(%d)---- 関連づいていない仕訳\n", setDtseq );
//			}
//			else {
////				MyTrace("(%d)---- 関連あり", setDtseq);
//			}
//		}
//	}

	if( prevImgSeq == setDtseq )	return 0;

#ifdef CLOSE //全原票を転記済みで、関連付けされていない仕訳行の場合は、フォーカスセット
//ここでフォーカスはセットしないようにする。
m_bDscroll = TRUE;
#endif

	if( IsDispSlip() ){

		if( INP_mode == _APPEND ){
			if( m_ImgDtseq != -1 ){
//--> '12.08.10 INS START
				if( m_pImgmng->GetImgDlgType() == 0 ){
					m_NowImgSeq = m_pImgmng->GetDocseqOnImgDlg();
				}
//<-- '12.08.10 INS END
#ifndef _SCANNER_SV_
				m_pImgmng->DispImgDlgOff();
#endif

				IMG_DLGOPTION	dlgOption;
				memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
//--> '12.08.13 INS START
				if( m_NowImgSeq != -1 ){
					dlgOption.imgDispSw |= 0x01;
					dlgOption.DispImgSeq = m_NowImgSeq;
				}
//<-- '12.08.13 INS END
/*- '12.09.22 -*/
//				dlgOption.btnDispSw |= (0x01|0x02|0x04|0x08|0x10|0x20);
/*-------------*/
				dlgOption.btnDispSw |= (0x01|0x02|0x04|0x08);
/*-- '12.10.03 --*/
//				if( (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_DO_LINKIMG) ){
/*---------------*/
				if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
/*---------------*/
					dlgOption.btnDispSw |= (0x10|0x20);
				}
/*-------------*/
			//	dlgOption.btnHideSw |= 0x01;
				m_pImgmng->DispImgDlgOn( this, dlgOption );
				m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
				m_DImgdlgType = ID_DBDINP_INPUTDLG_TYPE;
				ChgLinkBtnState( ID_LINKON_BSTATE );
				m_Imgcnt = 0;
				m_ImgDtseq = -1;

				// フォーカス再セット
				set_focus( get_nowpn() );
			}

			tooltype = 6;
		}
		else{
			if( getLINE_DATA()->_XXDTA.dd_dsign&0x800 ){
FILETrace( "   ------> ChangeImgDlgDisp m_ImgDtseq = %d, LINE = %d\n", m_ImgDtseq, __LINE__ );
				//if( ! IsImgDlgChanged( setDtseq ) ) {
				//	return 0;
				//}

				if( m_ImgDtseq != setDtseq ){
					m_SRECdseqAry.RemoveAll();
	//--> '12.08.10 INS START
					if( m_pImgmng->GetImgDlgType() == 0 ){
						m_NowImgSeq = m_pImgmng->GetDocseqOnImgDlg();
					}
	//<-- '12.08.10 INS END

/*- '12.09.24_MOVE -*/
//					m_pImgmng->DispImgDlgOff();
/*------------------*/

/*- '12.09.13 -*/
//					IMG_DLGOPTION	dlgOption;
//					memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
//					dlgOption.btnDispSw |= (0x04|0x08|0x10|0x20);
//
//					// 仕訳修正の部門欄/伝票番号欄にフォーカスがある時、イメージダイアログを表示すると
//					// Termination() 関数で部門/伝票番号欄が消えてしまう事への小細工。
//					int svEdit_pn = EDIT_PN;
//					if( (pn==BMON_PN) || (pn==DENP_PN) ){
//						EDIT_PN = -1;
//					}
//					m_pImgmng->DispImgDlgOn( pDBzm, setDtseq, this, dlgOption );
//					m_DImgdlgType = ID_DBDINP_CONFIRMDLG_TYPE;
//					EDIT_PN = svEdit_pn;
//
//					if( IsDispSlip() ){
//						ChgLinkBtnState( ID_LINKOFF_BSTATE );
//					}
//					else{
//						ChgLinkBtnState( ID_DISPSLIP_BSTATE );
//					}
/*-------------*/
/*- '12.09.29 -*/
//					if( m_ImgDispOption.dtypeInConfirm == ID_DISP_IN_CONFIRM ){
/*-------------*/
					if( m_DispSlipChk.GetCheck() ){
/*-------------*/
//--> '12.09.24 INS START
#ifndef _SCANNER_SV_
						m_pImgmng->DispImgDlgOff();
#endif
						IMG_DLGOPTION	dlgOption;
						memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
/*- '12.09.22 -*/
//						dlgOption.btnDispSw |= (0x04|0x08|0x10|0x20);
/*-------------*/
/*== '12.10.02 ==*/
//						dlgOption.btnDispSw |= (0x04|0x08);
/*===============*/
						dlgOption.btnDispSw |= (0x04|0x08|0x01|0x02);
/*===============*/
/*=== '12.10.03 ===*/
//						if( (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_DO_LINKIMG) ){
/*=================*/
						if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
/*=================*/
							dlgOption.btnDispSw |= (0x10|0x20);
						}
/*-------------*/
					//	dlgOption.btnHideSw |= 0x01;

						// 仕訳修正の部門欄/伝票番号欄にフォーカスがある時、イメージダイアログを表示すると
						// Termination() 関数で部門/伝票番号欄が消えてしまう事への小細工。
						int svEdit_pn = EDIT_PN;
						if( (pn==BMON_PN) || (pn==DENP_PN) ){
							EDIT_PN = -1;
						}
#ifdef _SCANNER_SV_
						dlgOption.imgDispSw |= 0x02;
						dlgOption.imgDispSw |= 0x04;	// '16.06.28
#endif
						dlgOption.imgDispSw |= 0x01;

						m_pImgmng->DispImgDlgOn( pDBzm, setDtseq, this, dlgOption );
						m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );

//--> '12.09.13 INS START
						m_ImgDtseq = setDtseq;
//<-- '12.09.13 INs END
						m_DImgdlgType = ID_DBDINP_CONFIRMDLG_TYPE;
						EDIT_PN = svEdit_pn;

						m_pImgmng->GetRelImgAry(setDtseq, m_SRECdseqAry);
//MyTrace( "m_SRECdseqAry(1)[seq=%d] = %d, IsDisp (%d)", setDtseq, m_SRECdseqAry.GetCount(), IsDispSlip() );

						if( IsDispSlip() ){
							ChgLinkBtnState( ID_LINKOFF_BSTATE );
						}
						else{
// '12.09.29 TEST_ HIDE
//							ChgLinkBtnState( ID_DISPSLIP_BSTATE );
						}
					}
					else{
/*- '12.09.24 -*/
//						ChgLinkBtnState( ID_DISPSLIP_BSTATE );
/*-------------*/
						HideImgDlg();
/*-------------*/
					}
/*-------------*/

//FILETrace( "   ------> ChangeImgDlgDisp LINE = %d\n", __LINE__ );

/*- '12.10.06:フォーカス乱移動カット -*/
//					// フォーカス再セット 
					if( ! m_bDscroll )	set_focus( get_nowpn() );
/*------------------------------------*/
				}
//--> '12.10.17 INS START
				else if( m_ImgDtseq == setDtseq ){
					if( m_DispSlipChk.GetCheck() ){
						if( IsDispSlip() ){
//							ChgLinkBtnState( ID_LINKOFF_BSTATE );
						}
					}
				}
//<-- '12.10.17 INS END
				m_ImgDtseq = setDtseq;

//FILETrace( "   ------> ChangeImgDlgDisp LINE = %d, m_ImgDtseq = %d\n", __LINE__ , m_ImgDtseq );

			}
			else{
/*- '12.10.01 -*/
//				if( m_DImgdlgType == ID_DBDINP_CONFIRMDLG_TYPE ){
//					HideImgDlg();
//				}
/*-------------*/
			//	if( ! IsImgDlgChanged( setDtseq ) ) {
			//		// 前回と同じ分が表示。
			//		return 0;
			//	}

				if( m_DispSlipChk.GetCheck() ){
//--> '12.10.03 INS START
					if( m_ImgDispOption.dtypeInCorrect == ID_HIDE_IN_CORRECT ){
						HideDispImgDlg();
					}
//<-- '12.10.03 INS END

/*- '12.10.03 -*/
//					if( m_ImgDtseq != -1 ){
/*-------------*/
					if( (m_ImgDtseq!=-1) && (m_ImgDispOption.dtypeInCorrect==ID_DISP_IN_CORRECT) ){
/*-------------*/
#ifndef _SCANNER_SV_
						m_pImgmng->DispImgDlgOff();
#endif

						IMG_DLGOPTION	dlgOption;
						memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );

						if( m_NowImgSeq != -1 ){
							dlgOption.imgDispSw |= 0x01;
							dlgOption.DispImgSeq = m_NowImgSeq;
						}
						dlgOption.btnDispSw |= (0x01|0x02|0x04|0x08);
/*- '12.10.03 -*/
//						if( (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_DO_LINKIMG) ){
/*-------------*/
						if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
/*-------------*/
							dlgOption.btnDispSw |= (0x10|0x20);
						}
#ifdef _SCANNER_SV_
						dlgOption.imgDispSw |= 0x02;
						dlgOption.imgDispSw |= 0x04;	// '16.06.28
#endif
					//	dlgOption.btnHideSw |= 0x01;

						// 仕訳修正の部門欄/伝票番号欄にフォーカスがある時、イメージダイアログを表示すると
						// Termination() 関数で部門/伝票番号欄が消えてしまう事への小細工。
						int svEdit_pn = EDIT_PN;
						if( (pn==BMON_PN) || (pn==DENP_PN) ){
							EDIT_PN = -1;
						}

						m_pImgmng->DispImgDlgOn( this, dlgOption );
						m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );

						m_DImgdlgType = ID_DBDINP_INPUTDLG_TYPE;
						ChgLinkBtnState( ID_LINKON_BSTATE );
						m_Imgcnt = 0;
						m_ImgDtseq = -1;

						EDIT_PN = svEdit_pn;

						// フォーカス再セット
						if( ! m_bDscroll )	set_focus( get_nowpn() );


					}
				}
/*-------------*/
			}

			tooltype = 0;
		}
	}
	else if( (INP_mode!=_APPEND) && (m_pImgmng!=NULL) ){

		m_SRECdseqAry.RemoveAll();
		m_pImgmng->GetRelImgAry(setDtseq, m_SRECdseqAry);

/*- '12.09.29 -*/
//		if( m_ImgDispOption.dtypeInConfirm == ID_DISP_IN_CONFIRM ){
/*-------------*/
		if( m_DispSlipChk.GetCheck() ){
/*-------------*/
			int	setDtseq = getLINE_DATA()->_XXDTA.dd_seq;
			if( m_ImgDtseq != setDtseq ){
				IMG_DLGOPTION	dlgOption;
				memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
/*- '12.09.22 -*/
//				dlgOption.btnDispSw |= (0x04|0x08|0x10|0x20);
/*-------------*/
/*== '12.10.02 ==*/
//				dlgOption.btnDispSw |= (0x04|0x08);
/*===============*/
				dlgOption.btnDispSw |= (0x04|0x08|0x01|0x02);
/*===============*/
/*=== '12.10.03 ===*/
//				if( (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_DO_LINKIMG) ){
/*=================*/
				if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
/*=================*/
					dlgOption.btnDispSw |= (0x10|0x20);
				}
/*-------------*/
#ifdef _SCANNER_SV_
				dlgOption.imgDispSw |= 0x02;
#endif
			//	dlgOption.btnHideSw |= 0x01;

				// 仕訳修正の部門欄/伝票番号欄にフォーカスがある時、イメージダイアログを表示すると
				// Termination() 関数で部門/伝票番号欄が消えてしまう事への小細工。
				int svEdit_pn = EDIT_PN;
				if( (pn==BMON_PN) || (pn==DENP_PN) ){
					EDIT_PN = -1;
				}
				m_pImgmng->DispImgDlgOn( pDBzm, setDtseq, this, dlgOption );
				m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
//--> '12.09.13 INS START
				m_ImgDtseq = setDtseq;
//<-- '12.09.13 INs END
				m_DImgdlgType = ID_DBDINP_CONFIRMDLG_TYPE;
				EDIT_PN = svEdit_pn;

				if( IsDispSlip() ){
					ChgLinkBtnState( ID_LINKOFF_BSTATE );
				}
				else{
// '12.09.29 TEST_ HIDE
//					ChgLinkBtnState( ID_DISPSLIP_BSTATE );
				}
				// フォーカス再セット
				if( ! m_bDscroll )	set_focus( get_nowpn() );
			}
/*- '12.09.13_CUT -*/
//			m_ImgDtseq = getLINE_DATA()->_XXDTA.dd_seq;
/*-----------------*/
		}

		tooltype = 0;
	}
	else{
		tooltype = 0;
		//if( INP_mode == _APPEND ) {
		//	set_focus(get_nowpn());
		//}
	}

	if( m_bDscroll )	m_bDscroll = FALSE;

//FILETrace( "┗━━━━━ ChangeImgDlgDisp END ━━━━━━┛\n" );
	prevImgSeq = setDtseq;

	return 0;
}
#endif



// 現在ポジションゲット
int CDBDinpView::get_nowpn()
{
	return( _get_nowpn( get_nowln() ) );
}

int CDBDinpView::_get_nowpn( int ln )
{
	int line = get_nowln();

	return( (ln == APPEND_LINE) ? NOW_PN : DDATA_NOW_PN[line-1] );
}


void CDBDinpView::AutoInp( int pn, BOOL bNXT /*=FALSE*/ )
{
	int i;
	BOOL job_sw = FALSE;
	int ln = get_nowln();
	int	type;
	type =  IsKojinGyosyuMaster( pDBzm );

	if( (INP_mode == _APPEND) && BOTOM_SEQ )
	{
		m_AutoInp_use = TRUE;

#ifdef REGDATA	// 04.12 /01
		BOOL rdtamode = m_RDTATBL.RD_cnt > 0 ? TRUE : FALSE;	// 定型仕訳処理中 check
#endif

		int nPos;

		for( i = 0; m_AutoInp_use && gPNtbl[i] != pn && gPNtbl[i] != -1; ++i )
		{

			nPos = gPNtbl[i];
			if( _getLINE_DATA( ln )->LINE_CND[nPos].INP_sg == FALSE )
			{
				job_sw = TRUE;

				DBeditCtrlSet( getINP_CTL( DATE_PN )->IDC_X, nPos );

				switch( nPos )
				{
				case SEQ_PN:	//番号
					break;
				case DATE_PN:	//日付
					TerminationIcsinputDate( this, VK_RETURN, 0, 0 );
					TerminationIcsinputDate( this, 0, 0, 0);
					break;
				case DENP_PN:	//伝票番号
					TerminationIcsinputDenp( this, ln, VK_RETURN, 0, 0 ); 
					TerminationIcsinputDenp( this, ln, 0, 0, 0 ); 
					break;
				case BMON_PN:	//借方部門
					if( BMON_MST )
					{
						if( Rdta_autocheck(nPos) ) {
							TerminationIcsinputBumon( this, 0, ln, VK_RETURN, 0, 0 );
							TerminationIcsinputBumon( this, 0, ln, 0, 0, 0 );
						}
					}
					break;
				case CBMN_PN:	//貸方部門
					if( BMON_MST )
					{
						if( Rdta_autocheck(nPos) ) {
							TerminationIcsinputBumon( this, 1, ln, VK_RETURN, 0, 0 );
							TerminationIcsinputBumon( this, 1, ln, 0, 0, 0 );
						}
					}
					break;
				case DEBT_PN:	//借方
					TerminationIcsinputDebt( this, ln, VK_RETURN, 0, 0 );
					TerminationIcsinputDebt( this, ln, 0, 0, 0 );

					// 現在の業種でない科目の 空うちEnter での 入力をチェックする。
					if( type ) {
						if( IsKojinKengyoMaster( type ) ) {
							// ループ終了
							if( _getLINE_DATA(ln)->_XXDTA.dd_dbt.IsEmpty() ) {
								pn = DEBT_PN;
								i = pn+1;
								break;
							}
						}
					}
					break;
				case CRED_PN:	//貸方
					TerminationIcsinputCred( this, ln, VK_RETURN, 0, 0 );
					TerminationIcsinputCred( this, ln, 0, 0, 0 );

					// 現在の業種でない科目の 空うちEnter での 入力をチェックする。
					if( type ) {
						if( IsKojinKengyoMaster( type ) ) {
							// ループ終了
							if( _getLINE_DATA(ln)->_XXDTA.dd_cre.IsEmpty() ) {
								pn = CRED_PN;
								i = pn+1;
								break;
							}
						}
					}
					break;
				case KOJI_PN:	//借方工事
					if( M_KOJI )
					{
						if( Rdta_autocheck(nPos) ) {
							TerminationIcsinputKoji( this, 0, ln, VK_RETURN, 0, 0 );
							TerminationIcsinputKoji( this, 0, ln, 0, 0, 0 );
						}
					}
					break;
				case CKJI_PN:	//貸方工事
					if( M_KOJI )
					{
						if( Rdta_autocheck(nPos) ) {
							TerminationIcsinputKoji( this, 1, ln, VK_RETURN, 0, 0 );
							TerminationIcsinputKoji( this, 1, ln, 0, 0, 0 );
						}
					}
					break;
				case VAL_PN:	//金額
					TerminationIcsinputVal(this, ln, VK_RETURN, 0, 0);
					TerminationIcsinputVal( this, ln, 0, 0, 0 );
					break;
				case SKBN_PN:	//消費税
					break;
				case TKY_PN:	//摘要
					TerminationIcsinputTky( this, ln, VK_RETURN, 0, 0 );
					TerminationIcsinputTky( this, ln, 0, 0, 0 );
					break;

				case SYON_PN:	//証憑番号 -> 手形番号
					TerminationIcsinputSyono( this, ln, VK_RETURN, 0, 0 );
					TerminationIcsinputSyono( this, ln, 0, 0, 0 );
					break;
				case TGDT_PN:	//手形期日
					TerminationIcsinputTgDate( this, ln, VK_RETURN, 0, 0 );
					TerminationIcsinputTgDate( this, ln, 0, 0, 0 );
					break;
				case DOCEVI_PN:	//証憑番号
					TerminationIcsinputDocEviNo( this, ln, VK_RETURN, 0, 0 );
					TerminationIcsinputDocEviNo( this, ln, 0, 0, 0 );
					break;
				default:
					break;
				}
			}
		}
		if( job_sw )
		{
			PROGRAM_OK = FALSE;
			c_set( IDC_ICSDISPCTRL10 );	// 入力モード表示にダミーフォーカス
			inpline_dsp( /*&INP_CTL[0],*/ &INP_LINE, APPEND_LINE );
			DBeditCtrlSet( getINP_CTL( DATE_PN )->IDC_X, pn );
			_set_focus( ln, pn );
			PROGRAM_OK = TRUE;
		}
		m_AutoInp_use = FALSE;
	}
	else {
		if( INP_mode == _APPEND ) {
			for( i = 0 ; gPNtbl[i] != pn ; ++i )
			{
				if( _getLINE_DATA( ln )->LINE_CND[i].INP_sg == FALSE )
				{
					switch( gPNtbl[i] )
					{
					case SEQ_PN:	//番号
						break;
					case DATE_PN:	//日付
						break;
					case DENP_PN:	//伝票番号
						// 仕訳が　0 のとき、カーソルが伝票番号に合わないのを防ぐ
						_getLINE_DATA( ln )->LINE_CND[i].INP_sg = TRUE;
						break;
					}
				}
			}
		}
	}
}


//科目・摘要選択画面リセット
void CDBDinpView::SelDspRst( int inp_mode )
{
	int pn;
	pn = get_nowpn();

	SelPar par;

	switch( pn ) {
	case BMON_PN: case CBMN_PN:
		par.sel_pn = SL_BUMON_PN;
		break;
	case KOJI_PN: case CKJI_PN:
		par.sel_pn = SL_KOJI_PN;
		break;
	case TKY_PN:
		par.sel_pn = SL_RESET;
		break;
	default:
		par.sel_pn = SL_RESET;
		break;
	}

	m_Seldata.SelectJob( &par, -1 );
	
}

// 部門工事 選択画面更新
void CDBDinpView::BmnKjSelDispRst()
{
	int pn;
	pn = get_nowpn();

	SelPar par;

	switch( pn ) {
	case BMON_PN: case CBMN_PN:
	case KOJI_PN: case CKJI_PN:
		par.sel_pn = SL_RESET;
		break;
	}

	m_Seldata.SelectJob( &par, -1 );
}


//科目・摘要選択パラメータイニシャル
void CDBDinpView::SelDspParaRst()
{
	//連想摘要
//	RTKct[0] = RTKct[1] = -1;
//	RTblk[0] = RTblk[1] = -1;
}

// 枝番摘要画面リセット
void CDBDinpView::brntky_rst()
{
	if( BRNTKY_sel )
	{
		if( !brntky_page( &TKYBRN_SEL, SelVal32, &m_selval32, TKYBRN_SEL.BRN_PAGE ) )
		{
//			BRNTKY_sel_set( brntky_page( &TKYBRN_SEL, SelVal32, &m_selval32, 0 ));
		}
	}
}

// 枝番未入力チェックＪＯＢ
BOOL CDBDinpView::isNoBRNInpChk()
{

	if( !FNC_BUTTON )
	{
		int st;

		if( !dinp_modefy_ok( getLINE_DATA() ) )
			return FALSE;

		if( IsSCAN_TKYONLY() ) return FALSE;

		PROGRAM_OK = FALSE;
		//
		// 借方 枝番未入力チェック
		if( (m_NOBRNCHK_CND & 0x11) != 0x11 /*ln == APPEND_LINE*/ /*&& !IN_BRNTKY*/  )
		{

			st = inp_brchk( CREC.m_dbt, CREC.m_dbr, pAUTOSEL->BRN_OPT, &m_NOBRNCHK_CND, 0);

			switch( st )
			{
			case 0:
				break;
			default:
//				_getLINE_DATA(ln)->LINE_CND[DEBT_PN].NEXT_MOVE_KEY = FALSE;
//				m_AutoInp_use = FALSE;
				((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(0);

				BRNCHK_PN = DEBT_PN;
				PostMessage( WM_RETROFOCUS, DEBT_PN );
//				set_focus( DEBT_PN );

				set_nowpn( DEBT_PN );
				BRNTKY_select( CREC.m_dbmn, CREC.m_dbt, NULL, TRUE );

				PROGRAM_OK = TRUE;

				return TRUE;
				break;
			}
		}
		//
		// 貸方 枝番未入力チェック
		if( (m_NOBRNCHK_CND & 0x12) != 0x12 /*ln == APPEND_LINE*/ /*&& !IN_BRNTKY*/ )
		{

			st = inp_brchk( CREC.m_cre, CREC.m_cbr, pAUTOSEL->BRN_OPT, &m_NOBRNCHK_CND, 1);

			switch( st )
			{
			case 0:
				break;
			default:
//				_getLINE_DATA(ln)->LINE_CND[CRED_PN].NEXT_MOVE_KEY = FALSE;
//				m_AutoInp_use = FALSE;
				((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(0);

				BRNCHK_PN = CRED_PN;
				PostMessage( WM_RETROFOCUS, CRED_PN );
//				set_focus( CRED_PN );

				set_nowpn( CRED_PN );
				BRNTKY_select( CREC.m_cbmn, CREC.m_cre, NULL, TRUE );

				PROGRAM_OK = TRUE;

				return TRUE;
				break;
			}
		}

	}
	
	PROGRAM_OK = TRUE;

	return FALSE;
}


BOOL CDBDinpView::tab_copy( CWnd* pwnd, int idc, char *tks )
{
	int max = get_inpl( (unsigned char*)tks, Voln1->tk_ln );	// コピー元適要欄の文字ﾊﾞｲﾄ数
	int maxbyte = (Voln1->tk_ln * 2);
	int now, pos, btpos, cnt, i, st;
	char txt[50] = {0};

	// 現在の文字ﾊﾞｲﾄ数
	VARIANT var;
	((CICSDBEDT*)pwnd->GetDlgItem( idc ))->GetData( &var, ICSDBEDT_TYPE_STRING, 0);
	now = strlen( (LPCTSTR)var.pbVal );

	// ﾊﾞｲﾄ単位のｶｰｿﾙ位置
	btpos = ((CICSDBEDT*)pwnd->GetDlgItem( idc ))->GetCaretPosition();
	pos = btpos;

	//  02.09 /02 -ANKINP- 
	if( now == btpos && btpos < maxbyte )
	{
		for( i = 0, cnt = 0; i < max;  ) {
			st = IsKindOfLetter(tks, i);
			if( cnt == pos ) {
				if( st == 1 ) {			// ASCII
					txt[0] = *(tks + i);	
				}
				else if( st == 2 ) {	// 漢字
					if( (btpos+2) <= maxbyte )	memcpy(txt, (tks+i), 2 );
					else	return FALSE;
				}
				else {
					return FALSE;	// それ以外
				}				
				break;
			}
			else {
				if( st == 1 ) {
					cnt++;	i++;
				}
				else if( st == 2 ) {
					cnt += 2;	i += 2;
				}
			}
		}
	//	sprintf( txt, "%.2s", tks + pos*2 );		// コピー元摘要１文字（漢字）取り出し
	//	tek_insert( idc, (LPCTSTR)txt, CICSInput );		// 現在の適要欄のキャレット位置に挿入

		var.pbVal = (BYTE*)txt;
		((CICSDBEDT*)pwnd->GetDlgItem( idc ))->InsertData( &var, ICSDBEDT_TYPE_STRING, 0, btpos );
		
	}
	else if( btpos >= now )
	{
		return FALSE;
	}
	else {
		st = IsKindOfLetter(tks, btpos);
		int mv = 0;
		if( st == 1 )		mv = 1;
		else if( st == 2 )	mv = 2;
		// ポジションを１文字分進める
		((CICSDBEDT*)pwnd->GetDlgItem( idc ))->SetCaretPosition( btpos+mv );		
	}

	return TRUE;
}


//  仕訳摘要エリア取得
//
int CDBDinpView::swktky_alloc()
{
	if(( m_ASTKCX = new WORD[Voln1->tk_max] ) == NULL ) {
		ermset( ERROR_MEMORY, "自動仕訳摘要エリアが作成できませんでした。");
		return -1;
	}

	return 0;
}

// 仕訳摘要エリアクリア
//
void CDBDinpView::swktky_clear()
{
	if( m_ASTKCX ) {
		::ZeroMemory( m_ASTKCX, sizeof(WORD)*Voln1->tk_max );
	}
}

// 科目対応摘要 TKDsw の ｻｲﾝ 取得
int CDBDinpView::get_rentk_sw()
{
//	if( GET_MASTERTYPE() == DRIVE_FD )
//		return 1;

	return ((pAUTOSEL->SWKTEK_OPT == OPT_SWKTEK) ? 2 : 1);
}


// 仕訳対応摘要 TKDsw の ｻｲﾝ 取得
int CDBDinpView::get_swktky_sw()
{
//	if( GET_MASTERTYPE() == DRIVE_FD )
//		return 2;

	return ((pAUTOSEL->SWKTEK_OPT == OPT_SWKTEK) ? 1 : 2);
}


#ifdef CUT_CLOSE

/*================================
	12.09 /02
	自動仕訳ファイルより摘要表示
==================================*/
int CDBDinpView::tky_atswk_dsp( CString deb, CString cre, BOOL bDISP/*=TRUE*/ )
{
	int cnt;

	{
		swktky_clear();
		
		if( GET_MASTERTYPE() != DRIVE_FD )
			cnt = tkat_srch( 0x03, deb, cre, m_ASTKCX );
		else
			cnt = 0;

		SW_AutoTK = cnt;	
	//	if( cnt )
	//	{
			if( bDISP ) {
				if( make_autotky() ) {	// <- use to SW_AutoTK
				//!	m_selval32.SetData( (long)SelVal32 );
					SetICSSel32( (long)SelVal32 );
				}
			}
	//	}
		return 0;
	}
	return -1;
}

/*====================================================================
	12.09 /02
	自動仕訳摘要表示テーブル作成
	
	BOOL bLastDisp .... TRUEのとき 最終データを表示
						FALSE のとき 1ページ目より
  ====================================================================*/
BOOL CDBDinpView::make_autotky( BOOL bLastDisp /*= FALSE*/ )
{
	if( GET_MASTERTYPE() == DRIVE_FD )
		return FALSE;

//	if( SW_AutoTK ) {
		TKDsw = SWKTKNO; 
		RTKdsw = 0;
		TATpage = 1;
		TATpmax = SW_AutoTK / AC_MAX;
		if( SW_AutoTK % AC_MAX ) TATpmax++;

		if( bLastDisp ) {
			int data_pos = 0;

			// 最終ページを表示
			if( TATpmax > 0 ) {
				TATpage = TATpmax;
				TATdcnt = SW_AutoTK - ( AC_MAX * (TATpage - 1) );

				data_pos = ( AC_MAX*(TATpage-1) );
			}
			else
				TATdcnt = 0;

			tkauto_dsp( TATpage, TATdcnt );
		}
		else {
			if( TATpmax > 0 )
				TATdcnt = ( TATpmax == 1 ) ? SW_AutoTK : AC_MAX;
			else
				TATdcnt = 0;
	
			tkauto_dsp( TATpage, TATdcnt );
		}

		return TRUE;
//	}
//	return FALSE
;
}

#endif


// 欄外イメージ摘要表示
#ifdef BLK_CLOSE
void CDBDinpView::big_img( struct _DATA_LINE *dtline, struct _InpCtl *ctl )
{
	CWnd *tkyorg;
	CWnd *btmorg;
	CWnd *bigimg;
	WINDOWPLACEMENT btmplace,tkyplace;
	double rate_w, rate_h;
	RECT newrect,tkyrect;
	long ofs;
#ifdef BIGIMG	// 03.16 /00
	WINDOWPLACEMENT dmyplace;
	CWnd *dmywnd;
	RECT limit = {0};
#endif

	VERIFY(bigimg = GetDlgItem( IDC_DISPBIGIMG ));

	if( dtline->LINE_CND[ TKY_PN ].IMG_sg )
	{
		get_viewrate( rate_w, rate_h );		//ビューサイズ比率をゲット

		((CICSDisp*)bigimg)->ClearDisp();	// 表示エリアクリア
		((CICSDisp*)bigimg)->SetBackColor( SELECT_BACK_COL );

		//========================
		// イメージ摘要欄を欄外に
		//========================

		//
		// 「高さ」と「表示開始位置」を最下行の「金額」欄から得る
		//

		// 金額欄の位置情報から算出
//		VERIFY(btmorg = GetDlgItem( _getINP_CTL( SCROLL_NL_, VAL_PN )->IDC_X ));
		VERIFY(btmorg = GetDlgItem(IDC_ICSDISPSYOGT));
		btmorg->GetWindowPlacement( &btmplace );
		newrect = btmplace.rcNormalPosition;
		int ht = newrect.bottom - newrect.top;
		VERIFY(btmorg = GetDlgItem( IDC_DTLINE ));
		btmorg->GetWindowPlacement( &btmplace );
		newrect.top = btmplace.rcNormalPosition.bottom;

//		newrect.bottom = newrect.top + ht + (ht / 3);
		// 04.04 /12
		newrect.top -= 2;
		newrect.bottom = newrect.top + ht + (ht / 6);
		ofs = (newrect.bottom - newrect.top);

		if( dtline->ImgSEQ != 0 )
		{
			// イメージデータあり
		//	ofs = (newrect.bottom - newrect.top)*17/10;					// 高さ ＝ 金額欄の高さ×1.7
		//	newrect.top = newrect.bottom;								// 頂上 ＝ 金額欄の底
		//	newrect.bottom = newrect.top + ofs;							// 底   ＝ 金額欄の底＋高さ
			newrect.left = newrect.right + (rate_w * 40);				// 左   ＝ 金額欄の左－（幅拡大率×40）
		}
		else
		{
			// イメージデータなし“≪イメージ≫”
		//	ofs = (newrect.bottom - newrect.top);						// 高さ ＝ 金額欄の高さ
		//	newrect.top = newrect.bottom;								// 頂上 ＝ 金額欄の底
		//	newrect.bottom = newrect.top + ofs;							// 底   ＝ 金額欄の底＋高さ
			newrect.left = newrect.right + (rate_w * 40);				// 左   ＝ 金額欄の左－（幅拡大率×40）
			newrect.right = newrect.left + (BIGIMG_RECT.right*rate_w);	// 右   ＝ 左＋（元のイメージ表示幅×幅拡大率）
		}

		//
		// 最下行の「摘要表示欄」の下に「イメージ表示欄」置く
		//
		VERIFY( tkyorg = GetDlgItem( ctl->IDC_X ) );
		tkyorg->GetWindowPlacement( &tkyplace );
		tkyrect = tkyplace.rcNormalPosition;	// 摘要位置情報
		if( tkyrect.bottom < newrect.top )
		{
			;
		}
		else
		{
#ifdef _ICSPEN_	// 01.20 /01
			if( flgPen )
			{
				;
			}
			else
			{
				newrect.top = tkyrect.bottom+1;
				newrect.bottom = newrect.top + ofs;
			}
#else
			newrect.top = tkyrect.bottom+1;
			newrect.bottom = newrect.top + ofs;
#endif
		}

#ifdef _ICSPEN_	// 01.20 /01
		bigimg->SetWindowPos( tkyorg, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE );
#else
		bigimg->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE );
#endif

#ifdef BIGIMG	// 03.16 /00

		//
		// 制限幅を算出
		//

		// 左端
//		VERIFY(dmywnd = GetDlgItem( IDC_ICSDISPCTRL7 ));	// 番号 タイトル
		VERIFY(dmywnd = GetDlgItem( IDC_DTLIST ));	// 番号 タイトル
		dmywnd->GetWindowPlacement( &dmyplace );
		limit.left = dmyplace.rcNormalPosition.left;

		// 右端
//		VERIFY(dmywnd = GetDlgItem( IDC_ICSDISPCTRL8 ));	// 摘要 タイトル
		dmywnd->GetWindowPlacement( &dmyplace );
		limit.right = dmyplace.rcNormalPosition.right;

		// 幅制限付きの表示
# ifdef _ICSPEN_	// 01.20 /01
		dsp_img( bigimg, dtline, &newrect, TRUE, &limit, &tkyrect );
# else
		dsp_img( bigimg, dtline, &newrect, TRUE, &limit );
# endif

#else
		dsp_img( bigimg, dtline, &newrect, TRUE );
#endif
	}
	else
	{
		((CICSDisp*)bigimg)->ClearDisp();	// 表示エリアクリア
		((CICSDisp*)bigimg)->SetBackColor(m_vwColor);
		// 欄外イメージを最小に（見えなくする）。
		bigimg->SetWindowPos( 0, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER );
		bigimg->UpdateWindow();
	}
}

#ifdef _ICSPEN_	// 01.20 /01
//
// イメージ位置調整
void imgposadj( RECT* rect, RECT* tkyrect )
{
	if( tkyrect )
	{
		if( rect->top >= tkyrect->bottom-2/* && rect->bottom > tkyrect->bottom*/ )
			;
		else
		{
			long n = rect->right - tkyrect->left;
			rect->left -= n;
			rect->right -= n;
		}
	}
}
#endif

#ifdef BIGIMG	// 03.16 /00
# ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::dsp_img( CWnd *imgorg, struct _DATA_LINE *dtline, RECT* newrect, BOOL real, RECT* lmt_width, RECT* tkyrect )
# else
void CDBDinpView::dsp_img( CWnd *imgorg, struct _DATA_LINE *dtline, RECT* newrect, BOOL real, RECT* lmt_width )
# endif
#else
void CDBDinpView::dsp_img( CWnd *imgorg, struct _DATA_LINE *dtline, RECT* newrect, BOOL real )
#endif
{
	int imgseq = dtline->ImgSEQ;
	IMREC *pIMREC;
	RECT rect;
	WINDOWPLACEMENT orgplace;
	#define xpary (pIMREC->dotys ? pIMREC->dotxs / pIMREC->dotys : 1)

	if( !newrect )
	{
		imgorg->GetWindowPlacement( &orgplace );
		rect = orgplace.rcNormalPosition;
	}
	else
		rect = *newrect;

	// イメージファイル
//	CString filter;
//	filter.Format( "iseq = '%d'", imgseq );

//	if( pDBzm->image->Requery( filter ) == 0 )
	if( pDBzm->image->SetAbsolutePosition( imgseq ) == 0 )
	{
		pIMREC = pDBzm->image->DecodeImage();
		if( pIMREC )
		{
			if( real )
			{
				rect.right = rect.left + (rect.bottom - rect.top)*xpary;
#ifdef BIGIMG	// 03.16 /00
				// 範囲制限
				if( lmt_width )
				{
					if( rect.right > lmt_width->right )
					{
						long ofs = rect.right - lmt_width->right;

						if( rect.left - ofs < lmt_width->left )
						{
							ofs = rect.right - rect.left;

							rect.left = lmt_width->left;
							rect.right = rect.left + ofs; 
						}
						else
						{
							rect.right -= ofs;
							rect.left -= ofs;
						}
					}
				}
# ifdef _ICSPEN_	// 01.20 /01
				imgposadj( &rect, tkyrect );
# endif
				imgorg->SetWindowPos( 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER );
				if( dtline->LINE_CND[ TKY_PN ].IMG_sg == 1 )
					((CICSDisp*)imgorg)->ImgDisp( 0, 0, pIMREC->dotxs, pIMREC->dotys, (LPUNKNOWN)pIMREC->imdata, 1 );
				return;
#endif
			}
#ifdef BIGIMG	// 03.16 /00
			else
			{
			//	IMREC dmy;
			//	unsigned char *p;
			//	p = img_trim( &dmy, pIMREC, rect.right - rect.left, rect.bottom - rect.top );

# ifdef _ICSPEN_	// 01.20 /01
				imgposadj( &rect, tkyrect );
# endif
				imgorg->SetWindowPos( 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER );
				if( dtline->LINE_CND[ TKY_PN ].IMG_sg == 1 )
					((CICSDisp*)imgorg)->ImgDisp( 0, 0, pIMREC->dotxs, pIMREC->dotys, (LPUNKNOWN)pIMREC->imdata, 1 );
				return;

			}
#else
			imgorg->SetWindowPos( 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER );
			if( dtline->LINE_CND[ TKY_PN ].IMG_sg == 1 )
				((CICSDisp*)imgorg)->ImgDisp( 0, 0, pIMREC->dotxs, pIMREC->dotys, (LPUNKNOWN)pIMREC->imdata, 1 );
			return;
#endif
		}	
	}

#ifdef _ICSPEN_	// 01.20 /01
	imgposadj( &rect, tkyrect );
#endif
	imgorg->SetWindowPos( 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER );
	if( dtline->LINE_CND[ TKY_PN ].IMG_sg == 1 )
		((CICSDisp*)imgorg)->ColmDisp( 0, Image );
	else	// == 2
		((CICSDisp*)imgorg)->ColmDisp( 0, "" );
}

#else

void CDBDinpView::big_img( struct _DATA_LINE *dtline, struct _InpCtl *ctl )
{
	CWnd *bigimg;
	double rate_w, rate_h;
	RECT newrect;
#ifdef BIGIMG	// 03.16 /00
	WINDOWPLACEMENT dmyplace;
	CWnd *dmywnd;
	RECT limit = {0};
#endif

	VERIFY(bigimg = GetDlgItem( IDC_DISPBIGIMG ));

	BOOL bBigImg = FALSE;
	if( IMG_master != ERR && get_nowln() != -1 &&
		dtline->LINE_CND[ TKY_PN ].IMG_sg ) {
		bBigImg = TRUE;
	}

//	if( dtline->LINE_CND[ TKY_PN ].IMG_sg )
	if( bBigImg )
	{
		get_viewrate( rate_w, rate_h );		//ビューサイズ比率をゲット

		((CICSDisp*)bigimg)->ClearDisp();	// 表示エリアクリア
		((CICSDisp*)bigimg)->SetBackColor( SELECT_BACK_COL );

		//========================
		// イメージ摘要欄を欄外に
		//========================

		//
		// 「高さ」と「表示開始位置」を最下行の「金額」欄から得る
		//
		CRect	baseRect;

		// 追加入力用に画面変更
		BOOL bSgn = FALSE;
		if( IsMAsterKAKUTEI() )	bSgn = TRUE;
		else {
			if( INP_mode == _KEYSCAN )	bSgn = TRUE;
		}

		BOOL bInpGnkinDevide = IsBlkInpGnkinDevide();

		if( bInpGnkinDevide && bSgn ) {
			GetDlgItem( IDC_ICSDISPCTRL10 )->GetWindowRect( &baseRect );
		}
		else {
			GetDlgItem( IDC_ICSDISPSYOGT )->GetWindowRect( &baseRect );
		}

		ScreenToClient( baseRect );

		int ht = baseRect.Height();

		if( bSgn ) {
			newrect.left = baseRect.left;
			newrect.top  = baseRect.bottom;
		}
		else {
			newrect.left = baseRect.right + 2;
			newrect.top  = baseRect.top;
		}

		newrect.bottom = newrect.top + ht + (ht /8);
//		newrect.right = newrect.left + (BIGIMG_RECT.right*rate_w);
		newrect.right = newrect.left + baseRect.Width();

		//
		// 制限幅を算出
		//
		// 左端
		VERIFY(dmywnd = GetDlgItem( IDC_DTLIST ));	// 番号 タイトル
		dmywnd->GetWindowPlacement( &dmyplace );
		limit.left = dmyplace.rcNormalPosition.left;

		// 右端
		dmywnd->GetWindowPlacement( &dmyplace );
		limit.right = dmyplace.rcNormalPosition.right;

//MyTrace( "big_img newrect %d,%d,%d,%d", newrect.left, newrect.top, newrect.right, newrect.bottom );

		// 幅制限付きの表示
		dsp_img( bigimg, dtline, &newrect, TRUE, &limit );
	}
	else
	{
		((CICSDisp*)bigimg)->ClearDisp();	// 表示エリアクリア
		((CICSDisp*)bigimg)->SetBackColor(m_vwColor);
		// 欄外イメージを最小に（見えなくする）。
		bigimg->SetWindowPos( 0, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER );
		bigimg->UpdateWindow();

//MyTrace( "big_img 欄外イメージを最小-----" );
	}

//MyTrace( "big_img END" );
}


void CDBDinpView::dsp_img( CWnd *imgorg, struct _DATA_LINE *dtline, RECT* newrect, BOOL real, RECT* lmt_width )
{
	int imgseq = dtline->ImgSEQ;
	IMREC *pIMREC;
	RECT rect;
	WINDOWPLACEMENT orgplace;
	#define xpary (pIMREC->dotys ? pIMREC->dotxs / pIMREC->dotys : 1)

	if( !newrect )
	{
		imgorg->GetWindowPlacement( &orgplace );
		rect = orgplace.rcNormalPosition;
	}
	else
		rect = *newrect;

	// イメージファイル
//	CString filter;
//	filter.Format( "iseq = '%d'", imgseq );

//	if( pDBzm->image->Requery( filter ) == 0 )
	if( pDBzm != NULL && pDBzm->image->SetAbsolutePosition( imgseq ) == 0 )
	{
		pIMREC = pDBzm->image->DecodeImage();
		if( pIMREC )
		{
			if( real )
			{
				rect.right = rect.left + (rect.bottom - rect.top)*xpary;
#ifdef BIGIMG	// 03.16 /00
				// 範囲制限
				if( lmt_width )
				{
					if( rect.right > lmt_width->right )
					{
						long ofs = rect.right - lmt_width->right;

						if( rect.left - ofs < lmt_width->left )
						{
							ofs = rect.right - rect.left;

							rect.left = lmt_width->left;
							rect.right = rect.left + ofs; 
						}
						else
						{
							rect.right -= ofs;
							rect.left -= ofs;
						}
					}
				}
				imgorg->SetWindowPos( 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER );
				if( dtline->LINE_CND[ TKY_PN ].IMG_sg == 1 )
					((CICSDisp*)imgorg)->ImgDisp( 0, 0, pIMREC->dotxs, pIMREC->dotys, (LPUNKNOWN)pIMREC->imdata, 1 );
				return;
#endif
			}
#ifdef BIGIMG	// 03.16 /00
			else
			{
			//	IMREC dmy;
			//	unsigned char *p;
			//	p = img_trim( &dmy, pIMREC, rect.right - rect.left, rect.bottom - rect.top );

				imgorg->SetWindowPos( 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER );
				if( dtline->LINE_CND[ TKY_PN ].IMG_sg == 1 )
					((CICSDisp*)imgorg)->ImgDisp( 0, 0, pIMREC->dotxs, pIMREC->dotys, (LPUNKNOWN)pIMREC->imdata, 1 );
				return;

			}
#else
			imgorg->SetWindowPos( 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER );
			if( dtline->LINE_CND[ TKY_PN ].IMG_sg == 1 )
				((CICSDisp*)imgorg)->ImgDisp( 0, 0, pIMREC->dotxs, pIMREC->dotys, (LPUNKNOWN)pIMREC->imdata, 1 );
			return;
#endif
		}	
	}

	imgorg->SetWindowPos( 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER );
	if( dtline->LINE_CND[ TKY_PN ].IMG_sg == 1 )
		((CICSDisp*)imgorg)->ColmDisp( 0, Image );
	else	// == 2
		((CICSDisp*)imgorg)->ColmDisp( 0, "" );
}

#endif


// 仕訳フル時のメッセージ
char *CDBDinpView::get_Dfullmsg()
{
	char dev[MAX_PATH] = {0}, media[MAX_PATH] = {0}, capa[MAX_PATH] = {0};

	if( pDBzm->m_dacnt >= pDBzm->m_damax )
	{
		// システム上のMax
		return MFULL3;
	}
	else
	{
		PCOM_GetString( "MasterDevice", dev, media, capa );
		if( !_stricmp( media, "FD" ) )
			return MFULL;
		else
			return MFULL2;
	}

}

void CDBDinpView::OnUpdateButtonF8(CCmdUI* pCmdUI) 
{
	if( !PROGRAM_OK )
	{
		pCmdUI->Enable( FALSE );
		return;
	}

#ifndef _SLIP_ACCOUNTS_
	// データ数
	pCmdUI->Enable( pDBzm->m_dacnt ? TRUE : FALSE );
#else
	if( IsDispSlip() == FALSE ){
		// データ数
		pCmdUI->Enable( pDBzm->m_dacnt ? TRUE : FALSE );
	}
	else{
		pCmdUI->Enable( TRUE );
	}
#endif
}

#ifndef _SLIP_ACCOUNTS_
void CDBDinpView::OnButtonF8() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	try
	{
		FNC_BUTTON = TRUE;
		{
			switch( INP_mode )
			{
			case _KEYSCAN:
			case _APPEND:
			case _MODEFY:
				// データセットのため	04.09 /99
				if( get_nowpn() == TKY_PN )
					TerminationIcsinputTky( this, get_nowln(), 0, 0, 0 );

				//検索項目入力
				gotoscankey( /*TRUE*/ );
				break;
			case _RETREV:
				// ここには もうこないはず
				ASSERT( FALSE );

				break;
			}
		}
		FNC_BUTTON = FALSE;
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}
#else
void CDBDinpView::OnButtonF8() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	if( IsDispSlip() ){
		m_pImgmng->DispImgDlgNextPage();

		// フォーカス再セット
		set_focus( get_nowpn() );
		return;
	}

	try
	{
		FNC_BUTTON = TRUE;
		{
			switch( INP_mode )
			{
			case _KEYSCAN:
			case _APPEND:
			case _MODEFY:
				// データセットのため	04.09 /99
				if( get_nowpn() == TKY_PN )
					TerminationIcsinputTky( this, get_nowln(), 0, 0, 0 );

				//検索項目入力
				gotoscankey( /*TRUE*/ );
				break;
			case _RETREV:
				// ここには もうこないはず
				ASSERT( FALSE );

				break;
			}
		}
		FNC_BUTTON = FALSE;
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}
#endif


void CDBDinpView::CalcMouseEvent()
{
	if( get_nowpn() == VAL_PN || get_nowpn() == ZEI_PN ) {
		CWnd* pwnd, *pParent;
		pwnd = GetFocus();

		CRect rct;
		if( pwnd ) {
			if( pwnd->m_hWnd != m_Input.m_hWnd &&  pwnd->m_hWnd != m_Inp2.m_hWnd ) {
				pParent = pwnd->GetParent();

				::SendMessage( pwnd->m_hWnd, WM_KEYDOWN, VK_ESCAPE, 0 );
			}
		}
	}
}


// 検索項目入力へ
void CDBDinpView::gotoscankey()
{
	int inp_mode;

	if( DATA_1RD )
	{
		//データの戻し
		if( INP_mode == _KEYSCAN )
			m_pScanView->scan_zrec_set( &CREC, SCROLL_NL() - get_nowln() );

		//CREC to 残高ADD.
		int ret = rewupdat( &CREC, &SCAN_rec, INP_mode );
		if( rewupdat_job( ret ) )
			return;

		DATA_1RD = 0;
	}

	// 電卓ウィンドウ消去
	CalcMouseEvent();
	// 新規入力DBEDIT を消す
	m_Input.ShowWindow(SW_HIDE);

	//消費税入力消去
	if( get_nowpn() == SKBN_PN )
	{
		PROGRAM_OK = FALSE;
# ifdef _NEWFOCUS_	// 01.27 /01
		__set_focus( get_nowln(), SKBN_PN );
# else
		set_focus( SKBN_PN );
# endif

		Destroy_SkbnIn();

		PROGRAM_OK = TRUE;
	}

	if( get_nowpn() == BMON_PN ) {
		Destroy_BkInp();
	}
	if( get_nowpn() == HSEN_PN ) {
		Destroy_HusenInp();
	}
	Destroy_SubDataInp();

	SelWndShow( FALSE );

	m_DtList.HideTitleTipWindow();

	//入力または行戻りの修正はモードをセーブしておく
	if( INP_mode == _APPEND || INP_mode == _MODEFY )
	{
		_save_inpcondition();
	}

	SaveDinpPara( INP_mode );	//選択科目等表示属性セーブ
	SAVE_PN = -1;

#ifdef CUT_CLOSE
	CDPG = 0;
	KPGcnt = resetpag( CDPG );
//!	m_selval32.SetData( (long)SelVal32 );
	SetICSSel32( (long)SelVal32, (pAUTOSEL->KMNO_OPT != OPT_KMNO) );
#endif
	SelPar par;
	par.sel_pn = SL_KAMOKU_PN;
	m_Seldata.SelectJob( &par, -1 );

	inp_mode = INP_mode;
	inpmdisp( _RETREV );

	double x, y;
	x = y = 1.0;
//	GetRate( x, y );
	
	if( inp_mode != _KEYSCAN )
	{
		scanitem_initEX( &SC_Item );
//		SC_Tky.RemoveAll();
	}

	insert_clear();

	DWORD nFlag = 0;

	if( pAUTOSEL->KMNO_OPT == OPT_KMNO )	// 科目コード表示
		nFlag |= SCF_KMKCODE;
	if( pAUTOSEL->BRTEK_OPT == OPT_DSP )	// 枝番名称表示
		nFlag |= SCF_BRNTKYO;
	if( pAUTOSEL->SYGBRN_OPT == OPT_DSP )	// 諸口枝番表示
		nFlag |= SCF_SYOGBRN;
	if( pAUTOSEL->SWKTEK_OPT == OPT_SWKTEK )	// 仕訳摘要表示優先
		nFlag |= SCF_TKYSWK;

	//	摘要のみ修正
	((CMainFrame*)GetDBDinpMainFrame())->IsTkyOnly() = FALSE;	// サインの初期化
	((CMainFrame*)GetDBDinpMainFrame())->ChgToSkeyView();

//	if( ScanItemDialog( &SC_Item, nFlag, pDBzm, this, x, y ) == IDOK ) {
//		PostMessage( WM_KEYDOWN, VK_F8 );
//	}
//	else {
//		PostMessage( WM_KEYDOWN, VK_END );	
//	}

#ifdef _SLIP_ACCOUNTS_
	if( IsDispSlip() ){
		m_pImgmng->DispImgDlgOff();
		ChgLinkBtnState( ID_DISPSLIP_BSTATE );
	}
#endif

}



// 直接 検索項目入力へいく場合
void CDBDinpView::directscankey()
{
	int inp_mode;

	if( DATA_1RD )
	{
		//データの戻し
		if( INP_mode == _KEYSCAN )
			m_pScanView->scan_zrec_set( &CREC, SCROLL_NL() - get_nowln() );

		//CREC to 残高ADD.
		rewupdat( &CREC, &SCAN_rec, INP_mode );
		DATA_1RD = 0;
	}

	//消費税入力消去
	if( get_nowpn() == SKBN_PN )
	{
		PROGRAM_OK = FALSE;
# ifdef _NEWFOCUS_	// 01.27 /01
		__set_focus( get_nowln(), SKBN_PN );
# else
		set_focus( SKBN_PN );
# endif

		Destroy_SkbnIn();

		PROGRAM_OK = TRUE;
	}

	if( get_nowpn() == BMON_PN ) {
		Destroy_BkInp();
	}
	if( get_nowpn() == HSEN_PN ) {
		Destroy_HusenInp();
	}

	m_DtList.HideTitleTipWindow();

	//入力または行戻りの修正はモードをセーブしておく
	if( INP_mode == _APPEND || INP_mode == _MODEFY )
	{
		_save_inpcondition();
	}

	SaveDinpPara( INP_mode );	//選択科目等表示属性セーブ
	SAVE_PN = -1;

#ifdef CUT_CLOSE
	CDPG = 0;
	KPGcnt = resetpag( CDPG );
//!	m_selval32.SetData( (long)SelVal32 );
	SetICSSel32( (long)SelVal32, (pAUTOSEL->KMNO_OPT != OPT_KMNO) );
#endif
	SelPar par;
	par.sel_pn = SL_KAMOKU_PN;
	m_Seldata.SelectJob( &par, -1 );

	inp_mode = INP_mode;
	inpmdisp( _RETREV );

	insert_clear();

	double x, y;
	x = y = 1.0;
//	GetRate( x, y );

	//	摘要のみ修正
	((CMainFrame*)GetDBDinpMainFrame())->IsTkyOnly() = FALSE;	// サインの初期化

	if( inp_mode != _KEYSCAN )
	{
		scanitem_initEX( &SC_Item );
	//	SC_Tky.RemoveAll();
	}
}



// 検索開始
void CDBDinpView::gotokeyscan()
{
	int rno;

	inpmdisp( _KEYSCAN );

	// PROGRAM_OK が FALSEになっているので 最終データが取得できていなかったため 移動
	// ~~~~~~~~~~~~~~~~~~~ <-- 05.16 /01 の修正
#ifdef CLOSE //#* 03.07 /02
	m_pScanKey->last_data_set();	// 最終データのセット
#endif

//	if( m_pScanKey->SCAN_KEY.LINE_CND[ SCN_SEQ1_PN ].INP_sg )
//		rno = m_pScanKey->SCAN_KEY.SCN_SEQ1_DT.IP_SEQ - 1;
//	else
		rno = 0;


	((CMainFrame*)GetDBDinpMainFrame())->ChgToScanView( 0, rno, 0 );

}

// 出納帳へ タブクリックで 移動した場合
//
void CDBDinpView::directacbook()
{

	if( DATA_1RD )
	{
		//データの戻し
		if( INP_mode == _KEYSCAN ) 
			m_pScanView->scan_zrec_set( &CREC, SCROLL_NL() - get_nowln() );

		//CREC to 残高ADD.
		rewupdat( &CREC, &SCAN_rec, INP_mode );
		DATA_1RD = 0;
	}

	//消費税入力消去
	if( get_nowpn() == SKBN_PN )
	{
		PROGRAM_OK = FALSE;

# ifdef _NEWFOCUS_	// 01.27 /01
		__set_focus( get_nowln(), SKBN_PN );
# else
		set_focus( SKBN_PN );
# endif

		Destroy_SkbnIn();

		PROGRAM_OK = TRUE;
	}

	if( get_nowpn() == BMON_PN ) {
		Destroy_BkInp();
	}
	if( get_nowpn() == HSEN_PN ) {
		Destroy_HusenInp();
	}

	m_DtList.HideTitleTipWindow();

	//入力または行戻りの修正はモードをセーブしておく
	if( INP_mode == _APPEND || INP_mode == _MODEFY )
	{
		_save_inpcondition();
	}

	SaveDinpPara( INP_mode );	//選択科目等表示属性セーブ
	SAVE_PN = -1;

#ifdef CUT_CLOSE
	CDPG = 0;
	KPGcnt = resetpag( CDPG );
//!	m_selval32.SetData( (long)SelVal32 );
	SetICSSel32( (long)SelVal32, (pAUTOSEL->KMNO_OPT != OPT_KMNO) );
#endif

	SelPar par;
	par.sel_pn = SL_KAMOKU_PN;
	m_Seldata.SelectJob( &par, -1 );

}


// 摘要の カナターミネーション処理
//
//
void CDBDinpView::_KanaTermIcsinputTky(LPCTSTR data, BOOL bTkcut)
{
	int dc_sw;
	CDBipTKREC tkrec;
	struct _ItemData dmy_data = {0};

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	try
	{
		if( tky_50kana_chk((char*)data) ) {
			//５０音摘要選択画面に表示
			SelPar par;
			par.sel_pn = SL_TKYKANA_PN;
			strncpy_s( par.tky_kana, sizeof par.tky_kana, m_tky_50kana, sizeof par.tky_kana );
			m_Seldata.SelectJob( &par, -1 );

			pDBzm->tkrec_kanamode() = TRUE;
			m_BrTek.set_tkquery( TRUE );
			//５０音摘要があった
			if( m_SelDispPN == SL_TKYKANA_PN ) {
				SelWndShow( TRUE );
				return;
			}
		}

#ifndef _SLIP_ACCOUNTS_
		//摘要選択
		if( dinp_modefy_ok( getLINE_DATA() ) && tky_select( &tkrec, (char *)data ) != -1 )
		{
			//摘要toデータ
			dc_sw = tky_select_to_drec( &CREC, &tkrec );
			//表示
			tky_select_dsp( &CREC, dc_sw, &tkrec, TRUE, bTkcut );
		}
		else
		{
//			data_reply( getINP_CTL( TKY_PN )->IDC_X, -1, &dmy_data, CICSInput );
		}
#else
		//摘要選択
		if( IsDispSlip() == FALSE ){
			if( dinp_modefy_ok( getLINE_DATA() ) && tky_select( &tkrec, (char *)data ) != -1 )
			{
				// 摘要toデータ
				dc_sw = tky_select_to_drec( &CREC, &tkrec );
				// 表示
				tky_select_dsp( &CREC, dc_sw, &tkrec, TRUE, bTkcut );
			}
			else
			{
//				data_reply( getINP_CTL( TKY_PN )->IDC_X, -1, &dmy_data, CICSInput );
			}
		}
		else{
			if( m_SelDispPN == SL_SLIP_TKY_PN ){
				if( dinp_modefy_ok(getLINE_DATA()) ){
					// 
					if( tky_select_in_img(&tkrec, (char *)data) != -1 ){
						// 摘要toデータ
						dc_sw = tky_select_to_drec( &CREC, &tkrec );
						// 表示
						tky_select_dsp( &CREC, dc_sw, &tkrec, TRUE, bTkcut );
					}
				}
			}
			else{
				// 原票からのイメージ摘要抽出を一時停止
				if( dinp_modefy_ok( getLINE_DATA() ) && tky_select( &tkrec, (char *)data ) != -1 ){
					// 摘要toデータ
					dc_sw = tky_select_to_drec( &CREC, &tkrec );
					// 表示
					tky_select_dsp( &CREC, dc_sw, &tkrec, TRUE, bTkcut );
				}
			}
		}
#endif
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

//
// 項目入力ＯＫチェック
BOOL CDBDinpView::inp_ok( int prev_pn, int now_pn, int* set_pn )
{
	int pn, chk_sgn;

	chk_sgn = 0;
	
	if( ( (prev_pn < now_pn) && !BOTOM_SEQ ) /*|| (chk_sgn && !BOTOM_SEQ)*/ )
	{
		int i;

		for( i = 0 ; gPNtbl[i] != now_pn ; ++i )
		{
			pn = gPNtbl[i];
			switch( pn )
			{
			case DEBT_PN:
			case CRED_PN:
			case VAL_PN:
				break;
			default:
				continue;
			}
			if( getLINE_DATA()->LINE_CND[pn].INP_sg == FALSE )
			{
				PROGRAM_OK = FALSE;
				inp_errmsg( pn );
				*set_pn = pn;
				PROGRAM_OK = TRUE;
				return FALSE;
			}
		}
	}
	return TRUE;
}


//仕訳データの摘要から摘要レコードを取得する
void CDBDinpView::datetekiyo_check_tkrec(CDBINPDataRec *CREC)
{
	//FILETrace( "+++tky_select_to_drec\n" );
	if( INP_mode == _APPEND )
	{
		//定型仕訳の書き込み時の摘要消費税を保存
		if( RemainRdata() ||
			!RemainRdata() && ((m_RDTATBL.RD_cnt != 0 && m_RDTATBL.RD_trn != 0) && m_RDTATBL.RD_cnt == m_RDTATBL.RD_trn) ) {

			CString filter, tekTmp;
			tekTmp = CREC->m_tekiyo;
			AdjustSQLsearch(tekTmp);
			filter.Format("tkname = \'%s\'", tekTmp);
			pDBzm->tkrec->Requery(filter, 0);

			if( pDBzm->tkrec->st == 0 ) {
				CDBipTKREC	tkd;

				do {
					pDBzm->FuncTekiyoToRecord(&tkd);

					if( tkd.m_tksgn[1] || pDBzm->CheckTkrec(&tkd) )
					{
						TKattr = getLINE_DATA()->LINE_CND[TKY_PN].TKattr = tkd.m_tksgn[1];	//消費税コード
						gTKattrec = tkd;
						break;
					}
				} while( pDBzm->tkrec->MoveNext() == 0 );
			} 
			tkrec_queryAll();
		}
		TKattr2 = 0;	// 前行分消費税コードクリア
	}
}


/*===============================
	選択摘要toデータ
================================*/
int CDBDinpView::tky_select_to_drec( CDBINPDataRec *CREC, CDBipTKREC *ptk )
{
	int dc_sw = 0;

//FILETrace( "+++tky_select_to_drec\n" );
	if( INP_mode == _APPEND )
	{
		// インボイス対応
		// 消費税属性もしくは、免税事業者からの課税仕入れサイン、登録番号セットされているか
		//消費税コードチェック
		//if( ptk->m_tksgn[1] )
		if( ptk->m_tksgn[1] || pDBzm->CheckTkrec( ptk ))
		{
			TKattr = getLINE_DATA()->LINE_CND[ TKY_PN ].TKattr = ptk->m_tksgn[1];	//消費税コード

			gTKattrec = *ptk;
		}

		TKattr2 = 0;	// 前行分消費税コードクリア
	}

	//摘要枝番を検索
	if( Voln1->tk_br && BRmst && (get_nowpn() == TKY_PN) )
	{

		dc_sw = tbrasrch( CREC, ptk->m_tkcod, &getLINE_DATA()->LINE_CND[0] );

		int sy_chk = 0;
		if (dc_sw & 0x01) {
			if (pDBzm->IsDiffEdabanSyzSgn((char*)(LPCTSTR)CREC->m_dbt, CREC->m_dbr)) {
				sy_chk++;
			}
		}
		if (dc_sw & 0x02) {
			if (pDBzm->IsDiffEdabanSyzSgn((char*)(LPCTSTR)CREC->m_cre, CREC->m_cbr)) {
				sy_chk++;
			}
		}
		//摘要枝番の枝番消費税サインを仕訳につける。そのあと、摘要の消費税属性をつける
		if (sy_chk) {
			sy_chk_txt(CREC, NULL, INP_mode);
		}
	}

	// インボイス対応
	// 消費税属性もしくは、免税事業者からの課税仕入れサイン、登録番号セットされているか
	//消費税セット
	//if( INP_mode == _APPEND && TKattr && pDBsy->IsSyohizeiMaster() )
	if( INP_mode == _APPEND && ( TKattr || pDBzm->CheckTkrec( ptk )) && pDBsy->IsSyohizeiMaster() )
	{
		pDBzm->SetCDBData( CREC );
		// インボイス対応
		//pDBsy->Sy_tkyo_inp( TKattr );
		_SY_TKREC_ syTkrec = { 0 };
		pDBzm->SetSyTkrec( &syTkrec, ptk, CREC );
		pDBsy->Sy_tkyo_inp( &syTkrec );

		CREC->m_dsign.Copy( pDBzm->dbdata->dsign );

		DBKNREC* pKn1, *pKn2;
		pKn1 = pDBzm->DB_PjisToKnrec(CREC->m_dbt);
		pKn2 = pDBzm->DB_PjisToKnrec(CREC->m_cre);
		int siire = 0;
		if( pKn1 && pDBzm->km_siire(pKn1 ) ) {
			siire++;
		}
		if( pKn2 && pDBzm->km_siire(pKn2) ) {
			siire++;
		}

		int tmp = 0;
		tmp = _atoh(( char* )( LPCTSTR )CREC->m_dbt );
		if( IsKaribarai( tmp )) {
			siire++;
		}

		tmp = _atoh(( char* )( LPCTSTR )CREC->m_cre );
		if( IsKaribarai( tmp )) {
			siire++;
		}

		if( siire ) {
			sprintf_s(CREC->m_invno, sizeof(CREC->m_invno), _T("%s"), pDBzm->dbdata->invno);
		}
		else {
			::ZeroMemory(CREC->m_invno, sizeof CREC->m_invno);
		}
		l_input( CREC->m_zei, (char*)(LPCTSTR)pDBzm->dbdata->zei );

		_DATA_LINE* ldata;
		ldata = getLINE_DATA();

		if( ldata->LINE_CND[ DEBT_PN ].INP_sg && ldata->LINE_CND[ CRED_PN ].INP_sg )
			TKattr = 0;
	}

	return dc_sw;
}

/*==========================
	摘要セットデータ表示
===========================*/
void CDBDinpView::tky_select_dsp( CDBINPDataRec *CREC, int dc_sw, CDBipTKREC *ptk, BOOL F12_KEY, BOOL bTkcut/*=FALSE*/ )
{
//	char fmt[10];

	//摘要枝番表示
	if( dc_sw & 0x01 )
	{
		//借方枝番セット＆表示
		SREC.m_dbr = CREC->m_dbr;
		getLINE_DATA()->_XXDTA.dd_dbr = CREC->m_dbr;
		// 資金繰諸口枝番等セット
		set_etcdsp( get_nowln(), DEBT_PN );
		TerminationDataSet( DEBT_PN );
	}
	if( dc_sw & 0x02 )
	{
		//貸方枝番セット＆表示
		SREC.m_cbr = CREC->m_cbr;
		getLINE_DATA()->_XXDTA.dd_cbr = CREC->m_cbr;
		// 資金繰諸口枝番等セット
		set_etcdsp( get_nowln(), CRED_PN );
		TerminationDataSet( CRED_PN );
	}

	//#* 03.14 /02 ... 摘要追加
	if( !bTkcut ) {
		//仕訳摘要表示
//#ifndef _SLIP_ACCOUNTS_
		char tkstr[64];
//#else
//		char tkstr[128] = {0};
//#endif
		strcpy_s( tkstr, sizeof tkstr, ptk->m_tkname );
		
		if( get_nowpn() == TKY_PN ) {
			VARIANT var;
			var.pbVal = (BYTE*)tkstr;
			CWnd* pwnd = GetDlgItem( getINP_CTL( TKY_PN )->IDC_X );
			short pos = ((CICSDBEDT*)pwnd)->GetCaretPosition();
			((CICSDBEDT*)pwnd)->InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );
			//データセットのためのダミーフォーカスＯＦＦ
			TerminationIcsinputTky( this, get_nowln(), 0, 0, 0 );
		}
		else {
			// 現レコードにセット
			int len = CREC->m_tekiyo.GetLength();
			int byte = (Voln1->tk_ln*2);
			if( len < byte ) {
				CREC->m_tekiyo += tkstr;
				len = CREC->m_tekiyo.GetLength();
				if( len > byte ) {
					int n = get_strcnt( (char*)(LPCTSTR)CREC->m_tekiyo, byte );
					CREC->m_tekiyo.SetAt( n, '\0' );
				}
			}
		}
	}
	else {
		// 摘要追加せず
	//	if( F12_KEY )
	//		;
	//		data_reply( getINP_CTL( TKY_PN )->IDC_X, 0, &ins_data, CICSInput );
	}

	//消費税
	getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt( CREC, NULL, INP_mode);
	//税額
	memcpy( getLINE_DATA()->_XXDTA.dd_zei, CREC->m_zei, 6 );
	//消費税再表示
	int pntbl[] = {SKBN_PN, ZEI_PN};
	TerminationDataSet( pntbl, 2 );
}

/*==============================
	枝番除いてセットデータ表示
================================*/
void CDBDinpView::remove_edabandsp( CDBINPDataRec *crec, int pn )
{
	struct _ItemData check={0};

	if( pn == DEBT_PN || pn == CRED_PN ) {
		if( pn == DEBT_PN ) {
			if( crec->m_dbr != -1 ) crec->m_dbr = -1;
			else	return;
		}
		else {
			if( crec->m_cbr != -1 ) crec->m_cbr = -1;
			else	return;
		}

		// 枝番セット＆表示
		getLINE_DATA()->LINE_CND[ pn ].INP_type = brn_dfltinptype( -1 );

		if( pn == DEBT_PN ) {
			check.IP_BRN = getLINE_DATA()->_XXDTA.dd_dbr = SREC.m_dbr = crec->m_dbr;
		}
		else {
			check.IP_BRN = getLINE_DATA()->_XXDTA.dd_cbr = SREC.m_cbr = crec->m_cbr;
		}

		// 資金繰諸口枝番等セット
		set_etcdsp( get_nowln(), pn );

		TerminationDataSet( pn );
	}
}




int CDBDinpView::getAUTOSELpn( int ln, int nowpn )
{
	DT_RECT* pdt;
	int sgn = 0;

	// フォーカス順は、CDataListCtrl での アイテム矩形の登録順による
	if( ln == APPEND_LINE )
		pdt = m_DtLine.GetDtRect();
	else
		pdt = m_DtList.GetDtRect();

	if( pdt == NULL )
		return nowpn;

	for( int i = 0; i < pdt->Dtcnt; i++ ) {
#ifdef REGDATA	// 04.12 /01
		if( m_RDTATBL.RD_cnt > 0 )	// 定型仕訳処理中
			if( pdt->DtID[i] > DENP_PN )	// DENP_PN 以上
				return nowpn;
#endif

		if( nowpn == pdt->DtID[i] )
			sgn = 1;

		if( sgn ) {
			switch( pdt->DtID[i] ) {
			case DATE_PN: 
				if( pAUTOSEL->DATE_OPT == OPT_NONOPT /*!AUTOSEL.DATE_FIX*/ )
					return DATE_PN;
				break;
			case DENP_PN: 
				if( pAUTOSEL->DENP_OPT == OPT_NONOPT /*!AUTOSEL.DENP_MODE*/ )
					return DENP_PN;
				break;

			case DOCEVI_PN:		// 証憑追加
				if( pAUTOSEL->DOCEVI_OPT == OPT_NONOPT )
					return DOCEVI_PN;
				break;

			case BMON_PN:
				if( BMON_MST )
					if( pAUTOSEL->BMON_OPT == OPT_NONOPT /*!AUTOSEL.BMON_FIX*/|| !BOTOM_SEQ )
						return BMON_PN;
				break;

			case KOJI_PN:
				if( M_KOJI )
					if( pAUTOSEL->KOJI_OPT == OPT_NONOPT /*!AUTOSEL.BMON_FIX*/|| !BOTOM_SEQ )
						return KOJI_PN;
				break;
			case DEBT_PN:
				if( pAUTOSEL->DEBT_OPT == OPT_NONOPT /*!AUTOSEL.DEBT_FIX*/ || !BOTOM_SEQ )
					return DEBT_PN;
				break;
			case CBMN_PN:
				if( BMON_MST )
					if( pAUTOSEL->BMON_OPT == OPT_NONOPT /*!AUTOSEL.BMON_FIX*/|| !BOTOM_SEQ )
						return CBMN_PN;
				break;
			case CKJI_PN:
				if( M_KOJI )
					if( pAUTOSEL->KOJI_OPT == OPT_NONOPT /*!AUTOSEL.BMON_FIX*/|| !BOTOM_SEQ )
						return CKJI_PN;
				break;

			case CRED_PN:
				if( pAUTOSEL->CRED_OPT == OPT_NONOPT /*!AUTOSEL.CRED_FIX*/ || !BOTOM_SEQ )
					return CRED_PN;
				break;
			}
		}
	}


	return VAL_PN;
}


////////////////////////////////////////////////////////////////////////////////
// DtListCtrl
//	入力ウィンドウ開始
void CDBDinpView::OnBeginLabelEdit(UINT ID, NMHDR* pNMHDR, LRESULT* pResult)
{
//MyTrace( "xxx--- TOP BeginLabelEdit --- POK = %d, itm=[%d]\n", PROGRAM_OK, ((NM_DLCTVIEW*)pNMHDR)->iItem );
	if( PROGRAM_OK == FALSE )
		return;

	//挿入時
	if( m_InsLine != -1 && INP_mode == _APPEND && ID !=IDC_DTLINE ) {
		set_focus(get_nowpn());
		return ;
	}

	NM_DLCTVIEW* dv;
	dv = (NM_DLCTVIEW*)pNMHDR;
//TRACE( "xxx--- ID %d, nItem %d nLine %d ItemRect %d,%d,%d,%d\n", ID, dv->iItem, dv->iLine, 
//							dv->rect.left, dv->rect.top, dv->rect.right, dv->rect.bottom );
//TRACE( "xxx--- OnBeginLabelEdit iClick = %d\n", dv->iClick );

	//伝票タイプ
	if( dv->iItem == DATA_USER1 ) {
		set_focus(get_nowpn());
		return ;
	}
	CRect inprc, rect;
	inprc = dv->rect;
	GetDlgItem( ID )->ClientToScreen( inprc );
//	m_DtList.ClientToScreen( inprc );
	ScreenToClient( inprc );

	char tmp[30];
	CString koji;
	struct _KamokuData check = {0};

	int line, edit_id, bmn, pos, i;
	line = (ID ==IDC_DTLINE) ? APPEND_LINE : dv->iLine;

/*
	if( line == APPEND_LINE ) {
		switch( dv->iItem ) {
		case DATA_SYONO:
		case DATA_TGDATE:
			if( pAUTOSEL->BILL_OPT == OPT_NOINP ) {
				set_focus( get_nowpn() );
				return;
			}
			break;
		case DATA_HUSEN:
			if( pAUTOSEL->HUSEN_OPT == OPT_NOINP ) {
				set_focus( get_nowpn() );
				return;
			}
			break;
		}
	}
*/

	if( m_InsLine != -1 ) {
		((CMainFrame*)GetDBDinpMainFrame())->StatusGuideDisp(IDS_DTINS_GUIDE);
	}
	else {
		((CMainFrame*)GetDBDinpMainFrame())->StatusGuideDisp(AFX_IDS_IDLEMESSAGE);
	}

	//日付選択
	m_selDate.ShowWindow(SW_SHOW);

	if( dv->iItem == DATA_HUSEN ) {
		FocusHusenInput( this, line, dv->iClick );
		return;
	}
	else {
		// 付箋フォーカス サインを 戻す。
		if( ! m_bHusenFocus )	m_bHusenFocus = TRUE;
	}
	

	CICSDBEDT* pDBed;
	CDataListCtrl* pListCtrl;
	if( ID == IDC_DTLINE ) {
		edit_id = IDC_ICSDBEDTCTRL1;
		pDBed = &m_Input;
		pListCtrl = &m_DtLine;
	}
	else {
		edit_id = IDC_ICSDBEDTCTRL2;
		pDBed = &m_Inp2;
		pListCtrl = &m_DtList;
	}
	DBeditCtrlSet( edit_id, dv->iItem );
//	pDBed->SetRegColor( TRUE );

	BYTE buf[100] = {0};
	VARIANT var;

	pDBed->ShowWindow(SW_SHOW);

	if( dv->iItem == DATA_DATE && m_bMnthFix ) {
		int wd;
		wd = inprc.Width();
	//	i = (wd / 5) + 1;
	//	i *= 2;
		i = wd/2 -1;
		pDBed->MoveWindow( inprc.left + (wd-i), inprc.top, i, inprc.Height() );
	}
	else {
		pDBed->MoveWindow( inprc.left, inprc.top, inprc.Width(), inprc.Height() );
	}
	c_set( edit_id );

	switch( dv->iItem ) {
	case DATA_SEQ:
		FocusIcsSeq( this, line, dv->iClick );

		if( line == APPEND_LINE )
		{
//			GetDlgItem( _getINP_CTL( SCROLL_NL, SEQ_PN )->IDC_X )->PostMessage( WM_LBUTTONDOWN );
//			GetDlgItem( _getINP_CTL( SCROLL_NL, SEQ_PN )->IDC_X )->PostMessage( WM_LBUTTONUP );
			set_focus( /*get_nowpn()*/ DATE_PN );
			return;
		}
		sprintf_s( (char*)buf, sizeof buf, "%d",  dv->data.dd_seq );
		var.pbVal = buf;
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );

#ifdef _SLIP_ACCOUNTS_
		if( line == APPEND_LINE ){
			ChangeImgDlgDisp( DATE_PN );
		}
		else{
			ChangeImgDlgDisp( dv->iItem );
		}
#endif
		break;
	case DATA_DATE:
		FocusIcsDate( this, line, dv->iClick );

#ifdef _SLIP_ACCOUNTS_
		ChangeImgDlgDisp( dv->iItem );
		if( IsDispSlip() ){
/*- '12.11.08 -*/
//			m_pImgmng->DeleteSmWindow();
/*-------------*/
			m_pImgmng->MoveSelectInDlg( ID_DATE_SEARCH );
/*-------------*/
		}
#endif
		if( ! m_bMnthFix ) {
			memcpy( buf, getLINE_DATA()->BcdDate, 2 );
		}
		else {
			buf[0] = getLINE_DATA()->BcdDate[1];
		}
		var.pbVal = buf;
		pDBed->SetData( &var, ICSDBEDT_TYPE_BCD, m_bMnthFix ? 2 : 4 );
		pDBzm->BcdDateToString( tmp, sizeof tmp, getLINE_DATA()->BcdDate, m_bMnthFix );
		pDBed->SetDisplayString( tmp );
		break;
	case DATA_DENP:	
		FocusIcsDenp( this, line, dv->iClick );

#ifdef _SLIP_ACCOUNTS_
		ChangeImgDlgDisp( dv->iItem );
		if( IsDispSlip() ){
/*- '12.11.08 -*/
//			m_pImgmng->MoveSelectInDlg( ID_DATE_SEARCH );
/*-------------*/
			m_pImgmng->DeleteSmWindow();
/*-------------*/
		}
#endif
		if( getLINE_DATA()->_XXDTA.dd_cno != -1 ) {
			sprintf_s( (char*)buf, sizeof buf, "%d", getLINE_DATA()->_XXDTA.dd_cno );
			var.pbVal = buf;
		}
		else {
			var.pbVal = NULL;
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
		break;	
	case DATA_DEB_BMON:
	case DATA_CRE_BMON:
		FocusIcsBmon( this, line, (dv->iItem==DATA_DEB_BMON) ? 0 : 1, dv->iClick );

#ifdef _SLIP_ACCOUNTS_
		ChangeImgDlgDisp( dv->iItem );
		if( IsDispSlip() ){
			m_pImgmng->DeleteSmWindow();
		}
#endif
		if( dv->iItem==DATA_DEB_BMON ) {
			bmn = getLINE_DATA()->_XXDTA.dd_dbmn;
		}
		else {
			bmn = getLINE_DATA()->_XXDTA.dd_cbmn;
		}
		if( bmn != -1 ) {
			pDBzm->BumonCodeToStr( (char*)buf, sizeof buf, bmn );
			var.pbVal = buf;
		}
		else {
			var.pbVal = NULL;
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );

		if( dv->iItem == DATA_DEB_BMON ) {
			if( ! getLINE_DATA()->_XXDTA.dd_dbmn_etc.IsEmpty() )
				pListCtrl->EtcStaticDisp( dv->rect, dv->data.dd_dbmn_etc, getLINE_DATA()->LINE_CND[BMON_PN].ETC_TXT_ForeCOL );
		}
		else {
			if( ! getLINE_DATA()->_XXDTA.dd_cbmn_etc.IsEmpty() )
				pListCtrl->EtcStaticDisp( dv->rect, dv->data.dd_cbmn_etc, getLINE_DATA()->LINE_CND[CBMN_PN].ETC_TXT_ForeCOL );		
		}
		break;	
	case DATA_DEB_KNO:
	case DATA_CRE_KNO:
		FocusIcsKoji( this, line, (dv->iItem==DATA_DEB_KNO) ? 0 : 1, dv->iClick );

#ifdef _SLIP_ACCOUNTS_
		ChangeImgDlgDisp( dv->iItem );
		if( IsDispSlip() ){
			m_pImgmng->DeleteSmWindow();
		}
#endif
		if( dv->iItem == DATA_DEB_KNO ) {
			koji = getLINE_DATA()->_XXDTA.dd_dkno;
		}
		else {
			koji = getLINE_DATA()->_XXDTA.dd_ckno;
		}
		if( ! koji.IsEmpty() ) {
			pDBzm->KojiCodeToStr( (char*)buf, sizeof buf, koji );
			var.pbVal = buf;
		}
		else {
			var.pbVal = NULL;
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );

		if( dv->iItem == DATA_DEB_KNO ) {
			if( ! getLINE_DATA()->_XXDTA.dd_dkno_etc.IsEmpty() )
				pListCtrl->EtcStaticDisp( dv->rect, dv->data.dd_dkno_etc, getLINE_DATA()->LINE_CND[KOJI_PN].ETC_TXT_ForeCOL );
		}
		else {
			if( ! getLINE_DATA()->_XXDTA.dd_ckno_etc.IsEmpty() )
				pListCtrl->EtcStaticDisp( dv->rect, dv->data.dd_ckno_etc, getLINE_DATA()->LINE_CND[CKJI_PN].ETC_TXT_ForeCOL );
		}
		break;	
	case DATA_DEBT:
		FocusIcsDebt( this, line, dv->iClick );
		
#ifdef _SLIP_ACCOUNTS_
		ChangeImgDlgDisp( dv->iItem );
		if( IsDispSlip() ){
			m_pImgmng->DeleteSmWindow();
		}
#endif
		memcpy( check.kd_code, getLINE_DATA()->_XXDTA.dd_dbt, KMKCD_LN );
		check.kd_eda = getLINE_DATA()->_XXDTA.dd_dbr;
		strcpy_s( check.kd_name, sizeof check.kd_name, getLINE_DATA()->_XXDTA.dd_deb_name );
		exDBkamokudata_set( DEBT_PN, getINP_CTL( this, DEBT_PN )->IDC_X, &check );

		if( ! getLINE_DATA()->_XXDTA.dd_deb_etc.IsEmpty() )
			pListCtrl->EtcStaticDisp( dv->rect, dv->data.dd_deb_etc, getLINE_DATA()->LINE_CND[DEBT_PN].ETC_TXT_ForeCOL );
		break;	
	case DATA_CRED:
		FocusIcsCred( this, line, dv->iClick );
		
#ifdef _SLIP_ACCOUNTS_
		ChangeImgDlgDisp( dv->iItem );
		if( IsDispSlip() ){
			m_pImgmng->DeleteSmWindow();
		}
#endif
		memcpy( check.kd_code, getLINE_DATA()->_XXDTA.dd_cre, KMKCD_LN );
		check.kd_eda = getLINE_DATA()->_XXDTA.dd_cbr;
		memcpy( check.kd_name, getLINE_DATA()->_XXDTA.dd_cre_name, sizeof check.kd_name );
		exDBkamokudata_set( CRED_PN, getINP_CTL( this, CRED_PN )->IDC_X, &check );

		if( ! getLINE_DATA()->_XXDTA.dd_cre_etc.IsEmpty() )
			pListCtrl->EtcStaticDisp( dv->rect, dv->data.dd_cre_etc, getLINE_DATA()->LINE_CND[CRED_PN].ETC_TXT_ForeCOL );

		break;
	case DATA_VAL:
		FocusIcsVal( this, line, dv->iClick );

		if( getLINE_DATA()->LINE_CND[VAL_PN].INP_sg ) {
			memcpy( buf, getLINE_DATA()->_XXDTA.dd_val, 6 );
			var.pbVal = buf; 
		}
		else  {
			var.pbVal = NULL; 
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_ARITH, 0 );
#ifndef _SLIP_ACCOUNTS_
		if( m_bSLIP_MASTER ){
			int tooltype = 0;
			ChangeImgDlgDisp( dv->iItem, tooltype );
		}
		((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(0, 1);
#else
		ChangeImgDlgDisp( dv->iItem );
		if( IsDispSlip() ){
			m_pImgmng->DeleteSmWindow();
			((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(6, 0);
		}
		else {
			((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(0, 1);
		}
#endif
		break;
	case DATA_ZEI:
		FocusIcsZei( this, line, dv->iClick );

		if( getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg ) {
			memcpy( buf, getLINE_DATA()->_XXDTA.dd_zei, 6 );
			var.pbVal = buf; 
		}
		else  {
			var.pbVal = NULL; 
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_ARITH, 0 );

#ifdef _SLIP_ACCOUNTS_
		ChangeImgDlgDisp( dv->iItem );
		if( IsDispSlip() ){
			m_pImgmng->DeleteSmWindow();
		}
#endif
		break;
	case DATA_SYZ:
		FocusIcsSyz( this, line, dv->iClick, 0 );
#ifdef _SLIP_ACCOUNTS_
		ChangeImgDlgDisp( dv->iItem );
		if( IsDispSlip() ){
			m_pImgmng->DeleteSmWindow();
		}
#endif
		return;
		break;

	case DATA_HJIYU:
		FocusIcsHjiyu( this, line, dv->iClick );
#ifdef _SLIP_ACCOUNTS_
		ChangeImgDlgDisp( dv->iItem );
		if( IsDispSlip() ){
			m_pImgmng->DeleteSmWindow();
		}
#endif
		return;
		break;
	case DATA_TEKIYO:
		FocusIcsTky( this, line, dv->iClick );
	
		if( line == SCROLL_NL() ) {
			m_selDate.GetWindowRect( rect );
			ScreenToClient( rect );
			if( rect.IntersectRect( inprc, rect ) ) {
				m_selDate.ShowWindow(SW_HIDE);
			}
		}

		::ZeroMemory( buf, sizeof buf );
		strcpy_s( (char*)buf, sizeof buf, getLINE_DATA()->_XXDTA.dd_tek.tekiyo );
		var.pbVal = buf; 
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
		pos = strlen( (char*)buf );
		pDBed->SetCaretPosition(pos);
	//	pDBed->SetCaretPosition(0);

#ifndef _SLIP_ACCOUNTS_
		((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(1, pos == 0 ? 1 : 0);
#else
		if( m_DenpSeq == 0 ) {	//伝票修正ダイアログ表示中はツールバー空白
			ChangeImgDlgDisp( DATA_TEKIYO );

			if( IsDispSlip() == FALSE ){
				((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(1, pos == 0 ? 1 : 0);
			}
			else{
				m_pImgmng->MoveSelectInDlg( ID_ALL_SEARCH );
				((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(7, pos == 0 ? 1 : 0);
			}
		}
#endif

		if( INP_mode != _APPEND ) {
			if( ! BmonKoji_DebCre_Inp( get_nowln() ) ) {	// 工事が 一つのみ表示されている
				((CMainFrame*)GetDBDinpMainFrame())->StatusGuideDisp(IDS_HUSEN_GUIDE);
			}
		}
		break;

	case DATA_SYONO:
		FocusIcsSyono( this, line, dv->iClick );
#ifdef _SLIP_ACCOUNTS_
		if( IsDispSlip() ){
			m_pImgmng->DeleteSmWindow();
		}
#endif
		if( ! getLINE_DATA()->_XXDTA.dd_snumber.IsEmpty() ) {
			strcpy_s( (char*)buf, sizeof buf, getLINE_DATA()->_XXDTA.dd_snumber );
			var.pbVal = buf;
		}
		else {
			var.pbVal = NULL;
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );

		break;
	case DATA_TGDATE:
		FocusIcsTgDate( this, line, dv->iClick );
#ifdef _SLIP_ACCOUNTS_
		if( IsDispSlip() ){
			m_pImgmng->DeleteSmWindow();
		}
#endif
		if( getLINE_DATA()->_XXDTA.dd_tdate > 0 ) {
			int gen_tgdate, gengo;
			CVolDateDB vd;
			vd.db_datecnvGen( 0, getLINE_DATA()->_XXDTA.dd_tdate, &gengo, &gen_tgdate, 0, 0 );
			sprintf_s( (char*)buf, sizeof buf, "%06d", gen_tgdate );

			var.pbVal = buf;
			IntsrekitoGenstr( tmp, sizeof tmp, getLINE_DATA()->_XXDTA.dd_tdate );
		}
		else {
			var.pbVal = NULL;
			::ZeroMemory( tmp, sizeof tmp );
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
		pDBed->SetDisplayString( tmp );
		break;

	case DATA_DOCEVI:	//証憑番号
		FocusIcsDocEviNo( this, line, dv->iClick );
#ifdef _SLIP_ACCOUNTS_
		if( IsDispSlip() ){
			m_pImgmng->DeleteSmWindow();
		}
#endif
		if( ! getLINE_DATA()->_XXDTA.dd_docevi.IsEmpty() ) {
			strcpy_s( (char*)buf, sizeof buf, getLINE_DATA()->_XXDTA.dd_docevi );
			var.pbVal = buf;
		}
		else {
			var.pbVal = NULL;
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
		break;
	}	

	//選択表示を別ウィンドウにしている時
	if( pAUTOSEL->BLKSEL_OPT ) {
		if (m_pSelWnd != NULL && m_pSelWnd->IsWindowVisible() )
		{
			SelWndMoveJob( TRUE );
		}
	}

//ASSERT( get_nowpn() == dv->iItem );
//TRACE( "ICSINPUT str %s, form %s, border %d\n", str, form, border );

//	*pResult = 1;
}


//	入力ウィンドウ終了
void CDBDinpView::OnEndLabelEdit(UINT ID, NMHDR* pNMHDR, LRESULT* pResult)
{
//MyTrace( "XXX--- CDBDinpView::OnEndLabelEdit %d\n", ID );
	if( ID == IDC_DTLINE ) {
		m_Input.ShowWindow(SW_HIDE);
	}
	else {
		m_Inp2.ShowWindow(SW_HIDE);
	}
}

// 入力窓のサイズ調整
void CDBDinpView::OnSetDispInfo( UINT ID, NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_DLCTVIEW* dv;
	dv = (NM_DLCTVIEW*)pNMHDR;
//TRACE( "*** nItem %d nLine %d ItemRect %d,%d,%d,%d\n", dv->iItem, dv->iLine, 
//							dv->rect.left, dv->rect.top, dv->rect.right, dv->rect.bottom );
//MyTrace( "**OnSetDispInfo %d\n", ID );
	CRect inprc;
	inprc = dv->rect;
	GetDlgItem( ID )->ClientToScreen( inprc );
//	m_DtList.ClientToScreen( inprc );
	ScreenToClient( inprc );

	if( EDIT_PN != dv->iItem )
		return;

	if( dv->iItem == DATA_HUSEN ) {
	}
	else {
		if( dv->iItem == DATA_DATE && m_bMnthFix ) {
			int wd, i;
			wd = inprc.Width();
		//	i = (wd / 5) + 1;
		//	i *= 2;
			i = wd/2 -1;
			inprc.left += (wd-i);
		}

		if( ID == IDC_DTLINE ) 
			m_Input.MoveWindow( inprc.left, inprc.top, inprc.Width(), inprc.Height() );
		else
			m_Inp2.MoveWindow( inprc.left, inprc.top, inprc.Width(), inprc.Height() );
	}
}

void CDBDinpView::OnLineChanged( NMHDR* pNMHDR, LRESULT* pResult)
{
//	AfxMessageBox("表示行数可能行数が変わりました。");
}

BOOL CDBDinpView::GetDtListEnable( int ln, int pn )
{
	BOOL bEnable = FALSE;

//	if( pn != SKBN_PN ) {
		if( ln == APPEND_LINE )
			bEnable = m_DtLine.GetListItemEnable( 1, pn );
		else
			bEnable = m_DtList.GetListItemEnable( ln, pn );
//	}

	return bEnable;
}

// エディットコントロールの セット
void CDBDinpView::DBeditCtrlSet( UINT ID, int id_pn )
{
	BYTE buf[100] = {0};
	VARIANT var;
	var.pbVal = NULL;
	CICSDBEDT* pDBed;
	pDBed = (ID==IDC_ICSDBEDTCTRL1) ? &m_Input : &m_Inp2;

	// エディットコントロールの ポジションサイン
	EDIT_PN = id_pn;

	if( id_pn == DATA_DATE ) {
		pDBed->SetData( &var, ICSDBEDT_TYPE_BCD, 4 );
	}
	if( id_pn == DATA_VAL || id_pn == DATA_ZEI ) {
		pDBed->SetData( &var, ICSDBEDT_TYPE_ARITH, 0 );

		BOOL ena = TRUE;
		if( id_pn == DATA_ZEI ) {
			if( (pAUTOSEL->ZEIMDFY_OPT != OPT_ZEIMDFY) )	ena = FALSE;
		}
		pDBed->SetCalculator( ena, (long)this );
	}
	else {
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
		pDBed->SetCalculator( FALSE, (long)this );
	}
	if( id_pn == DATA_DEBT || id_pn == DATA_CRED )
		pDBed->SetDisplayString( "         " );
	else
		pDBed->SetDisplayString( NULL );
	pDBed->SetCaretPosition(0);

	// 左詰 右詰
	switch( id_pn ) {
	case DATA_DEBT:
	case DATA_CRED:
	case DATA_TEKIYO:
		pDBed->SetVector( FALSE );
		break;
	default:
		pDBed->SetVector( TRUE );
		break;
	}

	int kmklen;
	kmklen = KMKCD_LN + 1 + pDBzm->zvol->edcol;

	pDBed->SetKanaTerm(FALSE, 1, ICSDBEDT_KTYPE_KANA );
	pDBed->ImeEndMode( TRUE );
	pDBed->SetCmove( TRUE );
	pDBed->SetMultiLine( FALSE );

	if( id_pn == DATA_SEQ ) {
		// 右クリックメニュー表示しない。
		pDBed->SetExtension( 0x06, 0 );
	}
	else {
		pDBed->SetExtension( 0, 0 );
	}

	switch( id_pn ) {
	case DATA_SEQ:
	case DATA_SYZ:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( FALSE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( FALSE );
		pDBed->SetAlphabet( FALSE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "1" );
		if( id_pn == DATA_SYZ )
				pDBed->SetMaxval( "999999" );
		else	pDBed->SetMaxval( "9999999" );
		pDBed->SetMaxLen( 0 );
		pDBed->SetInverse( TRUE );
		break;
	case DATA_DATE:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( FALSE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( FALSE );
		pDBed->SetAlphabet( FALSE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		if( m_bMnthFix )	pDBed->SetMaxval( "31" );
		else				pDBed->SetMaxval( "9931" );
		pDBed->SetMaxLen( 0 );
		pDBed->SetInverse( TRUE );
		break;
	case DATA_DENP:	
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( FALSE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( FALSE );
		pDBed->SetAlphabet( FALSE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		pDBed->SetMaxval( "9999999" );
		pDBed->SetMaxLen( 0 );
		pDBed->SetInverse( TRUE );
		break;	
	case DATA_DEB_BMON:
	case DATA_CRE_BMON:
		pDBed->SetNumeric( TRUE );
	//	pDBed->SetKana( FALSE );
		pDBed->SetKana( TRUE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( FALSE );
		pDBed->SetAlphabet( FALSE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
	//	sprintf_s( tmp, sizeof tmp, "%d", pDBzm->GetBumonMaxDecimal() );
	//	pDBed->SetMaxval( tmp );
	//	pDBed->SetMaxLen( 0 );
		pDBed->SetMaxval( "0" );
		pDBed->SetMaxLen( pDBzm->zvol->bmcol );
		pDBed->SetInverse( TRUE );
		break;	
	case DATA_DEB_KNO:
	case DATA_CRE_KNO:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( TRUE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( TRUE );
		pDBed->SetAlphabet( TRUE );
		pDBed->SetFormat( FALSE ); // カンマ編集
//		pDBed->SetMinval( "0" );
//		sprintf_s( tmp, sizeof tmp, "%d", pDBzm->GetKojiMaxDecimal() );
//		pDBed->SetMaxval( tmp );
		pDBed->SetMinval( "0" );
		pDBed->SetMaxval( "0" );
		pDBed->SetMaxLen( pDBzm->zvol->kjcol );
		pDBed->SetInverse( TRUE );
		break;	
	case DATA_DEBT:
	case DATA_CRED:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( TRUE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( TRUE );
		pDBed->SetAlphabet( TRUE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		pDBed->SetMaxval( "0" );
		pDBed->SetMaxLen( kmklen );
		pDBed->SetInverse( TRUE );
		pDBed->SetKanaTerm(TRUE, KanaSearchKeta(), ICSDBEDT_KTYPE_KANA );
		break;
	case DATA_VAL:
	case DATA_ZEI:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( FALSE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( FALSE );
		pDBed->SetAlphabet( FALSE );
		pDBed->SetFormat( TRUE ); // カンマ編集
		pDBed->SetMinval( "-99999999999" );
		pDBed->SetMaxval( "999999999999" );
		pDBed->SetMaxLen( 0 );
		pDBed->SetInverse( TRUE );
		pDBed->ImeEndMode( FALSE );
		break;
	case DATA_TEKIYO:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( TRUE );
		pDBed->SetZenkaku( TRUE );
		pDBed->SetAscii( TRUE );
		pDBed->SetAlphabet( TRUE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		pDBed->SetMaxval( "0" );
		pDBed->SetMaxLen( (pDBzm->zvol->tk_ln*2) );
		pDBed->SetInverse( FALSE );
	//	pDBed->ImeEndMode( FALSE );
		break;

	case DATA_SYONO:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( TRUE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( TRUE );
		pDBed->SetAlphabet( TRUE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		pDBed->SetMaxval( "0" );
		pDBed->SetMaxLen( 20 );
		pDBed->SetInverse( TRUE );
		pDBed->ImeEndMode( FALSE );
		break;

	case DATA_TGDATE:
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( FALSE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( FALSE );
		pDBed->SetAlphabet( FALSE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		pDBed->SetMaxval( "991231" );
		pDBed->SetMaxLen( 0 );
		pDBed->SetInverse( TRUE );
		pDBed->ImeEndMode( FALSE );
		
		break;

	case DATA_DOCEVI:	//証憑番号
		pDBed->SetNumeric( TRUE );
		pDBed->SetKana( FALSE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( TRUE );
		pDBed->SetAlphabet( TRUE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
		pDBed->SetMaxval( "0" );
		pDBed->SetMaxLen( Voln1->snocol );
		pDBed->SetInverse( TRUE );
		pDBed->ImeEndMode( FALSE );
		break;
	}
}

// 手形科目のチェック
BOOL CDBDinpView::IsBillData( CString& dbt, CString& cre )
{
	BOOL bRet = FALSE;

	if( ! dbt.IsEmpty() ) {
		bRet = IsBillCodeEx( dbt, pDBzm );
	}
	if( ! bRet ) {
		if( ! cre.IsEmpty() ) {
			bRet = IsBillCodeEx( cre, pDBzm );
		}
	}

	return bRet;
}


BEGIN_EVENTSINK_MAP(CDBDinpView, CDBbaseView)
    //{{AFX_EVENTSINK_MAP(CDBDinpView)
	ON_EVENT(CDBDinpView, IDC_ICSSELCTRL1, 1 /* Selected */, OnSelectedIcsselctrl1, VTS_I2 VTS_I4)
	ON_EVENT(CDBDinpView, IDC_ICSSELCTRL2, 1 /* Selected */, OnSelectedIcsselctrl2, VTS_I2 VTS_I4)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT_RANGE(CDBDinpView, IDC_ICSDBEDTCTRL1, IDC_ICSDBEDTCTRL2, 3 /* SetFocus */, OnSetFocusIcsdbedtctrl, VTS_I4)
	ON_EVENT_RANGE(CDBDinpView, IDC_ICSDBEDTCTRL1, IDC_ICSDBEDTCTRL2, 2 /* KillFocus */, OnKillFocusIcsdbedtctrl, VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CDBDinpView, IDC_ICSDBEDTCTRL1, IDC_ICSDBEDTCTRL2, 1 /* Termination */, OnTerminationIcsdbedtctrl, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CDBDinpView, IDC_ICSDBEDTCTRL1, IDC_ICSDBEDTCTRL2, 5 /* ImeEndComposition */, OnImeEndCompositionIcsdbedtctrl, VTS_I4 VTS_I2 VTS_BSTR VTS_BSTR)
	ON_EVENT_RANGE(CDBDinpView, IDC_ICSDBEDTCTRL1, IDC_ICSDBEDTCTRL2, 4 /* KanaTermination */, OnKanaTerminationIcsdbedtctrl, VTS_I4 VTS_BSTR)
	ON_EVENT_RANGE(CDBDinpView, IDC_ICSDBEDTCTRL1, IDC_ICSDBEDTCTRL2, 7, RButtonUpIcsdbedtctrl, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CDBDinpView::OnSetFocusIcsdbedtctrl( UINT ID )
{
/*	if( PROGRAM_OK == FALSE )
		return;

	if( ! ::IsWindow( m_Input.m_hWnd ) )
		return;
*/
	DT_EDIT edit;

	edit.hdr.hwndFrom = GetDlgItem(ID)->GetSafeHwnd();
	edit.hdr.idFrom   = ID;
	edit.hdr.code     = DLUM_EDITFOCUS;

	if( ID == IDC_ICSDBEDTCTRL1 )
		m_DtLine.SendMessage(WM_DTLISTUSER, edit.hdr.code, (LPARAM)&edit );
	else
		m_DtList.SendMessage(WM_DTLISTUSER, edit.hdr.code, (LPARAM)&edit );
}

void CDBDinpView::OnKillFocusIcsdbedtctrl( UINT ID, long inplen )
{
//	if( ! ::IsWindow( m_Input.m_hWnd ) )
//		return;

	_OnTerminationIcsdbedtctrl( ID, 0, inplen, 0 );
}


void CDBDinpView::OnKanaTerminationIcsdbedtctrl( UINT ID, LPCTSTR Kana) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
//TRACE( "**KanaTerm %s\n", Kana );	
	SelWndShow( TRUE );

	switch( get_nowpn() ) {
	case DEBT_PN:
		KanaTermIcsinputDebt( Kana );
		break;
	case CRED_PN:
		KanaTermIcsinputCred( Kana );
		break;
	}
}


void CDBDinpView::OnTerminationIcsdbedtctrl( UINT ID, long nChar, long inplen, long kst)
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! ::IsWindow( m_Input.m_hWnd ) )
		return;

	_OnTerminationIcsdbedtctrl( ID, nChar, inplen, kst );
}

void CDBDinpView::OnImeEndCompositionIcsdbedtctrl( UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! ::IsWindow( m_Input.m_hWnd ) )
		return;

	if( PROGRAM_OK == FALSE )
		return;
	int st, term, pos, pn, len;
	VARIANT var;
	char	tmp[128];
	char	ystr[128];

	pn = get_nowpn();

	if( EDIT_PN != pn )
		return;

	switch( pn ) {
	case BMON_PN:	//部門カナ検索
	case CBMN_PN:
		if( CheckImeEndBumon( this, pn, nChar, string ) != 0 )
			return;
		TerminationIcsinputBumon( this, (pn==BMON_PN) ? 0 : 1, get_nowln(), nChar, 1, 0 );
		break;

	case KOJI_PN:
	case CKJI_PN:
		if( CheckImeEndKoji( this, pn, nChar, string ) != 0 )
			return;
		TerminationIcsinputKoji( this, (pn==KOJI_PN) ? 0 : 1, get_nowln(), nChar, 1, 0 );
		break;
	case DEBT_PN:
		if( ! lastBrnInpStr(string,  tmp, sizeof tmp) )
			return;
		if( CheckDataIcsinputDebt( this, nChar, tmp) <= 0 )
			return;
		TerminationIcsinputDebt( this, get_nowln(), nChar, 1, 0 );
		break;
	case CRED_PN:
		if( ! lastBrnInpStr(string, tmp, sizeof tmp) )
			return;
		if( CheckDataIcsinputCred( this, nChar, tmp) <= 0 )
			return;
		TerminationIcsinputCred( this, get_nowln(), nChar, 1, 0 );
		break;
	case TKY_PN:
		term = 1;
		len = get_strcnt( (char*)string, Voln1->tk_ln * 2 );
		memcpy( tmp, string, len );
		tmp[len] = '\0';

		memcpy( ystr, ystring, len );
		ystr[len] = '\0';

		if( (st = TekiyoStr_Chk( tmp )) > 0 ) {
			if( TekiyoKeyMode() ) {	// 摘要コード＋Enter で 摘要をセット
				if(	st == 1 ) {
					if( nChar == VK_RETURN ) {
						_KanaTermIcsinputTky( ystr, FALSE );
						term = 0;
					}
				}
				if( nChar == VK_TAB ) {
					if( ! isdigitstr( (char*)ystr ) ) {
						_KanaTermIcsinputTky( ystr, FALSE );
						term = 0;
					}
					else {
						if( TKtabMode() ) {
							_KanaTermIcsinputTky( ystr, TRUE );
							term = 0;
						}
					}
				}
				else if( nChar == VK_F12 ) {
					if( ! isdigitstr( (char*)ystr ) ) {
						_KanaTermIcsinputTky( ystr, FALSE );
						term = 0;
					}
				}
			}
			else {
				if( nChar == VK_TAB || nChar == VK_F12 ) {
					_KanaTermIcsinputTky( ystr, FALSE );
					term = 0;
				}
			}
		}

		if( term ) {
			var.pbVal = (BYTE*)tmp;
			pos = ((CICSDBEDT*)GetDlgItem(ID))->GetCaretPosition();
			((CICSDBEDT*)GetDlgItem(ID))->InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );
		//	TerminationIcsinputTky( this, get_nowln(), nChar );
		}
		break;
	case SYON_PN:
		break;
	default:
		ASSERT(FALSE);
		break;
	}

//TRACE( "**ImeEnd %d, %s, %s\n", nChar, string, ystring );	
}

//--------------------------------------------------------
//	ICSDBEDT のターミネーション処理
//		関数の最後の END_TAG 部分を必ず実行すること
//		CDataListCtrl に 入力ｳｨﾝﾄﾞｳの状態を知らせるため
//--------------------------------------------------------
void CDBDinpView::_OnTerminationIcsdbedtctrl( UINT ID, long nChar, long inplen, long kst )
{
//MyTrace( "--CDBDinpView::_OnTerm ID %d nChar %04x, inplen %d, pok %d, ln %d, pn %d, ed_pn %d\n", 
//				ID, nChar, inplen, PROGRAM_OK, get_nowln(), get_nowpn(), EDIT_PN );

	int ln, pn;

	if( PROGRAM_OK == FALSE )
		goto END_TAG;

	ln = get_nowln();
	if( ln == -1 )	goto END_TAG;
	pn = get_nowpn();

	// エディットボックスの項目設定状態と異なる場合、AutoInp()中などは、
	// KillFocus等 ターミネーションはさせない
	if( EDIT_PN != pn ) {
		goto END_TAG;
	}

	if( kst & ICSDBEDT_KST_SHIFT ) {
		if( nChar == VK_TAB )	nChar = VK_LEFT;
	}

	switch( pn )
	{
	case SEQ_PN:
		TerminationIcsinputSeq( this, nChar, inplen, kst ); 
		break;
	case DATE_PN:
		if( inplen > 0 || nChar == 0x16 ) {	// 貼り付け時もチェックする
			if( CheckDataIcsinputDate( this, nChar ) < 0 )
				goto END_TAG;
		}
		TerminationIcsinputDate( this, nChar, inplen, kst ); 
		break;
	case BMON_PN:
		TerminationIcsinputBumon( this, 0, get_nowln(), nChar, inplen, kst );
		break;
	case CBMN_PN:
		TerminationIcsinputBumon( this, 1, get_nowln(), nChar, inplen, kst );
		break;
	case DENP_PN:
		TerminationIcsinputDenp( this, get_nowln(), nChar, inplen, kst ); 
		break;
	case DEBT_PN:
		if( nChar != VK_DELETE )
			TerminationIcsinputDebt( this, get_nowln(), nChar, inplen, kst );
		break;
	case CRED_PN:
		if( nChar != VK_DELETE )
			TerminationIcsinputCred( this, get_nowln(), nChar, inplen, kst );
		break;
	case KOJI_PN:
		TerminationIcsinputKoji( this, 0, get_nowln(), nChar, inplen, kst );
		break;
	case CKJI_PN:
		TerminationIcsinputKoji( this, 1, get_nowln(), nChar, inplen, kst );
		break;
	case VAL_PN:
		TerminationIcsinputVal( this, get_nowln(), nChar, inplen, kst );
		break;
	case ZEI_PN:
		TerminationIcsinputZei( this, get_nowln(), nChar, inplen, kst );
		break;
	case SKBN_PN:
		TerminationIcsinputSyz( this, get_nowln(), nChar, inplen, kst );
		break;
	case HJIYU_PN:
		OnHjiyuInEnd( nChar, 0 );
		break;
	case TKY_PN:
		TerminationIcsinputTky( this, get_nowln(), nChar, inplen, kst );
		break;
	case SYON_PN:
		TerminationIcsinputSyono( this, get_nowln(), nChar, inplen, kst );
		break;
	case TGDT_PN:
		if( inplen > 0 || nChar == 0x16 ) {	// 貼り付け時もチェックする
			if( CheckDataIcsinputTgDate( this, nChar ) <= 0 )
				return;
		}
		TerminationIcsinputTgDate( this, get_nowln(), nChar, inplen, kst );
		break;
	case DOCEVI_PN:
		TerminationIcsinputDocEviNo( this, get_nowln(), nChar, inplen, kst );
		break;
	default:
//		ASSERT( FALSE );
		break;
	}

	if( nChar == 0 ) {
//TRACE( "m_Input.ShowWindow(SW_HIDE)\n" );
//		m_Input.ShowWindow(SW_HIDE);
//		m_Input.PostMessage( WM_SHOWWINDOW, FALSE, 0 );
	}


END_TAG:

	SendEndLabelEdit( ID, nChar );

	//マルチウィンドウ時の金額欄で、ウィンドウサイズ変更するとEnter押下で消費税額が0になる 162412
	if( EDIT_PN == -10 ) {
		DB_SyIniz(&CREC);
		EDIT_PN = VAL_PN;
	}
//TRACE( "-- _OnTerm End %d\n", ID );
}


void CDBDinpView::SendEndLabelEdit( UINT ID, long nChar ) 
{
	DT_EDIT edit;

	if( ID == IDC_ICSDBEDTCTRL1 )
		edit.hdr.hwndFrom = m_Input.GetSafeHwnd();
	else
		edit.hdr.hwndFrom = m_Inp2.GetSafeHwnd();
	edit.hdr.idFrom   = ID;
	edit.hdr.code     = DLN_ENDLABELEDIT;

	edit.nChar = nChar;

	if( ID == IDC_ICSDBEDTCTRL1 )
		m_DtLine.SendMessage(WM_DTLISTUSER, DLN_ENDLABELEDIT, (LPARAM)&edit );
	else
		m_DtList.SendMessage(WM_DTLISTUSER, DLN_ENDLABELEDIT, (LPARAM)&edit );

}

// コントロールよりデータ取得
int CDBDinpView::_GetDataList_Data( int ln, DTLIST_DATA* pData ) 
{
	int ret;

	if( ln == APPEND_LINE ) {
		ret = m_DtLine.GetDtListData( 0, pData );
	}
	else {
		ret = m_DtList.GetDtListData( ln-1, pData );
	}

	return ret;
}

// コントロールにデータセット
void CDBDinpView::_SetDataList_Data( int ln, DTLIST_DATA* pData, BOOL bRedraw/*= TRUE*/ ) 
{
	if( ln == APPEND_LINE ) {
		m_DtLine.SetDtListData( 0, pData, bRedraw );
	}
	else {
		m_DtList.SetDtListData( ln-1, pData, bRedraw );
	}
}

//--------------------------------------------------
//	摘要イメージ部分の 修正を行わない
//
//--------------------------------------------------
void CDBDinpView::_UpdateDataList_Data( int ln, DTLIST_DATA* pData, BOOL bRedraw/*= TRUE*/ ) 
{
	if( ln == APPEND_LINE ) {
		m_DtLine.UpdateDtListData( 0, pData, bRedraw );
	}
	else {
		m_DtList.UpdateDtListData( ln-1, pData, bRedraw );
	}
}


// ターミネーション時のデータセット
void CDBDinpView::TerminationDataSet( int pn ) 
{

	DTLIST_DATA data;
	int ln = get_nowln();

	_GetDataList_Data( ln, &data );
	
	struct _DATA_LINE *dtline;
	dtline = _getLINE_DATA( ln );

	_TerminationDataSet( &data, dtline, &pn, 1 );

	// 摘要以外の更新は、イメージを変更しない
	if( pn != TKY_PN )
		_UpdateDataList_Data( ln, &data );
	else
		_SetDataList_Data( ln, &data );
}

// ターミネーション時のデータセット 複数項目
void CDBDinpView::TerminationDataSet( int pn[], int pncnt ) 
{

	DTLIST_DATA data;
	int ln = get_nowln();

	_GetDataList_Data( ln, &data );
	
	struct _DATA_LINE *dtline;
	dtline = _getLINE_DATA( ln );

	_TerminationDataSet( &data, dtline, pn, pncnt );

	BOOL bTekiyoPN = FALSE;

	for( int i = 0; i < pncnt; i++ ) {
		if( pn[i] == TKY_PN ) {
			bTekiyoPN = TRUE;
		}
	}
	// 摘要以外の更新は、イメージを変更しない
	if( ! bTekiyoPN )
		_UpdateDataList_Data( ln, &data );
	else
		_SetDataList_Data( ln, &data );
}


void CDBDinpView::_TerminationDataSet( DTLIST_DATA* data, struct _DATA_LINE *dtline, int pntbl[], int tblcnt ) 
{
//	BYTE ksgn;
	int pn, sg;
	int ln = get_nowln();

	for( int i = 0; i < tblcnt; i++ ) {
		pn = pntbl[i];
		data->dd_ctbl[pn] = dtline->_XXDTA.dd_ctbl[pn];

		if( ln != APPEND_LINE ) {
			if( ! BmonKoji_DebCre_Inp(ln) ) {
				if( pn == BMON_PN ) {
					if( ! data->dd_cbmn_etc.IsEmpty() && data->dd_dbmn_etc.IsEmpty() ) {
						data->dd_ctbl[BMON_PN].text = data->dd_ctbl[CBMN_PN].text;
					}
				}
				else if( pn == KOJI_PN ) {
					if( ! data->dd_ckno_etc.IsEmpty() && data->dd_dkno_etc.IsEmpty() ) {
						data->dd_ctbl[KOJI_PN].text = data->dd_ctbl[CKJI_PN].text;
					}
				}
			}
		}

		switch( pn ) {
		case SEQ_PN:
			// 選択部分
			data->dd_dsign = dtline->_XXDTA.dd_dsign;
		case DATE_PN:
			data->dd_ksign = 0;
			data->dd_mofs = 0;
			if( dtline->LINE_CND[DATE_PN].INP_sg ) {
				data->dd_ddate	= dtline->_XXDTA.dd_ddate;
				data->dd_mofs	= dtline->_XXDTA.dd_mofs;
				data->dd_ksign	= dtline->_XXDTA.dd_ksign;

#ifdef OLD_CLOSE
				if( data->dd_ddate > 0 ) {
					ksgn = KESY_SG_GET( dtline->BcdDate[0] );
					data->dd_ksign = !ksgn ? 0 : 1;
					BYTE tmp[10] = {0};
					memcpy( tmp, dtline->BcdDate, 2 );

					CVolDateDB vd;
					int offset;
				//	if( ksgn > 0 )	tmp[0] = (ksgn==1) ? 0x00 : 0x60;
					switch( ksgn ) {
					case 1: tmp[0] = 0x61;	break;
					case 2: tmp[0] = 0x62;	break;
					case 3: tmp[0] = 0x63;	break;
					case 4: tmp[0] = 0x00;	break;
					default:
						break;
					}
					if( vd.db_vd_offset( tmp, &offset, pDBzm ) != 0 ) {
//FILETrace( "@_TerminationDataSet tmp %02x%02x\n", tmp[0], tmp[1] );
						ICSMessageBox( "月のオフセットを取得できません！ ");
						return;
					}
					data->dd_mofs = offset;
				}
#endif
			}
			else {
				data->dd_ddate = 0;
			}
			break;
		case DENP_PN:
			data->dd_cno = dtline->_XXDTA.dd_cno;
			break;
		case BMON_PN:
			data->dd_dbmn = dtline->_XXDTA.dd_dbmn;
			data->dd_dbmn_etc = dtline->_XXDTA.dd_dbmn_etc;
			break;
		case KOJI_PN:
			data->dd_dkno = dtline->_XXDTA.dd_dkno;
			data->dd_dkno_etc = dtline->_XXDTA.dd_dkno_etc;
			break;
		case DEBT_PN:
			data->dd_dbt = dtline->_XXDTA.dd_dbt;
			data->dd_dbr = dtline->_XXDTA.dd_dbr;
			data->dd_deb_name.Format( "%.14s", dtline->_XXDTA.dd_deb_name );
			data->dd_deb_etc = dtline->_XXDTA.dd_deb_etc;
			break;
		case CBMN_PN:
			data->dd_cbmn = dtline->_XXDTA.dd_cbmn;
			data->dd_cbmn_etc = dtline->_XXDTA.dd_cbmn_etc;
			break;
		case CKJI_PN:
			data->dd_ckno = dtline->_XXDTA.dd_ckno;
			data->dd_ckno_etc = dtline->_XXDTA.dd_ckno_etc;
			break;
		case CRED_PN:
			data->dd_cre = dtline->_XXDTA.dd_cre;
			data->dd_cbr = dtline->_XXDTA.dd_cbr;
			data->dd_cre_name.Format( "%.14s", dtline->_XXDTA.dd_cre_name );
			data->dd_cre_etc = dtline->_XXDTA.dd_cre_etc;
			break;
		case VAL_PN:
			memcpy( data->dd_val, dtline->_XXDTA.dd_val, 6 );
			data->val_flag = dtline->LINE_CND[VAL_PN].INP_sg;
			break;
		case ZEI_PN:

			memcpy( data->dd_zei, dtline->_XXDTA.dd_zei, 6 );
			data->zei_flag = dtline->LINE_CND[ZEI_PN].INP_sg;
			/* 非課税有価証券譲渡仕訳 */
			if( pDBsy->IsTaikaData( pDBzm->dbdata ) == 2)
					data->zei_flag |= 0x10;
			else	data->zei_flag &= ~0x10;

			dtline->_XXDTA.zei_flag = data->zei_flag;
			
			COLORREF back;
			if (isnot_defzeigaku(pDBzm->dbdata, back)) {
				dtline->_XXDTA.dd_ctbl[ZEI_PN].back = back;
				data->dd_ctbl[ZEI_PN].back = back;
			}
			else {
				dtline->_XXDTA.dd_ctbl[ZEI_PN].back = (COLORREF)-1;
				data->dd_ctbl[ZEI_PN].back = (COLORREF)-1;
			}

			break;
		case SKBN_PN:
			data->dd_syz_str = dtline->_XXDTA.dd_syz_str;
			sg = SYZMDFY_BIT.bitncnd( data->dd_seq - 1);

			if( sg > 0 ) {
				dtline->_XXDTA.dd_ctbl[SKBN_PN].back = RGB_SYZ_MDFY;
				data->dd_ctbl[SKBN_PN].back = RGB_SYZ_MDFY;
			}
			else {
				dtline->_XXDTA.dd_ctbl[SKBN_PN].back = (COLORREF)-1;
				data->dd_ctbl[SKBN_PN].back = (COLORREF)-1;
			}
			break;
		case TKY_PN:
			if( dtline->LINE_CND[ TKY_PN ].IMG_sg == 1 && dtline->ImgSEQ > 0 ) {
				data->dd_tek.tekiyo = "";
			//	CString filter;
			//	filter.Format( "iseq = '%d'", dtline->ImgSEQ );
			//	if( pDBzm->image->Requery( filter ) == 0 )
				if( pDBzm->image->SetAbsolutePosition( dtline->ImgSEQ ) == 0 )
				{
					IMREC* pIMREC = pDBzm->image->DecodeImage();
					if( pIMREC != NULL ) {
						data->dd_tek.width = pIMREC->dotxs;
						data->dd_tek.height = pIMREC->dotys;
						data->dd_tek.image = (char*)pIMREC->imdata;
					}
					else {
						data->dd_tek.tekiyo = Image;
						data->dd_tek.image = 0;
						data->dd_tek.width = 0;
						data->dd_tek.height = 0;
					}
				}
				else {
					data->dd_tek.tekiyo = Image;
					data->dd_tek.image = 0;
					data->dd_tek.width = 0;
					data->dd_tek.height = 0;
				}
			}
			else {
				data->dd_tek.tekiyo = dtline->_XXDTA.dd_tek.tekiyo;
				data->dd_tek.image = 0;
				data->dd_tek.width = 0;
				data->dd_tek.height = 0;
			}
			break;
		case SYON_PN:
			data->dd_snumber = dtline->_XXDTA.dd_snumber;
			break;
		case TGDT_PN:
			data->dd_tdate = dtline->_XXDTA.dd_tdate;
			IntsrekitoGenstr( data->dd_tdstr.GetBuffer(30), 30, data->dd_tdate );
			data->dd_tdstr.ReleaseBuffer();
			break;

		case SKIN_PN:

			break;
		case HSEN_PN:
			data->dd_husen		= dtline->_XXDTA.dd_husen;
			data->dd_hscomment	= dtline->_XXDTA.dd_hscomment;
			break;

		case HJIYU_PN:
			data->dd_hjiyu = dtline->_XXDTA.dd_hjiyu;
			break;

		case DOCEVI_PN:
			data->dd_docevi = dtline->_XXDTA.dd_docevi;
			break;
		}
	}
}

// ダイレクトターミネーション
//
//	int off_type	nChar ゼロ のときの、動作タイプ
//
void CDBDinpView::TermDistribute( long nChar, int off_type/* = 0*/ )
{
//	return;
//TRACE( "@@@ TermDistribute %d\n", nChar );

	if( off_type == 0 ) {
		if( nChar == 0 ) {	// DBEdit から フォーカスをはずす。
			m_DtList.SetFocus();
			return;
		}
	}

	switch( get_nowpn() )
	{
	case SEQ_PN:	//番号
		TerminationIcsinputSeq( this, nChar, 0, 0 );
		break;
	case DATE_PN:	//日付
		TerminationIcsinputDate( this, nChar, 0, 0 );
		break;
	case DENP_PN:	//伝票番号
		TerminationIcsinputDenp( this, get_nowln(), nChar, 0, 0 );
		break;
	case BMON_PN:	//借方部門
		TerminationIcsinputBumon( this, 0, get_nowln(), nChar, 0, 0 );
		break;
	case CBMN_PN:	//貸方部門
		TerminationIcsinputBumon( this, 1, get_nowln(), nChar, 0, 0 );
		break;
	case KOJI_PN:	//借方工事
		TerminationIcsinputKoji( this, 0, get_nowln(), nChar, 0, 0 );
		break;
	case CKJI_PN:	//貸方工事
		TerminationIcsinputKoji( this, 1, get_nowln(), nChar, 0, 0 );
		break;
	case DEBT_PN:	//借方
		TerminationIcsinputDebt( this, get_nowln(), nChar, 0, 0 );
		break;
	case CRED_PN:	//貸方
		TerminationIcsinputCred( this, get_nowln(), nChar, 0, 0 );
		break;
	case VAL_PN:	//金額
		TerminationIcsinputVal( this, get_nowln(), nChar, 0, 0 );
		break;
	case ZEI_PN:	//税額
		TerminationIcsinputZei( this, get_nowln(), nChar, 0, 0 );
		break;		
	case SKBN_PN:	//消費税
		TerminationIcsinputSyz( this, get_nowln(), nChar, 0, 0 );
		break;
	case TKY_PN:	//摘要
		TerminationIcsinputTky( this, get_nowln(), nChar, 0, 0 );
		break;

	case HJIYU_PN:	//変動事由
		OnHjiyuInEnd( nChar,0 );
		break;
	case SYON_PN:	//手形番号
		TerminationIcsinputSyono( this, get_nowln(), nChar, 0, 0 );
		break;
	case TGDT_PN:	//手形期日
		TerminationIcsinputTgDate( this, get_nowln(), nChar, 0, 0 );
		break;
	case DOCEVI_PN:	//証憑
		TerminationIcsinputDocEviNo( this, get_nowln(), nChar, 0, 0 );
		break;

	case HSEN_PN:	//付箋
		OnHusenInpMsg( MAKELONG( 0, VK_END ), 0 );
		break;
	}
}


#ifndef _SLIP_ACCOUNTS_
void CDBDinpView::OnButtonF9() 
{

	try
	{
		FNC_BUTTON = TRUE;
		{
			PROGRAM_OK = FALSE;
			totaldisp( MODE_NORMAL );
			PROGRAM_OK = TRUE;
		}
		FNC_BUTTON = FALSE;
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}
#else
void CDBDinpView::OnButtonF9() 
{
	// 電卓ウィンドウ消去
	CalcMouseEvent();

	if( IsDispSlip() == FALSE ){
		try
		{
			FNC_BUTTON = TRUE;
			{
				PROGRAM_OK = FALSE;
				totaldisp( MODE_NORMAL );
				PROGRAM_OK = TRUE;
			}
			FNC_BUTTON = FALSE;
		}
		catch ( CErrBlk E )
		{
			ErrExit( E.ErrCode, E.ErrMsg );
			return;
		}
	}
	else{
/*- '12.09.22 -*/
//		OnBnClickedLinkslipBtn();
/*-------------*/
/*-- '12.10.03 --*/
//		if( (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_DO_LINKIMG) ){
/*---------------*/
		if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
/*---------------*/
			OnBnClickedLinkslipBtn();
		}
/*-------------*/
	}
}
#endif

#ifndef _SLIP_ACCOUNTS_
void CDBDinpView::OnUpdateButtonF9(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	if( !PROGRAM_OK )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
	if( bIKAT_SCAN )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
}
#else
void CDBDinpView::OnUpdateButtonF9(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	if( IsDispSlip() ){
		pCmdUI->Enable( TRUE );
		return;
	}

	if( !PROGRAM_OK )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
	if( bIKAT_SCAN )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
}
#endif

//--------------------------------------------------
//合計問い合わせ
//	mode	MODE_NORMAL		通常のF9での表示
//			MODE_END		終了時の表示
//			MODE_COSEL		会社再選択での表示
//
//--------------------------------------------------
int CDBDinpView::totaldisp( int mode )
{
	int rt;
	int n;

	CWaitCursor wc;

	n = (INP_mode == _KEYSCAN && mode==MODE_NORMAL ? 1 : 2 );

	// 取消検索
	if( pDBzm->IsDelScan() ) {
		if( n == 1 )	n = 2;
	}

	do
	{
		if( n == 1 )
		{
			if( !m_TaiTtl.IsEndCalq() ) {

				if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() == INP_VIEW && INP_mode == _KEYSCAN ) {
					if( DATA_1RD ) {
						//CREC to 残高ADD.
						rewupdat(&CREC, &SCAN_rec, INP_mode);
						reupdat(&CREC);
					}
				}
			}

			if( !(m_pTaiDsp = new CTaiDsp) )
			{
				myICSMessageBox( _T("検索合計オブジェクトが作成できませんでした。") );
				return -1;
			}

			if( ! m_TaiTtl.IsEndCalq() ) {
				//ここに来るまでに、仕訳を変更された場合があるので、金額初期化
				m_TaiTtl.taidata_inz(-1);

				pDBzm->dbdata->MoveFirst();
				CDBINPDataRec data;

				do {
#ifdef _SPEED_CLOSE
					pDBzm->DBDATA_SetPosition( pDBzm->data->seq );
#endif
					pDBzm->GetCDBData( &data );
					if( data.m_seq > 0 ) {
						m_TaiTtl.add_taidata( &data );		// 貸借データへ加算
					}
				} while( pDBzm->dbdata->MoveNext() == 0 ) ;

				// 科目テーブルのソート
				m_TaiTtl.kmktbl_sort();

				m_TaiTtl.end_taiclq();
			}

			m_pTaiDsp->set_alt( IDALT, "各月合計(F9)>>" );
			m_pTaiDsp->dsp_data( m_TaiTtl.get_taidata() );
			m_pTaiDsp->tai_dsp( mode );

			((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(2);
			
			rt = m_pTaiDsp->DoModal();

			((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(0);
			delete m_pTaiDsp;
			m_pTaiDsp = 0;
		
		}
		else if( n == 2 )
		{
			//イニシャライズ&表示
			class CTtlDsp ttl;
			
			((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(2);
			//取消検索でない スキャンの 表示
			if( ! pDBzm->IsDelScan() ) {
				if( INP_mode == _KEYSCAN )
					ttl.set_alt( IDALT, "検索合計(F9)>>" );
			}

			if( ttl.ttl_dsp( mode ) != -1 )
				rt = ttl.DoModal();
			else
				rt = -1;

			((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(0);
		}
		else
			rt = -1;

		if( rt == IDALT )
			n = n == 1 ? 2 : 1;
	}while( rt == IDALT );

	return rt;


return 0;
}



HWND CDBDinpView::GetKamokuCtrlHWND()
{
	UINT ID;
	int pn = get_nowpn();
	ID = getINP_CTL(pn)->IDC_X;

	return GetDlgItem(ID)->m_hWnd;
}


BOOL CDBDinpView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
#ifdef _SLIP_ACCOUNTS_
	m_tooltip.RelayEvent( pMsg );
#endif

#ifdef CLOSE_DEBUG
	if( pMsg->message == WM_MOUSEMOVE ) {
		CWnd* pwnd;
		pwnd = GetFocus();
		if( pwnd && IsWindow(pwnd->m_hWnd) ) {
			int id = pwnd->GetDlgCtrlID();
			BOOL b = pwnd->IsWindowVisible();
			CString msg;
			msg.Format("DBDinp Now Focus Wnd ID = %d, visible %d", id, b);
			((CMainFrame*)GetDBDinpMainFrame())->err_statusdsp(msg);

			CWnd*pwnd = GetFocus();
			CWnd*pwnd2 = GetCapture();
			ONSIZETrace("@Pretrans pwnd = %08x, pwnd2 %08x\n", pwnd, pwnd2);
			ONSIZETrace("@Pretrans m_Input = %08x\n", m_Input.m_hWnd);
			if( pwnd ) {
				ONSIZETrace("@Pretrans Focus = %08x\n", pwnd->m_hWnd);
				ONSIZETrace("@Pretrans FocusParent = %08x\n", pwnd->GetParent()->m_hWnd);
			}
			if( pwnd2 ) {
				ONSIZETrace("@Pretrans Capture %08x\n", pwnd2->m_hWnd);
				ONSIZETrace("@Pretrans CaptureParent = %08x\n", pwnd2->GetParent()->m_hWnd);
			}
		}
	}
#endif
	int pn;

	if( PROGRAM_OK /*&& pMsg->message != WM_PAINT*/ )
	{

#ifdef REGDATA	// 07.28 /00
		if( pMsg->message == WM_KEYDOWN ) {
			if( pMsg->wParam == VK_INSERT )
			{
				PostMessage( WM_COMMAND, MAKELONG(ID_BUTTON_INSERT, 0), 0 );
				return 1;
			}
		}
#endif
		if( pMsg->message == WM_KEYUP ) {
			int pn = get_nowpn();
			if( pn == DEBT_PN || pn == CRED_PN ) {
		//		TRACE("Kana ● pMsg->message = %08x, WP = %08x\n", pMsg->message, pMsg->wParam);
				if( FukuKanaSearch(pMsg) )
					return 1;
			}
		}

#ifdef _SLIP_ACCOUNTS_
		if( pMsg->message == WM_KEYDOWN ){

			pn = get_nowpn();

			if( pn == DEBT_PN || pn == CRED_PN ) {
				if( KanaCheckVK_BACK(pMsg) )
					return 1;
			}
			int	mkSt = 0;
			if( pMsg->wParam == VK_HOME ){
				if( (GetKeyState(VK_CONTROL)&0x8000) ){
#ifdef CLOSE_826	//イメージ表示後、別行に移行すると不正終了。マウス操作ではOKなので、マウス操作をエミュレート
					if( m_DispSlipChk.GetCheck() == 1 ){
						m_DispSlipChk.SetCheck( 0 );
					}
					else{
						m_DispSlipChk.SetCheck( 1 );
					}
					PostMessage( WM_COMMAND, IDC_DISPSLIP_CHK );
#endif

					CPoint nowPt;
					::GetCursorPos(&nowPt);
					CRect rect;
					GetDlgItem(IDC_DISPSLIP_CHK)->GetWindowRect(&rect);
					int x = (rect.left + rect.Width()/2);
					int y = (rect.top + rect.Height()/2);
				//	'実際にマウスを移動させる為の補正値を求める
				//	'画面の解像度を取得
					int smx, smy, posx, posy;
				//	'画面解像度の取得
					smx = GetSystemMetrics(SM_CXSCREEN);
					smy = GetSystemMetrics(SM_CYSCREEN);
				//	'マウスの移動量を計算(絶対位置)
					posx = x * (65535 / smx);
					posy = y * (65535 / smy);
					mouse_event( MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE, posx,posy,0,0 );
					mouse_event( MOUSEEVENTF_LEFTDOWN, 0,0,0,0 );
					mouse_event( MOUSEEVENTF_LEFTUP, 0,0,0,0 );
					//元の位置に戻す
					posx = nowPt.x * (65535 / smx);
					posy = nowPt.y * (65535 / smy);
					mouse_event( MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE, posx,posy,0,0 );
					return 1;
				}
				else if( (GetKeyState(VK_SHIFT) & 0x8000) ) {
					//原票読取
					if( m_GnpScanBtn.IsWindowVisible() ) {
						CPoint nowPt;
						::GetCursorPos(&nowPt);
						CRect rect;
						GetDlgItem(IDC_GNPSCAN_BTN)->GetWindowRect(&rect);
						int x = (rect.left + rect.Width() / 2);
						int y = (rect.top + rect.Height() / 2);
						//	'実際にマウスを移動させる為の補正値を求める
						//	'画面の解像度を取得
						int smx, smy, posx, posy;
						//	'画面解像度の取得
						smx = GetSystemMetrics(SM_CXSCREEN);
						smy = GetSystemMetrics(SM_CYSCREEN);
						//	'マウスの移動量を計算(絶対位置)
						posx = x * (65535 / smx);
						posy = y * (65535 / smy);
						mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, posx, posy, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						//元の位置に戻す
						posx = nowPt.x * (65535 / smx);
						posy = nowPt.y * (65535 / smy);
						mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, posx, posy, 0, 0);


					//	PostMessage(WM_COMMAND, MAKELONG(IDC_GNPSCAN_BTN, BN_CLICKED), (LPARAM)m_GnpScanBtn.m_hWnd);
						return 1;
					}
				}
			}
			else if( pMsg->wParam == VK_DOWN ){
				if( (pn==TKY_PN) || (pn==DATE_PN) || (pn==VAL_PN) ){
					if( IsDispSlip() ){
						if( GetKeyState(VK_CONTROL)&0x8000 ){
							m_pImgmng->MoveSelectText( +1, mkSt );
							if( mkSt ){
								// フォーカス再セット
								set_focus( get_nowpn() );
							}
							return 1;
						}
					}
				}
			}
			else if( pMsg->wParam == VK_UP ){
				if( (pn==TKY_PN) || (pn==DATE_PN) || (pn==VAL_PN) ){
					if( IsDispSlip() ){
						if( GetKeyState(VK_CONTROL)&0x8000 ){
							m_pImgmng->MoveSelectText( -1, mkSt );
							if( mkSt ){
								// フォーカス再セット
								set_focus( get_nowpn() );
							}
							return 1;
						}
					}
				}
			}
			else if( pMsg->wParam == VK_PROCESSKEY ){
				BmnIMEkeydownJob( pMsg->wParam );
			}
		}
#endif
		//選択窓を表示していたら ESCAPE で非表示
		if( pMsg->message == WM_KEYDOWN ) {
			if( pMsg->wParam == VK_ESCAPE )
			{
				if( pAUTOSEL->BLKSEL_OPT && m_pSelWnd != NULL ) {
					if( m_pSelWnd->IsWindowVisible() ) {
						m_pSelWnd->ShowWindow( SW_HIDE );
						return 1;
					}
				}
			}
		}
		if( pMsg->message == WM_CHAR ) {
			pn = get_nowpn();

			if( pn == ZEI_PN ) {
				//インボイス外税免税は、税額常に修正
				if( ! is_sotomensiwake(&CREC)) {
					if( !is_syohizeisiwake(&CREC) || is_yusyutsusiwake(&CREC) )
						return TRUE;
					else if( pAUTOSEL->ZEIMDFY_OPT != OPT_ZEIMDFY )	// 税額修正不可
						return TRUE;
				}
			}
		//	else if( pn == SYON_PN || pn == TGDT_PN ) {
		//		if( ! IsBillData( CREC.m_dbt, CREC.m_cre ) )
		//			return TRUE;
		//	}
			else if( pn == HJIYU_PN ) {
				if( ! IsEnableHjiyu( &CREC ) )
					return TRUE;
			}
		}

		if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F10) {
			pn = get_nowpn();
			if( pn == VAL_PN ) {
				if( ChangeZeiritsu() ) {
				}
				return TRUE;
			}
			else if( pn == ZEI_PN ) {
				if( ChangeSotoMenZei() ) {
				}
				return TRUE;
			}
		}
		if( pMsg->message == WM_LBUTTONDOWN ) {
			pn = get_nowpn();
			if( pn == SKBN_PN ) {
				m_pSyzInp->CancelInvNoIME();
			}
		}
	}
	return CDBbaseView::PreTranslateMessage(pMsg);
}

// ICS Select からの メッセージハンドラ
void CDBDinpView::OnSelectedIcsselctrl1(short position, long Number) 
{
	try
	{
		SelectedIcsselctrl( position, Number, 0);
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
	
}

void CDBDinpView::OnSelectedIcsselctrl2(short position, long Number) 
{
	try
	{
		SelectedIcsselctrl( position, Number, 1);
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}


//枝番名称ウィンドウの更新
void CDBDinpView::SelectedEtcUpdate(int pn) 
{
	struct _DATA_LINE *ldata = getLINE_DATA();

	int ln = get_nowln();

	CICSDBEDT* pDBed;
	CDataListCtrl* pListCtrl;
	if( ln == APPEND_LINE ) {
		pDBed = &m_Input;
		pListCtrl = &m_DtLine;
	}
	else {
		pDBed = &m_Inp2;
		pListCtrl = &m_DtList;
	}

	CString* pEtc = NULL;
	COLORREF etccol;
	etccol = ldata->LINE_CND[pn].ETC_TXT_ForeCOL;

	CRect rect;
	pDBed->GetWindowRect(&rect);
	pListCtrl->ScreenToClient(&rect);

	switch( pn ) {
	case DEBT_PN:
		pEtc = &ldata->_XXDTA.dd_deb_etc;
		break;
	case CRED_PN:
		pEtc = &ldata->_XXDTA.dd_cre_etc;
		break;
	case BMON_PN:
		pEtc = &ldata->_XXDTA.dd_dbmn_etc;
		break;
	case CBMN_PN:
		pEtc = &ldata->_XXDTA.dd_cbmn_etc;
		break;
	case KOJI_PN:
		pEtc = &ldata->_XXDTA.dd_dkno_etc;
		break;
	case CKJI_PN:
		pEtc = &ldata->_XXDTA.dd_ckno_etc;
		break;
	}
	if( pEtc != NULL && !pEtc->IsEmpty() )
		pListCtrl->EtcStaticDisp(rect, *pEtc, etccol);
}



/*=======================
	FIX 8科目選択
	科目・摘要32項目選択
		本体
========================*/
void CDBDinpView::SelectedIcsselctrl(short position, long Number, int place ) 
{
	int dc_sw;
	CDBipTKREC tkrec;
	int dcsw, pn;
	struct _KamokuData check={0}, ans={0};

	if( bCONFIRM_MASTER )	return;

	CString temp;
//TRACE( "**SelectedIcsselctrl\n" );

	if( m_SelDispPN == BVW_SYZ_PN && place )
	{
		if( m_pSyzInp )
		{
#ifdef	_ICSPEN_	// 01.20 /01
			int sw = m_pSyzInp->SetSkbnSelNo( Number );	// 消費税区分選択番号受付

			//選択後次へ移動
			if( IsPen() != FALSE && !sw )	m_pSyzInp->set_nextpos();
			else							m_pSyzInp->set_nowfocus();
#else
			m_pSyzInp->SetSkbnSelNo( Number );	// 消費税区分選択番号受付

			m_pSyzInp->set_nowfocus();
#endif
		}
		return;
	}

	pn = get_nowpn();

	if( INP_mode == _RETREV )	return;
	
	if( dinp_modefy_ok( getLINE_DATA() ) )
	{
		if( pn == DEBT_PN || pn == CRED_PN )
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

			dcsw  = ((pn == DEBT_PN) ? 0 : 1 );

			struct _DATA_LINE* ldata;
			ldata = getLINE_DATA();
		//	check = ( !dcsw ? getLINE_DATA()->DEBT_DATA : getLINE_DATA()->CRED_DATA );

			if( ! dcsw ) {
				sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", (LPCTSTR)ldata->_XXDTA.dd_deb_name );
				memcpy( check.kd_code, ldata->_XXDTA.dd_dbt, KMKCD_LN );
				check.kd_eda = ldata->_XXDTA.dd_dbr;
			}
			else {
				sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", (LPCTSTR)ldata->_XXDTA.dd_cre_name );
				memcpy( check.kd_code, ldata->_XXDTA.dd_cre, KMKCD_LN );
				check.kd_eda = ldata->_XXDTA.dd_cbr;
			}

			// 科目ｺｰﾄﾞ or 番号 02.20/02
			if( pAUTOSEL->KMNO_OPT == OPT_KMNO )
				sprintf_s( check.kd_code, sizeof check.kd_code, "%d", position + (place ? 9 : 1) );
			else
				sprintf_s( check.kd_code, sizeof check.kd_code, "%d", Number );	// 番号

			check.kd_sgn = 1;
			check.kd_eda = -1;

			if( m_Seldata.InputItemCheck( ans, check, dcsw, TRUE ) == TRUE )
			{
//				data_get( getINP_CTL( !dcsw ? DEBT_PN : CRED_PN )->IDC_X, &check, CICSInput );
//				ans.IP_BRN = check.IP_BRN;
//				data_set( getINP_CTL( !dcsw ? DEBT_PN : CRED_PN )->IDC_X, &ans, CICSInput );
				
				exDBkamokudata_set(  !dcsw ? DEBT_PN : CRED_PN, getINP_CTL( this, !dcsw ? DEBT_PN : CRED_PN )->IDC_X, &ans );

				getLINE_DATA()->LINE_CND[pn].NEXT_MOVE_KEY = TRUE;

				if( INP_mode == _APPEND )
				{
					struct _DATA_LINE *ldata = getLINE_DATA();

					if( ldata->LINE_CND[ DEBT_PN ].INP_sg && ldata->LINE_CND[ CRED_PN ].INP_sg )
						TKattr = 0;
					if( !dcsw )
					{
						if( is_mdfybrnitem( ldata->_XXDTA.dd_dbt, ldata->_XXDTA.dd_dbr, ans.kd_code, ans.kd_eda ) )
							TKattr2 = 0;
					}
					else
					{
						if( is_mdfybrnitem( ldata->_XXDTA.dd_cre, ldata->_XXDTA.dd_cbr, ans.kd_code, ans.kd_eda ) )
							TKattr2 = 0;
					}
				}
#ifdef	_ICSPEN_	// 01.20 /01
				if( IsPen() != FALSE )
				{
					//選択後次へ移動
					GetDlgItem( getINP_CTL( this, pn )->IDC_X )->PostMessage( WM_KEYDOWN, VK_TAB );
//					if( !dcsw )	TerminationIcsinputctrl4( get_nowln(), VK_TAB );
//					else		TerminationIcsinputctrl5( get_nowln(), VK_TAB );
					return;
				}
				else
				{
					//データセットのためのダミーフォーカスＯＦＦ
					if( !dcsw )	TerminationIcsinputDebt( this, get_nowln(), 0, 0, 0 );
					else		TerminationIcsinputCred( this, get_nowln(), 0, 0, 0 );
				}
#else
				//データセットのためのダミーフォーカスＯＦＦ
				if( !dcsw )
				{
					TerminationIcsinputDebt( this, get_nowln(), 0 );
				}
				else
				{
					TerminationIcsinputCred( this, get_nowln(), 0 );
				}
#endif
				// 諸口枝番等再表示セット
				SelectedEtcUpdate(pn);
			}

			return;
		}
		else if( pn == TKY_PN && place )
		{
			if( m_pSubDtInp->IsDisplay() )
				return;

			//摘要選択
			temp.Format("%d", Number);
			PROGRAM_OK = FALSE;

			if( tky_select( &tkrec, (char *)(LPCTSTR)temp ) != -1 )
			{
				//摘要toデータ
				dc_sw = tky_select_to_drec( &CREC, &tkrec );
				//表示
				tky_select_dsp( &CREC, dc_sw, &tkrec, FALSE );
			}
			PROGRAM_OK = TRUE;
			return;
		}
		else if( (pn == BMON_PN || pn == CBMN_PN) && place )	// 部門選択
		{
			if( m_pBkInp == NULL ) {
				dcsw  = ((pn == BMON_PN) ? 0 : 1 );
				CString bmnname;
				if( bmncode_todata2( &CREC, Number, dcsw, &bmnname ) != FALSE ) {
					bmon_select_dsp( this, (pn==BMON_PN) ? 0 : 1, &CREC, TRUE );
					bmnkoji_to_tekiyo( &CREC, &bmnname );

					int dcup_sgn;
					if( !BmonKoji_DebCre_Inp(get_nowln()) ) dcup_sgn = -1;
					else {
						dcup_sgn = dcsw;
					}
					bmneda_update(&CREC, dcup_sgn);
				}
			}
			else {
				m_pBkInp->SelectedIcsselctrl( position, Number, place );
				return;
			}
		}
		else if( (pn == KOJI_PN || pn == CKJI_PN) && place )	// 工事選択
		{
			if( m_pBkInp == NULL ) {
				dcsw  = ((pn == KOJI_PN) ? 0 : 1 );
				CString kjnam;
				if( kojicode_todata( &CREC, &kjnam, Number, dcsw ) != FALSE ) {
					koji_select_dsp( this, (pn==KOJI_PN) ? 0 : 1, &CREC, TRUE );

					bmnkoji_to_tekiyo( &CREC, &kjnam, 1 );
				}
			}
			else {
				m_pBkInp->SelectedIcsselctrl( position, Number, place );
				return;
			}
		}
		else if( pn == HJIYU_PN ) {
			// 変動事由選択
			_hjiyu_select( Number );
		}
	}
	set_focus( get_nowpn() );
}


////データスキャンビュークラスアドレスセーブ
void CDBDinpView::save_scanview( CView *pn )
{
	if( !m_pScanView )
		m_pScanView = (CScanView*)pn;
}

#ifdef OLD_CLOSE
// スキャン時の ウィンドウ移動
void CDBDinpView::ScanMode_WndMove( BOOL bKeyScan, BOOL bOnSize/*=FALSE*/ )
{
FILETrace( "ScanMode_WndMove %d, onsize = %d\n", bKeyScan, bOnSize );

	CRect rect, rc, view_rect, tmp;
	int	id[] = { 
		IDC_ICSDISPCTRL10, IDC_ICSDISPRDREMAIN, 
		IDC_ICSDISPCTRL12, IDC_ICSDISPGENKIN, IDC_ICSDISPCTRL11, IDC_ICSDISPSYOGT, 0
	};

	GetClientRect( view_rect );

	int x, y;
	int list_ht;

	if( bKeyScan ) {
		GetDlgItem( IDC_DISPBIGIMG )->GetWindowRect( &rect );
		ScreenToClient( rect );

		GetDlgItem( IDC_MNTHSEL )->GetWindowRect( &tmp );
		ScreenToClient( tmp );
		GetDlgItem( IDC_MNTHSEL )->MoveWindow( tmp.left, rect.top - tmp.Height(), 
															tmp.Width(), tmp.Height()  );
#ifndef _SLIP_ACCOUNTS_
		y = rect.top - tmp.Height();
#else
		if( m_bSLIP_MASTER == FALSE ){
			y = rect.top - tmp.Height();
		}
		else{
			y = rect.top - (tmp.Height() * 2);
		}
#endif

		for( int i = 0; id[i] != 0; i++ ) {
			if( i <= 1 ) {
				GetDlgItem( id[i] )->GetWindowRect( &rc );
				ScreenToClient( rc );

				GetDlgItem( id[i] )->MoveWindow( rc.left, y, rc.Width(), rc.Height() );
				if( i == 0 )	x = rc.left + rc.Width() + 1;
			}
			else {
				// 横に移動
				GetDlgItem( id[i] )->GetWindowRect( &rc );
				ScreenToClient( rc );

				GetDlgItem( id[i] )->MoveWindow( x, y, rc.Width(), rc.Height() );
				x += rc.Width();
			}
		}

#ifdef _SLIP_ACCOUNTS_
		CRect	baseRect;
		GetDlgItem( IDC_ICSDISPCTRL10 )->GetWindowRect( &baseRect );
		ScreenToClient( baseRect );

		CRect	dspRect;
//		GetDlgItem( IDC_DISPSLIP_BTN )->GetWindowRect( &dspRect );
		GetDlgItem( IDC_DISPSLIP_CHK )->GetWindowRect( &dspRect );
		ScreenToClient( dspRect );
//		GetDlgItem( IDC_DISPSLIP_BTN )->MoveWindow( baseRect.left, baseRect.bottom + 2, 
		GetDlgItem( IDC_DISPSLIP_CHK )->MoveWindow( baseRect.left, baseRect.bottom + 2, 
															dspRect.Width(), dspRect.Height()  );
		CRect	lnkRect;
		GetDlgItem( IDC_DISPSLIP_CHK )->GetWindowRect( &lnkRect );
		ScreenToClient( lnkRect );
		GetDlgItem( IDC_LINKSLIP_BTN )->MoveWindow( baseRect.left + dspRect.Width() + 2, baseRect.bottom + 2, 
															lnkRect.Width(), lnkRect.Height()  );
#endif
	}
	else {
		// 入力時

		GetDlgItem( IDC_STATIC_POS )->GetWindowRect( &rect );
		ScreenToClient( rect );
		GetDlgItem( IDC_STATIC_POS2 )->GetWindowRect( &tmp );
		ScreenToClient( tmp );

FILETrace( "ScanMode_WndMove POS  (%d,%d,%d,%d)\n", rect.left, rect.top, rect.right, rect.bottom );
FILETrace( "ScanMode_WndMove POS2 (%d,%d,%d,%d)\n", tmp.left, tmp.top, tmp.right, tmp.bottom );

		for( int i = 0; id[i] != 0; i++ ) {
			GetDlgItem( id[i] )->GetWindowRect( &rc );
			ScreenToClient( rc );
			if( i <= 1 ) {
				y = rect.top;
				GetDlgItem( id[i] )->MoveWindow( rc.left, y, rc.Width(), rc.Height() );
				if( i == 0 )	x = rc.left;
			}
			else {
				GetDlgItem( id[i] )->MoveWindow( x, tmp.top, rc.Width(), rc.Height() );
				x += rc.Width();
			}
		}	
	}

	m_DtList.GetWindowRect( &rc );
	ScreenToClient( rc );
	list_ht = bKeyScan ? (y-1) - rc.top : (y-4) - rc.top;
	m_DtList.MoveWindow( rc.left, rc.top, rc.Width(), (y-2) - rc.top );
	m_DtList.EnforceLayout();

	if( ! bKeyScan ) {

		m_DtList.GetWindowRect( &rc );
		ScreenToClient( &rc );
		int list_btm = rc.bottom;

#ifdef _SLIP_ACCOUNTS_
		CRect	lnkRect;
		GetDlgItem( IDC_ICSDISPRDREMAIN )->GetWindowRect( &lnkRect );
		ScreenToClient( lnkRect );

		CRect	dspRect;
		GetDlgItem( IDC_DISPSLIP_CHK )->GetWindowRect( &dspRect );
		ScreenToClient( dspRect );
		GetDlgItem( IDC_DISPSLIP_CHK )->MoveWindow( lnkRect.left+(lnkRect.Width()+1), list_btm, dspRect.Width(), dspRect.Height() );

		GetDlgItem( IDC_LINKSLIP_BTN )->GetWindowRect( &tmp );
		ScreenToClient( tmp );
		GetDlgItem( IDC_LINKSLIP_BTN )->MoveWindow( lnkRect.left+(lnkRect.Width()+1)+dspRect.Width(), list_btm, tmp.Width(), tmp.Height() );
#endif

		GetDlgItem( IDC_MNTHSEL )->GetWindowRect( &tmp );
		ScreenToClient( tmp );
		GetDlgItem( IDC_MNTHSEL )->MoveWindow( tmp.left, list_btm, tmp.Width(), tmp.Height() );

	}

	if( ! bOnSize ) {
		// 表示可能な行数セット
		SCROLL_NL_ = m_DtList.GetDispDataCount();
		if( SCROLL_NL_ > DDATA_LNMAX )	SCROLL_NL_ = DDATA_LNMAX;
		// 決算確定マスター
		if( ! bCONFIRM_MASTER ) {
			if( ! bKeyScan )	SCROLL_NL_ += 1;	// 追加入力行分
		}
	}

	m_DtLine.ShowWindow( bKeyScan ? SW_HIDE : SW_SHOW );
	
}
#endif


//
//検索モードの初期化
void CDBDinpView::to_scanmode( int seq_send /*=0*/)
{
	int i,ofs,sft;

//	m_Input.ShowWindow(SW_HIDE);
	// 新規用エディットが残る場合があるので、サイズ　0
	// ShowWindow(SW_HIDE)　がきかない？
	m_Input.MoveWindow( 0,0,0,0 );

	PROGRAM_OK = FALSE;

	ScanMode_WndMove( TRUE );

	//データディスプレイニシャライズ
	for( i = 0 ; i < DDATA_LNMAX ; ++i )
	{
		inpline_inz( &DDATA_LINE[i] );
	//	inpctlline_inz( &DDATA_CTL[i][0] );
	}
	m_DtList.RemoveAllDtListData();

	//初期データリード＆テーブル作成
//	_fillbuf( (char *)&ZREC[0], sizeof( ZREC ), 0 );
	for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
		ZREC[i].Reset();
	}
	
	ofs = m_pScanView->scan_rdpnt();	//読み出し開始オフセットリード = SCAN_RD_BOTOM

	int scroll_cnt;
	scroll_cnt = SCROLL_NL();

	if( ofs < scroll_cnt ) {
		sft = scroll_cnt - ofs - 1;
		if( (scroll_cnt - sft) == 1 )	//１行目だと 上にデータがある場合、スクロールしてしまう
			sft--;
	}
	else
		sft = 1;

	if( m_pScanView->scan_data_rd( &ZREC[ scroll_cnt - 1 ], scroll_cnt, sft ) == sft ) {
		set_nowln( scroll_cnt - sft );	//最下行 - sft にポジションセット
	}
	else
	{
		m_pScanView->scan_data_rd( &ZREC[ scroll_cnt - 1 ], scroll_cnt, 0 );
		set_nowln( scroll_cnt );		//最下行にポジションセット
	}
	scandat_dspn( &ZREC[0], DDATA_LINE, scroll_cnt );
	//データテーブル表示
	_ddata_dsp( -1 );

	//修正データ初期読み出し
//	memmove( (char *)&CREC, (char *)&ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1], sizeof( struct _DATAREC ) );
	CREC = ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1];
	//残高 SUB. from CREC
	reupdat(&CREC);
	DATA_1RD = 1;

	inpmdisp( _KEYSCAN );

	/* 諸口合計	*/
	g_stotal( SYOGT, (char *)SYOG_d, (char *)SYOG_c, (char *)SG_def );
	syogdsp( (char *)SG_def );
	/* 現金合計	*/
	g_stotal( /*GNKIN*/pAUTOSEL->DCKM_CODE, (char *)GENK_d, (char *)GENK_c, (char *)GK_def );
	genkdsp( (char *)GK_def );

	//#* 05.16 /01 摘要のみ修正
	if( IsSCAN_TKYONLY() ) {
		//摘要にフォーカスセット
		set_nowpn( TKY_PN );
		if( ! seq_send ) {
#if (FOCUS_DEBUG == 1)
			set_focusTrace( TKY_PN, 1, __LINE__ );
#else
			set_focus( TKY_PN, 1 );
#endif
		}
	}
	else {
		//SEQにフォーカスセット
		set_nowpn( SEQ_PN );
		if( ! seq_send ) {
#if (FOCUS_DEBUG == 1)
			set_focusTrace( SEQ_PN, 1, __LINE__ );
#else
			set_focus( SEQ_PN, 1 );
#endif
		}
	}

	if( (((CMainFrame*)GetDBDinpMainFrame())->IsKeiriJzEBEnv() || IsBuyScannerSave() ) && m_bSLACC_BUTTON_DISP ) {
		m_GnpScanBtn.EnableWindow(FALSE);
		m_GnpScanBtn.ShowWindow(SW_HIDE);
	}
	
	this->UpdateWindow();

	PROGRAM_OK = TRUE;
}

//
// 番号指定による修正
void CDBDinpView::to_scanmode2( int seq, int now_seq )
{
	m_bScanMode = TRUE;
	//検索モードの初期化
	to_scanmode( 1 );
	// 番号の送り出し
	PostMessage( WM_SENDSEQ, seq, now_seq );
}

//
// 番号の送り出し
LRESULT CDBDinpView::OnSendSeq( WPARAM wParam, LPARAM lParam )
{
	int seq =(int)wParam;
	int now_seq = (int)lParam;

	SeqDataJob( seq, now_seq );

	return 1;
}


void CDBDinpView::set_nowln(int ln)
{
	int prev_ln = get_nowln();
	BOOL bRedraw = FALSE;

//FILETrace( "@Dinp::set_nowln ln = %d, prev_ln = %d\n", ln, prev_ln );

	if( prev_ln == APPEND_LINE ) {
		if( ln != APPEND_LINE )	bRedraw = TRUE;
	}
	else {
		if( ln == APPEND_LINE )	bRedraw = TRUE;
	}

	set_prevln( prev_ln );

	_set_nowln( ln );

	BOOL bFocus;
	if( ln == APPEND_LINE )	bFocus = TRUE;
	else					bFocus = FALSE;

	// フォーカスが消費税のCDBEDTに移動しても リストのカレントラインの色づけ
	m_DtLine.SetFocusedSign( bFocus );
	if( bRedraw )	m_DtLine.RedrawDataListCtrl();
	m_DtList.SetFocusedSign( ! bFocus );
	if( bRedraw )	m_DtList.RedrawDataListCtrl();

	m_DtLine.HideTitleTipWindow();
	m_DtList.HideTitleTipWindow();

}

//前行を保存
void CDBDinpView::set_prevln(int ln)
{
	m_prevln = ln;
}
//前行をゲット
int CDBDinpView::get_prevln()
{
	return m_prevln;
}


#ifdef	_ICSPEN_	// 01.20 /01
short CDBDinpView::SkbnSetPen( BOOL flg )
{
#ifdef DB_CLOSE
	if( m_pSkbnInX ) {
		return	m_pSkbnInX->SetPen( flg, GetPenObject() );
	}
#endif
	return	0;
}

void CDBDinpView::SkbnSetPenRect( CRect& rect )
{
#ifdef DB_CLOSE
	if( m_pSkbnInX ) {
		m_pSkbnInX->SetPenRect( rect );
	}
#endif
}

short CDBDinpView::ScanKeySetPen( BOOL flg )
{
//	if( m_pScanKey ) {
//		return	m_pScanKey->SetPen( flg, GetPenObject() );
//	}
	return	0;
}


CRect CDBDinpView::GetPenRect()
{
	CRect	rect, crect, rect1, rect2;
	this->GetClientRect( &crect );
	this->ClientToScreen( &crect );
	((CWnd*)this)->GetDlgItem( IDC_ICSSELCTRL1 )->GetWindowRect( rect1 );
	((CWnd*)this)->GetDlgItem( IDC_ICSSELCTRL2 )->GetWindowRect( rect2 );
	rect.left	=	crect.left;
	rect.top	=	rect2.bottom + 1;
	rect.right	=	rect.left + crect.Width();
	rect.bottom	=	rect.top + (crect.bottom - rect2.bottom - 1);

	return rect;
}



void CDBDinpView::OnICSInputFocus( int pn )
{
}

CRect CDBDinpView::GetICSAnkRect()
{
	CRect	rect, crect, rect1, rect2;
	this->GetClientRect( &crect );
	this->ClientToScreen( &crect );
	((CWnd*)this)->GetDlgItem( IDC_ICSSELCTRL1 )->GetWindowRect( rect1 );
	((CWnd*)this)->GetDlgItem( IDC_ICSSELCTRL2 )->GetWindowRect( rect2 );
	rect.left	=	crect.left;
	rect.top	=	rect1.top + rect1.Height() + rect2.Height();
	rect.right	=	rect.left + crect.Width();
	rect.bottom	=	rect.top + (crect.bottom - (rect1.top + rect1.Height() + rect2.Height()) - 1);

	return rect;
}

void CDBDinpView::OnICSAnkFocus()
{
	if( flgPen != FALSE ) {

		CRect	rect = GetICSAnkRect();

		SkbnSetPenRect( rect );
	}
}


#endif


//
// 自動項目表示
void CDBDinpView::AutoSelIndicator( struct _AUTOSEL *sel, int inp_mode )
{
	BOOL df_disp_sw;
	BOOL disp_sw;

	df_disp_sw = (inp_mode != _KEYSCAN) ? TRUE : FALSE;

	int sw;		// 0 ... OFF, 1 ... ON, 2 ... スキップ 
	int i;
	COLORREF back, text, deftitle, deftext;
	deftitle	= m_DtLine.GetDtTitleColor();
	deftext		= m_DtLine.GetDtTitleTextColor();

	double src_darken, dest_darken;
	// 自動項目の色はタイトル色より、作成
	src_darken = (double)40 / 100.0;	//薄め
	dest_darken = 1.0 - src_darken;
	back = PixelAlpha(deftitle, src_darken, RGB_WHITE, dest_darken);
	// 文字色は、濃い目
	src_darken = (double)70 / 100.0;
	dest_darken = 1.0 - src_darken;
	text = PixelAlpha(deftitle, src_darken, RGB_BLACK, dest_darken);

	for( i = 0 ; i < DATA_DOCEVI+1 ; ++i )
	{
		disp_sw = df_disp_sw;	// 戻す
		sw = 0;

		switch( i )
		{
		case DATA_SEQ:	// 番号
			break;
		case DATA_DATE:	// 日付
			if( sel->DATE_OPT != -1 )
			{
				sw = sel->DATE_OPT == OPT_NONOPT ? 0 : 1;
			}
			break;
		case DATA_DENP:	// 伝票番号
			if( sel->DENP_OPT != -1 )
			{
				sw =  sel->DENP_OPT == OPT_NONOPT ? 0 : (sel->DENP_OPT == OPT_NON ? 2 : 1);

				if( sel->DENP_OPT == OPT_NON )
				{
					disp_sw = TRUE;
				}
			}
			break;
		case DATA_DEB_BMON:	// 部門
		case DATA_CRE_BMON:
			if( ! BMON_MST )	sw = -1;
			else {
				if( sel->BMON_OPT != -1 ){
					sw = sel->BMON_OPT == OPT_NONOPT ? 0 : 1;
				}
			}
			break;
		case DATA_DEB_KNO:	// 工事番号
		case DATA_CRE_KNO:
			if( ! M_KOJI )		sw = -1;
			else {
				if( sel->KOJI_OPT != -1 ) {
					sw = sel->KOJI_OPT == OPT_NONOPT ? 0 : 1;
				}
			}
			break;
		case DATA_DEBT:	// 借方
			if( sel->DEBT_OPT != -1 )
			{
				sw = sel->DEBT_OPT == OPT_NONOPT ? 0 : 1;
			}
			break;
		case DATA_CRED:	// 貸方
			if( sel->CRED_OPT != -1 )
			{
				sw = sel->CRED_OPT == OPT_NONOPT ? 0 : 1;
			}
			break;
#ifdef CUT_CLOSE
		case DATA_SYZ:	// 消費税（輸入仕入）
			if( sel->SYIMPORT_OPT != -1 )
			{
				sw = sel->SYIMPORT_OPT == OPT_NONOPT ? 0 : 1;
			}
#endif
		case DATA_ZEI:
			if( sel->ZEIMDFY_OPT != -1 )
			{
				sw = sel->ZEIMDFY_OPT == OPT_NONOPT ? 0 : 1;
			}
			break;

		case DATA_HJIYU:// 変動事由
			if( sel->HJIYU_OPT != -1 )
			{
				sw = sel->HJIYU_OPT == OPT_NONOPT ? 0 : 1;
			}

			if( IsHjiyuItemGyosyu() == 0 ) {
		//	if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ ) {
				sw = -1;
			}
			break;

		case DATA_DOCEVI:	//証憑
			if( sel->DOCEVI_OPT != -1 )
			{
				sw = sel->DOCEVI_OPT == OPT_NONOPT ? 0 : 1;
			}
			break;

		default:
			break;
		}

		if( sw == -1 ) 
			m_DtLine.SetTitleClr( i, RGB_WHITE, RGB_GRAY );
		else
			m_DtLine.SetTitleClr( i, sw ? text : deftext, sw ? back : deftitle );
	}

	m_DtLine.RedrawDataListCtrl();
}


void CDBDinpView::OnButtonF4() 
{
	if( !PROGRAM_OK )
		return;

	if( get_nowpn() == TKY_PN )
			remove_edabandsp( &CREC, CRED_PN );
	else {
		// 電卓ウィンドウ消去
		CalcMouseEvent();
		sel_inpmode(get_nowpn());	// 入力モード選択
	}
}


// 入力モード選択
void CDBDinpView::sel_inpmode( int pn )
{
	CAutoInp autodlg;
	int grpno = 0;
	int sw;
	int brtek_sw;
	int kmno_sw;
	int bmndsp_sw;
	int scncd_dspsw;
	int cno, nowpn;
	int old_kjun, mode, old_bmntkbr, old_brnkana, old_rtswk, old_denptype, old_kanasearch;
	char	dc_tmp[16] = {0};
	short old_BmnSel;
	int selupdate = 0;
	int tky_update = 0;
//	short oldBILL_OPT;
//	short oldHUSEN_OPT;

	FNC_BUTTON = TRUE;
	{
		//イニシャライズ&表示
		if( INP_mode == _APPEND || INP_mode == _MODEFY || INP_mode == _KEYSCAN )
		{
			switch( pn )
			{
			case DATE_PN: grpno = 1; break;
			case DENP_PN: grpno = 2; break;
			case BMON_PN: grpno = 3; break;
			case DEBT_PN: grpno = 4; break;
			case CRED_PN: grpno = 5; break;
			case SKBN_PN: grpno = 10; break;
			default:
				grpno = 1;
				break;
			}
		}
		else
			grpno = 1;

		brtek_sw = DinpSW.isBrTekDisp();	// 変更前枝番摘要表示ＳＷ
		kmno_sw = pAUTOSEL->KMNO_OPT;		// 変更前科目選択コードＳＷ 02.02 /02
		bmndsp_sw = GetBmnDspMode();		// 変更前 部門名称 08.30 /04
		strcpy_s( dc_tmp, sizeof dc_tmp, pAUTOSEL->DCKM_CODE );
		old_kjun = pAUTOSEL->KANADSP_OPT;
		scncd_dspsw = pAUTOSEL->SCNCD_OPT;
		old_BmnSel  = pAUTOSEL->BMNSEL_OPT;
//		oldBILL_OPT = pAUTOSEL->BILL_OPT;
//		oldHUSEN_OPT = pAUTOSEL->HUSEN_OPT;

		old_bmntkbr = pAUTOSEL->BMNTKBR_OPT;
		old_brnkana = pAUTOSEL->BRNKANA_OPT;
		old_rtswk	= pAUTOSEL->RTSWKKANA_OPT;
		old_denptype	= DenpTypeMode();
		old_kanasearch = KanaSearchType();

		autodlg.OnInitial( M_KOJI, pAUTOSEL, grpno, VK_F4, INP_mode == _APPEND );

		((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(2);

		PROGRAM_OK = FALSE;

		if( autodlg.DoModal() == IDOK )
		{

			// データ取出し
			autodlg.get_data( pAUTOSEL );

			// タイトル欄色表示
			AutoSelIndicator( pAUTOSEL, INP_mode );

			// 設定をDBに保存
			set_inpmode( pAUTOSEL );
			// 再描画フラグをセットする
			pDBzm->IsModify() = TRUE;

#ifdef LATER_CLOSE
			// 輸入仕入仕訳作成ＳＷセット
			pDBsy->Sy_impdta_mk( DinpSW.isMkSyImpDta( pDBzm ) );
#endif

			// 02.20 /02 科目選択画面サイン
			if( kmno_sw != pAUTOSEL->KMNO_OPT )
				SelKmkUpdate( INP_mode );

			// 貸借科目
			if( strcmp( pAUTOSEL->DCKM_CODE, dc_tmp )) {
				g_stotal( pAUTOSEL->DCKM_CODE, (char *)GENK_d, (char *)GENK_c, (char *)GK_def );
				genk_strchg( pAUTOSEL->DCKM_CODE );
				genkdsp( (char *)GK_def );
			}
			if( pAUTOSEL->DCKM_OPT == OPT_NODSP ) {
				genk_ctrlshow( FALSE );
			}
			else {
				genk_ctrlshow( TRUE );
			}
			// カナ順表示
			if( old_kjun != pAUTOSEL->KANADSP_OPT ) {
				mode = (pAUTOSEL->KANADSP_OPT == OPT_KANAJUN ) ? SL_KJUN_DSP : SL_NORMAL_MODE;
				m_Seldata.SetKanaDspMode( mode );
				m_Seldata.SelectUpdate();
				selupdate = 1;
			}
			// 先頭ページ
			m_Seldata.SetKamokuPageHold( pAUTOSEL->KMPG_OPT == OPT_KMPG );

			// 03.16 /11
			if( old_BmnSel != pAUTOSEL->BMNSEL_OPT )
				BmnKjSelDispRst();

			// 選択フラグセット
			DWORD opt = 0;
			opt = m_Seldata.GetSelOptMode();
			(opt &= ~0x01);
			if( pAUTOSEL->SWTKYD_OPT == OPT_SWTKYD )	opt |= 0x01;
			m_Seldata.SetSelOptMode( opt );

			//部門摘要枝番を使用する。
			if( old_bmntkbr != pAUTOSEL->BMNTKBR_OPT ) {
				opt = m_Seldata.GetSelOptMode();
				(opt &= ~0x02);
				if( pAUTOSEL->BMNTKBR_OPT == OPT_CHKON ) {
					opt |= 0x02;
				}
				m_Seldata.SetSelOptMode( opt );
				if( BRNTKY_sel )	SelDspRst( INP_mode );
			}
			//枝番名称カナ順
			if( old_brnkana != pAUTOSEL->BRNKANA_OPT ) {
				mode = (pAUTOSEL->BRNKANA_OPT == OPT_KANAJUN ) ? SL_KJUN_DSP : SL_NORMAL_MODE;
				m_Seldata.SetBrnSelDispMode( mode );

				if( BRNTKY_sel ) {
					BRNTKY_PAR bpar;
					bpar = m_Seldata.GetBrnTkyPar();
					m_Seldata.BrnTekClear();	//内部情報クリア

					int bmn = -1;
					if( pAUTOSEL->BMNTKBR_OPT == OPT_CHKON ) {
						bmn = bpar.bt_bmn;
					}
					BRNTKY_select( bmn, bpar.bt_code, NULL, TRUE);
				}
				else {
					m_Seldata.BrnTekClear();	//内部情報クリア
				}
			}
			//対応摘要のカナ表示
			if( old_rtswk	!= pAUTOSEL->RTSWKKANA_OPT ) {
				opt = m_Seldata.GetSelOptMode();
				(opt &= ~0x04);
				if( pAUTOSEL->RTSWKKANA_OPT == OPT_KANAJUN ) {
					opt |= 0x04;
				}
				m_Seldata.SetSelOptMode( opt );

				tky_update = 1;
			}

			// 枝番摘要＆部門名称 表示
			if( ( brtek_sw != (sw = DinpSW.isBrTekDisp()) ) || bmndsp_sw != GetBmnDspMode() || 
				scncd_dspsw != pAUTOSEL->SCNCD_OPT || old_bmntkbr != pAUTOSEL->BMNTKBR_OPT )
			{
				if( scncd_dspsw != pAUTOSEL->SCNCD_OPT ) {
					KamokucodeSign( (pAUTOSEL->SCNCD_OPT == OPT_SCNCD) );
					// 枝番文字列に科目コード追加 サインをセット
					m_BrTek.brntky_kmcode( pAUTOSEL->SCNCD_OPT == OPT_SCNCD );
				}

				if( INP_mode == _APPEND || INP_mode == _MODEFY || INP_mode == _KEYSCAN )
				{
					/*--------------------
						仕訳再表示
					--------------------*/
					// データテーブル再表示
					scandat_rdsp( &ZREC[0], DDATA_LINE, SCROLL_NL() );
					//データテーブル表示
					_ddata_dsp( -1 );

					DB_SyIniz( &CREC );

					// 入力行再表示
					if( INP_mode == _APPEND )
					{
						// 部門名称 08.30 /04
						set_etcdsp( get_nowln(), BMON_PN );
						set_etcdsp( get_nowln(), CBMN_PN );

						// 借方
						set_etcdsp( get_nowln(), DEBT_PN );	// 今回ポジションに枝番摘要データをセット
						// 貸方
						set_etcdsp( get_nowln(), CRED_PN );	// 今回ポジションに枝番摘要データをセット
						inpline_dsp( getLINE_DATA(), get_nowln() );
					}
					// 枝番摘要選択画面のクリア
					if( (sw != brtek_sw) && BRNTKY_sel )
					{
						//科目・摘要選択画面リセット
						SelDspRst( INP_mode );
						enable_fixval( fixvalcnd() );	// 元背景色強制描画のため
					}
				}
			}

#ifdef OLD_CLOSE
			CString section;
		//	section.Format( "Software\\日本ICS株式会社\\%s\\DBAPP\\ValCopy", GET_LOGINNAME() );
			section = "ValCopy";
			ICSRegistryManager*	reg;
			reg = theApp.GetRegistryManager();
			DWORD valsgn = ValEnterCopyMode();
			BOOL b = reg->WriteProfileDword( section, "mode", &valsgn );
			section = "TkyReg";
			valsgn = TkyRegMode();
			b = reg->WriteProfileDword( section, "type", &valsgn );
#endif
			CString section, key;
		//	section = OPT_SECTION;
			// 金額 Enter 複写
			theApp.SetOptionInt( OPT_SECTION, OPT_KEYVALCPY, ValEnterCopyMode() );
			// 摘要登録
			theApp.SetOptionInt( OPT_SECTION, OPT_KEYTKYREG, TkyRegMode() );
			// 残高問合せ移行
			theApp.SetOptionInt( OPT_SECTION, OPT_KEYZMOVE, ZmoveMode() );

			// Tabキーの動作
			theApp.SetOptionInt( OPT_SECTION, OPT_KEYTKTAB, TKtabMode() );

			// 上手クラウド
			if( IsCloudSetupEnv() ) {
				theApp.SetOptionInt( JZCLOUD_SEC, JZCLOUD_KEY, JZcloudEndKakutei() );
			}
			// 伝票タイプの表示
			int st_chg = 0;
			if( DenpTypeMode() != old_denptype ) {
				if( old_denptype == 0 && INP_mode != _APPEND ) {
					if( pn != SEQ_PN ) {
						EDIT_PN = -1;
						set_focus( SEQ_PN );
					}
				}
				st_chg++;
			}

			theApp.SetOptionInt( OPT_SECTION, OPT_KEYDENPTYPE, DenpTypeMode() );

			theApp.SetOptionInt(OPT_SECTION, OPT_KANASEARCH, KanaSearchType());
			if( KanaSearchType() != old_kanasearch ) {
				nowpn = get_nowpn();
				if( nowpn == DEBT_PN || nowpn == CRED_PN ) {
					CICSDBEDT* pDBed;
					if( INP_mode == _APPEND ) {
						pDBed = &m_Input;
					}
					else {
						pDBed = &m_Inp2;
					}
					pDBed->SetKanaTerm(TRUE, KanaSearchKeta(), ICSDBEDT_KTYPE_KANA);
				}
				if( !selupdate ) {
					m_Seldata.SelectUpdate();
				}
			}
			//消費税欄毎回停止
			theApp.SetOptionInt(OPT_SECTION, OPT_SYZSTOP, SyzStopModeValue());

			if( st_chg ) {
				SetListColumn( pAUTOSEL->DSPLINE_OPT );
				SetDtListItemWidth();
				m_DtList.EnforceLayout();
				ddata_dsp(-1);
			}

/*			if( pAUTOSEL->BILL_OPT != oldBILL_OPT ) {
				if( pAUTOSEL->BILL_OPT == OPT_NOINP ) {
					m_DtLine.SetTitleClr( DATA_SYONO, RGB_DARKER_GRAY, RGB_GRAY );
					m_DtLine.SetTitleClr( DATA_TGDATE, RGB_DARKER_GRAY, RGB_GRAY );

					m_DtLine.SetListItemEnable( 1, DATA_SYONO, FALSE );
					m_DtLine.SetListItemEnable( 1, DATA_TGDATE, FALSE );
				}
				else {
					m_DtLine.SetTitleClr( DATA_SYONO,  (COLORREF)-1, (COLORREF)-1 );
					m_DtLine.SetTitleClr( DATA_TGDATE, (COLORREF)-1, (COLORREF)-1 );

					m_DtLine.SetListItemEnable( 1, DATA_SYONO, TRUE );
					m_DtLine.SetListItemEnable( 1, DATA_TGDATE, TRUE );
				}
			}
			if( pAUTOSEL->HUSEN_OPT != oldHUSEN_OPT ) {
				if( pAUTOSEL->HUSEN_OPT == OPT_NOINP ) {
					m_DtLine.SetTitleClr( DATA_HUSEN, RGB_DARKER_GRAY, RGB_GRAY );
					m_DtLine.SetListItemEnable( 1, DATA_HUSEN, FALSE );
				}
				else {
					m_DtLine.SetTitleClr( DATA_HUSEN, (COLORREF)-1, (COLORREF)-1 );
					m_DtLine.SetListItemEnable( 1, DATA_HUSEN, TRUE );
				}
			}
*/
		}

		((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(0);

		if( INP_mode == _APPEND )
		{
			// 伝票番号
			nowpn = get_nowpn();
			if( nowpn >= DENP_PN )
			{
				// 伝票番号自動入力
				if( auto_cno( &cno, &CREC, pAUTOSEL ) )
				{
					// セットされた
					getLINE_DATA()->_XXDTA.dd_cno = cno;

					if( nowpn == DENP_PN ) {
						VARIANT var;
						char bf[20];
						if( cno != -1 ) {
							sprintf_s( bf, sizeof bf, "%d", cno );
							var.pbVal = (BYTE*)bf;
						}
						else {
							var.pbVal = NULL;
						}
						exDBdata_set( DENP_PN, getINP_CTL( DENP_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
					}
					TerminationDataSet( DENP_PN );

					CREC.m_cno = SREC.m_cno = cno;	// データにセット
					// 自動加算
					if( pAUTOSEL->DENP_OPT == OPT_AUTO /*1*/ ) {
						DENP_NO = int_seetno( CREC.m_cno );
					}
				}
			}
		}

		// 枝番表示
		int mode = m_Seldata.GetBrnTkyMode();

		if( (pAUTOSEL->BRTEK_OPT == OPT_DSP) )	mode |= BT_NOMALBRN_TEK;
		else									mode &= ~BT_NOMALBRN_TEK;

		if( (pAUTOSEL->SYGBRN_OPT == OPT_DSP) )	mode |= BT_SYOGTBRN_TEK;
		else									mode &= ~BT_SYOGTBRN_TEK;

		m_Seldata.SetBrnTkyMode( mode );
		// 仕訳摘要表示優先
		m_Seldata.SetTekiyoMode( (pAUTOSEL->SWKTEK_OPT == OPT_SWKTEK) ? SL_TKY_SIWAKE : SL_TKY_RENSO );

		if( tky_update ) {
			m_Seldata.RenewInvno();
			m_pSyzInp->RenewInvNo();
		}
		PROGRAM_OK = TRUE;
	}
	FNC_BUTTON = FALSE;
}



// 科目選択画面の更新 02.20 /02
void CDBDinpView::SelKmkUpdate( int inp_mode )
{
	//科目名称選択表示
	int pn;

	pn = get_nowpn();				// 修正画面
	if( pn == TKY_PN ) return;

	int mode;
	mode = (pAUTOSEL->KMNO_OPT == OPT_KMNO) ? SL_KMK_CODE : SL_KMK_NUM;

	m_Seldata.SetKmDispMode( mode );
	m_Seldata.SelectUpdate();
}

void CDBDinpView::OnUpdateButtonF4(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	if( !PROGRAM_OK )
	{
		pCmdUI->Enable( FALSE );
		return;
	}

	pCmdUI->Enable( isUSE_AUTOSEL( pAUTOSEL, INP_mode == _APPEND ) ? TRUE : FALSE );
	
}


BOOL CDBDinpView::isUSE_AUTOSEL( struct _AUTOSEL *sel, BOOL APPEND_MODE )
{
	if( sel )
	{
		if( APPEND_MODE )
			return TRUE;
		else if( sel->BRN_OPT != -1 || sel->BRTEK_OPT != -1 || sel->BRTOTKY_OPT != -1 || sel->SYGBRN_OPT != -1 )
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}


// 付箋の入力 フォーカス
void CDBDinpView::FocusHusenInput( CWnd* pwnd, int ln, short click ) 
{
	if( ! m_bHusenFocus ) {
		m_bHusenFocus = TRUE;
		if( ! click )	return;
	}

	if( PROGRAM_OK == FALSE )
		return;

TRACE( "**FocusHusenInput %d\n", ln );	

	try
	{
		//別画面入力消去
		Destroy_ModeLessDlg();

		if( _dline_chg( pwnd, ln, HSEN_PN, click ? 0 : 1  ) == -1 )
			return;

		if( CheckAppendSansyo() )
			return;

		// 取消データの場合はＳＥＱに
		if( isdeldata() )
		{
			SAVE_PN = HSEN_PN;
			set_focus( SEQ_PN );
			return;
		}
		else
			SAVE_PN = -1;

		//入力項目チェック
		int set_pn;
		if( inp_ok( get_nowpn(), HSEN_PN, &set_pn ) == FALSE )
		{
			set_nowpn( set_pn );
			set_focus( /*get_nowpn()*/set_pn );
			return;
		}

		if( click ) {
			//当項目まで自動入力
			if( INP_mode == _APPEND && !is_AutoInp_use() )
				AutoInp( HSEN_PN );
		}

		getLINE_DATA()->LINE_CND[HSEN_PN].INP_sg = TRUE;

		set_nowpn( HSEN_PN );

		HSEN_DATA crec = {0}, srec = {0};
		crec.hsen_inpsg = getLINE_DATA()->LINE_CND[HSEN_PN].INP_sg;
		crec.hsen_sel = (CREC.m_dsign[7]&0x0f);
		strcpy_s( crec.hsen_cmt, sizeof crec.hsen_cmt, CREC.m_tag );

		srec.hsen_sel = (SREC.m_dsign[7]&0x0f);
		strcpy_s( srec.hsen_cmt, sizeof srec.hsen_cmt, SREC.m_tag );

		m_pHusenInp->SetHusenData( &crec, &srec );
		HusenInpDisp();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

/*
long CDBDinpView::TerminationHusenCombo( long nChar )
{

	if( nChar == VK_DOWN ) {
		if( ! m_Hcbo.GetDroppedState() ) {
			m_Hcbo.ShowDropDown( TRUE );
			return 0;
		}
		return nChar;
	}

	switch( nChar ) {
	case VK_LEFT :
		set_termfocus(get_nowln(), HSEN_PN, -1);
		return 0;
		break;	
	case VK_RIGHT : 
	case VK_TAB :
	case VK_RETURN :
		set_termfocus(get_nowln(), HSEN_PN, 1);
		return 0;
		break;
	}

	if( nChar == 0 ) {
//TRACE( "m_Hcbo.ShowWindow(SW_HIDE)\n" );
//		m_Hcbo.ShowWindow(SW_HIDE);
//		m_Hcbo.PostMessage( WM_SHOWWINDOW, FALSE, 0 );
	}
	DT_EDIT edit;

	edit.hdr.hwndFrom = m_Hcbo.GetSafeHwnd();
	edit.hdr.idFrom   = IDC_HS_COMBO;
	edit.hdr.code     = DLN_ENDLABELEDIT;
	edit.nChar = nChar;

	if( get_nowln() == APPEND_LINE )
		m_DtLine.SendMessage(WM_DTLISTUSER, DLN_ENDLABELEDIT, (LPARAM)&edit );
	else
		m_DtList.SendMessage(WM_DTLISTUSER, DLN_ENDLABELEDIT, (LPARAM)&edit );

	return nChar;
}

*/

//	付箋入力のメッセージ処理
//
//
LRESULT CDBDinpView::OnHusenInpMsg( WPARAM wParam, LPARAM lParam )
{
	long nChar;
	int i;

	WORD sgn, key;
	sgn = LOWORD( wParam );
	key = HIWORD( wParam );
	HSEN_DATA* pHdt;

	if( lParam ) {
		pHdt = (HSEN_DATA*)lParam;
		CREC.m_dsign[7] &= (~0x0f);
		CREC.m_dsign[7] |= pHdt->hsen_sel;
		CREC.m_tag = pHdt->hsen_cmt;
		getLINE_DATA()->_XXDTA.dd_husen = pHdt->hsen_sel;
		getLINE_DATA()->_XXDTA.dd_hscomment = pHdt->hsen_cmt;
	}

	if(	sgn == 0 ) {	// キーによる終了

		if( m_pHusenInp != NULL && key ) {

			TerminationDataSet( HSEN_PN );

			if( key == VK_RETURN || key == VK_TAB ) {

				if( m_OneHusen || check_lastitem( get_nowln(), HSEN_PN, lParam ) ) {
					// 枝番未入力チェックＪＯＢ
					if( isNoBRNInpChk() )
						return 1;
					//工事番号チェック
					if( isKojiChk() )
						return 1;
					//部門範囲チェック
					if( isBmnHaniChk() ) 
						return 1;
					//工事番号 科目チェック
					if( isKojiDataChk() )
						return 1;
					//部門未入力チェック
					if( isNoBmnInpChk() )
						return 1;

					if( INP_mode == _APPEND ) {
						DATAandDOC_AppendOperation( HSEN_PN );
						//付箋データをクリア
						HSEN_DATA crec = { 0 }, srec = { 0 };
						m_pHusenInp->SetHusenData(&crec, &srec);
					}
					else {
						// dline_chg でスクロールした場合、dline_chg より FocusHusenInputが呼ばれて、
						// 再度付箋のダイアログを表示されて、set_focus が効かなくなるで フラグを立てる
						m_bHusenFocus = FALSE;

						i = dline_chg( this, VK_DOWN );

						if( !i ) {	
							if( IsSCAN_TKYONLY() ) {
								set_focus( TKY_PN );
							}
							else {
								set_focus( DATE_PN );// SCAN 時など 次の行がない
							}
						}
						else{
							m_bHusenFocus = TRUE;
							set_focus( get_nowpn() );
						}
					}
				}
				else {
					set_termfocus( get_nowln(), HSEN_PN, 1 );
				}
			}
			else {

				nChar = dline_chg( this, key );
				if( nChar == -1 )
				{
	//				MessageBeep( 0xffffffff );
					return 1;
				}
				else if( !nChar )
				{
					// 取消データの場合はＳＥＱに
					if( isdeldata() )
					{
						SAVE_PN = (m_OneHusen == 0) ? HSEN_PN : TKY_PN;
						set_focus( SEQ_PN );
						return 1;
					}
					else
						SAVE_PN = -1;

					set_focus( (m_OneHusen == 0) ? HSEN_PN : TKY_PN );
					return 1;
				}

				switch( nChar ) {
				case VK_LEFT:
				case VK_F2:
					if( (m_OneHusen == 0) )
						set_termfocus( get_nowln(), HSEN_PN, -1 );
					else
						set_focus( TKY_PN );
					break;
				}
			}
		}
	}
	else if( sgn == 1 ) {
		//表示を更新
		TerminationDataSet( HSEN_PN );
	}

	return 1;
}


//
//	サブ入力画面からのキー処理
//
LRESULT CDBDinpView::OnSubDataInEnd( WPARAM wParam, LPARAM lParam )
{
	long nChar;
	int i;

	WORD sgn, key;
	sgn = LOWORD( wParam );
	key = HIWORD( wParam );
	CDBINPDataRec* prec;

	if( m_pSubDtInp == NULL )
		return 1;

	if( lParam ) {
		prec = (CDBINPDataRec*)lParam;

		CREC.m_dsign[7] &= (~0x0f);
		CREC.m_dsign[7] |= (prec->m_dsign[7]&0x0f);
		CREC.m_tag = prec->m_tag;
		CREC.m_rnumber	= prec->m_rnumber;
		CREC.m_tdate	= prec->m_tdate;

		getLINE_DATA()->_XXDTA.dd_husen = (prec->m_dsign[7]&0x0f);
		getLINE_DATA()->_XXDTA.dd_hscomment = prec->m_tag;

		getLINE_DATA()->_XXDTA.dd_tdate = prec->m_tdate;
		IntsrekitoGenstr( getLINE_DATA()->_XXDTA.dd_tdstr.GetBuffer(30), 30, prec->m_tdate );
		getLINE_DATA()->_XXDTA.dd_tdstr.ReleaseBuffer();
	}

	if(	sgn == 0 ) {	// キーによる終了
		// データの再表示は、付箋のみ[手形関係は1行表示時は見えない]
		TerminationDataSet( HSEN_PN );

		if( key ) {
			// 仕訳を書き込む
			if( key == VK_RETURN || key == VK_TAB ) {

				// 枝番未入力チェックＪＯＢ
				if( isNoBRNInpChk() )
					return 1;
				//工事番号チェック
				if( isKojiChk() )
					return 1;
				//部門範囲チェック
				if( isBmnHaniChk() ) 
					return 1;
				//工事番号 科目チェック
				if( isKojiDataChk() )
					return 1;
				//部門未入力チェック
				if( isNoBmnInpChk() )
					return 1;

				if( INP_mode == _APPEND ) {
					DATAandDOC_AppendOperation( HSEN_PN );
				}
				else {

					i = dline_chg( this, VK_DOWN );

					if( !i ) {	
						if( IsSCAN_TKYONLY() ) {
							set_focus( TKY_PN );
						}
						else {
							set_focus( DATE_PN );// SCAN 時など 次の行がない
						}
					}
					else{
						set_focus( get_nowpn() );
					}
				}
			}
			else {

				nChar = dline_chg( this, key );
				if( nChar == -1 )
				{
	//				MessageBeep( 0xffffffff );
					return 1;
				}
				else if( !nChar )
				{
					// 取消データの場合はＳＥＱに
					if( isdeldata() )
					{
						SAVE_PN = TKY_PN;
						set_focus( SEQ_PN );
						return 1;
					}
					else
						SAVE_PN = -1;

					set_focus( TKY_PN );
					return 1;
				}

				switch( nChar ) {
				case VK_LEFT:
				case VK_F2:
					set_focus( TKY_PN );
					break;
				}
			}
		}
	}
	else if( sgn == 1 ) {
		// フォーカスOFFによるデータ更新
		// データの再表示は、付箋のみ[手形関係は1行表示時は見えない]
		TerminationDataSet( HSEN_PN );
	}

	return 1;
}


// 定型仕訳ダイアログ表示サイズ算出
//
RECT CDBDinpView::GetRDDlgRECT( )
{
	RECT	rect;

	// top, left の位置からダイアログを表示する
	GetDlgItem( IDC_DTLIST )->GetWindowRect( &rect );

	return rect;
}


void CDBDinpView::OnButtonF7() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
#ifdef _SLIP_ACCOUNTS_
	if( IsDispSlip() ){
		m_pImgmng->DispImgDlgPrevPage();

		// フォーカス再セット
		set_focus( get_nowpn() );
		return;
	}
#endif

	double x, y;
	x = y = 1.0;
//	GetRate( x, y );
//	unsigned short code, brn;

#ifndef TKY_VER2
	TKY_REGPAREX	par;
#else
	TKY_REGPAR par;
#endif
	CString code1, code2;
	int pos, st /*, dc_sw*/;
	struct _DATA_LINE *ldta;
	char tky[128];

	int pn = get_nowpn();

	// 定型仕訳登録
	if( (INP_mode == _APPEND && pn != TKY_PN) ) {
		// 電卓ウィンドウ消去
		CalcMouseEvent();
		Rdadd_job( APPEND_LINE );
		return;
	}
	else {
		if( INP_mode == _MODEFY || INP_mode == _KEYSCAN ){
			if( pn != TKY_PN ) {
				// 電卓ウィンドウ消去
				CalcMouseEvent();
				Rdadd_job( get_nowln() );
				return;
			}
		}
	}

//	FNC_BUTTON = TRUE;
	{
		if( INP_mode == _APPEND || INP_mode == _MODEFY || INP_mode == _KEYSCAN )
		{
			if( pn == TKY_PN )
			{
				ldta = getLINE_DATA();

				// 摘要辞書
				CWnd* pwnd;
				pwnd = GetDlgItem( getINP_CTL( TKY_PN )->IDC_X );

				if( ldta->LINE_CND[ TKY_PN ].IMG_sg != 1 ||		//イメージ摘要の Del キーによる 取消
					(ldta->LINE_CND[ TKY_PN ].IMG_sg == 1 && IsSCAN_TKYONLY()) ) 
				{
					pos = ((CICSDBEDT*)pwnd)->GetCaretPosition();

					VARIANT var;
					var.pbVal = NULL;
					exDBdata_get( TKY_PN, getINP_CTL( TKY_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

					::ZeroMemory( tky, sizeof tky );
					if( var.pbVal != NULL ) {
						strcpy_s( tky, sizeof tky, (char*)var.pbVal );
					}

					if( tky[0] == '\0' ) {
						pos = -1;
					}
					else {
						ldta->_XXDTA.dd_tek.tekiyo = tky;
						TerminationDataSet( TKY_PN );
					}
				}
				else
				{
					pos = -1;
					tky[0] = '\0';
				}

				// 科目対応摘要
				if( ldta->LINE_CND[DEBT_PN].INP_sg == TRUE )
					code1 = ldta->_XXDTA.dd_dbt;
				else
					code1 = "";

				if( ldta->LINE_CND[CRED_PN].INP_sg == TRUE )
					code2 = ldta->_XXDTA.dd_cre;
				else
					code2 = "";

				// 摘要登録処理へ (DBInpSub.DLL)
				::ZeroMemory( &par, sizeof par );

#ifndef TKY_VER2
				strncpy_s( par.tex_debt, sizeof par.tex_debt, code1, KMKCD_LN );
				strncpy_s( par.tex_cred, sizeof par.tex_cred, code2, KMKCD_LN );
				par.tex_pos = pos;
				memcpy( par.tex_tky, tky, sizeof par.tex_tky );
				memcpy(par.tex_invno, CREC.m_invno, sizeof par.tex_invno);
				par.tex_menchk = (CREC.m_dsign[6] & 0x20) ? 1 : 0;

				// 選択部分
				if( pos >= 0 ) {
					par.tex_sel	= ((CICSDBEDT*)pwnd)->GetSelect();
				}
#else
				strncpy_s( par.tr_debt, sizeof par.tr_debt, code1, KMKCD_LN );
				strncpy_s( par.tr_cred, sizeof par.tr_cred, code2, KMKCD_LN );
				par.tr_pos = pos;
				memcpy( par.tr_tky, tky, sizeof par.tr_tky );
				par.tr_jobtyp = TP_SELECT;
#endif

				PROGRAM_OK = FALSE;
				((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(2);
				BOOL bTkquery;
#ifndef TKY_VER2
				st = TkyEdaTorokDialog( &par, pDBzm, pDBsy, this, &bTkquery );
			//	st = TekiyoTorokDialogEX( &par, pDBzm, pDBsy, this, &bTkquery );
#else
				st = TekiyoTorokDialog( &par, pDBzm, pDBsy, this, &bTkquery );
#endif
				if( bTkquery ) {
				//	pDBzm->tkrec->Requery( "", 0 );
				//	pDBzm->tkrec_kanamode() = FALSE;
					tkrec_queryAll();
				}

				if( st == 1 ) {
					pDBzm->IsModify() = TRUE;
					m_Seldata.SelectUpdate();
				}

				((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(0);
				PROGRAM_OK = TRUE;


				if( par.tex_brnsgn & 0x03 ) {
					int dc_sw =  par.tex_brnsgn;

					if( par.tex_brnsgn & 0x01 ) {
						CREC.m_dbr = par.tex_brndeb;
					}

					if( par.tex_brnsgn & 0x02 ) {
						CREC.m_cbr = par.tex_brncre;
					}

					tky_select_dsp( &CREC, dc_sw, NULL, FALSE, TRUE );
				}

				if( pos != -1 ) {
					CWnd* pwnd;
					pwnd = GetDlgItem( getINP_CTL( TKY_PN )->IDC_X );
					((CICSDBEDT*)pwnd)->SetCaretPosition( pos );
				}

				set_focus( TKY_PN );
			}
		}
	}
//	FNC_BUTTON = FALSE;


}

void CDBDinpView::OnUpdateButtonF7(CCmdUI* pCmdUI) 
{
#ifdef _SLIP_ACCOUNTS_
	if( IsDispSlip() ){
		pCmdUI->Enable( TRUE );
		return;
	}
#endif

struct _DATA_LINE *ldta;
int pn;

	if( !PROGRAM_OK || bCONFIRM_MASTER )
	{
		pCmdUI->Enable( FALSE );
		return;
	}

	if( INP_mode == _RETREV )	//スキャンキー登録
	{
		pCmdUI->Enable( FALSE );
	}
	else
	{
		ldta = getLINE_DATA();
		pn = get_nowpn();
	
		if( pn == TKY_PN ) {
			if( dinp_modefy_ok( ldta ) )
				pCmdUI->Enable( TRUE );
		}
		else {
			BOOL bEna = FALSE;

			if( INP_mode == _APPEND ) {
				switch( pn ) {
				case DATE_PN: case DENP_PN:
				case BMON_PN: case KOJI_PN: case DEBT_PN:
				case CBMN_PN: case CKJI_PN: case CRED_PN:
				case VAL_PN:  case ZEI_PN: 
				case DOCEVI_PN: case HJIYU_PN:
					bEna = TRUE;
					break;
				}
			}
			else {
				if( pn != TKY_PN && pn != SKBN_PN )	bEna = TRUE;
			}
			pCmdUI->Enable( bEna );
		}
	}	
}


//　入力ライン属性表示
void CDBDinpView::OnLineattr_dsp()
{
int seq;
int ln,n,sg,chk, redraw;
CDBINPDataRec *dta;
DTLIST_DATA	lstdta;

	try
	{
		if( INP_mode == _RETREV )
			return;

		redraw = 0;

		for( ln = 1 ; ln <= SCROLL_NL() ; ++ln )
		{
			chk = 0;
			n = DDATA_LINE[ln-1].ZREC_NO;
			if( n ) {
				dta = &ZREC[ n - 1];
				//修正サイン
				sg = MODEFY_BIT.bitncnd( (seq = dta->m_seq) - 1);
			
				m_DtList.GetDtListData( ln-1, &lstdta );

				if( sg == -1 || !sg ) {
					if( lstdta.dd_dsign & 0x04 ) {
						chk++;
						(lstdta.dd_dsign &= ~0x04);
					}
				}
				else {
					if( !(lstdta.dd_dsign & 0x04) ) {
						chk++;
						(lstdta.dd_dsign |= 0x04);
					}
#ifdef _SLIP_ACCOUNTS_
/*- '12.11.09 -*/
//					if( (lstdta.dd_dsign&0x800) && !(dta->m_dsign[6]&0x01) ){
//						chk++;
//						(lstdta.dd_dsign &= ~0x800);
//					}
//					else if( !(lstdta.dd_dsign&0x800) && (dta->m_dsign[6]&0x01) ){
//						chk++;
//						(lstdta.dd_dsign |= 0x800);
//					}
/*-------------*/
				//	if( IsPakageCd(ICS_IMGMNG_SLIP_PACKAGE_CODE) == 0 ){
					if( m_SlipPakageCd == 0 && _IS_SLIP_MASTER ) {	// 環境によって矢印による移動が遅くなる。[03.22 /13]
						if( (lstdta.dd_dsign&0x800) && !(dta->m_dsign[6]&0x01) ){
							chk++;
							(lstdta.dd_dsign &= ~0x800);
						}
						else if( !(lstdta.dd_dsign&0x800) && (dta->m_dsign[6]&0x01) ){
							chk++;
							(lstdta.dd_dsign |= 0x800);
						}
					}
/*-------------*/
#endif
				}

				// イメージサインＯＦＦ
				if( sg ) {
					if( _getLINE_DATA( ln )->LINE_CND[ TKY_PN ].IMG_sg == 2 ) {
						chk++;
						_getLINE_DATA( ln )->LINE_CND[ TKY_PN ].IMG_sg = 0;
					}
				}

				if( chk ) {
					m_DtList.UpdateDtListData( ln-1, &lstdta, FALSE );
					redraw++;
				}
			}
		}

		if( redraw )	m_DtList.RedrawDataListCtrl();

		return;
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


// 入力画面からタブで切り替えられたとき
void CDBDinpView::TabClickChangeWork()
{
	// 修正データリライト
	if( DATA_1RD )
	{
		//CREC to 残高ADD.
		rewupdat( &CREC, &SCAN_rec, INP_mode );
		DATA_1RD = 0;
	}
}


#ifdef REGDATA

// 定型仕訳共有情報：owntb から読み込み
int CDBDinpView::GetCmnOwntb()
{
	const	TCHAR	MKRD_APLNAME[]	=	_T("MKRDDB");
	const	TCHAR	MKRD_CMNITEM[]	=	_T("MKRDCMN");

	CString		filter;
	filter.Format( "apl_name = '%s' and itm_name = '%s' and u_id is null", MKRD_APLNAME, MKRD_CMNITEM );
	int owntb_opn = 0;

	if( pDBzm == NULL )	return 0;

	if( ! pDBzm->owntb ) {
		if( pDBzm->OwnTblOpen() == -1 )
			return -1;

		owntb_opn = 1;
	}

	if ( pDBzm->owntb->Requery( filter ) == -1 ) {
		// レコードがない場合は 共有で。
		m_CmnId = 0;
		pDBzm->owntb->AddNew();
		pDBzm->owntb->apl_name	=	MKRD_APLNAME;	// アプリケーション名
		pDBzm->owntb->itm_name	=	MKRD_CMNITEM;	// アイテム名
		pDBzm->owntb->u_id		=	-1;				// ユーザーＩＤは NULL
		pDBzm->owntb->vl[0]		=	m_CmnId;
		pDBzm->owntb->Update();
	}
	else {
		m_CmnId	=	(char)pDBzm->owntb->vl[0];	// 共有定型仕訳番号

		if( m_CmnId > 0  ) {
			m_CmnId = pDBzm->DBUserNo();
		}
	}

	if( owntb_opn ) {
		pDBzm->OwnTblClose();
	}

	return	0;
}
	
// 定型仕訳初期設定
int	 CDBDinpView::InitRdata()
{
	CString filter;

	if( GetCmnOwntb() != 0 ) {
		ermset( ERROR_READFILE, "定型仕訳の共有情報取得ができませんでした。");
		return -1;
	}

	Mkrd.RD_SetDataBase( pDBzm->m_database, pDBzm->m_sysctl, pDBzm );

	if( Mkrd.IsExistMkrdTable( FALSE, Voln1->apno ) ) {
		if( Mkrd.MkrdOpen() == ERR ) {
			ermset( ERROR_READFILE, "定型仕訳の初期化ができませんでした。");
			return -1;
		}
		if( m_CmnId > 0 ) 
			filter.Format( "rl_owner = %d", m_CmnId );	// ユーザごと
		else
			filter = "rl_owner = 0";	// マスターで共有

		Mkrd.label->Requery( filter );

		if( Mkrd.label->GetRecordCount() == 0 )
				m_bRGDTA_BUTTON_DISP = FALSE;
		else {
			if( IsMasterType( MST_INSERT|MST_SYZ5_8|MST_SYZ8 ) ) {
			//	if( Mkrd.check_H26syz( pDBsy ) != 0 ) {
			//		ermset( 0, "定型仕訳の消費税チェックでエラーが発生しました。");
			//		return -1;
			//	}

				// 上の関数で、バッファとして使用しているため元に戻す。
				pDBzm->dbdata->MoveLast();
			}
			m_bRGDTA_BUTTON_DISP = TRUE;
		}
	}

	return 0;
}


//　定型仕訳中の 部門・工事 上段複写のチェック
BOOL CDBDinpView::Rdta_autocheck( int pn )
{
	BOOL rdtamode = m_RDTATBL.RD_cnt > 0 ? TRUE : FALSE;

	if( ! rdtamode )	return TRUE;

	if( pn == BMON_PN ) {
		if( CREC.m_dbmn == -1 && SREC.m_dbmn != -1 ) {
			return FALSE;
		}
	}
	else if( pn == CBMN_PN ) {
		if( CREC.m_cbmn == -1 && SREC.m_cbmn != -1 ) {
			return FALSE;
		}
	}
	else if( pn == KOJI_PN ) {
		if( CREC.m_dkno.IsEmpty() && ! SREC.m_dkno.IsEmpty() ) {
			return FALSE;
		}
	}
	else if( pn == CKJI_PN ) {
		if( CREC.m_ckno.IsEmpty() && ! SREC.m_ckno.IsEmpty() ) {
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CDBDinpView::RdataToInpData( CDBINPDataRec *crec, CDBINPDataRec *srec )
{
	BOOL sg;
	CDBINPDataRec rdrec;
	int pn;
	BOOL cnd[10] = {FALSE};

	// 定型仕訳１行読取り
	if( sg = GetRdata( &rdrec, &CREC, &SREC, cnd ) )
	{
		// 消費税データ作成
		rdrec.m_seq = REAL_SEQ_NO+1;

		// 一旦フォーカスを 別に移動する。
		pn = get_nowpn();
		if( pn == BMON_PN || pn == CBMN_PN || pn == DEBT_PN || pn == CRED_PN ||
			pn == KOJI_PN || pn == CKJI_PN ) {
			set_nowpn( DATE_PN );
			c_set( IDC_ICSDISPCTRL10 );
		}

		// 一旦 科目なし+SEQ 変更で で消費税 イニシャライズ
		crec->Reset();
		crec->m_seq		= rdrec.m_seq+1;
		crec->m_ddate	= rdrec.m_ddate;
		crec->m_mofs	= rdrec.m_mofs;
		crec->m_ksign	= rdrec.m_ksign;

		// インボイス対応
		sprintf_s( crec->m_invno, sizeof( crec->m_invno ), _T( "%s" ), rdrec.m_invno );

		DB_SyIniz( crec );

		int  syzCnt = 0;
		DWORD syCode[10] = {0};

		syzCnt = rd_reccheck( &CREC, &rdrec, syCode );

#ifdef CLOSE
		if( IsMasterType( MST_SYZ5_8 ) ) {
			if( CREC.m_ddate < SYZ8_YMD ) {
				syCode = CD_RITU5;
				//消費税対象＋８％＋課税
				if( (rdrec.m_dsign[0]&0x40) && (rdrec.m_dsign[1]&0x0f) == 0x03 &&
					(rdrec.m_dsign[2]&0x0f) == 0x01 ) {
					bSyzSET = TRUE;
				}
				else {
					int dbt_sy, cre_sy;
					dbt_sy = pDBzm->km_syattr( rdrec.m_dbt );
					cre_sy = pDBzm->km_syattr( rdrec.m_cre );

					if( (dbt_sy & 0x0200) || (cre_sy & 0x0200) ) {

						if( (rdrec.m_dsign[1]&0x0f) == 0x03 ) {
							bSyzSET = TRUE;
						}
					}
				}
			}
			else {
				syCode = CD_RITU8;
				//消費税対象＋５％＋課税
				if( (rdrec.m_dsign[0]&0x40) && (rdrec.m_dsign[1]&0x0f) == 0x00 &&
					(rdrec.m_dsign[2]&0x0f) == 0x01 ) {
					bSyzSET = TRUE;
				}
				else {
					int dbt_sy, cre_sy;
					dbt_sy = pDBzm->km_syattr( rdrec.m_dbt );
					cre_sy = pDBzm->km_syattr( rdrec.m_cre );

					if( (dbt_sy & 0x0200) || (cre_sy & 0x0200) ) {

						if( (rdrec.m_dsign[1]&0x0f) == 0x00 ) {
							bSyzSET = TRUE;
						}
					}
				}
			}
		}

		if( pDBsy->IsKaniMaster() ) {
 			if( CREC.m_ddate < KANI6_YMD ) {
				//簡易課税６種 対象外？
				if( (rdrec.m_dsign[0]&0x40) && (rdrec.m_dsign[2]&0x0f) == 0x01 &&
					(rdrec.m_dsign[3]&0x0f) == 0x06 ) {

					BYTE defKbn = KaniDefaultKubun( Voln1 );
					//サインOFF
					rdrec.m_dsign[3] &= ~0x0f;
					rdrec.m_dsign[3] |=  defKbn;	//基本業種セット
				}
			}
		}
		//リバースチャージ
		if( CREC.m_ddate < RVCHAR_YMD ) {
			//簡易課税６種 対象外？
			if( (rdrec.m_dsign[0]&0x40) && (rdrec.m_dsign[2]&0x0f) == 0x01 &&
				((rdrec.m_dsign[4]) == 0x1d ||(rdrec.m_dsign[4]) == 0x1e) ) {

				//サインOFF
				rdrec.m_dsign[0] = rdrec.m_dsign[1] = 0;
				rdrec.m_dsign[2] = rdrec.m_dsign[3] = rdrec.m_dsign[5] = 0;
				rdrec.m_dsign[4] = 0;
			}
		}
#endif

		*crec = rdrec;

		// 消費税サインをセット
		DB_SyIniz( crec );

		if( syzCnt ) {
			for( int n = 0; n < syzCnt; n++ ) {
				if( pDBsy->SyFnc(syCode[n]) == 0 ) {
					struct _SY_MSG_PACK2 smpk;
					pDBsy->SyMsg(&smpk);
					pDBzm->GetCDBData(crec);
				}
			}
		}
		else {
			pDBzm->GetCDBData(crec);
		}

		// 追加行イニシャライズ ＆ 再表示
		inpline_inz( &INP_LINE );					// 追加行イニシャライズ
		pn = _rd_recdsp( crec, &INP_LINE, cnd );	// 追加行表示データ作成
		inpline_dsp( &INP_LINE, APPEND_LINE );		// 追加行表示

		// 直前データのセーブ
		RdataSaveSrec( srec );
		// 直前データの作成
		prevdata( srec, APPEND_LINE );
		// カーソルポジション
//		if( !is_AutoInp_use() )

		// 08.30 /04 --- 定型仕訳の日付で カーソル 止める
		if( pAUTOSEL->RDDATE_OPT == OPT_RDDATE )
			pn = DATE_PN;
		else if( pAUTOSEL->DENP_OPT == OPT_NONOPT )	// 伝票番号 毎回入力
		{
			pn = DENP_PN;
		}
		else if( pAUTOSEL->RDBRN_OPT == OPT_RDBRN )
			pn = DEBT_PN;

		// 02.07 /05 いったんDisp(入力モード表示)に フォーカスをセットし、再度 _FocusIcsinputctrl8
		// が Callされるようにして、選択欄に摘要を表示させる
		if( pn == TKY_PN ) {
			set_nowpn( DATE_PN );
			c_set( IDC_ICSDISPCTRL10 );
		}

		AutoInp( pn );
		set_focus( pn );

	}
	return sg;

}




void CDBDinpView::RDRemainDisp( BOOL sw )
{
	RECT rct;
	GetDlgItem( IDC_ICSDISPRDREMAIN )->GetWindowRect( &rct );

//MyTrace( "RDRemainDisp %d\n", sw );

	if( !sw  )
	{
		if( rct.bottom - rct.top )
			GetDlgItem( IDC_ICSDISPRDREMAIN )->SetWindowPos( 0, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOMOVE);
	}
	else
	{
//		if( !(rct.bottom - rct.top) )
//		{
		//	double rate_w, rate_h;
		//	get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット
		//	double hight = (RDRemainRECT.bottom - RDRemainRECT.top)*rate_h;
		//	double width = (RDRemainRECT.right - RDRemainRECT.left)*rate_w;

#ifndef _SLIP_ACCOUNTS_
			int hight, width;
			CRect rect;
			GetDlgItem( IDC_ICSDISPCTRL10 )->GetWindowRect( &rect );

			hight = rect.bottom - rect.top;
			width = (RDRemainRECT.right - RDRemainRECT.left);
			GetDlgItem( IDC_ICSDISPRDREMAIN )->SetWindowPos( 0, 0, 0, width, hight, SWP_NOZORDER|SWP_NOMOVE );
#else
		int hight, width;
		CRect rect, dspRect;

		if( m_bSLACC_BUTTON_DISP == FALSE ){
			if( m_InsLine != -1 && m_InsMode == _KEYSCAN ) {

				if( IsBlkInpGnkinDevide() ) {
					GetDlgItem(IDC_ICSDISPCTRL10)->GetWindowRect(&rect);

					GetDlgItem(IDC_ICSDISPCTRL10)->GetWindowRect(&dspRect);
					ScreenToClient(&dspRect);

					hight = rect.bottom - rect.top;
					width = (rect.right - rect.left);
					GetDlgItem(IDC_ICSDISPRDREMAIN)->SetWindowPos(0, (dspRect.right + 1), dspRect.top, width, hight, SWP_NOZORDER);
				}
				else {
					GetDlgItem(IDC_ICSDISPCTRL12)->GetWindowRect(&rect);
					GetDlgItem(IDC_ICSDISPCTRL12)->GetWindowRect(&dspRect);
					ScreenToClient(&dspRect);

					hight = rect.bottom - rect.top;
					width = (rect.right - rect.left);
					GetDlgItem(IDC_ICSDISPRDREMAIN)->SetWindowPos(0, (dspRect.left), dspRect.bottom, width, hight, SWP_NOZORDER);
				}
			}
			else {
				GetDlgItem(IDC_ICSDISPCTRL10)->GetWindowRect(&rect);

				GetDlgItem(IDC_ICSDISPCTRL10)->GetWindowRect(&dspRect);
				ScreenToClient(&dspRect);

				hight = rect.bottom - rect.top;
				width = (RDRemainRECT.right - RDRemainRECT.left);
				GetDlgItem(IDC_ICSDISPRDREMAIN)->SetWindowPos(0, (dspRect.right + 1), dspRect.top, width, hight, SWP_NOZORDER);
			}
		}
		else{
			int disp_ok = 0;
			if( IsBlkInpGnkinDevide() ) {
				if( m_InsLine != -1 && m_InsMode == _KEYSCAN ) {
					GetDlgItem(IDC_ICSDISPCTRL10)->GetWindowRect(&rect);
					GetDlgItem(IDC_ICSDISPCTRL10)->GetWindowRect(&dspRect);
					ScreenToClient(&dspRect);

					hight = rect.bottom - rect.top;
					width = (rect.right - rect.left);
					GetDlgItem(IDC_ICSDISPRDREMAIN)->SetWindowPos(0, (dspRect.right + 1), dspRect.top, width, hight, SWP_NOZORDER);
					disp_ok++;
				}
			}
			if( !disp_ok ) {
				CRect btnRect;
				m_LinkSlipBtn.GetWindowRect(&btnRect);
				ScreenToClient(&btnRect);
				GetDlgItem(IDC_ICSDISPCTRL10)->GetWindowRect(&rect);

				hight = rect.bottom - rect.top;
				width = (RDRemainRECT.right - RDRemainRECT.left);
				GetDlgItem(IDC_ICSDISPRDREMAIN)->SetWindowPos(0, (btnRect.right + 1), btnRect.top, width, hight, SWP_NOZORDER);
			}
		}
#endif

//		}

		int num = m_RDTATBL.RD_cnt - m_RDTATBL.RD_trn;

		CString str;
		str.Format( _T("定型残%5u"), num );
		GetDlgItem( IDC_ICSDISPRDREMAIN )->SetWindowText( str );

	}
}

void CDBDinpView::RDRemainIniz( void )
{
	GetDlgItem( IDC_ICSDISPRDREMAIN )->GetWindowRect( &RDRemainRECT );
}

void CDBDinpView::RDRemainDispJOB( void )
{
	RDRemainDisp( ((INP_mode == _APPEND) && (m_RDTATBL.RD_trn > 0 )) ? TRUE : FALSE );
}


void CDBDinpView::prevdata( CDBINPDataRec *data, int ln )
{
	int uln, pn;
	DTLIST_DATA* udata;
	char val_txt[30] = {0};
	CString txt;

	if( (ln > 1) || (ln == APPEND_LINE) )
	{
		uln = (ln == APPEND_LINE ? SCROLL_NL() : ln - 1);

		for( pn = DENP_PN ; pn < TKY_PN+1 ; ++pn )
		{
			// 直前データのチェック
			if( _getLINE_DATA( uln )->LINE_CND[ pn ].INP_sg == FALSE )
			{
				udata = &_getLINE_DATA( uln )->_XXDTA;

				switch( pn )
				{
				case DATE_PN:
					data->m_ddate = udata->dd_ddate;
					data->m_ksign = udata->dd_ksign;
					data->m_mofs = 	udata->dd_mofs;
					continue;

				case DENP_PN:
					data->m_cno = 	udata->dd_cno;
					continue;
				case BMON_PN:
					data->m_dbmn = 	udata->dd_dbmn;
					continue;
				case CBMN_PN:
					data->m_cbmn = 	udata->dd_cbmn;
					continue;

				case KOJI_PN:
					data->m_dkno = 	udata->dd_dkno;
					continue;
				case CKJI_PN:
					data->m_ckno = 	udata->dd_ckno;
					continue;

				case DEBT_PN:
				case CRED_PN:
					if( pn == DEBT_PN )
					{
						data->m_dbt = udata->dd_dbt;
						data->m_dbr = udata->dd_dbr;
					}
					else // pn == CRED_PN
					{
						data->m_cre = udata->dd_cre;
						data->m_cbr = udata->dd_cbr;
					}
					continue;

				case VAL_PN:
					memcpy( data->m_val, udata->dd_val, sizeof(data->m_val) );
					continue;

				case ZEI_PN:
					memcpy( data->m_zei, udata->dd_zei, sizeof(data->m_zei) );
					continue;

				case TKY_PN:
					data->m_tekiyo = udata->dd_tek.tekiyo;
					continue;
				}
			}
		}
	}
}


// SREC セーブ
static CDBINPDataRec RdSV_SREC;
void CDBDinpView::RdataSaveSrec( CDBINPDataRec *srec )
{
	RdSV_SREC = *srec;
}
//SREC レストア
void CDBDinpView::RdataRstrSrec( CDBINPDataRec *srec )
{
	*srec = RdSV_SREC;
}


static int sv_pn, next_pn;
static BOOL OnREGDATA = FALSE;

// 定型仕訳ダイアログ表示状態
BOOL CDBDinpView::isRGDTDLG()
{
	return OnREGDATA;
}

//
LRESULT CDBDinpView::OnRGDTsel( WPARAM wParam, LPARAM lParam )
{
	int st = -1;
	int next_pn, sv_pn;
	RECT rect;
	
	if( INP_mode == _APPEND )
	{
		switch( wParam )
		{
		case defRDT_DIALOG_ON:	// ダイアログ表示

			Rddata_Check();

			OnREGDATA = TRUE;	// 定型仕訳モードＯＮ

			next_pn = sv_pn = get_nowpn();// 現在ポジションセーブ

			{
				rect = GetRDDlgRECT();	// 表示サイズＧｅｔ
				// 先の定型仕訳テーブルの削除
				EndRdata();

				if( m_bRGDTA_BUTTON_DISP )
				{
					CRDSel	seldlg( CRect(rect), this);
					seldlg.m_pTBL	= &m_RDTATBL;
					seldlg.m_CmnId	= m_CmnId;
					seldlg.m_LinNum = (pAUTOSEL->INP2BK_OPT == OPT_INP2BK) ? 2 : 1;
					seldlg.m_RDdate = CREC.m_ddate;
					seldlg.m_RDmofs = CREC.m_mofs;

					int zritu_mode = RDsiwakeZeirituMode();

					st = seldlg.DoModal();

					if( st == IDOK )
						st = 0;
					else
						st = -2;

					RDzeritsuModeWrite( zritu_mode );
				}
			}
			OnREGDATA = FALSE;	// 定型仕訳モードＯＦＦ

			if( st != 0 )
			{
//				in_syfnctodata( &CREC, pDBzm, pDBsy, &SyCode );	// 消費税セット
				RdselCancelJob();

				if( st == -1 )
				{
					Buzzer();	// Buzzer
				}
			}
			else	// st == 0
				PostMessage( WM_REGDATA, defRDT_GET_RGDTA ); 

			break;

		case defRDT_GET_RGDTA:	// 定型仕訳ゲット
			RdataToInpData( &CREC, &SREC );
			// 定型仕訳残り数表示
			RDRemainDispJOB();
			break;
		}
	}
	else
		Buzzer();	// Buzzer

	return 1;

}


void CDBDinpView::RdselCancelJob()
{
	CDBINPDataRec	data;

	// 一旦 科目なし+SEQ 変更で で消費税 イニシャライズ
	data.Reset();
	data.m_seq		= CREC.m_seq+1;
	data.m_ddate	= CREC.m_ddate;
	data.m_mofs		= CREC.m_mofs;
	data.m_ksign	= CREC.m_ksign;
	DB_SyIniz( &data );

	DB_SyIniz( &CREC );
}


#endif



// 定型仕訳
void CDBDinpView::OnButtonInsert()
{
	if( !isRGDTDLG() )
	{
		if( PROGRAM_OK )
		{
			if( get_nowpn() == SEQ_PN && (INP_mode == _MODEFY || INP_mode == _KEYSCAN) ) {
			//	keybd_event( VK_INSERT, 0, 0, 0 );
			//	keybd_event( VK_INSERT, 0, KEYEVENTF_KEYUP, 0 );
				TerminationIcsinputSeq( this, VK_INSERT, 0, 0 ); 
				return;
			}

			if( is_AutoInp_use() )
				return;
			else if( RemainRdata() )
			{
				int st	=	myICSMessageBox( _T("定型仕訳処理中です。\n\n処理を終わりますか？"), MB_YESNO|MB_DEFBUTTON2 );

				if( st == IDNO )
					return;
			}
			PostMessage( WM_REGDATA, defRDT_DIALOG_ON );
		}
	}
}



void CDBDinpView::OnUpdateButtonInsert(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;

	if( PROGRAM_OK && ! bCONFIRM_MASTER ) {
		if( get_nowpn() == SEQ_PN && (INP_mode == _MODEFY || INP_mode == _KEYSCAN) ) {
			if( IsMasterType(MST_INSERT) )
				bEnable = TRUE;
		}
		else {
			if( INP_mode == _APPEND )
				bEnable = TRUE;
		}
	}

	pCmdUI->Enable( bEnable );
}


//------------------------------------------------------------------
//    LAST DATA 取得
//
//------------------------------------------------------------------
void CDBDinpView::LastDataGet()
{
	if( pDBzm == NULL ) return;

	// 各ターミネーション関数を呼ぶ
	TermDistribute(0, 1);
}


// フォーク用の科目コードをUCOMにセットする
void CDBDinpView::ForkUcomSet()
{
	int pn, sgn;

	if( pDBzm == NULL )
		return;

FILETrace( "CDBDinpView::ForkUcomSet modify %d, DATA_1RD %d, %02x, %02x, m_DenpSeq=%d\n", pDBzm->IsModify(), DATA_1RD, CREC.m_dsign[0], SCAN_rec.m_dsign[0], m_DenpSeq );

	//伝票ダイアログ表示中
	if( m_DenpSeq != 0 )
		return;

	//原票読取ダイアログ中に来ることがあった
	if( ! m_bForkUCOM )
		return;

	PROGRAM_OK = FALSE;

	pn = get_nowpn();
	char buf[36] = {0}, buf1[36] = {0}, buf2[36] = {0}, buf3[36] = {0};

	if( DATA_1RD ){
		//CREC to 残高ADD.

//		BOOL bDiff = FALSE;
//		if( CREC != SCAN_rec )	bDiff = TRUE;
		int ret = rewupdat( &CREC, &SCAN_rec, INP_mode, 0, 0 );
//FILETrace( "CDBDinpView::ForkUcomSet bDiff %d, ret %d, dbt = %s, %s\n", bDiff, ret, CREC.m_dbt, SCAN_rec.m_dbt );

		//別プロセスからの戻りで、同一行データを修正するかもしれないので、
		//DATA_1RDをクリアしない
		SCAN_rec = CREC;	// 02.19 /08

		/*原票イメージ再初期化*/
		if( m_bSLIP_MASTER ){
			if( m_pImgmng != NULL ){
				if( IsDispSlip() ) {
				//	IMG_INITREC	initrec;
				//	memset( &initrec, '\0', sizeof(IMG_INITREC) );
				//	initrec.pDBZmSub = pDBzm;
				//	m_pImgmng->ReInit( initrec );

					IMG_DLGOPTION	dlgOption = {0};
					dlgOption.btnDispSw |= (0x04|0x08|0x01|0x02);
					if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
						dlgOption.btnDispSw |= (0x10|0x20);
					}
					if( INP_mode == _KEYSCAN )	dlgOption.imgDispSw |= 0x02;
			//		dlgOption.btnHideSw |= 0x01;

			//		m_pImgmng->DispImgDlgOn( pDBzm, CREC.m_seq, this, dlgOption );
			//		m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
				}
			}
		}
	}

//MyTrace( "CDBDinpView::ForkUcomSet02 modify %d, %02x, %02x\n\n", pDBzm->IsModify(), CREC.m_dsign[0], SCAN_rec.m_dsign[0] );
//MyTrace( "CDBDinpView::ForkUcomSet CREC.m_ksign %d, CREC.m_ddate = %d\n", CREC.m_ksign, CREC.m_ddate );
//MyTrace( "CDBDinpView::ForkUcomSet ZmoveMode = %d\n", ZmoveMode() );

	char sm_buf[36] = {0}, em_buf[36] = {0};
	char zandc[16] = { 0 };

	if( ZmoveMode() ) {
//		pDBzm->MakeUcomSeldate( buf, sizeof buf, buf1, sizeof buf1 );
		::ZeroMemory( buf, sizeof buf );
		::ZeroMemory( buf1, sizeof buf1 );
	}
	else {
		if( CREC.m_ksign ) {
			BYTE bcd[8] = {0};
			pDBzm->DateConv( bcd, &CREC, CV2_1 );
			int yy, dd;
			yy = CREC.m_ddate / 10000;
			dd = CREC.m_ddate % 100;

			sgn = KESY_SG_GET( bcd[0] );

//FILETrace( "CDBDinpView::ForkUcomSet bcd = %02x%02x, sgn %d\n", bcd[0], bcd[1], sgn );

			switch( sgn ) {
			case 1:	sprintf_s( buf, sizeof buf, "%04d61%02d", yy, dd );	break;
			case 2:	sprintf_s( buf, sizeof buf, "%04d62%02d", yy, dd );	break;
			case 3:	sprintf_s( buf, sizeof buf, "%04d63%02d", yy, dd );	break;
			case 4:	sprintf_s( buf, sizeof buf, "%04d00%02d", yy, dd );	break;
			}
		}
		else {
			sprintf_s( buf, sizeof buf,  "%d",CREC.m_ddate );
		}

#ifdef MINFO_BUF_CLOSE
		pDBzm->minfo->MoveFirst();
		do {
			if( (pDBzm->minfo->insw & 0x01) ) {
				if( CREC.m_ddate >= pDBzm->minfo->smd && 
					CREC.m_ddate <= pDBzm->minfo->emd ) {
					CREC.m_mofs = pDBzm->minfo->ofset;

					if( pDBzm->minfo->kes_sgn == 0 ) {
						sprintf_s( sm_buf,sizeof sm_buf, "%d",pDBzm->minfo->smd );
						sprintf_s( em_buf,sizeof em_buf, "%d",pDBzm->minfo->emd );
					}
					else {
						int yy, dd;
						yy = pDBzm->minfo->smd / 10000;
						dd = pDBzm->minfo->smd % 100;

						switch( pDBzm->minfo->kes_sgn ) {
						case 1:	sprintf_s( buf, sizeof buf, "%04d61%02d", yy, dd );	break;
						case 2:	sprintf_s( buf, sizeof buf, "%04d62%02d", yy, dd );	break;
						case 3:	sprintf_s( buf, sizeof buf, "%04d63%02d", yy, dd );	break;
						case 4:	sprintf_s( buf, sizeof buf, "%04d00%02d", yy, dd );	break;
						}

						yy = pDBzm->minfo->emd / 10000;
						dd = pDBzm->minfo->emd % 100;

						switch( pDBzm->minfo->kes_sgn ) {
						case 1:	sprintf_s( buf, sizeof buf, "%04d61%02d", yy, dd );	break;
						case 2:	sprintf_s( buf, sizeof buf, "%04d62%02d", yy, dd );	break;
						case 3:	sprintf_s( buf, sizeof buf, "%04d63%02d", yy, dd );	break;
						case 4:	sprintf_s( buf, sizeof buf, "%04d00%02d", yy, dd );	break;
						}
					}
					break;
				}
			}
		} while( pDBzm->minfo->MoveNext() == 0 );
#endif
	}

	if( pn == DEBT_PN ) {
//		sprintf_s( buf1,  sizeof buf1, "%d", CREC.m_mofs );

		if( ! CREC.m_dbt.IsEmpty() )
			sprintf_s( buf2, sizeof buf2, "%s",(LPCTSTR)CREC.m_dbt );

		UCOM_SetString_BC( _T( "共通項目１" ), buf, buf1, buf2, buf3 );

		::ZeroMemory( buf, sizeof buf );
		::ZeroMemory( buf2, sizeof buf2 );
		::ZeroMemory( buf1, sizeof buf1 );

		if( CREC.m_dbr != -1 )
			sprintf_s( buf, sizeof buf, "%d", CREC.m_dbr );
		if( CREC.m_dbmn != -1 )
			sprintf_s( buf2, sizeof buf2, "%d", CREC.m_dbmn );

		UCOM_SetString_BC( _T( "共通項目２" ), buf, buf1, buf2, buf3 );


		strcpy_s(zandc, sizeof zandc, "0");
	}
	else if( pn == CRED_PN ) {
//		sprintf_s( buf1,  sizeof buf1, "%d", CREC.m_mofs );

		if( ! CREC.m_cre.IsEmpty() )
			sprintf_s( buf2, sizeof buf2, "%s",(LPCTSTR)CREC.m_cre );

		UCOM_SetString_BC( _T( "共通項目１" ), buf, buf1, buf2, buf3 );

		::ZeroMemory( buf, sizeof buf );
		::ZeroMemory( buf2, sizeof buf2 );
		::ZeroMemory( buf1, sizeof buf1 );

		if( CREC.m_cbr != -1 )
			sprintf_s( buf, sizeof buf, "%d", CREC.m_cbr );
		if( CREC.m_cbmn != -1 )
			sprintf_s( buf2, sizeof buf2, "%d", CREC.m_cbmn );

		UCOM_SetString_BC( _T( "共通項目２" ), buf, buf1, buf2, buf3 );
		strcpy_s(zandc, sizeof zandc, "1");

		MyTrace("共通項目2 cred %s,%s,%s,%s", buf, buf1, buf2, buf3);

	}
	else {
		if( INP_mode == _APPEND ) {
			int chkpn[] = {
				DENP_PN, BMON_PN, KOJI_PN, DEBT_PN, CBMN_PN, CKJI_PN, CRED_PN, 0
			};
			int inpsgn = 0;

			for( int n = 0; chkpn[n] != 0; n++ ) {
				if( _getLINE_DATA( APPEND_LINE )->LINE_CND[ chkpn[n] ].INP_sg ) {
					inpsgn++;
					break;
				}
			}

		//	if( inpsgn ) {
				if( ZmoveMode() ) {
					::ZeroMemory( buf2, sizeof buf2 );
					UCOM_SetString_BC( _T( "共通項目１" ), buf, buf1, buf2, buf2 );
					UCOM_SetString_BC( _T( "共通項目２" ), buf2, buf2, buf2, buf2 );
				}
				else {
					//開始日付のみセット
					::ZeroMemory( buf1, sizeof buf1 );
					UCOM_SetString_BC( _T( "共通項目１" ), buf, buf1, buf1, buf1 );
					UCOM_SetString_BC( _T( "共通項目２" ), buf1, buf1, buf1, buf1 );
				}
		//	}
		//	else {
		//		//何もセットしない。
		//		::ZeroMemory( buf1, sizeof buf1 );
		//		UCOM_SetString_BC( _T( "共通項目１" ), buf1, buf1, buf1, buf1 );
		//		UCOM_SetString_BC( _T( "共通項目２" ), buf1, buf1, buf1, buf1 );
		//	}
		}
		else {
			if( ZmoveMode() ) {
				::ZeroMemory( buf2, sizeof buf2 );
				UCOM_SetString_BC( _T( "共通項目１" ), buf, buf1, buf2, buf2 );
				UCOM_SetString_BC( _T( "共通項目２" ), buf2, buf2, buf2, buf2 );
			}
			else {
				//開始日付のみセット
				::ZeroMemory( buf1, sizeof buf1 );
				UCOM_SetString_BC( _T( "共通項目１" ), buf, buf1, buf1, buf1 );
				UCOM_SetString_BC( _T( "共通項目２" ), buf1, buf1, buf1, buf1 );
			}
		}
	}

	CString proTitle;
	proTitle.LoadString( AFX_IDS_APP_TITLE );
	UCOM_SetString_BC( _T("呼び出しプログラム"), (LPVOID)(LPCTSTR)proTitle );
	UCOM_SetString_BC(_T("残問貸借チェック"), zandc);

//AfxMessageBox( "ForkUcomSet end" );

	PROGRAM_OK = TRUE;
}

int CDBDinpView::ReloadView()
{
FILETrace( "@CDBDinpView::ReloadView\n" );
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

	CDBINPDataRec key;

	if( INP_mode == _APPEND ) {
		if( m_InsLine != -1 ) {
			if( m_InsMode == _MODEFY )	disp_insswklist( 0, 0 );
			else						disp_keyscan_insswklist( 0, 0 );
		}
		else {
			//ZREC ReSet
			int i, n;
			for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
				ZREC[i].Reset();
			}

			//仕訳表示用
			pDBzm->dbdata_Requery() = TRUE;

			CDBINPDataRec key;

			key.m_seq = APPEND_SEQ.ngetseq( SEQ_NO - 1);
			if( key.m_seq <= 0 ) key.m_seq = USERS_SEQ;
		//	n = scandat_getn( &ZREC[0], &key, SCROLL_NL() );
			n = scaninsdat_getn( &ZREC[0], -1, SCROLL_NL() );

			scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );
			D_SEQN = SCROLL_NL()-1;

			INP_LINE._XXDTA.dd_seq = REAL_SEQ_NO + 1;	// 他で追加があるため SEQ更新
			CREC.m_seq = INP_LINE._XXDTA.dd_seq;

			// 08.18 /11
			if( SREC.m_seq < key.m_seq )
				SREC.m_seq = key.m_seq;
			if( NXTDTA_sg ) {
				if( NXT_rec2.m_seq != -1 && ( NXT_rec2.m_seq < key.m_seq ) )
					NXT_rec2.m_seq = key.m_seq;
			}
			// 08.19 /09
			// 直前のサンプル仕訳 SREC の修正
			set_SREC( &SREC, &ZREC[0], DDATA_LINE, SCROLL_NL(), &INP_LINE, _APPEND );
#ifdef IMPORT	// 10.14 /98
			if( NXTDTA_sg )
				set_SREC( &NXT_rec2, &ZREC[0], DDATA_LINE, SCROLL_NL(), &INP_LINE, _APPEND );
#endif
		}
	}
	else {

		//仕訳行を再表示
		// ZREC を更新
		int i, recno;
		for( i = 0; i < RECMAX; i++ ) {
			if( ZREC[i].m_seq > 0 ) {
				key = ZREC[i];

#ifdef CLOSE // 07.31 /08
				data_for( &key, &ZREC[i], 1 );
#else
				data_search( &key, &ZREC[i], 1, key.m_seq );

				// 他で仕訳が取消された場合、修正サインON
				if( ! (key.m_dsign[0] & 0x01) && (ZREC[i].m_dsign[0] & 0x01) ) {
					MODEFY_BIT.onbitn( key.m_seq - 1);
				}
#endif
				// 他で更新されているかもしれないので
				if( CREC.m_seq == key.m_seq ) {
					CREC		= ZREC[i];
					SCAN_rec	= CREC;
				}
			}
		}

		if( INP_mode == _KEYSCAN ) {
			CString tmp;
		//	DB_DataScan( tmp );
		//	pDBzm->dbdata_Reload() = FALSE;
		//	for( i = 0; i < RECMAX; i++ ) {
		//		if( ZREC[i].m_seq > 0 ) {
		//			key = ZREC[i];
		//			// 他のプログラムで 仕訳が変更されて、クエリー上からなくなった。
		//			if( data_search( &key, &ZREC[i], 1, key.m_seq ) == 0 ) {
		//				DATA_1RD = 0;
		//				gotoscankey();
		//				break;
		//			}
		//		}
		//	}
			for( i = 0; i < RECMAX; i++ ) {
				if( ZREC[i].m_seq > 0 ) {
					key = ZREC[i];
					data_search( &key, &ZREC[i], 1, key.m_seq );
					// 他で仕訳が取消された場合、修正サインON
					if( ! (key.m_dsign[0] & 0x01) && (ZREC[i].m_dsign[0] & 0x01) ) {
						MODEFY_BIT.onbitn( key.m_seq - 1);
					}
				}
			}
		}

		// データラインを更新
		struct _DATA_LINE* ddata;
		ddata = &DDATA_LINE[0];
		for( i = 0; i < DDATA_LNMAX; i++ ) {
			recno = (ddata+i)->ZREC_NO;
			if( recno > 0 ) {
				if( ZREC[(recno-1)].m_seq != -1 )
					recdsp( ZREC, (recno-1), ddata, i );	// 表示
			}
			else {
				// データ初期化
				nline_del( ddata, i );
			}
		}

		INP_LINE._XXDTA.dd_seq = REAL_SEQ_NO + 1;	// 追加分 SEQ更新

		// 新規入力の seq更新 08.19 /08
		SaveDinpData_SEQ( _APPEND, INP_LINE._XXDTA.dd_seq );
	}

	//消費税を初期化
	DB_SyIniz( &CREC );

	if( INP_mode == _APPEND && m_InsLine != -1 ) {
		//挿入モードはすでに表示済み
	}
	else {
		//データスクロール行表示
		ddata_dsp(-1);

		if( INP_mode != _KEYSCAN ) {
			inpline_dsp( &INP_LINE, APPEND_LINE );
		}
	}

	/*原票イメージ再初期化*/
	if( m_bSLIP_MASTER ){
		if( m_pImgmng != NULL ){
			IMG_INITREC	initrec;
			memset( &initrec, '\0', sizeof(IMG_INITREC) );
			initrec.pDBZmSub = pDBzm;
			m_pImgmng->ReInit( initrec );

			if( (CREC.m_dsign[6]&0x01) && m_DispSlipChk.GetCheck() ){
				IMG_DLGOPTION	dlgOption = {0};
				dlgOption.btnDispSw |= (0x04|0x08|0x01|0x02);
				if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
					dlgOption.btnDispSw |= (0x10|0x20);
				}
				if( INP_mode == _KEYSCAN )	dlgOption.imgDispSw |= 0x02;
			//	dlgOption.btnHideSw |= 0x01;

				m_pImgmng->DispImgDlgOn( pDBzm, CREC.m_seq, this, dlgOption );
				m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
			}
		}
	}
	m_Seldata.SelectUpdate();

	/* 諸口合計	*/
	g_stotal( SYOGT, (char *)SYOG_d, (char *)SYOG_c, (char *)SG_def );
	syogdsp( (char *)SG_def );

	/* 現金合計	*/
	g_stotal( /*GNKIN*/pAUTOSEL->DCKM_CODE, (char *)GENK_d, (char *)GENK_c, (char *)GK_def );
	genkdsp( (char *)GK_def );

	// 元帳検索 からの変更が元に戻るので killfocusを無視する。 [12.14 /12]
	EDIT_PN = -1;

	// フォーカスを再セット
	set_focus( get_nowpn() );

FILETrace( "@CDBDinpView::ReloadView END ------------\n" );

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////
// 表示行設定
//
#include "DtClmItem.h"


DT_CLMITEM ctLINE1[] = {
	{ 0, "", DATA_SEQ,	0, "", -1,-1,	7, 7,	0,	0, 0, -1 },
	{ 0, "伝", DATA_USER1,DT_KIND_STRING|DT_FMT_CENTER, "", -1,-1,	3, 2,	0,	-1, 0, -1 },
	{ 0, "", DATA_DATE,	0, "", -1,-1,	6, 6,	0, -1, 0, -1 },
	{ 0, "", DATA_DENP,	0, "", -1,-1,	7, 7,	0, -1, 0, -1 },

	{ 0, "", DATA_DEB_BMON,	0, "", -1,-1, 8, 8, 0, -1, 0, -1 },
	{ 0, "", DATA_DEB_KNO,	0, "", -1,-1,12,12, 0, -1, 0, -1 },

	{ 0, "", DATA_DEBT,		0, "", -1,-1, 0,0, 0, -1, 0, -1 },
	{ 0, "", DATA_CRED,		0, "", -1,-1, 0,0, 0, -1, 0, -1 },

	{ 0, "", DATA_VAL,	0, "", -1,-1,	15,15, 0, -1, 0, -1 },
	{ 0, "", DATA_ZEI,	0, "", -1,-1,	13,13, 0, -1, 0, -1 },
	{ 0, "", DATA_SYZ,	0, "", -1,-1,	8,	8, 0, -1, 0, -1 },

	{ 0, "変",	DATA_HJIYU,		0, "", -1,-1,	2,	2, 0, -1, 0, -1 },
	{ 0, "",	DATA_TEKIYO,	DT_INP_BOTTOMRIGHT, "", -1,-1,	80,80, 0, -1, 0,  -1 },
};

// 伝票 + 部門(2) + 工事(2)
DT_CLMITEM ctLN2_BK2[] = {

	{ 0, "", DATA_SEQ,	0, "", -1,-1,	7,	7,	0,	0, 0, -1 },
	{ 1, "伝", DATA_USER1, DT_KIND_STRING|DT_FMT_CENTER, "",  -1, -1, 3, 2,	0, 0, 0, 0 },
	{ 0, "", DATA_DATE,	0, "", -1,-1,	7,	7,	0, -1, 0, -1 },
	{ 1, "", DATA_DENP,	0, "", -1,-1,	7,	7,	0,  0, 0,  1 },

	{ 0, "", DATA_DEB_BMON,	0, "", -1,-1,	8,  8,  0, -1, 0,  2 },
	{ 0, "", DATA_DEB_KNO,	0, "", -1,-1,  12, 12,  0, -1, 0, -1 },
	{ 1, "", DATA_DEBT,		0, "", -1,-1,14+6,14+6, 1, -1, 0,  3 },

	{ 0, "", DATA_CRE_BMON, 0, "", -1,-1,	8,  8,  0, -1, 0,  6 },
	{ 0, "", DATA_CRE_KNO,	0, "", -1,-1,  12, 12,  0, -1, 0, -1 },
	{ 1, "", DATA_CRED,		0, "", -1,-1,14+6,14+6, 1, -1, 0,  6 },

	{ 0, "", DATA_VAL,	0, "", -1,-1,	15,	15,	0, -1, 0,  9 },
	{ 0, "", DATA_ZEI,	0, "", -1,-1,	15,	15,	0, -1, 0, -1 },
	{ 0, "", DATA_SYZ,	0, "", -1,-1,	8,	8,	0, -1, 0, -1 },
	{ 0, "", DATA_HJIYU,0, "", -1,-1,	4,	4,	0, -1, 0, -1 },

	{ 1, "", DATA_TEKIYO,	DT_INP_TOPRIGHT, "", -1,-1,	80,80,	0, -1, 0, 9 },

	{ 0, "", DATA_SYONO,	0, "", -1,-1,  20, 10,	0, -1, 0, 13 },
	{ 0, "", DATA_TGDATE,	0, "", -1,-1,	9,	9,	0, -1, 0, -1 },
	{ 0, "", DATA_HUSEN,	0, "", -1,-1,	4,	4,	0, -1, 0, -1 },
};


// インボイス対応 伝票 + 部門(2) + 工事(2)
DT_CLMITEM ctLN2_INV[] = {

	{ 0, "", DATA_SEQ,	0, "", -1,-1,	7,	7,	0,	0, 0, -1 },
	{ 1, "伝", DATA_USER1, DT_KIND_STRING | DT_FMT_CENTER, "",  -1, -1, 3, 2,	0, 0, 0, 0 },
	{ 0, "", DATA_DATE,	0, "", -1,-1,	7,	7,	0, -1, 0, -1 },
	{ 1, "", DATA_DENP,	0, "", -1,-1,	7,	7,	0,  0, 0,  1 },

	{ 0, "", DATA_DEB_BMON,	0, "", -1,-1,	8,  8,  0, -1, 0,  2 },
	{ 0, "", DATA_DEB_KNO,	0, "", -1,-1,  12, 12,  0, -1, 0, -1 },
	{ 1, "", DATA_DEBT,		0, "", -1,-1,14 + 6,14 + 6, 1, -1, 0,  3 },

	{ 0, "", DATA_CRE_BMON, 0, "", -1,-1,	8,  8,  0, -1, 0,  6 },
	{ 0, "", DATA_CRE_KNO,	0, "", -1,-1,  12, 12,  0, -1, 0, -1 },
	{ 1, "", DATA_CRED,		0, "", -1,-1,14 + 6,14 + 6, 1, -1, 0,  6 },

	{ 0, "", DATA_VAL,	0, "", -1,-1,	15,	15,	0, -1, 0,  9 },
	{ 0, "", DATA_ZEI,	0, "", -1,-1,	12,	12,	0, -1, 0, -1 },
	{ 0, "", DATA_SYZ,	0, "", -1,-1,	8,	8,	0, -1, 0, -1 },

	{ 0, "免",   DATA_USER2,	DT_KIND_STRING | DT_FMT_CENTER, "", -1,-1,	4,	2,	0, -1, 0, -1 },
	{ 0, "登録", DATA_USER3,	DT_KIND_STRING | DT_FMT_CENTER, "", -1,-1,	16,	4,	0, -1, 0, -1 },

	{ 0, "変", DATA_HJIYU,0, "", -1,-1,	2,	2,	0, -1, 0, -1 },

	{ 1, "", DATA_TEKIYO,	DT_INP_TOPRIGHT, "", -1,-1,	80,80,	0, -1, 0, 9 },

	{ 0, "", DATA_SYONO,	0, "", -1,-1,  20, 10,	0, -1, 0, 15 },
	{ 0, "", DATA_TGDATE,	0, "", -1,-1,	9,	9,	0, -1, 0, -1 },
	{ 0, "", DATA_HUSEN,	0, "", -1,-1,	4,	4,	0, -1, 0, -1 },
};




//証憑番号対応 -------------------------------------------------------------------------
DT_CLMITEM snoLINE1[] = {
	{ 0, "", DATA_SEQ,	0, "", -1,-1,	7, 7,	0,	0, 0, -1 },
	
	{ 0, "伝", DATA_USER1, DT_KIND_STRING|DT_FMT_CENTER, "", -1,-1,	3, 2,	0,	-1, 0, -1 },

	{ 0, "", DATA_DATE,	0, "", -1,-1,	6, 6,	0, -1, 0, -1 },
	{ 0, "", DATA_DENP,	0, "", -1,-1,	7, 7,	0, -1, 0, -1 },

	{ 0, "", DATA_DOCEVI,	0, "", -1,-1,	8, 8,	0, -1, 0, -1 },

	{ 0, "", DATA_DEB_BMON,	0, "", -1,-1, 8, 8, 0, -1, 0, -1 },
	{ 0, "", DATA_DEB_KNO,	0, "", -1,-1,12,12, 0, -1, 0, -1 },

	{ 0, "", DATA_DEBT,		0, "", -1,-1, 0,0, 0, -1, 0, -1 },
	{ 0, "", DATA_CRED,		0, "", -1,-1, 0,0, 0, -1, 0, -1 },

	{ 0, "", DATA_VAL,	0, "", -1,-1,	15,15, 0, -1, 0, -1 },
	{ 0, "", DATA_ZEI,	0, "", -1,-1,	13,13, 0, -1, 0, -1 },
	{ 0, "", DATA_SYZ,	0, "", -1,-1,	8,	8, 0, -1, 0, -1 },

	{ 0, "変",	DATA_HJIYU,		0, "", -1,-1,	2,	2, 0, -1, 0, -1 },
	{ 0, "",	DATA_TEKIYO,	DT_INP_BOTTOMRIGHT, "", -1,-1,	80,80, 0, -1, 0,  -1 },
};

// 伝票 + 部門(2) + 工事(2)
DT_CLMITEM snoLN2_BK2[] = {

	{ 0, "", DATA_SEQ,	0, "", -1,-1,	7,	7,	0,	0, 0, -1 },

	{ 0, "伝", DATA_USER1, DT_KIND_STRING|DT_FMT_CENTER, "", -1,-1,	3, 2,	0,	-1, 0, -1 },

	{ 0, "", DATA_DATE,	0, "", -1,-1,		7,	8,	0, -1, 0, -1 },
	{ 1, "", DATA_DENP,	0, "",		-1,-1,	7,	7,	0, 0, 0, -1 },

	{ 1, "", DATA_DOCEVI,	0, "",	-1,-1,	8,	8,	0, -1, 0, 1 },

	{ 0, "", DATA_DEB_BMON,	0, "", -1,-1,	8,  8,  0, -1, 0,  2 },
	{ 0, "", DATA_DEB_KNO,	0, "", -1,-1,  12, 12,  0, -1, 0, -1 },
	{ 1, "", DATA_DEBT,		0, "", -1,-1,14+6,14+6, 1, -1, 0,  4 },

	{ 0, "", DATA_CRE_BMON, 0, "", -1,-1,	8,  8,  0, -1, 0,  6 },
	{ 0, "", DATA_CRE_KNO,	0, "", -1,-1,  12, 12,  0, -1, 0, -1 },
	{ 1, "", DATA_CRED,		0, "", -1,-1,14+6,14+6, 1, -1, 0,  6 },

	{ 0, "", DATA_VAL,	0, "", -1,-1,	15,	15,	0, -1, 0,  9 },
	{ 0, "", DATA_ZEI,	0, "", -1,-1,	15,	15,	0, -1, 0, -1 },
	{ 0, "", DATA_SYZ,	0, "", -1,-1,	8,	8,	0, -1, 0, -1 },
	{ 0, "", DATA_HJIYU,0, "", -1,-1,	4,	4,	0, -1, 0, -1 },

	{ 1, "", DATA_TEKIYO,	DT_INP_TOPRIGHT, "", -1,-1,	80,80,	0, -1, 0,  10 },

	{ 0, "", DATA_SYONO,	0, "", -1,-1,  20, 10,	0, -1, 0, 14 },

	{ 0, "", DATA_TGDATE,	0, "", -1,-1,	9,	9,	0, -1, 0, -1 },
	{ 0, "", DATA_HUSEN,	0, "", -1,-1,	4,	4,	0, -1, 0, -1 },
};


DT_CLMITEM*	GetDtListColumn( int type, int& clmax )
{
	DT_CLMITEM* pClm = NULL;
	switch( type )
	{
	case DTCLM_LINE1:
		pClm = ctLINE1;
		clmax = sizeof ctLINE1 / sizeof ctLINE1[0];
		break;
	case DTCLM_LINE2:
		pClm = ctLN2_BK2;
		clmax = sizeof ctLN2_BK2 / sizeof ctLN2_BK2[0];
		break;
	case DTCLM_SNOL1:
		pClm = snoLINE1;
		clmax = sizeof snoLINE1 / sizeof snoLINE1[0];
		break;
	case DTCLM_SNOL2:
		pClm = snoLN2_BK2;
		clmax = sizeof snoLN2_BK2 / sizeof snoLN2_BK2[0];
		break;
	default:
		clmax = 0;
		break;
	}
	return pClm;
}


// リストのタイトル文字列の設定
void CDBDinpView::SetListTitle()
{
	DT_CLMITEM* pDisp;
	int	i, max, id;

	if( bDOCEVI_Master ) {
		max = (sizeof snoLINE1 / sizeof snoLINE1[0]);
		pDisp = snoLINE1;
	}
	else {
		max = (sizeof ctLINE1 / sizeof ctLINE1[0]);
		pDisp = ctLINE1;
	}

	for( i = 0; i < max; i++ ) {
		id = pDisp[i].id;

		if( id == DATA_HJIYU ) {
			// 変動事由・振替科目区分[公益] タイトル
			pDisp[i].title = (char*)pDBzm->get_hjiyutitle(1);
			break;
		}
	}

	if( bDOCEVI_Master ) {
		max = (sizeof snoLN2_BK2 / sizeof snoLN2_BK2[0]);
		pDisp = snoLN2_BK2;
	}
	else {
		max = (sizeof ctLN2_BK2 / sizeof ctLN2_BK2[0]);
		pDisp = ctLN2_BK2;
	}

	for( i = 0; i < max; i++ ) {
		id = pDisp[i].id;

		if( id == DATA_HJIYU ) {
			// 変動事由・振替科目区分[公益] タイトル
			pDisp[i].title = (char*)pDBzm->get_hjiyutitle(0);
			break;
		}
	}

}

// 証憑番号表示時の カラム幅調整
void CDBDinpView::ColumnSizeAdjust( int& bmon, int& koji, int& syonum )
{
	bmon = koji = syonum = -1;

	if( ! bDOCEVI_Master )
		return;

	// 建設マスター
	if( Voln1->apno == 0x20 ) {
		if( (Voln1->sub_sw&0x02) && (Voln1->sub_sw&0x04) ) {
			int snocol = Voln1->snocol;
			int bmcol = Voln1->bmcol;
			int kjcol = Voln1->kjcol;

			int edcol = Voln1->edcol;
			if( edcol < 4 )	edcol = 4;

			int tmp;
			tmp = snocol + bmcol + kjcol + (edcol*2);

			// サイズ変更を行う
			if( tmp > 32 ) {
				snocol = 4;

				for( int i = 0; ; i++ ) {
					tmp = snocol + bmcol + kjcol + (edcol*2);

					if( tmp <= 32 )
						break;
			
					if( (i%2) == 0 ) {
						kjcol -= 2;
					}
					else {
						bmcol--;
					}
				}
				bmon	= bmcol;
				koji	= kjcol;
				syonum	= snocol;
			}
		}
	}
}

// DT_COLUMN 情報を取得する
int GetDtListColumn(int nLine, CArray<DT_COLUMN, DT_COLUMN&>& clm, int inv/*= 0*/)
{
	int i, max, cnt, edlen, id;
	DT_COLUMN dtcol;
	DT_CLMITEM* pDisp;

	edlen = Voln1->edcol;
	if (edlen < 4)	edlen = 4;
	cnt = 0;

	if (nLine == 1) {
		int bmln, kjln, syoln;
		p_DBDinpView->ColumnSizeAdjust(bmln, kjln, syoln);

		if (bDOCEVI_Master) {
			max = (sizeof snoLINE1 / sizeof snoLINE1[0]);
			pDisp = snoLINE1;
		}
		else {
			max = (sizeof ctLINE1 / sizeof ctLINE1[0]);
			pDisp = ctLINE1;
		}

		clm.SetSize(max);

		for (i = cnt = 0; i < max; i++) {
			id = pDisp[i].id;

			if (id == DATA_DENP) {
				if (pAUTOSEL->DSPDENP_OPT == OPT_NODSP)
					continue;
			}
			if (id == DATA_DEB_BMON) {
				if (pAUTOSEL->DSPBMON_OPT == OPT_NODSP)
					continue;
				if (!(Voln1->sub_sw & 0x02))
					continue;

				pDisp[i].inpsize = Voln1->bmcol;
				pDisp[i].dispsize = Voln1->bmcol;
			}
			if (id == DATA_DEB_KNO) {
				if (Voln1->apno == 0x20 && (Voln1->sub_sw & 0x04)) {
					if (pAUTOSEL->DSPKOJI_OPT == OPT_NODSP)
						continue;
				}
				else {
					continue;
				}
				pDisp[i].inpsize = Voln1->kjcol;
				pDisp[i].dispsize = Voln1->kjcol;
			}
			if (id == DATA_DEBT || id == DATA_CRED) {
				pDisp[i].inpsize = 14 + edlen;
				pDisp[i].dispsize = 14 + edlen;
			}

			if (id == DATA_USER1) {
				if (!DenpTypeMode())
					continue;
			}
			//証憑カラム調整
			if (bDOCEVI_Master) {
				if (Voln1->apno == 0x20) {
					if (id == DATA_DEB_BMON) {
						if (bmln != -1) {
							pDisp[i].dispsize = bmln;
						}
					}
					if (id == DATA_DEB_KNO) {
						if (kjln != -1) {
							pDisp[i].dispsize = kjln;
						}
					}
					if (id == DATA_DOCEVI) {
						if (syoln != -1) {
							pDisp[i].dispsize = syoln;
						}
						else {
							pDisp[i].inpsize = Voln1->snocol;
							pDisp[i].dispsize = Voln1->snocol;
						}
					}
				}
				else {
					if (id == DATA_DOCEVI) {
						pDisp[i].inpsize = Voln1->snocol;
						pDisp[i].dispsize = Voln1->snocol;
					}
				}
			}

			if (id == DATA_HJIYU) {
				if (IsHjiyuItemGyosyu() == 0)  continue;
				//	if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ ) continue;

				if (pAUTOSEL->DSPHJIYU_OPT == OPT_NODSP)
					continue;
			}
			// 税額		09.20 /07
			if (id == DATA_ZEI) {
				if (pAUTOSEL->DSPZEI_OPT == OPT_NODSP)
					continue;
			}
			// 消費税	09.02 /08
			if (id == DATA_SYZ) {
				if (pAUTOSEL->DSPSYZ_OPT == OPT_NODSP)
					continue;

				if (CheckWidthSyohi())
					pDisp[i].dispsize = 8 + 2;
				else
					pDisp[i].dispsize = 8;
			}

			dtcol.ln_ofs = pDisp[i].ln_ofs;
			dtcol.clm_title = pDisp[i].title;
			dtcol.clm_id = pDisp[i].id;
			dtcol.clm_type = pDisp[i].type;
			dtcol.clm_fmt = pDisp[i].format;
			dtcol.clm_back = pDisp[i].back;
			dtcol.clm_text = pDisp[i].text;
			dtcol.clm_inpsz = pDisp[i].inpsize;
			dtcol.clm_size = pDisp[i].dispsize;
			//		dtcol.clm_size	= 7;
			dtcol.clm_szadj = pDisp[i].size_adj;
			dtcol.clm_spos = pDisp[i].spos;
			dtcol.clm_adj = pDisp[i].spos_adj;
			dtcol.clm_sidx = pDisp[i].clm_sidx;

			clm.SetAt(cnt, dtcol);
			cnt++;
		}
	}
	else {
		// 2行うち
		if( !inv ) {
		if (bDOCEVI_Master) {
			max = (sizeof snoLN2_BK2 / sizeof snoLN2_BK2[0]);
			pDisp = snoLN2_BK2;
		}
		else {
			max = (sizeof ctLN2_BK2 / sizeof ctLN2_BK2[0]);
			pDisp = ctLN2_BK2;
		}
		}
		else {
			if( bDOCEVI_Master ) {
				max = (sizeof ctLN2_INV / sizeof ctLN2_INV[0]);
				pDisp = ctLN2_INV;
			}
			else {
				max = (sizeof ctLN2_INV / sizeof ctLN2_INV[0]);
				pDisp = ctLN2_INV;
			}
		}
		clm.SetSize(max);

		int bmnkoji_lng, kmk_lng, nMax;
		bmnkoji_lng = Voln1->bmcol + 12;
		kmk_lng = 14 + Voln1->edcol;

		nMax = __max(bmnkoji_lng, kmk_lng);

		int	adj = 0;
		//区切り位置の項目
		int dbt_idx = -1;
		int cre_idx = -1;
		int hjiyu_idx = -1;

		int koji_dsp = 0;
		if (Voln1->apno == 0x20) {
			if ((Voln1->sub_sw & 0x04) && pAUTOSEL->DSPKOJI_OPT != OPT_NODSP) {
				koji_dsp = 1;
			}
		}

		for (i = cnt = 0; i < max; i++) {
			id = pDisp[i].id;

			if (id == DATA_USER1) {
				if (!DenpTypeMode()) {
					adj = -1;
					continue;
				}
			}
			//インボイス
			if( inv ) {
				if( !bInvMaster || !bInvDisp || !bInvUse ) {
					if( id == DATA_USER3 ) {	//登録番号
						continue;
					}
				}
				if( !bInvMaster || !bInvUse ) {
					if( id == DATA_USER2 ) {	//免税
						continue;
					}
				}
			}
			if (id == DATA_DENP) {
				if (pAUTOSEL->DSPDENP_OPT == OPT_NODSP) {
					id = DATA_NONE;
				}
			}

			if (id == DATA_DEBT) {
				if (dbt_idx == -1) {
					dbt_idx = cnt;
					//伝票タイプ非表示の調整分
					if (!DenpTypeMode()) dbt_idx++;
				}
			}
			if (id == DATA_CRED) {
				if (cre_idx == -1) {
					cre_idx = cnt;
					if (!DenpTypeMode()) cre_idx++;
				}
			}
			if (id == DATA_HJIYU) {
				hjiyu_idx = cnt;
				if (!DenpTypeMode()) hjiyu_idx++;
			}

			if (id == DATA_CRE_BMON || id == DATA_CRED) {
				pDisp[i].clm_sidx = dbt_idx;
			}

			if (id == DATA_VAL || id == DATA_TEKIYO) {
				pDisp[i].clm_sidx = cre_idx;
			}
			if (id == DATA_SYONO) {
				pDisp[i].clm_sidx = hjiyu_idx;
			}

			if (id == DATA_DEB_BMON || id == DATA_CRE_BMON) {
				if (pAUTOSEL->DSPBMON_OPT == OPT_NODSP || !(Voln1->sub_sw & 0x02)) {
					id = DATA_NONE;
				}
				if (!koji_dsp ) {//工事がないときは枠幅広げる
					pDisp[i].inpsize = nMax;
					pDisp[i].dispsize = pDisp[i].inpsize;

					pDisp[i].type &= ~DT_FMT_CENTER;
					pDisp[i].type |= DT_FMT_RIGHT;
				}
				else {
					pDisp[i].inpsize = nMax - 12;
					pDisp[i].dispsize = pDisp[i].inpsize;
				}
			}
			if (id == DATA_DEB_KNO || id == DATA_CRE_KNO) {
				if (!koji_dsp) {
					continue;
					//		id = DATA_NONE;
					//		pDisp[i].dispsize = 0;
					//		pDisp[i].inpsize = 0;
				}
			}
			if (id == DATA_DEBT || id == DATA_CRED) {
				pDisp[i].inpsize = nMax;
				pDisp[i].dispsize = nMax;
				pDisp[i].size_adj = koji_dsp ? 1 : 0;
			}
			if (id == DATA_SYONO) {
				if (pAUTOSEL->DSPTGNO_OPT == OPT_NODSP) {
					id = DATA_NONE;
				}
			}
			if (id == DATA_TGDATE) {
				if (pAUTOSEL->DSPTGDT_OPT == OPT_NODSP) {
					id = DATA_NONE;
				}
			}
			if (id == DATA_HJIYU) {
#ifdef OLD_CLOSE
				if (pAUTOSEL->DSPHJIYU_OPT == OPT_NODSP || IsOKGyosyu(Voln1->apno, SW_SOCIAL | SW_SYUKYO)/* == 0x52*/) {
#endif
					if (pAUTOSEL->DSPHJIYU_OPT == OPT_NODSP) {
						id = DATA_NONE;
					}
					else if (!IsHjiyuItemGyosyu())	id = DATA_NONE;
					//	else if( ! IsSyafuKaisei( pDBzm->zvol ) ) {
					//		if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ ) id = DATA_NONE;	
					//	}
			}
			// 税額		09.20 /07
			if (id == DATA_ZEI) {
				if (pAUTOSEL->DSPZEI_OPT == OPT_NODSP) {
					id = DATA_NONE;
				}
			}
			// 消費税	09.02 /08
			if (id == DATA_SYZ) {
				if (pAUTOSEL->DSPSYZ_OPT == OPT_NODSP) {
					id = DATA_NONE;
				}
				if (CheckWidthSyohi())
					pDisp[i].dispsize = 8 + 2;
				else
					pDisp[i].dispsize = 8;
			}

			dtcol.ln_ofs = pDisp[i].ln_ofs;
			if (id != DATA_NONE) {
				dtcol.clm_title = pDisp[i].title;
			}
			else {
				dtcol.clm_title.Empty();
			}
			dtcol.clm_id = id;
			dtcol.clm_type = pDisp[i].type;
			dtcol.clm_fmt = pDisp[i].format;
			dtcol.clm_back = pDisp[i].back;
			dtcol.clm_text = pDisp[i].text;
			dtcol.clm_inpsz = pDisp[i].inpsize;
			dtcol.clm_size = pDisp[i].dispsize;
			dtcol.clm_szadj = pDisp[i].size_adj;
			dtcol.clm_spos = pDisp[i].spos;
			dtcol.clm_adj = pDisp[i].spos_adj;
			if (pDisp[i].clm_sidx != -1) {
				dtcol.clm_sidx = pDisp[i].clm_sidx + adj;
			}
			else
				dtcol.clm_sidx = -1;

			clm.SetAt(cnt, dtcol);
			cnt++;
		}
	}
	
	return cnt;
}


void CDBDinpView::SetListColumn( int line )
{
	CArray<DT_COLUMN, DT_COLUMN&> clm;
	int cnt, edlen;
	DT_COLUMN dtcol;

	edlen = Voln1->edcol;
	if( edlen < 4 )	edlen = 4;

	DWORD st = m_DtList.GetStyle();
	
	cnt = GetDtListColumn(line, clm);

	if( line == 1 ) {
		st &= ~DS_BK_DEBCRE;
		m_DtList.SetStyle( st );
		m_DtList.SetItemColumn( &clm[0], cnt );
	}
	else {
		// 2行うち
		st |= DS_BK_DEBCRE;
		m_DtList.SetStyle( st );
		m_DtList.SetItemColumn( &clm[0], cnt );

//MyTrace("dbt_idx = %d, cre_idx = %d  hjiyu_idx = %d\n", dbt_idx, cre_idx, hjiyu_idx);
//for (int i = 0; i < cnt; i++) {
//	MyTrace("clm[%2d]:, id = %d, clm_sidx = %d\n", i, clm[i].clm_id, clm[i].clm_sidx);
//}
	}

	DT_ITEMMAX im;
	im.im_seq = 8;
	im.im_denp = 7;
	im.im_eda = Voln1->edcol;
	im.im_bmn = Voln1->bmcol;
	im.im_koji= Voln1->kjcol;
	im.im_tky = (Voln1->tk_ln * 2);
	im.im_sno= Voln1->snocol;

	m_DtList.SetImax( &im );
}


LRESULT CDBDinpView::OnDateSelectMessageChangeMNTHSEL(WPARAM wParam, LPARAM lParam)
{
	//処理を追加
	int sofs, eofs, sy, ey;
	m_selDate.GetMonthOffset(sofs, eofs, sy, ey);
	pDBzm->set_datelimit( sofs, eofs, sy, ey );

//FILETrace( "OnDateSelectMessageChangeMNTHSEL\n" );

	// 新規入力ラインの日付のチェック
	if( INP_mode == _APPEND || INP_mode == _MODEFY ) {
		struct _DATA_LINE* ldata;
		ldata = _getLINE_DATA( APPEND_LINE );
		BYTE	check[4] = {0};

		int sgn = KESY_SG_GET( ldata->BcdDate[0] );
		switch( sgn ) {
		case 1 :	check[0] = 0x61;	break;
		case 2 :	check[0] = 0x62;	break;
		case 3 :	check[0] = 0x63;	break;
		case 4 :	check[0] = 0x00;	break;
		default:
			check[0] = ldata->BcdDate[0];	break;
		}
		check[1] = ldata->BcdDate[1];

		if( ! pDBzm->check_datelimit( check ) ) {
			pDBzm->get_ofsdate( eofs, ldata->BcdDate );
			pDBzm->SetInpDate( ldata->BcdDate );	// 入力日を更新

//FILETrace( "==New Date %02x%02x\n", ldata->BcdDate[0], ldata->BcdDate[1] );

			ldata->_XXDTA.dd_mofs = eofs;
			sgn = KESY_SG_GET( ldata->BcdDate[0] );
			ldata->_XXDTA.dd_ksign = sgn ? 1 : 0;
			pDBzm->BcdDateToYmd( ldata->BcdDate, &ldata->_XXDTA.dd_ddate );
			inpline_dsp( &INP_LINE, APPEND_LINE );

			if( INP_mode == _APPEND ) {
				CREC.m_ddate	= ldata->_XXDTA.dd_ddate;
				CREC.m_mofs		= ldata->_XXDTA.dd_mofs;
				CREC.m_ksign	= ldata->_XXDTA.dd_ksign;

				if( get_nowpn() == DATE_PN ) {
					VARIANT var;
					BYTE	bcd[10] = {0};
					CString str;
					memcpy( bcd, ldata->BcdDate, sizeof ldata->BcdDate );

					if( ! m_bMnthFix ) {
						var.pbVal = bcd;
						exDBdata_set( DATE_PN, getINP_CTL(this, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 4 );
					}
					else  {
						var.pbVal = &bcd[1];
						exDBdata_set( DATE_PN, getINP_CTL(this, DATE_PN)->IDC_X, &var, ICSDBEDT_TYPE_BCD, 2 );
					}

					pDBzm->BcdDateToString( str, ldata->BcdDate, m_bMnthFix );
					exDBSetDispString( DATE_PN, getINP_CTL(this, DATE_PN)->IDC_X, str );
				}
			}
		}
	}

	// このハンドラにきたときはFOCUSを再セットする( DateInputChange() は中で行っている)
	BOOL bFocusSet = FALSE;
	if( ! m_bMnthFix ) {
		if( sofs == eofs ) {
			if( get_nowpn() == DATE_PN ) {
				DateInputChange();
				bFocusSet = TRUE;
			}
			else {
				m_bMnthFix = TRUE;
			}
		}
	}
	else {
		if( sofs != eofs ) {
			if( get_nowpn() == DATE_PN ) {
				DateInputChange();
				bFocusSet = TRUE;
			}
			else {
				m_bMnthFix = FALSE;
			}
		}
	}

	if( ! bFocusSet ) {
		int pn = get_nowpn();
		if( pn != -1 ) {
			set_focus( get_nowpn() );

			if( INP_mode == _APPEND ) {
				switch( pn ) {
				case DENP_PN:	case DOCEVI_PN:
				case BMON_PN:	case CBMN_PN:
				case KOJI_PN:	case CKJI_PN:
				case TKY_PN:
				case ZEI_PN:	case HJIYU_PN:
				case SYON_PN:	case TGDT_PN:
					{
						//消費税再表示
						getLINE_DATA()->_XXDTA.dd_syz_str = sy_chk_txt( &CREC, NULL, INP_mode );
						memcpy( getLINE_DATA()->_XXDTA.dd_zei, CREC.m_zei, 6 );
						int pntbl[] = { ZEI_PN, SKBN_PN };

						if( is_syohizeisiwake( &CREC ) ) {
							getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = TRUE;
						}
						else {
							getLINE_DATA()->LINE_CND[ZEI_PN].INP_sg = FALSE;
						}
						TerminationDataSet( pntbl, 2 );
					}
					break;
				}
			}
		}
	}

	return	0L;
	return LRESULT();
}

void CDBDinpView::OnButtonF2()
{

	if( !PROGRAM_OK )
		return;

	int ln, pn;
	ln = get_nowln();
	pn = get_nowpn();

	if( pn == TKY_PN ) {
		short pos;
		int idc;
		// 摘要は先頭文字へ
		idc = getINP_CTL(TKY_PN)->IDC_X;

		pos = ((CICSDBEDT*)GetDlgItem(idc))->GetCaretPosition();
		
		// すでに先頭にあるときは、前項目
		if( pos > 0 ) {
			((CICSDBEDT*)GetDlgItem(idc))->SetCaretPosition(0);
			return;
		}

		if( /*m_OneHusen != 0*/ m_pSubDtInp->IsDisplay() ) {
			m_pSubDtInp->F2Job();
			return;
		}
	}
	else if( m_pBkInp != NULL ) {
		// 部門・工事処理中
		if( m_pBkInp->bk_F2job() )
			return;
	}
	// 電卓ウィンドウ消去
	CalcMouseEvent();

	set_termfocus( ln, pn, -1 );
}

void CDBDinpView::OnUpdateButtonF2(CCmdUI *pCmdUI)
{
	if( ! PROGRAM_OK )	pCmdUI->Enable( FALSE );
	else {
		if( get_nowpn() == TKY_PN ) {
			static short pos = -1;
			short	tmp;
			int idc;
			idc = getINP_CTL(TKY_PN)->IDC_X;
			tmp = ((CICSDBEDT*)GetDlgItem(idc))->GetCaretPosition();

			// この判定を入れないと CPU使用率が 100%になる
			if( pos != tmp ) {
				// ツールバーの表示文字を先頭文字 or 前項目 へ
				pos = tmp;
#ifndef _SLIP_ACCOUNTS_
				((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(1, pos == 0 ? 1 : 0);
#else
				if( IsDispSlip() == FALSE ){
					((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(1, pos == 0 ? 1 : 0);
				}
				else{
					((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(7, pos == 0 ? 1 : 0);
				}
#endif
			}
		}

		pCmdUI->Enable( TRUE );
	}
}


int CDBDinpView::TabClickChangedPrevView()
{
//FILETrace( "XXX--- CDBDinpView::TabClickChangedPrevView()\n" );

//	m_DtList.SetFocus();
	c_set( IDC_ICSDISPCTRL10 );

/*
	DT_EDIT edit;

	edit.hdr.hwndFrom = m_Input.GetSafeHwnd();
	edit.hdr.idFrom   = IDC_ICSDBEDTCTRL1;
	edit.hdr.code     = DLN_ENDLABELEDIT;
	edit.nChar = 0;

	m_DtLine.SendMessage(WM_DTLISTUSER, DLN_ENDLABELEDIT, (LPARAM)&edit );

	edit.hdr.hwndFrom = m_Inp2.GetSafeHwnd();
	edit.hdr.idFrom   = IDC_ICSDBEDTCTRL2;
	edit.hdr.code     = DLN_ENDLABELEDIT;
	edit.nChar = 0;

	m_DtList.SendMessage(WM_DTLISTUSER, DLN_ENDLABELEDIT, (LPARAM)&edit );
*/


	return 0;
}


// マウスホイール をキーボードに変換
BOOL CDBDinpView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	CWnd* pwnd;
	pwnd = GetFocus();

	if( pwnd->GetSafeHwnd() ) {
		int id, key = 1;
		id = pwnd->GetDlgCtrlID();
		if( id == IDC_ICSDBEDTCTRL1 || id == IDC_ICSDBEDTCTRL2 ) {

			//2回連続で来るので2回目は無視
			static int wheelcnt = 0;
			if( ((CMainFrame*)GetDBDinpMainFrame())->GetMouseRoutingInfo() ) {
				if( wheelcnt == 0 ) {
					key = 1;
					wheelcnt++;
				}
				else {
					key = 0;
					wheelcnt = 0;
				}
			}
			if( key ) {
				if( zDelta > 0 ) {
					keybd_event(VK_UP, 0, KEYEVENTF_EXTENDEDKEY, 0);
					keybd_event(VK_UP, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
				}
				else {
					keybd_event(VK_DOWN, 0, KEYEVENTF_EXTENDEDKEY, 0);
					keybd_event(VK_DOWN, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
				}
			}
			return FALSE;
		}
	}

	return CDBbaseView::OnMouseWheel(nFlags, zDelta, pt);
}


// 月指定
void CDBDinpView::OnButtonF6()
{
	CMnthDlg	dlg;

	// 電卓ウィンドウ消去
	CalcMouseEvent();

	int st = dlg.DoModal();

	if( st == IDOK ) {
		m_selDate.SetMonthOffset( dlg.m_Sofs, dlg.m_Eofs, dlg.m_Sdate, dlg.m_Edate );
		OnDateSelectMessageChangeMNTHSEL(0,0);
	}
}

void CDBDinpView::OnUpdateButtonF6(CCmdUI *pCmdUI)
{
	if( ! PROGRAM_OK || bCONFIRM_MASTER )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
	pCmdUI->Enable( TRUE );
}


// 部門番号が変更されたとき、科目名称の枝番表示を変更する
void CDBDinpView::bmneda_update(CDBINPDataRec* rec, int dc_sw)
{
	if( !BMON_MST )	return;

	if( !BRmst )	return;

	if( pAUTOSEL->BMNTKBR_OPT != OPT_CHKON )
		return;

	if( pAUTOSEL->BRTEK_OPT != OPT_DSP )
		return;

	struct _DATA_LINE* ldata;
	ldata = getLINE_DATA();

	if( dc_sw == 0 ) {
		if( ldata->_XXDTA.dd_dbr == -1 )
			return;
	}
	else if( dc_sw == 1 ) {
		if( ldata->_XXDTA.dd_cbr == -1 )
			return;
	}
	else {
		if( ldata->_XXDTA.dd_dbr == -1 && ldata->_XXDTA.dd_cbr == -1 )
			return;
	}

	int ln = get_nowln();

	if( dc_sw == -1 ) {
		// 枝番等セット
		set_etcdsp(ln, DEBT_PN);
		set_etcdsp(ln, CRED_PN);
		int pntbl[] = { DEBT_PN, CRED_PN };
		TerminationDataSet(pntbl, 2);
	}
	else {
		int pn = dc_sw == 0 ? DEBT_PN : CRED_PN;
		set_etcdsp(ln, pn);
		TerminationDataSet(pn);
	}
}


// 工事番号を仕訳摘要へ
void CDBDinpView::bmnkoji_to_tekiyo( CDBINPDataRec* rec, CString* meisyo, int type/* = 0*/ )
{
	if( type ) {
		if( pAUTOSEL->KJTOTKY_OPT != OPT_KJTOTKY )
			return;
	}
	else {
		if( pAUTOSEL->BMNTOTKY_OPT != OPT_CHKON )
			return;
	}


	if( getLINE_DATA()->LINE_CND[ TKY_PN ].IMG_sg == 0 ) {
		CString tmp;
		tmp = rec->m_tekiyo;

		if( ! IsIncludeString( tmp, *meisyo ) ) {
			tmp += *meisyo;
			int len;
			len = tmp.GetLength();

			if( len > (Voln1->tk_ln * 2) ) {
				LPSTR p = tmp.GetBuffer(256);
				int n = get_strcnt( p, (Voln1->tk_ln * 2) );

				*(p + n) = '\0';
				tmp.ReleaseBuffer();
			}

			rec->m_tekiyo = tmp;
			getLINE_DATA()->_XXDTA.dd_tek.tekiyo = tmp;
			TerminationDataSet( TKY_PN );
		}
	}
}

//
// 画面切り替え時に、項目の表示を更新する。
//
void CDBDinpView::UpdateDataLineDisp( int* set_pn )
{
	DT_RECT* dt; 
	int n, stchg = 0;
	int bm_chk, kj_chk, hjiyu_chk, zei_chk, denp_chk, syz_chk;
	BOOL bBmon, bKoji, bHjiyu, bZei, bDenp, bSyz;

	dt = m_DtList.GetDtRect();

	bDenp = (pAUTOSEL->DSPDENP_OPT == OPT_NONOPT);

	if( BMON_MST )	bBmon = (pAUTOSEL->DSPBMON_OPT == OPT_NONOPT);
	else			bBmon = FALSE;

	if( M_KOJI )	bKoji = (pAUTOSEL->DSPKOJI_OPT == OPT_NONOPT);
	else			bKoji = FALSE;

	if( IsHjiyuItemGyosyu() == 0 )	bHjiyu = FALSE;
//	if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ )	bHjiyu = FALSE;
	else {
		bHjiyu	= (pAUTOSEL->DSPHJIYU_OPT == OPT_NONOPT);
	}
	bZei	= (pAUTOSEL->DSPZEI_OPT == OPT_NONOPT);

	bSyz	= (pAUTOSEL->DSPSYZ_OPT == OPT_NONOPT);

	bm_chk = kj_chk = hjiyu_chk = zei_chk = denp_chk = syz_chk = 0;

	// 部門・工事・変動事由・税額のチェック
	for( n = 0; n < dt->Dtcnt; n++ ) {
		if( dt->DtID[n] == DATA_DEB_BMON || dt->DtID[n] == DATA_DEB_BMON ) {
			if( bBmon != dt->DtEnable[n] ) {
				stchg++;
			}
			bm_chk++;
		}

		if( dt->DtID[n] == DATA_DEB_KNO || dt->DtID[n] == DATA_DEB_KNO ) {
			if( bKoji != dt->DtEnable[n] ) {
				stchg++;
			}
			kj_chk++;
		}
		
		if( dt->DtID[n] == DATA_HJIYU ) {
			if( bHjiyu != dt->DtEnable[n] ) {
				stchg++;
			}
			hjiyu_chk++;
		}

		if( dt->DtID[n] == DATA_ZEI ) {
			if( bZei != dt->DtEnable[n] ) {
				stchg++;
			}
			zei_chk++;
		}
		if (dt->DtID[n] == DATA_SYZ) {
			if (bSyz != dt->DtEnable[n]) {
				stchg++;
			}
			syz_chk++;
		}

		if( dt->DtID[n] == DATA_DENP ) {
			if( bDenp != dt->DtEnable[n] ) {
				stchg++;
			}
			denp_chk++;
		}

	}

	if( BMON_MST ) {
		if( ! bm_chk && bBmon )		stchg++;
	}
	if( M_KOJI ) {
		if( ! kj_chk && bKoji )		stchg++;
	}

	if( bHjiyu && ! hjiyu_chk )		stchg++;
	if( bZei && ! zei_chk )			stchg++;
	if (bSyz && ! syz_chk)			stchg++;

	if( bDenp && ! denp_chk )		stchg++;

	*set_pn = -1;

	if( stchg ) {
		SetListColumn( pAUTOSEL->DSPLINE_OPT );
		m_DtList.EnforceLayout();
		m_DtList.RedrawDataListCtrl();

		// フォーカスがセット可能なところにあるか？
		int pn = get_nowpn();

		if( pn == BMON_PN || pn == CBMN_PN ) {
			if( ! bBmon )	*set_pn = DATE_PN;
		}

		if( pn == KOJI_PN || pn == CKJI_PN ) {
			if( ! bBmon )	*set_pn = DATE_PN;
		}

		if( pn == HJIYU_PN )
			if( ! bHjiyu )	*set_pn = DATE_PN;

		if( pn == ZEI_PN )
			if( ! bZei )	*set_pn = DATE_PN;

		if (pn == SKBN_PN)
			if (!bSyz)		*set_pn = DATE_PN;

		if( pn == DENP_PN )
			if( ! bDenp )	*set_pn = DATE_PN;
	}
}

LRESULT CDBDinpView::OnCorrectMsg( WPARAM wParam, LPARAM lParam )
{

	PROGRAM_OK = FALSE;

	myICSMessageBox( (LPCTSTR)pDBzm->GetZmErrMsg() );
	/*--------------------
		仕訳再表示
	--------------------*/
	//仕訳行を再表示
	// ZREC を更新
	int i, recno;
	for( i = 0; i < RECMAX; i++ ) {
		if( ZREC[i].m_seq > 0 ) {
			if( m_DCrct.m_seq == ZREC[i].m_seq ) {
				ZREC[i] = m_DCrct;
			}
		}
	}

	// データラインを更新
	struct _DATA_LINE* ddata;
	ddata = &DDATA_LINE[0];
	for( i = 0; i < DDATA_LNMAX; i++ ) {
		recno = (ddata+i)->ZREC_NO;
		if( recno > 0 ) {
			if( ZREC[(recno-1)].m_seq != -1 )
				recdsp( ZREC, (recno-1), ddata, i );	// 表示
		}
		else {
			// データ初期化
			nline_del( ddata, i );
		}
	}
	//データスクロール行表示
	ddata_dsp(-1);

	PROGRAM_OK = TRUE;

	set_focus( DATE_PN );

	return 1;
}



// 工事番号チェックジョブ
BOOL CDBDinpView::isKojiChk()
{

	if( !FNC_BUTTON )
	{
		int st;

		if( !dinp_modefy_ok( getLINE_DATA() ) )
			return FALSE;

		if( IsSCAN_TKYONLY() ) return FALSE;

		BOOL both = FALSE;

		both = BmonKoji_DebCre_Inp( get_nowln() );

		PROGRAM_OK = FALSE;

		//
		// 借方 工事未入力チェック
		if( (m_BMKJICHK_CND & 0x11) != 0x11 )
		{
			st = koji_noinpcheck( CREC.m_dkno, CREC.m_dbt, 0 );
			m_BMKJICHK_CND |= 0x11;

			switch( st )
			{
			case 0:
				break;
			default:
				((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(0);
				PostMessage( WM_RETROFOCUS, KOJI_PN );

				set_nowpn( KOJI_PN );
				PROGRAM_OK = TRUE;

				return TRUE;
				break;
			}
		}
		//
		// 貸方  工事未入力チェック
		if( (m_BMKJICHK_CND & 0x12) != 0x12 )
		{
			st = koji_noinpcheck( CREC.m_ckno, CREC.m_cre, 1 );
			m_BMKJICHK_CND |= 0x12;

			switch( st )
			{
			case 0:
				break;
			default:
				((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(0);

				if( both ) {
					PostMessage( WM_RETROFOCUS, CKJI_PN );
					set_nowpn( CKJI_PN );
				}
				else {
					PostMessage( WM_RETROFOCUS, KOJI_PN );
					set_nowpn( KOJI_PN );

					m_BKINP_PN = CKJI_PN;
				}

				PROGRAM_OK = TRUE;

				return TRUE;
				break;
			}
		}

		// 完成工事チェック
		if( (m_BMKJICHK_CND & 0x1100) != 0x1100 ) {
			st = koji_kanseicheck( CREC.m_dkno, CREC.m_dbt, 0 );

			switch( st )
			{
			case 0:
				break;
			default:
				((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(0);
				PostMessage( WM_RETROFOCUS, KOJI_PN );

				set_nowpn( KOJI_PN );
				PROGRAM_OK = TRUE;

				return TRUE;
				break;
			}
		}

		if( (m_BMKJICHK_CND & 0x1200) != 0x1200 ) {
			st = koji_kanseicheck( CREC.m_ckno, CREC.m_cre, 1 );


			switch( st )
			{
			case 0:
				break;
			default:
				((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(0);

				if( both ) {
					PostMessage( WM_RETROFOCUS, CKJI_PN );
					set_nowpn( CKJI_PN );
				}
				else {
					PostMessage( WM_RETROFOCUS, KOJI_PN );
					set_nowpn( KOJI_PN );

					m_BKINP_PN = CKJI_PN;
				}

				PROGRAM_OK = TRUE;

				return TRUE;
				break;
			}
		}
	}
	
	PROGRAM_OK = TRUE;

	return FALSE;
}



BOOL CDBDinpView::GetKriMode()
{
	if( m_pSjb == NULL )
		return FALSE;

	return m_pSjb->IsKrmod();
}

void CDBDinpView::SetKriMode( BOOL bMode )
{
	if( m_pSjb == NULL )
		return;

	m_pSjb->IsKrmod() = bMode;
}

void CDBDinpView::GetKriSiwake( CDBINPDataRec* data )
{
	if( m_pSjb == NULL )
		return;

	m_pSjb->GetKrswk( data );
}

void CDBDinpView::KeepKriSiwake( CDBINPDataRec* data )
{
	if( m_pSjb == NULL )
		return;

	m_pSjb->KeepKrswk( data );
}


// 切返し対象仕訳か( 定型仕訳中はなし )
int CDBDinpView::KiriSwk( CDBINPDataRec* pKrec, CDBINPDataRec* crec )
{
	int ret = 0;

	if( m_pSjb == NULL )
		return 0;

	// 切返し仕訳あり
	if((ret = m_pSjb->AutoSwk( pKrec, crec )) != 0 ) {

		if (ret >= 2) {
			CNPautoDlg	dlg;

			ddata_dsp(-1);
			ddata_dspFlg = TRUE;
			inpline_inz(&INP_LINE);
			INP_LINE._XXDTA.dd_ddate = SREC.m_ddate;//初期日付
			inpline_dsp(&INP_LINE, APPEND_LINE);

			dlg.pNpsame = &m_pSjb->m_Npsame;
			dlg.pCrec = &CREC;

			if (get_nowpn() == HSEN_PN) {
				Destroy_HusenInp();
			}
			SelWndShow(FALSE);

			if (dlg.DoModal() == IDOK) {
				m_pSjb->GetAutoSwk(pKrec, crec, dlg.m_SelIndex);
			}
			else {
				ret = 0;
			}
		}
		// 対象元仕訳を保存
	//	m_pSjb->KeepCrec( crec );
	}

	return ret;
}


// 切返し元の仕訳のチェック
int CDBDinpView::ChkMotoSwk()
{
	CDBINPDataRec tmp, *pdata;
	int i, st;

	if( m_pSjb == NULL )
		return 0;

	m_pSjb->GetMotoRec(&tmp);
	
	st = -1;

	// 修正画面データのリセット
	for( i = 0 ; i < RECMAX ; ++i )
	{
		if( ZREC[i].m_seq <= 0 )
			continue;

		pdata = &ZREC[i];

		if( pdata->m_seq == tmp.m_seq ) {

			if( tmp.m_dbt.CompareNoCase( pdata->m_dbt ) ||
				tmp.m_cre.CompareNoCase( pdata->m_cre )) {
				st = 1;
			}
			else	st = 0;

			break;
		}
	}

	return st;
}


// 部門コード 範囲チェック用
BOOL CDBDinpView::bmncode_todata2( CDBINPDataRec* data, int number, int dc_sw, CString* bmnname )
{
	int hchk = 0;

	return bmncode_todata( data, number, dc_sw, hchk, bmnname );
}



// 部門コード 範囲チェック
BOOL CDBDinpView::isBmnHaniChk()
{

	BOOL bRet = FALSE;

	m_sdbtBmn = -1;
	m_screBmn = -1;

	if( !FNC_BUTTON )
	{
		int st;

		if( !dinp_modefy_ok( getLINE_DATA() ) )
			return FALSE;

		if( IsSCAN_TKYONLY() ) return FALSE;

		BOOL both = FALSE;

		both = BmonKoji_DebCre_Inp( get_nowln() );

		PROGRAM_OK = FALSE;
		CString msg, str;
		char buf[64];
		struct _DBKNREC* pKn;
		int	pn, dc_sw = 0;
		DWORD	dwCode;

		msg = "《部門範囲チェック》\r\n";
		int son_update = 0;

		//借方 部門範囲チェック
		if( CREC.m_dbmn >= 0 ) {

			if( ! BmChk.BmnHaniCheck( CREC.m_dbmn, CREC.m_dbt ) ) {
				dwCode = _atoh( (LPSTR)(LPCTSTR)CREC.m_dbt );
				dc_sw |= 0x01;

				if( (Voln1->bm_isw & 0x10)) {
					if( dwCode <= SONEKICODE ) {
						m_sdbtBmn = CREC.m_dbmn;
						CREC.m_dbmn = -1;
						dc_sw &= ~0x01;
						son_update++;
					}
					else {
						m_sdbtBmn = -1;
					}
				}
				else {
					m_sdbtBmn = -1;
				}

				if( dc_sw & 0x01 ) {
					if( ( pKn = pDBzm->DB_PjisToKnrec( CREC.m_dbt ) ) == NULL )
						return -1;

					pDBzm->BumonCodeToStr( buf, sizeof buf, CREC.m_dbmn );
					str.Format( "\r\n借方科目「%.14s」に、部門番号『%s』は設定できません。", pKn->knnam, buf );
					msg += str;
				}
			}
		}

		//貸方 部門範囲チェック
		if( CREC.m_cbmn >= 0 ) {

			if( ! BmChk.BmnHaniCheck( CREC.m_cbmn, CREC.m_cre ) ) {

				dwCode = _atoh( (LPSTR)(LPCTSTR)CREC.m_cre );
				dc_sw |= 0x02;

				if( (Voln1->bm_isw & 0x10)) {
					if( dwCode <= SONEKICODE ) {
						m_screBmn = CREC.m_cbmn;
						CREC.m_cbmn = -1;
						dc_sw &= ~0x02;
						son_update++;
					}
					else {
						m_screBmn = -1;
					}
				}
				else {
					m_screBmn = -1;
				}

				if( dc_sw & 0x02 ) {
					if( ( pKn = pDBzm->DB_PjisToKnrec( CREC.m_cre ) ) == NULL )
						return -1;
					pDBzm->BumonCodeToStr( buf, sizeof buf, CREC.m_cbmn );
					str.Format( "\r\n貸方科目「%.14s」に、部門番号『%s』は設定できません。", pKn->knnam, buf );
					msg += str;
				}
			}
		}

		if( dc_sw ) {
			msg += "\r\n\r\n修正しますか？\r\nいいえの場合、部門を取消して仕訳を書き込みます。";

			st = myICSMessageBox( (LPCSTR)msg, MB_YESNO,0,0,this );

			if( st == IDYES ) {

				if( both ) {
					if( dc_sw & 0x01 ) {
						pn = BMON_PN;
					}
					else {
						pn = CBMN_PN;
					}
				}
				else {
					pn = BMON_PN;

					if( dc_sw & 0x02 ) {
						m_BKINP_PN = CBMN_PN;
					}
				}

				PostMessage( WM_RETROFOCUS, pn );
				set_nowpn( pn );

				bRet = TRUE;
			}
			else {
				if( dc_sw & 0x01 ) {
					CREC.m_dbmn = -1;
				}
				if( dc_sw & 0x02 ) {
					CREC.m_cbmn = -1;
				}
				rewupdat_disp();
			}
		}
		else {
			if( son_update ) {
				rewupdat_disp();
			}
		}
	}

	
	PROGRAM_OK = TRUE;

	return bRet;
}


int CDBDinpView::CheckKamokuInput( int dc_sw, struct _KamokuData* data )
{
	// チェックしない
	return 0;
	
	if( INP_mode == _APPEND )
		return 0;

	int bmn;
	CString code;

	if( dc_sw == 0 ){
		bmn = CREC.m_dbmn;
	}
	else {
		bmn = CREC.m_cbmn;
	}

	if( bmn < 0 )
		return 0;

	code = data->kd_code;
	
	if( ! BmChk.BmnHaniCheck( bmn, code ) )
		return -1;

	return 0;
}


BOOL CDBDinpView::rewupdat_job( int ret ) 
{
	BOOL bRet = FALSE;

	if( ret == -2 )
		return FALSE;

	if( (ret & 0x1000) ) {
		BOOL both = FALSE;
		both = BmonKoji_DebCre_Inp( get_nowln() );

		if( ret & 0x01 ) {
			set_focus( BMON_PN );
		}
		else if( ret & 0x02 ) {
			if( both ) {
				set_focus( CBMN_PN );
			}
			else {
				set_focus( BMON_PN );
			}
		}
		bRet = TRUE;

	}

	if( ! bRet ) {
		if( (ret & 0x2000) ) {
			BOOL both = FALSE;
			both = BmonKoji_DebCre_Inp( get_nowln() );

			if( ret & 0x01 ) {
				set_focus( KOJI_PN );
			}
			else if( ret & 0x02 ) {
				if( both ) {
					set_focus( CKJI_PN );
				}
				else {
					set_focus( KOJI_PN );
				}
			}
			bRet = TRUE;

		}
	}

	//枝番チェック
	if( ! bRet ) {
		if( (ret & 0x4000) ) {
			if( ret & 0x01 ) {
				set_focus( DEBT_PN );
			}
			else if( ret & 0x02 ) {
				set_focus( CRED_PN );
			}
			bRet = TRUE;
		}
	}

	return bRet;
}


// 部門 再表示
void CDBDinpView::rewupdat_disp() 
{
	getLINE_DATA()->_XXDTA.dd_dbmn = CREC.m_dbmn;
	getLINE_DATA()->_XXDTA.dd_cbmn = CREC.m_cbmn;

	if( CREC.m_dbmn == -1 )
		getLINE_DATA()->_XXDTA.dd_dbmn_etc.Empty();

	if( CREC.m_cbmn == -1 )
		getLINE_DATA()->_XXDTA.dd_cbmn_etc.Empty();

	BOOL both = FALSE;
	both = BmonKoji_DebCre_Inp( get_nowln() );

	int pntbl[] = { BMON_PN, CBMN_PN };
	int pn = BMON_PN;

	TerminationDataSet( pntbl, 2 );
}



// 工事 再表示
void CDBDinpView::rewupdat_kojidisp() 
{
	getLINE_DATA()->_XXDTA.dd_dkno = CREC.m_dkno;
	getLINE_DATA()->_XXDTA.dd_ckno = CREC.m_ckno;

	if( CREC.m_dkno.IsEmpty() )
		getLINE_DATA()->_XXDTA.dd_dkno_etc.Empty();

	if( CREC.m_ckno.IsEmpty() )
		getLINE_DATA()->_XXDTA.dd_ckno_etc.Empty();

	BOOL both = FALSE;
	both = BmonKoji_DebCre_Inp( get_nowln() );

	int pntbl[] = { KOJI_PN, CKJI_PN };
	int pn = KOJI_PN;

	TerminationDataSet( pntbl, 2 );
}

//
// 消費税変更 背景色変更
//
void CDBDinpView::rewupdat_syzdisp() 
{
	int sg = SYZMDFY_BIT.bitncnd( CREC.m_seq - 1);

	if( sg > 0 ) {
		TerminationDataSet( SKBN_PN );
	}
}


//
//
//
void CDBDinpView::kmkbmn_change( int dc_sw, CDBINPDataRec* data, CString& chgcd ) 
{
	if( ! (Voln1->bm_isw & 0x10) )
		return;

	if( INP_mode == _APPEND )
		return;

	DWORD dwCd1, dwChgcd;
	dwChgcd = _atoh( (char*)(LPCTSTR)chgcd );

	// 部門が 一つのみ表示されている
	if( ! BmonKoji_DebCre_Inp( get_nowln() ) ) {
		if( dc_sw == 0 ) {
			dwCd1 = _atoh( (char*)(LPCTSTR)data->m_dbt );

			if( dwCd1 != dwChgcd ) {
				// 貸借から損益に変更の時
				if( dwChgcd >= SONEKICODE && dwCd1 < SONEKICODE ) {
					if( data->m_dbmn == -1 && data->m_cbmn != -1 ) {
						data->m_dbmn = data->m_cbmn;
					}
				}
			}
		}
		else {
			dwCd1 = _atoh( (char*)(LPCTSTR)data->m_cre );
			if( dwCd1 != dwChgcd ) {
				// 貸借から損益に変更の時
				if( dwChgcd >= SONEKICODE && dwCd1 < SONEKICODE ) {
					if( data->m_cbmn == -1 && data->m_dbmn != -1 ) {
						data->m_cbmn = data->m_dbmn;
					}
				}
			}
		}
	}
}

// 部門未入力 ジョブ
BOOL CDBDinpView::isNoBmnInpChk()
{

	if( !FNC_BUTTON )
	{
		int st;

		if( !dinp_modefy_ok( getLINE_DATA() ) )
			return FALSE;

		if( IsSCAN_TKYONLY() ) return FALSE;

		BOOL both = FALSE;

		both = BmonKoji_DebCre_Inp( get_nowln() );

		PROGRAM_OK = FALSE;

		//
		// 借方 部門未入力チェック
		if( (m_BMKJICHK_CND & 0x110000) != 0x110000 )
		{
			st = bmn_noinpcheck( CREC.m_dbmn, CREC.m_dbt, 0 );
			m_BMKJICHK_CND |= 0x110000;

			switch( st )
			{
			case 0:
				break;
			default:
				((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(0);

				PostMessage( WM_RETROFOCUS, BMON_PN );

				set_nowpn( BMON_PN );
				PROGRAM_OK = TRUE;

				return TRUE;
				break;
			}
		}
		//
		// 貸方  部門未入力チェック
		if( (m_BMKJICHK_CND & 0x120000) != 0x120000 )
		{
			st = bmn_noinpcheck( CREC.m_cbmn, CREC.m_cre, 1 );
			m_BMKJICHK_CND |= 0x120000;

			switch( st )
			{
			case 0:
				break;
			default:
				((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(0);

				if( both ) {
					PostMessage( WM_RETROFOCUS, CBMN_PN );
					set_nowpn( CBMN_PN );
				}
				else {
					PostMessage( WM_RETROFOCUS, BMON_PN );
					set_nowpn( BMON_PN );

					m_BKINP_PN = CBMN_PN;
				}

				PROGRAM_OK = TRUE;

				return TRUE;
				break;
			}
		}
	}
	
	PROGRAM_OK = TRUE;

	return FALSE;
}


int CDBDinpView::SetNprtParam()
{
	int st = 0;

	memset( &m_Pset, '\0', sizeof NPRT_PAR );		// 印刷毎に初期化 ('15.06.18)

	PCOM_GetString( "SelectPrinter", m_Pset.psPrinterName );	// プリンタ名のみこちらでのセットが必要

	if (m_ImgPrintFlg == PRINT_VERIFY) {		// 一括検証
		st = m_pImgmng->ImgPrintInitVerify(&m_Pset);	// NPRT_PARをセット	
	}
	else if (m_ImgPrintFlg == PRINT_DOCIMG ) {	// 原票イメージ
		st = m_pImgmng->ImgPrintInitData(&m_Pset);	// NPRT_PARをセット	
	}

	if (st < 0) {	
		if (st == -2)	// ドットプリンタは処理不可
			ICSMessageBox("当業務では､ドットプリンターによる出力はできません！", MB_ICONSTOP);
		else if (st == -1)
			ICSMessageBox(m_pImgmng->GetErrorMsg(), MB_ICONSTOP);
		if (m_ImgPrintFlg == PRINT_VERIFY) {
			m_pImgmng->ImgPrintResetDlg();
		}
		else {
			m_pImgmng->ImgEndPrintData();
		}
		m_ImgPrintFlg = PRINT_NONE;
		return st;
	}
	return 0;
}


//固定処理をキーで
void CDBDinpView::OnButtonF5()
{
	if( ! PROGRAM_OK ) 
	{
		return;
	}

	if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() == INP_VIEW )
	{
		//印刷へ
		if( m_ImgPrintFlg >= PRINT_VERIFY ) {
			HideImgDlg(1);
			if( CDBbaseView::SetNprtParam(m_pImgmng) < 0 )
				return;
			ICSFormView::OnFilePrint();
			return;
		}

		if( bCONFIRM_MASTER || pAUTOSEL == NULL )
			return;

		if( INP_mode == _APPEND ) {
			int notfix = 0;
			int subtool = 0;

			switch( get_nowpn() ) {
			case DATE_PN:
				if( pAUTOSEL->DATE_OPT == OPT_FIX )
					pAUTOSEL->DATE_OPT = OPT_NONOPT;
				else {
					pAUTOSEL->DATE_OPT = OPT_FIX;
					subtool = 1;
				}
				break;
			case BMON_PN:
				if( pAUTOSEL->BMON_OPT == OPT_FIX )
					pAUTOSEL->BMON_OPT = OPT_NONOPT;
				else {
					pAUTOSEL->BMON_OPT = OPT_FIX;
					subtool = 1;
				}
				break;
			case KOJI_PN:
				if( pAUTOSEL->KOJI_OPT == OPT_FIX )
					pAUTOSEL->KOJI_OPT = OPT_NONOPT;
				else {
					pAUTOSEL->KOJI_OPT = OPT_FIX;
					subtool = 1;
				}
				break;
			case DEBT_PN:
				if( pAUTOSEL->DEBT_OPT == OPT_FIX )
					pAUTOSEL->DEBT_OPT = OPT_NONOPT;
				else {
					pAUTOSEL->DEBT_OPT = OPT_FIX;
					subtool = 1;
				}
				break;
			case CBMN_PN:
				if( pAUTOSEL->BMON_OPT == OPT_FIX )
					pAUTOSEL->BMON_OPT = OPT_NONOPT;
				else {
					pAUTOSEL->BMON_OPT = OPT_FIX;
					subtool = 1;
				}
				break;
			case CKJI_PN:
				if( pAUTOSEL->KOJI_OPT == OPT_FIX )
					pAUTOSEL->KOJI_OPT = OPT_NONOPT;
				else {
					pAUTOSEL->KOJI_OPT = OPT_FIX;
					subtool = 1;
				}
				break;
			case CRED_PN:
				if( pAUTOSEL->CRED_OPT == OPT_FIX )
					pAUTOSEL->CRED_OPT = OPT_NONOPT;
				else {
					pAUTOSEL->CRED_OPT = OPT_FIX;
					subtool = 1;
				}
				break;

			case VAL_PN:	//全固定解除
				pAUTOSEL->DATE_OPT = OPT_NONOPT;
				pAUTOSEL->BMON_OPT = OPT_NONOPT;
				pAUTOSEL->KOJI_OPT = OPT_NONOPT;
				pAUTOSEL->DEBT_OPT = OPT_NONOPT;
				pAUTOSEL->CRED_OPT = OPT_NONOPT;
				subtool = -1;
				break;

			default:
				notfix = 1;
				break;
			}

			if( ! notfix ) {
				AutoSelIndicator( pAUTOSEL, _APPEND );

				if( subtool >= 0 ) {
					((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(0, subtool);
				}
			}
		}
	}
}

void CDBDinpView::OnUpdateButtonF5(CCmdUI *pCmdUI)
{
	if( PROGRAM_OK && bCONFIRM_MASTER ) {
		if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() == INP_VIEW ) {
			if( m_ImgPrintFlg >= PRINT_VERIFY ) {
				pCmdUI->Enable( TRUE );
				return;
			}
		}
	}

	if( ! PROGRAM_OK || bCONFIRM_MASTER )
	{
		pCmdUI->Enable( FALSE );
		return;
	}

	if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() == INP_VIEW )
	{
		if( INP_mode == _APPEND ) {
			switch( get_nowpn() ) {
			case DATE_PN:
			case BMON_PN:
			case KOJI_PN:
			case DEBT_PN:
			case CBMN_PN:
			case CKJI_PN:
			case CRED_PN:
				pCmdUI->Enable( TRUE );
				return;
			case VAL_PN:
				if( pAUTOSEL != NULL ) {
					if( pAUTOSEL->DATE_OPT == OPT_FIX || pAUTOSEL->DENP_OPT == OPT_FIX ||
						pAUTOSEL->BMON_OPT == OPT_FIX || pAUTOSEL->KOJI_OPT == OPT_FIX ||
						pAUTOSEL->DEBT_OPT == OPT_FIX || pAUTOSEL->CRED_OPT == OPT_FIX ) {
						pCmdUI->Enable( TRUE );
						return;
					}
				}
				break;
			}
		}
		else {
			if( m_ImgPrintFlg >= PRINT_VERIFY ) {
				pCmdUI->Enable( TRUE );
				return;
			}
		}
	}

	pCmdUI->Enable( FALSE );
	return;
}


//-----------------------------------------------------------------
//	返送値 FALSE: 個人業種の絞込みで、前行の科目が
//				 現在行の科目として不可のとき [02.08 /11]
//
//-----------------------------------------------------------------
BOOL CDBDinpView::KojinKamokuAutoCheck( int dc_sw )
{
BOOL bRet = TRUE;

	int type =  IsKojinGyosyuMaster( pDBzm );
	if( type ) {

		if( (INP_mode == _APPEND) && BOTOM_SEQ )
		{
			CString kmkcd_str;
			if( dc_sw )	kmkcd_str = SREC.m_cre;
			else		kmkcd_str = SREC.m_dbt;

			if( IsKojinKengyoMaster( type ) ) {
				struct _KamokuData  akd = {0}, ckd = {0};

				strcpy_s( ckd.kd_code, sizeof ckd.kd_code, kmkcd_str );
				ckd.kd_sgn = 2;	// 科目コードチェック用に 枝番のみのサインセット
				if( ! m_Seldata.InputItemCheck( akd, ckd, 0 ) ) {
				//	Buzzer();
					bRet = FALSE;
				}
			}
		}
	}

	return bRet;
}


// 仕訳コピー 選択処理(Shift)
void CDBDinpView::sel_shiftjob( int ln, long nChar, BOOL bPress )
{
	// SHIFT 押下し
	if( bPress ) {
		int zno = _getLINE_DATA( ln )->ZREC_NO;
		int seq, i;
		BOOL bAdd = FALSE;

		if( zno > 0 ) {
			seq = ZREC[ zno-1 ].m_seq;

			if( seq > 0 ) {

				gSelType = SEL_SHIFT;

				if( gSelSeq == 0 ) {
					gSelSeq = seq;

					for( i = 0; i < gSelArray.GetCount(); i++ ) {
						if( gSelArray[i] == seq ) {
							bAdd = TRUE;
							break;
						}
					}
					if( ! bAdd )	gSelArray.Add( seq );

					gSelDirection = (nChar==VK_UP) ? SELCPY_UP : SELCPY_DOWN;
				}
				else {
					if( gSelDirection == SELCPY_UP ) {
						if( (nChar == VK_UP) )  {
							for( i = 0; i < gSelArray.GetCount(); i++ ) {
								if( gSelArray[i] == seq ) {
									bAdd = TRUE;
									break;
								}
							}
							if( ! bAdd )	gSelArray.Add( seq );
						}
						else {
							// 選択方向が変わったので、該当の仕訳を削除する。
							if( gSelSeq == seq ) {
								gSelDirection = SELCPY_DOWN;
							}
							else {
								for( i = 0; i < gSelArray.GetCount(); i++ ) {
									if( gSelArray[i] == seq ) {
										gSelArray.RemoveAt( i );
										break;
									}
								}
							}
						}
					}
					else {
						if( (nChar == VK_DOWN) )  {
							for( i = 0; i < gSelArray.GetCount(); i++ ) {
								if( gSelArray[i] == seq ) {
									bAdd = TRUE;
									break;
								}
							}
							if( ! bAdd )	gSelArray.Add( seq );
						}
						else {
							// 選択方向が変わったので、該当の仕訳を削除する。
							if( gSelSeq == seq ) {
								gSelDirection = SELCPY_UP;
							}
							else {
								for( i = 0; i < gSelArray.GetCount(); i++ ) {
									if( gSelArray[i] == seq ) {
										gSelArray.RemoveAt( i );
										break;
									}
								}
							}
						}
					}
				}

				gSelUpdate = TRUE;
			}
		}
	}
	else {
		// SHIFT 離した。
		if( gSelDirection != SELCPY_NONE ) {
			gSelArray.RemoveAll();
			gSelDirection	= SELCPY_NONE;
			gSelType		= SEL_NONE;
			gSelSeq = 0;

			gSelUpdate = TRUE;
		}
	}
}


void CDBDinpView::sel_lineupdate( BOOL bDoUpdate/*=FALSE*/ )
{
	if( gSelUpdate || bDoUpdate ) {
		int i, n, zidx, sgn;
		DTLIST_DATA data;
		struct _DATA_LINE *dtline;
		int pntbl[2] = {SEQ_PN, -1};
		int curidx = get_nowln() - 1;

		//現在行も選択範囲に入れる。
		if( ! bDoUpdate ) {
			if( gSelDirection != SELCPY_NONE ) {
				if( curidx >= 0 ) {
					if( DDATA_LINE[curidx].ZREC_NO ) {
						zidx = DDATA_LINE[curidx].ZREC_NO -1;
						sgn = 0;

						for( n = 0; n < gSelArray.GetCount(); n++ ) {
							if( gSelArray[n] == ZREC[zidx].m_seq ) {
								sgn = 1;
								break;
							}
						}

						if( ! sgn ) {
							gSelArray.Add( ZREC[zidx].m_seq );
						}
					}
				}
			}
		}

		for( i = 0; i < DDATA_LNMAX; i++ ) {
			if( DDATA_LINE[i].ZREC_NO ) {
				zidx = DDATA_LINE[i].ZREC_NO -1;
				sgn = 0;

				for( n = 0; n < gSelArray.GetCount(); n++ ) {
					if( gSelArray[n] == ZREC[zidx].m_seq ) {
						DDATA_LINE[i]._XXDTA.dd_dsign |= 0x0400;
						sgn = 1;
						break;
					}
				}

				if( ! sgn ) {
					DDATA_LINE[i]._XXDTA.dd_dsign &= ~0x0400;
				}

				_GetDataList_Data( (i+1), &data );
				
				dtline = _getLINE_DATA( (i+1) );

				_TerminationDataSet( &data, dtline, pntbl, 1 );

				// 摘要以外の更新は、イメージを変更しない
				_UpdateDataList_Data( (i+1), &data, FALSE );
			}
		}

		m_DtList.RedrawDataListCtrl();
		gSelUpdate = FALSE;
	}
}

/***********************************************
	int line	クリックして変更したライン


 ***********************************************/
void CDBDinpView::click_seljob( int line, long nChar )
{
	int i, n, zidx, sgn, lnidx;
	DTLIST_DATA data;
	int pntbl[2] = {SEQ_PN, -1};
	int curidx = get_nowln() - 1;
	int curseq = 0;
	BOOL curAdd = FALSE;

	if( (nChar == VK_SHIFT || nChar == VK_CONTROL) && curidx >= 0 ) {

		//現在行も選択範囲に入れる。
		if( curidx >= 0 ) {
			if( DDATA_LINE[curidx].ZREC_NO ) {
				zidx = DDATA_LINE[curidx].ZREC_NO -1;
				sgn = 0;

				if( gSelSeq == 0 ) {
					gSelSeq = ZREC[zidx].m_seq;
				}

				curseq = ZREC[zidx].m_seq;

				for( n = 0; n < gSelArray.GetCount(); n++ ) {
					if( gSelArray[n] == curseq ) {
						sgn = 1;
						break;
					}
				}

				if( ! sgn ) {
					gSelArray.Add( curseq );
					curAdd = TRUE;
				}

				gSelUpdate = TRUE;
				gSelDirection = SELCPY_UP;
			}
		}

		// 基本となる仕訳SEQの位置が画面上にあるか？
		int baseln = -1;
		int seljob = 0;
		if( gSelSeq != 0 ) {
			for( i = 0; i <  DDATA_LNMAX; i++ ) {
				if( DDATA_LINE[i].ZREC_NO ) {
					zidx = DDATA_LINE[i].ZREC_NO -1;

					if( gSelSeq == ZREC[zidx].m_seq ) {
						baseln = i+1;
						break;
					}
				}
			}
		}

		lnidx = line - 1;
		if( lnidx >= 0 ) {

			if( baseln != -1 && nChar == VK_SHIFT ) {
				// 選択開始行 を またいで クリックされたとき( 下から上 )
				if( baseln < (curidx+1) && line <= baseln ) {
					for( i = baseln; i <= curidx; i++ ){
						sgn = -1;
						if( DDATA_LINE[i].ZREC_NO ) {
							zidx = DDATA_LINE[i].ZREC_NO -1;

							for( n = 0; n < gSelArray.GetCount(); n++ ) {
								if( gSelArray[n] == ZREC[zidx].m_seq ) {
									sgn = n;
									break;
								}
							}
							if( sgn != -1 ) {
								gSelArray.RemoveAt( n );
							}
						}
					}

					for( i = lnidx; i < baseln; i++ ){
						sgn = -1;
						if( DDATA_LINE[i].ZREC_NO ) {
							zidx = DDATA_LINE[i].ZREC_NO -1;

							for( n = 0; n < gSelArray.GetCount(); n++ ) {
								if( gSelArray[n] == ZREC[zidx].m_seq ) {
									sgn = n;
									break;
								}
							}
							if( sgn == -1 ) {
								gSelArray.Add( ZREC[zidx].m_seq );
							}
						}
					}

					gSelDirection = SELCPY_UP;
					seljob = 1;
				}
				else if( baseln > (curidx+1) && line >= baseln ) {
					// 選択開始行 を またいで クリックされたとき( 上から下 )
					for( i = curidx; i < baseln; i++ ){
						sgn = -1;
						if( DDATA_LINE[i].ZREC_NO ) {
							zidx = DDATA_LINE[i].ZREC_NO -1;

							for( n = 0; n < gSelArray.GetCount(); n++ ) {
								if( gSelArray[n] == ZREC[zidx].m_seq ) {
									sgn = n;
									break;
								}
							}
							if( sgn != -1 ) {
								gSelArray.RemoveAt( n );
							}
						}
					}

					for( i = (baseln-1); i < line; i++ ){
						sgn = -1;
						if( DDATA_LINE[i].ZREC_NO ) {
							zidx = DDATA_LINE[i].ZREC_NO -1;

							for( n = 0; n < gSelArray.GetCount(); n++ ) {
								if( gSelArray[n] == ZREC[zidx].m_seq ) {
									sgn = n;
									break;
								}
							}
							if( sgn == -1 ) {
								gSelArray.Add( ZREC[zidx].m_seq );
							}
						}
					}

					gSelDirection = SELCPY_DOWN;
					seljob = 1;
				}
			}

			if( ! seljob ) {
				if( DDATA_LINE[lnidx].ZREC_NO ) {
					int start, end, ln_sel = 0;
					if( nChar == VK_SHIFT ) {
						// SHIFT
						gSelType	= SEL_SHIFT;

						// 移動先の選択状態をチェック
						zidx = DDATA_LINE[lnidx].ZREC_NO - 1;
						for( n = 0; n < gSelArray.GetCount(); n++ ) {
							if( gSelArray[n] == ZREC[zidx].m_seq ) {
								ln_sel = 1;
								break;
							}
						}

						if( lnidx < curidx ) {
							start	= lnidx;
							end		= curidx;

							gSelDirection = SELCPY_UP;
						}
						else {
							start	= curidx;
							end		= lnidx;

							gSelDirection = SELCPY_DOWN;
						}

						for( i = start; i <= end; i++ ){
							sgn = -1;
							if( DDATA_LINE[i].ZREC_NO ) {
								zidx = DDATA_LINE[i].ZREC_NO -1;

								if( curAdd ) {
									if( curseq == ZREC[zidx].m_seq )
										continue;
								}
								else {
									// 移動先が未選択のときは、現行の選択をはずさない。
									if( ! ln_sel ) {
										if( curseq == ZREC[zidx].m_seq )
											continue;
									}
								}

								for( n = 0; n < gSelArray.GetCount(); n++ ) {
									if( gSelArray[n] == ZREC[zidx].m_seq ) {
										sgn = n;
										break;
									}
								}
								if( sgn == -1 ) {
									gSelArray.Add( ZREC[zidx].m_seq );
								}
								else {
									gSelArray.RemoveAt( n );
								}
							}
						}
					}
					else {
						zidx = DDATA_LINE[lnidx].ZREC_NO -1;
						// CTRL キー
						gSelType	= SEL_CTRL;
						sgn = -1;

						for( n = 0; n < gSelArray.GetCount(); n++ ) {
							if( gSelArray[n] == ZREC[zidx].m_seq ) {
								sgn = n;
								break;
							}
						}

						if( sgn == -1 ) {
							gSelArray.Add( ZREC[zidx].m_seq );
						}
					//	else {
					//		gSelArray.RemoveAt( n );
					//	}
					}
					if( lnidx < curidx ) {
						gSelDirection = SELCPY_UP;
					}
					else {
						gSelDirection = SELCPY_DOWN;
					}

					gSelUpdate = TRUE;
				}
			}
		}
	}
	else {
		// SHIFT, CTRL 離した。
		if( gSelDirection != SELCPY_NONE ) {
			gSelArray.RemoveAll();
			gSelDirection	= SELCPY_NONE;
			gSelType		= SEL_NONE;
			gSelSeq = 0;

			gSelUpdate = TRUE;
		}
	}

}

///////////////////////////////////////////////////////////////////
// 給与DB取り込み
//
#include <DBKyZmconnectMod8.h>

int CDBDinpView::GetWorkDirFunc(LPSTR wkPath, int szWkPath)
{
	CMainFrame* main = (CMainFrame*)GetDBDinpMainFrame();
	CString strErr;

	if( main->IsOnCloudEnv() ) {
		CString	strPath;

		if( GetSaveDefaultDirectory(strPath, strErr, _T("ICSTfm")) != 0 ) {
			AfxMessageBox(strErr);
			return -1;
		}
		strcpy_s(wkPath, szWkPath, strPath);
	}
	else {
		COM_GetWorkDirectory((LPVOID)wkPath);
	}

	return 0;
}


void CDBDinpView::OnMenuKyucsv()
{
	char	wkPath[256];

	GetWorkDirFunc(wkPath, sizeof wkPath);

	strcat_s( wkPath, sizeof wkPath, "\\kyuyodb_rendo.csv" );
	int st;

#ifdef _DEBUG
	strcpy_s( wkPath, sizeof wkPath, "E:\\Temp\\work\\csv_test.csv" );
#endif

	((CMainFrame*)GetDBDinpMainFrame())->CurrentDirCheck();

	CConnectMod*	ConnectM;

	ConnectM = NULL;
	CFileFind	fnd;
	BOOL bFind;
		
	if( (ConnectM = new CConnectMod()) == NULL ){ICSExit( 0, "[給与db]メモリ確保に失敗しました。" );}

	int sofs, eofs, sdate, edate, ym_st, ym_en;
	m_selDate.GetMonthOffset( sofs, eofs, sdate, edate );
	ym_st = sdate / 100;
	ym_en = edate / 100;
//FILETrace( "sdate = %d, edate = %d, ym_st = %d, ym_en = %d", sdate, edate, ym_st, ym_en );

	char pbuf[1024];
	if( PCOM_GetStringAll( "SELECTCODSN", pbuf, sizeof pbuf ) == -1 )
		goto ERR_END;

	char pApno[512];
	if( PCOM_GetStringAll( "AppsNumber", pApno, sizeof pApno ) == -1 )
		goto ERR_END;

	if( (st = ConnectM->Connect_Iniz( wkPath, 0, 0, ym_st, ym_en )) == -1 )
		goto ERR_END;

	if( (st = ConnectM->Connect_Dialog(this)) == -1 )
		goto ERR_END;

	bFind = fnd.FindFile( wkPath );

	if( bFind ) {
		fnd.Close();
		kyuyocsv_append( wkPath );
		//ファイル消去
		_unlink( wkPath );
	}

	PCOM_SetStringAll( "SELECTCODSN", pbuf );
	PCOM_SetStringAll( "AppsNumber", pApno );

	if( ConnectM ){delete ConnectM;}
	return;

ERR_END:

	if( ConnectM ){delete ConnectM;}

}


void CDBDinpView::OnUpdateMenuKyucsv(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if( bCONFIRM_MASTER ) {
		pCmdUI->Enable(FALSE);
		return;
	}
	BOOL bKyu = ((CMainFrame*)GetDBDinpMainFrame())->GetKyuyoDBsign();
	pCmdUI->Enable( bKyu && INP_mode == _APPEND );
}


void CDBDinpView::kyuyocsv_append( LPCTSTR fpath, BOOL bGenDB/*= FALSE*/ )
{
	CCsv	csv;
	CStringArray	sa;

	csv.CsvOpen( fpath );

	if( ! csv.CsvRead( &sa ) ) {
		return;
	}

	CString str;
	int cnt, i, set_ok, ipc, nxt;
	int len, byte, n;
	CDBINPDataRec	rec, tmprec;
	DWORD	dwCode;
	char work[256];
	BOOL bAdd = FALSE;

	if( csv.CsvRead( &sa ) ) {
		do {
			cnt = sa.GetCount();

			rec.Reset();
			rec.m_seq = REAL_SEQ_NO+1;
			set_ok = 0;

			for( i = 0; i < cnt; i++ ) {
				str = sa[i];
//MyTrace( "i = %d, str = %s\n", i, str );

				switch( i ) {
				case 0:	//日付
					if( csvstr_check( work, sizeof work, (LPCTSTR)str ) > 0 ) {

						if( strlen( work ) == 6 ) {
							// 元号 ＋ 月日
							int gen_ymd, ymd, gengo;
							gen_ymd = atoi( work );
							CVolDateDB vd;

							// 元号 -> 西暦
							vd.db_datecnvGen( 0, gen_ymd, &gengo, &ymd, 1, 0 );
							rec.m_ddate = ymd;
						}
						else {
							rec.m_ddate = atoi( work );
						}

						if( rec.m_ddate > 0 ) {
							pDBzm->minfo->MoveFirst();
							do {
								if( (pDBzm->minfo->insw & 0x01) ) {
									if( rec.m_ddate >= pDBzm->minfo->smd && 
										rec.m_ddate <= pDBzm->minfo->emd ) {
										rec.m_mofs = pDBzm->minfo->ofset;
										set_ok++;
										break;
									}
								}
							} while( pDBzm->minfo->MoveNext() == 0 );

							rec.m_ksign = 0;
						}
					}
					break;

				case 1:	//決修サイン
					if( set_ok == 1 ) {
						if( csvstr_check( work, sizeof work, (LPCTSTR)str ) > 0 ) {
							int ks_sgn = atoi( work );
							if( ks_sgn > 0 ) {
								pDBzm->minfo->MoveFirst();
								do {
									if( (pDBzm->minfo->insw & 0x01) ) {
										if( rec.m_ddate >= pDBzm->minfo->smd && 
											rec.m_ddate <= pDBzm->minfo->emd &&
											pDBzm->minfo->kes_sgn > 0 ) {
											rec.m_mofs	= pDBzm->minfo->ofset;
											rec.m_ksign = 1;
											break;
										}
									}
								} while( pDBzm->minfo->MoveNext() == 0 );
							}
						}
					}
					break;

				case 2:	//伝票番号	[06.07 /13]
					if( csvstr_check( work, sizeof work, (LPCTSTR)str ) > 0 ) {
						rec.m_cno =  atoi( work );
					}
					break;

				case 3:	//借方部門コード
					if( BMON_MST ) {
						if( csvstr_check( work, sizeof work, (LPCTSTR)str ) > 0 ) {
							rec.m_dbmn =  atoi( work );
						}
					}
					break;
				case 5:	//借方科目コード
					if( csvstr_check( work, sizeof work, (LPCTSTR)str ) > 0 ) {
						ipc = atoi( work );
						dwCode = pDBzm->InpcToPjis( ipc );
						if( dwCode != 0 ) {
							hl_rev( &dwCode, 4 );
							rec.m_dbt.Format( "%08x", dwCode );
							set_ok++;
						}
					}
					break;
				case 7:	//借方枝番
					if( BRmst || isSYOGT_CODE( rec.m_dbt ) )	{
						if( csvstr_check( work, sizeof work, (LPCTSTR)str ) > 0 ) {
							rec.m_dbr =  atoi( work );
						}
					}
					break;

				case 9:	//貸方部門コード
					if( BMON_MST ) {
						if( csvstr_check( work, sizeof work, (LPCTSTR)str ) > 0 ) {
							rec.m_cbmn =  atoi( work );
						}
					}
					break;
				case 11://貸方科目コード
					if( csvstr_check( work, sizeof work, (LPCTSTR)str ) > 0 ) {
						ipc = atoi( work );
						dwCode = pDBzm->InpcToPjis( ipc );
						if( dwCode != 0 ) {
							hl_rev( &dwCode, 4 );
							rec.m_cre.Format( "%08x", dwCode );
							set_ok++;
						}
					}
					break;

				case 13://貸方枝番
					if( BRmst || isSYOGT_CODE( rec.m_cre ) )	{
						if( csvstr_check( work, sizeof work, (LPCTSTR)str ) > 0 ) {
							rec.m_cbr =  atoi( work );
						}
					}
					break;
				case 15://金額
					if( csvstr_check( work, sizeof work, (LPCTSTR)str, 1 ) > 0 ) {
						l_input( rec.m_val, (void*)work );
					}
					break;
				case 16://摘要
					rec.m_tekiyo = str;
					len = rec.m_tekiyo.GetLength();
					byte = (Voln1->tk_ln*2);
					if( len > byte ) {
						n = get_strcnt( (char*)(LPCTSTR)rec.m_tekiyo, byte );
						rec.m_tekiyo.SetAt( n, '\0' );
					}
					break;

				case 18://対価
					if( csvstr_check( work, sizeof work, (LPCTSTR)str, 1 ) > 0 ) {
						l_input( rec.m_taika, (void*)work );
					}
					break;
				}
			}

			if( set_ok < 3 )	continue;

			// 一旦 科目なし で で消費税 イニシャライズ
			tmprec.Reset();
			tmprec.m_seq	=	rec.m_seq;
			tmprec.m_ddate	=	rec.m_ddate;
			tmprec.m_mofs	=	rec.m_mofs;
			tmprec.m_ksign	=	rec.m_ksign;

			DB_SyIniz( &tmprec );

			struct _SY_MSG_PACK2 smp;
			sy_chk_txt( &rec, &smp, INP_mode);

			int taika_sw = smp. SY_MSG_TAIKAKBN->sel_no;
			if( ! taika_sw ) ::ZeroMemory( rec.m_taika, sizeof rec.m_taika );

			//減価償却ｄｂ 経理方式に従う。
			if( bGenDB && smp.SY_MSG_ZEI->sel_no > 0 ) {
				DWORD dwFncCode = 0;
				if( ! (Voln1->s_sgn1 & 0x80) ) {	//税込み
					if( sotozei_data( &rec ) ) {
						dwFncCode = CD_UTIZ;
					}
				}
				else {
					if( ! (Voln1->s_sgn1 & 0x40) ) {	//税抜き
						if( uchizei_data( &rec ) ) {
							dwFncCode = CD_SOTOZ;
						}
					}
					else {	//一括税抜き
						if( uchizei_data( &rec ) ) {
							dwFncCode = CD_SOTOZ;
						}
					}
				}

				if( dwFncCode ) {
					if( pDBsy->SyFnc( dwFncCode ) == 0 ) {
						struct _SY_MSG_PACK2 smpk;
						pDBsy->SyMsg( &smp );
						pDBzm->GetCDBData( &rec );
					}
				}
			}

			CREC = rec;

			nxt = WandNextData();

			INP_LINE._XXDTA.dd_seq	= CREC.m_seq;

			bAdd = TRUE;

		} while( csv.CsvRead( &sa ) );
	}

	if( bAdd ) {
		ddata_dsp(-1);
		//データラインイニシャライズ
		inpline_inz( &INP_LINE );
		INP_LINE._XXDTA.dd_seq		= CREC.m_seq;
		INP_LINE._XXDTA.dd_ddate	= SREC.m_ddate;	//初期日付
		INP_LINE._XXDTA.dd_ksign	= SREC.m_ksign;
		INP_LINE._XXDTA.dd_mofs		= SREC.m_mofs;
		pDBzm->DateConv( INP_LINE.BcdDate, &SREC, CV2_1 );
		inpline_dsp( &INP_LINE, APPEND_LINE );

	//	SaveDinpDataSREC( _APPEND );
		SaveDinpParaSREC( _APPEND );

		// 枝番が自動で増えている可能性があるため
		ezquery_flag() = TRUE;
	}
}		

// 02.21 /11
LRESULT CDBDinpView::OnDBDinpMsg( WPARAM wParam, LPARAM lParam )
{
	if( wParam == DWP_LINECOUNT ) {
FILETrace( "@OnDBDinpMsg DWP_LINECOUNT TOP" );

		if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() != INP_VIEW )
			return 1;

		m_bDWP_LINECOUNT = TRUE;

		// 表示行数が変わった
		int old_lncnt = SCROLL_NL();
		m_DtList.GetDispDataCount();
		int now_lncnt = m_DtList.GetDispDataCount();
		if( now_lncnt > DDATA_LNMAX )	now_lncnt = DDATA_LNMAX;
//MyTrace( "@OnDBDinpMsg old_lncnt = %d, now_lncnt = %d", old_lncnt, now_lncnt );

		CDBINPDataRec	svCREC;
		svCREC = CREC;

		int focus_sgn = 0;
		if( old_lncnt != now_lncnt ) {
			if( INP_mode != _KEYSCAN ) {
				if( m_InsLine != -1 ) {
					if( m_InsMode == _MODEFY )	disp_insswklist( old_lncnt, now_lncnt );
					else						disp_keyscan_insswklist( old_lncnt, now_lncnt );
				}
				else {
					if( swkline_dispchg( old_lncnt, now_lncnt, &focus_sgn ) != 0 ) {
						ErrExit( 0, "仕訳表示行の変更ができませんでした。" );
					}
				}
			}
			else {
				if( swkline_keyscan_dispchg( old_lncnt, now_lncnt, &focus_sgn ) != 0 ) {
					ErrExit( 0, "仕訳表示行の変更ができませんでした。" );
				}
			}
		}
//MyTrace( "@OnDBDinpMsg focus_sgn = %d, m_bScanMode = %d", focus_sgn, m_bScanMode );

		m_bDWP_LINECOUNT = FALSE;
		//サイズ変更による描画で、情報が欠落する
		if( svCREC.m_seq == CREC.m_seq ) {
			memcpy(CREC.m_invno, svCREC.m_invno, sizeof svCREC.m_invno);
			if( svCREC.m_dsign[6] & 0x10 ) {
				CREC.m_dsign[6] |= 0x10;
			}
		}

		if( ! focus_sgn && ! m_bScanMode ) {
			_set_focus( get_nowln(), get_nowpn() );
		}
	}
	else if( wParam == DWP_COPYMENU ) {
		// いきなり修正行の番号欄を右クリック
		NM_DLCTMOUSE* nmh;
		m_bSwkCopy = TRUE;
		m_bSwkPaste = FALSE;
		nmh = (NM_DLCTMOUSE*)lParam;
		RButtonMenuJob(nmh->hdr.idFrom, nmh->point);
	}
	else if( wParam == DWP_DISPLINE ) {
		sel_lineupdate( TRUE );
	}
	else if( wParam == DWP_SELECTED1 ) {
		WORD pos, num;
		pos = LOWORD( lParam );
		num = HIWORD( lParam );
		SelectedIcsselctrl( pos, num, 0 );
	}
	else if( wParam == DWP_SELECTED2 ) {
		WORD pos, num;
		pos = LOWORD( lParam );
		num = HIWORD( lParam );
		SelectedIcsselctrl( pos, num, 1 );
	}
	else if( wParam == DWP_DENPDIALOG ) {
		DENP_INPUTPAR	dnppar;
		dnppar.pZmsub = pDBzm;
		dnppar.pSyohi = pDBsy;

		dnppar.parent = this;
		dnppar.winApp = &theApp;

		dnppar.seq = m_DenpSeq;
		dnppar.nosyzmdf = 0;
		DenpInputSyzMdfyVectorSet( &dnppar );
		int ln = get_nowln();

		PROGRAM_OK = FALSE;
		((CMainFrame*)GetDBDinpMainFrame())->ToolBarClear();

		int st = DenpInputDialog( &dnppar );
		if( st != IDCANCEL ) {

//AfxMessageBox( "DenpInputDialog end" );
			if( dnppar.addseq.size() ) {
				pDBzm->insseq_make() = FALSE;
			}
			// 修正・追加のサインをセット
			DenpEnd_addmdfcheck( &dnppar );
			pDBzm->data_Requery() = TRUE;

			//仕訳を再表示
			if( INP_mode == _KEYSCAN ) {
				query_denpend( m_DenpICNO, m_DenpSeq );
				denpend_keyscandisp( m_DenpSeq );
			}
			else {
				denpend_swkdisp( ln );
			}

			/* 諸口合計	*/
			pDBzm->kzrec_Requery() = TRUE;
			g_stotal( SYOGT, (char *)SYOG_d, (char *)SYOG_c, (char *)SG_def );
			syogdsp( (char *)SG_def );
			/* 現金合計	*/
			g_stotal( /*GNKIN*/pAUTOSEL->DCKM_CODE, (char *)GENK_d, (char *)GENK_c, (char *)GK_def );
			genkdsp( (char *)GK_def );
		}
		m_DenpSeq = m_DenpICNO = 0;
		PROGRAM_OK = TRUE;
		m_bActiveIgnore = TRUE;

		_set_focus( ln, SEQ_PN );
	}

	return 1;
}


// 工事番号コード チェック
BOOL CDBDinpView::isKojiDataChk()
{

	BOOL bRet = FALSE;

	if( !FNC_BUTTON )
	{
		int st;

		if( !dinp_modefy_ok( getLINE_DATA() ) )
			return FALSE;

		if( IsSCAN_TKYONLY() ) return FALSE;

		if( pAUTOSEL->KJIKMK_OPT != OPT_KJIKMK)
			return  FALSE;

		BOOL both = FALSE;

		both = BmonKoji_DebCre_Inp( get_nowln() );

		PROGRAM_OK = FALSE;
		CString msg, str;
		char buf[64];
		struct _DBKNREC* pKn;
		int	pn, dc_sw = 0;
		DWORD	dwCode;

		msg = "《工事番号 科目チェック》\r\n";

		//借方 工事番号 チェック
		if( ! CREC.m_dkno.IsEmpty() ) {

			if( ! KjChk.KojiKmkCheck( CREC.m_dbt ) ) {

				dwCode = _atoh( (LPSTR)(LPCTSTR)CREC.m_dbt );
				dc_sw |= 0x01;

				if( dc_sw & 0x01 ) {
					if( ( pKn = pDBzm->DB_PjisToKnrec( CREC.m_dbt ) ) == NULL )
						return -1;

					pDBzm->KojiCodeToStr( buf, sizeof buf, CREC.m_dkno );
					str.Format( "\r\n借方科目「%.14s」に、工事番号『%s』は設定できません。", pKn->knnam, buf );
					msg += str;
				}
			}
		}

		//貸方 工事番号 チェック
		if( ! CREC.m_ckno.IsEmpty() ) {

			if( ! KjChk.KojiKmkCheck( CREC.m_cre ) ) {

				dwCode = _atoh( (LPSTR)(LPCTSTR)CREC.m_cre );
				dc_sw |= 0x02;

				if( dc_sw & 0x02 ) {
					if( ( pKn = pDBzm->DB_PjisToKnrec( CREC.m_cre ) ) == NULL )
						return -1;
					pDBzm->KojiCodeToStr( buf, sizeof buf, CREC.m_ckno );
					str.Format( "\r\n貸方科目「%.14s」に、工事番号『%s』は設定できません。", pKn->knnam, buf );
					msg += str;
				}
			}
		}

		if( dc_sw ) {
			msg += "\r\n\r\n修正しますか？\r\nいいえの場合、工事番号を取消して仕訳を書き込みます。";

		//	st = myICSMessageBox( (LPCSTR)msg, MB_YESNO,0,0,this );
			st = IDNO;

			if( st == IDYES ) {

				if( both ) {
					if( dc_sw & 0x01 ) {
						pn = KOJI_PN;
					}
					else {
						pn = CKJI_PN;
					}
				}
				else {
					pn = KOJI_PN;

					if( dc_sw & 0x02 ) {
						m_BKINP_PN = CKJI_PN;
					}
				}

				PostMessage( WM_RETROFOCUS, pn );
				set_nowpn( pn );

				bRet = TRUE;
			}
			else {
				if( dc_sw & 0x01 ) {
					CREC.m_dkno.Empty();
				}
				if( dc_sw & 0x02 ) {
					CREC.m_ckno.Empty();
				}

				rewupdat_kojidisp();
			}
		}
	}

	
	PROGRAM_OK = TRUE;

	return bRet;
}



void CDBDinpView::RButtonMenuJob( UINT nID, CPoint point )
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_SWKMENU));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

//	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
//		 point.x, point.y, this);

//MyTrace( "@RButtonMenuJob m_bSwkCopy = %d, m_bSwkPaste = %d", m_bSwkCopy, m_bSwkPaste );

	UINT flag;

	if( m_bSwkCopy )	flag = MF_ENABLED;
	else				flag = MF_GRAYED;
	pPopup->EnableMenuItem( IDM_SWK_COPY, MF_BYCOMMAND| flag );

	if( m_bSwkPaste )	flag = MF_ENABLED;
	else				flag = MF_GRAYED;
	pPopup->EnableMenuItem( IDM_SWK_PASTE, MF_BYCOMMAND| flag );

	Rddata_Check();

	if( m_bRGDTA_BUTTON_DISP ) {

		if( m_bRegSwk )	flag = MF_ENABLED;
		else			flag = MF_GRAYED;
		pPopup->EnableMenuItem( IDM_REGDATA, MF_BYCOMMAND| flag );

		if( m_bRegDlg )	flag = MF_ENABLED;
		else			flag = MF_GRAYED;
		if( INP_mode != _APPEND )
			flag = MF_GRAYED;
		pPopup->EnableMenuItem( IDM_REGDLGUSE, MF_BYCOMMAND| flag );
	}
	else {
		pPopup->RemoveMenu( IDM_REGDATA,   MF_BYCOMMAND );
		pPopup->RemoveMenu( IDM_REGDLGUSE, MF_BYCOMMAND );
	}

	DWORD dwRetCode = pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
											  point.x, point.y, this);
	
	if( dwRetCode == IDM_SWK_COPY ) {
	//	PostMessage( WM_COMMAND, IDM_SWK_COPY );
		OnSwkCopy();
	}
	else if( dwRetCode == IDM_SWK_PASTE ) {
	//	PostMessage( WM_COMMAND, IDM_SWK_PASTE );
		OnSwkPaste();
	}
	else if( dwRetCode == IDM_REGDATA ) {
		Rdadd_job( get_nowln() );
	}
	else if( dwRetCode == IDM_REGDLGUSE ) {
		OnButtonInsert();
	}

//MyTrace( "@RButtonMenuJob dwRetCode = %d", dwRetCode );
//MyTrace( "@RButtonMenuJob x,y = (%d, %d)", point.x, point.y );
}


// 現在行がコピー用の選択仕訳の中に入っているか？
BOOL CDBDinpView::CheckCopySeq( int line )
{
	int curidx, zidx, curseq, n;
	BOOL bRet = FALSE;
	curidx = line - 1;

	if( curidx >= 0 ) {
		if( DDATA_LINE[curidx].ZREC_NO ) {
			zidx = DDATA_LINE[curidx].ZREC_NO -1;

			curseq = ZREC[zidx].m_seq;

			for( n = 0; n < gSelArray.GetCount(); n++ ) {
				if( gSelArray[n] == curseq ) {
					bRet = TRUE;
					break;
				}
			}
		}
	}

	return bRet;
}



BOOL CDBDinpView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	m_bSwkCopy = FALSE;
	m_bSwkPaste = FALSE;
	m_bRegSwk = FALSE;
	m_bRegDlg = FALSE;

	static NM_DLCTMOUSE nmhbf;

	if( wParam == IDC_DTLIST ) {
		NM_DLCTMOUSE* nmh;
		nmh = (NM_DLCTMOUSE*)lParam;

		if( !bCONFIRM_MASTER ) {
			m_bRegSwk = TRUE;
		}
		m_bRegDlg = FALSE;

		if( INP_mode != _APPEND ) {
			if( nmh->hdr.code == NM_RCLICK ) {
				m_bSwkCopy = TRUE;
				m_bSwkPaste = FALSE;
				int ln = get_nowln();

				if( nmh->iLine == ln ) {
					RButtonMenuJob( IDC_DTLIST, nmh->point );
				}
				else {
					// 選択仕訳行の範囲に入っているか？
					if( ! CheckCopySeq( nmh->iLine ) ) {
						click_seljob( nmh->iLine, 0 );
						sel_lineupdate();
					}

					_set_focus( nmh->iLine, SEQ_PN );
					nmhbf = *nmh;
					PostMessage( WM_DBDINPMSG, DWP_DISPLINE );
					PostMessage( WM_DBDINPMSG, DWP_COPYMENU, (LPARAM)&nmhbf );
				}
			}
		}
		else {
			if( nmh->hdr.code == NM_RCLICK && m_InsLine == -1 ) {
				m_bSwkCopy = TRUE;
				m_bSwkPaste = FALSE;

				// 選択クリア
				click_seljob( nmh->iLine, 0 );

				_set_focus( nmh->iLine, SEQ_PN );
				nmhbf = *nmh;

				PostMessage( WM_DBDINPMSG, DWP_COPYMENU, (LPARAM)&nmhbf );
			//	RButtonMenuJob( IDC_DTLIST, nmh->point );
			}
		}
 	}
	else if( wParam == IDC_DTLINE ) {
		NM_DLCTMOUSE* nmh;
		nmh = (NM_DLCTMOUSE*)lParam;

		if( ! bCONFIRM_MASTER ) {
			if( nmh->hdr.code == NM_RCLICK ) {
				m_pSCSrc->SrcDataRead();
				// FOR DB
				SWKCPY_SRC*	pScpy;
				pScpy = m_pSCSrc->m_pScpySrc;

				if( pScpy->sc_cocode[0]== '\0' || pScpy->sc_cnt <= 0 ) {
					m_bSwkPaste = FALSE;
				}
				else {
					m_bSwkPaste = TRUE;
				}

				if( m_bRGDTA_BUTTON_DISP ) {
					m_bRegSwk = FALSE;
					m_bRegDlg = TRUE;
				}

				RButtonMenuJob( IDC_DTLINE, nmh->point );
			}
		}
	}

	return CDBbaseView::OnNotify(wParam, lParam, pResult);
}


// 選択仕訳ソート
int __cdecl SelSeqCompare(void *context, const void *ele1, const void *ele2 )
{
	int ret;

	DWORD* dw1, *dw2;

	dw1 = (DWORD*)ele1;
	dw2 = (DWORD*)ele2;

/*
	if( *dw1 > *dw2 )		ret = 1;
	else if( *dw1 < *dw2 )	ret = -1;
	else					ret = 0;
*/
	CDipZmSub* pz;
	pz = (CDipZmSub*)context;

	ret = datarec_seqcmp( pz->m_sortMode, (int)*dw1, (int)*dw2 );

	return ret;
}


// 右クリックコピー処理
// 共有メモリに コピーマスター情報・仕訳番号を書き込む
//
void RClickSwkCopyJob( CWnd* pWnd, BOOL bSort )
{
	if( ! m_pSCSrc->SrcMappingCondition() ) {
		ICSMessageBox( "コピー機能が使用できません[Mapping]",0,0,0,pWnd);
	}

	if( gSelArray.GetCount() == 0 ) {
		return;
	}

	CByteArray ba;
	ba.SetSize( SIZE_SHAREMEM );
	::ZeroMemory( &ba[0], SIZE_SHAREMEM ); 

	SWKCPY_SRC*	pSrc;
	pSrc = (SWKCPY_SRC*)&ba[0];

	memcpy( pSrc->sc_sqlsvr,	(LPCTSTR)CBlkIns::pcom_sqlsvr,  CBlkIns::pcom_sqlsvr.GetLength() );
	memcpy( pSrc->sc_apno,		(LPCTSTR)CBlkIns::pcom_apno,	CBlkIns::pcom_apno.GetLength() );
	memcpy( pSrc->sc_cocode,	(LPCTSTR)CBlkIns::pcom_cocode,	CBlkIns::pcom_cocode.GetLength() );
	memcpy( pSrc->sc_kikan,		(LPCTSTR)CBlkIns::pcom_kikan,	CBlkIns::pcom_kikan.GetLength() );

	if( bSort ) {
		if( gSelArray.GetCount() > 1 ) {
			pDBzm->m_sortMode = 0;
			qsort_s( &gSelArray[0], gSelArray.GetCount(), sizeof DWORD, SelSeqCompare, pDBzm );
		}
	}

	pSrc->sc_cnt = gSelArray.GetCount();
	for( int n = 0; n < pSrc->sc_cnt; n++ ) {
		pSrc->sc_seq[n] = gSelArray[n];
	}
	/*
	pSrc->sc_cnt = 50;
	for( int n = 0; n < pSrc->sc_cnt; n++ ) {
		pSrc->sc_seq[n] = n+1;
	}
	*/

	m_pSCSrc->SrcDataWrite( pSrc );
}


void CDBDinpView::OnSwkCopy()
{
//MyTrace( "@OnSwkCopy TOP count = %d\n", gSelArray.GetCount() );

	if( gSelArray.GetCount() > 0 ) {
	//	qsort_s( &gSelArray[0], gSelArray.GetCount(), sizeof DWORD, SelSeqCompare, pDBzm );
	}
	else {
		if( INP_mode == _APPEND )
			return;
		else {
			int ln = get_nowln();
			gSelSeq = _getLINE_DATA(ln)->_XXDTA.dd_seq;
			gSelArray.Add( gSelSeq );
			// 行選択サインをセット[反転行制御のため]
			if( gSelDirection == SELCPY_NONE )
				gSelDirection	=	SELCPY_UP;
			if( gSelType == SEL_NONE )
				gSelType = SEL_CTRL;
		}
	}

	BOOL bSort = TRUE;
	if( INP_mode == _KEYSCAN ) {
		if( SC_Item.sc_type == SCT_ORDERDATE )
			bSort = FALSE;
	}
	RClickSwkCopyJob( this, bSort );
}

void CDBDinpView::OnSwkPaste()
{
	if( ! m_pSCSrc->SrcMappingCondition() ) {
		ICSMessageBox( "コピー機能が使用できません[Mapping]",0,0,0,this);
	}

	m_pSCSrc->SrcDataRead();

	// FOR DB
	SWKCPY_SRC*	pScpy;
	pScpy = m_pSCSrc->m_pScpySrc;

	if( pScpy->sc_cocode[0]== '\0' || pScpy->sc_cnt <= 0 ) {
		return;
	}

	if( pScpy->sc_cnt > COPY_MAXSWKCNT ) {
		CString msg;
		msg.Format( " 最大コピー可能数(=%d)を超えています。\r\n %d 仕訳をコピーします。", COPY_MAXSWKCNT, COPY_MAXSWKCNT );
		pScpy->sc_cnt = COPY_MAXSWKCNT;
		ICSMessageBox( msg, 0,0,0,this);
	}

	CDipZmSub* pSzm = NULL;

	if( (pSzm = new CDipZmSub) == NULL ) {
		ICSMessageBox( "財務クラスオブジェクト[コピー元]を作成できません！",0,0,0,this);
		return;
	}

	pSzm->ExitMode(1);


	try {
		if( pSzm->SetMasterDB( SMDB_VERS, pScpy->sc_sqlsvr,
										  atoi( pScpy->sc_apno ),
										  atoi( pScpy->sc_cocode ),
										  atoi( pScpy->sc_kikan ) ) == -1 ) {
			ermset( -1, (char*)(LPCTSTR)pSzm->GetZmErrMsg() );
		}

		/*--- ボリューム・ラベル Ｌｏａｄ ---*/
		if (pSzm->VolumeOpen() == ERR ) {
			ermset( -1, (char*)(LPCTSTR)pSzm->GetZmErrMsg() );
		}
		// --- 月別情報 ---
		if( pSzm->MinfoOpen() == ERR ) {
			ermset( -1, (char*)(LPCTSTR)pSzm->GetZmErrMsg() );
		}

		// ( 抽出元仕訳 )
		CString filter;
		filter.Format( "seq = %d", 1  );	//オープン高速化
		if( pSzm->DBDataOpenX( /*0*/-2, 0, filter ) != 0 ) {	
			ermset( -1, (char*)(LPCTSTR)pSzm->GetZmErrMsg() );
		}

		//摘要イメージ
		int src_imgmast = -1;

		if( (pSzm->zvol->ocr_idev&0x0f) == 3 )
		{
			// イメージ摘要ファイルのオープン 
			if( (src_imgmast = pSzm->ImageOpen()) == ERR )	// ERR = 無し OK = 有り
				ermset( -1, (char*)(LPCTSTR)pSzm->GetZmErrMsg() );
		}
	}
	catch ( CErrBlk E ) {
		delete pSzm;
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}

	m_pSCSrc->SetCpyZmsub( pDBzm, pSzm, this );

	if( m_pSCSrc->MastChk() == 0 ) {
		// コピー処理
		if( m_pSCSrc->PickData( get_insseq() ) > 0 ) {
			// 共有メモリに書き込み
#ifdef CLOSE
			CByteArray ba;
			ba.SetSize( SIZE_SHAREMEM );
			::ZeroMemory( &ba[0], SIZE_SHAREMEM ); 

			SWKCPY_SRC*	pSrc;
			pSrc = (SWKCPY_SRC*)&ba[0];
			m_pSCSrc->SrcDataWrite( pSrc );
#endif
			//入力行再表示
			INP_LINE._XXDTA.dd_seq = REAL_SEQ_NO + 1;	// 他で追加があるため SEQ更新
			inpline_dsp( &INP_LINE, APPEND_LINE);

			SREC = pDBzm->m_lastdata;
			SREC.m_seq--;	//次データ用にプラスされている分

			if( INP_mode == _APPEND ) {
				if( m_InsLine != -1 ) {
					if( m_InsMode == _MODEFY )	disp_insswklist( 0, 0 );
					else {
						disp_keyscan_insswkcopy();
					}

					CREC.m_seq = INP_LINE._XXDTA.dd_seq;
				}
				else {
					int i, n;
					//ZREC ReSet
					for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
						ZREC[i].Reset();
					}
					//仕訳表示用
					pDBzm->dbdata_Requery() = TRUE;
					CDBINPDataRec key;

					n = scaninsdat_getn( &ZREC[0], -1, SCROLL_NL() );

					scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );
					D_SEQN = SCROLL_NL()-1;

					ddata_dsp(-1);
					CREC.m_seq = INP_LINE._XXDTA.dd_seq;
				}
			}
			else {
				// 新規入力の seq更新
				SaveDinpData_SEQ( _APPEND, INP_LINE._XXDTA.dd_seq );
				SaveDinpParaSREC( _APPEND );
			//	SaveDinpDataSREC( _APPEND );
			}
			// 消費税サインをセット
			DB_SyIniz(&CREC);
			// 枝番が自動で増えている可能性があるため
			ezquery_flag() = TRUE;
		}
	}

	delete pSzm;
}

void CDBDinpView::OnUpdateSwkCopy(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable( m_bSwkCopy );
}

void CDBDinpView::OnUpdateSwkPaste(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable( m_bSwkPaste );
}

void CDBDinpView::RButtonUpIcsdbedtctrl( UINT nID, long nFlags, long pointx, long pointy)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
//MyTrace("RButtonUpIcsdbedt TOP nID(%d), nFlages = %08x, x, y = (%d, %d)", nID, nFlags, pointx, pointy );
	CPoint pt;
	pt.x = pointx;
	pt.y = pointy;

	GetDlgItem( nID )->ClientToScreen( &pt );

//MyTrace("RButtonUpIcsdbedt(2) x, y = (%d, %d)", pt.x, pt.y );

	m_bSwkCopy = FALSE;
	m_bSwkPaste = FALSE;

//MyTrace( "@OnNotify TOP" );
	static NM_DLCTMOUSE nmhbf;

	if( nID == IDC_ICSDBEDTCTRL2 ) { //既入力仕訳表示行
		if( INP_mode != _APPEND ) {
			m_bSwkCopy = TRUE;
			m_bSwkPaste = FALSE;
			RButtonMenuJob( IDC_DTLIST, pt );
		}
	}
}

// MainFrame Call 用 --- 05.13 /11
void CDBDinpView::SaveDinpParaFunc()
{
	if( INP_mode == _APPEND )	//to 修正ライン
	{
		SaveDinpPara( _APPEND );
	}
}


//#undef FILETrace
//#define FILETrace MyTrace


void CDBDinpView::OnSizeJob()
{

	CRect vwRect1, vwRect2;
	BOOL	bMove = FALSE;

	try
	{
#ifdef REGDATA	// 11.16 /00
		if( INP_mode == _APPEND || INP_mode == _MODEFY )
		{
			// 定型仕訳残り数表示
			RDRemainDispJOB( );
		}
#endif
		else 
		{
			RDRemainDisp( FALSE );
		}

		// view の 下端 へコントロールを移動する
		CRect vwRect, rc;
		GetClientRect( vwRect );

		int chk_wd = vwRect.Width() - m_ChkViewRect.Width();
		int chk_ht = vwRect.Height() - m_ChkViewRect.Height();

		//以前より、縦・横 3ピクセル以上動いたら、画面サイズ変更
		if( abs( chk_wd ) >= 3 || abs( chk_ht ) >= 3 ) {
			bMove = TRUE;
		}

		// 04.19 /12
		if( IsNotMultiWindow() ) {
			bMove = FALSE;
		}
		else {
			bMove = TRUE;
		}
ONSIZETrace( "@OnSizeJob vwRect = (%d,%d,%d,%d) m_ChkViewRect = (%d,%d,%d,%d),  bMove(%d)\n", 
		   vwRect.left, vwRect.top, vwRect.right, vwRect.bottom, 
		   m_ChkViewRect.left, m_ChkViewRect.top, m_ChkViewRect.right, m_ChkViewRect.bottom, bMove );

		m_ChkViewRect = vwRect;

ONSIZETrace( "@OnSizeJob Org  => [%d,%d,%d,%d]\n", m_ChkViewRect.left, m_ChkViewRect.top, m_ChkViewRect.right, m_ChkViewRect.bottom);
ONSIZETrace( "@OnSizeJob view => [%d,%d,%d,%d]\n", vwRect.left, vwRect.top, vwRect.right, vwRect.bottom);

		if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() == INP_VIEW && bMove ) {

			// 追加入力用に画面変更
			BOOL bSgn = FALSE;
			if( IsMAsterKAKUTEI() )	bSgn = TRUE;
			else {
				if( INP_mode == _KEYSCAN )	bSgn = TRUE;
				else if( INP_mode == _APPEND ) {
					if( m_InsLine != -1 && m_InsMode == _KEYSCAN )	bSgn = TRUE;
				}
			}

			GetDlgItem(IDC_ICSDISPGENKIN)->GetWindowRect( rc );
			ScreenToClient( &rc );

			int ht, basebottom, posTop[2], top1, top2;

			basebottom = vwRect.bottom;

			int org_vht = m_original_rect.bottom - m_original_rect.top;
			if( basebottom < org_vht )
				basebottom = org_vht;

			if( basebottom > rc.bottom || basebottom < rc.bottom ) {
				// オフセット分移動
				for( int i = 0; i < (sizeof gCtrlID / sizeof gCtrlID[0]); i++ ) {

					if( gCtrlID[i] == IDC_STATIC_POS || gCtrlID[i] == IDC_STATIC_POS2 )
						continue;

					if( gCtrlID[i] == IDC_DTLINE ) { 
						basebottom = top1;
					}
					else if( gCtrlID[i] == IDC_ICSDISPCTRL10 ) {
						basebottom = top2;
					}

					GetDlgItem( gCtrlID[i] )->GetWindowRect( rc );
					ScreenToClient( rc );
					ht = rc.Height();

//ONSIZETrace( "OnSize i = %d, rc.top = %d, ht = %d", i, rc.top, ht );

					rc.top		= basebottom - ht;
					rc.bottom	= basebottom;
					GetDlgItem( gCtrlID[i] )->MoveWindow( rc.left, rc.top, rc.Width(), ht/*rc.Height()*/ );

					//入力区分の位置
					if( gCtrlID[i] == IDC_MNTHSEL )			posTop[0] = rc.top;
					//現金貸借の位置
					if( gCtrlID[i] == IDC_ICSDISPCTRL12 )	posTop[1] = rc.top;

					// 入力カスタム用 開始底位置
					if( gCtrlID[i] == IDC_ICSDISPGENKIN )	top1 = rc.top;
					// 入力区分の 開始底位置
					if( gCtrlID[i] == IDC_DTLINE )			top2 = rc.top;
				}

				GetDlgItem( IDC_STATIC_POS )->GetWindowRect( rc );
				ScreenToClient( rc );
				ht = rc.Height();
				rc.top		= posTop[0];
				rc.bottom	= posTop[0] + ht;
		
				GetDlgItem( IDC_STATIC_POS )->MoveWindow( rc.left, rc.top, rc.Width(), rc.Height() );

				GetDlgItem( IDC_STATIC_POS2 )->GetWindowRect( rc );
				ScreenToClient( rc );
				ht = rc.Height();
				rc.top		= posTop[1];
				rc.bottom	= posTop[1] + ht;
				GetDlgItem( IDC_STATIC_POS2 )->MoveWindow( rc.left, rc.top, rc.Width(), rc.Height() );

			}

			ScanMode_WndMove( bSgn, FALSE );

		}
		// 欄外大きなイメージ
		big_img( getLINE_DATA(), getINP_CTL( TKY_PN ));
		_disp_change();

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

#ifdef _SLIP_ACCOUNTS_
void CDBDinpView::OnBnClickedDispslipBtn()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	// 表示切替
	if( IsDispSlip() ){
		HideImgDlg();
//--> '12.10.02 INS START
		int pn = get_nowpn();
		if( INP_mode !=_KEYSCAN ){
			set_focus( pn );
		}
//<-- '12.10.02 INS END
	}
	else if( m_bSLACC_BUTTON_DISP ){
		DispImgDlg( FALSE );
	}
}

void CDBDinpView::OnBnClickedLinkslipBtn()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	// リンク処理
	if( IsDispSlip() ){

//--> '12.10.04 INS START
		if( ((CREC.m_dsign[7]&0x30)&&!(pDBzm->zvol->tl_cor&0x01)) || !(pDBzm->zvol->tl_cor&0x10) || IsJzSansyo()){
			return;
		}
//<-- '12.10.04 INS END

		if( m_SlipBtnState == ID_LINKON_BSTATE ){
			int st = 0;
			if( INP_mode == _APPEND ){
				// この時点では直リンクしない！
				int docseq;
				if( (docseq=m_pImgmng->GetDocseqOnImgDlg()) != -1 ){
					m_ContDocseqAry.Add( docseq );
					CREC.m_dsign[6] |= 0x01;
#ifndef _SCANNER_SV_
					ChgLinkBtnState( ID_LINKOFF_BSTATE );
#else
					ChgLink();
#endif

//--> '12.10.29 INS START
					// フォーカス再セット (フォーカスが痙攣しないか要注意)
					set_focus( get_nowpn() );
//<-- '12.10.29 INS END
				}
				CArray<int> docseqAry;
				m_pImgmng->GetLinkDocseqOnDlg(docseqAry);
				if( docseqAry.GetCount() == 0 )	CREC.m_dsign[6] &= ~0x01;
			}
			else{
				if( (m_DImgdlgType==ID_DBDINP_INPUTDLG_TYPE) || (m_DImgdlgType==ID_DBDINP_ADDLINKDLG_TYPE) ){
					int docseq;
					if( (docseq=m_pImgmng->GetDocseqOnImgDlg()) != -1 ){
#ifndef _SCANNER_SV_
						ChgLinkBtnState( ID_LINKOFF_BSTATE );
#else
						ChgLink();
#endif

//--> '16.03.25 INS START
						CREC.m_dsign[6] &= ~0x01;

						if( m_pImgmng->GetLinkDocseqOnDlg(m_ContDocseqAry) == 0 ){
							if( m_ContDocseqAry.GetCount() ){
								CREC.m_dsign[6] |= 0x01;
							}
						}
//<-- '16.03.25 INS END

//--> '12.10.29 INS START
						// フォーカス再セット (フォーカスが痙攣しないか要注意)
						set_focus( get_nowpn() );
//<-- '12.10.29 INS END
					}
				}
			}
		}
		else if( m_SlipBtnState == ID_LINKOFF_BSTATE ){
			// リンク解除
			if( INP_mode == _APPEND ){
/*- '12.07.02 -*/
//				m_pImgmng->DisConnectDataToImgFile( pDBzm, INP_LINE._XXDTA.dd_seq );
//				CREC.m_dsign[6] &= ~0x01;
//				ChgLinkBtnState( ID_LINKON_BSTATE );
/*-------------*/
				DelDocseq();
				int cnt = (int)m_ContDocseqAry.GetCount();
				if( cnt == 0 ){
					CREC.m_dsign[6] &= ~0x01;
				}
#ifndef _SCANNER_SV_
				ChgLinkBtnState( ID_LINKON_BSTATE );
#else
				ChgLink();
#endif
/*-------------*/

//--> '12.10.29 INS START
				// フォーカス再セット (フォーカスが痙攣しないか要注意)
				set_focus( get_nowpn() );
//<-- '12.10.29 INS END
			}
			else{
#ifndef _SCANNER_SV_
/*- '12.07.02 -*/
//				m_pImgmng->DisConnectDataToImgFile( pDBzm, getLINE_DATA()->_XXDTA.dd_seq );
//				CREC.m_dsign[6] &= ~0x01;
//				ChgLinkBtnState( ID_LINKON_BSTATE );
/*-------------*/
				int nowCnt = m_pImgmng->GetCountRelImg( pDBzm, getLINE_DATA()->_XXDTA.dd_seq );
				if( nowCnt > 0 ){
					BOOL	nowAddflg = FALSE;
					int		nowDocseq = m_pImgmng->GetDocseqOnImgDlg();
					int		max = (int)m_ContDocseqAry.GetCount();
					for( int i=0; i<max; i++ ){
						if( m_ContDocseqAry[i] == nowDocseq ){
							nowAddflg = TRUE;
							break;
						}
					}

					if( nowAddflg == FALSE ){
						if( ICSMessageBox(_T("表示中の原票と仕訳の関連を解除してよろしいですか？"), (MB_YESNO|MB_DEFBUTTON2), 0, 0, this) == IDYES ){
							m_pImgmng->DisConnectDataToImgFile( pDBzm, getLINE_DATA()->_XXDTA.dd_seq );
							int newCnt = m_pImgmng->GetCountRelImg( pDBzm, getLINE_DATA()->_XXDTA.dd_seq );
							if( (nowCnt==-1) || (newCnt==-1) ){
								// エラー
								;
							}
							else if( newCnt == 0 ){
								CREC.m_dsign[6] &= ~0x01;
								getLINE_DATA()->_XXDTA.dd_dsign &= ~0x0800;

/*- '12.10.29 -*/
								ChgLinkBtnState( ID_DISPSLIP_BSTATE );
								MoveSubWindow( 0 );
/*-------------*/
//								ChgLinkBtnState( ID_LINKON_BSTATE, FALSE );
//								MoveSubWindow( 0 );
/*-------------*/
							}
							else if( newCnt && (nowCnt>newCnt) ){
								MoveSubWindow( 0 );
								DispImgDlg( FALSE );
							}
						}
						else{
							m_pImgmng->ChgLinkStateInImgDlg( 1 );
						}
					}
					else{
						DelDocseq();
						ChgLinkBtnState( ID_LINKON_BSTATE );
					}

					// フォーカス再セット
					set_focus( get_nowpn() );
				}
				else{
					DelDocseq();
					int cnt = (int)m_ContDocseqAry.GetCount();
					if( cnt == 0 ){
						CREC.m_dsign[6] &= ~0x01;
					}
					ChgLinkBtnState( ID_LINKON_BSTATE );

//--> '12.10.29 INS START
					// フォーカス再セット (フォーカスが痙攣しないか要注意)
					set_focus( get_nowpn() );
//<-- '12.10.29 INS END
				}
/*-------------*/
#else
				ChgLink();
#endif

			}
		}
	}
	else if( m_bSLACC_BUTTON_DISP ){
		DispImgDlg( FALSE );
	}
}
#endif

#ifdef _SLIP_ACCOUNTS_
//-----------------------------------------------------------------------------
// サブウィンドウ移動
//-----------------------------------------------------------------------------
// 引数	sw	：	表示切替スイッチ
//				 0：非表示にする, 1：移動する, 2:移動する(multi 左寄せ時)
//-----------------------------------------------------------------------------
int CDBDinpView::MoveSubWindow( int sw )
{
	if( m_pImgmng ){

		if( sw == 0 ){
			if( m_pImgmng->IsDlgVisible() ){
//--> '12.08.10 INS START
				if( m_pImgmng->GetImgDlgType() == 0 ){
					m_NowImgSeq = m_pImgmng->GetDocseqOnImgDlg();
				}
//<-- '12.08.10 INS END
/*- '12.10.02 -*/
//				m_pImgmng->DispImgDlgOff();
/*-------------*/
				TurnOffDispImgDlg();;
/*-------------*/

#ifdef _SCANNER_SV_
				// 痙攣注意！
				set_focus( get_nowpn() );
#endif
			}
		}
		else if( sw == 1 || sw == 2 ){
			int wType = GetMultiWindowType();
			if( (wType==MULTIWINDOW_FULL) || (wType==MULTIWINDOW_SIMPLE) ){
				RECT	clRect;
/*- '12.03.02 -*/
//				GetClientRect( &clRect );
//				this->ClientToScreen( &clRect );
/*-------------*/
//				GetWindowRect( &clRect );
				((CMainFrame*)GetDBDinpMainFrame())->GetWindowRect( &clRect );
/*-------------*/
				CPoint	pt(clRect.right, clRect.top);
				if( m_pImgmng->IsDlgVisible() == FALSE && sw == 1 ){
					IMG_DLGOPTION	dlgOption;
					memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
/*- '12.09.22 -*/
//					dlgOption.btnDispSw |= (0x01|0x02|0x04|0x08|0x10|0x20);
/*-------------*/
					dlgOption.btnDispSw |= (0x01|0x02|0x04|0x08);
/*-- '12.10.03 --*/
//					if( (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_DO_LINKIMG) ){
/*---------------*/
					if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
/*---------------*/
						dlgOption.btnDispSw |= (0x10|0x20);
					}
/*-------------*/
//--> '12.08.13 INS START
					if( m_NowImgSeq != -1 ){
						dlgOption.imgDispSw |= 0x01;
						dlgOption.DispImgSeq = m_NowImgSeq;
					}
//<-- '12.08.13 INS END
			//		dlgOption.btnHideSw |= 0x01;
					m_pImgmng->DispImgDlgOn( this, dlgOption );
					m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
					m_DImgdlgType = ID_DBDINP_INPUTDLG_TYPE;

				}
				m_pImgmng->MoveImgDlg( &pt );
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 原票会計 初期化 ('12.04.16)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBDinpView::InitSldata()
{
	ASSERT( pDBzm );

	m_bSLIP_MASTER = FALSE;
	_IS_SLIP_MASTER = 0;
	m_bSLACC_BUTTON_DISP = FALSE;
	BOOL bSlipCheck = FALSE;

	CMainFrame* main = (CMainFrame*)GetDBDinpMainFrame();

FILETrace("InitSldata bSCANSAVE_Master = %d, pDBzm->zvol->sub_sw = %04x\n", bSCANSAVE_Master, pDBzm->zvol->sub_sw);
FILETrace("InitSldata IsBuyScannerSave(%d), IsEnkakuUser = %d\n", IsBuyScannerSave(), IsEnkakuUser());

	if( main->IsKeiriJzEBEnv() ) {
//FILETrace("InitSldata 経理上手くん（電子帳簿)\n" );

		if( bSCANSAVE_Master && (pDBzm->zvol->sub_sw & 0x40) ) {
			bSlipCheck = TRUE;
		}
	}
	else {

		if( IsBuyScannerSave() && bSCANSAVE_Master && (pDBzm->zvol->sub_sw & 0x40) && !IsEnkakuUser() ) {
			bSlipCheck = TRUE;
		}

		if( !bSlipCheck ) {
			// OCR-Sのみ購入している
			if( GET_USERTYPE() ){
				// 顧問先ユーザーは除外
				;
			}
			else if( IsBuyOCRScan() && bSCANSAVE_Master ) {
				bSlipCheck = TRUE;
			}
		}
	}
//FILETrace("InitSldata bSlipCheck(%d), \n", bSlipCheck);

//	if( (IsPakageCd(ICS_IMGMNG_SLIP_PACKAGE_CODE)==0) && (pDBzm->zvol->sub_sw&0x40) && ! IsEnkakuUser() ){
//	if( IsBuyScannerSave() && bSCANSAVE_Master && (pDBzm->zvol->sub_sw&0x40) && ! IsEnkakuUser() ){
	if( bSlipCheck ) {
		if( m_pImgmng == NULL ){
			m_pImgmng = new CImgMng;
			IMG_INITREC	initrec;
			memset( &initrec, '\0', sizeof(IMG_INITREC) );
			initrec.pDBZmSub = pDBzm;
			int st = m_pImgmng->Init( initrec );

FILETrace("m_pImgmng st(%d), \n", st );
			if( st == 0 ){
				m_bSLIP_MASTER = TRUE;
				_IS_SLIP_MASTER = 1;
			}
			else if( st == 1 ){
				delete m_pImgmng;
				m_pImgmng = NULL;
				return 0;
			}
			// 経理上手くんの環境で、アンテナハウスのPDFをインストールするための呼び出し
			if( main->IsKeiriJzEBEnv() ) {
				if( !m_pImgmng->IsPdfViewerInstall() ) {
					delete m_pImgmng;
					m_pImgmng = NULL;
					ermset(ERROR_ENV, "PDF描画用のモジュールがインストールされていません！");
					return	-1;
				}
			}
			m_pImgmng->GetImgDispOption( m_ImgDispOption );
			m_Seldata.SetImgMng( m_pImgmng );
		}

		if( m_ImgDispOption.dtypeInInput == ID_HIDE_IN_INPUT ){
			m_bSLACC_BUTTON_DISP = FALSE;
/*- '12.10.01_CUT -*/
//			m_DispSlipBtn.EnableWindow( FALSE );
//			m_DispSlipBtn.ShowWindow( SW_SHOW );
/*-----------------*/
			m_LinkSlipBtn.EnableWindow( FALSE );
/*- '12.09.21 -*/
//			m_LinkSlipBtn.ShowWindow( SW_SHOW );
/*-------------*/
			m_LinkSlipBtn.ShowWindow( SW_HIDE );
/*-------------*/
//--> '12.10.02 INS START
			m_DispSlipChk.EnableWindow( FALSE );
			m_DispSlipChk.ShowWindow( SW_SHOW );
//<-- '12.10.02 INS END
			if( main->IsKeiriJzEBEnv() || IsBuyScannerSave() ) {
				m_bSLACC_BUTTON_DISP = TRUE;
				m_DispSlipChk.EnableWindow(TRUE);
				m_DispSlipChk.ShowWindow(SW_SHOW);

				if( !IsJzSansyo() && !FromReadOCR() ) {
					m_GnpScanBtn.EnableWindow(TRUE);
					m_GnpScanBtn.ShowWindow(SW_SHOW);
				}
				else {
					m_GnpScanBtn.EnableWindow(FALSE);
					m_GnpScanBtn.ShowWindow(SW_HIDE);
				}
			}
			else {
				if( IsBuyOCRScan() ) {
					m_bSLACC_BUTTON_DISP = TRUE;
					m_DispSlipChk.EnableWindow(TRUE);
					m_DispSlipChk.ShowWindow(SW_SHOW);
				}
				else {
					m_DispSlipChk.EnableWindow(FALSE);
					m_DispSlipChk.ShowWindow(SW_SHOW);
				}
				m_GnpScanBtn.EnableWindow(FALSE);
				m_GnpScanBtn.ShowWindow(SW_HIDE);
			}
		}
		else{
			m_bSLACC_BUTTON_DISP = TRUE;
			m_LinkSlipBtn.EnableWindow( FALSE );
			m_LinkSlipBtn.ShowWindow( SW_HIDE );

			m_DispSlipChk.EnableWindow(TRUE);
			m_DispSlipChk.ShowWindow(SW_SHOW);

			if( (main->IsKeiriJzEBEnv() || IsBuyScannerSave()) && ! IsJzSansyo() && !FromReadOCR() ) {
				m_GnpScanBtn.EnableWindow(TRUE);
				m_GnpScanBtn.ShowWindow(SW_SHOW);
			}
			else {
				m_GnpScanBtn.EnableWindow(FALSE);
				m_GnpScanBtn.ShowWindow(SW_HIDE);
			}
		}

//--> '12.09.28 INS START
/*- '12.10.03 -*/
//		if( (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_NOT_LINKIMG) ){
/*-------------*/
		if( m_ImgDispOption.autoLkType == ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
/*-------------*/
			m_NowImgSeq = pAUTOSEL->SLIP_SEQ;
		}
//<-- '12.09.28 INS END

//--> '12.10.01 INS START
		// 原票表示ボタン初期化
		CString	tipText;
		HICON	hDispIcon = NULL;
		hDispIcon = ::LoadIcon( (HINSTANCE)GetWindowLong(this->m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON_DISPSLIP) );
		m_DispSlipBtn.SetXButtonStyle( ICS_BS_WINXP_COMPAT|ICS_BS_TWOROWS );
		if( hDispIcon ){
			m_DispSlipChk.SetIcon( hDispIcon );
		}

		tipText = _T("原票を表示します〔Ctrl+Home〕");
		m_tooltip.AddTool( GetDlgItem(IDC_DISPSLIP_CHK), tipText );
		if( main->IsKeiriJzEBEnv() ) {
			hDispIcon = ::LoadIcon((HINSTANCE)GetWindowLong(this->m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON_DOCREAD));
		}
		else {
			hDispIcon = ::LoadIcon((HINSTANCE)GetWindowLong(this->m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON_SCAN));
		}
		m_GnpScanBtn.SetWindowText("");
		if( hDispIcon ) {
			m_GnpScanBtn.SetIcon(hDispIcon);
		}

		if( main->IsKeiriJzEBEnv() ) {
			tipText = _T("原票を読取ります〔Shift+Home〕");
		}
		else {
			tipText = _T("原票をスキャンします〔Shift+Home〕");
		}
		m_tooltip.AddTool(GetDlgItem(IDC_GNPSCAN_BTN), tipText);

//<-- '12.10.01 INS END
	}
	else{
		m_bSLACC_BUTTON_DISP = FALSE;
/*- '12.10.01_CUT -*/
//		m_DispSlipBtn.EnableWindow( FALSE );
//		m_DispSlipBtn.ShowWindow( SW_HIDE );
/*-----------------*/
		m_LinkSlipBtn.EnableWindow( FALSE );
		m_LinkSlipBtn.ShowWindow( SW_HIDE );
//--> '12.10.02 INS START
		m_DispSlipChk.EnableWindow( FALSE );
		m_DispSlipChk.ShowWindow( SW_HIDE );
//<-- '12.10.02 INS END
		m_GnpScanBtn.EnableWindow(FALSE);
		m_GnpScanBtn.ShowWindow(SW_HIDE);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 原票：表示中？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	表示中
//			FALSE	：	表示中でない
//-----------------------------------------------------------------------------
BOOL CDBDinpView::IsDispSlip()
{
	BOOL	rt = FALSE;

	if( m_bSLACC_BUTTON_DISP ){
		if( m_pImgmng && m_pImgmng->IsDlgVisible() ){
			rt = TRUE;
		}
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 表示中のイメージが関連付けられたもの？
//-----------------------------------------------------------------------------
// 返送値	1	：	関連付けられている
//			0	：	関連付けられていない
//-----------------------------------------------------------------------------
int CDBDinpView::IsConnectedDoc()
{
	int st = 0;

	if( INP_mode == _APPEND ){
		int cnt = (int)m_ContDocseqAry.GetCount();
		if( cnt && (IsDispSlip()==TRUE) ){
			int docseq;
			if( (docseq=m_pImgmng->GetDocseqOnImgDlg()) != -1 ){
				for( int i=0; i<cnt; i++ ){
					if( m_ContDocseqAry[i] == docseq ){
						st = 1;
						break;
					}
				}
			}
		}
	}
	else{
		int cnt = m_pImgmng->GetCountRelImg( pDBzm, getLINE_DATA()->_XXDTA.dd_seq );
		if( cnt > 0 ){
			st = 1;
		}
	}

	return st;
}

//-----------------------------------------------------------------------------
// リンクボタン状態変更
//-----------------------------------------------------------------------------
// 引数	btnSt	：	変更状態
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBDinpView::ChgOnlyLinkBtnState( EnumIdSlipdispBtnstate btnSt )
{
	int		cnt = 0;
	CString	btnText;

	EnumIdColorBtnStyle	toColorBtnStyle = ID_NORMAL_COLORBTN_STYLE;

	if( btnSt == ID_DISPSLIP_BSTATE ){
	}
	else if( btnSt == ID_LINKON_BSTATE ){
#ifndef _SCANNER_SV_
		if( m_pImgmng ){
			m_pImgmng->ChgLinkStateInImgDlg( 0 );
		}
#endif
		int cnt = (int)m_ContDocseqAry.GetCount();
		if( (INP_mode!=_APPEND) && m_pImgmng ){
			int tmpCnt = 0;
			tmpCnt = m_pImgmng->GetCountRelImg( pDBzm, getLINE_DATA()->_XXDTA.dd_seq );
			cnt += tmpCnt;
//--> '12.10.22 INS START
			if( tmpCnt ){
				toColorBtnStyle = ID_LINKDISP_COLORBTN_STYLE;
			}
//<-- '12.10.22 INS END
		}
		// 表示テキスト
		if( cnt ){
			btnText.Format( _T("%d"), cnt );
		}
		else{
			btnText.Empty();
		}

//--> '12.10.22 INS START
		m_LinkSlipBtn.SetBtnDispStyle( toColorBtnStyle );
//<-- '12.10.22 INS END
		m_LinkSlipBtn.SetWindowText( btnText );
	}
	else if( btnSt == ID_LINKOFF_BSTATE ){
#ifndef _SCANNER_SV_
		if( m_pImgmng ){
			m_pImgmng->ChgLinkStateInImgDlg( 1 );
		}
#endif
		if( (INP_mode!=_APPEND) && m_pImgmng ){
			int tmpCnt = 0;
			tmpCnt = m_pImgmng->GetCountRelImg( pDBzm, getLINE_DATA()->_XXDTA.dd_seq );
			cnt += tmpCnt;
//--> '12.10.22 INS START
			if( tmpCnt ){
				toColorBtnStyle = ID_LINKDISP_COLORBTN_STYLE;
			}
//<-- '12.10.22 INS END
		}
		if( cnt ){
			btnText.Format( _T("%d"), cnt );
		}
		else{
			btnText.Empty();
		}

//--> '12.10.22 INS START
		m_LinkSlipBtn.SetBtnDispStyle( toColorBtnStyle );
//<-- '12.10.22 INS END
		m_LinkSlipBtn.SetWindowText( btnText );
	}

	return 0;
}

#ifdef _20121003_EXTEND_
//-----------------------------------------------------------------------------
// リンクボタン状態変更
//-----------------------------------------------------------------------------
// 引数		btnSt	：	変更状態
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CDBDinpView::ChgLinkBtnState( EnumIdSlipdispBtnstate btnSt )
{
//--> '12.10.01 INS START
	if( m_SlipBtnState == btnSt ){
		ChgOnlyLinkBtnState( btnSt );
		return 0;
	}
//<-- '12.10.01 INS END

//--> '12.10.01 INS START
	EnumIdColorBtnStyle	toColorBtnStyle = ID_NORMAL_COLORBTN_STYLE;
//<-- '12.10.01 INS END

	// 表示開始ボタン
	CString	btnText, tipText;
	HICON	hDispIcon = NULL;
/*- '12.10.01_CUT -*/
//	hDispIcon = ::LoadIcon( (HINSTANCE)GetWindowLong(this->m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON_DISPSLIP) );
//	m_DispSlipBtn.SetXButtonStyle( ICS_BS_WINXP_COMPAT|ICS_BS_TWOROWS );
//	if( hDispIcon ){
//		m_DispSlipBtn.SetIcon( ICSBUTTON_SIZEICON32, hDispIcon );
////--> '12.10.01 INS START
//		m_DispSlipChk.SetIcon( hDispIcon );
////<-- '12.10.01 INS END
//	}
//	tipText = _T("原票を表示します〔Ctrl+Home〕");
//	m_tooltip.AddTool( GetDlgItem(IDC_DISPSLIP_BTN), tipText );
/*-----------------*/

	int nowSeq = -1;
	if( m_pImgmng ){
		nowSeq = m_pImgmng->GetDocseqOnImgDlg();
	}

	// 関連付けボタン
	BOOL	enableFlg = FALSE;
//--> '12.09.21 INS START
	int		nSlipCmdshow = SW_HIDE;
//<-- '12.09.21 INS END
	HICON	hIcon = NULL;
	if( btnSt == ID_DISPSLIP_BSTATE ){
//		hIcon = ::LoadIcon( (HINSTANCE)GetWindowLong(this->m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON_DISPSLIP) );
		hIcon = ::LoadIcon( (HINSTANCE)GetWindowLong(this->m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON_ATTACH) );
		m_SlipBtnState = ID_DISPSLIP_BSTATE;
		tipText = _T("原票を表示します〔Ctrl+Home〕");
		btnText.Empty();

//--> '12.09.29 INS START
		if( m_DispSlipChk.GetCheck() ){
			m_DispSlipChk.SetCheck( 0 );
		}
//<-- '12.09.29 INS END
	}
	else if( btnSt == ID_LINKON_BSTATE ){
		// ダイアログ側もイメージ無しに
		if( m_pImgmng ){
			m_pImgmng->ChgLinkStateInImgDlg( 0 );
		}

		hIcon = ::LoadIcon( (HINSTANCE)GetWindowLong(this->m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON_ATTACH) );
		m_SlipBtnState = ID_LINKON_BSTATE;
		tipText = _T("原票と仕訳を関連付けます〔F9〕");
		int cnt = (int)m_ContDocseqAry.GetCount();
//--> '12.09.21 INS START
		if( (INP_mode!=_APPEND) && m_pImgmng ){
			int tmpCnt = 0;
			tmpCnt = m_pImgmng->GetCountRelImg( pDBzm, getLINE_DATA()->_XXDTA.dd_seq );
			cnt += tmpCnt;
//--> '12.10.01 INS START
			if( tmpCnt ){
				toColorBtnStyle = ID_LINKDISP_COLORBTN_STYLE;
			}
//<-- '12.10.01 INS END
		}
//<-- '12.09.21 INS END

//--> '12.09.26 INS START
/*- '12.09.28 -*/
//		if( cnt > 0 ){
//			hIcon = ::LoadIcon( (HINSTANCE)GetWindowLong(this->m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON_PLURAL_ATTACH) );
//		}
/*-------------*/
		// '12.09.28: 現段階は、一つのクリップを表示するように
		hIcon = ::LoadIcon( (HINSTANCE)GetWindowLong(this->m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON_PLURAL_ATTACH) );
/*-------------*/
//<-- '12.09.26 INS END

		if( cnt ){
//		if( cnt > 1 ){
/*- '12.09.06 -*/
			btnText.Format( _T("%d"), cnt );
/*-------------*/
//			int nowIdx = -1;
//			for( int i=0; i<cnt; i++ ){
//				if( m_ContDocseqAry[i] == nowSeq ){
//					nowIdx = i+1;
//					break;
//				}
//			}
//			if( nowIdx != -1 ){
//				btnText.Format( _T("%2d/%2d"), nowIdx, cnt );
//			}
//			else{
//				btnText.Format( _T("--/%2d"), cnt );
//			}
/*-------------*/
		}
		else{
			btnText.Empty();
		}
		enableFlg = TRUE;
//--> '12.09.21 INS START
		nSlipCmdshow = SW_SHOW;
//<-- '12.09.21 INS END

//--> '12.09.29 INS START
		if( m_DispSlipChk.GetCheck() == 0 ){
			m_DispSlipChk.SetCheck( 1 );
		}
//<-- '12.09.29 INS END
	}
	else if( btnSt == ID_LINKOFF_BSTATE ){
		// ダイアログ側もイメージ有りに
		if( m_pImgmng ){
			m_pImgmng->ChgLinkStateInImgDlg( 1 );
		}
		hIcon = ::LoadIcon( (HINSTANCE)GetWindowLong(this->m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON_ATTACHED) );
		m_SlipBtnState = ID_LINKOFF_BSTATE;
		tipText = _T("原票と仕訳の関連を解除します〔F9〕");
		int cnt = (int)m_ContDocseqAry.GetCount();
//--> '12.09.21 INS START
		if( (INP_mode!=_APPEND) && m_pImgmng ){
			int tmpCnt = 0;
			tmpCnt = m_pImgmng->GetCountRelImg( pDBzm, getLINE_DATA()->_XXDTA.dd_seq );
			cnt += tmpCnt;
//--> '12.10.01 INS START
			if( tmpCnt ){
				toColorBtnStyle = ID_LINKDISP_COLORBTN_STYLE;
			}
//<-- '12.10.01 INS END
		}
//<-- '12.09.21 INS END
//		if( cnt > 1 ){
		if( cnt ){
/*- '12.09.06 -*/
			btnText.Format( _T("%d"), cnt );
/*-------------*/
//			int nowIdx = -1;
//			for( int i=0; i<cnt; i++ ){
//				if( m_ContDocseqAry[i] == nowSeq ){
//					nowIdx = i+1;
//					break;
//				}
//			}
//			if( nowIdx != -1 ){
//				btnText.Format( _T("%2d/%2d"), nowIdx, cnt );
//			}
//			else{
//				btnText.Format( _T("--/%2d"), cnt );
//			}
/*-------------*/
		}
		else{
			btnText.Empty();
		}
		enableFlg = TRUE;
//--> '12.09.21 INS START
		nSlipCmdshow = SW_SHOW;
//<-- '12.09.21 INS END

//--> '12.09.29 INS START
		if( m_DispSlipChk.GetCheck() == 0 ){
			m_DispSlipChk.SetCheck( 1 );
		}
//<-- '12.09.29 INS END
	}

	if( btnText.GetLength() == 0 ){
//		m_LinkSlipBtn.SetXButtonStyle( ICS_BS_WINXP_COMPAT|ICS_BS_TWOROWS );
		m_LinkSlipBtn.SetWindowText( btnText );
	}
	else{
//		m_LinkSlipBtn.SetXButtonStyle( ICS_BS_WINXP_COMPAT );
		m_LinkSlipBtn.SetWindowText( btnText );
	}
	if( hIcon ){
//		m_LinkSlipBtn.SetIcon( ICSBUTTON_SIZEICON32, hIcon );
		m_LinkSlipBtn.SetIcon( hIcon );
	}
//--> '12.09.22 INS START
/*- '12.10.03 -*/
//	if( (m_ImgDispOption.dtypeInInput!=ID_DISP_IN_INPUT) || (m_ImgDispOption.linkWorkType==ID_NOT_LINKIMG) ){
/*-------------*/
	if( m_ImgDispOption.autoLkType == ID_AUTOLINK_NOLINK ){
/*-------------*/
		enableFlg = FALSE;
		nSlipCmdshow = SW_HIDE;
	}
//<-- '12.09.22 INS END

//--> '12.10.01 INS START
	m_LinkSlipBtn.SetBtnDispStyle( toColorBtnStyle );
//<-- '12.10.01 INS END

	m_LinkSlipBtn.EnableWindow( enableFlg );
//--> '12.09.21 INS START
	m_LinkSlipBtn.ShowWindow( nSlipCmdshow );
//<-- '12.09.21 INS END

//	m_tooltip.AddTool( GetDlgItem(IDC_LINKSLIP_BTN), tipText );
	m_tooltip.AddTool( GetDlgItem(IDC_DISPSLIP_CHK), tipText );

	return 0;
}
#else
//-----------------------------------------------------------------------------
// リンクボタン状態変更
//-----------------------------------------------------------------------------
// 引数	btnSt				：	変更状態
//		isChgDepressState	：	原票表示ボタンの押下状態を変更するかどうか
//-----------------------------------------------------------------------------
// 返送値	0				：	正常終了
//			-1				：	エラー
//-----------------------------------------------------------------------------
int CDBDinpView::ChgLinkBtnState( EnumIdSlipdispBtnstate btnSt, BOOL isChgDepressState/*=TRUE*/ )
{

	if( m_SlipBtnState == btnSt ){
		ChgOnlyLinkBtnState( btnSt );
		return 0;
	}

	EnumIdColorBtnStyle	toColorBtnStyle = ID_NORMAL_COLORBTN_STYLE;

	// 表示開始ボタン
	CString	btnText, tipText;
	HICON	hDispIcon = NULL;

	int nowSeq = -1;
	if( m_pImgmng ){
		nowSeq = m_pImgmng->GetDocseqOnImgDlg();
	}

	// 関連付けボタン
	BOOL	enableFlg = FALSE;
	int		nSlipCmdshow = SW_HIDE;
	HICON	hIcon = NULL;
	if( btnSt == ID_DISPSLIP_BSTATE ){
		hIcon = ::LoadIcon( (HINSTANCE)GetWindowLong(this->m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON_DISPSLIP) );
		m_SlipBtnState = ID_DISPSLIP_BSTATE;
/*- '12.10.17 -*/
//		tipText = _T("原票を表示します〔Ctrl+Home〕");
/*-------------*/
		tipText.Empty();
/*-------------*/
		btnText.Empty();

		if( isChgDepressState ){
			if( m_DispSlipChk.GetCheck() ){
				m_DispSlipChk.SetCheck( 0 );
			}
		}
	}
	else if( btnSt == ID_LINKON_BSTATE ){
		// ダイアログ側もイメージ無しに
#ifndef _SCANNER_SV_
		if( m_pImgmng ){
			m_pImgmng->ChgLinkStateInImgDlg( 0 );
		}
#endif

		hIcon = ::LoadIcon( (HINSTANCE)GetWindowLong(this->m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON_ATTACH) );
		m_SlipBtnState = ID_LINKON_BSTATE;
		tipText = _T("原票と仕訳を関連付けます〔F9〕");
		int cnt = (int)m_ContDocseqAry.GetCount();
		if( (INP_mode!=_APPEND) && m_pImgmng ){
			int tmpCnt = 0;
			tmpCnt = m_pImgmng->GetCountRelImg( pDBzm, getLINE_DATA()->_XXDTA.dd_seq );
			cnt += tmpCnt;
			if( tmpCnt ){
				toColorBtnStyle = ID_LINKDISP_COLORBTN_STYLE;
			}
		}

		// '12.09.28: 現段階は、一つのクリップを表示するように
		hIcon = ::LoadIcon( (HINSTANCE)GetWindowLong(this->m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON_PLURAL_ATTACH) );

		if( cnt ){
			btnText.Format( _T("%d"), cnt );
		}
		else{
			btnText.Empty();
		}
		enableFlg = TRUE;
		nSlipCmdshow = SW_SHOW;

		if( isChgDepressState ){
			if( m_DispSlipChk.GetCheck() == 0 ){
				m_DispSlipChk.SetCheck( 1 );
			}
		}
	}
	else if( btnSt == ID_LINKOFF_BSTATE ){
		// ダイアログ側もイメージ有りに
#ifndef _SCANNER_SV_
		if( m_pImgmng ){
			m_pImgmng->ChgLinkStateInImgDlg( 1 );
		}
#endif
		hIcon = ::LoadIcon( (HINSTANCE)GetWindowLong(this->m_hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON_ATTACHED) );
		m_SlipBtnState = ID_LINKOFF_BSTATE;
		tipText = _T("原票と仕訳の関連を解除します〔F9〕");
		int cnt = (int)m_ContDocseqAry.GetCount();
		if( (INP_mode!=_APPEND) && m_pImgmng ){
			int tmpCnt = 0;
			tmpCnt = m_pImgmng->GetCountRelImg( pDBzm, getLINE_DATA()->_XXDTA.dd_seq );
			cnt += tmpCnt;
			if( tmpCnt ){
				toColorBtnStyle = ID_LINKDISP_COLORBTN_STYLE;
			}
		}
		if( cnt ){
			btnText.Format( _T("%d"), cnt );
		}
		else{
			btnText.Empty();
		}
		enableFlg = TRUE;
		nSlipCmdshow = SW_SHOW;

		if( isChgDepressState ){
			if( m_DispSlipChk.GetCheck() == 0 ){
				m_DispSlipChk.SetCheck( 1 );
			}
		}
	}

	if( btnText.GetLength() == 0 ){
		m_LinkSlipBtn.SetWindowText( btnText );
	}
	else{
		m_LinkSlipBtn.SetWindowText( btnText );
	}
	if( hIcon ){
		m_LinkSlipBtn.SetIcon( hIcon );
	}

#ifndef _SCANNER_SV_
	if( m_ImgDispOption.autoLkType == ID_AUTOLINK_NOLINK ){
		enableFlg = FALSE;
		nSlipCmdshow = SW_HIDE;
	}
#else
	enableFlg = FALSE;
	nSlipCmdshow = SW_HIDE;
#endif
	m_LinkSlipBtn.SetBtnDispStyle( toColorBtnStyle );
/*- '12.10.29 -*/
//	m_LinkSlipBtn.EnableWindow( enableFlg );
//	m_LinkSlipBtn.ShowWindow( nSlipCmdshow );
#ifdef CLOSE	//原票読取対応
/*-------------*/
	if( enableFlg && (m_LinkSlipBtn.IsWindowEnabled()==FALSE) ){
		m_LinkSlipBtn.EnableWindow( enableFlg );
	}
	else if( !enableFlg && (m_LinkSlipBtn.IsWindowEnabled()==TRUE) ){
		m_LinkSlipBtn.EnableWindow( enableFlg );
	}
	if( (nSlipCmdshow==SW_SHOW) && (m_LinkSlipBtn.IsWindowVisible()==FALSE) ){
		m_LinkSlipBtn.ShowWindow( nSlipCmdshow );
	}
	else if( (nSlipCmdshow==SW_HIDE) && (m_LinkSlipBtn.IsWindowVisible()==TRUE) ){
		m_LinkSlipBtn.ShowWindow( nSlipCmdshow );
	}
/*-------------*/
#endif

/*- '12.10.17 -*/
//	m_tooltip.AddTool( GetDlgItem(IDC_LINKSLIP_BTN), tipText );
/*-------------*/
	if( tipText.GetLength() ){
		m_tooltip.AddTool( GetDlgItem(IDC_LINKSLIP_BTN), tipText );
	}
/*-------------*/

	return 0;
}
#endif

//-----------------------------------------------------------------------------
// イメージダイアログ表示
//-----------------------------------------------------------------------------
// 引数	bKeyDown	：	キーダウンによるものかどうか
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CDBDinpView::DispImgDlg( BOOL bKeyDown )
{
	if( m_bSLACC_BUTTON_DISP == FALSE ){
//--> '12.10.01 INS START
		if( m_SlipBtnState == ID_DISPSLIP_BSTATE ){
			ChgLinkBtnState( ID_LINKON_BSTATE );
		}
		else{
			ChgLinkBtnState( ID_DISPSLIP_BSTATE );
		}
//<-- '12.10.01 INS END
		return 0;
	}

	BOOL	isShift = FALSE;
	if( GetKeyState(VK_SHIFT)&0x8000 ){
		isShift = TRUE;
	}

	// イメージ表示コントロール
	RECT	clRect;
	GetClientRect( &clRect );
	this->ClientToScreen( &clRect );

/*--*/
// 過去仕訳の位置ではなく、画面左上に表示するように
//	RECT	rect;
//	rect = GetRDDlgRECT();
//	CRect	rc( rect );
/*--*/

	int wType = GetMultiWindowType();
//	if( wType == MULTIWINDOW_FULL ){
	if( (wType==MULTIWINDOW_FULL) || (wType==MULTIWINDOW_SIMPLE) ){
/*- '12.03.02 -*/
//		GetClientRect( &clRect );
//		this->ClientToScreen( &clRect );
/*-------------*/
//		GetWindowRect( &clRect );
		((CMainFrame*)GetDBDinpMainFrame())->GetWindowRect( &clRect );
/*-------------*/

//		long adWidth = (clRect.right - clRect.left) / 3;
		long adWidth = (clRect.right - clRect.left) / 3 * 2;
		long adHeight = (clRect.bottom - clRect.top) / 3 * 2;

		clRect.left = clRect.right;
		clRect.right = clRect.left + adWidth;
		clRect.bottom = clRect.top + adHeight;

		int maxHeight = GetSystemMetrics( SM_CYSCREEN );	// 高さ
		int maxWidth = GetSystemMetrics( SM_CXSCREEN );		// 幅

		if( clRect.bottom > maxHeight ){
			long sub = clRect.bottom - maxHeight;
			clRect.bottom -= sub;
			clRect.top -= sub;
		}
		if( clRect.right > maxWidth ){
			long sub = clRect.right - maxWidth;
			clRect.left -= sub;
			clRect.right -= sub;
		}
	}
	else{
		long adWidth = (clRect.right - clRect.left) / 2;
		long adHeight = (clRect.bottom - clRect.top) / 2;

		clRect.left += adWidth;
		clRect.bottom -= adHeight;
	}

	// フォーカス位置取得
	int pn = get_nowpn();

	BOOL	mkFlg = FALSE;
	if( m_pImgmng == NULL ){
		m_pImgmng = new CImgMng;
		IMG_INITREC	initrec;
		memset( &initrec, '\0', sizeof(IMG_INITREC) );
		initrec.pDBZmSub = pDBzm;
		m_pImgmng->Init( initrec );
		m_pImgmng->GetImgDispOption( m_ImgDispOption );
//--> '12.04.12 INS START
		m_Seldata.SetImgMng( m_pImgmng );
//<-- '12.04.12 INS END
		mkFlg = TRUE;
	}

	CRect	rc( clRect );
	if( m_Imgcnt == -1 ){
		IMG_DLGOPTION	dlgOption;
		memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
		if( INP_mode == _APPEND ){
/*- '12.09.22 -*/
//			dlgOption.btnDispSw |= (0x01|0x02|0x04|0x08|0x10|0x20);
/*-------------*/
			dlgOption.btnDispSw |= (0x01|0x02|0x04|0x08);
/*== '12.10.03 ==*/
//			if( (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_DO_LINKIMG) ){
/*===============*/
			if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
/*===============*/
				dlgOption.btnDispSw |= (0x10|0x20);
			}
/*-------------*/
//			m_pImgmng->DispImgDlgOn( this, dlgOption, &rc );
//--> '12.08.13 INS START
			if( m_NowImgSeq != -1 ){
				dlgOption.imgDispSw |= 0x01;
				dlgOption.DispImgSeq = m_NowImgSeq;
			}
//<-- '12.08.13 INS END
		//	dlgOption.btnHideSw |= 0x01;

			m_pImgmng->DispImgDlgOn( this, dlgOption, &rc );
			m_DImgdlgType = ID_DBDINP_INPUTDLG_TYPE;
			m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
			m_Imgcnt = 0;
			ChgLinkBtnState( ID_LINKON_BSTATE );
		}
		else{
/*- '12.09.07 -*/
//			dlgOption.btnDispSw |= (0x04|0x08);
//			m_pImgmng->DispImgDlgOn( pDBzm, getLINE_DATA()->_XXDTA.dd_seq, this, dlgOption );
//			m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
//			if( IsDispSlip() ){
//				ChgLinkBtnState( ID_LINKOFF_BSTATE );
//			}
//			else{
//				ChgLinkBtnState( ID_DISPSLIP_BSTATE );
//			}
/*-------------*/
			m_SRECdseqAry.RemoveAll();

			if( getLINE_DATA()->_XXDTA.dd_dsign&0x800 ){
/*- '12.09.22 -*/
//				dlgOption.btnDispSw |= (0x04|0x08|0x10|0x20);
/*-------------*/
/*== '12.10.02 ==*/
//				dlgOption.btnDispSw |= (0x04|0x08);
/*===============*/
				dlgOption.btnDispSw |= (0x01 | 0x02 | 0x04 | 0x08 );
/*===============*/
/*=== '12.10.03 ===*/
//				if( (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_DO_LINKIMG) ){
/*=================*/
				if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
/*=================*/
					dlgOption.btnDispSw |= (0x10|0x20);
				}
/*-------------*/
#ifdef _SCANNER_SV_
				dlgOption.imgDispSw |= 0x02;
#endif
			//	dlgOption.btnHideSw |= 0x01;

				m_pImgmng->DispImgDlgOn( pDBzm, getLINE_DATA()->_XXDTA.dd_seq, this, dlgOption );
				m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
//--> '12.09.13 INS START
				m_ImgDtseq = getLINE_DATA()->_XXDTA.dd_seq;
//<-- '12.09.13 INs END
				m_DImgdlgType = ID_DBDINP_CONFIRMDLG_TYPE;
				m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );

				m_pImgmng->GetRelImgAry(m_ImgDtseq, m_SRECdseqAry);

				if( IsDispSlip() ){
					ChgLinkBtnState( ID_LINKOFF_BSTATE );
				}
				else{
					ChgLinkBtnState( ID_DISPSLIP_BSTATE );
				}
			}
			else{
/*- '12.09.22 -*/
//				dlgOption.btnDispSw |= (0x01|0x02|0x04|0x08|0x10|0x20);
/*-------------*/
				dlgOption.btnDispSw |= (0x01 | 0x02 | 0x04 | 0x08);
/*== '12.10.03 ==*/
//				if( (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_DO_LINKIMG) ){
/*===============*/
				if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
/*===============*/
					dlgOption.btnDispSw |= (0x10|0x20);
				}
/*-------------*/
				if( m_NowImgSeq != -1 ){
					dlgOption.imgDispSw |= 0x01;
					dlgOption.DispImgSeq = m_NowImgSeq;
				}
#ifdef _SCANNER_SV_
				dlgOption.imgDispSw |= 0x02;
#endif
			//	dlgOption.btnHideSw |= 0x01;
				m_pImgmng->DispImgDlgOn( this, dlgOption, &rc );
				m_DImgdlgType = ID_DBDINP_INPUTDLG_TYPE;
				m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
//--> '12.09.24 INS START
				m_ImgDtseq = -1;
//<-- '12.09.24 INS END
				m_Imgcnt = 0;
				ChgLinkBtnState( ID_LINKON_BSTATE );
			}
/*-------------*/
		}
	}
	else{
		if( m_pImgmng->IsDlgVisible() == FALSE ){
			if( INP_mode == _APPEND ){
				IMG_DLGOPTION	dlgOption;
				memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
/*- '12.09.22 -*/
//				dlgOption.btnDispSw |= (0x01|0x02|0x04|0x08|0x10|0x20);
/*-------------*/
				dlgOption.btnDispSw |= (0x01 | 0x02 | 0x04 | 0x08);
/*== '12.10.03 ==*/
//				if( (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_DO_LINKIMG) ){
/*===============*/
				if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
/*===============*/
					dlgOption.btnDispSw |= (0x10|0x20);
				}
/*-------------*/
//--> '12.08.13 INS START
				if( m_NowImgSeq != -1 ){
					dlgOption.imgDispSw |= 0x01;
					dlgOption.DispImgSeq = m_NowImgSeq;
				}
//<-- '12.08.13 INS END
			//	dlgOption.btnHideSw |= 0x01;
				m_pImgmng->DispImgDlgOn( this, dlgOption );
				m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
				m_DImgdlgType = ID_DBDINP_INPUTDLG_TYPE;
				ChgLinkBtnState( ID_LINKON_BSTATE );
			}
			else{
/*- '12.09.07 -*/
//				IMG_DLGOPTION	dlgOption;
//				memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
//				dlgOption.btnDispSw |= (0x04|0x08);
//				m_pImgmng->DispImgDlgOn( pDBzm, getLINE_DATA()->_XXDTA.dd_seq, this, dlgOption );
//				if( IsDispSlip() ){
//					ChgLinkBtnState( ID_LINKOFF_BSTATE );
//				}
//				else{
//					ChgLinkBtnState( ID_DISPSLIP_BSTATE );
//				}
/*-------------*/
				if( getLINE_DATA()->_XXDTA.dd_dsign&0x800 ){
					IMG_DLGOPTION	dlgOption;
					memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
/*- '12.09.22 -*/
//					dlgOption.btnDispSw |= (0x04|0x08|0x10|0x20);
/*-------------*/
/*== '12.10.02 ==*/
//					dlgOption.btnDispSw |= (0x04|0x08);
/*===============*/
					dlgOption.btnDispSw |= (0x04|0x08|0x01|0x02);
/*===============*/
/*=== '12.10.03 ===*/
//					if( (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_DO_LINKIMG) ){
/*=================*/
					if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
/*=================*/
						dlgOption.btnDispSw |= (0x10|0x20);
					}
/*-------------*/
#ifdef _SCANNER_SV_
					dlgOption.imgDispSw |= 0x02;
#endif
				//	dlgOption.btnHideSw |= 0x01;
					m_pImgmng->DispImgDlgOn( pDBzm, getLINE_DATA()->_XXDTA.dd_seq, this, dlgOption );
					m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
//--> '12.09.13 INS START
					m_ImgDtseq = getLINE_DATA()->_XXDTA.dd_seq;
//<-- '12.09.13 INs END
					m_DImgdlgType = ID_DBDINP_CONFIRMDLG_TYPE;
					if( IsDispSlip() ){
						ChgLinkBtnState( ID_LINKOFF_BSTATE );
					}
					else{
						ChgLinkBtnState( ID_DISPSLIP_BSTATE );
					}
				}
				else{
					IMG_DLGOPTION	dlgOption;
					memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
/*- '12.09.22 -*/
//					dlgOption.btnDispSw |= (0x01|0x02|0x04|0x08|0x10|0x20);
/*-------------*/
					dlgOption.btnDispSw |= (0x01 | 0x02 | 0x04 | 0x08);
					/*== '12.10.03 ==*/
//					if( (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) && (m_ImgDispOption.linkWorkType==ID_DO_LINKIMG) ){
/*===============*/
					if( m_ImgDispOption.autoLkType != ID_AUTOLINK_NOLINK && !IsJzSansyo() ){
/*===============*/
						dlgOption.btnDispSw |= (0x10|0x20);
					}
/*-------------*/
					if( m_NowImgSeq != -1 ){
						dlgOption.imgDispSw |= 0x01;
						dlgOption.DispImgSeq = m_NowImgSeq;
					}
#ifdef _SCANNER_SV_
					dlgOption.imgDispSw |= 0x02;
#endif
				//	dlgOption.btnHideSw |= 0x01;
					m_pImgmng->DispImgDlgOn( this, dlgOption );
					m_pImgmng->DispDlgSetCallBack( ImgDlgFunction, this );
					m_DImgdlgType = ID_DBDINP_INPUTDLG_TYPE;
//--> '12.09.24 INS START
					m_ImgDtseq = -1;
//<-- '12.09.24 INS END
					ChgLinkBtnState( ID_LINKON_BSTATE );
				}
/*-------------*/
			}
		}
		else{
			if( isShift ){
				m_pImgmng->DispImgDlgPrevPage();
			}
			else{
				m_pImgmng->DispImgDlgNextPage();
			}
		}
	}

//--> '12.10.02 INS START
	// ツールバー変更
	if( pn != TKY_PN ){
		ChangeToolbarSub( pn );
	}
	else{
		ChangeToolbarSubInTkypn( pn );
	}
//<-- '12.10.02 INS END


//MyTrace("@DispImgDlg pn = %d, INP_mode = %d, mkFlg = %d", pn, INP_mode, mkFlg);

	// フォーカス再セット
	if( pn != -1 ){
		int pn = get_nowpn();
//		if( (bKeyDown!=FALSE) || (INP_mode!=_KEYSCAN) ){
		if( INP_mode !=_KEYSCAN ){
			set_focus( pn );
		}
		else{
			int old_lncnt = SCROLL_NL();
			m_DtList.GetDispDataCount();
			int now_lncnt = m_DtList.GetDispDataCount();
			if( old_lncnt == now_lncnt ){
				set_focus( pn );
			}
		}
		//フォーカスが原票表示ボタンのままの対応
		SetTimer(nTimerID, 1000, NULL);

		if( mkFlg && (INP_mode==_APPEND) ){
			if( pn == DATE_PN ){
				m_pImgmng->MoveSelectInDlg( ID_DATE_SEARCH );
			}
			else if( pn == VAL_PN ){
				m_pImgmng->MoveSelectInDlg( ID_MONY_SEARCH );
			}
			else if( pn == TKY_PN ){
				m_pImgmng->MoveSelectInDlg( ID_ALL_SEARCH );
			}
		}
	}

	return 0;
}


void CDBDinpView::OnTimer(UINT nIDEvent)
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if(  nIDEvent == nTimerID ) {
		//原票表示ダイアログの表示に時間がかかった場合など、DBEDITにフォーカスがセットされない場合の対処
		//Enter押下で、次項目にカーソルがセットされる場合もあった。
		if( IsDispSlip() ) {
			CWnd* pwnd;
			pwnd = GetFocus();

			if( pwnd && IsWindow(pwnd->m_hWnd) ) {
				int id = pwnd->GetDlgCtrlID();

				if( id == IDC_DISPSLIP_BTN ) {
					int pn = get_nowpn();
					if( pn != -1 ) {
						set_focus(pn);
					}
				}
				else if( id == IDC_ICSDBEDTCTRL2 ) {
					if( !m_Inp2.IsWindowVisible() ) {
						int pn = get_nowpn();
						if( pn != -1 ) {
							set_focus(pn);
						}
					}
				}
				else if( id == IDC_ICSDBEDTCTRL1 ) {
					if( !m_Input.IsWindowVisible() ) {
						int pn = get_nowpn();
						if( pn != -1 ) {
							set_focus(pn);
						}
					}
				}
			}
		}
		//タイマーは最初のみ
		KillTimer(nTimerID);
		return;
	}
	__super::OnTimer(nIDEvent);
}


//-----------------------------------------------------------------------------
// フォーカス再セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBDinpView::ResetFocusFormImg()
{
	int pn = get_nowpn();
	int ln = get_nowln();

	if( ln == APPEND_LINE ) {
		set_focus( pn );
	}
	else if( ln != -1 ) {
		set_focus( pn );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 摘要再セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBDinpView::ResetTekiyoFromImg()
{
	// 摘要を元に戻す
	if( m_SelDispPN == SL_SLIP_TKY_PN ){
		SelPar	par;
		par.sel_pn = SL_TKY_PN;
		par.dbt	= CREC.m_dbt;
		par.cre = CREC.m_cre;
		::ZeroMemory( par.tky_kana, sizeof par.tky_kana );

		m_Seldata.SelectJob( &par, -1 );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// タブクリックによる切替時の チェック
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBDinpView::TabLbuttonDownJob()
{
	HideImgDlg();

	SelWndShow( FALSE );
	
	GetDtListItemWidth();

	return 0;
}

//-----------------------------------------------------------------------------
// イメージダイアログ非表示
//-----------------------------------------------------------------------------
void CDBDinpView::HideImgDlg(int prn/*=0*/)
{
	if( IsDispSlip() || prn ){
//--> '12.08.10 INS START
		if( m_pImgmng->GetImgDlgType() == 0 ){
			m_NowImgSeq = m_pImgmng->GetDocseqOnImgDlg();
		}
//<-- '12.08.10 INS END
/*- '12.10.02 -*/
//		m_pImgmng->DispImgDlgOff();
/*-------------*/
		TurnOffDispImgDlg();;
/*-------------*/
//--> '12.09.21 INS START
		DelAllDocseq();
//<-- '12.09.21 INS END

// '12.09.29 TEST_HIDE
		ChgLinkBtnState( ID_DISPSLIP_BSTATE );
	}

//--> '12.09.24 INS START
	ChangeImgdlgType( ID_DBDINP_NODISPDLG_TYPE );
	m_ImgDtseq = -1;
//<-- '12.09.24 INS END
}

//-----------------------------------------------------------------------------
// 関連付けるイメージから表示中のイメージを削除する
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBDinpView::DelDocseq()
{
	if( IsDispSlip() == FALSE ){
		return 0;
	}

	int	docseq;
	if( (docseq=m_pImgmng->GetDocseqOnImgDlg()) == -1 ){
		return 0;
	}

	int max = (int)m_ContDocseqAry.GetCount();
	for( int i=(max-1); i>=0; i-- ){
		if( m_ContDocseqAry[i] == docseq ){
			m_ContDocseqAry.RemoveAt( i );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 関連付けるイメージを全て削除する
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBDinpView::DelAllDocseq()
{
	if( INP_mode == _APPEND ){
		m_ContDocseqAry.RemoveAll();
		CREC.m_dsign[6] &= ~0x01;
	}
	else{
		m_ContDocseqAry.RemoveAll();
		if( IsConnectedDoc() == 0 ){
			CREC.m_dsign[6] &= ~0x01;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// イメージダイアログの表示タイプを変更する ('12.09.18)
//-----------------------------------------------------------------------------
// 引数	dtype	：	変更タイプ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBDinpView::ChangeImgdlgType( EnumIdDispImgDlgType dtype )
{
	m_DImgdlgType = dtype;

	return 0;
}

//-----------------------------------------------------------------------------
// アクティブになった時にフォーカスを再セットして良い状態？ ('12.09.21)
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	再セットOK
//			FALSE	：	再セットNG
//-----------------------------------------------------------------------------
BOOL CDBDinpView::IsAbleToResetFocusState()
{
	BOOL	rt = TRUE;

	if( IsDispSlip() ){
		if( (INP_mode==_APPEND) && (m_DImgdlgType==ID_DBDINP_INPUTDLG_TYPE) ){
			;
		}
		else{
			rt = FALSE;
		}
	}

	return rt;
}


void CDBDinpView::OnMenuSlip()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	if( m_bSLIP_MASTER ){
		if( m_pImgmng == NULL ){
			m_pImgmng = new CImgMng;
			IMG_INITREC	initrec;
			memset( &initrec, '\0', sizeof(IMG_INITREC) );
			initrec.pDBZmSub = pDBzm;
			m_pImgmng->Init( initrec );
			m_pImgmng->GetImgDispOption( m_ImgDispOption );
			m_Seldata.SetImgMng( m_pImgmng );
		}
		else{
			if( IsDispSlip() ){
				HideImgDlg();
			}
			else {
				ChgLinkBtnState( ID_DISPSLIP_BSTATE );
			}
		}

		m_pImgmng->ImgOptionGoModal( this );
		ImageOptionChanged();
		// 他のビューに伝える
		((CMainFrame*)GetDBDinpMainFrame())->SendChangeSlipOption();
	}
}

void CDBDinpView::OnUpdateMenuSlip(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if( m_bSLIP_MASTER ){
		pCmdUI->Enable( TRUE );
	}
	else{
		pCmdUI->Enable( FALSE );
	}
}

//-----------------------------------------------------------------------------
// イメージ表示オプション変更 ('12.09.26)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBDinpView::ImageOptionChanged()
{
	if( m_bSLIP_MASTER && m_pImgmng ){
		m_pImgmng->GetImgDispOption( m_ImgDispOption );
		//m_ImgDispOption.dtypeInInput は常にID_DISP_IN_INPUT

		if( (m_bSLACC_BUTTON_DISP==FALSE) && (m_ImgDispOption.dtypeInInput==ID_DISP_IN_INPUT) ){
			m_bSLACC_BUTTON_DISP = TRUE;
/*- '12.10.01_CUT -*/
//			m_DispSlipBtn.EnableWindow( TRUE );
/*-----------------*/
			ChgLinkBtnState( ID_DISPSLIP_BSTATE );
		}
		else if( (m_bSLACC_BUTTON_DISP==TRUE) && (m_ImgDispOption.dtypeInInput==ID_HIDE_IN_INPUT) ){
			m_bSLACC_BUTTON_DISP = FALSE;
/*- '12.10.01_CUT -*/
//			m_DispSlipBtn.EnableWindow( FALSE );
/*-----------------*/
			ChgLinkBtnState( ID_DISPSLIP_BSTATE ); 
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 最新表示ドキュメントシーケンス保存 ('12.09.28)
//-----------------------------------------------------------------------------
// 引数	svDocseq	：	保存するドキュメントシーケンス
//-----------------------------------------------------------------------------
void CDBDinpView::SaveDocseq( int svDocseq )
{
	m_NowImgSeq = svDocseq;
}


void CDBDinpView::OnBnClickedDispslipChk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
//--> '12.10.03 INS START
	if( IsDispSlip() ){
		prevImgSeq = -1;
		HideImgDlg();
		int pn = get_nowpn();
		if( INP_mode !=_KEYSCAN ){
			set_focus( pn );
		}
	}
	else if( (INP_mode!=_APPEND) && (m_DispSlipChk.GetCheck()==0) && (m_ImgDispOption.dtypeInCorrect==ID_HIDE_IN_CORRECT) ){
		HideImgDlg();
		int pn = get_nowpn();
		if( INP_mode !=_KEYSCAN ){
			set_focus( pn );
		}
	}
	else if( m_bSLACC_BUTTON_DISP ){
		DispImgDlg(FALSE);
	}
}
//<-- '12.10.03 INS END


//-----------------------------------------------------------------------------
// 原票表示モードをオフにする ('12.10.02)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBDinpView::TurnOffDispImgDlg()
{
	if( IsDispSlip() ){
		// ダイアログを消す
		m_pImgmng->DispImgDlgOff(); 
		// ツールバーを切り替える
		int pn = get_nowpn();

		if( pn != TKY_PN ){
			ChangeToolbarSub( get_nowpn() );
		}
		else{
			ChangeToolbarSubInTkypn( get_nowpn() );
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 表示中のイメージを消す ('12.10.03)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBDinpView::HideDispImgDlg( BOOL bScanViewEx/*=FALSE*/ )
{

	if( IsDispSlip() ){
		// 関連対象クリア
		m_ContDocseqAry.RemoveAll();

		// 表示更新
		TurnOffDispImgDlg();

		// ボタンの状態変更
		ChgLinkBtnState( ID_DISPSLIP_BSTATE, FALSE );

		// 表示状態変更
		ChangeImgdlgType( ID_DBDINP_NODISPDLG_TYPE );
//		m_ImgDtseq = -1;
	}

	if( bScanViewEx ) {
		if( m_DispSlipChk.GetCheck() ) {
			m_DispSlipChk.SetCheck(0);
		}
	}

	return 0;
}

#endif	// _SLIP_ACCOUNTS_

#ifdef _SCANNER_SV_
//-----------------------------------------------------------------------------
// 原票との関連付け状態を変更する('16.01.24)
//-----------------------------------------------------------------------------
// 引数		sw	：	変更後の状態
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBDinpView::ChgLink()
{
	if( IsDispSlip() ){
		if( m_pImgmng ){
			m_pImgmng->ReverseLinkStateInImgDlg();
		}
	}
	return 0;
}
#endif	 // _SCANNER_SV_



// 高さ が一番高いコントロールから並べる
static int blkID_0[] = {
	IDC_ICSSELCTRL1, IDC_ICSSELCTRL2, IDC_SELGUIDE, 0
};

static int blkID_1[] = {
	IDC_DTLIST, 0
};

static int blkID_2[] = {
	IDC_MNTHSEL, IDC_ICSDISPCTRL10, IDC_ICSDISPRDREMAIN, 
	IDC_DISPSLIP_BTN, IDC_LINKSLIP_BTN, IDC_DISPSLIP_CHK, IDC_STATIC_POS, 0
};

static int blkID_3[] = {
	IDC_DTLINE, 0
};

static int blkID_4[] = {
	IDC_ICSDISPCTRL12, IDC_ICSDISPGENKIN, IDC_ICSDISPCTRL11, IDC_ICSDISPSYOGT, IDC_STATIC_POS2, 0
};

// 移動用 表示ビットマップ作成
//	int mode	0:新規入力行あり
//				1:確定マスター等 新規入力行なし
//
void CDBDinpView::DispBmpMake( int mode )
{
	if( ! m_bBmpMake ) {
		if( m_ScrnBmp.GetSafeHandle() != NULL ) {
			m_ScrnBmp.DeleteObject();
		}

		CDC* pdc = GetDC();
//		CDC* pdc = GetWindowDC();

		CRect bmRect;
		GetClientRect(&bmRect);
		m_ScrnBmp.CreateCompatibleBitmap(pdc, bmRect.Width(), bmRect.Height()/*1500*/ );

		CDC mDC;
		mDC.CreateCompatibleDC(pdc);
		//ビューイメージをbitmapへ描写する
		CGdiObject* pOld = mDC.SelectObject(&m_ScrnBmp);
		mDC.BitBlt(0, 0, bmRect.Width(), bmRect.Height(), pdc, 0, 0, SRCCOPY);
		mDC.SelectObject(pOld);
		mDC.DeleteDC();

		ReleaseDC( pdc );

		int mrgn = 2;

		// 選択欄
		CRect rect;
		GetDlgItem( blkID_0[0] )->GetWindowRect( &rect );
		ScreenToClient( rect );
		m_blkRect[0].SetRectEmpty();
		m_blkRect[0].top	= rect.top;
		m_blkRect[0].bottom = rect.bottom + mrgn;

		// 仕訳リスト
		GetDlgItem( blkID_1[0] )->GetWindowRect( &rect );
		ScreenToClient( rect );
		m_blkRect[1].SetRectEmpty();
		m_blkRect[1].top	= rect.top;
		m_blkRect[1].bottom = rect.bottom + mrgn;

		// 入力タイプ
		GetDlgItem( blkID_2[0] )->GetWindowRect( &rect );
		ScreenToClient( rect );
		m_blkRect[2].SetRectEmpty();
		m_blkRect[2].top	= rect.top;
		m_blkRect[2].bottom = rect.bottom + mrgn;

		// 新規行入力
		GetDlgItem( blkID_3[0] )->GetWindowRect( &rect );
		ScreenToClient( rect );
		m_blkRect[3].SetRectEmpty();
		m_blkRect[3].top	= rect.top;
		m_blkRect[3].bottom = rect.bottom + mrgn;

		// 現金貸借
		GetDlgItem( blkID_4[0] )->GetWindowRect( &rect );
		ScreenToClient( rect );
		m_blkRect[4].SetRectEmpty();
		m_blkRect[4].top	= rect.top;
		m_blkRect[4].bottom = rect.bottom + mrgn/*(mrgn*5)*/;

		// 縦スクロールバー
		if( pAUTOSEL->BLKSEL_OPT == OPT_NONOPT ) {
			SCROLLINFO	si = {0};
			CRect	rcView;
			GetClientRect(rcView);

			si.cbSize = sizeof(SCROLLINFO);
			GetScrollInfo(SB_VERT, &si );

			BOOL	bScrol = FALSE;
			// スクロールバーの表示あり
			if( rcView.bottom < si.nMax )
				bScrol = TRUE;

			if( ! bScrol && ! mode ) {
				m_bBmpMake = TRUE;
			}
		}
	}
}


//入力タイプ・現金欄 ブロックをスキャン時に分けるか？
BOOL CDBDinpView::IsBlkInpGnkinDevide()
{
	BOOL bDevide = FALSE;

	int	pos_inptype, pos_dtlist, pos_gnkin, pos_select;
	pos_inptype = pos_dtlist = pos_gnkin = pos_select = 0;

	//入力タイプ・現金欄 の位置をチェック
	for( int n = 0; n < DINP_BLKMAX; n++ ) {
		if( m_blkIdx[n] == BLK_INPTYPE ) {
			pos_inptype = n;
		}
		if( m_blkIdx[n] == BLK_DTLIST ) {
			pos_dtlist = n;
		}
		if( m_blkIdx[n] == BLK_GNKIN ) {
			pos_gnkin = n;
		}
		if( m_blkIdx[n] == BLK_SELECT ) {
			pos_select = n;
		}
	}
	//仕訳リストの上下に　入力タイプ・現金欄 ブロック
	if( pos_inptype < pos_dtlist && pos_gnkin > pos_dtlist ) {
		bDevide = TRUE;
	}
	if( pos_gnkin < pos_dtlist && pos_inptype > pos_dtlist ) {
		bDevide = TRUE;
	}
	//科目等選択欄の上下に　入力タイプ・現金欄 ブロック
	if( pAUTOSEL->BLKSEL_OPT == OPT_NONOPT ) {
		if( pos_inptype < pos_select && pos_gnkin > pos_select ) {
			bDevide = TRUE;
		}
		if( pos_gnkin < pos_select && pos_inptype > pos_select ) {
			bDevide = TRUE;
		}
	}


	return bDevide;
}



void CDBDinpView::BlkWndMove( int nKeyScanFlg/* = -1*/)
{
	int *pid;
	int n, listidx, i, height, nxtYpos;
	CRect	rc, rcView;

FILETrace( "---------------> BlkWndMove TOP nKeyScan = %d\n", nKeyScanFlg );

	CRect	rcList;	// 仕訳リストの座標位置
	rcList.SetRectEmpty();

	listidx = nxtYpos = 0;

	GetClientRect(rcView);

	SCROLLINFO	si = {0};
	si.cbSize = sizeof(SCROLLINFO);
	GetScrollInfo(SB_VERT, &si );

	BOOL	bScrol = FALSE;

	// スクロールバーの表示あり
	if( rcView.bottom < si.nMax ) {
		bScrol = TRUE;
		if( si.nPos > 0 )	nxtYpos = (si.nPos * -1);
	}

//MyTrace( "si.nMin = %d, si.nMax = %d, si.nPage = %d, si.nPos = %d, si.nTrackPos = %d\n", si.nMin, si.nMax, si.nPage, si.nPos, si.nTrackPos );
//MyTrace( "bScrol = %d, rcView left = %d, top = %d, right = %d, bottom = %d\n", bScrol, rcView.left, rcView.top, rcView.right, rcView.bottom );

	// スキャンモード用
	BOOL bKeyScan = FALSE;
	BOOL bInpGnkinDevide = IsBlkInpGnkinDevide();

	if( nKeyScanFlg == -1 ) {
		if( IsMAsterKAKUTEI() )	bKeyScan = TRUE;
		else {
			if( INP_mode == _KEYSCAN )	bKeyScan = TRUE;
		}
	}
	else {
		bKeyScan = nKeyScanFlg;
	}

	int ht_adj = 0;

	if( IMG_master != ERR ) { //OCR
		CRect rcSYOG;
		GetDlgItem(IDC_ICSDISPSYOGT)->GetWindowRect( rcSYOG );
		ht_adj	= rcSYOG.Height();
		ht_adj += (ht_adj / 6);
	}

	if( m_bSLIP_MASTER ) {	//原票保存
		CRect rcSlip;
		GetDlgItem(IDC_DISPSLIP_CHK)->GetWindowRect( rcSlip );
		ht_adj	= rcSlip.Height();
	}
	else {
		CRect rcSYOG;
		GetDlgItem(IDC_ICSDISPSYOGT)->GetWindowRect(rcSYOG);
		ht_adj = rcSYOG.Height();
		ht_adj += (ht_adj / 6);
	}

	int nShow = SW_SHOW;
	BOOL	bListHtadj = FALSE;

	if( pAUTOSEL->BLKSEL_OPT != OPT_NONOPT ) {
		nShow = SW_HIDE;

		if( bKeyScan ) {
			if( pAUTOSEL->DSPLNWD_OPT == OPT_NARROW ) {
				bListHtadj = TRUE;
			}
		}
	}
	m_selfix8.ShowWindow(nShow);
	m_selval32.ShowWindow(nShow);
	m_selGuid.ShowWindow(nShow);

	// 画面の上から コントロールをセット
	for( n = 0; n < DINP_BLKMAX; n++ ) {
		pid = NULL;

		if( bKeyScan ) {
			if( m_InsLine != -1 ) {
				if( ! bInpGnkinDevide ) {
					if( m_blkIdx[n] == BLK_GNKIN ) {
						continue;
					}
				}
			}
			else {
				if( ! bInpGnkinDevide ) {
					if( m_blkIdx[n] == BLK_DTNEW || m_blkIdx[n] == BLK_GNKIN ) {
						continue;
					}
				}
				else {
					if( m_blkIdx[n] == BLK_DTNEW ) {
						continue;
					}
				}
			}
		}

		if( pAUTOSEL->BLKSEL_OPT != OPT_NONOPT ) {
			if( m_blkIdx[n] == BLK_SELECT )
				continue;
		}

		switch( m_blkIdx[n] ) {
			case BLK_SELECT:
				pid = blkID_0;	break;
			case BLK_DTLIST:
				pid = blkID_1;	break;	// 仕訳リスト
			case BLK_INPTYPE:
				pid = blkID_2;	break;
			case BLK_DTNEW:
				pid = blkID_3;	break;
			case BLK_GNKIN:
				pid = blkID_4;	break;
		}

		if( m_blkIdx[n] == BLK_DTLIST ) {
			rcList.top = nxtYpos;
			listidx = n;
			break;
		}
		else {
			height = 0;

			for( i = 0; *(pid+i) != 0; i++ ) {
				GetDlgItem( *(pid+i) )->GetWindowRect( &rc );
				ScreenToClient( rc );

				int ht = rc.Height();

				rc.top		= nxtYpos;
				rc.bottom	= rc.top + ht;

				if( height < ht ) {
					height = ht;
				}
				GetDlgItem( *(pid+i) )->MoveWindow( &rc );
			}

			//OCRイメージ表示ウィンドウ or 原票表示ボタン分 調整
			if(  m_blkIdx[n] == BLK_INPTYPE ) {
				if( bKeyScan ) {
					height += ht_adj;
				}
			}

			nxtYpos += height;
		}
	}

	// 画面の下部から コントロールをセット
	nxtYpos = rcView.bottom;

	if( bScrol ) {
		int sbn = si.nMax - si.nPage - si.nPos;

		if( sbn > 0 ) {
			nxtYpos += sbn;
		}
	}

	for( n = (DINP_BLKMAX-1); n >= listidx; n-- ) {

		pid = NULL;

		if( bKeyScan ) {
			if( m_InsLine != -1 ) {
				if( ! bInpGnkinDevide ) {
					if( m_blkIdx[n] == BLK_GNKIN ) {
						continue;
					}
				}
			}
			else {
				if( ! bInpGnkinDevide ) {
					if( m_blkIdx[n] == BLK_DTNEW || m_blkIdx[n] == BLK_GNKIN ) {
						continue;
					}
				}
				else {
					if( m_blkIdx[n] == BLK_DTNEW )
						continue;
				}
			}
		}
		if( pAUTOSEL->BLKSEL_OPT != OPT_NONOPT ) {
			if( m_blkIdx[n] == BLK_SELECT )
				continue;
		}

		switch( m_blkIdx[n] ) {
			case BLK_SELECT:
				pid = blkID_0;	break;
			case BLK_DTLIST:
				pid = blkID_1;	break;	// 仕訳リスト
			case BLK_INPTYPE:
				pid = blkID_2;	break;
			case BLK_DTNEW:
				pid = blkID_3;	break;
			case BLK_GNKIN:
				pid = blkID_4;	break;
		}

		if( m_blkIdx[n] == BLK_DTLIST ) {
			rcList.bottom = nxtYpos;
			break;
		}
		else {
			height = 0;

			if(  m_blkIdx[n] == BLK_INPTYPE ) {
				if( bKeyScan ) {
					nxtYpos -= ht_adj;
				}
			}

			for( i = 0; *(pid+i) != 0; i++ ) {
				GetDlgItem( *(pid+i) )->GetWindowRect( &rc );
				ScreenToClient( rc );

				int ht = rc.Height();

				rc.bottom	= nxtYpos;
				if( ht > height ) {
					rc.top		= nxtYpos - ht;
				}
				else {
					//同ブロックは 上側座標で揃える
					rc.top		= nxtYpos - height;
					rc.bottom	= rc.top + ht;
				}

				if( height < ht ) {
					height = ht;
				}
				GetDlgItem( *(pid+i) )->MoveWindow( &rc );
			}

			//OCRイメージ表示ウィンドウ or 原票表示ボタン分 調整
		//	if(  m_blkIdx[n] == BLK_INPTYPE ) {
		//		if( bKeyScan ) {
		//			height += ht_adj;
		//		}
		//	}

			nxtYpos -= height;
		}
	}

	m_DtList.GetWindowRect( &rc );
	ScreenToClient( rc );

	rc.top		= rcList.top;
	rc.bottom	= rcList.bottom;

	m_DtList.MoveWindow( &rc );

	if( bListHtadj ) {
		int Dtcnt = m_DtList.GetDispDataCount();
		int Sccnt = m_pScanView->GetScListCount();

		if( Dtcnt > 0 ) {
			if( Dtcnt > Sccnt ) {
				int orgHeight = rc.bottom - rc.top;

				int ln_ht = (orgHeight / Dtcnt);
				rc.bottom = rc.top + (ln_ht * Sccnt);

				int newHeight = rc.bottom - rc.top;
				int ht = orgHeight - newHeight;
				rc.top		= rcList.top + (ht/2);
				rc.bottom	= rcList.bottom - (ht/2);

				m_DtList.MoveWindow( &rc );
			}
		}
	}

	m_ChkViewRect.SetRectEmpty();

	_disp_change();

	//再描画
	m_selDate.RedrawWindow();
	m_selGuid.RedrawWindow();
}

//--------------------------------------------------------------------------------
//		int beginApp	= 1	beginApp 時の呼び出し
//		int bOnSize		= 1	OnSize   時の呼び出し, = 10 強制的に移動処理
//
//		返送値 TRUE; OnSizeで大きさ変更あり
//--------------------------------------------------------------------------------
void CDBDinpView::BlkMoveOnSizeJob( int beginApp /*=0*/, int bOnSize/* = 0*/)
{
	CRect vwRect1, vwRect2;
	BOOL	bMove = FALSE;
ONSIZETrace( "@BlkMoveOnSizeJob TOP(%d, %d)\n", beginApp, bOnSize );

	if( beginApp ) {

		if( pAUTOSEL->BLK_OPT[0] != 0 ) {
			m_blkIdx[0] = pAUTOSEL->BLK_OPT[0];
			m_blkIdx[1] = pAUTOSEL->BLK_OPT[1];
			m_blkIdx[2] = pAUTOSEL->BLK_OPT[2];
			m_blkIdx[3] = pAUTOSEL->BLK_OPT[3];
			m_blkIdx[4] = pAUTOSEL->BLK_OPT[4];
		}
		else {
			m_blkIdx[0] = BLK_SELECT;
			m_blkIdx[1] = BLK_DTLIST;
			m_blkIdx[2] = BLK_INPTYPE;
			m_blkIdx[3] = BLK_DTNEW;
			m_blkIdx[4] = BLK_GNKIN;
		}
	}

	try
	{
#ifdef REGDATA	// 11.16 /00
		if( INP_mode == _APPEND || INP_mode == _MODEFY )
		{
			// 定型仕訳残り数表示
			RDRemainDispJOB( );
		}
#endif
		else 
		{
			RDRemainDisp( FALSE );
		}

		// view の 下端 へコントロールを移動する
		CRect vwRect, rc;
		GetClientRect( vwRect );

		int chk_wd = vwRect.Width() - m_ChkViewRect.Width();
		int chk_ht = vwRect.Height() - m_ChkViewRect.Height();

		int view_wd, view_ht;
		view_wd = vwRect.Width();
		view_ht = vwRect.Height();
ONSIZETrace( "@BlkMoveOnSizeJob vwRect(%d, %d), m_ChkViewRect(%d, %d)\n", vwRect.Width(), vwRect.Height(),
			m_ChkViewRect.Width(), m_ChkViewRect.Height());

		// タスクバーのアイコンクリックで最小化したら、View の幅・高さがゼロ
		if( view_wd > 0 && view_ht > 0 ) {
			//以前より、縦・横 3ピクセル以上動いたら、画面サイズ変更
			if( abs( chk_wd ) >= 3 || abs( chk_ht ) >= 3 ) {
				bMove = TRUE;
			}
			else if( bOnSize > 1 ) {
				bMove = TRUE;
			}
		}
/*
		// 04.19 /12
		if( IsNotMultiWindow() ) {
			bMove = FALSE;
		}
		else {
			bMove = TRUE;
		}
*/

		if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() == INP_VIEW && bMove ) {

			// 追加入力用に画面変更
			BOOL bSgn = FALSE;

			if( bOnSize > 1 ) {
				if( IsMAsterKAKUTEI() )	bSgn = TRUE;
			}
			else {
				if( IsMAsterKAKUTEI() )	bSgn = TRUE;
				else {
					if( INP_mode == _KEYSCAN )	bSgn = TRUE;
					else if( INP_mode == _APPEND ) {
						if( m_InsLine != -1 && m_InsMode == _KEYSCAN )
							bSgn = TRUE;
					}
				}
			}
ONSIZETrace( "@BlkMoveOnSizeJob ========= 追加入力用に画面変更 ========(%08x, %02x)\n", bCONFIRM_MASTER, INP_mode );

			ScanMode_WndMove( bSgn, bOnSize );

		}
		// 欄外大きなイメージ
		big_img( getLINE_DATA(), getINP_CTL( TKY_PN ));
		_disp_change();
		AutoSelIndicator( pAUTOSEL, INP_mode );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


void CDBDinpView::SlipWndMove( BOOL bKeyScan )
{
	if( bKeyScan ) {
		CRect	baseRect;
		GetDlgItem( IDC_ICSDISPCTRL10 )->GetWindowRect( &baseRect );
		ScreenToClient( baseRect );

		CRect	dspRect;
//		GetDlgItem( IDC_DISPSLIP_BTN )->GetWindowRect( &dspRect );
		GetDlgItem( IDC_DISPSLIP_CHK )->GetWindowRect( &dspRect );
		ScreenToClient( dspRect );
//		GetDlgItem( IDC_DISPSLIP_BTN )->MoveWindow( baseRect.left, baseRect.bottom + 2, 
		GetDlgItem( IDC_DISPSLIP_CHK )->MoveWindow( baseRect.left, baseRect.bottom + 2, 
															dspRect.Width(), dspRect.Height()  );

		CRect	lnkRect;
		GetDlgItem( IDC_DISPSLIP_CHK )->GetWindowRect( &lnkRect );
		ScreenToClient( lnkRect );
		GetDlgItem( IDC_LINKSLIP_BTN )->MoveWindow( baseRect.left + dspRect.Width() + 2, baseRect.bottom + 2, 
															lnkRect.Width(), lnkRect.Height()  );

		GetDlgItem(IDC_GNPSCAN_BTN)->MoveWindow(baseRect.left + dspRect.Width() + 2, baseRect.bottom + 2,
			lnkRect.Width(), lnkRect.Height());
	}
	else {
		CRect	baseRect;
		GetDlgItem( IDC_ICSDISPCTRL10 )->GetWindowRect( &baseRect );
		ScreenToClient( baseRect );

		CRect	dspRect;
		GetDlgItem( IDC_DISPSLIP_CHK )->GetWindowRect( &dspRect );
		ScreenToClient( dspRect );
		GetDlgItem( IDC_DISPSLIP_CHK )->MoveWindow( baseRect.right+2, baseRect.top, 
															dspRect.Width(), dspRect.Height()  );
		CRect	lnkRect;
		GetDlgItem( IDC_DISPSLIP_CHK )->GetWindowRect( &lnkRect );
		ScreenToClient( lnkRect );
		GetDlgItem( IDC_LINKSLIP_BTN )->MoveWindow( baseRect.right + dspRect.Width() + 2, baseRect.top, 
															lnkRect.Width(), lnkRect.Height()  );

		GetDlgItem(IDC_GNPSCAN_BTN)->MoveWindow(baseRect.right + dspRect.Width() + 2, baseRect.top,
												lnkRect.Width(), lnkRect.Height());
	}

	m_DispSlipBtn.RedrawWindow();
	m_LinkSlipBtn.RedrawWindow();
	m_DispSlipChk.RedrawWindow();
	m_GnpScanBtn.RedrawWindow();
}

// スキャン時の ウィンドウ移動
//	bKeyScan	1: スキャン時
//	bOnSize		1: OnSize からの呼び出し	10: BlkMoveOnSizeJob 関数からの呼び出しで渡される場合がある
//
void CDBDinpView::ScanMode_WndMove( BOOL bKeyScan, BOOL bOnSize/*=FALSE*/ )
{
FILETrace( "ScanMode_WndMove %d, onsize = %d\n", bKeyScan, bOnSize );

	BlkWndMove( bKeyScan );

//	return;
/*---------------------------*/

	CRect rect, rc, view_rect, tmp;
	int	id[] = { 
		IDC_ICSDISPCTRL10, IDC_ICSDISPRDREMAIN, 
		IDC_ICSDISPCTRL12, IDC_ICSDISPGENKIN, IDC_ICSDISPCTRL11, IDC_ICSDISPSYOGT, 0
	};

	GetClientRect( view_rect );

	int x, y;
	
	BOOL bInpGnkinDevide = IsBlkInpGnkinDevide();


	if( bKeyScan ) {

		if( ! bInpGnkinDevide ) {
			GetDlgItem( IDC_MNTHSEL )->GetWindowRect( &tmp );
			ScreenToClient( tmp );

			y = tmp.top;

			for( int i = 0; id[i] != 0; i++ ) {
				if( i <= 1 ) {
					GetDlgItem( id[i] )->GetWindowRect( &rc );
					ScreenToClient( rc );

					GetDlgItem( id[i] )->MoveWindow( rc.left, y, rc.Width(), rc.Height() );
					if( i == 0 )	x = rc.left + rc.Width() + 1;
				}
				else {
					// 横に移動
					GetDlgItem( id[i] )->GetWindowRect( &rc );
					ScreenToClient( rc );

					GetDlgItem( id[i] )->MoveWindow( x, y, rc.Width(), rc.Height() );
					x += rc.Width();

				}
			}
		}

	}
	else {
		// 入力時
		GetDlgItem( IDC_STATIC_POS )->GetWindowRect( &rect );
		ScreenToClient( rect );
		GetDlgItem( IDC_STATIC_POS2 )->GetWindowRect( &tmp );
		ScreenToClient( tmp );

//FILETrace( "ScanMode_WndMove POS  (%d,%d,%d,%d)\n", rect.left, rect.top, rect.right, rect.bottom );
//FILETrace( "ScanMode_WndMove POS2 (%d,%d,%d,%d)\n", tmp.left, tmp.top, tmp.right, tmp.bottom );

		for( int i = 0; id[i] != 0; i++ ) {
			GetDlgItem( id[i] )->GetWindowRect( &rc );
			ScreenToClient( rc );
			if( i <= 1 ) {
				y = rect.top;
				GetDlgItem( id[i] )->MoveWindow( rc.left, y, rc.Width(), rc.Height() );
				if( i == 0 )	x = rc.left;
			}
			else {
				GetDlgItem( id[i] )->MoveWindow( x, tmp.top, rc.Width(), rc.Height() );
				x += rc.Width();
			}
		}
	}

	SlipWndMove( bKeyScan );

	m_DtList.EnforceLayout();

	if( ! bOnSize || bOnSize == 10 ) {
		// 表示可能な行数セット
		SCROLL_NL_ = m_DtList.GetDispDataCount();
		if( SCROLL_NL_ > DDATA_LNMAX )	SCROLL_NL_ = DDATA_LNMAX;

		// マルチウィンドウでリストが見えないくらい縦を縮めた。
		if( SCROLL_NL_ <= 0 )	SCROLL_NL_ = 7;
				
		// 決算確定マスター
		if( !IsMAsterKAKUTEI() ) {
			if( ! bKeyScan )	SCROLL_NL_ += 1;	// 追加入力行分
		}
	}

	int nShow = SW_SHOW;
	if( bKeyScan && m_InsLine == -1 )
		nShow = SW_HIDE;

	m_DtLine.ShowWindow( nShow );
//	m_DtLine.ShowWindow( bKeyScan ? SW_HIDE : SW_SHOW );
	
}




void CDBDinpView::OnSize(UINT nType, int cx, int cy)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	double w, h;
ONSIZETrace( ">>>>◆◆◆ CDBDinpView::OnSize() TOP %d, [%d, %d], PROGRAM_OK = %d, EDIT_PN(%d)\n", nType, cx, cy, PROGRAM_OK, EDIT_PN );

	// 未入力枝番チェック等 メッセージボックス表示時で 行数が変更されるのを防ぐ[06.13 /13 下より移動]
	if( ((CMainFrame*)GetDBDinpMainFrame())->MsgBoxOnSizeFlag() )
		return;
	if( ((CMainFrame*)GetDBDinpMainFrame())->IgnoreOnSizeFlag() )
		return;

	if( ((CMainFrame*)GetDBDinpMainFrame())->IsReCOSEL() )
		return;

	BOOL multiJob = TRUE;
	if (IsNotMultiWindow()) {
		multiJob = FALSE;
	}
	BOOL bZoom = ((CMainFrame*)GetDBDinpMainFrame())->IsZoomed();
ONSIZETrace("MulitType = %d, multiJob(%d), bZoom(%d)\n", GetMultiWindowType(), multiJob, bZoom);

	if (multiJob) {
		//マルチウィンドウ時のCMainFrame::OnActivate() で OnSize()が何回も呼ばれるので無視する
		if (((CMainFrame*)GetDBDinpMainFrame())->GetActivateFlag() == WA_INACTIVE) {
			return;
		}
		//電卓ウィンドウがOFFされた
		if( m_CalcOFF ) {
			ULONG timeCount = GetTickCount();

			ONSIZETrace("m_CalcOFF = %d, timeCount(%d), 経過時間(%d)\n", m_CalcOFF, timeCount, (timeCount - m_CalcOFF));

			if( (timeCount - m_CalcOFF) < 250 ) {
				return;
			}
			else {
				m_CalcOFF = 0;
			}
		}
	}

	CDBbaseView::OnSize(nType, cx, cy);
	
//	GetRate( w, h );	//ビューサイズ比率をゲット
	w = h = 1.0;
	set_viewrate( w, h );	//ビューサイズをAppにセーブ

	// タブクリックで画面切り替えがされたときはOnSize() 処理をしない
	if( ((CMainFrame*)GetDBDinpMainFrame())->TabClickChange() ) {
		((CMainFrame*)GetDBDinpMainFrame())->TabClickChange() = FALSE;
ONSIZETrace( ">>> CDBDinpView::OnSize() TabClickChange return\n" );
		if( !bZoom ) {
			return;
		}
	}
	// 未入力枝番チェック時で 行数が変更されるのを防ぐ
//	if( ((CMainFrame*)GetDBDinpMainFrame())->MsgBoxOnSizeFlag() )
//		return;

	BOOL bMove = FALSE;

	//タスクバークリックの最小化
	if( nType == SIZE_RESTORED && cx == 0 && cy == 0 )
		return;

	if( PROGRAM_OK == FALSE ) {
/*--FILETrace-------------------------------*/
		// 表示行数が変わった
		if( ((CMainFrame*)GetDBDinpMainFrame())->BeginAppFlag() ) {
			if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() == INP_VIEW ) {

				BlkMoveOnSizeJob(0, 1);

				int old_lncnt = SCROLL_NL();
				m_DtList.GetDispDataCount();
				int now_lncnt = m_DtList.GetDispDataCount();

				if( now_lncnt > 0 && old_lncnt > 0 ) {
					if( old_lncnt != now_lncnt ) {
						if( !m_bScanMode ) {
							PostMessage( WM_DBDINPMSG, DWP_LINECOUNT );
							_disp_change();
						}
					}
					else {
						DBeditOnSizeJob();
					}
				}
				RDRemainDispJOB();

ONSIZETrace( "@OnSize old_lncnt = %d, now_lncnt = %d, m_bScanMode = %d\n", old_lncnt, now_lncnt, m_bScanMode );
			}
		}
/*--FILETrace-------------------------------*/
		return;
	}

	try
	{
		if( ((CMainFrame*)GetDBDinpMainFrame())->NowView() == INP_VIEW ) {
			BlkMoveOnSizeJob(0, 1);

			int old_lncnt = SCROLL_NL();
			m_DtList.GetDispDataCount();
			int now_lncnt = m_DtList.GetDispDataCount();

ONSIZETrace( "@OnSize(2) old_lncnt = %d, now_lncnt = %d, bMove(%d)\n", old_lncnt, now_lncnt, bMove);

			if( now_lncnt > 0 && old_lncnt > 0 ) {
				if( old_lncnt != now_lncnt ) {
					if( ! m_bScanMode )	PostMessage( WM_DBDINPMSG, DWP_LINECOUNT );
				}
				else {
					DBeditOnSizeJob();
				}
			}
			//新規行の金額欄 killfocus無視(162412, 162366)
			if( get_nowpn() == VAL_PN && get_nowln() == APPEND_LINE && multiJob ) {
				EDIT_PN = -10;
			}
ONSIZETrace("@OnSize(4) EDIT_PN = %d\n",  EDIT_PN);
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}

ONSIZETrace( ">>>>◆◆◆ @OnSize END -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n" );

}



LRESULT CDBDinpView::OnSelGuideMsg(WPARAM w, LPARAM l )
{
#ifdef CLOSE	//マウスでアクティブにしないようにした。
	if( l == 1 ) {	//ダイアログの選択より
		int ln = get_nowln();
		int pn = get_nowpn();

		if( ln == APPEND_LINE ) {
			set_focus( pn );
		}
		else if( ln != -1 ) {
			set_focus( pn );
		}
		PostMessage(WM_SELGUIDEMSG, w, 0);
		return 1;
	}
#endif

	if( w == VK_NEXT || w == VK_PRIOR ) {
		keybd_event( w, 0, 0, 0 );
		keybd_event( w, 0, KEYEVENTF_KEYUP, 0 );
	}

	return 1;
}


void CDBDinpView::SelWndShow( BOOL bShow )
{
	BOOL bDisp = FALSE;

	if( bShow ) {
		if( m_pSelWnd == NULL )	bDisp = TRUE;

		if( pAUTOSEL->BLKSEL_OPT ) {
			MakeSelWnd( TRUE );
			if( bDisp ) {
				SelWndMoveJob(bShow);
				//作成直後は 常に表示
				m_SelDispPN = -1;
				disp_change( -1, get_nowpn() );
			}
		}
	}

	if (m_pSelWnd != NULL)
	{
		if( pAUTOSEL->BLKSEL_OPT ) {
			SelWndMoveJob( bShow );
		}
		else {
			if( m_pSelWnd->IsWindowVisible() ) {
				m_pSelWnd->ShowWindow( SW_HIDE );
			}
		}
	}
}

void CDBDinpView::SelWndMoveJob( BOOL bShow )
{
	UINT nFlags = SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER;

	if( bShow )	nFlags |= SWP_SHOWWINDOW;
	else		nFlags |= SWP_HIDEWINDOW;

	CRect rect, listRect;
	m_pSelWnd->GetWindowRect( &rect );
//	ScreenToClient( rect );
	m_DtList.GetWindowRect(&listRect);

	CRect	irect, syzItem;

	int ln = get_nowln();
	if( ln == APPEND_LINE ) {
		m_Input.GetWindowRect( &irect );
//		m_DtLine.GetItemScreenRect( 0, DATA_SEQ, irect );
	}
	else {
		m_Inp2.GetWindowRect( &irect );;
		m_DtList.GetItemScreenRect( ln, DATA_SYZ, syzItem );
	}
//	this->ScreenToClient( &irect );

	CSize size;
	size.SetSize(0,0);
	int syzflg = 0;

	if( get_nowpn() == SKBN_PN ) {
		syzflg = 1;
		if( m_pSyzInp->IsDisplay() ) {
			m_pSyzInp->GetCrntRect( size );

			m_pSyzInp->GetCrntWindowRect(irect);

#ifdef CLOSE
			if( pAUTOSEL->BLKSEL_OPT != OPT_NONOPT ) {
				if( ln == APPEND_LINE ) {
					int inpHt = irect.Height();

					CRect listRect;
					m_DtList.GetWindowRect(&listRect);
					if( m_DtList.GetLines() == 1 ) {
						irect.top = listRect.bottom - (inpHt * 2) - (inpHt / 2);
					}
					else {
						irect.top = listRect.bottom - (inpHt * 4) - (inpHt / 2);
					}
				}
			}
#endif
		}
	}
	else if( get_nowpn() == HJIYU_PN ) {
		if( m_pHjiyuInp->IsDisplay() ) {
			m_pHjiyuInp->GetCrntRect( size );
		}
	}
	else if( get_nowpn() == TKY_PN ) {
		if( m_pSubDtInp != NULL ) {
			if( m_pSubDtInp->IsDisplay() ) {
				m_pSubDtInp->GetCrntRect( size );
			}
		}
	}

//MyTrace("@SelWndMoveJob size  %d, %d\n", size.cx, size.cy);
//MyTrace("@SelWndMoveJob irect org  %d, %d, %d, %d\n", irect.left, irect.top, irect.right, irect.bottom);

	if( !syzflg ) {
		if( size.cy != 0 ) {
			if( (irect.top - size.cy) < 0 ) {
				irect.top = irect.bottom;
				irect.bottom = irect.top + size.cy;
			}
			else {
				irect.top = irect.top - size.cy;
			}
		}
	}

	//移動するか？
	int x, y;

	POINT pt = {irect.left, irect.top};
	POINT pt2 = {irect.right, irect.bottom};

//MyTrace( "@SelWndMoveJob MOVE (irect = %d, %d, %d, %d), (rect = %d, %d, %d, %d)\n",
//		irect.left, irect.top, irect.right, irect.bottom,	rect.left, rect.top, rect.right, rect.bottom );

	if( bShow ) {
		if( ! syzflg ) {
			rect.InflateRect(0, 1);
			if( (rect.PtInRect(pt) || rect.PtInRect(pt2)) ) {
				//	if( irect.top >= rect.top && irect.top <= rect.bottom ) {
				x = rect.left;

				CRect vwrc;
				GetWindowRect(vwrc);

				CRect newRect;
				newRect = rect;

				//下側
				y = vwrc.bottom - rect.Height();
				newRect.top = y;
				newRect.bottom = newRect.top + rect.Height();

				if( newRect.PtInRect(pt) || newRect.PtInRect(pt2) ) {
					//上側
					y = vwrc.top;
				}
			}
			else {
				x = rect.left;
				y = rect.top;
			}
		}
		else{
			//消費税ダイアログ 上側表示
			CRect chkRect;
			CRect vwrc;
			GetWindowRect(vwrc);

			chkRect = irect;
//MyTrace("@SelWndMoveJob MOVE (chkRect = %d, %d, %d, %d)\n", chkRect.left, chkRect.top, chkRect.right, chkRect.bottom);

			pt.x = chkRect.left;
			pt.y = chkRect.top;
			pt2.x = chkRect.left;
			pt2.y = chkRect.bottom;

			POINT pt3, pt4;

			pt3.x = chkRect.right;
			pt3.y = chkRect.top;
			pt4.x = chkRect.right;
			pt4.y = chkRect.bottom;

			x = rect.left;
			y = chkRect.top - rect.Height();

			if( rect.PtInRect(pt) || rect.PtInRect(pt2) || rect.PtInRect(pt3) || rect.PtInRect(pt4) ) {
				CRect newRect;
				newRect = rect;

				int n1 = chkRect.top - listRect.top;
				int n2 = listRect.bottom - chkRect.bottom;

				//上側の余白多い
				if( n1 > n2 ) {
					if( irect.top < chkRect.top ) {
						y = irect.top - rect.Height();
					}
					else {
						y = chkRect.top - rect.Height();
					}
				}
				else {	//下側の余白多い
					if( irect.bottom > chkRect.bottom ) {
						y = irect.bottom;
					}
					else {
						y = chkRect.bottom;
					}
				}
			}
			else {
				y = rect.top;
			}

			if( ln != APPEND_LINE ) {
				int y2 = y + rect.Height();

				if( y >= syzItem.top && y <= syzItem.bottom ) {
					y = syzItem.bottom;
				}
				else if( y2 >= syzItem.top && y2 <= syzItem.bottom ) {
					y = syzItem.top - rect.Height();
				}

				if( y <= 0 ) {
					y = syzItem.bottom;
				}
			}
		}
	}
	else {
		nFlags |= SWP_NOMOVE;
		x = y = 0;
	}

	//ダイアログは、リソース上で 無効(disalbed)で作成している。
	// Create直後に アクティブになってフォーカスが失われるため
	if( bShow )		m_pSelWnd->EnableWindow(TRUE);
	else			m_pSelWnd->EnableWindow(FALSE);

//MyTrace("@SelWndMoveJob SetWindowPos x = %d, y = %d\n", x, y);

	m_pSelWnd->SetWindowPos( &CWnd::wndBottom, x, y,0,0, nFlags );
}

//////////////////////////////////////////////////////////////////////////////////////
// 証憑番号

// フォーカス
#ifdef _ICSPEN_	// 01.20 /01
void CDBDinpView::FocusIcsDocEviNo( CWnd* pwnd, int ln, short click )
{
	_FocusIcsDocEviNo( pwnd, ln, click );

	OnICSInputFocus( DOCEVI_PN );
}
void CDBDinpView::_FocusIcsDocEviNo( CWnd* pwnd, int ln, short click )
{
#else
void CDBDinpView::FocusIcsDocEviNo( CWnd* pwnd, int ln, short click )
{
#endif

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
		//別画面入力消去
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}

		if( _dline_chg( pwnd, ln, DOCEVI_PN, click ? 0 : 1  ) == -1 )
			return;

		if( CheckAppendSansyo() )
			return;

		//当項目まで自動入力
		if( click ) {
			if( INP_mode == _APPEND && !is_AutoInp_use() )
				AutoInp( DOCEVI_PN );
		}

		// 取消データの場合はＳＥＱに
		if( isdeldata() )
		{
			SAVE_PN = DOCEVI_PN;
			set_focus( SEQ_PN );
			return;
		}
		else {
			if( data_denpdialog(ln) != 0 )
				return;
			SAVE_PN = -1;
		}

		// 証憑 自動入力
		int nextpn;
		if( INP_mode == _APPEND )
			if( getLINE_DATA()->LINE_CND[DOCEVI_PN].INP_sg == FALSE )
				if( (nextpn = getAUTOSELpn( get_nowln(), DOCEVI_PN )) != DOCEVI_PN )
				{
					set_nowpn( DOCEVI_PN );
					AutoInp( nextpn );
					set_focus( nextpn );
					return;
				}

		//直前のポジションが当ポジションより前で、借方科目が入力されていない場合は直前のポジションに返す。
		int set_pn;
		if( inp_ok( get_nowpn(), DOCEVI_PN, &set_pn ) == FALSE )
		{
			set_nowpn( set_pn );
			set_focus( /*get_nowpn()*/set_pn );
			return;
		}

		SelWndShow( FALSE );

		set_nowpn( DOCEVI_PN );
	
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

/***********************
	証憑番号入力メイン
************************/
void CDBDinpView::TerminationIcsinputDocEviNo( CWnd* pwnd, int ln, long nChar, long inplen, long kst )
{
CString ip_docevi;
char buf[30];
size_t length;
VARIANT var;

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
//		if( !nChar )	//フォーカスＯＦＦ
//		{
			exDBdata_get( DOCEVI_PN, getINP_CTL( pwnd, DOCEVI_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			if( var.pbVal )	length = strlen( (LPCTSTR)var.pbVal );
			else			length = 0;
			
			if( length > 0 ) {
				ip_docevi = var.pbVal;
			}
			else {
				ip_docevi.Empty();
			}

			if( !dinp_modefy_ok( getLINE_DATA() ) )
			{


				if( ip_docevi.Compare( getLINE_DATA()->_XXDTA.dd_docevi ) != 0 )
				{
					Buzzer();
				}

				if( ! getLINE_DATA()->_XXDTA.dd_docevi.IsEmpty() ) {
					strcpy_s( buf, sizeof buf, getLINE_DATA()->_XXDTA.dd_docevi );
				}
				else {
					::ZeroMemory( buf, sizeof buf );
				}
				var.pbVal = (BYTE*)buf;
				exDBdata_set( DOCEVI_PN, getINP_CTL( pwnd, DOCEVI_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			else
			{
				if( getLINE_DATA()->LINE_CND[DOCEVI_PN].INP_sg == FALSE && ip_docevi.IsEmpty() && ! inplen )
				{
					if( getLINE_DATA()->LINE_CND[DOCEVI_PN].NEXT_MOVE_KEY == FALSE )
						goto NEXT;
				}

				if( INP_mode == _APPEND && getLINE_DATA()->LINE_CND[DOCEVI_PN].INP_sg == FALSE )
				{
					// 証憑 自動入力
					if( ip_docevi.IsEmpty() ) {
						ip_docevi = SREC.m_skn;
						auto_docevi( ip_docevi, &SREC, pAUTOSEL );
					}
				}

				CREC.m_skn = ip_docevi;
				SREC.m_skn = ip_docevi;

				getLINE_DATA()->_XXDTA.dd_docevi = ip_docevi;
				getLINE_DATA()->LINE_CND[DOCEVI_PN].INP_sg = TRUE;

				if( ! getLINE_DATA()->_XXDTA.dd_docevi.IsEmpty() ) {
					strcpy_s( buf, sizeof buf, getLINE_DATA()->_XXDTA.dd_docevi );
				}
				else {
					::ZeroMemory( buf, sizeof buf );
				}
				var.pbVal = (BYTE*)buf;
				exDBdata_set( DOCEVI_PN, getINP_CTL( pwnd, DOCEVI_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				TerminationDataSet( DOCEVI_PN );

			}
//			return;
//		}

NEXT:
		getLINE_DATA()->LINE_CND[DOCEVI_PN].NEXT_MOVE_KEY = FALSE;

		if( !nChar ) {
			return;
		}

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return;

		if( nChar )
		{
			if( nChar == VK_UP || nChar == VK_DOWN ) {
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			nChar = dline_chg( pwnd, nChar );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合はＳＥＱに
				if( isdeldata() )
				{
					SAVE_PN = DOCEVI_PN;
					set_focus( SEQ_PN );
					return;
				}
				else
					SAVE_PN = -1;

				set_focus( DOCEVI_PN );
				return;
			}
		}

		if( dinp_modefy_ok( getLINE_DATA() ) && nChar == VK_DELETE )
		{
			ip_docevi.Empty();
			::ZeroMemory( buf, sizeof buf );
			var.pbVal = (BYTE*)buf;
			exDBdata_set( DOCEVI_PN, getINP_CTL( pwnd, DOCEVI_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

			CREC.m_skn = ip_docevi;
			SREC.m_skn = ip_docevi;

			getLINE_DATA()->_XXDTA.dd_docevi.Empty();
			getLINE_DATA()->LINE_CND[DOCEVI_PN].INP_sg = TRUE;

			TerminationDataSet( DOCEVI_PN );
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
		{
			getLINE_DATA()->LINE_CND[DOCEVI_PN].NEXT_MOVE_KEY = TRUE;

			if( !is_AutoInp_use() )
			{
				set_termfocus( get_nowln(), DOCEVI_PN, 1 );
			}
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 )
			set_termfocus( get_nowln(), DOCEVI_PN, -1 );
		else if( nChar == VK_DELETE )
			;
		else {
			if( nChar != 0x16 )
				Buzzer();
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


/*==========================
	証憑 番号取り出し
  ===========================*/
BOOL CDBDinpView::auto_docevi( CString& next, CDBINPDataRec *data, struct _AUTOSEL *mode )
{
	LPSTR p;

	switch( mode->DOCEVI_OPT )
	{
	case SNO_AUTO:			// 自動加算
		p = next.GetBuffer(128);
		DocEviAutoString( p, 128, Voln1->snocol );
		next.ReleaseBuffer();
		return TRUE;
		break;
	default: // 毎回入力
		break;
	}
	return FALSE;	// セットせず
}

// 枝番未入力時の自動表示
int	 CDBDinpView::BRNTKY_select( int bmn, CString& code, char* kana/* = NULL*/, BOOL bDisp/* = FALSE*/ )
{
	BOOL bMake = FALSE;
	
	if( m_pSelWnd == NULL )	{

		if( pAUTOSEL->BLKSEL_OPT ) {
			MakeSelWnd( TRUE );

			//作成直後は 常に表示
			m_SelDispPN = -1;
			disp_change( -1, get_nowpn() );

			bMake = TRUE;
		}
	}

	int st = CDBbaseView::BRNTKY_select( bmn, code, kana, bDisp );

//MyTrace( "BRNTKY_select = %d\n", st );

	if (m_pSelWnd != NULL)
	{
		if( pAUTOSEL->BLKSEL_OPT && st ) {
			SelWndMoveJob( TRUE );
		}
		else {
			if( m_pSelWnd->IsWindowVisible() ) {
				m_pSelWnd->ShowWindow( SW_HIDE );
			}
		}

//		if( bMake ) {
//			if( pAUTOSEL->BLKSEL_OPT ) {
//				if( st ) {
//					set_focus( get_nowpn() );
//				}
//			}
//		}
	}

	return st;
}

int CDBDinpView::pn_movecheck( int pn )
{
	if( get_nowln() == APPEND_LINE )
		return pn;

	int ln = m_DtList.GetDispDataCount();

	BOOL bEnable = GetDtListEnable( ln, pn );

	switch( pn ) {
		case DENP_PN:
			if( pAUTOSEL->DSPDENP_OPT == OPT_NODSP )
				bEnable = FALSE;
			break;
		case BMON_PN:
		case CBMN_PN:
			if( pAUTOSEL->DSPBMON_OPT == OPT_NODSP )
				bEnable = FALSE;
			break;
		case KOJI_PN:
		case CKJI_PN:
			if( pAUTOSEL->DSPKOJI_OPT == OPT_NODSP )
				bEnable = FALSE;
			break;

		case SYON_PN:
			if( pAUTOSEL->DSPTGNO_OPT == OPT_NODSP || pAUTOSEL->DSPLINE_OPT == 1 )
				bEnable = FALSE;
			break;

		case TGDT_PN:
			if( pAUTOSEL->DSPTGDT_OPT == OPT_NODSP || pAUTOSEL->DSPLINE_OPT == 1)
				bEnable = FALSE;
			break;

		case HJIYU_PN:
			if( IsHjiyuItemGyosyu() == 0 )	bEnable = FALSE;
			else {
				if( pAUTOSEL->DSPHJIYU_OPT == OPT_NODSP )
					bEnable = FALSE;
			}
			break;
		default:
			break;
	}

	if( ! bEnable )	pn = DATE_PN;

	return pn;
}

//
void CDBDinpView::dbedit_clear()
{
	VARIANT var;
	var.pbVal = NULL;
	int pn = get_nowpn();

	//　リスト行のDBEditをクリア
	DBdata_set( this, _getINP_CTL( 1, DATE_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	EDIT_PN = -1;
}


#include <DEPFinancialJournalDLL.h>


void CDBDinpView::OnMenuGencsv()
{
	CString	sFolder, sFName, wkPath;

	int type =  IsKojinGyosyuMaster( pDBzm );
	if( type ) {
		if( IsKojinKengyoMaster( type ) ) {
			myICSMessageBox( "個人兼業マスターは処理できません！" );
			return;
		}
	}

	//共有オプション
	int ret = 0;
	ret = Jz2CloudApi::SetCloudUseMasterSign(Jz2CloudApi::CLOUD_DEP, Voln1->apno, Voln1->v_cod, Voln1->ee_ymd );

	if( ret == -1 ) {
		CString errmsg;
		int   errcode;
		Jz2CloudApi::clouduperrormsg(errmsg, errcode);
		myICSMessageBox(errmsg);
		return;
	}

	LPSTR p = sFolder.GetBuffer(256);
	GetWorkDirFunc(p, 256);

	sFolder.ReleaseBuffer();

	sFName.Format( "gendb_rendo.csv" );

#ifdef _DEBUG
	sFolder	= "E:\\Temp\\work";
	sFName	= "gendb_rendo.csv";
#endif

	char pbuf[1024];
	if( PCOM_GetStringAll( "SELECTCODSN", pbuf, sizeof pbuf ) == -1 )
		return;

	((CMainFrame*)GetDBDinpMainFrame())->CurrentDirCheck();

#ifndef _DEBUG
	int st;
	st = DoFinancialJournalInputModal( this, sFolder, sFName );
#endif

	wkPath.Format( "%s\\%s", sFolder, sFName );

	CFileFind	fnd;
	BOOL bFind = fnd.FindFile( wkPath );


	if( bFind ) {
		fnd.Close();
		kyuyocsv_append( wkPath, TRUE );
		//ファイル消去
		_unlink( wkPath );
	}

	PCOM_SetStringAll( "SELECTCODSN", pbuf );

	//月情報再セット(減償の月設定が影響されるため)
	int sofs, eofs, sdate, edate;
	pDBzm->get_datelimit( sofs, eofs, sdate, edate );
	m_selDate.SetMonthOffset( sofs, eofs, sdate, edate );
}


void CDBDinpView::OnUpdateMenuGencsv(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	BOOL bEnable = ((CMainFrame*)GetDBDinpMainFrame())->GetGensyoDBsign();
	pCmdUI->Enable( bEnable && INP_mode == _APPEND );
}



// Wiz マスター連携解除
void CDBDinpView::OnMenuWiz()
{
	if( ! ((CMainFrame*)GetDBDinpMainFrame())->GetWizMasterSign() ) {
		return;
	}
	CString msg;
	msg.Format("%sとの連携を解除しますか？", WizTitle);
	int st = myICSMessageBox(msg, MB_YESNO|MB_DEFBUTTON2, 0, 0, this);

	if( st == IDYES ) {
		((CMainFrame*)GetDBDinpMainFrame())->SetWizMasterOFF();
		myICSMessageBox("請求書管理オプションの解除処理が正常に完了しました。", MB_OK, 0, 0, this);
	}
}


void CDBDinpView::OnUpdateMenuWiz(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	BOOL bEnable = ((CMainFrame*)GetDBDinpMainFrame())->GetWizMasterSign();
	pCmdUI->Enable(bEnable);
}



void CDBDinpView::SwkListSortSign( int data_id, int type )
{

	m_DtList.SetClickSortInfo( data_id, type );

	m_sortID	= data_id;
	m_sortType	= type;
}


void CDBDinpView::SwkListSortClear()
{
	m_sortID	= -1;
	m_sortType	= 0;
	m_DtList.ClickSortClear();
}


void CDBDinpView::GetSwkListSortSign( int& data_id, int& type )
{
	data_id = m_sortID;
	type	= m_sortType;
}


/*==========================
	挿入するSEQを取得する
  ===========================*/
int CDBDinpView::get_insseq()
{
	if( m_InsLine == -1 )
		return -1;

	return m_InsSeq;
}

/*==========================
	修正データの戻し
	ZRECに戻す
  ===========================*/
int CDBDinpView::CRECtoZREC()
{
	if( ! DATA_1RD )
		return 0;

	int ln, idx, n;
	ln = get_nowln();


	if( ln < 0 )	return 0;

	if( m_InsLine != -1 ) {
		if( ln > 1 )
			ln--;

		for( n = ln; n <= SCROLL_NL(); n++ ) {
			if( ln == m_InsLine )	continue;

			idx = DDATA_LINE[ n - 1 ].ZREC_NO - 1;

			if( idx > 0 ) {
				if( ZREC[idx].m_seq == CREC.m_seq ) {
					ZREC[idx] = CREC;
					break;
				}
			}
		}
	}
	else {
		idx = DDATA_LINE[ ln - 1 ].ZREC_NO - 1;
		if( idx >= 0 ) {
			ZREC[idx] = CREC;
		}
	}

	return 0;
}



// [仕訳挿入]検索時の仕訳表示ライン数の変更
int CDBDinpView::keyscan_insline_dispchg( int oldln, int nowln, int* pFocusSgn )
{
ASSERT( INP_mode != _RETREV );

	int ln, i, setln, pn;
	ln = get_nowln();
	pn = get_nowpn();
	setln = -1;

#ifdef CLOSE
	//修正データの戻し
	if( DATA_1RD )
	{
	//	memmove( (char *)&ZREC[DDATA_LINE[ get_nowln() - 1 ].ZREC_NO - 1], (char *)&CREC, sizeof( struct _DATAREC ) );
	//	ZREC[DDATA_LINE[ ln - 1 ].ZREC_NO - 1] = CREC;
		CRECtoZREC();

		if( INP_mode == _KEYSCAN )
		{
			if( m_pScanView->scan_zrec_set( &CREC, oldln - ln ) == -1 )
				return -1;
		}
	}
#endif

	for( i = 0; i < oldln; i++ ) {
		if( DDATA_LINE[i].ZREC_NO )
			break;
	}
	// リストにデータなし
	if( i == oldln )	return 0;

	// 新規行を挿入なので 既入力仕訳リスト表示行数は減る
	if( DATA_1RD )
	{
		//CREC to 残高ADD.
		rewupdat( &CREC, &SCAN_rec, INP_mode );
		DATA_1RD = 0;
//		PROGRAM_OK = FALSE;
	}

	int sht, idx, insLine = -1;
	int dcnt = 0;

	for( i = 1; i <= oldln; i++ )
	{
		idx = DDATA_LINE[i-1].ZREC_NO - 1;

		if( idx >= 0 ) {
			if( m_InsSeq == ZREC[idx].m_seq ) {
				insLine = i;
			}
			dcnt++;
		}
	}

	m_InsSREC.Reset();
	if( insLine != -1 ) {
		int lnpos = 0;
		if( insLine > 1 )	lnpos = insLine-2; //1行目以外は上の行
		idx = DDATA_LINE[ lnpos ].ZREC_NO - 1;
		if( idx >= 0 )	m_InsSREC = ZREC[idx];
		else {
			//一番上に挿入
			idx = DDATA_LINE[ insLine-1 ].ZREC_NO - 1;
			if( idx >= 0 )	m_InsSREC = ZREC[idx];
		}
	}

	// 読み取るデータ数
	int rd_cnt = nowln - 1;

	if( insLine == oldln ) {		// 最下行の時
		sht		= 0;
	}
	else if( insLine == 1 || ! DDATA_LINE[insLine-2].ZREC_NO ) {	// 先頭行 に挿入
		if( (dcnt + 1) > nowln ) {	// 底のデータが変わる
			sht = oldln - (nowln - 1);
		}
		else {
			sht = 0;		// 底のデータは変わらない
		}
	}
	else {
		// 挿入行が表示行の減少でどうなるか？
		if( insLine >= nowln ) {	//底データは変えない。
			sht = 0;
		}
		else {
			sht = oldln - nowln + 1;
		}
	}

	sht *= -1;

	//データディスプレイニシャライズ
	for( i = 0 ; i < DDATA_LNMAX; ++i )
	{
		inpline_inz( &DDATA_LINE[i] );
	}

	//データリード＆テーブル作成
	for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
		ZREC[i].Reset();
	}
	SCROLL_NL_ = nowln;

	m_pScanView->scan_data_rd( &ZREC[ SCROLL_NL() - 1 ], rd_cnt, sht );

	scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );

	int lcnt, end_pn;

	for( i = 1; i <= SCROLL_NL(); i++ )
	{
		idx = DDATA_LINE[i-1].ZREC_NO - 1;

		if( idx >= 0 ) {
			if( m_InsSeq == ZREC[idx].m_seq ) {
				m_InsLine = i-1;
				break;
			}
		}
	}

	lcnt = nowln;
	end_pn = lcnt - 1;	//スクロール終了 DDATA_LINE ポジション

	ln = m_InsLine;
	int ins = 0;

	struct _DATA_LINE tmpLine, saveLine;

	for( i = end_pn ; i >= 1; i-- )
	{
		if( i == ln ) {
			ins = 1;
			tmpLine = DDATA_LINE[i-1];
			inpline_inz( &DDATA_LINE[i-1] );
		}
		else {
			if( ins ) {
				saveLine = DDATA_LINE[i-1];
				DDATA_LINE[i-1] = tmpLine;
				tmpLine = saveLine;
			}
		}
	}

	//データテーブル表示
	_ddata_dsp( -1 );

	if( !IsMAsterKAKUTEI() )	//新規入力[挿入]分追加
		SCROLL_NL_ += 1;

	return 0;
}



int CDBDinpView::KeyscanInsertJob()
{
	int old_lncnt = m_DtList.GetDispDataCount();

	ScanMode_WndMove( TRUE );

	int now_lncnt = m_DtList.GetDispDataCount();
	if( now_lncnt > DDATA_LNMAX )	now_lncnt = DDATA_LNMAX;

	int focus_sgn = 0;
	if( old_lncnt != now_lncnt ) {
		if( keyscan_insline_dispchg( old_lncnt, now_lncnt, &focus_sgn ) != 0 ) {
			ErrExit( 0, "仕訳表示行の変更ができませんでした。" );
		}
	}

	_set_focus( APPEND_LINE, DATE_PN );

	return 0;
}



// [仕訳挿入]修正行から、挿入モードに変更する場合。
int CDBDinpView::modefy_insline_dispchg()
{
	int lcnt, i;
	int idx, insLine = -1;
	int dcnt = 0;

	lcnt = SCROLL_NL();

	for( i = 1; i <= lcnt; i++ )
	{
		idx = DDATA_LINE[i-1].ZREC_NO - 1;

		if( idx >= 0 ) {
			if( m_InsSeq == ZREC[idx].m_seq ) {
				insLine = i;
			}
			dcnt++;
		}
	}

	m_InsSREC.Reset();
	if( insLine != -1 ) {
		int lnpos = 0;
		if( insLine > 1 )	lnpos = insLine-2; //1行目以外は上の行
		idx = DDATA_LINE[ lnpos ].ZREC_NO - 1;
		if( idx >= 0 )	m_InsSREC = ZREC[idx];
		else {
			//一番上に挿入
			idx = DDATA_LINE[ insLine-1 ].ZREC_NO - 1;
			if( idx >= 0 )	m_InsSREC = ZREC[idx];
		}
	}

	if( DATA_1RD )
	{
		//CREC to 残高ADD.
		rewupdat( &CREC, &SCAN_rec, INP_mode );
		DATA_1RD = 0;
//		PROGRAM_OK = FALSE;
	}

	struct _DATA_LINE	saveLine;

	if( insLine == 1 || ! DDATA_LINE[insLine-2].ZREC_NO ) {	// 先頭行 に挿入
		if( (dcnt + 1) > lcnt ) {	// 底のデータが変わる
			for( i = lcnt; i >= insLine; i-- )
			{
				if( i == insLine ) {
					inpline_inz( &DDATA_LINE[i-1] );
				}
				else {
					if( (i-2) >= 0 ) {
						DDATA_LINE[i-1] = DDATA_LINE[i-2];		// n-1 行目→ n 行目へ
					}
				}
			}
		}
		else {
			// 底のデータは変わらない
			insLine--;
		}
	}
	else if( insLine == lcnt || insLine == lcnt-1 ) {		// 底データはそのまま。
		insLine--;

		for( i = 1; i <= insLine; i++ )
		{
			if( i == insLine ) {
				inpline_inz( &DDATA_LINE[i-1] );
			}
			else {
				saveLine		= DDATA_LINE[i];		// 2行目→ 1行目へ
				DDATA_LINE[i-1] = saveLine;
			}
		}
	}
	else {
		// 挿入行が表示行の減少でどうなるか？
		for( i = lcnt; i >= insLine; i-- )
		{
			if( i == insLine ) {
				inpline_inz( &DDATA_LINE[i-1] );
			}
			else {
				if( (i-2) >= 0 ) {
					DDATA_LINE[i-1] = DDATA_LINE[i-2];		// n-1 行目→ n 行目へ
				}
			}
		}
	}

	m_InsLine = insLine;

	ddata_dsp(-1);
	_set_focus( APPEND_LINE, DATE_PN );

	return 0;
}




// 挿入モードから 修正モードに戻す
void CDBDinpView::ins_back_modefy()
{	
	// 確定マスターは、仕訳挿入モードにならない。
	if( bCONFIRM_MASTER )	return;

	int mode, i, n;
	mode = _MODEFY;

	//
	int zrec_idx = -1 /*= DDATA_LINE[ SCROLL_NL()-1 ].ZREC_NO - 1*/;
	int seq = -1;
	int sv_insseq = m_InsSeq;
	seq = DDATA_LINE[ SCROLL_NL()-1 ]._XXDTA.dd_seq;

//	if( zrec_idx >= 0 ) {
//		seq = ZREC[zrec_idx].m_seq;
//	}
//TRACE( "zrec_idx = %d, sv_insseq = %d, seq = %d\n", zrec_idx, sv_insseq, seq );

//MyTrace( "ins_back_modefy seq = %d\n", seq );

	//ZREC ReSet
	for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
		ZREC[i].Reset();
	}

	//仕訳表示用
	pDBzm->dbdata_Requery() = TRUE;

	CDBINPDataRec key;

	n = scaninsdat_getn( &ZREC[0], seq, SCROLL_NL() );
	scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );
	D_SEQN = SCROLL_NL()-1;

	int set_ln = -1;

	for( int ln = 1; ln <= SCROLL_NL(); ln++ ) {
		zrec_idx = DDATA_LINE[ ln-1 ].ZREC_NO - 1;

		if( zrec_idx >= 0 ) {
			if( ZREC[zrec_idx].m_seq == sv_insseq ) {
				CREC = ZREC[zrec_idx];
				set_ln = ln;
				break;
			}
		}
	}

	// 空データをOCX にセット( kill focus にデータを取得するので)
	VARIANT var;
	var.pbVal = NULL;
	m_Input.SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
	// killfocus 無視
	EDIT_PN = -1;
TRACE("ins_back_modefy = %d\n", EDIT_PN );

	inpmdisp( mode );

	set_nowln( set_ln );
	ddata_dsp(-1);	

	//SREC+消費税
	reupdat(&CREC);
	DATA_1RD = 1;

	set_focus( DATE_PN );
}


//挿入モードクリア
void CDBDinpView::insert_clear()
{	
	m_InsLine = -1;
	m_InsMode = _APPEND;

	m_InsSeq = -1;
	m_InsSREC.Reset();

	m_InsRecArray.RemoveAll();

	((CMainFrame*)GetDBDinpMainFrame())->StatusGuideDisp(AFX_IDS_IDLEMESSAGE);
}

//	挿入レコードの保存
void CDBDinpView::set_insrec(CDBINPDataRec* data, int bseq )
{
	if( m_InsLine == -1 )
		return;

	m_InsRecArray.Add( *data );

	//挿入分のSEQを追加
	if( m_InsMode == _KEYSCAN ) {
		SCAN_PGTBL.ins_data++;
		SCAN_PGTBL.insSeq.Add( data->m_seq );

		insert_keyscanseq( data->m_seq, bseq );
	}
}


//	挿入時の仕訳リスト表示
void CDBDinpView::disp_insswklist( int oldln, int nowln )
{
	int old_lncnt, now_lncnt;

	if( oldln == 0 || nowln == 0 ) {
		old_lncnt = SCROLL_NL();
		now_lncnt = m_DtList.GetDispDataCount();

		if( now_lncnt > DDATA_LNMAX )
			now_lncnt = DDATA_LNMAX;
	}
	else {
		old_lncnt = oldln;
		now_lncnt = nowln;
	}

//MyTrace( "@disp_insswklist old = %d, now = %d\n", old_lncnt, now_lncnt );

	int keyseq = -1;
	int src_idx;
	int n, i, idx;

	if( now_lncnt >= old_lncnt ) {
		// 底データの変更はしない。
		src_idx = (old_lncnt - 1);

		n = DDATA_LINE[src_idx].ZREC_NO - 1;

		if( n >= 0 )	keyseq = ZREC[n].m_seq;
	}
	else {	//表示行数が減った。
		if( now_lncnt < (m_InsLine+1) ) {
			src_idx = (m_InsLine + 1) - 1;
		}
		else {
			src_idx = (now_lncnt - 1);
		}

		n = DDATA_LINE[src_idx].ZREC_NO - 1;

		if( n >= 0 )	keyseq = ZREC[n].m_seq;
	}

	//データディスプレイニシャライズ
	for( i = 0 ; i < DDATA_LNMAX; ++i )
	{
		inpline_inz( &DDATA_LINE[i] );
	}
	//データリード＆テーブル作成
	for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
		ZREC[i].Reset();
	}

	scaninsdat_getn( &ZREC[0], keyseq, now_lncnt );
	scandat_dspn( &ZREC[0], DDATA_LINE, now_lncnt );

	// 挿入行を検索
	for( i = 1; i <= now_lncnt; i++ )
	{
		idx = DDATA_LINE[i-1].ZREC_NO - 1;

		if( idx >= 0 ) {
			if( m_InsSeq == ZREC[idx].m_seq ) {
				m_InsLine = i-1;
				break;
			}
		}
	}

	int ln = m_InsLine;
	int ins = 0;
	struct _DATA_LINE tmpLine, saveLine;

	for( i = now_lncnt ; i >= 1; i-- )
	{
		if( i == ln ) {
			ins = 1;
			tmpLine = DDATA_LINE[i-1];
			inpline_inz( &DDATA_LINE[i-1] );
		}
		else {
			if( ins ) {
				saveLine = DDATA_LINE[i-1];
				DDATA_LINE[i-1] = tmpLine;
				tmpLine = saveLine;
			}
		}
	}

	SCROLL_NL_ = now_lncnt;
	if( !IsMAsterKAKUTEI() )	SCROLL_NL_ += 1;	// 新規入力分

	_ddata_dsp(-1);
}



// スキャン時の仕訳挿入モードの時の行が増えた場合等の再表示
void CDBDinpView::disp_keyscan_insswklist( int oldln, int nowln )
{
	int old_lncnt, now_lncnt;

	if( oldln == 0 || nowln == 0 ) {
		old_lncnt = SCROLL_NL();
		now_lncnt = m_DtList.GetDispDataCount();

		if( now_lncnt > DDATA_LNMAX )
			now_lncnt = DDATA_LNMAX;
	}
	else {
		old_lncnt = oldln;
		now_lncnt = nowln;
	}
	int keyseq = -1;
	int src_idx;
	int n, i, idx;

	if( now_lncnt >= old_lncnt ) {
		// 底データの変更はしない。
		src_idx = (old_lncnt - 1);

		n = DDATA_LINE[src_idx].ZREC_NO - 1;

		if( n >= 0 )	keyseq = ZREC[n].m_seq;
	}
	else {	//表示行数が減った。
	//	if( now_lncnt < (m_InsLine+1) ) {
			src_idx = (m_InsLine + 1) - 1;
	//	}
	//	else {
	//		src_idx = (now_lncnt - 1);
	//	}

		n = DDATA_LINE[src_idx].ZREC_NO - 1;

		if( n >= 0 )	keyseq = ZREC[n].m_seq;
	}

	//データディスプレイニシャライズ
	for( i = 0 ; i < DDATA_LNMAX; ++i )
	{
		inpline_inz( &DDATA_LINE[i] );
	}
	//データリード＆テーブル作成
	for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
		ZREC[i].Reset();
	}

//	int nl = SCROLL_NL();
	int nl = now_lncnt;
	CDBINPDataRec key;
	key.m_seq = keyseq;

	// 追加仕訳なら 挿入SEQ
	int cnt = m_InsRecArray.GetCount();
	for( int n = 0; n < cnt; n++ ) {
		if( keyseq == m_InsRecArray[n].m_seq ) {
			key.m_seq = m_InsSeq;
			break;
		}
	}

	data_keyscan_back( &key, &ZREC[nl-1], nl );
	scandat_dspn( &ZREC[0], DDATA_LINE, nl );

	// 挿入行を検索
	for( i = 1; i <= now_lncnt; i++ )
	{
		idx = DDATA_LINE[i-1].ZREC_NO - 1;

		if( idx >= 0 ) {

			if( m_InsSeq == ZREC[idx].m_seq ) {
				m_InsLine = i-1;
				break;
			}
		}
	}

	// 挿入行より 上行には、挿入分を表示する。
	cnt = m_InsRecArray.GetCount();

	// 挿入仕訳分 ZREC を調整する
	int lidx = now_lncnt-1;
	int last_zrec, ins_zrec;
	last_zrec = ins_zrec = 0;

	lidx = m_InsLine-1;
	if( lidx >= 0 ) {
		ins_zrec = DDATA_LINE[lidx].ZREC_NO;
	}
	CDBINPDataRec tmpRec, setRec;
	int sidx;

	sidx	= ins_zrec - 1;

	if( sidx >= 0 ) {
		int i, src, prev, zct, start_pos;

		for( i = 0; i < cnt; i++ ) {
			//ZREC はリングバッファ
			for( src = (sidx-i), zct = 0; ; src--, zct++ ) {

				if( src < 0 ) {
					src = (RECMAX - 1) - src;
				}
				if( zct == 0 ) {
					//初回のみ
					start_pos = src;
					setRec = ZREC[src];
				}
				else {
					//終了判定
					if( src == start_pos )
						break;
				}
				prev = (src - 1);

				if( prev >= 0 ) {
					tmpRec = ZREC[ prev ];
				}
				else {
					prev = (RECMAX - 1);
					tmpRec = ZREC[ prev ];
				}

				ZREC[prev] = setRec;

				if( tmpRec.m_seq < 0 )
					break;

				if( prev == (RECMAX - 1) ) {
					src = RECMAX /*-1*/;	// ループの src-- 分
				}

				setRec = tmpRec;
			}
		}

		int ins_idx;

		ins_idx = sidx;

		//挿入仕訳の追加
		for( int i = cnt-1; i >= 0; i-- ) {

			ZREC[ins_idx] = m_InsRecArray[i];

			ins_idx--;
			if( ins_idx < 0 )	ins_idx = (RECMAX - 1);

			//終了判定
			if( i < (cnt-1) ) {
				if( sidx == ins_idx )	break;
			}
		}
	}

	last_zrec = -1;
	int z_idx = -1;

	for( int ln = now_lncnt; ln >= 1; ln-- ) {
		if( ln == m_InsLine ) {
			inpline_inz( &DDATA_LINE[ln-1] );
		}
		else if( ln > m_InsLine ) {
			last_zrec = DDATA_LINE[ln-1].ZREC_NO;
		}
		else {
			if( z_idx == -1 ) {
				z_idx = last_zrec - 1;
			}
			z_idx--;

			if( z_idx < 0 )	z_idx = RECMAX - 1;

			if( ZREC[z_idx].m_seq < 0 ) {

				inpline_inz( &DDATA_LINE[ln-1] );
			}
			else {
				recdsp( &ZREC[0], z_idx, &DDATA_LINE[ln-1], 0 );
			}
		}
	}

	SCROLL_NL_ = now_lncnt;
	if( !IsMAsterKAKUTEI() )	SCROLL_NL_ += 1;	// 新規入力分

	_ddata_dsp(-1);

}


//
void CDBDinpView::DBeditOnSizeJob()
{
	if( INP_mode == _APPEND ) {

		int pn = get_nowpn();

		CRect rect;
		int x, y, wd, ht;

		m_DtLine.GetItemScreenRect( 1, pn, rect );
		ScreenToClient(&rect);

		x = rect.left;
		y = rect.top;

		wd = rect.right - x;
		ht = rect.bottom - y;

		m_Input.MoveWindow(x, y, wd, ht);
	}
}


//
void CDBDinpView::BmnIMEkeydownJob( long nChar )
{
	HIMC	hImc;
	char	buf[128] = {0};
	char	yomi[128] = {0};

	int pn = get_nowpn();
	UINT ID = 0;
	
	if( pn == BMON_PN || pn == CBMN_PN ) {
		ID = getINP_CTL( pn )->IDC_X;
	}

	if( ID == 0 )
		return;

	UINT vKey = 0;

	if( nChar == VK_PROCESSKEY ) {
		HWND hwnd = GetDlgItem(ID)->m_hWnd;
		vKey = ImmGetVirtualKey(hwnd);
		hImc = ImmGetContext(hwnd);

		ImmGetCompositionString( hImc, GCS_COMPSTR, buf, sizeof buf );
		ImmGetCompositionString( hImc, GCS_COMPREADSTR, yomi, sizeof yomi );

		if( buf[0] && yomi[0] ) {
			if( isdigitstr( (char*)(LPCTSTR)buf ) ) {
				if( vKey == VK_UP || vKey == VK_DOWN ) {
					ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
					OnImeEndCompositionIcsdbedtctrl( ID, vKey, buf, yomi );
				}
			}
		}
		ImmReleaseContext(hwnd, hImc);
		hImc = NULL;
	}
}


BOOL CDBDinpView::IsKeyScanMode()
{
	BOOL bRet = FALSE;

	if( INP_mode == _KEYSCAN ) {
		bRet = TRUE;
	}
	
	return bRet;
}


// 定型仕訳に登録する
void CDBDinpView::Rdadd_job( int line )
{
	// 追加数を ５個へ
	INT_PTR max;
	max = gSelArray.GetCount();
	CRdregDlg dlg( this );
	dlg.m_Type = 0;

	int cline;
	int zrec_idx = -1;

	Rddata_Check();

	if( line == APPEND_LINE ) {
		struct _DATA_LINE* dline; 
		dline = _getLINE_DATA( line );

		if( dline->LINE_CND[DEBT_PN].INP_sg == FALSE &&
			dline->LINE_CND[CRED_PN].INP_sg == FALSE ) {
			myICSMessageBox( "借方・貸方の科目が未入力のため、定型仕訳に登録できません！" );
			return;
		}

		dlg.m_RegData[0] = CREC;
		dlg.m_recCnt	= 1;
		dlg.m_CmnId		= m_CmnId;

		PROGRAM_OK = FALSE;

		dlg.DoModal();
		RdselCancelJob();

		PROGRAM_OK = TRUE;
		set_focus( get_nowpn() );

		return;
	}
	else {
		cline = line;
		zrec_idx = DDATA_LINE[ cline-1 ].ZREC_NO - 1;
	}

	zrec_idx = DDATA_LINE[ cline-1 ].ZREC_NO - 1;

	if( max >= 1 ) {
		if( max > RDADD_MAX ) {
			RegDataTorokMsg();

			if( zrec_idx >= 0 ) {
				int i, cnt, curidx = -1;
				for( i = cnt = 0; i < max; i++ ) {
					if( gSelArray[i] == ZREC[zrec_idx].m_seq ) {
						curidx = i;
						break;
					}
				}

				if( curidx < 0 )	return;

				CDWordArray	da;
				da.Add( gSelArray[i] );
				cnt++;

				if( i > RDADD_MAX ) {
					i--;
					for( ; i >= 0; i-- ) {
						da.Add( gSelArray[i] );
						cnt++;
						if( cnt == RDADD_MAX )	break;
					}

					if( cnt < RDADD_MAX ) {
						i = curidx + 1;
						for( ; i < max; i++ ) {
							da.Add( gSelArray[i] );
							cnt++;
							if( cnt == RDADD_MAX )	break;
						}
					}
				}
				else {
					i++;
					for( ; i < max; i++ ) {
						da.Add( gSelArray[i] );
						cnt++;
						if( cnt == RDADD_MAX )	break;
					}

					if( cnt < RDADD_MAX ) {
						i = curidx - 1;
						for( ; i >= 0; i-- ) {
							da.Add( gSelArray[i] );
							cnt++;
							if( cnt == RDADD_MAX )	break;
						}
					}
				}
				gSelArray.RemoveAll();
				gSelArray.Copy( da );
			}
			qsort_s( &gSelArray[0], gSelArray.GetCount(), sizeof DWORD, SelSeqCompare, pDBzm );
			max = RDADD_MAX;
			gSelUpdate = TRUE;
			sel_lineupdate();
		}
		else if( max > 1 ) {
			qsort_s( &gSelArray[0], gSelArray.GetCount(), sizeof DWORD, SelSeqCompare, pDBzm );
		}

		for( int i = 0; i < max; i++ ) {
			pDBzm->DBDATA_SetPosition( gSelArray[i] );
			pDBzm->GetCDBData( &dlg.m_RegData[i] );
		}
		dlg.m_recCnt	= max;
		dlg.m_CmnId		= m_CmnId;

		PROGRAM_OK = FALSE;

		dlg.DoModal();
		RdselCancelJob();

		PROGRAM_OK = TRUE;
		set_focus( get_nowpn() );
	}
	else {
		if( zrec_idx >= 0 ) {
			dlg.m_RegData[0] = ZREC[zrec_idx];
			dlg.m_recCnt	= 1;
			dlg.m_CmnId		= m_CmnId;

			PROGRAM_OK = FALSE;

			dlg.DoModal();
			RdselCancelJob();

			PROGRAM_OK = TRUE;
			set_focus( get_nowpn() );
		}
	}
}



//--------------------------------------------
//		定型仕訳のデータチェック
//
//--------------------------------------------
int CDBDinpView::Rddata_Check()
{

	if( ! m_bRGDTA_BUTTON_DISP ) {
		//テーブル作成
		if( Mkrd.IsExistMkrdTable( FALSE, Voln1->apno ) ) {

			CString filter;
			filter = _T("rl_owner = 0");	// マスターで共有
			Mkrd.label->Requery( filter );

			if( Mkrd.label->GetRecordCount() == 0 ) {
				m_CmnId = 0;

				if( Mkrd.rdtbl_copy( m_CmnId, FALSE ) == ERR ) {
					ermset( -1, (char*)Mkrd.GetMkrdErrMsg() );
					return -1;
				}
				set_rddatasyz();

				if( IsMasterType( MST_INSERT|MST_SYZ5_8|MST_SYZ8 ) ) {
				//	if( Mkrd.check_H26syz( pDBsy ) != 0 ) {
				//		ermset( 0, "定型仕訳の消費税チェックでエラーが発生しました。");
				//		return -1;
				//	}

					// 上の関数で、バッファとして使用しているため元に戻す。
					pDBzm->dbdata->MoveLast();
				}
				m_bRGDTA_BUTTON_DISP = TRUE;

				Mkrd.label->Requery( filter );

				if( Mkrd.label->GetRecordCount() == 0 )
					m_bRGDTA_BUTTON_DISP = FALSE;
				else {
					if( IsMasterType( MST_INSERT|MST_SYZ5_8|MST_SYZ8 ) ) {
					//	if( Mkrd.check_H26syz( pDBsy ) != 0 ) {
					//		ermset( 0, "定型仕訳の消費税チェックでエラーが発生しました。");
					//		return -1;
					//	}

						// 上の関数で、バッファとして使用しているため元に戻す。
						pDBzm->dbdata->MoveLast();
					}
					m_bRGDTA_BUTTON_DISP = TRUE;
				}
			}
		}
	}
	return 0;
}



// 新規作成時 消費税サインをセット
void CDBDinpView::set_rddatasyz()
{
	CString filter;

	filter = _T( "rd_owner = 0" );

	Mkrd.data->Requery( filter );
	Mkrd.data->MoveFirst();

	CDBSyohi* pSy;
	//消費税
	if( (pSy = new CDBSyohi()) == NULL ) {
		ermset( ERROR_MEMORY, "消費税オブジェクトの作成に失敗しました！" );
		return;
	}

	if( pSy->SyOpen( pDBzm ) == -1 ) {
		WORD	sy_errno;
		char	sy_ermsg[256];
		pSy->Get_sy_error( &sy_errno, sy_ermsg );
		ermset( sy_errno, sy_ermsg );
		delete pSy;
		return;
	}


	CDBDATA	dbdata;

	if( Mkrd.data->st != -1 ) {
		do {
			dbdata.seq = Mkrd.data->seq;
			dbdata.dbt.Empty();
			dbdata.cre.Empty();
			if( dbdata.dsign.GetSize() < SZ_DSGN ) {
				dbdata.dsign.SetSize( SZ_DSGN );
			}
			::ZeroMemory( &dbdata.dsign[0], SZ_DSGN );

			dbdata.val.Empty();
			dbdata.zei.Empty();
			dbdata.taika.Empty();
			// 一旦 科目未入力の データで SyIniz しないと 追加データとして、判定しない。
			pSy->SyIniz( &dbdata );

			dbdata_set( &dbdata, pSy, TRUE );
			Mkrd.data->dsign.Copy( dbdata.dsign );
			Mkrd.data->Correct();

		} while( Mkrd.data->MoveNext() == 0 );
	}

	if( pSy != NULL ){
		delete pSy;
	}
}



// 消費税モジュールのSyIniz 呼び出しため CDBDATA にデータをセット
void CDBDinpView::dbdata_set( CDBDATA* dbdata, CDBSyohi* pSy, BOOL syinit )
{

	dbdata->seq = Mkrd.data->seq;
	dbdata->ksign = 0;
	dbdata->ddate = pDBzm->zvol->ss_ymd;
	dbdata->mofs = 0;
	dbdata->cno = -1;
	dbdata->dbmn	= Mkrd.data->dbmn;
	dbdata->dkno	= Mkrd.data->dkno;
	dbdata->dbt	= Mkrd.data->dbt;
	dbdata->dbr	= Mkrd.data->dbr;
	dbdata->cbmn	= Mkrd.data->cbmn;
	dbdata->ckno	= Mkrd.data->ckno;
	dbdata->cre	= Mkrd.data->cre;
	dbdata->cbr	= Mkrd.data->cbr;
	dbdata->val	= Mkrd.data->val;
	if( dbdata->val.IsEmpty() )	dbdata->val = "0";
	dbdata->zei	= Mkrd.data->zei;
	dbdata->dsign.Copy( Mkrd.data->dsign );
	dbdata->owner	= -1;
	dbdata->dtype	= -1;
	dbdata->tekiyo	= Mkrd.data->tekiyo;
	dbdata->imgsq	= -1;
	dbdata->snumber	= Mkrd.data->snumber;
	dbdata->tdate	= Mkrd.data->tdate;
	dbdata->edate	= -1;
	dbdata->udate	= -1;

	dbdata->hjiyu	= Mkrd.data->hjiyu;
	dbdata->taika	= Mkrd.data->taika;

	if( syinit ) {
		pSy->SyIniz( dbdata );
	}
}


// コピー仕訳のチェック
BOOL CDBDinpView::check_cpydata( CDBINPDataRec* data, DWORD& syCode )
{
	BOOL bSyzSET = FALSE;

	if ( ! IsMasterType( MST_SYZ8_10| MST_SYZ10 )) {
		syCode = CD_RITU8;
		//消費税対象＋10％(8軽)＋課税  → 8%へ
		if ((data->m_dsign[0] & 0x40) && ((data->m_dsign[1] & 0x0f) == 0x05 || (data->m_dsign[1] & 0x0f) == 0x04) &&
			(data->m_dsign[2] & 0x0f) == 0x01) {
			bSyzSET = TRUE;
		}
		else {
			int dbt_sy, cre_sy;
			dbt_sy = pDBzm->km_syattr( data->m_dbt );
			cre_sy = pDBzm->km_syattr( data->m_cre );

			if ((dbt_sy & 0x0200) || (cre_sy & 0x0200)) {
				if ((data->m_dsign[1] & 0x0f) == 0x05 || (data->m_dsign[1] & 0x0f) == 0x04) {
					bSyzSET = TRUE;
				}
			}
		}
	}
	if( ! IsMasterType( MST_SYZ5_8|MST_SYZ8|MST_SYZ8_10 | MST_SYZ10 ) ) {
		syCode = CD_RITU5;
		//消費税対象＋８％,10%,軽減＋課税
		if( (data->m_dsign[0]&0x40) && (data->m_dsign[1]&0x0f) >= 0x03 &&
			(data->m_dsign[2]&0x0f) == 0x01 ) {
			bSyzSET = TRUE;
		}
		else {
			int dbt_sy, cre_sy;
			dbt_sy = pDBzm->km_syattr( data->m_dbt );
			cre_sy = pDBzm->km_syattr( data->m_cre );
			//仮受・仮払
			if( (dbt_sy & 0x0200) || (cre_sy & 0x0200) ) {

				if( (data->m_dsign[1]&0x0f) >= 0x03 ) {
					bSyzSET = TRUE;
				}
			}
		}
	}

	if( pDBsy->IsKaniMaster() ) {
		if( data->m_ddate < KANI6_YMD ) {
			//簡易課税６種 対象外？
			if( (data->m_dsign[0]&0x40) && (data->m_dsign[2]&0x0f) == 0x01 &&
				(data->m_dsign[3]&0x0f) == 0x06 ) {

				BYTE defKbn = KaniDefaultKubun( Voln1 );
				//サインOFF
				data->m_dsign[3] &= ~0x0f;
				data->m_dsign[3] |=  defKbn;	//基本業種セット
			}
		}
	}
	//リバースチャージ
	if( data->m_ddate < RVCHAR_YMD ) {
		//簡易課税６種 対象外？
		if( (data->m_dsign[0]&0x40) && (data->m_dsign[2]&0x0f) == 0x01 &&
			((data->m_dsign[4]) == 0x1d ||(data->m_dsign[4]) == 0x1e) ) {

			//サインOFF
			data->m_dsign[4] = 0;

			int dbt_kbn, cre_kbn;
			dbt_kbn = pDBzm->km_zeikbn( data->m_dbt );
			cre_kbn = pDBzm->km_zeikbn( data->m_cre );

			if( dbt_kbn < 0 && cre_kbn < 0 ) {
				::ZeroMemory( &data->m_dsign[0], SZ_SYZDSGN );
			}
			else {
				if( dbt_kbn > 0 || cre_kbn > 0 ) {
					syCode = CD_SOTOZ;
				}
				else {
					syCode = CD_UTIZ;
				}
				bSyzSET = TRUE;
			}
		}
	}
	return bSyzSET;
}



// チェックコピー仕訳データ
void CDBDinpView::CheckCopySwkData( CDBINPDataRec* pcpy )
{
	// 一旦 科目なし+SEQ 変更 で消費税 イニシャライズ
	CDBINPDataRec	data;

	data.Reset();
	data.m_seq		= pcpy->m_seq+1;
	data.m_ddate	= pcpy->m_ddate;
	data.m_mofs		= pcpy->m_mofs;
	data.m_ksign	= pcpy->m_ksign;
	DB_SyIniz( &data );

	BOOL bSyzSET = FALSE;
	DWORD syCode;

	data = *pcpy;

	if( ! bSyzSET ) {
		bSyzSET = check_cpydata( &data, syCode );
	}

	*pcpy = data;

	// 消費税サインをセット
	DB_SyIniz( pcpy );

	if( bSyzSET ) {
		if( pDBsy->SyFnc( syCode ) == 0 ) {
			struct _SY_MSG_PACK2 smpk;
			pDBsy->SyMsg( &smpk );
			pDBzm->GetCDBData( pcpy );
		}
	}
}

BOOL CDBDinpView::OnPreparePrinting(CPrintInfo* pInfo)
{
//MyTrace( "@CDBDinpView::OnPreparePrinting TOP m_bPreview = %d\n", pInfo->m_bPreview );
	if (m_Vprn.InitPrint(2, pInfo, &m_Pset, this) != 0) {
//MyTrace( "@OnPreparePrinting InitPrint m_Vprn.m_ApErrCod = %d", m_Vprn.m_ApErrCod );

		if(m_Vprn.m_ApErrCod == 2) {
			ICSExit(0, "出力処理エラーの為、プログラムを終了します。");
		}
		return	FALSE;
	}

	int		st =	DoPreparePrinting(pInfo);

	m_End_PrintDlg = 0;
	if ( st == FALSE ) {
		m_End_PrintDlg = m_Vprn.End_PrintDlg(this);
		if( m_End_PrintDlg < 1 ) {	// 返送値＝１の場合、印刷プレビュー処理へ
			// 印刷ダイアログにおいて、エラー 又は キャンセル押下時
			if (m_ImgPrintFlg == PRINT_VERIFY ) {	// 一括検証
				m_pImgmng->ImgPrintResetDlg();
			}
			else {
				m_pImgmng->ImgEndPrintData();
			}
			m_ImgPrintFlg = PRINT_NONE;
			set_focus( get_nowpn() );
		}
	}
	return st;
}


void CDBDinpView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo )
{
//MyTrace( "@CDBDinpView::OnEndPrinting TOP\n");
	m_Vprn.EndPrint();

	if (m_ImgPrintFlg) {
		if (m_Vprn.PreviewOnPrintdlg())
			return;
		m_pImgmng->ImgPrintResetDlg();
		m_ImgPrintFlg = PRINT_NONE;
		set_focus( get_nowpn() );
	}
}


void CDBDinpView::OnPrint(CDC* pDC, CPrintInfo*)
{
	// TODO: コントロールを印刷するコードを追加してください。
	if ( m_Vprn.StartPageJob(pDC) != 0 )	return;
//	if ( m_Vprn.SetCoMode(1) )				return;	// 座標モード

	if(m_ImgPrintFlg == PRINT_VERIFY){
		int rt = 	m_pImgmng->ImgPrintVerify(&m_Vprn);
		if(rt == 1)	m_Vprn.LastPage();
	}
	else if(m_ImgPrintFlg == PRINT_DOCIMG){
		CDocPInfoArray Doc_pArry;	// 出力情報Array
		//
		// ImgPrintVerifyは最終ページの時1として返送値を返します。
		int rt = m_pImgmng-> ImgPrintData (&m_Vprn, &Doc_pArry);
		if(rt == 1)	m_Vprn.LastPage();
	}

	m_Vprn.EndPageJob(pDC);
}


//データ追加(原票も含めて)
void CDBDinpView::DATAandDOC_AppendOperation( int pn )
{
#ifdef _SLIP_ACCOUNTS_
	if( IsDispSlip() ){
		if( (m_ImgDispOption.autoLkType==ID_AUTOLINK_ON) && (m_SlipBtnState==ID_LINKON_BSTATE) ){
			CArray<int> tmpDocseqAry;
			if( m_pImgmng->GetLinkDocseqOnDlg(tmpDocseqAry) != -1 ){
				CREC.m_dsign[6] |= 0x01;
				m_ContDocseqAry.RemoveAll();
				m_ContDocseqAry.Copy( tmpDocseqAry );
			}
		}
	}
#endif
	DB_AppendOperation( this, pn );

#ifdef _SLIP_ACCOUNTS_
	if( IsDispSlip() ){
		// 関連対象有り？ ('12.11.01)
		int cnt = (int)m_ContDocseqAry.GetCount();
		if( cnt ){
			m_pImgmng->ConnectDataToImgFile( pDBzm, REAL_SEQ_NO, m_ContDocseqAry );
			m_ContDocseqAry.RemoveAll();
			if( m_ImgDispOption.nextpgType == ID_AUTO_NEXTPAGE ){
				m_pImgmng->DispImgDlgUnlinkNextPage();
			}
		}
		else{
			if( m_ImgDispOption.nextpgType == ID_AUTO_NEXTPAGE ){
				m_pImgmng->DispImgDlgNextPage();
			}
		}

		if( m_ImgDispOption.nextpgType == ID_AUTO_NEXTPAGE ){
			ChgLinkBtnState( ID_LINKON_BSTATE );
		}
		else{
			ChgLinkBtnState( ID_LINKON_BSTATE );
		}
		// 摘要にフォーカスが移動してしまう為の対処
		set_nowpn( DATE_PN );
		set_focus( DATE_PN );
	}
#endif
}



// 挿入モードから、検索修正に戻す。
void CDBDinpView::ins_back_keyscan()
{
	// 確定マスターは、仕訳挿入モードにならない。
	if( bCONFIRM_MASTER )	return;

	int mode, i;
	mode = _KEYSCAN;

	PROGRAM_OK = FALSE;

	//
	int zrec_idx = -1;
	int btm_seq = -1;
	int sv_insseq = m_InsSeq;

	int idx = SCROLL_NL()-1 - 1; //挿入追加行分 マイナス
	btm_seq = DDATA_LINE[idx ]._XXDTA.dd_seq;

	m_pScanView->set_insdata_toscan( m_InsSeq, btm_seq );

	m_pScanView->set_scan_bottom( btm_seq );

	ScanMode_WndMove( TRUE );

	//データディスプレイニシャライズ
	for( i = 0 ; i < DDATA_LNMAX ; ++i )
	{
		inpline_inz( &DDATA_LINE[i] );
	}
	m_DtList.RemoveAllDtListData();


	for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
		ZREC[i].Reset();
	}

	//仕訳表示用
	int sft = m_pScanView->scan_data_rd( &ZREC[ SCROLL_NL() - 1 ], SCROLL_NL(), 0 );
	scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );

	D_SEQN = SCROLL_NL()-1;

	int set_ln = -1;

	for( int ln = 1; ln <= SCROLL_NL(); ln++ ) {
		zrec_idx = DDATA_LINE[ ln-1 ].ZREC_NO - 1;

		if( zrec_idx >= 0 ) {
			if( ZREC[zrec_idx].m_seq == sv_insseq ) {
				CREC = ZREC[zrec_idx];
				set_ln = ln;
				break;
			}
		}
	}

	PROGRAM_OK = TRUE;

	// 空データをOCX にセット( kill focus にデータを取得するので)
	VARIANT var;
	var.pbVal = NULL;
	m_Input.SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
	// killfocus 無視
	EDIT_PN = -1;
TRACE("ins_back_keyscan = %d\n", EDIT_PN );

	inpmdisp( mode );


	set_nowln( set_ln );
	ddata_dsp(-1);	

	//SREC+消費税
	reupdat(&CREC);
	DATA_1RD = 1;
	//貸借合計を再計算する
	m_TaiTtl.init_taiclq();

	set_focus( DATE_PN );
}



int CDBDinpView::get_insrec( int seq, CDBINPDataRec* pdata )
{
	int ret = 0;

	for( int n = 0; n < m_InsRecArray.GetCount(); n++ ) {
		if( seq == m_InsRecArray[n].m_seq ) {
			*pdata = m_InsRecArray[n];
			ret++;
			break;
		}
	}
	return ret;
}


BOOL CDBDinpView::IsImgDlgChanged( int setDtseq )
{
	BOOL	change = FALSE;
	int prev_cnt = m_SRECdseqAry.GetCount();
	CArray<int>	prevArray;
	prevArray.Copy( m_SRECdseqAry );

FILETrace( "  IsImgDlgChanged set_Dtseq %d, LINE = %d\n", setDtseq, __LINE__ );

	// 一度 _APPEND に移行した場合や、関連づいている仕訳でない場合。
	if( m_ImgDtseq == -1 )	return TRUE;

	m_SRECdseqAry.RemoveAll();
	m_pImgmng->GetRelImgAry(setDtseq, m_SRECdseqAry);

	int now_cnt = m_SRECdseqAry.GetCount();

FILETrace( "  IsImgDlgChanged prev = %d, now =  = %d, LINE = %d\n", prev_cnt, now_cnt, __LINE__ );

	if( prev_cnt != now_cnt ) 
		change = TRUE;
	else {
		if( now_cnt > 0 )
			qsort_s( &m_SRECdseqAry[0], now_cnt, sizeof(int), SeqCompare, this );

		for( int n = 0; n < now_cnt; n++ ) {
			if( m_SRECdseqAry[n] != prevArray[n] ) {
				change = TRUE;
				break;
			}
		}
	}

FILETrace( "  IsImgDlgChanged END change = %d LINE = %d\n", change, __LINE__ );

	return change;
}


BOOL CDBDinpView::IsDenpyoDataLine( int ln )
{
	if( ln == APPEND_LINE )	return 0;

	int zrec_idx;
	zrec_idx = DDATA_LINE[ ln-1 ].ZREC_NO - 1;
	if( zrec_idx < 0 )	return 0;

	if( DenpTypeMode() && IsDenpyoSiwake( & ZREC[ zrec_idx ] ) ) {
		return TRUE;
	}

	return FALSE;
}

int CDBDinpView::data_denpdialog( int ln )
{
	m_DenpSeq = m_DenpICNO = 0;

	if( ln == APPEND_LINE )	return 0;

	int zrec_idx;
	zrec_idx = DDATA_LINE[ ln-1 ].ZREC_NO - 1;
	if( zrec_idx < 0 )	return 0;

	int icno = ZREC[ zrec_idx ].m_icno;

	if( DenpTypeMode() && IsDenpyoSiwake( & ZREC[ zrec_idx ] ) ) {
		//新規入力から上矢印キーでの移動は、SEQへ
		if( m_bAppendUpkey ) {
			m_bAppendUpkey = FALSE;
			_set_focus( ln, SEQ_PN );
			return 1;
		}

		if( IsDispSlip() ){
			// 関連対象有り？
			int cnt = (int)m_ContDocseqAry.GetCount();
//MyTrace( "関連付け 個数=%d\n", cnt );
			if( cnt ){
				CREC.m_dsign[6] |= 0x01;
				dataupd( &CREC, &SCAN_rec, INP_mode, 0, this );
				m_pImgmng->ConnectDataToImgFile( pDBzm, CREC.m_seq, m_ContDocseqAry );
				m_ContDocseqAry.RemoveAll();
				m_pImgmng->DeleteAllChildImgDlg();
			}
			else {
				CArray<int>		docseqAry;
				m_pImgmng->GetRelImgAry( CREC.m_seq, docseqAry);
				if( docseqAry.GetCount() == 0 ) {
					CREC.m_dsign[6] &= ~0x01;
				}
			}

			ChgLinkBtnState( ID_DISPSLIP_BSTATE );
			// 表示更新
			TurnOffDispImgDlg();
			// 表示状態変更
			ChangeImgdlgType( ID_DBDINP_NODISPDLG_TYPE );
			m_ImgDtseq = -1;
			if( m_DispSlipChk.GetCheck() ) {
				m_DispSlipChk.SetCheck(0);
			}
		}
		m_DenpSeq	= ZREC[ zrec_idx ].m_seq;
		m_DenpICNO	= ZREC[ zrec_idx ].m_icno;

//		DATA_1RD = 0;
		SendMessage( WM_DBDINPMSG, DWP_DENPDIALOG );

		return 1;
	}

	return 0;
}

//
// 伝票形式で修正後、仕訳表示の更新( スキャン以外 ）
//
//
int CDBDinpView::denpend_swkdisp( int ln )
{
ASSERT( INP_mode != _RETREV );

	int i, index;
	CDBINPDataRec key;

	if( (ln - 1) < 0 )	return 0;

	index = -1;

	if( DDATA_LINE[ln - 1].ZREC_NO ) {
		index = DDATA_LINE[ln - 1].ZREC_NO - 1;
	}
	if( index == -1 )	return 0;

	// 現在 表示しているデータからからセット
	key.m_seq = ZREC[index].m_seq;
	key.m_ddate = ZREC[index].m_ddate;

	//データディスプレイニシャライズ
	for( i = 0 ; i < DDATA_LNMAX; ++i )
	{
		inpline_inz( &DDATA_LINE[i] );
	}
	//データリード＆テーブル作成
	for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
		ZREC[i].Reset();
	}

	int seq = key.m_seq;
	if( IsMAsterKAKUTEI() )	seq = 0;
	else {
		//入力行 追加ＳＥＱ分 更新
		INP_LINE._XXDTA.dd_seq = pDBzm->m_lastdata.m_seq;
		SaveDinpData_SEQ( _APPEND, INP_LINE._XXDTA.dd_seq );
		inpline_dsp( &INP_LINE, APPEND_LINE);
	}

	pDBzm->dbdata_Requery() = TRUE;
	int n = endkeyscan_getn(&ZREC[0], &key, SCROLL_NL(), ln, seq );
	scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );
	_ddata_dsp( -1 );

	//修正データ読み出し
	SetCRECfromDPend( &::CREC );

	return 0;
}


int CDBDinpView::SetCRECfromDPend( CDBINPDataRec *pCREC )
{
	int zrec_idx = -1;
	if( pCREC->m_seq == 0 )	return 0;

	for( int n = 0; n < DDATA_LNMAX; n++ ) {
		zrec_idx = DDATA_LINE[ n ].ZREC_NO - 1;
		if( zrec_idx < 0 )	continue;
		if( ZREC[ zrec_idx ].m_seq == pCREC->m_seq ) {
			*pCREC = ZREC[ zrec_idx ];
			reupdat(&CREC);
			DATA_1RD = 1;
			break;
		}
	}
	return 0;
}


//
// 伝票形式で修正後、仕訳表示の更新( スキャン以外 ）
//
//
int CDBDinpView::denpend_keyscandisp( int ln )
{
ASSERT( INP_mode != _RETREV );

	int i, index;
	CDBINPDataRec key;

	if( (ln - 1) < 0 )	return 0;

	int btm_seq, seq;
	int btm_line = SCROLL_NL();
	index = -1;

	//底のデータ
	if( DDATA_LINE[btm_line - 1].ZREC_NO ) {
		index = DDATA_LINE[btm_line - 1].ZREC_NO - 1;
	}
	if( index != -1 ) {
		// 現在 表示しているデータからからセット
		btm_seq = ZREC[index].m_seq;
	}

	if( ! m_pScanView->denpend_keyscan_search( btm_seq ) ) {
		index = -1;
		if( DDATA_LINE[ln - 1].ZREC_NO ) {
			index = DDATA_LINE[ln - 1].ZREC_NO - 1;
		}
		if( index == -1 )	return 0;
		seq = ZREC[index].m_seq;
		m_pScanView->denpend_keyscan_search( seq );
	}

	//データディスプレイニシャライズ
	for( i = 0 ; i < DDATA_LNMAX; ++i )
	{
		inpline_inz( &DDATA_LINE[i] );
	}
	//データリード＆テーブル作成
	for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
		ZREC[i].Reset();
	}

	m_pScanView->scan_data_rd( &ZREC[ SCROLL_NL() - 1 ], SCROLL_NL(), 0 );
	scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );

	//データテーブル表示
	_ddata_dsp( -1 );

	//検索仕訳数が少なくなった場合への対処 内税で検索→　外税に変更(変更した分が検索から消える)
	int lnidx = get_nowln() - 1;
	if( lnidx >= 0 ) {
		int recidx = DDATA_LINE[lnidx].ZREC_NO - 1;
		if( recidx >= 0 ) {
			if( CREC.m_seq != ZREC[recidx].m_seq ) {
				CREC = ZREC[recidx];
			}
		}
	}
	//修正データ読み出し
	SetCRECfromDPend( &::CREC );

	return 0;
}


// 伝票形式での修正時は SEQ_PN にセットする。
int CDBDinpView::GetNowLineSetPN( int ln/*=-1*/ )
{
	if( ! DenpTypeMode() )	return DATE_PN;

	int index, line;
	CDBINPDataRec key;

	if( ln == -1 )	{
		line = get_nowln();
		if( line == APPEND_LINE )	return DATE_PN;
	}
	else {
		line = ln;
	}

	if( (line - 1) < 0 )	return SEQ_PN;
	index = -1;

	if( DDATA_LINE[line - 1].ZREC_NO ) {
		index = DDATA_LINE[line - 1].ZREC_NO - 1;
	}
	if( index == -1 )	return SEQ_PN;

	if( ZREC[index].m_icno > 0 && ZREC[index].m_dtype ) {
		return SEQ_PN;
	}
	return DATE_PN;
}


BOOL CDBDinpView::ChangeZeiritsu()
{
	BOOL ret = FALSE;

	if (!IsMasterType( MST_SYZ8_10 | MST_SYZ10 )) {
		return FALSE;
	}
	DWORD	dwCode = 0;

	if (!dinp_modefy_ok( getLINE_DATA() ))
		return FALSE;

	char ip_val[6] = { 0 };

	int ln = get_nowln();
	TerminationIcsinputVal( this, ln, 0, 0, 0 );

	switch( CREC.m_dsign[1]&0x0f ) {
	case 4:		dwCode = CD_RITU10;		break;	//8軽 → 10%
	case 5:		dwCode = CD_RD_RITU8;	break;	//10% → 8軽
	}

	if (dwCode != 0) {
		if (pDBsy->SyFnc( dwCode ) == 0) {
			struct _SY_MSG_PACK2 smpk;
			pDBsy->SyMsg( &smpk );
			pDBzm->GetCDBData( &CREC );

			getLINE_DATA()->_XXDTA.dd_syz_str = sy_line_txt( &CREC );
			memmove( getLINE_DATA()->_XXDTA.dd_zei, CREC.m_zei, 6 );
			int pntbl[] = {
				SKBN_PN, ZEI_PN
			};

			TerminationDataSet( pntbl, 2 );
			ret = TRUE;
		}
	}
	return ret;
}


// スキャン時に仕訳挿入したときのリストの表示
void CDBDinpView::disp_keyscan_insswkcopy()
{
	//挿入した仕訳をZRECにセット
	int rcnt = m_InsRecArray.GetCount();

	if (++D_SEQN >= RECMAX) D_SEQN = 0;
	ZREC[D_SEQN] = m_InsRecArray[rcnt-1];
	datadsp(D_SEQN, 1, m_InsLine);
	ddata_dsp(-1);
}

//-------------------------------------------------
//	参照モード時の、新規追加入力防止チェック
//返送値：
//		参照モードで、メッセージを出した
//-------------------------------------------------
BOOL CDBDinpView::CheckAppendSansyo()
{
	BOOL ret = FALSE;
	if( INP_mode == _APPEND ) {
		if( IsJzSansyo() ) {
			CString msg, str;
			GetConfirmString(str);
			msg.Format("%s\r\n\r\n仕訳の入力・修正をすることはできません！", str );
			myICSMessageBox(msg, MB_OK,0,0,this);
			set_focus(DATE_PN);
			ret = TRUE;
		}
	}
	
	return ret;
}


// 原票スキャンボタン
void CDBDinpView::OnBnClickedGnpScanBtn()
{
	//原票読取に変更する。
	if( m_bSLACC_BUTTON_DISP ) {
		if( IsDispSlip() ) {
			HideImgDlg();
		}

		CMainFrame* pMain;
		pMain = ((CMainFrame*)GetDBDinpMainFrame());

		if( INP_mode == _MODEFY ) {
			if( DATA_1RD )
			{
				//CREC to 残高ADD.
				int sg = rewupdat(&CREC, &SCAN_rec, INP_mode, FALSE, FALSE);
			//	DATA_1RD = 0;
			}
		}

		if( IsJzSansyo() || FromReadOCR() )	return;

		//経理上手(電子帳簿) か 原票会計購入ユーザ
		if( !pMain->IsKeiriJzEBEnv() ) {
			if( !IsBuyScannerSave() ) {
				return;
			}
		}

		pMain->CurrentDirCheck();

		//設定を保存
		set_inpmode(pAUTOSEL);
		int pn = get_nowpn();

		GNPSIWAKEDLG_PAR	spar;

		spar.pzmsub = pDBzm;
		spar.pdocsub = pDCsb;
		spar.psyohi = pDBsy;

		spar.parent = this;
		spar.pApp = &theApp;

		pDBzm->minfo->MoveFirst();

		spar.s_ofst = -1;
		spar.e_ofst = -1;

		spar.s_ymd = -1;
		spar.e_ymd = -1;

		spar.inpymd = CREC.m_ddate;
		spar.inpofs = CREC.m_mofs;
		spar.inpksgn = CREC.m_ksign;

		spar.inputseq = INP_LINE._XXDTA.dd_seq;
		spar.ins_seq = get_insseq();
		spar.mnthfix = m_bMnthFix ? 1 : 0;

		if( pDBzm->minfo->st != -1 ) {
			do {
				if( pDBzm->minfo->insw & 0x01 ) {
					if( spar.s_ofst == -1 ) {
						spar.s_ofst = pDBzm->minfo->ofset;
						spar.s_ymd = pDBzm->minfo->smd;
					}
					spar.e_ofst = pDBzm->minfo->ofset;
					spar.e_ymd = pDBzm->minfo->emd;
				}
				else {
					break;
				}
			} while( pDBzm->minfo->MoveNext() == 0 );
		}
//MyTrace("GenpyoScanAndMakeSiwakeDialog call\n");
		PROGRAM_OK = FALSE;
		pMain->PopupCloseState(2);
		pMain->ToolBarClear();
		pMain->m_bGenpyoScan = TRUE;
		m_bForkUCOM = FALSE;

		//設定初期レコード作成
		OwnInitSetup para{ 0 };
		para.own = pDCsb->owntb;
		para.user_id = -1;
		ICSMCTL	mctrl;
		mctl_usertbl	utbl;
		if( mctrl.SetDatabase() == 0 ) {
			mctrl.get_usertbl(utbl);
			para.user_id = utbl.user_id;
		}
		OwntbInitSetupGenpyo(&para);
		OwntbInitSetupCard(&para);
		OwntbInitSetupBank(&para);

		int st = GenpyoScanAndMakeSiwakeDialog(&spar);

		pMain->PopupCloseState(0);
		PROGRAM_OK = TRUE;
		m_bForkUCOM = TRUE;
		pMain->m_bGenpyoScan = FALSE;

//MyTrace("GenpyoScanAndMakeSiwakeDialog st = %d\n", st);

		if( st == -1 ) {
			ICSExit(0, "原票表示ダイアログでエラーが発生しました。");
		}
		else {
			if( m_pImgmng != NULL ) {
				IMG_INITREC	initrec;
				memset(&initrec, '\0', sizeof(IMG_INITREC));
				initrec.pDBZmSub = pDBzm;
				m_pImgmng->ReInit(initrec);
			}

			if( st > 0 ) {
				CString filter;
				filter = _T("seq = 1");
				pDBzm->dcntl->Requery(filter);

				if( pDBzm->dcntl->st != -1 ) {
					pDBzm->insseq_make() = FALSE;
					DBDinpAppendParamReload(pDBzm->m_dacnt, pDBzm->dcntl->cnt);
					pDBzm->m_dacnt = pDBzm->dcntl->cnt;
				}

				if( INP_mode == _APPEND ) {
					if( m_InsLine != -1 ) {
						if( m_InsMode == _MODEFY )	disp_insswklist(0, 0);
						else						disp_keyscan_insswklist(0, 0);

						INP_LINE._XXDTA.dd_seq = REAL_SEQ_NO + 1;	// 他で追加があるため SEQ更新
						CREC.m_seq = INP_LINE._XXDTA.dd_seq;
						//入力行のSEQ更新
						inpline_update();
					}
					else {
						//ZREC ReSet
						int i, n;
						for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
							ZREC[i].Reset();
						}
						//仕訳表示用
						pDBzm->dbdata_Requery() = TRUE;

						CDBINPDataRec key;
						key.m_seq = APPEND_SEQ.ngetseq(SEQ_NO - 1);
						if( key.m_seq <= 0 ) key.m_seq = USERS_SEQ;
						n = scaninsdat_getn(&ZREC[0], -1, SCROLL_NL());

						scandat_dspn(&ZREC[0], DDATA_LINE, SCROLL_NL());
						ddata_dsp(-1);
						D_SEQN = SCROLL_NL() - 1;

						INP_LINE._XXDTA.dd_seq = REAL_SEQ_NO + 1;	// 他で追加があるため SEQ更新
						CREC.m_seq = INP_LINE._XXDTA.dd_seq;

						// 08.18 /11
						if( SREC.m_seq < key.m_seq )
							SREC.m_seq = key.m_seq;
						if( NXTDTA_sg ) {
							if( NXT_rec2.m_seq != -1 && (NXT_rec2.m_seq < key.m_seq) )
								NXT_rec2.m_seq = key.m_seq;
						}
						// 08.19 /09
						// 直前のサンプル仕訳 SREC の修正
						set_SREC(&SREC, &ZREC[0], DDATA_LINE, SCROLL_NL(), &INP_LINE, _APPEND);
#ifdef IMPORT	// 10.14 /98
						if( NXTDTA_sg )
							set_SREC(&NXT_rec2, &ZREC[0], DDATA_LINE, SCROLL_NL(), &INP_LINE, _APPEND);
#endif

						//入力行のSEQ更新
						inpline_update();
					}
				}
				else {
					INP_LINE._XXDTA.dd_seq = REAL_SEQ_NO + 1;
					SaveDinpData_SEQ(_APPEND, INP_LINE._XXDTA.dd_seq);
					//入力行のSEQ更新
					inpline_update();
				}
				pDBzm->kzrec_Requery() = TRUE;
				/* 諸口合計	*/
				g_stotal(SYOGT, (char *)SYOG_d, (char *)SYOG_c, (char *)SG_def);
				syogdsp((char *)SG_def);
				/* 現金合計	*/
				g_stotal( /*GNKIN*/pAUTOSEL->DCKM_CODE, (char *)GENK_d, (char *)GENK_c, (char *)GK_def);
				genkdsp((char *)GK_def);
			}
		}
		//修正行 消費税再iniz
		DB_SyIniz(&CREC);

		pDCsb->sheet_type->Requery("");
		((CMainFrame*)GetDBDinpMainFrame())->set_DocShtypCombo();

		set_focus(pn);

#ifdef CLOSE //認識有はexe 経由
		CString str_prog, str_command;
		int		wMode, oMode;

		str_prog.Format("%C:\\ICSWin\\Cmds\\GR1C\\JOBS31\\GnpScanApp.exe", ::GET_PROGRAMDEVICE());
		str_command = "";

		//		wMode = SW_SHOWNORMAL;
		wMode = SW_MAXIMIZE;

		CWnd* pwnd = GetDBDinpMainFrame();
		if( pwnd->IsZoomed() )	oMode = SW_MAXIMIZE;
		else
		{
			oMode = SW_SHOWNORMAL;
		}

		//		if( ::ICSChain(m_hWnd, str_prog, str_command, wMode) == 0 ) {
		if( ::ICSFork8_Ex(pwnd->m_hWnd, str_prog, str_command, wMode, oMode) == 0 ) {
		}
		else {
			ICSMessageBox("原票スキャン起動に失敗しました！");
		}
#endif

	}
}


//原票表示入力
void CDBDinpView::GnpSiwakeDlgJob()
{
	GNPSIWAKEDLG_PAR	spar;
	spar.pzmsub = pDBzm;
	spar.pdocsub = pDCsb;
	spar.psyohi = pDBsy;

	spar.parent = this;
	spar.pApp = &theApp;

	pDBzm->minfo->MoveFirst();

	spar.s_ofst = -1;
	spar.e_ofst = -1;

	spar.s_ymd = -1;
	spar.e_ymd = -1;

	spar.inpymd = CREC.m_ddate;
	spar.inpofs = CREC.m_mofs;
	spar.inpksgn = CREC.m_ksign;

	if( pDBzm->minfo->st != -1 ) {
		do {
			if( pDBzm->minfo->insw & 0x01 ) {
				if( spar.s_ofst == -1 ) {
					spar.s_ofst = pDBzm->minfo->ofset;
					spar.s_ymd = pDBzm->minfo->smd;
				}
				spar.e_ofst = pDBzm->minfo->ofset;
				spar.e_ymd = pDBzm->minfo->emd;
			}
			else {
				break;
			}
		} while( pDBzm->minfo->MoveNext() == 0 );
	}

	int st = GnpSiwakeMakeDialog(&spar);

//	MyTrace("GnpSiwakeMakeDialog st = %d\n", st);

	if( st > 0 ) {
		CString filter;
		filter = _T("seq = 1");
		pDBzm->dcntl->Requery(filter);

		if( pDBzm->dcntl->st != -1 ) {
			pDBzm->insseq_make() = FALSE;
			DBDinpAppendParamReload(pDBzm->m_dacnt, pDBzm->dcntl->cnt);
			pDBzm->m_dacnt = pDBzm->dcntl->cnt;
		}
	}
	if( INP_mode == _APPEND ) {
		if( m_InsLine != -1 ) {
			if( m_InsMode == _MODEFY )	disp_insswklist(0, 0);
			else						disp_keyscan_insswklist(0, 0);
		}
		else {
			//ZREC ReSet
			int i, n;
			for( i = 0; i < sizeof ZREC / sizeof ZREC[0]; i++ ) {
				ZREC[i].Reset();
			}
			//仕訳表示用
			pDBzm->dbdata_Requery() = TRUE;

			CDBINPDataRec key;
			key.m_seq = APPEND_SEQ.ngetseq(SEQ_NO - 1);
			if( key.m_seq <= 0 ) key.m_seq = USERS_SEQ;
			n = scaninsdat_getn(&ZREC[0], -1, SCROLL_NL());

			scandat_dspn(&ZREC[0], DDATA_LINE, SCROLL_NL());
			ddata_dsp(-1);
			D_SEQN = SCROLL_NL() - 1;

			INP_LINE._XXDTA.dd_seq = REAL_SEQ_NO + 1;	// 他で追加があるため SEQ更新
			CREC.m_seq = INP_LINE._XXDTA.dd_seq;

			// 08.18 /11
			if( SREC.m_seq < key.m_seq )
				SREC.m_seq = key.m_seq;
			if( NXTDTA_sg ) {
				if( NXT_rec2.m_seq != -1 && (NXT_rec2.m_seq < key.m_seq) )
					NXT_rec2.m_seq = key.m_seq;
			}
			// 08.19 /09
			// 直前のサンプル仕訳 SREC の修正
			set_SREC(&SREC, &ZREC[0], DDATA_LINE, SCROLL_NL(), &INP_LINE, _APPEND);
#ifdef IMPORT	// 10.14 /98
			if( NXTDTA_sg )
				set_SREC(&NXT_rec2, &ZREC[0], DDATA_LINE, SCROLL_NL(), &INP_LINE, _APPEND);
#endif
		}
	}
	else {
		INP_LINE._XXDTA.dd_seq = REAL_SEQ_NO + 1;
		CREC.m_seq = INP_LINE._XXDTA.dd_seq;
		//入力行のSEQ更新
		inpline_update();
	}
}


BOOL CDBDinpView::EnableCallPro()
{
	MyTrace("CDBDinpView::EnableCallPro TOP");

	if( m_pSyzInp == NULL )
		return TRUE;

	int pn = get_nowpn();
	MyTrace("CDBDinpView::EnableCallPro pn = %d\n", pn);

	if( pn == SKBN_PN ) {
		if( !m_pSyzInp->CheckInvno() )
			return FALSE;
	}

	MyTrace("CDBDinpView::EnableCallPro END TRUE");

	return TRUE;
}


// 外税免税自動計算
BOOL CDBDinpView::ChangeSotoMenZei()
{
	BOOL ret = FALSE;

	if( !IsMasterType(MST_SYZ_INV) ) {
		return FALSE;
	}
	DWORD	dwCode = 0;

	struct _DATA_LINE *dtline;
	dtline = getLINE_DATA();

	if( !dinp_modefy_ok(dtline) )
		return FALSE;

	char ip_val[6] = { 0 };

	if( !is_sotomensiwake(&CREC) )
		return FALSE;

	if( CREC.m_ddate < SYZ_INV_YMD )
		return FALSE;

	if( dtline->LINE_CND[ZEI_PN].KEY_inp )
		return FALSE;

	int ln = get_nowln();
	TerminationIcsinputZei(this, ln, 0, 0, 0);

	pDBzm->SetCDBData(&CREC);

	CZmGen8 zmGen;
	zmGen.RepairMenzeiZeigaku(Voln1, pDBzm->dbdata);

	pDBzm->GetCDBData(&CREC);
	memmove(dtline->_XXDTA.dd_val, CREC.m_val, 6);
	memmove(dtline->_XXDTA.dd_zei, CREC.m_zei, 6);

	//税額インプット更新
	VARIANT var;
	memcpy(ip_val, CREC.m_zei, 6);
	var.pbVal = (BYTE*)ip_val;
	exDBdata_set(ZEI_PN, getINP_CTL(this, ZEI_PN)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0);

	int pntbl[] = {
		VAL_PN, ZEI_PN
	};
	TerminationDataSet(pntbl, 2);
	ret = TRUE;
	dtline->LINE_CND[ZEI_PN].KEY_inp = ret;

	return ret;

}


//フローティング選択ウィンドウの表示(
void CDBDinpView::SelectWindowShow(BOOL bShow)
{ 
	SelWndShow(bShow);
};

//WIZ終了時の情報更新
void CDBDinpView::WizRenkeiEndJob()
{
	//入力行 追加ＳＥＱ分 更新
	INP_LINE._XXDTA.dd_seq = pDBzm->m_lastdata.m_seq;
	SaveDinpData_SEQ(_APPEND, INP_LINE._XXDTA.dd_seq);
	inpline_dsp(&INP_LINE, APPEND_LINE);
}

