// ShinKshiire.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinKshiire.h"
#include "ShinKshiireIdx.h"
#include "afxdialogex.h"
// CShinKshiire ダイアログ

IMPLEMENT_DYNAMIC(CShinKshiire, CSyzBaseDlg)

CShinKshiire::CShinKshiire(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(IDD_DIALOG_KSHIIRE, pParent)
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
	m_dispCol_left = 0;
	m_Focus = 0;
	m_SnSeq = -1;
}

CShinKshiire::~CShinKshiire()
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

void CShinKshiire::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_KshiireDiag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_KshiireDiag02);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_bar);
	DDX_Control(pDX, IDC_BUTTON_SHIIRESYUKEI, m_ButtonShiireSyukei);
	DDX_Control(pDX, IDC_BUTTON1, m_ButtonShiireColIns);
	DDX_Control(pDX, IDC_BUTTON2, m_ButtonShiireColDel);
}


BEGIN_MESSAGE_MAP(CShinKshiire, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_SHIIRESYUKEI, &CShinKshiire::OnBnClickedButtonShiiresyukei)
	ON_BN_CLICKED(IDC_BUTTON1, &CShinKshiire::OnBnClickedKubunIns)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinKshiire::OnBnClickedKubunDel)
END_MESSAGE_MAP()


// CShinKshiire メッセージ ハンドラー


BOOL CShinKshiire::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Init();

	// 縦スクロール
	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2);
	m_BottomIdx = KSHIIRE_DIAGCTRLID_MAX;	// 一番下のindexをセットする

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数					：	
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//		pSnHonpyoData	：	本表データ読込みクラス
//		SyzSyukei		：	集計クラス
//		snseq			：	履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinKshiire::InitInfo( CSnHeadData **pSnHeadData, SYC_SHININFO *pShinInfo, CArithEx *pArith, CDBNpSub *pZmSub, CSyzSyukei **pSyzSyukei, int snseq)
{
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

	m_pSimKuriwari = new CSimKuriwari(pZmSub);
	m_pDBSyzAccess = new CDBSyzAccess();

	m_SnSeq = snseq;

	return 0;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
void CShinKshiire::Init()
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

	// 特例テーブルバージョンチェック.
	TokureiTblVersionCheck(m_SnSeq);

	// データ読込み
	CString filter;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), m_SnSeq, ICS_SN_TOKUREI_3_HYOID, TOKUREI_TBL_VERSION);
	ReadData(m_SnSeq, TOKUREI_TBL_VERSION, filter);
	// 第5-(1)号様式と併用しておらず非連動の場合には、全金額取得後に小売等軽減売上割合を計算する
	if ( ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) == 0x00 ) && ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		ResetRate();
	}

	if( !(*m_pSnHeadData)->IsNeedTkkzDisp() ){
		Update_KSHIIRE_18();
		Update_KSHIIRE_20();
	}

	SCROLLINFO SCI;
	SCI.cbSize = 0;
	SCI.fMask = SIF_ALL;
	SCI.nMin = 0;
	SCI.nMax = JIGYO_MAX_TOKUREI_23 - 1;
	SCI.nPage = JIGYO_PER_PAGE_SHIIRE;
	SCI.nPos = 0;
	m_bar.SetScrollInfo(&SCI);
	m_bar.SetScrollPos(m_dispCol_left);

	if ((m_TokureiMode == TOKUREIMODE_HANRENDO)
		&& !((m_pZmSub->zvol->m_type & 0x01) == 0x01) //合併マスター
		&& !((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {   //入力ロック
		//ButtonShiireSyukei.EnableWindow(TRUE);
	}
	else {
		//m_ButtonShiireSyukei.EnableWindow(FALSE);
	}

	// 半連動を閉じたため、集計ボタンを非表示
	m_ButtonShiireSyukei.ShowWindow ( SW_HIDE );

	if (m_TokureiMode == TOKUREIMODE_RENDO 
		|| ((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {    //入力ロック
		m_ButtonShiireColIns.EnableWindow(FALSE);
		m_ButtonShiireColDel.EnableWindow(FALSE);
	}
	else {
		m_ButtonShiireColIns.EnableWindow(TRUE);
		m_ButtonShiireColDel.EnableWindow(TRUE);
	}

	InitDiagAttr();    // ダイアグラム初期設定
	DispKshiire();     // 画面出力
	SetItemPosition(); // 初期ポジションセット

	if (m_TokureiMode == TOKUREIMODE_RENDO) {
        SetSyukeiOnRendo();
	}
}

//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
void CShinKshiire::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if (m_curwnd == IDC_ICSDIAG8CTRL2) {
		index = m_KshiireDiag02.GetPosition();
		if ((index = m_KshiireDiag02.GetPosition()) != -1) {
			m_KshiireDiag02.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット
			m_KshiireDiag02.SetPosition(index);
			m_KshiireDiag02.SetFocus();
			return;
		}
	}

	m_KshiireDiag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット
	m_KshiireDiag02.SetFocus();

	// 課税資産の譲渡等（免税取引及び旧税率(6.3%等)が適用される取引は除く。）の税込み価格の合計
	diag_setposition(IDC_ICSDIAG8CTRL2, ID_KSHIIRE_TITLE1, CIcsdiagctrl);
}

void CShinKshiire::InitTblInfo()
{
	kbndatainfo[0] = { _T("CWB00030"), DATAKIND_STR, KSHIIRE_TITLE[0], 64, &idx_kshiire_title[0] };
	kbndatainfo[1] = { _T("CWB00040"), DATAKIND_VAL, KSHIIRE_1[0], MONY_BUF_SIZE, &idx_kshiire_1[0] };
	kbndatainfo[2] = { _T("CWB00050"), DATAKIND_VAL, KSHIIRE_2[0], MONY_BUF_SIZE, &idx_kshiire_2[0] };
	kbndatainfo[3] = { _T("CWB00060"), DATAKIND_RATE, KSHIIRE_3[0], MONY_BUF_SIZE, &idx_kshiire_3[0] };
	kbndatainfo[4] = { _T("CWB00065"), DATAKIND_SIGN, (char*)&KSHIIRE_3_SIGN[0], sizeof(int), &idx_kshiire_dummy[0] }; 
	kbndatainfo[5] = { _T("CWB00080"), DATAKIND_VAL, KSHIIRE_4[0], MONY_BUF_SIZE, &idx_kshiire_4[0] };
	kbndatainfo[6] = { _T("CWB00090"), DATAKIND_VAL, KSHIIRE_5[0], MONY_BUF_SIZE, &idx_kshiire_5[0] };
	kbndatainfo[7] = { _T("CWB00100"), DATAKIND_VAL, KSHIIRE_6[0], MONY_BUF_SIZE, &idx_kshiire_6[0] };
	kbndatainfo[8] = { _T("CWB00110"), DATAKIND_VAL, KSHIIRE_7[0], MONY_BUF_SIZE, &idx_kshiire_7[0] };
	kbndatainfo[9] = { _T("CWB00115"),DATAKIND_SIGN, (char*)&KSHIIRE_7_SIGN[0], sizeof(int), &idx_kshiire_dummy[0] };
	kbndatainfo[10] = { _T("CWB00116"), DATAKIND_VAL, KSHIIRE_sirehenkannkei8[0], MONY_BUF_SIZE, &idx_kshiire_dummy[0] };
	kbndatainfo[11] = { _T("CWB00120"), DATAKIND_VAL, KSHIIRE_8[0], MONY_BUF_SIZE, &idx_kshiire_8[0] };
	kbndatainfo[12] = { _T("CWB00125"), DATAKIND_SIGN, (char*)&KSHIIRE_8_SIGN[0], sizeof(int), &idx_kshiire_dummy[0] };
	kbndatainfo[13] = { _T("CWB00126"), DATAKIND_VAL, KSHIIRE_sirehenkann10[0], MONY_BUF_SIZE, &idx_kshiire_dummy[0] };

	datainfo[0] = { _T("CWB00140"), DATAKIND_VAL, KSHIIRE_7_KEI, MONY_BUF_SIZE, &idx_kshiire_7_kei[0] };
	datainfo[1] = { _T("CWB00150"), DATAKIND_VAL, KSHIIRE_8_KEI, MONY_BUF_SIZE, &idx_kshiire_8_kei[0] };
	datainfo[2] = { _T("CWB00170"), DATAKIND_VAL, KSHIIRE_9, MONY_BUF_SIZE, &idx_kshiire_9[0] };
	datainfo[3] = { _T("CWB00180"), DATAKIND_VAL, KSHIIRE_10, MONY_BUF_SIZE, &idx_kshiire_10[0] };
	datainfo[4] = { _T("CWC00030"), DATAKIND_VAL, KSHIIRE_11_1, MONY_BUF_SIZE, &idx_kshiire_11_1[0] };
	datainfo[5] = { _T("CWC00040"), DATAKIND_VAL, KSHIIRE_11_2, MONY_BUF_SIZE, &idx_kshiire_11_2[0] };
	datainfo[6] = { _T("CWC00060"), DATAKIND_VAL, KSHIIRE_12_1, MONY_BUF_SIZE, &idx_kshiire_12_1[0] };
	datainfo[7] = { _T("CWC00065"), DATAKIND_SIGN, (char*)&KSHIIRE_12_1_SIGN, sizeof(int), &idx_kshiire_dummy[0] };
	datainfo[8] = { _T("CWC00070"), DATAKIND_VAL, KSHIIRE_12_2, MONY_BUF_SIZE, &idx_kshiire_12_2[0] };
	datainfo[9] = { _T("CWC00075"), DATAKIND_SIGN, (char*)&KSHIIRE_12_2_SIGN, sizeof(int), &idx_kshiire_dummy[0] };
	datainfo[10] = { _T("CWC00080"), DATAKIND_VAL, KSHIIRE_13_2, MONY_BUF_SIZE, &idx_kshiire_13_2[0] };
	datainfo[11] = { _T("CWC00090"), DATAKIND_VAL, KSHIIRE_14_2, MONY_BUF_SIZE, &idx_kshiire_14_2[0] };
	datainfo[12] = { _T("CWC00095"), DATAKIND_SIGN, (char*)&KSHIIRE_14_2_SIGN, sizeof(int), &idx_kshiire_dummy[0] };
	datainfo[13] = { _T("CWC00110"), DATAKIND_VAL, KSHIIRE_15_1, MONY_BUF_SIZE, &idx_kshiire_15_1[0] };
	datainfo[14] = { _T("CWC00120"), DATAKIND_VAL, KSHIIRE_15_2, MONY_BUF_SIZE, &idx_kshiire_15_2[0] };
	datainfo[15] = { _T("CWC00140"), DATAKIND_VAL, KSHIIRE_16_1, MONY_BUF_SIZE, &idx_kshiire_16_1[0] };
	datainfo[16] = { _T("CWC00150"), DATAKIND_VAL, KSHIIRE_16_2, MONY_BUF_SIZE, &idx_kshiire_16_2[0] };
	datainfo[17] = { _T("CWC00170"), DATAKIND_VAL, KSHIIRE_17, MONY_BUF_SIZE, &idx_kshiire_17[0] };
	datainfo[18] = { _T("CWC00180"), DATAKIND_VAL, KSHIIRE_18, MONY_BUF_SIZE, &idx_kshiire_18[0] };
	datainfo[19] = { _T("CWD00010"), DATAKIND_VAL, KSHIIRE_19, MONY_BUF_SIZE, &idx_kshiire_19[0] };
	datainfo[20] = { _T("CWD00020"), DATAKIND_VAL, KSHIIRE_20, MONY_BUF_SIZE, &idx_kshiire_20[0] };

}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
int CShinKshiire::InitDiagAttr()
{
	int	cnt = 0;
	while (1) {
		if (Kshiire[cnt].Index == 0) {
			break;
		}

		int	Flg;
		// 確定時は全入力不可
		if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
			Flg = Kshiire[cnt].AllUnConnect;
		}
		else {
			if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
				if (((*m_pSnHeadData)->m_DispTabSgn & 0x10)
					&& (m_TokureiMode == TOKUREIMODE_HIRENDO) 
					&& ((Kshiire[cnt].Index == ID_KSHIIRE_3_1) || (Kshiire[cnt].Index == ID_KSHIIRE_3_2)) ) {
					Flg = 1;
				}
                else {
					Flg = Kshiire[cnt].UnConnect;
				}
			}
			else {
				if (m_TokureiMode == TOKUREIMODE_HANRENDO) {
					Flg = Kshiire[cnt].HalfConnect;
                }
                else {
					Flg = Kshiire[cnt].Connect;
				}
			}
		}
		ATRIB_MOD(IDC_ICSDIAG8CTRL2, Kshiire[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Kshiire[cnt].Connect);
		cnt++;
	}

	m_KshiireDiag02.EnableDelete(0);//DELETEキー１回で削除モード設定


	return 0;
}

//-----------------------------------------------------------------------------
// 連動時のデータ集計
//-----------------------------------------------------------------------------
// 引数     sign	:	どのタブから移動してきたか
//					:	0 : 基本情報タブから移動 / 1 : それ以外から移動
//-----------------------------------------------------------------------------
void CShinKshiire::SetSyukeiOnRendo( int sign )
{

	if (m_TokureiMode != TOKUREIMODE_RENDO) {
		return;
	}

	if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
		//入力ロック中.
		return;
	}

	// 基本情報タブから移動してきた場合は1列目に合わせる
	if( sign == 0 ){
		m_dispCol_left = 0;
	}
    else {
        GetAllDiagData();
		WriteData(0, TOKUREI_TBL_VERSION);
	}

	SIMKURIWARIZREC uriwariRec;
	SIMKSIREWARIZREC sirewariRec;

	if ((*m_pSnHeadData)->m_DispTabSgn & 0x10) {
		//第5-(1)号様式と併用
        memset(&uriwariRec, '\0', sizeof(uriwariRec));
		m_pSimKuriwari->SyukeiKuriwari10Eigyobi(&uriwariRec, m_tekiyotaisyokikan_s, m_tekiyotaisyokikan_e);
	}
	m_pSimKuriwari->SyukeiKsirewari(&sirewariRec, (*m_pSnHeadData)->IsNeedTkkzDisp(), m_tekiyotaisyokikan_s, m_tekiyotaisyokikan_e);

	SetSyukeiData(&sirewariRec, &uriwariRec);

	CheckSub_ChangeBackColor(idx_kshiire_7[0], NULL, NULL, true);
	CheckSub_ChangeBackColor(idx_kshiire_8[0], NULL, NULL, true);

	DispClmNum();
	m_KshiireDiag02.SetPosition(ID_KSHIIRE_TITLE1);
	if( sign == 0 ){
		m_bar.SetScrollPos(0);
	}
	else{
		m_bar.SetScrollPos(m_dispCol_left);
	}
}

void CShinKshiire::SetSyukeiData(SIMKSIREWARIZREC* pSirewariRec, SIMKURIWARIZREC* pUriwariRec)
{

	ClearData();

	if ( (pUriwariRec != NULL) && ((*m_pSnHeadData)->m_DispTabSgn & 0x10)) {

        //連動時
        if( m_TokureiMode == TOKUREIMODE_RENDO ) {
	
            if(Get50CheckStatusColumn0_5_1() ) {
                //第5-(1)号様式で□50%適用チェックON
				char val50[MONY_BUF_SIZE];
				CString str50 = _T("50");
				m_pArith->l_input(&val50, str50.GetBuffer());
				memmove(KSHIIRE_3[0], val50, MONY_BUF_SIZE);
			}
            else {
				memmove(KSHIIRE_1[0], pUriwariRec->uriage, MONY_BUF_SIZE);
				memmove(KSHIIRE_2[0], pUriwariRec->keigenuriage, MONY_BUF_SIZE);
				memmove(KSHIIRE_3[0], (char*)pUriwariRec->keigenwariai[0], MONY_BUF_SIZE);
			}
		}
        else {
			memmove(KSHIIRE_1[0], pUriwariRec->uriage, MONY_BUF_SIZE);
			memmove(KSHIIRE_2[0], pUriwariRec->keigenuriage, MONY_BUF_SIZE);
			memmove(KSHIIRE_3[0], (char*)pUriwariRec->keigenwariai[0], MONY_BUF_SIZE);
		}
	}
    else {
		memmove(KSHIIRE_1[0], pSirewariRec->uriagegoukei, MONY_BUF_SIZE);
		memmove(KSHIIRE_2[0], pSirewariRec->keigenuriagegoukei, MONY_BUF_SIZE);

		unsigned char keigenwariai[MONY_BUF_SIZE] = { 0 };
		if (m_pArith->l_test(pSirewariRec->keigenuriagegoukei) && m_pArith->l_test(pSirewariRec->uriagegoukei)) {
			m_pArith->l_par100(keigenwariai, pSirewariRec->keigenuriagegoukei, pSirewariRec->uriagegoukei, 0, 0);
		}
		else {
			memset(keigenwariai, '\0', MONY_BUF_SIZE);
		}
		memmove(KSHIIRE_3[0], keigenwariai, MONY_BUF_SIZE);

		Update_KSHIIRE_3( kbndatainfo[0].diagidx[0], 0 );
	}

	char val[MONY_BUF_SIZE] = { 0 };
	char val2[MONY_BUF_SIZE] = { 0 };
	memmove(val, pSirewariRec->sire, MONY_BUF_SIZE);
    m_pArith->l_sub(val, pSirewariRec->sire, pSirewariRec->sire358);
	memmove(KSHIIRE_4[0], val, MONY_BUF_SIZE);

	memset(&val, 0, sizeof(val));
	memset(&val2, 0, sizeof(val2));
	memmove(val, pSirewariRec->kamotsusire, MONY_BUF_SIZE);
	m_pArith->l_sub(val, pSirewariRec->kamotsusire, pSirewariRec->kamotsusire358);
	memmove(KSHIIRE_5[0], val, MONY_BUF_SIZE);

    m_pArith->l_add(KSHIIRE_6[0], KSHIIRE_4[0], KSHIIRE_5[0]);

	char val624[MONY_BUF_SIZE], val780[MONY_BUF_SIZE];
    char val100[MONY_BUF_SIZE], val108[MONY_BUF_SIZE], val110[MONY_BUF_SIZE];
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

	memset(&val, 0, sizeof(val));
	if ( Get50CheckStatusColumn0_5_1() ) {
		char val50[MONY_BUF_SIZE], val100[MONY_BUF_SIZE];
		CString str50 = _T("50");
		CString str100 = _T("100");
		m_pArith->l_input(&val50, str50.GetBuffer());
		m_pArith->l_input(&val100, str100.GetBuffer());
		l_pardotnet(&val[0], KSHIIRE_6[0], val50, val100);
	}
    else {
		l_pardotnet(&val[0], KSHIIRE_6[0], KSHIIRE_2[0], KSHIIRE_1[0]);
	}

//    if( m_pArith->l_test(val) ) {
		m_pArith->l_mul(&val, &val, val624);
		m_pArith->l_div(&val, &val, val100);
		m_pArith->l_div(&val, &val, val108);
		memset(&val2, 0, sizeof(val2));
		memmove(&val2, pSirewariRec->sirehenkannkei8, sizeof(val2));
		m_pArith->l_mul(&val2, &val2, val624);
		m_pArith->l_div(&val2, &val2, val100);
		m_pArith->l_div(&val2, &val2, val108);
		m_pArith->l_sub(&val, &val, &val2);
//	}
	memmove(KSHIIRE_7[0], &val, MONY_BUF_SIZE);

	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
		m_pArith->l_add(&KSHIIRE_7_KEI, &KSHIIRE_7_KEI, KSHIIRE_7[idx_jigyokbn]);
	}

	memset(&val, 0, sizeof(val));

	if (Get50CheckStatusColumn0_5_1()) {
		char val50[MONY_BUF_SIZE], val100[MONY_BUF_SIZE];
		CString str50 = _T("50");
		CString str100 = _T("100");
		m_pArith->l_input(&val50, str50.GetBuffer());
		m_pArith->l_input(&val100, str100.GetBuffer());
		l_pardotnet(&val[0], KSHIIRE_6[0], val50, val100);
	}
	else {
		l_pardotnet(&val[0], KSHIIRE_6[0], KSHIIRE_2[0], KSHIIRE_1[0]);
	}

    m_pArith->l_sub(&val, KSHIIRE_6[0], &val);
	m_pArith->l_mul(&val, &val, val780);
	m_pArith->l_div(&val, &val, val100);
	m_pArith->l_div(&val, &val, val110);

    //if( m_pArith->l_test(val) ) {
		memset(&val2, 0, sizeof(val2));
		memmove(&val2, pSirewariRec->sirehenkann10, sizeof(val2));
		m_pArith->l_mul(&val2, &val2, val780);
		m_pArith->l_div(&val2, &val2, val100);
		m_pArith->l_div(&val2, &val2, val110);
		m_pArith->l_sub(&val, &val, &val2);
	//}

	memmove(KSHIIRE_8[0], &val, MONY_BUF_SIZE);

	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
		m_pArith->l_add(&KSHIIRE_8_KEI, &KSHIIRE_8_KEI, KSHIIRE_8[idx_jigyokbn]);
	}

	memset(&val, 0, sizeof(val));
    m_pArith->l_add(&val, pSirewariRec->tanaoroshikasankei8, pSirewariRec->tanaoroshigensankei8);
	memmove(KSHIIRE_9, val, MONY_BUF_SIZE);

	memset(&val, 0, sizeof(val));
	m_pArith->l_add(&val, pSirewariRec->tanaoroshikasan10, pSirewariRec->tanaoroshigensan10);
	memmove(KSHIIRE_10, val, MONY_BUF_SIZE);

	memset(&val, 0, sizeof(val));
	m_pArith->l_add(&val, &val, pSirewariRec->tksire10);
	m_pArith->l_sub(&val, &val, pSirewariRec->tksirehenkann10);
    //この欄は税抜きにする.
	m_pArith->l_mul(&val, &val, val100);
	m_pArith->l_div(&val, &val, val110);
	memmove(KSHIIRE_13_2, &val, MONY_BUF_SIZE);

	memset(&val, 0, sizeof(val));
	memmove(&val, KSHIIRE_13_2, MONY_BUF_SIZE);
	m_pArith->l_mul(&val, &val, val780);
	m_pArith->l_div(&val, &val, val100);
	m_pArith->l_div(&val, &val, val100);
    memmove(KSHIIRE_14_2, &val, MONY_BUF_SIZE);

    Update_KSHIIRE_18();    

	memset(&val, 0, sizeof(val));
	m_pArith->l_add(&val, &val, &KSHIIRE_7_KEI);
	m_pArith->l_add(&val, &val, &KSHIIRE_9);
	m_pArith->l_add(&val, &val, &KSHIIRE_17);
	memmove(KSHIIRE_19, val, MONY_BUF_SIZE);

	memset(&val, 0, sizeof(val));
	m_pArith->l_add(&val, &val, &KSHIIRE_8_KEI);
	m_pArith->l_add(&val, &val, &KSHIIRE_10);
	m_pArith->l_add(&val, &val, &KSHIIRE_18);
	memmove(KSHIIRE_20, val, MONY_BUF_SIZE);

	// 連動時(編集＝チェックオフ)、事業区分名をテーブルから再取得
	if( m_TokureiMode == TOKUREIMODE_RENDO ){
		CString filter;
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d and itmId = 'CWB00030'"), m_SnSeq, ICS_SN_TOKUREI_3_HYOID, TOKUREI_TBL_VERSION);
		ReadData(m_SnSeq, TOKUREI_TBL_VERSION, filter );
	}

	Set_KSHIIRE_data();

	memmove(KSHIIRE_sirehenkann10[0], pSirewariRec->sirehenkann10, MONY_BUF_SIZE);
	memmove(KSHIIRE_sirehenkannkei8[0], pSirewariRec->sirehenkannkei8, MONY_BUF_SIZE);

    return;
}

//-----------------------------------------------------------------------------
// 特例テーブルバージョンチェック.
//-----------------------------------------------------------------------------
// 引数     sn_seq:履歴シーケンス
// 返送値	0：正常終了 -1：エラー
//-----------------------------------------------------------------------------
int CShinKshiire::TokureiTblVersionCheck(int sn_seq)
{
	CString filter;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TOKUREI_3_HYOID, TOKUREI_TBL_OLD_VERSION);

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
		sql.Format(_T("DELETE syz_tokurei WHERE version = %d and hyoId = \'%s\'"), TOKUREI_TBL_VERSION, ICS_SN_TOKUREI_3_HYOID);
		m_pZmSub->m_database->ExecuteSQL(sql);
		sql.Empty();
		sql.Format(_T("UPDATE syz_tokurei SET version = %d WHERE sn_seq = %d and hyoId = \'%s\'"), TOKUREI_TBL_VERSION, sn_seq, ICS_SN_TOKUREI_3_HYOID);
		m_pZmSub->m_database->ExecuteSQL(sql);

        if( TOKUREI_TBL_OLD_VERSION == 2 ) {
            //3の手入力サイン追加.
            TokureiTblV2ToV3(pSyzTokurei, sn_seq);
		}
	}

	return 0;
}

void CShinKshiire::TokureiTblV2ToV3(CSYZ_TOKUREI* pTokurei, int sn_seq)
{
	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
		pTokurei->AddNew();
		pTokurei->sn_seq = sn_seq;
		pTokurei->version = TOKUREI_TBL_VERSION;
		pTokurei->hyoId = ICS_SN_TOKUREI_3_HYOID;
		pTokurei->itemId = _T("CWB00065");
		pTokurei->idx = idx_jigyokbn;
		pTokurei->kind = DATAKIND_SIGN;
		pTokurei->sign1 = 0;
		pTokurei->Update();
	}
}

//-----------------------------------------------------------------------------
// データの読込み
//-----------------------------------------------------------------------------
// 引数     sn_seq:履歴シーケンス version:バージョン
// 返送値	0：正常終了 -1：エラー
//-----------------------------------------------------------------------------
int CShinKshiire::ReadData(int sn_seq, int version, CString filter)
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

int CShinKshiire::ReadSub_data(CSYZ_TOKUREI* pTokurei)
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
				}
				else if (pTokurei->kind == DATAKIND_RATE) {
					CString ratestr;
					ratestr.Format(_T("%d"), pTokurei->rate);
					m_SyzUtil.val_to_bin(
						(unsigned char*)(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn),
						ratestr);
				}
				else if (pTokurei->kind == DATAKIND_SIGN) {
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
				if( (datainfo[tblidx].diagidx[0] == ID_KSHIIRE_13_2) || (datainfo[tblidx].diagidx[0] == ID_KSHIIRE_14_2) ){
					if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){	// 特定課税仕入れ
						m_SyzUtil.val_to_bin((unsigned char*)datainfo[tblidx].data, pTokurei->val);
					}
					else{
						memset( datainfo[tblidx].data, '\0', datainfo[tblidx].size );
					}
				}
				else{
					m_SyzUtil.val_to_bin((unsigned char*)datainfo[tblidx].data, pTokurei->val);
				}
			}
			else if (pTokurei->kind == DATAKIND_RATE) {
				CString ratestr;
				ratestr.Format(_T("%d"), pTokurei->rate);
				m_SyzUtil.val_to_bin((unsigned char*)datainfo[tblidx].data, ratestr);
			}
			else if (pTokurei->kind == DATAKIND_SIGN) {
				*(datainfo[tblidx].data) = pTokurei->sign1;
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
int CShinKshiire::WriteData(int sn_seq, int version)
{
	ASSERT(m_pZmSub);
	if (m_pZmSub == NULL) {
		return -1;
	}

	CString filter;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TOKUREI_3_HYOID, TOKUREI_TBL_VERSION);

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

void CShinKshiire::WriteSub_data(CSYZ_TOKUREI* pTokurei, int sn_seq, int version)
{
	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {

			pTokurei->AddNew();
			pTokurei->sn_seq = sn_seq;
			pTokurei->version = version;
			pTokurei->hyoId = ICS_SN_TOKUREI_3_HYOID;
			pTokurei->itemId = kbndatainfo[tblidx].itemId;
			pTokurei->idx = idx_jigyokbn;
			pTokurei->kind = kbndatainfo[tblidx].kind;
			pTokurei->sign1 = 0;

			if (pTokurei->kind == DATAKIND_STR) {
				pTokurei->str = kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn;
			}
			else if (pTokurei->kind == DATAKIND_VAL) {
				m_SyzUtil.val_to_asci(&pTokurei->val,
					(unsigned char*)(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn));
			}
			else if (pTokurei->kind == DATAKIND_RATE) {
				CString ratestr;
				m_SyzUtil.val_to_asci(&ratestr,
					(unsigned char*)(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn));
				pTokurei->rate = _ttoi(ratestr);
			}
			else if (pTokurei->kind == DATAKIND_SIGN) {
				pTokurei->sign1 = *(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn);
			}

			pTokurei->Update();
		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {

		pTokurei->AddNew();
		pTokurei->sn_seq = sn_seq;
		pTokurei->version = version;
		pTokurei->hyoId = ICS_SN_TOKUREI_3_HYOID;
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

int CShinKshiire::Update_data(CSYZ_TOKUREI* pTokurei)
{
	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {

			if ((pTokurei->itemId == kbndatainfo[tblidx].itemId) && (pTokurei->idx == idx_jigyokbn)) {
				pTokurei->Edit();

				if (pTokurei->kind == DATAKIND_STR) {
					pTokurei->str = kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn;
				}
				else if (pTokurei->kind == DATAKIND_VAL) {
					m_SyzUtil.val_to_asci(&pTokurei->val,
						(unsigned char*)(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn));
				}
				else if (pTokurei->kind == DATAKIND_RATE) {
					CString ratestr;
					m_SyzUtil.val_to_asci(&ratestr,
						(unsigned char*)(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn));
					pTokurei->rate = _ttoi(ratestr);
				}
				else if (pTokurei->kind == DATAKIND_SIGN) {
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

int CShinKshiire::GetMxPrtPage()
{
	int PageNum = 1;

	if (m_pDBSyzAccess->m_pSyz_Tokurei == NULL) {
		return PageNum;
	}

	CString sq, subsq;
	CSYZ_TOKUREI* pSyzTokurei = m_pDBSyzAccess->m_pSyz_Tokurei;

	for (int page = 2; page <= KSHIIRE_PRTPAGE_MAX; page++) {

		sq.Empty();
		subsq.Empty();

		subsq.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d "), 0, ICS_SN_TOKUREI_3_HYOID, TOKUREI_TBL_VERSION);
		sq += subsq;
		subsq.Format(_T("and %d <= idx and idx <= %d "), (page - 1)*JIGYO_PER_PAGE_SHIIRE, page*JIGYO_PER_PAGE_SHIIRE - 1);
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
void CShinKshiire::DispKshiire()
{
	// 画面初期化
	for (int idx = KSHIIRE_DIAGCTRLID_NAME; idx <= KSHIIRE_DIAGCTRLID_TEKIYO_E; (short)idx++) {
		dsp_cls(IDC_ICSDIAG8CTRL2, idx);
	}
	for (int idx = ID_KSHIIRE_TITLE1; idx <= ID_KSHIIRE_20; idx++) {
		dsp_cls(IDC_ICSDIAG8CTRL2, (short)idx);
	}
	
	DispClmNum();
	SetKazeikikan();
	SetTekiyoTaisyokikan();
	SetName();
	Set_KSHIIRE_data();
}

//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//		Mony	：	表示金額
//-----------------------------------------------------------------------------
void CShinKshiire::dsp_prs(unsigned short Id, short Index, char *Mony)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	memmove(DiagData.data_val, Mony, MONY_BUF_SIZE);
	diag_setdata(Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl);
}

//-----------------------------------------------------------------------------
// 金額表示クリア
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//-----------------------------------------------------------------------------
void CShinKshiire::dsp_cls(unsigned short Id, short Index)
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch (Index) {
	case ID_KSHIIRE_TITLE1:
	case ID_KSHIIRE_TITLE2:
	case ID_KSHIIRE_3_1:
    case ID_KSHIIRE_3_2:
		diag_clear(Id, Index, TRUE, CIcsdiagctrl);
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
void CShinKshiire::GetAllDiagData()
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	for (int tblidx = 0; tblidx < (sizeof(kbndatainfo) / sizeof(kbndatainfo[0])); tblidx++) {
		for (int clmidx = 0; clmidx < JIGYO_PER_PAGE_SHIIRE; clmidx++) {

			diag_getdata(IDC_ICSDIAG8CTRL2, kbndatainfo[tblidx].diagidx[clmidx],
				(struct IUnknown*)&DiagData, CIcsdiagctrl);

			if (kbndatainfo[tblidx].kind == DATAKIND_VAL) {

				if ((kbndatainfo[tblidx].itemId.Compare(_T("CWB00116")) != 0)
					&& (kbndatainfo[tblidx].itemId.Compare(_T("CWB00126")) != 0)) {

					memmove(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size,
						DiagData.data_val, kbndatainfo[tblidx].size);
				}
			}
			else if (kbndatainfo[tblidx].kind == DATAKIND_STR) {

				memmove(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size,
					DiagData.data_edit.GetBuffer(), kbndatainfo[tblidx].size);

			}
			else if (kbndatainfo[tblidx].kind == DATAKIND_RATE) {
				m_SyzUtil.val_to_bin((unsigned char*)kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size, DiagData.data_edit);
			}
		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {

		if ( (datainfo[tblidx].itemId.Compare(_T("CWD00030")) != 0)
			&& (datainfo[tblidx].itemId.Compare(_T("CWD00040")) != 0) ) {

			diag_getdata(IDC_ICSDIAG8CTRL2, datainfo[tblidx].diagidx[0], (struct IUnknown*)&DiagData, CIcsdiagctrl);
			if (datainfo[tblidx].kind == DATAKIND_VAL) {
				memmove(datainfo[tblidx].data, DiagData.data_val, MONY_BUF_SIZE);
			}
		}

	}

}

void CShinKshiire::ClearData()
{
	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
			memset(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn,
				'\0', kbndatainfo[tblidx].size);
		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {
		memset(datainfo[tblidx].data, '\0', datainfo[tblidx].size);
	}

}

//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinKshiire::UpDownFocusMoveCheck(int Index, int Move)
{
	int nextFocusIdx = 0;
	int pos = 0;
	DIAGRAM_ATTRIBUTE	DA;

	// 現在ポジション検索
	for (pos = 0; pos < (sizeof(FCS_Kshiire)/sizeof(FCS_Kshiire[0])); pos++) {
		if (Index == FCS_Kshiire[pos].IDNo) {
			break;
		}
	}

	nextFocusIdx = FCS_Kshiire[pos].IDNo;	// 現在ポジションセット

	// 次ポジション検索
	if (Move == MOVE_DOWN) {	// 下方向
		pos++;
		for (int j = pos; j < (sizeof(FCS_Kshiire) / sizeof(FCS_Kshiire[0])); j++) {
			m_KshiireDiag02.GetAttribute(FCS_Kshiire[j].IDNo, (LPUNKNOWN)&DA);
			if ((DA.attr_bcolor == BC_CREAM) || (DA.attr_bcolor == BC_BLUE)) {
				nextFocusIdx = FCS_Kshiire[j].IDNo;
				break;
			}
		}
	}
	else if (Move == MOVE_UP) {	// 上方向
		pos--;
		for (int j = pos; j > -1; j--) {
			m_KshiireDiag02.GetAttribute(FCS_Kshiire[j].IDNo, (LPUNKNOWN)&DA);
			if ((DA.attr_bcolor == BC_CREAM) || (DA.attr_bcolor == BC_BLUE)) {
				nextFocusIdx = FCS_Kshiire[j].IDNo;
				break;
			}
		}
	}

	return nextFocusIdx;
}


//-----------------------------------------------------------------------------
// 課税期間
//-----------------------------------------------------------------------------
void CShinKshiire::SetKazeikikan()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char yy, mm, dd;
	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd);
	DiagData.data_disp.Format(_T("%02x.%02x.%02x"), (yy & 0xff), (mm & 0xff), (dd & 0xff));
	diag_setdata(IDC_ICSDIAG8CTRL2, KSHIIRE_DIAGCTRLID_KAZEI_S, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd);
	DiagData.data_disp.Format(_T("%02x.%02x.%02x"), (yy & 0xff), (mm & 0xff), (dd & 0xff));
	diag_setdata(IDC_ICSDIAG8CTRL2, KSHIIRE_DIAGCTRLID_KAZEI_E, (struct IUnknown *)&DiagData, CIcsdiagctrl);
}

//-----------------------------------------------------------------------------
// 適用対象期間
//-----------------------------------------------------------------------------
void CShinKshiire::SetTekiyoTaisyokikan()
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
	diag_setdata(IDC_ICSDIAG8CTRL2, KSHIIRE_DIAGCTRLID_TEKIYO_S, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	if ((TOKUREI_KIKAN_S <= m_tekiyotaisyokikan_e) && (m_tekiyotaisyokikan_e <= TOKUREI_KIKAN_E)) {
		voldate.db_datecnvGen(s_gengo, m_tekiyotaisyokikan_e, &d_gengo, &tekiyotaisyokikan_e, 0, 0);
	}
	else if ((m_tekiyotaisyokikan_s <= TOKUREI_KIKAN_E) && (TOKUREI_KIKAN_E <= m_tekiyotaisyokikan_e)) {
		voldate.db_datecnvGen(s_gengo, TOKUREI_KIKAN_E, &d_gengo, &tekiyotaisyokikan_e, 0, 0);
	}
	tmpstr.Format(_T("%08d"), tekiyotaisyokikan_e);
	DiagData.data_disp.Format(_T("%02s.%02s.%02s"), tmpstr.Mid(2, 2), tmpstr.Mid(4, 2), tmpstr.Mid(6, 2));
	diag_setdata(IDC_ICSDIAG8CTRL2, KSHIIRE_DIAGCTRLID_TEKIYO_E, (struct IUnknown *)&DiagData, CIcsdiagctrl);

}

void CShinKshiire::SetName()
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
	diag_setdata(IDC_ICSDIAG8CTRL2, KSHIIRE_DIAGCTRLID_NAME, (struct IUnknown *)&DiagData, CIcsdiagctrl);
}

void CShinKshiire::Set_KSHIIRE_data()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char buf[64] = { 0 };
	char val[6] = { 0 };

	for (int tblidx = 0; tblidx < (sizeof(kbndatainfo) / sizeof(kbndatainfo[0])); tblidx++) {
		for (int idx = 0; idx < JIGYO_PER_PAGE_SHIIRE; idx++) {

			if (kbndatainfo[tblidx].kind == DATAKIND_STR) {

				DiagData.data_edit.Empty();
				memset(buf, '\0', sizeof(buf));
				memmove(buf, kbndatainfo[tblidx].data + (m_dispCol_left + idx) * kbndatainfo[tblidx].size, kbndatainfo[tblidx].size);
				DiagData.data_edit = buf;
				m_KshiireDiag02.SetData(kbndatainfo[tblidx].diagidx[idx], (struct IUnknown*)&DiagData);
				SetSub_KSHIIRE_InputLockColor(kbndatainfo[tblidx].diagidx[idx]);
			}
			else if (kbndatainfo[tblidx].kind == DATAKIND_VAL) {

				if ((kbndatainfo[tblidx].itemId.Compare(_T("CWB00116")) != 0)
					&& (kbndatainfo[tblidx].itemId.Compare(_T("CWB00126")) != 0)) {
					
					dsp_prs(IDC_ICSDIAG8CTRL2,
						kbndatainfo[tblidx].diagidx[idx],
						kbndatainfo[tblidx].data + (m_dispCol_left + idx) * kbndatainfo[tblidx].size);

				}

				SetSub_KSHIIRE_InputLockColor(kbndatainfo[tblidx].diagidx[idx]);

				if (kbndatainfo[tblidx].itemId.Compare(_T("CWB00110")) == 0) {
					CheckSetData_KSHIIRE_7(idx_kshiire_7[idx], idx);
					SetSub_KSHIIRE_InputLockColor(idx_kshiire_7[idx]);
				}
                else if ( kbndatainfo[tblidx].itemId.Compare(_T("CWB00120")) == 0) {
					CheckSetData_KSHIIRE_8(idx_kshiire_8[idx], idx);
					SetSub_KSHIIRE_InputLockColor(idx_kshiire_8[idx]);
				}

			}
			else if (kbndatainfo[tblidx].kind == DATAKIND_RATE) {

				memset(&val, 0, sizeof(val));
				memmove(val, kbndatainfo[tblidx].data + (m_dispCol_left + idx) * kbndatainfo[tblidx].size, kbndatainfo[tblidx].size);
				m_pArith->l_print(val, val, _T("sss9"));
				DiagData.data_edit = val;
				diag_setdata(IDC_ICSDIAG8CTRL2, kbndatainfo[tblidx].diagidx[idx], (struct IUnknown*)&DiagData, CIcsdiagctrl);
				if ( (kbndatainfo[tblidx].itemId.Compare(_T("CWB00060")) == 0) && ((*m_pSnHeadData)->m_DispTabSgn & 0x10) ) {
					CheckSetData_KSHIIRE_3(idx_kshiire_3[idx], idx);
				}
				SetSub_KSHIIRE_InputLockColor(kbndatainfo[tblidx].diagidx[idx]);

			}
		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {

        if(datainfo[tblidx].diagidx != NULL ) {
		    dsp_prs(IDC_ICSDIAG8CTRL2, datainfo[tblidx].diagidx[0], datainfo[tblidx].data);
			SetSub_KSHIIRE_InputLockColor(datainfo[tblidx].diagidx[0]);
		}

		if (datainfo[tblidx].itemId.Compare(_T("CWC00060")) == 0) {
			CheckSetData_KSHIIRE_121(idx_kshiire_12_1[0]);
			SetSub_KSHIIRE_InputLockColor(idx_kshiire_12_1[0]);
		}
		else if (datainfo[tblidx].itemId.Compare(_T("CWC00070")) == 0) {
			CheckSetData_KSHIIRE_122(idx_kshiire_12_2[0]);
			SetSub_KSHIIRE_InputLockColor(idx_kshiire_12_2[0]);
		}
		else if (datainfo[tblidx].itemId.Compare(_T("CWC00080")) == 0) {
			if (((*m_pSnHeadData)->Sn_Sign4 & 0x01)) { //入力ロック.
				SetSub_KSHIIRE_InputLockColor(idx_kshiire_13_2[0]);
				ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_kshiire_13_2[0], 0, (*m_pSnHeadData)->Sn_Sign4, 0);
			}
            else {
				if (!(*m_pSnHeadData)->IsNeedTkkzDisp()      //特定課税仕入有:OFF
					|| m_TokureiMode == TOKUREIMODE_RENDO) { //連動
					ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_kshiire_13_2[0], 0, (*m_pSnHeadData)->Sn_Sign4, 0);
				}
                else {
					ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_kshiire_13_2[0], 1, (*m_pSnHeadData)->Sn_Sign4, 0);
				}
			}
		}
		else if (datainfo[tblidx].itemId.Compare(_T("CWC00090")) == 0) {
			if (((*m_pSnHeadData)->Sn_Sign4 & 0x01)) { //入力ロック.
				SetSub_KSHIIRE_InputLockColor(idx_kshiire_14_2[0]);
				ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_kshiire_14_2[0], 0, (*m_pSnHeadData)->Sn_Sign4, 0);
			}
            else {
				if (!(*m_pSnHeadData)->IsNeedTkkzDisp()      //特定課税仕入有:OFF
					|| m_TokureiMode == TOKUREIMODE_RENDO) { //連動
					ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_kshiire_14_2[0], 0, (*m_pSnHeadData)->Sn_Sign4, 0);
				}
				else {
					ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_kshiire_14_2[0], 1, (*m_pSnHeadData)->Sn_Sign4, 0);
					CheckSetData_KSHIIRE_142(idx_kshiire_14_2[0]);
				}
			}
		}
	}

	if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
		//入力ロック中.
		return;
	}

	//yoshida190816
	//付表に反映されてなかったのを修正
	if (m_SyzUtil.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
		// 付表2-1 15D欄へ
		(*m_pSyzSyukei)->SetShinkokuData(_T("324002"), KSHIIRE_19);
		// 付表2-1 15E欄へ
		(*m_pSyzSyukei)->SetShinkokuData(_T("324003"), KSHIIRE_20);

		// 還付申告2 10欄へ
		// 転記用コードに、課税仕入れ等の税額の合計額から納税義務税額を除外した金額をセットする
		memset(&val, 0, sizeof(val));
		m_pArith->l_sub(&val, &KSHIIRE_19, &KSHIIRE_9);
		m_pArith->l_sub(&val, &val, &KSHIIRE_16_1);
		(*m_pSyzSyukei)->SetShinkokuData(_T("32A802"), val);
		memset(&val, 0, sizeof(val));
		m_pArith->l_sub(&val, &KSHIIRE_20, &KSHIIRE_10);
		m_pArith->l_sub(&val, &val, &KSHIIRE_16_2);
		(*m_pSyzSyukei)->SetShinkokuData(_T("32A803"), val);

		m_pShinInfo->pParent->PostMessage( WM_USER_RESET_REFUND, (WPARAM)1, (LPARAM)1 );
	}

}

void CShinKshiire::SetSub_KSHIIRE_InputLockColor(short index)
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
		if (ID_KSHIIRE_3_1 <= index && index <= ID_KSHIIRE_3_2) {
			DA.attr_bcolor = BC_WHITE;
		}
		else if (ID_KSHIIRE_6_1 <= index && index <= ID_KSHIIRE_6_2) {
			DA.attr_bcolor = BC_WHITE;
		}
		else if (index == ID_KSHIIRE_17 || index == ID_KSHIIRE_18 
			|| index == ID_KSHIIRE_19 || index == ID_KSHIIRE_20 ) {
			DA.attr_bcolor = BC_WHITE;
		}
		else {
             if( (index == ID_KSHIIRE_14_2) || (index == ID_KSHIIRE_13_2) ) {
				 if (!(*m_pSnHeadData)->IsNeedTkkzDisp()) {
				     DA.attr_bcolor = BC_WHITE;
				 }
                 else {
					 DA.attr_bcolor = BC_GRAY;
				 }
			 }
             else {
				 DA.attr_bcolor = BC_GRAY;
			 }
		}
		break;
	case TOKUREIMODE_RENDO:
		break;
	default:
		break;
	}

	diag_setattr(IDC_ICSDIAG8CTRL2, index, &DA, FALSE, CIcsdiagctrl);
}

void CShinKshiire::Update_KSHIIRE_3(short index, int clmidx, bool bDel)
{
	if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) && ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		// 第5-(1)号様式と併用しているかつ非連動の場合に手入力サインをチェック
		if (KSHIIRE_3_SIGN[m_dispCol_left + clmidx] && !bDel) {
			return;
		}
	}
	char kourikeigenwariai[MONY_BUF_SIZE];
	Calc_KSHIIRE_3(index, clmidx, &kourikeigenwariai[0]);
	memmove(KSHIIRE_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, kourikeigenwariai, MONY_BUF_SIZE);

	if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) && ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		// 第5-(1)号様式と併用しているかつ非連動の場合のみ背景色を変更する
		CheckSub_ChangeBackColor(idx_kshiire_3[clmidx], NULL, NULL, true);
	}
}

void CShinKshiire::Update_KSHIIRE_6(short index, int clmidx)
{
    memset(KSHIIRE_6[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, 0, sizeof(MONY_BUF_SIZE));
    m_pArith->l_add(KSHIIRE_6[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
		KSHIIRE_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
		KSHIIRE_5[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);

}

void CShinKshiire::Update_KSHIIRE_7(short index, int clmidx, bool bDel)
{
    if( KSHIIRE_7_SIGN[m_dispCol_left + clmidx] && !bDel ) {
        return;
	}

	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSHIIRE_7(index, clmidx, &val[0]);
	memmove(KSHIIRE_7[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	CheckSub_ChangeBackColor(idx_kshiire_7[clmidx], NULL, NULL, true);
}

void CShinKshiire::Update_KSHIIRE_7_KEI(short index, int clmidx)
{
    memset(&KSHIIRE_7_KEI, 0, sizeof(KSHIIRE_7_KEI));
	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
		m_pArith->l_add(&KSHIIRE_7_KEI, &KSHIIRE_7_KEI, KSHIIRE_7[idx_jigyokbn]);
	}
}

void CShinKshiire::Update_KSHIIRE_8(short index, int clmidx, bool bDel)
{

	if (KSHIIRE_8_SIGN[m_dispCol_left + clmidx] && !bDel) {
		return;
	}

	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSHIIRE_8(index, clmidx, &val[0]);
	memmove(KSHIIRE_8[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	CheckSub_ChangeBackColor(idx_kshiire_8[clmidx], NULL, NULL, true);
}

void CShinKshiire::Update_KSHIIRE_8_KEI(short index, int clmidx)
{
	memset(&KSHIIRE_8_KEI, 0, sizeof(KSHIIRE_8_KEI));
	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_23; idx_jigyokbn++) {
		m_pArith->l_add(&KSHIIRE_8_KEI, &KSHIIRE_8_KEI, KSHIIRE_8[idx_jigyokbn]);
	}
}

void CShinKshiire::Update_KSHIIRE_121(short index, bool bDel)
{

	if (KSHIIRE_12_1_SIGN && !bDel) {
		return;
	}

	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSHIIRE_121(index, &val[0]);
	memmove(KSHIIRE_12_1, &val, MONY_BUF_SIZE);
	CheckSub_ChangeBackColor(idx_kshiire_12_1[0], NULL, NULL, true);
}

void CShinKshiire::Update_KSHIIRE_122(short index, bool bDel)
{

	if (KSHIIRE_12_2_SIGN && !bDel) {
		return;
	}

	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSHIIRE_122(index, &val[0]);
	memmove(KSHIIRE_12_2, &val, MONY_BUF_SIZE);
	CheckSub_ChangeBackColor(idx_kshiire_12_2[0], NULL, NULL, true);
}

void CShinKshiire::Update_KSHIIRE_142(short index, bool bDel)
{

	if (KSHIIRE_14_2_SIGN && !bDel) {
		return;
	}

	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSHIIRE_142(index, &val[0]);
	memmove(KSHIIRE_14_2, &val, MONY_BUF_SIZE);
	CheckSub_ChangeBackColor(idx_kshiire_14_2[0], NULL, NULL, true);

}

void CShinKshiire::Update_KSHIIRE_17()
{
	char val[MONY_BUF_SIZE] = { 0 };
	memset(&val, 0, sizeof(val));
	m_pArith->l_add(&val, &val, KSHIIRE_12_1);
	m_pArith->l_add(&val, &val, KSHIIRE_15_1);
	m_pArith->l_add(&val, &val, KSHIIRE_16_1);
	memmove(KSHIIRE_17, &val, MONY_BUF_SIZE);
}

void CShinKshiire::Update_KSHIIRE_18()
{
	char val[MONY_BUF_SIZE] = { 0 };
	memset(&val, 0, sizeof(val));
	m_pArith->l_add(&val, &val, KSHIIRE_12_2);
	m_pArith->l_add(&val, &val, KSHIIRE_14_2);
	m_pArith->l_add(&val, &val, KSHIIRE_15_2);
	m_pArith->l_add(&val, &val, KSHIIRE_16_2);
	memmove(KSHIIRE_18, &val, MONY_BUF_SIZE);
}

void CShinKshiire::Update_KSHIIRE_19()
{
	char val[MONY_BUF_SIZE] = { 0 };
	memset(&val, 0, sizeof(val));
	m_pArith->l_add(&val, &val, KSHIIRE_7_KEI);
	m_pArith->l_add(&val, &val, KSHIIRE_9);
	m_pArith->l_add(&val, &val, KSHIIRE_17);
	memmove(KSHIIRE_19, &val, MONY_BUF_SIZE);
}

void CShinKshiire::Update_KSHIIRE_20()
{
	char val[MONY_BUF_SIZE] = { 0 };
	memset(&val, 0, sizeof(val));
	m_pArith->l_add(&val, &val, KSHIIRE_8_KEI);
	m_pArith->l_add(&val, &val, KSHIIRE_10);
	m_pArith->l_add(&val, &val, KSHIIRE_18);
	memmove(KSHIIRE_20, &val, MONY_BUF_SIZE);
}

void CShinKshiire::Calc_KSHIIRE_3(short index, int clmidx, char* pVal)
{
	char val_kshiire1[MONY_BUF_SIZE], val_kshiire2[MONY_BUF_SIZE];

	memmove(val_kshiire1, KSHIIRE_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);
	memmove(val_kshiire2, KSHIIRE_2[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);

	if ((m_pArith->l_test(val_kshiire2) > 0) && (m_pArith->l_test(val_kshiire1) < 0)) {
		CString str100 = _T("100");
		m_pArith->l_input(pVal, str100.GetBuffer());
	}
	else if ((m_pArith->l_test(val_kshiire2) < 0) && (m_pArith->l_test(val_kshiire1) > 0)) {
		memset(pVal, '\0', MONY_BUF_SIZE);
	}
	else if (m_pArith->l_test(val_kshiire1) && m_pArith->l_test(val_kshiire2)) {
		m_pArith->l_par100((unsigned char*)pVal, (unsigned char*)val_kshiire2, (unsigned char*)val_kshiire1, 0, 0);
	}
	else {
		memset(pVal, '\0', MONY_BUF_SIZE);
	}

}

void CShinKshiire::Calc_KSHIIRE_7(short index, int clmidx, char* pVal)
{

	char val624[MONY_BUF_SIZE];
	char val100[MONY_BUF_SIZE], val108[MONY_BUF_SIZE];
	CString str624 = _T("624");
	CString str100 = _T("100");
	CString str108 = _T("108");

	m_pArith->l_input(&val624, str624.GetBuffer());
	m_pArith->l_input(&val100, str100.GetBuffer());
	m_pArith->l_input(&val108, str108.GetBuffer());

	memset(pVal, 0, MONY_BUF_SIZE);

	if ( ((*m_pSnHeadData)->m_DispTabSgn & 0x10)
	    && (m_TokureiMode == TOKUREIMODE_HIRENDO) 
		&& KSHIIRE_3_SIGN[m_dispCol_left + clmidx]) {

		char val100[MONY_BUF_SIZE];
		CString str100 = _T("100");
		m_pArith->l_input(&val100, str100.GetBuffer());

		l_pardotnet(pVal,
			KSHIIRE_6[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			KSHIIRE_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			val100);

	}
    else {

		l_pardotnet(pVal,
			KSHIIRE_6[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			KSHIIRE_2[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			KSHIIRE_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);

	}

	m_pArith->l_mul(pVal, pVal, val624);
	m_pArith->l_div(pVal, pVal, val108);
	m_pArith->l_div(pVal, pVal, val100);

	char val[MONY_BUF_SIZE];
	memset(&val, 0, sizeof(val));
	memmove(&val, KSHIIRE_sirehenkannkei8[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, sizeof(val));

	m_pArith->l_mul(&val, &val, val624);
	m_pArith->l_div(&val, &val, val100);
	m_pArith->l_div(&val, &val, val108);
	m_pArith->l_sub(pVal, pVal, &val);

}

void CShinKshiire::Calc_KSHIIRE_8(short index, int clmidx, char* pVal)
{
	char val780[MONY_BUF_SIZE];
	char val100[MONY_BUF_SIZE], val110[MONY_BUF_SIZE];
	CString str780 = _T("780");
	CString str100 = _T("100");
	CString str110 = _T("110");

	m_pArith->l_input(&val780, str780.GetBuffer());
	m_pArith->l_input(&val100, str100.GetBuffer());
	m_pArith->l_input(&val110, str110.GetBuffer());

	memset(pVal, 0, MONY_BUF_SIZE);

	if (((*m_pSnHeadData)->m_DispTabSgn & 0x10)
		&& (m_TokureiMode == TOKUREIMODE_HIRENDO)
		&& KSHIIRE_3_SIGN[m_dispCol_left + clmidx]) {

		char val100[MONY_BUF_SIZE];
		CString str100 = _T("100");
		m_pArith->l_input(&val100, str100.GetBuffer());

		l_pardotnet(pVal,
			KSHIIRE_6[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			KSHIIRE_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			val100);

	}
    else {
		l_pardotnet(pVal,
			KSHIIRE_6[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			KSHIIRE_2[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			KSHIIRE_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);
	}

    m_pArith->l_sub(pVal, KSHIIRE_6[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, pVal);
	m_pArith->l_mul(pVal, pVal, val780);
	m_pArith->l_div(pVal, pVal, val100);
	m_pArith->l_div(pVal, pVal, val110);

	char val[MONY_BUF_SIZE];
	memset(&val, 0, sizeof(val));
	memmove(&val, KSHIIRE_sirehenkann10[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, sizeof(val));
	m_pArith->l_mul(&val, &val, val780);
	m_pArith->l_div(&val, &val, val100);
	m_pArith->l_div(&val, &val, val110);
	m_pArith->l_sub(pVal, pVal, &val);

}

void CShinKshiire::Calc_KSHIIRE_121(short index, char* pVal)
{
	char val624[MONY_BUF_SIZE];
	char val100[MONY_BUF_SIZE], val108[MONY_BUF_SIZE];
	CString str624 = _T("624");
	CString str100 = _T("100");
	CString str108 = _T("108");
	m_pArith->l_input(&val624, str624.GetBuffer());
	m_pArith->l_input(&val100, str100.GetBuffer());
	m_pArith->l_input(&val108, str108.GetBuffer());

	m_pArith->l_mul(pVal, &KSHIIRE_11_1, val624);
	m_pArith->l_div(pVal, pVal, val100);
	m_pArith->l_div(pVal, pVal, val108);
}

void CShinKshiire::Calc_KSHIIRE_122(short index, char* pVal)
{
	char val780[MONY_BUF_SIZE];
	char val100[MONY_BUF_SIZE], val110[MONY_BUF_SIZE];
	CString str780 = _T("780");
	CString str100 = _T("100");
	CString str110 = _T("110");
	m_pArith->l_input(&val780, str780.GetBuffer());
	m_pArith->l_input(&val100, str100.GetBuffer());
	m_pArith->l_input(&val110, str110.GetBuffer());

	m_pArith->l_mul(pVal, &KSHIIRE_11_2, val780);
	m_pArith->l_div(pVal, pVal, val100);
	m_pArith->l_div(pVal, pVal, val110);
}

void CShinKshiire::Calc_KSHIIRE_142(short index, char* pVal)
{
	char val780[MONY_BUF_SIZE];
	char val100[MONY_BUF_SIZE];
	CString str780 = _T("780");
	CString str100 = _T("100");
	m_pArith->l_input(&val780, str780.GetBuffer());
	m_pArith->l_input(&val100, str100.GetBuffer());

	m_pArith->l_mul(pVal, &KSHIIRE_13_2, val780);
	m_pArith->l_div(pVal, pVal, val100);
	m_pArith->l_div(pVal, pVal, val100);

}

void CShinKshiire::CheckSetData_KSHIIRE_3(short index, int clmidx)
{
	//入力値取得.
    char val[MONY_BUF_SIZE];
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	diag_getdata(IDC_ICSDIAG8CTRL2, idx_kshiire_3[clmidx], (struct IUnknown*)&DiagData, CIcsdiagctrl);
	m_SyzUtil.val_to_bin((unsigned char*)val, DiagData.data_edit);

	//計算値取得.
	char val2[MONY_BUF_SIZE] = { 0 };
	Calc_KSHIIRE_3(index, clmidx, &val2[0]);

	if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) && ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		// 第5-(1)号様式と併用しているかつ非連動の場合のみ手入力サインをチェック
		KSHIIRE_3_SIGN[m_dispCol_left + clmidx] = CheckSub_ChangeBackColor(idx_kshiire_3[clmidx], val, val2);
	}

}

void CShinKshiire::CheckSetData_KSHIIRE_7(short index, int clmidx)
{
	//入力値取得.
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	diag_getdata(IDC_ICSDIAG8CTRL2, idx_kshiire_7[clmidx], (struct IUnknown*)&DiagData, CIcsdiagctrl);

	//計算値取得.
	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSHIIRE_7(index, clmidx, &val[0]);

	KSHIIRE_7_SIGN[m_dispCol_left + clmidx] = CheckSub_ChangeBackColor(idx_kshiire_7[clmidx], DiagData.data_val, val);
}

void CShinKshiire::CheckSetData_KSHIIRE_8(short index, int clmidx)
{
	//入力値取得.
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	diag_getdata(IDC_ICSDIAG8CTRL2, idx_kshiire_8[clmidx], (struct IUnknown*)&DiagData, CIcsdiagctrl);

	//計算値取得.
	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSHIIRE_8(index, clmidx, &val[0]);

	KSHIIRE_8_SIGN[m_dispCol_left + clmidx] = CheckSub_ChangeBackColor(idx_kshiire_8[clmidx], DiagData.data_val, val);
}

void CShinKshiire::CheckSetData_KSHIIRE_121(short index)
{
	//入力値取得.
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	diag_getdata(IDC_ICSDIAG8CTRL2, idx_kshiire_12_1[0], (struct IUnknown*)&DiagData, CIcsdiagctrl);

	//計算値取得.
	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSHIIRE_121(index, &val[0]);
	KSHIIRE_12_1_SIGN = CheckSub_ChangeBackColor(idx_kshiire_12_1[0], DiagData.data_val, val);

}

void CShinKshiire::CheckSetData_KSHIIRE_122(short index)
{
	//入力値取得.
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	diag_getdata(IDC_ICSDIAG8CTRL2, idx_kshiire_12_2[0], (struct IUnknown*)&DiagData, CIcsdiagctrl);

	//計算値取得.
	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSHIIRE_122(index, &val[0]);

	KSHIIRE_12_2_SIGN = CheckSub_ChangeBackColor(idx_kshiire_12_2[0], DiagData.data_val, val);
}


void CShinKshiire::CheckSetData_KSHIIRE_142(short index)
{
	//入力値取得.
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	diag_getdata(IDC_ICSDIAG8CTRL2, idx_kshiire_14_2[0], (struct IUnknown*)&DiagData, CIcsdiagctrl);

	//計算値取得.
	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSHIIRE_142(index, &val[0]);

	KSHIIRE_14_2_SIGN = CheckSub_ChangeBackColor(idx_kshiire_14_2[0], DiagData.data_val, val);
}

int CShinKshiire::CheckSub_ChangeBackColor(short index, char* pVal1, char* pVal2, bool bReset)
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
			if (m_pArith->l_cmp(pVal1, pVal2)) {
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

bool CShinKshiire::Get50CheckStatusColumn0_5_1()
{
    bool rt = false;

	//5-(1)号様式の1列目の□50%適用チェックボックスの状態を取得.
	CString filter;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d and itmId = 'CUB00105' and idx = %d"), m_SnSeq, ICS_SN_TOKUREI_1_HYOID, TOKUREI_TBL_VERSION, 0);

	if (m_pDBSyzAccess->m_pSyz_Tokurei) {
		m_pDBSyzAccess->m_pSyz_Tokurei->Requery(filter);
	}
	else {
		if (m_pDBSyzAccess->Syz_TokureiOpen(m_pZmSub->m_database, filter)) {
			return rt;
		}
	}
	CSYZ_TOKUREI* pSyzTokurei = m_pDBSyzAccess->m_pSyz_Tokurei;
	if( pSyzTokurei != NULL && pSyzTokurei->sign1) {
        rt = true;
	}

    return rt;
}

BEGIN_EVENTSINK_MAP(CShinKshiire, CSyzBaseDlg)
	ON_EVENT(CShinKshiire, IDC_ICSDIAG8CTRL2, 1, CShinKshiire::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinKshiire, IDC_ICSDIAG8CTRL2, 2, CShinKshiire::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinKshiire, IDC_ICSDIAG8CTRL2, 3, CShinKshiire::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()


void CShinKshiire::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;
	m_Focus = index;
}

void CShinKshiire::EditOFFIcsdiag8ctrl2(short index)
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	m_KshiireDiag02.GetData((int)index, (struct IUnknown *)&DiagData);

	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int clmidx = 0; clmidx < JIGYO_PER_PAGE_SHIIRE; clmidx++) {

			if (index == kbndatainfo[tblidx].diagidx[clmidx]) {

				if (kbndatainfo[tblidx].kind == DATAKIND_STR) {
					memmove(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size,
						DiagData.data_edit, kbndatainfo[tblidx].size);
				}
				else if (kbndatainfo[tblidx].kind == DATAKIND_VAL) {

					if ((kbndatainfo[tblidx].itemId.Compare(_T("CWB00116")) != 0)
						&& (kbndatainfo[tblidx].itemId.Compare(_T("CWB00126")) != 0) ) {

						memmove(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size,
							DiagData.data_val, MONY_BUF_SIZE);
					}

					if (index == idx_kshiire_1[clmidx] || index == idx_kshiire_2[clmidx]) {
						Update_KSHIIRE_3(index, clmidx);
						Update_KSHIIRE_7(index, clmidx);
						Update_KSHIIRE_7_KEI(index, clmidx);
						Update_KSHIIRE_8(index, clmidx);
						Update_KSHIIRE_8_KEI(index, clmidx);
						Update_KSHIIRE_19();
						Update_KSHIIRE_20();
					}

                    if (index == idx_kshiire_4[clmidx] || index == idx_kshiire_5[clmidx]) {
						Update_KSHIIRE_6(index, clmidx);
						Update_KSHIIRE_7(index, clmidx);
						Update_KSHIIRE_7_KEI(index, clmidx);
						Update_KSHIIRE_8(index, clmidx);
						Update_KSHIIRE_8_KEI(index, clmidx);
						Update_KSHIIRE_19();
						Update_KSHIIRE_20();
					}

					if ( index == idx_kshiire_7[clmidx] ) {
						CheckSetData_KSHIIRE_7(index, clmidx);
						Update_KSHIIRE_7_KEI(index, clmidx);
						Update_KSHIIRE_19();
					}

					if (index == idx_kshiire_8[clmidx]) {
						CheckSetData_KSHIIRE_8(index, clmidx);
						Update_KSHIIRE_8_KEI(index, clmidx);
						Update_KSHIIRE_20();
					}

				}
				else if (kbndatainfo[tblidx].kind == DATAKIND_RATE) {
					m_SyzUtil.val_to_bin((unsigned char*)kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size, DiagData.data_edit);

                    if ( index == idx_kshiire_3[clmidx] ) {
                        CheckSetData_KSHIIRE_3(index, clmidx);
						Update_KSHIIRE_7(index, clmidx);
						Update_KSHIIRE_7_KEI(index, clmidx);
						Update_KSHIIRE_8(index, clmidx);
						Update_KSHIIRE_8_KEI(index, clmidx);
						Update_KSHIIRE_19();
						Update_KSHIIRE_20();
					}

				}

			}
		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {
        
		if ((datainfo[tblidx].diagidx != NULL) && (index == datainfo[tblidx].diagidx[0])) {

			if (datainfo[tblidx].kind == DATAKIND_VAL) {
				memmove(datainfo[tblidx].data, DiagData.data_val, MONY_BUF_SIZE);

				if ( index == idx_kshiire_9[0] ) {
					Update_KSHIIRE_19();
				}
				else if ( index == idx_kshiire_10[0] ) {
					Update_KSHIIRE_20();
				}
				else if (index == idx_kshiire_11_1[0]) {
					Update_KSHIIRE_121(index);
				}
				else if (index == idx_kshiire_11_2[0]) {
					Update_KSHIIRE_122(index);
					Update_KSHIIRE_18();
					Update_KSHIIRE_20();
				}
				else if (index == idx_kshiire_12_1[0]) {
					CheckSetData_KSHIIRE_121(index);
					Update_KSHIIRE_17();
					Update_KSHIIRE_19();
				}
				else if (index == idx_kshiire_12_2[0]) {
					CheckSetData_KSHIIRE_122(index);
					Update_KSHIIRE_18();
					Update_KSHIIRE_20();
				}
				else if (index == idx_kshiire_13_2[0]) {
					Update_KSHIIRE_142(index);
					Update_KSHIIRE_18();
					Update_KSHIIRE_20();
				}
				else if (index == idx_kshiire_14_2[0]) {
					CheckSetData_KSHIIRE_142(index);
					Update_KSHIIRE_18();
					Update_KSHIIRE_20();

				}
				else if ( ( index == idx_kshiire_15_1[0] ) || ( index == idx_kshiire_16_1[0] ) ){
					Update_KSHIIRE_17();
					Update_KSHIIRE_19();
				}
				else if ( ( index == idx_kshiire_15_2[0] ) || ( index == idx_kshiire_16_2[0] ) ){
					Update_KSHIIRE_18();
					Update_KSHIIRE_20();
				}
			}

		}
	}

	Set_KSHIIRE_data();
}

void CShinKshiire::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if (nChar == 0) {
		return;
	}

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
	if ( (wTerm == VK_UP) || (wTerm == VK_TAB)) {
		pos = UpDownFocusMoveCheck(index, 1);
		m_KshiireDiag02.SetPosition(pos);
	}

	if ( (wTerm == VK_DOWN) || (wTerm == VK_RETURN)) {
		pos = UpDownFocusMoveCheck(index, 0);
		m_KshiireDiag02.SetPosition(pos);
	}


	if ((wTerm == VK_F3) || (wTerm == VK_RIGHT)) {
		if (IsRightEnd(index, &pos) && (GetCol_FromIndex(index) < JIGYO_MAX_TOKUREI_23)) {

			EditOFFIcsdiag8ctrl2(index);
			m_dispCol_left++;

			if (m_dispCol_left > JIGYO_MAX_TOKUREI_23 - JIGYO_PER_PAGE_SHIIRE) {
				if ((pos != ID_KSHIIRE_7_KEI) && (pos != ID_KSHIIRE_8_KEI)) {
					m_dispCol_left = 0;
					m_bar.SetScrollPos(m_dispCol_left);
				}
				else {
					m_dispCol_left--;
				}
				DispKshiire();
				m_KshiireDiag02.SetPosition(pos);
			}
			else {
				m_bar.SetScrollPos(m_dispCol_left);
				DispKshiire();
			}

		}
		else {
			m_KshiireDiag02.SetNextPosition();
		}
	}
	else if ((wTerm == VK_LEFT) || (wTerm == VK_F2)) {

		if (IsLeftEnd(index, &pos) && (GetCol_FromIndex(index) >= 0)) {

			EditOFFIcsdiag8ctrl2(index);
			if ((m_dispCol_left == 0) && (m_TokureiMode == TOKUREIMODE_RENDO)) {	// 連動の場合、事業区分名欄は1列目固定
				;
			}
			else
				m_dispCol_left--;

			if (m_dispCol_left < 0 || pos == ID_KSHIIRE_8_KEI) {

                if( (m_dispCol_left < 0) 
					&& (m_TokureiMode == TOKUREIMODE_HIRENDO)
					&& (index == ID_KSHIIRE_TITLE1)) {
					m_dispCol_left = 0;
					m_KshiireDiag02.SetPosition(ID_KSHIIRE_16_2);
				}
                else {
					m_dispCol_left = JIGYO_MAX_TOKUREI_23 - JIGYO_PER_PAGE_SHIIRE;
					m_bar.SetScrollPos(m_dispCol_left);
					DispKshiire();
					m_KshiireDiag02.SetPosition(pos);
				}

			}
			else {
				m_bar.SetScrollPos(m_dispCol_left);
				DispKshiire();
			}

		}
		else {
			m_KshiireDiag02.SetPrevPosition();
		}

	}

	if ( wTerm == VK_DELETE ){

		for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
			for (int clmidx = 0; clmidx < JIGYO_PER_PAGE_SHIIRE; clmidx++) {
				if (index == kbndatainfo[tblidx].diagidx[clmidx]) {

					if (index == idx_kshiire_3[clmidx]) {

						Update_KSHIIRE_3(index, clmidx, true);

                        char val[MONY_BUF_SIZE] = {0};
						memmove(val, kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size, kbndatainfo[tblidx].size);
						m_pArith->l_print(val, val, _T("sss9"));

						DIAGRAM_DATA DiagData;
						DiagData.data_imgdata = NULL;
						DiagData.data_edit = val;
						diag_setdata(IDC_ICSDIAG8CTRL2, kbndatainfo[tblidx].diagidx[clmidx], (struct IUnknown*)&DiagData, CIcsdiagctrl);

						KSHIIRE_3_SIGN[m_dispCol_left + clmidx] = 0;

					} else if (index == idx_kshiire_7[clmidx]) {

						Update_KSHIIRE_7(index, clmidx, true);
						dsp_prs(IDC_ICSDIAG8CTRL2,
							kbndatainfo[tblidx].diagidx[clmidx],
							kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size);
						KSHIIRE_7_SIGN[m_dispCol_left + clmidx] = 0;
					}
                    else if (index == idx_kshiire_8[clmidx]) {
						Update_KSHIIRE_8(index, clmidx, true);
						dsp_prs(IDC_ICSDIAG8CTRL2,
							kbndatainfo[tblidx].diagidx[clmidx],
							kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size);
						KSHIIRE_8_SIGN[m_dispCol_left + clmidx] = 0;
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
			if ( (datainfo[tblidx].diagidx != NULL) && (index == datainfo[tblidx].diagidx[0])) {
                if( index == idx_kshiire_12_1[0]) {
					Update_KSHIIRE_121(index, true);
					dsp_prs(IDC_ICSDIAG8CTRL2, idx_kshiire_12_1[0], KSHIIRE_12_1);
					KSHIIRE_12_1_SIGN = 0;
				}
				else if (index == idx_kshiire_12_2[0]) {
					Update_KSHIIRE_122(index, true);
					dsp_prs(IDC_ICSDIAG8CTRL2, idx_kshiire_12_2[0], KSHIIRE_12_2);
					KSHIIRE_12_2_SIGN = 0;
				}
				else if (index == idx_kshiire_14_2[0]) {
					Update_KSHIIRE_142(index, true);
					dsp_prs(IDC_ICSDIAG8CTRL2, idx_kshiire_14_2[0], KSHIIRE_14_2);
					KSHIIRE_14_2_SIGN = 0;
				}
				else {
					memset(datainfo[tblidx].data, '\0', datainfo[tblidx].size);
					dsp_cls(IDC_ICSDIAG8CTRL2, index);
				}
			}
		}

	}

	int nowpos = m_KshiireDiag02.GetPosition();	// 移動先または自身のポジション

	if( nowpos == ID_KSHIIRE_16_2 ){
		nowpos = ID_KSHIIRE_20;
	}

	RECT rectA, rectB;
	GetWindowRect(&rectA);
	diag_getrect(IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl);
	if ((rectA.top > rectB.top) || (rectA.bottom < rectB.bottom)) {
		int	cpos = GetScrollPos(SB_VERT);
		SetRedrawScroll((cpos + rectB.top - rectA.top) - ((rectA.bottom - rectA.top) / 2));
	}
}


BOOL CShinKshiire::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ) {
			// 更新
			GetAllDiagData();
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
				if ( m_curwnd = IDC_ICSDIAG8CTRL2 ) {
					TerminationIcsdiag8ctrl2( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

void CShinKshiire::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	MoveWindow_JigyoKubunBar();
}

void CShinKshiire::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);

	MoveWindow_JigyoKubunBar();
}

HBRUSH CShinKshiire::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinKshiire::OnBnClickedButtonShiiresyukei()
{

	SIMKURIWARIZREC uriwariRec;
	SIMKSIREWARIZREC sirewariRec;
	SIMKURIWARIZREC* pUriwariRec = NULL;

	if ((*m_pSnHeadData)->m_DispTabSgn & 0x10) {
		//第5-(1)号様式と併用
        memset(&uriwariRec, '\0', sizeof(uriwariRec));
		m_pSimKuriwari->SyukeiKuriwari10Eigyobi(&uriwariRec, m_tekiyotaisyokikan_s, m_tekiyotaisyokikan_e);
		pUriwariRec = &uriwariRec;
	}
	m_pSimKuriwari->SyukeiKsirewari(&sirewariRec, (*m_pSnHeadData)->IsNeedTkkzDisp(), m_tekiyotaisyokikan_s, m_tekiyotaisyokikan_e);

	CString msgstr;
	msgstr = _T("集計結果を転記しますか？\n※事業区分や金額は全て上書きされます。");

	if (ICSMessageBox(msgstr, MB_YESNO, 0, 0, this) == IDYES) {

		// 転記後に列番号１にフォーカスをセットするので、0を入れておく
		m_dispCol_left = 0;
		m_KshiireDiag02.SetPosition(ID_KSHIIRE_TITLE1);

		SetSyukeiData(&sirewariRec, pUriwariRec);

		CheckSub_ChangeBackColor(idx_kshiire_7[0], NULL, NULL, true);
		CheckSub_ChangeBackColor(idx_kshiire_8[0], NULL, NULL, true);

		DispClmNum();
		m_bar.SetScrollPos(0);
	}


}

void CShinKshiire::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
			m_dispCol_left -= JIGYO_PER_PAGE_SHIIRE;
			break;
		case SB_PAGERIGHT:
			m_dispCol_left += JIGYO_PER_PAGE_SHIIRE;
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
		default:	//SB_ENDSCROLL
			DrawSw = FALSE;		//描画されないときに、セット
			break;
		}

		if( !(*m_pSnHeadData)->Sn_Sign4 & 0x01 ){
			// 連動時、1列目以外は入力不可にする
			if( m_TokureiMode == TOKUREIMODE_RENDO ){
				if( m_dispCol_left == 0 ){		// 1列目→入力OK
					ATRIB_MOD(IDC_ICSDIAG8CTRL2, ID_KSHIIRE_TITLE1, 1, (*m_pSnHeadData)->Sn_Sign4, Kshiire[0].Connect);
				}
				else if( m_dispCol_left > 0 ){	// 1列目以外→入力不可
					ATRIB_MOD(IDC_ICSDIAG8CTRL2, ID_KSHIIRE_TITLE1, 0, (*m_pSnHeadData)->Sn_Sign4, Kshiire[0].Connect);
				}
			}
		}
		else{
			// 連動時、入力ロックが押されているときは1列目のみグレー背景にする。それ以外は白背景
			DIAGRAM_ATTRIBUTE	DA;
			if( m_TokureiMode == TOKUREIMODE_RENDO ){
				if( m_dispCol_left == 0 ){
					diag_getattr( IDC_ICSDIAG8CTRL2, ID_KSHIIRE_TITLE1, &DA, CIcsdiagctrl );
					DA.attr_bcolor = BC_GRAY;
					diag_setattr( IDC_ICSDIAG8CTRL2, ID_KSHIIRE_TITLE1, &DA, TRUE, CIcsdiagctrl );
				}
				else if( m_dispCol_left > 0 ){
					diag_getattr( IDC_ICSDIAG8CTRL2, ID_KSHIIRE_TITLE1, &DA, CIcsdiagctrl );
					DA.attr_bcolor = BC_WHITE;
					diag_setattr( IDC_ICSDIAG8CTRL2, ID_KSHIIRE_TITLE1, &DA, TRUE, CIcsdiagctrl );
				}
			}
		}

		if (m_dispCol_left > JIGYO_MAX_TOKUREI_23 - JIGYO_PER_PAGE_SHIIRE)	{
			m_dispCol_left = JIGYO_MAX_TOKUREI_23 - JIGYO_PER_PAGE_SHIIRE;
		}

		if (m_dispCol_left < 0) {
			m_dispCol_left = 0;
		}

		if (oldLeft == m_dispCol_left) {
			DrawSw = FALSE;
		}

		if (DrawSw) {
			m_bar.SetScrollPos(m_dispCol_left); //スクロールバーの移動
			DispKshiire();						//表示更新
		}

		diag_setposition(IDC_ICSDIAG8CTRL2, m_Focus, CIcsdiagctrl);

		this->UnlockWindowUpdate();
	}

	CSyzBaseDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

int CShinKshiire::GetCol_FromIndex(int index)
{
	int ret = -1;

    if( IsLeftEnd(index) ) {
		ret = m_dispCol_left;
	}

	if (IsRightEnd(index)) {
		ret = m_dispCol_left + 1;
	}

	return ret;
}

BOOL CShinKshiire::IsRightEnd(int index, int* pNextPos)
{
	BOOL ret = FALSE;
	for (int i = 0; i < (sizeof(idx_kshiire_row2) / sizeof(idx_kshiire_row2[0])); i++) {
		if (idx_kshiire_row2[i] == index) {
			if (pNextPos != NULL) {

				//if ((idx_kshiire_row2[i] == ID_KSHIIRE_7_2)
				//	&& (m_dispCol_left > JIGYO_MAX_TOKUREI_23 - JIGYO_PER_PAGE_SHIIRE - 1)) {
				//	*pNextPos = ID_KSHIIRE_7_KEI;
				//}
				//else if ((idx_kshiire_row2[i] == ID_KSHIIRE_8_2)
				//	&& (m_dispCol_left > JIGYO_MAX_TOKUREI_23 - JIGYO_PER_PAGE_SHIIRE - 1)) {
				//	*pNextPos = ID_KSHIIRE_8_KEI;
				//}
				//else {
					*pNextPos = idx_kshiireRend_next[i];
				//}

			}
			ret = TRUE;
			break;
		}
	}
	return ret;
}

BOOL CShinKshiire::IsLeftEnd(int index, int* pNextPos)
{
	BOOL ret = FALSE;
	for (int i = 0; i < (sizeof(idx_kshiire_row1) / sizeof(idx_kshiire_row1[0])); i++) {
		if (idx_kshiire_row1[i] == index) {
			if (idx_kshiire_row1[i] == index) {
				if (pNextPos != NULL) {
					*pNextPos = idx_kshiireLend_next[i];
				}
				ret = TRUE;
				break;
			}
		}
	}
	return ret;
}

void CShinKshiire::OnBnClickedKubunIns()
{
	// 列挿入

	int Clmindex = GetCol_FromIndex( m_Focus );
	if( Clmindex == -1 ){
		return;
	}

	if( IsNULL_Column( JIGYO_MAX_TOKUREI_23-1 ) == FALSE ){	// 最後尾のカラムに空きがあるかどうか
		m_KshiireDiag02.SetPosition(m_Focus);
		return;
	}

	MoveData( Clmindex, 1 );		// 列データを右に寄せる
	Init_KubunColumn( Clmindex );	// カラムデータを削除して空きを作る
	DispKshiire();

	m_KshiireDiag02.SetPosition(m_Focus);
}


void CShinKshiire::OnBnClickedKubunDel()
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
	DispKshiire();

	m_KshiireDiag02.SetPosition(m_Focus);
}

BOOL CShinKshiire::IsNULL_Column( int Clmindex )
{
	// 該当のカラムに空きがあるか？
	for( int clmidx = 0; clmidx < JIGYO_MAX_TOKUREI_23; clmidx++ ){
		if( clmidx == Clmindex ){
			for( int tblidx = 0; tblidx < sizeof( kbndatainfo ) / sizeof( kbndatainfo[0] ); tblidx++ ){

				CString	str;

				if( kbndatainfo[tblidx].kind == DATAKIND_RATE ){
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

void CShinKshiire::Init_KubunColumn( int ClmIndex )
{
	for( int clmidx = 0; clmidx < JIGYO_MAX_TOKUREI_23; clmidx++ ){	// 事業ごとのカラム
		if( clmidx == ClmIndex ){
			for( int tblidx = 0; tblidx < sizeof( kbndatainfo ) / sizeof( kbndatainfo[0] ); tblidx++ ){
				memset( kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * clmidx, '\0', kbndatainfo[tblidx].size );	// 削除
			}
		}
	}
}

void CShinKshiire::MoveData( int ClmIndex , int sign )
{
	if( sign == 0 ){	// 列削除
		
		for( int clmidx = ClmIndex; clmidx < JIGYO_MAX_TOKUREI_23; clmidx++ ){
			for( int tblidx = 0; tblidx < sizeof( kbndatainfo ) / sizeof( kbndatainfo[0] ); tblidx++ ){

				// 1つ前のカラムにデータを移す
				memmove( kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * clmidx,
						 kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * ( clmidx + 1 ),
						 kbndatainfo[tblidx].size );
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
			}
		}
	}
}

void CShinKshiire::ReCalq()
{
	// 再集計
	for( int clmidx = 0; clmidx < JIGYO_PER_PAGE_SHIIRE; clmidx++ ){
		for( int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++ ){

			if( kbndatainfo[tblidx].kind == DATAKIND_VAL ){

				int index = kbndatainfo[tblidx].diagidx[clmidx];

				if (index == idx_kshiire_1[clmidx] || index == idx_kshiire_2[clmidx]) {
					Update_KSHIIRE_3(index, clmidx);
					Update_KSHIIRE_7(index, clmidx);
					Update_KSHIIRE_7_KEI(index, clmidx);
					Update_KSHIIRE_8(index, clmidx);
					Update_KSHIIRE_8_KEI(index, clmidx);
					Update_KSHIIRE_19();
					Update_KSHIIRE_20();
					Set_KSHIIRE_data();
				}

                if (index == idx_kshiire_4[clmidx] || index == idx_kshiire_5[clmidx]) {
					Update_KSHIIRE_6(index, clmidx);
					Update_KSHIIRE_7(index, clmidx);
					Update_KSHIIRE_7_KEI(index, clmidx);
					Update_KSHIIRE_8(index, clmidx);
					Update_KSHIIRE_8_KEI(index, clmidx);
					Update_KSHIIRE_19();
					Update_KSHIIRE_20();
					Set_KSHIIRE_data();
				}
			}
		}
	}
}

void CShinKshiire::DispClmNum()
{
	// 列番号の表示
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	CString str;
	str.Format( "%d" , m_dispCol_left + 1 );
	DiagData.data_disp = str;
	diag_setdata(IDC_ICSDIAG8CTRL2, ID_KSHIIRE_NO1, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	str.Format( "%d" , m_dispCol_left + 2 );
	DiagData.data_disp = str;
	diag_setdata(IDC_ICSDIAG8CTRL2, ID_KSHIIRE_NO2, (struct IUnknown*)&DiagData, CIcsdiagctrl);
}

// 事業区分のスクロールバーを画面最下部に移動
void CShinKshiire::MoveWindow_JigyoKubunBar(bool bFromView)
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

//===========================================
// 小売等軽減売上割合を再計算
//===========================================
void CShinKshiire::ResetRate()
{
	char val[MONY_BUF_SIZE] = { 0 };
	memset ( KSHIIRE_3_SIGN, '\0', sizeof ( KSHIIRE_3_SIGN ) );
	for (int clmidx = 0; clmidx < JIGYO_MAX_TOKUREI_23; clmidx++) {
		for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
			memset ( val, '\0', sizeof ( val ) );
			if (kbndatainfo[tblidx].kind == DATAKIND_RATE) {
				Calc_KSHIIRE_3(kbndatainfo[tblidx].diagidx[clmidx], clmidx, &val[0]);
				memmove ( kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size, val, kbndatainfo[tblidx].size );
			}
		}
	}
}