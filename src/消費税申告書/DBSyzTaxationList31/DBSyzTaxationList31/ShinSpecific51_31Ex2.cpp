// ShinSpecific51_31Ex2.cpp : 実装ファイル
//
#include "stdafx.h"
#include "ShinSpecific51_31Ex2.h"
#include "ShinSpecific51_31_exIdx2.h"
#include "TaxationList31View.h"
#include "SpeAddDlg.h"

//== 定数 ==
const	int	BottomIndex = 16;

// CShinSpecific51_31_ex ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific51_31Ex2, CSpcBaseDlg)

CShinSpecific51_31Ex2::CShinSpecific51_31Ex2(CWnd* pParent /*=nullptr*/)
	: CSpcBaseDlg(IDD_DIALOG_K51_31_ex2, pParent)
	, m_wPos(0)
	, m_cmPos(0)
	, m_ScW(0)
{
}

CShinSpecific51_31Ex2::~CShinSpecific51_31Ex2()
{
}

void CShinSpecific51_31Ex2::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific51Ex2);
}

BEGIN_MESSAGE_MAP(CShinSpecific51_31Ex2, CSpcBaseDlg)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CShinSpecific51_31Ex2::OnBnClickedButton1)
END_MESSAGE_MAP()


// CShinSpecific51_31_ex メッセージ ハンドラー
BOOL CShinSpecific51_31Ex2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_END) {
			if (m_pParent) {
				m_pParent->PostMessageA(WM_KEYDOWN, VK_END);
			}
			return TRUE;
		}
	}

	return CSpcBaseDlg::PreTranslateMessage(pMsg);
}

BOOL CShinSpecific51_31Ex2::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();
	SpecificInit51Ex2();
	m_initfg = 1;

	CSpcBaseDlg::OnInitDialogEX();

	return TRUE;
}

//初期設定
void CShinSpecific51_31Ex2::SpecificInit51Ex2()
{
	// 倍長演算指定
	m_pArith->l_defn(0x16);

	// 再計算
	Specific51Ex2_Calq();

	// 表示
	SpecificDisp51Ex2();

	//カーソルの位置をセット
	SetItemPosition();
}

void CShinSpecific51_31Ex2::SetItemPosition()
{
	int index;

	// 前回のカーソルの位置を復元
	if (m_curwnd == IDC_ICSDIAG8CTRL1) {
		if ((index = m_Specific51Ex2.GetPosition()) != -1) {
			m_Specific51Ex2.SetPosition(index);
			m_Specific51Ex2.SetFocus();
			return;
		}
	}

	//非連動のとき、どの項目も入力可にはならないが、フォーカスセットしないとスクロールが反応しないためセットする
	this->SetFocus();
	if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
		m_Specific51Ex2.SetPosition(ID501_1A);
	}
	else {
	}
}

// 表示
void CShinSpecific51_31Ex2::SpecificDisp51Ex2()
{
	char			val[MONY_BUF_SIZE] = { 0 };
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;


	// 1.調整前の課税仕入れ等の税額の合計額
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpKgz624, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpKgz78, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 2.課税仕入れ等(税率6.24%)にのみ使途が特定されている特定収入
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpTsyu624, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_2A, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 3. 2× 6.24 / 108 (1円未満の端数切捨て)
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpGszg624, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_3A, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 4.課税仕入れ等(税率7.8%)にのみ使途が特定されている特定収入
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpTsyu78, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_4B, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 5. 4× 7.8 / 110 (1円未満の端数切捨て)
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpGszg78, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_5B, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 6. 1-3, 1-5
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_sub(val, (*m_pSpcListData)->stval.SpKgz624, (*m_pSpcListData)->stval.SpGszg624);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_6A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_sub(val, (*m_pSpcListData)->stval.SpKgz78, (*m_pSpcListData)->stval.SpGszg78);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_6B, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 7.調整割合(計算表44) 分子
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpFsyu, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_7_bunshi, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 7.調整割合(計算表44) 分母
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_add(val, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpFsyu);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_7_bunbo, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 8. 6×7 (1円未満の端数切捨て)
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpTwag624, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_8A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpTwag78, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_8B, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 9.特定収入に係る課税仕入れ等の税額
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpGgt624, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_9A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpGgt78, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_9B, (struct IUnknown *)&DiagData, CIcsdiagctrl);

// 24/02/19wd_特定収入対応 add -->
	// 10.控除対象外仕入れに係る調整額の合計額
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.Sp51kei624, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_10A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.Sp51kei78, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_10B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
// 24/02/19wd_特定収入対応 add <--

	// 11.控除対象仕入税額(1+10-9)
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpSiz624, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_11A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpSiz78, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_11B, (struct IUnknown *)&DiagData, CIcsdiagctrl);


}

//======================================================
// 再計算
//======================================================
void CShinSpecific51_31Ex2::Specific51Ex2_Calq(void)
{
	BOOL			IsChgDoSpc = FALSE;
	unsigned char	svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;

	(*m_pSpcListData)->SPassOffCalqMain(*m_pSnHeadData);

	IsChgDoSpc = FALSE;
	if((svSn_Sign2 & 0x02) != ((*m_pSnHeadData)->Sn_Sign2 & 0x02)) {
		IsChgDoSpc = TRUE;
	}
	((CTaxationList31View*)m_pParent)->LinkHonpyoData(IsChgDoSpc);
}

void CShinSpecific51_31Ex2::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	if (pScrollBar) {
		return;
	}
	SCROLLINFO SI;
	GetScrollInfo(SB_VERT, &SI);
	int nowPos = m_wPos;

	switch (nSBCode) {
	case SB_LINEUP:
	case SB_PAGEUP:
		if (nSBCode == SB_LINEUP) {
			m_wPos -= m_ScW;
		}
		else {
			m_wPos -= SI.nPage;
		}
		if (m_wPos <= 0) {
			m_wPos = 0;
		}
		break;
	case SB_LINEDOWN:
	case SB_PAGEDOWN:
		if (nSBCode == SB_LINEDOWN) {
			m_wPos += m_ScW;
		}
		else {
			m_wPos += SI.nPage;
		}
		if (m_wPos >= (SI.nMax - (int)SI.nPage)) {
			m_wPos = SI.nMax - SI.nPage;
		}
		break;
	case SB_THUMBTRACK:
		m_wPos = nPos;
		break;
	}
	if ((nSBCode == SB_LINEDOWN) || (nSBCode == SB_LINEUP) || (nSBCode == SB_PAGEDOWN) ||
		(nSBCode == SB_PAGEUP) || (nSBCode == SB_THUMBTRACK)) {
		ScrollWindow(0, nowPos - m_wPos);
		SetScrollPos(SB_VERT, m_wPos);
		m_cmPos = m_wPos;
	}
	CSpcBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CShinSpecific51_31Ex2::OnSize(UINT nType, int cx, int cy)
{
	if (m_initfg) {
		SetScrollPos(SB_VERT, 0, TRUE);
		ScrollWindow(0, m_cmPos, NULL);
		m_cmPos = 0;
	}

	CSpcBaseDlg::OnSize(nType, cx, cy);
	// TODO: ここにメッセージ ハンドラー コードを追加します。

	if (m_initfg) {
		RECT	a, b;
		SCROLLINFO ScrollInfo;
		GetWindowRect(&a);
		m_Specific51Ex2.GetRect(BottomIndex, (LPUNKNOWN)&b);
		if (GetScrollInfo(SB_VERT, &ScrollInfo) == FALSE) {
			ScrollInfo.nPos = 0;
		}
		ScrollInfo.nMin = 0;
		ScrollInfo.nMax = (b.bottom + 30) - a.top + ScrollInfo.nPos - m_cmPos;
		ScrollInfo.nPage = a.bottom - a.top;
		SetScrollInfo(SB_VERT, &ScrollInfo);
		m_wPos = 0;
		m_ScW = 15;
	}
}


HBRUSH CShinSpecific51_31Ex2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if (nCtlColor == CTLCOLOR_DLG) {
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

BEGIN_EVENTSINK_MAP(CShinSpecific51_31Ex2, CSpcBaseDlg)
	ON_EVENT(CShinSpecific51_31Ex2, IDC_ICSDIAG8CTRL1, 3, CShinSpecific51_31Ex2::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()


void CShinSpecific51_31Ex2::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	char	VK_FLG;
	short	wTerm;

	if (nChar == 0) {
		return;
	}

	// SHIFT+TABをTABに、TABをENTERに変換
	VK_FLG = 0;
	VK_FLG = (char)::GetKeyState(VK_SHIFT);
	if (nChar == VK_TAB) {
		if (VK_FLG & 0x80) {
			wTerm = VK_TAB;
		}
		else {
			wTerm = VK_RETURN;
		}
	}
	else {
		wTerm = nChar;
	}
}

void CShinSpecific51_31Ex2::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CSpeAddDlg SpeAdd(this);
	SpeAdd.InitInfo(*m_pSnHeadData, *m_pSpcListData,m_pArith,m_pZmSub);

	//SpeAdd.DoModal();
	if(SpeAdd.DoModal() == 0)	{
		// 再計算
		Specific51Ex2_Calq();
		// 再表示
		SpecificDisp51Ex2();
	}
// 240501_168729 add -->
	GetDlgItem(IDC_BUTTON1)->SetFocus();
// 240501_168729 add <--
}
