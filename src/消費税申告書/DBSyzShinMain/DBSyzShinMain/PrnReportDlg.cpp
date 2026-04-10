// PrnReportDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"
#include "PrnReportDlg.h"

#include "PrnPrint.h"
#include "PrnItem.h"
//240305 takemura
#include "PrintTokuteiSub.h"
#include "PrintZeitempSub.h"
#include "PrintKazeiSub.h"

#include "PrnTune.h"
#include "PrnPrintKani.h"	// '15.02.04

#include "PrnPrint_EX.h"
#include "PrnPrintKani_EX.h"
//----->('23.05.01 Add)
#include "PrnPrint_EX2.h"
#include "PrnPrintKani_EX2.h"
//<-----

// CPrnReportDlg ダイアログ

IMPLEMENT_DYNAMIC(CPrnReportDlg, ICSDialog)

CPrnReportDlg::CPrnReportDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CPrnReportDlg::IDD, pParent)
	, m_pPrnPrint(NULL)
	, m_pPrnItem(NULL)
	//20240306 takemura
	, m_pPrnKazS(NULL)
	, m_pPrnTokS(NULL)
	, m_pPrnZeiS(NULL)
	, m_pPrnTune(NULL)
	, m_pSnHeadData(NULL)
	, m_pZmSub(NULL)
	, m_pShinInfo(NULL)
	, m_IsPreview(FALSE)
	, m_swBeForked(0)
	, m_pPrnPrintKani(NULL)	// '15.02.04
	, m_pPrnPrint_EX(NULL)
	, m_pPrnPrintKani_EX(NULL)
//----->('23.05.01 Add)
	, m_pPrnPrint_EX2(NULL)
	, m_pPrnPrintKani_EX2(NULL)
//<-----
	, m_Kazei_DButton_sw(0)
	, m_Toku_DButton_sw(0)
	, m_Zeiri_DButton_sw(0)
{
}

CPrnReportDlg::~CPrnReportDlg()
{
	if( m_pPrnPrint ){
		delete m_pPrnPrint;
		m_pPrnPrint = NULL;
	}
	if( m_pPrnItem ){
		delete m_pPrnItem;
		m_pPrnItem = NULL;
	}
	//20240306 takemura 
	if (m_pPrnKazS) {
		delete m_pPrnKazS;
		m_pPrnKazS = NULL;
	}

	if (m_pPrnTokS) {
		delete m_pPrnTokS;
		m_pPrnTokS = NULL;
	}

	if (m_pPrnZeiS) {
		delete m_pPrnZeiS;
		m_pPrnZeiS = NULL;
	}
	if( m_pPrnTune ){
		delete m_pPrnTune;
		m_pPrnTune = NULL;
	}
//--> '15.02.04 INS START
	if( m_pPrnPrintKani ){
		delete m_pPrnPrintKani;
		m_pPrnPrintKani = NULL;
	}
//<-- '15.02.04 INS END
	if( m_pPrnPrint_EX ){
		delete m_pPrnPrint_EX;
		m_pPrnPrint_EX = NULL;
	}
	if( m_pPrnPrintKani_EX ){
		delete m_pPrnPrintKani_EX;
		m_pPrnPrintKani_EX = NULL;
	}
//----->('23.05.01 Add)
	if (m_pPrnPrint_EX2) {
		delete m_pPrnPrint_EX2;
		m_pPrnPrint_EX2 = NULL;
	}
	if (m_pPrnPrintKani_EX2) {
		delete m_pPrnPrintKani_EX2;
		m_pPrnPrintKani_EX2 = NULL;
	}
//<-----
}

void CPrnReportDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_PRINT, m_PrnTab);
}


BEGIN_MESSAGE_MAP(CPrnReportDlg, ICSDialog)
	ON_BN_CLICKED(IDOK, &CPrnReportDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPrnReportDlg::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CUSTOM_PRINT, OnTcnSelchangeCustomPrint)
END_MESSAGE_MAP()


// CPrnReportDlg メッセージ ハンドラ

BOOL CPrnReportDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// タブ初期化
	InitTab();

	if( m_IsPreview ){
		GetDlgItem(IDOK)->SetWindowText("プレビュー[F5]");
	}
	if( m_swBeForked ){
		GetDlgItem(IDOK)->SetWindowText("終了[End]");
		GetDlgItem(IDCANCEL)->EnableWindow( FALSE );
		m_pSnHeadData->Sn_TYPE |= 0x01;
	}

	// 依頼No.160523
	// IMEをOFFにする
	HIMC	hImeHandle = ImmGetContext(this->m_hWnd);
	ImmSetOpenStatus(hImeHandle,FALSE);
	ImmReleaseContext(this->m_hWnd,hImeHandle);

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期情報セット
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	消費税ヘッダ情報
//		pZmSub		：	公益クラス
//		pShinInfo		：	消費税申告書の基本情報
//		pPrintCmInfo	：	印刷情報
//		pMainFrame		：	親ウィンドウ
//		pPRtable		：	部数を考慮した出力帳票
//		pPGtable		：	ページ番号
//		pBStable		：	部数
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CPrnReportDlg::InitInfo( CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, SH_PRINT_CMINFO *pPrintCmInfo, 
	CMainFrame *pMainFrame, CArray<char>* pPRtable, CArray<int>* pPGtable, CArray<int>* pPRtable1 )
{
	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pZmSub );
	if( pZmSub == NULL ){
		return -1;
	}
	m_pZmSub = pZmSub;

	ASSERT( pShinInfo );
	if( pShinInfo == NULL ){
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT( pPrintCmInfo );
	if( pPrintCmInfo == NULL ){
		return -1;
	}
	m_pPrintCmInfo = pPrintCmInfo;

	if(pMainFrame == NULL)	{
		return -1;
	}
	m_pMainFrame = pMainFrame;

	if(pPRtable == NULL)	{
		return -1;
	}
	m_pPRtableEx = pPRtable;

	if(pPGtable == NULL)	{
		return -1;
	}
	m_pPGtable = pPGtable;

	if(pPRtable1 == NULL)	{
		return -1;
	}
	m_pPRtableEx1 = pPRtable1;

	return 0;
}

//-----------------------------------------------------------------------------
// タブ初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CPrnReportDlg::InitTab()
{
	int		tabIdx = 0;
	int		tabSetflg = 0;//takemura
	TC_ITEM	TabControlItem;
	TabControlItem.mask = TCIF_TEXT;

// 240308_h_160357 add -->
	m_Tokutei = 0;
// 240308_h_160357 add <--

//----->('23.05.01 Add)
	long eymd = 0;
	if (!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
	else								eymd = m_pSnHeadData->Sn_KDAYE;
//<-----

	// 申告書出力設定
//----->('23.05.01 Add)
	//tab設定　240305 takemura add
	if (m_pShinInfo->ActViewVer == ID_VER_SYZ31) {
		if ((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000) ) { //２割特例計算を使用する
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
				if (!m_pPrnPrint_EX2) {
// 24/03/22_背景色 cor -->
					//m_pPrnPrint_EX2 = new CPrnPrint_EX2();
// -----------------------
					m_pPrnPrint_EX2 = new CPrnPrint_EX2(this);
// 24/03/22_背景色 cor <--
					m_pPrnPrint_EX2->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, m_pPrintCmInfo, m_pPRtableEx, m_pPGtable, m_pPRtableEx1);
					m_pPrnPrint_EX2->m_swBeForked = m_swBeForked;
					m_pPrnPrint_EX2->Create(IDD_DIALOG_PRN_PRINT_EX2, &m_PrnTab);
				}
				TabControlItem.pszText = _T("申告書出力設定");
				m_PrnTab.InsertItem(tabIdx, TabControlItem.pszText, m_pPrnPrint_EX2->m_hWnd);
				tabIdx++;
			}
			else {
				if (!m_pPrnPrintKani_EX2) {
// 24/03/22_背景色 cor -->
					//m_pPrnPrintKani_EX2 = new CPrnPrintKani_EX2();
// -----------------------
					m_pPrnPrintKani_EX2 = new CPrnPrintKani_EX2(this);
// 24/03/22_背景色 cor <--
					m_pPrnPrintKani_EX2->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, m_pPrintCmInfo, m_pPRtableEx, m_pPGtable, m_pPRtableEx1);
					m_pPrnPrintKani_EX2->m_swBeForked = m_swBeForked;
					m_pPrnPrintKani_EX2->Create(IDD_DIALOG_PRN_PRINT_KANI_EX2, &m_PrnTab);
				}
				TabControlItem.pszText = _T("申告書出力設定");
				m_PrnTab.InsertItem(tabIdx, TabControlItem.pszText, m_pPrnPrintKani_EX2->m_hWnd);
				tabIdx++;
			}
		}
//<-----
		else {
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
				if (!m_pPrnPrint_EX) {
// 24/03/22_背景色 cor -->
					//m_pPrnPrint_EX = new CPrnPrint_EX();
// -----------------------
					m_pPrnPrint_EX = new CPrnPrint_EX(this);
// 24/03/22_背景色 cor <--
// 240308_h_160357 cor -->
					//m_pPrnPrint_EX->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, m_pPrintCmInfo, m_pPRtableEx, m_pPGtable, m_pPRtableEx1);
// -----------------------
					m_pPrnPrint_EX->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, m_pPrintCmInfo, m_pPRtableEx, m_pPGtable, m_pPRtableEx1, &m_Tokutei);
// 240308_h_160357 cor <--
					m_pPrnPrint_EX->m_swBeForked = m_swBeForked;
					m_pPrnPrint_EX->Create(IDD_DIALOG_PRN_PRINT_EX, &m_PrnTab);
				}
				TabControlItem.pszText = _T("申告書出力設定");
				m_PrnTab.InsertItem(tabIdx, TabControlItem.pszText, m_pPrnPrint_EX->m_hWnd);
				tabIdx++;
			}
			else {
				if (!m_pPrnPrintKani_EX) {
// 24/03/22_背景色 cor -->
					//m_pPrnPrintKani_EX = new CPrnPrintKani_EX();
// -----------------------
					m_pPrnPrintKani_EX = new CPrnPrintKani_EX(this);
// 24/03/22_背景色 cor <--
					m_pPrnPrintKani_EX->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, m_pPrintCmInfo, m_pPRtableEx, m_pPGtable, m_pPRtableEx1);
					m_pPrnPrintKani_EX->m_swBeForked = m_swBeForked;
					m_pPrnPrintKani_EX->Create(IDD_DIALOG_PRN_PRINT_KANI_EX, &m_PrnTab);
				}
				TabControlItem.pszText = _T("申告書出力設定");
				m_PrnTab.InsertItem(tabIdx, TabControlItem.pszText, m_pPrnPrintKani_EX->m_hWnd);
				tabIdx++;
			}
		}
	}else{
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
			if( !m_pPrnPrint ){
				m_pPrnPrint = new CPrnPrint();
				m_pPrnPrint->InitInfo( m_pSnHeadData, m_pZmSub, m_pShinInfo, m_pPrintCmInfo );
				m_pPrnPrint->m_swBeForked = m_swBeForked;
				m_pPrnPrint->Create( IDD_DIALOG_PRN_PRINT, &m_PrnTab );
			}
			TabControlItem.pszText = _T("申告書出力設定");
			m_PrnTab.InsertItem( tabIdx, TabControlItem.pszText, m_pPrnPrint->m_hWnd );
			tabIdx++;
		}
		else{
			if( !m_pPrnPrintKani ){
				m_pPrnPrintKani = new CPrnPrintKani();
				m_pPrnPrintKani->InitInfo( m_pSnHeadData, m_pZmSub, m_pShinInfo, m_pPrintCmInfo );
				m_pPrnPrintKani->m_swBeForked = m_swBeForked;
				m_pPrnPrintKani->Create( IDD_DIALOG_PRN_PRINT_KANI, &m_PrnTab );
			}
			TabControlItem.pszText = _T("申告書出力設定");
			m_PrnTab.InsertItem( tabIdx, TabControlItem.pszText, m_pPrnPrintKani->m_hWnd );
			tabIdx++;
		}
	}
	/////////////////////////////////240305 takemura タブ判定
	// 課税取引金額計算表//個人事業者
	tabSetflg = 0;

	if (m_pShinInfo->ActViewVer == ID_VER_SYZ31) {
		BOOL			syzSgn = FALSE;
		DIAGRAM_DATA	dd;

		//個人または個人事業者の場合
		//if (!(m_pSnHeadData->SVkojn & 0x01)) {
		//	tabSetflg = 0;
		//}
		if(m_pSnHeadData->IsSoleProprietor() == FALSE)	{
			tabSetflg = 0;
		}
		else tabSetflg = 1;

	
		if (tabSetflg) {
			if (!m_pPrnKazS) {
				m_pPrnKazS = new PrintKazeiSub(this);
				m_pPrnKazS->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, m_pPrintCmInfo, m_swBeForked, m_Kazei_DButton_sw);
				m_pPrnKazS->Create(IDD_DIALOG_PRN_KAZEI_SUB, &m_PrnTab);
			}
			//TabControlItem.pszText = _T("課税取引個別指定");		// 24/03/19_Awd del
			TabControlItem.pszText = _T("課税取引金額計算表");		// 24/03/19_Awd add
			m_PrnTab.InsertItem(tabIdx, TabControlItem.pszText, m_pPrnKazS->m_hWnd);
			tabIdx++;
// 24/03/22_背景色 add -->
			if(m_pPrnPrint_EX)		m_pPrnPrint_EX->BackColorControl2(0);
			if(m_pPrnPrint_EX2)		m_pPrnPrint_EX2->BackColorControl2(0);
			if(m_pPrnPrintKani_EX)	m_pPrnPrintKani_EX->BackColorControl2(0);
			if(m_pPrnPrintKani_EX2)	m_pPrnPrintKani_EX2->BackColorControl2(0);
// 24/03/22_背景色 add <--
		}


		// 特定収入　非営利法人以外は必要ない
		tabSetflg = 0;
		if ((m_pZmSub->zvol->apno & 0xf0) <= 0x30) {
			tabSetflg = 0;
		}
		//else if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {	// 簡易課税
		// 簡易課税または２割特例計算を使用する
		else if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI || 
				 (m_pSnHeadData->m_DispTabSgn & 0x2000))	{
			tabSetflg = 0;
		}
		else {
			//// 課税取引・旧税率ありなしの判定
			//if (!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
			//else								eymd = m_pSnHeadData->Sn_KDAYE;
			//if (m_pSnHeadData->SVmzsw == 1 || eymd < TAX_R04_SDAY) {
			//	syzSgn = TRUE;	// 旧税率あり
			//}
			//if (syzSgn == FALSE && eymd >= ICS_SH_INVOICE_DAY) {
			//	tabSetflg = 1;
			//}
			tabSetflg = 1;
		}

		// 特定収入計算表
		if (tabSetflg) {
			if (!m_pPrnTokS) {
				m_pPrnTokS = new PrintTokuteiSub(this);
				m_pPrnTokS->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, m_pPrintCmInfo, m_swBeForked, m_Toku_DButton_sw);
				m_pPrnTokS->Create(IDD_DIALOG_PRN_TOKUTEI_SUB, &m_PrnTab);
			}
			//TabControlItem.pszText = _T("特定収入個別指定");	// 24/03/19_Awd del
			TabControlItem.pszText = _T("特定収入計算表");		// 24/03/19_Awd add
			m_PrnTab.InsertItem(tabIdx, TabControlItem.pszText, m_pPrnTokS->m_hWnd);
			tabIdx++;
// 24/03/22_背景色 add -->
			if(m_pPrnPrint_EX)		m_pPrnPrint_EX->BackColorControl2(1);
			if(m_pPrnPrint_EX2)		m_pPrnPrint_EX2->BackColorControl2(1);
			if(m_pPrnPrintKani_EX)	m_pPrnPrintKani_EX->BackColorControl2(1);
			if(m_pPrnPrintKani_EX2)	m_pPrnPrintKani_EX2->BackColorControl2(1);
// 24/03/22_背景色 add <--
		}

		// 税務代理権限証書
// 24/03/18_税務代理書面 cor -->
		//if (!m_pPrnZeiS) {
		//	m_pPrnZeiS = new PrintZeitempSub(this);
		//	m_pPrnZeiS->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, m_pPrintCmInfo, m_swBeForked, m_Zeiri_DButton_sw);
		//	m_pPrnZeiS->Create(IDD_DIALOG_PRN_ZEITEMP_SUB, &m_PrnTab);
		//}
		//TabControlItem.pszText = _T("添付書面個別指定");
		//m_PrnTab.InsertItem(tabIdx, TabControlItem.pszText, m_pPrnZeiS->m_hWnd);
		//tabIdx++;
// -----------------------------
		if(eymd >= R06_ZEIMUDAIRI)	{
			if (!m_pPrnZeiS) {
				m_pPrnZeiS = new PrintZeitempSub(this);
				m_pPrnZeiS->InitInfo(m_pSnHeadData, m_pZmSub, m_pShinInfo, m_pPrintCmInfo, m_swBeForked, m_Zeiri_DButton_sw);
				m_pPrnZeiS->Create(IDD_DIALOG_PRN_ZEITEMP_SUB, &m_PrnTab);
			}
			//TabControlItem.pszText = _T("添付書面個別指定");		// 24/03/19_Awd del
			TabControlItem.pszText = _T("税務代理書面");			// 24/03/19_Awd add
			m_PrnTab.InsertItem(tabIdx, TabControlItem.pszText, m_pPrnZeiS->m_hWnd);
			tabIdx++;
// 24/03/22_背景色 add -->
			if(m_pPrnPrint_EX)		m_pPrnPrint_EX->BackColorControl2(2);
			if(m_pPrnPrint_EX2)		m_pPrnPrint_EX2->BackColorControl2(2);
			if(m_pPrnPrintKani_EX)	m_pPrnPrintKani_EX->BackColorControl2(2);
			if(m_pPrnPrintKani_EX2)	m_pPrnPrintKani_EX2->BackColorControl2(2);
// 24/03/22_背景色 add <--
		}
// 24/03/18_税務代理書面 cor <--
	}

	//////////////////////////////////////////////////////////////

	// 印刷項目設定
	if( !m_pPrnItem ){
		m_pPrnItem = new CPrnItem();
		m_pPrnItem->InitInfo( m_pSnHeadData, m_pZmSub, m_pShinInfo, m_pPrintCmInfo, m_swBeForked );
		m_pPrnItem->Create( IDD_DIALOG_PRN_ITEM, &m_PrnTab );
	}
	TabControlItem.pszText = _T("印刷項目設定");
	m_PrnTab.InsertItem( tabIdx, TabControlItem.pszText, m_pPrnItem->m_hWnd );
	tabIdx++;

	// 微調整
	if( !m_pPrnTune ){
		m_pPrnTune = new CPrnTune();
		m_pPrnTune->InitInfo( m_pSnHeadData, m_pZmSub, m_pShinInfo, m_pPrintCmInfo, m_swBeForked );
		m_pPrnTune->Create( IDD_DIALOG_PRN_TUNE, &m_PrnTab );
	}
	TabControlItem.pszText = _T("印刷微調整");
	m_PrnTab.InsertItem( tabIdx, TabControlItem.pszText, m_pPrnTune->m_hWnd );
	tabIdx++;

	RECT rect = { 0 };

	if (m_pShinInfo->ActViewVer == ID_VER_SYZ31) {
	//----->('23.05.01 Add)
		if ((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000)) { //２割特例計算を使用する
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
				m_pPrnPrint_EX2->MoveWindow(&rect);
			}
			else {
				m_pPrnPrintKani_EX2->MoveWindow(&rect);
			}
		}
	//<-----
		else {
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
				m_pPrnPrint_EX->MoveWindow(&rect);
			}
			else {
				m_pPrnPrintKani_EX->MoveWindow(&rect);
			}
		}
	}else{
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
			m_pPrnPrint->MoveWindow( &rect );
		}
		else{
			m_pPrnPrintKani->MoveWindow( &rect );
		}
	}
	m_pPrnItem->MoveWindow( &rect );

	RECT rect1, rect2;
	CWnd *pWnd = (CWnd *)&m_PrnTab;
	pWnd->GetClientRect( &rect1 );								// ｸﾗｲｱﾝﾄ領域の取得
	m_PrnTab.AdjustRect( FALSE, &rect1 );					// ﾀﾌﾞｺﾝﾄﾛｰﾙの表示領域の計算
	rect2.left   = rect1.left;
	rect2.top    = rect1.top;
	rect2.bottom = rect1.bottom;
	rect2.right  = rect1.right;
//-- '15.02.03 --
//	if( m_pPrnPrint ){
//		m_pPrnPrint->MoveWindow( &rect2 );
//	}
//---------------


	if (m_pShinInfo->ActViewVer == ID_VER_SYZ31) {
	//----->('23.05.01 Add)
		if ((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000)) { //２割特例計算を使用する
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
				if (m_pPrnPrint_EX2) {
					m_pPrnPrint_EX2->MoveWindow(&rect2);
				}
			}
			else {
				if (m_pPrnPrintKani_EX2) {
					m_pPrnPrintKani_EX2->MoveWindow(&rect2);
				}
			}
		}
	//<-----
		else {
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
				if (m_pPrnPrint_EX) {
					m_pPrnPrint_EX->MoveWindow(&rect2);
				}
			}
			else {
				if (m_pPrnPrintKani_EX) {
					m_pPrnPrintKani_EX->MoveWindow(&rect2);
				}
			}
		}
	}else{
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
			if( m_pPrnPrint ){
				m_pPrnPrint->MoveWindow( &rect2 );
			}
		}
		else{
			if( m_pPrnPrintKani ){
				m_pPrnPrintKani->MoveWindow( &rect2 );
			}
		}
	}
//---------------
	if( m_pPrnItem ){
		m_pPrnItem->MoveWindow( &rect2 );
	}
	if( m_pPrnTune ){
		m_pPrnTune->MoveWindow( &rect2 );
	}

	// 初期ページ設定
	m_PrnTab.SetCurSel( 0 );

	m_PrnTab.ModifyStyle( TCS_SINGLELINE, TCS_MULTILINE );

	return 0;
}


void CPrnReportDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
//-- '15.02.03 --
//	m_pPrnPrint->SetPrintInfo();
//---------------

//----->('23.05.01 Add)
	long eymd = 0;
	if (!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
	else								eymd = m_pSnHeadData->Sn_KDAYE;
//<-----

	// 各タブの情報を取得
	if(m_pPrnKazS)	{
		m_pPrnKazS->GetInputItemData();
	}
	if(m_pPrnTokS)	{
		m_pPrnTokS->GetInputItemData();
	}
	if(m_pPrnZeiS)	{
		m_pPrnZeiS->GetInputItemData();
	}

// 24/07/10_個別タブから出力 add -->
	// 各タブで印刷ボタンを押された時の対応
	int		seltb = -1;
	int		tb = m_PrnTab.GetCurSel();

	if(m_pPrnKazS && m_pPrnZeiS)	{		// 個別
		if(tb == 1)			seltb = 0x01;
		else if(tb == 2)	seltb = 0x04;
		else				seltb = 0x00;
	}
	else if(m_pPrnTokS && m_pPrnZeiS)	{	// 非営利
		if(tb == 1)			seltb = 0x02;
		else if(tb == 2)	seltb = 0x04;
		else				seltb = 0x00;
	}
	else if(m_pPrnKazS)	{					// 個別（至年月日が2024年10月1日以前）
		if(tb == 1)			seltb = 0x01;
		else				seltb = 0x00;
	}
	else if(m_pPrnTokS)	{					// 非営利（至年月日が2024年10月1日以前）
		if(tb == 1)			seltb = 0x02;
		else				seltb = 0x00;
	}
	else if(m_pPrnZeiS)	{					// 法人/運送業/病院
		if(tb == 1)			seltb = 0x04;
		else				seltb = 0x00;
	}
	else	{
		seltb = 0x00;
	}
// 24/07/10_個別タブから出力 add <--

	if (m_pShinInfo->ActViewVer == ID_VER_SYZ31) {
	//----->('23.05.01 Add)
		if ((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000)) { //２割特例計算を使用する
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
				//m_pPrnPrint_EX2->SetPrintInfo();			// 24/07/10_個別タブから出力 del
				m_pPrnPrint_EX2->SetPrintInfo(seltb);		// 24/07/10_個別タブから出力 add
			}
			else {
				//m_pPrnPrintKani_EX2->SetPrintInfo();		// 24/07/10_個別タブから出力 del
				m_pPrnPrintKani_EX2->SetPrintInfo(seltb);	// 24/07/10_個別タブから出力 add
			}
		}
	//<-----
		else {
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
				//m_pPrnPrint_EX->SetPrintInfo();			// 24/07/10_個別タブから出力 del
				m_pPrnPrint_EX->SetPrintInfo(seltb);		// 24/07/10_個別タブから出力 add
			}
			else {
				//m_pPrnPrintKani_EX->SetPrintInfo();		// 24/07/10_個別タブから出力 del
				m_pPrnPrintKani_EX->SetPrintInfo(seltb);	// 24/07/10_個別タブから出力 add
			}
		}
	}else{
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
			m_pPrnPrint->SetPrintInfo();
		}
		else{
			m_pPrnPrintKani->SetPrintInfo();
		}
	}
//---------------
	m_pPrnTune->SetInitPrnTune();

	//各種コントロール
	if (m_pPrnKazS) m_Kazei_DButton_sw	= m_pPrnKazS->m_Ctlsw;
	if (m_pPrnTokS) m_Toku_DButton_sw	= m_pPrnTokS->m_Ctlsw;
	if (m_pPrnZeiS) m_Zeiri_DButton_sw	= m_pPrnZeiS->m_Ctlsw;

	if( m_pPrintCmInfo->PgMx == 0 ){
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
		c_set( IDOK );
		return;
	}

	ICSDialog::OnOK();
}

void CPrnReportDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
//-- '15.02.03 --
//	m_pPrnPrint->SetPrintInfo();
//---------------

//----->('23.05.01 Add)
	long eymd = 0;
	if (!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
	else								eymd = m_pSnHeadData->Sn_KDAYE;
//<-----

// 24/04/04_341 add -->
	// 各タブの情報を取得
	if(m_pPrnKazS)	{
		m_pPrnKazS->GetInputItemData();
	}
	if(m_pPrnTokS)	{
		m_pPrnTokS->GetInputItemData();
	}
	if(m_pPrnZeiS)	{
		m_pPrnZeiS->GetInputItemData();
	}
// 24/04/04_341 add <--

	if (m_pShinInfo->ActViewVer == ID_VER_SYZ31) {
	//----->('23.05.01 Add)
		if ((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000)) { //２割特例計算を使用する
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
				//m_pPrnPrint_EX2->SetPrintInfo();		// 24/07/10_個別タブから出力 del
				m_pPrnPrint_EX2->SetPrintInfo(0x00);	// 24/07/10_個別タブから出力 add
			}
			else {
				//m_pPrnPrintKani_EX2->SetPrintInfo();		// 24/07/10_個別タブから出力 del
				m_pPrnPrintKani_EX2->SetPrintInfo(0x00);	// 24/07/10_個別タブから出力 add
			}
		}
	//<-----
		else {
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
				//m_pPrnPrint_EX->SetPrintInfo();		// 24/07/10_個別タブから出力 del
				m_pPrnPrint_EX->SetPrintInfo(0x00);		// 24/07/10_個別タブから出力 add
			}
			else {
				//m_pPrnPrintKani_EX->SetPrintInfo();	// 24/07/10_個別タブから出力 del
				m_pPrnPrintKani_EX->SetPrintInfo(0x00);	// 24/07/10_個別タブから出力 add
			}
		}
	}else{
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
			m_pPrnPrint->SetPrintInfo();
		}
		else{
			m_pPrnPrintKani->SetPrintInfo();
		}
	}
//---------------
	m_pPrnTune->SetInitPrnTune();

// 24/04/04_341 add -->
	if (m_pPrnKazS) m_Kazei_DButton_sw	= m_pPrnKazS->m_Ctlsw;
	if (m_pPrnTokS) m_Toku_DButton_sw	= m_pPrnTokS->m_Ctlsw;
	if (m_pPrnZeiS) m_Zeiri_DButton_sw	= m_pPrnZeiS->m_Ctlsw;
// 24/04/04_341 add <--

	ICSDialog::OnCancel();
}

BOOL CPrnReportDlg::PreTranslateMessage(MSG* pMsg)
{	
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	if( pMsg->wParam == VK_F10 ) 
		return TRUE;

	if( pMsg->message == WM_KEYDOWN ) {
		if( GetCombineKeyDown(VK_SHIFT) && VK_F1 <= pMsg->wParam && pMsg->wParam <= VK_F12 )	return TRUE;
		int	selno = m_PrnTab.GetCurSel();

		if( pMsg->wParam == VK_F11 ) {
			if( selno > 0 )	m_PrnTab.SetCurSel( selno - 1 );
			return TRUE;
		}
		if( pMsg->wParam == VK_F12 ) {
			//20240306 takemura 2-> 5

			int sel_max = 3;
			if (m_pPrnKazS != NULL) sel_max++;
			if (m_pPrnTokS != NULL) sel_max++;
			if (m_pPrnZeiS != NULL) sel_max++;

			if (selno < sel_max) {
				m_PrnTab.SetCurSel(selno + 1);
				SetTabForcs(selno + 1);
				
			}
			return TRUE;
		}
		if( pMsg->wParam == VK_F5 ) {
			OnBnClickedOk();
			return TRUE;
		}
		if( pMsg->wParam == VK_END && m_swBeForked ){//一括印刷フラグ後で追加しないといけないshimizu
			OnBnClickedOk();
			return TRUE;
		}
		if(pMsg->wParam == VK_F6)	{
			if(m_pPrnKazS != NULL) {
				if(selno == 0)	{
					long eymd = 0;
					if(!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
					else								eymd = m_pSnHeadData->Sn_KDAYE;
					if(m_pShinInfo->ActViewVer == ID_VER_SYZ31)	{
						if((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000))	{
							if(m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU)	{
								((CPrnPrint_EX2*)m_pPrnPrint_EX2)->OnBnClickedButton4();
							}
							else	{
								((CPrnPrintKani_EX2*)m_pPrnPrintKani_EX2)->OnBnClickedButton4();
							}
						}
						else	{
							if(m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU)	{
								((CPrnPrint_EX*)m_pPrnPrint_EX)->OnBnClickedButton4();
							}
							else	{
								((CPrnPrintKani_EX*)m_pPrnPrintKani_EX)->OnBnClickedButton4();
							}
						}
					}
				}
			}
		}
		if(pMsg->wParam == VK_F7)	{
			if(m_pPrnTokS != NULL) {
				if(selno == 0)	{
					long eymd = 0;
					if(!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
					else								eymd = m_pSnHeadData->Sn_KDAYE;
					if(m_pShinInfo->ActViewVer == ID_VER_SYZ31)	{
						if((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000))	{
							if(m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU)	{
								((CPrnPrint_EX2*)m_pPrnPrint_EX2)->OnBnClickedButton3();
							}
							else	{
								((CPrnPrintKani_EX2*)m_pPrnPrintKani_EX2)->OnBnClickedButton3();
							}
						}
						else	{
							if(m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU)	{
								((CPrnPrint_EX*)m_pPrnPrint_EX)->OnBnClickedButton3();
							}
							else	{
								((CPrnPrintKani_EX*)m_pPrnPrintKani_EX)->OnBnClickedButton3();
							}
						}
					}
				}
			}
		}
		if(pMsg->wParam == VK_F8)	{
			if(m_pPrnZeiS != NULL) {
				if(selno == 0)	{
					long eymd = 0;
					if(!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
					else								eymd = m_pSnHeadData->Sn_KDAYE;
					if(m_pShinInfo->ActViewVer == ID_VER_SYZ31)	{
						if((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000))	{
							if(m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU)	{
								((CPrnPrint_EX2*)m_pPrnPrint_EX2)->OnBnClickedButton5();
							}
							else	{
								((CPrnPrintKani_EX2*)m_pPrnPrintKani_EX2)->OnBnClickedButton5();
							}
						}
						else	{
							if(m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU)	{
								((CPrnPrint_EX*)m_pPrnPrint_EX)->OnBnClickedButton5();
							}
							else	{
								((CPrnPrintKani_EX*)m_pPrnPrintKani_EX)->OnBnClickedButton5();
							}
						}
					}
				}
			}
		}
		if( pMsg->wParam == VK_F9 ) {
//-- '15.02.03 --
//			if( selno == 0 )	((CPrnPrint*)m_pPrnPrint)->OnBnClickedButton1();
//---------------
			if( selno == 0 ){
			//----->('23.05.01 Add)
				long eymd = 0;
				if (!(m_pSnHeadData->Sn_SKKBN % 2)) eymd = m_pSnHeadData->Sn_MDAYE;
				else								eymd = m_pSnHeadData->Sn_KDAYE;
			//<-----
				if (m_pShinInfo->ActViewVer == ID_VER_SYZ31) {
				//----->('23.05.01 Add)
					if ((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000)) {	//２割特例計算を使用する
						if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
							((CPrnPrint_EX2*)m_pPrnPrint_EX2)->OnBnClickedButton1();
						}
						else {
							((CPrnPrintKani_EX2*)m_pPrnPrintKani_EX2)->OnBnClickedButton1();
						}
					}
				//<-----
					else {
						if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
							((CPrnPrint_EX*)m_pPrnPrint_EX)->OnBnClickedButton1();
						}
						else {
							((CPrnPrintKani_EX*)m_pPrnPrintKani_EX)->OnBnClickedButton1();
						}
					}
				}else{
					if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
						((CPrnPrint*)m_pPrnPrint)->OnBnClickedButton1();
					}
					else{
						((CPrnPrintKani*)m_pPrnPrintKani)->OnBnClickedButton1();
					}
				}
			}
//---------------
			//if( selno == 1 )	((CPrnItem*)m_pPrnItem)->OnBnClickedButton1();
			int	sw=0;	// 0:印刷項目設定　1:課税取引個別設定　2:特定収入個別設定　3:添付書面個別設定
			if(selno == 1)	{
				if(m_pPrnKazS != NULL)	{
					sw=1;
				}
				else if(m_pPrnTokS != NULL)	{
					sw=2;
				}
				else if(m_pPrnZeiS != NULL) {
					sw=3;
				}
			}
			else if(selno == 2)	{
				// 課税取引個別設定と特定収入個別設定が同時に表示されることはない
// 24/03/18_税務代理書面 cor -->
				//if(m_pPrnZeiS != NULL) {
				//	sw=3;
				//}
// -----------------------------
				if((m_pPrnKazS != NULL || m_pPrnTokS != NULL) && m_pPrnZeiS != NULL) {
					sw=3;
				}
				else if(m_pPrnKazS == NULL && m_pPrnTokS == NULL && m_pPrnZeiS == NULL) {
					sw=-1;
				}
// 24/03/18_税務代理書面 cor <--
			}
			if(sw == 1)	{
				((PrintKazeiSub*)m_pPrnKazS)->OnBnClickedKazAll();
			}
			else if(sw == 2)	{
				((PrintTokuteiSub*)m_pPrnTokS)->OnBnClickedTokAll();
			}
			else if(sw == 3)	{
				((PrintZeitempSub*)m_pPrnZeiS)->OnBnClickedZeiAll();
			}
// 24/03/18_税務代理書面 cor -->
			//else	{
// -----------------------------
			else if(sw == 0)	{
// 24/03/18_税務代理書面 cor <--
				((CPrnItem*)m_pPrnItem)->OnBnClickedButton1();
			}

			return( TRUE );	
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

void CPrnReportDlg::OnTcnSelchangeCustomPrint(NMHDR * pNotifyStruct, LRESULT * result)
{
// 24/03/27_168703 cor -->
	//if( m_PrnTab.GetCurSel() == 1 )	((CPrnItem*)m_pPrnItem)->ItemInit();
// -----------------------
	int		selno=m_PrnTab.GetCurSel();
	int		sw=0;
	if(selno == 1)	{
		if(m_pPrnKazS != NULL)		sw=0;
		else if(m_pPrnTokS != NULL)	sw=0;
		else if(m_pPrnZeiS != NULL)	sw=0;
		else						sw=1;
	}
	else if(selno == 2)	{
		if((m_pPrnKazS != NULL || m_pPrnTokS != NULL) && m_pPrnZeiS != NULL)		sw=0;
		else if(m_pPrnKazS == NULL && m_pPrnTokS == NULL && m_pPrnZeiS == NULL)		sw=0;
		else																		sw=1;
	}
	else if(selno == 3)	{
		sw=1;
	}
	if(sw != 0)	{
		((CPrnItem*)m_pPrnItem)->ItemInit();
	}
// 24/03/27_168703 cor <--

//-- '15.02.03 --
//	switch( m_PrnTab.GetCurSel() ){
//	case 0: m_pPrnPrint->SetFocus(); break;
//	case 1: m_pPrnItem->SetFocus(); break;
//	case 2: m_pPrnTune->SetFocus(); break;
//	}
//---------------

//----->('23.05.01 Add)
	long eymd = 0;
	if (!(m_pSnHeadData->Sn_SKKBN % 2)) eymd = m_pSnHeadData->Sn_MDAYE;
	else								eymd = m_pSnHeadData->Sn_KDAYE;
//<-----

	switch( m_PrnTab.GetCurSel() ){
		case 0:
			if (m_pShinInfo->ActViewVer == ID_VER_SYZ31) {
			//----->('23.05.01 Add)
				if ((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000)) {	//２割特例計算を使用する
					if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
						m_pPrnPrint_EX2->SetFocus();
					}
					else {
						m_pPrnPrintKani_EX2->SetFocus();
					}
				}
			//<-----
				else {
					if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU)	{
						// 個別指定の選択状況を見て、背景色を変える
						//m_pPrnPrint_EX->PostMessage(WM_MY_COLORCHANGE, 0, 0);	// 修正No.160356,168678 add

						m_pPrnPrint_EX->SetFocus();
					}
					else {
						m_pPrnPrintKani_EX->SetFocus();
					}
				}
			}else{
				if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
					m_pPrnPrint->SetFocus();
				}
				else{
					m_pPrnPrintKani->SetFocus();
				}
			}
			break;
		case 1:
			SetTabForcs(1);
	//		m_pPrnItem->SetFocus();
			break;
		case 2:
			SetTabForcs(2);
	//		m_pPrnTune->SetFocus();
			break;
		case 3:
			SetTabForcs(3);
	//		m_pPrnTune->SetFocus();
			break;
		case 4:
			SetTabForcs(4);
	//		m_pPrnTune->SetFocus();
			break;
		case 5:
			SetTabForcs(5);
	//		m_pPrnTune->SetFocus();
			break;
	}
//---------------
}

//タブへのカーソルセット
void CPrnReportDlg::SetFocusTab(int Type) 
{
	if(Type == 1)		m_pPrnKazS->PostMessage(WM_ACTIVE_SETFORCUS, 0, 0);
	else if (Type == 2) m_pPrnTokS->PostMessage(WM_ACTIVE_SETFORCUS, 0, 0);
	else if (Type == 3) m_pPrnZeiS->PostMessage(WM_ACTIVE_SETFORCUS, 0, 0);
	   	 
}

void CPrnReportDlg::SetTabForcs(int TabNum)
{
	int TargetNum = 1;

	//if (m_pPrnKazS != NULL) {
	//	if(TabNum == TargetNum)	m_pPrnKazS->SetFocus();
	//	else TargetNum++;
	//}

	if (m_pPrnKazS != NULL) {
		if (TabNum == TargetNum)	{
			//m_pPrnKazS->SetFocus();
			m_pPrnKazS->PostMessage(WM_ACTIVE_SETFORCUS, 0, 0);
			return;
		}
		else TargetNum++;
	}

	if (m_pPrnTokS != NULL) {
		if (TabNum == TargetNum)	{
			//m_pPrnTokS->SetFocus();
			m_pPrnTokS->PostMessage(WM_ACTIVE_SETFORCUS, 0, 0);
			return;
		}
		else TargetNum++;
	}

	if (m_pPrnZeiS != NULL) {
		if (TabNum == TargetNum)	{
			//m_pPrnZeiS->SetFocus();
			m_pPrnZeiS->PostMessage(WM_ACTIVE_SETFORCUS, 0, 0);
			return;
		}
		else TargetNum++;
	}

	if (m_pPrnItem != NULL) {
		if (TabNum == TargetNum)	{
			// 印刷設定だけPostMessageするとF9が反応しなくなる
			m_pPrnItem->SetFocus();
			//m_pPrnItem->PostMessage(WM_ACTIVE_SETFORCUS, 0, 0);
			return;
		}
		else TargetNum++;
	}

	if (m_pPrnPrint != NULL) {
		if (TabNum == TargetNum)	{
			//m_pPrnPrint->SetFocus();
			m_pPrnPrint->PostMessage(WM_ACTIVE_SETFORCUS, 0, 0);
			return;
		}
		else TargetNum++;
	}
}

// 24/03/22_背景色 add -->
// ---------------------------------------------------------------------------------
// 個別タブのチェック状況を返す
// ---------------------------------------------------------------------------------
// 【引　数】	int		0:課税取引金額計算表　1:特定収入計算表　2:税務代理書面
// 【戻り値】	bool	TRUE:すべてON　FALSE:それ以外
// ---------------------------------------------------------------------------------
bool CPrnReportDlg::IsAllCheckOn( int pSw )
{
	bool				brv=true;

// 24/03/25_168691 cor -->
	//if(pSw == 0)		brv = m_pPrnKazS->IsAllCheckOn();
	//else if(pSw == 1)	brv = m_pPrnTokS->IsAllCheckOn();
	//else				brv = m_pPrnZeiS->IsAllCheckOn();
// -----------------------
	if(pSw == 0)	{
		if(m_pPrnKazS)	{
			brv = m_pPrnKazS->IsAllCheckOn();
		}
	}
	else if(pSw == 1)	{
		if(m_pPrnTokS)	{
			brv = m_pPrnTokS->IsAllCheckOn();
		}
	}
	else	{
		if(m_pPrnZeiS)	{
			brv = m_pPrnZeiS->IsAllCheckOn();
		}
	}
// 24/03/25_168691 cor <--

	return(brv);
}

// ---------------------------------------------------------------------------------
// 申告書出力設定タブの背景色を変更する
// ---------------------------------------------------------------------------------
void CPrnReportDlg::BackColorControl( int pSw )
{
	if(m_pPrnPrint_EX)		m_pPrnPrint_EX->BackColorControl2(pSw);
	if(m_pPrnPrint_EX2)		m_pPrnPrint_EX2->BackColorControl2(pSw);
	if(m_pPrnPrintKani_EX)	m_pPrnPrintKani_EX->BackColorControl2(pSw);
	if(m_pPrnPrintKani_EX2)	m_pPrnPrintKani_EX2->BackColorControl2(pSw);
}
// 24/03/22_背景色 add <--

// 24/04/03_checkon add -->
// ---------------------------------------------------------------------------------
// 選択可能な個別タブのチェックをすべてONにする
// ---------------------------------------------------------------------------------
// 【引　数】	int		0:課税取引金額計算表　1:特定収入計算表　2:税務代理書面
// 【戻り値】	なし
// ---------------------------------------------------------------------------------
void CPrnReportDlg::AllCheckOn( int pSw )
{
	if(pSw == 0)	{
		if(m_pPrnKazS)	{
			m_pPrnKazS->AllCheckOn();
		}
	}
	else if(pSw == 1)	{
		if(m_pPrnTokS)	{
			m_pPrnTokS->AllCheckOn();
		}
	}
	else	{
		if(m_pPrnZeiS)	{
			m_pPrnZeiS->AllCheckOn();
		}
	}
}
// 24/04/03_checkon add <--

// 24/07/10_個別タブから出力 add -->
// -----------------------------------------------------------------------------
//	入力制限の設定を確認
// -----------------------------------------------------------------------------
//	【引　数】	なし
//	【戻り値】	int	0=税務代理書面未作成
// -----------------------------------------------------------------------------
int CPrnReportDlg::ZeitempEnableChk(void)
{
	int									ii=0;
	int									yflg=0,yno=0;
	int									flg=0;
	int									bnb;
	CArray<TAXAGENT_STFF,TAXAGENT_STFF>	car;

	// 呼び出し元業務
	if(m_Util.IsUnConnectMst(m_pZmSub->zvol))	yflg = 1;	// 消費税申告書（非連動）
	else										yflg = 0;	// 消費税申告書

	flg = 0;
	for(ii=0; ii<ZEI_MAXCOL+1; ii++)	{
		switch(ii)	{
			case 0:		// 税務代理権限証書
				yno = 0x01;
				bnb = ZtenpGetPrintPage(m_pZmSub->m_database, yflg, yno, 1, 1, &car, this);
				if(bnb != 0)	flg += 1;				
				break;

			case 1:		// 申告書の作成に関する計算事項記載書面 33の2(1) (1面)
				yno = 0x02;
				bnb = ZtenpGetPrintPage(m_pZmSub->m_database, yflg, yno, 1, 1, &car, this);
				if(bnb != 0)	flg += 1;
				break;

			case 2:		// 申告書の作成に関する計算事項記載書面 33の2(1) (2面)
				yno = 0x04;
				bnb = ZtenpGetPrintPage(m_pZmSub->m_database, yflg, yno, 1, 1, &car, this);
				if(bnb != 0)	flg += 1;
				break;

			case 3:		// 申告書の作成に関する計算事項記載書面 33の2(1) (3面)
				yno = 0x08;
				bnb = ZtenpGetPrintPage(m_pZmSub->m_database, yflg, yno, 1, 1, &car, this);
				if(bnb != 0)	flg += 1;
				break;

			case 4:		// 申告書の作成に関する計算事項記載書面 33の2(1) (4面 1枚目)
				yno = 0x10;
				bnb = ZtenpGetPrintPage(m_pZmSub->m_database, yflg, yno, 1, 1, &car, this);
				if(bnb != 0)	flg += 1;
				break;

			case 5:		// 申告書の作成に関する計算事項記載書面 33の2(1) (4面 2枚目)
				yno = 0x20;
				bnb = ZtenpGetPrintPage(m_pZmSub->m_database, yflg, yno, 1, 1, &car, this);
				if(bnb != 0)	flg += 1;
				break;

			case 6:		// 申告書に関する審査事項記載書面 33の2(2) (1面)
				yno = 0x40;
				bnb = ZtenpGetPrintPage(m_pZmSub->m_database, yflg, yno, 1, 1, &car, this);
				if(bnb != 0)	flg += 1;
				break;

			case 7:		// 申告書に関する審査事項記載書面 33の2(2) (2面)
				yno = 0x80;
				bnb = ZtenpGetPrintPage(m_pZmSub->m_database, yflg, yno, 1, 1, &car, this);
				if(bnb != 0)	flg += 1;
				break;

			case 8:		// 申告書に関する審査事項記載書面 33の2(2) (3面)
				yno = 0x100;
				bnb = ZtenpGetPrintPage(m_pZmSub->m_database, yflg, yno, 1, 1, &car, this);
				if(bnb != 0)	flg += 1;
				break;

			case 9:		// 申告書に関する審査事項記載書面 33の2(2) (4面 1枚目)
				yno = 0x200;
				bnb = ZtenpGetPrintPage(m_pZmSub->m_database, yflg, yno, 1, 1, &car, this);
				if(bnb != 0)	flg += 1;
				break;

			case 10:	// 申告書に関する審査事項記載書面 33の2(2) (4面 2枚目)
				yno = 0x400;
				bnb = ZtenpGetPrintPage(m_pZmSub->m_database, yflg, yno, 1, 1, &car, this);
				if(bnb != 0)	flg += 1;
				break;

			default:
				break;
		}
	}
	
	return(flg);
}

// -----------------------------------------------------------------------------
//	特定収入計算表タブの SetEnableItem関数 と SetInputItem関数 を呼び出し
// -----------------------------------------------------------------------------
//	【引　数】	なし
//	【戻り値】	なし
// -----------------------------------------------------------------------------
void CPrnReportDlg::CallSetEnableInputItem(void)
{
	if(m_pPrnTokS)	{
		m_pPrnTokS->SetEnableItem(-1);
		m_pPrnTokS->SetInputItem();
	}
}
// 24/07/10_個別タブから出力 add <--
