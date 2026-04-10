// ShinFhyo5_022.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo5_022.h"
#include "ShinFhyo5_022idx.h"
#include "H31HyoView.h"

// CShinFhyo5_022 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo5_022, CSyzBaseDlg)

CShinFhyo5_022::CShinFhyo5_022(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo5_022::IDD, pParent)
	, m_pSnHeadData ( NULL )
	, m_pParent ( pParent )
{

	memset(F5022_20A, '\0', sizeof(F5022_20A));
	memset(F5022_20B, '\0', sizeof(F5022_20B));
	memset(F5022_20C, '\0', sizeof(F5022_20C));
	memset(F5022_20X, '\0', sizeof(F5022_20X));

	memset(F5022_21A, '\0', sizeof(F5022_21A));
	memset(F5022_21B, '\0', sizeof(F5022_21B));
	memset(F5022_21C, '\0', sizeof(F5022_21C));
	memset(F5022_21X, '\0', sizeof(F5022_21X));

	memset(F5022_22A, '\0', sizeof(F5022_22A));
	memset(F5022_22B, '\0', sizeof(F5022_22B));
	memset(F5022_22C, '\0', sizeof(F5022_22C));
	memset(F5022_22X, '\0', sizeof(F5022_22X));

	memset(F5022_23A, '\0', sizeof(F5022_23A));
	memset(F5022_23B, '\0', sizeof(F5022_23B));
	memset(F5022_23C, '\0', sizeof(F5022_23C));
	memset(F5022_23X, '\0', sizeof(F5022_23X));

	memset(F5022_24A, '\0', sizeof(F5022_24A));
	memset(F5022_24B, '\0', sizeof(F5022_24B));
	memset(F5022_24C, '\0', sizeof(F5022_24C));
	memset(F5022_24X, '\0', sizeof(F5022_24X));

	memset(F5022_25A, '\0', sizeof(F5022_25A));
	memset(F5022_25B, '\0', sizeof(F5022_25B));
	memset(F5022_25C, '\0', sizeof(F5022_25C));
	memset(F5022_25X, '\0', sizeof(F5022_25X));

	memset(F5022_26A, '\0', sizeof(F5022_26A));
	memset(F5022_26B, '\0', sizeof(F5022_26B));
	memset(F5022_26C, '\0', sizeof(F5022_26C));
	memset(F5022_26X, '\0', sizeof(F5022_26X));

	memset(F5022_27A, '\0', sizeof(F5022_27A));
	memset(F5022_27B, '\0', sizeof(F5022_27B));
	memset(F5022_27C, '\0', sizeof(F5022_27C));
	memset(F5022_27X, '\0', sizeof(F5022_27X));

	memset(F5022_28A, '\0', sizeof(F5022_28A));
	memset(F5022_28B, '\0', sizeof(F5022_28B));
	memset(F5022_28C, '\0', sizeof(F5022_28C));
	memset(F5022_28X, '\0', sizeof(F5022_28X));

	memset(F5022_29A, '\0', sizeof(F5022_29A));
	memset(F5022_29B, '\0', sizeof(F5022_29B));
	memset(F5022_29C, '\0', sizeof(F5022_29C));
	memset(F5022_29X, '\0', sizeof(F5022_29X));

	memset(F5022_30A, '\0', sizeof(F5022_30A));
	memset(F5022_30B, '\0', sizeof(F5022_30B));
	memset(F5022_30C, '\0', sizeof(F5022_30C));
	memset(F5022_30X, '\0', sizeof(F5022_30X));

	memset(F5022_31A, '\0', sizeof(F5022_31A));
	memset(F5022_31B, '\0', sizeof(F5022_31B));
	memset(F5022_31C, '\0', sizeof(F5022_31C));
	memset(F5022_31X, '\0', sizeof(F5022_31X));

	memset(F5022_32A, '\0', sizeof(F5022_32A));
	memset(F5022_32B, '\0', sizeof(F5022_32B));
	memset(F5022_32C, '\0', sizeof(F5022_32C));
	memset(F5022_32X, '\0', sizeof(F5022_32X));

	memset(F5022_33A, '\0', sizeof(F5022_33A));
	memset(F5022_33B, '\0', sizeof(F5022_33B));
	memset(F5022_33C, '\0', sizeof(F5022_33C));
	memset(F5022_33X, '\0', sizeof(F5022_33X));

	memset(F5022_34A, '\0', sizeof(F5022_34A));
	memset(F5022_34B, '\0', sizeof(F5022_34B));
	memset(F5022_34C, '\0', sizeof(F5022_34C));
	memset(F5022_34X, '\0', sizeof(F5022_34X));

	memset(F5022_35A, '\0', sizeof(F5022_35A));
	memset(F5022_35B, '\0', sizeof(F5022_35B));
	memset(F5022_35C, '\0', sizeof(F5022_35C));
	memset(F5022_35X, '\0', sizeof(F5022_35X));

	memset(F5022_36A, '\0', sizeof(F5022_36A));
	memset(F5022_36B, '\0', sizeof(F5022_36B));
	memset(F5022_36C, '\0', sizeof(F5022_36C));
	memset(F5022_36X, '\0', sizeof(F5022_36X));

	memset(F5022_37A, '\0', sizeof(F5022_37A));
	memset(F5022_37B, '\0', sizeof(F5022_37B));
	memset(F5022_37C, '\0', sizeof(F5022_37C));
	memset(F5022_37X, '\0', sizeof(F5022_37X));
	F5022_21kbn = 0;

	// ★手入力サインの名前これでもいい？
	F5022_20f = F5022_21f = F5022_22f = F5022_23f = F5022_24f = F5022_25f = F5022_26f = F5022_27f = F5022_28f = F5022_29f = F5022_30f = F5022_31f = F5022_32f = F5022_32f = F5022_33f = F5022_34f = F5022_35f = F5022_36f = 0;

	F5022_21Disp = F5022_22Disp = F5022_23Disp = F5022_24Disp = F5022_25Disp = F5022_26Disp = F5022_27Disp = F5022_28Disp = F5022_29Disp = 0;
	F5022_30Disp = F5022_31Disp = F5022_32Disp = F5022_33Disp = F5022_34Disp = F5022_35Disp = F5022_36Disp = 0;

	m_id = 0;
	m_PrintId = 0;

	m_curwnd = -1;
}

CShinFhyo5_022::~CShinFhyo5_022()
{
}

void CShinFhyo5_022::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo5_022Diag01);
}


BEGIN_MESSAGE_MAP(CShinFhyo5_022, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CShinFhyo5_022 メッセージ ハンドラ

BOOL CShinFhyo5_022::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL1);
	m_BottomIdx = 309;//コントロール一番最下のDiagのID指定。これで縦スクロール出る

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo5_022::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo5_022::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo5_022::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

//=============================
// 初期化
//-----------------------------
// 返送値
//		0	:	正常
//		-1	:	エラー
//=============================
int CShinFhyo5_022::Init()
{
	// データ読み込み
	ReadData();

	// 集計モジュールにデータセット
	ReadData2();
	MemoryReget();

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		m_Fhyo5_022Diag01.ModifyItem ( 310, DIAG_MDFY_TRANSPARENT );
	}


	// 画面表示
	Disp_f5022();

	// ダイアグラム初期属性セット
	InitDiagAttr();

	// 初期ポジションセット
	SetItemPosition();

	return 0;
}

//=========================================================
// 初期情報のセット
//---------------------------------------------------------
// 引数
//		pSnHeadData		:	本表ヘッダデータ読込クラス
//---------------------------------------------------------
// 返送値
//			0			:	正常
//			-1			:	エラー
//=========================================================
int CShinFhyo5_022::InitInfo(CH31SnFhyo5_022Data **pSnFhyo5_022Data, CSnHeadData **pSnHeadData, CArithEx *pArith, CSyzSyukei **pSyzSyukei)
{
	ASSERT ( pSnHeadData );
	if ( pSnHeadData == NULL ) {
		return -1;
	}

	ASSERT ( pSnFhyo5_022Data );
	if (pSnFhyo5_022Data == NULL) {
		return -1;
	}

	ASSERT ( pArith );
	if (pArith == NULL) {
		return -1;
	}

	ASSERT ( pSyzSyukei );
	if ( pSyzSyukei == NULL ) {
		return -1;
	}

	m_pSnHeadData = pSnHeadData;
	m_pSnFhyo5_022Data = pSnFhyo5_022Data;
	m_pArith = pArith;
	m_pSyzSyukei = pSyzSyukei;

	return 0;
}

//=============================
// ダイアグラム初期属性セット
//-----------------------------
// 返送値
//		0	:	正常
//		-1	:	エラー
//=============================
int CShinFhyo5_022::InitDiagAttr()
{
	int cnt = 0;
	char mony[MONY_BUF_SIZE] = { 0 };

	IsEnableItems(0, m_id, m_code, m_typecnt);
	m_flg = Gokeiclctype();
	m_4val = Is4Val(0);
	if (!m_4val) {
		m_id = 0;
		m_code.Empty();
	}

	GetTokurei();

	while ( 1 ) {
		if ( Fhyo5_022[cnt].Index == 0 )
			break;

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo5_022[cnt].AllUnConnect;
		}
		else if ( (m_typecnt < 2) || (m_4val == 0) ) {
			// 単一業種の時には入力不可
			Flg = Fhyo5_022[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				/*if ( IsProvideEditCell ( Fhyo5_022[cnt].Index ) ) {
					Flg = 0;
				}*/
				if ((Fhyo5_022[cnt].Index == ID5022_20A) || (Fhyo5_022[cnt].Index == ID5022_20B) || (Fhyo5_022[cnt].Index == ID5022_20C)) {
					Flg = Fhyo5_022[cnt].UnConnect;
				}
				else if ((Fhyo5_022[cnt].Index == ID5022_20X) && (m_flg & 0x01)) {
					Flg = Fhyo5_022[cnt].UnConnect;
				}
				// 付表5-2(1)の売上割合をチェック
				/*else if ( (m_typecnt > 1) && ((Fhyo5_022[cnt].Index == ID5022_21A) || (Fhyo5_022[cnt].Index == ID5022_21B) || (Fhyo5_022[cnt].Index == ID5022_21C)) && (m_id == ID5022_21X) ) {
					Flg = Fhyo5_022[cnt].UnConnect;
				}
				else if ( (m_typecnt > 1) && (Fhyo5_022[cnt].Index == ID5022_21X) && (m_id == ID5022_21X) && (m_flg & 0x01)) {
					Flg = Fhyo5_022[cnt].UnConnect;
				}
				else if ( (m_typecnt > 2) && ((Fhyo5_022[cnt].Index == (m_id - 3)) || (Fhyo5_022[cnt].Index == (m_id - 2)) || (Fhyo5_022[cnt].Index == (m_id - 1))) ) {
					Flg = Fhyo5_022[cnt].UnConnect;
				}
				else if ( (m_typecnt > 2) && (Fhyo5_022[cnt].Index == m_id) && (m_flg & 0x01)) {
					Flg = Fhyo5_022[cnt].UnConnect;
				}*/
				else if ( IsDispArea ( Fhyo5_022[cnt].Index ) == 1 ) {
					Flg = Fhyo5_022[cnt].UnConnect;
					CalqGokei( Fhyo5_022[cnt].Index );
				}
				else {
					Flg = 0;
					ClearInput ( Fhyo5_022[cnt].Index );
				}
			}
			else {
				Flg = Fhyo5_022[cnt].Connect;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL1, Fhyo5_022[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo5_022[cnt].Connect );
		cnt++;

	}

	m_Fhyo5_022Diag01.EnableDelete ( 0 );

	//if ( IsKjCodeReset() == true ) {
	//	AutoKjCode();
	//}

	// 付表5-1(1)の売上割合チェック時に手入力のクリアを行っているため、再集計が必要
	MemoryReget();

	if ( ( *m_pSnHeadData )->Sn_GenericSgn & 0x40 ) {
		SetPrintArea();
	}
	else {
		AllPrint();
	}

	return 0;
}

//======================================
// 初期ポジションセット
//======================================
void CShinFhyo5_022::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		//index = m_Fhyo5_022Diag01.GetPosition();
		if ( ( index = m_Fhyo5_022Diag01.GetPosition() ) != -1 ) {
			m_Fhyo5_022Diag01.SetFocus();	// ダミーでセット
			m_Fhyo5_022Diag01.SetPosition ( index );
			m_Fhyo5_022Diag01.SetFocus();
			return;
		}
	}

	GetDlgItem(IDC_STATIC_INPUTLOCK)->SetFocus();	// ダミーでセット
	m_Fhyo5_022Diag01.SetFocus();	// ダミーでセット

	//　4×みなし仕入率へ
	m_Fhyo5_022Diag01.SetPosition ( ID5022_20A );
}

//=================================================
// 入力項目色変え
//-------------------------------------------------
// 引数
//		id		:	ダイアグラムID
//		index	:	ダイアグラム内インデックス
//		sign	:	変更サイン
//=================================================
void CShinFhyo5_022::ChangeColor(unsigned short id, short index, int sign)
{
	DIAGRAM_ATTRIBUTE DA;

	diag_getattr ( id, index, &DA, CIcsdiagctrl);
	switch ( sign ) {
		case 1:		// 入力不可 (白色)
			DA.attr_bcolor = BC_WHITE;
			break;
		case 2:		// ロック (灰色)
			DA.attr_bcolor = BC_GRAY;
			break;
		case 3:		// 特殊色① (青色)
			DA.attr_bcolor = BC_BLUE;
			break;
		case 4:		// 特殊色② (緑色)
			DA.attr_bcolor = BC_GREEN;
			break;
		case 5:		// 特殊色 (薄緑色)
			DA.attr_bcolor = BC_GREEN_L;
			break;
		default:
			DA.attr_bcolor = BC_CREAM;
	}

	diag_setattr ( id, index, &DA, FALSE, CIcsdiagctrl);
}

//==============================================
// フォーカス検索
//----------------------------------------------
// 引数
//		Index	:	ダイアグラム内インデックス
//		Move	:	0 = 下, 1 = 上
//----------------------------------------------
// 返送値
//		次のフォーカスインデックス
//==============================================
int CShinFhyo5_022::UpDownFocusMoveCheck(int Index, int Move)
{
	int idx = 0;
	int i = 0, j = 0;

	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY5_022CNT; i++ ) {
		if ( Index == FCS_Fhyo5_022[i].IDNo )
			break;
	}

	// 現在ポジションセット
	idx = FCS_Fhyo5_022[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) {			//　下方向
		for ( j = i + 1; j < FCS_FHY5_022CNT; j++ ) {
			m_Fhyo5_022Diag01.GetAttribute ( FCS_Fhyo5_022[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo5_022[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {		//　上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo5_022Diag01.GetAttribute ( FCS_Fhyo5_022[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo5_022[j].IDNo;
				break;
			}
		}
	}

	return idx;
}

//==============================================
// 複数事業で強制入力不可行
//----------------------------------------------
// 引数
//		Idx		:	チェックインデックス
//----------------------------------------------
// 返送値
//
//==============================================
int CShinFhyo5_022::IsProvideEditCell(short Idx)
{
	int rt = 0;

	return rt;
}


BEGIN_EVENTSINK_MAP(CShinFhyo5_022, CSyzBaseDlg)
	ON_EVENT(CShinFhyo5_022, IDC_ICSDIAG8CTRL1, 1, CShinFhyo5_022::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinFhyo5_022, IDC_ICSDIAG8CTRL1, 2, CShinFhyo5_022::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinFhyo5_022, IDC_ICSDIAG8CTRL1, 3, CShinFhyo5_022::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo5_022::EditONIcsdiag8ctrl1(short index)
{
	m_curwnd = IDC_ICSDIAG8CTRL1;
}

void CShinFhyo5_022::EditOFFIcsdiag8ctrl1(short index)
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	char *ptmpMony = NULL;
	char CkMy[MONY_BUF_SIZE] = { 0 };

	switch ( index ) {
			// 原則計算を適用する場合
		case ID5022_20A:
		case ID5022_20B:
		case ID5022_20C:
		case ID5022_20X:
			SyukeiDataSet ( index );
			Disp_f5022data ( 1 );
			break;

			// 特例計算を適用する場合
		case ID5022_21A:
		case ID5022_21B:
		case ID5022_21C:
		case ID5022_21X:
			SyukeiDataSet ( index );
			Disp_f5022data ( 2 );
			break;

		case ID5022_22A:
		case ID5022_22B:
		case ID5022_22C:
		case ID5022_22X:
			SyukeiDataSet(index);
			Disp_f5022data(3);
			break;

		case ID5022_23A:
		case ID5022_23B:
		case ID5022_23C:
		case ID5022_23X:
			SyukeiDataSet(index);
			Disp_f5022data(4);
			break;

		case ID5022_24A:
		case ID5022_24B:
		case ID5022_24C:
		case ID5022_24X:
			SyukeiDataSet(index);
			Disp_f5022data(5);
			break;

		case ID5022_25A:
		case ID5022_25B:
		case ID5022_25C:
		case ID5022_25X:
			SyukeiDataSet(index);
			Disp_f5022data(6);
			break;

		case ID5022_26A:
		case ID5022_26B:
		case ID5022_26C:
		case ID5022_26X:
			SyukeiDataSet(index);
			Disp_f5022data(7);
			break;

		case ID5022_27A:
		case ID5022_27B:
		case ID5022_27C:
		case ID5022_27X:
			SyukeiDataSet(index);
			Disp_f5022data(8);
			break;

		case ID5022_28A:
		case ID5022_28B:
		case ID5022_28C:
		case ID5022_28X:
			SyukeiDataSet(index);
			Disp_f5022data(9);
			break;

		case ID5022_29A:
		case ID5022_29B:
		case ID5022_29C:
		case ID5022_29X:
			SyukeiDataSet(index);
			Disp_f5022data(10);
			break;

		case ID5022_30A:
		case ID5022_30B:
		case ID5022_30C:
		case ID5022_30X:
			SyukeiDataSet(index);
			Disp_f5022data(11);
			break;

		case ID5022_31A:
		case ID5022_31B:
		case ID5022_31C:
		case ID5022_31X:
			SyukeiDataSet(index);
			Disp_f5022data(12);
			break;

		case ID5022_32A:
		case ID5022_32B:
		case ID5022_32C:
		case ID5022_32X:
			SyukeiDataSet(index);
			Disp_f5022data(13);
			break;

		case ID5022_33A:
		case ID5022_33B:
		case ID5022_33C:
		case ID5022_33X:
			SyukeiDataSet(index);
			Disp_f5022data(14);
			break;

		case ID5022_34A:
		case ID5022_34B:
		case ID5022_34C:
		case ID5022_34X:
			SyukeiDataSet(index);
			Disp_f5022data(15);
			break;

		case ID5022_35A:
		case ID5022_35B:
		case ID5022_35C:
		case ID5022_35X:
			SyukeiDataSet(index);
			Disp_f5022data(16);
			break;

		case ID5022_36A:
		case ID5022_36B:
		case ID5022_36C:
		case ID5022_36X:
			SyukeiDataSet(index);
			Disp_f5022data(17);
			break;
	
		default :
			break;
	}
}

void CShinFhyo5_022::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	if (nChar == 0) {
		return;
	}

	short wTerm = 0;
	char VK_FLG = 0x00;
	// Shift+TabをTabに、TabをEnterに変換
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
		pos = UpDownFocusMoveCheck(index, 1);
		m_Fhyo5_022Diag01.SetPosition(pos);
	}
	if (wTerm == VK_DOWN) {
		pos = UpDownFocusMoveCheck(index, 0);
		m_Fhyo5_022Diag01.SetPosition(pos);
	}

	if ((wTerm == VK_RETURN) || (wTerm == VK_RIGHT) || (wTerm == VK_F3)) {
		m_Fhyo5_022Diag01.SetNextPosition();
	}
	if ((wTerm == VK_TAB) || (wTerm == VK_LEFT) || (wTerm == VK_F2)) {
		m_Fhyo5_022Diag01.SetPrevPosition();
	}

	if (wTerm == VK_DELETE) {
		switch (index) {
		case ID5022_20A:
		case ID5022_20B:
		case ID5022_20C:
		case ID5022_20X:
			if( index == ID5022_20A ){
				F5022_20f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352001"));
			}
			else if( index == ID5022_20B ){
				F5022_20f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352002"));
			}
			else if( index == ID5022_20C ){
				F5022_20f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352003"));
			}
			else if( index == ID5022_20X ){
				F5022_20f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352000"));
			}
			Disp_f5022data(1);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_21A:
		case ID5022_21B:
		case ID5022_21C:
		case ID5022_21X:
			if( index == ID5022_21A ){
				F5022_21f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352101"));
			}
			else if( index == ID5022_21B ){
				F5022_21f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352102"));
			}
			else if( index == ID5022_21C ){
				F5022_21f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352103"));
			}
			else if( index == ID5022_21X ){
				F5022_21f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352100"));
			}
			Disp_f5022data(2);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_22A:
		case ID5022_22B:
		case ID5022_22C:
		case ID5022_22X:
			if( index == ID5022_22A ){
				F5022_22f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352201"));
			}
			else if( index == ID5022_22B ){
				F5022_22f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352202"));
			}
			else if( index == ID5022_22C ){
				F5022_22f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352203"));
			}
			else if( index == ID5022_22X ){
				F5022_22f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352200"));
			}
			Disp_f5022data(3);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_23A:
		case ID5022_23B:
		case ID5022_23C:
		case ID5022_23X:
			if( index == ID5022_23A ){
				F5022_23f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352301"));
			}
			else if( index == ID5022_23B ){
				F5022_23f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352302"));
			}
			else if( index == ID5022_23C ){
				F5022_23f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352303"));
			}
			else if( index == ID5022_23X ){
				F5022_23f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352300"));
			}
			Disp_f5022data(4);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_24A:
		case ID5022_24B:
		case ID5022_24C:
		case ID5022_24X:
			if( index == ID5022_24A ){
				F5022_24f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352401"));
			}
			else if( index == ID5022_24B ){
				F5022_24f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352402"));
			}
			else if( index == ID5022_24C ){
				F5022_24f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352403"));
			}
			else if( index == ID5022_24X ){
				F5022_24f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352400"));
			}
			Disp_f5022data(5);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_25A:
		case ID5022_25B:
		case ID5022_25C:
		case ID5022_25X:
			if( index == ID5022_25A ){
				F5022_25f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352501"));
			}
			else if( index == ID5022_25B ){
				F5022_25f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352502"));
			}
			else if( index == ID5022_25C ){
				F5022_25f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352503"));
			}
			else if( index == ID5022_25X ){
				F5022_25f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352500"));
			}
			Disp_f5022data(6);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_26A:
		case ID5022_26B:
		case ID5022_26C:
		case ID5022_26X:
			if( index == ID5022_26A ){
				F5022_26f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352601"));
			}
			else if( index == ID5022_26B ){
				F5022_26f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352602"));
			}
			else if( index == ID5022_26C ){
				F5022_26f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352603"));
			}
			else if( index == ID5022_26X ){
				F5022_26f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352600"));
			}
			Disp_f5022data(7);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_27A:
		case ID5022_27B:
		case ID5022_27C:
		case ID5022_27X:
			if( index == ID5022_27A ){
				F5022_27f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352701"));
			}
			else if( index == ID5022_27B ){
				F5022_27f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352702"));
			}
			else if( index == ID5022_27C ){
				F5022_27f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352703"));
			}
			else if( index == ID5022_27X ){
				F5022_27f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352700"));
			}
			Disp_f5022data(8);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_28A:
		case ID5022_28B:
		case ID5022_28C:
		case ID5022_28X:
			if( index == ID5022_28A ){
				F5022_28f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352801"));
			}
			else if( index == ID5022_28B ){
				F5022_28f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352802"));
			}
			else if( index == ID5022_28C ){
				F5022_28f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352803"));
			}
			else if( index == ID5022_28X ){
				F5022_28f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352800"));
			}
			Disp_f5022data(9);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_29A:
		case ID5022_29B:
		case ID5022_29C:
		case ID5022_29X:
			if( index == ID5022_29A ){
				F5022_29f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352901"));
			}
			else if( index == ID5022_29B ){
				F5022_29f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352902"));
			}
			else if( index == ID5022_29C ){
				F5022_29f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352903"));
			}
			else if( index == ID5022_29X ){
				F5022_29f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("352900"));
			}
			Disp_f5022data(10);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_30A:
		case ID5022_30B:
		case ID5022_30C:
		case ID5022_30X:
			if( index == ID5022_30A ){
				F5022_30f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353001"));
			}
			else if( index == ID5022_30B ){
				F5022_30f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353002"));
			}
			else if( index == ID5022_30C ){
				F5022_30f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353003"));
			}
			else if( index == ID5022_30X ){
				F5022_30f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353000"));
			}
			Disp_f5022data(11);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_31A:
		case ID5022_31B:
		case ID5022_31C:
		case ID5022_31X:
			if( index == ID5022_31A ){
				F5022_31f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353101"));
			}
			else if( index == ID5022_31B ){
				F5022_31f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353102"));
			}
			else if( index == ID5022_31C ){
				F5022_31f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353103"));
			}
			else if( index == ID5022_31X ){
				F5022_31f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353100"));
			}
			Disp_f5022data(12);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_32A:
		case ID5022_32B:
		case ID5022_32C:
		case ID5022_32X:
			if( index == ID5022_32A ){
				F5022_32f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353201"));
			}
			else if( index == ID5022_32B ){
				F5022_32f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353202"));
			}
			else if( index == ID5022_32C ){
				F5022_32f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353203"));
			}
			else if( index == ID5022_32X ){
				F5022_32f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353200"));
			}
			Disp_f5022data(13);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_33A:
		case ID5022_33B:
		case ID5022_33C:
		case ID5022_33X:
			if( index == ID5022_33A ){
				F5022_33f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353301"));
			}
			else if( index == ID5022_33B ){
				F5022_33f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353302"));
			}
			else if( index == ID5022_33C ){
				F5022_33f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353303"));
			}
			else if( index == ID5022_33X ){
				F5022_33f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353300"));
			}
			Disp_f5022data(14);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_34A:
		case ID5022_34B:
		case ID5022_34C:
		case ID5022_34X:
			if( index == ID5022_34A ){
				F5022_34f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353401"));
			}
			else if( index == ID5022_34B ){
				F5022_34f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353402"));
			}
			else if( index == ID5022_34C ){
				F5022_34f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353403"));
			}
			else if( index == ID5022_34X ){
				F5022_34f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353400"));
			}
			Disp_f5022data(15);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_35A:
		case ID5022_35B:
		case ID5022_35C:
		case ID5022_35X:
			if( index == ID5022_35A ){
				F5022_35f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353501"));
			}
			else if( index == ID5022_35B ){
				F5022_35f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353502"));
			}
			else if( index == ID5022_35C ){
				F5022_35f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353503"));
			}
			else if( index == ID5022_35X ){
				F5022_35f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353500"));
			}
			Disp_f5022data(16);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		case ID5022_36A:
		case ID5022_36B:
		case ID5022_36C:
		case ID5022_36X:
			if( index == ID5022_36A ){
				F5022_36f &=~ 0x08;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353601"));
			}
			else if( index == ID5022_36B ){
				F5022_36f &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353602"));
			}
			else if( index == ID5022_36C ){
				F5022_36f &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353603"));
			}
			else if( index == ID5022_36X ){
				F5022_36f &=~ 0x8000;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("353600"));
			}
			Disp_f5022data(17);
			m_Fhyo5_022Diag01.SetPosition( index );
			break;
		default:
			break;
		}

		ClearInput( index );
		m_Fhyo5_022Diag01.SetPosition( index );
	}

	int nowpos = m_Fhyo5_022Diag01.GetPosition();

	RECT rectA, rectB;
	GetWindowRect(&rectA);
	diag_getrect(IDC_ICSDIAG8CTRL1, nowpos, &rectB, CIcsdiagctrl);
	if ((rectA.top > rectB.top) || (rectA.bottom < rectB.bottom)) {
		int cpos = GetScrollPos(SB_VERT);
		SetRedrawScroll((cpos + rectB.top - rectA.top) - ((rectA.bottom - rectA.top) / 2));
	}
}

//============================
// データ読み込み
//============================
void CShinFhyo5_022::ReadData()
{
	// 4×みなし仕入率
	memmove(F5022_20A, (*m_pSnFhyo5_022Data)->Sn_F5022_20A, MONY_BUF_SIZE);
	memmove(F5022_20B, (*m_pSnFhyo5_022Data)->Sn_F5022_20B, MONY_BUF_SIZE);
	memmove(F5022_20C, (*m_pSnFhyo5_022Data)->Sn_F5022_20C, MONY_BUF_SIZE);
	memmove(F5022_20X, (*m_pSnFhyo5_022Data)->Sn_F5022_20X, MONY_BUF_SIZE);
	F5022_20f = (*m_pSnFhyo5_022Data)->Sn_F5022_20sw;

	// 4×みなし仕入率
	memmove(F5022_21A, (*m_pSnFhyo5_022Data)->Sn_F5022_21A, MONY_BUF_SIZE);
	memmove(F5022_21B, (*m_pSnFhyo5_022Data)->Sn_F5022_21B, MONY_BUF_SIZE);
	memmove(F5022_21C, (*m_pSnFhyo5_022Data)->Sn_F5022_21C, MONY_BUF_SIZE);
	memmove(F5022_21X, (*m_pSnFhyo5_022Data)->Sn_F5022_21X, MONY_BUF_SIZE);
	F5022_21f = (*m_pSnFhyo5_022Data)->Sn_F5022_21sw;

	// 第一種事業及び第二種事業
	memmove(F5022_22A, (*m_pSnFhyo5_022Data)->Sn_F5022_22A, MONY_BUF_SIZE);
	memmove(F5022_22B, (*m_pSnFhyo5_022Data)->Sn_F5022_22B, MONY_BUF_SIZE);
	memmove(F5022_22C, (*m_pSnFhyo5_022Data)->Sn_F5022_22C, MONY_BUF_SIZE);
	memmove(F5022_22X, (*m_pSnFhyo5_022Data)->Sn_F5022_22X, MONY_BUF_SIZE);
	F5022_22f = (*m_pSnFhyo5_022Data)->Sn_F5022_22sw;

	// 第一種事業及び第三種事業
	memmove(F5022_23A, (*m_pSnFhyo5_022Data)->Sn_F5022_23A, MONY_BUF_SIZE);
	memmove(F5022_23B, (*m_pSnFhyo5_022Data)->Sn_F5022_23B, MONY_BUF_SIZE);
	memmove(F5022_23C, (*m_pSnFhyo5_022Data)->Sn_F5022_23C, MONY_BUF_SIZE);
	memmove(F5022_23X, (*m_pSnFhyo5_022Data)->Sn_F5022_23X, MONY_BUF_SIZE);
	F5022_23f = (*m_pSnFhyo5_022Data)->Sn_F5022_23sw;

	// 第一種事業及び第四種事業
	memmove(F5022_24A, (*m_pSnFhyo5_022Data)->Sn_F5022_24A, MONY_BUF_SIZE);
	memmove(F5022_24B, (*m_pSnFhyo5_022Data)->Sn_F5022_24B, MONY_BUF_SIZE);
	memmove(F5022_24C, (*m_pSnFhyo5_022Data)->Sn_F5022_24C, MONY_BUF_SIZE);
	memmove(F5022_24X, (*m_pSnFhyo5_022Data)->Sn_F5022_24X, MONY_BUF_SIZE);
	F5022_24f = (*m_pSnFhyo5_022Data)->Sn_F5022_24sw;

	// 第一種事業及び第五種事業
	memmove(F5022_25A, (*m_pSnFhyo5_022Data)->Sn_F5022_25A, MONY_BUF_SIZE);
	memmove(F5022_25B, (*m_pSnFhyo5_022Data)->Sn_F5022_25B, MONY_BUF_SIZE);
	memmove(F5022_25C, (*m_pSnFhyo5_022Data)->Sn_F5022_25C, MONY_BUF_SIZE);
	memmove(F5022_25X, (*m_pSnFhyo5_022Data)->Sn_F5022_25X, MONY_BUF_SIZE);
	F5022_25f = (*m_pSnFhyo5_022Data)->Sn_F5022_25sw;

	// 第一種事業及び第六種事業
	memmove(F5022_26A, (*m_pSnFhyo5_022Data)->Sn_F5022_26A, MONY_BUF_SIZE);
	memmove(F5022_26B, (*m_pSnFhyo5_022Data)->Sn_F5022_26B, MONY_BUF_SIZE);
	memmove(F5022_26C, (*m_pSnFhyo5_022Data)->Sn_F5022_26C, MONY_BUF_SIZE);
	memmove(F5022_26X, (*m_pSnFhyo5_022Data)->Sn_F5022_26X, MONY_BUF_SIZE);
	F5022_26f = (*m_pSnFhyo5_022Data)->Sn_F5022_26sw;

	// 第二種事業及び第三種事業
	memmove(F5022_27A, (*m_pSnFhyo5_022Data)->Sn_F5022_27A, MONY_BUF_SIZE);
	memmove(F5022_27B, (*m_pSnFhyo5_022Data)->Sn_F5022_27B, MONY_BUF_SIZE);
	memmove(F5022_27C, (*m_pSnFhyo5_022Data)->Sn_F5022_27C, MONY_BUF_SIZE);
	memmove(F5022_27X, (*m_pSnFhyo5_022Data)->Sn_F5022_27X, MONY_BUF_SIZE);
	F5022_27f = (*m_pSnFhyo5_022Data)->Sn_F5022_27sw;

	// 第二種事業及び第四種事業
	memmove(F5022_28A, (*m_pSnFhyo5_022Data)->Sn_F5022_28A, MONY_BUF_SIZE);
	memmove(F5022_28B, (*m_pSnFhyo5_022Data)->Sn_F5022_28B, MONY_BUF_SIZE);
	memmove(F5022_28C, (*m_pSnFhyo5_022Data)->Sn_F5022_28C, MONY_BUF_SIZE);
	memmove(F5022_28X, (*m_pSnFhyo5_022Data)->Sn_F5022_28X, MONY_BUF_SIZE);
	F5022_28f = (*m_pSnFhyo5_022Data)->Sn_F5022_28sw;

	// 第二種事業及び第五種事業
	memmove(F5022_29A, (*m_pSnFhyo5_022Data)->Sn_F5022_29A, MONY_BUF_SIZE);
	memmove(F5022_29B, (*m_pSnFhyo5_022Data)->Sn_F5022_29B, MONY_BUF_SIZE);
	memmove(F5022_29C, (*m_pSnFhyo5_022Data)->Sn_F5022_29C, MONY_BUF_SIZE);
	memmove(F5022_29X, (*m_pSnFhyo5_022Data)->Sn_F5022_29X, MONY_BUF_SIZE);
	F5022_29f = (*m_pSnFhyo5_022Data)->Sn_F5022_29sw;

	// 第二種事業及び第六種事業
	memmove(F5022_30A, (*m_pSnFhyo5_022Data)->Sn_F5022_30A, MONY_BUF_SIZE);
	memmove(F5022_30B, (*m_pSnFhyo5_022Data)->Sn_F5022_30B, MONY_BUF_SIZE);
	memmove(F5022_30C, (*m_pSnFhyo5_022Data)->Sn_F5022_30C, MONY_BUF_SIZE);
	memmove(F5022_30X, (*m_pSnFhyo5_022Data)->Sn_F5022_30X, MONY_BUF_SIZE);
	F5022_30f = (*m_pSnFhyo5_022Data)->Sn_F5022_30sw;

	// 第三種事業及び第四種事業
	memmove(F5022_31A, (*m_pSnFhyo5_022Data)->Sn_F5022_31A, MONY_BUF_SIZE);
	memmove(F5022_31B, (*m_pSnFhyo5_022Data)->Sn_F5022_31B, MONY_BUF_SIZE);
	memmove(F5022_31C, (*m_pSnFhyo5_022Data)->Sn_F5022_31C, MONY_BUF_SIZE);
	memmove(F5022_31X, (*m_pSnFhyo5_022Data)->Sn_F5022_31X, MONY_BUF_SIZE);
	F5022_31f = (*m_pSnFhyo5_022Data)->Sn_F5022_31sw;

	// 第三種事業及び第五種事業
	memmove(F5022_32A, (*m_pSnFhyo5_022Data)->Sn_F5022_32A, MONY_BUF_SIZE);
	memmove(F5022_32B, (*m_pSnFhyo5_022Data)->Sn_F5022_32B, MONY_BUF_SIZE);
	memmove(F5022_32C, (*m_pSnFhyo5_022Data)->Sn_F5022_32C, MONY_BUF_SIZE);
	memmove(F5022_32X, (*m_pSnFhyo5_022Data)->Sn_F5022_32X, MONY_BUF_SIZE);
	F5022_32f = (*m_pSnFhyo5_022Data)->Sn_F5022_32sw;

	// 第三種事業及び第六種事業
	memmove(F5022_33A, (*m_pSnFhyo5_022Data)->Sn_F5022_33A, MONY_BUF_SIZE);
	memmove(F5022_33B, (*m_pSnFhyo5_022Data)->Sn_F5022_33B, MONY_BUF_SIZE);
	memmove(F5022_33C, (*m_pSnFhyo5_022Data)->Sn_F5022_33C, MONY_BUF_SIZE);
	memmove(F5022_33X, (*m_pSnFhyo5_022Data)->Sn_F5022_33X, MONY_BUF_SIZE);
	F5022_33f = (*m_pSnFhyo5_022Data)->Sn_F5022_33sw;

	// 第四種事業及び第五種事業
	memmove(F5022_34A, (*m_pSnFhyo5_022Data)->Sn_F5022_34A, MONY_BUF_SIZE);
	memmove(F5022_34B, (*m_pSnFhyo5_022Data)->Sn_F5022_34B, MONY_BUF_SIZE);
	memmove(F5022_34C, (*m_pSnFhyo5_022Data)->Sn_F5022_34C, MONY_BUF_SIZE);
	memmove(F5022_34X, (*m_pSnFhyo5_022Data)->Sn_F5022_34X, MONY_BUF_SIZE);
	F5022_34f = (*m_pSnFhyo5_022Data)->Sn_F5022_34sw;

	// 第四種事業及び第六種事業
	memmove(F5022_35A, (*m_pSnFhyo5_022Data)->Sn_F5022_35A, MONY_BUF_SIZE);
	memmove(F5022_35B, (*m_pSnFhyo5_022Data)->Sn_F5022_35B, MONY_BUF_SIZE);
	memmove(F5022_35C, (*m_pSnFhyo5_022Data)->Sn_F5022_35C, MONY_BUF_SIZE);
	memmove(F5022_35X, (*m_pSnFhyo5_022Data)->Sn_F5022_35X, MONY_BUF_SIZE);
	F5022_35f = (*m_pSnFhyo5_022Data)->Sn_F5022_35sw;

	// 第五種事業及び第六種事業
	memmove(F5022_36A, (*m_pSnFhyo5_022Data)->Sn_F5022_36A, MONY_BUF_SIZE);
	memmove(F5022_36B, (*m_pSnFhyo5_022Data)->Sn_F5022_36B, MONY_BUF_SIZE);
	memmove(F5022_36C, (*m_pSnFhyo5_022Data)->Sn_F5022_36C, MONY_BUF_SIZE);
	memmove(F5022_36X, (*m_pSnFhyo5_022Data)->Sn_F5022_36X, MONY_BUF_SIZE);
	F5022_36f = (*m_pSnFhyo5_022Data)->Sn_F5022_36sw;

	// 選択可能な計算式区分の内から選択した金額
	memmove(F5022_37A, (*m_pSnFhyo5_022Data)->Sn_F5022_37A, MONY_BUF_SIZE);
	memmove(F5022_37B, (*m_pSnFhyo5_022Data)->Sn_F5022_37B, MONY_BUF_SIZE);
	memmove(F5022_37C, (*m_pSnFhyo5_022Data)->Sn_F5022_37C, MONY_BUF_SIZE);
	memmove(F5022_37X, (*m_pSnFhyo5_022Data)->Sn_F5022_37X, MONY_BUF_SIZE);

}

//==================================
// 集計モジュールにデータセット
//==================================
void CShinFhyo5_022::ReadData2()
{
	//if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {	//　非連動の場合
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {	// 非連動、入力ロックの場合
		if( F5022_20f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352001"), F5022_20A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352001"));
		if( F5022_20f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352002"), F5022_20B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352002"));
		if( F5022_20f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352003"), F5022_20C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352003"));
		if( F5022_20f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352000"), F5022_20X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352000"));
		if( F5022_21f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352101"), F5022_21A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352101"));
		if( F5022_21f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352102"), F5022_21B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352102"));
		if( F5022_21f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352103"), F5022_21C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352103"));
		if( F5022_21f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352100"), F5022_21X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352100"));
		if( F5022_22f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352201"), F5022_22A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352201"));
		if( F5022_22f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352202"), F5022_22B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352202"));
		if( F5022_22f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352203"), F5022_22C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352203"));
		if( F5022_22f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352200"), F5022_22X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352200"));
		if( F5022_23f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352301"), F5022_23A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352301"));
		if( F5022_23f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352302"), F5022_23B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352302"));
		if( F5022_23f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352303"), F5022_23C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352303"));
		if( F5022_23f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352300"), F5022_23X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352300"));
		if( F5022_24f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352401"), F5022_24A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352401"));
		if( F5022_24f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352402"), F5022_24B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352402"));
		if( F5022_24f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352403"), F5022_24C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352403"));
		if( F5022_24f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352400"), F5022_24X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352400"));
		if( F5022_25f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352501"), F5022_25A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352501"));
		if( F5022_25f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352502"), F5022_25B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352502"));
		if( F5022_25f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352503"), F5022_25C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352503"));
		if( F5022_25f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352500"), F5022_25X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352500"));
		if( F5022_26f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352601"), F5022_26A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352601"));
		if( F5022_26f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352602"), F5022_26B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352602"));
		if( F5022_26f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352603"), F5022_26C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352603"));
		if( F5022_26f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352600"), F5022_26X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352600"));
		if( F5022_27f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352701"), F5022_27A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352701"));
		if( F5022_27f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352702"), F5022_27B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352702"));
		if( F5022_27f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352703"), F5022_27C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352703"));
		if( F5022_27f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352700"), F5022_27X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352700"));
		if( F5022_28f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352801"), F5022_28A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352801"));
		if( F5022_28f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352802"), F5022_28B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352802"));
		if( F5022_28f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352803"), F5022_28C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352803"));
		if( F5022_28f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352800"), F5022_28X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352800"));
		if( F5022_29f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352901"), F5022_29A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352901"));
		if( F5022_29f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352902"), F5022_29B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352902"));
		if( F5022_29f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352903"), F5022_29C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352903"));
		if( F5022_29f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("352900"), F5022_29X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("352900"));
		if( F5022_30f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353001"), F5022_30A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353001"));
		if( F5022_30f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353002"), F5022_30B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353002"));
		if( F5022_30f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353003"), F5022_30C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353003"));
		if( F5022_30f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353000"), F5022_30X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353000"));
		if( F5022_31f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353101"), F5022_31A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353101"));
		if( F5022_31f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353102"), F5022_31B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353102"));
		if( F5022_31f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353103"), F5022_31C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353103"));
		if( F5022_31f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353100"), F5022_31X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353100"));
		if( F5022_32f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353201"), F5022_32A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353201"));
		if( F5022_32f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353202"), F5022_32B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353202"));
		if( F5022_32f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353203"), F5022_32C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353203"));
		if( F5022_32f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353200"), F5022_32X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353200"));
		if( F5022_33f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353301"), F5022_33A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353301"));
		if( F5022_33f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353302"), F5022_33B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353302"));
		if( F5022_33f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353303"), F5022_33C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353303"));
		if( F5022_33f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353300"), F5022_33X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353300"));
		if( F5022_34f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353401"), F5022_34A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353401"));
		if( F5022_34f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353402"), F5022_34B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353402"));
		if( F5022_34f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353403"), F5022_34C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353403"));
		if( F5022_34f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353400"), F5022_34X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353400"));
		if( F5022_35f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353501"), F5022_35A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353501"));
		if( F5022_35f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353502"), F5022_35B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353502"));
		if( F5022_35f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353503"), F5022_35C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353503"));
		if( F5022_35f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353500"), F5022_35X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353500"));
		if( F5022_36f&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353601"), F5022_36A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353601"));
		if( F5022_36f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353602"), F5022_36B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353602"));
		if( F5022_36f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353603"), F5022_36C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353603"));
		if( F5022_36f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("353600"), F5022_36X);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("353600"));
		
	}
	else {
	}
}

//============================
// データ書き込み
//============================
void CShinFhyo5_022::WriteData()
{
	char zerobuf[MONY_BUF_SIZE] = { 0 };

	IsEnableItems(1, m_id, m_code, m_typecnt);
	m_4val = Is4Val(0);

	// 4×みなし仕入率
	if ((m_typecnt > 1) && (m_4val)) {
		// A欄が入力可能なら保存
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_20A, F5022_20A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_20B, F5022_20B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_20C, F5022_20C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_20X, F5022_20X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_20sw = F5022_20f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_20A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_20B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_20C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_20X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_20sw = 0;
	}

	// 4×みなし仕入率
	//if (m_id == ID5022_21X) {
	if ( IsDispArea ( ID5022_21A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_21A, F5022_21A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_21B, F5022_21B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_21C, F5022_21C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_21X, F5022_21X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_21sw = F5022_21f;
		(*m_pSnFhyo5_022Data)->Sn_F5022_21kbn = F5022_21kbn;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_21A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_21B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_21C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_21X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_21sw = 0;
		(*m_pSnFhyo5_022Data)->Sn_F5022_21kbn = 0;
	}

	// 第一種事業及び第二種事業
	//if (m_id == ID5022_22X) {
	if ( IsDispArea ( ID5022_22A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_22A, F5022_22A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_22B, F5022_22B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_22C, F5022_22C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_22X, F5022_22X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_22sw = F5022_22f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_22A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_22B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_22C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_22X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_22sw = 0;
	}

	// 第一種事業及び第三種事業
	//if (m_id == ID5022_23X) {
	if ( IsDispArea ( ID5022_23A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_23A, F5022_23A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_23B, F5022_23B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_23C, F5022_23C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_23X, F5022_23X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_23sw = F5022_23f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_23A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_23B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_23C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_23X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_23sw = 0;
	}

	// 第一種事業及び第四種事業
	//if (m_id == ID5022_24X) {
	if ( IsDispArea ( ID5022_24A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_24A, F5022_24A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_24B, F5022_24B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_24C, F5022_24C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_24X, F5022_24X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_24sw = F5022_24f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_24A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_24B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_24C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_24X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_24sw = 0;
	}

	// 第一種事業及び第五種事業
	//if (m_id == ID5022_25X) {
	if ( IsDispArea ( ID5022_25A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_25A, F5022_25A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_25B, F5022_25B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_25C, F5022_25C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_25X, F5022_25X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_25sw = F5022_25f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_25A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_25B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_25C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_25X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_25sw = 0;
	}

	// 第一種事業及び第六種事業
	//if (m_id == ID5022_26X) {
	if ( IsDispArea ( ID5022_26A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_26A, F5022_26A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_26B, F5022_26B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_26C, F5022_26C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_26X, F5022_26X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_26sw = F5022_26f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_26A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_26B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_26C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_26X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_26sw = 0;
	}

	// 第二種事業及び第三種事業
	//if (m_id == ID5022_27X) {
	if ( IsDispArea ( ID5022_27A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_27A, F5022_27A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_27B, F5022_27B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_27C, F5022_27C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_27X, F5022_27X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_27sw = F5022_27f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_27A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_27B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_27C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_27X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_27sw = 0;
	}

	// 第二種事業及び第四種事業
	//if (m_id == ID5022_28X) {
	if ( IsDispArea ( ID5022_28A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_28A, F5022_28A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_28B, F5022_28B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_28C, F5022_28C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_28X, F5022_28X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_28sw = F5022_28f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_28A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_28B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_28C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_28X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_28sw = 0;
	}

	// 第二種事業及び第五種事業
	//if (m_id == ID5022_29X) {
	if ( IsDispArea ( ID5022_29A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_29A, F5022_29A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_29B, F5022_29B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_29C, F5022_29C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_29X, F5022_29X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_29sw = F5022_29f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_29A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_29B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_29C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_29X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_29sw = 0;
	}

	// 第二種事業及び第六種事業
	//if (m_id == ID5022_30X) {
	if ( IsDispArea ( ID5022_30A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_30A, F5022_30A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_30B, F5022_30B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_30C, F5022_30C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_30X, F5022_30X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_30sw = F5022_30f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_30A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_30B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_30C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_30X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_30sw = 0;
	}

	// 第三種事業及び第四種事業
	//if (m_id == ID5022_31X) {
	if ( IsDispArea ( ID5022_31A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_31A, F5022_31A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_31B, F5022_31B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_31C, F5022_31C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_31X, F5022_31X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_31sw = F5022_31f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_31A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_31B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_31C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_31X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_31sw = 0;
	}

	// 第三種事業及び第五種事業
	//if (m_id == ID5022_32X) {
	if ( IsDispArea ( ID5022_32A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_32A, F5022_32A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_32B, F5022_32B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_32C, F5022_32C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_32X, F5022_32X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_32sw = F5022_32f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_32A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_32B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_32C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_32X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_32sw = 0;
	}

	// 第三種事業及び第六種事業
	//if (m_id == ID5022_33X) {
	if ( IsDispArea ( ID5022_33A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_33A, F5022_33A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_33B, F5022_33B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_33C, F5022_33C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_33X, F5022_33X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_33sw = F5022_33f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_33A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_33B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_33C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_33X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_33sw = 0;
	}

	// 第四種事業及び第五種事業
	//if (m_id == ID5022_34X) {
	if ( IsDispArea ( ID5022_34A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_34A, F5022_34A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_34B, F5022_34B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_34C, F5022_34C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_34X, F5022_34X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_34sw = F5022_34f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_34A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_34B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_34C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_34X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_34sw = 0;
	}

	// 第四種事業及び第六種事業
	//if (m_id == ID5022_35X) {
	if ( IsDispArea ( ID5022_35A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_35A, F5022_35A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_35B, F5022_35B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_35C, F5022_35C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_35X, F5022_35X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_35sw = F5022_35f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_35A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_35B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_35C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_35X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_35sw = 0;
	}

	// 第五種事業及び第六種事業
	//if (m_id == ID5022_36X) {
	if ( IsDispArea ( ID5022_36A ) ) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_36A, F5022_36A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_36B, F5022_36B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_36C, F5022_36C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_36X, F5022_36X, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_36sw = F5022_36f;
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_36A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_36B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_36C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_36X, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_022Data)->Sn_F5022_36sw = 0;
	}

	// 選択可能な計算式区分の内から選択した金額
	//---->19.11.05
	if (m_id != 0) {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_37A, F5022_37A, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_37B, F5022_37B, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_37C, F5022_37C, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_37X, F5022_37X, MONY_BUF_SIZE);
	}
	else {
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_37A, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_37B, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_37C, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_022Data)->Sn_F5022_37X, zerobuf, MONY_BUF_SIZE);

	}
	//------------------
//	memmove((*m_pSnFhyo5_022Data)->Sn_F5022_37A, F5022_37A, MONY_BUF_SIZE);
//	memmove((*m_pSnFhyo5_022Data)->Sn_F5022_37B, F5022_37B, MONY_BUF_SIZE);
//	memmove((*m_pSnFhyo5_022Data)->Sn_F5022_37C, F5022_37C, MONY_BUF_SIZE);
//	memmove((*m_pSnFhyo5_022Data)->Sn_F5022_37X, F5022_37X, MONY_BUF_SIZE);
	//<------
}

//==============================
// 4×みなし仕入率
//==============================
void CShinFhyo5_022::f5022_20data()
{
	if ((m_typecnt > 1) && (m_4val)) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_20A, (F5022_20f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_20B, (F5022_20f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_20C, (F5022_20f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_20X, (F5022_20f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_20A, F5022_20A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_20B, F5022_20B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_20C, F5022_20C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_20X, F5022_20X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_20A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_20B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_20C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_20X);
	}
}

//==============================
// 4×みなし仕入率
//==============================
void CShinFhyo5_022::f5022_21data()
{
	/*if (m_id == ID5022_21X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_21A, (F5022_21f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_21B, (F5022_21f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_21C, (F5022_21f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_21X, (F5022_21f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_21A, F5022_21A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_21B, F5022_21B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_21C, F5022_21C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_21X, F5022_21X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_21A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_21B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_21C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_21X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_21A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_21A, ( F5022_21f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_21B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_21B, ( F5022_21f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_21C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_21C, ( F5022_21f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_21X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_21X, ( F5022_21f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_21A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_21A, F5022_21A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_21B, F5022_21B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_21C, F5022_21C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_21X, F5022_21X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_21A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_21B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_21C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_21X );
	}
}

//==============================
// 第一種事業及び第二種事業
//==============================
void CShinFhyo5_022::f5022_22data()
{
	/*if (m_id == ID5022_22X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_22A, (F5022_22f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_22B, (F5022_22f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_22C, (F5022_22f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_22X, (F5022_22f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_22A, F5022_22A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_22B, F5022_22B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_22C, F5022_22C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_22X, F5022_22X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_22A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_22B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_22C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_22X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_22A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_22A, ( F5022_22f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_22B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_22B, ( F5022_22f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_22C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_22C, ( F5022_22f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_22X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_22X, ( F5022_22f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_22A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_22A, F5022_22A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_22B, F5022_22B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_22C, F5022_22C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_22X, F5022_22X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_22A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_22B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_22C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_22X );
	}
}

//==============================
// 第一種事業及び第三種事業
//==============================
void CShinFhyo5_022::f5022_23data()
{
	/*if (m_id == ID5022_23X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_23A, (F5022_23f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_23B, (F5022_23f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_23C, (F5022_23f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_23X, (F5022_23f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_23A, F5022_23A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_23B, F5022_23B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_23C, F5022_23C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_23X, F5022_23X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_23A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_23B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_23C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_23X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_23A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_23A, ( F5022_23f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_23B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_23B, ( F5022_23f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_23C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_23C, ( F5022_23f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_23X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_23X, ( F5022_23f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_23A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_23A, F5022_23A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_23B, F5022_23B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_23C, F5022_23C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_23X, F5022_23X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_23A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_23B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_23C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_23X );
	}
}

//==============================
// 第一種事業及び第四種事業
//==============================
void CShinFhyo5_022::f5022_24data()
{
	/*if (m_id == ID5022_24X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_24A, (F5022_24f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_24B, (F5022_24f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_24C, (F5022_24f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_24X, (F5022_24f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_24A, F5022_24A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_24B, F5022_24B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_24C, F5022_24C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_24X, F5022_24X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_24A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_24B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_24C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_24X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_24A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_24A, ( F5022_24f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_24B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_24B, ( F5022_24f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_24C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_24C, ( F5022_24f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_24X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_24X, ( F5022_24f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_24A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_24A, F5022_24A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_24B, F5022_24B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_24C, F5022_24C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_24X, F5022_24X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_24A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_24B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_24C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_24X );
	}
}

//==============================
// 第一種事業及び第五種事業
//==============================
void CShinFhyo5_022::f5022_25data()
{
	/*if (m_id == ID5022_25X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_25A, (F5022_25f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_25B, (F5022_25f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_25C, (F5022_25f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_25X, (F5022_25f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_25A, F5022_25A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_25B, F5022_25B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_25C, F5022_25C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_25X, F5022_25X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_25A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_25B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_25C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_25X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_25A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_25A, ( F5022_25f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_25B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_25B, ( F5022_25f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_25C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_25C, ( F5022_25f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_25X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_25X, ( F5022_25f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_25A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_25A, F5022_25A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_25B, F5022_25B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_25C, F5022_25C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_25X, F5022_25X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_25A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_25B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_25C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_25X );
	}
}

//==============================
// 第一種事業及び第六種事業
//==============================
void CShinFhyo5_022::f5022_26data()
{
	/*if (m_id == ID5022_26X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_26A, (F5022_26f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_26B, (F5022_26f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_26C, (F5022_26f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_26X, (F5022_26f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_26A, F5022_26A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_26B, F5022_26B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_26C, F5022_26C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_26X, F5022_26X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_26A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_26B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_26C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_26X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_26A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_26A, ( F5022_26f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_26B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_26B, ( F5022_26f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_26C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_26C, ( F5022_26f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_26X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_26X, ( F5022_26f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_26A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_26A, F5022_26A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_26B, F5022_26B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_26C, F5022_26C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_26X, F5022_26X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_26A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_26B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_26C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_26X );
	}
}

//==============================
// 第二種事業及び第三種事業
//==============================
void CShinFhyo5_022::f5022_27data()
{
	/*if (m_id == ID5022_27X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_27A, (F5022_27f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_27B, (F5022_27f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_27C, (F5022_27f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_27X, (F5022_27f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_27A, F5022_27A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_27B, F5022_27B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_27C, F5022_27C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_27X, F5022_27X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_27A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_27B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_27C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_27X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_27A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_27A, ( F5022_27f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_27B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_27B, ( F5022_27f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_27C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_27C, ( F5022_27f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_27X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_27X, ( F5022_27f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_27A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_27A, F5022_27A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_27B, F5022_27B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_27C, F5022_27C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_27X, F5022_27X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_27A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_27B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_27C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_27X );
	}
}

//==============================
// 第二種事業及び第四種事業
//==============================
void CShinFhyo5_022::f5022_28data()
{
	/*if (m_id == ID5022_28X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_28A, (F5022_28f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_28B, (F5022_28f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_28C, (F5022_28f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_28X, (F5022_28f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_28A, F5022_28A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_28B, F5022_28B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_28C, F5022_28C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_28X, F5022_28X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_28A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_28B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_28C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_28X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_28A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_28A, ( F5022_28f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_28B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_28B, ( F5022_28f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_28C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_28C, ( F5022_28f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_28X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_28X, ( F5022_28f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_28A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_28A, F5022_28A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_28B, F5022_28B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_28C, F5022_28C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_28X, F5022_28X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_28A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_28B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_28C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_28X );
	}
}

//==============================
// 第二種事業及び第五種事業
//==============================
void CShinFhyo5_022::f5022_29data()
{
	/*if (m_id == ID5022_29X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_29A, (F5022_29f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_29B, (F5022_29f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_29C, (F5022_29f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_29X, (F5022_29f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_29A, F5022_29A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_29B, F5022_29B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_29C, F5022_29C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_29X, F5022_29X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_29A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_29B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_29C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_29X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_29A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_29A, ( F5022_29f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_29B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_29B, ( F5022_29f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_29C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_29C, ( F5022_29f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_29X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_29X, ( F5022_29f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_29A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_29A, F5022_29A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_29B, F5022_29B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_29C, F5022_29C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_29X, F5022_29X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_29A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_29B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_29C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_29X );
	}
}

//==============================
// 第二種事業及び第六種事業
//==============================
void CShinFhyo5_022::f5022_30data()
{
	/*if (m_id == ID5022_30X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_30A, (F5022_30f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_30B, (F5022_30f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_30C, (F5022_30f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_30X, (F5022_30f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_30A, F5022_30A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_30B, F5022_30B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_30C, F5022_30C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_30X, F5022_30X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_30A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_30B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_30C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_30X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_30A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_30A, ( F5022_30f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_30B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_30B, ( F5022_30f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_30C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_30C, ( F5022_30f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_30X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_30X, ( F5022_30f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_30A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_30A, F5022_30A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_30B, F5022_30B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_30C, F5022_30C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_30X, F5022_30X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_30A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_30B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_30C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_30X );
	}
}

//==============================
// 第三種事業及び第四種事業
//==============================
void CShinFhyo5_022::f5022_31data()
{
	/*if (m_id == ID5022_31X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_31A, (F5022_31f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_31B, (F5022_31f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_31C, (F5022_31f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_31X, (F5022_31f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_31A, F5022_31A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_31B, F5022_31B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_31C, F5022_31C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_31X, F5022_31X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_31A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_31B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_31C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_31X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_31A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_31A, ( F5022_31f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_31B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_31B, ( F5022_31f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_31C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_31C, ( F5022_31f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_31X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_31X, ( F5022_31f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_31A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_31A, F5022_31A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_31B, F5022_31B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_31C, F5022_31C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_31X, F5022_31X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_31A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_31B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_31C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_31X );
	}
}

//==============================
// 第三種事業及び第五種事業
//==============================
void CShinFhyo5_022::f5022_32data()
{
	/*if (m_id == ID5022_32X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_32A, (F5022_32f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_32B, (F5022_32f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_32C, (F5022_32f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_32X, (F5022_32f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_32A, F5022_32A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_32B, F5022_32B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_32C, F5022_32C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_32X, F5022_32X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_32A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_32B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_32C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_32X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( (*m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_32A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_32A, ( F5022_32f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_32B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_32B, ( F5022_32f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_32C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_32C, ( F5022_32f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_32X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_32X, ( F5022_32f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_32A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_32A, F5022_32A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_32B, F5022_32B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_32C, F5022_32C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_32X, F5022_32X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_32A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_32B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_32C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_32X );
	}
}

//==============================
// 第三種事業及び第六種事業
//==============================
void CShinFhyo5_022::f5022_33data()
{
	/*if (m_id == ID5022_33X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_33A, (F5022_33f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_33B, (F5022_33f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_33C, (F5022_33f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_33X, (F5022_33f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_33A, F5022_33A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_33B, F5022_33B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_33C, F5022_33C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_33X, F5022_33X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_33A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_33B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_33C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_33X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( (*m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_33A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_33A, ( F5022_33f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_33B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_33B, ( F5022_33f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_33C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_33C, ( F5022_33f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_33X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_33X, ( F5022_33f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_33A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_33A, F5022_33A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_33B, F5022_33B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_33C, F5022_33C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_33X, F5022_33X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_33A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_33B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_33C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_33X );
	}
}

//==============================
// 第四種事業及び第五種事業
//==============================
void CShinFhyo5_022::f5022_34data()
{
	/*if (m_id == ID5022_34X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_34A, (F5022_34f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_34B, (F5022_34f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_34C, (F5022_34f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_34X, (F5022_34f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_34A, F5022_34A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_34B, F5022_34B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_34C, F5022_34C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_34X, F5022_34X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_34A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_34B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_34C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_34X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_34A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_34A, ( F5022_34f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_34B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_34B, ( F5022_34f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_34C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_34C, ( F5022_34f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_34X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_34X, ( F5022_34f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_34A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_34A, F5022_34A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_34B, F5022_34B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_34C, F5022_34C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_34X, F5022_34X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_34A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_34B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_34C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_34X );
	}
}

//==============================
// 第四種事業及び第六種事業
//==============================
void CShinFhyo5_022::f5022_35data()
{
	/*if (m_id == ID5022_35X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_35A, (F5022_35f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_35B, (F5022_35f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_35C, (F5022_35f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_35X, (F5022_35f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_35A, F5022_35A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_35B, F5022_35B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_35C, F5022_35C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_35X, F5022_35X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_35A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_35B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_35C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_35X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_35A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_35A, ( F5022_35f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_35B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_35B, ( F5022_35f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_35C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_35C, ( F5022_35f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_35X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_35X, ( F5022_35f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_35A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_35A, F5022_35A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_35B, F5022_35B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_35C, F5022_35C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_35X, F5022_35X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_35A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_35B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_35C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_35X );
	}
}

//==============================
// 第五種事業及び第六種事業
//==============================
void CShinFhyo5_022::f5022_36data()
{
	/*if (m_id == ID5022_36X) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_36A, (F5022_36f & 0x08) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_36B, (F5022_36f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_36C, (F5022_36f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5022_36X, (F5022_36f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_36A, F5022_36A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_36B, F5022_36B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_36C, F5022_36C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_36X, F5022_36X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_36A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_36B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_36C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_36X);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5022_36A ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_36A, ( F5022_36f & 0x08 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_36B ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_36B, ( F5022_36f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_36C ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_36C, ( F5022_36f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5022_36X ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5022_36X, ( F5022_36f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5022_36A ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_36A, F5022_36A );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_36B, F5022_36B );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_36C, F5022_36C );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5022_36X, F5022_36X );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_36A );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_36B );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_36C );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5022_36X );
	}
}

//============================================
// 選択可能な計算式区分の内から選択した金額
//============================================
void CShinFhyo5_022::f5022_37data()
{
	//---->19.11.05
	if (m_id != 0) {
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_37A, F5022_37A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_37B, F5022_37B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_37C, F5022_37C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_37X, F5022_37X);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_37A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_37B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_37C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5022_37X);
	}
	//-------
//	dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_37A, F5022_37A);
//	dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_37B, F5022_37B);
//	dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_37C, F5022_37C);
//	dsp_prs(IDC_ICSDIAG8CTRL1, ID5022_37X, F5022_37X);
	//<------
}

//============================================
// 入力データの表示
//--------------------------------------------
// 引数
//		index	：　
//============================================
void CShinFhyo5_022::Disp_f5022data( int index )
{
	switch (index) {
		case 0:
		case 1:
			f5022_20data();
		case 2:
			f5022_21data();
		case 3:
			f5022_22data();
		case 4:
			f5022_23data();
		case 5:
			f5022_24data();
		case 6:
			f5022_25data();
		case 7:
			f5022_26data();
		case 8:
			f5022_27data();
		case 9:
			f5022_28data();
		case 10:
			f5022_29data();
		case 11:
			f5022_30data();
		case 12:
			f5022_31data();
		case 13:
			f5022_32data();
		case 14:
			f5022_33data();
		case 15:
			f5022_34data();
		case 16:
			f5022_35data();
		case 17:
			f5022_36data();
		case 18:
			f5022_37data();

		default:
			break;
	}
}

//============================================
// 画面表示
//============================================
void CShinFhyo5_022::Disp_f5022()
{
	// 画面初期化
	for (int i = ID5022_20A; i <= ID5022_37X; i++) {
		dsp_cls(IDC_ICSDIAG8CTRL1, (short)i);
	}

	// ★手入力サイン初期化？

	InitDiagAttr();

	/*long flg = 0;
	BOOL sw = FALSE;
	if ( ( *m_pSnHeadData )->Sn_GenericSgn & 0x08 ) {
		flg = DIAG_MDFY_OPAQUE;
		sw = TRUE;
	}
	else {
		flg = DIAG_MDFY_TRANSPARENT;
		sw = TRUE;
	}
	if ( ChangeBackColor ( sw ) == 0 ) {
		m_Fhyo5_022Diag01.ModifyItem ( SELECTITEM, flg );
	}
	else {
		m_Fhyo5_022Diag01.ModifyItem ( SELECTITEM, DIAG_MDFY_TRANSPARENT );
	}*/

	// 入力データの表示
	Disp_f5022data(0);
}

//============================================
// 金額表示
//============================================
void CShinFhyo5_022::dsp_prs(unsigned short Id, short Index, char* Mony)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	memmove(DiagData.data_val, Mony, MONY_BUF_SIZE);
	diag_setdata(Id, Index, (struct IUnknown*)&DiagData, CIcsdiagctrl);
}

//============================================
// 金額表示クリア
//============================================
void CShinFhyo5_022::dsp_cls(unsigned short Id, short Index)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
		
	memset(DiagData.data_val, '\0', MONY_BUF_SIZE);
	diag_setdata(Id, Index, (struct IUnknown*)&DiagData, CIcsdiagctrl);
}

//============================================
// 各入力データ抽出
//============================================
void CShinFhyo5_022::GetAllDiagData()
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// 4×みなし仕入率
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_20A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_20A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_20B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_20B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_20C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_20C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_20X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_20X, DiagData.data_val, MONY_BUF_SIZE);

	// 4×みなし仕入率
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_21A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_21A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_21B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_21B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_21C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_21C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_21X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_21X, DiagData.data_val, MONY_BUF_SIZE);

	// 第一種事業及び第二種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_22A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_22A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_22B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_22B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_22C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_22C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_22X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_22X, DiagData.data_val, MONY_BUF_SIZE);

	// 第一種事業及び第三種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_23A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_23A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_23B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_23B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_23C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_23C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_23X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_23X, DiagData.data_val, MONY_BUF_SIZE);

	// 第一種事業及び第四種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_24A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_24A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_24B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_24B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_24C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_24C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_24X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_24X, DiagData.data_val, MONY_BUF_SIZE);

	// 第一種事業及び第五種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_25A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_25A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_25B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_25B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_25C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_25C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_25X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_25X, DiagData.data_val, MONY_BUF_SIZE);

	// 第一種事業及び第六種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_26A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_26A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_26B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_26B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_26C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_26C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_26X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_26X, DiagData.data_val, MONY_BUF_SIZE);

	// 第二種事業及び第三種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_27A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_27A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_27B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_27B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_27C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_27C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_27X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_27X, DiagData.data_val, MONY_BUF_SIZE);

	// 第二種事業及び第四種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_28A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_28A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_28B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_28B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_28C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_28C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_28X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_28X, DiagData.data_val, MONY_BUF_SIZE);

	// 第二種事業及び第五種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_29A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_29A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_29B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_29B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_29C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_29C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_29X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_29X, DiagData.data_val, MONY_BUF_SIZE);

	// 第二種事業及び第六種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_30A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_30A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_30B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_30B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_30C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_30C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_30X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_30X, DiagData.data_val, MONY_BUF_SIZE);

	// 第三種事業及び第四種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_31A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_31A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_31B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_31B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_31C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_31C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_31X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_31X, DiagData.data_val, MONY_BUF_SIZE);

	// 第三種事業及び第五種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_32A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_32A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_32B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_32B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_32C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_32C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_32X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_32X, DiagData.data_val, MONY_BUF_SIZE);

	// 第三種事業及び第六種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_33A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_33A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_33B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_33B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_33C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_33C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_33X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_33X, DiagData.data_val, MONY_BUF_SIZE);

	// 第四種事業及び第五種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_34A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_34A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_34B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_34B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_34C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_34C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_34X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_34X, DiagData.data_val, MONY_BUF_SIZE);

	// 第四種事業及び第六種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_35A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_35A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_35B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_35B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_35C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_35C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_35X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_35X, DiagData.data_val, MONY_BUF_SIZE);

	// 第五種事業及び第六種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_36A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_36A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_36B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_36B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_36C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_36C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_36X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_36X, DiagData.data_val, MONY_BUF_SIZE);

	// 選択可能な計算式区分の内から選択した金額
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_37A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_37A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_37B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_37B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_37C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_37C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5022_37X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5022_37X, DiagData.data_val, MONY_BUF_SIZE);

}

BOOL CShinFhyo5_022::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_END) {
			GetAllDiagData();
			WriteData();
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
				if (m_curwnd == IDC_ICSDIAG8CTRL1) {
					TerminationIcsdiag8ctrl1(result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd);
				}

				return TRUE;
			}
		}
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

//=================================================
// 集計モジュール更新
//-------------------------------------------------
// 引数
//		cID		:		ダイアグラム内インデックス
//-------------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//=================================================
int CShinFhyo5_022::SyukeiDataSet ( int cID )
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char val[MONY_BUF_SIZE] = { 0 };

	diag_getdata ( IDC_ICSDIAG8CTRL1, cID, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( val, DiagData.data_val, MONY_BUF_SIZE );

	switch ( cID ) {
	case ID5022_20A:
		if (m_pArith->l_cmp(F5022_20A, val)) {
			F5022_20f |= 0x08;
			memmove(F5022_20A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352001"), F5022_20A);
		}
		break;
	case ID5022_20B:
		if (m_pArith->l_cmp(F5022_20B, val)) {
			F5022_20f |= 0x80;
			memmove(F5022_20B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352002"), F5022_20B);
		}
		break;
	case ID5022_20C:
		if (m_pArith->l_cmp(F5022_20C, val)) {
			F5022_20f |= 0x800;
			memmove(F5022_20C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352003"), F5022_20C);
		}
		break;
	case ID5022_20X:
		if (m_pArith->l_cmp(F5022_20X, val)) {
			F5022_20f |= 0x8000;
			memmove(F5022_20X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352000"), F5022_20X);
		}
		break;
	case ID5022_21A:
		if (m_pArith->l_cmp(F5022_21A, val)) {
			F5022_21f |= 0x08;
			memmove(F5022_21A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352101"), F5022_21A);
		}
		break;
	case ID5022_21B:
		if (m_pArith->l_cmp(F5022_21B, val)) {
			F5022_21f |= 0x80;
			memmove(F5022_21B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352102"), F5022_21B);
		}
		break;
	case ID5022_21C:
		if (m_pArith->l_cmp(F5022_21C, val)) {
			F5022_21f |= 0x800;
			memmove(F5022_21C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352103"), F5022_21C);
		}
		break;
	case ID5022_21X:
		if (m_pArith->l_cmp(F5022_21X, val)) {
			F5022_21f |= 0x8000;
			memmove(F5022_21X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352100"), F5022_21X);
		}
		break;
	case ID5022_22A:
		if (m_pArith->l_cmp(F5022_22A, val)) {
			F5022_22f |= 0x08;
			memmove(F5022_22A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352201"), F5022_22A);
		}
		break;
	case ID5022_22B:
		if (m_pArith->l_cmp(F5022_22B, val)) {
			F5022_22f |= 0x80;
			memmove(F5022_22B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352202"), F5022_22B);
		}
		break;
	case ID5022_22C:
		if (m_pArith->l_cmp(F5022_22C, val)) {
			F5022_22f |= 0x800;
			memmove(F5022_22C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352203"), F5022_22C);
		}
		break;
	case ID5022_22X:
		if (m_pArith->l_cmp(F5022_22X, val)) {
			F5022_22f |= 0x8000;
			memmove(F5022_22X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352200"), F5022_22X);
		}
		break;
	case ID5022_23A:
		if (m_pArith->l_cmp(F5022_23A, val)) {
			F5022_23f |= 0x08;
			memmove(F5022_23A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352301"), F5022_23A);
		}
		break;
	case ID5022_23B:
		if (m_pArith->l_cmp(F5022_23B, val)) {
			F5022_23f |= 0x80;
			memmove(F5022_23B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352302"), F5022_23B);
		}
		break;
	case ID5022_23C:
		if (m_pArith->l_cmp(F5022_23C, val)) {
			F5022_23f |= 0x800;
			memmove(F5022_23C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352303"), F5022_23C);
		}
		break;
	case ID5022_23X:
		if (m_pArith->l_cmp(F5022_23X, val)) {
			F5022_23f |= 0x8000;
			memmove(F5022_23X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352300"), F5022_23X);
		}
		break;
	case ID5022_24A:
		if (m_pArith->l_cmp(F5022_24A, val)) {
			F5022_24f |= 0x08;
			memmove(F5022_24A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352401"), F5022_24A);
		}
		break;
	case ID5022_24B:
		if (m_pArith->l_cmp(F5022_24B, val)) {
			F5022_24f |= 0x80;
			memmove(F5022_24B, val, MONY_BUF_SIZE);
			//(*m_pSyzSyukei)->SetShinkokuData(_T("352402"), F5022_21B);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352402"), F5022_24B);
		}
		break;
	case ID5022_24C:
		if (m_pArith->l_cmp(F5022_24C, val)) {
			F5022_24f |= 0x800;
			memmove(F5022_24C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352403"), F5022_24C);
		}
		break;
	case ID5022_24X:
		if (m_pArith->l_cmp(F5022_24X, val)) {
			F5022_24f |= 0x8000;
			memmove(F5022_24X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352400"), F5022_24X);
		}
		break;
	case ID5022_25A:
		if (m_pArith->l_cmp(F5022_25A, val)) {
			F5022_25f |= 0x08;
			memmove(F5022_25A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352501"), F5022_25A);
		}
		break;
	case ID5022_25B:
		if (m_pArith->l_cmp(F5022_25B, val)) {
			F5022_25f |= 0x80;
			memmove(F5022_25B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352502"), F5022_25B);
		}
		break;
	case ID5022_25C:
		if (m_pArith->l_cmp(F5022_25C, val)) {
			F5022_25f |= 0x800;
			memmove(F5022_25C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352503"), F5022_25C);
		}
		break;
	case ID5022_25X:
		if (m_pArith->l_cmp(F5022_25X, val)) {
			F5022_25f |= 0x8000;
			memmove(F5022_25X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352500"), F5022_25X);
		}
		break;
	case ID5022_26A:
		if (m_pArith->l_cmp(F5022_26A, val)) {
			F5022_26f |= 0x08;
			memmove(F5022_26A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352601"), F5022_26A);
		}
		break;
	case ID5022_26B:
		if (m_pArith->l_cmp(F5022_26B, val)) {
			F5022_26f |= 0x80;
			memmove(F5022_26B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352602"), F5022_26B);
		}
		break;
	case ID5022_26C:
		if (m_pArith->l_cmp(F5022_26C, val)) {
			F5022_26f |= 0x800;
			memmove(F5022_26C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352603"), F5022_26C);
		}
		break;
	case ID5022_26X:
		if (m_pArith->l_cmp(F5022_26X, val)) {
			F5022_26f |= 0x8000;
			memmove(F5022_26X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352600"), F5022_26X);
		}
		break;
	case ID5022_27A:
		if (m_pArith->l_cmp(F5022_27A, val)) {
			F5022_27f |= 0x08;
			memmove(F5022_27A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352701"), F5022_27A);
		}
		break;
	case ID5022_27B:
		if (m_pArith->l_cmp(F5022_27B, val)) {
			F5022_27f |= 0x80;
			memmove(F5022_27B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352702"), F5022_27B);
		}
		break;
	case ID5022_27C:
		if (m_pArith->l_cmp(F5022_27C, val)) {
			F5022_27f |= 0x800;
			memmove(F5022_27C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352703"), F5022_27C);
		}
		break;
	case ID5022_27X:
		if (m_pArith->l_cmp(F5022_27X, val)) {
			F5022_27f |= 0x8000;
			memmove(F5022_27X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352700"), F5022_27X);
		}
		break;
	case ID5022_28A:
		if (m_pArith->l_cmp(F5022_28A, val)) {
			F5022_28f |= 0x08;
			memmove(F5022_28A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352801"), F5022_28A);
		}
		break;
	case ID5022_28B:
		if (m_pArith->l_cmp(F5022_28B, val)) {
			F5022_28f |= 0x80;
			memmove(F5022_28B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352802"), F5022_28B);
		}
		break;
	case ID5022_28C:
		if (m_pArith->l_cmp(F5022_28C, val)) {
			F5022_28f |= 0x800;
			memmove(F5022_28C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352803"), F5022_28C);
		}
		break;
	case ID5022_28X:
		if (m_pArith->l_cmp(F5022_28X, val)) {
			F5022_28f |= 0x8000;
			memmove(F5022_28X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352800"), F5022_28X);
		}
		break;
	case ID5022_29A:
		if (m_pArith->l_cmp(F5022_29A, val)) {
			F5022_29f |= 0x08;
			memmove(F5022_29A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352901"), F5022_29A);
		}
		break;
	case ID5022_29B:
		if (m_pArith->l_cmp(F5022_29B, val)) {
			F5022_29f |= 0x80;
			memmove(F5022_29B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352902"), F5022_29B);
		}
		break;
	case ID5022_29C:
		if (m_pArith->l_cmp(F5022_29C, val)) {
			F5022_29f |= 0x800;
			memmove(F5022_29C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352903"), F5022_29C);
		}
		break;
	case ID5022_29X:
		if (m_pArith->l_cmp(F5022_29X, val)) {
			F5022_29f |= 0x8000;
			memmove(F5022_29X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("352900"), F5022_29X);
		}
		break;
	case ID5022_30A:
		if (m_pArith->l_cmp(F5022_30A, val)) {
			F5022_30f |= 0x08;
			memmove(F5022_30A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353001"), F5022_30A);
		}
		break;
	case ID5022_30B:
		if (m_pArith->l_cmp(F5022_30B, val)) {
			F5022_30f |= 0x80;
			memmove(F5022_30B, val, MONY_BUF_SIZE);
			//(*m_pSyzSyukei)->SetShinkokuData(_T("352102"), F5022_30B);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353002"), F5022_30B);
		}
		break;
	case ID5022_30C:
		if (m_pArith->l_cmp(F5022_30C, val)) {
			F5022_30f |= 0x800;
			memmove(F5022_30C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353003"), F5022_30C);
		}
		break;
	case ID5022_30X:
		if (m_pArith->l_cmp(F5022_30X, val)) {
			F5022_30f |= 0x8000;
			memmove(F5022_30X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353000"), F5022_30X);
		}
		break;
	case ID5022_31A:
		if (m_pArith->l_cmp(F5022_31A, val)) {
			F5022_31f |= 0x08;
			memmove(F5022_31A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353101"), F5022_31A);
		}
		break;
	case ID5022_31B:
		if (m_pArith->l_cmp(F5022_31B, val)) {
			F5022_31f |= 0x80;
			memmove(F5022_31B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353102"), F5022_31B);
		}
		break;
	case ID5022_31C:
		if (m_pArith->l_cmp(F5022_31C, val)) {
			F5022_31f |= 0x800;
			memmove(F5022_31C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353103"), F5022_31C);
		}
		break;
	case ID5022_31X:
		if (m_pArith->l_cmp(F5022_31X, val)) {
			F5022_31f |= 0x8000;
			memmove(F5022_31X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353100"), F5022_31X);
		}
		break;
	case ID5022_32A:
		if (m_pArith->l_cmp(F5022_32A, val)) {
			F5022_32f |= 0x08;
			memmove(F5022_32A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353201"), F5022_32A);
		}
		break;
	case ID5022_32B:
		if (m_pArith->l_cmp(F5022_32B, val)) {
			F5022_32f |= 0x80;
			memmove(F5022_32B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353202"), F5022_32B);
		}
		break;
	case ID5022_32C:
		if (m_pArith->l_cmp(F5022_32C, val)) {
			F5022_32f |= 0x800;
			memmove(F5022_32C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353203"), F5022_32C);
		}
		break;
	case ID5022_32X:
		if (m_pArith->l_cmp(F5022_32X, val)) {
			F5022_32f |= 0x8000;
			memmove(F5022_32X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353200"), F5022_32X);
		}
		break;
	case ID5022_33A:
		if (m_pArith->l_cmp(F5022_33A, val)) {
			F5022_33f |= 0x08;
			memmove(F5022_33A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353301"), F5022_33A);
		}
		break;
	case ID5022_33B:
		if (m_pArith->l_cmp(F5022_33B, val)) {
			F5022_33f |= 0x80;
			memmove(F5022_33B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353302"), F5022_33B);
		}
		break;
	case ID5022_33C:
		if (m_pArith->l_cmp(F5022_33C, val)) {
			F5022_33f |= 0x800;
			memmove(F5022_33C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353303"), F5022_33C);
		}
		break;
	case ID5022_33X:
		if (m_pArith->l_cmp(F5022_33X, val)) {
			F5022_33f |= 0x8000;
			memmove(F5022_33X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353300"), F5022_33X);
		}
		break;
	case ID5022_34A:
		if (m_pArith->l_cmp(F5022_34A, val)) {
			F5022_34f |= 0x08;
			memmove(F5022_34A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353401"), F5022_34A);
		}
		break;
	case ID5022_34B:
		if (m_pArith->l_cmp(F5022_34B, val)) {
			F5022_34f |= 0x80;
			memmove(F5022_34B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353402"), F5022_34B);
		}
		break;
	case ID5022_34C:
		if (m_pArith->l_cmp(F5022_34C, val)) {
			F5022_34f |= 0x800;
			memmove(F5022_34C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353403"), F5022_34C);
		}
		break;
	case ID5022_34X:
		if (m_pArith->l_cmp(F5022_34X, val)) {
			F5022_34f |= 0x8000;
			memmove(F5022_34X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353400"), F5022_34X);
		}
		break;
	case ID5022_35A:
		if (m_pArith->l_cmp(F5022_35A, val)) {
			F5022_35f |= 0x08;
			memmove(F5022_35A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353501"), F5022_35A);
		}
		break;
	case ID5022_35B:
		if (m_pArith->l_cmp(F5022_35B, val)) {
			F5022_35f |= 0x80;
			memmove(F5022_35B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353502"), F5022_35B);
		}
		break;
	case ID5022_35C:
		if (m_pArith->l_cmp(F5022_35C, val)) {
			F5022_35f |= 0x800;
			memmove(F5022_35C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353503"), F5022_35C);
		}
		break;
	case ID5022_35X:
		if (m_pArith->l_cmp(F5022_35X, val)) {
			F5022_35f |= 0x8000;
			memmove(F5022_35X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353500"), F5022_35X);
		}
		break;
	case ID5022_36A:
		if (m_pArith->l_cmp(F5022_36A, val)) {
			F5022_36f |= 0x08;
			memmove(F5022_36A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353601"), F5022_36A);
		}
		break;
	case ID5022_36B:
		if (m_pArith->l_cmp(F5022_36B, val)) {
			F5022_36f |= 0x80;
			memmove(F5022_36B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353602"), F5022_36B);
		}
		break;
	case ID5022_36C:
		if (m_pArith->l_cmp(F5022_36C, val)) {
			F5022_36f |= 0x800;
			memmove(F5022_36C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353603"), F5022_36C);
			//break;
		}
		break;
	case ID5022_36X:
		if (m_pArith->l_cmp(F5022_36X, val)) {
			F5022_36f |= 0x8000;
			memmove(F5022_36X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("353600"), F5022_36X);
		}
		break;
	default:
		break;
	}

	CalqGokei ( cID );

	return MemoryReget();
}

//============================================
// 集計モジュールからデータ取得
//--------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//============================================
int CShinFhyo5_022::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	// 計算項目をすべて再取得
	MoneyBasejagArray money;

	m_money35 = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35" ) );

	money = m_money35;

	int cnt = ( int )money.GetCount();
	int incnt = 0;
	//char buf[6];

	for ( int l = 0; l < cnt; l++ ) {
		incnt = ( int )money[l].GetCount();
		for ( int m = 0; m < incnt; m++ ) {
			if ( money[l][m].code == _T( "352000" ) ) {
				memmove ( F5022_20X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352001" ) ) {
				memmove ( F5022_20A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352002" ) ) {
				memmove ( F5022_20B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352003" ) ) {
				memmove ( F5022_20C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352100" ) ) {
				memmove ( F5022_21X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352101" ) ) {
				memmove ( F5022_21A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352102" ) ) {
				memmove ( F5022_21B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352103" ) ) {
				memmove ( F5022_21C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352200" ) ) {
				memmove ( F5022_22X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352201" ) ) {
				memmove ( F5022_22A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352202" ) ) {
				memmove ( F5022_22B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352203" ) ) {
				memmove ( F5022_22C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352300" ) ) {
				memmove ( F5022_23X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352301" ) ) {
				memmove ( F5022_23A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352302" ) ) {
				memmove ( F5022_23B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352303" ) ) {
				memmove ( F5022_23C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352400" ) ) {
				memmove ( F5022_24X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352401" ) ) {
				memmove ( F5022_24A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352402" ) ) {
				memmove ( F5022_24B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352403" ) ) {
				memmove ( F5022_24C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352500" ) ) {
				memmove ( F5022_25X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352501" ) ) {
				memmove ( F5022_25A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352502" ) ) {
				memmove ( F5022_25B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352503" ) ) {
				memmove ( F5022_25C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352600" ) ) {
				memmove ( F5022_26X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352601" ) ) {
				memmove ( F5022_26A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352602" ) ) {
				memmove ( F5022_26B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352603" ) ) {
				memmove ( F5022_26C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352700" ) ) {
				memmove ( F5022_27X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352701" ) ) {
				memmove ( F5022_27A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352702" ) ) {
				memmove ( F5022_27B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352703" ) ) {
				memmove ( F5022_27C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352800" ) ) {
				memmove ( F5022_28X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352801" ) ) {
				memmove ( F5022_28A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352802" ) ) {
				memmove ( F5022_28B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352803" ) ) {
				memmove ( F5022_28C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352900" ) ) {
				memmove ( F5022_29X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352901" ) ) {
				memmove ( F5022_29A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352902" ) ) {
				memmove ( F5022_29B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "352903" ) ) {
				memmove ( F5022_29C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353000" ) ) {
				memmove ( F5022_30X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353001" ) ) {
				memmove ( F5022_30A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353002" ) ) {
				memmove ( F5022_30B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353003" ) ) {
				memmove ( F5022_30C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353100" ) ) {
				memmove ( F5022_31X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353101" ) ) {
				memmove ( F5022_31A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353102" ) ) {
				memmove ( F5022_31B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353103" ) ) {
				memmove ( F5022_31C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353200" ) ) {
				memmove ( F5022_32X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353201" ) ) {
				memmove ( F5022_32A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353202" ) ) {
				memmove ( F5022_32B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353203" ) ) {
				memmove ( F5022_32C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353300" ) ) {
				memmove ( F5022_33X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353301" ) ) {
				memmove ( F5022_33A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353302" ) ) {
				memmove ( F5022_33B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353303" ) ) {
				memmove ( F5022_33C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353400" ) ) {
				memmove ( F5022_34X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353401" ) ) {
				memmove ( F5022_34A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353402" ) ) {
				memmove ( F5022_34B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353403" ) ) {
				memmove ( F5022_34C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353500" ) ) {
				memmove ( F5022_35X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353501" ) ) {
				memmove ( F5022_35A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353502" ) ) {
				memmove ( F5022_35B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353503" ) ) {
				memmove ( F5022_35C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353600" ) ) {
				memmove ( F5022_36X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353601" ) ) {
				memmove ( F5022_36A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353602" ) ) {
				memmove ( F5022_36B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353603" ) ) {
				memmove ( F5022_36C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353700" ) ) {
				memmove ( F5022_37X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353701" ) ) {
				memmove ( F5022_37A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353702" ) ) {
				memmove ( F5022_37B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "353703" ) ) {
				memmove ( F5022_37C, money[l][m].arith, MONY_BUF_SIZE );
			}
		}
	}

	return 0;
}


//==============================================
// 有利な計算式を判別
//----------------------------------------------
// 引数
//		reflg	:	(ON)再集計フラグ
//		Idx		:	合計行のインデックス
//		code	:	合計行のコード
//		typeCnt	:	業種数
//----------------------------------------------
// 返送値
//
//==============================================
int CShinFhyo5_022::IsEnableItems(int reflg, short& Idx, CString& code, int& typeCnt)
{
	MoneyBasejagArray	money;
//	char	val[MONY_BUF_SIZE],
	char 	max_valttl[MONY_BUF_SIZE] = { 0 };
	char	buff[256];
	//short	id, 
	short   maxid = 0;
	CString	maxcode;

	typeCnt = 0;
	/*money = (*m_pSyzSyukei)->GetShinkokuData( _T("35") );

	int cnt = money.GetCount();
	int incnt = 0;

	for (int i = 0; i < cnt; i++) {
		incnt = money[i].GetCount();
		for (int j = 0; j < incnt; j++) {
			// 業種数
			if (money[i][j].code == _T("35F000")) {
				m_pArith->l_print( buff, money[i][j].arith, _T("SSS") );
				typeCnt = atoi( buff );
			}
			// 4×みなし仕入率
			else if (money[i][j].code == _T("35E000")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_21X;
			}
			// 第一種事業及び第二種事業
			else if (money[i][j].code == _T("35E001")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_22X;
			}
			// 第一種事業及び第三種事業
			else if (money[i][j].code == _T("35E002")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_23X;
			}
			// 第一種事業及び第四種事業
			else if (money[i][j].code == _T("35E003")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_24X;
			}
			// 第一種事業及び第五種事業
			else if (money[i][j].code == _T("35E004")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_25X;
			}
			// 第一種事業及び第六種事業
			else if (money[i][j].code == _T("35E005")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_26X;
			}
			// 第二種事業及び第三種事業
			else if (money[i][j].code == _T("35E006")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_27X;
			}
			// 第二種事業及び第四種事業
			else if (money[i][j].code == _T("35E007")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_28X;
			}
			// 第二種事業及び第五種事業
			else if (money[i][j].code == _T("35E008")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_29X;
			}
			// 第二種事業及び第六種事業
			else if (money[i][j].code == _T("35E009")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_30X;
			}
			// 第三種事業及び第四種事業
			else if (money[i][j].code == _T("35E00A")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_31X;
			}
			// 第三種事業及び第五種事業
			else if (money[i][j].code == _T("35E00B")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_32X;
			}
			// 第三種事業及び第六種事業
			else if (money[i][j].code == _T("35E00C")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_33X;
			}
			// 第四種事業及び第五種事業
			else if (money[i][j].code == _T("35E00D")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_34X;
			}
			// 第四種事業及び第六種事業
			else if (money[i][j].code == _T("35E00E")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_35X;
			}
			// 第五種事業及び第六種事業
			else if (money[i][j].code == _T("35E00F")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5022_36X;
			}
			else continue;

			if (m_pArith->l_cmp(max_valttl, val) < 0) {
				memmove(max_valttl, val, MONY_BUF_SIZE);
				maxid = id;
				maxcode = money[i][j].code;
			}
		}
	}

	if (typeCnt > 1) {
		Idx = maxid;
		code = maxcode;
	}
	else {	// １業種では有利判定不要
		Idx = 0;
		code.Empty();
	}*/

	// 業種数
	if (reflg) {
		m_money35 = (*m_pSyzSyukei)->GetShinkokuData(_T("35"));
	}
	money = m_money35;

	int cnt = money.GetCount();
	int incnt = 0;

	for (int i = 0; i < cnt; i++) {
		incnt = money[i].GetCount();
		for (int j = 0; j < incnt; j++) {
			if (money[i][j].code == _T("35F000")) {
				m_pArith->l_print(buff, money[i][j].arith, _T("SSS"));
				typeCnt = atoi(buff);
			}
		}
	}

	int tkubun = 0;
	money = (*m_pSyzSyukei)->GetShinkokuData(_T("362006"));
	m_pArith->l_print(buff, money[0][0].arith, _T("SSSS"));
	tkubun = atoi(buff);
	switch (tkubun) {
	case 1:
		Idx = ID5022_21X;
		code = _T("35E000");
		break;
	case 2:
		Idx = ID5022_22X;
		code = _T("35E001");
		break;
	case 3:
		Idx = ID5022_23X;
		code = _T("35E002");
		break;
	case 4:
		Idx = ID5022_24X;
		code = _T("35E003");
		break;
	case 5:
		Idx = ID5022_25X;
		code = _T("35E004");
		break;
	case 6:
		Idx = ID5022_26X;
		code = _T("35E005");
		break;
	case 7:
		Idx = ID5022_27X;
		code = _T("35E006");
		break;
	case 8:
		Idx = ID5022_28X;
		code = _T("35E007");
		break;
	case 9:
		Idx = ID5022_29X;
		code = _T("35E008");
		break;
	case 10:
		Idx = ID5022_30X;
		code = _T("35E009");
		break;
	case 11:
		Idx = ID5022_31X;
		code = _T("35E00A");
		break;
	case 12:
		Idx = ID5022_32X;
		code = _T("35E00B");
		break;
	case 13:
		Idx = ID5022_33X;
		code = _T("35E00C");
		break;
	case 14:
		Idx = ID5022_34X;
		code = _T("35E00D");
		break;
	case 15:
		Idx = ID5022_35X;
		code = _T("35E00E");
		break;
	case 16:
		Idx = ID5022_36X;
		code = _T("35E00F");
		break;
	default:
		if (typeCnt >= 2) {
			Idx = ID5022_20X;
			code.Empty();
		}
		else {
			Idx = 0;
			code.Empty();
		}
	}

	F5022_21kbn = 0;
	//if (Idx == ID5022_21X) {
	if ( IsDispArea ( ID5022_21A ) ) {
		int minashi;
		money = (*m_pSyzSyukei)->GetShinkokuData(_T("362002"));
		m_pArith->l_print(buff, money[0][0].arith, _T("SSSS"));
		minashi = atoi(buff);
		switch (minashi) {
		case 9000:
			F5022_21kbn = 1;
			break;
		case 8000:
			F5022_21kbn = 2;
			break;
		case 7000:
			F5022_21kbn = 3;
			break;
		case 6000:
			F5022_21kbn = 4;
			break;
		case 5000:
			F5022_21kbn = 5;
			break;
		case 4000:
			F5022_21kbn = 6;
			break;
		default:
			F5022_21kbn = 0;
			break;
		}
	}

	m_PrintId = Idx;

	return 0;
}

//==============================================
// 合計欄の計算式を判別
//----------------------------------------------
// 返送値
//		0:横計
//		1:別計算（実額入力可）
//==============================================
int CShinFhyo5_022::Gokeiclctype()
{
	MoneyBasejagArray	money;
	money = (*m_pSyzSyukei)->GetShinkokuData( _T("360003") );

	if (m_pArith->l_test(money[0][0].arith))
		return 1;

	return 0;
}


//==============================================
// 控除対象仕入税額計算の為の消費税額判定
//----------------------------------------------
// 引数
//		reflg	:	(ON)再集計フラグ
//----------------------------------------------
// 返送値
//		0:全税率に金額なし
//		1:金額あり
//==============================================
int CShinFhyo5_022::Is4Val(int reflg)
{
	MoneyBasejagArray	money;
	char	val[MONY_BUF_SIZE],
			max_valttl[MONY_BUF_SIZE] = { 0 };

	if (reflg) {
		m_money35 = (*m_pSyzSyukei)->GetShinkokuData(_T("35"));
	}
	money = m_money35;

	int cnt = money.GetCount();
	int incnt = 0;

	for (int i = 0; i < cnt; i++) {
		incnt = money[i].GetCount();
		for (int j = 0; j < incnt; j++) {
			// 控除対象仕入税額計算の為の消費税額 3% 適用分
			if (money[i][j].code == _T("350401")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				if (m_pArith->l_test(val) != 0) {
					return 1;
				}
			}
			else if (money[i][j].code == _T("350402")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				if (m_pArith->l_test(val) != 0) {
					return 1;
				}
			}
			else if (money[i][j].code == _T("350403")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				if (m_pArith->l_test(val) != 0) {
					return 1;
				}
			}
			else if (money[i][j].code == _T("357402")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				if (m_pArith->l_test(val) != 0) {
					return 1;
				}
			}
			else if (money[i][j].code == _T("357403")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				if (m_pArith->l_test(val) != 0) {
					return 1;
				}
			}

		}
	}

	return 0;
}

//=========================================================
// 手入力項目あり？
//---------------------------------------------------------
// 返送値
//	TRUE		:		あり
//	FALSE		:		なし
//=========================================================
BOOL CShinFhyo5_022::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( ( F5022_20f & 0x8888 ) || ( F5022_21f & 0x8888 ) || ( F5022_22f & 0x8888 ) || ( F5022_23f & 0x8888 ) ||
			 ( F5022_24f & 0x8888 ) || ( F5022_25f & 0x8888 ) || ( F5022_26f & 0x8888 ) || ( F5022_27f & 0x8888 ) ||
			 ( F5022_28f & 0x8888 ) || ( F5022_29f & 0x8888 ) || ( F5022_30f & 0x8888 ) || ( F5022_31f & 0x8888 ) ||
			 ( F5022_32f & 0x8888 ) || ( F5022_33f & 0x8888 ) || ( F5022_34f & 0x8888 ) || ( F5022_35f & 0x8888 ) ||
			 ( F5022_36f & 0x8888 ) ) {
			ret = TRUE;
		}
	}

	return ret;
}

//=========================================================
// 表示項目？
//---------------------------------------------------------
// 引数
//			index		：	ダイアグラム内インデックス
//---------------------------------------------------------
// 返送値
//			0			：	非表示項目
//			1			：	表示項目
//			-1			：	エラー
//=========================================================
int CShinFhyo5_022::IsDispArea ( short index )
{
	int ret = 0;

	//int id1 = 0, id2 = 0;
	int sw = 0;		// 1 = 第一種
	int *pDispSign = NULL;
	bool getFlg = false;

	//unsigned short uWari = 0, tmpWari1 = 0, tmpWari2 = 0;

	//char tmpMony[MONY_BUF_SIZE] = { 0 }, tmpMony2[MONY_BUF_SIZE] = { 0 };
	//DIAGRAM_DATA DiagData;

	if( m_id == 0 ) {
		return 0;
	}

	switch ( index ) {
	case ID5022_21A:
	case ID5022_21B:
	case ID5022_21C:
	case ID5022_21X:
		if ( index == ID5022_21X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		sw = 1;
		pDispSign = &F5022_21Disp;
		getFlg = true;
		break;

	case ID5022_22A:
	case ID5022_22B:
	case ID5022_22C:
	case ID5022_22X:
		if ( index == ID5022_22X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAINISHU;
		pDispSign = &F5022_22Disp;
		getFlg = true;
		break;

	case ID5022_23A:
	case ID5022_23B:
	case ID5022_23C:
	case ID5022_23X:
		if ( index == ID5022_23X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAISANSHU;
		pDispSign = &F5022_23Disp;
		getFlg = true;
		break;

	case ID5022_24A:
	case ID5022_24B:
	case ID5022_24C:
	case ID5022_24X:
		if ( index == ID5022_24X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAIYONSHU;
		pDispSign = &F5022_24Disp;
		getFlg = true;
		break;

	case ID5022_25A:
	case ID5022_25B:
	case ID5022_25C:
	case ID5022_25X:
		if ( index == ID5022_25X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAIGOSHU;
		pDispSign = &F5022_25Disp;
		getFlg = true;
		break;

	case ID5022_26A:
	case ID5022_26B:
	case ID5022_26C:
	case ID5022_26X:
		if ( index == ID5022_26X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5022_26Disp;
		getFlg = true;
		break;

	case ID5022_27A:
	case ID5022_27B:
	case ID5022_27C:
	case ID5022_27X:
		if ( index == ID5022_27X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAINISHU;
		//id2 = DAISANSHU;
		pDispSign = &F5022_27Disp;
		getFlg = true;
		break;

	case ID5022_28A:
	case ID5022_28B:
	case ID5022_28C:
	case ID5022_28X:
		if ( index == ID5022_28X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAINISHU;
		//id2 = DAIYONSHU;
		pDispSign = &F5022_28Disp;
		getFlg = true;
		break;

	case ID5022_29A:
	case ID5022_29B:
	case ID5022_29C:
	case ID5022_29X:
		if ( index == ID5022_29X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAINISHU;
		//id2 = DAIGOSHU;
		pDispSign = &F5022_29Disp;
		getFlg = true;
		break;

	case ID5022_30A:
	case ID5022_30B:
	case ID5022_30C:
	case ID5022_30X:
		if ( index == ID5022_30X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAINISHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5022_30Disp;
		getFlg = true;
		break;

	case ID5022_31A:
	case ID5022_31B:
	case ID5022_31C:
	case ID5022_31X:
		if ( index == ID5022_31X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAISANSHU;
		//id2 = DAIYONSHU;
		pDispSign = &F5022_31Disp;
		getFlg = true;
		break;

	case ID5022_32A:
	case ID5022_32B:
	case ID5022_32C:
	case ID5022_32X:
		if ( index == ID5022_32X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAISANSHU;
		//id2 = DAIGOSHU;
		pDispSign = &F5022_32Disp;
		getFlg = true;
		break;

	case ID5022_33A:
	case ID5022_33B:
	case ID5022_33C:
	case ID5022_33X:
		if ( index == ID5022_33X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAISANSHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5022_33Disp;
		getFlg = true;
		break;

	case ID5022_34A:
	case ID5022_34B:
	case ID5022_34C:
	case ID5022_34X:
		if ( index == ID5022_34X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIYONSHU;
		//id2 = DAIGOSHU;
		pDispSign = &F5022_34Disp;
		getFlg = true;
		break;

	case ID5022_35A:
	case ID5022_35B:
	case ID5022_35C:
	case ID5022_35X:
		if ( index == ID5022_35X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIYONSHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5022_35Disp;
		getFlg = true;
		break;

	case ID5022_36A:
	case ID5022_36B:
	case ID5022_36C:
	case ID5022_36X:
		if ( index == ID5022_36X ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIGOSHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5022_36Disp;
		getFlg = true;
		break;

	default:
		break;
	}

	ret = 0;
	/*if ( sw ) {
		// 単一事業
		for ( int i = DAIICHISHU; i <= DAIROKUSHU; i++ ) {
			uWari = 0;
			memset( tmpMony, '\0', MONY_BUF_SIZE );
			GetUriageWariai2 ( i, &uWari, tmpMony );
			uWari /= 10;

			if ( uWari >= URIAGEWARIAI ) {
				ret = 1;
				break;
			}
		}
	}
	else if ( ( id1 != 0 ) && ( id2 != 0 ) ) {
		if( m_typecnt > 2 ) {
			// 複数事業
			tmpWari1 = 0;
			memset( tmpMony, '\0', MONY_BUF_SIZE );
			GetUriageWariai2 ( id1, &tmpWari1, tmpMony );

			tmpWari2 = 0;
			memset( tmpMony2, '\0', MONY_BUF_SIZE );
			GetUriageWariai2 ( id2, &tmpWari2, tmpMony2 );

			//if ( ( tmpWari1 != 0 ) && ( tmpWari2 != 0 ) ) {
			if( ( m_pArith->l_test( tmpMony ) > 0 ) && ( m_pArith->l_test( tmpMony2 ) > 0 ) ) {
				uWari = 0;
				uWari = tmpWari1 + tmpWari2;
				uWari /= 10;
				if ( uWari >= URIAGEWARIAI ) {
					ret = 1;
				}
			}
		}
	}*/
	if( getFlg ) {
		ASSERT( pDispSign );
		if( pDispSign ) {
			if( sw ) {
				// 単一業種
				if( m_typecnt > 1 ) {
					if( *pDispSign & 0x01 ) {
						ret = 1;
					}
				}
			}
			else {
				// 複数業種
				if( m_typecnt > 2 ) {
					if( *pDispSign & 0x01 ) {
						ret = 1;
					}
				}
			}
		}
	}

	return ret;
}

//=========================================================
// 手入力クリア
//---------------------------------------------------------
// 引数
//			index		：	ダイアグラム内インデックス
//=========================================================
void CShinFhyo5_022::ClearInput ( short index )
{
	int inputSign = 0;

	switch ( index ) {
	case ID5022_21A:
	case ID5022_21B:
	case ID5022_21C:
	case ID5022_21X:
		if ( index == ID5022_21A ) {
			F5022_21f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352101" ) );
		}
		else if ( index == ID5022_21B ) {
			F5022_21f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352102" ) );
		}
		else if ( index == ID5022_21C ) {
			F5022_21f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352103" ) );
		}
		else if ( index == ID5022_21X ) {
			F5022_21f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352100" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F521, inputSign );
		if ( !( F5022_21f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E000" ) );
		}
		break;

	case ID5022_22A:
	case ID5022_22B:
	case ID5022_22C:
	case ID5022_22X:
		if ( index == ID5022_22A ) {
			F5022_22f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352201" ) );
		}
		else if ( index == ID5022_22B ) {
			F5022_22f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352202" ) );
		}
		else if ( index == ID5022_22C ) {
			F5022_22f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352203" ) );
		}
		else if ( index == ID5022_22X ) {
			F5022_22f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352200" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F522, inputSign );
		if ( !( F5022_22f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E001" ) );
		}
		break;

	case ID5022_23A:
	case ID5022_23B:
	case ID5022_23C:
	case ID5022_23X:
		if ( index == ID5022_23A ) {
			F5022_23f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352301" ) );
		}
		else if ( index == ID5022_23B ) {
			F5022_23f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352302" ) );
		}
		else if ( index == ID5022_23C ) {
			F5022_23f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352303" ) );
		}
		else if ( index == ID5022_23X ) {
			F5022_23f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352300" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F523, inputSign );
		if ( !( F5022_23f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E002" ) );
		}
		break;

	case ID5022_24A:
	case ID5022_24B:
	case ID5022_24C:
	case ID5022_24X:
		if ( index == ID5022_24A ) {
			F5022_24f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352401" ) );
		}
		else if ( index == ID5022_24B ) {
			F5022_24f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352402" ) );
		}
		else if ( index == ID5022_24C ) {
			F5022_24f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352403" ) );
		}
		else if ( index == ID5022_24X ) {
			F5022_24f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352400" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F524, inputSign );
		if ( !( F5022_24f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E003" ) );
		}
		break;

	case ID5022_25A:
	case ID5022_25B:
	case ID5022_25C:
	case ID5022_25X:
		if ( index == ID5022_25A ) {
			F5022_25f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352501" ) );
		}
		else if ( index == ID5022_25B ) {
			F5022_25f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352502" ) );
		}
		else if ( index == ID5022_25C ) {
			F5022_25f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352503" ) );
		}
		else if ( index == ID5022_25X ) {
			F5022_25f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352500" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F525, inputSign );
		if ( !( F5022_25f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E004" ) );
		}
		break;

	case ID5022_26A:
	case ID5022_26B:
	case ID5022_26C:
	case ID5022_26X:
		if ( index == ID5022_26A ) {
			F5022_26f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352601" ) );
		}
		else if ( index == ID5022_26B ) {
			F5022_26f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352602" ) );
		}
		else if ( index == ID5022_26C ) {
			F5022_26f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352603" ) );
		}
		else if ( index == ID5022_26X ) {
			F5022_26f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352600" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F526, inputSign );
		if ( !( F5022_26f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E005" ) );
		}
		break;

	case ID5022_27A:
	case ID5022_27B:
	case ID5022_27C:
	case ID5022_27X:
		if ( index == ID5022_27A ) {
			F5022_27f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352701" ) );
		}
		else if ( index == ID5022_27B ) {
			F5022_27f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352702" ) );
		}
		else if ( index == ID5022_27C ) {
			F5022_27f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352703" ) );
		}
		else if ( index == ID5022_27X ) {
			F5022_27f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352700" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F527, inputSign );
		if ( !( F5022_27f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E006" ) );
		}
		break;

	case ID5022_28A:
	case ID5022_28B:
	case ID5022_28C:
	case ID5022_28X:
		if ( index == ID5022_28A ) {
			F5022_28f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352801" ) );
		}
		else if ( index == ID5022_28B ) {
			F5022_28f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352802" ) );
		}
		else if ( index == ID5022_28C ) {
			F5022_28f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352803" ) );
		}
		else if ( index == ID5022_28X ) {
			F5022_28f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352800" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F528, inputSign );
		if ( !( F5022_28f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E007" ) );
		}
		break;

	case ID5022_29A:
	case ID5022_29B:
	case ID5022_29C:
	case ID5022_29X:
		if ( index == ID5022_29A ) {
			F5022_29f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352901" ) );
		}
		else if ( index == ID5022_29B ) {
			F5022_29f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352902" ) );
		}
		else if ( index == ID5022_29C ) {
			F5022_29f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352903" ) );
		}
		else if ( index == ID5022_29X ) {
			F5022_29f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "352900" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F529, inputSign );
		if ( !( F5022_29f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E008" ) );
		}
		break;

	case ID5022_30A:
	case ID5022_30B:
	case ID5022_30C:
	case ID5022_30X:
		if ( index == ID5022_30A ) {
			F5022_30f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353001" ) );
		}
		else if ( index == ID5022_30B ) {
			F5022_30f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353002" ) );
		}
		else if ( index == ID5022_30C ) {
			F5022_30f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353003" ) );
		}
		else if ( index == ID5022_30X ) {
			F5022_30f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353000" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F530, inputSign );
		if ( !( F5022_30f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E009" ) );
		}
		break;

	case ID5022_31A:
	case ID5022_31B:
	case ID5022_31C:
	case ID5022_31X:
		if ( index == ID5022_31A ) {
			F5022_31f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353101" ) );
		}
		else if ( index == ID5022_31B ) {
			F5022_31f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353102" ) );
		}
		else if ( index == ID5022_31C ) {
			F5022_31f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353103" ) );
		}
		else if ( index == ID5022_31X ) {
			F5022_31f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353100" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F531, inputSign );
		if ( !( F5022_31f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00A" ) );
		}
		break;

	case ID5022_32A:
	case ID5022_32B:
	case ID5022_32C:
	case ID5022_32X:
		if ( index == ID5022_32A ) {
			F5022_32f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353201" ) );
		}
		else if ( index == ID5022_32B ) {
			F5022_32f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353202" ) );
		}
		else if ( index == ID5022_32C ) {
			F5022_32f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353203" ) );
		}
		else if ( index == ID5022_32X ) {
			F5022_32f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353200" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F532, inputSign );
		if ( !( F5022_32f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00B" ) );
		}
		break;

	case ID5022_33A:
	case ID5022_33B:
	case ID5022_33C:
	case ID5022_33X:
		if ( index == ID5022_33A ) {
			F5022_33f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353301" ) );
		}
		else if ( index == ID5022_33B ) {
			F5022_33f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353302" ) );
		}
		else if ( index == ID5022_33C ) {
			F5022_33f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353303" ) );
		}
		else if ( index == ID5022_33X ) {
			F5022_33f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353300" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F533, inputSign );
		if ( !( F5022_33f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00C" ) );
		}
		break;

	case ID5022_34A:
	case ID5022_34B:
	case ID5022_34C:
	case ID5022_34X:
		if ( index == ID5022_34A ) {
			F5022_34f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353401" ) );
		}
		else if ( index == ID5022_34B ) {
			F5022_34f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353402" ) );
		}
		else if ( index == ID5022_34C ) {
			F5022_34f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353403" ) );
		}
		else if ( index == ID5022_34X ) {
			F5022_34f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353400" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F534, inputSign );
		if ( !( F5022_34f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00D" ) );
		}
		break;

	case ID5022_35A:
	case ID5022_35B:
	case ID5022_35C:
	case ID5022_35X:
		if ( index == ID5022_35A ) {
			F5022_35f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353501" ) );
		}
		else if ( index == ID5022_35B ) {
			F5022_35f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353502" ) );
		}
		else if ( index == ID5022_35C ) {
			F5022_35f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353503" ) );
		}
		else if ( index == ID5022_35X ) {
			F5022_35f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353500" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F535, inputSign );
		if ( !( F5022_35f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00E" ) );
		}
		break;

	case ID5022_36A:
	case ID5022_36B:
	case ID5022_36C:
	case ID5022_36X:
		if ( index == ID5022_36A ) {
			F5022_36f &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353601" ) );
		}
		else if ( index == ID5022_36B ) {
			F5022_36f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353602" ) );
		}
		else if ( index == ID5022_36C ) {
			F5022_36f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353603" ) );
		}
		else if ( index == ID5022_36X ) {
			F5022_36f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "353600" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F536, inputSign );
		if ( !( F5022_36f & 0x8888 ) && !( inputSign & 0x8880 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00F" ) );
		}
		break;

	default :
		break;
	}
}

//=========================================================
// 合計欄の計算
//---------------------------------------------------------
// 引数
//			idx			：	ダイアグラム内インデックス
//---------------------------------------------------------
// 返送値
//			0			:	正常終了
//=========================================================
int CShinFhyo5_022::CalqGokei ( int index )
{
	if( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	char tmpMony[MONY_BUF_SIZE] = { 0 };
	CString code, setCode;
	MoneyBasejagArray money;

	int inputSign = 0;

	memset ( tmpMony, '\0', sizeof ( tmpMony ) );
	switch ( index ) {
	case ID5022_21A:
	case ID5022_21B:
	case ID5022_21C:
	case ID5022_21X:
		inputSign = 0;
		GetInputSign ( ID_F521, inputSign );
		if ( ( F5022_21f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "359100" );
			setCode = _T( "35E000" );
		}
		break;

	case ID5022_22A:
	case ID5022_22B:
	case ID5022_22C:
	case ID5022_22X:
		inputSign = 0;
		GetInputSign ( ID_F522, inputSign );
		if ( ( F5022_22f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "359200" );
			setCode = _T( "35E001" );
		}
		break;

	case ID5022_23A:
	case ID5022_23B:
	case ID5022_23C:
	case ID5022_23X:
		inputSign = 0;
		GetInputSign ( ID_F523, inputSign );
		if ( ( F5022_23f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "359300" );
			setCode = _T( "35E002" );
		}
		break;

	case ID5022_24A:
	case ID5022_24B:
	case ID5022_24C:
	case ID5022_24X:
		inputSign = 0;
		GetInputSign ( ID_F524, inputSign );
		if ( ( F5022_24f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "359400" );
			setCode = _T( "35E003" );
		}
		break;

	case ID5022_25A:
	case ID5022_25B:
	case ID5022_25C:
	case ID5022_25X:
		inputSign = 0;
		GetInputSign ( ID_F525, inputSign );
		if ( ( F5022_25f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "359500" );
			setCode = _T( "35E004" );
		}
		break;

	case ID5022_26A:
	case ID5022_26B:
	case ID5022_26C:
	case ID5022_26X:
		inputSign = 0;
		GetInputSign ( ID_F526, inputSign );
		if ( ( F5022_26f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "359600" );
			setCode = _T( "35E005" );
		}
		break;

	case ID5022_27A:
	case ID5022_27B:
	case ID5022_27C:
	case ID5022_27X:
		inputSign = 0;
		GetInputSign ( ID_F527, inputSign );
		if ( ( F5022_27f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "359700" );
			setCode = _T( "35E006" );
		}
		break;

	case ID5022_28A:
	case ID5022_28B:
	case ID5022_28C:
	case ID5022_28X:
		inputSign = 0;
		GetInputSign ( ID_F528, inputSign );
		if ( ( F5022_28f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "359800" );
			setCode = _T( "35E007" );
		}
		break;

	case ID5022_29A:
	case ID5022_29B:
	case ID5022_29C:
	case ID5022_29X:
		inputSign = 0;
		GetInputSign ( ID_F529, inputSign );
		if ( ( F5022_29f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "359900" );
			setCode = _T( "35E008" );
		}
		break;

	case ID5022_30A:
	case ID5022_30B:
	case ID5022_30C:
	case ID5022_30X:
		inputSign = 0;
		GetInputSign ( ID_F530, inputSign );
		if ( ( F5022_30f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "35A000" );
			setCode = _T( "35E009" );
		}
		break;

	case ID5022_31A:
	case ID5022_31B:
	case ID5022_31C:
	case ID5022_31X:
		inputSign = 0;
		GetInputSign ( ID_F531, inputSign );
		if ( ( F5022_31f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "35A100" );
			setCode = _T( "35E00A" );
		}
		break;

	case ID5022_32A:
	case ID5022_32B:
	case ID5022_32C:
	case ID5022_32X:
		inputSign = 0;
		GetInputSign ( ID_F532, inputSign );
		if ( ( F5022_32f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "35A200" );
			setCode = _T( "35E00B" );
		}
		break;

	case ID5022_33A:
	case ID5022_33B:
	case ID5022_33C:
	case ID5022_33X:
		inputSign = 0;
		GetInputSign ( ID_F533, inputSign );
		if ( ( F5022_33f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "35A300" );
			setCode = _T( "35E00C" );
		}
		break;

	case ID5022_34A:
	case ID5022_34B:
	case ID5022_34C:
	case ID5022_34X:
		inputSign = 0;
		GetInputSign ( ID_F534, inputSign );
		if ( ( F5022_34f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "35A400" );
			setCode = _T( "35E00D" );
		}
		break;

	case ID5022_35A:
	case ID5022_35B:
	case ID5022_35C:
	case ID5022_35X:
		inputSign = 0;
		GetInputSign ( ID_F535, inputSign );
		if ( ( F5022_35f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "35A500" );
			setCode = _T( "35E00E" );
		}
		break;

	case ID5022_36A:
	case ID5022_36B:
	case ID5022_36C:
	case ID5022_36X:
		inputSign = 0;
		GetInputSign ( ID_F536, inputSign );
		if ( ( F5022_36f & 0x8888 ) || ( inputSign & 0x8880 ) ) {
			code = _T( "35A600" );
			setCode = _T( "35E00F" );
		}
		break;

	default:
		break;
	}

	if ( ( !code.IsEmpty() ) && ( !setCode.IsEmpty() ) ) {
		money = ( *m_pSyzSyukei )->GetShinkokuData ( code );
		memmove ( tmpMony, money[0][0].arith, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( setCode, tmpMony );
	}

	return 0;
}

//=========================================================
// 売上割合の取得
//---------------------------------------------------------
// 引数
//			id			：	取得する売上割合の事業種
//			pWari		：	売上割合
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//			-1			：	エラー
//=========================================================
int CShinFhyo5_022::GetUriageWariai ( int id, unsigned short *pWari )
{
	ASSERT ( pWari );
	if ( pWari == NULL ) {
		return -1;
	}

	int ret = 0;
	CString tag, tmpTag;

	tag.Empty();
	switch ( id ) {
	case DAIICHISHU:
		tag = _T( "CTD00130" );
		break;
	case DAINISHU:
		tag = _T( "CTD00200" );
		break;
	case DAISANSHU:
		tag = _T( "CTD00270" );
		break;
	case DAIYONSHU:
		tag = _T( "CTD00340" );
		break;
	case DAIGOSHU:
		tag = _T( "CTD00410" );
		break;
	case DAIROKUSHU:
		tag = _T( "CTD00480" );
		break;
	default:
		break;
	}

	*pWari = 0;
	if ( !tag.IsEmpty() ) {
		tmpTag.Empty();
		( ( CH31HyoView* )m_pParent )->GetPercentage ( tag, tmpTag, pWari );
		ret = 0;
	}
	else {
		ret = -1;
	}

	return ret;
}

//=========================================================
// 売上割合の取得
//---------------------------------------------------------
// 引数
//			id			：	取得する売上割合の事業種
//			pWari		：	売上割合
//			pArith		:	合計金額
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//			-1			：	エラー
//=========================================================
int CShinFhyo5_022::GetUriageWariai2 ( int id, unsigned short *pWari, char *pArith )
{
	ASSERT ( pWari );
	ASSERT ( pArith );
	if ( ( pWari == NULL ) || ( pArith == NULL ) ) {
		return -1;
	}

	int ret = 0;

	*pWari = 0;
	memset( pArith, '\0', MONY_BUF_SIZE );
	ret = ( ( CH31HyoView* )m_pParent )->GetPercentage2( id, pWari, pArith );

	return ret;
}

//=========================================================
// 出力する控除対象仕入税額をセット
//---------------------------------------------------------
// 返送値
//			0			:		正常終了
//			-1			:		エラー
//=========================================================
int CShinFhyo5_022::SetPrintArea()
{
	int ret = 0;
	int index = 0;
	bool errFlg = false;

	if ( !( ( *m_pSnHeadData )->Sn_GenericSgn & 0x40 ) ) {
		return 0;
	}

	// 一度リセットしておく
	AllPrint();

	// 入力ロック中等、m_idがクリアされていることがあったため、別変数を用意
	//switch ( m_id ) {
	switch ( m_PrintId ) {
	case ID5022_20X:
		F5022_20f |= 0x01;
		break;
	case ID5022_21X:
		F5022_21f |= 0x01;
		break;
	case ID5022_22X:
		F5022_22f |= 0x01;
		break;
	case ID5022_23X:
		F5022_23f |= 0x01;
		break;
	case ID5022_24X:
		F5022_24f |= 0x01;
		break;
	case ID5022_25X:
		F5022_25f |= 0x01;
		break;
	case ID5022_26X:
		F5022_26f |= 0x01;
		break;
	case ID5022_27X:
		F5022_27f |= 0x01;
		break;
	case ID5022_28X:
		F5022_28f |= 0x01;
		break;
	case ID5022_29X:
		F5022_29f |= 0x01;
		break;
	case ID5022_30X:
		F5022_30f |= 0x01;
		break;
	case ID5022_31X:
		F5022_31f |= 0x01;
		break;
	case ID5022_32X:
		F5022_32f |= 0x01;
		break;
	case ID5022_33X:
		F5022_33f |= 0x01;
		break;
	case ID5022_34X:
		F5022_34f |= 0x01;
		break;
	case ID5022_35X:
		F5022_35f |= 0x01;
		break;
	case ID5022_36X:
		F5022_36f |= 0x01;
		break;
	default:
		errFlg = true;
		break;
	}

	if ( errFlg ) {
		return -1;
	}

	return 0;
}

//=========================================================
// 全項目出力
//=========================================================
void CShinFhyo5_022::AllPrint()
{
	F5022_20f &= ~0x01;
	F5022_21f &= ~0x01;
	F5022_22f &= ~0x01;
	F5022_23f &= ~0x01;
	F5022_24f &= ~0x01;
	F5022_25f &= ~0x01;
	F5022_26f &= ~0x01;
	F5022_27f &= ~0x01;
	F5022_28f &= ~0x01;
	F5022_29f &= ~0x01;
	F5022_30f &= ~0x01;
	F5022_31f &= ~0x01;
	F5022_32f &= ~0x01;
	F5022_33f &= ~0x01;
	F5022_34f &= ~0x01;
	F5022_35f &= ~0x01;
	F5022_36f &= ~0x01;
}

//=========================================================
// 入力項目？
//---------------------------------------------------------
// 返送値
//			0			:		非入力項目
//			1			:		入力項目
//			-1			:		エラー
//=========================================================
int CShinFhyo5_022::IsInputArea ( short index )
{
	int ret = 0;

	// 単一業種の場合は不可
	if ( ( m_typecnt < 2 ) || ( m_4val == 0 ) ) {
		ret = 0;
	}
	else {
		if ( IsDispArea ( index ) == 1 ) {
			ret = 1;
		}
		else {
			ret = 0;
		}
	}

	return ret;
}

//=========================================================
// 付表5-1(2) 手入力サイン取得
//---------------------------------------------------------
// 引数
//			id			:		ID
//			inputSign	:		取得した手入力サイン
//---------------------------------------------------------
// 返送値
//			0			:		正常終了
//			-1			:		エラー
//=========================================================
int CShinFhyo5_022::GetInputSign ( int id, int &inputSign )
{
	int ret = 0;

	inputSign = 0;
	ret = ( ( CH31HyoView* )m_pParent )->GetInputSign ( FHYO5_1, id, inputSign );

	return ret;
}

//====================================================================
// 付表5 手入力サイン取得
//--------------------------------------------------------------------
// 引数
//		type		:		0 = 付表5-1を取得, 1 = 付表5-2を取得
//		id			:		項目ID
//		inputSign	:		取得した手入力サイン
//--------------------------------------------------------------------
// 返送値
//		0			:		正常終了
//		-1			:		エラー
//====================================================================
int CShinFhyo5_022::GetInputSign_F522 ( int id, int &inputSign )
{
	int ret = 0;
	int tmpSign = 0;
	bool errFlg = false;

	inputSign = 0;

	switch ( id ) {
	case ID_F521:
		tmpSign = F5022_21f;
		break;
	case ID_F522:
		tmpSign = F5022_22f;
		break;
	case ID_F523:
		tmpSign = F5022_23f;
		break;
	case ID_F524:
		tmpSign = F5022_24f;
		break;
	case ID_F525:
		tmpSign = F5022_25f;
		break;
	case ID_F526:
		tmpSign = F5022_26f;
		break;
	case ID_F527:
		tmpSign = F5022_27f;
		break;
	case ID_F528:
		tmpSign = F5022_28f;
		break;
	case ID_F529:
		tmpSign = F5022_29f;
		break;
	case ID_F530:
		tmpSign = F5022_30f;
		break;
	case ID_F531:
		tmpSign = F5022_31f;
		break;
	case ID_F532:
		tmpSign = F5022_32f;
		break;
	case ID_F533:
		tmpSign = F5022_33f;
		break;
	case ID_F534:
		tmpSign = F5022_34f;
		break;
	case ID_F535:
		tmpSign = F5022_35f;
		break;
	case ID_F536:
		tmpSign = F5022_36f;
		break;
	default:
		errFlg = true;
		break;
	}

	if ( errFlg ) {
		ret = -1;
	}
	else {
		inputSign = tmpSign;
	}

	return ret;
}

//=========================================================
// 特例計算取得
//---------------------------------------------------------
// 返送値
//		0			:			正常終了
//=========================================================
int CShinFhyo5_022::GetTokurei()
{
	MoneyBasejagArray money;
	int count = 0, inCount = 0;
	char tmpMony[MONY_BUF_SIZE] = { 0 };
	int *pDispSign = NULL;
	bool bGet = false;

	if( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		money = ( *m_pSyzSyukei )->GetShinkokuData( _T( "35" ) );

		count = ( int )money.GetCount();
		for( int i = 0; i < count; i++ ) {
			inCount = ( int )money[i].GetCount();
			for( int j = 0; j < inCount; j++ ) {
				memset( tmpMony, '\0', MONY_BUF_SIZE );
				pDispSign = NULL;
				bGet = false;

				if( money[i][j].code == _T( "35E200" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_21Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E201" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_22Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E202" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_23Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E203" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_24Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E204" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_25Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E205" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_26Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E206" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_27Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E207" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_28Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E208" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_29Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E209" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_30Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20A" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_31Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20B" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_32Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20C" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_33Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20D" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_34Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20E" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_35Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20F" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5022_36Disp;
					bGet = true;
				}

				if( bGet ) {
					ASSERT( pDispSign );
					if( pDispSign ) {
						if( m_pArith->l_test( tmpMony ) > 0 ) {
							*pDispSign |= 0x01;
						}
						else {
							*pDispSign &= ~0x01;
						}
					}
				}
			}
		}
	}
	else {
		// 入力ロック中は全表示
		F5022_21Disp |= 0x01;
		F5022_22Disp |= 0x01;
		F5022_23Disp |= 0x01;
		F5022_24Disp |= 0x01;
		F5022_25Disp |= 0x01;
		F5022_26Disp |= 0x01;
		F5022_27Disp |= 0x01;
		F5022_28Disp |= 0x01;
		F5022_29Disp |= 0x01;
		F5022_30Disp |= 0x01;
		F5022_31Disp |= 0x01;
		F5022_32Disp |= 0x01;
		F5022_33Disp |= 0x01;
		F5022_34Disp |= 0x01;
		F5022_35Disp |= 0x01;
		F5022_36Disp |= 0x01;
	}

	return 0;
}

/*// 付表5 控除対象仕入税額の選択をICSP連携専用の機能とするため一時的にカット
//=========================================================
// 選択された項目の控除対象仕入税額をセット
//---------------------------------------------------------
// 引数
//			selId		：	選択された項目のID
//=========================================================
void CShinFhyo5_022::SetSelectCode ( int  selId )
{
	bool errFlg = false;

	switch ( selId ) {
	case ID_F520:
		F5022_20f |= 0x02;
		break;
	case ID_F521:
		F5022_21f |= 0x02;
		break;
	case ID_F522:
		F5022_22f |= 0x02;
		break;
	case ID_F523:
		F5022_23f |= 0x02;
		break;
	case ID_F524:
		F5022_24f |= 0x02;
		break;
	case ID_F525:
		F5022_25f |= 0x02;
		break;
	case ID_F526:
		F5022_26f |= 0x02;
		break;
	case ID_F527:
		F5022_27f |= 0x02;
		break;
	case ID_F528:
		F5022_28f |= 0x02;
		break;
	case ID_F529:
		F5022_29f |= 0x02;
		break;
	case ID_F530:
		F5022_30f |= 0x02;
		break;
	case ID_F531:
		F5022_31f |= 0x02;
		break;
	case ID_F532:
		F5022_32f |= 0x02;
		break;
	case ID_F533:
		F5022_33f |= 0x02;
		break;
	case ID_F534:
		F5022_34f |= 0x02;
		break;
	case ID_F535:
		F5022_35f |= 0x02;
		break;
	case ID_F536:
		F5022_36f |= 0x02;
		break;
	default:
		errFlg = true;
		break;
	}

	if ( errFlg ) {
		return;
	}

	if ( ChangeBackColor ( TRUE ) == 0 ) {
		m_Fhyo5_022Diag01.ModifyItem ( SELECTITEM, DIAG_MDFY_OPAQUE );
		m_Fhyo5_022Diag01.Refresh();
	}

	// 集計モジュールへのフラグセットは付表5-1(2)で行うため、付表5-2(2)では行わない
}

//=========================================================
// 控除対象仕入税額の自動集計
//=========================================================
void CShinFhyo5_022::AutoKjCode()
{
	ChangeBackColor ( FALSE );
	m_Fhyo5_022Diag01.ModifyItem ( SELECTITEM, DIAG_MDFY_TRANSPARENT );
	m_Fhyo5_022Diag01.Refresh();

	F5022_20f &= ~0x02;
	F5022_21f &= ~0x02;
	F5022_22f &= ~0x02;
	F5022_23f &= ~0x02;
	F5022_24f &= ~0x02;
	F5022_25f &= ~0x02;
	F5022_26f &= ~0x02;
	F5022_27f &= ~0x02;
	F5022_28f &= ~0x02;
	F5022_29f &= ~0x02;
	F5022_30f &= ~0x02;
	F5022_31f &= ~0x02;
	F5022_32f &= ~0x02;
	F5022_33f &= ~0x02;
	F5022_34f &= ~0x02;
	F5022_35f &= ~0x02;
	F5022_36f &= ~0x02;
}

//=========================================================
// 選択された項目の背景色変更
//---------------------------------------------------------
// 引数
//			bSw		:		TRUE = ON, FALSE = OFF
//---------------------------------------------------------
// 返送値
//			0		:		正常終了
//			-1		:		エラー
//=========================================================
int CShinFhyo5_022::ChangeBackColor ( BOOL bSw )
{
	int s_idx = 0, e_idx;
	int colNo = 0;

	if ( F5022_20f & 0x02 ) {
		s_idx = ID5022_NO20_S;
		e_idx = ID5022_NO20_E;
	}
	else if ( F5022_21f & 0x02 ) {
		s_idx = ID5022_NO21_S;
		e_idx = ID5022_NO21_E;
	}
	else if ( F5022_22f & 0x02 ) {
		s_idx = ID5022_NO22_S;
		e_idx = ID5022_NO22_E;
	}
	else if ( F5022_23f & 0x02 ) {
		s_idx = ID5022_NO23_S;
		e_idx = ID5022_NO23_E;
	}
	else if ( F5022_24f & 0x02 ) {
		s_idx = ID5022_NO24_S;
		e_idx = ID5022_NO24_E;
	}
	else if ( F5022_25f & 0x02 ) {
		s_idx = ID5022_NO25_S;
		e_idx = ID5022_NO25_E;
	}
	else if ( F5022_26f & 0x02 ) {
		s_idx = ID5022_NO26_S;
		e_idx = ID5022_NO26_E;
	}
	else if ( F5022_27f & 0x02 ) {
		s_idx = ID5022_NO27_S;
		e_idx = ID5022_NO27_E;
	}
	else if ( F5022_28f & 0x02 ) {
		s_idx = ID5022_NO28_S;
		e_idx = ID5022_NO28_E;
	}
	else if ( F5022_29f & 0x02 ) {
		s_idx = ID5022_NO29_S;
		e_idx = ID5022_NO29_E;
	}
	else if ( F5022_30f & 0x02 ) {
		s_idx = ID5022_NO30_S;
		e_idx = ID5022_NO30_E;
	}
	else if ( F5022_31f & 0x02 ) {
		s_idx = ID5022_NO31_S;
		e_idx = ID5022_NO31_E;
	}
	else if ( F5022_32f & 0x02 ) {
		s_idx = ID5022_NO32_S;
		e_idx = ID5022_NO32_E;
	}
	else if ( F5022_33f & 0x02 ) {
		s_idx = ID5022_NO33_S;
		e_idx = ID5022_NO33_E;
	}
	else if ( F5022_34f & 0x02 ) {
		s_idx = ID5022_NO34_S;
		e_idx = ID5022_NO34_E;
	}
	else if ( F5022_35f & 0x02 ) {
		s_idx = ID5022_NO35_S;
		e_idx = ID5022_NO35_E;
	}
	else if ( F5022_36f & 0x02 ) {
		s_idx = ID5022_NO36_S;
		e_idx = ID5022_NO36_E;
	}
	else {
		;
	}

	if ( ( s_idx != 0 ) && ( e_idx != 0 ) ) {
		if ( bSw ) {
			colNo = 4;
		}
		else {
			colNo = 1;
		}

		for ( int i = s_idx; i <= e_idx; i++ ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, i, colNo );
		}
	}
	else {
		return -1;
	}

	return 0;
}

//=========================================================
// 控除対象仕入税額の再セット必要
//---------------------------------------------------------
// 引数
//		bRet		:		true  = 再セット必要
//							false = 不必要
//=========================================================
bool CShinFhyo5_022::IsKjCodeReset()
{
	bool bRet = false;
	int id = 0, st = 0;
	if ( ( *m_pSnHeadData )->Sn_GenericSgn & 0x08 ) {
		st = GetSelectItem ( id );
		if ( ( st != -1 ) && ( id != 0 ) ) {
			if ( IsDispArea ( id ) != 1 ) {
				bRet = true;
			}
		}
	}

	return bRet;
}

//=========================================================
// 選択中の項目取得
//---------------------------------------------------------
// 引数
//			getID		:		選択中の項目のA欄インデックス
//---------------------------------------------------------
// 返送値
//			0			:		正常終了
//			-1			:		エラー
//=========================================================
int CShinFhyo5_022::GetSelectItem ( int &getID )
{
	int id = 0;

	if ( F5022_20f & 0x01 ) {
		id = ID5022_20A;
	}
	else if ( F5022_21f & 0x02 ) {
		id = ID5022_21A;
	}
	else if ( F5022_22f & 0x02 ) {
		id = ID5022_22A;
	}
	else if ( F5022_23f & 0x02 ) {
		id = ID5022_23A;
	}
	else if ( F5022_24f & 0x02 ) {
		id = ID5022_24A;
	}
	else if ( F5022_25f & 0x02 ) {
		id = ID5022_25A;
	}
	else if ( F5022_26f & 0x02 ) {
		id = ID5022_26A;
	}
	else if ( F5022_27f & 0x02 ) {
		id = ID5022_27A;
	}
	else if ( F5022_28f & 0x02 ) {
		id = ID5022_28A;
	}
	else if ( F5022_29f & 0x02 ) {
		id = ID5022_29A;
	}
	else if ( F5022_30f & 0x02 ) {
		id = ID5022_30A;
	}
	else if ( F5022_31f & 0x02 ) {
		id = ID5022_31A;
	}
	else if ( F5022_32f & 0x02 ) {
		id = ID5022_32A;
	}
	else if ( F5022_33f & 0x02 ) {
		id = ID5022_33A;
	}
	else if ( F5022_34f & 0x02 ) {
		id = ID5022_34A;
	}
	else if ( F5022_35f & 0x02 ) {
		id = ID5022_35A;
	}
	else if ( F5022_36f & 0x02 ) {
		id = ID5022_36A;
	}
	else {
		id = -1;
	}

	getID = 0;
	if ( id != -1 ) {
		getID = id;
	}
	else {
		AutoKjCode();
	}

	return 0;
}

//=========================================================
// 再集計
//=========================================================
void CShinFhyo5_022::ReSyukei()
{
	MemoryReget();
	Disp_f5022data ( 0 );
}*/