// ShinSpecific52_31_31.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific52_31_31.h"
#include "ShinSpecific52_31_31_exIdx.h"
#include "TaxationList31View.h"
#include "SpeAddDlg.h"

//== 定数 ==
const	int	BottomIndex = 98;
//const int ID502_10[] = { ID502_10A, ID502_10B, 0 };

// CShinSpecific52_31_31 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific52_31_31, CSpcBaseDlg)

CShinSpecific52_31_31::CShinSpecific52_31_31(CWnd* pParent, TSREC52* pRec /*=nullptr*/)
	: CSpcBaseDlg(IDD_DIALOG_K52_31_31, pParent)
	, m_pParent(pParent)
	, m_wPos(0)
	, m_cmPos(0)
	, m_ScW(0)
{
	m_curwnd = -1;
	// 24/02/15_特定収入 add -->
	m_pRec = pRec;
	// 24/02/15_特定収入 add <--
}

CShinSpecific52_31_31::~CShinSpecific52_31_31()
{
}

void CShinSpecific52_31_31::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific52_31_31);
}


BEGIN_MESSAGE_MAP(CShinSpecific52_31_31, CSpcBaseDlg)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CShinSpecific52_31_31 メッセージ ハンドラー
BOOL CShinSpecific52_31_31::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_END) {
			if (m_pParent) {
				m_pParent->PostMessageA(WM_KEYDOWN, VK_END);
			}
			return TRUE;
		}
		//F2キーを押したときの処理
		if (pMsg->wParam == VK_F2) {
			DIAGRAM_DATA dd;
			if (m_curwnd != -1) {
				//m_curwndにダイアグラムのハンドルを代入　resultにカーソル位置のオフセットを代入
				short result = ((CIcsdiagctrl*)GetDlgItem(m_curwnd))->GetPosition();
				//カーソル位置のオフセットのデータを取得
				diag_getdata(m_curwnd, result, &dd, CIcsdiagctrl);
				if (m_curwnd == IDC_ICSDIAG8CTRL1) {
					TerminationIcsdiag8ctrl1(result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd);
				}
				return TRUE;
			}
		}
		else if (pMsg->wParam == VK_ESCAPE) {
			((CSpeAddDlg*)m_pParent)->OnBnClickedCancel();
			return TRUE;
		}
	}

	return CSpcBaseDlg::PreTranslateMessage(pMsg);
}


BOOL CShinSpecific52_31_31::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	if (!(m_pRec->ts5231.sel & 0x01)) {
		fcs = FCS_Specific52_31_31;
		fcslr = FCS_Specific52_31_31_LR;
		fcs_cnt = FCS_SPECIFIC1CNT;
	}
	else {
		fcs = FCS_Specific52_31_31_EX;
		fcslr = FCS_Specific52_31_31_LR_EX;
		fcs_cnt = FCS_SPECIFIC1CNT_EX;
	}
	SpecificInit52_31_31();
	m_initfg = 1;

	CSpcBaseDlg::OnInitDialogEX();
	// TODO: ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinSpecific52_31_31::SpecificInit52_31_31()
{
	// 倍長演算指定
	m_pArith->l_defn(0x16);

	InitDiagAttr();
	
	//入力の可、不可を設定
	IndexControl();

	// 金額表示
	SpecificDisp52_31_31();

	//カーソルの位置をセット
	SetItemPosition();
}

void CShinSpecific52_31_31::SetItemPosition()
{
	int index;

	//前回のポジション復元
	if (m_curwnd == IDC_ICSDIAG8CTRL1) {

		if ((index = m_Specific52_31_31.GetPosition()) != -1) {

			switch (index) {
			case ID502_1A:
				index = ID502_1A;
				break;
			case ID502_3A:
				index = ID502_3A;
				break;
			case ID502_9_bunshi:
				index = ID502_9_bunshi;
				break;
			case ID502_9_bunbo:
				index = ID502_9_bunbo;
				break;
			case ID502_12_bunshi:
				index = ID502_12_bunshi;
				break;
			case ID502_12_bunbo:
				index = ID502_12_bunbo;
				break;
			case ID502_5B:
				index = ID502_5B;
				break;
			case ID502_7B:
				index = ID502_7B;
				break;
			default:
				break;
			}
			m_Specific52_31_31.SetPosition(index);
			return;
		}
	}

	this->SetFocus();
	//前回のポジションがないとき
	//非連動のときは43番（4E）にセット

	m_Specific52_31_31.SetPosition(ID502_1A);

	SetRedrawScroll(0);
}

//==============================
// 金額表示
//==============================
void CShinSpecific52_31_31::SpecificDisp52_31_31()
{
	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;

	//（税率６．２４％）控除対象外仕入れに係る支払対価の額の合計額
	memmove(dd.data_val, m_pRec->ts5231.val011, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_1A, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率６．２４％）１×６．２４／１０８
	memmove(dd.data_val, m_pRec->ts5231.val021, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_2A, (struct IUnknown *)&dd, CIcsdiagctrl);
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {
		ChangeColor(IDC_ICSDIAG8CTRL1, ID502_2A, (m_pRec->ts5231.msgn[0] & 0x01) ? 3 : 0);//手入力の場合背景色を青色に
	}
	//（税率６．２４％）控除対象外仕入れに係る支払対価の額
	memmove(dd.data_val, m_pRec->ts5231.val031, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_3A, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率６．２４％）３×６．２４／１０８
	memmove(dd.data_val, m_pRec->ts5231.val041, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_4A, (struct IUnknown *)&dd, CIcsdiagctrl);
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {
		ChangeColor(IDC_ICSDIAG8CTRL1, ID502_4A, (m_pRec->ts5231.msgn[0] & 0x02) ? 3 : 0);//手入力の場合背景色を青色に
	}
	//（税率７．８％）控除対象外仕入れに係る支払対価の額の合計額
	memmove(dd.data_val, m_pRec->ts5231.val052, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_5B, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率７．８％）５×７．８／１１０
	memmove(dd.data_val, m_pRec->ts5231.val062, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_6B, (struct IUnknown *)&dd, CIcsdiagctrl);
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {
		ChangeColor(IDC_ICSDIAG8CTRL1, ID502_6B, (m_pRec->ts5231.msgn[0] & 0x04) ? 3 : 0);//手入力の場合背景色を青色に
	}
	//（税率７．８％）控除対象外仕入れに係る支払対価の額
	memmove(dd.data_val, m_pRec->ts5231.val072, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_7B, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率７．８％）７×７．８／１１０
	memmove(dd.data_val, m_pRec->ts5231.val082, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_8B, (struct IUnknown *)&dd, CIcsdiagctrl);
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {
		ChangeColor(IDC_ICSDIAG8CTRL1, ID502_8B, (m_pRec->ts5231.msgn[0] & 0x08) ? 3 : 0);//手入力の場合背景色を青色に
	}

	//取戻し対象特定収入のあった課税期間の課税売上割合
	if (!(m_pRec->ts5231.sel & 0x01)) {
		memmove(dd.data_val, m_pRec->ts5231.val091, sizeof(dd.data_val));
		diag_setdata(IDC_ICSDIAG8CTRL1, ID502_9_bunshi, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(dd.data_val, m_pRec->ts5231.val092, sizeof(dd.data_val));
		diag_setdata(IDC_ICSDIAG8CTRL1, ID502_9_bunbo, (struct IUnknown *)&dd, CIcsdiagctrl);
	}
	else {
		char bf[128] = { 0 };
		m_pArith->l_print(bf, m_pRec->ts5231.val090, _T("SS9.99"));
		dd.data_edit = bf;
		diag_setdata(IDC_ICSDIAG8CTRL1, ID502_9_EX, (struct IUnknown *)&dd, CIcsdiagctrl);
	}
	//（税率６．２４％）４×９
	memmove(dd.data_val, m_pRec->ts5231.val101, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_10A, (struct IUnknown *)&dd, CIcsdiagctrl);
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {
		if (!(m_pRec->ts5231.sel & 0x01)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID502_10A, 1);//手入力の場合背景色を青色に
		}
		else
			ChangeColor(IDC_ICSDIAG8CTRL1, ID502_10A, (m_pRec->ts5231.msgn[0] & 0x10) ? 3 : 0);//手入力の場合背景色を青色に
	}
																						   //（税率７．８％）８×９
	memmove(dd.data_val, m_pRec->ts5231.val102, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_10B, (struct IUnknown *)&dd, CIcsdiagctrl);
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {
		if (!(m_pRec->ts5231.sel & 0x01)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID502_10B, 1);//手入力の場合背景色を青色に
		}
		else
			ChangeColor(IDC_ICSDIAG8CTRL1, ID502_10B, (m_pRec->ts5231.msgn[0] & 0x20) ? 3 : 0);//手入力の場合背景色を青色に
	}
																						   //（税率６．２４％）２＋１０
	memmove(dd.data_val, m_pRec->ts5231.val111, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_11A, (struct IUnknown *)&dd, CIcsdiagctrl);
	//（税率７．８％）６＋１０
	memmove(dd.data_val, m_pRec->ts5231.val112, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_11B, (struct IUnknown *)&dd, CIcsdiagctrl);

	//取戻し対象特定収入のあった課税期間の調整割合
	memmove(dd.data_val, m_pRec->ts5231.val121, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_12_bunshi, (struct IUnknown *)&dd, CIcsdiagctrl);
	memmove(dd.data_val, m_pRec->ts5231.val122, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_12_bunbo, (struct IUnknown *)&dd, CIcsdiagctrl);

	//１－１２
	memmove(dd.data_val, m_pRec->ts5231.val131, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_13_bunshi, (struct IUnknown *)&dd, CIcsdiagctrl);
	memmove(dd.data_val, m_pRec->ts5231.val132, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_13_bunbo, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率６．２４％）控除対象外仕入れに係る調整額
	memmove(dd.data_val, m_pRec->ts5231.val141, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_14A, (struct IUnknown *)&dd, CIcsdiagctrl);
	//（税率７．８％）控除対象外仕入れに係る調整額
	memmove(dd.data_val, m_pRec->ts5231.val142, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_14B, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率６．２４％）控除対象外仕入れに係る調整額の合計額
	memmove(dd.data_val, m_pRec->ts5231.val151, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_15A, (struct IUnknown *)&dd, CIcsdiagctrl);
	//（税率７．８％）控除対象外仕入れに係る調整額の合計額
	memmove(dd.data_val, m_pRec->ts5231.val152, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_15B, (struct IUnknown *)&dd, CIcsdiagctrl);

}

//入力可能な範囲を調べる
void CShinSpecific52_31_31::SetFocusRange()
{
	DIAGRAM_ATTRIBUTE DA;

	m_FcsUpEnd = -1;
	m_FcsDownEnd = -1;
	m_FcsLeftEnd = -1;
	m_FcsRightEnd = -1;

	//上から確かめていって入力可能なものがあったらm_FcsUpEndに代入し、break
	for (int idx = 0; idx < fcs_cnt; idx++) {
		m_Specific52_31_31.GetAttribute(fcs[idx].IDNo, (LPUNKNOWN)&DA);
		//入力ボックスの色がクリーム色なら入力可である。
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsUpEnd = fcs[idx].IDNo;
			break;
		}
	}

	//左から確かめていって入力可能なものがあったらm_FcsLeftEndに代入し、break
	for (int idx = 0; idx < fcs_cnt; idx++) {
		m_Specific52_31_31.GetAttribute(fcslr[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsLeftEnd = fcslr[idx].IDNo;
			break;
		}
	}

	//右から確かめていって入力可能なものがあったらm_FcsRightEndに代入し、break（構造体を逆からたどる）
	for (int idx = fcs_cnt - 1; idx > -1; idx--) {
		m_Specific52_31_31.GetAttribute(fcslr[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsRightEnd = fcslr[idx].IDNo;
			break;
		}
	}
	//下から確かめていって入力可能なものがあったらm_FcsDownEndに代入し、break（構造体を逆からたどる）
	for (int idx = fcs_cnt - 1; idx > -1; idx--) {
		m_Specific52_31_31.GetAttribute(fcs[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsDownEnd = fcs[idx].IDNo;
			break;
		}
	}
}

//入力の可、不可を設定
void CShinSpecific52_31_31::IndexControl(void)
{
	int Cnt = 0;

// 240308_h_168688 add -->
	if (!(m_pRec->ts5231.sel & 0x01)) {
		fcs = FCS_Specific52_31_31;
		fcslr = FCS_Specific52_31_31_LR;
		fcs_cnt = FCS_SPECIFIC1CNT;
	}
	else {
		fcs = FCS_Specific52_31_31_EX;
		fcslr = FCS_Specific52_31_31_LR_EX;
		fcs_cnt = FCS_SPECIFIC1CNT_EX;
	}
// 240308_h_168688 add <--

	while (1) {

		if (Specific52_31_31[Cnt].Index == 0) {
			break;
		}

		int Flg;
		//入力ロック中のとき
		if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
			Flg = Specific52_31_31[Cnt].AllUnConnect;
		}
		//入力ロック中でない時
		else {
			//入力ロック中でない　かつ　非連動がオン
			if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
				Flg = Specific52_31_31[Cnt].UnConnect;
			}
			//入力ロック中でない　かつ　非連動がオフ（連動）
			else {
				Flg = Specific52_31_31[Cnt].Connect;
			}
		}

		//入力の可不可をセット
		ATRIB_MOD(IDC_ICSDIAG8CTRL1, Specific52_31_31[Cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Specific52_31_31[Cnt].Connect);
		Cnt++;
	}

	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;

	if (!(m_pRec->ts5231.sel & 0x01)) {
		m_Specific52_31_31.ModifyItem(ID502_9_EX, DIAG_MDFY_TRANSPARENT);
		m_Specific52_31_31.ModifyItem(106, DIAG_MDFY_TRANSPARENT);

		m_Specific52_31_31.ModifyItem(ID502_9_bunshi, DIAG_MDFY_OPAQUE);
		m_Specific52_31_31.ModifyItem(ID502_9_bunbo, DIAG_MDFY_OPAQUE);

		if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
			m_Specific52_31_31.ModifyItem(ID502_9_bunshi, DIAG_MDFY_READONLY);
			m_Specific52_31_31.ModifyItem(ID502_9_bunbo, DIAG_MDFY_READONLY);
		}

		dd.data_check = 0;
		diag_setdata(IDC_ICSDIAG8CTRL1, ID502_check, (struct IUnknown *)&dd, CIcsdiagctrl);
	}
	else {
		m_Specific52_31_31.ModifyItem(ID502_9_bunshi, DIAG_MDFY_TRANSPARENT);
		m_Specific52_31_31.ModifyItem(ID502_9_bunbo, DIAG_MDFY_TRANSPARENT);

		m_Specific52_31_31.ModifyItem(ID502_9_EX, DIAG_MDFY_OPAQUE);
		m_Specific52_31_31.ModifyItem(106, DIAG_MDFY_OPAQUE);

		if ((*m_pSnHeadData)->Sn_Sign4 & 0x01)
			m_Specific52_31_31.ModifyItem(ID502_9_EX, DIAG_MDFY_READONLY);

		dd.data_check = 1;
		diag_setdata(IDC_ICSDIAG8CTRL1, ID502_check, (struct IUnknown *)&dd, CIcsdiagctrl);
	}

	//入力ロック中のとき水色のメッセージは非表示
	if ((*m_pSnHeadData)->Sn_Sign4 & 0x01)
		diag_modify(IDC_ICSDIAG8CTRL1, 107, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl);

	SetFocusRange();

	//DELETEキー１回で削除モード設定
	//この設定をしないと、Deleteを２回押さないとTerminationに入ってこなくなる。
	m_Specific52_31_31.EnableDelete(0);

}


//======================================================
// ダイアグラム初期設定
//======================================================
void CShinSpecific52_31_31::InitDiagAttr()
{
	int	cnt = 0;
// 24/02/20_higuchi_test del -->
	//int	Flg;
// 24/02/20_higuchi_test del <--

	while (1) {
		if (Specific52_31_31[cnt].Index == 0) {
			break;
		}
		if (Specific52_31_31[cnt].Index == ID502_10A || Specific52_31_31[cnt].Index == ID502_10B) {
			if (!(m_pRec->ts5231.sel & 0x01)) {
				Specific52_31_31[cnt].Connect = 0;
				Specific52_31_31[cnt].UnConnect = 0;
			}
			else {
				Specific52_31_31[cnt].Connect = 1;
				Specific52_31_31[cnt].UnConnect = 1;
			}
		}
		cnt++;
	}
	SetFocusRange();
	m_Specific52_31_31.EnableDelete(0);
}

//=========================================================
// 入力項目色変更
//---------------------------------------------------------
// 引数
//			id			:		ダイアグラムID
//			Index		:		ダイアグラム内インデックス
//			sign		:		変更サイン
//=========================================================
void CShinSpecific52_31_31::ChangeColor(unsigned short id, short Index, int sign)
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

HBRUSH CShinSpecific52_31_31::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ここで DC の属性を変更してください。
	if (nCtlColor == CTLCOLOR_DLG) {
		hbr = Ibrsh_ShinSpecific;
	}
	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


void CShinSpecific52_31_31::OnSize(UINT nType, int cx, int cy)
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
		m_Specific52_31_31.GetRect(BottomIndex, (LPUNKNOWN)&b);
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


void CShinSpecific52_31_31::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
BEGIN_EVENTSINK_MAP(CShinSpecific52_31_31, CSpcBaseDlg)
	ON_EVENT(CShinSpecific52_31_31, IDC_ICSDIAG8CTRL1, 2, CShinSpecific52_31_31::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific52_31_31, IDC_ICSDIAG8CTRL1, 1, CShinSpecific52_31_31::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific52_31_31, IDC_ICSDIAG8CTRL1, 3, CShinSpecific52_31_31::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinSpecific52_31_31, IDC_ICSDIAG8CTRL1, 9, CShinSpecific52_31_31::CheckButtonIcsdiag8ctrl1, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSpecific52_31_31::Specific52_31_31_Calq()
{
	DIAGRAM_DATA	dd;
	dd.data_imgdata = NULL;

	char bf20[6], bf50[6], bf100[6], per[6];
	m_pArith->l_input(bf20, "20");
	m_pArith->l_input(bf50, "50");
	m_pArith->l_input(bf100, "100");
	m_pArith->l_input(per, "10000");
	//（税率６．２４％）１×６．２４／１０８
	if (!(m_pRec->ts5231.msgn[0] & 0x01)) {
		m_Util.percent(m_pRec->ts5231.val021, m_pRec->ts5231.val011, 11, 62, 0);
	}
	//（税率６．２４％）３×６．２４／１０８
	if (!(m_pRec->ts5231.msgn[0] & 0x02)) {
		m_Util.percent(m_pRec->ts5231.val041, m_pRec->ts5231.val031, 11, 62, 0);
	}

	//（税率７．８％）５×７．８／１１０
	if (!(m_pRec->ts5231.msgn[0] & 0x04)) {
		m_Util.percent(m_pRec->ts5231.val062, m_pRec->ts5231.val052, 12, 78, 0);
	}
	//（税率７．８％）７×７．８／１１０
	if (!(m_pRec->ts5231.msgn[0] & 0x08)) {
		m_Util.percent(m_pRec->ts5231.val082, m_pRec->ts5231.val072, 12, 78, 0);
	}

	//「変更する」にチェックが無い時
	if (!(m_pRec->ts5231.sel & 0x01)) {
		if (m_pArith->l_test(m_pRec->ts5231.val091) && m_pArith->l_test(m_pRec->ts5231.val092)) {
			//（税率６．２４％）４×９
			m_Util.l_pardotnet_syz(m_pRec->ts5231.val101, m_pRec->ts5231.val041, m_pRec->ts5231.val091, m_pRec->ts5231.val092);
			//m_pArith->l_mul(m_pRec->ts5231.val101, m_pRec->ts5231.val041, m_pRec->ts5231.val091);
			//m_pArith->l_div(m_pRec->ts5231.val101, m_pRec->ts5231.val101, m_pRec->ts5231.val092);

			//（税率７．８％）８×９
			m_Util.l_pardotnet_syz(m_pRec->ts5231.val102, m_pRec->ts5231.val082, m_pRec->ts5231.val091, m_pRec->ts5231.val092);
			//m_pArith->l_mul(m_pRec->ts5231.val102, m_pRec->ts5231.val082, m_pRec->ts5231.val091);
			//m_pArith->l_div(m_pRec->ts5231.val102, m_pRec->ts5231.val102, m_pRec->ts5231.val092);
		}
		else {
			memset(m_pRec->ts5231.val101, '\0', MONY_BUF_SIZE);
			memset(m_pRec->ts5231.val102, '\0', MONY_BUF_SIZE);
		}
	}
	//「変更する」にチェックがある時
	else {
		//（税率６．２４％）４×９
		if (!(m_pRec->ts5231.msgn[0] & 0x10)) {
			m_Util.l_pardotnet_syz(m_pRec->ts5231.val101, m_pRec->ts5231.val041, m_pRec->ts5231.val090, per);
			//m_pArith->l_mul(m_pRec->ts5231.val101, m_pRec->ts5231.val041, m_pRec->ts5231.val090);
			//m_pArith->l_div(m_pRec->ts5231.val101, m_pRec->ts5231.val101, per);
		}
		//（税率７．８％）８×９
		if (!(m_pRec->ts5231.msgn[0] & 0x20)) {
			m_Util.l_pardotnet_syz(m_pRec->ts5231.val102, m_pRec->ts5231.val082, m_pRec->ts5231.val090, per);
			//m_pArith->l_mul(m_pRec->ts5231.val102, m_pRec->ts5231.val082, m_pRec->ts5231.val090);
			//m_pArith->l_div(m_pRec->ts5231.val102, m_pRec->ts5231.val102, per);
		}
	}

	//（税率６．２４％）２＋１０
	m_pArith->l_add(m_pRec->ts5231.val111, m_pRec->ts5231.val021, m_pRec->ts5231.val101);

	//（税率７．８％）６＋１０
	m_pArith->l_add(m_pRec->ts5231.val112, m_pRec->ts5231.val062, m_pRec->ts5231.val102);

	//１－［１２］
	if (m_pArith->l_test(m_pRec->ts5231.val121) && m_pArith->l_test(m_pRec->ts5231.val122)) {
		m_pArith->l_sub(m_pRec->ts5231.val131, m_pRec->ts5231.val122, m_pRec->ts5231.val121);
		memmove(m_pRec->ts5231.val132, m_pRec->ts5231.val122, MONY_BUF_SIZE);
		// 1/-3,-1/-3など分母にマイナスがついている場合は符号を反転する (24,03,19 kajima)
		if (m_pArith->l_test(m_pRec->ts5231.val132) < 0) {
			m_pArith->l_neg(m_pRec->ts5231.val131);
			m_pArith->l_neg(m_pRec->ts5231.val132);
		}
	}
	else if (!m_pArith->l_test(m_pRec->ts5231.val121)) {
		memmove(m_pRec->ts5231.val131, m_pRec->ts5231.val122, MONY_BUF_SIZE);
		memmove(m_pRec->ts5231.val132, m_pRec->ts5231.val122, MONY_BUF_SIZE);
	}
	else if (!m_pArith->l_test(m_pRec->ts5231.val122)) {
		memset(m_pRec->ts5231.val131, '\0', MONY_BUF_SIZE);
		memset(m_pRec->ts5231.val132, '\0', MONY_BUF_SIZE);
	}
	//（税率６．２４％）控除対象外仕入れに係る調整額
	m_Util.l_pardotnet_syz(m_pRec->ts5231.val141, m_pRec->ts5231.val111, m_pRec->ts5231.val131, m_pRec->ts5231.val132);
	//m_pArith->l_mul(m_pRec->ts5231.val141, m_pRec->ts5231.val111, m_pRec->ts5231.val131);
	//m_pArith->l_div(m_pRec->ts5231.val141, m_pRec->ts5231.val141, m_pRec->ts5231.val132);

	//（税率７．８％）控除対象外仕入れに係る調整額
	m_Util.l_pardotnet_syz(m_pRec->ts5231.val142, m_pRec->ts5231.val112, m_pRec->ts5231.val131, m_pRec->ts5231.val132);
	//m_pArith->l_mul(m_pRec->ts5231.val142, m_pRec->ts5231.val112, m_pRec->ts5231.val131);
	//m_pArith->l_div(m_pRec->ts5231.val142, m_pRec->ts5231.val142, m_pRec->ts5231.val132);

	//（税率６．２４％）控除対象外仕入れに係る調整額の合計額
	m_pArith->l_add(m_pRec->ts5231.val151, m_pRec->ts5231.val141, m_pRec->ts5232.val151);
	m_pArith->l_add(m_pRec->ts5231.val151, m_pRec->ts5231.val151, m_pRec->ts5233.val151);
	
	//（税率７．８％）控除対象外仕入れに係る調整額の合計額
	m_pArith->l_add(m_pRec->ts5231.val152, m_pRec->ts5231.val142, m_pRec->ts5232.val152);
	m_pArith->l_add(m_pRec->ts5231.val152, m_pRec->ts5231.val152, m_pRec->ts5233.val152);

	SpecificDisp52_31_31();
}

void CShinSpecific52_31_31::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	DIAGRAM_DATA	dd;
	dd.data_imgdata = NULL;
	int length, tmpUWexp;
	char buf[525];
	// 編集した金額を構造体 TSREC52 に保存
	switch (index) {
	case	ID502_1A:			//（税率６．２４％）控除対象外仕入れに係る支払対価の額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val011, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_2A:	//（税率６．２４％）１×６．２４／１０８
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		if (m_pArith->l_cmp(m_pRec->ts5231.val021, dd.data_val)) {
			memmove(m_pRec->ts5231.val021, dd.data_val, MONY_BUF_SIZE);
			m_pRec->ts5231.msgn[0] |= 0x01;
		}
		break;
	case	ID502_3A:	//（税率６．２４％）控除対象外仕入れに係る支払対価の額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val031, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_4A:		// （税率６．２４％）３×６．２４／１０８
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		if (m_pArith->l_cmp(m_pRec->ts5231.val041, dd.data_val)) {
			memmove(m_pRec->ts5231.val041, dd.data_val, MONY_BUF_SIZE);
			m_pRec->ts5231.msgn[0] |= 0x02;
		}
		break;
	case	ID502_5B:		//（税率７．８％）控除対象外仕入れに係る支払対価の額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val052, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_6B:		//（税率７．８％）５×７．８／１１０
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		if (m_pArith->l_cmp(m_pRec->ts5231.val062, dd.data_val)) {
			memmove(m_pRec->ts5231.val062, dd.data_val, MONY_BUF_SIZE);
			m_pRec->ts5231.msgn[0] |= 0x04;
		}
		break;
	case	ID502_7B:		//（税率７．８％）控除対象外仕入れに係る支払対価の額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val072, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_8B:			 //（税率７．８％）７×７．８／１１００
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		if (m_pArith->l_cmp(m_pRec->ts5231.val082, dd.data_val)) {
			memmove(m_pRec->ts5231.val082, dd.data_val, MONY_BUF_SIZE);
			m_pRec->ts5231.msgn[0] |= 0x08;
		}
		break;
	case	ID502_9_bunshi:			// 取戻し対象特定収入のあった課税期間の課税売上割合　分子
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val091, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_9_bunbo:			//取戻し対象特定収入のあった課税期間の課税売上割合　分母
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val092, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_10A:			//（税率６．２４％）４×９
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		if (m_pArith->l_cmp(m_pRec->ts5231.val101, dd.data_val)) {
			memmove(m_pRec->ts5231.val101, dd.data_val, MONY_BUF_SIZE);
			m_pRec->ts5231.msgn[0] |= 0x10;
		}
		break;
	case	ID502_10B:			//（税率７．８％）８×９
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		if (m_pArith->l_cmp(m_pRec->ts5231.val102, dd.data_val)) {
			memmove(m_pRec->ts5231.val102, dd.data_val, MONY_BUF_SIZE);
			m_pRec->ts5231.msgn[0] |= 0x20;
		}
		break;
	case	ID502_11A:			//（税率６．２４％）２＋１０
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val111, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_11B:			//（税率７．８％）６＋１０
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val112, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_12_bunshi:			//取戻し対象特定収入のあった課税期間の調整割合　分子
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val121, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_12_bunbo:			// 取戻し対象特定収入のあった課税期間の調整割合　分母
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val122, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_13_bunshi:			// １－１２　分子
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val131, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_13_bunbo:			//１－１２　分母
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val132, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_14A:			//（税率６．２４％）控除対象外仕入れに係る調整額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val141, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_14B:			//（税率７．８％）控除対象外仕入れに係る調整額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val142, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_15A:			//（税率６．２４％）控除対象外仕入れに係る調整額の合計額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val151, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_15B:			//（税率７．８％）控除対象外仕入れに係る調整額の合計額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts5231.val152, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_9_EX:
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);	
		length = dd.data_edit.GetLength();
		memset(buf, '\0', sizeof(buf));
		tmpUWexp = 0;
		if (length) {
			memmove(buf, dd.data_edit, length);
		}
		m_Util.perinp(3, 2, buf, (short*)&tmpUWexp);
		if (tmpUWexp <= 10000) {
			memmove(m_pRec->ts5231.val090, dd.data_val, MONY_BUF_SIZE);
		}
		else {
			m_Specific52_31_31.DataClear(ID502_9_EX, TRUE);
		}
		break;
	case	ID502_check:
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		if (dd.data_check)	m_pRec->ts5231.sel |= 0x01;
		else				m_pRec->ts5231.sel &= ~0x01;
		break;
	}

	// 再計算
	Specific52_31_31_Calq();
	// 24/02/15_特定収入 add <--
}


void CShinSpecific52_31_31::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL1;
}


void CShinSpecific52_31_31::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
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

	int pos = 0;

	//上へ：Move = 1
	if (wTerm == VK_UP) {
		pos = FocusMoveCheck(index, 1);
		m_Specific52_31_31.SetPosition(pos);
	}
	//下へ：0
	if (wTerm == VK_DOWN) {
		pos = FocusMoveCheck(index, 0);
		m_Specific52_31_31.SetPosition(pos);
	}
	//右へ：2
	if ((wTerm == VK_RETURN) || (wTerm == VK_F3) || (wTerm == VK_RIGHT)) {
		pos = FocusMoveCheck(index, 2);
		m_Specific52_31_31.SetPosition(pos);
	}
	//左へ：3
	else if ((wTerm == VK_TAB) || (wTerm == VK_LEFT) || (wTerm == VK_F2)) {
		pos = FocusMoveCheck(index, 3);
		m_Specific52_31_31.SetPosition(pos);
	}
	if (wTerm == VK_DELETE) {
		switch (index) {
		case ID502_2A:
			m_pRec->ts5231.msgn[0] &= ~0x01;
			break;
		case ID502_4A:
			m_pRec->ts5231.msgn[0] &= ~0x02;
			break;
		case ID502_6B:
			m_pRec->ts5231.msgn[0] &= ~0x04;
			break;
		case ID502_8B:
			m_pRec->ts5231.msgn[0] &= ~0x08;
			break;
		case ID502_10A:
			m_pRec->ts5231.msgn[0] &= ~0x10;
			break;
		case ID502_10B:
			m_pRec->ts5231.msgn[0] &= ~0x20;
			break;
// 240228_No5.higuchi add -->
		case ID502_check:	// 変更チェックはDeleteキー押下で何もしない
			break;
// 240228_No5.higuchi add <--
		default:
			m_Specific52_31_31.DataClear(index, TRUE);
			break;
		}
		m_Specific52_31_31.SetPosition(index);
	}

	SetRedrawScrollToIdx(m_Specific52_31_31.GetPosition());
}

//次のフォーカスの場所検索（上下左右動作）
//戻り値：次のフォーカスのインデックス
int CShinSpecific52_31_31::FocusMoveCheck(int Index, int Move)
{
	int idx = 0; //フォーカスのインデックス
	int i, j = 0; //forループ用
	BOOL end_flg = FALSE; //端に到達しているかどうか

	//端に到達しているか判定
	if (Move == 0) { //下方向への移動のとき
		if (Index == m_FcsDownEnd) {
			idx = m_FcsUpEnd;
			end_flg = TRUE;
		}
	}
	else if (Move == 1) { //上方向への移動のとき
		if (Index == m_FcsUpEnd) {
			idx = m_FcsDownEnd;
			end_flg = TRUE;
		}
	}
	else if (Move == 2) { //右方向への移動のとき
		if (Index == m_FcsRightEnd) {
			idx = m_FcsLeftEnd;
			end_flg = TRUE;
		}
	}
	else if (Move == 3) { //左方向への移動のとき
		if (Index == m_FcsLeftEnd) {
			idx = m_FcsRightEnd;
			end_flg = TRUE;
		}
	}
	//もしend_flgがTRUE（端に到達している）なら、逆の端の位置へ飛ぶ
	//例：右端にいるときに→を押されたら、左端へ飛ぶ
	if (end_flg == TRUE) {
		return idx;
	}

	//現在のポジションを判定
	//縦移動の場合
	if (Move == 0 || Move == 1) {
		for (i = 0; i < fcs_cnt; i++) {
			if (Index == fcs[i].IDNo) {
				break;
			}
		}
		idx = fcs[i].IDNo; //現在のポジションをidxに代入
	}
	//横移動の場合
	else if (Move == 2 || Move == 3) {
		for (i = 0; i < fcs_cnt; i++) {
			if (Index == fcs[i].IDNo) {
				break;
			}
		}
		idx = fcslr[i].IDNo; //現在のポジションをidxに代入
	}

	//移動後の位置を判定
	DIAGRAM_ATTRIBUTE DA;
	//下方向
	if (Move == 0) {
		for (j = i + 1; j < fcs_cnt; j++) {
			m_Specific52_31_31.GetAttribute(fcs[j].IDNo, (LPUNKNOWN)&DA);
			//入力可のボックスかどうかを色で判定、入力可ならそこが移動先
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = fcs[j].IDNo;
				break;
			}
		}
	}

	//上方向
	if (Move == 1) {
		for (j = i - 1; j > -1; j--) {
			m_Specific52_31_31.GetAttribute(fcs[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = fcs[j].IDNo;
				break;
			}
		}
	}

	//右方向
	if (Move == 2) {
		for (j = i + 1; j < fcs_cnt; j++) {
			m_Specific52_31_31.GetAttribute(fcslr[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = fcslr[j].IDNo;
				break;
			}
		}
	}

	//左方向
	if (Move == 3) {
		for (j = i - 1; j > -1; j--) {
			m_Specific52_31_31.GetAttribute(fcslr[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = fcslr[j].IDNo;
				break;
			}
		}
	}
	
	return idx;
}

void CShinSpecific52_31_31::CheckButtonIcsdiag8ctrl1(short index, LPUNKNOWN data)
{
	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;

	diag_getdata(IDC_ICSDIAG8CTRL1, ID502_check, (struct IUnknown *)&dd, CIcsdiagctrl);
	if (index == ID502_check) {
		if (dd.data_check) { 
			m_pRec->ts5231.sel |= 0x01;

			fcs = FCS_Specific52_31_31_EX;
			fcslr = FCS_Specific52_31_31_LR_EX;
			fcs_cnt = FCS_SPECIFIC1CNT_EX;
			//「変更する」にチェックを付けた際に分数の入力をクリア(24,03,21 kajima)
			memset(m_pRec->ts5231.val091, '\0', MONY_BUF_SIZE);
			memset(m_pRec->ts5231.val092, '\0', MONY_BUF_SIZE);
		}
		else { 
			m_pRec->ts5231.sel &= ~0x01; 
			m_pRec->ts5231.msgn[0] &= ~0x10;
			m_pRec->ts5231.msgn[0] &= ~0x20;

			fcs = FCS_Specific52_31_31;
			fcslr = FCS_Specific52_31_31_LR;
			fcs_cnt = FCS_SPECIFIC1CNT;
			//「変更する」のチェックを外した際に小数の入力をクリア(24,03,21 kajima)
			memset(m_pRec->ts5231.val090, '\0', MONY_BUF_SIZE);
		}
		//「変更する」にチェックを付けた際に分数→小数の変換をしていたがやめる(24,03,19 kajima)
		/*char t[128];
		char per[6];
		m_pArith->l_input(per, "10000");
		if (dd.data_check) {
			if (!m_pArith->l_test(m_pRec->ts5231.val090) && (m_pArith->l_test(m_pRec->ts5231.val091) && m_pArith->l_test(m_pRec->ts5231.val092))) {
				m_pArith->l_mul(m_pRec->ts5231.val090, m_pRec->ts5231.val091, per);
				m_pArith->l_div(m_pRec->ts5231.val090, m_pRec->ts5231.val090, m_pRec->ts5231.val092);

				m_pArith->l_print(t, m_pRec->ts5231.val090, "sssssssss");
			}
		}*/
		Specific52_31_31_Calq();
		SpecificInit52_31_31();
	}
}