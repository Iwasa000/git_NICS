// DBSyzShinMainView.cpp : CDBSyzShinMainView クラスの実装
//

#include "stdafx.h"
#include "DBSyzShinMain.h"

#include "MainFrm.h"
#include "DBSyzShinMainDoc.h"
#include "DBSyzShinMainView.h"

#include "PrnReportDlg.h"
#include "MiddlePyment.h"
#include "SkjHjnConvert.h"
#include "SkjKjnConvert.h"
//2015.12.15 INSERT START
#include "DenshiInfo.h"
//2015.12.15 INSERT END
#include "CDiagException.h"
#include "SkjUserSettings.h" // TISW修正[22-0676] 2026/03/19

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PRNEXSUB_INF								m_PrnEXSub;
CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub;

//課税取引金額計算表
PRNEXSUB_INF								m_PrnEXSub_KAZ;
CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub_KAZ;

//税務代理帳票
PRNEXSUB_INF								m_PrnEXSub_ZEI;
CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub_ZEI;
// CDBSyzShinMainView

IMPLEMENT_DYNCREATE(CDBSyzShinMainView, ICSFormView)

BEGIN_MESSAGE_MAP(CDBSyzShinMainView, ICSFormView)
	ON_MESSAGE(WM_USER_UNLOCK, OnUserUnlock)
	ON_MESSAGE(WM_USER_SYZINIT, OnUserSyzInit)//'14.10.08
	ON_MESSAGE(WM_USER_TENKI, OnUserTenkiData)//'14.10.08
	ON_COMMAND(ID_BUTTON_F4, OnButtonF4)
	ON_COMMAND(ID_BUTTON_F5, OnButtonF5)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, ICSFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, ICSFormView::OnFilePrintPreview)
	ON_CBN_SELCHANGE(IDC_SKKBN_CMB, &CDBSyzShinMainView::OnCbnSelchangeSkkbnCmb)
	ON_CBN_SELCHANGE(IDC_MONTH_CMB, &CDBSyzShinMainView::OnCbnSelchangeMonthCmb)
	ON_CBN_SELCHANGE(IDC_KIKANT_CMB, &CDBSyzShinMainView::OnCbnSelchangeKikantCmb)
	ON_BN_CLICKED(IDC_CHECK_TOKUSYU, &CDBSyzShinMainView::OnBnClickedCheckTokusyu)
	ON_BN_CLICKED(IDC_CHECK_RIREKISEL, &CDBSyzShinMainView::OnBnClickedCheckRirekisel)
	ON_CBN_SELCHANGE(IDC_TYUKAN_KIKAN_CMB, &CDBSyzShinMainView::OnCbnSelchangeTyukanKikanCmb)
	ON_BN_CLICKED(IDC_INPUT_BTN, &CDBSyzShinMainView::OnBnClickedInputBtn)
	ON_BN_CLICKED(IDC_CHECK21, &CDBSyzShinMainView::OnBnClickedCheck21)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK1, &CDBSyzShinMainView::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CDBSyzShinMainView::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CDBSyzShinMainView::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CDBSyzShinMainView::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CDBSyzShinMainView::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_TNOZE_BTN, &CDBSyzShinMainView::OnBnClickedTnozeBtn)
	ON_WM_SIZE()
	ON_COMMAND(ID_BUTTON_F9, &CDBSyzShinMainView::OnButtonF9)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F9, &CDBSyzShinMainView::OnUpdateButtonF9)
//	ON_UPDATE_COMMAND_UI(ID_SHIN_PAYMENT, &CDBSyzShinMainView::OnUpdateShinPayment)
	ON_UPDATE_COMMAND_UI(ID_ZEIRISHI_INF, &CDBSyzShinMainView::OnUpdateZeirishiInf)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F4, &CDBSyzShinMainView::OnUpdateButtonF4)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F5, &CDBSyzShinMainView::OnUpdateButtonF5)
	ON_BN_CLICKED(IDC_MINASHI_CHK, &CDBSyzShinMainView::OnBnClickedMinashiChk)
//	ON_COMMAND(ID_KOMONSAKI_INF, &CDBSyzShinMainView::OnKomonsakiInf)
	ON_NOTIFY(ICS_NM_REPORT_SELCHANGED, IDC_PAST_REPORT, OnNMReportSelchangedPastReport)
	ON_CBN_SELCHANGE(IDC_TCOUNT_CMB, &CDBSyzShinMainView::OnCbnSelchangeTcountCmb)
	ON_UPDATE_COMMAND_UI(ID_KOMONSAKI_INF, &CDBSyzShinMainView::OnUpdateKomonsakiInf)
	ON_BN_CLICKED(IDC_BUTTON_KJDEL, &CDBSyzShinMainView::OnBnClickedButtonKjdel)
	ON_BN_CLICKED(IDC_CHECK6, &CDBSyzShinMainView::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_DENSHI_INFO, &CDBSyzShinMainView::OnBnClickedDenshiInfo)
	ON_BN_CLICKED(IDC_CHECK7, &CDBSyzShinMainView::OnClickedCheck7)
	ON_BN_CLICKED(IDC_BUTTON1, &CDBSyzShinMainView::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_KESSAN_KIKAN_CMB, &CDBSyzShinMainView::OnCbnSelchangeKessanKikanCmb)
	ON_BN_CLICKED(IDC_CHECK8, &CDBSyzShinMainView::OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_CHECK10, &CDBSyzShinMainView::OnBnClickedCheck10)
END_MESSAGE_MAP()


// CDBSyzShinMainView コンストラクション/デストラクション

CDBSyzShinMainView::CDBSyzShinMainView()
	: ICSFormView(CDBSyzShinMainView::IDD)
	, m_pZmSub(NULL)
//	, m_OPEla(0)
	, m_pSnHeadData(0)
	, m_PrintType(0)
	, m_K221sw(0)
	, m_SimpSw(0)
	, m_H301sw(0)
	, m_toku(0)
	, m_Kenei(0)
	, m_MatuDay(0)
	, m_IsInitFlg(FALSE)
	, m_pTblhdl(NULL)
	, m_pSkjCnvtbl(NULL)
	, m_TaxReforms(1)
	, m_IsDispedFlg(FALSE)
	, m_swBeFork(0)
	, m_Printsw(0)
	, m_SvSn_Sign4(0)
	, m_SvSVmzsw(0)
	, m_SpcSg(0)						// '14.08.08
	, m_SixKindFlg(TRUE)				// 150106
	, m_pMnec(NULL)						// '15.07.21
	, m_SvDispTabSgn(0)					// '15.08.21
	, m_IsAbleTkkz(FALSE)				// '15.08.27
	, m_ShRevType(ID_ICSSH_NO_REVTYPE)	// '15.08.27
	, m_SvIsPrintableKojinNo(false)
//2018.08.21 INSERT START
	, m_MyNumBrowse(FALSE)
//2018.08.21 INSERT END
	, m_IsAbleTkrei(FALSE)
	, m_Drophistorysw(FALSE)//19.11.07
	, m_IsAbleR02Reform ( FALSE )
// kasai add -->
	, m_PnoJigyo(0)
	, m_PnoFudousan(0)
	, m_PnoNougyou(0)
	, m_PnoSell(0)
	, m_PnoBuy(0)
// kasai add <--
// 220412 add -->
	, m_PnoKuchiTC(0)
	, m_PnoKuchiTM(0)
	, m_PnoKuchiHC(0)
	, m_PnoKuchiHM(0)
// 220412 add <--
{
	// TODO: 構築コードをここに追加します。

	m_Arith.l_defn( 0x16 );
	memset( &m_PrintCmInfo, '\0', sizeof(SH_PRINT_CMINFO) );

	// ブラシ作成
	LOGBRUSH	lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = HSTATIC_COLOR_BCOLOR;
//	lb.lbColor = PALETTERGB( 0xff, 0xff, 0xff );
	m_hBrushBkgnd.CreateBrushIndirect( &lb );

//2016.02.23 INSERT START
	//LOGBRUSH	lb2;
	//lb2.lbStyle = BS_SOLID;
	//lb2.lbColor = PALETTERGB( 255, 0, 0 );
	//m_hBrushLock.CreateBrushIndirect( &lb2 );
//2016.02.23 INSERT END

//--> '15.07.21 INS START
	memset( &m_svMynoInf, '\0', sizeof(SYZ_MYNO_SVINF) );
	m_svMynoInf.kubun = -1;
//<-- '15.07.21 INS END
	//takemura 
//	m_Tokurei_USign = -1;
//	m_Tokurei_SSign = -1;

	Kazei_DButton_sw = 0;
	Toku_DButton_sw = 0;
	Zeiri_DButton_sw = 0;

// 24/04/01_246 add -->
	m_GetSki = 0;
// 24/04/01_246 add <--

//----> 20240116
	m_isScoreLinkCall = -1;
	m_ScoreLinkPdfNam.Empty();
//<---- 20240116
}

CDBSyzShinMainView::~CDBSyzShinMainView()
{
}

void CDBSyzShinMainView::DoDataExchange(CDataExchange* pDX)
{
	ICSFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_BASE, m_BaseGroup);
	DDX_Control(pDX, IDC_GROUP_CALQ, m_CalqGroup);
	DDX_Control(pDX, IDC_GROUP_INFO, m_InfoGroup);
	DDX_Control(pDX, IDC_PAST_REPORT, m_PastReport);
	DDX_Control(pDX, IDC_MONTH_CMB, m_ComboMonthCnt);
	DDX_Control(pDX, IDC_SKKBN_CMB, m_ComboSkkbn);
	DDX_Control(pDX, IDC_KIKANT_CMB, m_ComboKikanTansyuku);
	DDX_Control(pDX, IDC_TCOUNT_CMB, m_ComboTyukanAri);
	DDX_Control(pDX, IDC_TYUKAN_KIKAN_CMB, m_ComboTyukanKikan);
	DDX_Control(pDX, IDC_KESSAN_KIKAN_CMB, m_ComboKessanKikan);
	DDX_Control(pDX, IDC_CHECK_TOKUSYU, m_CheckTokusyu);
	DDX_Control(pDX, IDC_CHECK_RIREKISEL, m_ButtonRirekiSel);
	DDX_Control(pDX, IDC_MINASHI_CHK, m_CheckMinashi);
	DDX_Control(pDX, IDC_STATIC_B01, m_StaticB01);
	DDX_Control(pDX, IDC_STATIC_B02, m_StaticB02);
	DDX_Control(pDX, IDC_STATIC_B03, m_StaticB03);
	DDX_Control(pDX, IDC_STATIC_B04, m_StaticB04);
	DDX_Control(pDX, IDC_STATIC_B05, m_StaticB05);
	DDX_Control(pDX, IDC_STATIC_B06, m_StaticB06);
	DDX_Control(pDX, IDC_STATIC_B07, m_StaticB07);
	DDX_Control(pDX, IDC_STATIC_B08, m_StaticB08);
	DDX_Control(pDX, IDC_STATIC_B09, m_StaticB09);
	DDX_Control(pDX, IDC_STATIC_B51, m_StaticB51);
	DDX_Control(pDX, IDC_STATIC_B52, m_StaticB52);
	DDX_Control(pDX, IDC_STATIC_B53, m_StaticB53);
	DDX_Control(pDX, IDC_STATIC_B54, m_StaticB54);
	DDX_Control(pDX, IDC_STATIC_B55, m_StaticB55);
	DDX_Control(pDX, IDC_STATIC_B56, m_StaticB56);
	DDX_Control(pDX, IDC_STATIC_B57, m_StaticB57);
	DDX_Control(pDX, IDC_STATIC_B59, m_StaticB59);
	DDX_Control(pDX, IDC_STATIC_I01, m_StaticI01);
	DDX_Control(pDX, IDC_STATIC_I02, m_StaticI02);
	DDX_Control(pDX, IDC_STATIC_I03, m_StaticI03);
	DDX_Control(pDX, IDC_STATIC_I04, m_StaticI04);
	DDX_Control(pDX, IDC_STATIC_I05, m_StaticI05);
	DDX_Control(pDX, IDC_STATIC_I06, m_StaticI06);
	DDX_Control(pDX, IDC_CHECK21, m_Check21);
	DDX_Control(pDX, IDC_GROUP_RINFO, m_RInfoGroup);
	DDX_Control(pDX, IDC_CHECK1, m_Check1);
	DDX_Control(pDX, IDC_CHECK2, m_Check2);
	DDX_Control(pDX, IDC_CHECK3, m_Check3);
	DDX_Control(pDX, IDC_CHECK4, m_Check4);
	DDX_Control(pDX, IDC_CHECK5, m_Check5);
	DDX_Control(pDX, IDC_STATIC_HELP, m_StaticHelp);
	DDX_Control(pDX, IDC_EDIT01, m_MnEdit1);
	DDX_Control(pDX, IDC_EDIT02, m_MnEdit2);
	DDX_Control(pDX, IDC_EDIT03, m_MnEdit3);
	DDX_Control(pDX, IDC_EDIT04, m_MnEdit4);
	DDX_Control(pDX, IDC_STATIC_B10, m_StaticB10);
	DDX_Control(pDX, IDC_STATIC_B11, m_StaticB11);
	DDX_Control(pDX, IDC_STATIC_B12, m_StaticB12);
	DDX_Control(pDX, IDC_STATIC_B13, m_StaticB13);
	DDX_Control(pDX, IDC_BUTTON_KJDEL, m_KjdelBtn);
	DDX_Control(pDX, IDC_CHECK6, m_Check6);
	DDX_Control(pDX, IDC_STATIC_INPUTLOCK, m_InputLock);
	DDX_Control(pDX, IDC_CHECK7, m_Check7);
	DDX_Control(pDX, IDC_BUTTON1, m_btn1);
	DDX_Control(pDX, IDC_CHECK9, m_Check9);
	DDX_Control(pDX, IDC_CHECK10, m_Check10);
	DDX_Control(pDX, IDC_STATIC_C01, m_StaticC01);
	DDX_Control(pDX, IDC_STATIC_B58, m_StaticB58);
	DDX_Control(pDX, IDC_STATIC_B14, m_StaticB14);
}

BOOL CDBSyzShinMainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return ICSFormView::PreCreateWindow(cs);
}

void CDBSyzShinMainView::OnInitialUpdate()
{
	ICSFormView::OnInitialUpdate();
	ResizeParentToFit();

	// グループテキスト初期化
	m_BaseGroup.SetTitleString( _T("基本情報") );
	m_CalqGroup.SetTitleString( _T("設定項目") );
	m_InfoGroup.SetTitleString( _T("申告選択") );
	m_RInfoGroup.SetTitleString( _T("履歴選択中") );
	m_BaseGroup.ModifyStyle( 0, WS_CLIPSIBLINGS );
	m_CalqGroup.ModifyStyle( 0, WS_CLIPSIBLINGS );
	m_InfoGroup.ModifyStyle( 0, WS_CLIPSIBLINGS );
	m_RInfoGroup.ModifyStyle( 0, WS_CLIPSIBLINGS );

	m_RInfoGroup.SetBkColor( PALETTERGB(0xcd, 0xcd, 0xcd) );

//2016.02.23 INSERT START
	Make_StaticFont();

	m_InputLock.DefaultColor( FALSE );
	COLORREF color;
	color = PALETTERGB( 255, 0, 0 );
	m_InputLock.SetBackColor(color);
	color = PALETTERGB( 255, 255, 255 );
	m_InputLock.SetTextColor(color);
//2016.02.23 INSERT END

	//PALETTERGB( 0xe0, 0xff, 0xff )
}


// CDBSyzShinMainView 診断

#ifdef _DEBUG
void CDBSyzShinMainView::AssertValid() const
{
	ICSFormView::AssertValid();
}

void CDBSyzShinMainView::Dump(CDumpContext& dc) const
{
	ICSFormView::Dump(dc);
}

CDBSyzShinMainDoc* CDBSyzShinMainView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDBSyzShinMainDoc)));
	return (CDBSyzShinMainDoc*)m_pDocument;
}
#endif //_DEBUG

// CDBSyzShinMainViewクラスの印刷

//-----------------------------------------------------------------------------
// 印刷情報のクリア('15.12.21)
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::ClearPsetInfo()
{
	long svPsSigCoprn = m_Pset.psSigCoprn;
	memset( (char *)&m_Pset, '\0', sizeof( NPRT_PAR ) );
//-- CUT --
//	m_Pset.psSigCoprn = svPsSigCoprn;
//---------
	m_SvIsPrintableKojinNo = false;
}

//-----------------------------------------------------------------------------
// 起動中のビューのバージョンの取得
//-----------------------------------------------------------------------------
// 返送値	：	起動中のビューのバージョン
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::GetActViewVer()
{
	ASSERT(m_pMainFrame);
	if( m_pMainFrame ){
		return m_pMainFrame->GetActViewVer();
	}
	return -1;
}

//-----------------------------------------------------------------------------
// 起動中のビューのバージョンを設定（ヘッドデータを使用）
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	取得元のヘッドデータ
//-----------------------------------------------------------------------------
// 返送値			：	セット後のバージョン
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::SetActViewVer(CSnHeadData *pSnHeadData)
{
	int svActver = GetActViewVer();

	ASSERT(m_pMainFrame);
	if( m_pMainFrame ){
		int rtActver = m_pMainFrame->SetActViewVer(pSnHeadData);

		if (m_Drophistorysw == FALSE) {
			if ((rtActver != -1) && (svActver != rtActver)) {
				// データの引っ越し作業？
				m_pMainFrame->TakeOverData(svActver, rtActver);
			}
		}

		return rtActver;
	}
	return -1;
}

//-----------------------------------------------------------------------------
// 起動中のビューのバージョンを設定（ビュー情報を使用）
//-----------------------------------------------------------------------------
// 引数	pSnset	：	取得元のビュー情報
//-----------------------------------------------------------------------------
// 返送値		：	セット後のバージョン
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::SetActViewVer(CSnset *pSnset)
{
	int svActver = GetActViewVer();

	ASSERT(m_pMainFrame);
	if( m_pMainFrame ){

		long	eymd=0;
		if( pSnset->IsTyukanShinkoku() ){
			TimePair t1 = GetCurrentTyukanKikan();
			eymd = CTimeControl::GetIntTime(t1.second);
		}
		else{
			TimePair t1 = GetCurrentKessanKikan();
			eymd = CTimeControl::GetIntTime(t1.second);
		}
		if( eymd ){
			int rtActver =  m_pMainFrame->SetActViewVerByDay(eymd);

			if( (rtActver!=-1) && (svActver!=rtActver) ){
				// データの引っ越し作業？
				m_pMainFrame->TakeOverData(svActver, rtActver);
			}

			return rtActver;
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
// メインアプリで管理している改正バージョンの取得
//-----------------------------------------------------------------------------
// 返送値	：	改正バージョン
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::GetKaiseiVer()
{
	ASSERT(m_pMainFrame);
	if( m_pMainFrame ){
		return m_pMainFrame->GetKaiseiVer();
	}
	return -1;
}

BOOL CDBSyzShinMainView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの印刷準備
	if (GetActViewVer() == ID_VER_SYZ31) {
		CH31HyoView			*pH31HyoView = NULL;
		pH31HyoView = (CH31HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(1));
		if (pH31HyoView != NULL) {
			pH31HyoView->SetPrtPar(&m_Vprn, &m_Pset, &m_PrintCmInfo);
		}
	}
	else {
		CH26HyoView			*pH26HyoView = NULL;
		pH26HyoView = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(2));
		if (pH26HyoView != NULL) {
			pH26HyoView->SetPrtPar(&m_Vprn, &m_Pset, &m_PrintCmInfo);

			//=== TEST ===
			//		PRtype = 0x00;
			//		if( m_pSnHeadData->Sn_TYPE&0x01 ){
			//			PRtype = (char)0x80;
			//		}
			//		PgMx = 1;
			//============
		}
	}
	if (GetActViewVer() == ID_VER_SYZ31) {
		CTaxationList31View	*pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 4 ));
		if( pTaxlist31View != NULL ){
			pTaxlist31View->SetPrtPar( &m_Vprn, &m_Pset , &m_PrintCmInfo );

	//=== TEST ===
	//		PRtype = 0x00;
	//		if( m_pSnHeadData->Sn_TYPE&0x01 ){
	//			PRtype = (char)0x80;
	//		}
	//		PgMx = 1;
	//============
		}
	}
	else{
		CTaxationListView	*pTaxlistView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 3 ));
		if( pTaxlistView != NULL ){
			pTaxlistView->SetPrtPar( &m_Vprn, &m_Pset , &m_PrintCmInfo );
		}
	}

	// 一括印刷情報の取得
	EnumIdShBatchPrintState	BpState = ID_ORIGINALSTART_BPSTATE;
	SH_BPPRINT_INFO			BpPrintInfo = {0};
	if( m_pMainFrame ){
		m_pMainFrame->GetBatchPrintInfo( &BpState, &BpPrintInfo );
	}

	//----> 20240116
	if( m_pMainFrame ){
		m_pMainFrame->GetScoreLinkInfo( m_isScoreLinkCall, m_ScoreLinkPdfNam );
	}
	//<---- 20240116

	// デフォルトの印刷準備
	switch( m_PrintType ){
		case 1 :
			// 還付申告の明細書財務連動チェックリスト
			strcpy_s( (char *)m_Pset.psJobName, sizeof(m_Pset.psJobName),  "還付申告に関する明細書ﾘｽﾄ" );		//	プログラム名称
			if( PCOM_GetString( _T("SelectPrinter"), m_Pset.psPrinterName) == -1 )	return( FALSE );
			m_Pset.psPaperBin		= DMBIN_AUTO;																// 給紙
			m_Pset.psPaperSize		= DMPAPER_A4;																// 用紙サイズ
			m_Pset.psOrientation	= DMORIENT_PORTRAIT;														// 方向
			m_Pset.psScale = 0;																					// 縮小出力サイン
			m_Pset.psCopies = 1;
			m_Pset.psSigDate = m_PrintCmInfo.PsSigDate;
			m_Pset.psSigConame = m_PrintCmInfo.PsSigConame;
			m_Pset.psFncFlg = 0x210;																			// 印刷ダイアログでのプレビュー禁止
			strcpy_s( (char *)m_Pset.psSigCopath, sizeof(m_Pset.psSigCopath), m_PrintCmInfo.PsOfceName );		// 事務所ファイルパス
			strcpy_s( m_Pset.psPsecName, sizeof(m_Pset.psPsecName), "還付申告に関する明細書ﾘｽﾄ" );				// プログラム・セッション名
			memmove( m_Pset.psListName, "消費税申告書", 12 );													// 帳票名
			// カラー指定
			m_Pset.psPrnFlg = 0x300;
			m_Cset.pcFlag = 0x01;
			m_Cset.pcNlin = 3;
			m_Cset.pcNscr = 2;
			m_Cset.pcLpat[0].ppSsel = 7;
			m_Cset.pcLpat[1].ppSsel = 7;
			m_Cset.pcLpat[2].ppSsel = 7;
			m_Cset.pcSpat[0].ppSsel = 7;
			m_Cset.pcSpat[1].ppSsel = 7;
			m_Cset.pcLcol[0] = -1L;
			m_Cset.pcScol[0] = -1L;
			m_Pset.psColorSet= &m_Cset;
			SetNprtParInf( &m_Pset );																		// 顧問先名称及びコード印刷情報の取得
			if( m_Vprn.InitPrint( 2L, pInfo, &m_Pset, this ) != 0 )				return( FALSE );			// プリント初期設定
			if( DoPreparePrinting(pInfo) == FALSE ) {
				if (m_Vprn.End_PrintDlg(this) < 1)  {	// 返送値＝１の場合、印刷プレビュー処理へ
					// 印刷ダイアログにおいて、エラー 又は キャンセル押下時
				}
				return( FALSE );
			}
			else	{
				m_PrintCmInfo.PsSigDate = m_Pset.psSigDate;													// 作成日
				m_PrintCmInfo.PsSigConame = m_Pset.psSigConame;												// 事務所名
				memset( m_PrintCmInfo.PrinterName, '\0', sizeof(m_PrintCmInfo.PrinterName) );
				wsprintf( m_PrintCmInfo.PrinterName, _T("%s"), m_Pset.psPrinterName );
			}
			m_Vprn.GetLineColorX(0, NULL, &Lcol[0], &Lcol[1], &Lcol[2], NULL);
			m_Vprn.GetPaintColorX(0, NULL, &Scol[0], &Scol[1], &Scol[2], NULL);
			break;

		case 2:
			// 有り得ないはずなので未実装！
			break;
		case 0:
			// 還付申告（付表）情報更新
			
			// 電子申告情報のゲット
			if( *m_pShinInfo->pProgress ){
				GetElectronicReport();
			}

			if( m_Vprn.m_ErrFlag == 0 ) {
				if( m_pSnHeadData->IsMiddleProvisional() ){
					// 前年実績による中間申告（第２６号様式）
					if( m_isScoreLinkCall != -1 ){ // 20240116
						strcpy_s( (char *)m_Pset.psJobName, sizeof(m_Pset.psJobName), m_ScoreLinkPdfNam.GetBuffer() );
					}
					else{
						strcpy_s( (char *)m_Pset.psJobName, sizeof(m_Pset.psJobName),  "消費税の中間申告(26号様式)" );		//	プログラム名称
					}
					if( m_Vprn.GetCurKindNo() <= 1 ){
						if( BpState ){
							memmove( m_Pset.psPrinterName, BpPrintInfo.PrinterName, sizeof(m_Pset.psPrinterName) );
						}
						else{
							if( PCOM_GetString( _T("SelectPrinter"), m_Pset.psPrinterName) == -1 ){
								return FALSE;
							}
						}
					}
					m_Pset.psPaperBin	= DMBIN_AUTO;														// 給紙
					m_Pset.psPaperSize	= DMPAPER_A4;														// 用紙サイズ
					m_Pset.psOrientation	= DMORIENT_PORTRAIT;											// 方向
					m_Pset.psScale = 0;																		// 縮小出力サイン
					m_Pset.psPrevFlg = 0x100;																// 印刷プレビュー制御フラグ
					// 印刷設定画面で"2"以上の部数が指定されていれば印刷ダイアログの部数指定は不可とする（10%以降のみ）
					//m_Pset.psCopies = (BpState? (short)BpPrintInfo.copynum: 1);
					if(IsBusuSelect(&m_PrintCmInfo) == TRUE)	{
						m_Pset.psCopies = -1;
					}
					else	{
						m_Pset.psCopies = (BpState? (short)BpPrintInfo.copynum: 1);
					}
					strcpy_s( m_Pset.psPsecName, sizeof(m_Pset.psPsecName), "消費税の中間申告(26号様式)" );	// プログラム・セッション名
					strcpy_s( m_Pset.psListName, sizeof(m_Pset.psListName), "消費税の中間申告(26号様式)" );	// 帳票名

					if( IsPrintableKojinNo() == TRUE ){
						if( m_SvIsPrintableKojinNo == false ){
							m_Pset.psSigCoprn &= ~0x38000;
						}
						m_Pset.psSigCoprn |= 0x4000;
						m_SvIsPrintableKojinNo = true;
					}
					else{
						m_SvIsPrintableKojinNo = false;
					}

					m_PrFlg = 0x101;
					if( _Init_prn(m_PrFlg, &m_Vprn, pInfo, &m_Pset, this) != 0 ){
						SetBpPrintErr();																	// 一括印刷エラーサインセット
						if( (BpState==ID_NORMALPRINT_BPSTATE) || (BpState==ID_PDFPRINT_BPSTATE) ){
							AfxGetMainWnd()->PostMessageA( WM_CLOSE, 0 );									// 特有の処理でICSExit()などの修理終了を行っていない場合
						}
						return FALSE;
					}
					// 一括印刷
					if( BpState == ID_ESTABLISH_BPSTATE ){
						m_Vprn.SetOptionMode( POF_PRCPRINT, 1 );
					}
					else{
						if( (BpState==ID_NORMALPRINT_BPSTATE) || (BpState==ID_PDFPRINT_BPSTATE) ){
							m_Vprn.SetOptionMode( POF_PRCPRINT, 2 );
						}
					}
					if( DoPreparePrinting(pInfo) == FALSE ){
						if( BpState == ID_ESTABLISH_BPSTATE ){	
							m_PrintCmInfo.PsSigDate = m_Pset.psSigDate;										// 作成日
							m_PrintCmInfo.PsSigConame = m_Pset.psSigConame;									// 事務所名
						}
						m_Vprn.End_PrintDlg(this);
						SetBpPrintErr();																	// 一括印刷エラーサインセット
						return FALSE;
					}
					memset( m_PrintCmInfo.PrinterName, '\0', sizeof(m_PrintCmInfo.PrinterName) );
					wsprintf( m_PrintCmInfo.PrinterName, _T("%s"), m_Pset.psPrinterName );
					return TRUE;
				}
				else{
					// プリント初期設定
					// ドキュメント名
					if( m_isScoreLinkCall != -1 ){ // 20240116
						strcpy_s( (char *)m_Pset.psJobName, sizeof(m_Pset.psJobName), m_ScoreLinkPdfNam.GetBuffer() );
					}
					else{
						strcpy_s( (char *)m_Pset.psJobName, sizeof(m_Pset.psJobName), _T("消費税申告書") );
					}
					//yoshida200213
					//プリンタ名しかGetCurKindNoの制御入ってなかったのをm_Pset初期値セットするのをすべて処理しないようにした。.SetContinueMode使用する際の仕様
					if (m_Vprn.GetCurKindNo() <= 1) {
						// プリンタ名
						if (BpState != ID_ORIGINALSTART_BPSTATE) {
							memmove(m_Pset.psPrinterName, BpPrintInfo.PrinterName, sizeof(m_Pset.psPrinterName));
						}
						else {
							if (PCOM_GetString(_T("SelectPrinter"), m_Pset.psPrinterName) == -1) {
								return FALSE;
							}
							memset(m_PrintCmInfo.PrinterName, '\0', sizeof(m_PrintCmInfo.PrinterName));
							memmove(m_PrintCmInfo.PrinterName, m_Pset.psPrinterName, sizeof(m_Pset.psPrinterName));
						}

						m_Pset.psPaperSize = DMPAPER_A4;														// 用紙サイズ
						m_Pset.psOrientation = DMORIENT_PORTRAIT;												// 方向（PORTRAIT／LANDSCAPE）
						// ホッパ	
						if (m_PrintCmInfo.PRtype & 0x80) {
							m_Pset.psPaperBin = 0;
						}
						else {
							m_Pset.psPaperBin = (short)(DMBIN_MANUAL | 0x8000);
						}

						m_Pset.psScale = 0;																		// 縮小印刷サイン
						// 印刷設定画面で"2"以上の部数が指定されていれば印刷ダイアログの部数指定は不可とする（10%以降のみ）
						//m_Pset.psCopies = ((BpState != ID_ORIGINALSTART_BPSTATE) ? (short)BpPrintInfo.copynum : 1);	// 印刷部数				
						if(IsBusuSelect(&m_PrintCmInfo) == TRUE)	{
							m_Pset.psCopies = -1;
						}
						else	{
							m_Pset.psCopies = ((BpState != ID_ORIGINALSTART_BPSTATE) ? (short)BpPrintInfo.copynum : 1);	// 印刷部数				
						}
						m_Pset.psPrevFlg = 0x100;																// プレビュー・コントロール・フラグ	
						pInfo->SetMaxPage(m_PrintCmInfo.PgMx);												// 最終ページ番号設定
						PageNo = 0;																				//ページ番号初期設定
						if (m_PrintCmInfo.PRtype & 0x80) {
							m_Pset.psFncFlg = 0x1;																// 丁合印刷
							m_Pset.psPageNum = 0;
						}
						SetNprtParInf(&m_Pset);																// 顧問先名称及びコード印刷情報の取得

						if (IsPrintableKojinNo() == TRUE) {
							if (m_SvIsPrintableKojinNo == false) {
								m_Pset.psSigCoprn &= ~0x38000;
							}
							m_Pset.psSigCoprn |= 0x4000;
							m_SvIsPrintableKojinNo = true;
						}
						else {
							m_SvIsPrintableKojinNo = false;
						}
					}
				}
			}

			// 改正様式
			EnumIdIcsShRevType	shrevtype = GetShRevType();
			//if(shrevtype >= ID_ICSSH_10PER_REVTYPE)	{
			//}

			// プリント初期設定　カラー対応
			//if( (m_PrintCmInfo.isColorPrn) && (m_PrintCmInfo.PRtype&0x80) && ( (m_pSnHeadData->Sn_TYPE&0x02) || (m_pSnHeadData->Sn_TYPE&0x04 ) )){
			//if(IsColorSelect(&m_PrintCmInfo,m_pSnHeadData) == TRUE)	{
			// 一括印刷時、こっちを通さないと _IsColorMode() の返送値が常にFALSEになる
// 22-0720 del -->
			//if(IsColorSelect(&m_PrintCmInfo,m_pSnHeadData) == TRUE || BpState)	{
// 22-0720 del <--
// 22-0720 add -->
			// 白紙 or 一括印刷
			if((m_PrintCmInfo.PRtype & 0x80) || BpState)	{
// 22-0720 add <--
				//if((shrevtype >= ID_ICSSH_10PER_REVTYPE) && (BpState == 0))	{
				//	m_PrFlg = 0x103;
				//}
				//else	{
					m_PrFlg = 0x102;
				//}
				if( _Init_prn( m_PrFlg, &m_Vprn, pInfo, &m_Pset, this ) != 0 ){
					SetBpPrintErr();		// 一括印刷エラーサインセット
					if( (BpState==ID_NORMALPRINT_BPSTATE) || (BpState==ID_PDFPRINT_BPSTATE) ){
						// 特有の処理でICSExit()などの修理終了を行っていない場合
						AfxGetMainWnd()->PostMessageA( WM_CLOSE, 0 );
					}
					return FALSE;
				}
				// 改正様式(かつ一括印刷以外)では印刷ダイアログにカラー／モノクロの選択を表示しない
				if((shrevtype >= ID_ICSSH_10PER_REVTYPE) && (BpState == 0))	{
					// プレビューのときも実行する
					// 初回は問題ないが、一度モノクロで印刷したあとにカラーを選択してプレビューすると
					// _IsColorMode()がFALSEを返すため
					//if(pInfo->m_bPreview == FALSE)	{
						m_Vprn.SetOptionMode(POF_COLORSPCF,2,0);
					//}
				}
			}
			// ＯＣＲ用紙
			else	{
// 22-0720 del -->
				//// モノクロ出力
				//if( m_PrintCmInfo.PRtype&0x80 ){
				//	m_PrFlg = 0x101;
				//}
				//else{
// 22-0720 del <--
					m_PrFlg = 0x1;
// 22-0720 del -->
				//}
// 22-0720 del <--
				if (_Init_prn(m_PrFlg, &m_Vprn, pInfo, &m_Pset, this) != 0) {
					SetBpPrintErr();
					if ((BpState == ID_NORMALPRINT_BPSTATE) || (BpState == ID_PDFPRINT_BPSTATE)) {
						// 特有の処理でICSExit()などの修理終了を行っていない場合
						AfxGetMainWnd()->PostMessageA(WM_CLOSE, 0);
					}
					return FALSE;
				}
				if((shrevtype >= ID_ICSSH_10PER_REVTYPE) && (BpState == 0))	{
					//if(pInfo->m_bPreview == FALSE)	{
						m_Vprn.SetOptionMode(POF_COLORSPCF,1,0);
					//}
				}
			}
			// 一括印刷
			if( BpState == ID_ESTABLISH_BPSTATE ){
				m_Vprn.SetOptionMode( POF_PRCPRINT, 1 );
			}
			else if( (BpState==ID_NORMALPRINT_BPSTATE) || (BpState==ID_PDFPRINT_BPSTATE) ){
				m_Vprn.SetOptionMode( POF_PRCPRINT, 2 );
			}

			if( DoPreparePrinting(pInfo) == FALSE ){
				if( BpState == ID_ESTABLISH_BPSTATE ){	
					m_PrintCmInfo.PsSigDate = m_Pset.psSigDate;													// 作成日
					m_PrintCmInfo.PsSigConame = m_Pset.psSigConame;												// 事務所名
				}
				m_Vprn.End_PrintDlg(this);
				SetBpPrintErr();																				// 一括印刷エラーサインセット
				return FALSE;
			}
			memset( m_PrintCmInfo.PrinterName, '\0', sizeof(m_PrintCmInfo.PrinterName) );
			wsprintf( m_PrintCmInfo.PrinterName, _T("%s"), m_Pset.psPrinterName );

			CString		msg;
			if( !(m_PrintCmInfo.PRtype&0x80) ){
				// 手差し
				long dFlag = GetPrinterParamEx( (int)MAKELONG(PPI_PRINTPOS, 0), m_PrintCmInfo.PrinterName, -1, 1 );
				KdNo = m_Vprn.GetCurKindNo();
				if(( pInfo->m_bPreview == FALSE )&&( m_Vprn.IsAhPDFPrinter((LPCTSTR)m_Pset.psPrinterName) == FALSE )){
					msg = _T("手差しトレイに");
					//switch( m_PrintCmInfo.PRtable[KdNo-1] & 0x7f ){
					switch(GetPRtable(&m_PrintCmInfo,KdNo-1) & 0x7f)	{
						case 0x01 :	// 本表（一般・簡易)
							if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
								msg += _T("消費税申告書本表（一般用）");
							}
							else{
								msg += _T("消費税申告書本表（簡易課税用）");
							}
							msg += _T("ＯＣＲ用紙を");
							break;
						case 0x57 :	// 特定課税仕入れがある場合の課税標準額等の内訳書
							//msg += _T("特定課税仕入れがある場合の課税標準額等の内訳書");
							if ( GetActViewVer() == ID_VER_SYZ26 ) {
								msg += _T("特定課税仕入れがある場合の");
							}
							msg += _T("課税標準額等の内訳書");
							msg += _T("ＯＣＲ用紙を");
							break;
						default   :
							return FALSE;
							break;
					}
					if( dFlag <= 0L ){
						msg += _T("セットしてください。");
					}
					else{
						if( dFlag > 1L ){
							msg += _T("縦方向にセットしてください。");
						}
						else{
							msg += _T("横方向にセットしてください。");
						}
					}
					if( ICSMessageBox( msg, MB_OKCANCEL, 0 ) == IDCANCEL ){
						m_Vprn.SetContinueMode( -1 );
						psch_end();
						SetBpPrintErr();		// 一括印刷エラーサインセット
						if( (BpState==ID_NORMALPRINT_BPSTATE) || (BpState==ID_PDFPRINT_BPSTATE) ){
							// 特有の処理でICSExit()などの修理終了を行っていない場合
							AfxGetMainWnd()->PostMessageA( WM_CLOSE, 0 );
						}
						return FALSE;
					}
				}
			}
			else{

				//白紙Ａ４用紙をセットしてください・・・のメッセージを出さずに印刷するよう変更yoshida190723
				/*
				// 白紙
				if(( pInfo->m_bPreview == FALSE )&&( m_Vprn.IsAhPDFPrinter((LPCTSTR)m_Pset.psPrinterName) == FALSE )){
					if( BpState ){	// 一括印刷
						return TRUE;
					}
				
					msg = _T("白紙Ａ４用紙をセットしてください。");
				
					if( ((m_PrintCmInfo.PRtable[0]&0x7f)==0x01) && (m_PrintCmInfo.isColorPrn||(m_pSnHeadData->Sn_TYPE&0x01) ) ){
						if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
							
							msg += _T("\n\n本表（一般用）");
							
						}
						else{
							
							msg += _T("\n\n本表（簡易課税用）");
							
						}
						if( !(m_pSnHeadData->Sn_TYPE&0x02) ){
							msg += _T("の提出用印刷は、レーザー用専用白紙を使用してください。");
						}
						else{
							msg += _T("の提出用カラー印刷は、カラーレーザー用専用白紙を使用してください。");
						}
					}
					if( ICSMessageBox(msg, MB_OKCANCEL, 0) == IDCANCEL ){
						psch_end();
						return( FALSE );
					}
				}
				*/
			}
			break;
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------
// 印刷スケジュールの読み込み（平成26年：8%）
//-------------------------------------------------------------------------------------
// 引数	shrevtype	：	改正様式タイプ
//		PrgPath		：	プログラムパス
//-------------------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-------------------------------------------------------------------------------------
int CDBSyzShinMainView::ReadXYSPrintSch_H26( EnumIdIcsShRevType shrevtype, CString& PrgPath )
{
	static	char	*FOLDER;



	CH26HyoView			*pH26HyoView = NULL;
	pH26HyoView = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(2));
	if( pH26HyoView == NULL ){
		return -1;
	}

	int ItCt = 0;
	if( m_pSnHeadData->IsMiddleProvisional() ){	// 前年実績による中間申告（第２６号様式）
		for( ItCt=0; ItCt!=128; ItCt++ ){
			nList[ItCt] = 0;
		}
		for( ItCt=0; ItCt!=m_PrintCmInfo.PgMx; ItCt++ ){
			//switch( m_PrintCmInfo.PRtable[ItCt]&0x7f ) {
			switch(GetPRtable(&m_PrintCmInfo,ItCt) & 0x7f)	{
			case 0x50:
				nList[ItCt] = 1;
				break;
			case 0x51:
				nList[ItCt] = 2;	// 付表１･４
				break;
			default:
				break;
			}
		}

		memset(Hpath, '\0', sizeof(Hpath));
		if( m_pSnHeadData->Sn_KDAYS >= ICS_SH_MYNUMBER_REFORM_DAY ){
			if( pH26HyoView->IsSwitch_HeiseiReiwa(false) ){
				sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBSYZCH00_H27-3.SCH"), PrgPath.GetBuffer());
			}
			else {
				sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBSYZCH00_H27-2.SCH"), PrgPath.GetBuffer());
			}
		}
		else{
			sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27\\DBSYZCH00_H27.SCH"), PrgPath.GetBuffer());
		}
		m_Vprn.m_ErrFlag = psch_inz(0x80, nList, 2, Hpath);
	}
	else{
		// カラープリンター設定
		if( m_Vprn._IsColorMode() == TRUE ){
			PcSw = 0;													// カラー　プリンター
		}
		else{
			PcSw = 1;													// モノクロプリンター
		}
		memset(Hpath, '\0', sizeof(Hpath));
		if( m_PrintCmInfo.PRtype&0x80 ){
			// イメージ出力
			// カラーモード
			m_Vprn.SetOptionMode(POF_IMGPOSCTL, 1, -1);
			m_Vprn.SetOptionMode(POF_POSADJUST, 0, -1);
			// 税理士法施行帳票
			if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
				if( shrevtype >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
					if( m_pSnHeadData->IsSoleProprietor() ){
						if( m_pSnHeadData->Sn_KDAYS >= ICS_SH_MYNUMBER_REFORM_DAY ){
							//if( m_Util.GetPrePrintGengo(m_pSnHeadData, FALSE) == 2 ){
							if( m_Util.GetPrePrintGengo(m_pSnHeadData, GetActViewVer(),  FALSE) == 2 ){
								sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBSYZSHINIPPANZK_H27-3.SCH"), PrgPath.GetBuffer());
							}
							else {
								sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBSYZSHINIPPANZK_H27-2.SCH"), PrgPath.GetBuffer());
							}
						}
						else{
							sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBSYZSHINIPPANZ_H27-2.SCH"), PrgPath.GetBuffer());
						}
					}
					else{
						if( pH26HyoView->IsSwitch_HeiseiReiwa() ) {
							sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBSYZSHINIPPANZH_H27-3.SCH"), PrgPath.GetBuffer());
						}
						else {
							sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBSYZSHINIPPANZH_H27-2.SCH"), PrgPath.GetBuffer());
						}
					}
				}
				else{
					sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H26\\DBSYZSHINIPPANZ_H26.SCH"), PrgPath.GetBuffer());
				}
			}
			else{
				if( m_SixKindFlg ){
					if( m_pSnHeadData->IsSoleProprietor() ){
						if( (shrevtype>=ID_ICSSH_MYNUMBER_REVTYPE) || IsPrintWithMynoHonpyo() ){
							if( m_pSnHeadData->Sn_KDAYS >= ICS_SH_MYNUMBER_REFORM_DAY ){
								//if( m_Util.GetPrePrintGengo(m_pSnHeadData, FALSE) == 2 ) {
								if( m_Util.GetPrePrintGengo(m_pSnHeadData, GetActViewVer(), FALSE) == 2 ) {
									sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBSYZSHINKANIZK_H27-3.SCH"), PrgPath.GetBuffer());
								}
								else {
									sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBSYZSHINKANIZK_H27-2.SCH"), PrgPath.GetBuffer());
								}
							}
							else{
								sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBSYZSHINKANIZ_H27-2.SCH"), PrgPath.GetBuffer());
							}
						}
						else{
							sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27\\DBSYZSHINKANIZ_H27.SCH"), PrgPath.GetBuffer());
						}
					}
					else{
						if( (shrevtype>=ID_ICSSH_MYNUMBER_REVTYPE) || IsPrintWithMynoHonpyo() ){
							if( pH26HyoView->IsSwitch_HeiseiReiwa() ) {
								sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBSYZSHINKANIZH_H27-3.SCH"), PrgPath.GetBuffer());
							}
							else {
								sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBSYZSHINKANIZH_H27-2.SCH"), PrgPath.GetBuffer());
							}
						}
						else{
							sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27\\DBSYZSHINKANIZH_H27.SCH"), PrgPath.GetBuffer());
						}
					}
				}
				else{
					if( m_pSnHeadData->IsSoleProprietor() ){
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H26\\DBSYZSHINKANIZ_H26.SCH"), PrgPath.GetBuffer());
					}
					else{
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H26\\DBSYZSHINKANIZH_H26.SCH"), PrgPath.GetBuffer());
					}
				}
			}
		}
		else{
			// ＯＣＲ印刷
			// 開始位置調整
			m_Vprn.SetOptionMode(POF_POSADJUST, 0, -1);
			m_Vprn.SetOptionMode(POF_SIZEADJUST, 0, -1);
			if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
				if( shrevtype >= ID_ICSSH_TKKZSIIRE_REVTYPE ) {
					if( pH26HyoView->IsSwitch_HeiseiReiwa() ) {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBIPPANKSK_H27-3.SCH"), PrgPath.GetBuffer());
					}
					else {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBIPPANKSK_H27-2.SCH"), PrgPath.GetBuffer());
					}
				}
				else {
					sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H26\\DBIPPANKSK_H26.SCH"), PrgPath.GetBuffer());
				}
			}
			else{
				if( (shrevtype >= ID_ICSSH_MYNUMBER_REVTYPE) || (IsPrintWithMynoHonpyo()) ) {
					if( m_pSnHeadData->IsSoleProprietor() ) {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBKANIKKSK_H27.SCH"), PrgPath.GetBuffer());
					}
					else {
						if( pH26HyoView->IsSwitch_HeiseiReiwa() ) {
							sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBKANIKKSKH_H27-3.SCH"), PrgPath.GetBuffer());
						}
						else {
							sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBKANIKKSKH_H27.SCH"), PrgPath.GetBuffer());
						}
					}
				}
				else if( m_SixKindFlg ) {
					if( m_pSnHeadData->IsSoleProprietor() ) {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27\\DBKANIKKSK_H27.SCH"), PrgPath.GetBuffer());
					}
					else {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27\\DBKANIKKSKH_H27.SCH"), PrgPath.GetBuffer());
					}
				}
				else {
					if( m_pSnHeadData->IsSoleProprietor() ) {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H26\\DBKANIKKSK_H26.SCH"), PrgPath.GetBuffer());
					}
					else {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H26\\DBKANIKKSKH_H26.SCH"), PrgPath.GetBuffer());
					}
				}
			}
		}
		for( ItCt=0; ItCt!=128; ItCt++ ){
			nList[ItCt] = 0;
		}

		//=== TEST ===
		//			PRtable[0] = 0x10;
		//			PRtable[0] = 0x01;
		//============
		for( ItCt = 0; ItCt != m_PrintCmInfo.PgMx; ItCt++ ) {
			//switch( m_PrintCmInfo.PRtable[ItCt] & 0x7f ) {
			switch(GetPRtable(&m_PrintCmInfo,ItCt) & 0x7f)	{
			case 0x01:		// 本表（一般・簡易）
				if( (m_pSnHeadData->Sn_TYPE & 0x01) == 0x00 ) {
					nList[ItCt] = 1;
				}
				else {
					if( !(m_pSnHeadData->Sn_TYPE & 0x02) || PcSw ) {
						// モノクロ提出用
						nList[ItCt] = 2;
					}
					else {
						// 改訂版
						nList[ItCt] = 1;
					}
				}
				break;
			case 0x02:
				if( (m_pSnHeadData->Sn_TYPE & 0x04) && !PcSw ) {
					// 改訂版
					nList[ItCt] = 3;
				}
				else {
					// 控用
					nList[ItCt] = 4;
				}
				break;
			case 0x03:
				// 付表１･４
				nList[ItCt] = 5;
				break;
			case 0x04:
				// 付表２･５
				nList[ItCt] = 6;
				break;
			case 0x05:
				// 付表２-２･５-２
				nList[ItCt] = 7;
				break;
			case 0x06:
				if( m_pSnHeadData->IsSoleProprietor() ) {
					// 仕入控除明細書（個人）
					nList[ItCt] = 9;
				}
				else {
					// 仕入控除明細書（法人）
					nList[ItCt] = 10;
				}
				break;
			case 0x07:
				if( m_pSnHeadData->IsSoleProprietor() ) {
					// 還付申告明細書（個人）
					nList[ItCt] = 13;
				}
				else {
					// 還付申告明細書（法人）
					nList[ItCt] = 11;
				}
				break;
			case 0x08:
				if( m_pSnHeadData->IsSoleProprietor() ) {
					// 還付申告明細書（個人）
					nList[ItCt] = 14;
				}
				else {
					// 還付申告明細書（法人）
					nList[ItCt] = 12;
				}
				break;
			case 0x09:
				/*if( m_pSnHeadData->IsSoleProprietor() ) {
					// 更正の請求（個人）
					nList[ItCt] = 16;
				}
				else {
					// 更正の請求（法人）
					nList[ItCt] = 15;
				}*/

				if( m_pSnHeadData->IsSoleProprietor() ) {
					// 更正の請求（個人）
					if ( pH26HyoView->GetGengoNo_Ksk() == ID_ICSSH_REIWA_GENGO ) {
						nList[ItCt] = 26;
					}
					else {
						nList[ItCt] = 25;
					}
				}
				else {
					// 更正の請求（法人）
					if ( pH26HyoView->GetGengoNo_Ksk() == ID_ICSSH_REIWA_GENGO ) {
						nList[ItCt] = 21;
					}
					else {
						nList[ItCt] = 20;
					}
				}
				break;
			case 0x10:
				if( m_Util.IsBasicMst(m_pZmSub->zvol) ) {
					if( curset.SVmzsw == 0x00 ) {
						// 課税売上高計算表
						//nList[ItCt] = 17;
						nList[ItCt] = 30;
					}
					else {
						// 課税売上高計算表 経過措置
						//nList[ItCt] = 22;
						nList[ItCt] = 40;
					}
				}
				break;
			case 0x11:
				if( m_Util.IsBasicMst(m_pZmSub->zvol) ) {
					if( curset.SVmzsw == 0x00 ) {
						// 課税仕入高計算表
						//nList[ItCt] = 18;
						nList[ItCt] = 31;
					}
					else {
						// 課税仕入高計算表 経過措置
						//nList[ItCt] = 23;
						nList[ItCt] = 41;
					}
				}
				break;
			case 0x12:
				if( m_Util.IsBasicMst(m_pZmSub->zvol) ) {
					if( curset.SVmzsw == 0x00 ) {
						// 課税取引金額計算表 事業所得
						//nList[ItCt] = 19;
						nList[ItCt] = 32;
					}
					else {
						// 課税取引金額計算表 事業所得 経過措置
						//nList[ItCt] = 24;
						nList[ItCt] = 42;
					}
				}
				break;
			case 0x13:
				if( m_Util.IsBasicMst(m_pZmSub->zvol) ) {
					if( curset.SVmzsw == 0x00 ) {
						// 課税取引金額計算表 不動産所得
						//nList[ItCt] = 20;
						nList[ItCt] = 33;
					}
					else {
						// 課税取引金額計算表 不動産所得 経過措置
						//nList[ItCt] = 25;
						nList[ItCt] = 43;
					}
				}
				break;
			case 0x14:
				if( m_Util.IsBasicMst(m_pZmSub->zvol) ) {
					if( curset.SVmzsw == 0x00 ) {
						// 課税取引金額計算表 農業所得
						//nList[ItCt] = 21;
						nList[ItCt] = 34;
					}
					else {
						// 課税取引金額計算表 農業所得 経過措置
						//nList[ItCt] = 26;
						nList[ItCt] = 44;
					}
				}
				break;
			case 0x15:
				// 付表５－２－２
				nList[ItCt] = 8;
				break;
			case 0x16:
				// 付表６
				nList[ItCt] = 9; //仕入控除明細書（個人）は現在使用していないため
				break;
			case 0x57:
				if( !(m_pSnHeadData->Sn_TYPE & 0x02) || PcSw ) {
					//提出モノクロ
					//nList[ItCt] = 28;
					nList[ItCt] = 51;
				}
				else {
					//提出カラー
					//nList[ItCt] = 27;
					nList[ItCt] = 50;
				}
				break;
			case 0x58:
				if( (m_pSnHeadData->Sn_TYPE & 0x04) && !PcSw ) {
					//控用カラー
					//nList[ItCt] = 29;
					nList[ItCt] = 55;
				}
				else {
					//控用モノクロ
					//nList[ItCt] = 30;
					nList[ItCt] = 56;
				}
				break;
			default:
				break;
			}
		}
		//※nListの中身は必ず昇順且つ出力順であること！
		m_Vprn.m_ErrFlag = psch_inz(0x80, nList, 2, Hpath);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 印刷スケジュールの読み込み（令和01年：10%）
//-------------------------------------------------------------------------------------
// 引数	shrevtype	：	改正様式タイプ
//		PrgPath		：	プログラムパス
//-------------------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-------------------------------------------------------------------------------------
int CDBSyzShinMainView::ReadXYSPrintSch_R01( EnumIdIcsShRevType shrevtype, CString& PrgPath )
{
	int ItCt = 0;
// 24/03/19_税務代理書面 add -->
	int ItCt2 = 0;
// 24/03/19_税務代理書面 add <--
	if( m_pSnHeadData->IsMiddleProvisional() ){	// 前年実績による中間申告（第２６号様式）
		for( ItCt=0; ItCt!=128; ItCt++ ){
			nList[ItCt] = 0;
		}
		//for( ItCt=0; ItCt!=m_PrintCmInfo.PgMx; ItCt++ ){
		//	switch( m_PrintCmInfo.PRtable[ItCt]&0x7f ) {
		for( ItCt=0; ItCt!=m_PrintCmInfo.PgMx2; ItCt++ ){
// 24/04/04_341 cor -->
			//switch(GetPRtable(&m_PrintCmInfo,ItCt) & 0x7f)	{
// --------------------
			switch(GetPRtable(&m_PrintCmInfo,ItCt) & 0xff)	{
// 24/04/04_341 cor <--
			case 0x50:
				nList[ItCt] = 1;
				break;
			case 0x51:
				nList[ItCt] = 2;	// 付表１･４
				break;
// 24/04/04_341 add -->
			case 0x80:
				// 税務代理権限証書
				nList[ItCt] = 100;
				break;
			case 0x81:
				// 税理士法第33条の2第1項に規定する添付書面（第1面
				nList[ItCt] = 102;
				break;
			case 0x82:
				// 税理士法第33条の2第1項に規定する添付書面（第2面）
				nList[ItCt] = 103;
				break;
			case 0x83:
				// 税理士法第33条の2第1項に規定する添付書面（第3面）
				nList[ItCt] = 104;
				break;
			case 0x84:
				// 計税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
				nList[ItCt] = 105;
				break;
			case 0x85:
				// 税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
				nList[ItCt] = 106;
				break;
			case 0x86:
				// 税理士法第33条の2第2項に規定する添付書面（第1面）
				nList[ItCt] = 107;
				break;
			case 0x87:
				// 税理士法第33条の2第2項に規定する添付書面（第2面）
				nList[ItCt] = 108;
				break;
			case 0x88:
				// 税理士法第33条の2第2項に規定する添付書面（第3面）
				nList[ItCt] = 109;
				break;
			case 0x89:
				// 税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
				nList[ItCt] = 110;
				break;
			case 0x90:
				// 税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）
				nList[ItCt] = 111;
				break;
// 24/04/04_341 add <--
			default:
				break;
			}
		}
		memset(Hpath, '\0', sizeof(Hpath));
		//if( m_pSnHeadData->Sn_KDAYS >= ICS_SH_MYNUMBER_REFORM_DAY ){
		//	sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27-2\\DBSYZCH00_H27-3.SCH"), PrgPath.GetBuffer());
		//}
		//else{
		//	sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H27\\DBSYZCH00_H27.SCH"), PrgPath.GetBuffer());
		//}
		sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBSYZCH00_H31.SCH"), PrgPath.GetBuffer());
		m_Vprn.m_ErrFlag = psch_inz(0x80, nList, 2, Hpath);
	}
	else{

		// kasai add -->
		// 課税取引・旧税率ありなしの切り替え
		BOOL syzSgn = FALSE;
		long eymd	= 0;
		if(!(m_pSnHeadData->Sn_SKKBN % 2))	{
			eymd = m_pSnHeadData->Sn_MDAYE;
		}
		else	{
			eymd = m_pSnHeadData->Sn_KDAYE;
		}
// 24/03/19_税務代理書面 add -->
		// 税務代理書面が使用できる期間は、特定収入の後ろに税務代理書面が選択されている
		// 場合がある
		if(eymd < R06_ZEIMUDAIRI)	{
			char	pt=GetPRtable(&m_PrintCmInfo,0);
			if( ((pt & 0x70) == 0x20) ||		// 還付申告の明細書（次葉）はスケジュールでは無
				((pt & 0x70) == 0x30) ||
				((pt & 0x70) == 0x40) ||
				((pt & 0x70) == 0x60) ||
				((pt & 0xff) == 0x52) ||
				((pt & 0xff) == 0x56) ){
				return(0);
			}
		}
// 24/03/19_税務代理書面 add <--
		if( m_pSnHeadData->SVmzsw == 1 || eymd < TAX_R04_SDAY ) {
			// 旧税率あり
			syzSgn = TRUE;
		}

		// 2023/07/13 h.wada add -->
		BYTE syzSgn2 = 0x00; //D0:令和5年10月1日以降に終了する課税期間用の帳票を使用する
		//　23.10.10課税取引金額計算表の制御を12/31→10/01切り替えに
		//	if (eymd >= TAX_R05_EDAY) {
		if (eymd >= ICS_SH_INVOICE_DAY) {
			syzSgn2 = 0x01;
		}
		// h.wada <--

// 220412 add -->
		// 令和4年4月1日以降の課税期間であれば、特例計算（5-3）のチェック欄がない
		// emfを使用したスケジュールを使用する
		BOOL	tok53 = FALSE;
		if(eymd >= TOK53MODIFY)	{
			tok53 = TRUE;
		}
		m_PnoKuchiTC = 0;
		m_PnoKuchiTM = 0;
		m_PnoKuchiHC = 0;
		m_PnoKuchiHM = 0;
// 220412 add <--

		// 初期化
		m_PnoJigyo		= 0;
		m_PnoFudousan	= 0;
		m_PnoNougyou	= 0;
		m_PnoSell		= 0;
		m_PnoBuy		= 0;
		// kasai add <--


		// カラープリンター設定
		if( m_Vprn._IsColorMode() == TRUE ){
			PcSw = 0;													// カラー　プリンター
		}
		else{
			PcSw = 1;													// モノクロプリンター
		}
		memset(Hpath, '\0', sizeof(Hpath));
		if( m_PrintCmInfo.PRtype&0x80 ){
			// イメージ出力
			// カラーモード
			m_Vprn.SetOptionMode(POF_IMGPOSCTL, 1, -1);
			m_Vprn.SetOptionMode(POF_POSADJUST, 0, -1);
			// 税理士法施行帳票
			if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
				// 原則
				if( m_pSnHeadData->IsSoleProprietor() ){
					// 個人
					//----->('23.05.01 Add)
					if ( eymd >= ICS_SH_INVOICE_DAY ) {
						//----> 20230616
						if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
							sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBSYZSHINIPPANZK_H31-4.sch"), PrgPath.GetBuffer());
						}
						//<---- 20230616
						else{
							sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBSYZSHINIPPANZK_H31-3.SCH"), PrgPath.GetBuffer());
						}
					}
					//<-----
					else if( eymd >= R04REFORM ) {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBSYZSHINIPPANZK_H31-2.SCH"), PrgPath.GetBuffer());
					}
					else {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBSYZSHINIPPANZK_H31.SCH"), PrgPath.GetBuffer());
					}
				}
				else{
					// 法人
					//----->('23.05.01 Add)
					if ( eymd >= ICS_SH_INVOICE_DAY ) {
						//----> 20230616
						if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
							sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBSYZSHINIPPANZH_H31-4.sch"), PrgPath.GetBuffer());
						}
						//<---- 20230616
						else{
							sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBSYZSHINIPPANZH_H31-3.SCH"), PrgPath.GetBuffer());
						}
					}
					//<-----
					else {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBSYZSHINIPPANZH_H31.SCH"), PrgPath.GetBuffer());
					}
				}
			}
			else{
				// 簡易課税
				if( m_pSnHeadData->IsSoleProprietor() ){
					// 個人
					//----->('23.05.01 Add)
					if ( eymd >= ICS_SH_INVOICE_DAY ) {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBSYZSHINKANIZK_H31-3.SCH"), PrgPath.GetBuffer());
					}
					//<-----
					else if( eymd >= R04REFORM ) {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBSYZSHINKANIZK_H31-2.SCH"), PrgPath.GetBuffer());
					}
					else {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBSYZSHINKANIZK_H31.SCH"), PrgPath.GetBuffer());
					}
				}
				else{
					// 法人
					//----->('23.05.01 Add)
					if ( eymd >= ICS_SH_INVOICE_DAY ) {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBSYZSHINKANIZH_H31-3.sch"), PrgPath.GetBuffer());
					}
					//<-----
					else {
						sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBSYZSHINKANIZH_H31.sch"), PrgPath.GetBuffer());
					}
				}
			}
		}
		else{
			// ＯＣＲ印刷
			// 開始位置調整
			m_Vprn.SetOptionMode(POF_POSADJUST, 0, -1);
			m_Vprn.SetOptionMode(POF_SIZEADJUST, 0, -1);
			if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
				if( eymd >= ICS_SH_INVOICE_DAY ){
					sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBIPPANKSK_H31-3.SCH"), PrgPath.GetBuffer());
				}
				else if(( eymd >= R04REFORM ) && ( m_pSnHeadData->IsSoleProprietor() )) {
					sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBIPPANKSK_H31-2.SCH"), PrgPath.GetBuffer());
				}
				else {
					sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBIPPANKSK_H31.SCH"), PrgPath.GetBuffer());
				}
			}
			else{
				if( eymd >= ICS_SH_INVOICE_DAY ){
					sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBKANIKKSK_H31-3.SCH"), PrgPath.GetBuffer());
				}
				else if(( eymd >= R04REFORM ) && ( m_pSnHeadData->IsSoleProprietor() )) {
					sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBKANIKKSK_H31-2.SCH"), PrgPath.GetBuffer());
				}
				else {
					sprintf_s(Hpath, sizeof(Hpath), _T("%s\\H31\\DBKANIKKSK_H31.SCH"), PrgPath.GetBuffer());
				}
			}
		}
		for( ItCt=0; ItCt!=128; ItCt++ ){
			nList[ItCt] = 0;
		}

		BOOL isNoneMTForm = TRUE;
		EnumIdIcsShKazeihoushiki kazei;
		kazei = m_Util.GetKazeihoushiki ( m_pZmSub->zvol );

		CH31HyoView	*pH31View = ( CH31HyoView* )( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( H31_HONPYO_WND_IDX ) );
		isNoneMTForm = pH31View->IsNoneMTForm();

		//// 一括印刷情報の取得
		//EnumIdShBatchPrintState	BpState = ID_ORIGINALSTART_BPSTATE;
		//SH_BPPRINT_INFO			BpPrintInfo = {0};
		//if(m_pMainFrame)	{
		//	m_pMainFrame->GetBatchPrintInfo(&BpState,&BpPrintInfo);
		//}

		//=== TEST ===
		//			PRtable[0] = 0x10;
		//			PRtable[0] = 0x01;
		//============
		//nListが昇順且つ出力順の必要があるため、後の事を考えて空き番を設けて再定義
		//for( ItCt = 0; ItCt != m_PrintCmInfo.PgMx; ItCt++ ) {
		//	switch( m_PrintCmInfo.PRtable[ItCt] & 0x7f ) {
// 24/03/18_税務代理書面 cor -->
		//for( ItCt = 0; ItCt != m_PrintCmInfo.PgMx2; ItCt++ ) {
		//	switch(GetPRtable(&m_PrintCmInfo,ItCt) & 0x7f)	{
// -----------------------------
		for(ItCt=0,ItCt2=0; ItCt2!=m_PrintCmInfo.PgMx2; ItCt2++)	{
			switch(GetPRtable(&m_PrintCmInfo,ItCt2) & 0xff)	{
// 24/03/18_税務代理書面 cor <--
			case 0x01:		// 本表（一般・簡易）
				if( (m_pSnHeadData->Sn_TYPE & 0x01) == 0x00 ) {
					// ＯＣＲ用紙
					nList[ItCt] = 1;
				}
				else	{
					// 白紙
					//if(GetActViewVer() == ID_VER_SYZ31)	{
						if(PcSw)	nList[ItCt]=2;	// モノクロ
						else		nList[ItCt]=1;	// カラー
					//}
					//else	{
					//	// モノクロ提出用
					//	if(!(m_pSnHeadData->Sn_TYPE & 0x02) || PcSw)	nList[ItCt]=2;
					//	// 改訂版
					//	else											nList[ItCt]=1;
					//}
				}
				break;

			case 0x03:	// 申告書本表「提出用モノクロ」
				//if(BpState && PcSw == 0)	nList[ItCt]=1;	// 一括印刷でカラーを指定
				//else						nList[ItCt]=2;
				// 2022/08/25
				// 詳細設定で「モノクロ」一括印刷で「カラー」と指定している場合
				// 一括印刷の設定どおりに出力されないが、従前からそうなっているため
				// ここでの指定はモノクロのままとする
				// 将来的には法人税ｄｂ、所得税ｄｂと同じように一括印刷での設定（カラー／モノクロ）を
				// 行えないようにすることを検討する
				nList[ItCt]=2;
				break;

			case 0x57: //特定課税仕入れがある場合の課税標準額等の内訳書
				// ＯＣＲ or モノクロ
				if(((m_pSnHeadData->Sn_TYPE & 0x01) == 0x00) || PcSw)	{
					if(tok53)	m_PnoKuchiTM = 8;
					else		m_PnoKuchiTM = 6;
					nList[ItCt] = m_PnoKuchiTM;
				}
				// 白紙
				else	{
					if(tok53)	m_PnoKuchiTC = 7;
					else		m_PnoKuchiTC = 5;
					nList[ItCt] = m_PnoKuchiTC;
				}
				break;

			case 0x05:	// 課税標準額等の内訳書「提出用モノクロ」
				//if(BpState && PcSw == 0)	{
				//	// 一括印刷でカラーを指定
				//	if(tok53)	m_PnoKuchiTC = 7;
				//	else		m_PnoKuchiTC = 5;
				//	nList[ItCt] = m_PnoKuchiTC;
				//}
				//else	{
					if(tok53)	m_PnoKuchiTM = 8;
					else		m_PnoKuchiTM = 6;
					nList[ItCt] = m_PnoKuchiTM;
				//}
				break;

			case 0x02:
				if(PcSw)	nList[ItCt] = 4;	// モノクロ
				else		nList[ItCt] = 3;	// カラー
				break;

			case 0x04:	// 申告書本表「控用」モノクロ
				//if(BpState && PcSw == 0)	nList[ItCt]=3;	// 一括印刷でカラーを指定
				//else						nList[ItCt]=4;
				nList[ItCt]=4;
				break;

			case 0x58: //特定課税仕入れがある場合の課税標準額等の内訳書(控用)
				if(!PcSw)	{
					// カラー
					if(tok53)	m_PnoKuchiHC = 15;
					else		m_PnoKuchiHC = 13;
					nList[ItCt] = m_PnoKuchiHC;
				}
				else	{
					// モノクロ
					if(tok53)	m_PnoKuchiHM = 16;
					else		m_PnoKuchiHM = 14;
					nList[ItCt] = m_PnoKuchiHM;
				}
				break;

			case 0x06:	// 課税標準額等の内訳書「控用」モノクロ
				//if(BpState && PcSw == 0)	{
				//	// 一括印刷でカラーを指定
				//	if(tok53)	m_PnoKuchiHC = 15;
				//	else		m_PnoKuchiHC = 13;
				//	nList[ItCt] = m_PnoKuchiHC;
				//}
				//else	{
					if(tok53)	m_PnoKuchiHM = 16;
					else		m_PnoKuchiHM = 14;
					nList[ItCt] = m_PnoKuchiHM;
				//}
				break;

			case 0x70:
				// 付表１－１　付表４－１ 
				nList[ItCt] = 20;
				break;
			case 0x71:
				// 付表１－２　付表４－２
				nList[ItCt] = 21;
				break;
			case 0x17:
				// 付表１－３　付表４－３
				//----> 20230616
				if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
					// 付表５－１
					nList[ItCt] = 25;
				}
				//<---- 20230616
				else{
					nList[ItCt] = 22;
				}
				break;
			case 0x72:
				// 付表２－１	付表５－１
				//nList[ItCt] = 25;
				//---->20230120
				if ( ( kazei == ID_ICSSH_GENSOKU ) && ( eymd >= ICS_SH_INVOICE_DAY ) ) {
					//----> 20230616
					if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
						// 付表５－１(２)
						nList[ItCt] = 26;
					}
					//<---- 20230616
					else{
						// 付表２－１
						nList[ItCt] = 31;
					}
				}
				//<----20230120
				else if ( ( kazei == ID_ICSSH_GENSOKU ) && ( ( m_ShRevType >= ID_ICSSH_KYOJU_REVTYPR ) && ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) ) ) {
					// 付表２－１
					nList[ItCt] = 27;
				}
				else {
					// 付表２－１	付表５－１
					nList[ItCt] = 25;
				}
				break;
			case 0x73:
				// 付表２－２	付表５－１(２)
				//nList[ItCt] = 26;
				//---->20230120
				if ( ( kazei == ID_ICSSH_GENSOKU ) && ( eymd >= ICS_SH_INVOICE_DAY ) ) {
					//----> 20230616
					if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
						// 付表５－２
						nList[ItCt] = 27;
					}
					//<---- 20230616
					else{
						// 付表２－２
						nList[ItCt] = 32;
					}
				}
				//<----20230120
				else if ( ( kazei == ID_ICSSH_GENSOKU ) && ( ( m_ShRevType >= ID_ICSSH_KYOJU_REVTYPR ) && ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) ) ) {
					// 付表２－２
					nList[ItCt] = 28;
				}
				else {
					// 付表２－２	付表５－１(２)
					nList[ItCt] = 26;
				}
				break;
			case 0x18:
				// 付表２－３	付表５－３
				//---->20230120
				if ( ( kazei == ID_ICSSH_GENSOKU ) && ( eymd >= ICS_SH_INVOICE_DAY ) ) {
					//----> 20230616
					if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
						// 付表５－２(２)
						nList[ItCt] = 28;
					}
					//<---- 20230616
					else{
						nList[ItCt] = 33;
					}
				}
				//<----20230120
				else {
					nList[ItCt] = 30;
				}
				break;
			case 0x19:
				// 付表５－３（２）
				nList[ItCt] = 31;
				break;
			case 0x74:
				//				付表５－２
				nList[ItCt] = 27;
				break;
			case 0x75:
				//			　	付表５－２(２)
				nList[ItCt] = 28;
				break;
			case 0x16:
				//				付表６
				//nList[ItCt] = 35;
				if ( m_pSnHeadData->Sn_GenericSgn & 0x20 ) {
					nList[ItCt] = 35;
				}
				else {
					nList[ItCt] = 36;
				}
				break;
			//----->('23.05.01 Add)
			case 0x79:
				//				付表６（税率別消費税額計算表）
				nList[ItCt] = 34;
				break;
			//<-----
			case 0x76:
				//				特例１
				nList[ItCt] = 60;
				break;
			case 0x77:
				//				特例２
				nList[ItCt] = 61;
				break;
			case 0x78:
				//				特例３
				nList[ItCt] = 62;
				break;
			case 0x07:
				// 還付申告明細書①
				nList[ItCt] = 50;
				break;
			case 0x08:
				// 還付申告明細書②
				if( eymd >= ICS_SH_INVOICE_DAY ){
					nList[ItCt] = 53;
				}
				else{
					nList[ItCt] = 51;
				}
				break;
			case 0x09:
				//更正の請求
				if( eymd >= KSKMODIFY ){
					nList[ItCt] = 56;
				}
				else{
					nList[ItCt] = 55;
				}
				break;
			case 0x10:
				if( m_Util.IsBasicMst(m_pZmSub->zvol) ) {
					//nList[ItCt] = 73;
					if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
						
						//nList[ItCt] = 83;
						if (syzSgn2 & 0x01) {
							m_PnoSell = 98;
						}
						else if( syzSgn ){
							// 旧税率あり
							m_PnoSell = 83;
						}
						else{
							// 旧税率なし
							m_PnoSell = 93;
						}
						nList[ItCt] = m_PnoSell;
					}
					else {
						nList[ItCt] = 73;
					}

///					if( curset.SVmzsw == 0x00 ) {
//						// 課税売上高計算表
//						nList[ItCt] = 60;
//					}
//					else {
//						// 課税売上高計算表 経過措置
//						nList[ItCt] = 65;
//					}
				}
				break;
			case 0x11:
				if( m_Util.IsBasicMst(m_pZmSub->zvol) ) {
					//nList[ItCt] = 74;
					if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {

						//nList[ItCt] = 84;
						//--->0724 戎居
						if (syzSgn2 & 0x01) {
							m_PnoBuy = 99;
						}
						else if( syzSgn ){
							// 旧税率あり
							m_PnoBuy = 84;
						}
						else{
							// 旧税率なし
							m_PnoBuy = 94;
						}
						nList[ItCt] = m_PnoBuy;
					}
					else {
						nList[ItCt] = 74;
					}
					//if( curset.SVmzsw == 0x00 ) {
					//	// 課税仕入高計算表
					//	nList[ItCt] = 61;
					//}
					//else {
					//	// 課税仕入高計算表 経過措置
					//	nList[ItCt] = 66;
					//}
				}
				break;
			case 0x12:
				if( m_Util.IsBasicMst(m_pZmSub->zvol) ) {
					//nList[ItCt] = 70;
					if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {

						//nList[ItCt] = 80;
						// 2023/07/13 h.wada change --> 課税取引金額計算表(令和5年10月1日以降に終了する課税期間用)のID選択処理を追加
						if (syzSgn2 & 0x01) {
							m_PnoJigyo = 95;
						}
						else if( syzSgn ){
							// 旧税率あり
							m_PnoJigyo = 80;
						}
						else{
							// 旧税率なし
							m_PnoJigyo = 90;
						}
						// h.wada change <--
						nList[ItCt] = m_PnoJigyo;
					}
					else {
						nList[ItCt] = 70;
					}
					//if( curset.SVmzsw == 0x00 ) {
					//	// 課税取引金額計算表 事業所得
					//	nList[ItCt] = 62;
					//}
					//else {
					//	// 課税取引金額計算表 事業所得 経過措置
					//	nList[ItCt] = 67;
					//}
				}
				break;
			case 0x13:
				if( m_Util.IsBasicMst(m_pZmSub->zvol) ) {
					//nList[ItCt] = 72;
					if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
// 211203h del -->
						//nList[ItCt] = 82;
// 211203h del <--
// 2023/07/13 h.wada change --> 課税取引金額計算表(令和5年10月1日以降に終了する課税期間用)のID選択処理を追加
						if (syzSgn2 & 0x01) {
						//	m_PnoFudousan = 96;
							m_PnoFudousan = 97;
						}
// 211203h add -->					
						else if( syzSgn )	{
							// 旧税率あり
							m_PnoFudousan = 82;
						}
						else	{
							// 旧税率なし
							m_PnoFudousan = 92;
						}
						nList[ItCt] = m_PnoFudousan;
// 211203h add <--
// h.wada change <--
					}
					else {
						nList[ItCt] = 72;
					}
					//if( curset.SVmzsw == 0x00 ) {
					//	// 課税取引金額計算表 不動産所得
					//	nList[ItCt] = 63;
					//}
					//else {
					//	// 課税取引金額計算表 不動産所得 経過措置
					//	nList[ItCt] = 68;
					//}
				}
				break;
			case 0x14:
				if( m_Util.IsBasicMst(m_pZmSub->zvol) ) {
					//nList[ItCt] = 71;
					if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
// 211203h del -->
						//nList[ItCt] = 81;
// 211203h del <--
// 2023/07/13 h.wada change --> 課税取引金額計算表(令和5年10月1日以降に終了する課税期間用)のID選択処理を追加
						if (syzSgn2 & 0x01) {
						//	m_PnoNougyou = 97;
							m_PnoNougyou = 96;
						}
// 211203h add -->
						else if( syzSgn )	{
							// 旧税率あり
							m_PnoNougyou = 81;
						}
						else	{
							// 旧税率なし
							m_PnoNougyou = 91;
						}
						nList[ItCt] = m_PnoNougyou;
// 211203h add <--
// h.wada change <--
					}
					else {
						nList[ItCt] = 71;
					}
					//if( curset.SVmzsw == 0x00 ) {
					//	// 課税取引金額計算表 農業所得
					//	nList[ItCt] = 64;
					//}
					//else {
					//	// 課税取引金額計算表 農業所得 経過措置
					//	nList[ItCt] = 69;
					//}
				}
				break;

// 24/03/18_税務代理書面 add -->
			case 0x80:
				// 税務代理権限証書
				nList[ItCt] = 100;
				break;
			case 0x81:
				// 税理士法第33条の2第1項に規定する添付書面（第1面
				nList[ItCt] = 102;
				break;
			case 0x82:
				// 税理士法第33条の2第1項に規定する添付書面（第2面）
				nList[ItCt] = 103;
				break;
			case 0x83:
				// 税理士法第33条の2第1項に規定する添付書面（第3面）
				nList[ItCt] = 104;
				break;
			case 0x84:
				// 計税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
				nList[ItCt] = 105;
				break;
			case 0x85:
				// 税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
				nList[ItCt] = 106;
				break;
			case 0x86:
				// 税理士法第33条の2第2項に規定する添付書面（第1面）
				nList[ItCt] = 107;
				break;
			case 0x87:
				// 税理士法第33条の2第2項に規定する添付書面（第2面）
				nList[ItCt] = 108;
				break;
			case 0x88:
				// 税理士法第33条の2第2項に規定する添付書面（第3面）
				nList[ItCt] = 109;
				break;
			case 0x89:
				// 税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
				nList[ItCt] = 110;
				break;
			case 0x90:
				// 税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）
				nList[ItCt] = 111;
				break;
// 24/03/18_税務代理書面 add <--

			default:
// 24/03/19_税務代理書面 cor -->
				//break;
// -----------------------------
				continue;
// 24/03/19_税務代理書面 cor <--
			}
// 24/03/19_税務代理書面 add -->
			ItCt++;
// 24/03/19_税務代理書面 add <--
		}
// 24/03/19_税務代理書面 add -->
		// 税務代理書面が使用できる期間は、特定収入の後ろに税務代理書面が選択されている
		// 場合があるので、ここで特定収入のみかの判定を行う
		if(ItCt == 0)	return(0);
// 24/03/19_税務代理書面 add <--
		//※nListの中身は必ず昇順且つ出力順であること！
		m_Vprn.m_ErrFlag = psch_inz(0x80, nList, 2, Hpath);
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 印刷スケジュールの読み込み
//-------------------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-------------------------------------------------------------------------------------
int CDBSyzShinMainView::ReadXYSPrintSch()
{
	int	rt = 0;
	if( m_PrintType == 0 ){
		//if( ((m_PrintCmInfo.PRtable[0]&0x70) == 0x20) ||		// 還付申告の明細書（次葉）はスケジュールでは無
		//	((m_PrintCmInfo.PRtable[0]&0x70) == 0x30) ||
		//	((m_PrintCmInfo.PRtable[0]&0x70) == 0x40) ||
		//	((m_PrintCmInfo.PRtable[0]&0xff) == 0x52) ){
// 24/03/19_税務代理書面 del -->
//		char	pt=GetPRtable(&m_PrintCmInfo,0);
//		if( ((pt & 0x70) == 0x20) ||		// 還付申告の明細書（次葉）はスケジュールでは無
//			((pt & 0x70) == 0x30) ||
//			((pt & 0x70) == 0x40) ||
//// 24/02/27wd_特定収入対応 cor -->
//			//((pt & 0xff) == 0x52) ){
//// -------------------------------
//			((pt & 0x70) == 0x60) ||
//			((pt & 0xff) == 0x52) ||
//			((pt & 0xff) == 0x56) ){
//// 24/02/27wd_特定収入対応 cor <--
//			return rt;
//		}
// 24/03/19_税務代理書面 del <--

		// 改正様式
		EnumIdIcsShRevType shrevtype = GetShRevType();

		CString	PrgPath;
		if( m_pMainFrame ){
			PrgPath = m_pMainFrame->GetPrgPath();
		}

		if( shrevtype >= ID_ICSSH_10PER_REVTYPE ){
			rt = ReadXYSPrintSch_R01(shrevtype, PrgPath);
		}
		else{
// 24/03/19_税務代理書面 add -->
			char	pt=GetPRtable(&m_PrintCmInfo,0);
			if( ((pt & 0x70) == 0x20) ||		// 還付申告の明細書（次葉）はスケジュールでは無
				((pt & 0x70) == 0x30) ||
				((pt & 0x70) == 0x40) ||
				((pt & 0xff) == 0x52) ){
				return rt;
			}
// 24/03/19_税務代理書面 add <--
			rt = ReadXYSPrintSch_H26(shrevtype, PrgPath);
		}
	}
	return rt;
}

void CDBSyzShinMainView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
	m_Vprn.PrePrint( 1, pDC, MM_HIMETRIC );

	//マイナンバー履歴の出力
	if( IsPrintKojinNo() == TRUE ){
		if( pInfo->m_bPreview == TRUE ){
			WriteNoHistory( 0 );
		}
		else{
			WriteNoHistory( 1 );
		}
	}

	// スケジュール印刷の時化ジュールの読み込み
	ReadXYSPrintSch();

	m_Printsw = 1;	// 印刷開始 ('14.03.13)
}

void CDBSyzShinMainView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	ICSFormView::OnPrepareDC(pDC, pInfo);

	if( pDC->IsPrinting() != 0 ){
		// 伸縮（２６号様式以外の申告書印刷で手差しのみ）
		if( !m_PrintType && (m_pSnHeadData->IsMiddleProvisional()==FALSE) && !(m_PrintCmInfo.PRtype&0x80) ) {
			m_Vprn.SetOptionMode( POF_SIZEADJUST, MAKELONG(0, m_PrintCmInfo.OPEla), -2 );
		}
		if( m_pSnHeadData->IsMiddleProvisional() ){
			// 前年実績による中間申告（第２６号様式）
			PageNo = m_Vprn.GetCurPageNo();
			if(PageNo < (m_PrintCmInfo.PgMx + 1))	{
// 24/04/04_341 cor -->
				////if( (m_PrintCmInfo.PRtable[PageNo-1]&0x7f) == 0x50 ){
				//if((GetPRtable(&m_PrintCmInfo,PageNo-1) & 0x7f) == 0x50)	{
				//	m_Pset.psOrientation = DMORIENT_PORTRAIT;
				//	m_Vprn.ResetPrtparam( 0, pDC, &m_Pset, MM_HIMETRIC );
				//}
				////if( (m_PrintCmInfo.PRtable[ PageNo - 1 ] & 0x7f) == 0x51 ) {
				//if((GetPRtable(&m_PrintCmInfo,PageNo-1) & 0x7f) == 0x51)	{
				//	m_Pset.psOrientation = DMORIENT_LANDSCAPE;
				//	m_Vprn.ResetPrtparam( 0, pDC, &m_Pset, MM_HIMETRIC );
				//}
// --------------------
				if((GetPRtable(&m_PrintCmInfo,PageNo-1) & 0xff) == 0x51)	{
					m_Pset.psOrientation = DMORIENT_LANDSCAPE;
					m_Vprn.ResetPrtparam( 0, pDC, &m_Pset, MM_HIMETRIC );
				}
				else	{
					m_Pset.psOrientation = DMORIENT_PORTRAIT;
					m_Vprn.ResetPrtparam( 0, pDC, &m_Pset, MM_HIMETRIC );
				}
// 24/04/04_341 cor <--
			}
		}
		else {
			long eymd = 0;
			if (!(m_pSnHeadData->Sn_SKKBN % 2)) {
				eymd = m_pSnHeadData->Sn_MDAYE;
			}
			else {
				eymd = m_pSnHeadData->Sn_KDAYE;
			}
			//　23.10.10課税取引金額計算表の制御を12/31→10/01切り替えに
			//	if (eymd >= TAX_R05_EDAY) {
			if (eymd >= ICS_SH_INVOICE_DAY) {
				PageNo = m_Vprn.GetCurPageNo();
				if(PageNo < (m_PrintCmInfo.PgMx + 1))	{
					if ((GetPRtable(&m_PrintCmInfo, PageNo - 1) & 0x7f) == 0x12) {
						m_Pset.psOrientation = DMORIENT_LANDSCAPE;
						m_Vprn.ResetPrtparam(0, pDC, &m_Pset, MM_HIMETRIC);
					}
					else if ((GetPRtable(&m_PrintCmInfo, PageNo - 1) & 0x7f) == 0x13) {
						m_Pset.psOrientation = DMORIENT_LANDSCAPE;
						m_Vprn.ResetPrtparam(0, pDC, &m_Pset, MM_HIMETRIC);
					}
					else if ((GetPRtable(&m_PrintCmInfo, PageNo - 1) & 0x7f) == 0x14) {
						m_Pset.psOrientation = DMORIENT_LANDSCAPE;
						m_Vprn.ResetPrtparam(0, pDC, &m_Pset, MM_HIMETRIC);
					}
					else{
						m_Pset.psOrientation = DMORIENT_PORTRAIT;
						m_Vprn.ResetPrtparam(0, pDC, &m_Pset, MM_HIMETRIC);
					}
				}
			}

		}


		m_Vprn.InitPage();
	}
}

void CDBSyzShinMainView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
	m_Printsw = 0;	// 印刷終了 ('14.03.13)
	int		PrEnd = 0;

	m_Vprn.EndPrint();																										// m_VPrintは、CVPrintクラスのオブジェクト変数

//--> '15.03.18 INS START
	if( (m_Vprn.IsPrintFromPreview()==TRUE) || (m_Vprn.PreviewOnPrintdlg()==1) ){
		m_PrintCmInfo.OP_PSEL |= 0x04;
	}
	else{
		m_PrintCmInfo.OP_PSEL &= ~0x04;
	}
//<-- '15.03.18 INS END

	if( m_PrintType == 0 ){
// 24/07/10_個別タブから出力 cor -->
		////if( ((m_PrintCmInfo.PRtable[0]&0x70) == 0x20) ||																	// 還付申告の明細書（次葉）はスケジュールでは無
		////	((m_PrintCmInfo.PRtable[0]&0x70) == 0x30) ||																		
		////	((m_PrintCmInfo.PRtable[0]&0x70) == 0x40) ||															
		////	((m_PrintCmInfo.PRtable[0]&0xff) == 0x52) ){
		//char	pt=GetPRtable(&m_PrintCmInfo,0);
		//if( ((pt & 0x70) == 0x20) ||																	// 還付申告の明細書（次葉）はスケジュールでは無
		//	((pt & 0x70) == 0x30) ||																		
		//	((pt & 0x70) == 0x40) ||															
		//	((pt & 0xff) == 0x52) ){
		//	return;																
		//}
		//PrEnd = m_Vprn.IsContinueMode();		
		//psch_end();
// ---------------------------------
		// 一括印刷で特定収入計算書のみを指定して出力した場合、業務が終了しない現象が起こっていたため修正
		PrEnd = m_Vprn.IsContinueMode();
		if(nList[0] != 0)	{
			psch_end();
		}
// 24/07/10_個別タブから出力 cor <--
		// 一括印刷情報の取得
		EnumIdShBatchPrintState	BpState = ID_ORIGINALSTART_BPSTATE;
		SH_BPPRINT_INFO			BpPrintInfo = {0};
		if( m_pMainFrame ){
			m_pMainFrame->GetBatchPrintInfo( &BpState, &BpPrintInfo );
		}
		// 一括印刷
		if( BpState ){
			if( !(m_PrintCmInfo.PRtype&0x80) ) {																			// ＯＣＲ印刷
				if( m_PrintCmInfo.PgMx != KdNo ){
					return;
				}
			}
			if( (BpState==ID_NORMALPRINT_BPSTATE) || (BpState==ID_PDFPRINT_BPSTATE) ){
				if( m_Vprn.m_ErrFlag  ==  0 ) {
					AfxGetMainWnd()->PostMessageA( WM_CLOSE, 0 );
				}
			}
		}
	}

	if ( GetActViewVer() == ID_VER_SYZ31 ) {
		CH31HyoView			*pH31HyoView = NULL;
		pH31HyoView = (CH31HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(1));
		if (pH31HyoView != NULL) {
			pH31HyoView->CloseSyzTokurei();
		}
	}

//--> '16.01.07 INS START
	if( (m_Vprn.IsPrintFromPreview()==TRUE) || (m_Vprn.PreviewOnPrintdlg()==1) ){
		// プレビュー→印刷ダイアログ、または印刷ダイアログ→プレビューの時
	}
	else{
		// フォーカスセット
		AfxGetMainWnd()->PostMessage( WM_USER_REFOCUS_PRINT, 0, 0 );
	}
//<-- '16.01.07 INS END
}

//2015.08.04 UPDATE START
//void CDBSyzShinMainView::OnPrint(CDC* pDC, CPrintInfo*)
void CDBSyzShinMainView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
//2015.08.04 UPDATE END
{
	// TODO: コントロールを印刷するコードを追加してください。
	int		PrEnd;
	int		m_PcSw = 0 , PP = 1;
	long	dotx=0, doty=0;
// 24/03/18_税務代理書面 add -->
	int		yflg=0,yno=0;
	int		electronic=0;
	int		opt1=0,opt2=0,opt3=0;
	int		syorikbn=0;
	int		sn_tetuzuki_kbn=0;
	int		stffid=0;
	CString	strSaki=_T("");
// 24/04/01_246 del -->
	//KMNCTL_GETSKINFPAC	kmn;
// 24/04/01_246 del <--
// 24/03/18_税務代理書面 add <--
// 24/04/04_モジュール仕様変更 add -->
	R_HEAD	rh;
	R_ZEIRI	rz;
// 24/04/04_モジュール仕様変更 add <--
// 24/04/04_341 add -->
	int		ofs=0;
// 24/04/04_341 add <--

	if(	m_PrintType	== 1){
		// 還付申告の明細書財務連動チェックリスト
			if( m_Vprn.StartPageJob( pDC ) != 0 )				return;																// 財務連動は共用する
			if( m_Vprn.SetCoMode( 1 ) != 0 )					return;																// 座標モード
			PageNo = m_Vprn.GetCurPageNo();																						// ページ番号の取得
			PrEnd = SkjCheckList( PageNo );																							// 出力
			if( PrEnd )			m_Vprn.LastPage();
			if( nList[0] != -1 )	m_Vprn.DeleteList( nList[0] );
			nList[0] = -1;
	}
	else if( m_PrintType == 0 ){	// 申告書
		PrEnd = m_Vprn.IsContinueMode();
		if( m_Vprn.StartPageJob(pDC) != 0 ){
			return;
		}
		if( m_Vprn.m_ErrFlag ){
			switch( m_Vprn.m_ErrFlag ){
				case -1:	
					m_Vprn.ErrorAbort( 0, "ｲﾒｰｼﾞﾌｧｲﾙの初期設定に失敗しました。" );
					break;
				case -2:	
					m_Vprn.ErrorAbort( 0, "ｲﾒｰｼﾞﾌｧｲﾙの出力に失敗しました。" );
					break;
				case -3:	
					m_Vprn.ErrorAbort( 0, "ｲﾒｰｼﾞﾌｧｲﾙの設定解除に失敗しました。" );
					break;
				case 1 :	
					//m_Vprn.ErrorAbort( 0, "出力スケジュールファイルがオープン出来ません。" );
					m_Vprn.ErrorAbort( 0, "出力スケジュールファイルがオープンできません。" );
					break;
				case 2 :	
					//m_Vprn.ErrorAbort( 0, "出力スケジュールファイルの読込が出来ません。" );
					m_Vprn.ErrorAbort( 0, "出力スケジュールファイルの読込ができません。" );
					break;
				case 3 :	
					//m_Vprn.ErrorAbort( 0, "イメージファイルが検索出来ません。" );
					m_Vprn.ErrorAbort( 0, "イメージファイルが検索できません。" );
					break;
				case 4 :	
					m_Vprn.ErrorAbort( 0, "メモリ領域が十分ではありません。" );
					break;
				case 5 :	
					m_Vprn.ErrorAbort( 0, "出力スケジュールファイルの記述に不都合があります。" );
					break;
				case 6 :	
					m_Vprn.ErrorAbort( 0, "フォントの作成に失敗しました。" );
					break;
				default:	
					m_Vprn.ErrorAbort( 0, "印刷準備中に不都合があります。" );
					break;
			}
			// 一括印刷エラーサインセット
			SetBpPrintErr();
			return;
		}
		if( m_Vprn.SetCoMode(1) != 0 ){	// 座標モード
			return;
		}

		CH31HyoView			*pH31HyoView = NULL;
		CH26HyoView			*pH26HyoView = NULL;
		if (GetActViewVer() == ID_VER_SYZ31) {
			// 31 印刷処理
			pH31HyoView = (CH31HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(1)); //初回起動時NULLが返送されるので注意
		}
		else {	
			pH26HyoView = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(2));
		}

		// 改正様式
		EnumIdIcsShRevType shrevtype = GetShRevType();

		if( m_pSnHeadData->IsMiddleProvisional() ){
			// 前年実績による中間申告（第２６号様式）
			PageNo = m_Vprn.GetCurPageNo();
// 24/04/04_341 cor -->
			////switch( m_PrintCmInfo.PRtable[PageNo-1] & 0x7f ){
			//switch(GetPRtable(&m_PrintCmInfo,PageNo-1) & 0x7f)	{
// --------------------
			switch(GetPRtable(&m_PrintCmInfo,PageNo-1) & 0xff)	{
// 24/04/04_341 cor <--
				case 0x50 :	// 第２６号様式
					if( pH26HyoView ){
						PrEnd = pH26HyoView->data_mast( pDC );
						pH26HyoView->Lin_Print();
					}
					if (pH31HyoView) {
						PrEnd = pH31HyoView->PrintChukan(pDC);
						pH31HyoView->Lin_Print();
					}

					break;
				case 0x51 :	// 第２６号様式Ａ４ヨコ控用
					if( pH26HyoView ){
						PrEnd = pH26HyoView->data_mast_EX( pDC );
					}
					if (pH31HyoView) {
						PrEnd = pH31HyoView->PrintChukanEx(pDC);
					}
					break;
// 24/04/04_341 add -->
				case 0x80:	// 税務代理権限証書
				case 0x81:	// 税理士法第33条の2第1項に規定する添付書面（第1面
				case 0x82:	// 税理士法第33条の2第1項に規定する添付書面（第2面）
				case 0x83:	// 税理士法第33条の2第1項に規定する添付書面（第3面）
				case 0x84:	// 計税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
				case 0x85:	// 税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
				case 0x86:	// 税理士法第33条の2第2項に規定する添付書面（第1面）
				case 0x87:	// 税理士法第33条の2第2項に規定する添付書面（第2面）
				case 0x88:	// 税理士法第33条の2第2項に規定する添付書面（第3面）
				case 0x89:	// 税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
				case 0x90:	// 税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）
					// ------------------------------------------------
					// 代理権限証書に印字微調整値を反映させる
					// ------------------------------------------------
					HP  = m_PrMv[AD_ZEIMUDAIRI]._PIHad;
					VP  = m_PrMv[AD_ZEIMUDAIRI]._PIVad;
					m_PcSw = MAKEWORD(1,1);
					ofs=GetPrOfs(GetPRtable(&m_PrintCmInfo,PageNo-1));
					_xy_offs(ofs,0,(long)(HP),(long)(VP),m_PcSw);
					_xy_offs(ofs,1,(long)(HP),(long)(VP),m_PcSw);

					// ------------------------------------------------
					// 印刷関数に渡す引数のセット
					// ------------------------------------------------
					// 呼び出し元業務
					if(m_Util.IsUnConnectMst(m_pZmSub->zvol))	yflg=1;	// 消費税申告書（非連動）
					else										yflg=0;	// 消費税申告書
					// 出力帳票
					yno = GetPRtable(&m_PrintCmInfo,PageNo-1);
					if((yno & 0xff) == 0x80)		yno = 0x01;		// 税務代理権限証書
					else if((yno & 0xff) == 0x81)	yno = 0x02;		// 申告書の作成に関する計算事項記載書面 33の2(1) (1面)
					else if((yno & 0xff) == 0x82)	yno = 0x04;		// 申告書の作成に関する計算事項記載書面 33の2(1) (2面)
					else if((yno & 0xff) == 0x83)	yno = 0x08;		// 申告書の作成に関する計算事項記載書面 33の2(1) (3面)
					else if((yno & 0xff) == 0x84)	yno = 0x10;		// 申告書の作成に関する計算事項記載書面 33の2(1) (4面 1枚目) 
					else if((yno & 0xff) == 0x85)	yno = 0x20;		// 申告書の作成に関する計算事項記載書面 33の2(1) (4面 2枚目)
					else if((yno & 0xff) == 0x86)	yno = 0x40;		// 申告書に関する審査事項記載書面 33の2(2) (1面)
					else if((yno & 0xff) == 0x87)	yno = 0x80;		// 申告書に関する審査事項記載書面 33の2(2) (2面)
					else if((yno & 0xff) == 0x88)	yno = 0x100;	// 申告書に関する審査事項記載書面 33の2(2) (3面)
					else if((yno & 0xff) == 0x89)	yno = 0x200;	// 申告書に関する審査事項記載書面 33の2(2) (4面 1枚目)
					else if((yno & 0xff) == 0x90)	yno = 0x400;	// 申告書に関する審査事項記載書面 33の2(2) (4面 2枚目)
					// 提出先名
					strSaki = _T("");
					// 電子申告済みを印字する
					if(Zeiri_DButton_sw & 0x04)	{
						electronic = 1;
					}
					// 印刷項目　依頼者：氏名又は名称
					if(Zeiri_DButton_sw & 0x01)	{
						opt1 = 1;
					}
					// 印刷項目　依頼者：住所又は事業所の所在地
					if(Zeiri_DButton_sw & 0x02)	{
						opt2 = 1;
					}
					// 給与処理ｄｂ　訂正区分
					syorikbn = 1;
					// 給与処理ｄｂ　異動届出書の種類
					sn_tetuzuki_kbn = 0;
					// 給与処理ｄｂ　社員番号
					stffid = 0;
					// 
					PrEnd = 0;
					if(m_GetSki != 1)	electronic = 0;
					m_Fnt108gt = m_Vprn.RegisterFont(108,0,0,_T("ＭＳ ゴシック"));
					m_Fnt090gt = m_Vprn.RegisterFont( 90,0,0,_T("ＭＳ ゴシック"));
					ZtenpSetFontSize(m_Fnt108gt,m_Fnt090gt);
					if(m_pSnHeadData->m_Zrkbn == 2)	{
						// 税理士情報が所属税理士
						if(m_pSnHeadData->m_ZroutSgn & 0x01)	{
							// 税理士区分を出力
							opt3=opt3|0x01;
						}
						if(m_pSnHeadData->m_ZroutSgn & 0x02)	{
							// 直接受任を出力
							opt3=opt3|0x02;
						}
					}
					m_pMainFrame->GetZeiriInfo(&rh,&rz);
					// ここは予定26号の出力（本表・付表は別にあるのでそちらの変更も忘れずに！）
					if(ZtenpPrint(m_pZmSub->m_database,&m_Vprn,pDC,yflg,yno,strSaki,electronic,opt1,opt2,syorikbn,sn_tetuzuki_kbn,stffid,&m_Kanryo,this,opt3,0,&rh,&rz) == -1)	{
						PrEnd = 1;
					}
					m_Vprn.DeleteRegFont(m_Fnt108gt);
					m_Vprn.DeleteRegFont(m_Fnt090gt);
					break;
// 24/04/04_341 add <--
				default   :	
					PrEnd = 1;
					break;
			}
			if( PrEnd ){
				m_Vprn.LastPage();
			}
			else{
				// 次ページ　印刷終了チェック
				if( PageNo == m_PrintCmInfo.PgMx ){
					m_Vprn.LastPage();
				}
			}
		}
		else{
			// 指定ページの作成

			if((!( m_PrintCmInfo.PRtype&0x80 ))&&( pInfo->m_bPreview == FALSE )){
				PageNo = m_Vprn.GetCurKindNo();
			}
			else{
				PageNo = m_Vprn.GetCurPageNo();
			}

			//---->20230120
			long symd = 0, eymd = 0;
			if( pH31HyoView ){
				pH31HyoView->GetKazeiKikan( symd, eymd );
			}
			//<----20230120

			PrEnd = 0;
			//if(( (m_PrintCmInfo.PRtable[0]&0x7f) < 0x20 )
			//	||((m_PrintCmInfo.PRtable[0]&0x7f)==0x57)
			//	||((m_PrintCmInfo.PRtable[0]&0x7f)==0x58)
			//	||(((m_PrintCmInfo.PRtable[0]&0x7f)>=0x70)&&((m_PrintCmInfo.PRtable[0]&0x7f)<0x80))){
// 24/03/19_税務代理書面 cor -->
			//char	pt=GetPRtable(&m_PrintCmInfo,0);
			//if(((pt & 0x7f) < 0x20) || ((pt & 0x7f) == 0x57) || ((pt & 0x7f) == 0x58) || 
			//	(((pt & 0x7f) >= 0x70) && ((pt & 0x7f) < 0x80))){
// -----------------------------
			char	pt=GetPRtable(&m_PrintCmInfo,PageNo-1);
			int		sw=1;
			if( ((pt & 0x70) == 0x20) ||		// 還付申告の明細書（次葉）はスケジュールでは無
				((pt & 0x70) == 0x30) ||
				((pt & 0x70) == 0x40) ||
				((pt & 0x70) == 0x60) ||
				((pt & 0xff) == 0x52) ||
				((pt & 0xff) == 0x56) ){
				sw=0;
			}
			if(sw != 0){
// 24/03/19_税務代理書面 cor <--
				// 出力開始位置設定
				AdSw = 0;
				//switch( m_PrintCmInfo.PRtable[PageNo-1]&0x7f ){
// 24/03/19_税務代理書面 cor -->
				//switch(GetPRtable(&m_PrintCmInfo,PageNo-1) & 0x7f)	{
// -----------------------------
				switch(GetPRtable(&m_PrintCmInfo,PageNo-1) & 0xff)	{
// 24/03/19_税務代理書面 cor <--
					case 0x01 :	// 本表　提出用
					case 0x02 :	// 本表　控用
						if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) != ID_ICSSH_KANNI ){
							AdSw = AD_IPPAN;
						}
						else{
							AdSw = AD_KANI;
						}
						break;
					case 0x03 :	// 付表（１．４）
						if(GetActViewVer() == ID_VER_SYZ31)	{
							// 10%以降は 0x03=申告書本表「提出用」モノクロ
							if(m_Util.GetKazeihoushiki(m_pZmSub->zvol) != ID_ICSSH_KANNI)	AdSw=AD_IPPAN;
							else															AdSw=AD_KANI;
						}
						else	{
							if(m_Util.GetKazeihoushiki(m_pZmSub->zvol) != ID_ICSSH_KANNI)	AdSw=1;
							else															AdSw=5;
						}
						break;
					case 0x70: // 付表（１－１、４－１）
					case 0x71: // 付表（１－２、４－２）
						if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) != ID_ICSSH_KANNI) {
							//----> 20230616
							if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
								AdSw = AD_FHYO4;
							}
							//<---- 20230616
							else{
								AdSw = AD_FHYO1;
							}
						}
						else {
							AdSw = AD_FHYO4;
						}
						break;
					//----> 20230616
					case 0x17: // 付表（１－３、４－３）
						if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) != ID_ICSSH_KANNI) {
							if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
								AdSw = AD_FHYO5;
							}
							else{
								AdSw = AD_FHYO1;
							}
						}
						else {
							AdSw = AD_FHYO4;
						}
						break;
					//<---- 20230616
					case 0x04 :	// 付表（２．５）
						if(GetActViewVer() == ID_VER_SYZ31)	{
							// 10%以降は 0x03=申告書本表「控用」モノクロ
							if(m_Util.GetKazeihoushiki(m_pZmSub->zvol) != ID_ICSSH_KANNI)	AdSw=AD_IPPAN;
							else															AdSw=AD_KANI;
						}
						else	{
							if(m_Util.GetKazeihoushiki(m_pZmSub->zvol) != ID_ICSSH_KANNI)	AdSw=2;
							else															AdSw=6;
						}	
						break;
					case 0x72:	// 付表（２－１、５－１）
					case 0x73:	// 付表（２－２、５－１(２)）
					case 0x18:  // 付表（２－３、５－３（１））
					case 0x19:  // 付表（５－３（２））
						if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) != ID_ICSSH_KANNI) {
							//----> 20230616
							if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
								AdSw = AD_FHYO5;
							}
							//<---- 20230616
							else{
								AdSw = AD_FHYO2;
							}
						}
						else {
							AdSw = AD_FHYO5;
						}
						break;
					case 0x05 :	// 付表（２－２．５－２）
						if(GetActViewVer() == ID_VER_SYZ31)	{
							// 10%以降は 0x05=課税標準額等の内訳書「提出用」モノクロ
							AdSw=AD_IPPAN2;
							break;
						}
					case 0x15 :
						if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) != ID_ICSSH_KANNI ){
							AdSw = 3;
						}
						else{
							AdSw = 7;
						}
						break;
					case 0x74: // 付表（５－２）
					case 0x75: // 付表（５－２(２)）
						//AdSw = 19;
						AdSw = AD_FHYO5;
						break;
					case 0x06 :	// 仕入控除税額に関する明細書又は消費税の還付申告に関する明細書
						if(GetActViewVer() == ID_VER_SYZ31)	{
							// 10%以降は 0x05=課税標準額等の内訳書「提出用」モノクロ
							AdSw=AD_IPPAN2;
							break;
						}
					case 0x07 :	// 消費税の還付申告に関する明細書(1/2)
					case 0x08 :	// 消費税の還付申告に関する明細書(2/2)
						if( m_pSnHeadData->IsSoleProprietor() ){
							AdSw = AD_KANPU_K;
						}
						else{
							AdSw = AD_KANPU_H;
						}
						break;
					case 0x09 :	// 更正の請求
						if( m_pSnHeadData->IsSoleProprietor() ){
							AdSw = AD_KOSEI_K;
						}
						else{
							AdSw = AD_KOSEI_H;
						}
						break;
					case 0x10 : // 課税売上高計算表
					case 0x11 : // 課税仕入高計算表
					case 0x12 : // 課税取引金額計算表 事業所得	
					case 0x13 : // 課税取引金額計算表 不動産所得				
					case 0x14 : // 課税取引金額計算表 農業所得					
						if( m_pSnHeadData->IsSoleProprietor() ){
							AdSw = AD_KAZEI;
						}
						break;
					case 0x16 : //付表６
						AdSw = AD_FHYO6;
						break;
					case 0x20 :	// 仕入控除税額に関する明細書又は消費税の還付申告に関する明細書（次葉）
					case 0x30 :	// 主な課税資産の譲渡等
					case 0x31 :	// 主な輸出取引等の明細
					case 0x32 :	// 主な棚卸資産・原材料等の取得
					case 0x33 :	// 主な固定資産等の取得
						if( m_pSnHeadData->IsSoleProprietor() ){
							AdSw = 13;
						}
						else{
							AdSw = 13;
						}
						break;
					case 0x57 : //特定課税仕入
					case 0x58 : //特定課税仕入
						if( m_pSnHeadData->IsNeedTkkzDisp() ){
							AdSw = AD_IPPAN2;
						}
						else {
							if ( GetActViewVer() == ID_VER_SYZ31 ) {
								AdSw = AD_IPPAN2;
							}
						}
						break;
					case 0x76: //特例１　課税資産の譲渡等の対価の額の計算表１
						AdSw = AD_TOKUREI1;
						break;
					case 0x77: //特例２　課税資産の譲渡等の対価の額の計算表２
						AdSw = AD_TOKUREI2;
						break;
					case 0x78: //特例３　課税仕入れ等の税額の計算表
						AdSw = AD_TOKUREI3;
						break;
					case 0x79: // 付表６（税率別消費税額計算表）
					//----->('23.05.01 Add)
						AdSw = AD_FHYO6_2wari;
						break;
					//<-----
// 24/03/19_税務代理書面 add -->
					case 0x80:	// 税務代理権限証書
					case 0x81:	// 税理士法第33条の2第1項に規定する添付書面（第1面
					case 0x82:	// 税理士法第33条の2第1項に規定する添付書面（第2面）
					case 0x83:	// 税理士法第33条の2第1項に規定する添付書面（第3面）
					case 0x84:	// 税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
					case 0x85:	// 税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
					case 0x86:	// 税理士法第33条の2第2項に規定する添付書面（第1面）
					case 0x87:	// 税理士法第33条の2第2項に規定する添付書面（第2面）
					case 0x88:	// 税理士法第33条の2第2項に規定する添付書面（第3面）
					case 0x89:	// 税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
					case 0x90:	// 税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）
						AdSw = AD_ZEIMUDAIRI;
						break;
// 24/03/19_税務代理書面 add <--
					default   :
						PrEnd = 1;
						break;
				}

				HP  = m_PrMv[AdSw]._PIHad;
				VP  = m_PrMv[AdSw]._PIVad;
				DHP = m_PrMv[AdSw]._PKHad;
				DVP = m_PrMv[AdSw]._PKVad;

				// ＯＣＲ用紙
				if( !(m_PrintCmInfo.PRtype&0x80) ){
					_xy_offs( PageNo - 1, 1, (long)(DHP), (long)(DVP), PcSw );
					//switch( m_PrintCmInfo.PRtable[PageNo-1]&0x7f ){
					switch(GetPRtable(&m_PrintCmInfo,PageNo-1) & 0x7f)	{
						case 0x01 : //本表
							PP = 1;
							break;
						case 0x57 : //内訳書
							if ( GetActViewVer() == ID_VER_SYZ31 ) {
								PP = m_PnoKuchiTM;	// OCRはモノクロ
							}
							else {
								PP = 51;
							}
							break;
					}
				}
				// 白紙
				else	{
					m_PcSw = PcSw;
					//// 一括印刷情報の取得
					//EnumIdShBatchPrintState	BpState = ID_ORIGINALSTART_BPSTATE;
					//SH_BPPRINT_INFO			BpPrintInfo = {0};
					//if(m_pMainFrame)	{
					//	m_pMainFrame->GetBatchPrintInfo(&BpState,&BpPrintInfo);
					//}
					//switch( m_PrintCmInfo.PRtable[PageNo-1]&0x7f ){
					switch(GetPRtable(&m_PrintCmInfo,PageNo-1) & 0x7f)	{
						case 0x01 :
							// ９５％ルール改正対応期間サイン
							if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
								//if( !(m_pSnHeadData->Sn_TYPE&0x02) || PcSw ){	// モノクロ提出用
								// モノクロ提出用（10%以降は0x01=必ずカラー）
								if( (!(m_pSnHeadData->Sn_TYPE&0x02) || PcSw) && 
									(GetActViewVer() != ID_VER_SYZ31) )	{

									if (GetActViewVer() == ID_VER_SYZ31) {
										m_PcSw = MAKEWORD( 1, 1 );
									}
									else {
										if (m_TaxReforms) {
											m_PcSw = MAKEWORD(2, 2); 	// 改正後	
										}
										else {
											//こっちはもう入らない
											m_PcSw = MAKEWORD(1, 1);	// 改正前
										}
									}
									PP = 2;
								}
								else{
									if (GetActViewVer() == ID_VER_SYZ31) {
										m_PcSw = MAKEWORD( 1, 1 );
									}
									else {
										if (m_TaxReforms) {
											if (m_pSnHeadData->IsSoleProprietor()) {
												m_PcSw = MAKEWORD(4, 4); 	// 閉じ込まない・未出力
											}
											else {
												m_PcSw = MAKEWORD(3, 3);	// 閉じ込まない・出力
											}
										}
										else {
											//こっちはもう入らない
											if (m_pSnHeadData->IsSoleProprietor()) {
												m_PcSw = MAKEWORD(2, 2); 	// 閉じ込まない・未出力
											}
											else {
												m_PcSw = MAKEWORD(1, 1);	// 閉じ込まない・出力
											}
										}
									}
									if(PcSw)	PP = 2;
									else		PP = 1;
								}
							}
							else{
								//if( !(m_pSnHeadData->Sn_TYPE&0x02) || PcSw ){
								// モノクロ提出用（10%以降は0x01=必ずカラー）
								if( (!(m_pSnHeadData->Sn_TYPE&0x02) || PcSw) && 
									(GetActViewVer() != ID_VER_SYZ31) )	{

									m_PcSw = MAKEWORD( 1, 1 );	// モノクロ提出用
									PP = 2;
								}
								else{
									if (GetActViewVer() == ID_VER_SYZ31) {
										m_PcSw = MAKEWORD( 1, 1 );
									}
									else {
										if (m_pSnHeadData->IsSoleProprietor()) {
											m_PcSw = MAKEWORD(4, 4);	// 閉じ込まない・未出力
										}
										else {
											m_PcSw = MAKEWORD(1, 1);	// 閉じ込まない・出力
										}
									}
									if(PcSw)	PP = 2;
									else		PP = 1;
								}
							}
							break;

						case 0x03:
							// 10%以降は0x03=申告書本表「提出用」モノクロ
							if(GetActViewVer() == ID_VER_SYZ31)	{
								m_PcSw = MAKEWORD(1,1);
								//if(BpState && PcSw == 0)	PP=1;	// 一括印刷でカラーを指定
								//else						PP=2;
								PP=2;
								break;
							}

						case 0x02 : // 控用
							//if( !PcSw && (m_pSnHeadData->Sn_TYPE&0x04) ){
							// カラー控用（10%以降は0x02=必ずカラー）
							if((!PcSw && (m_pSnHeadData->Sn_TYPE&0x04)) || (GetActViewVer() == ID_VER_SYZ31))	{

								// カラー
								if (GetActViewVer() == ID_VER_SYZ31) {
									m_PcSw = MAKEWORD( 1, 1 );
								}
								else {
									if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
										if (m_TaxReforms) {
											m_PcSw = MAKEWORD(4, 4);	// 閉じ込まない・未出力
										}
										else {
											//こっちはもう入らない
											m_PcSw = MAKEWORD(2, 2);	// 閉じ込まない・未出力
										}
									}
									else {
										if (m_pSnHeadData->IsSoleProprietor()) {
											m_PcSw = MAKEWORD(2, 2);	// 閉じ込まない・未出力
										}
										else {
											m_PcSw = MAKEWORD(4, 4);	// 閉じ込まない・未出力
										}
									}
								}
								if(GetActViewVer() == ID_VER_SYZ31)	{
									if(PcSw)	PP=4;	// モノクロ
									else		PP=3;	// カラー
								}
								else	{
									PP=3;	// カラー
								}
							}
							else{
								// モノクロ
								//if (GetActViewVer() == ID_VER_SYZ31) {
								//	m_PcSw = MAKEWORD( 1, 1 );
								//}
								//else {
									if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
										if (m_TaxReforms) {
											m_PcSw = MAKEWORD(2, 2); 	// 改正後
										}
										else {
											//こっちはもう入らない
											m_PcSw = MAKEWORD(1, 1);	// 改正前
										}
										//if (GetActViewVer() == ID_VER_SYZ31) {
										//	PP = 9;
										//}
										//else {
										//	PP = 3;
										//}
									}
									else {
										//									if( m_pSnHeadData->IsSoleProprietor() ){
										m_PcSw = MAKEWORD(1, 1); 	// 個人控用
									}
								//}
								PP = 4;
							}
							break;
						case 0x04 :	// 付表（２．５）
							// 10%以降は0x04=申告書本表「控用」モノクロ
							if(GetActViewVer() == ID_VER_SYZ31)	{
								m_PcSw = MAKEWORD(1,1);
								//if(BpState && PcSw == 0)	PP=3;	// 一括印刷でカラーを指定
								//else						PP=4;
								PP=4;
								break;
							}

						case 0x05 :	// 付表（２－２．５－２）
							// 10%以降は0x05=課税標準額等の内訳書「提出用」モノクロ
							if(GetActViewVer() == ID_VER_SYZ31)	{
								//if(BpState && PcSw == 0)	PP = m_PnoKuchiTC;	// 一括印刷でカラーを指定
								//else						PP = m_PnoKuchiTM;
								PP = m_PnoKuchiTM;
								break;
							}

						//case 0x72 :	// 付表（２－１）
						case 0x73 :	// 付表（２－１）
						case 0x18:  // 付表（２－３）
							if( m_TaxReforms && (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_GENSOKU) ){
								m_PcSw = MAKEWORD( 1, 1 );	// 平成２３年消費税改正版
							}
							break;
						case 0x72:
							if( m_TaxReforms && (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_GENSOKU) ){
								m_PcSw = MAKEWORD( 1, 1 );	// 平成２３年消費税改正版
							}
							else {
								if ( GetActViewVer() == ID_VER_SYZ31 ) {
									if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
										m_PcSw = MAKEWORD( 2, 2 );
									}
									else {
										m_PcSw = MAKEWORD( 1, 1 );
									}
								}
							}
							break;
						case 0x06 :	// 仕入控除税額に関する明細書
							// 10%以降は0x04=申告書本表「控用」モノクロ
							if(GetActViewVer() == ID_VER_SYZ31)	{
								//if(BpState && PcSw == 0)	PP = m_PnoKuchiHC;	// 一括印刷でカラーを指定
								//else						PP = m_PnoKuchiHM;
								PP = m_PnoKuchiHM;
							}
							else	{
								if(!PcSw)	m_PcSw = MAKEWORD(1,1);
								else		m_PcSw = MAKEWORD(2,2);
							}
							break;
						case 0x09 :	// 更正の請求
							if( m_pSnHeadData->Sn_Syuusei&0x04 ){
								m_PcSw = MAKEWORD( 2, 2 );
							}
							else{
								m_PcSw = MAKEWORD( 1, 1 );
							}
							break;
						case 0x16 : // 付表６
							//----->インボイス施工日以降は付表７を出力する('23.07.10 cor)
							if (eymd >= ICS_SH_INVOICE_DAY) {
								m_PcSw = MAKEWORD(2, 2);
							}
							else {
								m_PcSw = MAKEWORD(1, 1);
							}
							//<-----
							break;
						case 0x19:	// 付表5-3(2)
							m_PcSw = MAKEWORD( 1, 1 );
							break;
						case 0x57 : //特定課税仕入れがある場合の課税標準額等の内訳書　提出用
							if(GetActViewVer() == ID_VER_SYZ31)	{
								if(PcSw)	PP = m_PnoKuchiTM;
								else		PP = m_PnoKuchiTC;
							}
							else {
								if( !(m_pSnHeadData->Sn_TYPE&0x02) || PcSw ){
									//モノクロ
									//PP = 28;
									PP = 51;
								}else{
									//カラー
									//PP = 27;
									PP = 50;
								}
							}
							break;
						case 0x58 : //特定課税仕入れがある場合の課税標準額等の内訳書　控用
							if (GetActViewVer() == ID_VER_SYZ31) {
								if(PcSw)	PP = m_PnoKuchiHM;
								else		PP = m_PnoKuchiHC;
							}
							else {
								if (!PcSw && (m_pSnHeadData->Sn_TYPE & 0x04)) {
									//カラー
									//PP = 29;
									PP = 55;
								}
								else {
									//モノクロ
									//PP = 30;
									PP = 56;
								}
							}
							break;
						case 0x70:
							if ( GetActViewVer() == ID_VER_SYZ31 ) {
								//----> 20230120
								if( eymd >= ICS_SH_INVOICE_DAY ){
									//----> 20230616
									// 簡易、または原則で2割特例・経過措置ありの場合は付表4-1
									if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI || ((m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw) ){
										// 付表4-1
										m_PcSw = MAKEWORD ( 2, 2 );
									}
									//<---- 20230616
									else{
										// 付表1-1
										m_PcSw = MAKEWORD ( 1, 1 );
									}
								}
								//<---- 20230120
								else if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
									m_PcSw = MAKEWORD ( 2, 2 );
								}
								else {
									m_PcSw = MAKEWORD ( 1, 1 );
								}
							}
							break;
						case 0x71:
							if ( GetActViewVer() == ID_VER_SYZ31 ) {
								if ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_GENSOKU ) {
									//----> 20230120
									if( eymd >= ICS_SH_INVOICE_DAY ){
										m_PcSw = MAKEWORD ( 1, 1 );
									}
									//<---- 20230120
									else if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
										m_PcSw = MAKEWORD ( 2, 2 );
									}
									else {
										m_PcSw = MAKEWORD ( 1, 1 );
									}
								}
								else {
									m_PcSw = MAKEWORD ( 1, 1 );
								}
							}
							break;
						case 0x74:
							if ( GetActViewVer() == ID_VER_SYZ31 ) {
								if ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_GENSOKU ) {
									if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
										m_PcSw = MAKEWORD ( 2, 2 );
									}
									else {
										m_PcSw = MAKEWORD ( 1, 1 );
									}
								}
								else {
									m_PcSw = MAKEWORD ( 1, 1 );
								}
							}
							break;
					//----->('23.05.01 Add)
						case 0x79: // 付表６（税率別消費税額計算表）
							m_PcSw = MAKEWORD(1, 1);
							break;
					//<-----
						//----> 20230616
						case 0x17:	// 2割特例・経過措置ありの場合のときだけ付表5-1とする
							if ( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ) {
								m_PcSw = MAKEWORD( 2, 2 );
							}
							else{
								// 付表1-3、4-3
								m_PcSw = MAKEWORD( 1, 1 );
							}
							break;
						//<---- 20230616

						default:
							// 課税取引金額計算表等 
							m_PcSw = MAKEWORD( 1, 1 );
							break;
					}
//-- '15.11.24 --
//					if(	(m_PrintCmInfo.PRtable[PageNo-1]&0x7f) < 0x20 ){
//---------------
//					if(	((m_PrintCmInfo.PRtable[PageNo-1]&0x7f)<0x20) || ((m_PrintCmInfo.PRtable[PageNo-1]&0x7f)==0x57) || ((m_PrintCmInfo.PRtable[PageNo-1]&0x7f)==0x58) ){
					// 10%改正対応
					//if(	((m_PrintCmInfo.PRtable[PageNo-1]&0x7f)<0x20) || ((m_PrintCmInfo.PRtable[PageNo-1]&0x7f)==0x57) || ((m_PrintCmInfo.PRtable[PageNo-1]&0x7f)==0x58) || (((m_PrintCmInfo.PRtable[PageNo-1]&0x7f)>=0x70)&&((m_PrintCmInfo.PRtable[PageNo-1]&0x7f)<=0x75)) ){
					//if(	((m_PrintCmInfo.PRtable[PageNo-1]&0x7f)<0x20) || ((m_PrintCmInfo.PRtable[PageNo-1]&0x7f)==0x57) || ((m_PrintCmInfo.PRtable[PageNo-1]&0x7f)==0x58) || (((m_PrintCmInfo.PRtable[PageNo-1]&0x7f)>=0x70)&&((m_PrintCmInfo.PRtable[PageNo-1]&0x7f)<=0x78)) ){
//---------------
					//	_xy_offs( PageNo-1, 0, (long)(HP), (long)(VP), m_PcSw );
					//	_xy_offs( PageNo-1, 1, (long)(HP), (long)(VP), m_PcSw );
					//}
					char	pt=GetPRtable(&m_PrintCmInfo,PageNo-1);
				//----->('23.05.01 cor)
				//	if(((pt & 0x7f) < 0x20) || ((pt & 0x7f) == 0x57) || ((pt & 0x7f) == 0x58) || (((pt & 0x7f) >= 0x70) && ((pt & 0x7f) <= 0x78)))	{
				//------
					//付表６のprt_noまで範囲拡大
// 24/03/19_税務代理書面 cor -->
					//if(((pt & 0x7f) < 0x20) || ((pt & 0x7f) == 0x57) || ((pt & 0x7f) == 0x58) || (((pt & 0x7f) >= 0x70) && ((pt & 0x7f) <= 0x79))) {
// -----------------------------
					if( ((pt & 0x7f) < 0x20) || ((pt & 0x7f) == 0x57) || ((pt & 0x7f) == 0x58) || (((pt & 0x7f) >= 0x70) && ((pt & 0x7f) <= 0x79)) || 
						((pt & 0xff) >= 0x80)) {
// 24/03/19_税務代理書面 cor <--
				//<-----
// 24/04/04_341 del -->
						//int	ofs=0,busu=0;
// 24/04/04_341 del <--
						if(GetActViewVer() == ID_VER_SYZ31)	{
							ofs=GetPrOfs(pt);
						}
						else	{
							ofs=PageNo-1;
						}
						_xy_offs( ofs, 0, (long)(HP), (long)(VP), m_PcSw );
						_xy_offs( ofs, 1, (long)(HP), (long)(VP), m_PcSw );
					}
				}
			}
			unsigned char kojin_sgn = m_pSnHeadData->Sn_KOJIN;
			if( (m_pZmSub->zvol->apno&0xf0) == 0x10 ){
				kojin_sgn = 0x01;
			}

			CTaxationList31View	*pTaxlist31View = NULL;
			CTaxationListView	*pTaxlistView = NULL;
			if (GetActViewVer() == ID_VER_SYZ31) {
				//if( IsPrintTaxationList(m_PrintCmInfo.PRtable[PageNo-1]&0x7f) ){
				if(IsPrintTaxationList(GetPRtable(&m_PrintCmInfo,PageNo-1) & 0x7f))	{
					pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));
				}
				//else if( IsPrintSpcList(m_PrintCmInfo.PRtable[PageNo-1]&0x7f) ){
				else if(IsPrintSpcList(GetPRtable(&m_PrintCmInfo,PageNo-1) & 0x7f))	{
					pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));
				}
			}
			else{
				//if( IsPrintTaxationList(m_PrintCmInfo.PRtable[PageNo-1]&0x7f) ){
				if(IsPrintTaxationList(GetPRtable(&m_PrintCmInfo,PageNo-1) & 0x7f))	{
					pTaxlistView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(3));
				}
				//else if( IsPrintSpcList(m_PrintCmInfo.PRtable[PageNo-1]&0x7f) ){
				else if(IsPrintSpcList(GetPRtable(&m_PrintCmInfo,PageNo-1) & 0x7f))	{
					pTaxlistView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(3));
				}
			}
//<-- '15.02.13 INS END

			//switch( m_PrintCmInfo.PRtable[PageNo-1]&0x7f ){	// 0x3f
// 24/03/18_税務代理書面 cor -->
			//switch(GetPRtable(&m_PrintCmInfo,PageNo-1) & 0x7f)	{	// 0x3f
// -----------------------------
			switch(GetPRtable(&m_PrintCmInfo,PageNo-1) & 0xff)	{	// 0x3f
// 24/03/18_税務代理書面 cor <--
				case 0x01 :	// 本表提出用
					if( pH26HyoView ){
//						if( pH26HyoView->PrintHonPyo(pDC, 0, PRno[PageNo-1]) ){
						if( pH26HyoView->PrintHonPyo(pDC, FALSE, PP , m_PcSw/*不要？？*/) ){
							PrEnd = 1;
						}
					}
					if (pH31HyoView) {
						if (pH31HyoView->PrintHonPyo(pDC, FALSE, PP )) {
							PrEnd = 1;
						}
					}
					break;
				case 0x02 :	// 本表控用
//					if( HonPyoPrint( pDC, 1, PRno[PageNo-1] ) ){
					if( pH26HyoView ){
						if( pH26HyoView->PrintHonPyo(pDC, TRUE, PP , m_PcSw) ){
							PrEnd = 1;
						}
					}
					if (pH31HyoView) {
						if (pH31HyoView->PrintHonPyo(pDC, TRUE, PP )) {
							PrEnd = 1;
						}
					}
					break;
				case 0x03 :	// 付表（１．４）
					if( pH26HyoView ){
//						if( pH26HyoView->PrintFuhyo10( pDC, kojin_sgn, PRno[PageNo-1] ) ){
//2015.08.04 UPDATE START
//						if( pH26HyoView->PrintFuhyo10(pDC, kojin_sgn, 1) ){
//							PrEnd = 1;
//						}
//-- '15.08.27 --
//						if( m_TokuteiFlg == TRUE ){
//---------------
//--- '15.11.24 ---
//						if( shrevtype >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//-----------------
						if( (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_GENSOKU) && (shrevtype>=ID_ICSSH_TKKZSIIRE_REVTYPE) ){
//-----------------
//---------------
							if( pH26HyoView->PrintFuhyo10Ex(pDC, kojin_sgn, 1) ){
								PrEnd = 1;
							}
						}else{
							if( pH26HyoView->PrintFuhyo10(pDC, kojin_sgn, 1) ){
								PrEnd = 1;
							}
						}
//2015.08.04 UPDATE END
					}
					// 10%以降は0x03=申告書本表「提出用」モノクロ
					else	{
						if(pH31HyoView->PrintHonPyo(pDC,FALSE,PP))	{
							PrEnd = 1;
						}
					}
					break;
				case 0x70: //付表１－１ 付表４－１
					if( pH31HyoView ){
						if( m_Util.GetKazeihoushiki(m_pZmSub->zvol)== ID_ICSSH_GENSOKU){
							//----> 20230616
							if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
								if( pH31HyoView->PrintFuhyo4_01(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}
							//<---- 20230616
							else{
								if( pH31HyoView->PrintFuhyo1_01(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}
						}
						else {
							if(m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
								if (pH31HyoView->PrintFuhyo4_01(pDC, kojin_sgn, 1)) {
									PrEnd = 1;
								}
							}
						}
					}
					break;
				case 0x71: //付表１－２　付表４－２
					if (pH31HyoView) {
						if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
							//----> 20230616
							if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
								if( pH31HyoView->PrintFuhyo4_02(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}
							//<---- 20230616
							else{
								if (pH31HyoView->PrintFuhyo1_02(pDC, kojin_sgn, 1)) {
									PrEnd = 1;
								}
							}
						}
						else						
						if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
							if (pH31HyoView->PrintFuhyo4_02(pDC, kojin_sgn, 1)) {
								PrEnd = 1;
							}
						}
					}
					break;
				case 0x17:
					if ( pH31HyoView ) {
						if ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_GENSOKU ) {
							//----> 20230616
							if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
								if( pH31HyoView->PrintFuhyo5_011(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}
							//<---- 20230616
							else{
								if ( pH31HyoView->PrintFuhyo1_03 ( pDC, kojin_sgn, 1 ) ) {
									PrEnd = 1;
								}
							}
						}
						else {
							if ( pH31HyoView->PrintFuhyo4_03 ( pDC, kojin_sgn, 1 ) ) {
								PrEnd = 1;
							}
						}
					}
					break;
				case 0x04 :	// 付表（２．５）
					// 10%以降は0x04=申告書本表「控用」モノクロ
					if(pH31HyoView)	{
						if(pH31HyoView->PrintHonPyo(pDC,TRUE,PP))	{
							PrEnd = 1;
						}
						break;
					}
					if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
						if( pH26HyoView ){
//							if( pH26HyoView->PrintFuhyo50(pDC, kojin_sgn, PRno[PageNo-1]) ){

							//---->150120
							if( m_SixKindFlg == TRUE ){
								if( pH26HyoView->PrintFuhyo50_EX(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}
							else{
								if( pH26HyoView->PrintFuhyo50(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}
							//<-----------------

							//150120
							//if( pH26HyoView->PrintFuhyo50(pDC, kojin_sgn, 1) ){
							//	PrEnd = 1;
							//}
						}
					}
					else{
						if( pH26HyoView ){
//							if( pH26HyoView->PrintFuhyo20(pDC, kojin_sgn, PRno[PageNo-1]) ){
//2015.08.04 UPDATE START
//							if( pH26HyoView->PrintFuhyo20(pDC, kojin_sgn, 1) ){
//								PrEnd = 1;
//							}
//-- '15.08.27 --
//							if( m_TokuteiFlg == TRUE ){
//---------------
							if( shrevtype >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//---------------
								if( pH26HyoView->PrintFuhyo20Ex(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}else{
								if( pH26HyoView->PrintFuhyo20(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}
//2015.08.04 UPDATE END
						}
					}
					break;
				case 0x72:
					if( pH31HyoView ){
							if( m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_GENSOKU ){
								//if( pH31HyoView->PrintFuhyo2_01(pDC, kojin_sgn, 1) ){
								//	PrEnd = 1;
								//}
								//---->20230120
								if ( eymd >= ICS_SH_INVOICE_DAY ) {
									//----> 20230616
									if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
										if( pH31HyoView->PrintFuhyo5_012(pDC, kojin_sgn, 1) ){
											PrEnd = 1;
										}
									}
									//<---- 20230616
									else{
										if( pH31HyoView->PrintFuhyo2_11Ex ( pDC, kojin_sgn, 1 ) ) {
											PrEnd = 1;
										}
									}
								}
								//<----20230120
								else if ( ( m_ShRevType >= ID_ICSSH_KYOJU_REVTYPR ) && ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) ) {
									if ( pH31HyoView->PrintFuhyo2_11 ( pDC, kojin_sgn, 1 ) ) {
										PrEnd = 1;
									}
								}
								else {
									if (pH31HyoView->PrintFuhyo2_01 ( pDC, kojin_sgn, 1 ) ) {
										PrEnd = 1;
									}
								}
							}
							else {
								if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
									if (pH31HyoView->PrintFuhyo5_011(pDC, kojin_sgn, 1)) {
										PrEnd = 1;
									}
								}
							}
						}
					break;
				case 0x73:
					if( pH31HyoView ){
						if( m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_GENSOKU ){
							//---->20230120
							if ( eymd >= ICS_SH_INVOICE_DAY ) {
								//----> 20230616
								if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
									if( pH31HyoView->PrintFuhyo5_021(pDC, kojin_sgn, 1) ){
										PrEnd = 1;
									}
								}
								//<---- 20230616
								else{
									if( pH31HyoView->PrintFuhyo2_12Ex ( pDC, kojin_sgn, 1 ) ) {
										PrEnd = 1;
									}
								}
							}
							//<----20230120
							else if ( ( m_ShRevType >= ID_ICSSH_KYOJU_REVTYPR ) && ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) ) {
								if( pH31HyoView->PrintFuhyo2_12 ( pDC, kojin_sgn, 1 ) ) {
									PrEnd = 1;
								}
							}
							else {
								if( pH31HyoView->PrintFuhyo2_02(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}
						}
						else {
							if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
								if (pH31HyoView->PrintFuhyo5_012(pDC, kojin_sgn, 1)) {
									PrEnd = 1;
								}
							}
						}
					}
					break;
				case 0x18:
					if ( pH31HyoView ) {
						if ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_GENSOKU ) {
							//---->20230120
							if ( eymd >= ICS_SH_INVOICE_DAY ) {
								//----> 20230616
								if( (m_pSnHeadData->m_DispTabSgn & 0x2000) && m_pSnHeadData->SVmzsw ){
									if( pH31HyoView->PrintFuhyo5_022(pDC, kojin_sgn, 1) ){
										PrEnd = 1;
									}
								}
								//<---- 20230616
								else{
									if ( pH31HyoView->PrintFuhyo2_03Ex ( pDC, kojin_sgn, 1 ) ) {
										PrEnd = 1;
									}
								}
							}
							//<----20230120
							else{
								if ( pH31HyoView->PrintFuhyo2_03 ( pDC, kojin_sgn, 1 ) ) {
									PrEnd = 1;
								}
							}
						}
						else {
							if ( pH31HyoView->PrintFuhyo5_031 ( pDC, kojin_sgn, 1 ) ) {
								PrEnd = 1;
							}
						}
					}
					break;
				case 0x19:
					if ( pH31HyoView ) {
						if ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_GENSOKU ) {
						}
						else {
							if ( pH31HyoView->PrintFuhyo5_032 ( pDC, kojin_sgn, 1 ) ) {
								PrEnd = 1;
							}
						}
					}
				case 0x74:
					if (pH31HyoView) {
						if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
							if (pH31HyoView->PrintFuhyo5_021(pDC, kojin_sgn, 1)) {
								PrEnd = 1;
							}
						}
					}
					break;
				case 0x75:
					if (pH31HyoView) {
						if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
							if (pH31HyoView->PrintFuhyo5_022(pDC, kojin_sgn, 1)) {
								PrEnd = 1;
							}
						}
					}
					break;
				case 0x05 :	// 付表（２－２．５－２）
					// 10%以降は0x05=課税標準額等の内訳書「提出用」モノクロ
					if(pH31HyoView)	{
						if(pH31HyoView->PrintHonPyo2(pDC,FALSE,PP))	{	// FALSE=提出用
							PrEnd = 1;
						}
						break;
					}
					if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
						if( pH26HyoView ){
//							if( pH26HyoView->PrintFuhyo52(pDC, kojin_sgn, PRno[PageNo-1]) ){

							//---->150120
							if( m_SixKindFlg == TRUE ){
								if( pH26HyoView->PrintFuhyo52_1_EX(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}
							else{
								if( pH26HyoView->PrintFuhyo52_1(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}
							//<-----------------

							//150120
							//if( pH26HyoView->PrintFuhyo52_1(pDC, kojin_sgn, 1) ){
							//	PrEnd = 1;
							//}
						}
					}
					else{
						if( pH26HyoView ){
//							if( pH26HyoView->PrintFuhyo22(pDC, kojin_sgn, PRno[PageNo-1]) ){
//2015.08.04 UPDATE START
//							if( pH26HyoView->PrintFuhyo22(pDC, kojin_sgn, 1) ){
//								PrEnd = 1;
//							}
//-- '15.08.27 --
//							if( m_TokuteiFlg == TRUE ){
//---------------
							if( shrevtype >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//---------------
								if( pH26HyoView->PrintFuhyo22Ex(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}else{
								if( pH26HyoView->PrintFuhyo22(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}
//2015.08.04 UPDATE END
						}
//テスト用
						if( pH31HyoView ){
							if( m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_GENSOKU ){
								if( pH31HyoView->PrintFuhyo2_02(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}
						}
//テスト用
					}
					break;
				case 0x06 :	// 仕入控除税額に関する明細書又は消費税の還付申告に関する明細書①
					// 10%以降は0x06=課税標準額等の内訳書「控用」モノクロ
					if(pH31HyoView)	{
						if(pH31HyoView->PrintHonPyo2(pDC,TRUE,PP))	{	// TRUE=控用
							PrEnd = 1;
						}
					}
					break;
				case 0x07 :	// 仕入控除税額に関する明細書又は消費税の還付申告に関する明細書①
					if( pH26HyoView ){
						if( m_pSnHeadData->IsSoleProprietor() ){
							pH26HyoView->PrintKojinRefund( pDC, 1 );
						}
						else{
							pH26HyoView->PrintHojinRefund( pDC, 1 );
						}
					}
					if (pH31HyoView) {
						if (m_pSnHeadData->IsSoleProprietor()) {
							pH31HyoView->PrintRefund_Kojin(pDC, 1);
						}
						else {
							pH31HyoView->PrintRefund_Hojin(pDC, 1);
						}
					}
					break;
				case 0x08 :	// 消費税の還付申告に関する明細書②
					if( pH26HyoView ){
						if( m_pSnHeadData->IsSoleProprietor() ){
							pH26HyoView->PrintKojinRefund( pDC, 2 );
						}
						else{
							pH26HyoView->PrintHojinRefund( pDC, 2 );
						}
					}
					if (pH31HyoView) {
						if (m_pSnHeadData->IsSoleProprietor()) {
							pH31HyoView->PrintRefund_Kojin(pDC, 2);
						}
						else {
							pH31HyoView->PrintRefund_Hojin(pDC, 2);
						}
					}
					break;
				case 0x09 :	// 更正の請求
					if( pH26HyoView ){
						if( m_pSnHeadData->IsSoleProprietor() ){
							if( pH26HyoView->data_kjn(pDC) ){
								PrEnd = 1;
							}
						}
						else{
							if( pH26HyoView->data_hjn(pDC) ){
								PrEnd = 1;
							}
						}
					}else
					if (pH31HyoView) {
						if (m_pSnHeadData->IsSoleProprietor()) {
							if (pH31HyoView->PrintKosei_Kojin(pDC)) {
								PrEnd = 1;
							}
						}
						else {
							if (pH31HyoView->PrintKosei_Hojin(pDC)) {
								PrEnd = 1;
							}
						}
					}
					break;
				case 0x10 : // 課税売上高計算表
					if( pTaxlistView ){
						// 通ることがないためカット
						//if (GetActViewVer() == ID_VER_SYZ31) {
						//	if (curset.SVmzsw == 0x00) {
						//		PrEnd = pTaxlistView->PrintSellList(60, 1, pDC);
						//	}
						//	else {
						//		PrEnd = pTaxlistView->PrintSellKeikaList(65, 1, pDC);
						//	}
						//}
						//else {
							if (curset.SVmzsw == 0x00) {
								//PrEnd = pTaxlistView->PrintSellList(17, 1, pDC);
								PrEnd = pTaxlistView->PrintSellList(30, 1, pDC);
							}
							else {
								//PrEnd = pTaxlistView->PrintSellKeikaList(22, 1, pDC);
								PrEnd = pTaxlistView->PrintSellKeikaList(40, 1, pDC);
							}
						//}
					}
					else if( pTaxlist31View ){
						//PrEnd = pTaxlist31View->PrintSell31List(73, 1, pDC);
						if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
							//PrEnd = pTaxlist31View->PrintSell31List ( 83, 1, pDC );
							PrEnd = pTaxlist31View->PrintSell31List ( m_PnoSell, 1, pDC );
						}
						else {
							PrEnd = pTaxlist31View->PrintSell31List ( 73, 1, pDC );
						}
					}
					break;
				case 0x11 : // 課税仕入高計算表
					if( pTaxlistView ){
						// 通ることがないためカット
						//if (GetActViewVer() == ID_VER_SYZ31) {
						//	if (curset.SVmzsw == 0x00) {
						//		PrEnd = pTaxlistView->PrintBuyList(61, 1, pDC);//16→18
						//	}
						//	else {
						//		PrEnd = pTaxlistView->PrintBuyKeikaList(66, 1, pDC);
						//	}
						//}
						//else {
							if (curset.SVmzsw == 0x00) {
								//PrEnd = pTaxlistView->PrintBuyList(18, 1, pDC);//16→18
								PrEnd = pTaxlistView->PrintBuyList(31, 1, pDC);//16→18
							}
							else {
								//PrEnd = pTaxlistView->PrintBuyKeikaList(23, 1, pDC);
								PrEnd = pTaxlistView->PrintBuyKeikaList(41, 1, pDC);
							}
						//}
					}
					else if( pTaxlist31View ){
						//PrEnd = pTaxlist31View->PrintBuy31List(74, 1, pDC);
						if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
							//PrEnd = pTaxlist31View->PrintBuy31List ( 84, 1, pDC );
							PrEnd = pTaxlist31View->PrintBuy31List ( m_PnoBuy, 1, pDC );
						}
						else {
							PrEnd = pTaxlist31View->PrintBuy31List ( 74, 1, pDC );
						}
					}
					break;
				case 0x12 : // 課税取引金額計算表 事業所得
					if( pTaxlistView ){
						// 通ることがないためカット
						//if (GetActViewVer() == ID_VER_SYZ31) {
						//	if (curset.SVmzsw == 0x00) {
						//		PrEnd = pTaxlistView->PrintJigyoList(62, 1, pDC);
						//	}
						//	else {
						//		PrEnd = pTaxlistView->PrintJigyoKeikaList(67, 1, pDC);
						//	}
						//}
						//else {
							if (curset.SVmzsw == 0x00) {
								//PrEnd = pTaxlistView->PrintJigyoList(19, 1, pDC);
								PrEnd = pTaxlistView->PrintJigyoList(32, 1, pDC);
							}
							else {
								//PrEnd = pTaxlistView->PrintJigyoKeikaList(24, 1, pDC);
								PrEnd = pTaxlistView->PrintJigyoKeikaList(42, 1, pDC);
							}
						//}
					}
					else if( pTaxlist31View ){
						//PrEnd = pTaxlist31View->PrintJigyo31List(70, 1, pDC);
						if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
							//PrEnd = pTaxlist31View->PrintJigyo31List ( 80, 1, pDC );
							PrEnd = pTaxlist31View->PrintJigyo31List ( m_PnoJigyo, 1, pDC );
						}
						else {
							PrEnd = pTaxlist31View->PrintJigyo31List ( 70, 1, pDC );
						}
					}
					break;
				case 0x13 : // 課税取引金額計算表 不動産所得
					if( pTaxlistView ){
						// 通ることがないためカット
						//if (GetActViewVer() == ID_VER_SYZ31) {
						//	if (curset.SVmzsw == 0x00) {
						//		PrEnd = pTaxlistView->PrintFudousanList(63, 1, pDC);
						//	}
						//	else {
						//		PrEnd = pTaxlistView->PrintFudousanKeikaList(68, 1, pDC);
						//	}
						//}
						//else {
							if (curset.SVmzsw == 0x00) {
								//PrEnd = pTaxlistView->PrintFudousanList(20, 1, pDC);
								PrEnd = pTaxlistView->PrintFudousanList(33, 1, pDC);
							}
							else {
								//PrEnd = pTaxlistView->PrintFudousanKeikaList(25, 1, pDC);
								PrEnd = pTaxlistView->PrintFudousanKeikaList(43, 1, pDC);
							}
						//}
					}
					else if( pTaxlist31View ){
						//PrEnd = pTaxlist31View->PrintFudousan31List(72, 1, pDC);
						if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
// 211203h del -->
							//PrEnd = pTaxlist31View->PrintFudousan31List ( 82, 1, pDC );
// 211203h del <--
// 211203h add -->
							PrEnd = pTaxlist31View->PrintFudousan31List ( m_PnoFudousan, 1, pDC );
// 211203h add <--
						}
						else {
							PrEnd = pTaxlist31View->PrintFudousan31List ( 72, 1, pDC );
						}
					}
					break;
				case 0x14 : // 課税取引金額計算表 農業所得
					if( pTaxlistView ){
						// 通ることがないためカット
						//if (GetActViewVer() == ID_VER_SYZ31) {
						//	if (curset.SVmzsw == 0x00) {
						//		PrEnd = pTaxlistView->PrintNougyouList(64, 1, pDC);
						//	}
						//	else {
						//		PrEnd = pTaxlistView->PrintNougyouKeikaList(69, 1, pDC);
						//	}
						//}
						//else {
							if (curset.SVmzsw == 0x00) {
								//PrEnd = pTaxlistView->PrintNougyouList(21, 1, pDC);
								PrEnd = pTaxlistView->PrintNougyouList(34, 1, pDC);
							}
							else {
								//PrEnd = pTaxlistView->PrintNougyouKeikaList(26, 1, pDC);
								PrEnd = pTaxlistView->PrintNougyouKeikaList(44, 1, pDC);
							}
						//}
					}
					else if( pTaxlist31View ){
						//PrEnd = pTaxlist31View->PrintNougyou31List(71, 1, pDC);
						if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
// 211203h del -->
							//PrEnd = pTaxlist31View->PrintNougyou31List ( 81, 1, pDC );
// 211203h del <--
// 211203h add -->
							PrEnd = pTaxlist31View->PrintNougyou31List ( m_PnoNougyou, 1, pDC );
// 211203h add <--
						}
						else {
							PrEnd = pTaxlist31View->PrintNougyou31List ( 71, 1, pDC );
						}
					}
					break;
				case 0x15 : // 付表５－２－２
					if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
						if( pH26HyoView ){
//							if( pH26HyoView->PrintFuhyo52(pDC, kojin_sgn, PRno[PageNo-1]) ){

							//---->150120
							if( m_SixKindFlg == TRUE ){
								if( pH26HyoView->PrintFuhyo52_2_EX(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}
							else{
								if( pH26HyoView->PrintFuhyo52_2(pDC, kojin_sgn, 1) ){
									PrEnd = 1;
								}
							}
							//<-----------------

							//150120
							//if( pH26HyoView->PrintFuhyo52_2(pDC, kojin_sgn, 1) ){
							//	PrEnd = 1;
							//}
						}
					}
					break;
				case 0x16 : // 付表６
					if( pH26HyoView ){
						PageNo = m_Vprn.GetCurPageNo();
						m_PrintCmInfo.Fhy6Ct = ( PageNo - m_PrintCmInfo.Fhy6Tp ) + 1;

						if( pH26HyoView->PrintFuhyo60( pDC, m_PrintCmInfo.Fhy6Ct ) ){
							PrEnd = 1;
						}
					}
					if (pH31HyoView) {
						PageNo = m_Vprn.GetCurPageNo();
						//m_PrintCmInfo.Fhy6Ct = (PageNo - m_PrintCmInfo.Fhy6Tp) + 1;
						m_PrintCmInfo.Fhy6Ct = GetPGtable(PageNo-1) + 1;

						//if (pH31HyoView->PrintFuhyo6(pDC, m_PrintCmInfo.Fhy6Ct)) {
						//	PrEnd = 1;
						//}
						if ( m_pSnHeadData->Sn_GenericSgn & 0x20 ) {
							if (pH31HyoView->PrintFuhyo6 ( pDC, m_PrintCmInfo.Fhy6Ct ) ) {
								PrEnd = 1;
							}
						}
						else {
							if ( pH31HyoView->PrintFuhyo6_02 ( pDC, m_PrintCmInfo.Fhy6Ct ) ) {
								PrEnd = 1;
							}
						}
					}
					break;
				case 0x79: // 付表６（税率別消費税額計算表）	('23.05.01 Add)
					if (pH31HyoView) {
						if ( pH31HyoView->PrintFuhyo6_2wari( pDC, kojin_sgn, 1 ) ) {
							PrEnd = 1;
						}
					}
					break;
				case 0x20 :	// 仕入控除税額に関する明細書
					// ページ番号の取得
					PageNo = m_Vprn.GetCurPageNo();
					m_CntPage = (PageNo-m_TopPage) + 1;
					// 出力
//					SkjListPrint( m_CntPage );
//					if( ThList != -1 ){
//						m_Vprn.DeleteList( ThList );
//						ThList = -1;				// 作表ハンドル　タイトル①
//					}
//					if( hList != -1 ){
//						m_Vprn.DeleteList( hList );
//						hList  = -1;				// 作表ハンドル　データ
//					}
					break;
				case 0x30 :	// 主な課税資産の譲渡等
					m_KindSw = 1;
					PageNo = m_Vprn.GetCurPageNo();
					m_CntPage = (PageNo-m_RefJotTop) + 1;
					m_KindPg = m_CntPage;												// 次葉ページ番号（申告選択で次葉を選択時）
					// 出力
//					RefListPrint( m_CntPage );
//					if( ThList != -1 ){
//						m_Vprn.DeleteList( ThList );
//						ThList = -1;				// 作表ハンドル　タイトル①
//					}
//					if( TkList != -1 ){
//						m_Vprn.DeleteList( TkList );
//						TkList = -1;				// 作表ハンドル　タイトル②
//					}
//					if( hList != -1 ){
//						m_Vprn.DeleteList( hList );
//						hList  = -1;				// 作表ハンドル　データ
//					}
					break;
				case 0x31 :	// 主な輸出取引等の明細
					m_KindSw = 2;
					PageNo = m_Vprn.GetCurPageNo();
					m_CntPage = (PageNo-m_RefExpTop) + 1;
					m_KindPg = m_CntPage + m_RefJotMax;									// 次葉ページ番号（申告選択で次葉を選択時）
					// 出力
//					RefListPrintExp( m_CntPage );
//					if( ThList != -1 ){
//						pVPrint->DeleteList( ThList );
//						ThList = -1;				// 作表ハンドル　タイトル①
//					}
//					if( TkList != -1 ){
//						pVPrint->DeleteList( TkList );
//						TkList = -1;				// 作表ハンドル　タイトル②
//					}
//					if( hList != -1 ){
//						pVPrint->DeleteList( hList );
//						hList  = -1;				// 作表ハンドル　データ
//					}
					break;
				case 0x32 :	// 主な棚卸資産・原材料等の取得
					m_KindSw = 3;
					PageNo = m_Vprn.GetCurPageNo();
					m_CntPage = ( PageNo - m_RefTnaTop ) + 1;
					// 出力
					m_KindPg = m_CntPage + m_RefJotMax + m_RefExpMax;					// 次葉ページ番号（申告選択で次葉を選択時）
//					RefListPrint( m_CntPage );
//					if( ThList != -1 ){
//						m_Vprn.DeleteList( ThList );
//						ThList = -1;				// 作表ハンドル　タイトル①
//					}
//					if( TkList != -1 ){
//						m_Vprn.DeleteList( TkList );
//						TkList = -1;				// 作表ハンドル　タイトル②
//					}
//					if( hList != -1 ){
//						m_Vprn.DeleteList( hList );
//						hList  = -1;				// 作表ハンドル　データ
//					}
					break;
				case 0x33 :	// 主な固定資産等の取得
					m_KindSw = 4;
					PageNo = m_Vprn.GetCurPageNo();
					m_CntPage = (PageNo-m_RefKotTop) + 1;
					m_KindPg = m_CntPage + m_RefJotMax + m_RefExpMax + m_RefTnaMax;		// 次葉ページ番号（申告選択で次葉を選択時）
					// 出力
//					RefListPrint( m_CntPage );
//					if( ThList != -1 ){
//						m_Vprn.DeleteList( ThList );
//						ThList = -1;				// 作表ハンドル　タイトル①
//					}
//					if( TkList != -1 ){
//						m_Vprn.DeleteList( TkList );
//						TkList = -1;				// 作表ハンドル　タイトル②
//					}
//					if( hList != -1 ){
//						m_Vprn.DeleteList( hList );
//						hList  = -1;				// 作表ハンドル　データ
//					}
					break;

				case 0x76:  //特例１　課税資産の譲渡等の対価の額の計算表１
				case 0x77:  //特例２　課税資産の譲渡等の対価の額の計算表２
				case 0x78:  //特例３　課税仕入れ等の税額の計算表
					if (pH31HyoView) {
						PageNo = m_Vprn.GetCurPageNo();

						//int idx = (m_PrintCmInfo.PRtable[PageNo - 1] & 0x7f) - 0x76;
						int idx = (GetPRtable(&m_PrintCmInfo,PageNo - 1) & 0x7f) - 0x76;

						int* pTokurei_Ct[3] = { &m_PrintCmInfo.Tokurei_Uri1Ct,
												&m_PrintCmInfo.Tokurei_Uri2Ct,
												&m_PrintCmInfo.Tokurei_ShiireCt };

						int* pTokurei_Tp[3] = { &m_PrintCmInfo.Tokurei_Uri1Tp,
												&m_PrintCmInfo.Tokurei_Uri2Tp,
												&m_PrintCmInfo.Tokurei_ShiireTp };

						//*pTokurei_Ct[idx] = (PageNo - *pTokurei_Tp[idx]) + 1;
						*pTokurei_Ct[idx] = GetPGtable(PageNo-1)+1;

						//switch(m_PrintCmInfo.PRtable[PageNo - 1] & 0x7f){
						switch(GetPRtable(&m_PrintCmInfo,PageNo - 1) & 0x7f)	{
						    case 0x76:
							    pH31HyoView->PrintTokurei_Uri1(pDC, kojin_sgn, *pTokurei_Ct[idx]);
							    break;
						    case 0x77:
								pH31HyoView->PrintTokurei_Uri2(pDC, kojin_sgn, *pTokurei_Ct[idx]);
							    break;
						    case 0x78:
								pH31HyoView->PrintTokurei_Shiire(pDC, kojin_sgn, *pTokurei_Ct[idx]);
							    break;
						}
					}
                    break;

				case 0x40 :	// 特定収入計算表
					if( pTaxlistView ){
						PrEnd = pTaxlistView->SpecificList( 1, 0 );
					}
					else if ( pTaxlist31View ) {
						// 計算表１
						PrEnd = pTaxlist31View->SpecificList31( 1, 0 );
					}
					break;
				case 0x41 :	// 特定収入計算表
					// ページ番号の取得
					PageNo = m_Vprn.GetCurPageNo();
					//m_PrintCmInfo.SpcCt1 = ( PageNo - m_PrintCmInfo.SpcTp1 ) + 1;
					if(pH31HyoView)	m_PrintCmInfo.SpcCt1 = GetPGtable(PageNo-1)+1;
					else			m_PrintCmInfo.SpcCt1 = (PageNo-m_PrintCmInfo.SpcTp1)+1;
					// 出力
					if( pTaxlistView ){
						PrEnd = pTaxlistView->SpecificList( 2, m_PrintCmInfo.SpcCt1 );
					}
					else if ( pTaxlist31View ) {
						// 計算表２（１）
						PrEnd = pTaxlist31View->SpecificList31( 2, m_PrintCmInfo.SpcCt1 );
					}
					break;
				case 0x42 :	// 特定収入計算表
					// ページ番号の取得
					PageNo = m_Vprn.GetCurPageNo();
					// 出力
					if( pTaxlistView ){
						m_PrintCmInfo.SpcCt2_5per = ( PageNo - m_PrintCmInfo.SpcTp2_5per ) + 1;
						PrEnd = pTaxlistView->SpecificList( 3, m_PrintCmInfo.SpcCt2_5per );
					}
					else if ( pTaxlist31View ) {
						// 計算表２（２）表面
						m_PrintCmInfo.SpcCt2_5per = GetPGtable(PageNo-1)+1;
						PrEnd = pTaxlist31View->SpecificList31( 3, m_PrintCmInfo.SpcCt2_5per );
					}
					break;
				case 0x43 :	// 特定収入計算表
					if( pTaxlistView ){
						PrEnd = pTaxlistView->SpecificList( 4, 0 );
					}
					break;
				case 0x44 :	// 特定収入計算表
					if( pTaxlistView ){
						PrEnd = pTaxlistView->SpecificList( 5, 0 );
					}
					break;
				case 0x45 :	// 特定収入計算表
					if( pTaxlistView ){
						PrEnd = pTaxlistView->SpecificList( 5, 0 );
					}
					break;
				case 0x46 :	// 特定収入計算表
					if( pTaxlistView ){
						PrEnd = pTaxlistView->SpecificList( 5, 0 );
					}
					break;
				case 0x47 :	// 特定収入計算表
					if( pTaxlistView ){
						PrEnd = pTaxlistView->SpecificList( 6, 0 );
					}
					break;
				case 0x52 : // 事業別売上高 [13'09.19]
//					PrEnd = data_kniinp();
					if( pTaxlistView ){
						if( curset.SVmzsw == 0x00 ){
							PrEnd = pTaxlistView->PrintKniinpList( 0, 1,pDC );
						}else{
							PrEnd = pTaxlistView->PrintKniinpKeikaList( 0, 1,pDC );
						}
					}
					else if( pTaxlist31View ){
						PrEnd = pTaxlist31View->PrintKniinp31List( 0, 1,pDC );
					}
					break;
				case 0x53 :	// 特定収入計算表
					if( pTaxlistView ){
						PrEnd = pTaxlistView->SpecificList( 7, 0 );
					}
					break;
				case 0x54 :	// 特定収入計算表
					if( pTaxlistView ){
						PrEnd = pTaxlistView->SpecificList( 7, 0 );
					}
					break;
				case 0x55 :	// 特定収入計算表
					if( pTaxlistView ){
						PrEnd = pTaxlistView->SpecificList( 7, 0 );
					}
					break;
				case 0x56 :	// 特定収入計算表 ２(２)　８％
					// ページ番号の取得
					PageNo = m_Vprn.GetCurPageNo();
					if( pTaxlistView ){
						m_PrintCmInfo.SpcCt2_8per = ( PageNo - m_PrintCmInfo.SpcTp2_8per ) + 1;
						PrEnd = pTaxlistView->SpecificList( 8, m_PrintCmInfo.SpcCt2_8per );
					}
					else if ( pTaxlist31View ) {
						// 計算表２（２）裏面
						m_PrintCmInfo.SpcCt2_8per = GetPGtable(PageNo-1)+1;
						PrEnd = pTaxlist31View->SpecificList31( 4, m_PrintCmInfo.SpcCt2_8per );
					}
					break;
				case 0x57 : // 特定課税仕入れがある場合の課税標準額等の内訳書 提出用
				case 0x58 : // 特定課税仕入れがある場合の課税標準額等の内訳書 控用
					if( pH26HyoView ){
//-- '15.11.30 --
//						if( pH26HyoView->PrintUchiwake(pDC, PP, 1)){
//							PrEnd = 1;
//						}
//---------------
						BOOL IsHikae = FALSE;
						//if( (m_PrintCmInfo.PRtable[PageNo-1]&0x7f) == 0x58 ){
						if((GetPRtable(&m_PrintCmInfo,PageNo-1) & 0x7f) == 0x58)	{
							IsHikae = TRUE;
						}
						if( pH26HyoView->PrintUchiwake(pDC, IsHikae, PP, 1)){
							PrEnd = 1;
						}
//---------------
					}
					if (pH31HyoView) {
						BOOL IsHikae = FALSE;
						//if ( ( m_PrintCmInfo.PRtable[PageNo - 1] & 0x7f ) == 0x58 ) {
						if((GetPRtable(&m_PrintCmInfo,PageNo-1) & 0x7f) == 0x58)	{
							IsHikae = TRUE;
						}

						//if (pH31HyoView->PrintHonPyo2(pDC, FALSE, PP )) {
						if ( pH31HyoView->PrintHonPyo2 ( pDC, IsHikae, PP ) ) {
							PrEnd = 1;
						}
					}
					break;
				case 0x59 : // 特定収入計算表 ２(２)　経過措置
					PageNo = m_Vprn.GetCurPageNo();
					m_PrintCmInfo.SpcCt2_keika = ( PageNo - m_PrintCmInfo.SpcTp2_keika ) + 1;
					if( pTaxlistView ){
						PrEnd = pTaxlistView->SpecificList( 9, m_PrintCmInfo.SpcCt2_keika );
					}
					break;
				case 0x60 :	// 特定収入計算表
					if( pTaxlistView ){
						PrEnd = pTaxlistView->SpecificList( 10, 0 );
					}
					else if ( pTaxlist31View ) {
						// 計算表３４
						PrEnd = pTaxlist31View->SpecificList31( 5, 0 );
					}
					break;
				case 0x61 :	// 特定収入計算表
					if( pTaxlistView ){
						PrEnd = pTaxlistView->SpecificList( 10, 0 );
					}
					else if ( pTaxlist31View ) {
						// 計算表５（１）（２）（３）
						PrEnd = pTaxlist31View->SpecificList31( 6, 0 );
					}
					break;
				case 0x62 :	// 特定収入計算表
					if( pTaxlistView ){
						PrEnd = pTaxlistView->SpecificList( 10, 0 );
					}
// 24/02/26_特定収入対応 add -->
					// 特定収入計算表 5-2(1)
					else	{
//int	a=m_PGtable2.GetCount();
//int	b=0,c=0;
//for(b=0; b<a; b++)	{
//	c=m_PGtable2.GetAt(b);
//}
						PageNo = m_Vprn.GetCurPageNo();
						PrEnd = pTaxlist31View->SpecificList31(7,GetPGtable(PageNo-1));
					}
// 24/02/26_特定収入対応 add <--
					break;
// 24/02/26_特定収入対応 add -->
				case 0x63:	// 特定収入計算表 5-2(2)
					PageNo = m_Vprn.GetCurPageNo();
					PrEnd = pTaxlist31View->SpecificList31(8,GetPGtable(PageNo-1));
					break;
				case 0x64:	// 特定収入計算表 5-2(3)-1
					PageNo = m_Vprn.GetCurPageNo();
					PrEnd = pTaxlist31View->SpecificList31(9,GetPGtable(PageNo-1));
					break;
				case 0x65:	// 特定収入計算表 5-2(3)-2
					PageNo = m_Vprn.GetCurPageNo();
					PrEnd = pTaxlist31View->SpecificList31(10,GetPGtable(PageNo-1));
					break;
				case 0x66:	// 特定収入計算表 5-2(3)-3
					PageNo = m_Vprn.GetCurPageNo();
					PrEnd = pTaxlist31View->SpecificList31(11,GetPGtable(PageNo-1));
					break;
				case 0x67:	// 特定収入計算表 5-2(4)-1
					PageNo = m_Vprn.GetCurPageNo();
					PrEnd = pTaxlist31View->SpecificList31(12,GetPGtable(PageNo-1));
					break;
				case 0x68:	// 特定収入計算表 5-2(4)-2
					PageNo = m_Vprn.GetCurPageNo();
					PrEnd = pTaxlist31View->SpecificList31(13,GetPGtable(PageNo-1));
					break;
				case 0x69:	// 特定収入計算表 5-2(4)-3
					PageNo = m_Vprn.GetCurPageNo();
					PrEnd = pTaxlist31View->SpecificList31(14,GetPGtable(PageNo-1));
					break;
// 24/02/26_特定収入対応 add <--

// 24/03/18_税務代理書面 add -->
				case 0x80:	// 税務代理権限証書
				case 0x81:	// 税理士法第33条の2第1項に規定する添付書面（第1面
				case 0x82:	// 税理士法第33条の2第1項に規定する添付書面（第2面）
				case 0x83:	// 税理士法第33条の2第1項に規定する添付書面（第3面）
				case 0x84:	// 計税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
				case 0x85:	// 税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
				case 0x86:	// 税理士法第33条の2第2項に規定する添付書面（第1面）
				case 0x87:	// 税理士法第33条の2第2項に規定する添付書面（第2面）
				case 0x88:	// 税理士法第33条の2第2項に規定する添付書面（第3面）
				case 0x89:	// 税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
				case 0x90:	// 税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）
					// 呼び出し元業務
// 24/03/21_税務代理書面 cor -->
					//if(m_Util.IsUnConnectMst(m_pZmSub->zvol))	yno=1;	// 消費税申告書（非連動）
					//else										yno=0;	// 消費税申告書
// -----------------------------
					if(m_Util.IsUnConnectMst(m_pZmSub->zvol))	yflg=1;	// 消費税申告書（非連動）
					else										yflg=0;	// 消費税申告書
// 24/03/21_税務代理書面 cor <--
					// 出力帳票
					yno = GetPRtable(&m_PrintCmInfo,PageNo-1);
					if((yno & 0xff) == 0x80)		yno = 0x01;		// 税務代理権限証書
					else if((yno & 0xff) == 0x81)	yno = 0x02;		// 申告書の作成に関する計算事項記載書面 33の2(1) (1面)
					else if((yno & 0xff) == 0x82)	yno = 0x04;		// 申告書の作成に関する計算事項記載書面 33の2(1) (2面)
					else if((yno & 0xff) == 0x83)	yno = 0x08;		// 申告書の作成に関する計算事項記載書面 33の2(1) (3面)
					else if((yno & 0xff) == 0x84)	yno = 0x10;		// 申告書の作成に関する計算事項記載書面 33の2(1) (4面 1枚目) 
					else if((yno & 0xff) == 0x85)	yno = 0x20;		// 申告書の作成に関する計算事項記載書面 33の2(1) (4面 2枚目)
					else if((yno & 0xff) == 0x86)	yno = 0x40;		// 申告書に関する審査事項記載書面 33の2(2) (1面)
					else if((yno & 0xff) == 0x87)	yno = 0x80;		// 申告書に関する審査事項記載書面 33の2(2) (2面)
					else if((yno & 0xff) == 0x88)	yno = 0x100;	// 申告書に関する審査事項記載書面 33の2(2) (3面)
					else if((yno & 0xff) == 0x89)	yno = 0x200;	// 申告書に関する審査事項記載書面 33の2(2) (4面 1枚目)
					else if((yno & 0xff) == 0x90)	yno = 0x400;	// 申告書に関する審査事項記載書面 33の2(2) (4面 2枚目)
					// 提出先名
					strSaki = _T("");
					// 電子申告済みを印字する
					if(Zeiri_DButton_sw & 0x04)	{
						electronic = 1;
					}
					// 印刷項目　依頼者：氏名又は名称
					if(Zeiri_DButton_sw & 0x01)	{
						opt1 = 1;
					}
					// 印刷項目　依頼者：住所又は事業所の所在地
					if(Zeiri_DButton_sw & 0x02)	{
						opt2 = 1;
					}
					// 給与処理ｄｂ　訂正区分
					syorikbn = 1;
					// 給与処理ｄｂ　異動届出書の種類
					sn_tetuzuki_kbn = 0;
					// 給与処理ｄｂ　社員番号
					stffid = 0;
					// 
					PrEnd = 0;
					//if(ZtenpPrint(m_pZmSub->m_database,&m_Vprn,pDC,yflg,yno,strSaki,electronic,opt1,opt2,syorikbn,sn_tetuzuki_kbn,stffid,this) == -1)	{
					//if(ZtenpPrint(m_pZmSub->m_database,&m_Vprn,pDC,yflg,yno,strSaki,electronic,opt1,opt2,syorikbn,sn_tetuzuki_kbn,stffid,_T(""),_T(""),this) == -1)	{
// 24/04/01_246 cor -->
					//m_pMainFrame->SetGkspacInfo(&kmn);
// --------------------
					if(m_GetSki != 1)	electronic = 0;
// 24/04/01_246 cor <--
// 24/03/26_税務代理書面 add -->
					m_Fnt108gt = m_Vprn.RegisterFont(108,0,0,_T("ＭＳ ゴシック"));
					m_Fnt090gt = m_Vprn.RegisterFont( 90,0,0,_T("ＭＳ ゴシック"));
					ZtenpSetFontSize(m_Fnt108gt,m_Fnt090gt);
// 24/03/26_税務代理書面 add <--
//if(1)	{
//	CString	cs=_T("");
//	cs.Format(_T("***** electronic=%d"),electronic);
//	DbgViewTrace(cs);
//}
// 24/04/01_246 cor -->
					//if(ZtenpPrint(m_pZmSub->m_database,&m_Vprn,pDC,yflg,yno,strSaki,electronic,opt1,opt2,syorikbn,sn_tetuzuki_kbn,stffid,&kmn.kanryo,this) == -1)	{
// --------------------
					if(m_pSnHeadData->m_Zrkbn == 2)	{
						// 税理士情報が所属税理士
						if(m_pSnHeadData->m_ZroutSgn & 0x01)	{
							// 税理士区分を出力
							opt3=opt3|0x01;
						}
						if(m_pSnHeadData->m_ZroutSgn & 0x02)	{
							// 直接受任を出力
							opt3=opt3|0x02;
						}
					}
					//if(ZtenpPrint(m_pZmSub->m_database,&m_Vprn,pDC,yflg,yno,strSaki,electronic,opt1,opt2,syorikbn,sn_tetuzuki_kbn,stffid,&m_Kanryo,this) == -1)	{
// 24/04/04_モジュール仕様変更 cor -->
					//if(ZtenpPrint(m_pZmSub->m_database,&m_Vprn,pDC,yflg,yno,strSaki,electronic,opt1,opt2,syorikbn,sn_tetuzuki_kbn,stffid,&m_Kanryo,this,opt3) == -1)	{
// -----------------------------------
					m_pMainFrame->GetZeiriInfo(&rh,&rz);
					// ここは本表・付表の出力（予定26号は別にあるのでそちらの変更も忘れずに！）
					if(ZtenpPrint(m_pZmSub->m_database,&m_Vprn,pDC,yflg,yno,strSaki,electronic,opt1,opt2,syorikbn,sn_tetuzuki_kbn,stffid,&m_Kanryo,this,opt3,0,&rh,&rz) == -1)	{
// 24/04/04_モジュール仕様変更 cor <--
// 24/04/01_246 cor <--
						PrEnd = 1;
					}
// 24/03/26_税務代理書面 add -->
					m_Vprn.DeleteRegFont(m_Fnt108gt);
					m_Vprn.DeleteRegFont(m_Fnt090gt);
// 24/03/26_税務代理書面 add <--

					break;
// 24/03/18_税務代理書面 add <--

				default   :
					PrEnd = 1;
					break;
			}

			// 指定ページの作成
			if( !(m_PrintCmInfo.PRtype&0x80) ){
				if( PrEnd ){
					m_Vprn.LastPage( 1 );
				}
				else{
					// 次ページ　印刷終了チェック
					if( PageNo == m_PrintCmInfo.PgMx ){
						m_Vprn.LastPage( 1 );
					}
					else{
						if( (!(m_PrintCmInfo.PRtype&0x80))&&(pInfo->m_bPreview == FALSE) ){	// ＯＣＲ印刷
							m_Vprn.LastPage();
						}
					}
				}
			}
			else{
				if( PrEnd ){
					m_Vprn.LastPage();
				}
				else{
					// 次ページ　印刷終了チェック
					if( PageNo == m_PrintCmInfo.PgMx ){
						m_Vprn.LastPage();
					}
				}
			}
		}
	}

	CH26HyoView	*pH26View = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 2 ));
	if( pH26View != NULL ){
//		pH26View->PrintSyzHyo();
	}

//	m_Vprn.LastPage();
	m_Vprn.EndPageJob(pDC);
}

//-----------------------------------------------------------------------------
// 印刷する帳票が課税取引金額計算表？
//-----------------------------------------------------------------------------
// 引数	sign		：	チェックサイン
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	課税取引金額計算表
//			FALSE	：	課税取引金額計算表でない
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::IsPrintTaxationList( char sign )
{
	BOOL	bRt = FALSE;

	if( (sign==0x10) ||	// 課税売上高計算表
		(sign==0x11) ||	// 課税仕入高計算表
		(sign==0x12) ||	// 課税取引金額計算表 事業所得
		(sign==0x13) ||	// 課税取引金額計算表 不動産所得
		(sign==0x14) ||	// 課税取引金額計算表 農業所得
		(sign==0x52) ){	// 事業別売上高

		bRt = TRUE;
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 印刷する帳票が特定収入計算表？
//-----------------------------------------------------------------------------
// 引数	sign		：	チェックサイン
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	課税取引金額計算表
//			FALSE	：	課税取引金額計算表でない
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::IsPrintSpcList( char sign )
{
	BOOL	bRt = FALSE;

	if( (sign==0x40) ||
		(sign==0x41) ||
		(sign==0x42) ||
		(sign==0x43) ||
		(sign==0x44) ||
		(sign==0x45) ||
		(sign==0x46) ||
//2015.03.12 UPDATE START
//		(sign==0x47) ){
		(sign==0x47) ||
		(sign==0x53) ||
		(sign==0x54) ||
		(sign==0x55) ||
//2017.03.01 UPDATE START
//		(sign==0x56) ){
		(sign==0x56) ||
		(sign==0x59) ||
		(sign==0x60) ||
		(sign==0x61) ||
// 24/02/26wd_特定収入対応 cor -->
//		(sign==0x62) ){
// -------------------------------
		(sign==0x62) ||
		(sign==0x63) ||
		(sign==0x64) ||
		(sign==0x65) ||
		(sign==0x66) ||
		(sign==0x67) ||
		(sign==0x68) ||
		(sign==0x69) ){
// 24/02/26wd_特定収入対応 cor <--
//2017.03.01 UPDATE END
//2015.03.12 UPDATE END
		bRt = TRUE;
	}

	return bRt;
}

// CDBSyzShinMainView メッセージ ハンドラ
void CDBSyzShinMainView::OnButtonF4()
{
	
//2016.06.22 INSERT START
	if( m_pMainFrame && (m_Vprn.PreviewOnPrintdlg()==0) ){
		m_pMainFrame->MoveKanpuFocus();
//2018.03.13 INSERT START
		if( m_pMainFrame->Message_MyNumber_error() == -1 ){
			return;
		}
		if( m_pMainFrame->Message_MyNumber_overlap() == -1 ){
			return;
		}
		m_pMainFrame->EditOff_Fuhyo6();
		if( m_pMainFrame->Message_SozokuWariai() == -1 ){
			return;
		}
		if( m_pMainFrame->Message_NofuKanpuGokei() == IDYES ){
			return;
		}
//2018.03.13 INSERT END
		int	erst=0;
		if( m_pMainFrame->Message_BlankKinyu(erst) == IDYES ){
			m_pMainFrame->GuideKanpuKinyu(erst);
			return;
		}
//2017.12.04 INSERT START
		if( m_pMainFrame->Message_BlankKijunKazei() == IDYES ){
			return;
		}

		if( m_pMainFrame->Message_BlankSoneki() == IDYES ){
			return;
		}

		if ( m_pMainFrame->Message_H31ManualInputSign() == -1 ) {
			return;
		}

		//----> 2022/12/27
		if( m_pMainFrame->Message_KoukinUketori() == -1 ){
			return;
		}
		//<---- 2022/12/27

//2017.12.04 INSERT END
	}
//2016.06.22 INSERT END

//m_Vprn.SetContinueMode( -1 );
	 m_PrintType = 0;
//	 ClearPrintCmInfo();

//	CH26HyoView	*pH26View = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 2 ));

	if( m_Vprn.m_ErrFlag == 0 ){
		if( (m_Vprn.IsContinueMode()==FALSE) || (m_PrintCmInfo.PRtype&0x80) ){
//--> '15.12.26 INS START
			// 印刷情報のクリア
			ClearPsetInfo();
//<-- '15.12.26 INS END
//		if( (m_Vprn.IsContinueMode()==FALSE) ){
			// 印刷共有情報のクリア
			ClearPrintCmInfo();

			GetPrnExSubInfo2();	// 特定収入計算表 個別出力設定		// 24/02/27wd_特定収入対応 add

			if( CheckSyzShinPrint() ){
				return;				// 印刷情報の更新
			}

			if( m_Vprn.PreviewOnPrintdlg() == 0 ){
				CPrnReportDlg	PrDlg;

				m_pShinInfo->pSgSpc = m_pMainFrame->GetsgSpc();
				m_pShinInfo->ActViewVer = GetActViewVer();
				
				m_PrintCmInfo.pMvRec = m_MvRec;
				m_PrintCmInfo.pPrMv = m_PrMv;
				m_PrintCmInfo.pPrMove = m_PrMove;
//--> '15.02.25 INS START
				if( m_pMainFrame ){
					m_PrintCmInfo.zr_kbn = m_pMainFrame->GetZeirishiType();
				}
//<-- '15.02.25 INS END
//2015.08.04 INSERT START
				m_PrintCmInfo.Shrevtype = GetShRevType();
//2015.08.04 INSERT END
//				PrDlg.InitInfo( m_pSnHeadData, m_pZmSub, &ShinInfo, &m_PrintCmInfo );
				//PrDlg.InitInfo( m_pSnHeadData, m_pZmSub, m_pShinInfo, &m_PrintCmInfo );
				for(int ii=0; ii<50; ii++)	m_PrintCmInfo.busu[ii]=m_BusuSave[ii];
				//各種タブボタン情報
				PrDlg.m_Kazei_DButton_sw = Kazei_DButton_sw;
				PrDlg.m_Toku_DButton_sw = Toku_DButton_sw;
				PrDlg.m_Zeiri_DButton_sw = Zeiri_DButton_sw;

	
				PrDlg.InitInfo(m_pSnHeadData,m_pZmSub,m_pShinInfo,&m_PrintCmInfo,m_pMainFrame,&m_PRtableEx,&m_PGtable,&m_PRtableEx1);
				PrDlg.m_IsPreview = TRUE;

				int		rv=PrDlg.DoModal();
// 24/04/01_246 add -->
				Kazei_DButton_sw	= PrDlg.m_Kazei_DButton_sw;
				Toku_DButton_sw		= PrDlg.m_Toku_DButton_sw;
				Zeiri_DButton_sw	= PrDlg.m_Zeiri_DButton_sw;
// 24/04/01_246 add <--
				if(rv != IDOK)	{
					return;
				}
				if( m_pSnHeadData->IsMiddleProvisional() ){
					if( PrntBasisSelect() ){
						return;
					}
				}
			}
		}
		if( !(m_PrintCmInfo.PRtype&0x80) ){	// ＯＣＲ印刷
//shimizu	m_Vprn.SetContinueMode( 0, m_PrintCmInfo.PgMx );
			if (m_Vprn.IsContinueMode() == TRUE)
				m_Vprn.SetContinueMode( -1 );	// 印刷からのプレビュー時に、連続モードをＯＦＦにするため
		}
	}

// 24/06/13_税務代理書面 add -->
	((CMainFrame*)m_pMainFrame)->CallWriteTaxAgentDB(m_pZmSub,m_pSnHeadData,this);
// 24/06/13_税務代理書面 add <--

//	ICSFormView::OnButtonF4();
	m_Vprn.DoPrintPreview( this );
}

void CDBSyzShinMainView::OnButtonF5()
{

	 m_PrintType = 0;
//	 ClearPrintCmInfo();

	int	FromPreview = 0;
	if( m_Vprn.IsPrintFromPreview() == TRUE ){
		FromPreview = 1;
	}
	else if( m_Vprn.PreviewOnPrintdlg() == 1 ){
//		;
// 依頼No.160101の対応
// [F5 印刷] → 申告書出力設定 →「印刷[F5]」→ 印刷ダイアログ →「プレビュー[F4]」→「閉じる[End]
// 上記の手順で申告書出力設定ダイアログが表示されていたところを印刷ダイアログに戻るように修正
		FromPreview = 1;
	}
//--> '15.12.26 INS START
	else{

		//--->yoshida200213
		if(!(m_PrintCmInfo.PRtype & 0x80)) {//KSK印刷。GetCurKindNoはKSK印刷のみ？（仕様書で確認。他で制御してないが一応制御をしておく）
			if (m_Vprn.GetCurKindNo() <= 1) {
				// 印刷情報のクリア
				ClearPsetInfo();
			}
		}
		else {
			// 印刷情報のクリア
			ClearPsetInfo();
		}
		//<-------
		
//2016.06.22 INSERT START
		if( m_pMainFrame ){
			m_pMainFrame->MoveKanpuFocus();
//2018.03.13 INSERT START
			if (m_Vprn.GetCurKindNo() <= 1) {	// 連続モード（ＯＣＲ）時は、２頁以降は通らないように
				if (m_pMainFrame->Message_MyNumber_error() == -1) {
					return;
				}
				if (m_pMainFrame->Message_MyNumber_overlap() == -1) {
					return;
				}
				m_pMainFrame->EditOff_Fuhyo6();
				if (m_pMainFrame->Message_SozokuWariai() == -1) {
					return;
				}
				if (m_pMainFrame->Message_NofuKanpuGokei() == IDYES) {
					return;
				}
				//2018.03.13 INSERT END
				int erst = 0;
				if (m_pMainFrame->Message_BlankKinyu(erst) == IDYES) {
					m_pMainFrame->GuideKanpuKinyu(erst);
					return;
				}
				//2017.12.04 INSERT START
				if (m_pMainFrame->Message_BlankKijunKazei() == IDYES) {
					return;
				}

				if (m_pMainFrame->Message_BlankSoneki() == IDYES) {
					return;
				}

				if ( m_pMainFrame->Message_H31ManualInputSign() == -1 ) {
					return;
				}

				//----> 2022/12/27
				if( m_pMainFrame->Message_KoukinUketori() == -1 ){
					return;
				}
				//<---- 2022/12/27
			}
//2017.12.04 INSERT END
		}
//2016.06.22 INSERT END
	}
//<-- '15.12.26 INS END
//	else{
//		 ClearPrintCmInfo();
//	}

// 24/07/01_税務代理書面 add -->
	((CMainFrame*)m_pMainFrame)->CallWriteTaxAgentDB(m_pZmSub,m_pSnHeadData,this);
// 24/07/01_税務代理書面 add <--

	if( m_swBeFork ){
// 24/06/17_ScoreLink追加対応 add -->
		if(m_isScoreLinkCall != -1)	{
			// 予定(26号)
			if(m_pSnHeadData->IsMiddleProvisional())	{
				// 第２６号様式にチェックが付いていなければチェックを付ける
				if(!(m_pSnHeadData->Sn_PrintOutSgn & 0x800))	{
					m_pSnHeadData->Sn_PrintOutSgn |= 0x800;
				}
				// 第２６号様式Ａ４ヨコ控用にチェックが付いていればチェックを外す
				if((m_pSnHeadData->Sn_PrintOutSgn & 0x1000))	{
					m_pSnHeadData->Sn_PrintOutSgn &= ~0x1000;
				}
			}
			else	{
				// ＯＣＲ用紙が選択されていれば白紙に切り替える
				if(!(m_pSnHeadData->Sn_TYPE & 0x01))	{
					m_pSnHeadData->Sn_TYPE &= 0xfc;
					m_pSnHeadData->Sn_TYPE |= 0x01;
				}
				// 申告書本表にチェックが付いていなければチェックを付ける
				if(!(m_pSnHeadData->Sn_PrintOutSgn2 & 0x04))	{
					m_pSnHeadData->Sn_PrintOutSgn2 |= 0x04;
				}
				// 申告書本表の提出用にチェックが付いていなければチェックを付ける
				if(!(m_pSnHeadData->Sn_PrintOutSgn2 & 0x08))	{
					m_pSnHeadData->Sn_PrintOutSgn2 |= 0x08;
				}
				// 申告書本表の控用にチェックが付いていればチェックを外す
				if((m_pSnHeadData->Sn_PrintOutSgn2 & 0x10))	{
					m_pSnHeadData->Sn_PrintOutSgn2 &= ~0x10;
				}
				// 課税標準額の内訳書にチェックが付いていなければチェックを付ける
				if(!(m_pSnHeadData->Sn_PrintOutSgn2 & 0x20))	{
					m_pSnHeadData->Sn_PrintOutSgn2 |= 0x20;
				}
				// 課税標準額の内訳書の提出用にチェックが付いていなければチェックを付ける
				if(!(m_pSnHeadData->Sn_PrintOutSgn2 & 0x40))	{
					m_pSnHeadData->Sn_PrintOutSgn2 |= 0x40;
				}
				// 課税標準額の内訳書の控用にチェックが付いていればチェックを外す
				if((m_pSnHeadData->Sn_PrintOutSgn2 & 0x80))	{
					m_pSnHeadData->Sn_PrintOutSgn2 &= ~0x80;
				}
			}
		}
// 24/06/17_ScoreLink追加対応 add <--
		if( m_swBeFork == 1 ) {
			GetPrnExSubInfo2();	// 特定収入計算表 個別出力設定		// 24/02/27wd_特定収入対応 add

			// 印刷情報の更新
			if( CheckSyzShinPrint() )	{		// 印刷情報の更新
				SetBpPrintErr();
				AfxGetMainWnd()->PostMessageA( WM_CLOSE, 0 );
				return;
			}
			CPrnReportDlg	PRDialog;

			m_pShinInfo->ActViewVer = GetActViewVer();
						
			m_PrintCmInfo.pMvRec = m_MvRec;
			m_PrintCmInfo.pPrMv = m_PrMv;
			m_PrintCmInfo.pPrMove = m_PrMove;
//--> '15.02.25 INS START
			if( m_pMainFrame ){
				m_PrintCmInfo.zr_kbn = m_pMainFrame->GetZeirishiType();
			}
//<-- '15.02.25 INS END

//--> '16.07.21 INS START
			m_PrintCmInfo.Shrevtype = m_ShRevType;
//<-- '16.07.21 INS END

			//PRDialog.InitInfo( m_pSnHeadData, m_pZmSub, m_pShinInfo, &m_PrintCmInfo );
			for(int ii=0; ii<50; ii++)	m_PrintCmInfo.busu[ii]=m_BusuSave[ii];

// 24/04/01_246 add -->
			PRDialog.m_Kazei_DButton_sw	= Kazei_DButton_sw;
			PRDialog.m_Toku_DButton_sw	= Toku_DButton_sw;
			PRDialog.m_Zeiri_DButton_sw	= Zeiri_DButton_sw;
// 24/04/01_246 add <--

			PRDialog.InitInfo(m_pSnHeadData,m_pZmSub,m_pShinInfo,&m_PrintCmInfo,m_pMainFrame,&m_PRtableEx,&m_PGtable,&m_PRtableEx1);
			PRDialog.m_swBeForked = m_swBeFork;									// ボタン表示 「プレビュー」
			INT_PTR st = PRDialog.DoModal();
// 24/04/01_246 add -->
			Kazei_DButton_sw	= PRDialog.m_Kazei_DButton_sw;
			Toku_DButton_sw		= PRDialog.m_Toku_DButton_sw;
			Zeiri_DButton_sw	= PRDialog.m_Zeiri_DButton_sw;
// 24/04/01_246 add <--
//			if( st != IDOK )	return;				// 処理中断
			if( m_pSnHeadData->IsMiddleProvisional() ) {
				if( PrntBasisSelect() )	{
					SetBpPrintErr();
					AfxGetMainWnd()->PostMessageA( WM_CLOSE, 0 );
					return;
				}
			}
			AfxGetMainWnd()->PostMessageA( WM_CLOSE, 0 );

			return;
		}
		if( m_pSnHeadData->IsMiddleProvisional() ) {
			// 出力情報の更新
			if( PrntBasisSelect() )	{
				AfxGetMainWnd()->PostMessageA( WM_CLOSE, 0x2000 );
				SetBpPrintErr();
				return;
			}
		}
		else{
/*			if( SumUpPrintCalq( 1 ) )	{
				AfxGetMainWnd()->PostMessageA( WM_CLOSE, 0x2000 );
				m_swForkErr = 1;
				return;
			}
			// 印刷情報の更新
			if( SyzShinPrint() ) {
				AfxGetMainWnd()->PostMessageA( WM_CLOSE, 0x2000 );
				m_swForkErr = 1;
				return;
			}
*/			
			GetPrnExSubInfo2();	// 特定収入計算表 個別出力設定		// 24/02/27wd_特定収入対応 add

			if( CheckSyzShinPrint() ){
				AfxGetMainWnd()->PostMessageA( WM_CLOSE, 0x2000 );
				SetBpPrintErr();
				return;			// 印刷情報の更新
			}
		}
		// 一括印刷初期設定
		if( SumUpPrintIntinitial())	{
			AfxGetMainWnd()->PostMessageA( WM_CLOSE, 0x2000 );
			SetBpPrintErr();
			return;
		}
/*		if( !(m_PrintCmInfo.PRtype&0x80) ) {											// ＯＣＲ印刷
			pVPrint->SetContinueMode( 0, PgMx );
		}
*/		
// 24/07/02_360 add -->
		if(m_pMainFrame)	{
			m_PrintCmInfo.zr_kbn = m_pMainFrame->GetZeirishiType();
		}
// 24/07/02_360 add <--
		ICSFormView::OnFilePrint();
		return;
	}

	// 還付申告の財務連動チェックリスト印刷及び還付申告の明細書（次葉）印刷の解除
	// プレビューから印刷を選択以外は解除
	if( m_Vprn.IsPrintFromPreview() == FALSE ){
		m_PrintType = 0;
	}
	// 連続モード印刷時の処理
	m_Vprn.SetOptionMode( POF_PVRESET );									// プレビューフラグのリセット
	// 還付申告の財務連動チェックリスト印刷及び還付申告の明細書（次葉）印刷は印刷開始
	if( m_PrintType ){
		ICSFormView::OnFilePrint();
		return;
	}

	//== 消費税申告書 ==
	if( m_Vprn.m_ErrFlag == 0 ){
		// 通常の申告書印刷
		if( (m_Vprn.IsContinueMode()==FALSE) || (m_PrintCmInfo.PRtype&0x80) ){
			if( FromPreview == 0 ){	
				if( m_pSnHeadData->IsMiddleProvisional() ){
					if( PrntBasisSelect() ){
						return;
					}
				}
				// 印刷共有情報のクリア
				ClearPrintCmInfo();

				GetPrnExSubInfo2();	// 特定収入計算表 個別出力設定		// 24/02/27wd_特定収入対応 add

				if( CheckSyzShinPrint() ){
					return;			// 印刷情報の更新
				}
				CPrnReportDlg	PrDlg;

				m_pShinInfo->ActViewVer = GetActViewVer();

				m_PrintCmInfo.pMvRec = m_MvRec;
				m_PrintCmInfo.pPrMv = m_PrMv;
				m_PrintCmInfo.pPrMove = m_PrMove;
//--> '15.02.25 INS START
				if( m_pMainFrame ){
					m_PrintCmInfo.zr_kbn = m_pMainFrame->GetZeirishiType();
				}
//<-- '15.02.25 INS END
//2015.08.04 INSERT START
				m_PrintCmInfo.Shrevtype = GetShRevType();
//2015.08.04 INSERT END
				//PrDlg.InitInfo( m_pSnHeadData, m_pZmSub, m_pShinInfo, &m_PrintCmInfo );
				for(int ii=0; ii<50; ii++)	m_PrintCmInfo.busu[ii]=m_BusuSave[ii];

// 24/04/01_246 add -->
				PrDlg.m_Kazei_DButton_sw	= Kazei_DButton_sw;
				PrDlg.m_Toku_DButton_sw		= Toku_DButton_sw;
				PrDlg.m_Zeiri_DButton_sw	= Zeiri_DButton_sw;
// 24/04/01_246 add <--

				PrDlg.InitInfo(m_pSnHeadData,m_pZmSub,m_pShinInfo,&m_PrintCmInfo,m_pMainFrame,&m_PRtableEx,&m_PGtable,&m_PRtableEx1);
				PrDlg.m_IsPreview = FALSE;		// ボタン表示 「印刷」
				INT_PTR st = PrDlg.DoModal();

				Kazei_DButton_sw	= PrDlg.m_Kazei_DButton_sw;
				Toku_DButton_sw		= PrDlg.m_Toku_DButton_sw;
				Zeiri_DButton_sw	= PrDlg.m_Zeiri_DButton_sw;

				if( st != IDOK ){
					return;			// 処理中断
				}
			}
		}																
		if( !(m_PrintCmInfo.PRtype&0x80) ){	// ＯＣＲ印刷
//2015.08.04 REVERSE START
			m_Vprn.SetContinueMode( 0, m_PrintCmInfo.PgMx );
//2015.08.04 REVERSE END
		}
	}
/*	if( m_swFork ){	// 印刷 or CDR出力
		AfxGetMainWnd()->PostMessageA( WM_CLOSE, 0 );
		return;
	}*/

//// 24/06/13_税務代理書面 add -->
//	((CMainFrame*)m_pMainFrame)->CallWriteTaxAgentDB(m_pZmSub,m_pSnHeadData,this);
//// 24/06/13_税務代理書面 add <--

	ICSFormView::OnFilePrint();
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 引数	pZmSub		：	公益クラスのポインタ
//		pMainFrame	：	親ウィンドウクラスのポインタ
//		pSnHeadData	：	消費税ヘッダ情報のポインタ
//		pShinInfo	：	共通情報
//		pTblhdl		：	テーブルハンドリングクラス
//		pTbPar		：	仕入控除用テーブル
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::Init( CDBNpSub *pZmSub, CMainFrame *pMainFrame, CSnHeadData *pSnHeadData, SYC_SHININFO *pShinInfo, CTblhdl *pTblhdl, _TB_PAR *pTbPar )
{
	ASSERT( pZmSub );
	if( pZmSub == NULL ){
		return -1;
	}
	m_pZmSub = pZmSub;

	ASSERT( pMainFrame );
	if( pMainFrame == NULL ){
		return -1;
	}
	m_pMainFrame = pMainFrame;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pShinInfo );
	if( pShinInfo == NULL ){
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT( pTblhdl );
	if( pTblhdl == NULL ){
		return -1;
	}
	m_pTblhdl = pTblhdl;

	ASSERT( pTbPar );
	if( pTbPar == NULL ){
		return -1;
	}
	m_pSkjCnvtbl = pTbPar;

	// フォント設定
/*	m_StaticB01.SetFontWeight( FW_BOLD );
	m_StaticB02.SetFontWeight( FW_BOLD );
	m_StaticB03.SetFontWeight( FW_BOLD );
	m_StaticB04.SetFontWeight( FW_BOLD );
	m_StaticB05.SetFontWeight( FW_BOLD );
	m_StaticB06.SetFontWeight( FW_BOLD );
	m_StaticB07.SetFontWeight( FW_BOLD );
	m_StaticB08.SetFontWeight( FW_BOLD );
	m_StaticB09.SetFontWeight( FW_BOLD );*/
	m_StaticB51.SetFontWeight( FW_NORMAL );
	m_StaticB52.SetFontWeight( FW_NORMAL );
	m_StaticB53.SetFontWeight( FW_NORMAL );
	m_StaticB54.SetFontWeight( FW_NORMAL );
	m_StaticB55.SetFontWeight( FW_SEMIBOLD );
	m_StaticB56.SetFontWeight( FW_SEMIBOLD );
	m_StaticB57.SetFontWeight( FW_SEMIBOLD );
	m_StaticB58.SetFontWeight( FW_SEMIBOLD );
//	m_StaticHelp.SetFontWeight( FW_SEMIBOLD );
	m_StaticHelp.SetFontWeight( FW_NORMAL );
	m_StaticI06.SetFontWeight( FW_SEMIBOLD );
	m_StaticHelp.SetTextColor( RGB(0x00, 0x99, 0x00) );
//	m_StaticHelp.SetTextColor( RGB(247, 193, 106) );
	m_StaticC01.IsSmallFont( 1 );

	// 印刷微調整情報の取得
	InitPrintInfo();

	// 画面の初期化
//	InitDisp();

//	m_IsInitFlg = TRUE;

//--> '15.07.21 INS START
	InitMynumberControl();
//<-- '15.07.21 INS END

//--> '15.08.27 INS START
	m_ShRevType = GetShRevType();
//<-- '15.08.27 INS END

	// 会社切り替えを行うとメッセージが残ったままになるため、初期化しておく
	GetDlgItem ( IDC_STATIC_HELP )->SetWindowText ( "" );

	return 0;
}

BOOL CDBSyzShinMainView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CWnd*	wnd;
	int		id;

	if( pMsg->message == WM_KEYDOWN ){
		if( m_pMainFrame ){
			if( m_pMainFrame->IsKeyoff() ){
				return TRUE;
			}
		}

		if( pMsg->wParam == VK_RETURN ){
			// OnBnClickedInputBtnが来ない謎の現象を回避するため harino ('14.03.09)
			wnd = GetFocus();
			if( wnd ){
				id = wnd->GetDlgCtrlID();
				if( id == IDC_INPUT_BTN) {
					PostMessage( WM_COMMAND, IDC_INPUT_BTN );
					return TRUE;
				}
			}
		}
	}

	return ICSFormView::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------
// フォーカスの初期値セット harino ('14.03.09)
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetDefFocus()
{
	c_set(IDC_INPUT_BTN);
}

//-----------------------------------------------------------------------------
// 画面の初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::InitDisp()
{
	int		ey, em, ed;
//	char	BUF[128],  CBF[128];
	char	buf[128]={0}, cbf[128]={0};
	int		gengo;

	// 基本情報
/*	if( Vol1.S_SGN2 & 0x80 )	Kenei = 0x01;	// 兼業事業者
	else						Kenei = 0x00;	// 単一事業者
	if( Vol1.S_SGN4 & 0x80 )	Anbun = 1;		// 個別対応
	else						Anbun = 0;		// 比例配分
	Kani = (int)(Vol1.S_SGN2 & 0x0f);			// 業種区分
	Skbn = (int)(Snh.Sn_SKKBN&0xff);			// 消費税集計の為のサイン生成
	toku = (int)(Snh.Sn_TOKUR & 0xff);			// 特例計算の適用 ★[13'09.11]
*/
	m_toku = (int)(m_pSnHeadData->Sn_TOKUR&0xff);	// 特例計算の適用

	// 決算期間の期末年月日より締め日を取得
	//  年
	CVolDateDB	voldate;
//	voldate.db_datecnv( m_pZmSub->zvol->ee_ymd, &ey, 0, 0 );
	voldate.db_datecnvGen( 0, m_pZmSub->zvol->ee_ymd, &gengo, &ey, 0, 0 );
	//  月
	int	mmdd = m_pZmSub->zvol->ee_ymd % 10000;
	em = mmdd / 100;
	//  日
	ed = mmdd % 100;
	// 締め日
	int Matu = ChkLastDayEntry( em, ed, (IsLeapYear(ey) ? 1:0) );

	// １ヶ月目のデータの至る月日より締め日を取得
	m_pZmSub->minfo->MoveFirst();

	// 年
//	voldate.db_datecnv( m_pZmSub->minfo->emd, &ey, 0, 0 );
	voldate.db_datecnvGen( 0, m_pZmSub->minfo->emd, &gengo, &ey, 0, 0 );
	// 月
	mmdd = m_pZmSub->minfo->emd % 10000;
	em = mmdd / 100;
	// 日
	ed = mmdd % 100;
	// 締め日	
	if( ChkLastDayEntry(em, ed, (IsLeapYear(ey) ? 1:0)) != Matu ){
		Matu = 0;
	}

	// 申告区分
	if( m_pSnHeadData->Sn_SKKBN == 0x00 ){
		m_pSnHeadData->Sn_SKKBN = 0x01;
	}
	// 中間申告時中間回数
	switch( m_pSnHeadData->Sn_SKKBN&0xff ){
		case 2 :	// 中間
		case 4 :	
			switch( m_pSnHeadData->Sn_MCOUNT ) {
				case 3 :	// 年３回
					break;
				case 11:	// 年11回	
					break;
				default:	// 年１回
					m_pSnHeadData->Sn_MCOUNT = 1;	
					break;
			}
			m_pSnHeadData->Sn_KZTNSK &= 0xf0;
			break;

		default:	// 確定
			break;
	}

	CString	str;
	if( m_IsDispedFlg == FALSE ){
		// 会社番号
		str.Format( _T("%08d"), m_pZmSub->zvol->v_cod );
		GetDlgItem(IDC_STATIC_B51)->SetWindowTextA( str );
		// 会社名
		GetDlgItem(IDC_STATIC_B52)->SetWindowTextA( m_pZmSub->zvol->c_nam );
		// 決算期間 自
		CVolDateDB	voldate;
		int		yymmdd=0, yy=0, mm=0, dd=0;
		int		g = 0;
		CString gengo;
		//db_datecnv( m_pZmSub->zvol->ss_ymd, (int *)&yymmdd, 0, 0 );
		voldate.db_datecnvGen( 0, m_pZmSub->zvol->ss_ymd, &g, (int *)&yymmdd, 0, 0 );
		yy = yymmdd / 10000;
		mmdd = yymmdd % 10000;
		mm = mmdd / 100;
		dd = mmdd % 100;
		voldate.db_vd_NoToStrGen( g, gengo );
		//str.Format( _T("平成  %02d 年  %02d 月  %02d 日"), yy, mm, dd );
		str.Format( _T(gengo + "  %02d 年  %02d 月  %02d 日"), yy, mm, dd );
		GetDlgItem(IDC_STATIC_B53)->SetWindowTextA( str );
		// 決算期間 至
		//db_datecnv( m_pZmSub->zvol->ee_ymd, (int *)&yymmdd, 0, 0 );
		yymmdd=0;
		g=0;
		voldate.db_datecnvGen( 0, m_pZmSub->zvol->ee_ymd, &g, (int *)&yymmdd, 0, 0 );
		yy = yymmdd / 10000;
		mmdd = yymmdd % 10000;
		mm = mmdd / 100;
		dd = mmdd % 100;
		voldate.db_vd_NoToStrGen( g, gengo );
		//str.Format( _T("平成  %02d 年  %02d 月  %02d 日"), yy, mm, dd );
		str.Format( _T(gengo + "  %02d 年  %02d 月  %02d 日"), yy, mm, dd );
		GetDlgItem(IDC_STATIC_B54)->SetWindowTextA( str );

		// 課税方式
		if( (m_pZmSub->zvol->s_sgn2&0x0f) > 1 ){
			str = _T("簡易課税");
		}
		else{
			str = _T("原則課税");
		}
		GetDlgItem(IDC_STATIC_B55)->SetWindowTextA( str );
		// 控除方式
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
			str = _T("控除方式" );
		}
		else{
			str = _T("特例計算" );
		}
		GetDlgItem(IDC_STATIC_B06)->SetWindowTextA( str );
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
			if( m_pSnHeadData->IsKobetuSiireAnbun() ){
				str = _T("個別対応" );
			}
			else{
				str = _T("比例配分" );
			}
		}
		else{
			if( m_toku ){
				str = _T("適用有り" );
			}
			else{
				str = _T("適用無し" );
			}
		}
		GetDlgItem(IDC_STATIC_B56)->SetWindowTextA( str );
		// 事業区分
		if( m_pZmSub->zvol->ind_type == 0x00 ){
			str = _T("法人事業者");
		}
		else{
			str = _T("個人事業者");
		}
		GetDlgItem(IDC_STATIC_B57)->SetWindowTextA( str );

		long kazeiKaishibi = GetKazeiKaishibi();
		if( kazeiKaishibi == 0 ) {
			// 課税期間の開始日　未登録
			GetDlgItem( IDC_STATIC_B14 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_STATIC_B58 )->ShowWindow( SW_HIDE );
		}
		else {
			GetDlgItem( IDC_STATIC_B14 )->ShowWindow( SW_SHOW );
			GetDlgItem( IDC_STATIC_B58 )->ShowWindow( SW_SHOW );

			CString strDate, strGengo;
			int wareki = 0, gengoNo = 0;

			db_datecnvGen( 0, kazeiKaishibi, &gengoNo, &wareki, 0, 0 );
			gengoNo = db_vd_GetGenNo( wareki );
			db_vd_NoToStrGen( gengoNo, strGengo );
			strDate.Format( _T( "%s%02d年%02d月%02d日" ), strGengo, ( wareki / 10000 ), (( wareki / 100 ) % 100 ), ( wareki % 100 ));
			GetDlgItem( IDC_STATIC_B58 )->SetWindowText( strDate );
		}
	}

	//== 特殊選択 ==
	//--------------------------------------->2019.12.20.yokono
	//if (m_pSnHeadData->Sn_Sign4 & 0x01) {
	//	NyuryokuLock();
	//}
	//<--------------------------------------------------------
	//else {
		//------------------------------>2019.12.23.、2020.1.7.、2020.1.24.、2020.1.29.yokono
        //入力ロックを解除した際

		//チェックボックス「非連動処理を行う」、「経過措置対象課税資産の譲渡等有り」、「（軽減税率）税額計算の特例を適用する」
		//非連動マスターのときは「非連動処理を行う」非表示、「経過措置対象課税資産の譲渡等有り」、「（軽減税率）税額計算の特例を適用する」表示
		if (m_Util.IsUnConnectMst(m_pZmSub->zvol)) {
			m_Check21.EnableWindow(FALSE);
			m_Check5.EnableWindow(TRUE);
			//m_Check7.EnableWindow(TRUE);
			SetTkreiChk();
		}
		else {
			m_Check21.EnableWindow(!IsHistoryMode());
		}

		//コンボボックス「課税期間」
		ChangeComboKessanKikan();

		//<-----------------------------------------------

		//*****************************************************
		// 非連動処理を行う
		//*****************************************************
		if (m_Util.IsUnConnectMst(m_pZmSub->zvol)) {
			m_pSnHeadData->Sn_Sign4 &= 0x7f;
			m_pSnHeadData->Sn_Sign4 |= 0x80;
			m_Check21.SetCheck(1);
			m_Check21.EnableWindow(FALSE);
		}
		else {
			if (m_pSnHeadData->Sn_Sign4 & 0x80) {
				m_Check5.EnableWindow(TRUE);
	//--> '15.08.21 INS START
				if (m_IsAbleTkkz) {
					m_Check6.EnableWindow(TRUE);
				}
				else {
					m_Check6.EnableWindow(FALSE);
				}
	//<-- '15.08.21 INS END
				m_Check21.SetCheck(1);
			}
			else {
				m_Check5.EnableWindow(FALSE);
	//--> '15.08.21 INS START
				m_Check6.EnableWindow(FALSE);
	//<-- '15.08.21 INS END
				//<--------------------------------
				m_Check21.SetCheck(0);
			}

			//特例計算チェック		
			SetTkreiChk();
		}

		//*****************************************************
		// 積上げ計算を行う (課税標準額の消費税額)		②
		// 積上げ計算を行う (控除対象仕入税額)			③
		//*****************************************************
		SetTumiageChk();

		//*****************************************************
		// 簡易計算[税込（税抜）の本体価格より集計]④
		//*****************************************************
		SetKaniKeisanChk();

		//*****************************************************
		// 個別対方式を期間計で計算する⑤
		//*****************************************************
		if (m_Util.IsUnConnectMst(m_pZmSub->zvol)) {
			// 非連動
	//		Snd.Sn_Sign2 &= 0xb0;
			m_pSnHeadData->Sn_Sign2 &= 0xbf;
			m_Check4.EnableWindow(FALSE);
		}
		else {
			// 連動
			if ((m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) || (m_pSnHeadData->IsKobetuSiireAnbun() == FALSE) || m_SimpSw) {
				m_Check4.EnableWindow(FALSE);
			}
			else {
				if (m_pSnHeadData->Sn_Sign2 & 0x40) {
					m_Check4.SetCheck(1);
				}
				else {
					m_Check4.SetCheck(0);
				}
			}
		}

		//*****************************************************
		// 経過措置対象課税資産の譲渡有⑥
		//*****************************************************
		if (m_pSnHeadData->SVmzsw == 1) {
			m_Check5.SetCheck(1);
		}
		else {
			m_Check5.SetCheck(0);
		}

		//*****************************************************
		// 特定課税仕入れ有り
		//*****************************************************
		//--> '15.08.21 INS START
		if (m_pSnHeadData->m_DispTabSgn & 0x01) {
			m_Check6.SetCheck(1);
		}
		else {
			m_Check6.SetCheck(0);
		}
		//<-- '15.08.21 INS END

		//*****************************************************
		// （軽減税率）税額計算の特例を適用する
		//*****************************************************
		//-->特例計算チェック追加
		TimePair time = GetCurrentKessanKikan();
		int smd = CTimeControl::GetIntTime( time.first );
		if( smd >= ICS_SH_INVOICE_DAY ){
			m_Check7.SetCheck(0);
			m_Check7.EnableWindow(FALSE);
			m_btn1.ShowWindow(SW_HIDE);
		}
		else{
			if (m_pSnHeadData->m_DispTabSgn & 0x80) {
				m_Check7.SetCheck(1);
				m_btn1.ShowWindow(SW_SHOW);
			}
			else {
				m_Check7.SetCheck(0);
				m_btn1.ShowWindow(SW_HIDE);
			}
		}

		//*****************************************************
		// 令和2年4月1日改正チェック
		//*****************************************************
		if( m_pZmSub->zvol->ee_ymd >= ICS_SH_INVOICE_DAY ){
			m_Check9.ShowWindow( SW_HIDE );
		}
		else{
			m_Check9.ShowWindow( SW_SHOW );

			long symd = 0, eymd = 0;
			if ( !( m_pSnHeadData->Sn_SKKBN % 2 ) ) {
				symd = m_pSnHeadData->Sn_MDAYS;
				eymd = m_pSnHeadData->Sn_MDAYE;
			}
			else {
				symd = m_pSnHeadData->Sn_KDAYS;
				eymd = m_pSnHeadData->Sn_KDAYE;
			}
			if ( eymd >= R03_SDAY ) {
				SetR02Chk();
			}
			else {
				if ( m_IsAbleR02Reform ) {
					m_Check9.EnableWindow ( TRUE );
				}
				else {
					m_Check9.EnableWindow ( FALSE );
				}
	
				if ( m_pSnHeadData->m_DispTabSgn & 0x1000 ) {
					m_Check9.SetCheck ( 1 );
				}
				else {
					m_Check9.SetCheck ( 0 );
				}
			}
		}

		//*****************************************************
		// 2割特例計算を使用する
		//*****************************************************
		//----->２割特例計算を使用する('23.05.01)
		Set2wariChk();

		if( m_Util.IsUnConnectMst( m_pZmSub->zvol ) ){
			GetDlgItem( IDC_STATIC_C01 )->SetWindowTextA( "※2割特例計算を使用する場合、\n　会計基本情報で「課税開始日」を設定してください。" );
		}
		else{
			GetDlgItem( IDC_STATIC_C01 )->SetWindowTextA( "※2割特例計算を使用する場合、\n　会社登録で「課税開始日」を設定してください。" );
		}
		//<-----


		// 履歴選択時の制御
		if (IsHistoryMode()) {	// ('14.03.27)
			m_Check1.EnableWindow(FALSE);
			m_Check2.EnableWindow(FALSE);
			m_Check5.EnableWindow(FALSE);
			//--> '15.08.21 INS START
			m_Check6.EnableWindow(FALSE);
			//<-- '15.08.21 INS END

			//-->特例計算チェック追加
			m_Check7.EnableWindow(FALSE);
			m_IsAbleTkrei = FALSE;

			m_btn1.ShowWindow(SW_HIDE);
			//<------------------------

			m_Check9.EnableWindow ( FALSE );
			m_IsAbleR02Reform = FALSE;

			m_Check10.EnableWindow( FALSE );
		}
	//}

	if ( InLockInput() ) {
		NyuryokuLock();
	}

	// 仕入控除税額の明細書(平成24年3月以前提出様式)⑦
//	if( m_pSnHeadData->Sn_ZeiHou & 0x80 )	m_Check8.SetCheck( 0 );	 
//	else									m_Check8.SetCheck( 1 );

	//== 各処理サインの生成 ==

	// 課税売上高サイン
//	m_Million = 0;			// 1=５億円超え 2=４億５千万円以上５億円以下 3=４億５千万未満

	// 必要？？？
	m_pSnHeadData->Sn_KOJIN = 0x00;
	if( ((m_pZmSub->zvol->apno&0xf0)==0x10) || m_pZmSub->zvol->ind_type ){
		m_pSnHeadData->Sn_KOJIN = 0x01;
	}

	// 課税売上高５億円超えチェック
	CheckOver500Million();

	// みなし課税チェックのセット
	if( m_pSnHeadData->Sn_Sign4 & 0x40 ){
		m_CheckMinashi.SetCheck( 1 );	 
	}
	else{
		m_CheckMinashi.SetCheck( 0 );
	}

//2016.02.23 INSERT START
	Static_Update( m_pSnHeadData->Sn_Sign4 );
//2016.02.23 INSERT END

	if( IsHistoryMode() ){
		m_PastReport.SetFocus();
		m_PastReport.SetFocusedRow( max( GetHistoryCurSel(), 0 ) );
	}

	m_IsInitFlg = TRUE;
	m_IsDispedFlg = TRUE;

	return 0;
}

//------------------------------------------------------------>2019.12.19.、2020.1.24.、2020.1.29.yokono
//-----------------------------------------------------------------------------
// 入力ロック時に、チェックボックスなどを操作できないようにする
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::NyuryokuLock()
{
	//m_ComboKessanKikan.EnableWindow(FALSE);
	m_Check1.EnableWindow(FALSE);
	m_Check2.EnableWindow(FALSE);
	m_Check3.EnableWindow(FALSE);
	m_Check4.EnableWindow(FALSE);
	m_Check5.EnableWindow(FALSE);
	m_Check6.EnableWindow(FALSE);
	m_Check7.EnableWindow(FALSE);
	m_IsAbleTkrei = FALSE;
	m_btn1.ShowWindow(SW_HIDE);
	m_Check21.EnableWindow(FALSE);

	m_Check9.EnableWindow(FALSE);
	m_Check10.EnableWindow(FALSE);	//('23.05.01 Add)

}
//<-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 年度に合わせたプログラムの呼び出し
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::CallSyzProgram()
{
	char	path[_MAX_PATH];
	char	dev[20] = {0};
	char	cmline[128] = {0};

	PCOM_GetString( _T("ProgramDevice"), dev );

	((CMainFrame*)GetParentOwner())->ToggleMDITabs();

	// 年度チェック
	if( m_pZmSub->zvol->ee_ymd <= 20140331 ){
		// 平成26年4月改正対応前まで
//		wsprintf( path, _T("%s:\\ICSWin\\CMDS\\GR1b\\JOBS30\\H26\\DBSyzShin.exe"), dev );
		wsprintf( path, _T("%s:\\ICSWin\\CMDS\\GR1b\\JOBS30\\DBSyzShin_H26.exe"), dev );
		
		ICSFork8_Ex( AfxGetMainWnd()->m_hWnd,
					path, 
					cmline,
					SW_SHOWMAXIMIZED,
					SW_HIDE,//SW_MAXIMIZE 
					FALSE,//TRUE
					0x0004000
					);
	}
	else{
		// 平成26年4月改正対応以降
		wsprintf( path, _T("%s:\\ICSWin\\CMDS\\GR1b\\JOBS30\\DBSyzShin.exe"), dev );

		ICSFork8_Ex( AfxGetMainWnd()->m_hWnd,
					path, 
					cmline,
					SW_SHOWMAXIMIZED,
					SW_HIDE,//SW_MAXIMIZE 
					FALSE,//TRUE
					0x0004000
					);
	}



	return 0;
}

//-----------------------------------------------------------------------------
// フォーク終わり
//-----------------------------------------------------------------------------
// 引数	wParam	：
//		lParam	：
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::EndProcess( WPARAM wParam, LPARAM lParam )
{
	this->EnableWindow( TRUE );

	return 0;
}

//-----------------------------------------------------------------------------
// 閏年？
//-----------------------------------------------------------------------------
// 引数	year		：	チェック対象
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	閏年
//			FALSE	：	閏年でない
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::IsLeapYear( int year )
{
	if( ((year%4)==0) && ((year%100)!=0) || ((year%400)==0)){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// 末日チェック
//-----------------------------------------------------------------------------
// 引数	month	：	月
//		day		：	日
//		sw		：	0:通常年, 1:閏年
//-----------------------------------------------------------------------------
// 返送値	1	：	末日
//			0	：	末日以外の日
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::ChkLastDayEntry( int month, int day, int sw )
{
	// 引数チェック
	if( (month<1) || (12<month) ){
		return -1;
	}
	if( (sw<0) || (1<sw) ){
		return -1;
	}

	if( (day<1) || (day>LASTDAY_EVMONTH[sw][month]) ){
		return -1;
	}
	else{
		if( day == LASTDAY_EVMONTH[sw][month] ){
			return 1;
		}
	}
	return 0;
}

//★//[13'05.21]///
// 本表の計算(1)～(26)迄
// 前回が確定で今回が修正確定の場合納付税額を既確定へ転送する準備
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::AlreadyDecisionCalc( int type )
{
	if( m_pMainFrame != NULL ){
		m_pMainFrame->AlreadyDecisionCalc( type );
	}	
}

void CDBSyzShinMainView::OnCbnSelchangeSkkbnCmb()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	SetCurrentSkkbn();

	ChangeTyukanKikanTansyuku();
	ChangeComboTyukanKikan();
	GetComboTyukanKikan();

	ChangeComboKessanKikan();

	SetTumiageChk();

	SetKaniKeisanChk();

	// 中間申告の場合も２割特例不可
	if( curset.IsTyukanShinkoku() ){
		if ( !InLockInput() ) {
			m_Check10.SetCheck( 0 );	// サインは付表タブに移動するときに落とす
			m_Check10.EnableWindow( FALSE );
		}
	}
	else{
		//m_Check10.SetCheck( curset.m_DispTabSgn & 0x2000 );
		//m_Check10.EnableWindow( TRUE );
		Set2wariChk();
	}

//--> '15.11.14 INS START
	SetTkkzChk();
//<-- '15.11.14 INS END

	SetTkreiChk();

	SetR02Chk();

	SetActViewVer(&curset);

	if ( InLockInput() ) {
		NyuryokuLock();
	}
}

void CDBSyzShinMainView::OnCbnSelchangeMonthCmb()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	SetCurrentMcount();

	ChangeComboSkkbn();
	SetComboSkkbn();

	ChangeComboTyukanKikan();
	GetComboTyukanKikan();

	ChangeComboKessanKikan();
//--> '15.11.14 INS START
	SetTkkzChk();
//<-- '15.11.14 INS END

	SetTkreiChk();

	SetR02Chk();

	SetActViewVer(&curset);

	if ( InLockInput() ) {
		NyuryokuLock();
	}
}

void CDBSyzShinMainView::OnCbnSelchangeKikantCmb()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	SetCurrentMcount();

	ChangeComboSkkbn();
	SetComboSkkbn();

	ChangeTyukanKikanTansyuku();
	ChangeComboTyukanKikan();
	GetComboTyukanKikan();

	ChangeComboKessanKikan();

//--> '15.11.14 INS START
	SetTkkzChk();
//<-- '15.11.14 INS END

	SetTumiageChk();

	SetKaniKeisanChk();

	Set2wariChk();

	SetTkreiChk();

	SetR02Chk();
	SetActViewVer(&curset);

	if ( InLockInput() ) {
		NyuryokuLock();
	}
}

void CDBSyzShinMainView::OnBnClickedCheckTokusyu()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	ChangeComboSkkbn();
	SetComboSkkbn();

	ChangeTyukanKikanTansyuku();
	ChangeComboTyukanKikan();
	GetComboTyukanKikan();

	ChangeComboKessanKikan();
//--> '15.11.14 INS START
	SetTkkzChk();
//<-- '15.11.14 INS END

	SetTkreiChk();

	SetR02Chk();

	SetActViewVer(&curset);
	if ( InLockInput() ) {
		NyuryokuLock();
	}

}

void CDBSyzShinMainView::OnBnClickedCheckRirekisel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	// 履歴選択時は税額情報の引継ぎを行わない
	m_Drophistorysw = TRUE;
	SetCtrl();
	m_Drophistorysw = FALSE;
}

void CDBSyzShinMainView::OnCbnSelchangeTyukanKikanCmb()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	GetComboTyukanKikan();

	//yoshida190729
	SetTkreiChk();

	SetR02Chk();

	SetActViewVer(&curset);

	if ( InLockInput() ) {
		NyuryokuLock();
	}
}

//-----------------------------------------------------------------------------
// 呼出プログラム選択
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::ChkCallProgramType(int mode)
{
	long	ymd = 0;	// チェック対象年月日
	long	symd = 0;	// チェック対象期首年月日('15.02.19)
	
	// 履歴の年度取得
	if( IsHistoryMode() ){
		CHistoryReport::RecType rec = HistoryVec[GetHistoryCurSel()];
		CSnset	tmpSnset = rec.GetSnset();
		TimePair	date;
		if( tmpSnset.IsTyukanShinkoku() ){
			date = tmpSnset.GetSetTyukanKikan();
		}
		else{
			date = tmpSnset.GetSetKessanKikan();
		}
		CString	day = date.second.Format( _T("%Y%m%d") );
		ymd = atoi(day);
//--> '15.02.19 INS START
//-- '15.04.28 --
//		day = date.first.Format( _T("%Y%m%d") );
//---------------
		date = tmpSnset.GetSetKessanKikan();
		day = date.first.Format( _T("%Y%m%d") );
//---------------
		symd = atoi(day);
//<-- '15.02.19 INS END
	}
	else{
//		CHistoryReport::RecType rec = HistoryVec[0];
//		CSnset	tmpSnset = rec.GetSnset();
//		BYTE	tmpkbn = tmpSnset.GetKubun();
//		TimePair	pair;
//		if( (tmpkbn==kakutei) || (tmpkbn==kakutei_syuuse) ){
//			pair = tmpSnset.GetSetTyukanKikan();
//		}
//		else{
//			pair = tmpSnset.GetSetKessanKikan();
//		}
//		CString	day = pair.second.Format( _T("%Y%m%d") );
//		ymd = atoi(day);

		switch( m_pSnHeadData->Sn_SKKBN&0xff ){
			case 1 : case 3 :	// 確定・確定修正
				ymd = m_pSnHeadData->Sn_KDAYE;
				symd = m_pSnHeadData->Sn_KDAYS;
				break;
			default:			//中間・中間修正
				ymd = m_pSnHeadData->Sn_MDAYE;
//-- '15.04.28 --
//				symd = m_pSnHeadData->Sn_MDAYS;
//---------------
				symd = m_pSnHeadData->Sn_KDAYS;
//---------------
				break;
		}
		
	}

	//-------->150107
//-- '15.05.01 --
//	if( symd >= ICS_SH_SIXKIND_REFORM_DAY ){
//		m_SixKindFlg = TRUE;
//	}
//	else{
//		m_SixKindFlg = FALSE;
//	}
//---------------
	m_SixKindFlg = ((CMainFrame*)GetParentOwner())->IsSixKindKani(symd);
//---------------
	//<---------------------

	if( ymd <= 20140331 ){
		if( 1 ){
//			pastset = curset = CSnset(*m_pSnHeadData);
			if(mode == 0){
				pastset = curset;
			}
		}
		return 0;	// 過年度プログラム
	}
	else{
		return 1;	// 26年改正プログラム
	}
}

void CDBSyzShinMainView::OnBnClickedInputBtn()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( SyzEdit() ){
		ASSERT(m_pMainFrame);

		if( ChkCallProgramType() == 1 ){
			m_pMainFrame->SetActiveChild( 1 );
		}
		else{
#ifdef _DEBUG
			AfxMessageBox( _T("過年度プロ呼出") );
#else
			// 呼び出し
			if( IsHistoryMode() ){
				m_pMainFrame->CallSyzHistoryProgram( GetHistoryCurSel()+1 );
			}
			else{
				m_pMainFrame->CallSyzHistoryProgram( 0 );
			}
			return;
#endif
		}
	}
	else{
		RedrawGroupText();
	}
}

//-----------------------------------------------------------------------------
// 非連動処理を行う
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::OnBnClickedCheck21()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_Check21.GetCheck() )	{
		//if( ICSMessageBox( "非連動処理に切替えますか？\n※自動集計は解除されます。但し、集計された金額はそのまま残ります。", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2, 0, 0, this ) != IDYES ){
		CString msg;
		msg.Format ( _T( "非連動処理に切替えますか？\n※自動集計は解除されます。但し、集計された金額はそのまま残ります。" ) );
		if ( GetKaiseiVer() == ID_VER_SYZ31 ) {
			msg += _T( "\r\n※「課税標準額に対する消費税額」・「仕入れに対する消費税額」については、実額入力状態（背景色が水色）になります。" );
			msg += _T( "\r\n※背景色が水色の項目は、Deleteキーを押下しないと再集計されません。" );
		}
		if ( ICSMessageBox ( msg, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2, 0, 0, this ) != IDYES ) {
			m_Check21.SetCheck(0);		
			return;
		}
	}
	else{
		long symd = 0, eymd = 0;
		GetSelectKazeiKikan( symd, eymd );

		if( eymd >= ICS_SH_INVOICE_DAY ) {
			if( ICSMessageBox( "連動処理に切替えますか？\n※自動集計に切替わり、入力した金額は全て上書きされます。", ( MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ), 0, 0, this ) != IDYES ) {
				m_Check21.SetCheck(1);		
				return;
			}
		}
		else {
			if( ICSMessageBox( "連動処理に切替えますか？\n※自動集計に切替わり、入力した金額は全て上書きされます。"
							   "\n※連動時、合併マスターでは軽減税率特例計算は選択できません。", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2, 0, 0, this ) != IDYES ){
				m_Check21.SetCheck(1);		
				return;
			}
		}
	}

	curset.Sn_Sign4 &= 0x7f;
	m_pSnHeadData->Sn_Sign4 &= 0x7f;
	if( m_Check21.GetCheck() == 1 ){
		curset.Sn_Sign4 |= 0x80;
		m_pSnHeadData->Sn_Sign4 |= 0x80;
	}
	else {//チェックなし（連動の時）yoshida190911
		//「仕入税額の按分法が個別対応」かつ「連動」なら、特例５－（３）号を無効化させる　連動の時
		if ((m_pSnHeadData->m_s_sgn4 & 0x80) && !(m_pSnHeadData->Sn_Sign4 & 0x80)) {
			//５－（３）号落とす
			curset.m_DispTabSgn &= ~0x40;
			m_pSnHeadData->m_DispTabSgn &= ~0x40;
		}
	}


	// 経過措置対象課税資産の譲渡等
	if( m_pSnHeadData->SVmzsw == 1 ){
		m_Check5.SetCheck( 1 );
	}
	else{
		m_Check5.SetCheck( 0 );
	}
//--> '15.08.21 INS START
	// 特定課税仕入有り
	if( m_pSnHeadData->m_DispTabSgn & 0x01 ){
		m_Check6.SetCheck( 1 );
	}
	else{
		m_Check6.SetCheck( 0 );
	}

	//--->特例帳票チェック
	SetTkreiChk();
	if (m_pSnHeadData->m_DispTabSgn & 0x80) {
		m_Check7.SetCheck(1);
		m_btn1.ShowWindow(SW_SHOW);
	}
	else {
		m_Check7.SetCheck(0);
		m_btn1.ShowWindow(SW_HIDE);
	}
	//<-------------------------

//<-- '15.08.21 INS END
	// 非連動処理を行う
	if( curset.Sn_Sign4 & 0x80 ){
		TimePair time = GetCurrentKessanKikan();
		CString smd = time.first.Format("%Y%m%d");
		long symd = atoi(smd);
		if ( symd >= ICS_SH_INVOICE_DAY ) {
			m_Check1.EnableWindow( FALSE );
			m_Check2.EnableWindow( FALSE );
		}
		else {
			m_Check1.EnableWindow( m_K221sw );
			m_Check2.EnableWindow( m_H301sw );
		}
		m_Check5.EnableWindow( TRUE );
//--> '15.08.21 INS START
		if( m_IsAbleTkkz ){
			m_Check6.EnableWindow( TRUE );
		}
		else{
			m_Check6.EnableWindow( FALSE );
		}
//<-- '15.08.21 INS END

	}
	else{
		m_Check1.EnableWindow( m_K221sw );
		m_Check2.EnableWindow( m_H301sw );
		m_Check5.EnableWindow( FALSE );
//--> '15.08.21 INS START
		m_Check6.EnableWindow( FALSE );
//<-- '15.08.21 INS END

	}
}

LRESULT CDBSyzShinMainView::OnUserUnlock( WPARAM wParam, LPARAM lParam ) 
{
	if( !IsHistoryMode() ){
		curset.Sn_Sign4 = m_pSnHeadData->Sn_Sign4;
		return 0;
	}

	CDatabase	*pDb = m_pZmSub->m_database;
	CTime CurTime = CTime::GetCurrentTime();
	CString NowDay = CurTime.Format( _T("%Y%m%d ") );

	// ロック解除時 最終更新日の更新
	CString w;
	w.Format( _T("where seq = %d"), GetHistorySeq() );
	CString sq = _T("update ShinInfo set updateday = ") + NowDay + w + _T("  \n");

	pDb->ExecuteSQL(sq);

	m_PastReport.SetRecordText(GetHistoryCurSel(), CHistoryReport::saisyu,CTimeControl::GetDateStr( CurTime, _T("%2d年%2d月%2d日") ));

	return 0;
}


HBRUSH CDBSyzShinMainView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。

	if( pWnd ){
		if( (pWnd->m_hWnd==GetDlgItem(IDC_CHECK21)->m_hWnd) ||
			(pWnd->m_hWnd==GetDlgItem(IDC_CHECK1)->m_hWnd) ||
			(pWnd->m_hWnd==GetDlgItem(IDC_CHECK2)->m_hWnd) ||
			(pWnd->m_hWnd==GetDlgItem(IDC_CHECK3)->m_hWnd) ||
			(pWnd->m_hWnd==GetDlgItem(IDC_CHECK4)->m_hWnd) ||
			(pWnd->m_hWnd==GetDlgItem(IDC_CHECK5)->m_hWnd) ||
			(pWnd->m_hWnd==GetDlgItem(IDC_CHECK6)->m_hWnd) ||
			(pWnd->m_hWnd==GetDlgItem(IDC_CHECK7)->m_hWnd) ||//特例計算チェック追加　
			(pWnd->m_hWnd==GetDlgItem(IDC_CHECK9)->m_hWnd) ){ // 令和2年4月1日改正様式
			return (HBRUSH)m_hBrushBkgnd;
		}
	}

//2016.02.23 INSERT START
	//if( GetDlgItem(IDC_STATIC_INPUTLOCK) != NULL ){
	//	if(pWnd->m_hWnd==GetDlgItem(IDC_STATIC_INPUTLOCK)->m_hWnd){
	//		pDC->SetTextColor( RGB(255, 255, 255) );
	//		//pDC->SetTextColor( RGB(0, 0, 0) );
	//		return (HBRUSH)m_hBrushLock;
	//	}
	//}
//2016.02.23 INSERT END

	return hbr;
}

void CDBSyzShinMainView::OnBnClickedCheck1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_K221sw ){
		m_K221sw = 0;
	}
	else{
		m_K221sw = 1;
	}
	if( m_K221sw || m_H301sw ){
		m_SimpSw = 0;
		m_Check3.SetCheck( m_SimpSw );
	}
	else{
		m_Check3.SetCheck( m_SimpSw );
	}
	curset.Sn_TUMIAG &= 0xfe;
	m_pSnHeadData->Sn_TUMIAG &= 0xfe;
	if( m_K221sw ){
		curset.Sn_TUMIAG |= 0x01;		// 規則２２条１項の適用（積み上げ方式）
		m_pSnHeadData->Sn_TUMIAG |= 0x01;
	}
	if( (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) || (m_pSnHeadData->IsKobetuSiireAnbun()==FALSE) || m_SimpSw ){
		m_Check4.EnableWindow( FALSE );
	}
	else{
		m_Check4.EnableWindow( TRUE );
	}
}

void CDBSyzShinMainView::OnBnClickedCheck2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_H301sw ){
		m_H301sw = 0;
	}
	else{
		m_H301sw = 1;
	}
	if( m_H301sw || m_K221sw )	{
		m_SimpSw = 0;
		m_Check3.SetCheck( m_SimpSw );
	}
	else	{
		m_Check3.SetCheck( m_SimpSw );
	}
	curset.Sn_TUMIAG &= 0xfd;
	m_pSnHeadData->Sn_TUMIAG &= 0xfd;
	if( m_H301sw ){
		curset.Sn_TUMIAG |=0x02;		// 法第３０条第１項の適用（積み上げ方式）
		m_pSnHeadData->Sn_TUMIAG |=0x02;
	}

	if( (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) || (m_pSnHeadData->IsKobetuSiireAnbun()==FALSE) || m_SimpSw ){
		m_Check4.EnableWindow( FALSE );
	}
	else{
		m_Check4.EnableWindow( TRUE );
	}
}

void CDBSyzShinMainView::OnBnClickedCheck3()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( !m_SimpSw ){
		// 税額計算
		m_K221sw = m_H301sw = 0;
		curset.Sn_TUMIAG &= 0xfc;
		m_pSnHeadData->Sn_TUMIAG &= 0xfc;
		// 規則２２条１項の適用
		m_Check1.SetCheck( m_K221sw );
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
			// 法第２０条第１項の適用
			m_Check2.SetCheck( m_H301sw );
		}
		m_SimpSw = 1;
	}
	else{
		m_SimpSw = 0;
	}
	// 簡易計算
	curset.Sn_SPECIAL &= 0xfe;
	m_pSnHeadData->Sn_SPECIAL &= 0xfe;
	if( m_SimpSw ){
		curset.Sn_SPECIAL |= 0x01;
		m_pSnHeadData->Sn_SPECIAL |= 0x01;
	}

	if( (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) || (m_pSnHeadData->IsKobetuSiireAnbun()==FALSE) || m_SimpSw ){
		m_Check4.EnableWindow( FALSE );
	}
	else{
		m_Check4.EnableWindow( TRUE );
	}
}

void CDBSyzShinMainView::OnBnClickedCheck4()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	curset.Sn_Sign2 &= 0xbf;
	m_pSnHeadData->Sn_Sign2 &= 0xbf;
	if( m_Check4.GetCheck() == 1 ){
		curset.Sn_Sign2 |= 0x40;
		m_pSnHeadData->Sn_Sign2 |= 0x40;
	}
}

void CDBSyzShinMainView::OnBnClickedCheck5()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( curset.SVmzsw ){
		curset.SVmzsw = 0x00;			// 経過措置　無し
		m_pSnHeadData->SVmzsw = 0x00;
	}
	else{
		curset.SVmzsw = 0x01;	// 〃		 有り
		m_pSnHeadData->SVmzsw = 0x01;

		//経過措置チェックつけられた際、付表２の仕入税額の部分の手入力サインをONとする。連動と非連動で計算方法が違い、金額に差異がでる為yoshida190824
		//３１の場合のみ処理を通る用作成する
		if (GetActViewVer() == ID_VER_SYZ31) {
			CH31HyoView *pH31View = (CH31HyoView*)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(1));

		}


//		CH31HyoView	*pH31View = (CH31HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(H31_HONPYO_WND_IDX));

	}
}

void CDBSyzShinMainView::OnBnClickedCheck6()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( curset.m_DispTabSgn&0x01 ){
		curset.m_DispTabSgn &= ~0x01;
		m_pSnHeadData->m_DispTabSgn &= ~0x01;
	}
	else{
		curset.m_DispTabSgn |= 0x01;
		m_pSnHeadData->m_DispTabSgn |= 0x01;
	}
}

void CDBSyzShinMainView::OnBnClickedTnozeBtn()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CMiddlePyment dlg;
	dlg.pSyzShin = this;
	dlg.IsRireki = IsHistoryMode();
	dlg.sn_sign4 = m_pSnHeadData->Sn_Sign4;
	dlg.m_ActViewVer = GetActViewVer();

	// 8%と10%の期間をまたいでいる場合、KaiseiVerにより履歴seqが正常に反映されていない
	// そのため、税額情報の更新も正常に行われていない
	// 税額情報の更新のタイミングで、履歴seqを更新する
	if ( ( m_pZmSub->zvol->ss_ymd < ICS_SH_10PER_REFORM_DAY ) && ( m_pZmSub->zvol->ee_ymd >= ICS_SH_10PER_REFORM_DAY ) ) {
		m_pMainFrame->ChangeViewSnSeq ( m_pMainFrame->GetSnSeq(), GetActViewVer() );
	}

	if( !(m_pSnHeadData->Sn_Sign4 & 0x80) ){
		long symd = 0, eymd = 0;
		GetSelectKazeiKikan( symd, eymd );

		// 令和5年10月1日以降
		if( eymd >= ICS_SH_INVOICE_DAY ) {
			CBaseHyoView *pBaseView = GetHonHyoView();
			if( pBaseView != NULL ){
				char money[MONY_BUF_SIZE] = { 0 };
				CString tag, chtag;
				tag.Format( _T( "AAM00030" ));

				// 基準期間の課税売上高を退避しておく
				// pBaseView->UpdateSheetFromOuter();で、簡易本表内の基準期間の課税売上高が反映されるため
				GetMony( tag, chtag, money );

				//pBaseView->UpdateSheetFromOuter();
				pBaseView->ReGetCalqedMony();

				// 復元
				SetMony( tag, chtag, money );
			}
		}
	}

	// このタイミングで、８％か１０％かを見分けるべき？？
	// 金額の取込元を切り分ければ大丈夫？？

	if( dlg.DoModal() == IDOK ){
		m_pMainFrame->WriteSepData( 0x01 );
		m_pMainFrame->ReadSepData( 0x01 );
	}
}

//-----------------------------------------------------------------------------
// 課税売上高５億円チェック（連動用）
//-----------------------------------------------------------------------------
//	1: ５億円超え
//  0: 対象外及び５億円未満
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::CheckConnectTaxationSales()
{
	long			Ymds, Ymde;
	CString			ReviseMesAl;

	m_pShinInfo->isShinRevice = 0;

	// 集計期間の取得
	switch( m_pSnHeadData->Sn_SKKBN&0xff ){
		case 1 : case 3 :	//確定・確定修正
			Ymds = m_pSnHeadData->Sn_KDAYS;
			Ymde = m_pSnHeadData->Sn_KDAYE;
			break;
		default:				//中間・中間修正
			Ymds = m_pSnHeadData->Sn_MDAYS;
			Ymde = m_pSnHeadData->Sn_MDAYE;
			break;
	}

	int st = 0;
	if( 0/*非営利？*/ ){
		st = NPShinReviseCheck( 0, Ymds, Ymde, m_pZmSub, ReviseMesAl );
	}
	else{
		st = DBShinReviseCheck( 0, Ymds, Ymde, m_pZmSub, ReviseMesAl );
	}

	// 一旦クリアしておく
	CString	tmpStr;
	GetDlgItem( IDC_STATIC_HELP )->GetWindowText( tmpStr );
	GetDlgItem( IDC_STATIC_HELP )->SetWindowText( _T( "" ));

	if( st == 1 ){
		if( ReviseMesAl.GetLength() ){
			m_pShinInfo->isShinRevice = 1;
		}

		//CString	tmpStr;
		//GetDlgItem(IDC_STATIC_HELP)->GetWindowText( tmpStr );
		if( tmpStr != ReviseMesAl ){
			//ヘルプメッセージの内容が変わっていたら新しいものをセット
			GetDlgItem(IDC_STATIC_HELP)->SetWindowText( ReviseMesAl );
		}
		else {
			//メッセージ変更なければ元のメッセージを再セット 修正163595
			GetDlgItem(IDC_STATIC_HELP)->SetWindowText( tmpStr );
		}
		// 個人
		char	buf[128] = {0};
		memmove( buf, ReviseMesAl, 6 );
		if( strncmp(buf, _T("上半期"), 6) == 0 ){

//			m_pShinInfo->isShinRevice = 1;

			return 0;
		}

		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 課税売上高５億円チェック（非連動申告書用）
//-----------------------------------------------------------------------------
//	1: ５億円超え
//  0: 対象外及び５億円未満
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::CheckUnconnectTaxationSales()
{
	char		WKS[6], WKB[6], WKK[6], WKC[6], WK1[6], WK2[6], WK3[6], WK4[6], WKZ[6], CBF[20];
//	char		YMDS[4], YMDE[4];
	CString		ReviseMesAl;

	ReviseMesAl = _T("");
	GetDlgItem(IDC_STATIC_HELP)->SetWindowText(ReviseMesAl);

	// 簡易課税チェックはチェック対象外
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ){
		return 0;
	}

	memset( WKK, '\0', sizeof( WKK ) );
	memset( WKC, '\0', sizeof( WKC ) );
	memset( WKZ, '\0', sizeof( WKZ ) );

	// 課税売上高
	char	tmpMony[6]  = { 0 },
		    tmpMony2[6] = { 0 };
	CString	tag, chtag;

	// 10%改正対応
	if (GetActViewVer() == ID_VER_SYZ26) {
		tag = _T("AEB00030");
		m_pMainFrame->GetMony( tag, chtag, tmpMony );
		m_Arith.l_add( WKK, WKK, tmpMony );
		tag = _T("AEB00040");
		m_pMainFrame->GetMony( tag, chtag, tmpMony );
		m_Arith.l_add( WKK, WKK, tmpMony );
		tag = _T("AEB00050");
		m_pMainFrame->GetMony( tag, chtag, tmpMony );
		m_Arith.l_add( WKK, WKK, tmpMony );
	}
	else {
		if( IsInputKazeiUriage() ) {
			tag.Format( _T( "FFF00000" ));
			memset( tmpMony, '\0', MONY_BUF_SIZE );
			m_pMainFrame->GetMony( tag, chtag, tmpMony );

			memset( WKK, '\0', MONY_BUF_SIZE );
			memmove( WKK, tmpMony, MONY_BUF_SIZE );

			memset( WKZ, '\0', MONY_BUF_SIZE );
		}
		else {
			tag = _T("CRB00050");	// 付表２－１　課税売上高合計
			m_pMainFrame->GetMony(tag, chtag, tmpMony);
			memmove(WKK, tmpMony, MONY_BUF_SIZE);

			tag = _T("CRB00060");  //  付表２－１　 免税売上高合計  /* 2020.02.18. Furukawa */
			m_pMainFrame->GetMony(tag, chtag, tmpMony2);
			memmove(WKZ, tmpMony2, MONY_BUF_SIZE);
		}
	}
	
	// 集計期間の取得
	long	tmpSymd=0, tmpEymd=0;
	switch( m_pSnHeadData->Sn_SKKBN&0xff ){
		case 1 : case 3 :		//確定・確定修正
			tmpSymd = m_pSnHeadData->Sn_KDAYS;
			tmpEymd = m_pSnHeadData->Sn_KDAYE;
			break;
		default:				//中間・中間修正
			tmpSymd = m_pSnHeadData->Sn_MDAYS;
			tmpEymd = m_pSnHeadData->Sn_MDAYE;
			break;
	}

	int	MKno=0, SKno=0, EKno=11;


	// 入力年月　開始・最終設定
	if( m_pMainFrame ){
		m_pMainFrame->GetMofs();
	}
	SKno = m_pShinInfo->bil_stno;
	EKno = m_pShinInfo->bil_dtno;


	// 開始オフセットの取得
//	if( shin_ofst( (unsigned char*)&YMDS[1], &SKno, &Vol1, &Vol2 ) ){
//		SKno = 0;
//	}
	// 最終オフセットの取得
//	if( shin_ofst( (unsigned char*)&YMDE[1], &EKno, &Vol1, &Vol2 ) ){
//		EKno = 11;
//	}
	MKno = ( EKno - SKno ) + 1;

	m_Arith.l_add(WK4, WKK, WKZ); //  課税売上高 + 免税売上高 /* 2020.02.18 Furukawa */
	memmove(WK1, WK4, 6);

	m_Arith.l_input( WK2, "500000000" );		// ５億
	m_Arith.l_input( WK3, "450000000" );		// ４億５千万
	if( MKno != 12 ) {
		sprintf_s( CBF, sizeof( CBF ), "%d", MKno );
		m_Arith.l_input( WKB, CBF );				// * 分母
		m_Arith.l_input( WKS, "12" );				// / 分子
		m_Arith.l_div( WK1, WK1, WKB );				// sou = sou / 分母
		m_Arith.l_mul( WK1, WK1, WKS );				// sou = sou * 分子
		memmove( WKC, WK1, 6 );
	}
	else{
		memmove( WKC, WKK, 6 );
	}

	if( m_pSnHeadData->IsKobetuSiireAnbun() == FALSE ){
		CString	tmpStr;
		GetDlgItem(IDC_STATIC_HELP)->GetWindowText( tmpStr );
		ReviseMesAl.Empty();
		if( tmpStr != ReviseMesAl ){
			GetDlgItem(IDC_STATIC_HELP)->SetWindowText( ReviseMesAl );
		}
	}

	if( m_Arith.l_cmp(WK1, WK2) > 0 ){
		if( m_pSnHeadData->IsKobetuSiireAnbun() == FALSE ){
			if( m_Arith.l_cmp(WKC, WKK ) == 0 ) {
				ReviseMesAl = _T( "課税売上高が、５億円を超えています。\n" );
			}
			else{
				ReviseMesAl = _T( "課税売上高が、年換算で５億円を超えています。\n" );
			}
			ReviseMesAl += _T( "比例配分方式の選択も含めて注意してください。" );
			CString	tmpStr;
			GetDlgItem(IDC_STATIC_HELP)->GetWindowText( tmpStr );
			if( tmpStr != ReviseMesAl ){
				GetDlgItem(IDC_STATIC_HELP)->SetWindowText( ReviseMesAl );
			}
		}
		return 1;	// ５億越え
	}

	return 0;
}

void CDBSyzShinMainView::OnSize(UINT nType, int cx, int cy)
{
	ICSFormView::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	
	if( m_IsInitFlg ){
		if( GetMultiWindowType() == MULTIWINDOW_NONE ){
			if( m_PastReport.IsWindowEnabled() ){
				CRect	rcRep;
				m_PastReport.GetWindowRect( rcRep );
				this->ScreenToClient( rcRep );

				if( (rcRep.top!=m_rcHrep.top) && (m_rcHrep.top!=0) ){
					m_PastReport.MoveWindow( m_rcHrep, TRUE );
					m_PastReport.Populate();
				}
			}
		}

		// 個人番号・法人番号のコントロール位置調整
		MoveEditControl( m_svMynoInf.kubun, m_svMynoInf.sn_seq );


		// 履歴レポート移動
		CRect	rcRep, rcStatic;
		m_PastReport.GetWindowRect( rcRep );
		this->ScreenToClient( rcRep );
		GetDlgItem(IDC_STATIC_HELP)->GetWindowRect( rcStatic );
		this->ScreenToClient( rcStatic );

		if( m_pShinInfo ){
/*- '14.02.28 -*/
//			if( *(m_pShinInfo->pOver500MillionSw) != 1 ){ 
//
//				if(m_pShinInfo->isShinRevice != 1){
//					rcRep.top = rcStatic.top;
//					m_rcHrep = rcRep;
//					m_PastReport.MoveWindow( rcRep, TRUE );
//					m_PastReport.Populate();
//				}
//				else{
//					rcRep.top = rcStatic.bottom;
//					m_rcHrep = rcRep;
//					m_PastReport.MoveWindow( rcRep, TRUE );
//					m_PastReport.Populate();
//				}
//		
//		//		rcRep.top = rcStatic.top;
//		//		m_rcHrep = rcRep;
//		//		m_PastReport.MoveWindow( rcRep, TRUE );
//		//		m_PastReport.Populate();				
//			}
/*-------------*/
			CString	str;
			GetDlgItem(IDC_STATIC_HELP)->GetWindowTextA( str );
			str.TrimRight();
			if( str.GetLength() == 0 ){
				rcRep.top = rcStatic.top;
				m_rcHrep = rcRep;
				m_PastReport.MoveWindow( rcRep, TRUE );
				m_PastReport.Populate();
			}
			else{
				rcRep.top = rcStatic.bottom;
				m_rcHrep = rcRep;
				m_PastReport.MoveWindow( rcRep, TRUE );
				m_PastReport.Populate();

			}
/*-------------*/
		}
	}
}

//-----------------------------------------------------------------------------
// 仕入控除税額に関する明細書の変換テーブルの保存
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::SaveSkjCnvInfo()
{
	if( m_pMainFrame == NULL ){
		return -1;
	}
	if( m_pZmSub == NULL ){
		return -1;
	}

	// ユーザ情報の取得
	mctl_usertbl usertbl = m_pMainFrame->GetUserTbl();

	// TISW修正[22-0676] 2026/03/19 START
//	// 取り消し
//	CString	filter;
//	if( m_pZmSub->zvol->apno == 0x52 &&  m_pZmSub->zvol->m_ver == 0x11 ){
//		filter.Format( SKJ_OWNTBL_SQL, DBSYZSHIN_APLNAME, SKJ52_ITMNAME, usertbl.user_id );
//	}
//	else{
//		filter.Format( SKJ_OWNTBL_SQL, DBSYZSHIN_APLNAME, SKJ_ITMNAME, usertbl.user_id );
//	}
//	if( m_pZmSub->owntb->Requery(filter) != -1 ){
///*- '14.03.27 -*/
////		while( 1 ){
////			if( m_pZmSub->owntb->Move( 0 ) == ERR ){
////				break;
////			}
////			if( m_pZmSub->owntb->Delete() == ERR ){
////				continue;
////			}
////		}
///*-------------*/
//		int ownCnt = (int)m_pZmSub->owntb->GetRecordCount();
//		for( int i=ownCnt; i>0; i-- ){
//			m_pZmSub->owntb->SetAbsolutePosition( i );
//			m_pZmSub->owntb->Delete();
//		}
///*-------------*/
//	}
//
//	// 保存
//	int cnt = 0;
//	SH_SKJ_CNV	SkjCnv;
//	while( 1 ){
//		if( m_pTblhdl->th_rget( &SkjCnv, m_pSkjCnvtbl, cnt ) ) break;
//		m_pZmSub->owntb->AddNew();
//		m_pZmSub->owntb->apl_name = DBSYZSHIN_APLNAME;
//		if( m_pZmSub->zvol->apno == 0x52 &&  m_pZmSub->zvol->m_ver == 0x11 )
//			m_pZmSub->owntb->itm_name = SKJ52_ITMNAME;
//		else	m_pZmSub->owntb->itm_name = SKJ_ITMNAME;
//		m_pZmSub->owntb->itm_seq  = SkjCnv.SKJSeq;
//		m_pZmSub->owntb->u_id	   = usertbl.user_id;
//		m_Util.char_to_cstring( &m_pZmSub->owntb->code[0], SkjCnv.SKJCod, 8 );
//		m_pZmSub->owntb->vl[0]  = SkjCnv.HJNCod;
//		m_pZmSub->owntb->vl[1]  = SkjCnv.KJNCod;
//		m_pZmSub->owntb->Update();
//		cnt++;
//	}

	// 保存
	SkjUserSettingsAccessor skjSettingsAccess{ m_pZmSub, usertbl, m_pTblhdl, m_pSkjCnvtbl };
	skjSettingsAccess.SaveSkjSettings();

	// TISW修正[22-0676] 2026/03/19 END

	return 0;
}

void CDBSyzShinMainView::OnButtonF9()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	int st = 0;
	if( m_pSnHeadData->IsSoleProprietor() ){
		CSkjKjnConvert	dlg;
		dlg.m_isSkjUsrUnconfirmed = m_pMainFrame->IsSkjUsrUnconfirmed(); // TISW修正[22-0676] 2026/03/19
		dlg.InitInfo( m_pZmSub, m_pSnHeadData, m_pTblhdl, m_pSkjCnvtbl);
		st = (int)dlg.DoModal();
	}
	else{
		CSkjHjnConvert	dlg;
		dlg.m_isSkjUsrUnconfirmed = m_pMainFrame->IsSkjUsrUnconfirmed(); // TISW修正[22-0676] 2026/03/19
		dlg.InitInfo( m_pZmSub, m_pSnHeadData, m_pTblhdl, m_pSkjCnvtbl);
		st = (int)dlg.DoModal();
	}

//--> '14.03.24 INS START
	SaveSkjCnvInfo();
//<-- '14.03.24 INS END

	m_PrintType = 0;


	// 印刷情報のクリア
	ClearPsetInfo();
	// 印刷情報の更新
	ClearPrintCmInfo();

	GetPrnExSubInfo2();	// 特定収入計算表 個別出力設定		// 24/02/27wd_特定収入対応 add

	if( st == -2 ){
		m_PrintType = 1;
		m_Vprn.DoPrintPreview( this );
	}
	else if( st == -3 ){
		m_PrintType = 1;
		ICSFormView::OnFilePrint();
	}
}

void CDBSyzShinMainView::OnUpdateButtonF9(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if( (m_pZmSub==NULL) || (m_pZmSub->zvol==NULL) ){
		return;
	}
	else if( m_pSnHeadData == NULL ){
		return;
	}

	if(m_pZmSub != NULL && m_pSnHeadData != NULL){
		if( (m_pSnHeadData->Sn_Sign4&0x81) || (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) || m_Util.IsUnConnectMst(m_pZmSub->zvol) || m_pSnHeadData->IsSoleProprietor() ){
			pCmdUI->Enable( FALSE );
		}
		else{
			pCmdUI->Enable( TRUE );
		}
	}
}

//-----------------------------------------------------------------------------
// 印刷共有情報のクリア
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::ClearPrintCmInfo()
{
	memset( m_PrintCmInfo.PRtable, '\0', sizeof(m_PrintCmInfo.PRtable) );
	m_PRtableEx.RemoveAll();
	m_PGtable.RemoveAll();
	m_PRtableEx1.RemoveAll();
	m_PrintCmInfo.PgMx2 = 0;
	m_PrintCmInfo.PRtype = 0;
	m_PrintCmInfo.PgMx = 0;
}

//-----------------------------------------------------------------------------
// 金額取得
//-----------------------------------------------------------------------------
// 引数	tag		：	メインタグ
//		chtag	：	明細タグ
//		pArith	：	金額の取得先
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::GetMony( CString &tag, CString &chtag, char *pArith )
{
	if( m_pMainFrame != NULL ){
		return m_pMainFrame->GetMony(tag, chtag, pArith);
	}

	return -1;
}

//-----------------------------------------------------------------------------
// 金額取得
//-----------------------------------------------------------------------------
// 引数	tag		：	メインタグ
//		chtag	：	明細タグ
//		pArith	：	金額の取得先
//		kver	：	取得先ビューのバージョン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::GetMony( CString &tag, CString &chtag, char *pArith, int kver )
{
	if( m_pMainFrame != NULL ){
		return m_pMainFrame->GetMony( tag, chtag, pArith, kver );
	}

	return -1;
}

//-----------------------------------------------------------------------------
// 金額セット
//-----------------------------------------------------------------------------
// 引数	tag		：	メインタグ
//		chtag	：	明細タグ
//		pArith	：	セットする金額
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::SetMony( CString &tag, CString &chtag, char *pArith )
{
	if( m_pMainFrame != NULL ){
		return m_pMainFrame->SetMony( tag, chtag, pArith );
	}

	return -1;
}

//-----------------------------------------------------------------------------
// 金額セット
//-----------------------------------------------------------------------------
// 引数	tag		：	メインタグ
//		chtag	：	明細タグ
//		pArith	：	セットする金額
//		kver	：	セット先ビューのバージョン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::SetMony( CString &tag, CString &chtag, char *pArith, int kver )
{
	if( m_pMainFrame != NULL ){
		return m_pMainFrame->SetMony(tag, chtag, pArith, kver);
	}

	return -1;
}

//void CDBSyzShinMainView::OnUpdateShinPayment(CCmdUI *pCmdUI)
//{
//	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
//}

void CDBSyzShinMainView::OnUpdateZeirishiInf(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if( pCmdUI ){
		pCmdUI->Enable( FALSE );
	}
}

void CDBSyzShinMainView::OnUpdateButtonF4(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	int	Flag = 0;
	if( m_pMainFrame ){
		if( m_pMainFrame->IsAblePrint() ){
			Flag = 1;
		}
	}
	pCmdUI->Enable( Flag ? TRUE : FALSE );
}

void CDBSyzShinMainView::OnUpdateButtonF5(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	int	Flag = 0;
	if( m_pMainFrame ){
		if( m_pMainFrame->IsAblePrint() ){
			Flag = 1;
		}
	}
	pCmdUI->Enable( Flag ? TRUE : FALSE );
}

//void CDBSyzShinMainView::OnKomonsakiInf()
//{
//	// TODO: ここにコマンド ハンドラ コードを追加します。
//}

void CDBSyzShinMainView::OnCbnSelchangeTcountCmb()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	SetCurrentMcount();
	SetComboKikanTansyuku();

	ChangeComboSkkbn();
	SetComboSkkbn();

	ChangeTyukanKikanTansyuku();
	ChangeComboTyukanKikan();
	GetComboTyukanKikan();

	ChangeComboKessanKikan();
//--> '15.11.14 INS START
	SetTkkzChk();
//<-- '15.11.14 INS END

	SetTkreiChk();

	SetR02Chk();

	if ( InLockInput() ) {
		NyuryokuLock();
	}

}
///////////////////
// チェックリスト //
////////////////////
int CDBSyzShinMainView::SkjCheckList( int pgno )
{
	//表の作成
	int rs = 0;
	rs = PLIST_NLINE;
	LIST_PAR m_ListPar;

	ZeroMemory( &m_ListPar, sizeof( LIST_PAR ));
	m_ListPar.lsSx = COLM_OFS*3;		m_ListPar.lsSy = LINE_OFS*4;
	m_ListPar.lsLins = PLIST_NLINE+1;
	m_ListPar.lsRows = 3;
	m_ListPar.lsTlinWidth = LINE_OFS*3/2;
	m_ListPar.lsTrowWidth = COLM_OFS*20;
	m_ListPar.lsLinWidth = LINE_OFS;
	for(int i = 0; i < m_ListPar.lsRows-1; i++)	m_ListPar.lsRowWidth[i]	=  COLM_OFS*26;
	//線種
	m_ListPar.lsLwfpar.lwOtyp  = LKnrW1 | FKrect;
	m_ListPar.lsLwfpar.lwThtyp = LKnrW1;	m_ListPar.lsLwfpar.lwTvtyp = LKnrW1;
	m_ListPar.lsLwfpar.lwDhtyp = LKnrW0;	m_ListPar.lsLwfpar.lwDvtyp = LKnrW1;
	nList[0] = m_Vprn.MakeList( &m_ListPar );

	SF_PAR	PrtFont;
	CString	PrtBuf;

	memset( &PrtFont, 0, sizeof( SF_PAR ));

	int i;

	memset( &PrtFont, 0x00, sizeof SF_PAR);
	PrtFont.sf_hvsg = 0;	PrtFont.sf_fcsg = SFp_xc | SFp_yc;
	PrtFont.sf_xfld = 0;	PrtFont.sf_yfld = 0;
	PrtFont.sf_kpit = 0;	PrtFont.sf_ksiz = FNT174z;
	PrtFont.sf_erto = 0;	PrtFont.sf_deco = MAKELONG( LKnrWd, 0);	//アンダーライン

	//タイトル
	CString	buf, tapno, Titl;
	//業種
	switch( m_pZmSub->zvol->apno & 0xff ) {
	case 0x00:	tapno = "標準"; break;
	case 0x01:	tapno = "運送"; break;
	case 0x10:	tapno = "個人"; break;
	case 0x20:	tapno = "建設"; break;
	case 0x30:	tapno = "医療"; break;
	case 0x50:	tapno = "公益"; break;
	case 0x51:	tapno = "公益Ⅲ"; break;
	case 0x52:	tapno = "社会福祉"; break;
	case 0x58:	tapno = "学校"; break;
	case 0x59:	tapno = "宗教"; break;
	}
	buf = "消費税の還付申告に関する明細書連動のチェックリスト";
//	else	buf = "仕入控除税額に関する明細書連動のチェックリスト";

	Titl.Format( "%s：%s", (LPCTSTR)tapno, (LPCTSTR)buf);
	m_Vprn.DrawString( 0, COLM_OFS*5, (LINE_OFS*2)+(LINE_OFS/2), Titl, &PrtFont );

	//文字フォント等の指定
	memset( &PrtFont, 0x00, sizeof SF_PAR);
	PrtFont.sf_hvsg = 0;	PrtFont.sf_fcsg = SFp_xc | SFp_yc;
	PrtFont.sf_xfld = 0;	PrtFont.sf_yfld = 0;
	PrtFont.sf_kpit = 0;	PrtFont.sf_ksiz = FNT096z;
	PrtFont.sf_deco = 0;	PrtFont.sf_erto = 0;

	//会社
/*	if( m_Pset.psSigCoprn&0x2 ) {
		// 顧問先
		if( m_Pset.psSigCoprn&0x200 )	
				Titl.Format( "%-40.40s", CopNam );
		else	Titl.Format( "[%08d]%-40.40s", CopNum, CopNam );
	}
	else	{
		// マスター名
		if( m_Pset.psSigCoprn&0x200 )	
				Titl.Format( "%-40.40s", m_pZmSub->zvol->c_nam );
		else	Titl.Format( "[%08d]%-40.40s", m_pZmSub->zvol->v_cod, m_pZmSub->zvol->c_nam );
	}

*/	
	Titl.Format( _T("%s") , m_pSnHeadData->Sn_CONAM );
	m_Vprn.DrawString( 0, COLM_OFS*5, LINE_OFS*4, Titl, &PrtFont );

	//ページ
	int ttl_lc = m_pSkjCnvtbl->tp_rnum;
	int ttlpg = ttl_lc / (PLIST_NLINE);
	if( ttl_lc % (PLIST_NLINE) ) ttlpg++;

	Titl.Format( "Page %d/%d", pgno, ttlpg);
	m_Vprn.DrawString( 0, COLM_OFS*60, (LINE_OFS*3)+(LINE_OFS/2), Titl, &PrtFont );

	//表のタイトル
	m_Vprn.DrawString( nList[0], 0, 0, "科 　目 　 名 　称", &PrtFont );
	
	if( (m_pZmSub->zvol->apno&0xf0) == 0x10 ) {
		// 個人
		m_Vprn.DrawString( nList[0], 1, 0, "個人用・集計先項目", &PrtFont );
	}
	else if( (m_pZmSub->zvol->apno&0xf0) == 0x50 ) {
		// 非営利法人
		m_Vprn.DrawString( nList[0], 1, 0, "法人用・集計先項目", &PrtFont );
	}
	else	{
		if( m_pSnHeadData->IsSoleProprietor() )	
				m_Vprn.DrawString( nList[0], 1, 0, "法人用・集計先項目", &PrtFont );
		else	m_Vprn.DrawString( nList[0], 1, 0, "◎　法人用・集計先項目", &PrtFont );

		if( m_pSnHeadData->IsSoleProprietor() )
				m_Vprn.DrawString( nList[0], 2, 0, "◎　個人用・集計先項目", &PrtFont );
		else	m_Vprn.DrawString( nList[0], 2, 0, "個人用・集計先項目", &PrtFont );
	}

	PrtFont.sf_fcsg = 0x02;		// ｾﾝﾀｰﾘﾝｸﾞ
	int ofs = PLIST_NLINE * (pgno-1);

	CString str, dstr;
	char	kkbf[128];
	int lofs = 0;
	int cofs = 0;
	SH_SKJ_CNV	SkjCnv;

	for( i = 0; i < PLIST_NLINE; i++ )  {

		if( ofs >= ttl_lc ) break;

		if( m_pTblhdl->th_rget( &SkjCnv , m_pSkjCnvtbl , ofs ) ) break;
		ofs++;

		// 科目名称
		memset( kkbf, '\0', sizeof( kkbf ) );
		if( m_Util.GetKnrecName( m_pZmSub , (char *)SkjCnv.SKJCod, kkbf ) ) {
			dstr.Format( "%-14.14s", kkbf );
		}
		else	dstr = "―――――――";
		m_Vprn.DrawString( nList[0], cofs+0, lofs+i+1, dstr, &PrtFont );

		if( (m_pZmSub->zvol->apno&0xf0) == 0x10 ) {
			// 個人用　項目名称
			switch( SkjCnv.KJNCod ) {
				case 1 :	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "仕入金額 ① 事業　所得");	break;
				case 2 :	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "必要経費 ② 事業　所得");	break;
				case 3 :	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "固定資産 ③ 事業　所得");	break;
				case 5 :	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "必要経費 ⑤ 不動産取得");	break;
				case 6 :	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "固定資産 ⑥ 不動産所得");	break;
				case 8 :	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "仕入金額 ⑧ その他所得");	break;
				case 9 :	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "必要経費 ⑨ その他所得");	break;
				case 10:	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "固定資産 ⑩ その他所得");	break;
				default:	str.Format( "(00)%-24.24s",                  "集　計　外　科　目　×");	break;
			}
			m_Vprn.DrawString( nList[0], cofs+1, lofs+i+1, str, &PrtFont );
		}
		else	{
			// 法人用　項目名称
			switch( SkjCnv.HJNCod ) {
				case 1 :	str.Format( "(%02d)%-24.24s", SkjCnv.HJNCod, "商　品　仕　入　等　①" );	break;
				case 2 :	str.Format( "(%02d)%-24.24s", SkjCnv.HJNCod, "販売費・一般管理費　②" );	break;
				case 3 :	str.Format( "(%02d)%-24.24s", SkjCnv.HJNCod, "営　業　外　費　用　③" );	break;
				case 4 :	str.Format( "(%02d)%-24.24s", SkjCnv.HJNCod, "そ　　　の　　　他　④" );	break;
				case 6 :	str.Format( "(%02d)%-24.24s", SkjCnv.HJNCod, "固 　定  　資 　産　⑥" );	break;
				case 7 :	str.Format( "(%02d)%-24.24s", SkjCnv.HJNCod, "繰 　延  　資 　産　⑦" );	break;
				case 8 :	str.Format( "(%02d)%-24.24s", SkjCnv.HJNCod, "そ　　　の　　　他　⑧" );	break;
				default:	str.Format( "(00)%-24.24s",                  "集　計　外　科　目　×" );	break;
			}
			m_Vprn.DrawString( nList[0], cofs+1, lofs+i+1, str, &PrtFont );
			if( (m_pZmSub->zvol->apno&0xf0) == 0x50 )	continue;
			// 個人用　項目名称
			switch( SkjCnv.KJNCod ) {
				case 1 :	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "仕入金額 ① 事業　所得");	break;
				case 2 :	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "必要経費 ② 事業　所得");	break;
				case 3 :	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "固定資産 ③ 事業　所得");	break;
				case 5 :	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "必要経費 ⑤ 不動産取得");	break;
				case 6 :	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "固定資産 ⑥ 不動産所得");	break;
				case 8 :	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "仕入金額 ⑧ その他所得");	break;
				case 9 :	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "必要経費 ⑨ その他所得");	break;
				case 10:	str.Format( "(%02d)%-24.24s", SkjCnv.KJNCod, "固定資産 ⑩ その他所得");	break;
				default:	str.Format( "(00)%-24.24s",                  "集　計　外　科　目　×");	break;
			}
			m_Vprn.DrawString( nList[0], cofs+2, lofs+i+1, str, &PrtFont );
		}
	}

	m_ListPar.lsLwfpar.lwCol_h = Lcol[1];
	m_ListPar.lsLwfpar.lwCol_v = Lcol[0];
	m_ListPar.lsLwfpar.lwCol_o = Lcol[2];

	m_Vprn.DrawLwframe( nList[0], &m_ListPar.lsLwfpar );

	if( ofs >= ttl_lc ) return 1;
	else				return 0;

}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::EndJob( int swBeForked, int swForkErr, mctl_usertbl &usertbl, CString &Grpath )
{
//	if( m_UPsign ){
	if( 1 ){
		// ユーザー情報保存
		switch( swBeForked ){
			case 0 :
				SetInitInfo( usertbl );
				SetInitInfo2();	// 部数
// 24/02/27wd_特定収入対応 add -->
				// 特定収入計算表 個別出力設定
				if( m_pMapPrnEXSub.GetCount() > 0 || 
					m_pMapPrnEXSub_KAZ.GetCount() > 0 || 
					m_pMapPrnEXSub_ZEI.GetCount() > 0 ) {
					SetPrnExSubInfo();
				}
// 24/02/27wd_特定収入対応 add <--
				break;
			case 1 :
			case 2 :	// 20230614 一括印刷で出力した情報が財務dbの情報を上書きしてしまっていたため、出力時も保存の関数を通す。
				if( !swForkErr ){
					SetInitInfo( usertbl );
					SetInitInfo2();	// 部数
// 24/02/27wd_特定収入対応 add -->
					// 特定収入計算表 個別出力設定
					if( m_pMapPrnEXSub.GetCount() > 0 || 
						m_pMapPrnEXSub_KAZ.GetCount() > 0 || 
						m_pMapPrnEXSub_ZEI.GetCount() > 0 ) {
						SetPrnExSubInfo();
					}
// 24/02/27wd_特定収入対応 add <--
				}
				break;
			default:	
				break;
		}

		// 印刷微調整情報の保存
		SetPrAdjustInfo();
		// 消費税申告書情報の書き込み
//		if( SyzShin_UpDate() == -1 ){
//			return 1;
//		}
	}

	// 一括印刷
	PRT_TAXSETREC	PrtTaxsetrec;
	memset( &PrtTaxsetrec, '\0', sizeof( PRT_TAXSETREC ) );
	strcpy_s( PrtTaxsetrec.grno, sizeof(PrtTaxsetrec.grno), Grpath );
	strcpy_s( PrtTaxsetrec.proname, sizeof(PrtTaxsetrec.proname), "DBSyzShin.exe" );
	PrtTaxsetrec.symd = m_pZmSub->zvol->ss_ymd;
	PrtTaxsetrec.symd = m_pZmSub->zvol->ee_ymd;

	int	SzsnkFlg = 0;
	if( m_pZmSub->szsnk == NULL ) {
		m_pZmSub->SzsnkOpen( _T("Sn_SEQ = 0") );
		SzsnkFlg = 1;
	}
	else{
		m_pZmSub->szsnk->Requery( _T("Sn_SEQ = 0") ); 
	}

	if( m_pZmSub->szsnk->st != -1 ){
		switch( m_pZmSub->szsnk->Sn_SKKBN ){
			case 0x01 :
				if( m_pZmSub->szsnk->Sn_Syuusei&0x02 ){
					PrtTaxsetrec.repDivision = 6;	// 更正
				}
				else{
					PrtTaxsetrec.repDivision = 1;	// 確定
				}
				PrtTaxsetrec.sRpymd = m_pZmSub->szsnk->Sn_KDAYS;
				PrtTaxsetrec.eRpymd = m_pZmSub->szsnk->Sn_KDAYE;
				break;
			case 0x02 : 
				PrtTaxsetrec.repDivision = 2;		// 中間
				PrtTaxsetrec.sRpymd = m_pZmSub->szsnk->Sn_MDAYS;
				PrtTaxsetrec.eRpymd = m_pZmSub->szsnk->Sn_MDAYE;
				break;
			case 0x03 :
				PrtTaxsetrec.repDivision = 3;		// 修正確定
				PrtTaxsetrec.sRpymd = m_pZmSub->szsnk->Sn_KDAYS;
				PrtTaxsetrec.eRpymd = m_pZmSub->szsnk->Sn_KDAYE;
				break;
			case 0x04 :
				PrtTaxsetrec.repDivision = 4;		// 修正中間
				PrtTaxsetrec.sRpymd = m_pZmSub->szsnk->Sn_MDAYS;
				PrtTaxsetrec.eRpymd = m_pZmSub->szsnk->Sn_MDAYE;
				break;
			default   : 
				PrtTaxsetrec.repDivision = 0;		// 無
				PrtTaxsetrec.sRpymd = PrtTaxsetrec.symd;
				PrtTaxsetrec.eRpymd = PrtTaxsetrec.symd;
				break;	
		}
	}
	else{
		PrtTaxsetrec.repDivision = 0;				// 無
		PrtTaxsetrec.sRpymd = PrtTaxsetrec.symd;
		PrtTaxsetrec.eRpymd = PrtTaxsetrec.symd;
	}
	if( SzsnkFlg ) {
		m_pZmSub->SzsnkClose();
	}

	// プリントモジュール
	CPrintSch	*pPrintSch=NULL;
	pPrintSch = new CPrintSch;
	pPrintSch->SetPrtschYmd( m_pZmSub, &PrtTaxsetrec );
	delete( pPrintSch );

//---> '16.01.05 MOVE START
////--> '15.07.11 INS START
//	// マイナンバー
//	if( m_pMnec ){
//		delete m_pMnec;
//		m_pMnec = NULL;
//
//		NTBMOD8_EndProc();
//	}
////<-- '15.07.11 INS END
//<--- '16.01.05 MOVE END

	return 0;
}

#pragma optimize("", off)
//-----------------------------------------------------------------------------
// マイナンバー関連情報の破棄 ('16.01.05)
//  EndJob() 関数内で処理を行うと早過ぎるのか、まだマイナンバーの処理を行おうと
//  するので最後に持って行く為に別関数化。
//-----------------------------------------------------------------------------
// 引数	isYotei	：	予定申告？
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::FinalizeMyno( bool isYotei )
{
	if( m_pMnec ){

		//yoshida190816
		//予定申告の場合だけ解放されてない。なぜこの処理があるのか判断できない。
		//中垣内さんと相談の結果はずすことに。個人の予定申告を中心にチェック
//		if( isYotei == false ){
			delete m_pMnec;
			m_pMnec = NULL;
//		}

		NTBMOD8_EndProc();
	}
}
#pragma optimize("", on)

//-----------------------------------------------------------------------------
// 変数初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::InitVariable()
{
	m_IsDispedFlg = FALSE;
	memset( &m_svMynoInf, '\0', sizeof(SYZ_MYNO_SVINF) );
	m_svMynoInf.kubun = -1;
//--> '16.07.22 INS START
	memset( &m_Pset, '\0', sizeof(NPRT_PAR) );
//<-- '16.07.22 INS END

	return 0;
}

//-----------------------------------------------------------------------------
// 顧問先名称及びコード印刷情報の取得
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetNprtParInf( NPRT_PAR *pPrtSet )
{
	TgPackData	TPDat;			// コモン情報クラス
	CINF_REC	CoRec = {0};	// 会社情報レコード
	CTBL_PAR	CtPar = {0};	// 会社関連テーブル・パラメータ
	char		WKbf[20]= {0};
	char		Code[10]= {0};
	char		MDdm[4] = {0};
	char		MDev[4] = {0};
	if( m_pMainFrame == NULL ){
		return;
	}
//	CString		mdev;
//	mdev = m_pMainFrame->GetPrgMdev();
	// マスターデバイス

	if( PCOM_GetString( _T("MasterDevice"), MDev, MDdm ) == -1 ) {
//		errmsg =  _T("マスターデバイスが取得できません。" );
//		return -1;
		return;
	}
	sprintf_s( MDdm, sizeof(MDdm), _T("%s:"), MDev );

	memset( m_PrintCmInfo.CopNam, '\0', sizeof(m_PrintCmInfo.CopNam) );
	m_PrintCmInfo.CopNum = 0;

	if( GET_TGCoCode(&TPDat) == 1 ){
		CCoinfCtl *pCoinfCtl = NULL;
		if( (pCoinfCtl = new CCoinfCtl) == NULL ){
			return;
		}

		long	nFunc = 0L;
		// マスターデバイス
		sprintf_s( MDdm, sizeof( MDdm ), "%s:", MDev );
		if( pCoinfCtl->OpenFile(1, MDdm) == TRUE ){
			CtPar.tpDtyp = 1;
			CtPar.tpMtyp = 0x00;
			CtPar.tpApno = m_pZmSub->zvol->apno;
			memmove( WKbf, (LPCTSTR)&m_pZmSub->zvol->v_cod, 8 );
			m_Arith.l_pack( Code, WKbf, 8 );
			memmove( CtPar.tpCcod, Code, 4 );
			if( pCoinfCtl->FindRecFromMcod(nFunc, &CoRec, &CtPar) == 1 ) {
				pPrtSet->psSigCoprn |= 0x1;
				if( CoRec.ciCopsw & 0x01 ){
					pPrtSet->psSigCoprn |= 0x2;
				}
//--> '15.06.27 INS START
				memset( m_PrintCmInfo.HojinNo1, '\0', sizeof(m_PrintCmInfo.HojinNo1) );
				memmove( m_PrintCmInfo.HojinNo1, CoRec.ciHjnNo1, sizeof(CoRec.ciHjnNo1) );
				memset( m_PrintCmInfo.HojinNo2, '\0', sizeof(m_PrintCmInfo.HojinNo2) );
				memmove( m_PrintCmInfo.HojinNo2, CoRec.ciHjnNo2, sizeof(CoRec.ciHjnNo2) );
//<-- '15.06.27 INS END
			}
			pCoinfCtl->CloseFile();
		}
		delete( pCoinfCtl );
		pCoinfCtl = NULL;
		
		sprintf_s( m_PrintCmInfo.CopNam, sizeof(m_PrintCmInfo.CopNam), "%-40.40s", TPDat.TgName );	// 顧問先名
		m_PrintCmInfo.CopNum = TPDat.TgCode;														// 顧問先コード
	}

/*shimizu 課税取引のときに復活
	pPrtSet->psSigCoprn |= 0x100;
	if( (m_pZmSub->zvol->g_hsw & 0x01) ){
		pPrtSet->psSigCoprn |= 0x200;
	}
*/
}

//-----------------------------------------------------------------------------
// 出力情報選択
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::PrntBasisSelect()
{
	int st = 0;
	memset( m_PrintCmInfo.Zname, '\0', sizeof(m_PrintCmInfo.Zname) );
	memset( m_PrintCmInfo.Ztel, '\0', sizeof(m_PrintCmInfo.Ztel) );
	if( m_pShinInfo->isExistOffice ){
		if( !m_PrintCmInfo.OP2 ){
			if( m_PrintCmInfo.OPA ){
				memcpy( m_PrintCmInfo.Zname, m_pShinInfo->pO_DATA->o_ofcnam, sizeof(m_pShinInfo->pO_DATA->o_ofcnam) );
			}
			else{
				memcpy( m_PrintCmInfo.Zname, m_pShinInfo->pZ_DATA->z_zeinam, sizeof(m_pShinInfo->pZ_DATA->z_zeinam) );
			}
		}
		memset( m_PrintCmInfo.ZtelP, '\0', sizeof(m_PrintCmInfo.ZtelP) );
		if( !m_PrintCmInfo.OP9 ){
			memcpy( m_PrintCmInfo.ZtelP, m_pShinInfo->pZ_DATA->ji_denwa, sizeof(m_pShinInfo->pZ_DATA->ji_denwa) );
		}
//		Tel_cnvEx( Ztel, ZtelP, 16, 30 );
	}
	m_pSnHeadData->Sn_TYPE |= 0x01;

	return st;
}

void CDBSyzShinMainView::OnBnClickedMinashiChk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int Minashi_chk = 0;
	if( Minashi_chk = m_CheckMinashi.GetCheck() ){
		curset.Sn_Sign4 |=  0x40;
		m_pSnHeadData->Sn_Sign4 |=  0x40;
	}
	else{
		curset.Sn_Sign4 &= ~0x40; 
		m_pSnHeadData->Sn_Sign4 &= ~0x40;
	}

	ChangeComboTyukanKikan();
	ChangeComboKessanKikan();

	//yoshida190729
	/*
//--> '15.11.14 INS START
	SetTkkzChk();
//<-- '15.11.14 INS END

	SetTkreiChk();
	*/

	if( !m_ComboKessanKikan.GetCount() ){
		CString mes;
		mes += _T("選択された申告区分・期間では、「みなし課税期間」の変更はできません。\n");
		mes += _T("申告区分・期間をご確認ください。");
		ICSMessageBox( mes );
		//m_CheckMinashi.PostMessage( WM_LBUTTONDOWN );
		//m_CheckMinashi.PostMessage( WM_LBUTTONUP );

		// メッセージの切り分けがうまくいかなかった為、関数が2回通らないようにした。
		if( Minashi_chk ){
			m_CheckMinashi.SetCheck(0);
			curset.Sn_Sign4 &= ~0x40;
			m_pSnHeadData->Sn_Sign4 &= ~0x40;
		}
		else{
			m_CheckMinashi.SetCheck(1);
			curset.Sn_Sign4 |= 0x40; 
			m_pSnHeadData->Sn_Sign4 |= 0x40;
		}
		ChangeComboTyukanKikan();
		ChangeComboKessanKikan();

		SetTkkzChk();
		SetTkreiChk();

		SetR02Chk();

		if ( InLockInput() ) {
			NyuryokuLock();
		}

		return;
	}
	

	CString mes;
	if( Minashi_chk ){
		if( m_ComboKessanKikan.GetCount() ){
			mes.Format("みなし課税期間に切り替えます。\r\nよろしいですか？");
			//int st = ICSMessageBox( mes, MB_YESNO, 0 );
			int st = ICSMessageBox( mes, ( MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ), 0, 0, this );
			if( st == IDNO ){
				// 一旦入ってるチェックを外す
				m_CheckMinashi.SetCheck(0);
				curset.Sn_Sign4 &= ~0x40; 
				m_pSnHeadData->Sn_Sign4 &= ~0x40;

				ChangeComboTyukanKikan();
				ChangeComboKessanKikan();

				SetTkkzChk();
				SetTkreiChk();
				SetR02Chk();

				if ( InLockInput() ) {
					NyuryokuLock();
				}

				return;
			}
		}
	}
	else {
		if ( m_ComboKessanKikan.GetCount() ) {
			mes.Format ( _T( "みなし課税期間を解除します。\r\nよろしいですか？" ) );
			int st = ICSMessageBox( mes, ( MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ), 0, 0, this );;
			if ( st == IDNO ) {
				m_CheckMinashi.SetCheck(1);
				curset.Sn_Sign4 |= 0x40; 
				m_pSnHeadData->Sn_Sign4 |= 0x40;

				ChangeComboTyukanKikan();
				ChangeComboKessanKikan();

				SetTkkzChk();
				SetTkreiChk();
				SetR02Chk();

				if ( InLockInput() ) {
					NyuryokuLock();
				}

				return;
			}
		}
	}


	//上記エラーの場合、「特定課税仕入」と「特例計算」のチェックが外れるので、場所変更yoshida190728
	SetTkkzChk();
	
	SetTkreiChk();

	SetR02Chk();

	if ( InLockInput() ) {
		NyuryokuLock();
	}
}

void CDBSyzShinMainView::OnUpdateKomonsakiInf(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	BOOL	bSw = FALSE;
	if( m_pSnHeadData ){
		if( !(m_pSnHeadData->Sn_Sign4&0x01) ){
			bSw = TRUE;
		}
	}

	pCmdUI->Enable( bSw );
}
int CDBSyzShinMainView::ForkJob(){

	m_swBeFork = 0;
	m_swBeFork = m_pMainFrame->IsCalledBeFork();
	
	// 依頼として、このタイミングで集計処理を行って申告書にデータが即時反映されるようにして欲しいという依頼もありますが
	// 現段階（'19.02.26）では対応無しとします
	OnButtonF5();

	return 0;
}	
// 一括印刷初期設定
#include "PrnPrint.h"
#include "PrnPrintKani.h"
#include "PrnPrint_EX.h"
#include "PrnPrintKani_EX.h"
#include "PrnPrint_EX2.h"
#include "PrnPrintKani_EX2.h"

int CDBSyzShinMainView::SumUpPrintIntinitial( void )
{
	// 初期設定
//	CPrnPrint	*m_pPrnPrint;
//	m_pPrnPrint = new CPrnPrint();

	m_pShinInfo->ActViewVer = GetActViewVer();
				
	m_PrintCmInfo.pMvRec = m_MvRec;
	m_PrintCmInfo.pPrMv = m_PrMv;
	m_PrintCmInfo.pPrMove = m_PrMove;
//--> '16.07.21 INS START
	m_PrintCmInfo.Shrevtype = m_ShRevType;
//<-- '16.07.21 INS END

// 240308_h_160357 add -->
	m_Tokutei = 0;
// 240308_h_160357 add <--

//----->('23.05.01 Add)
	long eymd = 0;
	if (!(m_pSnHeadData->Sn_SKKBN % 2)) eymd = m_pSnHeadData->Sn_MDAYE;
	else								eymd = m_pSnHeadData->Sn_KDAYE;
//<-----
	if (GetActViewVer() == ID_VER_SYZ31) {
//----->('23.05.01 Add)
		if ((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000) ) {	//２割特例計算を使用する
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
				CPrnPrint_EX2	*m_pPrnPrintEx2;
				m_pPrnPrintEx2 = new CPrnPrint_EX2();
				//m_pPrnPrintEx->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, &m_PrintCmInfo);
				m_pPrnPrintEx2->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, &m_PrintCmInfo, &m_PRtableEx, &m_PGtable, &m_PRtableEx1);
				m_pPrnPrintEx2->m_swBeForked = m_swBeFork;
				//m_pPrnPrintEx2->SetPrintInfo();	// 24/07/10_個別タブから出力 del
				m_pPrnPrintEx2->SetPrintInfo(0x00);	// 24/07/10_個別タブから出力 add
				delete(m_pPrnPrintEx2);
			}
			else {
				CPrnPrintKani_EX2	*m_pPrnPrintKaniEx2;
				m_pPrnPrintKaniEx2 = new CPrnPrintKani_EX2();
				//m_pPrnPrintKaniEx->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, &m_PrintCmInfo);
				m_pPrnPrintKaniEx2->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, &m_PrintCmInfo, &m_PRtableEx, &m_PGtable, &m_PRtableEx1);
				m_pPrnPrintKaniEx2->m_swBeForked = m_swBeFork;
				//m_pPrnPrintKaniEx2->SetPrintInfo();	// 24/07/10_個別タブから出力 del
				m_pPrnPrintKaniEx2->SetPrintInfo(0x00);	// 24/07/10_個別タブから出力 add
				delete(m_pPrnPrintKaniEx2);
			}
		}
//<-----
		else {
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
				CPrnPrint_EX	*m_pPrnPrintEx;
				m_pPrnPrintEx = new CPrnPrint_EX();
				//m_pPrnPrintEx->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, &m_PrintCmInfo);
// 240308_h_160357 cor -->
				//m_pPrnPrintEx->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, &m_PrintCmInfo, &m_PRtableEx, &m_PGtable, &m_PRtableEx1);
// -----------------------
				m_pPrnPrintEx->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, &m_PrintCmInfo, &m_PRtableEx, &m_PGtable, &m_PRtableEx1, &m_Tokutei);
// 240308_h_160357 cor <--
				m_pPrnPrintEx->m_swBeForked = m_swBeFork;
				//m_pPrnPrintEx->SetPrintInfo();	// 24/07/10_個別タブから出力 del
				m_pPrnPrintEx->SetPrintInfo(0x00);	// 24/07/10_個別タブから出力 add
				delete(m_pPrnPrintEx);
			}
			else {
				CPrnPrintKani_EX	*m_pPrnPrintKaniEx;
				m_pPrnPrintKaniEx = new CPrnPrintKani_EX();
				//m_pPrnPrintKaniEx->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, &m_PrintCmInfo);
				m_pPrnPrintKaniEx->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, &m_PrintCmInfo, &m_PRtableEx, &m_PGtable, &m_PRtableEx1);
				m_pPrnPrintKaniEx->m_swBeForked = m_swBeFork;
				//m_pPrnPrintKaniEx->SetPrintInfo();	// 24/07/10_個別タブから出力 del
				m_pPrnPrintKaniEx->SetPrintInfo(0x00);	// 24/07/10_個別タブから出力 add
				delete(m_pPrnPrintKaniEx);
			}
		}
	}
	else {
		if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
			CPrnPrint	*m_pPrnPrint;
			m_pPrnPrint = new CPrnPrint();
			m_pPrnPrint->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, &m_PrintCmInfo);
			m_pPrnPrint->m_swBeForked = m_swBeFork;
			m_pPrnPrint->SetPrintInfo();
			delete(m_pPrnPrint);
		}
		else {
			CPrnPrintKani	*m_pPrnPrintKani;
			m_pPrnPrintKani = new CPrnPrintKani();
			m_pPrnPrintKani->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, &m_PrintCmInfo);
			m_pPrnPrintKani->m_swBeForked = m_swBeFork;
			m_pPrnPrintKani->SetPrintInfo();
			delete(m_pPrnPrintKani);
		}
	}
	if( m_PrintCmInfo.PgMx == 0 ) {
// 240308_h_160357 cor -->
		//ICSMessageBox( _T("出力帳票が選択されていません。"), MB_OK, 0, 0, this  );
// -----------------------
		if(m_Tokutei != 0)	{
			ICSMessageBox( _T("出力対象データがありません。"), MB_OK, 0, 0, this  );
		}
		else	{
			ICSMessageBox( _T("出力帳票が選択されていません。"), MB_OK, 0, 0, this  );
		}
// 240308_h_160357 cor <--
		return 1;
	}
	return 0;
}

void CDBSyzShinMainView::OnNMReportSelchangedPastReport(NMHDR * pNotifyStruct, LRESULT * result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;
	//以下の項目が有効
	//変更後の行
	ICSReportRow*	pRow	=	pItemNotify->pRow;
	int	indexRow	=	pRow->GetIndex();
	//クリックされた位置
	POINT pt	=	pItemNotify->pt;

	// 履歴選択時は税額情報の引継ぎを行わない
	m_Drophistorysw = TRUE;
	SetCurHistoryCheck();
	m_Drophistorysw = FALSE;
}

void CDBSyzShinMainView::GetElectronicReport( void )
{
	CKmnCtrl			KmnCtrl;
	int					Status = 1;
	PRCT_BPINFO			Bpinfo;
	KMNCTL_GETSKINFPAC	Gkspac;
	char buff[1024] = {0};
	memset( m_pShinInfo->pFinishPrn, '\0', sizeof( m_pShinInfo->pFinishPrn ) );
	Gkspac.flg = 0;
	memset( Gkspac.dmy, '\0', sizeof(Gkspac.dmy) );
	memset( Gkspac.kanryo.dmy, '\0', sizeof(Gkspac.kanryo.dmy) );

	if( GetShincyokuSignEX() != -1 ){
		Bpinfo.svname = GET_DBNAME();
		if( KmnCtrl.BeginProc( &Bpinfo ) != ERR ){
			char	Bymd[4] = {0};
			char	Aymd[20] = {0};
			int				gengo = 0;
			unsigned char	year = 0x00;
			unsigned char	dmme = 0x00;
			int		date = 0;
			Gkspac.kanryo.nen = -1;																// 年分及び年月分 (yyyymm)
			Gkspac.kanryo.from = -1;															// 事業年度　自 (yyyymmdd)
			Gkspac.kanryo.to = -1;																// 事業年度　至 (yyyymmdd)
			if( !(m_pSnHeadData->Sn_SKKBN%2) ){
				char	yymmdd[4]={0};
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo );
				memmove( Bymd, yymmdd, 3 );	
			}
			else{
				char	yymmdd[4]={0};
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
				memmove( Bymd, yymmdd, 3 );	
			}
			// 和暦→西暦変換
//			m_pMainFrame->shin_datecnv( Bymd[0], &year, 1 );
//			Bymd[0] = year;	// 和暦→西暦
			int		tmpGengo=0;
			UCHAR	tmpYear[4]={0};
			_db_datecnvGen( gengo, (UCHAR*)Bymd, &tmpGengo, tmpYear, 1, 0 );
			Bymd[0] = tmpYear[0];

			m_Arith.l_unpac( Aymd, Bymd, 6 );
			date = atoi( Aymd );
			Gkspac.kanryo.kzfrom	= ( date + 20000000 );										// 課税期間　自 (yyyymmdd)
			if( !(m_pSnHeadData->Sn_SKKBN%2) ){
				char	yymmdd[4]={0};
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
				memmove( Bymd, yymmdd, 3 );	
			}
			else{
				char	yymmdd[4]={0};
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo);
				memmove( Bymd, yymmdd, 3 );	
			}
			// 和暦→西暦変換
//			m_pMainFrame->shin_datecnv( Bymd[0], &year, 1 );
//			Bymd[0] = year;	// 和暦→西暦
			tmpGengo=0;
			memset( tmpYear, '\0', sizeof(tmpYear) );
			_db_datecnvGen(gengo, (UCHAR*)Bymd, &tmpGengo, tmpYear, 1, 0);
			Bymd[0] = tmpYear[0];

			m_Arith.l_unpac( Aymd, Bymd, 6 );
			date = atoi( Aymd );
			Gkspac.kanryo.kzto	= ( date + 20000000 );											// 課税期間　至 (yyyymmdd)
			Gkspac.kanryo.gkbn =  _T( "GK000240" );												// 業務区分
			Gkspac.kanryo.mskbn.Empty();														// 業務明細区分ID
			switch( m_pSnHeadData->Sn_SKKBN ){
				case 1 :	
					if( m_pSnHeadData->Sn_Syuusei & 0x02 ){
						if( m_pSnHeadData->Sn_Syuusei & 0x04 ){
							Gkspac.kanryo.skkbn = _T( "SK999102" );
						}// 更正の申出
						else{
							Gkspac.kanryo.skkbn = _T( "SK999100" );				// 更正の請求
						}
					}
					else{
						Gkspac.kanryo.skkbn = _T( "SK000100" );						// 確定申告	
					}
					break;
				case 2 : 
					Gkspac.kanryo.skkbn = _T( "SK000020" );
					break;						// 中間申告
				case 3 : 
					Gkspac.kanryo.skkbn = _T( "SK000101" );	
					break;						// 修正確定	
				case 4 : 
					Gkspac.kanryo.skkbn = _T( "SK000021" );	
					break;						// 修正中間	
				default: 
					Gkspac.kanryo.skkbn = _T( "SK999999" );	
					break;						// その他
			}
			Gkspac.kanryo.esign = 1;															// 電子申告サイン	1:電子申告 -1:以外
			Gkspac.kanryo.zckd_code.Empty();													// 全国地方公共団体コード
			// 会社名
			if( m_pSnHeadData->IsSoleProprietor() ){
				Gkspac.kanryo.kmn_name = m_pSnHeadData->Sn_DIHYO;	// 個人
			}
			else{
				Gkspac.kanryo.kmn_name = m_pSnHeadData->Sn_CONAM;	// 法人
			}
// 24/04/01_246 cor -->
			//if(  KmnCtrl.GetShinkokuKanryoInfo( &Gkspac ) == 1 )	{
// --------------------
//Gkspac.kanryo.kzfrom = -1;
//Gkspac.kanryo.kzto = -1;
//Gkspac.kanryo.gkbn.Empty();
//Gkspac.kanryo.skkbn.Empty();
//Gkspac.kanryo.esign = -1;
//Gkspac.kanryo.zeijim_code.Empty();
//Gkspac.kanryo.employee_id = 0;
//Gkspac.kanryo.scount = 0;
			m_GetSki = KmnCtrl.GetShinkokuKanryoInfo(&Gkspac);
			if(m_GetSki == 1)	{
// 24/04/01_246 cor <--
				sprintf_s( m_pShinInfo->pFinishPrn, 512 , "[電子申告(受付)日時:%02d/%02d/%02d %02d:%02d:%02d 受付番号:%s]",
					Gkspac.kanryo.date.wYear, Gkspac.kanryo.date.wMonth, Gkspac.kanryo.date.wDay,
					Gkspac.kanryo.date.wHour, Gkspac.kanryo.date.wMinute, Gkspac.kanryo.date.wSecond,
					Gkspac.kanryo.number.GetBuffer() );
			}
// 24/04/01_246 add -->
			m_Kanryo = Gkspac.kanryo;
// 24/04/01_246 add <--
			// 後始末
			 KmnCtrl.EndProc();
		}
	}
}

//-----------------------------------------------------------------------------
// 第２画面作成時の比較情報を保存
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SaveTabMakeInfo()
{
	m_SvSVmzsw = curset.SVmzsw;
	m_SvSn_Sign4 = curset.Sn_Sign4;
//--> '15.08.21 INS START
	m_SvDispTabSgn = curset.m_DispTabSgn;
//<-- '15.08.21 INS END
}


//'14.10.08
LRESULT CDBSyzShinMainView::OnUserSyzInit( WPARAM wParam, LPARAM lParam ) 
{

	CBaseHyoView *pBaseView = (CBaseHyoView *)((CDBSyzShinMainApp*)AfxGetApp())->GetHonHyoView();
	if( pBaseView != NULL ){
		pBaseView->InputRock(1);
//		pH26View->InitTab(TRUE);
	}

	return 0;
}

//'14.10.08
//課税取引金額計算表から申告書へ転記
LRESULT CDBSyzShinMainView::OnUserTenkiData( WPARAM wParam, LPARAM lParam ) 
{
	//----->('23.09.14 Tanaka Add)
	long smd, emd;
	GetKazeiKikan(smd, emd);
	//　23.10.10課税取引金額計算表の制御を12/31→10/01切り替えに
	//if (emd >= TAX_R05_EDAY) {
	if (emd >= ICS_SH_INVOICE_DAY) {
		TenkiInvData();
	}
	else {
		TenkiData();
	}
	//<-----

//------->('23.09.14 Tanaka Del)課税期間を見て転記処理部分を関数化(TenkiData / TenkiInvData)
//	//転記データ取得
//	//++++++++++++++++++データ転記+++++++++++++++++++++++++//未実装
//	if (GetActViewVer() == ID_VER_SYZ31) {
//		CTaxationList31View	*pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 4 ));
//		if( pTaxlist31View != NULL ){
//		//----->('23.09.14 Add)
//			long smd, emd;
//			GetKazeiKikan( smd, emd );
//		//<-----
//			SH_TENKIDATA31				tenkidata;
//			::ZeroMemory(&tenkidata, sizeof(tenkidata));
//			pTaxlist31View->GetTenkiData(&tenkidata);
//
//			// 31 転記処理
//			CH31HyoView	*pH31View = (CH31HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(1));
//			if( pH31View != NULL && pTaxlist31View != NULL ){
//				char	mony[MONY_BUF_SIZE] = { 0 };
//				CString	tag, chtag;
//
//				long smd, emd;
//				GetKazeiKikan( smd, emd );
//
//				if( emd >= ICS_SH_INVOICE_DAY && m_pSnHeadData->m_DispTabSgn & 0x2000 ){
//					
//					//----> 20230616
//					if( m_pSnHeadData->SVmzsw ){
//
//						// 2割特例・経過措置ありの場合は付表4・5に転記する
//						if (pH31View) {
//							pH31View->MoveFocusInTaxTenki();
//
//							// '20.01.29 付表5-1、付表5-2の手入力サインを落とす。手入力サインが残っていると消費税額が転記できないため
//							pH31View->ClearManualInputSign_Fhyo5();
//						}
//
//						// 課税資産の譲渡等の対価の額
//						tag = _T("CSC00020"); // 4-1
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
//						tag = _T("CQC00030"); // 1-1
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
//						tag = _T("CSC00030");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);
//						tag = _T("CQC00040");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);
//
//						// 消費税額
//						tag = _T("CSD00020");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
//						tag = _T("CQD00020");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
//						tag = _T("CSD00030");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);
//						tag = _T("CQD00030");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);
//
//						// 返還等対価に係る税額
//						tag = _T("CSF00080");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
//						tag = _T("CQF00080");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
//						tag = _T("CSF00090");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);
//						tag = _T("CQF00090");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);
//
//						// 付表5-1　第二種
//						tag = _T("CTD00160");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2C);
//						tag = _T("CTD00170");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2D);
//
//						tag = _T("ABI00170");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURITA);
//
//						// 事業区分別の合計額
//						tag = _T("CTD00540");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZTT);
//
//						// 付表5-1　第二種
//						tag = _T("CTD00640");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT2);
//
//						// 旧税率分
//						// 課税資産の譲渡等の対価の額
//						tag = _T("BBL00030");	// 4-2
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
//						tag = _T("BAM00040");	// 1-2
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
//
//						// 消費税額
//						tag = _T("BBC00030");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);
//						tag = _T("BAC00030");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);
//
//						// 返還等対価に係る税額
//						tag = _T("BBE00090");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
//						tag = _T("BAE00090");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
//
//						// 付表5-2　第二種
//						tag = _T("BCD00160");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2B);
//					}
//					//<---- 20230616
//					else{
//						// 2割特例・経過措置なしの場合は付表6に転記する
//						// 課税資産の譲渡等の対価の額
//						tag = _T("AYB00020");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
//
//						tag = _T("AYB00030");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);
//
//						// 消費税額
//						tag = _T("AYB00100");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
//					
//						tag = _T("AYB00110");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);
//
//						// 返還等対価に係る税額
//						tag = _T("AYB00180");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
//
//						tag = _T("AYB00190");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);
//					}
//				}
//
//				//if (m_Util.IsUnConnectMst(m_pZmSub->zvol) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI)) {// 非連動ｄｂで簡易課税
//				if ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) {	// 簡易課税
//				
//					if (pH31View) {
//						pH31View->MoveFocusInTaxTenki();	// これ作成必要
//
//						// '20.01.29 付表5-1、付表5-2の手入力サインを落とす。手入力サインが残っていると消費税額が転記できないため
//						pH31View->ClearManualInputSign_Fhyo5();
//					}
//
//					//前の流れ引き継いで、付表１のところにセットがされてる。非連動だと４にも転記する必要があるので見直し必要と思われる。yoshida190728
//					//簡易課税は付表４と付表１にセット
//					//Sn_1F1C
//					//yoshida190730
//					tag = _T("CSC00020"); 
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
//					tag = _T("CQC00030");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
//
//					//Sn_1F1D
//					//yoshida190730
//					tag = _T("CSC00030");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);
//					tag = _T("CQC00040");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);
//
//					//Sn_1F2C
//					//yoshida190730
//					tag = _T("CSD00020");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
//					tag = _T("CQD00020");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
//
//					//Sn_1F2D
//					//yoshida190730
//					tag = _T("CSD00030");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);
//					tag = _T("CQD00030");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);
//
//					//Sn_1F5C
//					//yoshida190730
//					tag = _T("CSF00080");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
//					tag = _T("CQF00080");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
//
//					//Sn_1F5D
//					//yoshida190730
//					tag = _T("CSF00090");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);
//					tag = _T("CQF00090");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);
//
//					//Sn_5FURI1C
//					tag = _T("CTD00090");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1C);
//					//Sn_5FURI1D
//					tag = _T("CTD00100");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1D);
//
//					//Sn_5FURI2C
//					tag = _T("CTD00160");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2C);
//					//Sn_5FURI2D
//					tag = _T("CTD00170");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2D);
//
//					//Sn_5FURI3C
//					tag = _T("CTD00230");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3C);
//					//Sn_5FURI3D
//					tag = _T("CTD00240");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3D);
//
//					//Sn_5FURI4C
//					tag = _T("CTD00300");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4C);
//					//Sn_5FURI4D
//					tag = _T("CTD00310");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4D);
//
//					//Sn_5FURI5C
//					tag = _T("CTD00370");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5C);
//					//Sn_5FURI5C
//					tag = _T("CTD00380");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5D);
//
//					//Sn_5FURI6C
//					tag = _T("CTD00440");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6C);
//					tag = _T("CTD00450");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6D);
//
//					//Sn_5FURITA
//					tag = _T("ABI00170");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURITA);
//
//					//Sn_5FUZTT
//					tag = _T("CTD00540");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZTT);
//
//					//Sn_5FUZT1
//					tag = _T("CTD00590");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT1);
//
//					//Sn_5FUZT2
//					tag = _T("CTD00640");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT2);
//
//					//Sn_5FUZT3
//					tag = _T("CTD00690");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT3);
//
//					//Sn_5FUZT4
//					tag = _T("CTD00740");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT4);
//
//					//Sn_5FUZT5
//					tag = _T("CTD00790");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT5);
//
//					//Sn_5FUZT6
//					tag = _T("CTD00840");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT6);
//
//					//経過措置あり
//					if (m_pSnHeadData->SVmzsw) {
//
//						//Sn_1F1B
//						//yoshida190730
//						tag = _T("BBL00030");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
//						tag = _T("BAM00040");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
//
//						//Sn_1F2B
//						//yoshida190730
//						tag = _T("BBC00030");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);
//						tag = _T("BAC00030");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);
//
//						//Sn_1F5B
//						//yoshida190730
//						tag = _T("BBE00090");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
//						tag = _T("BAE00090");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
//
//						//Sn_5FURI1B
//						tag = _T("BCD00100");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1B);
//
//						//Sn_5FURI2B
//						tag = _T("BCD00160");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2B);
//
//						//Sn_5FURI3B
//						tag = _T("BCD00220");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3B);
//
//						//Sn_5FURI4B
//						tag = _T("BCD00280");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4B);
//
//						//Sn_5FURI5B
//						tag = _T("BCD00340");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5B);
//
//						//Sn_5FURI6B
//						tag = _T("BCD00710");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6B);
//					}
//				}
//				else {// 原則課税
//					//Sn_1F1C
//					tag = _T("CQC00030");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
//					//Sn_1F1D
//					tag = _T("CQC00040");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);
//
//					//Sn_1F2C
//					tag = _T("CQD00020");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
//					//Sn_1F2D
//					tag = _T("CQD00030");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);
//
//					//Sn_1F5C
//					tag = _T("CQF00140");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
//					//Sn_1F5D
//					tag = _T("CQF00150");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);
//
//					//Sn_2F1C
//					tag = _T("CRB00030");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_2F1C);
//					//Sn_2F1D
//					tag = _T("CRB00040");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_2F1D);
//
//					//Sn_2F8C
//					tag = _T("CRE00030");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_2F8C);
//					//Sn_2F8D
//					tag = _T("CRE00040");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_2F8D);
//
//					//Sn_2F9C
//					tag = _T("CRE00080");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_2F9C);
//					//Sn_2F9D
//					tag = _T("CRE00090");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_2F9D);
//
//// インボイス_23/07/31_higuchi add -->
//					//Sn_2F11C
//					tag = _T("CRE2");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_2F11C);
//					//Sn_2F11D
//					tag = _T("CRE3");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_2F11D);
//
//					//Sn_2F12C
//					tag = _T("CRE6");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_2F12C);
//					//Sn_2F12D
//					tag = _T("CRE7");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_2F12D);
//// インボイス_23/07/31_higuchi add <--
//
//					//経過措置あり
//					if (m_pSnHeadData->SVmzsw) {
//						//Sn_1F1B
//						tag = _T("BAM00040");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
//
//						//Sn_1F2B
//						tag = _T("BAC00030");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);
//
//						//Sn_1F5B
//						tag = _T("BAE00250");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
//
//						//Sn_2F1B
//						tag = _T("AEB00045");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_2F1B);
//
//						//Sn_2F8B
//						tag = _T("AEC00035");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_2F8B);
//
//						//Sn_2F9B
//						tag = _T("AEC00075");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_2F9B);
//					}
//				}
//
//			}
//			//+++++++++++++++++++++++++++++++++++++++++++++++++++++//
//			//本表側の再表示
//			if (pH31View) {
//				pH31View->ReInitDisp(FALSE, m_SixKindFlg, m_ShRevType);
//			}
//			if (pTaxlist31View) {
//				pTaxlist31View->ReInitDisp(FALSE, m_SixKindFlg, m_ShRevType);
//			}
//
//		}
//	}
//	else {
//		CTaxationListView	*pTaxlistView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 3 ));
//		if( pTaxlistView != NULL ){
//			SH_TENKIDATA		tenkidata;
//			::ZeroMemory(&tenkidata , sizeof(tenkidata));
//			pTaxlistView->GetTenkiData( &tenkidata );
//			CH26HyoView	*pH26View = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(2));
//
//			if (pH26View != NULL && pTaxlistView != NULL ) {
//				char	mony[MONY_BUF_SIZE] = { 0 };
//				CString	tag, chtag;
//
//				//if (m_Util.IsUnConnectMst(m_pZmSub->zvol) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI)) {// 非連動ｄｂで簡易課税
//				if ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) {	// 簡易課税
//
//					if (pH26View) {
//						pH26View->MoveFocusInTaxTenki();
//					}
//
//					//Sn_1F1C
//					tag = _T("ACB00030");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
//
//					//Sn_1F2C
//					tag = _T("ACC00030");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
//
//					//Sn_1F5C
//					tag = _T("ACE00090");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
//
//					//Sn_5FURI1C
//					tag = _T("AHG00110");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1C);
//
//					//Sn_5FURI2C
//					tag = _T("AHG00170");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2C);
//
//					//Sn_5FURI3C
//					tag = _T("AHG00230");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3C);
//
//					//Sn_5FURI4C
//					tag = _T("AHG00290");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4C);
//
//					//Sn_5FURI5C
//					tag = _T("AHG00350");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5C);
//
//					//2016.10.20 INSERT START
//									//Sn_5FURI6C
//					if (m_SixKindFlg) {
//						tag = _T("AHG00410");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6C);
//					}
//					//2016.10.20 INSERT END
//
//									//Sn_5FURITA
//					tag = _T("ABB00170");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURITA);
//
//					//Sn_5FUZTT
//					tag = _T("AHG00430");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZTT);
//
//					//Sn_5FUZT1
//					tag = _T("AHG00480");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT1);
//
//					//Sn_5FUZT2
//					tag = _T("AHG00530");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT2);
//
//					//Sn_5FUZT3
//					tag = _T("AHG00580");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT3);
//
//					//Sn_5FUZT4
//					tag = _T("AHG00630");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT4);
//
//					//Sn_5FUZT5
//					tag = _T("AHG00680");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT5);
//
//					//--> '16.12.27 INS START
//					if (m_SixKindFlg) {
//						//Sn_5FUZT6
//						tag = _T("AHG01730");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT6);
//					}
//					//<-- '16.12.27 INS END
//
//									//経過措置あり
//					if (m_pSnHeadData->SVmzsw) {
//
//						//Sn_1F1B
//						tag = _T("ACB00020");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
//
//						//Sn_1F2B
//						tag = _T("ACC00020");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);
//
//						//Sn_1F5B
//						tag = _T("ACE00080");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
//
//						//Sn_5FURI1B
//						tag = _T("AHG00100");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1B);
//
//						//Sn_5FURI2B
//						tag = _T("AHG00160");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2B);
//
//						//Sn_5FURI3B
//						tag = _T("AHG00220");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3B);
//
//						//Sn_5FURI4B
//						tag = _T("AHG00280");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4B);
//
//						//Sn_5FURI5B
//						tag = _T("AHG00340");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5B);
//
//						//2016.10.20 INSERT START
//											//Sn_5FURI6B
//						if (m_SixKindFlg) {
//							tag = _T("AHG00400");
//							SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6B);
//						}
//						//2016.10.20 INSERT END
//					}
//
//				}
//				else {// 原則課税
//
//					//Sn_1F1C
//					tag = _T("ACB00030");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
//
//					//--> '15.11.14 IN START
//					if ((m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE) && (m_pSnHeadData->IsNeedTkkzDisp())) {
//						tag = _T("ACB00080");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
//					}
//					//<-- '15.11.14 IN END
//
//									//Sn_1F2C
//					tag = _T("ACC00030");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
//
//					//Sn_1F5C
//					tag = _T("ACE00090");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
//
//					//--> '15.11.14 IN START
//					if ((m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE) && (m_pSnHeadData->IsNeedTkkzDisp())) {
//						tag = _T("ACE00240");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
//					}
//					//<-- '15.11.14 IN END
//
//
//									//Sn_2F1C
//					tag = _T("AEB00050");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_2F1C);
//
//					//Sn_2F8C
//					tag = _T("AEC00040");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_2F8C);
//
//					//Sn_2F9C
//					tag = _T("AEC00090");
//					SetMony(tag, chtag, (char*)tenkidata.Sn_2F9C);
//
//					//経過措置あり
//					if (m_pSnHeadData->SVmzsw) {
//						//Sn_1F1B
//						tag = _T("ACB00020");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
//
//						//--> '15.11.14 IN START
//						if ((m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE) && (m_pSnHeadData->IsNeedTkkzDisp())) {
//							tag = _T("ACB00070");
//							SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
//						}
//						//<-- '15.11.14 IN END
//
//											//Sn_1F2B
//						tag = _T("ACC00020");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);
//
//						//Sn_1F5B
//						tag = _T("ACE00080");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
//
//						//--> '15.11.14 IN START
//						if ((m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE) && (m_pSnHeadData->IsNeedTkkzDisp())) {
//							tag = _T("ACE00230");
//							SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
//						}
//						//<-- '15.11.14 IN END
//
//											//Sn_2F1B
//						tag = _T("AEB00040");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_2F1B);
//
//						//Sn_2F8B
//						tag = _T("AEC00030");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_2F8B);
//
//						//Sn_2F9B
//						tag = _T("AEC00080");
//						SetMony(tag, chtag, (char*)tenkidata.Sn_2F9B);
//					}
//				}
//
//			}
//			//+++++++++++++++++++++++++++++++++++++++++++++++++++++//
//			//本表側の再表示
//			if (pH26View) {
//				//-- '15.11.14 --
//				//			pH26View->ReInitDisp( FALSE , m_SixKindFlg );//150106
//				//---------------
//				pH26View->ReInitDisp(FALSE, m_SixKindFlg, m_ShRevType);
//				//---------------
//				//			pH26View->ReInitDisp( FALSE );
//			}
//		}
//	}
//<-------('23.09.14 Tanaka Del)課税期間を見て転記処理部分を関数化(TenkiData / TenkiInvData)

	return 0;

}

//-----------------------------------------------------------------------------
// 既確定税額のクリア('15.01.28)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::ClearAlreadyDecision()
{
	if( m_pMainFrame != NULL ){
		return m_pMainFrame->ClearAlreadyDecision();
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 還付申告の要・不要の切り替え ('15.02.24)
//-----------------------------------------------------------------------------
// 引数		sw	：	0:不要, 1:要
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::ChangeRefundOut( int sw )
{
	if( sw == 0 ){
		m_PrintCmInfo.OP_PSEL &= ~0x01;
	}
	else if( sw == 1 ){
		m_PrintCmInfo.OP_PSEL |= 0x01;
	}

	return 0;
}

void CDBSyzShinMainView::OnBnClickedButtonKjdel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	// アクセス権限がないユーザーは削除しない（未決定事項）
	if( MynoAccessPerm() == FALSE ){
		return;
	}

//--> '15.08.24 INS START
	// 個人番号の登録有り？
	CString	data;
	m_pMnec->GetData( data );
//<-- '15.08.24 INS END

	CICSMNSub8 mnsub;
//-- '15.08.24 --
//	int st = mnsub.NoMessageBox_Dsp( this, MEG_KIND_ONE_DEL );
//---------------
	int st;
	if( data.IsEmpty() ){
		st = mnsub.NoMessageBox_Dsp( this, MEG_KIND_ONE_DEL|MEG_KIND_NODB_KBN );
	}
	else{
		st = mnsub.NoMessageBox_Dsp( this, MEG_KIND_ONE_DEL );
	}
//---------------
	if( st == RET_NO_CANCEL ){		// キャンセル
		c_set( IDC_BUTTON_KJDEL );
		return;
	}
	else if( st == RET_NODEL_H ){	// データ削除
//		m_pZVolume->kojin_hash.RemoveAll();
		ClearKjhash( true );
//		m_mneh.SetData( *m_pZVolume );
		SetMNData();
//		m_mneh.EnableWindow( TRUE );
		if( m_pMnec ){
			m_pMnec->EnableWindow( TRUE );
		}
	}
	else if( st == RET_NODEL_DB ){	// 全体から削除
		DeleteKjhashFromNoDB();
//		m_mneh.SetDeleteKjFlg( TRUE, *m_pZVolume );
		SetMNData();
//		m_mneh.EnableWindow( TRUE );
		if( m_pMnec ){
			m_pMnec->EnableWindow( TRUE );
		}
	}

	// ボタン表示変更
	ChangeDispKjdelBtn();

	// フォーカスセット
	m_MnEdit2.SetFocus();
}

//-----------------------------------------------------------------------------
// 改正様式は？
//-----------------------------------------------------------------------------
// 返送値	：	改正様式
//-----------------------------------------------------------------------------
EnumIdIcsShRevType CDBSyzShinMainView::GetShRevTypeFromSnset( CSnset& snset )
{
	BYTE	kbn = snset.GetKubun();
	long	ksymd=0, keymd=0, msymd=0, meymd=0;
	CString	ymdStr;
	if( (kbn==kakutei) || (kbn==kakutei_syuuse) || (kbn==kousei) || kbn== junkakutei){
		ymdStr = snset.GetSetKessanKikan().first.Format("%Y%m%d");;
		ksymd = atoi( ymdStr );
		ymdStr = snset.GetSetKessanKikan().second.Format("%Y%m%d");;
		keymd = atoi( ymdStr );
		msymd = meymd = -1;
	}
	else if( (kbn==tyuukan) || (kbn==yotei) || (kbn==tyuukan_syuuse) ){
		ymdStr = snset.GetSetTyukanKikan().first.Format("%Y%m%d");;
		msymd = atoi( ymdStr );
		ymdStr = snset.GetSetTyukanKikan().second.Format("%Y%m%d");;
		meymd = atoi( ymdStr );
		ymdStr = snset.GetSetKessanKikan().first.Format("%Y%m%d");;
		ksymd = atoi( ymdStr );
		ymdStr = snset.GetSetKessanKikan().second.Format("%Y%m%d");;
		keymd = atoi( ymdStr );
	}

	return m_Util.GetShRevType( m_pZmSub->zvol, ksymd, keymd, msymd, meymd, (EnumIdIcsShShinkokuKubun)kbn );
}

#ifdef _20171226_CUT
//-----------------------------------------------------------------------------
// 改正様式は？
//-----------------------------------------------------------------------------
// 返送値	：	改正様式
//-----------------------------------------------------------------------------
EnumIdIcsShRevType CDBSyzShinMainView::GetShRevType()
{
	// チェック用年月日
	BYTE	kbn = m_pSnHeadData->GetKubun();
	long	symd=0, eymd=0;
//2016.06.22 UPDATE START
	//if( (kbn==ID_ICSSH_KAKUTEI) || (kbn==ID_ICSSH_KAKUTEI_SYUUSEI) || (kbn==ID_ICSSH_KOUSEI) ){
	//	symd = m_pSnHeadData->Sn_KDAYS;
	//	eymd = m_pSnHeadData->Sn_KDAYE;
	//}
	//else if( (kbn==ID_ICSSH_TYUUKAN) || (kbn==ID_ICSSH_YOTEI) || (kbn==ID_ICSSH_TYUUKAN_SYUUSEI) ){
	//	symd = m_pSnHeadData->Sn_MDAYS;
	//	eymd = m_pSnHeadData->Sn_MDAYE;
	//}

	symd = m_pSnHeadData->Sn_KDAYS;
	eymd = m_pSnHeadData->Sn_KDAYE;
//2016.06.22 UPDATE END

	return m_Util.GetShRevType( m_pZmSub->zvol, symd, eymd, (EnumIdIcsShShinkokuKubun)kbn );
}
#else
//-----------------------------------------------------------------------------
// 改正様式は？
//-----------------------------------------------------------------------------
// 返送値	：	改正様式
//-----------------------------------------------------------------------------
EnumIdIcsShRevType CDBSyzShinMainView::GetShRevType()
{
	// チェック用年月日
	BYTE	kbn = m_pSnHeadData->GetKubun();
	long	ksymd=0, keymd=0, msymd=0, meymd=0;
//2018.03.13 UPDATE START
//	if( (kbn==ID_ICSSH_KAKUTEI) || (kbn==ID_ICSSH_KAKUTEI_SYUUSEI) || (kbn==ID_ICSSH_KOUSEI) ){
	if( (kbn==ID_ICSSH_KAKUTEI) ||(kbn==ID_ICSSH_JUNKAKUTEI) || (kbn==ID_ICSSH_KAKUTEI_SYUUSEI) || (kbn==ID_ICSSH_KOUSEI) ){
//2018.03.13 UPDATE END
		ksymd = m_pSnHeadData->Sn_KDAYS;
		keymd = m_pSnHeadData->Sn_KDAYE;
		msymd = meymd = -1;
	}
	else if( (kbn==ID_ICSSH_TYUUKAN) || (kbn==ID_ICSSH_YOTEI) || (kbn==ID_ICSSH_TYUUKAN_SYUUSEI) ){
		ksymd = m_pSnHeadData->Sn_KDAYS;
		keymd = m_pSnHeadData->Sn_KDAYE;
		msymd = m_pSnHeadData->Sn_MDAYS;
		meymd = m_pSnHeadData->Sn_MDAYE;
	}

	return m_Util.GetShRevType( m_pZmSub->zvol, ksymd, keymd, msymd, meymd, (EnumIdIcsShShinkokuKubun)kbn );
}
#endif

//-----------------------------------------------------------------------------
// 特定課税仕入の状態をセット('15.08.27)
//-----------------------------------------------------------------------------
// 引数	eymd	：	状態を判定する為に必要になる末日
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetTkkzSign( long eymd )
{
	ASSERT(m_pZmSub);
	ASSERT(m_pZmSub->zvol);
	if( (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_GENSOKU) && (eymd>=ICS_SH_TOKUTEI_REFORM_DAY) ){
		m_IsAbleTkkz = TRUE;
	}
	else{
		m_IsAbleTkkz = FALSE;
	}
}

//-----------------------------------------------------------------------------
// 特定課税仕入有りチェックの状態をセット('15.09.09)
//-----------------------------------------------------------------------------
// 引数	onoff	：	0:オフ, 1:オン
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetTkkzChk( int onoff )
{
	if( onoff == 0 ){
		m_Check6.SetCheck( BST_UNCHECKED );
		curset.m_DispTabSgn &= ~0x01;
		m_pSnHeadData->m_DispTabSgn &= ~0x01;

	}
	else if( onoff == 1 ){
		m_Check6.SetCheck( BST_CHECKED );
		curset.m_DispTabSgn |= 0x01;
		m_pSnHeadData->m_DispTabSgn |= 0x01;
	}
}


//2015.12.03 INSERT START
/////////////////////////////////////////////////////////
//処理概要：マイナンバーの履歴を書き込む(GetData()からコピー！！)
//
//引数　　：write_sgn  0…閲覧　1…出力
/////////////////////////////////////////////////////////
void CDBSyzShinMainView::WriteNoHistory(int write_sgn)
{

	// 履歴情報作成
	NOHISTTBL tmp_nhtbl;
	NTBMOD8_InitNoHistoryTbl( 0, &tmp_nhtbl );
	tmp_nhtbl = m_nhtbl;

	switch( write_sgn ){
		case 0:
			tmp_nhtbl.user_play = NTBMOD_USER_PLAY_BROWSE | NTBMOD_USER_PLAY_APPLI;
			break;
		case 1:
			tmp_nhtbl.user_play = NTBMOD_USER_PLAY_PRINT | NTBMOD_USER_PLAY_APPLI;
			break;
		default:
			return;
			break;
	}

	CNoHistoryTblArray nhtblary;
	nhtblary.Add( tmp_nhtbl );

	// 履歴テーブルにセット
	NTBMOD8_SetNoHistoryTbl( 0, &nhtblary );
}

/////////////////////////////////////////////////////////
//処理概要：個人番号を印刷・プレビューするか判定
/////////////////////////////////////////////////////////
BOOL CDBSyzShinMainView::IsPrintKojinNo()
{
//2016.02.23 UPDATE START
	//if( m_ShRevType<ID_ICSSH_MYNUMBER_REVTYPE ){ //マイナンバー対応
	//	return FALSE;
	//}

	//if( m_pSnHeadData->IsSoleProprietor() == FALSE ){ //not個人
	//	return FALSE;
	//}

	//BOOL kojin_num_sgn = FALSE; //個人番号記載する帳表を出力するか否か
	//for( int pg_cnt = 0; pg_cnt!=m_PrintCmInfo.PgMx; pg_cnt++ ){
	//	switch( m_PrintCmInfo.PRtable[pg_cnt] & 0x7f ){
	//		case 0x01: //申告書本表提出
	//		case 0x09: //更正の請求
	//		case 0x50: //26号様式
	//			kojin_num_sgn = TRUE;
	//			break;
	//		default:
	//			break;
	//	}
	//}
	//if( kojin_num_sgn == FALSE ){
	//	return FALSE;
	//}

//2018.08.21 UPDATE START
//	if( IsPrintableKojinNo() == FALSE ){
	char print_sgn = 0x00;
	if( IsPrintableKojinNo(&print_sgn) == FALSE ){
//2018.08.21 UPDATE END
		return FALSE;
	}
//2016.02.23 UPDATE END

	if(( m_Vprn.Get_KojinPrintFlg() == 0 ) || ( m_Vprn.Get_KojinPrintFlg() == 1 )){ //アスタリスクで出力or出力しない
		return FALSE;
	}

	CString dummy;
//2018.08.21 UPDATE START
	//if( m_pSnHeadData->GetMyNumber(dummy) != 0 ){ //個人番号がない
	//	return FALSE;
	//}

	CBaseHyoView *pBaseView = (CBaseHyoView *)((CDBSyzShinMainApp*)AfxGetApp())->GetHonHyoView();
	if( print_sgn == 0x01){
		if( m_pSnHeadData->GetMyNumber(dummy) != 0 ){ //個人番号がない
			return FALSE;
		}
	}else if( print_sgn == 0x02){
		if( pBaseView != NULL ){
			if( !pBaseView->IsExist_MyNumber_Fuhyo6() ){
				return FALSE;
			}
		}
	}else if( print_sgn == 0x03){
		BOOL rt = TRUE;
		if( m_pSnHeadData->GetMyNumber(dummy) != 0 ){ //個人番号がない
			rt = FALSE;
			if( pBaseView != NULL ){
				if( !pBaseView->IsExist_MyNumber_Fuhyo6() ){
					rt = FALSE;
				}else{
					rt = TRUE;
				}
			}
			return rt;
		}
	}
//2018.08.21 UPDATE END

	return TRUE;
}
//2015.12.03 INSERT END
//2016.02.23 INSERT START
/////////////////////////////////////////////////////////
//処理概要：個人番号が印刷・プレビュー可能か判定
//
//引数　　：prnt_sgn  D0:事業主の個人番号出力帳票
//　　　　　　　　　　D1:相続人の個人番号出力帳票
/////////////////////////////////////////////////////////
//2018.08.21 UPDATE START
//BOOL CDBSyzShinMainView::IsPrintableKojinNo()
int CDBSyzShinMainView::IsPrintableKojinNo(char *print_sgn)
//2018.08.21 UPDATE END
{
	if( m_pSnHeadData->IsSoleProprietor() == FALSE ){ //not個人
		return FALSE;
	}

	char prt_sgn = 0x00;
	BOOL kojin_num_sgn = FALSE; //個人番号記載する帳表を出力するか否か
	for( int pg_cnt = 0; pg_cnt!=m_PrintCmInfo.PgMx; pg_cnt++ ){
		//switch( m_PrintCmInfo.PRtable[pg_cnt] & 0x7f ){
		switch(GetPRtable(&m_PrintCmInfo,pg_cnt) & 0x7f)	{
			case 0x09: //更正の請求
//2018.03.13 INSERT START
//2018.08.21 DELETE START
//			case 0x16: //付表６
//2018.08.21 DELETE END
//2018.03.13 INSERT END
//2018.08.21 UPDATE START
//				kojin_num_sgn = TRUE;
				prt_sgn |= 0x01;
//2018.08.21 UPDATE END
				break;
			case 0x01: //申告書本表提出（カラー）
			case 0x03: //申告書本表提出（モノクロ）
			case 0x50: //26号様式
				if( m_ShRevType>=ID_ICSSH_MYNUMBER_REVTYPE ){ //マイナンバー対応
//2018.08.21 UPDATE START
//					kojin_num_sgn = TRUE;
					prt_sgn |= 0x01;
//2018.08.21 UPDATE END
				}
				break;
//2018.08.21 INSERT START
			case 0x16: //付表６
				prt_sgn |= 0x02;
				break;
//2018.08.21 INSERT END
			default:
				break;
		}
	}
//2018.08.21 UPDATE START
//	if( kojin_num_sgn == FALSE ){
	if( prt_sgn == 0x00 ){
//2018.08.21 UPDATE END
		return FALSE;
	}

//2018.08.21 INSERT START
	if( print_sgn ){
		*print_sgn = prt_sgn;
	}
//2018.08.21 INSERT END

	return TRUE;
}
//2016.02.23 INSERT END

void CDBSyzShinMainView::OnBnClickedDenshiInfo()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CDenshiInfo dlg;
	dlg.InitInfo( m_pSnHeadData );
	dlg.DoModal();
}

//2016.02.23 INSERT START
int CDBSyzShinMainView::Make_StaticFont()
{
	CWnd *item;
	item = GetDlgItem(IDC_STATIC_INPUTLOCK);

	if( item == NULL ){
		return -1;
	}

	LOGFONT log_font;
	item->GetFont()->GetLogFont(&log_font);
	log_font.lfWeight = FW_BOLD;

	m_font.DeleteObject();
	m_font.CreateFontIndirectA( &log_font );

	item->SetFont( &m_font, FALSE );

	return 0;
}

void CDBSyzShinMainView::Static_Update( unsigned char Sn_Sign4 )
{
	CWnd *item;
	item = GetDlgItem(IDC_STATIC_INPUTLOCK);

	if( item == NULL ){
		return;
	}

	if( Sn_Sign4&0x01){
		GetDlgItem(IDC_STATIC_INPUTLOCK)->ShowWindow( SW_SHOW );
	}else{
		GetDlgItem(IDC_STATIC_INPUTLOCK)->ShowWindow( SW_HIDE );
	}
}

int CDBSyzShinMainView::Message_BlankKinyu_Honpyo()
{
	char tmp_char1[128]={0},tmp_char2[128]={0},tmp_char3[128]={0},tmp_char4[128]={0};
	CString	msg;
	BOOL blank1 = FALSE,blank2 = FALSE,blank3 = FALSE,blank4 = FALSE; //空白判定

	if( m_pSnHeadData->Sn_BYsw == 0 ){ //銀行
		memmove( tmp_char1, m_pSnHeadData->Sn_BANK, sizeof( m_pSnHeadData->Sn_BANK ));		//銀行名称
		memmove( tmp_char2, m_pSnHeadData->Sn_SITEN, sizeof( m_pSnHeadData->Sn_SITEN ));	//支店名称
		memmove( tmp_char3, m_pSnHeadData->Sn_YOKIN, sizeof( m_pSnHeadData->Sn_YOKIN ));	//預金種別
		l_unpac( tmp_char4, m_pSnHeadData->Sn_KOZA, 10 );	//口座番号

		if( m_Util.MixSpaceCutLength(tmp_char1, 20) == 0 ){
			blank1 = TRUE;
		}
		if( m_Util.MixSpaceCutLength(tmp_char2, 20) == 0 ){
			if(( m_pSnHeadData->Sn_CHAIN != 0 )&&( m_pSnHeadData->Sn_CHAIN != 2 )){ //本店、本所は空白OK
				blank2 = TRUE;
			}
		}
		if( m_Util.MixSpaceCutLength(tmp_char3, 8) == 0 ){
			blank3 = TRUE;
		}
		if( m_Util.MixSpaceCutLength(tmp_char4, 10) == 0 ){
			blank4 = TRUE;
		}

		if( blank1&&blank2&&blank3&&blank4){
			msg = _T("還付申告ですが、還付を受けようとする金融機関等（本表）が登録されていません！");
			msg += _T("\n還付を受けようとする金融機関等を入力しますか？");
		}else
		if( blank1||blank2||blank3||blank4){
			msg = _T("還付を受けようとする金融機関等（本表）の登録において、以下の内容を再確認してください。\n");
			if(blank1){
				msg += _T("\n銀行名（入力されていません）");
			}
			if(blank2){
				msg += _T("\n支店名（入力されていません）");
			}
			if(blank3){
				msg += _T("\n預金種別（入力されていません）");
			}
			if(blank4){
				msg += _T("\n口座番号（入力されていません）");
			}

			msg += _T("\n\n還付を受けようとする金融機関等を入力しますか？");
		}
	
	}else{ //郵便局
		memmove( tmp_char1, m_pSnHeadData->Sn_YUBIN, sizeof( m_pSnHeadData->Sn_YUBIN ));	//郵便局名
		memmove( tmp_char2, m_pSnHeadData->Sn_YUNUM1, sizeof( m_pSnHeadData->Sn_YUNUM1 ));	//郵便貯金の記号番号１
		memmove( tmp_char3, m_pSnHeadData->Sn_YUNUM2, sizeof( m_pSnHeadData->Sn_YUNUM2 ));	//郵便貯金の記号番号２

		if( m_Util.MixSpaceCutLength(tmp_char1, 16) == 0 ){
			blank1 = TRUE;
		}
		if( m_Util.MixSpaceCutLength(tmp_char2, 5) == 0 ){
			blank2 = TRUE;
		}
		if( m_Util.MixSpaceCutLength(tmp_char3, 8) == 0 ){
			blank3 = TRUE;
		}

		if( blank1&&(blank2||blank3) ){ //「ゆうちょ銀行の貯金記号番号」と「郵便局名等」のいずれかの入力があればメッセージを出さない
			msg = _T("還付申告ですが、還付を受けようとする金融機関等（本表）が登録されていません！");
			msg += _T("\n還付を受けようとする金融機関等を入力しますか？");
		}
	}
	if( msg.IsEmpty() == FALSE ){
		//'22.12.26
		//---->22.12.27
		long eymd = 0;
		if (!(m_pSnHeadData->Sn_SKKBN % 2)) {
			eymd = m_pSnHeadData->Sn_MDAYE;
		}
		else {
			eymd = m_pSnHeadData->Sn_KDAYE;
		}
		//<------
		//---->22.12.27
		if ( ( eymd >= R04REFORM ) && ( m_pSnHeadData->IsSoleProprietor() ) && (m_pSnHeadData->Sn_GenericSgn & 0x400)) {//個人で公金受取口座に○がある場合には空欄でもメッセージ表示しない
		//---
//		if (m_pSnHeadData->IsSoleProprietor() && (m_pSnHeadData->Sn_GenericSgn & 0x400)) {//個人で公金受取口座に○がある場合には空欄でもメッセージ表示しない
		//<---
			msg.Empty();
			return 0;
		}
		else {
			return ICSMessageBox(msg, MB_YESNO, 0);
		}
	}

	return 0;
}
//2016.02.23 INSERT END

//-----------------------------------------------------------------------------
// 法人番号付き用紙での印刷？('16.07.11)
//-----------------------------------------------------------------------------
// 返送値	true	：	法人番号付き用紙
//			false	：	法人番号付き用紙でない
//-----------------------------------------------------------------------------
bool CDBSyzShinMainView::IsPrintWithMynoHonpyo()
{
	if( m_SixKindFlg && m_pSnHeadData->IsPrintWithMynoHonpyo() ){
		return true;
	}
	else{
		return false;
	}
}

//-----------------------------------------------------------------------------
// グループテキスト再描画
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::RedrawGroupText()
{
	m_BaseGroup.ShowWindow(SW_HIDE);
	m_BaseGroup.ShowWindow(SW_SHOW);

	if( IsHistoryMode() ){
		m_RInfoGroup.ShowWindow(SW_HIDE);
		m_RInfoGroup.ShowWindow(SW_SHOW);
	}
	else{
		m_InfoGroup.ShowWindow(SW_HIDE);
		m_InfoGroup.ShowWindow(SW_SHOW);
	}
}

//-----------------------------------------------------------------------------
// 予定申告の月数クリア('17.02.09)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CDBSyzShinMainView::ClearYoteiMonth()
{
	if( m_pMainFrame != NULL ){
		return m_pMainFrame->ClearYoteiMonth();
	}
	return 0;
}


//-----------------------------------------------------------------------------
// 特例計算チェック追加
//-----------------------------------------------------------------------------
// 返送値	なし
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::OnClickedCheck7()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	//チェック切替時にはチェックしないとの仕様で決定、処理をカット
	/*
	//--->基準期間の課税売上高チェック（5000万以下の中小事業者）。5000万超えてる場合は注意メッセージを表示yoshida190713
	char	Sn_ZZURI[MONY_BUF_SIZE] = { 0 };
	CString	tag, chtag, msg;
	if (m_pSnHeadData->Kaisei_VER == ID_VER_SYZ31) {
		if ((m_pSnHeadData->m_s_sgn2 & 0x0f) > 0x01) {	// 簡易課税
			tag = _T("AAM00030");
		}
		else {	// 原則課税
			tag = _T("ABI00180");
		}
	}
	else {//ありえない
		tag = _T("ABB00180");
	}
	m_pMainFrame->GetMony(tag, chtag, Sn_ZZURI);

	if (m_Arith.l_test(Sn_ZZURI)) {
		if (m_Arith.l_cmp(m_pSnHeadData->SVkan1, Sn_ZZURI) < 0) {//5000万チェック
			msg = "基準期間の課税売上高が5,000万円以上の為、特例適用対象外となります。";
			ICSMessageBox(msg, MB_OK, 0);

			m_pSnHeadData->m_DispTabSgn &= ~0x80;
			curset.m_DispTabSgn &= ~0x80;
			m_btn1.ShowWindow(SW_HIDE);
			m_Check7.SetCheck(0);
			return;
		}
	}
	//<-------
	*/



	if (curset.m_DispTabSgn & 0x80) {//チェックOFFへ
		curset.m_DispTabSgn &= ~0x80;
		m_pSnHeadData->m_DispTabSgn &= ~0x80;
		m_btn1.ShowWindow(SW_HIDE);

		//---->yoshida190926
		if ((m_pZmSub->zvol->s_sgn1 & 0xc0)) {
			m_Check1.EnableWindow(TRUE);
			m_Check2.EnableWindow(TRUE);
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
				m_Check2.EnableWindow(FALSE);
			}
		}

		if (m_Util.IsUnConnectMst(m_pZmSub->zvol) == FALSE) {//非連動ｄｂじゃない時。非連動ｄｂでは無効化されてる
			m_Check3.EnableWindow(TRUE);
		}

		// 特例のチェックが落とされた場合は2割特例を有効にする
		Set2wariChk();
	}
	else {

		//----> 20231025
		if( !(m_pSnHeadData->Sn_Sign4 & 0x80) ){
			long smd = 0, emd = 0;
			GetSelectKazeiKikan( smd, emd );
			if( IsCrossOverR050930( smd, emd ) ){
				ICSMessageBox( "税額計算の特例について、令和5年9月30日をまたぐ期間の自動集計は非対応です。\n非連動処理に切り替えてから処理を行ってください。" );
				m_Check7.SetCheck( 0 );
				return;
			}
		}
		//----> 20231025

		curset.m_DispTabSgn |= 0x80;
		m_pSnHeadData->m_DispTabSgn |= 0x80;
		m_btn1.ShowWindow(SW_SHOW);
		OnBnClickedButton1();

		if ( ( !( m_pSnHeadData->m_DispTabSgn & 0x10 ) ) && ( !( m_pSnHeadData->m_DispTabSgn & 0x20 ) ) && ( !( m_pSnHeadData->m_DispTabSgn & 0x40 ) ) ) {
			m_pSnHeadData->m_DispTabSgn &= ~0x80;
			curset.m_DispTabSgn &= ~0x80;
			m_btn1.ShowWindow ( SW_HIDE );
			m_Check7.SetCheck ( 0 );

			//---->yoshida190926
			if ((m_pZmSub->zvol->s_sgn1 & 0xc0)) {
				m_Check1.EnableWindow(TRUE);
				m_Check2.EnableWindow(TRUE);
				if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
					m_Check2.EnableWindow(FALSE);
				}
			}

			if (m_Util.IsUnConnectMst(m_pZmSub->zvol) == FALSE) {//非連動ｄｂじゃない時。非連動ｄｂでは無効化されてる
				m_Check3.EnableWindow(TRUE);
			}
			//<------

		}
		else {
			//---->yoshida190926
			if (!(m_pSnHeadData->Sn_Sign4 & 0x80)) {//連動の場合のみ制御
				
				//積上げ計算及び簡易計算のチェックボックスを無効化する。特例使用時は選択不可に
				//簡易計算OFF
				if (m_Util.IsUnConnectMst(m_pZmSub->zvol) == FALSE) {//非連動ｄｂじゃない時。非連動ｄｂでは無効化されてる
					m_Check3.EnableWindow(FALSE);

					m_SimpSw = 0;
					curset.Sn_SPECIAL &= 0xfe;
					m_pSnHeadData->Sn_SPECIAL &= 0xfe;

					m_Check3.SetCheck(m_SimpSw);
				}

				//積上げ計算OFF
				if ((m_pZmSub->zvol->s_sgn1 & 0xc0)) {

					m_Check1.EnableWindow(FALSE);
					m_Check2.EnableWindow(FALSE);

					m_K221sw = m_H301sw = 0;
					curset.Sn_TUMIAG &= 0xfc;
					m_pSnHeadData->Sn_TUMIAG &= 0xfc;
					m_Check1.SetCheck(m_K221sw);
					if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
						// 法第２０条第１項の適用
						m_Check2.SetCheck(m_H301sw);
					}
				}
				
			}
			else {
				if ((m_pZmSub->zvol->s_sgn1 & 0xc0)) {
					m_Check1.EnableWindow(TRUE);
					m_Check2.EnableWindow(TRUE);
					if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
						m_Check2.EnableWindow(FALSE);
					}
				}

				if (m_Util.IsUnConnectMst(m_pZmSub->zvol) == FALSE) {//非連動ｄｂじゃない時。非連動ｄｂでは無効化されてる
					m_Check3.EnableWindow(TRUE);
				}

			}
			//<------

			Set2wariChk();
		}
	}
}


void CDBSyzShinMainView::OnBnClickedButton1()
{


	//takemura
	CDiagException ExcepDlg(this);
	//SetKikan();

	long symd = 0, eymd = 0;
	if (curset.IsTyukanShinkoku()) {
		TimePair t1 = GetCurrentTyukanKikan();
		symd = CTimeControl::GetIntTime(t1.first);
		eymd = CTimeControl::GetIntTime(t1.second);
	}
	else if (curset.IsKakuteiShinkoku()) {
		TimePair t1 = GetCurrentKessanKikan();
		symd = CTimeControl::GetIntTime(t1.first);
		eymd = CTimeControl::GetIntTime(t1.second);
	}

	ExcepDlg.InitInfo(m_pSnHeadData, symd, eymd);

	int st = ExcepDlg.DoModal();
	if (st == IDOK) {
		// 令和2年4月1日改正様式のチェックを付けた後に特例帳票のチェックを付けると、
		// m_DispTabSgnが上書きされていた不具合を修正
		//curset.m_DispTabSgn = m_pSnHeadData->m_DispTabSgn;

		BOOL reformFlg = FALSE;
		if ( curset.m_DispTabSgn & 0x1000 ) {
			reformFlg = TRUE;
		}
		curset.m_DispTabSgn = m_pSnHeadData->m_DispTabSgn;

		if ( reformFlg ) {
			curset.m_DispTabSgn |= 0x1000;
		}
//		m_Tokurei_USign = ExcepDlg.USign;
//		m_Tokurei_SSign = ExcepDlg.SSign;
	}

}

//-----------------------------------------------
//　課税期間の切替
//-----------------------------------------------
void CDBSyzShinMainView::OnCbnSelchangeKessanKikanCmb()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	SetTumiageChk();

	SetKaniKeisanChk();

	SetTkreiChk();

	SetR02Chk();

	// ビューのバージョンを切り替え
	SetActViewVer(&curset);

	if ( InLockInput() ) {
		NyuryokuLock();
	}
}


void CDBSyzShinMainView::OnBnClickedCheck9()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CString msg;
	if ( curset.m_DispTabSgn & 0x1000 ) {
		msg.Format ( _T( "令和2年4月1日改正前様式へ切り替えます。\r\nよろしいですか？" ) );
		if ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_GENSOKU ) {
			long eymd = 0;
			if ( curset.IsTyukanShinkoku() ) {
				TimePair t1 = GetCurrentTyukanKikan();
				eymd = CTimeControl::GetIntTime ( t1.second );
			}
			else if ( curset.IsKakuteiShinkoku() ) {
				TimePair t1 = GetCurrentKessanKikan();
				eymd = CTimeControl::GetIntTime ( t1.second );
			}

			if ( eymd >= ICS_SH_KYOJU_INPUT_DAY ) { 
				msg += _T( "\r\n\r\n※付表２　「居住用賃貸建物を課税賃貸用に供した（譲渡した）場合の加算額」" );
				msg += _T( "\r\nに入力されていた金額はクリアされます。" );
			}
		}
	}
	else {
		// 令和3年1月4日受付開始分から電子申告が可能なため、電子申告が行えないメッセージを表示しない
		//msg.Format ( _T( "令和2年4月1日改正様式へ切り替えます。\r\nよろしいですか？\
		//				  \r\n\r\n※e-taxが改正様式に未対応のため、電子申告を行えません。" ) );
		msg.Format ( _T( "令和2年4月1日改正様式へ切り替えます。\r\nよろしいですか？" ) );
	}

	if ( ICSMessageBox( msg, ( MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ), 0, 0, this ) == IDYES ) {
		if ( curset.m_DispTabSgn & 0x1000 ) {
			curset.m_DispTabSgn &= ~0x1000;
		}
		else {
			curset.m_DispTabSgn |= 0x1000;
		}
	}
	else {
		if ( curset.m_DispTabSgn & 0x1000 ) {
			m_Check9.SetCheck ( 1 );
		}
		else {
			m_Check9.SetCheck ( 0 );
		}
	}
}

//-----------------------------------------------------------------------------
// カラーの帳票が選択されているかを返す【部数指定】
//-----------------------------------------------------------------------------
// 引　数	SH_PRINT_CMINFO*：	印刷情報
//			CSnHeadData*	：	消費税申告書クラス
// 返送値	BOOL			：	TRUE 選択されている
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::IsColorSelect( SH_PRINT_CMINFO* pPrInfo, CSnHeadData* pSnHead )
{
	BOOL	brv=FALSE;
	if(pPrInfo->isColorPrn && (pPrInfo->PRtype & 0x80))	{
		if(GetActViewVer() == ID_VER_SYZ31)	{
			//if( (pSnHead->Sn_PrintOutSgn2 & 0x02) || (pSnHead->Sn_PrintOutSgn2 & 0x08) || 
			//	(pSnHead->Sn_PrintOutSgn2 & 0x40) || (pSnHead->Sn_PrintOutSgn2 & 0x100) )	{
			if((pSnHead->Sn_PrintOutSgn2 & 0x01) || (pSnHead->Sn_PrintOutSgn2 & 0x02))	{
				brv=TRUE;
			}
		}
		else	{
			if((pSnHead->Sn_TYPE & 0x02) || (pSnHead->Sn_TYPE & 0x04))	{
				brv=TRUE;
			}
		}
	}
	return(brv);
}

//-----------------------------------------------------------------------------
// 印刷設定画面で"2"以上の部数が指定されているかを返す【部数指定】
//-----------------------------------------------------------------------------
// 引　数	SH_PRINT_CMINFO*：	印刷情報
// 返送値	BOOL			：	TRUE 指定されている
//-----------------------------------------------------------------------------
BOOL CDBSyzShinMainView::IsBusuSelect( SH_PRINT_CMINFO* pPrInfo )
{
	BOOL	brv=FALSE;

	if(GetActViewVer() == ID_VER_SYZ31)	{
		int	ii=0;

		for(ii=0; ii<50; ii++)	{
			if(pPrInfo->busu[ii] > 1)	{
				brv=TRUE;
				break;
			}
		}

// 修正No.168922 add -->
		// 各個別タブの部数を確認する
		// 特定収入計算表
		if(m_pMapPrnEXSub.GetCount() > 0)	{
			for(ii=1; ii<(int)m_pMapPrnEXSub.GetCount(); ii++)	{
				::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));
				m_pMapPrnEXSub.Lookup(ii, m_PrnEXSub);
				if(m_PrnEXSub.busu > 1)	{
					brv = TRUE;
					break;
				}
			}
		}
		// 課税取引金額計算表
		if(m_pMapPrnEXSub_KAZ.GetCount() > 0)	{
			for(ii=0; ii<(int)m_pMapPrnEXSub_KAZ.GetCount(); ii++)	{
				::ZeroMemory(&m_PrnEXSub_KAZ, sizeof(PRNEXSUB_INF));
				m_pMapPrnEXSub_KAZ.Lookup(ii, m_PrnEXSub_KAZ);
				if(m_PrnEXSub_KAZ.busu > 1)	{
					brv = TRUE;
					break;
				}
			}
		}
		// 税務代理権限証書
		if(m_pMapPrnEXSub_ZEI.GetCount() > 0)	{
			for(ii=0; ii<(int)m_pMapPrnEXSub_ZEI.GetCount(); ii++)	{
				::ZeroMemory(&m_PrnEXSub_ZEI, sizeof(PRNEXSUB_INF));
				m_pMapPrnEXSub_ZEI.Lookup(ii, m_PrnEXSub_ZEI);
				if(m_PrnEXSub_ZEI.busu > 1)	{
					brv = TRUE;
					break;
				}
			}
		}
// 修正No.168922 add <--

	}
	return(brv);
}

//-----------------------------------------------------------------------------
// PRtableの値を返す【部数指定】
//-----------------------------------------------------------------------------
// 引　数	SH_PRINT_CMINFO*：	印刷情報
//			int				：	インデックス番号
// 返送値	char			：	帳票番号
//-----------------------------------------------------------------------------
char CDBSyzShinMainView::GetPRtable( SH_PRINT_CMINFO* pPrInfo, int pNo )
{
	char	ret=0;

	if(GetActViewVer() == ID_VER_SYZ31)	{
		//ret = pPrInfo->PRtableEx.GetAt(pNo);
		ret = m_PRtableEx.GetAt(pNo);
	}
	else	{
		ret = pPrInfo->PRtable[pNo];
	}

	return(ret);
}

//-----------------------------------------------------------------------------
// PGtableの値を返す【部数指定】
//-----------------------------------------------------------------------------
// 引　数	int				：	インデックス番号
// 返送値	int				：	部数（何部目か）
//-----------------------------------------------------------------------------
int	CDBSyzShinMainView::GetPGtable( int pNo )
{
	int		ret=0;

	if(GetActViewVer() == ID_VER_SYZ31)	{
		ret = m_PGtable.GetAt(pNo);
	}
	else	{
		ret = 1;
	}

	return(ret);
}

//-----------------------------------------------------------------------------
// m_PRtableEx1 から帳票番号が一致するオフセットを返す【部数指定】
//-----------------------------------------------------------------------------
// 引　数	char			：	帳票番号
// 返送値	int				：	オフセット
//-----------------------------------------------------------------------------
int	CDBSyzShinMainView::GetPrOfs( char pNo )
{
// 24/03/19_税務代理書面 cor -->
	//int	cur=0,max=0;

	//max=m_PRtableEx1.GetCount();
	//for(cur=0; cur<max; cur++)	{
	//	if(m_PRtableEx1.GetAt(cur) == pNo)	{
	//		break;
	//	}
	//}
	//return(cur);
// -----------------------------
	int		cur=0,cur2=0,max=0;
	char	no=0;
	max=m_PRtableEx1.GetCount();
	for(cur=0,cur2=0; cur<max; cur++)	{
		no = m_PRtableEx1.GetAt(cur);
		if( ((no & 0x70) == 0x20) ||
			((no & 0x70) == 0x30) ||
			((no & 0x70) == 0x40) ||
			((no & 0x70) == 0x60) ||
			((no & 0xff) == 0x52) ||
			((no & 0xff) == 0x56) )	{
			// psch_inzで指定したオフセットなので、スケージュールを必要としない帳票はとばす
			continue;
		}
		if((no & 0xff) == (pNo & 0xff))	{
			break;
		}
		cur2++;
	}
	return(cur2);
// 24/03/19_税務代理書面 cor <--
}

//-----------------------------------------------------------------------------
// ２割特例計算を使用する('23.05.01 Add)
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::OnBnClickedCheck10()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	// 決算期間が令和5年10月1日をまたがない場合で、
	// 「2割特例計算を使用する」にチェックがある場合は注意喚起メッセージを出す
	if ( m_Check10.GetCheck() ) {
		if( !(m_pZmSub->zvol->ss_ymd < ICS_SH_INVOICE_DAY && m_pZmSub->zvol->ee_ymd >= ICS_SH_INVOICE_DAY) ){
			if( GetKazeiKaishibi() == 0 ) {
				CString str;
				if( m_Util.IsUnConnectMst( m_pZmSub->zvol ) ){
					str.Format("２割特例は、インボイス制度を機に免税事業者からインボイス発行事業者として課税事業者になられた方が対象です。\n"
							   "※当年から課税事業者になられた場合は、当業務を起動直後の「会計基本情報」にて必ず課税開始日を設定し、\n"
							   "税額情報にて免税期間を含めた当課税期間の課税売上高を入力してください。\n"
							   "処理を継続してもよろしいですか？");		
				}
				else{
					str.Format("２割特例は、インボイス制度を機に免税事業者からインボイス発行事業者として課税事業者になられた方が対象です。\n"
							   "※当年から課税事業者になられた場合は、「新規会社登録・修正・削除」業務にて、必ず課税開始日を設定してください。\n"
							   "設定がない場合、課税開始日より前の免税期間も含めて集計されますのでご注意ください。\n"
							   "処理を継続してもよろしいですか？");
				}
				if( ICSMessageBox( str, MB_YESNO|MB_ICONQUESTION, 0, 0, this ) == IDNO ){
					m_Check10.SetCheck( 0 );
					return;
				}
			}
		}
	}

	if (m_Check10.GetCheck()) {
		curset.m_DispTabSgn |= 0x2000;
	}
	else{
		curset.m_DispTabSgn &= ~0x2000;
	}

	Set2wariChk();

	SetTkreiChk();

	m_pMainFrame->ReSyzSyukei();
}

//-----------------------------------------------------------------------------
// 2割特例のチェックの状態をセット
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::Set2wariChk()
{
	// 決算期間が令和5年10月1日を超えない場合は非表示とする
	if( m_pZmSub->zvol->ee_ymd < ICS_SH_INVOICE_DAY ){
		m_Check10.ShowWindow( SW_HIDE );
		GetDlgItem( IDC_STATIC_C01 )->ShowWindow( SW_HIDE );
		return;
	}
	else{
		m_Check10.ShowWindow( SW_SHOW );
		GetDlgItem( IDC_STATIC_C01 )->ShowWindow( SW_HIDE ); // 「履歴選択」→「現在申告に戻る」を押した時にフォントが一時的に戻るため、一旦非表示にしてから表示する
		GetDlgItem( IDC_STATIC_C01 )->ShowWindow( SW_SHOW );		
	}


	//最新の課税期間を取得
	TimePair time = GetCurrentKessanKikan();
	CString smd = time.first.Format("%Y%m%d");
	CString emd = time.second.Format("%Y%m%d");
	long symd = atoi(smd);
	long eymd = atoi(emd);

	BOOL DispCheck = TRUE;		// チェックの有効・無効
	int  DispGuide = SW_HIDE;	// チェック下の説明文の表示・非表示

	if( eymd >= ICS_SH_INVOICE_DAY && symd <= INVOICE_2WARI_EDAY ){
		// 期間内の場合でも、以下は2割特例不可
		if( IsKikanTansyuku() ){
			if ( !InLockInput() ) {	//入力ロック中は２割特例のサインを落とさない様に修正
				curset.m_DispTabSgn &= ~0x2000;
				DispCheck = FALSE;
			}
		}
		if( curset.IsTyukanShinkoku() ){
			if ( !InLockInput() ) {
				curset.m_DispTabSgn &= ~0x2000;
				DispCheck = FALSE;
			}
		}
		if( m_pSnHeadData->m_DispTabSgn & 0x80 ){
			curset.m_DispTabSgn &= ~0x2000;
			DispCheck = FALSE;
		}
	}
	else{
		// 期間外の場合は2割特例不可
		curset.m_DispTabSgn &= ~0x2000;
		DispCheck = FALSE;
	}

	// 令和5年10月1日をまたぐ期間の場合は、課税開始日をチェックし、登録がなければ2割特例不可とする。
	// チェック下の説明文も表示させる。
	if( m_pZmSub->zvol->ss_ymd < ICS_SH_INVOICE_DAY && m_pZmSub->zvol->ee_ymd >= ICS_SH_INVOICE_DAY ){
		if( GetKazeiKaishibi() == 0 ){
			curset.m_DispTabSgn &= ~0x2000;
			DispCheck = FALSE;
			DispGuide = SW_SHOW;
		}
	}

	
	// チェックの状態をセット
	m_Check10.EnableWindow( DispCheck );
	GetDlgItem( IDC_STATIC_C01 )->ShowWindow( DispGuide );

	if (curset.m_DispTabSgn & 0x2000) {
		m_Check10.SetCheck( 1 );
	}
	else {
		m_Check10.SetCheck( 0 );
	}


	// 2割特例を適用する場合は簡易と同じ扱いになるため、特定収入タブを非表示にする
	if( curset.m_DispTabSgn & 0x2000 ){
		if( m_Util.IsSpcMaster(m_pZmSub) && m_pMainFrame->m_ShinInfo.pSgSpc ){
			m_pMainFrame->ShowTabOrgIndex( 4, FALSE, FALSE );
		}
	}
	else{
		if( m_Util.IsSpcMaster(m_pZmSub) && m_pMainFrame->m_ShinInfo.pSgSpc ){
			m_pMainFrame->ShowTabOrgIndex( 4, TRUE, FALSE );
		}		
	}
}

//-----------------------------------------------------------------------------
// 簡易計算のチェックの状態をセット
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetKaniKeisanChk()
{
	if (m_Util.IsUnConnectMst(m_pZmSub->zvol)) {
		// 非連動
		m_pSnHeadData->Sn_SPECIAL &= 0xfe;
		m_SimpSw = 0;
		m_Check3.EnableWindow(FALSE);
	}
	else {
		// 連動
		if (m_K221sw || m_H301sw || m_Util.IsUnConnectMst(m_pZmSub->zvol)) {
			m_SimpSw = 0;	// 積み上げ方式選択時は処理不可！
		}
		else {
			if (m_pSnHeadData->Sn_SPECIAL & 0x01) {
				m_SimpSw = 1;
			}
			else {
				m_SimpSw = 0;
			}
		}
		if( m_pSnHeadData->m_DispTabSgn & 0x80 ){
			m_Check3.EnableWindow( FALSE );
		}
		else{
			m_Check3.EnableWindow( !IsHistoryMode() );
		}
		m_Check3.SetCheck(m_SimpSw);
	}
}

//-----------------------------------------------------------------------------
// 積上げ計算を行うのチェックの状態をセット
//-----------------------------------------------------------------------------
void CDBSyzShinMainView::SetTumiageChk()
{
	if (!(m_pZmSub->zvol->s_sgn1 & 0xc0)) {
		// 込み経理処理方式
		m_K221sw = m_H301sw = 0;
		m_pSnHeadData->Sn_TUMIAG &= 0xfc;
		m_Check1.SetCheck(FALSE);
		m_Check1.EnableWindow(FALSE);
		m_Check2.SetCheck(FALSE);
		m_Check2.EnableWindow(FALSE);
	}
	else {
		// 課税期間開始日が令和5年10月1日以降の場合、積上げ計算のチェック２つを非表示にする
		TimePair time = GetCurrentKessanKikan();
		int smd = CTimeControl::GetIntTime( time.first );
		if( smd >= ICS_SH_INVOICE_DAY ){
			m_K221sw = m_H301sw = 0;
			m_pSnHeadData->Sn_TUMIAG &= 0xfc;
			m_Check1.EnableWindow( FALSE );
			m_Check2.EnableWindow( FALSE );
			m_Check1.SetCheck( 0 );
			m_Check2.SetCheck( 0 );
		}
		else{
			m_K221sw = m_H301sw = 0;
			if (m_pSnHeadData->Sn_TUMIAG & 0x01) {
				m_K221sw = 1;	// 規則２２条１項の適用（積み上げ方式）
			}
			if ((m_pSnHeadData->Sn_TUMIAG & 0x02) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU)) {
				m_H301sw = 1;	// 法第３０条第１項の適用（積み上げ方式）
			}

			//yoshida190926
			if (!(m_pSnHeadData->Sn_Sign4 & 0x80)) {//連動の場合のみ制御
				if ((m_pSnHeadData->m_DispTabSgn & 0x80)) {
					m_Check1.EnableWindow(FALSE);
					m_Check2.EnableWindow(FALSE);
				}
				else {
					m_Check1.EnableWindow(TRUE);
					m_Check2.EnableWindow(TRUE);
					if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
						m_Check2.EnableWindow(FALSE);
					}
				}
			}
			else {
				m_Check1.EnableWindow(TRUE);
				m_Check2.EnableWindow(TRUE);
				if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {
					m_Check2.EnableWindow(FALSE);
				}
			}
		}

		// 売上積み上げ
		if (m_K221sw) {
			m_Check1.SetCheck(1);
		}
		else {
			m_Check1.SetCheck(0);
		}

		// 仕入積み上げ
		if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
			if (m_H301sw) {
				m_Check2.SetCheck(1);
			}
			else {
				m_Check2.SetCheck(0);
			}
		}
		else {
			m_H301sw = 0;
			m_Check2.SetCheck(FALSE);
			m_Check2.EnableWindow(FALSE);
		}
	}
}

//=====================================================================
// 課税期間の開始日を取得
//---------------------------------------------------------------------
// 返送値
//		0			:			未登録
//		0以上		:			課税期間の開始日（西暦）
//=====================================================================
long CDBSyzShinMainView::GetKazeiKaishibi()
{
	long kazeiKaishibi = m_pZmSub->zvol->to_kz_ymd;
	if( kazeiKaishibi <= 0 ) {
		kazeiKaishibi = 0;
	}

	return kazeiKaishibi;
}

//=====================================================================
// インボイスの期間マスター？
//---------------------------------------------------------------------
// 返送値
//		true		:			インボイスマスター
//		false		:			否インボイスマスター
//=====================================================================
bool CDBSyzShinMainView::IsInvoiceMaster()
{
	bool bRet = false;

	CZmGen8 zmGen;
	if( zmGen.IsAbleToUseInvoice( m_pZmSub )) {
		bRet = true;
	}

	return bRet;
}

//=====================================================================
// 課税期間における課税売上高の入力が可能か？
//---------------------------------------------------------------------
// 返送値
//		true		:			入力可
//		false		:			入力不可
//=====================================================================
bool CDBSyzShinMainView::IsInputKazeiUriage()
{
	// 8%が起動中は入力不可
	if( GetActViewVer() != ID_VER_SYZ31 ) {
		return false;
	}

	// 連動の場合は入力不要
	if( !( m_pSnHeadData->Sn_Sign4 & 0x80 )) {
		return false;
	}

	// 課税期間の末日が、令和5年10月1日を超えていない場合は入力不可
	long symd = 0, eymd = 0;
	GetSelectKazeiKikan( symd, eymd );
	if( eymd < ICS_SH_INVOICE_DAY ) {
		return false;
	}

	// 原則で課税開始日がなく、2割特例にチェックがない場合は入力不可
	long kazeiKaishibi = 0;
	kazeiKaishibi = GetKazeiKaishibi();
	if( (kazeiKaishibi == 0) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) && !(curset.m_DispTabSgn & 0x2000) ){
		return false;
	}

	// 課税事業者になった日が課税期間の開始日よりも前
// 240501_168733 cor -->
	//if( (kazeiKaishibi != 0) && (kazeiKaishibi <= symd) ) {
// ---------------------
	if( (kazeiKaishibi != 0) && (kazeiKaishibi < symd) ) {
// 240501_168733 cor <--
		return false;
	}

	return true;
}

//=====================================================================
// 課税期間における課税売上高の入力が必要？
//---------------------------------------------------------------------
// 返送値
//		true		:			必要
//		false		:			不要
//=====================================================================
bool CDBSyzShinMainView::IsNeedInputKazeiUriage()
{
	// 連動の場合は入力不要
	if( !( m_pSnHeadData->Sn_Sign4 & 0x80 )) {
		return false;
	}

	// 課税開始日が未設定
	long kazeiKaishibi = 0;
	kazeiKaishibi = GetKazeiKaishibi();
	if( kazeiKaishibi == 0 ) {

		// 原則課税で2割特例（旧税率あり）の場合は入力可（連動元の付表2がないため）
		if( (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ) && (curset.m_DispTabSgn & 0x2000) && (m_pSnHeadData->SVmzsw) ){
			return true;
		}
		// 簡易課税で2割特例の場合は入力可（簡易本表「この課税期間の課税売上高」が入力不可のため）
		else if( (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI ) && (curset.m_DispTabSgn & 0x2000) ){
			return true;
		}
		else{
			return false;
		}
	}

	return true;
}

//=====================================================================
// 課税期間における課税売上高の保存コード取得
//---------------------------------------------------------------------
// 返送値
//		FFF00000		:			原則課税のコード
//		ABI00170		:			簡易課税のコード
//=====================================================================
CString CDBSyzShinMainView::GetKazeiUriageCode()
{
	CString saveCode;

	// 免税マスターは起動できないため、考慮しない

	if( m_Util.GetKazeihoushiki( m_pZmSub->zvol ) == ID_ICSSH_GENSOKU ) {
		saveCode.Format( _T( "FFF00000" ));
	}
	else {
		saveCode.Format( _T( "ABI00170" ));
	}

	return saveCode;
}

//=====================================================================
// 課税期間における課税売上高を取得
//---------------------------------------------------------------------
// 引数
//		pArith			:			取得した課税売上高
//=====================================================================
void CDBSyzShinMainView::GetKazeiUriagedaka( char *pArith )
{
	ASSERT( pArith );
	if( pArith == nullptr ) {
		return;
	}

	memset( pArith, '\0', MONY_BUF_SIZE );
	if( GetActViewVer() != ID_VER_SYZ31 ) {
		return;
	}

	if( ( InLockInput() ) || ( IsInputKazeiUriage() )) {
		// 入力ロック中もしくは入力可になっている
		CString	tag, chtag;
		tag = GetKazeiUriageCode();
		GetMony( tag, chtag, pArith );
	}
	else {
		long symd = 0, eymd = 0;
		char money[MONY_BUF_SIZE] = { 0 };

		// 免税期間集計
		GetSelectKazeiKikan( symd, eymd );
		CH31HyoView *pHyoView = ( CH31HyoView* )GetHonHyoView( ID_VER_SYZ31 );
		pHyoView->MenzeiKikanSyukei( symd, eymd, money );
		m_Arith.l_add( pArith, pArith, money );

		// 課税期間集計
		memset( money, '\0', MONY_BUF_SIZE );
		//-->2024.02.26 change
		//CH31HyoView::KazeiUriageSyukei( money )は内部でHeadDataのサインを参照して2割特例を適用しているかの判定を行い、集計元のコードを切り替えている
		//2割特例のチェックを変更した場合、HeadDataはタブ遷移するまで更新されないのでcursetからの判定結果を引数に取る関数の呼び出しに変更
		//pHyoView->KazeiUriageSyukei( money );
		pHyoView->KazeiUriageSyukei(money, curset.m_DispTabSgn & 0x2000);
		//<--2024.02.26 change
		m_Arith.l_add( pArith, pArith, money );
	}
}

//=====================================================================
// 選択中の課税期間の取得
//---------------------------------------------------------------------
// 引数
//		sYmd		:			自　課税期間
//		eYmd		:			至　課税期間
//=====================================================================
void CDBSyzShinMainView::GetSelectKazeiKikan( long &sYmd, long &eYmd )
{
	long s_yymmdd = 0, e_yymmdd = 0;
	TimePair timePair;

	// 初期化
	sYmd = 0;
	eYmd = 0;

	if( curset.IsTyukanShinkoku() ) {
		// 中間申告
		timePair = GetCurrentTyukanKikan();
	}
	else {
		// 確定申告
		timePair = GetCurrentKessanKikan();
	}

	s_yymmdd = CTimeControl::GetIntTime( timePair.first );	// 自　課税期間
	e_yymmdd = CTimeControl::GetIntTime( timePair.second );	// 至　課税期間

	sYmd = s_yymmdd;
	eYmd = e_yymmdd;
}

//=====================================================================
// 課税売上高５億円超えチェック処理まとめ
//=====================================================================
void CDBSyzShinMainView::CheckOver500Million()
{
	*m_pShinInfo->pOver500MillionSw = 0;
	m_pSnHeadData->Sn_Sign4 &= 0xef;
//	strcpy_s( RatioAsc, sizeof( RatioAsc ), "9500" );
	m_pSnHeadData->m_UriRatioBorder = 9500;

	//２割特例使用時は５億円チェック不要
	//if ( !(m_pSnHeadData->m_DispTabSgn & 0x2000) ) {

		// 課税売上高５億円チェック 関数呼び出し用
		if (m_pSnHeadData->Sn_Sign4 & 0x80) {
			if (CheckUnconnectTaxationSales()) {
				*m_pShinInfo->pOver500MillionSw = 1;
			}
		}
		else {
			if (CheckConnectTaxationSales()) {
				*m_pShinInfo->pOver500MillionSw = 1;
			}
		}
	//}

	if( *m_pShinInfo->pOver500MillionSw ){
//		memset( RatioAsc, '\0', sizeof( RatioAsc ) );
//		strcpy_s( RatioAsc, sizeof( RatioAsc ), "11000" );
		m_pSnHeadData->m_UriRatioBorder = 11000;
		m_pSnHeadData->Sn_Sign4 |= 0x10;
	}


	// 履歴レポート移動
	CRect	rcRep, rcStatic;
	m_PastReport.GetWindowRect( rcRep );
	this->ScreenToClient( rcRep );
	GetDlgItem(IDC_STATIC_HELP)->GetWindowRect( rcStatic );
	this->ScreenToClient( rcStatic );

//	CString	hoge;
//	hoge.Format( _T("%d"), *(m_pShinInfo->pOver500MillionSw) );
//	AfxMessageBox( hoge );

/*- '14.02.28 -*/
//	if( *(m_pShinInfo->pOver500MillionSw) != 1 ){ 
//		if(m_pShinInfo->isShinRevice != 1){
//			rcRep.top = rcStatic.top;
//			m_rcHrep = rcRep;
//			m_PastReport.MoveWindow( rcRep, TRUE );
//			m_PastReport.Populate();
//		}
//		else{
//			rcRep.top = rcStatic.bottom;
//			m_rcHrep = rcRep;
//			m_PastReport.MoveWindow( rcRep, TRUE );
//			m_PastReport.Populate();
//
//		}
//
////		m_PastReport.ShowWindow( SW_HIDE );
////		m_PastReport.ShowWindow( SW_SHOW );
//
////		if( GetMultiWindowType() != MULTIWINDOW_NONE ){	// マルチでない時に縮小するとおかしくなるのを回避するため ('10.09.22)
////			SetResize( IDC_PAST_REPORT );
////		}
//	}
//	else{
//		rcRep.top = rcStatic.bottom;
//		m_rcHrep = rcRep;
//		m_PastReport.MoveWindow( rcRep, TRUE );
//		m_PastReport.Populate();
//	}
/*-------------*/

	CString str;
	GetDlgItem(IDC_STATIC_HELP)->GetWindowTextA( str );
	str.TrimRight();

	if( str.GetLength() == 0 ){
		rcRep.top = rcStatic.top;
		m_rcHrep = rcRep;
		m_PastReport.MoveWindow( rcRep, TRUE );
		m_PastReport.Populate();
	}
	else{
		rcRep.top = rcStatic.bottom;
		m_rcHrep = rcRep;
		m_PastReport.MoveWindow( rcRep, TRUE );
		m_PastReport.Populate();
	}
/*-------------*/

}

//=====================================================================
// 課税期間末日が令和5年10月1日以前の転記処理
//=====================================================================
void CDBSyzShinMainView::TenkiData()
{
	SH_TENKIDATA31				tenkidata;
	::ZeroMemory(&tenkidata, sizeof(tenkidata));

	//転記データ取得
	//++++++++++++++++++データ転記+++++++++++++++++++++++++//未実装
	if (GetActViewVer() == ID_VER_SYZ31) {
		CTaxationList31View	*pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));
		if (pTaxlist31View != NULL) {
	//----->('23.09.14 Del)
		//		SH_TENKIDATA31				tenkidata;
		//		::ZeroMemory(&tenkidata, sizeof(tenkidata));
		//		pTaxlist31View->GetTenkiData(&tenkidata);
	//<-----
			pTaxlist31View->GetTenkiData(&tenkidata);
				// 31 転記処理
			CH31HyoView	*pH31View = (CH31HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(1));
			if (pH31View != NULL && pTaxlist31View != NULL) {
				char	mony[MONY_BUF_SIZE] = { 0 };
				CString	tag, chtag;

				long smd, emd;
				GetKazeiKikan(smd, emd);

				if (emd >= ICS_SH_INVOICE_DAY && m_pSnHeadData->m_DispTabSgn & 0x2000) {

					//----> 20230616
					if (m_pSnHeadData->SVmzsw) {

						// 2割特例・経過措置ありの場合は付表4・5に転記する
						if (pH31View) {
							pH31View->MoveFocusInTaxTenki();

							// '20.01.29 付表5-1、付表5-2の手入力サインを落とす。手入力サインが残っていると消費税額が転記できないため
							pH31View->ClearManualInputSign_Fhyo5();
						}

						// 課税資産の譲渡等の対価の額
						tag = _T("CSC00020"); // 4-1
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
						tag = _T("CQC00030"); // 1-1
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
						tag = _T("CSC00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);
						tag = _T("CQC00040");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);

						// 消費税額
						tag = _T("CSD00020");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
						tag = _T("CQD00020");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
						tag = _T("CSD00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);
						tag = _T("CQD00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);

						// 返還等対価に係る税額
						tag = _T("CSF00080");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
						tag = _T("CQF00080");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
						tag = _T("CSF00090");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);
						tag = _T("CQF00090");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);

						// 付表5-1　第二種
						tag = _T("CTD00160");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2C);
						tag = _T("CTD00170");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2D);

						tag = _T("ABI00170");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURITA);

						// 事業区分別の合計額
						tag = _T("CTD00540");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZTT);

						// 付表5-1　第二種
						tag = _T("CTD00640");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT2);

						// 旧税率分
						// 課税資産の譲渡等の対価の額
						tag = _T("BBL00030");	// 4-2
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
						tag = _T("BAM00040");	// 1-2
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);

						// 消費税額
						tag = _T("BBC00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);
						tag = _T("BAC00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);

						// 返還等対価に係る税額
						tag = _T("BBE00090");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
						tag = _T("BAE00090");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);

						// 付表5-2　第二種
						tag = _T("BCD00160");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2B);
					}
					//<---- 20230616
					else {
						// 2割特例・経過措置なしの場合は付表6に転記する
						// 課税資産の譲渡等の対価の額
						tag = _T("AYB00020");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);

						tag = _T("AYB00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);

						// 消費税額
						tag = _T("AYB00100");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);

						tag = _T("AYB00110");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);

						// 返還等対価に係る税額
						tag = _T("AYB00180");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);

						tag = _T("AYB00190");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);
					}
				}

				//if (m_Util.IsUnConnectMst(m_pZmSub->zvol) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI)) {// 非連動ｄｂで簡易課税
				if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {	// 簡易課税

					if (pH31View) {
						pH31View->MoveFocusInTaxTenki();	// これ作成必要

						// '20.01.29 付表5-1、付表5-2の手入力サインを落とす。手入力サインが残っていると消費税額が転記できないため
						pH31View->ClearManualInputSign_Fhyo5();
					}

					//前の流れ引き継いで、付表１のところにセットがされてる。非連動だと４にも転記する必要があるので見直し必要と思われる。yoshida190728
					//簡易課税は付表４と付表１にセット
					//Sn_1F1C
					//yoshida190730
					tag = _T("CSC00020");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
					tag = _T("CQC00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);

					//Sn_1F1D
					//yoshida190730
					tag = _T("CSC00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);
					tag = _T("CQC00040");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);

					//Sn_1F2C
					//yoshida190730
					tag = _T("CSD00020");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
					tag = _T("CQD00020");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);

					//Sn_1F2D
					//yoshida190730
					tag = _T("CSD00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);
					tag = _T("CQD00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);

					//Sn_1F5C
					//yoshida190730
					tag = _T("CSF00080");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
					tag = _T("CQF00080");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);

					//Sn_1F5D
					//yoshida190730
					tag = _T("CSF00090");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);
					tag = _T("CQF00090");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);

					//Sn_5FURI1C
					tag = _T("CTD00090");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1C);
					//Sn_5FURI1D
					tag = _T("CTD00100");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1D);

					//Sn_5FURI2C
					tag = _T("CTD00160");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2C);
					//Sn_5FURI2D
					tag = _T("CTD00170");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2D);

					//Sn_5FURI3C
					tag = _T("CTD00230");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3C);
					//Sn_5FURI3D
					tag = _T("CTD00240");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3D);

					//Sn_5FURI4C
					tag = _T("CTD00300");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4C);
					//Sn_5FURI4D
					tag = _T("CTD00310");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4D);

					//Sn_5FURI5C
					tag = _T("CTD00370");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5C);
					//Sn_5FURI5C
					tag = _T("CTD00380");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5D);

					//Sn_5FURI6C
					tag = _T("CTD00440");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6C);
					tag = _T("CTD00450");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6D);

					//Sn_5FURITA
					tag = _T("ABI00170");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURITA);

					//Sn_5FUZTT
					tag = _T("CTD00540");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZTT);

					//Sn_5FUZT1
					tag = _T("CTD00590");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT1);

					//Sn_5FUZT2
					tag = _T("CTD00640");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT2);

					//Sn_5FUZT3
					tag = _T("CTD00690");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT3);

					//Sn_5FUZT4
					tag = _T("CTD00740");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT4);

					//Sn_5FUZT5
					tag = _T("CTD00790");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT5);

					//Sn_5FUZT6
					tag = _T("CTD00840");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT6);

					//経過措置あり
					if (m_pSnHeadData->SVmzsw) {

						//Sn_1F1B
						//yoshida190730
						tag = _T("BBL00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
						tag = _T("BAM00040");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);

						//Sn_1F2B
						//yoshida190730
						tag = _T("BBC00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);
						tag = _T("BAC00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);

						//Sn_1F5B
						//yoshida190730
						tag = _T("BBE00090");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
						tag = _T("BAE00090");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);

						//Sn_5FURI1B
						tag = _T("BCD00100");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1B);

						//Sn_5FURI2B
						tag = _T("BCD00160");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2B);

						//Sn_5FURI3B
						tag = _T("BCD00220");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3B);

						//Sn_5FURI4B
						tag = _T("BCD00280");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4B);

						//Sn_5FURI5B
						tag = _T("BCD00340");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5B);

						//Sn_5FURI6B
						tag = _T("BCD00710");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6B);
					}
				}
				else {// 原則課税
					//Sn_1F1C
					tag = _T("CQC00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
					//Sn_1F1D
					tag = _T("CQC00040");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);

					//Sn_1F2C
					tag = _T("CQD00020");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
					//Sn_1F2D
					tag = _T("CQD00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);

					//Sn_1F5C
					tag = _T("CQF00140");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
					//Sn_1F5D
					tag = _T("CQF00150");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);

					//Sn_2F1C
					tag = _T("CRB00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F1C);
					//Sn_2F1D
					tag = _T("CRB00040");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F1D);

					//Sn_2F8C
					tag = _T("CRE00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F8C);
					//Sn_2F8D
					tag = _T("CRE00040");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F8D);

					//Sn_2F9C
					tag = _T("CRE00080");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F9C);
					//Sn_2F9D
					tag = _T("CRE00090");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F9D);

//----->('23.09.14 Tanaka del)
					//// インボイス_23/07/31_higuchi add -->
					//					//Sn_2F11C
					//tag = _T("CRE2");
					//SetMony(tag, chtag, (char*)tenkidata.Sn_2F11C);
					////Sn_2F11D
					//tag = _T("CRE3");
					//SetMony(tag, chtag, (char*)tenkidata.Sn_2F11D);

					////Sn_2F12C
					//tag = _T("CRE6");
					//SetMony(tag, chtag, (char*)tenkidata.Sn_2F12C);
					////Sn_2F12D
					//tag = _T("CRE7");
					//SetMony(tag, chtag, (char*)tenkidata.Sn_2F12D);
					//// インボイス_23/07/31_higuchi add <--
//<-----
										//経過措置あり
					if (m_pSnHeadData->SVmzsw) {
						//Sn_1F1B
						tag = _T("BAM00040");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);

						//Sn_1F2B
						tag = _T("BAC00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);

						//Sn_1F5B
						tag = _T("BAE00250");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);

						//Sn_2F1B
						tag = _T("AEB00045");
						SetMony(tag, chtag, (char*)tenkidata.Sn_2F1B);

						//Sn_2F8B
						tag = _T("AEC00035");
						SetMony(tag, chtag, (char*)tenkidata.Sn_2F8B);

						//Sn_2F9B
						tag = _T("AEC00075");
						SetMony(tag, chtag, (char*)tenkidata.Sn_2F9B);
					}
				}

			}
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//本表側の再表示
			if (pH31View) {
				pH31View->ReInitDisp(FALSE, m_SixKindFlg, m_ShRevType);
			}
			if (pTaxlist31View) {
				pTaxlist31View->ReInitDisp(FALSE, m_SixKindFlg, m_ShRevType);
			}

		}
	}
	else {
		CTaxationListView	*pTaxlistView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(3));
		if (pTaxlistView != NULL) {
			SH_TENKIDATA		tenkidata;
			::ZeroMemory(&tenkidata, sizeof(tenkidata));
			pTaxlistView->GetTenkiData(&tenkidata);
			CH26HyoView	*pH26View = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(2));

			if (pH26View != NULL && pTaxlistView != NULL) {
				char	mony[MONY_BUF_SIZE] = { 0 };
				CString	tag, chtag;

				//if (m_Util.IsUnConnectMst(m_pZmSub->zvol) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI)) {// 非連動ｄｂで簡易課税
				if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {	// 簡易課税

					if (pH26View) {
						pH26View->MoveFocusInTaxTenki();
					}

					//Sn_1F1C
					tag = _T("ACB00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);

					//Sn_1F2C
					tag = _T("ACC00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);

					//Sn_1F5C
					tag = _T("ACE00090");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);

					//Sn_5FURI1C
					tag = _T("AHG00110");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1C);

					//Sn_5FURI2C
					tag = _T("AHG00170");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2C);

					//Sn_5FURI3C
					tag = _T("AHG00230");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3C);

					//Sn_5FURI4C
					tag = _T("AHG00290");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4C);

					//Sn_5FURI5C
					tag = _T("AHG00350");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5C);

					//2016.10.20 INSERT START
									//Sn_5FURI6C
					if (m_SixKindFlg) {
						tag = _T("AHG00410");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6C);
					}
					//2016.10.20 INSERT END

									//Sn_5FURITA
					tag = _T("ABB00170");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURITA);

					//Sn_5FUZTT
					tag = _T("AHG00430");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZTT);

					//Sn_5FUZT1
					tag = _T("AHG00480");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT1);

					//Sn_5FUZT2
					tag = _T("AHG00530");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT2);

					//Sn_5FUZT3
					tag = _T("AHG00580");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT3);

					//Sn_5FUZT4
					tag = _T("AHG00630");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT4);

					//Sn_5FUZT5
					tag = _T("AHG00680");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT5);

					//--> '16.12.27 INS START
					if (m_SixKindFlg) {
						//Sn_5FUZT6
						tag = _T("AHG01730");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT6);
					}
					//<-- '16.12.27 INS END

									//経過措置あり
					if (m_pSnHeadData->SVmzsw) {

						//Sn_1F1B
						tag = _T("ACB00020");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);

						//Sn_1F2B
						tag = _T("ACC00020");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);

						//Sn_1F5B
						tag = _T("ACE00080");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);

						//Sn_5FURI1B
						tag = _T("AHG00100");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1B);

						//Sn_5FURI2B
						tag = _T("AHG00160");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2B);

						//Sn_5FURI3B
						tag = _T("AHG00220");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3B);

						//Sn_5FURI4B
						tag = _T("AHG00280");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4B);

						//Sn_5FURI5B
						tag = _T("AHG00340");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5B);

						//2016.10.20 INSERT START
											//Sn_5FURI6B
						if (m_SixKindFlg) {
							tag = _T("AHG00400");
							SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6B);
						}
						//2016.10.20 INSERT END
					}

				}
				else {// 原則課税

					//Sn_1F1C
					tag = _T("ACB00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);

					//--> '15.11.14 IN START
					if ((m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE) && (m_pSnHeadData->IsNeedTkkzDisp())) {
						tag = _T("ACB00080");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
					}
					//<-- '15.11.14 IN END

									//Sn_1F2C
					tag = _T("ACC00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);

					//Sn_1F5C
					tag = _T("ACE00090");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);

					//--> '15.11.14 IN START
					if ((m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE) && (m_pSnHeadData->IsNeedTkkzDisp())) {
						tag = _T("ACE00240");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
					}
					//<-- '15.11.14 IN END


									//Sn_2F1C
					tag = _T("AEB00050");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F1C);

					//Sn_2F8C
					tag = _T("AEC00040");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F8C);

					//Sn_2F9C
					tag = _T("AEC00090");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F9C);

					//経過措置あり
					if (m_pSnHeadData->SVmzsw) {
						//Sn_1F1B
						tag = _T("ACB00020");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);

						//--> '15.11.14 IN START
						if ((m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE) && (m_pSnHeadData->IsNeedTkkzDisp())) {
							tag = _T("ACB00070");
							SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
						}
						//<-- '15.11.14 IN END

											//Sn_1F2B
						tag = _T("ACC00020");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);

						//Sn_1F5B
						tag = _T("ACE00080");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);

						//--> '15.11.14 IN START
						if ((m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE) && (m_pSnHeadData->IsNeedTkkzDisp())) {
							tag = _T("ACE00230");
							SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
						}
						//<-- '15.11.14 IN END

											//Sn_2F1B
						tag = _T("AEB00040");
						SetMony(tag, chtag, (char*)tenkidata.Sn_2F1B);

						//Sn_2F8B
						tag = _T("AEC00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_2F8B);

						//Sn_2F9B
						tag = _T("AEC00080");
						SetMony(tag, chtag, (char*)tenkidata.Sn_2F9B);
					}
				}

			}
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//本表側の再表示
			if (pH26View) {
				//-- '15.11.14 --
				//			pH26View->ReInitDisp( FALSE , m_SixKindFlg );//150106
				//---------------
				pH26View->ReInitDisp(FALSE, m_SixKindFlg, m_ShRevType);
				//---------------
				//			pH26View->ReInitDisp( FALSE );
			}
		}
	}
}

//=====================================================================
// 課税期間末日が令和5年10月1日以降の転記処理
//=====================================================================
void CDBSyzShinMainView::TenkiInvData()
{
	SH_INVOICE_TENKIDATA31		tenkidata;
	::ZeroMemory(&tenkidata, sizeof(tenkidata));

	//転記データ取得
	//++++++++++++++++++データ転記+++++++++++++++++++++++++//未実装
	if (GetActViewVer() == ID_VER_SYZ31) {
		CTaxationList31View	*pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));
		if (pTaxlist31View != NULL) {
	//----->('23.09.14 Del)
		//		SH_TENKIDATA31				tenkidata;
		//		::ZeroMemory(&tenkidata, sizeof(tenkidata));
		//		pTaxlist31View->GetTenkiData(&tenkidata);
	//<-----
			pTaxlist31View->GetTenkiData(&tenkidata);

				// 31 転記処理
			CH31HyoView	*pH31View = (CH31HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(1));
			if (pH31View != NULL && pTaxlist31View != NULL) {
				char	mony[MONY_BUF_SIZE] = { 0 };
				CString	tag, chtag;

				long smd, emd;
				GetKazeiKikan(smd, emd);

				if (emd >= ICS_SH_INVOICE_DAY && m_pSnHeadData->m_DispTabSgn & 0x2000) {

					//----> 20230616
					if (m_pSnHeadData->SVmzsw) {

						// 2割特例・経過措置ありの場合は付表4・5に転記する
						if (pH31View) {
							pH31View->MoveFocusInTaxTenki();

							//----->[非連動ｄｂ かつ 簡易課税 かつ 経過措置有り]の場合を除き、付表5-1の手入力サインのみ落とすように変更('24.03.08 cor)
							//// '20.01.29 付表5-1、付表5-2の手入力サインを落とす。手入力サインが残っていると消費税額が転記できないため
							//pH31View->ClearManualInputSign_Fhyo5();
							//------
							if (m_Util.IsUnConnectMst(m_pZmSub->zvol)) {
								pH31View->ClearManualInputSign_Fhyo5();
							}
							else {
								pH31View->ClearManualInputSign_Fhyo51();
							}
							//<-----
						}

						// 課税資産の譲渡等の対価の額
						tag = _T("CSC00020"); // 4-1
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
						tag = _T("CQC00030"); // 1-1
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
						tag = _T("CSC00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);
						tag = _T("CQC00040");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);

						// 消費税額
						tag = _T("CSD00020");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
						tag = _T("CQD00020");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
						tag = _T("CSD00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);
						tag = _T("CQD00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);

						// 返還等対価に係る税額
						tag = _T("CSF00080");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
						tag = _T("CQF00080");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
						tag = _T("CSF00090");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);
						tag = _T("CQF00090");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);

						// 付表5-1　第二種
						tag = _T("CTD00160");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2C);
						tag = _T("CTD00170");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2D);

						tag = _T("ABI00170");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURITA);

						// 事業区分別の合計額
						tag = _T("CTD00540");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZTT);

						// 付表5-1　第二種
						tag = _T("CTD00640");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT2);

						// 旧税率分
					//----->('24.03.08 cor)
						//// 課税資産の譲渡等の対価の額
						//tag = _T("BBL00030");	// 4-2
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
						//tag = _T("BAM00040");	// 1-2
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);

						//// 消費税額
						//tag = _T("BBC00030");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);
						//tag = _T("BAC00030");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);

						//// 返還等対価に係る税額
						//tag = _T("BBE00090");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
						//tag = _T("BAE00090");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);

						//// 付表5-2　第二種
						//tag = _T("BCD00160");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2B);
					//------
						if (m_Util.IsUnConnectMst(m_pZmSub->zvol)) {	//非連動ｄｂ
							// 課税資産の譲渡等の対価の額
							tag = _T("BBL00030");	// 4-2
							SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
							tag = _T("BAM00040");	// 1-2
							SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);

							// 消費税額
							tag = _T("BBC00030");
							SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);
							tag = _T("BAC00030");
							SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);

							// 返還等対価に係る税額
							tag = _T("BBE00090");
							SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
							tag = _T("BAE00090");
							SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);

							// 付表5-2　第二種
							tag = _T("BCD00160");
							SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2B);
						}
					//<-----('24.03.08 cor)
					}
					//<---- 20230616
					else {
						// 2割特例・経過措置なしの場合は付表6に転記する
						// 課税資産の譲渡等の対価の額
						tag = _T("AYB00020");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);

						tag = _T("AYB00030");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);

						// 消費税額
						tag = _T("AYB00100");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);

						tag = _T("AYB00110");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);

						// 返還等対価に係る税額
						tag = _T("AYB00180");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);

						tag = _T("AYB00190");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);
					}
				}

				//if (m_Util.IsUnConnectMst(m_pZmSub->zvol) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI)) {// 非連動ｄｂで簡易課税
				if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {	// 簡易課税

					if (pH31View) {
						pH31View->MoveFocusInTaxTenki();	// これ作成必要

					//----->[非連動ｄｂ かつ 簡易課税 かつ 経過措置有り]の場合を除き、付表5-1の手入力サインのみ落とすように変更('24.03.08 cor)
						//// '20.01.29 付表5-1、付表5-2の手入力サインを落とす。手入力サインが残っていると消費税額が転記できないため
						//pH31View->ClearManualInputSign_Fhyo5();
					//------
						if (m_Util.IsUnConnectMst(m_pZmSub->zvol) && (m_pSnHeadData->SVmzsw)) {
							pH31View->ClearManualInputSign_Fhyo5();
						}
						else {
							pH31View->ClearManualInputSign_Fhyo51();
						}						
					//<-----
					}

					//前の流れ引き継いで、付表１のところにセットがされてる。非連動だと４にも転記する必要があるので見直し必要と思われる。yoshida190728
					//簡易課税は付表４と付表１にセット
					//Sn_1F1C
					//yoshida190730
					tag = _T("CSC00020");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
					tag = _T("CQC00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);

					//Sn_1F1D
					//yoshida190730
					tag = _T("CSC00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);
					tag = _T("CQC00040");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);

					//Sn_1F2C
					//yoshida190730
					tag = _T("CSD00020");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
					tag = _T("CQD00020");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);

					//Sn_1F2D
					//yoshida190730
					tag = _T("CSD00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);
					tag = _T("CQD00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);

					//Sn_1F5C
					//yoshida190730
					tag = _T("CSF00080");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
					tag = _T("CQF00080");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);

					//Sn_1F5D
					//yoshida190730
					tag = _T("CSF00090");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);
					tag = _T("CQF00090");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);

					//Sn_5FURI1C
					tag = _T("CTD00090");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1C);
					//Sn_5FURI1D
					tag = _T("CTD00100");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1D);

					//Sn_5FURI2C
					tag = _T("CTD00160");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2C);
					//Sn_5FURI2D
					tag = _T("CTD00170");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2D);

					//Sn_5FURI3C
					tag = _T("CTD00230");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3C);
					//Sn_5FURI3D
					tag = _T("CTD00240");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3D);

					//Sn_5FURI4C
					tag = _T("CTD00300");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4C);
					//Sn_5FURI4D
					tag = _T("CTD00310");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4D);

					//Sn_5FURI5C
					tag = _T("CTD00370");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5C);
					//Sn_5FURI5C
					tag = _T("CTD00380");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5D);

					//Sn_5FURI6C
					tag = _T("CTD00440");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6C);
					tag = _T("CTD00450");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6D);

					//Sn_5FURITA
					tag = _T("ABI00170");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURITA);

					//Sn_5FUZTT
					tag = _T("CTD00540");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZTT);

					//Sn_5FUZT1
					tag = _T("CTD00590");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT1);

					//Sn_5FUZT2
					tag = _T("CTD00640");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT2);

					//Sn_5FUZT3
					tag = _T("CTD00690");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT3);

					//Sn_5FUZT4
					tag = _T("CTD00740");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT4);

					//Sn_5FUZT5
					tag = _T("CTD00790");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT5);

					//Sn_5FUZT6
					tag = _T("CTD00840");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT6);

					//経過措置あり
					if (m_pSnHeadData->SVmzsw) {
					//----->('24.03.06 cor)
						////Sn_1F1B
						////yoshida190730
						//tag = _T("BBL00030");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
						//tag = _T("BAM00040");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);

						////Sn_1F2B
						////yoshida190730
						//tag = _T("BBC00030");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);
						//tag = _T("BAC00030");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);

						////Sn_1F5B
						////yoshida190730
						//tag = _T("BBE00090");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
						//tag = _T("BAE00090");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);

						////Sn_5FURI1B
						//tag = _T("BCD00100");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1B);

						////Sn_5FURI2B
						//tag = _T("BCD00160");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2B);

						////Sn_5FURI3B
						//tag = _T("BCD00220");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3B);

						////Sn_5FURI4B
						//tag = _T("BCD00280");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4B);

						////Sn_5FURI5B
						//tag = _T("BCD00340");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5B);

						////Sn_5FURI6B
						//tag = _T("BCD00710");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6B);
					//-------
						if (m_Util.IsUnConnectMst(m_pZmSub->zvol)) {	//非連動ｄｂ
							//Sn_1F1B
							//yoshida190730
							tag = _T("BBL00030");
							SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
							tag = _T("BAM00040");
							SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);

							//Sn_1F2B
							//yoshida190730
							tag = _T("BBC00030");
							SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);
							tag = _T("BAC00030");
							SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);

							//Sn_1F5B
							//yoshida190730
							tag = _T("BBE00090");
							SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
							tag = _T("BAE00090");
							SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);

							//Sn_5FURI1B
							tag = _T("BCD00100");
							SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1B);

							//Sn_5FURI2B
							tag = _T("BCD00160");
							SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2B);

							//Sn_5FURI3B
							tag = _T("BCD00220");
							SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3B);

							//Sn_5FURI4B
							tag = _T("BCD00280");
							SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4B);

							//Sn_5FURI5B
							tag = _T("BCD00340");
							SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5B);

							//Sn_5FURI6B
							tag = _T("BCD00710");
							SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6B);
						}
					//<------('24.03.06 cor)
					}
				}
				else {// 原則課税
					//Sn_1F1C
					tag = _T("CQC00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
					//Sn_1F1D
					tag = _T("CQC00040");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1D);

					//Sn_1F2C
					tag = _T("CQD00020");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);
					//Sn_1F2D
					tag = _T("CQD00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2D);

					//Sn_1F5C
					tag = _T("CQF00140");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
					//Sn_1F5D
					tag = _T("CQF00150");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5D);

					//Sn_2F1C
					tag = _T("CRB00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F1C);
					//Sn_2F1D
					tag = _T("CRB00040");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F1D);

					//Sn_2F8C
					tag = _T("CRE00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F8C);
					//Sn_2F8D
					tag = _T("CRE00040");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F8D);

					//Sn_2F9C
					tag = _T("CRE00080");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F9C);
					//Sn_2F9D
					tag = _T("CRE00090");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F9D);
//----->('23.09.14 Tanaka cor)タグ名を正式なものに変更
					// インボイス_23/07/31_higuchi add -->
					//Sn_2F11C
//					tag = _T("CRE2");
					tag = _T("CRE00360");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F11C);
					//Sn_2F11D
//					tag = _T("CRE3");
					tag = _T("CRE00370");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F11D);

					//Sn_2F12C
//					tag = _T("CRE6");
					tag = _T("CRE00410");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F12C);
					//Sn_2F12D
//					tag = _T("CRE7");
					tag = _T("CRE00420");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F12D);
					// インボイス_23/07/31_higuchi add <--
//<-----('23.09.14 Tanaka cor)タグ名を正式なものに変更

										//経過措置あり
					if (m_pSnHeadData->SVmzsw) {
					//------>('24.03.06 Del)
						////Sn_1F1B
						//tag = _T("BAM00040");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);

						////Sn_1F2B
						//tag = _T("BAC00030");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);

						////Sn_1F5B
						//tag = _T("BAE00250");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);

						////Sn_2F1B
						//tag = _T("AEB00045");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_2F1B);

						////Sn_2F8B
						//tag = _T("AEC00035");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_2F8B);

						////Sn_2F9B
						//tag = _T("AEC00075");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_2F9B);
					//<------('24.03.06 Del)
					}
				}

			}
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//本表側の再表示
			if (pH31View) {
				pH31View->ReInitDisp(FALSE, m_SixKindFlg, m_ShRevType);
			}
			if (pTaxlist31View) {
				pTaxlist31View->ReInitDisp(FALSE, m_SixKindFlg, m_ShRevType);
			}

		}
	}
	else {
		CTaxationListView	*pTaxlistView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(3));
		if (pTaxlistView != NULL) {
			SH_TENKIDATA		tenkidata;
			::ZeroMemory(&tenkidata, sizeof(tenkidata));
			pTaxlistView->GetTenkiData(&tenkidata);
			CH26HyoView	*pH26View = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(2));

			if (pH26View != NULL && pTaxlistView != NULL) {
				char	mony[MONY_BUF_SIZE] = { 0 };
				CString	tag, chtag;

				//if (m_Util.IsUnConnectMst(m_pZmSub->zvol) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI)) {// 非連動ｄｂで簡易課税
				if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {	// 簡易課税

					if (pH26View) {
						pH26View->MoveFocusInTaxTenki();
					}

					//Sn_1F1C
					tag = _T("ACB00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);

					//Sn_1F2C
					tag = _T("ACC00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);

					//Sn_1F5C
					tag = _T("ACE00090");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);

					//Sn_5FURI1C
					tag = _T("AHG00110");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1C);

					//Sn_5FURI2C
					tag = _T("AHG00170");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2C);

					//Sn_5FURI3C
					tag = _T("AHG00230");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3C);

					//Sn_5FURI4C
					tag = _T("AHG00290");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4C);

					//Sn_5FURI5C
					tag = _T("AHG00350");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5C);

					//2016.10.20 INSERT START
									//Sn_5FURI6C
					if (m_SixKindFlg) {
						tag = _T("AHG00410");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6C);
					}
					//2016.10.20 INSERT END

									//Sn_5FURITA
					tag = _T("ABB00170");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FURITA);

					//Sn_5FUZTT
					tag = _T("AHG00430");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZTT);

					//Sn_5FUZT1
					tag = _T("AHG00480");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT1);

					//Sn_5FUZT2
					tag = _T("AHG00530");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT2);

					//Sn_5FUZT3
					tag = _T("AHG00580");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT3);

					//Sn_5FUZT4
					tag = _T("AHG00630");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT4);

					//Sn_5FUZT5
					tag = _T("AHG00680");
					SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT5);

					//--> '16.12.27 INS START
					if (m_SixKindFlg) {
						//Sn_5FUZT6
						tag = _T("AHG01730");
						SetMony(tag, chtag, (char*)tenkidata.Sn_5FUZT6);
					}
					//<-- '16.12.27 INS END

									//経過措置あり
					if (m_pSnHeadData->SVmzsw) {
						//----->('24.03.06 Del)
						////Sn_1F1B
						//tag = _T("ACB00020");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);

						////Sn_1F2B
						//tag = _T("ACC00020");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);

						////Sn_1F5B
						//tag = _T("ACE00080");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);

						////Sn_5FURI1B
						//tag = _T("AHG00100");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI1B);

						////Sn_5FURI2B
						//tag = _T("AHG00160");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI2B);

						////Sn_5FURI3B
						//tag = _T("AHG00220");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI3B);

						////Sn_5FURI4B
						//tag = _T("AHG00280");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI4B);

						////Sn_5FURI5B
						//tag = _T("AHG00340");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI5B);

						////2016.10.20 INSERT START
						//					//Sn_5FURI6B
						//if (m_SixKindFlg) {
						//	tag = _T("AHG00400");
						//	SetMony(tag, chtag, (char*)tenkidata.Sn_5FURI6B);
						//}
						////2016.10.20 INSERT END
						//<-----('24.03.06 Del)
					}

				}
				else {// 原則課税

					//Sn_1F1C
					tag = _T("ACB00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);

					//--> '15.11.14 IN START
					if ((m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE) && (m_pSnHeadData->IsNeedTkkzDisp())) {
						tag = _T("ACB00080");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F1C);
					}
					//<-- '15.11.14 IN END

									//Sn_1F2C
					tag = _T("ACC00030");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F2C);

					//Sn_1F5C
					tag = _T("ACE00090");
					SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);

					//--> '15.11.14 IN START
					if ((m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE) && (m_pSnHeadData->IsNeedTkkzDisp())) {
						tag = _T("ACE00240");
						SetMony(tag, chtag, (char*)tenkidata.Sn_1F5C);
					}
					//<-- '15.11.14 IN END


									//Sn_2F1C
					tag = _T("AEB00050");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F1C);

					//Sn_2F8C
					tag = _T("AEC00040");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F8C);

					//Sn_2F9C
					tag = _T("AEC00090");
					SetMony(tag, chtag, (char*)tenkidata.Sn_2F9C);

					//経過措置あり
					if (m_pSnHeadData->SVmzsw) {
						//----->('24.03.06 Del)
						////Sn_1F1B
						//tag = _T("ACB00020");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);

						////--> '15.11.14 IN START
						//if ((m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE) && (m_pSnHeadData->IsNeedTkkzDisp())) {
						//	tag = _T("ACB00070");
						//	SetMony(tag, chtag, (char*)tenkidata.Sn_1F1B);
						//}
						////<-- '15.11.14 IN END

						//					//Sn_1F2B
						//tag = _T("ACC00020");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F2B);

						////Sn_1F5B
						//tag = _T("ACE00080");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);

						////--> '15.11.14 IN START
						//if ((m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE) && (m_pSnHeadData->IsNeedTkkzDisp())) {
						//	tag = _T("ACE00230");
						//	SetMony(tag, chtag, (char*)tenkidata.Sn_1F5B);
						//}
						////<-- '15.11.14 IN END

						//					//Sn_2F1B
						//tag = _T("AEB00040");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_2F1B);

						////Sn_2F8B
						//tag = _T("AEC00030");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_2F8B);

						////Sn_2F9B
						//tag = _T("AEC00080");
						//SetMony(tag, chtag, (char*)tenkidata.Sn_2F9B);
						//<-----
					}
				}

			}
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++//
			//本表側の再表示
			if (pH26View) {
				//-- '15.11.14 --
				//			pH26View->ReInitDisp( FALSE , m_SixKindFlg );//150106
				//---------------
				pH26View->ReInitDisp(FALSE, m_SixKindFlg, m_ShRevType);
				//---------------
				//			pH26View->ReInitDisp( FALSE );
			}
		}
	}
}

//=====================================================================
// 令和5年9月30日をまたぐ期間か  20231025
//---------------------------------------------------------------------
// 返送値
//		true		:		またぐ期間
//		false		:		またがない期間
//=====================================================================
bool CDBSyzShinMainView::IsCrossOverR050930( long smd, long emd )
{
	bool rt = false;

	if( smd < URIAGETOKUREI_EDAY && emd > URIAGETOKUREI_EDAY ){
		rt = true;
	}

	return rt;
}
