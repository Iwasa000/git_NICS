// ShinSpecific52_2_31.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific52_2_31.h"
#include "ShinSpecific52_2_31_exIdx.h"
#include "TaxationList31View.h"
#include "SpeAddDlg.h"

//== 定数 ==
const	int	BottomIndex = 143;

// CShinSpecific52_2_31 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific52_2_31, CSpcBaseDlg)

CShinSpecific52_2_31::CShinSpecific52_2_31(CWnd* pParent, TSREC52* pRec /*=nullptr*/)
	: CSpcBaseDlg(IDD_DIALOG_K52_2_31, pParent)
	, m_pParent(pParent)
	, m_wPos(0)
	, m_cmPos(0)
	, m_ScW(0)
	, m_SySgn(FALSE)
{
	m_curwnd = -1;
	// 24/02/15_特定収入 add -->
	m_pRec = pRec;
	// 24/02/15_特定収入 add <--
}

CShinSpecific52_2_31::~CShinSpecific52_2_31()
{
}

void CShinSpecific52_2_31::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific52_2_31);
}


BEGIN_MESSAGE_MAP(CShinSpecific52_2_31, CSpcBaseDlg)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CShinSpecific52_2_31 メッセージ ハンドラー


BOOL CShinSpecific52_2_31::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	//キーを押したときの処理
	if (pMsg->message == WM_KEYDOWN) {
		//ENDキーを押したときの処理
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


BOOL CShinSpecific52_2_31::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	SpecificInit52_2_31();
	m_initfg = 1;

	CSpcBaseDlg::OnInitDialogEX();
	// TODO: ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinSpecific52_2_31::SpecificInit52_2_31()
{
	// 倍長演算指定
	m_pArith->l_defn(0x16);

	//入力の可、不可を設定
	IndexControl();

	// 金額表示
	SpecificDisp52_2_31();

	//カーソルの位置をセット
	SetItemPosition();
}

void CShinSpecific52_2_31::SetItemPosition()
{
	int index;

	//前回のポジション復元
	if (m_curwnd == IDC_ICSDIAG8CTRL1) {

		if ((index = m_Specific52_2_31.GetPosition()) != -1) {

			switch (index) {
			case ID502_1_bunshi:
				index = ID502_1_bunshi;
				break;
			case ID502_1_bunbo:
				index = ID502_1_bunbo;
				break;
			case ID502_3A:
				index = ID502_3A;
				break;
			case ID502_5B:
				index = ID502_5B;
				break;
			case ID502_8A:
				index = ID502_8A;
				break;
			case ID502_10B:
				index = ID502_10B;
				break;
			case ID502_14A:
				index = ID502_14A;
				break;
			case ID502_16B:
				index = ID502_16B;
				break;
			default:
				break;
			}
			m_Specific52_2_31.SetPosition(index);
			return;
		}
	}

	this->SetFocus();
	//前回のポジションがないとき
	//非連動のときは43番（4E）にセット

	m_Specific52_2_31.SetPosition(ID502_1_bunshi);

	SetRedrawScroll(0);
}

//==============================
// 金額表示
//==============================
void CShinSpecific52_2_31::SpecificDisp52_2_31()
{
	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;

	//取戻し対象特定収入のあった課税期間の調整割合
	memmove(dd.data_val, m_pRec->ts522.val011, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_1_bunshi, (struct IUnknown *)&dd, CIcsdiagctrl);
	memmove(dd.data_val, m_pRec->ts522.val012, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_1_bunbo, (struct IUnknown *)&dd, CIcsdiagctrl);

	//１－［１］
	memmove(dd.data_val, m_pRec->ts522.val021, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_2_bunshi, (struct IUnknown *)&dd, CIcsdiagctrl);
	memmove(dd.data_val, m_pRec->ts522.val022, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_2_bunbo, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率６．２４％）控除対象外仕入れに係る支払対価の額の合計額
	memmove(dd.data_val, m_pRec->ts522.val031, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_3A, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率６．２４％）３×６．２４／１０８
	memmove(dd.data_val, m_pRec->ts522.val041, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_4A, (struct IUnknown *)&dd, CIcsdiagctrl);
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {
		ChangeColor(IDC_ICSDIAG8CTRL1, ID502_4A, (m_pRec->ts522.msgn[0] & 0x01) ? 3 : 0);//手入力の場合背景色を青色に
	}
	//（税率７．８％）控除対象外仕入れに係る支払対価の額の合計額
	memmove(dd.data_val, m_pRec->ts522.val052, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_5B, (struct IUnknown *)&dd, CIcsdiagctrl);
	
	//（税率７．８％）５×７．８／１１０
	memmove(dd.data_val, m_pRec->ts522.val062, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_6B, (struct IUnknown *)&dd, CIcsdiagctrl);
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {
		ChangeColor(IDC_ICSDIAG8CTRL1, ID502_6B, (m_pRec->ts522.msgn[0] & 0x02) ? 3 : 0);//手入力の場合背景色を青色に
	}
	//（税率６．２４％）控除対象外仕入れに係る調整額
	memmove(dd.data_val, m_pRec->ts522.val071, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_7A, (struct IUnknown *)&dd, CIcsdiagctrl);
	//（税率７．８％）控除対象外仕入れに係る調整額
	memmove(dd.data_val, m_pRec->ts522.val072, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_7B, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率６．２４％）28年改正法附則第52条第1項(80%控除)の適用を受ける控除対象外仕入れに係る支払対価の額(税率6.24%)の合計額
	memmove(dd.data_val, m_pRec->ts522.val081, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_8A, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率６．２４％）８×６．２４／１０８
	memmove(dd.data_val, m_pRec->ts522.val091, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_9A, (struct IUnknown *)&dd, CIcsdiagctrl);
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {
		ChangeColor(IDC_ICSDIAG8CTRL1, ID502_9A, (m_pRec->ts522.msgn[0] & 0x04) ? 3 : 0);//手入力の場合背景色を青色に
	}
	//（税率７．８％）28年改正法附則第52条第1項(50%控除)の適用を受ける控除対象外仕入れに係る支払対価の額(税率7.8%)の合計額
	memmove(dd.data_val, m_pRec->ts522.val102, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_10B, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率７．８％）１０×７．８／１１０
	memmove(dd.data_val, m_pRec->ts522.val112, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_11B, (struct IUnknown *)&dd, CIcsdiagctrl);
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {
		ChangeColor(IDC_ICSDIAG8CTRL1, ID502_11B, (m_pRec->ts522.msgn[0] & 0x08) ? 3 : 0);//手入力の場合背景色を青色に
	}
	//（税率６．２４％）９×２
	memmove(dd.data_val, m_pRec->ts522.val121, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_12A, (struct IUnknown *)&dd, CIcsdiagctrl);
	//（税率７．８％）１１×２
	memmove(dd.data_val, m_pRec->ts522.val122, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_12B, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率６．２４％）控除対象外仕入れに係る調整額
	memmove(dd.data_val, m_pRec->ts522.val131, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_13A, (struct IUnknown *)&dd, CIcsdiagctrl);
	//（税率７．８％）控除対象外仕入れに係る調整額
	memmove(dd.data_val, m_pRec->ts522.val132, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_13B, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率６．２４％）28年改正法附則第53条第1項(50%控除)の適用を受ける控除対象外仕入れに係る支払対価の額(税率6.24%)の合計額
	memmove(dd.data_val, m_pRec->ts522.val141, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_14A, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率６．２４％）１４×６．２４／１０８
	memmove(dd.data_val, m_pRec->ts522.val151, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_15A, (struct IUnknown *)&dd, CIcsdiagctrl);
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {
		ChangeColor(IDC_ICSDIAG8CTRL1, ID502_15A, (m_pRec->ts522.msgn[0] & 0x10) ? 3 : 0);//手入力の場合背景色を青色に
	}
	//（税率７．８％）28年改正法附則第53条第1項(50%控除)の適用を受ける控除対象外仕入れに係る支払対価の額(税率7.8%)の合計額
	memmove(dd.data_val, m_pRec->ts522.val162, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_16B, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率７．８％）１６×７．８／１１０
	memmove(dd.data_val, m_pRec->ts522.val172, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_17B, (struct IUnknown *)&dd, CIcsdiagctrl);
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {
		ChangeColor(IDC_ICSDIAG8CTRL1, ID502_17B, (m_pRec->ts522.msgn[0] & 0x20) ? 3 : 0);//手入力の場合背景色を青色に
	}
	//（税率６．２４％）１５×２
	memmove(dd.data_val, m_pRec->ts522.val181, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_18A, (struct IUnknown *)&dd, CIcsdiagctrl);
	//（税率７．８％）１７×２
	memmove(dd.data_val, m_pRec->ts522.val182, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_18B, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率６．２４％）控除対象外仕入れに係る調整額
	memmove(dd.data_val, m_pRec->ts522.val191, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_19A, (struct IUnknown *)&dd, CIcsdiagctrl);
	//（税率７．８％）控除対象外仕入れに係る調整額
	memmove(dd.data_val, m_pRec->ts522.val192, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_19B, (struct IUnknown *)&dd, CIcsdiagctrl);

	//（税率６．２４％）控除対象外仕入れに係る調整額の合計額
	memmove(dd.data_val, m_pRec->ts522.val201, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_20A, (struct IUnknown *)&dd, CIcsdiagctrl);
	//（税率７．８％）控除対象外仕入れに係る調整額の合計額
	memmove(dd.data_val, m_pRec->ts522.val202, sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_20B, (struct IUnknown *)&dd, CIcsdiagctrl);
}

//入力可能な範囲を調べる
void CShinSpecific52_2_31::SetFocusRange()
{
	DIAGRAM_ATTRIBUTE DA;

	m_FcsUpEnd = -1;
	m_FcsDownEnd = -1;
	m_FcsLeftEnd = -1;
	m_FcsRightEnd = -1;

	//上から確かめていって入力可能なものがあったらm_FcsUpEndに代入し、break
	for (int idx = 0; idx < FCS_SPECIFIC1CNT; idx++) {
		m_Specific52_2_31.GetAttribute(FCS_Specific52_2_31[idx].IDNo, (LPUNKNOWN)&DA);
		//入力ボックスの色がクリーム色なら入力可である。
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsUpEnd = FCS_Specific52_2_31[idx].IDNo;
			break;
		}
	}

	//左から確かめていって入力可能なものがあったらm_FcsLeftEndに代入し、break
	for (int idx = 0; idx < FCS_SPECIFIC1CNT; idx++) {
		m_Specific52_2_31.GetAttribute(FCS_Specific52_2_31_LR[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsLeftEnd = FCS_Specific52_2_31_LR[idx].IDNo;
			break;
		}
	}

	//右から確かめていって入力可能なものがあったらm_FcsRightEndに代入し、break（構造体を逆からたどる）
	for (int idx = FCS_SPECIFIC1CNT - 1; idx > -1; idx--) {
		m_Specific52_2_31.GetAttribute(FCS_Specific52_2_31_LR[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsRightEnd = FCS_Specific52_2_31_LR[idx].IDNo;
			break;
		}
	}
	//下から確かめていって入力可能なものがあったらm_FcsDownEndに代入し、break（構造体を逆からたどる）
	for (int idx = FCS_SPECIFIC1CNT - 1; idx > -1; idx--) {
		m_Specific52_2_31.GetAttribute(FCS_Specific52_2_31[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsDownEnd = FCS_Specific52_2_31[idx].IDNo;
			break;
		}
	}
}

//入力の可、不可を設定
void CShinSpecific52_2_31::IndexControl(void)
{
	int Cnt = 0;

	while (1) {

		if (Specific52_2_31[Cnt].Index == 0) {
			break;
		}

		int Flg;
		//入力ロック中のとき
		if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
			Flg = Specific52_2_31[Cnt].AllUnConnect;
		}
		//入力ロック中でない時
		else {
			//入力ロック中でない　かつ　非連動がオン
			if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
				Flg = Specific52_2_31[Cnt].UnConnect;
			}
			//入力ロック中でない　かつ　非連動がオフ（連動）
			else {
				Flg = Specific52_2_31[Cnt].Connect;
			}
		}

		//入力の可不可をセット
		ATRIB_MOD(IDC_ICSDIAG8CTRL1, Specific52_2_31[Cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Specific52_2_31[Cnt].Connect);
		Cnt++;
	}
	//入力ロック中のとき水色のメッセージは非表示
	if((*m_pSnHeadData)->Sn_Sign4 & 0x01)
		//diag_modify(IDC_ICSDIAG8CTRL1, 153, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl);
		diag_modify(IDC_ICSDIAG8CTRL1, 155, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl);

	SetFocusRange();

	//DELETEキー１回で削除モード設定
	//この設定をしないと、Deleteを２回押さないとTerminationに入ってこなくなる。
	m_Specific52_2_31.EnableDelete(0);

}

//=========================================================
// 入力項目色変更
//---------------------------------------------------------
// 引数
//			id			:		ダイアグラムID
//			Index		:		ダイアグラム内インデックス
//			sign		:		変更サイン
//=========================================================
void CShinSpecific52_2_31::ChangeColor(unsigned short id, short Index, int sign)
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



HBRUSH CShinSpecific52_2_31::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ここで DC の属性を変更してください。
	if (nCtlColor == CTLCOLOR_DLG) {
		hbr = Ibrsh_ShinSpecific;
	}
	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


void CShinSpecific52_2_31::OnSize(UINT nType, int cx, int cy)
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
		m_Specific52_2_31.GetRect(BottomIndex, (LPUNKNOWN)&b);
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


void CShinSpecific52_2_31::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
BEGIN_EVENTSINK_MAP(CShinSpecific52_2_31, CSpcBaseDlg)
	ON_EVENT(CShinSpecific52_2_31, IDC_ICSDIAG8CTRL1, 2, CShinSpecific52_2_31::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific52_2_31, IDC_ICSDIAG8CTRL1, 1, CShinSpecific52_2_31::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific52_2_31, IDC_ICSDIAG8CTRL1, 3, CShinSpecific52_2_31::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()


void CShinSpecific52_2_31::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 24/02/15_特定収入 add -->
	DIAGRAM_DATA	dd;
	dd.data_imgdata = NULL;
	// 編集した金額を構造体 TSREC52 に保存
	switch (index) {
	case	ID502_1_bunshi:			//1. 取戻し対象特定収入のあった課税期間の調整割合　分子
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val011, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_1_bunbo:	//1. 取戻し対象特定収入のあった課税期間の調整割合　分母
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val012, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_2_bunshi:	//2. １－［１］　分子
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val021, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_2_bunbo:		//2. １－［１］　分母
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val022, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_3A:		//3. （税率６．２４％）控除対象外仕入れに係る支払対価の額の合計額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val031, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_4A:		//4. （税率６．２４％）３×６．２４／１０８
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		if (m_pArith->l_cmp(m_pRec->ts522.val041, dd.data_val)) {
			memmove(m_pRec->ts522.val041, dd.data_val, MONY_BUF_SIZE);
			m_pRec->ts522.msgn[0] |= 0x01;
		}
		break;
	case	ID502_5B:		//5. （税率７．８％）控除対象外仕入れに係る支払対価の額の合計額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val052, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_6B:			 //6. （税率７．８％）５×７．８／１１０
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		if (m_pArith->l_cmp(m_pRec->ts522.val062, dd.data_val)) {
			memmove(m_pRec->ts522.val062, dd.data_val, MONY_BUF_SIZE);
			m_pRec->ts522.msgn[0] |= 0x02;
		}
		break;
	case	ID502_7A:			//7. （税率６．２４％）控除対象外仕入れに係る調整額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val071, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_7B:			//7. （税率７．８％）控除対象外仕入れに係る調整額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val072, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_8A:			//8. （税率６．２４％）28年改正法附則第52条第1項(80%控除)の適用を受ける控除対象外仕入れに係る支払対価の額(税率6.24%)の合計額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val081, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_9A:			//9. （税率６．２４％）８×６．２４／１０８
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		if (m_pArith->l_cmp(m_pRec->ts522.val091, dd.data_val)) {
			memmove(m_pRec->ts522.val091, dd.data_val, MONY_BUF_SIZE);
			m_pRec->ts522.msgn[0] |= 0x04;
		}
		break;
	case	ID502_10B:			//10.（税率７．８％）28年改正法附則第52条第1項(50%控除)の適用を受ける控除対象外仕入れに係る支払対価の額(税率7.8%)の合計額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val102, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_11B:			//11.（税率７．８％）１０×７．８／１１０
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		if (m_pArith->l_cmp(m_pRec->ts522.val112, dd.data_val)) {
			memmove(m_pRec->ts522.val112, dd.data_val, MONY_BUF_SIZE);
			m_pRec->ts522.msgn[0] |= 0x08;
		}
		break;
	case	ID502_12A:			//12.（税率６．２４％）９×２
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val121, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_12B:			//12.（税率７．８％）１１×２
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val122, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_13A:			//13.（税率６．２４％）控除対象外仕入れに係る調整額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val131, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_13B:			//13.（税率７．８％）控除対象外仕入れに係る調整額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val132, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_14A:			//14.（税率６．２４％）28年改正法附則第53条第1項(50%控除)の適用を受ける控除対象外仕入れに係る支払対価の額(税率6.24%)の合計額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val141, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_15A:			//15.（税率６．２４％）１４×６．２４／１０８
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		if (m_pArith->l_cmp(m_pRec->ts522.val151, dd.data_val)) {
			memmove(m_pRec->ts522.val151, dd.data_val, MONY_BUF_SIZE);
			m_pRec->ts522.msgn[0] |= 0x10;
		}
		break;
	case	ID502_16B:			//16.（税率７．８％）28年改正法附則第53条第1項(50%控除)の適用を受ける控除対象外仕入れに係る支払対価の額(税率7.8%)の合計額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val162, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_17B:			//17.（税率７．８％）１６×７．８／１１０
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		if (m_pArith->l_cmp(m_pRec->ts522.val172, dd.data_val)) {
			memmove(m_pRec->ts522.val172, dd.data_val, MONY_BUF_SIZE);
			m_pRec->ts522.msgn[0] |= 0x20;
		}
		break;
	case	ID502_18A:			//18.（税率６．２４％）１５×２
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val181, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_18B:			//18.（税率７．８％）１７×２
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val182, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_19A:			//19.（税率６．２４％）控除対象外仕入れに係る調整額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val191, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_19B:			//19.（税率７．８％）控除対象外仕入れに係る調整額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val192, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_20A:			//20.（税率６．２４％）控除対象外仕入れに係る調整額の合計額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val201, dd.data_val, MONY_BUF_SIZE);
		break;
	case	ID502_20B:			//20.（税率７．８％）控除対象外仕入れに係る調整額の合計額
		diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
		memmove(m_pRec->ts522.val202, dd.data_val, MONY_BUF_SIZE);
		break;
	}
	// 再計算
	Specific52_2_31_Calq();
	// 24/02/15_特定収入 add <--
}

void CShinSpecific52_2_31::Specific52_2_31_Calq()
{
	DIAGRAM_DATA	dd;
	dd.data_imgdata = NULL;

	char bf20[6], bf50[6], bf100[6];
	m_pArith->l_input(bf20, "20");
	m_pArith->l_input(bf50, "50");
	m_pArith->l_input(bf100, "100");
	//１－［１］の計算
	if (m_pArith->l_test(m_pRec->ts522.val011) && m_pArith->l_test(m_pRec->ts522.val012)) {
		m_pArith->l_sub(m_pRec->ts522.val021, m_pRec->ts522.val012, m_pRec->ts522.val011);
		memmove(m_pRec->ts522.val022, m_pRec->ts522.val012, MONY_BUF_SIZE);
		// 1/-3,-1/-3など分母にマイナスがついている場合は符号を反転する (24,03,19 kajima)
		if ( m_pArith->l_test(m_pRec->ts522.val022) < 0) {
			m_pArith->l_neg(m_pRec->ts522.val021);
			m_pArith->l_neg(m_pRec->ts522.val022);
		}
	}
	else if (!m_pArith->l_test(m_pRec->ts522.val011)) {
		memmove(m_pRec->ts522.val021, m_pRec->ts522.val012, MONY_BUF_SIZE);
		memmove(m_pRec->ts522.val022, m_pRec->ts522.val012, MONY_BUF_SIZE);
	}
	else if (!m_pArith->l_test(m_pRec->ts522.val012)) {
		memset(m_pRec->ts522.val021, '\0', MONY_BUF_SIZE);
		memset(m_pRec->ts522.val022, '\0', MONY_BUF_SIZE);
	}
	//（税率６．２４％）３×６．２４／１０８
	if (!(m_pRec->ts522.msgn[0] & 0x01)) {
		m_Util.percent(m_pRec->ts522.val041, m_pRec->ts522.val031, 11, 62, 0);
	}
	//（税率６．２４％）控除対象外仕入れに係る調整額
	//m_pArith->l_mul(m_pRec->ts522.val071, m_pRec->ts522.val041, m_pRec->ts522.val021);
	//m_pArith->l_div(m_pRec->ts522.val071, m_pRec->ts522.val071, m_pRec->ts522.val022);
	m_Util.l_pardotnet_syz(m_pRec->ts522.val071, m_pRec->ts522.val041, m_pRec->ts522.val021, m_pRec->ts522.val022);
	
	//（税率７．８％）５×７．８／１１０
	if (!(m_pRec->ts522.msgn[0] & 0x02)) {
		m_Util.percent(m_pRec->ts522.val062, m_pRec->ts522.val052, 12, 78, 0);
	}
	//（税率７．８％）控除対象外仕入れに係る調整額
	//m_pArith->l_mul(m_pRec->ts522.val072, m_pRec->ts522.val062, m_pRec->ts522.val021);
	//m_pArith->l_div(m_pRec->ts522.val072, m_pRec->ts522.val072, m_pRec->ts522.val022);
	m_Util.l_pardotnet_syz(m_pRec->ts522.val072, m_pRec->ts522.val062, m_pRec->ts522.val021, m_pRec->ts522.val022);

	//（税率６．２４％）８×６．２４／１０８
	if (!(m_pRec->ts522.msgn[0] & 0x04)) {
		m_Util.percent(m_pRec->ts522.val091, m_pRec->ts522.val081, 11, 62, 0);
	}
	//（税率６．２４％）９×２
	//m_pArith->l_mul(m_pRec->ts522.val121, m_pRec->ts522.val091, m_pRec->ts522.val021);
	//m_pArith->l_div(m_pRec->ts522.val121, m_pRec->ts522.val121, m_pRec->ts522.val022);
	m_Util.l_pardotnet_syz(m_pRec->ts522.val121, m_pRec->ts522.val091, m_pRec->ts522.val021, m_pRec->ts522.val022);

	//（税率６．２４％）控除対象外仕入れに係る調整額
	//m_pArith->l_mul(m_pRec->ts522.val131, m_pRec->ts522.val121, bf20);
	//m_pArith->l_div(m_pRec->ts522.val131, m_pRec->ts522.val131, bf100);
	m_Util.l_pardotnet_syz(m_pRec->ts522.val131, m_pRec->ts522.val121, bf20, bf100);

	//（税率７．８％）１０×７．８／１１０
	if (!(m_pRec->ts522.msgn[0] & 0x08)) {
		m_Util.percent(m_pRec->ts522.val112, m_pRec->ts522.val102, 12, 78, 0);
	}
	//（税率７．８％）１１×２
	//m_pArith->l_mul(m_pRec->ts522.val122, m_pRec->ts522.val112, m_pRec->ts522.val021);
	//m_pArith->l_div(m_pRec->ts522.val122, m_pRec->ts522.val122, m_pRec->ts522.val022);
	m_Util.l_pardotnet_syz(m_pRec->ts522.val122, m_pRec->ts522.val112, m_pRec->ts522.val021, m_pRec->ts522.val022);

	//（税率７．８％）控除対象外仕入れに係る調整額
	//m_pArith->l_mul(m_pRec->ts522.val132, m_pRec->ts522.val122, bf20);
	//m_pArith->l_div(m_pRec->ts522.val132, m_pRec->ts522.val132, bf100);
	m_Util.l_pardotnet_syz(m_pRec->ts522.val132, m_pRec->ts522.val122, bf20, bf100);

	//（税率６．２４％）１４×６．２４／１０８
	if (!(m_pRec->ts522.msgn[0] & 0x10)) {
		m_Util.percent(m_pRec->ts522.val151, m_pRec->ts522.val141, 11, 62, 0);
	}
	//（税率６．２４％）１５×２
	//m_pArith->l_mul(m_pRec->ts522.val181, m_pRec->ts522.val151, m_pRec->ts522.val021);
	//m_pArith->l_div(m_pRec->ts522.val181, m_pRec->ts522.val181, m_pRec->ts522.val022);
	m_Util.l_pardotnet_syz(m_pRec->ts522.val181, m_pRec->ts522.val151, m_pRec->ts522.val021, m_pRec->ts522.val022);

	//（税率６．２４％）控除対象外仕入れに係る調整額
	//m_pArith->l_mul(m_pRec->ts522.val191, m_pRec->ts522.val181, bf50);
	//m_pArith->l_div(m_pRec->ts522.val191, m_pRec->ts522.val191, bf100);
	m_Util.l_pardotnet_syz(m_pRec->ts522.val191, m_pRec->ts522.val181, bf50, bf100);

	//（税率７．８％）１６×７．８／１１０
	if (!(m_pRec->ts522.msgn[0] & 0x20)) {
		m_Util.percent(m_pRec->ts522.val172, m_pRec->ts522.val162, 12, 78, 0);
	}
	//（税率７．８％）１７×２
	//m_pArith->l_mul(m_pRec->ts522.val182, m_pRec->ts522.val172, m_pRec->ts522.val021);
	//m_pArith->l_div(m_pRec->ts522.val182, m_pRec->ts522.val182, m_pRec->ts522.val022);
	m_Util.l_pardotnet_syz(m_pRec->ts522.val182, m_pRec->ts522.val172, m_pRec->ts522.val021, m_pRec->ts522.val022);

	//（税率７．８％）控除対象外仕入れに係る調整額
	//m_pArith->l_mul(m_pRec->ts522.val192, m_pRec->ts522.val182, bf20);
	//m_pArith->l_div(m_pRec->ts522.val192, m_pRec->ts522.val192, bf100);
	m_Util.l_pardotnet_syz(m_pRec->ts522.val192, m_pRec->ts522.val182, bf50, bf100);

	//20.（税率６．２４％）控除対象外仕入れに係る調整額の合計額
	//if (m_pArith->l_test(m_pRec->ts522.val071) || m_pArith->l_test(m_pRec->ts522.val131) || m_pArith->l_test(m_pRec->ts522.val191)) {		// 24/03/05_特定収入対応 del
		m_pArith->l_add(m_pRec->ts522.val201, m_pRec->ts522.val071, m_pRec->ts522.val131);
		m_pArith->l_add(m_pRec->ts522.val201, m_pRec->ts522.val201, m_pRec->ts522.val191);
	//}
	//20.（税率７．８％）控除対象外仕入れに係る調整額の合計額
	//if (m_pArith->l_test(m_pRec->ts522.val072) || m_pArith->l_test(m_pRec->ts522.val132) || m_pArith->l_test(m_pRec->ts522.val192)) {		// 24/03/05_特定収入対応 del
		m_pArith->l_add(m_pRec->ts522.val202, m_pRec->ts522.val072, m_pRec->ts522.val132);
		m_pArith->l_add(m_pRec->ts522.val202, m_pRec->ts522.val202, m_pRec->ts522.val192);
	//}
	SpecificDisp52_2_31();
}

void CShinSpecific52_2_31::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL1;
}

void CShinSpecific52_2_31::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
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
		m_Specific52_2_31.SetPosition(pos);
	}
	//下へ：0
	if (wTerm == VK_DOWN) {
		pos = FocusMoveCheck(index, 0);
		m_Specific52_2_31.SetPosition(pos);
	}
	//右へ：2
	if ((wTerm == VK_RETURN) || (wTerm == VK_F3) || (wTerm == VK_RIGHT)) {
		pos = FocusMoveCheck(index, 2);
		m_Specific52_2_31.SetPosition(pos);
	}
	//左へ：3
	else if ((wTerm == VK_TAB) || (wTerm == VK_LEFT) || (wTerm == VK_F2)) {
		pos = FocusMoveCheck(index, 3);
		m_Specific52_2_31.SetPosition(pos);
	}
	if (wTerm == VK_DELETE) {
		switch (index) {
		case ID502_4A:
			m_pRec->ts522.msgn[0] &= ~0x01;
			break;
		case ID502_6B:
			m_pRec->ts522.msgn[0] &= ~0x02;
			break;
		case ID502_9A:
			m_pRec->ts522.msgn[0] &= ~0x04;
			break;
		case ID502_11B:
			m_pRec->ts522.msgn[0] &= ~0x08;
			break;
		case ID502_15A:
			m_pRec->ts522.msgn[0] &= ~0x10;
			break;
		case ID502_17B:
			m_pRec->ts522.msgn[0] &= ~0x20;
			break;

		default:
			m_Specific52_2_31.DataClear(index, TRUE);
		
			break;
		}
		m_Specific52_2_31.SetPosition(index);
	}

	SetRedrawScrollToIdx(m_Specific52_2_31.GetPosition());
}


//次のフォーカスの場所検索（上下左右動作）
//戻り値：次のフォーカスのインデックス
int CShinSpecific52_2_31::FocusMoveCheck(int Index, int Move)
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
		for (i = 0; i < FCS_SPECIFIC1CNT; i++) {
			if (Index == FCS_Specific52_2_31[i].IDNo) {
				break;
			}
		}
		idx = FCS_Specific52_2_31[i].IDNo; //現在のポジションをidxに代入
	}
	//横移動の場合
	else if (Move == 2 || Move == 3) {
		for (i = 0; i < FCS_SPECIFIC1CNT; i++) {
			if (Index == FCS_Specific52_2_31[i].IDNo) {
				break;
			}
		}
		idx = FCS_Specific52_2_31_LR[i].IDNo; //現在のポジションをidxに代入
	}

	//移動後の位置を判定
	DIAGRAM_ATTRIBUTE DA;
	//下方向
	if (Move == 0) {
		for (j = i + 1; j < FCS_SPECIFIC1CNT; j++) {
			m_Specific52_2_31.GetAttribute(FCS_Specific52_2_31[j].IDNo, (LPUNKNOWN)&DA);
			//入力可のボックスかどうかを色で判定、入力可ならそこが移動先
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = FCS_Specific52_2_31[j].IDNo;
				break;
			}
		}
	}

	//上方向
	if (Move == 1) {
		for (j = i - 1; j > -1; j--) {
			m_Specific52_2_31.GetAttribute(FCS_Specific52_2_31[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = FCS_Specific52_2_31[j].IDNo;
				break;
			}
		}
	}

	//右方向
	if (Move == 2) {
		for (j = i + 1; j < FCS_SPECIFIC1CNT; j++) {
			m_Specific52_2_31.GetAttribute(FCS_Specific52_2_31_LR[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = FCS_Specific52_2_31_LR[j].IDNo;
				break;
			}
		}
	}

	//左方向
	if (Move == 3) {
		for (j = i - 1; j > -1; j--) {
			m_Specific52_2_31.GetAttribute(FCS_Specific52_2_31_LR[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = FCS_Specific52_2_31_LR[j].IDNo;
				break;
			}
		}
	}

	return idx;
}