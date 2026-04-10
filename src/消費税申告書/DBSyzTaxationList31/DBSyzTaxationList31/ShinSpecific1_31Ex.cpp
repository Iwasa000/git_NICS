
#include "stdafx.h"
#include "ShinSpecific1_31Ex.h"
#include "ShinSpecific1_31_exIdx.h"
#include "TaxationList31View.h"


// CShinSpecific1_31Ex ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific1_31Ex, CSpcBaseDlg)

CShinSpecific1_31Ex::CShinSpecific1_31Ex(CWnd* pParent /*=nullptr*/)
	: CSpcBaseDlg(IDD_DIALOG_K1_31_ex, pParent)
{
	m_curwnd = -1;

	m_FcsUpEnd = -1;
	m_FcsLeftEnd = -1;
	m_FcsDownEnd = -1;
	m_FcsRightEnd = -1;
}

CShinSpecific1_31Ex::~CShinSpecific1_31Ex()
{
}

void CShinSpecific1_31Ex::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific1Ex);
}


BEGIN_MESSAGE_MAP(CShinSpecific1_31Ex, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CShinSpecific1_31Ex メッセージ ハンドラー
BOOL CShinSpecific1_31Ex::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	//初期設定
	SpecificInit1Ex();

	CSpcBaseDlg::OnInitDialogEX();

	return TRUE ;
}

void CShinSpecific1_31Ex::OnSize(UINT nType, int cx, int cy)
{
	CSpcBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
}


HBRUSH CShinSpecific1_31Ex::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific1_31Ex::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CSpcBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CShinSpecific1_31Ex::PreTranslateMessage(MSG* pMsg)
{
	//キーを押したときの処理
	if (pMsg->message == WM_KEYDOWN) {
		//ENDキーを押したときの処理
		if (pMsg->wParam == VK_END ) {
			if (m_pParent){
				m_pParent->PostMessageA(WM_KEYDOWN , VK_END) ;
			}
			return TRUE ;
		}
		//F2キーを押したときの処理
		if (pMsg->wParam == VK_F2) {
			DIAGRAM_DATA dd ;
			if (m_curwnd != -1) {
				//m_curwndにダイアグラムのハンドルを代入　resultにカーソル位置のオフセットを代入
				short result = ((CIcsdiagctrl*)GetDlgItem(m_curwnd))->GetPosition() ;
				//カーソル位置のオフセットのデータを取得
				diag_getdata(m_curwnd , result , &dd , CIcsdiagctrl) ;
				if (m_curwnd == IDC_ICSDIAG8CTRL1) {
					TerminationIcsdiag8ctrl1 (result , (short)pMsg->wParam , 0 , (LPUNKNOWN)&dd) ;
				}
				return TRUE ;
			}
		}
	}

	return CSpcBaseDlg::PreTranslateMessage(pMsg);
}

//初期設定
void CShinSpecific1_31Ex::SpecificInit1Ex()
{
	// 倍長演算指定
	m_pArith->l_defn(0x16);

	//
	Specific1Ex_Calq();

	//金額の表示
	SpecificDisp1Ex();

	//入力の可、不可を設定
	IndexControl();

	//起動時のカーソル位置を設定
	SetItemPosition();

}

//金額の表示
void CShinSpecific1_31Ex::SpecificDisp1Ex(void)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	//課税売上
	//「一括税抜き」のとき、かつ申告書基本情報「積上げ計算を行う」のチェックがONのとき
	//２に金額が表示され、１には何も表示されない
	if ((m_pZmSub->zvol->s_sgn1 & 0xc0) && ((*m_pSnHeadData)->Sn_TUMIAG & 0x01)) {
	
		//税率６．２４％適用分
		memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpKzur624, sizeof(DiagData.data_val));
		diag_setdata(IDC_ICSDIAG8CTRL1, ID101_2A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		diag_clear(IDC_ICSDIAG8CTRL1, ID101_1A, TRUE, CIcsdiagctrl);

		//税率７．８％適用分
		memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpKzur78, sizeof(DiagData.data_val));
		diag_setdata(IDC_ICSDIAG8CTRL1, ID101_2B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		diag_clear(IDC_ICSDIAG8CTRL1, ID101_1B, TRUE, CIcsdiagctrl);

		//合計
		memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpKzur, sizeof(DiagData.data_val));
		diag_setdata(IDC_ICSDIAG8CTRL1, ID101_2C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		diag_clear(IDC_ICSDIAG8CTRL1, ID101_1C, TRUE, CIcsdiagctrl);

	}
	//申告書基本情報「積上げ計算を行う」のチェックがOFFのとき
	//１に金額が表示され、２には何も表示されない
	else {

		//税率６．２４％適用分
		memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpKzur624, sizeof(DiagData.data_val));
		diag_setdata(IDC_ICSDIAG8CTRL1, ID101_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		diag_clear(IDC_ICSDIAG8CTRL1, ID101_2A, TRUE, CIcsdiagctrl);

		//税率７．８％適用分
		memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpKzur78, sizeof(DiagData.data_val));
		diag_setdata(IDC_ICSDIAG8CTRL1, ID101_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		diag_clear(IDC_ICSDIAG8CTRL1, ID101_2B, TRUE, CIcsdiagctrl);

		//合計
		memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpKzur, sizeof(DiagData.data_val));
		diag_setdata(IDC_ICSDIAG8CTRL1, ID101_1C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		diag_clear(IDC_ICSDIAG8CTRL1, ID101_2C, TRUE, CIcsdiagctrl);
	}

	//免税売上げ
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpMzur, sizeof(DiagData.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID101_3C, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	//非課税売上げ
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpHkur, sizeof(DiagData.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID101_4C, (struct IUnknown *)&DiagData, CIcsdiagctrl);


	//国外における資産の譲渡等の対価の額
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpKgjt, sizeof(DiagData.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID101_5C, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	//資産の譲渡等の対価の額の合計額
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpTuwb, sizeof(DiagData.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID101_6C, (struct IUnknown *)&DiagData, CIcsdiagctrl);

}

//入力の可、不可を設定
void CShinSpecific1_31Ex::IndexControl(void)
{
	int Cnt = 0;

	while (1) {

		if (Specific1_31Ex[Cnt].Index == 0) {
			break;
		}

		int Flg;
		//入力ロック中のとき
		if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
			Flg = Specific1_31Ex[Cnt].AllUnConnect;
		}

		//入力ロック中でない時
		else {
			//入力ロック中でない　かつ　非連動がオン
			if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {

				if ((*m_pSnHeadData)->Sn_TUMIAG & 0x01) {
					if ((Specific1_31Ex[Cnt].Index == ID101_1A) || (Specific1_31Ex[Cnt].Index == ID101_1B)) {
						Flg = 0;
					}
					else {
						Flg = Specific1_31Ex[Cnt].UnConnect;
					}
				}
				else {
					if ((Specific1_31Ex[Cnt].Index == ID101_2A) || (Specific1_31Ex[Cnt].Index == ID101_2B)) {
						Flg = 0;
					}
					else {
						Flg = Specific1_31Ex[Cnt].UnConnect;
					}
				}
			}
			//入力ロック中でない　かつ　非連動がオフ（連動）
			else {
				Flg = Specific1_31Ex[Cnt].Connect;
			}
		}

		//入力の可不可をセット
		ATRIB_MOD(IDC_ICSDIAG8CTRL1, Specific1_31Ex[Cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Specific1_31Ex[Cnt].Connect);
		Cnt++;
	}

	SetFocusRange();

	//DELETEキー１回で削除モード設定
	//この設定をしないと、Deleteを２回押さないとTerminationに入ってこなくなる。
	m_Specific1Ex.EnableDelete(0);

}

//入力可能な範囲を調べる
void CShinSpecific1_31Ex::SetFocusRange()
{
	DIAGRAM_ATTRIBUTE DA;

	m_FcsUpEnd = -1;
	m_FcsDownEnd = -1;
	m_FcsLeftEnd = -1;
	m_FcsRightEnd = -1;

	//上から確かめていって入力可能なものがあったらm_FcsUpEndに代入し、break
	for (int idx = 0; idx < FCS_SPECIFIC1CNT; idx++) {
		m_Specific1Ex.GetAttribute(FCS_Specific1_31Ex[idx].IDNo, (LPUNKNOWN)&DA);
		//入力ボックスの色がクリーム色なら入力可である。
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsUpEnd = FCS_Specific1_31Ex[idx].IDNo;
			break;
		}
	}

	//左から確かめていって入力可能なものがあったらm_FcsLeftEndに代入し、break
	for (int idx = 0; idx < FCS_SPECIFIC1CNT; idx++) {
		m_Specific1Ex.GetAttribute(FCS_Specific1_31Ex_LR[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsLeftEnd = FCS_Specific1_31Ex_LR[idx].IDNo;
			break;
		}
	}

	//右から確かめていって入力可能なものがあったらm_FcsRightEndに代入し、break（構造体を逆からたどる）
	for (int idx = FCS_SPECIFIC1CNT - 1; idx > -1; idx--) {
		m_Specific1Ex.GetAttribute(FCS_Specific1_31Ex_LR[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsRightEnd = FCS_Specific1_31Ex_LR[idx].IDNo;
			break;
		}
	}
	//下から確かめていって入力可能なものがあったらm_FcsDownEndに代入し、break（構造体を逆からたどる）
	for (int idx = FCS_SPECIFIC1CNT - 1; idx > -1; idx--) {
		m_Specific1Ex.GetAttribute(FCS_Specific1_31Ex[idx].IDNo, (LPUNKNOWN)&DA);
		if (DA.attr_bcolor == BC_CREAM) {
			m_FcsDownEnd = FCS_Specific1_31Ex[idx].IDNo;
			break;
		}
	}

}


void CShinSpecific1_31Ex::Specific1Ex_Calq()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
		// 合計額の計算
		memset((*m_pSpcListData)->stval.SpKzur, '\0', sizeof((*m_pSpcListData)->stval.SpKzur));
		m_pArith->l_add((*m_pSpcListData)->stval.SpKzur, (*m_pSpcListData)->stval.SpKzur, (*m_pSpcListData)->stval.SpKzur624);
		m_pArith->l_add((*m_pSpcListData)->stval.SpKzur, (*m_pSpcListData)->stval.SpKzur, (*m_pSpcListData)->stval.SpKzur78);
	}
	memset((*m_pSpcListData)->stval.SpTuwb, '\0', sizeof((*m_pSpcListData)->stval.SpTuwb));	// 分母
	m_pArith->l_add((*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpKzur);
	m_pArith->l_add((*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpMzur);
	m_pArith->l_add((*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpHkur);
	m_pArith->l_add((*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpKgjt);
	// 資産の譲渡等の対価の額の合計
	memmove(DiagData.data_val, (*m_pSpcListData)->stval.SpTuwb, sizeof(DiagData.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID101_6C, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	unsigned char	svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;
	(*m_pSpcListData)->SPassOffCalqMain(*m_pSnHeadData);
	BOOL	IsChgDoSpc = FALSE;
	if ((svSn_Sign2 & 0x02) != ((*m_pSnHeadData)->Sn_Sign2 & 0x02)) {
		IsChgDoSpc = TRUE;
	}
	((CTaxationList31View*)m_pParent)->LinkHonpyoData(IsChgDoSpc);

	SpecificDisp1Ex();
}

//起動時のカーソル位置を設定
void CShinSpecific1_31Ex::SetItemPosition()
{
	int index;

	//前回のポジション復元
	if (m_curwnd == IDC_ICSDIAG8CTRL1) {

		if ((index = m_Specific1Ex.GetPosition()) != -1) {

			switch (index) {
			case ID101_1A:
			case ID101_1B:
				if (((*m_pSnHeadData)->Sn_TUMIAG & 0x01)) {
					if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
						index = ID101_2A;
					}
					else {
						index = ID101_5C;
					}
				}
				else 
					index = ID101_1A;
				break;
			case ID101_2A:
			case ID101_2B:
				if (!((*m_pSnHeadData)->Sn_TUMIAG & 0x01)) {
					if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
						index = ID101_1A;
					}
					else {
						index = ID101_5C;
					}
				}
				else
					index = ID101_2A;
				break;
			default:
				break;
			}
			m_Specific1Ex.SetPosition(index);
			return;
		}
	}

	this->SetFocus();
	//前回のポジションがないとき
	//非連動のときは43番（4E）にセット
	if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
		if (((*m_pSnHeadData)->Sn_TUMIAG & 0x01)) {
			m_Specific1Ex.SetPosition(ID101_4C);
		}
		else {
			m_Specific1Ex.SetPosition(ID101_1A);
		}
	}
	//連動のときは44番（5E)にセット
	else {
		m_Specific1Ex.SetPosition(ID101_5C);
	}
	SetRedrawScroll(0);
}

BEGIN_EVENTSINK_MAP(CShinSpecific1_31Ex, CSpcBaseDlg)
	ON_EVENT(CShinSpecific1_31Ex, IDC_ICSDIAG8CTRL1, 1, CShinSpecific1_31Ex::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific1_31Ex, IDC_ICSDIAG8CTRL1, 2, CShinSpecific1_31Ex::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific1_31Ex, IDC_ICSDIAG8CTRL1, 3, CShinSpecific1_31Ex::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSpecific1_31Ex::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL1;
}

void CShinSpecific1_31Ex::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	ASSERT(m_pSpcListData);
	if (m_pSpcListData == NULL) {
		return;
	}
	ASSERT(*m_pSpcListData);
	if (*m_pSpcListData == NULL) {
		return;
	}
	if (IsWindowVisible() == FALSE) {
		return;
	}

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	//編集した金額を構造体_STUVALに保存
	switch (index) {
		case ID101_1A:
			diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			memmove((*m_pSpcListData)->stval.SpKzur624, DiagData.data_val, MONY_BUF_SIZE);
			break;
		case ID101_2A:
			diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			memmove((*m_pSpcListData)->stval.SpKzur624, DiagData.data_val, MONY_BUF_SIZE);
			break;
		case ID101_1B:
			diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			memmove((*m_pSpcListData)->stval.SpKzur78, DiagData.data_val, MONY_BUF_SIZE);
			break;
		case ID101_2B:
			diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			memmove((*m_pSpcListData)->stval.SpKzur78, DiagData.data_val, MONY_BUF_SIZE);
			break;
		case ID101_1C:
			diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			memmove((*m_pSpcListData)->stval.SpKzur, DiagData.data_val, MONY_BUF_SIZE);
			break;
		case ID101_2C:
			diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			memmove((*m_pSpcListData)->stval.SpKzur, DiagData.data_val, MONY_BUF_SIZE);
			break;
		case ID101_3C:
			diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			memmove((*m_pSpcListData)->stval.SpMzur, DiagData.data_val, MONY_BUF_SIZE);
			break;
		case ID101_4C:
			diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			memmove((*m_pSpcListData)->stval.SpHkur, DiagData.data_val, MONY_BUF_SIZE);
			break;
		case ID101_5C:
			diag_getdata(IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			memmove((*m_pSpcListData)->stval.SpKgjt, DiagData.data_val, MONY_BUF_SIZE);
			break;
		default:
			break;
	}

	//再計算
	Specific1Ex_Calq();
}



void CShinSpecific1_31Ex::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
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
		m_Specific1Ex.SetPosition(pos);
	}
	//下へ：0
	if (wTerm == VK_DOWN) {
		pos = FocusMoveCheck(index, 0);
		m_Specific1Ex.SetPosition(pos);
	}
	//右へ：2
	if ((wTerm == VK_RETURN) || (wTerm == VK_F3) || (wTerm == VK_RIGHT)) {
		pos = FocusMoveCheck(index, 2);
		m_Specific1Ex.SetPosition(pos);
	}
	//左へ：3
	else if ((wTerm == VK_TAB) || (wTerm == VK_LEFT) || (wTerm == VK_F2)) {
		pos = FocusMoveCheck(index, 3);
		m_Specific1Ex.SetPosition(pos);
	}
	if (wTerm == VK_DELETE) {
		m_Specific1Ex.DataClear(index, TRUE);
		m_Specific1Ex.SetPosition(index);
	}

	SetRedrawScrollToIdx(m_Specific1Ex.GetPosition());
}

//次のフォーカスの場所検索（上下左右動作）
//戻り値：次のフォーカスのインデックス
int CShinSpecific1_31Ex::FocusMoveCheck(int Index, int Move)
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
			if (Index == FCS_Specific1_31Ex[i].IDNo) {
				break;
			}
		}
		idx = FCS_Specific1_31Ex[i].IDNo; //現在のポジションをidxに代入
	}
	//横移動の場合
	else if (Move == 2 || Move == 3) {
		for (i = 0; i < FCS_SPECIFIC1CNT; i++) {
			if (Index == FCS_Specific1_31Ex_LR[i].IDNo) {
				break;
			}
		}
		idx = FCS_Specific1_31Ex_LR[i].IDNo; //現在のポジションをidxに代入
	}

	//移動後の位置を判定
	DIAGRAM_ATTRIBUTE DA;
	//下方向
	if (Move == 0) {
		for (j = i + 1; j < FCS_SPECIFIC1CNT; j++) {
			m_Specific1Ex.GetAttribute(FCS_Specific1_31Ex[j].IDNo, (LPUNKNOWN)&DA);
			//入力可のボックスかどうかを色で判定、入力可ならそこが移動先
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = FCS_Specific1_31Ex[j].IDNo;
				break;
			}
		}
	}

	//上方向
	if (Move == 1) {
		for (j = i - 1; j > -1; j--) {
			m_Specific1Ex.GetAttribute(FCS_Specific1_31Ex[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = FCS_Specific1_31Ex[j].IDNo;
				break;
			}
		}
	}

	//右方向
	if (Move == 2) {
		for (j = i + 1; j < FCS_SPECIFIC1CNT; j++) {
			m_Specific1Ex.GetAttribute(FCS_Specific1_31Ex_LR[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = FCS_Specific1_31Ex_LR[j].IDNo;
				break;
			}
		}
	}

	//左方向
	if (Move == 3) {
		for (j = i - 1; j > -1; j--) {
			m_Specific1Ex.GetAttribute(FCS_Specific1_31Ex_LR[j].IDNo, (LPUNKNOWN)&DA);
			if (DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE) {
				idx = FCS_Specific1_31Ex_LR[j].IDNo;
				break;
			}
		}
	}

	return idx;
}