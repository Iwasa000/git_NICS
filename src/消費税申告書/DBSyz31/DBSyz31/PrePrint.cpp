// PrePrint.cpp : 実装ファイル
//

#include "stdafx.h"
#include "PrePrint.h"
//#include "afxdialogex.h"


int	PrePrintItemNo[50] = {
	IDC_ICSDISP8CTRL1,
	IDC_ICSDISP8CTRL2,
	IDC_ICSDISP8CTRL3,
	IDC_ICSDISP8CTRL4,
	IDC_ICSDISP8CTRL5,
	IDC_ICSDISP8CTRL20,
	0
};



// CPrePrint ダイアログ

IMPLEMENT_DYNAMIC(CPrePrint, ICSDialog)

CPrePrint::CPrePrint(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_DIALOG_PREPRINT, pParent)
	, m_pSnHeadData(NULL)
{
	memset(ADRU1, '\0', sizeof(ADRU1));
	memset(ADRU2, '\0', sizeof(ADRU2));
	memset(ADRC1, '\0', sizeof(ADRC1));
	memset(ADRC2, '\0', sizeof(ADRC2));
	memset(ADRD1, '\0', sizeof(ADRD1));
	memset(ADRD2, '\0', sizeof(ADRD2));
	memset(ADRD3, '\0', sizeof(ADRD3));
	memset(ADKUP, '\0', sizeof(ADKUP));
	memset(ADKDW, '\0', sizeof(ADKDW));
	memset(ADKNO, '\0', sizeof(ADKNO));
	memset(ADRD4, '\0', sizeof(ADRD4));
	memset(ADSNO, '\0', sizeof(ADSNO));
	memset(ADDR, '\0', sizeof(ADDR));
	memset(ADDRX, '\0', sizeof(ADDRX));
	memset(NUMBF, '\0', sizeof(NUMBF));
}

CPrePrint::~CPrePrint()
{
}

void CPrePrint::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDBEDT8CTRL1, m_DbEdit1);
	DDX_Control(pDX, IDC_ICSDBEDT8CTRL2, m_DbEdit2);
	DDX_Control(pDX, IDC_ICSDBEDT8CTRL3, m_DbEdit3);
	DDX_Control(pDX, IDC_ICSDBEDT8CTRL4, m_DbEdit4);
	DDX_Control(pDX, IDC_ICSDBEDT8CTRL7, m_DbEdit7);
	DDX_Control(pDX, IDC_STATIC_NOTE, m_PrePrintNote);
	DDX_Control(pDX, IDC_ICSDISP8CTRL1, m_Disp1);
	DDX_Control(pDX, IDC_ICSDISP8CTRL2, m_Disp2);
	DDX_Control(pDX, IDC_ICSDISP8CTRL3, m_Disp3);
	DDX_Control(pDX, IDC_ICSDISP8CTRL4, m_Disp4);
	DDX_Control(pDX, IDC_ICSDISP8CTRL5, m_Disp5);
	DDX_Control(pDX, IDC_ICSDISP8CTRL20, m_Disp20);
	DDX_Control(pDX, IDC_ICSDBEDT8CTRL20, m_DbEdit20);
}


BEGIN_MESSAGE_MAP(CPrePrint, ICSDialog)
	ON_BN_SETFOCUS(IDOK, &CPrePrint::OnBnSetfocusOk)
	ON_BN_SETFOCUS(IDCANCEL, &CPrePrint::OnBnSetfocusCancel)
END_MESSAGE_MAP()


//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	消費税ヘッダ情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CPrePrint::InitInfo(CSnHeadData *pSnHeadData)
{
	ASSERT(pSnHeadData);
	if (pSnHeadData == NULL) {
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	return 0;
}

// CPrePrint メッセージ ハンドラー
BOOL CPrePrint::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	/*- 標準色作成 -*/
	((ICSWinApp*)AfxGetApp())->GetStanderdColor(ColorInfo);
	if (ColorInfo.m_swOnOff == 1) {
		// バックカラーの取得
		COLORREF Pre = ColorInfo.ViewColor();
		for (int i = 0; PrePrintItemNo[i]; i++) {
			cl_dotoff(PrePrintItemNo[i], Pre, CIcsdisp8ctrl1);
		}
	}

	VARIANT	Val;
	int		ankLn = 0;

	// 上段①　左
	memset(ADRU1, '\0', sizeof(ADRU1));
	memmove(ADRU1, m_pSnHeadData->Sn_ADRU1, 30);
	memset(ADDR, '\0', sizeof(ADDR));
	ankLn = m_Util.MixSpaceCutLength((char *)m_pSnHeadData->Sn_ADRU1, 30);
	memmove(ADDR, m_pSnHeadData->Sn_ADRU1, ankLn);
	Val.pbVal = (unsigned char*)ADDR;
	m_DbEdit1.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	// 上段①　右
	memset(ADRU2, '\0', sizeof(ADRU2));
	memmove(ADRU2, m_pSnHeadData->Sn_ADRU2, 20);
	memset(ADDR, '\0', sizeof(ADDR));
	ankLn = m_Util.MixSpaceCutLength((char *)m_pSnHeadData->Sn_ADRU2, 20);
	memmove(ADDR, m_pSnHeadData->Sn_ADRU2, ankLn);
	Val.pbVal = (unsigned char*)ADDR;
	m_DbEdit2.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	// 上段②　左
	memset(ADRC1, '\0', sizeof(ADRC1));
	memmove(ADRC1, m_pSnHeadData->Sn_ADRC1, 20);
	memset(ADDR, '\0', sizeof(ADDR));
	ankLn = m_Util.MixSpaceCutLength((char *)m_pSnHeadData->Sn_ADRC1, 20);
	memmove(ADDR, m_pSnHeadData->Sn_ADRC1, ankLn);
	Val.pbVal = (unsigned char*)ADDR;
	m_DbEdit3.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	// 上段②　右
	memset(ADRC2, '\0', sizeof(ADRC2));
	memmove(ADRC2, m_pSnHeadData->Sn_ADRC2, 20);
	memset(ADDR, '\0', sizeof(ADDR));
	ankLn = m_Util.MixSpaceCutLength((char *)m_pSnHeadData->Sn_ADRC2, 20);
	memmove(ADDR, m_pSnHeadData->Sn_ADRC2, ankLn);
	Val.pbVal = (unsigned char*)ADDR;
	m_DbEdit4.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);

	// 下段の編集
	memset(ADRD4, '\0', sizeof(ADRD4));
	memmove(ADRD4, m_pSnHeadData->Sn_ADRD4, 44);
	memset(ADDR, '\0', sizeof(ADDR));
	memset(ADDR, 0x20, 60);
	ankLn = m_Util.GetByteStrLen((unsigned char *)m_pSnHeadData->Sn_ADRD4, 44);
	memmove(ADDR, m_pSnHeadData->Sn_ADRD4, ankLn);
	memset(ADRD2, '\0', sizeof(ADRD2));
	memset(ADRD2, 0x20, 16);
	if ((ankLn = m_Util.GetByteStrLen((unsigned char *)m_pSnHeadData->Sn_ADRD2, 16))) {
		memmove(ADRD2, m_pSnHeadData->Sn_ADRD2, ankLn);
		memmove(&ADDR[44], m_pSnHeadData->Sn_ADRD2, ankLn);
	}
	memset(ADDRX, '\0', sizeof(ADDRX));
	ankLn = m_Util.MixSpaceCutLength((char *)ADDR, 60);
	memmove(ADDRX, ADDR, ankLn);
	Val.pbVal = (unsigned char*)ADDRX;
	m_DbEdit7.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);

	int Sign = 0,
		Type = 0;
	// 署別一覧番号
	m_DbEdit20.EnableWindow(TRUE);
	memset(ADSNO, '\0', sizeof(ADSNO));
	memmove(ADSNO, m_pSnHeadData->Sn_ADSNO, 7);
	memset(ADDR, '\0', sizeof(ADDR));
	ankLn = m_Util.MixSpaceCutLength((char *)m_pSnHeadData->Sn_ADSNO, 7);
	memmove(ADDR, m_pSnHeadData->Sn_ADSNO, ankLn);
	Val.pbVal = (unsigned char*)ADDR;
	m_DbEdit20.SetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	c_set(IDC_ICSDBEDT8CTRL1);

	ChangeUnderTitle();

	ICSDialog::OnInitDialogEX();
	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CPrePrint::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	VARIANT	Val;
	int		ankLn = 0;

	m_DbEdit1.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	memset(ADDR, '\0', sizeof(ADDR));
	memset(ADDR, 0x20, 30);
	ankLn = m_Util.GetByteStrLen(Val.pbVal, 30);
	memmove(ADDR, Val.pbVal, ankLn);
	memmove(m_pSnHeadData->Sn_ADRU1, ADDR, 30);

	m_DbEdit2.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	memset(ADDR, '\0', sizeof(ADDR));
	memset(ADDR, 0x20, 20);
	ankLn = m_Util.GetByteStrLen((unsigned char *)Val.pbVal, 20);
	memmove(ADDR, Val.pbVal, ankLn);
	memmove(m_pSnHeadData->Sn_ADRU2, ADDR, 20);

	m_DbEdit3.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	memset(ADDR, '\0', sizeof(ADDR));
	memset(ADDR, 0x20, 20);
	ankLn = m_Util.GetByteStrLen((unsigned char *)Val.pbVal, 20);
	memmove(ADDR, Val.pbVal, ankLn);
	memmove(m_pSnHeadData->Sn_ADRC1, ADDR, 20);

	m_DbEdit4.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	memset(ADDR, '\0', sizeof(ADDR));
	memset(ADDR, 0x20, 20);
	ankLn = m_Util.GetByteStrLen((unsigned char *)Val.pbVal, 20);
	memmove(ADDR, Val.pbVal, ankLn);
	memmove(m_pSnHeadData->Sn_ADRC2, ADDR, 20);

	m_DbEdit7.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	memset(ADDR, '\0', sizeof(ADDR));
	memset(ADDR, 0x20, 60);
	ankLn = m_Util.GetByteStrLen((unsigned char *)Val.pbVal, 60);
	memmove(ADDR, Val.pbVal, ankLn);
	memmove(m_pSnHeadData->Sn_ADRD4, ADDR, 46);			// 下段①		[09'08.01]
	memmove(m_pSnHeadData->Sn_ADRD2, &ADDR[44], 16);		// 下段②

	m_DbEdit20.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
	memset(ADDR, '\0', sizeof(ADDR));
	memset(ADDR, 0x20, 7);
	ankLn = m_Util.GetByteStrLen((unsigned char *)Val.pbVal, 7);
	memmove(ADDR, Val.pbVal, ankLn);
	memmove(m_pSnHeadData->Sn_ADSNO, ADDR, 7);

	ICSDialog::OnOK();
}

void CPrePrint::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	memmove(m_pSnHeadData->Sn_ADRU1, ADRU1, 30);	// 上段①　左
	memmove(m_pSnHeadData->Sn_ADRU2, ADRU2, 20);	// 上段①　右
	memmove(m_pSnHeadData->Sn_ADRC1, ADRC1, 20);	// 上段②　左
	memmove(m_pSnHeadData->Sn_ADRC2, ADRC2, 20);	// 上段②　右
	memmove(m_pSnHeadData->Sn_ADRD4, ADRD4, 44);	// 下段①
	memmove(m_pSnHeadData->Sn_ADRD2, ADRD2, 16);	// 下段②
	memmove(m_pSnHeadData->Sn_ADSNO, ADSNO, 7);
	ICSDialog::OnCancel();
}

BOOL CPrePrint::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if (pMsg->message == WM_KEYDOWN) {

		CWnd *pWnd = GetFocus();
		char	VK_FLG = 0x00;
		VK_FLG = (char)::GetKeyState(VK_SHIFT);
		if (GetCombineKeyDown(VK_SHIFT) && VK_F1 <= pMsg->wParam && pMsg->wParam <= VK_F12)	return TRUE;
		if (pMsg->wParam == VK_RETURN ||
			pMsg->wParam == VK_F2 ||
			pMsg->wParam == VK_F3) {
		}

		if (pMsg->wParam == VK_RETURN) {
			if (pMsg->hwnd != ::GetDlgItem(m_hWnd, IDOK) &&
				pMsg->hwnd != ::GetDlgItem(m_hWnd, IDCANCEL)) {
				((CDialog*)this)->NextDlgCtrl();
				return TRUE;
			}
		}

		else if ((pMsg->wParam == VK_F2) || (pMsg->wParam == VK_UP) || ((VK_FLG & 0x80) && (pMsg->wParam == VK_TAB))) {
			if (pWnd == GetDlgItem(IDC_ICSDBEDT8CTRL1)) {
				c_set(IDCANCEL);
				return TRUE;
			}
			((CDialog*)this)->PrevDlgCtrl();
			return TRUE;
		}
		else if ((pMsg->wParam == VK_TAB) || (pMsg->wParam == VK_F3) || (pMsg->wParam == VK_DOWN)) {
			if (pWnd == GetDlgItem(IDCANCEL)) {
				c_set(IDC_ICSDBEDT8CTRL1);
				return TRUE;
			}
			else {
				((CDialog*)this)->NextDlgCtrl();
				return TRUE;
			}
		}
		else if (pMsg->wParam == VK_ESCAPE) {
			OnCancel();
			return TRUE;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

BEGIN_EVENTSINK_MAP(CPrePrint, ICSDialog)
	ON_EVENT(CPrePrint, IDC_ICSDBEDT8CTRL1, 1, CPrePrint::TerminationIcsdbedt8ctrl1, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CPrePrint, IDC_ICSDBEDT8CTRL2, 1, CPrePrint::TerminationIcsdbedt8ctrl2, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CPrePrint, IDC_ICSDBEDT8CTRL3, 1, CPrePrint::TerminationIcsdbedt8ctrl3, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CPrePrint, IDC_ICSDBEDT8CTRL4, 1, CPrePrint::TerminationIcsdbedt8ctrl4, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CPrePrint, IDC_ICSDBEDT8CTRL7, 1, CPrePrint::TerminationIcsdbedt8ctrl7, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CPrePrint, IDC_ICSDBEDT8CTRL1, 3, CPrePrint::SetFocusIcsdbedt8ctrl1, VTS_NONE)
	ON_EVENT(CPrePrint, IDC_ICSDBEDT8CTRL2, 3, CPrePrint::SetFocusIcsdbedt8ctrl2, VTS_NONE)
	ON_EVENT(CPrePrint, IDC_ICSDBEDT8CTRL3, 3, CPrePrint::SetFocusIcsdbedt8ctrl3, VTS_NONE)
	ON_EVENT(CPrePrint, IDC_ICSDBEDT8CTRL4, 3, CPrePrint::SetFocusIcsdbedt8ctrl4, VTS_NONE)
	ON_EVENT(CPrePrint, IDC_ICSDBEDT8CTRL7, 3, CPrePrint::SetFocusIcsdbedt8ctrl7, VTS_NONE)
	ON_EVENT(CPrePrint, IDC_ICSDBEDT8CTRL20, 3, CPrePrint::SetFocusIcsdbedt8ctrl20, VTS_NONE)
	ON_EVENT(CPrePrint, IDC_ICSDBEDT8CTRL20, 1, CPrePrint::TerminationIcsdbedt8ctrl20, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CPrePrint::TerminationIcsdbedt8ctrl1(long nChar, long inplen, long kst)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	VARIANT	Val;

	switch (nChar) {
	case NULL:
		return;
	case VK_DELETE:
		break;
	case VK_RETURN:
		m_DbEdit1.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		memset(ADDR, '\0', sizeof(ADDR));
		memset(ADDR, 0x20, 30);
		memmove(ADDR, Val.pbVal, inplen);
		memmove(m_pSnHeadData->Sn_ADRU1, ADDR, 30);

		((CDialog*)this)->NextDlgCtrl();

		break;
	default:	break;
	}
}

void CPrePrint::TerminationIcsdbedt8ctrl2(long nChar, long inplen, long kst)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	VARIANT	Val;

	switch (nChar) {
	case NULL:
		return;
	case VK_DELETE:
		break;
	case VK_RETURN:
		m_DbEdit2.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		memset(ADDR, '\0', sizeof(ADDR));
		memset(ADDR, 0x20, 20);
		memmove(ADDR, Val.pbVal, inplen);
		memmove(m_pSnHeadData->Sn_ADRU2, ADDR, 20);

		((CDialog*)this)->NextDlgCtrl();

		break;
	default:	break;
	}
}

void CPrePrint::TerminationIcsdbedt8ctrl3(long nChar, long inplen, long kst)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	VARIANT	Val;

	switch (nChar) {
	case NULL:
		return;
	case VK_DELETE:
		break;
	case VK_RETURN:
		m_DbEdit3.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		memset(ADDR, '\0', sizeof(ADDR));
		memset(ADDR, 0x20, 20);
		memmove(ADDR, Val.pbVal, inplen);
		memmove(m_pSnHeadData->Sn_ADRC1, ADDR, 20);

		((CDialog*)this)->NextDlgCtrl();

		break;
	default:	break;
	}
}

void CPrePrint::TerminationIcsdbedt8ctrl4(long nChar, long inplen, long kst)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	VARIANT	Val;

	switch (nChar) {
	case NULL:
		return;
	case VK_DELETE:
		break;
	case VK_RETURN:
		m_DbEdit4.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		memset(ADDR, '\0', sizeof(ADDR));
		memset(ADDR, 0x20, 20);
		memmove(ADDR, Val.pbVal, inplen);
		memmove(m_pSnHeadData->Sn_ADRC2, ADDR, 20);

		((CDialog*)this)->NextDlgCtrl();

		break;
	default:	break;
	}
}

void CPrePrint::TerminationIcsdbedt8ctrl7(long nChar, long inplen, long kst)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	VARIANT	Val;

	switch (nChar) {
	case NULL:
		return;
	case VK_DELETE:
		break;
	case VK_RETURN:
		m_DbEdit7.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		memset(ADDR, '\0', sizeof(ADDR));
		memset(ADDR, 0x20, 60);
		memmove(ADDR, Val.pbVal, inplen);
		memmove(m_pSnHeadData->Sn_ADRD4, ADDR, 46);				// ①			
		memmove(m_pSnHeadData->Sn_ADRD2, &ADDR[30], 16);		// ②
		((CDialog*)this)->NextDlgCtrl();

		break;
	default:	break;
	}
}


void CPrePrint::SetFocusIcsdbedt8ctrl1()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_PrePrintNote.EnableWindow(TRUE);
	m_PrePrintNote.SetWindowTextA(_T("最大全角１５文字、\n半角３０文字まで登録できます"));
}

void CPrePrint::SetFocusIcsdbedt8ctrl2()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_PrePrintNote.EnableWindow(TRUE);
	m_PrePrintNote.SetWindowTextA(_T("２０文字まで登録できます"));
}

void CPrePrint::SetFocusIcsdbedt8ctrl3()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_PrePrintNote.EnableWindow(TRUE);
	m_PrePrintNote.SetWindowTextA(_T("最大全角１０文字、\n半角２０文字まで登録できます"));
}

void CPrePrint::SetFocusIcsdbedt8ctrl4()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_PrePrintNote.EnableWindow(TRUE);
	m_PrePrintNote.SetWindowTextA(_T("２０文字まで登録できます"));
}

void CPrePrint::SetFocusIcsdbedt8ctrl7()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_PrePrintNote.EnableWindow(TRUE);
	m_PrePrintNote.SetWindowTextA(_T("最大全角３０文字、\n半角６０文字まで登録できます"));
}

void CPrePrint::OnBnSetfocusOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_PrePrintNote.SetWindowTextA(_T(""));
	m_PrePrintNote.EnableWindow(FALSE);
}

void CPrePrint::OnBnSetfocusCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_PrePrintNote.SetWindowTextA(_T(""));
	m_PrePrintNote.EnableWindow(FALSE);
}

// 欄外印字　署別一連番号　（法人専用）
void CPrePrint::SetFocusIcsdbedt8ctrl20()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_PrePrintNote.EnableWindow(TRUE);
	m_PrePrintNote.SetWindowTextA(_T("７文字まで登録できます"));
}

void CPrePrint::TerminationIcsdbedt8ctrl20(long nChar, long inplen, long kst)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	VARIANT	Val;

	switch (nChar) {
	case NULL:
		return;
	case VK_DELETE:
		break;
	case VK_RETURN:
		m_DbEdit2.GetData(&Val, ICSDBEDT_TYPE_STRING, 0);
		memset(ADDR, '\0', sizeof(ADDR));
		memset(ADDR, 0x20, 7);
		memmove(ADDR, Val.pbVal, inplen);
		memmove(m_pSnHeadData->Sn_ADSNO, ADDR, 7);

		((CDialog*)this)->NextDlgCtrl();

		break;
	default:	break;
	}
}

//=======================================================
// 下段１と２のタイトルを変更
//=======================================================
void CPrePrint::ChangeUnderTitle()
{
	// 令和5年10月1日以降用の本表には、下部に注意書きが追加された
	// そのため、下段１と２の出力位置を変更する
	long eymd = 0;

	// 親ウィンドウを受け取っていないため、課税期間取得関数を呼び出せない…
	if( !( m_pSnHeadData->Sn_SKKBN % 2 )) {
		// 中間申告
		eymd = m_pSnHeadData->Sn_MDAYE;
	}
	else {
		// 確定申告
		eymd = m_pSnHeadData->Sn_KDAYE;
	}

	if( eymd >= ICS_SH_INVOICE_DAY ) {
		// タイトルを変更
		CString strDisp5, strDisp20;

		// タイトル取得
		m_Disp5.GetWindowText( strDisp5 );
		m_Disp20.GetWindowText( strDisp20 );

		// 逆転
		m_Disp5.SetWindowText( strDisp20 );
		m_Disp20.SetWindowText( strDisp5 );
	}
}