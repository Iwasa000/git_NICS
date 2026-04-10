// ShinBuy31.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinBuy31Ex.h"
#include "ShinBuy31ExIdx.h"
#include "afxdialogex.h"


// CShinBuy31Ex ダイアログ

IMPLEMENT_DYNAMIC(CShinBuy31Ex, CDialogEx)

CShinBuy31Ex::CShinBuy31Ex(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(CShinBuy31Ex::IDD, pParent)
	, m_pParent(pParent)
	, m_Idx(0)
	, m_Lin(0)
{
	m_curwnd = -1;

	m_FcsUpEnd = -1;
	m_FcsLeftEnd = -1;
	m_FcsDownEnd = -1;
	m_FcsRightEnd = -1;
}

CShinBuy31Ex::~CShinBuy31Ex()
{
}

void CShinBuy31Ex::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_BuyDiag);
}


BEGIN_MESSAGE_MAP(CShinBuy31Ex, CSyzBaseDlg)
END_MESSAGE_MAP()

BOOL CShinBuy31Ex::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_BuyDiag;
	//m_BottomIdx = 121;//コントロールの一番下のIDX指定するとスクロールがそこまでいく
	m_BottomIdx = 90;


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinBuy31Ex::Init()
{
	InitDiagAttr();

	DispList(0);

	SetItemPosition();

	return 0;
}



// CShinBuy31Ex メッセージ ハンドラー
BEGIN_EVENTSINK_MAP(CShinBuy31Ex, CSyzBaseDlg)
	ON_EVENT(CShinBuy31Ex, IDC_ICSDIAGCTRL1, 1, CShinBuy31Ex::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinBuy31Ex, IDC_ICSDIAGCTRL1, 2, CShinBuy31Ex::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinBuy31Ex, IDC_ICSDIAGCTRL1, 3, CShinBuy31Ex::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
// 22-0167,22-0361 add -->
	ON_EVENT(CShinBuy31Ex, IDC_ICSDIAGCTRL1, 9, CShinBuy31Ex::CheckButtonIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
// 22-0167,22-0361 add <--
END_EVENTSINK_MAP()

void CShinBuy31Ex::EditONIcsdiagctrl1(short index)
{
	m_BuyDiag.OffControl(FALSE);

	m_Idx = index;
	if ((m_Idx >= BKVMINex) && (m_Idx <= BKVMAXex)) {
		/*if (m_Idx == BKVMAX) {
			m_Lin = ((m_Idx - BKVMIN) / 3) - 1;
		}*/
		if (m_Idx == KBUYVAL11ex) {
			m_Lin = 10;
		}
		else if (m_Idx == KBUYVAL12ex) {
			m_Lin = 11;
		}
		else {
			m_Lin = (m_Idx - BKVMINex) / 3;
		}
	}
	else {
		if((m_Idx == KBUYVAL11Kex) || (m_Idx == KBUYVAL11Bex)){
			m_Lin = 10;
		}
		else if ((m_Idx == KBUYVAL12Kex) || (m_Idx == KBUYVAL12Bex)) {
			m_Lin = 11;
		}
		else {
			m_Lin = -1;
		}
	}

	m_curwnd = IDC_ICSDIAGCTRL1;
}


void CShinBuy31Ex::EditOFFIcsdiagctrl1(short index)
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	diag_getdata(IDC_ICSDIAGCTRL1, index, &DiagData, CIcsdiagctrl);
	int	st = GetDiagData(index, DiagData.data_check, &DiagData, 0);
	diag_deledit(IDC_ICSDIAGCTRL1, CIcsdiagctrl);
	if (st < 0) {
		diag_setposition(IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl);
	}
}

void CShinBuy31Ex::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if (nChar == 0) {
		return;
	}

	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;

	UINT	flg = 0;

	idata = (DIAGRAM_DATA *)data;
	m_Util.DiagOcxIniz(&xdata);
	m_Util.MoveDiagData(&xdata, idata);


	//フォーカス移動処理
	// Shift+TABをTABに、TABをENTERに返還
	short	wTerm;
	char	VK_FLG = 0x00;
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

	int pos = 0;
	if (wTerm == VK_UP) {

		pos = FocusMoveCheck(index, 1);
		m_BuyDiag.SetPosition(pos);
	}
	if (wTerm == VK_DOWN) {
		
		pos = FocusMoveCheck(index, 0);
		m_BuyDiag.SetPosition(pos);
	}

	if ((wTerm == VK_RETURN) || (wTerm == VK_F3) || (wTerm == VK_RIGHT)) {
		pos = FocusMoveCheck(index, 2);
		m_BuyDiag.SetPosition(pos);
	}
	else if ((wTerm == VK_TAB) || (wTerm == VK_LEFT) || (wTerm == VK_F2)) {
		pos = FocusMoveCheck(index, 3);
		m_BuyDiag.SetPosition(pos);
	}

	if (wTerm == VK_DELETE) {
		GetDiagData(index, length, &xdata, -1);
	}


	SetRedrawScrollToIdx(m_BuyDiag.GetPosition(), IDC_ICSDIAGCTRL1);

}

BOOL CShinBuy31Ex::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_END) {

			if (m_pParent) {
				m_pParent->PostMessageA(WM_KEYDOWN, VK_END);
			}

			return TRUE;
		}
		if (pMsg->wParam == VK_F2) {
			DIAGRAM_DATA   dd;
			if (m_curwnd != -1) {
				short result = ((CIcsdiagctrl*)GetDlgItem(m_curwnd))->GetPosition();
				diag_getdata(m_curwnd, result, &dd, CIcsdiagctrl);
				if (m_curwnd == IDC_ICSDIAGCTRL1) {
					TerminationIcsdiagctrl1(result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd);
				}
				return TRUE;
			}
		}
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
void CShinBuy31Ex::InitDiagAttr()
{

	DIAGRAM_ATTRIBUTE data;

	int	cnt = 0;
	int	Flg;
	while (1) {
		if (ShinBuy31Ex[cnt].Index == 0) {
			break;
		}
		// 確定時は全入力不可
		if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
			Flg = ShinBuy31Ex[cnt].AllUnConnect;
		}
		else {
			if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
				// 経過措置ありのときは6.3％入力不可
				if (!((*m_pSnHeadData)->SVmzsw) && (ShinBuy31Ex[cnt].UnConnect == 2)) {
					Flg = 0;
				}
				else
					Flg = ShinBuy31Ex[cnt].UnConnect;
			}
			else {
				// 経過措置ありのときは6.3％入力不可
				if (!((*m_pSnHeadData)->SVmzsw) && (ShinBuy31Ex[cnt].Connect == 2)) {
					Flg = 0;
				}
				else
					Flg = ShinBuy31Ex[cnt].Connect;
			}
		}
		ATRIB_MOD(IDC_ICSDIAGCTRL1, ShinBuy31Ex[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, ShinBuy31Ex[cnt].Connect);
		cnt++;
	}

// 22-0167,22-0361 del -->
	//InitAddText();
// 22-0167,22-0361 del <--

	SetFocusRange();

	m_BuyDiag.EnableDelete(0);//DELETEキー１回で削除モード設定

}

// 22-0167,22-0361 del -->
////=====================================================
//// 追加文言の表示/非表示
////=====================================================
//void CShinBuy31Ex::InitAddText()
//{
//	long flg = 0; 
//	if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) {
//		flg = DIAG_MDFY_OPAQUE;
//	}
//	else {
//		flg = DIAG_MDFY_TRANSPARENT;
//	}
//
//	for ( short index = KBADDTEXTSex; index <= KBADDTEXTEex; index++ ) {
//		m_BuyDiag.ModifyItem ( index, flg );
//	}
//}
// 22-0167,22-0361 del <--

// CShinBuy31Ex メッセージ ハンドラー
//-----------------------------------------------------------------------------
// 入力項目色変え
//-----------------------------------------------------------------------------
// 引数	id		；	ダイアグラムID
//		index	：	ダイアグラム内インデックス
//		sign	：	変更サイン
//-----------------------------------------------------------------------------
void CShinBuy31Ex::ChangeColor(unsigned short id, short Index, int sign)
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr(id, Index, &DA, CIcsdiagctrl);
	switch (sign) {
	case 1:	// 入力不可	（白色）
		DA.attr_bcolor = BC_WHITE;
		break;
	case 2:	// ロック	（灰色）
		DA.attr_bcolor = BC_GRAY;
		break;
	case 3:	// 特殊色①	（青色）
		DA.attr_bcolor = BC_BLUE;
		break;
	case 4:	// 特殊色②	（緑色）	
		DA.attr_bcolor = BC_GREEN;
		break;
	case 5:	// 特殊色③	（薄緑色）	 
		DA.attr_bcolor = BC_GREEN_L;
		break;
	default:	// 実額入力	（クリーム色） 
		DA.attr_bcolor = BC_CREAM;
		break;
	}
	diag_setattr(id, Index, &DA, FALSE, CIcsdiagctrl);
}

//-----------------------------------------------------------------------------
// 次フォーカスの場所検索
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinBuy31Ex::FocusMoveCheck(int Index, int Move)
{
	int idx = 0;
	int i = 0, j = 0;
	BOOL end_flg = FALSE;

	DIAGRAM_ATTRIBUTE	DA;

	//ポジションの端到達判定
	if (Move == 0) {//下方向
		if (Index == m_FcsDownEnd) {
			idx = m_FcsUpEnd;
			end_flg = TRUE;
		}
	}
	else if (Move == 1) {//上方向
		if (Index == m_FcsUpEnd) {
			idx = m_FcsDownEnd;
			end_flg = TRUE;
		}
	}
	else if (Move == 2) {//右方向
		if (Index == m_FcsRightEnd) {
			idx = m_FcsLeftEnd;
			end_flg = TRUE;
		}
	}
	else if (Move == 3) {//左方向
		if (Index == m_FcsLeftEnd) {
			idx = m_FcsRightEnd;
			end_flg = TRUE;
		}
	}

	if (end_flg == TRUE) {
		return idx;
	}

	//現在ポジション検索
	if ((Move == 0) || (Move == 1)) {
		for (i = 0; i < FCS_SHINBUY31CNTex; i++) {
			if (Index == FCS_ShinBuy31Ex[i].IDNo) {
				break;
			}
		}
		idx = FCS_ShinBuy31Ex[i].IDNo;//現在ポジションセット
	}
	else if ((Move == 2) || (Move == 3)) {
		for (i = 0; i < FCS_SHINBUY31CNTex; i++) {
			if (Index == FCS_ShinBuy31Ex_LR[i].IDNo) {
				break;
			}
		}
		idx = FCS_ShinBuy31Ex_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if (Move == 0) {//下方向
		for (j = i + 1; j < FCS_SHINBUY31CNTex; j++) {
			m_BuyDiag.GetAttribute(FCS_ShinBuy31Ex[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = FCS_ShinBuy31Ex[j].IDNo;
				break;
			}
		}
	}
	else if (Move == 1) {//上方向
		for (j = i - 1; j > -1; j--) {
			m_BuyDiag.GetAttribute(FCS_ShinBuy31Ex[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = FCS_ShinBuy31Ex[j].IDNo;
				break;
			}
		}
	}
	else if (Move == 2) {//右方向
		for (j = i + 1; j < FCS_SHINBUY31CNTex; j++) {
			m_BuyDiag.GetAttribute(FCS_ShinBuy31Ex_LR[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = FCS_ShinBuy31Ex_LR[j].IDNo;
				break;
			}
		}
	}
	else if (Move == 3) {//左方向
		for (j = i - 1; j > -1; j--) {
			m_BuyDiag.GetAttribute(FCS_ShinBuy31Ex_LR[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = FCS_ShinBuy31Ex_LR[j].IDNo;
				break;
			}
		}
	}
	return idx;
}

//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
void CShinBuy31Ex::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if (m_curwnd == IDC_ICSDIAGCTRL1) {
		if ((index = m_BuyDiag.GetPosition()) != -1) {
			m_BuyDiag.SetPosition(index);
			m_BuyDiag.SetFocus();
			return;
		}
	}

	this->SetFocus();
// 22-0167,22-0361 cor -->
	//if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
	//	m_BuyDiag.SetPosition(KBUYVAL01Aex);
	//}
	//else {
	//	m_BuyDiag.SetPosition(KBUYVAL04Aex);
	//}
// -----------------------
	DIAGRAM_ATTRIBUTE	da;
	if((*m_pSnHeadData)->Sn_Sign4 & 0x80)	{
		m_BuyDiag.GetAttribute(KBUYVAL01Aex,(LPUNKNOWN)&da);
		if(da.attr_bcolor == BC_CREAM || da.attr_bcolor == BC_BLUE){
			m_BuyDiag.SetPosition(KBUYVAL01Aex);
		}
		else	{
			m_BuyDiag.SetPosition(KBUYVAL01Bex);
		}
	}
	else {
		m_BuyDiag.GetAttribute(KBUYVAL04Aex,(LPUNKNOWN)&da);
		if(da.attr_bcolor == BC_CREAM || da.attr_bcolor == BC_BLUE){
			m_BuyDiag.SetPosition(KBUYVAL04Aex);
		}
		else	{
			m_BuyDiag.SetPosition(KBUYVAL04Bex);
		}
	}
// 22-0167,22-0361 cor <--
	SetRedrawScroll(0);
}

void CShinBuy31Ex::SetItemPositionEx()
{
	this->SetFocus();
	DIAGRAM_ATTRIBUTE	da;
	if((*m_pSnHeadData)->Sn_Sign4 & 0x80)	{
		m_BuyDiag.GetAttribute(KBUYVAL01Aex,(LPUNKNOWN)&da);
		if(da.attr_bcolor == BC_CREAM || da.attr_bcolor == BC_BLUE){
			m_BuyDiag.SetPosition(KBUYVAL01Aex);
		}
		else	{
			m_BuyDiag.SetPosition(KBUYVAL01Bex);
		}
	}
	else {
		m_BuyDiag.GetAttribute(KBUYVAL04Aex,(LPUNKNOWN)&da);
		if(da.attr_bcolor == BC_CREAM || da.attr_bcolor == BC_BLUE){
			m_BuyDiag.SetPosition(KBUYVAL04Aex);
		}
		else	{
			m_BuyDiag.SetPosition(KBUYVAL04Bex);
		}
	}
	SetRedrawScroll(0);
}

int CShinBuy31Ex::DispJudge(int clm, int line)
{

	int st = 0;

	KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;

	/*if (clm > 0) {
		if (line == KBUYLINE1 || line == KBUYLINE2 || line == KBUYLINE4 || line == KBUYLINE5 || line == KBUYLINE7 || line == KBUYLINE8 ||
			line == KBUYLINE10 || line == KBUYLINE11 || line == KBUYLINE16)
		{
			return st;
		}
	}*/

	/*if ( ( ( *m_pSnHeadData )->SVmzsw == 0 ) && ( clm == 1 ) ) {
		return 0;
	}*/

	switch (line) {
	case KBUYLINE6://【( )所得に係る課税仕入高】差引課税仕入高（④－⑤）
		if (clm == 0) {
			if ((pK + KBUYLINE4)->KKauto & 0x01 || (pK + KBUYLINE5)->KKauto & 0x01) {
				st = 1;
			}
			if (m_pArith->l_test((pK + KBUYLINE4)->KKval1) || m_pArith->l_test((pK + KBUYLINE5)->KKval1)) {
				st = 1;
			}
		}
		else if (clm == 1) {
			if ((pK + KBUYLINE4)->KKauto & 0x04 || (pK + KBUYLINE5)->KKauto & 0x04) {
				st = 1;
			}
			if (m_pArith->l_test((pK + KBUYLINE4)->KKval3) || m_pArith->l_test((pK + KBUYLINE5)->KKval3)) {
				st = 1;
			}
		}
		else if (clm == 2) {
			if ((pK + KBUYLINE4)->KKauto & 0x08 || (pK + KBUYLINE5)->KKauto & 0x08) {
				st = 1;
			}
			if (m_pArith->l_test((pK + KBUYLINE4)->KKval4) || m_pArith->l_test((pK + KBUYLINE5)->KKval4)) {
				st = 1;
			}
		}
		break;

	case KBUYLINE9://【 業務用資産の譲渡所得に係る課税仕入高】差引課税仕入高（⑦－⑧）
		if (clm == 0) {
			if (m_pArith->l_test((pK + KBUYLINE7)->KKval1) || (m_pArith->l_test((pK + KBUYLINE8)->KKval1))) {
				st = 1;
			}
			if ((pK + KBUYLINE7)->KKauto & 0x01 || (pK + KBUYLINE8)->KKauto & 0x01) {
				st = 1;
			}
		}
		else if (clm == 1) {
			if (m_pArith->l_test((pK + KBUYLINE7)->KKval3) || (m_pArith->l_test((pK + KBUYLINE8)->KKval3))) {
				st = 1;
			}
			if ((pK + KBUYLINE7)->KKauto & 0x04 || (pK + KBUYLINE8)->KKauto & 0x04) {
				st = 1;
			}
		}
		else if (clm == 2) {
			if (m_pArith->l_test((pK + KBUYLINE7)->KKval4) || (m_pArith->l_test((pK + KBUYLINE8)->KKval4))) {
				st = 1;
			}
			if ((pK + KBUYLINE7)->KKauto & 0x08 || (pK + KBUYLINE8)->KKauto & 0x08) {
				st = 1;
			}
		}
		
		break;

	case KBUYLINE10://課税仕入高の合計額（①＋②＋③＋⑥＋⑨）
		if (clm == 0) {
			if ((pK + KBUYLINE1)->KKauto & 0x01 || (pK + KBUYLINE2)->KKauto & 0x01) {
				st = 1;
			}
			if ((pK + KBUYLINE3)->KKauto & 0x01) {
				st = 1;
			}

			if ((pK + KBUYLINE4)->KKauto & 0x01 || (pK + KBUYLINE5)->KKauto & 0x01) {
				st = 1;
			}
			if ((pK + KBUYLINE7)->KKauto & 0x01 || (pK + KBUYLINE8)->KKauto & 0x01) {
				st = 1;
			}

			if (m_pArith->l_test((pK + KBUYLINE1)->KKval1) || m_pArith->l_test((pK + KBUYLINE2)->KKval1) ||
				m_pArith->l_test((pK + KBUYLINE3)->KKval1) ||
				m_pArith->l_test((pK + KBUYLINE4)->KKval1) || m_pArith->l_test((pK + KBUYLINE5)->KKval1) ||
				m_pArith->l_test((pK + KBUYLINE7)->KKval1) || m_pArith->l_test((pK + KBUYLINE8)->KKval1)) {
				st = 1;
			}

		}
		else if (clm == 1) {
			if ((pK + KBUYLINE1)->KKauto & 0x04 || (pK + KBUYLINE2)->KKauto & 0x04) {
				st = 1;
			}
			if ((pK + KBUYLINE3)->KKauto & 0x04) {
				st = 1;
			}

			if ((pK + KBUYLINE4)->KKauto & 0x04 || (pK + KBUYLINE5)->KKauto & 0x04) {
				st = 1;
			}
			if ((pK + KBUYLINE7)->KKauto & 0x04 || (pK + KBUYLINE8)->KKauto & 0x04) {
				st = 1;
			}

			if (m_pArith->l_test((pK + KBUYLINE1)->KKval3) || m_pArith->l_test((pK + KBUYLINE2)->KKval3) ||
				m_pArith->l_test((pK + KBUYLINE3)->KKval3) ||
				m_pArith->l_test((pK + KBUYLINE4)->KKval3) || m_pArith->l_test((pK + KBUYLINE5)->KKval3) ||
				m_pArith->l_test((pK + KBUYLINE7)->KKval3) || m_pArith->l_test((pK + KBUYLINE8)->KKval3)) {
				st = 1;
			}

			/*if ((pK + KBUYLINE3)->KKauto & 0x04 || (pK + KBUYLINE6)->KKauto & 0x04 || (pK + KBUYLINE9)->KKauto & 0x04 || (pK + KBUYLINE12)->KKauto & 0x04) {
				st = 1;
			}
			if (m_pArith->l_test((pK + KBUYLINE3)->KKval3) || m_pArith->l_test((pK + KBUYLINE6)->KKval3)
				|| m_pArith->l_test((pK + KBUYLINE9)->KKval3) || m_pArith->l_test((pK + KBUYLINE12)->KKval3)) {
				st = 1;
			}*/
		}
		
		else if (clm == 2) {
			if ((pK + KBUYLINE1)->KKauto & 0x08 || (pK + KBUYLINE2)->KKauto & 0x08) {
				st = 1;
			}
			if ((pK + KBUYLINE3)->KKauto & 0x08) {
				st = 1;
			}

			if ((pK + KBUYLINE4)->KKauto & 0x08 || (pK + KBUYLINE5)->KKauto & 0x08) {
				st = 1;
			}
			if ((pK + KBUYLINE7)->KKauto & 0x08 || (pK + KBUYLINE8)->KKauto & 0x08) {
				st = 1;
			}

			if (m_pArith->l_test((pK + KBUYLINE1)->KKval4) || m_pArith->l_test((pK + KBUYLINE2)->KKval4) ||
				m_pArith->l_test((pK + KBUYLINE3)->KKval4) ||
				m_pArith->l_test((pK + KBUYLINE4)->KKval4) || m_pArith->l_test((pK + KBUYLINE5)->KKval4) ||
				m_pArith->l_test((pK + KBUYLINE7)->KKval4) || m_pArith->l_test((pK + KBUYLINE8)->KKval4)) {
				st = 1;
			}

		}
		
		break;

	case KBUYLINE11+1://課税仕入れに係る消費税額の計算(6.24%)	// calcの関係で+1する
//			if( m_pArith->l_test((pK + KBUYLINE13)->KKval1) ){
		if (m_pArith->l_test((pK + KBUYLINE10)->KKval3)) {
			st = 1;
		}
		if ((pK + KBUYLINE11+1)->KKauto & 0x03) {	// calcの関係で+1する
			st = 1;
		}
		break;

	case KBUYLINE12+1://課税仕入れに係る消費税額の計算(7.8%)	// calcの関係で+1する
//			if( m_pArith->l_test((pK + KBUYLINE13)->KKval1) ){
		if (m_pArith->l_test((pK + KBUYLINE10)->KKval4)) {
			st = 1;
		}
		if ((pK + KBUYLINE12+1)->KKauto & 0x03) {
			st = 1;
		}
		break;


	default:
		if (clm == 0) {
			if (m_pArith->l_test((pK + line)->KKval1)) {
				st = 1;
			}
			if ((pK + line)->KKauto & 0x01) {
				st = 1;
			}
		}
		else if (clm == 1) {
			if (m_pArith->l_test((pK + line)->KKval3)) {
				st = 1;
			}
			if ((pK + line)->KKauto & 0x04) {
				st = 1;
			}
		}
		else if (clm == 2) {
			if (m_pArith->l_test((pK + line)->KKval4)) {
				st = 1;
			}
			if ((pK + line)->KKauto & 0x08) {
				st = 1;
			}
		}
		break;
	}

	return st;
}

void CShinBuy31Ex::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd = -1;
	m_FcsLeftEnd = -1;
	m_FcsDownEnd = -1;
	m_FcsRightEnd = -1;

	for (int idx = 0; idx < FCS_SHINBUY31CNTex; idx++) {
		m_BuyDiag.GetAttribute(FCS_ShinBuy31Ex[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
			m_FcsUpEnd = FCS_ShinBuy31Ex[idx].IDNo;
			break;
		}
	}
	for (int idx = 0; idx < FCS_SHINBUY31CNTex; idx++) {
		m_BuyDiag.GetAttribute(FCS_ShinBuy31Ex_LR[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
			m_FcsLeftEnd = FCS_ShinBuy31Ex_LR[idx].IDNo;
			break;
		}
	}
	for (int idx = FCS_SHINBUY31CNTex - 1; idx > -1; idx--) {
		m_BuyDiag.GetAttribute(FCS_ShinBuy31Ex[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
			m_FcsDownEnd = FCS_ShinBuy31Ex[idx].IDNo;
			break;
		}
	}
	for (int idx = FCS_SHINBUY31CNTex - 1; idx > -1; idx--) {
		m_BuyDiag.GetAttribute(FCS_ShinBuy31Ex_LR[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
			m_FcsRightEnd = FCS_ShinBuy31Ex_LR[idx].IDNo;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 画面表示
//-----------------------------------------------------------------------------
// 引数	mode	：	
//-----------------------------------------------------------------------------
void CShinBuy31Ex::DispList(int mode)
{
	m_BuyDiag.OffControl(TRUE);
	DispBuyDiagData();
	m_BuyDiag.UpdateWindow();

	DIAGRAM_DATA	data;
	if (!mode) {
		m_Util.DiagOcxIniz(&data);
		diag_clear(IDC_ICSDIAGCTRL1, BKYEARIDex, TRUE, CIcsdiagctrl);
		data.data_disp.Format("（令和%2x年分）", (*m_pTaxListData)->YEAR);
		diag_setdata(IDC_ICSDIAGCTRL1, BKYEARIDex, &data, CIcsdiagctrl);

		PostMessage(WM_VSCROLL, SB_TOP, 0);
		m_Idx = BKVMINex;

		if ((*m_pTaxListData)->GKBN & 0x04) {//農業
		}
		else {
			if ((*m_pTaxListData)->GKBN & 0x02) {//不動産
				m_Idx = KBUYVAL03Aex;
			}
			else {
				m_Idx = BKVMINex;
			}
		}
	}

	m_Util.DiagOcxIniz(&data);
	diag_clear(IDC_ICSDIAGCTRL1, BKZKBNIDex, TRUE, CIcsdiagctrl);
	data.data_disp = _T("経理方式：");
	if ((*m_pTaxListData)->Head.KHstax & 0x01) {
		data.data_disp += _T("税抜き");
	}
	else {
		data.data_disp += _T("税込み");
	}
	diag_setdata(IDC_ICSDIAGCTRL1, BKZKBNIDex, &data, CIcsdiagctrl);

	if (mode) {
		/*if (m_Idx == KBUYVAL11K && !(*m_pTaxListData)->Head.KHstax) {
			m_Idx = KBUYVAL11C;
		}*/
		diag_setposition(IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl);
	}

	//diag_setposition(IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl);

	//SetRedrawScrollToIdx(m_BuyKei.GetPosition());

// 22-0167,22-0361 add -->
	// (pK+3)について、3と4以外は連動時にクリアされるので3を使用する
	// ※事業・農業・不動産については連動時は自動計算チェックを含む
	// 　すべてがクリアされるので問題なし
	KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;
	int		idx=0;
	for(idx=KBUYChk1ex; idx<=KBUYChk3ex; idx++)	{
		diag_getdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
		data.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
	}
	idx=0;
	if((*m_pSnHeadData)->Sn_GenericSgn & 0x800)		idx=KBUYChk3ex;
	//else if((pK+3)->KKauto & 0x40)	idx=KBUYChk2ex;
	//else if((pK+3)->KKauto & 0x80)	idx=KBUYChk3ex;
	if(idx != 0)	{
		diag_getdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
		data.data_check = 1;
		diag_setdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
	}
	//// 確定時は選択不可
	//if((*m_pSnHeadData)->Sn_Sign4 & 0x01)	{
	//	for(idx=KBUYChk1ex; idx<=KBUYChk3ex; idx++)	{
	//		ATRIB_MOD(IDC_ICSDIAGCTRL1,idx,0,(*m_pSnHeadData)->Sn_Sign4,0);
	//	}
	//}
	// 金額、6.24%適用分は非表示にする(今後要望があったときのために残しておく)
	diag_modify(IDC_ICSDIAGCTRL1,KBUYChk1ex,DIAG_MDFY_TRANSPARENT,CIcsdiagctrl);
	diag_modify(IDC_ICSDIAGCTRL1,KBUYChk2ex,DIAG_MDFY_TRANSPARENT,CIcsdiagctrl);
	// 確定時は7.8%適用分を選択不可にする
	if((*m_pSnHeadData)->Sn_Sign4 & 0x01)	{
		//ATRIB_MOD(IDC_ICSDIAGCTRL1,KBUYChk3ex,0,(*m_pSnHeadData)->Sn_Sign4,0);
		DIAGRAM_ATTRIBUTE	da;
		diag_modify(IDC_ICSDIAGCTRL1,KBUYChk3ex,DIAG_MDFY_READONLY,CIcsdiagctrl);
		diag_getattr(IDC_ICSDIAGCTRL1,KBUYChk3ex,&da,CIcsdiagctrl);
		da.attr_bcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAGCTRL1,KBUYChk3ex,&da,FALSE,CIcsdiagctrl);
	}
	else	{
		DIAGRAM_ATTRIBUTE	da,da2;
		diag_getattr(IDC_ICSDIAGCTRL1,0,&da2,CIcsdiagctrl);
		diag_getattr(IDC_ICSDIAGCTRL1,KBUYChk3ex,&da,CIcsdiagctrl);
		da.attr_bcolor = da2.attr_bcolor;
		diag_setattr(IDC_ICSDIAGCTRL1,KBUYChk3ex,&da,FALSE,CIcsdiagctrl);
		diag_modify(IDC_ICSDIAGCTRL1,KBUYChk3ex,DIAG_MDFY_EDIT,CIcsdiagctrl);
	}

// 22-0167,22-0361 add <--
}

//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
void CShinBuy31Ex::DispBuyDiagData()
{

	short	idx, i;
	KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;
	DIAGRAM_DATA	data;
	int		st = 0, skip = 0;
	short	j = 0;

	//'14.08.20
	for (i = 0; i < BKLINCNTex+1; i++) {

		////------>'14.10.08
		////農業の部分スキップ ???
		//if (i == 3 || i == 4 || i == 5) {
		//	continue;
		//}
		////<-------------------

		for (j = 0; j < 3; j++) {

			st = 0;
			skip = 0;

			m_Util.DiagOcxIniz(&data);

			switch (i) {

				//--------->'14.10.08
			case KBUYLINE1:
				idx = KBUYVAL01Aex + j;//インデックスをセット
				break;
			case KBUYLINE2:
				idx = KBUYVAL02Aex + j;//インデックスをセット
				break;
			case KBUYLINE3:
				idx = KBUYVAL03Aex + j;//インデックスをセット
				break;
			case KBUYLINE4:
				idx = KBUYVAL04Aex + j;//インデックスをセット
				break;
			case KBUYLINE5:
				idx = KBUYVAL05Aex + j;//インデックスをセット
				break;
			case KBUYLINE6:
				idx = KBUYVAL06Aex + j;//インデックスをセット
				break;
			case KBUYLINE7:
				idx = KBUYVAL07Aex + j;//インデックスをセット
				break;
			case KBUYLINE8:
				idx = KBUYVAL08Aex + j;//インデックスをセット
				break;
			case KBUYLINE9:
				idx = KBUYVAL09Aex + j;//インデックスをセット
				break;
			case KBUYLINE10:
				idx = KBUYVAL10Aex + j;//インデックスをセット
				break;
			case KBUYLINE11+1:	// calcの関係で+1する
				switch (j) {
				case 0:
					idx = KBUYVAL11ex;
					break;
				case 1:
					idx = KBUYVAL11Bex;
					break;
				case 2:
					idx = KBUYVAL11Kex;
					break;
				}
				break;
			case KBUYLINE12+1:	// calcの関係で+1する
				switch (j) {
				case 0:
					idx = KBUYVAL12ex;
					break;
				case 1:
					idx = KBUYVAL12Bex;
					break;
				case 2:
					idx = KBUYVAL12Kex;
					break;
				}
				break;
			default:
				skip = 1;
				break;
			}

			if( skip ){
				continue;
			}

			diag_clear(IDC_ICSDIAGCTRL1, idx, TRUE, CIcsdiagctrl);


			st = DispJudge(j, i);

			if (st) {
				/*if (j == 0)			memcpy(data.data_val, (pK + i)->KKval1, 6);
				else if (j == 1)	memcpy(data.data_val, (pK + i)->KKval3, 6);
				else if (j == 2)	memcpy(data.data_val, (pK + i)->KKval4, 6);*/

				switch( i ){
					case KBUYLINE11+1:	// calcの関係で+1する
					case KBUYLINE12+1:
						if (j == 0)			memcpy(data.data_val, (pK + i)->KKval1, 6);
						else if (j == 1)	memcpy(data.data_val, (pK + i)->KKval2, 6);
						else if (j == 2)	memcpy(data.data_val, (pK + i)->KKval3, 6);
						break;
					default:
						if (j == 0)			memcpy(data.data_val, (pK + i)->KKval1, 6);
						else if (j == 1)	memcpy(data.data_val, (pK + i)->KKval3, 6);
						else if (j == 2)	memcpy(data.data_val, (pK + i)->KKval4, 6);
						break;
				}

				diag_setdata(IDC_ICSDIAGCTRL1, idx, &data, CIcsdiagctrl);
			}
		}
	}

	// 項目名
	char	buf[128];
	::ZeroMemory(buf, sizeof(buf));
	m_Util.DiagOcxIniz(&data);
	diag_clear(IDC_ICSDIAGCTRL1, KBUYNAMEex, TRUE, CIcsdiagctrl);

	data.data_edit =  m_Util.CutHankaku((*m_pTaxListData)->Head.KHksnm, sizeof((*m_pTaxListData)->Head.KHksnm));;
	diag_setdata(IDC_ICSDIAGCTRL1, KBUYNAMEex, &data, CIcsdiagctrl);


	// 11	仮払消費税
	st = 0;
	m_Util.DiagOcxIniz(&data);
	diag_clear(IDC_ICSDIAGCTRL1, KBUYVAL11Kex, TRUE, CIcsdiagctrl);
	if ((*m_pTaxListData)->Head.KHstax) {
		if (m_pArith->l_test((pK + KBUYLINE10)->KKval3)) {
			st = 1;
		}
		if ((pK + KBUYLINE11+1)->KKauto & 0x04) {		// calcの関係で+1する
			st = 1;
		}
	}
	if (st) {
		memcpy(data.data_val, (pK + KBUYLINE11+1)->KKval3, 6);		// calcの関係で+1する
		diag_setdata(IDC_ICSDIAGCTRL1, KBUYVAL11Kex, &data, CIcsdiagctrl);
	}

	// 11	計算部
	st = 0;
	m_Util.DiagOcxIniz(&data);
	diag_clear(IDC_ICSDIAGCTRL1, KBUYVAL11Bex, TRUE, CIcsdiagctrl);
	if (l_test((pK + KBUYLINE10)->KKval3)) {
		st = 1;
	}
	if ((pK + KBUYLINE11+1)->KKauto & 0x02) {
		st = 1;
	}
	if (st) {
		memcpy(data.data_val, (pK + KBUYLINE11+1)->KKval2, 6);
		diag_setdata(IDC_ICSDIAGCTRL1, KBUYVAL11Bex, &data, CIcsdiagctrl);
	}

	// 12	仮払消費税
	st = 0;
	m_Util.DiagOcxIniz(&data);
	diag_clear(IDC_ICSDIAGCTRL1, KBUYVAL12Kex, TRUE, CIcsdiagctrl);
	if ((*m_pTaxListData)->Head.KHstax) {
		if (m_pArith->l_test((pK + KBUYLINE10)->KKval4)) {
			st = 1;
		}
		if ((pK + KBUYLINE12+1)->KKauto & 0x04) {
			st = 1;
		}
	}
	if (st) {
		memcpy(data.data_val, (pK + KBUYLINE12+1)->KKval3, 6);
		diag_setdata(IDC_ICSDIAGCTRL1, KBUYVAL12Kex, &data, CIcsdiagctrl);
	}

	// 12	計算部
	st = 0;
	m_Util.DiagOcxIniz(&data);
	diag_clear(IDC_ICSDIAGCTRL1, KBUYVAL12Bex, TRUE, CIcsdiagctrl);
	if (l_test((pK + KBUYLINE10)->KKval4)) {
		st = 1;
	}
	if ((pK + KBUYLINE12+1)->KKauto & 0x02) {
		st = 1;
	}
	if (st) {
		memcpy(data.data_val, (pK + KBUYLINE12+1)->KKval2, 6);
		diag_setdata(IDC_ICSDIAGCTRL1, KBUYVAL12Bex, &data, CIcsdiagctrl);
	}

	///////////背景色変更
	int cnt = 0;
	int Flg = 0;
	int line = -1;
// 22-0167,22-0361 add -->
	int colm = -1;
// 22-0167,22-0361 add <--
	unsigned char sgn = 0x00;

	while (ShinBuy31Ex[cnt].Index != 0) {
		if ((ShinBuy31Ex[cnt].Index == KBUYVAL01Aex) || (ShinBuy31Ex[cnt].Index == KBUYVAL01Bex) || (ShinBuy31Ex[cnt].Index == KBUYVAL01Cex) ||
			(ShinBuy31Ex[cnt].Index == KBUYVAL02Aex) || (ShinBuy31Ex[cnt].Index == KBUYVAL02Bex) || (ShinBuy31Ex[cnt].Index == KBUYVAL02Cex) ||
			(ShinBuy31Ex[cnt].Index == KBUYVAL03Aex) || (ShinBuy31Ex[cnt].Index == KBUYVAL03Bex) || (ShinBuy31Ex[cnt].Index == KBUYVAL03Cex) ||
// 22-0167,22-0361 add -->
			(ShinBuy31Ex[cnt].Index == KBUYVAL04Aex) || (ShinBuy31Ex[cnt].Index == KBUYVAL04Bex) || (ShinBuy31Ex[cnt].Index == KBUYVAL04Cex) ||
			(ShinBuy31Ex[cnt].Index == KBUYVAL05Aex) || (ShinBuy31Ex[cnt].Index == KBUYVAL05Bex) || (ShinBuy31Ex[cnt].Index == KBUYVAL05Cex) ||
// 22-0167,22-0361 add <--
			(ShinBuy31Ex[cnt].Index == KBUYVAL07Aex) || (ShinBuy31Ex[cnt].Index == KBUYVAL07Bex) || (ShinBuy31Ex[cnt].Index == KBUYVAL07Cex) ||
			(ShinBuy31Ex[cnt].Index == KBUYVAL08Aex) || (ShinBuy31Ex[cnt].Index == KBUYVAL08Bex) || (ShinBuy31Ex[cnt].Index == KBUYVAL08Cex) ||
			(ShinBuy31Ex[cnt].Index == KBUYVAL11Kex) || (ShinBuy31Ex[cnt].Index == KBUYVAL11Bex) || (ShinBuy31Ex[cnt].Index == KBUYVAL11ex)  ||
			(ShinBuy31Ex[cnt].Index == KBUYVAL12Kex) || (ShinBuy31Ex[cnt].Index == KBUYVAL12Bex) || (ShinBuy31Ex[cnt].Index == KBUYVAL12ex)) {
// 22-0167,22-0361 add -->
			colm = GetColumNum(ShinBuy31Ex[cnt].Index);
// 22-0167,22-0361 add <--
			// 確定時は全入力不可
			if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
				Flg = ShinBuy31Ex[cnt].AllUnConnect;
			}
			else {
				if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
					// 経過措置ありのときは6.3％入力不可
					/*if (!((*m_pSnHeadData)->SVmzsw) && (ShinBuy31Ex[cnt].UnConnect == 2)) {
						Flg = 0;
					}
					else
						Flg = ShinBuy31Ex[cnt].UnConnect;*/
					Flg = ShinBuy31Ex[cnt].UnConnect;
				}
				else {
					// 経過措置ありのときは6.3％入力不可
					/*if (!((*m_pSnHeadData)->SVmzsw) && (ShinBuy31Ex[cnt].Connect == 2)) {
						Flg = 0;
					}
					else
						Flg = ShinBuy31Ex[cnt].Connect;*/
					Flg = ShinBuy31Ex[cnt].Connect;
				}
			}

			if (Flg) {
				DIAGRAM_ATTRIBUTE attr;
				diag_getattr(IDC_ICSDIAGCTRL1, ShinBuy31Ex[cnt].Index, &attr, CIcsdiagctrl);

				switch (ShinBuy31Ex[cnt].Index) {
// 22-0167,22-0361 add -->
				case KBUYVAL04Aex:
				case KBUYVAL04Bex:
				case KBUYVAL04Cex:
				case KBUYVAL05Aex:
				case KBUYVAL05Bex:
				case KBUYVAL05Cex:
// 161147 add -->
				case KBUYVAL07Aex:
				case KBUYVAL07Bex:
				case KBUYVAL07Cex:
				case KBUYVAL08Aex:
				case KBUYVAL08Bex:
				case KBUYVAL08Cex:
// 161147 add <--
					attr.attr_bcolor = BC_CREAM;
					break;
// 22-0167,22-0361 add <--
				case KBUYVAL01Aex:
				case KBUYVAL01Bex:
				case KBUYVAL01Cex:
				case KBUYVAL02Aex:
				case KBUYVAL02Bex:
				case KBUYVAL02Cex:
				case KBUYVAL03Aex:
				case KBUYVAL03Bex:
				case KBUYVAL03Cex:
// 161147 del -->
				//case KBUYVAL07Aex:
				//case KBUYVAL07Bex:
				//case KBUYVAL07Cex:
				//case KBUYVAL08Aex:
				//case KBUYVAL08Bex:
				//case KBUYVAL08Cex:
// 161147 del <--

					line = -1;
					switch (ShinBuy31Ex[cnt].Index) {
					case KBUYVAL01Aex:
					case KBUYVAL01Bex:
					case KBUYVAL01Cex:
						line = KBUYLINE1;
						break;
					case KBUYVAL02Aex:
					case KBUYVAL02Bex:
					case KBUYVAL02Cex:
						line = KBUYLINE2;
						break;
					case KBUYVAL03Aex:
					case KBUYVAL03Bex:
					case KBUYVAL03Cex:
						line = KBUYLINE3;
						break;

					case KBUYVAL07Aex:
					case KBUYVAL07Bex:
					case KBUYVAL07Cex:
						line = KBUYLINE7;
						break;

					case KBUYVAL08Aex:
					case KBUYVAL08Bex:
					case KBUYVAL08Cex:
						line = KBUYLINE8;
						break;
					}

					sgn = 0x00;
					switch (ShinBuy31Ex[cnt].Index) {
					case KBUYVAL01Aex:
					case KBUYVAL02Aex:
					case KBUYVAL03Aex:
					case KBUYVAL07Aex:
					case KBUYVAL08Aex:
						sgn = 0x01;
						break;
					case KBUYVAL01Bex:
					case KBUYVAL02Bex:
					case KBUYVAL03Bex:
					case KBUYVAL07Bex:
					case KBUYVAL08Bex:
						sgn = 0x04;
						break;
					case KBUYVAL01Cex:
					case KBUYVAL02Cex:
					case KBUYVAL03Cex:
					case KBUYVAL07Cex:
					case KBUYVAL08Cex:
						sgn = 0x08;
						break;
					}

					if ((*m_pTaxListData)->NRec.KSval[line].KKauto&sgn) {
						attr.attr_bcolor = BC_BLUE;
					}
					else {
						attr.attr_bcolor = BC_CREAM;
					}

					break;
				case KBUYVAL11Kex:
					if ((*m_pTaxListData)->NRec.KSval[KBUYLINE11+1].KKauto & 0x04) {
						attr.attr_bcolor = BC_BLUE;
					}
					else {
						attr.attr_bcolor = BC_CREAM;
					}
					break;
				case KBUYVAL11Bex:
					if ((*m_pTaxListData)->NRec.KSval[KBUYLINE11+1].KKauto & 0x02) {
						attr.attr_bcolor = BC_BLUE;
					}
					else {
						attr.attr_bcolor = BC_CREAM;
					}
					break;
				case KBUYVAL11ex:
					if ((*m_pTaxListData)->NRec.KSval[KBUYLINE11+1].KKauto & 0x01) {
						attr.attr_bcolor = BC_BLUE;
					}
					else {
						attr.attr_bcolor = BC_CREAM;
					}
					break;
				case KBUYVAL12Kex:
					if ((*m_pTaxListData)->NRec.KSval[KBUYLINE12+1].KKauto & 0x04) {
						attr.attr_bcolor = BC_BLUE;
					}
					else {
						attr.attr_bcolor = BC_CREAM;
					}
					break;
				case KBUYVAL12Bex:
					if ((*m_pTaxListData)->NRec.KSval[KBUYLINE12+1].KKauto & 0x02) {
						attr.attr_bcolor = BC_BLUE;
					}
					else {
						attr.attr_bcolor = BC_CREAM;
					}
					break;
				case KBUYVAL12ex:
					if ((*m_pTaxListData)->NRec.KSval[KBUYLINE12+1].KKauto & 0x01) {
						attr.attr_bcolor = BC_BLUE;
					}
					else {
						attr.attr_bcolor = BC_CREAM;
					}
					break;
					
				default:
					break;
				}

// 22-0167,22-0361 cor -->
				//diag_setattr(IDC_ICSDIAGCTRL1, ShinBuy31Ex[cnt].Index, &attr, FALSE, CIcsdiagctrl);
// -----------------------
				int	flg2=-1;	// 0:列が自動計算　1:列が自動計算ではない
				//if(colm == 0 || colm == 1 || colm == 2)	{
				//	if(colm == 0)	{
				//		if((pK+3)->KKauto & 0x10)	flg2=0;
				//		else						flg2=1;
				//	}
				//	else if(colm == 1)	{
				//		if((pK+3)->KKauto & 0x40)	flg2=0;
				//		else						flg2=1;
				//	}
				//	else if(colm == 2)	{
				//		if((pK+3)->KKauto & 0x80)	flg2=0;
				//		else						flg2=1;
				//	}
				//}
				if(colm == 2)	{
					if((*m_pSnHeadData)->Sn_GenericSgn & 0x800)	flg2=0;
					else										flg2=1;
				}
				// 金額列であれば入力可／不可を切り替える
				if(flg2 != -1)	{
					ATRIB_MOD(IDC_ICSDIAGCTRL1,ShinBuy31Ex[cnt].Index,flg2,(*m_pSnHeadData)->Sn_Sign4,ShinBuy31Ex[cnt].Connect);
				}
				// 列が自動計算でなければ背景色を付ける
				if(flg2 != 0)	{
					diag_setattr(IDC_ICSDIAGCTRL1,ShinBuy31Ex[cnt].Index,&attr,FALSE,CIcsdiagctrl);
				}
// 22-0167,22-0361 cor <--
			}
		}
		cnt++;
	}

}


//-----------------------------------------------------------------------------
// 各入力データ抽出
//-----------------------------------------------------------------------------
int CShinBuy31Ex::GetDiagData(long index, short leng, DIAGRAM_DATA *data, int mode)
{
// 22-0167,22-0361 cor -->
	//if ( index < 0 ) {
// -----------------------
	if(index < 0 || index == KBUYChk1ex || index == KBUYChk2ex || index == KBUYChk3ex) {
// 22-0167,22-0361 cor <--
		return 0;
	}

	char	dbuf[128], buf[128];
	char	*buff;
	int		len;
	KKVAL*	pK;
	int		Lin_tmp = -1;

	len = data->data_edit.GetLength();
	memset(dbuf, '\0', sizeof(dbuf));
	buff = (char *)((LPCTSTR)(data->data_edit));
	memmove(dbuf, buff, len);

	if ((m_Idx == KBUYVAL11Kex) || (m_Idx == KBUYVAL12Kex)) {	//　仮払消費税？
		if (!(*m_pTaxListData)->Head.KHstax & 0x01) {
			DispBuyDiagData();
			return 0;
		}
	}

	int clm_pos = 0;

	if ((m_Idx == KBUYVAL11Kex) || (m_Idx == KBUYVAL12Kex)) {
		clm_pos = 2;
	}
	else if ((m_Idx == KBUYVAL11Bex) || (m_Idx == KBUYVAL12Bex)) {
		clm_pos = 1;
	}
	else if ((m_Idx == KBUYVAL11ex) || (m_Idx == KBUYVAL12ex)) {
		clm_pos = 0;
	}
	else {
		clm_pos = (m_Idx - BKVMINex) % 3;
	}

	if (m_Lin == 0)			Lin_tmp = KBUYLINE1;
	else if (m_Lin == 1)	Lin_tmp = KBUYLINE2;
	else if (m_Lin == 2)	Lin_tmp = KBUYLINE3;
	else if (m_Lin == 3)	Lin_tmp = KBUYLINE4;
	else if (m_Lin == 4)	Lin_tmp = KBUYLINE5;
	else if (m_Lin == 5)	Lin_tmp = KBUYLINE6;
	else if (m_Lin == 6)	Lin_tmp = KBUYLINE7;
	else if (m_Lin == 7)	Lin_tmp = KBUYLINE8;
	else if (m_Lin == 8)	Lin_tmp = KBUYLINE9;
	else if (m_Lin == 9)	Lin_tmp = KBUYLINE10;
	else if (m_Lin == 10)	Lin_tmp = KBUYLINE11+1;	// calcの関係で+1する
	else if (m_Lin == 11)	Lin_tmp = KBUYLINE12+1;	// calcの関係で+1する


	if (!mode) {
		if (m_Idx == KBUYNAMEex) {
			::ZeroMemory(buf, sizeof(buf));
			m_Util.LineDataGet(dbuf, buf, sizeof(dbuf));
			memcpy((*m_pTaxListData)->Head.KHksnm, buf, sizeof((*m_pTaxListData)->Head.KHksnm));
		}
		else {
			pK = &(*m_pTaxListData)->NRec.KSval[Lin_tmp];
			if (leng) {

				switch (Lin_tmp) {
				case KBUYLINE1:
				case KBUYLINE2:
				case KBUYLINE3:
				/*case KBUYLINE4:
				case KBUYLINE5:
				case KBUYLINE6:*/
					if (clm_pos == 0) {
						pK->KKauto |= 0x01;
					}
					else if (clm_pos == 1) {
						pK->KKauto |= 0x04;
					}
					else if (clm_pos == 2) {
						pK->KKauto |= 0x08;
					}
					break;
				case KBUYLINE11+1:	// calcの関係で+1する
				case KBUYLINE12+1:
					if (clm_pos == 0) {
						pK->KKauto |= 0x01;
					}
					else if (clm_pos == 1) {
						pK->KKauto |= 0x02;
					}
					else if (clm_pos == 2) {
						pK->KKauto |= 0x04;
					}
					break;

				default:
					if (m_pArith->l_test(data->data_val)) {
						if (clm_pos == 0) {
							pK->KKauto |= 0x01;
						}
						else if (clm_pos == 1) {
							pK->KKauto |= 0x04;
						}
						else if (clm_pos == 2) {
							pK->KKauto |= 0x08;
						}
					}
					else {
						if (clm_pos == 0) {
							pK->KKauto &= 0xfe;
						}
						else if (clm_pos == 1) {
							pK->KKauto &= 0xfb;
						}
						else if (clm_pos == 2) {
							pK->KKauto &= 0xf7;
						}
					}
					break;
				}

				/*if (clm_pos == 0)		memcpy(pK->KKval1, data->data_val, 6);
				else if (clm_pos == 1)	memcpy(pK->KKval3, data->data_val, 6);
				else if (clm_pos == 2)  memcpy(pK->KKval4, data->data_val, 6);*/

				switch( Lin_tmp ){
					case KBUYLINE11+1:	// calcの関係で+1する
					case KBUYLINE12+1:
						if (clm_pos == 0)		memcpy(pK->KKval1, data->data_val, 6);
						else if (clm_pos == 1)	memcpy(pK->KKval2, data->data_val, 6);
						else if (clm_pos == 2)  memcpy(pK->KKval3, data->data_val, 6);
						break;

					default:
						if (clm_pos == 0)		memcpy(pK->KKval1, data->data_val, 6);
						else if (clm_pos == 1)	memcpy(pK->KKval3, data->data_val, 6);
						else if (clm_pos == 2)  memcpy(pK->KKval4, data->data_val, 6);
						break;
				}
			}
		}
	}
	else {
		if (m_Idx == KBUYNAMEex) {
			::ZeroMemory((*m_pTaxListData)->Head.KHksnm, 6);
		}
		else {
			pK = &(*m_pTaxListData)->NRec.KSval[Lin_tmp];
			if (m_Idx == KBUYVAL11Kex) {
				pK->KKauto &= 0xfb;
				::ZeroMemory(pK->KKval3, 6);
			}
			else if (m_Idx == KBUYVAL11Bex) {
				pK->KKauto &= 0xfd;
				::ZeroMemory(pK->KKval2, 6);
			}
			else if (m_Idx == KBUYVAL12Kex) {
				pK->KKauto &= 0xfb;
				::ZeroMemory(pK->KKval3, 6);
			}
			else if (m_Idx == KBUYVAL12Bex) {
				pK->KKauto &= 0xfd;
				::ZeroMemory(pK->KKval2, 6);
			}
			else {
				if (clm_pos == 0) {
					pK->KKauto &= 0xfe;
					::ZeroMemory(pK->KKval1, 6);
				}
				else if (clm_pos == 1) {
					pK->KKauto &= 0xfb;
					::ZeroMemory(pK->KKval3, 6);
				}
				else if (clm_pos == 2) {
					pK->KKauto &= 0xf7;
					::ZeroMemory(pK->KKval4, 6);
				}
			}
			
		}
	}

	// 集計
	(*m_Scalc)->Shz_Buy31Post();
	DispBuyDiagData();
	

	return 0;
}


// 手入力サインのクリア
int CShinBuy31Ex::ClearManualInputSign()
{
	int rt = 0;

	for (int idx = 0; idx < 50; idx++) {
		(*m_pTaxListData)->NRec.KSval[idx].KKauto = 0x00;
	}

	//事業所得の金額をクリアする
	//金額クリア後に金額を連動してくる為
	//for (int i = 0; i < 6; i++) {
	for (int i = 0; i < 3; i++) {
		memset((*m_pTaxListData)->NRec.KSval[i].KKval1, '\0', sizeof((*m_pTaxListData)->NRec.KSval[i].KKval1));
		memset((*m_pTaxListData)->NRec.KSval[i].KKval2, '\0', sizeof((*m_pTaxListData)->NRec.KSval[i].KKval2));
		memset((*m_pTaxListData)->NRec.KSval[i].KKval3, '\0', sizeof((*m_pTaxListData)->NRec.KSval[i].KKval3));
		memset((*m_pTaxListData)->NRec.KSval[i].KKval4, '\0', sizeof((*m_pTaxListData)->NRec.KSval[i].KKval4));
	}

	return rt;
}

// 印刷情報のセット
void CShinBuy31Ex::SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo )
{
	ASSERT(pVprn);
	ASSERT(pPset);

	m_pVprn = pVprn;
	m_pPset = pPset;
	m_PrintCmInfo = pPrintCmInfo;

}

#define		Msiz	512*20
// 印刷１
int CShinBuy31Ex::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j;
	char	cnm[128], buf[128], CVAL[6];
	int		len;
	KKVAL*	pS = (*m_pTaxListData)->NRec.KSval;

	// 初期化
	memset(PF, '\0', sizeof(PF));


	i = 0;
	memset(pBp, '\0', Msiz);
	char	*pBpMv = pBp;
	//	pBpMv = BP;
	::ZeroMemory(CVAL, sizeof(CVAL));

	// 会社名
	::ZeroMemory(buf, sizeof(buf));
	::ZeroMemory(cnm, sizeof(cnm));

	if (m_pPset->psSigCoprn & 0x2) {
		// 顧問先
		if (m_PrintCmInfo->OPL) {
			//			sprintf_s( buf, sizeof( buf ), _T(" %s"), CopNam );
			sprintf_s( buf, sizeof( buf ), _T(" %s"), m_PrintCmInfo->CopNam );
		}
		else {
			//			sprintf_s( buf, sizeof( buf ), _T("[%08d] %s"), CopNum, CopNam );
			sprintf_s( buf, sizeof( buf ), _T("[%08d] %s"), m_PrintCmInfo->CopNum, m_PrintCmInfo->CopNam );
		}
	}
	else {
		// マスター
		if ((*m_pSnHeadData)->IsSoleProprietor()) {
			j = sizeof((*m_pSnHeadData)->Sn_DIHYO);
			memmove(cnm, (*m_pSnHeadData)->Sn_DIHYO, j);
		}
		else {			// 法人名
			j = sizeof((*m_pSnHeadData)->Sn_CONAM);
			memmove(cnm, (*m_pSnHeadData)->Sn_CONAM, j);
		}
		if (m_PrintCmInfo->OPL) {
			sprintf_s(buf, sizeof(buf), _T(" %s"), cnm);
		}
		else {
			sprintf_s(buf, sizeof(buf), _T("[%08d] %s"), m_pZmsub->zvol->v_cod, cnm);
		}
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (len + 1);

	// 年分
	::ZeroMemory(buf, sizeof(buf));
	sprintf_s(buf, sizeof(buf), _T("%x"), (*m_pTaxListData)->YEAR);
	memmove(pBpMv, buf, 2);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (2 + 1);

	return _xprn(pno, fno, PF, 0, pDC, 0, 1);
}
// 印刷２
int CShinBuy31Ex::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j, lin, max;
	char	buf[128], CVAL[6];
	int		len, st;
	KKVAL*	pK;
	KKVAL*	pS = (*m_pTaxListData)->NRec.KSval;

	// 初期化
	memset(PF, '\0', sizeof(PF));


	i = 0;
	memset(pBp, '\0', Msiz);
	char	*pBpMv = pBp;
	//	pBpMv = BP;
	::ZeroMemory(CVAL, sizeof(CVAL));

	// 会社名
	::ZeroMemory(buf, sizeof(buf));
	//	::ZeroMemory( cnm, sizeof(cnm) );


		// 事業所得に係る課税仕入高
		// 金額
	lin = 0;	max = 2;
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		if (DispJudge(0, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval1, FMTX130);
		}

		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;

	//うち税率6.3％適用分
	/*for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		if (DispJudge(1, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval2, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;*/

	//うち税率6.24％適用分
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		//if (DispJudge(2, l_idx + lin) == 1) {
		if (DispJudge(1, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval3, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;

	//うち税率7.8％適用分
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		//if (DispJudge(3, l_idx + lin) == 1) {
		if (DispJudge(2, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval4, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;
	// 不動産所得に係る課税売上高
	// 金額
	lin = 2;	max = 3;
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		if (DispJudge(0, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval1, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;

	//うち税率6.3％適用分
	/*for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		if (DispJudge(1, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval2, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;*/

	//うち税率6.24％適用分
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		//if (DispJudge(2, l_idx + lin) == 1) {
		if (DispJudge(1, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval3, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;

	//うち税率7.8％適用分
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		//if (DispJudge(3, l_idx + lin) == 1) {
		if (DispJudge(2, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval4, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;

	// (     )に係る課税売上高
	// 金額
	lin = 3;	max = 6;
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		if (DispJudge(0, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval1, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;

	//うち税率6.3％適用分
	/*for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		if (DispJudge(1, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval2, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;*/

	//うち税率6.24％適用分
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		//if (DispJudge(2, l_idx + lin) == 1) {
		if (DispJudge(1, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval3, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;

	//うち税率7.8％適用分
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		//if (DispJudge(3, l_idx + lin) == 1) {
		if (DispJudge(2, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval4, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;

	// 業務用資産の譲渡所得に係る課税売上高
	// 金額
	lin = 6;	max = 9;
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		if (DispJudge(0, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval1, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;

	//うち税率6.3％適用分
	/*for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		if (DispJudge(1, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval2, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;*/

	//うち税率6.24％適用分
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		//if (DispJudge(2, l_idx + lin) == 1) {
		if (DispJudge(1, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval3, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;

	//うち税率7.8％適用分
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KSval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		//if (DispJudge(3, l_idx + lin) == 1) {
		if (DispJudge(2, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval4, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;

	// 課税売上高の合計
	// 金額
	pK = &(*m_pTaxListData)->NRec.KSval[KBUYLINE10];
	::ZeroMemory(buf, sizeof(buf));

	if (DispJudge(0, KBUYLINE10) == 1) {
		m_pArith->l_print(buf, pK->KKval1, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (len + 1);

	//うち税率6.3％適用分
	//	pK = &(*m_pTaxListData)->NRec.KSval[12];
	/*pK = &(*m_pTaxListData)->NRec.KSval[KBUYLINE10];
	::ZeroMemory(buf, sizeof(buf));

	if (DispJudge(1, KBUYLINE10) == 1) {
		m_pArith->l_print(buf, pK->KKval2, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (len + 1);*/

	//うち税率6.24％適用分
	pK = &(*m_pTaxListData)->NRec.KSval[KBUYLINE10];
	::ZeroMemory(buf, sizeof(buf));

	//if (DispJudge(2, KBUYLINE10) == 1) {
	if (DispJudge(1, KBUYLINE10) == 1) {
		m_pArith->l_print(buf, pK->KKval3, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (len + 1);

	//うち税率7.8％適用分
	pK = &(*m_pTaxListData)->NRec.KSval[KBUYLINE10];
	::ZeroMemory(buf, sizeof(buf));

	//if (DispJudge(3, KBUYLINE10) == 1) {
	if (DispJudge(2, KBUYLINE10) == 1) {
		m_pArith->l_print(buf, pK->KKval4, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (len + 1);

	// 課税標準額の計算
	// B欄の金額
	/*st = 0;

	pK = &(*m_pTaxListData)->NRec.KSval[KBUYLINE11];

	::ZeroMemory(buf, sizeof(buf));

	if (DispJudge(0, KBUYLINE11) == 1) { //列？
		m_pArith->l_print(buf, pK->KKval2, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (len + 1);


	::ZeroMemory(buf, sizeof(buf));

	if (DispJudge(0, KBUYLINE11) == 1) { //列？
		m_pArith->l_print(buf, pK->KKval1, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (len + 1);*/

	// C欄の金額
	st = 0;

	pK = &(*m_pTaxListData)->NRec.KSval[KBUYLINE11+1];	// calcの関係で+1する

	::ZeroMemory(buf, sizeof(buf));

	if (DispJudge(0, KBUYLINE11+1) == 1) { //列？
		m_pArith->l_print(buf, pK->KKval2, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (len + 1);

	::ZeroMemory(buf, sizeof(buf));
	
	if (DispJudge(0, KBUYLINE11+1) == 1) { //列？
		m_pArith->l_print(buf, pK->KKval1, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (len + 1);

	// D欄の金額
	st = 0;

	pK = &(*m_pTaxListData)->NRec.KSval[KBUYLINE12+1];

	::ZeroMemory(buf, sizeof(buf));

	if (DispJudge(0, KBUYLINE12+1) == 1) { //列？
		m_pArith->l_print(buf, pK->KKval2, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (len + 1);

	::ZeroMemory(buf, sizeof(buf));
	
	if (DispJudge(0, KBUYLINE12+1) == 1) { //列？
		m_pArith->l_print(buf, pK->KKval1, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (len + 1);

	//// 合計
	//pK = &(*m_pTaxListData)->NRec.KSval[KBUYLINE16];
	//::ZeroMemory(buf, sizeof(buf));

	//if (DispJudge(0, KBUYLINE16) == 1) { //列？
	//	m_pArith->l_print(buf, pK->KKval1, FMTX130);
	//}
	//len = (int)strlen(buf);
	//memmove(pBpMv, buf, len);
	//PF[i++].PITM[0] = pBpMv;
	//pBpMv += (len + 1);


	int fontsize = 0;
	char val[5][128];
	CString str;
	for (int l_cnt = KBUYLINE1; l_cnt <= KBUYLINE12+1; l_cnt++) {
		switch (l_cnt) {
		case KBUYLINE1:
		case KBUYLINE2:
		case KBUYLINE3:
		case KBUYLINE4:
		case KBUYLINE5:
		case KBUYLINE6:
		case KBUYLINE7:
		case KBUYLINE8:
		case KBUYLINE9:
		case KBUYLINE10:
		case KBUYLINE11+1:	// calcの関係で+1する
		case KBUYLINE12+1:	// calcの関係で+1する
			for (int idx = 0; idx < 5; idx++) {
				::ZeroMemory(val[idx], sizeof(val[idx]));
			}
			m_pArith->l_print(val[0], (*m_pTaxListData)->NRec.KSval[l_cnt].KKval1, FMTX130);
			m_pArith->l_print(val[1], (*m_pTaxListData)->NRec.KSval[l_cnt].KKval2, FMTX130);
			m_pArith->l_print(val[2], (*m_pTaxListData)->NRec.KSval[l_cnt].KKval3, FMTX130);
			m_pArith->l_print(val[3], (*m_pTaxListData)->NRec.KSval[l_cnt].KKval4, FMTX130);
			m_pArith->l_print(val[4], (*m_pTaxListData)->NRec.KSval[l_cnt].KKval5, FMTX130);
			for (int idx = 0; idx < 5; idx++) {
				str.Format(val[idx]);
				str.Trim();
				// 10億円 ( カンマ含む )以上ならフォントサイズを下げる
				//if (str.GetLength() > 13) {
				if ( str.GetLength() > 12 ) {
					fontsize = 1;
				}
			}
			break;
		default:
			break;
		}
		if (fontsize != 0) {
			break;
		}
	}

	if (fontsize == 1) {
		return _xprn(pno, 3, PF, 0, pDC, 0, 1);
	}

	return _xprn(pno, fno, PF, 0, pDC, 0, 1);
}
// 印刷３
int CShinBuy31Ex::PrintHyo3( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, len;
	char	buf[64], dbuf[64];

	i = 0;
	memset(pBp, '\0', Msiz);
	//	BP1 = BP;	
	char	*pBpMv = pBp;

	memset( PF, '\0', sizeof(PF) );

	// 項目名
	::ZeroMemory(buf, sizeof(buf));
	::ZeroMemory(dbuf, sizeof(dbuf));

	memmove(buf, (*m_pTaxListData)->Head.KHksnm, sizeof((*m_pTaxListData)->Head.KHksnm));
	len = (int)strlen(buf);
	if (len > 8) {
		memmove(pBpMv, buf, len);
		PF[i++].PITM[0] = pBpMv;
		pBpMv += (len + 1);

		memmove(pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHksnm));
		PF[i].PITM[0] = pBpMv;

		pBpMv += (sizeof((*m_pTaxListData)->Head.KHksnm) + 1);

		memmove(pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHksnm));
		PF[i++].PITM[0] = pBpMv;

		pBpMv += (sizeof((*m_pTaxListData)->Head.KHksnm) + 1);
	}
	else if (len > 6) {
		memmove(pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHksnm));
		PF[i++].PITM[0] = pBpMv;

		pBpMv += (sizeof((*m_pTaxListData)->Head.KHksnm) + 1);

		memmove(pBpMv, buf, len);
		PF[i++].PITM[0] = pBpMv;
		pBpMv += (len + 1);

		memmove(pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHksnm));
		PF[i].PITM[0] = pBpMv;

		pBpMv += (sizeof((*m_pTaxListData)->Head.KHksnm) + 1);
	}
	else {
		memmove(pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHksnm));
		PF[i++].PITM[0] = pBpMv;

		pBpMv += (sizeof((*m_pTaxListData)->Head.KHksnm) + 1);

		memmove(pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHksnm));
		PF[i++].PITM[0] = pBpMv;

		pBpMv += (sizeof((*m_pTaxListData)->Head.KHksnm) + 1);
		memmove(pBpMv, buf, len);
		PF[i].PITM[0] = pBpMv;
		pBpMv += (len + 1);
	}

	return(_xprn(pno, 4, PF, 0, pDC, 0));
}

// 22-0167,22-0361 add -->
void CShinBuy31Ex::CheckButtonIcsdiagctrl1(short index, LPUNKNOWN data)
{
	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;
	int				idx=0;
	int				mode=0;
	CString			msg=_T("");

	idata = (DIAGRAM_DATA *)data;
	if(idata->data_check)	{
		msg = _T("うち標準税率7.8%適用分について\n");
		msg+= _T("うち軽減税率6.24%適用分を差し引いて自動計算を行いますか？\n");
		msg+= _T("※入力済の金額は自動計算の金額に上書きされるため、元には戻せません。");
	}
	else	{
		msg = _T("うち標準税率7.8%適用分の自動計算を解除しますか？");
	}
	if(ICSMessageBox(msg,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)	{
		//short	pos = m_BuyDiag.GetPosition();
		m_BuyDiag.DeleteInput();
		diag_getdata(IDC_ICSDIAGCTRL1,index,&xdata,CIcsdiagctrl);
		if(!idata->data_check)	xdata.data_check = 1;
		else					xdata.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1,index,&xdata,CIcsdiagctrl);
		//m_BuyDiag.SetPosition(pos);
		SetItemPositionEx();
		return;
	}
	//(*m_pTaxListData)->NRec.KSval[3].KKauto &= 0x2f;	// フラグの上位を一旦クリア
//----->('23.10.02 tanaka cor)インボイス対応で追加されたD12,D13のサインも落ちていた為、修正
//	(*m_pSnHeadData)->Sn_GenericSgn &= 0x7ff;
//------
	(*m_pSnHeadData)->Sn_GenericSgn &= 0xfffff7ff;
	//<-----
	// チェックON
	if(idata->data_check)	{
		//// 自身以外のチェックをOFFにする
		//if(index == KBUYChk1ex)	{
		//	for(idx=KBUYChk2ex; idx<=KBUYChk3ex; idx++)	{
		//		diag_getdata(IDC_ICSDIAGCTRL1,idx,&xdata,CIcsdiagctrl);
		//		xdata.data_check = 0;
		//		diag_setdata(IDC_ICSDIAGCTRL1,idx,&xdata,CIcsdiagctrl);
		//	}
		//	(*m_pTaxListData)->NRec.KSval[3].KKauto |= 0x10;
		//}
		//else if(index == KBUYChk2ex)	{
		//	for(idx=KBUYChk1ex; idx<=KBUYChk3ex; idx+=2)	{
		//		diag_getdata(IDC_ICSDIAGCTRL1,idx,&xdata,CIcsdiagctrl);
		//		xdata.data_check = 0;
		//		diag_setdata(IDC_ICSDIAGCTRL1,idx,&xdata,CIcsdiagctrl);
		//	}
		//	(*m_pTaxListData)->NRec.KSval[3].KKauto |= 0x40;
		//}
		//else if(index == KBUYChk3ex)	{
		//	for(idx=KBUYChk1ex; idx<=KBUYChk2ex; idx++)	{
		//		diag_getdata(IDC_ICSDIAGCTRL1,idx,&xdata,CIcsdiagctrl);
		//		xdata.data_check = 0;
		//		diag_setdata(IDC_ICSDIAGCTRL1,idx,&xdata,CIcsdiagctrl);
		//	}
		//	(*m_pTaxListData)->NRec.KSval[3].KKauto |= 0x80;
		//}
		(*m_pSnHeadData)->Sn_GenericSgn |= 0x800;
	}
	else	{
		mode=1;
	}
	// 集計（自動計算チェックは全画面で共通のため、関係する画面の計算も行う）
	if(m_Util.IsUnConnectMst(m_pZmsub->zvol) && (m_Util.GetKazeihoushiki(m_pZmsub->zvol) == ID_ICSSH_KANNI))	{
		(*m_Scalc)->Shz_31AllCalc();		// 事業別売上高
		(*m_Scalc)->Shz_Sale31Post();		// 課税売上高
	}
	else	{
		if(m_pZmsub->zvol->ind_type&0x01)	{
			(*m_Scalc)->Shz_31AllCalc(1);	// 事業所得用
		}
		if(m_pZmsub->zvol->ind_type&0x04)	{
			(*m_Scalc)->Shz_31AllCalc(3);	// 農業所得用
		}
		if(m_pZmsub->zvol->ind_type&0x02)	{
			(*m_Scalc)->Shz_31AllCalc(2);	// 不動産所得用
		}
		(*m_Scalc)->Shz_Sale31Post();		// 課税売上高
		if(m_Util.GetKazeihoushiki(m_pZmsub->zvol) != ID_ICSSH_KANNI)	{
			(*m_Scalc)->Shz_Buy31Post();	// 課税仕入高
		}
	}
	// 再表示
	m_BuyDiag.OffControl(TRUE);
	DispBuyDiagData();
	// カーソルの終点を決める
	SetFocusRange();
	// カーソルセット
	SetItemPositionEx();
}

//-----------------------------------------------------------------------------
// カラム番号を取得
//-----------------------------------------------------------------------------
// 引数	index		：	インデックス
//-----------------------------------------------------------------------------
// 返送値	rt		：	カラム番号
//-----------------------------------------------------------------------------
int CShinBuy31Ex::GetColumNum( int index )
{
	int	rt=-1;

	switch(index)	{
		case KBUYVAL01Aex:
		case KBUYVAL02Aex:
		case KBUYVAL03Aex:
		case KBUYVAL04Aex:
		case KBUYVAL05Aex:
		//case KBUYVAL06Aex:
		case KBUYVAL07Aex:
		case KBUYVAL08Aex:
			rt = 0;
			break;
		case KBUYVAL01Bex:
		case KBUYVAL02Bex:
		case KBUYVAL03Bex:
		case KBUYVAL04Bex:
		case KBUYVAL05Bex:
		//case KBUYVAL06Bex:
		case KBUYVAL07Bex:
		case KBUYVAL08Bex:
			rt = 1;
			break;
		case KBUYVAL01Cex:
		case KBUYVAL02Cex:
		case KBUYVAL03Cex:
		case KBUYVAL04Cex:
		case KBUYVAL05Cex:
		//case KBUYVAL06Cex:
		case KBUYVAL07Cex:
		case KBUYVAL08Cex:
			 rt = 2;
			break;
		default:
			break;
	}
	return(rt);
}
// 22-0167,22-0361 add <--

// 修正No.168690 add -->
void CShinBuy31Ex::DiagReSetPosition(void)
{
	//this->SetFocus();
}
// 修正No.168690 add <--
