// DBAbookView.cpp : CDBAbookView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "DBDinp.h"

#include "MainFrm.h"
#include "DBDinpDoc.h"
#include "DBDinpView.h"

#include "icsocx.h"
#include "ICSDisp.h"

#include "input1.h"
#include "external.h"
#include "ACSelDlg.h"

#include "DBAbookView.h"
#include "DinpINI.h"
#include "dinpsub.h"
#include "MstRw.h"

#include "MnthDlg.h"
#include "ACDspOptDlg.h"
#include "BmnChk.h"

#include "ACInpSupDlg.h"
#include "RdregDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBAbookView
extern CDipZmSub* pDBzm;
extern CDBSyohi*  pDBsy;
extern CBrnTky m_BrTek;
extern struct _AUTOSEL* pAUTOSEL;
extern CBmnHaniCheck	BmChk;
extern CKojiDataCheck	KjChk;

extern CDBDinpView *p_DBDinpView;

extern BOOL	PROGRAM_OK;
//BOOL	FNC_BUTTON;

// 仕訳状態ビットテーブル
#include "bit.h"
extern CBit	MODEFY_BIT;		//修正仕訳ビットテーブル
extern CBit	SYZMDFY_BIT;	//消費税サイン修正ビットテーブル

extern	char	Image[];

#include "SEQ.h"
extern	CSEQ	SCMDFY_SEQ;		//定型仕訳ソート用 データＳＥＱテーブル


BOOL IsGenkin(  CZVOLUME* pz, DWORD kncd );
BOOL IsUrikake( CZVOLUME* pz, DWORD kncd );
BOOL IsKaikake( CZVOLUME* pz, DWORD kncd );
BOOL IsKaribarai( DWORD kncd );
BOOL IsKariuke( DWORD kncd );
BOOL IsBaseCheckKamoku( CZVOLUME* pz, DWORD kncd );


const char* swk_kansei = "最後の仕訳が不完全です。仕訳を完成させますか？\r\n「いいえ」の場合この仕訳は破棄されます。";


#if (ACBFOCUS_DEBUG == 1)
#define set_focus(a)		set_focusTrace(a, 0, __LINE__, __FUNCTION__, __FILE__)
#define _set_focus(a,b)	_set_focusTrace(a, b, 0, __LINE__, __FUNCTION__, __FILE__)
#else
#undef set_focus
#undef _set_focus
#endif


IMPLEMENT_DYNCREATE(CDBAbookView, CDBbaseView)

BEGIN_MESSAGE_MAP(CDBAbookView, CDBbaseView)
	//{{AFX_MSG_MAP(CDBAbookView)
	ON_COMMAND(ID_BUTTON_END, OnButtonEnd)
	ON_COMMAND(ID_BUTTON_F11, OnButtonF11)
	//}}AFX_MSG_MAP
	// 標準印刷コマンド
//	ON_COMMAND(ID_BUTTON_F5, CDBbaseView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CDBbaseView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CDBbaseView::OnFilePrintPreview)

	ON_NOTIFY( DLN_BEGINLABELEDIT, IDC_ACBOOKLIST, OnBeginLabelEdit )
	ON_NOTIFY( DLN_ENDLABELEDIT,   IDC_ACBOOKLIST, OnEndLabelEdit )
	ON_NOTIFY( DLN_SETDISPINFO,	IDC_ACBOOKLIST, OnSetDispInfo )
	
	ON_MESSAGE( WM_RETROFOCUS, OnRetroFocus)
	ON_COMMAND(ID_BUTTON_F9, &CDBAbookView::OnButtonF9)

	ON_MESSAGE( WM_SKBN_INEND, OnSkbnInend)
	ON_MESSAGE( WM_SKBN_ICHIRN, DspSkbnIchiran)
	ON_MESSAGE( WM_SKBN_LINEDISP, OnSkbnLineDisp)
	ON_MESSAGE( ICS_SELDATE_MESSAGE_CHANGE, OnDateSelectMessageChangeMNTHSEL)

	ON_MESSAGE( WM_HSINP_INEND, OnHusenInpMsg )
	ON_MESSAGE( WM_SUBDATA_INEND, OnSubDataInEnd)

	ON_MESSAGE( WM_HJIYU_INEND, OnHjiyuInEnd )
	ON_MESSAGE( WM_HJIYU_SEL, OnHjiyuSel )
	ON_MESSAGE( WM_SELGUIDEMSG, OnSelGuideMsg)

	ON_COMMAND(ID_BUTTON_F8, &CDBAbookView::OnButtonF8)
	ON_COMMAND(ID_BUTTON_F7, &CDBAbookView::OnButtonF7)
//ON_COMMAND(ID_BUTTON_F3, &CDBAbookView::OnButtonF3)
ON_COMMAND(ID_BUTTON_F12, &CDBAbookView::OnButtonF12)
ON_BN_CLICKED(IDC_BUTTON_DISP, &CDBAbookView::OnBnClickedButtonDisp)
ON_UPDATE_COMMAND_UI(ID_BUTTON_F4, &CDBAbookView::OnUpdateButtonF4)
ON_UPDATE_COMMAND_UI(ID_BUTTON_F7, &CDBAbookView::OnUpdateButtonF7)
ON_UPDATE_COMMAND_UI(ID_BUTTON_F8, &CDBAbookView::OnUpdateButtonF8)
ON_UPDATE_COMMAND_UI(ID_BUTTON_F9, &CDBAbookView::OnUpdateButtonF9)
ON_UPDATE_COMMAND_UI(ID_BUTTON_F12, &CDBAbookView::OnUpdateButtonF12)
ON_UPDATE_COMMAND_UI(ID_BUTTON_F2, &CDBAbookView::OnUpdateButtonF2)
ON_COMMAND(ID_BUTTON_F2, &CDBAbookView::OnButtonF2)
ON_COMMAND(ID_BUTTON_INSERT, &CDBAbookView::OnButtonInsert)
ON_UPDATE_COMMAND_UI(ID_BUTTON_INSERT, &CDBAbookView::OnUpdateButtonInsert)
ON_WM_MOUSEWHEEL()

ON_COMMAND(ID_BUTTON_DELETE, &CDBAbookView::OnButtonDelete)
ON_UPDATE_COMMAND_UI(ID_BUTTON_DELETE, &CDBAbookView::OnUpdateButtonDelete)

ON_COMMAND(ID_BUTTON_F6, &CDBAbookView::OnButtonF6)
ON_UPDATE_COMMAND_UI(ID_BUTTON_F6, &CDBAbookView::OnUpdateButtonF6)
ON_COMMAND(ID_BUTTON_F3, &CDBAbookView::OnButtonF3)
ON_WM_SIZE()
ON_COMMAND(ID_BUTTON_F5, &CDBAbookView::OnButtonF5)
ON_UPDATE_COMMAND_UI(ID_BUTTON_F5, &CDBAbookView::OnUpdateButtonF5)
ON_COMMAND(ID_BUTTON_F4, &CDBAbookView::OnButtonF4)

	ON_MESSAGE( WM_DBDINPMSG, OnDBDinpMsg)
	ON_MESSAGE( WM_REGDATA, OnRGDTsel )

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBAbookView クラスの構築/消滅

CDBAbookView::CDBAbookView()
	: CDBbaseView(CDBAbookView::IDD), m_List(110)
{
	//{{AFX_DATA_INIT(CDBAbookView)
		// メモ: ClassWizard はこの位置にメンバの初期化処理を追加します。
	//}}AFX_DATA_INIT
	
	//注意！この関数を変更してはいけません
	FirstTime()	=	TRUE;

	m_BaseCode = "01010101";
	m_BaseAtt = 0;
	m_BaseEda = -1;
	m_BaseBmn = -1;
	m_BaseKoji.Empty();

	m_BaseDate = -1;
	m_BaseOfst = m_BaseEndOfs = -1;
	m_BaseKsign = 0;

	PROGRAM_OK = FALSE;
//	FNC_BUTTON = FALSE;
	m_bMnthFix = FALSE;

	m_NowPN = m_NowLine = -1;

	m_bIgnoreDateTerm	= FALSE;
	m_bIgnoreTermVal	= FALSE;
	// 基本項目設定時は、このサインをセットする
	bINP_mode = _RETREV;

	m_TKsyz = m_TKsyz2 = 0;
	m_InsLine = 0;
	m_bAutoSwk = FALSE;

	m_bSyzYunyu = m_bSyzSotoMen = FALSE;
	m_pnZeiSotoMen = 0;

	m_ListCnt = -1;
	m_dwOpt = 0;
	
	::ZeroMemory( m_svInpDate, sizeof m_svInpDate );

	m_bLinChg = FALSE;
	m_bReloadFocus = FALSE;

	m_bTkyLineChg = FALSE;

	m_sdbtBmn = m_screBmn = -1;

	m_SelDirection = 0;
	m_SelType = 0;
	m_SelSeq = 0;
	m_SelUpdate = FALSE;

	RdataTblIniz();

	m_pRDremainDlg = NULL;
	m_bLineDispChg = FALSE;

	m_bTermValLinChg = FALSE;

	// インボイス対応
	m_InpMode = INPMODE_NORMMAL;	// 通常入力
	m_bMnthFocusIgnore = FALSE;
}

CDBAbookView::~CDBAbookView()
{
	SubDataInpDelete();
}



void CDBAbookView::DoDataExchange(CDataExchange* pDX)
{
	CDBbaseView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBAbookView)
	DDX_Control(pDX, IDC_ICSDBEDTCTRL1, m_Input);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL2, m_BmnInp);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL3, m_KojiInp);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL4, m_KamokInp);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL5, m_BrnInp);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ACBOOKLIST, m_List);
	DDX_Control(pDX, IDC_MNTHSEL, m_selDate);
	DDX_Control(pDX, IDC_ST_BMN, m_StBmn);
	DDX_Control(pDX, IDC_ST_KOJI, m_StKoji);
}

BOOL CDBAbookView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CDBbaseView::PreCreateWindow(cs);
}

void CDBAbookView::BeginApp()
{
	//二回目以降は処理を行いません。注意！これより以下の二行は変更してはいけません。
//	if( !FirstTime() )	return;
	FirstTime()	=	FALSE;

//	UpdateAllChild();

	//この位置にアプリケーションの初期化処理を記述して下さい。
	enable_fixval( TRUE );
	BaseWndMove();

	try {
		// 選択初期化 & 消費税画面作成
		CDBbaseView::InitWork(TRUE);

		// サブ入力画面
		SubDataInpMake();

		// 輸入仕入れサインを クリア
		m_bSyzYunyu = FALSE;
		m_bSyzSotoMen = FALSE;
		m_pnZeiSotoMen = 0;
		// 仕訳検索条件
		m_dwQuery = ACQ_NONE;

		int opt = ((CMainFrame*)GetDBDinpMainFrame())->AcbkOption();
		int opt_d = (opt & 0x0f);

		m_dwQuery = (opt & 0x0f);

		if( BMON_MST ) {
			if( opt_d & ACQ_BUMON )	m_dwQuery |= ACQ_BUMON;
		}
		else {
			m_dwQuery &= ~ACQ_BUMON;
		}

		if( M_KOJI ) {
			if( opt_d & ACQ_KOJI )	m_dwQuery |= ACQ_KOJI;
		}
		else {
			m_dwQuery &= ~ACQ_KOJI;
		}

		if( BRmst )	{
			if( opt_d & ACQ_EDA )	m_dwQuery |= ACQ_EDA;
		}
		else {
			m_dwQuery &= ~ACQ_EDA;
		}

		//仕訳表示順 および、固定
		m_dwOpt = (opt >> 8);

#ifdef OLD_CLOSE
		if( BMON_MST )	m_dwQuery |= ACQ_BUMON;
		if( M_KOJI )	m_dwQuery |= ACQ_KOJI;
		if( BRmst )		m_dwQuery |= ACQ_EDA;
#endif

		SelPar par;
		par.sel_pn = SL_KAMOKU_PN;
		m_Seldata.SelectJob( &par, -1 );

		if( BMON_MST )	m_BmnInp.EnableWindow( TRUE );
		else			m_BmnInp.EnableWindow( FALSE );

		if( M_KOJI )	m_KojiInp.EnableWindow( TRUE );
		else			m_KojiInp.EnableWindow( FALSE );

//		char tmp[64];
		m_BmnInp.SetMinval( "0" );
//		sprintf_s( tmp, sizeof tmp, "%d", pDBzm->GetBumonMaxDecimal() );
//		m_BmnInp.SetMaxval( tmp );
		m_BmnInp.SetMaxval( "0" );
		m_BmnInp.SetMaxLen( pDBzm->zvol->bmcol );

		m_KojiInp.SetMaxLen( pDBzm->zvol->kjcol );
#ifdef CLOSE
		m_BrnInp.SetMinval( "0" );
		sprintf_s( tmp, sizeof tmp, "%d", pDBzm->GetEdabanMaxDecimal() );
		m_BrnInp.SetMaxval( tmp );
#endif
		m_BrnInp.SetMaxLen( pDBzm->zvol->edcol );

	//	m_BaseCode = "01010101";
		m_BaseCode = GNKINCode( Voln1->apno );
		m_BaseAtt = 0;
		m_BaseEda = -1;
		m_BaseBmn = -1;
		m_BaseKoji.Empty();

		VARIANT var;
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDTCTRL2, &var, ICSDBEDT_TYPE_STRING, 0 );
		DBdata_set( this, IDC_ICSDBEDTCTRL3, &var, ICSDBEDT_TYPE_STRING, 0 );
		DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );

		m_BmnInp.ImeEndMode( TRUE );
		m_KojiInp.ImeEndMode( TRUE );

		if( BMON_MST ) {
			m_BmnInp.EnableWindow( (m_dwQuery & ACQ_BUMON) ? TRUE : FALSE );
		}
		if( M_KOJI ) {
			m_KojiInp.EnableWindow( (m_dwQuery & ACQ_KOJI) ? TRUE : FALSE );
		}
		if( BRmst ) {
			m_BrnInp.EnableWindow( (m_dwQuery & ACQ_EDA) ? TRUE : FALSE );
		}

		m_BaseDate = -1;
		m_BaseOfst = -1;
		m_BaseKsign = 0;

		m_NOBRNCHK	 = 0;
		m_ACKOJICHK	 = 0;

		// 06.22 /10
		int nAswk = ((CMainFrame*)GetDBDinpMainFrame())->AutoSwkType();;
		if( nAswk & 0x01 )	m_bAutoSwk = TRUE;
		else				m_bAutoSwk = FALSE;
	//	m_bAutoSwk = ((CMainFrame*)GetDBDinpMainFrame())->AutoSwkType();

		m_tkjourAry.RemoveAll();
		m_AutoSwkCnt = m_AutoSwkTrn = 0;

		GetDlgItem(IDC_STATIC5)->SetWindowText("");

		m_selDate.SetMode( ICS_DATESELECT_MODE_ONESELF );
		m_selDate.Iniz( pDBzm, ICS_DATESELECT_RANGE_PERIOD|ICS_DATESELECT_EDIT_OFF|ICS_DATESELECT_INPOK_ONLY, 0, 0 );
//		int sofs, eofs, sdate, edate;
//		m_selDate.GetMonthOffset( sofs, eofs, sdate, edate );

		// 初期科目名セット
		m_KamokInp.ImeEndMode( TRUE );
		m_KamokInp.SetKanaTerm( TRUE, KanaSearchKeta(), ICSDBEDT_KTYPE_KANA );
		struct _KamokuData check = {0};
		struct _DBKNREC* pKn;

		strncpy_s( check.kd_code, sizeof check.kd_code, m_BaseCode, m_BaseCode.GetLength() );
		check.kd_eda = m_BaseEda;
		pKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
		if( pKn ) {
			KamokuString( pKn, check.kd_name, sizeof check.kd_name );
			m_BaseAtt = pKn->knatt;
		}

		DBkamokudata_set( this, IDC_ICSDBEDTCTRL4, &check, pDBzm );
		m_BrnInp.ImeEndMode(TRUE);

		struct _SET_KMKETC sk;

		m_BmnSK.bmn = -1;
		m_BmnSK.kno.Empty();
		m_BmnSK.code.Empty();
		m_BmnSK.brn = -1;
		m_BmnSK.etc.Empty();
		m_BmnSK.etc_col = (COLORREF)-1;

		m_KojiSK.bmn = -1;
		m_KojiSK.kno.Empty();
		m_KojiSK.code.Empty();
		m_KojiSK.brn = -1;
		m_KojiSK.etc.Empty();
		m_KojiSK.etc_col = (COLORREF)-1;

		SetBmnEtcDisp( FALSE );
		SetKojiEtcDisp( FALSE );

		//兼業のための基本科目チェック
		BaseKmkIndCheck();

		m_Input.ShowWindow( SW_HIDE );

		BaseBmnCtrl();

		if( m_ListCnt == -1 ) {
		//	m_ListCnt = m_List.GetDispDataCount();
			m_ListCnt = GetACListCount();
		}
		InitNMbeginLabel();

//MyTrace( "@CDBAbookView::BeginApp --> m_ListCnt = %d\n", m_ListCnt );

	}
	catch( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;	
	}

	//月指定の初期化
	CMnthDlg::m_TanOfst = -1;

	ListClmSet();

//	PostMessage( WM_COMMAND, MAKELONG(ID_BUTTON_F11, 0) );

}


//--------------------------------------------
//	部門名称の表示
//--------------------------------------------
void CDBAbookView::SetBmnEtcDisp( BOOL bFocus )
{
	if( pDBzm == NULL )	return;

	BOOL bDisp;
	bDisp = ((pAUTOSEL->BMNDSP_OPT == OPT_BMNDSP) ? TRUE : FALSE);

	if( bDisp )
		_dsp_etcwnd( &m_BmnSK, this, &m_StBmn, IDC_ICSDBEDTCTRL2, bFocus );
	else
		m_StBmn.ShowWindow(SW_HIDE);
}

//--------------------------------------------
//	工事名称の表示
//--------------------------------------------
void CDBAbookView::SetKojiEtcDisp( BOOL bFocus )
{
	if( pDBzm == NULL )	return;

	BOOL bDisp;
	bDisp = ((pAUTOSEL->BMNDSP_OPT == OPT_BMNDSP) ? TRUE : FALSE);

	if( bDisp )
		_dsp_etcwnd( &m_KojiSK, this, &m_StKoji, IDC_ICSDBEDTCTRL3, bFocus );
	else
		m_StKoji.ShowWindow(SW_HIDE);
}

// 基本項目 のウインドウ 移動
void CDBAbookView::BaseWndMove()
{
	CRect rect, rc;
	int wd, left;
	int kjsgn = 0;

	if( pDBzm == NULL )
		return;

	if( pDBzm->zvol->apno == 0x20 ) {
		if( (pDBzm->zvol->sub_sw&0x04) ) kjsgn = 1;
	}

	// 部門なし
	if( !(pDBzm->zvol->sub_sw&0x02) ) {
		GetDlgItem( IDC_STATIC1 )->ShowWindow(SW_HIDE);
		m_BmnInp.ShowWindow(SW_HIDE);
	}
	else {
		GetDlgItem( IDC_STATIC1 )->ShowWindow(SW_SHOW);
		m_BmnInp.ShowWindow(SW_SHOW);
	}
	// 工事なし
	if( ! kjsgn ) {
		GetDlgItem( IDC_STATIC2 )->ShowWindow(SW_HIDE);
		m_KojiInp.ShowWindow(SW_HIDE);
	}
	else {
		GetDlgItem( IDC_STATIC2 )->ShowWindow(SW_SHOW);
		m_KojiInp.ShowWindow(SW_SHOW);
	}

	if( ! BRmst ) {
		GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
		m_BrnInp.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(SW_HIDE);
	}
	else {
		GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
		m_BrnInp.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC5)->ShowWindow(SW_SHOW);
	}

	// 開始位置
	m_selfix8.GetWindowRect( rect );
	ScreenToClient( rect );
	left = rect.left;
	CWnd* pwnd;

	for( int i = 0; ; i++ ) {
		switch(i) {
		case 0:
			if( !(pDBzm->zvol->sub_sw&0x02) )	continue;
			else	pwnd = GetDlgItem(IDC_STATIC1);
			break;
		case 1:
			if( !(pDBzm->zvol->sub_sw&0x02) )	continue;
			else	pwnd = (CWnd*)&m_BmnInp;
			break;
		case 2:
			if( ! kjsgn )	continue;
			else	pwnd = GetDlgItem(IDC_STATIC2);
			break;
		case 3:
			if( ! kjsgn )	continue;
			else	pwnd = (CWnd*)&m_KojiInp;
			break;
		case 4:		pwnd = GetDlgItem(IDC_STATIC3);	break;
		case 5:		pwnd = &m_KamokInp;	break;

		case 6:	
			if( !BRmst )	continue;
			pwnd = GetDlgItem(IDC_STATIC4);	break;
		case 7:		
			if( !BRmst )	continue;
			pwnd = &m_BrnInp;	break;
		case 8:		
			if( !BRmst )	continue;
			pwnd = GetDlgItem(IDC_STATIC5);	break;

		default:
			pwnd = NULL;
			break;
		}

		if( ! pwnd )	break;

		pwnd->GetWindowRect( rc );
		ScreenToClient( rc );
		wd = rc.Width();

		rc.left = left;
		rc.right = rc.left + wd;

		// 建設のときは、枝番名称のｳｨﾝﾄﾞｳを小さく
		if( i == 8 ) {
			CRect btn;
			GetDlgItem(IDC_BUTTON_DISP)->GetWindowRect( btn );
			ScreenToClient( btn );

			if( rc.right > btn.left )	rc.right = btn.left;
		}

		pwnd->MoveWindow( rc );
		pwnd->Invalidate();
		pwnd->UpdateWindow();

		left = rc.right;
	}
}


int CDBAbookView::GetClmSize( int clmid)
{
	map<int,int>::iterator	it;
	int ret = 0;

	it = m_clmMap.find( clmid );
	if( it != m_clmMap.end() ) {
		ret = it->second;
	}
	else {
		ret = 2;
	}

	return ret;
}


// リストの設定
void CDBAbookView::ListClmSet( BOOL bOnSize/*=FALSE*/)
{
	DT_COLUMN	data[16];

	int total, width;
	int max, tksz, tkinp, idx;

	BOOL bBmon, bKoji, bHjiyu, bZei, bDenpType, bSyz;
	bBmon = bKoji = bHjiyu = bZei = bDenpType = bSyz = FALSE;

	if( BMON_MST ) {
		if( pAUTOSEL->DSPBMON_OPT == OPT_NONOPT)
			bBmon = TRUE;
	}
	if( M_KOJI ) {
		if( pAUTOSEL->DSPKOJI_OPT == OPT_NONOPT)
			bKoji = TRUE;
	}

	if( DenpTypeMode() )	bDenpType = TRUE;

	if( IsHjiyuItemGyosyu() != 0 ) {
//	if( ! IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO) /* != 0x52*/ ) {
		if( pAUTOSEL->DSPHJIYU_OPT == OPT_NONOPT)
			bHjiyu = TRUE;
	}

	if( pAUTOSEL->DSPZEI_OPT == OPT_NONOPT)
		bZei = TRUE;

	if (pAUTOSEL->DSPSYZ_OPT == OPT_NONOPT)
		bSyz = TRUE;
	
	int denp = 0;
	if( pAUTOSEL->DSPDENP_OPT != OPT_NODSP ) {
		denp = 7;
	}

	int docevi = 0;
	if( bDOCEVI_Master ) {
		docevi = Voln1->snocol;
	}

	int komok_num = 7;
	
	if( bBmon )		komok_num++;
	if( bKoji )		komok_num++;
	if( bHjiyu )	komok_num++;
	if( bZei )		komok_num++;
	if( denp )		komok_num++;
	if( docevi )	komok_num++;

	if( bDenpType )	komok_num++;
	if( bSyz )		komok_num++;

	//工事幅
	int scrX = GetSystemMetrics(SM_CXFULLSCREEN);
	int kjwd = 0;
	int val_adj = 0;

	CFont* pFont, *pOld;
	pFont = m_List.GetFont();

	// 摘要幅を調整する
	CClientDC dc(&m_List);
	CSize sz, *scrn;
	pOld = dc.SelectObject( pFont );

	sz = dc.GetTextExtent( "A", 1 );

	dc.SelectObject( pOld );

	scrn = GetScreenSize();

	CRect rect;
	m_List.GetClientRect( rect );
	width = rect.Width();
	// (項目個数*調整ピクセル数)
	max = (width-(komok_num*4)) / sz.cx;

	m_clmMap.clear();

	m_clmMap.insert( make_pair(DATA_SEQ, 7) );
	if( bDenpType ) {
		m_clmMap.insert( make_pair(DATA_USER3, 2) );
		val_adj++;
	}

	m_clmMap.insert( make_pair(DATA_DATE, 6) );
	int nTmp;
	nTmp = 14 + Voln1->edcol;
	m_clmMap.insert( make_pair(DATA_DEBT, nTmp) );


	if( denp ) {
		m_clmMap.insert( make_pair(DATA_DENP, denp) );
	}
	if( docevi ) {
		m_clmMap.insert( make_pair(DATA_DOCEVI, docevi) );
		val_adj++;
	}

	if( bBmon) {
		m_clmMap.insert( make_pair(DATA_DEB_BMON, 4/*Voln1->bmcol*/) );
	}
	if( bKoji ) {
		//工事幅保存分 調整する
		int kjcnt = 4;
		//if( pAUTOSEL->RESOL_OPT == scrX ) {
		//	kjwd = ((CMainFrame*)GetDBDinpMainFrame())->AcbkKojiWd();
		//}
		//if( kjwd > 0 ) {
		//	kjcnt = (kjwd / sz.cx);
		//}
		m_clmMap.insert( make_pair(DATA_DEB_KNO, kjcnt/*Voln1->kjcol*/) );
		val_adj++;
	}

	if( bHjiyu ) {
		m_clmMap.insert( make_pair(DATA_HJIYU, 2) );
	}
	if( bZei ) {
		m_clmMap.insert( make_pair(DATA_ZEI, 13-val_adj) );
	}

	m_clmMap.insert( make_pair(DATA_VAL, 15-val_adj) );
	m_clmMap.insert( make_pair(DATA_USER1, 15-val_adj) );
	m_clmMap.insert( make_pair(DATA_USER2, 15-val_adj) );

	if (bSyz) {
		if (CheckWidthSyohi())  nTmp = 8 + 2;
		else					nTmp = 8;
		m_clmMap.insert(make_pair(DATA_SYZ, nTmp));
	}

	map<int,int>::iterator it;
	total = 0;
	for( it = m_clmMap.begin(); it != m_clmMap.end(); it++ ) {
		total += it->second;
	}

	//摘要は、それ以外の項目の残り分
	tksz = max - total;
	if( tksz <= 0 )
		tksz = 2;
	m_clmMap.insert( make_pair(DATA_TEKIYO, tksz) );

	idx = 0;

	data[idx].ln_ofs = 0;
	data[idx].clm_title = "";
	data[idx].clm_id = DATA_SEQ;
	data[idx].clm_type = 0;
	data[idx].clm_back = -1;
	data[idx].clm_text = -1;
	data[idx].clm_inpsz = 0;
	data[idx].clm_size = GetClmSize(data[idx].clm_id);
	data[idx].clm_szadj = 0;
	data[idx].clm_spos = 0;
	data[idx].clm_adj = 0;
	data[idx].clm_sidx = 0;
	idx++;

	if( bDenpType ) {
		data[idx].ln_ofs = 0;
		data[idx].clm_title = "伝";
		data[idx].clm_id	= DATA_USER3;
		data[idx].clm_type	= DT_KIND_STRING|DT_FMT_CENTER;
		data[idx].clm_back = -1;
		data[idx].clm_text = -1;
		data[idx].clm_inpsz = 3;
		data[idx].clm_size = GetClmSize(data[idx].clm_id);
		data[idx].clm_szadj = 0;
		data[idx].clm_spos = -1;
		data[idx].clm_adj = 0;
		data[idx].clm_sidx = -1;
		idx++;
	}

	data[idx].ln_ofs = 0;
	data[idx].clm_title = "";
	data[idx].clm_id = DATA_DATE;
	data[idx].clm_type = 0;
	data[idx].clm_back = -1;
	data[idx].clm_text = -1;
	data[idx].clm_inpsz = 0;
	data[idx].clm_size = GetClmSize(data[idx].clm_id);
	data[idx].clm_szadj = 0;
	data[idx].clm_spos = -1;
	data[idx].clm_adj = 0;
	data[idx].clm_sidx = -1;
	idx++;

	if( denp ) {
		data[idx].ln_ofs = 0;
		data[idx].clm_title = "";
		data[idx].clm_id = DATA_DENP;
		data[idx].clm_type = 0;
		data[idx].clm_back = -1;
		data[idx].clm_text = -1;
		data[idx].clm_inpsz = denp;
		data[idx].clm_size = GetClmSize(data[idx].clm_id);
		data[idx].clm_szadj = 0;
		data[idx].clm_spos = -1;
		data[idx].clm_adj = 0;
		data[idx].clm_sidx = -1;
		idx++;
	}
	if( bDOCEVI_Master ) {
		data[idx].ln_ofs = 0;
		data[idx].clm_title = "";
		data[idx].clm_id = DATA_DOCEVI;
		data[idx].clm_type = 0;
		data[idx].clm_back = -1;
		data[idx].clm_text = -1;
		data[idx].clm_inpsz = Voln1->snocol;
		data[idx].clm_size = GetClmSize(data[idx].clm_id);
		data[idx].clm_szadj = 0;
		data[idx].clm_spos = -1;
		data[idx].clm_adj = 0;
		data[idx].clm_sidx = -1;
		idx++;
	}

	if( bBmon ) {
		data[idx].ln_ofs = 0;
		data[idx].clm_title = "部門";
		data[idx].clm_id = DATA_DEB_BMON;
		data[idx].clm_type = DT_FMT_LEFT;
		data[idx].clm_back = -1;
		data[idx].clm_text = -1;
		data[idx].clm_inpsz = Voln1->bmcol;
		data[idx].clm_size = GetClmSize(data[idx].clm_id);
		data[idx].clm_szadj = 0;
		data[idx].clm_spos = -1;
		data[idx].clm_adj = 0;
		data[idx].clm_sidx = -1;
		idx++;
	}
	if( bKoji ) {
		data[idx].ln_ofs = 0;
		data[idx].clm_title = "工事";
		data[idx].clm_id = DATA_DEB_KNO;
		data[idx].clm_type = DT_FMT_LEFT;
		data[idx].clm_back = -1;
		data[idx].clm_text = -1;
		data[idx].clm_inpsz = Voln1->kjcol;
		data[idx].clm_size = GetClmSize(data[idx].clm_id);
		data[idx].clm_szadj = 0;
		data[idx].clm_spos = -1;
		data[idx].clm_adj = 0;
		data[idx].clm_sidx = -1;
		idx++;
	}

	data[idx].ln_ofs = 0;
	data[idx].clm_title = "相 手 科 目";
	data[idx].clm_id = DATA_DEBT;
	data[idx].clm_type = 0;
	data[idx].clm_back = -1;
	data[idx].clm_text = -1;
	data[idx].clm_inpsz = 0;
	data[idx].clm_size = GetClmSize(data[idx].clm_id);
	data[idx].clm_szadj = 0;
	data[idx].clm_spos = -1;
	data[idx].clm_adj = 0;
	data[idx].clm_sidx = -1;
	idx++;

	// 摘要
	if( (pDBzm->zvol->tk_ln * 2) > tksz )
			tkinp = (pDBzm->zvol->tk_ln * 2);
	else	tkinp = tksz;
	data[idx].ln_ofs = 0;
	data[idx].clm_title = "";
	data[idx].clm_id = DATA_TEKIYO;
	data[idx].clm_type = DT_INP_BOTTOMLEFT;
	data[idx].clm_back = -1;
	data[idx].clm_text = -1;
	data[idx].clm_inpsz	= tkinp;
	data[idx].clm_size	= GetClmSize(data[idx].clm_id);
	data[idx].clm_szadj 	= 0;
	data[idx].clm_spos 	= -1;
	data[idx].clm_adj 	= 0;
	data[idx].clm_sidx	= -1;
	idx++;

	// 変動事由
	if( bHjiyu ) {
		data[idx].ln_ofs = 0;
		data[idx].clm_title = pDBzm->get_hjiyutitle( 1 );
		data[idx].clm_id = DATA_HJIYU;
		data[idx].clm_type = 0;
		data[idx].clm_back = -1;
		data[idx].clm_text = -1;
		data[idx].clm_inpsz	= 2;
		data[idx].clm_size	= GetClmSize(data[idx].clm_id);
		data[idx].clm_szadj = 0;
		data[idx].clm_spos 	= -1;
		data[idx].clm_adj 	= 0;
		data[idx].clm_sidx	= -1;
		idx++;
	}

	data[idx].ln_ofs = 0;
	data[idx].clm_title = "入　金";
	data[idx].clm_id = DATA_VAL;
	data[idx].clm_type = 0;
	data[idx].clm_back = -1;
	data[idx].clm_text = -1;
	data[idx].clm_inpsz = 0;
	data[idx].clm_size = GetClmSize(data[idx].clm_id);
	data[idx].clm_szadj = 0;
	data[idx].clm_spos = -1;
	data[idx].clm_adj = 0;
	data[idx].clm_sidx = -1;
	idx++;

	data[idx].ln_ofs = 0;
	data[idx].clm_title = "出　金";
	data[idx].clm_id = DATA_USER1;
	data[idx].clm_type = DT_KIND_VAL | DT_FMT_RIGHT;
	data[idx].clm_fmt = "sss,sss,sss,ss9";
	data[idx].clm_back = -1;
	data[idx].clm_text = -1;
	data[idx].clm_inpsz = 0;
	data[idx].clm_size = GetClmSize(data[idx].clm_id);
	data[idx].clm_szadj = 0;
	data[idx].clm_spos = -1;
	data[idx].clm_adj = 0;
	data[idx].clm_sidx = -1;
	idx++;

	if( bZei ) {
		data[idx].ln_ofs = 0;
		data[idx].clm_title = "税　額";
		data[idx].clm_id = DATA_ZEI;
		data[idx].clm_type = 0;
		data[idx].clm_back = -1;
		data[idx].clm_text = -1;
		data[idx].clm_inpsz = 0;
		data[idx].clm_size = GetClmSize(data[idx].clm_id);
		data[idx].clm_szadj = 0;
		data[idx].clm_spos = -1;
		data[idx].clm_adj = 0;
		data[idx].clm_sidx = -1;
		idx++;
	}

	if (bSyz) {
		data[idx].ln_ofs = 0;
		data[idx].clm_title = "";
		data[idx].clm_id = DATA_SYZ;
		data[idx].clm_type = 0;
		data[idx].clm_back = -1;
		data[idx].clm_text = -1;
		data[idx].clm_inpsz = 0;
		data[idx].clm_size = GetClmSize(data[idx].clm_id);
		data[idx].clm_szadj = 0;
		data[idx].clm_spos = -1;
		data[idx].clm_adj = 0;
		data[idx].clm_sidx = -1;
		idx++;
	}

	data[idx].ln_ofs = 0;
	data[idx].clm_title = "差引残高";
	data[idx].clm_id = DATA_USER2;
	data[idx].clm_type = DT_KIND_VAL | DT_FMT_RIGHT;
	data[idx].clm_fmt = "sss,sss,sss,ss9";
	data[idx].clm_back = -1;
	data[idx].clm_text = -1;
	data[idx].clm_inpsz = 0;
	data[idx].clm_size = GetClmSize(data[idx].clm_id);
	data[idx].clm_szadj = 0;
	data[idx].clm_spos = -1;
	data[idx].clm_adj = 0;
	data[idx].clm_sidx = -1;
	idx++;

	m_List.SetItemColumn( &data[0], idx );

	if( bOnSize ) {
		for( int n = 0; n < idx; n++ ) {
			m_List.SetListItemWidth( data[n].clm_id, 0 );
		}
	}

	m_List.SetFocusedSign( TRUE );
	m_List.EnforceLayout();

	int itm_wd;
	int save_wd_use = 1;

	DT_RECT* pdt;
	pdt = m_List.GetDtRect();

	int clq_wd = 0;
	int list_wd = pdt->LineRect.Width();

	for( int i = 0; i < pdt->Dtcnt; i++ ) {
		itm_wd = pdt->DtRect[i].Width();

		switch( pdt->DtID[i] ) {
		case DATA_SEQ:
			if( pAUTOSEL->AWSEQ_OPT != 0 ) {
				clq_wd += pAUTOSEL->AWSEQ_OPT;
			}
			else {
				clq_wd += itm_wd;
			}
			break;
		case DATA_DATE:
			if( pAUTOSEL->AWDATE_OPT != 0 ) {
				clq_wd += pAUTOSEL->AWDATE_OPT;
			}
			else {
				clq_wd += itm_wd;
			}
			break;

		case DATA_DENP:
			if( pAUTOSEL->AWDENP_OPT != 0 ) {
				clq_wd += pAUTOSEL->AWDENP_OPT;
			}
			else {
				clq_wd += itm_wd;
			}
			break;

		case DATA_DEB_BMON:
			if( pAUTOSEL->AWBMON_OPT != 0 ) {
				clq_wd += pAUTOSEL->AWBMON_OPT;
			}
			else {
				clq_wd += itm_wd;
			}
			break;

		case DATA_DEB_KNO:
			if( pAUTOSEL->AWKOJI_OPT != 0 ) {
				clq_wd += pAUTOSEL->AWKOJI_OPT;
			}
			else {
				clq_wd += itm_wd;
			}
			break;

		case DATA_DEBT:
			if( pAUTOSEL->AWKMK_OPT != 0 ) {
				clq_wd += pAUTOSEL->AWKMK_OPT;
			}
			else {
				clq_wd += itm_wd;
			}
			break;
		case DATA_VAL:
			if( pAUTOSEL->AWNKN_OPT != 0 ) {
				clq_wd += pAUTOSEL->AWNKN_OPT;
			}
			else {
				clq_wd += itm_wd;
			}
			break;
		case DATA_USER1:
			if( pAUTOSEL->AWSKN_OPT != 0 ) {
				clq_wd += pAUTOSEL->AWSKN_OPT;
			}
			else {
				clq_wd += itm_wd;
			}
			break;
		case DATA_SYZ:
			if( pAUTOSEL->AWSYZ_OPT != 0 ) {
				clq_wd += pAUTOSEL->AWSYZ_OPT;
			}
			else {
				clq_wd += itm_wd;
			}
			break;
		case DATA_TEKIYO:
			if( pAUTOSEL->AWTKY_OPT != 0 ) {
				clq_wd += pAUTOSEL->AWTKY_OPT;
			}
			else {
				clq_wd += itm_wd;
			}
			break;
		case DATA_DOCEVI:
			if( pAUTOSEL->AWSYON_OPT != 0 ) {
				clq_wd += pAUTOSEL->AWSYON_OPT;
			}
			else {
				clq_wd += itm_wd;
			}
			break;

		case DATA_USER2:
			break;
		default:
			clq_wd += itm_wd;
			break;
		}
//MyTrace("ID[%d], clq_wd = %d\n", pdt->DtID[i], clq_wd);

	}
	if( list_wd < clq_wd /*|| pAUTOSEL->RESOL_OPT != scrX*/ ) {
		save_wd_use = 0;
	}
//
//MyTrace("----> pAUTOSEL->RESOL_OPT = %d, scrX = %d\n", pAUTOSEL->RESOL_OPT, scrX);
//MyTrace("----> list_wd = %d, clq_wd = %d\n", list_wd, clq_wd);

	if( save_wd_use ) {
		m_List.SetListItemWidth(DATA_SEQ, pAUTOSEL->AWSEQ_OPT);
		m_List.SetListItemWidth(DATA_DATE, pAUTOSEL->AWDATE_OPT);
		m_List.SetListItemWidth(DATA_DENP, pAUTOSEL->AWDENP_OPT);
		m_List.SetListItemWidth(DATA_DEB_BMON, pAUTOSEL->AWBMON_OPT);
		m_List.SetListItemWidth(DATA_DEB_KNO, pAUTOSEL->AWKOJI_OPT);
		m_List.SetListItemWidth(DATA_DEBT, pAUTOSEL->AWKMK_OPT);
		m_List.SetListItemWidth(DATA_VAL, pAUTOSEL->AWNKN_OPT);
		m_List.SetListItemWidth(DATA_USER1, pAUTOSEL->AWSKN_OPT);
		m_List.SetListItemWidth(DATA_ZEI, pAUTOSEL->AWZEI_OPT);
		m_List.SetListItemWidth(DATA_SYZ, pAUTOSEL->AWSYZ_OPT);
		m_List.SetListItemWidth(DATA_TEKIYO, pAUTOSEL->AWTKY_OPT);
		m_List.SetListItemWidth(DATA_HJIYU, pAUTOSEL->AWHJYU_OPT);
		m_List.SetListItemWidth(DATA_DOCEVI, pAUTOSEL->AWSYON_OPT);
		m_List.EnforceLayout();
	}

	DT_ITEMMAX im;
	im.im_seq = 8;
	im.im_denp = 7;
	im.im_eda = Voln1->edcol;
	im.im_bmn = Voln1->bmcol;
	im.im_koji= Voln1->kjcol;
	im.im_tky = (Voln1->tk_ln * 2);
	im.im_sno= Voln1->snocol;

	m_List.SetImax( &im );
	m_List.SetConfirmMode( Confirm_Mode() );
	m_List.SetDispDataLineMax(ACREC_MAX);

	m_List.RedrawDataListCtrl();

}


BOOL CDBAbookView::EndApp( WPARAM wParam, BOOL bCosel )
{
	//通常の終了処理を記述して下さい。
	//終了を許可する場合はTRUE、許可しない場合はFALSEを返します。
	if( (wParam == 1) ) {
		//ICSExit()でエラー終了した場合はこちらに記述します。
//				mst_close();
	}
	else {
		//それ以外はこちらに記述します。
		int	st = IDYES;

		if( ! bCosel ) {
			if( ((CMainFrame*)GetDBDinpMainFrame())->IsCloudEndMsg() ) {
				//クラウド共有マスターでかつ事務所(仕訳確定への移行メッセージを後で出す)
			}
			else {
				st	=	ICSJobEndBox( _T("仕訳入力を終了してもよろしいですか？") );
			}
		}

		if( st != IDYES ) {
			AutoSelIndicator( acINP_mode );
			return FALSE;
		}

		// 追加仕訳のチェック
		for( int i = 0; i < ACREC_MAX; i++ ) {
			if( m_ALine[i].abs_pos == 0 ) {
				if( AppendDataCheck( (i+1) ) ) {
					PROGRAM_OK = FALSE;

					int st = myICSMessageBox( swk_kansei, MB_YESNO|MB_DEFBUTTON2 );

					if( st == IDYES ) {
						PROGRAM_OK = TRUE;

						_set_focus( i+1, AC_DATE_PN );
						return FALSE;
					}
				}
				break;
			}
		}

		int ret = UpdateModfyData();
		if( ret == -2 ) {
			myICSMessageBox( (LPCTSTR)pDBzm->GetZmErrMsg() );
			// 現在行の書き込み
			int nowln = get_nowln();
			DataLineUpdate( &m_CREC, nowln );
		}
		else if( ret != 0 ) {
			PROGRAM_OK = TRUE;
			return FALSE;
		}
		GetAcbookItemWidth(1);

		if( ((CMainFrame*)GetDBDinpMainFrame())->IsCloudEndMsg() ) {
			if( ! ((CMainFrame*)GetDBDinpMainFrame())->CloudEndChainJob() ) {
				st	=	ICSJobEndBox( _T("仕訳入力を終了してもよろしいですか？") );
				if( st != IDYES ) {
					PROGRAM_OK = TRUE;
					set_focus( AC_DATE_PN );
					return FALSE;
				}
			}
		}

		Destroy_ModeLessDlg();
	}

	return	TRUE;
}


void CDBAbookView::OnActivateJob(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	if( nState == WA_INACTIVE ) {
		GetAcbookItemWidth();
	}
}


//項目幅を取得
void CDBAbookView::GetAcbookItemWidth(int saveAlways/*=0*/ )
{
	if( !saveAlways ) {
		if( PROGRAM_OK == FALSE )
			return;
	}
	if( pAUTOSEL == NULL )	return;

	if( !::IsWindow(m_List.m_hWnd) )
		return;

	int scrX = GetSystemMetrics(SM_CXFULLSCREEN);
	if( pAUTOSEL->RESOL_OPT == 0 ) {
		pAUTOSEL->RESOL_OPT = scrX;
	}
	pAUTOSEL->AWSEQ_OPT = m_List.GetListItemWidth(DATA_SEQ);
	pAUTOSEL->AWDATE_OPT = m_List.GetListItemWidth(DATA_DATE);
	pAUTOSEL->AWDENP_OPT = m_List.GetListItemWidth(DATA_DENP);
	pAUTOSEL->AWBMON_OPT = m_List.GetListItemWidth(DATA_DEB_BMON);
	pAUTOSEL->AWKOJI_OPT = m_List.GetListItemWidth(DATA_DEB_KNO);
	pAUTOSEL->AWKMK_OPT = m_List.GetListItemWidth(DATA_DEBT);
	pAUTOSEL->AWNKN_OPT = m_List.GetListItemWidth(DATA_VAL);
	pAUTOSEL->AWSKN_OPT = m_List.GetListItemWidth(DATA_USER1);
	pAUTOSEL->AWZEI_OPT = m_List.GetListItemWidth(DATA_ZEI);
	pAUTOSEL->AWSYZ_OPT = m_List.GetListItemWidth(DATA_SYZ);
	pAUTOSEL->AWTKY_OPT = m_List.GetListItemWidth(DATA_TEKIYO);
	pAUTOSEL->AWHJYU_OPT = m_List.GetListItemWidth(DATA_HJIYU);
	pAUTOSEL->AWSYON_OPT = m_List.GetListItemWidth(DATA_DOCEVI);
}

BOOL	CDBAbookView::EndChk()
{

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CDBAbookViewクラスの印刷

BOOL CDBAbookView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの印刷準備
	return DoPreparePrinting(pInfo);
}

void CDBAbookView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CDBAbookView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: OnPrepareDCこの位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	CDBbaseView::OnPrepareDC(pDC, pInfo);
}

void CDBAbookView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

void CDBAbookView::OnPrint(CDC* pDC, CPrintInfo*)
{
	// TODO: コントロールを印刷するコードを追加してください。
}

/////////////////////////////////////////////////////////////////////////////
// CDBAbookViewクラスの診断

#ifdef _DEBUG
void CDBAbookView::AssertValid() const
{
	CDBbaseView::AssertValid();
}

void CDBAbookView::Dump(CDumpContext& dc) const
{
	CDBbaseView::Dump(dc);
}
/*
CDBAbookDoc* CDBAbookView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDBAbookDoc)));
	return (CDBAbookDoc*)m_pDocument;
}
*/

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDBAbookViewクラスのメッセージ ハンドラ

void CDBAbookView::OnInitialUpdate() 
{
	CDBbaseView::SelectDispAdjust();

	int rsl = ResolutionRatio();
	if( rsl == RSL_LONGERHANWIDE /*psz->cy >= 1024*/ ) {
		CRect rect;
		GetDlgItem( IDC_ACBOOKLIST )->GetWindowRect( rect );
		ScreenToClient( &rect );
		rect.bottom -= 5;
		GetDlgItem( IDC_ACBOOKLIST )->MoveWindow( rect.left, rect.top, rect.Width(), rect.Height() );
	}
	CDBbaseView::OnInitialUpdate();

	//フォームの大きさにウィンドウの大きさを合わせます。
	//この機能が不要な場合は、以下の二行を削除するか
	//ResizeParentToFit()のパラメータをTRUEにして下さい。
//	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

//	((CMainFrame*)GetParentFrame())->MinMaxFlg()	=	TRUE;
	// TODO: OnInitialUpdate() 固有の処理を追加するか、または基本クラスを呼び出してください

	// 縦の長さ変更
	SetResize( IDC_ACBOOKLIST, ICS_RESIZE_BOTTOM );

	int i;
	int clipID[] = {
		IDC_BUTTON_DISP, IDC_STATIC5, IDC_MNTHSEL, 
		IDC_STATIC1, IDC_STATIC2, IDC_STATIC3, IDC_STATIC4, IDC_STATIC5, 
		IDC_ICSDBEDTCTRL2, IDC_ICSDBEDTCTRL3, IDC_ICSDBEDTCTRL4, IDC_ICSDBEDTCTRL5, IDC_ICSDISPRDREMAIN
	};
	// 兄弟ウィンドウをクリップする(ウィンドウを重ねる場合)
	for( i = 0; i < (sizeof clipID/ sizeof clipID[0]); i++ ) {
		GetDlgItem( clipID[i] )->ModifyStyle(0, WS_CLIPSIBLINGS);
	}

	m_Input.EnableDelete(0);
	m_Input.EnableHome(1);
	m_BmnInp.EnableDelete(0);
	m_KojiInp.EnableDelete(0);
	m_KamokInp.EnableDelete(0);
	m_BrnInp.EnableDelete(0);

	m_BmnInp.SetFrame( -1, RGB_DARK_GRAY );
	m_KojiInp.SetFrame( -1, RGB_DARK_GRAY );
	m_KamokInp.SetFrame( -1, RGB_DARK_GRAY );
	m_BrnInp.SetFrame( -1, RGB_DARK_GRAY );

	m_List.SetInputHwnd(0, m_Input.m_hWnd );
	m_List.SetDoubleBuffering(FALSE);
//	m_List.SetDoubleBuffering(TRUE);

	// 部門・工事名称表示
	m_StBmn.DefaultColor(FALSE);
	m_StKoji.DefaultColor(FALSE);

	MakeRemainDlg();
}


void CDBAbookView::MakeRemainDlg()
{
	// マルチウィンドウの形式を切り替えた際、残表示ダイアログが変な位置に移動したので、
	// WS_POPUP -> WS_CHILDに変更
	if( m_pRDremainDlg == NULL ) {
		m_pRDremainDlg = new CRdremainDlg(this);
		if( ! m_pRDremainDlg ) {
			ermset( ERROR_MEMORY, "サブ入力オブジェクトが作成できません。");
			return;
		}
	}
}

void CDBAbookView::RemainDlgDelete()
{
	if ( m_pRDremainDlg ) {
		delete m_pRDremainDlg;
		m_pRDremainDlg = NULL;
	}
}


void CDBAbookView::OnDraw(CDC* pDC) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	//注意！この関数は変更してはいけません。呼出し位置は必ずOnDraw()の最終行です。
//	BeginApp();
}

void CDBAbookView::OnButtonEnd()
{
	CalcWindowDelete();

	//この関数は、キーボードからEndキーを押した時と
	//ツールバーのEndボタンを押した時に呼び出されます。
	if( acINP_mode == _APPEND && m_InsLine > 0 ) {
		BackNormalAppendMode( get_nowln() );
		return;
	}
	CString strType;
	if( RemainSwkdataCheck(strType) ) {
		int	st;
		CString msg;
		msg.Format("%s。\n\n処理を終わりますか？", strType);
		st	=	myICSMessageBox( msg, MB_YESNO|MB_DEFBUTTON2 );

		if( st == IDYES ) {
			EndRemaindata();	// 定型仕訳テーブルの削除
		}
		_set_focus( get_nowln(), get_nowpn() );
		return;
	}
	else {
		if( m_RDTATBL.RD_trn > 0 || (m_bAutoSwk && m_AutoSwkTrn > 0)) {
			EndRemaindata();
		}
	}

	((CMainFrame*)GetDBDinpMainFrame())->PostMessage( WM_CLOSE );
}

#ifdef CLOSE
void CDBAbookView::OnButtonF5()
{
	//この関数は、キーボードからF5キーを押した時と
	//ツールバーのF5ボタンを押した時、
	//及びメニューから印刷が選択された時に呼び出されます。
	CDBbaseView::OnFilePrint();
}
#endif


void CDBAbookView::OnButtonF11() 
{
	CalcWindowDelete();

	set_focus( AC_SELKAMOK_PN );

#ifdef OLD_CLOSE
	CACSelDlg dlg( this );
	dlg.m_pZm	= pDBzm;
	dlg.m_pSy	= pDBsy;
	dlg.m_code	= m_BaseCode;
	dlg.m_koji	= m_BaseKoji;
	dlg.m_brn 	= m_BaseEda;
	dlg.m_bmn	= m_BaseBmn;
	dlg.m_date	= m_BaseDate;
	dlg.m_ofset	= m_BaseOfst;
	dlg.m_ksign	= m_BaseKsign;

	dlg.BRNNAM_Disp = (pAUTOSEL->BRTEK_OPT == OPT_DSP);
	dlg.KMCD_Disp = (pAUTOSEL->KMNO_OPT == OPT_KMNO);

	if( dlg.DoModal() == IDOK ) {
		m_BaseCode = dlg.m_code;
		m_BaseEda = dlg.m_brn;
		m_BaseBmn = dlg.m_bmn;
		m_BaseKoji = dlg.m_koji;

		m_BaseDate = dlg.m_date;
		m_BaseOfst = dlg.m_ofset;
		m_BaseKsign = dlg.m_ksign;

		DBKNREC* pKn;
		CString str, s2;
		LPSTR p;
		pKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
		if( pKn ) {
			str.Format( "%.14s", pKn->knnam );
			m_DSPkmd.SetCaption( str );

			if( m_BaseEda != -1 ) {
				p = str.GetBuffer(64);
				pDBzm->EdabanToStr( p, 64, m_BaseEda );
				str.ReleaseBuffer();

				int tno;
				if( (tno = m_BrTek.get_brtek( m_BaseBmn, m_BaseCode, m_BaseEda )) >= 0 ) {
				/*	if( tno > 0 ) {
						if( pDBzm->DB_TekiyoNoRead( &tkrec, tno ) == ERR )
							return FALSE;
						//摘要toデータ
						dc_sw = tky_select_to_drec( &m_CREC, &tkrec );
					}
					else {
						// 枝番残高本体に登録された枝番名称
						if( pDBzm->DB_EdabanZanSearch( code, brn ) != 0 )
							return FALSE;

						tkrec.Reset();
						tkrec.m_tkname = pDBzm->ezrec->ezname;
						dc_sw = 0;
					}
					*/
				}
				s2.Empty();
			}
			else {
				str.Empty();
			}

			m_DSPedad.SetCaption( str );
			m_DSPenamd.SetCaption( str );

			if( m_BaseBmn != -1 ) {
				p = str.GetBuffer(64);
				pDBzm->BumonCodeToStr( p, 64, m_BaseBmn );
				str.ReleaseBuffer();
			}
			else	str.Empty();
			m_DSPbmd.SetCaption( str );

			if( ! m_BaseKoji.IsEmpty() ) {
				p = str.GetBuffer(64);
				pDBzm->KojiCodeToStr( p, 64, m_BaseKoji );
				str.ReleaseBuffer();
			}
			else	str.Empty();

			m_DSPkjd.SetCaption( str );

			m_LASTSEQ = 0;	// 最終仕訳番号を セットするため
			int n;
			if( ( n= InitDataSet() ) < 0 )
				return;

			ListDataSet();
			_set_focus( n+1, AC_DATE_PN );

		}
		else {
			ICSExit(0, "科目名称コードが正しくありません！");
			return;
		}
	}
#endif

}


// 表示ボタン
void CDBAbookView::OnBnClickedButtonDisp()
{
	if( ! PROGRAM_OK )	return;

	_OnBnClickedButtonDisp();

#ifdef _DBG_CLOSE
	CWnd* pwnd = GetWindow(GW_CHILD);

	while( pwnd ) {
		MyTrace( "@OnBnClickedButtonDisp pwnd->m_hWnd(id=%d)	= %08x\n", pwnd->GetDlgCtrlID(), pwnd->m_hWnd );

		pwnd	=	pwnd->GetWindow( GW_HWNDNEXT );
	}
#endif
}

// 10.01 /07
void CDBAbookView::_OnBnClickedButtonDisp()
{
	int n;
	CWaitCursor wc;

	set_nowpn( AC_SELDISP_PN );
	InitNMbeginLabel();

	sel_shiftjob( -1, 0, FALSE);
	if( m_RDTATBL.RD_trn > 0 || m_AutoSwkTrn > 0 ) {
		EndRemaindata();
	}

	// 摘要かな検索 をしていたら、元に戻す
	if( pDBzm->tkrec_kanamode() ) {
	//	pDBzm->tkrec->Requery( "", 0 );
	//	pDBzm->tkrec_kanamode() = FALSE;
		tkrec_queryAll();
	}
	// 11.12 /12
	m_InsLine = 0;

	if( ( n= InitDataSet() ) < 0 )
		return;

	if( bCONFIRM_MASTER && n == 0 ) {
		myICSMessageBox( _T( "該当の仕訳がありません！" ) );
		PostMessage( WM_COMMAND, MAKELONG( ID_BUTTON_F11, 0 ) );
		return;
	}

	ListDataSet();

	int sofs, eofs, sy, ey;
	pDBzm->get_datelimit( sofs, eofs, sy, ey );
	if( sofs == eofs ) {
		m_bMnthFix = TRUE;
	}
	else {
		m_bMnthFix = FALSE;
	}

	if( ! bCONFIRM_MASTER )	n = n+1;

	_set_focus( n, AC_DATE_PN );

}


//	表にデータをセットする
//		CDATA クラスに 先頭より表示するデータをセットしておく
//
void CDBAbookView::ListDataSet()
{
	DTLIST_DATA data;

	m_List.RemoveAllDtListData();
	int dispcnt = GetACListCount(); /*m_List.GetDispDataCount();*/
	int dcsw, st;

	DBKNREC* pKn;
	BOOL bNyuKin;
	ACBOOK_LINE*	al;
	al = m_ALine;
	BOOL bDisp;
	bDisp = ((pAUTOSEL->BMNDSP_OPT == OPT_BMNDSP) ? TRUE : FALSE);

	for (int i = 0; i < dispcnt; i++) {
		data.DTLIST_Init();
		data.dd_seq = al[i].drec.m_seq;

		if (al[i].lattr == 1) {
			data.dd_seq = 0;
			data.dd_ddate = -1;
			data.dd_ksign = 0;

			data.dd_cno = -1;
			data.dd_dbmn = -1;
			data.dd_dbmn_etc.Empty();
			data.dd_dkno.Empty();
			data.dd_dkno_etc.Empty();
			data.dd_dbt.Empty();
			data.dd_deb_name.Empty();

			data.dd_dbr = -1;
			data.dd_deb_etc.Empty();
			data.dd_cbmn = -1;
			data.dd_cbmn_etc.Empty();
			data.dd_ckno.Empty();
			data.dd_ckno_etc.Empty();
			data.dd_cre.Empty();
			data.dd_cre_name.Empty();
			data.dd_cbr = -1;
			data.dd_cre_etc.Empty();
			::ZeroMemory(data.dd_val, sizeof data.dd_val);
			data.val_flag = 0;
			::ZeroMemory(data.dd_zei, sizeof data.dd_zei);
			data.zei_flag = 0;
			data.dd_syz_str.Empty();
			data.dd_dsign = 0;

			data.dd_snumber.Empty();
			data.dd_tdate = -1;
			data.dd_tdstr.Empty();
			data.dd_husen = -1;
			data.dd_hscomment.Empty();

			CVolDateDB vd;
			int offset;
			CString zenTitle;
			if (vd.db_vd_offset(0, Voln1->ip_ymd, &offset, pDBzm) == 0) {
				if (offset > 0) {
					zenTitle = "※ 前月より繰越 ※";
				}
			}
			//仕訳期間対応
			if( ((CMainFrame*)GetDBDinpMainFrame())->AcbkKikanSwk() ) {
				if( m_BaseOfst > offset ) {
					zenTitle = "※ 前月より繰越 ※";
				}
			}

			if (zenTitle.IsEmpty()) {
				data.dd_tek.tekiyo = "※ 前期より繰越 ※";
			}
			else {
				data.dd_tek.tekiyo = zenTitle;
			}
			data.dd_tek.width = 0;
			data.dd_tek.height = 0;
			data.dd_tek.image = NULL;

			BYTE val[6];
			memcpy(val, al[i].szan, 6);
			if ((m_BaseAtt & 0x01))	l_neg(val);
			data.dd_udtbl[1].userdata = val;
			data.dd_udtbl[1].initflag = 1;

			for (int ncol = 0; ncol < DATA_CNT; ncol++) {
				if(ncol ==DATA_TEKIYO)
					data.dd_ctbl[ncol].text = RGB_BLUE;
				else
					data.dd_ctbl[ncol].text = (DWORD)-1;
				data.dd_ctbl[ncol].back = RGB_WINE_BLUE;
			}
			m_List.SetDtListData(i, &data, FALSE);
			continue;
		}

		if( al[i].abs_pos < 0 )
			continue;

		data.dd_ddate = al[i].drec.m_ddate;
		data.dd_ksign = al[i].drec.m_ksign;
		data.dd_mofs = al[i].drec.m_mofs;

		data.dd_cno		= al[i].drec.m_cno;
		data.dd_docevi	= al[i].drec.m_skn;

		bNyuKin = al[i].lcnd[AC_NKIN_PN].INP_sg;

		dcsw = GetRecordDCsgn( &al[i].drec );

		// 基本科目が 借方か、借方／貸方 同じ場合
		if( dcsw == 0 || dcsw == -1 ) {
			pKn = pDBzm->DB_PjisToKnrec( al[i].drec.m_cre, TRUE );

			if( pKn ) {
				KamokuString( pKn, data.dd_deb_name );
			}
			else {
				data.dd_deb_name.Empty();
			}
			data.dd_dbr = al[i].drec.m_cbr;

			// 枝番名称表示
			struct _SET_KMKETC sk;
			sk.bmn	= al[i].drec.m_cbmn;
			sk.brn	= al[i].drec.m_cbr;
			sk.code	= al[i].drec.m_cre;

			if( _set_kmketc( &sk, TRUE ) ) {
				data.dd_deb_etc = sk.etc;
				data.dd_ctbl[DATA_DEBT].text = sk.etc_col;
				al[i].lcnd[AC_KMK_PN].Etcmsg = sk.etc;
				al[i].lcnd[AC_KMK_PN].ETC_txtcol = sk.etc_col;
			}
			else {
				data.dd_deb_etc.Empty();
				data.dd_ctbl[DATA_DEBT].text = (DWORD)-1;
				al[i].lcnd[AC_KMK_PN].Etcmsg.Empty();
				al[i].lcnd[AC_KMK_PN].ETC_txtcol = 0;
			}

			// 部門
			data.dd_dbmn = al[i].drec.m_cbmn;
			data.dd_cbmn = data.dd_dbmn;

			sk.bmn	= al[i].drec.m_cbmn;
			sk.brn	= -1;
			sk.code.Empty();
			sk.kno.Empty();

			if( bDisp && _set_bmnetc( &sk ) ) {
				data.dd_dbmn_etc = sk.etc;
				data.dd_ctbl[DATA_DEB_BMON].text = sk.etc_col;
				al[i].lcnd[AC_BMON_PN].Etcmsg = sk.etc;
				al[i].lcnd[AC_BMON_PN].ETC_txtcol = sk.etc_col;
			}
			else {
				data.dd_dbmn_etc.Empty();
				data.dd_ctbl[DATA_DEB_BMON].text = (DWORD)-1;
				al[i].lcnd[AC_BMON_PN].Etcmsg.Empty();
				al[i].lcnd[AC_BMON_PN].ETC_txtcol = 0;
			}

			// 工事
			data.dd_dkno = al[i].drec.m_ckno;
			data.dd_ckno = data.dd_dkno;

			sk.bmn	= -1;
			sk.brn	= -1;
			sk.code.Empty();
			sk.kno	= al[i].drec.m_ckno;

			if( bDisp && _set_kojietc( &sk ) ) {
				data.dd_dkno_etc = sk.etc;
				data.dd_ctbl[DATA_DEB_KNO].text = sk.etc_col;
				al[i].lcnd[AC_KOJI_PN].Etcmsg = sk.etc;
				al[i].lcnd[AC_KOJI_PN].ETC_txtcol = sk.etc_col;
			}
			else {
				data.dd_dkno_etc.Empty();
				data.dd_ctbl[DATA_DEB_KNO].text = (DWORD)-1;
				al[i].lcnd[AC_KOJI_PN].Etcmsg.Empty();
				al[i].lcnd[AC_KOJI_PN].ETC_txtcol = 0;
			}
		}
		else {
			pKn = pDBzm->DB_PjisToKnrec( al[i].drec.m_dbt, TRUE );

			if( pKn ) {
				KamokuString( pKn, data.dd_deb_name );
			}
			else	data.dd_deb_name.Empty();
			data.dd_dbr = al[i].drec.m_dbr;

			// 枝番名称表示
			struct _SET_KMKETC sk;
			sk.bmn	= al[i].drec.m_dbmn;
			sk.brn	= al[i].drec.m_dbr;
			sk.code	= al[i].drec.m_dbt;

			if( _set_kmketc( &sk, TRUE ) ) {
				data.dd_deb_etc = sk.etc;
				data.dd_ctbl[DATA_DEBT].text = sk.etc_col;
				al[i].lcnd[AC_KMK_PN].Etcmsg = sk.etc;
				al[i].lcnd[AC_KMK_PN].ETC_txtcol = sk.etc_col;
			}
			else {
				data.dd_deb_etc.Empty();
				data.dd_ctbl[DATA_DEBT].text = (DWORD)-1;
				al[i].lcnd[AC_KMK_PN].Etcmsg.Empty();
				al[i].lcnd[AC_KMK_PN].ETC_txtcol = 0;
			}

			// 部門
			data.dd_dbmn = al[i].drec.m_dbmn;
			data.dd_cbmn = data.dd_dbmn;

			sk.bmn	= al[i].drec.m_dbmn;
			sk.brn	= -1;
			sk.code.Empty();
			sk.kno.Empty();

			if( bDisp && _set_bmnetc( &sk ) ) {
				data.dd_dbmn_etc = sk.etc;
				data.dd_ctbl[DATA_DEB_BMON].text = sk.etc_col;
				al[i].lcnd[AC_BMON_PN].Etcmsg = sk.etc;
				al[i].lcnd[AC_BMON_PN].ETC_txtcol = sk.etc_col;
			}
			else {
				data.dd_dbmn_etc.Empty();
				data.dd_ctbl[DATA_DEB_BMON].text = (DWORD)-1;
				al[i].lcnd[AC_BMON_PN].Etcmsg.Empty();
				al[i].lcnd[AC_BMON_PN].ETC_txtcol = 0;
			}

			// 工事
			data.dd_dkno = al[i].drec.m_dkno;
			data.dd_ckno = data.dd_dkno;

			sk.bmn	= -1;
			sk.brn	= -1;
			sk.code.Empty();
			sk.kno	= al[i].drec.m_dkno;

			if( bDisp && _set_kojietc( &sk ) ) {
				data.dd_dkno_etc = sk.etc;
				data.dd_ctbl[DATA_DEB_KNO].text = sk.etc_col;
				al[i].lcnd[AC_KOJI_PN].Etcmsg = sk.etc;
				al[i].lcnd[AC_KOJI_PN].ETC_txtcol = sk.etc_col;
			}
			else {
				data.dd_dkno_etc.Empty();
				data.dd_ctbl[DATA_DEB_KNO].text = (DWORD)-1;
				al[i].lcnd[AC_KOJI_PN].Etcmsg.Empty();
				al[i].lcnd[AC_KOJI_PN].ETC_txtcol = 0;
			}
		}

		BYTE kin[6], kin2[6];

		::ZeroMemory( data.dd_udtbl, sizeof data.dd_udtbl );
		::ZeroMemory( data.dd_val, sizeof data.dd_val );
		data.val_flag = 0;

		// 追加行は 金額欄を空白で表示
		if( bNyuKin ) {
			memcpy( data.dd_val, al[i].drec.m_val, 6 );
			data.val_flag = al[i].abs_pos > 0 ? 1 : 0;
		}
		else {
			memcpy( kin, al[i].drec.m_val, 6 );
			data.dd_udtbl[0].userdata = kin;
			data.dd_udtbl[0].initflag = al[i].abs_pos > 0 ? 1 : 0;
		}

		memcpy( data.dd_zei, al[i].drec.m_zei, 6 );
		if( is_syohizeisiwake( &al[i].drec) ) {
			data.zei_flag = al[i].abs_pos > 0 ? 1 : 0;
			/* 非課税有価証券譲渡仕訳 */
			if( ( al[i].ac_sign & 0x01) )
				data.zei_flag |= 0x10;

			int sg = 0;
			COLORREF back;
			if (isnot_defzeigaku(&al[i].drec, back))
				sg = 1;

			if (sg > 0)	data.dd_ctbl[ZEI_PN].back = back;
			else		data.dd_ctbl[ZEI_PN].back = (COLORREF)-1;
		}
		else {
			// 消費税税仕訳でない
			data.zei_flag = 0;
			data.dd_ctbl[ZEI_PN].back = (COLORREF)-1;
		}

		data.dd_syz_str = al[i].syzstr;

		//消費税修正サイン
		int sg = SYZMDFY_BIT.bitncnd( data.dd_seq - 1);
		if( sg > 0 )	data.dd_ctbl[DATA_SYZ].back = RGB_SYZ_MDFY;
		else			data.dd_ctbl[DATA_SYZ].back = (COLORREF)-1;

		data.dd_tek.tekiyo = al[i].drec.m_tekiyo;
		// イメージ摘要
		if( al[i].lcnd[ AC_TKY_PN ].IMG_sg == 1 && al[i].drec.m_imgsq > 0 ) {
			if( pDBzm->image->SetAbsolutePosition( al[i].drec.m_imgsq ) == 0 ) {
				IMREC* pIMREC = pDBzm->image->DecodeImage();
				if( pIMREC != NULL ) {
					data.dd_tek.width	= pIMREC->dotxs;
					data.dd_tek.height	= pIMREC->dotys;
					data.dd_tek.image	= (char*)pIMREC->imdata;
				}
				else {
					data.dd_tek.tekiyo	= Image;
					data.dd_tek.width	= 0;
					data.dd_tek.height	= 0;
					data.dd_tek.image	= NULL;
				}
			}
			else {
				data.dd_tek.tekiyo = Image;

				data.dd_tek.width	= 0;
				data.dd_tek.height	= 0;
				data.dd_tek.image	= NULL;
			}
			al[i].lcnd[ AC_TKY_PN ].IMG_sg = 1;
		}
		else {
			data.dd_tek.width	= 0;
			data.dd_tek.height	= 0;
			data.dd_tek.image	= NULL;
		}

		// 変動事由 or 振替区分[公益]
		data.dd_hjiyu.Empty();
		_get_hjiyustr( al[i].drec.m_hjiyu, al[i].drec.m_nbcd, data.dd_hjiyu );

		// 付箋
		data.dd_husen = (al[i].drec.m_dsign[7] & 0x0f);
		data.dd_hscomment = al[i].drec.m_tag;

		data.dd_dsign = 0;
		// 転送済みか？
		if( (st = tline_chk( &al[i].drec )) < 0  ) {
			if( st == -1 )
				data.dd_dsign |= 0x02;
			else
				data.dd_dsign |= 0x08;
		}
		// 取消し
		if( al[i].drec.m_dsign[0] & 0x01 ) {
			data.dd_dsign |= 0x01;
		}

		if( al[i].abs_pos > 0 ) {
			int sg = MODEFY_BIT.bitncnd( al[i].drec.m_seq - 1);
			if( sg == -1 || !sg )
				;
			else
				(data.dd_dsign |= 0x04);

			//選択行か？
			int sgn = 0;

			for( int n = 0; n < m_SelArray.GetCount(); n++ ) {
				if( m_SelArray[n] == al[i].drec.m_seq ) {
					data.dd_dsign |= 0x0400;
					sgn = 1;
					break;
				}
			}
			if( ! sgn ) {
				data.dd_dsign &= ~0x0400;
			}
		}

		memcpy( kin2, al[i].szan, 6 );
//L_PRINT( "---> ListDataSet kin2 ", kin2 ); TRACE( "\n" );

		if( (m_BaseAtt&0x01) )	l_neg( kin2 );
		data.dd_udtbl[1].userdata = kin2;
		data.dd_udtbl[1].initflag = al[i].abs_pos > 0 ? 1 : 0;

		CString stmp;
		if( al[i].drec.m_icno > 0 ) {
			DenpTypeString( al[i].drec.m_dtype, stmp );
		}

		if( stmp.IsEmpty() ) {
			::ZeroMemory( al[i].denp, sizeof al[i].denp );
			data.dd_udtbl[2].userdata = NULL;
		}
		else {
			strcpy_s( al[i].denp, sizeof al[i].denp, stmp );
			data.dd_udtbl[2].userdata = (BYTE*)al[i].denp;
		}

		m_List.SetDtListData( i, &data, FALSE );

	}

	m_List.RedrawDataListCtrl();

}



//	表の1行分データクリア
//
void CDBAbookView::LineDataClear( int ln )
{
	DTLIST_DATA data;
	int i, sv_seq, sv_ddate;
	BYTE sv_ksign, sv_mofs;
	ACBOOK_LINE* al;
	i = ln-1;

	al = m_ALine;

	sv_seq	 = al[i].drec.m_seq;
	sv_ddate = al[i].drec.m_ddate;
	sv_ksign = al[i].drec.m_ksign;
	sv_mofs  = al[i].drec.m_mofs;

	acline_init( &al[i], 1 );
	al[i].abs_pos = 0;

	al[i].drec.m_seq = sv_seq;
	al[i].drec.m_ddate = sv_ddate;
	al[i].drec.m_ksign = sv_ksign;
	al[i].drec.m_mofs = sv_mofs;

	// BCD 日付をセット
	pDBzm->DateConv( al[i].bcddate, &al[i].drec, CV2_1 );

	al[i].lcnd[AC_SEQ_PN].INP_sg = TRUE;
	al[i].lcnd[AC_DATE_PN].INP_sg = TRUE;

	data.dd_seq = al[i].drec.m_seq;
	data.dd_ddate = al[i].drec.m_ddate;
	data.dd_ksign = al[i].drec.m_ksign;
	data.dd_mofs  = al[i].drec.m_mofs;

	data.dd_cno	= -1;
	data.dd_dbmn = -1;
	data.dd_dbmn_etc.Empty();
	data.dd_dkno.Empty();
	data.dd_dkno_etc.Empty();
	data.dd_dbt.Empty();
	data.dd_deb_name.Empty();

	data.dd_dbr = -1;
	data.dd_deb_etc.Empty();
	data.dd_cbmn = -1;
	data.dd_cbmn_etc.Empty();
	data.dd_ckno.Empty();
	data.dd_ckno_etc.Empty();
	data.dd_cre.Empty();
	data.dd_cre_name.Empty();
	data.dd_cbr = -1;
	data.dd_cre_etc.Empty();
	::ZeroMemory( data.dd_val, sizeof data.dd_val );
	data.val_flag = 0;
	::ZeroMemory( data.dd_zei, sizeof data.dd_zei );
	data.zei_flag = 0;
	data.dd_syz_str.Empty();
	data.dd_dsign = 0;

	data.dd_tek.tekiyo.Empty();
	data.dd_tek.width = 0;
	data.dd_tek.height = 0;
	data.dd_tek.image = NULL;
	data.dd_snumber.Empty();
	data.dd_tdate = -1;
	data.dd_tdstr.Empty();
	data.dd_husen = -1;
	data.dd_hscomment.Empty();

	m_List.SetDtListData( i, &data, TRUE );
}


void CDBAbookView::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_DLCTVIEW* dv;
	dv = (NM_DLCTVIEW*)pNMHDR;
//MyTrace( "++Abook++ OnBeginLabelEdit(%d) *** nItem %d nLine %d ItemRect %d,%d,%d,%d\n", PROGRAM_OK, dv->iItem, dv->iLine, 
//							dv->rect.left, dv->rect.top, dv->rect.right, dv->rect.bottom );
//MyTrace( "--------> OnBeginLabelEdit m_bActiveFocus = %d, m_bReloadFocus = (%d)\n", m_bActiveFocus, m_bReloadFocus );
	m_NMbegin = *dv;

	if( PROGRAM_OK == FALSE ) {
		m_bActiveFocus = FALSE;
		return;
	}

	if( acINP_mode == _RETREV && get_nowpn() ==  AC_SELKAMOK_PN && get_nowln() == -1 ) {
		m_bActiveFocus = FALSE;
		return;
	}

	// 他業務からの戻りで ビューの再描画の直前の OnActivateView のフォーカス処理は無視
	// OnActivateView のフォーカスセットが ビューの再描画より先に来たり、ビューの再描画後に来たりする。
	if( m_bReloadFocus && m_bActiveFocus ) {
		m_bActiveFocus = FALSE;
		m_bReloadFocus = FALSE;
		return;
	}
	m_bReloadFocus = FALSE;
	m_bActiveFocus = FALSE;

//MyTrace("--------> OnBeginLabelEdit m_bTermValLinChg = %d, m_bAutoSwk = (%d)\n", m_bTermValLinChg, m_bAutoSwk);

	//仕訳入力複数起動して、タスクバーで切替したとき、日付欄なのに、金額欄のBeginLabeEdit が来てしまう場合があった。
	if (m_bTermValLinChg) {
		if( m_bAutoSwk ) {
			if ( (dv->iItem != DATA_TEKIYO && dv->iItem != DATA_DATE)) {
				m_bTermValLinChg = FALSE;
				return;
			}
		}
		else {
			if (dv->iItem != DATA_DATE) {
				m_bTermValLinChg = FALSE;
				return;
			}
		}
		m_bTermValLinChg = FALSE;
	}
	// 基本項目設定時

//TRACE( "++Abook++ **OnBeginLabelEdit nItem = %d\n", dv->iItem );
	CRect inprc, vwrect;
	inprc = dv->rect;
	m_List.ClientToScreen( inprc );
	ScreenToClient( inprc );

	// 下側が隠れてしまうのを調整
	GetClientRect( vwrect );
	if( inprc.bottom > vwrect.bottom ) {
		int mv = inprc.bottom - vwrect.bottom;
		mv *= -1;
		inprc.OffsetRect( 0, mv );
	}

	char tmp[30];
	CString koji;
	struct _KamokuData check = {0};
	struct _DBKNREC* pKn;

	CString kno;
	int line, edit_id, i, dcsw, bmn;
	INT_PTR pos;
	line = dv->iLine;
/*
	if( dv->iItem == DATA_HUSEN ) {
		FocusHusenInput( this, line, dv->iClick );	
		return;
	}
	else {
		// 付箋フォーカス サインを 戻す。
		if( ! m_bHusenFocus )	m_bHusenFocus = TRUE;
	}
*/

	ACBOOK_LINE* al;

	if( dv->iItem != DATA_TEKIYO ) {
		((CMainFrame*)GetDBDinpMainFrame())->StatusGuideDisp(AFX_IDS_IDLEMESSAGE);

		if( acINP_mode == _APPEND && m_InsLine > 0 ) {
			((CMainFrame*)GetDBDinpMainFrame())->StatusGuideDisp(IDS_DTINS_GUIDE);
		}
	}

	//前期からの繰越
	if ((al = _getLINE_DATA(line)) != NULL) {
		if (al->lattr == 1) {
			int nowpn = get_nowpn();
			int nowln = get_nowln();
			set_nowpn(-1);	// DBEDT のフォーカスOFFの処理で、データ取得をさせない
			_set_focus(nowln, nowpn);
			return;
		}
	}

	if( dv->iItem == DATA_SEQ || dv->iItem == DATA_USER2 || dv->iItem == DATA_USER3 ) {
//MyTrace( "--------> OnBeginLabelEdit DATA_SEQ iClick = %d\n", dv->iClick );

		if( dv->iItem == DATA_SEQ && dv->iClick ) {
			long vkKey = 0;
			if( dv->iClick & 0x10 )	vkKey = VK_SHIFT;
			if( dv->iClick & 0x20 )	vkKey = VK_CONTROL;
			click_seljob( line, vkKey );
		}

		// 番号・差引残高・伝票タイプ
		int nowpn = get_nowpn();
		set_nowpn(-1);	// DBEDT のフォーカスOFFの処理で、データ取得をさせない
		_set_focus( line, nowpn );
		return;
	}
	else if( dv->iItem == DATA_DEBT ) {
		al = _getLINE_DATA( line );
		if( al->abs_pos == 0 ) {
			if( m_bAutoSwk ) {
				if( ! al->lcnd[AC_KMK_PN].INP_sg ) {	//科目未入力
				//	set_nowpn(-1);	// DBEDT のフォーカスOFFの処理で、データ取得をさせない
				//	_set_focus( line, AC_TKY_PN );
				//	return;
				}
			}
		}
	}
	else if( dv->iItem == DATA_TEKIYO || dv->iItem == DATA_VAL || dv->iItem == DATA_ZEI || 
			 dv->iItem == DATA_USER1 || dv->iItem == DATA_SYZ ) {
		al = _getLINE_DATA( line );

		BOOL bDenpData = FALSE;

		if( al->abs_pos == 0 ) {
			if( ! m_bAutoSwk ) {
				if( ! al->lcnd[AC_KMK_PN].INP_sg ) {	//科目未入力
					set_nowpn(-1);	// DBEDT のフォーカスOFFの処理で、データ取得をさせない
					_set_focus( line, AC_KMK_PN );
					return;
				}
			}
			else {	// 自動仕訳入力
				if( dv->iItem != DATA_TEKIYO ) {
					if( ! al->lcnd[AC_KMK_PN].INP_sg ) {	//科目未入力
						set_nowpn(-1);	// DBEDT のフォーカスOFFの処理で、データ取得をさせない
						_set_focus( line, AC_TKY_PN );
						return;
					}
				}
			}
		}
		else {
			if( DenpTypeMode() ) {
				if( al->drec.m_icno > 0 ) {
					bDenpData = TRUE;
				}
			}
		}

		if( !bDenpData ) {
			// 借方／貸方が同じ科目の場合は、入金として処理
			if( dv->iItem == DATA_USER1 ) {
				if( _IsSameKamoku( &al->drec ) ) {
					//科目を変更した場合で、出金にある場合
					if( al->lcnd[AC_SKIN_PN].INP_sg ) {
						al->lcnd[AC_NKIN_PN].INP_sg = TRUE;
						al->lcnd[AC_SKIN_PN].INP_sg = FALSE;
						TerminationDataSet( AC_SKIN_PN );
					}

					set_nowpn(-1);	// DBEDT のフォーカスOFFの処理で、データ取得をさせない
					_set_focus( line, AC_NKIN_PN );
					return;
				}
			}
		}
	}

	CICSDBEDT* pDBed;
	CDataListCtrl* pListCtrl;
	edit_id = IDC_ICSDBEDTCTRL1;
	pDBed = &m_Input;
	pListCtrl = &m_List;
	
	DBeditCtrlSet( dv->iItem );

	BYTE buf[100] = {0};
//	char fmt[20];
	VARIANT var;

	pDBed->ShowWindow(SW_SHOW);

	if( dv->iItem == DATA_DATE && m_bMnthFix ) {
		i = inprc.Width() / 2;
		pDBed->MoveWindow( inprc.left + i, inprc.top, i, inprc.Height() );
	}
	else {
		pDBed->MoveWindow( inprc.left, inprc.top, inprc.Width(), inprc.Height() );
	}
//	pDBed->MoveWindow( inprc.left, inprc.top, inprc.Width(), inprc.Height() );

	c_set( edit_id );

	switch( dv->iItem ) {
	case DATA_SEQ:

		break;
	case DATA_DATE:
		FocusIcsDate( line, dv->iClick );

		if( ! m_bMnthFix ) {
			memcpy( buf, getLINE_DATA()->bcddate, 2 );
		}
		else {
			buf[0] = getLINE_DATA()->bcddate[1];
		}
		var.pbVal = buf;
		pDBed->SetData( &var, ICSDBEDT_TYPE_BCD, m_bMnthFix ? 2 : 4 );
		pDBzm->BcdDateToString( tmp, sizeof tmp, getLINE_DATA()->bcddate, m_bMnthFix );
		pDBed->SetDisplayString( tmp );
		break;

	case DATA_DENP:
		FocusIcsDenp( line, dv->iClick );
		if( getLINE_DATA()->drec.m_cno != -1 ) {
			sprintf_s( (char*)buf, sizeof buf, "%d", getLINE_DATA()->drec.m_cno );
			var.pbVal = buf;
		}
		else {
			var.pbVal = NULL;
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
		break;

	case DATA_DOCEVI:
		FocusIcsDocevi( line, dv->iClick );
		if( ! getLINE_DATA()->drec.m_skn.IsEmpty() ) {
			strcpy_s( (char*)buf, sizeof buf, getLINE_DATA()->drec.m_skn );
			var.pbVal = buf;
		}
		else {
			var.pbVal = NULL;
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
		break;

	case DATA_DEB_BMON:
		FocusIcsBumon( line, dv->iClick );

		// 基本科目がどちら側か？
		dcsw = GetRecordDCsgn( &getLINE_DATA()->drec );

		if( dcsw == 1 ) {
			bmn = getLINE_DATA()->drec.m_dbmn;
		}
		else {
			bmn = getLINE_DATA()->drec.m_cbmn;
		}
		if( bmn != -1 ) {
			pDBzm->BumonCodeToStr( (char*)buf, sizeof buf, bmn );
			var.pbVal = buf;
		}
		else {
			var.pbVal = NULL;
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
		// 部門名称を表示？
		if( ! getLINE_DATA()->lcnd[AC_BMON_PN].Etcmsg.IsEmpty() )
			pListCtrl->EtcStaticDisp( dv->rect, getLINE_DATA()->lcnd[AC_BMON_PN].Etcmsg, getLINE_DATA()->lcnd[AC_BMON_PN].ETC_txtcol );

		break;

	case DATA_DEB_KNO:
		FocusIcsKoji( line, dv->iClick );

		// 基本科目がどちら側か？
		dcsw = GetRecordDCsgn( &getLINE_DATA()->drec );

		if( dcsw == 1 ) {
			kno = getLINE_DATA()->drec.m_dkno;
		}
		else {
			kno = getLINE_DATA()->drec.m_ckno;
		}
		if( ! kno.IsEmpty() ) {
			pDBzm->KojiCodeToStr( (char*)buf, sizeof buf, kno );
			var.pbVal = buf;
		}
		else {
			var.pbVal = NULL;
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
		// 工事名称を表示？
		if( ! getLINE_DATA()->lcnd[AC_KOJI_PN].Etcmsg.IsEmpty() )
			pListCtrl->EtcStaticDisp( dv->rect, getLINE_DATA()->lcnd[AC_KOJI_PN].Etcmsg, getLINE_DATA()->lcnd[AC_KOJI_PN].ETC_txtcol );

		break;
	case DATA_DEBT:
		FocusIcsKamoku( line, dv->iClick );
		// 基本科目がどちら側か？
		dcsw = GetRecordDCsgn( &getLINE_DATA()->drec );

		if( dcsw == 1 ) {
			memcpy( check.kd_code, getLINE_DATA()->drec.m_dbt, KMKCD_LN );
			check.kd_eda = getLINE_DATA()->drec.m_dbr;
			pKn = pDBzm->DB_PjisToKnrec( getLINE_DATA()->drec.m_dbt, TRUE );
		}		
		else {
			memcpy( check.kd_code, getLINE_DATA()->drec.m_cre, KMKCD_LN );
			check.kd_eda = getLINE_DATA()->drec.m_cbr;
			pKn = pDBzm->DB_PjisToKnrec( getLINE_DATA()->drec.m_cre, TRUE );
		}

		if( pKn ) {
			KamokuString( pKn, check.kd_name, sizeof check.kd_name );
		}
		else {
			::ZeroMemory( &check, sizeof check );
		}
		DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, pDBzm );

		if( ! getLINE_DATA()->lcnd[AC_KMK_PN].Etcmsg.IsEmpty() )
			pListCtrl->EtcStaticDisp( dv->rect, getLINE_DATA()->lcnd[AC_KMK_PN].Etcmsg, getLINE_DATA()->lcnd[AC_KMK_PN].ETC_txtcol );
		break;	

	case DATA_TEKIYO:
		FocusIcsTekiyo( line, dv->iClick );

		::ZeroMemory( buf, sizeof buf );
		strcpy_s( (char*)buf, sizeof buf, getLINE_DATA()->drec.m_tekiyo );
		var.pbVal = buf; 
		pDBed->SetData( &var, ICSDBEDT_TYPE_STRING, 0 );
		pos = strlen( (char*)buf );

		pDBed->SetCaretPosition(pos);
		((CMainFrame*)GetDBDinpMainFrame())->StatusGuideDisp(IDS_HUSEN_GUIDE);
		((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(5, pos == 0 ? 0x80 : 0x82);
		break;

	case DATA_HJIYU:
		FocusIcsHjiyu( line, dv->iClick );
		break;

	case DATA_VAL:	// 入金
		FocusIcsVal( line, 0, dv->iClick );

		if( getLINE_DATA()->lcnd[AC_NKIN_PN].INP_sg ) {
			memcpy( buf, getLINE_DATA()->drec.m_val, 6 );
			var.pbVal = buf; 

//L_PRINT( "---> nyukin", buf );
		}
		else  {
			var.pbVal = NULL; 
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_ARITH, 0 );

		StatusBarValGuide();
		break;

	case DATA_USER1:	// 出金
		FocusIcsVal( line, 1, dv->iClick );

		if( getLINE_DATA()->lcnd[AC_SKIN_PN].INP_sg ) {
			memcpy( buf, getLINE_DATA()->drec.m_val, 6 );
			var.pbVal = buf; 

//L_PRINT( "---> nyukin", buf );
//TRACE( "nowln = %d, line %d\n", get_nowln(), line );
		}
		else  {
			var.pbVal = NULL; 
		}
		pDBed->SetData( &var, ICSDBEDT_TYPE_ARITH, 0 );

		StatusBarValGuide();
		break;

	case DATA_ZEI:
		FocusIcsZei( line, dv->iClick );

		if( getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg ) {
			memcpy( buf, getLINE_DATA()->drec.m_zei, 6 );
			var.pbVal = buf; 
		}
		else  {
			var.pbVal = NULL; 
		}

		pDBed->SetData( &var, ICSDBEDT_TYPE_ARITH, 0 );

		break;
	case DATA_SYZ:
		FocusIcsSyz( line, dv->iClick );
		break;

	default:
		break;
	}

	//ツールバー文字列
	int sub_sw = 0;
	if( dv->iItem != DATA_TEKIYO ) {	//摘要欄は上で変更済み
		if( dv->iItem == DATA_DATE && 
			(acINP_mode == _APPEND && m_InsLine == 0) )	sub_sw = 1;

		((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(5, sub_sw);
	}
//MyTrace( "--------> OnBeginLabelEdit END --------------->\n" );
}

void CDBAbookView::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
//MyTrace( "++Abook++ **OnEndLabelEdit\n" );
	m_Input.ShowWindow(SW_HIDE);
}

void CDBAbookView::OnSetDispInfo( NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_DLCTVIEW* dv;
	dv = (NM_DLCTVIEW*)pNMHDR;
//TRACE( "*** nItem %d nLine %d ItemRect %d,%d,%d,%d\n", dv->iItem, dv->iLine, 
//							dv->rect.left, dv->rect.top, dv->rect.right, dv->rect.bottom );
//MyTrace( "++Abook++ **OnSetDispInfo　PROGRAM_OK(%d)\n", PROGRAM_OK );
	CRect inprc;
	inprc = dv->rect;
	m_List.ClientToScreen( inprc );
	ScreenToClient( inprc );

	if( dv->iItem == DATA_HUSEN ) {
	}
	else {
		CRect inpRect;
		m_Input.GetWindowRect(&inpRect);
		ScreenToClient( inpRect );
//MyTrace( "++Abook++ **OnSetDispInfo (%d, %d, %d, %d) nowRect(%d, %d, %d, %d) \n", inprc.left, inprc.top, inprc.Width(), inprc.Height(),
//		inpRect.left, inpRect.top, inpRect.Width(), inpRect.Height() );
		if( inprc.EqualRect( inpRect ) )	return;
		//枝番未登録のダイアログが出ると、科目欄に入力ボックスがあった、OnSetDispInfo が遅れてくる。
		//OnRetroFocus()内 のm_List.SetPosition()で 入力ボックスの情報を更新した後、PostMessageされた分が来るため。
		if (!PROGRAM_OK)	return;

		if (dv->iItem == m_NMbegin.iItem) {
			m_Input.MoveWindow(inprc.left, inprc.top, inprc.Width(), inprc.Height());
		}
	}
}


// エディットコントロールの セット
void CDBAbookView::DBeditCtrlSet( int id_pn )
{
	BYTE buf[100] = {0};
	VARIANT var;
	var.pbVal = NULL;
	CICSDBEDT* pDBed;
	pDBed = &m_Input;

	if( id_pn == DATA_DATE ) {
		pDBed->SetData( &var, ICSDBEDT_TYPE_BCD, 4 );
	}
	if( id_pn == DATA_VAL || id_pn == DATA_ZEI || id_pn == DATA_USER1 ) {
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
//		pDBed->SetKana( FALSE );
		pDBed->SetKana( TRUE );
		pDBed->SetZenkaku( FALSE );
		pDBed->SetAscii( FALSE );
		pDBed->SetAlphabet( FALSE );
		pDBed->SetFormat( FALSE ); // カンマ編集
		pDBed->SetMinval( "0" );
//		sprintf_s( tmp, sizeof tmp, "%d", _op_max_calq( pDBzm->zvol->bmcol ) );
//		pDBed->SetMaxval( tmp );
//		pDBed->SetMaxLen( 0 );
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
	case DATA_VAL:		// 入金
	case DATA_USER1:	// 出金
	case DATA_ZEI:		// 税額
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
		pDBed->SetMaxLen( 10 );
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


//---------------------------------------------------------------------------------
// 基本科目の開始残高セット
//   返送値は 底に表示しているデータ位置(１からはじまる)
//
//---------------------------------------------------------------------------------
void CDBAbookView::BaseSzanSet( CACSzan* pas )
{
	CString filter;
	char	szan[6] = {0};
	DBKNREC* pKn;

	pKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
	if( ! pKn ) {
		memcpy( pas->ac_szan, szan, 6 );
		return;
	}

	int mode = pDBzm->zanview_authority();

	if( mode == 0 ) {
		// 残高は、閲覧不可
		memcpy( pas->ac_szan, szan, 6 );
		return;
	}
	else if( mode == 2 ) {
		// 利益の残高は、閲覧不可
		DWORD dwCode = _atoh( (LPSTR)(LPCTSTR)m_BaseCode );

		if( (dwCode & 0xff000000) == 0x07000000 ) {
			memcpy( pas->ac_szan, szan, 6 );
			return;
		}
	}

	int		i, st_ofs = -1;
	char	val[6] = {0}, work[6], dbt[6], cre[6];

	pDBzm->minfo->MoveFirst();
	do {
		if( (pDBzm->minfo->insw & 0x01) ) {
			st_ofs = pDBzm->minfo->ofset;
			break;
		}
	} while( pDBzm->minfo->MoveNext() == 0 );

	//仕訳期間対応
	if( ((CMainFrame*)GetDBDinpMainFrame())->AcbkKikanSwk() ) {
		if( m_BaseOfst > st_ofs ) {
			st_ofs = m_BaseOfst;
		}
	}

	if( m_BaseBmn != -1 ) {
		if( m_BaseEda != -1 ) {
			filter.Format( "ezkcd = '%s' and ezbmn = %d and ezecd = %d", m_BaseCode, m_BaseBmn, m_BaseEda );
			pDBzm->bmezrec->Requery( filter );

			if( pDBzm->bmezrec->st == 0 ) {
				l_input( szan, (char*)(LPCTSTR)pDBzm->bmezrec->ezzan );

				if( st_ofs > 0 ) {
					for( i = 0; i < st_ofs; i++ ) {
						l_input( dbt, (char*)(LPCTSTR)pDBzm->bmezrec->lhs[i] );
						l_input( cre, (char*)(LPCTSTR)pDBzm->bmezrec->rhs[i] );

						l_sub( work, dbt, cre );
						l_add( val, val, work );
					}
				}
			}
		}
		else {
			filter.Format( "kzkcd = '%s' and kzbmn = %d", m_BaseCode, m_BaseBmn );
			pDBzm->bmkzrec->Requery( filter );
		
			if( pDBzm->bmkzrec->st == 0 ) {
				l_input( szan, (char*)(LPCTSTR)pDBzm->bmkzrec->kzzan );

				if( st_ofs > 0 ) {
					for( i = 0; i < st_ofs; i++ ) {
						l_input( dbt, (char*)(LPCTSTR)pDBzm->bmkzrec->lhs[i] );
						l_input( cre, (char*)(LPCTSTR)pDBzm->bmkzrec->rhs[i] );

						l_sub( work, dbt, cre );
						l_add( val, val, work );
					}
				}
			}
		}
	}
	else if( m_BaseEda != -1 ) {
		filter.Format( "ezkcd = '%s' and ezecd = %d", m_BaseCode, m_BaseEda );
		pDBzm->ezrec->Requery( filter );

		if( pDBzm->ezrec->st == 0 ) {
			l_input( szan, (char*)(LPCTSTR)pDBzm->ezrec->ezzan );

			if( st_ofs > 0 ) {
				for( i = 0; i < st_ofs; i++ ) {
					l_input( dbt, (char*)(LPCTSTR)pDBzm->ezrec->lhs[i] );
					l_input( cre, (char*)(LPCTSTR)pDBzm->ezrec->rhs[i] );

					l_sub( work, dbt, cre );
					l_add( val, val, work );
				}
			}
		}
	}
	else {
		filter.Format( "kzkcd = '%s'", m_BaseCode );
		pDBzm->kzrec->Requery( filter );
		pDBzm->kzrec_Requery() = TRUE;

		if( pDBzm->kzrec->st == 0 ) {
			l_input( szan, (char*)(LPCTSTR)pDBzm->kzrec->kzzan );

			if( st_ofs > 0 ) {
				for( i = 0; i < st_ofs; i++ ) {
					l_input( dbt, (char*)(LPCTSTR)pDBzm->kzrec->lhs[i] );
					l_input( cre, (char*)(LPCTSTR)pDBzm->kzrec->rhs[i] );

					l_sub( work, dbt, cre );
					l_add( val, val, work );
				}
			}
		}
	}

	memcpy( pas->ac_szan, szan, 6 );

	if( (m_BaseAtt & 0x01) ) {
		l_neg( pas->ac_szan );
	}
	l_add( pas->ac_szan, pas->ac_szan, val );
}

//---------------------------------------------------------------------------------
// 先頭データから表示
//   返送値は 底に表示しているデータ位置(１からはじまる)
//
//---------------------------------------------------------------------------------
int CDBAbookView::TopDataSet(int mofs/*=-1*/)
{
	char szan[6] = {0}, tmp[6] = {0}, work[6] = {0};

	int dispcnt = GetACListCount(); /*m_List.GetDispDataCount();*/
	int i, lcnt, abpos, reccnt, dcnt, dcsw, addsw, rval=0;
	CACSzan	aszan;
	CDBINPDataRec	rec;

	CWaitCursor wc;

	Acbook_Requery();

	acline_init( m_ALine, ACREC_MAX );

	lcnt = dispcnt;

	pDBzm->data->MoveFirst();
	abpos = 1;

	if( pDBzm->data->st != -1 ) {
		if( pDBzm->DBDATA_SetPosition( pDBzm->data->seq ) == 0 ) {
			pDBzm->GetCDBData( &m_SREC );
		}
		else {
			ICSExit( 0, "データを取得できません！" );
			return -1;
		}
	}

	dcnt	= 0;
	reccnt	= pDBzm->data->GetRecordCount();
	// 差引残高エリアクリア
	m_SZan.RemoveAll();
	m_SZan.SetSize( reccnt+1 );//開始残高用＋１

	// 基本科目の開始残高を取得
	BaseSzanSet( &aszan );
	m_SZan[0] = aszan;

	if( reccnt > 0 ) {
		do {
			pDBzm->GetCData( &rec );
			dcsw = GetRecordDCsgn( &rec );
			aszan.ac_szrec	= rec;

#ifndef SZAN_CLOSE	// 11.05 /12
			if( dcsw != -1 && IsSzanCalq( &rec ) ) {
#else
			if( dcsw != -1 ) {
#endif
				if( dcsw == 0 ) {
					l_add( aszan.ac_szan, aszan.ac_szan, rec.m_val );
					if( sotozei_data( &rec ) )
						l_add( aszan.ac_szan, aszan.ac_szan, rec.m_zei );
				}
				else {
					l_sub( aszan.ac_szan, aszan.ac_szan, rec.m_val );
					if( sotozei_data( &rec ) )
						l_sub( aszan.ac_szan, aszan.ac_szan, rec.m_zei );
				}
			}
			m_SZan[dcnt+1] = aszan;
			dcnt++;

		} while( pDBzm->data->MoveNext() == 0 );
	}

	i = 0;
	if( reccnt > 0 ) {
		//先頭から
		if( mofs < 0 || (m_dwOpt&0x0f) != 0 ) {
			pDBzm->data->MoveFirst();

			for( i = 0; i < lcnt; i++ ) {
				if (i == 0) {
					//開始残高セットする
					if (l_test(m_SZan[0].ac_szan) != 0) {
						acline_zanset(m_ALine, &m_SZan[0]);
						continue;
					}
				}

				acline_set( m_ALine, i, abpos );

				memcpy( m_ALine[i].szan, m_SZan[abpos].ac_szan, sizeof szan );
				abpos++;

				pDBzm->data->MoveNext();
				if( pDBzm->data->st == -1 ) {
					pDBzm->data->MovePrev();
					break;
				}
			}
		}
		else {
			//指定月より
			pDBzm->data->MoveFirst();

			i = addsw = 0;
			while(1) {
				if( pDBzm->data->mofs >= mofs ) {
					addsw = 1;
					if (abpos == 1 && i == 0 ) {
						//開始残高セットする
						if (l_test(m_SZan[0].ac_szan) != 0) {
							acline_zanset(m_ALine, &m_SZan[0]);
							i++;
							continue;
						}
					}
					acline_set( m_ALine, i, abpos );
					memcpy( m_ALine[i].szan, m_SZan[abpos].ac_szan, sizeof szan );
				}
				abpos++;

				pDBzm->data->MoveNext();
				if( pDBzm->data->st == -1 ) {
					pDBzm->data->MovePrev();
					break;
				}

				if( addsw ) {
					i++;
					if( i >= lcnt )
						break;
				}
			}
		}
	}
	else {
		i = 0;
		//開始残高セットする
		if (l_test(m_SZan[0].ac_szan) != 0) {
			acline_zanset(m_ALine, &m_SZan[0]);
			i = 1;
		}
	}

	int addidx = -1;
	if( reccnt > 0 ) {

		if( mofs > 0 ) {
			if( i == 0 && ! addsw ) {
				addidx = 0;
				rval = 0;
			}
		}

		if( addidx < 0 ) {
			if( (i+1) < lcnt ) {
				addidx = (i+1);
			}
			rval = (i+1);
		}
	}
	else {
		addidx = i;
		rval = 0;
	}

	if( addidx != -1 && ! bCONFIRM_MASTER ) {
		// 新規行 を追加
		m_ALine[addidx].abs_pos = 0;
		int newseq = pDBzm->m_lastdata.m_seq;

		pDBzm->GetInpDate( &m_ALine[addidx].drec );
		pDBzm->GetInpDate( m_ALine[addidx].bcddate );
		m_ALine[addidx].drec.m_seq = newseq;
		m_ALine[addidx].lcnd[AC_SEQ_PN].INP_sg = TRUE;
		m_ALine[addidx].lcnd[AC_DATE_PN].INP_sg = TRUE;
	}

	return rval;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 初期データ表示
// 返送値は 底に表示しているデータ位置(１からはじまる)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int CDBAbookView::InitDataSet()
{
	Acbook_Requery();

	char szan[6] = {0}, tmp[6] = {0}, work[6] = {0};

	// 基本科目・枝番が未登録か、枝番チェック
	CDBINPDataRec data;
	BOOL	bEdaDel = FALSE;
	if( m_BaseEda != -1 && ! bCONFIRM_MASTER ) {
		data.m_dbmn	= m_BaseBmn;
		data.m_dbt	= m_BaseCode;
		data.m_dbr	= m_BaseEda;
		int st = ac_brchk1( &data, 0 );
		VARIANT var;

		if( st == 99 )
		{
			//枝番登録選択
			st = brzan_torok( this, data.m_dbmn, data.m_dbt, data.m_dbr, 0 );

			if( st == -1 ) {		//キャンセル
				m_BaseEda = -1;		//基本枝番消去
				var.pbVal = NULL;
				DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
				GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
				bEdaDel = TRUE;
			}
			else {
				if (st == 0) {
					if (pDBzm->IsDiffEdabanSyzSgn((char*)(LPCTSTR)data.m_dbt, data.m_dbr)) {
						if (pDBsy->SyReOpen(pDBzm) == -1) {
							ICSExit(0, "消費税クラスの再読み込みができません。！");
							return -1;
						}
					}
				}

				// 枝番名称表示
				struct _SET_KMKETC sk;
				sk.bmn = m_BaseBmn;
				sk.brn = m_BaseEda;
				sk.code = m_BaseCode;

				if( _set_kmketc( &sk, FALSE ) ) {
					GetDlgItem(IDC_STATIC5)->SetWindowText( sk.etc );
				}
				else {
					GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
				}
			}
		}
		else if( st == 2 || st == 3 ) {	
			// 枝番無しか登録MAX
			m_BaseEda = -1;		//基本枝番消去
			var.pbVal = NULL;
			DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
			GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
			bEdaDel = TRUE;
		}
	}

	int dcnt = 0;
	int i, lcnt, abpos, n, dcsw;

	int reccnt = pDBzm->data->GetRecordCount();
	if( bEdaDel ) {
		Acbook_Requery();
		reccnt = pDBzm->data->GetRecordCount();
	}

	CACSzan aszan;

	// 差引残高エリアクリア
	m_SZan.RemoveAll();
	m_SZan.SetSize(reccnt + 1);//開始残高用＋１

	// 基本科目の開始残高を取得
	BaseSzanSet( &aszan );
	m_SZan[0] = aszan;

	//定型登録 ソート用
	SCMDFY_SEQ.fill();

//MyTrace( "InitDataSet reccnt = %d\n", reccnt );
	if( reccnt > 0 ) {
		do {
			pDBzm->GetCData( &data );
			dcsw = GetRecordDCsgn( &data );
			aszan.ac_szrec	= data;

#ifndef SZAN_CLOSE	// 11.05 /12
			if( dcsw != -1 && IsSzanCalq( &data ) ) {
#else
			if( dcsw != -1 ) {
#endif
				if( dcsw == 0 ) {
					l_add( aszan.ac_szan, aszan.ac_szan, data.m_val );
					if( sotozei_data( &data ) )
						l_add( aszan.ac_szan, aszan.ac_szan, data.m_zei );
				}
				else {
					l_input( tmp, (char*)(LPCTSTR)pDBzm->data->val );
					l_sub( aszan.ac_szan, aszan.ac_szan, data.m_val );
					if( sotozei_data( &data ) )
						l_sub( aszan.ac_szan, aszan.ac_szan, data.m_zei );
				}
			}
			m_SZan[dcnt+1] = aszan;
			dcnt++;
			// 定型仕訳 ソート用
			SCMDFY_SEQ.nsetseq( dcnt, pDBzm->data->seq-1);

		} while( pDBzm->data->MoveNext() == 0 );
	}
	int dispcnt = GetACListCount(); /*m_List.GetDispDataCount();*/
	m_ListCnt = dispcnt;

//MyTrace( "InitDataSet m_ListCnt = %d\n", m_ListCnt );

	acline_init( m_ALine, ACREC_MAX );

	lcnt = (dispcnt / 2);
	if (dcnt < lcnt) {
		lcnt = dcnt;
		//前期より繰越あり
		if (l_test(m_SZan[0].ac_szan) != 0) {
			lcnt++;
			//前残行をセットする
			acline_zanset(&m_ALine[0], &m_SZan[0]);
		}
	}

	pDBzm->data->MoveLast();
	abpos = dcnt;
	memcpy( work, szan, sizeof work );

	if( reccnt > 0 ) {
		if( pDBzm->DBDATA_SetPosition( pDBzm->data->seq ) == 0 ) {
			pDBzm->GetCDBData( &m_SREC );
		}
		else {
			ICSExit( 0, "データを取得できません！" );
			return -1;
		}
	}
	// 差引残高最終インデックス(前期残高が[0]にある)
	n = dcnt;

	for( i = lcnt; i > 0; i--, n-- ) {
		// 新規行 を考えて、1行前から セット
		acline_set( m_ALine, (i-1), abpos );

		memcpy( m_ALine[i-1].szan, m_SZan[n].ac_szan, sizeof m_SZan[n].ac_szan );

		abpos--;

		pDBzm->data->MovePrev();
		if( pDBzm->data->st == -1 ) {
			pDBzm->data->MoveNext();
			break;
		}
		if (abpos == 0)
			break;
	}

	// 新規行 を追加
	if( ! bCONFIRM_MASTER ) {
		m_ALine[lcnt].abs_pos = 0;
		int newseq = pDBzm->m_lastdata.m_seq;

		pDBzm->GetInpDate( &m_ALine[lcnt].drec );
		pDBzm->GetInpDate( m_ALine[lcnt].bcddate );
		m_ALine[lcnt].drec.m_seq = newseq;
	}
	else {
#ifdef CLOSE // 03.04 /11
		if( lcnt > 0 )	// もしデータがある場合は、新規行分 減らす
			lcnt--;
#endif
	}

	return lcnt;
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// データ追加時の再SET
// CDBINPDataRec* pcrec ---- 追加したレコード
//
// 返送値は 底に表示しているデータ位置(１からはじまる)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int CDBAbookView::DataAppendReset( CDBINPDataRec* pcrec )
{
	char szan[6] = {0}, tmp[6] = {0}, work[6] = {0};
	int n, dcnt = 0;
	int dispcnt = GetACListCount(); /*m_List.GetDispDataCount();*/
	int i, abpos, reccnt, dcsw;
	CACSzan aszan;
	CDBINPDataRec	drec;

	// 日付順に並べ替える
	Acbook_Requery();

	reccnt = pDBzm->data->GetRecordCount();

	// 先頭で表示している仕訳を覚える
	int sv_seq = -1;
	int zen_zan = 0;

	for (i = 0; i < ACREC_MAX; i++) {
		if (m_ALine[0].lattr == 1) {
			zen_zan = 1;	//前期繰越行あり
			continue;
		}
		else {
			if (m_ALine[i].abs_pos <= 0) {
				break;
			}
			//取り消し仕訳
			if (m_ALine[i].drec.m_dsign[0] & 0x01) {
				continue;
			}
			sv_seq = m_ALine[i].drec.m_seq;
			break;
		}
	}

	// 差引残高エリアクリア
	m_SZan.RemoveAll();
	m_SZan.SetSize(reccnt + 1);//開始残高用＋１

	// 基本科目の開始残高を取得
	BaseSzanSet( &aszan );
	m_SZan[0] = aszan;

	n = 0;
	if( reccnt > 0 ) {
		do {
			pDBzm->GetCData( &drec );
			dcsw = GetRecordDCsgn( &drec );
			aszan.ac_szrec	= drec;

			// 差引残高を配列にセット
#ifndef SZAN_CLOSE	// 11.05 /12
			if( dcsw != -1 && IsSzanCalq( &drec ) ) {
#else
			if( dcsw != -1 ) {
#endif
				if( dcsw == 0 ) {
					l_add( aszan.ac_szan, aszan.ac_szan, drec.m_val );
					if( sotozei_data( &drec ) )
						l_add( aszan.ac_szan, aszan.ac_szan, drec.m_zei );
				}
				else {
					l_sub( aszan.ac_szan, aszan.ac_szan, drec.m_val );
					if( sotozei_data( &drec ) )
						l_sub( aszan.ac_szan, aszan.ac_szan, drec.m_zei );
				}
			}
			m_SZan[n+1] = aszan;
			n++;
			if (dcnt == 0) {
				if (reccnt < dispcnt) {
					//先頭より前に今回追加した分があれば、それを表示する
					if (sv_seq == pDBzm->data->seq || pcrec->m_seq == pDBzm->data->seq) {
						dcnt = n;
					}
				}
				else {
					if (sv_seq == pDBzm->data->seq) {
						dcnt = n;
					}
				}
			}
		} while( pDBzm->data->MoveNext() == 0 );

		if (sv_seq == -1) {
			dcnt = 1;
		}
	}
	else {
		dcnt = 0;
	}

	// 該当仕訳位置まで検索
	if( dcnt > 0 ) {
		pDBzm->data->SetAbsolutePosition( dcnt );
	}

	acline_init( m_ALine, ACREC_MAX );

	abpos = dcnt;
	memcpy( work, szan, sizeof work );

/*	if( pDBzm->DBDATA_SetPosition( pDBzm->data->seq ) == 0 ) {
		pDBzm->GetCDBData( &m_SREC );
	}
	else {
		ICSExit( 0, "データを取得できません！" );
		return;
	}
*/

	if (zen_zan) {
		acline_zanset(&m_ALine[0], &m_SZan[0]);
		i = 1;
	}
	else {
		i = 0;
	}
	if( abpos > 0 ) {
		do {
			acline_set( m_ALine, i, abpos );

			memcpy( m_ALine[i].szan, m_SZan[abpos].ac_szan, sizeof m_SZan[abpos].ac_szan );
			i++;
			abpos++;

			// 最終行
			if( i >= ACREC_MAX )
				break;

		} while( pDBzm->data->MoveNext() == 0 );
	}

	// 新規行 を追加
	int newseq;
	newseq = m_CREC.m_seq;
	newseq++;

	if( m_InsLine != 0 ) {
		pDBzm->SetInpDate( m_svInpDate );
	}
	if( i < ACREC_MAX ) {
		m_ALine[i].abs_pos = 0;
		pDBzm->GetInpDate( &m_ALine[i].drec );
		pDBzm->GetInpDate( m_ALine[i].bcddate );
		m_ALine[i].drec.m_seq	= newseq;
	}
/*
	if( i > 0 ) {
		m_ALine[i].drec.m_seq	= newseq;
		m_ALine[i].drec.m_ddate	= pcrec->m_ddate;
		m_ALine[i].drec.m_mofs	= pcrec->m_mofs;
		m_ALine[i].drec.m_ksign	= pcrec->m_ksign;
	}
	else {
		m_ALine[i].drec.m_seq	= newseq;
		// 入力日付をセット
		set_inpdate( &m_ALine[i].drec );
	}
*/
	// 先頭データセット
	int top_idx = zen_zan ? 1 : 0;

	if( m_ALine[top_idx].abs_pos > 0 ) {
		pDBzm->data->SetAbsolutePosition( m_ALine[top_idx].abs_pos );
	}

	return i+1;
}


// 出納帳 ページレコードクリア
void CDBAbookView::acline_init( ACBOOK_LINE* aline, int size )
{
	int i, n;

	for( i = 0; i < size; i++ ) {
		(aline + i)->abs_pos = -1;
		(aline + i)->dtype	 = -1;
		(aline + i)->ac_sign = 0;
		(aline + i)->drec.Reset();

		::ZeroMemory( (aline + i)->bcddate, sizeof (aline + i)->bcddate );

		for( n = 0; n < AC_ICNT; n++ ) {
			(aline + i)->lcnd[n].INP_sg = 0;
			(aline + i)->lcnd[n].INP_type = 0;
			(aline + i)->lcnd[n].TKattr = 0;
			(aline + i)->lcnd[n].KEY_inp = 0;
			(aline + i)->lcnd[n].NEXT_MOVE_KEY = 0;
			(aline + i)->lcnd[n].IMG_sg = 0;

			(aline + i)->lcnd[n].ETC_txtcol = 0;
			(aline + i)->lcnd[n].Etcmsg.Empty();
		}

		::ZeroMemory( (aline + i)->syzstr, sizeof (aline + i)->syzstr );

		::ZeroMemory( (aline + i)->szan, sizeof (aline + i)->szan );

		::ZeroMemory((aline + i)->denp, sizeof(aline + i)->denp);
		(aline + i)->lattr = -1;
	}
}


//-------------------------------------------------------------
//	消費税ボリュームに登録されている科目か？
//		消費税ボリュームは SyOpen() で 開かれている
//
//-------------------------------------------------------------
BOOL CDBAbookView::syzvol_kmkchk( CString& dbt, CString& cre )
{
	if( pDBzm->szvol->SVkcod1 == dbt || pDBzm->szvol->SVkcod1 == cre )
		return TRUE;

	if( pDBzm->szvol->SVkcod2 == dbt || pDBzm->szvol->SVkcod2 == cre )
		return TRUE;

	return FALSE;
}


// 出納帳 ページレコードにセット
void CDBAbookView::acline_set( ACBOOK_LINE* aline, int idx, int abpos )
{
	CDBINPDataRec* rec;

	(aline + idx)->abs_pos = abpos;
	rec = &(aline + idx)->drec;

	pDBzm->DBDATA_SetPosition( pDBzm->data->seq );

	pDBzm->GetCDBData( rec );

	// 消費税 属性取得
	DB_SyIniz( rec );
#ifdef SYZDSP_CLOSE
	sprintf_s( (aline + idx)->syzstr, sizeof (aline + idx)->syzstr, "%s", sy_chk_txt( rec, NULL, acINP_mode) );
#else
	sprintf_s( (aline + idx)->syzstr, sizeof (aline + idx)->syzstr, "%s", sy_line_txt( rec ) );
#endif
	if( pDBsy->IsTaikaData( pDBzm->dbdata )==2 )
			(aline + idx)->ac_sign |=  0x01;
	else	(aline + idx)->ac_sign &= ~0x01;

	// BCD 日付をセット
	pDBzm->DateConv( (aline + idx)->bcddate, rec, CV2_1 );

	// 入力サインセット
	int dc_sw;
	dc_sw = GetRecordDCsgn( rec );

	if( dc_sw == -1 || dc_sw == 0 ) {
		// 入金
		(aline + idx)->lcnd[AC_NKIN_PN].INP_sg = TRUE; 
		(aline + idx)->lcnd[AC_SKIN_PN].INP_sg = FALSE;
		dc_sw = 0;

		(aline + idx)->lcnd[ AC_KMK_PN ].INP_type	=	brn_dfltinptype( rec->m_cbr );
	}
	else {
		// 出金	
		(aline + idx)->lcnd[AC_NKIN_PN].INP_sg = FALSE;
		(aline + idx)->lcnd[AC_SKIN_PN].INP_sg = TRUE;

		(aline + idx)->lcnd[ AC_KMK_PN ].INP_type	=	brn_dfltinptype( rec->m_dbr );
	}

	// 税額
	if( is_syohizeisiwake( rec ) )
			(aline + idx)->lcnd[AC_ZEI_PN].INP_sg = TRUE;
	else	(aline + idx)->lcnd[AC_ZEI_PN].INP_sg = FALSE;

	(aline + idx)->lcnd[AC_SEQ_PN].INP_sg	= TRUE;
	(aline + idx)->lcnd[AC_DATE_PN].INP_sg	= TRUE;

	BOOL bInpSgn = FALSE;
	if( ! dc_sw ) {
		if( rec->m_cbmn != -1 ) bInpSgn = TRUE;
	}
	else {
		if( rec->m_dbmn != -1 ) bInpSgn = TRUE;
	}
	(aline + idx)->lcnd[AC_BMON_PN].INP_sg	= bInpSgn;

	bInpSgn = FALSE;
	if( pDBzm->zvol->apno == 0x20 ) {
		if( ! dc_sw ) {
			if( ! rec->m_dkno.IsEmpty() ) bInpSgn = TRUE;
		}
		else {
			if( ! rec->m_ckno.IsEmpty() ) bInpSgn = TRUE;
		}
	}
	(aline + idx)->lcnd[AC_KOJI_PN].INP_sg	= bInpSgn;

	(aline + idx)->lcnd[AC_TKY_PN].INP_sg	= TRUE;
	// 06.12 /07
	if( (aline + idx)->drec.m_imgsq > 0 )
		(aline + idx)->lcnd[ AC_TKY_PN ].IMG_sg = 1;
	else
		(aline + idx)->lcnd[ AC_TKY_PN ].IMG_sg = 0;	//02.08 /14

	(aline + idx)->lcnd[AC_SYZ_PN].INP_sg	= TRUE;

	(aline + idx)->lcnd[AC_SZAN_PN].INP_sg	= TRUE;

	// 03.11 /09
	if( pDBzm->data->hjiyu ){
		if( pDBzm->zvol->apno < 0x50 ){
			pDBzm->get_hjiyustr( pDBzm->data->hjiyu, (aline + idx)->lcnd[AC_HJIYU_PN].Etcmsg );
		}
		else{
			if( IsSyafuKaisei( pDBzm->zvol ) ) {
				m_Seldata.GetNaibuString( pDBzm->data->hjiyu, rec->m_nbcd, (aline + idx)->lcnd[AC_HJIYU_PN].Etcmsg );
			}
			else {
				m_Seldata.GetNPfuriStr( pDBzm->data->hjiyu, (aline + idx)->lcnd[AC_HJIYU_PN].Etcmsg );
			}
		}
	}
}


// 出納帳 ページレコードに『前期より繰越残高』セット
void CDBAbookView::acline_zanset(ACBOOK_LINE* aline, CACSzan* aczan)
{
	memcpy(aline->szan, aczan->ac_szan, sizeof aline->szan);
	aline->abs_pos = -1;
	aline->lcnd[AC_SZAN_PN].INP_sg = TRUE;
	aline->lattr = 1;
}


//--------------------------------------------------
// 最上段で上を押された場合のデータ移動
//	return 0 データ移動しない
//
//	例外(CErrBlk) throw します
//--------------------------------------------------
int CDBAbookView::acline_scrolup()
{
	int set_abs = 0;
	int i, idx, last = 0;

	if (m_ALine[0].abs_pos <= 0 && m_ALine[0].lattr == 1) {
		return 0;
	}
	else {
		if (m_ALine[0].abs_pos <= 0) {
			if (m_ALine[1].abs_pos <= 0) {
				set_abs = pDBzm->data->GetRecordCount();
				last = 1;
				if (set_abs <= 0)
					return 0;
			}
			else
				set_abs = m_ALine[1].abs_pos;
		}
		else {
			set_abs = m_ALine[0].abs_pos;
		}
	}

	pDBzm->data->SetAbsolutePosition(set_abs);

	if( pDBzm->data->st == -1 ) {
		ermset( 0, "データポジションを取得できません！" );
		return 0;
	}
	if( !last )	pDBzm->data->MovePrev();
	int zen_zan = 0;

	if (pDBzm->data->st == -1) {
		if (l_test(m_SZan[0].ac_szan) != 0) {
			zen_zan = 1;
		}
		else {
			return 0;
		}
	}

	// データを一つずつずらす
	for( i = ACREC_MAX-1; i > 0; i-- ) {
		m_ALine[i] = m_ALine[i-1];
	}

	int lcnt = GetACListCount();

	int init_cnt = (ACREC_MAX - lcnt);
	if( init_cnt > 0 ) {
		acline_init( &m_ALine[lcnt], init_cnt );
	}

	if (zen_zan) {
		acline_zanset(&m_ALine[0], &m_SZan[0]);
	}
	else {
		if (!last)	set_abs--;
		acline_set(&m_ALine[0], 0, set_abs);
		idx = set_abs;
		// 差引残高を セット
		memcpy(m_ALine[0].szan, m_SZan[idx].ac_szan, sizeof m_SZan[idx].ac_szan);
	}

	return 1;
}


//-------------------------------------------------------------------
// 最下段で下を押された場合のデータ移動
//	return 0 データ移動しない
//	return 1 データ移動した。
//	return 2 データ移動したが、最下行が空行(確定マスターのため)
//
//	例外(CErrBlk) throw します
//-------------------------------------------------------------------
int CDBAbookView::acline_scroldown()
{
	int set_abs = 0;
	int i;

	int lcnt = GetACListCount(); /*m_List.GetDispDataCount();*/

	if( m_ALine[lcnt-1].abs_pos <= 0 ) {
		if( m_InsLine == 0 )	return 0;
		else {
			if( m_ALine[lcnt-2].abs_pos <= 0 )
				return 0;
			else
				set_abs = m_ALine[lcnt-2].abs_pos;
		}
	}
	else {
		set_abs = m_ALine[lcnt-1].abs_pos;
	}

	pDBzm->data->SetAbsolutePosition(set_abs);

	if( pDBzm->data->st == -1 ) {
		ermset( 0, "データポジションを取得できません！" );
		return 0;
	}

	pDBzm->data->MoveNext();
	// データを一つずつずらす
	for( i = 0; i < ACREC_MAX-1; i++ ) {
		m_ALine[i] = m_ALine[i+1];
		if( i >= lcnt ) {
			acline_init( &m_ALine[i], 1 );
		}
	}
	
	if( pDBzm->data->st == -1 ) {
		// すでに 新規ラインを作成ずみ
		if( m_ALine[lcnt-2].abs_pos == 0 )
			return 0;

		if( bCONFIRM_MASTER ) {
			acline_init( &m_ALine[lcnt-1], 1 );
			return 2;
		}

		// 新規ラインを追加作成
		CDBINPDataRec* smp;
		smp = &m_ALine[lcnt-2].drec;

		acline_init( &m_ALine[lcnt-1], 1 );

		m_ALine[lcnt-1].drec.m_seq = pDBzm->m_lastdata.m_seq;

		pDBzm->GetInpDate( &m_ALine[lcnt-1].drec );
		pDBzm->GetInpDate( m_ALine[lcnt-1].bcddate );

		m_ALine[lcnt-1].abs_pos = 0;
		m_ALine[lcnt-1].dtype = 0;

		pDBzm->data->SetAbsolutePosition(m_ALine[0].abs_pos);

		return 1;
	}
	else {
		set_abs++;
		acline_set( &m_ALine[0], lcnt-1, set_abs );
	}

	char work[6];
	memcpy( work, m_ALine[lcnt-2].szan, sizeof work );

	// 差引残高を 計算
	int dcsw = GetRecordDCsgn( &m_ALine[lcnt-1].drec );

#ifndef SZAN_CLOSE	// 11.05 /12
	if( dcsw != -1 && IsSzanCalq( &m_ALine[lcnt-1].drec ) ) {
#else
	if( dcsw != -1 ) {
#endif
		if( dcsw == 0 ) {
			l_add( work, work, m_ALine[lcnt-1].drec.m_val );
			if( sotozei_data( &m_ALine[lcnt-1].drec ))
				l_add( work, work, m_ALine[lcnt-1].drec.m_zei );
		}
		else {
			l_sub( work, work, m_ALine[lcnt-1].drec.m_val );
			if( sotozei_data( &m_ALine[lcnt-1].drec ))
				l_sub( work, work, m_ALine[lcnt-1].drec.m_zei );
		}
	}
	memcpy( m_ALine[lcnt-1].szan, work, sizeof work );
	pDBzm->data->SetAbsolutePosition(m_ALine[0].abs_pos);

	return 1;

}



//--------------------------------------------------
// PageUp が押された場合のページ移動
//	return 0 ページ移動しない
//
//	例外(CErrBlk) throw します
//--------------------------------------------------
int CDBAbookView::acline_pageup()
{
	// 挿入時は 移動しない
	if( m_InsLine != 0 )
		return 0;

	// データの変更があるか？
	int append = 0;
	int i, ln, lcnt, pos, reccnt;
	ln = get_nowln();

	if( ! PageMoveDataUpdate( ln ) ) {
		return 0;
	}

	lcnt = GetACListCount(); /*m_List.GetDispDataCount();*/

	pos = m_ALine[0].abs_pos;

	int top_zan = 0;
	if (pos < 0 && m_ALine[0].lattr == 1)
		return 0;

	if (l_test(m_SZan[0].ac_szan) != 0) {
		top_zan = 1;
	}

	if( pos == 1 ) {	//レコードセットの先頭
		if( ! top_zan )	return 0;
	}
	else if( pos > 1 ) {
		for( i = 0; i < lcnt; i++ ) {
			pos--;
			if( pos == 1 )	break;
		}
		// 先頭行にセット
		if( i < lcnt )
			ln = 1;
	}
	else {
		reccnt = pDBzm->data->GetRecordCount();
		if( reccnt > 0  ) { 
			pos = reccnt;
			pDBzm->data->MoveLast();

			for( i = 1; i < lcnt; i++ ) {
				pos--;
				if( pos == 1 )	break;
			}
			// 読み出しデータをセット
			pDBzm->data->SetAbsolutePosition( pos );
			// 先頭行にセット
			if( i < lcnt )
				ln = 1;
		}
		else  {
			return 0;
		}
	}

TRACE( "ac pgup %d, %d\n", pos, m_ALine[0].abs_pos );

	pDBzm->data->SetAbsolutePosition(pos);

	if( pDBzm->data->st == -1 ) {
		ermset( 0, "データポジションを取得できません！" );
		return 0;
	}

	//先頭データでない場合は、前期からの繰越 表示しない
	if (pos != 1)	top_zan = 0;

	// ページデータセット
	acline_init( m_ALine, ACREC_MAX );
	for( i = 0; i < lcnt; i++ ) {
		if (i == 0 && top_zan) {
			acline_zanset(&m_ALine[0], &m_SZan[0]);
			continue;
		}

		acline_set( m_ALine, i, pos );

		memcpy( m_ALine[i].szan, m_SZan[pos].ac_szan, sizeof m_SZan[pos].ac_szan );
		pos++;

		pDBzm->data->MoveNext();
		if( pDBzm->data->st == -1 ) {
			if( (i+1)+1 <= lcnt ) {
				append = (i+1)+1;	// 追加行
			}
			break;
		}
	}

	// 新規行 を追加
	if( append && ! bCONFIRM_MASTER ) {
		i = (append-1);
		m_ALine[i].abs_pos = 0;
		int newseq = pDBzm->m_lastdata.m_seq;

		pDBzm->GetInpDate( &m_ALine[i].drec );
		pDBzm->GetInpDate( m_ALine[i].bcddate );
		m_ALine[i].drec.m_seq = newseq;

	}

	return 1;
}


//--------------------------------------------------
// PageDown が押された場合のページ移動
//	return 0 ページ移動しない
//
//	例外(CErrBlk) throw します
//--------------------------------------------------
int CDBAbookView::acline_pagedown()
{
	// 挿入時は 移動しない
	if( m_InsLine != 0 )
		return 0;

	int pos, append;
	int i, ln;

	ln = get_nowln();

	if( ! PageMoveDataUpdate( ln ) ) {
		return 0;
	}

	int lcnt = GetACListCount(); /*m_List.GetDispDataCount();*/

	pos = m_ALine[lcnt-1].abs_pos;
	if( pos <= 0 )	// 新規入力行
		return 0;

	pDBzm->data->SetAbsolutePosition( pos );
	if( pDBzm->data->st == -1 ) {
		ermset( 0, "データポジションを取得できません！" );
		return -1;
	}
	// 次ページ開始
	pos++;
	pDBzm->data->SetAbsolutePosition( pos );

	if( pDBzm->data->st == -1 ) {
		pos = 0;
	}

	acline_init( m_ALine, ACREC_MAX );
	if( pos > 0 ) {
		// ページデータセット
		append = 0;
		for( i = 0; i < lcnt; i++ ) {
			acline_set( m_ALine, i, pos );

			memcpy( m_ALine[i].szan, m_SZan[pos].ac_szan, sizeof m_SZan[pos].ac_szan );
			pos++;

			pDBzm->data->MoveNext();
			if( pDBzm->data->st == -1 ) {
				if( (i+2) <= lcnt ) {	// 追加行 が表示行数内
					append = (i+2);
				}
				break;
			}
		}
	}
	else {
		append = 1;	// 先頭が新規行
	}
	
	// 新規行 を追加
	if( append && ! bCONFIRM_MASTER ) {
		i = (append-1);
		m_ALine[i].abs_pos = 0;
		int newseq;
		newseq = pDBzm->m_lastdata.m_seq;
		
		pDBzm->GetInpDate( &m_ALine[i].drec );
		pDBzm->GetInpDate( m_ALine[i].bcddate );
		m_ALine[i].drec.m_seq = newseq;
	}

	return 1;

}


//--------------------------------------------------
// ページ移動の前に行う 現行のデータチェック
//	return	TRUE --- check OK
//			FALSE -- ページ移動しない
//
//--------------------------------------------------
BOOL CDBAbookView::PageMoveDataUpdate(int nowln )
{
	// 現在の行のKillFocus 呼び出す(データの取得＆バッファにセット)
	m_List.SetFocus();
	
	if( m_ALine[nowln-1].abs_pos > 0 ) {
		if( IsAbookDiff_Data( &m_CREC, &m_SCAN_rec ) ) {

			if( CheckModifyBmonData( &m_CREC, -1 ) != 0 ) {
				_set_focus( nowln, AC_BMON_PN );
				return FALSE;
			}

			if( CheckModifyEdabanData( &m_CREC, -1 ) != 0 ) {
				_set_focus( nowln, AC_KMK_PN );
				return FALSE;
			}

			MODEFY_BIT.onbitn( m_CREC.m_seq - 1);
			if( isSyzdiff_data( &m_CREC, &m_SCAN_rec ) ) {
				SYZMDFY_BIT.onbitn( m_CREC.m_seq - 1);
			}

			pDBzm->DB_DataCorrect( &m_CREC );
			if( memcmp( m_CREC.m_val, m_SCAN_rec.m_val, sizeof m_CREC.m_val ) ||
				memcmp( &m_CREC.m_dsign[0], &m_SCAN_rec.m_dsign[0], SZ_DSGN ) ||
				m_CREC.m_dbt != m_SCAN_rec.m_dbt ) {
				UpdateACZanRec(m_ALine[nowln-1].abs_pos, &m_CREC );
				SzanUpdate( nowln, &m_CREC, &m_SCAN_rec );
			}

			//諸口・現金貸借
			if( m_CREC.m_dsign[0] & 0x01 ){
				syog_genk( &m_CREC, _SUB );
			}
			else{
				syog_genk( &m_CREC, _ADD );
			}
		}
	}
	else if( m_ALine[nowln-1].abs_pos == 0 ) {
		if( AppendDataCheck(nowln) ) {
			PROGRAM_OK = FALSE;
			
			int st = myICSMessageBox( swk_kansei, MB_YESNO|MB_DEFBUTTON2 );
		
			PROGRAM_OK = TRUE;

			if( st == IDYES ) {
				_set_focus( nowln, AC_DATE_PN );
				return FALSE;
			}
		}
	}

	m_NOBRNCHK = 0;
	m_ACKOJICHK	 = 0;

	return TRUE;
}


//ポジションによる表示切替
void CDBAbookView::disp_change()
{
BOOL sg;
int pn = get_nowpn();

	sg = TRUE;
	// 選択画面の表示色
	if( ! fixvalcnd() )
	{
		enable_fixval( sg );
	}

	// 固定科目選択のディセーブル／イネーブル
	switch( pn ) {
	case AC_BMON_PN:	case AC_KOJI_PN :
	case AC_SYZ_PN:		case AC_TKY_PN :
	case AC_HJIYU_PN:
	case AC_SELBMON_PN:
	case AC_SELKOJI_PN:
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

	AutoSelIndicator( acINP_mode );
}

//ポジションによる表示切替（強制表示）
void CDBAbookView::_disp_change()
{
BOOL sg;
int pn = get_nowpn();

	sg = TRUE;

	// 固定科目選択のディセーブル／イネーブル
	switch( pn ) {
	case AC_BMON_PN:	case AC_KOJI_PN :
	case AC_SYZ_PN:		case AC_TKY_PN :
	case AC_HJIYU_PN:
	case AC_SELBMON_PN:
	case AC_SELKOJI_PN:
		sg = FALSE;	
		break;
	default:
		sg = TRUE;
		break;
	}

	fix8disp( sg );
}

//------------------------------------
//	選択画面を初期表示にする
//
//------------------------------------
void CDBAbookView::SelectReset()
{
	int pn = get_nowpn();
	BOOL bReset = FALSE;

	switch( pn ) {
	case AC_SEQ_PN:
	case AC_DATE_PN:
	case AC_DENP_PN:
	case AC_DOCEVI_PN:
	case AC_NKIN_PN:
	case AC_SKIN_PN:
	case AC_SELDISP_PN:
		if( m_SelDispPN != SL_KAMOKU_PN )
			bReset = TRUE;
		break;
	}

	if( bReset ) {
		SelPar par;
		par.sel_pn = SL_KAMOKU_PN;
		m_Seldata.SelectJob(&par, -1);
	}
}


void CDBAbookView::set_nowpn( int pn, int reload/*=0*/ )
{
	int prev_pn;

	prev_pn = m_NowPN;
	m_NowPN = pn;

	if( pn >= AC_SELBMON_PN && pn <= AC_SELDISP_PN ) {
		// 出納帳入力部分から、基本項目に移動
		if( prev_pn < AC_SELBMON_PN ) {
			if( ! reload ) {
				int ret = UpdateModfyData(1);
				if( ret == -2 ) {
					myICSMessageBox( (LPCTSTR)pDBzm->GetZmErrMsg() );
				}
			}

			acline_init( m_ALine, ACREC_MAX );
			m_List.RemoveAllDtListData();
			m_List.RedrawDataListCtrl();
			set_nowln(-1);
			acINP_mode = _RETREV;

			AutoSelIndicator( acINP_mode );

			Destroy_ModeLessDlg();
		}
	}

	disp_change();
	SelectReset();
}

int CDBAbookView::get_nowpn()
{
	return m_NowPN;
}

void CDBAbookView::set_nowln( int ln )
{
	m_NowLine = ln;
}

int CDBAbookView::get_nowln()
{
	return m_NowLine;
}


ACBOOK_LINE* CDBAbookView::getLINE_DATA()
{
	int ln = get_nowln();
ASSERT( ln != -1 );
//MyTrace( "getLINE_DATA ln = %d\n", ln );

	return &m_ALine[ln-1];
}


ACBOOK_LINE* CDBAbookView::_getLINE_DATA( int ln )
{
	return &m_ALine[ln-1];
}


// モードレス ダイアログを消去
void CDBAbookView::Destroy_ModeLessDlg()
{
	// 消費税ダイアログ消去
	Hide_SyzInp();
	// 付箋ダイアログ消去
	Destroy_HusenInp();
	// 変動事由入力消去
	Destroy_HjiyuInp();
	// サブ入力消去
	Destroy_SubDataInp();
}


BOOL CDBAbookView::isdeldata()
{
	int nowln;
	nowln = get_nowln();

	return _isdeldata( nowln );
}

BOOL CDBAbookView::_isdeldata( int ln )
{
	if( ln < 0 ) return FALSE;

	if( m_ALine[ln-1].abs_pos > 0 ) {
		if( m_ALine[ln-1].drec.m_dsign[0] & 0x01 )
			return TRUE;
	}

	return FALSE;
}


#if (ACBFOCUS_DEBUG == 1)
void CDBAbookView::set_focusTrace(int pn, int iClick/*=0*/, int line/*=0*/, char* func/*=0*/, char* fname/*=0*/)
{
	FILETrace("====> set_focus pn = %d, iClick = %d, L:%d, Func(= %s), File = %s\n", pn, iClick, line, func, fname);
	_set_focusTrace(get_nowln(), pn, iClick, line, func, fname);
#else
void CDBAbookView::set_focus( int pn, int iClick/*=0*/ )
{
	_set_focus( get_nowln(), pn );
#endif
}

#if (ACBFOCUS_DEBUG == 1)
void CDBAbookView::_set_focusTrace(int ln, int pn, int iClick/* = 0*/, int line/*=0*/, char* func/*=0*/, char* fname/*=0*/)
{
	FILETrace("====> _set_focus pn = %d, iClick = %d, L:%d, Func(= %s), File = %s\n", pn, iClick, line, func, fname);
#else
void CDBAbookView::_set_focus( int ln, int pn, int iClick/*=0*/  )
{
#endif
	if( pn >= AC_SELBMON_PN ) {
		PostMessage( WM_RETROFOCUS, -1, MAKELONG( pn, 0) );
	}
	else {
		if( pn < 0 ) return;

		int id = get_dctrl_id( pn );
		PostMessage( WM_RETROFOCUS, ln, MAKELONG( id, iClick) );
	}
}


LRESULT CDBAbookView::OnRetroFocus( WPARAM wParam, LPARAM lParam )
{
	WORD low;
	low = LOWORD( lParam );

	// リストにフォーカスをセットして、DBEDTのフォーカスOFFを発生させる。
	// そのフォーカスOFFでデータを取得する
	m_List.SetFocus();

	if( low >= AC_SELBMON_PN ) {
		switch( low ) {
		case AC_SELBMON_PN:		c_set( IDC_ICSDBEDTCTRL2 );	break;
		case AC_SELKOJI_PN:		c_set( IDC_ICSDBEDTCTRL3 );	break;
		case AC_SELKAMOK_PN:	c_set( IDC_ICSDBEDTCTRL4 );	break;
		case AC_SELBRN_PN:		c_set( IDC_ICSDBEDTCTRL5 );	break;
		case AC_SELDISP_PN:		c_set( IDC_BUTTON_DISP );	break;
		default:
			ASSERT( FALSE );
			break;
		}
	}
	else {
	//	m_List.SetWindowPos( &m_Input, 0,0,0,0,SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOSIZE );
		m_List.SetPosition( wParam, LOWORD( lParam ), HIWORD( lParam ) );
	}

	return 1;
}


// データコントロール ID を取得
int CDBAbookView::get_dctrl_id( int pn )
{
	int did = -1;

	switch( pn ) {
	case AC_SEQ_PN:		did = DATA_SEQ;		break;
	case AC_DATE_PN:	did = DATA_DATE;	break;
	case AC_DENP_PN:	did = DATA_DENP;	break;
	case AC_DOCEVI_PN:	did = DATA_DOCEVI;	break;
	case AC_BMON_PN:	did = DATA_DEB_BMON;	break;
	case AC_KOJI_PN:	did = DATA_DEB_KNO;	break;
	case AC_KMK_PN:		did = DATA_DEBT;	break;
	case AC_TKY_PN:		did = DATA_TEKIYO;	break;
	case AC_HJIYU_PN:	did = DATA_HJIYU;	break;
	case AC_NKIN_PN:	did = DATA_VAL;		break;
	case AC_SKIN_PN:	did = DATA_USER1;	break;
	case AC_ZEI_PN:		did = DATA_ZEI;		break;
	case AC_SYZ_PN:		did = DATA_SYZ;		break;
	case AC_SZAN_PN:	did = DATA_USER2;	break;
	default:
		ASSERT( FALSE );
		break;
	}

	return did;
}


// データコントロール ID を取得
int CDBAbookView::dctrlid_toacpn( int id )
{
	int pn = -1;

	switch( id ) {
	case DATA_SEQ:		pn = AC_SEQ_PN;		break;
	case DATA_DATE:		pn = AC_DATE_PN;	break;
	case DATA_DENP:		pn = AC_DENP_PN;	break;
	case DATA_DOCEVI:	pn = AC_DOCEVI_PN;	break;
	case DATA_DEB_BMON:	pn = AC_BMON_PN;	break;
	case DATA_DEB_KNO:	pn = AC_KOJI_PN;	break;
	case DATA_DEBT:		pn = AC_KMK_PN;		break;
	case DATA_TEKIYO:	pn = AC_TKY_PN;		break;
	case DATA_HJIYU:	pn = AC_HJIYU_PN;	break;
	case DATA_VAL:		pn = AC_NKIN_PN;	break;
	case DATA_USER1:	pn = AC_SKIN_PN;	break;
	case DATA_ZEI:		pn = AC_ZEI_PN;		break;
	case DATA_SYZ:		pn = AC_SYZ_PN;		break;
	case DATA_USER2:	pn = AC_SZAN_PN;	break;
	default:
		ASSERT( FALSE );
		break;;
	}

	return pn;
}

//--------------------------------------------------------------------
// ターミネーション時のフォーカス処理
//	int ln ........ 現在ライン
//	int acpn ...... 現在カーソルポジション
//	int vect_n .... 1 = 次項目, -1 = 全項目
//
//--------------------------------------------------------------------
void CDBAbookView::set_termfocus( int ln, int acpn, int vect_n )
{
	DT_RECT* pdt;
	CDataListCtrl* pCtrl;

	pdt = m_List.GetDtRect();
	pCtrl = &m_List;

	if( pdt == NULL )
		return;

	int pos, nxt, pn, dcsw, ns_flg;
	pn = get_dctrl_id( acpn );

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

	ACBOOK_LINE* al;
	al = _getLINE_DATA( ln );

	if( pos != -1 ) {
		switch( pos ) {

		case DATA_ZEI:
			if( vect_n > 0 ) {	// 税額へは、ターミネーションで行かせない
				pn = DATA_ZEI;
				goto LOOP;
			}
			break;
		// 入金
		case DATA_VAL:
			if( acINP_mode == _APPEND ) {
//MyTrace("set_termfocus {_APPEND} DATA_VAL nkin(%d), skin(%d)\n", al->lcnd[AC_NKIN_PN].INP_sg, al->lcnd[AC_SKIN_PN].INP_sg );

				// まだ金額未入力
				if( ! al->lcnd[AC_NKIN_PN].INP_sg && ! al->lcnd[AC_SKIN_PN].INP_sg ) {
					dcsw = GetRecordDCsgn( &al->drec );

					if( dcsw != -1 ) {
						if( m_bAutoSwk ) {	//自動仕訳の設定による移動
						//	ns_flg = (dcsw != 0) ? 0 : 1;

							if( (m_dwOpt&0xf0) == 0 ) {	// 固定なし
								ns_flg = (dcsw != 0) ? 0 : 1;
							}
							else if( (m_dwOpt&0xf0) == 0x10 ) {
								ns_flg = 1;				// 入金固定
							}
							else {
								ns_flg = 0;				// 出金固定
							}

							if( ! ns_flg ) {
								pos = DATA_USER1;
								break;
							}
						}
						else {
							ns_flg = GetKcodeDCsgn( (dcsw == 0) ? al->drec.m_cre : al->drec.m_dbt );
						}

						// 出金科目
						if( ! ns_flg ) {
							pn = DATA_VAL;
							goto LOOP;
						}
					}
				}
				else if( ! al->lcnd[AC_NKIN_PN].INP_sg ) {
					pn = DATA_VAL;
					goto LOOP;
				}
			}
			else {
				// 入金ではなく 出金
				if( ! al->lcnd[AC_NKIN_PN].INP_sg ) {
					pn = DATA_VAL;
					goto LOOP;
				}
			}
//MyTrace("set_termfocus {_APPEND} DATA_VAL case END pn = %d", pn );
			break;
		// 出金
		case DATA_USER1:
			if( acINP_mode == _APPEND ) {
				if( ! al->lcnd[AC_NKIN_PN].INP_sg && ! al->lcnd[AC_SKIN_PN].INP_sg ) {
					dcsw = GetRecordDCsgn( &al->drec );

					if( dcsw != -1 ) {
						if( m_bAutoSwk ) {	//自動仕訳の設定による移動
						//	ns_flg = (dcsw != 0) ? 0 : 1;

							if( (m_dwOpt&0xf0) == 0 ) {	// 固定なし
								ns_flg = (dcsw != 0) ? 0 : 1;
							}
							else if( (m_dwOpt&0xf0) == 0x10 ) {
								ns_flg = 1;				// 入金固定
							}
							else {
								ns_flg = 0;				// 出金固定
							}

							if( ! ns_flg ) {
								pos = DATA_USER1;
							}
							else {
								pos = DATA_VAL;
							}
							break;
						}

						ns_flg = GetKcodeDCsgn( (dcsw == 0) ? al->drec.m_cre : al->drec.m_dbt );
						// 入金科目
						if( ns_flg ) {
						//	pn = DATA_USER1; //---> Enterで入金・出金移動
						//	goto LOOP;
						}
					}
				}
				else if( ! al->lcnd[AC_SKIN_PN].INP_sg ) {
					pn = DATA_USER1;
					goto LOOP;
				}
			}
			else {
				// 入金ではなく 出金
				if( ! al->lcnd[AC_SKIN_PN].INP_sg ) {
					pn = DATA_USER1;
					goto LOOP;
				}
			}
			break;

		case DATA_SYZ:
			if (pAUTOSEL->DSPSYZ_OPT == OPT_NON) {
				pn = pos;
				goto LOOP;
			}
			break;

		case DATA_HJIYU:
			if( ! IsHjiyuOrNPfuri( getLINE_DATA()->drec.m_dbt, getLINE_DATA()->drec.m_cre ) ) {
				pn = pos;
				goto LOOP;
			}
			break;

//		case SYON_PN:	// 手形科目でない仕訳は、FOCUS なし
//		case TGDT_PN:
//			if( ! IsBillData(getLINE_DATA()->drec.m_dbt, getLINE_DATA()->drec.m_cre ) ) {
//				pn = pos;
//				goto LOOP;			
//			}
//			break;

		case DATA_DENP:
			if(pAUTOSEL->DENP_OPT == OPT_NON) {
				pn = pos;
				goto LOOP;
			}
			break;

		default:
			break;
		}

		pn = dctrlid_toacpn( pos );
		_set_focus( ln, pn );

//MyTrace("set_termfocus END pos = %d, pn = %d\n", pos, pn );
	}
}

//----------------------------------------------------------------
//	データ行の移動
//	int ln ---- 1 ～ は 移動する行番号(画面先頭行を １とする)
//				画面データをスクロールする場合は 
//				ACD_SCROLLUP
//				ACD_SCROLLDOWN
//				ACD_PAGEUP
//				ACD_PAGEDOWN
//
//	int pn ---- 項目ポジション
//	int focs_sw	フォーカスサイン
//				マウスクリックでの行移動は、"0", キーの時は "1"
//
//	返送値	-1 ... 正常処理ができなかった
//			 0 ... 正常終了(行の変更の有無は関係なく)
//			 1 ... 正常終了( 新規入力行の修正に戻す )
//		エラーの場合 例外(CErrBlk)をthrow します
//----------------------------------------------------------------
int CDBAbookView::_dline_chg( int ln, int pn, int focs_sw/* = 0 */ )
{
	int i, pos, nowln, linemax, st;
	BOOL bSzanUpdate = FALSE;
	BOOL bRequery = FALSE;
	BOOL bCorrected = FALSE;
	BOOL bSyzModify = FALSE;

	linemax = GetACListCount(); /*m_List.GetDispDataCount();*/
	nowln = get_nowln();

//MyTrace( "++Abook++ _dline_chg ln = %d, nowln = %d, fcsw %d\n", ln, nowln, focs_sw );

	if( nowln != -1 ) {
		if( nowln == ln )	return 0;
		else {
			// 10.25 /12 ------------------------------------------
			if( ln == ACD_PAGEDOWN ){ 
				if( m_ALine[linemax-1].abs_pos <= 0 ) {
					_set_focus( nowln, get_nowpn() );
					return 0;
				}
			}
			// 10.25 /12 ------------------------------------------

			// 現在の行のKillFocus 呼び出す(データの取得＆バッファにセット)
			if( focs_sw ) m_List.SetFocus();

			if( m_ALine[nowln-1].abs_pos > 0 ) {
				if( IsAbookDiff_Data( &m_CREC, &m_SCAN_rec )  ) {
					CDBINPDataRec chkRec;
					chkRec = m_CREC;

					// 移動した
					if( CheckModifyBmonData( &m_CREC, nowln ) != 0 ) {
						_set_focus( nowln, AC_BMON_PN );
						return -1;
					}
					// 03.16 /11 追加
					if( CheckModifyKojiKmkData( &m_CREC, nowln ) != 0 ) {
						_set_focus( nowln, AC_KOJI_PN );
						return -1;
					}
					// 05.01 /12
					if( CheckModifyEdabanData( &m_CREC, nowln ) != 0 ) {
						_set_focus( nowln, AC_KMK_PN );
						return FALSE;
					}

					int ret = pDBzm->DB_DataCorrect( &m_CREC );
					if( ret == -2 ) {
						PROGRAM_OK = FALSE;
						
						int st = myICSMessageBox( (LPCTSTR)pDBzm->GetZmErrMsg() );

						pDBzm->GetCDBData( &m_CREC );
						m_SCAN_rec = m_CREC;
						DataLineUpdate( &m_CREC, nowln );
					
						PROGRAM_OK = TRUE;
					}
					else {
					//	if( CheckModifyBmonData( &m_CREC, nowln ) != 0 ) {
					//		_set_focus( nowln, AC_BMON_PN );
					//		return 0;
					//	}
						if( isdiff_data( &m_CREC, &m_SCAN_rec ) ) {	//部門チェック等で、変更がなくなる場合がある。
							MODEFY_BIT.onbitn( m_CREC.m_seq - 1);
							if( isSyzdiff_data( &m_CREC, &m_SCAN_rec ) ) {
								SYZMDFY_BIT.onbitn( m_CREC.m_seq - 1);
								bSyzModify = TRUE;
							}
						}
						else if( isdiff_data( &m_CREC, &chkRec ) ) {
							//部門チェック等で、変更がなくなった。
							if( m_CREC.m_dbmn != chkRec.m_dbmn || m_CREC.m_cbmn != chkRec.m_cbmn ) {
								set_etcdsp( nowln, AC_BMON_PN );
							}
							if( m_CREC.m_dkno.CompareNoCase( chkRec.m_dkno ) != 0 || m_CREC.m_ckno.CompareNoCase( chkRec.m_ckno ) != 0 ) {
								set_etcdsp( nowln, AC_KOJI_PN );
							}
							DataLineUpdate( &m_CREC, nowln );
						}
					}

					bCorrected = TRUE;
					// 差引残高を更新するか？
					if( memcmp( m_CREC.m_val, m_SCAN_rec.m_val, sizeof m_CREC.m_val ) ||
						memcmp( m_CREC.m_zei, m_SCAN_rec.m_zei, sizeof m_CREC.m_zei ) ||
						memcmp( &m_CREC.m_dsign[0], &m_SCAN_rec.m_dsign[0], SZ_DSGN ) ||
						m_CREC.m_dbt != m_SCAN_rec.m_dbt ) {
						UpdateACZanRec(m_ALine[nowln-1].abs_pos, &m_CREC );
						SzanUpdate( nowln, &m_CREC, &m_SCAN_rec );
						bSzanUpdate = TRUE;
					}
					if( m_CREC.m_ddate != m_SCAN_rec.m_ddate || 
						(m_CREC.m_ddate == m_SCAN_rec.m_ddate && m_CREC.m_mofs != m_SCAN_rec.m_mofs ) ) {
						bRequery = TRUE;
					}

					//諸口・現金貸借
					if( m_CREC.m_dsign[0] & 0x01 ){
						syog_genk( &m_CREC, _SUB );
					}
					else{
						syog_genk( &m_CREC, _ADD );
					}

					// 08.29 /12
					m_ALine[nowln-1].drec = m_CREC;
					m_ALine[nowln - 1].lcnd[AC_ZEI_PN].KEY_inp = 0;
				}
			}
			else if( m_ALine[nowln-1].abs_pos == 0 ) {
				// 挿入モード時は 行移動を許さない
				if( m_InsLine > 0 ) {
					_set_focus( nowln, get_nowpn() );
					return 0;
				}

				CString strType;
				if( RemainSwkdataCheck(strType) ) {
					int	st;
					CString msg;
					msg.Format(_T("%s。\n\n処理を終わりますか？"), strType);

					st	=	myICSMessageBox( msg, MB_YESNO|MB_DEFBUTTON2 );

					if( st == IDYES ) {
						EndRemaindata();	// 定型仕訳テーブルの削除
					}
					else {
						_set_focus( nowln, pn );
						return 1;
					}
				}

			//	if( ! AppendDataCheck(nowln) ) {
			//		pDBzm->DB_DataAppend( &m_CREC );
			//		m_ALine[nowln-1].drec.m_seq = m_CREC.m_seq;

			//		SzanUpdate( nowln, m_CREC.m_val, NULL, 0 );
			//		bSzanUpdate = TRUE;
			//	}
				if( AppendDataCheck(nowln) ) {
					PROGRAM_OK = FALSE;
					
					st = myICSMessageBox( swk_kansei, MB_YESNO|MB_DEFBUTTON2 );
				
					PROGRAM_OK = TRUE;

					if( st == IDYES ) {
						_set_focus( nowln, AC_DATE_PN );
						return 1;
					}
					else {
						// カレント行の表示をクリア
						m_CREC.Reset();
						LineDataClear( nowln );
					}
				}
			}
			// 輸入仕入れに 変更サインをクリア
			m_bSyzYunyu = FALSE;
			m_bSyzSotoMen = FALSE;
			m_pnZeiSotoMen = 0;
		}
	}

	m_NOBRNCHK = 0;
	m_ACKOJICHK	 = 0;

//MyTrace("L%d:abook::_dline_chg  ln = %d\n", __LINE__, ln);
	int top_zan = 0;

	// 画面データスクロール
	if( ln < 0 ) {
		switch( ln ) {
		case ACD_SCROLLUP:
			if( acline_scrolup() ) {
				if( ! bRequery )	ListDataSet();
			}
			else {
				if( bSzanUpdate ) {	// 差引残高更新
					SzanDataSet(1);
				}
				if( bCorrected )	TerminationDataSet( AC_DATE_PN ); // 仕訳修正サイン描画
			}
			if (m_ALine[0].lattr == 1) {
				top_zan = 1;
				ln = 2;
			}
			else {
				ln = 1;
			}
			break;
		case ACD_SCROLLDOWN:
			st = acline_scroldown();
			if( st > 0 ) {
				if( ! bRequery )	ListDataSet();
			}
			// 確定マスターは、空行を表示する
			if( st == 2 ) {
				ln = linemax-1;
			}
			else {
				ln = linemax;
			}
			break;
		case ACD_PAGEUP:
			if( acline_pageup() ) {
				if( ! bRequery )	ListDataSet();
			}
			else {
				if( bSzanUpdate ) {	// 差引残高更新
					SzanDataSet(1);
				}
			}
			if (m_ALine[0].lattr == 1) {
				top_zan = 1;
				ln = 2;
			}
			else {
				ln = 1;
			}
			break;
		case ACD_PAGEDOWN:
			if( acline_pagedown() ) {
				if( ! bRequery )	ListDataSet();
			}
			else {
				return -1;
			}
			ln = 1;
			break;
		default:
			ln = 1;
			break;
		}
	}
	else  {
		if( bSzanUpdate ) {	// 差引残高更新
			SzanDataSet(nowln);
		}
		if( bCorrected ) {
			if( bSyzModify ) {
				int pntbl[2] = {AC_DATE_PN, AC_SYZ_PN};
				TerminationDataSet( pntbl, 2 ); // 仕訳修正・消費税修正サイン描画
			}
			else {
				TerminationDataSet( AC_DATE_PN ); // 仕訳修正サイン描画
			}
		}
		//一行目に移動するときで、前期より繰越の時
		if (m_ALine[0].lattr == 1 && ln == 1) {
			top_zan = 1;
			ln = 2;
		}
	}

	// 仕訳リクエリー
	if( bRequery ) {
		int nxtseq = -1;
		if (m_ALine[0].lattr == 1)
			top_zan = 1;

		if( m_ALine[ln-1].abs_pos < 0 ) {
			if (m_ALine[0].lattr == 1)	ln = 2;
			else {
				ln = 1;
			}
		}
		else {
			nxtseq = m_ALine[ln-1].drec.m_seq;
		}

		ln = RequeryACLine( nxtseq, ln, top_zan);
	}

	if( m_ALine[ln-1].abs_pos < 0 ) {	// 未入力部分
		if( focs_sw ) {
			_set_focus( nowln, pn );
		}
		return 0;
	}
	else if( m_ALine[ln-1].abs_pos == 0 ) {	//追加
		acINP_mode = _APPEND;
	}
	else {
		acINP_mode = _MODEFY;	// 修正
	}

	AutoSelIndicator( acINP_mode );

	m_CREC = m_ALine[ln-1].drec;

//MyTrace( "=== acbook dline_chg m_CREC.m_seq %d, [%02x%02x%02x]", m_CREC.m_seq, m_CREC.m_dsign[0], m_CREC.m_dsign[1], m_CREC.m_dsign[2] );
//L_PRINT( "=val", m_CREC.m_val ); L_PRINT( " =zei", m_CREC.m_zei );
//TRACE( "\n" );

	pos = m_ALine[ln-1].abs_pos;

	if( pos <= 0 ) {
		for( i = ln-1; i > 0; i-- ) {
			pos = m_ALine[i-1].abs_pos;
			if( pos > 0 )	break;
		}
	}
	else	pos--;

	if( pos <= 0 ) {
		m_SREC.Reset();
		m_SREC = m_CREC;
	}
	else {
		// この処理は、SyIniz()より後にしてはいけない。
		// 消費税処理のデータがずれてしまうから
		pDBzm->data->SetAbsolutePosition( pos );

		if( pDBzm->DBDATA_SetPosition( pDBzm->data->seq ) == 0 ) {
			if( acINP_mode != _APPEND ) {
				pDBzm->GetCDBData( &m_SREC );
			}
			else {
				//損益のみ部門で、間に貸借の仕訳を入力しても、部門番号を引き継ぐようにするため
				if( pDBzm->data->seq != m_appSREC.m_seq ) {
					pDBzm->GetCDBData( &m_SREC );
				}
				else {
					m_SREC = m_appSREC;
				}
			}
		}
		else {
			m_SREC.Reset();

			// 入力日付をセット
			pDBzm->GetInpDate( &m_SREC );
		}
	}

	DB_SyIniz( &m_CREC );

	if( m_bLineDispChg ) {	// 貸借変更で、マウスで画面を縮めた場合、ＤＢは更新していないので、サンプルレコードにセットしない。
		m_bLineDispChg = FALSE;
	}
	else {
		m_SCAN_rec = m_CREC;
	}

	if( acINP_mode == _APPEND && m_bAutoSwk )
		m_List.SetCurBkColor( RGB_WINE_GREEN );
	else
		m_List.SetCurBkColor( RGB_WINE_BLUE );

	set_nowln( ln );

	sel_lineupdate();

	int rval = 0;

	if( focs_sw ) {
		// キーによる移動時、入金・出金が異なる場合。
		if( pn == AC_NKIN_PN || pn == AC_SKIN_PN ) {
			int dcsw = GetRecordDCsgn( &getLINE_DATA()->drec );

			if( dcsw == 0 || dcsw == -1 ) {
				if( pn == AC_SKIN_PN )	pn = AC_NKIN_PN;
			}
			else {
				if( pn == AC_NKIN_PN )	pn = AC_SKIN_PN;
			}

			if( acINP_mode == _APPEND && ! m_bAutoSwk ) {
				pn = AC_DATE_PN;
				rval = VK_DOWN;
			}
		}
		//修正時の金額でのEnter・Tabは日付欄に移行するため、ここではセットしない。
		if (!m_bTermValLinChg) {
			_set_focus(ln, pn);
		}
		else {
			if (acINP_mode == _APPEND) {
		//		m_bTermValLinChg = FALSE;
				if (m_bAutoSwk)	pn = AC_TKY_PN;
				else {
					pn = AC_DATE_PN;
				}
				_set_focus(ln, pn);
			}
		}

	}
	else {
		m_List.RedrawDataListCtrl();
	}

	return rval;
}


//-----------------------------------------------------
//	キーによる行移動
//	long nChar ---- キーコード
//
//	エラーの場合 例外(CErrBlk)を throw します
//-----------------------------------------------------
int CDBAbookView::dline_chg( long nChar )
{
	int ln, linemax;
//MyTrace( "++Abook++ dline_chg TOP = %d\n", nChar );

	linemax = GetACListCount(); /*m_List.GetDispDataCount();*/

	// 追加行は キーによる移動を 許さない
	if( m_InsLine == 0 ) {
#ifdef OLD_CLOSE
		if( acINP_mode == _APPEND && get_nowpn() != AC_DATE_PN ) {
			if( nChar == VK_UP || nChar == VK_PRIOR ) {
				return 0;
			}
			if( nChar == VK_DOWN || nChar == VK_NEXT ) {
				return 0;
			}
		}
#else
		if( acINP_mode == _APPEND ) {
			if( get_nowpn() != AC_DATE_PN ) {
				if( nChar == VK_UP || nChar == VK_PRIOR ) {
					return 0;
				}
			}
			// 新規行の下方向の移動は許さない。 11.08 /12
			if( nChar == VK_DOWN || nChar == VK_NEXT ) {
				return 0;
			}
		}
#endif
	}

	switch( nChar )
	{
/*		case VK_END:	//修正の終了
			if( acINP_mode != c_KEYSCAN && get_nowln() != APPEND_LINE )
				ln = APPEND_LINE;	//入力行に戻る
			else
				return( nChar );
			break;
*/
		case VK_UP:	//１行戻る
			ln = get_nowln();
			if( ln == 1 || (ln == 2 && m_ALine[0].lattr == 1) )
				ln = ACD_SCROLLUP;
			else
				ln--;

			break;

		case VK_DOWN:	//１行進む
			ln = get_nowln() + 1;
			if( ln > linemax )	
				ln = ACD_SCROLLDOWN;
			break;

		case VK_PRIOR:
			ln = ACD_PAGEUP;
			break;
		case VK_NEXT:
			ln = ACD_PAGEDOWN;
			break;

		default:
			return( nChar );
			break;
	}

	return( _dline_chg( ln, get_nowpn(), 1 ) );

}

//-------------------------------------------
// 追加用仕訳データのチェック
//	int ln ... 追加した 行
//	return	TRUE ---- 日付以外の入力があった
//			FALSE --- 日付以外入力がない
//----------------------------------------------
int CDBAbookView::AppendDataCheck( int ln ) 
{
	ACBOOK_LINE* al;
	BOOL bRet = FALSE;

	al = _getLINE_DATA( ln );

	if( al->abs_pos > 0 )
		return TRUE;

	if( al->lcnd[AC_BMON_PN].INP_sg ||
		al->lcnd[AC_KOJI_PN].INP_sg ||
		al->lcnd[AC_KMK_PN].INP_sg ||
		al->lcnd[AC_TKY_PN].INP_sg ||
		al->lcnd[AC_NKIN_PN].INP_sg || al->lcnd[AC_SKIN_PN].INP_sg ) {
		bRet = TRUE;
	}
	else {
		bRet = FALSE;
	}

	return bRet;
}

//------------------------------------------------------
// 仕訳をクエリーする
//	int nxtseq ... 今回 選択行になる仕訳番号
//  int ln	...... セットする行(１から)
//	int top_zan .. 1=先頭行に※前期より繰越※を表示
//  
//	セットする行番号を返送する(１から)
//------------------------------------------------------
int CDBAbookView::RequeryACLine( int nxtseq, int ln, int top_zan/*=0*/ )
{
	Acbook_Requery();

	if( pDBzm->data->st == -1 ) {
		ermset( 0, "仕訳データクエリーエラー！" );
		return -1;
	}

	char szan[6] = {0}, tmp[6] = {0}, work[6] = {0};
	int dcnt = 0;
	int i, lcnt, n, abpos, dcsw;
	int reccnt = pDBzm->data->GetRecordCount();
	CACSzan aszan;
	CDBINPDataRec rec;

	lcnt = GetACListCount(); /*m_List.GetDispDataCount();*/

	// 差引残高エリアクリア
	m_SZan.RemoveAll();
	m_SZan.SetSize(reccnt + 1);//開始残高用＋１

	// 基本科目の開始残高を取得
	BaseSzanSet( &aszan );
	m_SZan[0] = aszan;

	dcnt = n = 0;
	//定型登録 ソート用
	SCMDFY_SEQ.fill();

	do {
		pDBzm->GetCData( &rec );
		dcsw = GetRecordDCsgn( &rec );
		aszan.ac_szrec	= rec;
	
		// 差引残高を配列にセット
#ifndef SZAN_CLOSE	// 11.05 /12
		if( dcsw != -1 && IsSzanCalq( &rec ) ) {
#else
		if( dcsw != -1 ) {
#endif
			if( dcsw == 0 ) {
				l_add( aszan.ac_szan, aszan.ac_szan, rec.m_val );
				if( sotozei_data( &rec ) )
					l_add( aszan.ac_szan, aszan.ac_szan, rec.m_zei );
			}
			else {
				l_sub( aszan.ac_szan, aszan.ac_szan, rec.m_val );
				if( sotozei_data( &rec ) )
					l_sub( aszan.ac_szan, aszan.ac_szan, rec.m_zei );
			}
		}
		m_SZan[n+1] = aszan;
		n++;
		if (top_zan) {
			dcnt = 1;
		}
		else {
			if (pDBzm->data->seq == nxtseq) {
				dcnt = n;
			}
		}
		// 定型仕訳 ソート用
		SCMDFY_SEQ.nsetseq( n, pDBzm->data->seq-1);

	} while( pDBzm->data->MoveNext() == 0 );

	int chk_ln;

	if( dcnt > 0 ) {
		if( pDBzm->data->SetAbsolutePosition( dcnt ) != 0 ) {
			ermset( 0, "仕訳データクエリーエラー！" );
			return -1;
		}
		chk_ln = (ln - 1);
	}
	else {
		//新規行へ
		pDBzm->data->MoveLast();
		dcnt = pDBzm->data->GetRecordCount();
		chk_ln = (ln - 2);
	}
//MyTrace( "L%d:@RequeryACLine\n", __LINE__ );

	acline_init( m_ALine, ACREC_MAX );
	abpos = dcnt;

	if (!top_zan) {
		// 指定行に表示できるかチェック
		for( i = chk_ln; i > 0; i-- ) {
			pDBzm->data->MovePrev();
			abpos--;
			if( pDBzm->data->st == -1 ) {
				abpos++;
				pDBzm->data->MoveNext();
				break;
			}
		}
//MyTrace( "L%d:@RequeryACLine	org - ln = %d\n", __LINE__, ln );
		// 表示行が変更
		if (i > 0)	ln -= i;
	}

	for( i = 0; i < lcnt; i++ ) {
		if (i == 0 && top_zan ) {
			acline_zanset(&m_ALine[0], &m_SZan[0]);
			continue;
		}
		acline_set( m_ALine, i, abpos );

//MyTrace( "L%d:@RequeryACLine	nxtseq = %d, data->seq = %d, ln %d\n", __LINE__, nxtseq, pDBzm->data->seq, ln );

		//差引残高
		memcpy( m_ALine[i].szan, &m_SZan[abpos], 6 );
		abpos++;

		pDBzm->data->MoveNext();
		if( pDBzm->data->st == -1 )
			break;
	} 

	// 新規行を追加
	if( (i+1) < lcnt && ! bCONFIRM_MASTER) {
		m_ALine[i+1].abs_pos = 0;
		m_ALine[i+1].drec.m_seq = pDBzm->m_lastdata.m_seq;

		pDBzm->GetInpDate( &m_ALine[i+1].drec );
		pDBzm->GetInpDate( m_ALine[i+1].bcddate );
	}

	ListDataSet();

	return ln;
}


//-------------------------------------------
// 差引残高を更新
//	int ln ... 修正・追加した 行
//  CDBINPDataRec* now ... 現在の仕訳
//  CDBINPDataRec* old ... 修正前の仕訳
//----------------------------------------------
int CDBAbookView::SzanUpdate( int ln, CDBINPDataRec* now, CDBINPDataRec* old ) 
{
	char work[6] = {0};
	CDBINPDataRec*	rec;
	BOOL bDel = FALSE;

	memcpy( work, m_ALine[ln-1].szan, sizeof work );
	int n = m_ALine[ln-1].abs_pos;

	int dcsw = GetRecordDCsgn( now );

	if( (now->m_dsign[0] & 0x01) && ! (old->m_dsign[0] & 0x01) )
		bDel = TRUE;

	if( n > 1 ) {
		// 指定行より、一つ前のデータから、作成
#ifndef SZAN_CLOSE	// 11.05 /12
		if( ! bDel && IsSzanCalq( now ) ) {
#else
		if( ! bDel ) {
#endif
			if( dcsw == 0 ) {
				l_add( work, m_SZan[n-1].ac_szan, now->m_val );	// 入金
				if( sotozei_data( now ) )
					l_add( work, work, now->m_zei );

//L_PRINT( "SzanUpdate(0) work = ", work );	TRACE( "\n" );
			}
			else if( dcsw == 1 ) {
				l_sub( work, m_SZan[n-1].ac_szan, now->m_val );	// 出金
				if( sotozei_data( now ) )
					l_sub( work, work, now->m_zei );

//L_PRINT( "SzanUpdate(1) work = ", work );	TRACE( "\n" );
			}
		}
		else {
			memcpy( work, m_SZan[n-1].ac_szan, 6 );
		}
	}
	else {
		// 最初のデータ
		CACSzan aszan;
		BaseSzanSet( &aszan );
		m_SZan[0] = aszan;

		memcpy( work, aszan.ac_szan, 6 );
		// 差引残高より 計算
#ifndef SZAN_CLOSE	// 11.05 /12
		if( ! bDel && IsSzanCalq( now ) ) {
#else
		if( ! bDel ) {
#endif
			if( dcsw != -1 ) {
				if( dcsw == 0 ) {	
					l_add( work, work, now->m_val );
					if( sotozei_data( now ) )
						l_add( work, work, now->m_zei );
				}
				else {
					l_sub( work, work, now->m_val );
					if( sotozei_data( now ) )
						l_sub( work, work, now->m_zei );
				}
			}
		}
	}

//MyTrace("L%d:::SzanUpdate ln = %d\n", __LINE__, ln);
//L_PRINT( "SzanUpdate_a work = ", work );	TRACE( "\n" );

	memcpy( m_SZan[n].ac_szan, work, sizeof work );
	memcpy( m_ALine[ln-1].szan, work, sizeof work );

	int dcnt = pDBzm->data->GetRecordCount();
	int last_pos = -1;

	// 現行以降 の差引残高を更新
	for( int i = ln; i < ACREC_MAX; i++ ) {
		rec = &m_ALine[i].drec;

		if( m_ALine[i].abs_pos > 0 ) {
			dcsw = GetRecordDCsgn( rec );

#ifndef SZAN_CLOSE	// 11.05 /12
			if( dcsw != -1 && IsSzanCalq( rec ) && !(rec->m_dsign[0]&0x01) ) {
#else
			if( dcsw != -1 ) {
#endif
				if( dcsw == 0 ) {		// 入金
					l_add( work, work, rec->m_val );
					if( sotozei_data( rec ) )
						l_add( work, work, rec->m_zei );
				}
				else {					// 出金
					l_sub( work, work, rec->m_val );
					if( sotozei_data( rec ) )
						l_sub( work, work, rec->m_zei );
				}
			}
			n = m_ALine[i].abs_pos;

			if( n >= 0 ) {
				memcpy( &m_SZan[n].ac_szan, work, sizeof work );
				memcpy( m_ALine[i].szan, work, sizeof work );
			}

			last_pos = m_ALine[i].abs_pos;
		}
		else {
			//最終行の場合、1行前のチェック
			if( i > 0 ) {
				if( m_ALine[i-1].abs_pos > 0 ) {
					last_pos = m_ALine[i-1].abs_pos;
					break;
				}
			}
		}
	}

	// PgUP 等で 後の表示部分の計算の更新がされていなかった。 10.25 /12
//MyTrace( "******** last_pos = %d, dcnt = %d ********\n", last_pos, dcnt );

	if( last_pos > 0 && last_pos < dcnt ) {
		ACBOOK_LINE	 acl;
		int counter = 0;

		for( int row = last_pos+1; row <= dcnt; row++ ) {
			n = row;
			rec	= &m_SZan[n].ac_szrec;

			dcsw = GetRecordDCsgn( rec );

#ifndef SZAN_CLOSE	// 11.05 /12
			if( dcsw != -1 && IsSzanCalq( rec ) && !(rec->m_dsign[0]&0x01) ) {
#else
			if( dcsw != -1 ) {
#endif
				if( dcsw == 0 ) {		// 入金
					l_add( work, work, rec->m_val );
					if( sotozei_data( rec ) )
						l_add( work, work, rec->m_zei );
				}
				else {					// 出金
					l_sub( work, work, rec->m_val );
					if( sotozei_data( rec ) )
						l_sub( work, work, rec->m_zei );
				}
			}
//L_PRINT( "---> *SzanUpdate work = ", work );	TRACE( "\n" );

			if( n >= 1 ) {
				memcpy( &m_SZan[n].ac_szan, work, sizeof work );
			}
		}
	}

	return 0;
}


//-------------------------------------------
// 差引残高を画面に再表示
//	int ln ... 再表示開始 行
//
//----------------------------------------------
void CDBAbookView::SzanDataSet( int ln ) 
{
	DTLIST_DATA data;

	int lcnt;
	lcnt = GetACListCount(); /*m_List.GetDispDataCount();*/
	int pn = AC_SZAN_PN;

	for( int line = ln; line <= lcnt; line++ ) {
		if( m_List.GetDtListData( line-1, &data ) < 0 ) {
			data.DTLIST_Init();
		}

//TRACE( "ooo::SzanDataSet01 (0) %08x, (1) %08xdata\n", data.dd_udtbl[0].userdata, data.dd_udtbl[1].userdata );

		ACBOOK_LINE *dtline;
		dtline = _getLINE_DATA( line );

		_TerminationDataSet( &data, dtline, &pn, 1 );

//TRACE( "ooo::SzanDataSet02 (0) %08x, (1) %08xdata\n", data.dd_udtbl[0].userdata, data.dd_udtbl[1].userdata );

		m_List.UpdateDtListData( line-1, &data, FALSE );
	//	m_List.SetDtListData( line-1, &data, FALSE );
	}

	m_List.RedrawDataListCtrl();
}


void CDBAbookView::FocusIcsDate( int ln, short click )
{
//MyTrace( "++Abook++ FocusIcsDate ln = %d, click = %d\n", ln, click );

	if( PROGRAM_OK == FALSE )
		return;

	try {
		//別画面入力消去
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			if( click & 0x10 )	nChar = VK_SHIFT;
			if( click & 0x20 )	nChar = VK_CONTROL;

			click_seljob( ln, nChar );
		}

	//	if( data_denpdialog( ln ) != 0 )
	//		return;

		if( _dline_chg( ln, AC_DATE_PN, click ? 0 : 1  ) == -1 )
			return;

/*
		// 取消データの場合
		if( isdeldata() )
		{
			set_focus( AC_SEQ_PN );
			return;
		}
*/
		set_nowpn( AC_DATE_PN );

	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}



// 日付チェックデータ
long CDBAbookView::CheckDataIcsDate( long nChar ) 
{
	char check[3] = {0};
	char ymd[3] = {0};
	char data[20] = {0};
	BYTE bcd[10] = {0};
	VARIANT var;
	CString str;

	//
	DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

	ACBOOK_LINE *ldata;
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
					int sgn = KESY_SG_GET( ldata->bcddate[0] );
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
						sprintf_s( data, sizeof data, "%02x", ldata->bcddate[0] );
						break;
					}
					if( len == 1 ) {
						data[2] = '0';
						data[3] = var.pbVal[0];
					}
					else	strncpy_s( &data[2], sizeof(data)-2, (char*)var.pbVal, 2 );
					data[4] = 0;

					// 09.06 /07
					if( pDBzm->inpdate_shimechk( ldata->bcddate, data ) != 0 )
						return -1;
				}
			}
		}
	}
	else {
		ldata = getLINE_DATA();
		int sgn = KESY_SG_GET( ldata->bcddate[0] );
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
			sprintf_s( data, sizeof data, "%02x", ldata->bcddate[0] );
			break;
		}
		if( var.pbVal != NULL ) {
			len = strlen( (char*)var.pbVal );
			if( len == 1 ) {
				data[2] = '0';
				data[3] = var.pbVal[0];
			}
			else	strncpy_s( &data[2], sizeof(data) -2, (char*)var.pbVal, 2 );
		}
		else {
			data[2] = data[3] = '0';
		}
		data[4] = 0;

		// 09.06 /07
		if( pDBzm->inpdate_shimechk( ldata->bcddate, data ) != 0 )
			return -1;
	}

	if( DateInputCheck( data, bcd ) != 0 ) {
		// 不正データが入力された
		ldata = getLINE_DATA();
		if( ! m_bMnthFix ) {
			var.pbVal = ldata->bcddate;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 4 );
		}
		else  {
			var.pbVal = &ldata->bcddate[1];
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 2 );
		}
		pDBzm->BcdDateToString( str, ldata->bcddate, m_bMnthFix );
		DBSetDispString( this, IDC_ICSDBEDTCTRL1, str );
		return -1;
	}
	else {
		memcpy( check, bcd, 3 );
	}

/*	if( !check[1] )
	{
		check[0] = 0;
		check[1] = (char)0xff;
	}
*/
	CVolDateDB vd;
	BOOL bCheck = FALSE;
//	if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) && !vd.db_vd_check( (BYTE*)check, pDBzm ) && !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd ) )
	if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) && pDBzm->check_datelimit( (BYTE*)check ) && 
		!pDBzm->Myvd_chek( (BYTE*)check ) && !pDBzm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd ) )
	{
		if( bCheck = AcbookDateCheck( (BYTE*)check ) ) {
			memmove( bcd, &ymd[0], 2 );

			if( ! m_bMnthFix ) {
				var.pbVal = bcd;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 4 );
			}
			else  {
				var.pbVal = &bcd[1];
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 2 );

				// 09.06 /07
				ldata = getLINE_DATA();
				ldata->bcddate[0] = bcd[0];
			}
		}
//*DEBUG
//VARIANT dbgvar;
//DBdata_get( this, IDC_ICSDBEDTCTRL1, &dbgvar, ICSDBEDT_TYPE_BCD, 4 );
//TRACE( "date %02x%02x", dbgvar.pbVal[0], dbgvar.pbVal[1] );
	}

	if( ! bCheck ) {
		Buzzer();
		memset( bcd, 0, sizeof bcd );
		var.pbVal = bcd;
		ldata = getLINE_DATA();

		DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, m_bMnthFix ? 2 : 4 );
		pDBzm->BcdDateToString( str, ldata->bcddate, m_bMnthFix );
		DBSetDispString( this, IDC_ICSDBEDTCTRL1, str );
		return -1;
	}

	return nChar;
}


//-------------------------------------------------------
//	仕訳挿入時 の 日付をチェック
//	BYTE* bcddate ... bcd 日付
//	return TRUE = チェックOK, FALSE = 不正な日付
//
//-------------------------------------------------------
BOOL CDBAbookView::AcbookDateCheck( BYTE* bcddate )
{
	if( m_InsLine == 0 )	return TRUE;

	int ln, lcnt;
	ln = get_nowln();

	if( ln != m_InsLine ) {
		ermset( 0, "挿入時の日付チェックエラー！" );
		ASSERT( FALSE );
		return FALSE;
	}

	lcnt = GetACListCount(); /*m_List.GetDispDataCount();*/

	if( IsMasterType(MST_INSERT) ) {
		//SEQ順
		if( (m_dwOpt & 0x01) ) {
			return TRUE;
		}
	}

	int date, pos, gengo;
	char buf[64];
	BOOL bRet = FALSE;
	CVolDateDB vd;
	VDPAC pac;

	sprintf_s( buf, sizeof buf, "%02x%02x", bcddate[0], bcddate[1] );
	date = atoi( buf );

	if( vd.db_vd_dpacGen( 1, 0, date, &pac, pDBzm ) != 0 )
		return FALSE;

	// 和暦→西暦
	db_datecnvGen( pac.Dgengo, pac.Dymd, &gengo, &date, 1, 0 );

	if( ln == 1 ) {
		// 現行 より 1つ後の行
		if( m_ALine[ln].abs_pos > 1 ) {
			pos = m_ALine[ln].abs_pos - 1;	// 上の行の ポジション

			pDBzm->data->SetAbsolutePosition(pos);

#ifndef _SPEED_CLOSE
			pDBzm->DBDATA_SetPosition( pDBzm->data->seq );
#else
			pDBzm->dbdata->SetAbsolutePosition( pDBzm->data->seq );
#endif

			if( pDBzm->dbdata->st == -1 ) {
				ermset( 0, "挿入時の日付チェックエラー！" );
				return FALSE;
			}

			if( ( date >= pDBzm->dbdata->ddate && pac.Doffset >= pDBzm->dbdata->mofs ) && 
				( date <= m_ALine[ln].drec.m_ddate && pac.Doffset <= m_ALine[ln].drec.m_mofs )) {
				bRet = TRUE;
			}
		}
		else {
			// 入力開始日付と比較
			if( ( date >= pDBzm->zvol->ip_ymd ) && 
				( date <= m_ALine[ln].drec.m_ddate && pac.Doffset <= m_ALine[ln].drec.m_mofs )) {
				bRet = TRUE;
			}
		}
	}
	else if( ln == lcnt ) {	// 画面上の最終行で挿入
		pos = m_ALine[ln-2].abs_pos;	// 現在行より1行上

		pDBzm->data->SetAbsolutePosition(pos);
		pDBzm->data->MoveNext();
		if( pDBzm->data->st == -1 )
			return FALSE;

#ifndef _SPEED_CLOSE
		pDBzm->DBDATA_SetPosition( pDBzm->data->seq );
#else
		pDBzm->dbdata->SetAbsolutePosition( pDBzm->data->seq );
#endif
		if( pDBzm->dbdata->st == -1 ) {
			ermset( 0, "挿入時の日付チェックエラー！" );
			return FALSE;
		}

		if( ( date >= m_ALine[ln-2].drec.m_ddate && pac.Doffset >= m_ALine[ln-2].drec.m_mofs ) && 
			( date <= pDBzm->dbdata->ddate && pac.Doffset <= pDBzm->dbdata->mofs ))  {
			bRet = TRUE;
		}
	}
	else {
		if( ( date >= m_ALine[ln-2].drec.m_ddate && pac.Doffset >= m_ALine[ln-2].drec.m_mofs ) && 
			( date <= m_ALine[ln].drec.m_ddate && pac.Doffset <= m_ALine[ln].drec.m_mofs ))  {
			bRet = TRUE;
		}
	}

	return bRet;
}


void CDBAbookView::TerminationIcsDate( long nChar, long inplen, long kst ) 
{
	BYTE prev_bcd[10] = {0};
	BYTE bcd[10] = {0};
	BYTE tmp[10] = {0};
	int date, ksgn, offset;
	VARIANT var;
	CString str;
	CVolDateDB vd;
	BOOL bDelData = FALSE;

	if( PROGRAM_OK == FALSE )
		return;

	if( ! nChar && m_bIgnoreDateTerm ) {
		m_bIgnoreDateTerm = FALSE;
		return;
	}

	try {

//		if( !nChar )	//フォーカスＯＦＦ
//		{
			if( nChar == VK_UP || nChar == VK_DOWN ) {
				int ln = get_nowln();
				if( kst & ICSDBEDT_KST_SHIFT ) {
					sel_shiftjob(ln, nChar, TRUE);
				}
				else {
					sel_shiftjob(ln, nChar, FALSE);
				}
			}

			if( !modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
			{
				goto NEXT;
			}
			else if( (getLINE_DATA()->drec.m_dsign[0] & 0x01) ) {
				bDelData = TRUE;
				goto NEXT;
			}
			else if( DenpTypeMode() && IsDenpyoSiwake( &getLINE_DATA()->drec) ) {
				goto NEXT;
			}
			memcpy( prev_bcd, getLINE_DATA()->bcddate, 2 );

			if( ! m_bMnthFix ) {
				DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 4 );
				if( var.pbVal != NULL )	memcpy( bcd, var.pbVal, 3 );
			}
			else {
				DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 2 );
				
				if( var.pbVal != NULL )	memcpy( &bcd[1], var.pbVal, 2 );
				bcd[0] = getLINE_DATA()->bcddate[0];
			}

			if( ! bcd[0] || ! bcd[1] ) {
				if( acINP_mode == _APPEND )
					pDBzm->DateConv( bcd, &m_SREC, CV2_1 );
				else
					pDBzm->DateConv( bcd, &m_CREC, CV2_1 );
			}
			else pDBzm->DateConv( bcd, &m_SREC, CV1_2 );
		
			if( m_SREC.m_ddate > 0 && m_SREC.m_mofs >= 0 /*!date_chk( nChar )*/ )
			{
				m_CREC.m_ddate = m_SREC.m_ddate;
				m_CREC.m_ksign = m_SREC.m_ksign;
				m_CREC.m_mofs  = m_SREC.m_mofs;
				
				//消費税
				struct _SY_MSG_PACK2 smp;
				sprintf_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, "%s", sy_chk_txt( &m_CREC, &smp, acINP_mode) );


				//消費税再表示
				int pntbl[] = {AC_ZEI_PN, AC_SYZ_PN};
				// 税額をセット(税率変更のため)
				char zei[6];
				::ZeroMemory( zei, sizeof zei );
				l_input( zei, (char*)(LPCTSTR)smp.SY_MSG_SYZEI );	
				memcpy( getLINE_DATA()->drec.m_zei, zei, 6 );
				memmove( m_CREC.m_zei, zei, sizeof(zei) );

				if( nChar != VK_NEXT && nChar != VK_PRIOR ) // for page down speed up
					TerminationDataSet( pntbl, 2 );
			}
			else
			{
				Buzzer();
				getLINE_DATA()->drec.m_ddate = 0;
				getLINE_DATA()->drec.m_mofs = 0;
				getLINE_DATA()->drec.m_ksign = 0;
				memset( getLINE_DATA()->bcddate, '\0', sizeof getLINE_DATA()->bcddate );

				TerminationDataSet( AC_DATE_PN );
				return;
			}

			pDBzm->BcdDateToYmd( bcd, &date );
			ksgn = KESY_SG_GET (bcd[0]);
			memcpy( getLINE_DATA()->bcddate, bcd, 2 );
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
				PROGRAM_OK = FALSE;
				ICSMessageBox( "月のオフセットを取得できません！ ");

				// 10.31 /07
				memcpy( bcd, prev_bcd, 2 );

				if( ! m_bMnthFix ) {
					var.pbVal = bcd;
					DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 4 );
				}
				else {
					var.pbVal = &bcd[1];
					DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 2 );
				}

				PROGRAM_OK = TRUE;

				set_focus( AC_DATE_PN );

				return;
			}

			getLINE_DATA()->drec.m_ddate = date;
			getLINE_DATA()->drec.m_mofs = offset;
			getLINE_DATA()->drec.m_ksign = (KESY_SG_GET (bcd[0]) == 0) ? 0 : 1;

			getLINE_DATA()->lcnd[AC_DATE_PN].INP_sg = TRUE;
			if( ! m_bMnthFix ) {
				var.pbVal = bcd;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 4 );
			}
			else  {
				var.pbVal = &bcd[1];
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 2 );
			}

			pDBzm->BcdDateToString( str, getLINE_DATA()->bcddate, m_bMnthFix );
			DBSetDispString( this, IDC_ICSDBEDTCTRL1, str );
			TerminationDataSet( AC_DATE_PN );

//			return;
//		}

NEXT:

		if( !nChar ) {
			// 10.16 /03 ... 諸口ブザー
			if( acINP_mode == _APPEND ) {
#ifdef LATER_CLOSE
				chk_syobz( &m_CREC, AC_DATE_PN );	
				if( pAUTOSEL->	SYOBUZ_OPT == OPT_SYOBUZ && is_syobuzzer_on(AC_DATE_PN) )
					Buzzer();
#endif
			}
			return;
		}

		if (nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT))
			nChar = VK_F2;

		// 取消データの Return キー は移動
		if( nChar == VK_RETURN ) {
			if( bDelData ) {
				nChar = VK_DOWN;
			}
		}

		if( nChar )
		{
			nChar = dline_chg( nChar );
			if(	nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合は
				if( isdeldata() )
				{
					set_focus( AC_DATE_PN );
					return;
				}

//				set_focus( AC_DATE_PN );
				return;
			}
		}

		long mnthFixKey = VK_TAB;
		if( !DateTabKeyMode() ) {
			mnthFixKey = VK_HOME;
			if( nChar == VK_TAB )	nChar = VK_RETURN;
		}

		if( nChar == VK_RETURN || nChar == VK_RIGHT || nChar == VK_F3 )
		{
			if( ! bDelData ) {
				int fsgn = 0;
				if( acINP_mode == _APPEND ) {
					if( m_bAutoSwk ) {
						if( ! getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg ) {
							_set_focus( get_nowln(), AC_TKY_PN );
							fsgn = 1;
						}
					}
				}
				if( ! fsgn ) {
					set_termfocus( get_nowln(), AC_DATE_PN, 1 );
				}
			}
		}
		else if( nChar == mnthFixKey ) {	// 日付固定切り替え
			DateInputChange();
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 )
		{
		//	if( acINP_mode != _APPEND )
		//		set_termfocus( get_nowln(), AC_DATE_PN, -1 );
		}
		else if( nChar == VK_DELETE ) {
			// 仕訳取消
			acbook_datadelete();
		}
		else if( nChar == VK_INSERT ) {
			// 仕訳挿入
		//	if( IsMasterType(MST_INSERT) ) {
				if( m_InsLine == 0 ) {
					if( InsertSiwake( get_nowln() ) == 0 ) {
						//定型仕訳へ
						OnRegdlguse();
					}
				}
				else {
					BackNormalAppendMode( get_nowln() );
				}
		//	}
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

// 仕訳取消し
void CDBAbookView::acbook_datadelete()
{
	int ln = get_nowln();
//	int lcnt;
//	lcnt = m_List.GetDispDataCount();

	if( ln <= 0 )	return;

	if( acINP_mode == _APPEND )	return;

	ACBOOK_LINE* ldata;
	ldata = getLINE_DATA();

	// 仕訳取消
	if( modefy_ok( acINP_mode, &ldata->drec ) ) 
	{
		// 実データの取消しを行う
#ifdef CLOSE // 行がわりで書き込む
		pDBzm->DBDATA_SetPosition( ldata->drec.m_seq );++++++++++++++++++++++++++

		if( pDBzm->dbdata->st == -1 ) {
			ermset( 0, "データを取得できません！" );
			return;
		}

		if( pDBzm->dbdata->dsign[0] & 0x01 )	// 取消し済み
		{
			if( !isDataDelOK( 1 ) )
				return;
			// 取消サインＯＦＦ
			pDBzm->dbdata->dsign[0] &= 0xfe;
		}
		else
		{
			if( !isDataDelOK( 0 ) )
				return;
			// 取消サインＯＮ

			pDBzm->dbdata->dsign[0] |= 0x01;
		}

		m_CREC.m_dsign[0]		= pDBzm->dbdata->dsign[0];
		ldata->drec.m_dsign[0] = pDBzm->dbdata->dsign[0];
		m_SCAN_rec.m_dsign[0]	= pDBzm->dbdata->dsign[0];

		if( pDBzm->DB_DataCorrect() == -1 ) {
			ermset( 0, "取消データの更新ができません！" );
			return;
		}
#endif
		if( m_CREC.m_dsign[0] & 0x01 )	// 取消し済み
		{
			if( !isDataDelOK( 1 ) )
				return;
			// 取消サインＯＦＦ
			m_CREC.m_dsign[0] &= 0xfe;
		}
		else
		{
			if( !isDataDelOK( 0 ) )
				return;
			// 取消サインＯＮ

			m_CREC.m_dsign[0] |= 0x01;
		}

		ldata->drec.m_dsign[0] = m_CREC.m_dsign[0];

		TerminationDataSet(AC_SEQ_PN);

		if( DenpTypeMode() ) {
			MODEFY_BIT.onbitn( m_CREC.m_seq - 1);
			pDBzm->DB_DataCorrect( &m_CREC );
			m_SCAN_rec = m_CREC;
		}
	}
}

// 日付入力の変更
void CDBAbookView::DateInputChange()
{
	// つぎのkillfocus は無視する
	m_bIgnoreDateTerm = TRUE;

	// モードの変更
	m_bMnthFix = ! m_bMnthFix;
	// 日付表示エリアを更新
	m_List.SetFocus();
//	c_set( IDC_BUTTON_DISP );
	
	set_focus( AC_DATE_PN );
}


// 部門番号IMEのEND処理 カナ検索
int CDBAbookView::CheckImeEndBumon( int pn, long nChar, LPCTSTR string ) 
{
	VARIANT var;

	CString buf = string;
	int		ret = 0;
	int ID;
	ID = (pn == AC_BMON_PN) ? IDC_ICSDBEDTCTRL1 : IDC_ICSDBEDTCTRL2;

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
		if( isdigitstr( (char*)(LPCTSTR)buf ) ) {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();

			DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
		else {
		//	var.pbVal = NULL;
		//	DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
			ret = 1;
		}
	}

	return ret;
}

//
// 仕訳行 部門 フォーカス
//
void CDBAbookView::FocusIcsBumon( int ln, short click )
{
	if( PROGRAM_OK == FALSE )
		return;

	try {
		//別画面入力消去
		Destroy_ModeLessDlg();

		int prev_ln = get_nowln();
		int prev_pn = get_nowpn();

		if( _dline_chg( ln, AC_BMON_PN, click ? 0 : 1  ) == -1 )
			return;

		// 取消データの場合は
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return;
		}
		if (data_denpdialog(ln) != 0)
			return;

		if( ln != prev_ln || prev_pn != AC_BMON_PN ) {
			SelPar par;
			par.sel_pn = SL_BUMON_PN;
			m_Seldata.SelectJob( &par, -1 );
		}
		set_nowpn( AC_BMON_PN );
	
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

//
// 仕訳行 部門 キーターミネーション
//
void CDBAbookView::TerminationBumon( int ln, long nChar, long inplen, long kst )
{
int check = -1, prev = -1;
int number, sel_sw, bmnno_input;
short len, n;
int bmon, pn, DCsgn, dcsw, srec_dcsw, nTmp;
VARIANT var;
long nKey;
static int	do_bmncheck = 0;
int set_bmntky = 0;

pn = AC_BMON_PN;

char	tmp[30] = {0}, bf[30] = {0};

	if( PROGRAM_OK == FALSE )
		return;

	// 借方／貸方 どちらにセットするか？
	DCsgn = GetRecordDCsgn( &getLINE_DATA()->drec );

	nTmp = GetRecordDCsgn( &m_SREC );
	if( nTmp == 0 || nTmp == -1 ) {
		srec_dcsw = 1;
	}
	else {
		srec_dcsw = 0;
	}

	try
	{
//		if( !nChar )	//フォーカスＯＦＦ
//		{
			// 部門選択 画面番号優先( VK_RETURN )
			nKey = (pAUTOSEL->BMNSEL_OPT == OPT_BMNSEL) ? VK_RETURN : VK_TAB;
			bmnno_input = 0;

			sel_sw = 1;
			// 直前のデータを保存
			
			if( DCsgn == 0 || DCsgn == -1 ) {
				prev = getLINE_DATA()->drec.m_cbmn;

				dcsw = 1;
				if( DCsgn == -1 && acINP_mode == _APPEND && ! getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg )
					dcsw = -1;
			}
			else {
				prev = getLINE_DATA()->drec.m_dbmn;
				dcsw = 0;
			}

			DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

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
				if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) ) {
					number = atoi( bf );
					CString bmnname;

					if( bmncode_todata2( &m_CREC, number, dcsw, &bmnname ) != FALSE ) {
						bmon_select_dsp( dcsw, &m_CREC, FALSE );
						check = (dcsw) ? m_CREC.m_cbmn : m_CREC.m_dbmn;
						bmnkoji_to_tekiyo( &m_CREC, &bmnname );
						bmneda_update(&m_CREC);
					}
					else {
						check = prev;
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

			if( ! modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
			{
				if( check != prev )
				{
					Buzzer();
				}

				pDBzm->BumonCodeToStr( (char*)tmp, sizeof tmp, prev );
				var.pbVal = (BYTE*)tmp;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

			}
			else
			{
				if( getLINE_DATA()->lcnd[pn].INP_sg == FALSE && len <= 0 && !inplen  )
				{
					if( getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY == FALSE )
						goto NEXT;
				}

				if( acINP_mode == _APPEND && getLINE_DATA()->lcnd[pn].INP_sg == FALSE )
				{
					if( check == -1 && sel_sw ) {
						check = (srec_dcsw) ? m_SREC.m_cbmn : m_SREC.m_dbmn;
						if( check != -1 )	set_bmntky = 1;
					}
				}

				if( dcsw ) {
					if( srec_dcsw )	m_SREC.m_cbmn = check;
					else			m_SREC.m_dbmn = check;

					m_CREC.m_cbmn = check;
					getLINE_DATA()->drec.m_cbmn = check;

					if( dcsw == -1 ) {
						m_CREC.m_dbmn = check;
						getLINE_DATA()->drec.m_dbmn = check;
					}
				}
				else {
					m_CREC.m_dbmn = check;
					getLINE_DATA()->drec.m_dbmn = check;
				}
//				if( check != -1 )	set_bmntky = 1;

				set_etcdsp( ln, pn );
				if( set_bmntky ) {
					struct _SET_KMKETC sk;
					// 部門
					sk.bmn = check;
					_set_bmnetc( &sk );
					bmnkoji_to_tekiyo( &m_CREC, &sk.etc );
					bmneda_update(&m_CREC);
				}

				//個別対応で部門の消費税あり
				if (pDBzm->IsKobetsuBmnSyz() && set_bmntky && acINP_mode == _APPEND ) {
					DWORD dwKobe = pDBzm->KobetsuSiwakeCheck(&m_CREC, check);

					if (dwKobe) {
						pDBzm->SetCDBData(&m_CREC);
						pDBsy->SyFnc(dwKobe);
						_SY_MSG_PACK2 smp;
						sprintf_s(getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, "%s", sy_chk_txt(&m_CREC, &smp, acINP_mode));
						//消費税再表示
						int pntbl[] = { AC_ZEI_PN, AC_SYZ_PN };
						char zei[6] = { 0 };
						::ZeroMemory(zei, sizeof zei);
						l_input(zei, (char*)(LPCTSTR)smp.SY_MSG_SYZEI);	// 税額をセット
						memcpy(getLINE_DATA()->drec.m_zei, zei, 6);
						memmove(m_CREC.m_zei, zei, sizeof(zei));

						if (is_syohizeisiwake(&m_CREC)) {
							getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = TRUE;
						}
						else {
							getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = FALSE;
						}
						if (nChar != VK_NEXT && nChar != VK_PRIOR) // for page down speed up
							TerminationDataSet(pntbl, 2);
					}
				}
				getLINE_DATA()->lcnd[pn].INP_sg = TRUE;

				pDBzm->BumonCodeToStr( tmp, sizeof tmp, check );
				var.pbVal = (BYTE*)tmp;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

				TerminationDataSet( pn );
			}
//			return;
//		}
NEXT:
		getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY = FALSE;

		if( !nChar ) {
			// 08.30 /04 ... 画面番号優先機能をつけたため、チェックする
			if( do_bmncheck ) {
				do_bmncheck = 0;
				if( BmnCodeChk( check, sel_sw, pn ) )
					return;
			}
			return;
		}
		// 合計部門入力は、カーソル再セット
		if( nChar == -1 ) {
			set_focus( pn );
			return;
		}

		if (nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT))
			nChar = VK_F2;

		if( nChar )
		{
			//#* 部門選択
			nChar = SelBmonDsp( nChar );

			//#* 部門チェック ... 08.28 /02
			if( acINP_mode != _APPEND && (nChar == VK_UP || nChar == VK_DOWN) ) {
				if( BmnCodeChk( check, sel_sw, pn ) )
					return;
			}

			nChar = dline_chg( nChar );
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
					set_focus( AC_DATE_PN );
					return;
				}

				set_focus( pn );
				return;
			}
		}

		if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) && nChar == VK_DELETE )
		{
			check = -1;
			::ZeroMemory( tmp, sizeof tmp );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

			if( dcsw ) {
				m_SREC.m_cbmn = -1;
				m_CREC.m_cbmn = -1;

				if( dcsw == -1 ) {
					m_SREC.m_dbmn = -1;
					m_CREC.m_dbmn = -1;
				}
			}
			else {
				m_SREC.m_dbmn = -1;
				m_CREC.m_dbmn = -1;
			}			
			if( dcsw ) {
				getLINE_DATA()->drec.m_cbmn = check;
				if( dcsw == -1 )
					getLINE_DATA()->drec.m_dbmn = check;
			}
			else {
				getLINE_DATA()->drec.m_dbmn = check;
			}
			getLINE_DATA()->lcnd[pn].INP_sg = TRUE;
			TerminationDataSet( pn );
			bmneda_update(&m_CREC);
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT )
		{
			//#* 部門チェック ... 08.28 /02
			// 画面番号優先のときはチェックしない [08.30 /04]
			if( nChar == VK_RETURN && nKey == VK_TAB ) {
				do_bmncheck = 0;
				if( BmnCodeChk( check, sel_sw, pn ) )
					return;
			}

			getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY = TRUE;

			if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) ) {
				// 部門番号優先時は、ここで 画面番号より選択
				if( nKey == VK_TAB && nChar == nKey && ! bmnno_input ) {
					if( check != -1 ) {
						number = check;

						// 番号が不正なときはブザー
						CString bmnname;
						if( !bmncode_todata2( &m_CREC, number, dcsw, &bmnname ) ) {
							if( dcsw ) {
								m_CREC.m_cbmn = prev;
								if( dcsw == -1 )
									m_CREC.m_dbmn = prev;
							}
							else {
								m_CREC.m_dbmn = prev;
							}
							Buzzer();
						}
						// 部門再表示
						bmon_select_dsp( dcsw, &m_CREC, FALSE );
						bmnkoji_to_tekiyo( &m_CREC, &bmnname );
						bmneda_update(&m_CREC);
					}
				}
			}

			if( acINP_mode == _APPEND && m_bAutoSwk ) {	// 自動仕訳
				int fcs = 0;

				if( M_KOJI ) {
					if( ! getLINE_DATA()->lcnd[AC_KOJI_PN].INP_sg ) {
						set_termfocus( get_nowln(), AC_BMON_PN, 1 );
						fcs++;
					}
				}
				if( ! fcs ) {
					if( ! getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg ) {
						set_focus( AC_KMK_PN );
						fcs++;
					}
				}
				if( ! fcs ) {
					set_termfocus( get_nowln(), AC_TKY_PN, 1 ); //金額へ
				}
			}
			else {
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

//#* 部門を表示 ... 02.27 /02
void CDBAbookView::bmon_select_dsp( int dc_sw, CDBINPDataRec *pCrec, BOOL bTerm )
{
	char	tmp[40] = {0};
	VARIANT var;
	pDBzm->BumonCodeToStr( tmp, sizeof tmp, dc_sw ? pCrec->m_cbmn : pCrec->m_dbmn );	
	var.pbVal = (BYTE*)tmp;
	DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	TerminationDataSet(AC_BMON_PN);

	if( bTerm ) {
		getLINE_DATA()->lcnd[AC_BMON_PN].NEXT_MOVE_KEY = TRUE;

#ifdef	_ICSPEN_
	//	if( IsPen() != FALSE )
	//	{
	//		//選択後次へ移動
	//		m_Input.PostMessage( WM_KEYDOWN, VK_TAB );
	//		return;
	//	}
	//	else
		{
			//データセットのためのダミーフォーカスＯＦＦ
			TerminationBumon( get_nowln(), 0, 0, 0 );
		}
#else
		//データセットのためのダミーフォーカスＯＦＦ
		TerminationBumon( pwnd, dc_sw, get_nowln(), 0, 0, 0 );
#endif
	}
}

// 
// 部門のチェック
BOOL CDBAbookView::BmnCodeChk( int bmn, int sw, int pn )
{
	BOOL bRet = FALSE;
	int st;
	CString msg;
	char tmp[20] = {0};

//	if( !FNC_BUTTON )
//	{
		if( pAUTOSEL->BMNCHK_OPT != OPT_BMNCHK )
			return FALSE;

		if( pn != AC_SELBMON_PN ) {
			if( ! modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
				return FALSE;
		}

		// 部門入力不可
		if( ! BMON_LNG ) return FALSE;
		// 部門未入力
		if( bmn == -1 )	return FALSE;

		// 不正な選択番号を入力したのでチェックせずにもどる
		if( !sw )	return TRUE;

		PROGRAM_OK = FALSE;

		if( !pDBzm->bmncode_ok( bmn ) ) {
			pDBzm->BumonCodeToStr( tmp, sizeof tmp, bmn );

		//	msg.Format( " 部門[%s]は登録されていません！\n\n 修正しますか？", tmp );
			msg.Format( " 部門[%s]は登録されていません！\n\n 登録しますか？\r\n\r\n はい…自動登録、いいえ…部門入力に戻る", tmp );

			st = myICSMessageBox( msg, MB_YESNO|MB_DEFBUTTON2 );
		//	if( st == IDYES ) {
			if( st == IDNO ) {
				set_focus( pn );
				set_nowpn( pn );
				bRet = TRUE;
			}		
		}
		
		PROGRAM_OK = TRUE;
//	}

	return bRet;
}

//
// 仕訳行 部門 フォーカス
//
void CDBAbookView::FocusIcsKoji( int ln, short click )
{
	if( PROGRAM_OK == FALSE )
		return;

	try {
		//別画面入力消去
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}

		int prev_ln = get_nowln();
		int prev_pn = get_nowpn();

		if( _dline_chg( ln, AC_KOJI_PN, click ? 0 : 1  ) == -1 )
			return;

		// 取消データの場合は
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return;
		}
		if( data_denpdialog( ln ) != 0 )
			return;

		if( ln != prev_ln || prev_pn != AC_KOJI_PN ) {
			SelPar par;
			par.sel_pn = SL_KOJI_PN;
			m_Seldata.SelectJob( &par, -1 );
		}

		set_nowpn( AC_KOJI_PN );
	
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


//
// 仕訳行 工事 キーターミネーション
//
void CDBAbookView::TerminationKoji( int ln, long nChar, long inplen, long kst )
{
CString check, prev, koji, zerochk, kjnam;
int number, sel_sw, kj_dcset = 0, kno_input;
int set_kjtky = 0;
int digit_sgn = 0;
int	DCsgn, nTmp, srec_dcsw, dcsw;
short len, n;
int pn;
VARIANT var;
long nKey;
static int	do_knocheck = 0;

pn = AC_KOJI_PN;

char	tmp[30] = {0}, bf[30] = {0};

	if( PROGRAM_OK == FALSE )
		return;

	// 借方／貸方 どちらにセットするか？
	DCsgn = GetRecordDCsgn( &getLINE_DATA()->drec );

	nTmp = GetRecordDCsgn( &m_SREC );
	if( nTmp == 0 || nTmp == -1 ) {
		srec_dcsw = 1;
	}
	else {
		srec_dcsw = 0;
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
			if( DCsgn == 0 || DCsgn == -1 ) {
				prev = getLINE_DATA()->drec.m_ckno;
				dcsw = 1;
				if( DCsgn == -1 && acINP_mode == _APPEND && ! getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg )
					dcsw = -1;
			}
			else {
				prev = getLINE_DATA()->drec.m_dkno;
				dcsw = 0;
			}

			DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
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
				if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) ) {
					number = atoi( bf );
					if( kojicode_todata( &m_CREC, &kjnam, number, dcsw ) != FALSE ) {
						koji_select_dsp( dcsw, &m_CREC, FALSE );
						check = (dcsw) ? m_CREC.m_ckno : m_CREC.m_dkno;

						// 工事名称から、仕訳摘要へ
						bmnkoji_to_tekiyo( &m_CREC, &kjnam, 1 );
					}
					else {
						check = prev;
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
				else
					set_kjtky = 1;
			}

			if( ! modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
			{
				if( check != prev )
				{
					Buzzer();
				}

				pDBzm->KojiCodeToStr( (char*)tmp, sizeof tmp, prev );
				var.pbVal = (BYTE*)tmp;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

			}
			else
			{
				if( getLINE_DATA()->lcnd[pn].INP_sg == FALSE && len <= 0 && !inplen )
				{
					if( getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY == FALSE )
						goto NEXT;
				}
				if( INP_mode == _APPEND && getLINE_DATA()->lcnd[pn].INP_sg == FALSE )
				{
					if( check.IsEmpty() && sel_sw ) {
						check = (srec_dcsw) ? m_SREC.m_ckno : m_SREC.m_dkno;
						set_kjtky = 1;
					}
				}

				if( dcsw )	m_SREC.m_ckno = check;
				else		m_SREC.m_dkno = check;

				if( dcsw ){
					m_CREC.m_ckno = check;
					getLINE_DATA()->drec.m_ckno = check;
					if( dcsw == -1 ) {
						m_CREC.m_dkno = check;
						getLINE_DATA()->drec.m_dkno = check;
					}
				}
				else {
					m_CREC.m_dkno = m_SREC.m_dkno;
					getLINE_DATA()->drec.m_dkno = check;
				}

				// 名称表示
				set_etcdsp( ln, pn );
				if( set_kjtky ) {
					m_Seldata.GetKojiName( check, kjnam, FALSE );
					bmnkoji_to_tekiyo( &m_CREC, &kjnam, 1 );
				}

				getLINE_DATA()->lcnd[pn].INP_sg = TRUE;

				pDBzm->KojiCodeToStr( tmp, sizeof tmp, check );
				var.pbVal = (BYTE*)tmp;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

				TerminationDataSet( AC_KOJI_PN );
			}
//			return;
//		}
NEXT:
		getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY = FALSE;

		if( !nChar ) {
			// 08.30 /04 ... 画面番号優先機能をつけたため、チェックする
			if( do_knocheck ) {
				do_knocheck = 0;
				if( KojiCodeChk( check, sel_sw, pn ) )
					return;
			}
			return;
		}

		if (nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT))
			nChar = VK_F2;

		if( nChar )
		{
			// 工事選択 
			nChar = SelKojiDsp( nChar );

			// 工事チェック
			if( INP_mode != _APPEND && (nChar == VK_UP || nChar == VK_DOWN) ) {
				if( KojiCodeChk( check, sel_sw, pn ) )
					return;
			}

			nChar = dline_chg( nChar );
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
					set_focus( AC_DATE_PN );
					return;
				}

				set_focus( pn );
				return;
			}
		}

		if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) && nChar == VK_DELETE )
		{
			check.Empty();
			::ZeroMemory( tmp, sizeof tmp );
			var.pbVal = NULL;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

			if( dcsw ) {
				m_SREC.m_ckno.Empty();
				m_CREC.m_ckno.Empty();

				getLINE_DATA()->drec.m_ckno .Empty();
				if( dcsw == -1 )
					getLINE_DATA()->drec.m_dkno .Empty();
			}
			else {
				m_SREC.m_dkno.Empty();
				m_CREC.m_dkno.Empty();
				getLINE_DATA()->drec.m_dkno .Empty();
			}			

			getLINE_DATA()->lcnd[pn].INP_sg = TRUE;
			TerminationDataSet( pn );	
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
		{
			// 画面番号優先のときはチェックしない [08.30 /04]
			if( nChar == VK_RETURN && nKey == VK_TAB ) {
				do_knocheck = 0;
				if( KojiCodeChk( check, sel_sw, pn ) )
					return;
			}

			getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY = TRUE;

			if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) ) {
				// 工事番号優先時は、ここで 画面番号より選択( TABキー押された場合 )
				if( nKey == VK_TAB && nChar == nKey && ! kno_input ) {
					if( ! check.IsEmpty() ) {
						digit_sgn = isdigitstr( (char*)(LPCTSTR)check );
						if( digit_sgn ) {
							number = atoi( check );

							// 番号が不正なときはブザー
							if( !kojicode_todata( &m_CREC, &kjnam, number, dcsw ) ) {
								if( dcsw ) {
									m_CREC.m_ckno = prev;
									if( dcsw == -1 )
										m_CREC.m_dkno = prev;
								}
								else {
									m_CREC.m_dkno = prev;
								}
								Buzzer();
							}
							else {
								// 工事名称から、仕訳摘要へ
								bmnkoji_to_tekiyo( &m_CREC, &kjnam, 1 );
							}
							// 部門再表示
							koji_select_dsp( dcsw, &m_CREC, FALSE );
						}
					}
				}
			}

			if( acINP_mode == _APPEND && m_bAutoSwk ) {	// 自動仕訳
				int fcs = 0;

				if( ! getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg ) {
					set_focus( AC_KMK_PN );
					fcs++;
				}
				if( ! fcs ) {
					set_termfocus( get_nowln(), AC_TKY_PN, 1 ); //金額へ
				}
			}
			else {
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
void CDBAbookView::koji_select_dsp( int dc_sw, CDBINPDataRec *pCrec, BOOL bTerm )
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
	DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	TerminationDataSet(pn);

	if( bTerm ) {
		getLINE_DATA()->lcnd[get_nowpn()].NEXT_MOVE_KEY = TRUE;

#ifdef	_ICSPEN_
	//	if( IsPen() != FALSE )
	//	{
	//		//選択後次へ移動
	//		pwnd->GetDlgItem( getINP_CTL( pwnd, get_nowpn() )->IDC_X )->PostMessage( WM_KEYDOWN, VK_TAB );
	//		return;
	//	}
	//	else
		{
			//データセットのためのダミーフォーカスＯＦＦ
			TerminationKoji( get_nowln(), 0, 0, 0 );
		}
#else
		//データセットのためのダミーフォーカスＯＦＦ
		TerminationIcsinputKoji( pwnd, dc_sw, get_nowln(), 0, 0, 0 );
#endif
	}
}

// 08.28 /02
// 工事番号のチェック
BOOL CDBAbookView::KojiCodeChk( CString koji, int sw, int pn )
{
	BOOL bRet = FALSE;
	int st;
	CString msg;
	char tmp[20] = {0};

//	if( !FNC_BUTTON )
//	{
		if( pAUTOSEL->BMNCHK_OPT != OPT_BMNCHK )
			return FALSE;

		if( ! modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
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

		//	msg.Format( " 工事番号[%s]は登録されていません！\n\n 修正しますか？", tmp );
			msg.Format( " 工事番号[%s]は登録されていません！\n\n 処理を進めますか？\r\n\r\n はい…処理続行、いいえ…工事入力に戻る", tmp );

			st = myICSMessageBox( msg, MB_YESNO|MB_DEFBUTTON2 );
		//	if( st == IDYES ) {
			if( st == IDNO ) {
				set_focus( pn );
				set_nowpn( pn );
				bRet = TRUE;
			}		
		}
		
		PROGRAM_OK = TRUE;
//	}

	return bRet;
}


// 部門番号が変更されたとき、基本科目の枝番表示を変更する
void CDBAbookView::BaseBmnEda_update()
{
	if( !BMON_MST )	return;

	if( !BRmst )	return;

	if( pAUTOSEL->BRTEK_OPT != OPT_DSP )
		return;

	if( m_BaseEda == -1 ) {
		GetDlgItem(IDC_STATIC5)->SetWindowText("");
		return;
	}

	// 枝番名称表示
	struct _SET_KMKETC sk;
	sk.bmn = m_BaseBmn;
	//科目枝番を表示
	if( pAUTOSEL->BMNTKBR_OPT != OPT_CHKON )
		sk.bmn = -1;

	sk.brn = m_BaseEda;
	sk.code = m_BaseCode;

	if( _set_kmketc(&sk, FALSE) ) {
		GetDlgItem(IDC_STATIC5)->SetWindowText(sk.etc);
	}
	else {
		GetDlgItem(IDC_STATIC5)->SetWindowText("");
	}
}



// 部門番号が変更されたとき、科目名称の枝番表示を変更する
void CDBAbookView::bmneda_update(CDBINPDataRec* rec)
{
	if( !BMON_MST )	return;

	if( !BRmst )	return;

	if( pAUTOSEL->BMNTKBR_OPT != OPT_CHKON )
		return;

	if( pAUTOSEL->BRTEK_OPT != OPT_DSP )
		return;

	int dc_sw = GetRecordDCsgn(&m_CREC);

	if( dc_sw == 0 || dc_sw == -1) {
		if( m_CREC.m_cbr == -1 )
			return;
	}
	else {
		if( m_CREC.m_dbr == -1 )
			return;
	}
	int ln = get_nowln();
	// 枝番等セット
	set_etcdsp(ln, AC_KMK_PN);
	TerminationDataSet(AC_KMK_PN);
}


// 工事番号を仕訳摘要へ
void CDBAbookView::bmnkoji_to_tekiyo( CDBINPDataRec* rec, CString* kjnam, int type/* = 0*/ )
{
	if( type ) {
		if( pAUTOSEL->KJTOTKY_OPT != OPT_KJTOTKY )
			return;
	}
	else {
		if( pAUTOSEL->BMNTOTKY_OPT != OPT_CHKON )
			return;
	}

	if( getLINE_DATA()->lcnd[ AC_TKY_PN ].IMG_sg == 0 ) {
		CString tmp;
		tmp = rec->m_tekiyo;

		if( ! IsIncludeString( tmp, *kjnam ) ) {
			tmp += *kjnam;
			int len;
			len = tmp.GetLength();

			if( len > (Voln1->tk_ln * 2) ) {
				LPSTR p = tmp.GetBuffer(256);
				int n = get_strcnt( p, (Voln1->tk_ln * 2) );

				*(p + n) = '\0';
				tmp.ReleaseBuffer();
			}

			rec->m_tekiyo = tmp;
			getLINE_DATA()->drec.m_tekiyo = tmp;
			TerminationDataSet( AC_TKY_PN );
		}
	}
}


// 科目 フォーカス
void CDBAbookView::FocusIcsKamoku( int ln, short click )
{

	if( PROGRAM_OK == FALSE )
		return;

	try {
		//別画面入力消去
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}

		int prev_ln = get_nowln();
		int prev_pn = get_nowpn();

		if( _dline_chg( ln, AC_KMK_PN, click ? 0 : 1  ) == -1 )
			return;

		// 取消データの場合は
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return;
		}
		if( data_denpdialog( ln ) != 0 )
			return;

		if( ln != prev_ln || prev_pn != AC_KMK_PN ) {
			SelPar par;
			par.sel_pn = SL_KAMOKU_PN;
			m_Seldata.SelectJob( &par, -1 );
		}

		set_nowpn( AC_KMK_PN );
	
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}

// 科目チェックデータ
long CDBAbookView::CheckDataKamoku( long nChar, LPCTSTR string ) 
{
	CString kmkstr, ldata_code;
	int edaban = -1, ldata_brn = -1;
	int dcsw;
	struct _KamokuData check = {0}, ans = {0};
	DBKNREC* pKn;
	BOOL bCodeInput = FALSE;

	// 文字列チェック
	check.kd_eda = -1;
	KamokuStr_ChkLength( string, &check, Voln1->edcol );

	int kamoku = atoi( check.kd_code );

	if( pAUTOSEL->KMNO_OPT == OPT_KMNO ) {
		if( ! BRNTKY_sel || ( BRNTKY_sel && kamoku < 9 ) ) {
			if( check.kd_sgn == 1 || check.kd_sgn == 5 ) {
				bCodeInput = TRUE;
			}
		}
	}
	// [ . +枝番 のみ]
	if( (check.kd_sgn == 2 || check.kd_sgn == 3) ) {
		if( acINP_mode == _APPEND ) {

			if( ! getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg ) {	//科目未入力
				dcsw = GetRecordDCsgn( &m_SREC );

				if( dcsw == 0 || dcsw == -1 )
					memcpy( check.kd_code, m_SREC.m_cre, KMKCD_LN );
				else
					memcpy( check.kd_code, m_SREC.m_dbt, KMKCD_LN );
			}
			else {
				dcsw = GetRecordDCsgn( &getLINE_DATA()->drec );

				if( dcsw == 0 || dcsw == -1 )
					memcpy( check.kd_code, getLINE_DATA()->drec.m_cre, KMKCD_LN );
				else
					memcpy( check.kd_code, getLINE_DATA()->drec.m_dbt, KMKCD_LN );
			}
		}
		else {
			dcsw = GetRecordDCsgn( &getLINE_DATA()->drec );

			if( dcsw == 0 || dcsw == -1 )
				memcpy( check.kd_code, getLINE_DATA()->drec.m_cre, KMKCD_LN );
			else
				memcpy( check.kd_code, getLINE_DATA()->drec.m_dbt, KMKCD_LN );
		}
	}

	if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) && m_Seldata.InputItemCheck( ans, check, 0 ) == TRUE )
	//	&& CheckKamokuInput(&ans) == 0 )
	{
		// [コード + .] 前行枝番コピー
		if( acINP_mode == _APPEND ) {
			if( check.kd_sgn == 4 || check.kd_sgn == 5 ) {
				if( GetRecordDCsgn( &m_SREC ) == 0 )
					ans.kd_eda = m_SREC.m_cbr;
				else
					ans.kd_eda = m_SREC.m_dbr;
			}
		}

//		if( BRNTKY_sel && ans.kd_eda != -1 ) {
//			set_brn( ans.kd_eda );
//		}
		DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &ans, pDBzm );

		ACBOOK_LINE *ldata = getLINE_DATA();

		ldata->lcnd[ AC_KMK_PN ].KEY_inp = TRUE;

		if( acINP_mode == _APPEND )
		{
			if( ldata->lcnd[ AC_KMK_PN ].INP_sg )
				m_TKsyz = 0;
			kmkstr = ans.kd_code;
			edaban = ans.kd_eda;

			int dcsw = GetRecordDCsgn( &ldata->drec );

			if( dcsw == 0 ) {
				ldata_code = ldata->drec.m_cre;
				ldata_brn = ldata->drec.m_cbr;
			}
			else {
				ldata_code = ldata->drec.m_dbt;
				ldata_brn  = ldata->drec.m_dbr;
			}

			//科目が変更されたら摘要消費税をクリア
			if( ! ldata_code.IsEmpty() ) {
				if( is_mdfybrnitem( ldata_code, ldata_brn, kmkstr, edaban ) )
					m_TKsyz2 = 0;
			}
		}
	}
	else
	{
		Buzzer();

		::ZeroMemory( &ans, sizeof(ans) );
		ans.kd_eda = -1;
		if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
		{
			if( getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg )
				DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &ans, pDBzm );
			else {
				Buzzer();
				DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &ans, pDBzm );
			}

		//	set_focus( AC_KMK_PN );
		}
		else
		{
			ACBOOK_LINE *ldata = getLINE_DATA();

			dcsw = GetRecordDCsgn( &ldata->drec );

			if( dcsw == 0 || dcsw == -1 ) {
				ldata_code = ldata->drec.m_cre;
				ldata_brn = ldata->drec.m_cbr;
			}
			else {
				ldata_code = ldata->drec.m_dbt;
				ldata_brn  = ldata->drec.m_dbr;
			}

			strcpy_s( ans.kd_code, sizeof ans.kd_code, ldata_code );
			ans.kd_eda = ldata_brn;

			pKn = pDBzm->DB_PjisToKnrec( ldata_code );
			if( pKn ) {
				KamokuString( pKn, ans.kd_name, sizeof ans.kd_name );
			}
			else	ZeroMemory( ans.kd_name, sizeof ans.kd_name );
			DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &ans, pDBzm );
		}
		return 0;
	}

	return nChar;
}


// 科目ターミネーション
void CDBAbookView::TerminationKamoku( int ln, long nChar, long inplen, long kst )
{
int st;
ACBOOK_LINE *ldata;

BOOL set_brtek = FALSE;
CString kmkcd_str, ld_kmk;
int eda_code, ld_eda, DCflg;
struct _KamokuData check = {0};
char	zei[6];
struct _SY_MSG_PACK2 smp;
DBKNREC* pKn;
int dcsw = -1;
int clear = 0;
int kmk_diff = 0;
int eda_diff = 0;

	if( PROGRAM_OK == FALSE )
		return;

//	if( !nChar )	//フォーカスＯＦＦ
//	{
		if( !nChar )
			getLINE_DATA()->lcnd[ AC_KMK_PN ].KEY_inp = FALSE;

		l_defn( _BIN );

		if( nChar == 0x16 ) {
			CDBINPDataRec* pd;

			::ZeroMemory( &check, sizeof check );

			pd = &getLINE_DATA()->drec;

			dcsw = GetRecordDCsgn( pd );

			if( dcsw == 0 || dcsw == -1 ) {
				memcpy( check.kd_code, pd->m_cre, KMKCD_LN );
				check.kd_eda = pd->m_cbr;
			}
			else {
				memcpy( check.kd_code, pd->m_dbt, KMKCD_LN );
				check.kd_eda = pd->m_dbr;
			}

			if( (pKn = pDBzm->DB_PjisToKnrec( check.kd_code ) ) != NULL ) {
				KamokuString( pKn, check.kd_name, sizeof check.kd_name );
			//	memmove( check.kd_name, pKn->knnam, 14 );
			}

			DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, pDBzm );
		}

		if( !modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
			goto NEXT;

		DBkamokudata_get( this, IDC_ICSDBEDTCTRL1, &check, pDBzm );

/*
		// 枝番セット
		if( BRNTKY_sel && get_brn() != -1 )
		{
//			check.ip_kmk.edaban = hl_lev( get_brn() );
			eda_code = get_brn();
			check.kd_eda = eda_code;
			set_brn( -1 );	// リセット
		}
*/
		if( !check.kd_code[0] /*check.CdNum == 0xffff*/ && getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg == FALSE )
		{
			if( getLINE_DATA()->lcnd[AC_KMK_PN].NEXT_MOVE_KEY == FALSE )
//				return;
				goto NEXT;

			dcsw = GetRecordDCsgn( &m_SREC );

			if( pAUTOSEL->BRCPY_OPT == OPT_NONOPT ) {	// 08.02 /02
				if( dcsw == 0 || dcsw == -1 )
					memcpy( check.kd_code, m_SREC.m_cre, KMKCD_LN );
				else
					memcpy( check.kd_code, m_SREC.m_dbt, KMKCD_LN );

				check.kd_eda = -1;
			}
			else {
				if( dcsw == 0 || dcsw == -1 ) {
					memcpy( check.kd_code, m_SREC.m_cre, KMKCD_LN );
					check.kd_eda = m_SREC.m_cbr;
				}
				else {
					memcpy( check.kd_code, m_SREC.m_dbt, KMKCD_LN );
					check.kd_eda = m_SREC.m_dbr;
				}
			}

			struct	_DBKNREC* pKn;
			if( (pKn = pDBzm->DB_PjisToKnrec( check.kd_code ) ) != NULL ) {
				KamokuString( pKn, check.kd_name, sizeof check.kd_name );
			//	memmove( check.kd_name, pKn->knnam, 14 );
			}

			set_brtek = FALSE;

			// 入力枝番タイプセット
			ldata = getLINE_DATA();
			kmkcd_str = check.kd_code;
			eda_code = check.kd_eda;
			ld_kmk = "";

			ldata->lcnd[ AC_KMK_PN ].INP_type = brn_inptype( ldata->lcnd[ AC_KMK_PN ].INP_type, ld_kmk, -1, kmkcd_str, eda_code );
		}
		else	// すでに科目が入力されている
		{
			set_brtek = FALSE;

			// 入力枝番タイプセット
			ldata = getLINE_DATA();
			kmkcd_str = check.kd_code;
			eda_code = check.kd_eda;
			int brntyp;
			dcsw = GetRecordDCsgn( &ldata->drec );

			if( dcsw == 0 || dcsw == -1 ) {
				ld_kmk = ldata->drec.m_cre;
				ld_eda = ldata->drec.m_cbr;
				brntyp = brn_inptype( ldata->lcnd[ AC_KMK_PN ].INP_type,  ldata->drec.m_cre, ldata->drec.m_cbr, kmkcd_str, eda_code );

			}
			else {
				ld_kmk = ldata->drec.m_dbt;
				ld_eda = ldata->drec.m_dbr;
				brntyp = brn_inptype( ldata->lcnd[ AC_KMK_PN ].INP_type,  ldata->drec.m_dbt, ldata->drec.m_dbr, kmkcd_str, eda_code );
			}

			if( ld_kmk != kmkcd_str )
				kmk_diff = 1;

			if (ld_eda != eda_code)
				eda_diff = 1;

			ldata->lcnd[ AC_KMK_PN ].INP_type = brntyp;

			// 枝番摘要セット
			if( ldata->lcnd[ AC_KMK_PN ].INP_type == 1 )
			{
				// 科目.枝番 に変更があったか？
				if( pAUTOSEL->BRTOTKY_OPT == OPT_EXEC ) {
					kmkcd_str = check.kd_code;
					eda_code = check.kd_eda;

					if( ldata->lcnd[ AC_KMK_PN ].KEY_inp || is_mdfybrnitem( ld_kmk, ld_eda, kmkcd_str, eda_code ) )
						set_brtek = TRUE;
				}
			}
		}

		ldata->lcnd[ AC_KMK_PN ].KEY_inp = FALSE;

		if( check.kd_code[0] != '\0' )
		{
			dcsw = GetRecordDCsgn( &m_SREC );

			if( dcsw == 0 || dcsw == -1 ) {
				m_SREC.m_cre = check.kd_code;
				m_SREC.m_cbr = check.kd_eda;
			}
			else {
				m_SREC.m_dbt = check.kd_code;
				m_SREC.m_dbr = check.kd_eda;
			}

			ldata = getLINE_DATA();

			int kmksgn = GetKamokuSign();
			if( kmksgn == -1 ) {
				int crec_dc = GetRecordDCsgn(&m_CREC);

				if( crec_dc != -1 ) {
					kmksgn = crec_dc == 0 ? 1 : 0;
				}
			}

			switch( kmksgn ) {
			case 0:
				m_CREC.m_dbt = check.kd_code;
				m_CREC.m_dbr = check.kd_eda;
				ldata->drec.m_dbt = check.kd_code;
				ldata->drec.m_dbr = check.kd_eda;
				break;
			case 1:
				m_CREC.m_cre = check.kd_code;
				m_CREC.m_cbr = check.kd_eda;
				ldata->drec.m_cre = check.kd_code;
				ldata->drec.m_cbr = check.kd_eda;
				break;
			default:
				SetAcbookKamoku( &m_CREC, &check );
				SetAcbookKamoku( &ldata->drec, &check );

			}

			getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg = TRUE;
		}
		else
		{
			ldata = getLINE_DATA();
			dcsw = GetRecordDCsgn( &ldata->drec );

			if( dcsw == 0 || dcsw == -1 ) {
				memcpy( check.kd_code, ldata->drec.m_cre, KMKCD_LN );
				check.kd_eda = ldata->drec.m_cbr;
			}
			else {
				memcpy( check.kd_code, ldata->drec.m_dbt, KMKCD_LN );
				check.kd_eda = ldata->drec.m_dbr;
			}

			pKn = pDBzm->DB_PjisToKnrec( check.kd_code );
			if( pKn ) {
				KamokuString( pKn, check.kd_name, sizeof check.kd_name );
			//	sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", pKn->knnam );
			}
			else	::ZeroMemory( &check.kd_name, sizeof check.kd_name );

			DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, pDBzm );

			// 資金繰諸口枝番等セット
			set_etcdsp( ln, AC_KMK_PN );

			TerminationDataSet( AC_KMK_PN );
			return;
		}

		//消費税
		sprintf_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, "%s", sy_chk_txt( &m_CREC, &smp, acINP_mode) );
		//消費税 部門仕入区分を優先
		if ((kmk_diff || eda_diff) && acINP_mode == _APPEND && pDBzm->IsKobetsuBmnSyz()) {
			int dcsw;
			dcsw = GetRecordDCsgn(&m_CREC);
			int bmn;
			if (dcsw == 0) {
				bmn = m_CREC.m_cbmn;
			}
			else {
				bmn = m_CREC.m_dbmn;
			}
			DWORD dwKobe = pDBzm->KobetsuSiwakeCheck(&m_CREC, bmn);
			if (dwKobe) {
				//枝番の仕入区分に変わるので、部門の分に変更する
				pDBzm->SetCDBData(&m_CREC);
				pDBsy->SyFnc(dwKobe);
				//消費税再セット
				sprintf_s(getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, "%s", sy_chk_txt(&m_CREC, &smp, acINP_mode));
			}
		}

		//消費税再表示
		int pntbl[] = {AC_ZEI_PN, AC_SYZ_PN};
		// 05.22 /08
		::ZeroMemory( zei, sizeof zei );
		l_input( zei, (char*)(LPCTSTR)smp.SY_MSG_SYZEI );	// 税額をセット
		memcpy( getLINE_DATA()->drec.m_zei, zei, 6 );
		memmove( m_CREC.m_zei, zei, sizeof(zei) );

		if( is_syohizeisiwake( &m_CREC  )) {
			getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = TRUE;
		}
		else {
			getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = FALSE;
		}
		if( nChar != VK_NEXT && nChar != VK_PRIOR ) // for page down speed up
			TerminationDataSet( pntbl, 2 );

		dcsw = GetRecordDCsgn( &m_CREC );
		DCflg = (dcsw == 1) ? 0 : 1;	// 相手科目の 借方／貸方サイン

		if( IsSyafuKaisei( pDBzm->zvol ) ) {
			if( kmk_diff )	clear = 1;
		}

		HendJiyuCheck( &m_CREC, clear );
		InvnoCheck(&m_CREC);

		if( set_brtek )
		{
			// 枝番摘要を摘要に
			if( DCflg ) {
				if( ! brnitm_to_tky( ln, m_CREC.m_cbmn, m_CREC.m_cre, m_CREC.m_cbr ) )
					ldata->lcnd[ AC_KMK_PN ].INP_type = 3;	// 枝番登録に期待
			}
			else {			
				if( ! brnitm_to_tky( ln, m_CREC.m_dbmn, m_CREC.m_dbt, m_CREC.m_dbr ) )
					ldata->lcnd[ AC_KMK_PN ].INP_type = 3;	// 枝番登録に期待
			}
		}
		if( !nChar )
		{
			PROGRAM_OK = FALSE;
			{

//				if( !FNC_BUTTON )
				{
					//枝番チェック
					st = ac_brchk1( &m_CREC, DCflg );

					if( st == 99 )
					{
						//枝番登録選択
						if( ! DCflg ) 
							st = brzan_torok( this, m_CREC.m_dbmn, m_CREC.m_dbt, m_CREC.m_dbr, 0 );
						else
							st = brzan_torok( this, m_CREC.m_cbmn, m_CREC.m_cre, m_CREC.m_cbr, 1 );

						if( st == 0 ) {
							// 枝番摘要 -> 仕訳摘要
							if( pAUTOSEL->BRTOTKY_OPT == OPT_EXEC ) {
								if( ! DCflg )
									brnitm_to_tky( ln, m_CREC.m_dbmn, m_CREC.m_dbt, m_CREC.m_dbr );
								else
									brnitm_to_tky( ln, m_CREC.m_cbmn, m_CREC.m_cre, m_CREC.m_cbr );
							}
							pDBzm->IsModify() = TRUE;

							CString kmkcd;
							int brn;
							if (!DCflg) {
								kmkcd = m_CREC.m_dbt;
								brn = m_CREC.m_dbr;
							}
							else {
								kmkcd = m_CREC.m_cre;
								brn = m_CREC.m_cbr;
							}

							if (pDBzm->IsDiffEdabanSyzSgn((char*)(LPCTSTR)kmkcd, brn)) {
								if (pDBsy->SyReOpen(pDBzm) == -1) {
									ermset(0, "消費税モジュール再オープンに失敗しました。");
									return;
								}
								//枝番なしでイニシャライズしてから、枝番をセットして消費税属性を得る
								int neweda;
								if (!DCflg) {
									neweda = m_CREC.m_dbr;
									m_CREC.m_dbr = -1;
								}
								else {
									neweda = m_CREC.m_cbr;
									m_CREC.m_cbr = -1;
								}

								DB_SyIniz(&m_CREC);
								if (!DCflg) {
									m_CREC.m_dbr = neweda;
								}
								else {
									m_CREC.m_cbr = neweda;
								}
								//消費税
								sprintf_s(getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, "%s", sy_chk_txt(&m_CREC, &smp, acINP_mode));
								//消費税再表示
								int pntbl[] = { AC_ZEI_PN, AC_SYZ_PN };
								// 05.22 /08
								::ZeroMemory(zei, sizeof zei);
								l_input(zei, (char*)(LPCTSTR)smp.SY_MSG_SYZEI);	// 税額をセット
								memcpy(getLINE_DATA()->drec.m_zei, zei, 6);
								memmove(m_CREC.m_zei, zei, sizeof(zei));

								if (is_syohizeisiwake(&m_CREC)) {
									getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = TRUE;
								}
								else {
									getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = FALSE;
								}
								TerminationDataSet(pntbl, 2);
							}
						}
						else if( st == -1 )				//キャンセル
						{
							del_dtaeda( &m_CREC, DCflg );			//仕訳枝番消去
						}
					}
					else if( st == 0 ) {
						// ゼロ登録
						pDBzm->IsModify() = TRUE;
					}
					else if( st == 2 || st == 3 ) {	
						// 枝番無しか登録MAXなので枝番名称を表示しない
					}

					if( st )
					{
						if( ! DCflg ) {
							if( getLINE_DATA()->drec.m_dbr != m_CREC.m_dbr ) {
								// 入力枝番タイプOFFセット
								getLINE_DATA()->lcnd[ AC_KMK_PN ].INP_type = brn_dfltinptype( m_CREC.m_dbr );
							}
							getLINE_DATA()->drec.m_dbr = m_SREC.m_dbr = m_CREC.m_dbr;	// 0xffff
						}
						else {
							if( getLINE_DATA()->drec.m_cbr != m_CREC.m_cbr ) {
								// 入力枝番タイプOFFセット
								getLINE_DATA()->lcnd[ AC_KMK_PN ].INP_type = brn_dfltinptype( m_CREC.m_cbr );
							}
							getLINE_DATA()->drec.m_cbr = m_SREC.m_cbr = m_CREC.m_cbr;
						}
					}

				}
			}
			PROGRAM_OK = TRUE;

		}


		::ZeroMemory( &check, sizeof check );
		ldata = getLINE_DATA();

		if( ! DCflg ) {
			memcpy( check.kd_code, ldata->drec.m_dbt, KMKCD_LN );
			check.kd_eda = ldata->drec.m_dbr;
			pKn = pDBzm->DB_PjisToKnrec( ldata->drec.m_dbt );
		}
		else {
			memcpy( check.kd_code, ldata->drec.m_cre, KMKCD_LN );
			check.kd_eda = ldata->drec.m_cbr;
			pKn = pDBzm->DB_PjisToKnrec( ldata->drec.m_cre );
		}
		if( pKn ) {
			KamokuString( pKn, check.kd_name, sizeof check.kd_name );
		//	sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", pKn->knnam );
		}
		else	::ZeroMemory( check.kd_name, sizeof check.kd_name );
		
		DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, pDBzm );
		// 資金繰諸口枝番等セット
		set_etcdsp( ln, AC_KMK_PN );

		if( nChar != VK_NEXT && nChar != VK_PRIOR ) // for page down speed up
			TerminationDataSet( AC_KMK_PN );

//		return;
//	}

NEXT:
	getLINE_DATA()->lcnd[AC_KMK_PN].NEXT_MOVE_KEY = FALSE;

	if( !nChar )
	{
		return;
	}

	if (nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT))
		nChar = VK_F2;

	if( nChar )
	{
		nChar = SelItemDsp( nChar );

		if( !nChar )
			return;

		try {
			nChar = dline_chg( nChar );
			if( nChar == -1 )
			{
	//			MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合は
				if( isdeldata() )
				{
					set_focus( AC_DATE_PN );
					return;
				}

	//			set_focus( AC_KMK_PN );
				return;
			}
		}
		catch ( CErrBlk E )
		{
			ErrExit( E.ErrCode, E.ErrMsg );
			return;
		}
	}

	if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT || nChar == VK_F3 )
	{
		DBkamokudata_get( this, IDC_ICSDBEDTCTRL1, &check, pDBzm );
		int bmn_code = -1;

		if( dcsw == 1 ) {
			eda_code = getLINE_DATA()->drec.m_dbr;
			bmn_code = getLINE_DATA()->drec.m_dbmn;
		}
		else {
			eda_code = getLINE_DATA()->drec.m_cbr;
			bmn_code = getLINE_DATA()->drec.m_cbmn;
		}
		
		// 枝番摘要検索＆表示
		if( (nChar == VK_TAB || (nChar == VK_RETURN && eda_code == -1 )) && modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
		{
			if( check.kd_code[0] != 0 ) {
				kmkcd_str = check.kd_code;
			}
			else {
				int n;
				n = GetRecordDCsgn( &m_SREC );

				if( n == 0 || n == -1 ) {
					kmkcd_str = m_SREC.m_cre;
					check.kd_eda = m_SREC.m_cbr;
				}
				else {
					kmkcd_str = m_SREC.m_dbt;
					check.kd_eda = m_SREC.m_dbr;
				}

				if( pAUTOSEL->BRCPY_OPT == OPT_NONOPT ) {	// 10.24 /11
					check.kd_eda = -1;
				}

				memcpy( check.kd_code, kmkcd_str, KMKCD_LN );

				// 現在の業種でない科目の 空うちEnter での 入力をチェックする。
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

				pKn = pDBzm->DB_PjisToKnrec( check.kd_code );
				if( pKn ) {
					KamokuString( pKn, check.kd_name, sizeof check.kd_name );
				//	sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", pKn->knnam );
				}
				else	::ZeroMemory( check.kd_name, sizeof check.kd_name );
	
				DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, pDBzm );
			}

			// 枝番摘要を表示
		//	BRNTKY_PAR bpar;
		//	bpar = m_Seldata.GetBrnTkyPar();
		//	if( /*!BRNTKY_sel ||*/ kmkcd_str.CompareNoCase( bpar.bt_code ) != 0 ) {
			if( EdabanDispReload( kmkcd_str ) ) {

				if( BRNTKY_select(bmn_code, kmkcd_str, NULL, nChar == VK_TAB ? TRUE : FALSE ) )
				{
					::ZeroMemory( saveITEM50kana, sizeof saveITEM50kana );
					if( nChar == VK_RETURN )	nChar = VK_TAB;
				}
				else {
					nChar = VK_RETURN;
				}
			}
			else {
				nChar = VK_RETURN;
			}
		}
		if( ! check.kd_code[0] )
		{
			Buzzer();
			return;
		}

		if( eda_code == -1 ) {
			if( pAUTOSEL->BRNKAK_OPT == OPT_BRNKAK ) {
				// 枝番残高があるか？
				if( pDBzm->DB_EdabanZanSearch( CString(check.kd_code), -1 ) == 0 )
					return;
			}
		}

		if( nChar == VK_TAB && ! modefy_ok( acINP_mode, &getLINE_DATA()->drec ) ) {
			nChar = VK_RETURN;
		}

		getLINE_DATA()->lcnd[AC_KMK_PN].NEXT_MOVE_KEY = TRUE;

		if( /*!is_AutoInp_use() &&*/ nChar != VK_TAB ) {
			set_termfocus( get_nowln(), AC_KMK_PN, 1 );
		}
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 )
	{
		set_termfocus( get_nowln(), AC_KMK_PN, -1 );
	}	
	else {
		if( nChar != 0x16 )
			Buzzer();
	}

}


//---------------------------------------------------------
//	科目が借／貸どちらかを取得
//	return 0 = 借方, 1 = 貸方, -1 = 科目属性に従う
//
//---------------------------------------------------------
int CDBAbookView::GetKamokuSign() 
{
	int ret = -1;
	ACBOOK_LINE* al;
	al = getLINE_DATA();

	if( acINP_mode == _APPEND ) {
		// 新規追加
		if( al->lcnd[AC_KMK_PN].INP_sg ) {
			if( al->lcnd[AC_NKIN_PN].INP_sg ) {
				ret = 1;
			}
			else if( al->lcnd[AC_SKIN_PN].INP_sg ) {
				ret = 0;
			}
		}
	}
	else {
		int dcsw;
		dcsw = GetRecordDCsgn( &al->drec );

		// 修正
		if (dcsw == -1) {	//入力金額の種類で判定
			if (al->lcnd[AC_NKIN_PN].INP_sg) {
				ret = 1;
			}
			else if (al->lcnd[AC_SKIN_PN].INP_sg) {
				ret = 0;
			}
			else {
				ret = 1;
			}
		}
		else if( dcsw == 0 ) {
			ret = 1;
		}
		else {
			ret = 0;
		}
	}

	return ret;
}


//---------------------------------------------------------
//	科目コード、枝番をセットする
//	CDBINPDataRec* rec	セットするレコード
//	DBKNREC*	pKn		セットする科目コードを含んだ名称
//	int			eda		セットする科目枝番
//
//---------------------------------------------------------
void CDBAbookView::SetAcbookKamoku(CDBINPDataRec* rec, struct _KamokuData* pKm )
{

	int sgn = GetKcodeDCsgn( pKm->kd_code );

	if( sgn != -1 ) {
		if( sgn ) {	// 入金
			rec->m_cre = pKm->kd_code;
			rec->m_cbr = pKm->kd_eda;
#ifdef CLOSE
		//	仕訳ごとに入力させる
			if( rec->m_cbmn == -1 )
				rec->m_cbmn = m_BaseBmn;
			if( rec->m_ckno.IsEmpty() )
				rec->m_ckno = m_BaseKoji;
#endif
			rec->m_dbt = m_BaseCode;
			rec->m_dbr = m_BaseEda;
			if( m_BaseBmn != -1 )
				rec->m_dbmn = m_BaseBmn;
			if( ! m_BaseKoji.IsEmpty() )
				rec->m_dkno = m_BaseKoji;
		}
		else {		// 出金
			rec->m_dbt = pKm->kd_code;
			rec->m_dbr = pKm->kd_eda;
#ifdef CLOSE
		//	仕訳ごとに入力させる
			if( rec->m_dbmn == -1 )
				rec->m_dbmn = m_BaseBmn;
			if( rec->m_dkno.IsEmpty() )
				rec->m_dkno = m_BaseKoji;
#endif
			rec->m_cre = m_BaseCode;
			rec->m_cbr = m_BaseEda;
			if( m_BaseBmn != -1 )
				rec->m_cbmn = m_BaseBmn;
			if( ! m_BaseKoji.IsEmpty() )
				rec->m_ckno = m_BaseKoji;
		}
	}
	else {
		::ZeroMemory( pKm, sizeof(struct _KamokuData) );
	}
}

//--------------------------------------------
//	入金・出金への振りわけサイン取得
//	dcsw	0: 出金
//			1: 入金
//--------------------------------------------
int CDBAbookView::GetKcodeDCsgn( LPCTSTR code )
{
	DBKNREC* pKn;

	pKn = pDBzm->DB_PjisToKnrec( code );
	int dcsw = -1;
	DWORD tmp, kncod, dwCode;

	if( pKn ) {
		if( (pKn->knatt & 0x01) ) {	// 科目属性で、入金・出金 振り分ける
			dcsw = 1;	//入金
		}
		else {
			dcsw = 0;	//出金
		}

		tmp = _atoh( (char*)(LPCTSTR)m_BaseCode );
//		hl_rev( &tmp, 4 );
		kncod = pKn->kncod;
		hl_rev( &kncod, 4 );

		DBGKREC* pGk;

		//小分類拡張のとき、拡張元取得
		if( (tmp & 0x0000f000) >= 0x00008000 ) {
			dwCode = (tmp & 0xffffff00);
			hl_rev( &dwCode, 4 );
			pGk = pDBzm->get_gksrc( dwCode );
			tmp = pGk->gksrc;
			hl_rev( &tmp, 4 );
		}
		if( (kncod & 0x0000f000) >= 0x00008000 ) {
			dwCode = (kncod & 0xffffff00);
			hl_rev( &dwCode, 4 );

			pGk = pDBzm->get_gksrc( dwCode );
			kncod = pGk->gksrc;
			hl_rev( &kncod, 4 );
		}

		if( (m_dwOpt&0xf0) == 0 ) {
#ifndef _NEWCLOSE

			if( (tmp & 0xffff0000) < 0x08000000 ) {	//基本科目 が 貸借
				if( IsGenkin( Voln1, tmp ) ) {	//基本科目が現金
					if( IsUrikake( Voln1, kncod )) {
						// 売掛金等、入金として判定
						dcsw = 1;
					}
					else if( IsKaikake( Voln1, kncod )) {
						// 買掛金等、出金として判定
						dcsw = 0;
					}
				}
				else if( IsUrikake( Voln1, tmp )) {	//基本科目が売掛金等
					if( IsGenkin( Voln1, kncod )) {
						// 出金として判定
						dcsw = 0;
					}
				}
				else if( IsKaikake( Voln1, tmp )) {	//基本科目が買掛金等
					if( IsGenkin( Voln1, kncod )) {
						// 入金として判定
						dcsw = 1;
					}
				}
				else if( IsKaribarai( tmp )) {		//基本科目が仮払消費税
					if( IsKariuke( kncod )) {
						dcsw = 0;	// 出金として判定
					}
					else {
						dcsw = 1;	// 入金として判定
					}
				}
				else if( IsKariuke( tmp )) {		//基本科目が仮受消費税
					if( IsKaribarai( kncod )) {
						dcsw = 1;	// 入金として判定
					}
					else {
						dcsw = 0;	// 出金として判定
					}
				}
				else if( IsBaseCheckKamoku( Voln1, tmp ) ) {//相手が現金等で 入金等すべき科目か？
					if( IsGenkin( Voln1, kncod )) {
						// 入金として判定
						dcsw = 1;
					}
				}
				else {
					if( Voln1->apno == 0x10 ) {
						if( (tmp&0xffffff00) == 0x03010400 ) {		//基本科目が事業主貸
							if( (kncod&0xffffff00) == 0x07020500 ) {//  相手  が事業主借
								dcsw = 0;	// 出金として判定
							}
							else {
								dcsw = 1;	// 入金として判定
							}
						}
						else if( (tmp&0xffffff00) == 0x07020500 ) {	//基本科目が事業主借
							if( (kncod&0xffffff00) == 0x03010400 ) {//  相手  が事業主借
								dcsw = 1;	// 入金として判定
							}
							else {
								dcsw = 0;	// 出金として判定
							}
						}
					}
				}
			}
			else {
				//基本科目 が 損益科目
				DBKNREC* pBaseKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
				if( (pBaseKn->knatt & 0x01) ) {	// 売上等 営業外収入 →出金欄へ
					dcsw = 0;
				}
				else {
					dcsw = 1;					// 仕入・経費		 →入金欄へ
				}
			}
#else
			if( (tmp & 0xffff0000) < 0x08000000 ) {	//基本科目 が 貸借
				// 相手科目が 貸借科目
				if( (kncod & 0xffff0000) < 0x08000000 ) {
					if( (pKn->knatt & 0x01) ) {	// 負債等貸方科目は 出金
						dcsw = 0;
					}
					else {
						dcsw = 1;				// 資産等 借方科目は入金
					}
				}
			}
			else {
				//基本科目 が 損益科目
				DBKNREC* pBaseKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
				if( (pBaseKn->knatt & 0x01) ) {	// 売上等 営業外収入 →出金欄へ
					dcsw = 0;
				}
				else {
					dcsw = 1;					// 仕入・経費		 →入金欄へ
				}
			}
#endif
		}
		else if( (m_dwOpt&0xf0) == 0x10 ) {
			dcsw = 1;				// 入金固定
		}
		else {
			dcsw = 0;				// 出金固定
		}

#ifdef CLOSE
		// 基本科目が、現金の場合
		if( (tmp & 0xffff0000) < 0x01040000 ) {
			if( (kncod & 0xffff0000) == 0x01040000 ) {
				// 売掛金等、入金として判定
				dcsw = 1;
			}
			else {
				if( pDBzm->zvol->apno < 0x50 ) {
					if( (kncod & 0xffff0000) >= 0x04010000 && (kncod & 0xffffff00) <= 0x04050500 
						&& (kncod & 0xffffff00) != 0x04030400) {
						// 買掛金・預かり金等、出金として判定
						dcsw = 0;
					}
				}
				else {
					if( (kncod & 0xffff0000) >= 0x04010000 && (kncod & 0xffffff00) <= 0x04010600 ) {
						// 買掛金・預かり金等、出金として判定
						dcsw = 0;
					}
				}
			}
		}
		else {
			//基本科目が買掛金・預かり金等のとき
			if( pDBzm->zvol->apno < 0x50 ) {
				if( (tmp & 0xffff0000) >= 0x04010000 && (tmp & 0xffffff00) <= 0x04050500 
					&& (tmp & 0xffffff00) != 0x04030400) {
					if( (kncod & 0xffff0000) < 0x01040000 ) {
						// 現金等、入金として判定
						dcsw = 1;
					}
				}
			}
			else {
				if( (tmp & 0xffff0000) >= 0x04010000 && (tmp & 0xffffff00) <= 0x04010600 ) {
					if( (kncod & 0xffff0000) < 0x01040000 ) {
						// 現金等、入金として判定
						dcsw = 1;
					}
				}
			}
		}
#endif
	}

	return dcsw;
}


/***************************
	カナチェック
****************************/
void CDBAbookView::KanaTermKamoku(LPCTSTR data) 
{
	char kana[20] = {0};
	char k_data[20] = {0};

	if( KanaSearchType() )
		return;

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
		sprintf_s( par.brn_kana, sizeof par.brn_kana, "%s%s", saveITEM50kana, k_data );
	}
	else if( m_SelDispPN == SL_KMKANA_PN ) {
		par.sel_pn = SL_KMKANA_PN;
#ifdef CLOSE
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


void CDBAbookView::FocusIcsTekiyo( int ln, short click )
{
	if( PROGRAM_OK == FALSE )
		return;

	//別画面入力消去
	try {
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}

		int prev_ln = get_nowln();
		int prev_pn = get_nowpn();

		if( _dline_chg( ln, AC_TKY_PN, click ? 0 : 1 ) == -1 )
			return;

		// 取消データの場合は
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return;
		}
		if( data_denpdialog( ln ) != 0 )
			return;

		//直前のポジションが当ポジションより前で、金額が入力されていない場合は直前のポジションに返す。
	#ifdef CLOSE
		int set_pn;
		if( inp_ok( get_nowpn(), AC_TKY_PN, &set_pn ) == FALSE )
		{
			set_nowpn( set_pn );
			set_focus( /*get_nowpn()*/set_pn );
			return;
		}
	#endif

	#ifdef CLOSE	// 06.28 /00
		m_TKsyz = 0;	//適用レコード消費税コード
	#endif

//MyTrace( "FocusIcsTekiyo prev_ln = %d, ln = %d\n", prev_ln, ln );

		if( get_nowpn() != AC_TKY_PN || prev_ln != ln || m_bTkyLineChg )	//行替わりで摘要表示更新
		{
			SelPar sel;
			// 自動仕訳で、科目未登録
			::ZeroMemory( sel.tky_kana, sizeof sel.tky_kana );
			if( (acINP_mode == _APPEND && m_bAutoSwk) && ! getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg ) {
				sel.sel_pn = SL_TKYALL_PN;
				sel.dbt.Empty();
				sel.cre.Empty();
				// 摘要選択を表示
				m_Seldata.SelectJob( &sel, -1);
			}
			else {
			//	sel.sel_pn = SL_TKY_PN;
				if( acINP_mode == _APPEND && m_bAutoSwk ) {
					sel.sel_pn = SL_TKYALL_PN;
				}
				else {
					sel.sel_pn = SL_TKY_PN;
				}
				sel.dbt = getLINE_DATA()->drec.m_dbt;
				sel.cre = getLINE_DATA()->drec.m_cre;
				// 摘要選択を表示
				m_Seldata.SelectJob( &sel, -1);
				if( acINP_mode == _APPEND && m_bAutoSwk ) {
					m_Seldata.SelectJob( &sel, VK_HOME);
				}
			}
		}
		m_bTkyLineChg = FALSE;
		
		set_nowpn( AC_TKY_PN );
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


void CDBAbookView::TerminationTekiyo( int ln, long nChar, long inplen, long kst )
{
	VARIANT var;
	char	buf[128];
	BOOL	del_move = FALSE;

//MyTrace( "TerminationTekiyo (%d)\n", PROGRAM_OK );
	int i, first = 0;

	if( PROGRAM_OK == FALSE )
		return;

	int del_pos = 0;

	if( nChar == VK_DELETE ) {
		del_pos = m_Input.GetCaretPosition();
	}

	int pntbl[] = {
		AC_SYZ_PN, AC_TKY_PN
	};
//	if( !nChar )
//	{

		DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
		if( var.pbVal != NULL ) {
			strcpy_s( buf, sizeof buf, (const char*)var.pbVal );
			teki_stringcheck( buf, Voln1->tk_ln * 2 );
		}
		else {
			ZeroMemory( buf, sizeof buf );
		}
	
		if( !modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
		{

			if( strcmp( getLINE_DATA()->drec.m_tekiyo, buf ) )
			{
				Buzzer();
			}

			strcpy_s( buf, sizeof buf, getLINE_DATA()->drec.m_tekiyo );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
			TerminationDataSet( AC_TKY_PN );
		}
		else
		{
			if( getLINE_DATA()->lcnd[AC_TKY_PN].INP_sg == FALSE && ! buf[0] )
			{
				// 11.24 /10
				if( nChar == VK_RETURN && acINP_mode == _APPEND ) {
					i = get_inpl( (unsigned char*)(LPCTSTR)m_SREC.m_tekiyo, Voln1->tk_ln );
					if( i > 0 )	{
						strcpy_s( buf, sizeof buf, m_SREC.m_tekiyo );
						first = 1;
					}
				}
				if( ! m_CREC.m_tekiyo.IsEmpty() ) {
					m_CREC.m_tekiyo.Empty();
					getLINE_DATA()->drec.m_tekiyo.Empty();
					TerminationDataSet( AC_TKY_PN );
				}

				if( getLINE_DATA()->lcnd[AC_TKY_PN].NEXT_MOVE_KEY == FALSE )
//					return;
					goto NEXT;

				strcpy_s( buf, sizeof buf, m_SREC.m_tekiyo );
			}

			// イメージ摘要以外
			if( getLINE_DATA()->lcnd[ AC_TKY_PN ].IMG_sg != 1 ) {
				m_SREC.m_tekiyo = buf;
				m_CREC.m_tekiyo = buf;

				getLINE_DATA()->drec.m_tekiyo = buf;
				getLINE_DATA()->lcnd[AC_TKY_PN].INP_sg = TRUE;
			}

			//消費税
			struct _SY_MSG_PACK2 smp;
			strcpy_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, sy_chk_txt( &m_CREC, &smp, acINP_mode) );
			//消費税再表示
#ifdef CLOSEKEEP
			char zei[6] = {0};
			l_input( zei, (char*)(LPCTSTR)smp.SY_MSG_SYZEI );	// 税額をセット
			memcpy( getLINE_DATA()->drec.m_zei, zei, 6 );
			memmove( m_SREC.m_zei, zei, sizeof(m_SREC.m_zei) );
			memmove( m_CREC.m_zei, m_SREC.m_zei, sizeof(m_SREC.m_zei) );

			getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = TRUE;
			TerminationDataSet( AC_ZEI_PN );
#endif

			int pos = m_Input.GetCaretPosition();
			::ZeroMemory( buf, sizeof buf );
			strcpy_s( buf, sizeof buf, getLINE_DATA()->drec.m_tekiyo );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
			m_Input.SetCaretPosition(pos);

			TerminationDataSet( pntbl, 2 );
		}
//		return;
//	}
NEXT:
	getLINE_DATA()->lcnd[AC_TKY_PN].NEXT_MOVE_KEY = FALSE;
		
	if( !nChar )
		return;

	if (nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT))
		nChar = VK_F2;

	if( nChar )
	{

		nChar = SelTkyDsp( nChar );

		if( !nChar )
			return;

		try {
			if( nChar == VK_UP || nChar == VK_DOWN ) {
				m_bTkyLineChg = TRUE;
			}

			nChar = dline_chg( nChar );
			if( nChar == -1 )
			{
	//			MessageBeep( 0xffffffff );
				m_bTkyLineChg = FALSE;
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合は
				if( isdeldata() )
				{
					m_bTkyLineChg = FALSE;
					set_focus( AC_DATE_PN );
					return;
				}

				return;
			}
		}
		catch ( CErrBlk E )
		{
			ErrExit( E.ErrCode, E.ErrMsg );
			return;
		}
	}

	if( nChar == VK_RETURN || nChar == VK_TAB /*|| nChar == VK_RIGHT || nChar == VK_F3*/ )
	{
		getLINE_DATA()->lcnd[AC_TKY_PN].NEXT_MOVE_KEY = TRUE;

		int movesgn = 0;
		if( nChar == VK_RETURN ) {
			if( ! first  ){
				movesgn = 1;
			}
			else {
				var.pbVal = (BYTE*)buf;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

				// 空打ち複写の時、摘要消費税のチェックをする。文字列が一致しない場合はクリア [01.07 /21]
					CString stmp;
					stmp = buf;
			//	if (m_TKattrec.m_tksgn[1] == m_TKsyz2) { //文字入力が保存摘要と異なる場合はクリアする 04.20/25
					if (stmp.Find(m_TKattrec.m_tkname) < 0) {
						m_TKsyz2 = 0;
						m_TKattrec.Reset();
					}
			//	}

				//摘要欄で消費税セット[10.28 /11]
				if( acINP_mode == _APPEND && (m_TKsyz2 || pDBzm->CheckTkrec(&m_TKattrec)) && pDBsy->IsSyohizeiMaster() )
				{
					pDBzm->SetCDBData( &m_CREC );
					sy_chk_txt( &m_CREC, NULL, acINP_mode);

					_SY_TKREC_ syTkrec = { 0 };
					pDBzm->SetSyTkrec(&syTkrec, &m_TKattrec, &m_CREC);
					pDBsy->Sy_tkyo_inp(&syTkrec);
					m_CREC.m_dsign.Copy( pDBzm->dbdata->dsign );
					l_input( m_CREC.m_zei, (char*)(LPCTSTR)pDBzm->dbdata->zei );
					//消費税
					getLINE_DATA()->drec.m_dsign.Copy( m_CREC.m_dsign );
					//登録番号
					if( pDBzm->is_invnosiwake(&m_CREC) ) {
						//未入力の場合のみ
						if( m_CREC.m_invno[0] == 0 ) {
							sprintf_s(m_CREC.m_invno, sizeof(m_CREC.m_invno), _T("%s"), pDBzm->dbdata->invno);
							sprintf_s(getLINE_DATA()->drec.m_invno, sizeof(m_CREC.m_invno), _T("%s"), pDBzm->dbdata->invno);

							MyTrace("acbook invno = %s", pDBzm->dbdata->invno);
						}
					}
					strcpy_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, sy_chk_txt( &m_CREC, NULL, acINP_mode) );
					memcpy( getLINE_DATA()->drec.m_zei, m_CREC.m_zei, 6 );

					MyTrace("acbook m_TKattrec = %s, [%s]", m_TKattrec.m_tkname, m_TKattrec.m_tkinvno );

					//消費税再表示
					TerminationDataSet( pntbl, 2 );
				}
			}
		}
		else	movesgn = 1;

		// TAB で１文字コピー
		int cpy_job = 0;
		if( acINP_mode == _APPEND && nChar == VK_TAB ) {
			int prev = get_nowln();
			prev--;
			if( prev > 0 ){
				if( get_inpl( (unsigned char*)(LPCTSTR)m_ALine[prev-1].drec.m_tekiyo, Voln1->tk_ln ) > 0 ) {
					strcpy_s( buf, sizeof buf, m_ALine[prev-1].drec.m_tekiyo );
					if( tekyo_tabcopy( this, IDC_ICSDBEDTCTRL1, buf ) ) {
						cpy_job = 1;
					}
				}
			}
		}

		BOOL bBmon, bKoji;
		bBmon = bKoji = FALSE;
		if( BMON_MST ) {
			if( pAUTOSEL->DSPBMON_OPT == OPT_NONOPT)
				bBmon = TRUE;
		}
		if( M_KOJI ) {
			if( pAUTOSEL->DSPKOJI_OPT == OPT_NONOPT)
				bKoji = TRUE;
		}

		if( ! cpy_job ) {
			if( acINP_mode == _APPEND && m_bAutoSwk ) {	// 自動仕訳で科目未入力の場合
				if( getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg ) {
					if( movesgn ) {
						if( bBmon ) {
							if( getLINE_DATA()->lcnd[AC_BMON_PN].INP_sg ) {
								set_termfocus( get_nowln(), AC_TKY_PN, 1 );
							}
							else {
								set_focus( AC_BMON_PN );
							}
						}
						else if( bKoji ) {
							if( getLINE_DATA()->lcnd[AC_KOJI_PN].INP_sg ) {
								set_termfocus( get_nowln(), AC_TKY_PN, 1 );
							}
							else {
								set_focus( AC_KOJI_PN );
							}
						}
						else {
							set_termfocus( get_nowln(), AC_TKY_PN, 1 );
						}
					}
				}
				else if( nChar == VK_RETURN && first ) {
					m_CREC.m_dbt = m_SREC.m_dbt;
					m_CREC.m_dbr = m_SREC.m_dbr;
					m_CREC.m_cre = m_SREC.m_cre;
					m_CREC.m_cbr = m_SREC.m_cbr;
					getLINE_DATA()->drec.m_dbt = m_SREC.m_dbt;
					getLINE_DATA()->drec.m_dbr = m_SREC.m_dbr;
					getLINE_DATA()->drec.m_cre = m_SREC.m_cre;
					getLINE_DATA()->drec.m_cbr = m_SREC.m_cbr;
					getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg = TRUE;

					int srec_dc = GetRecordDCsgn( &m_SREC );
					if( srec_dc == 0 || srec_dc == -1 ) {
						// 11.08 /12
						if( BMON_MST ) {
							m_CREC.m_dbmn	= m_BaseBmn;
							getLINE_DATA()->drec.m_dbmn	= m_BaseBmn;
						}
						if( M_KOJI ) {
							m_CREC.m_dkno	= m_BaseKoji;
							getLINE_DATA()->drec.m_dkno	= m_BaseKoji;
						}
					}
					else {
						// 11.08 /12
						if( BMON_MST ) {
							m_CREC.m_cbmn	= m_BaseBmn;
							getLINE_DATA()->drec.m_cbmn	= m_BaseBmn;
						}
						if( M_KOJI ) {
							m_CREC.m_ckno	= m_BaseKoji;
							getLINE_DATA()->drec.m_ckno	= m_BaseKoji;
						}
					}

					TerminationDataSet( AC_KMK_PN );
				}
				else if( inplen == 0 ) {
					int fcs = 0;

					if( bBmon ) {
						if( ! getLINE_DATA()->lcnd[AC_BMON_PN].INP_sg ) {
							set_focus( AC_BMON_PN );
							fcs++;
						}
					}
					if( ! fcs ) {
						if( bKoji ) {
							if( ! getLINE_DATA()->lcnd[AC_KOJI_PN].INP_sg ) {
								set_focus( AC_KOJI_PN );
								fcs++;
							}
						}
					}

					if( ! fcs ) {
						set_focus( AC_KMK_PN );
					}
				}
			}
			else {
				if( movesgn )	{
					set_termfocus( get_nowln(), AC_TKY_PN, 1 );
				}
			}
		}
	}
	else if( nChar == VK_DELETE ) {
		DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
		int linelength = 0;
		if( var.pbVal != NULL )	{
			linelength = strlen( (const char*)var.pbVal );
		}

		// イメージ削除
		if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
		{
			switch( getLINE_DATA()->lcnd[ AC_TKY_PN ].IMG_sg )
			{
			case 1:	// イメージ摘要の取消
				memset( buf, '\0', sizeof(buf));
				var.pbVal = (BYTE*)buf;
				DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
				m_SREC.m_dsign[0] &= ~0x80;
				m_CREC.m_dsign[0] &= ~0x80;
				m_SREC.m_imgsq = 0;
				m_CREC.m_imgsq = 0;

				getLINE_DATA()->lcnd[ AC_TKY_PN ].IMG_sg = 2;
				TerminationDataSet( AC_TKY_PN );
				
			//	goto NEXT_POS1;
				break;

			case 2: // イメージ摘要の取消の取消
				if( ! linelength )
				{
					m_SREC.m_tekiyo.Empty();
					m_CREC.m_tekiyo.Empty();
					m_SREC.m_imgsq = getLINE_DATA()->drec.m_imgsq;
					m_CREC.m_imgsq = getLINE_DATA()->drec.m_imgsq;
					m_SREC.m_dsign[0] |= 0x80;
					m_CREC.m_dsign[0] |= 0x80;

					getLINE_DATA()->lcnd[ AC_TKY_PN ].IMG_sg = 1;
					TerminationDataSet( AC_TKY_PN );

			//		goto NEXT_POS2;
				}
			default:	// 通常の摘要の取消
				if( del_pos == 0 && buf[del_pos] == '\0' )	// Del を摘要なしで 押下しは カーソル移動
					del_move = TRUE;

				//カーソルポジション以降の文字列取り消し
				buf[del_pos] = '\0';
				var.pbVal = (BYTE*)buf;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

				m_SREC.m_tekiyo = buf;
				m_CREC.m_tekiyo = buf;

				getLINE_DATA()->drec.m_tekiyo = buf;
				//再表示
				TerminationDataSet( pntbl, 2 );

				if( del_move ) {
					set_termfocus( get_nowln(), AC_TKY_PN, 1 );
				}
				break;
			}
		}
		else
		{
			Buzzer();
			return;
		}
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 ) {
		set_termfocus( get_nowln(), AC_TKY_PN, -1 );
	}
	else if( nChar == VK_RIGHT ) {
		int sub_pn;
		BOOL bOK = modefy_ok( acINP_mode, &getLINE_DATA()->drec );
		if( bCONFIRM_MASTER )	bOK = FALSE;
		sub_pn = m_pSubDtInp->SetCurData( bOK, &m_CREC );
		SubDataInpDisp( sub_pn );

#ifdef VER2_CLOSE
		//付箋のダイアログを表示
		HSEN_DATA crec = {0}, srec = {0};
		crec.hsen_inpsg = TRUE;
		crec.hsen_sel = (m_CREC.m_dsign[7]&0x0f);
		strcpy_s( crec.hsen_cmt, sizeof crec.hsen_cmt, m_CREC.m_tag );

		srec.hsen_sel = (m_SREC.m_dsign[7]&0x0f);
		strcpy_s( srec.hsen_cmt, sizeof srec.hsen_cmt, m_SREC.m_tag );

		m_pHusenInp->SetHusenData( &crec, &srec );

		HusenInpDisp();
#endif
	}
	else {
		if( nChar != 0x16 )
			Buzzer();
	}

}


//	摘要カナターミネーション
//   LPCTSTR data  カナ文字列
//   BOOL bTkcut   TRUE で摘要をセットしない
//
void CDBAbookView::_KanaTermIcsTekiyo(LPCTSTR data, BOOL bTkcut)
{
	int dc_sw;
	CDBipTKREC tkrec;
	struct _ItemData dmy_data = {0};

	if( tky_50kana_chk((char*)data) ) {
		//５０音摘要選択画面に表示
		SelPar par;
		par.sel_pn = SL_TKYKANA_PN;
		strncpy_s( par.tky_kana, sizeof par.tky_kana, m_tky_50kana, sizeof par.tky_kana );
		m_Seldata.SelectJob( &par, -1 );
		// 摘要 カナ検索中
		pDBzm->tkrec_kanamode() = TRUE;
		m_BrTek.set_tkquery( TRUE );
		//５０音摘要があった
		if( m_SelDispPN == SL_TKYKANA_PN )
			return;
	}

	//摘要選択
	if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) && tky_select( &tkrec, (char *)data ) != -1 )
	{
		//摘要toデータ
		dc_sw = tky_select_to_drec( &m_CREC, &tkrec, 1 );
		//表示
		tky_select_dsp( &m_CREC, dc_sw, &tkrec, TRUE, bTkcut );
	}
	else
	{
#ifdef CLOSE	// 10.18 /99
		Buzzer();
#endif
//		data_reply( getINP_CTL( AC_TKY_PN )->IDC_X, -1, &dmy_data, CICSInput );
	}

}

//-----------------------------------------------------------
// 選択摘要toデータ
//   CDBINPDataRec *m_CREC  仕訳レコード
//   CDBipTKREC *ptk     摘要レコード
//
//-----------------------------------------------------------
int CDBAbookView::tky_select_to_drec( CDBINPDataRec *m_CREC, CDBipTKREC *ptk, int kanaterm/*=0*/ )
{
	int dc_sw = 0;

	if( acINP_mode == _APPEND )
	{
		//消費税コードチェック
		if( ptk->m_tksgn[1] )
		{
			m_TKsyz = getLINE_DATA()->lcnd[ AC_TKY_PN ].TKattr = ptk->m_tksgn[1];	//消費税コード
		}
		//自動仕訳
		if( m_bAutoSwk && (m_AutoSwkCnt  == 0 && m_AutoSwkTrn == 0) ) {

			ICS_TKREC_PAC tkpac;
			int attr;
			attr = 0x13;
			ICS_ACBOOK_PAC	acpac;
			acpac.bmn = m_BaseBmn;
			acpac.kno = m_BaseKoji;
			acpac.kmk = m_BaseCode;
			acpac.eda = m_BaseEda;

			m_tkjourAry.RemoveAll();
			m_AutoSwkCnt = m_AutoSwkTrn = 0;

			int jcnt;
			jcnt = pTkjour->SelectTkjourCount_Acbook(ptk->m_tkcod, attr, acpac);
//MyTrace("ptk->m_tkcod = %d, jcnt = %d\n", ptk->m_tkcod, jcnt);

			if( jcnt == 0 )	return -1;

			//摘要が空白時は、自動仕訳科目をセットする
			m_bAutoKmkSet = FALSE;
			if( get_nowpn() == AC_TKY_PN ) {
				VARIANT var;

				DBdata_get(this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0);
				int linelength = 0;
				if( var.pbVal != NULL ) {
					linelength = strlen((const char*)var.pbVal);
				}
				if( linelength == 0 ) {
					m_bAutoKmkSet = TRUE;
				}
			}
			//摘要文字入力ありで科目セット済みの場合は、自動仕訳しない。
			if( !m_bAutoKmkSet ) {
				if( getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg )
					return-1;
			}

			m_autoTKrec = *ptk;
			PostMessage(WM_DBDINPMSG, DWP_TKYSWKDLG, jcnt);
			return -1;
		}

		m_TKsyz2 = 0;	// 前行分消費税コードクリア
	}

	dc_sw = tky_select_to_drec_sub(m_CREC, ptk );

#ifdef CLOSE
	//摘要枝番を検索
	if( Voln1->tk_br && BRmst && (get_nowpn() == AC_TKY_PN) )
	{
		dc_sw = tkybrn_search( m_CREC, ptk->m_tkcod, &getLINE_DATA()->lcnd[0] );
	}

	// 摘要消費税 処理
	if( acINP_mode == _APPEND && m_TKsyz && pDBsy->IsSyohizeiMaster() )
	{
		pDBzm->SetCDBData( m_CREC );
		sy_chk_txt( m_CREC, NULL, acINP_mode);

		pDBsy->Sy_tkyo_inp( m_TKsyz );
		m_CREC->m_dsign.Copy( pDBzm->dbdata->dsign );
		l_input( m_CREC->m_zei, (char*)(LPCTSTR)pDBzm->dbdata->zei );
		//消費税
		getLINE_DATA()->drec.m_dsign.Copy( m_CREC->m_dsign );
		strcpy_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, sy_chk_txt( m_CREC, NULL, acINP_mode) );
		memcpy( getLINE_DATA()->drec.m_zei, m_CREC->m_zei, 6 );
		//消費税再表示
		int pntbl[] = {AC_SYZ_PN, AC_ZEI_PN};
		TerminationDataSet( pntbl, 2 );

		// 10.28 /11
		m_TKsyz2 = m_TKsyz;
		m_TKattrec = *ptk;

		m_TKsyz = 0;
	}
#endif

	return dc_sw;
}


//------------------------------------------------------------------------
//	摘要を選択した後の摘要枝番・摘要消費税の処理
//	crec	仕訳レコード
//	ptk		摘要レコード
//	nowln	現在行位置
//  tkbrn	1 = 摘要枝番処理必ずする。
//返送値：摘要枝番を借方(D0),貸方(D1)にセットした場合、各ビットを立てる
//------------------------------------------------------------------------
int CDBAbookView::tky_select_to_drec_sub(CDBINPDataRec *crec, CDBipTKREC* ptk, int nowln/*=-1*/,int tkbrn/*=0*/)
{
	int dc_sw = 0;
	//摘要枝番を検索

	int do_tkbrn = 0;
	if( (get_nowpn() == AC_TKY_PN) || tkbrn )
		do_tkbrn = 1;

	ACBOOK_LINE* aline;
	if( nowln == -1 ) {
		aline = getLINE_DATA();
	}
	else {
		aline = _getLINE_DATA(nowln);
	}

	if( Voln1->tk_br && BRmst && do_tkbrn )
	{
		dc_sw = tkybrn_search(crec, ptk->m_tkcod, &aline->lcnd[0]);
	}

	// インボイス対応
	// 摘要消費税 処理
	// Voln1->s_sgn3 : 0x02 ON → 摘要属性登録
	//if( acINP_mode == _APPEND && m_TKsyz && pDBsy->IsSyohizeiMaster() && (Voln1->s_sgn3 & 0x02) )
	if( acINP_mode == _APPEND && ( m_TKsyz || pDBzm->CheckTkrec( ptk )) && pDBsy->IsSyohizeiMaster() /*&& (Voln1->s_sgn3 & 0x02)*/ )
	{
		pDBzm->SetCDBData(crec);
		sy_chk_txt(crec, NULL, acINP_mode);

		//pDBsy->Sy_tkyo_inp(m_TKsyz);
		_SY_TKREC_ syTkrec = { 0 };
		pDBzm->SetSyTkrec( &syTkrec, ptk, crec );
		pDBsy->Sy_tkyo_inp( &syTkrec );

		sprintf_s( crec->m_invno, sizeof( crec->m_invno ), _T( "%s" ), pDBzm->dbdata->invno );
		crec->m_dsign.Copy(pDBzm->dbdata->dsign);
		l_input(crec->m_zei, (char*)(LPCTSTR)pDBzm->dbdata->zei);
		//消費税
		aline->drec.m_dsign.Copy(crec->m_dsign);
		strcpy_s(aline->syzstr, sizeof aline->syzstr, sy_chk_txt(crec, NULL, acINP_mode));
		memcpy(aline->drec.m_zei, crec->m_zei, 6);
		if( pDBzm->is_invnosiwake(crec) ) {
			//未入力の場合のみ
			if( crec->m_invno[0] == 0 ) {
				sprintf_s(crec->m_invno, sizeof(crec->m_invno), _T("%s"), pDBzm->dbdata->invno);
			}
		}

		//消費税再表示
		int pntbl[] = { AC_SYZ_PN, AC_ZEI_PN };
		TerminationDataSet(pntbl, 2, nowln);

		// 10.28 /11
		m_TKsyz2 = m_TKsyz;
		m_TKattrec = *ptk;

		m_TKsyz = 0;
	}
	else {
		m_TKattrec.Reset();
		m_TKsyz = 0;
		m_TKsyz2 = 0;
	}

	return dc_sw;
}


//摘要自動仕訳から科目・枝番・部門・工事を取得
int CDBAbookView::GetAutoSwkItem(ICS_TKJOUR_PAC* pJour, CString& code, int& eda, int& bmn, CString& kno)
{
	int sgn = -1;

	if( !pJour->dbt.IsEmpty() || !pJour->cre.IsEmpty() ) {
		if( !pJour->dbt.IsEmpty() && !pJour->cre.IsEmpty() ) {
			if( _stricmp(pJour->dbt, m_BaseCode) == 0 ) {
				code = pJour->cre;
				eda = pJour->jour_cbr;
				bmn = pJour->cbmn;
				kno = pJour->ckno;
				sgn = 1;
			}
			else if( _stricmp(pJour->cre, m_BaseCode) == 0 ) {
				code = pJour->dbt;
				eda = pJour->jour_dbr;
				bmn = pJour->dbmn;
				kno = pJour->dkno;
				sgn = 0;
			}
			else {
				// 借方／貸方で同じ科目を指定していた場合
				if( _stricmp(pJour->dbt, pJour->cre) == 0 ) {
					code = pJour->dbt;
					eda = pJour->jour_dbr;
					bmn = pJour->dbmn;
					kno = pJour->dkno;
					sgn = GetKcodeDCsgn(code);
				}
			}
		}
		else {
			if( !pJour->dbt.IsEmpty() ) {
				code = pJour->dbt;
				eda = pJour->jour_dbr;
				bmn = pJour->dbmn;
				kno = pJour->dkno;
				sgn = 0;
			}
			else {
				code = pJour->cre;
				eda = pJour->jour_cbr;
				bmn = pJour->cbmn;
				kno = pJour->ckno;
				sgn = 1;
			}

			//片側のみ入力でヘッド部と同じ場合は、何もしない。 --- 06.24 /10
			if( code.CompareNoCase(m_BaseCode) == 0 )
				sgn = -1;
		}
	}
	return sgn;
}

// 自動仕訳のデータをレコードにセット
void CDBAbookView::SetAutoSwkItemToRecord(int sgn, CDBINPDataRec* rec, CString code, int eda, int bmn, CString kno)
{
	if( sgn == 1 ) {	// 入金
		rec->m_dbt = m_BaseCode;
		rec->m_dbr = m_BaseEda;
		// 11.07 /12
		if( BMON_MST ) {
			rec->m_dbmn = m_BaseBmn;
			rec->m_cbmn = bmn;
		}
		if( M_KOJI ) {
			rec->m_dkno = m_BaseKoji;
			rec->m_ckno = kno;
		}
		rec->m_cre = code;
		rec->m_cbr = eda;
	}
	else {				// 出金
		rec->m_dbt = code;
		rec->m_dbr = eda;
		rec->m_cre = m_BaseCode;
		rec->m_cbr = m_BaseEda;
		if( BMON_MST ) {
			rec->m_dbmn = bmn;
			rec->m_cbmn = m_BaseBmn;
		}
		if( M_KOJI ) {
			rec->m_dkno = kno;
			rec->m_ckno = m_BaseKoji;
		}
	}
}


// 自動仕訳行データセット関係
void CDBAbookView::AutoSwkSetJob(CDBINPDataRec *crec, int tkjourseq)
{
	CArray< ICS_TKJOUR_PAC, ICS_TKJOUR_PAC &> tkjourAry;//仕訳内容
	if( pTkjour->GetTkjour(tkjourseq, tkjourAry) == 1 ) {
		m_AutoSwkCnt = tkjourAry.GetCount();
	}
	ICS_TKJOUR_PAC* pJour;
	int sgn = -1;

	BOOL bKmkSet = FALSE;
	//摘要欄が空白のときは、科目も再セットする。
	bKmkSet = m_bAutoKmkSet;

	ACBOOK_LINE* lined = getLINE_DATA();;

	if( !bKmkSet ) {
		if( !lined->lcnd[AC_KMK_PN].INP_sg ) {
			bKmkSet = TRUE;
			m_bAutoKmkSet = TRUE;
		}
	}
	//摘要仕訳無し
	m_tkjourAry.RemoveAll();
	if( m_AutoSwkCnt != 0 ) {
		CString code, kno;
		int	eda = -1;
		int bmn = -1;
		//有効な自動仕訳かをチェック
		for( int n = 0; n < m_AutoSwkCnt; n++ ) {
			pJour = &tkjourAry[n];
			sgn = GetAutoSwkItem(pJour, code, eda, bmn, kno);
			if( sgn == -1 ) {
			}
			else {
				m_tkjourAry.Add(*pJour);
			}
		}
		m_AutoSwkCnt = m_tkjourAry.GetCount();
	}
	if( m_AutoSwkCnt == 0 ) {
		bKmkSet = FALSE;
		m_bAutoKmkSet = FALSE;
	}

	CArith ar(0x16);

	if( bKmkSet ) {
		pJour = &m_tkjourAry[0];
		m_AutoSwkTrn++;
		if( m_AutoSwkCnt == 1 ) {
			m_AutoSwkCnt = 0;
			m_AutoSwkTrn = 0;
		}
		// 摘要仕訳残り数表示
		RDRemainDispJOB();

		lined->lcnd[AC_KMK_PN].Etcmsg.Empty();
		lined->lcnd[AC_KMK_PN].ETC_txtcol = 0;

		char tkyval[6] = { 0 };
		BOOL tkyval_set = FALSE;

		CString code, kno;
		int	eda = -1;
		int bmn = -1;

		sgn = GetAutoSwkItem(pJour, code, eda, bmn, kno);

		if( sgn != -1 ) {
			if( sgn == 0 ) {
				ar.l_input(tkyval, (void*)(LPCTSTR)pJour->dval);
				if( ar.l_test(tkyval) != 0 )	tkyval_set = TRUE;
			}
			else {
				ar.l_input(tkyval, (void*)(LPCTSTR)pJour->cval);
				if( ar.l_test(tkyval) != 0 )	tkyval_set = TRUE;
			}
		}

#ifndef CLOSE	//科目コードの有効性をチェック	11.05 /12
		BOOL bValidCode = FALSE;
		if( !code.IsEmpty() ) {
			DBKNREC* pKn;
			pKn = pDBzm->DB_PjisToKnrec(code);
			if( pKn != NULL ) {
				bValidCode = TRUE;

				// 相手科目に同じ科目が入力されていて、枝番も入力されていた場合、
				// 自動仕訳を行うと枝番が消えるため、同科目の時は、自動仕訳をしない。
				if( lined->lcnd[AC_KMK_PN].INP_sg ) {
					int dcsw = GetRecordDCsgn(&lined->drec);
					if( dcsw == 0 ) {
						if( code.CompareNoCase(lined->drec.m_cre) == 0 ) {
							bValidCode = FALSE;
						}
					}
					else if( dcsw == 1 ) {
						if( code.CompareNoCase(lined->drec.m_dbt) == 0 ) {
							bValidCode = FALSE;
						}
					}
					else {
						if( code.CompareNoCase(lined->drec.m_dbt) == 0 ) {
							bValidCode = FALSE;
						}
					}
				}
			}
		}
		if( bValidCode ) {
#endif
			//	自動仕訳の登録より、入金・出金を判断する
			//	int sgn = GetKcodeDCsgn( code );

			if( sgn != -1 ) {
				SetAutoSwkItemToRecord(sgn, crec, code, eda, bmn, kno);
				SetAutoSwkItemToRecord(sgn, &lined->drec, code, eda, bmn, kno);

				lined->lcnd[AC_KMK_PN].INP_sg = TRUE;
				// 枝番セット済み(摘要枝番しない)
				if( eda != -1 ) {
					lined->lcnd[AC_KMK_PN].INP_type = 1;
					// 枝番等セット
					set_etcdsp(get_nowln(), AC_KMK_PN);
				}

				TerminationDataSet(AC_KMK_PN);

				if( BMON_MST ) {
					if( bmn != -1 ) {
						lined->lcnd[AC_BMON_PN].INP_sg = TRUE;
						TerminationDataSet(AC_BMON_PN);
					}
				}
				if( M_KOJI ) {
					if( ! kno.IsEmpty() ) {
						lined->lcnd[AC_KOJI_PN].INP_sg = TRUE;
						TerminationDataSet(AC_KOJI_PN);
					}
				}

				if( tkyval_set ) {
					memcpy(lined->drec.m_val, tkyval, 6);
					memcpy(crec->m_val, tkyval, 6);
					int val_pn;
					val_pn = (sgn == 1) ? AC_NKIN_PN : AC_SKIN_PN;
					lined->lcnd[val_pn].INP_sg = TRUE;

					//消費税
					struct _SY_MSG_PACK2 smp;
					pDBzm->SetCDBData(crec);
					strcpy_s(lined->syzstr, sizeof lined->syzstr, sy_chk_txt(crec, &smp, acINP_mode));
		
					//消費税再表示
					char zei[6] = { 0 };
					l_input(zei, (char*)(LPCTSTR)smp.SY_MSG_SYZEI);	// 税額をセット
					memcpy(lined->drec.m_zei, zei, 6);
					memcpy(crec->m_zei, zei, sizeof(zei));

					if( is_syohizeisiwake(crec) ) {
						lined->lcnd[AC_ZEI_PN].INP_sg = TRUE;
					}
					else {
						lined->lcnd[AC_ZEI_PN].INP_sg = FALSE;
					}
					int pntbl[] = { val_pn, AC_SYZ_PN, AC_ZEI_PN };
					TerminationDataSet(pntbl, 3);
				}

				if( !pJour->tekiyo.IsEmpty() ) {
					//自動仕訳の摘要がある場合は、選択摘要コードからの名称をセット
				//	lined->drec.m_tekiyo += m_autoTKrec.m_tkname;

					lined->lcnd[AC_TKY_PN].INP_sg = TRUE;
					lined->drec.m_tekiyo += pJour->tekiyo;

					if( get_nowpn() == AC_TKY_PN ) {
						VARIANT var;
						var.pbVal = (BYTE*)(LPCTSTR)lined->drec.m_tekiyo;
						m_Input.SetData(&var, ICSDBEDT_TYPE_STRING, 0);
						short pos = m_Input.GetCaretPosition();
						//データセットのためのダミーフォーカスＯＦＦ
						TerminationTekiyo(get_nowln(), 0, 0, 0);
						m_Input.SetCaretPosition(pos);
					}
				}
				SelPar sel;
				sel.sel_pn = SL_TKYALL_PN;
				sel.dbt = lined->drec.m_dbt;
				sel.cre = lined->drec.m_cre;
				// 摘要選択情報を更新(表示しない)
				m_Seldata.SelectJob(&sel, -2);
			}
		}
	}
	else {
		m_AutoSwkCnt = 0;
		m_AutoSwkTrn = 0;
	}
}


//-----------------------------------------------------------
// 摘要セットデータ表示
//   CDBINPDataRec *m_CREC  仕訳レコード
//	 int dc_sw           借貸 サイン
//   CDBipTKREC *ptk     摘要レコード
//   BOOL F12_KEY        IME のターミネーション
//   BOOL bTkcut         摘要をセットしない(=TRUE)
//   int  nowln			 処理中の行位置
//-----------------------------------------------------------
void CDBAbookView::tky_select_dsp( CDBINPDataRec *m_CREC, int dc_sw, CDBipTKREC *ptk, BOOL F12_KEY, BOOL bTkcut/*=FALSE*/, int nowln/*=-1*/ )
{
	int ln;
	if( nowln != -1 )	ln = nowln;
	else {
		ln = get_nowln();
	}

	ACBOOK_LINE* aline = _getLINE_DATA(ln);
	//摘要枝番表示
	if( dc_sw & 0x01 )
	{
		//借方枝番セット＆表示
		m_SREC.m_dbr = m_CREC->m_dbr;
		aline->drec.m_dbr = m_CREC->m_dbr;
		// 資金繰諸口枝番等セット
		set_etcdsp( ln, AC_KMK_PN );
		TerminationDataSet( AC_KMK_PN, ln );
	}
	if( dc_sw & 0x02 )
	{
		//貸方枝番セット＆表示
		m_SREC.m_cbr = m_CREC->m_cbr;
		aline->drec.m_cbr = m_CREC->m_cbr;
		// 資金繰諸口枝番等セット
		set_etcdsp( ln, AC_KMK_PN );
		TerminationDataSet( AC_KMK_PN, ln );
	}

	//#* 03.14 /02 ... 摘要追加
	if( !bTkcut ) {
		//仕訳摘要表示
		char tkstr[64];
		strcpy_s( tkstr, sizeof tkstr, ptk->m_tkname );

		if( get_nowpn() == AC_TKY_PN ) {
			VARIANT var;
			var.pbVal = (BYTE*)tkstr;
			short pos = m_Input.GetCaretPosition();
			m_Input.InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );
			pos = m_Input.GetCaretPosition();
			//データセットのためのダミーフォーカスＯＦＦ
			TerminationTekiyo( ln, 0, 0, 0 );
			m_Input.SetCaretPosition( pos );
		}
		else {
			// 現レコードにセット
			int len = m_CREC->m_tekiyo.GetLength();
			int byte = (Voln1->tk_ln*2);
			if( len < byte ) {
				m_CREC->m_tekiyo += tkstr;
				len = m_CREC->m_tekiyo.GetLength();
				if( len > byte ) {
					int n = get_strcnt( (char*)(LPCTSTR)m_CREC->m_tekiyo, byte );
					m_CREC->m_tekiyo.SetAt( n, '\0' );
				}
			}
		}
	}
	else {
		// 摘要追加せず
	//	if( F12_KEY )
	//		;
	//		data_reply( getINP_CTL( AC_TKY_PN )->IDC_X, 0, &ins_data, CICSInput );
	}

	//消費税
	strcpy_s( aline->syzstr, sizeof aline->syzstr, sy_chk_txt( m_CREC, NULL, acINP_mode) );
	//消費税再表示
	TerminationDataSet( AC_SYZ_PN, ln );

}


/* -----------------------------
	摘要枝番　サーチ＆セット
------------------------------- */
int CDBAbookView::tkybrn_search( CDBINPDataRec *data, int tkcd, ABLN_CND* cnd )
{
int ret = 0;
CDBINPDataRec sv;
int sv_dbr, sv_cbr, dcsgn;

	sv = *data;

	if( (cnd+AC_KMK_PN)->INP_type != 1 )
	{
		dcsgn = GetRecordDCsgn( data );

		if( dcsgn == 0 || dcsgn == -1 ) {
			dcsgn = 1;	// 相手科目が貸方
			sv_cbr = data->m_cbr;
			data->m_cbr = -1;

			sv_dbr = data->m_dbr;
		}
		else {
			dcsgn = 0;	// 相手科目が借方
			sv_dbr = data->m_dbr;
			data->m_dbr = -1;

			sv_cbr = data->m_cbr;
		}
	}
	else {
		dcsgn = GetRecordDCsgn(data);

		if( dcsgn == 0 || dcsgn == -1 ) {
			dcsgn = 1;	// 相手科目が貸方
		}
		else {
			dcsgn = 0;	// 相手科目が借方
		}
	}

	BOOL bBmn = FALSE;
	if( pAUTOSEL->BMNTKBR_OPT == OPT_CHKON )
		bBmn = TRUE;

	pDBzm->DB_TekiyoEdabanSet( data, tkcd, bBmn );

	if( (cnd+AC_KMK_PN)->INP_type != 1 ) {
		if( ! dcsgn ) {
			if( data->m_dbr != -1 && data->m_dbr != sv.m_dbr )
			{
				(cnd+AC_KMK_PN)->INP_type = 2;	// 自動

				ret |= 0x01;	//借方にセット
			}
			else
				data->m_dbr = sv_dbr;

			data->m_cbr = sv_cbr;
		}
		else {
			if( data->m_cbr != -1 && data->m_cbr != sv.m_cbr )
			{
				(cnd+AC_KMK_PN)->INP_type = 2;	// 自動

				ret |= 0x02;	//貸方にセット
			}
			else
				data->m_cbr = sv_cbr;

			data->m_dbr = sv_dbr;
		}
	}
	else {
		if( !dcsgn ) {
			data->m_cbr = sv.m_cbr;
		}
		else {
			data->m_dbr = sv.m_dbr;
		}
	}

	return ret;
}

//
//	ns_sw = [0] -> 入金, [1] -> 出金
//
int CDBAbookView::FocusIcsVal( int ln, int ns_sw, short click )
{
	if( PROGRAM_OK == FALSE )
		return -1;

	//別画面入力消去
	try {
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}
	
		ACBOOK_LINE* al;
		int setpn, pn, st;
		pn = (ns_sw == 0) ? AC_NKIN_PN : AC_SKIN_PN;

		st = _dline_chg( ln, pn, click ? 0 : 1 );
		if( st == -1 )		return -1;
		else if( st == 1 ) {
			// 新規行の入力に戻るため、ターミネーション無視
			m_bIgnoreTermVal = TRUE;
			return 0;
		}

		// 取消データの場合は
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return -1;
		}
		else {
			if( data_denpdialog( ln ) != 0 )
				return -1;

			if( ! modefy_ok( acINP_mode, &getLINE_DATA()->drec ) ){
				//確定データの場合 金額と反対側は、元の金額へもどす。
				al = getLINE_DATA();
				if( ! al->lcnd[pn].INP_sg ) {
					setpn = (pn == AC_NKIN_PN) ? AC_SKIN_PN : AC_NKIN_PN;
					set_focus( setpn );
					return -1;
				}
			}
			// 入金 および 出金 チェック
			al = getLINE_DATA();

			char* pNKIN = "入金";
			char* pSKIN = "出金";
			CString str;
			int	dcsw;

			int chg = 0;
			if( ! IsSameKamoku() ) {
				if( ns_sw == 0 ) {
					if( al->lcnd[AC_SKIN_PN].INP_sg ) {	// 出金 → 入金
						chg = 1;
						str.Format( "金額を%sから%sに変更します。", pSKIN, pNKIN );
					}
					else {
						if( acINP_mode == _APPEND ) {
							dcsw = GetRecordDCsgn( &al->drec );
							if( dcsw == 1 ) {
								chg = 1;
							}
						}
					}
				}
				else {
					if( al->lcnd[AC_NKIN_PN].INP_sg ) { // 入金 → 出金
						chg = 2;
						str.Format( "金額を%sから%sに変更します。", pNKIN, pSKIN);
					}
					else {
						if( acINP_mode == _APPEND ) {
							dcsw = GetRecordDCsgn( &al->drec );
							if( dcsw == 0 || dcsw == -1 ) {
								chg = 2;
							}
						}
					}
				}
			}

			setpn = (ns_sw == 0) ? AC_NKIN_PN : AC_SKIN_PN;
			if( chg && (acINP_mode != _APPEND) ) {
				PROGRAM_OK = FALSE;
//TRACE("!!++Abook FocusVal 01 -setpn %d, dbdata->seq %d, m_CREC.m_seq %d\n", setpn, pDBzm->dbdata->seq, m_CREC.m_seq );

				int st = myICSMessageBox( str, MB_OKCANCEL );
				
				PROGRAM_OK = TRUE;

				if( st == IDOK ) {	// 入金・出金 の変更
					setpn = (chg == 1) ? AC_NKIN_PN : AC_SKIN_PN;
					ChangeDebtCred(chg);

					struct _SY_MSG_PACK2 smp;
					BYTE oldmen = (m_CREC.m_dsign[6] & 0x20);

					sprintf_s(getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, "%s", sy_chk_txt(&m_CREC, &smp, acINP_mode));

					if( oldmen == 0x20 && !(pDBzm->dbdata->dsign[6] & 0x20) ) {
						// 免税事業者からの仕入れ サインが落ちるので元に戻す
						pDBsy->SyFnc(CD_SP_EXEMPT);
						m_CREC.m_dsign.Copy(pDBzm->dbdata->dsign);
					}
					set_focus( setpn );
				}
				else {
					// TerminationValの先頭で return させるため、 現在ポジションを今回のフォーカスにセット
					setpn = (ns_sw == 0) ? AC_NKIN_PN : AC_SKIN_PN;
					set_nowpn( setpn );
					// 現在の 入・出金にフォーカスを戻す
					setpn = (chg == 1) ? AC_SKIN_PN : AC_NKIN_PN;
					set_focus( setpn );
					return 0;
				}
TRACE("!!++Abook FocusVal 02 -setpn %d\n", setpn );
			}
			else if( chg ) {
				// 新規入力時 は、金額を移動する
				setpn = (chg == 1) ? AC_NKIN_PN : AC_SKIN_PN;
				ChangeDebtCred(chg);
			}

			set_nowpn( setpn );
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}

	return 0;
}



void CDBAbookView::TerminationVal( int ln, int ns_sw, long nChar, long inplen, long kst )
{
	VARIANT var;
	char	ip_val[6];
	int pn, dcsw;
	BOOL	bLineChg = FALSE;
	BOOL	bMoveZei = FALSE;
	pn = (ns_sw == 0) ? AC_NKIN_PN : AC_SKIN_PN;

//MyTrace("!!++Abook TermVal [nChar = %02x] dbt='%s', cre='%s', dbdata->seq %d, m_CREC.m_seq %d\n",	nChar,
//																				getLINE_DATA()->drec.m_dbt, getLINE_DATA()->drec.m_cre,
//																				pDBzm->dbdata->seq, m_CREC.m_seq );

	if( PROGRAM_OK == FALSE )
		return;
	// ターミネーション無視
	if( m_bIgnoreTermVal ) {
		m_bIgnoreTermVal = FALSE;
		return;
	}

	// 入金・出金とは 反対のターミネーションは無視
	dcsw = GetRecordDCsgn( &getLINE_DATA()->drec );

//MyTrace("@TermVal dcsw = %d, pn = %d\n",	dcsw, pn );

	if( dcsw == 0 || dcsw == -1 ) {
		if( pn == AC_SKIN_PN ) {
			// 11.05 /12 --- 借・貸 同じ科目の場合は、振替として チェックしない
			if( ! IsSameDbtCre(&getLINE_DATA()->drec) ) {
				return;
			}
		}
	}
	else {
		if( pn == AC_NKIN_PN )	return;	
	}

//	if( !nChar )
//	{

		DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
		if( var.pbVal != NULL )		memcpy( ip_val, var.pbVal, 6 );
		else						ZeroMemory( ip_val, sizeof ip_val );

//MyTrace("@TermVal INP_sg = %d, l_test(%d), inplen = %d\n",	getLINE_DATA()->lcnd[pn].INP_sg, l_test( ip_val ), inplen );

		if( !modefy_ok( acINP_mode, &getLINE_DATA()->drec ) || (getLINE_DATA()->drec.m_dsign[0] & 0x01) )	// 11.06 /12
//		if( !modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
		{
			if( l_jsgn( getLINE_DATA()->drec.m_val, ip_val, _BIN & 0x0f ) )
			{
				Buzzer();
			}
			memcpy( ip_val, getLINE_DATA()->drec.m_val, 6 );
			var.pbVal = (BYTE*)ip_val;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
			TerminationDataSet( pn );
		}
		else
		{
			if( getLINE_DATA()->lcnd[pn].INP_sg == FALSE && !l_test( ip_val ) && !inplen )
			{
				if( getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY == FALSE )
//					return;
					goto NEXT;
				memmove( ip_val, m_SREC.m_val, 6 );
			}
			//金額入力時は、税額F10 サインを落とす
			if( l_jsgn(m_CREC.m_val, ip_val, _BIN & 0x0f) )
			{
				getLINE_DATA()->lcnd[AC_ZEI_PN].KEY_inp = 0;
			}

			memmove( m_SREC.m_val, ip_val, sizeof(m_SREC.m_val) );
			memmove( m_CREC.m_val, m_SREC.m_val, sizeof(m_SREC.m_val) );

			memmove( getLINE_DATA()->drec.m_val, ip_val, sizeof(ip_val) );

			getLINE_DATA()->lcnd[pn].INP_sg = TRUE;

			//消費税
			struct _SY_MSG_PACK2 smp;
			strcpy_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, sy_chk_txt( &m_CREC, &smp, acINP_mode) );

			//消費税再表示
			int pntbl[] = { AC_ZEI_PN, AC_SYZ_PN };
			char zei[6] = {0};
			l_input( zei, (char*)(LPCTSTR)smp.SY_MSG_SYZEI );	// 税額をセット
			memcpy( getLINE_DATA()->drec.m_zei, zei, 6 );
			memmove( m_SREC.m_zei, zei, sizeof(m_SREC.m_zei) );
			memmove( m_CREC.m_zei, m_SREC.m_zei, sizeof(m_SREC.m_zei) );

			if( is_syohizeisiwake( &m_CREC )) {
				getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = TRUE;
			}
			else {
				getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = FALSE;
			}
			TerminationDataSet( pntbl, 2 );

			::ZeroMemory( ip_val, sizeof ip_val );
			memcpy( ip_val, getLINE_DATA()->drec.m_val, 6 );
			var.pbVal = (BYTE*)ip_val;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
			TerminationDataSet( pn );
		}
//		return;
//	}
NEXT:
	getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY = FALSE;
		
	if( !nChar )
		return;

	if (nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT))
		nChar = VK_F2;

	if( nChar )
	{
		try {

			if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
			{
#ifdef CLOSE
				// 09.08 /08
				if( ValEnterCopyMode() ) {
					if( acINP_mode == _APPEND ) {
						if( getLINE_DATA()->lcnd[pn].INP_sg == FALSE && !l_test( ip_val ) && !inplen )
							return;
					}
				}
#endif
				if( ValEnterCopyMode() ) {
TRACE("@TermVal nChar ==%d, pn = %d\n",	nChar,  pn );
					if( acINP_mode == _APPEND ) {
						if( pn == AC_NKIN_PN ) {
							if( !getLINE_DATA()->lcnd[pn].INP_sg && !getLINE_DATA()->lcnd[AC_SKIN_PN].INP_sg ) {
								set_focus( AC_SKIN_PN );
TRACE("@TermVal nChar ==%d(1)\n",	nChar );
								return;
							}
						}
						else if( pn == AC_SKIN_PN ) {
							if( !getLINE_DATA()->lcnd[AC_NKIN_PN].INP_sg && !getLINE_DATA()->lcnd[AC_SKIN_PN].INP_sg ) {
								set_focus( AC_NKIN_PN );
TRACE("@TermVal nChar ==%d(2)\n",	nChar );
								return;
							}
						}
					}
				}
				else if( nChar == VK_RIGHT ) {
					if( acINP_mode == _APPEND ) {
						if( pn == AC_NKIN_PN ) {
							if( !getLINE_DATA()->lcnd[pn].INP_sg && !getLINE_DATA()->lcnd[AC_SKIN_PN].INP_sg ) {
								set_focus( AC_SKIN_PN );
								return;
							}
						}
						else if( pn == AC_SKIN_PN ) {
							if( !getLINE_DATA()->lcnd[AC_NKIN_PN].INP_sg && !getLINE_DATA()->lcnd[AC_SKIN_PN].INP_sg ) {
								set_focus( AC_NKIN_PN );
								return;
							}
						}
					}
				}

				getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY = TRUE;

				int vect;
				vect = (nChar == VK_RETURN ) ? 1 : 2;
				bLineChg = FALSE;

				//消費税項目カーソル位置チェック
				if (pAUTOSEL->DSPSYZ_OPT == OPT_NODSP) {
					m_SET_SYZINP_PN = -1;
				}
				else {
					m_SET_SYZINP_PN = m_pSyzInp->CheckSyzPn(vect);
					if( m_SET_SYZINP_PN == -1 ) {
						if( IsMustStopSyz(&m_CREC)) {
							m_SET_SYZINP_PN = SY_SZKBN_PN;
						}
					}
				}

//				if( nChar != VK_RIGHT ) {
					if( acINP_mode == _APPEND ) {

						if( getLINE_DATA()->lcnd[pn].INP_sg == FALSE && !l_test( ip_val ) && !inplen )
						{
							memmove( ip_val, m_SREC.m_val, 6 );
						}

						memmove( m_SREC.m_val, ip_val, sizeof(m_SREC.m_val) );
						memmove( m_CREC.m_val, m_SREC.m_val, sizeof(m_SREC.m_val) );

						memmove( getLINE_DATA()->drec.m_val, ip_val, sizeof(ip_val) );

						getLINE_DATA()->lcnd[pn].INP_sg = TRUE;

						::ZeroMemory( ip_val, sizeof ip_val );
						memcpy( ip_val, getLINE_DATA()->drec.m_val, 6 );
						var.pbVal = (BYTE*)ip_val;
						DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );

						//消費税
						struct _SY_MSG_PACK2 smp;
						strcpy_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, sy_chk_txt( &m_CREC, &smp, acINP_mode) );

#ifndef KEEP
						char zei[6] = {0};
						l_input( zei, (char*)(LPCTSTR)smp.SY_MSG_SYZEI );	// 税額をセット
						memcpy( getLINE_DATA()->drec.m_zei, zei, 6 );
						memmove( m_SREC.m_zei, zei, sizeof(m_SREC.m_zei) );
						memmove( m_CREC.m_zei, m_SREC.m_zei, sizeof(m_SREC.m_zei) );

						getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = TRUE;
						TerminationDataSet( AC_ZEI_PN );
#endif
					}
//				}
				//インボイス外税免税は、税額常に修正
				if( is_sotomensiwake(&m_CREC) ) {
					m_SET_SYZINP_PN = -1;
					bMoveZei = TRUE;
				}
				else {
					// 消費税をスルーできる項目の場合、仕訳追加チェック
					if( m_SET_SYZINP_PN < 0 ) {
						//修正時の行移動があるか
						int mdfy_linechg = 0;
						if( nChar == VK_RETURN || nChar == VK_TAB ) {
							if( acINP_mode != _APPEND ) {
								mdfy_linechg = 1;
							}
						}

						nChar = TermAppendData(nChar);

						if( nChar == VK_DOWN ) {
							bLineChg = TRUE;

							//日付欄にカーソルセットするので、_dline_chg でフォーカスセットしない。
							if( isdeldata() || mdfy_linechg ) {
								m_bTermValLinChg = TRUE;
							}
						}
					}
				}
			}

			if( nChar ) {
				nChar = dline_chg( nChar );
				if( nChar == -1 )
				{
		//			MessageBeep( 0xffffffff );
					return;
				}
				else if( !nChar )
				{
					// 取消データの場合 or 行変更
					if( isdeldata() || (bLineChg && acINP_mode != _APPEND) )
					{
						set_focus( AC_DATE_PN );
						return;
					}

					return;
				}
			}
			else {
				return;
			}
		}
		catch ( CErrBlk E )
		{
			ErrExit( E.ErrCode, E.ErrMsg );
			return;
		}
	}
	if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT ) {
		if( bMoveZei ) {
			set_focus(AC_ZEI_PN);
		}
		else {
			set_termfocus(get_nowln(), pn, 1);
		}
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 ) {
		int focus_set = 0;
		if( acINP_mode == _APPEND ) {
			if( pn == AC_SKIN_PN ) {
				if( !getLINE_DATA()->lcnd[AC_NKIN_PN].INP_sg && !getLINE_DATA()->lcnd[AC_SKIN_PN].INP_sg ) {
					set_focus( AC_NKIN_PN );
					focus_set++;
				}
			}
		}
		if( ! focus_set ) {
			set_termfocus( get_nowln(), pn, -1 );
		}
	}
	else if( nChar == VK_INSERT ) {	//金額複写 --- 09.13 /12
		//金額複写
		if( acINP_mode == _APPEND ) {
			memmove( ip_val, m_SREC.m_val, sizeof(m_SREC.m_val) );
			memmove( m_CREC.m_val, m_SREC.m_val, sizeof(m_SREC.m_val) );

			memmove( getLINE_DATA()->drec.m_val, ip_val, sizeof(ip_val) );

			getLINE_DATA()->lcnd[pn].INP_sg = TRUE;

			//消費税
			struct _SY_MSG_PACK2 smp;
			strcpy_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, sy_chk_txt( &m_CREC, &smp, acINP_mode) );

			//消費税再表示
			int pntbl[] = { AC_ZEI_PN, AC_SYZ_PN };
			char zei[6] = {0};
			l_input( zei, (char*)(LPCTSTR)smp.SY_MSG_SYZEI );	// 税額をセット
			memcpy( getLINE_DATA()->drec.m_zei, zei, 6 );
			memmove( m_SREC.m_zei, zei, sizeof(m_SREC.m_zei) );
			memmove( m_CREC.m_zei, m_SREC.m_zei, sizeof(m_SREC.m_zei) );

			getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = (m_CREC.m_dsign[0]&0x40) ? TRUE : FALSE;
			TerminationDataSet( pntbl, 2 );

			::ZeroMemory( ip_val, sizeof ip_val );
			memcpy( ip_val, getLINE_DATA()->drec.m_val, 6 );
			var.pbVal = (BYTE*)ip_val;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
			TerminationDataSet( pn );
		}
	}
	else {
		if( nChar != 0x16 )
			Buzzer();
	}
}


// フォーカス 税額
int CDBAbookView::FocusIcsZei( int ln, short click )
{
	if( PROGRAM_OK == FALSE )
		return -1;

	//別画面入力消去
	try {
		Destroy_ModeLessDlg();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}
		if( _dline_chg( ln, AC_ZEI_PN, click ? 0 : 1 ) == -1 )
			return -1;

		// 取消データの場合は
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return -1;
		}
		else {
			if( data_denpdialog( ln ) != 0 )
				return -1;

			set_nowpn( AC_ZEI_PN );
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}

	return 0;
}

// ターミネーション 税額
void CDBAbookView::TerminationZei( int ln, long nChar, long inplen, long kst )
{
	VARIANT var;
	char	ip_val[6];
	char	buf[64];

	if( PROGRAM_OK == FALSE )
		return;

	// 税額修正不可 -- 05.21 /10
	BOOL bModify = TRUE;
	if( !is_sotomensiwake(&m_CREC) ) {
		if( !is_syohizeisiwake(&m_CREC) || pAUTOSEL->ZEIMDFY_OPT != OPT_ZEIMDFY || is_yusyutsusiwake(&m_CREC) ) {
			bModify = FALSE;
		}
	}

	try
	{
//		if( !nChar )
//		{
			l_defn( _BIN );

			DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
			if( var.pbVal != NULL )	memcpy( ip_val, var.pbVal, 6 );
			else					memset( ip_val, '\0', sizeof ip_val );
		
			if( !modefy_ok( acINP_mode, &getLINE_DATA()->drec ) || ! bModify )
			{
				if( l_jsgn( getLINE_DATA()->drec.m_zei, ip_val, _BIN & 0x0f ) )
				{
					Buzzer();
				}
				memcpy( ip_val, getLINE_DATA()->drec.m_zei, 6 );
				var.pbVal = (BYTE*)ip_val;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
				TerminationDataSet( AC_ZEI_PN );
			}
			else
			{
				if( getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg == FALSE && !l_test( ip_val ) && ! inplen  )
				{
					if( getLINE_DATA()->lcnd[AC_ZEI_PN].NEXT_MOVE_KEY == FALSE )
//						return;
						goto NEXT;

//					memmove( ip_val, m_SREC.m_zei, 6 );
				}

				memmove( getLINE_DATA()->drec.m_zei, ip_val, sizeof(ip_val) );
				getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = TRUE;

				l_print( buf, ip_val, FMT15 );

TRACE( ">>>> zeiterm ln = %d, dbdata->seq %d\n", get_nowln(), pDBzm->dbdata->seq );

				if( pDBsy->SyZei( buf ) == 0 ) {
					struct _SY_MSG_PACK2 smpk;
					pDBsy->SyMsg( &smpk );
					CDBINPDataRec rec;
					pDBzm->GetCDBData( &rec );

					memmove( m_SREC.m_zei, ip_val, sizeof(m_SREC.m_zei) );
					memmove( m_CREC.m_zei, m_SREC.m_zei, sizeof(m_SREC.m_zei) );

					memcpy( getLINE_DATA()->drec.m_zei, rec.m_zei, 6 );
					memcpy( ip_val, rec.m_zei, 6 );
					memcpy( getLINE_DATA()->drec.m_val, rec.m_val, 6 );

					var.pbVal = (BYTE*)ip_val;
					DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
					TerminationDataSet( AC_ZEI_PN );
				//	TerminationDataSet( AC_VAL_PN );
				}
				else {
					memmove( ip_val, m_CREC.m_zei, sizeof(m_CREC.m_zei) );
					var.pbVal = (BYTE*)ip_val;
					DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0 );
				}
			}
//			return;
//		}
NEXT:
		getLINE_DATA()->lcnd[VAL_PN].NEXT_MOVE_KEY = FALSE;
			
		if( !nChar )
			return;

		if (nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT))
			nChar = VK_F2;

		int termflg = 0;
		if( m_bSyzSotoMen && m_pnZeiSotoMen == AC_SYZ_PN ) {
			termflg = 1;
		}
		// 輸入仕入 に 変更した場合の処理
		BOOL bYunyu = FALSE;
		if( m_bSyzYunyu || termflg ) {
			// 仕訳書き込み処理
			nChar = TermAppendData( nChar );
			m_bSyzYunyu = FALSE;
			m_bSyzSotoMen = FALSE;
			m_pnZeiSotoMen = 0;

			bYunyu = TRUE;
			// 新規で仕訳を追加した
			if( ! nChar ) {
				set_focus( AC_DATE_PN );
				return;
			}
		}

		if( nChar )
		{
			try {

				nChar = dline_chg( nChar );
				if( nChar == -1 )
				{
	//				MessageBeep( 0xffffffff );
					return;
				}
				else if( !nChar )
				{
					// 取消データの場合は
					if( isdeldata() )
					{
						set_focus( AC_DATE_PN );
						return;
					}

					set_focus( bYunyu ? AC_DATE_PN : AC_ZEI_PN );
					return;
				}
			}
			catch ( CErrBlk E )
			{
				ErrExit( E.ErrCode, E.ErrMsg );
				return;
			}
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT /*|| nChar == VK_F3*/ )
		{
			getLINE_DATA()->lcnd[AC_ZEI_PN].NEXT_MOVE_KEY = TRUE;
			
		//	if( !is_AutoInp_use() )
		//	{
				if( is_sotomensiwake(&m_CREC) ) {
					m_bSyzSotoMen = TRUE;
					m_pnZeiSotoMen = AC_ZEI_PN;
				}
				set_termfocus( get_nowln(), AC_ZEI_PN, 1 );
		//	}
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 ) {
			set_termfocus( get_nowln(), AC_ZEI_PN, -1 );
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

void CDBAbookView::FocusIcsSyz( int ln, short click )
{
	if( PROGRAM_OK == FALSE )
		return;

	//別画面入力消去
	Destroy_ModeLessDlg();

	try {
		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}

		if( _dline_chg( ln, AC_SYZ_PN, click ? 0 : 1 ) == -1 )
			return;

		// 取消データの場合は
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return;
		}
		else {
			if( data_denpdialog( ln ) != 0 )
				return;

			//消費税
			strcpy_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, sy_chk_txt( &m_CREC, NULL, acINP_mode) );
			//消費税再表示
			TerminationDataSet( AC_SYZ_PN );

			set_nowpn( AC_SYZ_PN );
		}

		if( SyzInpDisp() != 0 )
		{
			PROGRAM_OK = FALSE;
			Destroy_SyzInp();
			PROGRAM_OK = TRUE;

			// 空の消費税区分欄表示
			PostMessage( WM_SKBN_ICHIRN, (WPARAM)0, (LPARAM)0 );
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}

	return;
}


// 消費税欄 ターミネーション
int CDBAbookView::TerminationSyz( int ln, long nChar, long inplen, long kst )
{
	int sv_ln = get_nowln();
	int dcsw;
//MSG msg = {0};

	if( PROGRAM_OK == FALSE )
		return -1;

	ACBOOK_LINE* al;

	try
	{
		if (nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT))
			nChar = VK_F2;

		// 輸入仕入れ 
		int termappend = 0;

		if( IsYunyuMove() ) {
			if( nChar == VK_TAB || nChar == VK_RETURN ) {
				_set_focus( get_nowln(), AC_ZEI_PN );
				return 0;
			}
			termappend = 1;
		}
		else if( is_sotomensiwake(&m_CREC) && pAUTOSEL->DSPZEI_OPT == OPT_NONOPT ) {

			if( !m_bSyzSotoMen ) {
				if( nChar == VK_RETURN ) {
					m_bSyzSotoMen = TRUE;
					m_pnZeiSotoMen = AC_SYZ_PN;
					_set_focus(get_nowln(), AC_ZEI_PN);
					return -1;  //OnSkbnInend の 次のTerminationSyzを通さないため
				}
			}
			termappend = 1;
		}
		else {
			termappend = 1;
		}

		if( termappend ) {
			// 仕訳書き込み処理
			nChar = TermAppendData( nChar );
		}

		if( nChar )
		{
			try {
				nChar = dline_chg( nChar );
				if( nChar == -1 )
				{
					return -1;
				}
				else if( !nChar )
				{
					_set_focus( get_nowln(), AC_DATE_PN );
					return -1;
				}
			}
			catch ( CErrBlk E )
			{
				ErrExit( E.ErrCode, E.ErrMsg );
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
			// TODO 仕訳書き込み および 行変更
			break;
		case VK_LEFT:
		case VK_F2:
			// 金額に戻る
			al = getLINE_DATA();
			dcsw = GetRecordDCsgn( &al->drec );

			if( dcsw == 0 || dcsw == -1 ) {
				set_focus( AC_NKIN_PN );
			}
			else {
				set_focus( AC_SKIN_PN );
			}
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


//---------------------------------------------
//	枝番未入力チェックＪＯＢ
//
//---------------------------------------------
BOOL CDBAbookView::isNoBRNInpChk()
{
	int st;

	if( !modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
		return FALSE;

	PROGRAM_OK = FALSE;

	//
	// 借方 枝番未入力チェック
	if( ! (m_NOBRNCHK & 0x10) )
	{
		int dcsw;
		dcsw  =GetRecordDCsgn( &m_CREC );

		CString code;
		int		brn = 0;

		if( dcsw == 0 ) {
			code	= m_CREC.m_cre;
			brn		= m_CREC.m_cbr;
		}
		else if( dcsw == 1 ) {
			code	= m_CREC.m_dbt;
			brn		= m_CREC.m_dbr;
		}

		st = inp_brchk( code, brn, pAUTOSEL->BRN_OPT, &m_NOBRNCHK, -1);

		switch( st )
		{
		case 0:
			break;
		default:
			set_focus( AC_KMK_PN );
		//	PostMessage( WM_RETROFOCUS, AC_KMK_PN );
		//	set_nowpn( AC_KMK_PN );
			BRNTKY_select( -1, code, NULL, TRUE );

			PROGRAM_OK = TRUE;
			return TRUE;
			break;
		}
	}
	
	PROGRAM_OK = TRUE;

	return FALSE;
}

//---------------------------------------------
//	キーによる、仕訳データ追加か？
//	仕訳データ行の最後の項目で呼び出す。
//
//---------------------------------------------
long CDBAbookView::TermAppendData( long nChar )
{
	if( nChar == VK_RETURN || nChar == VK_TAB ) {
		// 枝番未入力チェックＪＯＢ
		if( isNoBRNInpChk() )
			return 0;
		// 工事チェック
		if( isKojiCheck() )
			return 0;
		// 部門未入力チェック
		if( isBmnNoInp() )
			return 0;
		// 部門範囲チェック
		if( isBmnHaniCheck() )
			return 0;
		// 工事チェック
		if( isKojiKmkCheck() )
			return 0;

		// 仕訳書き込み処理
		if( acINP_mode == _APPEND ) {
			WriteAndNextData();
			nChar = 0;
		}
		else {
			nChar = VK_DOWN;
		}
	}

	return nChar;
}


//---------------------------------------------
//	データ追加 ＆ 次データ 作成
//
//
//---------------------------------------------
int CDBAbookView::WriteAndNextData()
{
	int nowln, i;
	nowln = get_nowln();
	int lcnt;
	lcnt = GetACListCount(); /*m_List.GetDispDataCount();*/

	// 枝番未入力クリア
	m_NOBRNCHK	= 0;
	m_ACKOJICHK	= 0;

	m_CREC.m_dtype = 0x40;

	int dcsw;
	dcsw = GetRecordDCsgn( &m_CREC );

	// 部門・工事のチェック
	// 仕訳欄で 先に 部門を入力した場合は、借貸 どちらにも部門をセットしている
	// 仕訳側の科目に修正する
	if( BMON_MST ) {
		if( m_BaseBmn == -1 ) {
			if( dcsw == 0 ) {		// 基本科目 借方
				m_CREC.m_dbmn = -1;
			}
			else if( dcsw == 1 ) {	// 基本科目 貸方
				m_CREC.m_cbmn = -1;
			}
			else {
				if( GetKamokuSign() == 0 ) {
					// 仕訳の科目欄が借方
					m_CREC.m_cbmn = -1;
				}
				else {
					// 仕訳の科目欄が貸方
					m_CREC.m_dbmn = -1;
				}
			}
		}
	}
	if( M_KOJI ) {
		if( m_BaseKoji.IsEmpty() ) {
			if( dcsw == 0 ) {		// 基本科目 借方
				m_CREC.m_dkno.Empty();
			}
			else if( dcsw == 1 ) {	// 基本科目 貸方
				m_CREC.m_ckno.Empty();
			}
			else {
				if( GetKamokuSign() == 0 ) {
					// 仕訳の科目欄が借方
					m_CREC.m_ckno.Empty();
				}
				else {
					// 仕訳の科目欄が貸方
					m_CREC.m_dkno.Empty();
				}
			}
		}
	}

	int bseq = -1;
	int insnxt_seq = -1;

	if( m_InsLine != 0 ) { //挿入SEQ 取得
		if( IsMasterType(MST_INSERT) ) {
			bseq = m_ALine[ m_InsLine ].drec.m_seq;
		}
		insnxt_seq = m_ALine[ m_InsLine ].drec.m_seq;
	}

	if( pDBzm->DB_DataAppend( &m_CREC, bseq ) != 0 )
		return -1;

	//諸口・現金貸借
	syog_genk( &m_CREC, _ADD );

	// 追加を通常入力にも反映
	DBDinpAppendParamUpdate( &m_CREC, bseq );
	// 次の SEQ 番号
	int seq = m_CREC.m_seq;
	seq++;

	m_ALine[nowln-1].drec.m_seq = m_CREC.m_seq;

	// 新しいデータをセット
	m_SREC = m_CREC;
	if( (Voln1->bm_isw & 0x10) ) {
		if( m_sdbtBmn != -1 ) {
			m_SREC.m_dbmn = m_sdbtBmn;
			m_sdbtBmn = -1;
		}
		if( m_screBmn != -1 ) {
			m_SREC.m_cbmn = m_screBmn;
			m_screBmn = -1;
		}
		//損益のみ部門で、間に貸借の仕訳を入力しても、部門番号を引き継ぐようにするため保存
		m_appSREC = m_SREC;
	}

	DataAppendReset( &m_CREC );
	if( lcnt <= nowln ) {
		// 一行スクロールする
		acline_scroldown();
	}
	else {
		nowln++;
	}

	int nxt = 0;
	//次の自動／定型仕訳有
	int edainp;
	if( NextAutoSwkData(&m_CREC, edainp) > 0 ) {
		nxt = 1;
	}
	if( ! nxt ) {
		datetekiyo_check_tkrec(&m_CREC);

		if( RdataToInpData(&m_CREC, &m_SREC) > 0 ) {
			nxt = 2;
		}
	}
	//挿入
	if( insnxt_seq != -1 ) {
		m_InsLine = 0;

		if( nxt ) {
			for( int i = 0; i < ACREC_MAX; i++ ) {
				if( m_ALine[i].drec.m_seq == insnxt_seq ) {
					m_InsLine = i+1;
				}
			}
		}

		if( m_InsLine != 0 ) {
			int n = (m_InsLine-1);
			// データを一つずつずらす
			for( int i = ACREC_MAX-1; i >= n; i-- ) {
				if( i == 0 )	break;

				m_ALine[i] = m_ALine[i-1];
				// 現在の追加行は空行にする
				if( m_ALine[i].abs_pos == 0 ) {
					acline_init( &m_ALine[i], 1 );
				}
			}

			acline_init( &m_ALine[n], 1 );
			m_ALine[n].abs_pos	= 0;
			m_ALine[n].drec		= m_CREC;
			m_ALine[n].drec.m_seq = pDBzm->m_lastdata.m_seq;

			pDBzm->DateConv( m_ALine[n].bcddate, &m_ALine[n].drec, CV2_1 );

			nowln = n+1;
		}
	}
	else {
		m_InsLine = 0;

		if( nxt ) {
			for( int i = 0; i < ACREC_MAX; i++ ) {
				if( m_ALine[i].abs_pos <= 0 && m_ALine[i].lattr <= 0 ) {
					m_ALine[i].abs_pos	= 0;
					m_ALine[i].drec		= m_CREC;
					m_ALine[i].drec.m_seq = pDBzm->m_lastdata.m_seq;

					pDBzm->DateConv( m_ALine[i].bcddate, &m_ALine[i].drec, CV2_1 );
					nowln = i+1;
					break;
				}
			}
		}
	}

	ListDataSet();
	if( nxt ) {
		sprintf_s( _getLINE_DATA(nowln)->syzstr, sizeof _getLINE_DATA(nowln)->syzstr, "%s", sy_line_txt( &m_CREC ) );

		if( nxt == 1 ) {
			if( edainp ) {
				_getLINE_DATA(nowln)->lcnd[AC_KMK_PN].INP_type = 1;
			}
			if( m_bAutoKmkSet ) {
				m_TKsyz = m_autoTKrec.m_tksgn[1];
			}
			else {
				m_TKsyz = 0;
			}
			int dc_sw = tky_select_to_drec_sub(&m_CREC, &m_autoTKrec, nowln, 1);
			//表示
			tky_select_dsp(&m_CREC, dc_sw, &m_autoTKrec, FALSE, TRUE, nowln);
		}
		DataLineUpdate( &m_CREC, nowln, 1 );
	}
	RDRemainDispJOB();

	if( ! nxt ) {
		m_CREC.Reset();
		m_CREC.m_seq	= seq;
		m_CREC.m_ddate	= m_SREC.m_ddate;
		m_CREC.m_mofs	= m_SREC.m_mofs;
		m_CREC.m_ksign	= m_SREC.m_ksign;
	}

	// _dline_chg の DB_DataCorrect() 関数を 呼ばないように 現在行を初期化
	set_nowln(-1);

	// 
	if( m_ALine[nowln-1].abs_pos < 0 ) {
		for( i = 0; i < ACREC_MAX; i++ ) {
			if (i == 0 && m_ALine[i].lattr == 1) {
				continue;
			}
			if( m_ALine[i].abs_pos >= 0 ) {
				nowln = (i+1);
			}
			else {
				break;
			}
		}
	}

	_set_focus( nowln, AC_DATE_PN );

	return 0;
}

// 仕訳データの摘要から摘要レコードを取得する
void CDBAbookView::datetekiyo_check_tkrec(CDBINPDataRec *CREC)
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
					m_TKsyz = tkd.m_tksgn[1];
					m_TKattrec = tkd;
					m_TKsyz2 = m_TKsyz;
					break;
				}
			} while( pDBzm->tkrec->MoveNext() == 0 );
		}
		tkrec_queryAll();;
	}
}


//消費税入力画面表示
int CDBAbookView::SyzInpDisp()
{
	if (m_pSyzInp == NULL)
	{
		ermset( ERROR_ENV, "消費税入力オブジェクトがありません。");
		return -1;
	}
	else
	{
		int sw;
		double rate_w, rate_h;
		double vrate = (flgPen ? 84 : 100);
//		get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット
		rate_w = rate_h = 1.0;

		CSize size;
		m_pSyzInp->GetCrntRect( size );
		CRect	irect, selRect;

		m_List.GetItemScreenRect( 0, DATA_SYZ, irect );

		m_Input.GetWindowRect( irect );
		this->ScreenToClient( &irect );
		irect.left = irect.right;

		BOOL bOK = modefy_ok( acINP_mode, &getLINE_DATA()->drec );

		int posflg = SYP_TOPLEFT;
		m_selval32.GetWindowRect( selRect );
		this->ScreenToClient( &selRect );
		if( selRect.bottom > (irect.top - size.cy) ) {
			irect.top = irect.bottom;
			posflg = SYP_BOTTOMLEFT;
		}

		sw = m_pSyzInp->DialogON( bOK, irect, posflg, m_SET_SYZINP_PN );

		return sw;
	}
}


void CDBAbookView::Hide_SyzInp()
{
	CDBbaseView::Destroy_SyzInp();

/*
	if( get_nowln() == -1 )	return;

	//消費税
	strcpy_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, sy_chk_txt( &m_CREC, NULL, acINP_mode) );
	//消費税再表示
	TerminationDataSet( AC_SYZ_PN );
*/
}



// 消費税区分一覧表示
LRESULT CDBAbookView::DspSkbnIchiran( WPARAM wParam, LPARAM lParam )
{
	DWORD i;
	CString *str = (CString*)lParam;

#define SYZSEL_MAX	8*4

static struct _SelTable SelVal32[SYZSEL_MAX];
static SelSetProcPar	spar;

	spar.sp_prevpn = m_SelDispPN;
	spar.sp_nowpn = BVW_SYZ_PN;

	WORD dspSign = HIWORD(wParam);
	if( dspSign & 0x01 ) {
		strcpy_s(spar.seljob, sizeof spar.seljob, "《登録番号》");
	}
	else {
		strcpy_s(spar.seljob, sizeof spar.seljob, "《消費税》");
	}
	int dspCnt = LOWORD(wParam);

	// 現在内容のセーブ と　表示
	for( i = 0 ; i < SYZSEL_MAX ; ++i )
	{
		// 表示
		SelVal32[i].Data = i < dspCnt ? *(str+i) : "";
	//	SelVal32[i].number = i + (isEntrySys() ? 9 : 1);
		SelVal32[i].number = i + 1;
	}

	SetSelectSel32( SelVal32, &spar, SYZSEL_MAX, this );

	return TRUE;
}


// キーによる 消費税ダイアログEND
LRESULT CDBAbookView::OnSkbnInend( WPARAM wParam, LPARAM lParam )
{
TRACE( "--++ CDBAbookView syohi KeyEnd %d\n", wParam );

	if( TerminationSyz( get_nowln(), wParam, 0, 0 ) != -1 )
	{
		TerminationSyz( get_nowln(), 0, 0, 0 );
	}
	else if( m_pSyzInp )
	{
		m_pSyzInp->set_nowfocus();
	}

	return 1;
}

// 消費税 の属性を再表示
LRESULT CDBAbookView::OnSkbnLineDisp( WPARAM wParam, LPARAM lParam )
{
TRACE( "--++ CDBAbookView syohi OnSkbnLineDisp\n" );

	struct _SY_MSG_PACK2 smpk;
	pDBsy->SyMsg( &smpk );

	DWORD dwKobe = 0;
	if (acINP_mode == _APPEND && pDBzm->IsKobetsuBmnSyz()) {
		CDBINPDataRec	tmprec;
		pDBzm->GetCDBData(&tmprec);
		if (pDBzm->IsTokuteiSyunyuData(&tmprec)) {
			//特定収入区分が異なる場合
			if ((tmprec.m_dsign[5] & 0x0f) != (m_CREC.m_dsign[5] & 0x0f)) {
				int dcsw;
				dcsw = GetRecordDCsgn(&m_CREC);
				int bmn;
				if (dcsw == 0) {
					bmn = tmprec.m_cbmn;
				}
				else {
					bmn = tmprec.m_dbmn;
				}
				dwKobe = pDBzm->KobetsuSiwakeCheck(&tmprec, bmn);
			}
		}
	}
	if (dwKobe) {
		int st = pDBsy->SyFnc(dwKobe);
	}

	pDBzm->GetCDBData( &m_CREC );
	sprintf_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, "%s", sy_line_txt( &m_CREC ) );

	getLINE_DATA()->drec.m_dsign.Copy( m_CREC.m_dsign );
	memcpy(getLINE_DATA()->drec.m_invno, m_CREC.m_invno, sizeof m_CREC.m_invno);

	if( pDBsy->IsTaikaData( pDBzm->dbdata ) )
			getLINE_DATA()->ac_sign |= 0x01;
	else	getLINE_DATA()->ac_sign &= ~0x01;

	memmove( getLINE_DATA()->drec.m_zei, m_CREC.m_zei, 6 );
	memmove( getLINE_DATA()->drec.m_taika, m_CREC.m_taika, 6 );

	getLINE_DATA()->lcnd[AC_SYZ_PN].INP_sg = TRUE;

	if( is_syohizeisiwake( &m_CREC  )) {
		getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = TRUE;
	}
	else {
		getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = FALSE;
	}

	// 消費税が、輸入仕入 に変更になったか？
	if( m_pSyzInp->GetSyzswkKbn() != CD_YUNYU ) {
		if( (m_CREC.m_dsign[0]&0x40) && (m_CREC.m_dsign[2]&0x0f) == 0x01 && 
			m_CREC.m_dsign[4] == 4 ) {
			m_bSyzYunyu = (pAUTOSEL->ZEIMDFY_OPT == OPT_ZEIMDFY) ? TRUE : FALSE;
		}
		else {
			m_bSyzYunyu = FALSE;
		}
	}

	int pntbl[] = {
		AC_SYZ_PN,AC_ZEI_PN
	};

	TerminationDataSet( pntbl, 2 );

	return 1;
}


// 輸入仕入で、税額欄に移動するか？
BOOL CDBAbookView::IsYunyuMove()
{
	BOOL ret = FALSE;

	// 税額欄を表示している場合のみ
	if( pAUTOSEL->DSPZEI_OPT == OPT_NONOPT && pAUTOSEL->ZEIMDFY_OPT == OPT_ZEIMDFY ) {
		if( m_bSyzYunyu )	ret = TRUE;
	}

	return ret;
}



// 付箋のダイアログを表示
//
void CDBAbookView::HusenInpDisp( )
{
	if (m_pHusenInp == NULL)
	{
		ermset( ERROR_ENV, "付箋入力オブジェクトがありません。");
		return;
	}
	else
	{
		int sw;

		CSize size;
		m_pHusenInp->GetCrntRect( size );
		CRect	irect;

		int ln = get_nowln();
		int item;

		// 摘要欄より、表示位置を取得
		item = DATA_TEKIYO;
		m_List.GetItemScreenRect( ln, item, irect );

		this->ScreenToClient( &irect );
		irect.left = irect.right;

		BOOL bOK = modefy_ok( acINP_mode, &getLINE_DATA()->drec );
		if( bCONFIRM_MASTER )	bOK = FALSE;
		sw = m_pHusenInp->DialogON( bOK, irect, HSN_TOPLEFT, HSN_HSEL_PN );

	}
}

//付箋入力 画面消去
void CDBAbookView::Destroy_HusenInp()
{
	if (m_pHusenInp ) {
		if( m_pHusenInp->IsDisplay() ) {
			CDBbaseView::Destroy_HusenInp();
			// データ表示
			TerminationDataSet( AC_HUSEN_PN );
		}
	}
}

//	付箋入力のメッセージ処理
//
//
LRESULT CDBAbookView::OnHusenInpMsg( WPARAM wParam, LPARAM lParam )
{
	long nChar;

	WORD sgn, key;
	sgn = LOWORD( wParam );
	key = HIWORD( wParam );
	HSEN_DATA* pHdt;

	if( lParam ) {
		pHdt = (HSEN_DATA*)lParam;
		m_CREC.m_dsign[7] &= (~0x0f);
		m_CREC.m_dsign[7] |= pHdt->hsen_sel;
		m_CREC.m_tag = pHdt->hsen_cmt;

		getLINE_DATA()->drec.m_dsign[7] &= (~0x0f);
		getLINE_DATA()->drec.m_dsign[7] |= pHdt->hsen_sel;
		getLINE_DATA()->drec.m_tag = pHdt->hsen_cmt;
	}

	if(	sgn == 0 ) {	// キーによる終了

		if( m_pHusenInp != NULL && key ) {

			TerminationDataSet( AC_HUSEN_PN );

			if( key == VK_RETURN || key == VK_TAB || key == VK_RIGHT ) {
				set_termfocus( get_nowln(), AC_TKY_PN, 1 );
			}
			else {

				nChar = dline_chg( key );
				if( nChar == -1 )
				{
	//				MessageBeep( 0xffffffff );
					return 1;
				}
				else if( !nChar )
				{
					// 取消データの場合は
					if( isdeldata() )
					{
						set_focus( AC_DATE_PN );
						return 1;
					}
					set_focus( AC_TKY_PN );
					return 1;
				}

				switch( nChar ) {
				case VK_LEFT:
				case VK_F2:
					set_focus( AC_TKY_PN );
				//	set_termfocus( get_nowln(), AC_DATE_PN, -1 );
					break;
				}
			}
		}
	}
	else if( sgn == 1 ) {
		//表示を更新
		TerminationDataSet( AC_HUSEN_PN );
	}

	return 1;
}



//サブ入力画面 表示
void CDBAbookView::SubDataInpDisp( int pn )
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
		CRect	irect;

		int ln = get_nowln();
		int item, pos;

		// 摘要欄より、表示位置を取得
		if( IsMasterType(MST_INSERT) ) {
			item	= DATA_SEQ;
			pos		= SDT_TOPRIGHT;
		}
		else {
			item	= DATA_TEKIYO;
			pos		= SDT_TOPLEFT;
		}
		m_List.GetItemScreenRect( ln, item, irect );
		this->ScreenToClient( &irect );
		irect.left = irect.right;

		sw = m_pSubDtInp->DialogON( irect, pos, pn );

		((CMainFrame*)GetDBDinpMainFrame())->StatusGuideDisp(AFX_IDS_IDLEMESSAGE);
	}
}


//サブ入力 画面消去
void CDBAbookView::Destroy_SubDataInp()
{
	if (m_pSubDtInp ) {
		if( m_pSubDtInp->IsDisplay() ) {
			CDBbaseView::Destroy_SubDataInp();
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////
// 変動事由

//変動事由入力表示
void CDBAbookView::HjiyuInpDisp()
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
		item = DATA_HJIYU;

		m_List.GetItemScreenRect( ln, item, irect );

		this->ScreenToClient( &irect );
		irect.left = irect.right;
		//はみ出た分調整
		if( (irect.right - size.cx) < 0 ) {
			int sa = (irect.right - size.cx);
			irect.left += abs(sa);
		}
		BOOL bOK = modefy_ok( acINP_mode, &getLINE_DATA()->drec );
		sw = m_pHjiyuInp->DialogON( bOK, irect, HJY_TOPLEFT );
	}
}

//変動事由 画面消去
void CDBAbookView::Destroy_HjiyuInp()
{
	if (m_pHjiyuInp ) {
		if( m_pHjiyuInp->IsDisplay() ) {
			CDBbaseView::Destroy_HjiyuInp();
			// データ表示
/*- '09.03.11 -*/
//			TerminationDataSet( DATA_HJIYU );
/*-------------*/
			TerminationDataSet( AC_HJIYU_PN );
/*-------------*/
		}
	}
}


// 変動事由 フォーカス処理
void CDBAbookView::FocusIcsHjiyu( int ln, short click )
{

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
		//別画面入力消去
		Hide_SyzInp();
		Destroy_HusenInp();
		Destroy_SubDataInp();

		if( click ) {
			long nChar = 0;
			click_seljob( ln, nChar );
		}
		if( _dline_chg( ln, AC_HJIYU_PN, click ? 0 : 1  ) == -1 )
			return;

		// 取消データの場合は
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return;
		}
		if( data_denpdialog( ln ) != 0 )
			return;

		getLINE_DATA()->lcnd[AC_HJIYU_PN].INP_sg = TRUE;

		set_nowpn( AC_HJIYU_PN );

		HJIYU_DATA hd = {0};
		CString str;
		SelPar par;

		BOOL bHendo, bNaibu;

		if( pDBzm->zvol->apno < 0x50 )	bHendo = TRUE;
		else {
			bHendo = FALSE;

			if( IsSyafuKaisei( pDBzm->zvol ) )	bNaibu = TRUE;
			else								bNaibu = FALSE;
		}

		if( bHendo ) {
			par.sel_pn = SL_HJIYU_PN;
		}
		else {
			par.sel_pn = bNaibu ? SL_NAIBU_PN : SL_NPFURI_PN;
		}
		par.dbt = m_CREC.m_dbt;
		par.cre = m_CREC.m_cre;

		//内部取引の場合は、複数ページあり
		if( m_SelDispPN == SL_NAIBU_PN && par.sel_pn == SL_NAIBU_PN);
		else {
			m_Seldata.SelectJob( &par, -1 );
		}

		if( bHendo ) {
			if( get_khkbntype( m_CREC.m_dbt, m_CREC.m_cre, pDBzm ) >= 0 ) {
				hd.hjy_code = m_CREC.m_hjiyu;
				pDBzm->get_hjiyustr( hd.hjy_code, str );
				strcpy_s( hd.hjy_name, sizeof hd.hjy_name, str );
				m_pHjiyuInp->SetHjiyuData( &hd, FALSE );
				m_pHjiyuInp->set_mode(0);
				HjiyuInpDisp();
			}
			else {
				Destroy_HjiyuInp();
			}
		}
		else {
			if( bNaibu ) {
				//内部取引消去
				hd.hjy_code = m_CREC.m_hjiyu;
				hd.hjy_nbcd = m_CREC.m_nbcd;

				m_Seldata.GetNaibuString( hd.hjy_code, hd.hjy_nbcd, str );
				strcpy_s( hd.hjy_name, sizeof hd.hjy_name, str );
				m_pHjiyuInp->SetHjiyuData( &hd, FALSE );

				m_pHjiyuInp->set_mode(2);
				HjiyuInpDisp();
			}
			else {
				// 振替区分
				if( m_Seldata.IsNPfuridata( m_CREC.m_dbt, m_CREC.m_cre ) ) {
					hd.hjy_code = m_CREC.m_hjiyu;
					m_Seldata.GetNPfuriStr( hd.hjy_code, str );
					strcpy_s( hd.hjy_name, sizeof hd.hjy_name, str );
					m_pHjiyuInp->SetHjiyuData( &hd, FALSE );

					int sign = 1;
					if( IsGakkouH27Master( pDBzm ) )	sign = 4;
					m_pHjiyuInp->set_mode(sign);
					HjiyuInpDisp();
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


// 変動事由ダイアログより　ユーザメッセージ　
LRESULT CDBAbookView::OnHjiyuInEnd( WPARAM wParam, LPARAM lParam )
{
	long nChar;

	try {
		if( wParam )
		{
			TerminationDataSet( AC_HJIYU_PN );

			wParam = SelNaibuDsp( wParam );

			nChar = dline_chg( wParam );
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
					set_focus( AC_DATE_PN );
					return 1;
				}

				set_focus( AC_HJIYU_PN );
				return 1;
			}

			switch( nChar ) {
			case VK_RETURN : case VK_RIGHT : case VK_TAB : 
				set_termfocus( get_nowln(), AC_HJIYU_PN, 1 );
				break;
			case VK_LEFT:
				set_termfocus( get_nowln(), AC_HJIYU_PN, -1 );
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
LRESULT CDBAbookView::OnHjiyuSel( WPARAM wParam, LPARAM lParam )
{
	if( wParam > 0 ) {
		_hjiyu_select( wParam );
	}
	else {
		getLINE_DATA()->lcnd[AC_HJIYU_PN].Etcmsg = "";
		getLINE_DATA()->drec.m_hjiyu = 0;
		m_CREC.m_hjiyu = 0;
	}

	TerminationDataSet( HJIYU_PN );

	return 1;
}


void CDBAbookView::_hjiyu_select( int selno )
{
	char buf[128];
	SelResult srt;
	BOOL bHendo, bNaibu;

	if( pDBzm->zvol->apno < 0x50 )	bHendo = TRUE;
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
		// 変動事由選択OK
		getLINE_DATA()->lcnd[AC_HJIYU_PN].Etcmsg = srt.selname;

		HJIYU_DATA hd = {0};
		hd.hjy_code = srt.code;
		if( bNaibu )	hd.hjy_nbcd = srt.bmn;
		strcpy_s( hd.hjy_name, sizeof hd.hjy_name, srt.selname );
		m_pHjiyuInp->SetHjiyuData( &hd, TRUE );

		getLINE_DATA()->drec.m_hjiyu = srt.code;
		m_CREC.m_hjiyu = srt.code;
		if( bNaibu ) {
#ifndef NAIBU_VE2
			getLINE_DATA()->drec.m_nbcd = -1;
			m_CREC.m_nbcd = -1;
#else
			getLINE_DATA()->drec.m_nbcd = srt.code;
			m_CREC.m_nbcd = srt.bmn;
#endif
		}
		else {
			getLINE_DATA()->drec.m_nbcd = -1;
			m_CREC.m_nbcd = -1;
		}

		if( bHendo && getLINE_DATA()->lcnd[ AC_TKY_PN ].IMG_sg == 0 ) {
			if( pAUTOSEL->HJIYU_OPT == OPT_HJIYU ) {
				CString tmp;
				tmp = m_CREC.m_tekiyo;
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

					m_CREC.m_tekiyo = tmp;
					getLINE_DATA()->drec.m_tekiyo = tmp;
					TerminationDataSet( AC_TKY_PN );
				}
			}
		}
	}
	else {
		Buzzer();
	}

}

// 科目が変更になったときの変動事由をチェック
void CDBAbookView::HendJiyuCheck( CDBINPDataRec *pcrec, int clear/*=0*/ )
{
	if( pcrec->m_hjiyu != 0 ) {
		if( ! IsEnableHjiyu( pcrec ) || clear ) {
			// 変動事由コードが正しくないので 0 にする
			 pcrec->m_hjiyu = 0;

			 getLINE_DATA()->lcnd[AC_HJIYU_PN].Etcmsg = "";
			 TerminationDataSet( AC_HJIYU_PN );
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	サブ入力 キー処理
//
LRESULT CDBAbookView::OnSubDataInEnd( WPARAM wParam, LPARAM lParam )
{
	long nChar;

	WORD sgn, key;
	sgn = LOWORD( wParam );
	key = HIWORD( wParam );
	CDBINPDataRec* prec;

	if( m_pSubDtInp == NULL )
		return 1;

	if( lParam ) {
		prec = (CDBINPDataRec*)lParam;

		m_CREC.m_dsign[7] &= (~0x0f);
		m_CREC.m_dsign[7] |= (prec->m_dsign[7]&0x0f);
		m_CREC.m_tag = prec->m_tag;
	//	m_CREC.m_snumber	= prec->m_snumber;
		m_CREC.m_rnumber = prec->m_rnumber;
		m_CREC.m_tdate	= prec->m_tdate;

		getLINE_DATA()->drec.m_dsign[7] &= (~0x0f);
		getLINE_DATA()->drec.m_dsign[7] |=(prec->m_dsign[7]&0x0f);
		getLINE_DATA()->drec.m_tag = prec->m_tag;
	//	getLINE_DATA()->drec.m_snumber	= prec->m_snumber;
		getLINE_DATA()->drec.m_rnumber = prec->m_rnumber;
		getLINE_DATA()->drec.m_tdate	= prec->m_tdate;

	}

	if(	sgn == 0 ) {	// キーによる終了

		if( key ) {

			TerminationDataSet( AC_HUSEN_PN );

			if( key == VK_RETURN || key == VK_TAB || key == VK_RIGHT ) {
				set_termfocus( get_nowln(), AC_TKY_PN, 1 );
			}
			else {

				nChar = dline_chg( key );
				if( nChar == -1 )
				{
	//				MessageBeep( 0xffffffff );
					return 1;
				}
				else if( !nChar )
				{
					// 取消データの場合は
					if( isdeldata() )
					{
						set_focus( AC_DATE_PN );
						return 1;
					}
					set_focus( AC_TKY_PN );
					return 1;
				}

				switch( nChar ) {
				case VK_LEFT:
				case VK_F2:
					set_focus( AC_TKY_PN );
					break;
				}
			}
		}
	}
	else if( sgn == 1 ) {
		//表示を更新
		TerminationDataSet( AC_HUSEN_PN );
	}

	return 1;
}


//----------------------------------------------------
//		枝番名称をセット
//
//----------------------------------------------------
void CDBAbookView::set_etcdsp( int ln, int pn )
{
	struct _SET_KMKETC sk;
	int dcsw;
	ACBOOK_LINE* al;
	al = _getLINE_DATA(ln);

	dcsw = GetRecordDCsgn( &al->drec );

	//部門・工事 の対応 [12.17 /12]
	BOOL bDisp;
	bDisp = ((pAUTOSEL->BMNDSP_OPT == OPT_BMNDSP) ? TRUE : FALSE);

	if( pn == AC_BMON_PN ) {
		// 部門
		if( dcsw == 0 ) {
			sk.bmn	= al->drec.m_cbmn;
		}
		else {
			sk.bmn	= al->drec.m_dbmn;
		}
		sk.brn	= -1;
		sk.code.Empty();
		sk.kno.Empty();

		if( bDisp && _set_bmnetc( &sk ) ) {
			al->lcnd[pn].Etcmsg = sk.etc;
			al->lcnd[pn].ETC_txtcol = sk.etc_col;
		}
		else {
			al->lcnd[pn].Etcmsg.Empty();
			al->lcnd[pn].ETC_txtcol = 0;
		}
		return;
	}
	else if( pn == AC_KOJI_PN ) {
		// 工事
		sk.bmn	= -1;
		sk.brn	= -1;
		sk.code.Empty();
		if( dcsw == 0 ) {
			sk.kno	= al->drec.m_ckno;
		}
		else {
			sk.kno	= al->drec.m_dkno;
		}

		if( bDisp && _set_kojietc( &sk ) ) {
			al->lcnd[pn].Etcmsg = sk.etc;
			al->lcnd[pn].ETC_txtcol = sk.etc_col;
		}
		else {
			al->lcnd[pn].Etcmsg.Empty();
			al->lcnd[pn].ETC_txtcol = 0;
		}
		return;
	}

	// 枝番名称表示
	sk.bmn	= -1;
	if( dcsw == 0 ) {
		sk.bmn = al->drec.m_cbmn;
		sk.brn	= al->drec.m_cbr;
		sk.code	= al->drec.m_cre;
	}
	else {
		sk.bmn = al->drec.m_dbmn;
		sk.brn	= al->drec.m_dbr;
		sk.code	= al->drec.m_dbt;
	}

	if( _set_kmketc( &sk, TRUE ) ) {
		al->lcnd[AC_KMK_PN].Etcmsg = sk.etc;
		al->lcnd[AC_KMK_PN].ETC_txtcol = sk.etc_col;
	}
	else {
		al->lcnd[AC_KMK_PN].Etcmsg.Empty();
		al->lcnd[AC_KMK_PN].ETC_txtcol = 0;
	}

}

// ターミネーション時のデータセット
void CDBAbookView::TerminationDataSet( int pn, int nowln/*=-1*/ ) 
{
	DTLIST_DATA data;
	int ln;
	if( nowln == -1 )	ln	= get_nowln();
	else				ln	= nowln;
	if( ln <= 0 )	return;

	m_List.GetDtListData( ln-1, &data );
	
	ACBOOK_LINE *dtline;
	dtline = _getLINE_DATA( ln );

	_TerminationDataSet( &data, dtline, &pn, 1 );

	if( pn != AC_TKY_PN )
		m_List.UpdateDtListData( ln-1, &data );
	else
		m_List.SetDtListData( ln-1, &data );
}

// ターミネーション時のデータセット 複数項目
void CDBAbookView::TerminationDataSet( int pn[], int pncnt, int nowln/*=-1*/ ) 
{
	DTLIST_DATA data;
	int ln;
	if( nowln == -1 )	ln	= get_nowln();
	else				ln	= nowln;
	if( ln <= 0 )	return;

	m_List.GetDtListData( ln-1, &data );
	
	ACBOOK_LINE *dtline;
	dtline = _getLINE_DATA( ln );

	_TerminationDataSet( &data, dtline, pn, pncnt );

	BOOL bTekiyoPN = FALSE;

	for( int i = 0; i < pncnt; i++ ) {
		if( pn[i] == AC_TKY_PN ) {
			bTekiyoPN = TRUE;
		}
	}

	if( ! bTekiyoPN )
		m_List.UpdateDtListData( ln-1, &data );
	else
		m_List.SetDtListData( ln-1, &data );
}


void CDBAbookView::_TerminationDataSet( DTLIST_DATA* data, ACBOOK_LINE *dtline, int pntbl[], int tblcnt ) 
{
//	BYTE ksgn;
	int pn, sg, dcsw, st;
	DBKNREC* pKn;

	data->dd_dsign = 0;

	if( dtline->abs_pos > 0 ) {
		// 転送済みか？
		if( (st = tline_chk( &dtline->drec )) < 0 ) {
			if( st == -1 )
				data->dd_dsign |= 0x02;
			else
				data->dd_dsign |= 0x08;
		}
		// 取消し
		if( dtline->drec.m_dsign[0] & 0x01 ) {
			data->dd_dsign |= 0x01;
		}

		sg = MODEFY_BIT.bitncnd( dtline->drec.m_seq - 1);
		if( sg == -1 || !sg )
			;
		else
			(data->dd_dsign |= 0x04);
	}

	dcsw = GetRecordDCsgn( &dtline->drec );

	for( int i = 0; i < tblcnt; i++ ) {
		pn = pntbl[i];
//		data->dd_ctbl[pn] = dtline->drec.m_ctbl[pn];

		switch( pn ) {
		case AC_SEQ_PN:
		case AC_DATE_PN:
			data->dd_ksign = 0;
			data->dd_mofs = 0;

			if( dtline->lcnd[AC_DATE_PN].INP_sg ) {
				data->dd_ddate	= dtline->drec.m_ddate;
				data->dd_ksign	= dtline->drec.m_ksign;
				data->dd_mofs	= dtline->drec.m_mofs;
/*
				if( data->dd_ddate > 0 ) {
					ksgn = KESY_SG_GET( dtline->bcddate[0] );
					data->dd_ksign = !ksgn ? 0 : 1;
					BYTE tmp[10] = {0};
					memcpy( tmp, dtline->bcddate, 2 );

					CVolDateDB vd;
					int offset;
					switch( ksgn ) {
					case 1: tmp[0] = 0x61;	break;
					case 2: tmp[0] = 0x62;	break;
					case 3: tmp[0] = 0x63;	break;
					case 4: tmp[0] = 0x00;	break;
					default:
						break;
					}
					if( vd.db_vd_offset( tmp, &offset, pDBzm ) != 0 ) {
						ICSMessageBox( "月のオフセットを取得できません！ ");
						return;
					}
					data->dd_mofs = offset;
				}
*/
			}
			else {
				data->dd_ddate = 0;
			}
			break;
		case AC_DENP_PN:
			data->dd_cno = dtline->drec.m_cno;
			break;
		case AC_DOCEVI_PN:
			data->dd_docevi = dtline->drec.m_skn;
			break;

		case AC_BMON_PN:
			if( dcsw == 0 || dcsw == -1 ) {
				data->dd_dbmn = dtline->drec.m_cbmn;
				data->dd_cbmn = data->dd_dbmn;
			}
			else {
				data->dd_dbmn = dtline->drec.m_dbmn;
				data->dd_cbmn = data->dd_dbmn;
			}
			data->dd_dbmn_etc = dtline->lcnd[pn].Etcmsg;
			data->dd_ctbl[DATA_DEB_BMON].text = dtline->lcnd[pn].ETC_txtcol;
			break;
		case AC_KOJI_PN:
			if( dcsw == 0 || dcsw == -1 ) {
				data->dd_dkno = dtline->drec.m_ckno;
				data->dd_ckno = data->dd_dkno;
			}
			else {
				data->dd_dkno = dtline->drec.m_dkno;
				data->dd_ckno = data->dd_dkno;
			}
			data->dd_dkno_etc = dtline->lcnd[pn].Etcmsg;
			data->dd_ctbl[DATA_DEB_KNO].text = dtline->lcnd[pn].ETC_txtcol;
			break;
		case AC_KMK_PN:

			if( dcsw == 0 || dcsw == -1 ) {
				data->dd_dbt = dtline->drec.m_cre;
				data->dd_dbr = dtline->drec.m_cbr;

				pKn = pDBzm->DB_PjisToKnrec( data->dd_dbt );
				if( pKn ) {
					KamokuString( pKn, data->dd_deb_name );
				//	data->dd_deb_name.Format( "%.14s", pKn->knnam );
				}
			}
			else {
				data->dd_dbt = dtline->drec.m_dbt;
				data->dd_dbr = dtline->drec.m_dbr;

				pKn = pDBzm->DB_PjisToKnrec( data->dd_dbt );
				if( pKn ) {
					KamokuString( pKn, data->dd_deb_name );
				//	data->dd_deb_name.Format( "%.14s", pKn->knnam );
				}
			}

			data->dd_deb_etc = dtline->lcnd[AC_KMK_PN].Etcmsg;
			data->dd_ctbl[DATA_DEBT].text = dtline->lcnd[AC_KMK_PN].ETC_txtcol;
			break;
/*		case AC_CBMN_PN:
			data->dd_cbmn = dtline->drec.m_cbmn;
			data->dd_cbmn_etc = dtline->drec.m_cbmn_etc;
			break;
		case AC_CKJI_PN:
			data->dd_ckno = dtline->drec.m_ckno;
			data->dd_ckno_etc = dtline->drec.m_ckno_etc;
			break;
		case AC_CRED_PN:
			data->dd_cre = dtline->drec.m_cre;
			data->dd_cbr = dtline->drec.m_cbr;
			data->dd_cre_name.Format( "%.14s", dtline->drec.m_cre_name );
			data->dd_cre_etc = dtline->drec.m_cre_etc;
			break;
*/
		case AC_NKIN_PN:
			memcpy( data->dd_val, dtline->drec.m_val, 6 );
			data->val_flag = dtline->lcnd[AC_NKIN_PN].INP_sg;
			break;

		case AC_SKIN_PN:
			data->dd_udtbl[0].userdata = (BYTE*)dtline->drec.m_val;
			data->dd_udtbl[0].initflag = dtline->lcnd[AC_SKIN_PN].INP_sg;
			break;

		case AC_ZEI_PN:
			memcpy( data->dd_zei, dtline->drec.m_zei, 6 );
			data->zei_flag = dtline->lcnd[AC_ZEI_PN].INP_sg;
			/* 非課税有価証券譲渡仕訳 */
			if( (dtline->ac_sign & 0x01) )
				data->zei_flag |= 0x10;

			COLORREF back;
			sg = isnot_defzeigaku(&dtline->drec, back);

			if (sg > 0)	data->dd_ctbl[DATA_ZEI].back = back;
			else		data->dd_ctbl[DATA_ZEI].back = (COLORREF)-1;
			break;
		
		case AC_SYZ_PN:
			data->dd_syz_str = dtline->syzstr;
			//消費税修正サイン
			sg = SYZMDFY_BIT.bitncnd( dtline->drec.m_seq - 1);
			if( sg > 0 )	data->dd_ctbl[DATA_SYZ].back = RGB_SYZ_MDFY;
			else			data->dd_ctbl[DATA_SYZ].back = (COLORREF)-1;
			break;
		case AC_TKY_PN:

			if( dtline->lcnd[ AC_TKY_PN ].IMG_sg == 1 && dtline->drec.m_imgsq > 0 ) {
				data->dd_tek.tekiyo = "";
			//	CString filter;
			//	filter.Format( "iseq = '%d'", dtline->drec.m_imgsq );
			//	if( pDBzm->image->Requery( filter ) == 0 )
				if( pDBzm->image->SetAbsolutePosition( dtline->drec.m_imgsq ) == 0 )
				{
					IMREC* pIMREC = pDBzm->image->DecodeImage();
					if( pIMREC != NULL ) {
						data->dd_tek.width	= pIMREC->dotxs;
						data->dd_tek.height	= pIMREC->dotys;
						data->dd_tek.image	= (char*)pIMREC->imdata;
					}
					else {
						data->dd_tek.tekiyo = Image;
						data->dd_tek.width	= 0;
						data->dd_tek.height	= 0;
						data->dd_tek.image	= NULL;
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
				data->dd_tek.tekiyo = dtline->drec.m_tekiyo;
				data->dd_tek.image = 0;
				data->dd_tek.width = 0;
				data->dd_tek.height = 0;
			}
			break;

		case AC_HJIYU_PN:
			data->dd_hjiyu = dtline->lcnd[AC_HJIYU_PN].Etcmsg;
			break;

		case AC_SZAN_PN:
//L_PRINT( "!! termdataset szan", dtline->szan );
#ifdef OLD_CLOSE	// 10.25 /12
			data->dd_udtbl[1].userdata = (BYTE*)dtline->szan;
			if( (m_BaseAtt&0x01) )	l_neg( data->dd_udtbl[1].userdata );
#else
			{
				static char kin[6];
				memcpy( kin, dtline->szan, 6 );
				data->dd_udtbl[1].userdata = (BYTE*)kin;
				if( (m_BaseAtt&0x01) )	l_neg( data->dd_udtbl[1].userdata );
			}
#endif
			data->dd_udtbl[1].initflag = dtline->lcnd[AC_SZAN_PN].INP_sg;
			break;

		case AC_HUSEN_PN:
			data->dd_husen		= (dtline->drec.m_dsign[7]&0x0f);
			data->dd_hscomment	= dtline->drec.m_tag;
			break;

/*		case AC_SYON_PN:
			data->dd_snumber = dtline->drec.m_snumber;
			break;
		case AC_TGDT_PN:
			data->dd_tdate = dtline->drec.m_tdate;
			pDBzm->IntsrekitoGenstr( data->dd_tdstr.GetBuffer(30), data->dd_tdate );
			data->dd_tdstr.ReleaseBuffer();
			break;

		case SKIN_PN:

			break;
*/		}
	}
}


int* CDBAbookView::acfnINP_mode()
{
	return &bINP_mode;
}


// 消費税イニシャライズ
int CDBAbookView::DB_SyIniz( CDBINPDataRec* data )
{
	pDBzm->SetCDBData( data );
	int st = pDBsy->SyIniz( pDBzm->dbdata );
//TRACE( "****>>> pDBsy->SyIniz st = %d, seq = %d\n", st, pDBzm->dbdata->seq );
	return st;
}

char *CDBAbookView::sy_chk_txt( CDBINPDataRec *data,  struct _SY_MSG_PACK2* smp, int inp_mode )
{
//ASSERT( data->m_seq == pDBzm->dbdata->seq );

	static CString txt;

	struct _SY_MSG_PACK2 smpk, *pSmp;
	pSmp = (smp == NULL) ? &smpk : smp;

	pDBzm->SetCDBData( data );

//MyTrace( ">>> sy_chk_txt seq = %d (%02x%02x%02x)\n", pDBzm->dbdata->seq, data->m_dsign[0], data->m_dsign[1], data->m_dsign[2] );

	if( pDBsy->SyMsg( pSmp ) == 0) {
	//	txt = pSmp->SY_LIN_MSG;
		txt = pDBsy->Sy_get_message( pDBzm->dbdata, SYGET_TYPE );
		data->m_dsign.Copy( pDBzm->dbdata->dsign );

//MyTrace( ">>> CDBAbookView::sy_chk_txt(2) SyMsg zei = %s\n", pSmp->SY_MSG_SYZEI );
	}
	else	txt.Empty();

#ifdef OLD_CLOSE
	pDBsy->Sy_chk_sign( data, (inp_mode != _APPEND ? 1 : 0 ) );

	if( inp_mode == _APPEND )
		if( m_TKsyz )
			pDBsy->Sy_tkyo_inp( data, m_TKsyz, 1 );	// 摘要属性
			
	txt = pDBsy->getSYLINETXT();
#endif
//MyTrace( ">>> sy_chk_txt END txt = %s (%02x%02x%02x)\n", txt, data->m_dsign[0], data->m_dsign[1], data->m_dsign[2] );

	systr_adj( txt );

	return (char*)(LPCTSTR)txt;

}

// Sy_line_dsp() text get
char *CDBAbookView::sy_line_txt( CDBINPDataRec *data )
{
ASSERT( data->m_seq == pDBzm->dbdata->seq );

	static CString txt;
//	if( data->m_seq != pDBzm->dbdata->seq ) {
//		pDBzm->SetCDBData( data );
//	}
	txt = pDBsy->Sy_get_message( pDBzm->dbdata, SYGET_TYPE );

#ifdef KEEP
	struct _SY_MSG_PACK2 smpk;
	if( pDBsy->SyMsg( &smpk ) == 0 ) {
		txt = smpk.SY_LIN_MSG;
	}
	else	txt.Empty();
#endif

	systr_adj( txt );

	return (char*)(LPCTSTR)txt;

}


/*===============================
	枝番摘要toデータ摘要
================================*/
BOOL CDBAbookView::brnitm_to_tky( int ln, int bmn, CString& code, int brn )
{
	int dc_sw, tno;
	CDBipTKREC tkrec;

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
				//同一文字列があるか？
				if( IsIncludeString( m_CREC.m_tekiyo, tkrec.m_tkname ) )
					return TRUE;

				//摘要toデータ
				dc_sw = tky_select_to_drec( &m_CREC, &tkrec );
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
				//同一文字列があるか？
				if( IsIncludeString( m_CREC.m_tekiyo, tkrec.m_tkname ) )
					return TRUE;
			}
			//表示
			tky_select_dsp( &m_CREC, dc_sw, &tkrec, FALSE );
			//データセットのためのダミーフォーカスＯＦＦ

			_getLINE_DATA( ln )->drec.m_tekiyo = m_CREC.m_tekiyo;
			TerminationDataSet(AC_TKY_PN);
		//	TerminationTekiyo( this, ln, 0, 0, 0 );

			return TRUE;
		}
	}
	return FALSE;
}



BEGIN_EVENTSINK_MAP(CDBAbookView, CDBbaseView)
    //{{AFX_EVENTSINK_MAP(CDBAbookView)
	ON_EVENT_RANGE(CDBAbookView, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDTCTRL5, 3 /* SetFocus */, OnSetFocusIcsdbedtctrl, VTS_I4)
	ON_EVENT_RANGE(CDBAbookView, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDTCTRL5, 2 /* KillFocus */, OnKillFocusIcsdbedtctrl, VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CDBAbookView, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDTCTRL5, 1 /* Termination */, OnTerminationIcsdbedtctrl, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CDBAbookView, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDTCTRL5, 5 /* ImeEndComposition */, OnImeEndCompositionIcsdbedtctrl, VTS_I4 VTS_I2 VTS_BSTR VTS_BSTR)
	ON_EVENT_RANGE(CDBAbookView, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDTCTRL5, 4 /* KanaTermination */, OnKanaTerminationIcsdbedtctrl, VTS_I4 VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CDBAbookView, IDC_ICSSELCTRL1, 1, CDBAbookView::SelectedIcsselctrl1, VTS_I2 VTS_I4)
	ON_EVENT(CDBAbookView, IDC_ICSSELCTRL2, 1, CDBAbookView::SelectedIcsselctrl2, VTS_I2 VTS_I4)
END_EVENTSINK_MAP()


// DBEDIT セットフォーカス	のまとまり
//
void CDBAbookView::OnSetFocusIcsdbedtctrl(UINT ID)
{
	if( ! PROGRAM_OK )	return;

	switch( ID ) {
	case IDC_ICSDBEDTCTRL1: 
		OnSetFocusIcsdbedtctrl1();	break;
	case IDC_ICSDBEDTCTRL2:
		OnSetFocusBaseBumon();		break;
	case IDC_ICSDBEDTCTRL3:
		OnSetFocusBaseKoji();		break;
	case IDC_ICSDBEDTCTRL4:
		OnSetFocusBaseKamok();		break;
	case IDC_ICSDBEDTCTRL5:
		OnSetFocusBaseBrn();		break;
	}
}

// DBEDIT KILLフォーカス	のまとまり
//
void CDBAbookView::OnKillFocusIcsdbedtctrl(UINT ID, long inplen) 
{
	if( ! PROGRAM_OK )	return;

	switch( ID ) {
	case IDC_ICSDBEDTCTRL1: 
		OnKillFocusIcsdbedtctrl1( inplen );	break;
	case IDC_ICSDBEDTCTRL2:
		OnKillFocusBaseBumon( inplen );		break;
	case IDC_ICSDBEDTCTRL3:
		OnKillFocusBaseKoji( inplen );		break;
	case IDC_ICSDBEDTCTRL4:
		OnKillFocusBaseKamok( inplen );		break;
	case IDC_ICSDBEDTCTRL5:
		OnKillFocusBaseBrn( inplen );		break;
	}
}

// DBEDIT ターミネーション	のまとまり
//
void CDBAbookView::OnTerminationIcsdbedtctrl(UINT ID, long nChar, long inplen, long kst) 
{
	if( ! PROGRAM_OK )	return;

	switch( ID ) {
	case IDC_ICSDBEDTCTRL1:
		OnTerminationIcsdbedtctrl1( nChar, inplen, kst );	break;
	case IDC_ICSDBEDTCTRL2:
		OnTerminationBaseBumon( nChar, inplen, kst );		break;
	case IDC_ICSDBEDTCTRL3:
		OnTerminationBaseKoji( nChar, inplen, kst );		break;
	case IDC_ICSDBEDTCTRL4:
		OnTerminationBaseKamok( nChar, inplen, kst );		break;
	case IDC_ICSDBEDTCTRL5:
		OnTerminationBaseBrn( nChar, inplen, kst );			break;
	}
}

// DBEDIT IME End	のまとまり
//
void CDBAbookView::OnImeEndCompositionIcsdbedtctrl(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring) 
{
	if( ! PROGRAM_OK )	return;
	int ret, inplen = 1;

	switch( ID ) {
	case IDC_ICSDBEDTCTRL1:
		OnImeEndCompositionIcsdbedtctrl1( nChar, string, ystring );	break;
	case IDC_ICSDBEDTCTRL4:
		OnImeEndCompositionBaseKamok( nChar, string, ystring );		break;
	case IDC_ICSDBEDTCTRL3:	//工事番号
		if( CheckImeEndKoji( AC_SELKOJI_PN, nChar, string ) != 0 )
			return;
		OnTerminationBaseKoji( nChar, 1, 0 );
		break;

	case IDC_ICSDBEDTCTRL5:	//枝番
		ret = CheckImeEndBaseBrn(nChar, string);
		if( ret == 1 )
			return;
		OnTerminationBaseBrn( nChar, 1, 0, ret == 2 ? 1 :0 );
		break;

	case IDC_ICSDBEDTCTRL2:	//部門番号
		if( CheckImeEndBumon( AC_SELBMON_PN, nChar, string ) != 0 )
			return;
		OnTerminationBaseBumon( nChar, 1, 0 );
		break;

	}
}

// DBEDIT カナターミネーション	のまとまり
void CDBAbookView::OnKanaTerminationIcsdbedtctrl(UINT ID, LPCTSTR Kana) 
{
	if( ! PROGRAM_OK )	return;

	switch( ID ) {
	case IDC_ICSDBEDTCTRL1:
	case IDC_ICSDBEDTCTRL4:
		KanaTermKamoku( Kana );	break;
//	case IDC_ICSDBEDTCTRL4:
//		OnImeEndCompositionBaseKamok( Kana );		break;
	}
}


//////////////////////////////////////////////////////////////////////////////////
//	基本項目 部門 セットフォーカス
//
void CDBAbookView::OnSetFocusBaseBumon()
{
	set_nowpn( AC_SELBMON_PN );

	SelPar par;

	if( m_SelDispPN != SL_BUMON_PN && m_SelDispPN != SL_BMNKANA_PN ) {
		par.sel_pn = SL_BUMON_PN;
		par.sel_flg = 0;

		m_Seldata.SelectJob(&par, -1);
	}

	SetBmnEtcDisp( TRUE );
}

//	基本項目 部門 キルフォーカス
void CDBAbookView::OnKillFocusBaseBumon(long inplen)
{
	OnTerminationBaseBumon( 0, inplen, 0);
}

//	基本項目 部門 ターミネーション
void CDBAbookView::OnTerminationBaseBumon(long nChar, long inplen, long kst)
{
	char bf[64];
	int bmon, prev, len, number, n, sel_sw;
	CDBINPDataRec	rec;
	VARIANT var;
	BOOL bScrnNo;	//:TRUE 部門選択 画面番号優先

	bScrnNo = (pAUTOSEL->BMNSEL_OPT == OPT_BMNSEL);

	// 直前のデータを保存
	prev = m_BaseBmn;

	DBdata_get( this, IDC_ICSDBEDTCTRL2, &var, ICSDBEDT_TYPE_STRING, 0 );

	if( var.pbVal != NULL )
		strcpy_s( bf, sizeof bf, (const char*)var.pbVal );
	len = 0;

	len = strlen( (const char*)bf );
	bmon = -1;

	if( len > 0 ) {
		bmon = atoi( (LPCTSTR)bf );
	}

	sel_sw = 1;

#ifdef _2KT_BSEL	// 2桁で選択画面より番号選択
	if( inplen && (len  > 0 && len <= 2) && bScrnNo ) {
		// 選択画面より番号選択
		number = atoi( bf );
		CString bmnname;
		if( bmncode_todata2( &rec, number, 0, &bmnname ) != FALSE ) {
			bmon = rec.m_dbmn;
		}
		else {
			bmon = m_BaseBmn;
			sel_sw = 0;
			Buzzer();
		}
	}
	else if( inplen && len > 0 ) {
#else
	else {
#endif

		// 部門を手入力
		n = BMON_LNG-len;
			
		if( n >= 0 ) {
			bmon = atoi( bf );

			if( pDBzm->IsGokeiBumon( bmon ) ) {
				PROGRAM_OK = FALSE;
				myICSMessageBox("合計部門は入力できません！");
				PROGRAM_OK = TRUE;
				// 合計部門は入力不可
				nChar = 0;
				bmon = prev;
			}
			else if( CheckBaseBumonInput( bmon ) ) {
				nChar = 0;
				bmon = prev;
			}
		}
		else {
			bmon = -1;	//入力桁数オーバー
		}
	}
	// 画面番号優先でない場合
	if( inplen && nChar == VK_TAB && ! bScrnNo ) {
		number = atoi( bf );
		CString	bmnname;
		if( bmncode_todata2( &rec, number, 0, NULL ) != FALSE ) {
			if( CheckBaseBumonInput( rec.m_dbmn ) ) {
				bmon = prev;
			}
			else {
				bmon = rec.m_dbmn;
			}
		}
		else {
			bmon = m_BaseBmn;
			Buzzer();
		}
	}

	if( nChar != 0 && nChar != VK_DELETE ) {
		if( BmnCodeChk( bmon, sel_sw, AC_SELBMON_PN ) ) {
			return;
		}
	}

	pDBzm->BumonCodeToStr( bf, sizeof bf, bmon );
	var.pbVal = (BYTE*)bf;
	DBdata_set( this, IDC_ICSDBEDTCTRL2, &var, ICSDBEDT_TYPE_STRING, 0 );
	if( inplen ) {
		check_basebrn();
		m_BaseBmn = bmon;
		// 部門名称を表示
		m_BmnSK.bmn = m_BaseBmn;
		if( ! _set_bmnetc( &m_BmnSK ) ) {
			m_BmnSK.bmn = -1;
			m_BmnSK.etc.Empty();
			m_BmnSK.etc_col = (DWORD)-1;
		}
		SetBmnEtcDisp( TRUE );
		BaseBmnEda_update();
	}

	// 部門名称を表示
	if( !nChar ) {
		SetBmnEtcDisp(TRUE);
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		nChar = VK_F2;

	if( nChar ) {
		nChar = SelBmonDsp( nChar );
	}

	if( nChar == VK_RETURN || nChar == VK_RIGHT || nChar == VK_TAB ) {
		if( IsMoveBaseKOJI() )	set_focus( AC_SELKOJI_PN );
		else					set_focus( AC_SELKAMOK_PN );
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 ) {
		set_focus( AC_SELDISP_PN );
	}
	else if( nChar == VK_DELETE ) {
		m_BaseBmn = -1;
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDTCTRL2, &var, ICSDBEDT_TYPE_STRING, 0 );

		m_BmnSK.bmn = -1;
		m_BmnSK.etc.Empty();
		m_BmnSK.etc_col = (DWORD)-1;

		SetBmnEtcDisp( FALSE );
		BaseBmnEda_update();
	}
}

//////////////////////////////////////////////////////////////////////////////////
//	基本項目 工事番号 セットフォーカス
//
void CDBAbookView::OnSetFocusBaseKoji()
{
	set_nowpn( AC_SELKOJI_PN );

	SelPar par;
	par.sel_pn = SL_KOJI_PN;
	par.sel_flg = 0;

	m_Seldata.SelectJob(&par, -1);

	SetKojiEtcDisp( TRUE );
}


//	基本項目 工事番号 キルフォーカス
void CDBAbookView::OnKillFocusBaseKoji(long inplen)
{
	OnTerminationBaseKoji( 0, inplen, 0 );
}

//	基本項目 工事番号 ターミネーション
void CDBAbookView::OnTerminationBaseKoji(long nChar, long inplen, long kst)
{
	char bf[64];
	CString koji, prev;
	int len, number, n;
	CDBINPDataRec	rec;
	VARIANT var;
	BOOL bScrnNo;	//:TRUE 部門選択 画面番号優先

	bScrnNo = (pAUTOSEL->BMNSEL_OPT == OPT_BMNSEL);

	// 直前のデータを保存
	prev = m_BaseKoji;

	DBdata_get( this, IDC_ICSDBEDTCTRL3, &var, ICSDBEDT_TYPE_STRING, 0 );

	len = 0;
	if( var.pbVal != NULL ) {
		strcpy_s( bf, sizeof bf, (const char*)var.pbVal );
		len = strlen( (const char*)bf );
		trim_zero( bf, sizeof bf, strlen( bf ));
	}
	koji.Empty();

	if( len > 0 ) {
		koji = bf;
	}

#ifdef _2KT_BSEL	// 2桁で選択画面より番号選択
	if( inplen && (len  > 0 && len <= 2) && isdigitstr( bf ) && bScrnNo ) {
		// 選択画面より番号選択
		number = atoi( bf );
		if( kojicode_todata( &rec, NULL, number, 0 ) != FALSE ) {
			koji = rec.m_dkno;
		}
		else {
			koji = m_BaseKoji;
			Buzzer();
		}
	}
	else if( inplen && len > 2 ) {
#else
	else {
#endif

		// 工事を手入力
		n = Voln1->kjcol - len;
			
		if( n >= 0 ) {
			// 工事最大桁数まで入力されていない
			koji = bf;

			if( CheckBaseKojiInput( koji ) ) {
				nChar = 0;
				koji = prev;
			}
		}
		else {
			koji.Empty();	//桁数オーバー
		}
	}
	else {
		if( len > 0 ) {
			if( CheckBaseKojiInput( koji ) ) {
				koji = prev;
			}
		}
	}

	if( inplen && nChar == VK_TAB && isdigitstr( bf ) && ! bScrnNo ) {
		// 選択画面より番号選択
		number = atoi( bf );
		if( kojicode_todata( &rec, NULL, number, 0 ) != FALSE ) {
			if( CheckBaseKojiInput( rec.m_dkno ) ) {
				koji = prev;
			}
			else {
				koji = rec.m_dkno;
			}
		}
		else {
			koji = m_BaseKoji;
			Buzzer();
		}
	}

	pDBzm->KojiCodeToStr( bf, sizeof bf, koji );
	var.pbVal = (BYTE*)bf;
	DBdata_set( this, IDC_ICSDBEDTCTRL3, &var, ICSDBEDT_TYPE_STRING, 0 );

	if( inplen ) {
		koji.TrimRight();
		m_BaseKoji = koji;
		// 工事名称を表示
		m_KojiSK.bmn = -1;
		m_KojiSK.kno = m_BaseKoji;
		m_KojiSK.brn = -1;
		m_KojiSK.code.Empty();

		if( ! _set_kojietc( &m_KojiSK ) ) {
			m_KojiSK.kno.Empty();
			m_KojiSK.etc.Empty();
			m_KojiSK.etc_col = (DWORD)-1;
		}
		SetKojiEtcDisp( TRUE );
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		nChar = VK_F2;

	if( nChar ) {
		nChar = SelKojiDsp( nChar );
		if( ! nChar )
			return;
	}
	// 工事名称を表示
	if( ! nChar )
		SetKojiEtcDisp( TRUE );

	if( nChar == VK_RETURN || nChar == VK_RIGHT || nChar == VK_TAB ) {
		set_focus( AC_SELKAMOK_PN );
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 ) {
		if( IsMoveBaseBmon() )	set_focus( AC_SELBMON_PN );
		else					set_focus( AC_SELDISP_PN );
	}
	else if( nChar == VK_DELETE ) {
		m_BaseKoji.Empty();
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDTCTRL3, &var, ICSDBEDT_TYPE_STRING, 0 );

		m_KojiSK.bmn = -1;
		m_KojiSK.kno.Empty();
		m_KojiSK.etc.Empty();
		m_KojiSK.etc_col = (DWORD)-1;

		SetKojiEtcDisp( FALSE );
	}
}


// 工事番号IMEのEND処理
int CDBAbookView::CheckImeEndKoji( int pn, long nChar, LPCTSTR string ) 
{
	VARIANT var;

	CString buf = string;
	int		ret = 0;
	UINT id;

	if( pn == AC_KOJI_PN ){
		id = IDC_ICSDBEDTCTRL1;
	}
	else {
		id = IDC_ICSDBEDTCTRL3;
		if( CheckBaseKojiInput( buf ) ) {
			var.pbVal = NULL;
			DBdata_set( this, id, &var, ICSDBEDT_TYPE_STRING, 0 );
			return 0;
		}
	}

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
		else {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();
			DBdata_set( this, id, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else {
		kana_chk( &buf, buf.GetLength() );

		var.pbVal = (BYTE*)buf.GetBuffer(64);
		buf.ReleaseBuffer();
		DBdata_set( this, id, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////////////
//	基本項目 科目 セットフォーカス
//
void CDBAbookView::OnSetFocusBaseKamok()
{
	set_nowpn( AC_SELKAMOK_PN );

	SelPar par;
	par.sel_pn = SL_KAMOKU_PN;
	m_Seldata.SelectJob(&par, -1);
}

//	基本項目 科目 キルフォーカス
void CDBAbookView::OnKillFocusBaseKamok(long inplen)
{
}

//	基本項目 科目 ターミネーション
void CDBAbookView::OnTerminationBaseKamok(long nChar, long inplen, long kst)
{
	if( ! PROGRAM_OK )	return;

	struct _KamokuData check = {0};
	DBKNREC* pKn;

	if( nChar == 0x16 ) {

		::ZeroMemory( (char *)&check, sizeof(check) );
		memcpy( check.kd_code, m_BaseCode, KMKCD_LN );
		check.kd_eda = m_BaseEda;
		pKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
		if( pKn ) {
			KamokuString( pKn, check.kd_name, sizeof check.kd_name );
		//	memcpy( check.kd_name, pKn->knnam, sizeof pKn->knnam );
		}

		DBkamokudata_set( this, IDC_ICSDBEDTCTRL4, &check, pDBzm );
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		nChar = VK_F2;

	if( nChar ) {
		SelPar par;
		switch( m_SelDispPN ) {
		case SL_KAMOKU_PN:
		case SL_KMKANA_PN:
		case SL_BRNTKY_PN:
			par.sel_pn = m_SelDispPN;
			m_Seldata.SelectJob( &par, nChar );

			switch( nChar ) {
			case VK_PRIOR: case VK_NEXT:
			case VK_HOME:
				nChar = 0;
				break;
			}
			break;
		}
	}

	if( nChar == VK_RETURN || nChar == VK_RIGHT || nChar == VK_TAB ) {
		if( IsMoveBaseEda() )	set_focus( AC_SELBRN_PN );
		else					set_focus( AC_SELDISP_PN );
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 ) {
		if( IsMoveBaseKOJI() )		set_focus( AC_SELKOJI_PN );
		else if( IsMoveBaseBmon() )	set_focus( AC_SELBMON_PN );
		else {
			set_focus( AC_SELDISP_PN );
		}
	}
}

//基本科目 移動可能か？
BOOL CDBAbookView::IsMoveBaseEda()
{
	if( BRmst && (m_dwQuery & ACQ_EDA) )
		return TRUE;

	return FALSE;
}

BOOL CDBAbookView::IsMoveBaseKOJI()
{
	if( M_KOJI && (m_dwQuery & ACQ_KOJI) )
		return TRUE;

	return FALSE;
}

BOOL CDBAbookView::IsMoveBaseBmon()
{
	if( BMON_MST && (m_dwQuery & ACQ_BUMON) ) {

		if( Voln1->bm_isw & 0x10 ) {// 損益のみ
			DWORD dwCode = _atoh( (LPSTR)(LPCTSTR)m_BaseCode );
			if( dwCode < SONEKICODE ) 
				return FALSE;
		}
		return TRUE;
	}

	return FALSE;
}

//	基本項目 科目 IME End
void CDBAbookView::OnImeEndCompositionBaseKamok(short nChar, LPCTSTR string, LPCTSTR ystring)
{
	if( ! PROGRAM_OK )	return;

	char tmp[128] = { 0 };
	if( !lastBrnInpStr(string, tmp, sizeof tmp) )
		return;

	if( CheckDataBaseKamoku( nChar, tmp) <= 0 )
		return;
	OnTerminationBaseKamok( nChar, 1, 0 );
}

// 基本科目チェックデータ
long CDBAbookView::CheckDataBaseKamoku( long nChar, LPCTSTR string ) 
{
	CString kmkstr, ldata_code;
	int edaban = -1, ldata_brn = -1;
	struct _KamokuData check = {0}, ans = {0};
	DBKNREC* pKn;

	KamokuStr_ChkLength( string, &check, 0 );
	check.kd_eda = -1;

	if( m_Seldata.InputItemCheck( ans, check, 0 ) == TRUE )
		/*&&	CheckBaseKamokuInput( &ans ) == 0 )*/
	{
		DBkamokudata_set( this, IDC_ICSDBEDTCTRL4, &ans, pDBzm );
		m_BaseCode = ans.kd_code;
		pKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
		if( pKn ) {
			m_BaseAtt = pKn->knatt;
		}

		check_basebrn();

		BaseBmnCtrl();
		BaseKojiCtrl();
	}
	else {
		Buzzer();

		::ZeroMemory( (char *)&ans, sizeof(ans) );
		memcpy( ans.kd_code, m_BaseCode, KMKCD_LN );
		ans.kd_eda = -1;
		pKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
		if( pKn ) {
			KamokuString( pKn, ans.kd_name, sizeof ans.kd_name );
		//	memcpy( ans.kd_name, pKn->knnam, sizeof pKn->knnam );
		}

		DBkamokudata_set( this, IDC_ICSDBEDTCTRL4, &ans, pDBzm );
		return 0;
	}

	return nChar;
}

//////////////////////////////////////////////////////////////////////////////////
//	基本項目 枝番 セットフォーカス
//
void CDBAbookView::OnSetFocusBaseBrn()
{
	set_nowpn( AC_SELBRN_PN );

	if( EdabanDispReload(m_BaseCode, m_BaseBmn) ) {
		m_Seldata.BrnTekClear();
	}

	SelPar par;
	par.sel_pn	= SL_BRNTKY_PN;
	par.tkybrn	= m_BaseCode;
	par.brn_bmn	= m_BaseBmn;
	par.brn_cd	= -1;
	::ZeroMemory( par.brn_kana, sizeof par.brn_kana );
	
	int st = m_Seldata.SelectJob(&par, -1);

	if( BRNTKY_sel == 0 && m_SelDispPN != SL_KAMOKU_PN ) {
		SelPar par;
		par.sel_pn = SL_KAMOKU_PN;
		m_Seldata.SelectJob(&par, -1);
	}
}


//	基本項目 枝番 キルフォーカス
void CDBAbookView::OnKillFocusBaseBrn(long inplen)
{
	OnTerminationBaseBrn( 0, inplen, 0 );
}

//	基本項目 枝番 ターミネーション
//	priodinp 「.+番号」での枝番入力
void CDBAbookView::OnTerminationBaseBrn(long nChar, long inplen, long kst, int priodinp/*=0*/)
{
	int n, num;
	VARIANT var;
	BOOL	bSetNew = FALSE;
	char str[128] = {0};
	n = num = 0;

	if( inplen ) {
		DBdata_get( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
		if( var.pbVal != NULL )	
			n = strlen( (const char*)var.pbVal );
	}

	if( n && n < 3 && (m_SelDispPN == SL_BRNTKY_PN) && !priodinp )
	{
		num = atoi( (const char*)var.pbVal );

		if( num > 8 ) {
			SelResult srt;
			sprintf_s( str, sizeof str, "%d", num );

			srt.sel_pn = SL_BRNTKY_PN;
			if( m_Seldata.GetSelResult( &srt, str ) ) {

				m_BaseEda = srt.brn_no;
				//コントロールへ再セット
				pDBzm->EdabanToStr( str, sizeof str,m_BaseEda );
				var.pbVal = (BYTE*)str;
				DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
				bSetNew = TRUE;
			}
		}
		// 元の枝番をセット
		if( ! bSetNew ) {
			pDBzm->EdabanToStr( str, sizeof str,m_BaseEda );
			var.pbVal = (BYTE*)str;
			DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else if( n != 0 ) {
		// 枝番手入力
		num = atoi( (const char*)var.pbVal );
		m_BaseEda = num;
		pDBzm->EdabanToStr( str, sizeof str,m_BaseEda );
		var.pbVal = (BYTE*)str;
		DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	else if( n == 0 ) {
		// BackSpaceで 枝番消去
		if( inplen < 0 ) {
			m_BaseEda = -1;
			var.pbVal = NULL;
			DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
			GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
		}
	}

	// 枝番名称を表示
	if( inplen ) {
		struct _SET_KMKETC sk;
		sk.bmn = m_BaseBmn;
		sk.brn = m_BaseEda;
		sk.code = m_BaseCode;

		if( _set_kmketc( &sk, FALSE ) ) {
			GetDlgItem(IDC_STATIC5)->SetWindowText( sk.etc );
		}
		else {
			GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
		}
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		nChar = VK_F2;

	if( nChar ) {
		SelPar par;
		switch( m_SelDispPN ) {
		case SL_BRNTKY_PN:
		case SL_KAMOKU_PN:
		//	par.sel_pn = m_SelDispPN;
			SelPar par;
			par.sel_pn	= SL_BRNTKY_PN;
			par.tkybrn	= m_BaseCode;
			par.brn_bmn	= m_BaseBmn;
			par.brn_cd	= -1;
			::ZeroMemory( par.brn_kana, sizeof par.brn_kana );
		//	m_Seldata.SelectJob( &par,  -1 );
			m_Seldata.SelectJob( &par, nChar == VK_HOME ? -1 : nChar );

			switch( nChar ) {
			case VK_PRIOR: case VK_NEXT:
			case VK_HOME:
				nChar = 0;
				break;
			}
			break;
		}
	}

	if( nChar == VK_RETURN || nChar == VK_RIGHT || nChar == VK_TAB ) {
		set_focus( AC_SELDISP_PN );
	}
	else if( nChar == VK_LEFT || nChar == VK_F2 ) {
		set_focus( AC_SELKAMOK_PN );
	}
	else if( nChar == VK_DELETE ) {
		m_BaseEda = -1;
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
		GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
	}
}


// 基本項目の枝番 IMEのEND処理
int CDBAbookView::CheckImeEndBaseBrn( long nChar, LPCTSTR string ) 
{
	VARIANT var;
	char tmp[128] = { 0 };

	if( !lastBrnInpStr(string, tmp, sizeof tmp) ) {
		strcpy_s(tmp, sizeof tmp, string);
	}

	CString buf = tmp;
	int		ret = 0;
	UINT id = IDC_ICSDBEDTCTRL5;

	if( nChar == VK_TAB || nChar == VK_F12 ) {
		// カナ検索
		if( ! isdigitstr( (char*)tmp) ) {
			SelPar par;

			for( int k = 0; k < 2; k++ ) {
				par.sel_pn	= SL_BRNTKY_PN;
				par.tkybrn	= m_BaseCode;
				par.brn_bmn	= m_BaseBmn;
				par.brn_cd	= -1;
				::ZeroMemory( par.brn_kana, sizeof par.brn_kana );
				if( k == 0 ) {
					memcpy( par.brn_kana, tmp, sizeof par.brn_kana );
				}

				int st = m_Seldata.SelectJob( &par, -1 );

				// 検索結果なしの場合、再度全枝番で検索
				if( m_SelDispPN != SL_BRNTKY_PN )	continue;
				else								break;
			}
			ret = 1;
		}
		else {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();
			DBdata_set( this, id, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else {
		if( isdigitstr( (char*)tmp ) ) {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();
			DBdata_set( this, id, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
		else {
			if( tmp[0] == '.' && isdigitstr( (char*)(tmp +1) ) ) {
				int brn;
				brn = atoi( (LPCTSTR)(tmp +1) );

				LPSTR pBuf = buf.GetBuffer(64);
				pDBzm->EdabanToStr( pBuf, 64, brn );

				var.pbVal = NULL;
				DBdata_set( this, id, &var, ICSDBEDT_TYPE_STRING, 0 );

				var.pbVal = (BYTE*)pBuf;
				buf.ReleaseBuffer();
				DBdata_set( this, id, &var, ICSDBEDT_TYPE_STRING, 0 );
				ret = 2;
			}
			else {
				ret = 1;
			}
		}
	}

	return ret;
}



// リストの中身の DBEDIT セットフォーカス
void CDBAbookView::OnSetFocusIcsdbedtctrl1()
{
	if( PROGRAM_OK == FALSE )
		return;

	if( ! ::IsWindow( m_Input.m_hWnd ) )
		return;

//TRACE( "++Abook++ **SetFocus\n" );	
	DT_EDIT edit;

	edit.hdr.hwndFrom = m_Input.m_hWnd;
	edit.hdr.idFrom   = IDC_ICSDBEDTCTRL1;
	edit.hdr.code     = DLUM_EDITFOCUS;

	m_List.SendMessage(WM_DTLISTUSER, edit.hdr.code, (LPARAM)&edit );

}

// リストの中身の DBEDIT キルフォーカス
void CDBAbookView::OnKillFocusIcsdbedtctrl1(long inplen) 
{
//TRACE( "++Abook++ **killFocus , pok %d\n", PROGRAM_OK );	

	_OnTerminationIcsdbedtctrl( 0, inplen, 0 );
	
}

// リストの中身の DBEDIT ターミネーション
void CDBAbookView::OnTerminationIcsdbedtctrl1(long nChar, long inplen, long kst) 
{
	if( ! ::IsWindow( m_Input.m_hWnd ) )
		return;

	_OnTerminationIcsdbedtctrl( nChar, inplen, kst );
}

// リストの中身の DBEDIT IME END
void CDBAbookView::OnImeEndCompositionIcsdbedtctrl1(short nChar, LPCTSTR string, LPCTSTR ystring) 
{
	if( ! ::IsWindow( m_Input.m_hWnd ) )
		return;

	if( PROGRAM_OK == FALSE )
		return;
	int st, term, pos, len;
	VARIANT var;
	char tmp[128];
	char ystr[128];

	switch( get_nowpn() ) {
	case AC_KMK_PN:
		if( !lastBrnInpStr(string, tmp, sizeof tmp) )
			return;

		if( CheckDataKamoku( nChar, tmp) <= 0 )
			return;
		TerminationKamoku( get_nowln(), nChar, 1, 0 );
		break;

	case AC_KOJI_PN:
		if( CheckImeEndKoji( AC_KOJI_PN, nChar, string ) != 0 )
			return;
		TerminationKoji( get_nowln(), nChar, 1, 0 );
		break;

	case AC_BMON_PN:
		if( CheckImeEndBumon( AC_BMON_PN, nChar, string ) != 0 )
			return;
		TerminationBumon ( get_nowln(), nChar, 1, 0 );
		break;

	case AC_TKY_PN:
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
						_KanaTermIcsTekiyo( ystr, FALSE );
						term = 0;
					}
				}
				if( nChar == VK_TAB ) {
					if( ! isdigitstr( (char*)ystr ) ) {
						_KanaTermIcsTekiyo( ystr, FALSE );
						term = 0;
					}
					else {
						if( TKtabMode() ) {
							_KanaTermIcsTekiyo( ystr, TRUE );
							term = 0;
						}
					}
				}
				else if( nChar == VK_F12 ) {
					if( ! isdigitstr( (char*)ystr ) ) {
						_KanaTermIcsTekiyo( ystr, FALSE );
						term = 0;
					}
				}
			}
			else {
				if( nChar == VK_TAB || nChar == VK_F12 ) {
					_KanaTermIcsTekiyo( ystr, FALSE );
					term = 0;
				}
			}
		}
		if( term ) {
			var.pbVal = (BYTE*)tmp;
			pos = m_Input.GetCaretPosition();
			m_Input.InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );
		//	TerminationIcsinputTky( this, get_nowln(), nChar );
		}
		break;

	default:
		ASSERT(FALSE);
		break;
	}	
}


//--------------------------------------------------------
//	ICSDBEDT のターミネーション処理
//		関数の最後の END_TAG 部分を必ず実行すること
//		CDataListCtrl に 入力ｳｨﾝﾄﾞｳの状態を知らせるため
//--------------------------------------------------------
void CDBAbookView::_OnTerminationIcsdbedtctrl( long nChar, long inplen, long kst )
{
	int ln, pn;

//MyTrace( "++Abook++ _OnTerminationIcsdbedtctrl1 nChar %04x, inplen %d, ln %d, pn %d, tabclick(%d)\n", nChar, inplen, get_nowln(), get_nowpn(),
//																						((CMainFrame*)GetDBDinpMainFrame())->TabClickChange());

	if( PROGRAM_OK == FALSE )
		goto END_TAG;

	if( ((CMainFrame*)GetDBDinpMainFrame())->TabClickChange() ) {
		goto END_TAG;
	}

	ln = get_nowln();
	if( ln == -1 )	goto END_TAG;
	pn = get_nowpn();

	if( DenpTypeMode() ) {
		if( getLINE_DATA()->drec.m_icno > 0 ) {
			if( pn != AC_DATE_PN )
				goto END_TAG;
		}
	}

	switch( pn )
	{
	case AC_SEQ_PN:
//		TerminationIcsinputSeq( nChar, inplen, kst ); 
		break;
	case AC_DATE_PN:
		if( inplen > 0 || nChar == 0x16 ) {
			if( CheckDataIcsDate( nChar ) < 0 ) {
				goto END_TAG;
			}
		}
		else if( inplen == 0 ) {
			BYTE prev_bcd[4] = {0}, bcd[4] = {0};
			VARIANT var;
			memcpy( prev_bcd, getLINE_DATA()->bcddate, 2 );

			if( ! m_bMnthFix ) {
				DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 4 );
				if( var.pbVal != NULL )	memcpy( bcd, var.pbVal, 3 );
			}
			else {
				DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 2 );
				
				if( var.pbVal != NULL )	memcpy( &bcd[1], var.pbVal, 2 );
				bcd[0] = getLINE_DATA()->bcddate[0];
			}

			if( prev_bcd[0] != bcd[0] || prev_bcd[1] != bcd[1] ) {
				if( CheckDataIcsDate( nChar ) < 0 ) {
					// 不正データが入力された
					ACBOOK_LINE* ldata = getLINE_DATA();
					if( ! m_bMnthFix ) {
						var.pbVal = ldata->bcddate;
						DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 4 );
					}
					else  {
						var.pbVal = &ldata->bcddate[1];
						DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 2 );
					}
					CString str;
					pDBzm->BcdDateToString( str, ldata->bcddate, m_bMnthFix );
					DBSetDispString( this, IDC_ICSDBEDTCTRL1, str );
					goto END_TAG;
				}
			}
		}
		TerminationIcsDate( nChar, inplen, kst ); 
		break;

	case AC_DENP_PN:
		TerminationDenp( get_nowln(), nChar, inplen, kst );
		break;
	case AC_DOCEVI_PN:
		TerminationDocevi( get_nowln(), nChar, inplen, kst );
		break;
	case AC_BMON_PN:
		TerminationBumon( get_nowln(), nChar, inplen, kst );
		break;
	case AC_KOJI_PN:
		TerminationKoji( get_nowln(), nChar, inplen, kst );
		break;

	case AC_KMK_PN:
		if( nChar != VK_DELETE )
			TerminationKamoku( get_nowln(), nChar, inplen, kst );
		break;
	case AC_NKIN_PN:
		TerminationVal( get_nowln(), 0, nChar, inplen, kst );
		break;

	case AC_SKIN_PN:
		TerminationVal( get_nowln(), 1, nChar, inplen, kst );
		break;
	
	case AC_ZEI_PN:
		TerminationZei( get_nowln(), nChar, inplen, kst );
		break;
	case AC_SYZ_PN:
		TerminationSyz( get_nowln(), nChar, inplen, kst );
		break;
	case AC_TKY_PN:
		TerminationTekiyo( get_nowln(), nChar, inplen, kst );
		break;

	case AC_HJIYU_PN:
		OnHjiyuInEnd( nChar, 0 );
		break;

/*	case SYON_PN:
		TerminationIcsinputSyono( get_nowln(), nChar, inplen, kst );
		break;
	case TGDT_PN:
		if( inplen > 0 ) {
			if( CheckDataIcsinputTgDate( nChar ) <= 0 )
				return;
		}
		TerminationIcsinputTgDate( get_nowln(), nChar, inplen, kst );
		break;
*/	default:
//		ASSERT( FALSE );
		break;
	}

	if( nChar == 0 ) {
//TRACE( "m_Input.ShowWindow(SW_HIDE)\n" );
//		m_Input.ShowWindow(SW_HIDE);
//		m_Input.PostMessage( WM_SHOWWINDOW, FALSE, 0 );
	}


END_TAG:

	DT_EDIT edit;

	edit.hdr.hwndFrom = m_Input.GetSafeHwnd();
	edit.hdr.idFrom   = IDC_ICSDBEDTCTRL1;
	edit.hdr.code     = DLN_ENDLABELEDIT;

	edit.nChar = (short)nChar;

	m_List.SendMessage(WM_DTLISTUSER, DLN_ENDLABELEDIT, (LPARAM)&edit );

	if( ((CMainFrame*)GetDBDinpMainFrame())->TabClickChange() ) {
		((CMainFrame*)GetDBDinpMainFrame())->TabClickChange() = FALSE;
	}
//MyTrace( "++Abook++ **_OnTerm END nChar %d\n", nChar );

}


// CMainFrame より、マウスによる画面切替
//
void CDBAbookView::to_acbkmode()
{
	static int init_sgn = 0;

	if( ((CMainFrame*)GetDBDinpMainFrame())->TabClickChange() ) {
		((CMainFrame*)GetDBDinpMainFrame())->TabClickChange() = FALSE;
	}

	if( ! init_sgn ) {
		int date = pDBzm->zvol->ip_ymd;
		date %= 10000;

		VDPAC pac;
		// 年度は 平成年
		if( db_vd_dpacGen( 1, 0, date, &pac, pDBzm ) == 0 ) {
			m_BaseDate = pac.Dymd;
			m_BaseOfst = pac.Doffset;
			m_BaseKsign = (pac.Dsgn & 0x80) ? 1 : 0;

			pDBzm->minfo->MoveLast();
			do {
				if( (pDBzm->minfo->insw & 0x01) ) {
					m_BaseEndDate = pDBzm->minfo->emd;
					m_BaseEndOfs = pDBzm->minfo->ofset;
					break;
				}
			} while( pDBzm->minfo->MovePrev() == 0 );
		}
		else {
			ICSExit( 0, "期間を取得できません" );
			return;
		}

		init_sgn = 1;
	}
	// 兼業のときの基本科目チェック
	BaseKmkIndCheck();

	pDBzm->dbdata_Job()		= DBDATA_JOB_ACBOOK;
	pDBzm->dbdata_Sort()	= DBDATA_SORT_SEQ;
	pDBzm->dbdata_Requery()	= TRUE;
	pDBzm->dbdata_Speed()	= FALSE;
	pDBzm->dbdata->Requery(-2,0,"seq=-1",0,0);	// for speed up from scan

//	m_selDate.Iniz( pDBzm, ICS_DATESELECT_RANGE_PERIOD|ICS_DATESELECT_EDIT_OFF|ICS_DATESELECT_INPOK_ONLY, 0, 0 );
	int sofs, eofs, sdate, edate;
	pDBzm->get_datelimit( sofs, eofs, sdate, edate );
	m_selDate.SetMonthOffset( sofs, eofs, sdate, edate );
	m_selDate.GetMonthOffset( sofs, eofs, sdate, edate );

	SetBmnEtcDisp( TRUE );
	BaseKojiCtrl();
	BaseBmnEda_update();
	//カナ検索桁数
	m_KamokInp.SetKanaTerm(TRUE, KanaSearchKeta(), ICSDBEDT_KTYPE_KANA);

	ListClmSet();

	PostMessage( WM_COMMAND, MAKELONG(ID_BUTTON_F11, 0) );
}


// 借／貸 変更
void CDBAbookView::OnButtonF9()
{
	//取消データ
	if( isdeldata() )	return;

	if( CalcWindowDelete() ) {
		PostMessage(WM_COMMAND, ID_BUTTON_F9);
		return;
	}

	int chg = 0;
	if( getLINE_DATA()->lcnd[AC_NKIN_PN].INP_sg )		chg = 2;	//入金->出金
	else if( getLINE_DATA()->lcnd[AC_SKIN_PN].INP_sg )	chg = 1;	//出金->入金

	int ret = ChangeDebtCred(chg);
	// 借方／貸方が同じ
	if( ret < 0 )	return;

	//消費税 --- 08.30 /12 ------------------
	struct _SY_MSG_PACK2 smp;
	char	zei[6];
	BYTE oldmen = (m_CREC.m_dsign[6]& 0x20);

	sprintf_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, "%s", sy_chk_txt( &m_CREC, &smp, acINP_mode) );

	if( oldmen == 0x20 && !(pDBzm->dbdata->dsign[6] & 0x20) ) {
		// 免税事業者からの仕入れ サインが落ちるので元に戻す
		pDBsy->SyFnc(CD_SP_EXEMPT);
		m_CREC.m_dsign.Copy(pDBzm->dbdata->dsign);
	}

	//消費税再表示
	int pntbl[] = {AC_ZEI_PN, AC_SYZ_PN};
	::ZeroMemory( zei, sizeof zei );
	l_input( zei, (char*)(LPCTSTR)smp.SY_MSG_SYZEI );	// 税額をセット
	memcpy( getLINE_DATA()->drec.m_zei, zei, 6 );
	memmove( m_CREC.m_zei, zei, sizeof(zei) );

	if( is_syohizeisiwake(&m_CREC) ) {
		getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = TRUE;
	}
	else {
		getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = FALSE;
	}
	TerminationDataSet( pntbl, 2 );
	//消費税 --- 08.30 /12 ------------------

	int pn = get_nowpn();
	if( pn == AC_NKIN_PN || pn == AC_SKIN_PN ) {
		if( ret == 0 && pn == AC_SKIN_PN )
			set_focus( AC_NKIN_PN );
		else if( ret == 1 && pn == AC_NKIN_PN )
			set_focus( AC_SKIN_PN );
	}
}


// 借方／貸方の科目・枝番・部門・工事が同じか？
BOOL	CDBAbookView::IsSameKamoku()
{
	ACBOOK_LINE* al = getLINE_DATA();
	
	return _IsSameKamoku( &al->drec );
}


// 借方／貸方の科目・枝番・部門・工事が同じか？
BOOL	CDBAbookView::_IsSameKamoku( CDBINPDataRec* drec ) 
{
	BOOL ret = FALSE;

	if( drec->m_dbmn	== drec->m_cbmn && 
		drec->m_dkno	== drec->m_ckno &&
		drec->m_dbt		== drec->m_cre && 
		drec->m_dbr		== drec->m_cbr )
		ret = TRUE;

	return ret;

}


//-----------------------------------------------------------------------
// 基本科目の登録が、仕訳レコードの借方／貸方どちらか？
//	return	0:	基本科目が借方
//			1:	基本科目が貸方
//		   -1:	同じ科目が指定された
//-----------------------------------------------------------------------
int	CDBAbookView::GetRecordDCsgn( CDBINPDataRec* drec )
{
	int dcsw = -1;

	if( _IsSameKamoku( drec ) )
		return -1;

	WORD debt_bit = 0;
	WORD cred_bit = 0;
	WORD chk_bit = 0x01;	// D0:	科目
	int d_sgn = 0;
	int c_sgn = 0;

	if( m_BaseEda != -1 )
		chk_bit |= 0x02;		// D1:	枝番

	if( BMON_MST ) {
		if( m_BaseBmn != -1 )
			chk_bit |= 0x04;	// D2:	部門
	}
	if( M_KOJI ) {
		if( ! m_BaseKoji.IsEmpty() )
			chk_bit |= 0x08;	// D3:	工事
	}

	if( drec->m_dbt != drec->m_cre ) {
		if( drec->m_dbt == m_BaseCode )
			dcsw = 0;
		else if( drec->m_cre == m_BaseCode )
			dcsw = 1;
	}
	else {
		// 借方／貸方が同じ場合
		// 借方のチェック
		if( drec->m_dbt == m_BaseCode ) {
			debt_bit |= 0x01;

			if( (chk_bit & 0x02) ) {
				if(	drec->m_dbr == m_BaseEda ) {
					debt_bit |= 0x02;
				}
			}
			else {
				// 枝番が入っていたら、基本とは異なる
				if( (m_dwQuery & ACQ_EDA) ) {
					if( drec->m_dbr != -1 )
						d_sgn++;
				}
				else {
					// 11.05 /12
					if( drec->m_dbr != -1 && drec->m_cbr == -1 )
						d_sgn++;
				}
			}

			if( (chk_bit & 0x04) ) {
				if( drec->m_dbmn == m_BaseBmn )
					debt_bit |= 0x04;
			}
			else {
				// 部門が入っていたら、基本とは異なる
				if( (m_dwQuery & ACQ_BUMON) ) {
					if( drec->m_dbmn != -1 )
						d_sgn++;
				}
				else {
					// 11.05 /12
					if( drec->m_dbmn != -1 && drec->m_cbmn == -1 )
						d_sgn++;
				}
			}

			if( (chk_bit & 0x08) ) {
				if( drec->m_dkno == m_BaseKoji ) {
					debt_bit |= 0x08;
				}
			}
			else if( M_KOJI ) {
				// 工事番号が入っていたら、基本とは異なる
				if( (m_dwQuery & ACQ_KOJI) ) {
					if( ! drec->m_dkno.IsEmpty() )
						d_sgn++;
				}
				else {
					// 11.05 /12
					if( ! drec->m_dkno.IsEmpty() && drec->m_ckno.IsEmpty() )
						d_sgn++;
				}
			}

			if( ! d_sgn ) {
				if( debt_bit == chk_bit )
					dcsw = 0;
			}
		}

		// 貸方のチェック
		if( dcsw == -1 ) {
			if( drec->m_cre == m_BaseCode ) {
				cred_bit |= 0x01;

				if( (chk_bit & 0x02) ) {
					if(	drec->m_cbr == m_BaseEda ) {
						cred_bit |= 0x02;
					}
				}
				else {
					// 枝番が入っていたら、基本とは異なる
					if( (m_dwQuery & ACQ_EDA) ) {
						if( drec->m_cbr != -1 )
							c_sgn++;
					}
				}

				if( (chk_bit & 0x04) ) {
					if( drec->m_cbmn == m_BaseBmn )
						cred_bit |= 0x04;
				}
				else {
					// 部門が入っていたら、基本とは異なる
					if( (m_dwQuery & ACQ_BUMON) ) {
						if( drec->m_cbmn != -1 )
							c_sgn++;
					}
				}

				if( (chk_bit & 0x08) ) {
					if( drec->m_ckno == m_BaseKoji ) {
						cred_bit |= 0x08;
					}
				}
				else if( M_KOJI ) {
					// 工事が入っていたら、基本とは異なる
					if( (m_dwQuery & ACQ_KOJI) ) {
						if( ! drec->m_ckno.IsEmpty() )
							c_sgn++;
					}
				}

				if( ! c_sgn ) {
					if( cred_bit == chk_bit )
						dcsw = 1;
				}
			}
		}

		//借・貸 判定の最終チェック
		if( dcsw == -1 ) {
			if( d_sgn ) {
				if( debt_bit == chk_bit )
					dcsw = 0;
			}
		}
		if( dcsw == -1 ) {
			if( c_sgn ) {
				if( cred_bit == chk_bit )
					dcsw = 1;
			}
		}
	}

	return dcsw;
}

// 借／貸 変更
//
//	int chg		1:入金, 2:出金
//
int CDBAbookView::ChangeDebtCred( int chg/*=0*/)
{
	ACBOOK_LINE* al = getLINE_DATA();
	int ret, nTmp, dcsw;

	// 修正不可
	if( ! modefy_ok( acINP_mode, &al->drec ) )
		return -1;

	// 借方／貸方の科目・枝番・部門・工事が同じ場合は、なにもしない
	if( IsSameKamoku() )
		return -1;

	CString tmp;
	tmp = al->drec.m_dbt;
	// 科目コード
	al->drec.m_dbt = al->drec.m_cre;
	al->drec.m_cre = tmp;
	// 枝番
	nTmp = al->drec.m_dbr;
	al->drec.m_dbr = al->drec.m_cbr;
	al->drec.m_cbr = nTmp;
	// 部門
	nTmp = al->drec.m_dbmn;
	al->drec.m_dbmn = al->drec.m_cbmn;
	al->drec.m_cbmn = nTmp;
	// 工事番号
	tmp = al->drec.m_dkno;
	al->drec.m_dkno = al->drec.m_ckno;
	al->drec.m_ckno = tmp;

	m_CREC.m_dbt = al->drec.m_dbt;
	m_CREC.m_dbr = al->drec.m_dbr;
	m_CREC.m_dbmn = al->drec.m_dbmn;
	m_CREC.m_dkno = al->drec.m_dkno;

	m_CREC.m_cre = al->drec.m_cre;
	m_CREC.m_cbr = al->drec.m_cbr;
	m_CREC.m_cbmn = al->drec.m_cbmn;
	m_CREC.m_ckno = al->drec.m_ckno;

	// 差引残高
	if( chg == 0 ) {
		dcsw = GetRecordDCsgn( &al->drec );
	}
	else {
		if( chg == 1 )	dcsw = 0;
		else			dcsw = 1;
	}

	BOOL bSoto = sotozei_data( &al->drec );

	if( dcsw == 0 || dcsw == -1 ) {	
		l_sub( al->szan, al->szan, al->drec.m_val );	// 入金
		if( bSoto )	l_sub( al->szan, al->szan, al->drec.m_zei );

		l_add( al->szan, al->szan, al->drec.m_val );
		if( bSoto )	l_add( al->szan, al->szan, al->drec.m_zei );
	}
	else {
		l_add( al->szan, al->szan, al->drec.m_val );	// 出金	
		if( bSoto )	l_add( al->szan, al->szan, al->drec.m_zei );

		l_sub( al->szan, al->szan, al->drec.m_val );
		if( bSoto )	l_sub( al->szan, al->szan, al->drec.m_zei );
	}

	// リスト再表示
	int pn[] = { AC_NKIN_PN, AC_SKIN_PN };

	if( dcsw == 0 || dcsw == -1 ) {
		if( acINP_mode == _APPEND ) {
			if( al->lcnd[AC_NKIN_PN].INP_sg || al->lcnd[AC_SKIN_PN].INP_sg ) {
				al->lcnd[AC_NKIN_PN].INP_sg = TRUE;
				al->lcnd[AC_SKIN_PN].INP_sg = FALSE;
			}
		}
		else {
			al->lcnd[AC_NKIN_PN].INP_sg = TRUE;
			al->lcnd[AC_SKIN_PN].INP_sg = FALSE;
		}

		ret = 0;	// 入金
	}
	else {
		if( acINP_mode == _APPEND ) {
			if( al->lcnd[AC_NKIN_PN].INP_sg || al->lcnd[AC_SKIN_PN].INP_sg ) {
				al->lcnd[AC_NKIN_PN].INP_sg = FALSE;
				al->lcnd[AC_SKIN_PN].INP_sg = TRUE;	
			}
		}
		else {
			al->lcnd[AC_NKIN_PN].INP_sg = FALSE;
			al->lcnd[AC_SKIN_PN].INP_sg = TRUE;	
		}

		ret = 1;	// 出金
	}

	TerminationDataSet( pn, 2 );

	return ret;
}


// 先頭仕訳 に
void CDBAbookView::OnButtonF8()
{
	if (CalcWindowDelete()) {
		PostMessage(WM_COMMAND, ID_BUTTON_F8);
		return;
	}

	int pn = get_nowpn();
	int st, old_ln;
	old_ln = -1;

	if( pn < AC_SELBMON_PN ) {
		if( !PageMoveDataUpdate(get_nowln()) ) {
			return;
		}
		// _dline_chg で DataCorrect()しないように

		old_ln = get_nowln();
		set_nowln(-1);
	}

	//入力順
	if( (m_dwOpt & 0x0f) != 0 ) {
		st = ICSMessageBox( "先頭の仕訳を表示します。", MB_OKCANCEL );

		if( st == IDOK ) {
			if( TopDataSet() >= 0 ) {
				ListDataSet();
				int ln = 1;
				if (m_ALine[0].lattr == 1)	ln++;
				_set_focus( ln, AC_DATE_PN );
			}
		}
		else {
			_set_focus( old_ln, get_nowpn() );
		}
	}
	else {
		//日付順 月指定
		CMnthDlg	dlg;
		dlg.m_bTan = TRUE;
		dlg.m_st1 = "先頭に表示する仕訳の月を選択してください。";

		if( dlg.DoModal() == IDOK ) {
			if( TopDataSet( dlg.m_TanOfst ) >= 0 ) {
				ListDataSet();
				int ln = 1;
				if (m_ALine[0].lattr == 1)	ln++;
				_set_focus(ln, AC_DATE_PN);
			}
		}
		else {
			_set_focus( old_ln, get_nowpn() );
		}
	}
}


// 登録処理
void CDBAbookView::OnButtonF7()
{
	if( ! PROGRAM_OK )	return;

#ifndef TKY_VER2
	TKY_REGPAREX	par;
#else
	TKY_REGPAR par;
#endif
	int ln, pos;
	char	tky[128] = {0};
	CString code1, code2;

	ln = get_nowln();

	int pn = get_nowpn();

	if(pn != AC_TKY_PN && pn != AC_SYZ_PN ) {
		Rdadd_job();
		return;
	}
	else if( pn != AC_TKY_PN )
		return;

	// 摘要辞書
	CICSDBEDT* pDedt;
	pDedt = (CICSDBEDT*)GetDlgItem( IDC_ICSDBEDTCTRL1 );
	pos = pDedt->GetCaretPosition();

	if( getLINE_DATA()->drec.m_imgsq <= 0 )
	{
		pos = pDedt->GetCaretPosition();

		VARIANT var;
		var.pbVal = NULL;

		pDedt->GetData( &var, ICSDBEDT_TYPE_STRING, 0 );

		if( var.pbVal != NULL ) {
			strcpy_s( tky, sizeof tky, (char*)var.pbVal );
		}

		if( tky[0] == '\0' ) {
			pos = -1;
		}
	}
	else
	{
		pos = -1;
		tky[0] = '\0';
	}

	// 科目対応摘要
	if( acINP_mode == _APPEND ) {
		code1 = getLINE_DATA()->drec.m_dbt;
		code2 = getLINE_DATA()->drec.m_cre;
	}
	else {
		if( getLINE_DATA()->lcnd[AC_NKIN_PN].INP_sg ) {
			code1 = m_BaseCode;
			code2 = getLINE_DATA()->drec.m_cre;
		}
		else {
			code1 = getLINE_DATA()->drec.m_dbt;
			code2 = m_BaseCode;
		}
	}

	// 摘要登録処理へ (DBInpSub.DLL)
	::ZeroMemory( &par, sizeof par );

#ifndef TKY_VER2
	strncpy_s( par.tex_debt, sizeof par.tex_debt, code1, KMKCD_LN );
	strncpy_s( par.tex_cred, sizeof par.tex_cred, code2, KMKCD_LN );
	par.tex_pos = pos;
	memcpy( par.tex_tky, tky, sizeof par.tex_tky );

	memcpy(par.tex_invno, getLINE_DATA()->drec.m_invno, sizeof par.tex_invno);
	par.tex_menchk = (getLINE_DATA()->drec.m_dsign[6] & 0x20) ? 1 : 0;;

	// 選択部分
	if( pos >= 0 ) {
		par.tex_sel	= pDedt->GetSelect();
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
	int st = TkyEdaTorokDialog( &par, pDBzm, pDBsy, this, &bTkquery );
//	int st = TekiyoTorokDialogEX( &par, pDBzm, pDBsy, this, &bTkquery );
#else
	int st = TekiyoTorokDialog( &par, pDBzm, pDBsy, this, &bTkquery );
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

	//枝番セット
	if( par.tex_brnsgn & 0x03 ) {
		int dc_sw, dsp;
		dc_sw = dsp = 0;

		int dcsgn = GetRecordDCsgn( &m_CREC );

		if( dcsgn == 0 || dcsgn == -1 ) {
			dc_sw |= 0x02;	// 相手科目が貸方

			if( par.tex_brnsgn & 0x02 ) {
				m_CREC.m_cbr = par.tex_brncre;
				dsp++;
			}
		}
		else {
			dc_sw |= 0x01;	// 相手科目が借方

			if( par.tex_brnsgn & 0x01 ) {
				m_CREC.m_dbr = par.tex_brndeb;
				dsp++;
			}
		}

		if( dsp ) {
			tky_select_dsp( &m_CREC, dc_sw, NULL, FALSE, TRUE );
		}
	}

	((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(0);
	PROGRAM_OK = TRUE;

	if( pos != -1 ) {
		pDedt->SetCaretPosition( pos );
	}

}


//--------------------------------------------------------
// 仕訳挿入
//	返送値	-1 = エラー,
//			 1 = 仕訳挿入モードに
//			 0 = なにもしない
//	エラー時 例外をthrow
//--------------------------------------------------------
int CDBAbookView::InsertSiwake( int ln )
{
	// 現在 仕訳挿入モード
	if( ! PROGRAM_OK )	return 0;

	if( IsMasterType(MST_INSERT) ) {
		if( ! isInsertMaster() )
			return 0;
	}

	if( ln < 1 )	return 0;

#ifdef NEWINS_CLOSE
	//入力順は除く
	if( (m_dwOpt & 0x0f) != 0 )
		return 0;
#endif
	//入力順は除く
	if( ! IsMasterType(MST_INSERT) ) {
		if( (m_dwOpt & 0x0f) != 0 )
			return 0;
	}

	if( m_InsLine > 0 )
		return 0;

	if( acINP_mode == _APPEND )
		return 0;

	// 確定マスター は挿入できない
	if( bCONFIRM_MASTER )
		return 0;

	int pos = m_ALine[ln-1].abs_pos;
	if( pos <= 0 )
		return 0;

	CDBINPDataRec*	prec;
	int		offset, ddate, ksign;

	if( pos >= 1 ) {
		prec = &m_ALine[ln-1].drec;

		int chk = 0;

		if( pos > 1 ) {
			pos--;

			pDBzm->data->SetAbsolutePosition( pos );
			if( pDBzm->data->st == -1 ) {
				ermset( 0, "データポジションを取得できません！" );
				return -1;
			}

#ifndef _SPEED_CLOSE
			pDBzm->DBDATA_SetPosition( pDBzm->data->seq );
#else
			pDBzm->dbdata->SetAbsolutePosition( pDBzm->data->seq );
#endif
			if( pDBzm->dbdata->st == -1 ) {
				ermset( 0, "データポジションを取得できません！" );
				return -1;
			}

				if( ! IsMasterType(MST_INSERT) ) {
					if( prec->m_ddate != pDBzm->dbdata->ddate ||
						(prec->m_ddate == pDBzm->dbdata->ddate && prec->m_mofs != pDBzm->dbdata->mofs) ) {
						chk = 1;

						ddate	= pDBzm->dbdata->ddate;
						offset	= pDBzm->dbdata->mofs;
						ksign	= pDBzm->dbdata->ksign;
					}
				}
				else {
					chk = 1;

					ddate	= pDBzm->dbdata->ddate;
					offset	= pDBzm->dbdata->mofs;
					ksign	= pDBzm->dbdata->ksign;
				}

#ifdef NEWINS_CLOSE
			if( prec->m_ddate != pDBzm->dbdata->ddate ||
				(prec->m_ddate == pDBzm->dbdata->ddate && prec->m_mofs != pDBzm->dbdata->mofs) ) {
				chk = 1;

				ddate	= pDBzm->dbdata->ddate;
				offset	= pDBzm->dbdata->mofs;
				ksign	= pDBzm->dbdata->ksign;
			}
#endif
		}
		else {
			// 先頭仕訳は、入力開始日と比較
			CVolDateDB vd;
			if( vd.db_vd_offset( 0, Voln1->ip_ymd, &offset, pDBzm) == 0) {
				if( ! IsMasterType(MST_INSERT) ) {
					if( prec->m_ddate > Voln1->ip_ymd && prec->m_mofs >= offset ) {
						chk = 1;
						ddate	= Voln1->ip_ymd;
						ksign	= 0;
					}
				}
				else {
					chk = 1;
					ddate	= Voln1->ip_ymd;
					ksign	= 0;
				}

#ifdef NEWINS_CLOSE
				if( prec->m_ddate > Voln1->ip_ymd && prec->m_mofs >= offset ) {
					chk = 1;

					ddate	= Voln1->ip_ymd;
					ksign	= 0;
				}
#endif
			}
		}

		// 挿入モードへ
		if( chk ) {
			// 月指定バーの範囲内か？
			int sofs, eofs, sy, ey;
			pDBzm->get_datelimit( sofs, eofs, sy, ey );

			if( offset >= sofs && prec->m_mofs <= eofs ) {
				CDBINPDataRec id;
				BYTE	bcddate[10] = {0};
				id.m_ddate	= ddate;
				id.m_mofs	= offset;
				id.m_ksign	= ksign;
				// 挿入終了のために日付を保存
				pDBzm->GetInpDate( m_svInpDate );

				pDBzm->DateConv( bcddate, &id, CONV2_1 );
				pDBzm->SetInpDate( bcddate );

				m_InsLine = ln;
			}
			else {
				m_InsLine = 0;
			}
		}
	}
	else {
		// 挿入モードへ
		m_InsLine = ln;
	}

	// 表示を更新
	if( m_InsLine != 0 ) {
		PROGRAM_OK = FALSE;
		m_List.SetFocus();
		int n = (ln-1);
		// データを一つずつずらす
		for( int i = ACREC_MAX-1; i >= n; i-- ) {
			if( i == 0 )	break;

			m_ALine[i] = m_ALine[i-1];
			// 現在の追加行は空行にする
			if( m_ALine[i].abs_pos == 0 ) {
				acline_init( &m_ALine[i], 1 );
			}
		}

		acline_init( &m_ALine[n], 1 );
		m_ALine[n].abs_pos = 0;
		m_ALine[n].drec.m_seq = pDBzm->m_lastdata.m_seq;

		pDBzm->GetInpDate( &m_ALine[n].drec );
		pDBzm->GetInpDate( m_ALine[n].bcddate );

		ListDataSet();
		PROGRAM_OK = TRUE;
		set_nowln(-1);	// _dline_chg で 現在レコードをセットするため
		_set_focus( ln, AC_DATE_PN );
	}

	return ( m_InsLine > 0 ) ? 1 : 0;
}


//--------------------------------------------------------
// 仕訳挿入 から 最終行に入力ラインがあるモードへ戻す
//	返送値	-1 = エラー,
//			 1 = 仕訳挿入モードに
//			 0 = なにもしない
//
//--------------------------------------------------------
int CDBAbookView::BackNormalAppendMode( int ln )
{
	// 現在 仕訳挿入モード
	if( ! PROGRAM_OK )	return 0;

	if( m_InsLine == 0 )
		return 0;

	if( bCONFIRM_MASTER )
		return 0;

ASSERT( m_InsLine == ln );

	int i, lcnt, pos, apnd_idx;
	lcnt = GetACListCount(); /*m_List.GetDispDataCount();*/

	// 次のページの最初の仕訳を読み込む
	if( ln == lcnt ) {
		pos = m_ALine[ln-2].abs_pos;
		pos++;
	}
	else {
		// 現ページの 最終ラインデータ
		pos = m_ALine[lcnt-1].abs_pos;
		pos++;

		if( pDBzm->data->GetRecordCount() < pos )
			pos = 0;
	}

	if( pos > 1 ) {
		pDBzm->data->SetAbsolutePosition( pos );
		if( pDBzm->data->st == -1 ) {
			ermset( 0, "データポジションを取得できません！" );
			return -1;
		}
	}

	apnd_idx = -1;
	for( i = (ln-1); i < lcnt; i++ ) {

		if( i < (lcnt-1) ) {
			m_ALine[i] = m_ALine[i+1];
		}
		else {
			acline_init( &m_ALine[i], 1 );

			if( pos > 0 ) {
				acline_set( m_ALine, i, pos );

				// 消えていた最終行の差引残高を 計算
				char work[6];
				memcpy( work, m_ALine[i-1].szan, sizeof work );

				int dcsw = GetRecordDCsgn( &m_ALine[i].drec );

#ifndef SZAN_CLOSE	// 11.05 /12
				if( dcsw != -1 && IsSzanCalq( &m_ALine[i].drec ) ) {
#else
				if( dcsw != -1 ) {
#endif
					if( dcsw == 0 ) {
						l_add( work, work, m_ALine[i].drec.m_val );
						if( sotozei_data( &m_ALine[i].drec ) )
							l_add( work, work, m_ALine[i].drec.m_zei );
					}
					else {
						l_sub( work, work, m_ALine[i].drec.m_val );
						if( sotozei_data( &m_ALine[i].drec ) )
							l_sub( work, work, m_ALine[i].drec.m_zei );
					}
				}
				memcpy( m_ALine[i].szan, work, sizeof work );
			}
		}
		// 追加データセット用インデックス
		if( m_ALine[i].abs_pos < 0 ) {
			if( apnd_idx == -1 )	apnd_idx = i;
		}
	}

	m_InsLine = 0;
	pDBzm->SetInpDate( m_svInpDate );

	if( apnd_idx != -1 ) {
		// 新規行をセット
		acline_init( &m_ALine[apnd_idx], 1 );
		m_ALine[apnd_idx].abs_pos = 0;
		m_ALine[apnd_idx].drec.m_seq = pDBzm->m_lastdata.m_seq;

		pDBzm->GetInpDate( &m_ALine[apnd_idx].drec );
		pDBzm->GetInpDate( m_ALine[apnd_idx].bcddate );
/*
		if( apnd_idx > 0 ) {
			m_ALine[apnd_idx].drec.m_ddate	= m_ALine[apnd_idx-1].drec.m_ddate;
			m_ALine[apnd_idx].drec.m_mofs	= m_ALine[apnd_idx-1].drec.m_mofs;
			m_ALine[apnd_idx].drec.m_ksign	= m_ALine[apnd_idx-1].drec.m_ksign;
		}
		else {
			set_inpdate( &m_ALine[apnd_idx].drec );
		}
*/
	}

	ListDataSet();
	set_nowln(-1);	// _dline_chg で 現在レコードをセットするため
	_set_focus( ln, AC_DATE_PN );

	return 0;
}



// F12 自動仕訳
void CDBAbookView::OnButtonF12()
{
	AutoSiwake( get_nowln(), m_bAutoSwk ? TRUE : FALSE );
}

//--------------------------------------------------------
// 自動仕訳
//	int ln ...... 自動仕訳行
//	BOOL bAuto .. 自動仕訳モードへ(TRUE)
//
//--------------------------------------------------------
void CDBAbookView::AutoSiwake( int ln, BOOL bAuto )
{
	if( ! PROGRAM_OK )
		return;

	if( acINP_mode != _APPEND )
		return;

	// 行の背景色を変更
	DTLIST_DATA data;
/*
	m_List.GetDtListData( ln-1, &data );
	for( int i = 0; i < sizeof(data.dd_ctbl) / sizeof (data.dd_ctbl[0]); i++ ) {
		data.dd_ctbl[i].back = RGB_WINE_GREEN;
	}
	m_List.SetDtListData( ln-1, &data, TRUE );
*/

	if( ! m_bAutoSwk ) {
		m_List.SetCurBkColor( RGB_WINE_GREEN );
		m_bAutoSwk = TRUE;
		// 摘要欄にフォーカスセット
		_set_focus( ln, AC_TKY_PN );
	}
	else {
		m_List.SetCurBkColor( RGB_WINE_BLUE );
		m_bAutoSwk = FALSE;

		if( ! _getLINE_DATA(ln)->lcnd[AC_KMK_PN].INP_sg ) {
			_set_focus( ln, AC_KMK_PN );
		}
		
		m_List.RedrawDataListCtrl();
	}
}


// 固定選択画面クリック
void CDBAbookView::SelectedIcsselctrl1(short position, long Number)
{
	SelectedIcsselctrl( position, Number, 0 );
}

// 選択画面クリック
void CDBAbookView::SelectedIcsselctrl2(short position, long Number)
{
	SelectedIcsselctrl( position, Number, 1 );
}


/*=======================
	FIX 8科目選択
	科目・摘要32項目選択
		本体
========================*/
void CDBAbookView::SelectedIcsselctrl(short position, long Number, int place ) 
{
	int pn, tmp;
	CDBipTKREC tkrec;
	CDBINPDataRec tmpdt;	// 部門コード取得用
	struct _ItemData dmy_data = {0};
	int dcsw;
	struct	_KamokuData check={0}, ans={0};
	char temp[10];
	char str[128];
	DBKNREC* pKn;

	pn = get_nowpn();

	// 基本項目エリアでの選択
	if( base_selected( pn, position, Number, place ) ) {
		return;
	}

	if( bCONFIRM_MASTER )	return;

	int base_sw = GetRecordDCsgn( &getLINE_DATA()->drec );

	if( acINP_mode == _APPEND ) {
		if( ! getLINE_DATA()->lcnd[AC_NKIN_PN].INP_sg && 
			! getLINE_DATA()->lcnd[AC_SKIN_PN].INP_sg ) {
			dcsw = -1;
		}
		else {
			// 基本科目が借方／貸方どちらか？
			if( base_sw == 0 || base_sw == -1 ) {
				dcsw = 1;
			}
			else {
				dcsw = 0;
			}
		}
	}
	else {
		if( ! modefy_ok(acINP_mode, &getLINE_DATA()->drec) ) {
			return;
		}
		if( base_sw == 0 || base_sw == -1 ) {
			dcsw = 1;
		}
		else {
			dcsw = 0;
		}
	}

	if( ! BRNTKY_sel && pn == AC_KMK_PN )
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

	//	check.IP_CHKCdNum = (int)Number;
	//	check.IP_CHKCdNumSg = 1;
		// 科目ｺｰﾄﾞ or 番号 02.20/02
		if( bKamokuCode ) {
			sprintf_s( check.kd_code, sizeof check.kd_code, "%d", position + (place ? 9 : 1) );
			check.kd_sgn = 1;
		}
		else {
			sprintf_s( check.kd_code, sizeof check.kd_code, "%d", Number );	// 番号
			check.kd_sgn = 1;
		}
		check.kd_eda = -1;

		if( m_Seldata.InputItemCheck( ans, check, dcsw, TRUE ) /*&& CheckKamokuInput(&ans) == 0*/ )
		{
			DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &ans, pDBzm );
			//データセット
			getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg = TRUE;

			//データセットのための ダミーターミネーション
			TerminationKamoku( get_nowln(), 0, 0, 0 );
		}
		return;
	}
	else if( BRNTKY_sel && pn == AC_KMK_PN )
	{
		if( Number > 8 )
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

			SelResult srt;
			sprintf_s( str, sizeof str, "%d", Number );

			srt.sel_pn = SL_BRNTKY_PN;
			if( m_Seldata.GetSelResult( &srt, str ) ) {

				strcpy_s( ans.kd_code, sizeof ans.kd_code, srt.km_code );
				ans.kd_eda = srt.brn_no;
				if( (pKn = pDBzm->DB_PjisToKnrec( ans.kd_code ) ) ) {
					KamokuString( pKn, ans.kd_name, sizeof ans.kd_name );
				//	sprintf_s( ans.kd_name, sizeof ans.kd_name, "%.14s", pKn->knnam );
				}
				//データセット
				DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &ans, pDBzm );
				getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg = TRUE;

				//次に移動
				//m_CREC などには、Termination 0 で データセット
			//	set_termfocus( get_nowln(), AC_KMK_PN, 1 );

			}
		}
	}
	else if( pn == AC_TKY_PN && place )
	{
		if( m_pSubDtInp->IsDisplay() )
			return;

		//摘要選択
		sprintf_s( temp, sizeof temp, "%d", Number );
		if( tky_select( &tkrec, (char *)temp ) != -1 )
		{
			//摘要toデータ
			int dc_sw = tky_select_to_drec( &m_CREC, &tkrec );
			//表示
			tky_select_dsp( &m_CREC, dc_sw, &tkrec,FALSE );
		}
		return;
	}
	else if( pn == AC_BMON_PN || pn == AC_KOJI_PN ) {
		// 部門・工事選択処理
		int chk_dcsw;
		CString strNam;
		chk_dcsw = dcsw;
		if( base_sw == -1 )
			chk_dcsw = -1;
		else
			chk_dcsw = base_sw ? 0 : 1;	//選択欄クリックで 借・貸 同じ部門が入っていた。[11.07 /12]

		switch( pn ) {
		case AC_BMON_PN:
			if( bmncode_todata2( &m_CREC, Number, chk_dcsw, &strNam ) != FALSE ) {
				getLINE_DATA()->drec.m_dbmn = m_CREC.m_dbmn;
				getLINE_DATA()->drec.m_cbmn = m_CREC.m_cbmn;

				bmon_select_dsp( chk_dcsw, &m_CREC, TRUE );
				bmnkoji_to_tekiyo( &m_CREC, &strNam );
				bmneda_update(&m_CREC);
			}
			break;

		case AC_KOJI_PN:
			if( kojicode_todata( &m_CREC, &strNam, Number, chk_dcsw ) != FALSE ) {
				getLINE_DATA()->drec.m_dkno = m_CREC.m_dkno;
				getLINE_DATA()->drec.m_ckno = m_CREC.m_ckno;

				koji_select_dsp( chk_dcsw, &m_CREC, TRUE );

				bmnkoji_to_tekiyo( &m_CREC, &strNam, 1 );
			}
			break;
		}
	}
	else if( pn == AC_SYZ_PN ) {
		sprintf_s( str, sizeof str, "%d", Number );
		tmp = atoi( str );

		m_pSyzInp->SetSkbnSelNo( tmp );
		return;
	}
	else if( pn == AC_HJIYU_PN ) {
		_hjiyu_select( Number );
		return;
	}

	set_focus( pn );

}


//--------------------------------------------------------------
//	基本項目の選択欄クリックによる選択
//
//--------------------------------------------------------------
BOOL CDBAbookView::base_selected( int pn, short position, long Number, int place )
{
	struct	_KamokuData check={0}, ans={0};
	BOOL bRet = TRUE;
	SelResult	srt;
	char	tmp[128] = {0};
	VARIANT var;
	DBKNREC* pKn;

	switch( pn ) {
	case AC_SELBMON_PN:
		srt.sel_pn = SL_BUMON_PN;
		sprintf_s( tmp, sizeof tmp, "%d", Number );

		if( m_Seldata.GetSelResult( &srt, tmp ) && CheckBaseBumonInput( srt.bmn ) == 0 ) {
			m_BaseBmn = srt.bmn;
			//コントロールへ再セット
			pDBzm->BumonCodeToStr( tmp, sizeof tmp, m_BaseBmn );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, IDC_ICSDBEDTCTRL2, &var, ICSDBEDT_TYPE_STRING, 0 );
			check_basebrn();

			m_BmnSK.bmn = srt.bmn;
			m_BmnSK.etc = srt.selname;
			m_BmnSK.etc_col = RGB_BLUE;

			SetBmnEtcDisp( TRUE );
		}
		break;

	case AC_SELKOJI_PN:
		srt.sel_pn = SL_KOJI_PN;
		sprintf_s( tmp, sizeof tmp, "%d", Number );

		if( m_Seldata.GetSelResult( &srt, tmp ) && CheckBaseKojiInput( srt.kjcode ) == 0  ) {
			srt.kjcode.TrimRight();
			m_BaseKoji = srt.kjcode;
			//コントロールへ再セット
			pDBzm->KojiCodeToStr( tmp, sizeof tmp, m_BaseKoji );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, IDC_ICSDBEDTCTRL3, &var, ICSDBEDT_TYPE_STRING, 0 );

			m_KojiSK.bmn = -1;
			m_KojiSK.kno = srt.kjcode;
			m_KojiSK.etc = srt.selname;
			m_KojiSK.etc_col = RGB_BLUE;

			SetKojiEtcDisp( TRUE );
		}
		break;

	case AC_SELKAMOK_PN:
		// 科目ｺｰﾄﾞ or 番号
		if( bKamokuCode ) {
			sprintf_s( check.kd_code, sizeof check.kd_code, "%d", position + (place ? 9 : 1) );
			check.kd_sgn = 1;
		}
		else {
			sprintf_s( check.kd_code, sizeof check.kd_code, "%d", Number );	// 番号
			check.kd_sgn = 1;
		}
		check.kd_eda = -1;

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

		if( m_Seldata.InputItemCheck( ans, check, 0, TRUE ) )
			/*&& CheckBaseKamokuInput( &ans ) == 0 ) */
		{
			m_BaseCode = ans.kd_code;
			DBkamokudata_set( this, IDC_ICSDBEDTCTRL4, &ans, pDBzm );
			DBKNREC* pKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
			if( pKn ) {
				m_BaseAtt = pKn->knatt;
			}
			check_basebrn();
			BaseBmnCtrl();
			BaseKojiCtrl();
		}
		else {
			::ZeroMemory( &ans, sizeof ans );
			memcpy( ans.kd_code, m_BaseCode, KMKCD_LN );
			ans.kd_eda = -1;

			pKn = pDBzm->DB_PjisToKnrec( ans.kd_code );
			if( pKn ) {
				KamokuString( pKn, ans.kd_name, sizeof ans.kd_name );
				m_BaseAtt = pKn->knatt;
			}
			DBkamokudata_set( this, IDC_ICSDBEDTCTRL4, &ans, pDBzm );
		}
		break;

	case AC_SELBRN_PN:
		if( ! BRNTKY_sel )
			break;

		srt.sel_pn = SL_BRNTKY_PN;
		sprintf_s( tmp, sizeof tmp, "%d", Number );

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

		if( m_Seldata.GetSelResult( &srt, tmp ) ) {
			m_BaseEda = srt.brn_no;
			//コントロールへ再セット
			pDBzm->EdabanToStr( tmp, sizeof tmp, m_BaseEda );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
			// 枝番名称表示
			struct _SET_KMKETC sk;
			sk.bmn = m_BaseBmn;
			sk.brn = m_BaseEda;
			sk.code = m_BaseCode;

			if( _set_kmketc( &sk, FALSE ) ) {
				GetDlgItem(IDC_STATIC5)->SetWindowText( sk.etc );
			}
			else {
				GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
			}
		}
		break;

	case AC_SELDISP_PN:
		break;
	default:
		bRet = FALSE;
		break;
	}

	return bRet;
}

//
//	科目・部門変わりの枝番チェック
//		return FALSE	該当枝番がない
//
BOOL CDBAbookView::check_basebrn()
{
	CDBINPDataRec tmp;

	if( m_BaseEda == -1 )
		return FALSE;

	tmp.m_dbmn	= m_BaseBmn;
	tmp.m_dbt	= m_BaseCode;
	tmp.m_dbr	= m_BaseEda;

	if( pDBzm->DB_EdabanMasterSearch( &tmp, 0 ) != 0 ) {
		m_BaseEda = -1;

		VARIANT var;
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
		GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
		return FALSE;
	}

	// 枝番名称表示
	struct _SET_KMKETC sk;
	sk.bmn = m_BaseBmn;
	sk.code = m_BaseCode;
	sk.brn = m_BaseEda;

	if( _set_kmketc( &sk, FALSE ) ) {
		GetDlgItem(IDC_STATIC5)->SetWindowText( sk.etc );
	}
	else {
		GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
	}

	return TRUE;
}


// タブクリックによる切替時の チェック
//
int CDBAbookView::TabLbuttonDownJob()
{
	c_set( IDC_ICSDBEDTCTRL4 );

	UpdateModfyData(1);

	GetAcbookItemWidth();

	Destroy_ModeLessDlg();

	set_nowln(-1);
//	acline_init( m_ALine, ACREC_MAX );

	return 0;
}

//---------------------------------------
//	修正仕訳の更新
//
//---------------------------------------
int CDBAbookView::UpdateModfyData( int nomsg/*=0*/ )
{
	int ret = 0;
	// 現在行の書き込み
	int nowln = get_nowln();

//MyTrace( "CDBAbookView::UpdateModfyData TOP(%d), nowln = %d\n", nomsg, nowln );

	if( acINP_mode == _MODEFY ) {
		if( nowln >= 1 ) {
			if( m_ALine[nowln-1].abs_pos > 0 ) {
				if( isdiff_data( &m_CREC, &m_SCAN_rec ) ) {

					if( CheckModifyBmonData( &m_CREC, nowln, nomsg ) != 0 ) {
						_set_focus( nowln, AC_BMON_PN );
						return -1;
					}
					// 05.01 /12
					if( CheckModifyEdabanData( &m_CREC, nowln, nomsg ) != 0 ) {
						_set_focus( nowln, AC_KMK_PN );
						return -1;
					}

					ret = pDBzm->DB_DataCorrect( &m_CREC );

					if( ret != -2 ) {
						MODEFY_BIT.onbitn( m_CREC.m_seq - 1);
						if( isSyzdiff_data( &m_CREC, &m_SCAN_rec ) ) {
							SYZMDFY_BIT.onbitn( m_CREC.m_seq - 1);
						}
					}
					else {
						pDBzm->GetCDBData( &m_CREC );
						UpdateACZanRec(m_ALine[nowln-1].abs_pos, &m_CREC);
					}

					m_SCAN_rec = m_CREC;
				}
			}
		}
	}

//MyTrace( "CDBAbookView::UpdateModfyData END ret (%d)\n", ret );

	return ret;
}


BOOL CDBAbookView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: pretranslate ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	set_nonICSpn( pMsg );

	if( PROGRAM_OK ) {

//		if( pMsg->message == WM_ACTIVATE ) {
//MyTrace( "CDBAbookView::PreTranslateMessage message = %08x wp = %d, lp = %d\n", pMsg->message, pMsg->wParam, pMsg->lParam );
//		}

//		if( IsWindow( m_List.m_hWnd ) ) {
//			if( pMsg->hwnd == m_List.m_hWnd ) {
//MyTrace( "CDBAbookView::PreTranslateMessage m_List.m_hWnd message = %d\n", pMsg->message );
//			}
//		}
		if (pMsg->message == WM_LBUTTONDOWN) {
			// 出納帳から、業務メニューで科目設定等を起動し、そのプログラムを最小化。タスクバーで元に戻すを
			// 繰り返すと、DBEditが白くなってカーソルがセットされていな状態になる。
			// そのとき、別の行をクリックしたときに、カーソルをセットする処理
			int pn = get_nowpn();
			if (m_Input.IsWindowVisible()) {
				CWnd* pwnd = GetFocus();
				if ( pwnd && m_List.m_hWnd == pwnd->m_hWnd) {
					if (pn < AC_SELBMON_PN) {
						int ln = get_nowln();
						if (ln != -1) {
							_set_focus(ln, pn);
							return TRUE;
						}
					}
				}
			}
			if( pn == AC_SYZ_PN ) {
				m_pSyzInp->CancelInvNoIME();
			}
		}

		if( pMsg->message == WM_KEYDOWN ) {

//MyTrace( "CDBAbookView::PreTranslateMessage WM_KEYDOWN(hWnd = %08x) message = %d\n", pMsg->hwnd, pMsg->message );

			if( GetDlgItem( IDC_BUTTON_DISP )->m_hWnd == pMsg->hwnd ){
				if( ! OnTermBaseDispBtn(pMsg->wParam) )
					return TRUE;
			}

			if( m_KamokInp.m_hWnd == pMsg->hwnd ) {
				if( pMsg->wParam == VK_BACK )
					return TRUE;
			}

			if( m_Input.m_hWnd == pMsg->hwnd ) {
				if( BmnIMEkeydownJob( pMsg->wParam ) )
					return TRUE;
			}
			int pn = get_nowpn();
			if( pn == AC_KMK_PN ) {
				if( KanaCheckVK_BACK(pMsg) )
					return 1;
			}
			// 出納帳から、業務メニューにウィンドウを切り換えて、チェックリスト等を起動して
			// タスクバーから仕訳入力に戻ると、OnActivateViweに来ないで、m_List に フォーカスがある。
			// その時、キーを押したら、アクティブになるようにする。
			if( m_List.m_hWnd == pMsg->hwnd ) {
				if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_TAB || 
					pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN ) {
					int pn = get_nowpn();
					if( pn < AC_SELBMON_PN ) {
						int ln = get_nowln();
						if( ln != -1 ) {
							_set_focus( ln, pn );
							return TRUE;
						}
					}
					else {
						_set_focus( -1, pn );
						return TRUE;
					}
				}
			}
		}
		if( pMsg->message == WM_KEYUP ) {
			int pn = get_nowpn();
			if( pn == AC_KMK_PN || pn == AC_SELKAMOK_PN || pn == AC_SELBRN_PN ) {
				TRACE("Kana ● pMsg->message = %08x, WP = %08x\n", pMsg->message, pMsg->wParam);
				if( pn == AC_SELBRN_PN && !BRNTKY_sel ) {
				}
				else {
					FukuKanaSearch(pMsg, pn == AC_SELBRN_PN ? 1 : 0);
				}
			}
		}

		if( pMsg->message == WM_CHAR ) {
			if( get_nowpn() == AC_ZEI_PN ) {
				if( !is_sotomensiwake(&m_CREC) ) {
					if( !is_syohizeisiwake(&m_CREC) || is_yusyutsusiwake(&m_CREC) )
						return TRUE;
					else if( pAUTOSEL->ZEIMDFY_OPT != OPT_ZEIMDFY )
						return TRUE;
				}
			}
		}

		if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F10) {
			if( get_nowpn() == AC_ZEI_PN ) {
				if( ChangeSotoMenZei() ) {
				}
				return TRUE;
			}
			else {
				if( ChangeZeiritsu() ) {
					return TRUE;
				}
			}
		}
	}

	return CDBbaseView::PreTranslateMessage(pMsg);
}


int CDBAbookView::set_nonICSpn( tagMSG* pMsg )
{
	CWnd *pwnd;

	if( pwnd = GetFocus() )
	{
		if( get_nowpn() != AC_SELDISP_PN && pwnd->m_hWnd == GetDlgItem( IDC_BUTTON_DISP )->m_hWnd )
		{
			set_nowpn( AC_SELDISP_PN );
			return 1;
		}
	}
	return 0;
}


long	CDBAbookView::OnTermBaseDispBtn( long nChar )
{

	if( ! PROGRAM_OK ) return nChar;

	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	switch( nChar ) {
	case VK_UP:
	case VK_LEFT:
	case VK_DOWN:
	case VK_RIGHT:
	case VK_F2:
		if( BRmst )		set_focus( AC_SELBRN_PN );
		else			set_focus( AC_SELKAMOK_PN );
		nChar = 0;
		break;

	case VK_TAB:
		if( BMON_MST )		set_focus( AC_SELBMON_PN );
		else if( M_KOJI )	set_focus( AC_SELKOJI_PN );
		else				set_focus( AC_SELKAMOK_PN );
		nChar = 0;
		break;
	}

	return nChar;
}

//----------------------------------------------------------------------------
//	出納帳仕訳 クエリー
//
//----------------------------------------------------------------------------
int CDBAbookView::Acbook_Requery()
{
	CString filter, tmp, buf;
	// 取消は除く
	filter = ( "(cast(cast(dsign as binary(1)) as int) & 0x01) = 0 and " );

	//開始月、終了月
	if( ((CMainFrame*)GetDBDinpMainFrame())->AcbkKikanSwk() ) {


		buf.Format(" (mofs >= %d and mofs <= %d) and ", m_BaseOfst, m_BaseEndOfs);
		filter += buf;
	}

	tmp.Format( "((dbt = '%s'", m_BaseCode );

	BOOL bBmnQuery = TRUE;
	DWORD dwCode;

	dwCode = _atoh( (LPSTR)(LPCTSTR)m_BaseCode );
	if( (Voln1->bm_isw & 0x10) && dwCode < SONEKICODE )
		bBmnQuery = FALSE;

	if( (m_dwQuery & ACQ_EDA) ) {
		if( m_BaseEda != -1 ) {
			buf.Format( " and dbr = %d", m_BaseEda );
			tmp += buf;
		}
		else {
			buf = " and dbr is null";
			tmp += buf;
		}
	}

	if( (m_dwQuery & ACQ_BUMON) && bBmnQuery ) {
		if( m_BaseBmn != -1 ) {
			buf.Format( " and dbmn = %d", m_BaseBmn );
			tmp += buf;
		}
		else {
			buf = " and dbmn is null";
			tmp += buf;
		}
	}

	if( (m_dwQuery & ACQ_KOJI) ) {
		if( ! m_BaseKoji.IsEmpty() ) {
			buf.Format( " and dkno = '%s'", m_BaseKoji );
			tmp += buf;
		}
		else {
			buf = " and dkno is null";
			tmp += buf;
		}
	}
	tmp += ")";
	tmp += " OR ";

	filter += tmp;

	tmp.Format( "(cre = '%s'", m_BaseCode );

	if( (m_dwQuery & ACQ_EDA) ) {
		if( m_BaseEda != -1 ) {
			buf.Format( " and cbr = %d", m_BaseEda );
			tmp += buf;
		}
		else {
			buf = " and cbr is null";
			tmp += buf;
		}
	}

	if( (m_dwQuery & ACQ_BUMON) && bBmnQuery ) {
		if( m_BaseBmn != -1 ) {
			buf.Format( " and cbmn = %d", m_BaseBmn );
			tmp += buf;
		}
		else {
			buf = " and cbmn is null";
			tmp += buf;
		}
	}

	if( (m_dwQuery & ACQ_KOJI) ) {
		if( ! m_BaseKoji.IsEmpty() ) {
			buf.Format( " and ckno = '%s'", m_BaseKoji );
			tmp += buf;
		}
		else {
			buf = " and ckno is null";
			tmp += buf;
		}
	}
	tmp += "))";
	filter += tmp;

	pDBzm->data_Requery() = TRUE;

	int sort;

	if( IsMasterType(MST_INSERT) ) {
		sort = (m_dwOpt & 0x01) ? 3 : 1;	// 挿入SEQ順 : 日付順
	}
	else {
		sort = (m_dwOpt & 0x01) ? 0 : 1;	// SEQ順 : 日付順
	}

	return pDBzm->data->Requery( DOPEN_MODE, 0, filter, 0, sort );

}


void CDBAbookView::OnUpdateButtonF4(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

// 登録処理
void CDBAbookView::OnUpdateButtonF7(CCmdUI *pCmdUI)
{
	BOOL bRet = FALSE;

	if( ! bCONFIRM_MASTER ) {
		int pn = get_nowpn();

		if( PROGRAM_OK && pn < AC_SELBMON_PN ) {
			if( pn == AC_TKY_PN )
				bRet = TRUE;
			else if( pn != AC_SYZ_PN ) {
				if( acINP_mode == _MODEFY ) {
					bRet = TRUE;
				}
			}
		}
	}
	pCmdUI->Enable( bRet );
}

//先頭仕訳
void CDBAbookView::OnUpdateButtonF8(CCmdUI *pCmdUI)
{
	if( PROGRAM_OK && get_nowpn() < AC_SELBMON_PN ) {
		pCmdUI->Enable( TRUE );
		return;
	}
	pCmdUI->Enable( FALSE );
}

// 貸借変更
void CDBAbookView::OnUpdateButtonF9(CCmdUI *pCmdUI)
{
	if( ! bCONFIRM_MASTER ) {
		if( PROGRAM_OK && get_nowpn() < AC_SELBMON_PN ) {
			pCmdUI->Enable( TRUE );
			return;
		}
	}
	pCmdUI->Enable( FALSE );
}

// 自動仕訳
void CDBAbookView::OnUpdateButtonF12(CCmdUI *pCmdUI)
{
	if( ! bCONFIRM_MASTER ) {
	//	if( PROGRAM_OK && get_nowpn() < AC_SELBMON_PN && acINP_mode == _APPEND ) {
		if( PROGRAM_OK && acINP_mode == _APPEND ) {
			if( get_nowpn() < AC_SELBMON_PN && get_nowpn() != AC_TKY_PN ) {
				pCmdUI->Enable( TRUE );
				return;
			}
		}
	}
	pCmdUI->Enable( FALSE );
}

void CDBAbookView::OnButtonF2()
{
	if( ! PROGRAM_OK )	return;

	int pn;
	pn = get_nowpn();

	if( pn < AC_SELBMON_PN ) {
		CalcWindowDelete();

		if( pn == AC_TKY_PN ) {
			if (m_pSubDtInp->IsDisplay()) {
				m_pSubDtInp->F2Job();
			}
			else {
				// 摘要は先頭文字へ
				short pos;
				pos = m_Input.GetCaretPosition();
				if (pos > 0) {
					m_Input.SetCaretPosition(0);
					return;
				}
				set_termfocus(get_nowln(), pn, -1);
			}
		}
		else {
			int focus_set = 0;
			if( acINP_mode == _APPEND ) {
				if( pn == AC_SKIN_PN ) {
					if( !getLINE_DATA()->lcnd[AC_NKIN_PN].INP_sg && !getLINE_DATA()->lcnd[AC_SKIN_PN].INP_sg ) {
						set_focus( AC_NKIN_PN );
						focus_set++;
					}
				}
			}
			if( ! focus_set ) {
				set_termfocus( get_nowln(), pn, -1 );
			}
		}
	}
	else {
		UINT ID = 0;
		switch( pn ) {
		case AC_SELBMON_PN:
			ID = IDC_ICSDBEDTCTRL2;	break;
		case AC_SELKOJI_PN:
			ID = IDC_ICSDBEDTCTRL3;	break;
		case AC_SELKAMOK_PN:
			ID = IDC_ICSDBEDTCTRL4;	break;
		case AC_SELBRN_PN:
			ID = IDC_ICSDBEDTCTRL5;	break;
		case AC_SELDISP_PN:
			ID = IDC_BUTTON_DISP;	break;
		}

		if( ID == IDC_BUTTON_DISP ) {
			OnTermBaseDispBtn(VK_F2);
		}
		else if( ID > 0 ) {
			OnTerminationIcsdbedtctrl( ID, VK_F2, 0, 0 );
		}
	}
}

void CDBAbookView::OnUpdateButtonF2(CCmdUI *pCmdUI)
{
	if (!PROGRAM_OK)	pCmdUI->Enable(FALSE);
	else {
		if (get_nowpn() == AC_TKY_PN) {
			static short pos = -1;
			short	tmp;
			tmp = m_Input.GetCaretPosition();

			// この判定を入れないと CPU使用率が 100%になる
			if (pos != tmp) {
				// ツールバーの表示文字を先頭文字 or 前項目 へ
				pos = tmp;
				((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(5, pos == 0 ? 0x80 : 0x82 );
			}
		}
		pCmdUI->Enable(TRUE);
	}
}


// 日付コントロールからの選択
LRESULT CDBAbookView::OnDateSelectMessageChangeMNTHSEL(WPARAM wParam, LPARAM lParam)
{
	//処理を追加
	int sofs, eofs, sy, ey, pn;
	BYTE	check[4] = {0};
	m_selDate.GetMonthOffset(sofs, eofs, sy, ey);
	pDBzm->set_datelimit( sofs, eofs, sy, ey );
	BOOL	bFixMode;
		
	if( sofs == eofs ) {
		bFixMode = TRUE;
	}
	else {
		bFixMode = FALSE;
	}
	// 月指定 期間で仕訳表示
	if( ((CMainFrame*)GetDBDinpMainFrame())->AcbkKikanSwk() ) {
		m_BaseOfst = sofs;
		m_BaseEndOfs = eofs;
		m_BaseDate = sy;
		m_BaseEndDate = ey;
	}

	pn = get_nowpn();

	if( pn >= AC_SELBMON_PN ) {
		pDBzm->get_ofsdate( eofs, check );
		CDBINPDataRec drec;
		BYTE	inpd[6] = { 0 };
		pDBzm->GetInpDate( inpd );
		pDBzm->DateConv( inpd, &drec, CONV1_2 );
		if( drec.m_mofs < sofs || drec.m_mofs > eofs ) {
			pDBzm->SetInpDate( check );
		}
		if( ! m_bMnthFocusIgnore ) {
			set_focus( pn );
		}
		return 0;
	}
	else {
		if( ((CMainFrame*)GetDBDinpMainFrame())->AcbkKikanSwk() ) {
			pDBzm->get_ofsdate(eofs, check);
			CDBINPDataRec drec;
			BYTE	inpd[6] = { 0 };
			pDBzm->GetInpDate(inpd);
			pDBzm->DateConv(inpd, &drec, CONV1_2);
			if( drec.m_mofs < sofs || drec.m_mofs > eofs ) {
				pDBzm->SetInpDate(check);
			}
			//日付変更された場合は、科目欄に戻す。
			if( !m_bMnthFocusIgnore ) {
				set_focus(AC_SELKAMOK_PN);
			}
			return 0;
		}
	}

	int ln = get_nowln();
	if( ln <= 0 )	return 0;
	int focus = 0;

	if( acINP_mode == _APPEND ) {

		ACBOOK_LINE* ldata;
		ldata = _getLINE_DATA(ln);

		int sgn = KESY_SG_GET( ldata->bcddate[0] );
		switch( sgn ) {
		case 1 :	check[0] = 0x61;	break;
		case 2 :	check[0] = 0x62;	break;
		case 3 :	check[0] = 0x63;	break;
		case 4 :	check[0] = 0x00;	break;
		default:
			check[0] = ldata->bcddate[0];	break;
		}
		check[1] = ldata->bcddate[1];

		if( ! pDBzm->check_datelimit( check ) ) {
			if( m_InsLine != 0 ) {
				// 仕訳挿入時は入力日のみセットしておく
				BYTE dd[10] = {0};
				pDBzm->get_ofsdate( eofs, dd );
				pDBzm->SetInpDate( dd );
				return 0;;
			}
			else {
				pDBzm->get_ofsdate( eofs, ldata->bcddate );
				pDBzm->SetInpDate( ldata->bcddate );
			}

			sgn = KESY_SG_GET( ldata->bcddate[0] );
			ldata->drec.m_ksign = sgn ? 1 : 0;
			pDBzm->BcdDateToYmd( ldata->bcddate, &ldata->drec.m_ddate );
			ldata->drec.m_mofs = eofs;
			ldata->lcnd[AC_DATE_PN].INP_sg = TRUE;

			TerminationDataSet( AC_DATE_PN );

			if( INP_mode == _APPEND ) {
				m_CREC.m_ddate	= ldata->drec.m_ddate;
				m_CREC.m_mofs		= ldata->drec.m_mofs;
				m_CREC.m_ksign	= ldata->drec.m_ksign;
			}

			if( get_nowpn() == AC_DATE_PN ) {
				VARIANT var;
				BYTE	bcd[10] = {0};
				CString str;
				memcpy( bcd, ldata->bcddate, sizeof ldata->bcddate );

				if( ! bFixMode ) {
					var.pbVal = bcd;
					DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 4 );	
				}
				else {
					var.pbVal = &bcd[1];
					DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_BCD, 2 );	
				}

				pDBzm->BcdDateToString( str, ldata->bcddate, m_bMnthFix );
				DBSetDispString( this, IDC_ICSDBEDTCTRL1, str );
			}
		}
		focus++;
	}
	else {
		//新規入力行が画面内にあるか？
		ACBOOK_LINE* ldata;
		int ln = -1;

		for( int i = 0; i < ACREC_MAX; i++ ) {
			if( m_ALine[i].abs_pos == 0 ) {
				ldata = &m_ALine[i];
				ln = (i+1);
				break;
			}
		}

		if( ln != -1 ) {
			int sgn = KESY_SG_GET( ldata->bcddate[0] );
			switch( sgn ) {
			case 1 :	check[0] = 0x61;	break;
			case 2 :	check[0] = 0x62;	break;
			case 3 :	check[0] = 0x63;	break;
			case 4 :	check[0] = 0x00;	break;
			default:
				check[0] = ldata->bcddate[0];	break;
			}
			check[1] = ldata->bcddate[1];

			if( ! pDBzm->check_datelimit( check ) ) {
				pDBzm->get_ofsdate( eofs, ldata->bcddate );
				pDBzm->SetInpDate( ldata->bcddate );

				sgn = KESY_SG_GET( ldata->bcddate[0] );
				ldata->drec.m_ksign = sgn ? 1 : 0;
				pDBzm->BcdDateToYmd( ldata->bcddate, &ldata->drec.m_ddate );
				ldata->drec.m_mofs = eofs;
				TerminationDataSet( AC_DATE_PN, ln );
			}
		}
		else {
			pDBzm->GetInpDate(check);
			if (!pDBzm->check_datelimit(check)) {
				pDBzm->get_ofsdate(eofs, check);
				CDBINPDataRec drec;
				BYTE	inpd[6] = { 0 };
//pDBzm->GetInpDate(inpd);
//pDBzm->DateConv(inpd, &drec, CONV1_2);
				pDBzm->SetInpDate(check);
			}
		}
	}

	// このハンドラにきたときはFOCUSを再セットする( DateInputChange() は中で行っている)
	BOOL bFocusSet = FALSE;
	if( ! m_bMnthFix ) {	// 月日入力
		if( sofs == eofs ) {
			if( get_nowpn() == AC_DATE_PN ) {
				DateInputChange();		// 日付入力へ
				bFocusSet = TRUE;
			}
			else {
				m_bMnthFix = TRUE;
			}
		}
	}
	else {	// 日付入力
		if( sofs != eofs ) {
			if( get_nowpn() == AC_DATE_PN ) {
				DateInputChange();		// 月日入力へ
				bFocusSet = TRUE;
			}
			else {
				m_bMnthFix = FALSE;
			}
		}
	}

	if( ! bFocusSet && focus ) {
		pn = get_nowpn();
		if( pn != -1 )	set_focus( get_nowpn() );
	}

	return	0L;
}


void CDBAbookView::OnButtonDelete()
{
	if( get_nowpn() == AC_DATE_PN ) {
		keybd_event( VK_DELETE, 0, 0, 0 );
		keybd_event( VK_DELETE, 0, KEYEVENTF_KEYUP, 0 );
	}
}


void CDBAbookView::OnUpdateButtonDelete(CCmdUI *pCmdUI)
{
	if( ! bCONFIRM_MASTER ) {
		if( get_nowpn() == AC_DATE_PN && acINP_mode != _APPEND ) {
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}


void CDBAbookView::OnButtonInsert()
{
	if( get_nowpn() == AC_DATE_PN ) {
		keybd_event( VK_INSERT, 0, 0, 0 );
		keybd_event( VK_INSERT, 0, KEYEVENTF_KEYUP, 0 );
	}
}

void CDBAbookView::OnUpdateButtonInsert(CCmdUI *pCmdUI)
{
	BOOL bEnaIns = FALSE;

	if( ! bCONFIRM_MASTER ) {
		if( ! IsMasterType(MST_INSERT) ) {
			if( (m_dwOpt & 0x0f) == 0 ) {	//日付順のみ
				bEnaIns = TRUE;
			}
		}
		else {
			bEnaIns = TRUE;	//挿入SEQ対応マスター
		}
	}

	if( bEnaIns ) {
		if( get_nowpn() == AC_DATE_PN ) {
			if( acINP_mode != _APPEND || (acINP_mode == _APPEND && m_InsLine > 0) ) {
				pCmdUI->Enable(TRUE);
				return;
			}
		}
	}
	//定型仕訳へ
	if( get_nowpn() == AC_DATE_PN ) {
		if( acINP_mode == _APPEND && m_InsLine == 0) {
			pCmdUI->Enable(TRUE);
			return;
		}
	}

	pCmdUI->Enable(FALSE);
}

// 業務切り替え時の 最終データ取得
void CDBAbookView::LastDataGet()
{
	// FocusOFFを発生させて、各ターミネーションOFF処理で 記述
	if( pDBzm == NULL ) return;

	m_List.SetFocus();
}


// フォーク用の科目コードをUCOMにセットする
void CDBAbookView::ForkUcomSet()
{
	int pn, sgn;
	if( pDBzm == NULL )
		return;
	// F6 月指定 で 仕訳期間表示ダイアログ表示中に別ウィンドウ切替
	if( m_bMnthFocusIgnore ) {
		return;
	}

	PROGRAM_OK = FALSE;

	pn = get_nowpn();
	char buf[64] = {0}, buf1[36] = {0}, buf2[36] = {0}, buf3[36] = {0};
	char	zandc[16] = { 0 };

	// データ書き込み
	UpdateModfyData(1);

	if( pn == AC_SELKAMOK_PN ) {
		if( ! m_BaseCode.IsEmpty() )
			sprintf_s( buf2, sizeof buf2, "%s", m_BaseCode );

		UCOM_SetString_BC( _T( "共通項目１" ), buf, buf1, buf2, buf3 );

		::ZeroMemory( buf, sizeof buf );
		::ZeroMemory( buf2, sizeof buf2 );

		if( m_BaseEda != -1 )
			sprintf_s( buf, sizeof buf, "%d", m_BaseEda );
		if( m_BaseBmn != -1 )
			sprintf_s( buf2, sizeof buf2, "%d", m_BaseBmn );

		UCOM_SetString_BC( _T( "共通項目２" ), buf, buf1, buf2, buf3 );
		UCOM_SetString_BC(_T("残問貸借チェック"), zandc);
	}
	else {
		if( ZmoveMode() ) {
		//	pDBzm->MakeUcomSeldate( buf, sizeof buf, buf1, sizeof buf1 );
			::ZeroMemory( buf, sizeof buf );
			::ZeroMemory( buf1, sizeof buf1 );
		}
		else {
			if( m_CREC.m_ksign ) {
				BYTE bcd[8] = {0};
				pDBzm->DateConv( bcd, &m_CREC, CV2_1 );
				int yy, dd;
				yy = m_CREC.m_ddate / 10000;
				dd = m_CREC.m_ddate % 100;

				sgn = KESY_SG_GET( bcd[0] );

				switch( sgn ) {
				case 1:	sprintf_s( buf, sizeof buf, "%04d61%02d", yy, dd );	break;
				case 2:	sprintf_s( buf, sizeof buf, "%04d62%02d", yy, dd );	break;
				case 3:	sprintf_s( buf, sizeof buf, "%04d63%02d", yy, dd );	break;
				case 4:	sprintf_s( buf, sizeof buf, "%04d00%02d", yy, dd );	break;
				}
			}
			else {
				sprintf_s( buf, sizeof buf,  "%d",m_CREC.m_ddate );
			}
		}

		if( pn == AC_KMK_PN || acINP_mode == _MODEFY ) {
			int dcsw;
			dcsw = GetRecordDCsgn( &m_CREC );

			if( dcsw == 0 || dcsw == -1 ) {
				// 基本科目が借方 or 同じ
				if( ! m_CREC.m_cre.IsEmpty() )
					sprintf_s( buf2, sizeof buf2, "%s",m_CREC.m_cre );

				UCOM_SetString_BC( _T( "共通項目１" ), buf, buf1, buf2, buf3 );

				::ZeroMemory( buf, sizeof buf );
				::ZeroMemory( buf2, sizeof buf2 );
				::ZeroMemory( buf1, sizeof buf1 );

				if( m_CREC.m_cbr != -1 )
					sprintf_s( buf, sizeof buf, "%d", m_CREC.m_cbr );
				if( m_CREC.m_cbmn != -1 )
					sprintf_s( buf2, sizeof buf2, "%d", m_CREC.m_cbmn );

				UCOM_SetString_BC( _T( "共通項目２" ), buf, buf1, buf2, buf3 );
				if( dcsw == 0 ) {
					strcpy_s(zandc, sizeof zandc, "1");
				}
			}
			else {
				// 基本科目が貸方
				if( ! m_CREC.m_dbt.IsEmpty() )
					sprintf_s( buf2, sizeof buf2, "%s",m_CREC.m_dbt );

				UCOM_SetString_BC( _T( "共通項目１" ), buf, buf1, buf2, buf3 );

				::ZeroMemory( buf, sizeof buf );
				::ZeroMemory( buf2, sizeof buf2 );
				::ZeroMemory( buf1, sizeof buf1 );

				if( m_CREC.m_dbr != -1 )
					sprintf_s( buf, sizeof buf, "%d", m_CREC.m_dbr );
				if( m_CREC.m_dbmn != -1 )
					sprintf_s( buf2, sizeof buf2, "%d", m_CREC.m_dbmn );

				UCOM_SetString_BC( _T( "共通項目２" ), buf, buf1, buf2, buf3 );
				strcpy_s(zandc, sizeof zandc, "0");
			}
		}
		else {
			if( ZmoveMode() ) {
				::ZeroMemory( buf2, sizeof buf2 );
				UCOM_SetString_BC( _T( "共通項目１" ), buf, buf1, buf2, buf2 );
				UCOM_SetString_BC( _T( "共通項目２" ), buf2, buf2, buf2, buf2 );
			}
			else {
				::ZeroMemory( buf1, sizeof buf1 );
				UCOM_SetString_BC( _T( "共通項目１" ), buf, buf1, buf1, buf1 );
				UCOM_SetString_BC( _T( "共通項目２" ), buf1, buf1, buf1, buf1 );
			}
		}
		UCOM_SetString_BC(_T("残問貸借チェック"), zandc);
	}

	PROGRAM_OK = TRUE;
}


// 他プロセスからの戻り再表示
int CDBAbookView::ReloadView()
{
//MyTrace( "===> @ReloadView TOP\n" );

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


	// 選択部分の更新
	m_Seldata.SelectUpdate();

	// 科目指定部分 再表示
	struct _KamokuData check = {0};
	struct _DBKNREC* pKn;

	strncpy_s( check.kd_code, sizeof check.kd_code, m_BaseCode, m_BaseCode.GetLength() );
	check.kd_eda = -1;
	pKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
	if( pKn ) {
		KamokuString( pKn, check.kd_name, sizeof check.kd_name );
	//	memcpy( check.kd_name, pKn->knnam, sizeof pKn->knnam );
	}
	else {
		m_BaseCode = GNKINCode( Voln1->apno );
		pKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
		if( pKn ) {
			KamokuString( pKn, check.kd_name, sizeof check.kd_name );
		}
	}

	DBkamokudata_set( this, IDC_ICSDBEDTCTRL4, &check, pDBzm );

	// 枝番部分表示
	struct _SET_KMKETC sk;
	sk.bmn = m_BaseBmn;
	sk.brn = m_BaseEda;
	sk.code = m_BaseCode;

	if( _set_kmketc( &sk, FALSE ) ) {
		GetDlgItem(IDC_STATIC5)->SetWindowText( sk.etc );
	}
	else {
		GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
	}

	if( get_nowpn() >= AC_SELBMON_PN ) {
		//基本設定部分にある
	}
	else {
	//	m_List.SetFocus();

	//	((CDialog*)this)->GotoDlgCtrl( GetDlgItem( IDC_BUTTON_DISP ) );
	//	_set_focus( -1, AC_SELDISP_PN );
	//	m_Input.ShowWindow( SW_HIDE );
	//	set_nowpn( AC_SELDISP_PN, 1 );
	// データ部分 再表示
	//	_OnBnClickedButtonDisp();
		// 12.01 /09
	//	PostMessage( WM_COMMAND, MAKELONG(IDC_BUTTON_DISP, BN_CLICKED), 
	//				(LPARAM)GetDlgItem(IDC_BUTTON_DISP)->m_hWnd);

		ReloadDataDisp();
	}

	AutoSelIndicator( acINP_mode );
//MyTrace( "===> @ReloadView END\n" );

	return 0;
}

// 07.26 /10
void CDBAbookView::ReloadDataDisp()
{
	ACBOOK_LINE* al;
	ACBOOK_LINE data;

	int set_append = 0;

//MyTrace( "====> @ReloadDataDisp TOP\n" );

	m_bReloadFocus = TRUE;

	if( acINP_mode == _APPEND ) {
		set_append = 1;
		al = getLINE_DATA();
		data = *al;
	}

	acline_init( m_ALine, ACREC_MAX );
	m_List.RemoveAllDtListData();
	m_List.RedrawDataListCtrl();
	// Termination OFF 用
	set_nowln(-1);
	acINP_mode = _RETREV;

	int n;
	if( ( n= InitDataSet() ) < 0 )
		return;

	int ln = 0;

	//前回分の新規ライン
	for( int i = 0; i < ACREC_MAX; i++ ) {
		if( m_ALine[i].abs_pos == 0 ) {
			if( set_append ) {
				m_ALine[i] = data;
			}
			ln = i+1;
			break;
		}
	}

	ListDataSet();

	// フォーカスをセットする。
	_set_focus( ln, AC_DATE_PN );

//MyTrace( "====> @ReloadDataDisp END _set_focus( ln = %d, AC_DATE_PN )\n", ln );
}


// 会社再選択時
void CDBAbookView::ReCoselSetFocus( BOOL bNewCo )
{
	if( bNewCo ) {
		// 11.21 /08 会社を再選択したとき、仕訳データを取得するのに失敗していた。
		pDBzm->dbdata_Job()		= DBDATA_JOB_ACBOOK;
		pDBzm->dbdata_Sort()	= DBDATA_SORT_SEQ;
		pDBzm->dbdata_Requery()	= TRUE;
		pDBzm->dbdata_Speed()	= FALSE;

		m_selDate.SetMode( ICS_DATESELECT_MODE_ONESELF );
		m_selDate.Iniz( pDBzm, ICS_DATESELECT_RANGE_PERIOD|ICS_DATESELECT_EDIT_OFF|ICS_DATESELECT_INPOK_ONLY, 0, 0 );
		int sofs, eofs, sdate, edate;
		m_selDate.GetMonthOffset( sofs, eofs, sdate, edate );

		PostMessage( WM_COMMAND, MAKELONG(ID_BUTTON_F11, 0) );
	}
	else {
		int ln, pn;
		ln = get_nowln();
		pn = get_nowpn();

		_set_focus( ln, pn );
	}
}


// マウスホイール をキーボードに変換
BOOL CDBAbookView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CWnd* pwnd;
	pwnd = GetFocus();

	if( pwnd->GetSafeHwnd() ) {
		int id, key = 1;
		id = pwnd->GetDlgCtrlID();

		//2回連続で来るので2回目は無視
		static int wheelcnt = 0;
		if( id == IDC_ICSDBEDTCTRL1 ) {
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
		}
	}

	return CDBbaseView::OnMouseWheel(nFlags, zDelta, pt);
}



void CDBAbookView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
MyTrace( "XXX@CDBAbookView::OnActivateView TOP bActivate = %d, PROGRAM_OK = %d\n", bActivate, PROGRAM_OK );	
	CWnd* pwnd;
	pwnd = GetDBDinpMainFrame();

	if( PROGRAM_OK || ((CMainFrame*)pwnd)->IsForkChain() ) {

		CWnd* pwnd;
		pwnd = GetDBDinpMainFrame();
		if( pwnd && ((CMainFrame*)pwnd)->NowView() == ACBK_VIEW ) {

			if( bActivate ) {
				if( IsWindow( m_Input.m_hWnd ) ) {
					int pn;
					pn = get_nowpn();
MyTrace( "XXX@CDBAbookView::OnActivateView LINE= %d, bReload = %d, bActive = %d\n", __LINE__, m_bReloadFocus, m_bActiveFocus );	

					if( ! ((CMainFrame*)pwnd)->IsReCOSEL() ) {
MyTrace( "XXX@CDBAbookView::OnActivateView LINE= %d, bReload = %d, bActive = %d\n", __LINE__, m_bReloadFocus, m_bActiveFocus );	

						if( pn != AC_SYZ_PN  ) {
							int ln = get_nowln();
							if( ! m_bReloadFocus ) {	//他からの戻りで、画面再表示時に set_focus 済
								set_focus( pn );
								m_bActiveFocus = TRUE;
							}
MyTrace( "XXX@CDBAbookView::OnActivateView set_focus(re = %d, actv = %d) ln %d pn %d \n", m_bReloadFocus, m_bActiveFocus, ln, pn );	
						}
					}
				}
			}
			else {
			//	GetAcbookItemWidth();
			}
		}
	}

	CDBbaseView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


// 月指定
void CDBAbookView::OnButtonF6()
{
	CalcWindowDelete();
	CMnthDlg	dlg;

	SRCTrace;

	if( ((CMainFrame*)GetDBDinpMainFrame())->AcbkKikanSwk() ) {
		PROGRAM_OK = FALSE;
		m_bMnthFocusIgnore = TRUE;
	}

	SRCTrace;

	int st = dlg.DoModal();


	SRCTrace;

	if( st == IDOK ) {
		m_selDate.SetMonthOffset( dlg.m_Sofs, dlg.m_Eofs, dlg.m_Sdate, dlg.m_Edate );
		SRCTrace;

		OnDateSelectMessageChangeMNTHSEL(0,0);

		SRCTrace;

		if( ((CMainFrame*)GetDBDinpMainFrame())->AcbkKikanSwk() ) {
			_OnTerminationIcsdbedtctrl(0, 0, 0);
			set_nowpn(AC_SELKAMOK_PN);
		//	PostMessage( WM_COMMAND, MAKELONG(IDC_BUTTON_DISP, BN_CLICKED),	(LPARAM)GetDlgItem(IDC_BUTTON_DISP)->m_hWnd);
			_OnBnClickedButtonDisp();
			m_bMnthFocusIgnore = FALSE;
			PROGRAM_OK = TRUE;
		}
	}
	else {
		if( ((CMainFrame*)GetDBDinpMainFrame())->AcbkKikanSwk() ) {
			m_bMnthFocusIgnore = FALSE;
			PROGRAM_OK = TRUE;
		}
	}
	SRCTrace;
}

void CDBAbookView::OnUpdateButtonF6(CCmdUI *pCmdUI)
{
	BOOL bRet = FALSE;

	if( ! bCONFIRM_MASTER ) {
		if( PROGRAM_OK && get_nowpn() < AC_SELBMON_PN ) {
			bRet = TRUE;
		}
	}
	pCmdUI->Enable( bRet );
}


// 表示設定
void CDBAbookView::OnButtonF3()
{
	CACDspOptDlg dlg;
	BOOL bKeepSgn[10] = {0};
	int ret, stchg;

	CalcWindowDelete();

	GetAcbookItemWidth();

	DBKNREC* pKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
	CString strKnam;
	if( pKn != NULL ) {
		strKnam.Format( "%.14s", pKn->knnam );
		dlg.m_BaseKnam = strKnam;
	}

	dlg.m_BmnYes	= (pAUTOSEL->DSPBMON_OPT == OPT_NONOPT) ? 0 : 1;
	dlg.m_KojiYes	= (pAUTOSEL->DSPKOJI_OPT == OPT_NONOPT) ? 0 : 1;
//	if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ )	dlg.m_Hjiyu = 1;
	if( IsHjiyuItemGyosyu() == 0 )	dlg.m_Hjiyu = 1;
	else {
		dlg.m_Hjiyu		= (pAUTOSEL->DSPHJIYU_OPT == OPT_NONOPT) ? 0 : 1;
	}
	dlg.m_Zei		= (pAUTOSEL->DSPZEI_OPT == OPT_NONOPT) ? 0 : 1;
	dlg.m_DenpYes	= (pAUTOSEL->DSPDENP_OPT == OPT_NONOPT) ? 0 : 1;
	dlg.m_Syz		= (pAUTOSEL->DSPSYZ_OPT == OPT_NONOPT) ? 0 : 1;

	bKeepSgn[0] = dlg.m_BmnYes;
	bKeepSgn[1] = dlg.m_KojiYes;
	bKeepSgn[2] = dlg.m_Hjiyu;
	bKeepSgn[3] = dlg.m_Zei;
	bKeepSgn[4] = dlg.m_DenpYes;
	bKeepSgn[5] = dlg.m_Syz;

	if( get_nowpn() < AC_SELBMON_PN )
		dlg.m_bChkQuery = FALSE;
	else {
		dlg.m_bChkQuery = TRUE;
	}
	if( BMON_MST ) {
		if( pDBzm->zvol->bm_isw & 0x10 ) {
			DWORD	dwCode;
			dwCode = _atoh( (LPSTR)(LPCTSTR)m_BaseCode );

			if( dwCode < SONEKICODE ) {
				dlg.m_bEnableBmn	= FALSE;
				dlg.m_chkBumon		= FALSE;
			}
			else {
				dlg.m_chkBumon	= (m_dwQuery & ACQ_BUMON) ? TRUE : FALSE;
			}
		}
		else {
			dlg.m_chkBumon	=	(m_dwQuery & ACQ_BUMON) ? TRUE : FALSE;
		}
	}
	if( M_KOJI )	dlg.m_chkKoji	=	(m_dwQuery & ACQ_KOJI) ? TRUE : FALSE;
	if( BRmst )		dlg.m_chkEda	=	(m_dwQuery & ACQ_EDA) ? TRUE : FALSE;

	//仕訳表示順
	dlg.m_DispDate = (m_dwOpt & 0x01) ? 1 : 0;
	dlg.m_chkKikan = ((CMainFrame*)GetDBDinpMainFrame())->AcbkKikanSwk();

PROGRAM_OK = FALSE;

	ret = dlg.DoModal();

PROGRAM_OK = TRUE;

	if( ret == IDOK ) {
		VARIANT var;

		if( dlg.m_bChkQuery ) {
			if( BMON_MST ) {	// 部門
				if( dlg.m_chkBumon )	m_dwQuery |= ACQ_BUMON;
				else {
					var.pbVal = NULL;
					DBdata_set( this, IDC_ICSDBEDTCTRL2, &var, ICSDBEDT_TYPE_STRING, 0 );
					
					m_dwQuery &= ~ACQ_BUMON;
					m_BaseBmn	= -1;

					m_BmnSK.bmn = -1;
					m_BmnSK.kno.Empty();
					m_BmnSK.code.Empty();
					m_BmnSK.brn = -1;
					m_BmnSK.etc.Empty();
					m_BmnSK.etc_col = (COLORREF)-1;
					SetBmnEtcDisp( FALSE );
				}
				m_BmnInp.EnableWindow( dlg.m_chkBumon );

				BaseBmnCtrl();
			}
			if( M_KOJI ) {		// 工事番号
				if( dlg.m_chkKoji )	m_dwQuery |= ACQ_KOJI;
				else {
					var.pbVal = NULL;
					DBdata_set( this, IDC_ICSDBEDTCTRL3, &var, ICSDBEDT_TYPE_STRING, 0 );
					
					m_dwQuery &= ~ACQ_KOJI;
					m_BaseKoji.Empty();

					m_KojiSK.bmn = -1;
					m_KojiSK.kno.Empty();
					m_KojiSK.code.Empty();
					m_KojiSK.brn = -1;
					m_KojiSK.etc.Empty();
					m_KojiSK.etc_col = (COLORREF)-1;
					SetKojiEtcDisp( FALSE );
				}

				m_KojiInp.EnableWindow( dlg.m_chkKoji );
			}
			if( BRmst ) {		// 枝番
				if( dlg.m_chkEda )	m_dwQuery |= ACQ_EDA;
				else {
					var.pbVal = NULL;
					DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
					
					m_dwQuery &= ~ACQ_EDA;
					m_BaseEda	=	-1;

					GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
				}

				m_BrnInp.EnableWindow( dlg.m_chkEda );
			}
			int pn = get_nowpn();

			if( pn == AC_SELBMON_PN ) {
				if( ! (m_dwQuery & ACQ_BUMON) ) {
					set_focus( AC_SELKAMOK_PN );
				}
			}
			else if( pn == AC_SELKOJI_PN ) {
				if( ! (m_dwQuery & ACQ_KOJI) ) {
					set_focus( AC_SELKAMOK_PN );
				}
			}
			else if( pn == AC_SELBRN_PN ) {
				if( ! (m_dwQuery & ACQ_EDA) ) {
					set_focus( AC_SELKAMOK_PN );
				}
			}

			//仕訳表示順
			m_dwOpt = dlg.m_DispDate ? 1 : 0;

			int opt;
			opt = m_dwQuery;
			if( m_dwOpt & 0x01 )	opt |= 0x100;

			((CMainFrame*)GetDBDinpMainFrame())->AcbkOption() = opt;
			((CMainFrame*)GetDBDinpMainFrame())->AcbkKikanSwk() = dlg.m_chkKikan;

			int sofs, eofs, sy, ey;
			m_selDate.GetMonthOffset(sofs, eofs, sy, ey);
			pDBzm->set_datelimit(sofs, eofs, sy, ey);
			// 月指定 期間で仕訳表示
			if( ((CMainFrame*)GetDBDinpMainFrame())->AcbkKikanSwk() ) {
				m_BaseOfst = sofs;
				m_BaseEndOfs = eofs;
				m_BaseDate = sy;
				m_BaseEndDate = ey;
			}
		}

		stchg = 0;

		// 伝票
		if( ! dlg.m_DenpYes ) {
			pAUTOSEL->DSPDENP_OPT = OPT_NONOPT;
		}
		else {
			pAUTOSEL->DSPDENP_OPT = OPT_NODSP;
		}
		if( bKeepSgn[4] != dlg.m_DenpYes )	stchg++;

		// 部門
		if( ! dlg.m_BmnYes ) {
			pAUTOSEL->DSPBMON_OPT = OPT_NONOPT;
		}
		else {
			pAUTOSEL->DSPBMON_OPT = OPT_NODSP;
		}
		if( bKeepSgn[0] != dlg.m_BmnYes )	stchg++;

		// 工事
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
		if( bKeepSgn[1] != dlg.m_KojiYes )	stchg++;
		
		// 変動事由
		if( ! dlg.m_Hjiyu ) {
			pAUTOSEL->DSPHJIYU_OPT = OPT_NONOPT;
		}
		else {
			pAUTOSEL->DSPHJIYU_OPT = OPT_NODSP;
		}
		if( bKeepSgn[2] != dlg.m_Hjiyu  )	stchg++;

		// 税額		09.20 /07
		if( ! dlg.m_Zei ) {
			pAUTOSEL->DSPZEI_OPT = OPT_NONOPT;
		}
		else {
			pAUTOSEL->DSPZEI_OPT = OPT_NODSP;
		}
		if( bKeepSgn[3] != dlg.m_Zei  )	stchg++;

		// 消費税	11.05 /20
		if (!dlg.m_Syz) {
			pAUTOSEL->DSPSYZ_OPT = OPT_NONOPT;
		}
		else {
			pAUTOSEL->DSPSYZ_OPT = OPT_NODSP;
		}
		if (bKeepSgn[5] != dlg.m_Syz)	stchg++;

		if( stchg ) {
			ListClmSet();

			// 今フォーカスがあったところが 非表示になる場合
			int set_focus_sgn = 0;
			int acpn = get_nowpn();

			switch( acpn ) {
			case AC_BMON_PN:
				if( dlg.m_BmnYes ) {
					set_focus( AC_DATE_PN );
					set_focus_sgn = 1;
				}	
				break;
			case AC_KOJI_PN:
				if( dlg.m_KojiYes ) {
					set_focus( AC_DATE_PN );
					set_focus_sgn = 1;
				}	
				break;
			case AC_HJIYU_PN:
				if( dlg.m_Hjiyu ) {
					set_focus( AC_DATE_PN );
					set_focus_sgn = 1;
				}	
				break;
			// 税額		09.20 /07
			case AC_ZEI_PN:
				if( dlg.m_Zei ) {
					set_focus( AC_DATE_PN );
					set_focus_sgn = 1;
				}	
				break;
			case AC_SYZ_PN:
				if (dlg.m_Syz) {
					set_focus(AC_DATE_PN);
					set_focus_sgn = 1;
				}
				break;

			case AC_DENP_PN:
				if( dlg.m_DenpYes ) {
					set_focus( AC_DATE_PN );
					set_focus_sgn = 1;
				}	
				break;
			}

			// 入力の DBEDT の 位置調整
			if( ! set_focus_sgn ) {
				DT_RECT* pdt;
				pdt = m_List.GetDtRect();

				if( pdt == NULL )
					return;

				int did;
				did = get_dctrl_id( acpn );

				for( int i = 0; i < pdt->Dtcnt; i++ ) {
					if( pdt->DtID[i] == did ) {
						CRect inprc, rect;
						inprc = pdt->DtInpRect[i];

						m_List.ClientToScreen( inprc );
						ScreenToClient( inprc );

						m_Input.GetWindowRect( &rect );
						ScreenToClient( &rect );

						if( did != DATA_HUSEN ) {
							m_Input.MoveWindow( inprc.left, rect.top, inprc.Width(), rect.Height() );
						}
						break;
					}
				}
			}
		}
	}
}


//
//	兼業時の現金　科目チェック
//
void CDBAbookView::BaseKmkIndCheck()
{	
	int type =  IsKojinGyosyuMaster( pDBzm );
	if( type ) {
		// データ入力中は変更しない。
		if( get_nowpn() != -1 ) {
			if( get_nowpn() < AC_SELBMON_PN )
				return;
		}
		struct _KamokuData  akd = {0}, ckd = {0};
		VARIANT	var;
		int i;

		strcpy_s( ckd.kd_code, sizeof ckd.kd_code, m_BaseCode );
		ckd.kd_sgn = 2;	// 科目コードチェック用に 枝番のみのサインセット
		if( ! m_Seldata.InputItemCheck( akd, ckd, 0 ) ) {
			// 
			DBKNREC* pKn;
			DWORD dwBaseCode = _atoh( (LPSTR)(LPCTSTR)m_BaseCode );
			BOOL	bSet = FALSE;

			for( i = 0; i < pDBzm->knm_info->reccount; i++ ) {

				pKn = pDBzm->pKnrec+i;

				DWORD dwCode;
				dwCode = pKn->kncod;
				hl_rev( &dwCode, 4 );

				if( dwCode == dwBaseCode )
					continue;

				if( dwCode >= 0x01010200 )
					break;

				if( (dwCode & 0xffffff00) == 0x01010100 ) {
					sprintf_s( ckd.kd_code, sizeof ckd.kd_code, "%08x", dwCode );
					ckd.kd_sgn = 2;	// 科目コードチェック用に 枝番のみのサインセット

					if( ! m_Seldata.InputItemCheck( akd, ckd, 0 ) )
						continue;

					m_BaseCode.Format( "%08x", dwCode );
					m_BaseAtt	= pKn->knatt;
					akd.kd_eda = -1;
					DBkamokudata_set( this, IDC_ICSDBEDTCTRL4, &akd, pDBzm );

					m_BaseEda = -1;		//基本枝番消去
					var.pbVal = NULL;
					DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
					GetDlgItem(IDC_STATIC5)->SetWindowText( "" );

					bSet = TRUE;
					break;
				}
			}

			if( ! bSet ) {
				// 現金のグループで該当科目がない場合は、未確定科目
				m_BaseCode = "00010101";

				strncpy_s( akd.kd_code, sizeof akd.kd_code, m_BaseCode, m_BaseCode.GetLength() );
				akd.kd_eda = -1;
				pKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
				if( pKn ) {
					KamokuString( pKn, akd.kd_name, sizeof akd.kd_name );
				//	memcpy( akd.kd_name, pKn->knnam, sizeof pKn->knnam );
					m_BaseAtt = pKn->knatt;
				}
				DBkamokudata_set( this, IDC_ICSDBEDTCTRL4, &akd, pDBzm );

				m_BaseEda = -1;		//基本枝番消去
				var.pbVal = NULL;
				DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
				GetDlgItem(IDC_STATIC5)->SetWindowText( "" );
			}
		}
	}
}

//
//	仕訳行再表示 
//
void CDBAbookView::DataLineUpdate( CDBINPDataRec *rec, int line, int rdsgn/*=0*/ )
{	
	DTLIST_DATA data;

	int pncnt;
	int pntbl[] = 
	{
		AC_SEQ_PN, AC_DATE_PN, AC_BMON_PN, AC_KOJI_PN,
		AC_KMK_PN, AC_TKY_PN, AC_HJIYU_PN,
		AC_NKIN_PN,AC_SKIN_PN,AC_ZEI_PN,AC_SYZ_PN,AC_SZAN_PN,AC_HUSEN_PN
	};
	pncnt = sizeof(pntbl)/sizeof(pntbl[0]);

	m_List.GetDtListData( line-1, &data );
	
	ACBOOK_LINE *dtline;
	dtline = _getLINE_DATA( line );
	dtline->drec = *rec;

	char buf[64];
	l_print(buf, rec->m_val, FMT15);

	if( rdsgn ) {
		//入力状態サインをセットする
		int dc_sw = GetRecordDCsgn( rec );
		dtline->lcnd[AC_KMK_PN].INP_sg = TRUE;

		if( l_test( dtline->drec.m_val ) ) {

			// 借方／貸方が同じ科目の場合は、入金として処理
			if( _IsSameKamoku( &dtline->drec ) ) {
				//科目を変更した場合で、出金にある場合
				dtline->lcnd[AC_NKIN_PN].INP_sg = TRUE;
				dtline->lcnd[AC_SKIN_PN].INP_sg = FALSE;
			}
			else {
				if( dc_sw == -1 || dc_sw == 0  ) {
					dtline->lcnd[AC_NKIN_PN].INP_sg = TRUE;
					dtline->lcnd[AC_SKIN_PN].INP_sg = FALSE;
					dc_sw = 0;
				}
				else {
					dtline->lcnd[AC_NKIN_PN].INP_sg = FALSE;
					dtline->lcnd[AC_SKIN_PN].INP_sg = TRUE;
				}
			}
		}
		else {
			dtline->lcnd[AC_NKIN_PN].INP_sg = FALSE;
			dtline->lcnd[AC_SKIN_PN].INP_sg = FALSE;
		}

		// 税額
		if (is_syohizeisiwake( rec )) {
			dtline->lcnd[AC_ZEI_PN].INP_sg = TRUE;
		}
		else {
			dtline->lcnd[AC_ZEI_PN].INP_sg = FALSE;
		}

		dtline->lcnd[AC_SEQ_PN].INP_sg	= TRUE;
		dtline->lcnd[AC_DATE_PN].INP_sg	= TRUE;

		BOOL bInpSgn = FALSE;

		BOOL bDisp;
		bDisp = ((pAUTOSEL->BMNDSP_OPT == OPT_BMNDSP) ? TRUE : FALSE);

		int bmncode = -1;

		if( ! dc_sw ) {
			if (rec->m_cbmn != -1) {
				bmncode = rec->m_cbmn;
				bInpSgn = TRUE;
			}
		}
		else {
			if (rec->m_dbmn != -1) {
				bmncode = rec->m_dbmn;
				bInpSgn = TRUE;
			}
		}
		struct _SET_KMKETC sk;
		sk.bmn = bmncode;
		sk.brn = -1;
		sk.code.Empty();
		sk.kno.Empty();

		if (bDisp && bmncode != -1 && _set_bmnetc(&sk)) {
			dtline->lcnd[AC_BMON_PN].Etcmsg = sk.etc;
			dtline->lcnd[AC_BMON_PN].ETC_txtcol = sk.etc_col;
		}
		else {
			dtline->lcnd[AC_BMON_PN].Etcmsg.Empty();
			dtline->lcnd[AC_BMON_PN].ETC_txtcol = 0;
		}
		dtline->lcnd[AC_BMON_PN].INP_sg	= bInpSgn;

		bInpSgn = FALSE;
		CString strkno;
		if( pDBzm->zvol->apno == 0x20 ) {
			if( ! dc_sw ) {
				if (!rec->m_dkno.IsEmpty()) {
					bInpSgn = TRUE;
					strkno = rec->m_dkno;
				}
			}
			else {
				if (!rec->m_ckno.IsEmpty()) {
					bInpSgn = TRUE;
					strkno = rec->m_ckno;
				}
			}
			sk.bmn = -1;
			sk.brn = -1;
			sk.code.Empty();
			sk.kno = strkno;

			if (bDisp && ! strkno.IsEmpty() && _set_kojietc(&sk)) {
				dtline->lcnd[AC_KOJI_PN].Etcmsg = sk.etc;
				dtline->lcnd[AC_KOJI_PN].ETC_txtcol = sk.etc_col;
			}
			else {
				dtline->lcnd[AC_KOJI_PN].Etcmsg.Empty();
				dtline->lcnd[AC_KOJI_PN].ETC_txtcol = 0;
			}
		}
		dtline->lcnd[AC_KOJI_PN].INP_sg	= bInpSgn;

		dtline->lcnd[AC_TKY_PN].INP_sg	= TRUE;
		dtline->lcnd[AC_SYZ_PN].INP_sg	= TRUE;
	}

	_TerminationDataSet( &data, dtline, pntbl, pncnt );

	BOOL bTekiyoPN = TRUE;
/*
	for( int i = 0; i < pncnt; i++ ) {
		if( pn[i] == AC_TKY_PN ) {
			bTekiyoPN = TRUE;
		}
	}
*/
	if( ! bTekiyoPN )
		m_List.UpdateDtListData( line-1, &data );
	else
		m_List.SetDtListData( line-1, &data );

	m_List.RedrawDataListCtrl();
}

//---------------------------------------------
//	工事未入力・完成工事チェックＪＯＢ
//
//---------------------------------------------
BOOL CDBAbookView::isKojiCheck()
{
	if( !modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
		return FALSE;

	PROGRAM_OK = FALSE;

	int dcsw;
	dcsw  =GetRecordDCsgn( &m_CREC );

	CString code, kno;

	if( dcsw == 0 ) {
		code	= m_CREC.m_cre;
		kno		= m_CREC.m_ckno;
	}
	else if( dcsw == 1 ) {
		code	= m_CREC.m_dbt;
		kno		= m_CREC.m_dkno;
	}

	//
	// 枝番未入力チェック
	if( ! (m_ACKOJICHK & 0x10) )
	{
		m_ACKOJICHK |= 0x10;
		if( koji_noinpcheck( kno, code, -1 ) ) {
			set_focus( AC_KOJI_PN );
			PROGRAM_OK = TRUE;
			return TRUE;
		}
	}

	if( ! (m_ACKOJICHK & 0x20) )
	{
		if( koji_kanseicheck( kno, code, -1 ) ) {
			set_focus( AC_KOJI_PN );
			PROGRAM_OK = TRUE;
			return TRUE;
		}
	}
	
	PROGRAM_OK = TRUE;

	return FALSE;
}


//---------------------------------------------
//	基本科目によって、部門のウィンドウ制御
//
//---------------------------------------------
void CDBAbookView::BaseBmnCtrl()
{
	if( ! BMON_MST ) return;

	//損益のみ
	if( pDBzm->zvol->bm_isw & 0x10 ) {

		DWORD	dwCode;
		dwCode = _atoh( (LPSTR)(LPCTSTR)m_BaseCode );

		if( dwCode < SONEKICODE ) {
			VARIANT var;
			var.pbVal = NULL;
			DBdata_set( this, IDC_ICSDBEDTCTRL2, &var, ICSDBEDT_TYPE_STRING, 0 );

		//	m_dwQuery &= ~ACQ_BUMON;
			m_BaseBmn	= -1;
			m_BmnSK.bmn = -1;
			m_BmnSK.etc.Empty();
			m_BmnSK.etc_col = (DWORD)-1;
			SetBmnEtcDisp( FALSE );

			m_BmnInp.EnableWindow( FALSE );
		}
		else {
			if( m_dwQuery & ACQ_BUMON ) {
				m_BmnInp.EnableWindow( TRUE );
			}
		}
	}

	if( m_BaseBmn != -1 ) {
		if( ! BmChk.BmnHaniCheck( m_BaseBmn, m_BaseCode ) ) {
			m_BaseBmn = -1;
			VARIANT var;
			var.pbVal = NULL;
			DBdata_set( this, IDC_ICSDBEDTCTRL2, &var, ICSDBEDT_TYPE_STRING, 0 );

			m_BmnSK.bmn = -1;
			m_BmnSK.etc.Empty();
			m_BmnSK.etc_col = (DWORD)-1;

			SetBmnEtcDisp( FALSE );
		}
	}
}


// 部門コード 範囲チェック用
BOOL CDBAbookView::bmncode_todata2( CDBINPDataRec* data, int number, int dc_sw, CString* bmnname )
{
	int hchk = 0;

#ifdef CLOSE
	if( get_nowpn() == AC_SELBMON_PN ) {
		hchk = 1;

		if( dc_sw ) {
			data->m_cre = m_BaseCode;
		}
		else {
			data->m_dbt = m_BaseCode;
		}
	}
	else {
		// 新規行のときはチェックしない。
		if( acINP_mode == _APPEND )	hchk = 0;
		else						hchk = 1;
	}
#endif

	return bmncode_todata( data, number, dc_sw, hchk, bmnname );
}


//---------------------------------------------
//	部門範囲チェック
//
//---------------------------------------------
BOOL CDBAbookView::isBmnHaniCheck()
{
	int st;
	BOOL bRet = FALSE;
	char buf[64];

	if( !modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
		return FALSE;

	// 修正時は、_dline_chg で行う
	if( acINP_mode != _APPEND ) {
		return FALSE;
	}

	PROGRAM_OK = FALSE;

	int dcsw;
	dcsw  =GetRecordDCsgn( &m_CREC );

	//
	// 枝番未入力チェック
	struct _DBKNREC* pKn;
	int	bm_sgn;
	CString msg, str;
	DWORD dwCode;

	msg = "《部門範囲チェック》\r\n";

	bm_sgn = 0;

	m_sdbtBmn = m_screBmn = -1;

	//部門範囲チェック
	if( m_CREC.m_dbmn >= 0 ) {
		if( ! BmChk.BmnHaniCheck( m_CREC.m_dbmn, m_CREC.m_dbt ) ) {

			dwCode = _atoh( (LPSTR)(LPCTSTR)m_CREC.m_dbt );
			bm_sgn |= 0x01;

			if( (Voln1->bm_isw & 0x10) ) {
				if( dwCode < SONEKICODE ) {
					m_sdbtBmn = m_CREC.m_dbmn;
					m_CREC.m_dbmn = -1;
					bm_sgn &= ~0x01;
				}
			}
		}
	}

	if( m_CREC.m_cbmn >= 0 ) {
		if( ! BmChk.BmnHaniCheck( m_CREC.m_cbmn, m_CREC.m_cre ) ) {

			dwCode = _atoh( (LPSTR)(LPCTSTR)m_CREC.m_cre );
			bm_sgn |= 0x02;

			if( (Voln1->bm_isw & 0x10) ) {
				if( dwCode < SONEKICODE ) {
					m_screBmn = m_CREC.m_cbmn;
					m_CREC.m_cbmn = -1;
					bm_sgn &= ~0x02;
				}
			}
		}
	}

	if( bm_sgn ) {
		if( bm_sgn & 0x01 ) {
			if( ( pKn = pDBzm->DB_PjisToKnrec( m_CREC.m_dbt ) ) == NULL )
				return -1;

			pDBzm->BumonCodeToStr( buf, sizeof buf, m_CREC.m_dbmn );
			str.Format( "\r\n科目「%.14s」に、部門番号『%s』は設定できません。", pKn->knnam, buf );
			msg += str;
		}

		if( bm_sgn & 0x02 ) {
			if( ( pKn = pDBzm->DB_PjisToKnrec( m_CREC.m_cre ) ) == NULL )
				return -1;

			pDBzm->BumonCodeToStr( buf, sizeof buf, m_CREC.m_cbmn );
			str.Format( "\r\n科目「%.14s」に、部門番号『%s』は設定できません。", pKn->knnam, buf );
			msg += str;
		}

		msg += "\r\n\r\n修正しますか？\r\nいいえの場合、部門を取消して仕訳を書き込みます。";
		st = myICSMessageBox( (LPCSTR)msg, MB_YESNO );

		if( st == IDYES ) {
			set_focus( AC_BMON_PN );
			bRet = TRUE;
		}
		else {
			if( bm_sgn & 0x01 ) {
				m_CREC.m_dbmn = -1;
			}
			if( bm_sgn & 0x02 ) {
				m_CREC.m_cbmn = -1;
			}
		}
	}
	
	PROGRAM_OK = TRUE;

	return bRet;
}


int CDBAbookView::CheckKamokuInput( struct _KamokuData* data )
{
	if( acINP_mode == _APPEND )
		return 0;

	int bmn;
	CString code;

	if( !modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
		return 0;

	int dcsw;
	dcsw  =GetRecordDCsgn( &m_CREC );

	if( dcsw == 0 ) {
		bmn		= m_CREC.m_cbmn;
	}
	else if( dcsw == 1 ) {
		bmn		= m_CREC.m_dbmn;
	}

	if( bmn < 0 )
		return 0;

	code = data->kd_code;
	
	if( ! BmChk.BmnHaniCheck( bmn, code ) )
		return -1;

	return 0;
}


int CDBAbookView::CheckBaseBumonInput( int bmn )
{
	if( bmn < 0 )
		return 0;

	CString code = m_BaseCode;
	
	if( ! BmChk.BmnHaniCheck( bmn, code ) )
		return -1;

	return 0;
}

int CDBAbookView::CheckBaseKamokuInput( struct _KamokuData* data )
{
	int bmn;
	bmn = m_BaseBmn;

	if( bmn < 0 )
		return 0;

	CString code = data->kd_code;
	
	if( ! BmChk.BmnHaniCheck( bmn, code ) )
		return -1;

	return 0;
}

// 仕訳チェック
int CDBAbookView::CheckModifyBmonData( CDBINPDataRec *data, int nowln, int nomsg /*=0*/ )
{
	int dcsw = GetRecordDCsgn( data );

	CString code;
	int bmn = -1;

	if( dcsw == 0 ) {
		code	= data->m_cre;
		bmn		= data->m_cbmn;
	}
	else if( dcsw == 1 ) {
		code	= data->m_dbt;
		bmn		= data->m_dbmn;
	}

	DWORD dwCode;
	CString msg, str;
	char buf[32];
	DBKNREC* pKn;
	int st, ret;

	ret = 0;

	m_sdbtBmn = m_screBmn = -1;

	if( ! BmChk.BmnHaniCheck( bmn, code ) ) {
		dwCode = _atoh( (LPSTR)(LPCTSTR)code );
		if( (Voln1->bm_isw & 0x10) && dwCode < SONEKICODE ) {
			if( dcsw == 0 ) {
				m_screBmn = data->m_cbmn;
				data->m_cbmn = -1;
			}
			else if( dcsw == 1 ) {
				m_sdbtBmn = data->m_dbmn;
				data->m_dbmn = -1;
			}
			if( nowln != -1 ) {
				ACBOOK_LINE *dtline;
				dtline = _getLINE_DATA( nowln );
				dtline->drec = *data;
				TerminationDataSet( AC_BMON_PN, nowln );
			}
		}
		else {

			msg = "《部門範囲チェック》\r\n";
			if( ( pKn = pDBzm->DB_PjisToKnrec( code ) ) == NULL )
				return -1;

			pDBzm->BumonCodeToStr( buf, sizeof buf, bmn );
			str.Format( "\r\n科目「%.14s」に、部門番号『%s』は設定できません。", pKn->knnam, buf );
			msg += str;

			BOOL bOKchg = FALSE;
			if( PROGRAM_OK ) {	
				PROGRAM_OK = FALSE;
				bOKchg = TRUE;
			}

			if( ! nomsg ) {
				msg += "\r\n\r\n修正しますか？\r\nいいえの場合、部門を取消して仕訳を書き込みます。";
				st = myICSMessageBox( (LPCSTR)msg, MB_YESNO );
			}
			else {
				st = IDNO;
			}

			if( bOKchg ) {
				PROGRAM_OK = TRUE;
			}

			if( st == IDNO ) {
				if( dcsw == 0 ) {
					data->m_cbmn = -1;
				}
				else if( dcsw == 1 ) {
					data->m_dbmn = -1;
				}
				if( nowln != -1 ) {
					ACBOOK_LINE *dtline;
					dtline = _getLINE_DATA( nowln );
					dtline->drec = *data;
					TerminationDataSet( AC_BMON_PN, nowln );
				}
			}
			else {
				ret = -1;
			}
		}
	}

	return ret;
}

//---------------------------------------------
//	部門未入力 ＪＯＢ
//
//---------------------------------------------
BOOL CDBAbookView::isBmnNoInp()
{
	if( !modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
		return FALSE;

	PROGRAM_OK = FALSE;

	int dcsw, bmn;
	dcsw  =GetRecordDCsgn( &m_CREC );

	CString code;

	if( dcsw == 0 ) {
		code	= m_CREC.m_cre;
		bmn		= m_CREC.m_cbmn;
	}
	else if( dcsw == 1 ) {
		code	= m_CREC.m_dbt;
		bmn		= m_CREC.m_dbmn;
	}
	else {
		bmn = -1;
	}

	//
	// 枝番未入力チェック
	if( ! (m_ACKOJICHK & 0x1000) )
	{
		m_ACKOJICHK |= 0x1000;
		if( bmn_noinpcheck( bmn, code, -1 ) ) {
			set_focus( AC_BMON_PN );
			PROGRAM_OK = TRUE;
			return TRUE;
		}
	}

	PROGRAM_OK = TRUE;

	return FALSE;
}



// 仕訳の枝番未入力チェック
int CDBAbookView::CheckModifyEdabanData( CDBINPDataRec *data, int nowln, int nomsg /*=0*/ )
{
	int dcsw = GetRecordDCsgn( data );

	if( pAUTOSEL->BRNKAK_OPT != OPT_BRNKAK ) {
		return 0;
	}

	CString code;
	int brn;

	if( dcsw == 0 ) {
		code	= data->m_cre;
		brn		= data->m_cbr;
	}
	else if( dcsw == 1 ) {
		code	= data->m_dbt;
		brn		= data->m_dbr;
	}

	CString msg, str;
	DBKNREC* pKn;
	int st, ret;

	if( brn != -1 ) {
		return 0;
	}
	ret = 0;

	// 枝番残高があるか？
	if( pDBzm->DB_EdabanZanSearch( code, -1 ) == 0 ) {
		msg = "《枝番未入力チェック》\r\n";
		if( ( pKn = pDBzm->DB_PjisToKnrec( code ) ) == NULL )
			return -1;

		str.Format( "\r\n科目「%.14s」に、枝番が入力されていません！", pKn->knnam );
		msg += str;

		BOOL bOKchg = FALSE;
		if( PROGRAM_OK ) {	
			PROGRAM_OK = FALSE;
			bOKchg = TRUE;
		}

		if( ! nomsg ) {
			msg += "\r\n\r\n仕訳の科目を修正しますか？";
			st = myICSMessageBox( (LPCSTR)msg, MB_YESNO );
		}
		else {
			st = IDNO;
		}

		if( bOKchg ) {
			PROGRAM_OK = TRUE;
		}

		if( st == IDNO ) {
			if( nowln != -1 ) {
				ACBOOK_LINE *dtline;
				dtline = _getLINE_DATA( nowln );
				dtline->drec = *data;
				TerminationDataSet( AC_KMK_PN, nowln );
			}
		}
		else {
			ret = -1;
		}
	}

	return ret;
}



// 自動仕訳
int CDBAbookView::GetAutoSwkType()
{
	return m_bAutoSwk ? 1 : 0;
}

//サイズ変更
void CDBAbookView::OnSize(UINT nType, int cx, int cy)
{
	// 未入力枝番チェック等 メッセージボックス表示時で 行数が変更されるのを防ぐ[06.13 /13 下より移動]
	if( ((CMainFrame*)GetDBDinpMainFrame())->MsgBoxOnSizeFlag() )
		return;
	if( ((CMainFrame*)GetDBDinpMainFrame())->IgnoreOnSizeFlag() )
		return;

	CDBbaseView::OnSize(nType, cx, cy);

	BOOL multiJob = TRUE;
	if( IsNotMultiWindow() ) {
		multiJob = FALSE;
	}

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( PROGRAM_OK == FALSE ) {
		if( multiJob && !FirstTime()) {
			BaseWndMove();
			SetBmnEtcDisp(TRUE);
			SetKojiEtcDisp(TRUE);
			_disp_change();
		}
		return;
	}

	BaseWndMove();
	BaseBmnCtrl();
	_disp_change();

	ListClmSet(TRUE);

	int pn = get_nowpn();

	SetBmnEtcDisp( TRUE );
	SetKojiEtcDisp( TRUE );

	// リストを画面に合わせる
	CRect parentRect;
	this->GetClientRect( parentRect );
	CRect childRect;
	m_List.GetClientRect( childRect );

	CRect mvRect;
	m_List.GetWindowRect( mvRect );
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
	m_List.MoveWindow( mvRect );

	//仕訳データ表示チェック
	if( pn < AC_SELBMON_PN ) {
		int dispcnt = GetACListCount(); /*m_List.GetDispDataCount();*/

		int nowln = get_nowln();

		int oldcnt = m_ListCnt;

MyTrace( "@CDBAbookView::OnSize --> dispcnt = %d, oldcnt = %d, nowln = %d", dispcnt, oldcnt, nowln );
		if( oldcnt > 0 ) {
			if( ! m_bLinChg ) {
				line_dispchg( dispcnt, oldcnt );
				m_ListCnt = dispcnt;
			}
		}
	}
	RDRemainDispJOB();
}


//---------------------------------------------------
//		仕訳表示行数の増減によるデータ管理・表示
//		int dispcnt		新表示行数
//		int oldcnt		旧で表示している仕訳数
//						→リストの行数ではない。
//
//---------------------------------------------------
void CDBAbookView::line_dispchg( int dispcnt, int oldcnt )
{
	int i;
//MyTrace( "@CDBAbookView::line_dispchg TOP disp(%d), old(%d)\n", dispcnt, oldcnt );

	m_bLinChg = TRUE;

	if( dispcnt < oldcnt ) {	//表示行数が減った。
		int mv = oldcnt - dispcnt;

//MyTrace( "@CDBAbookView::line_dispchg --> get_nowln() = %d, mv = %d", get_nowln(), mv );
		m_bLineDispChg = TRUE;

		if( get_nowln() > dispcnt ) {
			// 表示ライン 減った分 ずらす
			for( i = 0; i < ACREC_MAX; i++ ) {
				if( (i+mv) > ACREC_MAX-1 ) {
					acline_init( &m_ALine[i], 1 );
				}
				else {
					m_ALine[i] = m_ALine[i+mv];
				}
			}

			if( m_InsLine != 0 ) {
				if( m_InsLine < dispcnt )
					m_InsLine -= mv;
			}
			ListDataSet();

			set_nowln(-1);
			_set_focus( dispcnt, get_nowpn() );
		}
		else {
			int old = get_nowln();
			set_nowln(-1);
			_set_focus( old, get_nowpn() );
		}
	}
	else if( dispcnt > oldcnt ) {
	
		int reccnt	= pDBzm->data->GetRecordCount();
		int index, pos, append = 0;
//MyTrace( "@CDBAbookView::line_dispchg --> reccnt = %d, m_ALine[oldcnt-1].abs_pos = %d", reccnt, m_ALine[oldcnt-1].abs_pos );

		if( m_InsLine == oldcnt ) {
			//最終ラインが挿入
			index = (oldcnt-2);
		}
		else {
			index = (oldcnt-1);
		}

		// 空白行は 何もしない。[]
		pos = m_ALine[index].abs_pos;
		if( pos <= 0 )	index = -1;

		if( index >= 0 ) {
			pos = m_ALine[index].abs_pos;
			pos++;

			pDBzm->data->SetAbsolutePosition(pos);

			if( pDBzm->data->st == -1 ) {
				if( m_InsLine == 0 ) {
					append = oldcnt+1;
				}
			}
			else {
				// ページデータセット
				// 表示ラインが増えた分 作成
				if( append == 0 ) {
					for( i = oldcnt; i < dispcnt; i++ ) {
						acline_set( &m_ALine[0], i, pos );
						memcpy( m_ALine[i].szan, m_SZan[pos].ac_szan, sizeof m_ALine[i].szan );
						pos++;
//MyTrace( "@CDBAbookView::line_dispchg i = %d, pos = %d\n", i, pos );

						pDBzm->data->MoveNext();
						if( pDBzm->data->st == -1 ) {
							if( (i+1)+1 <= dispcnt ) {
								if( m_InsLine == 0 ) {
									append = (i+1)+1;	// 追加行
								}
							}
							break;
						}
					}
				}
			}

			// 新規行 を追加
			if( append && ! bCONFIRM_MASTER ) {
				i = (append-1);
				m_ALine[i].abs_pos = 0;
				int newseq = pDBzm->m_lastdata.m_seq;

				pDBzm->GetInpDate( &m_ALine[i].drec );
				pDBzm->GetInpDate( m_ALine[i].bcddate );
				m_ALine[i].drec.m_seq = newseq;
			}

			ListDataSet();
		}

		_set_focus( get_nowln(), get_nowpn() );
	}

	m_bLinChg = FALSE;

//MyTrace( "@CDBAbookView::line_dispchg END\n" );
}


// 10.18 /10
int CDBAbookView::GetAcbkKojiWidth()
{
	int kjwd = 0;
	
	//最大化時のみ保存
	if( GetDBDinpMainFrame()->IsZoomed() ) {
		kjwd = m_List.GetListItemWidth( DATA_DEB_KNO );
	}

	((CMainFrame*)GetDBDinpMainFrame())->AcbkKojiWd() = kjwd;

	return kjwd;
}


// 会社再選択時の 出納帳データ消去
int CDBAbookView::ReCosel_Job()
{
	set_nowln(-1);
	set_nowpn( AC_SELKAMOK_PN );
	acINP_mode = _RETREV;

	acline_init( m_ALine, ACREC_MAX );
	m_List.RemoveAllDtListData();
	m_List.SetCurrentLine(-1);

	// 仕訳選択状態をクリア
	click_seljob( -1, 0 );

	return 0;
}


//---------------------------------------------
//	工事番号 科目チェック
//
//---------------------------------------------
BOOL CDBAbookView::isKojiKmkCheck()
{
	int st;
	BOOL bRet = FALSE;
	char buf[64];

	if( ! M_KOJI )	return FALSE;

	if( !modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
		return FALSE;

	// 修正時は、_dline_chg で行う
	if( acINP_mode != _APPEND ) {
		return FALSE;
	}

	if( pAUTOSEL->KJIKMK_OPT != OPT_KJIKMK)
		return FALSE;

	PROGRAM_OK = FALSE;

	int dcsw;
	dcsw  =GetRecordDCsgn( &m_CREC );

	struct _DBKNREC* pKn;
	int	kj_sgn;
	CString msg, str;
	DWORD dwCode;

	msg = "《工事番号 科目チェック》\r\n";

	kj_sgn = 0;

	if( ! m_CREC.m_dkno.IsEmpty() ) {
		if( ! KjChk.KojiKmkCheck( m_CREC.m_dbt ) ) {

			dwCode = _atoh( (LPSTR)(LPCTSTR)m_CREC.m_dbt );
			kj_sgn |= 0x01;
		}
	}

	if( ! m_CREC.m_ckno.IsEmpty() ) {
		if( ! KjChk.KojiKmkCheck( m_CREC.m_cre ) ) {

			dwCode = _atoh( (LPSTR)(LPCTSTR)m_CREC.m_cre );
			kj_sgn |= 0x02;
		}
	}

	if( kj_sgn ) {
		if( kj_sgn & 0x01 ) {
			if( ( pKn = pDBzm->DB_PjisToKnrec( m_CREC.m_dbt ) ) == NULL )
				return -1;

			pDBzm->KojiCodeToStr( buf, sizeof buf, m_CREC.m_dkno );
			str.Format( "\r\n科目「%.14s」に、工事番号『%s』は設定できません。", pKn->knnam, buf );
			msg += str;
		}

		if( kj_sgn & 0x02 ) {
			if( ( pKn = pDBzm->DB_PjisToKnrec( m_CREC.m_cre ) ) == NULL )
				return -1;

			pDBzm->KojiCodeToStr( buf, sizeof buf, m_CREC.m_ckno );
			str.Format( "\r\n科目「%.14s」に、工事番号『%s』は設定できません。", pKn->knnam, buf );
			msg += str;
		}

		msg += "\r\n\r\n修正しますか？\r\nいいえの場合、工事番号取消して仕訳を書き込みます。";
	//	st = myICSMessageBox( (LPCSTR)msg, MB_YESNO );
		st = IDNO;

		if( st == IDYES ) {
			set_focus( AC_KOJI_PN );
			bRet = TRUE;
		}
		else {
			if( kj_sgn & 0x01 ) {
				m_CREC.m_dkno.Empty();
			}
			if( kj_sgn & 0x02 ) {
				m_CREC.m_ckno.Empty();
			}
		}
	}
	
	PROGRAM_OK = TRUE;

	return bRet;
}


int CDBAbookView::CheckBaseKojiInput( CString kno )
{
	if( pAUTOSEL->KJIKMK_OPT != OPT_KJIKMK)
		return 0;

	if( kno.IsEmpty() )
		return 0;

	CString code = m_BaseCode;
	
	if( ! KjChk.KojiKmkCheck( code ) )
		return -1;

	return 0;
}

int CDBAbookView::CheckKoji_BaseKamokuInput( struct _KamokuData* data )
{
	if( pAUTOSEL->KJIKMK_OPT != OPT_KJIKMK)
		return 0;

	CString kno;
	kno = m_BaseKoji;

	if( kno.IsEmpty() )
		return 0;

	CString code = data->kd_code;
	
	if( ! KjChk.KojiKmkCheck( code ) )
		return -1;

	return 0;
}

// 仕訳チェック
int CDBAbookView::CheckModifyKojiKmkData( CDBINPDataRec *data, int nowln, int nomsg /*=0*/ )
{
	if( pAUTOSEL->KJIKMK_OPT != OPT_KJIKMK)
		return 0;

	int dcsw = GetRecordDCsgn( data );

	CString code, kno;

	if( dcsw == 0 ) {
		code	= data->m_cre;
		kno		= data->m_ckno;
	}
	else if( dcsw == 1 ) {
		code	= data->m_dbt;
		kno		= data->m_dkno;
	}

	if( kno.IsEmpty() )
		return 0;

	DWORD dwCode;
	CString msg, str;
	char buf[32];
	DBKNREC* pKn;
	int st, ret;

	ret = 0;

	if( ! KjChk.KojiKmkCheck( code ) ) {
		dwCode = _atoh( (LPSTR)(LPCTSTR)code );

		msg = "《工事番号 科目チェック》\r\n";
		if( ( pKn = pDBzm->DB_PjisToKnrec( code ) ) == NULL )
			return -1;

		pDBzm->KojiCodeToStr( buf, sizeof buf, kno );
		str.Format( "\r\n科目「%.14s」に、工事番号『%s』は設定できません。", pKn->knnam, buf );
		msg += str;

		BOOL bOKchg = FALSE;
		if( PROGRAM_OK ) {	
			PROGRAM_OK = FALSE;
			bOKchg = TRUE;
		}

		if( ! nomsg ) {
			msg += "\r\n\r\n修正しますか？\r\nいいえの場合、工事番号を取消して仕訳を書き込みます。";
			st = myICSMessageBox( (LPCSTR)msg, MB_YESNO );
		}
		else {
			st = IDNO;
		}

		if( bOKchg ) {
			PROGRAM_OK = TRUE;
		}

		if( st == IDNO ) {
			if( dcsw == 0 ) {
				data->m_ckno.Empty();
			}
			else if( dcsw == 1 ) {
				data->m_dkno.Empty();
			}
			if( nowln != -1 ) {
				ACBOOK_LINE *dtline;
				dtline = _getLINE_DATA( nowln );
				dtline->drec = *data;
				TerminationDataSet( AC_KOJI_PN, nowln );
			}
		}
		else {
			ret = -1;
		}
	}

	return ret;
}


//---------------------------------------------
//	基本科目によって、工事番号のウィンドウ制御
//
//---------------------------------------------
void CDBAbookView::BaseKojiCtrl()
{
	if( ! M_KOJI )	return;

	if( pAUTOSEL->KJIKMK_OPT != OPT_KJIKMK)
		return;

	if( ! m_BaseKoji.IsEmpty() ) {

		if( ! KjChk.KojiKmkCheck( m_BaseCode ) ) {
			m_BaseKoji.Empty();
			VARIANT var;
			var.pbVal = NULL;
			DBdata_set( this, IDC_ICSDBEDTCTRL3, &var, ICSDBEDT_TYPE_STRING, 0 );

			m_KojiSK.bmn = -1;
			m_KojiSK.kno.Empty();
			m_KojiSK.code.Empty();
			m_KojiSK.brn = -1;
			m_KojiSK.etc.Empty();
			m_KojiSK.etc_col = (DWORD)-1;

			SetKojiEtcDisp( FALSE );
		}
	}
}


//
// 固定項目表示
void CDBAbookView::AutoSelIndicator( int inp_mode )
{

	int sw;		// 0 ... OFF, 1 ... ON, 2 ... スキップ 
	int i;
	COLORREF back, text, deftitle, deftext;
	deftitle	= m_List.GetDtTitleColor();
	deftext		= m_List.GetDtTitleTextColor();

	double src_darken, dest_darken;
	// 自動項目の色はタイトル色より、作成
	src_darken = (double)40 / 100.0;	//薄め
	dest_darken = 1.0 - src_darken;
	back = PixelAlpha(deftitle, src_darken, RGB_WHITE, dest_darken);
	// 文字色は、濃い目
	src_darken = (double)70 / 100.0;
	dest_darken = 1.0 - src_darken;
	text = PixelAlpha(deftitle, src_darken, RGB_BLACK, dest_darken);

	int dID[] = {
		DATA_VAL, DATA_USER1, -1
	};

	for( i = 0 ; dID[i] != -1; ++i )
	{
		sw = 0;

		if( inp_mode == _APPEND ) {
			switch( dID[i] ) {
				case DATA_VAL:
					if( (m_dwOpt&0xf0) == 0x10 ) {
						sw = 1;
					}
					break;
				case DATA_USER1:
					if( (m_dwOpt&0xf0) == 0x20 ) {
						sw = 1;
					}
					break;
			}
		}
		m_List.SetTitleClr( dID[i], sw ? text : deftext, sw ? back : deftitle );
	}

	m_List.RedrawDataListCtrl();
}

//固定処理
void CDBAbookView::OnButtonF5()
{
	if( ! PROGRAM_OK || bCONFIRM_MASTER )
	{
		return;
	}

	if( acINP_mode == _APPEND ) {
		int pn = get_nowpn();

		if( pn == AC_NKIN_PN || pn == AC_SKIN_PN ) {
			switch( pn ) {
			case AC_NKIN_PN:
				if( (m_dwOpt&0xf0) == 0x10 ) {
					m_dwOpt &= ~0xf0;
				}
				else {
					m_dwOpt &= ~0xf0;
					m_dwOpt |= 0x10;
				}
				break;
			case AC_SKIN_PN:
				if( (m_dwOpt&0xf0) == 0x20 ) {
					m_dwOpt &= ~0xf0;
				}
				else {
					m_dwOpt &= ~0xf0;
					m_dwOpt |= 0x20;
				}
				break;
			}
			SetAcbkOptionVal();
			AutoSelIndicator( _APPEND );
		}
	}
}


void CDBAbookView::OnUpdateButtonF5(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if( ! PROGRAM_OK || bCONFIRM_MASTER )
	{
		pCmdUI->Enable( FALSE );
		return;
	}
	if( acINP_mode == _APPEND ) {
		switch( get_nowpn() ) {
		case AC_NKIN_PN:
		case AC_SKIN_PN:
			pCmdUI->Enable( TRUE );
			return;
		}
	}

	pCmdUI->Enable( FALSE );
}


// 出納帳 オプション
void CDBAbookView::SetAcbkOptionVal()
{
	int opt;
	opt = m_dwQuery;
	if( m_dwOpt & 0x01 )	opt |= 0x100;

	// 入金・出金固定
	int tmpOpt;
	tmpOpt = (m_dwOpt & 0xf0) << 8;
	opt |= (tmpOpt&0xf000);

	((CMainFrame*)GetDBDinpMainFrame())->AcbkOption() = opt;
}


void CDBAbookView::OnButtonF4()
{
	CalcWindowDelete();

	CACInpSupDlg	dlg(this);
	int ret;

	//固定
	dlg.m_ValFix = ((m_dwOpt & 0xf0) >> 4);

PROGRAM_OK = FALSE;

	ret = dlg.DoModal();

PROGRAM_OK = TRUE;

	if( ret == IDOK ) {
		//固定
		m_dwOpt &= ~0xf0;
		m_dwOpt |= (dlg.m_ValFix<<4);

		SetAcbkOptionVal();
		AutoSelIndicator( acINP_mode );

		// 金額 Enter 複写
		theApp.SetOptionInt( OPT_SECTION, OPT_KEYVALCPY, ValEnterCopyMode() );

		StatusBarValGuide();
	}
}


void CDBAbookView::StatusBarValGuide()
{
	if( acINP_mode == _APPEND ) {
		if( ValEnterCopyMode() ) {	//金額複写 なし
			((CMainFrame*)GetDBDinpMainFrame())->StatusGuideDisp(IDS_ACVAL_GUIDE2);
		}
		else {						//金額複写 する
			((CMainFrame*)GetDBDinpMainFrame())->StatusGuideDisp(IDS_ACVAL_GUIDE1);
		}
	}
	else {
		((CMainFrame*)GetDBDinpMainFrame())->StatusGuideDisp(AFX_IDS_IDLEMESSAGE);
	}
}


// 差引残高 の計算を行うか？
BOOL CDBAbookView::IsSzanCalq( CDBINPDataRec* pcrec )
{
	BOOL ret = TRUE;

	int chk = 0;

	// 基本科目の指定で、工事・部門・枝番を検索条件として指定しない。
	if( M_KOJI ) {		//工事マスター
		if( BMON_MST && BRmst ) {
			if( ! (m_dwQuery & ACQ_KOJI) && ! (m_dwQuery & ACQ_BUMON) && ! (m_dwQuery & ACQ_EDA) )	chk++;
		}
		else if( BMON_MST ) {
			if( ! (m_dwQuery & ACQ_KOJI) && ! (m_dwQuery & ACQ_BUMON) )	chk++;
		}
		else {
			if( ! (m_dwQuery & ACQ_KOJI) && ! (m_dwQuery & ACQ_EDA) )	chk++;
		}
	}
	else if( BMON_MST ) {	//部門マスター
		if( BRmst ) {
			if( ! (m_dwQuery & ACQ_BUMON) && ! (m_dwQuery & ACQ_EDA) )	chk++;
		}
		else {
			if( ! (m_dwQuery & ACQ_BUMON) )	chk++;
		}
	}
	else if( BRmst ) {
		//枝番マスターで 枝番指定なし
		if( ! (m_dwQuery & ACQ_EDA) )	chk++;
	}

	// 基本科目の開始残高が科目残高になる。
	if( chk ) {
		// 借／貸 同一科目の場合は、振替として計算しない。
		if( pcrec->m_dbt.CompareNoCase( pcrec->m_cre ) == 0 ) {
			ret = FALSE;
		}
	}
	return ret;
}


BOOL CDBAbookView::IsSameDbtCre( CDBINPDataRec* pcrec )
{
	BOOL ret = FALSE;

	if( pcrec->m_dbt.CompareNoCase( pcrec->m_cre ) == 0 ) {
		ret = TRUE;
	}

	return ret;
}



LRESULT CDBAbookView::OnSelGuideMsg(WPARAM w, LPARAM l )
{
	if( w == VK_NEXT || w == VK_PRIOR ) {
		keybd_event( w, 0, 0, 0 );
		keybd_event( w, 0, KEYEVENTF_KEYUP, 0 );
	}

	return 1;
}


// 06.27 /13
int CDBAbookView::GetACListCount()
{
	int cnt = m_List.GetDispDataCount();

	if( cnt > ACREC_MAX )
		cnt = ACREC_MAX;

	return cnt;
}


//
BOOL CDBAbookView::BmnIMEkeydownJob( long nChar )
{
	HIMC	hImc;
	char	buf[128] = {0};
	char	yomi[128] = {0};

	int pn = get_nowpn();
	
	if( pn != AC_BMON_PN )
		return FALSE;

	UINT vKey = 0;

	if( nChar == VK_PROCESSKEY ) {
		vKey = ImmGetVirtualKey( m_Input.m_hWnd );
		hImc = ImmGetContext( m_Input.m_hWnd );

		ImmGetCompositionString( hImc, GCS_COMPSTR, buf, sizeof buf );
		ImmGetCompositionString( hImc, GCS_COMPREADSTR, yomi, sizeof yomi );

		if( buf[0] && yomi[0] ) {
			if( isdigitstr( (char*)(LPCTSTR)buf ) ) {
				if( vKey == VK_UP || vKey == VK_DOWN ) {
					if( CheckImeEndBumon( AC_BMON_PN, vKey, buf ) == 0 ) {
						ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
						ImmReleaseContext(m_Input.m_hWnd, hImc);
						hImc = NULL;

						TerminationBumon ( get_nowln(), vKey, 1, 0 );
					}
					return TRUE;
				}
			}
		}
		if( hImc != NULL ) {
			ImmReleaseContext(m_Input.m_hWnd, hImc);
		}
	}

	return FALSE;
}



// 仕訳行 選択処理(Shift)
void CDBAbookView::sel_shiftjob( int ln, long nChar, BOOL bPress )
{
	// SHIFT 押下し
	if( bPress ) {
		int seq, i;
		BOOL bAdd = FALSE;

		if( m_ALine[ln-1].abs_pos > 0 ) {

			seq = m_ALine[ln-1].drec.m_seq;
		
			if( seq > 0 ) {

				m_SelType = SEL_SHIFT;

				if( m_SelSeq == 0 ) {
					m_SelSeq = seq;

					for( i = 0; i < m_SelArray.GetCount(); i++ ) {
						if( m_SelArray[i] == seq ) {
							bAdd = TRUE;
							break;
						}
					}
					if( ! bAdd )	m_SelArray.Add( seq );

					m_SelDirection = (nChar==VK_UP) ? SELCPY_UP : SELCPY_DOWN;
				}
				else {
					if( m_SelDirection == SELCPY_UP ) {
						if( (nChar == VK_UP) )  {
							for( i = 0; i < m_SelArray.GetCount(); i++ ) {
								if( m_SelArray[i] == seq ) {
									bAdd = TRUE;
									break;
								}
							}
							if( ! bAdd )	m_SelArray.Add( seq );
						}
						else {
							// 選択方向が変わったので、該当の仕訳を削除する。
							if( m_SelSeq == seq ) {
								m_SelDirection = SELCPY_DOWN;
							}
							else {
								for( i = 0; i < m_SelArray.GetCount(); i++ ) {
									if( m_SelArray[i] == seq ) {
										m_SelArray.RemoveAt( i );
										break;
									}
								}
							}
						}
					}
					else {
						if( (nChar == VK_DOWN) )  {
							for( i = 0; i < m_SelArray.GetCount(); i++ ) {
								if( m_SelArray[i] == seq ) {
									bAdd = TRUE;
									break;
								}
							}
							if( ! bAdd )	m_SelArray.Add( seq );
						}
						else {
							// 選択方向が変わったので、該当の仕訳を削除する。
							if( m_SelSeq == seq ) {
								m_SelDirection = SELCPY_UP;
							}
							else {
								for( i = 0; i < m_SelArray.GetCount(); i++ ) {
									if( m_SelArray[i] == seq ) {
										m_SelArray.RemoveAt( i );
										break;
									}
								}
							}
						}
					}
				}

				m_SelUpdate = TRUE;
			}
		}
	}
	else {
		// SHIFT 離した。
		if( m_SelDirection != SELCPY_NONE ) {
			m_SelArray.RemoveAll();
			m_SelDirection	= SELCPY_NONE;
			m_SelType		= SEL_NONE;
			m_SelSeq = 0;

			m_SelUpdate = TRUE;
		}
	}
}


/***********************************************
	int line	クリックして変更したライン


 ***********************************************/
void CDBAbookView::click_seljob( int line, long nChar )
{
	int i, n, sgn, lnidx;

	int curidx = get_nowln() - 1;
	int curseq = 0;
	BOOL curAdd = FALSE;

//	if( ! p_DBDinpView->m_bRGDTA_BUTTON_DISP ) {
//		return;
//	}

	if( (nChar == VK_SHIFT || nChar == VK_CONTROL) && curidx >= 0 ) {

		//現在行も選択範囲に入れる。
		if( curidx >= 0 ) {
			if( m_ALine[curidx].abs_pos > 0 ) {
				sgn = 0;

				if( m_SelSeq == 0 ) {
					m_SelSeq = m_ALine[curidx].drec.m_seq;
				}

				curseq = m_ALine[curidx].drec.m_seq;

				for( n = 0; n < m_SelArray.GetCount(); n++ ) {
					if( m_SelArray[n] == curseq ) {
						sgn = 1;
						break;
					}
				}

				if( ! sgn ) {
					m_SelArray.Add( curseq );
					curAdd = TRUE;
				}

				m_SelUpdate = TRUE;
				m_SelDirection = SELCPY_UP;
			}
		}

		// 基本となる仕訳SEQの位置が画面上にあるか？
		int baseln = -1;
		int seljob = 0;
		if( m_SelSeq != 0 ) {
			for( i = 0; i < GetACListCount(); i++ ) {
				if( m_ALine[i].abs_pos > 0 ) {
					if( m_SelSeq == m_ALine[i].drec.m_seq ) {
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
						if( m_ALine[i].abs_pos > 0 ) {

							for( n = 0; n < m_SelArray.GetCount(); n++ ) {
								if( m_SelArray[n] == m_ALine[i].drec.m_seq ) {
									sgn = n;
									break;
								}
							}
							if( sgn != -1 ) {
								m_SelArray.RemoveAt( n );
							}
						}
					}

					for( i = lnidx; i < baseln; i++ ){
						sgn = -1;
						if( m_ALine[i].abs_pos > 0 ) {

							for( n = 0; n < m_SelArray.GetCount(); n++ ) {
								if( m_SelArray[n] == m_ALine[i].drec.m_seq ) {
									sgn = n;
									break;
								}
							}
							if( sgn == -1 ) {
								m_SelArray.Add( m_ALine[i].drec.m_seq );
							}
						}
					}

					m_SelDirection = SELCPY_UP;
					seljob = 1;
				}
				else if( baseln > (curidx+1) && line >= baseln ) {
					// 選択開始行 を またいで クリックされたとき( 上から下 )
					for( i = curidx; i < baseln; i++ ){
						sgn = -1;
						if( m_ALine[i].abs_pos > 0 ) {

							for( n = 0; n < m_SelArray.GetCount(); n++ ) {
								if( m_SelArray[n] == m_ALine[i].drec.m_seq ) {
									sgn = n;
									break;
								}
							}
							if( sgn != -1 ) {
								m_SelArray.RemoveAt( n );
							}
						}
					}

					for( i = (baseln-1); i < line; i++ ){
						sgn = -1;
						if( m_ALine[i].abs_pos > 0  ) {

							for( n = 0; n < m_SelArray.GetCount(); n++ ) {
								if( m_SelArray[n] == m_ALine[i].drec.m_seq ) {
									sgn = n;
									break;
								}
							}
							if( sgn == -1 ) {
								m_SelArray.Add( m_ALine[i].drec.m_seq );
							}
						}
					}

					m_SelDirection = SELCPY_DOWN;
					seljob = 1;
				}
			}

			if( ! seljob ) {
				if( m_ALine[lnidx].abs_pos > 0 ) {
					int start, end, ln_sel = 0;
					if( nChar == VK_SHIFT ) {
						// SHIFT
						m_SelType	= SEL_SHIFT;

						// 移動先の選択状態をチェック
						for( n = 0; n < m_SelArray.GetCount(); n++ ) {
							if( m_SelArray[n] ==  m_ALine[lnidx].drec.m_seq ) {
								ln_sel = 1;
								break;
							}
						}

						if( lnidx < curidx ) {
							start	= lnidx;
							end		= curidx;

							m_SelDirection = SELCPY_UP;
						}
						else {
							start	= curidx;
							end		= lnidx;

							m_SelDirection = SELCPY_DOWN;
						}

						for( i = start; i <= end; i++ ){
							sgn = -1;
							if( m_ALine[i].abs_pos > 0 ) {

								if( curAdd ) {
									if( curseq == m_ALine[i].drec.m_seq )
										continue;
								}
								else {
									// 移動先が未選択のときは、現行の選択をはずさない。
									if( ! ln_sel ) {
										if( curseq == m_ALine[i].drec.m_seq )
											continue;
									}
								}

								for( n = 0; n < m_SelArray.GetCount(); n++ ) {
									if( m_SelArray[n] == m_ALine[i].drec.m_seq ) {
										sgn = n;
										break;
									}
								}
								if( sgn == -1 ) {
									m_SelArray.Add( m_ALine[i].drec.m_seq );
								}
								else {
									m_SelArray.RemoveAt( n );
								}
							}
						}
					}
					else {
						// CTRL キー
						m_SelType	= SEL_CTRL;
						sgn = -1;

						for( n = 0; n < m_SelArray.GetCount(); n++ ) {
							if( m_SelArray[n] == m_ALine[lnidx].drec.m_seq ) {
								sgn = n;
								break;
							}
						}

						if( sgn == -1 ) {
							m_SelArray.Add( m_ALine[lnidx].drec.m_seq );
						}
					//	else {
					//		m_SelArray.RemoveAt( n );
					//	}
					}
					if( lnidx < curidx ) {
						m_SelDirection = SELCPY_UP;
					}
					else {
						m_SelDirection = SELCPY_DOWN;
					}

					m_SelUpdate = TRUE;
				}
			}
		}
	}
	else {
		// SHIFT, CTRL 離した。
		if( m_SelDirection != SELCPY_NONE ) {
			m_SelArray.RemoveAll();
			m_SelDirection	= SELCPY_NONE;
			m_SelType		= SEL_NONE;
			m_SelSeq = 0;

			m_SelUpdate = TRUE;
		}
	}

}


// 選択ライン再描画
void CDBAbookView::sel_lineupdate( BOOL bDoUpdate/*=FALSE*/)
{
	if( m_SelUpdate || bDoUpdate ) {
		int n, sgn;
		int curidx = get_nowln() - 1;

		//現在行も選択範囲に入れる。
		if( m_SelDirection != SELCPY_NONE ) {
			if( curidx >= 0 ) {
				if( m_ALine[curidx].abs_pos > 0 ) {
					sgn = 0;

					for( n = 0; n < m_SelArray.GetCount(); n++ ) {
						if( m_SelArray[n] == m_ALine[curidx].drec.m_seq ) {
							sgn = 1;
							break;
						}
					}

					if( ! sgn ) {
						m_SelArray.Add( m_ALine[curidx].drec.m_seq );
					}
				}
			}
		}

		//画面表示
		int old_cline;
		old_cline = get_nowln();

		ListDataSet();

		m_SelUpdate = FALSE;
	}
}


void CDBAbookView::RButtonMenuJob( UINT nID, CPoint point )
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_ACBMENU));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	UINT flag;

	if( m_bRegSwk )	flag = MF_ENABLED;
	else			flag = MF_GRAYED;
	pPopup->EnableMenuItem( IDM_REGDATA, MF_BYCOMMAND| flag );

	if( m_bRegDlg )	flag = MF_ENABLED;
	else			flag = MF_GRAYED;
	pPopup->EnableMenuItem( IDM_REGDLGUSE, MF_BYCOMMAND| flag );

	DWORD dwRetCode = pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
											  point.x, point.y, this);
	
	if( dwRetCode == IDM_REGDATA ) {
		OnRegdata();
	}
	else if( dwRetCode == IDM_REGDLGUSE ) {
		OnRegdlguse();
	}

}



BOOL CDBAbookView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	m_bRegSwk	= FALSE;
	m_bRegDlg	= FALSE;

	NMHDR* pnm;
	pnm = (NMHDR*)lParam;

//MyTrace( "CDBAbookView::OnNotify(TOP) wParam = %d, hwndFrom = %08x, idFrom = %d, code = %d\n", wParam, pnm->hwndFrom, pnm->idFrom, pnm->code );

	if( p_DBDinpView->m_bRGDTA_BUTTON_DISP ) {
		static NM_DLCTMOUSE nmhbf;

		if( wParam == IDC_ACBOOKLIST ) {
			NM_DLCTMOUSE* nmh;
			nmh = (NM_DLCTMOUSE*)lParam;
			int nowln = get_nowln();

			if( nmh->hdr.code == NM_RCLICK ) {

				if( nmh->iLine != nowln ) {
				//	_OnTerminationIcsdbedtctrl(0,0,0);
					set_nowpn(-1);
					_set_focus( nmh->iLine, AC_DATE_PN );
				}

				if( acINP_mode != _APPEND ) {
					if( !bCONFIRM_MASTER ) {
						m_bRegSwk = TRUE;
					}
					m_bRegDlg	= FALSE;

					if( nmh->iLine == nowln ) {
						RButtonMenuJob( IDC_ACBOOKLIST, nmh->point );
					}
					else {
						if( nmh->iLine != nowln ) {
							_set_focus( nmh->iLine, AC_DATE_PN );
						}
						// 選択仕訳行の範囲に入っているか？
						if( ! CheckCopySeq( nmh->iLine ) ) {
							click_seljob( nmh->iLine, 0 );
							sel_lineupdate();
						}

						nmhbf = *nmh;
						PostMessage( WM_DBDINPMSG, DWP_DISPLINE );
						PostMessage( WM_DBDINPMSG, DWP_COPYMENU, (LPARAM)&nmhbf );

					}
				}
				else {
					BOOL bRclick = TRUE;
					CString strType;
					if( RemainSwkdataCheck(	strType) ) {
						CString msg;
						msg.Format("%s！右クリック処理はできません。",strType );
						myICSMessageBox( msg );
						bRclick = FALSE;
					}
					else if( m_ALine[nowln-1].abs_pos == 0 ) {
						//定型仕訳を使用する場合は、入力仕訳をチェックする
						if( nmh->iLine == nowln ) {
							if( AppendDataCheck(nowln) ) {
								PROGRAM_OK = FALSE;
								
								int st = myICSMessageBox( swk_kansei, MB_YESNO|MB_DEFBUTTON2 );
							
								PROGRAM_OK = TRUE;

								if( st == IDYES ) {
									_set_focus( nowln, AC_DATE_PN );
									bRclick = FALSE;
								}
							}
						}
					}

					if( bRclick ) {
						if( nmh->iLine != nowln ) {
							set_nowpn(-1);
							set_nowln(-1);
							_set_focus( nmh->iLine, AC_DATE_PN );
						}

						// 選択クリア
						click_seljob( nmh->iLine, 0 );

						nmhbf = *nmh;

						PostMessage( WM_DBDINPMSG, DWP_COPYMENU, (LPARAM)&nmhbf );
					}
				}
			}
 		}
	}

//MyTrace( "CDBAbookView::OnNotify END" );

	return CDBbaseView::OnNotify(wParam, lParam, pResult);
}



// 現在行が選択仕訳の中に入っているか？
BOOL CDBAbookView::CheckCopySeq( int line )
{
	int curidx, curseq, n;
	BOOL bRet = FALSE;
	curidx = line - 1;

	if( curidx >= 0 ) {
		if( m_ALine[curidx].abs_pos > 0 ) {
			curseq = m_ALine[curidx].drec.m_seq;

			for( n = 0; n < m_SelArray.GetCount(); n++ ) {
				if( m_SelArray[n] == curseq ) {
					bRet = TRUE;
					break;
				}
			}
		}
	}

	return bRet;
}


LRESULT CDBAbookView::OnDBDinpMsg( WPARAM wp, LPARAM lp )
{
	if( wp == DWP_COPYMENU ) {
		// いきなり修正行の番号欄を右クリック
		NM_DLCTMOUSE* nmh;
		nmh = (NM_DLCTMOUSE*)lp;

		if( acINP_mode != _APPEND ) {
			if( !bCONFIRM_MASTER ) {
				m_bRegSwk = TRUE;
			}
			else {
				m_bRegSwk = FALSE;
			}
			m_bRegDlg = FALSE;
		}
		else {
			m_bRegSwk = FALSE;
			m_bRegDlg = TRUE;
		}

		RButtonMenuJob(nmh->hdr.idFrom, nmh->point);

	}
	else if( wp == DWP_DISPLINE ) {
		sel_lineupdate( TRUE );
	}
	else if( wp == DWP_SETFOCUS ) {
		NM_DLCTMOUSE* nmh;
		nmh = (NM_DLCTMOUSE*)lp;
		_set_focus( nmh->iLine, AC_DATE_PN );
	}
	else if( wp == DWP_TKYSWKDLG ) {
		int attr, tkjourseq, st;
		ICS_ACBOOK_PAC	acpac;
		acpac.bmn = m_BaseBmn;
		acpac.kno = m_BaseKoji;
		acpac.kmk = m_BaseCode;
		acpac.eda = m_BaseEda;

		attr = 0x13;

		if( lp > 1 ) {
			st = pTkjour->SelectTkjourDlg_Acbook(m_autoTKrec.m_tkcod, attr, tkjourseq, acpac,this);
		}
		else {
			st = pTkjour->SelectTkjour_Acbook(m_autoTKrec.m_tkcod, attr, tkjourseq, acpac );
		}
		m_TKsyz2 = 0;	// 前行分消費税コードクリア
		AutoSwkSetJob(&m_CREC, tkjourseq);

		int dc_sw = tky_select_to_drec_sub(&m_CREC, &m_autoTKrec);
		//表示
		tky_select_dsp(&m_CREC, dc_sw, &m_autoTKrec, TRUE, TRUE);
	}
	return 1;
}

// 選択仕訳ソート
int __cdecl AcbkRegCompare(void *context, const void *ele1, const void *ele2 )
{
	int ret;

	DWORD* dw1, *dw2;

	dw1 = (DWORD*)ele1;
	dw2 = (DWORD*)ele2;

	CDBAbookView* pvw;
	pvw = (CDBAbookView*)context;

	ret = pvw->reg_sort( (int)*dw1, (int)*dw2 ); 

	return ret;
}


int CDBAbookView::reg_sort( int seq1, int seq2 )
{
	int idx1, idx2, ret;
	idx1 = idx2 = -1;


	if( seq1 >= 1 ) {
		idx1 = SCMDFY_SEQ.ngetseq( seq1-1 );
	}
	
	if( seq2 >= 1 ) {
		idx2 = SCMDFY_SEQ.ngetseq( seq2-1 );
	}
/*
	for( int i = 0; i < ACREC_MAX; i++ ) {
		if( m_ALine[i].drec.m_seq == seq1 ) {
			idx1 = i;
		}
		else if( m_ALine[i].drec.m_seq == seq2 ) {
			idx2 = i;
		}
	}
*/
	if( idx1 == idx2 )		ret = 0;
	else if( idx1 < idx2 )	ret = -1;
	else					ret = 1;

	return ret;
}


void CDBAbookView::Rdadd_job()
{
	// 追加数を増やす
	INT_PTR max;
	max = m_SelArray.GetCount();
	CRdregDlg dlg( this );
	dlg.m_Type = 1;

	int cline;
	cline = get_nowln();

	p_DBDinpView->Rddata_Check();

	if( max >= 1 ) {
		if( max > RDADD_MAX ) {
			RegDataTorokMsg();

			if( m_ALine[cline-1].abs_pos > 0 ) {
				int i, cnt, curidx = -1;
				for( i = cnt = 0; i < max; i++ ) {
					if( m_SelArray[i] == m_ALine[cline-1].drec.m_seq ) {
						curidx = i;
						break;
					}
				}

				if( curidx < 0 )	return;

				CDWordArray	da;
				da.Add( m_SelArray[i] );
				cnt++;

				if( i > RDADD_MAX ) {
					i--;
					for( ; i >= 0; i-- ) {
						da.Add( m_SelArray[i] );
						cnt++;
						if( cnt == RDADD_MAX )	break;
					}

					if( cnt < RDADD_MAX ) {
						i = curidx + 1;
						for( ; i < max; i++ ) {
							da.Add( m_SelArray[i] );
							cnt++;
							if( cnt == RDADD_MAX )	break;
						}
					}
				}
				else {
					i++;
					for( ; i < max; i++ ) {
						da.Add( m_SelArray[i] );
						cnt++;
						if( cnt == RDADD_MAX )	break;
					}

					if( cnt < RDADD_MAX ) {
						i = curidx - 1;
						for( ; i >= 0; i-- ) {
							da.Add( m_SelArray[i] );
							cnt++;
							if( cnt == RDADD_MAX )	break;
						}
					}
				}
				m_SelArray.RemoveAll();
				m_SelArray.Copy( da );
			}
			qsort_s( &m_SelArray[0], m_SelArray.GetCount(), sizeof DWORD, AcbkRegCompare, this );

			max = RDADD_MAX;
			m_SelUpdate = TRUE;
			sel_lineupdate();
		}
		else if( max > 1 ) {
			qsort_s( &m_SelArray[0], m_SelArray.GetCount(), sizeof DWORD, AcbkRegCompare, this );
		}

		for( int i = 0; i < max; i++ ) {
			pDBzm->DBDATA_SetPosition( m_SelArray[i] );
			pDBzm->GetCDBData( &dlg.m_RegData[i] );
		}
		dlg.m_recCnt	= max;
		dlg.m_CmnId		= p_DBDinpView->m_CmnId;
		dlg.DoModal();
	}
	else {
		if( m_ALine[cline-1].abs_pos > 0 ) {
			dlg.m_RegData[0] = m_ALine[cline-1].drec;
			dlg.m_recCnt	= 1;
			dlg.m_CmnId		= p_DBDinpView->m_CmnId;
			dlg.DoModal();
		}
	}
}

void CDBAbookView::OnRegdata()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	Rdadd_job();
}


void CDBAbookView::OnRegdlguse()
{
	// 定型仕訳処理チェック
	CString strType;
	if( RemainSwkdataCheck( strType) ) {
		int	st;
		CString msg;
		msg.Format(_T("%s。\n\n処理を終わりますか？"), strType );

		st	=	myICSMessageBox( msg, MB_YESNO|MB_DEFBUTTON2 );

		if( st == IDYES ) {
			EndRemaindata();	// 定型仕訳テーブルの削除
		}
		else {
			return;
		}
	}

	p_DBDinpView->Rddata_Check();

	PostMessage( WM_REGDATA, defRDT_DIALOG_ON );
}

// 定型仕訳選択用 文字列作成
void CDBAbookView::GetACRdselBaseStr( CString& bmn, CString& koji, CString& kmk, CString& brn)
{
	DBKNREC* pKn;
	LPSTR	p;
	int		bfSize = 64;

	bmn.Empty();
	if( BMON_MST ) {
		if( m_BaseBmn != -1 ) {
			p = bmn.GetBuffer( bfSize );
			pDBzm->BumonCodeToStr( p, bfSize, m_BaseBmn );
			bmn.ReleaseBuffer(-1);
		}
	}

	koji.Empty();
	if( M_KOJI ) {
		if( ! m_BaseKoji.IsEmpty() ) {
			p = koji.GetBuffer( bfSize );
			pDBzm->KojiCodeToStr( p, bfSize, m_BaseKoji );
			koji.ReleaseBuffer(-1);
		}
	}

	pKn = pDBzm->DB_PjisToKnrec( m_BaseCode );
	if( pKn != NULL ) {
		kmk.Format( "%.14s", pKn->knnam );
	}
	else {
		kmk.Empty();
	}

	brn.Empty();
	if( BRmst ) {
		if( m_BaseEda != -1 ) {
			p = brn.GetBuffer( bfSize );
			pDBzm->EdabanToStr( p, bfSize, m_BaseEda );
			brn.ReleaseBuffer(-1);
		}
	}
}


LRESULT CDBAbookView::OnRGDTsel( WPARAM wp, LPARAM lp )
{
	int st;

	if( acINP_mode == _APPEND )
	{
		switch( wp )
		{
		case defRDT_DIALOG_ON:	// ダイアログ表示
			{
				RECT rect;
				m_selfix8.GetWindowRect( &rect );
				this->ScreenToClient( &rect );

				CRect irect, vwRect, selRect;
				GetClientRect( &vwRect );

				int ln = get_nowln();
				int item = DATA_SEQ;
				m_List.GetItemScreenRect( ln, item, irect );
				int adj = (irect.Height() / 2);
				this->ScreenToClient( &irect );

				if( irect.top > (vwRect.Height() / 2) ) {
					//画面上段へ
					selRect.bottom	= irect.top - adj;
					selRect.left	= irect.left;
					selRect.top		= rect.top;
					selRect.right	= irect.right;
				}
				else {
					//画面下段へ
					selRect.left	= irect.left;
					selRect.top		= irect.bottom + adj;
					selRect.bottom	= vwRect.bottom;
					selRect.right	= irect.right;
				}

				ClientToScreen( selRect );

				CRDSel	seldlg( selRect, this);
				seldlg.m_pTBL	= &m_RDTATBL;
				seldlg.m_CmnId	= p_DBDinpView->m_CmnId;
				seldlg.m_LinNum = 1;
				seldlg.m_Type = 1;
				seldlg.m_RDdate = m_CREC.m_ddate;
				seldlg.m_RDmofs = m_CREC.m_mofs;

				int zritu_mode = RDsiwakeZeirituMode();

				st = seldlg.DoModal();

				if( st == IDOK )
					st = 0;
				else
					st = -2;

				RDzeritsuModeWrite( zritu_mode );
			}

			if( st != 0 ) {
				if( st == -1 ) {
					Buzzer();	// Buzzer
				}
			}
			else	// st == 0
				PostMessage( WM_REGDATA, defRDT_GET_RGDTA ); 

			break;

		case defRDT_GET_RGDTA:	// 定型仕訳ゲット
			RdataToInpData( &m_CREC, &m_SREC );
			sprintf_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, "%s", sy_line_txt( &m_CREC ) );
			DataLineUpdate( &m_CREC, get_nowln(), 1 );
			// 定型仕訳残り数表示
			RDRemainDispJOB();
			set_focus( AC_DATE_PN );
			break;
		}
	}

	return 1;
}


// 次の自動仕訳を取得（複合）
//	crec	自動仕訳セットするレコード
//	edainp	自動仕訳で枝番をセットした場合＝1
//
BOOL CDBAbookView::NextAutoSwkData(CDBINPDataRec *crec, int& edainp)
{
	BOOL ret = FALSE;
	edainp = 0;

	//次の自動仕訳有
	if( m_AutoSwkCnt > m_AutoSwkTrn ) {
		ICS_TKJOUR_PAC* pJour = &m_tkjourAry[m_AutoSwkTrn++];

		CString code, kno;
		int eda = -1;
		int bmn = -1;
		int sgn = 0;

		CArith ar(0x16);
		char tkyval[6] = { 0 };
		BOOL tkyval_set = FALSE;

		sgn = GetAutoSwkItem(pJour, code, eda, bmn, kno);

		//自動仕訳で金額セットする？
		if( sgn != -1 ) {
			if( sgn == 0 ) {
				ar.l_input(tkyval, (void*)(LPCTSTR)pJour->dval);
				if( ar.l_test(tkyval) != 0 )	tkyval_set = TRUE;
			}
			else {
				ar.l_input(tkyval, (void*)(LPCTSTR)pJour->cval);
				if( ar.l_test(tkyval) != 0 )	tkyval_set = TRUE;
			}
		}
//MyTrace("NextAuto code = %s, sgn = %d, dval = %s, cval = %s, tkyval_set(%d)\n", code, sgn, pJour->dval, pJour->cval, tkyval_set);

		BOOL bValidCode = FALSE;
		if( !code.IsEmpty() ) {
			DBKNREC* pKn;
			pKn = pDBzm->DB_PjisToKnrec(code);
			if( pKn != NULL ) {
				bValidCode = TRUE;
			}
		}
		if( bValidCode ) {
			if( sgn != -1 ) {

				CDBINPDataRec tmprec;
				tmprec.Reset();
				// 消費税データ作成
				tmprec.m_seq = pDBzm->m_lastdata.m_seq;
				tmprec.m_seq++;
				tmprec.m_ddate = crec->m_ddate;
				tmprec.m_mofs = crec->m_mofs;
				tmprec.m_ksign = crec->m_ksign;
				// 一旦 科目なし+SEQ 変更で で消費税 イニシャライズ
				DB_SyIniz(&tmprec);

				if( eda != -1 ) {
					edainp = 1;
				}

				SetAutoSwkItemToRecord(sgn, &tmprec, code, eda, bmn, kno);

				if( !pJour->tekiyo.IsEmpty() ) {
					tmprec.m_tekiyo = pJour->tekiyo;
				}
				if( tkyval_set ) {
					memcpy( tmprec.m_val, tkyval, 6);
				}
				DB_SyIniz(&tmprec);

				pDBzm->GetCDBData(&tmprec);

				*crec = tmprec;
				ret = TRUE;
			}
		}
	}
	else {
		EndRemainAutodata();
	}

	return ret;
}


BOOL CDBAbookView::RdataToInpData( CDBINPDataRec *crec, CDBINPDataRec *srec )
{
	BOOL sg;
	CDBINPDataRec rdrec;
	BOOL cnd[10] = {FALSE};

	// 定型仕訳１行読取り
	if( sg = GetRdata( &rdrec, crec, srec, cnd ) )
	{
		// 消費税データ作成
		rdrec.m_seq = pDBzm->m_lastdata.m_seq;

		// 一旦 科目なし+SEQ 変更で で消費税 イニシャライズ
		crec->Reset();
		crec->m_seq		= rdrec.m_seq+1;
		crec->m_ddate	= rdrec.m_ddate;
		crec->m_mofs	= rdrec.m_mofs;
		crec->m_ksign	= rdrec.m_ksign;
		DB_SyIniz( crec );

		int	  syzCnt = 0;
		DWORD syCode[10] = {0};

		syzCnt = rd_reccheck( crec, &rdrec, syCode );

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

		pDBzm->GetCDBData( crec );

		return TRUE;
	}

	return FALSE;
}


void CDBAbookView::ForkActiveJob()
{
	int focus = 0;

	if( m_pRDremainDlg == NULL ) {
		if( (acINP_mode == _APPEND) && (m_RDTATBL.RD_trn > 0 )) {
			m_List.SetFocus();
			focus = 1;
		}
	}
//MyTrace( "ForkActiveJob focus = %d\n", focus );

	RDRemainDispJOB();

//	if( focus ) {
		set_focus( get_nowpn() );
//	}
}

void CDBAbookView::RDRemainDispJOB( void )
{
	BOOL sw = FALSE;
	if( (acINP_mode == _APPEND) ) {
		if( m_bAutoSwk ) {
			sw = (m_AutoSwkTrn > 0) ? TRUE : FALSE;
		}
		if( ! sw  ) {
			sw = (m_RDTATBL.RD_trn > 0) ? TRUE : FALSE;
		}
	}
	RDRemainDisp( sw );
}


void CDBAbookView::RDRemainDisp( BOOL sw )
{
	UINT nFlags = SWP_NOACTIVATE;

	if( sw )	nFlags |= SWP_SHOWWINDOW;
	else		nFlags |= SWP_HIDEWINDOW;

	int x		= 0, y = 0;
	int height	= 0, width = 0;

	if( m_pRDremainDlg == NULL ) {
		if( sw )	MakeRemainDlg();
		else		return;
	}

	// m_pRDremainDlg は、リソース上で 無効(disalbed)で作成している。
	// Create直後に アクティブになってフォーカスが失われるため
	if( sw ) {
		CRect rect, dspRect, rct;
//		m_pRDremainDlg->EnableWindow(TRUE);

		m_List.GetItemScreenRect( 0, DATA_SEQ, rect );
		dspRect = rect;
		ScreenToClient(dspRect);
		x = dspRect.left;
		y = dspRect.top;

//		y -= dspRect.Height();

		m_List.GetItemScreenRect( 0, DATA_DATE, rect );
		ScreenToClient(rect);
		height	= rect.bottom - y;
		width	= rect.right - x;

		int num = -1;
		CString str;

		if( m_bAutoSwk ) {
			if( m_AutoSwkTrn > 0 ) {
				num = m_AutoSwkCnt - m_AutoSwkTrn;
				str.Format(_T("自動残%3d"), num);
			}
		}
		if( num ==-1 ) {
			num = m_RDTATBL.RD_cnt - m_RDTATBL.RD_trn;
			str.Format(_T("定型残%3d"), num);
		}

		m_pRDremainDlg->SetRemainString( str, height );
	}
	else {
//		m_pRDremainDlg->EnableWindow(FALSE);
	}

	m_pRDremainDlg->MoveWindow(x, y, width, height);
	m_pRDremainDlg->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, nFlags|SWP_NOMOVE | SWP_NOSIZE);
//	m_pRDremainDlg->SetWindowPos( &CWnd::wndTopMost, x, y, width, height, nFlags );
}


//------------------------------------------------------------------
//    タブで切り替えられたときの直前のビューの後処理の関数
//
//------------------------------------------------------------------
int CDBAbookView::TabClickChangedPrevView()
{
	EndRemaindata();
	RDRemainDisp( FALSE );
	m_Input.ShowWindow(SW_HIDE);
	//摘要消費税情報クリア
	m_TKattrec.Reset();
	m_TKsyz = 0;
	m_TKsyz2 = 0;

	return 0;
}


// 仕訳行 伝票番号 フォーカス
//
void CDBAbookView::FocusIcsDenp( int ln, short click )
{
	if( PROGRAM_OK == FALSE )
		return;

	try {
		//別画面入力消去
		Destroy_ModeLessDlg();

		int prev_ln = get_nowln();
		int prev_pn = get_nowpn();

		if( _dline_chg( ln, AC_DENP_PN, click ? 0 : 1  ) == -1 )
			return;

		// 取消データの場合は
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return;
		}
		if( data_denpdialog( ln ) != 0 )
			return;

		set_nowpn( AC_DENP_PN );
	
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}
//
// 仕訳行 伝票番号 キーターミネーション
//
void CDBAbookView::TerminationDenp( int ln, long nChar, long inplen, long kst )
{
	int check = -1, prev = -1;
	int len, denp, pn;
	VARIANT var;

	pn = AC_DENP_PN;

	char	tmp[30] = {0}, bf[30] = {0};

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
		// 直前のデータを保存
		prev = getLINE_DATA()->drec.m_cno;
		DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

		if( var.pbVal != NULL )
			strcpy_s( bf, sizeof bf, (const char*)var.pbVal );
		len = 0;

		len = strlen( (const char*)bf );
		denp = -1;

		if( len > 0 ) {
			denp = atoi( (LPCTSTR)bf );
		}
		check = denp;

		if( ! modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
		{
			if( check != prev )
			{
				Buzzer();
			}

			if( prev != -1 ) {
				sprintf_s( tmp, sizeof tmp, "%d", prev );
				var.pbVal = (BYTE*)tmp;
			}
			else {
				var.pbVal = NULL;
			}
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
		else
		{
			if( getLINE_DATA()->lcnd[pn].INP_sg == FALSE && len <= 0 && !inplen )
			{
				if( getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY == FALSE )
					goto NEXT;
			}

			if( acINP_mode == _APPEND && getLINE_DATA()->lcnd[pn].INP_sg == FALSE )
			{
				if( check == -1 ) {
					check = m_SREC.m_cno;
				}
			}

			m_CREC.m_cno = check;
			getLINE_DATA()->drec.m_cno = check;
			getLINE_DATA()->lcnd[pn].INP_sg = TRUE;

			if( check != -1 ) {
				sprintf_s( tmp, sizeof tmp, "%d", check );
				var.pbVal = (BYTE*)tmp;
			}
			else {
				var.pbVal = NULL;
			}
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

			TerminationDataSet( pn );
		}

NEXT:
		getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY = FALSE;

		if( !nChar ) {
			return;
		}
		if( nChar == -1 ) {
			set_focus( pn );
			return;
		}

		if (nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT))
			nChar = VK_F2;

		if( nChar )
		{

			nChar = dline_chg( nChar );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合は日付
				if( isdeldata() )
				{
					set_focus( AC_DATE_PN );
					return;
				}

				set_focus( pn );
				return;
			}
		}

		if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) && nChar == VK_DELETE )
		{
			check = -1;
			::ZeroMemory( tmp, sizeof tmp );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

			m_SREC.m_cno = check;
			getLINE_DATA()->drec.m_cno = check;
			getLINE_DATA()->lcnd[pn].INP_sg = TRUE;
			TerminationDataSet( pn );	
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT )
		{
			getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY = TRUE;

			if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) ) {
			}

			if( acINP_mode == _APPEND && m_bAutoSwk ) {	// 自動仕訳
				int fcs = 0;

				if( bDOCEVI_Master ) {
					if( ! getLINE_DATA()->lcnd[AC_DOCEVI_PN].INP_sg ) {
						set_termfocus( get_nowln(), AC_DENP_PN, 1 );
						fcs++;
					}
				}
				if( ! fcs ) {
					if( BMON_MST ) {
						if( ! getLINE_DATA()->lcnd[AC_BMON_PN].INP_sg ) {
							int acpn = bDOCEVI_Master ? AC_DOCEVI_PN : AC_DENP_PN;
							set_termfocus( get_nowln(), acpn, 1 );
							fcs++;
						}
					}
				}
				if( ! fcs ) {
					if( M_KOJI ) {
						if( ! getLINE_DATA()->lcnd[AC_KOJI_PN].INP_sg ) {
							set_termfocus( get_nowln(), AC_BMON_PN, 1 );
							fcs++;
						}
					}
				}
				if( ! fcs ) {
					if( ! getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg ) {
						set_focus( AC_KMK_PN );
						fcs++;
					}
				}
				if( ! fcs ) {
					set_termfocus( get_nowln(), AC_TKY_PN, 1 ); //金額へ
				}
			}
			else {
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



// 仕訳行 証憑番号 フォーカス
//
void CDBAbookView::FocusIcsDocevi( int ln, short click )
{
	if( PROGRAM_OK == FALSE )
		return;

	try {
		//別画面入力消去
		Destroy_ModeLessDlg();

		int prev_ln = get_nowln();
		int prev_pn = get_nowpn();

		if( _dline_chg( ln, AC_DOCEVI_PN, click ? 0 : 1  ) == -1 )
			return;

		// 取消データの場合は
		if( isdeldata() )
		{
			set_focus( AC_DATE_PN );
			return;
		}
		if( data_denpdialog( ln ) != 0 )
			return;

		set_nowpn( AC_DOCEVI_PN );
	
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}
//
// 仕訳行 証憑番号 キーターミネーション
//
void CDBAbookView::TerminationDocevi( int ln, long nChar, long inplen, long kst )
{
	CString check, prev;
	int len, pn;
	VARIANT var;

	pn = AC_DOCEVI_PN;

	char	tmp[30] = {0}, bf[30] = {0};

	if( PROGRAM_OK == FALSE )
		return;

	try
	{
		// 直前のデータを保存
		prev = getLINE_DATA()->drec.m_skn;
		DBdata_get( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

		if( var.pbVal != NULL )
			strcpy_s( bf, sizeof bf, (const char*)var.pbVal );
		len = 0;

		len = strlen( (const char*)bf );

		if( len > 0 ) {
			check = bf;
		}
		else {
			check.Empty();
		}

		if( ! modefy_ok( acINP_mode, &getLINE_DATA()->drec ) )
		{
			if( check != prev )
			{
				Buzzer();
			}

			if( ! prev.IsEmpty() ) {
				sprintf_s( tmp, sizeof tmp, "%s", prev );
				var.pbVal = (BYTE*)tmp;
			}
			else {
				var.pbVal = NULL;
			}
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
		else
		{
			if( getLINE_DATA()->lcnd[pn].INP_sg == FALSE && len <= 0 && !inplen  )
			{
				if( getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY == FALSE )
					goto NEXT;
			}

			if( acINP_mode == _APPEND && getLINE_DATA()->lcnd[pn].INP_sg == FALSE )
			{
				if( check.IsEmpty() ) {
					check = m_SREC.m_skn;
				}
			}

			m_CREC.m_skn = check;
			getLINE_DATA()->drec.m_skn = check;
			getLINE_DATA()->lcnd[pn].INP_sg = TRUE;

			sprintf_s( tmp, sizeof tmp, "%s", check );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

			TerminationDataSet( pn );
		}

NEXT:
		getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY = FALSE;

		if( !nChar ) {
			return;
		}
		if( nChar == -1 ) {
			set_focus( pn );
			return;
		}

		if (nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT))
			nChar = VK_F2;

		if( nChar )
		{

			nChar = dline_chg( nChar );
			if( nChar == -1 )
			{
//				MessageBeep( 0xffffffff );
				return;
			}
			else if( !nChar )
			{
				// 取消データの場合は日付
				if( isdeldata() )
				{
					set_focus( AC_DATE_PN );
					return;
				}

				set_focus( pn );
				return;
			}
		}

		if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) && nChar == VK_DELETE )
		{
			check.Empty();
			::ZeroMemory( tmp, sizeof tmp );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

			m_SREC.m_skn = check;
			getLINE_DATA()->drec.m_skn = check;
			getLINE_DATA()->lcnd[pn].INP_sg = TRUE;
			TerminationDataSet( pn );	
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT )
		{
			getLINE_DATA()->lcnd[pn].NEXT_MOVE_KEY = TRUE;

			if( modefy_ok( acINP_mode, &getLINE_DATA()->drec ) ) {
			}

			if( acINP_mode == _APPEND && m_bAutoSwk ) {	// 自動仕訳
				int fcs = 0;

				if( BMON_MST ) {
					if( ! getLINE_DATA()->lcnd[AC_BMON_PN].INP_sg ) {
						set_termfocus( get_nowln(), AC_DOCEVI_PN, 1 );
						fcs++;
					}
				}
				if( ! fcs ) {
					if( M_KOJI ) {
						if( ! getLINE_DATA()->lcnd[AC_KOJI_PN].INP_sg ) {
							set_termfocus( get_nowln(), AC_BMON_PN, 1 );
							fcs++;
						}
					}
				}
				if( ! fcs ) {
					if( ! getLINE_DATA()->lcnd[AC_KMK_PN].INP_sg ) {
						set_focus( AC_KMK_PN );
						fcs++;
					}
				}
				if( ! fcs ) {
					set_termfocus( get_nowln(), AC_TKY_PN, 1 ); //金額へ
				}
			}
			else {
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


// 定型仕訳として表示できるか？
BOOL CDBAbookView::IsACRdData( CDBINPDataRec* rec )
{
	if( rec->m_dbt.IsEmpty() && rec->m_cre.IsEmpty() ) {
		return FALSE;
	}

	if( rec->m_dbt.IsEmpty() ) {
		if( rec->m_cre.CompareNoCase( m_BaseCode ) == 0 )
			return FALSE;
	}
	else if( rec->m_cre.IsEmpty() ) {
		if( rec->m_dbt.CompareNoCase( m_BaseCode ) == 0 )
			return FALSE;
	}
	else {
		if( rec->m_dbt.CompareNoCase( m_BaseCode ) == 0  ||
			rec->m_cre.CompareNoCase( m_BaseCode ) == 0 ) {
			return TRUE;
		}
	}

	return FALSE;
}



// 差引残高用の仕訳データを更新
void CDBAbookView::UpdateACZanRec( int abs_pos, CDBINPDataRec* rec )
{
	int n = abs_pos;
	if( m_SZan[n].ac_szrec.m_seq == rec->m_seq ) {
		m_SZan[n].ac_szrec = *rec;
	}
}


//電卓ウィンドウの消去
BOOL CDBAbookView::CalcWindowDelete()
{
	int pn = get_nowpn();

	BOOL bCalcWndC = FALSE;

	switch( pn ) {
	case AC_NKIN_PN:
	case AC_SKIN_PN:
	case AC_ZEI_PN:
		CWnd* pwnd;
		pwnd = GetFocus();
		CRect rct;
		if( pwnd ) {
			if( m_Input.m_hWnd != pwnd->m_hWnd )
				bCalcWndC = TRUE;

			if( bCalcWndC ) {
				::SendMessage( pwnd->m_hWnd, WM_KEYDOWN, VK_ESCAPE, 0 );
			}
		}
	}

	return bCalcWndC;
}


int CDBAbookView::data_denpdialog( int ln )
{
	// 引数lnに0以下が指定されると、配列に不正アクセスする
	if( ln <= 0 ) {
		return 0;
	}

	if( m_ALine[ln-1].abs_pos <= 0 )
		return 0;

	int icno = m_ALine[ln-1].drec.m_icno;

	if( DenpTypeMode() && IsDenpyoSiwake( &m_ALine[ln-1].drec) ) {

		DENP_INPUTPAR	dnppar;
		dnppar.pZmsub = pDBzm;
		dnppar.pSyohi = pDBsy;

		dnppar.parent = this;
		dnppar.winApp = &theApp;

		int seq = m_ALine[ln-1].drec.m_seq;
		dnppar.seq = seq;
		dnppar.nosyzmdf = 0;
		DenpInputSyzMdfyVectorSet( &dnppar );

		PROGRAM_OK = FALSE;
		set_nowln(-1);

		((CMainFrame*)GetDBDinpMainFrame())->ToolBarClear();

		int st = DenpInputDialog( &dnppar );
		if( st != IDCANCEL ) {
			if( dnppar.addseq.size() ) {
				pDBzm->insseq_make() = FALSE;
			}
			// 修正・追加のサインをセット
			DenpEnd_addmdfcheck( &dnppar );

			int top_zan = 0;
			if (m_ALine[0].lattr == 1)
				top_zan = 1;

			//仕訳を再表示
			RequeryACLine( seq, ln, top_zan );
			m_CREC = m_ALine[ln - 1].drec;
			m_SCAN_rec = m_CREC;

			pDBzm->kzrec_Requery() = TRUE;
		}
		else {
			//伝票形式でクエリが変更されている
			Acbook_Requery();
		}

		PROGRAM_OK = TRUE;
		set_nowln(ln);
		_set_focus( ln, AC_DATE_PN );

		return 1;
	}

	return 0;
}


//伝票形式での修正では、書込みをしないようにする。
int CDBAbookView::IsAbookDiff_Data( CDBINPDataRec *newdata, CDBINPDataRec *moto )
{
	if( DenpTypeMode() && IsDenpyoSiwake( newdata ) )
		return 0;

	return isdiff_data( newdata, moto );
}




BOOL CDBAbookView::ChangeZeiritsu()
{
	BOOL ret = FALSE;

	int pn = get_nowpn();
	if (!(pn == AC_NKIN_PN || pn == AC_SKIN_PN)) {
		return FALSE;
	}

	if (!IsMasterType( MST_SYZ8_10 | MST_SYZ10 )) {
		return TRUE;
	}
	
	int ns_sw;
	if (pn == AC_NKIN_PN)	ns_sw = 0;
	else {
		ns_sw = 1;
	}
	int ln = get_nowln();

	TerminationVal( ln, ns_sw, 0, 0, 0 );
	
	DWORD	dwCode = 0;

	switch( m_CREC.m_dsign[1]&0x0f ) {
	case 4:		dwCode = CD_RITU10;		break;	//8軽 → 10%
	case 5:		dwCode = CD_RD_RITU8;	break;	//10% → 8軽
	}

	if (dwCode != 0) {
		if (pDBsy->SyFnc( dwCode ) == 0) {
			struct _SY_MSG_PACK2 smp;
			pDBsy->SyMsg( &smp );
			pDBzm->GetCDBData( &m_CREC );
			char zei[6];

			//消費税
			sprintf_s( getLINE_DATA()->syzstr, sizeof getLINE_DATA()->syzstr, "%s", sy_chk_txt( &m_CREC, &smp, acINP_mode ) );
			//消費税再表示
			int pntbl[] = { AC_ZEI_PN, AC_SYZ_PN };
			// 05.22 /08
			::ZeroMemory( zei, sizeof zei );
			l_input( zei, ( char* )( LPCTSTR )smp.SY_MSG_SYZEI );	// 税額をセット
			memcpy( getLINE_DATA()->drec.m_zei, zei, 6 );
			memmove( m_CREC.m_zei, zei, sizeof( zei ) );

			if (is_syohizeisiwake( &m_CREC )) {
				getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = TRUE;
			}
			else {
				getLINE_DATA()->lcnd[AC_ZEI_PN].INP_sg = FALSE;
			}
			TerminationDataSet( pntbl, 2 );
			ret = TRUE;
		}
	}
	return TRUE;
}


// OnBeginLabelEdit() が呼ばれたときの 入力ボックスの情報をクリア
void CDBAbookView::InitNMbeginLabel()
{
	m_NMbegin.hdr.hwndFrom = NULL;
	m_NMbegin.hdr.idFrom = 0;
	m_NMbegin.hdr.code = 0;

	m_NMbegin.iItem = -1;
	m_NMbegin.iClick = 0;
	m_NMbegin.iLine = 0;
	m_NMbegin.rect.SetRectEmpty();
	m_NMbegin.data.DTLIST_Init();
}


//定型仕訳か摘要自動仕訳の残りチェック
int CDBAbookView::RemainSwkdataCheck(CString& strType)
{
	int rdata = 0;

	if( m_bAutoSwk ) {
		if( m_AutoSwkCnt > 0 ) {
			if( (rdata = (m_AutoSwkCnt - m_AutoSwkTrn)) > 0 ) {
				strType = "摘要自動仕訳処理中です";
				return rdata;
			}
		}
	}
	//定型仕訳
	rdata = RemainRdata();

	if( rdata > 0 ) {
		strType = "定型仕訳処理中です";
	}

	return rdata;
}


int CDBAbookView::EndRemainAutodata()
{
	if( m_AutoSwkCnt > 0 ) {
		m_AutoSwkCnt = m_AutoSwkTrn = 0;
		m_tkjourAry.RemoveAll();
	}
	return 0;
}


int CDBAbookView::EndRemaindata()
{
	//摘要自動仕訳終わり
	EndRemainAutodata();

	//定型仕訳
	EndRdata();

	return 0;
}


HWND CDBAbookView::GetKamokuCtrlHWND()
{
	int pn = get_nowpn();
	HWND hwnd;

	switch( pn ) {
	case AC_KMK_PN:
		hwnd = m_Input.m_hWnd;	break;
	case AC_SELKAMOK_PN:
		hwnd = m_KamokInp.m_hWnd;	break;
	case AC_SELBRN_PN:
		hwnd = m_BrnInp.m_hWnd;	break;
	}
	return hwnd;
}

BOOL CDBAbookView::EnableCallPro()
{
	MyTrace("CDBAbookView::EnableCallPro TOP");

	if( m_pSyzInp == NULL )
		return TRUE;

	int pn = get_nowpn();
	MyTrace("CDBAbookView::EnableCallPro pn = %d\n", pn);

	if( pn == AC_SYZ_PN ) {
		if( !m_pSyzInp->CheckInvno() )
			return FALSE;
	}

	MyTrace("CDBAbookView::EnableCallPro END TRUE");

	return TRUE;
}

// 外税免税自動計算
BOOL CDBAbookView::ChangeSotoMenZei()
{
	BOOL ret = FALSE;

	if( !IsMasterType(MST_SYZ_INV) ) {
		return FALSE;
	}
	DWORD	dwCode = 0;

	ACBOOK_LINE *dtline;
	dtline = getLINE_DATA();

	if( !modefy_ok(acINP_mode, &getLINE_DATA()->drec) )
		return FALSE;

	char ip_val[6] = { 0 };

	if( !is_sotomensiwake(&m_CREC) )
		return FALSE;

	if( m_CREC.m_ddate < SYZ_INV_YMD )
		return FALSE;

	if( dtline->lcnd[AC_ZEI_PN].KEY_inp )
		return FALSE;

	int ln = get_nowln();
	TerminationZei(ln, 0, 0, 0);

	pDBzm->SetCDBData(&m_CREC);

	CZmGen8 zmGen;
	zmGen.RepairMenzeiZeigaku(Voln1, pDBzm->dbdata);

	pDBzm->GetCDBData(&m_CREC);
	memmove(dtline->drec.m_val, m_CREC.m_val, 6);
	memmove(dtline->drec.m_zei, m_CREC.m_zei, 6);

	//税額インプット更新
	VARIANT var;
	memcpy(ip_val, m_CREC.m_zei, 6);
	var.pbVal = (BYTE*)ip_val;
	DBdata_set(this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_ARITH, 0);

	int pntbl[] = {
		AC_NKIN_PN, ZEI_PN
	};

	if( getLINE_DATA()->lcnd[AC_SKIN_PN].INP_sg ) {
		pntbl[0] = AC_SKIN_PN;
	}
	TerminationDataSet(pntbl, 2);
	ret = TRUE;
	dtline->lcnd[AC_ZEI_PN].KEY_inp = ret;

	return ret;

}

// 現金等か？
BOOL IsGenkin( CZVOLUME* pz, DWORD kncd )
{
	BOOL bRet = FALSE;
	BYTE apno = pz->apno;

	if( apno == 0x59 ) {		//宗教
		if( (kncd & 0xffff0000) == 0x03040000 ) {
			bRet = TRUE;
		}
	}
	else if( apno == 0x58 ) {	//学校
		if( (kncd & 0xffff0000) >= 0x03010000 &&
			(kncd & 0xffff0000) <  0x03040000 ) {
			bRet = TRUE;
		}
	}
	else {	//法人等 
		if( (kncd & 0xffff0000) >= 0x01000000 &&
			(kncd & 0xffff0000) <  0x01040000 ) {
			bRet = TRUE;
		}
	}

	return bRet;
}

//法人・個人・建設・運送
DWORD uri00[] = {
	0x01040100, 0x01040200, 0x01040300, 0x01040900, 0x01070400, 0x01071200, 0
};
//個人簡易
DWORD uri10k[] = {
	0x01040100, 0x01040200, 0x01040300, 0x01040400, 0x01071200, 0
};
//病院
DWORD uri30[] = {
	0x01040100, 0x01040200, 0x01040300, 0x01040900, 0x01070300, 0x01071200, 0
};
//公益
DWORD uri50[] = {
	0x01040100, 0x01040200, 0x01040300, 0x01040900, 0x01070700, 0
};
//社会福祉
DWORD uri52[] = {
	0x01050100, 0x01050200, 0x01070d00, 0
};

//社会福祉改正対応
DWORD uri52ks[] = {
	0x01050100, 0x01050200, 0x01050300, 0x01050400, 0x01050500, 0x01071200, 0
};

//宗教
DWORD uri59[] = {
	0x03050100, 0x03050300, 0x03051000, 0
};
//学校
DWORD uri58[] = {
	0x03040100, 0x03040200, 0x03040300, 0x03072200, 0
};


// 売掛金等、未収入金
BOOL IsUrikake( CZVOLUME* pz, DWORD kncd )
{
	BOOL bRet = FALSE;
	DWORD *pCDtb;
	BYTE apno = pz->apno;

	if( apno == 0x50 || apno == 0x51 ) {
		pCDtb = uri50;
	}
	else if( apno == 0x52 ) {
		if( IsSyafuKaisei( pz ) )	//社会福祉改正対応
			pCDtb = uri52ks;
		else
			pCDtb = uri52;
	}
	else if( apno == 0x59 ) {
		pCDtb = uri59;
	}
	else if( apno == 0x58 ) {
		pCDtb = uri58;
	}
	else if( apno == 0x30 ) {
		pCDtb = uri30;
	}
	else {
		pCDtb = uri00;
		if( apno == 0x10 && (pz->ind_table&0x01) )
			pCDtb = uri10k;
	}

	for( int i = 0; pCDtb[i] != 0; i++ ) {
		if( (kncd & 0xffffff00) == pCDtb[i] ) {
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}


// 買掛金等 コードテーブル
DWORD kai50[] = {
	0x04010100, 0x04010200, 0x04010500, 0x04010a00, 0x04010d00, 0x04010400, 0
};

DWORD kai51[] = {
	0x04010100, 0x04010200, 0x04010500, 0x04010600, 0x04010C00, 0x04010F00, 0x04010400, 0
};

DWORD kai52[] = {
	0x04010100, 0x04010200, 0x04010300, 0x04010400, 0x04010700, 0x04010800, 0x04010900, 0x04010a00, 0x04011400, 0x04010500, 0
};

DWORD kai52ks[] = {
	0x04010100, 0x04010200, 0x04010300, 0x04010400, 0x04010500, 0x04010600, 0x04010700, 0x04010800, 0x04010900, 0x04010a00, 
	0x04010b00, 0x04010c00, 0x04010d00, 0x04010e00, 0x04011300, 0x04011400, 0x04011500, 0x04011600, 
	0x04011700, 0x04011d00, 0x04012200, 0x04010f00, 0x04011000, 0
};

DWORD kai59[] = {
	0x05010100, 0x05010200, 0x05010500, 0x05010600, 0x05010800, 0x05010e00, 0x05010400, 0
};

DWORD kai58[] = {
	0x06010100, 0x06010200, 0x06010300, 0x06010400, 0x06010500, 0x06010600, 0x06010700, 0x06010800, 
	0x06011200, 0x06011800, 0x06012200, 0x06011100, 0
};


// 買掛金等(預り金も)
BOOL IsKaikake( CZVOLUME* pz, DWORD kncd )
{
	BOOL bRet = FALSE;
	BYTE apno = pz->apno;

	if( apno < 0x50 ) {	//非営利以外
		if( (kncd & 0xffff0000) >= 0x04010000 && (kncd & 0xffffff00) <= 0x04040100 ) {
			bRet = TRUE;
		}
		else if( (kncd & 0xffffff00) == 0x04051700 ) {	//未払消費税等
			bRet = TRUE;
		}
		else if( (kncd & 0xffffff00) == 0x04050300 ) {	//預り金
			bRet = TRUE;
		}

		if( ! bRet ) {
			if( apno == 0x30 ) {
				if( (kncd & 0xffffff00) == 0x04050200 ) {	//預り金
					bRet = TRUE;
				}
			}
		}
	}
	else {
		DWORD *pCDtb;

		if( apno == 0x50 ) {
			pCDtb = kai50;
		}
		else if( apno == 0x51 ) {
			pCDtb = kai51;
		}
		else if( apno == 0x52 ) {
			if( IsSyafuKaisei( pz ) )	//社会福祉改正対応
				pCDtb = kai52ks;
			else
				pCDtb = kai52;
		}
		else if( apno == 0x59 ) {
			pCDtb = kai59;
		}
		else if( apno == 0x58 ) {
			pCDtb = kai58;
		}
		else {
			return FALSE;
		}

		for( int i = 0; pCDtb[i] != 0; i++ ) {
			if( (kncd & 0xffffff00) == pCDtb[i] ) {
				bRet = TRUE;
				break;
			}
		}
	}

	return bRet;
}


// 仮払消費税？
BOOL IsKaribarai( DWORD kncd )
{
	BOOL bRet = FALSE;

	DWORD	svkcd1;
	svkcd1 = _atoh( (LPSTR)(LPCTSTR)pDBzm->szvol->SVkcod1 );

	if( (kncd & 0xffffff00) == (svkcd1&0xffffff00) ) {
		bRet = TRUE;
	}

	return bRet;
}


// 仮受消費税？
BOOL IsKariuke( DWORD kncd )
{
	BOOL bRet = FALSE;

	DWORD	svkcd2;
	svkcd2 = _atoh( (LPSTR)(LPCTSTR)pDBzm->szvol->SVkcod2 );

	if( (kncd & 0xffffff00) == (svkcd2&0xffffff00) ) {
		bRet = TRUE;
	}

	return bRet;
}


// 基本科目になった場合に相手科目が現金等で
// 基本の仕様と異なる科目か？
BOOL IsBaseCheckKamoku( CZVOLUME* pz, DWORD kncd )
{
	BOOL bRet = FALSE;
	BYTE apno = pz->apno;

	if( apno == 0x20 ) {
		if( (kncd & 0xffffff00) == 0x01060200 ) {	//未成工事支出金
			bRet = TRUE;
		}
	}

	return bRet;
}



