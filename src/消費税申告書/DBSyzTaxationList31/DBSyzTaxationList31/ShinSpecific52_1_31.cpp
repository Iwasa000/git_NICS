#include "stdafx.h"
#include "ShinSpecific52_1_31.h"
#include "ShinSpecific52_1_31_exIdx.h"
#include "TaxationList31View.h"
#include "SpeAddDlg.h"

// CShinSpecific52_1_31 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific52_1_31, CSpcBaseDlg)

// 24/02/15_特定収入 cor -->
//CShinSpecific52_1_31::CShinSpecific52_1_31(CWnd* pParent /*=nullptr*/)
// -------------------------
CShinSpecific52_1_31::CShinSpecific52_1_31(CWnd* pParent /*=nullptr*/, TSREC52* pRec/*=NULL*/)
// 24/02/15_特定収入 cor <--
	: CSpcBaseDlg(IDD_DIALOG_K52_1_31, pParent)
	, m_pParent(pParent)
	, m_wPos(0)
	, m_cmPos(0)
	, m_ScW(0)
	, m_SySgn(FALSE)
{
	m_curwnd = -1;
// 24/02/15_特定収入 add -->
	m_pRec=pRec;
// 24/02/15_特定収入 add <--
}

CShinSpecific52_1_31::~CShinSpecific52_1_31()
{
}

void CShinSpecific52_1_31::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific52_1_31);
}


BEGIN_MESSAGE_MAP(CShinSpecific52_1_31, CSpcBaseDlg)
// 240306_160349 add -->
	ON_MESSAGE(WM_MYMESSAGESETFOCUS, OnMyMessage)
// 240306_160349 add <--
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CShinSpecific52_1_31 メッセージ ハンドラー


BOOL CShinSpecific52_1_31::PreTranslateMessage(MSG* pMsg)
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


BOOL CShinSpecific52_1_31::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();
	SpecificInit52_1_31();
	m_initfg = 1;

	// TODO: ここに初期化を追加してください
	CSpcBaseDlg::OnInitDialogEX();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinSpecific52_1_31::SpecificInit52_1_31()
{
	// 倍長演算指定
	m_pArith->l_defn(0x16);

	// 金額表示
	SpecificDisp52_1_31();
	//入力の可、不可を設定
	IndexControl();
	//カーソルの位置をセット
	SetItemPosition();
}

void CShinSpecific52_1_31::SetItemPosition()
{
	int index;

	//前回のポジション復元
	if (m_curwnd == IDC_ICSDIAG8CTRL1) {

		if ((index = m_Specific52_1_31.GetPosition()) != -1) {

			switch (index) {
			case ID502_name:
				index = ID502_name;
				break;
			case ID502_1:
				index = ID502_1;
				break;
			case ID502_2:
				index = ID502_2;
				break;
			default:
				break;
			}
			m_Specific52_1_31.SetPosition(index);
			return;
		}
	}

	this->SetFocus();
	//前回のポジションがないとき
	//非連動のときは43番（4E）にセット

	m_Specific52_1_31.SetPosition(ID502_name);

	SetRedrawScroll(0);
}

//==============================
// 金額表示
//==============================
void CShinSpecific52_1_31::SpecificDisp52_1_31()
{
	char			bf[128]={0};
	DIAGRAM_DATA	dd;
	dd.data_imgdata = NULL;

	// 課税仕入れ等に係る特定収入の種類・名称等
	dd.data_edit = m_pRec->ts521.name;
// 240228_No4.higuchi cor -->
	//diag_setdata(IDC_ICSDIAG8CTRL1,ID502_name,(struct IUnknown *)&dd,CIcsdiagctrl);
// --------------------------
	if(dd.data_edit.IsEmpty() == FALSE)	{
		diag_setdata(IDC_ICSDIAG8CTRL1,ID502_name,(struct IUnknown *)&dd,CIcsdiagctrl);
	}
	else	{
		diag_clear(IDC_ICSDIAG8CTRL1,ID502_name,TRUE,CIcsdiagctrl);
	}
// 240228_No4.higuchi cor <--

	// 課税仕入れ等に係る特定収入のあった課税期間（自）
	memset(bf,0,sizeof(bf));
	l_pack(bf,m_pRec->ts521.kikans,2);
	dd.data_day[0] = bf[0];
	diag_setdata(IDC_ICSDIAG8CTRL1,ID502_year_from,(struct IUnknown *)&dd,CIcsdiagctrl);
	memset(bf,0,sizeof(bf));
	l_pack(bf,&m_pRec->ts521.kikans[2],2);
	dd.data_day[0] = bf[0];
	diag_setdata(IDC_ICSDIAG8CTRL1,ID502_month_from,(struct IUnknown *)&dd,CIcsdiagctrl);
	memset(bf,0,sizeof(bf));
	l_pack(bf,&m_pRec->ts521.kikans[4],2);
	dd.data_day[0] = bf[0];
	diag_setdata(IDC_ICSDIAG8CTRL1,ID502_day_from,(struct IUnknown *)&dd,CIcsdiagctrl);

	// 課税仕入れ等に係る特定収入のあった課税期間（至）
	memset(bf,0,sizeof(bf));
	l_pack(bf,m_pRec->ts521.kikane,2);
	dd.data_day[0] = bf[0];
	diag_setdata(IDC_ICSDIAG8CTRL1,ID502_year_to,(struct IUnknown *)&dd,CIcsdiagctrl);
	memset(bf,0,sizeof(bf));
	l_pack(bf,&m_pRec->ts521.kikane[2],2);
	dd.data_day[0] = bf[0];
	diag_setdata(IDC_ICSDIAG8CTRL1,ID502_month_to,(struct IUnknown *)&dd,CIcsdiagctrl);
	memset(bf,0,sizeof(bf));
	l_pack(bf,&m_pRec->ts521.kikane[4],2);
	dd.data_day[0] = bf[0];
	diag_setdata(IDC_ICSDIAG8CTRL1,ID502_day_to,(struct IUnknown *)&dd,CIcsdiagctrl);

	// 課税仕入れ等に係る特定収入により支出された課税仕入れに係る支払対価の額の合計額
	memmove(dd.data_val,m_pRec->ts521.val1,sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1,ID502_1,(struct IUnknown *)&dd,CIcsdiagctrl);

	// 課税仕入れ等に係る特定収入により支出された控除対象外仕入れ等に係る支払対価の額の合計額
	memmove(dd.data_val,m_pRec->ts521.val2,sizeof(dd.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1,ID502_2,(struct IUnknown *)&dd,CIcsdiagctrl);

	// 取戻し対象特定収入の判定（２÷１）
	memmove(dd.data_val,m_pRec->ts521.val3,sizeof(dd.data_val));
	// 整数部のみ表示
	//char	vl[6]={0};
	//m_pArith->l_input(vl,_T("10"));
	//m_pArith->l_div(dd.data_val,dd.data_val,vl);
	diag_setdata(IDC_ICSDIAG8CTRL1,ID502_3,(struct IUnknown *)&dd,CIcsdiagctrl);

	CalcSelDisable();
}

//入力可能な範囲を調べる
void CShinSpecific52_1_31::SetFocusRange()
{
	DIAGRAM_ATTRIBUTE DA;

	m_FcsUpEnd = -1;
	m_FcsDownEnd = -1;
	m_FcsLeftEnd = -1;
	m_FcsRightEnd = -1;

	//上から確かめていって入力可能なものがあったらm_FcsUpEndに代入し、break
	for (int idx = 0; idx < FCS_SPECIFIC1CNT; idx++) {
		m_Specific52_1_31.GetAttribute(FCS_Specific52_1_31[idx].IDNo, (LPUNKNOWN)&DA);
		//入力ボックスの色がクリーム色なら入力可である。
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsUpEnd = FCS_Specific52_1_31[idx].IDNo;
			break;
		}
	}

	//左から確かめていって入力可能なものがあったらm_FcsLeftEndに代入し、break
	for (int idx = 0; idx < FCS_SPECIFIC1CNT; idx++) {
		m_Specific52_1_31.GetAttribute(FCS_Specific52_1_31_LR[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsLeftEnd = FCS_Specific52_1_31_LR[idx].IDNo;
			break;
		}
	}

	//右から確かめていって入力可能なものがあったらm_FcsRightEndに代入し、break（構造体を逆からたどる）
	for (int idx = FCS_SPECIFIC1CNT - 1; idx > -1; idx--) {
		m_Specific52_1_31.GetAttribute(FCS_Specific52_1_31_LR[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsRightEnd = FCS_Specific52_1_31_LR[idx].IDNo;
			break;
		}
	}
	//下から確かめていって入力可能なものがあったらm_FcsDownEndに代入し、break（構造体を逆からたどる）
	for (int idx = FCS_SPECIFIC1CNT - 1; idx > -1; idx--) {
		m_Specific52_1_31.GetAttribute(FCS_Specific52_1_31[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsDownEnd = FCS_Specific52_1_31[idx].IDNo;
			break;
		}
	}
}

//入力の可、不可を設定
void CShinSpecific52_1_31::IndexControl(void)
{
	int Cnt = 0;
	DIAGRAM_ATTRIBUTE DA;

	while (1) {

		if (Specific52_1_31[Cnt].Index == 0) {
			break;
		}

		int Flg;
		//入力ロック中のとき
		if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
			Flg = Specific52_1_31[Cnt].AllUnConnect;
		}
		//入力ロック中でない時
		else {
			//入力ロック中でない　かつ　非連動がオン
			if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
				Flg = Specific52_1_31[Cnt].UnConnect;
			}
			//入力ロック中でない　かつ　非連動がオフ（連動）
			else {
				Flg = Specific52_1_31[Cnt].Connect;
			}
		}

		//入力の可不可をセット
		ATRIB_MOD(IDC_ICSDIAG8CTRL1, Specific52_1_31[Cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Specific52_1_31[Cnt].Connect);
		Cnt++;
	}
	// のちにCalcSelDisable()が呼び出されエディット可能な状態に上書きされるのでここでは入力ロック時の処理を行わない(24,03,19 kajima)
/*	if((*m_pSnHeadData)->Sn_Sign4 & 0x01){
		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check2, &DA, CIcsdiagctrl);
		DA.attr_fcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check2, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check2, DIAG_MDFY_READONLY);

		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check3, &DA, CIcsdiagctrl);
		DA.attr_fcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check3, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check3, DIAG_MDFY_READONLY);

		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check4, &DA, CIcsdiagctrl);
		DA.attr_fcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check4, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check4, DIAG_MDFY_READONLY);
	}*/
	
	m_Specific52_1_31.GetAttribute(ID502_name, (LPUNKNOWN)&DA);
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_Specific52_1_31.SetAttribute(ID502_name, (LPUNKNOWN)&DA, TRUE);

	SetFocusRange();

	//DELETEキー１回で削除モード設定
	//この設定をしないと、Deleteを２回押さないとTerminationに入ってこなくなる。
	m_Specific52_1_31.EnableDelete(0);

}


HBRUSH CShinSpecific52_1_31::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ここで DC の属性を変更してください。
	if (nCtlColor == CTLCOLOR_DLG) {
		hbr = Ibrsh_ShinSpecific;
	}
	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


void CShinSpecific52_1_31::OnSize(UINT nType, int cx, int cy)
{
	CSpcBaseDlg::OnSize(nType, cx, cy);
	// TODO: ここにメッセージ ハンドラー コードを追加します。
}


void CShinSpecific52_1_31::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CSpcBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}
BEGIN_EVENTSINK_MAP(CShinSpecific52_1_31, CSpcBaseDlg)
	ON_EVENT(CShinSpecific52_1_31, IDC_ICSDIAG8CTRL1, 2, CShinSpecific52_1_31::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific52_1_31, IDC_ICSDIAG8CTRL1, 1, CShinSpecific52_1_31::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific52_1_31, IDC_ICSDIAG8CTRL1, 3, CShinSpecific52_1_31::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinSpecific52_1_31, IDC_ICSDIAG8CTRL1, 9, CShinSpecific52_1_31::CheckButtonIcsdiag8ctrl1, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()


void CShinSpecific52_1_31::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
// 24/02/15_特定収入 add -->
	char			bf[128]={0};
	DIAGRAM_DATA	dd;
	dd.data_imgdata = NULL;
	// 編集した金額を構造体 TSREC52 に保存
	switch(index)	{
		case	ID502_name:			//  6:課税仕入れ等に係る特定収入の種類・名称等
			diag_getdata(IDC_ICSDIAG8CTRL1,index,(struct IUnknown *)&dd,CIcsdiagctrl);
			sprintf_s(m_pRec->ts521.name, sizeof(m_pRec->ts521.name),_T("%s"),dd.data_edit.GetBuffer());
			break;
		case	ID502_year_from:	// 15:課税仕入れ等に係る特定収入のあった課税期間（自）
			diag_getdata(IDC_ICSDIAG8CTRL1,index,(struct IUnknown *)&dd,CIcsdiagctrl);
			memset(bf,0,sizeof(bf));
			sprintf_s(bf,sizeof(bf),_T("%02x"),(unsigned int)dd.data_day[0]);
			memmove(m_pRec->ts521.kikans,bf,2);
			break;
		case	ID502_month_from:	// 16:課税仕入れ等に係る特定収入のあった課税期間（自）
			diag_getdata(IDC_ICSDIAG8CTRL1,index,(struct IUnknown *)&dd,CIcsdiagctrl);
			memset(bf,0,sizeof(bf));
			sprintf_s(bf,sizeof(bf),_T("%02x"),(unsigned int)dd.data_day[0]);
			memmove(&m_pRec->ts521.kikans[2],bf,2);
			break;
		case	ID502_day_from:		// 17:課税仕入れ等に係る特定収入のあった課税期間（自）
			diag_getdata(IDC_ICSDIAG8CTRL1,index,(struct IUnknown *)&dd, CIcsdiagctrl);
			memset(bf,0,sizeof(bf));
			sprintf_s(bf,sizeof(bf),_T("%02x"),(unsigned int)dd.data_day[0]);
			memmove(&m_pRec->ts521.kikans[4],bf,2);
			break;
		case	ID502_year_to:		// 18:課税仕入れ等に係る特定収入のあった課税期間（至）
			diag_getdata(IDC_ICSDIAG8CTRL1,index,(struct IUnknown *)&dd,CIcsdiagctrl);
			memset(bf,0,sizeof(bf));
			sprintf_s(bf,sizeof(bf),_T("%02x"),(unsigned int)dd.data_day[0]);
			memmove(m_pRec->ts521.kikane,bf,2);
			break;
		case	ID502_month_to:		// 19:課税仕入れ等に係る特定収入のあった課税期間（至）
			diag_getdata(IDC_ICSDIAG8CTRL1,index,(struct IUnknown *)&dd,CIcsdiagctrl);
			memset(bf,0,sizeof(bf));
			sprintf_s(bf,sizeof(bf),_T("%02x"),(unsigned int)dd.data_day[0]);
			memmove(&m_pRec->ts521.kikane[2],bf,2);
			break;
		case	ID502_day_to:		// 20:課税仕入れ等に係る特定収入のあった課税期間（至）
			diag_getdata(IDC_ICSDIAG8CTRL1,index,(struct IUnknown *)&dd,CIcsdiagctrl);
			memset(bf,0,sizeof(bf));
			sprintf_s(bf,sizeof(bf),_T("%02x"),(unsigned int)dd.data_day[0]);
			memmove(&m_pRec->ts521.kikane[4],bf,2);
			break;
		case	ID502_1:			// 33:①課税仕入れ等に係る特定収入により支出された課税仕入れに係る支払対価の額の合計額
			diag_getdata(IDC_ICSDIAG8CTRL1,index,(struct IUnknown *)&dd,CIcsdiagctrl);
			memmove(m_pRec->ts521.val1,dd.data_val,MONY_BUF_SIZE);
			break;
		case	ID502_2:			// 34:②課税仕入れ等に係る特定収入により支出された控除対象外仕入れ等に係る支払対価の額の合計額
			diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&dd, CIcsdiagctrl);
			memmove(m_pRec->ts521.val2, dd.data_val, MONY_BUF_SIZE);
			break;
// 240229_h del -->
//// 240228_2_No3.higuchi cor -->
//		//case	ID502_check2:		// 37:計算表選択　5-2(2)
//		//	diag_getdata(IDC_ICSDIAG8CTRL1,index,(struct IUnknown *)&dd,CIcsdiagctrl);
//		//	if(dd.data_check)	m_pRec->ts521.sel |= 0x01;
//		//	else				m_pRec->ts521.sel &= ~0x01;
//		//	break;
//		//case	ID502_check3:		// 38:計算表選択　5-2(3)
//		//	diag_getdata(IDC_ICSDIAG8CTRL1,index,(struct IUnknown *)&dd,CIcsdiagctrl);
//		//	if(dd.data_check)	m_pRec->ts521.sel |= 0x02;
//		//	else				m_pRec->ts521.sel &= ~0x02;
//		//	break;
//		//case	ID502_check4:		// 39:計算表選択　5-2(4)
//		//	diag_getdata(IDC_ICSDIAG8CTRL1,index,(struct IUnknown *)&dd,CIcsdiagctrl);
//		//	if(dd.data_check)	m_pRec->ts521.sel |= 0x04;
//		//	else				m_pRec->ts521.sel &= ~0x04;
//		//	break;
//// -----------------------------
//		case	ID502_check2:		// 37:計算表選択　5-2(2)
//			diag_getdata(IDC_ICSDIAG8CTRL1,index,(struct IUnknown *)&dd,CIcsdiagctrl);
//			if(dd.data_check)	m_pRec->ts521.sel = 0x01;
//			else				m_pRec->ts521.sel = 0;
//			break;
//		case	ID502_check3:		// 38:計算表選択　5-2(3)
//			diag_getdata(IDC_ICSDIAG8CTRL1,index,(struct IUnknown *)&dd,CIcsdiagctrl);
//			if(dd.data_check)	m_pRec->ts521.sel = 0x02;
//			else				m_pRec->ts521.sel = 0;
//			break;
//		case	ID502_check4:		// 39:計算表選択　5-2(4)
//			diag_getdata(IDC_ICSDIAG8CTRL1,index,(struct IUnknown *)&dd,CIcsdiagctrl);
//			if(dd.data_check)	m_pRec->ts521.sel = 0x04;
//			else				m_pRec->ts521.sel = 0;
//			break;
//// 240228_2_No3.higuchi cor <--
// 240229_h del <--
	}
	// 再計算
	Specific52_1_31_Calq();
// 24/02/15_特定収入 add <--
}

void CShinSpecific52_1_31::Specific52_1_31_Calq()
{
	DIAGRAM_DATA	dd;
	dd.data_imgdata = NULL;

	//diag_getdata(IDC_ICSDIAG8CTRL1,ID502_3,(struct IUnknown *)&dd,CIcsdiagctrl);
	// 資産の譲渡等の対価の額の合計
	if(m_pArith->l_test(m_pRec->ts521.val1) && m_pArith->l_test(m_pRec->ts521.val2))	{
		//m_pArith->l_par100((unsigned char *)m_pRec->ts521.val3,(unsigned char *)m_pRec->ts521.val2,(unsigned char *)m_pRec->ts521.val1,1,9);
		//小数点第４位以下の端数を切り上げ(24,03,19 kajima)
		char SUB[6] = { 0 };
		m_pArith->l_input(SUB, _T("1000"));

		//切り上げ用の新たなモジュール関数作成
		m_Util.l_pardotnet_syz2(m_pRec->ts521.val3, SUB, m_pRec->ts521.val2, m_pRec->ts521.val1, 1);
	}
	else	{
		m_pArith->l_clear(m_pRec->ts521.val3);
	}
	//memmove(dd.data_val,m_pRec->ts521.val3,sizeof(dd.data_val));
	//diag_setdata(IDC_ICSDIAG8CTRL1,ID502_3,(struct IUnknown *)&dd,CIcsdiagctrl);
	SpecificDisp52_1_31();
}

void CShinSpecific52_1_31::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL1;
}


void CShinSpecific52_1_31::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if (nChar == 0) {
		return;
	}

	//フォーカス移動処理対応
	// Shift+TABをTABに、TABをENTERに返還
	short wTerm;
	char VK_FLG = 0x00;
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
		m_Specific52_1_31.SetPosition(pos);
	}
	//下へ：0
	if (wTerm == VK_DOWN) {
		pos = FocusMoveCheck(index, 0);
		m_Specific52_1_31.SetPosition(pos);
	}
	//右へ：2
	if ((wTerm == VK_RETURN) || (wTerm == VK_F3) || (wTerm == VK_RIGHT)) {
		pos = FocusMoveCheck(index, 2);
		m_Specific52_1_31.SetPosition(pos);
// 240306_160349 add -->
		PostMessage(WM_MYMESSAGESETFOCUS,index);
// 240306_160349 add <--
	}
	//左へ：3
	else if ((wTerm == VK_TAB) || (wTerm == VK_LEFT) || (wTerm == VK_F2)) {
		pos = FocusMoveCheck(index, 3);
		m_Specific52_1_31.SetPosition(pos);
	}
	if (wTerm == VK_DELETE) {
		m_Specific52_1_31.DataClear(index, TRUE);
		m_Specific52_1_31.SetPosition(index);
	}

	SetRedrawScrollToIdx(m_Specific52_1_31.GetPosition());
}


//次のフォーカスの場所検索（上下左右動作）
//戻り値：次のフォーカスのインデックス
int CShinSpecific52_1_31::FocusMoveCheck(int Index, int Move)
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
			if (Index == FCS_Specific52_1_31[i].IDNo) {
				break;
			}
		}
		idx = FCS_Specific52_1_31[i].IDNo; //現在のポジションをidxに代入
	}
	//横移動の場合
	else if (Move == 2 || Move == 3) {
		for (i = 0; i < FCS_SPECIFIC1CNT; i++) {
			if (Index == FCS_Specific52_1_31[i].IDNo) {
				break;
			}
		}
		idx = FCS_Specific52_1_31_LR[i].IDNo; //現在のポジションをidxに代入
	}

	//移動後の位置を判定
	DIAGRAM_ATTRIBUTE DA;
	//下方向
	if (Move == 0) {
		for (j = i + 1; j < FCS_SPECIFIC1CNT; j++) {
			m_Specific52_1_31.GetAttribute(FCS_Specific52_1_31[j].IDNo, (LPUNKNOWN)&DA);
			//入力可のボックスかどうかを色で判定、入力可ならそこが移動先
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE || DA.attr_bcolor == RGB(255, 251, 240)) {
				idx = FCS_Specific52_1_31[j].IDNo;
				break;
			}
		}
	}

	//上方向
	if (Move == 1) {
		for (j = i - 1; j > -1; j--) {
			m_Specific52_1_31.GetAttribute(FCS_Specific52_1_31[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE || DA.attr_bcolor == RGB(255, 251, 240)) {
				idx = FCS_Specific52_1_31[j].IDNo;
				break;
			}
		}
	}

	//右方向
	if (Move == 2) {
		for (j = i + 1; j < FCS_SPECIFIC1CNT; j++) {
			m_Specific52_1_31.GetAttribute(FCS_Specific52_1_31_LR[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE || DA.attr_bcolor == RGB(255, 251, 240)) {
				idx = FCS_Specific52_1_31_LR[j].IDNo;
				break;
			}
		}
	}

	//左方向
	if (Move == 3) {
		for (j = i - 1; j > -1; j--) {
			m_Specific52_1_31.GetAttribute(FCS_Specific52_1_31_LR[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE || DA.attr_bcolor == RGB(255, 251, 240)) {
				idx = FCS_Specific52_1_31_LR[j].IDNo;
				break;
			}
		}
	}

	return idx;
}

void CShinSpecific52_1_31::CheckButtonIcsdiag8ctrl1(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	DIAGRAM_DATA		DiagData2, DiagData3, DiagData4;
	DiagData2.data_imgdata = NULL;
	DiagData3.data_imgdata = NULL;
	DiagData4.data_imgdata = NULL;
	char flg[MONY_BUF_SIZE] = { 0 };
	diag_getdata(IDC_ICSDIAG8CTRL1, ID502_check2, (struct IUnknown *)&DiagData2, CIcsdiagctrl);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID502_check3, (struct IUnknown *)&DiagData3, CIcsdiagctrl);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID502_check4, (struct IUnknown *)&DiagData4, CIcsdiagctrl);
	// ICSMessageBoxによってEditOffへはしるのを防ぐ

	// 課税売上割合の変更チェック
	if (index == ID502_check2) {
		((CSpeAddDlg*)m_pParent)->chktype = 0;	
		if (DiagData2.data_check) {
			((CSpeAddDlg*)m_pParent)->AddTab(0);
			if (DiagData3.data_check) {
				DiagData3.data_check = 0;
				diag_setdata(IDC_ICSDIAG8CTRL1, ID502_check3, (struct IUnknown *)&DiagData3, CIcsdiagctrl);
			}
			if (DiagData4.data_check) {
				DiagData4.data_check = 0;
				diag_setdata(IDC_ICSDIAG8CTRL1, ID502_check4, (struct IUnknown *)&DiagData4, CIcsdiagctrl);
			}
// 240229_h add -->
			m_pRec->ts521.sel = 0x01;
// 240229_h add <--
		}
		else {
			((CSpeAddDlg*)m_pParent)->DelTab(0);
// 240229_h add -->
			m_pRec->ts521.sel = 0x00;
// 240229_h add <--
		}
	}
	else if (index == ID502_check3) {
		((CSpeAddDlg*)m_pParent)->chktype = 1;
		if (DiagData3.data_check) {
			((CSpeAddDlg*)m_pParent)->AddTab(1);
			if (DiagData2.data_check) {
				DiagData2.data_check = 0;
				diag_setdata(IDC_ICSDIAG8CTRL1, ID502_check2, (struct IUnknown *)&DiagData2, CIcsdiagctrl);
			}
			if (DiagData4.data_check) {
				DiagData4.data_check = 0;
				diag_setdata(IDC_ICSDIAG8CTRL1, ID502_check4, (struct IUnknown *)&DiagData4, CIcsdiagctrl);
			}
// 240229_h add -->
			m_pRec->ts521.sel = 0x02;
// 240229_h add <--
		}
		else {
			((CSpeAddDlg*)m_pParent)->DelTab(1);
// 240229_h add -->
			m_pRec->ts521.sel = 0x00;
// 240229_h add <--
		}
	}
	else if (index == ID502_check4) {
		((CSpeAddDlg*)m_pParent)->chktype = 2;
		if (DiagData4.data_check) {
			((CSpeAddDlg*)m_pParent)->AddTab(2);
			if (DiagData2.data_check) {
				DiagData2.data_check = 0;
				diag_setdata(IDC_ICSDIAG8CTRL1, ID502_check2, (struct IUnknown *)&DiagData2, CIcsdiagctrl);
			}
			if (DiagData3.data_check) {
				DiagData3.data_check = 0;
				diag_setdata(IDC_ICSDIAG8CTRL1, ID502_check3, (struct IUnknown *)&DiagData3, CIcsdiagctrl);
			}
// 240229_h add -->
			m_pRec->ts521.sel = 0x04;
// 240229_h add <--
		}
		else {
			((CSpeAddDlg*)m_pParent)->DelTab(2);
// 240229_h add -->
			m_pRec->ts521.sel = 0x00;
// 240229_h add <--
		}
	}
// 240305_160345 add -->
	((CSpeAddDlg*)m_pParent)->ListChgButtonCtrl();
// 240305_160345 add <--
}

void CShinSpecific52_1_31::CalcSelChange( int pSel )
{
	// 計算表選択
	DIAGRAM_DATA		dd2,dd3,dd4;
	int					oldsel=0;
	dd2.data_imgdata = NULL;
	dd3.data_imgdata = NULL;
	dd4.data_imgdata = NULL;
	diag_getdata(IDC_ICSDIAG8CTRL1,ID502_check2,(struct IUnknown *)&dd2,CIcsdiagctrl);
	diag_getdata(IDC_ICSDIAG8CTRL1,ID502_check3,(struct IUnknown *)&dd3,CIcsdiagctrl);
	diag_getdata(IDC_ICSDIAG8CTRL1,ID502_check4,(struct IUnknown *)&dd4,CIcsdiagctrl);
	if(dd2.data_check != 0)			oldsel = 0x01;
	else if(dd3.data_check != 0)	oldsel = 0x02;
	else if(dd4.data_check != 0)	oldsel = 0x04;
	if(pSel != oldsel)	{
		if(pSel == 0)	{
			// 5-2(2)をオフにする
			if(oldsel == 0x01)	{
				((CSpeAddDlg*)m_pParent)->DelTab(0);
				dd2.data_check = 0;
				diag_setdata(IDC_ICSDIAG8CTRL1,ID502_check2,(struct IUnknown *)&dd2,CIcsdiagctrl);
			}
			// 5-2(3)をオフにする
			else if(oldsel == 0x02)	{
				((CSpeAddDlg*)m_pParent)->DelTab(1);
				dd3.data_check = 0;
				diag_setdata(IDC_ICSDIAG8CTRL1,ID502_check3,(struct IUnknown *)&dd3,CIcsdiagctrl);
			}
			// 5-2(4)をオフにする
			else if(oldsel == 0x04)	{
				((CSpeAddDlg*)m_pParent)->DelTab(2);
				dd4.data_check = 0;
				diag_setdata(IDC_ICSDIAG8CTRL1,ID502_check4,(struct IUnknown *)&dd4,CIcsdiagctrl);
			}
		}
		else	{
			if(pSel == 0x01)	{
				// 5-2(2)をオンにする
				((CSpeAddDlg*)m_pParent)->chktype = 0;
				((CSpeAddDlg*)m_pParent)->AddTab(0);
				if(dd3.data_check)	{
					dd3.data_check = 0;
					diag_setdata(IDC_ICSDIAG8CTRL1,ID502_check3,(struct IUnknown *)&dd3,CIcsdiagctrl);
				}
				if(dd4.data_check)	{
					((CSpeAddDlg*)m_pParent)->chktype = 1;
					dd4.data_check = 0;
					diag_setdata(IDC_ICSDIAG8CTRL1,ID502_check4,(struct IUnknown *)&dd4,CIcsdiagctrl);
				}
				dd2.data_check = 1;
				diag_setdata(IDC_ICSDIAG8CTRL1,ID502_check2,(struct IUnknown *)&dd2,CIcsdiagctrl);
			}
			else if(pSel == 0x02)	{
				// 5-2(3)をオンにする
				((CSpeAddDlg*)m_pParent)->chktype = 1;
				((CSpeAddDlg*)m_pParent)->AddTab(1);
				if(dd2.data_check)	{
					dd2.data_check = 0;
					diag_setdata(IDC_ICSDIAG8CTRL1,ID502_check2,(struct IUnknown *)&dd2,CIcsdiagctrl);
				}
				if(dd4.data_check)	{
					dd4.data_check = 0;
					diag_setdata(IDC_ICSDIAG8CTRL1,ID502_check4,(struct IUnknown *)&dd4,CIcsdiagctrl);
				}
				dd3.data_check = 1;
				diag_setdata(IDC_ICSDIAG8CTRL1,ID502_check3,(struct IUnknown *)&dd3,CIcsdiagctrl);
			}
			else if(pSel == 0x04)	{
				// 5-2(4)をオンにする
				((CSpeAddDlg*)m_pParent)->chktype = 2;
				((CSpeAddDlg*)m_pParent)->AddTab(2);
				if(dd2.data_check)	{
					dd2.data_check = 0;
					diag_setdata(IDC_ICSDIAG8CTRL1,ID502_check2,(struct IUnknown *)&dd2,CIcsdiagctrl);
				}
				if(dd3.data_check)	{
					dd3.data_check = 0;
					diag_setdata(IDC_ICSDIAG8CTRL1,ID502_check3,(struct IUnknown *)&dd3,CIcsdiagctrl);
				}
				dd4.data_check = 1;
				diag_setdata(IDC_ICSDIAG8CTRL1,ID502_check4,(struct IUnknown *)&dd4,CIcsdiagctrl);
			}
		}
		//diag_redraw(IDC_ICSDIAG8CTRL1,CIcsdiagctrl);
	}
// 240305_160345 add -->
	((CSpeAddDlg*)m_pParent)->ListChgButtonCtrl();
// 240305_160345 add <--
}

void CShinSpecific52_1_31::CalcSelDisable()
{
	DIAGRAM_ATTRIBUTE	DA;
	char id502_3[128];

	// 入力ロック時、計算表選択のチェックはオフにする(24,03,19 kajima)
	if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check2, &DA, CIcsdiagctrl);
		DA.attr_fcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check2, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check2, DIAG_MDFY_READONLY);

		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check3, &DA, CIcsdiagctrl);
		DA.attr_fcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check3, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check3, DIAG_MDFY_READONLY);

		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check4, &DA, CIcsdiagctrl);
		DA.attr_fcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check4, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check4, DIAG_MDFY_READONLY);
		return;
	}


	m_pArith->l_print(id502_3, m_pRec->ts521.val3, "ssssssssssss");
	if (atoi(id502_3) <= 50) {
		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check2, &DA, CIcsdiagctrl);
		DA.attr_fcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check2, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check2, DIAG_MDFY_READONLY);

		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check3, &DA, CIcsdiagctrl);
		DA.attr_fcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check3, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check3, DIAG_MDFY_READONLY);

		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check4, &DA, CIcsdiagctrl);
		DA.attr_fcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check4, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check4, DIAG_MDFY_READONLY);

		SetFocusRange();


		DIAGRAM_DATA		dd2, dd3, dd4;
		int					oldsel = 0;
		dd2.data_imgdata = NULL;
		dd3.data_imgdata = NULL;
		dd4.data_imgdata = NULL;
		diag_getdata(IDC_ICSDIAG8CTRL1, ID502_check2, (struct IUnknown *)&dd2, CIcsdiagctrl);
		diag_getdata(IDC_ICSDIAG8CTRL1, ID502_check3, (struct IUnknown *)&dd3, CIcsdiagctrl);
		diag_getdata(IDC_ICSDIAG8CTRL1, ID502_check4, (struct IUnknown *)&dd4, CIcsdiagctrl);
		if (dd2.data_check != 0)			oldsel = 0x01;
		else if (dd3.data_check != 0)	oldsel = 0x02;
		else if (dd4.data_check != 0)	oldsel = 0x04;
		if (!(oldsel & 0x0f)) return;
		switch ((oldsel & 0x0f)) {
		case 0x01:
			// 5-2(2)をオフにする
			((CSpeAddDlg*)m_pParent)->chktype = 0;
			((CSpeAddDlg*)m_pParent)->m_SpeTab.DeleteItem(1);
			dd2.data_check = 0;
			diag_setdata(IDC_ICSDIAG8CTRL1, ID502_check2, (struct IUnknown *)&dd2, CIcsdiagctrl);
			break;
		case 0x02:
			// 5-2(3)をオフにする
			((CSpeAddDlg*)m_pParent)->chktype = 1;
// 240308_h_160358 cor -->
//// 240228_No2.higuchi add -->
//			((CSpeAddDlg*)m_pParent)->m_SpeTab.DeleteItem(3);
//// 240228_No2.higuchi add <--
// -----------------------
			if(((CSpeAddDlg*)m_pParent)->m_Eymd > KIKAN_END_INV80)	{
				((CSpeAddDlg*)m_pParent)->m_SpeTab.DeleteItem(3);
			}
// 240308_h_160358 cor <--
			((CSpeAddDlg*)m_pParent)->m_SpeTab.DeleteItem(2);
			((CSpeAddDlg*)m_pParent)->m_SpeTab.DeleteItem(1);
			dd3.data_check = 0;
			diag_setdata(IDC_ICSDIAG8CTRL1, ID502_check3, (struct IUnknown *)&dd3, CIcsdiagctrl);
			break;
		case 0x04:
			// 5-2(4)をオフにする
			((CSpeAddDlg*)m_pParent)->chktype = 2;
// 240308_h_160358 cor -->
//// 240228_No2.higuchi add -->
//			((CSpeAddDlg*)m_pParent)->m_SpeTab.DeleteItem(3);
//// 240228_No2.higuchi add <--
// -----------------------
			if(((CSpeAddDlg*)m_pParent)->m_Eymd > KIKAN_END_INV80)	{
				((CSpeAddDlg*)m_pParent)->m_SpeTab.DeleteItem(3);
			}
// 240308_h_160358 cor <--
			((CSpeAddDlg*)m_pParent)->m_SpeTab.DeleteItem(2);
			((CSpeAddDlg*)m_pParent)->m_SpeTab.DeleteItem(1);
			dd4.data_check = 0;
			diag_setdata(IDC_ICSDIAG8CTRL1, ID502_check4, (struct IUnknown *)&dd4, CIcsdiagctrl);
			break;
		}
// 240305_160344 add -->
		m_pRec->ts521.sel = 0x00;
// 240305_160344 add <--
	}
	else {
		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check2, &DA, CIcsdiagctrl);
		DA.attr_bcolor = BC_CREAM;
		DA.attr_fcolor = RGB(0, 0, 0);
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check2, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check2, DIAG_MDFY_EDIT);

		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check3, &DA, CIcsdiagctrl);
		DA.attr_bcolor = BC_CREAM;
		DA.attr_fcolor = RGB(0, 0, 0);
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check3, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check3, DIAG_MDFY_EDIT);

		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check4, &DA, CIcsdiagctrl);
		DA.attr_bcolor = BC_CREAM;
		DA.attr_fcolor = RGB(0, 0, 0);
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check4, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check4, DIAG_MDFY_EDIT);

		SetFocusRange();

		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check2, &DA, CIcsdiagctrl);
		DA.attr_bcolor = RGB(255, 251, 240);
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check2, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check2, DIAG_MDFY_EDIT);

		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check3, &DA, CIcsdiagctrl);
		DA.attr_bcolor = RGB(255, 251, 240);
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check3, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check3, DIAG_MDFY_EDIT);

		diag_getattr(IDC_ICSDIAG8CTRL1, ID502_check4, &DA, CIcsdiagctrl);
		DA.attr_bcolor = RGB(255, 251, 240);
		diag_setattr(IDC_ICSDIAG8CTRL1, ID502_check4, &DA, TRUE, CIcsdiagctrl);
		m_Specific52_1_31.ModifyItem(ID502_check4, DIAG_MDFY_EDIT);
	}
// 240305_160345 add -->
	((CSpeAddDlg*)m_pParent)->ListChgButtonCtrl();
// 240305_160345 add <--
}

// 240306_160349 add -->
LRESULT CShinSpecific52_1_31::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	if((short)wParam == ID502_2)	{
		char				per[6]={0};
		char				id502_3[128]={0};
		short				pos=0;
		DIAGRAM_ATTRIBUTE	da;
		if(m_pArith->l_test(m_pRec->ts521.val1) && m_pArith->l_test(m_pRec->ts521.val2))	{
			m_pArith->l_par100((unsigned char *)per,(unsigned char *)m_pRec->ts521.val2,(unsigned char *)m_pRec->ts521.val1,1,9);
		}
		m_pArith->l_print(id502_3, m_pRec->ts521.val3, "ssssssssssss");
		diag_getattr(IDC_ICSDIAG8CTRL1,ID502_check2,&da,CIcsdiagctrl);
		if(atoi(id502_3) <= 50)	{
			da.attr_fcolor = BC_GRAY;
			m_Specific52_1_31.ModifyItem(ID502_check2,DIAG_MDFY_READONLY);
			pos = ID502_name;
		}
		else	{
			da.attr_bcolor = BC_CREAM;
			da.attr_fcolor = RGB(0,0,0);
			m_Specific52_1_31.ModifyItem(ID502_check2,DIAG_MDFY_EDIT);
			pos = ID502_check2;
		}
		diag_setattr(IDC_ICSDIAG8CTRL1,ID502_check2,&da,TRUE,CIcsdiagctrl);
		if(m_Specific52_1_31.GetPosition() != pos)	{
			m_Specific52_1_31.SetPosition(pos);
		}
	}
	return(0);
}
// 240306_160349 add <--
