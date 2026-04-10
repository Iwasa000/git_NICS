// ShinFhyo5_012.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo5_012.h"
#include "ShinFhyo5_012idx.h"
#include "H31HyoView.h"

// CShinFhyo5_012 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo5_012, CSyzBaseDlg)

CShinFhyo5_012::CShinFhyo5_012(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo5_012::IDD, pParent)
	, m_pParent ( pParent )
	, m_pSnHeadData ( NULL )
	, m_pSyzSyukei( NULL )
{

	memset(F5012_20X, '\0', sizeof(F5012_20X));
	memset(F5012_20D, '\0', sizeof(F5012_20D));
	memset(F5012_20E, '\0', sizeof(F5012_20E));
	memset(F5012_20F, '\0', sizeof(F5012_20F));

	memset(F5012_21X, '\0', sizeof(F5012_21X));
	memset(F5012_21D, '\0', sizeof(F5012_21D));
	memset(F5012_21E, '\0', sizeof(F5012_21E));
	memset(F5012_21F, '\0', sizeof(F5012_21F));

	memset(F5012_22X, '\0', sizeof(F5012_22X));
	memset(F5012_22D, '\0', sizeof(F5012_22D));
	memset(F5012_22E, '\0', sizeof(F5012_22E));
	memset(F5012_22F, '\0', sizeof(F5012_22F));

	memset(F5012_23X, '\0', sizeof(F5012_23X));
	memset(F5012_23D, '\0', sizeof(F5012_23D));
	memset(F5012_23E, '\0', sizeof(F5012_23E));
	memset(F5012_23F, '\0', sizeof(F5012_23F));

	memset(F5012_24X, '\0', sizeof(F5012_24X));
	memset(F5012_24D, '\0', sizeof(F5012_24D));
	memset(F5012_24E, '\0', sizeof(F5012_24E));
	memset(F5012_24F, '\0', sizeof(F5012_24F));

	memset(F5012_25X, '\0', sizeof(F5012_25X));
	memset(F5012_25D, '\0', sizeof(F5012_25D));
	memset(F5012_25E, '\0', sizeof(F5012_25E));
	memset(F5012_25F, '\0', sizeof(F5012_25F));

	memset(F5012_26X, '\0', sizeof(F5012_26X));
	memset(F5012_26D, '\0', sizeof(F5012_26D));
	memset(F5012_26E, '\0', sizeof(F5012_26E));
	memset(F5012_26F, '\0', sizeof(F5012_26F));

	memset(F5012_27X, '\0', sizeof(F5012_27X));
	memset(F5012_27D, '\0', sizeof(F5012_27D));
	memset(F5012_27E, '\0', sizeof(F5012_27E));
	memset(F5012_27F, '\0', sizeof(F5012_27F));

	memset(F5012_28X, '\0', sizeof(F5012_28X));
	memset(F5012_28D, '\0', sizeof(F5012_28D));
	memset(F5012_28E, '\0', sizeof(F5012_28E));
	memset(F5012_28F, '\0', sizeof(F5012_28F));

	memset(F5012_29X, '\0', sizeof(F5012_29X));
	memset(F5012_29D, '\0', sizeof(F5012_29D));
	memset(F5012_29E, '\0', sizeof(F5012_29E));
	memset(F5012_29F, '\0', sizeof(F5012_29F));

	memset(F5012_30X, '\0', sizeof(F5012_30X));
	memset(F5012_30D, '\0', sizeof(F5012_30D));
	memset(F5012_30E, '\0', sizeof(F5012_30E));
	memset(F5012_30F, '\0', sizeof(F5012_30F));

	memset(F5012_31X, '\0', sizeof(F5012_31X));
	memset(F5012_31D, '\0', sizeof(F5012_31D));
	memset(F5012_31E, '\0', sizeof(F5012_31E));
	memset(F5012_31F, '\0', sizeof(F5012_31F));

	memset(F5012_32X, '\0', sizeof(F5012_32X));
	memset(F5012_32D, '\0', sizeof(F5012_32D));
	memset(F5012_32E, '\0', sizeof(F5012_32E));
	memset(F5012_32F, '\0', sizeof(F5012_32F));

	memset(F5012_33X, '\0', sizeof(F5012_33X));
	memset(F5012_33D, '\0', sizeof(F5012_33D));
	memset(F5012_33E, '\0', sizeof(F5012_33E));
	memset(F5012_33F, '\0', sizeof(F5012_33F));

	memset(F5012_34X, '\0', sizeof(F5012_34X));
	memset(F5012_34D, '\0', sizeof(F5012_34D));
	memset(F5012_34E, '\0', sizeof(F5012_34E));
	memset(F5012_34F, '\0', sizeof(F5012_34F));

	memset(F5012_35X, '\0', sizeof(F5012_35X));
	memset(F5012_35D, '\0', sizeof(F5012_35D));
	memset(F5012_35E, '\0', sizeof(F5012_35E));
	memset(F5012_35F, '\0', sizeof(F5012_35F));

	memset(F5012_36X, '\0', sizeof(F5012_36X));
	memset(F5012_36D, '\0', sizeof(F5012_36D));
	memset(F5012_36E, '\0', sizeof(F5012_36E));
	memset(F5012_36F, '\0', sizeof(F5012_36F));

	memset(F5012_37X, '\0', sizeof(F5012_37X));
	memset(F5012_37D, '\0', sizeof(F5012_37D));
	memset(F5012_37E, '\0', sizeof(F5012_37E));
	memset(F5012_37F, '\0', sizeof(F5012_37F));
	F5012_21kbn = 0;

	F5012_20f = F5012_21f = F5012_22f = F5012_23f = F5012_24f = F5012_25f = F5012_26f = F5012_27f = F5012_28f = F5012_29f = F5012_30f = F5012_31f = F5012_32f = F5012_32f = F5012_33f = F5012_34f = F5012_35f = F5012_36f = 0;

	F5012_21Disp = F5012_22Disp = F5012_23Disp = F5012_24Disp = F5012_25Disp = F5012_26Disp = F5012_27Disp = F5012_28Disp = F5012_29Disp = 0;
	F5012_30Disp = F5012_31Disp = F5012_32Disp = F5012_33Disp = F5012_34Disp = F5012_35Disp = F5012_36Disp = 0;

	m_PrintId = 0;
	m_curwnd = -1;
	m_flg = 0;
}

CShinFhyo5_012::~CShinFhyo5_012()
{
}

void CShinFhyo5_012::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo5_012Diag01);
}


BEGIN_MESSAGE_MAP(CShinFhyo5_012, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CShinFhyo5_012 メッセージ ハンドラ

BOOL CShinFhyo5_012::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL1);
	m_BottomIdx = 307;//コントロール一番最下のDiagのID指定。これで縦スクロール出る

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo5_012::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo5_012::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo5_012::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
int CShinFhyo5_012::Init()
{
	// データ読み込み
	ReadData();

	// 読み込みデータを集計モジュールにセット
	ReadData2();
	MemoryReget();

	// ダイアグラム初期属性セット
	InitDiagAttr();

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		m_Fhyo5_012Diag01.ModifyItem ( 308, DIAG_MDFY_TRANSPARENT );
	}

	// 画面表示
	Disp_f5012();

	// 初期セットポジション
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
int CShinFhyo5_012::InitInfo(CH31SnFhyo5_012Data **pSnFhyo5_012Data, CSnHeadData **pSnHeadData, CSyzSyukei **pSyzSyukei, CArithEx *pArith)
{
	if ( pSnHeadData == NULL )
		return -1;

	if (pSnFhyo5_012Data == NULL) {
		return -1;
	}
	if (pArith == NULL) {
		return -1;
	}
	if (pSyzSyukei == NULL) {
		return -1;
	}

	m_pSnHeadData = pSnHeadData;
	m_pSnFhyo5_012Data = pSnFhyo5_012Data;

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
int CShinFhyo5_012::InitDiagAttr()
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
		if ( Fhyo5_012[cnt].Index == 0 )
			break;

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo5_012[cnt].AllUnConnect;
		}
		else if ( (m_typecnt < 2) || (m_4val == 0) ) {
			// 単一業種の時には入力不可
			Flg = Fhyo5_012[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				/*if ( IsProvideEditCell ( Fhyo5_012[cnt].Index ) ) {
					Flg = 0;
				}
				else {
					Flg = Fhyo5_012[cnt].UnConnect;
				}*/
				if ((Fhyo5_012[cnt].Index == ID5012_20D) || (Fhyo5_012[cnt].Index == ID5012_20E)) {
					Flg = Fhyo5_012[cnt].UnConnect;
				}
				else if ((Fhyo5_012[cnt].Index == ID5012_20F) && (m_flg & 0x01)) {
					Flg = Fhyo5_012[cnt].UnConnect;
				}
				// 付表5-1(1)の売上割合をチェック
				/*else if ( (m_typecnt > 1) && ((Fhyo5_012[cnt].Index == ID5012_21D) || (Fhyo5_012[cnt].Index == ID5012_21E)) && (m_id == ID5012_21F) ) {
					Flg = Fhyo5_012[cnt].UnConnect;
				}
				else if ( (m_typecnt > 1) && (Fhyo5_012[cnt].Index == ID5012_21F) && (m_id == ID5012_21F) && (m_flg & 0x01)) {
					Flg = Fhyo5_012[cnt].UnConnect;
				}
				else if ( (m_typecnt > 2) && ((Fhyo5_012[cnt].Index == (m_id - 2)) || (Fhyo5_012[cnt].Index == (m_id - 1))) ) {
					Flg = Fhyo5_012[cnt].UnConnect;
				}
				else if ( (m_typecnt > 2) && (Fhyo5_012[cnt].Index == m_id) && (m_flg & 0x01)) {
					Flg = Fhyo5_012[cnt].UnConnect;
				}*/
				// 付表5-1(1)の売上割合をチェック
				else if ( IsDispArea ( Fhyo5_012[cnt].Index ) == 1 ) {
					Flg = Fhyo5_012[cnt].UnConnect;
					CalqGokei( Fhyo5_012[cnt].Index );
				}
				else {
					Flg = 0;
					ClearInput ( Fhyo5_012[cnt].Index );
				}
			}
			else {
				Flg = Fhyo5_012[cnt].Connect;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL1, Fhyo5_012[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo5_012[cnt].Connect );
		cnt++;

	}

	m_Fhyo5_012Diag01.EnableDelete ( 0 );

	//if ( IsKjCodeReset() == true ) {
	//	ResetKjCode();
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
void CShinFhyo5_012::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		//index = m_Fhyo5_012Diag01.GetPosition();
		if ( ( index = m_Fhyo5_012Diag01.GetPosition() ) != -1 ) {
			m_Fhyo5_012Diag01.SetFocus();	// ダミーでセット
			m_Fhyo5_012Diag01.SetPosition ( index );
			m_Fhyo5_012Diag01.SetFocus();
			return;
		}
	}

	GetDlgItem(IDC_STATIC_INPUTLOCK)->SetFocus();	// ダミーでセット
	m_Fhyo5_012Diag01.SetFocus();	// ダミーでセット

	//　4×みなし仕入率へ
	m_Fhyo5_012Diag01.SetPosition ( ID5012_20D );
}

//=================================================
// 入力項目色変え
//-------------------------------------------------
// 引数
//		id		:	ダイアグラムID
//		index	:	ダイアグラム内インデックス
//		sign	:	変更サイン
//=================================================
void CShinFhyo5_012::ChangeColor(unsigned short id, short index, int sign)
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
int CShinFhyo5_012::UpDownFocusMoveCheck(int Index, int Move)
{
	int idx = 0;
	int i = 0, j = 0;

	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY5_012CNT; i++ ) {
		if ( Index == FCS_Fhyo5_012[i].IDNo )
			break;
	}

	// 現在ポジションセット
	idx = FCS_Fhyo5_012[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) {			//　下方向
		for ( j = i + 1; j < FCS_FHY5_012CNT; j++ ) {
			m_Fhyo5_012Diag01.GetAttribute ( FCS_Fhyo5_012[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo5_012[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {		//　上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo5_012Diag01.GetAttribute ( FCS_Fhyo5_012[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo5_012[j].IDNo;
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
int CShinFhyo5_012::IsProvideEditCell(short Idx)
{
	int rt = 0;

	return rt;
}

BEGIN_EVENTSINK_MAP(CShinFhyo5_012, CSyzBaseDlg)
	ON_EVENT(CShinFhyo5_012, IDC_ICSDIAG8CTRL1, 1, CShinFhyo5_012::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinFhyo5_012, IDC_ICSDIAG8CTRL1, 2, CShinFhyo5_012::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinFhyo5_012, IDC_ICSDIAG8CTRL1, 3, CShinFhyo5_012::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo5_012::EditONIcsdiag8ctrl1(short index)
{
	m_curwnd = IDC_ICSDIAG8CTRL1;
}

void CShinFhyo5_012::EditOFFIcsdiag8ctrl1(short index)
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	char  CkMy[MONY_BUF_SIZE] = { 0 };
	
	switch ( index ) {
		// 原則計算を適用する場合
		case ID5012_20X:
		case ID5012_20D:
		case ID5012_20E:
		case ID5012_20F:
			SyukeiDataSet(index);
			Disp_f5012data(1);
			break;

		// 特例計算を適用する場合
		case ID5012_21X:
		case ID5012_21D:
		case ID5012_21E:
		case ID5012_21F:
			SyukeiDataSet(index);
			Disp_f5012data(2);
			break;
		

		case ID5012_22X:
		case ID5012_22D:
		case ID5012_22E:
		case ID5012_22F:
			SyukeiDataSet(index);
			Disp_f5012data(3);
			break;

		case ID5012_23X:
		case ID5012_23D:
		case ID5012_23E:
		case ID5012_23F:
			SyukeiDataSet(index);
			Disp_f5012data(4);
			break;

		case ID5012_24X:
		case ID5012_24D:
		case ID5012_24E:
		case ID5012_24F:
			SyukeiDataSet(index);
			Disp_f5012data(5);
			break;

		case ID5012_25X:
		case ID5012_25D:
		case ID5012_25E:
		case ID5012_25F:
			SyukeiDataSet(index);
			Disp_f5012data(6);
			break;

		case ID5012_26X:
		case ID5012_26D:
		case ID5012_26E:
		case ID5012_26F:
			SyukeiDataSet(index);
			Disp_f5012data(7);
			break;

		case ID5012_27X:
		case ID5012_27D:
		case ID5012_27E:
		case ID5012_27F:
			SyukeiDataSet(index);
			Disp_f5012data(8);
			break;

		case ID5012_28X:
		case ID5012_28D:
		case ID5012_28E:
		case ID5012_28F:
			SyukeiDataSet(index);
			Disp_f5012data(9);
			break;

		case ID5012_29X:
		case ID5012_29D:
		case ID5012_29E:
		case ID5012_29F:
			SyukeiDataSet(index);
			Disp_f5012data(10);
			break;

		case ID5012_30X:
		case ID5012_30D:
		case ID5012_30E:
		case ID5012_30F:
			SyukeiDataSet(index);
			Disp_f5012data(11);
			break;

		case ID5012_31X:
		case ID5012_31D:
		case ID5012_31E:
		case ID5012_31F:
			SyukeiDataSet(index);
			Disp_f5012data(12);
			break;

		case ID5012_32X:
		case ID5012_32D:
		case ID5012_32E:
		case ID5012_32F:
			SyukeiDataSet(index);
			Disp_f5012data(13);
			break;

		case ID5012_33X:
		case ID5012_33D:
		case ID5012_33E:
		case ID5012_33F:
			SyukeiDataSet(index);
			Disp_f5012data(14);
			break;

		case ID5012_34X:
		case ID5012_34D:
		case ID5012_34E:
		case ID5012_34F:
			SyukeiDataSet(index);
			Disp_f5012data(15);
			break;

		case ID5012_35X:
		case ID5012_35D:
		case ID5012_35E:
		case ID5012_35F:
			SyukeiDataSet(index);
			Disp_f5012data(16);
			break;

		case ID5012_36X:
		case ID5012_36D:
		case ID5012_36E:
		case ID5012_36F:
			SyukeiDataSet(index);
			Disp_f5012data(17);
			break;

		default:
			break;
	}
}

void CShinFhyo5_012::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	if ( nChar == 0 ) {
		return;
	}

	short wTerm = 0;
	char VK_FLG = 0x00;
	// Shift+TabをTabに、TabをEnterに変換
	VK_FLG = (char)::GetKeyState( VK_SHIFT );
	if ( nChar == VK_TAB ) {
		if ( VK_FLG & 0x80 ) {
			wTerm = VK_TAB;
		}
		else{
			wTerm = VK_RETURN;
		}
	}
	else {
		wTerm = nChar;
	}

	int pos = 0;
	if ( wTerm == VK_UP ) {
		pos = UpDownFocusMoveCheck( index, 1 );
		m_Fhyo5_012Diag01.SetPosition( pos );
	}
	if ( wTerm == VK_DOWN ) {
		pos = UpDownFocusMoveCheck( index, 0 );
		m_Fhyo5_012Diag01.SetPosition( pos );
	}

	if ( (wTerm == VK_RETURN) || (wTerm == VK_RIGHT) || (wTerm == VK_F3) ) {
		m_Fhyo5_012Diag01.SetNextPosition();
	}
	if ( (wTerm == VK_TAB) || (wTerm == VK_LEFT) || (wTerm == VK_F2) ) {
		m_Fhyo5_012Diag01.SetPrevPosition();
	}

	if ( wTerm == VK_DELETE ) {
		switch ( index ) {
		case ID5012_20D:
			F5012_20f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359002"));
			// 各入力の表示
			Disp_f5012data( 1 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_20D );
			break;
		case ID5012_20E:
			F5012_20f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359003"));
			// 各入力の表示
			Disp_f5012data( 1 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_20E );
			break;
		case ID5012_20F:
			F5012_20f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359000"));
			// 各入力の表示
			Disp_f5012data( 1 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_20F );
			break;

		case ID5012_21D:
			F5012_21f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359102"));
			// 各入力の表示
			Disp_f5012data( 2 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_21D );
			break;
		case ID5012_21E:
			F5012_21f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359103"));
			// 各入力の表示
			Disp_f5012data( 2 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_21E );
			break;
		case ID5012_21F:
			F5012_21f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359100"));
			// 各入力の表示
			Disp_f5012data( 2 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_21F );
			break;

		case ID5012_22D:
			F5012_22f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359202"));
			// 各入力の表示
			Disp_f5012data( 3 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_22D );
			break;
		case ID5012_22E:
			F5012_22f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359203"));
			// 各入力の表示
			Disp_f5012data( 3 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_22E );
			break;
		case ID5012_22F:
			F5012_22f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359200"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E001"));
			// 各入力の表示
			Disp_f5012data( 3 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_22F );
			break;

		case ID5012_23D:
			F5012_23f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359302"));
			// 各入力の表示
			Disp_f5012data( 4 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_23D );
			break;
		case ID5012_23E:
			F5012_23f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359303"));
			// 各入力の表示
			Disp_f5012data( 4 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_23E );
			break;
		case ID5012_23F:
			F5012_23f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359300"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E002"));
			// 各入力の表示
			Disp_f5012data( 4 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_23F );
			break;

		case ID5012_24D:
			F5012_24f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359402"));
			// 各入力の表示
			Disp_f5012data( 5 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_24D );
			break;
		case ID5012_24E:
			F5012_24f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359403"));
			// 各入力の表示
			Disp_f5012data( 5 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_24E );
			break;
		case ID5012_24F:
			F5012_24f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359400"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E003"));
			// 各入力の表示
			Disp_f5012data( 5 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_24F );
			break;

		case ID5012_25D:
			F5012_25f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359502"));
			// 各入力の表示
			Disp_f5012data( 6 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_25D );
			break;
		case ID5012_25E:
			F5012_25f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359503"));
			// 各入力の表示
			Disp_f5012data( 6 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_25E );
			break;
		case ID5012_25F:
			F5012_25f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359500"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E004"));
			// 各入力の表示
			Disp_f5012data( 6 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_25F );
			break;

		case ID5012_26D:
			F5012_26f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359602"));
			// 各入力の表示
			Disp_f5012data( 7 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_26D );
			break;
		case ID5012_26E:
			F5012_26f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359603"));
			// 各入力の表示
			Disp_f5012data( 7 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_26E );
			break;
		case ID5012_26F:
			F5012_26f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359600"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E005"));
			// 各入力の表示
			Disp_f5012data( 7 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_26F );
			break;

		case ID5012_27D:
			F5012_27f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359702"));
			// 各入力の表示
			Disp_f5012data( 8 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_27D );
			break;
		case ID5012_27E:
			F5012_27f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359703"));
			// 各入力の表示
			Disp_f5012data( 8 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_27E );
			break;
		case ID5012_27F:
			F5012_27f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359700"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E006"));
			// 各入力の表示
			Disp_f5012data( 8 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_27F );
			break;

		case ID5012_28D:
			F5012_28f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359802"));
			// 各入力の表示
			Disp_f5012data( 9 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_28D );
			break;
		case ID5012_28E:
			F5012_28f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359803"));
			// 各入力の表示
			Disp_f5012data( 9 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_28E );
			break;
		case ID5012_28F:
			F5012_28f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359800"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E007"));
			// 各入力の表示
			Disp_f5012data( 9 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_28F );
			break;

		case ID5012_29D:
			F5012_29f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359902"));
			// 各入力の表示
			Disp_f5012data( 10 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_29D );
			break;
		case ID5012_29E:
			F5012_29f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359903"));
			// 各入力の表示
			Disp_f5012data( 10 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_29E );
			break;
		case ID5012_29F:
			F5012_29f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359900"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E008"));
			// 各入力の表示
			Disp_f5012data( 10 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_29F );
			break;

		case ID5012_30D:
			F5012_30f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A002"));
			// 各入力の表示
			Disp_f5012data( 11 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_30D );
			break;
		case ID5012_30E:
			F5012_30f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A003"));
			// 各入力の表示
			Disp_f5012data( 11 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_30E );
			break;
		case ID5012_30F:
			F5012_30f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A000"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E009"));
			// 各入力の表示
			Disp_f5012data( 11 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_30F );
			break;

		case ID5012_31D:
			F5012_31f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A102"));
			// 各入力の表示
			Disp_f5012data( 12 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_31D );
			break;
		case ID5012_31E:
			F5012_31f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A103"));
			// 各入力の表示
			Disp_f5012data( 12 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_31E );
			break;
		case ID5012_31F:
			F5012_31f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A100"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E00A"));
			// 各入力の表示
			Disp_f5012data( 12 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_31F );
			break;

		case ID5012_32D:
			F5012_32f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A202"));
			// 各入力の表示
			Disp_f5012data( 13 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_32D );
			break;
		case ID5012_32E:
			//F5012_20f &= ~0x800;
			F5012_32f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A203"));
			// 各入力の表示
			Disp_f5012data( 13 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_32E );
			break;
		case ID5012_32F:
			F5012_32f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A200"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E00B"));
			// 各入力の表示
			Disp_f5012data( 13 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_32F );
			break;

		case ID5012_33D:
			F5012_33f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A302"));
			// 各入力の表示
			Disp_f5012data( 14 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_33D );
			break;
		case ID5012_33E:
			F5012_33f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A303"));
			// 各入力の表示
			Disp_f5012data( 14 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_33E );
			break;
		case ID5012_33F:
			F5012_33f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A300"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E00C"));
			// 各入力の表示
			Disp_f5012data( 14 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_33F );
			break;

		case ID5012_34D:
			F5012_34f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A402"));
			// 各入力の表示
			Disp_f5012data( 15 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_34D );
			break;
		case ID5012_34E:
			F5012_34f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A403"));
			// 各入力の表示
			Disp_f5012data( 15 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_34E );
			break;
		case ID5012_34F:
			F5012_34f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A400"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E00D"));
			// 各入力の表示
			Disp_f5012data( 15 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_34F );
			break;

		case ID5012_35D:
			F5012_35f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A502"));
			// 各入力の表示
			Disp_f5012data( 16 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_35D );
			break;
		case ID5012_35E:
			F5012_35f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A503"));
			// 各入力の表示
			Disp_f5012data( 16 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_35E );
			break;
		case ID5012_35F:
			F5012_35f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A500"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E00E"));
			// 各入力の表示
			Disp_f5012data( 16 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_35F );
			break;
			
		case ID5012_36D:
			F5012_36f &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A602"));
			// 各入力の表示
			Disp_f5012data( 17 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_36D );
			break;
		case ID5012_36E:
			F5012_36f &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A603"));
			// 各入力の表示
			Disp_f5012data( 17 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_36E );
			break;
		case ID5012_36F:
			F5012_36f &= ~0x8000;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A600"));
			//(*m_pSyzSyukei)->ResetShinkokuData(_T("35E00F"));
			// 各入力の表示
			Disp_f5012data( 17 );
			m_Fhyo5_012Diag01.SetPosition( ID5012_36F );
			break;

		default :
			dsp_cls ( IDC_ICSDIAG8CTRL1, index );
			break;
		}

		ClearInput ( index );
		m_Fhyo5_012Diag01.SetPosition( index );
	}

	int nowpos = m_Fhyo5_012Diag01.GetPosition();

	RECT rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAG8CTRL1, nowpos, &rectB, CIcsdiagctrl );
	if ((rectA.top > rectB.top) || (rectA.bottom < rectB.bottom)) {
		int cpos = GetScrollPos(SB_VERT);
		SetRedrawScroll((cpos + rectB.top - rectA.top) - ((rectA.bottom - rectA.top) / 2));
	}
}

//============================
// データ読み込み
//============================
void CShinFhyo5_012::ReadData()
{
	// 4×みなし仕入率
	memmove(F5012_20X, (*m_pSnFhyo5_012Data)->Sn_F5012_20X, MONY_BUF_SIZE);
	memmove(F5012_20D, (*m_pSnFhyo5_012Data)->Sn_F5012_20D, MONY_BUF_SIZE);
	memmove(F5012_20E, (*m_pSnFhyo5_012Data)->Sn_F5012_20E, MONY_BUF_SIZE);
	memmove(F5012_20F, (*m_pSnFhyo5_012Data)->Sn_F5012_20F, MONY_BUF_SIZE);
	F5012_20f = (*m_pSnFhyo5_012Data)->Sn_F5012_20sw;

	// 4×みなし仕入率
	memmove(F5012_21X, (*m_pSnFhyo5_012Data)->Sn_F5012_21X, MONY_BUF_SIZE);
	memmove(F5012_21D, (*m_pSnFhyo5_012Data)->Sn_F5012_21D, MONY_BUF_SIZE);
	memmove(F5012_21E, (*m_pSnFhyo5_012Data)->Sn_F5012_21E, MONY_BUF_SIZE);
	memmove(F5012_21F, (*m_pSnFhyo5_012Data)->Sn_F5012_21F, MONY_BUF_SIZE);
	F5012_21f = (*m_pSnFhyo5_012Data)->Sn_F5012_21sw;

	// 第一種事業及び第二種事業
	memmove(F5012_22X, (*m_pSnFhyo5_012Data)->Sn_F5012_22X, MONY_BUF_SIZE);
	memmove(F5012_22D, (*m_pSnFhyo5_012Data)->Sn_F5012_22D, MONY_BUF_SIZE);
	memmove(F5012_22E, (*m_pSnFhyo5_012Data)->Sn_F5012_22E, MONY_BUF_SIZE);
	memmove(F5012_22F, (*m_pSnFhyo5_012Data)->Sn_F5012_22F, MONY_BUF_SIZE);
	F5012_22f = (*m_pSnFhyo5_012Data)->Sn_F5012_22sw;

	// 第一種事業及び第三種事業
	memmove(F5012_23X, (*m_pSnFhyo5_012Data)->Sn_F5012_23X, MONY_BUF_SIZE);
	memmove(F5012_23D, (*m_pSnFhyo5_012Data)->Sn_F5012_23D, MONY_BUF_SIZE);
	memmove(F5012_23E, (*m_pSnFhyo5_012Data)->Sn_F5012_23E, MONY_BUF_SIZE);
	memmove(F5012_23F, (*m_pSnFhyo5_012Data)->Sn_F5012_23F, MONY_BUF_SIZE);
	F5012_23f = (*m_pSnFhyo5_012Data)->Sn_F5012_23sw;

	// 第一種事業及び第四種事業
	memmove(F5012_24X, (*m_pSnFhyo5_012Data)->Sn_F5012_24X, MONY_BUF_SIZE);
	memmove(F5012_24D, (*m_pSnFhyo5_012Data)->Sn_F5012_24D, MONY_BUF_SIZE);
	memmove(F5012_24E, (*m_pSnFhyo5_012Data)->Sn_F5012_24E, MONY_BUF_SIZE);
	memmove(F5012_24F, (*m_pSnFhyo5_012Data)->Sn_F5012_24F, MONY_BUF_SIZE);
	F5012_24f = (*m_pSnFhyo5_012Data)->Sn_F5012_24sw;

	// 第一種事業及び第五種事業
	memmove(F5012_25X, (*m_pSnFhyo5_012Data)->Sn_F5012_25X, MONY_BUF_SIZE);
	memmove(F5012_25D, (*m_pSnFhyo5_012Data)->Sn_F5012_25D, MONY_BUF_SIZE);
	memmove(F5012_25E, (*m_pSnFhyo5_012Data)->Sn_F5012_25E, MONY_BUF_SIZE);
	memmove(F5012_25F, (*m_pSnFhyo5_012Data)->Sn_F5012_25F, MONY_BUF_SIZE);
	F5012_25f = (*m_pSnFhyo5_012Data)->Sn_F5012_25sw;

	// 第一種事業及び第六種事業
	memmove(F5012_26X, (*m_pSnFhyo5_012Data)->Sn_F5012_26X, MONY_BUF_SIZE);
	memmove(F5012_26D, (*m_pSnFhyo5_012Data)->Sn_F5012_26D, MONY_BUF_SIZE);
	memmove(F5012_26E, (*m_pSnFhyo5_012Data)->Sn_F5012_26E, MONY_BUF_SIZE);
	memmove(F5012_26F, (*m_pSnFhyo5_012Data)->Sn_F5012_26F, MONY_BUF_SIZE);
	F5012_26f = (*m_pSnFhyo5_012Data)->Sn_F5012_26sw;

	// 第二種事業及び第三種事業
	memmove(F5012_27X, (*m_pSnFhyo5_012Data)->Sn_F5012_27X, MONY_BUF_SIZE);
	memmove(F5012_27D, (*m_pSnFhyo5_012Data)->Sn_F5012_27D, MONY_BUF_SIZE);
	memmove(F5012_27E, (*m_pSnFhyo5_012Data)->Sn_F5012_27E, MONY_BUF_SIZE);
	memmove(F5012_27F, (*m_pSnFhyo5_012Data)->Sn_F5012_27F, MONY_BUF_SIZE);
	F5012_27f = (*m_pSnFhyo5_012Data)->Sn_F5012_27sw;

	// 第二種事業及び第四種事業
	memmove(F5012_28X, (*m_pSnFhyo5_012Data)->Sn_F5012_28X, MONY_BUF_SIZE);
	memmove(F5012_28D, (*m_pSnFhyo5_012Data)->Sn_F5012_28D, MONY_BUF_SIZE);
	memmove(F5012_28E, (*m_pSnFhyo5_012Data)->Sn_F5012_28E, MONY_BUF_SIZE);
	memmove(F5012_28F, (*m_pSnFhyo5_012Data)->Sn_F5012_28F, MONY_BUF_SIZE);
	F5012_28f = (*m_pSnFhyo5_012Data)->Sn_F5012_28sw;

	// 第二種事業及び第五種事業
	memmove(F5012_29X, (*m_pSnFhyo5_012Data)->Sn_F5012_29X, MONY_BUF_SIZE);
	memmove(F5012_29D, (*m_pSnFhyo5_012Data)->Sn_F5012_29D, MONY_BUF_SIZE);
	memmove(F5012_29E, (*m_pSnFhyo5_012Data)->Sn_F5012_29E, MONY_BUF_SIZE);
	memmove(F5012_29F, (*m_pSnFhyo5_012Data)->Sn_F5012_29F, MONY_BUF_SIZE);
	F5012_29f = (*m_pSnFhyo5_012Data)->Sn_F5012_29sw;

	// 第二種事業及び第六種事業
	memmove(F5012_30X, (*m_pSnFhyo5_012Data)->Sn_F5012_30X, MONY_BUF_SIZE);
	memmove(F5012_30D, (*m_pSnFhyo5_012Data)->Sn_F5012_30D, MONY_BUF_SIZE);
	memmove(F5012_30E, (*m_pSnFhyo5_012Data)->Sn_F5012_30E, MONY_BUF_SIZE);
	memmove(F5012_30F, (*m_pSnFhyo5_012Data)->Sn_F5012_30F, MONY_BUF_SIZE);
	F5012_30f = (*m_pSnFhyo5_012Data)->Sn_F5012_30sw;

	// 第三種事業及び第四種事業
	memmove(F5012_31X, (*m_pSnFhyo5_012Data)->Sn_F5012_31X, MONY_BUF_SIZE);
	memmove(F5012_31D, (*m_pSnFhyo5_012Data)->Sn_F5012_31D, MONY_BUF_SIZE);
	memmove(F5012_31E, (*m_pSnFhyo5_012Data)->Sn_F5012_31E, MONY_BUF_SIZE);
	memmove(F5012_31F, (*m_pSnFhyo5_012Data)->Sn_F5012_31F, MONY_BUF_SIZE);
	F5012_31f = (*m_pSnFhyo5_012Data)->Sn_F5012_31sw;

	// 第三種事業及び第五種事業
	memmove(F5012_32X, (*m_pSnFhyo5_012Data)->Sn_F5012_32X, MONY_BUF_SIZE);
	memmove(F5012_32D, (*m_pSnFhyo5_012Data)->Sn_F5012_32D, MONY_BUF_SIZE);
	memmove(F5012_32E, (*m_pSnFhyo5_012Data)->Sn_F5012_32E, MONY_BUF_SIZE);
	memmove(F5012_32F, (*m_pSnFhyo5_012Data)->Sn_F5012_32F, MONY_BUF_SIZE);
	F5012_32f = (*m_pSnFhyo5_012Data)->Sn_F5012_32sw;

	// 第三種事業及び第六種事業
	memmove(F5012_33X, (*m_pSnFhyo5_012Data)->Sn_F5012_33X, MONY_BUF_SIZE);
	memmove(F5012_33D, (*m_pSnFhyo5_012Data)->Sn_F5012_33D, MONY_BUF_SIZE);
	memmove(F5012_33E, (*m_pSnFhyo5_012Data)->Sn_F5012_33E, MONY_BUF_SIZE);
	memmove(F5012_33F, (*m_pSnFhyo5_012Data)->Sn_F5012_33F, MONY_BUF_SIZE);
	F5012_33f = (*m_pSnFhyo5_012Data)->Sn_F5012_33sw;

	// 第四種事業及び第五種事業
	memmove(F5012_34X, (*m_pSnFhyo5_012Data)->Sn_F5012_34X, MONY_BUF_SIZE);
	memmove(F5012_34D, (*m_pSnFhyo5_012Data)->Sn_F5012_34D, MONY_BUF_SIZE);
	memmove(F5012_34E, (*m_pSnFhyo5_012Data)->Sn_F5012_34E, MONY_BUF_SIZE);
	memmove(F5012_34F, (*m_pSnFhyo5_012Data)->Sn_F5012_34F, MONY_BUF_SIZE);
	F5012_34f = (*m_pSnFhyo5_012Data)->Sn_F5012_34sw;

	// 第四種事業及び第六種事業
	memmove(F5012_35X, (*m_pSnFhyo5_012Data)->Sn_F5012_35X, MONY_BUF_SIZE);
	memmove(F5012_35D, (*m_pSnFhyo5_012Data)->Sn_F5012_35D, MONY_BUF_SIZE);
	memmove(F5012_35E, (*m_pSnFhyo5_012Data)->Sn_F5012_35E, MONY_BUF_SIZE);
	memmove(F5012_35F, (*m_pSnFhyo5_012Data)->Sn_F5012_35F, MONY_BUF_SIZE);
	F5012_35f = (*m_pSnFhyo5_012Data)->Sn_F5012_35sw;

	// 第五種事業及び第六種事業
	memmove(F5012_36X, (*m_pSnFhyo5_012Data)->Sn_F5012_36X, MONY_BUF_SIZE);
	memmove(F5012_36D, (*m_pSnFhyo5_012Data)->Sn_F5012_36D, MONY_BUF_SIZE);
	memmove(F5012_36E, (*m_pSnFhyo5_012Data)->Sn_F5012_36E, MONY_BUF_SIZE);
	memmove(F5012_36F, (*m_pSnFhyo5_012Data)->Sn_F5012_36F, MONY_BUF_SIZE);
	F5012_36f = (*m_pSnFhyo5_012Data)->Sn_F5012_36sw;

	// 選択可能な計算式区分の内から選択した金額
	memmove(F5012_37X, (*m_pSnFhyo5_012Data)->Sn_F5012_37X, MONY_BUF_SIZE);
	memmove(F5012_37D, (*m_pSnFhyo5_012Data)->Sn_F5012_37D, MONY_BUF_SIZE);
	memmove(F5012_37E, (*m_pSnFhyo5_012Data)->Sn_F5012_37E, MONY_BUF_SIZE);
	memmove(F5012_37F, (*m_pSnFhyo5_012Data)->Sn_F5012_37F, MONY_BUF_SIZE);

}


//============================
// データ書き込み
//============================
void CShinFhyo5_012::WriteData()
{
	char zerobuf[MONY_BUF_SIZE] = { 0 };

	IsEnableItems(1, m_id, m_code, m_typecnt);
	m_4val = Is4Val(0);

	// 4×みなし仕入率
	if ((m_typecnt > 1) && (m_4val)) {
		// D欄が入力可能なら保存
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_20X, F5012_20X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_20D, F5012_20D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_20E, F5012_20E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_20F, F5012_20F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_20sw = F5012_20f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_20X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_20D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_20E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_20F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_20sw = 0;
	}

	// 4×みなし仕入率
	//if (m_id == ID5012_21F) {
	if ( IsDispArea ( ID5012_21D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_21X, F5012_21X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_21D, F5012_21D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_21E, F5012_21E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_21F, F5012_21F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_21sw = F5012_21f;
		(*m_pSnFhyo5_012Data)->Sn_F5012_21kbn = F5012_21kbn;

	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_21X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_21D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_21E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_21F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_21sw = 0;
		(*m_pSnFhyo5_012Data)->Sn_F5012_21kbn = 0;
	}

	// 第一種事業及び第二種事業
	//if (m_id == ID5012_22F) {
	if ( IsDispArea ( ID5012_22D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_22X, F5012_22X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_22D, F5012_22D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_22E, F5012_22E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_22F, F5012_22F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_22sw = F5012_22f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_22X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_22D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_22E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_22F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_22sw = 0;
	}

	// 第一種事業及び第三種事業
	//if (m_id == ID5012_23F) {
	if ( IsDispArea ( ID5012_23D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_23X, F5012_23X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_23D, F5012_23D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_23E, F5012_23E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_23F, F5012_23F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_23sw = F5012_23f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_23X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_23D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_23E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_23F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_23sw = 0;
	}

	// 第一種事業及び第四種事業
	//if (m_id == ID5012_24F) {
	if ( IsDispArea ( ID5012_24D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_24X, F5012_24X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_24D, F5012_24D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_24E, F5012_24E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_24F, F5012_24F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_24sw = F5012_24f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_24X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_24D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_24E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_24F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_24sw = 0;
	}

	// 第一種事業及び第五種事業
	//if (m_id == ID5012_25F) {
	if ( IsDispArea ( ID5012_25D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_25X, F5012_25X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_25D, F5012_25D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_25E, F5012_25E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_25F, F5012_25F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_25sw = F5012_25f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_25X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_25D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_25E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_25F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_25sw = 0;
	}

	// 第一種事業及び第六種事業
	//if (m_id == ID5012_26F) {
	if ( IsDispArea ( ID5012_26D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_26X, F5012_26X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_26D, F5012_26D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_26E, F5012_26E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_26F, F5012_26F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_26sw = F5012_26f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_26X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_26D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_26E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_26F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_26sw = 0;
	}

	// 第二種事業及び第三種事業
	//if (m_id == ID5012_27F) {
	if ( IsDispArea ( ID5012_27D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_27X, F5012_27X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_27D, F5012_27D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_27E, F5012_27E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_27F, F5012_27F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_27sw = F5012_27f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_27X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_27D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_27E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_27F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_27sw = 0;
	}

	// 第二種事業及び第四種事業
	//if (m_id == ID5012_28F) {
	if ( IsDispArea ( ID5012_28D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_28X, F5012_28X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_28D, F5012_28D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_28E, F5012_28E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_28F, F5012_28F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_28sw = F5012_28f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_28X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_28D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_28E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_28F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_28sw = 0;
	}

	// 第二種事業及び第五種事業
	//if (m_id == ID5012_29F) {
	if ( IsDispArea ( ID5012_29D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_29X, F5012_29X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_29D, F5012_29D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_29E, F5012_29E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_29F, F5012_29F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_29sw = F5012_29f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_29X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_29D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_29E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_29F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_29sw = 0;
	}

	// 第二種事業及び第六種事業
	//if (m_id == ID5012_30F) {
	if ( IsDispArea ( ID5012_30D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_30X, F5012_30X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_30D, F5012_30D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_30E, F5012_30E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_30F, F5012_30F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_30sw = F5012_30f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_30X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_30D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_30E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_30F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_30sw = 0;
	}

	// 第三種事業及び第四種事業
	//if (m_id == ID5012_31F) {
	if ( IsDispArea ( ID5012_31D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_31X, F5012_31X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_31D, F5012_31D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_31E, F5012_31E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_31F, F5012_31F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_31sw = F5012_31f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_31X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_31D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_31E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_31F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_31sw = 0;
	}

	// 第三種事業及び第五種事業
	//if (m_id == ID5012_32F) {
	if ( IsDispArea ( ID5012_32D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_32X, F5012_32X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_32D, F5012_32D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_32E, F5012_32E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_32F, F5012_32F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_32sw = F5012_32f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_32X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_32D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_32E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_32F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_32sw = 0;
	}

	// 第三種事業及び第六種事業
	//if (m_id == ID5012_33F) {
	if ( IsDispArea ( ID5012_33D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_33X, F5012_33X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_33D, F5012_33D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_33E, F5012_33E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_33F, F5012_33F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_33sw = F5012_33f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_33X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_33D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_33E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_33F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_33sw = 0;
	}

	// 第四種事業及び第五種事業
	//if (m_id == ID5012_34F) {
	if ( IsDispArea ( ID5012_34D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_34X, F5012_34X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_34D, F5012_34D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_34E, F5012_34E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_34F, F5012_34F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_34sw = F5012_34f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_34X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_34D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_34E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_34F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_34sw = 0;
	}

	// 第四種事業及び第六種事業
	//if (m_id == ID5012_35F) {
	if ( IsDispArea ( ID5012_35D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_35X, F5012_35X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_35D, F5012_35D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_35E, F5012_35E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_35F, F5012_35F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_35sw = F5012_35f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_35X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_35D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_35E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_35F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_35sw = 0;
	}

	// 第五種事業及び第六種事業
	//if (m_id == ID5012_36F) {
	if ( IsDispArea ( ID5012_36D ) ) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_36X, F5012_36X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_36D, F5012_36D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_36E, F5012_36E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_36F, F5012_36F, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_36sw = F5012_36f;
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_36X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_36D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_36E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_36F, zerobuf, MONY_BUF_SIZE);
		(*m_pSnFhyo5_012Data)->Sn_F5012_36sw = 0;
	}

	// 選択可能な計算式区分の内から選択した金額
	if (m_id != 0) {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_37X, F5012_37X, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_37D, F5012_37D, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_37E, F5012_37E, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_37F, F5012_37F, MONY_BUF_SIZE);
	}
	else {
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_37X, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_37D, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_37E, zerobuf, MONY_BUF_SIZE);
		memmove((*m_pSnFhyo5_012Data)->Sn_F5012_37F, zerobuf, MONY_BUF_SIZE);
	}
}

//==============================
// 4×みなし仕入率
//==============================
void CShinFhyo5_012::f5012_20data()
{
	if ((m_typecnt > 1) && (m_4val)) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_20D, (F5012_20f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_20E, (F5012_20f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_20F, (F5012_20f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_20X, F5012_20X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_20D, F5012_20D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_20E, F5012_20E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_20F, F5012_20F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_20X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_20D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_20E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_20F);
	}
}

//==============================
// 4×みなし仕入率
//==============================
void CShinFhyo5_012::f5012_21data()
{
	/*if (m_id == ID5012_21F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_21D, (F5012_21f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_21E, (F5012_21f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_21F, (F5012_21f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_21X, F5012_21X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_21D, F5012_21D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_21E, F5012_21E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_21F, F5012_21F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_21X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_21D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_21E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_21F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_21D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_21D, ( F5012_21f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_21E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_21E, ( F5012_21f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_21F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_21F, ( F5012_21f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_21D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_21X, F5012_21X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_21D, F5012_21D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_21E, F5012_21E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_21F, F5012_21F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_21X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_21D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_21E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_21F );
	}
}

//==============================
// 第一種事業及び第二種事業
//==============================
void CShinFhyo5_012::f5012_22data()
{
	/*if (m_id == ID5012_22F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_22D, (F5012_22f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_22E, (F5012_22f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_22F, (F5012_22f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_22X, F5012_22X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_22D, F5012_22D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_22E, F5012_22E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_22F, F5012_22F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_22X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_22D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_22E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_22F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_22D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_22D, ( F5012_22f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_22E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_22E, ( F5012_22f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_22F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_22F, ( F5012_22f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_22D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_22X, F5012_22X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_22D, F5012_22D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_22E, F5012_22E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_22F, F5012_22F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_22X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_22D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_22E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_22F );
	}
}

//==============================
// 第一種事業及び第三種事業
//==============================
void CShinFhyo5_012::f5012_23data()
{
	/*if (m_id == ID5012_23F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_23D, (F5012_23f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_23E, (F5012_23f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_23F, (F5012_23f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_23X, F5012_23X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_23D, F5012_23D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_23E, F5012_23E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_23F, F5012_23F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_23X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_23D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_23E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_23F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_23D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_23D, ( F5012_23f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_23E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_23E, ( F5012_23f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_23F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_23F, ( F5012_23f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_23D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_23X, F5012_23X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_23D, F5012_23D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_23E, F5012_23E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_23F, F5012_23F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_23X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_23D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_23E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_23F );
	}
}

//==============================
// 第一種事業及び第四種事業
//==============================
void CShinFhyo5_012::f5012_24data()
{
	/*if (m_id == ID5012_24F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_24D, (F5012_24f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_24E, (F5012_24f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_24F, (F5012_24f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_24X, F5012_24X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_24D, F5012_24D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_24E, F5012_24E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_24F, F5012_24F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_24X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_24D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_24E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_24F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_24D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_24D, ( F5012_24f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_24E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_24E, ( F5012_24f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_24F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_24F, ( F5012_24f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_24D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_24X, F5012_24X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_24D, F5012_24D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_24E, F5012_24E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_24F, F5012_24F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_24X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_24D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_24E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_24F );
	}
}

//==============================
// 第一種事業及び第五種事業
//==============================
void CShinFhyo5_012::f5012_25data()
{
	/*if (m_id == ID5012_25F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_25D, (F5012_25f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_25E, (F5012_25f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_25F, (F5012_25f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_25X, F5012_25X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_25D, F5012_25D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_25E, F5012_25E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_25F, F5012_25F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_25X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_25D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_25E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_25F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_25D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_25D, ( F5012_25f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_25E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_25E, ( F5012_25f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_25F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_25F, ( F5012_25f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_25D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_25X, F5012_25X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_25D, F5012_25D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_25E, F5012_25E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_25F, F5012_25F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_25X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_25D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_25E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_25F );
	}
}

//==============================
// 第一種事業及び第六種事業
//==============================
void CShinFhyo5_012::f5012_26data()
{
	/*if (m_id == ID5012_26F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) ) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_26D, (F5012_26f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_26E, (F5012_26f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_26F, (F5012_26f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_26X, F5012_26X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_26D, F5012_26D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_26E, F5012_26E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_26F, F5012_26F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_26X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_26D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_26E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_26F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_26D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_26D, ( F5012_26f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_26E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_26E, ( F5012_26f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_26F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_26F, ( F5012_26f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_26D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_26X, F5012_26X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_26D, F5012_26D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_26E, F5012_26E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_26F, F5012_26F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_26X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_26D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_26E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_26F );
	}
}

//==============================
// 第二種事業及び第三種事業
//==============================
void CShinFhyo5_012::f5012_27data()
{
	/*if (m_id == ID5012_27F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_27D, (F5012_27f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_27E, (F5012_27f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_27F, (F5012_27f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_27X, F5012_27X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_27D, F5012_27D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_27E, F5012_27E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_27F, F5012_27F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_27X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_27D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_27E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_27F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_27D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_27D, ( F5012_27f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_27E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_27E, ( F5012_27f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_27F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_27F, ( F5012_27f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_27D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_27X, F5012_27X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_27D, F5012_27D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_27E, F5012_27E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_27F, F5012_27F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_27X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_27D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_27E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_27F );
	}
}

//==============================
// 第二種事業及び第四種事業
//==============================
void CShinFhyo5_012::f5012_28data()
{
	/*if (m_id == ID5012_28F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_28D, (F5012_28f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_28E, (F5012_28f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_28F, (F5012_28f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_28X, F5012_28X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_28D, F5012_28D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_28E, F5012_28E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_28F, F5012_28F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_28X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_28D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_28E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_28F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_28D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_28D, ( F5012_28f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_28E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_28E, ( F5012_28f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_28F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_28F, ( F5012_28f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_28D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_28X, F5012_28X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_28D, F5012_28D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_28E, F5012_28E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_28F, F5012_28F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_28X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_28D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_28E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_28F );
	}
}

//==============================
// 第二種事業及び第五種事業
//==============================
void CShinFhyo5_012::f5012_29data()
{
	/*if (m_id == ID5012_29F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_29D, (F5012_29f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_29E, (F5012_29f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_29F, (F5012_29f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_29X, F5012_29X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_29D, F5012_29D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_29E, F5012_29E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_29F, F5012_29F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_29X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_29D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_29E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_29F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_29D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_29D, ( F5012_29f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_29E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_29E, ( F5012_29f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_29F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_29F, ( F5012_29f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_29D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_29X, F5012_29X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_29D, F5012_29D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_29E, F5012_29E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_29F, F5012_29F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_29X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_29D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_29E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_29F );
	}
}

//==============================
// 第二種事業及び第六種事業
//==============================
void CShinFhyo5_012::f5012_30data()
{
	/*if (m_id == ID5012_30F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) ) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_30D, (F5012_30f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_30E, (F5012_30f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_30F, (F5012_30f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_30X, F5012_30X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_30D, F5012_30D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_30E, F5012_30E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_30F, F5012_30F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_30X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_30D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_30E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_30F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_30D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_30D, ( F5012_30f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_30E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_30E, ( F5012_30f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_30F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_30F, ( F5012_30f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_30D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_30X, F5012_30X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_30D, F5012_30D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_30E, F5012_30E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_30F, F5012_30F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_30X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_30D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_30E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_30F );
	}
}

//==============================
// 第三種事業及び第四種事業
//==============================
void CShinFhyo5_012::f5012_31data()
{
	/*if (m_id == ID5012_31F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) ) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_31D, (F5012_31f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_31E, (F5012_31f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_31F, (F5012_31f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_31X, F5012_31X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_31D, F5012_31D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_31E, F5012_31E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_31F, F5012_31F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_31X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_31D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_31E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_31F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_31D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_31D, ( F5012_31f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_31E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_31E, ( F5012_31f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_31F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_31F, ( F5012_31f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_31D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_31X, F5012_31X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_31D, F5012_31D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_31E, F5012_31E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_31F, F5012_31F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_31X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_31D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_31E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_31F );
	}
}

//==============================
// 第三種事業及び第五種事業
//==============================
void CShinFhyo5_012::f5012_32data()
{
	/*if (m_id == ID5012_32F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) ) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_32D, (F5012_32f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_32E, (F5012_32f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_32F, (F5012_32f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_32X, F5012_32X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_32D, F5012_32D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_32E, F5012_32E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_32F, F5012_32F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_32X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_32D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_32E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_32F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_32D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_32D, ( F5012_32f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_32E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_32E, ( F5012_32f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_32F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_32F, ( F5012_32f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_32D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_32X, F5012_32X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_32D, F5012_32D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_32E, F5012_32E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_32F, F5012_32F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_32X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_32D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_32E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_32F );
	}
}

//==============================
// 第三種事業及び第六種事業
//==============================
void CShinFhyo5_012::f5012_33data()
{
	/*if (m_id == ID5012_33F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_33D, (F5012_33f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_33E, (F5012_33f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_33F, (F5012_33f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_33X, F5012_33X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_33D, F5012_33D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_33E, F5012_33E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_33F, F5012_33F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_33X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_33D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_33E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_33F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_33D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_33D, ( F5012_33f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_33E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_33E, ( F5012_33f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_33F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_33F, ( F5012_33f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_33D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_33X, F5012_33X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_33D, F5012_33D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_33E, F5012_33E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_33F, F5012_33F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_33X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_33D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_33E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_33F );
	}
}

//==============================
// 第四種事業及び第五種事業
//==============================
void CShinFhyo5_012::f5012_34data()
{
	/*if (m_id == ID5012_34F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_34D, (F5012_34f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_34E, (F5012_34f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_34F, (F5012_34f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_34X, F5012_34X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_34D, F5012_34D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_34E, F5012_34E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_34F, F5012_34F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_34X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_34D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_34E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_34F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_34D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_34D, ( F5012_34f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_34E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_34E, ( F5012_34f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_34F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_34F, ( F5012_34f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_34D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_34X, F5012_34X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_34D, F5012_34D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_34E, F5012_34E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_34F, F5012_34F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_34X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_34D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_34E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_34F );
	}
}

//==============================
// 第四種事業及び第六種事業
//==============================
void CShinFhyo5_012::f5012_35data()
{
	/*if (m_id == ID5012_35F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_35D, (F5012_35f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_35E, (F5012_35f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_35F, (F5012_35f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_35X, F5012_35X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_35D, F5012_35D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_35E, F5012_35E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_35F, F5012_35F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_35X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_35D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_35E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_35F);
	}*/

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_35D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_35D, ( F5012_35f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_35E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_35E, ( F5012_35f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_35F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_35F, ( F5012_35f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_35D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_35X, F5012_35X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_35D, F5012_35D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_35E, F5012_35E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_35F, F5012_35F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_35X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_35D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_35E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_35F );
	}
}

//==============================
// 第五種事業及び第六種事業
//==============================
void CShinFhyo5_012::f5012_36data()
{
	/*if (m_id == ID5012_36F) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) ) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_36D, (F5012_36f & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_36E, (F5012_36f & 0x800) ? 3 : 0);
			if (m_flg & 0x01)
				ChangeColor(IDC_ICSDIAG8CTRL1, ID5012_36F, (F5012_36f & 0x8000) ? 3 : 0);
		}
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_36X, F5012_36X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_36D, F5012_36D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_36E, F5012_36E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_36F, F5012_36F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_36X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_36D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_36E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_36F);
	}*/
	
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		if ( IsInputArea ( ID5012_36D ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_36D, ( F5012_36f & 0x80 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_36E ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_36E, ( F5012_36f & 0x800 ) ? 3 : 0 );
		}
		if ( IsInputArea ( ID5012_36F ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID5012_36F, ( F5012_36f & 0x8000 ) ? 3 : 0 );
		}
	}
	if ( IsDispArea ( ID5012_36D ) ) {
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_36X, F5012_36X );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_36D, F5012_36D );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_36E, F5012_36E );
		dsp_prs ( IDC_ICSDIAG8CTRL1, ID5012_36F, F5012_36F );
	}
	else {
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_36X );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_36D );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_36E );
		dsp_cls ( IDC_ICSDIAG8CTRL1, ID5012_36F );
	}
}

//============================================
// 選択可能な計算式区分の内から選択した金額
//============================================
void CShinFhyo5_012::f5012_37data()
{
	if (m_id != 0) {
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_37X, F5012_37X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_37D, F5012_37D);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_37E, F5012_37E);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID5012_37F, F5012_37F);
	}
	else {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_37X);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_37D);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_37E);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID5012_37F);
	}
}

//==============================
// 入力データの表示
//------------------------------
// 引数
//		index	：　
//==============================
void CShinFhyo5_012::Disp_f5012data( int index )
{
	//while( 1 ){
		switch (index) {
			case 0:
			case 1:
				f5012_20data();
			case 2:
				f5012_21data();
			case 3:
				f5012_22data();
			case 4:
				f5012_23data();
			case 5:
				f5012_24data();
			case 6:
				f5012_25data();
			case 7:
				f5012_26data();
			case 8:
				f5012_27data();
			case 9:
				f5012_28data();
			case 10:
				f5012_29data();
			case 11:
				f5012_30data();
			case 12:
				f5012_31data();
			case 13:
				f5012_32data();
			case 14:
				f5012_33data();
			case 15:
				f5012_34data();
			case 16:
				f5012_35data();
			case 17:
				f5012_36data();
			case 18:
				f5012_37data();

			default:
				break;
		}
		//break;
	//}
}

//==============================
// 画面表示
//==============================
void CShinFhyo5_012::Disp_f5012()
{
	// 画面初期化
	for (int i = ID5012_20X; i <= ID5012_37F; i++ ) {
		dsp_cls(IDC_ICSDIAG8CTRL1, (short)i);
	}

	// ★手入力サイン初期化？

	InitDiagAttr();

/*	long flg = 0;
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
		m_Fhyo5_012Diag01.ModifyItem ( SELECTITEM, flg );
	}
	else {
		m_Fhyo5_012Diag01.ModifyItem ( SELECTITEM, DIAG_MDFY_TRANSPARENT );
	}
*/	

	// 入力データの表示
	Disp_f5012data(0);

}

//=================================================
// 金額表示
//-------------------------------------------------
// 引数
//		Id		:	ダイアグラムID
//		Index	:	ダイアグラム内インデックス
//		Mony	:	金額
//=================================================
void CShinFhyo5_012::dsp_prs(unsigned short Id, short Index, char* Mony)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	memmove(DiagData.data_val, Mony, MONY_BUF_SIZE);
	diag_setdata(Id, Index, (struct IUnknown*)&DiagData, CIcsdiagctrl);
}

//=================================================
// 金額表示クリア
//-------------------------------------------------
// 引数
//		Id		:	ダイアグラムID
//		Index	:	ダイアグラム内インデックス
//=================================================
void CShinFhyo5_012::dsp_cls(unsigned short Id, short Index)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	memset(DiagData.data_val, '\0', MONY_BUF_SIZE);
	diag_setdata(Id, Index, (struct IUnknown*)&DiagData, CIcsdiagctrl);
}

//========================================
// 各入力データ抽出
//========================================
void CShinFhyo5_012::GetAllDiagData()
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// 4×みなし仕入率
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_20X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_20X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_20D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_20D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_20E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_20E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_20F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_20F, DiagData.data_val, MONY_BUF_SIZE);

	// 4×みなし仕入率
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_21X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_21X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_21D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_21D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_21E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_21E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_21F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_21F, DiagData.data_val, MONY_BUF_SIZE);

	// 第一種事業及び第二種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_22X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_22X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_22D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_22D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_22E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_22E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_22F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_22F, DiagData.data_val, MONY_BUF_SIZE);

	// 第一種事業及び第三種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_23X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_23X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_23D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_23D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_23E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_23E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_23F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_23F, DiagData.data_val, MONY_BUF_SIZE);

	// 第一種事業及び第四種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_24X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_24X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_24D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_24D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_24E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_24E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_24F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_24F, DiagData.data_val, MONY_BUF_SIZE);

	// 第一種事業及び第五種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_25X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_25X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_25D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_25D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_25E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_25E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_25F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_25F, DiagData.data_val, MONY_BUF_SIZE);

	// 第一種事業及び第六種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_26X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_26X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_26D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_26D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_26E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_26E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_26F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_26F, DiagData.data_val, MONY_BUF_SIZE);

	// 第二種事業及び第三種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_27X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_27X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_27D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_27D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_27E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_27E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_27F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_27F, DiagData.data_val, MONY_BUF_SIZE);

	// 第二種事業及び第四種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_28X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_28X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_28D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_28D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_28E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_28E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_28F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_28F, DiagData.data_val, MONY_BUF_SIZE);

	// 第二種事業及び第五種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_29X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_29X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_29D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_29D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_29E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_29E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_29F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_29F, DiagData.data_val, MONY_BUF_SIZE);

	// 第二種事業及び第六種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_30X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_30X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_30D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_30D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_30E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_30E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_30F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_30F, DiagData.data_val, MONY_BUF_SIZE);

	// 第三種事業及び第四種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_31X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_31X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_31D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_31D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_31E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_31E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_31F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_31F, DiagData.data_val, MONY_BUF_SIZE);

	// 第三種事業及び第五種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_32X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_32X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_32D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_32D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_32E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_32E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_32F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_32F, DiagData.data_val, MONY_BUF_SIZE);

	// 第三種事業及び第六種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_33X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_33X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_33D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_33D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_33E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_33E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_33F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_33F, DiagData.data_val, MONY_BUF_SIZE);

	// 第四種事業及び第五種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_34X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_34X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_34D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_34D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_34E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_34E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_34F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_34F, DiagData.data_val, MONY_BUF_SIZE);

	// 第四種事業及び第六種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_35X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_35X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_35D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_35D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_35E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_35E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_35F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_35F, DiagData.data_val, MONY_BUF_SIZE);

	// 第五種事業及び第六種事業
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_36X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_36X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_36D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_36D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_36E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_36E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_36F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_36F, DiagData.data_val, MONY_BUF_SIZE);

	// 選択可能な計算式区分の内から選択した金額
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_37X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_37X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_37D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_37D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_37E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_37E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID5012_37F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F5012_37F, DiagData.data_val, MONY_BUF_SIZE);
}

BOOL CShinFhyo5_012::PreTranslateMessage(MSG* pMsg)
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

int CShinFhyo5_012::SyukeiDataSet(int cID)
{
	char			val[MONY_BUF_SIZE] = {0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	diag_getdata(IDC_ICSDIAG8CTRL1, cID, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove( val, DiagData.data_val, MONY_BUF_SIZE );

	// 合計額出すのに必要な箇所をセット
	switch ( cID ) {
			// 原則計算を適用する場合　4×みなし仕入率
		case ID5012_20F:
			if (m_pArith->l_cmp(F5012_20F, val)) {
				F5012_20f |= 0x8000;
				memmove(F5012_20F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359000"), F5012_20F);
			}
			break;
		case ID5012_20D:
			if (m_pArith->l_cmp(F5012_20D, val)) {
				F5012_20f |= 0x80;
				memmove(F5012_20D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359002"), F5012_20D);
			}
			break;
		case ID5012_20E:
			if (m_pArith->l_cmp(F5012_20E, val)) {
				F5012_20f |= 0x800;
				memmove(F5012_20E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359003"), F5012_20E);
			}
			break;

			// 1種類の事業で75％以上　4×みなし仕入率
		case ID5012_21F:
			if (m_pArith->l_cmp(F5012_21F, val)) {
				F5012_21f |= 0x8000;
				memmove(F5012_21F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359100"), F5012_21F);
			}
			break;
		case ID5012_21D:
			if (m_pArith->l_cmp(F5012_21D, val)) {
				F5012_21f |= 0x80;
				memmove(F5012_21D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359102"), F5012_21D);
			}
			break;
		case ID5012_21E:
			if (m_pArith->l_cmp(F5012_21E, val)) {
				F5012_21f |= 0x800;
				memmove(F5012_21E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359103"), F5012_21E);
			}
			break;

			// 第一種事業及び第二種事業
		case ID5012_22F:
			if (m_pArith->l_cmp(F5012_22F, val)) {
				F5012_22f |= 0x8000;
				memmove(F5012_22F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359200"), F5012_22F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E001"), F5012_22F);
			}
			break;
		case ID5012_22D:
			if (m_pArith->l_cmp(F5012_22D, val)) {
				F5012_22f |= 0x80;
				memmove(F5012_22D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359202"), F5012_22D);
			}
			break;
		case ID5012_22E:
			if (m_pArith->l_cmp(F5012_22E, val)) {
				F5012_22f |= 0x800;
				memmove(F5012_22E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359203"), F5012_22E);
			}
			break;

			// 第一種事業及び第三種事業
		case ID5012_23F:
			if (m_pArith->l_cmp(F5012_23F, val)) {
				F5012_23f |= 0x8000;
				memmove(F5012_23F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359300"), F5012_23F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E002"), F5012_23F);
			}
			break;
		case ID5012_23D:
			if (m_pArith->l_cmp(F5012_23D, val)) {
				F5012_23f |= 0x80;
				memmove(F5012_23D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359302"), F5012_23D);
			}
			break;
		case ID5012_23E:
			if (m_pArith->l_cmp(F5012_23E, val)) {
				F5012_23f |= 0x800;
				memmove(F5012_23E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359303"), F5012_23E);
			}
			break;

			// 第一種事業及び第四種事業
		case ID5012_24F:
			if (m_pArith->l_cmp(F5012_24F, val)) {
				F5012_24f |= 0x8000;
				memmove(F5012_24F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359400"), F5012_24F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E003"), F5012_24F);
			}
			break;
		case ID5012_24D:
			if (m_pArith->l_cmp(F5012_24D, val)) {
				F5012_24f |= 0x80;
				memmove(F5012_24D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359402"), F5012_24D);
			}
			break;
		case ID5012_24E:
			if (m_pArith->l_cmp(F5012_24E, val)) {
				F5012_24f |= 0x800;
				memmove(F5012_24E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359403"), F5012_24E);
			}
			break;

			// 第一種事業及び第五種事業
		case ID5012_25F:
			if (m_pArith->l_cmp(F5012_25F, val)) {
				F5012_25f |= 0x8000;
				memmove(F5012_25F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359500"), F5012_25F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E004"), F5012_25F);
			}
			break;
		case ID5012_25D:
			if (m_pArith->l_cmp(F5012_25D, val)) {
				F5012_25f |= 0x80;
				memmove(F5012_25D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359502"), F5012_25D);
			}
			break;
		case ID5012_25E:
			if (m_pArith->l_cmp(F5012_25E, val)) {
				F5012_25f |= 0x800;
				memmove(F5012_25E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359503"), F5012_25E);
			}
			break;

			// 第一種事業及び第六種事業
		case ID5012_26F:
			if (m_pArith->l_cmp(F5012_26F, val)) {
				//F5012_26f |= 0x80;
				F5012_26f |= 0x8000;
				memmove(F5012_26F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359600"), F5012_26F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E005"), F5012_26F);
			}
			break;
		case ID5012_26D:
			if (m_pArith->l_cmp(F5012_26D, val)) {
				F5012_26f |= 0x80;
				memmove(F5012_26D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359602"), F5012_26D);
			}
			break;
		case ID5012_26E:
			if (m_pArith->l_cmp(F5012_26E, val)) {
				F5012_26f |= 0x800;
				memmove(F5012_26E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359603"), F5012_26E);
			}
			break;

			// 第二種事業及び第三種事業
		case ID5012_27F:
			if (m_pArith->l_cmp(F5012_27F, val)) {
				F5012_27f |= 0x8000;
				memmove(F5012_27F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359700"), F5012_27F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E006"), F5012_27F);
			}
			break;
		case ID5012_27D:
			if (m_pArith->l_cmp(F5012_27D, val)) {
				F5012_27f |= 0x80;
				memmove(F5012_27D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359702"), F5012_27D);
			}
			break;
		case ID5012_27E:
			if (m_pArith->l_cmp(F5012_27E, val)) {
				F5012_27f |= 0x800;
				memmove(F5012_27E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359703"), F5012_27E);
			}
			break;

			// 第二種事業及び第四種事業
		case ID5012_28F:
			if (m_pArith->l_cmp(F5012_28F, val)) {
				F5012_28f |= 0x8000;
				memmove(F5012_28F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359800"), F5012_28F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E007"), F5012_28F);
			}
			break;
		case ID5012_28D:
			if (m_pArith->l_cmp(F5012_28D, val)) {
				F5012_28f |= 0x80;
				memmove(F5012_28D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359802"), F5012_28D);
			}
			break;
		case ID5012_28E:
			if (m_pArith->l_cmp(F5012_28E, val)) {
				F5012_28f |= 0x800;
				memmove(F5012_28E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359803"), F5012_28E);
			}
			break;

			// 第二種事業及び第五種事業
		case ID5012_29F:
			if (m_pArith->l_cmp(F5012_29F, val)) {
				F5012_29f |= 0x8000;
				memmove(F5012_29F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359900"), F5012_29F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E008"), F5012_29F);
			}
			break;
		case ID5012_29D:
			if (m_pArith->l_cmp(F5012_29D, val)) {
				F5012_29f |= 0x80;
				memmove(F5012_29D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359902"), F5012_29D);
			}
			break;
		case ID5012_29E:
			if (m_pArith->l_cmp(F5012_29E, val)) {
				F5012_29f |= 0x800;
				memmove(F5012_29E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("359903"), F5012_29E);
			}
			break;


			// 第二種事業及び第六種事業
		case ID5012_30F:
			if (m_pArith->l_cmp(F5012_30F, val)) {
				F5012_30f |= 0x8000;
				memmove(F5012_30F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A000"), F5012_30F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E009"), F5012_30F);
			}
			break;
		case ID5012_30D:
			if (m_pArith->l_cmp(F5012_30D, val)) {
				F5012_30f |= 0x80;
				memmove(F5012_30D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A002"), F5012_30D);
			}
			break;
		case ID5012_30E:
			if (m_pArith->l_cmp(F5012_30E, val)) {
				F5012_30f |= 0x800;
				memmove(F5012_30E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A003"), F5012_30E);
			}
			break;


			// 第三種事業及び第四種事業
		case ID5012_31F:
			if (m_pArith->l_cmp(F5012_31F, val)) {
				F5012_31f |= 0x8000;
				memmove(F5012_31F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A100"), F5012_31F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E00A"), F5012_31F);
			}
			break;
		case ID5012_31D:
			if (m_pArith->l_cmp(F5012_31D, val)) {
				F5012_31f |= 0x80;
				memmove(F5012_31D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A102"), F5012_31D);
			}
			break;
		case ID5012_31E:
			if (m_pArith->l_cmp(F5012_31E, val)) {
				F5012_31f |= 0x800;
				memmove(F5012_31E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A103"), F5012_31E);
			}
			break;

			// 第三種事業及び第五種事業
		case ID5012_32F:
			if (m_pArith->l_cmp(F5012_32F, val)) {
				F5012_32f |= 0x8000;
				memmove(F5012_32F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A200"), F5012_32F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E00B"), F5012_32F);
			}
			break;
		case ID5012_32D:
			if (m_pArith->l_cmp(F5012_32D, val)) {
				F5012_32f |= 0x80;
				memmove(F5012_32D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A202"), F5012_32D);
			}
			break;
		case ID5012_32E:
			if (m_pArith->l_cmp(F5012_32E, val)) {
				F5012_32f |= 0x800;
				memmove(F5012_32E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A203"), F5012_32E);
			}
			break;

			// 第三種事業及び第六種事業
		case ID5012_33F:
			if (m_pArith->l_cmp(F5012_33F, val)) {
				F5012_33f |= 0x8000;
				memmove(F5012_33F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A300"), F5012_33F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E00C"), F5012_33F);
			}
			break;
		case ID5012_33D:
			if (m_pArith->l_cmp(F5012_33D, val)) {
				F5012_33f |= 0x80;
				memmove(F5012_33D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A302"), F5012_33D);
			}
			break;
		case ID5012_33E:
			if (m_pArith->l_cmp(F5012_33E, val)) {
				F5012_33f |= 0x800;
				memmove(F5012_33E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A303"), F5012_33E);
			}
			break;

			// 第四種事業及び第五種事業
		case ID5012_34F:
			if (m_pArith->l_cmp(F5012_34F, val)) {
				F5012_34f |= 0x8000;
				memmove(F5012_34F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A400"), F5012_34F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E00D"), F5012_34F);
			}
			break;
		case ID5012_34D:
			if (m_pArith->l_cmp(F5012_34D, val)) {
				F5012_34f |= 0x80;
				memmove(F5012_34D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A402"), F5012_34D);
			}
			break;
		case ID5012_34E:
			if (m_pArith->l_cmp(F5012_34E, val)) {
				F5012_34f |= 0x800;
				memmove(F5012_34E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A403"), F5012_34E);
			}
			break;

			// 第四種事業及び第六種事業
		case ID5012_35F:
			if (m_pArith->l_cmp(F5012_35F, val)) {
				F5012_35f |= 0x8000;
				memmove(F5012_35F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A500"), F5012_35F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E00E"), F5012_35F);
			}
			break;
		case ID5012_35D:
			if (m_pArith->l_cmp(F5012_35D, val)) {
				F5012_35f |= 0x80;
				memmove(F5012_35D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A502"), F5012_35D);
			}
			break;
		case ID5012_35E:
			if (m_pArith->l_cmp(F5012_35E, val)) {
				F5012_35f |= 0x800;
				memmove(F5012_35E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A503"), F5012_35E);
			}
			break;

			// 第五種事業及び第六種事業
		case ID5012_36F:
			if (m_pArith->l_cmp(F5012_36F, val)) {
				F5012_36f |= 0x8000;
				memmove(F5012_36F, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A600"), F5012_36F);
				//(*m_pSyzSyukei)->SetShinkokuData(_T("35E00F"), F5012_36F);
			}
			break;
		case ID5012_36D:
			if (m_pArith->l_cmp(F5012_36D, val)) {
				F5012_36f |= 0x80;
				memmove(F5012_36D, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A602"), F5012_36D);
			}
			break;
		case ID5012_36E:
			if (m_pArith->l_cmp(F5012_36E, val)) {
				F5012_36f |= 0x800;
				memmove(F5012_36E, val, MONY_BUF_SIZE);
				(*m_pSyzSyukei)->SetShinkokuData(_T("35A603"), F5012_36E);
			}
			break;
	}

	CalqGokei ( cID );

	return MemoryReget();
}

int CShinFhyo5_012::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	MoneyBasejagArray	money;

	m_money35 = (*m_pSyzSyukei)->GetShinkokuData( _T("35") );

	money = m_money35;

	int cnt = money.GetCount();
	int incnt = 0;

	for (int i = 0; i < cnt; i++ ) {
		incnt = money[i].GetCount();
		for (int j = 0; j < incnt; j++ ) {
			// 原則計算を適用する場合　4×みなし仕入率
			if (money[i][j].code == _T("359001")) {
				memmove(F5012_20X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359002")) {
				memmove(F5012_20D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359003")) {
				memmove(F5012_20E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if ( money[i][j].code == _T("359000") ) {
				memmove(F5012_20F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 1種類の事業で75%以上　4×みなし仕入率
			if (money[i][j].code == _T("359101")) {
				memmove(F5012_21X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359102")) {
				memmove(F5012_21D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359103")) {
				memmove(F5012_21E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359100")) {
				memmove(F5012_21F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第一種事業及び第二種事業
			if (money[i][j].code == _T("359201")) {
				memmove(F5012_22X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359202")) {
				memmove(F5012_22D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359203")) {
				memmove(F5012_22E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359200")) {
				memmove(F5012_22F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第一種事業及び第三種事業
			if (money[i][j].code == _T("359301")) {
				memmove(F5012_23X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359302")) {
				memmove(F5012_23D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359303")) {
				memmove(F5012_23E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359300")) {
				memmove(F5012_23F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第一種事業及び第四種事業
			if (money[i][j].code == _T("359401")) {
				memmove(F5012_24X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359402")) {
				memmove(F5012_24D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359403")) {
				memmove(F5012_24E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359400")) {
				memmove(F5012_24F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第一種事業及び第五種事業
			if (money[i][j].code == _T("359501")) {
				memmove(F5012_25X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359502")) {
				memmove(F5012_25D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359503")) {
				memmove(F5012_25E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359500")) {
				memmove(F5012_25F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第一種事業及び第六種事業
			if (money[i][j].code == _T("359601")) {
				memmove(F5012_26X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359602")) {
				memmove(F5012_26D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359603")) {
				memmove(F5012_26E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359600")) {
				memmove(F5012_26F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第二種事業及び第三種事業
			if (money[i][j].code == _T("359701")) {
				memmove(F5012_27X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359702")) {
				memmove(F5012_27D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359703")) {
				memmove(F5012_27E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359700")) {
				memmove(F5012_27F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第二種事業及び第四種事業
			if (money[i][j].code == _T("359801")) {
				memmove(F5012_28X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359802")) {
				memmove(F5012_28D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359803")) {
				memmove(F5012_28E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359800")) {
				memmove(F5012_28F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第二種事業及び第五種事業
			if (money[i][j].code == _T("359901")) {
				memmove(F5012_29X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359902")) {
				memmove(F5012_29D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359903")) {
				memmove(F5012_29E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("359900")) {
				memmove(F5012_29F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第二種事業及び第六種事業
			if (money[i][j].code == _T("35A001")) {
				memmove(F5012_30X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A002")) {
				memmove(F5012_30D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A003")) {
				memmove(F5012_30E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A000")) {
				memmove(F5012_30F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第三種事業及び第四種事業
			if (money[i][j].code == _T("35A101")) {
				memmove(F5012_31X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A102")) {
				memmove(F5012_31D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A103")) {
				memmove(F5012_31E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A100")) {
				memmove(F5012_31F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第三種事業及び第五種事業
			if (money[i][j].code == _T("35A201")) {
				memmove(F5012_32X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A202")) {
				memmove(F5012_32D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A203")) {
				memmove(F5012_32E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A200")) {
				memmove(F5012_32F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第三種事業及び第六種事業
			if (money[i][j].code == _T("35A301")) {
				memmove(F5012_33X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A302")) {
				memmove(F5012_33D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A303")) {
				memmove(F5012_33E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A300")) {
				memmove(F5012_33F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第四種事業及び第五種事業
			if (money[i][j].code == _T("35A401")) {
				memmove(F5012_34X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A402")) {
				memmove(F5012_34D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A403")) {
				memmove(F5012_34E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A400")) {
				memmove(F5012_34F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第四種事業及び第六種事業
			if (money[i][j].code == _T("35A501")) {
				memmove(F5012_35X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A502")) {
				memmove(F5012_35D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A503")) {
				memmove(F5012_35E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A500")) {
				memmove(F5012_35F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 第五種事業及び第六種事業
			if (money[i][j].code == _T("35A601")) {
				memmove(F5012_36X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A602")) {
				memmove(F5012_36D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A603")) {
				memmove(F5012_36E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A600")) {
				memmove(F5012_36F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 選択可能な計算式区分のうちから選択した金額
			if (money[i][j].code == _T("35A701")) {
				memmove(F5012_37X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A702")) {
				memmove(F5012_37D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A703")) {
				memmove(F5012_37E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("35A700")) {
				memmove(F5012_37F, money[i][j].arith, MONY_BUF_SIZE);
			}
		}
	}

	int		gensoku;
	char	buff[256];
	money = (*m_pSyzSyukei)->GetShinkokuData(_T("362007"));
	m_pArith->l_print(buff, money[0][0].arith, _T("SSSS"));
	gensoku = atoi(buff);
	if ( gensoku )
		( *m_pSnHeadData )->Sn_TOKUR = 0;
	else
		( *m_pSnHeadData )->Sn_TOKUR = 1;

	return 0;
}

void CShinFhyo5_012::ReadData2()
{
	//if ( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ) {	// 非連動の場合
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {	// 非連動、入力ロックの場合
		if( F5012_20f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359002"), F5012_20D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359002"));
		if( F5012_20f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359003"), F5012_20E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359003"));
		if( F5012_20f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359000"), F5012_20F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359000"));
		if( F5012_21f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359102"), F5012_21D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359102"));
		if( F5012_21f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359103"), F5012_21E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359103"));
		if( F5012_21f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359100"), F5012_21F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359100"));
		if( F5012_22f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359202"), F5012_22D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359202"));
		if( F5012_22f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359203"), F5012_22E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359203"));
		if( F5012_22f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359200"), F5012_22F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359200"));
		if( F5012_23f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359302"), F5012_23D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359302"));
		if( F5012_23f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359303"), F5012_23E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359303"));
		if( F5012_23f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359300"), F5012_23F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359300"));
		if( F5012_24f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359402"), F5012_24D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359402"));
		if( F5012_24f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359403"), F5012_24E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359403"));
		if( F5012_24f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359400"), F5012_24F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359400"));
		if( F5012_25f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359502"), F5012_25D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359502"));
		if( F5012_25f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359503"), F5012_25E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359503"));
		if( F5012_25f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359500"), F5012_25F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359500"));
		if( F5012_26f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359602"), F5012_26D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359602"));
		if( F5012_26f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359603"), F5012_26E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359603"));
		if( F5012_26f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359600"), F5012_26F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359600"));
		if( F5012_27f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359702"), F5012_27D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359702"));
		if( F5012_27f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359703"), F5012_27E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359703"));
		if( F5012_27f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359700"), F5012_27F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359700"));
		if( F5012_28f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359802"), F5012_28D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359802"));
		if( F5012_28f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359803"), F5012_28E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359803"));
		if( F5012_28f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359800"), F5012_28F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359800"));
		if( F5012_29f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359902"), F5012_29D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359902"));
		if( F5012_29f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359903"), F5012_29E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359903"));
		if( F5012_29f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("359900"), F5012_29F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359900"));
		if( F5012_30f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A002"), F5012_30D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A002"));
		if( F5012_30f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A003"), F5012_30E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A003"));
		if( F5012_30f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A000"), F5012_30F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A000"));
		if( F5012_31f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A102"), F5012_31D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A102"));
		if( F5012_31f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A103"), F5012_31E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A103"));
		if( F5012_31f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A100"), F5012_31F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A100"));
		if( F5012_32f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A202"), F5012_32D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A202"));
		if( F5012_32f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A203"), F5012_32E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A203"));
		if( F5012_32f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A200"), F5012_32F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A200"));
		if( F5012_33f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A302"), F5012_33D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A302"));
		if( F5012_33f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A303"), F5012_33E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A303"));
		if( F5012_33f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A300"), F5012_33F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A300"));
		if( F5012_34f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A402"), F5012_34D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A402"));
		if( F5012_34f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A403"), F5012_34E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A403"));
		if( F5012_34f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A400"), F5012_34F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A400"));
		if( F5012_35f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A502"), F5012_35D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A502"));
		if( F5012_35f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A503"), F5012_35E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A503"));
		if( F5012_35f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A500"), F5012_35F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A500"));
		if( F5012_36f&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A602"), F5012_36D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A602"));
		if( F5012_36f&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A603"), F5012_36E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A603"));
		if( F5012_36f&0x8000 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A600"), F5012_36F);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A600"));


		int inputSign = 0;
		GetInputSign ( ID_F521, inputSign );
		if ( ( F5012_21f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E000" ), F5012_21F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E000" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F522, inputSign );
		if ( ( F5012_22f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E001" ), F5012_22F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E001" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F523, inputSign );
		if ( ( F5012_23f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E002" ), F5012_23F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E002" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F524, inputSign );
		if ( ( F5012_24f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E003" ), F5012_24F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E003" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F525, inputSign );
		if ( ( F5012_25f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E004" ), F5012_25F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E004" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F526, inputSign );
		if ( ( F5012_26f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E005" ), F5012_26F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E005" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F527, inputSign );
		if ( ( F5012_27f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E006" ), F5012_27F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E006" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F528, inputSign );
		if ( ( F5012_28f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E007" ), F5012_28F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E007" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F529, inputSign );
		if ( ( F5012_29f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E008" ), F5012_29F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E008" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F530, inputSign );
		if ( ( F5012_30f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E009" ), F5012_30F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E009" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F531, inputSign );
		if ( ( F5012_31f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00A" ), F5012_31F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00A" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F532, inputSign );
		if ( ( F5012_32f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00B" ), F5012_32F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00B" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F533, inputSign );
		if ( ( F5012_33f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00C" ), F5012_33F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00C" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F534, inputSign );
		if ( ( F5012_34f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00D" ), F5012_34F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00D" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F535, inputSign );
		if ( ( F5012_35f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00E" ), F5012_35F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00E" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F536, inputSign );
		if ( ( F5012_36f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "35E00F" ), F5012_36F );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00F" ) );
		}

		if ((*m_pSnHeadData)->SVmzsw == 1) {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359001"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359101"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359201"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359301"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359401"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359501"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359601"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359701"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359801"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("359901"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A001"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A101"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A201"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A301"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A401"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A501"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A601"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("35A701"));
		}
		else {
			// 経過措置なし　旧税率合計を０クリア
			char mony[MONY_BUF_SIZE] = { 0 };
			memset(mony, '\0', MONY_BUF_SIZE);

			(*m_pSyzSyukei)->SetShinkokuData(_T("359001"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("359101"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("359201"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("359301"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("359401"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("359501"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("359601"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("359701"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("359801"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("359901"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A001"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A101"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A201"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A301"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A401"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A501"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A601"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("35A701"), mony);
		}
		
	}
	else {
		
	}

	//if ( ( *m_pSnHeadData )->Sn_GenericSgn & 0x08 ) {
	//	SetKjCode();
	//}
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
int CShinFhyo5_012::IsEnableItems(int reflg, short& Idx, CString& code, int& typeCnt)
{
	MoneyBasejagArray	money;
	//char	val[MONY_BUF_SIZE],
	char		max_valttl[MONY_BUF_SIZE] = { 0 };
	char	buff[256];
	//short	id, 
	short	maxid = 0;
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
				id = ID5012_21F;
			}
			// 第一種事業及び第二種事業
			else if (money[i][j].code == _T("35E001")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_22F;
			}
			// 第一種事業及び第三種事業
			else if (money[i][j].code == _T("35E002")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_23F;
			}
			// 第一種事業及び第四種事業
			else if (money[i][j].code == _T("35E003")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_24F;
			}
			// 第一種事業及び第五種事業
			else if (money[i][j].code == _T("35E004")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_25F;
			}
			// 第一種事業及び第六種事業
			else if (money[i][j].code == _T("35E005")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_26F;
			}
			// 第二種事業及び第三種事業
			else if (money[i][j].code == _T("35E006")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_27F;
			}
			// 第二種事業及び第四種事業
			else if (money[i][j].code == _T("35E007")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_28F;
			}
			// 第二種事業及び第五種事業
			else if (money[i][j].code == _T("35E008")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_29F;
			}
			// 第二種事業及び第六種事業
			else if (money[i][j].code == _T("35E009")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_30F;
			}
			// 第三種事業及び第四種事業
			else if (money[i][j].code == _T("35E00A")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_31F;
			}
			// 第三種事業及び第五種事業
			else if (money[i][j].code == _T("35E00B")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_32F;
			}
			// 第三種事業及び第六種事業
			else if (money[i][j].code == _T("35E00C")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_33F;
			}
			// 第四種事業及び第五種事業
			else if (money[i][j].code == _T("35E00D")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_34F;
			}
			// 第四種事業及び第六種事業
			else if (money[i][j].code == _T("35E00E")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_35F;
			}
			// 第五種事業及び第六種事業
			else if (money[i][j].code == _T("35E00F")) {
				memmove(val, money[i][j].arith, MONY_BUF_SIZE);
				id = ID5012_36F;
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
		Idx = ID5012_21F;
		code = _T("35E000");
		break;
	case 2:
		Idx = ID5012_22F;
		code = _T("35E001");
		break;
	case 3:
		Idx = ID5012_23F;
		code = _T("35E002");
		break;
	case 4:
		Idx = ID5012_24F;
		code = _T("35E003");
		break;
	case 5:
		Idx = ID5012_25F;
		code = _T("35E004");
		break;
	case 6:
		Idx = ID5012_26F;
		code = _T("35E005");
		break;
	case 7:
		Idx = ID5012_27F;
		code = _T("35E006");
		break;
	case 8:
		Idx = ID5012_28F;
		code = _T("35E007");
		break;
	case 9:
		Idx = ID5012_29F;
		code = _T("35E008");
		break;
	case 10:
		Idx = ID5012_30F;
		code = _T("35E009");
		break;
	case 11:
		Idx = ID5012_31F;
		code = _T("35E00A");
		break;
	case 12:
		Idx = ID5012_32F;
		code = _T("35E00B");
		break;
	case 13:
		Idx = ID5012_33F;
		code = _T("35E00C");
		break;
	case 14:
		Idx = ID5012_34F;
		code = _T("35E00D");
		break;
	case 15:
		Idx = ID5012_35F;
		code = _T("35E00E");
		break;
	case 16:
		Idx = ID5012_36F;
		code = _T("35E00F");
		break;
	default:
		if (typeCnt >= 2) {
			Idx = ID5012_20F;
			code.Empty();
		}
		else {
			Idx = 0;
			code.Empty();
		}
	}

	F5012_21kbn = 0;
	//if (Idx == ID5012_21F) {
	if ( IsDispArea ( ID5012_21D ) ) {
		int minashi;
		money = (*m_pSyzSyukei)->GetShinkokuData(_T("362002"));
		m_pArith->l_print(buff, money[0][0].arith, _T("SSSS"));
		minashi = atoi(buff);
		switch (minashi) {
		case 9000:
			F5012_21kbn = 1;
			break;
		case 8000:
			F5012_21kbn = 2;
			break;
		case 7000:
			F5012_21kbn = 3;
			break;
		case 6000:
			F5012_21kbn = 4;
			break;
		case 5000:
			F5012_21kbn = 5;
			break;
		case 4000:
			F5012_21kbn = 6;
			break;
		default:
			F5012_21kbn = 0;
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
int CShinFhyo5_012::Gokeiclctype()
{
	MoneyBasejagArray	money;
	money = (*m_pSyzSyukei)->GetShinkokuData( _T("362003") );

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
int CShinFhyo5_012::Is4Val(int reflg)
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
BOOL CShinFhyo5_012::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( ( F5012_20f & 0x8880 ) || ( F5012_21f & 0x8880 ) || ( F5012_22f & 0x8880 ) || ( F5012_23f & 0x8880 ) ||
			 ( F5012_24f & 0x8880 ) || ( F5012_25f & 0x8880 ) || ( F5012_26f & 0x8880 ) || ( F5012_27f & 0x8880 ) ||
			 ( F5012_28f & 0x8880 ) || ( F5012_29f & 0x8880 ) || ( F5012_30f & 0x8880 ) || ( F5012_31f & 0x8880 ) ||
			 ( F5012_32f & 0x8880 ) || ( F5012_33f & 0x8880 ) || ( F5012_34f & 0x8880 ) || ( F5012_35f & 0x8880 ) ||
			 ( F5012_36f & 0x8880 ) ) {
			ret = TRUE;
		}
	}

	return ret;
}

//=========================================================
// 表示項目？
//---------------------------------------------------------
// 引数
//			idx			：	ダイアグラム内インデックス
//---------------------------------------------------------
// 返送値
//			0			：	非表示項目
//			1			：	表示項目
//			-1			：	エラー
//=========================================================
int CShinFhyo5_012::IsDispArea ( short index )
{
	int ret = 0;

	//int id1 = 0, id2 = 0;
	int sw = 0;		// 1 = 第一種
	int *pDispSign = NULL;
	bool getFlg = false;

	//unsigned short uWari = 0, tmpWari1 = 0, tmpWari2 = 0;

	//char tmpMony[MONY_BUF_SIZE] = { 0 }, tmpMony2[MONY_BUF_SIZE] = { 0 };

	if( m_id == 0 ) {
		return 0;
	}

	switch ( index ) {
	case ID5012_21D:
	case ID5012_21E:
	case ID5012_21F:
		if ( index == ID5012_21F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		sw = 1;
		pDispSign = &F5012_21Disp;
		getFlg = true;
		break;

	case ID5012_22D:
	case ID5012_22E:
	case ID5012_22F:
		if ( index == ID5012_22F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAINISHU;
		pDispSign = &F5012_22Disp;
		getFlg = true;
		break;

	case ID5012_23D:
	case ID5012_23E:
	case ID5012_23F:
		if ( index == ID5012_23F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAISANSHU;
		pDispSign = &F5012_23Disp;
		getFlg = true;
		break;

	case ID5012_24D:
	case ID5012_24E:
	case ID5012_24F:
		if ( index == ID5012_24F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAIYONSHU;
		pDispSign = &F5012_24Disp;
		getFlg = true;
		break;

	case ID5012_25D:
	case ID5012_25E:
	case ID5012_25F:
		if ( index == ID5012_25F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAIGOSHU;
		pDispSign = &F5012_25Disp;
		getFlg = true;
		break;

	case ID5012_26D:
	case ID5012_26E:
	case ID5012_26F:
		if ( index == ID5012_26F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIICHISHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5012_26Disp;
		getFlg = true;
		break;

	case ID5012_27D:
	case ID5012_27E:
	case ID5012_27F:
		if ( index == ID5012_27F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAINISHU;
		//id2 = DAISANSHU;
		pDispSign = &F5012_27Disp;
		getFlg = true;
		break;

	case ID5012_28D:
	case ID5012_28E:
	case ID5012_28F:
		if ( index == ID5012_28F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAINISHU;
		//id2 = DAIYONSHU;
		pDispSign = &F5012_28Disp;
		getFlg = true;
		break;

	case ID5012_29D:
	case ID5012_29E:
	case ID5012_29F:
		if ( index == ID5012_29F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAINISHU;
		//id2 = DAIGOSHU;
		pDispSign = &F5012_29Disp;
		getFlg = true;
		break;

	case ID5012_30D:
	case ID5012_30E:
	case ID5012_30F:
		if ( index == ID5012_30F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAINISHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5012_30Disp;
		getFlg = true;
		break;

	case ID5012_31D:
	case ID5012_31E:
	case ID5012_31F:
		if ( index == ID5012_31F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAISANSHU;
		//id2 = DAIYONSHU;
		pDispSign = &F5012_31Disp;
		getFlg = true;
		break;

	case ID5012_32D:
	case ID5012_32E:
	case ID5012_32F:
		if ( index == ID5012_32F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAISANSHU;
		//id2 = DAIGOSHU;
		pDispSign = &F5012_32Disp;
		getFlg = true;
		break;

	case ID5012_33D:
	case ID5012_33E:
	case ID5012_33F:
		if ( index == ID5012_33F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAISANSHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5012_33Disp;
		getFlg = true;
		break;

	case ID5012_34D:
	case ID5012_34E:
	case ID5012_34F:
		if ( index == ID5012_34F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIYONSHU;
		//id2 = DAIGOSHU;
		pDispSign = &F5012_34Disp;
		getFlg = true;
		break;

	case ID5012_35D:
	case ID5012_35E:
	case ID5012_35F:
		if ( index == ID5012_35F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIYONSHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5012_35Disp;
		getFlg = true;
		break;

	case ID5012_36D:
	case ID5012_36E:
	case ID5012_36F:
		if ( index == ID5012_36F ) {
			if ( !( m_flg & 0x01 ) ) {
				break;
			}
		}

		//id1 = DAIGOSHU;
		//id2 = DAIROKUSHU;
		pDispSign = &F5012_36Disp;
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
void CShinFhyo5_012::ClearInput ( short index )
{
	int inputSign = 0;
	switch ( index ) {
	case ID5012_21D:
	case ID5012_21E:
	case ID5012_21F:
		if ( index == ID5012_21D ) {
			F5012_21f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359102" ) );
		}
		else if ( index == ID5012_21E ) {
			F5012_21f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359103" ) );
		}
		else if ( index == ID5012_21F ) {
			F5012_21f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359100" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F521, inputSign );
		if ( !( F5012_21f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E000" ) );
		}
		break;

	case ID5012_22D:
	case ID5012_22E:
	case ID5012_22F:
		if ( index == ID5012_22D ) {
			F5012_22f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359202" ) );
		}
		else if ( index == ID5012_22E ) {
			F5012_22f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359203" ) );
		}
		else if ( index == ID5012_22F ) {
			F5012_22f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359200" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F522, inputSign );
		if ( !( F5012_22f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E001" ) );
		}
		break;

	case ID5012_23D:
	case ID5012_23E:
	case ID5012_23F:
		if ( index == ID5012_23D ) {
			F5012_23f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359302" ) );
		}
		else if ( index == ID5012_23E ) {
			F5012_23f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359303" ) );
		}
		else if ( index == ID5012_23F ) {
			F5012_23f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359300" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F523, inputSign );
		if ( !( F5012_23f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E002" ) );
		}
		break;

	case ID5012_24D:
	case ID5012_24E:
	case ID5012_24F:
		if ( index == ID5012_24D ) {
			F5012_24f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359402" ) );
		}
		else if ( index == ID5012_24E ) {
			F5012_24f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359403" ) );
		}
		else if ( index == ID5012_24F ) {
			F5012_24f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359400" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F524, inputSign );
		if ( !( F5012_24f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E003" ) );
		}
		break;

	case ID5012_25D:
	case ID5012_25E:
	case ID5012_25F:
		if ( index == ID5012_25D ) {
			F5012_25f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359502" ) );
		}
		else if ( index == ID5012_25E ) {
			F5012_25f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359503" ) );
		}
		else if ( index == ID5012_25F ) {
			F5012_25f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359500" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F525, inputSign );
		if ( !( F5012_25f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E004" ) );
		}
		break;

	case ID5012_26D:
	case ID5012_26E:
	case ID5012_26F:
		if ( index == ID5012_26D ) {
			F5012_26f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359602" ) );
		}
		else if ( index == ID5012_26E ) {
			F5012_26f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359603" ) );
		}
		else if ( index == ID5012_26F ) {
			F5012_26f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359600" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F526, inputSign );
		if ( !( F5012_26f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E005" ) );
		}
		break;

	case ID5012_27D:
	case ID5012_27E:
	case ID5012_27F:
		if ( index == ID5012_27D ) {
			F5012_27f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359702" ) );
		}
		else if ( index == ID5012_27E ) {
			F5012_27f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359703" ) );
		}
		else if ( index == ID5012_27F ) {
			F5012_27f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359700" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F527, inputSign );
		if ( !( F5012_27f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E006" ) );
		}
		break;

	case ID5012_28D:
	case ID5012_28E:
	case ID5012_28F:
		if ( index == ID5012_28D ) {
			F5012_28f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359802" ) );
		}
		else if ( index == ID5012_28E ) {
			F5012_28f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359803" ) );
		}
		else if ( index == ID5012_28F ) {
			F5012_28f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359800" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F528, inputSign );
		if ( !( F5012_28f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E007" ) );
		}
		break;

	case ID5012_29D:
	case ID5012_29E:
	case ID5012_29F:
		if ( index == ID5012_29D ) {
			F5012_29f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359902" ) );
		}
		else if ( index == ID5012_29E ) {
			F5012_29f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359903" ) );
		}
		else if ( index == ID5012_29F ) {
			F5012_29f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "359900" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F529, inputSign );
		if ( !( F5012_29f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E008" ) );
		}
		break;

	case ID5012_30D:
	case ID5012_30E:
	case ID5012_30F:
		if ( index == ID5012_30D ) {
			F5012_30f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A002" ) );
		}
		else if ( index == ID5012_30E ) {
			F5012_30f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A003" ) );
		}
		else if ( index == ID5012_30F ) {
			F5012_30f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A000" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F530, inputSign );
		if ( !( F5012_30f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E009" ) );
		}
		break;

	case ID5012_31D:
	case ID5012_31E:
	case ID5012_31F:
		if ( index == ID5012_31D ) {
			F5012_31f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A102" ) );
		}
		else if ( index == ID5012_31E ) {
			F5012_31f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A103" ) );
		}
		else if ( index == ID5012_31F ) {
			F5012_31f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A100" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F531, inputSign );
		if ( !( F5012_31f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00A" ) );
		}
		break;

	case ID5012_32D:
	case ID5012_32E:
	case ID5012_32F:
		if ( index == ID5012_32D ) {
			F5012_32f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A202" ) );
		}
		else if ( index == ID5012_32E ) {
			F5012_32f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A203" ) );
		}
		else if ( index == ID5012_32F ) {
			F5012_32f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A200" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F532, inputSign );
		if ( !( F5012_32f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00B" ) );
		}
		break;

	case ID5012_33D:
	case ID5012_33E:
	case ID5012_33F:
		if ( index == ID5012_33D ) {
			F5012_33f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A302" ) );
		}
		else if ( index == ID5012_33E ) {
			F5012_33f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A303" ) );
		}
		else if ( index == ID5012_33F ) {
			F5012_33f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A300" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F533, inputSign );
		if ( !( F5012_33f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00C" ) );
		}
		break;

	case ID5012_34D:
	case ID5012_34E:
	case ID5012_34F:
		if ( index == ID5012_34D ) {
			F5012_34f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A402" ) );
		}
		else if ( index == ID5012_34E ) {
			F5012_34f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A403" ) );
		}
		else if ( index == ID5012_34F ) {
			F5012_34f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A400" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F534, inputSign );
		if ( !( F5012_34f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00D" ) );
		}
		break;

	case ID5012_35D:
	case ID5012_35E:
	case ID5012_35F:
		if ( index == ID5012_35D ) {
			F5012_35f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A502" ) );
		}
		else if ( index == ID5012_35E ) {
			F5012_35f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A503" ) );
		}
		else if ( index == ID5012_35F ) {
			F5012_35f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A500" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F535, inputSign );
		if ( !( F5012_35f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00E" ) );
		}
		break;

	case ID5012_36D:
	case ID5012_36E:
	case ID5012_36F:
		if ( index == ID5012_36D ) {
			F5012_36f &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A602" ) );
		}
		else if ( index == ID5012_36E ) {
			F5012_36f &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A603" ) );
		}
		else if ( index == ID5012_36F ) {
			F5012_36f &= ~0x8000;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35A600" ) );
		}

		inputSign = 0;
		GetInputSign ( ID_F536, inputSign );
		if ( !( F5012_36f & 0x8880 ) && !( inputSign & 0x8888 ) ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "35E00F" ) );
		}
		break;

	default:
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
int CShinFhyo5_012::CalqGokei ( int index )
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
	case ID5012_21D:
	case ID5012_21E:
	case ID5012_21F:
		inputSign = 0;
		GetInputSign ( ID_F521, inputSign );
		if ( ( F5012_21f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "359100" );
			setCode = _T( "35E000" );
		}
		break;

	case ID5012_22D:
	case ID5012_22E:
	case ID5012_22F:
		inputSign = 0;
		GetInputSign ( ID_F522, inputSign );
		if ( ( F5012_22f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "359200" );
			setCode = _T( "35E001" );
		}
		break;

	case ID5012_23D:
	case ID5012_23E:
	case ID5012_23F:
		inputSign = 0;
		GetInputSign ( ID_F523, inputSign );
		if ( ( F5012_23f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "359300" );
			setCode = _T( "35E002" );
		}
		break;

	case ID5012_24D:
	case ID5012_24E:
	case ID5012_24F:
		inputSign = 0;
		GetInputSign ( ID_F524, inputSign );
		if ( ( F5012_24f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "359400" );
			setCode = _T( "35E003" );
		}
		break;

	case ID5012_25D:
	case ID5012_25E:
	case ID5012_25F:
		inputSign = 0;
		GetInputSign ( ID_F525, inputSign );
		if ( ( F5012_25f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "359500" );
			setCode = _T( "35E004" );
		}
		break;

	case ID5012_26D:
	case ID5012_26E:
	case ID5012_26F:
		inputSign = 0;
		GetInputSign ( ID_F526, inputSign );
		if ( ( F5012_26f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "359600" );
			setCode = _T( "35E005" );
		}
		break;

	case ID5012_27D:
	case ID5012_27E:
	case ID5012_27F:
		inputSign = 0;
		GetInputSign ( ID_F527, inputSign );
		if ( ( F5012_27f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "359700" );
			setCode = _T( "35E006" );
		}
		break;

	case ID5012_28D:
	case ID5012_28E:
	case ID5012_28F:
		inputSign = 0;
		GetInputSign ( ID_F528, inputSign );
		if ( ( F5012_28f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "359800" );
			setCode = _T( "35E007" );
		}
		break;

	case ID5012_29D:
	case ID5012_29E:
	case ID5012_29F:
		inputSign = 0;
		GetInputSign ( ID_F529, inputSign );
		if ( ( F5012_29f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "359900" );
			setCode = _T( "35E008" );
		}
		break;

	case ID5012_30D:
	case ID5012_30E:
	case ID5012_30F:
		inputSign = 0;
		GetInputSign ( ID_F530, inputSign );
		if ( ( F5012_30f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "35A000" );
			setCode = _T( "35E009" );
		}
		break;

	case ID5012_31D:
	case ID5012_31E:
	case ID5012_31F:
		inputSign = 0;
		GetInputSign ( ID_F531, inputSign );
		if ( ( F5012_31f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "35A100" );
			setCode = _T( "35E00A" );
		}
		break;

	case ID5012_32D:
	case ID5012_32E:
	case ID5012_32F:
		inputSign = 0;
		GetInputSign ( ID_F532, inputSign );
		if ( ( F5012_32f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "35A200" );
			setCode = _T( "35E00B" );
		}
		break;

	case ID5012_33D:
	case ID5012_33E:
	case ID5012_33F:
		inputSign = 0;
		GetInputSign ( ID_F533, inputSign );
		if ( ( F5012_33f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "35A300" );
			setCode = _T( "35E00C" );
		}
		break;

	case ID5012_34D:
	case ID5012_34E:
	case ID5012_34F:
		inputSign = 0;
		GetInputSign ( ID_F534, inputSign );
		if ( ( F5012_34f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "35A400" );
			setCode = _T( "35E00D" );
		}
		break;

	case ID5012_35D:
	case ID5012_35E:
	case ID5012_35F:
		inputSign = 0;
		GetInputSign ( ID_F535, inputSign );
		if ( ( F5012_35f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
			code = _T( "35A500" );
			setCode = _T( "35E00E" );
		}
		break;

	case ID5012_36D:
	case ID5012_36E:
	case ID5012_36F:
		inputSign = 0;
		GetInputSign ( ID_F536, inputSign );
		if ( ( F5012_36f & 0x8880 ) || ( inputSign & 0x8888 ) ) {
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
int CShinFhyo5_012::GetUriageWariai ( int id, unsigned short *pWari )
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
//			pArith		：	合計金額
//---------------------------------------------------------
// 返送値
//			0			：	正常終了
//			-1			：	エラー
//=========================================================
int CShinFhyo5_012::GetUriageWariai2 ( int id, unsigned short *pWari, char *pArith )
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
int CShinFhyo5_012::SetPrintArea()
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
	case ID5012_20F:
		F5012_20f |= 0x01;
		break;
	case ID5012_21F:
		F5012_21f |= 0x01;
		break;
	case ID5012_22F:
		F5012_22f |= 0x01;
		break;
	case ID5012_23F:
		F5012_23f |= 0x01;
		break;
	case ID5012_24F:
		F5012_24f |= 0x01;
		break;
	case ID5012_25F:
		F5012_25f |= 0x01;
		break;
	case ID5012_26F:
		F5012_26f |= 0x01;
		break;
	case ID5012_27F:
		F5012_27f |= 0x01;
		break;
	case ID5012_28F:
		F5012_28f |= 0x01;
		break;
	case ID5012_29F:
		F5012_29f |= 0x01;
		break;
	case ID5012_30F:
		F5012_30f |= 0x01;
		break;
	case ID5012_31F:
		F5012_31f |= 0x01;
		break;
	case ID5012_32F:
		F5012_32f |= 0x01;
		break;
	case ID5012_33F:
		F5012_33f |= 0x01;
		break;
	case ID5012_34F:
		F5012_34f |= 0x01;
		break;
	case ID5012_35F:
		F5012_35f |= 0x01;
		break;
	case ID5012_36F:
		F5012_36f |= 0x01;
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
void CShinFhyo5_012::AllPrint()
{
	F5012_20f &= ~0x01;
	F5012_21f &= ~0x01;
	F5012_22f &= ~0x01;
	F5012_23f &= ~0x01;
	F5012_24f &= ~0x01;
	F5012_25f &= ~0x01;
	F5012_26f &= ~0x01;
	F5012_27f &= ~0x01;
	F5012_28f &= ~0x01;
	F5012_29f &= ~0x01;
	F5012_30f &= ~0x01;
	F5012_31f &= ~0x01;
	F5012_32f &= ~0x01;
	F5012_33f &= ~0x01;
	F5012_34f &= ~0x01;
	F5012_35f &= ~0x01;
	F5012_36f &= ~0x01;
}

//=========================================================
// 入力項目？
//---------------------------------------------------------
// 返送値
//			0			:		非入力項目
//			1			:		入力項目
//			-1			:		エラー
//=========================================================
int CShinFhyo5_012::IsInputArea ( short index )
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
// 付表5-2(2) 手入力サイン取得
//---------------------------------------------------------
// 引数
//			id			:		ID
//			inputSign	:		取得した手入力サイン
//---------------------------------------------------------
// 返送値
//			0			:		正常終了
//			-1			:		エラー
//=========================================================
int CShinFhyo5_012::GetInputSign ( int id, int &inputSign )
{
	int ret = 0;

	inputSign = 0;
	ret = ( ( CH31HyoView* )m_pParent )->GetInputSign ( FHYO5_2, id, inputSign );

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
int CShinFhyo5_012::GetInputSign_F512 ( int id, int &inputSign )
{
	int ret = 0;
	int tmpSign = 0;
	bool errFlg = false;

	inputSign = 0;

	switch ( id ) {
	case ID_F521:
		tmpSign = F5012_21f;
		break;
	case ID_F522:
		tmpSign = F5012_22f;
		break;
	case ID_F523:
		tmpSign = F5012_23f;
		break;
	case ID_F524:
		tmpSign = F5012_24f;
		break;
	case ID_F525:
		tmpSign = F5012_25f;
		break;
	case ID_F526:
		tmpSign = F5012_26f;
		break;
	case ID_F527:
		tmpSign = F5012_27f;
		break;
	case ID_F528:
		tmpSign = F5012_28f;
		break;
	case ID_F529:
		tmpSign = F5012_29f;
		break;
	case ID_F530:
		tmpSign = F5012_30f;
		break;
	case ID_F531:
		tmpSign = F5012_31f;
		break;
	case ID_F532:
		tmpSign = F5012_32f;
		break;
	case ID_F533:
		tmpSign = F5012_33f;
		break;
	case ID_F534:
		tmpSign = F5012_34f;
		break;
	case ID_F535:
		tmpSign = F5012_35f;
		break;
	case ID_F536:
		tmpSign = F5012_36f;
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
int CShinFhyo5_012::GetTokurei()
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
					pDispSign = &F5012_21Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E201" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_22Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E202" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_23Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E203" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_24Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E204" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_25Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E205" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_26Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E206" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_27Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E207" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_28Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E208" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_29Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E209" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_30Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20A" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_31Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20B" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_32Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20C" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_33Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20D" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_34Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20E" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_35Disp;
					bGet = true;
				}
				else if( money[i][j].code == _T( "35E20F" ) ) {
					memmove( tmpMony, money[i][j].arith, MONY_BUF_SIZE );
					pDispSign = &F5012_36Disp;
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
		F5012_21Disp |= 0x01;
		F5012_22Disp |= 0x01;
		F5012_23Disp |= 0x01;
		F5012_24Disp |= 0x01;
		F5012_25Disp |= 0x01;
		F5012_26Disp |= 0x01;
		F5012_27Disp |= 0x01;
		F5012_28Disp |= 0x01;
		F5012_29Disp |= 0x01;
		F5012_30Disp |= 0x01;
		F5012_31Disp |= 0x01;
		F5012_32Disp |= 0x01;
		F5012_33Disp |= 0x01;
		F5012_34Disp |= 0x01;
		F5012_35Disp |= 0x01;
		F5012_36Disp |= 0x01;
	}

	return 0;
}

/* // 付表5 控除対象仕入税額の選択をICSP連携専用の機能とするため一時的にカット
//=========================================================
// 選択された項目の控除対象仕入税額をセット
//---------------------------------------------------------
// 引数
//			selId		：	選択された項目のID
//=========================================================
void CShinFhyo5_012::SetSelectCode ( int  selId )
{
	bool errFlg = false;

	switch ( selId ) {
	case ID_F520:
		F5012_20f |= 0x02;
		break;
	case ID_F521:
		F5012_21f |= 0x02;
		break;
	case ID_F522:
		F5012_22f |= 0x02;
		break;
	case ID_F523:
		F5012_23f |= 0x02;
		break;
	case ID_F524:
		F5012_24f |= 0x02;
		break;
	case ID_F525:
		F5012_25f |= 0x02;
		break;
	case ID_F526:
		F5012_26f |= 0x02;
		break;
	case ID_F527:
		F5012_27f |= 0x02;
		break;
	case ID_F528:
		F5012_28f |= 0x02;
		break;
	case ID_F529:
		F5012_29f |= 0x02;
		break;
	case ID_F530:
		F5012_30f |= 0x02;
		break;
	case ID_F531:
		F5012_31f |= 0x02;
		break;
	case ID_F532:
		F5012_32f |= 0x02;
		break;
	case ID_F533:
		F5012_33f |= 0x02;
		break;
	case ID_F534:
		F5012_34f |= 0x02;
		break;
	case ID_F535:
		F5012_35f |= 0x02;
		break;
	case ID_F536:
		F5012_36f |= 0x02;
		break;
	default:
		errFlg = true;
		break;
	}

	if ( errFlg ) {
		return;
	}

	if ( ChangeBackColor ( TRUE ) == 0 ) {
		m_Fhyo5_012Diag01.ModifyItem ( SELECTITEM, DIAG_MDFY_OPAQUE );
		m_Fhyo5_012Diag01.Refresh();
	}

	SetKjCode();
}

//=========================================================
// 控除対象仕入税額の項目指定
//=========================================================
void CShinFhyo5_012::SetKjCode()
{
	char setFlg[MONY_BUF_SIZE] = { 0 };
	char buf[32] = { 0 };
	int itemID = 0;
	bool zeroFlg = true;

	if ( F5012_20f & 0x02 ) {
		itemID = ID_F520;
		zeroFlg = false;
	}
	else if ( F5012_21f & 0x02 ) {
		itemID = ID_F521;
	}
	else if ( F5012_22f & 0x02 ) {
		itemID = ID_F522;
	}
	else if ( F5012_23f & 0x02 ) {
		itemID = ID_F523;
	}
	else if ( F5012_24f & 0x02 ) {
		itemID = ID_F524;
	}
	else if ( F5012_25f & 0x02 ) {
		itemID = ID_F525;
	}
	else if ( F5012_26f & 0x02 ) {
		itemID = ID_F526;
	}
	else if ( F5012_27f & 0x02 ) {
		itemID = ID_F527;
	}
	else if ( F5012_28f & 0x02 ) {
		itemID = ID_F528;
	}
	else if ( F5012_29f & 0x02 ) {
		itemID = ID_F529;
	}
	else if ( F5012_30f & 0x02 ) {
		itemID = ID_F530;
	}
	else if ( F5012_31f & 0x02 ) {
		itemID = ID_F531;
	}
	else if ( F5012_32f & 0x02 ) {
		itemID = ID_F532;
	}
	else if ( F5012_33f & 0x02 ) {
		itemID = ID_F533;
	}
	else if ( F5012_34f & 0x02 ) {
		itemID = ID_F534;
	}
	else if ( F5012_35f & 0x02 ) {
		itemID = ID_F535;
	}
	else if ( F5012_36f & 0x02 ) {
		itemID = ID_F536;
	}
	else {
		return;
	}

	memset ( setFlg, '\0', sizeof ( setFlg ) );
	memset ( buf, '\0', sizeof ( buf ) );
	sprintf_s ( buf, sizeof ( buf ), _T( "%d" ), itemID );
	m_pArith->l_input ( setFlg, buf );

	( *m_pSyzSyukei )->SetShinkokuData ( _T( "362006" ), setFlg );

	if ( zeroFlg ) {
		memset ( setFlg, '\0', sizeof ( setFlg ) );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "362007" ), setFlg );
	}
	else {
		( *m_pSyzSyukei )->ResetShinkokuData ( _T( "362007" ) );
	}
}

//=========================================================
// 控除対象仕入税額の自動集計
//=========================================================
void CShinFhyo5_012::AutoKjCode()
{
	ChangeBackColor ( FALSE );
	m_Fhyo5_012Diag01.ModifyItem ( SELECTITEM, DIAG_MDFY_TRANSPARENT );
	m_Fhyo5_012Diag01.Refresh();

	( *m_pSyzSyukei )->ResetShinkokuData ( _T( "362006" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T( "362007" ) );

	F5012_20f &= ~0x02;
	F5012_21f &= ~0x02;
	F5012_22f &= ~0x02;
	F5012_23f &= ~0x02;
	F5012_24f &= ~0x02;
	F5012_25f &= ~0x02;
	F5012_26f &= ~0x02;
	F5012_27f &= ~0x02;
	F5012_28f &= ~0x02;
	F5012_29f &= ~0x02;
	F5012_30f &= ~0x02;
	F5012_31f &= ~0x02;
	F5012_32f &= ~0x02;
	F5012_33f &= ~0x02;
	F5012_34f &= ~0x02;
	F5012_35f &= ~0x02;
	F5012_36f &= ~0x02;
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
int CShinFhyo5_012::ChangeBackColor ( BOOL bSw )
{
	int s_idx = 0, e_idx;
	int colNo = 0;

	if ( F5012_20f & 0x02 ) {
		s_idx = ID5012_NO20_S;
		e_idx = ID5012_NO20_E;
	}
	else if ( F5012_21f & 0x02 ) {
		s_idx = ID5012_NO21_S;
		e_idx = ID5012_NO21_E;
	}
	else if ( F5012_22f & 0x02 ) {
		s_idx = ID5012_NO22_S;
		e_idx = ID5012_NO22_E;
	}
	else if ( F5012_23f & 0x02 ) {
		s_idx = ID5012_NO23_S;
		e_idx = ID5012_NO23_E;
	}
	else if ( F5012_24f & 0x02 ) {
		s_idx = ID5012_NO24_S;
		e_idx = ID5012_NO24_E;
	}
	else if ( F5012_25f & 0x02 ) {
		s_idx = ID5012_NO25_S;
		e_idx = ID5012_NO25_E;
	}
	else if ( F5012_26f & 0x02 ) {
		s_idx = ID5012_NO26_S;
		e_idx = ID5012_NO26_E;
	}
	else if ( F5012_27f & 0x02 ) {
		s_idx = ID5012_NO27_S;
		e_idx = ID5012_NO27_E;
	}
	else if ( F5012_28f & 0x02 ) {
		s_idx = ID5012_NO28_S;
		e_idx = ID5012_NO28_E;
	}
	else if ( F5012_29f & 0x02 ) {
		s_idx = ID5012_NO29_S;
		e_idx = ID5012_NO29_E;
	}
	else if ( F5012_30f & 0x02 ) {
		s_idx = ID5012_NO30_S;
		e_idx = ID5012_NO30_E;
	}
	else if ( F5012_31f & 0x02 ) {
		s_idx = ID5012_NO31_S;
		e_idx = ID5012_NO31_E;
	}
	else if ( F5012_32f & 0x02 ) {
		s_idx = ID5012_NO32_S;
		e_idx = ID5012_NO32_E;
	}
	else if ( F5012_33f & 0x02 ) {
		s_idx = ID5012_NO33_S;
		e_idx = ID5012_NO33_E;
	}
	else if ( F5012_34f & 0x02 ) {
		s_idx = ID5012_NO34_S;
		e_idx = ID5012_NO34_E;
	}
	else if ( F5012_35f & 0x02 ) {
		s_idx = ID5012_NO35_S;
		e_idx = ID5012_NO35_E;
	}
	else if ( F5012_36f & 0x02 ) {
		s_idx = ID5012_NO36_S;
		e_idx = ID5012_NO36_E;
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

	return 01;
}

//=========================================================
// 選択された控除対象仕入税額のクリア
//=========================================================
void CShinFhyo5_012::ResetKjCode()
{
	// 控除対象仕入税額を選択中だが付表5-3(1)で売上割合が変更された場合の対応

	if ( !( ( *m_pSnHeadData )->Sn_GenericSgn & 0x08 ) ) {
		return;
	}

	AutoKjCode();

	char tmpID[MONY_BUF_SIZE] = { 0 };
	char buf[32] = { 0 };
	int nID = 0, selID = 0;

	MoneyBasejagArray money;

	memset ( tmpID, '\0', sizeof ( tmpID ) );
	memset ( buf, '\0', sizeof ( buf ) );

	money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "362007" ) );
	memmove ( tmpID, money[0][0].arith, sizeof ( tmpID ) );
	m_pArith->l_print ( buf, tmpID, _T( "SSSS") );
	nID = atoi ( buf );

	if ( !nID ) {
		memset ( tmpID, '\0', sizeof ( tmpID ) );
		memset ( buf, '\0', sizeof ( buf ) );
		nID = 0;

		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "362006" ) );
		memmove ( tmpID, money[0][0].arith, sizeof ( tmpID ) );
		m_pArith->l_print ( buf, tmpID, _T( "SSSS") );
		nID = atoi ( buf );
	}

	selID = nID;

	SetSelectCode ( selID );
}


//=========================================================
// 控除対象仕入税額の再セット必要
//---------------------------------------------------------
// 引数
//		bRet		:		true  = 再セット必要
//							false = 不必要
//=========================================================
bool CShinFhyo5_012::IsKjCodeReset()
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
int CShinFhyo5_012::GetSelectItem ( int &getID )
{
	int id = 0;

	if ( F5012_20f & 0x02 ) {
		id = ID5012_20D;
	}
	else if ( F5012_21f & 0x02 ) {
		id = ID5012_21D;
	}
	else if ( F5012_22f & 0x02 ) {
		id = ID5012_22D;
	}
	else if ( F5012_23f & 0x02 ) {
		id = ID5012_23D;
	}
	else if ( F5012_24f & 0x02 ) {
		id = ID5012_24D;
	}
	else if ( F5012_25f & 0x02 ) {
		id = ID5012_25D;
	}
	else if ( F5012_26f & 0x02 ) {
		id = ID5012_26D;
	}
	else if ( F5012_27f & 0x02 ) {
		id = ID5012_27D;
	}
	else if ( F5012_28f & 0x02 ) {
		id = ID5012_28D;
	}
	else if ( F5012_29f & 0x02 ) {
		id = ID5012_29D;
	}
	else if ( F5012_30f & 0x02 ) {
		id = ID5012_30D;
	}
	else if ( F5012_31f & 0x02 ) {
		id = ID5012_31D;
	}
	else if ( F5012_32f & 0x02 ) {
		id = ID5012_32D;
	}
	else if ( F5012_33f & 0x02 ) {
		id = ID5012_33D;
	}
	else if ( F5012_34f & 0x02 ) {
		id = ID5012_34D;
	}
	else if ( F5012_35f & 0x02 ) {
		id = ID5012_35D;
	}
	else if ( F5012_36f & 0x02 ) {
		id = ID5012_36D;
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
void CShinFhyo5_012::ReSyukei()
{
	MemoryReget();
	Disp_f5012data ( 0 );
}*/