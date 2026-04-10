// PrnPrint_EX_Sub.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"
#include "PrnPrint_EX_Sub.h"

#include "PrnPrintIdx_EX_Sub.h"

extern PRNEXSUB_INF									m_PrnEXSub;
extern CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub;

// CPrnPrint_EX_Sub ダイアログ

IMPLEMENT_DYNAMIC(CPrnPrint_EX_Sub, ICSDialog)

CPrnPrint_EX_Sub::CPrnPrint_EX_Sub(CWnd* pParent /*=NULL*/)
	: ICSDialog(CPrnPrint_EX_Sub::IDD, pParent)
	, m_pSnHeadData(NULL)
	, m_pZmSub(NULL)
	, m_pShinInfo(NULL)
	, m_pPrintCmInfo(NULL)
	, m_swBeForked(0)
	, m_PrselState(ID_SEP_PRINT_SEL_STATE)
	//, m_isNoneMTForm(FALSE)
{
}

CPrnPrint_EX_Sub::~CPrnPrint_EX_Sub()
{
}

void CPrnPrint_EX_Sub::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRN_EX_SUB_CK1, m_Check1);
	DDX_Control(pDX, IDC_PRN_EX_SUB_DG1, m_Diag1);
}

BEGIN_MESSAGE_MAP(CPrnPrint_EX_Sub, ICSDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CPrnPrint_EX_Sub::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPrnPrint_EX_Sub::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_PRN_EX_SUB_CK1, &CPrnPrint_EX_Sub::OnBnClickedPenExSubck01)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CPrnPrint_EX_Sub, ICSDialog)
	ON_EVENT(CPrnPrint_EX_Sub, IDC_PRN_EX_SUB_DG1, 2, CPrnPrint_EX_Sub::EditOFFPrnExSubdg01, VTS_I2)
	ON_EVENT(CPrnPrint_EX_Sub, IDC_PRN_EX_SUB_DG1, 3, CPrnPrint_EX_Sub::TerminationPrnExSubdg01, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

// CPrnPrint_EX_Sub メッセージ ハンドラー

//-----------------------------------------------------------------------------
// 初期情報セット
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	消費税ヘッダ情報
//		pZmSub		：	公益クラス
//		pShinInfo	：	共通情報
//		pPrintCmInfo：	印刷情報
//		pPRtable	：	部数を考慮した出力帳票
//		pPGtable	：	ページ番号
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CPrnPrint_EX_Sub::InitInfo(CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, 
	SH_PRINT_CMINFO *pPrintCmInfo, EnumIdPrintSelState pPrselState, int pswBeForked)
{
	ASSERT( pSnHeadData );
	if(pSnHeadData == NULL)	{
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pZmSub );
	if(pZmSub == NULL)	{
		return -1;
	}
	m_pZmSub = pZmSub;

	ASSERT( pShinInfo );
	if(pShinInfo == NULL)	{
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT( pPrintCmInfo );
	if(pPrintCmInfo == NULL)	{
		return -1;
	}
	m_pPrintCmInfo = pPrintCmInfo;

	if(pPrselState == NULL) {
		return(-1);
	}
	m_PrselState = pPrselState;

	m_swBeForked = pswBeForked;

	//CH31HyoView	*pH31View = ( CH31HyoView* )( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( H31_HONPYO_WND_IDX ) );
	//m_isNoneMTForm = pH31View->IsNoneMTForm();

	return(0);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BOOL CPrnPrint_EX_Sub::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	
	Init();

	// フォーカスセット
	m_Check1.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BOOL CPrnPrint_EX_Sub::PreTranslateMessage(MSG* pMsg)
{
	DIAGRAM_ATTRIBUTE	da;

	if(pMsg->wParam == VK_F10)		return(TRUE);

	if(pMsg->message == WM_KEYDOWN)	{
		if(GetCombineKeyDown(VK_SHIFT) && VK_F1 <= pMsg->wParam && pMsg->wParam <= VK_F12)	return TRUE;

		if(pMsg->wParam == VK_ESCAPE)	{
			OnBnClickedCancel();
			return(TRUE);
		}

		if( (pMsg->wParam == VK_RETURN) || 
			(pMsg->wParam == VK_TAB && !(::GetKeyState(VK_SHIFT))) )	{
			if(GetFocus()->m_hWnd == m_Check1.m_hWnd)	{
				DIAGRAM_ATTRIBUTE	da;
				// 選択可であれば計算表１のチェックにフォーカスをセットする
				diag_getattr(IDC_PRN_EX_SUB_DG1, CHECK_SUB_01, &da, CIcsdiagctrl);
				if(da.attr_bcolor == BC_WHITE)	{
					// 計算表１のチェックへ移動
					m_Diag1.SetPosition(CHECK_SUB_01);
				}
				else	{
					// OKボタンへ移動
					GetDlgItem(IDOK)->SetFocus();
				}
				return(TRUE);
			}
		}

		if(pMsg->wParam == VK_DELETE)	{
			// 法人税と同じようにDeleteキーは反応しないようにする
			return(TRUE);
		}

		if(pMsg->wParam == VK_END && m_swBeForked)	{
			GetInputItemData();
			GetParent()->GetParent()->PostMessage(WM_KEYDOWN, VK_END);
			return(TRUE);
		}
	}


	return ICSDialog::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CPrnPrint_EX_Sub::Init()
{
	// タイトルの背景色
	ChangeColor(IDC_PRN_EX_SUB_DG1, TITLE_SUB_01, 0);
	ChangeColor(IDC_PRN_EX_SUB_DG1, TITLE_SUB_02, 0);
	ChangeColor(IDC_PRN_EX_SUB_DG1, TITLE_SUB_03, 0);
	ChangeColor(IDC_PRN_EX_SUB_DG1, TITLE_SUB_04, 0);

	// 入力項目設定
	SetInputItem();

	// 入力制限の設定
	SetEnableItem(-1);

	// 環境共通設定の背景色を付ける
	ICSColorInfo	info;
	int				rv=0;
	rv = ((CDBSyzShinMainApp*)AfxGetApp())->GetStanderdColor(info);
	if(rv == 0 && info.m_swOnOff == 1)	{
		m_Diag1.SetBackColor(info.ViewColor());
	}

	return(0);
}

// -----------------------------------------------------------------------------
// 入力項目設定
// -----------------------------------------------------------------------------
// 【引　数】	なし
// 【戻り値】	なし
// -----------------------------------------------------------------------------
int CPrnPrint_EX_Sub::SetInputItem(void)
{
	int					ii=0, jj=0;
	//int					busu;				// 部数
	//int					stpg, edpg;			// 開始頁と終了頁
	//CString				strSQL;
	//CString				sData;
	//CRecordset			rs(m_pZmSub->m_database);

	for(ii=0; ii<16; ii++)	{
		if(m_pMapPrnEXSub.GetCount() > 0)	{
			::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));
			m_pMapPrnEXSub.Lookup(ii, m_PrnEXSub);
		}

		DIAGRAM_DATA	dd;
		switch(ii)	{
			case 0:		// 個別に出力設定を行う
				if(m_PrnEXSub.check == 0)	m_Check1.SetCheck(0);
				else						m_Check1.SetCheck(1);
				break;

			case 1:		// 計算表１
				// データをセット
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_01, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_01, (LPUNKNOWN)&dd);
				// 開始頁 ※なし
				// 終了頁 ※なし
				break;

			case 2:		// 計算表２(１)
				// データをセット
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_02, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_02, (LPUNKNOWN)&dd);
				// 開始頁 ※なし
				// 終了頁 ※なし
				break;

			case 3:		// 計算表２(２)
				// データをセット
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_03, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_03, (LPUNKNOWN)&dd);
				// 開始頁 ※なし
				// 終了頁 ※なし
				break;

			case 4:		// 計算表３、４
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_04, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_04, (LPUNKNOWN)&dd);
				// 開始頁 ※なし
				// 終了頁 ※なし
				break;

			case 5:		// 計算表５(１)
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_05, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_05, (LPUNKNOWN)&dd);
				// 開始頁 ※なし
				// 終了頁 ※なし
				break;

			case 6:		// 計算表５(２)
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_06, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_06, (LPUNKNOWN)&dd);
				// 開始頁 ※なし
				// 終了頁 ※なし
				break;

			case 7:		// 計算表５(３)
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_07, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_07, (LPUNKNOWN)&dd);
				// 開始頁 ※なし
				// 終了頁 ※なし
				break;

			case 8:		// 計算表５－２(１)
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_08, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_08, (LPUNKNOWN)&dd);
				// 開始頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.stpage);
				m_Diag1.SetData(STPG_SUB_08, (LPUNKNOWN)&dd);
				// 終了頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.edpage);
				m_Diag1.SetData(EDPG_SUB_08, (LPUNKNOWN)&dd);
				break;

			case 9:		// 計算表５－２(２)
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_09, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_09, (LPUNKNOWN)&dd);
				// 開始頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.stpage);
				m_Diag1.SetData(STPG_SUB_09, (LPUNKNOWN)&dd);
				// 終了頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.edpage);
				m_Diag1.SetData(EDPG_SUB_09, (LPUNKNOWN)&dd);
				break;

			case 10:	// 計算表５－２(３)－１
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_10, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_10, (LPUNKNOWN)&dd);
				// 開始頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.stpage);
				m_Diag1.SetData(STPG_SUB_10, (LPUNKNOWN)&dd);
				// 終了頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.edpage);
				m_Diag1.SetData(EDPG_SUB_10, (LPUNKNOWN)&dd);
				break;

			case 11:	// 計算表５－２(３)－２
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_11, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_11, (LPUNKNOWN)&dd);
				// 開始頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.stpage);
				m_Diag1.SetData(STPG_SUB_11, (LPUNKNOWN)&dd);
				// 終了頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.edpage);
				m_Diag1.SetData(EDPG_SUB_11, (LPUNKNOWN)&dd);
				break;

			case 12:	// 計算表５－２(３)－３
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_12, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_12, (LPUNKNOWN)&dd);
				// 開始頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.stpage);
				m_Diag1.SetData(STPG_SUB_12, (LPUNKNOWN)&dd);
				// 終了頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.edpage);
				m_Diag1.SetData(EDPG_SUB_12, (LPUNKNOWN)&dd);
				break;

			case 13:	// 計算表５－２(４)－１
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_13, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_13, (LPUNKNOWN)&dd);
				// 開始頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.stpage);
				m_Diag1.SetData(STPG_SUB_13, (LPUNKNOWN)&dd);
				// 終了頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.edpage);
				m_Diag1.SetData(EDPG_SUB_13, (LPUNKNOWN)&dd);
				break;

			case 14:	// 計算表５－２(４)－２
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_14, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_14, (LPUNKNOWN)&dd);
				// 開始頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.stpage);
				m_Diag1.SetData(STPG_SUB_14, (LPUNKNOWN)&dd);
				// 終了頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.edpage);
				m_Diag1.SetData(EDPG_SUB_14, (LPUNKNOWN)&dd);
				break;

			case 15:	// 計算表５－２(４)－３
				dd.data_imgdata = NULL;
				// チェックON/OFF
				if(m_PrnEXSub.check == 0)	dd.data_check = 0;
				else						dd.data_check = 1;
				m_Diag1.SetData(CHECK_SUB_15, (LPUNKNOWN)&dd);
				// 部数
				dd.data_check = 0;
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.busu);
				m_Diag1.SetData(BUSU_SUB_15, (LPUNKNOWN)&dd);
				// 開始頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.stpage);
				m_Diag1.SetData(STPG_SUB_15, (LPUNKNOWN)&dd);
				// 終了頁
				dd.data_edit.Format(_T("%d"), m_PrnEXSub.edpage);
				m_Diag1.SetData(EDPG_SUB_15, (LPUNKNOWN)&dd);
				break;

			default:
				break;
		}
	}

	m_Diag1.Refresh();

	return(0);
}

// -----------------------------------------------------------------------------
// 入力制限の設定
// -----------------------------------------------------------------------------
// 【引　数】	なし
// 【戻り値】	なし
// -----------------------------------------------------------------------------
int CPrnPrint_EX_Sub::SetEnableItem(int pSw)
{
	int					ii=0;
	int					ck1;				// 個別に出力設定を行うの設定
	long				flg;				// 入力制限サイン

	CTaxationList31View	*pTaxlist31View = NULL;
	pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));

	// 入力制限のサインをセット
	pTaxlist31View->IsSpecificEnable(&flg);

	if(pSw == -1)	ck1 = m_Check1.GetCheck();
	else			ck1 = pSw;

	for(ii=0; ii<16; ii++)	{
		switch(ii)	{
			case 0:		// 個別に出力設定を行う
				break;

			case 1:		// 計算表１
				// 入力制限
				if(ck1 && (flg & 0x0001))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_01, 1);
					m_Diag1.ModifyItem(CHECK_SUB_01, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_01, 1);
					m_Diag1.ModifyItem(BUSU_SUB_01, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_01, 2);
					m_Diag1.ModifyItem(CHECK_SUB_01, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_01, 2);
					m_Diag1.ModifyItem(BUSU_SUB_01, DIAG_MDFY_READONLY);
				}
				// 開始頁 入力不可
				ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_01, 2);
				m_Diag1.ModifyItem(STPG_SUB_01, DIAG_MDFY_READONLY);
				// 終了頁 入力不可
				ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_01, 2);
				m_Diag1.ModifyItem(EDPG_SUB_01, DIAG_MDFY_READONLY);
				break;

			case 2:		// 計算表２(１)
				// 入力制限
				if(ck1 && (flg & 0x0002))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_02, 1);
					m_Diag1.ModifyItem(CHECK_SUB_02, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_02, 1);
					m_Diag1.ModifyItem(BUSU_SUB_02, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_02, 2);
					m_Diag1.ModifyItem(CHECK_SUB_02, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_02, 2);
					m_Diag1.ModifyItem(BUSU_SUB_02, DIAG_MDFY_READONLY);
				}
				// 開始頁 入力不可
				ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_02, 2);
				m_Diag1.ModifyItem(STPG_SUB_02, DIAG_MDFY_READONLY);
				// 終了頁 入力不可
				ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_02, 2);
				m_Diag1.ModifyItem(EDPG_SUB_02, DIAG_MDFY_READONLY);
				break;

			case 3:		// 計算表２(２)
				// 入力制限
				if(ck1 && (flg & 0x0004))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_03, 1);
					m_Diag1.ModifyItem(CHECK_SUB_03, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_03, 1);
					m_Diag1.ModifyItem(BUSU_SUB_03, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_03, 2);
					m_Diag1.ModifyItem(CHECK_SUB_03, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_03, 2);
					m_Diag1.ModifyItem(BUSU_SUB_03, DIAG_MDFY_READONLY);
				}
				// 開始頁 入力不可
				ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_03, 2);
				m_Diag1.ModifyItem(STPG_SUB_03, DIAG_MDFY_READONLY);
				// 終了頁 入力不可
				ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_03, 2);
				m_Diag1.ModifyItem(EDPG_SUB_03, DIAG_MDFY_READONLY);
				break;

			case 4:		// 計算表３、４
				// 入力制限
				if(ck1 && (flg & 0x0008))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_04, 1);
					m_Diag1.ModifyItem(CHECK_SUB_04, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_04, 1);
					m_Diag1.ModifyItem(BUSU_SUB_04, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_04, 2);
					m_Diag1.ModifyItem(CHECK_SUB_04, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_04, 2);
					m_Diag1.ModifyItem(BUSU_SUB_04, DIAG_MDFY_READONLY);
				}
				// 開始頁 入力不可
				ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_04, 2);
				m_Diag1.ModifyItem(STPG_SUB_04, DIAG_MDFY_READONLY);
				// 終了頁 入力不可
				ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_04, 2);
				m_Diag1.ModifyItem(EDPG_SUB_04, DIAG_MDFY_READONLY);
				break;

			case 5:		// 計算表５(１)
				// 入力制限
				if(ck1 && (flg & 0x0010))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_05, 1);
					m_Diag1.ModifyItem(CHECK_SUB_05, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_05, 1);
					m_Diag1.ModifyItem(BUSU_SUB_05, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_05, 2);
					m_Diag1.ModifyItem(CHECK_SUB_05, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_05, 2);
					m_Diag1.ModifyItem(BUSU_SUB_05, DIAG_MDFY_READONLY);
				}
				// 開始頁 入力不可
				ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_05, 2);
				m_Diag1.ModifyItem(STPG_SUB_05, DIAG_MDFY_READONLY);
				// 終了頁 入力不可
				ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_05, 2);
				m_Diag1.ModifyItem(EDPG_SUB_05, DIAG_MDFY_READONLY);
				break;

			case 6:		// 計算表５(２)
				// 入力制限
				if(ck1 && (flg & 0x0020))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_06, 1);
					m_Diag1.ModifyItem(CHECK_SUB_06, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_06, 1);
					m_Diag1.ModifyItem(BUSU_SUB_06, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_06, 2);
					m_Diag1.ModifyItem(CHECK_SUB_06, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_06, 2);
					m_Diag1.ModifyItem(BUSU_SUB_06, DIAG_MDFY_READONLY);
				}
				// 開始頁 入力不可
				ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_06, 2);
				m_Diag1.ModifyItem(STPG_SUB_06, DIAG_MDFY_READONLY);
				// 終了頁 入力不可
				ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_06, 2);
				m_Diag1.ModifyItem(EDPG_SUB_06, DIAG_MDFY_READONLY);
				break;

			case 7:		// 計算表５(３)
				// 入力制限
				if(ck1 && (flg & 0x0040))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_07, 1);
					m_Diag1.ModifyItem(CHECK_SUB_07, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_07, 1);
					m_Diag1.ModifyItem(BUSU_SUB_07, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_07, 2);
					m_Diag1.ModifyItem(CHECK_SUB_07, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_07, 2);
					m_Diag1.ModifyItem(BUSU_SUB_07, DIAG_MDFY_READONLY);
				}
				// 開始頁 入力不可
				ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_07, 2);
				m_Diag1.ModifyItem(STPG_SUB_07, DIAG_MDFY_READONLY);
				// 終了頁 入力不可
				ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_07, 2);
				m_Diag1.ModifyItem(EDPG_SUB_07, DIAG_MDFY_READONLY);
				break;

			case 8:		// 計算表５－２(１)
				// 入力制限
				if(ck1 && (flg & 0x0080))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_08, 1);
					m_Diag1.ModifyItem(CHECK_SUB_08, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_08, 1);
					m_Diag1.ModifyItem(BUSU_SUB_08, DIAG_MDFY_EDIT);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_08, 1);
					m_Diag1.ModifyItem(STPG_SUB_08, DIAG_MDFY_EDIT);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_08, 1);
					m_Diag1.ModifyItem(EDPG_SUB_08, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_08, 2);
					m_Diag1.ModifyItem(CHECK_SUB_08, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_08, 2);
					m_Diag1.ModifyItem(BUSU_SUB_08, DIAG_MDFY_READONLY);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_08, 2);
					m_Diag1.ModifyItem(STPG_SUB_08, DIAG_MDFY_READONLY);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_08, 2);
					m_Diag1.ModifyItem(EDPG_SUB_08, DIAG_MDFY_READONLY);
				}
				break;

			case 9:		// 計算表５－２(２)
				// 入力制限
				if(ck1 && (flg & 0x0100))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_09, 1);
					m_Diag1.ModifyItem(CHECK_SUB_09, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_09, 1);
					m_Diag1.ModifyItem(BUSU_SUB_09, DIAG_MDFY_EDIT);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_09, 1);
					m_Diag1.ModifyItem(STPG_SUB_09, DIAG_MDFY_EDIT);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_09, 1);
					m_Diag1.ModifyItem(EDPG_SUB_09, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_09, 2);
					m_Diag1.ModifyItem(CHECK_SUB_09, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_09, 2);
					m_Diag1.ModifyItem(BUSU_SUB_09, DIAG_MDFY_READONLY);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_09, 2);
					m_Diag1.ModifyItem(STPG_SUB_09, DIAG_MDFY_READONLY);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_09, 2);
					m_Diag1.ModifyItem(EDPG_SUB_09, DIAG_MDFY_READONLY);
				}
				break;

			case 10:	// 計算表５－２(３)－１
				// 入力制限
				if(ck1 && (flg & 0x0200))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_10, 1);
					m_Diag1.ModifyItem(CHECK_SUB_10, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_10, 1);
					m_Diag1.ModifyItem(BUSU_SUB_10, DIAG_MDFY_EDIT);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_10, 1);
					m_Diag1.ModifyItem(STPG_SUB_10, DIAG_MDFY_EDIT);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_10, 1);
					m_Diag1.ModifyItem(EDPG_SUB_10, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_10, 2);
					m_Diag1.ModifyItem(CHECK_SUB_10, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_10, 2);
					m_Diag1.ModifyItem(BUSU_SUB_10, DIAG_MDFY_READONLY);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_10, 2);
					m_Diag1.ModifyItem(STPG_SUB_10, DIAG_MDFY_READONLY);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_10, 2);
					m_Diag1.ModifyItem(EDPG_SUB_10, DIAG_MDFY_READONLY);
				}
				break;

			case 11:	// 計算表５－２(３)－２
				// 入力制限
				if(ck1 && (flg & 0x0400))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_11, 1);
					m_Diag1.ModifyItem(CHECK_SUB_11, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_11, 1);
					m_Diag1.ModifyItem(BUSU_SUB_11, DIAG_MDFY_EDIT);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_11, 1);
					m_Diag1.ModifyItem(STPG_SUB_11, DIAG_MDFY_EDIT);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_11, 1);
					m_Diag1.ModifyItem(EDPG_SUB_11, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_11, 2);
					m_Diag1.ModifyItem(CHECK_SUB_11, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_11, 2);
					m_Diag1.ModifyItem(BUSU_SUB_11, DIAG_MDFY_READONLY);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_11, 2);
					m_Diag1.ModifyItem(STPG_SUB_11, DIAG_MDFY_READONLY);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_11, 2);
					m_Diag1.ModifyItem(EDPG_SUB_11, DIAG_MDFY_READONLY);
				}
				break;

			case 12:	// 計算表５－２(３)－３
				// 入力制限
				if(ck1 && (flg & 0x0800))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_12, 1);
					m_Diag1.ModifyItem(CHECK_SUB_12, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_12, 1);
					m_Diag1.ModifyItem(BUSU_SUB_12, DIAG_MDFY_EDIT);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_12, 1);
					m_Diag1.ModifyItem(STPG_SUB_12, DIAG_MDFY_EDIT);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_12, 1);
					m_Diag1.ModifyItem(EDPG_SUB_12, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_12, 2);
					m_Diag1.ModifyItem(CHECK_SUB_12, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_12, 2);
					m_Diag1.ModifyItem(BUSU_SUB_12, DIAG_MDFY_READONLY);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_12, 2);
					m_Diag1.ModifyItem(STPG_SUB_12, DIAG_MDFY_READONLY);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_12, 2);
					m_Diag1.ModifyItem(EDPG_SUB_12, DIAG_MDFY_READONLY);
				}
				break;

			case 13:	// 計算表５－２(４)－１
				// 入力制限
				if(ck1 && (flg & 0x1000))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_13, 1);
					m_Diag1.ModifyItem(CHECK_SUB_13, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_13, 1);
					m_Diag1.ModifyItem(BUSU_SUB_13, DIAG_MDFY_EDIT);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_13, 1);
					m_Diag1.ModifyItem(STPG_SUB_13, DIAG_MDFY_EDIT);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_13, 1);
					m_Diag1.ModifyItem(EDPG_SUB_13, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_13, 2);
					m_Diag1.ModifyItem(CHECK_SUB_13, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_13, 2);
					m_Diag1.ModifyItem(BUSU_SUB_13, DIAG_MDFY_READONLY);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_13, 2);
					m_Diag1.ModifyItem(STPG_SUB_13, DIAG_MDFY_READONLY);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_13, 2);
					m_Diag1.ModifyItem(EDPG_SUB_13, DIAG_MDFY_READONLY);
				}
				break;

			case 14:	// 計算表５－２(４)－２
				// 入力制限
				if(ck1 && (flg & 0x2000))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_14, 1);
					m_Diag1.ModifyItem(CHECK_SUB_14, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_14, 1);
					m_Diag1.ModifyItem(BUSU_SUB_14, DIAG_MDFY_EDIT);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_14, 1);
					m_Diag1.ModifyItem(STPG_SUB_14, DIAG_MDFY_EDIT);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_14, 1);
					m_Diag1.ModifyItem(EDPG_SUB_14, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_14, 2);
					m_Diag1.ModifyItem(CHECK_SUB_14, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_14, 2);
					m_Diag1.ModifyItem(BUSU_SUB_14, DIAG_MDFY_READONLY);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_14, 2);
					m_Diag1.ModifyItem(STPG_SUB_14, DIAG_MDFY_READONLY);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_14, 2);
					m_Diag1.ModifyItem(EDPG_SUB_14, DIAG_MDFY_READONLY);
				}
				break;

			case 15:	// 計算表５－２(４)－３
				// 入力制限
				if(ck1 && (flg & 0x4000))	{
					// 入力可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_15, 1);
					m_Diag1.ModifyItem(CHECK_SUB_15, DIAG_MDFY_EDIT);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_15, 1);
					m_Diag1.ModifyItem(BUSU_SUB_15, DIAG_MDFY_EDIT);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_15, 1);
					m_Diag1.ModifyItem(STPG_SUB_15, DIAG_MDFY_EDIT);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_15, 1);
					m_Diag1.ModifyItem(EDPG_SUB_15, DIAG_MDFY_EDIT);
				}
				else	{
					// 入力不可
					// チェック
					ChangeColor(IDC_PRN_EX_SUB_DG1, CHECK_SUB_15, 2);
					m_Diag1.ModifyItem(CHECK_SUB_15, DIAG_MDFY_READONLY);
					// 部数
					ChangeColor(IDC_PRN_EX_SUB_DG1, BUSU_SUB_15, 2);
					m_Diag1.ModifyItem(BUSU_SUB_15, DIAG_MDFY_READONLY);
					// 開始頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, STPG_SUB_15, 2);
					m_Diag1.ModifyItem(STPG_SUB_15, DIAG_MDFY_READONLY);
					// 終了頁
					ChangeColor(IDC_PRN_EX_SUB_DG1, EDPG_SUB_15, 2);
					m_Diag1.ModifyItem(EDPG_SUB_15, DIAG_MDFY_READONLY);
				}
				break;

			default:
				break;
		}
	}

	m_Diag1.Refresh();

	return(0);
}

//-----------------------------------------------------------------------------
// 入力項目取得
//-----------------------------------------------------------------------------
// 引数	
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CPrnPrint_EX_Sub::GetInputItemData(void)
{
	int				ii=0;
	int				chek;
	int				busu;
	int				stpg, edpg;
	//CString			strSQL;
	DIAGRAM_DATA	dd;
	//CDatabase*		db = m_pZmSub->m_database;

	for(ii=0; ii<16; ii++)	{
		// 初期化
		chek = 0;
		busu = NULL;
		stpg = edpg = NULL;

		switch(ii) {
			case 0:		// 個別に出力設定を行う
				chek  = m_Check1.GetCheck();
				break;

			case 1:		// 計算表１
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_01, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_01, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_01, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_01, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			case 2:		// 計算表２(１)
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_02, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_02, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_02, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_02, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			case 3:		// 計算表２(２)
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_03, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_03, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_03, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_03, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			case 4:		// 計算表３、４
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_04, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_04, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_04, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_04, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			case 5:		// 計算表５(１)
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_05, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_05, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_05, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_05, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			case 6:		// 計算表５(２)
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_06, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_06, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_06, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_06, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			case 7:		// 計算表５(３)
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_07, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_07, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_07, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_07, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			case 8:		// 計算表５－２(１)
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_08, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_08, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_08, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_08, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			case 9:		// 計算表５－２(２)
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_09, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_09, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_09, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_09, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			case 10:	// 計算表５－２(３)－１
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_10, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_10, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_10, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_10, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			case 11:	// 計算表５－２(３)－２
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_11, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_11, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_11, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_11, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			case 12:	// 計算表５－２(３)－３
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_12, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_12, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_12, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_12, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			case 13:	// 計算表５－２(４)－１
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_13, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_13, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_13, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_13, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			case 14:	// 計算表５－２(４)－２
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_14, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_14, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_14, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_14, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			case 15:	// 計算表５－２(４)－３
				dd.data_imgdata = NULL;
				// チェック
				m_Diag1.GetData(CHECK_SUB_15, (LPUNKNOWN)&dd);
				chek = dd.data_check;
				// 部数
				m_Diag1.GetData(BUSU_SUB_15, (LPUNKNOWN)&dd);
				// 1-99以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 99)	{
					busu = _tstoi(dd.data_edit);
				}
				// 開始頁
				m_Diag1.GetData(STPG_SUB_15, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					stpg = _tstoi(dd.data_edit);
				}
				// 終了頁
				m_Diag1.GetData(EDPG_SUB_15, (LPUNKNOWN)&dd);
				// 1-999以外は受け付けないようにする
				if(_tstoi(dd.data_edit) >= 1 && _tstoi(dd.data_edit) <= 999)	{
					edpg = _tstoi(dd.data_edit);
				}
				break;

			default:
				break;
		}

		// マップデータの取得
		::ZeroMemory(&m_PrnEXSub, sizeof(PRNEXSUB_INF));
		m_pMapPrnEXSub.Lookup(ii, m_PrnEXSub);

		m_PrnEXSub.check = chek;
		m_PrnEXSub.busu = busu;
		m_PrnEXSub.stpage = stpg;
		m_PrnEXSub.edpage = edpg;

		// 更新
		m_pMapPrnEXSub.SetAt(ii, m_PrnEXSub);
	}

	return(0);
}

//-----------------------------------------------------------------------------
// 入力項目色変更
//-----------------------------------------------------------------------------
// 引数	id		：
//		index	：
//		sign	：
//-----------------------------------------------------------------------------
void CPrnPrint_EX_Sub::ChangeColor(unsigned short id, short index, int sign)
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr(id, index, &DA, CIcsdiagctrl);
	switch(sign)	{
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
		default:
			DA.attr_bcolor = BC_CREAM;
			break;		// 実額入力	（クリーム色） 
	}
	diag_setattr(id, index, &DA, FALSE, CIcsdiagctrl);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
HBRUSH CPrnPrint_EX_Sub::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if(GetDlgItem(IDC_STATIC_INCOME) == pWnd)
		pDC->SetTextColor(RGB(255,0,0));

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CPrnPrint_EX_Sub::EditOFFPrnExSubdg01(short index)
{
	int				edpg;
	DIAGRAM_DATA	dd,dd2;

	// 終了頁を取得
	edpg = 999;
	CTaxationList31View	*pTaxlist31View = NULL;
	pTaxlist31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(4));
	edpg = pTaxlist31View->GetSpecListMaxPage();

	dd.data_imgdata = NULL;
	m_Diag1.GetData(index, (LPUNKNOWN)&dd);

	switch(index)	{
		// 部数
		case BUSU_SUB_01:			// 計画表１
		case BUSU_SUB_02:			// 計画表２(１)
		case BUSU_SUB_03:			// 計画表２(２)
		case BUSU_SUB_04:			// 計画表３、４
		case BUSU_SUB_05:			// 計画表５(１)
		case BUSU_SUB_06:			// 計画表５(２)
		case BUSU_SUB_07:			// 計画表５(３)
		case BUSU_SUB_08:			// 計画表５－２(１)
		case BUSU_SUB_09:			// 計画表５－２(２)
		case BUSU_SUB_10:			// 計画表５－２(３)－１
		case BUSU_SUB_11:			// 計画表５－２(３)－２
		case BUSU_SUB_12:			// 計画表５－２(３)－３
		case BUSU_SUB_13:			// 計画表５－２(４)－１
		case BUSU_SUB_14:			// 計画表５－２(４)－２
		case BUSU_SUB_15:			// 計画表５－２(４)－３
			// 1-99以外は受け付けないようにする
			if(_tstoi(dd.data_edit) < 1 || _tstoi(dd.data_edit) > 99)	{
				dd.data_edit = _T("1");
				m_Diag1.SetData(index,(LPUNKNOWN)&dd);
			}
			break;

		// 開始頁
		case STPG_SUB_01:			// 計画表１
		case STPG_SUB_02:			// 計画表２(１)
		case STPG_SUB_03:			// 計画表２(２)
		case STPG_SUB_04:			// 計画表３、４
		case STPG_SUB_05:			// 計画表５(１)
		case STPG_SUB_06:			// 計画表５(２)
		case STPG_SUB_07:			// 計画表５(３)
		case STPG_SUB_08:			// 計画表５－２(１)
		case STPG_SUB_09:			// 計画表５－２(２)
		case STPG_SUB_10:			// 計画表５－２(３)－１
		case STPG_SUB_11:			// 計画表５－２(３)－２
		case STPG_SUB_12:			// 計画表５－２(３)－３
		case STPG_SUB_13:			// 計画表５－２(４)－１
		case STPG_SUB_14:			// 計画表５－２(４)－２
		case STPG_SUB_15:			// 計画表５－２(４)－３
			// 1-999以外は受け付けないようにする
			if(_tstoi(dd.data_edit) < 1 || _tstoi(dd.data_edit) > 999)	{
				dd.data_edit = _T("1");
				m_Diag1.SetData(index, (LPUNKNOWN)&dd);
			}
			// 最終ページを超えないようにする
			if(_tstoi(dd.data_edit) > edpg)		{
				dd.data_edit = _T("1");
				m_Diag1.SetData(index, (LPUNKNOWN)&dd);
			}
			break;

		// 終了頁
		case EDPG_SUB_01:			// 計画表１
		case EDPG_SUB_02:			// 計画表２(１)
		case EDPG_SUB_03:			// 計画表２(２)
		case EDPG_SUB_04:			// 計画表３、４
		case EDPG_SUB_05:			// 計画表５(１)
		case EDPG_SUB_06:			// 計画表５(２)
		case EDPG_SUB_07:			// 計画表５(３)
		case EDPG_SUB_08:			// 計画表５－２(１)
		case EDPG_SUB_09:			// 計画表５－２(２)
		case EDPG_SUB_10:			// 計画表５－２(３)－１
		case EDPG_SUB_11:			// 計画表５－２(３)－２
		case EDPG_SUB_12:			// 計画表５－２(３)－３
		case EDPG_SUB_13:			// 計画表５－２(４)－１
		case EDPG_SUB_14:			// 計画表５－２(４)－２
		case EDPG_SUB_15:			// 計画表５－２(４)－３
			// 1-999以外は受け付けないようにする
			if(_tstoi(dd.data_edit) < 1 || _tstoi(dd.data_edit) > 999)	{
				dd.data_edit.Format(_T("%d"), edpg);
				m_Diag1.SetData(index, (LPUNKNOWN)&dd);
			}
			// 最終ページを超えないようにする
			if(_tstoi(dd.data_edit) > edpg)	{
				dd.data_edit.Format(_T("%d"), edpg);
				m_Diag1.SetData(index, (LPUNKNOWN)&dd);
			}
			// 開始頁に入力された数値を超えないようにする
			m_Diag1.GetData(index-16, (LPUNKNOWN)&dd2);
			if(_tstoi(dd.data_edit) < _tstoi(dd2.data_edit))	{
				dd.data_edit.Format(_T("%d"), edpg);
				m_Diag1.SetData(index, (LPUNKNOWN)&dd);
			}
			break;

		default:
			break;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CPrnPrint_EX_Sub::TerminationPrnExSubdg01(short index, short nChar, short length, LPUNKNOWN data)
{
	int					xx=0,yy=0;
	short				sw=0;
	short				wTerm=0;
	short				npos=-1;
	char				VK_FLG=0;
	DIAGRAM_ATTRIBUTE	da;

	if(nChar == 0)	{
		return;
	}

	VK_FLG = (char)::GetKeyState(VK_SHIFT);
	if(nChar == VK_TAB)	{
		if(VK_FLG & 0x80)		wTerm = VK_F2;
		else					wTerm = VK_RETURN;
	}
	else						wTerm = nChar;

	// ポジションの2次元配列から現在の場所を探す
	sw=0;
	for(xx=0; xx<5; xx++)	{
		for(yy=0; yy<16; yy++)	{
			if(CursorPos[yy][xx] == index)	{
				sw=1;
				break;
			}
		}
		if(sw != 0)	break;
	}
	if(sw == 0)	{
		return;	// ありえない
	}
	
	switch(wTerm)	{
		case VK_RETURN:
		case VK_F2:
			while(1)	{
				if(wTerm == VK_RETURN)	{
					xx++;
					if(xx >= 5)	{
						xx = 0;
						yy++;
					}
					if(yy >= 16)	{
						yy = 0;
					}
				}
				else if(wTerm == VK_F2)		{
					xx--;
					if(xx < 0)	{
						xx = 4;
						yy--;
					}
					if(yy < 0)	{
						yy = 15;
					}
				}
				if(CursorPos[yy][xx] == -1)	{
					continue;
				}
				diag_getattr(IDC_PRN_EX_SUB_DG1, CursorPos[yy][xx], &da, CIcsdiagctrl);
				if(da.attr_bcolor == BC_GRAY)	{
					continue;
				}
				if(da.attr_bcolor == BC_WHITE)	{
					npos = CursorPos[yy][xx];
					break;
				}
			}
			break;

		case VK_RIGHT:
		case VK_LEFT:
			while(1)	{
				if(wTerm == VK_RIGHT)		xx++;
				else if(wTerm == VK_LEFT)	xx--;

				if(xx < 0)	xx = 0;

				if(CursorPos[yy][xx] == -1)	{
					break;
				}
				diag_getattr(IDC_PRN_EX_SUB_DG1, CursorPos[yy][xx], &da, CIcsdiagctrl);
				if(da.attr_bcolor == BC_GRAY)	{
					continue;
				}
				if(da.attr_bcolor == BC_WHITE)	{
					npos = CursorPos[yy][xx];
					break;
				}
			}
			break;

		case VK_DOWN:
		case VK_UP:
			while(1)	{
				if(wTerm == VK_DOWN)	yy++;
				else if(wTerm == VK_UP)	yy--;

				if(yy < 0)	yy = 0;

				if(CursorPos[yy][xx] == -1)	{
					break;
				}
				diag_getattr(IDC_PRN_EX_SUB_DG1, CursorPos[yy][xx], &da, CIcsdiagctrl);
				if(da.attr_bcolor == BC_GRAY)	{
					continue;
				}
				if(da.attr_bcolor == BC_WHITE)	{
					npos = CursorPos[yy][xx];
					break;
				}
			}
			break;
	}

	if(npos != -1)	{
		// 最終のインデックスでEnterキーを押下した場合
		if(wTerm == VK_RETURN && npos == CHECK_SUB_01)	{
			// OKボタンへ移動
			GetDlgItem(IDOK)->SetFocus();
		}
		else	{
			m_Diag1.SetPosition(npos);
		}
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CPrnPrint_EX_Sub::OnBnClickedPenExSubck01()
{
	SetEnableItem(m_Check1.GetCheck());
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CPrnPrint_EX_Sub::OnBnClickedOk()
{
	GetInputItemData();

	ICSDialog::OnOK();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CPrnPrint_EX_Sub::OnBnClickedCancel()
{

	ICSDialog::OnCancel();
}
