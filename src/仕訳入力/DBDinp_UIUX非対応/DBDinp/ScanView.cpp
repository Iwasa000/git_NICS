// ScanView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dbdinp.h"

#include "DBbaseView.h"

#include "MainFrm.h"
#include "DBDinpDoc.h"
#include "DBDinpView.h"
extern class CDBDinpView *p_DBDinpView;

#include "mstrw.h"

#include "Input1.h"
#include "external.h"
#include "scan.h"
#include "SubFnc.h"

#include "MsgDlg.h"
#include "DelDataDlg.h"
#include "SiwkDlg.h"

#include "DinpSub.h"
#include "ScanView.h"
#include "RdregDlg.h"

#include "AutoInp.h"
extern struct _AUTOSEL *pAUTOSEL;

// 貸借合計問合わせ
#include "TaiTtl.h"
extern CTaiTtl m_TaiTtl;
#include "TaiDsp.h"
extern CTaiDsp* m_pTaiDsp;

// 枝番摘要モジュール
extern CBrnTky m_BrTek;

#include "RpKeyDlg.h"
extern CRpkeyDlg*	pRpkeyDlg;

#include "scansydef.h"

#include "SeqChgDlg.h"
#include "CStRecordSet.h"
//#include "perform_chk.h"


//
// 仕訳状態ビットテーブル
#include "bit.h"
extern CBit	MODEFY_BIT;		//修正仕訳ビットテーブル
extern CBit	SYZMDFY_BIT;

#include "SEQ.h"
extern CSEQ	APPEND_SEQ;		//追加仕訳ＳＥＱ登録テーブル
extern CSEQ	SEQ_APPEND;		//追加仕訳ＳＥＱ登録テーブル管理テーブル
extern CSEQ	SCAN_SEQ;		//スキャン仕訳ＳＥＱ登録テーブル
extern CSEQ	SEQ_SCAN;		//スキャン仕訳ＳＥＱ登録テーブル管理テーブル

extern CSEQ	SCMDFY_SEQ;		//日付順修正前データＳＥＱ管理テーブル
extern vector<DWORD>	SEQ_SCMDFY;
extern CSEQ	MDFYDATA_SEQ;	//カラムソート用 データＳＥＱ管理テーブル(dbdata)



// データベース用 データスキャン
extern SCAN_ITEMEX		SC_Item;
extern CStringArray		SC_Tky;

extern DB_SCANINFO		SCAN_PGTBL;

extern CSEQ				SCMDFY_SEQ;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
////////
// Extern

extern char Image[];	//in "イメージ摘要" text

extern BOOL bSCAN_ONLY_MODE;

//static int PROGRAM_OK = FALSE;
extern int PROGRAM_OK;

////////
//
static BOOL KEYSCAN_VISIBLE = FALSE;	//スキャン画面表示ＳＷ

#define SCAN_RECMAX		70		//キースキャンリードMax
//#define SCAN_RECMAX	40		//キースキャンリードMax
//#define SCAN_SCROLL_NL	20	// TEST 22	//キースキャン最大データ表示ライン数

static int SCAN_SCROLL_NL = 0;
static CDBINPDataRec	SCAN_ZREC[SCAN_RECMAX],	//キースキャンリードテーブル
					_SCAN_rec;		//修正データ

//スキャン表示データテーブル
static struct _SCAN_LINE	SCAN_DDATA_LINE[ SCAN_RECMAX/*SCAN_SCROLL_NL*/ ];
static int SCAN_LN;		//表示行数

//現在カーソル行
static int NOW_INDEX;

//修正データ取り出し
//開始ポジション SCAN_DDATA_LINE[] 内
static int SCAN_RD_BOTOM;
static int RD_DATAn;

//
// コントロールの位置
static int NOW_PN = -1;

#define SCANVW_PN 0
#define BUTTON1_PN 1
#define BUTTON2_PN 2
#define BUTTON3_PN 3
#define SEQINP_PN 4
//
//　ワーニングメッセージRECT

// 一括修正・取消復帰
int SearchForIkat( int first, CWnd* pWnd, CMsgDlg* pMsgDlg, BOOL* bCancel );
int SearchForDelDataBack( int first, CWnd* pWnd, CMsgDlg* pMsgDlg, BOOL* bCancel );

int DB_DataScan( CString& filter, WORD* dnpsgn = NULL, BOOL mkonly = FALSE );
static BOOL isScanKeyInp();
static void brnquery_str( SCAN_ITEMEX* psi, CString& brstr, int sgn );
/*---VER2---*/
static int kojiinp_querystr( CString& kjstr );

extern void RClickSwkCopyJob( CWnd* pWnd, BOOL bSort );

extern int __cdecl SelSeqCompare(void *context, const void *ele1, const void *ele2 );

extern void InitScanPgTBL( DB_SCANINFO* pSCpg );

int GetDataOwnerSign() 
{
	SCAN_ITEMEX *key = &SC_Item;

	int dtowner = -2;

	// 対象データ
	switch( key->sc_duser ) {
	case SCD_CURUESR:
		if( pDBzm->data_authority() ) {
			if( (key->sc_dtype&0x02) ) {
				dtowner = -2;
			}
			else {
				dtowner = 0;
			}
		}
		else {
			dtowner = 0;
		}
		break;
	default:
		dtowner = -2;	break;
	}

	return dtowner;
}


void QueryStringOwnerCheck( CString& filter ) 
{
	SCAN_ITEMEX *key = &SC_Item;

	if( key->sc_duser == SCD_CURUESR ) {
		if( pDBzm->data_authority() ) {
			if( key->sc_stype == SCS_OR ) {
				int userno = pDBzm->DBUserNo();
				CString stmp;

				if( (key->sc_dtype & 0x03) == 0x03 ) {
					stmp.Format( "(%s and owner = %d)", filter, userno );
					filter = stmp;
				}
				else if( (key->sc_dtype & 0x03) == 0x02 ) {
					stmp.Format( "(%s and owner = %d and cuser = %d)", filter, userno, userno );
					filter = stmp;
				}
			}
		}
	}
}

// ワイルドカード文字の無効化
// → ESCAPE 文字
//    0 ... ワイルドカード文字なし
static int toNotWildCardStr( CString& ans, LPCTSTR p )
{
   ans.Empty();

   int escape = 0;
   CString str;
   int i;
   for( i = 0 ; p[i] ; ) {
       // 漢字
       if( _ismbblead(p[i]) ) {
           str.Format( _T("%c"), p[i] );
           ans += str;
           i++;
           if( p[i] ) {
               str.Format( _T("%c"), p[i] );
               i++;
           }
           ans += str;
       }
       else {
           // ワイルドカード
           switch( p[i] ) {
               case '%':
               case '_':
               case '^':
               case '[':
                   str.Format( _T("%c"), escape = '?' );
                   ans += str;
                   break;
               default:
                   break;
           }
           str.Format( _T("%c"), p[i] );
           ans += str;
           i++;
       }
   }
   return escape;
}


CString MakeTekiyoSearchString( CString& tkyStr, int escape, int tagsgn = 0)
{
	// 検索文字列の作成

	CString	search, str1, str2, str3, escStr;
	int chgsgn = HaihunCheck( tkyStr, str1 );
	int chg2   = HaihunCheck( tkyStr, str2, 1 );
	int chg3   = HaihunCheck( tkyStr, str3, 2 );

	const char* pTkystr = "tekiyo LIKE";
	const char* pTagstr = "tag LIKE";
	const char* ptrType = (tagsgn==0) ? pTkystr : pTagstr;

	//〇（漢数字）
	const char* pJapBin = " COLLATE Japanese_BIN";
	int collate_bin = 0;
	if (tkyStr.Find( "〇" ) != -1) {
		collate_bin = 1;
	}

	if( escape ) escStr.Format( " ESCAPE '%c'", escape );

	if( ! chgsgn && ! chg2 && ! chg3 ) {
		if( escape )
			search.Format( _T("%s '%%%s%%' %s"), ptrType, tkyStr, escStr );
		else
			search.Format( _T("%s '%%%s%%'"), ptrType, tkyStr );

		if(collate_bin)	search += pJapBin;
	}
	else {
		search.Format( _T("(%s '%%%s%%'"), ptrType, tkyStr );
		if( escape )	search += escStr;
		if(collate_bin)	search += pJapBin;

		CString stmp;
		if( chgsgn ) {
			stmp.Format( "%s '%%%s%%'", ptrType, str1 );
			if( escape )	stmp += escStr;
			if(collate_bin)	stmp += pJapBin;
			search += " or ";
			search += stmp;
		}

		if( chg2 ) {
			stmp.Format( "%s '%%%s%%'", ptrType, str2 );
			if( escape )	stmp += escStr;
			if(collate_bin)	stmp += pJapBin;
			search += " or ";
			search += stmp;
		}

		if( chg3 ) {
			stmp.Format( "%s '%%%s%%'", ptrType, str3 );
			if( escape )	stmp += escStr;
			if(collate_bin)	stmp += pJapBin;
			search += " or ";
			search += stmp;
		}

		search += ")";
	}

	return search;
}


const char* eznoinp = "( EXISTS (select ezrec.ezecd from ezrec where ezrec.ezkcd = datarec.dbt and datarec.dbr is null) or \
EXISTS (select ezrec.ezecd from ezrec where ezrec.ezkcd = datarec.cre and datarec.cbr is null) )";

//"( EXISTS (select bmkzrec.kzkcd from bmkzrec where bmkzrec.kzkcd = datarec.dbt and datarec.dbmn is null and datarec.dbt >= '08000000') or \
//EXISTS (select bmkzrec.kzkcd from bmkzrec where bmkzrec.kzkcd = datarec.cre and datarec.cbmn is null and datarec.cre >= '08000000' ) )";
const char* bmnsonnoinp = "((dbt >= '08000000' and dbmn is null) or (cre >= '08000000' and cbmn is null))";

const char* bmnnoinp = "( EXISTS (select bmkzrec.kzkcd from bmkzrec where bmkzrec.kzkcd = datarec.dbt and datarec.dbmn is null) or \
EXISTS (select bmkzrec.kzkcd from bmkzrec where bmkzrec.kzkcd = datarec.cre and datarec.cbmn is null) or \
(((cre = '00000101' and cbmn is null) or (dbt = '00000101' and dbmn is null)) and EXISTS (select seq from datarec where cast (SUBSTRING( dsign, 1, 1 ) as int) & 0x01 = 0x00 and ((cre = '00000101' and cbmn is not null) or (dbt = '00000101' and dbmn is not null)) ) ) )";

const char* bmnedanoinp = "( EXISTS (select bmkzrec.kzkcd from bmkzrec where bmkzrec.kzkcd = datarec.dbt and datarec.dbmn is null) or \
EXISTS (select bmkzrec.kzkcd from bmkzrec where bmkzrec.kzkcd = datarec.cre and datarec.cbmn is null) or \
EXISTS (select bmezrec.ezkcd from bmezrec where bmezrec.ezkcd = datarec.dbt and bmezrec.ezbmn = datarec.dbmn and datarec.dbmn is null ) or \
EXISTS (select bmezrec.ezkcd from bmezrec where bmezrec.ezkcd = datarec.cre and bmezrec.ezbmn = datarec.cbmn and datarec.cbmn is null ) or \
(((cre = '00000101' and cbmn is null) or (dbt = '00000101' and dbmn is null)) and EXISTS (select seq from datarec where cast (SUBSTRING( dsign, 1, 1 ) as int) & 0x01 = 0x00 and ((cre = '00000101' and cbmn is not null) or (dbt = '00000101' and dbmn is not null)) ) ) )";



//一時テーブルの名前を作成
LPCTSTR GetTempTblName(LPSTR prefix, CWnd* pwnd)
{
	CStRecordSet st_rec(pDBzm->m_database);
	st_rec.m_sql = _T("SELECT @@SPID AS st");

	CString strError;
	int sid = 0;

	TRY{
		if( !st_rec.Open() ) {
			ICSMessageBox(_T("session_idが取得できません！"), 0, 0, 0, pwnd);
			return NULL;
		}
		if( !st_rec.IsBOF() ) {
			sid = st_rec.m_st;
		}

		st_rec.Close();
	}
		CATCH(CMemoryException, me) {
		me->GetErrorMessage(strError.GetBuffer(_MAX_PATH), _MAX_PATH);
		strError.ReleaseBuffer();
		ICSMessageBox(strError, 0, 0, 0, pwnd);
		return NULL;
	}
	CATCH(CDBException, de) {
		strError = de->m_strError;
		ICSMessageBox(strError, 0, 0, 0, pwnd);
		return NULL;
	}
	END_CATCH;

	static CString DBtemp;
	DBtemp.Format("%s_%d", prefix, sid);

	return (LPCTSTR)DBtemp;
}


//諸口で部門を入力している仕訳があるか？
BOOL IsSyogBmnSiwake(CWnd* pwnd)
{
	CStRecordSet st_rec(pDBzm->m_database);
	st_rec.m_sql = _T("select count(*) as st from datarec where (cre = '00000101' and cbmn is not null) or (dbt = '00000101' and dbmn is not null)");

	CString strError;
	int sid = 0;

	TRY{
		if( !st_rec.Open() ) {
			ICSMessageBox(_T("諸口部門仕訳が取得できません！"), 0, 0, 0, pwnd);
			return FALSE;
		}
		if( !st_rec.IsBOF() ) {
			sid = st_rec.m_st;
		}

		st_rec.Close();
	}
	CATCH(CMemoryException, me) {
		me->GetErrorMessage(strError.GetBuffer(_MAX_PATH), _MAX_PATH);
		strError.ReleaseBuffer();
		ICSMessageBox(strError, 0, 0, 0, pwnd);
		return FALSE;
	}
	CATCH(CDBException, de) {
		strError = de->m_strError;
		ICSMessageBox(strError, 0, 0, 0, pwnd);
		return FALSE;
	}
	END_CATCH;

	return st_rec.m_st ? TRUE : FALSE;
}



// イメージ表示モジュール CALLBACK 関数
//-----------------------------------------------------------------------------
void ImgDlgCallbackFunc( UINT msg, void *p, CWnd *pWnd  )
{
	IMG_CBMSGINFO	*pCbmsgInfo = (IMG_CBMSGINFO*)p;

	if( msg == ID_IMGDLG_BATCH_F5_MSG ){
		CDBbaseView::m_ImgPrintFlg = PRINT_VERIFY;
		pWnd->PostMessage(WM_COMMAND, ID_BUTTON_F5);
FILETrace( "ImgDlgCallbackFunc  msg = ID_IMGDLG_BATCH_F5_MSG\n" );
	}
	else if( msg == ID_IMGDLG_PRINTIMG_F5_MSG ){
		CDBbaseView::m_ImgPrintFlg = PRINT_DOCIMG;
		pWnd->PostMessage(WM_COMMAND, ID_BUTTON_F5);

FILETrace( "ImgDlgCallbackFunc  msg = ID_IMGDLG_PRINTIMG_F5_MSG\n" );
	}
}


//#undef FILETrace
//#define FILETrace			MyTrace

/////////////////////////////////////////////////////////////////////////////
// CScanView

IMPLEMENT_DYNCREATE(CScanView, ICSFormView)

#ifndef _SLIP_ACCOUNTS_
CScanView::CScanView()
	: ICSFormView(CScanView::IDD), m_ScList(120)
{
	//{{AFX_DATA_INIT(CScanView)
		// メモ: ClassWizard はこの位置にメンバの初期化処理を追加します
	//}}AFX_DATA_INIT
#ifdef	_ICSPEN_	// 01.20 /01
	FirstTime()	=	TRUE;

//	flgPen	=	FALSE;
#endif
}
#else
CScanView::CScanView()
	: ICSFormView(CScanView::IDD), m_ScList(120), m_pImgmng(NULL), m_Imgcnt(-1), m_IsAbleImg(FALSE), m_bSLIP_MASTER(FALSE),	m_bOCRS_ONLY( FALSE ),m_bSendLineMove(FALSE), m_callbyGenpyo(FALSE)
	// '12.09.25
{
	//{{AFX_DATA_INIT(CScanView)
		// メモ: ClassWizard はこの位置にメンバの初期化処理を追加します
	//}}AFX_DATA_INIT
#ifdef	_ICSPEN_	// 01.20 /01
	FirstTime()	=	TRUE;

//	flgPen	=	FALSE;
#endif
	m_bIgnoreOnsizeRedisp = FALSE;
}
#endif


CScanView::~CScanView()
{
}

void CScanView::DoDataExchange(CDataExchange* pDX)
{
	ICSFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScanView)
		// メモ: ClassWizard はこの位置に DDX および DDV の呼び出しを追加します
	DDX_Control(pDX, IDC_BUTTON1, m_Btn1 );
	DDX_Control(pDX, IDC_BUTTON2, m_Btn2 );
	DDX_Control(pDX, IDC_BUTTON3, m_Btn3 );
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ICSDBEDTCTRL1, m_NumInp);
	DDX_Control(pDX, IDC_SCANLIST, m_ScList);
	DDX_Control(pDX, IDC_WANING2, m_Waning);
}


BEGIN_MESSAGE_MAP(CScanView, ICSFormView)
	//{{AFX_MSG_MAP(CScanView)
	ON_COMMAND(ID_BUTTON_END, OnButtonEnd)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_COMMAND(ID_BUTTON_F8, OnButtonF8)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F8, OnUpdateButtonF8)
	ON_COMMAND(ID_BUTTON_F9, OnButtonF9)
	ON_COMMAND(ID_BUTTON_F4, OnButtonDmyf42)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F4, OnUpdateButtonDmyf42)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DSPSCANTBL, DspScanTBL)
	ON_NOTIFY( DLN_SELCHANGED, IDC_SCANLIST, OnScanListSelChanged )
	ON_NOTIFY( NM_DBLCLK, IDC_SCANLIST, OnScanListDblClick )

	ON_COMMAND(ID_BUTTON_DELETE, OnButtonDelete)
	ON_COMMAND(ID_BUTTON_HOME, OnButtonHome)

	ON_BN_CLICKED(IDC_BUTTON2, &CScanView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CScanView::OnBnClickedButton3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CScanView::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CScanView::OnDeltaposSpin2)
	ON_COMMAND(ID_BUTTON_F5, &CScanView::OnButtonF5)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F5, &CScanView::OnUpdateButtonF5)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_DELETE, &CScanView::OnUpdateButtonDelete)
	ON_COMMAND(ID_BUTTON_F3, &CScanView::OnButtonF3)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F3, &CScanView::OnUpdateButtonF3)

	ON_COMMAND(ID_BUTTON_F7, &CScanView::OnButtonF7)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F7, &CScanView::OnUpdateButtonF7)
	ON_COMMAND(IDM_SWK_COPY, &CScanView::OnSwkCopy)
	ON_COMMAND(ID_MENU_SLIP, &CScanView::OnMenuSlip)
	ON_UPDATE_COMMAND_UI(ID_MENU_SLIP, &CScanView::OnUpdateMenuSlip)
	ON_COMMAND(ID_BUTTON_F11, &CScanView::OnButtonF11)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F11, &CScanView::OnUpdateButtonF11)

	ON_NOTIFY( DLN_COLUMNCLICK, IDC_SCANLIST, OnScanListColumnClick )
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanView 診断

#ifdef _DEBUG
void CScanView::AssertValid() const
{
	ICSFormView::AssertValid();
}

void CScanView::Dump(CDumpContext& dc) const
{
	ICSFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScanView メッセージ ハンドラ

void CScanView::OnInitialize( LPCTSTR seq_path )
{
	SEQ_PATH = seq_path;

	DT_ITEMMAX im;
	im.im_seq = 6;
	im.im_denp = 6;
	im.im_eda = Voln1->edcol;
	im.im_bmn = Voln1->bmcol;
	im.im_koji= Voln1->kjcol;
	im.im_tky = (Voln1->tk_ln * 2);
	im.im_sno = Voln1->snocol;
	m_ScList.SetImax( &im );
	m_ScList.SetDispDataLineMax( SCAN_RECMAX );

	m_ScList.SetConfirmMode( Confirm_Mode() );

	// 03.10 /11
	m_OldIndex = -1;

/*
	DWORD style;
	style = m_ScList.GetStyle();
	
	if( Voln1->apno == 0x20 && !(pDBzm->zvol->sub_sw&0x04) ) 
		style |= DS_KOJI_USE;
	else						
		style &= ~DS_KOJI_USE;

	if( (pDBzm->zvol->sub_sw&0x02) )	style |= DS_BMON_USE;
	else								style &= ~DS_BMON_USE;

	m_ScList.SetStyle( style );
	m_ScList.SetLines(1);
*/
	SetListColumn( 1 );
	m_ScList.SetLineHeight( -1 );

//	m_ScList.SetUseDefaultColor( TRUE );
	m_ScList.SetDtTitleColor(SCAN_TITLE_BackCOL);
	m_ScList.SetDtTitleTextColor(SCAN_TITLE_ForeCOL);
	m_ScList.SetDtTitleLineColor(RGB_DARK_GRAY);
    m_ScList.SetDataBkColor( RGB_YELLOW2, 0 );
	m_ScList.SetDataBkColor( RGB_PAPER_WHITE, 1 );

	m_ScList.SetReadOnly( TRUE );
#ifdef SCANNER_SPD
	m_ScList.SetDoubleBuffering( FALSE );
#else
	m_ScList.SetDoubleBuffering( TRUE );
#endif

	m_ScList.EnforceLayout();

	SCAN_SCROLL_NL = m_ScList.GetDispDataCount();
	if( SCAN_SCROLL_NL > SCAN_RECMAX )
		SCAN_SCROLL_NL = SCAN_RECMAX;

	// 決算確定マスター の場合、ボタンの文字列変更
	if( bCONFIRM_MASTER ) {
		GetDlgItem(IDC_BUTTON2)->SetWindowText( _T("確認[Enter]") );
		GetDlgItem(IDC_STATIC1)->SetWindowText( _T("　　番号:") );
	}
	else {
		GetDlgItem(IDC_BUTTON2)->SetWindowText( _T("修正[Enter]") );
		GetDlgItem(IDC_STATIC1)->SetWindowText( _T("修正番号:") );
	}

	CFont* font = m_ScList.GetFont();
	LOGFONT lf;
	font->GetLogFont( &lf );

	IsCheckListPrint() = FALSE;

#ifdef _SLIP_ACCOUNTS_
/*- '12.09.26 -*/
	if( m_pImgmng != NULL ) {
		delete m_pImgmng;
		m_pImgmng = NULL;
		m_IsAbleImg		= FALSE;
		m_bSLIP_MASTER	= FALSE;
		m_bOCRS_ONLY	= FALSE;
	}
/*-------------*/

//	if( ((IsPakageCd(ICS_IMGMNG_OCR_PACKAGE_CODE)==0)|| IsBuyScannerSave() ) && (m_pImgmng==NULL) && ! IsEnkakuUser() ){
//-----------------
	if( (m_pImgmng == NULL) && SlipCheckJob() ){
//-----------------
		m_pImgmng = new CImgMng;
		IMG_INITREC	initrec;
		memset( &initrec, '\0', sizeof(IMG_INITREC) );
		initrec.pDBZmSub = pDBzm;
		if( m_pImgmng->Init(initrec) == 0 ){
			m_bSLIP_MASTER = TRUE;
			m_IsAbleImg = TRUE;
		}
		m_pImgmng->GetImgDispOption( m_ImgDispOption );
		if( m_ImgDispOption.dtypeInConfirm == ID_HIDE_IN_CONFIRM ){
			m_IsAbleImg = FALSE;
		}
	}
#endif

	m_bDspScList = FALSE;

	m_ScList.SetClickSortMode( TRUE );
}

// 入力の標準色を使用か？
void CScanView::SetScanListUseDefColor( BOOL bDefUse ) 
{
	m_ScList.SetUseDefaultColor( bDefUse );
	m_ScList.SetDtTitleTextColor( bDefUse ? SCAN_TITLE_ForeCOL : RGB_WHITE );
}


// 原票関連付けを表示するか？
BOOL CScanView::SlipCheckJob()
{
	BOOL bSlipCheck = FALSE;

	CMainFrame* main = ((CMainFrame*)GetDBDinpMainFrame());

	if( main->IsKeiriJzEBEnv() ) {
		if( bSCANSAVE_Master && (pDBzm->zvol->sub_sw & 0x40) ) {
			bSlipCheck = TRUE;
		}
	}
	else {
		if( (IsBuyOCRScan() || IsBuyScannerSave()) && !IsEnkakuUser() ) {
			bSlipCheck = TRUE;
		}
	}

	return bSlipCheck;
}


void CScanView::OnScanListSelChanged( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_DTLNCHG* nmdt;
	nmdt = (NM_DTLNCHG*)pNMHDR;

	int st, index, imgdlg_confirm = 0;

	if( PROGRAM_OK == FALSE )
		return;

	if( m_bDspScList == FALSE )
		return;

	index = (nmdt->iLine-1);

//MyTrace( "OnScanListSelChanged index = %d, nChar = %d", index, nmdt->nChar );

#ifdef _SLIP_ACCOUNTS_
	// イメージ表示コントロール
	RECT	clRect;
	GetImgDispRECT(&clRect);

//--- '17.03.10 ---
//	if( ((IsPakageCd(ICS_IMGMNG_OCR_PACKAGE_CODE)==0)|| IsBuyScannerSave() ) && (m_pImgmng==NULL) && ! IsEnkakuUser() ){
//-----------------
	if( (m_pImgmng == NULL) && SlipCheckJob() ){
//-----------------
		m_pImgmng = new CImgMng;
		IMG_INITREC	initrec;
		memset( &initrec, '\0', sizeof(IMG_INITREC) );
		initrec.pDBZmSub = pDBzm;
		if( m_pImgmng->Init(initrec) == 0 ){
			m_IsAbleImg = TRUE;
			m_bSLIP_MASTER = TRUE;	// '12.09.25

//-- '17.03.10 --
//			if( IsPakageCd(ICS_IMGMNG_OCR_PACKAGE_CODE)==0 && ! IsBuyScannerSave() && ! bSCANSAVE_Master )
//---------------
			if( IsBuyOCRScan() && ! IsBuyScannerSave() && ! bSCANSAVE_Master )
//---------------
				m_bOCRS_ONLY = TRUE;
			else
				m_bOCRS_ONLY = FALSE;
		}
		m_pImgmng->GetImgDispOption( m_ImgDispOption );
		if( m_ImgDispOption.dtypeInConfirm == ID_HIDE_IN_CONFIRM ){
			m_IsAbleImg = FALSE;
		}
	}

	if( m_IsAbleImg ){
		if( m_ImgDispOption.dtypeInConfirm == ID_DISP_IN_CONFIRM ){
#ifdef SCANNER_SPD_THREAD

			int curidx = m_ScList.GetCurrentLine() - 1;
			if( curidx >= 0 ){
				seqque.push(SCAN_DDATA_LINE[curidx].Data.dd_seq);
			}

			CWinThread* th = AfxBeginThread(DispImgDlgThreadFunc, this, THREAD_PRIORITY_HIGHEST );

#else
			CRect	rc( clRect );
			int curidx = m_ScList.GetCurrentLine() - 1;
			if( curidx >= 0 ){
				IMG_DLGOPTION	dlgOption;
				memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
				dlgOption.btnDispSw |= (0x04|0x08);
#ifdef _SCANNER_SV_
				if( ! m_bOCRS_ONLY ) {
					dlgOption.imgDispSw |= 0x02;
				}
#endif
			//	dlgOption.btnHideSw |= 0x01;
				//KeyUp時に表示
				if( (nmdt->nFlags & 0x02) || (nmdt->nFlags & 0x04 ) || (nmdt->iClick&0x0f) ) {
					m_pImgmng->DispImgDlgOn( pDBzm, SCAN_DDATA_LINE[curidx].Data.dd_seq, this, dlgOption, &rc );
					m_pImgmng->DispDlgSetCallBack( ImgDlgCallbackFunc, this );
				}
			}

#ifdef SCANNER_SPD
			// フォーカスを戻す
			m_ScList.SetFocus();
#endif
#endif
		}
		else if( m_ImgDispOption.dtypeInConfirm == ID_SELDISP_IN_CONFIRM ){
/*- '12.09.25 -*/
//			m_pImgmng->DispImgDlgOff();
/*-------------*/
			if( ((nmdt->iItem==0) && (nmdt->nChar==0)) || m_bSendLineMove || (nmdt->nChar==VK_SPACE) ){
				CRect	rc( clRect );
				int curidx = m_ScList.GetCurrentLine() - 1;
				if( curidx >= 0 ){
					IMG_DLGOPTION	dlgOption;
					memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
					dlgOption.btnDispSw |= (0x04|0x08);
#ifdef _SCANNER_SV_
					if( ! m_bOCRS_ONLY ) {
						dlgOption.imgDispSw |= 0x02;
					}
#endif				
				//	dlgOption.btnHideSw |= 0x01;
					//KeyUp時に表示
//-- '16.03.26 --
//					if( (nmdt->nFlags & 0x02) || (nmdt->nFlags & 0x04 )) {
//---------------
					if( (nmdt->nFlags & 0x02) || (nmdt->nFlags & 0x04) || (nmdt->iClick&0x0f) ) {
//---------------
						imgdlg_confirm = 1;
						m_pImgmng->DispImgDlgOn( pDBzm, SCAN_DDATA_LINE[curidx].Data.dd_seq, this, dlgOption, &rc );
						m_pImgmng->DispDlgSetCallBack( ImgDlgCallbackFunc, this );
					}
				}
#ifdef SCANNER_SPD
				// フォーカスを戻す
				m_ScList.SetFocus();
#endif
			}
			else{
				m_pImgmng->DispImgDlgOff();
			}
/*-------------*/

		}
	}
#endif

	// KeyUp からのメッセージの場合は直前のKeyDownで以下は処理済み
	if( (nmdt->nFlags & 0x02) || (nmdt->nFlags & 0x04 )) {
		return;
	}

	try
	{
		switch( nmdt->nChar ){
		case VK_NEXT:
			if( !scan_dpage_down() )
				Buzzer();
			Set_SeqInp( index );
			ReDispGenpyoImg();
			break;

		case VK_PRIOR:
			if( !scan_dpage_up() )
				Buzzer();
			Set_SeqInp( index );
			ReDispGenpyoImg();
			break;
		case VK_RETURN:
			Set_SeqInp( index );
			tomdfy();	//修正画面へ
			return;
		case VK_DELETE:
			// 取消
			delete_job();
			break;
		case VK_TAB:
			if( is_SHIFT() )	((CDialog*)this)->PrevDlgCtrl();
			else				((CDialog*)this)->NextDlgCtrl();
			break;
#ifdef _SLIP_ACCOUNTS_
		case VK_SPACE:
			if( m_IsAbleImg ){
				if( m_ImgDispOption.dtypeInConfirm == ID_SELDISP_IN_CONFIRM ){
					CRect	rc( clRect );
					int curidx = m_ScList.GetCurrentLine() - 1;
					if( curidx >= 0 ){
						IMG_DLGOPTION	dlgOption;
						memset( &dlgOption, '\0', sizeof(IMG_DLGOPTION) );
						dlgOption.btnDispSw |= (0x04|0x08);
#ifdef _SCANNER_SV_
						if( ! m_bOCRS_ONLY ) {
							dlgOption.imgDispSw |= 0x02;
						}
#endif
					//	dlgOption.btnHideSw |= 0x01;
						imgdlg_confirm = 1;
						m_pImgmng->DispImgDlgOn( pDBzm, SCAN_DDATA_LINE[curidx].Data.dd_seq, this, dlgOption, &rc );
						m_pImgmng->DispDlgSetCallBack( ImgDlgCallbackFunc, this );
					}
#ifdef SCANNER_SPD
					// フォーカスを戻す
					m_ScList.SetFocus();
#endif
				}
			}
			break;
#endif
		default:
			if( index >= 0 ) {
				if( nmdt->nChar == VK_UP || nmdt->nChar == VK_DOWN ) {
					sel_shiftjob( 0, nmdt->nChar, is_SHIFT() );
				}
				else {
					if( nmdt->nChar == 0 && nmdt->iClick ) {
						long nChar = 0;
						if( nmdt->iClick & 0x10 )	nChar = VK_SHIFT;
						if( nmdt->iClick & 0x20 )	nChar = VK_CONTROL;

						click_seljob( nmdt->iLine, nChar );
					}
				}

				Set_SeqInp( index );

			//	if( get_nowpn() != SCANVW_PN )
			//		set_focus( SCANVW_PN );
				int sgn;
				sgn = (nmdt->nFlags&0x01) ? 0x02 : 0;
				if( imgdlg_confirm )	sgn |= 0x04;

				st = _dline_chg( index, sgn );

				sel_lineupdate();

				if( st == -1 )
				{
					Buzzer();
					return;
				}
				else if( !st )
				{
					return;
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


void CScanView::OnScanListDblClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_DTLNCHG* nmdt;
	nmdt = (NM_DTLNCHG*)pNMHDR;

TRACE( "ScanView::OnScanListDblClick %d\n", nmdt->iLine ) ;

	if( PROGRAM_OK == FALSE )
		return;

	int index = (nmdt->iLine-1);

	try
	{
		if( index < 0 )		return;

		if( KEYSCAN_VISIBLE && SCAN_LN && SCAN_DDATA_LINE[ index ].ZREC_NO )
		{
			VARIANT var;
			char buf[30];
			int icno, seq;
			CDBINPDataRec* prec = &SCAN_ZREC[ SCAN_DDATA_LINE[ index ].ZREC_NO - 1];
			icno = prec->m_icno;
			// 取消仕訳は除く
			if( DenpTypeMode() && IsDenpyoSiwake( prec ) && !(prec->m_dsign[0]&0x01) ) {
				DENP_INPUTPAR	dnppar;
				dnppar.pZmsub = pDBzm;
				dnppar.pSyohi = pDBsy;

				dnppar.parent = this;
				dnppar.winApp = &theApp;

				seq = SCAN_ZREC[ SCAN_DDATA_LINE[ index ].ZREC_NO - 1].m_seq;
				dnppar.seq = seq;
				dnppar.nosyzmdf = 0;
				DenpInputSyzMdfyVectorSet( &dnppar );

				sprintf_s( buf, sizeof buf, "%d", seq );
				var.pbVal = (BYTE*)buf;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

				((CMainFrame*)GetDBDinpMainFrame())->ToolBarClear();

				int st = DenpInputDialog( &dnppar );
				if( st != IDCANCEL ) {
					//検索一覧を再表示
					if( dnppar.addseq.size() ) {
						pDBzm->insseq_make() = FALSE;
					}
					// 修正・追加のサインをセット
					DenpEnd_addmdfcheck( &dnppar );

					query_denpend( icno, seq );
					denpend_redisp( seq );
				}

				int scan_sgn = 0;
				if( pDBzm->IsDelScan() )	scan_sgn = 1;
				((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(3, scan_sgn);

				return;
			}
		}

		Set_SeqInp( (nmdt->iLine-1) );
		tomdfy();
		return;
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


void CScanView::denpend_redisp( int seq )
{
	if( PROGRAM_OK == FALSE )
		return;

	CWaitCursor	wc;

	int sort_id, sort_typ;
	((CMainFrame*)GetDBDinpMainFrame())->GetSwkListSortSign( sort_id, sort_typ );

	if( sort_id != -1 ) {
		if( make_scanorder( sort_id, sort_typ ) < 0 ) {
			ICSExit( 0, "仕訳データのソートができません！" );
			return;
		}
	}

	PROGRAM_OK = FALSE;
	m_bDspScList = FALSE;

	{
		//現在カーソル行イニシャライズ
		NOW_INDEX = 0;
		// 表示行数リセット
		SCAN_LN = 0;
		//リードテーブルイニシャライズ
		for( int i = 0; i < SCAN_RECMAX; i++ ) {
			SCAN_ZREC[i].Reset();
		}
		m_ScanSeq = seq;

		//全表示データイニシャライズ
		m_ScList.RemoveAllDtListData();
		m_ScList.RedrawDataListCtrl();

		KEYSCAN_VISIBLE = TRUE;		//画面表示サインセット
	}
	// ここはコメントアウトして、データ表示のメッセージ関数を終えてから TRUE にする
/*	PROGRAM_OK = TRUE;	*/

	PostMessage( WM_DSPSCANTBL, 0 ); 
}



int CScanView::denpend_keyscan_search( int seq )
{
	CWaitCursor	wc;

	int sort_id, sort_typ;
	((CMainFrame*)GetDBDinpMainFrame())->GetSwkListSortSign( sort_id, sort_typ );

	if( sort_id != -1 ) {
		if( make_scanorder( sort_id, sort_typ ) < 0 ) {
			ICSExit( 0, "仕訳データのソートができません！" );
			return -1;
		}
	}

	PROGRAM_OK = FALSE;
	m_bDspScList = FALSE;
	int i,n;

	for( i = 0 ; i < SCAN_RECMAX ; ++i )
	{
		mk_zrodata( &SCAN_DDATA_LINE[i] );
	}

	KEYSCAN_VISIBLE = FALSE;

	//現在カーソル行イニシャライズ
	NOW_INDEX = 0;
	// 表示行数リセット
	SCAN_LN = 0;
	//リードテーブルイニシャライズ
	for( int i = 0; i < SCAN_RECMAX; i++ ) {
		SCAN_ZREC[i].Reset();
	}
	m_ScanSeq = seq;

	//全表示データイニシャライズ
	m_ScList.RemoveAllDtListData();
	m_ScList.RedrawDataListCtrl();

	int nLine = 0;

	if( n = init_dataread( &SCAN_ZREC[SCAN_LN], SCAN_SCROLL_NL - SCAN_LN ) )
	{
		//表示データ作成
		mk_dspdata( &SCAN_DDATA_LINE[ SCAN_LN ], &SCAN_ZREC[ SCAN_LN ], SCAN_LN, n );
	}
	else {
		ICSExit( 0, "仕訳データがありません！" );
		return -1;
	}

	SCAN_RD_BOTOM = 0;
	SCAN_LN += n;

	CDBINPDataRec data;
	BOOL bret = seq_data_rd( &data, seq, 0, 1 );

	PROGRAM_OK = TRUE;
	m_bDspScList = TRUE;

	return bret ? 1 : 0;
}


// ツールバーより Delete	
//
void CScanView::OnButtonDelete()
{
	CWnd* pwnd;
	pwnd = GetFocus();

	if( pwnd->m_hWnd == m_ScList.m_hWnd ) {
		m_ScList.SendMessage( WM_KEYDOWN, VK_DELETE, 0 );
	}
}

// ツールバー Delete 状態
void CScanView::OnUpdateButtonDelete(CCmdUI *pCmdUI)
{
	if( bCONFIRM_MASTER || ! PROGRAM_OK )
		pCmdUI->Enable( FALSE );
	else
		pCmdUI->Enable( TRUE );
}

//	データ行の移動
//		idx			移動インデックス
//		focs_sgn	D0:フォーカスサイン
//					D1:KeyDownでキーが連続して押されている場合。
//					D2:行ごとに確認して原票ダイアログを表示
//
int CScanView::_dline_chg( int idx, int focs_sgn/* = 0*/ )
{
	int i;
	int focs_sw = (focs_sgn&0x01) ? 1 : 0;
	LPARAM lpKeyDown	= (focs_sgn&0x02) ? 0x40000000 : 0;

//MyTrace( "CScanView::_dline_chg idx = %d, focs_sgn = %04x\n", idx, focs_sgn );

	if( idx >= SCAN_LN )	//オーバーフロー
	{
#ifdef DB_OLD_CLOSE
		if( is_thread_active() )
		{
			if( SCAN_LN < SCAN_SCROLL_NL )
				return 0;
		}
#endif

		i = scan_dscroll_up(2);
		if( !i )
		{
			return( -1 );
		}
		else if( i == 1 )
		{
//			NOW_INDEX = SCAN_LN - 1;	//最下行
		}
		else	//i == 2
		{
			SendDlgItemMessage( IDC_SCANLIST, WM_KEYDOWN, VK_UP, lpKeyDown );
			if( ! lpKeyDown ) {
				SendDlgItemMessage( IDC_SCANLIST, WM_KEYUP, VK_UP, lpKeyDown );
			}
//			NOW_INDEX = SCAN_LN - 2;	//最下行の一つ上
		}
	}
	else if( idx < 0 )	//アンダーフロー
	{
		i = scan_dscroll_down(2);
		if( !i )
		{
			return(-1);
		}
		else if( i == 1 )
		{
//			NOW_INDEX = 0;				//先頭行
		}
		else	//i == 2
		{
			SendDlgItemMessage( IDC_SCANLIST, WM_KEYDOWN, VK_DOWN, lpKeyDown);
			if( ! lpKeyDown ) {
				SendDlgItemMessage( IDC_SCANLIST, WM_KEYUP, VK_DOWN, lpKeyDown );
			}
//			NOW_INDEX = 1;				//先頭行の一つ下

			POINT pt;
			if( GetCursorPos( &pt ) ) {
				m_ScList.ScreenToClient( &pt );
				m_ScList.SendMessage( WM_MOUSEMOVE, 0, MAKELONG( pt.x, pt.y ) );
			}
		}
	}
	else 
	{
		if( idx == 0 && !focs_sw )
		{
			if( scan_dscroll_down(1) )
			{
				if( (focs_sgn&0x04)) m_bSendLineMove = TRUE;

				SendDlgItemMessage( IDC_SCANLIST, WM_KEYDOWN, VK_DOWN, lpKeyDown );
				if( ! lpKeyDown ) {
					SendDlgItemMessage( IDC_SCANLIST, WM_KEYUP, VK_DOWN, lpKeyDown );
				}

				if( (focs_sgn&0x04)) m_bSendLineMove = FALSE;

				POINT pt;
				if( GetCursorPos( &pt ) ) {
					m_ScList.ScreenToClient( &pt );
					m_ScList.SendMessage( WM_MOUSEMOVE, 0, MAKELONG( pt.x, pt.y ) );
				}
//				NOW_INDEX = 1;				//先頭行の一つ下
			}
//			else
//				NOW_INDEX = 0;				//先頭行
		}
		else if( (idx == SCAN_LN - 1) && !focs_sw )
		{
			if( scan_dscroll_up(1) )
			{
				if( (focs_sgn&0x04)) m_bSendLineMove = TRUE;

				SendDlgItemMessage( IDC_SCANLIST, WM_KEYDOWN, VK_UP, lpKeyDown);
				if( ! lpKeyDown ) {
					SendDlgItemMessage( IDC_SCANLIST, WM_KEYUP, VK_UP, lpKeyDown );
				}
				if( (focs_sgn&0x04)) m_bSendLineMove = FALSE;

				POINT pt;
				if( GetCursorPos( &pt ) ) {
					m_ScList.ScreenToClient( &pt );
					m_ScList.SendMessage( WM_MOUSEMOVE, 0, MAKELONG( pt.x, pt.y ) );
				}
//				NOW_INDEX = SCAN_LN - 2;		//最下行の一つ上
			}
//			else
//				NOW_INDEX = SCAN_LN - 1;	//最下行
		}
		else
		{
//			NOW_INDEX = idx;
		}
	}

	return(0);
}


//
// ＳＥＱ入力へデータの送付
void CScanView::Set_SeqInp( int index )
{
	if( index < 0 )		return;

	m_OldIndex = index;

	if( KEYSCAN_VISIBLE && SCAN_LN && SCAN_DDATA_LINE[ index ].ZREC_NO )
	{
		VARIANT var;
		char buf[30];
		int seq;
		seq = SCAN_ZREC[ SCAN_DDATA_LINE[ index ].ZREC_NO - 1].m_seq;
		sprintf_s( buf, sizeof buf, "%d", seq );
		var.pbVal = (BYTE*)buf;
	
		DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	//	GetDlgItem( IDC_ICSDBEDTICTRL1 )->UpdateWindow();
	}
}


//
// インデックスのリード
static int get_modefy_idx( int ofs )
{
	int idx;
	int n;

	idx = SCAN_RD_BOTOM - ofs;

FILETrace( "@get_modefy_idx idx = %d, SCAN_RD_BOTOM = %d, ofs = %d\n", idx, SCAN_RD_BOTOM, ofs );

	if( idx < 0 )
	{
		ermset( ERROR_ENV, _T("検索表示データアンダーフロー") );
	}
	else
	{
		n = SCAN_DDATA_LINE[ idx ].ZREC_NO;
		if( n > 0 && n <= SCAN_RECMAX )
		{
			return idx;
		}
		else
		{
			ermset( ERROR_ENV, _T("検索データテーブル範囲エラー") );
		}
	}
	return -1;
}

//
//スキャンデータテーブルに変更データセット
int CScanView::scan_zrec_set( CDBINPDataRec *dta,
							  int ofs )	//SCAN_RD_BOTOMからのオフセット
{
FILETrace( "scan_zrec_set top ofs = %d, SCAN_RD_BOTOM = %d\n", ofs, SCAN_RD_BOTOM );

	int idx;
	int n;
	char pbuf[128];

	idx = get_modefy_idx( ofs );
	if( idx == -1 )
		return -1;

	n = SCAN_DDATA_LINE[ idx ].ZREC_NO;

FILETrace( "scan_zrec_set idx = %d, n = %d\n", idx, n );

	if( SCAN_ZREC[ n - 1 ].m_seq != dta->m_seq )
	{
		sprintf_s( pbuf, sizeof pbuf, "検索データテーブル位置エラー\n元seq=%06d 修seq=%06d\n", SCAN_ZREC[ n - 1 ].m_seq, dta->m_seq );
		ermset( ERROR_ENV, pbuf );
	}
//	memmove( &SCAN_ZREC[ n - 1 ], dta, sizeof( struct _DATAREC ) );	// 元に戻して　ＴＥＳＴ　よければこのまま	08.05 /98
	SCAN_ZREC[ n - 1 ] = *dta;

FILETrace( "scan_zrec_set end\n" );

	return 0;

}


// 修正データのページ単位の取り出し
//	取り出しは底から行う。
int CScanView::scan_data_rd( CDBINPDataRec *dta,	//修正データテーブル底アドレス
							int ln_max,				//読み出し行数
							int ln )				//読み出し開始オフセット
{
	int idx,sft;
	int i,n,botom;

//MyTrace( "scan_data_rd ln_max = %d, ln = %d, SCAN_RD_BOTOM = %d, SCAN_LN = %d\n", ln_max, ln, SCAN_RD_BOTOM, SCAN_LN );

	botom = SCAN_RD_BOTOM;
	idx = botom + ln;

	for( n = 0 ; n < ln_max ; ++n, --dta, --idx )
	{

		if( idx < 0 )	//アンダーフロー
		{
			i = scan_dscroll_down( -idx );
			if( !n )
				sft = -ln - (-idx - i);
			if( !i )
				break;
			idx = i - 1;

			if( !n )
				botom = idx;
			else
				++botom;
		}
		else if( idx >= SCAN_LN )	//オーバーフロー
		{
			i = scan_dscroll_up( idx - SCAN_LN + 1);
			if( !n )
				sft = i;
			if( !i )
				break;
			idx = SCAN_LN - 1;

			botom = idx;
		}
		else
		{
			if( !n )
			{
				botom = idx;
				sft = ln < 0 ? -ln : ln;
			}
		}
		//get data
		*dta = SCAN_ZREC[ SCAN_DDATA_LINE[ idx ].ZREC_NO - 1 ];
//		memmove( dta, &SCAN_ZREC[ SCAN_DDATA_LINE[ idx ].ZREC_NO - 1 ], sizeof( struct _DATAREC ) );

	}

	SCAN_RD_BOTOM = botom;
	RD_DATAn = n;
	return sft;
}

//読み込みデータ数
int CScanView::scan_get_rdatan()
{
	return RD_DATAn;
}


//	page down
int CScanView::scan_dpage_down()
{
	int ret;
	
	// 選択OFF
	sel_shiftjob( 1, VK_SHIFT, FALSE );

	if( ret = scan_dscroll_up( SCAN_SCROLL_NL ) )
	{
		// 残り行移動
		remain_line_move( SCAN_SCROLL_NL, ret );
		if( KEYSCAN_VISIBLE ) 
			m_ScList.RedrawDataListCtrl();
	}

	return ret;
}

// ページダウン時の残り行移動
void CScanView::remain_line_move( int max, int trn )
{

	return;	// TEST *****************

#ifdef DB_OLD_CLOSE
	int n;

	if( !is_thread_active() )
	{
		if( n = max - trn )
		{
			//仕訳表示データエリア n行アップ
			scan_scroll_up( SCAN_DDATA_LINE, SCAN_SCROLL_NL, n );

			if( KEYSCAN_VISIBLE )
			{
				while( n-- )
				{
					m_linp.RemoveData( 0 );	//先頭行を削除
				}
			}
		}
	}
#endif
}

//	page up
int CScanView::scan_dpage_up()
{
	int ret;
	
	// 選択OFF
	sel_shiftjob( 1, VK_SHIFT, FALSE );

	if( ret = scan_dscroll_down( SCAN_SCROLL_NL ) )
	{
		if( KEYSCAN_VISIBLE ) {
			m_ScList.RedrawDataListCtrl();
		}
	}

	return ret;
}


//	data table N line scroll down
int CScanView::scan_dscroll_down( int scrol_nl )
{
	int loop = 0;

	int seq,seq2,n;
	CDBINPDataRec dta, *rec1, *rec2;

#ifdef _MULTI_INP_CUT	// 02.21 /02
	if( scrol_nl > 0 )
	{
		scan_continue_rd( -scrol_nl );
	}
#endif

#ifdef OLD_CLOSE
	if( !found_scan_data() )
		return 0;
#endif

	BOOL bRedraw = FALSE;

	for( loop = 0 ; loop < scrol_nl ; ++loop )
	{
		n = SCAN_DDATA_LINE[0].ZREC_NO - 1;	//SCAN_ZREC[n]

		seq = SCAN_ZREC[n].m_seq;	//そのシーク番号
		rec1 = &SCAN_ZREC[n];

		if( n > 0 )
			n--;
		else	// n == 0
			n = SCAN_RECMAX - 1;
		seq2 = SCAN_ZREC[n].m_seq;	//一つ前のシーク番号
		rec2 = &SCAN_ZREC[n];

		//まだ読み込まれていないZREC[n]か，リングバッファの底か
		if( seq2 <= 0 || dbip_data_cmp( SCAN_PGTBL.data_mode, rec1, rec2 ) < 0 /*seq2 > seq*/ )
		{
#ifdef DB_OLD_CLOSE
			if( --seq <= 0 )
			{
				break;
			}
			if( !data_back( seq - 1, &dta, 1 ) )
#endif
			if( !data_nxt_back( rec1, &dta, 1 ) )
			{
				break;
			}
		//	memmove( &SCAN_ZREC[n], &dta, sizeof( struct _DATAREC ) );
			SCAN_ZREC[n] = dta;

		}

		//仕訳表示データエリア１行ダウン
		scan_scroll_down( SCAN_DDATA_LINE, SCAN_SCROLL_NL, 1 );

		//１行表示
		if( KEYSCAN_VISIBLE )
		{
		//	m_linp.RemoveData( SCAN_LN - 1 );	//最終行を削除

			m_ScList.RemoveDtListData( SCAN_LN - 1 );
			mk_dspdata( &SCAN_DDATA_LINE[ 0 ], &SCAN_ZREC[n], n, 1 );	//表示データを作成
#ifdef BIGIMG	// 03.16 /00
			set_imgtky( &SCAN_DDATA_LINE[ 0 ], 0 );	// イメージ摘要セット
#else
			set_imgtky( &SCAN_DDATA_LINE[ 0 ] );	// イメージ摘要セット
#endif
			bRedraw = TRUE;
			m_ScList.InsertDtListData( 0, &SCAN_DDATA_LINE[ 0 ].Data, FALSE );
		//	m_linp.InsertData( 0, (LPUNKNOWN)&SCAN_DDATA_LINE[ 0 ]._SCNDTA, KEYSCAN_VISIBLE );	//先頭行を表示
		}
		else
		{
			mk_dspdata( &SCAN_DDATA_LINE[ 0 ], &SCAN_ZREC[n], n, 1 );	//表示データを作成
		}
	}

	if( bRedraw ) {
		m_ScList.RedrawDataListCtrl();
	}

	if( !loop )
		return(0);

	return(loop);
}

//	data table N line scroll up
int CScanView::scan_dscroll_up( int scrol_nl )
{
	int loop = 0;

	int seq,seq2,n;
	CDBINPDataRec dta, *rec1, *rec2;

	BOOL bRedraw = FALSE;

	for ( loop = 0 ; loop < scrol_nl ; ++loop )
	{
		n = SCAN_DDATA_LINE[SCAN_LN-1].ZREC_NO - 1;	//SCAN_ZREC[n]

		seq = SCAN_ZREC[n].m_seq;	//そのシーク番号
		rec1 = &SCAN_ZREC[n];

		if( n < SCAN_RECMAX - 1 )
			n++;
		else	// n == SCAN_RECMAX - 1
			n = 0;
		seq2 = SCAN_ZREC[n].m_seq;	//一つ先のシーク番号
		rec2 = &SCAN_ZREC[n];
		//まだ読み込まれていないZREC[n]か，リングバッファのTOPか
		if( seq2 <= 0 || dbip_data_cmp( SCAN_PGTBL.data_mode, rec1, rec2 ) > 0 /*seq2 < seq*/ )
		{

			if( !data_nxt_for( rec1, &dta, 1 ) )
			{
				break;
			}
		//	memmove( &SCAN_ZREC[n], &dta, sizeof( struct _DATAREC ) );
			SCAN_ZREC[n] = dta;
		}
		//仕訳表示データエリア１行アップ
		scan_scroll_up( SCAN_DDATA_LINE, SCAN_SCROLL_NL, 1 );

		//１行表示
		if( KEYSCAN_VISIBLE )
		{
		//	m_linp.RemoveData( 0 );	//先頭行を削除
			m_ScList.RemoveDtListData( 0 );
			mk_dspdata( &SCAN_DDATA_LINE[ SCAN_LN - 1 ], &SCAN_ZREC[n], n, 1 );	//表示データを作成
#ifdef BIGIMG	// 03.16 /00
			set_imgtky( &SCAN_DDATA_LINE[ SCAN_LN - 1], SCAN_LN - 1 );	// イメージ摘要セット
#else
			set_imgtky( &SCAN_DDATA_LINE[ SCAN_LN - 1] );	// イメージ摘要セット
#endif
			m_ScList.SetDtListData( SCAN_LN - 1, &SCAN_DDATA_LINE[ SCAN_LN - 1 ].Data, FALSE/*(scrol_nl==1) ? TRUE : FALSE*/ );
			bRedraw = TRUE;
		//	SetData( SCAN_LN - 1, &SCAN_DDATA_LINE[ SCAN_LN - 1]._SCNDTA, TRUE );	//最終行を表示
		}
		else
		{
			mk_dspdata( &SCAN_DDATA_LINE[ SCAN_LN - 1 ], &SCAN_ZREC[n], n, 1 );	//表示データを作成
		}
	}

	if( bRedraw ) {
		m_ScList.RedrawDataListCtrl();
	}

	if( !loop )
		return(0);

	return(loop);
}


//表示 N 行分ダウン
void CScanView::scan_scroll_down( struct _SCAN_LINE *ldta, int max, int ln )
{
	int start_pn, end_pn;
	int i;

	start_pn = 0;		//スクロール開始ポジション
	end_pn = max - 1;	//スクロール終了ポジション
	while( ln-- )		//スクロール行数分
	{
		for( i = end_pn ; i > start_pn ; --i )
		{
			if( KEYSCAN_VISIBLE )	// 07.02 /98
				*(ldta + i ) = *(ldta + i - 1);
			else
				(ldta + i )->ZREC_NO = (ldta + i - 1)->ZREC_NO;
		}
		mk_zrolindt( ldta + i );	//ゼロデータセット
	}

#ifdef CLOSE	// 08.04 /98
	// データアップデート
	updatejob();
#endif
}

//表示 N 行分アップ
void CScanView::scan_scroll_up( struct _SCAN_LINE *ldta, int max, int ln )
{
	int start_pn, end_pn;
	int i;

	start_pn = 0;		//スクロール開始ポジション
	end_pn = max - 1;	//スクロール終了ポジション
	while( ln-- )		//スクロール行数分
	{
		for( i = start_pn ; i < end_pn ; ++i )
		{
			if( KEYSCAN_VISIBLE )	// 07.02 /98
				*(ldta + i ) = *(ldta + i + 1);
			else
				(ldta + i )->ZREC_NO = (ldta + i + 1)->ZREC_NO;
		}
		mk_zrolindt( ldta + i );	//ゼロデータセット
	}

#ifdef CLOSE	// 08.04 /98
	// データアップデート
	updatejob();
#endif
}


//------------------------------------------------------------------
//	初期検索
//	int scan_sgn	0: 通常検索		1: 取消仕訳検索
//
//------------------------------------------------------------------
void CScanView::scan_init_rd( int rno, int scan_sgn, int requery/*=1*/, int dispseq/*=-1*/ )
{
FILETrace( "@Scan::scan_init_rd TOP rno = %d, scan_sgn = %d, requery = %d\n", rno, scan_sgn, requery );
//	int n;
//	struct _ItemData data = {0};
//	struct _thread_data_scan *pThr;

	// 仕訳選択ボタン　淡色表示
	GetDlgItem( IDC_BUTTON2 )->EnableWindow( FALSE );

	// スキャンオンリー時
	if( bSCAN_ONLY_MODE )
		GetDlgItem( IDC_BUTTON1 )->SetWindowText( "<<終了(End)" );

	//ソートクリア
	m_ScList.ClickSortClear();
	((CMainFrame*)GetDBDinpMainFrame())->SwkListSortSign( -1, 0 );

	// 仕訳番号入力値イニシャライズ
	VARIANT var;
	var.pbVal = NULL;
	DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

	//
	// 前準備
	PROGRAM_OK = FALSE;

	m_bDspScList = FALSE;

	{
		//現在カーソル行イニシャライズ
		NOW_INDEX = 0;

		// 表示行数リセット
		SCAN_LN = 0;

		//リードテーブルイニシャライズ
		for( int i = 0; i < SCAN_RECMAX; i++ ) {
			SCAN_ZREC[i].Reset();
		}

		m_ScanSeq = dispseq;

		//全表示データイニシャライズ
		m_ScList.RemoveAllDtListData();
		m_ScList.RedrawDataListCtrl();

		// 画面表示
//		this->UpdateWindow();
//		set_focus( SCANVW_PN );

		KEYSCAN_VISIBLE = TRUE;		//画面表示サインセット
	}
	// ここはコメントアウトして、データ表示のメッセージ関数を終えてから TRUE にする
/*	PROGRAM_OK = TRUE;	*/

	// キーを受取るためにガイド表示にフォーカスする。
	GetDlgItem( IDC_WANING2 )->SetFocus();

	// 検索前に再描画
	// 子ｳｨﾝﾄﾞｳ 再描画
    CWnd* pwnd = GetWindow( GW_CHILD );
    while( pwnd ) {
      pwnd->UpdateWindow();
      pwnd = pwnd->GetWindow( GW_HWNDNEXT );
    }
	UpdateWindow();

	int cnt;
	cnt = pDBzm->knm_info->reccount;
	cnt += (cnt/4);

	m_TaiTtl.taidata_inz( cnt );	// 貸借データエリアイニシャライズ

	//テーブル作成
	// データベース 検索
	int st;
	if( requery ) {
		RequeryStr.Empty();
		st = DB_DataScan( RequeryStr );
	}
	else {
		pDBzm->dbdata->MoveFirst();
		if( pDBzm->dbdata->st == -1 )
			st = -1;
		else
			st = 0;
	}

	//仕訳数をチェックする
	pDBzm->CheckSiwakeCount();

FILETrace( "@Scan::scan_init_rd INP_mode = %04x\n", INP_mode );

	PostMessage( WM_DSPSCANTBL, st ); 

FILETrace( "@Scan::scan_init_rd END st = %d\n", st );

	return;
}

void CScanView::scan_init_seqrd( int seq )
{
FILETrace( "@Scan::scan_init_seqrd seq = %d, NL() = %d, mode = %02x\n", seq, SCROLL_NL(), INP_mode );
	// 仕訳選択ボタン　淡色表示
	GetDlgItem( IDC_BUTTON2 )->EnableWindow( FALSE );

	// スキャンオンリー時
	if( bSCAN_ONLY_MODE )
		GetDlgItem( IDC_BUTTON1 )->SetWindowText( "<<終了(End)" );

	// 仕訳番号入力値イニシャライズ
	VARIANT var;
	var.pbVal = NULL;
	DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

	//
	// 前準備
	PROGRAM_OK = FALSE;

	m_bDspScList = FALSE;
	{
		//現在カーソル行イニシャライズ
		NOW_INDEX = 0;

		// 表示行数リセット
		SCAN_LN = 0;

		// フォークで検索画面 表示ＳＥＱ
		m_ScanSeq = seq;

		//リードテーブルイニシャライズ
		for( int i = 0; i < SCAN_RECMAX; i++ ) {
			SCAN_ZREC[i].Reset();
		}

		//全表示データイニシャライズ
		m_ScList.RemoveAllDtListData();
		m_ScList.RedrawDataListCtrl();

		// 画面表示
//		this->UpdateWindow();
//		set_focus( SCANVW_PN );

		KEYSCAN_VISIBLE = TRUE;		//画面表示サインセット
	}
	// ここはコメントアウトして、データ表示のメッセージ関数を終えてから TRUE にする
/*	PROGRAM_OK = TRUE;	*/

	// キーを受取るためにガイド表示にフォーカスする。
	GetDlgItem( IDC_WANING2 )->SetFocus();

	// 検索前に再描画
	// 子ｳｨﾝﾄﾞｳ 再描画
    CWnd* pwnd = GetWindow( GW_CHILD );
    while( pwnd ) {
      pwnd->UpdateWindow();
      pwnd = pwnd->GetWindow( GW_HWNDNEXT );
    }
	UpdateWindow();

	int cnt;
	cnt = pDBzm->knm_info->reccount;
	cnt += (cnt/4);

	m_TaiTtl.taidata_inz( cnt );	// 貸借データエリアイニシャライズ

	inpmdisp( _KEYSCAN );

	if( m_IsAbleImg ){
		if( m_pImgmng && m_pImgmng->IsDlgVisible() ){
			m_pImgmng->DispImgDlgOff();
		}
	}
	//テーブル作成
	// データベース 検索
	int st;

	// 検索対象データは、権限があれば、全ユーザ
	if( ! pDBzm->data_authority() ) {
		SC_Item.sc_duser = SCD_CURUESR;
	}
	else {
		SC_Item.sc_duser = SCD_ALLDATA;
	}

	RequeryStr.Empty();
	st = DB_DataScan( RequeryStr );

FILETrace( "@Scan::scan_init_seqrd INP_mode = %04x\n", INP_mode );

	PostMessage( WM_DSPSCANTBL, st ); 

FILETrace( "@Scan::scan_init_seqrd END\n" );

	return;
}


static BOOL RetroChange = FALSE;

//
// スキャンテーブル作成＆表示
LRESULT CScanView::DspScanTBL( WPARAM wParam, LPARAM lParam)
{
FILETrace( "@Scan::DspScanTBL TOP SCAN_LN = %d\n" , SCAN_LN );

	int iconic_sg;
	int sv;

	sv = SCAN_LN;

	//検索仕訳数セット
//	set_scan_datan( pThr->n );

	if( ! wParam )
	{

		scan_init_dsp();	//画面の表示

		set_focus( SCANVW_PN );
	//	if( !sv )
	//	{
	//		set_focus( SEQINP_PN );
	//		set_focus( SCANVW_PN );
	//	}
	}
	else
	{
		if( m_pTaiDsp )
			if( m_pTaiDsp->m_Active )
			{
				m_pTaiDsp->PostMessage( WM_KEYDOWN, VK_ESCAPE, 0 );	// 貸借ダイアログの消去
				m_pTaiDsp->PostMessage( WM_KEYUP, VK_ESCAPE, 0 );	// 貸借ダイアログの消去
			}


		iconic_sg = GetDBDinpMainFrame()->IsIconic();

		if( !iconic_sg )
		{
			WINDOWPLACEMENT wndplc = {0};

			GetWindowPlacement( &wndplc );
			wndplc.flags = WPF_RESTORETOMAXIMIZED;

			SetWindowPlacement( &wndplc );
		}

		if( pDBzm->IsDelScan() )
			myICSMessageBox( "取消仕訳はありません。" );
		else
			myICSMessageBox( "指定された仕訳は見つかりませんでした。" );

		if( iconic_sg )
		{
			// アイコンの場合はビューチェンジは後で行う
			RetroChange = TRUE;
		}
		else
		{
			PostMessage( WM_COMMAND, MAKELONG(ID_BUTTON_F8, 0) );

			RetroChange = FALSE;
		}
	}
	
	// 移動後の現在行
	seqchg_curline();

	// DATABASE MDI 対応で移動
	PROGRAM_OK = TRUE;	

	m_bDspScList = TRUE;

	if( m_IsAbleImg ) {
		if( m_pImgmng && m_pImgmng->IsDlgVisible() ){
			m_ScList.PostMessageA( WM_KEYDOWN, VK_SPACE );
		}
	}

	waning( FALSE );

FILETrace( "@Scan::DspScanTBL END SCAN_LN = %d\n" , SCAN_LN );

	return 1;
}


//
// ワーニングメッセージ表示／非表示
void CScanView::waning( BOOL direct )
{
	if( direct )
	{
		m_Waning.ShowWindow( SW_SHOW );
	}
	else
	{
		m_Waning.ShowWindow( SW_HIDE );
	}
}

//
// 初期表示用のデータ読み込み
//	CDBINPDataRec *data	データ格納バッファ
//	int nl				データ数
//
int CScanView::init_dataread( CDBINPDataRec *data, int nl )
{
	int ret = 0;

	if( SCAN_PGTBL.sort_id == -1 ) {
		ret = _init_dataread( data, nl );
	}
	else {
		ret = _init_sortread( data, nl );
	}

	return ret;
}


int CScanView::_init_dataread( CDBINPDataRec *data, int nl )
{
CDBINPDataRec dta;
dta.Reset();
int n, vect_n;
vect_n = 1;		// 順方向への読み込み

int nl_max;

nl_max = nl;
if( nl_max > SCAN_RECMAX )
	nl_max = SCAN_RECMAX;

	pDBzm->dbdata->MoveFirst();

	if( pDBzm->dbdata->st == -1 )
		return 0;

	if( m_ScanSeq <= 0 ) {
		for( n = 0 ; n < nl_max ; )
		{
			// 本プログラム中に発生した取消[ MODEFY_BIT.bitncnd() で修正された仕訳 ] もＯＫとする
			if( pDBzm->IsDelScan() || !(pDBzm->dbdata->dsign[0] & 0x01) || MODEFY_BIT.bitncnd( pDBzm->dbdata->seq-1 ) ) {
				//データセット
				pDBzm->GetCDBData( &dta );
			//	ispread( pDBzm->data->seq-1, &dta );
				*data = dta;
				data += vect_n;
				n++;
			}

			pDBzm->dbdata->MoveNext();
			if( pDBzm->dbdata->st == -1 )
				break;
		}
	}
	else {
		//スキャン検索をフォークで表示  必ず SEQ順で
		CDWordArray	da;
		da.SetSize( nl_max );
		int rg_idx, i;
		DWORD abs_pos = 1;
		
		rg_idx = 0;

//FILETrace("@init_dataread m_ScanSeq = %d, nl = %d, %d\n", m_ScanSeq, nl, nl_max );

		for( i = n = 0 ; n < nl_max; )
		{
			// 本プログラム中に発生した取消[ MODEFY_BIT.bitncnd() で修正された仕訳 ] もＯＫとする
			if( pDBzm->IsDelScan() || !(pDBzm->dbdata->dsign[0] & 0x01) || MODEFY_BIT.bitncnd( pDBzm->dbdata->seq-1 ) ) {
				// リングバッファ先頭へ
				if( rg_idx >= nl_max ) {
					rg_idx = 0;
				}
				da[rg_idx] = abs_pos;
				rg_idx++;

			//	if( pDBzm->dbdata->seq >= m_ScanSeq ) {
				if( datarec_seqcmp( 0, pDBzm->dbdata->seq, m_ScanSeq ) >= 0 ) {
					n++;
				}
			}

			pDBzm->dbdata->MoveNext();
			if( pDBzm->dbdata->st == -1 )
				break;

			abs_pos++;
		}

		//リングバッファより内容を データレコードにセット
		abs_pos = 0;
		rg_idx = -1;
		for( i = 0; i < nl_max; i++ ) {
//FILETrace("@init_dataread da[%d] = %d\n", i, da[i] );

			if( da[i] == 0 )
				break;

			if( i == 0 ) {
				abs_pos = da[i];
				rg_idx = 0;
			}
			else {
				if( abs_pos > da[i] ) {
					abs_pos = da[i];
					rg_idx = i;
				}
			}
		}

//FILETrace("@init_dataread rg_idx = %d\n", rg_idx );

		if( rg_idx >= 0 ) {
			n = 0;

			for( i = rg_idx; i < nl_max; i++ ) {
				if( da[i] == 0 )
					break;

				pDBzm->dbdata->SetAbsolutePosition( da[i] );
				pDBzm->GetCDBData( &dta );
				*data = dta;
				data += vect_n;
				n++;
			}
			if( rg_idx > 0 ) {
				// リングバッファの残り部分
				for( i = 0; i < rg_idx; i++ ) {
					if( da[i] == 0 )
						break;

					pDBzm->dbdata->SetAbsolutePosition( da[i] );
					pDBzm->GetCDBData( &dta );
					*data = dta;
					data += vect_n;
					n++;
				}
			}
		}
		else {
			n = 0;
		}
	}

//FILETrace("@init_dataread END n = %d\n", n );

	return( n );
}


// SORT 順でのデータ読み取り
int CScanView::_init_sortread( CDBINPDataRec *data, int nl )
{
	CDBINPDataRec dta;
	dta.Reset();
	int n, pos, dbpos, nl_max, seq;

	nl_max = nl;
	if( nl_max > SCAN_RECMAX )
		nl_max = SCAN_RECMAX;

	if( SCAN_PGTBL.sort_cnt == 0 )
		return 0;

	int vect_n = 1;

	check_scan_insertquery();

	if( m_ScanSeq <= 0 ) {

		pos = 1;

		for( n = 0; n < nl ; pos++ )
		{
			if( pos > SCAN_PGTBL.sort_cnt || pos < 1 )
				break;


			seq = SEQ_SCMDFY[ pos-1 ];

			int dbpos = MDFYDATA_SEQ.ngetseq( seq-1 );

			pDBzm->dbdata->SetAbsolutePosition( dbpos );

			if( pDBzm->dbdata->st == -1 ) {
				break;
			}

			// 本プログラム中に発生した取消[ MODEFY_BIT.bitncnd() で修正された仕訳 ] もＯＫとする
			if( pDBzm->IsDelScan() || !(pDBzm->dbdata->dsign[0] & 0x01) || MODEFY_BIT.bitncnd( pDBzm->dbdata->seq-1 ) ) {
				//データセット
				pDBzm->GetCDBData( &dta );
			//	ispread( pDBzm->data->seq-1, &dta );
				*data = dta;
				data += vect_n;
				n++;
			}
		}
	}
	else {
		// 指定番号より表示する
		CDWordArray	da;
		da.SetSize( nl_max );
		int rg_idx, i, start_idx, last_idx;
		DWORD abs_pos = 1;
		
		rg_idx = 0;

//FILETrace("@init_dataread m_ScanSeq = %d, nl = %d, %d\n", m_ScanSeq, nl, nl_max );
		pos = 1;
		start_idx = -1;

		for( n = 0; n < nl ; pos++ )
		{
			if( pos > SCAN_PGTBL.sort_cnt || pos < 1 )
				break;

			seq = SEQ_SCMDFY[ pos-1 ];

			dbpos = MDFYDATA_SEQ.ngetseq( seq-1 );

			pDBzm->dbdata->SetAbsolutePosition( dbpos );

			if( pDBzm->dbdata->st == -1 ) {
				break;
			}

			// 本プログラム中に発生した取消[ MODEFY_BIT.bitncnd() で修正された仕訳 ] もＯＫとする
			if( pDBzm->IsDelScan() || !(pDBzm->dbdata->dsign[0] & 0x01) || MODEFY_BIT.bitncnd( pDBzm->dbdata->seq-1 ) ) {
				// リングバッファ先頭へ
				if( rg_idx >= nl_max ) {
					rg_idx = 0;
				}
				da[rg_idx] = dbpos;

				if( datarec_seqcmp( -1, pDBzm->dbdata->seq, m_ScanSeq ) >= 0 ) {
					if( start_idx == -1 ) {
						start_idx = rg_idx;
					}
					n++;
				}
				last_idx = rg_idx;

				rg_idx++;
			}
		}

//FILETrace("@init_dataread start_idx = %d\n", rg_idx );

		if( n == 0 || start_idx == -1 )
			return 0;

		if( n < nl ) {
			// データ表示がリスト行数に満たない場合は、底からデータを作成する。
			int i, tmpCnt;
			CDWordArray	daTmp;

			//最終データから１つずつ上に
			for( i = last_idx, tmpCnt = 0; tmpCnt < n; tmpCnt++, i-- ) {
				if( i < 0 )	i = nl_max - 1;

				if( da[i] == 0 )
					break;

				// 無限ループの防止
				if( tmpCnt > 0 ) {
					if( i == last_idx )
						break;
				}

				daTmp.Add( da[i] );
			}

			for( i = start_idx -1 ; ; i-- ) {
				if( i < 0 )	i = nl_max - 1;

				if( da[i] == 0 )
					break;

				// 上のループで処理済み
				if( i == last_idx )
					break;

				daTmp.Add( da[i] );
			}

			n = 0;
			int cnt = daTmp.GetCount();

			CDBINPDataRec *prec;
			prec = (data + cnt-1);

			for( i = 0; i < cnt; i++ ) {
				pDBzm->dbdata->SetAbsolutePosition( daTmp[i] );
				pDBzm->GetCDBData( &dta );
				*prec = dta;
				prec--;
				n++;
			}
		}
		else {
			rg_idx = start_idx;

			if( rg_idx != -1 ) {
				n = 0;

				for( i = rg_idx; i < nl_max; i++ ) {
					if( da[i] == 0 )
						break;

					pDBzm->dbdata->SetAbsolutePosition( da[i] );
					pDBzm->GetCDBData( &dta );
					*data = dta;
					data += vect_n;
					n++;
				}
				if( rg_idx > 0 ) {
					// リングバッファの残り部分
					for( i = 0; i < rg_idx; i++ ) {
						if( da[i] == 0 )
							break;

						pDBzm->dbdata->SetAbsolutePosition( da[i] );
						pDBzm->GetCDBData( &dta );
						*data = dta;
						data += vect_n;
						n++;
					}
				}
			}
			else {
				n = 0;
			}
		}
	}

//FILETrace("@init_dataread END n = %d\n", n );

	return( n );
}



//
//初期表示
void CScanView::scan_init_dsp()
{
	int i,n;

//	CDBINPDataRec  key;
//	key.m_seq = SCAN_PGTBL.tbl[0].s_seq;
//	key.m_ddate = SCAN_PGTBL.tbl[0].s_ddate;
//	key.m_ksign = SCAN_PGTBL.tbl[0].s_ksign;

	int nLine = 0;

//	if( n = data_for( &key, &SCAN_ZREC[SCAN_LN], SCAN_SCROLL_NL - SCAN_LN ) )
	if( n = init_dataread( &SCAN_ZREC[SCAN_LN], SCAN_SCROLL_NL - SCAN_LN ) )
	{
		//表示データ作成
		mk_dspdata( &SCAN_DDATA_LINE[ SCAN_LN ], &SCAN_ZREC[ SCAN_LN ], SCAN_LN, n );

		//データ表示
		if( KEYSCAN_VISIBLE )
		{
			for( i = 0 ; i < n ; ++i )
			{
#ifdef BIGIMG	// 03.16 /00
				set_imgtky( &SCAN_DDATA_LINE[ SCAN_LN+i ], SCAN_LN+i );
#else
				set_imgtky( &SCAN_DDATA_LINE[ SCAN_LN+i ] );
#endif
				m_ScList.SetDtListData( SCAN_LN+i, &SCAN_DDATA_LINE[ SCAN_LN+i ].Data, FALSE );

			//	SetData( SCAN_LN+i, &SCAN_DDATA_LINE[ SCAN_LN+i ]._SCNDTA, FALSE );

				if( m_ScanSeq > 0 ) {
				//	if( SCAN_DDATA_LINE[ SCAN_LN+i ].Data.dd_seq >= m_ScanSeq ) {
					if( datarec_seqcmp( SC_Item.sc_type, SCAN_DDATA_LINE[ SCAN_LN+i ].Data.dd_seq, m_ScanSeq ) >= 0 ) {
						if( ! nLine ) {
							nLine = i+1;
						}
					}
				}
			}

			if( nLine == 0 )	nLine = 1;
			m_ScList.SetCurrentLine( nLine );
			m_ScList.RedrawDataListCtrl();	// 再描画
		//	NowIndexDsp();			//ラインカーソル表示
		}
	}
	else {
		// データなし
		PostMessage( WM_DSPSCANTBL, 1, 0 );
FILETrace( "WM_DSPSCANTBL--> 1, 0\n" );
		return;
	}

	SCAN_LN += n;
	if( nLine == 0 )	nLine = 1;
	Set_SeqInp( nLine-1 );

	_EnterEnable();

}


//表示データ作成
void CScanView::mk_dspdata( struct _SCAN_LINE *ldta, CDBINPDataRec *data, int data_n, int nline )
{
	int i;

	for( i = 0 ; i < nline ; ++i )
	{
		if( KEYSCAN_VISIBLE )
			mk_1dspdata( ldta + i, data + i );
		(ldta + i)->ZREC_NO = ++data_n;	//SCAN_ZREC[] ポジション
	}
}


//１行分表示データ作成
void CScanView::mk_1dspdata( struct _SCAN_LINE *ldta, CDBINPDataRec *data )
{
	mk_zrodata( ldta );			//Controll data initialize
	scan_recset( ldta, data );	//DATA to Controll data
//	set_clmdspsg( ldta );		//表示データサインセット
}

//DATA to Controll data
void CScanView::scan_recset( struct _SCAN_LINE *ldta, CDBINPDataRec *data )
{
static char	fmt[20] = {0};
char	pbuf[82];
BRN_NAME *pnm;
BOOL bDisp;
struct _SET_KMKETC sk;
int i, st;

	if( data->m_ddate == 0 ) return;

	// 消費税初期化
	DB_SyIniz( data );

	// 色変更属性ＯＮ

	//番号	
	ldta->Data.dd_ctbl[DATA_SEQ].back = SCAN_TITLE_BackCOL;
//	if( m_ScList.GetUseDefaultColor() )
//		ldta->Data.dd_ctbl[DATA_SEQ].back = SCAN_TITLE_BackCOL;
//	else
//		ldta->Data.dd_ctbl[DATA_SEQ].back = -1;

	ldta->Data.dd_seq = data->m_seq;

	/* サイン */
	ldta->Data.dd_dsign = 0;
	if( data->m_dsign[0] & 0x01 )
		ldta->Data.dd_dsign |= 0x01;	// 取消表示
	// 仕訳背景色
	if( (st = tline_chk( data )) != 0 ) {
		if( st == -1 )	ldta->Data.dd_dsign |= 0x02;	// 修正不可
		else			ldta->Data.dd_dsign |= 0x08;	// 修正不可
	}
	int sg = MODEFY_BIT.bitncnd( ldta->Data.dd_seq -1 );	// 今回修正仕訳
	if( sg > 0 )	ldta->Data.dd_dsign |= 0x04;

	// 部門・工事名称表示？
	bDisp = ((pAUTOSEL->BMNDSP_OPT == OPT_BMNDSP) ? TRUE : FALSE);

	//日付
	ldta->Data.dd_ddate = data->m_ddate;
	ldta->Data.dd_mofs = data->m_mofs;
	ldta->Data.dd_ksign = data->m_ksign;

	//伝票タイプ
	CString stmp;
	if( data->m_icno > 0 ) {
		DenpTypeString( data->m_dtype, stmp );
	}

	if( stmp.IsEmpty() ) {
		::ZeroMemory( (ldta)->Denp, sizeof (ldta)->Denp );
		ldta->Data.dd_udtbl[0].userdata = NULL;
	}
	else {
		strcpy_s( (ldta)->Denp, sizeof (ldta)->Denp, stmp );
		ldta->Data.dd_udtbl[0].userdata = (BYTE*)(ldta)->Denp;
	}

	//伝票番号
	if( DenpNoInpCheck( data->m_cno ) ) { //入力漏れ
		ldta->Data.dd_cno = data->m_cno;
		ldta->Data.dd_dsign |= 0x2000;
		ldta->Data.dd_ctbl[DENP_PN].text = RGB_DARK_BLUE;
	}
	else {
		ldta->Data.dd_cno = data->m_cno;
		ldta->Data.dd_ctbl[DENP_PN].text = (DWORD)-1;
	}

	//借方部門
	if( BmnNoInpCheck(data->m_dbt, data->m_dbmn, data->m_cre) ) {
		ldta->Data.dd_dbmn = data->m_dbmn;
		ldta->Data.dd_dsign |= 0x10;
		ldta->Data.dd_ctbl[BMON_PN].text = RGB_DARK_BLUE;
	}
	else {
		// 部門
		sk.bmn = data->m_dbmn;
		sk.kno.Empty();
		sk.code.Empty();
		sk.brn = -1;

		if( bDisp && _set_bmnetc( &sk ) ) {
			ldta->Data.dd_dbmn = data->m_dbmn;
			ldta->Data.dd_dbmn_etc = sk.etc;
			ldta->Data.dd_ctbl[BMON_PN].text = sk.etc_col;	// 文字色
		}
		else {
			ldta->Data.dd_dbmn = data->m_dbmn;
			ldta->Data.dd_dbmn_etc.Empty();
			ldta->Data.dd_ctbl[BMON_PN].text = (DWORD)-1;
		}
	}
	//借方工事
	if( KojiNoInpCheck( data->m_dbt, data->m_dkno ) ) {
		ldta->Data.dd_dkno = data->m_dkno;
		ldta->Data.dd_dsign |= 0x100;
		ldta->Data.dd_ctbl[KOJI_PN].text = RGB_DARK_BLUE;
	}
	else {
		ldta->Data.dd_dkno = data->m_dkno;
		sk.bmn = -1;
		sk.kno = ldta->Data.dd_dkno;
		sk.code.Empty();
		sk.brn = -1;

		if( bDisp && _set_kojietc( &sk ) ) {
			ldta->Data.dd_dkno_etc = sk.etc;
			ldta->Data.dd_ctbl[KOJI_PN].text = sk.etc_col;	// 文字色
		}
		else {
			ldta->Data.dd_dkno_etc.Empty();
			ldta->Data.dd_ctbl[KOJI_PN].text = (DWORD)-1;
		}
	}

	// 科目名称フォーマット
	if( !fmt[0] )
		sprintf_s( fmt, sizeof fmt, "%%-%d.%ds", 14, 14 );

	//借方
	if( NoInpBrnCheck( data->m_dbt, data->m_dbr ) ) {
		ldta->Data.dd_dsign |= 0x40;
		ldta->Data.dd_ctbl[DATA_DEBT].text = RGB_DARK_BLUE;
	}
	else if( BmnNoInpBrnCheck( data->m_dbt, data->m_dbr, data->m_dbmn )) {
		ldta->Data.dd_dsign |= 0x40;
		ldta->Data.dd_ctbl[DATA_DEBT].text = RGB_DARK_BLUE;
	}
	else {
		// 当面 部門枝番は表示しない
		int bmncd = -1;
		if( pAUTOSEL->BMNTKBR_OPT == OPT_CHKON )	bmncd = data->m_dbmn;

		if( pnm = m_BrTek.get_brtekmsg( bmncd/*data->m_dbmn*/, data->m_dbt, data->m_dbr, TRUE, _GetBrTekMode()) )
		{
			ldta->Data.dd_deb_etc = pnm->Name;
			ldta->Data.dd_ctbl[DATA_DEBT].text = pnm->ForeColor;	// 文字色
		}
		else
		{
			ldta->Data.dd_deb_etc.Empty();
			ldta->Data.dd_ctbl[DATA_DEBT].text = (DWORD)-1;	// 文字色
		}
	}
	//　科目名称
	struct _DBKNREC	*pKn;

	if( pKn = pDBzm->DB_PjisToKnrec( data->m_dbt, TRUE ) ) {
		if( pAUTOSEL->SCNCD_OPT == OPT_SCNCD ) {
			kmkcode_string( 2, pKn, pbuf, sizeof pbuf );
		}
		else {
			sprintf_s( pbuf, sizeof pbuf, fmt, pKn->knnam );
		}
	}
	else
		sprintf_s( pbuf, sizeof pbuf, fmt, "" );
	ldta->Data.dd_deb_name = pbuf;
	ldta->Data.dd_dbt = data->m_dbt;
	ldta->Data.dd_dbr = data->m_dbr;
	
	//貸方部門
	if( BmnNoInpCheck(data->m_cre, data->m_cbmn, data->m_dbt) ) {
		ldta->Data.dd_cbmn = data->m_cbmn;
		ldta->Data.dd_dsign |= 0x20;
		ldta->Data.dd_ctbl[CBMN_PN].text = RGB_DARK_BLUE;
	}
	else {
		bDisp = ((pAUTOSEL->BMNDSP_OPT == OPT_BMNDSP) ? TRUE : FALSE);
		// 部門
		sk.bmn = data->m_cbmn;
		sk.kno.Empty();
		sk.code.Empty();
		sk.brn = -1;

		if( bDisp && _set_bmnetc( &sk ) ) {
			ldta->Data.dd_cbmn = data->m_cbmn;
			ldta->Data.dd_cbmn_etc = sk.etc;
			ldta->Data.dd_ctbl[BMON_PN].text = sk.etc_col;	// 文字色
			ldta->Data.dd_ctbl[CBMN_PN].text = sk.etc_col;	// 文字色
		}
		else {
			ldta->Data.dd_cbmn = data->m_cbmn;
			ldta->Data.dd_cbmn_etc.Empty();
			ldta->Data.dd_ctbl[CBMN_PN].text = (DWORD)-1;
		}
	}
	//貸方工事
	if( KojiNoInpCheck( data->m_cre, data->m_ckno ) ) {
		ldta->Data.dd_dkno = data->m_ckno;
		ldta->Data.dd_dsign |= 0x200;
		ldta->Data.dd_ctbl[CKJI_PN].text = RGB_DARK_BLUE;
	}
	else {
		ldta->Data.dd_ckno = data->m_ckno;
		sk.bmn = -1;
		sk.kno = ldta->Data.dd_ckno;
		sk.code.Empty();
		sk.brn = -1;

		if( bDisp && _set_kojietc( &sk ) ) {
			ldta->Data.dd_ckno_etc = sk.etc;
			ldta->Data.dd_ctbl[KOJI_PN].text = sk.etc_col;	// 文字色
			ldta->Data.dd_ctbl[CKJI_PN].text = sk.etc_col;	// 文字色
		}
		else {
			ldta->Data.dd_ckno_etc.Empty();
			ldta->Data.dd_ctbl[CKJI_PN].text = (DWORD)-1;
		}
	}

	//貸方
	if( NoInpBrnCheck( data->m_cre, data->m_cbr ) ) {
		ldta->Data.dd_dsign |= 0x80;
		ldta->Data.dd_ctbl[DATA_CRED].text = RGB_DARK_BLUE;
	}
	else if( BmnNoInpBrnCheck( data->m_cre, data->m_cbr, data->m_cbmn )) {
		ldta->Data.dd_dsign |= 0x80;
		ldta->Data.dd_ctbl[DATA_CRED].text = RGB_DARK_BLUE;
	}
	else {
		// 当面 部門枝番は表示しない
		int bmncd = -1;
		if( pAUTOSEL->BMNTKBR_OPT == OPT_CHKON )	bmncd = data->m_cbmn;

		if( pnm = m_BrTek.get_brtekmsg( bmncd/*data->m_cbmn*/, data->m_cre, data->m_cbr, TRUE, _GetBrTekMode()) )
		{
			ldta->Data.dd_cre_etc = pnm->Name;
			ldta->Data.dd_ctbl[DATA_CRED].text = pnm->ForeColor;	// 文字色
		}
		else
		{
			ldta->Data.dd_cre_etc.Empty();
			ldta->Data.dd_ctbl[DATA_CRED].text = (DWORD)-1;	// 文字色
		}
	}
	//　科目名称
	if( pKn = pDBzm->DB_PjisToKnrec( data->m_cre, TRUE ) ) {
		if( pAUTOSEL->SCNCD_OPT == OPT_SCNCD ) {
			kmkcode_string( 2, pKn, pbuf, sizeof pbuf );
		}
		else {
			sprintf_s( pbuf, sizeof pbuf, fmt, pKn->knnam );
		}
	}
	else
		sprintf_s( pbuf, sizeof pbuf, fmt, "" );
	ldta->Data.dd_cre_name = pbuf;
	ldta->Data.dd_cre = data->m_cre;
	ldta->Data.dd_cbr = data->m_cbr;

	//金額
	memmove( ldta->Data.dd_val, data->m_val, sizeof(data->m_val));
	ldta->LINE_CND[VAL_PN].INP_sg = TRUE;
	ldta->Data.val_flag = ldta->LINE_CND[VAL_PN].INP_sg;
	//税額
	memmove( ldta->Data.dd_zei, data->m_zei, sizeof(data->m_zei));
	ldta->LINE_CND[ZEI_PN].INP_sg = TRUE;
	ldta->Data.zei_flag = ldta->LINE_CND[ZEI_PN].INP_sg;

	sg = 0;
	COLORREF back;
	if (isnot_defzeigaku(data, back)) sg = 1;
	if (sg > 0)	ldta->Data.dd_ctbl[ZEI_PN].back = back;
	else		ldta->Data.dd_ctbl[ZEI_PN].back = (COLORREF)-1;

	//消費税
	ldta->Data.dd_syz_str = sy_line_txt( data );
	ldta->LINE_CND[SKBN_PN].INP_sg = TRUE;

	//有価証券非課税譲渡
	if( is_syohizeisiwake( data ) ) {
		if( pDBsy->IsTaikaData( pDBzm->dbdata ) == 2 )
			ldta->Data.zei_flag |= 0x10;
	}
	else {
		ldta->Data.zei_flag = 0;
	}
	//消費税変更
	sg = SYZMDFY_BIT.bitncnd( data->m_seq - 1);
	if( sg > 0 )	ldta->Data.dd_ctbl[SKBN_PN].back = RGB_SYZ_MDFY;
	else			ldta->Data.dd_ctbl[SKBN_PN].back = (COLORREF)-1;

	//摘要
	if( data->m_dsign[0] & 0x80 )
	{
		ldta->LINE_CND[TKY_PN].IMG_sg = 1;	//TRUE

		ldta->ImgSEQ = data->m_imgsq;
		i = wsprintf( pbuf, "%s", Image );
		ldta->Data.dd_tek.tekiyo = pbuf;
	}
	else
	{
		ldta->LINE_CND[TKY_PN ].IMG_sg = 0;	//FALSE
		ldta->ImgSEQ = 0;
		ldta->Data.dd_tek.tekiyo = data->m_tekiyo;
		ldta->Data.dd_tek.width = 0;
		ldta->Data.dd_tek.height = 0;
		ldta->Data.dd_tek.image = NULL;
	}
	/* ﾃｶﾞﾀﾊﾞﾝｺﾞｳ */
	ldta->Data.dd_snumber = data->m_rnumber;

	/* ﾃｶﾞﾀｷｼﾞﾂ */
	ldta->Data.dd_tdate = data->m_tdate;
	LPSTR p;
	p = ldta->Data.dd_tdstr.GetBuffer(30);
	IntsrekitoGenstr( p, 30, ldta->Data.dd_tdate );
	ldta->Data.dd_tdstr.ReleaseBuffer(-1);
	ldta->LINE_CND[ TGDT_PN ].INP_sg = TRUE;

	/* 証憑番号 */
	ldta->Data.dd_docevi = data->m_skn;
	ldta->LINE_CND[ DOCEVI_PN ].INP_sg = TRUE;

	/* 付箋 */
	ldta->Data.dd_husen = (data->m_dsign[7] & 0x0f);
	ldta->Data.dd_hscomment = data->m_tag;

	/* 変動事由 or 振替区分[公益] */
	_get_hjiyustr( data->m_hjiyu, data->m_nbcd, ldta->Data.dd_hjiyu );

#ifdef _SLIP_ACCOUNTS_	// '12.09.25
/*-  '12.09.26 -*/
//	if( data->m_dsign[6] & 0x01 ){
/*--------------*/
	if( m_bSLIP_MASTER && (data->m_dsign[6] & 0x01) ){
/*--------------*/
		ldta->Data.dd_dsign |= 0x800;
	}
#endif
}


// 枝番 入力漏れチェック
BOOL CScanView::NoInpBrnCheck( CString& code, int brn )
{
	if( SC_Item.sc_noinp != SC_NOINP_BRN &&
		SC_Item.sc_noinp != SC_NOINP_BMNBRN &&
		SC_Item.sc_noinp != SC_NOINP_BMORBR )	return FALSE;

	if( brn != -1 )		return FALSE;

/*	CString filter;
	filter.Format( "ezkcd = '%s'", code );

	if( pDBzm->ezrec->Requery( filter, 1 ) == 0 )
		return TRUE;	*/

	ezdb_init( pDBzm );
	if( pDBzm->ezrec->Find( (LPCTSTR)code ) > 0 ) return TRUE;

	return FALSE;
}


// 部門 入力漏れチェック
BOOL CScanView::BmnNoInpCheck( CString& code, int bmn, CString&  paircd)
{
	if( SC_Item.sc_noinp != SC_NOINP_BMON &&
		SC_Item.sc_noinp != SC_NOINP_BMNBRN &&
		SC_Item.sc_noinp != SC_NOINP_BMORBR )	return FALSE;

	if( bmn != -1 )		return FALSE;

	DWORD dwCode;

	if( code == SYOGT ) {
		if( Voln1->bm_isw & 0x10 ) {// 損益のみ

			dwCode = _atoh((char*)(LPCTSTR)paircd);
			hl_rev(&dwCode, 4);

			if( dwCode > 0x08000000 ) {
				if( IsSyogBmnSiwake(this) ) {
					return TRUE;
				}
			}
		}
		else {
			if( IsSyogBmnSiwake(this) ) {
				return TRUE;
			}
		}
	}

	if( Voln1->bm_isw & 0x10 ) {// 損益のみ
		dwCode = _atoh( (char*)(LPCTSTR)code );
		hl_rev( &dwCode, 4 );

		if( dwCode > 0x08000000 ) {
			return FALSE;
		}
		else {
			return TRUE;
		}
	}

	CString filter;
	filter.Format( "kzkcd = '%s'", code );

	if( pDBzm->bmkzrec->Requery( filter ) == 0 )
		return TRUE;

	return FALSE;
}


// 部門枝番 入力漏れチェック
BOOL CScanView::BmnNoInpBrnCheck( CString& code, int brn, int bmn )
{
	if( SC_Item.sc_noinp != SC_NOINP_BMON )	return FALSE;

	if( /*bmn == -1 ||*/ brn != -1 )	return FALSE;

	CString filter;
	if( bmn != -1 ) {
		filter.Format( "ezbmn = %d and ezkcd = '%s'", bmn, code );
	}
	else {
		filter.Format( "ezkcd = '%s'", code );	
	}

	if( pDBzm->bmezrec->Requery( filter, 1 ) == 0 )
		return TRUE;

	return FALSE;
}

// 工事 入力漏れチェック
BOOL CScanView::KojiNoInpCheck( CString& code, CString& kno )
{
	DWORD dwCode;
	dwCode = _atoh( (LPSTR)(LPCTSTR)code );
	BOOL	bRet = FALSE;

	if( SC_Item.sc_noinp != SC_NOINP_KOJI )	return FALSE;
	// 工事入力あり
	if( ! kno.IsEmpty() )
		return FALSE;

	if( (dwCode & 0x0f000000) ) {
		return TRUE;
	}
	else {
		INT_PTR max;
		max = pDBzm->m_KJchk.GetCount();

		for( int i = 0; i < max; i++ ) {
			if( (dwCode & 0xffffff00) == pDBzm->m_KJchk[i] ) {
				bRet = TRUE;
				break;
			}
		}
	}

	return bRet;
}


//１行分ゼロデータ作成
void CScanView::mk_zrolindt( struct _SCAN_LINE *ldta )
{
	mk_zrodata( ldta );	//項目ゼロセット
//	set_clmdspsg( ldta );	//項目表示サインセット
};

//1行分ゼロ項目作成
void CScanView::mk_zrodata( struct _SCAN_LINE *ldta )
{
	int i;

	ldta->ZREC_NO = 0;

	if( KEYSCAN_VISIBLE == FALSE )	// 07.02 /98
		return;
	
	for( i = SEQ_PN ; i <= HJIYU_PN ; ++i )
	{
		switch( i )
		{
		case SEQ_PN:
			ldta->Data.dd_seq = 0;
			break;
		case DATE_PN:
			ldta->Data.dd_ddate = 0;
			ldta->Data.dd_mofs = 0;
			ldta->Data.dd_ksign = 0;
//			::ZeroMemory( dtline->BcdDate, sizeof dtline->BcdDate );
			break;
		case DENP_PN:
			ldta->Data.dd_cno = -1;
			break;
		case BMON_PN:
			ldta->Data.dd_dbmn = -1;
			ldta->Data.dd_dbmn_etc.Empty();
			break;
		case KOJI_PN:
			ldta->Data.dd_dkno.Empty();
			ldta->Data.dd_dkno_etc.Empty();
			break;
		case CBMN_PN:
			ldta->Data.dd_cbmn = -1;
			ldta->Data.dd_cbmn_etc.Empty();
			break;
		case CKJI_PN:
			ldta->Data.dd_ckno.Empty();
			ldta->Data.dd_ckno_etc.Empty();
			break;
		case DEBT_PN:
			ldta->Data.dd_dbt.Empty();
			ldta->Data.dd_deb_name.Empty();
			ldta->Data.dd_dbr = -1;
			ldta->Data.dd_deb_etc.Empty();
			break;
		case CRED_PN:
			ldta->Data.dd_cre.Empty();
			ldta->Data.dd_cre_name.Empty();
			ldta->Data.dd_cbr = -1;
			ldta->Data.dd_cre_etc.Empty();
			break;
		case VAL_PN:
			_fillbuf( (char *)&ldta->Data.dd_val, sizeof(ldta->Data.dd_val), 0);
			break;
		case ZEI_PN:
			_fillbuf( (char *)&ldta->Data.dd_zei, sizeof(ldta->Data.dd_zei), 0);
			break;
		case SKBN_PN:
			ldta->Data.dd_syz_str.Empty();
			break;
		case TKY_PN:
			ldta->Data.dd_tek.tekiyo.Empty();
			ldta->Data.dd_tek.image = NULL;
			break;
		case HSEN_PN:
			ldta->Data.dd_husen = -1;
			ldta->Data.dd_hscomment.Empty();
			break;
		case HJIYU_PN:
			ldta->Data.dd_hjiyu.Empty();
			break;
		case DOCEVI_PN:
			ldta->Data.dd_docevi.Empty();
			break;

		default:
			continue;
			break;
		}
		ldta->ImgSEQ = 0;

		ldta->Data.dd_ctbl[i].back	= (DWORD)-1;
		ldta->Data.dd_ctbl[i].text	= (DWORD)-1;

	}
	//コンディションサイン OFF
	memset( ldta->LINE_CND, '\0', sizeof( ldta->LINE_CND ) );
}


// イメージ摘要セット
#ifdef BIGIMG	// 03.16 /00
void CScanView::set_imgtky( struct _SCAN_LINE *data, int index  )
#else
void CScanView::set_imgtky( struct _SCAN_LINE *data )
#endif
{
IMREC *pIMREC;

	data->Data.dd_tek.image = 0;

	if( data->ImgSEQ > 0 )
	{
//		CString filter;
//		filter.Format( "iseq = '%d'", data->ImgSEQ );

//		if( pDBzm->image->Requery( filter ) == 0 ) {
		if( pDBzm->image->SetAbsolutePosition( data->ImgSEQ ) == 0 ) {
			// イメージファイル
			pIMREC = pDBzm->image->DecodeImage();
			if( pIMREC )
			{
	#ifdef BIGIMG	// 03.16 /00
				data->Data.dd_tek.width = pIMREC->dotxs;
				data->Data.dd_tek.height = pIMREC->dotys;
				data->Data.dd_tek.image = (char*)pIMREC->imdata;
	#endif
			}
		}
	}
}


// ENTERキーEnable/Disable
int CScanView::_EnterEnable() 
{
	CWnd *pwnd = GetDlgItem( IDC_BUTTON2 );
	int condition = -1;

	if( pDBzm->IsDelScan() ) {
		// 取消仕訳検索時
		pwnd->EnableWindow( FALSE );
		pwnd->UpdateWindow();
		m_NumInp.EnableWindow( FALSE );

		condition = 0;
	}
	else {
		m_NumInp.EnableWindow( TRUE );

		if(SCAN_LN && !pwnd->IsWindowEnabled() )
		{
			pwnd->EnableWindow( TRUE );
			pwnd->UpdateWindow();

			condition = 1;
		}
		else if(!SCAN_LN && pwnd->IsWindowEnabled() )
		{
			pwnd->EnableWindow( FALSE );
			pwnd->UpdateWindow();

			condition = 0;
		}
	}
	return condition;
}

int CScanView::EnterEnable( struct tagMSG* pMsg ) 
{
#ifdef CLOSE	// 11.26 /99
	CWnd *pcwnd = GetDlgItem( IDC_BUTTON2 );
	static int condition = -1;

	if(SCAN_LN && condition != 1 )
	{
		pcwnd->EnableWindow( TRUE );
		pcwnd->UpdateWindow();

		condition = 1;
	}
	else if(!SCAN_LN && condition != 0 )
	{
		pcwnd->EnableWindow( FALSE );
		pcwnd->UpdateWindow();

		condition = 0;
	}
#else

	int condition;

	condition = _EnterEnable();
#endif

	if( !condition && ((pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) ||
					(pMsg->message == WM_LBUTTONDOWN &&  pMsg->hwnd == ((CWnd*)GetDlgItem( IDC_SCANLIST ))->m_hWnd)) )
		return 0;
	else
		return 1;
}



//
//修正から返る場合の表示
void CScanView::scan_now_dsp( int ofs )	//SCAN_RD_BOTOMからのオフセット
{
	int n,max;
	int pn;
	
	PROGRAM_OK = FALSE;
	{

		//画面表示サインセット
		KEYSCAN_VISIBLE = TRUE;

		//画面表示
		m_ScList.RemoveAllDtListData();		//全表示データイニシャライズ

		set_focus( SCANVW_PN );

		max = SCAN_LN;
		for( n = 0 ; n < max ; ++n )	//表示データ作成
		{
			if( pn = SCAN_DDATA_LINE[n].ZREC_NO )
			{
				mk_1dspdata( &SCAN_DDATA_LINE[n], &SCAN_ZREC[ pn - 1 ] );
				SCAN_DDATA_LINE[n].ZREC_NO = pn;	//SCAN_ZREC[] ポジション
			}

		}
		scan_ddata_dsp( SCAN_DDATA_LINE, n, 1 );	//データ表示

		//現在カーソル行
		NOW_INDEX = SCAN_RD_BOTOM - ofs;

//		NowIndexDsp();				//ラインカーソル表示
		m_ScList.SetCurrentLine( NOW_INDEX+1 );

		this->UpdateWindow();

	}
	PROGRAM_OK = TRUE;	

}


//
// ＳＥＱ番号検索
BOOL CScanView::seq_data_rd( CDBINPDataRec *data, int seq, int ofs, int vect )
{

	int count, count2;
	int start_idx;
	int end_idx;
	int vect_n = vect < 0 ? -1 : 1;
	int n;
	CDBINPDataRec *dta;

#ifdef DB_OLD_CLOSE
	// 検索中チェック
	if( in_sdanchk( seq, vect ) )
		return FALSE;	// まだ未検索の場合はエラーとする
#endif

	start_idx = get_modefy_idx( ofs );
	if( start_idx == -1 )
		return FALSE;

	// 該当ページまでページアップＯＲダウン
	for( count = 0 ; count < 10000 ; ++count )
	{
		if( vect < 0 )
			end_idx = 0;
		else
			end_idx = SCAN_LN - 1;
		if( end_idx < 0 )
			return FALSE;

		n = SCAN_DDATA_LINE[ end_idx ].ZREC_NO - 1;
		if( n < 0 )
			return FALSE;

		dta = &SCAN_ZREC[ n ];
		if( vect < 0 )
		{
		//	if( seq >= dta->m_seq )
		//		break;
			if( dbip_data_cmp( SCAN_PGTBL.data_mode, seq, -1, -1, dta->m_seq, -1, -1 ) >= 0 ) {
				break;
			}
			if( !scan_dpage_up() )
				break;
			start_idx = SCAN_LN - 1;
		}
		else
		{
		//	if( seq <= dta->m_seq )
		//		break;
			if( dbip_data_cmp( SCAN_PGTBL.data_mode, seq, -1, -1, dta->m_seq, -1, -1 ) <= 0 ) {
				break;
			}
			if( !scan_dpage_down() )
				break;
			start_idx = 0;
		}
	}
	if( count >= 10000 )
		return FALSE;

	if( start_idx < 0 )
		return FALSE;

	// 該当ページ内での検索
	for( count2 = 0 ; (count2 < 100) && (start_idx >= 0) && (start_idx < SCAN_LN) && ((vect < 0) ? (start_idx >= end_idx) : (start_idx <= end_idx)) ; start_idx += vect_n, ++count2 )
	{
		if( count || (!count && count2) )
			SCAN_RD_BOTOM = start_idx;	//修正での読み出し開始位置

		n = SCAN_DDATA_LINE[ start_idx ].ZREC_NO - 1;
		if( n < 0 )
			return FALSE;

		dta = &SCAN_ZREC[ n ];

		if( vect < 0 )
		{
		//	if( seq >= dta->m_seq )
		//		break;
			if( dbip_data_cmp( SCAN_PGTBL.data_mode, seq, -1, -1, dta->m_seq, -1, -1 ) >= 0 ) {
				break;
			}
		}
		else
		{
		//	if( seq <= dta->m_seq )
		//		break;
			if( dbip_data_cmp( SCAN_PGTBL.data_mode, seq, -1, -1, dta->m_seq, -1, -1 ) <= 0 ) {
				break;
			}
		}
	}
//	memmove( data, dta, sizeof( struct _DATAREC ));
	*data = *dta;	

	return TRUE;
}



void CScanView::OnInitialUpdate() 
{
	ICSFormView::OnInitialUpdate();
	ResizeParentToFit();

	// 縦の長さ変更
	SetResize( IDC_SCANLIST, ICS_RESIZE_BOTTOM );
	// オフセット分移動
	int nID[] = {
		IDC_BUTTON1, IDC_BUTTON2, IDC_BUTTON3,
		IDC_ICSDBEDTCTRL1,IDC_WANING2, IDC_SPIN1, IDC_SPIN2, IDC_STATIC1, IDC_STATIC2
	};
	for( int i = 0; i < (sizeof nID / sizeof nID[0]); i++ ) {
		SetResize( nID[i], ICS_RESIZE_TOPBOTTOM );
	}
}

void CScanView::OnDraw(CDC* pDC) 
{
	if( !FirstTime() )	return;
	FirstTime()	=	FALSE;


}

#ifdef	_ICSPEN_	// 01.20 /01

/*
short CScanView::SetPen( BOOL flg )
{
	flgPen	=	flg;
	if( flg != FALSE )
	{
		short	code;
		CString	msg;
		//初期化
		if( (code = icspen.InitPen( GetParent() )) ) {
			msg	=	icspen.GetError( code );
			ICSMessageBox( msg );
			return	-1;
		}
		//コントロールの登録
		icspen.RegisterControl( this );

		icspen = p_DBDinpView->GetPenObject();
		icspen->RegisterControl( this );
	}
	return	0;
}


void CScanView::OnICSInputFocus( )
{
	if( flgPen != FALSE ) {
		
		CRect	rect, crect, rect1, rect2;
		this->GetClientRect( &crect );
		this->ClientToScreen( &crect );
		((CWnd*)this)->GetDlgItem( IDC_ICSDBEDTCTRL1 )->GetWindowRect( rect1 );
		rect2 = p_DBDinpView->GetICSAnkRect();
		rect.left	=	crect.left;
		rect.top	=	rect1.top - rect2.Height() - 1;
		rect.right	=	rect.left + crect.Width();
		rect.bottom	=	rect1.top - 1;

 		icspen->ShowWindow( TRUE, PWINDOW_NUM );
 		icspen->SetPos( (void*)(LPCRECT)rect, PWINDOW_NUM );
	}
}
*/
#endif

void CScanView::OnButtonEnd() 
{
#ifdef CLOSE	// 08.04 /98
	// データアップデート
	updatejob();
#endif

#ifdef _SLIP_ACCOUNTS_
	TabLbuttonDownJob();
#endif

	m_ScList.HideTitleTipWindow();

	//一括修正項目 初期化
	CRpkeyDlg::keyline_inz();

	//入力にビュー切り替え
	if( ! IsMAsterKAKUTEI() ) {
		((CMainFrame*)GetDBDinpMainFrame())->ScanKeyClear();

		((CMainFrame*)GetDBDinpMainFrame())->ChgToInpView( 0 );
	}
	else {
		//検索項目入力にビュー切り替え
		((CMainFrame*)GetDBDinpMainFrame())->ChgToSkeyView();
	}
}



//	終了処理
//
BOOL CScanView::EndApp( WPARAM wParam, BOOL bCosel )
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


//終了処理
void CScanView::ScanCloseWork()
{
	//財務クラスの解放があるため、デストラクタで呼び出すのをやめる
	if( m_pImgmng != NULL ) {
		delete m_pImgmng;
		m_pImgmng = NULL;
		m_IsAbleImg = FALSE;
		m_bSLIP_MASTER = FALSE;
		m_bOCRS_ONLY = FALSE;
	}
}

// チェックリストを呼び出す
void CScanView::OnButtonF5()
{
	//印刷へ
	if( CDBbaseView::m_ImgPrintFlg >= PRINT_VERIFY ) {
		if( m_pImgmng && m_pImgmng->IsDlgVisible() ){
			m_pImgmng->DispImgDlgOff(); 
		}

		if( CDBbaseView::SetNprtParam( m_pImgmng ) < 0 )
			return;
		ICSFormView::OnFilePrint();
		return;
	}

	((CMainFrame*)GetDBDinpMainFrame())->CallOtherPro(0);
}

void CScanView::OnUpdateButtonF5(CCmdUI *pCmdUI)
{
	if( pDBzm->IsDelScan() ) {
		pCmdUI->Enable(FALSE);
	}
	else if( m_callbyGenpyo && CDBbaseView::m_ImgPrintFlg == PRINT_NONE ) {
		pCmdUI->Enable(FALSE);
	}
	else
		pCmdUI->Enable(TRUE);
}

// チェックリストを ＣＳＶ出力で、呼び出す
void CScanView::OnButtonF3()
{
	((CMainFrame*)GetDBDinpMainFrame())->CallOtherPro(1);
}

void CScanView::OnUpdateButtonF3(CCmdUI *pCmdUI)
{
	if( pDBzm->IsDelScan() || m_callbyGenpyo )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


//検索項目入力にビュー切り替え
void CScanView::OnButtonF8() 
{

	m_ScList.HideTitleTipWindow();

#ifdef _SLIP_ACCOUNTS_
	TabLbuttonDownJob();
#endif

	((CMainFrame*)GetDBDinpMainFrame())->ChgToSkeyView();
}


void CScanView::OnUpdateButtonF8(CCmdUI* pCmdUI) 
{
	// TODO: この位置OnUpdateButtonF8に command update UI ハンドラ用のコードを追加してください
	
}


void CScanView::OnButtonF9() 
{
	try
	{
		((CMainFrame*)GetDBDinpMainFrame())->totaldisp();
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}	
}


//読み込み開始位置
int CScanView::scan_rdpnt()
{
	return SCAN_RD_BOTOM;
}


//修正画面+ＳＥＱ呼出し
void CScanView::tomdfy2( int seq, int now_seq )
{
	if( pDBzm->IsDelScan() )	return;

#ifdef _SLIP_ACCOUNTS_
	TabLbuttonDownJob();
#endif

	SCAN_RD_BOTOM = m_ScList.GetCurrentLine();	//修正での読み出し開始位置
	SCAN_RD_BOTOM--;
	KEYSCAN_VISIBLE = FALSE;
	((CMainFrame*)GetDBDinpMainFrame())->ChgToInpView( 3, seq, now_seq );	
}


//修正画面へ
BOOL CScanView::tomdfy()
{
	int idx;

	if( pDBzm->IsDelScan() )	return FALSE;

	idx = m_ScList.GetCurrentLine()-1;
	if( idx >= SCAN_LN || idx < 0 )
	{
		Buzzer();
		set_focus( SCANVW_PN );
		return FALSE;
	}

#ifdef CLOSE	// 08.04 /98
	// データアップデート
	updatejob();
#endif

#ifdef _SLIP_ACCOUNTS_
	TabLbuttonDownJob();
#endif

	SCAN_RD_BOTOM = idx;	//修正での読み出し開始位置
	KEYSCAN_VISIBLE = FALSE;
	((CMainFrame*)GetDBDinpMainFrame())->ChgToInpView( 1 );	
	return TRUE;
}

// 取消
void CScanView::delete_job()
{
int idx;
CDBINPDataRec *data, key;

	idx = m_ScList.GetCurrentLine()-1;

	if( idx >= SCAN_LN || idx < 0 )
	{
		Buzzer();
		set_focus( SCANVW_PN );
		return;
	}

	data = &SCAN_ZREC[ SCAN_DDATA_LINE[ idx ].ZREC_NO - 1 ];

	if( ! modefy_ok( _KEYSCAN, data ) )
	{
		Buzzer();
		return;
	}

	//	rno = data->m_seq - 1;
	key.m_seq = data->m_seq;
	key.m_ddate = data->m_ddate;

	if( SCAN_DDATA_LINE[ idx ].Data.dd_dsign & 0x01 )
	{
#ifndef CLOSE	// 10.29 /98
		if( !isDataDelOK( 1 ) )
			return;
#else
		if( isSTRAGE() )
		{
			// 電子データ保存マスターでは取消の取消は行えない
			MessageBeep( 0xffffffff );
			return;
		}
#endif
		SCAN_DDATA_LINE[ idx ].Data.dd_dsign &= ~0x01;
		data->m_dsign[0] &= ~0x01;
	}
	else
	{
		if( !isDataDelOK( 0 ) )
			return;
		SCAN_DDATA_LINE[ idx ].Data.dd_dsign |= 0x01;
		data->m_dsign[0] |= 0x01;
	}

	// 今回修正サインON修正
	SCAN_DDATA_LINE[ idx ].Data.dd_dsign |= 0x04;

	// データリード
	data_for( &key, &_SCAN_rec, 1 );
#ifdef CLOSE	// 08.04 /98 TEST
	DATA_1RD = 1;
#endif

	int ret = dataupd( data, &_SCAN_rec, _KEYSCAN, 0, this );
	if( ret == -2 ) {
		PROGRAM_OK = FALSE;
		
		int st = myICSMessageBox( (LPCTSTR)pDBzm->GetZmErrMsg() );
	
		PROGRAM_OK = TRUE;
		pDBzm->GetCDBData( data );

		if( data->m_dsign[0] & 0x01 ) {
			SCAN_DDATA_LINE[ idx ].Data.dd_dsign |= 0x01;
		}
		else {
			SCAN_DDATA_LINE[ idx ].Data.dd_dsign &= ~0x01;
		}

		if( data->m_dsign[7] & 0x10 ) {
			SCAN_DDATA_LINE[ idx ].Data.dd_dsign |= 0x02;
		}
		else {
			SCAN_DDATA_LINE[ idx ].Data.dd_dsign &= ~0x02;
		}
		// 今回修正サインOFF
		SCAN_DDATA_LINE[ idx ].Data.dd_dsign &= ~0x04;

	}

	//諸口・現金貸借
	if( data->m_dsign[0] & 0x01 )
	{
		syog_genk( data, _SUB );
	}
	else
	{
		syog_genk( data, _ADD );
	}

	scan_line_dsp( &SCAN_DDATA_LINE[idx], idx, TRUE );
}


//1ページ分データ表示
void CScanView::scan_ddata_dsp( struct _SCAN_LINE *ldta, int nline, int vect )
{
	int i,n;

	for( i = 0 ; i < nline ; ++i )
	{
		n = vect < 0 ? nline - (i + 1) : i;
		scan_line_dsp( ldta + n, n );
	}
}

//１行分データ表示
void CScanView::scan_line_dsp( struct _SCAN_LINE *data, int index, BOOL bRedraw /*=FALSE*/ )
{
#ifdef BIGIMG	// 03.16 /00
	set_imgtky( data, index );
#else
	set_imgtky( data );
#endif
//	SetData( n, data->_SCNDTA, TRUE );
	m_ScList.SetDtListData( index, &data->Data, bRedraw );
}


void CScanView::set_focus( int pn ) 
{
	switch( pn )
	{
	case SCANVW_PN:
		GetDlgItem( IDC_SCANLIST )->SetFocus();
		break;

	case BUTTON1_PN:
		((CDialog*)this)->GotoDlgCtrl( GetDlgItem( IDC_BUTTON1 ));
		break;

	case BUTTON2_PN:
		((CDialog*)this)->GotoDlgCtrl( GetDlgItem( IDC_BUTTON2 ));
		break;

	case BUTTON3_PN:
		((CDialog*)this)->GotoDlgCtrl( GetDlgItem( IDC_BUTTON3 ));
		break;

	case SEQINP_PN:
		GetDlgItem( IDC_ICSDBEDTCTRL1 )->SetFocus();
		break;

	}
}

BOOL CScanView::PreTranslateMessage(MSG* pMsg) 
{
//	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F4 ) {
//		PostMessage(WM_COMMAND, MAKELONG(ID_BUTTON_DMYF4_2, 0));
//		return TRUE;
//	}

	CWnd* pwnd;
	pwnd = GetFocus();
	if( pwnd != NULL ) {
		if( pwnd->m_hWnd == m_ScList.m_hWnd ) {
			if(pMsg->message == WM_KEYDOWN ) {
				if( pMsg->wParam == VK_INSERT ) {
					PostMessage(WM_COMMAND, MAKELONG(ID_BUTTON_INSERT, 0));
					return TRUE;
				}
				else if(pMsg->wParam == VK_HOME) {
					PostMessage( WM_COMMAND, MAKELONG( ID_BUTTON_HOME, 0 ) );
					return TRUE;
				}
			}
		}
	}
	
	return ICSFormView::PreTranslateMessage(pMsg);
}



void CScanView::OnSize(UINT nType, int cx, int cy) 
{
	if( ((CMainFrame*)GetDBDinpMainFrame())->IgnoreOnSizeFlag() )
		return;

	ICSFormView::OnSize(nType, cx, cy);
	
	// TODO: この位置OnSizeにメッセージ ハンドラ用のコードを追加してください
	double w, h;
	w = h = 1.0;
//	GetRate( w, h );	//ビューサイズゲット
	set_viewrate( w, h );	//ビューサイズをAppにセーブ

	scan_ctrlmove();

	scan_onsizedisp();
}


void CScanView::scan_redisp()
{
	int n,max, seq;
	int pn;
	
	PROGRAM_OK = FALSE;
	{

		//リングバッファを更新
		for( int i = 0; i < (sizeof SCAN_ZREC / sizeof SCAN_ZREC[0]); i++ ) {
			seq = SCAN_ZREC[i].m_seq;
			if( seq > 0 ) {
				if( pDBzm->DBDATA_SetPosition( seq ) == 0 ) {
					pDBzm->GetCDBData( &SCAN_ZREC[i] );
				}
				else {
					ErrExit( 0, _T("一括再表示読み込みエラー") );
					return;
				}
			}
		}
		//画面表示サインセット
		KEYSCAN_VISIBLE = TRUE;

		//全表示データイニシャライズ
		m_ScList.RemoveAllDtListData();

		set_focus( SCANVW_PN );

		max = SCAN_LN;
		for( n = 0 ; n < max ; ++n )	//表示データ作成
		{
			if( pn = SCAN_DDATA_LINE[n].ZREC_NO )
			{
				mk_1dspdata( &SCAN_DDATA_LINE[n], &SCAN_ZREC[ pn - 1 ] );
				SCAN_DDATA_LINE[n].ZREC_NO = pn;	//SCAN_ZREC[] ポジション
			}
		}
		scan_ddata_dsp( SCAN_DDATA_LINE, n, 1 );	//データ表示

		m_ScList.RedrawDataListCtrl();

		//現在カーソル行
//		NOW_INDEX = SCAN_RD_BOTOM - ofs;
//		NowIndexDsp();				//ラインカーソル表示
//		m_ScList.SetCurrentLine( NOW_INDEX+1 );
//		waning( TRUE );	// "検索中"

		this->UpdateWindow();


	}
	PROGRAM_OK = TRUE;	

}

// OnSize でコントロールの移動
void CScanView::scan_ctrlmove()
{
	int nID[] = { 
		IDC_BUTTON1,IDC_BUTTON2,IDC_BUTTON3,IDC_STATIC1,IDC_SPIN1,
		IDC_ICSDBEDTCTRL1,IDC_STATIC2, IDC_SPIN2, 0
	};

	if( ! IsWindow( m_ScList.m_hWnd ) )
		return;

	CRect parentRect, scanRect;
	this->GetClientRect( parentRect );

	m_ScList.GetWindowRect(&scanRect);
	this->ScreenToClient( scanRect );

	for( int i = 0; nID[i] != 0; i++ ) {
		CRect childRect;

		GetDlgItem( nID[i] )->GetClientRect( childRect );
		int ht = childRect.Height();

		CRect mvRect;
		GetDlgItem( nID[i] )->GetWindowRect( mvRect );
		this->ScreenToClient( mvRect );

		SCROLLINFO	siV = {0};
		siV.cbSize = sizeof(SCROLLINFO);
		GetScrollInfo(SB_VERT, &siV );

		if( parentRect.Height() < siV.nMax ) {
			int adjust = 0;
			if( siV.nPos > 0 ) {	//スクロール位置が先頭でない時
				adjust = (siV.nPos * -1);
				mvRect.bottom = mvRect.top + (siV.nMax + (adjust - mvRect.top));
			}
			else
				mvRect.bottom = siV.nMax;
		}
		else {
			mvRect.bottom = parentRect.bottom;
		}

		mvRect.bottom -= (ht/3);
		mvRect.top = mvRect.bottom -ht;
		GetDlgItem( nID[i] )->MoveWindow( mvRect );

		if( nID[i] == IDC_SPIN1 ) {
			scanRect.bottom = mvRect.top;
		}
	}
	m_ScList.MoveWindow( &scanRect );
}


// OnSize で行数の変更 があったときの再表示
void CScanView::scan_onsizedisp()
{
	int n,max, seq, seq2;
	int newln, loop, scrol_nl, oldln;
	int	mv;
	CDBINPDataRec dta, *rec1, *rec2;
	BOOL bRedraw = FALSE;

	if(m_bIgnoreOnsizeRedisp)return;
	mv = 0;

	if( ! PROGRAM_OK )	return;

	newln = m_ScList.GetDispDataCount();
	if( newln > SCAN_RECMAX )
		newln = SCAN_RECMAX;

	if( newln == SCAN_SCROLL_NL )
		return;

	PROGRAM_OK = FALSE;
	{

		if( newln > SCAN_SCROLL_NL ) {
			// 表示行数が増えた -- 上にまだデータがある場合、表示する

			scrol_nl = newln - SCAN_SCROLL_NL;

			SCAN_SCROLL_NL = newln;

			for( loop = 0 ; loop < scrol_nl ; ++loop )
			{
				n = SCAN_DDATA_LINE[0].ZREC_NO - 1;	//SCAN_ZREC[n]

				seq = SCAN_ZREC[n].m_seq;	//そのシーク番号
				rec1 = &SCAN_ZREC[n];

				if( n > 0 )
					n--;
				else	// n == 0
					n = SCAN_RECMAX - 1;
				seq2 = SCAN_ZREC[n].m_seq;	//一つ前のシーク番号
				rec2 = &SCAN_ZREC[n];
				
				//まだ読み込まれていないZREC[n]か，リングバッファの底か
				if( seq2 <= 0 || dbip_data_cmp( SCAN_PGTBL.data_mode, rec1, rec2 ) < 0 /*seq2 > seq*/ )
				{

					if( !data_nxt_back( rec1, &dta, 1 ) )
					{
						break;
					}
					SCAN_ZREC[n] = dta;
				}
	
				SCAN_LN += 1;

				//仕訳表示データエリア１行ダウン
				scan_scroll_down( SCAN_DDATA_LINE, SCAN_SCROLL_NL, 1 );

				//１行表示
				mk_dspdata( &SCAN_DDATA_LINE[ 0 ], &SCAN_ZREC[n], n, 1 );	//表示データを作成
#ifdef BIGIMG	// 03.16 /00
				set_imgtky( &SCAN_DDATA_LINE[ 0 ], 0 );	// イメージ摘要セット
#else
				set_imgtky( &SCAN_DDATA_LINE[ 0 ] );	// イメージ摘要セット
#endif
				m_ScList.InsertDtListData( 0, &SCAN_DDATA_LINE[ 0 ].Data, FALSE );

				mv++;
			}

			if( mv ) {
				n =  m_ScList.GetCurrentLine();
				m_ScList.SetCurrentLine( n+mv );

				bRedraw = TRUE;
			}
			// まだ表示可能な行数がある
			if( mv < scrol_nl ) {
				scrol_nl = (scrol_nl - mv);
				mv = 0;
					
				for ( loop = 0 ; loop < scrol_nl ; ++loop )
				{
					n = SCAN_DDATA_LINE[SCAN_LN-1].ZREC_NO - 1;	//SCAN_ZREC[n]

					seq = SCAN_ZREC[n].m_seq;	//そのシーク番号
					rec1 = &SCAN_ZREC[n];

					if( n < SCAN_RECMAX - 1 )
						n++;
					else	// n == SCAN_RECMAX - 1
						n = 0;
					seq2 = SCAN_ZREC[n].m_seq;	//一つ先のシーク番号
					rec2 = &SCAN_ZREC[n];
 					//まだ読み込まれていないZREC[n]か，リングバッファのTOPか
					if( seq2 <= 0 || dbip_data_cmp( SCAN_PGTBL.data_mode, rec1, rec2 ) > 0 /*seq2 < seq*/ )
					{
						if( !data_nxt_for( rec1, &dta, 1 ) )
						{
							break;
						}
						SCAN_ZREC[n] = dta;
					}

					SCAN_LN++;

					mk_dspdata( &SCAN_DDATA_LINE[ SCAN_LN - 1 ], &SCAN_ZREC[n], n, 1 );	//表示データを作成
#ifdef BIGIMG	// 03.16 /00
					set_imgtky( &SCAN_DDATA_LINE[ SCAN_LN - 1], SCAN_LN - 1 );	// イメージ摘要セット
#else
					set_imgtky( &SCAN_DDATA_LINE[ SCAN_LN - 1] );	// イメージ摘要セット
#endif
					m_ScList.SetDtListData( SCAN_LN - 1, &SCAN_DDATA_LINE[ SCAN_LN - 1 ].Data, FALSE );
				}			
			}
			
			if( bRedraw )	m_ScList.RedrawDataListCtrl();
		}
		else {
			// 表示行数が減った
			scrol_nl = SCAN_SCROLL_NL - newln;
			oldln = SCAN_SCROLL_NL;
			SCAN_SCROLL_NL = newln;

			if( SCAN_LN > newln  ) {
				n = m_ScList.GetCurrentLine();

				// 現在選択ラインが見えなくなったら 上に移動
				if( n > newln ) {
					max = n - newln;

					mv = 0;

					for ( loop = 0 ; loop < max ; ++loop )
					{
						//仕訳表示データエリア１行アップ
						scan_scroll_up( SCAN_DDATA_LINE, oldln, 1 );
						
						m_ScList.RemoveDtListData( 0 );
						m_ScList.SetDtListData( newln - 1, &SCAN_DDATA_LINE[ newln - 1 ].Data, FALSE );
						
						mv++;
					}	
						
					if( mv ) {
						SCAN_LN = newln;	
						m_ScList.SetCurrentLine( n - mv );
						m_ScList.RedrawDataListCtrl();
						Set_SeqInp( n - mv -1 );
					}		
				}
				else {
					SCAN_LN -= scrol_nl;
				}
			}
		}

	}
	PROGRAM_OK = TRUE;	

}


void CScanView::OnButtonDmyf42() 
{
	BOOL bCancel;

	if( pDBzm->IsDelScan() ) {
		// 取消仕訳の検索
		m_bIgnoreOnsizeRedisp = TRUE;

		if( SearchForDelDataBack( 1, this, NULL, &bCancel ) > 0 ) {

			CDelDataDlg dlg( this );
			((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(2);

			dlg.DoModal();
			
			((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(0);
			
			// 修正したので再表示 
			if( dlg.IsDataBack() ) {
				scan_redisp();
			}
		}
		else {
			myICSMessageBox( "該当の取消仕訳がありません。", 0,0,0,this);
		}
		m_bIgnoreOnsizeRedisp = FALSE;
	}
	else {
	//	double x, y;
	//	x = y = 1.0;
	//	GetRate(x, y);

		// 修正科目入力ﾀﾞｲｱﾛｸﾞの作成
		if( pRpkeyDlg == NULL ) {
			if((pRpkeyDlg = new CRpkeyDlg(this)) == NULL ) {
				ICSExit(0, "項目入力ダイアログが作成できません！");
				return;
			}	
		}

	//	pRpkeyDlg->SetSize(-1,-1,x,y);

		int st;
		((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(2);

		m_bIgnoreOnsizeRedisp = TRUE;
		// 先頭に表示しているデータ
		int idx = SCAN_DDATA_LINE[ 0 ].ZREC_NO -1;
		CDBINPDataRec data;

		if( idx >= 0 ) {
			data = SCAN_ZREC[ idx ];
		}

		pRpkeyDlg->SetTekiyoDlgRecord( &data );

		pRpkeyDlg->m_bSwkDel = FALSE;
		st = pRpkeyDlg->DoModal();

		if( st == IDOK ) {

			// 一致する仕訳の検索
			if( SearchForIkat( 1, this, NULL, &bCancel ) > 0 ) {
				CSiwkDlg swdlg( this );
				swdlg.m_bSwkDel = pRpkeyDlg->m_bSwkDel;
	//			swdlg.SetSize( 0, 0, x, y );
				swdlg.DoModal();
				
				// 修正したので再表示 
				if( swdlg.IsCorrect() ) {
					scan_redisp();
					// 諸口貸借 を更新
					g_stotal( SYOGT, (char *)SYOG_d, (char *)SYOG_c, (char *)SG_def );
					// 現金貸借 を更新
					g_stotal( /*GNKIN*/pAUTOSEL->DCKM_CODE, (char *)GENK_d, (char *)GENK_c, (char *)GK_def );
				}
			}
			else {
				CString msg;
				if( pRpkeyDlg->m_bSwkDel ) {
					msg = "該当する取消可能な仕訳がありません。\r\n(確定仕訳は対象外です。)";
				}
				else if( pRpkeyDlg->IsConmdfyCheck() ) {
					msg = "未確定にする確定仕訳がありません。";
				}
				else {
					msg = "該当の仕訳がありません。\r\n(確定仕訳は対象外です。)";
				}

				ICSMessageBox( msg, 0,0,0,this);
			}
		}
		else if( st == -1 ) {
			ICSExit( 0, "一括修正のダイアログを表示できません！" );
		}

		((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(0);
		m_bIgnoreOnsizeRedisp = FALSE;

		// 修正科目入力ﾀﾞｲｱﾛｸﾞの破棄
		delete pRpkeyDlg;
		pRpkeyDlg = NULL;
	}
}

void CScanView::OnUpdateButtonDmyf42(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( bCONFIRM_MASTER ? FALSE : TRUE );
}


// 一括修正用の仕訳を検索
int SearchForIkat( int first, CWnd* pWnd, CMsgDlg* pMsgDlg, BOOL* bCancel )
{
	CMsgDlg* msgdlg = NULL;
ASSERT( pRpkeyDlg != NULL );
	int old_caption;

	if( pMsgDlg == NULL ) {	
		if( (msgdlg = new CMsgDlg(IDS_MSG_STRING) ) == NULL ) {
			return -1;
		}
//		msgdlg->SetResizeRate(x,y);
		msgdlg->Create(pWnd);
	}
	else {
		// 状況表示ダイアログを "検索中にする"
		msgdlg = pMsgDlg;
		old_caption = msgdlg->CaptionChange( IDS_MSG_STRING );
	}

	*bCancel = FALSE;

	MSG msg;
	int dcnt = 0;
	int pos, sort_seq, dbpos;

	if( SCAN_PGTBL.sort_id != -1 ) {
		if( first )	pos = 1;
		else {
			pos = SCMDFY_SEQ.ngetseq( pDBzm->dbdata->seq - 1 );
			pos++;
		}

		check_scan_insertquery();

		for( int n = pos; n <= SCAN_PGTBL.sort_cnt; n++ ) {
			sort_seq	= SEQ_SCMDFY[ n-1 ];
			dbpos		= MDFYDATA_SEQ.ngetseq( sort_seq-1 );

			pDBzm->dbdata->SetAbsolutePosition( dbpos );
			// 繰り返し処理のｷｬﾝｾﾙﾎﾞﾀﾝが押された場合のﾁｪｯｸ
			if( msgdlg ) {
				if( msgdlg->CheckCancelButton() ) {
					*bCancel = TRUE;
					break;
				}
			}
			if( pRpkeyDlg->datacmp( pDBzm->dbdata ) ) {
				dcnt++;
				break;
			}
		}
	}
	else {
		if( first )	pDBzm->dbdata->MoveFirst();
		else		pDBzm->dbdata->MoveNext();

		if( pDBzm->dbdata->st == 0 ) {
			do {
				// ﾒｯｾｰｼﾞ処理
				while( ::PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
				// 繰り返し処理のｷｬﾝｾﾙﾎﾞﾀﾝが押された場合のﾁｪｯｸ
				if( msgdlg ) {
					if( msgdlg->CheckCancelButton() ) {
						*bCancel = TRUE;
						break;
					}
				}
#ifdef _SPEED_CLOSE
				if( pDBzm->DBDATA_SetPosition( pDBzm->data->seq ) != 0 ) {
					dcnt = -1;
					break;
				}
#endif

				if( pRpkeyDlg->datacmp( pDBzm->dbdata ) ) {
					dcnt++;
					break;
				}

			} while(  pDBzm->dbdata->MoveNext() == 0 );

		}
	}

	if( pMsgDlg == NULL ) {
		// 繰り返し終了ﾀﾞｲｱﾛｸﾞの破棄
		msgdlg->DestroyWindow();
		delete msgdlg;
		msgdlg = NULL;
	}
	else {
		msgdlg->CaptionChange( old_caption );
	}

	if( *bCancel ) {
		if( SCAN_PGTBL.sort_id != -1 ) {
			sort_seq	= SEQ_SCMDFY[ pos-1 ];
			dbpos		= MDFYDATA_SEQ.ngetseq( sort_seq-1 );

			pDBzm->dbdata->SetAbsolutePosition( dbpos );
		}
		else {
			if( ! first )	pDBzm->dbdata->MovePrev();
		}
	}

	return dcnt;
}



// 取消復帰用の仕訳を検索
int SearchForDelDataBack( int first, CWnd* pWnd, CMsgDlg* pMsgDlg, BOOL* bCancel )
{
	CMsgDlg* msgdlg = NULL;
	int old_caption;

	if( pMsgDlg == NULL ) {	
		if( (msgdlg = new CMsgDlg(IDS_MSG_STRING) ) == NULL ) {
			return -1;
		}
		msgdlg->Create(pWnd);
	}
	else {
		// 状況表示ダイアログを "検索中にする"
		msgdlg = pMsgDlg;
		old_caption = msgdlg->CaptionChange( IDS_MSG_STRING );
	}

	*bCancel = FALSE;

	MSG msg;

	int dcnt = 0;
	int pos, sort_seq, dbpos;

	check_scan_insertquery();

	if( SCAN_PGTBL.sort_id != -1 ) {
		if( first )	pos = 1;
		else {
			pos = SCMDFY_SEQ.ngetseq( pDBzm->dbdata->seq - 1 );
			pos++;
		}

		for( int n = pos; n <= SCAN_PGTBL.sort_cnt; n++ ) {
			sort_seq	= SEQ_SCMDFY[ n-1 ];
			dbpos		= MDFYDATA_SEQ.ngetseq( sort_seq-1 );

			pDBzm->dbdata->SetAbsolutePosition( dbpos );
			// 繰り返し処理のｷｬﾝｾﾙﾎﾞﾀﾝが押された場合のﾁｪｯｸ
			if( msgdlg ) {
				if( msgdlg->CheckCancelButton() ) {
					*bCancel = TRUE;
					break;
				}
			}
			if( (pDBzm->dbdata->dsign[0] & 0x01) ) {
				dcnt++;
				break;
			}
		}
	}
	else {
		if( first )	pDBzm->dbdata->MoveFirst();
		else		pDBzm->dbdata->MoveNext();

		if( pDBzm->dbdata->st == 0 ) {
			do {
				// ﾒｯｾｰｼﾞ処理
				while( ::PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
				// 繰り返し処理のｷｬﾝｾﾙﾎﾞﾀﾝが押された場合のﾁｪｯｸ
				if( msgdlg ) {
					if( msgdlg->CheckCancelButton() ) {
						*bCancel = TRUE;
						break;
					}
				}
#ifdef _SPEED_CLOSE
				if( pDBzm->DBDATA_SetPosition( pDBzm->data->seq ) != 0 ) {
					dcnt = -1;
					break;
				}
#endif

				if( (pDBzm->dbdata->dsign[0] & 0x01) ) {
					dcnt++;
					break;
				}

			} while(  pDBzm->dbdata->MoveNext() == 0 );
		}
	}

	if( pMsgDlg == NULL ) {
		// 繰り返し終了ﾀﾞｲｱﾛｸﾞの破棄
		msgdlg->DestroyWindow();
		delete msgdlg;
		msgdlg = NULL;
	}
	else {
		msgdlg->CaptionChange( old_caption );
	}

	if( *bCancel ) {
		if( SCAN_PGTBL.sort_id != -1 ) {
			sort_seq	= SEQ_SCMDFY[ pos-1 ];
			dbpos		= MDFYDATA_SEQ.ngetseq( sort_seq-1 );

			pDBzm->dbdata->SetAbsolutePosition( dbpos );
		}
		else {
			if( ! first )	pDBzm->dbdata->MovePrev();
		}
	}

	return dcnt;
}


// F5呼び出し時の チェックリスト UCOMセット
//	 int type	０：印刷, １：ＣＳＶ
//
void CScanView::ChecklistUcomSet( int type )
{
	char buf[64], buf1[64];

	SCAN_ITEMEX *key = &SC_Item;
	int DtMode, DtOwner;

	// 表示順
	switch( (key->sc_type & 0x0f) ) {
	case SCT_ORDERSEQ:	DtMode = 0;	break;
	default:			DtMode = 1;	break;
	}
	// 対象データ
	DtOwner = GetDataOwnerSign();

	sprintf_s( buf, sizeof buf, "%d", DtMode);
	sprintf_s( buf1, sizeof buf1, "%d", DtOwner);

	UCOM_SetString_BC( CHECK_UCSTR1, buf, buf1, _T("1") );
	UCOM_SetStringAll_BC( CHECK_UCSTR2, (LPVOID)(LPCTSTR)RequeryStr );
	UCOM_SetInt_BC( CHECK_UCSTR3, type );
	UCOM_SetStringAll_BC( CHECK_UCSTR4, (LPVOID)(LPCTSTR)SCAN_PGTBL.sortOrder );

	IsCheckListPrint() = TRUE;
}


void CScanView::ForkUcomSet()
{

}


// 他業務からの戻りによる再表示
int CScanView::ReloadView()
{
FILETrace( "ReloadView TOP\n" );

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

	int i, n, max, pn;
	CDBINPDataRec tmp, key;

	for( i = 0; i < SCAN_RECMAX; i++ ) {
		if( SCAN_ZREC[i].m_seq > 0 ) {
			key = SCAN_ZREC[i];

#ifdef CLOSE // 07.31 /08
			data_for( &key, &SCAN_ZREC[i], 1 );
#else
			data_search( &key, &SCAN_ZREC[i], 1, key.m_seq );
			// 他で仕訳が取消された場合、修正サインON
			if( ! (key.m_dsign[0] & 0x01) && (SCAN_ZREC[i].m_dsign[0] & 0x01) ) {
				MODEFY_BIT.onbitn( key.m_seq - 1);
			}
#endif
		}
	}

	CString stmp;
	BOOL	bInit = FALSE;
//	int st = DB_DataScan( stmp );
	Reload_DataScan();

	pDBzm->dbdata_Reload() = FALSE;

	// 常に最初から表示
	bInit = TRUE;

#ifdef CLOSE
	for( i = 0; i < SCAN_RECMAX; i++ ) {
		if( SCAN_ZREC[i].m_seq > 0 ) {
			key = SCAN_ZREC[i];
			// 他のプログラムで 仕訳が変更されて、クエリー上からなくなった。
			if( data_search( &key, &SCAN_ZREC[i], 1, key.m_seq ) == 0 ) {
				bInit = TRUE;
				break;
			}
		}
	}
#endif

	if( bInit ) {
		if( ! ((CMainFrame*)GetDBDinpMainFrame())->IsSCANMDFY() ) {
			// 最初から再表示
			scan_init_rd( 0, 0, 0 );
		}
	}
	else {
		//画面表示
		int old_cline;
		old_cline = m_ScList.GetCurrentLine();

		m_ScList.RemoveAllDtListData();		//全表示データイニシャライズ

		max = SCAN_LN;
		for( n = 0 ; n < max ; ++n )	//表示データ作成
		{
			if( pn = SCAN_DDATA_LINE[n].ZREC_NO )
			{
				mk_1dspdata( &SCAN_DDATA_LINE[n], &SCAN_ZREC[ pn - 1 ] );
				SCAN_DDATA_LINE[n].ZREC_NO = pn;	//SCAN_ZREC[] ポジション
			}
		}
		scan_ddata_dsp( SCAN_DDATA_LINE, n, 1 );	//データ表示

		m_ScList.SetCurrentLine( old_cline );
		m_ScList.RedrawDataListCtrl();

		set_focus( SCANVW_PN );
	}

	return 0;
}

/*================================================
	スキャン科目に入力があるか？
	D0	借方
	D1	貸方
	D2　借／貸
  ================================================*/
int scnkmk_input( SCAN_ITEMEX *key )
{
	int ret = 0;
	// 借方
	if( (key->sc_dbtopt & SC_KMKHANI) ) {
		//範囲
		if( key->sc_dbt[0] || key->sc_dbt2[0] )
			ret |= 0x01;
	}
	else {
		if( key->sc_dbt[0] )
			ret |= 0x01;
	}
	// 貸方
	if( (key->sc_creopt & SC_KMKHANI) ) {
		//範囲
		if( key->sc_cre[0] || key->sc_cre2[0] )
			ret |= 0x02;
	}
	else {
		if( key->sc_cre[0] )
			ret |= 0x02;
	}
	// 借／貸
	if( (key->sc_dbcropt & SC_KMKHANI) ) {
		//範囲
		if( key->sc_dbcr[0] || key->sc_dbcr2[0] )
			ret |= 0x04;
	}
	else {
		if( key->sc_dbcr[0] )
			ret |= 0x04;
	}

	return ret;
}



void DelDataFilter( CString& filter )
{
		//取消仕訳検索
	filter = "(cast(cast(dsign as binary(1)) as int) & 0x01) = 1 AND \
((datarec.mofs = 0 and datarec.ddate between (select smd from month_info where ofset = 0 and insw = 1) and (select emd from month_info where ofset = 0 and insw = 1))\
or (datarec.mofs = 1 and datarec.ddate between (select smd from month_info where ofset = 1 and insw = 1) and (select emd from month_info where ofset = 1 and insw = 1))\
or (datarec.mofs = 2 and datarec.ddate between (select smd from month_info where ofset = 2 and insw = 1) and (select emd from month_info where ofset = 2 and insw = 1))\
or (datarec.mofs = 3 and datarec.ddate between (select smd from month_info where ofset = 3 and insw = 1) and (select emd from month_info where ofset = 3 and insw = 1))\
or (datarec.mofs = 4 and datarec.ddate between (select smd from month_info where ofset = 4 and insw = 1) and (select emd from month_info where ofset = 4 and insw = 1))\
or (datarec.mofs = 5 and datarec.ddate between (select smd from month_info where ofset = 5 and insw = 1) and (select emd from month_info where ofset = 5 and insw = 1))\
or (datarec.mofs = 6 and datarec.ddate between (select smd from month_info where ofset = 6 and insw = 1) and (select emd from month_info where ofset = 6 and insw = 1))\
or (datarec.mofs = 7 and datarec.ddate between (select smd from month_info where ofset = 7 and insw = 1) and (select emd from month_info where ofset = 7 and insw = 1))\
or (datarec.mofs = 8 and datarec.ddate between (select smd from month_info where ofset = 8 and insw = 1) and (select emd from month_info where ofset = 8 and insw = 1))\
or (datarec.mofs = 9 and datarec.ddate between (select smd from month_info where ofset = 9 and insw = 1) and (select emd from month_info where ofset = 9 and insw = 1))\
or (datarec.mofs = 10 and datarec.ddate between (select smd from month_info where ofset = 10 and insw = 1) and (select emd from month_info where ofset = 10 and insw = 1))\
or (datarec.mofs = 11 and datarec.ddate between (select smd from month_info where ofset = 11 and insw = 1) and (select emd from month_info where ofset = 11 and insw = 1))\
or (datarec.mofs = 12 and datarec.ddate between (select smd from month_info where ofset = 12 and insw = 1) and (select emd from month_info where ofset = 12 and insw = 1))\
or (datarec.mofs = 13 and datarec.ddate between (select smd from month_info where ofset = 13 and insw = 1) and (select emd from month_info where ofset = 13 and insw = 1))\
or (datarec.mofs = 14 and datarec.ddate between (select smd from month_info where ofset = 14 and insw = 1) and (select emd from month_info where ofset = 14 and insw = 1))\
or (datarec.mofs = 15 and datarec.ddate between (select smd from month_info where ofset = 15 and insw = 1) and (select emd from month_info where ofset = 15 and insw = 1)))";

//	filter = "(cast(cast(dsign as binary(1)) as int) & 0x01) = 1";
}


//重複仕訳 検索フィルター
void DupliDataFilter(CString& filter)
{
	int sgn = ((CMainFrame*)GetDBDinpMainFrame())->DupliSwk();

	CString partstr;
	if( sgn & DS_DENP_BIT ) {
		partstr = "cno";
	}
	if( BMON_MST ) {
		if( sgn & DS_BMN_BIT ) {
			if( !partstr.IsEmpty() ) partstr += ",";
			partstr += "dbmn,cbmn";
		}
	}
	if( M_KOJI ) {
		if( sgn & DS_KOJI_BIT ) {
			if( !partstr.IsEmpty() ) partstr += ",";
			partstr += "dkno,ckno";
		}
	}
	if( sgn & DS_TKY_BIT ) {
		if( !partstr.IsEmpty() ) partstr += ",";
		partstr += "tekiyo";
	}

	CString partfil;
	if( partstr.IsEmpty() ) {
		partfil = "(PARTITION BY ddate, mofs, dbt, cre, val)";
	}
	else {
		partfil.Format("(PARTITION BY ddate, mofs, dbt, cre, val, %s)", partstr);
	}
	filter.Format( "seq IN (select seq from (select seq, ddate, mofs, dbt, cre, val, count(*) over %s rcnt from datarec) WK_datarec where rcnt > 1)", partfil);
}


//Wiz連携仕訳 検索フィルター
void WizDataFilter(CString& filter)
{
	//WIZで追加仕訳分のクエリ(仕訳出自がWIZ連携)
	filter.Format("seq > %d and cast(SUBSTRING(dsign, 9, 1) as int) = 18 and cast(SUBSTRING(dsign, 10, 1) as int) = 0", START_SEQ);
}


//入力・修正仕訳検索
int datatype_filter( CString& filter )
{
	SCAN_ITEMEX *key = &SC_Item;

	if( key->sc_duser != SCD_CURUESR )
		return 0;

	if( (key->sc_dtype & 0x03) == 0 )
		return 0;

	int userno = pDBzm->DBUserNo();

	CString tmp1, tmp2, strfil;
		
	char* and	= " AND ";
	char* or	= " or ";

	if( key->sc_dtype & 0x01 ) {
		tmp1.Format( "owner = %d", userno );
	}
	if( key->sc_dtype & 0x02 ) {
		tmp2.Format( "cuser = %d", userno );
	}

	if( ! tmp1.IsEmpty() && ! tmp2.IsEmpty() ) {
		strfil.Format( "" );

		if( key->sc_stype == SCS_AND ) {
			strfil.Format( "(%s%s%s)", tmp1, and, tmp2 );
		}
		else {
			strfil.Format( "(%s%s%s)", tmp1, or, tmp2 );
		}
	}
	else if( ! tmp1.IsEmpty() ) {
		strfil = tmp1;
	}
	else {
		strfil = tmp2;
	}

	filter = strfil;

	return 1;
}


// データベース用 データスキャン
//	return	 0:
//			-1: クエリー結果に該当データなし
//
int DB_DataScan( CString& filter, WORD* dnpsgn, BOOL mkonly/*=FALSE*/ ) 
{
	SCAN_ITEMEX *key = &SC_Item;
	CString wk, wk2, wk3/*,wk4*/;
	int n, st, initsw = 0;
	int nop;
	char	valbf[20], tmp[64];
	char*	fmt = "sssssssssss9";
	CString buf, str;//, and;
	char* and = " AND ";
	char* or = " OR ";
	char* stype;

	int DtMode, DtOwner;

	// 表示順
	switch( (key->sc_type & 0x0f) ) {
	case SCT_ORDERSEQ:	DtMode = 0;	break;
	default:			DtMode = 1;	break;
	}

	if( ! pDBzm->data_authority() ) {
		// 他ユーザの仕訳の権限なし
		DtOwner = 0;
	}
	else {
		// 対象データ
		DtOwner = GetDataOwnerSign();
	}
	// 検索タイプ
	switch( key->sc_stype ) {
	case SCS_AND:	stype = and;	break;
	default:		stype = or;		break;
	}

	// マルチ で仕訳が追加されているので、これは必ず行う
//	pDBzm->dbdata_Requery() = TRUE;
//	pDBzm->DBDATA_SetPosition( 1 );

//BEGIN_PERFORMANCE

	// キー項目入力なし or 取消仕訳検索
	if( !isScanKeyInp() ) {
		// 取消仕訳は除く
		filter = /*"(cast(cast(dsign as binary(1)) as int) & 0x01) = 0"*/ "";

		if( pDBzm->IsDelScan() ) {
			DelDataFilter( filter );
		}
		else if( pDBzm->IsDupliScan() ) {
			DupliDataFilter(filter);
		}
		else if( pDBzm->IsWizScan() ) {
			WizDataFilter(filter);
#ifdef CLOSE
			MyTrace("■WIZ filter = %s\n", filter);
#endif
		}

		// filter 作成のみ
		if( mkonly )	return 0;

		pDBzm->dbdata_Requery() = TRUE;
		pDBzm->dbdata_Job()		= DBDATA_JOB_SCAN;
		pDBzm->dbdata_Sort()	= (DtMode == 0) ? DBDATA_SORT_SEQ : DBDATA_SORT_DATE;
		pDBzm->dbdata_Speed()	= FALSE;

		CString dtfil;
		if( datatype_filter( dtfil ) ) {
			QueryStringOwnerCheck( filter );
			filter += stype;
			filter += dtfil;
		}

		if( IsMasterType(MST_INSERT) ) {
			st = pDBzm->dbdata->Requery( DtOwner, 0, filter, 0, (DtMode==0) ? 3 : DtMode );
		}
		else {
			st = pDBzm->dbdata->Requery( DtOwner, 0, filter, 0, (DtMode==0) ? 0 : DtMode );
		}
#ifdef CLOSE
		MyTrace("■WIZ pDBzm->dbdata st = %d\n", st);
		MyTrace("■WIZ dbdata 先頭データ date = %d, dbt = %s, cre = %s, val = %d, tekiyo = %s\n", pDBzm->dbdata->ddate, pDBzm->dbdata->dbt, pDBzm->dbdata->cre, pDBzm->dbdata->val, pDBzm->dbdata->tekiyo);
#endif

		if( ! st ) {
			InitScanPgTBL( &SCAN_PGTBL );
			SCAN_PGTBL.data_owner	= DtOwner;
			SCAN_PGTBL.data_mode	= DtMode;
			SCAN_PGTBL.filter		= filter;
			SCAN_PGTBL.tbl_end		= 1;

			// 修正前仕訳テーブルをクリア
			SCMDFY_SEQ.fill();
		}

		return st;
	}

	filter.Empty();

	if( pDBzm->IsDelScan() ) {
		DelDataFilter( filter );
		if( ! initsw )	initsw = 1;
	}
	else if( pDBzm->IsDupliScan() ) {
		DupliDataFilter(filter);
		if( !initsw )	initsw = 1;
	}

	//
	// ＳＥＱ番号
	if( key->sc_seq1 > 0 || key->sc_seq2 > 0  )
	{
		if( key->sc_seq1 > 0 ) {
			buf.Format( "seq >= %d", key->sc_seq1 );

			if( key->sc_seq2 > 0 ) {
				buf += and;
				wk.Format( "seq <= %d", key->sc_seq2 );
				buf += wk;
			}
		}
		else {
			buf.Format( "seq <= %d", key->sc_seq2 );
		}

		if( initsw )	filter += stype;

		if( (key->sc_seqopt & SC_NOTSEARCH) )
				wk.Format( "NOT ( %s )", buf );
		else	wk.Format( "( %s )", buf );

		filter += wk;

		if( ! initsw )	initsw = 1;

	}
	//
	// 伝票番号
	if( key->sc_denp1 >= 0 || key->sc_denp2 >= 0 )
	{
		if( initsw )	filter += stype;

		if( key->sc_denp1 > 0 ) {
			buf.Format( "cno >= %d", key->sc_denp1 );

			if( key->sc_denp2 > 0 ) {
				buf += and;
				wk.Format( "cno <= %d", key->sc_denp2 );
				buf += wk;
			}
		}
		else {
			// 終了のみ
			buf.Format( "cno <= %d", key->sc_denp2 );
		}

		if( (key->sc_denpopt & SC_NOTSEARCH) )
				wk.Format( "NOT ( %s )", buf );
		else	wk.Format( "( %s )", buf );

		filter += wk;

		if( ! initsw )	initsw = 1;

	}
	// 伝票の種類
	if( dnpsgn != NULL )
		*dnpsgn = key->sc_denpopt;

	//
	// 部門番号
	// 借方 部門なし
	if( (key->sc_dbmnopt & SC_ITEMNONE) ) {
		if( initsw )	filter += stype;
		
		if( (key->sc_dbmnopt & SC_NOTSEARCH) )
				buf = "NOT (dbmn is NULL)";
		else	buf = "dbmn is NULL";

		filter += buf;

		if( ! initsw )	initsw = 1;
	}
	else {
		if( key->sc_dbmn1 >= 0 || key->sc_dbmn2 >= 0 )
		{
			if( initsw )	filter += stype;

			if( key->sc_dbmn1 > 0 ) {
				buf.Format( "dbmn >= %d", key->sc_dbmn1 );

				if( key->sc_dbmn2 > 0 ) {
					buf += and;
					wk.Format( "dbmn <= %d", key->sc_dbmn2 );
					buf += wk;
				}
			}
			else {
				// 終了のみ
				buf.Format( "dbmn <= %d", key->sc_dbmn2 );
			}

			if( (key->sc_dbmnopt & SC_NOTSEARCH) )
					wk.Format( "NOT ( %s )", buf );
			else	wk.Format( "( %s )", buf );

			filter += wk;

			if( ! initsw )	initsw = 1;

		}
	}
	// 貸方 部門なし
	if( (key->sc_cbmnopt & SC_ITEMNONE) ) {
		if( initsw )	filter += stype;

		if( (key->sc_cbmnopt & SC_NOTSEARCH) )
				buf = "NOT (cbmn is NULL)";
		else	buf = "cbmn is NULL";

		filter += buf;

		if( ! initsw )	initsw = 1;
	}
	else {
		if( key->sc_cbmn1 >= 0 || key->sc_cbmn2 >= 0 )
		{

			if( initsw )	filter += stype;

			if( key->sc_cbmn1 > 0 ) {
				buf.Format( "cbmn >= %d", key->sc_cbmn1 );

				if( key->sc_cbmn2 > 0 ) {
					buf += and;
					wk.Format( "cbmn <= %d", key->sc_cbmn2 );
					buf += wk;
				}
			}
			else {
				// 終了のみ
				buf.Format( "cbmn <= %d", key->sc_cbmn2 );
			}

			if( (key->sc_cbmnopt & SC_NOTSEARCH) )
					wk.Format( "NOT ( %s )", buf );
			else	wk.Format( "( %s )", buf );

			filter += wk;

			if( ! initsw )	initsw = 1;

		}
	}

	//
	// 貸借 部門番号
	if( !(key->sc_dbmnopt & SC_ITEMNONE) && !(key->sc_cbmnopt & SC_ITEMNONE) ) {
		if( key->sc_dcbmn1 >= 0 || key->sc_dcbmn2 >= 0 )
		{
			if( initsw )	filter += stype;

			CString dbm_str, cbm_str;

			if( key->sc_dcbmn1 > 0 ) {
				dbm_str.Format( "(dbmn >= %d", key->sc_dcbmn1 );

				if( key->sc_dcbmn2 > 0 ) {
					dbm_str += and;
					wk.Format( "dbmn <= %d)", key->sc_dcbmn2 );
					dbm_str += wk;
				}
				else {
					dbm_str += ")";
				}
			}
			else {
				// 終了のみ
				dbm_str.Format( "dbmn <= %d", key->sc_dcbmn2 );
			}

			if( key->sc_dcbmn1 > 0 ) {
				cbm_str.Format( "(cbmn >= %d", key->sc_dcbmn1 );

				if( key->sc_dcbmn2 > 0 ) {
					cbm_str += and;
					wk.Format( "cbmn <= %d)", key->sc_dcbmn2 );
					cbm_str += wk;
				}
				else {
					cbm_str += ")";
				}
			}
			else {
				// 終了のみ
				cbm_str.Format( "cbmn <= %d", key->sc_dcbmn2 );
			}

			if( (key->sc_dcbmnopt & SC_NOTSEARCH) ) {
			//	wk.Format( "NOT ( %s and %s)", dbm_str, cbm_str );

				if( ! dbm_str.IsEmpty() && ! cbm_str.IsEmpty() ) {
					wk.Format( "(NOT %s and NOT %s)", dbm_str, cbm_str );
				}
				else {
					if( dbm_str.IsEmpty() ) {
						wk.Format( "(NOT %s )", cbm_str );
					}
					else {
						wk.Format( "(NOT %s )", dbm_str );
					}
				}
			}
			else {
				wk.Format( "( %s or %s)" , dbm_str, cbm_str );
			}

			filter += wk;

			if( ! initsw )	initsw = 1;
		}
	}

	//
	// 工事番号
	// 借方 工事番号なし
	if( (key->sc_dkjiopt & SC_ITEMNONE) ) {
		if( initsw )	filter += stype;
		
		if( (key->sc_dkjiopt & SC_NOTSEARCH) )
				buf = "NOT (dkno is NULL)";
		else	buf = "dkno is NULL";

		filter += buf;

		if( ! initsw )	initsw = 1;
	}
	else {
		if( key->sc_dkji1[0] || key->sc_dkji2[0] )
		{

			if( initsw )	filter += stype;

			if( key->sc_dkji1[0] ) {
				buf.Format( "convert(varbinary(13), space( 12-len(dkno)) + dkno ) >= convert(varbinary(13), '%12s')", key->sc_dkji1 );

				if( key->sc_dkji2[0] ) {
					buf += and;
					wk.Format( "convert(varbinary(13), space( 12-len(dkno)) + dkno ) <= convert(varbinary(13), '%12s')", key->sc_dkji2 );
					buf += wk;
				}
			}
			else {
				// 終了のみ
				buf.Format( "convert(varbinary(13), space( 12-len(dkno)) + dkno ) <= convert(varbinary(13), '%12s')", key->sc_dkji2 );
			}

			if( (key->sc_dkjiopt & SC_NOTSEARCH) )
					wk.Format( "NOT ( %s )", buf );
			else	wk.Format( "( %s )", buf );

			filter += wk;

			if( ! initsw )	initsw = 1;

		}
	}
	// 貸方 工事なし
	if( (key->sc_ckjiopt & SC_ITEMNONE) ) {
		if( initsw )	filter += stype;
		
		if( (key->sc_ckjiopt & SC_NOTSEARCH) )
				buf = "NOT (ckno is NULL)";
		else	buf = "ckno is NULL";

		filter += buf;

		if( ! initsw )	initsw = 1;
	}
	else {
		if( key->sc_ckji1[0] || key->sc_ckji2[0] )
		{

			if( initsw )	filter += stype;

			//	convert(varbinary(11), RTrim(FIELD_NAME)) = convert(varbinary(11), 'abcdefghij') 
			if( key->sc_ckji1[0] ) {
				buf.Format( "convert(varbinary(13), space( 12-len(ckno)) + ckno ) >= convert(varbinary(13), '%12s')", key->sc_ckji1 );

				if( key->sc_ckji2[0] ) {
					buf += and;
					wk.Format( "convert(varbinary(13), space( 12-len(ckno)) + ckno ) <= convert(varbinary(13), '%12s')", key->sc_ckji2 );
					buf += wk;
				}
			}
			else {
				// 終了のみ
				buf.Format( "convert(varbinary(13), space( 12-len(ckno)) + ckno ) <= convert(varbinary(13), '%12s')", key->sc_ckji2 );
			}

			if( (key->sc_ckjiopt & SC_NOTSEARCH) )
					wk.Format( "NOT ( %s )", buf );
			else	wk.Format( "( %s )", buf );

			filter += wk;

			if( ! initsw )	initsw = 1;

		}
	}

	//
	// 貸借 工事番号
	if( !(key->sc_dkjiopt & SC_ITEMNONE) && !(key->sc_ckjiopt & SC_ITEMNONE) ) {
		if( key->sc_dckji1[0] || key->sc_dckji2[0] )
		{
			if( initsw )	filter += stype;

			CString dkji_str, ckji_str;

			//借方工事
			if( key->sc_dckji1[0] ) {
				dkji_str.Format( "(convert(varbinary(13), space( 12-len(dkno)) + dkno ) >= convert(varbinary(13), '%12s')", key->sc_dckji1 );

				if( key->sc_dckji2[0] ) {
					dkji_str += and;
					wk.Format( "convert(varbinary(13), space( 12-len(dkno)) + dkno ) <= convert(varbinary(13), '%12s'))", key->sc_dckji2 );
					dkji_str += wk;
				}
				else {
					dkji_str += ")";
				}
			}
			else {
				// 終了のみ
				dkji_str.Format( "convert(varbinary(13), space( 12-len(dkno)) + dkno ) <= convert(varbinary(13), '%12s')", key->sc_dckji2 );
			}

			//貸方工事
			if( key->sc_dckji1[0] ) {
				ckji_str.Format( "(convert(varbinary(13), space( 12-len(ckno)) + ckno ) >= convert(varbinary(13), '%12s')", key->sc_dckji1 );

				if( key->sc_dckji2[0] ) {
					ckji_str += and;
					wk.Format( "convert(varbinary(13), space( 12-len(ckno)) + ckno ) <= convert(varbinary(13), '%12s'))", key->sc_dckji2 );
					ckji_str += wk;
				}
				else {
					ckji_str += ")";
				}
			}
			else {
				// 終了のみ
				ckji_str.Format( "convert(varbinary(13), space( 12-len(ckno)) + ckno ) <= convert(varbinary(13), '%12s')", key->sc_dckji2 );
			}

			if( (key->sc_dckjiopt & SC_NOTSEARCH) ) {
//				wk.Format( "NOT ( %s and %s)", dkji_str, ckji_str );
				wk.Format( "NOT ( %s and %s)", dkji_str, ckji_str );

				if( ! dkji_str.IsEmpty() && ! ckji_str.IsEmpty() ) {
					wk.Format( "(NOT %s and NOT %s)", dkji_str, ckji_str);
				}
				else {
					if( dkji_str.IsEmpty() ) {
						wk.Format( "(NOT %s )", ckji_str );
					}
					else {
						wk.Format( "(NOT %s )", dkji_str );
					}
				}
			}
			else
				wk.Format( "( %s or %s )" , dkji_str, ckji_str );

			filter += wk;

			if( ! initsw )	initsw = 1;
		}
	}

	//
	// 借方、貸方、貸借　科目
	if( key->sc_dbt[0] || key->sc_cre[0]|| key->sc_dbcr[0] || 
		key->sc_dbt2[0] || key->sc_cre2[0]|| key->sc_dbcr2[0] )
	{
		wk	= key->sc_dbt[0]	? key->sc_dbt	: "";
		wk2 = key->sc_cre[0]	? key->sc_cre	: "";
		wk3 = key->sc_dbcr[0]	? key->sc_dbcr	: "";

		CString strw1, strw2, strw3a, strw3b, brstr, karikasi;
		CString	e_cd, e_cd2, e_cd3, work;
		int	setsgn;
		// 終了科目(範囲)コード
		e_cd	= key->sc_dbt2[0]	? key->sc_dbt2	: "";
		e_cd2	= key->sc_cre2[0]	? key->sc_cre2	: "";
		e_cd3	= key->sc_dbcr2[0]	? key->sc_dbcr2	: "";

		strw1.Empty();
		strw2.Empty();

		// 借方
		setsgn = 0;
		if( (key->sc_dbtopt & SC_KMKHANI) ) {
			//範囲
			work.Empty();

			if( ! wk.IsEmpty() && !e_cd.IsEmpty() ) {
				work.Format( "(select knseq from knrec where kncod = dbt) >= (select knseq from knrec where kncod = '%s')\
 and (select knseq from knrec where kncod = dbt) <= (select knseq from knrec where kncod = '%s')", wk, e_cd );
			}
			else if( wk.IsEmpty() && !e_cd.IsEmpty() ) {
				work.Format( "(select knseq from knrec where kncod = dbt) <= (select knseq from knrec where kncod = '%s')", e_cd );
			}
			else if( ! wk.IsEmpty() && e_cd.IsEmpty() ) {
				work.Format( "(select knseq from knrec where kncod = dbt) >= (select knseq from knrec where kncod = '%s')", wk );
			}

			if( ! work.IsEmpty() ) {
				if( (key->sc_dbtopt & SC_NOTSEARCH) )
					strw1.Format( "NOT( %s )", work ); 
				else
					strw1 = work;

				setsgn = 1;
			}
		}
		else {
			if( !wk.IsEmpty() ) {

				if( (key->sc_dbtopt & SC_NOTSEARCH) )
					strw1.Format( "NOT dbt = '%s'", wk ); 
				else
					strw1.Format( "dbt = '%s'", wk );

				setsgn = 1;
			}
		}

		brstr.Empty();

		if( (key->sc_dbropt & SC_ITEMNONE) ) {
			if( setsgn )	strw1 += and;
			brstr = "dbr is NULL";
			work.Format( "(%s%s)", strw1, brstr );
			strw1 = work;
		}
		else if( key->sc_dbr1 >= 0 || key->sc_dbr2 >= 0 ) {
			if( setsgn )	strw1 += and;

			brnquery_str( key, brstr, 0 );

			strw1 += brstr;
		}

		// 貸方
		setsgn = 0;
		if( (key->sc_creopt & SC_KMKHANI) ) {
			//範囲
			work.Empty();
			if( ! wk2.IsEmpty() && !e_cd2.IsEmpty() ) {
				work.Format( "(select knseq from knrec where kncod = cre) >= (select knseq from knrec where kncod = '%s')\
 and (select knseq from knrec where kncod = cre) <= (select knseq from knrec where kncod = '%s')", wk2, e_cd2 );
			}
			else if( wk2.IsEmpty() && !e_cd2.IsEmpty() ) {
				work.Format( "(select knseq from knrec where kncod = cre) <= (select knseq from knrec where kncod = '%s')", e_cd2 );
			}
			else if( ! wk2.IsEmpty() && e_cd2.IsEmpty() ) {
				work.Format( "(select knseq from knrec where kncod = cre) >= (select knseq from knrec where kncod = '%s')", wk2 );
			}

			if( ! work.IsEmpty() ) {
				if( (key->sc_creopt & SC_NOTSEARCH) )
					strw2.Format( "NOT( %s )", work ); 
				else
					strw2 = work;

				setsgn = 1;
			}
		}
		else {

			if( !wk2.IsEmpty() ) {

				if( (key->sc_creopt & SC_NOTSEARCH) )
					strw2.Format( "NOT cre = '%s'", wk2 ); 
				else
					strw2.Format( "cre = '%s'", wk2 );

				setsgn = 1;
			}
		}
		brstr.Empty();

		if( (key->sc_cbropt & SC_ITEMNONE) ) {
			if( setsgn )	strw2 += and;
			brstr = "cbr is NULL";

			work.Format( "(%s%s)", strw2, brstr );
			strw2 = work;
		}
		else if( key->sc_cbr1 >= 0 || key->sc_cbr2 >= 0 ) {
			if( setsgn )	strw2 += and;

			brnquery_str( key, brstr, 1 );

			strw2 += brstr;
		}
		// 借／貸
		brstr.Empty();

		setsgn = 0;
		if( (key->sc_dbcropt & SC_KMKHANI) ) {

			// 借　方
			work.Empty();
			if( ! wk3.IsEmpty() && !e_cd3.IsEmpty() ) {
				work.Format( "(select knseq from knrec where kncod = dbt) >= (select knseq from knrec where kncod = '%s')\
 and (select knseq from knrec where kncod = dbt) <= (select knseq from knrec where kncod = '%s')", wk3, e_cd3 );
			}
			else if( wk3.IsEmpty() && !e_cd3.IsEmpty() ) {
				work.Format( "(select knseq from knrec where kncod = dbt) <= (select knseq from knrec where kncod = '%s')", e_cd3 );
			}
			else if( ! wk3.IsEmpty() && e_cd3.IsEmpty() ) {
				work.Format( "(select knseq from knrec where kncod = dbt) >= (select knseq from knrec where kncod = '%s')", wk3 );
			}

			if( ! work.IsEmpty() ) {
				if( (key->sc_dbcropt & SC_NOTSEARCH) )
					strw3a.Format( "NOT( %s )", work ); 
				else
					strw3a = work;

				if( key->sc_dcbrn1 >= 0 || key->sc_dcbrn2 >= 0 ) {
					strw3a += and;
					brnquery_str( key, brstr, 2 );
					strw3a += brstr;
				}

				setsgn = 1;
			}

			// 貸　方
			work.Empty();
			if( ! wk3.IsEmpty() && !e_cd3.IsEmpty() ) {
				work.Format( "(select knseq from knrec where kncod = cre) >= (select knseq from knrec where kncod = '%s')\
 and (select knseq from knrec where kncod = cre) <= (select knseq from knrec where kncod = '%s')", wk3, e_cd3 );
			}
			else if( wk3.IsEmpty() && !e_cd3.IsEmpty() ) {
				work.Format( "(select knseq from knrec where kncod = cre) <= (select knseq from knrec where kncod = '%s')", e_cd3 );
			}
			else if( ! wk3.IsEmpty() && e_cd3.IsEmpty() ) {
				work.Format( "(select knseq from knrec where kncod = cre) >= (select knseq from knrec where kncod = '%s')", wk3 );
			}

			if( ! work.IsEmpty() ) {
				if( (key->sc_dbcropt & SC_NOTSEARCH) )
					strw3b.Format( "NOT( %s )", work ); 
				else
					strw3b = work;

				if( key->sc_dcbrn1 >= 0 || key->sc_dcbrn2 >= 0 ) {
					strw3b += and;
					brnquery_str( key, brstr, 3 );
					strw3b += brstr;
				}

				setsgn = 1;
			}

			if( setsgn ) {
				if( (key->sc_dbcropt & SC_NOTSEARCH) )
					karikasi.Format( "( %s and %s)", strw3a, strw3b );
				else
					karikasi.Format( "( %s or %s)" , strw3a, strw3b );
			}
		}
		else {
			if( ! wk3.IsEmpty() ) {
				// 借　方
				if( (key->sc_dbcropt & SC_NOTSEARCH) ) {
					strw3a.Format( "NOT dbt = '%s'", wk3 );
				}
				else {
					strw3a.Format( "dbt = '%s'", wk3 );
				}

				BOOL brn_qy_sgn = FALSE;
				if( (key->sc_dbropt & SC_ITEMNONE) || (key->sc_cbropt & SC_ITEMNONE) )
					brn_qy_sgn = TRUE;

				if( ! brn_qy_sgn ) {
					if( key->sc_dcbrn1 >= 0 || key->sc_dcbrn2 >= 0 ) {
						strw3a += and;
						brnquery_str( key, brstr, 2 );
						strw3a += brstr;
					}
				}

				// 貸　方
				if( (key->sc_dbcropt & SC_NOTSEARCH) ) {
					strw3b.Format( "NOT cre = '%s'", wk3 );
				}
				else {
					strw3b.Format( "cre = '%s'", wk3 );
				}

				if( ! brn_qy_sgn ) {
					if( key->sc_dcbrn1 >= 0 || key->sc_dcbrn2 >= 0 ) {
						strw3b += and;
						brnquery_str( key, brstr, 3 );
						strw3b += brstr;
					}
				}

				if( (key->sc_dbcropt & SC_NOTSEARCH) )
					karikasi.Format( "( %s and %s)", strw3a, strw3b );
				else
					karikasi.Format( "( %s or %s)" , strw3a, strw3b );
			}
		}

		if( ! strw1.IsEmpty() ) {
			if( initsw )	filter += stype;

			if( key->sc_stype != SCS_AND ) {
				buf.Format( "( %s )", strw1 );
			}
			else {
				buf = strw1;
			}

			filter += buf;
			if( !initsw )	initsw = 1;
		}

		if( ! strw2.IsEmpty() ) {
			if( initsw )	filter += stype;

			if( key->sc_stype != SCS_AND ) {
				buf.Format( "( %s )", strw2 );
			}
			else {
				buf = strw2;
			}
			filter += buf;

			if( !initsw )	initsw = 1;
		}
		// 借／貸
		if( ! karikasi.IsEmpty() ) {
			if( initsw )	filter += stype;
			filter += karikasi;
			if( !initsw )	initsw = 1;
		}
	}
	//
	// 借方、貸方	枝番
	// 枝番なし check ON ... 02.26 /03
	if( (key->sc_dbropt & SC_ITEMNONE) || (key->sc_cbropt & SC_ITEMNONE) ) {
		if( (key->sc_dbropt & SC_ITEMNONE) ) {	// 借方枝番なし
			if( initsw )	filter += stype;

			if( (key->sc_dbropt & SC_NOTSEARCH) )
					buf = "NOT (dbr is NULL)";
			else	buf = "dbr is NULL";

			filter += buf;

			if( ! initsw )	initsw = 1;
		}
		if( (key->sc_cbropt & SC_ITEMNONE) ) {	// 貸方枝番なし
			if( initsw )	filter += stype;

			if( (key->sc_cbropt & SC_NOTSEARCH) )
					buf = "NOT (cbr is NULL)";
			else	buf = "cbr is NULL";

			filter += buf;

			if( ! initsw )	initsw = 1;
		}
	}
	// 枝番チェック
	if( key->sc_dbr1 >= 0 || key->sc_cbr1 >= 0 || key->sc_dcbrn1 >= 0 ||
		key->sc_dbr2 >= 0 || key->sc_cbr2 >= 0 || key->sc_dcbrn2 >= 0 )
	{
		nop = 0;
		char	deb_brchk, cre_brchk;
		deb_brchk = cre_brchk = 1;
		// 検索科目入力チェック
		int kmkinp = scnkmk_input( key );

		// 借方科目なし		ありの場合は、科目で処理済
		if( ! (kmkinp&0x01) )
			deb_brchk = (key->sc_dbropt & SC_ITEMNONE) ? 1 : 0;
		// 貸方科目なし		ありの場合は、科目で処理済
		if( ! (kmkinp&0x02) )
			cre_brchk = (key->sc_cbropt & SC_ITEMNONE) ? 1 : 0;

		int wk_eda, wk2_eda;

		wk_eda = wk2_eda = -1;
		if( key->sc_dcbrn1 >= 0 )
			wk_eda = key->sc_dcbrn1;
		if( key->sc_dcbrn2 >= 0 )
			wk2_eda = key->sc_dcbrn2;

		wk.Empty();
		str.Empty();

		if( ! deb_brchk ) { // 借方枝番を検索

			if( key->sc_dbr1 >= 0 ) {
				wk.Format( "dbr >= %0d", key->sc_dbr1 );
				str = wk;
			}
			if( key->sc_dbr2 >= 0 ) {
				wk.Format( "dbr <= %0d", key->sc_dbr2 );

				if( ! str.IsEmpty() ) {
					str += and;
					str += wk;
				}
				else	str = wk;
			}

			if( ! str.IsEmpty() ) {
				if( (key->sc_dbropt & SC_NOTSEARCH) )	// NOT 
					buf.Format( "NOT ( %s )", str );
				else
					buf.Format( "( %s )", str );

				if( initsw )	filter += stype;
				filter += buf;
				if( ! initsw )	initsw = 1;
			}
		}
		wk.Empty();
		str.Empty();
		// 貸方枝番
		if( ! cre_brchk ) {
			if( key->sc_cbr1 >= 0 ) {
				wk.Format( "cbr >= %0d", key->sc_cbr1 );
				str = wk;
			}
			if( key->sc_cbr2 >= 0 ) {
				wk.Format( "cbr <= %0d", key->sc_cbr2 );

				if( ! str.IsEmpty() ) {
					str += and;
					str += wk;
				}
				else	str = wk;
			}

			if( ! str.IsEmpty() ) {
				if( (key->sc_cbropt & SC_NOTSEARCH) )	// NOT 
					buf.Format( "NOT ( %s )", str );
				else
					buf.Format( "( %s )", str );

				if( initsw )	filter += stype;
				filter += buf;
				if( ! initsw )	initsw = 1;
			}
		}

		wk.Empty();
		str.Empty();

		// 借方／貸方 科目入力なし で 借方・貸方 枝番なし
		if( !(kmkinp&0x04) && (!deb_brchk && !cre_brchk) ) {
			CString filter_dbt, filter_cre;

			if( wk_eda >= 0 ) {
				wk.Format( "dbr >= %0d", wk_eda );
				filter_dbt = wk;
			}
			if( wk2_eda >= 0 ) {
				wk.Format( "dbr <= %0d", wk2_eda );

				if( ! filter_dbt.IsEmpty() ) {
					wk3 = filter_dbt;

					wk3 += and;
					wk3 += wk;
					// ( ) でくくる
					filter_dbt.Format( "( %s )", wk3 );
				}
				else	filter_dbt = wk;
			}

			if( wk_eda >= 0 ) {
				wk.Format( "cbr >= %0d", wk_eda );
				filter_cre = wk;
			}
			if( wk2_eda >= 0 ) {
				wk.Format( "cbr <= %0d", wk2_eda );

				if( ! filter_cre.IsEmpty() ) {
					wk3 = filter_cre;

					wk3 += and;
					wk3 += wk;
					// ( ) でくくる
					filter_cre.Format( "( %s )", wk3 );
				}
				else	filter_cre = wk;
			}

			buf.Empty();

			if( ! filter_dbt.IsEmpty() && ! filter_cre.IsEmpty() ) {
				if( (key->sc_dcbropt & SC_NOTSEARCH) )	// NOT 
					buf.Format( "(NOT %s or NOT %s)", filter_dbt, filter_cre );
				else
					buf.Format( "( %s or %s )", filter_dbt, filter_cre );
			}
			else {
				if( ! filter_dbt.IsEmpty() || ! filter_cre.IsEmpty() ) {
					if( ! filter_dbt.IsEmpty() )	str = filter_dbt;
					else							str = filter_cre;

					if( (key->sc_dcbropt & SC_NOTSEARCH) )	// NOT 
						buf.Format( "NOT ( %s )", str );
					else
						buf.Format( "( %s )", str );
				}
			}

			if( ! buf.IsEmpty() ) {

				if( initsw )	filter += stype;
				filter += buf;
				if( ! initsw )	initsw = 1;
			}
		}
	}
	
	//
	// 金額
	if( key->sc_valsgn[0] || key->sc_valsgn[1] )
	{
		l_defn( _BIN );

		if( initsw )	filter += stype;

		if( key->sc_valsgn[0] ) {
			l_print( valbf, key->sc_val1, fmt );
			buf.Format( "val >= %s", valbf );

			if( key->sc_valsgn[1] ) {
				buf += and;
				l_print( valbf, key->sc_val2, fmt );
				wk.Format( "val <= %s", valbf );
				buf += wk;
			}
		}
		else {
			// 終了のみ
			l_print( valbf, key->sc_val2, fmt );
			buf.Format( "val <= %s", valbf );
		}

		if( (key->sc_valopt & SC_NOTSEARCH) )
				wk.Format( "NOT ( %s )", buf );
		else	wk.Format( "( %s )", buf );

		filter += wk;

		if( ! initsw )	initsw = 1;
	}

	//
	// 税額
	if( key->sc_zeisgn[0] || key->sc_zeisgn[1] )
	{
		l_defn( _BIN );

		if( initsw )	filter += stype;

		if( key->sc_zeisgn[0] ) {
			l_print( valbf, key->sc_zei1, fmt );
			buf.Format( "zei >= %s", valbf );

			if( key->sc_zeisgn[1] ) {
				buf += and;
				l_print( valbf, key->sc_zei2, fmt );
				wk.Format( "zei <= %s", valbf );
				buf += wk;
			}
		}
		else {
			// 終了のみ
			l_print( valbf, key->sc_zei2, fmt );
			wk.Format( "zei <= %s", valbf );
		}

		if( (key->sc_zeiopt & SC_NOTSEARCH) )
				wk.Format( "NOT ( %s )", buf );
		else	wk.Format( "( %s )", buf );

		filter += wk;

		if( ! initsw )	initsw = 1;

	}
	//
	// 対価
	if( key->sc_taisgn[0] || key->sc_taisgn[1] )
	{
		l_defn( _BIN );

		if( initsw )	filter += stype;

		CString syzfil;
		syzfil = "( cast (SUBSTRING( dsign, 1, 1 ) as int) & 0x40 = 0x40 AND cast (SUBSTRING( dsign, 3, 1 ) as int) & 0xf0 = 0x30 ) AND ";
		filter += syzfil;

		if( key->sc_taisgn[0] ) {
			l_print( valbf, key->sc_tai1, fmt );
			buf.Format( "taika >= %s", valbf );

			if( key->sc_taisgn[1] ) {
				buf += and;
				l_print( valbf, key->sc_tai2, fmt );
				wk.Format( "taika <= %s", valbf );
				buf += wk;
			}
		}
		else {
			// 終了のみ
			l_print( valbf, key->sc_tai2, fmt );
			wk.Format( "taika <= %s", valbf );
			buf = wk;
		}

		if( (key->sc_taiopt & SC_NOTSEARCH) )
				wk.Format( "NOT ( %s )", buf );
		else	wk.Format( "( %s )", buf );

		filter += wk;

		if( ! initsw )	initsw = 1;
	}

	// 消費税仕訳区分
	if( key->sc_syzsw > NON_SELECT )
	{
		buf = "cast (SUBSTRING( dsign, 5, 1 ) as int) & 0xff = ";

		sprintf_s( tmp, sizeof tmp, "%d", key->sc_syzsw );

		if( initsw )	filter += stype;
		buf += tmp;

		wk2.Empty();
		// 科目の属性も検索
		switch( key->sc_syzsw ) {
		case KASISON:	wk2 = "0x0803";		break;
		case KASISYU:	wk2 = "0x0205";		break;
		case HAB_SYUN:	wk2 = "0x0402";		break;
		case HAB_HIYO:	wk2 = "0x0206";		break;
		case SHIWARI:	wk2 = "0x0403";		break;
		case URIWARI:	wk2 = "0x0207";		break;
		// 輸出
		case H_EXPORT:		wk2 = "0x0208";		break;
		case MEN_EXPORT:	wk2 = "0x0209";		break;
		}

		if( ! wk2.IsEmpty() ) {
			if( key->sc_syzsw == MEN_EXPORT ) { //輸出免税 税率 ゼロのみ [04.26 /12]
				wk.Format( "( %s or ( cast (SUBSTRING( dsign, 2, 1 ) as int) & 0x0f = 0x02 AND\
 (SUBSTRING( (select knsgn from knrec where knrec.kncod = datarec.dbt), 1, 2 ) = %s or\
 SUBSTRING( (select knsgn from knrec where knrec.kncod = datarec.cre), 1, 2 ) = %s )))", buf, wk2, wk2 );
			}
			else {
				wk.Format( "( %s or SUBSTRING( (select knsgn from knrec where knrec.kncod = datarec.dbt), 1, 2 ) = %s or\
 SUBSTRING( (select knsgn from knrec where knrec.kncod = datarec.cre), 1, 2 ) = %s )", buf, wk2, wk2 );
			}
		}
		else {
			wk = buf;
		}

		if( (key->sc_syzswopt & SC_NOTSEARCH) )	// NOT 
			filter += "NOT ";
		filter += wk;
		if( ! initsw )	initsw = 1;
	}
	// 税率
	if( key->sc_zritu > NON_SELECT )
	{
		CString syz1, syz2, syzsbn1, syzsbn2;
		pDBzm->SyzKariCode( syz1, syz2 );

		syzsbn1 = syz1.Left(6);
		syzsbn2 = syz2.Left(6);

		buf.Format( "( (( cast (SUBSTRING( dsign, 1, 1 ) as int) & 0x40 = 0x40 AND cast (SUBSTRING( dsign, 3, 1 ) as int) & 0x0f = 0x01) OR substring(dbt,1,6) like '%s' OR substring(cre,1,6) like '%s' OR substring(dbt,1,6) like '%s' OR substring(cre,1,6) like '%s')", syzsbn1,syzsbn1, syzsbn2,syzsbn2);
		buf += " AND cast (SUBSTRING( dsign, 2, 1 ) as int) & 0x0f = ";

		switch( key->sc_zritu )
		{
		case ZR_10:	strcpy_s( tmp, sizeof tmp, "0x05 )" );	break;
		case ZR_8K:	strcpy_s( tmp, sizeof tmp, "0x04 )" );	break;
		case ZR_8:	strcpy_s( tmp, sizeof tmp, "0x03 )" );	break;
		case ZR_5:	strcpy_s( tmp, sizeof tmp, "0x00 )" );	break;
		case ZR_3:	strcpy_s( tmp, sizeof tmp, "0x01 )" );	break;
		default:
			strcpy_s( tmp, sizeof tmp, "0x02 )" );	break;
			break;
		}
		if( initsw )	filter += stype;
		buf += tmp;
		if( (key->sc_zrituopt & SC_NOTSEARCH) )	// NOT 
			filter += "NOT ";
		filter += buf;
		if( ! initsw )	initsw = 1;
	}
	// 消費税 区分
	if( key->sc_skbn > NON_SELECT )
	{
		if( (key->sc_skbnopt & SC_NOTSEARCH) )	// NOT 
			buf = "NOT ( cast (SUBSTRING( dsign, 1, 1 ) as int) & 0x40 = 0x40 AND ";
		else
			buf = "( cast (SUBSTRING( dsign, 1, 1 ) as int) & 0x40 = 0x40 AND ";

		if( key->sc_skbn == SK_HIKZ || key->sc_skbn == SK_HUKA ) {
			buf += "cast (SUBSTRING( dsign, 3, 1 ) as int) & 0x0f = ";
		}
		else {
			buf += "cast (SUBSTRING( dsign, 3, 1 ) as int) & 0x0f = 0x01 AND cast (SUBSTRING( dsign, 2, 1 ) as int) & 0xf0 = ";
		}

		switch( key->sc_skbn )
		{
		case SK_SOTO:	strcpy_s( tmp, sizeof tmp, "0x00 )" );	break;
		case SK_UCHI:	strcpy_s( tmp, sizeof tmp, "0x10 )" );	break;
		case SK_BETU:	strcpy_s( tmp, sizeof tmp, "0x20 )" );	break;
		case SK_HIKZ:	strcpy_s( tmp, sizeof tmp, "0x00 )" );	break;
		case SK_HUKA:
		default:
						strcpy_s( tmp, sizeof tmp, "0x02 )" );	break;
		}
		if( initsw )	filter += stype;
		buf += tmp;

		filter += buf;
		if( ! initsw )	initsw = 1;
	}

	// 消費税科目属性検索
	if( (key->sc_skbnopt & SC_KMKHANI) ) {
		if( initsw )	filter += stype;
		buf = "seq in(select * from szchk_seq_ret(0x1F))";

		filter += buf;
		if( ! initsw )	initsw = 1;
	}

	// 売上／仕入 区分
	if( key->sc_urisire > NON_SELECT )
	{
		int tp = is_symstyp();
		::ZeroMemory( tmp, sizeof tmp );
		if( tp == KANI_SYMST ) {
#ifdef OLD_CLOSE
			buf = "( cast (SUBSTRING( dsign, 4, 1 ) as int) & 0x0f = ";
			switch( key->sc_urisire )
			{
			case GY_OROSI:	strcpy_s( tmp, sizeof tmp, "0x01 )" );	break;
			case GY_KOURI:	strcpy_s( tmp, sizeof tmp, "0x02 )" );	break;
			case GY_SEIZO:	strcpy_s( tmp, sizeof tmp, "0x03 )" );	break;
			case GY_ETC:	strcpy_s( tmp, sizeof tmp, "0x04 )" );	break;
			case GY_SRVCE:
			default:
				strcpy_s( tmp, sizeof tmp, "0x05 )" );
				break;
			}
#else
//			buf.Format( "seq in (select * from urikub_seq_ret( %d ))", key->sc_urisire );

			//urikub_seq_ret では仮受分が取得できないため 一時テーブル作成し、そこで合わせる
			CString tmpDBtbl, Sql;
			//	tmpDBtbl.Format("##WORK_SEQ_%d", sid);
			tmpDBtbl = GetTempTblName("##URIWORK_SEQ", NULL);

			pDBzm->m_database->BeginTrans();
			try {
				Sql.Format("IF OBJECT_ID( N'[tempDB]..[%s]', N'U' ) IS NOT NULL BEGIN DROP TABLE [%s];  END", tmpDBtbl, tmpDBtbl);
				pDBzm->m_database->ExecuteSQL(Sql);

				Sql.Format("CREATE TABLE [%s]( wkseq INT ) ", tmpDBtbl);
				pDBzm->m_database->ExecuteSQL(Sql);

				//仮受仮払の仕訳区分
				if( (Voln1->s_sgn9 & 0x01) ) {
					//urikub_seq_ret + 仮受分
					Sql.Format("insert into %s select * from urikub_seq_ret(%d) union \
select seq from datarec where((dbt = (select SVKcod2 from szvol)) or (cre = (select SVKcod2 from szvol))) and (cast(SUBSTRING(dsign, 4, 1) as int) & 0x0f = %d)",
tmpDBtbl, key->sc_urisire, key->sc_urisire);

					pDBzm->m_database->ExecuteSQL(Sql);
				}
				else {
					Sql.Format("insert into %s select * from urikub_seq_ret(%d)", tmpDBtbl, key->sc_urisire );
					pDBzm->m_database->ExecuteSQL(Sql);
				}
			}
			catch( CDBException* dbe ) {	// SQL自体のエラー
				CString strError = dbe->m_strError;
				dbe->Delete();
				ermset(0, (char*)(LPCTSTR)strError);
				Sql.Format("IF OBJECT_ID('tempdb..[%s]', 'u') IS NOT NULL DROP TABLE [%s] ", tmpDBtbl, tmpDBtbl);
				pDBzm->m_database->ExecuteSQL(Sql);
				pDBzm->m_database->CommitTrans();
				return -1;
			}
			pDBzm->m_database->CommitTrans();

			buf.Format("seq in (select wkseq from %s)", tmpDBtbl);
#endif
		}
		else if( tp == KOBET_SYMST ) {
#ifdef OLD_CLOSE
			buf = "( cast (SUBSTRING( dsign, 4, 1 ) as int) & 0xf0 = ";
			switch( key->sc_urisire )
			{
			case URI_KAZEI:		strcpy_s( tmp, sizeof tmp, "0x10 )" );	break;
			case URI_HIKAZEI:	strcpy_s( tmp, sizeof tmp, "0x20 )" );	break;
			case URI_KYOTU:
			default:
				strcpy_s( tmp, sizeof tmp, "0x30 )" );
				break;
			}
#else
 #ifndef KOBETSU_SEQ_RET
			int k_ret = 0;
			int zero_kbn = 0; //サインがついていないものも検索
			::ZeroMemory(tmp, sizeof tmp);
			switch( key->sc_urisire )
			{
			case URI_KAZEI:		strcpy_s(tmp, sizeof tmp, "0x10");	break;
			case URI_HIKAZEI:	strcpy_s(tmp, sizeof tmp, "0x20");	break;
			case URI_KYOTU:		strcpy_s(tmp, sizeof tmp, "0x30");	break;
			case URI_KIHON:
				//基本設定より
				switch( (Voln1->s_sgn6 & 0x0f) ) {
				case 1:	strcpy_s(tmp, sizeof tmp, "0x10");	break;
				case 2:	strcpy_s(tmp, sizeof tmp, "0x20");	break;
				case 3:	strcpy_s(tmp, sizeof tmp, "0x30");	break;
				default:
					strcpy_s(tmp, sizeof tmp, "0x10");	break;
					break;
				}
				zero_kbn = 1;
				break;
			default:
				break;
			}

			switch (key->sc_urisire) {
			case URI_KAZEI:
			case URI_HIKAZEI:
			case URI_KYOTU:
				k_ret = key->sc_urisire;
				break;
			case URI_KIHON:
				k_ret = 0;
				break;
			case URI_TAISYO:
				k_ret = 254;
				break;
			}

			//基本設定より(設定変更で、サインがついていないものも検索)
			switch( (Voln1->s_sgn6 & 0x0f) ) {
			case 1:
				if( key->sc_urisire == URI_KAZEI ) {
					zero_kbn = 1;
				}
				break;
			case 2:
				if( key->sc_urisire == URI_HIKAZEI ) {
					zero_kbn = 1;
				}
				break;
			case 3:
				if( key->sc_urisire == URI_KYOTU ) {
					zero_kbn = 1;
				}
				break;
			}

			//kobetsu_seq_ret では、輸入・特定課税仕入などの仕訳区分のものが含まれないので、
			//seq の一時テーブルを作成し、そこに検索に合致する仕訳のseqをセットする
			//グローバル一時テーブルに変更(チェックリスト印刷時にエラーになった)
			CString tmpDBtbl, Sql;
		//	tmpDBtbl.Format("##WORK_SEQ_%d", sid);
			tmpDBtbl = GetTempTblName("##WORK_SEQ", NULL);

			pDBzm->m_database->BeginTrans();
			try {
				Sql.Format("IF OBJECT_ID( N'[tempDB]..[%s]', N'U' ) IS NOT NULL BEGIN DROP TABLE [%s];  END", tmpDBtbl, tmpDBtbl);
				pDBzm->m_database->ExecuteSQL(Sql);

				Sql.Format("CREATE TABLE [%s]( wkseq INT ) ", tmpDBtbl);
				pDBzm->m_database->ExecuteSQL(Sql);

#ifdef CLOSE
				if( tmp[0] ) {
					Sql.Format("insert into %s select * from kobetsu_seq_ret(%d) union \
 select seq from datarec where(((dbt = (select SVKcod1 from szvol)) or (cre = (select SVKcod1 from szvol))) and (cast(SUBSTRING(dsign, 4, 1) as int) & 0xf0 = %s))", tmpDBtbl, k_ret, tmp);
				}
				else{
					Sql.Format("insert into %s select * from kobetsu_seq_ret(%d)", tmpDBtbl, k_ret );
				}
#endif
				Sql.Format("insert into %s select * from kobetsu_seq_ret(%d)", tmpDBtbl, k_ret);
				pDBzm->m_database->ExecuteSQL(Sql);
//				::ZeroMemory(tmp, sizeof tmp);

				//一時テーブルから、対象外の仕訳区分(売上・貸倒損失など)の仕訳を削除する
				Sql.Format("delete from %s where wkseq in (select seq from datarec where seq in \
(select* from kobetsu_seq_ret(%d)) and cast(SUBSTRING(dsign, 5, 1) as int) != 0 and \
 not( (cast(SUBSTRING(dsign, 5, 1) as int) = 4 or cast(SUBSTRING(dsign, 5, 1) as int) = 29 or cast(SUBSTRING(dsign, 5, 1) as int) = 30 or\
      (cast(SUBSTRING(dsign, 5, 1) as int) >= 31 and cast(SUBSTRING(dsign, 5, 1) as int) <= 34))))", tmpDBtbl, k_ret);

				pDBzm->m_database->ExecuteSQL(Sql);

				//仮受仮払の仕訳区分
				if( (Voln1->s_sgn9 & 0x01) ) {
					if( key->sc_urisire == URI_TAISYO ) {
						Sql.Format("insert into %s select seq from datarec where (dbt = (select SVKcod1 from szvol) or cre = (select SVKcod1 from szvol)) and NOT (cast(SUBSTRING(dsign, 5, 1) as int) = 0x1c)", tmpDBtbl);
					}
					else if( key->sc_urisire == URI_KIHON ) {
						Sql.Format("insert into %s\
								select seq from datarec where(((dbt = (select SVKcod1 from szvol)) or (cre = (select SVKcod1 from szvol))) and NOT (cast(SUBSTRING(dsign, 5, 1) as int) = 0x1c) and ( \
								(cast(SUBSTRING(dsign, 4, 1) as int) & 0xf0 = 0x00)) )", tmpDBtbl );
					}
					else {
						if( zero_kbn ) {
							Sql.Format("insert into %s\
								select seq from datarec where(((dbt = (select SVKcod1 from szvol)) or (cre = (select SVKcod1 from szvol))) and NOT (cast(SUBSTRING(dsign, 5, 1) as int) = 0x1c) and ( \
								(cast(SUBSTRING(dsign, 4, 1) as int) & 0xf0 = %s) OR (cast(SUBSTRING(dsign, 4, 1) as int) & 0xf0 = 0x00)) )", tmpDBtbl, tmp);
						}
						else {
							Sql.Format("insert into %s\
								select seq from datarec where(((dbt = (select SVKcod1 from szvol)) or (cre = (select SVKcod1 from szvol))) and (cast(SUBSTRING(dsign, 4, 1) as int) & 0xf0 = %s) and NOT (cast(SUBSTRING(dsign, 5, 1) as int) = 0x1c) )", tmpDBtbl, tmp);
						}
					}

					pDBzm->m_database->ExecuteSQL(Sql);
				}
				::ZeroMemory(tmp, sizeof tmp);
			}
			catch (CDBException* dbe) {	// SQL自体のエラー
				CString strError = dbe->m_strError;
				dbe->Delete();
				ermset(0, (char*)(LPCTSTR)strError);
				Sql.Format("IF OBJECT_ID('tempdb..[%s]', 'u') IS NOT NULL DROP TABLE [%s] ", tmpDBtbl, tmpDBtbl);
				pDBzm->m_database->ExecuteSQL(Sql);
				pDBzm->m_database->CommitTrans();
				return -1;
			}
			pDBzm->m_database->CommitTrans();

			buf.Format("seq in (select wkseq from %s)", tmpDBtbl);

			//buf.Format("seq in (select * from kobetsu_seq_ret(%d))", k_ret);
 #endif
#endif
		}
		else {
			buf.Empty();
		}
		if( ! buf.IsEmpty() ) {
			if( initsw )	filter += stype;
			buf += tmp;
			if( (key->sc_urisireopt & SC_NOTSEARCH) )	// NOT 
				filter += "NOT ";

			filter += buf;
			if( ! initsw )	initsw = 1;
		}
	}

	//特定収入[06.03 /08]
	if( key->sc_tokus > NON_SELECT )
	{
		if( pDBzm->zvol->apno >= 0x50 ) {
			switch( key->sc_tokus ) {
			case TK_KATOK: case TK_HKATK: case TK_KYOTK:
			case TK_KATOK5: case TK_HKATK5: case TK_KYOTK5:
			case TK_KATOK8: case TK_HKATK8: case TK_KYOTK8:
			case TK_KATOK8K: case TK_HKATK8K: case TK_KYOTK8K:
			case TK_KATOK10: case TK_HKATK10: case TK_KYOTK10:
				buf = "( cast (SUBSTRING( dsign, 6, 1 ) as int) & 0xff = ";
				break;
			default:
				buf = "( cast (SUBSTRING( dsign, 6, 1 ) as int) & 0x0f = ";
				break;
			}

			switch( key->sc_tokus )
			{
			case TK_SYNYU:	strcpy_s( tmp, sizeof tmp, "0x01 )" );	break;
			// 個別対応の特定収入
			case TK_KATOK:	strcpy_s( tmp, sizeof tmp, "0x11 )" );	break;
			case TK_HKATK:	strcpy_s( tmp, sizeof tmp, "0x21 )" );	break;
			case TK_KYOTK:	strcpy_s( tmp, sizeof tmp, "0x31 )" );	break;

			case TK_FUTOK:	strcpy_s( tmp, sizeof tmp, "0x02 )" );	break;
			case TK_GAITK:	strcpy_s( tmp, sizeof tmp, "0x03 )" );	break;
			case TK_GAI:	strcpy_s( tmp, sizeof tmp, "0x00 )" );	break;
			// 5 or 8%
			case TK_SYNYU5:	strcpy_s( tmp, sizeof tmp, "0x04 )" );	break;
			case TK_KATOK5:	strcpy_s( tmp, sizeof tmp, "0x14 )" );	break;
			case TK_HKATK5:	strcpy_s( tmp, sizeof tmp, "0x24 )" );	break;
			case TK_KYOTK5:	strcpy_s( tmp, sizeof tmp, "0x34 )" );	break;
			case TK_FUTOK5:	strcpy_s( tmp, sizeof tmp, "0x05 )" );	break;

			case TK_SYNYU8:	strcpy_s( tmp, sizeof tmp, "0x06 )" );	break;
			case TK_KATOK8:	strcpy_s( tmp, sizeof tmp, "0x16 )" );	break;
			case TK_HKATK8:	strcpy_s( tmp, sizeof tmp, "0x26 )" );	break;
			case TK_KYOTK8:	strcpy_s( tmp, sizeof tmp, "0x36 )" );	break;
			case TK_FUTOK8:	strcpy_s( tmp, sizeof tmp, "0x07 )" );	break;
			//10%
			case TK_SYNYU10:	strcpy_s( tmp, sizeof tmp, "0x08 )" );	break;
			case TK_KATOK10:	strcpy_s( tmp, sizeof tmp, "0x18 )" );	break;
			case TK_HKATK10:	strcpy_s( tmp, sizeof tmp, "0x28 )" );	break;
			case TK_KYOTK10:	strcpy_s( tmp, sizeof tmp, "0x38 )" );	break;
			case TK_FUTOK10:	strcpy_s( tmp, sizeof tmp, "0x09 )" );	break;

			//8%% 軽減
			case TK_SYNYU8K:	strcpy_s( tmp, sizeof tmp, "0x0A )" );	break;
			case TK_KATOK8K:	strcpy_s( tmp, sizeof tmp, "0x1A )" );	break;
			case TK_HKATK8K:	strcpy_s( tmp, sizeof tmp, "0x2A )" );	break;
			case TK_KYOTK8K:	strcpy_s( tmp, sizeof tmp, "0x3A )" );	break;
			case TK_FUTOK8K:	strcpy_s( tmp, sizeof tmp, "0x0B )" );	break;

			default:
				buf.Empty();
				::ZeroMemory( tmp, sizeof tmp );
				break;
			}
		}
		else {
			buf.Empty();
		}
		if( ! buf.IsEmpty() ) {
			if( initsw )	filter += stype;
			buf += tmp;
			if( (key->sc_tokusopt & SC_NOTSEARCH) )	// NOT 
				filter += "NOT ";

			filter += buf;
			if( ! initsw )	initsw = 1;
		}
	}

	//
	// 摘要
	if( key->sc_tky[0] || SC_Tky.GetCount() > 0 || key->sc_tkytyp > STK_NORMAL )
	{
		if( key->sc_tkytyp == STK_IMG ) { // イメージのみ
			if( initsw )	filter += stype;
			
			buf = "cast (SUBSTRING( dsign, 1, 1 ) as int) & 0x80 = 0x80";
			filter += buf;

			if( ! initsw )	initsw = 1;
		}
		else if( key->sc_tkytyp == STK_STR ) {
			// イメージマスターで 文字摘要のみ
			if( initsw )	filter += stype;
			
			buf = "( cast (SUBSTRING(dsign, 1,1 ) as int) & 0x80 = 0x00 AND\
(tekiyo IS NOT NULL and DATALENGTH( RTRIM( REPLACE( tekiyo, '　', '' ))) > 0))";
			filter += buf;

			if( ! initsw )	initsw = 1;
		}
		else if( key->sc_tkytyp == STK_STRNONE ) {
			// 摘要なし
			if( initsw )	filter += stype;
			
			buf = "( cast (SUBSTRING(dsign, 1,1 ) as int) & 0x80 = 0x00 AND\
(tekiyo IS NULL or DATALENGTH( RTRIM( REPLACE( tekiyo, '　', '' ))) = 0))";

			filter += buf;
			if( ! initsw )	initsw = 1;
		}
		else {
			if( initsw )	filter += stype;

			buf = "( cast (SUBSTRING(dsign, 1,1 ) as int) & 0x80 = 0x00";
			filter += buf;

			if( ! initsw )	initsw = 1;
			n = strlen( (char *)key->sc_tky );
			CString tmp, ans;
			int sz =  sizeof key->sc_tky + 10;
			LPTSTR p = tmp.GetBuffer( sz );
			
			::ZeroMemory( p, sz );
			memcpy( p, key->sc_tky, sizeof key->sc_tky );
			tmp.ReleaseBuffer();
	//		buf.Format( " AND tekiyo LIKE '%%%s%%' )", tmp );
			AdjustSQLsearch( tmp );

			// 複数摘要検索
			if( SC_Tky.GetCount() > 0 ) {
				wk.Empty();
				wk2.Empty();

				for( n = 0; n < SC_Tky.GetCount(); n++ ) {
					if( n > 0 ) {
						wk += stype;
					}
					str = SC_Tky[n];
					AdjustSQLsearch( str );

					//　検索文字列の作成
					int escape =  toNotWildCardStr( ans, str );
					wk2 = MakeTekiyoSearchString( ans, escape ); 
					wk += wk2;
				}
				if( tmp[0] ) {
					wk += stype;
					int escape =  toNotWildCardStr( ans, tmp );
					wk2 = MakeTekiyoSearchString( ans, escape ); 
					wk += wk2;
				}

				if( key->sc_tkyopt & SC_NOTSEARCH ) {
					buf.Format( " AND NOT( %s )", wk );
				}
				else {
					buf.Format( " AND ( %s )", wk );
				}
			}
			else {
				if( key->sc_tkyopt & SC_NOTSEARCH ) {
					int escape =  toNotWildCardStr( ans, tmp );
					wk2 = MakeTekiyoSearchString( ans, escape ); 
					buf.Format( _T(" AND NOT(%s)"), wk2 );
				}
				else {
					int escape =  toNotWildCardStr( ans, tmp );
					wk2 = MakeTekiyoSearchString( ans, escape ); 
					buf.Format( _T(" AND %s"), wk2 );
				}
			}
			buf += ")";

			filter += buf;
		}
	}
	// 手形番号
	if( key->sc_rnum1[0] || key->sc_rnum2[0] )
	{

		if( initsw )	filter += stype;

		char snumbf[22] = {0};

		if( key->sc_rnum1[0] ) {
			memcpy( snumbf, key->sc_rnum1, sizeof key->sc_rnum1 );
			buf.Format( "convert(varbinary(21), space( 20-len(rnumber)) + rnumber) >= convert(varbinary(21), '%20s')", snumbf );

			if( key->sc_rnum2[0] ) {
				::ZeroMemory( snumbf, sizeof snumbf );
				memcpy( snumbf, key->sc_rnum2, sizeof key->sc_rnum2 );

				buf += and;
				wk.Format( "convert(varbinary(21), space( 20-len(rnumber)) + rnumber) <= convert(varbinary(21), '%20s')", snumbf );
				buf += wk;
			}
		}
		else {
			// 終了のみ
			memcpy( snumbf, key->sc_rnum2, sizeof key->sc_rnum2 );
			buf.Format( "convert(varbinary(21), space( 20-len(rnumber)) + rnumber) <= convert(varbinary(21), '%20s')", snumbf );
		}

		if( (key->sc_snumopt & SC_NOTSEARCH) )
				wk.Format( "NOT ( %s )", buf );
		else	wk.Format( "( %s )", buf );

		filter += wk;

		if( ! initsw )	initsw = 1;

	}
	// 手形期日
	if( key->sc_tgdate1 > 0 || key->sc_tgdate2 > 0 )
	{

		if( initsw )	filter += stype;

		if( key->sc_tgdate1 > 0 ) {
			buf.Format( "tdate >= %d", key->sc_tgdate1 );

			if( key->sc_tgdate2 > 0 ) {
				buf += and;
				wk.Format( "tdate <= %d", key->sc_tgdate2 );
				buf += wk;
			}
		}
		else {
			// 終了のみ
			buf.Format( "tdate <= %d", key->sc_tgdate2 );
		}

		if( (key->sc_tgdtopt & SC_NOTSEARCH) )
				wk.Format( "NOT ( %s )", buf );
		else	wk.Format( "( %s )", buf );

		filter += wk;

		if( ! initsw )	initsw = 1;

	}
	// 付箋
	if( key->sc_husen > NON_SELECT ) {
		if( key->sc_husen > 1 ) {
			if( initsw )	filter += stype;

			buf = "cast (SUBSTRING( dsign, 8, 1 ) as int) & 0x0f = ";
			sprintf_s( tmp, sizeof tmp, "0x0%d", (key->sc_husen-1) );
			buf += tmp;
			if( (key->sc_husenopt & SC_NOTSEARCH) ) { // NOT 
				filter += "NOT ";
				buf += " AND cast (SUBSTRING( dsign, 8, 1 ) as int) & 0x0f > 0x00";
			}
			filter += buf;

			if( ! initsw )	initsw = 1;
		}
		else {
			//付箋なし仕訳検索
			if( initsw )	filter += stype;

			if( (key->sc_husenopt & SC_NOTSEARCH) ) {
				filter += "NOT (cast (SUBSTRING( dsign, 8, 1 ) as int) & 0x0f = 0x00)";
			}
			else {
				filter += "cast (SUBSTRING( dsign, 8, 1 ) as int) & 0x0f = 0x00";
			}
		
			if( ! initsw )	initsw = 1;
		}
	}
	/*---VER2----*/
	else {
		// 付箋の指定なしで、NOT のときは、全ての付箋仕訳を検索する
		if( (key->sc_husenopt & SC_NOTSEARCH) ) {
			if( initsw )	filter += stype;

			filter += "NOT (cast (SUBSTRING( dsign, 8, 1 ) as int) & 0x0f = 0x00)";
		
			if( ! initsw )	initsw = 1;
		}
	}

	// 付箋コメント
	if( key->sc_hucmntEX[0] )
	{
		if( initsw )	filter += stype;

	//	wk.Format( "%.10s", key->sc_hucmnt );
		wk.Format( "%s", key->sc_hucmntEX );
		AdjustSQLsearch( wk );

		CString ans;
		int escape =  toNotWildCardStr( ans, wk );
		wk2 = MakeTekiyoSearchString( ans, escape, 1 ); 
//		wk2.Format( "tag LIKE '%%%s%%' ", wk );

		if( (key->sc_husenopt & SC_NOTSEARCH) ) {
			buf.Format( "NOT (%s)", wk2 );
		}
		else {
			buf = wk2;
		}

		filter += buf;

		if( ! initsw )	initsw = 1;
	}

	/*---VER2----*/
	if( key->sc_noinp == SC_NOINP_BRN ) {	// 枝番入力漏れ
		if( initsw )	filter += stype;
		buf = eznoinp;

		filter += buf;
		if( ! initsw )	initsw = 1;
	}
	// + 部門(+枝番)
	else if( key->sc_noinp == SC_NOINP_BMORBR ) {
		if( initsw )	filter += stype;

		char* ptype = or;

		if( !(Voln1->br_sw&0x02) ) {	// 部門枝番なし
			if( Voln1->bm_isw & 0x10 ) {// 損益のみ
				buf.Format( "(%s%s%s)", eznoinp, ptype, bmnsonnoinp );
			}
			else {
				buf.Format( "(%s%s%s)", eznoinp, ptype, bmnnoinp );
			}
		}
		else {
			if( Voln1->bm_isw & 0x10 ) {// 損益のみ
				buf.Format( "(%s%s%s)", eznoinp, ptype, bmnsonnoinp );
			}
			else {
				buf.Format( "(%s%s%s)", eznoinp, ptype, bmnedanoinp );
			}
		}

		filter += buf;
		if( ! initsw )	initsw = 1;
	}
	else if( key->sc_noinp == SC_NOINP_BMON || key->sc_noinp == SC_NOINP_BMNBRN ) {// 部門入力漏れ, 部門+(枝番)
		if( initsw ) {
			filter += stype;
		}
		// 部門入力漏れ, 部門+(枝番) は同じクエリーで、仕訳リストに表示するとき、『部門+(枝番)』 は枝番未入力分に XXXX と表示する。

//MyTrace( "bumon filter br_sw = %02x, bm_isw = %02x\n", Voln1->br_sw, Voln1->bm_isw );
		//部門枝番なし
		if( !(Voln1->br_sw&0x02) ) {
			if( Voln1->bm_isw & 0x10 ) {// 損益のみ
				buf = bmnsonnoinp;
			}
			else {
				buf = bmnnoinp;
			}
		}
		else {
			if( Voln1->bm_isw & 0x10 ) {// 損益のみ
				buf = bmnsonnoinp;
			}
			else {
				buf = bmnedanoinp;
			}
		}

		filter += buf;
		if( ! initsw )	initsw = 1;
	}
	else if( key->sc_noinp == SC_NOINP_KOJI ) {
		if( initsw )	filter += stype;
		// 工事番号入力漏れ
		kojiinp_querystr( buf );

TRACE( "koji noinp filter = %s\n", buf );

		filter += buf;
		if( ! initsw )	initsw = 1;
	}
	else if( key->sc_noinp == SC_NOINP_DENP ) {
		// 伝票番号入力漏れ
		if( initsw )	filter += stype;
		buf = "cno is null";

		filter += buf;
		if( ! initsw )	initsw = 1;
	}
	else if( key->sc_noinp == SC_NOINP_INVNO ) {
		// 登録番号入力漏れ
		if( initsw ) {
			filter += stype;
		}

		//対象は仕入仕訳
		CString sql;
		sql_sire_EX(sql);
		buf.Format( _T("invno is null and ddate >= 20231001 %s"), sql );

		filter += buf;
		if( !initsw ) {
			initsw = 1;
		}
	}
#ifdef KEEP_CLOSE
	else if( key->sc_noinp == SC_NOINP_BMNBRN ) {
		// 部門（＋枝番）
		if( initsw )	filter += stype;

		//部門枝番なし
		if( !(Voln1->br_sw&0x02) ) {
			if( Voln1->bm_isw & 0x10 ) {	//-- 損益のみ
buf = "(( EXISTS (select bmkzrec.kzkcd from bmkzrec where bmkzrec.kzkcd = datarec.dbt and datarec.dbmn is null and datarec.dbt >= '08000000') or \
EXISTS (select bmkzrec.kzkcd from bmkzrec where bmkzrec.kzkcd = datarec.cre and datarec.cbmn is null and datarec.cre >= '08000000' ) )";
			}
			else {
buf = "(( EXISTS (select bmkzrec.kzkcd from bmkzrec where bmkzrec.kzkcd = datarec.dbt and datarec.dbmn is null) or \
EXISTS (select bmkzrec.kzkcd from bmkzrec where bmkzrec.kzkcd = datarec.cre and datarec.cbmn is null) )";
			}
		}
		else {
			if( Voln1->bm_isw & 0x10 ) {	//-- 損益のみ
				buf = "(( EXISTS (select bmkzrec.kzkcd from bmkzrec where bmkzrec.kzkcd = datarec.dbt and datarec.dbmn is null and datarec.dbt >= '08000000') or \
EXISTS (select bmkzrec.kzkcd from bmkzrec where bmkzrec.kzkcd = datarec.cre and datarec.cbmn is null and datarec.cre >= '08000000' ) or \
EXISTS (select bmezrec.ezkcd from bmezrec where bmezrec.ezkcd = datarec.dbt and bmezrec.ezbmn = datarec.dbmn and datarec.dbmn is null and datarec.dbt >= '08000000') or \
EXISTS (select bmezrec.ezkcd from bmezrec where bmezrec.ezkcd = datarec.cre and bmezrec.ezbmn = datarec.cbmn and datarec.cbmn is null and datarec.cre >= '08000000') )";
			}
			else {
				buf = "(( EXISTS (select bmkzrec.kzkcd from bmkzrec where bmkzrec.kzkcd = datarec.dbt and datarec.dbmn is null) or \
EXISTS (select bmkzrec.kzkcd from bmkzrec where bmkzrec.kzkcd = datarec.cre and datarec.cbmn is null) or \
EXISTS (select bmezrec.ezkcd from bmezrec where bmezrec.ezkcd = datarec.dbt and bmezrec.ezbmn = datarec.dbmn and datarec.dbmn is null ) or \
EXISTS (select bmezrec.ezkcd from bmezrec where bmezrec.ezkcd = datarec.cre and bmezrec.ezbmn = datarec.cbmn and datarec.cbmn is null ) )";
			}
		}

		filter += buf;
		filter += " and ";
		if( ! initsw )	initsw = 1;

		buf = "( EXISTS (select ezrec.ezecd from ezrec where ezrec.ezkcd = datarec.dbt and datarec.dbr is null) or \
EXISTS (select ezrec.ezecd from ezrec where ezrec.ezkcd = datarec.cre and datarec.cbr is null) ))";

		filter += buf;
	}
#endif
	
	// 業種区分
	if( key->sc_gyokbn > 0  ) {
		if( initsw )	filter += stype;

		wk.Format( "0x%02d", key->sc_gyokbn );
		// 貸借共通で使用 で メインの業種の検索のとき、メインの貸借の仕訳すべてを検索しない
		if( pDBzm->zvol->ind_sgn2 == key->sc_gyokbn ) {
			buf.Format( "( ((select ind_ctg from knrec where knrec.kncod = datarec.dbt and datarec.dbt > '01000000' ) & 0x0f = %s and\
 (select ind_ctg from knrec where knrec.kncod = datarec.cre and datarec.cre > '01000000' ) & 0x0f = %s ) or\
 (dbt < '01000000' and (select ind_ctg from knrec where knrec.kncod = datarec.cre and datarec.cre > '01000000') & 0x0f = %s) or\
 (cre < '01000000' and (select ind_ctg from knrec where knrec.kncod = datarec.dbt and datarec.dbt > '01000000') & 0x0f = %s) )", wk, wk, wk, wk );
		}
		else {
			buf.Format( "( (select ind_ctg from knrec where knrec.kncod = datarec.dbt and datarec.dbt > '01000000' ) & 0x0f = %s or\
 (select ind_ctg from knrec where knrec.kncod = datarec.cre and datarec.cre > '01000000' ) & 0x0f = %s )", wk, wk );
		}

		filter += buf;
		if( ! initsw )	initsw = 1;
	}

	if( key->sc_hjiyu > 0  ) {
		if( initsw )	filter += stype;

		if( pDBzm->zvol->apno < 0x50 ) {
			// 変動事由
			buf = "(SUBSTRING( dbt, 2, 1 ) = '7' OR SUBSTRING( cre, 2, 1 ) = '7') ";

			if( key->sc_hjiyu == HJIYU_MITOROK ) {	// 変動事由未登録
				// 変動事由テーブルに登録されていて有効なコードか
				strcpy_s( tmp, sizeof tmp, "AND hjiyu NOT IN (select hnjcod from kshnam where hnpflg = 1)" );
				buf += tmp;
			}
			else {
				if( key->sc_hjiyu != HJIYU_TAISYO ) {	//変動事由対象仕訳
					//各変動事由に合致するものみ
					sprintf_s( tmp, sizeof tmp, "AND hjiyu = %d", key->sc_hjiyu );
					buf += tmp;
				}
			}
		}
		else {
			// 振替科目
			NPfuri_Query( key->sc_hjiyu, buf );
		}

		filter += buf;
		if( ! initsw )	initsw = 1;
	}

	//
	// 日付
	if( key->sc_date1[0] || key->sc_date2[0] )
	{
		int date, date1, date2, flg;
		int e_date, e_smd, e_emdflg, e_prvsmd, e_prvemd;
		int s_ksg, e_ksg, snxt_ksg, snxt_smd, s_nxt_emd;
		CString tmp1, tmp2;

		wk2.Empty();
		wk3.Empty();

		date = date1 = date2 = 0;
		e_date = e_smd = e_emdflg = e_prvsmd = e_prvemd = 0;
		s_ksg = e_ksg = snxt_ksg = snxt_smd = s_nxt_emd = 0;

		pDBzm->BcdDateToYmd( key->sc_date1, &date );
		s_ksg = KESY_SG_GET( key->sc_date1[0] );
		pDBzm->BcdDateToYmd( key->sc_date2, &e_date );
		e_ksg = KESY_SG_GET( key->sc_date2[0] );

		// 06.25 /13 
		// 20日が締日で、期間が1/1～12/31 までで 決修が指定された時 12/01 の日付が検索されない。
		if( s_ksg == 4 ) {
			int newmm = (date % 10000) / 100;
			char mmbf[16] = {0};
			sprintf_s( mmbf, sizeof mmbf, "%02x", (key->sc_date1[0]&0x1f) );
			int bcdmm = atoi( mmbf );
			if( newmm != bcdmm ) {
				int t_yy, t_dd;
				t_yy = date / 10000;
				t_dd = date % 100;
				date = (t_yy * 10000) + (bcdmm*100) + t_dd;
			}
		}

		if( e_ksg == 4 ) {
			int newmm = (e_date % 10000) / 100;
			char mmbf[16] = {0};
			sprintf_s( mmbf, sizeof mmbf, "%02x", (key->sc_date2[0]&0x1f) );
			int bcdmm = atoi( mmbf );
			if( newmm != bcdmm ) {
				int t_yy, t_dd;
				t_yy = e_date / 10000;
				t_dd = e_date % 100;
				e_date = (t_yy * 10000) + (bcdmm*100) + t_dd;
			}
		}

		// 同一月を指定
		if( key->sc_sofs == key->sc_eofs ) {
			buf.Format( "(ddate >= %d AND ddate <= %d AND mofs = %d)", date, e_date, key->sc_sofs );
		}
		else {
			Minfo->MoveFirst();
			flg = 0;
			do {
				if( Minfo->ofset == key->sc_sofs ) {
					flg = 1;
					date1	= Minfo->smd;
					date2	= Minfo->emd;
					continue;
				}
				if( flg ) {
					// 開始日の翌月が 中間・決修
					if( Minfo->kes_sgn ) {
						snxt_ksg	= 1;
						snxt_smd	= Minfo->smd;
						s_nxt_emd	= Minfo->emd;
					}
					break;
				}
			} while( Minfo->MoveNext() == 0 );

			if( e_ksg ) {
				Minfo->MoveFirst();
				do {
					// 終了日が 中間・決修で、途中の日か？
					if( Minfo->ofset == key->sc_eofs ) {
						if( e_date < Minfo->emd ) {
							e_emdflg = 1;
						}
						e_smd = Minfo->smd;
						break;
					}
					else {
						if( Minfo->ofset < key->sc_eofs && (Minfo->insw & 0x01) ) {
							e_prvsmd = Minfo->smd;
							e_prvemd = Minfo->emd;
						}
					}

				} while( Minfo->MoveNext() == 0 );
			}

			flg = 0;
			if( snxt_ksg ) {
				if( (key->sc_sofs + 1) == key->sc_eofs ) {
					wk2.Format( "( ddate >= %d AND ddate <= %d AND mofs = %d )", date, date2, key->sc_sofs );
					wk2 += or;
					wk3.Format( "( ddate >= %d AND ddate <= %d AND mofs = %d )", snxt_smd, e_date, key->sc_eofs );
					wk2 += wk3;
					flg = 1;
				}
				else {
					// 中間・決修とその前の月は、開始終了月日は同じなので、単なる年月日比較では、範囲外になる 日がある
					wk2.Format( "ddate >= %d AND mofs >= %d", snxt_smd, (key->sc_sofs + 1) );
					tmp1.Format( "(ddate >= %d AND mofs = %d)", date, key->sc_sofs );
				}
			}
			else {
				wk2.Format( "ddate >= %d AND mofs >= %d", date, key->sc_sofs );
			}

			// 決修か中間で、月途中を指定された場合
			if( e_emdflg ) {
				if( (key->sc_eofs - 1) != key->sc_sofs ) {
					wk3.Format( "ddate <= %d AND mofs <= %d", e_prvemd, (key->sc_eofs - 1) );
					tmp2.Format( "(ddate >= %d AND ddate <= %d AND mofs = %d)", e_smd, e_date, key->sc_eofs );
				}
				else if( ! snxt_ksg ) {
					wk3.Format( "ddate <= %d", e_date );
				}
			}
			else {
				wk3.Format( "ddate <= %d AND mofs <= %d", e_date, key->sc_eofs );
			}

ASSERT( ! wk3.IsEmpty()  );

			if( ! flg ) buf.Format( "(( %s AND %s )", wk2, wk3 );
			else		buf.Format( "(( %s )", wk2 );

			if( ! tmp1.IsEmpty() ) {
				buf += or;
				buf += tmp1;
			}

			if( ! tmp2.IsEmpty() ) {
				buf += or;
				buf += tmp2;
			}

			buf += ")";
		}

		if( initsw ) {
			// OR 日付はAND
			if( key->sc_stype == SCS_OR ) {
				tmp1.Format( "(%s) AND ", filter );
				filter = tmp1;
			}
			else {
				filter += stype;
			}
		}

		if( (key->sc_dateopt & SC_NOTSEARCH) )
			filter += "NOT";

		filter += buf;
		if( ! initsw )	initsw = 1;
	}

	// 証憑番号 [11.26 /12]
	if( key->sc_docev1[0] || key->sc_docev2[0] )
	{

		if( initsw )	filter += stype;

		char docbf[16] = {0};

		if( key->sc_docev1[0] ) {
			memcpy( docbf, key->sc_docev1, sizeof key->sc_docev1 );
			buf.Format( "convert(varbinary(8), space( 8-len(skn)) + skn) >= convert(varbinary(8), '%8s')", docbf );

			if( key->sc_docev2[0] ) {
				::ZeroMemory( docbf, sizeof docbf );
				memcpy( docbf, key->sc_docev2, sizeof key->sc_docev2 );

				buf += and;
				wk.Format( "convert(varbinary(8), space( 8-len(skn)) + skn) <= convert(varbinary(8), '%8s')", docbf );
				buf += wk;
			}
		}
		else {
			// 終了のみ
			memcpy( docbf, key->sc_docev2, sizeof key->sc_docev2 );
			buf.Format( "convert(varbinary(8), space( 8-len(skn)) + skn) >= convert(varbinary(8), '%8s')", docbf );
		}

		if( (key->sc_docevopt & SC_NOTSEARCH) )
				wk.Format( "NOT ( %s )", buf );
		else	wk.Format( "( %s )", buf );

		filter += wk;

		if( ! initsw )	initsw = 1;
	}

	// 文書番号
	if( key->sc_docseq1 > 0 || key->sc_docseq2 > 0 )
	{
		if( initsw )	filter += stype;

		if( key->sc_docseq1 > 0 ) {
			buf.Format( "seq = ANY(select dtseq from lnkseq where docseq >= %d", key->sc_docseq1 );

			if( key->sc_docseq2 > 0 ) {
				buf += and;
				wk.Format( "docseq <= %d)", key->sc_docseq2 );
				buf += wk;
			}
			else {
				buf += ")";
			}
		}
		else {
			// 終了のみ
			buf.Format( "seq = ANY(select dtseq from lnkseq where docseq <= %d)", key->sc_docseq2 );
		}

		if( (key->sc_docseqopt & SC_NOTSEARCH) )
				wk.Format( "NOT ( %s )", buf );
		else	wk.Format( "( %s )", buf );

		filter += wk;

		if( ! initsw )	initsw = 1;
	}

	if( key->sc_docshtyp != 0 )
	{
		if( initsw )	filter += stype;

		CString sDBdoc;
		CString endym;
		int apno = atoi(CBlkIns::pcom_apno);

		endym = CBlkIns::pcom_kikan.Mid(2,4);
		sDBdoc.Format( "D%02x_C%s_Y%s", apno,	CBlkIns::pcom_cocode, endym );
		int srtype;
		if( key->sc_docshtyp == -1 )	srtype = 0;	//-未設定-
		else							srtype = key->sc_docshtyp;

		buf.Format( "seq = ANY(select dtseq from lnkseq where docseq = ANY(select doc.docseq from %s.dbo.document as doc where doc.srtype = %d ))", sDBdoc, srtype);
		wk.Format( "( %s )", buf );

		filter += wk;

		if( ! initsw )	initsw = 1;
	}

	//資産
	if( key->sc_syzktsn != 0 )
	{
		if( initsw )	filter += stype;

		switch( key->sc_syzktsn )
		{
		case KTS_SYUTOK:strcpy_s( tmp, sizeof tmp, "0x10" );	break;
		case KTS_JOTO:	strcpy_s( tmp, sizeof tmp, "0x30" );	break;
		}

		buf.Format( "( cast(SUBSTRING( dsign, 1, 1 ) as int) & 0x40 = 0x40 AND cast (SUBSTRING( dsign, 3, 1 ) as int) & 0xf0 = %s) ", tmp );

		filter += buf;

		if( ! initsw )	initsw = 1;
	}

	//未確定仕訳・確定仕訳
	if( key->sc_mikad != 0 )
	{
		if( initsw )	filter += and;

		if( key->sc_mikad == 1 ) {
			buf = "( cast(SUBSTRING( dsign, 8, 1 ) as int) & 0x10 != 0x10)";
			filter += buf;
		}
		else {
			buf = "( cast(SUBSTRING( dsign, 8, 1 ) as int) & 0x10 = 0x10)";
			filter += buf;
		}

		if( ! initsw )	initsw = 1;
	}

	// インボイス対応
	// 登録番号
	if( key->sc_invsgn != 0 ) {
		if( initsw )	filter += stype;

		// datarec には 'T'+登録番号で格納
		buf.Format( _T( "invno = 'T%s'" ), key->sc_invno );
		if(( key->sc_invnoopt & SC_NOTSEARCH )) {
			wk.Format( _T( "NOT ( %s )" ), buf );
		}
		else {
			wk.Format( _T( "( %s )" ), buf );
		}
		filter += wk;

		if( !initsw ) {
			initsw = 1;
		}
	}
	else {
		if(( key->sc_invnoopt & SC_NOTSEARCH )) {
			if( initsw ) {
				filter += stype;
			}

			wk.Format( _T( "invno is not null" ), buf );

			filter += wk;

			if( !initsw ) {
				initsw = 1;
			}
		}
	}

	// 免税事業者からの課税仕入
	if( key->sc_menzei == 0 ) {
		if( (key->sc_menzeiopt & SC_NOTSEARCH) ) {
			if( initsw )	filter += stype;

			buf = _T("( CAST( SUBSTRING( dsign, 7, 1 ) as int ) & 0x20 = 0x20 )");

			filter += buf;

			if( !initsw ) {
				initsw = 1;
			}
		}
	}
	else {
		if( key->sc_menzei != 0 ) {
			if( initsw )	filter += stype;

			switch( key->sc_menwari ) {
			case 0:	//すべての控除
				buf = _T("( CAST( SUBSTRING( dsign, 7, 1 ) as int ) & 0x20 = 0x20 )");
				break;
			case 1: //80%控除 dsing[6] D6-7 = 0; D5:ON
				buf = _T("( CAST( SUBSTRING( dsign, 7, 1 ) as int ) & 0xE0 = 0x20 )");
				break;
			case 2:	//50%控除 dsing[6] D6-7 = 1, D5:ON
				buf = _T("( CAST( SUBSTRING( dsign, 7, 1 ) as int ) & 0xE0 = 0x60 )");
				break;
			}

			if( (key->sc_menzeiopt & SC_NOTSEARCH) ) {
				//対象は仕入仕訳
				CString sql;
				sql_sire(sql);
				wk.Format(_T("NOT ( %s ) %s and ddate >= 20231001"), buf, sql);
			}
			else {
				wk.Format(_T("( %s )"), buf);
			}
			filter += wk;

			if( !initsw ) {
				initsw = 1;
			}
		}
	}

//MyTrace( "scan filter %s", filter );
	CString dtfil;
	if( datatype_filter( dtfil ) ) {
		QueryStringOwnerCheck( filter );
		filter += stype;
		filter += dtfil;
	}

	// filter 作成のみ
	if( mkonly )	return 0;

	pDBzm->dbdata_Requery() = TRUE;
	pDBzm->dbdata_Job()		= DBDATA_JOB_SCAN;
	pDBzm->dbdata_Sort()	= (DtMode == 0) ? DBDATA_SORT_SEQ : DBDATA_SORT_DATE;
	pDBzm->dbdata_Speed()	= FALSE;

	if( IsMasterType(MST_INSERT) ) {
		st = pDBzm->dbdata->Requery( DtOwner, 0, filter, 0, (DtMode==0) ? 3 : DtMode );
	}
	else {
		st = pDBzm->dbdata->Requery( DtOwner, 0, filter, 0, (DtMode==0) ? 0 : DtMode );
	}

	if( ! st ) {
		InitScanPgTBL( &SCAN_PGTBL );

		SCAN_PGTBL.data_owner	= DtOwner;
		SCAN_PGTBL.data_mode	= DtMode;
		SCAN_PGTBL.filter = filter;
		SCAN_PGTBL.tbl_end = 1;

		// 修正前仕訳テーブルをクリア
		SCMDFY_SEQ.fill();
	}

//END_PERFORMANCE( "@scanview data->Requery" );

	return st;

}


//	枝番クエリー文字列作成
//	CString& brstr ... クエリー文字列
//	int sgn .......... 0 = 借方, 1 = 貸方, 2 = 借／貸 --> 借方, 3 = 借／貸 --> 貸方
//
static
void brnquery_str( SCAN_ITEMEX* psi, CString& brstr, int sgn )
{
	char* and	= " AND ";
	char* or	= " OR ";
	CString buf;

	if( sgn == 0 ) {	// 借方
		if( psi->sc_dbr1 >= 0 ) {
			if( (psi->sc_dbropt & SC_NOTSEARCH) )
				brstr.Format( "dbr < %d", psi->sc_dbr1 );
			else
				brstr.Format( "dbr >= %d", psi->sc_dbr1 );
		}

		if( psi->sc_dbr2 >= 0 ) {
			if( (psi->sc_dbropt & SC_NOTSEARCH) )
				buf.Format( "dbr > %d", psi->sc_dbr2 );
			else
				buf.Format( "dbr <= %d", psi->sc_dbr2 );

			if( psi->sc_dbr1 >= 0 ) {
				if( (psi->sc_dbropt & SC_NOTSEARCH) ) 
					brstr += or;
				else
					brstr += and;

				brstr += buf;
				buf.Format( "( %s )", brstr );
				brstr = buf;
			}
			else	brstr = buf;
		}
	}
	else if( sgn == 1 ) {	// 貸方
		if( psi->sc_cbr1 >= 0 ) {
			if( (psi->sc_cbropt & SC_NOTSEARCH) )
				brstr.Format( "cbr < %d", psi->sc_cbr1 );
			else
				brstr.Format( "cbr >= %d", psi->sc_cbr1 );
		}

		if( psi->sc_cbr2 >= 0 ) {
			if( (psi->sc_cbropt & SC_NOTSEARCH) )
				buf.Format( "cbr > %d", psi->sc_cbr2 );
			else
				buf.Format( "cbr <= %d", psi->sc_cbr2 );

			if( psi->sc_cbr1 >= 0 ) {
				if( (psi->sc_cbropt & SC_NOTSEARCH) ) 
					brstr += or;
				else
					brstr += and;

				brstr += buf;
				buf.Format( "( %s )", brstr );
				brstr = buf;
			}
			else	brstr = buf;
		}
	}
	else if( sgn == 2 ) {	// 借／貸 --> 借方
		if( psi->sc_dcbrn1 >= 0 ) {
			if( (psi->sc_dcbropt & SC_NOTSEARCH) )
				brstr.Format( "dbr < %d", psi->sc_dcbrn1 );
			else
				brstr.Format( "dbr >= %d", psi->sc_dcbrn1 );
		}

		if( psi->sc_dcbrn2 >= 0 ) {
			if( (psi->sc_dcbropt & SC_NOTSEARCH) )
				buf.Format( "dbr > %d", psi->sc_dcbrn2 );
			else
				buf.Format( "dbr <= %d", psi->sc_dcbrn2 );

			if( psi->sc_dcbrn1 >= 0 ) {
				if( (psi->sc_dcbropt & SC_NOTSEARCH) ) 
					brstr += or;
				else
					brstr += and;

				brstr += buf;
				buf.Format( "( %s )", brstr );
				brstr = buf;
			}
			else	brstr = buf;
		}
	}
	else {		// 借／貸 --> 貸方
		if( psi->sc_dcbrn1 >= 0 ) {
			if( (psi->sc_dcbropt & SC_NOTSEARCH) )
				brstr.Format( "cbr < %d", psi->sc_dcbrn1 );
			else
				brstr.Format( "cbr >= %d", psi->sc_dcbrn1 );
		}

		if( psi->sc_dcbrn2 >= 0 ) {
			if( (psi->sc_dcbropt & SC_NOTSEARCH) )
				buf.Format( "cbr > %d", psi->sc_dcbrn2 );
			else
				buf.Format( "cbr <= %d", psi->sc_dcbrn2 );

			if( psi->sc_dcbrn1 >= 0 ) {
				if( (psi->sc_dcbropt & SC_NOTSEARCH) ) 
					brstr += or;
				else
					brstr += and;

				brstr += buf;
				buf.Format( "( %s )", brstr );
				brstr = buf;
			}
			else	brstr = buf;
		}	
	}
}



/*---VER2----*/

// 拡張合計名称の 元合計名称取得
DBGKREC* get_gksrc( DWORD gkcode )
{
	DWORD dwCode, tmp;
	DBGKREC* pGk;

	do {
		pGk = pDBzm->PjisToGkrec( gkcode );

		if( pGk ) {
			tmp = pGk->gksrc;
			hl_rev( &tmp );
			
			dwCode = (tmp & 0x0000f000);
			if( (dwCode & 0x00008000) < 0 ) 
				break;
			else {
				// まだ、追加小分類
				gkcode = pGk->gksrc;
				pGk = NULL;
			}
		}
		else {
			pGk = NULL;
			break;
		}
	} while( 1 );

	// 追加基本小分類
	return pGk;
}


/*---VER2----*/
//--------------------------------------------------------------
//		工事番号の入力漏れクエリ 作成
//	
//
//--------------------------------------------------------------
static
int kojiinp_querystr( CString& kjstr )
{
	int gk_opn = 0;

	if( pDBzm->pGkrec == NULL ) {
		if( pDBzm->GokeiMeisyoRead() == ERR )
			return -1;

		gk_opn = 1;
	}

	CDWordArray da;
	pDBzm->m_KJchk.RemoveAll();

	DWORD		dwCode, work;
	DBGKREC*	pGk, *pBaseGk;
	int i;

	// 小分類で追加された項目をチェックする
	for( i = 0; i < pDBzm->gkn_info->reccount; i++ ) {
		pGk = pDBzm->pGkrec + i;

		if( pGk->gksrc > 0 ) {
			dwCode = pGk->gksrc;
			hl_rev( &dwCode, 4 );

			if( (dwCode & 0x0000F000) < 0x00008000 ) {
				pBaseGk = pGk;
			}
			else {
				// 追加元が、まだ、追加小分類のとき
				// 追加でない合計名称サーチ
				pBaseGk = get_gksrc( pGk->gksrc );

				dwCode = pBaseGk->gksrc;
				hl_rev( &dwCode, 4 );
			}

			// 追加科目で、工事番号入力漏れの科目
			switch( (dwCode & 0xffffff00) ) {
			case 0x01040300:	// 完成工事未収金
			case 0x01060200:	// 未成工事支出金
			case 0x04050200:	// 未成工事受入金
			case 0x08010100:	// 完成工事高
			case 0x08020100:	// 完成工事値引

				work = pGk->gkcod;
				hl_rev( &work, 4 );
				da.Add( work );
				break;
			}
		}
	}
	// 表示画面のチェック用
	pDBzm->m_KJchk.Copy( da );
	pDBzm->m_KJchk.Add( 0x01040300 );
	pDBzm->m_KJchk.Add( 0x01060200 );
	pDBzm->m_KJchk.Add( 0x04050200 );
	pDBzm->m_KJchk.Add( 0x08010100 );
	pDBzm->m_KJchk.Add( 0x08020100 );

	CString add_dbt, add_cre, cdstr, tmp;
	INT_PTR max;
	max = da.GetCount();

	for( i = 0; i < max; i++ ) {
		add_dbt += " OR ";
		add_cre += " OR ";

		cdstr.Format( "%08x", da[i] );

		tmp.Format( "SUBSTRING( dbt, 1, 6 ) = '%s'", cdstr.Left(6) );
		add_dbt += tmp;

		tmp.Format( "SUBSTRING( cre, 1, 6 ) = '%s'", cdstr.Left(6) );
		add_cre += tmp;
	}

	if( max > 0 ) {
		kjstr.Format( _T("( ((SUBSTRING( dbt, 1, 6 ) = '010403' or SUBSTRING( dbt, 1, 6 ) = '010602' or SUBSTRING( dbt, 1, 6 ) = '040502' or \
SUBSTRING( dbt, 1, 6 ) = '080101' or SUBSTRING( dbt, 1, 6 ) = '080201' or SUBSTRING( dbt, 1, 2 ) = '0F' %s) and dkno is null) or \
((SUBSTRING( cre, 1, 6 ) = '010403' or SUBSTRING( cre, 1, 6 ) = '010602' or SUBSTRING( cre, 1, 6 ) = '040502' or \
SUBSTRING( cre, 1, 6 ) = '080101' or SUBSTRING( cre, 1, 6 ) = '080201' or SUBSTRING( cre, 1, 2 ) = '0F' %s) and ckno is null ))"), add_dbt, add_cre );
	}
	else {
		kjstr = _T("( ((SUBSTRING( dbt, 1, 6 ) = '010403' or SUBSTRING( dbt, 1, 6 ) = '010602' or SUBSTRING( dbt, 1, 6 ) = '040502' or \
SUBSTRING( dbt, 1, 6 ) = '080101' or SUBSTRING( dbt, 1, 6 ) = '080201' or SUBSTRING( dbt, 1, 2 ) = '0F') and dkno is null) or \
(( SUBSTRING( cre, 1, 6 ) = '010403' or SUBSTRING( cre, 1, 6 ) = '010602' or SUBSTRING( cre, 1, 6 ) = '040502' or \
SUBSTRING( cre, 1, 6 ) = '080101' or SUBSTRING( cre, 1, 6 ) = '080201' or SUBSTRING( cre, 1, 2 ) = '0F') and ckno is null ))");
	}

	if( gk_opn ) {
		pDBzm->GokeiMeisyoFree();
	}

	return 0;
}


//
// キー項目入力ありチェック
static
BOOL isScanKeyInp()
{
	SCAN_ITEMEX *key = &SC_Item;

	int kmkinp	=	scnkmk_input( key );

	// ＳＥＱ番号
	if( key->sc_seq1 > 0 || key->sc_seq2 > 0 ||
	// 日付
		key->sc_date1[0] || key->sc_date2[0] ||
	// 伝票番号
		key->sc_denp1 >= 0 || key->sc_denp2 >= 0 ||
	// 部門番号
		key->sc_dbmn1 >= 0 || key->sc_dbmn2 >= 0 ||
		key->sc_cbmn1 >= 0 || key->sc_cbmn2 >= 0 ||
		key->sc_dcbmn1 >= 0 || key->sc_dcbmn2 >= 0 ||
	// 工事番号
		key->sc_dkji1[0] || key->sc_dkji2[0] ||
		key->sc_ckji1[0] || key->sc_ckji2[0] ||
		key->sc_dckji1[0] || key->sc_dckji2[0] ||
	// 借方、貸方、貸借　科目
	//	key->sc_dbt[0] || key->sc_cre[0] || key->sc_dbcr[0] ||
		kmkinp ||
	// 借方、貸方、貸借　枝番
		key->sc_dbr1 >= 0 || key->sc_cbr1 >= 0 || key->sc_dcbrn1 >= 0  ||
		key->sc_dbr2 >= 0 || key->sc_cbr2 >= 0 || key->sc_dcbrn2 >= 0  ||
	// 部門、枝番なし check OFF か 02.26 /03
		(key->sc_dbmnopt & SC_ITEMNONE)	|| (key->sc_cbmnopt & SC_ITEMNONE) ||
		(key->sc_dkjiopt & SC_ITEMNONE)	|| (key->sc_ckjiopt & SC_ITEMNONE) ||
		(key->sc_dbropt & SC_ITEMNONE)	|| (key->sc_cbropt & SC_ITEMNONE) ||
	// 金額
		key->sc_valsgn[0] || key->sc_valsgn[1] ||
	// 税額
		key->sc_zeisgn[0] || key->sc_zeisgn[1] ||
	// 対価
		key->sc_taisgn[0] || key->sc_taisgn[1] ||
	// 消費税
		key->sc_syzsw > NON_SELECT	|| key->sc_zritu > NON_SELECT ||
		key->sc_skbn > NON_SELECT	|| key->sc_urisire > NON_SELECT ||
		(key->sc_skbnopt & SC_KMKHANI) ||
	// 摘要
		key->sc_tky[0] || key->sc_tkytyp > STK_NORMAL ||
	// 証憑番号
		key->sc_rnum1[0] || key->sc_rnum2[0] ||
	// 手形期日
		key->sc_tgdate1 > 0 || key->sc_tgdate2 > 0 ||
	// 付箋
		key->sc_husen > NON_SELECT || 
	// 入力漏れ
		key->sc_noinp > 0 ||
	// 変動事由
		key->sc_hjiyu > 0 ||
	// 文書番号・書類種別
		key->sc_docseq1 > 0 || key->sc_docseq2 > 0 || key->sc_docshtyp != NON_SELECT 

		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



////////////////////////////////////////////////////////////////////////////////////////////
// 表示行設定
//
#include "DtClmItem.h"

extern
DT_CLMITEM*	GetDtListColumn( int type, int& clmax );


void CScanView::SetListColumn( int line )
{
	CArray<DT_COLUMN, DT_COLUMN&> clm;
	int i, max, cnt;
	DT_COLUMN dtcol;
	DT_CLMITEM* pDisp;

	if( bDOCEVI_Master ) {
		pDisp = GetDtListColumn( DTCLM_SNOL1, max );
	}
	else {
		pDisp = GetDtListColumn( DTCLM_LINE1, max );
	}
	clm.SetSize( max );

	for( i = cnt = 0; i < max; i++ ) {

		if( pDisp[i].id == DATA_USER1 ) {
			if( ! DenpTypeMode() )
				continue;
		}

		if( pDisp[i].id == DATA_DENP ) {
			if( pAUTOSEL->DSPDENP_OPT == OPT_NODSP )
				continue;
		}
		if( pDisp[i].id == DATA_DEB_BMON ) {
			if( pAUTOSEL->DSPBMON_OPT == OPT_NODSP )
				continue;
			if( !(pDBzm->zvol->sub_sw&0x02) )
				continue;

			pDisp[i].inpsize = Voln1->bmcol;
			pDisp[i].dispsize = Voln1->bmcol;
		}
		if( pDisp[i].id == DATA_DEB_KNO ) {
			if( Voln1->apno == 0x20 && (pDBzm->zvol->sub_sw&0x04) ) {
				if( pAUTOSEL->DSPKOJI_OPT == OPT_NODSP )
					continue;
		
				pDisp[i].inpsize = Voln1->kjcol;
				pDisp[i].dispsize = Voln1->kjcol;
			}
			else {
				continue;
			}
		}

		if( pDisp[i].id == DATA_HJIYU ) {
			if( IsHjiyuItemGyosyu() == 0 )	continue;
		//	if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ )	continue;

			if( pAUTOSEL->DSPHJIYU_OPT == OPT_NODSP )
				continue;
		}
		// 税額		09.20 /07
		if( pDisp[i].id == DATA_ZEI ) {
			if( pAUTOSEL->DSPZEI_OPT == OPT_NODSP )
				continue;
		}
		// 消費税	09.02 /08
		if( pDisp[i].id == DATA_SYZ ) {
			if (pAUTOSEL->DSPSYZ_OPT == OPT_NODSP)
				continue;

			if( CheckWidthSyohi() )
				pDisp[i].dispsize = 8+2;
			else
				pDisp[i].dispsize = 8;
		}

		dtcol.ln_ofs	= pDisp[i].ln_ofs;
		dtcol.clm_title	= pDisp[i].title;
		dtcol.clm_id	= pDisp[i].id;
		dtcol.clm_type	= pDisp[i].type;
		dtcol.clm_fmt	= pDisp[i].format;
		dtcol.clm_back	= pDisp[i].back;
		dtcol.clm_text	= pDisp[i].text;
		dtcol.clm_inpsz	= pDisp[i].inpsize;
		dtcol.clm_size	= pDisp[i].dispsize;
		dtcol.clm_szadj	= pDisp[i].size_adj;
		dtcol.clm_spos	= pDisp[i].spos;
		dtcol.clm_adj	= pDisp[i].spos_adj;
		dtcol.clm_sidx	= pDisp[i].clm_sidx;

		clm.SetAt( cnt, dtcol );
		cnt++;
	}
	m_ScList.SetItemColumn( &clm[0], cnt );
}


// ボタン処理
//	End
void CScanView::OnButton1() 
{
	if( ! PROGRAM_OK )	return;

	// スキャンオンリーモード時
	if( bSCAN_ONLY_MODE )
	{
		((CMainFrame*)GetDBDinpMainFrame())->PostMessage( WM_CLOSE );
		return;
	}

	//入力にビュー切り替え
	((CMainFrame*)GetDBDinpMainFrame())->ChgToInpView( 0 );
}

//	修正入力へ
void CScanView::OnBnClickedButton2()
{
	if( ! PROGRAM_OK )	return;

	try
	{
		tomdfy();	//修正画面へ
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

// 再検索
void CScanView::OnBnClickedButton3()
{
	if( ! PROGRAM_OK )	return;

	OnButtonF8();
}



BEGIN_EVENTSINK_MAP(CScanView, ICSFormView)
	ON_EVENT(CScanView, IDC_ICSDBEDTCTRL1, 1, CScanView::TerminationIcsdbedtctrl1, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CScanView::TerminationIcsdbedtctrl1(long nChar, long inplen, long kst)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	int seq, now_seq, ln;
	VARIANT var;
	char tmp[128];

	DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	seq = atoi( (const char*)var.pbVal );

	if( nChar == VK_RETURN ) {
		// 現在ポジションデータ取出し
		ln = m_ScList.GetCurrentLine();

		now_seq = SCAN_DDATA_LINE[ ln-1 ].Data.dd_seq;

		set_focus( SCANVW_PN );

		//修正画面へ
		if( seq - now_seq )
		{
			tomdfy2( seq, now_seq );
		}
		else
			tomdfy();
	}
	else if( nChar == VK_NEXT || nChar == VK_PRIOR )
	{
		set_focus( SCANVW_PN );
		m_ScList.PostMessage( WM_KEYDOWN, nChar );
		m_ScList.PostMessage( WM_KEYUP, nChar );
	}
	else if( nChar == VK_DELETE )
	{
		// 現在ポジションデータ取出し
		ln = m_ScList.GetCurrentLine();
		now_seq = SCAN_DDATA_LINE[ ln-1 ].Data.dd_seq;

		if( seq != now_seq )
		{
			Buzzer();

			sprintf_s(tmp, sizeof tmp, "%d", now_seq );

			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
		else
		{
			set_focus( SCANVW_PN );
			m_ScList.PostMessage( WM_KEYDOWN, nChar );
			m_ScList.PostMessage( WM_KEYUP, nChar );
		}
	}
	else if( nChar == VK_DOWN || nChar == VK_UP )
	{
		set_focus( SCANVW_PN );
	}
	else if( nChar == VK_TAB || nChar == VK_LEFT ) 
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			((CDialog*)this)->PrevDlgCtrl();
		else if( nChar == VK_LEFT )
			((CDialog*)this)->PrevDlgCtrl();
//		else
//			((CDialog*)this)->NextDlgCtrl();
	}
	else
	{
		// 現在ポジションデータ取出し
		ln = m_ScList.GetCurrentLine();
		now_seq = SCAN_DDATA_LINE[ ln-1 ].Data.dd_seq;

		sprintf_s(tmp, sizeof tmp, "%d", now_seq );

		var.pbVal = (BYTE*)tmp;
		DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
}


void CScanView::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CWnd* pwnd = GetFocus();
	if( pwnd->m_hWnd != m_ScList.m_hWnd )
		set_focus( SCANVW_PN );

	if( pNMUpDown->iDelta < 0 )
	{
		m_ScList.PostMessage( WM_KEYDOWN, VK_UP );
		m_ScList.PostMessage( WM_KEYUP, VK_UP );
	}
	else
	{
		m_ScList.PostMessage( WM_KEYDOWN, VK_DOWN );
		m_ScList.PostMessage( WM_KEYUP, VK_DOWN );
	}

	*pResult = 0;
}

void CScanView::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CWnd* pwnd = GetFocus();
	if( pwnd->m_hWnd != m_ScList.m_hWnd )
		set_focus( SCANVW_PN );

	if( pNMUpDown->iDelta < 0 )
	{
		m_ScList.PostMessage( WM_KEYDOWN, VK_PRIOR );
		m_ScList.PostMessage( WM_KEYUP, VK_PRIOR );
	}
	else
	{
		m_ScList.PostMessage( WM_KEYDOWN, VK_NEXT );
		m_ScList.PostMessage( WM_KEYUP, VK_NEXT );
	}

	*pResult = 0;
}


void CScanView::OnActivateJob(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	if( nState == WA_INACTIVE ) {
		m_ScList.HideTitleTipWindow();
	}
}

//
//	画面切り替え時に、仕訳表示項目を更新する
//
void CScanView::UpdateDataLineDisp()
{
	DT_RECT* dt; 
	int n, stchg = 0;
	int bm_chk, kj_chk, hjiyu_chk, zei_chk, denp_chk, dnptype_chk, syz_chk;
	BOOL bBmon, bKoji, bHjiyu, bZei, bDenp, bDnpType, bSyz;

	dt = m_ScList.GetDtRect();

	if( BMON_MST )	bBmon = (pAUTOSEL->DSPBMON_OPT == OPT_NONOPT);
	else			bBmon = FALSE;

	if( M_KOJI )	bKoji = (pAUTOSEL->DSPKOJI_OPT == OPT_NONOPT);
	else			bKoji = FALSE;

//	if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ )	bHjiyu = FALSE;
	if( IsHjiyuItemGyosyu() == 0 )	bHjiyu = FALSE;
	else {
		bHjiyu	= (pAUTOSEL->DSPHJIYU_OPT == OPT_NONOPT);
	}
	bZei	= (pAUTOSEL->DSPZEI_OPT == OPT_NONOPT);
	bSyz	= (pAUTOSEL->DSPSYZ_OPT == OPT_NONOPT);

	bDenp	= (pAUTOSEL->DSPDENP_OPT == OPT_NONOPT);

	bDnpType = DenpTypeMode();

	bm_chk = kj_chk = hjiyu_chk = zei_chk = denp_chk = dnptype_chk = syz_chk = 0;

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

		if( dt->DtID[n] == DATA_USER1 ) {
			if( bDnpType != dt->DtEnable[n] ) {
				stchg++;
			}
			dnptype_chk++;
//MyTrace( "UpdateDataLineDisp DATA_USER1 (dnptype_chk) = %d\n", dnptype_chk );
		}
	}

	if( BMON_MST ) {
		if( ! bm_chk && bBmon )		stchg++;
	}
	if( M_KOJI ) {
		if( ! kj_chk && bKoji )		stchg++;
	}

	//項目がない場合、追加する。
	if( bHjiyu && ! hjiyu_chk )		stchg++;
	if( bZei && ! zei_chk )			stchg++;
	if (bSyz && ! syz_chk)			stchg++;
	if( bDenp && ! denp_chk )		stchg++;
	if( bDnpType && ! dnptype_chk )	stchg++;

	if( stchg ) {
		SetListColumn( 1 );
		m_ScList.EnforceLayout();
		m_ScList.RedrawDataListCtrl();
	}

	// 09.24 /11
	if( IsNotMultiWindow() ) {
	}
	else {
		SCAN_SCROLL_NL = m_ScList.GetDispDataCount();
		if( SCAN_SCROLL_NL > SCAN_RECMAX )
			SCAN_SCROLL_NL = SCAN_RECMAX;
	}

	click_seljob( -1, 0 );
}


void CScanView::Rdadd_job()
{
	//画面表示
#ifdef CLOSE
	int cline;
	cline = m_ScList.GetCurrentLine();

	int n;
	if( (n = SCAN_DDATA_LINE[cline-1].ZREC_NO) ) {
		CRdregDlg dlg;
		dlg.m_pNowData	= &SCAN_ZREC[n-1];
		dlg.m_CmnId		= p_DBDinpView->m_CmnId;
		dlg.DoModal();
	}
#else
	// 追加数を増やす
	INT_PTR max;
	max = gSelArray.GetCount();
	CRdregDlg dlg;

	p_DBDinpView->Rddata_Check();

	if( max >= 1 ) {
		pDBzm->m_sortMode = (SC_Item.sc_type == SCT_ORDERSEQ) ? 0 : 1;

		if( max > RDADD_MAX ) {
			RegDataTorokMsg();

			int cline;
			cline = m_ScList.GetCurrentLine();

			int n;
			n = SCAN_DDATA_LINE[cline-1].ZREC_NO;
			
			if( n > 0 ) {
				int i, cnt, curidx = -1;
				for( i = cnt = 0; i < max; i++ ) {
					if( gSelArray[i] == SCAN_ZREC[n-1].m_seq ) {
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
		dlg.m_CmnId		= p_DBDinpView->m_CmnId;
		dlg.DoModal();
	}
	else {
		int cline;
		cline = m_ScList.GetCurrentLine();

		int n;
		if( (n = SCAN_DDATA_LINE[cline-1].ZREC_NO) ) {
			dlg.m_RegData[0] = SCAN_ZREC[n-1];
			dlg.m_recCnt	= 1;
			dlg.m_CmnId		= p_DBDinpView->m_CmnId;
			dlg.DoModal();
		}
	}
#endif
}

void CScanView::OnButtonF7()
{
	CWnd* pwnd;
	pwnd = GetFocus();

	if( pwnd->m_hWnd == m_ScList.m_hWnd ) {
		// 選択OFF
//		sel_shiftjob( 1, VK_SHIFT, FALSE );
//		sel_lineupdate();

		Rdadd_job();
	}
}

void CScanView::OnUpdateButtonF7(CCmdUI *pCmdUI)
{
	if( p_DBDinpView == NULL || bCONFIRM_MASTER ) pCmdUI->Enable( FALSE );
	else {
		pCmdUI->Enable( ! pDBzm->IsDelScan() );
	}
}



// 仕訳コピー 選択処理(Shift)
void CScanView::sel_shiftjob( int ln, long nChar, BOOL bPress )
{
	// SHIFT 押下し
	if( bPress ) {
		int index = m_OldIndex;
		int zno = SCAN_DDATA_LINE[ index ].ZREC_NO;
		int seq, i;
		BOOL bAdd = FALSE;

		if( zno > 0 ) {
			seq = SCAN_ZREC[ zno -1 ].m_seq;
		
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


/***********************************************
	int line	クリックして変更したライン


 ***********************************************/
void CScanView::click_seljob( int line, long nChar )
{
	int i, n, zidx, sgn, lnidx;

	int curidx = m_OldIndex;
	int curseq = 0;
	BOOL curAdd = FALSE;

	if( (nChar == VK_SHIFT || nChar == VK_CONTROL) && curidx >= 0 ) {

		//現在行も選択範囲に入れる。
		if( curidx >= 0 ) {
			if( SCAN_DDATA_LINE[curidx].ZREC_NO ) {
				zidx = SCAN_DDATA_LINE[curidx].ZREC_NO -1;
				sgn = 0;

				if( gSelSeq == 0 ) {
					gSelSeq = SCAN_ZREC[zidx].m_seq;
				}

				curseq = SCAN_ZREC[zidx].m_seq;

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
			for( i = 0; i <  SCAN_LN; i++ ) {
				if( SCAN_DDATA_LINE[i].ZREC_NO ) {
					zidx = SCAN_DDATA_LINE[i].ZREC_NO -1;

					if( gSelSeq == SCAN_ZREC[zidx].m_seq ) {
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
						if( SCAN_DDATA_LINE[i].ZREC_NO ) {
							zidx = SCAN_DDATA_LINE[i].ZREC_NO -1;

							for( n = 0; n < gSelArray.GetCount(); n++ ) {
								if( gSelArray[n] == SCAN_ZREC[zidx].m_seq ) {
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
						if( SCAN_DDATA_LINE[i].ZREC_NO ) {
							zidx = SCAN_DDATA_LINE[i].ZREC_NO -1;

							for( n = 0; n < gSelArray.GetCount(); n++ ) {
								if( gSelArray[n] == SCAN_ZREC[zidx].m_seq ) {
									sgn = n;
									break;
								}
							}
							if( sgn == -1 ) {
								gSelArray.Add( SCAN_ZREC[zidx].m_seq );
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
						if( SCAN_DDATA_LINE[i].ZREC_NO ) {
							zidx = SCAN_DDATA_LINE[i].ZREC_NO -1;

							for( n = 0; n < gSelArray.GetCount(); n++ ) {
								if( gSelArray[n] == SCAN_ZREC[zidx].m_seq ) {
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
						if( SCAN_DDATA_LINE[i].ZREC_NO ) {
							zidx = SCAN_DDATA_LINE[i].ZREC_NO -1;

							for( n = 0; n < gSelArray.GetCount(); n++ ) {
								if( gSelArray[n] == SCAN_ZREC[zidx].m_seq ) {
									sgn = n;
									break;
								}
							}
							if( sgn == -1 ) {
								gSelArray.Add( SCAN_ZREC[zidx].m_seq );
							}
						}
					}

					gSelDirection = SELCPY_DOWN;
					seljob = 1;
				}
			}

			if( ! seljob ) {
				if( SCAN_DDATA_LINE[lnidx].ZREC_NO ) {
					int start, end, ln_sel = 0;
					if( nChar == VK_SHIFT ) {
						// SHIFT
						gSelType	= SEL_SHIFT;

						// 移動先の選択状態をチェック
						zidx = SCAN_DDATA_LINE[lnidx].ZREC_NO - 1;
						for( n = 0; n < gSelArray.GetCount(); n++ ) {
							if( gSelArray[n] == SCAN_ZREC[zidx].m_seq ) {
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
							if( SCAN_DDATA_LINE[i].ZREC_NO ) {
								zidx = SCAN_DDATA_LINE[i].ZREC_NO -1;

								if( curAdd ) {
									if( curseq == SCAN_ZREC[zidx].m_seq )
										continue;
								}
								else {
									// 移動先が未選択のときは、現行の選択をはずさない。
									if( ! ln_sel ) {
										if( curseq == SCAN_ZREC[zidx].m_seq )
											continue;
									}
								}

								for( n = 0; n < gSelArray.GetCount(); n++ ) {
									if( gSelArray[n] == SCAN_ZREC[zidx].m_seq ) {
										sgn = n;
										break;
									}
								}
								if( sgn == -1 ) {
									gSelArray.Add( SCAN_ZREC[zidx].m_seq );
								}
								else {
									gSelArray.RemoveAt( n );
								}
							}
						}
					}
					else {
						zidx = SCAN_DDATA_LINE[lnidx].ZREC_NO -1;
						// CTRL キー
						gSelType	= SEL_CTRL;
						sgn = -1;

						for( n = 0; n < gSelArray.GetCount(); n++ ) {
							if( gSelArray[n] == SCAN_ZREC[zidx].m_seq ) {
								sgn = n;
								break;
							}
						}

						if( sgn == -1 ) {
							gSelArray.Add( SCAN_ZREC[zidx].m_seq );
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


// 選択ライン再描画
void CScanView::sel_lineupdate()
{
	if( gSelUpdate ) {
		int i, n, zidx, sgn, max;
		int curidx = m_ScList.GetCurrentLine() - 1;

		//現在行も選択範囲に入れる。
		if( gSelDirection != SELCPY_NONE ) {
			if( curidx >= 0 ) {
				if( SCAN_DDATA_LINE[curidx].ZREC_NO ) {
					zidx = SCAN_DDATA_LINE[curidx].ZREC_NO -1;
					sgn = 0;

					for( n = 0; n < gSelArray.GetCount(); n++ ) {
						if( gSelArray[n] == SCAN_ZREC[zidx].m_seq ) {
							sgn = 1;
							break;
						}
					}

					if( ! sgn ) {
						gSelArray.Add( SCAN_ZREC[zidx].m_seq );
					}
				}
			}
		}

		//画面表示
		int old_cline;
		old_cline = m_ScList.GetCurrentLine();

		m_ScList.RemoveAllDtListData();		//全表示データイニシャライズ

		max = SCAN_LN;
		for( i = 0 ; i < max ; ++i )	//表示データ作成
		{
			if( zidx = SCAN_DDATA_LINE[i].ZREC_NO )
			{
				sgn = 0;

				for( n = 0; n < gSelArray.GetCount(); n++ ) {
					if( gSelArray[n] == SCAN_ZREC[zidx-1].m_seq ) {
						SCAN_DDATA_LINE[i].Data.dd_dsign |= 0x0400;
						sgn = 1;
						break;
					}
				}

				if( ! sgn ) {
					SCAN_DDATA_LINE[i].Data.dd_dsign &= ~0x0400;
				}
			}
		}

		scan_ddata_dsp( SCAN_DDATA_LINE, i, 1 );	//データ表示

		m_ScList.SetCurrentLine( old_cline );
		m_ScList.RedrawDataListCtrl();

		gSelUpdate = FALSE;
	}
}


void CScanView::RButtonMenuJob( UINT nID, CPoint point )
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_SWKMENU));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	pPopup->EnableMenuItem( IDM_SWK_PASTE, MF_BYCOMMAND| MF_GRAYED );

	if( p_DBDinpView->m_bRGDTA_BUTTON_DISP ) {
		UINT nEnable = MF_BYCOMMAND | MF_ENABLED;
		if( bCONFIRM_MASTER ) {
			nEnable = MF_BYCOMMAND | MF_GRAYED;
		}
		pPopup->EnableMenuItem( IDM_REGDATA, nEnable );
		pPopup->RemoveMenu( IDM_REGDLGUSE, MF_BYCOMMAND );
	}
	else {
		pPopup->RemoveMenu( IDM_REGDATA,   MF_BYCOMMAND );
		pPopup->RemoveMenu( IDM_REGDLGUSE, MF_BYCOMMAND );
	}

	DWORD dwRetCode = pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
											  point.x, point.y, this);

	if( dwRetCode == IDM_SWK_COPY ) {
		SendMessage( WM_COMMAND, IDM_SWK_COPY, 0);
	}
	else if( dwRetCode == IDM_REGDATA ) {
		SendMessage( WM_COMMAND, ID_BUTTON_F7, 0);
	}
}


// 現在行がコピー用の選択仕訳の中に入っているか？
BOOL CScanView::CheckCopySeq( int line )
{
	int curidx, zidx, curseq, n;
	BOOL bRet = FALSE;
	curidx = line - 1;

	if( curidx >= 0 ) {
		if( SCAN_DDATA_LINE[curidx].ZREC_NO ) {
			zidx = SCAN_DDATA_LINE[curidx].ZREC_NO -1;

			curseq = SCAN_ZREC[zidx].m_seq;

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



BOOL CScanView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( wParam == IDC_SCANLIST ) {
		NM_DLCTMOUSE* nmh;
		nmh = (NM_DLCTMOUSE*)lParam;

		if( nmh->hdr.code == NM_RCLICK ) {

#ifndef TEST_CLOSE
			int c_idx, index, st;

			c_idx = m_ScList.GetCurrentLine() - 1;
			index = nmh->iLine -1;

			if( c_idx != index && index >= 0 ) {
			//	if( nmdt->iClick ) {
			//		long nChar = 0;
			//		if( nmdt->iClick & 0x10 )	nChar = VK_SHIFT;
			//		if( nmdt->iClick & 0x20 )	nChar = VK_CONTROL;
			//		click_seljob( nmdt->iLine, nChar );
			//	}

				Set_SeqInp( index );

				m_ScList.SetCurrentLine(nmh->iLine);

				if( ! CheckCopySeq( nmh->iLine ) ) {
					click_seljob( nmh->iLine, 0 );
				}

				st = _dline_chg( index );

				sel_lineupdate();

				if( st == -1 )
				{
					Buzzer();
					goto ERR_TAG;
				}
			}
#endif
			RButtonMenuJob( IDC_DTLIST, nmh->point );
		}
 	}

ERR_TAG:

	return ICSFormView::OnNotify(wParam, lParam, pResult);
}



void CScanView::OnSwkCopy()
{
	if( gSelArray.GetCount() == 0 ) {
		int curln = m_ScList.GetCurrentLine();

		if( curln > 0 ) {
			gSelSeq = SCAN_DDATA_LINE[ curln-1 ].Data.dd_seq;
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




#ifdef _SLIP_ACCOUNTS_
//-----------------------------------------------------------------------------
// タブクリックによる切替時の チェック
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CScanView::TabLbuttonDownJob()
{
	if( m_IsAbleImg ){
		if( m_pImgmng->IsDlgVisible() ){
			m_pImgmng->DispImgDlgOff();
		}
	}

	return 0;
}
#endif

#ifdef _SLIP_ACCOUNTS_
void CScanView::OnMenuSlip()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	if( m_bSLIP_MASTER && ! m_bOCRS_ONLY && m_pImgmng ){
		m_pImgmng->ImgOptionGoModal( this );
		m_pImgmng->GetImgDispOption( m_ImgDispOption );
	}
}

void CScanView::OnUpdateMenuSlip(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if( m_bSLIP_MASTER && ! m_bOCRS_ONLY ){
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
int CScanView::ImageOptionChanged()
{
	if( m_bSLIP_MASTER && m_pImgmng ){
		m_pImgmng->GetImgDispOption( m_ImgDispOption );
	}

	return 0;
}
#endif

// カラムクリックによるソート
void CScanView::OnScanListColumnClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_COLUMNCLICK* nmdt;
	nmdt = (NM_COLUMNCLICK*)pNMHDR;

	if( PROGRAM_OK == FALSE )
		return;

	CWaitCursor	wc;

	if( make_scanorder( nmdt->iItem, nmdt->iSort ) < 0 ) {
		ICSExit( 0, "仕訳データのソートができません！" );
		return;
	}

	PROGRAM_OK = FALSE;
	m_bDspScList = FALSE;

	{
		CDBINPDataRec *data;
		int idx;
		idx = m_ScList.GetCurrentLine()-1;

		if( idx >= SCAN_LN || idx < 0 )
		{
			m_ScanSeq = -1;
		}
		else {
			data = &SCAN_ZREC[ SCAN_DDATA_LINE[ idx ].ZREC_NO - 1 ];
			m_ScanSeq = data->m_seq;
		}
		//常に先頭
//		m_ScanSeq = -1;

		//現在カーソル行イニシャライズ
		NOW_INDEX = 0;
		// 表示行数リセット
		SCAN_LN = 0;
		//リードテーブルイニシャライズ
		for( int i = 0; i < SCAN_RECMAX; i++ ) {
			SCAN_ZREC[i].Reset();
		}

		//全表示データイニシャライズ
		m_ScList.RemoveAllDtListData();
		m_ScList.RedrawDataListCtrl();

		KEYSCAN_VISIBLE = TRUE;		//画面表示サインセット
	}
	// ここはコメントアウトして、データ表示のメッセージ関数を終えてから TRUE にする
/*	PROGRAM_OK = TRUE;	*/

	((CMainFrame*)GetDBDinpMainFrame())->SwkListSortSign( nmdt->iItem, nmdt->iSort );

	PostMessage( WM_DSPSCANTBL, 0 ); 
}

void CScanView::OnUpdateButtonF11(CCmdUI *pCmdUI)
{
	BOOL bEnable = TRUE;

	if( bCONFIRM_MASTER )	bEnable = FALSE;
	if( ! PROGRAM_OK )		bEnable = FALSE;
	if( ! IsMasterType(MST_INSERT) )
		bEnable = FALSE;

	if( pDBzm != NULL ) {
		if( Voln1 != NULL ) {
			if( Voln1->e_save )	bEnable = FALSE;
		}
	}
	pCmdUI->Enable( bEnable );
}


//#undef FILETrace
//#define FILETrace	MyTrace

// 仕訳移動
void CScanView::OnButtonF11()
{
	CSeqChgDlg	dlg;

	// 追加数を ５個へ
	INT_PTR max;
	max = gSelArray.GetCount();

	int st = IDCANCEL;

	int sort_id, sort_typ;
	((CMainFrame*)GetDBDinpMainFrame())->GetSwkListSortSign( sort_id, sort_typ );

	if( (sort_id != -1 && sort_id != DATA_SEQ && sort_id != DATA_DATE) ) {
		myICSMessageBox("仕訳をSEQ番号・日付順以外の並び順で表示している場合、\r\n仕訳移動はできません！",0,0,0,this);
		return;
	}

	if( seqchg_swkcheck( max ) ) {
		myICSMessageBox("確定・転送済み仕訳の移動はできません！",0,0,0,this);
		return;
	}

	if( max >= 1 ) {
		qsort_s( &gSelArray[0], gSelArray.GetCount(), sizeof DWORD, SelSeqCompare, pDBzm );

		dlg.m_SeqAry.Copy( gSelArray );
		dlg.m_sCnt = max;

		pDBzm->m_sortMode = (SC_Item.sc_type == SCT_ORDERSEQ) ? 0 : 1;

		st = dlg.DoModal();
	}
	else {
		int cline;
		cline = m_ScList.GetCurrentLine();

		int n;
		if( (n = SCAN_DDATA_LINE[cline-1].ZREC_NO) ) {
			dlg.m_SeqAry.Add( SCAN_ZREC[n-1].m_seq );
			dlg.m_sCnt	= 1;

			st = dlg.DoModal();
		}
	}

	if( st == IDOK ){
		// 移動先のSEQ が現在表示中の画面内にあるか？
		int n, max, rno, dispseq;
		max = SCAN_LN;
		dispseq = -1;

		m_moveSeq = dlg.m_SeqAry[0];
		BOOL bTopLine = FALSE;

		for( n = 0 ; n < max ; ++n ) {
			if( rno = SCAN_DDATA_LINE[n].ZREC_NO ) {
				if( n == 0 ) {
					//先頭行を移動する場合か？
					if( m_moveSeq == SCAN_ZREC[rno-1].m_seq ) {
						bTopLine = TRUE;
					}
				}

				if( dlg.m_BaseSeq == SCAN_ZREC[rno-1].m_seq ) {
					if( bTopLine ) {
						//先頭行を移動する場合は、先頭SEQ の次の行から
						rno = SCAN_DDATA_LINE[1].ZREC_NO;
						if( rno > 0 ) {
							dispseq = SCAN_ZREC[rno-1].m_seq;
							break;
						}
					}
					//先頭行の上に移動する場合は、移動する仕訳から表示
					if( n > 0 ) {
						//先頭SEQから表示する
						rno = SCAN_DDATA_LINE[0].ZREC_NO;
						dispseq = SCAN_ZREC[rno-1].m_seq;
					}
					break;
				}
			}
		}

		if( dispseq == -1 ) {
			dispseq = dlg.m_SeqAry[0];
		}

		scan_seqchg_read( 0, dispseq );
		pDBzm->insseq_make() = FALSE;

	//	scan_init_rd( 0, 0, 1, dispseq );

		// 選択OFF
		sel_shiftjob( 1, VK_SHIFT, FALSE );
	}
}




//------------------------------------------------------------------
//	仕訳移動時の確定仕訳チェック
//
//------------------------------------------------------------------
BOOL CScanView::seqchg_swkcheck( int aryCnt )
{
	BOOL bRet = FALSE;
	int i, n;

	// [改良依頼No.22-0633]
	//仕訳移動に制限をかける条件を対象が確定仕訳かどうかのみから
	//確定仕訳修正不可のマスターかつ、上手くん参照モードまたは確定仕訳に修正する
	//CRpKeyDlg::datacmpの一括修正時の確定仕訳判定に合わせた処理への修正
	BOOL allowModify = IsModifyConfirmData();
	BOOL isRefMode   = IsJzSansyo();

	if( aryCnt >= 1 ) {
		for( i = 0; i < SCAN_RECMAX; i++ ) {
			if( SCAN_ZREC[i].m_seq > 0 ) {
				for( n = 0; n < aryCnt; n++ ) {
					if( gSelArray[n] == SCAN_ZREC[i].m_seq ) {
						//if( (SCAN_ZREC[i].m_dsign[7] & 0x30) ) {
						// [改良依頼No.22-0633]
						if ( !allowModify && (isRefMode||(SCAN_ZREC[i].m_dsign[7] & 0x30)) ){
							bRet = TRUE;
							break;
						}
					}
				}
			}
		}
	}
	else {
		int cline;
		cline = m_ScList.GetCurrentLine();

		if( (n = SCAN_DDATA_LINE[cline-1].ZREC_NO) ) {
			//if( (SCAN_ZREC[n-1].m_dsign[7] & 0x30) ) {
			//[改良依頼No.22-0633]
			if(!allowModify && (SCAN_ZREC[n-1].m_dsign[7] & 0x30) ) {
				bRet = TRUE;
			}
		}
	}

	return bRet;
}



//------------------------------------------------------------------
//	仕訳移動後の 初期検索
//	int scan_sgn	0: 通常検索		1: 取消仕訳検索
//
//------------------------------------------------------------------
void CScanView::scan_seqchg_read( int scan_sgn, int dispseq/*=-1*/ )
{
FILETrace( "@Scan::scan_seqchg_read TOP scan_sgn = %d, dispseq = %d\n", scan_sgn, dispseq );

	// 仕訳番号入力値イニシャライズ
	VARIANT var;
	var.pbVal = NULL;
	DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

	//
	// 前準備
	PROGRAM_OK = FALSE;
	m_bDspScList = FALSE;

	//テーブル作成
	// データベース 検索
	int st;
	RequeryStr.Empty();
	st = DB_DataScan( RequeryStr );

	int sort_id, sort_typ;
	((CMainFrame*)GetDBDinpMainFrame())->GetSwkListSortSign( sort_id, sort_typ );

FILETrace( "@Scan::scan_seqchg_read  sort_id = %d, sort_typ = %d\n", sort_id, sort_typ );

	if( sort_id != -1 && st != -1 ) {
		if( make_scanorder( sort_id, sort_typ ) < 0 ) {
			ICSExit( 0, "仕訳データのソートができません！" );
			return;
		}
	}

	{
		//現在カーソル行イニシャライズ
		NOW_INDEX = 0;
		// 表示行数リセット
		SCAN_LN = 0;
		//リードテーブルイニシャライズ
		for( int i = 0; i < SCAN_RECMAX; i++ ) {
			SCAN_ZREC[i].Reset();
		}
		m_ScanSeq = dispseq;

		//全表示データイニシャライズ
		m_ScList.RemoveAllDtListData();
		m_ScList.RedrawDataListCtrl();

		KEYSCAN_VISIBLE = TRUE;		//画面表示サインセット
	}

	// データ表示のメッセージ関数を終えてから PROGRAM_OK = TRUE にする
	PostMessage( WM_DSPSCANTBL, st ); 

FILETrace( "@Scan::scan_seqchg_read END st = %d\n", st );

	return;
}


// 仕訳移動後の 現在行表示
void CScanView::seqchg_curline()
{
	if( m_moveSeq != -1 ) {

		int n, max, rno, line = 0;
		max = SCAN_LN;

		for( n = 0 ; n < max ; ++n ) {
			if( rno = SCAN_DDATA_LINE[n].ZREC_NO ) {
				if( m_moveSeq  == SCAN_ZREC[rno-1].m_seq ) {
					line = n + 1;
					break;
				}
			}
		}

		if( line ) {
			m_ScList.SetCurrentLine( line );
			m_ScList.RedrawDataListCtrl();	// 再描画
			Set_SeqInp( line-1 );
		}

		m_moveSeq = -1;
	}
}


int CScanView::GetScListCount()
{
#define SCAN_SCROLL_DEF		22

	int rval = 0;
	if( SCAN_SCROLL_NL <= SCAN_SCROLL_DEF )
		rval = SCAN_SCROLL_DEF;
	else
		rval = SCAN_SCROLL_NL;

	return rval;
}

// 伝票番号 入力漏れチェック
BOOL CScanView::DenpNoInpCheck( int denp_no )
{
	BOOL	bRet = FALSE;

	if( SC_Item.sc_noinp != SC_NOINP_DENP )	return FALSE;

	// 伝票番号 入力あり
	if( denp_no != -1 )
		return FALSE;

	return TRUE;
}


// vector 内の空き番を探す。
int get_zrecno( vector<int>& vec_recno )
{
	const int MIN_NO = 1;
	const int MAX_NO = SCAN_RECMAX;

	std::sort(vec_recno.begin(), vec_recno.end());

	size_t index = 0;
	int no = MIN_NO;
	while (no <= MAX_NO && index < vec_recno.size()) {
		if (no < vec_recno[index])
			return no;
		else if (no == vec_recno[index]) {
			no++;
			index++;
		} else {
			index++;
		}
	}

	if (no <= MAX_NO)
		return no;
	else
		return 0;
}



void CScanView::set_insdata_toscan( int ins_seq, int btm_seq )
{
	int n, max, rno, scrol_max;
	max = SCAN_LN;

	int ins_index = -1;
	int btm_index = -1;

	for( n = max-1 ; n >= 0 ; n-- ) {
		if( rno = SCAN_DDATA_LINE[n].ZREC_NO ) {
			if( ins_seq == SCAN_ZREC[rno-1].m_seq ) {
				ins_index = n;
				break;
			}
			else if( btm_seq == SCAN_ZREC[rno-1].m_seq ) {
				btm_index = n;
			}
		}
	}

	if( ins_seq == btm_seq ) {
		btm_index = ins_index;
	}

	if( ins_index == -1 )
		return;

	//挿入分を含めても、リストの行数の範囲内。
	if( (SCAN_LN + SCAN_PGTBL.ins_data) < SCAN_SCROLL_NL ) {
		scrol_max = SCAN_SCROLL_NL - (ins_index);
		scan_scroll_down( &SCAN_DDATA_LINE[ins_index], scrol_max, SCAN_PGTBL.ins_data );
		SCAN_LN += SCAN_PGTBL.ins_data;
	}
	else {
		//修正画面の最終データは、リスト上から消えないように、挿入する。
		if( (btm_index+1 + SCAN_PGTBL.ins_data) > SCAN_SCROLL_NL ) {
			int mv_ln = 0;
			mv_ln = SCAN_SCROLL_NL - (btm_index+1);
			if( mv_ln < 0 )	mv_ln = 0;

			if( mv_ln > 0 ) {
				scrol_max = SCAN_SCROLL_NL - (ins_index);
				scan_scroll_down( &SCAN_DDATA_LINE[ins_index], scrol_max, mv_ln );
				ins_index += mv_ln;
			}
			int nokori_ins = SCAN_PGTBL.ins_data - mv_ln;

			if( nokori_ins ) {
				scrol_max = ins_index;

				int up_cnt;
				if( nokori_ins > scrol_max )	up_cnt = scrol_max;
				else							up_cnt = nokori_ins;

				scan_scroll_up( &SCAN_DDATA_LINE[0], scrol_max, up_cnt );
			}
		}
		else {
			scrol_max = SCAN_SCROLL_NL - (ins_index);
			scan_scroll_down( &SCAN_DDATA_LINE[ins_index], scrol_max, SCAN_PGTBL.ins_data );
		}

		if( SCAN_LN < SCAN_SCROLL_NL ) {
			SCAN_LN = SCAN_SCROLL_NL;
		}
	}

	CArray<CDBINPDataRec, CDBINPDataRec&>	recArray;

	int ins_id = SCAN_PGTBL.ins_data-1;
	max = SCAN_LN;

	for( n = max-1 ; n >= 0 ; n-- ) {
		if( rno = SCAN_DDATA_LINE[n].ZREC_NO ) {
			recArray.InsertAt(0, SCAN_ZREC[rno-1] );
		}
		else {
			CDBINPDataRec	insd;
			//挿入データセット
			if( ins_id >= 0 ) {
				p_DBDinpView->get_insrec( SCAN_PGTBL.insSeq[ins_id], &insd );
				ins_id--;
				recArray.InsertAt( 0, insd );
			}
		}
	}

	INT_PTR reccnt = recArray.GetCount();

	for( n = 0; n < SCAN_RECMAX; n++ ) {
		if( n < reccnt ) {
			SCAN_ZREC[n] = recArray[n];
//TRACE( "set_insdata_toscan SCAN_ZREC[%d] seq = %d\n", n,  SCAN_ZREC[n].m_seq );
		}
		else {
			SCAN_ZREC[n].Reset();
		}
	}

	for( n = 0; n < max; n++ ) {
		SCAN_DDATA_LINE[n].ZREC_NO = n+1;
	}

	SCAN_PGTBL.scanInsSeq.Append( SCAN_PGTBL.insSeq );
	SCAN_PGTBL.ins_data = 0;
	SCAN_PGTBL.insSeq.RemoveAll();
	SCAN_PGTBL.ins_query = 1;
}


// inpview で表示している 底 SEQ から、bottom index をセットする。
void CScanView::set_scan_bottom( int btm_seq )
{
	int n, max, rno;
	max = SCAN_LN;

	for( n = max-1 ; n >= 0 ; n-- ) {
		if( rno = SCAN_DDATA_LINE[n].ZREC_NO ) {
			if( btm_seq == SCAN_ZREC[rno-1].m_seq ) {
				SCAN_RD_BOTOM = n;
				break;
			}
		}
	}
}


void CScanView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo )
{
	CVPrint* prn = &CDBbaseView::m_Vprn;
	prn->PrePrint(pDC, MM_LOMETRIC);
}

void CScanView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CVPrint* prn = &CDBbaseView::m_Vprn;
	ICSFormView::OnPrepareDC(pDC, pInfo);
	if(pDC->IsPrinting() != 0)	prn->InitPage(pDC,0,0);
}

BOOL CScanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	CVPrint* prn = &CDBbaseView::m_Vprn;

	if (prn->InitPrint(2, pInfo, &CDBbaseView::m_Pset, this) != 0) {
		if(prn->m_ApErrCod == 2) {
			ICSExit(0, "出力処理エラーの為、プログラムを終了します。");
		}
		return	FALSE;
	}

	int		st =	DoPreparePrinting(pInfo);

	CDBbaseView::m_End_PrintDlg = 0;
	if ( st == FALSE ) {
		CDBbaseView::m_End_PrintDlg = prn->End_PrintDlg(this);
		if( CDBbaseView::m_End_PrintDlg < 1 ) {	// 返送値＝１の場合、印刷プレビュー処理へ
			// 印刷ダイアログにおいて、エラー 又は キャンセル押下時
			if (CDBbaseView::m_ImgPrintFlg == PRINT_VERIFY ) {	// 一括検証
				m_pImgmng->ImgPrintResetDlg();
			}
			else {
				m_pImgmng->ImgEndPrintData();
			}
			CDBbaseView::m_ImgPrintFlg = PRINT_NONE;
		}
	}
	return st;
}


void CScanView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo )
{
	CVPrint* prn = &CDBbaseView::m_Vprn;

	prn->EndPrint();

	if (CDBbaseView::m_ImgPrintFlg) {
		if (prn->PreviewOnPrintdlg())
			return;
//		m_pImgmng->ImgPrintResetDlg();
		if( CDBbaseView::m_ImgPrintFlg == PRINT_VERIFY ) {	// 一括検証
		m_pImgmng->ImgPrintResetDlg();
		}
		else {
			m_pImgmng->ImgEndPrintData();
		}

		CDBbaseView::m_ImgPrintFlg = PRINT_NONE;
	}
}


void CScanView::OnPrint(CDC* pDC, CPrintInfo*)
{
	// TODO: コントロールを印刷するコードを追加してください。
	CVPrint* prn = &CDBbaseView::m_Vprn;
	if ( prn->StartPageJob(pDC) != 0 )	return;
//	if ( prn->SetCoMode(1) )				return;	// 座標モード

	if(CDBbaseView::m_ImgPrintFlg == PRINT_VERIFY){
		int rt = 	m_pImgmng->ImgPrintVerify( prn );
		if(rt == 1)	prn->LastPage();
	}
	else if(CDBbaseView::m_ImgPrintFlg == PRINT_DOCIMG){
		CDocPInfoArray Doc_pArry;	// 出力情報Array
		//
		// ImgPrintVerifyは最終ページの時1として返送値を返します。
		int rt = m_pImgmng-> ImgPrintData ( prn, &Doc_pArry);
		if(rt == 1)	prn->LastPage();
	}

	prn->EndPageJob(pDC);
}


BOOL CScanView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	SCROLLINFO	si = {0};
	si.cbSize = sizeof(SCROLLINFO);
	GetScrollInfo(SB_HORZ, &si );

	CRect viewRect;
	GetWindowRect( &viewRect );
//MyTrace( "viewRect.Width = %d, si.nMax = %d\n", viewRect.Width(), si.nMax );

	if( viewRect.Width() < si.nMax ) {
		//スクロール あり
		return FALSE;
	}

	return ICSFormView::OnMouseWheel(nFlags, zDelta, pt);
}




// ツールバーより Home
//
void CScanView::OnButtonHome()
{
//	CWnd* pwnd;
//	pwnd = GetFocus();

	m_ScanSeq = -1;

	SCAN_LN = 0;
	//リードテーブルイニシャライズ
	for (int i = 0; i < SCAN_RECMAX; i++) {
		SCAN_ZREC[i].Reset();
	}
//SCAN_SCROLL_NL - SCAN_LN
	PostMessage( WM_DSPSCANTBL, 0 ); 

}


//
int CScanView::Reload_DataScan()
{
	// キー項目入力なし or 取消仕訳検索
	int st;
	CString stmp;

	if( !isScanKeyInp() ) {
		st = DB_DataScan(stmp);
	}
	else {

		pDBzm->dbdata->MoveFirst();

		stmp.Empty();
		CString filter;
		int cnt = 0;
		//何かしら検索項目があれば、前に表示した分で
		do {
			if( pDBzm->dbdata->st != -1 ) {
				stmp.Format("seq = %d", pDBzm->dbdata->seq );
				if( cnt > 0 ) {
					filter += " OR ";
				}
				filter += stmp;
				cnt++;
				if( cnt >= 2000 ) {
					break;
				}
			}
		} while( pDBzm->dbdata->MoveNext() == 0 );

		int DtMode, DtOwner;

		// 表示順
		SCAN_ITEMEX *key = &SC_Item;
		switch( (key->sc_type & 0x0f) ) {
		case SCT_ORDERSEQ:	DtMode = 0;	break;
		default:			DtMode = 1;	break;
		}
		// 対象データ
		DtOwner = GetDataOwnerSign();

		pDBzm->dbdata_Requery() = TRUE;
		pDBzm->dbdata_Job() = DBDATA_JOB_SCAN;
		pDBzm->dbdata_Sort() = (DtMode == 0) ? DBDATA_SORT_SEQ : DBDATA_SORT_DATE;
		pDBzm->dbdata_Speed() = FALSE;

		if( IsMasterType(MST_INSERT) ) {
			st = pDBzm->dbdata->Requery(DtOwner, 0, filter, 0, (DtMode == 0) ? 3 : DtMode);
		}
		else {
			st = pDBzm->dbdata->Requery(DtOwner, 0, filter, 0, (DtMode == 0) ? 0 : DtMode);
		}

		if( !st ) {
			InitScanPgTBL(&SCAN_PGTBL);

			SCAN_PGTBL.data_owner = DtOwner;
			SCAN_PGTBL.data_mode = DtMode;
			SCAN_PGTBL.filter = filter;
			SCAN_PGTBL.tbl_end = 1;

			// 修正前仕訳テーブルをクリア
			SCMDFY_SEQ.fill();
		}
	}

	return 0;
}


// 原票表示 位置矩形取得
void CScanView::GetImgDispRECT(RECT* dispRect)
{
	// イメージ表示コントロール
	RECT	clRect;
	GetClientRect(&clRect);
	this->ClientToScreen(&clRect);

	int wType = GetMultiWindowType();
	if( (wType == MULTIWINDOW_FULL) || (wType == MULTIWINDOW_SIMPLE) ) {
		GetClientRect(&clRect);
		this->ClientToScreen(&clRect);

		long adWidth = (clRect.right - clRect.left) / 3;
		long adHeight = (clRect.bottom - clRect.top) / 4 * 3;

		clRect.left = clRect.right;
		clRect.right = clRect.left + adWidth;
		clRect.bottom = clRect.top + adHeight;

		int maxHeight = GetSystemMetrics(SM_CYSCREEN);	// 高さ
		int maxWidth = GetSystemMetrics(SM_CXSCREEN);		// 幅

		if( clRect.bottom > maxHeight ) {
			long sub = clRect.bottom - maxHeight;
			clRect.bottom -= sub;
			clRect.top -= sub;
		}
		if( clRect.right > maxWidth ) {
			long sub = clRect.right - maxWidth;
			clRect.left -= sub;
			clRect.right -= sub;
		}
	}
	else {
		long adWidth = (clRect.right - clRect.left) / 2;
		long adHeight = (clRect.bottom - clRect.top) / 2;

		clRect.left += adWidth;
		clRect.bottom -= adHeight;

		//		this->ScreenToClient( &clRect );
	}
	*dispRect = clRect;
}


//原票ダイアログ 再表示
int CScanView::ReDispGenpyoImg()
{
	if( m_IsAbleImg ) {
		if( m_ImgDispOption.dtypeInConfirm == ID_DISP_IN_CONFIRM ) {
#ifdef SCANNER_SPD_THREAD
			int curidx = m_ScList.GetCurrentLine() - 1;
			if( curidx >= 0 ) {
				seqque.push(SCAN_DDATA_LINE[curidx].Data.dd_seq);
			}

			CWinThread* th = AfxBeginThread(DispImgDlgThreadFunc, this, THREAD_PRIORITY_HIGHEST);

#else
			int curidx = m_ScList.GetCurrentLine() - 1;
			if( curidx >= 0 ) {
				IMG_DLGOPTION	dlgOption;
				memset(&dlgOption, '\0', sizeof(IMG_DLGOPTION));
				dlgOption.btnDispSw |= (0x04 | 0x08);
#ifdef _SCANNER_SV_
				if( !m_bOCRS_ONLY ) {
					dlgOption.imgDispSw |= 0x02;
				}
#endif
				CRect rc;
				GetImgDispRECT(&rc);
				//	if( (nmdt->nFlags & 0x02) || (nmdt->nFlags & 0x04) || (nmdt->iClick & 0x0f) ) {
				m_pImgmng->DispImgDlgOn(pDBzm, SCAN_DDATA_LINE[curidx].Data.dd_seq, this, dlgOption, &rc);
				m_pImgmng->DispDlgSetCallBack(ImgDlgCallbackFunc, this);
				//	}
			}
		}
#endif
	}
	return 0;
}


//WIZダウンロード仕訳を検索一覧に
void CScanView::scan_init_wizread(CString filter)
{
	FILETrace("@Scan::scan_init_wizread filter = %s, NL() = %d, mode = %02x\n", filter, SCROLL_NL(), INP_mode);
	// 仕訳選択ボタン　淡色表示
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);

	// スキャンオンリー時
	if( bSCAN_ONLY_MODE )
		GetDlgItem(IDC_BUTTON1)->SetWindowText("<<終了(End)");

	// 仕訳番号入力値イニシャライズ
	VARIANT var;
	var.pbVal = NULL;
	DBdata_set(this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0);

	//
	// 前準備
	PROGRAM_OK = FALSE;

	m_bDspScList = FALSE;
	{
		//現在カーソル行イニシャライズ
		NOW_INDEX = 0;

		// 表示行数リセット
		SCAN_LN = 0;

		// フォークで検索画面 表示ＳＥＱ
		m_ScanSeq = -1;

		//リードテーブルイニシャライズ
		for( int i = 0; i < SCAN_RECMAX; i++ ) {
			SCAN_ZREC[i].Reset();
		}

		//全表示データイニシャライズ
		m_ScList.RemoveAllDtListData();
		m_ScList.RedrawDataListCtrl();


		KEYSCAN_VISIBLE = TRUE;		//画面表示サインセット
	}
	// ここはコメントアウトして、データ表示のメッセージ関数を終えてから TRUE にする
/*	PROGRAM_OK = TRUE;	*/

	// キーを受取るためにガイド表示にフォーカスする。
	GetDlgItem(IDC_WANING2)->SetFocus();

	// 検索前に再描画
	// 子ｳｨﾝﾄﾞｳ 再描画
	CWnd* pwnd = GetWindow(GW_CHILD);
	while( pwnd ) {
		pwnd->UpdateWindow();
		pwnd = pwnd->GetWindow(GW_HWNDNEXT);
	}
	UpdateWindow();

	int cnt;
	cnt = pDBzm->knm_info->reccount;
	cnt += (cnt / 4);

	m_TaiTtl.taidata_inz(cnt);	// 貸借データエリアイニシャライズ

	inpmdisp(_KEYSCAN);

	if( m_IsAbleImg ) {
		if( m_pImgmng && m_pImgmng->IsDlgVisible() ) {
			m_pImgmng->DispImgDlgOff();
		}
	}
	//テーブル作成
	// データベース 検索
	int st;

	// 検索対象データは、権限があれば、全ユーザ
	if( !pDBzm->data_authority() ) {
		SC_Item.sc_duser = SCD_CURUESR;
	}
	else {
		SC_Item.sc_duser = SCD_ALLDATA;
	}
	RequeryStr = filter;
	st = DB_DataScan(RequeryStr);
	PostMessage(WM_DSPSCANTBL, st);

	FILETrace("@Scan::scan_init_wizread END\n");

	return;
}
