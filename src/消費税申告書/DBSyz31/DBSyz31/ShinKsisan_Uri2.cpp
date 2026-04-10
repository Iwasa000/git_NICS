//
// ShinKsisan_Uri2.cpp : 実装ファイル

#include "stdafx.h"
#include "ShinKsisan_Uri2.h"
#include "ShinKsisan_Uri2Idx.h"
#include "afxdialogex.h"


// CShinKsisan_Uri2 ダイアログ

IMPLEMENT_DYNAMIC(CShinKsisan_Uri2, CSyzBaseDlg)

CShinKsisan_Uri2::CShinKsisan_Uri2(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(IDD_DIALOG_KSISAN_URI2, pParent)
	, m_pSnHeadData(NULL)
	, m_pShinInfo(NULL)
	, m_pZmSub(NULL)
	, m_pArith(NULL)
	, m_pDBSyzAccess(NULL)
	, m_TokureiMode(TOKUREIMODE_RENDO)
	, m_pParent(pParent)
{
	InitTblInfo();
	ClearData();
	m_curwnd = -1;
	m_dispCol_left = 0;
	m_Focus = 0;
	m_SnSeq = -1;
}

CShinKsisan_Uri2::~CShinKsisan_Uri2()
{

	if (m_pSimKuriwari) {
		delete m_pSimKuriwari;
		m_pSimKuriwari = NULL;
	}

	if (m_pDBSyzAccess) {
		if (m_pDBSyzAccess->m_pSyz_Tokurei) {
			m_pDBSyzAccess->Syz_TokureiClose();
		}
		delete m_pDBSyzAccess;
		m_pDBSyzAccess = NULL;
	}

}

void CShinKsisan_Uri2::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Ksisan_Uri2Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Ksisan_Uri2Diag02);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_bar);
	DDX_Control(pDX, IDC_BUTTON_URI2_SYUKEI, m_ButtonUri2Syukei);
	DDX_Control(pDX, IDC_BUTTON1, m_ButtonUri2ColIns);
	DDX_Control(pDX, IDC_BUTTON2, m_ButtonUri2ColDel);
}


BEGIN_MESSAGE_MAP(CShinKsisan_Uri2, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_URI2_SYUKEI, &CShinKsisan_Uri2::OnBnClickedButtonUri2Syukei)
	ON_BN_CLICKED(IDC_BUTTON1, &CShinKsisan_Uri2::OnBnClickedKubunIns)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinKsisan_Uri2::OnBnClickedKubunDel)
END_MESSAGE_MAP()


// CShinKsisan_Uri2 メッセージ ハンドラー


BOOL CShinKsisan_Uri2::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください

	// 初期化
	Init();

	// 縦スクロール
	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2);
	m_BottomIdx = KSISANURI2_DIAGCTRLID_MAX;	// 一番下のindexをセットする

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinKsisan_Uri2::OnSize(UINT nType, int cx, int cy)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。.
	CSyzBaseDlg::OnSize(nType, cx, cy);

	MoveWindow_JigyoKubunBar();
}

void CShinKsisan_Uri2::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);

	MoveWindow_JigyoKubunBar();
}

HBRUSH CShinKsisan_Uri2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinKsisan_Uri2::OnBnClickedButtonUri2Syukei()
{

	SIMKSIREWARIZREC Rec;
	//第5-(2)号様式では特定課税仕入は課税売上割合が95%以上であっても記載.
//	m_pSimKuriwari->SyukeiKsirewari(&Rec, (*m_pSnHeadData)->IsNeedTkkzDisp(), m_tekiyotaisyokikan_s, m_tekiyotaisyokikan_e);
	m_pSimKuriwari->SyukeiKsirewari(&Rec, 1, m_tekiyotaisyokikan_s, m_tekiyotaisyokikan_e);

	CString msgstr;
	msgstr = _T("集計結果を転記しますか？\n※事業区分や金額は全て上書きされます。");

	if (ICSMessageBox(msgstr, MB_YESNO, 0, 0, this) == IDYES) {

		// 転記後に列番号１にフォーカスをセットするので、0を入れておく
		m_dispCol_left = 0;
		m_Ksisan_Uri2Diag02.SetPosition(ID_KSURI2_TITLE1);

		SetSyukeiData(&Rec);

		DispClmNum();
		m_bar.SetScrollPos(0);

	}

}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数					：	
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//		SyzSyukei		：	集計クラス
//		snseq			：	履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinKsisan_Uri2::InitInfo(CSnHeadData **pSnHeadData, SYC_SHININFO *pShinInfo, CArithEx *pArith, CDBNpSub *pZmSub, CSyzSyukei **pSyzSyukei, int snseq)
{
	// データクラス作成したら追加
	ASSERT(pSnHeadData);
	if (pSnHeadData == NULL) {
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT(pShinInfo);
	if (pShinInfo == NULL) {
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT(pArith);
	if (pArith == NULL) {
		return -1;
	}
	m_pArith = pArith;

	ASSERT(pZmSub);
	if (pZmSub == NULL) {
		return -1;
	}
	m_pZmSub = pZmSub;

	ASSERT ( pSyzSyukei );
	if ( pSyzSyukei == NULL ) {
		return -1;
	}
	m_pSyzSyukei = pSyzSyukei;

	m_pDBSyzAccess = new CDBSyzAccess();

	m_pSimKuriwari = new CSimKuriwari(pZmSub);

	m_SnSeq = snseq;

	return 0;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinKsisan_Uri2::Init()
{

	if ((m_SyzUtil.IsUnConnectMst(m_pZmSub->zvol) == TRUE) //非連動マスター
		|| ((*m_pSnHeadData)->Sn_Sign4 & 0x80)) {          //非連動チェックON
		m_TokureiMode = TOKUREIMODE_HIRENDO;
	}
	else if (!((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->m_DispTabSgn & 0x80) && ((*m_pSnHeadData)->m_DispTabSgn & 0x100))) {//非連動OFF && □特例非連動：ON
		m_TokureiMode = TOKUREIMODE_HANRENDO;
	}
	else if (!((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->m_DispTabSgn & 0x80) && !((*m_pSnHeadData)->m_DispTabSgn & 0x100))) { // 非連動OFF && 特例非連動：OFF
		m_TokureiMode = TOKUREIMODE_RENDO;
	}

	if (((*m_pSnHeadData)->Sn_SKKBN == 0x02)
		|| ((*m_pSnHeadData)->Sn_SKKBN == 0x04)) {
		//中間申告.
		m_tekiyotaisyokikan_s = (*m_pSnHeadData)->Sn_MDAYS;
		m_tekiyotaisyokikan_e = (*m_pSnHeadData)->Sn_MDAYE;
	}
	else {
		m_tekiyotaisyokikan_s = (*m_pSnHeadData)->Sn_KDAYS;
		m_tekiyotaisyokikan_e = (*m_pSnHeadData)->Sn_KDAYE;
	}

	TokureiTblVersionCheck(m_SnSeq);

	// データ読込み
	//非連動・連動 共通
	CString filter;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), m_SnSeq, ICS_SN_TOKUREI_2_HYOID, TOKUREI_TBL_VERSION);
 	ReadData(m_SnSeq, TOKUREI_TBL_VERSION, filter);
	Update_KSURI2_2_4_TokuteikazeisireOff();

	SCROLLINFO SCI;
	SCI.cbSize = 0;
	SCI.fMask = SIF_ALL;
	SCI.nMin = 0;
	SCI.nMax = JIGYO_MAX_TOKUREI_23 - 1;
	SCI.nPage = JIGYO_PER_PAGE_URI2;
	SCI.nPos = 0;
	m_bar.SetScrollInfo(&SCI);
	m_bar.SetScrollPos(m_dispCol_left);

	if ((m_TokureiMode == TOKUREIMODE_HANRENDO)
		&& !((m_pZmSub->zvol->m_type & 0x01) == 0x01) //合併マスター
		&& !((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {   //入力ロック
		//m_ButtonUri2Syukei.EnableWindow(TRUE);
	}
	else {
		//m_ButtonUri2Syukei.EnableWindow(FALSE);
	}

	// 半連動を閉じたため、集計ボタンを非表示
	m_ButtonUri2Syukei.ShowWindow ( SW_HIDE );

	if (m_TokureiMode == TOKUREIMODE_RENDO
		|| ((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {    //入力ロック
		m_ButtonUri2ColIns.EnableWindow(FALSE);
		m_ButtonUri2ColDel.EnableWindow(FALSE);
	}
	else {
		m_ButtonUri2ColIns.EnableWindow(TRUE);
		m_ButtonUri2ColDel.EnableWindow(TRUE);
	}

	InitDiagAttr();    //ダイアグラム初期属性セット
	DispKsisan_Uri2(); //画面出力
	SetItemPosition(); //初期ポジションセット

	if (m_TokureiMode == TOKUREIMODE_RENDO) {
		SetSyukeiOnRendo();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
void CShinKsisan_Uri2::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if (m_curwnd == IDC_ICSDIAG8CTRL2) {
		index = m_Ksisan_Uri2Diag02.GetPosition();
		if ((index = m_Ksisan_Uri2Diag02.GetPosition()) != -1) {
			m_Ksisan_Uri2Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット
			m_Ksisan_Uri2Diag02.SetPosition(index);
			m_Ksisan_Uri2Diag02.SetFocus();
			return;
		}
	}

	m_Ksisan_Uri2Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット
	m_Ksisan_Uri2Diag02.SetFocus();

	// 課税仕入れに係る支払い対価の額（税込み）
	//if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
		diag_setposition(IDC_ICSDIAG8CTRL2, ID_KSURI2_TITLE1, CIcsdiagctrl);//初期フォーカス場所変更yoshida190812
//		diag_setposition(IDC_ICSDIAG8CTRL2, ID_KSURI2_1_1, CIcsdiagctrl);
	//}
}

void CShinKsisan_Uri2::InitTblInfo()
{
	kbndatainfo[0] = { _T("CVB00030"), DATAKIND_STR, KSURI2_TITLE[0], 64, &idx_ksuri2_title[0] };
	kbndatainfo[1] = { _T("CVB00050"), DATAKIND_VAL, KSURI2_1[0], MONY_BUF_SIZE, &idx_ksuri2_1[0] };
	kbndatainfo[2] = { _T("CVB00060"), DATAKIND_VAL, KSURI2_2[0], MONY_BUF_SIZE, &idx_ksuri2_2[0] };
	kbndatainfo[3] = { _T("CVB00070"), DATAKIND_VAL, KSURI2_3[0], MONY_BUF_SIZE, &idx_ksuri2_3[0] };
	kbndatainfo[4] = { _T("CVB00080"), DATAKIND_VAL, KSURI2_4[0], MONY_BUF_SIZE, &idx_ksuri2_4[0] };
	kbndatainfo[5] = { _T("CVB00090"), DATAKIND_VAL, KSURI2_5[0], MONY_BUF_SIZE, &idx_ksuri2_5[0] };
	kbndatainfo[6] = { _T("CVB00100"), DATAKIND_RATE, KSURI2_6[0], MONY_BUF_SIZE, &idx_ksuri2_6[0] };
	kbndatainfo[7] = { _T("CVB00105"), DATAKIND_CHECK, (char*)&KSURI2_6_CHECK[0], sizeof(int), &idx_ksuri2_6_check[0] };
	kbndatainfo[8] = { _T("CVB00110"), DATAKIND_VAL, KSURI2_7[0], MONY_BUF_SIZE, &idx_ksuri2_7[0] };
	kbndatainfo[9] = { _T("CVB00120"), DATAKIND_VAL, KSURI2_8[0], MONY_BUF_SIZE, &idx_ksuri2_8[0] };
	kbndatainfo[10] = { _T("CVB00125"), DATAKIND_SIGN, (char*)&KSURI2_8_SIGN[0], sizeof(int), &idx_ksuri2_dummy[0] };
	kbndatainfo[11] = { _T("CVB00130"), DATAKIND_VAL, KSURI2_9[0], MONY_BUF_SIZE, &idx_ksuri2_9[0] };
	kbndatainfo[12] = { _T("CVB00135"), DATAKIND_SIGN, (char*)&KSURI2_9_SIGN[0], sizeof(int), &idx_ksuri2_dummy[0] };

	datainfo[0] = { _T("CVB00150"), DATAKIND_VAL, KSURI2_8_KEI, MONY_BUF_SIZE, &idx_ksuri2_8_kei[0] };
	datainfo[1] = { _T("CVB00160"), DATAKIND_VAL, KSURI2_9_KEI, MONY_BUF_SIZE, &idx_ksuri2_9_kei[0] };
	datainfo[2] = { _T("CVC00010"), DATAKIND_VAL, KSURI2_10, MONY_BUF_SIZE, &idx_ksuri2_10[0] };
	datainfo[3] = { _T("CVC00020"), DATAKIND_VAL, KSURI2_11, MONY_BUF_SIZE, &idx_ksuri2_11[0] };
	datainfo[4] = { _T("CVD00010"), DATAKIND_VAL, KSURI2_12, MONY_BUF_SIZE, &idx_ksuri2_12[0] };
	datainfo[5] = { _T("CVD00020"), DATAKIND_VAL, KSURI2_13, MONY_BUF_SIZE, &idx_ksuri2_13[0] };
}

void CShinKsisan_Uri2::SetSyukeiOnRendo()
{

	if (m_TokureiMode != TOKUREIMODE_RENDO) {
		return;
	}

	if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
		//入力ロック中.
		return;
	}

	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	// 転記後に列番号１にフォーカスをセットするので、0を入れておく
	m_dispCol_left = 0;

	SIMKSIREWARIZREC Rec = {0};
	//第5-(2)号様式では特定課税仕入は課税売上割合が95%以上であっても記載.
	//m_pSimKuriwari->SyukeiKsirewari(&Rec, (*m_pSnHeadData)->IsNeedTkkzDisp(), m_tekiyotaisyokikan_s, m_tekiyotaisyokikan_e);
	m_pSimKuriwari->SyukeiKsirewari(&Rec, 1, m_tekiyotaisyokikan_s, m_tekiyotaisyokikan_e);

	SIMKURIWARIZREC Rec2 = {0};
	m_pSimKuriwari->SyukeiKuriwari10Eigyobi(&Rec2, m_tekiyotaisyokikan_s, m_tekiyotaisyokikan_e);

	SetSyukeiData(&Rec, &Rec2);

	DispClmNum();
	m_Ksisan_Uri2Diag02.SetPosition(ID_KSURI2_TITLE1);
	m_bar.SetScrollPos(0);
}

int CShinKsisan_Uri2::SetSyukeiData(SIMKSIREWARIZREC* pRec, SIMKURIWARIZREC* pRec2)
{

	ClearData();

	char val[MONY_BUF_SIZE] = { 0 } , val2[MONY_BUF_SIZE] = { 0 };
	char val100[MONY_BUF_SIZE] = { 0 }, val108[MONY_BUF_SIZE] = { 0 }, val110[MONY_BUF_SIZE] = { 0 };
	CString str100 = _T("100");
	CString str108 = _T("108");
	CString str110 = _T("110");

	m_pArith->l_input(&val100, str100.GetBuffer());
	m_pArith->l_input(&val108, str108.GetBuffer());
	m_pArith->l_input(&val110, str110.GetBuffer());

	memmove(KSURI2_1[0], pRec->sire, MONY_BUF_SIZE);

	memset(&val, 0, sizeof(val));
	m_pArith->l_add(&val, &val, pRec->tksire8);
	m_pArith->l_add(&val, &val, pRec->tksire10);
	m_pArith->l_add(KSURI2_2[0], KSURI2_2[0], &val);

	memmove(KSURI2_3[0], pRec->kamotsusire, MONY_BUF_SIZE);
	m_pArith->l_add(KSURI2_4[0], KSURI2_4[0], KSURI2_1[0]);
	m_pArith->l_add(KSURI2_4[0], KSURI2_4[0], KSURI2_2[0]);
	m_pArith->l_add(KSURI2_4[0], KSURI2_4[0], KSURI2_3[0]);
	memmove(KSURI2_5[0], pRec->keigensire, MONY_BUF_SIZE);
	m_pArith->l_par100((unsigned char*)KSURI2_6[0], (unsigned char*)KSURI2_5[0], (unsigned char*)KSURI2_4[0], 0, 0);

	//連動時、□50%適用チェック状態を取得.
	if (m_TokureiMode == TOKUREIMODE_RENDO) {
		CString filter;
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d and itmId = 'CVB00105' and idx = %d"),
			m_SnSeq, ICS_SN_TOKUREI_2_HYOID, TOKUREI_TBL_VERSION, 0);

		ReadData(m_SnSeq, TOKUREI_TBL_VERSION, filter);
	}

	memmove(KSURI2_7[0], pRec->uriagegoukei, MONY_BUF_SIZE);

	l_pardotnet(&val[0], KSURI2_7[0], KSURI2_5[0], KSURI2_4[0]);
	m_pArith->l_mul(&val, &val, val100);
	m_pArith->l_div(&val, &val, val108); 
	memmove(KSURI2_8[0], &val, MONY_BUF_SIZE);

	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
		m_pArith->l_add(&KSURI2_8_KEI, &KSURI2_8_KEI, KSURI2_8[idx_jigyokbn]);
	}

	memset(&val, 0, sizeof(val));
	l_pardotnet(&val[0], KSURI2_7[0], KSURI2_5[0], KSURI2_4[0]);

	m_pArith->l_sub(&val, KSURI2_7[0], val);
	m_pArith->l_mul(&val, &val, val100);
	m_pArith->l_div(&val, &val, val110);
	memmove(KSURI2_9[0], &val, MONY_BUF_SIZE);

	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
		m_pArith->l_add(&KSURI2_9_KEI, &KSURI2_9_KEI, KSURI2_9[idx_jigyokbn]);
	}

	m_pArith->l_add(&KSURI2_12, &KSURI2_8_KEI, KSURI2_10);
	m_pArith->l_add(&KSURI2_13, &KSURI2_9_KEI, KSURI2_11);

	m_pArith->l_add(HENKAN_GOKEI[0], (char*)pRec2->hanbaisyoreikinhiyo[0], (char*)pRec2->uriagewaribiki[0]);

	memset(&val, 0, sizeof(val));
	l_pardotnet(&val[0], HENKAN_GOKEI[0], KSURI2_5[0], KSURI2_4[0]);
//	l_pardotnet(&val[0], HENKAN_GOKEI[0], (char*)Rec.keigenuriage[0], (char*)Rec.uriage[0]);

	memset(&val2, 0, sizeof(val2));
	m_pArith->l_sub(&val2, HENKAN_GOKEI[0], &val);

	//売上変換（付表１用　税抜き計算処理いらない）
	memmove(KSURI_NEBIKI_KEI8_FHYO1[0], &val, MONY_BUF_SIZE);
	memmove(KSURI_NEBIKI_10_FHYO1[0], &val2, MONY_BUF_SIZE);

	m_pArith->l_mul(&val, &val, val100);
	m_pArith->l_div(&val, &val, val108);

	m_pArith->l_mul(&val2, &val2, val100);
	m_pArith->l_div(&val2, &val2, val110);


	memmove(KSURI_NEBIKI_KEI8[0], &val, MONY_BUF_SIZE);
	memmove(KSURI_NEBIKI_10[0], &val2, MONY_BUF_SIZE);

	//貸倒損失
	memmove(KASISON_GOKEI[0], (char*)pRec2->kashidaoreson[0], MONY_BUF_SIZE);

	memset(&val, 0, sizeof(val));
	l_pardotnet(&val[0], KASISON_GOKEI[0], KSURI2_5[0], KSURI2_4[0]);

	memset(&val2, 0, sizeof(val2));
	m_pArith->l_sub(&val2, KASISON_GOKEI[0], &val);


	memmove(KSURI_KASISON_KEI8[0], &val, MONY_BUF_SIZE);
	memmove(KSURI_KASISON_10[0], &val2, MONY_BUF_SIZE);

	//貸倒回収
	memmove(KASIKAISYU_GOKEI[0], (char*)pRec2->kashidaorekaisyu[0], MONY_BUF_SIZE);

	memset(&val, 0, sizeof(val));
	l_pardotnet(&val[0], KASIKAISYU_GOKEI[0], KSURI2_5[0], KSURI2_4[0]);

	memset(&val2, 0, sizeof(val2));
	m_pArith->l_sub(&val2, KASIKAISYU_GOKEI[0], &val);

	memmove(KSURI_KASIKAISYU_KEI8[0], &val, MONY_BUF_SIZE);
	memmove(KSURI_KASIKAISYU_10[0], &val2, MONY_BUF_SIZE);
	//<-----------------


	// 連動時(編集＝チェックオフ)、事業区分名をテーブルから再取得
	if( m_TokureiMode == TOKUREIMODE_RENDO ){
		CString filter;
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d and itmId = 'CVB00030'"), m_SnSeq, ICS_SN_TOKUREI_2_HYOID, TOKUREI_TBL_VERSION);
		ReadData(m_SnSeq, TOKUREI_TBL_VERSION, filter );
	}

	//連動時、□50%適用チェックONの場合、連動項目変更.
	if (m_TokureiMode == TOKUREIMODE_RENDO) {
		if (KSURI2_6_CHECK[0]) {
			CheckSub_KSURI2_6CheckOn(idx_ksuri2_6_check[0], 0);
		}
	}

	Set_KSURI2_data();

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinKsisan_Uri2::InitDiagAttr()
{
	int	cnt = 0;
	while (1) {
		if (Ksisan_Uri2[cnt].Index == 0) {
			break;
		}

		int	Flg;
		// 確定時は全入力不可
		if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) { //入力ロック
			Flg = Ksisan_Uri2[cnt].AllUnConnect;
		}
		else {
			if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
				Flg = Ksisan_Uri2[cnt].UnConnect;
			}
			else {
				if( m_TokureiMode ==TOKUREIMODE_HANRENDO ) {
					Flg = Ksisan_Uri2[cnt].HalfConnect;
				}
				else {
					Flg = Ksisan_Uri2[cnt].Connect;
				}
			}
		}
		ATRIB_MOD(IDC_ICSDIAG8CTRL2, Ksisan_Uri2[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Ksisan_Uri2[cnt].Connect);
		cnt++;
	}

	m_Ksisan_Uri2Diag02.EnableDelete(0);//DELETEキー１回で削除モード設定

	return 0;
}

//-----------------------------------------------------------------------------
// 特例テーブルバージョンチェック.
//-----------------------------------------------------------------------------
// 引数     sn_seq:履歴シーケンス
// 返送値	0：正常終了 -1：エラー
//-----------------------------------------------------------------------------
int CShinKsisan_Uri2::TokureiTblVersionCheck(int sn_seq)
{
	CString filter;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TOKUREI_2_HYOID, TOKUREI_TBL_OLD_VERSION);

	ASSERT(m_pZmSub);
	if (m_pZmSub == NULL) {
		return -1;
	}

	if (m_pDBSyzAccess->m_pSyz_Tokurei) {
		m_pDBSyzAccess->m_pSyz_Tokurei->Requery(filter);
	}
	else {
		if (m_pDBSyzAccess->Syz_TokureiOpen(m_pZmSub->m_database, filter)) {
			return -1;
		}
	}

	CSYZ_TOKUREI* pSyzTokurei = m_pDBSyzAccess->m_pSyz_Tokurei;
	if ((pSyzTokurei->MoveFirst() == -1) || (pSyzTokurei->st == -1)) {
		;
	}
	else {
		CString sql;
		sql.Format(_T("DELETE syz_tokurei WHERE version = %d and hyoId = \'%s\'"), TOKUREI_TBL_VERSION, ICS_SN_TOKUREI_2_HYOID);
		m_pZmSub->m_database->ExecuteSQL(sql);
		sql.Empty();
		sql.Format(_T("UPDATE syz_tokurei SET version = %d WHERE sn_seq = %d and hyoId = \'%s\'"), TOKUREI_TBL_VERSION, sn_seq, ICS_SN_TOKUREI_2_HYOID);
		m_pZmSub->m_database->ExecuteSQL(sql);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// データの読込み
//-----------------------------------------------------------------------------
// 引数     sn_seq:履歴シーケンス version:バージョン
// 返送値	0：正常終了 -1：エラー
//-----------------------------------------------------------------------------
int CShinKsisan_Uri2::ReadData(int sn_seq, int version, CString filter)
{

	ASSERT(m_pZmSub);
	if (m_pZmSub == NULL) {
		return -1;
	}

	if (m_pDBSyzAccess->m_pSyz_Tokurei) {
		m_pDBSyzAccess->m_pSyz_Tokurei->Requery(filter);
	}
	else {
		if (m_pDBSyzAccess->Syz_TokureiOpen(m_pZmSub->m_database, filter)) {
			return -1;
		}
	}

	CSYZ_TOKUREI* pSyzTokurei = m_pDBSyzAccess->m_pSyz_Tokurei;
	if ((pSyzTokurei->MoveFirst() == -1) || (pSyzTokurei->st == -1)) {
		return 0;
	}
	else {

		for (int i = 0;; i++) {

			if (i == 0) {
				if ((pSyzTokurei->MoveFirst() == -1) || pSyzTokurei->st == -1) {
					break;
				}
			}
			else {
				if ((pSyzTokurei->MoveNext() == -1) || pSyzTokurei->st == -1) {
					break;
				}
			}

			if (ReadSub_data(pSyzTokurei)) {
				continue;
			}
		}
	}

	return 0;
}

int CShinKsisan_Uri2::ReadSub_data(CSYZ_TOKUREI* pTokurei)
{
	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {

			if ((pTokurei->itemId == kbndatainfo[tblidx].itemId) && (pTokurei->idx == idx_jigyokbn)) {

				if (pTokurei->kind == DATAKIND_STR) {
					// 連動は入力不可の事業区分はReadしない
					if ((m_TokureiMode == TOKUREIMODE_RENDO) && (idx_jigyokbn != 0))
						continue;

					memmove(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn,
						pTokurei->str, kbndatainfo[tblidx].size);
				}
				else if (pTokurei->kind == DATAKIND_VAL) {
					m_SyzUtil.val_to_bin(
						(unsigned char*)(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn),
						pTokurei->val);

					if ((pTokurei->itemId.Compare(_T("CVB00050")) == 0)) {
						m_SyzUtil.val_to_bin(
							(unsigned char*)(KSURI2_1_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn),
							pTokurei->val);
					}

					if ((pTokurei->itemId.Compare(_T("CVB00060")) == 0)) {
						m_SyzUtil.val_to_bin(
							(unsigned char*)(KSURI2_2_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn),
							pTokurei->val);
					}

					if ((pTokurei->itemId.Compare(_T("CVB00070")) == 0)) {
						m_SyzUtil.val_to_bin(
							(unsigned char*)(KSURI2_3_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn),
							pTokurei->val);
					}

					if ((pTokurei->itemId.Compare(_T("CVB00080")) == 0)) {
						m_SyzUtil.val_to_bin(
							(unsigned char*)(KSURI2_4_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn),
							pTokurei->val);
					}

					if ((pTokurei->itemId.Compare(_T("CVB00090")) == 0)) {
						m_SyzUtil.val_to_bin(
							(unsigned char*)(KSURI2_5_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn),
							pTokurei->val);
					}

				}
				else if (pTokurei->kind == DATAKIND_RATE) {
					CString ratestr;
					ratestr.Format(_T("%d"), pTokurei->rate);
					m_SyzUtil.val_to_bin(
						(unsigned char*)(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn),
						ratestr);
				}
				else if ( (pTokurei->kind == DATAKIND_CHECK) 
					|| (pTokurei->kind == DATAKIND_SIGN) ) {
					*(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn) = pTokurei->sign1;
				}
				return 1;
			}

		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {
		if (pTokurei->itemId == datainfo[tblidx].itemId) {

			if (pTokurei->kind == DATAKIND_STR) {
				memmove(datainfo[tblidx].data, pTokurei->str, datainfo[tblidx].size);
			}
			else if (pTokurei->kind == DATAKIND_VAL) {
				m_SyzUtil.val_to_bin((unsigned char*)datainfo[tblidx].data, pTokurei->val);
			}
			else if (pTokurei->kind == DATAKIND_RATE) {
				CString ratestr;
				ratestr.Format(_T("%d"), pTokurei->rate);
				m_SyzUtil.val_to_bin((unsigned char*)datainfo[tblidx].data, ratestr);
			}

			return 1;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// データの書込み
//-----------------------------------------------------------------------------
// 引数     sn_seq:履歴シーケンス version:バージョン
// 返送値	0：正常終了 -1：エラー
//-----------------------------------------------------------------------------
int CShinKsisan_Uri2::WriteData(int sn_seq, int version)
{
	ASSERT(m_pZmSub);
	if (m_pZmSub == NULL) {
		return -1;
	}

	CString filter;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TOKUREI_2_HYOID, version);

	if (m_pDBSyzAccess->m_pSyz_Tokurei) {
		m_pDBSyzAccess->m_pSyz_Tokurei->Requery(filter);
	}
	else {
		if (m_pDBSyzAccess->Syz_TokureiOpen(m_pZmSub->m_database, filter)) {
			return -1;
		}
	}

	CSYZ_TOKUREI* pSyzTokurei = m_pDBSyzAccess->m_pSyz_Tokurei;
	if ((pSyzTokurei->MoveFirst() == -1) || (pSyzTokurei->st == -1)) {

		//データ無し
		WriteSub_data(pSyzTokurei, sn_seq, version);
	}
	else {

		//上書き.
		for (int i = 0;; i++) {

			if (i == 0) {
				if ((pSyzTokurei->MoveFirst() == -1) || pSyzTokurei->st == -1) {
					break;
				}
			}
			else {
				if ((pSyzTokurei->MoveNext() == -1) || pSyzTokurei->st == -1) {
					break;
				}
			}

			if( Update_data(pSyzTokurei) ) {
				continue;
			}

		}

	}

	return 0;
}

void CShinKsisan_Uri2::WriteSub_data(CSYZ_TOKUREI* pTokurei, int sn_seq, int version)
{
	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {

			pTokurei->AddNew();
			pTokurei->sn_seq = sn_seq;
			pTokurei->version = version;
			pTokurei->hyoId = ICS_SN_TOKUREI_2_HYOID;
			pTokurei->itemId = kbndatainfo[tblidx].itemId;
			pTokurei->idx = idx_jigyokbn;
			pTokurei->kind = kbndatainfo[tblidx].kind;
			pTokurei->sign1 = 0;

			if (pTokurei->kind == DATAKIND_STR) {
				pTokurei->str = kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn;
			}
			else if (pTokurei->kind == DATAKIND_VAL) {
				if ((pTokurei->itemId.Compare(_T("CVB00050")) == 0) && KSURI2_6_CHECK[idx_jigyokbn]) {
					m_SyzUtil.val_to_asci(&pTokurei->val,
						(unsigned char*)(KSURI2_1_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn));
				}
				else if((pTokurei->itemId.Compare(_T("CVB00060")) == 0) && KSURI2_6_CHECK[idx_jigyokbn]) {
					m_SyzUtil.val_to_asci(&pTokurei->val,
						(unsigned char*)(KSURI2_2_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn));
				}
				else if ((pTokurei->itemId.Compare(_T("CVB00070")) == 0) && KSURI2_6_CHECK[idx_jigyokbn]) {
					m_SyzUtil.val_to_asci(&pTokurei->val,
						(unsigned char*)(KSURI2_3_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn));
				}
				else if ((pTokurei->itemId.Compare(_T("CVB00080")) == 0) && KSURI2_6_CHECK[idx_jigyokbn]) {
					m_SyzUtil.val_to_asci(&pTokurei->val,
						(unsigned char*)(KSURI2_4_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn));
				}
				else if ((pTokurei->itemId.Compare(_T("CVB00090")) == 0) && KSURI2_6_CHECK[idx_jigyokbn]) {
					m_SyzUtil.val_to_asci(&pTokurei->val,
						(unsigned char*)(KSURI2_5_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn));
				}
				else {
					m_SyzUtil.val_to_asci(&pTokurei->val,
						(unsigned char*)(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn));
				}
			}
			else if (pTokurei->kind == DATAKIND_RATE) {
				CString ratestr;
				m_SyzUtil.val_to_asci(&ratestr,
					(unsigned char*)(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn));
				pTokurei->rate = _ttoi(ratestr);

			}
			else if ((pTokurei->kind == DATAKIND_CHECK) 
				|| (pTokurei->kind == DATAKIND_SIGN)) {
				pTokurei->sign1 = *(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn);
			}

			pTokurei->Update();
		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {

		pTokurei->AddNew();
		pTokurei->sn_seq = sn_seq;
		pTokurei->version = version;
		pTokurei->hyoId = ICS_SN_TOKUREI_2_HYOID;
		pTokurei->itemId = datainfo[tblidx].itemId;
		pTokurei->idx = 0;
		pTokurei->kind = datainfo[tblidx].kind;

		if (pTokurei->kind == DATAKIND_VAL) {
			m_SyzUtil.val_to_asci(&pTokurei->val, (unsigned char*)(datainfo[tblidx].data));
		}

		pTokurei->sign1 = 0;
		pTokurei->Update();

	}
}

int CShinKsisan_Uri2::Update_data(CSYZ_TOKUREI* pTokurei) 
{
	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {

			if ((pTokurei->itemId == kbndatainfo[tblidx].itemId) && (pTokurei->idx == idx_jigyokbn)) {
				pTokurei->Edit();

				if (pTokurei->kind == DATAKIND_STR) {
					pTokurei->str = kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn;
				}
				else if (pTokurei->kind == DATAKIND_VAL) {

					if ((pTokurei->itemId.Compare(_T("CVB00050")) == 0) && KSURI2_6_CHECK[idx_jigyokbn]) {
						m_SyzUtil.val_to_asci(&pTokurei->val,
							(unsigned char*)(KSURI2_1_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn));
					}
					else if ((pTokurei->itemId.Compare(_T("CVB00060")) == 0) && KSURI2_6_CHECK[idx_jigyokbn]) {
						m_SyzUtil.val_to_asci(&pTokurei->val,
							(unsigned char*)(KSURI2_2_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn));
					}
					else if ((pTokurei->itemId.Compare(_T("CVB00070")) == 0) && KSURI2_6_CHECK[idx_jigyokbn]) {
						m_SyzUtil.val_to_asci(&pTokurei->val,
							(unsigned char*)(KSURI2_3_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn));
					}
					else if ((pTokurei->itemId.Compare(_T("CVB00080")) == 0) && KSURI2_6_CHECK[idx_jigyokbn]) {
						m_SyzUtil.val_to_asci(&pTokurei->val,
							(unsigned char*)(KSURI2_4_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn));
					}
					else if ((pTokurei->itemId.Compare(_T("CVB00090")) == 0) && KSURI2_6_CHECK[idx_jigyokbn]) {
						m_SyzUtil.val_to_asci(&pTokurei->val,
							(unsigned char*)(KSURI2_5_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn));
					}
					else {
						m_SyzUtil.val_to_asci(&pTokurei->val,
							(unsigned char*)(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn));
					}
				}
				else if (pTokurei->kind == DATAKIND_RATE) {
					CString ratestr;
					m_SyzUtil.val_to_asci(&ratestr,
						(unsigned char*)(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn));
					pTokurei->rate = _ttoi(ratestr);
				}
				else if ( (pTokurei->kind == DATAKIND_CHECK) 
					|| (pTokurei->kind == DATAKIND_SIGN)) {
					pTokurei->sign1 = *(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn);
				}
				pTokurei->Update();
				return 1;
			}
		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {
		if (pTokurei->itemId == datainfo[tblidx].itemId) {
			pTokurei->Edit();
			if (pTokurei->kind == DATAKIND_VAL) {
				m_SyzUtil.val_to_asci(&pTokurei->val, (unsigned char*)(datainfo[tblidx].data));
			}
			pTokurei->Update();
			return 1;
		}
	}

	return 0;
}

//印刷最大ページ数判定.
int CShinKsisan_Uri2::GetMxPrtPage()
{
	int PageNum = 1;

	if (m_pDBSyzAccess->m_pSyz_Tokurei == NULL) {
		return PageNum;
	}

	CString sq, subsq;
	CSYZ_TOKUREI* pSyzTokurei = m_pDBSyzAccess->m_pSyz_Tokurei;

	for (int page = 2; page <= KSISANURI2_PRT_PAGE_MAX; page++) {

		sq.Empty();
		subsq.Empty();

		subsq.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d "), 0, ICS_SN_TOKUREI_2_HYOID, TOKUREI_TBL_VERSION);
		sq += subsq;
		subsq.Format(_T("and %d <= idx and idx <= %d "), (page - 1)*JIGYO_PER_PAGE_URI2, page*JIGYO_PER_PAGE_URI2 - 1);
		sq += subsq;
		subsq.Format(_T("and ((val > 0) or (rate IS NOT NULL and rate != 0) or (str IS NOT NULL and str <> '' and str != '0')) "));
		sq += subsq;

		pSyzTokurei->Requery(sq);

		if ((pSyzTokurei->MoveFirst() != -1) && (pSyzTokurei->st != -1)) {
			PageNum = page;
		}

	}

	return PageNum;
}

//-----------------------------------------------------------------------------
//  画面出力
//-----------------------------------------------------------------------------
void CShinKsisan_Uri2::DispKsisan_Uri2()
{
	// 画面初期化
	for ( int idx = KSISANURI2_DIAGCTRLID_NAME; idx <= KSISANURI2_DIAGCTRLID_TEKIYO_E; (short)idx++ ) {
		dsp_cls(IDC_ICSDIAG8CTRL2, idx);
	}
	for (int idx = ID_KSURI2_TITLE1; idx <= ID_KSURI2_13; idx++ ) {
		dsp_cls(IDC_ICSDIAG8CTRL2, (short)idx);

	}

	DispClmNum();
	SetKazeikikan();
	SetTekiyoTaisyokikan();
	SetName();
	Set_KSURI2_data();
}


//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//		Mony	：	表示金額
//-----------------------------------------------------------------------------
void CShinKsisan_Uri2::dsp_prs(unsigned short Id, short Index, char *Mony)
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	memmove(DiagData.data_val, Mony, MONY_BUF_SIZE);
	diag_setdata(Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
}

//-----------------------------------------------------------------------------
// 各項目表示クリア
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//-----------------------------------------------------------------------------
void CShinKsisan_Uri2::dsp_cls(unsigned short Id, short Index)
{

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	switch (Index) {
		case ID_KSURI2_TITLE1:
		case ID_KSURI2_TITLE2:
		case ID_KSURI2_6_1:
		case ID_KSURI2_6_2:
			diag_clear(Id, Index, TRUE, CIcsdiagctrl);
			break;
		case ID_KSURI2_6CHECK_1:
		case ID_KSURI2_6CHECK_2:
			DiagData.data_check = 0;
			diag_setdata(Id, Index, (struct IUnknown*)&DiagData, CIcsdiagctrl);
			break;
		default:
			memset(DiagData.data_val, '\0', MONY_BUF_SIZE);
			diag_setdata(Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
			break;
	}
}

//-----------------------------------------------------------------------------
// 各入力データ抽出
//-----------------------------------------------------------------------------
void CShinKsisan_Uri2::GetAllDiagData()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	for (int tblidx = 0; tblidx < (sizeof(kbndatainfo) / sizeof(kbndatainfo[0])); tblidx++) {
		for (int clmidx = 0; clmidx < JIGYO_PER_PAGE_URI2; clmidx++) {

			diag_getdata(IDC_ICSDIAG8CTRL2, kbndatainfo[tblidx].diagidx[clmidx],
				(struct IUnknown*)&DiagData, CIcsdiagctrl);

			if (kbndatainfo[tblidx].kind == DATAKIND_VAL) {
				memmove(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size,
					DiagData.data_val, kbndatainfo[tblidx].size);
			}
			else if (kbndatainfo[tblidx].kind == DATAKIND_STR) {

				memmove(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size,
					DiagData.data_edit.GetBuffer(), kbndatainfo[tblidx].size);

			}
			else if (kbndatainfo[tblidx].kind == DATAKIND_RATE) {
				m_SyzUtil.val_to_bin((unsigned char*)kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size, DiagData.data_edit);
			}
			else if ( (kbndatainfo[tblidx].kind == DATAKIND_CHECK) 
				|| (kbndatainfo[tblidx].kind == DATAKIND_SIGN)) {
				*(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size) = (char)DiagData.data_check;
			}
		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {
		diag_getdata(IDC_ICSDIAG8CTRL2, datainfo[tblidx].diagidx[0], (struct IUnknown*)&DiagData, CIcsdiagctrl);
		if (datainfo[tblidx].kind == DATAKIND_VAL) {
			memmove(datainfo[tblidx].data, DiagData.data_val, MONY_BUF_SIZE);
		}
	}

}

//-----------------------------------------------------------------------------
// 課税期間
//-----------------------------------------------------------------------------
void CShinKsisan_Uri2::SetKazeikikan()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char yy, mm, dd;
	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	DiagData.data_disp.Format(_T("%02x.%02x.%02x"), (yy & 0xff), (mm & 0xff), (dd & 0xff));
	diag_setdata(IDC_ICSDIAG8CTRL2, KSISANURI2_DIAGCTRLID_KAZEI_S, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd);
	DiagData.data_disp.Format(_T("%02x.%02x.%02x"), (yy & 0xff), (mm & 0xff), (dd & 0xff));
	diag_setdata(IDC_ICSDIAG8CTRL2, KSISANURI2_DIAGCTRLID_KAZEI_E, (struct IUnknown *)&DiagData, CIcsdiagctrl);
}

//-----------------------------------------------------------------------------
// 適用対象期間
//-----------------------------------------------------------------------------
void CShinKsisan_Uri2::SetTekiyoTaisyokikan()
{

	CVolDateDB voldate;
	int s_gengo = 0, d_gengo = 0;
	int year = 0, tekiyotaisyokikan_s = 0, tekiyotaisyokikan_e = 0;

	CString yystr, mmstr, ddstr;

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	CString tmpstr = _T("");
	if ((TOKUREI_KIKAN_S <= m_tekiyotaisyokikan_s) && (m_tekiyotaisyokikan_s <= TOKUREI_KIKAN_E)) {
		voldate.db_datecnvGen(s_gengo, m_tekiyotaisyokikan_s, &d_gengo, &tekiyotaisyokikan_s, 0, 0);
	}
	else if ((m_tekiyotaisyokikan_s <= TOKUREI_KIKAN_S) && (TOKUREI_KIKAN_S <= m_tekiyotaisyokikan_e)) {
		voldate.db_datecnvGen(s_gengo, TOKUREI_KIKAN_S, &d_gengo, &tekiyotaisyokikan_s, 0, 0);
	}
	tmpstr.Format(_T("%08d"), tekiyotaisyokikan_s);
	DiagData.data_disp.Format(_T("%02s.%02s.%02s"), tmpstr.Mid(2, 2), tmpstr.Mid(4, 2), tmpstr.Mid(6, 2));
	diag_setdata(IDC_ICSDIAG8CTRL2, KSISANURI2_DIAGCTRLID_TEKIYO_S, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	if ((TOKUREI_KIKAN_S <= m_tekiyotaisyokikan_e) && (m_tekiyotaisyokikan_e <= TOKUREI_KIKAN_E)) {
		voldate.db_datecnvGen(s_gengo, m_tekiyotaisyokikan_e, &d_gengo, &tekiyotaisyokikan_e, 0, 0);
	}
	else if ((m_tekiyotaisyokikan_s <= TOKUREI_KIKAN_E) && (TOKUREI_KIKAN_E <= m_tekiyotaisyokikan_e)) {
		voldate.db_datecnvGen(s_gengo, TOKUREI_KIKAN_E, &d_gengo, &tekiyotaisyokikan_e, 0, 0);
	}
	tmpstr.Format(_T("%08d"), tekiyotaisyokikan_e);
	DiagData.data_disp.Format(_T("%02s.%02s.%02s"), tmpstr.Mid(2, 2), tmpstr.Mid(4, 2), tmpstr.Mid(6, 2));
	diag_setdata(IDC_ICSDIAG8CTRL2, KSISANURI2_DIAGCTRLID_TEKIYO_E, (struct IUnknown *)&DiagData, CIcsdiagctrl);

}

void CShinKsisan_Uri2::SetName()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	char	buf[512] = { 0 };

	memset(buf, '\0', sizeof(buf));
	if ((*m_pSnHeadData)->IsSoleProprietor()) {
		memmove(buf, (*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO));
	}
	else {
		memmove(buf, (*m_pSnHeadData)->Sn_CONAM, sizeof((*m_pSnHeadData)->Sn_CONAM));
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata(IDC_ICSDIAG8CTRL2, KSISANURI2_DIAGCTRLID_NAME, (struct IUnknown *)&DiagData, CIcsdiagctrl);
}

void CShinKsisan_Uri2::Set_KSURI2_data()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char buf[64] = { 0 };
	char val[6] = { 0 };

	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int idx = 0; idx < JIGYO_PER_PAGE_URI2; idx++) {

			if (kbndatainfo[tblidx].kind == DATAKIND_STR) {

					DiagData.data_edit.Empty();
					memset(buf, '\0', sizeof(buf));
					memmove(buf, kbndatainfo[tblidx].data + (m_dispCol_left + idx) * kbndatainfo[tblidx].size, kbndatainfo[tblidx].size);
					DiagData.data_edit = buf;
					m_Ksisan_Uri2Diag02.SetData(kbndatainfo[tblidx].diagidx[idx], (struct IUnknown*)&DiagData);
					SetSub_KSURI2_InputLockColor(kbndatainfo[tblidx].diagidx[idx]);

			}
			else if (kbndatainfo[tblidx].kind == DATAKIND_VAL) {
				SetSub_KSURI2_data_kindval(tblidx, idx);
				SetSub_KSURI2_InputLockColor(kbndatainfo[tblidx].diagidx[idx]);
			}
			else if (kbndatainfo[tblidx].kind == DATAKIND_RATE) {

				memset(&val, 0, sizeof(val));
				memmove(val, kbndatainfo[tblidx].data + (m_dispCol_left + idx) * kbndatainfo[tblidx].size, kbndatainfo[tblidx].size);
				m_pArith->l_print(val, val, _T("sss9"));
				DiagData.data_edit = val;
				diag_setdata(IDC_ICSDIAG8CTRL2, kbndatainfo[tblidx].diagidx[idx], (struct IUnknown*)&DiagData, CIcsdiagctrl);
				SetSub_KSURI2_InputLockColor(kbndatainfo[tblidx].diagidx[idx]);

			}
			else if ( kbndatainfo[tblidx].kind == DATAKIND_CHECK ) {

				DiagData.data_check = *(kbndatainfo[tblidx].data + (m_dispCol_left + idx) * kbndatainfo[tblidx].size);
				m_Ksisan_Uri2Diag02.SetData(kbndatainfo[tblidx].diagidx[idx], (struct IUnknown*)&DiagData);
				SetSub_KSURI2_InputLockColor(kbndatainfo[tblidx].diagidx[idx]);

				short Flg;
				if (((*m_pSnHeadData)->Sn_Sign4 & 0x01) || (m_TokureiMode == TOKUREIMODE_RENDO)) {
					Flg = 0;
				}
				else {
					Flg = !DiagData.data_check;
				}

				int idx_1235[4] = { idx_ksuri2_1[idx] , idx_ksuri2_2[idx], idx_ksuri2_3[idx], idx_ksuri2_5[idx] };
				for (int i = 0; i < sizeof(idx_1235) / sizeof(idx_1235[0]); i++) {
					if( idx_1235[i] == idx_ksuri2_2[idx] ) {
						if (!(*m_pSnHeadData)->IsNeedTkkzDisp()) {
							if ( ((*m_pSnHeadData)->Sn_Sign4 & 0x80) //非連動.
								&& !((*m_pSnHeadData)->Sn_Sign4 & 0x01) ){ //入力ロック中でない.
								ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_1235[i], 1, (*m_pSnHeadData)->Sn_Sign4, 0);
							}
                            else {
								ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_1235[i], 0, (*m_pSnHeadData)->Sn_Sign4, 0);
							}
						}
						ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_1235[i], Flg, (*m_pSnHeadData)->Sn_Sign4, 0);
					}
					else {
						ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_1235[i], Flg, (*m_pSnHeadData)->Sn_Sign4, 0);
					}
					SetSub_KSURI2_InputLockColor(idx_1235[i]);
				}
				SetSub_KSURI2_InputLockColor(kbndatainfo[tblidx].diagidx[idx]);
			}

		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {
		dsp_prs(IDC_ICSDIAG8CTRL2, datainfo[tblidx].diagidx[0], datainfo[tblidx].data);
		SetSub_KSURI2_InputLockColor(datainfo[tblidx].diagidx[0]);
	}

	if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
		//入力ロック中.
		return;
	}

	//yoshida190816
	//場所変更
	if (m_SyzUtil.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
		
		if( !((CH31HyoView*)m_pParent)->IsCrossOverR050930() ){	// 20231025 令和5年9月30日をまたぐ場合はセットしない
			// 付表1-1 1-1D欄へ
			(*m_pSyzSyukei)->SetShinkokuData(_T("312202"), KSURI2_12);
			// 付表1-1 1-1E欄へ
			(*m_pSyzSyukei)->SetShinkokuData(_T("312203"), KSURI2_13);
		}

		// 付表2-1にも転記する
		//付表２－１に転記するのは連動のみ。非連動では転記しないようにするyoshida190912
		if (m_TokureiMode == TOKUREIMODE_RENDO) {//連動モード

			//---->yoshida190919
			char KSURI2_12_FHYO2[MONY_BUF_SIZE] = { 0 };//返還金額計算値
			char KSURI2_13_FHYO2[MONY_BUF_SIZE] = { 0 };

			char URIHENKAN_KEI8_FHYO1[MONY_BUF_SIZE] = { 0 };//売上返還転記金額軽減８％
			char URIHENKAN_10_FHYO1[MONY_BUF_SIZE] = { 0 };//売上返還転記金額１０％

			char val624[MONY_BUF_SIZE] = { 0 }, val780[MONY_BUF_SIZE] = { 0 }, val100[MONY_BUF_SIZE] = { 0 }, val108[MONY_BUF_SIZE] = { 0 }, val110[MONY_BUF_SIZE] = { 0 };
			CString str624 = _T("624");
			CString str780 = _T("780");
			CString str100 = _T("100");
			CString str108 = _T("108");
			CString str110 = _T("110");
			m_pArith->l_input(&val624, str624.GetBuffer());
			m_pArith->l_input(&val780, str780.GetBuffer());
			m_pArith->l_input(&val100, str100.GetBuffer());
			m_pArith->l_input(&val108, str108.GetBuffer());
			m_pArith->l_input(&val110, str110.GetBuffer());

			//++++++++売上返還に転記+++++++//
			memmove(URIHENKAN_KEI8_FHYO1, &KSURI_NEBIKI_KEI8_FHYO1[0], MONY_BUF_SIZE);
			memmove(URIHENKAN_10_FHYO1, &KSURI_NEBIKI_10_FHYO1[0], MONY_BUF_SIZE);

			m_pArith->l_mul(&URIHENKAN_KEI8_FHYO1, &URIHENKAN_KEI8_FHYO1, val624);
			m_pArith->l_div(&URIHENKAN_KEI8_FHYO1, &URIHENKAN_KEI8_FHYO1, val100);
			m_pArith->l_div(&URIHENKAN_KEI8_FHYO1, &URIHENKAN_KEI8_FHYO1, val108);

			m_pArith->l_mul(&URIHENKAN_10_FHYO1, &URIHENKAN_10_FHYO1, val780);
			m_pArith->l_div(&URIHENKAN_10_FHYO1, &URIHENKAN_10_FHYO1, val100);
			m_pArith->l_div(&URIHENKAN_10_FHYO1, &URIHENKAN_10_FHYO1, val110);

			(*m_pSyzSyukei)->SetShinkokuData(_T("312802"), URIHENKAN_KEI8_FHYO1);
			(*m_pSyzSyukei)->SetShinkokuData(_T("312803"), URIHENKAN_10_FHYO1);

			//++++++++貸倒損失に転記+++++++//
			char KASISON_KEI8_FHYO1[MONY_BUF_SIZE] = { 0 };//貸倒損失転記金額軽減８％
			char KASISON_10_FHYO1[MONY_BUF_SIZE] = { 0 };//貸倒損失転記金額１０％

			memmove(KASISON_KEI8_FHYO1, &KSURI_KASISON_KEI8[0], MONY_BUF_SIZE);
			memmove(KASISON_10_FHYO1, &KSURI_KASISON_10[0], MONY_BUF_SIZE);

			m_pArith->l_mul(&KASISON_KEI8_FHYO1, &KASISON_KEI8_FHYO1, val624);
			m_pArith->l_div(&KASISON_KEI8_FHYO1, &KASISON_KEI8_FHYO1, val100);
			m_pArith->l_div(&KASISON_KEI8_FHYO1, &KASISON_KEI8_FHYO1, val108);

			m_pArith->l_mul(&KASISON_10_FHYO1, &KASISON_10_FHYO1, val780);
			m_pArith->l_div(&KASISON_10_FHYO1, &KASISON_10_FHYO1, val100);
			m_pArith->l_div(&KASISON_10_FHYO1, &KASISON_10_FHYO1, val110);

			(*m_pSyzSyukei)->SetShinkokuData(_T("313002"), KASISON_KEI8_FHYO1);
			(*m_pSyzSyukei)->SetShinkokuData(_T("313003"), KASISON_10_FHYO1);


			//+++++++貸倒回収に転記++++++++++//
			char KASIKAISYU_KEI8_FHYO2[MONY_BUF_SIZE] = { 0 };//貸倒回収転記金額軽減８％
			char KASIKAISYU_10_FHYO2[MONY_BUF_SIZE] = { 0 };//貸倒回収転記金額１０％

			memmove(KASIKAISYU_KEI8_FHYO2, &KSURI_KASIKAISYU_KEI8[0], MONY_BUF_SIZE);
			memmove(KASIKAISYU_10_FHYO2, &KSURI_KASIKAISYU_10[0], MONY_BUF_SIZE);

			m_pArith->l_mul(&KASIKAISYU_KEI8_FHYO2, &KASIKAISYU_KEI8_FHYO2, val624);
			m_pArith->l_div(&KASIKAISYU_KEI8_FHYO2, &KASIKAISYU_KEI8_FHYO2, val100);
			m_pArith->l_div(&KASIKAISYU_KEI8_FHYO2, &KASIKAISYU_KEI8_FHYO2, val108);

			m_pArith->l_mul(&KASIKAISYU_10_FHYO2, &KASIKAISYU_10_FHYO2, val780);
			m_pArith->l_div(&KASIKAISYU_10_FHYO2, &KASIKAISYU_10_FHYO2, val100);
			m_pArith->l_div(&KASIKAISYU_10_FHYO2, &KASIKAISYU_10_FHYO2, val110);

			(*m_pSyzSyukei)->SetShinkokuData(_T("325002"), KASIKAISYU_KEI8_FHYO2);
			(*m_pSyzSyukei)->SetShinkokuData(_T("325003"), KASIKAISYU_10_FHYO2);


			//課税標準額転記
			m_pArith->l_sub(&KSURI2_12_FHYO2, KSURI2_12, KSURI_NEBIKI_KEI8[0]);
			m_pArith->l_sub(&KSURI2_13_FHYO2, KSURI2_13, KSURI_NEBIKI_10[0]);

			(*m_pSyzSyukei)->SetShinkokuData(_T("322602"), KSURI2_12_FHYO2);
			(*m_pSyzSyukei)->SetShinkokuData(_T("322603"), KSURI2_13_FHYO2);
			(*m_pSyzSyukei)->SetShinkokuData(_T("911104"), KSURI2_12_FHYO2);
			(*m_pSyzSyukei)->SetShinkokuData(_T("911105"), KSURI2_13_FHYO2);
			//------
//			(*m_pSyzSyukei)->SetShinkokuData(_T("322602"), KSURI2_12);
//			(*m_pSyzSyukei)->SetShinkokuData(_T("322603"), KSURI2_13);
			//<------
		}
	}
}

void CShinKsisan_Uri2::SetSub_KSURI2_InputLockColor(short index)
{
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {
		//入力ロック中でなければreturn;
		return;
	}

	DIAGRAM_ATTRIBUTE DA;
	diag_getattr(IDC_ICSDIAG8CTRL2, index, &DA, CIcsdiagctrl);

	switch (m_TokureiMode) {
	case TOKUREIMODE_HIRENDO:
	case TOKUREIMODE_HANRENDO:
		if (ID_KSURI2_4_1 <= index && index <= ID_KSURI2_4_2) {
			DA.attr_bcolor = BC_WHITE;
		}
		else if (ID_KSURI2_6_1 <= index && index <= ID_KSURI2_6_2) {
			DA.attr_bcolor = BC_WHITE;
		}
		else if (index == ID_KSURI2_12 || index == ID_KSURI2_13) {
			DA.attr_bcolor = BC_WHITE;
		}
		else {
			DA.attr_bcolor = BC_GRAY;
		}
		break;
	case TOKUREIMODE_RENDO:
		break;
	default:
		break;
	}

	diag_setattr(IDC_ICSDIAG8CTRL2, index, &DA, FALSE, CIcsdiagctrl);
}

void CShinKsisan_Uri2::SetSub_KSURI2_data_kindval(int tblidx, int clmidx)
{

	if ((kbndatainfo[tblidx].itemId.Compare(_T("CVB00050")) == 0)
		|| (kbndatainfo[tblidx].itemId.Compare(_T("CVB00060")) == 0)
		|| (kbndatainfo[tblidx].itemId.Compare(_T("CVB00070")) == 0)
		|| (kbndatainfo[tblidx].itemId.Compare(_T("CVB00080")) == 0)
		|| (kbndatainfo[tblidx].itemId.Compare(_T("CVB00090")) == 0) ) {

		if (KSURI2_6_CHECK[0 + m_dispCol_left + clmidx]) {
			dsp_cls(IDC_ICSDIAG8CTRL2, kbndatainfo[tblidx].diagidx[clmidx]);
		}
		else {
			dsp_prs(IDC_ICSDIAG8CTRL2,
				kbndatainfo[tblidx].diagidx[clmidx],
				kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size);
		}

	}
	else {

		dsp_prs(IDC_ICSDIAG8CTRL2,
		kbndatainfo[tblidx].diagidx[clmidx],
		kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size);

		if (kbndatainfo[tblidx].itemId.Compare(_T("CVB00120")) == 0) {
			CheckSetData_KSURI2_8(kbndatainfo[tblidx].diagidx[clmidx], clmidx);
		}
		else if (kbndatainfo[tblidx].itemId.Compare(_T("CVB00130")) == 0) {
			CheckSetData_KSURI2_9(kbndatainfo[tblidx].diagidx[clmidx], clmidx);
		}

	}

}

void CShinKsisan_Uri2::Update_KSURI2_2_4_TokuteikazeisireOff()
{
	//特定課税仕入れ有り:OFFの時、KSURI2_2[]のデータクリア,KSURI2_4[]を再計算.
	char val[MONY_BUF_SIZE] = { 0 };
	memset(&val, 0, sizeof(val));
	
	if ( !((*m_pSnHeadData)->m_DispTabSgn & 0x01) && (m_TokureiMode == TOKUREIMODE_RENDO) ) {

		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
            memset(KSURI2_2[idx_jigyokbn], '\0', MONY_BUF_SIZE);
		}

		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
			memset(&val, 0, sizeof(val));
			m_pArith->l_add(&val, &val, KSURI2_1[idx_jigyokbn]);
			m_pArith->l_add(&val, &val, KSURI2_2[idx_jigyokbn]);
			m_pArith->l_add(&val, &val, KSURI2_3[idx_jigyokbn]);
			memmove(KSURI2_4[idx_jigyokbn], &val, MONY_BUF_SIZE);
		}

	}
}

void CShinKsisan_Uri2::Update_KSURI2_4(short index, int clmidx)
{
	char val[MONY_BUF_SIZE] = { 0 };
	memset(&val, 0, sizeof(val));
	m_pArith->l_add(&val, &val, KSURI2_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);
	m_pArith->l_add(&val, &val, KSURI2_2[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);
	m_pArith->l_add(&val, &val, KSURI2_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);
	memmove(KSURI2_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
}

void CShinKsisan_Uri2::Update_KSURI2_6(short index, int clmidx)
{
	//軽減仕入割合を再計算.
	unsigned char keigenwariai[MONY_BUF_SIZE];

	char val_ksuri2_4[MONY_BUF_SIZE], val_ksuri2_5[MONY_BUF_SIZE];

	memmove(val_ksuri2_4, KSURI2_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);
	memmove(val_ksuri2_5, KSURI2_5[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);

	if (m_pArith->l_test(val_ksuri2_4) && m_pArith->l_test(val_ksuri2_5)) {
		m_pArith->l_par100(keigenwariai, (unsigned char*)val_ksuri2_5, (unsigned char*)val_ksuri2_4, 0, 0);
	}
	else {
		memset(keigenwariai, '\0', MONY_BUF_SIZE);
	}

	memmove(KSURI2_6[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, keigenwariai, MONY_BUF_SIZE);
}

void CShinKsisan_Uri2::Update_KSURI2_8(short index, int clmidx, bool bDel)
{
	if( KSURI2_8_SIGN[m_dispCol_left+clmidx] && !bDel) {
		return;
	}

	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSURI2_8(index, clmidx, &val[0]);
	memmove(KSURI2_8[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	CheckSub_ChangeBackColor(idx_ksuri2_8[clmidx], NULL, NULL, true);
}

void CShinKsisan_Uri2::Update_KSURI2_8_KEI(short index, int clmidx)
{
	memset(&KSURI2_8_KEI, 0, sizeof(KSURI2_8_KEI));
	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
		m_pArith->l_add(&KSURI2_8_KEI, &KSURI2_8_KEI, KSURI2_8[idx_jigyokbn]);
	}
}

void CShinKsisan_Uri2::Update_KSURI2_9(short index, int clmidx, bool bDel)
{

	if (KSURI2_9_SIGN[m_dispCol_left + clmidx] && !bDel) {
		return;
	}

	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSURI2_9(index, clmidx, &val[0]);
	memmove(KSURI2_9[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	CheckSub_ChangeBackColor(idx_ksuri2_9[clmidx], NULL, NULL, true);

}

void CShinKsisan_Uri2::Update_KSURI2_9_KEI(short index, int clmidx)
{
	memset(&KSURI2_9_KEI, 0, sizeof(KSURI2_9_KEI));
	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
		m_pArith->l_add(&KSURI2_9_KEI, &KSURI2_9_KEI, KSURI2_9[idx_jigyokbn]);
	}
}

void CShinKsisan_Uri2::Update_KSURI2_12(short index)
{
	char val[MONY_BUF_SIZE] = { 0 };
	memset(&val, 0, sizeof(val));
	m_pArith->l_add(&val, &val, KSURI2_8_KEI);
	m_pArith->l_add(&val, &val, KSURI2_10);
	memmove(KSURI2_12, &val, MONY_BUF_SIZE);

	//yoshida190816
	//場所変更
	//if ( m_SyzUtil.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_GENSOKU ) {
	//	// 付表1-1 1-1D欄へ
	//	( *m_pSyzSyukei )->SetShinkokuData ( _T( "312202" ), KSURI2_12 );
	//}
}

void CShinKsisan_Uri2::Update_KSURI2_13(short index)
{
	char val[MONY_BUF_SIZE] = { 0 };
	memset(&val, 0, sizeof(val));
	m_pArith->l_add(&val, &val, KSURI2_9_KEI);
	m_pArith->l_add(&val, &val, KSURI2_11);
	memmove(KSURI2_13, &val, MONY_BUF_SIZE);

	//yoshida190816
	//場所変更
	//if ( m_SyzUtil.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_GENSOKU ) {
	//	// 付表1-1 1-1E欄へ
	//	( *m_pSyzSyukei )->SetShinkokuData ( _T( "312203" ), KSURI2_13 );
	//}
}

void CShinKsisan_Uri2::Check_KSURI2_6Check(short index, int clmidx)
{

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	m_Ksisan_Uri2Diag02.GetData((int)index, (struct IUnknown *)&DiagData);

	KSURI2_6_CHECK[0 + m_dispCol_left + clmidx] = DiagData.data_check;

	if (DiagData.data_check) {
		//チェックON
		CheckSub_KSURI2_6CheckOn(index, clmidx);
	}
	else {
		//チェックOFF
		CheckSub_KSURI2_6CheckOff(index, clmidx);
	}

	Set_KSURI2_data();

}

void CShinKsisan_Uri2::CheckSub_KSURI2_6CheckOn(short index, int clmidx, bool bDel)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	//一時データ保存.
	memmove(KSURI2_1_TMP[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);
	memmove(KSURI2_2_TMP[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_2[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);
	memmove(KSURI2_3_TMP[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);
	memmove(KSURI2_4_TMP[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);
	memmove(KSURI2_5_TMP[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_5[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);

	//1.2.3.5入力禁止
	int idx_1235[4] = { idx_ksuri2_1[clmidx] , idx_ksuri2_2[clmidx], idx_ksuri2_3[clmidx],  idx_ksuri2_5[clmidx] };
	for (int i = 0; i < sizeof(idx_1235) / sizeof(idx_1235[0]); i++) {
		ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_1235[i], 0, (*m_pSnHeadData)->Sn_Sign4, 0);
	}
	char val[MONY_BUF_SIZE] = { 0 }, val2[MONY_BUF_SIZE] = { 0 }, val50[MONY_BUF_SIZE] = { 0 }, val100[MONY_BUF_SIZE] = { 0 }, val108[MONY_BUF_SIZE] = { 0 }, val110[MONY_BUF_SIZE] = { 0 };
	CString str50 = _T("50");
	CString str100 = _T("100");
	CString str108 = _T("108");
	CString str110 = _T("110");

	m_pArith->l_input(&val50, str50.GetBuffer());
	m_pArith->l_input(&val100, str100.GetBuffer());
	m_pArith->l_input(&val108, str108.GetBuffer());
	m_pArith->l_input(&val110, str110.GetBuffer());

	memmove(KSURI2_6[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, val50, MONY_BUF_SIZE);

	//8.9再集計
    if( bDel && (index == idx_ksuri2_8[clmidx]) ){
        Update_KSURI2_8(index, clmidx, true);
	}
    else if( bDel && (index == idx_ksuri2_9[clmidx]) ) {
        ;
	}
    else {
		diag_getdata(IDC_ICSDIAG8CTRL2, idx_ksuri2_7[clmidx], (struct IUnknown*)&DiagData, CIcsdiagctrl);
		memmove(val, DiagData.data_val, MONY_BUF_SIZE);
		l_pardotnet(val, val, val50, val100);
		m_pArith->l_mul(&val, &val, val100);
		m_pArith->l_div(&val, &val, val108);
		memmove(KSURI2_8[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, val, MONY_BUF_SIZE);
		Update_KSURI2_8(index, clmidx, true);
	}
	Update_KSURI2_8_KEI(index, clmidx);

	if (bDel && (index == idx_ksuri2_9[clmidx])) {
		Update_KSURI2_9(index, clmidx, true);
	}
	else if (bDel && (index == idx_ksuri2_8[clmidx])) {
		;
	}
	else {
		memset(&val, 0, sizeof(val));
		memmove(&val, DiagData.data_val, MONY_BUF_SIZE);
		m_pArith->l_mul(&val, &val, val50);
		m_pArith->l_div(&val, &val, val100);
		m_pArith->l_sub(&val, DiagData.data_val, &val);
		m_pArith->l_mul(&val, &val, val100);
		m_pArith->l_div(&val, &val, val110);
		memmove(KSURI2_9[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, val, MONY_BUF_SIZE);
	}
	Update_KSURI2_9_KEI(index, clmidx);

	Update_KSURI2_12(index);
	Update_KSURI2_13(index);

	//---->yoshida190919
	//返還金額等再集計
	memset(&val, 0, sizeof(val));

	m_pArith->l_mul(&val, HENKAN_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, val50);
	m_pArith->l_div(&val, &val, val100);


	memset(&val2, 0, sizeof(val2));
	m_pArith->l_sub(&val2, HENKAN_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val);

	//売上変換（付表１用　税抜き計算処理いらない）
	memmove(KSURI_NEBIKI_KEI8_FHYO1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	memmove(KSURI_NEBIKI_10_FHYO1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val2, MONY_BUF_SIZE);


	m_pArith->l_mul(&val, &val, val100);
	m_pArith->l_div(&val, &val, val108);

	m_pArith->l_mul(&val2, &val2, val100);
	m_pArith->l_div(&val2, &val2, val110);


	memmove(KSURI_NEBIKI_KEI8[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	memmove(KSURI_NEBIKI_10[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val2, MONY_BUF_SIZE);

	//貸倒損失
	memset(&val, 0, sizeof(val));
	m_pArith->l_mul(&val, KASISON_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, val50);
	m_pArith->l_div(&val, &val, val100);

	memset(&val2, 0, sizeof(val2));
	m_pArith->l_sub(&val2, KASISON_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val);

	memmove(KSURI_KASISON_KEI8[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	memmove(KSURI_KASISON_10[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val2, MONY_BUF_SIZE);


	//貸倒回収
	memset(&val, 0, sizeof(val));
	m_pArith->l_mul(&val, KASIKAISYU_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, val50);
	m_pArith->l_div(&val, &val, val100);

	memset(&val2, 0, sizeof(val2));
	m_pArith->l_sub(&val2, KASIKAISYU_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val);

	memmove(KSURI_KASIKAISYU_KEI8[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	memmove(KSURI_KASIKAISYU_10[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val2, MONY_BUF_SIZE);
	//<---


}

void CShinKsisan_Uri2::CheckSub_KSURI2_6CheckOff(short index, int clmidx)
{

	int idx_1235[4] = { idx_ksuri2_1[clmidx] , idx_ksuri2_2[clmidx], idx_ksuri2_3[clmidx], idx_ksuri2_5[clmidx] };

	//1.2.3.5入力許可
	for (int i = 0; i < sizeof(idx_1235) / sizeof(idx_1235[0]); i++) {
		ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_1235[i], 1, (*m_pSnHeadData)->Sn_Sign4, 0);
	}

	//一時データに戻す.
	memmove(KSURI2_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_1_TMP[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);
	memmove(KSURI2_2[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_2_TMP[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);
	memmove(KSURI2_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_3_TMP[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);
	memmove(KSURI2_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_4_TMP[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);
	memmove(KSURI2_5[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_5_TMP[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);

	Update_KSURI2_6(index, clmidx);
	Update_KSURI2_8(index, clmidx, true);
	Update_KSURI2_8_KEI(index, clmidx);
	Update_KSURI2_9(index, clmidx);
	Update_KSURI2_9_KEI(index, clmidx);
	Update_KSURI2_12(index);
	Update_KSURI2_13(index);


	//---->yoshida190919
	//返還金額等再集計

	char val[MONY_BUF_SIZE] = { 0 }, val2[MONY_BUF_SIZE] = { 0 }, val50[MONY_BUF_SIZE] = { 0 }, val100[MONY_BUF_SIZE] = { 0 }, val108[MONY_BUF_SIZE] = { 0 }, val110[MONY_BUF_SIZE] = { 0 };
	CString str50 = _T("50");
	CString str100 = _T("100");
	CString str108 = _T("108");
	CString str110 = _T("110");
	m_pArith->l_input(&val50, str50.GetBuffer());
	m_pArith->l_input(&val100, str100.GetBuffer());
	m_pArith->l_input(&val108, str108.GetBuffer());
	m_pArith->l_input(&val110, str110.GetBuffer());

	memset(&val, 0, sizeof(val));

	l_pardotnet(&val[0], HENKAN_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_5[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);


	memset(&val2, 0, sizeof(val2));
	m_pArith->l_sub(&val2, HENKAN_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val);

	//売上変換（付表１用　税抜き計算処理いらない）
	memmove(KSURI_NEBIKI_KEI8_FHYO1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	memmove(KSURI_NEBIKI_10_FHYO1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val2, MONY_BUF_SIZE);


	m_pArith->l_mul(&val, &val, val100);
	m_pArith->l_div(&val, &val, val108);

	m_pArith->l_mul(&val2, &val2, val100);
	m_pArith->l_div(&val2, &val2, val110);


	memmove(KSURI_NEBIKI_KEI8[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	memmove(KSURI_NEBIKI_10[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val2, MONY_BUF_SIZE);


	//貸倒損失
	memset(&val, 0, sizeof(val));

	l_pardotnet(&val[0], KASISON_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_5[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);

	memset(&val2, 0, sizeof(val2));
	m_pArith->l_sub(&val2, KASISON_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val);

	memmove(KSURI_KASISON_KEI8[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	memmove(KSURI_KASISON_10[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val2, MONY_BUF_SIZE);


	//貸倒回収
	memset(&val, 0, sizeof(val));

	l_pardotnet(&val[0], KASIKAISYU_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_5[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI2_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);

	memset(&val2, 0, sizeof(val2));
	m_pArith->l_sub(&val2, KASIKAISYU_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val);

	memmove(KSURI_KASIKAISYU_KEI8[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	memmove(KSURI_KASIKAISYU_10[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val2, MONY_BUF_SIZE);
	//<---

}

void CShinKsisan_Uri2::Calc_KSURI2_8(short index, int clmidx, char* pVal)
{

	char val50[MONY_BUF_SIZE], val100[MONY_BUF_SIZE], val108[MONY_BUF_SIZE];
	CString str50 = _T("50");
	CString str100 = _T("100");
	CString str108 = _T("108");
	m_pArith->l_input(&val50, str50.GetBuffer());
	m_pArith->l_input(&val100, str100.GetBuffer());
	m_pArith->l_input(&val108, str108.GetBuffer());

	memset(pVal, 0, MONY_BUF_SIZE);
	if( KSURI2_6_CHECK[m_dispCol_left + clmidx] ) {
		l_pardotnet(pVal,
			KSURI2_7[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			val50,
			val100);
	}
	else {
		l_pardotnet(pVal,
			KSURI2_7[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			KSURI2_5[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			KSURI2_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);
	}

	m_pArith->l_mul(pVal, pVal, val100);
	m_pArith->l_div(pVal, pVal, val108);

}

void CShinKsisan_Uri2::Calc_KSURI2_9(short index, int clmidx, char* pVal)
{
	char val50[MONY_BUF_SIZE], val100[MONY_BUF_SIZE], val110[MONY_BUF_SIZE];
	CString str50 = _T("50");
	CString str100 = _T("100");
	CString str110 = _T("110");

	m_pArith->l_input(&val50, str50.GetBuffer());
	m_pArith->l_input(&val100, str100.GetBuffer());
	m_pArith->l_input(&val110, str110.GetBuffer());

	memset(pVal, 0, MONY_BUF_SIZE);
	if( KSURI2_6_CHECK[m_dispCol_left + clmidx] ) {
		l_pardotnet(pVal,
			KSURI2_7[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			val50,
			val100);
	}
	else {
		l_pardotnet(pVal,
			KSURI2_7[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			KSURI2_5[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			KSURI2_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);
	}

	m_pArith->l_sub(pVal, KSURI2_7[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, pVal);
	m_pArith->l_mul(pVal, pVal, val100);
	m_pArith->l_div(pVal, pVal, val110);

}

void CShinKsisan_Uri2::CheckSetData_KSURI2_8(short index, int clmidx)
{
	//入力値取得.
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	diag_getdata(IDC_ICSDIAG8CTRL2, idx_ksuri2_8[clmidx], (struct IUnknown*)&DiagData, CIcsdiagctrl);

	//計算値取得.
	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSURI2_8(index, clmidx, &val[0]);

	KSURI2_8_SIGN[m_dispCol_left + clmidx] = CheckSub_ChangeBackColor(idx_ksuri2_8[clmidx], DiagData.data_val, val );
}

void CShinKsisan_Uri2::CheckSetData_KSURI2_9(short index, int clmidx)
{
	//入力値取得.
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	diag_getdata(IDC_ICSDIAG8CTRL2, idx_ksuri2_9[clmidx], (struct IUnknown*)&DiagData, CIcsdiagctrl);

	//計算値取得.
	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSURI2_9(index, clmidx, &val[0]);

	KSURI2_9_SIGN[m_dispCol_left + clmidx] = CheckSub_ChangeBackColor(idx_ksuri2_9[clmidx], DiagData.data_val, val);

}

int CShinKsisan_Uri2::CheckSub_ChangeBackColor(short index, char* pVal1, char* pVal2, bool bReset)
{

	int rt = 0;

	DIAGRAM_ATTRIBUTE DA;
	diag_getattr(IDC_ICSDIAG8CTRL2, index, &DA, CIcsdiagctrl);

	if (m_TokureiMode == TOKUREIMODE_RENDO ) {
		DA.attr_bcolor = BC_WHITE;
	}
	else {
		if (bReset) {
			DA.attr_bcolor = BC_CREAM;
		}
		else {

			char tmpval[MONY_BUF_SIZE];
			memmove(tmpval, pVal2, MONY_BUF_SIZE);
			m_pArith->l_neg(tmpval);

			if (m_pArith->l_cmp(pVal1, pVal2) && m_pArith->l_cmp(pVal1, tmpval)) {
				DA.attr_bcolor = BC_BLUE;
				rt = 1;
			}
			else {
				DA.attr_bcolor = BC_CREAM;
			}
		}
	}

	diag_setattr(IDC_ICSDIAG8CTRL2, index, &DA, FALSE, CIcsdiagctrl);

	return rt;

}

void CShinKsisan_Uri2::ClearData(bool bFromRenew)
{
	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
			if (bFromRenew && kbndatainfo[tblidx].kind == DATAKIND_CHECK) {
				;
			}
			else if (bFromRenew && kbndatainfo[tblidx].kind == DATAKIND_RATE){
				// 50%適用にチェックがない場合はクリアする
				if( *(kbndatainfo[tblidx+1].data + kbndatainfo[tblidx+1].size * idx_jigyokbn) == 0 ){
					memset(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn,
					'\0', kbndatainfo[tblidx].size);
				}
			}
			else {
				memset(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn,
				'\0', kbndatainfo[tblidx].size);
			}
		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {
		memset(datainfo[tblidx].data, '\0', datainfo[tblidx].size);
	}

	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
		memset(KSURI2_1_TMP[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);
		memset(KSURI2_2_TMP[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);
		memset(KSURI2_3_TMP[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);
		memset(KSURI2_4_TMP[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);
		memset(KSURI2_5_TMP[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);

		//--->yoshida190919
		memset(HENKAN_GOKEI[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);
		memset(KSURI_NEBIKI_KEI8[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);
		memset(KSURI_NEBIKI_10[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);

		memset(KSURI_NEBIKI_KEI8_FHYO1[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);
		memset(KSURI_NEBIKI_10_FHYO1[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);

		memset(KASISON_GOKEI[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);
		memset(KSURI_KASISON_KEI8[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);
		memset(KSURI_KASISON_10[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);

		memset(KASIKAISYU_GOKEI[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);
		memset(KSURI_KASIKAISYU_KEI8[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);
		memset(KSURI_KASIKAISYU_10[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);
		//<-----

	}

	return;
}

//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinKsisan_Uri2::UpDownFocusMoveCheck(int Index, int Move)
{
	int nextFocusIdx = 0;
	int pos = 0;
	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for (pos = 0; pos < (sizeof(FCS_Ksisan_Uri2)/sizeof(FCS_Ksisan_Uri2[0])); pos++ ) {
		if ( Index == FCS_Ksisan_Uri2[pos].IDNo ) {
			break;
		}
	}

	nextFocusIdx = FCS_Ksisan_Uri2[pos].IDNo;	// 現在ポジションセット

	// 次ポジション検索
	if ( Move == MOVE_DOWN) {
		pos++;        
		for (int j = pos; j < (sizeof(FCS_Ksisan_Uri2) / sizeof(FCS_Ksisan_Uri2[0])); j++ ) {
			m_Ksisan_Uri2Diag02.GetAttribute( FCS_Ksisan_Uri2[j].IDNo, (LPUNKNOWN)&DA );
			if ( (DA.attr_bcolor == BC_CREAM) || (DA.attr_bcolor == BC_BLUE) ) {
				nextFocusIdx = FCS_Ksisan_Uri2[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == MOVE_UP) {
		pos--;
		for (int j = pos; j > -1; j-- ) {
			m_Ksisan_Uri2Diag02.GetAttribute(FCS_Ksisan_Uri2[j].IDNo, (LPUNKNOWN)&DA);
			if ((DA.attr_bcolor == BC_CREAM) || (DA.attr_bcolor == BC_BLUE)) {
				nextFocusIdx = FCS_Ksisan_Uri2[j].IDNo;
				break;
			}
		}
	}

	return nextFocusIdx;
}

BOOL CShinKsisan_Uri2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ) {
			// 更新
			GetAllDiagData();
			// テーブルバージョンアップ
			//WriteData(0, 2);
			WriteData(0, TOKUREI_TBL_VERSION);

			if ( m_pParent ) {
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}

			return TRUE;
		}
		if ( pMsg->wParam == VK_F2 ) {
			DIAGRAM_DATA	dd;
			if ( m_curwnd != -1 ) {
				short result = ((CIcsdiagctrl*)GetDlgItem(m_curwnd))->GetPosition();
				diag_getdata( m_curwnd, result, &dd, CIcsdiagctrl );
				if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
					TerminationIcsdiag8ctrl2( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}

		// test 190717
		/*
		if( (pMsg->message == SB_PAGEDOWN) || (pMsg->message == SB_PAGEUP) ){
			//m_pParent->PostMessageA(  );	
			return TRUE;
		}*/
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

BEGIN_EVENTSINK_MAP(CShinKsisan_Uri2, CSyzBaseDlg)
	ON_EVENT(CShinKsisan_Uri2, IDC_ICSDIAG8CTRL2, 1, CShinKsisan_Uri2::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinKsisan_Uri2, IDC_ICSDIAG8CTRL2, 2, CShinKsisan_Uri2::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinKsisan_Uri2, IDC_ICSDIAG8CTRL2, 3, CShinKsisan_Uri2::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinKsisan_Uri2, IDC_ICSDIAG8CTRL2, 9, CShinKsisan_Uri2::CheckButtonIcsdiag8ctrl2, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()


void CShinKsisan_Uri2::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;
	m_Focus = index;
}


void CShinKsisan_Uri2::EditOFFIcsdiag8ctrl2(short index)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	m_Ksisan_Uri2Diag02.GetData((int)index, (struct IUnknown *)&DiagData);

	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int clmidx = 0; clmidx < JIGYO_PER_PAGE_URI2; clmidx++) {

			if (index == kbndatainfo[tblidx].diagidx[clmidx]) {

				if (kbndatainfo[tblidx].kind == DATAKIND_STR) {
					memmove(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size,
						DiagData.data_edit, kbndatainfo[tblidx].size);
				}
				else if (kbndatainfo[tblidx].kind == DATAKIND_VAL) {
					memmove(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size,
						DiagData.data_val, MONY_BUF_SIZE);

					if (index == idx_ksuri2_1[clmidx] || index == idx_ksuri2_2[clmidx] || index == idx_ksuri2_3[clmidx]) {
						Update_KSURI2_4(index, clmidx);
						//if ( KSURI2_6_CHECK[clmidx] == 0 ) {
						if ( KSURI2_6_CHECK[m_dispCol_left + clmidx] == 0 ) {
							Update_KSURI2_6(index, clmidx);
						}
						Update_KSURI2_8(index, clmidx);
						Update_KSURI2_8_KEI(index, clmidx);
						Update_KSURI2_9(index, clmidx);
						Update_KSURI2_9_KEI(index, clmidx);
						Update_KSURI2_12(index);
						Update_KSURI2_13(index);
					}

					if (index == idx_ksuri2_5[clmidx]) {
						//if ( KSURI2_6_CHECK[clmidx] == 0 ) {
						if ( KSURI2_6_CHECK[m_dispCol_left + clmidx] == 0 ) {
							Update_KSURI2_6(index, clmidx);
						}
						Update_KSURI2_8(index, clmidx);
						Update_KSURI2_8_KEI(index, clmidx);
						Update_KSURI2_9(index, clmidx);
						Update_KSURI2_9_KEI(index, clmidx);
						Update_KSURI2_12(index);
						Update_KSURI2_13(index);
					}

					if (index == idx_ksuri2_7[clmidx]) {
						Update_KSURI2_8(index, clmidx);
						Update_KSURI2_8_KEI(index, clmidx);
						Update_KSURI2_9(index, clmidx);
						Update_KSURI2_9_KEI(index, clmidx);
						Update_KSURI2_12(index);
						Update_KSURI2_13(index);
					}
					
					if(index == idx_ksuri2_8[clmidx]){
						CheckSetData_KSURI2_8(index, clmidx);
						Update_KSURI2_8_KEI(index, clmidx);
						Update_KSURI2_12(index);
						Update_KSURI2_13(index);
					}

					if(index == idx_ksuri2_9[clmidx]){
						CheckSetData_KSURI2_9(index, clmidx);
						Update_KSURI2_9_KEI(index, clmidx);
						Update_KSURI2_12(index);
						Update_KSURI2_13(index);
					}

				}
				else if (kbndatainfo[tblidx].kind == DATAKIND_RATE) {
					m_SyzUtil.val_to_bin((unsigned char*)kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size, DiagData.data_edit);
				}

			}
		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {
		if (index == datainfo[tblidx].diagidx[0]) {

			if (datainfo[tblidx].kind == DATAKIND_VAL) {
				memmove(datainfo[tblidx].data, DiagData.data_val, MONY_BUF_SIZE);

				if (index == idx_ksuri2_10[0]) {
					Update_KSURI2_12(index);
				}

				if (index == idx_ksuri2_11[0]) {
					Update_KSURI2_13(index);
				}

				if(index == idx_ksuri2_8_kei[0]){
					Update_KSURI2_12(index);
				}

				if(index == idx_ksuri2_9_kei[0]){
					Update_KSURI2_13(index);
				}

			}

		}
	}

	Set_KSURI2_data();
}

void CShinKsisan_Uri2::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if ( nChar == 0 ){
		return;
	}

	// Shift+TabをTabに、TabをEnterに変換する
	short wTerm;
	char VK_FLG = 0x00;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );
	if ( nChar == VK_TAB ) {
		if ( VK_FLG & 0x80 ) {
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
	if ( (wTerm == VK_UP) || (wTerm == VK_TAB) ) {
		pos = UpDownFocusMoveCheck(index, 1);
		m_Ksisan_Uri2Diag02.SetPosition(pos);
	}

	if ( (wTerm == VK_DOWN) || (wTerm == VK_RETURN) ) {
		pos = UpDownFocusMoveCheck(index, 0);
		m_Ksisan_Uri2Diag02.SetPosition(pos);
	}

	if ( (wTerm==VK_F3) || (wTerm==VK_RIGHT) ) {
		if (IsRightEnd(index, &pos) && (GetCol_FromIndex(index) < JIGYO_MAX_TOKUREI_23)) {

			EditOFFIcsdiag8ctrl2(index);
			m_dispCol_left++;

			if (m_dispCol_left > JIGYO_MAX_TOKUREI_23 - JIGYO_PER_PAGE_URI2) {
				if ((pos != ID_KSURI2_8_KEI) && (pos != ID_KSURI2_9_KEI)) {
					m_dispCol_left = 0;
					m_bar.SetScrollPos(m_dispCol_left);
				}
				else {
					m_dispCol_left--;
				}
				DispKsisan_Uri2();
				m_Ksisan_Uri2Diag02.SetPosition(pos);
			}
			else {
				m_bar.SetScrollPos(m_dispCol_left);
				DispKsisan_Uri2();
				m_Ksisan_Uri2Diag02.SetPosition(index);
			}

		}
		else {

			if ((m_TokureiMode == TOKUREIMODE_HIRENDO) && (index == ID_KSURI2_11)) {
				m_Ksisan_Uri2Diag02.SetPosition(ID_KSURI2_TITLE1);
			}
            else {
				m_Ksisan_Uri2Diag02.SetNextPosition();
			}

		}
	}
	else if ( (wTerm==VK_F2) || (wTerm==VK_LEFT) ) {	// 左
	
		if (IsLeftEnd(index, &pos) && (GetCol_FromIndex(index) >= 0)) {

			EditOFFIcsdiag8ctrl2(index);
			if ( (m_dispCol_left == 0) && (m_TokureiMode == TOKUREIMODE_RENDO)) {	// 連動の場合、事業区分名欄は1列目固定
				;
			}
			else
				m_dispCol_left--;

			if( m_dispCol_left < 0 ) {

				if ((m_TokureiMode == TOKUREIMODE_HIRENDO) && (index == ID_KSURI2_TITLE1)) {
					m_dispCol_left = 0;
					m_Ksisan_Uri2Diag02.SetPosition(ID_KSURI2_11);
				}
                else {
					m_dispCol_left = JIGYO_MAX_TOKUREI_23 - JIGYO_PER_PAGE_URI2;
					m_bar.SetScrollPos(m_dispCol_left);
					DispKsisan_Uri2();
					m_Ksisan_Uri2Diag02.SetPosition(pos);
			    }

			}
			else {
				m_bar.SetScrollPos(m_dispCol_left);
				DispKsisan_Uri2();
				m_Ksisan_Uri2Diag02.SetPosition(index);
			}

		}
		else {
			m_Ksisan_Uri2Diag02.SetPrevPosition();
		}
	}

	if ( wTerm == VK_DELETE ) {

		for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
			for (int clmidx = 0; clmidx < JIGYO_PER_PAGE_URI2; clmidx++) {
				if (index == kbndatainfo[tblidx].diagidx[clmidx]) {
					if( index == idx_ksuri2_8[clmidx] ) {
						if (KSURI2_6_CHECK[m_dispCol_left + clmidx] == 0) {
							Update_KSURI2_8(index, clmidx, true);
						}
						else {
							CheckSub_KSURI2_6CheckOn(index, clmidx, true);
						}
						Set_KSURI2_data();
						KSURI2_8_SIGN[m_dispCol_left + clmidx] = 0;
					}
					else if( index == idx_ksuri2_9[clmidx] ) {

						if (KSURI2_6_CHECK[m_dispCol_left + clmidx] == 0) {
							Update_KSURI2_9(index, clmidx, true);
						}
						else {
							CheckSub_KSURI2_6CheckOn(index, clmidx, true);
						}
						Set_KSURI2_data();
						KSURI2_9_SIGN[m_dispCol_left + clmidx] = 0;
					}
					else {
						memset(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size,
							'\0', kbndatainfo[tblidx].size);
						dsp_cls(IDC_ICSDIAG8CTRL2, index);
					}
				}
			}
		}

		for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {
			if (index == datainfo[tblidx].diagidx[0]) {
				memset(datainfo[tblidx].data, '\0', datainfo[tblidx].size);
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
			}
		}

	}

	int nowpos = m_Ksisan_Uri2Diag02.GetPosition();	// 移動先または自身のポジション

	if( nowpos == ID_KSURI2_11 ){
		nowpos = ID_KSURI2_13;
	}

	RECT rectA, rectB;
	GetWindowRect(&rectA);
	diag_getrect(IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl);
	if ((rectA.top > rectB.top) || (rectA.bottom < rectB.bottom)) {
		int	cpos = GetScrollPos(SB_VERT);
		SetRedrawScroll((cpos + rectB.top - rectA.top) - ((rectA.bottom - rectA.top) / 2));
	}
}


void CShinKsisan_Uri2::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	if (pScrollBar == &m_bar) {	//横スクロールバー
		this->LockWindowUpdate();

		BOOL DrawSw = TRUE;
		int	oldLeft = m_dispCol_left;

		EditOFFIcsdiag8ctrl2(m_Focus);
		diag_deledit(IDC_ICSDIAG8CTRL2, CIcsdiagctrl);

		switch (nSBCode) {
			case SB_LINELEFT:
				m_dispCol_left--;
				break;
			case SB_LINERIGHT:
				m_dispCol_left++;
				break;
			case SB_PAGELEFT:
				m_dispCol_left -= JIGYO_PER_PAGE_URI2;
				break;
			case SB_PAGERIGHT:
				m_dispCol_left += JIGYO_PER_PAGE_URI2;
				break;
			case SB_LEFT:
				m_dispCol_left = 0;
				break;
			case SB_RIGHT:
				m_dispCol_left = JIGYO_MAX_TOKUREI_23 - 1;
				break;
			case SB_THUMBTRACK:
				m_dispCol_left = nPos;
				break;	//Drag
			default: //SB_ENDSCROLL
				DrawSw = FALSE;		//描画されないときに、セット
				break;
		}

		if( !(*m_pSnHeadData)->Sn_Sign4 & 0x01 ){
			// 連動時、1列目以外は入力不可にする (事業区分名)
			if( m_TokureiMode == TOKUREIMODE_RENDO ){
				if( m_dispCol_left == 0 ){		// 1列目→入力OK
					ATRIB_MOD(IDC_ICSDIAG8CTRL2, ID_KSURI2_TITLE1, 1, (*m_pSnHeadData)->Sn_Sign4, Ksisan_Uri2[0].Connect);
				}
				else if( m_dispCol_left > 0 ){	// 1列目以外→入力不可
					ATRIB_MOD(IDC_ICSDIAG8CTRL2, ID_KSURI2_TITLE1, 0, (*m_pSnHeadData)->Sn_Sign4, Ksisan_Uri2[0].Connect);
				}
			}

			// 連動時、1列目以外は入力不可にする (５０％適用チェックボックス)
			if (m_TokureiMode == TOKUREIMODE_RENDO) {
				if (m_dispCol_left == 0) {		// 1列目→入力OK
					ATRIB_MOD(IDC_ICSDIAG8CTRL2, ID_KSURI2_6CHECK_1, 1, (*m_pSnHeadData)->Sn_Sign4, Ksisan_Uri2[12].Connect);
				}
				else if (m_dispCol_left > 0) {	// 1列目以外→入力不可
					ATRIB_MOD(IDC_ICSDIAG8CTRL2, ID_KSURI2_6CHECK_1, 0, (*m_pSnHeadData)->Sn_Sign4, Ksisan_Uri2[12].Connect);
				}
			}
		}
		else{
			// 連動時、入力ロックが押されているときは1列目のみグレー背景にする。それ以外は白背景
			DIAGRAM_ATTRIBUTE	DA;
			if( m_TokureiMode == TOKUREIMODE_RENDO ){
				if( m_dispCol_left == 0 ){
					diag_getattr( IDC_ICSDIAG8CTRL2, ID_KSURI2_TITLE1, &DA, CIcsdiagctrl );
					DA.attr_bcolor = BC_GRAY;
					diag_setattr( IDC_ICSDIAG8CTRL2, ID_KSURI2_TITLE1, &DA, TRUE, CIcsdiagctrl );
					diag_getattr( IDC_ICSDIAG8CTRL2, ID_KSURI2_6CHECK_1, &DA, CIcsdiagctrl );
					DA.attr_bcolor = BC_GRAY;
					diag_setattr( IDC_ICSDIAG8CTRL2, ID_KSURI2_6CHECK_1, &DA, TRUE, CIcsdiagctrl );
				}
				else if( m_dispCol_left > 0 ){
					diag_getattr( IDC_ICSDIAG8CTRL2, ID_KSURI2_TITLE1, &DA, CIcsdiagctrl );
					DA.attr_bcolor = BC_WHITE;
					diag_setattr( IDC_ICSDIAG8CTRL2, ID_KSURI2_TITLE1, &DA, TRUE, CIcsdiagctrl );
					diag_getattr( IDC_ICSDIAG8CTRL2, ID_KSURI2_6CHECK_1, &DA, CIcsdiagctrl );
					DA.attr_bcolor = BC_WHITE;
					diag_setattr( IDC_ICSDIAG8CTRL2, ID_KSURI2_6CHECK_1, &DA, TRUE, CIcsdiagctrl );
				}
			}
		}

		if (m_dispCol_left > JIGYO_MAX_TOKUREI_23 - JIGYO_PER_PAGE_URI2) {
			m_dispCol_left = JIGYO_MAX_TOKUREI_23 - JIGYO_PER_PAGE_URI2;
		}

		if (m_dispCol_left < 0) {
			m_dispCol_left = 0;
		}

		if (oldLeft == m_dispCol_left) {
			DrawSw = FALSE;
		}

		if (DrawSw) {
			m_bar.SetScrollPos(m_dispCol_left);			//スクロールバーの移動
			DispKsisan_Uri2();							//表示更新
		}

		diag_setposition(IDC_ICSDIAG8CTRL2, m_Focus, CIcsdiagctrl);

		this->UnlockWindowUpdate();
	}

	CSyzBaseDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

int CShinKsisan_Uri2::GetCol_FromIndex(int index)
{
	int ret = -1;

	if ( IsLeftEnd(index) ) {
		ret = m_dispCol_left;
	}

	if (IsRightEnd(index)) {
		ret = m_dispCol_left+1;
	}

	return ret;
}

BOOL CShinKsisan_Uri2::IsRightEnd(int index, int* pNextPos)
{
	BOOL ret = FALSE;

	for(int i = 0; i < (sizeof(idx_ksuri2_row2)/sizeof(idx_ksuri2_row2[0])) ; i++){
		if( idx_ksuri2_row2[i] == index ){
			if( pNextPos != NULL) {

				//if( (idx_ksuri2_row2[i] == ID_KSURI2_8_2) 
				//	&& (m_dispCol_left > JIGYO_MAX_TOKUREI_23 - JIGYO_PER_PAGE_URI2 - 1)) {
				//	*pNextPos = ID_KSURI2_8_KEI;
				//}
				//else if ((idx_ksuri2_row2[i] == ID_KSURI2_9_2)
				//	&& (m_dispCol_left > JIGYO_MAX_TOKUREI_23 - JIGYO_PER_PAGE_URI2 - 1)) {
				//	*pNextPos = ID_KSURI2_9_KEI;
				//}
				//else {
					*pNextPos = idx_ksuri2Rend_next[i];
				//}

			}
			ret = TRUE;
			break;
		}
	}

	return ret;
}

BOOL CShinKsisan_Uri2::IsLeftEnd(int index, int* pNextPos)
{
	BOOL ret = FALSE;

	for (int i = 0; i < (sizeof(idx_ksuri2_row1) / sizeof(idx_ksuri2_row1[0])); i++) {
		if (idx_ksuri2_row1[i] == index) {
			if( pNextPos != NULL){
				*pNextPos = idx_ksuri2Lend_next[i];
			}
			ret = TRUE;
			break;
		}
	}

	return ret;
}



void CShinKsisan_Uri2::CheckButtonIcsdiag8ctrl2(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。

	for (int clmidx = 0; clmidx < JIGYO_PER_PAGE_URI2; clmidx++) {
		if (index == idx_ksuri2_6_check[clmidx]) {
			Check_KSURI2_6Check(index, clmidx);
		}
	}

}


void CShinKsisan_Uri2::OnBnClickedKubunIns()
{
	// 列挿入

	int Clmindex = GetCol_FromIndex( m_Focus );
	if( Clmindex == -1 ){
		return;
	}

	if( IsNULL_Column( JIGYO_MAX_TOKUREI_23-1 ) == FALSE ){	// 最後尾のカラムに空きがあるかどうか
		m_Ksisan_Uri2Diag02.SetPosition(m_Focus);
		return;
	}

	MoveData( Clmindex, 1 );		// 列データを右に寄せる
	Init_KubunColumn( Clmindex );	// カラムデータを削除して空きを作る
	DispKsisan_Uri2();

	m_Ksisan_Uri2Diag02.SetPosition(m_Focus);
}


void CShinKsisan_Uri2::OnBnClickedKubunDel()
{
	// 列削除

	int Clmindex = GetCol_FromIndex( m_Focus );
	if( Clmindex == -1 ){
		return;
	}
	CString msg;
	msg.Format("番号［ %d ］の情報を削除しますか？", Clmindex + 1 );
	if( ICSMessageBox(msg, MB_YESNO|MB_DEFBUTTON2, 0)!=IDYES ){
		return;
	}

	MoveData( Clmindex, 0 );			// 列データを左に寄せる
	Init_KubunColumn( JIGYO_MAX_TOKUREI_23-1 );	// 一番右端のデータを削除
	ReCalq();							// 再集計
	DispKsisan_Uri2();

	// 移動先が入力不可項目のこともあるため、常に事業区分にセット
	//m_Ksisan_Uri2Diag02.SetPosition(m_Focus);
	m_Ksisan_Uri2Diag02.SetPosition ( idx_ksuri2_title[Clmindex] );
}

BOOL CShinKsisan_Uri2::IsNULL_Column( int Clmindex )
{
	// 該当のカラムに空きがあるか？
	for( int clmidx = 0; clmidx < JIGYO_MAX_TOKUREI_23; clmidx++ ){
		if( clmidx == Clmindex ){
			for( int tblidx = 0; tblidx < sizeof( kbndatainfo ) / sizeof( kbndatainfo[0] ); tblidx++ ){

				CString	str;

				if( kbndatainfo[tblidx].kind == DATAKIND_CHECK ){
					continue;
				}
				else if( kbndatainfo[tblidx].kind == DATAKIND_RATE ){
					if( m_pArith->l_test( kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * clmidx ) > 0 ){
						return FALSE;
					}
				}
				else if( kbndatainfo[tblidx].kind == DATAKIND_VAL ){
					if( m_pArith->l_test( kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * clmidx ) > 0 ){
						return FALSE;
					}
				} 
				else if( kbndatainfo[tblidx].kind == DATAKIND_STR ){
					str.Format( "%s", kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * clmidx );
					if( str.GetLength() != 0 ){
						return FALSE;
					}
				}
				else if( kbndatainfo[tblidx].kind == DATAKIND_DATE ){
					str.Format( "%s", kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * clmidx );
					if( str.GetLength() != 0 ){
						return FALSE;
					}
				}
			}
		}
	}

	return TRUE;
}

void CShinKsisan_Uri2::Init_KubunColumn( int ClmIndex )
{
	for( int clmidx = 0; clmidx < JIGYO_MAX_TOKUREI_23; clmidx++ ){	// 事業ごとのカラム
		if( clmidx == ClmIndex ){
			for( int tblidx = 0; tblidx < sizeof( kbndatainfo ) / sizeof( kbndatainfo[0] ); tblidx++ ){
				memset( kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * clmidx, '\0', kbndatainfo[tblidx].size );	// 削除
			}
		}
	}
}

void CShinKsisan_Uri2::MoveData( int ClmIndex , int sign )
{
	if( sign == 0 ){	// 列削除
		
		for( int clmidx = ClmIndex; clmidx < JIGYO_MAX_TOKUREI_23; clmidx++ ){
			for( int tblidx = 0; tblidx < sizeof( kbndatainfo ) / sizeof( kbndatainfo[0] ); tblidx++ ){

				// 1つ前のカラムにデータを移す
				memmove( kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * clmidx,
						 kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * ( clmidx + 1 ),
						 kbndatainfo[tblidx].size );

				memmove(KSURI2_1_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI2_1_TMP[0] + (clmidx + 1) * MONY_BUF_SIZE, MONY_BUF_SIZE);
				memmove(KSURI2_2_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI2_2_TMP[0] + (clmidx + 1) * MONY_BUF_SIZE, MONY_BUF_SIZE);
				memmove(KSURI2_3_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI2_3_TMP[0] + (clmidx + 1) * MONY_BUF_SIZE, MONY_BUF_SIZE);
				memmove(KSURI2_4_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI2_4_TMP[0] + (clmidx + 1) * MONY_BUF_SIZE, MONY_BUF_SIZE);
				memmove(KSURI2_5_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI2_5_TMP[0] + (clmidx + 1) * MONY_BUF_SIZE, MONY_BUF_SIZE);

			}
		}
	}
	else if( sign == 1 ){	// 列挿入

		for( int clmidx = JIGYO_MAX_TOKUREI_23-1; clmidx >= ClmIndex; clmidx-- ){
			for( int tblidx = 0; tblidx < sizeof( kbndatainfo ) / sizeof( kbndatainfo[0] ); tblidx++ ){
			
				// 次のカラムにデータを移す
				memmove( kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * clmidx,
						 kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * ( clmidx - 1 ),
						 kbndatainfo[tblidx].size );

				memmove(KSURI2_1_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI2_1_TMP[0] + (clmidx - 1) * MONY_BUF_SIZE, MONY_BUF_SIZE);
				memmove(KSURI2_2_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI2_2_TMP[0] + (clmidx - 1) * MONY_BUF_SIZE, MONY_BUF_SIZE);
				memmove(KSURI2_3_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI2_3_TMP[0] + (clmidx - 1) * MONY_BUF_SIZE, MONY_BUF_SIZE);
				memmove(KSURI2_4_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI2_4_TMP[0] + (clmidx - 1) * MONY_BUF_SIZE, MONY_BUF_SIZE);
				memmove(KSURI2_5_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI2_5_TMP[0] + (clmidx - 1) * MONY_BUF_SIZE, MONY_BUF_SIZE);

			}
		}
	}
}

void CShinKsisan_Uri2::ReCalq()
{
	// 再集計
	for( int clmidx = 0; clmidx < JIGYO_PER_PAGE_URI2; clmidx++ ){
		for( int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++ ){

			if( kbndatainfo[tblidx].kind == DATAKIND_VAL ){

				int index = kbndatainfo[tblidx].diagidx[clmidx];

				//　合計のみ再集計
				if (index == idx_ksuri2_1[clmidx] || index == idx_ksuri2_2[clmidx] || index == idx_ksuri2_3[clmidx]) {
					//Update_KSURI2_4(index, clmidx);
					//Update_KSURI2_6(index, clmidx);
					//Update_KSURI2_8(index, clmidx);
					Update_KSURI2_8_KEI(index, clmidx);
					//Update_KSURI2_9(index, clmidx);
					Update_KSURI2_9_KEI(index, clmidx);
					Update_KSURI2_12(index);
					Update_KSURI2_13(index);
					Set_KSURI2_data();
				}

				if (index == idx_ksuri2_5[clmidx]) {

					//Update_KSURI2_6(index, clmidx);
					//Update_KSURI2_8(index, clmidx);
					Update_KSURI2_8_KEI(index, clmidx);
					//Update_KSURI2_9(index, clmidx);
					Update_KSURI2_9_KEI(index, clmidx);
					Update_KSURI2_12(index);
					Update_KSURI2_13(index);
					Set_KSURI2_data();
				}

				if (index == idx_ksuri2_7[clmidx]) {
					//Update_KSURI2_8(index, clmidx);
					Update_KSURI2_8_KEI(index, clmidx);
					//Update_KSURI2_9(index, clmidx);
					Update_KSURI2_9_KEI(index, clmidx);
					Update_KSURI2_12(index);
					Update_KSURI2_13(index);
					Set_KSURI2_data();
				}

				if(index == idx_ksuri2_8[clmidx]){
					Update_KSURI2_8_KEI(index, clmidx);
					Update_KSURI2_12(index);
					Update_KSURI2_13(index);
					Set_KSURI2_data();
				}

				if(index == idx_ksuri2_9[clmidx]){
					Update_KSURI2_9_KEI(index, clmidx);
					Update_KSURI2_12(index);
					Update_KSURI2_13(index);
					Set_KSURI2_data();
				}
			}
		}
	}
}

void CShinKsisan_Uri2::DispClmNum()
{
	// 列番号の表示
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	CString str;
	str.Format( "%d", m_dispCol_left + 1 );
	DiagData.data_disp = str;
	diag_setdata( IDC_ICSDIAG8CTRL2, ID_KSURI2_NO1, (struct IUnknown*)&DiagData, CIcsdiagctrl );
	str.Format( "%d", m_dispCol_left + 2 );
	DiagData.data_disp = str;
	diag_setdata( IDC_ICSDIAG8CTRL2, ID_KSURI2_NO2, (struct IUnknown*)&DiagData, CIcsdiagctrl );
}

// 事業区分のスクロールバーを画面最下部に移動
void CShinKsisan_Uri2::MoveWindow_JigyoKubunBar(bool bFromView)
{
	if( !m_bar ){
		return;
	}

	CRect crect_bar,crect_window;
	long hight;
	m_pParent->GetWindowRect( &crect_window );
	m_bar.GetWindowRect( &crect_bar );
	if(( crect_window.bottom < 0 )||(crect_window.top < 0)
		||(crect_window.left < 0 )||(crect_window.right < 0 )){
			return;
	}
	ScreenToClient( &crect_window );
	ScreenToClient( &crect_bar );
	hight = crect_bar.bottom - crect_bar.top;
	if (bFromView) {
		hight = 40;
	}
	if( hight < 20 ){	// マルチの時に下半分が切れてしまうため調整
		hight = 20;
	}
	crect_bar.bottom = crect_window.bottom;
	crect_bar.top = crect_bar.bottom - hight;
	m_bar.MoveWindow( &crect_bar );
}
