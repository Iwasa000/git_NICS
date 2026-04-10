//yokono
// ShinSpecific51_31.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific51_31.h"
#include "ShinSpecific51_31Idx.h"
#include "TaxationList31View.h"
// 24/04/19特定収入追加 add -->
#include "SpeAddDlg.h"
// 24/04/19特定収入追加 add <--

//== 定数 ==
const	int	BottomIndex = 23;
const	int	StartIndex = 96;


// CShinSpecific51_31 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific51_31, CSpcBaseDlg)

CShinSpecific51_31::CShinSpecific51_31(CWnd* pParent /*=nullptr*/)
	: CSpcBaseDlg(IDD_DIALOG_K51_31, pParent)
	, m_wPos(0)
	, m_cmPos(0)
	, m_ScW(0)
{

}

CShinSpecific51_31::~CShinSpecific51_31()
{
}

void CShinSpecific51_31::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific51);
}


BEGIN_MESSAGE_MAP(CShinSpecific51_31, CSpcBaseDlg)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
// 24/04/19特定収入追加 add -->
	ON_BN_CLICKED(IDC_BUTTON1,&CShinSpecific51_31::OnBnClickedButton1)
// 24/04/19特定収入追加 add <--
END_MESSAGE_MAP()


// CShinSpecific51_31 メッセージ ハンドラー


BOOL CShinSpecific51_31::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	//初期設定
	SpecificInit51() ;

	m_initfg = 1 ;

	CSpcBaseDlg::OnInitDialogEX();

	//return TRUE;  // return TRUE unless you set the focus to a control
	return FALSE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//初期設定
void CShinSpecific51_31::SpecificInit51()
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );

// 24/04/19特定収入追加 add -->
	// 取戻し対象特定収入ボタンはインボイス後のみ表示する
	long eymd = 0;
	if(!((*m_pSnHeadData)->Sn_SKKBN % 2))	{
		eymd = (*m_pSnHeadData)->Sn_MDAYE;
	}
	else	{
		eymd = (*m_pSnHeadData)->Sn_KDAYE;
	}
	if(eymd >= KIKAN_START_INV80)	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
	}
	else	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	}

	// 再計算
	Specific51_Calq();
// 24/04/19特定収入追加 add <--

	// 表示
	SpecificDisp51();

	//カーソルの位置をセット
	SetItemPosition();
}

BOOL CShinSpecific51_31::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}
			return TRUE;
		}
	}

	return CSpcBaseDlg::PreTranslateMessage(pMsg);
}

BEGIN_EVENTSINK_MAP(CShinSpecific51_31, CSpcBaseDlg)
	ON_EVENT(CShinSpecific51_31, IDC_ICSDIAG8CTRL1, 3, CShinSpecific51_31::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()


void CShinSpecific51_31::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	char	VK_FLG;
	short	wTerm;

	if( nChar == 0 ){
		return;
	}

	// SHIFT+TABをTABに、TABをENTERに変換
	VK_FLG = 0;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );
	if( nChar == VK_TAB ){
		if( VK_FLG&0x80 ){
			wTerm = VK_TAB;
		}
		else{
			wTerm = VK_RETURN;
		}
	}
	else{
		wTerm = nChar;
	}
}

//カーソルの位置をセット
void CShinSpecific51_31::SetItemPosition()
{
	int index;

	// 前回のカーソルの位置を復元
	if( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if( (index = m_Specific51.GetPosition()) != -1) {
			m_Specific51.SetPosition(index);
			m_Specific51.SetFocus();
			return;
		}
	}

	//非連動のとき、96番（税率４％適用分の１）にセット
	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Specific51.SetPosition(ID501_1A);
	}
	else{
	}

	//スクロール再描画
	//SetRedrawScroll( 0 );
}

// 表示
void CShinSpecific51_31::SpecificDisp51()
{
	char			val[MONY_BUF_SIZE] = { 0 };
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	
	// 1.調整前の課税仕入れ等の税額の合計額
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpKgz4, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpKgz63, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpKgz624, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_1C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpKgz78, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_1D, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 2.課税仕入れ等(税率4%)にのみ使途が特定されている特定収入
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpTsyu4, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_2A, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 3.②× 4 / 105 (1円未満の端数切捨て)
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpGszg4, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_3A, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 4.課税仕入れ等(税率6.3%)にのみ使途が特定されている特定収入
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpTsyu63, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_4B, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 5.④× 6.3 / 108 (1円未満の端数切捨て)
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpGszg63, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_5B, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 6.課税仕入れ等(税率6.24%)にのみ使途が特定されている特定収入
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpTsyu624, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_6C, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 7.⑥× 6.24 / 108 (1円未満の端数切捨て)
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpGszg624, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_7C, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 8.課税仕入れ等(税率7.8%)にのみ使途が特定されている特定収入
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpTsyu78, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_8D, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 9.④× 7.8 / 110 (1円未満の端数切捨て)
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpGszg78, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_9D, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 10.①－③
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_sub(val, (*m_pSpcListData)->stval.SpKgz4, (*m_pSpcListData)->stval.SpGszg4);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_10A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_sub(val, (*m_pSpcListData)->stval.SpKgz63, (*m_pSpcListData)->stval.SpGszg63);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_10B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_sub(val, (*m_pSpcListData)->stval.SpKgz624, (*m_pSpcListData)->stval.SpGszg624);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_10C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_sub(val, (*m_pSpcListData)->stval.SpKgz78, (*m_pSpcListData)->stval.SpGszg78);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_10D, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 11.調整割合(計算表4④) 分子
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpFsyu, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_11_bunshi, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 11.調整割合(計算表4④) 分母
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_add(val, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpFsyu);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_11_bunbo, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 12. ⑩×⑪ (1円未満の端数切捨て)
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpTwag4, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_12A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpTwag63, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_12B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpTwag624, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_12C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpTwag78, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_12D, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 13.特定収入に係る課税仕入れ等の税額
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpGgt4, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_13A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpGgt63, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_13B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpGgt624, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_13C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpGgt78, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_13D, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	// 14.控除対象仕入税額(①－⑬)
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpSiz4, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_14A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpSiz63, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_14B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpSiz624, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_14C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpSiz78, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID501_14D, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	

}

void CShinSpecific51_31::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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


void CShinSpecific51_31::OnSize(UINT nType, int cx, int cy)
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
		m_Specific51.GetRect(BottomIndex, (LPUNKNOWN)&b);
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

// 24/04/19特定収入追加 add -->
// 取戻し対象特定収入ボタン押下
void CShinSpecific51_31::OnBnClickedButton1()
{
	CSpeAddDlg SpeAdd(this);
	SpeAdd.InitInfo(*m_pSnHeadData,*m_pSpcListData,m_pArith,m_pZmSub);
	if(SpeAdd.DoModal() == 0)	{
		// 再計算
		Specific51_Calq();
		// 再表示
		SpecificDisp51();
	}
// 240501_168729 add -->
	GetDlgItem(IDC_BUTTON1)->SetFocus();
// 240501_168729 add <--
}

// 再計算
void CShinSpecific51_31::Specific51_Calq(void)
{
	BOOL			IsChgDoSpc = FALSE;
	unsigned char	svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;

	(*m_pSpcListData)->SPassOffCalqMain(*m_pSnHeadData);

// 24/04/26特定収入追加 add -->
	// 5-2のデータがあればガイドを表示する
	if((*m_pSpcListData)->m_bIs52Data == TRUE)	{
		// 表示
		diag_modify(IDC_ICSDIAG8CTRL1,ID501_GUID,DIAG_MDFY_OPAQUE,CIcsdiagctrl);
	}
	else	{
		// 非表示
		diag_modify(IDC_ICSDIAG8CTRL1,ID501_GUID,DIAG_MDFY_TRANSPARENT,CIcsdiagctrl);
	}
// 24/04/26特定収入追加 add <--

	IsChgDoSpc = FALSE;
	if((svSn_Sign2 & 0x02) != ((*m_pSnHeadData)->Sn_Sign2 & 0x02)) {
		IsChgDoSpc = TRUE;
	}
	((CTaxationList31View*)m_pParent)->LinkHonpyoData(IsChgDoSpc);
}
// 24/04/19特定収入追加 add <--
