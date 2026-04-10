// PrintTokuteiSub.cpp : 実装ファイル
//

#include "stdafx.h"
//#include "pch.h"
#include "DBSyzShinMain.h"
#include "PrnReportDlg.h"

#include "PrintTokuteiSub.h"
// 24/03/21_税務代理書面 del -->
//#include "PrnPrintSubIdx.h"
// 24/03/21_税務代理書面 del <--
#include "afxdialogex.h"

// ※　MAP0番目には　//個別に出力設定（既存のものがある為）
extern PRNEXSUB_INF									m_PrnEXSub;
extern CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub;

short	CursorPosTOK[TOK_MAXCOL+1][TOK_MAXROW+1] =
{
// 24/07/10_個別タブから出力 cor -->
	//{ CHEK_TO01,-1,-1,-1,-1 },
	//{ CHEK_TO02,-1,-1,-1,-1 },
	//{ CHEK_TO03,-1,-1,-1,-1 },
	//{ CHEK_TO04,-1,-1,-1,-1 },
	//{ CHEK_TO05,-1,-1,-1,-1 },
	//{ CHEK_TO06,-1,-1,-1,-1 },
	//{ CHEK_TO07,-1,-1,-1,-1 },
	//{ CHEK_TO08,-1,CHEK_TO08_ST,CHEK_TO08_ED,-1 },
	//{ CHEK_TO09,-1,CHEK_TO09_ST,CHEK_TO09_ED,-1 },
	//{ CHEK_TO10,-1,CHEK_TO10_ST,CHEK_TO10_ED,-1 },
	//{ CHEK_TO11,-1,CHEK_TO11_ST,CHEK_TO11_ED,-1 },
	//{ CHEK_TO12,-1,CHEK_TO12_ST,CHEK_TO12_ED,-1 },
	//{ CHEK_TO13,-1,CHEK_TO13_ST,CHEK_TO13_ED,-1 },
	//{ CHEK_TO14,-1,CHEK_TO14_ST,CHEK_TO14_ED,-1 },
	//{ CHEK_TO15,-1,CHEK_TO15_ST,CHEK_TO15_ED,-1 },
	//{ -1,-1,-1,-1,-1 }
// ---------------------------------
	{ CHEK_TO01,CHEK_TO01_BUS,-1,-1,-1,-1 },
	{ CHEK_TO02,CHEK_TO02_BUS,-1,-1,-1,-1 },
	{ CHEK_TO03,CHEK_TO03_BUS,-1,-1,-1,-1 },
	{ CHEK_TO04,CHEK_TO04_BUS,-1,-1,-1,-1 },
	{ CHEK_TO05,CHEK_TO05_BUS,-1,-1,-1,-1 },
	{ CHEK_TO06,CHEK_TO06_BUS,-1,-1,-1,-1 },
	{ CHEK_TO07,CHEK_TO07_BUS,-1,-1,-1,-1 },
	{ CHEK_TO08,CHEK_TO08_BUS,-1,CHEK_TO08_ST,CHEK_TO08_ED,-1 },
	{ CHEK_TO09,CHEK_TO09_BUS,-1,CHEK_TO09_ST,CHEK_TO09_ED,-1 },
	{ CHEK_TO10,CHEK_TO10_BUS,-1,CHEK_TO10_ST,CHEK_TO10_ED,-1 },
	{ CHEK_TO11,CHEK_TO11_BUS,-1,CHEK_TO11_ST,CHEK_TO11_ED,-1 },
	{ CHEK_TO12,CHEK_TO12_BUS,-1,CHEK_TO12_ST,CHEK_TO12_ED,-1 },
	{ CHEK_TO13,CHEK_TO13_BUS,-1,CHEK_TO13_ST,CHEK_TO13_ED,-1 },
	{ CHEK_TO14,CHEK_TO14_BUS,-1,CHEK_TO14_ST,CHEK_TO14_ED,-1 },
	{ CHEK_TO15,CHEK_TO15_BUS,-1,CHEK_TO15_ST,CHEK_TO15_ED,-1 },
	{ -1,-1,-1,-1,-1,-1 }
// 24/07/10_個別タブから出力 cor <--
};

// PrintTokuteiSub ダイアログ

IMPLEMENT_DYNAMIC(PrintTokuteiSub, ICSDialog)

PrintTokuteiSub::PrintTokuteiSub(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_DIALOG_PRN_TOKUTEI_SUB, pParent)
	, m_pSnHeadData(NULL)
	, m_pZmSub(NULL)
	, m_pShinInfo(NULL)
	, m_pPrintCmInfo(NULL)
	, m_SelSw(0)
	, m_Ctlsw(0)
	, m_PoutKcode(0)
{
	m_pParent = pParent;

// 24/03/21_印刷枚数 add -->
	for(int ii=0; ii<TOK_MAXCOL; ii++)	{
		m_Bunsi[ii]=0;
		m_Bunbo[ii]=0;
	}
// 24/03/21_印刷枚数 add <--
// 24/03/22_168718 add -->
	for(int ii=0; ii<256; ii++)	m_Sel[ii]=0;
// 24/03/22_168718 add <--
}

PrintTokuteiSub::~PrintTokuteiSub()
{
}

void PrintTokuteiSub::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRN_TOKUTEI_SUB_DG, m_Diag_Tok);
	DDX_Control(pDX, IDC_Tokutei_code, m_Check_Tok);
}


BEGIN_MESSAGE_MAP(PrintTokuteiSub, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &PrintTokuteiSub::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON7, &PrintTokuteiSub::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_TOK_ALL, &PrintTokuteiSub::OnBnClickedTokAll)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_Tokutei_code, &PrintTokuteiSub::OnBnClickedTokuteicode)
	ON_MESSAGE(WM_ACTIVE_SETFORCUS, OnMyFocusControl)
	
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(PrintTokuteiSub, ICSDialog)
	ON_EVENT(PrintTokuteiSub, IDC_PRN_TOKUTEI_SUB_DG, 2, PrintTokuteiSub::EditOFFPrnTokuteiSub, VTS_I2)
	ON_EVENT(PrintTokuteiSub, IDC_PRN_TOKUTEI_SUB_DG, 3, PrintTokuteiSub::TerminationPrnTokuteiSub, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(PrintTokuteiSub, IDC_PRN_TOKUTEI_SUB_DG, 9, PrintTokuteiSub::CheckButtonPrnTokuteiSubDg, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()


// PrintTokuteiSub メッセージ ハンドラー
//-----------------------------------------------------------------------------
// 初期情報セット
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	消費税ヘッダ情報
//		pZmSub		：	公益クラス
//		pShinInfo	：	共通情報
//		pPrintCmInfo：	印刷情報
//		pBeForked	：	1:一括印刷
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int PrintTokuteiSub::InitInfo(CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, SH_PRINT_CMINFO *pPrintCmInfo, int pBeForked, int Ctlsw)
{
	ASSERT(pSnHeadData);
	if (pSnHeadData == NULL) {
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT(pZmSub);
	if (pZmSub == NULL) {
		return -1;
	}
	m_pZmSub = pZmSub;

	ASSERT(pShinInfo);
	if (pShinInfo == NULL) {
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT(pPrintCmInfo);
	if (pPrintCmInfo == NULL) {
		return -1;
	}
	m_pPrintCmInfo = pPrintCmInfo;

	m_swBeForked = pBeForked;
	m_Ctlsw = Ctlsw;

	return 0;
}

BOOL PrintTokuteiSub::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Init();
	m_SelSw = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

LRESULT PrintTokuteiSub::OnMyFocusControl(WPARAM w, LPARAM l)
{
	//初期セット
	m_Diag_Tok.SetFocus();

	//m_Diag_Tok.SetPosition(CHEK_TO01);
	int					yy=0;
	DIAGRAM_ATTRIBUTE	da;
	while(1)	{
		if(CursorPosTOK[yy][0] == -1)	{
			break;
		}
		diag_getattr(IDC_PRN_TOKUTEI_SUB_DG,CursorPosTOK[yy][0],&da,CIcsdiagctrl);
		if(da.attr_bcolor != BC_GRAY)	{								// 修正No.168720 del
			if(da.attr_bcolor == BC_WHITE)	{
				m_Diag_Tok.SetPosition(CursorPosTOK[yy][0]);
				break;
			}
		}
		yy++;
	}
// 24/03/25_168694 add -->
	if(CursorPosTOK[yy][0] == -1)	{
		GetDlgItem(IDC_Tokutei_code)->SetFocus();
	}
// 24/03/25_168694 add <--
	return 0;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int PrintTokuteiSub::Init()
{
	//m_Check_Tok.SetCheck(m_Ctlsw & 0x01);
	m_Check_Tok.SetCheck(m_pPrintCmInfo->OPL ? FALSE : TRUE);

	// タイトルの背景色
	ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, TITLE_TO00, 0);
	ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, TITLE_TO04, 0);	// 24/07/10_個別タブから出力 add
	ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, TITLE_TO01, 0);
	ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, TITLE_TO02, 0);
	ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, TITLE_TO03, 0);


// 24/03/21_168685 cor -->
	//// 入力項目設定
	//SetInputItem();
	//// 入力制限の設定
	//SetEnableItem(-1);
// -----------------------
	// 入力制限の設定
	SetEnableItem(-1);
	// 入力項目設定
	SetInputItem();
// 24/03/21_168685 cor <--

	// 環境共通設定の背景色を付ける
	ICSColorInfo	info;
	int				rv = 0;
	rv = ((CDBSyzShinMainApp*)AfxGetApp())->GetStanderdColor(info);
	if (rv == 0 && info.m_swOnOff == 1) {
	//	m_Diag1.SetBackColor(info.ViewColor());
	}

	/*
	// ------------------------------------------------------------
	// 中央表示
	// ------------------------------------------------------------
	DIAGRAM_ATTRIBUTE	dattr;
	int					ii = 0;
	// チェックボックス
	for (ii = HP_TEI_CHK; ii <= KZ_HIK_CHK; ii += 2) {
		diag_getattr(IDC_PRN_KAZEI_SUB_DG, ii, &dattr, CIcsdiagctrl);
		dattr.attr_align = DT_CENTER;
		diag_setattr(IDC_PRN_KAZEI_SUB_DG, ii, &dattr, TRUE, CIcsdiagctrl);
	}
	*/

// 24/07/10_個別タブから出力 add -->
	GetDlgItem(IDC_STATIC_INCOME)->SetWindowText(" ");
	if(*m_pShinInfo->pSgSpc) {
		if(m_pSnHeadData->Sn_Sign2 & 0x02) {
			CString	Mes;
			Mes = _T("特定収入割合が５％以下です。この場合、課税仕入等の税額の\n");
			Mes += _T("調整は不要のため、申告書・付表には計上しません。");
			GetDlgItem(IDC_STATIC_INCOME)->SetWindowText(Mes);
		}
	}
// 24/07/10_個別タブから出力 add <--

// 修正No.168895 add -->
	// 業種の設定によって事業(一般)が先頭行でない時もあるため、先頭行が何行目か取得しておく
	m_Yy = 0;
	DIAGRAM_ATTRIBUTE	da;
	//while(1)	{					// 修正No.168918 del
	while(m_Yy < TOK_MAXCOL)	{	// 修正No.168918 add
		diag_getattr(IDC_PRN_TOKUTEI_SUB_DG, CursorPosTOK[m_Yy][0], &da, CIcsdiagctrl);
		if(da.attr_bcolor == BC_WHITE)	{
			break;
		}
		m_Yy++;
	}
// 修正No.168895 add <--

	return 0;
}



BOOL PrintTokuteiSub::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
		// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if (pMsg->wParam == VK_F10)
		return TRUE;
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if (pMsg->message == WM_KEYDOWN) {
		if (GetCombineKeyDown(VK_SHIFT) && VK_F1 <= pMsg->wParam && pMsg->wParam <= VK_F12)	return TRUE;
		if (pMsg->wParam == VK_ESCAPE) {

			((CPrnReportDlg*)m_pParent)->OnBnClickedCancel();
			return TRUE;
		}

		if ((pMsg->wParam == VK_RETURN) ||
			((pMsg->wParam == VK_TAB) && !(::GetKeyState(VK_SHIFT)))) {

			int id = GetFocus()->GetDlgCtrlID();

			if ((id == IDC_PRN_TOKUTEI_SUB_DG) && (m_Diag_Tok.GetPosition() == CHEK_TO15_ED)) {
				GetDlgItem(IDC_TOK_ALL)->SetFocus();
				return(TRUE);
			}
			if ((id == IDC_TOK_ALL)) {
				GetDlgItem(IDC_Tokutei_code)->SetFocus();
				return(TRUE);
			}
// 24/03/25_168692 cor -->
			//if ((id == IDC_Tokutei_code) && (pMsg->wParam == VK_TAB)) {
// -----------------------
			if (id == IDC_Tokutei_code) {
// 24/03/25_168692 cor <--
				GetDlgItem(IDC_BUTTON2)->SetFocus();
				return(TRUE);
			}
			if ((id == IDC_BUTTON2) && (pMsg->wParam == VK_TAB)) {
				GetDlgItem(IDC_BUTTON7)->SetFocus();
				return(TRUE);
			}
			if ((id == IDC_BUTTON7) && (pMsg->wParam == VK_TAB)) {
				m_Diag_Tok.SetFocus();
				m_Diag_Tok.SetPosition(CHEK_TO01);
				return(TRUE);
			}
		}

		if (pMsg->wParam == VK_DELETE) {
			// 法人税と同じようにDeleteキーは反応しないようにする
			return(TRUE);
		}
		if (pMsg->wParam == VK_F9) {
			OnBnClickedTokAll();
			return TRUE;
		}
		if (pMsg->wParam == VK_F11) {
			OnBnClickedButton2();
			return TRUE;
		}
		if (pMsg->wParam == VK_F12) {
			OnBnClickedButton7();
			return TRUE;
		}
		if (pMsg->wParam == VK_F5) {
			((CPrnReportDlg*)m_pParent)->PostMessage(WM_KEYDOWN, VK_F5);
			return TRUE;
		}
		// 依頼No.971
		if (pMsg->wParam == VK_END && m_swBeForked) {
			((CPrnReportDlg*)m_pParent)->PostMessage(WM_KEYDOWN, VK_END);
			return(TRUE);
		}
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}


void PrintTokuteiSub::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	ICSDialog::OnCancel();
}


void PrintTokuteiSub::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	ICSDialog::OnOK();
}


void PrintTokuteiSub::OnBnClickedButton2()
{
	GetParent()->GetParent()->PostMessage(WM_KEYDOWN, VK_F11);
}


void PrintTokuteiSub::OnBnClickedButton7()
{
	GetParent()->GetParent()->PostMessage(WM_KEYDOWN, VK_F12);
}


void PrintTokuteiSub::OnBnClickedTokAll()
{
	ChoiceItem(0);
	GetInputItemData();

	// 入力項目設定
	SetInputItem();
	//枚数の再セット
	ReSetAllNumDg();
// 24/03/22_背景色 add -->
	((CPrnReportDlg*)m_pParent)->BackColorControl(1);
// 24/03/22_背景色 add <--
// 24/03/25_168694 add -->
	PostMessage(WM_ACTIVE_SETFORCUS,0,0);
// 24/03/25_168694 add <--
}

// -----------------------------------------------------------------------------
// 入力項目設定
// -----------------------------------------------------------------------------
// 【引　数】	なし
// 【戻り値】	なし
// -----------------------------------------------------------------------------
int PrintTokuteiSub::SetInputItem(void)
{
	int					ii = 0, jj = 0;
// 修正No.168711 add -->
	int					st, ed;
	int					bs;					// 24/07/10_個別タブから出力 add
	long				flg;				// 入力制限サイン

	CTaxationList31View	*pTaxlist31View = NULL;
	pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));
	// 入力制限のサインをセット
// 24/07/10_個別タブから出力 cor -->
	//pTaxlist31View->IsSpecificEnable(&flg);
// ---------------------------------
	flg = 0;
	// 「全表出力可」チェックON もしくは 特定収入計算表が出力できるデータ
	if((m_pPrintCmInfo->OP_PSEL & 0x02) || (!(m_pSnHeadData->Sn_Sign2 & 0x02)))	{
		pTaxlist31View->IsSpecificEnable(&flg);
	}
// 24/07/10_個別タブから出力 cor <--
// 修正No.168711 add <--

	for (ii = 1; ii < TOK_MAXCOL + 1; ii++) {
		if (m_pMapPrnEXSub.GetCount() > 0) {
			::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));
			m_pMapPrnEXSub.Lookup(ii, m_PrnEXSub);
		}

		DIAGRAM_DATA	dd;
		switch (ii) {
// 24/07/10_個別タブから出力 cor -->
//		case 1:		// 計算表１ データをセット チェックON/OFF 部数
//			SetInputItemSub(CHEK_TO01, CHEK_TO01_NUM, -1, -1, &m_PrnEXSub, &dd);
//			break;
//
//		case 2:		// 計算表２(１)
//			SetInputItemSub(CHEK_TO02, CHEK_TO02_NUM, -1, -1, &m_PrnEXSub, &dd);
//			break;
//
//		case 3:		// 計算表２(２)
//			SetInputItemSub(CHEK_TO03, CHEK_TO03_NUM, -1, -1, &m_PrnEXSub, &dd);
//			break;
//
//		case 4:		// 計算表３、４
//			SetInputItemSub(CHEK_TO04, CHEK_TO04_NUM, -1, -1, &m_PrnEXSub, &dd);
//			break;
//
//		case 5:		// 計算表５(１)
//			SetInputItemSub(CHEK_TO05, CHEK_TO05_NUM, -1, -1, &m_PrnEXSub, &dd);
//			break;
//
//		case 6:		// 計算表５(２)
//			SetInputItemSub(CHEK_TO06, CHEK_TO06_NUM, -1, -1, &m_PrnEXSub, &dd);
//			break;
//
//		case 7:		// 計算表５(３)
//			SetInputItemSub(CHEK_TO07, CHEK_TO07_NUM, -1, -1, &m_PrnEXSub, &dd);
//			break;
//
//// 修正No.168711 cor -->
//		//case 8:		// 計算表５－２(１)
//		//	SetInputItemSub(CHEK_TO08, CHEK_TO08_NUM, CHEK_TO08_ST, CHEK_TO08_ED, &m_PrnEXSub, &dd);
//		//	break;
//
//		//case 9:		// 計算表５－２(２)
//		//	SetInputItemSub(CHEK_TO09, CHEK_TO09_NUM, CHEK_TO09_ST, CHEK_TO09_ED, &m_PrnEXSub, &dd);
//		//	break;
//
//		//case 10:	// 計算表５－２(３)－１
//		//	SetInputItemSub(CHEK_TO10, CHEK_TO10_NUM, CHEK_TO10_ST, CHEK_TO10_ED, &m_PrnEXSub, &dd);
//		//	break;
//
//		//case 11:	// 計算表５－２(３)－２
//		//	SetInputItemSub(CHEK_TO11, CHEK_TO11_NUM, CHEK_TO11_ST, CHEK_TO11_ED, &m_PrnEXSub, &dd);
//		//	break;
//
//		//case 12:	// 計算表５－２(３)－３
//		//	SetInputItemSub(CHEK_TO12, CHEK_TO12_NUM, CHEK_TO12_ST, CHEK_TO12_ED, &m_PrnEXSub, &dd);
//		//	break;
//
//		//case 13:	// 計算表５－２(４)－１
//		//	SetInputItemSub(CHEK_TO13, CHEK_TO13_NUM, CHEK_TO13_ST, CHEK_TO13_ED, &m_PrnEXSub, &dd);
//		//	break;
//
//		//case 14:	// 計算表５－２(４)－２
//		//	SetInputItemSub(CHEK_TO14, CHEK_TO14_NUM, CHEK_TO14_ST, CHEK_TO14_ED, &m_PrnEXSub, &dd);
//		//	break;
//
//		//case 15:	// 計算表５－２(４)－３
//		//	SetInputItemSub(CHEK_TO15, CHEK_TO15_NUM, CHEK_TO15_ST, CHEK_TO15_ED, &m_PrnEXSub, &dd);
//		//	break;
//// ---------------------
//		case 8:		// 計算表５－２(１)
//			st = ed = -1;
//			if((flg & 0x0080))	{
//				st = CHEK_TO08_ST;
//				ed = CHEK_TO08_ED;
//			}
//			SetInputItemSub(CHEK_TO08, CHEK_TO08_NUM, st, ed, &m_PrnEXSub, &dd);
//			break;
//
//		case 9:		// 計算表５－２(２)
//			st = ed = -1;
//			if((flg & 0x0100))	{
//				st = CHEK_TO09_ST;
//				ed = CHEK_TO09_ED;
//			}
//			SetInputItemSub(CHEK_TO09, CHEK_TO09_NUM, st, ed, &m_PrnEXSub, &dd);
//			break;
//
//		case 10:	// 計算表５－２(３)－１
//			st = ed = -1;
//			if((flg & 0x0200))	{
//				st = CHEK_TO10_ST;
//				ed = CHEK_TO10_ED;
//			}
//			SetInputItemSub(CHEK_TO10, CHEK_TO10_NUM, st, ed, &m_PrnEXSub, &dd);
//			break;
//
//		case 11:	// 計算表５－２(３)－２
//			st = ed = -1;
//			if((flg & 0x0400))	{
//				st = CHEK_TO11_ST;
//				ed = CHEK_TO11_ED;
//			}
//			SetInputItemSub(CHEK_TO11, CHEK_TO11_NUM, st, ed, &m_PrnEXSub, &dd);
//			break;
//
//		case 12:	// 計算表５－２(３)－３
//			st = ed = -1;
//			if((flg & 0x0800))	{
//				st = CHEK_TO12_ST;
//				ed = CHEK_TO12_ED;
//			}
//			SetInputItemSub(CHEK_TO12, CHEK_TO12_NUM, st, ed, &m_PrnEXSub, &dd);
//			break;
//
//		case 13:	// 計算表５－２(４)－１
//			st = ed = -1;
//			if((flg & 0x1000))	{
//				st = CHEK_TO13_ST;
//				ed = CHEK_TO13_ED;
//			}
//			SetInputItemSub(CHEK_TO13, CHEK_TO13_NUM, st, ed, &m_PrnEXSub, &dd);
//			break;
//
//		case 14:	// 計算表５－２(４)－２
//			st = ed = -1;
//			if((flg & 0x2000))	{
//				st = CHEK_TO14_ST;
//				ed = CHEK_TO14_ED;
//			}
//			SetInputItemSub(CHEK_TO14, CHEK_TO14_NUM, st, ed, &m_PrnEXSub, &dd);
//			break;
//
//		case 15:	// 計算表５－２(４)－３
//			st = ed = -1;
//			if((flg & 0x4000))	{
//				st = CHEK_TO15_ST;
//				ed = CHEK_TO15_ED;
//			}
//			SetInputItemSub(CHEK_TO15, CHEK_TO15_NUM, st, ed, &m_PrnEXSub, &dd);
//			break;
//// 修正No.168711 cor <--
// ---------------------------------
		case 1:		// 計算表１ データをセット チェックON/OFF 部数
			SetInputItemSub((flg & 0x0001), CHEK_TO01, CHEK_TO01_BUS, CHEK_TO01_NUM, -1, -1, &m_PrnEXSub, &dd);
			break;

		case 2:		// 計算表２(１)
			SetInputItemSub((flg & 0x0002), CHEK_TO02, CHEK_TO02_BUS, CHEK_TO02_NUM, -1, -1, &m_PrnEXSub, &dd);
			break;

		case 3:		// 計算表２(２)
			SetInputItemSub((flg & 0x0004), CHEK_TO03, CHEK_TO03_BUS, CHEK_TO03_NUM, -1, -1, &m_PrnEXSub, &dd);
			break;

		case 4:		// 計算表３、４
			SetInputItemSub((flg & 0x0008), CHEK_TO04, CHEK_TO04_BUS, CHEK_TO04_NUM, -1, -1, &m_PrnEXSub, &dd);
			break;

		case 5:		// 計算表５(１)
			SetInputItemSub((flg & 0x0010), CHEK_TO05, CHEK_TO05_BUS, CHEK_TO05_NUM, -1, -1, &m_PrnEXSub, &dd);
			break;

		case 6:		// 計算表５(２)
			SetInputItemSub((flg & 0x0020), CHEK_TO06, CHEK_TO06_BUS, CHEK_TO06_NUM, -1, -1, &m_PrnEXSub, &dd);
			break;

		case 7:		// 計算表５(３)
			SetInputItemSub((flg & 0x0040), CHEK_TO07, CHEK_TO07_BUS, CHEK_TO07_NUM, -1, -1, &m_PrnEXSub, &dd);
			break;

		case 8:		// 計算表５－２(１)
			st = ed = -1;
			if((flg & 0x0080))	{
				st = CHEK_TO08_ST;
				ed = CHEK_TO08_ED;
			}
			SetInputItemSub((flg & 0x0080), CHEK_TO08, CHEK_TO08_BUS, CHEK_TO08_NUM, st, ed, &m_PrnEXSub, &dd);
			break;

		case 9:		// 計算表５－２(２)
			st = ed = -1;
			if((flg & 0x0100))	{
				st = CHEK_TO09_ST;
				ed = CHEK_TO09_ED;
			}
			SetInputItemSub((flg & 0x0100), CHEK_TO09, CHEK_TO09_BUS, CHEK_TO09_NUM, st, ed, &m_PrnEXSub, &dd);
			break;

		case 10:	// 計算表５－２(３)－１
			st = ed = -1;
			if((flg & 0x0200))	{
				st = CHEK_TO10_ST;
				ed = CHEK_TO10_ED;
			}
			SetInputItemSub((flg & 0x0200), CHEK_TO10, CHEK_TO10_BUS, CHEK_TO10_NUM, st, ed, &m_PrnEXSub, &dd);
			break;

		case 11:	// 計算表５－２(３)－２
			st = ed = -1;
			if((flg & 0x0400))	{
				st = CHEK_TO11_ST;
				ed = CHEK_TO11_ED;
			}
			SetInputItemSub((flg & 0x0400), CHEK_TO11, CHEK_TO11_BUS, CHEK_TO11_NUM, st, ed, &m_PrnEXSub, &dd);
			break;

		case 12:	// 計算表５－２(３)－３
			st = ed = -1;
			if((flg & 0x0800))	{
				st = CHEK_TO12_ST;
				ed = CHEK_TO12_ED;
			}
			SetInputItemSub((flg & 0x0800), CHEK_TO12, CHEK_TO12_BUS, CHEK_TO12_NUM, st, ed, &m_PrnEXSub, &dd);
			break;

		case 13:	// 計算表５－２(４)－１
			st = ed = -1;
			if((flg & 0x1000))	{
				st = CHEK_TO13_ST;
				ed = CHEK_TO13_ED;
			}
			SetInputItemSub((flg & 0x1000), CHEK_TO13, CHEK_TO13_BUS, CHEK_TO13_NUM, st, ed, &m_PrnEXSub, &dd);
			break;

		case 14:	// 計算表５－２(４)－２
			st = ed = -1;
			if((flg & 0x2000))	{
				st = CHEK_TO14_ST;
				ed = CHEK_TO14_ED;
			}
			SetInputItemSub((flg & 0x2000), CHEK_TO14, CHEK_TO14_BUS, CHEK_TO14_NUM, st, ed, &m_PrnEXSub, &dd);
			break;

		case 15:	// 計算表５－２(４)－３
			st = ed = -1;
			if((flg & 0x4000))	{
				st = CHEK_TO15_ST;
				ed = CHEK_TO15_ED;
			}
			SetInputItemSub((flg & 0x4000), CHEK_TO15, CHEK_TO15_BUS, CHEK_TO15_NUM, st, ed, &m_PrnEXSub, &dd);
			break;
// 24/07/10_個別タブから出力 cor <--

		default:
			break;
		}
	}

	m_Diag_Tok.Refresh();
	
	return(0);
}

// -----------------------------------------------------------------------------
// 入力制限の設定
// -----------------------------------------------------------------------------
// 【引　数】	なし
// 【戻り値】	なし
// -----------------------------------------------------------------------------
int PrintTokuteiSub::SetEnableItem(int pSw)
{
	int					ii = 0;
	long				flg;				// 入力制限サイン

	CTaxationList31View	*pTaxlist31View = NULL;
	pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));
	
	// 入力制限のサインをセット
// 24/07/10_個別タブから出力 cor -->
	//pTaxlist31View->IsSpecificEnable(&flg);
// ---------------------------------
	flg = 0;
	// 「全表出力可」チェックON もしくは 特定収入計算表が出力できるデータ
	if((m_pPrintCmInfo->OP_PSEL & 0x02) || (!(m_pSnHeadData->Sn_Sign2 & 0x02)))	{
		pTaxlist31View->IsSpecificEnable(&flg);
	}
// 24/07/10_個別タブから出力 cor <--

//	if (pSw == -1)	ck1 = m_Check1.GetCheck();
//	else			ck1 = pSw;

	for (ii = 1; ii < TOK_MAXCOL + 1; ii++) {
		switch (ii) {
// 24/07/10_個別タブから出力 cor -->
		//case 1:		// 計算表１　入力制限
		//	SetColorItemSub(1, (flg & 0x0001), CHEK_TO01, CHEK_TO01_NUM, CHEK_TO01_ST, CHEK_TO01_ED, 0);
		//	break;

		//case 2:		// 計算表２(１)
		//	SetColorItemSub(1, (flg & 0x0002), CHEK_TO02, CHEK_TO02_NUM, CHEK_TO02_ST, CHEK_TO02_ED, 0);
		//	break;

		//case 3:		// 計算表２(２)
		//	SetColorItemSub(1, (flg & 0x0004), CHEK_TO03, CHEK_TO03_NUM, CHEK_TO03_ST, CHEK_TO03_ED, 0);
		//	break;

		//case 4:		// 計算表３、４
		//	SetColorItemSub(1, (flg & 0x0008), CHEK_TO04, CHEK_TO04_NUM, CHEK_TO04_ST, CHEK_TO04_ED, 0);
		//	break;

		//case 5:		// 計算表５(１)
		//	SetColorItemSub(1, (flg & 0x0010), CHEK_TO05, CHEK_TO05_NUM, CHEK_TO05_ST, CHEK_TO05_ED, 0);
		//	break;

		//case 6:		// 計算表５(２)
		//	SetColorItemSub(1, (flg & 0x0020), CHEK_TO06, CHEK_TO06_NUM, CHEK_TO06_ST, CHEK_TO06_ED, 0);
		//	break;

		//case 7:		// 計算表５(３)
		//	SetColorItemSub(1, (flg & 0x0040), CHEK_TO07, CHEK_TO07_NUM, CHEK_TO07_ST, CHEK_TO07_ED, 0);
		//	break;

		//case 8:		// 計算表５－２(１)
		//	SetColorItemSub(1, (flg & 0x0080), CHEK_TO08, CHEK_TO08_NUM, CHEK_TO08_ST, CHEK_TO08_ED, 1);
		//	break;

		//case 9:		// 計算表５－２(２)
		//	SetColorItemSub(1, (flg & 0x0100), CHEK_TO09, CHEK_TO09_NUM, CHEK_TO09_ST, CHEK_TO09_ED, 1);
		//	break;

		//case 10:	// 計算表５－２(３)－１
		//	SetColorItemSub(1, (flg & 0x0200), CHEK_TO10, CHEK_TO10_NUM, CHEK_TO10_ST, CHEK_TO10_ED, 1);
		//	break;

		//case 11:	// 計算表５－２(３)－２
		//	SetColorItemSub(1, (flg & 0x0400), CHEK_TO11, CHEK_TO11_NUM, CHEK_TO11_ST, CHEK_TO11_ED, 1);
		//	break;

		//case 12:	// 計算表５－２(３)－３
		//	SetColorItemSub(1, (flg & 0x0800), CHEK_TO12, CHEK_TO12_NUM, CHEK_TO12_ST, CHEK_TO12_ED, 1);
		//	break;

		//case 13:	// 計算表５－２(４)－１
		//	SetColorItemSub(1, (flg & 0x1000), CHEK_TO13, CHEK_TO13_NUM, CHEK_TO13_ST, CHEK_TO13_ED, 1);
		//	break;

		//case 14:	// 計算表５－２(４)－２
		//	SetColorItemSub(1, (flg & 0x2000), CHEK_TO14, CHEK_TO14_NUM, CHEK_TO14_ST, CHEK_TO14_ED, 1);
		//	break;

		//case 15:	// 計算表５－２(４)－３
		//	SetColorItemSub(1, (flg & 0x4000), CHEK_TO15, CHEK_TO15_NUM, CHEK_TO15_ST, CHEK_TO15_ED, 1);
		//	break;
// ---------------------------------
		case 1:		// 計算表１　入力制限
			SetColorItemSub(1, (flg & 0x0001), CHEK_TO01, CHEK_TO01_BUS, CHEK_TO01_NUM, CHEK_TO01_ST, CHEK_TO01_ED, 0);
			break;

		case 2:		// 計算表２(１)
			SetColorItemSub(1, (flg & 0x0002), CHEK_TO02, CHEK_TO02_BUS, CHEK_TO02_NUM, CHEK_TO02_ST, CHEK_TO02_ED, 0);
			break;

		case 3:		// 計算表２(２)
			SetColorItemSub(1, (flg & 0x0004), CHEK_TO03, CHEK_TO03_BUS, CHEK_TO03_NUM, CHEK_TO03_ST, CHEK_TO03_ED, 0);
			break;

		case 4:		// 計算表３、４
			SetColorItemSub(1, (flg & 0x0008), CHEK_TO04, CHEK_TO04_BUS, CHEK_TO04_NUM, CHEK_TO04_ST, CHEK_TO04_ED, 0);
			break;

		case 5:		// 計算表５(１)
			SetColorItemSub(1, (flg & 0x0010), CHEK_TO05, CHEK_TO05_BUS, CHEK_TO05_NUM, CHEK_TO05_ST, CHEK_TO05_ED, 0);
			break;

		case 6:		// 計算表５(２)
			SetColorItemSub(1, (flg & 0x0020), CHEK_TO06, CHEK_TO06_BUS, CHEK_TO06_NUM, CHEK_TO06_ST, CHEK_TO06_ED, 0);
			break;

		case 7:		// 計算表５(３)
			SetColorItemSub(1, (flg & 0x0040), CHEK_TO07, CHEK_TO07_BUS, CHEK_TO07_NUM, CHEK_TO07_ST, CHEK_TO07_ED, 0);
			break;

		case 8:		// 計算表５－２(１)
			SetColorItemSub(1, (flg & 0x0080), CHEK_TO08, CHEK_TO08_BUS, CHEK_TO08_NUM, CHEK_TO08_ST, CHEK_TO08_ED, 1);
			break;

		case 9:		// 計算表５－２(２)
			SetColorItemSub(1, (flg & 0x0100), CHEK_TO09, CHEK_TO09_BUS, CHEK_TO09_NUM, CHEK_TO09_ST, CHEK_TO09_ED, 1);
			break;

		case 10:	// 計算表５－２(３)－１
			SetColorItemSub(1, (flg & 0x0200), CHEK_TO10, CHEK_TO10_BUS, CHEK_TO10_NUM, CHEK_TO10_ST, CHEK_TO10_ED, 1);
			break;

		case 11:	// 計算表５－２(３)－２
			SetColorItemSub(1, (flg & 0x0400), CHEK_TO11, CHEK_TO11_BUS, CHEK_TO11_NUM, CHEK_TO11_ST, CHEK_TO11_ED, 1);
			break;

		case 12:	// 計算表５－２(３)－３
			SetColorItemSub(1, (flg & 0x0800), CHEK_TO12, CHEK_TO12_BUS, CHEK_TO12_NUM, CHEK_TO12_ST, CHEK_TO12_ED, 1);
			break;

		case 13:	// 計算表５－２(４)－１
			SetColorItemSub(1, (flg & 0x1000), CHEK_TO13, CHEK_TO13_BUS, CHEK_TO13_NUM, CHEK_TO13_ST, CHEK_TO13_ED, 1);
			break;

		case 14:	// 計算表５－２(４)－２
			SetColorItemSub(1, (flg & 0x2000), CHEK_TO14, CHEK_TO14_BUS, CHEK_TO14_NUM, CHEK_TO14_ST, CHEK_TO14_ED, 1);
			break;

		case 15:	// 計算表５－２(４)－３
			SetColorItemSub(1, (flg & 0x4000), CHEK_TO15, CHEK_TO15_BUS, CHEK_TO15_NUM, CHEK_TO15_ST, CHEK_TO15_ED, 1);
			break;
// 24/07/10_個別タブから出力 cor <--

		default:
			break;
		}
	}

	m_Diag_Tok.Refresh();
	
	return(0);
}

//-----------------------------------------------------------------------------
// 入力項目取得
//-----------------------------------------------------------------------------
// 引数	
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int PrintTokuteiSub::GetInputItemData(void)
{
	int				ii = 0;
	int				chek;
	int				busu;
	int				stpg, edpg;
	DIAGRAM_DATA	dd;
		
	for (ii = 1; ii < TOK_MAXCOL + 1; ii++) {
		// 初期化
		chek = 0;
		busu = NULL;
		stpg = edpg = NULL;

		switch (ii) {
		// マップデータの取得
		::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));
		m_pMapPrnEXSub.Lookup(ii, m_PrnEXSub);

// 24/07/10_個別タブから出力 cor -->
		//case 1:		// 計算表１
		//	GetInputItemDataSub(CHEK_TO01, CHEK_TO01_NUM, CHEK_TO01_ST, CHEK_TO01_ED, &m_PrnEXSub, &dd);
		//	break;

		//case 2:		// 計算表２(１)
		//	GetInputItemDataSub(CHEK_TO02, CHEK_TO02_NUM, CHEK_TO02_ST, CHEK_TO02_ED, &m_PrnEXSub, &dd);
		//	break;

		//case 3:		// 計算表２(２)
		//	GetInputItemDataSub(CHEK_TO03, CHEK_TO03_NUM, CHEK_TO03_ST, CHEK_TO03_ED, &m_PrnEXSub, &dd);
		//	break;

		//case 4:		// 計算表３、４
		//	GetInputItemDataSub(CHEK_TO04, CHEK_TO04_NUM, CHEK_TO04_ST, CHEK_TO04_ED, &m_PrnEXSub, &dd);
		//	break;

		//case 5:		// 計算表５(１)
		//	GetInputItemDataSub(CHEK_TO05, CHEK_TO05_NUM, CHEK_TO05_ST, CHEK_TO05_ED, &m_PrnEXSub, &dd);
		//	break;

		//case 6:		// 計算表５(２)
		//	GetInputItemDataSub(CHEK_TO06, CHEK_TO06_NUM, CHEK_TO06_ST, CHEK_TO06_ED, &m_PrnEXSub, &dd);
		//	break;

		//case 7:		// 計算表５(３)
		//	GetInputItemDataSub(CHEK_TO07, CHEK_TO07_NUM, CHEK_TO07_ST, CHEK_TO07_ED, &m_PrnEXSub, &dd);
		//	break;

		//case 8:		// 計算表５－２(１)
		//	GetInputItemDataSub(CHEK_TO08, CHEK_TO08_NUM, CHEK_TO08_ST, CHEK_TO08_ED, &m_PrnEXSub, &dd);
		//	break;

		//case 9:		// 計算表５－２(２)
		//	GetInputItemDataSub(CHEK_TO09, CHEK_TO09_NUM, CHEK_TO09_ST, CHEK_TO09_ED, &m_PrnEXSub, &dd);
		//	break;

		//case 10:	// 計算表５－２(３)－１
		//	GetInputItemDataSub(CHEK_TO10, CHEK_TO10_NUM, CHEK_TO10_ST, CHEK_TO10_ED, &m_PrnEXSub, &dd);
		//	break;

		//case 11:	// 計算表５－２(３)－２
		//	GetInputItemDataSub(CHEK_TO11, CHEK_TO11_NUM, CHEK_TO11_ST, CHEK_TO11_ED, &m_PrnEXSub, &dd);
		//	break;

		//case 12:	// 計算表５－２(３)－３
		//	GetInputItemDataSub(CHEK_TO12, CHEK_TO12_NUM, CHEK_TO12_ST, CHEK_TO12_ED, &m_PrnEXSub, &dd);
		//	break;

		//case 13:	// 計算表５－２(４)－１
		//	GetInputItemDataSub(CHEK_TO13, CHEK_TO13_NUM, CHEK_TO13_ST, CHEK_TO13_ED, &m_PrnEXSub, &dd);
		//	break;

		//case 14:	// 計算表５－２(４)－２
		//	GetInputItemDataSub(CHEK_TO14, CHEK_TO14_NUM, CHEK_TO14_ST, CHEK_TO14_ED, &m_PrnEXSub, &dd);
		//	break;

		//case 15:	// 計算表５－２(４)－３
		//	GetInputItemDataSub(CHEK_TO15, CHEK_TO15_NUM, CHEK_TO15_ST, CHEK_TO15_ED, &m_PrnEXSub, &dd);
		//	break;
// ---------------------------------
		case 1:		// 計算表１
			GetInputItemDataSub(CHEK_TO01, CHEK_TO01_BUS, CHEK_TO01_NUM, CHEK_TO01_ST, CHEK_TO01_ED, &m_PrnEXSub, &dd);
			break;

		case 2:		// 計算表２(１)
			GetInputItemDataSub(CHEK_TO02, CHEK_TO02_BUS, CHEK_TO02_NUM, CHEK_TO02_ST, CHEK_TO02_ED, &m_PrnEXSub, &dd);
			break;

		case 3:		// 計算表２(２)
			GetInputItemDataSub(CHEK_TO03, CHEK_TO03_BUS, CHEK_TO03_NUM, CHEK_TO03_ST, CHEK_TO03_ED, &m_PrnEXSub, &dd);
			break;

		case 4:		// 計算表３、４
			GetInputItemDataSub(CHEK_TO04, CHEK_TO04_BUS, CHEK_TO04_NUM, CHEK_TO04_ST, CHEK_TO04_ED, &m_PrnEXSub, &dd);
			break;

		case 5:		// 計算表５(１)
			GetInputItemDataSub(CHEK_TO05, CHEK_TO05_BUS, CHEK_TO05_NUM, CHEK_TO05_ST, CHEK_TO05_ED, &m_PrnEXSub, &dd);
			break;

		case 6:		// 計算表５(２)
			GetInputItemDataSub(CHEK_TO06, CHEK_TO06_BUS, CHEK_TO06_NUM, CHEK_TO06_ST, CHEK_TO06_ED, &m_PrnEXSub, &dd);
			break;

		case 7:		// 計算表５(３)
			GetInputItemDataSub(CHEK_TO07, CHEK_TO07_BUS, CHEK_TO07_NUM, CHEK_TO07_ST, CHEK_TO07_ED, &m_PrnEXSub, &dd);
			break;

		case 8:		// 計算表５－２(１)
			GetInputItemDataSub(CHEK_TO08, CHEK_TO08_BUS, CHEK_TO08_NUM, CHEK_TO08_ST, CHEK_TO08_ED, &m_PrnEXSub, &dd);
			break;

		case 9:		// 計算表５－２(２)
			GetInputItemDataSub(CHEK_TO09, CHEK_TO09_BUS, CHEK_TO09_NUM, CHEK_TO09_ST, CHEK_TO09_ED, &m_PrnEXSub, &dd);
			break;

		case 10:	// 計算表５－２(３)－１
			GetInputItemDataSub(CHEK_TO10, CHEK_TO10_BUS, CHEK_TO10_NUM, CHEK_TO10_ST, CHEK_TO10_ED, &m_PrnEXSub, &dd);
			break;

		case 11:	// 計算表５－２(３)－２
			GetInputItemDataSub(CHEK_TO11, CHEK_TO11_BUS, CHEK_TO11_NUM, CHEK_TO11_ST, CHEK_TO11_ED, &m_PrnEXSub, &dd);
			break;

		case 12:	// 計算表５－２(３)－３
			GetInputItemDataSub(CHEK_TO12, CHEK_TO12_BUS, CHEK_TO12_NUM, CHEK_TO12_ST, CHEK_TO12_ED, &m_PrnEXSub, &dd);
			break;

		case 13:	// 計算表５－２(４)－１
			GetInputItemDataSub(CHEK_TO13, CHEK_TO13_BUS, CHEK_TO13_NUM, CHEK_TO13_ST, CHEK_TO13_ED, &m_PrnEXSub, &dd);
			break;

		case 14:	// 計算表５－２(４)－２
			GetInputItemDataSub(CHEK_TO14, CHEK_TO14_BUS, CHEK_TO14_NUM, CHEK_TO14_ST, CHEK_TO14_ED, &m_PrnEXSub, &dd);
			break;

		case 15:	// 計算表５－２(４)－３
			GetInputItemDataSub(CHEK_TO15, CHEK_TO15_BUS, CHEK_TO15_NUM, CHEK_TO15_ST, CHEK_TO15_ED, &m_PrnEXSub, &dd);
			break;
// 24/07/10_個別タブから出力 cor <--

		default:
			break;
		}
		// マップデータの更新
		m_pMapPrnEXSub.SetAt(ii, m_PrnEXSub);
	}
	
	return(0);
}

HBRUSH PrintTokuteiSub::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ここで DC の属性を変更してください。
// 24/07/10_個別タブから出力 add -->
	if(GetDlgItem(IDC_STATIC_INCOME) == pWnd)	pDC->SetTextColor(RGB(255,0,0));
// 24/07/10_個別タブから出力 add <--

	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


//-----------------------------------------------------------------------------
// 入力項目色変更
//-----------------------------------------------------------------------------
// 引数	id		：
//		index	：
//		sign	：
//-----------------------------------------------------------------------------
void PrintTokuteiSub::ChangeColor(unsigned short id, short index, int sign)
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr(id, index, &DA, CIcsdiagctrl);
	switch (sign) {
	case 1:
		DA.attr_bcolor = BC_WHITE;
		break;		// 入力不可	（白色）
	case 2:
		DA.attr_bcolor = BC_GRAY;
		break;		// ロック	（灰色）
	case 3:
		DA.attr_bcolor = BC_BLUE;
		break;		// 特殊色①	（青色）
	case 4:
		DA.attr_bcolor = BC_GREEN;
		break;		// 特殊色②	（緑色）	
	case 5:
		DA.attr_bcolor = BC_GREEN_L;
		break;		// 特殊色③	（薄緑色）	 
// 修正No.168720 add -->
	case 6:
		DA.attr_bcolor = BC_CYAN;
		break;		// 特殊色④	（水色）
// 修正No.168720 add <--
	default:
		DA.attr_bcolor = BC_CREAM;
		break;		// 実額入力	（クリーム色） 
	}
	diag_setattr(id, index, &DA, FALSE, CIcsdiagctrl);
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void PrintTokuteiSub::EditOFFPrnTokuteiSub(short index)
{
	int				edpg;
// 24/03/21_印刷枚数 add -->
	int				chk=0,pos=0;
// 24/03/21_印刷枚数 add <--
// 24/07/10_個別タブから出力 add -->
	int				idx=0;
	int				ii=0;
// 24/07/10_個別タブから出力 add <--
	DIAGRAM_DATA	dd, dd2;

	// 終了頁を取得
	edpg = 999;
	CTaxationList31View	*pTaxlist31View = NULL;
	pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));
	edpg = pTaxlist31View->GetSpecListMaxPage();

	dd.data_imgdata = NULL;
	m_Diag_Tok.GetData(index, (LPUNKNOWN)&dd);

	switch (index) {

// 24/03/21_印刷枚数 del -->
	//	// 部数
	//case CHEK_TO01_NUM:			// 計画表１
	//case CHEK_TO02_NUM:			// 計画表２(１)
	//case CHEK_TO03_NUM:			// 計画表２(２)
	//case CHEK_TO04_NUM:			// 計画表３、４
	//case CHEK_TO05_NUM:			// 計画表５(１)
	//case CHEK_TO06_NUM:			// 計画表５(２)
	//case CHEK_TO07_NUM:			// 計画表５(３)
	//case CHEK_TO08_NUM:			// 計画表５－２(１)
	//case CHEK_TO09_NUM:			// 計画表５－２(２)
	//case CHEK_TO10_NUM:			// 計画表５－２(３)－１
	//case CHEK_TO11_NUM:			// 計画表５－２(３)－２
	//case CHEK_TO12_NUM:			// 計画表５－２(３)－３
	//case CHEK_TO13_NUM:			// 計画表５－２(４)－１
	//case CHEK_TO14_NUM:			// 計画表５－２(４)－２
	//case CHEK_TO15_NUM:			// 計画表５－２(４)－３
	//	// 1-99以外は受け付けないようにする
	//	if (_tstoi(dd.data_edit) < 1 || _tstoi(dd.data_edit) > 99) {
	//		dd.data_edit = _T("1");
	//		m_Diag_Tok.SetData(index, (LPUNKNOWN)&dd);
	//	}
	//	break;
// 24/03/21_印刷枚数 del <--

// 24/07/10_個別タブから出力 add -->
	// 部数
	case CHEK_TO01_BUS:			// 計画表１
	case CHEK_TO02_BUS:			// 計画表２(１)
	case CHEK_TO03_BUS:			// 計画表２(２)
	case CHEK_TO04_BUS:			// 計画表３、４
	case CHEK_TO05_BUS:			// 計画表５(１)
	case CHEK_TO06_BUS:			// 計画表５(２)
	case CHEK_TO07_BUS:			// 計画表５(３)
	case CHEK_TO08_BUS:			// 計画表５－２(１)
	case CHEK_TO09_BUS:			// 計画表５－２(２)
	case CHEK_TO10_BUS:			// 計画表５－２(３)－１
	case CHEK_TO11_BUS:			// 計画表５－２(３)－２
	case CHEK_TO12_BUS:			// 計画表５－２(３)－３
	case CHEK_TO13_BUS:			// 計画表５－２(４)－１
	case CHEK_TO14_BUS:			// 計画表５－２(４)－２
	case CHEK_TO15_BUS:			// 計画表５－２(４)－３
		// 1-99以外は受け付けないようにする
		if (_tstoi(dd.data_edit) < 1 || _tstoi(dd.data_edit) > 99)	{
			dd.data_edit = _T("1");
			m_Diag_Tok.SetData(index, (LPUNKNOWN)&dd);
		}
		break;
// 24/07/10_個別タブから出力 add <--

	// 開始頁
	case CHEK_TO01_ST:			// 計画表１
	case CHEK_TO02_ST:			// 計画表２(１)
	case CHEK_TO03_ST:			// 計画表２(２)
	case CHEK_TO04_ST:			// 計画表３、４
	case CHEK_TO05_ST:			// 計画表５(１)
	case CHEK_TO06_ST:			// 計画表５(２)
	case CHEK_TO07_ST:			// 計画表５(３)
	case CHEK_TO08_ST:			// 計画表５－２(１)
	case CHEK_TO09_ST:			// 計画表５－２(２)
	case CHEK_TO10_ST:			// 計画表５－２(３)－１
	case CHEK_TO11_ST:			// 計画表５－２(３)－２
	case CHEK_TO12_ST:			// 計画表５－２(３)－３
	case CHEK_TO13_ST:			// 計画表５－２(４)－１
	case CHEK_TO14_ST:			// 計画表５－２(４)－２
	case CHEK_TO15_ST:			// 計画表５－２(４)－３
		// 1-999以外は受け付けないようにする
		if (_tstoi(dd.data_edit) < 1 || _tstoi(dd.data_edit) > 999) {
			dd.data_edit = _T("1");
			m_Diag_Tok.SetData(index, (LPUNKNOWN)&dd);
		}
		// 最終ページを超えないようにする
		if (_tstoi(dd.data_edit) > edpg) {
			dd.data_edit = _T("1");
			m_Diag_Tok.SetData(index, (LPUNKNOWN)&dd);
		}
// 24/03/21_印刷枚数 add -->
		m_Diag_Tok.GetData(index-2,(LPUNKNOWN)&dd);
		chk = dd.data_check;
		pos = (index-2)/4-1;
		GetBunbo(pos);
		dd.data_imgdata = NULL;
		m_Diag_Tok.GetData(index-1,(LPUNKNOWN)&dd);
		if(chk)	dd.data_edit.Format(_T("%d/%d"),m_Bunsi[pos],m_Bunbo[pos]);
		else	dd.data_edit.Format(_T("0/%d"),m_Bunbo[pos]);
		m_Diag_Tok.SetData(index-1,(LPUNKNOWN)&dd);
// 24/03/21_印刷枚数 add <--
		break;

	// 終了頁
	case CHEK_TO01_ED:			// 計画表１
	case CHEK_TO02_ED:			// 計画表２(１)
	case CHEK_TO03_ED:			// 計画表２(２)
	case CHEK_TO04_ED:			// 計画表３、４
	case CHEK_TO05_ED:			// 計画表５(１)
	case CHEK_TO06_ED:			// 計画表５(２)
	case CHEK_TO07_ED:			// 計画表５(３)
	case CHEK_TO08_ED:			// 計画表５－２(１)
	case CHEK_TO09_ED:			// 計画表５－２(２)
	case CHEK_TO10_ED:			// 計画表５－２(３)－１
	case CHEK_TO11_ED:			// 計画表５－２(３)－２
	case CHEK_TO12_ED:			// 計画表５－２(３)－３
	case CHEK_TO13_ED:			// 計画表５－２(４)－１
	case CHEK_TO14_ED:			// 計画表５－２(４)－２
	case CHEK_TO15_ED:			// 計画表５－２(４)－３
		// 1-999以外は受け付けないようにする
		if (_tstoi(dd.data_edit) < 1 || _tstoi(dd.data_edit) > 999) {
			dd.data_edit.Format(_T("%d"), edpg);
			m_Diag_Tok.SetData(index, (LPUNKNOWN)&dd);
		}
		// 最終ページを超えないようにする
		if (_tstoi(dd.data_edit) > edpg) {
			dd.data_edit.Format(_T("%d"), edpg);
			m_Diag_Tok.SetData(index, (LPUNKNOWN)&dd);
		}
		// 開始頁に入力された数値を超えないようにする
		//m_Diag_Tok.GetData(index - 16, (LPUNKNOWN)&dd2);
		m_Diag_Tok.GetData(index - 1, (LPUNKNOWN)&dd2);
		if (_tstoi(dd.data_edit) < _tstoi(dd2.data_edit)) {
			dd.data_edit.Format(_T("%d"), edpg);
			m_Diag_Tok.SetData(index, (LPUNKNOWN)&dd);
		}
// 24/03/21_印刷枚数 add -->
		m_Diag_Tok.GetData(index-3,(LPUNKNOWN)&dd);
		chk = dd.data_check;
		//pos = (index-2)/4-1;	// 24/07/10_個別タブから出力 del
		pos = (index-3)/4-1;	// 24/07/10_個別タブから出力 add
		GetBunbo(pos);
		dd.data_imgdata = NULL;
		m_Diag_Tok.GetData(index-2,(LPUNKNOWN)&dd);
		if(chk)	dd.data_edit.Format(_T("%d/%d"),m_Bunsi[pos],m_Bunbo[pos]);
		else	dd.data_edit.Format(_T("0/%d"),m_Bunbo[pos]);
		m_Diag_Tok.SetData(index-2,(LPUNKNOWN)&dd);
// 24/03/21_印刷枚数 add <--
		break;

	default:
		break;
	}
	
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void PrintTokuteiSub::TerminationPrnTokuteiSub(short index, short nChar, short length, LPUNKNOWN data)
{
	int					xx = 0, yy = 0;
	short				sw = 0;
	short				wTerm = 0;
	short				npos = -1;
	char				VK_FLG = 0;
	DIAGRAM_ATTRIBUTE	da;
	
	if (nChar == 0) {
		return;
	}

	VK_FLG = (char)::GetKeyState(VK_SHIFT);
	if (nChar == VK_TAB) {
		if (VK_FLG & 0x80)		wTerm = VK_F2;
		else					wTerm = VK_RETURN;
	}
	else						wTerm = nChar;

	// ポジションの2次元配列から現在の場所を探す
	sw = 0;
	for (xx = 0; xx < TOK_MAXROW; xx++) {
		for (yy = 0; yy < TOK_MAXCOL; yy++) {
			if (CursorPosTOK[yy][xx] == index) {
				sw = 1;
				break;
			}
		}
		if (sw != 0)	break;
	}
	if (sw == 0) {
		return;	// ありえない
	}

	switch (wTerm) {
	case VK_RETURN:
	case VK_F2:
		while (1) {
			if (wTerm == VK_RETURN) {
				xx++;
				if (xx > TOK_MAXROW) {
					xx = 0;
					yy++;
				}
				if (yy > TOK_MAXCOL) {
					yy = 0;
				}
			}
			else if (wTerm == VK_F2) {
				xx--;
				if (xx < 0) {
					//xx = VK_RETURN-1;		// 修正No.168721 del
					xx = TOK_MAXROW-1;		// 修正No.168721 del
					yy--;
				}
				if (yy < 0) {
					yy = TOK_MAXCOL;
				}
			}
			if (CursorPosTOK[yy][xx] == -1) {
				continue;
			}
			diag_getattr(IDC_PRN_TOKUTEI_SUB_DG, CursorPosTOK[yy][xx], &da, CIcsdiagctrl);
			//if(da.attr_bcolor == BC_GRAY) {								// 修正No.168720 del
			if(da.attr_bcolor == BC_GRAY || da.attr_bcolor == BC_CYAN)	{	// 修正No.168720 add
				continue;
			}
			if (da.attr_bcolor == BC_WHITE) {
				npos = CursorPosTOK[yy][xx];
				break;
			}
		}
		break;

	case VK_RIGHT:
	case VK_LEFT:
		while (1) {
// 修正No.168721 cor -->
			//if(wTerm == VK_RIGHT)		xx++;
			//else if(wTerm == VK_LEFT)	xx--;
// ---------------------
			if(wTerm == VK_RIGHT)	{
// 24/07/10_個別タブから出力 cor -->
				//// 今いる場所がチェックボックスなら+2
				//if((index % TOK_MAXROW) == 0)	xx += 2;
// ---------------------------------
				// 今いる場所が「部数」なら+2
				if(CursorPosTOK[yy][1] == index)	xx += 2;
// 24/07/10_個別タブから出力 cor <--
				else								xx++;
			}
			else if(wTerm == VK_LEFT)	{
				// 今いる場所が「開始頁」なら-2
// 24/07/10_個別タブから出力 cor -->
				//if((index % TOK_MAXROW) == 2)	xx -= 2;
// ---------------------------------
				if(CursorPosTOK[yy][3] == index)	xx -= 2;
// 24/07/10_個別タブから出力 cor <--
				else								xx--;
			}
// 修正No.168721 cor <--

			if(xx < 0)					xx = 0;

			if(CursorPosTOK[yy][xx] == -1)	{
				break;
			}
			diag_getattr(IDC_PRN_TOKUTEI_SUB_DG, CursorPosTOK[yy][xx], &da, CIcsdiagctrl);
			//if(da.attr_bcolor == BC_GRAY) {								// 修正No.168720 del
			if(da.attr_bcolor == BC_GRAY || da.attr_bcolor == BC_CYAN)	{	// 修正No.168720 add
				continue;
			}
			if(da.attr_bcolor == BC_WHITE) {
				npos = CursorPosTOK[yy][xx];
				break;
			}
		}
		break;

	case VK_DOWN:
	case VK_UP:
		while (1) {
			if(wTerm == VK_DOWN)	yy++;
			else if(wTerm == VK_UP)	yy--;

// 修正No.168895 cor -->
			//if(yy < 0)	yy = 0;
// ---------------------
			if(yy < m_Yy)	yy = m_Yy;
// 修正No.168895 cor <--

			if(CursorPosTOK[yy][xx] == -1) {
				break;
			}
			diag_getattr(IDC_PRN_TOKUTEI_SUB_DG, CursorPosTOK[yy][xx], &da, CIcsdiagctrl);
			//if(da.attr_bcolor == BC_GRAY) {								// 修正No.168720 del
			if(da.attr_bcolor == BC_GRAY || da.attr_bcolor == BC_CYAN)	{	// 修正No.168720 add
				continue;
			}
			if(da.attr_bcolor == BC_WHITE) {
				npos = CursorPosTOK[yy][xx];
				break;
			}
		}
		break;
	}

	if (npos != -1) {
		//// 最終のインデックスでEnterキーを押下した場合
		//if (wTerm == VK_RETURN && npos == CHEK_TO01) {
		//	// OKボタンへ移動
		//	GetDlgItem(IDOK)->SetFocus();
		//}
		//else {
			m_Diag_Tok.SetPosition(npos);
		//}
	}
	
}

//op D0:頁があるかないか
// 24/07/10_個別タブから出力 cor -->
//int PrintTokuteiSub::SetInputItemSub(int CtIDCh, int CtIDNum, int CtIDSp, int CtIDEp, PRNEXSUB_INF *SubInf, DIAGRAM_DATA *pdd)
// ---------------------------------
int PrintTokuteiSub::SetInputItemSub(int nflg, int CtIDCh, int CtIDBus, int CtIDNum, int CtIDSp, int CtIDEp, PRNEXSUB_INF *SubInf, DIAGRAM_DATA *pdd)
// 24/07/10_個別タブから出力 cor <--
{
	int set_num = 0;
	int pos = 0;		// 24/07/10_個別タブから出力 add

	// データをセット
	pdd->data_imgdata = NULL;
	// チェックON/OFF
// 24/03/21_168685 cor -->
	//if (SubInf->check == 0)	pdd->data_check = 0;
// -----------------------
	DIAGRAM_ATTRIBUTE	da;
	diag_getattr(IDC_PRN_TOKUTEI_SUB_DG,CtIDCh,&da,CIcsdiagctrl);
	if (SubInf->check == 0 || da.attr_bcolor == BC_GRAY )	{
		pdd->data_check = 0;
	}
// 24/03/21_168685 cor <--
	else {
		pdd->data_check = 1;
		set_num = 1;
	}
	m_Diag_Tok.SetData(CtIDCh, (LPUNKNOWN)pdd);

// 24/03/21_印刷枚数 del -->
	//// 部数
	//pdd->data_check = 0;
	//if (set_num)	pdd->data_edit.Format(_T("%d/1"), SubInf->busu);
	//else			pdd->data_edit.Format(_T("0/1"));
	//m_Diag_Tok.SetData(CtIDNum, (LPUNKNOWN)pdd);
// 24/03/21_印刷枚数 del <--

// 24/07/10_個別タブから出力 add -->
	pdd->data_check = 0;

	// 印刷部数の取得
	GetBunbo(CtIDCh/4-1);
	pos = CtIDCh / 4 - 1;
	// 部数
	//if(m_Bunbo[pos] == 0)	pdd->data_edit.Format(_T("1"));
	//else					pdd->data_edit.Format(_T("%d"), SubInf->busu);
	if(nflg)	pdd->data_edit.Format(_T("%d"), SubInf->busu);
	else		pdd->data_edit.Format(_T("1"));
	m_Diag_Tok.SetData(CtIDBus, (LPUNKNOWN)pdd);
// 24/07/10_個別タブから出力 add <--

	if ((CtIDSp != -1) && (CtIDEp != -1)) {
		// 開始頁
		pdd->data_edit.Format(_T("%d"), SubInf->stpage);
		m_Diag_Tok.SetData(CtIDSp, (LPUNKNOWN)pdd);
		// 終了頁
		pdd->data_edit.Format(_T("%d"), SubInf->edpage);
		m_Diag_Tok.SetData(CtIDEp, (LPUNKNOWN)pdd);
	}
// 24/03/21_印刷枚数 add -->
	// 印刷枚数
// 24/07/10_個別タブから出力 cor -->
	//GetBunbo(CtIDCh/4-1);
	//pdd->data_check = 0;
	//int pos=CtIDCh/4-1;
// ---------------------------------
	pdd->data_check = 0;
	pos = CtIDCh / 4 - 1;
// 24/07/10_個別タブから出力 cor <--
	if(set_num)	pdd->data_edit.Format(_T("%d/%d"),m_Bunsi[pos],m_Bunbo[pos]);
	else		pdd->data_edit.Format(_T("0/%d"),m_Bunbo[pos]);
	m_Diag_Tok.SetData(CtIDNum, (LPUNKNOWN)pdd);
// 24/03/21_印刷枚数 add <--

	return 0;
}



//nflgはマスクしたものを渡す
//op  D0:入力可
// 24/07/10_個別タブから出力 cor -->
//int PrintTokuteiSub::SetColorItemSub(int ck, int nflg, int CtIDCh, int CtIDNum, int CtIDSp, int CtIDEp, int op)
// ---------------------------------
int PrintTokuteiSub::SetColorItemSub(int ck, int nflg, int CtIDCh, int CtIDBus, int CtIDNum, int CtIDSp, int CtIDEp, int op)
// 24/07/10_個別タブから出力 cor <--
{
	// 入力制限
	if (ck && nflg) {
		// 入力可
		// チェック
		ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, CtIDCh, 1);
		m_Diag_Tok.ModifyItem(CtIDCh, DIAG_MDFY_EDIT);
// 24/07/10_個別タブから出力 add -->
		// 部数
		ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, CtIDBus, 1);
		m_Diag_Tok.ModifyItem(CtIDBus, DIAG_MDFY_EDIT);
// 24/07/10_個別タブから出力 add <--
	}
	else {
		// 入力不可
		// チェック
		ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, CtIDCh, 2);
		m_Diag_Tok.ModifyItem(CtIDCh, DIAG_MDFY_READONLY);
// 24/07/10_個別タブから出力 add -->
		// 部数
		ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, CtIDBus, 2);
		m_Diag_Tok.ModifyItem(CtIDBus, DIAG_MDFY_READONLY);
// 24/07/10_個別タブから出力 add <--
	}

	// 印刷枚数　入力不可
	//ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, CtIDNum, 2);	// 修正No.168720 del
	ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, CtIDNum, 6);	// 修正No.168720 add
	m_Diag_Tok.ModifyItem(CtIDNum, DIAG_MDFY_READONLY);

	if (ck && nflg && (op & 0x01)) {
		// 開始頁 入力不可
		ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, CtIDSp, 1);
		m_Diag_Tok.ModifyItem(CtIDSp, DIAG_MDFY_EDIT);
		// 終了頁 入力不可
		ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, CtIDEp, 1);
		m_Diag_Tok.ModifyItem(CtIDEp, DIAG_MDFY_EDIT);

	}
	else {
		// 開始頁 入力不可
		ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, CtIDSp, 2);
		m_Diag_Tok.ModifyItem(CtIDSp, DIAG_MDFY_READONLY);
		// 終了頁 入力不可
		ChangeColor(IDC_PRN_TOKUTEI_SUB_DG, CtIDEp, 2);
		m_Diag_Tok.ModifyItem(CtIDEp, DIAG_MDFY_READONLY);
	}

	return 0;
}

// 24/07/10_個別タブから出力 cor -->
//int PrintTokuteiSub::GetInputItemDataSub(int CtIDCh, int CtIDNum, int CtIDSp, int CtIDEp, PRNEXSUB_INF *SubInf, DIAGRAM_DATA	*pdd)
//{
//	int		chek = 0;
//	int		busu = 0;
//	int		stpg = 0, edpg = 0;
//
//	pdd->data_imgdata = NULL;
//	// チェック
//	m_Diag_Tok.GetData(CtIDCh, (LPUNKNOWN)pdd);
//	chek = pdd->data_check;
//// 24/03/21_税務代理書面 del -->
//	//// 部数
//	//m_Diag_Tok.GetData(CtIDNum, (LPUNKNOWN)pdd);
//	//// 1-99以外は受け付けないようにする
//	//if (_tstoi(pdd->data_edit) >= 1 && _tstoi(pdd->data_edit) <= 99) {
//	//	busu = _tstoi(pdd->data_edit);
//	//}
//// 24/03/21_税務代理書面 del <--
//	// 開始頁
//	m_Diag_Tok.GetData(CtIDSp, (LPUNKNOWN)pdd);
//	// 1-999以外は受け付けないようにする
//	if (_tstoi(pdd->data_edit) >= 1 && _tstoi(pdd->data_edit) <= 999) {
//		stpg = _tstoi(pdd->data_edit);
//	}
//	// 終了頁
//	m_Diag_Tok.GetData(CtIDEp, (LPUNKNOWN)pdd);
//	// 1-999以外は受け付けないようにする
//	if (_tstoi(pdd->data_edit) >= 1 && _tstoi(pdd->data_edit) <= 999) {
//		edpg = _tstoi(pdd->data_edit);
//	}
//
//	::ZeroMemory(SubInf, sizeof(PRNEXSUB_INF));
//
//	SubInf->check = chek;
//// 24/03/21_税務代理書面 del -->
//	//SubInf->busu = busu;
//// 24/03/21_税務代理書面 del <--
//	SubInf->stpage = stpg;
//	SubInf->edpage = edpg;
//
//	return 0;
//}
// ---------------------------------
int PrintTokuteiSub::GetInputItemDataSub(int CtIDCh, int CtIDBus, int CtIDNum, int CtIDSp, int CtIDEp, PRNEXSUB_INF *SubInf, DIAGRAM_DATA *pdd)
{
	int		chek = 0;
	int		busu = 0;
	int		stpg = 0, edpg = 0;

	pdd->data_imgdata = NULL;
	// チェック
	m_Diag_Tok.GetData(CtIDCh, (LPUNKNOWN)pdd);
	chek = pdd->data_check;

	// 部数
	m_Diag_Tok.GetData(CtIDBus, (LPUNKNOWN)pdd);
	// 1-99以外は受け付けないようにする
	if(_tstoi(pdd->data_edit) >= 1 && _tstoi(pdd->data_edit) <= 99)	{
		busu = _tstoi(pdd->data_edit);
	}

	// 開始頁
	m_Diag_Tok.GetData(CtIDSp, (LPUNKNOWN)pdd);
	// 1-999以外は受け付けないようにする
	if(_tstoi(pdd->data_edit) >= 1 && _tstoi(pdd->data_edit) <= 999)	{
		stpg = _tstoi(pdd->data_edit);
	}

	// 終了頁
	m_Diag_Tok.GetData(CtIDEp, (LPUNKNOWN)pdd);
	// 1-999以外は受け付けないようにする
	if(_tstoi(pdd->data_edit) >= 1 && _tstoi(pdd->data_edit) <= 999)	{
		edpg = _tstoi(pdd->data_edit);
	}

	::ZeroMemory(SubInf, sizeof(PRNEXSUB_INF));

	SubInf->check = chek;
	SubInf->busu = busu;
	SubInf->stpage = stpg;
	SubInf->edpage = edpg;

	return 0;
}
// 24/07/10_個別タブから出力 cor <--

//-----------------------------------------------------------------------------
// 全選択＆全解除
//-----------------------------------------------------------------------------
// 【引　数】	int		0:全選択＆全解除　1:チェックOFFが1箇所でもあるか調べる
// 【戻り値】	BOOL	チェックOFFが1箇所でもあればFALSE
//-----------------------------------------------------------------------------
BOOL PrintTokuteiSub::ChoiceItem(int pSw)
{
	BOOL			brv = TRUE;
	SH_ITEMINDEX	*pPrnTbl;
	short			idx = 0;

	if (pSw == 0) {
		// フォーカスOFF
		idx = ((CIcsdiagctrl*)GetDlgItem(IDC_PRN_TOKUTEI_SUB_DG))->GetPosition();
		diag_deledit(IDC_PRN_TOKUTEI_SUB_DG, CIcsdiagctrl);
	}


	int chek = 0;
	DIAGRAM_DATA	dd;
	if (pSw == 1) {
		for (int ii = 0; ii < TOK_MAXCOL + 1; ii++) {
			int  index = CursorPosTOK[ii][0];
			if (index == -1) break;
			m_Diag_Tok.GetData(index, (LPUNKNOWN)&dd);
			//入力不可を見る
			DIAGRAM_ATTRIBUTE	da;
			diag_getattr(IDC_PRN_TOKUTEI_SUB_DG, CursorPosTOK[ii][0], &da, CIcsdiagctrl);
			if (da.attr_bcolor == BC_GRAY) {
				continue;
			}
			if (!dd.data_check) return FALSE;
		}

		return TRUE;
	}
	else {
		//0:全選択＆全解除
		BOOL Itemflg = ChoiceItem(1);

		for (int ii = 0; ii < TOK_MAXCOL + 1; ii++) {
			dd.data_imgdata = NULL;
			dd.data_check = 0;
			dd.data_combo = 0;
			int  index = CursorPosTOK[ii][0];
			if (index == -1) break;
			m_Diag_Tok.GetData(index, (LPUNKNOWN)&dd);
			//入力不可を見る
			DIAGRAM_ATTRIBUTE	da;
			diag_getattr(IDC_PRN_TOKUTEI_SUB_DG, CursorPosTOK[ii][0], &da, CIcsdiagctrl);
			if (da.attr_bcolor == BC_GRAY) {
				continue;
			}

			if (Itemflg) 	dd.data_check = 0;	//消す
			else			dd.data_check = 1; // 付ける
			m_Diag_Tok.SetData(index, (LPUNKNOWN)&dd);
		}
		m_Diag_Tok.Refresh();
	}
	return(TRUE);
}


void PrintTokuteiSub::OnBnClickedTokuteicode()
{
	//m_PoutKcode = m_Check_Tok.GetCheck();
	//if (m_PoutKcode) m_Ctlsw |= 0x01;
	//else		     m_Ctlsw &= ~0x01;

	if(m_pPrintCmInfo->OPL)	m_pPrintCmInfo->OPL = 0;
	else					m_pPrintCmInfo->OPL = 1;
}


void PrintTokuteiSub::CheckButtonPrnTokuteiSubDg(short index, LPUNKNOWN data)
{
	DIAGRAM_DATA *pdata = (DIAGRAM_DATA*)data;
	DIAGRAM_DATA setdata;
	int getflg = 0;
// 24/03/21_税務代理書面 add -->
	int pos=index/4-1;
// 24/03/21_税務代理書面 add <--
	if (pdata->data_check) {
		for (int ii = 0; ii < TOK_MAXCOL + 1; ii++) {
			if (CursorPosTOK[ii][0] == index) {
				if (m_pMapPrnEXSub.GetCount() > 0) {
					::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));
					m_pMapPrnEXSub.Lookup(ii+1, m_PrnEXSub);
					getflg = 1;

					break;
				}
			}
		}
// 24/03/21_税務代理書面 cor -->
	//	if (getflg)	setdata.data_edit.Format(_T("%d/1"), m_PrnEXSub.busu);
	//	else		setdata.data_edit.Format(_T("0/1"));
	//}
	//else setdata.data_edit.Format(_T("0/1"));
// -----------------------------
		if (getflg)	setdata.data_edit.Format(_T("%d/%d"),m_Bunsi[pos],m_Bunbo[pos]);
		else		setdata.data_edit.Format(_T("0/%d"),m_Bunbo[pos]);
	}
	else setdata.data_edit.Format(_T("0/%d"),m_Bunbo[pos]);
// 24/03/21_税務代理書面 cor <--

	m_Diag_Tok.SetData(index + 1, (LPUNKNOWN)&setdata);

// 24/03/22_背景色 add -->
	((CPrnReportDlg*)m_pParent)->BackColorControl(1);
// 24/03/22_背景色 add <--
}

void PrintTokuteiSub::ReSetAllNumDg()
{
	DIAGRAM_DATA setdata;

	for (int ii = 0; ii < TOK_MAXCOL + 1; ii++) {
		int index = CursorPosTOK[ii][0];
		if (index == -1) break;
		if (m_pMapPrnEXSub.GetCount() > 0) {
			::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));
			m_pMapPrnEXSub.Lookup(ii + 1, m_PrnEXSub);
// 24/03/21_税務代理書面 cor -->
			//if (m_PrnEXSub.check)	setdata.data_edit.Format(_T("%d/1"), m_PrnEXSub.busu);
			//else					setdata.data_edit.Format(_T("0/1"));
// -----------------------------
			if(m_PrnEXSub.check)	{
				setdata.data_edit.Format(_T("%d/%d"),m_Bunsi[ii],m_Bunbo[ii]);
			}
			else	{
				setdata.data_edit.Format(_T("0/%d"),m_Bunbo[ii]);
			}
// 24/03/21_税務代理書面 cor <--
			m_Diag_Tok.SetData(index + 1, (LPUNKNOWN)&setdata);
		}
	}
}

// 24/03/21_印刷枚数 add -->
// 印刷枚数の取得
void PrintTokuteiSub::GetBunbo( int pSw )
{
	int					ii=0,jj=0;
	int					st=0,ed=0;
	DIAGRAM_ATTRIBUTE	da;
	DIAGRAM_DATA		dd;

	CTaxationList31View	*pTaxlist31View = NULL;
	pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));

	if(pSw == -1)	ii = 0;
	else			ii = pSw;
	for(; ii<TOK_MAXCOL; ii++)	{
		diag_getattr(IDC_PRN_TOKUTEI_SUB_DG,CursorPosTOK[ii][0],&da,CIcsdiagctrl);
		if(da.attr_bcolor == BC_GRAY)	{
			m_Bunsi[ii] = 0;
			m_Bunbo[ii] = 0;
		}
		else	{
			//  0=計算式１
			if(ii == 0)	{
				m_Bunsi[ii] = m_Bunbo[ii] = 1;
			}
			//  1=計算式２（１）
			else if(ii == 1)	{
				m_Bunsi[ii] = m_Bunbo[ii] = pTaxlist31View->GetMxList21_31();
			}
			//  2=計算式２（２）
			else if(ii == 2)	{
				m_Bunsi[ii] = m_Bunbo[ii] = pTaxlist31View->GetMxList22_2_31();
			}
			//  3=計算式３，４
			else if(ii == 3)	{
				m_Bunsi[ii] = m_Bunbo[ii] = 1;
			}
			//  4=計算式５（１）
			else if(ii == 4)	{
				m_Bunsi[ii] = m_Bunbo[ii] = 1;
			}
			//  5=計算式５（２） 
			else if(ii == 5)	{
				m_Bunsi[ii] = m_Bunbo[ii] = 1;
			}
			//  6=計算式５（３）
			else if(ii == 6)	{
				m_Bunsi[ii] = m_Bunbo[ii] = 1;
			}
			//  7=計算式５－２（１）
			//  8=計算式５－２（２）
			//  9=計算式５－２（３）－１
			// 10=計算式５－２（３）－２
			// 11=計算式５－２（３）－３
			// 12=計算式５－２（４）－１
			// 13=計算式５－２（４）－２
			// 14=計算式５－２（４）－３
			else	{
				m_Bunsi[ii] = 0;
				m_Bunbo[ii] = 0;
				//m_Diag_Tok.GetData(CursorPosTOK[ii][2],(LPUNKNOWN)&dd);	// 24/07/10_個別タブから出力 del
				m_Diag_Tok.GetData(CursorPosTOK[ii][3], (LPUNKNOWN)&dd);	// 24/07/10_個別タブから出力 add
				st = _tstoi(dd.data_edit);
				//m_Diag_Tok.GetData(CursorPosTOK[ii][3],(LPUNKNOWN)&dd);	// 24/07/10_個別タブから出力 del
				m_Diag_Tok.GetData(CursorPosTOK[ii][4], (LPUNKNOWN)&dd);	// 24/07/10_個別タブから出力 add
				ed = _tstoi(dd.data_edit);
				// 分子
				int	max = pTaxlist31View->GetSpecListMaxPage();
				//for(jj=st-1; jj<ed; jj++)	{
				for(jj=0; jj<max; jj++)	{
					if(ii == 7)	{
						if(pTaxlist31View->IsGetSpecListData(jj,ii-7) == TRUE)	{
							m_Bunbo[ii]++;
							if(jj >= (st - 1) && jj<ed)	m_Bunsi[ii]++;
						}
						m_Sel[jj] = pTaxlist31View->GetSpecListType(jj);
					}
					else if(ii == 8)	{
						if(m_Sel[jj] == 0x01)	{
							if(pTaxlist31View->IsGetSpecListData(jj,ii-7) == TRUE)	{
								m_Bunbo[ii]++;
								if(jj >= (st - 1) && jj<ed)	m_Bunsi[ii]++;
							}
						}
					}
					else if(ii >= 9 && ii <= 11)	{
						if(m_Sel[jj] == 0x02)	{
							if(pTaxlist31View->IsGetSpecListData(jj,ii-7) == TRUE)	{
								m_Bunbo[ii]++;
								if(jj >= (st - 1) && jj<ed)	m_Bunsi[ii]++;
							}
						}
					}
					else	{
						if(m_Sel[jj] == 0x04)	{
							if(pTaxlist31View->IsGetSpecListData(jj,ii-7) == TRUE)	{
								m_Bunbo[ii]++;
								if(jj >= (st - 1) && jj<ed)	m_Bunsi[ii]++;
							}
						}
					}
				}
			}
		}
		if(pSw != -1)	break;
	}
}
// 24/03/21_印刷枚数 add <--

// 24/03/22_背景色 add -->
// ---------------------------------------------------------------------
// 選択可能なチェックがすべてＯＮになっているかを調べる
// ---------------------------------------------------------------------
// 【引　数】	なし
// 【戻り値】	bool	TRUE:すべてON　FALSE:それ以外
// ---------------------------------------------------------------------
bool PrintTokuteiSub::IsAllCheckOn( void )
{
	bool				brv=true;
	int					yy=0;
	DIAGRAM_ATTRIBUTE	da;
	DIAGRAM_DATA		dd;

	while(1)	{
		if(CursorPosTOK[yy][0] == -1)	{
			break;
		}
		diag_getattr(IDC_PRN_TOKUTEI_SUB_DG,CursorPosTOK[yy][0],&da,CIcsdiagctrl);
		if(da.attr_bcolor != BC_GRAY)	{
			m_Diag_Tok.GetData(CursorPosTOK[yy][0],(LPUNKNOWN)&dd);
			if(dd.data_check == 0)	{
				brv = false;
				break;
			}
		}
		yy++;
	}
	return(brv);
}
// 24/03/22_背景色 add <--

// 24/04/03_checkon add -->
// ---------------------------------------------------------------------
// 選択可能なチェックをすべてＯＮにする
// ---------------------------------------------------------------------
// 【引　数】	なし
// 【戻り値】	なし
// ---------------------------------------------------------------------
void PrintTokuteiSub::AllCheckOn( void )
{
	int					sw=1;
	int					yy=0;
	DIAGRAM_ATTRIBUTE	da;
	DIAGRAM_DATA		dd;

	// 選択できるチェックがすべてOFFの場合のみ実行する
	while(1)	{
		if(CursorPosTOK[yy][0] == -1)	{
			break;
		}
		diag_getattr(IDC_PRN_TOKUTEI_SUB_DG,CursorPosTOK[yy][0],&da,CIcsdiagctrl);
		if(da.attr_bcolor != BC_GRAY)	{
			m_Diag_Tok.GetData(CursorPosTOK[yy][0],(LPUNKNOWN)&dd);
			if(dd.data_check != 0)	{
				sw=0;
			}
		}
		yy++;
	}
	if(sw != 0)	{
		yy=0;
		while(1)	{
			if(CursorPosTOK[yy][0] == -1)	{
				break;
			}
			diag_getattr(IDC_PRN_TOKUTEI_SUB_DG,CursorPosTOK[yy][0],&da,CIcsdiagctrl);
			if(da.attr_bcolor != BC_GRAY)	{
				m_Diag_Tok.GetData(CursorPosTOK[yy][0],(LPUNKNOWN)&dd);
				if(dd.data_check == 0)	{
					dd.data_check = 1;
					m_Diag_Tok.SetData(CursorPosTOK[yy][0],(LPUNKNOWN)&dd);
					CheckButtonPrnTokuteiSubDg(CursorPosTOK[yy][0],(LPUNKNOWN)&dd);
				}
			}
			yy++;
		}
	}
}
// 24/04/03_checkon add <--
