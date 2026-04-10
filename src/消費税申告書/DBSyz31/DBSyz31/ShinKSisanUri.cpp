// ShinKSisanUri.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinKSisanUri.h"
#include "afxdialogex.h"
#include "ShinKSisanUriIdx.h"

// CShinKSisanUri ダイアログ

IMPLEMENT_DYNAMIC(CShinKSisanUri, CSyzBaseDlg)

CShinKSisanUri::CShinKSisanUri(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(CShinKSisanUri::IDD, pParent)
	, m_pZmSub(NULL)
	, m_pSnHeadData(NULL)
	, m_pShinInfo(NULL)
    , m_pArith(NULL)
	, m_pDBSyzAccess(NULL)
	, m_TokureiMode(TOKUREIMODE_RENDO)
	, m_pParent(pParent)
{

    InitTblInfo();
	ClearData();
	m_curwnd = 0;
	m_dispCol_left = 0;
	m_Focus = 0;
	m_SnSeq = -1;
}

CShinKSisanUri::~CShinKSisanUri()
{
    if( m_pSimKuriwari ) {
        delete m_pSimKuriwari;
        m_pSimKuriwari = NULL;
	}

    if( m_pDBSyzAccess ) {
		if (m_pDBSyzAccess->m_pSyz_Tokurei) {
			m_pDBSyzAccess->Syz_TokureiClose();
		}
        delete m_pDBSyzAccess;
        m_pDBSyzAccess = NULL;
	}
}

void CShinKSisanUri::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_KsisanUriDiag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_KsisanUriDiag02);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_bar);
	DDX_Control(pDX, IDC_BUTTON_BSDAY, m_ButtonBsday);
	DDX_Control(pDX, IDC_BUTTON_URI_SYUKEI, m_ButtonUriSyukei);
	DDX_Control(pDX, IDC_BUTTON1, m_ButtonUriColIns);
	DDX_Control(pDX, IDC_BUTTON2, m_ButtonUriColDel);
}


BEGIN_MESSAGE_MAP(CShinKSisanUri, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_BSDAY, &CShinKSisanUri::OnBnClickedButtonBsday)
	ON_BN_CLICKED(IDC_BUTTON1, &CShinKSisanUri::OnBnClickedKubunIns)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinKSisanUri::OnBnClickedKubunDel)
	ON_BN_CLICKED(IDC_BUTTON_URI_SYUKEI, &CShinKSisanUri::OnBnClickedButtonUriSyukei)
END_MESSAGE_MAP()


//営業日一覧ダイアログ呼び出し
void CShinKSisanUri::OnBnClickedButtonBsday()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int pos = m_KsisanUriDiag02.GetPosition();

	m_pSimKuriwari->CreateDlg(this);

	m_KsisanUriDiag02.SetPosition(pos);

	if (m_TokureiMode == TOKUREIMODE_RENDO) {
		GetAllDiagData();
		WriteData(0, TOKUREI_TBL_VERSION);
        SetSyukeiOnRendo();
        ChangeDiagAttr();
	}

}

void CShinKSisanUri::OnBnClickedButtonUriSyukei()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
#if 0 //現在未使用.
	if (m_pSimKuriwari->CreateSyukeiDlg(this, 
                                        m_tekiyotaisyokikan_s, m_tekiyotaisyokikan_e) == IDOK) { //積上げ計算を行う(課税標準額の消費税額)

		SIMKURIWARIZREC Rec = {0};
		m_pSimKuriwari->ReadDlgSyukeiData(&Rec);

        bool IsTenkiFlg = FALSE;
        for(int idx_jigyokbn = 0; idx_jigyokbn <  JIGYOKUBUN_MAX; idx_jigyokbn++){
            if( Rec.tenkisign[idx_jigyokbn] ) {
                IsTenkiFlg = TRUE;
			}
		}
        if( !IsTenkiFlg ) {
            return;
		}

        CString msgstr;
		msgstr = _T("集計結果を転記しますか？\n※事業区分や金額は全て上書きされます。");
		if (ICSMessageBox(msgstr, MB_YESNO, 0, 0, this) == IDYES) {

			// 転記後に列番号１にフォーカスをセットするので、0を入れておく
			m_dispCol_left = 0;

			// 列番号１にフォーカスを移動させる
			m_KsisanUriDiag02.SetPosition(ID_KSURI_1_1);

			if (m_kazeihousiki == ID_ICSSH_GENSOKU) {
				//原則課税
				SetSyukeiDataGensoku(&Rec);
			}
			else {
				//簡易課税
				SetSyukeiDataKanni(&Rec);
			}

			DispClmNum();
			m_bar.SetScrollPos( 0 );
		}
	}
#endif
}

BOOL CShinKSisanUri::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2);
	m_BottomIdx = KSISANURI_DIAGCTRLID_MAX;//コントロール一番最下のDiagのID指定。これで縦スクロール出る

	Init();

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
//		SyzSyukei		：	集計クラス
//		snseq			：	履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinKSisanUri::InitInfo(CSnHeadData **pSnHeadData, SYC_SHININFO *pShinInfo, CArithEx *pArith, CDBNpSub *pZmSub, CSyzSyukei **pSyzSyukei, int snseq)
{
	ASSERT( pSnHeadData );
	if ( pSnHeadData == NULL ) {
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
int CShinKSisanUri::Init()
{
	if ((m_SyzUtil.IsUnConnectMst(m_pZmSub->zvol) == TRUE) //非連動マスター
		|| ((*m_pSnHeadData)->Sn_Sign4 & 0x80)) {          //非連動チェックON
		m_TokureiMode = TOKUREIMODE_HIRENDO;
	}
	else if (!((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->m_DispTabSgn & 0x80) && ((*m_pSnHeadData)->m_DispTabSgn & 0x100))) { //非連動OFF && □特例非連動：ON
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

	// 課税方式
	m_kazeihousiki = m_SyzUtil.GetKazeihoushiki(m_pZmSub->zvol);

    // 特例テーブルバージョンチェック.
    TokureiTblVersionCheck(m_SnSeq);

	// データ読み込み
    //連動・非連動 共通.
	CString filter;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), m_SnSeq, ICS_SN_TOKUREI_1_HYOID, TOKUREI_TBL_VERSION);
	ReadData(m_SnSeq, TOKUREI_TBL_VERSION, filter);

	SCROLLINFO SCI;
	SCI.cbSize = 0;
	SCI.fMask = SIF_ALL;
	SCI.nMin = 0;
	SCI.nMax = JIGYO_MAX_TOKUREI_1 - 1;
	SCI.nPage = JIGYO_PER_PAGE_URI;
	SCI.nPos = 0;
	m_bar.SetScrollInfo(&SCI);
	m_bar.SetScrollPos(m_dispCol_left);

	InitDiagAttr();    // ダイアグラム初期属性セット
	DispKsisan_Uri();  // 画面出力
	SetItemPosition(); // 初期ポジションセット

    if( (m_TokureiMode == TOKUREIMODE_HANRENDO) 
		&& !((m_pZmSub->zvol->m_type & 0x01) == 0x01) //合併マスター
		&& !((*m_pSnHeadData)->Sn_Sign4 & 0x01) ) {   //入力ロック
		m_ButtonBsday.EnableWindow(TRUE);
	//	m_ButtonUriSyukei.EnableWindow(TRUE);
	}
    else {
        if( m_TokureiMode == TOKUREIMODE_RENDO){
			if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {//入力ロックでは設定ボタン選択不可にyoshida190816
				m_ButtonBsday.EnableWindow(FALSE);
			}
			else{
				m_ButtonBsday.EnableWindow(TRUE);
			}
		}
        else {
			m_ButtonBsday.EnableWindow(FALSE);
		}
	//	m_ButtonUriSyukei.EnableWindow(FALSE);
	}

	// 半連動を閉じたため、集計ボタンを非表示
	m_ButtonUriSyukei.ShowWindow ( SW_HIDE );

	if (m_TokureiMode == TOKUREIMODE_RENDO
		|| ((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {      //入力ロック.
        m_ButtonUriColIns.EnableWindow(FALSE);
        m_ButtonUriColDel.EnableWindow(FALSE);
	}
    else {
		m_ButtonUriColIns.EnableWindow(TRUE);
		m_ButtonUriColDel.EnableWindow(TRUE);
	}

	if (m_TokureiMode == TOKUREIMODE_RENDO ) {
		SetSyukeiOnRendo();
	}

	return 0;
}


void CShinKSisanUri::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	MoveWindow_JigyoKubunBar();
}

void CShinKSisanUri::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);

	MoveWindow_JigyoKubunBar();
}

HBRUSH CShinKSisanUri::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinKSisanUri::InitDiagAttr()
{

	int cnt = 0;
	while ( 1 ) {

		if ( KSisanUri[cnt].Index == 0 ) {
			break;
		}

		int Flg;
		if ( (m_kazeihousiki == ID_ICSSH_KANNI) && (m_TokureiMode == TOKUREIMODE_RENDO) ){	// 簡易・特例が連動の時
			// 全項目入力不可
			Flg = 0;
		}
		else {
			// 確定時は全入力不可
			if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
				Flg = KSisanUri[cnt].AllUnConnect;
			}
			else {
				if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {
					Flg = KSisanUri[cnt].UnConnect;
				}
				else {
					if( m_TokureiMode == TOKUREIMODE_HANRENDO ) {
						Flg = KSisanUri[cnt].HalfConnect;
					}
					else {
						Flg = KSisanUri[cnt].Connect;
					}
				}
			}
		}

		ATRIB_MOD(IDC_ICSDIAG8CTRL2, KSisanUri[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, KSisanUri[cnt].Connect);
		cnt++;
	}

	m_KsisanUriDiag02.EnableDelete(0);	// deleteキ１回で削除モード設定

	return 0;
}

//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
void CShinKSisanUri::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		index = m_KsisanUriDiag02.GetPosition();
		if ( (index = m_KsisanUriDiag02.GetPosition()) != -1 ) {
			m_KsisanUriDiag01.SetFocus();	// 下のsetfocusを有効にするためにダミーでセット
			m_KsisanUriDiag02.SetPosition(index);
			m_KsisanUriDiag02.SetFocus();
			return;
		}
	}

	m_KsisanUriDiag01.SetFocus();
	m_KsisanUriDiag02.SetFocus();

	// 課税資産の譲渡等の税込価額の合計額　１
	diag_setposition(IDC_ICSDIAG8CTRL2, ID_KSURI_TITLE1, CIcsdiagctrl);//初期フォーカス場所変更yoshida190812
//	diag_setposition(IDC_ICSDIAG8CTRL2, ID_KSURI_1_1, CIcsdiagctrl);
}

void CShinKSisanUri::InitTblInfo()
{

	kbndatainfo[0] = { _T("CUB00030"), DATAKIND_STR, KSURI_TITLE[0], 64, &idx_ksuri_title[0] };
	kbndatainfo[1] = { _T("CUB00040"), DATAKIND_VAL, KSURI_1[0], MONY_BUF_SIZE, &idx_ksuri_1[0] };
	kbndatainfo[2] = { _T("CUB00060"), DATAKIND_DATE, KSURI_2_S[0], 4, &idx_ksuri_2_s[0] };
	kbndatainfo[3] = { _T("CUB00070"), DATAKIND_DATE, KSURI_2_E[0], 4, &idx_ksuri_2_e[0] };
	kbndatainfo[4] = { _T("CUB00080"), DATAKIND_VAL, KSURI_3[0], MONY_BUF_SIZE, &idx_ksuri_3[0] };
	kbndatainfo[5] = { _T("CUB00090"), DATAKIND_VAL, KSURI_4[0], MONY_BUF_SIZE, &idx_ksuri_4[0] };
	kbndatainfo[6] = { _T("CUB00100"), DATAKIND_RATE, KSURI_5[0], MONY_BUF_SIZE, &idx_ksuri_5[0] };
	kbndatainfo[7] = { _T("CUB00105"), DATAKIND_CHECK, (char*)&KSURI_5_CHECK[0], sizeof(int), &idx_ksuri_5_check[0] };
	kbndatainfo[8] = { _T("CUB00110"), DATAKIND_VAL, KSURI_6[0], MONY_BUF_SIZE, &idx_ksuri_6[0] };
	kbndatainfo[9] = { _T("CUB00115"), DATAKIND_SIGN, (char*)&KSURI_6_SIGN[0], sizeof(int), &idx_ksuri_dummy[0] };
	kbndatainfo[10] = { _T("CUB00120"), DATAKIND_VAL, KSURI_7[0], MONY_BUF_SIZE, &idx_ksuri_7[0] };
	kbndatainfo[11] = { _T("CUB00125"), DATAKIND_SIGN, (char*)&KSURI_7_SIGN[0], sizeof(int), &idx_ksuri_dummy[0] };

	datainfo[0] = { _T("CUB00140"), DATAKIND_VAL, KSURI_6_KEI, MONY_BUF_SIZE, &idx_ksuri_6_kei[0] };
	datainfo[1] = { _T("CUB00150"), DATAKIND_VAL, KSURI_7_KEI, MONY_BUF_SIZE, &idx_ksuri_7_kei[0] };
	datainfo[2] = { _T("CUC00010"), DATAKIND_VAL, KSURI_8, MONY_BUF_SIZE, &idx_ksuri_8[0] };
	datainfo[3] = { _T("CUC00020"), DATAKIND_VAL, KSURI_9, MONY_BUF_SIZE, &idx_ksuri_9[0] };
	datainfo[4] = { _T("CUD00010"), DATAKIND_VAL, KSURI_10, MONY_BUF_SIZE, &idx_ksuri_10[0] };
	datainfo[5] = { _T("CUD00020"), DATAKIND_VAL, KSURI_11, MONY_BUF_SIZE, &idx_ksuri_11[0] };

}

//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinKSisanUri::UpDownFocusMoveCheck( int Index, int Move )
{
	int nextFocusIdx = 0;
	int pos = 0;
	DIAGRAM_ATTRIBUTE	DA;

	// 現在ポジション検索
	for (pos = 0; pos < (sizeof(FCS_KSisanUri)/sizeof(FCS_KSisanUri[0])); pos++) {
		if (Index == FCS_KSisanUri[pos].IDNo) {
			break;
		}
	}

	nextFocusIdx = FCS_KSisanUri[pos].IDNo;	// 現在ポジションセット

	// 次ポジション検索
	if (Move == MOVE_DOWN) {
        pos++;
		for (int j = pos; j < (sizeof(FCS_KSisanUri) / sizeof(FCS_KSisanUri[0])); j++) {
			m_KsisanUriDiag02.GetAttribute(FCS_KSisanUri[j].IDNo, (LPUNKNOWN)&DA);
			if ((DA.attr_bcolor == BC_CREAM) || (DA.attr_bcolor == BC_BLUE)) {
				nextFocusIdx = FCS_KSisanUri[j].IDNo;
				break;
			}
		}
	}
	else if (Move == MOVE_UP) {
        pos--;
		for (int j = pos; j > -1; j--) {
			m_KsisanUriDiag02.GetAttribute(FCS_KSisanUri[j].IDNo, (LPUNKNOWN)&DA);
			if ((DA.attr_bcolor == BC_CREAM) || (DA.attr_bcolor == BC_BLUE)) {
				nextFocusIdx = FCS_KSisanUri[j].IDNo;
				break;
			}
		}
	}

	return nextFocusIdx;
}

//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//		Mony	：	表示金額
//-----------------------------------------------------------------------------
void CShinKSisanUri::dsp_prs(unsigned short Id, short Index, char *Mony)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );

	diag_setdata(Id, Index, (struct IUnknown*)&DiagData, CIcsdiagctrl);
}


//-----------------------------------------------------------------------------
// 各項目クリア
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//-----------------------------------------------------------------------------
void CShinKSisanUri::dsp_cls(unsigned short Id, short Index)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	switch ( Index ) {
	case ID_KSURI_TITLE1:
	case ID_KSURI_TITLE2:
	case ID_KSURI_TITLE3:
	case ID_KSURI_5_1:
	case ID_KSURI_5_2:
	case ID_KSURI_5_3:
		diag_clear(Id, Index, 1, CIcsdiagctrl);
		break;
	case ID_KSURI_2_1_S:
	case ID_KSURI_2_2_S:
	case ID_KSURI_2_3_S:
	case ID_KSURI_2_1_E:
	case ID_KSURI_2_2_E:
	case ID_KSURI_2_3_E:
		memset(DiagData.data_day, '\0', sizeof(DiagData.data_day));
		diag_setdata(Id, Index, (struct IUnknown*)&DiagData, CIcsdiagctrl);
		break;
	case ID_KSURI_5CHECK_1:
	case ID_KSURI_5CHECK_2:
	case ID_KSURI_5CHECK_3:
		DiagData.data_check=0;
		diag_setdata(Id, Index, (struct IUnknown*)&DiagData, CIcsdiagctrl);
		break;
	default:
		memset(DiagData.data_val, '\0', sizeof(DiagData.data_val));
		diag_setdata( Id, Index, (struct IUnknown*)&DiagData, CIcsdiagctrl );
		break;
	}
}

void CShinKSisanUri::SetSyukeiOnRendo()
{

	if (m_TokureiMode != TOKUREIMODE_RENDO) {
        return;
	}

	if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) { 
        //入力ロック中.
        return;
	}

	SIMKURIWARIZREC Rec = { 0 };
	m_pSimKuriwari->SyukeiKuriwari10Eigyobi(&Rec, m_tekiyotaisyokikan_s, m_tekiyotaisyokikan_e);

	// 転記後に列番号１にフォーカスをセットするので、0を入れておく
	m_dispCol_left = 0;

	if (m_kazeihousiki == ID_ICSSH_GENSOKU) {
		//原則課税
		SetSyukeiDataGensoku(&Rec);
	}
	else {
		//簡易課税
		SetSyukeiDataKanni(&Rec);
	}

	// 列番号１にフォーカスを移動させる
	m_KsisanUriDiag02.SetPosition(ID_KSURI_TITLE1);//yoshida190812
	DispClmNum();
	m_bar.SetScrollPos(0);

}

int CShinKSisanUri::SetSyukeiDataGensoku(SIMKURIWARIZREC* pRec)
{
	//if (pRec->tenkisign[0] == 0) {
	//	return 0;
	//}

	ClearData();

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	KSURI_KBN[0] = 0;

    memset(KSURI_TITLE[0], '\0', sizeof(KSURI_TITLE[0]));
    memmove(KSURI_1[0], (char*)pRec->uriagegoukei[0], MONY_BUF_SIZE);

	CVolDateDB voldate;
	int s_gengo = 0, d_gengo = 0, d_year = 0;
	CString datestr;

	voldate.db_datecnvGen(s_gengo, pRec->sddate[0], &d_gengo, &d_year, 0, 0);
	datestr.Format(_T("%06d"), d_year);

	if (m_pArith->l_test(KSURI_1[0]) && pRec->sddate[0]) {
		KSURI_2_S[0][0] = (char)strtol(datestr.Mid(0, 2), NULL, 16);
		KSURI_2_S[0][1] = (char)strtol(datestr.Mid(2, 2), NULL, 16);
		KSURI_2_S[0][2] = (char)strtol(datestr.Mid(4, 2), NULL, 16);
		KSURI_2_S[0][3] = '\0';
	}

	voldate.db_datecnvGen(s_gengo, pRec->eddate[0], &d_gengo, &d_year, 0, 0);
	datestr.Format(_T("%06d"), d_year);

	if (m_pArith->l_test(KSURI_1[0]) && pRec->eddate[0]) {
		KSURI_2_E[0][0] = (char)strtol(datestr.Mid(0, 2), NULL, 16);
		KSURI_2_E[0][1] = (char)strtol(datestr.Mid(2, 2), NULL, 16);
		KSURI_2_E[0][2] = (char)strtol(datestr.Mid(4, 2), NULL, 16);
		KSURI_2_E[0][3] = '\0';
	}

    memmove(KSURI_3[0], (char*)pRec->uriage[0], MONY_BUF_SIZE);
    memmove(KSURI_4[0], (char*)pRec->keigenuriage[0], MONY_BUF_SIZE);

	char val[MONY_BUF_SIZE] = { 0 } , val2[MONY_BUF_SIZE] = { 0 };
	char val100[MONY_BUF_SIZE], val108[MONY_BUF_SIZE], val110[MONY_BUF_SIZE];
	CString str100 = _T("100");
	CString str108 = _T("108");

	m_pArith->l_input(&val100, str100.GetBuffer());
	m_pArith->l_input(&val108, str108.GetBuffer());

	memmove(KSURI_5[0], (char*)pRec->keigenwariai[0], MONY_BUF_SIZE);

	//連動時、□50%適用チェック状態を取得.
	if (m_TokureiMode == TOKUREIMODE_RENDO) {
		CString filter;
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d and itmId = 'CUB00105' and idx = %d"),
			m_SnSeq, ICS_SN_TOKUREI_1_HYOID, TOKUREI_TBL_VERSION, 0);

		ReadData(m_SnSeq, TOKUREI_TBL_VERSION, filter);
	}

	memset(&val, 0, MONY_BUF_SIZE);
	l_pardotnet(&val[0], (char*)pRec->uriagegoukei[0], (char*)pRec->keigenuriage[0], (char*)pRec->uriage[0]);

	m_pArith->l_mul(&val, &val, val100);
	m_pArith->l_div(&val, &val, val108);

	memmove(KSURI_6[0], &val, MONY_BUF_SIZE);

	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_1; idx_jigyokbn++) {
		m_pArith->l_add(&KSURI_6_KEI, &KSURI_6_KEI, KSURI_6[idx_jigyokbn]);
	}

	CString str110 = _T("110");
	m_pArith->l_input(&val110, str110.GetBuffer());
	memset(&val, 0, sizeof(val));

	l_pardotnet(&val[0], (char*)pRec->uriagegoukei[0], (char*)pRec->keigenuriage[0], (char*)pRec->uriage[0]);

	m_pArith->l_sub(&val, pRec->uriagegoukei[0], &val);
	m_pArith->l_mul(&val, &val, val100);
	m_pArith->l_div(&val, &val, val110);
	memmove(KSURI_7[0], &val, MONY_BUF_SIZE);

	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_1; idx_jigyokbn++) {
		m_pArith->l_add(&KSURI_7_KEI, &KSURI_7_KEI, KSURI_7[idx_jigyokbn]);
	}

	m_pArith->l_add(&KSURI_10, &KSURI_6_KEI, &KSURI_8);
	m_pArith->l_add(&KSURI_11, &KSURI_7_KEI, &KSURI_9);


	//---->yoshida190919
	//売上変換
	m_pArith->l_add(HENKAN_GOKEI[0], (char*)pRec->hanbaisyoreikinhiyo[0], (char*)pRec->uriagewaribiki[0]);

	memset(&val, 0, sizeof(val));
	l_pardotnet(&val[0], HENKAN_GOKEI[0], (char*)pRec->keigenuriage[0], (char*)pRec->uriage[0]);

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
	memmove(KASISON_GOKEI[0], (char*)pRec->kashidaoreson[0], MONY_BUF_SIZE);

	memset(&val, 0, sizeof(val));
	l_pardotnet(&val[0], KASISON_GOKEI[0], (char*)pRec->keigenuriage[0], (char*)pRec->uriage[0]);

	memset(&val2, 0, sizeof(val2));
	m_pArith->l_sub(&val2, KASISON_GOKEI[0], &val);

//	m_pArith->l_mul(&val, &val, val100);
//	m_pArith->l_div(&val, &val, val108);

//	m_pArith->l_mul(&val2, &val2, val100);
//	m_pArith->l_div(&val2, &val2, val110);


	memmove(KSURI_KASISON_KEI8[0], &val, MONY_BUF_SIZE);
	memmove(KSURI_KASISON_10[0], &val2, MONY_BUF_SIZE);


	//貸倒回収
	memmove(KASIKAISYU_GOKEI[0], (char*)pRec->kashidaorekaisyu[0], MONY_BUF_SIZE);

	memset(&val, 0, sizeof(val));
	l_pardotnet(&val[0], KASIKAISYU_GOKEI[0], (char*)pRec->keigenuriage[0], (char*)pRec->uriage[0]);

	memset(&val2, 0, sizeof(val2));
	m_pArith->l_sub(&val2, KASIKAISYU_GOKEI[0], &val);

//	m_pArith->l_mul(&val, &val, val100);
//	m_pArith->l_div(&val, &val, val108);

//	m_pArith->l_mul(&val2, &val2, val100);
//	m_pArith->l_div(&val2, &val2, val110);


	memmove(KSURI_KASIKAISYU_KEI8[0], &val, MONY_BUF_SIZE);
	memmove(KSURI_KASIKAISYU_10[0], &val2, MONY_BUF_SIZE);
	//<-----------------




	// 連動時(編集＝チェックオフ)、事業区分名をテーブルから再取得。1列目の名称のみ。
	if (m_TokureiMode == TOKUREIMODE_RENDO) {
		CString filter;
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d and itmId = 'CUB00030' and idx = %d"), 
                          m_SnSeq, ICS_SN_TOKUREI_1_HYOID, TOKUREI_TBL_VERSION, 0);

		ReadData(m_SnSeq, TOKUREI_TBL_VERSION, filter);
	}

	//連動時、□50%適用チェックONの場合、連動項目変更.
	if (m_TokureiMode == TOKUREIMODE_RENDO) {
		if (KSURI_5_CHECK[0]) {
			CheckSub_KSURI_5CheckOn(idx_ksuri_5_check[0], 0);
		}
	}


	Set_KSURI_data();

    return 0;
}

int CShinKSisanUri::SetSyukeiDataKanni(SIMKURIWARIZREC* pRec)
{

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char val[MONY_BUF_SIZE] = { 0 };
	char val2[MONY_BUF_SIZE] = { 0 };
	char valtmp[MONY_BUF_SIZE] = { 0 };
	char val100[MONY_BUF_SIZE], val108[MONY_BUF_SIZE], val110[MONY_BUF_SIZE];
	CString str100 = _T("100");
	CString str108 = _T("108");
	CString str110 = _T("110");

	m_pArith->l_input(&val100, str100.GetBuffer());
	m_pArith->l_input(&val108, str108.GetBuffer());
	m_pArith->l_input(&val110, str110.GetBuffer());

	CVolDateDB voldate;
	int s_gengo = 0, d_gengo = 0, d_year = 0;
	CString datestr;

	for (int idx_jigyokbn = 1; idx_jigyokbn < JIGYOKUBUN_MAX; idx_jigyokbn++) {
		ClearData();
    }

	for (int idx_jigyokbn = 1, idx_disp = 0; idx_jigyokbn < JIGYOKUBUN_MAX; idx_jigyokbn++) {

        //データがない事業区分を飛ばす.
		if (pRec->tenkisign[idx_jigyokbn] == 0) {
			continue;
		}
		KSURI_KBN[idx_disp] = idx_jigyokbn;

		memset(&val, 0, MONY_BUF_SIZE);
		memmove(&val, (char*)pRec->uriagegoukei[idx_jigyokbn], MONY_BUF_SIZE);

        //別記金額を税込み扱いとして、税額分加算.
		memset(&val2, 0, MONY_BUF_SIZE);
		memmove(&val2, (char*)pRec->uriagegoukei_bekki[idx_jigyokbn], MONY_BUF_SIZE);
		m_pArith->l_sub(&val2, &val2, (char*)pRec->keigenuriagegoukei_bekki[idx_jigyokbn]);

		memset(&valtmp, 0, MONY_BUF_SIZE);
		memmove(&valtmp, &val2, MONY_BUF_SIZE);
		m_pArith->l_mul(&val2, &val2, val110);
		m_pArith->l_div(&val2, &val2, val100);
		m_pArith->l_sub(&val2, &val2, valtmp);

		m_pArith->l_add(&val, &val, &val2);

        //別記金額(軽減)を税込み扱いとして、税額分加算.
		memset(&val2, 0, MONY_BUF_SIZE);
		memmove(&val2, (char*)pRec->keigenuriagegoukei_bekki[idx_jigyokbn], MONY_BUF_SIZE);
		memset(&valtmp, 0, MONY_BUF_SIZE);
		memmove(&valtmp, &val2, MONY_BUF_SIZE);
		m_pArith->l_mul(&val2, &val2, val108);
		m_pArith->l_div(&val2, &val2, val100);
		m_pArith->l_sub(&val2, &val2, valtmp);

		m_pArith->l_add(&val, &val, &val2);

		// START 220929_1303 2022.12.01 (MDCR_HHA)
		//memmove(KSURI_1[idx_disp], val, MONY_BUF_SIZE);

		//if( m_pArith->l_test(KSURI_1[idx_disp]) ) {
		if (!m_pArith->l_test(val)) {
			continue;
		}else{
			memmove(KSURI_1[idx_disp], val, MONY_BUF_SIZE);
		// END 220929_1303 2022.12.01 (MDCR_HHA)

			memmove(KSURI_TITLE[idx_disp], ksuri_kbntitle[idx_jigyokbn], sizeof(KSURI_TITLE[idx_disp]));

			voldate.db_datecnvGen(s_gengo, pRec->sddate[idx_jigyokbn], &d_gengo, &d_year, 0, 0);
			datestr.Format(_T("%06d"), d_year);

			KSURI_2_S[idx_disp][0] = (char)strtol(datestr.Mid(0, 2), NULL, 16);
			KSURI_2_S[idx_disp][1] = (char)strtol(datestr.Mid(2, 2), NULL, 16);
			KSURI_2_S[idx_disp][2] = (char)strtol(datestr.Mid(4, 2), NULL, 16);
			KSURI_2_S[idx_disp][3] = '\0';

			voldate.db_datecnvGen(s_gengo, pRec->eddate[idx_jigyokbn], &d_gengo, &d_year, 0, 0);
			datestr.Format(_T("%06d"), d_year);

			KSURI_2_E[idx_disp][0] = (char)strtol(datestr.Mid(0, 2), NULL, 16);
			KSURI_2_E[idx_disp][1] = (char)strtol(datestr.Mid(2, 2), NULL, 16);
			KSURI_2_E[idx_disp][2] = (char)strtol(datestr.Mid(4, 2), NULL, 16);
			KSURI_2_E[idx_disp][3] = '\0';
		}

		memset(&val, 0, MONY_BUF_SIZE);
		memmove(&val, (char*)pRec->uriage[idx_jigyokbn], MONY_BUF_SIZE);

		//別記金額を税込み扱いとして、税額分加算.
		memset(&val2, 0, MONY_BUF_SIZE);
		memmove(&val2, (char*)pRec->uriage_bekki[idx_jigyokbn], MONY_BUF_SIZE);
		m_pArith->l_sub(&val2, &val2, (char*)pRec->keigenuriage_bekki[idx_jigyokbn]);

		memset(&valtmp, 0, MONY_BUF_SIZE);
		memmove(&valtmp, &val2, MONY_BUF_SIZE);
		m_pArith->l_mul(&val2, &val2, val110);
		m_pArith->l_div(&val2, &val2, val100);
		m_pArith->l_sub(&val2, &val2, valtmp);

		m_pArith->l_add(&val, &val, &val2);

		//別記金額(軽減)を税込み扱いとして、税額分加算.
		memset(&val2, 0, MONY_BUF_SIZE);
		memmove(&val2, (char*)pRec->keigenuriage_bekki[idx_jigyokbn], MONY_BUF_SIZE);
		memset(&valtmp, 0, MONY_BUF_SIZE);
		memmove(&valtmp, &val2, MONY_BUF_SIZE);
		m_pArith->l_mul(&val2, &val2, val108);
		m_pArith->l_div(&val2, &val2, val100);
		m_pArith->l_sub(&val2, &val2, valtmp);

		m_pArith->l_add(&val, &val, &val2);
		memmove(KSURI_3[idx_disp], val, MONY_BUF_SIZE);

		memset(&val, 0, MONY_BUF_SIZE);
		memmove(&val, (char*)pRec->keigenuriage[idx_jigyokbn], MONY_BUF_SIZE);

		memset(&val2, 0, MONY_BUF_SIZE);
		memmove(&val2, (char*)pRec->keigenuriage_bekki[idx_jigyokbn], MONY_BUF_SIZE);
		m_pArith->l_mul(&val2, &val2, val108);
		m_pArith->l_div(&val2, &val2, val100);
		m_pArith->l_sub(&val2, &val2, (char*)pRec->keigenuriage_bekki[idx_jigyokbn]);

		m_pArith->l_add(&val, &val, &val2);
		memmove(KSURI_4[idx_disp], val, MONY_BUF_SIZE);

		memmove(KSURI_5[idx_disp], (char*)pRec->keigenwariai[idx_jigyokbn], MONY_BUF_SIZE);

		//連動時、□50%適用チェック状態を取得.
		if (m_TokureiMode == TOKUREIMODE_RENDO) {
			CString filter;
			filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d and itmId = 'CUB00105'"),
				m_SnSeq, ICS_SN_TOKUREI_1_HYOID, TOKUREI_TBL_VERSION);

			ReadData(m_SnSeq, TOKUREI_TBL_VERSION, filter);
		}

		memset(&val, 0, MONY_BUF_SIZE);
		l_pardotnet(&val[0], KSURI_1[idx_disp], (char*)pRec->keigenuriage[idx_jigyokbn], (char*)pRec->uriage[idx_jigyokbn]);
		m_pArith->l_mul(&val, &val, val100);
		m_pArith->l_div(&val, &val, val108);

		memmove(KSURI_6[idx_disp], &val, MONY_BUF_SIZE);

		memset(&val, 0, sizeof(val));
		l_pardotnet(&val[0], (char*)pRec->uriagegoukei[idx_jigyokbn], (char*)pRec->keigenuriage[idx_jigyokbn], (char*)pRec->uriage[idx_jigyokbn]);

		if (m_pArith->l_cmp(val, pRec->uriagegoukei[idx_jigyokbn])) {
			m_pArith->l_sub(&val, KSURI_1[idx_disp], &val);
			m_pArith->l_mul(&val, &val, val100);
			m_pArith->l_div(&val, &val, val110);
			memmove(KSURI_7[idx_disp], &val, MONY_BUF_SIZE);
		}


		//---->yoshida190919
		m_pArith->l_add(HENKAN_GOKEI[idx_disp], (char*)pRec->hanbaisyoreikinhiyo[idx_jigyokbn], (char*)pRec->uriagewaribiki[idx_jigyokbn]);


		memset(&val, 0, sizeof(val));
		l_pardotnet(&val[0], HENKAN_GOKEI[idx_disp], (char*)pRec->keigenuriage[idx_jigyokbn], (char*)pRec->uriage[idx_jigyokbn]);

		memset(&val2, 0, sizeof(val2));
		m_pArith->l_sub(&val2, HENKAN_GOKEI[idx_disp], &val);

		//売上変換（付表１用　税抜き計算処理いらない）
		memmove(KSURI_NEBIKI_KEI8_FHYO1[idx_disp], &val, MONY_BUF_SIZE);
		memmove(KSURI_NEBIKI_10_FHYO1[idx_disp], &val2, MONY_BUF_SIZE);


		m_pArith->l_mul(&val, &val, val100);
		m_pArith->l_div(&val, &val, val108);

		m_pArith->l_mul(&val2, &val2, val100);
		m_pArith->l_div(&val2, &val2, val110);


		memmove(KSURI_NEBIKI_KEI8[idx_disp], &val, MONY_BUF_SIZE);
		memmove(KSURI_NEBIKI_10[idx_disp], &val2, MONY_BUF_SIZE);


		//貸倒損失
		memmove(KASISON_GOKEI[idx_disp], (char*)pRec->kashidaoreson[idx_jigyokbn], MONY_BUF_SIZE);

		memset(&val, 0, sizeof(val));
		l_pardotnet(&val[0], KASISON_GOKEI[idx_disp], (char*)pRec->keigenuriage[idx_jigyokbn], (char*)pRec->uriage[idx_jigyokbn]);

		memset(&val2, 0, sizeof(val2));
		m_pArith->l_sub(&val2, KASISON_GOKEI[idx_disp], &val);

		memmove(KSURI_KASISON_KEI8[idx_disp], &val, MONY_BUF_SIZE);
		memmove(KSURI_KASISON_10[idx_disp], &val2, MONY_BUF_SIZE);


		//貸倒回収
		memmove(KASIKAISYU_GOKEI[idx_disp], (char*)pRec->kashidaorekaisyu[idx_jigyokbn], MONY_BUF_SIZE);

		memset(&val, 0, sizeof(val));
		l_pardotnet(&val[0], KASIKAISYU_GOKEI[idx_disp], (char*)pRec->keigenuriage[idx_jigyokbn], (char*)pRec->uriage[idx_jigyokbn]);

		memset(&val2, 0, sizeof(val2));
		m_pArith->l_sub(&val2, KASIKAISYU_GOKEI[idx_disp], &val);


		memmove(KSURI_KASIKAISYU_KEI8[idx_disp], &val, MONY_BUF_SIZE);
		memmove(KSURI_KASIKAISYU_10[idx_disp], &val2, MONY_BUF_SIZE);
		//<------------------


		idx_disp++;

	}

	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_1; idx_jigyokbn++) {
		m_pArith->l_add(&KSURI_6_KEI, &KSURI_6_KEI, KSURI_6[idx_jigyokbn]);
	}

	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_1; idx_jigyokbn++) {
		m_pArith->l_add(&KSURI_7_KEI, &KSURI_7_KEI, KSURI_7[idx_jigyokbn]);
	}

	memmove(KSURI_8, pRec->sgaikbn_keigenuriage_sum, MONY_BUF_SIZE);
	memmove(KSURI_9, pRec->sgaikbn_uriage_sum, MONY_BUF_SIZE);

	m_pArith->l_add(&KSURI_10, &KSURI_6_KEI, &KSURI_8);
	m_pArith->l_add(&KSURI_11, &KSURI_7_KEI, &KSURI_9);

	//連動時、□50%適用チェックONの場合、連動項目変更.
	if (m_TokureiMode == TOKUREIMODE_RENDO) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYOKUBUN_MAX; idx_jigyokbn++) {
			if (KSURI_5_CHECK[idx_jigyokbn]){
				CheckSub_KSURI_5CheckOn_KanniRendo(idx_jigyokbn);
			}
		}
	}

    Set_KSURI_data();

    return 0;
}

//-----------------------------------------------------------------------------
// 特例テーブルバージョンチェック.
//-----------------------------------------------------------------------------
// 引数     sn_seq:履歴シーケンス
// 返送値	0：正常終了 -1：エラー
//-----------------------------------------------------------------------------
int CShinKSisanUri::TokureiTblVersionCheck(int sn_seq)
{
	CString filter;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TOKUREI_1_HYOID, TOKUREI_TBL_OLD_VERSION);

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
		sql.Format(_T("DELETE syz_tokurei WHERE version = %d and hyoId = \'%s\'"), TOKUREI_TBL_VERSION, ICS_SN_TOKUREI_1_HYOID);
		m_pZmSub->m_database->ExecuteSQL(sql);
        sql.Empty();
		sql.Format(_T("UPDATE syz_tokurei SET version = %d WHERE sn_seq = %d and hyoId = \'%s\'"), TOKUREI_TBL_VERSION, sn_seq, ICS_SN_TOKUREI_1_HYOID);
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
int CShinKSisanUri::ReadData(int sn_seq, int version, CString filter)
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

int CShinKSisanUri::ReadSub_data(CSYZ_TOKUREI* pTokurei)
{

	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_1; idx_jigyokbn++) {

			if ((pTokurei->itemId == kbndatainfo[tblidx].itemId) && (pTokurei->idx == idx_jigyokbn)) {

				if (pTokurei->kind == DATAKIND_STR) {
					// 連動時入力不可の事業区分はReadしない
					if ((m_TokureiMode == TOKUREIMODE_RENDO) && (idx_jigyokbn != 0) && (m_kazeihousiki == ID_ICSSH_GENSOKU))
						continue;

					memmove(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn,
						pTokurei->str, kbndatainfo[tblidx].size);
				}
				else if (pTokurei->kind == DATAKIND_VAL) {
					m_SyzUtil.val_to_bin(
						(unsigned char*)(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn),
						pTokurei->val);

					if ((pTokurei->itemId.Compare(_T("CUB00080")) == 0) ) {
						m_SyzUtil.val_to_bin(
							(unsigned char*)(KSURI_3_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn),
							pTokurei->val);
					}

					if ((pTokurei->itemId.Compare(_T("CUB00090")) == 0)) {
						m_SyzUtil.val_to_bin(
							(unsigned char*)(KSURI_4_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn),
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
				else if (pTokurei->kind == DATAKIND_DATE) {
					char *date = kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn;
					date[0] = (char)strtol(pTokurei->str.Mid(0, 2), NULL, 16);
					date[1] = (char)strtol(pTokurei->str.Mid(2, 2), NULL, 16);
					date[2] = (char)strtol(pTokurei->str.Mid(4, 2), NULL, 16);
					date[3] = '\0';

					if ( pTokurei->itemId.Compare(_T("CUB00060")) == 0 ) {
                        date = KSURI_2_S_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn;
						date[0] = (char)strtol(pTokurei->str.Mid(0, 2), NULL, 16);
						date[1] = (char)strtol(pTokurei->str.Mid(2, 2), NULL, 16);
						date[2] = (char)strtol(pTokurei->str.Mid(4, 2), NULL, 16);
						date[3] = '\0';
					}

					if (pTokurei->itemId.Compare(_T("CUB00070")) == 0) {
						date = KSURI_2_E_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn;
						date[0] = (char)strtol(pTokurei->str.Mid(0, 2), NULL, 16);
						date[1] = (char)strtol(pTokurei->str.Mid(2, 2), NULL, 16);
						date[2] = (char)strtol(pTokurei->str.Mid(4, 2), NULL, 16);
						date[3] = '\0';
					}

				}
				else if ( (pTokurei->kind == DATAKIND_CHECK) 
					|| (pTokurei->kind == DATAKIND_SIGN)) {
					*(kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn) = pTokurei->sign1;
				}
				return 1;
			}

		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {
		if ( pTokurei->itemId == datainfo[tblidx].itemId ) {

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
int CShinKSisanUri::WriteData(int sn_seq, int version)
{

    ASSERT( m_pZmSub );
    if( m_pZmSub == NULL ) {
        return -1;
	}

    CString filter;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TOKUREI_1_HYOID, version);
   
    if( m_pDBSyzAccess->m_pSyz_Tokurei ) {
        m_pDBSyzAccess->m_pSyz_Tokurei->Requery(filter);
	}
    else {
        if( m_pDBSyzAccess->Syz_TokureiOpen(m_pZmSub->m_database, filter) ) {
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
        for( int i = 0;; i++){

            if( i == 0 ) {
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

void CShinKSisanUri::WriteSub_data(CSYZ_TOKUREI* pTokurei, int sn_seq, int version)
{

	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_1; idx_jigyokbn++) {

			pTokurei->AddNew();
			pTokurei->sn_seq = sn_seq;
			pTokurei->version = version;
			pTokurei->hyoId = ICS_SN_TOKUREI_1_HYOID;
			pTokurei->itemId = kbndatainfo[tblidx].itemId;
			pTokurei->idx = idx_jigyokbn;
			pTokurei->kind = kbndatainfo[tblidx].kind;
			pTokurei->sign1 = 0;

			// 初期化
			pTokurei->str.Empty();

			if (pTokurei->kind == DATAKIND_STR) {
				pTokurei->str = kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn;
			}
			else if (pTokurei->kind == DATAKIND_VAL) {

				if ((pTokurei->itemId.Compare(_T("CUB00080")) == 0) && KSURI_5_CHECK[idx_jigyokbn]) {
					m_SyzUtil.val_to_asci(&pTokurei->val,
						(unsigned char*)KSURI_3_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn);
				}
				else if (pTokurei->itemId.Compare(_T("CUB00090")) == 0 && KSURI_5_CHECK[idx_jigyokbn]) {
					m_SyzUtil.val_to_asci(&pTokurei->val,
						(unsigned char*)KSURI_4_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn);
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
			else if (pTokurei->kind == DATAKIND_DATE) {

				char date[4] = { 0 };
				if ((pTokurei->itemId.Compare(_T("CUB00060")) == 0) && KSURI_5_CHECK[idx_jigyokbn]) {
					memcpy(date, KSURI_2_S_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn, kbndatainfo[tblidx].size);
				}
				else if ((pTokurei->itemId.Compare(_T("CUB00070")) == 0) && KSURI_5_CHECK[idx_jigyokbn]) {
					memcpy(date, KSURI_2_E_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn, kbndatainfo[tblidx].size);
				}
				else {
					memcpy(date, kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn, kbndatainfo[tblidx].size);
				}
				if ((date[0] != 0) && (date[1] != 0) && (date[2] != 0)) {
					pTokurei->str.Format(_T("%02x%02x%02x%02x"), date[0], date[1], date[2], date[3]);
				}

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
		pTokurei->hyoId = ICS_SN_TOKUREI_1_HYOID;
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

int CShinKSisanUri::Update_data(CSYZ_TOKUREI* pTokurei)
{

	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_1; idx_jigyokbn++) {

			if ((pTokurei->itemId == kbndatainfo[tblidx].itemId) && (pTokurei->idx == idx_jigyokbn)) {
				pTokurei->Edit();

				// 初期化
				pTokurei->str.Empty();

				if (pTokurei->kind == DATAKIND_STR) {
					pTokurei->str = kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn;
				}
				else if (pTokurei->kind == DATAKIND_VAL) {

					if ((pTokurei->itemId.Compare(_T("CUB00080")) == 0) && KSURI_5_CHECK[idx_jigyokbn]) {
						m_SyzUtil.val_to_asci(&pTokurei->val,
                            (unsigned char*)KSURI_3_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn);
					}
                    else if ( pTokurei->itemId.Compare(_T("CUB00090")) == 0 && KSURI_5_CHECK[idx_jigyokbn]) {
						m_SyzUtil.val_to_asci(&pTokurei->val,
							(unsigned char*)KSURI_4_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn);
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
				else if (pTokurei->kind == DATAKIND_DATE) {

					char date[4] = { 0 };
					if ((pTokurei->itemId.Compare(_T("CUB00060")) == 0) && KSURI_5_CHECK[idx_jigyokbn]) {
						memcpy(date, KSURI_2_S_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn, kbndatainfo[tblidx].size);
					}
                    else if ((pTokurei->itemId.Compare(_T("CUB00070")) == 0) && KSURI_5_CHECK[idx_jigyokbn]) {
						memcpy(date, KSURI_2_E_TMP[0] + MONY_BUF_SIZE * idx_jigyokbn, kbndatainfo[tblidx].size);
					}
                    else {
						memcpy(date, kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * idx_jigyokbn, kbndatainfo[tblidx].size);
				    }
					if ((date[0] != 0) && (date[1] != 0) && (date[2] != 0)) {
						pTokurei->str.Format(_T("%02x%02x%02x%02x"), date[0], date[1], date[2], date[3]);
					}

				}
				else if ((pTokurei->kind == DATAKIND_CHECK) 
					|| (pTokurei->kind == DATAKIND_SIGN) ) {
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
int CShinKSisanUri::GetMxPrtPage()
{
    int PageNum = 1;

	if (m_pDBSyzAccess->m_pSyz_Tokurei == NULL) {
        return PageNum;
	}

	CString sq, subsq;
	CSYZ_TOKUREI* pSyzTokurei = m_pDBSyzAccess->m_pSyz_Tokurei;

    for(int page = 2; page <= KSISANURI_PRT_PAGE_MAX; page++){

        sq.Empty();
        subsq.Empty();

		subsq.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d "), m_SnSeq, ICS_SN_TOKUREI_1_HYOID, TOKUREI_TBL_VERSION);
        sq += subsq;
		subsq.Format(_T("and %d <= idx and idx <= %d "), (page-1)*JIGYO_PER_PAGE_URI, page*JIGYO_PER_PAGE_URI-1);
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
// 各入力データ抽出
//-----------------------------------------------------------------------------
void CShinKSisanUri::GetAllDiagData()
{

	DIAGRAM_DATA DiagData;
	memset(DiagData.data_day, '\0', sizeof(DiagData.data_day));
	DiagData.data_check = 0;
	DiagData.data_imgdata = NULL;

	for (int tblidx = 0; tblidx < (sizeof(kbndatainfo) / sizeof(kbndatainfo[0])); tblidx++) {
		for (int clmidx = 0; clmidx < JIGYO_PER_PAGE_URI; clmidx++) {

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
			else if (kbndatainfo[tblidx].kind == DATAKIND_DATE) {
                if( DiagData.data_day[0] != '\0') {
					memmove(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size,
						DiagData.data_day, kbndatainfo[tblidx].size);
				}
			}
			else if (kbndatainfo[tblidx].kind == DATAKIND_CHECK) {
				*(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size) = (char)DiagData.data_check;
			}

		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {
		diag_getdata(IDC_ICSDIAG8CTRL2, datainfo[tblidx].diagidx[0], (struct IUnknown*)&DiagData, CIcsdiagctrl);
        if( datainfo[tblidx].kind == DATAKIND_VAL) {
			memmove(datainfo[tblidx].data, DiagData.data_val, MONY_BUF_SIZE);
		}
	}

}


//-----------------------------------------------------------------------------
// 課税期間
//-----------------------------------------------------------------------------
void CShinKSisanUri::SetKazeikikan()
{

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char yy, mm, dd;
	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd);
	DiagData.data_disp.Format(_T("%02x.%02x.%02x"), (yy & 0xff), (mm & 0xff), (dd & 0xff));
	diag_setdata(IDC_ICSDIAG8CTRL2, KSISANURI_DIAGCTRLID_KAZEI_S, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd);
	DiagData.data_disp.Format(_T("%02x.%02x.%02x"), (yy & 0xff), (mm & 0xff), (dd & 0xff));
	diag_setdata(IDC_ICSDIAG8CTRL2, KSISANURI_DIAGCTRLID_KAZEI_E, (struct IUnknown *)&DiagData, CIcsdiagctrl);

}

//-----------------------------------------------------------------------------
// 適用対象期間
//-----------------------------------------------------------------------------
void CShinKSisanUri::SetTekiyoTaisyokikan()
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
	diag_setdata(IDC_ICSDIAG8CTRL2, KSISANURI_DIAGCTRLID_TEKIYO_S, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	if ((TOKUREI_KIKAN_S <= m_tekiyotaisyokikan_e) && (m_tekiyotaisyokikan_e <= TOKUREI_KIKAN_E)) {
		voldate.db_datecnvGen(s_gengo, m_tekiyotaisyokikan_e, &d_gengo, &tekiyotaisyokikan_e, 0, 0);
	}
	else if ((m_tekiyotaisyokikan_s <= TOKUREI_KIKAN_E) && (TOKUREI_KIKAN_E <= m_tekiyotaisyokikan_e)) {
		voldate.db_datecnvGen(s_gengo, TOKUREI_KIKAN_E, &d_gengo, &tekiyotaisyokikan_e, 0, 0);
	}
	tmpstr.Format(_T("%08d"), tekiyotaisyokikan_e);
	DiagData.data_disp.Format(_T("%02s.%02s.%02s"), tmpstr.Mid(2, 2), tmpstr.Mid(4, 2), tmpstr.Mid(6, 2));
	diag_setdata(IDC_ICSDIAG8CTRL2, KSISANURI_DIAGCTRLID_TEKIYO_E, (struct IUnknown *)&DiagData, CIcsdiagctrl);

}

//-----------------------------------------------------------------------------
// 氏名又は名称
//-----------------------------------------------------------------------------
void CShinKSisanUri::SetName()
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
	diag_setdata(IDC_ICSDIAG8CTRL2, KSISANURI_DIAGCTRLID_NAME, (struct IUnknown *)&DiagData, CIcsdiagctrl);

}

void CShinKSisanUri::Set_KSURI_data()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char buf[64] = { 0 };
    char val[6] = { 0 };

	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int idx = 0; idx < JIGYO_PER_PAGE_URI; idx++) {

            if( kbndatainfo[tblidx].kind == DATAKIND_STR ) {
				DiagData.data_edit.Empty();
				memset(buf, '\0', sizeof(buf));
				memmove(buf, kbndatainfo[tblidx].data + (m_dispCol_left + idx) * kbndatainfo[tblidx].size, kbndatainfo[tblidx].size);

                char* p = kbndatainfo[tblidx].data + (m_dispCol_left + idx) * kbndatainfo[tblidx].size;

				DiagData.data_edit = buf;
				m_KsisanUriDiag02.SetData(kbndatainfo[tblidx].diagidx[idx], (struct IUnknown*)&DiagData);

				// 簡易課税・連動
				if ( ( m_kazeihousiki == ID_ICSSH_KANNI ) && ( m_TokureiMode == TOKUREIMODE_RENDO ) ) {
					// 事業区分が空欄でないは、50%適用チェック可能
					if ( (DiagData.data_edit.GetLength() != 0) && !((*m_pSnHeadData)->Sn_Sign4 & 0x01) ) {
						ATRIB_MOD ( IDC_ICSDIAG8CTRL2, idx_ksuri_5_check[idx], 1, ( *m_pSnHeadData )->Sn_Sign4, 0 );
					}
				}

				SetSub_KSURI_InputLockColor(kbndatainfo[tblidx].diagidx[idx]);
			}
			else if (kbndatainfo[tblidx].kind == DATAKIND_VAL) {

                SetSub_KSURI_data_kindval(tblidx, idx);
				SetSub_KSURI_InputLockColor(kbndatainfo[tblidx].diagidx[idx]);
			}
			else if (kbndatainfo[tblidx].kind == DATAKIND_RATE) {

				memset(&val, 0, sizeof(val));
                memmove(val, kbndatainfo[tblidx].data + (m_dispCol_left + idx) * kbndatainfo[tblidx].size, kbndatainfo[tblidx].size);
				m_pArith->l_print(val, val, _T("sss9"));
                DiagData.data_edit = val;
				diag_setdata(IDC_ICSDIAG8CTRL2, kbndatainfo[tblidx].diagidx[idx], (struct IUnknown*)&DiagData, CIcsdiagctrl);
				SetSub_KSURI_InputLockColor(kbndatainfo[tblidx].diagidx[idx]);
			}
			else if (kbndatainfo[tblidx].kind == DATAKIND_DATE) {

				memset(DiagData.data_day, '\0', sizeof(DiagData.data_day));

                if( !KSURI_5_CHECK[0 + (m_dispCol_left + idx)] ) {
					char *date;
					date = kbndatainfo[tblidx].data + (m_dispCol_left + idx) * kbndatainfo[tblidx].size, kbndatainfo[tblidx].size;
					memmove(DiagData.data_day, date, sizeof(DiagData.data_day));
				}

				m_KsisanUriDiag02.SetData(kbndatainfo[tblidx].diagidx[idx], (struct IUnknown*)&DiagData);
				SetSub_KSURI_InputLockColor(kbndatainfo[tblidx].diagidx[idx]);

			}
			else if (kbndatainfo[tblidx].kind == DATAKIND_CHECK) {

				DiagData.data_check = *(kbndatainfo[tblidx].data + (m_dispCol_left + idx) * kbndatainfo[tblidx].size);
                m_KsisanUriDiag02.SetData(kbndatainfo[tblidx].diagidx[idx], (struct IUnknown*)&DiagData);

                short Flg;
                if( ((*m_pSnHeadData)->Sn_Sign4 & 0x01) || (m_TokureiMode == TOKUREIMODE_RENDO)) {
                    Flg = 0;
				}
				else {
					Flg = !DiagData.data_check;
				}

				int idx_234[4] = { idx_ksuri_2_s[idx] , idx_ksuri_2_e[idx], idx_ksuri_3[idx], idx_ksuri_4[idx] };
				for (int i = 0; i < sizeof(idx_234) / sizeof(idx_234[0]); i++) {
					ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_234[i], Flg, (*m_pSnHeadData)->Sn_Sign4, 0);
					SetSub_KSURI_InputLockColor(idx_234[i]);
				}
				SetSub_KSURI_InputLockColor(kbndatainfo[tblidx].diagidx[idx]);

			}

		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {
		dsp_prs(IDC_ICSDIAG8CTRL2, datainfo[tblidx].diagidx[0], datainfo[tblidx].data);
		SetSub_KSURI_InputLockColor(datainfo[tblidx].diagidx[0]);
	}

	if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) { 
        //入力ロック中.
        return;
	}

	//yoshida190816
	//場所変更
	// 付表1-1 1-1D欄 または 付表4-1 1-1D欄へ
	// 付表1-1 1-1E欄 または 付表4-1 1-1E欄へ
	if( !((CH31HyoView*)m_pParent)->IsCrossOverR050930() ){	// 20231025 令和5年9月30日をまたぐ場合はセットしない
		(*m_pSyzSyukei)->SetShinkokuData(_T("312202"), KSURI_10);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312203"), KSURI_11);
	}

	if (m_kazeihousiki == ID_ICSSH_KANNI) {

		if( !((CH31HyoView*)m_pParent)->IsCrossOverR050930() ){	// 20231025 令和5年9月30日をまたぐ場合はセットしない
			(*m_pSyzSyukei)->SetShinkokuData(_T("341902"), KSURI_10);
			(*m_pSyzSyukei)->SetShinkokuData(_T("341903"), KSURI_11);
		}

		// 付表5-1 7～12DE欄へ
		if (m_TokureiMode == TOKUREIMODE_RENDO) {
			char zbuf[6] = { 0 };

			//---->yoshida190919
			char KSURI_6_FHYO5[MONY_BUF_SIZE] = { 0 };//返還金額計算値
			char KSURI_7_FHYO5[MONY_BUF_SIZE] = { 0 };
			//<-----------------

			(*m_pSyzSyukei)->SetShinkokuData(_T("357702"), zbuf);
			(*m_pSyzSyukei)->SetShinkokuData(_T("357703"), zbuf);
			(*m_pSyzSyukei)->SetShinkokuData(_T("357802"), zbuf);
			(*m_pSyzSyukei)->SetShinkokuData(_T("357803"), zbuf);
			(*m_pSyzSyukei)->SetShinkokuData(_T("357902"), zbuf);
			(*m_pSyzSyukei)->SetShinkokuData(_T("357903"), zbuf);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358002"), zbuf);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358003"), zbuf);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358102"), zbuf);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358103"), zbuf);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358202"), zbuf);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358203"), zbuf);
			for (int i = 0; i < JIGYO_MAX_TOKUREI_1; i++) {

				
				//---->yoshida190919
				memset(KSURI_6_FHYO5, '\0', sizeof(KSURI_6_FHYO5));
				memset(KSURI_7_FHYO5, '\0', sizeof(KSURI_7_FHYO5));

				m_pArith->l_sub(&KSURI_6_FHYO5, KSURI_6[i], KSURI_NEBIKI_KEI8[i]);
				m_pArith->l_sub(&KSURI_7_FHYO5, KSURI_7[i], KSURI_NEBIKI_10[i]);

				switch (KSURI_KBN[i]) {
					case	1:
						(*m_pSyzSyukei)->SetShinkokuData(_T("357702"), KSURI_6_FHYO5);
						(*m_pSyzSyukei)->SetShinkokuData(_T("357703"), KSURI_7_FHYO5);
						break;
					case	2:
						(*m_pSyzSyukei)->SetShinkokuData(_T("357802"), KSURI_6_FHYO5);
						(*m_pSyzSyukei)->SetShinkokuData(_T("357803"), KSURI_7_FHYO5);
						break;
					case	3:
						(*m_pSyzSyukei)->SetShinkokuData(_T("357902"), KSURI_6_FHYO5);
						(*m_pSyzSyukei)->SetShinkokuData(_T("357903"), KSURI_7_FHYO5);
						break;
					case	4:
						(*m_pSyzSyukei)->SetShinkokuData(_T("358002"), KSURI_6_FHYO5);
						(*m_pSyzSyukei)->SetShinkokuData(_T("358003"), KSURI_7_FHYO5);
						break;
					case	5:
						(*m_pSyzSyukei)->SetShinkokuData(_T("358102"), KSURI_6_FHYO5);
						(*m_pSyzSyukei)->SetShinkokuData(_T("358103"), KSURI_7_FHYO5);
						break;
					case	6:
						(*m_pSyzSyukei)->SetShinkokuData(_T("358202"), KSURI_6_FHYO5);
						(*m_pSyzSyukei)->SetShinkokuData(_T("358203"), KSURI_7_FHYO5);
						break;
				}


				//-------------------
				//switch (KSURI_KBN[i]) {
				//case	1:
				//	(*m_pSyzSyukei)->SetShinkokuData(_T("357702"), KSURI_6[i]);
				//	(*m_pSyzSyukei)->SetShinkokuData(_T("357703"), KSURI_7[i]);
				//	break;
				//case	2:
				//	(*m_pSyzSyukei)->SetShinkokuData(_T("357802"), KSURI_6[i]);
				//	(*m_pSyzSyukei)->SetShinkokuData(_T("357803"), KSURI_7[i]);
				//	break;
				//case	3:
				//	(*m_pSyzSyukei)->SetShinkokuData(_T("357902"), KSURI_6[i]);
				//	(*m_pSyzSyukei)->SetShinkokuData(_T("357903"), KSURI_7[i]);
				//	break;
				//case	4:
				//	(*m_pSyzSyukei)->SetShinkokuData(_T("358002"), KSURI_6[i]);
				//	(*m_pSyzSyukei)->SetShinkokuData(_T("358003"), KSURI_7[i]);
				//	break;
				//case	5:
				//	(*m_pSyzSyukei)->SetShinkokuData(_T("358102"), KSURI_6[i]);
				//	(*m_pSyzSyukei)->SetShinkokuData(_T("358103"), KSURI_7[i]);
				//	break;
				//case	6:
				//	(*m_pSyzSyukei)->SetShinkokuData(_T("358202"), KSURI_6[i]);
				//	(*m_pSyzSyukei)->SetShinkokuData(_T("358203"), KSURI_7[i]);
				//	break;
				//}
				//<--------------------
			}
			(*m_pSyzSyukei)->ResetShinkokuData(_T("358402"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("358403"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("358502"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("358503"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("358602"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("358603"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("358702"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("358703"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("358802"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("358803"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("358902"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("358903"));


			//----->yoshida190919
			char HENKAN_KEI8_FHYO4[MONY_BUF_SIZE] = { 0 };//返還転記金額軽減８％
			char HENKAN_10_FHYO4[MONY_BUF_SIZE] = { 0 };//返還転記金額１０％

			char KASISON_KEI8_FHYO4[MONY_BUF_SIZE] = { 0 };//貸倒損失転記金額軽減８％
			char KASISON_10_FHYO4[MONY_BUF_SIZE] = { 0 };//貸倒損失転記金額１０％

			char KASIKAI_KEI8_FHYO4[MONY_BUF_SIZE] = { 0 };//貸倒回収転記金額軽減８％
			char KASIKAI_10_FHYO4[MONY_BUF_SIZE] = { 0 };//貸倒回収転記金額１０％

			char KSURI_10_FHYO4[MONY_BUF_SIZE] = { 0 };
			char KSURI_11_FHYO4[MONY_BUF_SIZE] = { 0 };

			char val[MONY_BUF_SIZE] = { 0 } , val2[MONY_BUF_SIZE] = { 0 };
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

			memmove( &KSURI_10_FHYO4, &KSURI_10, MONY_BUF_SIZE );
			memmove( &KSURI_11_FHYO4, &KSURI_11, MONY_BUF_SIZE );
			
			for (int i = 0; i < JIGYO_MAX_TOKUREI_1; i++) {
				//各事業毎を計算して足していく

				//++++++++返還等対価に転記（計算）+++++++//
				memset(&val, 0, sizeof(val));
				memset(&val2, 0, sizeof(val2));

				memmove(val, &KSURI_NEBIKI_KEI8_FHYO1[i], MONY_BUF_SIZE);
				memmove(val2, &KSURI_NEBIKI_10_FHYO1[i], MONY_BUF_SIZE);

				m_pArith->l_mul(&val, &val, val624);
				m_pArith->l_div(&val, &val, val100);
				m_pArith->l_div(&val, &val, val108);

				m_pArith->l_mul(&val2, &val2, val780);
				m_pArith->l_div(&val2, &val2, val100);
				m_pArith->l_div(&val2, &val2, val110);

				m_pArith->l_add(&HENKAN_KEI8_FHYO4, &HENKAN_KEI8_FHYO4, &val);
				m_pArith->l_add(&HENKAN_10_FHYO4, &HENKAN_10_FHYO4, &val2);


				//++++++++貸倒損失に転記（計算）+++++++//
				memset(&val, 0, sizeof(val));
				memset(&val2, 0, sizeof(val2));

				memmove(val, &KSURI_KASISON_KEI8[i], MONY_BUF_SIZE);
				memmove(val2, &KSURI_KASISON_10[i], MONY_BUF_SIZE);

				m_pArith->l_mul(&val, &val, val624);
				m_pArith->l_div(&val, &val, val100);
				m_pArith->l_div(&val, &val, val108);

				m_pArith->l_mul(&val2, &val2, val780);
				m_pArith->l_div(&val2, &val2, val100);
				m_pArith->l_div(&val2, &val2, val110);

				m_pArith->l_add(&KASISON_KEI8_FHYO4, &KASISON_KEI8_FHYO4, &val);
				m_pArith->l_add(&KASISON_10_FHYO4, &KASISON_10_FHYO4, &val2);

				//+++++++貸倒回収に転記（計算）++++++++++//
				memset(&val, 0, sizeof(val));
				memset(&val2, 0, sizeof(val2));

				memmove(val, &KSURI_KASIKAISYU_KEI8[i], MONY_BUF_SIZE);
				memmove(val2, &KSURI_KASIKAISYU_10[i], MONY_BUF_SIZE);

				m_pArith->l_mul(&val, &val, val624);
				m_pArith->l_div(&val, &val, val100);
				m_pArith->l_div(&val, &val, val108);

				m_pArith->l_mul(&val2, &val2, val780);
				m_pArith->l_div(&val2, &val2, val100);
				m_pArith->l_div(&val2, &val2, val110);

				m_pArith->l_add(&KASIKAI_KEI8_FHYO4, &KASIKAI_KEI8_FHYO4, &val);
				m_pArith->l_add(&KASIKAI_10_FHYO4, &KASIKAI_10_FHYO4, &val2);

				m_pArith->l_sub( &KSURI_10_FHYO4, KSURI_10_FHYO4, KSURI_NEBIKI_KEI8[i] );
				m_pArith->l_sub( &KSURI_11_FHYO4, KSURI_11_FHYO4, KSURI_NEBIKI_10[i] );

			}

			//++++++++返還等対価に転記+++++++//
			(*m_pSyzSyukei)->SetShinkokuData(_T("342302"), HENKAN_KEI8_FHYO4);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342303"), HENKAN_10_FHYO4);
			(*m_pSyzSyukei)->SetShinkokuData(_T("312702"), HENKAN_KEI8_FHYO4);
			(*m_pSyzSyukei)->SetShinkokuData(_T("312703"), HENKAN_10_FHYO4);

			//++++++++貸倒損失に転記+++++++//
			(*m_pSyzSyukei)->SetShinkokuData(_T("342402"), KASISON_KEI8_FHYO4);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342403"), KASISON_10_FHYO4);
			(*m_pSyzSyukei)->SetShinkokuData(_T("313002"), KASISON_KEI8_FHYO4);
			(*m_pSyzSyukei)->SetShinkokuData(_T("313003"), KASISON_10_FHYO4);

			//+++++++貸倒回収に転記++++++++++//
			(*m_pSyzSyukei)->SetShinkokuData(_T("342102"), KASIKAI_KEI8_FHYO4);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342103"), KASIKAI_10_FHYO4);
			(*m_pSyzSyukei)->SetShinkokuData(_T("325002"), KASIKAI_KEI8_FHYO4);
			(*m_pSyzSyukei)->SetShinkokuData(_T("325003"), KASIKAI_10_FHYO4);

			//<------

			// 簡易課税で第5-(1)号様式を使用している場合、
			// 簡易本表のこの課税期間の課税売上高が特例帳票の金額を考慮していなかったため修正
			( *m_pSyzSyukei )->SetShinkokuData( _T( "322602" ), KSURI_10_FHYO4 );
			( *m_pSyzSyukei )->SetShinkokuData( _T( "322603" ), KSURI_11_FHYO4 );
			( *m_pSyzSyukei )->SetShinkokuData( _T( "911104" ), KSURI_10_FHYO4 );
			( *m_pSyzSyukei )->SetShinkokuData( _T( "911105" ), KSURI_11_FHYO4 );
		}
	}

	else {//原則

		//yoshida190821
		//付表２－１にも転記する（仮）
		//「911104」「911105」とかはいらない？？
		//付表２側変更出来てないので一旦コメント。正確に仕様決めて対応
		if (m_TokureiMode == TOKUREIMODE_RENDO) {//連動モード

			//---->yoshida190919
			char KSURI_10_FHYO2[MONY_BUF_SIZE] = { 0 };//返還金額計算値
			char KSURI_11_FHYO2[MONY_BUF_SIZE] = { 0 };

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
			m_pArith->l_sub(&KSURI_10_FHYO2, KSURI_10, KSURI_NEBIKI_KEI8[0]);
			m_pArith->l_sub(&KSURI_11_FHYO2, KSURI_11, KSURI_NEBIKI_10[0]);

			(*m_pSyzSyukei)->SetShinkokuData(_T("322602"), KSURI_10_FHYO2);
			(*m_pSyzSyukei)->SetShinkokuData(_T("322603"), KSURI_11_FHYO2);
			(*m_pSyzSyukei)->SetShinkokuData(_T("911104"), KSURI_10_FHYO2);
			(*m_pSyzSyukei)->SetShinkokuData(_T("911105"), KSURI_11_FHYO2);
			//------
//			(*m_pSyzSyukei)->SetShinkokuData(_T("322602"), KSURI_10);
//			(*m_pSyzSyukei)->SetShinkokuData(_T("322603"), KSURI_11);
			//<-----
		}
	}

}

void CShinKSisanUri::SetSub_KSURI_InputLockColor(short index)
{

	if ( !((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {
        //入力ロック中でなければreturn;
        return;
	}

	DIAGRAM_ATTRIBUTE DA;
	diag_getattr(IDC_ICSDIAG8CTRL2, index, &DA, CIcsdiagctrl);

	// 簡易課税マスターで連動の時は白背景で表示
	if( (m_kazeihousiki == ID_ICSSH_KANNI) && (m_TokureiMode == TOKUREIMODE_RENDO) ){
		DA.attr_bcolor = BC_WHITE;
		diag_setattr( IDC_ICSDIAG8CTRL2, index, &DA, FALSE, CIcsdiagctrl );
	}

    switch( m_TokureiMode ) {
        case TOKUREIMODE_HIRENDO:
		case TOKUREIMODE_HANRENDO:
			if (ID_KSURI_5_1 <= index && index <= ID_KSURI_5_3) {
				DA.attr_bcolor = BC_WHITE;
			}
			else if (index == ID_KSURI_10 || index == ID_KSURI_11) {
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

void CShinKSisanUri::SetSub_KSURI_data_kindval(int tblidx, int clmidx)
{
	if ( (kbndatainfo[tblidx].itemId.Compare(_T("CUB00080")) == 0) 
		|| (kbndatainfo[tblidx].itemId.Compare(_T("CUB00090")) == 0)) {

		if (KSURI_5_CHECK[0 + m_dispCol_left + clmidx]) {
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

		if (kbndatainfo[tblidx].itemId.Compare(_T("CUB00110")) == 0) {
			CheckSetData_KSURI_6(idx_ksuri_6[clmidx], clmidx);
		}
        else if(kbndatainfo[tblidx].itemId.Compare(_T("CUB00120")) == 0) {
			CheckSetData_KSURI_7(idx_ksuri_7[clmidx], clmidx);
		}

	}
}

void CShinKSisanUri::Update_KSURI_5(short index, int clmidx)
{

	//軽減売上割合を再計算.
	unsigned char keigenwariai[MONY_BUF_SIZE];

	char val_ksuri3[MONY_BUF_SIZE], val_ksuri4[MONY_BUF_SIZE];

	memmove(val_ksuri3, KSURI_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);
	memmove(val_ksuri4, KSURI_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);

	if (m_pArith->l_test(val_ksuri3) && m_pArith->l_test(val_ksuri4)) {
		m_pArith->l_par100(keigenwariai, (unsigned char*)val_ksuri4, (unsigned char*)val_ksuri3, 0, 0);
	}
	else {
		memset(keigenwariai, '\0', MONY_BUF_SIZE);
	}

	memmove(KSURI_5[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, keigenwariai, MONY_BUF_SIZE);
}

void CShinKSisanUri::Update_KSURI_6(short index, int clmidx, bool bDel)
{
    if( KSURI_6_SIGN[m_dispCol_left + clmidx] && !bDel) {
        return;
	}

	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSURI_6(index, clmidx, &val[0]);
	memmove(KSURI_6[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);

	CheckSub_ChangeBackColor(idx_ksuri_6[clmidx], NULL, NULL, true);
}

void CShinKSisanUri::Update_KSURI_6_KEI()
{
	memset(&KSURI_6_KEI, 0, sizeof(KSURI_6_KEI));
	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_1; idx_jigyokbn++) {
		m_pArith->l_add(&KSURI_6_KEI, &KSURI_6_KEI, KSURI_6[idx_jigyokbn]);
	}
}

void CShinKSisanUri::Update_KSURI_7(short index, int clmidx, bool bDel)
{

	if (KSURI_7_SIGN[m_dispCol_left + clmidx] && !bDel) {
		return;
	}

	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSURI_7(index, clmidx, &val[0]);
	memmove(KSURI_7[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	CheckSub_ChangeBackColor(idx_ksuri_7[clmidx], NULL, NULL, true);

}

void CShinKSisanUri::Update_KSURI_7_KEI()
{
	memset(&KSURI_7_KEI, 0, sizeof(KSURI_7_KEI));
	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_1; idx_jigyokbn++) {
		m_pArith->l_add(&KSURI_7_KEI, &KSURI_7_KEI, KSURI_7[idx_jigyokbn]);
	}
}

void CShinKSisanUri::Update_KSURI_10()
{
	char val[MONY_BUF_SIZE] = { 0 };
	memset(&val, 0, sizeof(val));
	m_pArith->l_add(&val, &val, KSURI_6_KEI);
	m_pArith->l_add(&val, &val, KSURI_8);
	memmove(KSURI_10, &val, MONY_BUF_SIZE);

	//yoshida190816
	//場所変更
	// 付表1-1 1-1D欄 または 付表4-1 1-1D欄へ
	//( *m_pSyzSyukei )->SetShinkokuData ( _T( "312202" ), KSURI_10 );
	//if ( m_kazeihousiki == ID_ICSSH_KANNI ) {
	//	( *m_pSyzSyukei )->SetShinkokuData ( _T( "341902" ), KSURI_10 );
	//}
}

void CShinKSisanUri::Update_KSURI_11()
{
	char val[MONY_BUF_SIZE] = { 0 };
	memset(&val, 0, sizeof(val));
	m_pArith->l_add(&val, &val, KSURI_7_KEI);
	m_pArith->l_add(&val, &val, KSURI_9);
	memmove(KSURI_11, &val, MONY_BUF_SIZE);

	//yoshida190816
	//場所変更
	// 付表1-1 1-1E欄 または 付表4-1 1-1E欄へ
	//( *m_pSyzSyukei )->SetShinkokuData ( _T( "312203" ), KSURI_11 );
	//if ( m_kazeihousiki == ID_ICSSH_KANNI ) {
	//	( *m_pSyzSyukei )->SetShinkokuData ( _T( "341903" ), KSURI_11 );
	//}
}

void CShinKSisanUri::Check_KSURI_5Check(short index, int clmidx)
{

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	m_KsisanUriDiag02.GetData((int)index, (struct IUnknown *)&DiagData);

	KSURI_5_CHECK[0 + m_dispCol_left + clmidx] = DiagData.data_check;

	if (DiagData.data_check) {
		//チェックON
        CheckSub_KSURI_5CheckOn(index, clmidx);
	}
    else {
		//チェックOFF
		CheckSub_KSURI_5CheckOff(index, clmidx);
	}

	Set_KSURI_data();

}

void CShinKSisanUri::CheckSub_KSURI_5CheckOn(short index, int clmidx, bool bDel)
{

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

    //一時データ保存.
	memmove(KSURI_2_S_TMP[0] + (m_dispCol_left + clmidx) * 4, KSURI_2_S[0] + (m_dispCol_left + clmidx) * 4, 4);
	memmove(KSURI_2_E_TMP[0] + (m_dispCol_left + clmidx) * 4, KSURI_2_E[0] + (m_dispCol_left + clmidx) * 4, 4);
	memmove(KSURI_3_TMP[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);
	memmove(KSURI_4_TMP[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);

	//2.3.4入力禁止
	int idx_234[4] = { idx_ksuri_2_s[clmidx] , idx_ksuri_2_e[clmidx], idx_ksuri_3[clmidx], idx_ksuri_4[clmidx] };
	for (int i = 0; i < sizeof(idx_234) / sizeof(idx_234[0]); i++) {
		ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_234[i], 0, (*m_pSnHeadData)->Sn_Sign4, 0);
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

	//5を50%
	memmove(KSURI_5[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, val50, MONY_BUF_SIZE);

	//6
	if (bDel && (index == idx_ksuri_6[clmidx])) {
		Update_KSURI_6(index, clmidx, true);
	}
	else if (bDel && (index == idx_ksuri_7[clmidx])) {
		;
	}
	else {
		l_pardotnet(val, KSURI_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, val50, val100);
		m_pArith->l_mul(&val, &val, val100);
		m_pArith->l_div(&val, &val, val108);
		memmove(KSURI_6[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, val, MONY_BUF_SIZE);
	}

	//6合計
	Update_KSURI_6_KEI();

	//7
	if (bDel && (index == idx_ksuri_7[clmidx])) {
		Update_KSURI_7(index, clmidx, true);
	}
    else if (bDel && (index == idx_ksuri_6[clmidx])) {
		;
	}
    else {
		memset(&val, 0, sizeof(val));
		memmove(&val, KSURI_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);
		m_pArith->l_mul(&val, &val, val50);
		m_pArith->l_div(&val, &val, val100);
		m_pArith->l_sub(&val, KSURI_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val);
		m_pArith->l_mul(&val, &val, val100);
		m_pArith->l_div(&val, &val, val110);
		memmove(KSURI_7[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, val, MONY_BUF_SIZE);
	}

	//7合計
	Update_KSURI_7_KEI();

	Update_KSURI_10();
	Update_KSURI_11();

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

void CShinKSisanUri::CheckSub_KSURI_5CheckOn_KanniRendo(int clmidx)
{

	//一時データ保存.
	memmove(KSURI_2_S_TMP[0] + clmidx * 4, KSURI_2_S[0] + clmidx * 4, 4);
	memmove(KSURI_2_E_TMP[0] + clmidx * 4, KSURI_2_E[0] + clmidx * 4, 4);
	memmove(KSURI_3_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI_3[0] + clmidx * MONY_BUF_SIZE, MONY_BUF_SIZE);
	memmove(KSURI_4_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI_4[0] + clmidx * MONY_BUF_SIZE, MONY_BUF_SIZE);

	//2.3.4入力禁止
    if( clmidx < JIGYO_PER_PAGE_URI ) {
		int idx_234[4] = { idx_ksuri_2_s[clmidx] , idx_ksuri_2_e[clmidx], idx_ksuri_3[clmidx], idx_ksuri_4[clmidx] };
		for (int i = 0; i < sizeof(idx_234) / sizeof(idx_234[0]); i++) {
			ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_234[i], 0, (*m_pSnHeadData)->Sn_Sign4, 0);
		}
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

	//5を50%
	memmove(KSURI_5[0] + clmidx * MONY_BUF_SIZE, val50, MONY_BUF_SIZE);

	//6
	memmove(val, KSURI_1[clmidx], MONY_BUF_SIZE);
	l_pardotnet(val, val, val50, val100);
	m_pArith->l_mul(&val, &val, val100);
	m_pArith->l_div(&val, &val, val108);
	memmove(KSURI_6[0] + clmidx * MONY_BUF_SIZE, val, MONY_BUF_SIZE);

	//6合計
	Update_KSURI_6_KEI();

	//7
	memset(&val, 0, sizeof(val));
	memmove(val, KSURI_1[clmidx], MONY_BUF_SIZE);
	m_pArith->l_mul(&val, &val, val50);
	m_pArith->l_div(&val, &val, val100);
	m_pArith->l_sub(&val, KSURI_1[clmidx], &val);
	m_pArith->l_mul(&val, &val, val100);
	m_pArith->l_div(&val, &val, val110);
	memmove(KSURI_7[0] + clmidx * MONY_BUF_SIZE, val, MONY_BUF_SIZE);

	//7合計
	Update_KSURI_7_KEI();

	Update_KSURI_10();
	Update_KSURI_11();


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

void CShinKSisanUri::CheckSub_KSURI_5CheckOff(short index, int clmidx)
{
	int idx_234[4] = { idx_ksuri_2_s[clmidx] , idx_ksuri_2_e[clmidx], idx_ksuri_3[clmidx], idx_ksuri_4[clmidx] };

	//2.3.4入力許可.
	for (int i = 0; i < sizeof(idx_234) / sizeof(idx_234[0]); i++) {
		ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_234[i], 1, (*m_pSnHeadData)->Sn_Sign4, 0);
	}

    //一時データに戻す.
	memmove(KSURI_2_S[0] + (m_dispCol_left + clmidx) * 4, KSURI_2_S_TMP[0] + (m_dispCol_left + clmidx) * 4, 4);
	memmove(KSURI_2_E[0] + (m_dispCol_left + clmidx) * 4, KSURI_2_E_TMP[0] + (m_dispCol_left + clmidx) * 4, 4);
	memmove(KSURI_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI_3_TMP[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);
	memmove(KSURI_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI_4_TMP[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, MONY_BUF_SIZE);

    //5を再計算
	Update_KSURI_5(index, clmidx);

	Update_KSURI_6(index, clmidx, true);
	Update_KSURI_6_KEI();

	Update_KSURI_7(index, clmidx);
	Update_KSURI_7_KEI();

	Update_KSURI_10();
	Update_KSURI_11();


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

	l_pardotnet(&val[0], HENKAN_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);


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

	l_pardotnet(&val[0], KASISON_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);

	memset(&val2, 0, sizeof(val2));
	m_pArith->l_sub(&val2, KASISON_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val);

	memmove(KSURI_KASISON_KEI8[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	memmove(KSURI_KASISON_10[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val2, MONY_BUF_SIZE);


	//貸倒回収
	memset(&val, 0, sizeof(val));

	l_pardotnet(&val[0], KASIKAISYU_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, KSURI_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);

	memset(&val2, 0, sizeof(val2));
	m_pArith->l_sub(&val2, KASIKAISYU_GOKEI[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val);

	memmove(KSURI_KASIKAISYU_KEI8[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val, MONY_BUF_SIZE);
	memmove(KSURI_KASIKAISYU_10[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, &val2, MONY_BUF_SIZE);
	//<---


}

void CShinKSisanUri::Calc_KSURI_6(short index, int clmidx, char* pVal)
{

	char val50[MONY_BUF_SIZE], val100[MONY_BUF_SIZE], val108[MONY_BUF_SIZE];
	CString str50 = _T("50");
	CString str100 = _T("100");
	CString str108 = _T("108");

	m_pArith->l_input(&val50, str50.GetBuffer());
	m_pArith->l_input(&val100, str100.GetBuffer());
	m_pArith->l_input(&val108, str108.GetBuffer());

	memset(pVal, 0, MONY_BUF_SIZE);

	if (KSURI_5_CHECK[m_dispCol_left + clmidx]) {
		l_pardotnet(pVal,
			KSURI_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			val50,
			val100);
	}
    else {
		l_pardotnet(pVal,
			KSURI_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			KSURI_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			KSURI_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);
	}

	m_pArith->l_mul(pVal, pVal, val100);
	m_pArith->l_div(pVal, pVal, val108);

}

void CShinKSisanUri::Calc_KSURI_7(short index, int clmidx, char* pVal)
{

	char val50[MONY_BUF_SIZE], val100[MONY_BUF_SIZE], val110[MONY_BUF_SIZE];
	CString str50 = _T("50");
	CString str100 = _T("100");
	CString str110 = _T("110");

	m_pArith->l_input(&val50, str50.GetBuffer());
	m_pArith->l_input(&val100, str100.GetBuffer());
	m_pArith->l_input(&val110, str110.GetBuffer());

	memset(pVal, 0, MONY_BUF_SIZE);

	if (KSURI_5_CHECK[m_dispCol_left + clmidx]) {
		l_pardotnet(pVal,
			KSURI_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			val50,
			val100);
	}
    else {
		l_pardotnet(pVal,
			KSURI_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			KSURI_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE,
			KSURI_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE);
	}

	if ((m_pArith->l_test(KSURI_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE))
		&& (m_pArith->l_test(KSURI_4[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE))
		&& !(m_pArith->l_test(KSURI_3[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE))) {
		memset(pVal, 0, MONY_BUF_SIZE);
	}
	else {
		m_pArith->l_sub(pVal, KSURI_1[0] + (m_dispCol_left + clmidx) * MONY_BUF_SIZE, pVal);
	}

	m_pArith->l_mul(pVal, pVal, val100);
	m_pArith->l_div(pVal, pVal, val110);

}

void CShinKSisanUri::CheckSetData_KSURI_6(short index, int clmidx)
{
	//入力値取得.
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	diag_getdata(IDC_ICSDIAG8CTRL2, idx_ksuri_6[clmidx], (struct IUnknown*)&DiagData, CIcsdiagctrl);

    //計算値取得.
	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSURI_6(index, clmidx, &val[0]);

	KSURI_6_SIGN[m_dispCol_left + clmidx] = CheckSub_ChangeBackColor(idx_ksuri_6[clmidx], DiagData.data_val, val);
}

void CShinKSisanUri::CheckSetData_KSURI_7(short index, int clmidx)
{

	//入力値取得.
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
    memset(DiagData.data_val, '\0', sizeof(DiagData.data_val));
	diag_getdata(IDC_ICSDIAG8CTRL2, idx_ksuri_7[clmidx], (struct IUnknown*)&DiagData, CIcsdiagctrl);

	//計算値取得.
	char val[MONY_BUF_SIZE] = { 0 };
	Calc_KSURI_7(index, clmidx, &val[0]);

	KSURI_7_SIGN[m_dispCol_left + clmidx] = CheckSub_ChangeBackColor(idx_ksuri_7[clmidx], DiagData.data_val, val);
}

int CShinKSisanUri::CheckSub_ChangeBackColor(short index, char* pVal1, char* pVal2, bool bReset)
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

void CShinKSisanUri::ChangeDiagAttr()
{
	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {

		// 連動時、1列目以外は入力不可にする（原則課税のみ）(事業区分名)
		if ((m_kazeihousiki == ID_ICSSH_GENSOKU) && (m_TokureiMode == TOKUREIMODE_RENDO)) {
			if (m_dispCol_left == 0) {		// 1列目→入力OK
				ATRIB_MOD(IDC_ICSDIAG8CTRL2, ID_KSURI_TITLE1, 1, (*m_pSnHeadData)->Sn_Sign4, 1);
			}
			else if (m_dispCol_left > 0) {	// 1列目以外→入力不可
				ATRIB_MOD(IDC_ICSDIAG8CTRL2, ID_KSURI_TITLE1, 0, (*m_pSnHeadData)->Sn_Sign4, 1);
			}
		}

		// 連動時、1列目以外は入力不可にする（原則課税のみ）(５０％適用チェックボックス)
		if ((m_kazeihousiki == ID_ICSSH_GENSOKU) && (m_TokureiMode == TOKUREIMODE_RENDO)) {
			if (m_dispCol_left == 0) {		// 1列目→入力OK
				ATRIB_MOD(IDC_ICSDIAG8CTRL2, ID_KSURI_5CHECK_1, 1, (*m_pSnHeadData)->Sn_Sign4, 1);
			}
			else if (m_dispCol_left > 0) {	// 1列目以外→入力不可
				ATRIB_MOD(IDC_ICSDIAG8CTRL2, ID_KSURI_5CHECK_1, 0, (*m_pSnHeadData)->Sn_Sign4, 1);
			}
		}
		// 連動時、事業区分名が入力されている列以外は入力不可にする　（簡易課税のみ）
		else if ((m_kazeihousiki == ID_ICSSH_KANNI) && (m_TokureiMode == TOKUREIMODE_RENDO)) {
            short Connect[3] = {1,0,0};
			for (int idx = 0; idx < 3; idx++) {
				if (IsNULL_Column(m_dispCol_left + idx) == FALSE) {
					ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_ksuri_5_check[idx], 1, (*m_pSnHeadData)->Sn_Sign4, Connect[idx]);
				}
				else {
					ATRIB_MOD(IDC_ICSDIAG8CTRL2, idx_ksuri_5_check[idx], 0, (*m_pSnHeadData)->Sn_Sign4, Connect[idx]);
				}
			}
		}
	}
	else {
		// 連動時、入力ロックが押されているときは1列目のみグレー背景にする。それ以外は白背景
		DIAGRAM_ATTRIBUTE	DA;
		if (m_kazeihousiki == ID_ICSSH_GENSOKU) {	// 原則
			if (m_TokureiMode == TOKUREIMODE_RENDO) {
				if (m_dispCol_left == 0) {
					diag_getattr(IDC_ICSDIAG8CTRL2, ID_KSURI_TITLE1, &DA, CIcsdiagctrl);
					DA.attr_bcolor = BC_GRAY;
					diag_setattr(IDC_ICSDIAG8CTRL2, ID_KSURI_TITLE1, &DA, TRUE, CIcsdiagctrl);
					diag_getattr(IDC_ICSDIAG8CTRL2, ID_KSURI_5CHECK_1, &DA, CIcsdiagctrl);
					DA.attr_bcolor = BC_GRAY;
					diag_setattr(IDC_ICSDIAG8CTRL2, ID_KSURI_5CHECK_1, &DA, TRUE, CIcsdiagctrl);
				}
				else if (m_dispCol_left > 0) {
					diag_getattr(IDC_ICSDIAG8CTRL2, ID_KSURI_TITLE1, &DA, CIcsdiagctrl);
					DA.attr_bcolor = BC_WHITE;
					diag_setattr(IDC_ICSDIAG8CTRL2, ID_KSURI_TITLE1, &DA, TRUE, CIcsdiagctrl);
					diag_getattr(IDC_ICSDIAG8CTRL2, ID_KSURI_5CHECK_1, &DA, CIcsdiagctrl);
					DA.attr_bcolor = BC_WHITE;
					diag_setattr(IDC_ICSDIAG8CTRL2, ID_KSURI_5CHECK_1, &DA, TRUE, CIcsdiagctrl);
				}
			}
		}
		else {	// 簡易
			if (m_TokureiMode == TOKUREIMODE_RENDO) {
				diag_getattr(IDC_ICSDIAG8CTRL2, ID_KSURI_TITLE1, &DA, CIcsdiagctrl);
				DA.attr_bcolor = BC_WHITE;
				diag_setattr(IDC_ICSDIAG8CTRL2, ID_KSURI_TITLE1, &DA, TRUE, CIcsdiagctrl);
				diag_getattr(IDC_ICSDIAG8CTRL2, ID_KSURI_5CHECK_1, &DA, CIcsdiagctrl);
				DA.attr_bcolor = BC_WHITE;
				diag_setattr(IDC_ICSDIAG8CTRL2, ID_KSURI_5CHECK_1, &DA, TRUE, CIcsdiagctrl);
			}
		}
	}

}

void CShinKSisanUri::ClearData(bool bFromRenew)
{
	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_1; idx_jigyokbn++) {
            if( bFromRenew && kbndatainfo[tblidx].kind == DATAKIND_CHECK) {
                ;
			}
			else if(bFromRenew && kbndatainfo[tblidx].kind == DATAKIND_RATE){
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

	for (int idx_jigyokbn = 0; idx_jigyokbn < JIGYO_MAX_TOKUREI_1; idx_jigyokbn++) {
		memset(KSURI_2_S_TMP[0] + idx_jigyokbn * 4, '\0', 4);
		memset(KSURI_2_E_TMP[0] + idx_jigyokbn * 4, '\0', 4);
		memset(KSURI_3_TMP[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);
		memset(KSURI_4_TMP[0] + idx_jigyokbn * MONY_BUF_SIZE, '\0', MONY_BUF_SIZE);

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

	memset(KSURI_KBN, '\0', sizeof(KSURI_KBN));

    return;
}

//-----------------------------------------------------------------------------
//  画面出力
//-----------------------------------------------------------------------------
void CShinKSisanUri::DispKsisan_Uri()
{
	// 初期化
	for (int i = KSISANURI_DIAGCTRLID_NAME; i <= KSISANURI_DIAGCTRLID_TEKIYO_E; i++) {
		dsp_cls(IDC_ICSDIAG8CTRL2, (short)i);
	}
	for (int i = ID_KSURI_TITLE1; i <= ID_KSURI_11; i++ ) {
		dsp_cls(IDC_ICSDIAG8CTRL2, (short)i);
	}

	DispClmNum();
    SetKazeikikan();
    SetTekiyoTaisyokikan();
    SetName();
    Set_KSURI_data();

}

BEGIN_EVENTSINK_MAP(CShinKSisanUri, CSyzBaseDlg)
	ON_EVENT(CShinKSisanUri, IDC_ICSDIAG8CTRL2, 1, CShinKSisanUri::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinKSisanUri, IDC_ICSDIAG8CTRL2, 2, CShinKSisanUri::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinKSisanUri, IDC_ICSDIAG8CTRL2, 3, CShinKSisanUri::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinKSisanUri, IDC_ICSDIAG8CTRL2, 9, CShinKSisanUri::CheckButtonIcsdiag8ctrl2, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()


void CShinKSisanUri::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;
	m_Focus = index;

}

void CShinKSisanUri::EditOFFIcsdiag8ctrl2(short index)
{

	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

    memset(DiagData.data_day, '\0', sizeof(DiagData.data_day));
	m_KsisanUriDiag02.GetData((int)index, (struct IUnknown *)&DiagData);

	for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
		for (int clmidx = 0; clmidx < JIGYO_PER_PAGE_URI; clmidx++) {

			if (index == kbndatainfo[tblidx].diagidx[clmidx]) {

				if (kbndatainfo[tblidx].kind == DATAKIND_STR) {
					memmove(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size,
						DiagData.data_edit, kbndatainfo[tblidx].size);
				}
				else if (kbndatainfo[tblidx].kind == DATAKIND_VAL) {

					memmove(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size,
						DiagData.data_val, MONY_BUF_SIZE);

					if ( index == idx_ksuri_1[clmidx] ) {
						Update_KSURI_6(index, clmidx);
						Update_KSURI_6_KEI();
						Update_KSURI_7(index, clmidx);
						Update_KSURI_7_KEI();
						Update_KSURI_10();
						Update_KSURI_11();
					}
					else if (index == idx_ksuri_3[clmidx] || index == idx_ksuri_4[clmidx]) {
						//if ( KSURI_5_CHECK[clmidx] == 0 ) {
						if ( KSURI_5_CHECK[m_dispCol_left + clmidx] == 0 ) {
							Update_KSURI_5(index, clmidx);
						}
						Update_KSURI_6(index, clmidx);
						Update_KSURI_6_KEI();
						Update_KSURI_7(index, clmidx);
						Update_KSURI_7_KEI();
						Update_KSURI_10();
						Update_KSURI_11();
					}
					else if (index == idx_ksuri_6[clmidx]) {
						CheckSetData_KSURI_6(index, clmidx);
						Update_KSURI_6_KEI();
						Update_KSURI_10();
					}
					else if (index == idx_ksuri_7[clmidx]) {
						CheckSetData_KSURI_7(index, clmidx);
						Update_KSURI_7_KEI();
						Update_KSURI_11();
					}
					else if (index == idx_ksuri_8[clmidx]) {
						Update_KSURI_10();
					}
					else if (index == idx_ksuri_9[clmidx]) {
						Update_KSURI_11();
					}

				}
				else if (kbndatainfo[tblidx].kind == DATAKIND_RATE) {
					m_SyzUtil.val_to_bin((unsigned char*)kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size, 
						                  DiagData.data_edit);
				}
                else if (kbndatainfo[tblidx].kind == DATAKIND_DATE) {
                    if( DiagData.data_day[0] != '\0' && (DiagData.data_day[0] > 0)) {
					    memmove(kbndatainfo[tblidx].data + (m_dispCol_left + clmidx) * kbndatainfo[tblidx].size,
						    DiagData.data_day, kbndatainfo[tblidx].size);
					}
				
				}
			}
		}
	}

	for (int tblidx = 0; tblidx < sizeof(datainfo) / sizeof(datainfo[0]); tblidx++) {
		if (index == datainfo[tblidx].diagidx[0]) {

			if (datainfo[tblidx].kind == DATAKIND_VAL) {
				memmove(datainfo[tblidx].data, DiagData.data_val, MONY_BUF_SIZE);

				if ((index == idx_ksuri_8[0]) || (index == idx_ksuri_6_kei[0])){
					Update_KSURI_10();
				}

				if ((index == idx_ksuri_9[0]) || (index == idx_ksuri_7_kei[0])){
					Update_KSURI_11();
				}

			}

		}
	}

	Set_KSURI_data();
}

void CShinKSisanUri::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if (nChar == 0) {
		return;
	}

	// Shift+TabをTabに、TabをEnterに変換する
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
	if ( (wTerm == VK_UP) || (wTerm == VK_TAB) ) {
		pos = UpDownFocusMoveCheck(index, MOVE_UP);
		m_KsisanUriDiag02.SetPosition(pos);
	}

	if ( (wTerm == VK_DOWN) || (wTerm == VK_RETURN)) {	
		pos = UpDownFocusMoveCheck(index, MOVE_DOWN);
		m_KsisanUriDiag02.SetPosition(pos);
	}

	if ((wTerm == VK_F3) || (wTerm == VK_RIGHT)) {

		if (IsRightEnd(index, &pos) && (GetCol_FromIndex(index) < JIGYO_MAX_TOKUREI_1)) {

			EditOFFIcsdiag8ctrl2(index);
			m_dispCol_left++;

		    if ((m_TokureiMode == TOKUREIMODE_RENDO)
				&& (m_kazeihousiki == ID_ICSSH_KANNI) && IsNULL_Column(m_dispCol_left+JIGYO_PER_PAGE_URI-1)) {
				// 簡易課税 連動 事業区分右端列の次列にデータなしの場合は右移動不可.
                m_dispCol_left--;
			}
			else if (m_dispCol_left >= JIGYO_MAX_TOKUREI_1 - JIGYO_PER_PAGE_URI + 1) {
				if ((pos != ID_KSURI_6_KEI) && (pos != ID_KSURI_7_KEI)) {
					m_dispCol_left = 0;
					m_bar.SetScrollPos(m_dispCol_left);
				}
                else {
					m_dispCol_left--;
				}
				DispKsisan_Uri();
				m_KsisanUriDiag02.SetPosition(pos);
			}
            else {
				m_bar.SetScrollPos(m_dispCol_left);
				DispKsisan_Uri();
				m_KsisanUriDiag02.SetPosition(index);
			}
		}
		else {

            if ( (m_TokureiMode == TOKUREIMODE_HIRENDO) && (index == ID_KSURI_9) ) {
				m_KsisanUriDiag02.SetPosition(idx_ksuri_title[0]);
			}
            else {
				m_KsisanUriDiag02.SetNextPosition();
			}

		}

	}
	else if ((wTerm == VK_F2) || (wTerm == VK_LEFT)) {

		if (IsLeftEnd(index, &pos) && (GetCol_FromIndex(index) >= 0)) {

			EditOFFIcsdiag8ctrl2(index);

			if ((m_dispCol_left == 0) && (m_TokureiMode == TOKUREIMODE_RENDO) && (m_kazeihousiki == ID_ICSSH_GENSOKU) ) {
				// 原則課税 連動 事業区分名欄は1列目固定.
				;
			}
            else if((m_dispCol_left == 0) && (m_TokureiMode == TOKUREIMODE_RENDO) 
				&& (m_kazeihousiki == ID_ICSSH_KANNI) && IsNULL_Column(JIGYO_MAX_TOKUREI_1-1)) {
                // 簡易課税 連動 事業区分15列目にデータなしの場合は左移動不可.
				;
			}
			else {
				m_dispCol_left--;
			}

			if (m_dispCol_left < 0) {

                if( (m_TokureiMode == TOKUREIMODE_HIRENDO) && (index == ID_KSURI_TITLE1)) {
					m_dispCol_left = 0;
					m_KsisanUriDiag02.SetPosition(idx_ksuri_9[0]);
				}
                else {
					m_dispCol_left = JIGYO_MAX_TOKUREI_1 - JIGYO_PER_PAGE_URI;
					m_bar.SetScrollPos(m_dispCol_left);
					DispKsisan_Uri();
					m_KsisanUriDiag02.SetPosition(pos);
				}

			}
            else {
			    m_bar.SetScrollPos(m_dispCol_left);
			    DispKsisan_Uri();
				m_KsisanUriDiag02.SetPosition(index);
			}

		}
		else {
			m_KsisanUriDiag02.SetPrevPosition();
		}

	}

	if (wTerm == VK_DELETE) {

		for (int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++) {
			for (int clmidx = 0; clmidx < JIGYO_PER_PAGE_URI; clmidx++) {
                if( index == kbndatainfo[tblidx].diagidx[clmidx] ) {
					if (index == idx_ksuri_6[clmidx]) {
						if (KSURI_5_CHECK[m_dispCol_left + clmidx] == 0) {
							Update_KSURI_6(index, clmidx, true);
						}
                        else {
							CheckSub_KSURI_5CheckOn(index, clmidx, true);
						}
						Set_KSURI_data();
						KSURI_6_SIGN[m_dispCol_left + clmidx] = 0;
				    }
                    else if(index == idx_ksuri_7[clmidx]) {
						if (KSURI_5_CHECK[m_dispCol_left + clmidx] == 0) {
							Update_KSURI_7(index, clmidx, true);
						}
                        else {
							CheckSub_KSURI_5CheckOn(index, clmidx, true);
						}
						Set_KSURI_data();
						KSURI_7_SIGN[m_dispCol_left + clmidx] = 0;
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

	int nowpos = m_KsisanUriDiag02.GetPosition();	// 移動先または自身のポジション

	if( nowpos == ID_KSURI_9 ){
		nowpos = ID_KSURI_11;
	}

	RECT rectA, rectB;
	GetWindowRect(&rectA);
	diag_getrect(IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl);
	if ((rectA.top > rectB.top) || (rectA.bottom < rectB.bottom)) {
		int	cpos = GetScrollPos(SB_VERT);
		SetRedrawScroll((cpos + rectB.top - rectA.top) - ((rectA.bottom - rectA.top) / 2));
	}
}


BOOL CShinKSisanUri::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_END) {
			// 更新
			GetAllDiagData();
			WriteData(0, TOKUREI_TBL_VERSION);

			if (m_pParent) {
				m_pParent->PostMessageA(WM_KEYDOWN, VK_END);
			}

			return TRUE;
		}
		if (pMsg->wParam == VK_F2) {
			DIAGRAM_DATA dd;
			if (m_curwnd != -1) {
				short result = ((CIcsdiagctrl*)GetDlgItem(m_curwnd))->GetPosition();
				diag_getdata(m_curwnd, result, &dd, CIcsdiagctrl);
				if (m_curwnd == IDC_ICSDIAG8CTRL2) {
					TerminationIcsdiag8ctrl2(result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd);
				}
				return TRUE;
			}
		}
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

void CShinKSisanUri::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

    //横スクロールバー
	if (pScrollBar == &m_bar) {

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
				m_dispCol_left -= JIGYO_PER_PAGE_URI;
				break;
		    case SB_PAGERIGHT:
				m_dispCol_left += JIGYO_PER_PAGE_URI;
				break;
		    case SB_LEFT:
				m_dispCol_left = 0;
				break;
		    case SB_RIGHT:
				m_dispCol_left = JIGYO_MAX_TOKUREI_1 - 1;
				break;
		    case SB_THUMBTRACK: //Drag
				m_dispCol_left = nPos;
				break;
		    default:
			    DrawSw = FALSE; //描画されないときに、セット
			    break;
		}

        ChangeDiagAttr();

		if (m_dispCol_left > JIGYO_MAX_TOKUREI_1 - JIGYO_PER_PAGE_URI) {
			m_dispCol_left = JIGYO_MAX_TOKUREI_1 - JIGYO_PER_PAGE_URI;
		}

		if (m_dispCol_left < 0) {
			m_dispCol_left = 0;
		}

		if (oldLeft == m_dispCol_left) {
			DrawSw = FALSE;
		}

		if (DrawSw) {
			m_bar.SetScrollPos(m_dispCol_left);
			DispKsisan_Uri();
		}

		diag_setposition(IDC_ICSDIAG8CTRL2, m_Focus, CIcsdiagctrl);
		this->UnlockWindowUpdate();
	}

	CSyzBaseDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

int CShinKSisanUri::GetCol_FromIndex(int index)
{
    int ret = -1;

    if( IsLeftEnd(index) ) {
		ret = m_dispCol_left;
	}

	for (int i = 0; i < (sizeof(idx_ksuri_row2) / sizeof(idx_ksuri_row2[0])); i++) {
		if (idx_ksuri_row2[i] == index) {
			ret = m_dispCol_left + 1;
		}
	}

	if (IsRightEnd(index)) {
		ret = m_dispCol_left + 2;
	}

    return ret;
}

BOOL CShinKSisanUri::IsRightEnd(int index, int* pNextPos)
{
	BOOL ret = FALSE;
    for(int i = 0; i < (sizeof(idx_ksuri_row3) / sizeof(idx_ksuri_row3[0])); i++){
        if( idx_ksuri_row3[i] == index ) {
			if (pNextPos != NULL) {

                //if( (idx_ksuri_row3[i] == ID_KSURI_6_3) 
				//	&& (m_dispCol_left >= JIGYO_MAX_TOKUREI_1 - JIGYO_PER_PAGE_URI - 1)) {
				//	*pNextPos = ID_KSURI_6_KEI;
				//}
				//else if ((idx_ksuri_row3[i] == ID_KSURI_7_3)
				//	&& (m_dispCol_left >= JIGYO_MAX_TOKUREI_1 - JIGYO_PER_PAGE_URI - 1)) {
				//	*pNextPos = ID_KSURI_7_KEI; 
				//}
                //else {
					*pNextPos = idx_ksuriRend_next[i];
				//}

			}
            ret = TRUE;
            break;
		}
	}
	return ret;
}

BOOL CShinKSisanUri::IsLeftEnd(int index, int* pNextPos)
{
	BOOL ret = FALSE;
	for (int i = 0; i < (sizeof(idx_ksuri_row1) / sizeof(idx_ksuri_row1[0])); i++) {
		if (idx_ksuri_row1[i] == index) {
			if (pNextPos != NULL) {
				*pNextPos = idx_ksuriLend_next[i];
			}
			ret = TRUE;
			break;
		}
	}
	return ret;
}


void CShinKSisanUri::CheckButtonIcsdiag8ctrl2(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。

    for( int clmidx = 0; clmidx < JIGYO_PER_PAGE_URI; clmidx++){
        if( index == idx_ksuri_5_check[clmidx]) {
			Check_KSURI_5Check(index, clmidx);
		}
	}


}


void CShinKSisanUri::OnBnClickedKubunDel()
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
	Init_KubunColumn( JIGYO_MAX_TOKUREI_1-1 );	// 一番右端のデータを削除
	ReCalq();							// 再集計
	DispKsisan_Uri();

	// 移動先が入力不可項目のこともあるため、常に事業区分にセット
	//m_KsisanUriDiag02.SetPosition(m_Focus);
	m_KsisanUriDiag02.SetPosition ( idx_ksuri_title[Clmindex] );
}


void CShinKSisanUri::OnBnClickedKubunIns()
{
	// 列挿入

	int Clmindex = GetCol_FromIndex( m_Focus );
	if( Clmindex == -1 ){
		return;
	}

	if( IsNULL_Column( JIGYO_MAX_TOKUREI_1-1 ) == FALSE ){	// 最後尾のカラムに空きがあるかどうか
		m_KsisanUriDiag02.SetPosition(m_Focus);
		return;
	}

	MoveData( Clmindex, 1 );		// 列データを右に寄せる
	Init_KubunColumn( Clmindex );	// カラムデータを削除して空きを作る
	DispKsisan_Uri();

	m_KsisanUriDiag02.SetPosition(m_Focus);
}

BOOL CShinKSisanUri::IsNULL_Column( int Clmindex )
{
	// 該当のカラムに空きがあるか？
	for( int clmidx = 0; clmidx < JIGYO_MAX_TOKUREI_1; clmidx++ ){
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

void CShinKSisanUri::Init_KubunColumn( int ClmIndex )
{
	for( int clmidx = 0; clmidx < JIGYO_MAX_TOKUREI_1; clmidx++ ){	// 事業ごとのカラム
		if( clmidx == ClmIndex ){
			for( int tblidx = 0; tblidx < sizeof( kbndatainfo ) / sizeof( kbndatainfo[0] ); tblidx++ ){
				memset( kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * clmidx, '\0', kbndatainfo[tblidx].size );	// 削除
			}
		}
	}
}

void CShinKSisanUri::MoveData( int ClmIndex , int sign )
{
	if( sign == 0 ){	// 列削除
		
		for( int clmidx = ClmIndex; clmidx < JIGYO_MAX_TOKUREI_1; clmidx++ ){

			for( int tblidx = 0; tblidx < sizeof( kbndatainfo ) / sizeof( kbndatainfo[0] ); tblidx++ ){
				// 1つ前のカラムにデータを移す
				memmove( kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * clmidx,
						 kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * ( clmidx + 1 ),
						 kbndatainfo[tblidx].size );
			}

			memmove(KSURI_2_S_TMP[0] + clmidx * 4, KSURI_2_S_TMP[0] + (clmidx + 1) * 4, 4);
			memmove(KSURI_2_E_TMP[0] + clmidx * 4, KSURI_2_E_TMP[0] + (clmidx + 1) * 4, 4);
			memmove(KSURI_3_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI_3_TMP[0] + (clmidx + 1) * MONY_BUF_SIZE, MONY_BUF_SIZE);
			memmove(KSURI_4_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI_4_TMP[0] + (clmidx + 1) * MONY_BUF_SIZE, MONY_BUF_SIZE);
		}
	}
	else if( sign == 1 ){	// 列挿入

		for( int clmidx = JIGYO_MAX_TOKUREI_1-1; clmidx >= ClmIndex; clmidx-- ){
			for( int tblidx = 0; tblidx < sizeof( kbndatainfo ) / sizeof( kbndatainfo[0] ); tblidx++ ){
			
				// 次のカラムにデータを移す
				memmove( kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * clmidx,
						 kbndatainfo[tblidx].data + kbndatainfo[tblidx].size * ( clmidx - 1 ),
						 kbndatainfo[tblidx].size );

				memmove(KSURI_2_S_TMP[0] + clmidx * 4, KSURI_2_S_TMP[0] + (clmidx - 1) * 4, 4);
				memmove(KSURI_2_E_TMP[0] + clmidx * 4, KSURI_2_E_TMP[0] + (clmidx - 1) * 4, 4);
				memmove(KSURI_3_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI_3_TMP[0] + (clmidx - 1) * MONY_BUF_SIZE, MONY_BUF_SIZE);
				memmove(KSURI_4_TMP[0] + clmidx * MONY_BUF_SIZE, KSURI_4_TMP[0] + (clmidx - 1) * MONY_BUF_SIZE, MONY_BUF_SIZE);
			}
		}
	}
}

void CShinKSisanUri::ReCalq()
{
	// 再集計
	for( int clmidx = 0; clmidx < JIGYO_PER_PAGE_URI; clmidx++ ){
		for( int tblidx = 0; tblidx < sizeof(kbndatainfo) / sizeof(kbndatainfo[0]); tblidx++ ){

			if( kbndatainfo[tblidx].kind == DATAKIND_VAL ){
				int index = kbndatainfo[tblidx].diagidx[clmidx];

				if (index == idx_ksuri_3[clmidx] || index == idx_ksuri_4[clmidx]) {
					// 合計のみ再集計
					//Update_KSURI_5(index, clmidx);
					//Update_KSURI_6(index, clmidx);
					Update_KSURI_6_KEI();
					//Update_KSURI_7(index, clmidx);
					Update_KSURI_7_KEI();
					Update_KSURI_10();
					Update_KSURI_11();
					Set_KSURI_data();
				}
			}
		}
	}
}

void CShinKSisanUri::DispClmNum()
{
	// 列番号の表示
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	CString str;
	str.Format( "%d", m_dispCol_left + 1 );
	DiagData.data_disp = str;
	diag_setdata( IDC_ICSDIAG8CTRL2, ID_KSURI_NO1, (struct IUnknown*)&DiagData, CIcsdiagctrl );
	str.Format( "%d", m_dispCol_left + 2 );
	DiagData.data_disp = str;
	diag_setdata( IDC_ICSDIAG8CTRL2, ID_KSURI_NO2, (struct IUnknown*)&DiagData, CIcsdiagctrl );
	str.Format( "%d", m_dispCol_left + 3 );
	DiagData.data_disp = str;
	diag_setdata( IDC_ICSDIAG8CTRL2, ID_KSURI_NO3, (struct IUnknown*)&DiagData, CIcsdiagctrl );
}

// 事業区分のスクロールバーを画面最下部に移動
void CShinKSisanUri::MoveWindow_JigyoKubunBar(bool bFromView)
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
    if( bFromView ) {
        hight = 40;
	}
	if( hight < 20 ){	// マルチの時に下半分が切れてしまうため調整
		hight = 20;
	}
	crect_bar.bottom = crect_window.bottom;
	crect_bar.top = crect_bar.bottom - hight;
	m_bar.MoveWindow( &crect_bar );
}

