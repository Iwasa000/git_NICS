// PrintZeitempSub.cpp : 実装ファイル
//

#include "stdafx.h"
//#include "pch.h"
#include "DBSyzShinMain.h"
#include "PrnReportDlg.h"

#include "PrintZeitempSub.h"
// 24/03/21_税務代理書面 del -->
//#include "PrnPrintSubIdx.h"
// 24/03/21_税務代理書面 del <--
#include "afxdialogex.h"

//税務代理帳票
extern PRNEXSUB_INF									m_PrnEXSub_ZEI;
extern CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub_ZEI;


short	CursorPosZEI[ZEI_MAXCOL+1][ZEI_MAXROW+1] =
{
// 24/07/10_個別タブから出力 cor -->
	//{ CHEK_ZE01,-1,-1 },
	//{ CHEK_ZE02,-1,-1 },
	//{ CHEK_ZE03,-1,-1 },
	//{ CHEK_ZE04,-1,-1 },
	//{ CHEK_ZE05,-1,-1 },
	//{ CHEK_ZE06,-1,-1 },
	//{ CHEK_ZE07,-1,-1 },
	//{ CHEK_ZE08,-1,-1 },
	//{ CHEK_ZE09,-1,-1 },
	//{ CHEK_ZE10,-1,-1 },
	//{ CHEK_ZE11,-1,-1 },
	//{ -1,-1,-1 }
// ---------------------------------
	{ CHEK_ZE01,CHEK_ZE01_BUS,-1,-1 },
	{ CHEK_ZE02,CHEK_ZE02_BUS,-1,-1 },
	{ CHEK_ZE03,CHEK_ZE03_BUS,-1,-1 },
	{ CHEK_ZE04,CHEK_ZE04_BUS,-1,-1 },
	{ CHEK_ZE05,CHEK_ZE05_BUS,-1,-1 },
	{ CHEK_ZE06,CHEK_ZE06_BUS,-1,-1 },
	{ CHEK_ZE07,CHEK_ZE07_BUS,-1,-1 },
	{ CHEK_ZE08,CHEK_ZE08_BUS,-1,-1 },
	{ CHEK_ZE09,CHEK_ZE09_BUS,-1,-1 },
	{ CHEK_ZE10,CHEK_ZE10_BUS,-1,-1 },
	{ CHEK_ZE11,CHEK_ZE11_BUS,-1,-1 },
	{ -1,-1,-1 }
// 24/07/10_個別タブから出力 cor <--
};



// PrintZeitempSub 

IMPLEMENT_DYNAMIC(PrintZeitempSub, ICSDialog)

PrintZeitempSub::PrintZeitempSub(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_DIALOG_PRN_ZEITEMP_SUB, pParent)
	, m_pSnHeadData(NULL)
	, m_pZmSub(NULL)
	, m_pShinInfo(NULL)
	, m_pPrintCmInfo(NULL)
	, m_SelSw(0)
	, m_Ctlsw(0)
	, m_PoutName(0)
	, m_PoutAdd(0)
	, m_PoutDensi(0)
{
	m_pParent= pParent;

}


PrintZeitempSub::~PrintZeitempSub()
{
}

void PrintZeitempSub::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRN_ZEITEMP_SUB_DG, m_Diag_Zei);
	DDX_Control(pDX, IDC_Zeimudairi_name, m_Check_Name);
	DDX_Control(pDX, IDC_Zeimudairi_address, m_Check_Add);
	DDX_Control(pDX, IDC_Zeimudairi_Densi, m_Check_Den);
	DDX_Control(pDX, IDC_ZEI_ALL, m_Check_All);
}


BEGIN_MESSAGE_MAP(PrintZeitempSub, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &PrintZeitempSub::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON7, &PrintZeitempSub::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_ZEI_ALL, &PrintZeitempSub::OnBnClickedZeiAll)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_Zeimudairi_name, &PrintZeitempSub::OnBnClickedZeimudairiname)
	ON_BN_CLICKED(IDC_Zeimudairi_address, &PrintZeitempSub::OnBnClickedZeimudairiaddress)
	ON_BN_CLICKED(IDC_Zeimudairi_Densi, &PrintZeitempSub::OnBnClickedZeimudairiDensi)
	ON_MESSAGE(WM_ACTIVE_SETFORCUS, OnMyFocusControl) 
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(PrintZeitempSub, ICSDialog)
	ON_EVENT(PrintZeitempSub, IDC_PRN_ZEITEMP_SUB_DG, 2, PrintZeitempSub::EditOFFPrnZeitempSub, VTS_I2)
	ON_EVENT(PrintZeitempSub, IDC_PRN_ZEITEMP_SUB_DG, 3, PrintZeitempSub::TerminationPrnZeitempSub, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(PrintZeitempSub, IDC_PRN_ZEITEMP_SUB_DG, 9, PrintZeitempSub::CheckButtonPrnZeitempSubDg, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

// PrintZeitempSub メッセージ ハンドラー
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
int PrintZeitempSub::InitInfo(CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, SH_PRINT_CMINFO *pPrintCmInfo, int pBeForked, int Ctlsw)
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

BOOL PrintZeitempSub::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Init();
	m_SelSw = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


LRESULT PrintZeitempSub::OnMyFocusControl(WPARAM w, LPARAM l)
{
	//初期セット
	m_Diag_Zei.SetFocus();
	//m_Diag_Zei.SetPosition(CHEK_ZE01);
	int					yy=0;
	DIAGRAM_ATTRIBUTE	da;
	while(1)	{
		if(CursorPosZEI[yy][0] == -1)	{
			break;
		}
		diag_getattr(IDC_PRN_ZEITEMP_SUB_DG,CursorPosZEI[yy][0],&da,CIcsdiagctrl);
		if(da.attr_bcolor != BC_GRAY)	{
			if(da.attr_bcolor == BC_WHITE)	{
				m_Diag_Zei.SetPosition(CursorPosZEI[yy][0]);
				break;
			}
		}
		yy++;
	}
// 24/03/25_168694 add -->
	if(CursorPosZEI[yy][0] == -1)	{
		GetDlgItem(IDC_Zeimudairi_name)->SetFocus();
	}
// 24/03/25_168694 add <--
	return 0;
}



//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int PrintZeitempSub::Init()
{
//CString	cs=_T("");
//cs.Format(_T("***** PrintZeitempSub::Init -> m_Ctlsw=%02x"),m_Ctlsw);
//DbgViewTrace(cs);

	m_Check_Name.SetCheck(m_Ctlsw & 0x01);
	m_Check_Add.SetCheck(m_Ctlsw & 0x02);
	m_Check_Den.SetCheck(m_Ctlsw & 0x04);

	// タイトルの背景色
	ChangeColor(IDC_PRN_ZEITEMP_SUB_DG, TITLE_ZE00, 0);
	ChangeColor(IDC_PRN_ZEITEMP_SUB_DG, TITLE_ZE02, 0);	// 24/07/10_個別タブから出力 add
	ChangeColor(IDC_PRN_ZEITEMP_SUB_DG, TITLE_ZE01, 0);

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

// 修正No.168895 add -->
	// 業種の設定によって事業(一般)が先頭行でない時もあるため、先頭行が何行目か取得しておく
	m_Yy = 0;
	DIAGRAM_ATTRIBUTE	da;
	//while(1)	{					// 修正No.168918 del
	while(m_Yy < ZEI_MAXCOL)	{	// 修正No.168918 add
		diag_getattr(IDC_PRN_ZEITEMP_SUB_DG, CursorPosZEI[m_Yy][0], &da, CIcsdiagctrl);
		if(da.attr_bcolor == BC_WHITE)	{
			break;
		}
		m_Yy++;
	}
// 修正No.168895 add <--

	return 0;
}


BOOL PrintZeitempSub::PreTranslateMessage(MSG* pMsg)
{
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
			  ( (pMsg->wParam == VK_TAB) && !(::GetKeyState(VK_SHIFT)) )  ) {
			
			int id = GetFocus()->GetDlgCtrlID();

			if ((id == IDC_PRN_ZEITEMP_SUB_DG) && (m_Diag_Zei.GetPosition() == CHEK_ZE11) ) {
				GetDlgItem(IDC_ZEI_ALL)->SetFocus();
				return(TRUE);
			}
			if ((id == IDC_ZEI_ALL) && (pMsg->wParam == VK_TAB)) {
				GetDlgItem(IDC_Zeimudairi_name)->SetFocus();
				return(TRUE);
			}
			if (id == IDC_Zeimudairi_name) {
				GetDlgItem(IDC_Zeimudairi_address)->SetFocus();
				return(TRUE);
			}
			if (id == IDC_Zeimudairi_address) {
				GetDlgItem(IDC_Zeimudairi_Densi)->SetFocus();
				return(TRUE);
			}
			if ((id == IDC_Zeimudairi_Densi) ) {
				GetDlgItem(IDC_BUTTON2)->SetFocus();
				return(TRUE);
			}
			if ((id == IDC_BUTTON2) && (pMsg->wParam == VK_TAB)) {
				GetDlgItem(IDC_BUTTON7)->SetFocus();
				return(TRUE);
			}

			if ((id == IDC_BUTTON7) && (pMsg->wParam == VK_TAB)) {
// 24/04/03_開発員3 cor -->
				//GetDlgItem(IDC_PRN_ZEITEMP_SUB_DG)->SetFocus();
				//m_Diag_Zei.SetPosition(CHEK_ZE01);
// ------------------------
				if(IsDiagEnabled() == true)	{
					GetDlgItem(IDC_PRN_ZEITEMP_SUB_DG)->SetFocus();
					m_Diag_Zei.SetPosition(CHEK_ZE01);
				}
				else	{
					GetDlgItem(IDC_ZEI_ALL)->SetFocus();
				}
// 24/04/03_開発員3 cor <--

				//c_set(IDC_PRN_ZEITEMP_SUB_DG);
				//diag_setposition(IDC_PRN_ZEITEMP_SUB_DG, CHEK_ZE01, CIcsdiagctrl);
				return(TRUE);
			}
		}
// 24/04/03_開発員3 add -->
		if((pMsg->wParam == VK_TAB) && (::GetKeyState(VK_SHIFT)))	{
			int id = GetFocus()->GetDlgCtrlID();
			if(id == IDC_ZEI_ALL) {
				if(IsDiagEnabled() != true)	{
					GetDlgItem(IDC_BUTTON7)->SetFocus();
					return(TRUE);
				}
			}
		}
// 24/04/03_開発員3 add <--

		if (pMsg->wParam == VK_DELETE) {
			// 法人税と同じようにDeleteキーは反応しないようにする
			return(TRUE);
		}

		if (pMsg->wParam == VK_F9) {
			OnBnClickedZeiAll();
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


void PrintZeitempSub::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	ICSDialog::OnCancel();
}


void PrintZeitempSub::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	ICSDialog::OnOK();
}


void PrintZeitempSub::OnBnClickedButton2()
{
	GetParent()->GetParent()->PostMessage(WM_KEYDOWN, VK_F11);
}


void PrintZeitempSub::OnBnClickedButton7()
{
	GetParent()->GetParent()->PostMessage(WM_KEYDOWN, VK_F12);
}



void PrintZeitempSub::OnBnClickedZeiAll()
{
	ChoiceItem(0);
	GetInputItemData();

	// 入力項目設定
	SetInputItem();
	//枚数の再セット
	ReSetAllNumDg();
// 24/03/22_背景色 add -->
	((CPrnReportDlg*)m_pParent)->BackColorControl(2);
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
int PrintZeitempSub::SetInputItem(void)
{
	int					ii = 0, jj = 0;

	for (ii = 0; ii < ZEI_MAXCOL + 1; ii++) {
		if (m_pMapPrnEXSub_ZEI.GetCount() > 0) {
			::ZeroMemory(&m_PrnEXSub_ZEI, sizeof(PRNEXSUB_INF));
			m_pMapPrnEXSub_ZEI.Lookup(ii, m_PrnEXSub_ZEI);
		}

		DIAGRAM_DATA	dd;
		switch (ii) {
// 24/07/10_個別タブから出力 cor -->
		//case 0:		// 税務代理権限証書
		//	SetInputItemSub(CHEK_ZE01, CHEK_ZE01_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 1:		// 税理士法第33条の2第1項に規定する添付書面（第1面
		//	SetInputItemSub(CHEK_ZE02, CHEK_ZE02_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 2:		// 税理士法第33条の2第1項に規定する添付書面（第2面）
		//	SetInputItemSub(CHEK_ZE03, CHEK_ZE03_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 3:		// 税理士法第33条の2第1項に規定する添付書面（第3面）
		//	SetInputItemSub(CHEK_ZE04, CHEK_ZE04_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 4:		// 計税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
		//	SetInputItemSub(CHEK_ZE05, CHEK_ZE05_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 5:		// 税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
		//	SetInputItemSub(CHEK_ZE06, CHEK_ZE06_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 6:		// 税理士法第33条の2第2項に規定する添付書面（第1面）
		//	SetInputItemSub(CHEK_ZE07, CHEK_ZE07_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 7:		// 税理士法第33条の2第2項に規定する添付書面（第2面）
		//	SetInputItemSub(CHEK_ZE08, CHEK_ZE08_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 8:		// 税理士法第33条の2第2項に規定する添付書面（第3面）
		//	SetInputItemSub(CHEK_ZE09, CHEK_ZE09_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 9:		// 税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
		//	SetInputItemSub(CHEK_ZE10, CHEK_ZE10_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 10:	// 税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）
		//	SetInputItemSub(CHEK_ZE11, CHEK_ZE11_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;
// ---------------------------------
		case 0:		// 税務代理権限証書
			SetInputItemSub(CHEK_ZE01, CHEK_ZE01_BUS, CHEK_ZE01_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 1:		// 税理士法第33条の2第1項に規定する添付書面（第1面
			SetInputItemSub(CHEK_ZE02, CHEK_ZE02_BUS, CHEK_ZE02_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 2:		// 税理士法第33条の2第1項に規定する添付書面（第2面）
			SetInputItemSub(CHEK_ZE03, CHEK_ZE03_BUS, CHEK_ZE03_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 3:		// 税理士法第33条の2第1項に規定する添付書面（第3面）
			SetInputItemSub(CHEK_ZE04, CHEK_ZE04_BUS, CHEK_ZE04_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 4:		// 計税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
			SetInputItemSub(CHEK_ZE05, CHEK_ZE05_BUS, CHEK_ZE05_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 5:		// 税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
			SetInputItemSub(CHEK_ZE06, CHEK_ZE06_BUS, CHEK_ZE06_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 6:		// 税理士法第33条の2第2項に規定する添付書面（第1面）
			SetInputItemSub(CHEK_ZE07, CHEK_ZE07_BUS, CHEK_ZE07_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 7:		// 税理士法第33条の2第2項に規定する添付書面（第2面）
			SetInputItemSub(CHEK_ZE08, CHEK_ZE08_BUS, CHEK_ZE08_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 8:		// 税理士法第33条の2第2項に規定する添付書面（第3面）
			SetInputItemSub(CHEK_ZE09, CHEK_ZE09_BUS, CHEK_ZE09_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 9:		// 税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
			SetInputItemSub(CHEK_ZE10, CHEK_ZE10_BUS, CHEK_ZE10_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 10:	// 税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）
			SetInputItemSub(CHEK_ZE11, CHEK_ZE11_BUS, CHEK_ZE11_NUM, &m_PrnEXSub_ZEI, &dd);
			break;
// 24/07/10_個別タブから出力 cor <--

		default:
			break;
		}
	}

	m_Diag_Zei.Refresh();
	
	return(0);
}

// -----------------------------------------------------------------------------
// 入力制限の設定
// -----------------------------------------------------------------------------
// 【引　数】	なし
// 【戻り値】	なし
// -----------------------------------------------------------------------------
int PrintZeitempSub::SetEnableItem(int pSw)
{
	int					ii = 0;
//	if (pSw == -1)	ck1 = m_Check1.GetCheck();
//	else			ck1 = pSw;

// 24/03/21_税務代理書面 cor -->
	////コントロールの制限はなし
	//for (ii = 0; ii < ZEI_MAXCOL + 1; ii++) {
	//	switch (ii) {

	//	case 0:		// 税務代理権限証書 入力制限
	//		SetColorItemSub(1, CHEK_ZE01, CHEK_ZE01_NUM);
	//		break;

	//	case 1:		// 税理士法第33条の2第1項に規定する添付書面（第1面）
	//		SetColorItemSub(1, CHEK_ZE02, CHEK_ZE02_NUM);
	//		break;

	//	case 2:		// 
	//		SetColorItemSub(1, CHEK_ZE03, CHEK_ZE03_NUM);
	//		break;

	//	case 3:		// 
	//		SetColorItemSub(1, CHEK_ZE04, CHEK_ZE04_NUM);
	//		break;

	//	case 4:		// 
	//		SetColorItemSub(1, CHEK_ZE05, CHEK_ZE05_NUM);
	//		break;

	//	case 5:		//
	//		SetColorItemSub(1, CHEK_ZE06, CHEK_ZE06_NUM);
	//		break;

	//	case 6:		//
	//		SetColorItemSub(1, CHEK_ZE07, CHEK_ZE07_NUM);
	//		break;

	//	case 7:		//
	//		SetColorItemSub(1, CHEK_ZE08, CHEK_ZE08_NUM);
	//		break;

	//	case 8:		// 
	//		SetColorItemSub(1, CHEK_ZE09, CHEK_ZE09_NUM);
	//		break;

	//	case 9:	// 
	//		SetColorItemSub(1, CHEK_ZE10, CHEK_ZE10_NUM);
	//		break;

	//	case 10:	// 
	//		SetColorItemSub(1, CHEK_ZE11, CHEK_ZE11_NUM);
	//		break;

	//	default:
	//		break;
	//	}
	//}
// -----------------------------
	int									yflg=0,yno=0;
	int									flg=0;
	CArray<TAXAGENT_STFF,TAXAGENT_STFF>	car;
	// 呼び出し元業務
	if(m_Util.IsUnConnectMst(m_pZmSub->zvol))	yflg=1;	// 消費税申告書（非連動）
	else										yflg=0;	// 消費税申告書

	for (ii = 0; ii < ZEI_MAXCOL + 1; ii++) {
		switch (ii) {
// 24/07/10_個別タブから出力 cor -->
		//case 0:		// 税務代理権限証書
		//	yno = 0x01;
		//	m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
		//	if(m_Bunbo[ii] == 0)	flg=0;
		//	else					flg=1;
		//	SetColorItemSub(flg, CHEK_ZE01, CHEK_ZE01_NUM);
		//	break;

		//case 1:		// 申告書の作成に関する計算事項記載書面 33の2(1) (1面)
		//	yno = 0x02;
		//	m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
		//	if(m_Bunbo[ii] == 0)	flg=0;
		//	else					flg=1;
		//	SetColorItemSub(flg, CHEK_ZE02, CHEK_ZE02_NUM);
		//	break;

		//case 2:		// 申告書の作成に関する計算事項記載書面 33の2(1) (2面)
		//	yno = 0x04;
		//	m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
		//	if(m_Bunbo[ii] == 0)	flg=0;
		//	else					flg=1;
		//	SetColorItemSub(flg, CHEK_ZE03, CHEK_ZE03_NUM);
		//	break;

		//case 3:		// 申告書の作成に関する計算事項記載書面 33の2(1) (3面)
		//	yno = 0x08;
		//	m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
		//	if(m_Bunbo[ii] == 0)	flg=0;
		//	else					flg=1;
		//	SetColorItemSub(flg, CHEK_ZE04, CHEK_ZE04_NUM);
		//	break;

		//case 4:		// 申告書の作成に関する計算事項記載書面 33の2(1) (4面 1枚目) 
		//	yno = 0x10;
		//	m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
		//	if(m_Bunbo[ii] == 0)	flg=0;
		//	else					flg=1;
		//	SetColorItemSub(flg, CHEK_ZE05, CHEK_ZE05_NUM);
		//	break;

		//case 5:		// 申告書の作成に関する計算事項記載書面 33の2(1) (4面 2枚目)
		//	yno = 0x20;
		//	m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
		//	if(m_Bunbo[ii] == 0)	flg=0;
		//	else					flg=1;
		//	SetColorItemSub(flg, CHEK_ZE06, CHEK_ZE06_NUM);
		//	break;

		//case 6:		// 申告書に関する審査事項記載書面 33の2(2) (1面)
		//	yno = 0x40;
		//	m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
		//	if(m_Bunbo[ii] == 0)	flg=0;
		//	else					flg=1;
		//	SetColorItemSub(flg, CHEK_ZE07, CHEK_ZE07_NUM);
		//	break;

		//case 7:		// 申告書に関する審査事項記載書面 33の2(2) (2面)
		//	yno = 0x80;
		//	m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
		//	if(m_Bunbo[ii] == 0)	flg=0;
		//	else					flg=1;
		//	SetColorItemSub(flg, CHEK_ZE08, CHEK_ZE08_NUM);
		//	break;

		//case 8:		// 申告書に関する審査事項記載書面 33の2(2) (3面)
		//	yno = 0x100;
		//	m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
		//	if(m_Bunbo[ii] == 0)	flg=0;
		//	else					flg=1;
		//	SetColorItemSub(flg, CHEK_ZE09, CHEK_ZE09_NUM);
		//	break;

		//case 9:		// 申告書に関する審査事項記載書面 33の2(2) (4面 1枚目)
		//	yno = 0x200;
		//	m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
		//	if(m_Bunbo[ii] == 0)	flg=0;
		//	else					flg=1;
		//	SetColorItemSub(flg, CHEK_ZE10, CHEK_ZE10_NUM);
		//	break;

		//case 10:	// 申告書に関する審査事項記載書面 33の2(2) (4面 2枚目)
		//	yno = 0x400;
		//	m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
		//	if(m_Bunbo[ii] == 0)	flg=0;
		//	else					flg=1;
		//	SetColorItemSub(flg, CHEK_ZE11, CHEK_ZE11_NUM);
		//	break;
// ---------------------------------
		case 0:		// 税務代理権限証書
			yno = 0x01;
			m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
			if(m_Bunbo[ii] == 0)	flg=0;
			else					flg=1;
			SetColorItemSub(flg, CHEK_ZE01, CHEK_ZE01_BUS, CHEK_ZE01_NUM);
			break;

		case 1:		// 申告書の作成に関する計算事項記載書面 33の2(1) (1面)
			yno = 0x02;
			m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
			if(m_Bunbo[ii] == 0)	flg=0;
			else					flg=1;
			SetColorItemSub(flg, CHEK_ZE02, CHEK_ZE02_BUS, CHEK_ZE02_NUM);
			break;

		case 2:		// 申告書の作成に関する計算事項記載書面 33の2(1) (2面)
			yno = 0x04;
			m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
			if(m_Bunbo[ii] == 0)	flg=0;
			else					flg=1;
			SetColorItemSub(flg, CHEK_ZE03, CHEK_ZE03_BUS, CHEK_ZE03_NUM);
			break;

		case 3:		// 申告書の作成に関する計算事項記載書面 33の2(1) (3面)
			yno = 0x08;
			m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
			if(m_Bunbo[ii] == 0)	flg=0;
			else					flg=1;
			SetColorItemSub(flg, CHEK_ZE04, CHEK_ZE04_BUS, CHEK_ZE04_NUM);
			break;

		case 4:		// 申告書の作成に関する計算事項記載書面 33の2(1) (4面 1枚目) 
			yno = 0x10;
			m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
			if(m_Bunbo[ii] == 0)	flg=0;
			else					flg=1;
			SetColorItemSub(flg, CHEK_ZE05, CHEK_ZE05_BUS, CHEK_ZE05_NUM);
			break;

		case 5:		// 申告書の作成に関する計算事項記載書面 33の2(1) (4面 2枚目)
			yno = 0x20;
			m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
			if(m_Bunbo[ii] == 0)	flg=0;
			else					flg=1;
			SetColorItemSub(flg, CHEK_ZE06, CHEK_ZE06_BUS, CHEK_ZE06_NUM);
			break;

		case 6:		// 申告書に関する審査事項記載書面 33の2(2) (1面)
			yno = 0x40;
			m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
			if(m_Bunbo[ii] == 0)	flg=0;
			else					flg=1;
			SetColorItemSub(flg, CHEK_ZE07, CHEK_ZE07_BUS, CHEK_ZE07_NUM);
			break;

		case 7:		// 申告書に関する審査事項記載書面 33の2(2) (2面)
			yno = 0x80;
			m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
			if(m_Bunbo[ii] == 0)	flg=0;
			else					flg=1;
			SetColorItemSub(flg, CHEK_ZE08, CHEK_ZE08_BUS, CHEK_ZE08_NUM);
			break;

		case 8:		// 申告書に関する審査事項記載書面 33の2(2) (3面)
			yno = 0x100;
			m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
			if(m_Bunbo[ii] == 0)	flg=0;
			else					flg=1;
			SetColorItemSub(flg, CHEK_ZE09, CHEK_ZE09_BUS, CHEK_ZE09_NUM);
			break;

		case 9:		// 申告書に関する審査事項記載書面 33の2(2) (4面 1枚目)
			yno = 0x200;
			m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
			if(m_Bunbo[ii] == 0)	flg=0;
			else					flg=1;
			SetColorItemSub(flg, CHEK_ZE10, CHEK_ZE10_BUS, CHEK_ZE10_NUM);
			break;

		case 10:	// 申告書に関する審査事項記載書面 33の2(2) (4面 2枚目)
			yno = 0x400;
			m_Bunbo[ii] = ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this);
			if(m_Bunbo[ii] == 0)	flg=0;
			else					flg=1;
			SetColorItemSub(flg, CHEK_ZE11, CHEK_ZE11_BUS, CHEK_ZE11_NUM);
			break;
// 24/07/10_個別タブから出力 cor <--

		default:
			break;
		}
	}
// 24/03/21_税務代理書面 cor <--

	m_Diag_Zei.Refresh();
	
	return(0);
}

//-----------------------------------------------------------------------------
// 入力項目取得
//-----------------------------------------------------------------------------
// 引数	
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int PrintZeitempSub::GetInputItemData(void)
{
	int				ii = 0;
	int				chek;
	int				busu;

	DIAGRAM_DATA		dd;
// 24/04/03_checkon2 add -->
	DIAGRAM_ATTRIBUTE	da;
// 24/04/03_checkon2 add <--

	for (ii = 0; ii < ZEI_MAXCOL + 1; ii++) {
		// 初期化
		chek = 0;
		busu = NULL;
	
// 24/04/03_checkon2 add -->
		// 選択不可の場合、書き込まない
		// （初回にONにしているので、後から帳票を追加したときのデフォルトがONになるように）
		diag_getattr(IDC_PRN_ZEITEMP_SUB_DG,CursorPosZEI[ii][0],&da,CIcsdiagctrl);
		if(da.attr_bcolor == BC_GRAY)	{
			continue;
		}
// 24/04/03_checkon2 add <--

		// マップデータの取得
		::ZeroMemory(&m_PrnEXSub_ZEI, sizeof(PRNEXSUB_INF));
		m_pMapPrnEXSub_ZEI.Lookup(ii, m_PrnEXSub_ZEI);

		switch (ii) {
// 24/07/10_個別タブから出力 cor -->
		//case 0:		// 税務代理権限証書
		//	GetInputItemDataSub(CHEK_ZE01, CHEK_ZE01_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 1:		// 
		//	GetInputItemDataSub(CHEK_ZE02, CHEK_ZE02_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 2:		// 
		//	GetInputItemDataSub(CHEK_ZE03, CHEK_ZE03_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 3:		//
		//	GetInputItemDataSub(CHEK_ZE04, CHEK_ZE04_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 4:		// 
		//	GetInputItemDataSub(CHEK_ZE05, CHEK_ZE05_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 5:		// 
		//	GetInputItemDataSub(CHEK_ZE06, CHEK_ZE06_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 6:		//
		//	GetInputItemDataSub(CHEK_ZE07, CHEK_ZE07_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 7:		// 
		//	GetInputItemDataSub(CHEK_ZE08, CHEK_ZE08_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 8:		//
		//	GetInputItemDataSub(CHEK_ZE09, CHEK_ZE09_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 9:	// 
		//	GetInputItemDataSub(CHEK_ZE10, CHEK_ZE10_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;

		//case 10:	// 
		//	GetInputItemDataSub(CHEK_ZE11, CHEK_ZE11_NUM, &m_PrnEXSub_ZEI, &dd);
		//	break;
// ---------------------------------
		case 0:		// 税務代理権限証書
			GetInputItemDataSub(CHEK_ZE01, CHEK_ZE01_BUS, CHEK_ZE01_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 1:		// 
			GetInputItemDataSub(CHEK_ZE02, CHEK_ZE02_BUS, CHEK_ZE02_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 2:		// 
			GetInputItemDataSub(CHEK_ZE03, CHEK_ZE03_BUS, CHEK_ZE03_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 3:		//
			GetInputItemDataSub(CHEK_ZE04, CHEK_ZE04_BUS, CHEK_ZE04_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 4:		// 
			GetInputItemDataSub(CHEK_ZE05, CHEK_ZE05_BUS, CHEK_ZE05_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 5:		// 
			GetInputItemDataSub(CHEK_ZE06, CHEK_ZE06_BUS, CHEK_ZE06_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 6:		//
			GetInputItemDataSub(CHEK_ZE07, CHEK_ZE07_BUS, CHEK_ZE07_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 7:		// 
			GetInputItemDataSub(CHEK_ZE08, CHEK_ZE08_BUS, CHEK_ZE08_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 8:		//
			GetInputItemDataSub(CHEK_ZE09, CHEK_ZE09_BUS, CHEK_ZE09_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 9:		// 
			GetInputItemDataSub(CHEK_ZE10, CHEK_ZE10_BUS, CHEK_ZE10_NUM, &m_PrnEXSub_ZEI, &dd);
			break;

		case 10:	// 
			GetInputItemDataSub(CHEK_ZE11, CHEK_ZE11_BUS, CHEK_ZE11_NUM, &m_PrnEXSub_ZEI, &dd);
			break;
// 24/07/10_個別タブから出力 cor <--

		default:
			break;
		}
		//m_PrnEXSub_ZEI.check = chek;
		//m_PrnEXSub_ZEI.busu = busu;
		//m_PrnEXSub_ZEI.stpage = stpg;
		//m_PrnEXSub_ZEI.edpage = edpg;

		// 更新
		m_pMapPrnEXSub_ZEI.SetAt(ii, m_PrnEXSub_ZEI);
	}

	return(0);

}


HBRUSH PrintZeitempSub::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ここで DC の属性を変更してください。

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
void PrintZeitempSub::ChangeColor(unsigned short id, short index, int sign)
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
void PrintZeitempSub::EditOFFPrnZeitempSub(short index)
{
// 24/03/21_税務代理書面 del -->
	//int				edpg;
	//DIAGRAM_DATA	dd, dd2;
	//
	//	// 終了頁を取得
	//	edpg = 999;
	////	CTaxationList31View	*pTaxlist31View = NULL;
	////	pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));
	////	edpg = pTaxlist31View->GetSpecListMaxPage();

	//	dd.data_imgdata = NULL;
	//	m_Diag_Zei.GetData(index, (LPUNKNOWN)&dd);

	//	switch (index) {
	//		// 部数
	//	case CHEK_ZE01_NUM:			// 税務代理権限証書１
	//	case CHEK_ZE02_NUM:			// 税理士法第33条の2第1項に規定する添付書面（第1面）
	//	case CHEK_ZE03_NUM:			// 税理士法第33条の2第1項に規定する添付書面（第2面）
	//	case CHEK_ZE04_NUM:			// 
	//	case CHEK_ZE05_NUM:			// 
	//	case CHEK_ZE06_NUM:			// 
	//	case CHEK_ZE07_NUM:			// 
	//	case CHEK_ZE08_NUM:			// 
	//	case CHEK_ZE09_NUM:			// 
	//	case CHEK_ZE10_NUM:			// 
	//	case CHEK_ZE11_NUM:			// 
	//		// 1-99以外は受け付けないようにする
	////		if (_tstoi(dd.data_edit) < 1 || _tstoi(dd.data_edit) > 99) {
	//			dd.data_edit = _T("1");
	//			m_Diag_Zei.SetData(index, (LPUNKNOWN)&dd);
	////		}
	//		break;

	//	default:
	//		break;
	//	}
// 24/03/21_税務代理書面 del <--

// 24/07/10_個別タブから出力 add -->
	int				edpg;
	DIAGRAM_DATA	dd, dd2;

	dd.data_imgdata = NULL;
	m_Diag_Zei.GetData(index, (LPUNKNOWN)&dd);

	switch (index) {
		// 部数
		case CHEK_ZE01_BUS:			// 税務代理権限証書
		case CHEK_ZE02_BUS:			// 申告書の作成に関する計算事項等記載書面 33の2(1) (1面)
		case CHEK_ZE03_BUS:			// 申告書の作成に関する計算事項等記載書面 33の2(1) (2面)
		case CHEK_ZE04_BUS:			// 申告書の作成に関する計算事項等記載書面 33の2(1) (3面)
		case CHEK_ZE05_BUS:			// 申告書の作成に関する計算事項等記載書面 33の2(1) (4面 1枚目)
		case CHEK_ZE06_BUS:			// 申告書の作成に関する計算事項等記載書面 33の2(1) (4面 2枚目)
		case CHEK_ZE07_BUS:			// 申告書に関する審査事項等記載書面 33の2(2) (1面)
		case CHEK_ZE08_BUS:			// 申告書に関する審査事項等記載書面 33の2(2) (2面)
		case CHEK_ZE09_BUS:			// 申告書に関する審査事項等記載書面 33の2(2) (3面)
		case CHEK_ZE10_BUS:			// 申告書に関する審査事項等記載書面 33の2(2) (4面 1枚目)
		case CHEK_ZE11_BUS:			// 申告書に関する審査事項等記載書面 33の2(2) (4面 2枚目)
			// 1-99以外は受け付けないようにする
			if (_tstoi(dd.data_edit) < 1 || _tstoi(dd.data_edit) > 99) {
				dd.data_edit = _T("1");
				m_Diag_Zei.SetData(index, (LPUNKNOWN)&dd);
			}
			break;

		default:
			break;
	}
// 24/07/10_個別タブから出力 add <--
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void PrintZeitempSub::TerminationPrnZeitempSub(short index, short nChar, short length, LPUNKNOWN data)
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
	for (xx = 0; xx < ZEI_MAXROW; xx++) {
		for (yy = 0; yy < ZEI_MAXCOL; yy++) {
			if (CursorPosZEI[yy][xx] == index) {
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
				if (xx > ZEI_MAXROW) {
					xx = 0;
					yy++;
				}
				if (yy > ZEI_MAXCOL) {
					yy = 0;
				}
			}
			else if (wTerm == VK_F2) {
				xx--;
				if (xx < 0) {
					xx = ZEI_MAXROW-1;
					yy--;
				}
				if (yy < 0) {
					yy = ZEI_MAXCOL;
				}
			}
			if (CursorPosZEI[yy][xx] == -1) {
				continue;
			}
			diag_getattr(IDC_PRN_ZEITEMP_SUB_DG, CursorPosZEI[yy][xx], &da, CIcsdiagctrl);
			if (da.attr_bcolor == BC_GRAY) {
				continue;
			}
			if (da.attr_bcolor == BC_WHITE) {
				npos = CursorPosZEI[yy][xx];
				break;
			}
		}
		break;

	case VK_RIGHT:
	case VK_LEFT:
		while (1) {
			if (wTerm == VK_RIGHT)		xx++;
			else if (wTerm == VK_LEFT)	xx--;

			if (xx < 0)	xx = 0;

			if (CursorPosZEI[yy][xx] == -1) {
				break;
			}
			diag_getattr(IDC_PRN_ZEITEMP_SUB_DG, CursorPosZEI[yy][xx], &da, CIcsdiagctrl);
			if (da.attr_bcolor == BC_GRAY) {
				continue;
			}
			if (da.attr_bcolor == BC_WHITE) {
				npos = CursorPosZEI[yy][xx];
				break;
			}
		}
		break;

	case VK_DOWN:
	case VK_UP:
		while (1) {
			if (wTerm == VK_DOWN)	yy++;
			else if (wTerm == VK_UP)	yy--;

// 修正No.168895 cor -->
			//if (yy < 0)	yy = 0;
// ---------------------
			if(yy < m_Yy)	yy = m_Yy;
// 修正No.168895 cor <--

			if (CursorPosZEI[yy][xx] == -1) {
				break;
			}
			diag_getattr(IDC_PRN_ZEITEMP_SUB_DG, CursorPosZEI[yy][xx], &da, CIcsdiagctrl);
			if (da.attr_bcolor == BC_GRAY) {
				continue;
			}
			if (da.attr_bcolor == BC_WHITE) {
				npos = CursorPosZEI[yy][xx];
				break;
			}
		}
		break;
	}

	if (npos != -1) {
		//// 最終のインデックスでEnterキーを押下した場合
		//if (wTerm == VK_RETURN && npos == CHEK_ZE01) {
		//	// OKボタンへ移動
		//	GetDlgItem(IDC_ZEI_ALL)->SetFocus();
		//}
		//else {
			m_Diag_Zei.SetPosition(npos);
		//}
	}
	
}

// 24/07/10_個別タブから出力 cor -->
//int PrintZeitempSub::SetInputItemSub(int CtIDCh, int CtIDNum, PRNEXSUB_INF *SubInf, DIAGRAM_DATA	*pdd)
// ---------------------------------
int PrintZeitempSub::SetInputItemSub(int CtIDCh, int CtIDBus, int CtIDNum, PRNEXSUB_INF *SubInf, DIAGRAM_DATA *pdd)
// 24/07/10_個別タブから出力 cor <--
{
	int set_num = 0;
	int pos = 0;		// 24/07/10_個別タブから出力 add

	// データをセット
	pdd->data_imgdata = NULL;
	// チェックON/OFF
// 24/03/21_168685 cor -->
	//if (SubInf->check == 0)		pdd->data_check = 0;
// -----------------------
	DIAGRAM_ATTRIBUTE	da;
	diag_getattr(IDC_PRN_ZEITEMP_SUB_DG,CtIDCh,&da,CIcsdiagctrl);
	if (SubInf->check == 0 || da.attr_bcolor == BC_GRAY)	{
		pdd->data_check = 0;
	}
// 24/03/21_168685 cor <--
	else {
		pdd->data_check = 1;
		set_num = 1;
	}
	m_Diag_Zei.SetData(CtIDCh, (LPUNKNOWN)pdd);

// 24/07/10_個別タブから出力 add -->
	// 部数
	pos = CtIDCh / 2 - 1;
	if(m_Bunbo[pos] == 0)	pdd->data_edit.Format(_T("1"));
	else					pdd->data_edit.Format(_T("%d"), SubInf->busu);
	m_Diag_Zei.SetData(CtIDBus, (LPUNKNOWN)pdd);
// 24/07/10_個別タブから出力 add <--

	// 印刷枚数
	pdd->data_check = 0;
// 24/03/21_税務代理書面 cor -->
	//if (set_num)	pdd->data_edit.Format(_T("%d/1"), SubInf->busu);
	//else			pdd->data_edit.Format(_T("0/1"));
// -----------------------------
	//int pos=CtIDCh/2-1;	// 24/07/10_個別タブから出力 del
	pos = CtIDCh / 2 - 1;	// 24/07/10_個別タブから出力 add
	if(set_num)	pdd->data_edit.Format(_T("%d/%d"),m_Bunbo[pos],m_Bunbo[pos]);
	else		pdd->data_edit.Format(_T("0/%d"),m_Bunbo[pos]);
// 24/03/21_税務代理書面 cor <--
	m_Diag_Zei.SetData(CtIDNum, (LPUNKNOWN)pdd);

	return 0;
}

//nflgはマスクしたものを渡す
// 24/07/10_個別タブから出力 cor -->
//int PrintZeitempSub::SetColorItemSub(int ck, int CtIDCh, int CtIDNum)
// ---------------------------------
int PrintZeitempSub::SetColorItemSub(int ck, int CtIDCh, int CtIBus, int CtIDNum)
// 24/07/10_個別タブから出力 cor <--
{
	// 入力制限
	if (ck) {
		// 入力可
		// チェック
		ChangeColor(IDC_PRN_ZEITEMP_SUB_DG, CtIDCh, 1);
		m_Diag_Zei.ModifyItem(CtIDCh, DIAG_MDFY_EDIT);
// 24/07/10_個別タブから出力 add -->
		ChangeColor(IDC_PRN_ZEITEMP_SUB_DG, CtIBus, 1);
		m_Diag_Zei.ModifyItem(CtIBus, DIAG_MDFY_EDIT);
// 24/07/10_個別タブから出力 add <--
	}
	else {
		// 入力不可
		// チェック
		ChangeColor(IDC_PRN_ZEITEMP_SUB_DG, CtIDCh, 2);
		m_Diag_Zei.ModifyItem(CtIDCh, DIAG_MDFY_READONLY);
// 24/07/10_個別タブから出力 add -->
		ChangeColor(IDC_PRN_ZEITEMP_SUB_DG, CtIBus, 2);
		m_Diag_Zei.ModifyItem(CtIBus, DIAG_MDFY_READONLY);
// 24/07/10_個別タブから出力 add <--
	}

	// 印刷枚数
	//ChangeColor(IDC_PRN_ZEITEMP_SUB_DG, CtIDNum, 2);	// 修正No.168720 del
	ChangeColor(IDC_PRN_ZEITEMP_SUB_DG, CtIDNum, 6);	// 修正No.168720 add
	m_Diag_Zei.ModifyItem(CtIDNum, DIAG_MDFY_READONLY);

	return 0;
}

// 24/07/10_個別タブから出力 cor -->
//int PrintZeitempSub::GetInputItemDataSub(int CtIDCh, int CtIDNum, PRNEXSUB_INF *SubInf, DIAGRAM_DATA	*pdd)
//{
//	int		chek = 0;
//	int		busu = 0;
//	int		stpg = 0, edpg = 0;
//
//	pdd->data_imgdata = NULL;
//	// チェック
//	m_Diag_Zei.GetData(CtIDCh, (LPUNKNOWN)pdd);
//	chek = pdd->data_check;
//// 24/03/21_税務代理書面 del -->
//	//// 部数
//	//m_Diag_Zei.GetData(CtIDNum, (LPUNKNOWN)pdd);
//	//// 1-99以外は受け付けないようにする
//	//if (_tstoi(pdd->data_edit) >= 1 && _tstoi(pdd->data_edit) <= 99) {
//	//	busu = _tstoi(pdd->data_edit);
//	//}
//// 24/03/21_税務代理書面 del <--
//
//	::ZeroMemory(SubInf, sizeof(PRNEXSUB_INF));
//
//	SubInf->check = chek;
//// 24/03/21_税務代理書面 del -->
//	//SubInf->busu = busu;
//	//SubInf->stpage = stpg;
//	//SubInf->edpage = edpg;
//// 24/03/21_税務代理書面 del <--
//
//	return 0;
//}
// ---------------------------------
int PrintZeitempSub::GetInputItemDataSub(int CtIDCh, int CtIDBus, int CtIDNum, PRNEXSUB_INF *SubInf, DIAGRAM_DATA *pdd)
{
	int		chek = 0;
	int		busu = 0;
	int		stpg = 0, edpg = 0;

	pdd->data_imgdata = NULL;

	// チェック
	m_Diag_Zei.GetData(CtIDCh, (LPUNKNOWN)pdd);
	chek = pdd->data_check;

	// 部数
	m_Diag_Zei.GetData(CtIDBus, (LPUNKNOWN)pdd);
	// 1-99以外は受け付けないようにする
	if(_tstoi(pdd->data_edit) >= 1 && _tstoi(pdd->data_edit) <= 99)	{
		busu = _tstoi(pdd->data_edit);
	}

	::ZeroMemory(SubInf, sizeof(PRNEXSUB_INF));

	SubInf->check = chek;
	SubInf->busu = busu;

	return 0;
}
// 24/07/10_個別タブから出力 cor <--

//-----------------------------------------------------------------------------
// 全選択＆全解除
//-----------------------------------------------------------------------------
// 【引　数】	int		0:全選択＆全解除　1:チェックOFFが1箇所でもあるか調べる
// 【戻り値】	BOOL	チェックOFFが1箇所でもあればFALSE
//-----------------------------------------------------------------------------
BOOL PrintZeitempSub::ChoiceItem(int pSw)
{
	BOOL			brv = TRUE;
	SH_ITEMINDEX	*pPrnTbl;
	short			idx = 0;

	if (pSw == 0) {
		// フォーカスOFF
		idx = ((CIcsdiagctrl*)GetDlgItem(IDC_PRN_ZEITEMP_SUB_DG))->GetPosition();
		diag_deledit(IDC_PRN_ZEITEMP_SUB_DG, CIcsdiagctrl);
	}
	int chek = 0;
	DIAGRAM_DATA	dd;
	if (pSw == 1) {
		for (int ii = 0; ii < ZEI_MAXCOL + 1; ii++) {
			int  index = CursorPosZEI[ii][0];
			if (index == -1) break;
			m_Diag_Zei.GetData(index, (LPUNKNOWN)&dd);
			//入力不可を見る
			DIAGRAM_ATTRIBUTE	da;
			diag_getattr(IDC_PRN_ZEITEMP_SUB_DG, CursorPosZEI[ii][0], &da, CIcsdiagctrl);
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

		for (int ii = 0; ii < ZEI_MAXCOL + 1; ii++) {
			dd.data_imgdata = NULL;
			dd.data_check = 0;
			dd.data_combo = 0;
			int  index = CursorPosZEI[ii][0];
			if (index == -1) break;
			m_Diag_Zei.GetData(index, (LPUNKNOWN)&dd);
			//入力不可を見る
			DIAGRAM_ATTRIBUTE	da;
			diag_getattr(IDC_PRN_ZEITEMP_SUB_DG, CursorPosZEI[ii][0], &da, CIcsdiagctrl);
			if (da.attr_bcolor == BC_GRAY) {
				continue;
			}

			if (Itemflg) 	dd.data_check = 0;	//消す
			else			dd.data_check = 1; // 付ける
			m_Diag_Zei.SetData(index, (LPUNKNOWN)&dd);
		}
		m_Diag_Zei.Refresh();
	}
	return(TRUE);
}


void PrintZeitempSub::OnBnClickedZeimudairiname()
{
	m_PoutName= m_Check_Name.GetCheck();


	if (m_PoutName) m_Ctlsw |= 0x01;
	else		    m_Ctlsw &= ~0x01;

}


void PrintZeitempSub::OnBnClickedZeimudairiaddress()
{
	m_PoutAdd = m_Check_Add.GetCheck();

	if (m_PoutAdd) m_Ctlsw |= 0x02;
	else		   m_Ctlsw &= ~0x02;

}


void PrintZeitempSub::OnBnClickedZeimudairiDensi()
{
	m_PoutDensi = m_Check_Den.GetCheck();

	if (m_PoutDensi) m_Ctlsw |= 0x04;
	else			 m_Ctlsw &= ~0x04;
}


void PrintZeitempSub::CheckButtonPrnZeitempSubDg(short index, LPUNKNOWN data)
{
	DIAGRAM_DATA *pdata = (DIAGRAM_DATA*)data;
	DIAGRAM_DATA setdata;
	int getflg = 0;
// 24/03/21_税務代理書面 add -->
	int pos=index/2-1;
// 24/03/21_税務代理書面 add <--
	if (pdata->data_check) {
		for (int ii = 0; ii < ZEI_MAXCOL + 1; ii++) {
			if (CursorPosZEI[ii][0] == index) {
				if (m_pMapPrnEXSub_ZEI.GetCount() > 0) {
					::ZeroMemory(&m_PrnEXSub_ZEI, sizeof(PRNEXSUB_INF));
					m_pMapPrnEXSub_ZEI.Lookup(ii, m_PrnEXSub_ZEI);
					getflg = 1;
					
					break;
				}
			}
		}
// 24/03/21_税務代理書面 cor -->
	//	if(getflg)	setdata.data_edit.Format(_T("%d/1"), m_PrnEXSub_ZEI.busu);
	//	else		setdata.data_edit.Format(_T("0/1"));
	//}
	//else setdata.data_edit.Format(_T("0/1"));
// -----------------------------
		setdata.data_edit.Format(_T("%d/%d"),m_Bunbo[pos],m_Bunbo[pos]);
	}
	else	{
		setdata.data_edit.Format(_T("0/%d"),m_Bunbo[pos]);
	}
// 24/03/21_税務代理書面 cor <--

	m_Diag_Zei.SetData(index + 1, (LPUNKNOWN)&setdata);

// 24/03/22_背景色 add -->
	((CPrnReportDlg*)m_pParent)->BackColorControl(2);
// 24/03/22_背景色 add <--
}


void PrintZeitempSub::ReSetAllNumDg()
{
	DIAGRAM_DATA setdata;
	
	for (int ii = 0; ii < ZEI_MAXCOL + 1; ii++) {
		int index = CursorPosZEI[ii][0];
		if (index == -1) break;
		if (m_pMapPrnEXSub_ZEI.GetCount() > 0) {
			::ZeroMemory(&m_PrnEXSub_ZEI, sizeof(PRNEXSUB_INF));
			m_pMapPrnEXSub_ZEI.Lookup(ii, m_PrnEXSub_ZEI);
// 24/03/21_税務代理書面 cor -->
			//if (m_PrnEXSub_ZEI.check)	setdata.data_edit.Format(_T("%d/1"), m_PrnEXSub_ZEI.busu);
			//else						setdata.data_edit.Format(_T("0/1"));
// -----------------------------
			if (m_PrnEXSub_ZEI.check)	{
				setdata.data_edit.Format(_T("%d/%d"),m_Bunbo[ii],m_Bunbo[ii]);
			}
			else	{
				setdata.data_edit.Format(_T("0/%d"),m_Bunbo[ii]);
			}
// 24/03/21_税務代理書面 cor <--
			m_Diag_Zei.SetData(index + 1, (LPUNKNOWN)&setdata);
		}
	}
}

// 24/03/22_背景色 add -->
// ---------------------------------------------------------------------
// 選択可能なチェックがすべてＯＮになっているかを調べる
// ---------------------------------------------------------------------
// 【引　数】	なし
// 【戻り値】	bool	TRUE:すべてON　FALSE:それ以外
// ---------------------------------------------------------------------
bool PrintZeitempSub::IsAllCheckOn( void )
{
	bool				brv=true;
	int					yy=0;
	DIAGRAM_ATTRIBUTE	da;
	DIAGRAM_DATA		dd;

	while(1)	{
		if(CursorPosZEI[yy][0] == -1)	{
			break;
		}
		diag_getattr(IDC_PRN_ZEITEMP_SUB_DG,CursorPosZEI[yy][0],&da,CIcsdiagctrl);
		if(da.attr_bcolor != BC_GRAY)	{
			m_Diag_Zei.GetData(CursorPosZEI[yy][0],(LPUNKNOWN)&dd);
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
void PrintZeitempSub::AllCheckOn( void )
{
	int					sw=1;
	int					yy=0;
	DIAGRAM_ATTRIBUTE	da;
	DIAGRAM_DATA		dd;

	// 選択できるチェックがすべてOFFの場合のみ実行する
	while(1)	{
		if(CursorPosZEI[yy][0] == -1)	{
			break;
		}
		diag_getattr(IDC_PRN_ZEITEMP_SUB_DG,CursorPosZEI[yy][0],&da,CIcsdiagctrl);
		if(da.attr_bcolor != BC_GRAY)	{
			m_Diag_Zei.GetData(CursorPosZEI[yy][0],(LPUNKNOWN)&dd);
			if(dd.data_check != 0)	{
				sw=0;
			}
		}
		yy++;
	}
	if(sw != 0)	{
		yy=0;
		while(1)	{
			if(CursorPosZEI[yy][0] == -1)	{
				break;
			}
			diag_getattr(IDC_PRN_ZEITEMP_SUB_DG,CursorPosZEI[yy][0],&da,CIcsdiagctrl);
			if(da.attr_bcolor != BC_GRAY)	{
				m_Diag_Zei.GetData(CursorPosZEI[yy][0],(LPUNKNOWN)&dd);
				if(dd.data_check == 0)	{
					dd.data_check = 1;
					m_Diag_Zei.SetData(CursorPosZEI[yy][0],(LPUNKNOWN)&dd);
					CheckButtonPrnZeitempSubDg(CursorPosZEI[yy][0],(LPUNKNOWN)&dd);
				}
			}
			yy++;
		}
	}
}
// 24/04/03_checkon add <--

// 24/04/03_開発員3 add -->
bool PrintZeitempSub::IsDiagEnabled( void )
{
	bool				brv=true;
	int					yy=0;
	DIAGRAM_ATTRIBUTE	da;

	while(1)	{
		if(CursorPosZEI[yy][0] == -1)	{
			break;
		}
		diag_getattr(IDC_PRN_ZEITEMP_SUB_DG,CursorPosZEI[yy][0],&da,CIcsdiagctrl);
		if(da.attr_bcolor != BC_GRAY)	{
			if(da.attr_bcolor == BC_WHITE)	{
				m_Diag_Zei.SetPosition(CursorPosZEI[yy][0]);
				break;
			}
		}
		yy++;
	}
	if(CursorPosZEI[yy][0] == -1)	{
		brv=false;
	}
	return(brv);
}
// 24/04/03_開発員3 add <--
