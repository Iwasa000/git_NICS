// PrintKazeiSub.cpp : 実装ファイル
//

#include "stdafx.h"
//#include "pch.h"
#include "DBSyzShinMain.h"
#include "PrnReportDlg.h"

#include "PrintKazeiSub.h"
// 24/03/21_税務代理書面 del -->
//#include "PrnPrintSubIdx.h"
// 24/03/21_税務代理書面 del <--
#include "afxdialogex.h"

//課税取引金額計算表
extern PRNEXSUB_INF									m_PrnEXSub_KAZ;
extern CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub_KAZ;


short	CursorPosKAZ[KAZ_MAXCOL+1][KAZ_MAXROW+1] =
{
// 24/07/10_個別タブから出力 cor -->
	//{CHEK_KA01,-1,-1},
	//{CHEK_KA02,-1,-1},
	//{CHEK_KA03,-1,-1},
	//{CHEK_KA04,-1,-1},
	//{CHEK_KA05,-1,-1},
	//{-1,-1,-1}
// ---------------------------------
	{CHEK_KA01,CHEK_KA01_BUS,-1,-1},
	{CHEK_KA02,CHEK_KA02_BUS,-1,-1},
	{CHEK_KA03,CHEK_KA03_BUS,-1,-1},
	{CHEK_KA04,CHEK_KA04_BUS,-1,-1},
	{CHEK_KA05,CHEK_KA05_BUS,-1,-1},
	{-1,-1,-1,-1}
// 24/07/10_個別タブから出力 cor <--
};


// PrintKazeiSub

IMPLEMENT_DYNAMIC(PrintKazeiSub, ICSDialog)

PrintKazeiSub::PrintKazeiSub(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_DIALOG_PRN_KAZEI_SUB, pParent)
	, m_pSnHeadData(NULL)
	, m_pZmSub(NULL)
	, m_pShinInfo(NULL)
	, m_pPrintCmInfo(NULL)
	, m_SelSw(0)
	, m_Ctlsw(0)
	, m_KazeiType(0)
{
	m_pParent = pParent;

}

PrintKazeiSub::~PrintKazeiSub()
{
}

void PrintKazeiSub::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRN_KAZEI_SUB_DG, m_Diag_Kaz);
	DDX_Control(pDX, IDC_Kazei_code, m_Check_Kaz);
}


BEGIN_MESSAGE_MAP(PrintKazeiSub, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &PrintKazeiSub::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON7, &PrintKazeiSub::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_KAZ_ALL, &PrintKazeiSub::OnBnClickedKazAll)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_Kazei_code, &PrintKazeiSub::OnBnClickedKazeicode)
	ON_MESSAGE(WM_ACTIVE_SETFORCUS, OnMyFocusControl)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(PrintKazeiSub, ICSDialog)
	ON_EVENT(PrintKazeiSub, IDC_PRN_KAZEI_SUB_DG, 2, PrintKazeiSub::EditOFFPrnKazeiSub, VTS_I2)
	ON_EVENT(PrintKazeiSub, IDC_PRN_KAZEI_SUB_DG, 3, PrintKazeiSub::TerminationPrnKazeiSub, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(PrintKazeiSub, IDC_PRN_KAZEI_SUB_DG, 9, PrintKazeiSub::CheckButtonPrnKazeiSubDg, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()



// PrintKazeiSub メッセージ ハンドラー
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
int PrintKazeiSub::InitInfo(CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, SH_PRINT_CMINFO *pPrintCmInfo, int pBeForked, int Ctlsw)
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




BOOL PrintKazeiSub::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Init();
	m_SelSw = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

LRESULT PrintKazeiSub::OnMyFocusControl(WPARAM w, LPARAM l)
{
	//初期セット
	m_Diag_Kaz.SetFocus();

	//m_Diag_Kaz.SetPosition(CHEK_KA01);
	int					yy=0;
	DIAGRAM_ATTRIBUTE	da;
	while(1)	{
		if(CursorPosKAZ[yy][0] == -1)	{
			break;
		}
		diag_getattr(IDC_PRN_KAZEI_SUB_DG,CursorPosKAZ[yy][0],&da,CIcsdiagctrl);
		if(da.attr_bcolor != BC_GRAY)	{
			if(da.attr_bcolor == BC_WHITE)	{
				m_Diag_Kaz.SetPosition(CursorPosKAZ[yy][0]);
				break;
			}
		}
		yy++;
	}
// 24/03/25_168694 add -->
	if(CursorPosKAZ[yy][0] == -1)	{
		GetDlgItem(IDC_Kazei_code)->SetFocus();
	}
// 24/03/25_168694 add <--
	return 0;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int PrintKazeiSub::Init()
{
	//m_Check_Kaz.SetCheck(m_Ctlsw & 0x01);
	m_Check_Kaz.SetCheck(m_pPrintCmInfo->OPL ? FALSE : TRUE);
	
	// タイトルの背景色
	ChangeColor(IDC_PRN_KAZEI_SUB_DG, TITLE_KA00, 0);
	ChangeColor(IDC_PRN_KAZEI_SUB_DG, TITLE_KA02, 0);	// 24/07/10_個別タブから出力 add
	ChangeColor(IDC_PRN_KAZEI_SUB_DG, TITLE_KA01, 0);

	//タイプ判別
	//int eymd = 0;
	if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) {	// 簡易課税
		//if (!(m_pSnHeadData->Sn_SKKBN % 2)) eymd = m_pSnHeadData->Sn_MDAYE;
		//else								eymd = m_pSnHeadData->Sn_KDAYE;
		//if ((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000)) {	//２割特例計算を使用する
		//	 m_KazeiType = 2;
		//}
		//else m_KazeiType = 1;
		// m_KazeiType = 0:原則課税　1:簡易課税　2:簡易課税（非連動）
		if(m_Util.IsUnConnectKaniMst(m_pZmSub->zvol))	{
			m_KazeiType = 2;
		}
		else	{
			m_KazeiType = 1;
		}
	}

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
	while(m_Yy < KAZ_MAXCOL)	{	// 修正No.168918 add
		diag_getattr(IDC_PRN_KAZEI_SUB_DG, CursorPosKAZ[m_Yy][0], &da, CIcsdiagctrl);
		if(da.attr_bcolor == BC_WHITE)	{
			break;
		}
		m_Yy++;
	}
// 修正No.168895 add <--

	return 0;
}

BOOL PrintKazeiSub::PreTranslateMessage(MSG* pMsg)
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


			if ((id == IDC_PRN_KAZEI_SUB_DG) && (m_Diag_Kaz.GetPosition() == CHEK_KA05)) {
				GetDlgItem(IDC_ZEI_ALL)->SetFocus();
				return(TRUE);
			}
			if ((id == IDC_ZEI_ALL)) {
				GetDlgItem(IDC_Kazei_code)->SetFocus();
				return(TRUE);
			}
// 24/03/25_168692 cor -->
			//if ((id == IDC_Kazei_code) && (pMsg->wParam == VK_TAB)) {
// -----------------------
			if (id == IDC_Kazei_code) {
// 24/03/25_168692 cor <--
				GetDlgItem(IDC_BUTTON2)->SetFocus();
				return(TRUE);
			}
			if ((id == IDC_BUTTON2) && (pMsg->wParam == VK_TAB)) {
				GetDlgItem(IDC_BUTTON7)->SetFocus();
				return(TRUE);
			}
			if ((id == IDC_BUTTON7) && (pMsg->wParam == VK_TAB)) {
				m_Diag_Kaz.SetFocus();
				//m_Diag_Kaz.SetPosition(CHEK_ZE01);	// 24/07/10_個別タブから出力 del
				m_Diag_Kaz.SetPosition(CHEK_KA01);		// 24/07/10_個別タブから出力 add
				return(TRUE);
			}
		}

		if (pMsg->wParam == VK_DELETE) {
			// 法人税と同じようにDeleteキーは反応しないようにする
			return(TRUE);
		}

		if (pMsg->wParam == VK_F9) {
			OnBnClickedKazAll();
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


void PrintKazeiSub::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	ICSDialog::OnCancel();
}


void PrintKazeiSub::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	ICSDialog::OnOK();
}

void PrintKazeiSub::OnBnClickedButton2()
{
	GetParent()->GetParent()->PostMessage(WM_KEYDOWN, VK_F11);
}


void PrintKazeiSub::OnBnClickedButton7()
{
	GetParent()->GetParent()->PostMessage(WM_KEYDOWN, VK_F12);
}


void PrintKazeiSub::OnBnClickedKazAll()
{

	ChoiceItem(0);
	GetInputItemData();
	// 入力項目設定
	SetInputItem();
	//枚数の再セット
	ReSetAllNumDg();
// 24/03/22_背景色 add -->
	((CPrnReportDlg*)m_pParent)->BackColorControl(0);
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
int PrintKazeiSub::SetInputItem(void)
{
	int		ii = 0, jj = 0, eymd = 0;
	char	buf[128] = { 0 };


	for (ii = 0; ii < KAZ_MAXCOL; ii++) {
		if (m_pMapPrnEXSub_KAZ.GetCount() > 0) {
			::ZeroMemory(&m_PrnEXSub_KAZ, sizeof(PRNEXSUB_INF));
			m_pMapPrnEXSub_KAZ.Lookup(ii, m_PrnEXSub_KAZ);
		}

		DIAGRAM_DATA	dd;
		switch (ii) {
// 24/07/10_個別タブから出力 cor -->
		//case 0:		// 課税取引金額計算表　事業所得用 
		//	SetInputItemSub(CHEK_KA01, CHEK_KA01_NUM, &m_PrnEXSub_KAZ, &dd);
		//	break;

		//case 1:		//  課税取引金額計算表　農業所得用 
		//	SetInputItemSub(CHEK_KA02, CHEK_KA02_NUM, &m_PrnEXSub_KAZ, &dd);
		//	break;

		//case 2:		// 課税取引金額計算表　不動産所得用
		//	SetInputItemSub(CHEK_KA03, CHEK_KA03_NUM, &m_PrnEXSub_KAZ, &dd);
		//	break;

		//case 3:		// 課税売上高計算表
		//	SetInputItemSub(CHEK_KA04, CHEK_KA04_NUM, &m_PrnEXSub_KAZ, &dd);
		//	break;

		//case 4:		// 課税仕入高計算表／事業別売上高
		//	SetInputItemSub(CHEK_KA05, CHEK_KA05_NUM, &m_PrnEXSub_KAZ, &dd);
// ---------------------------------
		case 0:		// 課税取引金額計算表　事業所得用
			SetInputItemSub(CHEK_KA01, CHEK_KA01_BUS, CHEK_KA01_NUM, &m_PrnEXSub_KAZ, &dd);
			break;

		case 1:		//  課税取引金額計算表　農業所得用 
			SetInputItemSub(CHEK_KA02, CHEK_KA02_BUS, CHEK_KA02_NUM, &m_PrnEXSub_KAZ, &dd);
			break;

		case 2:		// 課税取引金額計算表　不動産所得用
			SetInputItemSub(CHEK_KA03, CHEK_KA03_BUS, CHEK_KA03_NUM, &m_PrnEXSub_KAZ, &dd);
			break;

		case 3:		// 課税売上高計算表
			SetInputItemSub(CHEK_KA04, CHEK_KA04_BUS, CHEK_KA04_NUM, &m_PrnEXSub_KAZ, &dd);
			break;

		case 4:		// 課税仕入高計算表／事業別売上高
			SetInputItemSub(CHEK_KA05, CHEK_KA05_BUS, CHEK_KA05_NUM, &m_PrnEXSub_KAZ, &dd);
// 24/07/10_個別タブから出力 cor <--
			// 原則課税
			if (m_KazeiType == 0) {
				strcpy_s(buf, sizeof(buf), _T("課税仕入高計算表"));
			}
			// 簡易課税
			else if (m_KazeiType == 1)	{
				strcpy_s(buf, sizeof(buf), _T(""));
			}
			// 簡易課税＆非連動
			else if (m_KazeiType == 2) {
				strcpy_s(buf, sizeof(buf), _T("事業別売上高"));
			}
			diag_getdata(IDC_PRN_KAZEI_SUB_DG, CHEK_KA05, (struct IUnknown *)&dd, CIcsdiagctrl);
			//dd.data_disp = (char *)buf;
			dd.text_check = (char *)buf;

			if(m_KazeiType != 2)	{	// 修正No.168930 add
// 修正No.168682_Awd add -->
				if(!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
				else								eymd = m_pSnHeadData->Sn_KDAYE;
				// 「２割特例計算を使用する」になっていて、チェックがONの場合は、OFFにする
				if((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000))	{
					if(dd.data_check == 1) {
						dd.data_check = 0;
					}
				}
// 修正No.168682_Awd add <--
			}							// 修正No.168930 add

			diag_setdata(IDC_PRN_KAZEI_SUB_DG, CHEK_KA05, (struct IUnknown *)&dd, CIcsdiagctrl);
			break;

		default:
			break;
		}
	}

	m_Diag_Kaz.Refresh();
	
	return(0);
}

// -----------------------------------------------------------------------------
// 入力制限の設定
// -----------------------------------------------------------------------------
// 【引　数】	なし
// 【戻り値】	なし
// -----------------------------------------------------------------------------
int PrintKazeiSub::SetEnableItem(int pSw)
{
	int					ii = 0;
	int					flg = 0;
	long				eymd = 0;	// 修正No.168682_Awd add

//	if (pSw == -1)	ck1 = m_Check1.GetCheck();
//	else			ck1 = pSw;

	//コントロールの制限はなし
	for (ii = 0; ii < KAZ_MAXCOL; ii++) {
		switch (ii) {
		case 0:		// 課税取引金額計算表　事業所得用
			//SetColorItemSub(1,  CHEK_KA01, CHEK_KA01_NUM);
// 修正No.168688 add -->
			if(m_pSnHeadData->IsMiddleProvisional() == FALSE)	{	// 予定26号様式
// 修正No.168688 add <--
				if(m_Util.IsUnConnectKaniMst(m_pZmSub->zvol))	{
					flg=0;
				}
				else	{
					flg=(m_pZmSub->zvol->ind_type & 0x01);
				}
// 修正No.168688 add -->
			}
			else	flg=0;
// 修正No.168688 add <--
			//SetColorItemSub(flg,  CHEK_KA01, CHEK_KA01_NUM);				// 24/07/10_個別タブから出力 del
			SetColorItemSub(flg, CHEK_KA01, CHEK_KA01_BUS, CHEK_KA01_NUM);	// 24/07/10_個別タブから出力 add
			break;

		case 1:		// 課税取引金額計算表　農業所得用 
			//SetColorItemSub(1, CHEK_KA02, CHEK_KA02_NUM);
// 修正No.168688 add -->
			if(m_pSnHeadData->IsMiddleProvisional() == FALSE)	{	// 予定26号様式
// 修正No.168688 add <--
				if(m_Util.IsUnConnectKaniMst(m_pZmSub->zvol))	{
					flg=0;
				}
				else	{
					flg=(m_pZmSub->zvol->ind_type & 0x04);
				}
// 修正No.168688 add -->
			}
			else	flg = 0;
// 修正No.168688 add <--
			//SetColorItemSub(flg, CHEK_KA02, CHEK_KA02_NUM);				// 24/07/10_個別タブから出力 del
			SetColorItemSub(flg, CHEK_KA02, CHEK_KA02_BUS, CHEK_KA02_NUM);	// 24/07/10_個別タブから出力 add
			break;

		case 2:		// 課税取引金額計算表　不動産所得用
			//SetColorItemSub(1, CHEK_KA03, CHEK_KA03_NUM);
// 修正No.168688 add -->
			if(m_pSnHeadData->IsMiddleProvisional() == FALSE)	{	// 予定26号様式
// 修正No.168688 add <--
				if(m_Util.IsUnConnectKaniMst(m_pZmSub->zvol))	{
					flg=0;
				}
				else	{
					flg=(m_pZmSub->zvol->ind_type & 0x02);
				}
// 修正No.168688 add -->
			}
			else	flg = 0;
// 修正No.168688 add <--
			//SetColorItemSub(flg, CHEK_KA03, CHEK_KA03_NUM);				// 24/07/10_個別タブから出力 del
			SetColorItemSub(flg, CHEK_KA03, CHEK_KA03_BUS, CHEK_KA03_NUM);	// 24/07/10_個別タブから出力 add
			break;

		case 3:		// 課税売上高計算表
// 修正No.168688 add -->
			if(m_pSnHeadData->IsMiddleProvisional() == FALSE)	{	// 予定26号様式
// 修正No.168688 add <--
				flg = 1;
// 修正No.168688 add -->
			}
			else	flg = 0;
// 修正No.168688 add <--
			//SetColorItemSub(flg,  CHEK_KA04, CHEK_KA04_NUM);				// 24/07/10_個別タブから出力 del
			SetColorItemSub(flg, CHEK_KA04, CHEK_KA04_BUS, CHEK_KA04_NUM);	// 24/07/10_個別タブから出力 add
			break;

		case 4:		// 課税仕入高計算表／事業別売上高
// 修正No.168688 add -->
			if(m_pSnHeadData->IsMiddleProvisional() == FALSE)	{	// 予定26号様式
// 修正No.168688 add <--
				// 原則課税
				if (m_KazeiType == 0) {
// 修正No.168682_Awd cor -->
					//flg = 1;
// -------------------------
					if(!(m_pSnHeadData->Sn_SKKBN % 2))	eymd = m_pSnHeadData->Sn_MDAYE;
					else								eymd = m_pSnHeadData->Sn_KDAYE;
					if((eymd >= ICS_SH_INVOICE_DAY) && (m_pSnHeadData->m_DispTabSgn & 0x2000))	{	// ２割特例計算を使用する
						flg = 0;
					}
					else	{
						flg = 1;
					}
// 修正No.168682_Awd cor <--
				}
				// 簡易課税
				else if (m_KazeiType == 1) {
					flg = 0;
				}
				// 簡易課税＆非連動
				else if (m_KazeiType == 2) {
					flg = 1;
				}
// 修正No.168688 add -->
			}
			else	flg = 0;
// 修正No.168688 add <--
			//SetColorItemSub(flg, CHEK_KA05, CHEK_KA05_NUM);				// 24/07/10_個別タブから出力 del
			SetColorItemSub(flg, CHEK_KA05, CHEK_KA05_BUS, CHEK_KA05_NUM);	// 24/07/10_個別タブから出力 add
			break;

		default:
			break;
		}
// 24/03/22_168689 cor -->
		//m_Bunbo[ii] = flg;
// -----------------------
		if(flg)	m_Bunbo[ii] = 1;
		else	m_Bunbo[ii] = 0;
// 24/03/22_168689 cor <--
	}

	m_Diag_Kaz.Refresh();
	
	return(0);
}

//-----------------------------------------------------------------------------
// 入力項目取得
//-----------------------------------------------------------------------------
// 引数	
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int PrintKazeiSub::GetInputItemData(void)
{
	int				ii = 0;

	DIAGRAM_DATA	dd;

	for (ii = 0; ii < KAZ_MAXCOL; ii++) {
		// 初期化

		switch (ii) {
		// マップデータの取得
		::ZeroMemory(&m_PrnEXSub_KAZ, sizeof(PRNEXSUB_INF));
		m_pMapPrnEXSub_KAZ.Lookup(ii, m_PrnEXSub_KAZ);

// 24/07/10_個別タブから出力 cor -->
		//case 0:		// 課税取引金額計算表　事業所得用 
		//	GetInputItemDataSub(CHEK_ZE01, CHEK_ZE01_NUM, &m_PrnEXSub_KAZ, &dd);
		//	break;

		//case 1:		// 課税取引金額計算表　農業所得用
		//	GetInputItemDataSub(CHEK_ZE02, CHEK_ZE02_NUM, &m_PrnEXSub_KAZ, &dd);
		//	break;

		//case 2:		// 課税取引金額計算表　不動産所得用 
		//	GetInputItemDataSub(CHEK_ZE03, CHEK_ZE03_NUM, &m_PrnEXSub_KAZ, &dd);
		//	break;

		//case 3:		// 課税売上高計算表
		//	GetInputItemDataSub(CHEK_ZE04, CHEK_ZE04_NUM, &m_PrnEXSub_KAZ, &dd);
		//	break;

		//case 4:		// 課税仕入高計算表／事業別売上高
		//	GetInputItemDataSub(CHEK_ZE05, CHEK_ZE05_NUM, &m_PrnEXSub_KAZ, &dd);
		//	break;
// ---------------------------------
		case 0:		// 課税取引金額計算表　事業所得用 
			GetInputItemDataSub(CHEK_KA01, CHEK_KA01_BUS, CHEK_KA01_NUM, &m_PrnEXSub_KAZ, &dd);
			break;

		case 1:		// 課税取引金額計算表　農業所得用
			GetInputItemDataSub(CHEK_KA02, CHEK_KA02_BUS, CHEK_KA02_NUM, &m_PrnEXSub_KAZ, &dd);
			break;

		case 2:		// 課税取引金額計算表　不動産所得用 
			GetInputItemDataSub(CHEK_KA03, CHEK_KA03_BUS, CHEK_KA03_NUM, &m_PrnEXSub_KAZ, &dd);
			break;

		case 3:		// 課税売上高計算表
			GetInputItemDataSub(CHEK_KA04, CHEK_KA04_BUS, CHEK_KA04_NUM, &m_PrnEXSub_KAZ, &dd);
			break;

		case 4:		// 課税仕入高計算表／事業別売上高
			GetInputItemDataSub(CHEK_KA05, CHEK_KA05_BUS, CHEK_KA05_NUM, &m_PrnEXSub_KAZ, &dd);
			break;
// 24/07/10_個別タブから出力 cor <--

		default:
			break;
		}

		//m_PrnEXSub.check = chek;
		//m_PrnEXSub.busu = busu;
		//m_PrnEXSub.stpage = stpg;
		//m_PrnEXSub.edpage = edpg;

		// 更新
		m_pMapPrnEXSub_KAZ.SetAt(ii, m_PrnEXSub_KAZ);
	}

	return(0);
}


HBRUSH PrintKazeiSub::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void PrintKazeiSub::ChangeColor(unsigned short id, short index, int sign)
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
void PrintKazeiSub::EditOFFPrnKazeiSub(short index)
{
// 24/03/21_税務代理書面 del -->
	//int				edpg;
	//DIAGRAM_DATA	dd, dd2;
	//
	//	// 終了頁を取得
	////	edpg = 999;
	////	CTaxationList31View	*pTaxlist31View = NULL;
	////	pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));
	////	edpg = pTaxlist31View->GetSpecListMaxPage();

	//	dd.data_imgdata = NULL;
	//	m_Diag_Kaz.GetData(index, (LPUNKNOWN)&dd);

	//	switch (index) {
	//		// 部数
	//	case CHEK_KA01_NUM:			// 課税取引金額計算表　事業所得用 
	//	case CHEK_KA02_NUM:			// 課税取引金額計算表　農業所得用 
	//	case CHEK_KA03_NUM:			// 課税取引金額計算表　不動産所得用 
	//	case CHEK_KA04_NUM:			// 課税売上高計算表
	//	case CHEK_KA05_NUM:			// 課税仕入高計算表／事業別売上高

	//		// 1-99以外は受け付けないようにする
	//		if (_tstoi(dd.data_edit) < 1 || _tstoi(dd.data_edit) > 99) {
	//			dd.data_edit = _T("1");
	//			m_Diag_Kaz.SetData(index, (LPUNKNOWN)&dd);
	//		}
	//		break;

	//	default:
	//		break;
	//	}
// 24/03/21_税務代理書面 del <--

// 24/07/10_個別タブから出力 add -->
	int				edpg;
	DIAGRAM_DATA	dd, dd2;

	dd.data_imgdata = NULL;
	m_Diag_Kaz.GetData(index, (LPUNKNOWN)&dd);

	switch(index)	{
		// 部数
		case CHEK_KA01_BUS:			// 課税取引金額計算表　事業所得用 
		case CHEK_KA02_BUS:			// 課税取引金額計算表　農業所得用 
		case CHEK_KA03_BUS:			// 課税取引金額計算表　不動産所得用 
		case CHEK_KA04_BUS:			// 課税売上高計算表
		case CHEK_KA05_BUS:			// 課税仕入高計算表／事業別売上高
			// 1-99以外は受け付けないようにする
			if(_tstoi(dd.data_edit) < 1 || _tstoi(dd.data_edit) > 99)	{
				dd.data_edit = _T("1");
				m_Diag_Kaz.SetData(index, (LPUNKNOWN)&dd);
			}
			break;

		default:
			break;
	}
// 24/07/10_個別タブから出力 add <--
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void PrintKazeiSub::TerminationPrnKazeiSub(short index, short nChar, short length, LPUNKNOWN data)
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
	for (xx = 0; xx < KAZ_MAXROW; xx++) {
		for (yy = 0; yy < KAZ_MAXCOL; yy++) {
			if (CursorPosKAZ[yy][xx] == index) {
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
				if (xx > KAZ_MAXROW) {
					xx = 0;
					yy++;
				}
				if (yy > KAZ_MAXCOL) {
					yy = 0;
				}
			}
			else if (wTerm == VK_F2) {
				xx--;
				if (xx < 0) {
					xx = KAZ_MAXROW;
					yy--;
				}
				if (yy < 0) {
					yy = KAZ_MAXCOL;
				}
			}
			if (CursorPosKAZ[yy][xx] == -1) {
				continue;
			}
			diag_getattr(IDC_PRN_KAZEI_SUB_DG, CursorPosKAZ[yy][xx], &da, CIcsdiagctrl);
			if (da.attr_bcolor == BC_GRAY) {
				continue;
			}
			if (da.attr_bcolor == BC_WHITE) {
				npos = CursorPosKAZ[yy][xx];
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

			if (CursorPosKAZ[yy][xx] == -1) {
				break;
			}
			diag_getattr(IDC_PRN_KAZEI_SUB_DG, CursorPosKAZ[yy][xx], &da, CIcsdiagctrl);
			if (da.attr_bcolor == BC_GRAY) {
				continue;
			}
			if (da.attr_bcolor == BC_WHITE) {
				npos = CursorPosKAZ[yy][xx];
				break;
			}
		}
		break;

	case VK_DOWN:
	case VK_UP:
		while(1) {
			if(wTerm == VK_DOWN)	yy++;
			else if(wTerm == VK_UP)	yy--;

// 修正No.168895 cor -->
			//if (yy < 0)	yy = 0;
// ---------------------
			if(yy < m_Yy)	yy = m_Yy;
// 修正No.168895 cor <--
			if(CursorPosKAZ[yy][xx] == -1) {
				break;
			}

			diag_getattr(IDC_PRN_KAZEI_SUB_DG, CursorPosKAZ[yy][xx], &da, CIcsdiagctrl);
			if(da.attr_bcolor == BC_GRAY) {
				continue;
			}
			if(da.attr_bcolor == BC_WHITE) {
				npos = CursorPosKAZ[yy][xx];
				break;
			}
		}
		break;
	}

	if (npos != -1) {
		//// 最終のインデックスでEnterキーを押下した場合
		//if (wTerm == VK_RETURN && npos == CHEK_KA01) {
		//	// OKボタンへ移動
		//	GetDlgItem(IDOK)->SetFocus();
		//}
		//else {
			m_Diag_Kaz.SetPosition(npos);
		//}
	}
	
}

// 24/07/10_個別タブから出力 cor -->
//int PrintKazeiSub::SetInputItemSub(int CtIDCh, int CtIDNum, PRNEXSUB_INF *SubInf, DIAGRAM_DATA	*pdd)
// ---------------------------------
int PrintKazeiSub::SetInputItemSub(int CtIDCh, int CtIDBus, int CtIDNum, PRNEXSUB_INF *SubInf, DIAGRAM_DATA *pdd)
// 24/07/10_個別タブから出力 cor <--
{
	int		set_num = 0;
	int		pos = 0;		// 24/07/10_個別タブから出力 add

	// データをセット
	pdd->data_imgdata = NULL;
	// チェックON/OFF
// 24/03/21_168685 cor -->
	//if (SubInf->check == 0)	pdd->data_check = 0;
// -----------------------
	DIAGRAM_ATTRIBUTE	da;
	diag_getattr(IDC_PRN_KAZEI_SUB_DG,CtIDCh,&da,CIcsdiagctrl);
	if (SubInf->check == 0 || da.attr_bcolor == BC_GRAY)	{
		pdd->data_check = 0;
	}
// 24/03/21_168685 cor <--
	else {
		pdd->data_check = 1;
		set_num = 1;
	}
	m_Diag_Kaz.SetData(CtIDCh, (LPUNKNOWN)pdd);

// 24/07/10_個別タブから出力 add -->
	// 部数
	pos = CtIDCh / 2 - 1;
	if(m_Bunbo[pos] == 0)	pdd->data_edit.Format(_T("1"));
	else					pdd->data_edit.Format(_T("%d"), SubInf->busu);
	m_Diag_Kaz.SetData(CtIDBus, (LPUNKNOWN)pdd);
// 24/07/10_個別タブから出力 add <--

	// 印刷枚数
	pdd->data_check = 0;
// 24/03/21_税務代理書面 cor -->
	//if(set_num)		pdd->data_edit.Format(_T("%d/1"), SubInf->busu);
	//else			pdd->data_edit.Format(_T("0/1"));
// -----------------------------
	//int pos=CtIDCh/2-1;	// 24/07/10_個別タブから出力 del
	pos = CtIDCh / 2 - 1;	// 24/07/10_個別タブから出力 add
	if(set_num)	pdd->data_edit.Format(_T("%d/%d"),m_Bunbo[pos],m_Bunbo[pos]);
	else		pdd->data_edit.Format(_T("0/%d"),m_Bunbo[pos]);
// 24/03/21_税務代理書面 cor <--
	m_Diag_Kaz.SetData(CtIDNum, (LPUNKNOWN)pdd);

	return 0;
}

//nflgはマスクしたものを渡す
// 24/07/10_個別タブから出力 cor -->
//int PrintKazeiSub::SetColorItemSub(int ck, int CtIDCh, int CtIDNum)
// ---------------------------------
int PrintKazeiSub::SetColorItemSub(int ck, int CtIDCh, int CtIBus, int CtIDNum)
// 24/07/10_個別タブから出力 cor <--
{
	// 入力制限
	if (ck) {
		// 入力可
		// チェック
		ChangeColor(IDC_PRN_KAZEI_SUB_DG, CtIDCh, 1);
		m_Diag_Kaz.ModifyItem(CtIDCh, DIAG_MDFY_EDIT);
// 24/07/10_個別タブから出力 add -->
		// 部数
		ChangeColor(IDC_PRN_KAZEI_SUB_DG, CtIBus, 1);
		m_Diag_Kaz.ModifyItem(CtIBus, DIAG_MDFY_EDIT);
// 24/07/10_個別タブから出力 add <--
	}
	else {
		// 入力不可
		// チェック
		ChangeColor(IDC_PRN_KAZEI_SUB_DG, CtIDCh, 2);
		m_Diag_Kaz.ModifyItem(CtIDCh, DIAG_MDFY_READONLY);
// 24/07/10_個別タブから出力 add -->
		// 部数
		ChangeColor(IDC_PRN_KAZEI_SUB_DG, CtIBus, 2);
		m_Diag_Kaz.ModifyItem(CtIBus, DIAG_MDFY_READONLY);
// 24/07/10_個別タブから出力 add <--
	}

	// 印刷枚数　入力不可
	//ChangeColor(IDC_PRN_KAZEI_SUB_DG, CtIDNum, 2);	// 修正No.168720 del
	ChangeColor(IDC_PRN_KAZEI_SUB_DG, CtIDNum, 6);		// 修正No.168720 add
	m_Diag_Kaz.ModifyItem(CtIDNum, DIAG_MDFY_READONLY);

	return 0;
}

// 24/07/10_個別タブから出力 cor -->
//int PrintKazeiSub::GetInputItemDataSub(int CtIDCh, int CtIDNum, PRNEXSUB_INF *SubInf, DIAGRAM_DATA	*pdd)
//{
//	int		chek = 0;
//	int		busu = 0;
//	int		stpg = 0, edpg = 0;
//
//	pdd->data_imgdata = NULL;
//	// チェック
//	m_Diag_Kaz.GetData(CtIDCh, (LPUNKNOWN)pdd);
//	chek = pdd->data_check;
//// 24/03/21_税務代理書面 del -->
//	//// 枚数
//	//m_Diag_Kaz.GetData(CtIDNum, (LPUNKNOWN)pdd);
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
int PrintKazeiSub::GetInputItemDataSub(int CtIDCh, int CtIDBus, int CtIDNum, PRNEXSUB_INF *SubInf, DIAGRAM_DATA *pdd)
{
	int		chek = 0;
	int		busu = 0;
	int		stpg = 0, edpg = 0;

	pdd->data_imgdata = NULL;
	// チェック
	m_Diag_Kaz.GetData(CtIDCh, (LPUNKNOWN)pdd);
	chek = pdd->data_check;
	
	// 部数
	m_Diag_Kaz.GetData(CtIDBus, (LPUNKNOWN)pdd);
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
BOOL PrintKazeiSub::ChoiceItem(int pSw)
{
	BOOL			brv = TRUE;
	SH_ITEMINDEX	*pPrnTbl;
	short			idx = 0;

	if (pSw == 0) {
		// フォーカスOFF
		idx = ((CIcsdiagctrl*)GetDlgItem(IDC_PRN_KAZEI_SUB_DG))->GetPosition();
		diag_deledit(IDC_PRN_KAZEI_SUB_DG, CIcsdiagctrl);
	}
	
	int chek = 0;
	DIAGRAM_DATA	dd;
	if (pSw == 1) {
		for (int ii = 0; ii < KAZ_MAXCOL + 1; ii++) {
			int  index = CursorPosKAZ[ii][0];
			if (index == -1) break;
			m_Diag_Kaz.GetData(index, (LPUNKNOWN)&dd);
			//入力不可を見る
			DIAGRAM_ATTRIBUTE	da;
			diag_getattr(IDC_PRN_KAZEI_SUB_DG, CursorPosKAZ[ii][0], &da, CIcsdiagctrl);
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

		for (int ii = 0; ii < KAZ_MAXCOL + 1; ii++) {
			dd.data_imgdata = NULL;
			dd.data_check = 0;
			dd.data_combo = 0;
			int  index = CursorPosKAZ[ii][0];
			if (index == -1) break;
			m_Diag_Kaz.GetData(index, (LPUNKNOWN)&dd);
			//入力不可を見る
			DIAGRAM_ATTRIBUTE	da;
			diag_getattr(IDC_PRN_KAZEI_SUB_DG, CursorPosKAZ[ii][0], &da, CIcsdiagctrl);
			if (da.attr_bcolor == BC_GRAY) {
				continue;
			}

			if (Itemflg) 	dd.data_check = 0;	//消す
			else			dd.data_check = 1; // 付ける
			m_Diag_Kaz.SetData(index, (LPUNKNOWN)&dd);
		}
		m_Diag_Kaz.Refresh();
	}
	return(TRUE);
}



void PrintKazeiSub::OnBnClickedKazeicode()
{
	//m_PoutKcode = m_Check_Kaz.GetCheck();
	//if (m_PoutKcode) m_Ctlsw |= 0x01;
	//else		     m_Ctlsw &= ~0x01;

	if(m_pPrintCmInfo->OPL)	m_pPrintCmInfo->OPL = 0;
	else					m_pPrintCmInfo->OPL = 1;
}


void PrintKazeiSub::CheckButtonPrnKazeiSubDg(short index, LPUNKNOWN data)
{
	DIAGRAM_DATA *pdata = (DIAGRAM_DATA*)data;
	DIAGRAM_DATA setdata;
	int getflg = 0;
// 24/03/21_税務代理書面 add -->
	int pos=index/2-1;
// 24/03/21_税務代理書面 add <--
	if (pdata->data_check) {
		for (int ii = 0; ii < KAZ_MAXCOL + 1; ii++) {
			if (CursorPosKAZ[ii][0] == index) {
				if (m_pMapPrnEXSub_KAZ.GetCount() > 0) {
					::ZeroMemory(&m_PrnEXSub_KAZ, sizeof(PRNEXSUB_INF));
					m_pMapPrnEXSub_KAZ.Lookup(ii, m_PrnEXSub_KAZ);
					getflg = 1;

					break;
				}
			}
		}
// 24/03/21_税務代理書面 cor -->
	//	if (getflg)	setdata.data_edit.Format(_T("%d/1"), m_PrnEXSub_KAZ.busu);
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

	m_Diag_Kaz.SetData(index + 1, (LPUNKNOWN)&setdata);

// 24/03/22_背景色 add -->
	((CPrnReportDlg*)m_pParent)->BackColorControl(0);
// 24/03/22_背景色 add <--
}

void PrintKazeiSub::ReSetAllNumDg()
{
	DIAGRAM_DATA setdata;

	for (int ii = 0; ii < KAZ_MAXCOL + 1; ii++) {
		int index = CursorPosKAZ[ii][0];
		if (index == -1) break;
		if (m_pMapPrnEXSub_KAZ.GetCount() > 0) {
			::ZeroMemory(&m_PrnEXSub_KAZ, sizeof(PRNEXSUB_INF));
			m_pMapPrnEXSub_KAZ.Lookup(ii, m_PrnEXSub_KAZ);
// 24/03/21_税務代理書面 cor -->
			//if (m_PrnEXSub_KAZ.check)	setdata.data_edit.Format(_T("%d/1"), m_PrnEXSub_KAZ.busu);
			//else						setdata.data_edit.Format(_T("0/1"));
// -----------------------------
			if (m_PrnEXSub_KAZ.check)	{
				setdata.data_edit.Format(_T("%d/%d"),m_Bunbo[ii],m_Bunbo[ii]);
			}
			else	{
				setdata.data_edit.Format(_T("0/%d"),m_Bunbo[ii]);
			}
// 24/03/21_税務代理書面 cor <--

			m_Diag_Kaz.SetData(index + 1, (LPUNKNOWN)&setdata);
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
bool PrintKazeiSub::IsAllCheckOn( void )
{
	bool				brv=true;
	int					yy=0;
	DIAGRAM_ATTRIBUTE	da;
	DIAGRAM_DATA		dd;

	while(1)	{
		if(CursorPosKAZ[yy][0] == -1)	{
			break;
		}
		diag_getattr(IDC_PRN_KAZEI_SUB_DG,CursorPosKAZ[yy][0],&da,CIcsdiagctrl);
		if(da.attr_bcolor != BC_GRAY)	{
			m_Diag_Kaz.GetData(CursorPosKAZ[yy][0],(LPUNKNOWN)&dd);
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
void PrintKazeiSub::AllCheckOn( void )
{
	int					sw=1;
	int					yy=0;
	DIAGRAM_ATTRIBUTE	da;
	DIAGRAM_DATA		dd;

	// 選択できるチェックがすべてOFFの場合のみ実行する
	while(1)	{
		if(CursorPosKAZ[yy][0] == -1)	{
			break;
		}
		diag_getattr(IDC_PRN_KAZEI_SUB_DG,CursorPosKAZ[yy][0],&da,CIcsdiagctrl);
		if(da.attr_bcolor != BC_GRAY)	{
			m_Diag_Kaz.GetData(CursorPosKAZ[yy][0],(LPUNKNOWN)&dd);
			if(dd.data_check != 0)	{
				sw=0;
			}
		}
		yy++;
	}
	if(sw != 0)	{
		yy=0;
		while(1)	{
			if(CursorPosKAZ[yy][0] == -1)	{
				break;
			}
			diag_getattr(IDC_PRN_KAZEI_SUB_DG,CursorPosKAZ[yy][0],&da,CIcsdiagctrl);
			if(da.attr_bcolor != BC_GRAY)	{
				m_Diag_Kaz.GetData(CursorPosKAZ[yy][0],(LPUNKNOWN)&dd);
				if(dd.data_check == 0)	{
					dd.data_check = 1;
					m_Diag_Kaz.SetData(CursorPosKAZ[yy][0],(LPUNKNOWN)&dd);
					CheckButtonPrnKazeiSubDg(CursorPosKAZ[yy][0],(LPUNKNOWN)&dd);
				}
			}
			yy++;
		}
	}
}
// 24/04/03_checkon add <--
