// ShinFhyo4_02.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo4_02.h"
#include "ShinFhyo4_02idx.h"


// CShinFhyo4_02 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo4_02, CSyzBaseDlg)

CShinFhyo4_02::CShinFhyo4_02(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo4_02::IDD, pParent)
	, m_pParent ( pParent )
	, m_pSnHeadData ( NULL )
	, m_pSyzSyukei( NULL )
{

	memset ( F402_1A, '\0', sizeof ( F402_1A ) );
	memset ( F402_1B, '\0', sizeof ( F402_1B ) );
	memset ( F402_1C, '\0', sizeof ( F402_1C ) );
	memset ( F402_1X, '\0', sizeof ( F402_1X ) );

	memset ( F402_1_1A, '\0', sizeof ( F402_1_1A ) );
	memset ( F402_1_1B, '\0', sizeof ( F402_1_1B ) );
	memset ( F402_1_1C, '\0', sizeof ( F402_1_1C ) );
	memset ( F402_1_1X, '\0', sizeof ( F402_1_1X ) );

	memset ( F402_2A, '\0', sizeof ( F402_2A ) );
	memset ( F402_2B, '\0', sizeof ( F402_2B ) );
	memset ( F402_2C, '\0', sizeof ( F402_2C ) );
	memset ( F402_2X, '\0', sizeof ( F402_2X ) );

	memset ( F402_3A, '\0', sizeof ( F402_3A ) );
	memset ( F402_3B, '\0', sizeof ( F402_3B ) );
	memset ( F402_3C, '\0', sizeof ( F402_3C ) );
	memset ( F402_3X, '\0', sizeof ( F402_3X ) );

	memset ( F402_4A, '\0', sizeof ( F402_4A ) );
	memset ( F402_4B, '\0', sizeof ( F402_4B ) );
	memset ( F402_4C, '\0', sizeof ( F402_4C ) );
	memset ( F402_4X, '\0', sizeof ( F402_4X ) );

	memset ( F402_5A, '\0', sizeof ( F402_5A ) );
	memset ( F402_5B, '\0', sizeof ( F402_5B ) );
	memset ( F402_5C, '\0', sizeof ( F402_5C ) );
	memset ( F402_5X, '\0', sizeof ( F402_5X ) );

	memset ( F402_6A, '\0', sizeof ( F402_6A ) );
	memset ( F402_6B, '\0', sizeof ( F402_6B ) );
	memset ( F402_6C, '\0', sizeof ( F402_6C ) );
	memset ( F402_6X, '\0', sizeof ( F402_6X ) );

	memset ( F402_7A, '\0', sizeof ( F402_7A ) );
	memset ( F402_7B, '\0', sizeof ( F402_7B ) );
	memset ( F402_7C, '\0', sizeof ( F402_7C ) );
	memset ( F402_7X, '\0', sizeof ( F402_7X ) );

	memset ( F402_8A, '\0', sizeof ( F402_8A ) );
	memset ( F402_8B, '\0', sizeof ( F402_8B ) );
	memset ( F402_8C, '\0', sizeof ( F402_8C ) );
	memset ( F402_8X, '\0', sizeof ( F402_8X ) );

	memset ( F402_9A, '\0', sizeof ( F402_9A ) );
	memset ( F402_9B, '\0', sizeof ( F402_9B ) );
	memset ( F402_9C, '\0', sizeof ( F402_9C ) );
	memset ( F402_9X, '\0', sizeof ( F402_9X ) );

	memset ( F402_11B, '\0', sizeof ( F402_11B ) );
	memset ( F402_11C, '\0', sizeof ( F402_11C ) );
	memset ( F402_11X, '\0', sizeof ( F402_11X ) );

	memset ( F402_12B, '\0', sizeof ( F402_12B ) );
	memset ( F402_12C, '\0', sizeof ( F402_12C ) );
	memset ( F402_12X, '\0', sizeof ( F402_12X ) );

	memset ( F402_13B, '\0', sizeof ( F402_13B ) );
	memset ( F402_13C, '\0', sizeof ( F402_13C ) );
	memset ( F402_13X, '\0', sizeof ( F402_13X ) );

	memset ( F402_14B, '\0', sizeof ( F402_14B ) );
	memset ( F402_14C, '\0', sizeof ( F402_14C ) );
	memset ( F402_14X, '\0', sizeof ( F402_14X ) );

	memset ( F402_15B, '\0', sizeof ( F402_15B ) );
	memset ( F402_15C, '\0', sizeof ( F402_15C ) );
	memset ( F402_15X, '\0', sizeof ( F402_15X ) );

	F402 = 0;

	m_curwnd = -1;

}

CShinFhyo4_02::~CShinFhyo4_02()
{
}

void CShinFhyo4_02::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo4_02Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo4_02Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo4_02, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CShinFhyo4_02 メッセージ ハンドラ

BOOL CShinFhyo4_02::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2);
	m_BottomIdx = 131;//コントロール一番最下のDiagのID指定。これで縦スクロール出る

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo4_02::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo4_02::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo4_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
int CShinFhyo4_02::Init()
{
	// データの読込み
	ReadData();

	// 読み込みデータを集計モジュールにセット
	ReadData2();

	MemoryReget();

	// ダイアグラム初期属性セット
	InitDiagAttr();

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		m_Fhyo4_02Diag01.ModifyItem ( 4, DIAG_MDFY_TRANSPARENT );
	}

	// 画面出力
	DispFh402();

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
int CShinFhyo4_02::InitInfo(CH31SnFhyo4_02Data **pSnFhyo4_02Data, CSnHeadData **pSnHeadData, CSyzSyukei **pSyzSyukei, CArithEx *pArith)
{
	ASSERT ( pSnFhyo4_02Data );
	if ( pSnFhyo4_02Data == NULL )
		return -1;

	ASSERT ( pSnHeadData );
	if ( pSnHeadData == NULL )
		return -1;

	ASSERT ( pSyzSyukei );
	if (pSyzSyukei == NULL)
		return -1;

	ASSERT(pArith);
	if (pArith == NULL)
		return -1;

	m_pSnHeadData = pSnHeadData;

	m_pSnFhyo4_02Data = pSnFhyo4_02Data;

	m_pSyzSyukei = pSyzSyukei;

	m_pArith = pArith;

	return 0;
}

//=============================
// ダイアグラム初期属性セット
//-----------------------------
// 返送値
//		0	:	正常
//		-1	:	エラー
//=============================
int CShinFhyo4_02::InitDiagAttr()
{
	int cnt = 0;

	while ( 1 ) {
		if ( Fhyo4_02[cnt].Index == 0 )
			break;

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo4_02[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Fhyo4_02[cnt].UnConnect;
			}
			else {
				Flg = Fhyo4_02[cnt].Connect;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL2, Fhyo4_02[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo4_02[cnt].Connect );
		cnt++;

	}

	m_Fhyo4_02Diag02.EnableDelete ( 0 );

	return 0;
}

//======================================
// 初期ポジションセット
//======================================
void CShinFhyo4_02::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		//index = m_Fhyo4_02Diag02.GetPosition();
		if ( ( index = m_Fhyo4_02Diag02.GetPosition() ) != -1 ) {
			m_Fhyo4_02Diag01.SetFocus();	// ダミーでセット
			m_Fhyo4_02Diag02.SetPosition ( index );
			m_Fhyo4_02Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo4_02Diag01.SetFocus();	// ダミーでセット
	m_Fhyo4_02Diag02.SetFocus();

	// 課税標準額A欄
	m_Fhyo4_02Diag02.SetPosition ( ID402_1_1A );
}

//=================================================
// 入力項目色変え
//-------------------------------------------------
// 引数
//		id		:	ダイアグラムID
//		index	:	ダイアグラム内インデックス
//		sign	:	変更サイン
//=================================================
void CShinFhyo4_02::ChangeColor(unsigned short id, short index, int sign)
{
	DIAGRAM_ATTRIBUTE DA;

	diag_getattr ( id, index, &DA, CIcsdiagctrl );
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

	diag_setattr ( id, index, &DA, FALSE, CIcsdiagctrl );
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
int CShinFhyo4_02::UpDownFocusMoveCheck(int Index, int Move)
{
	int idx = 0;
	int i = 0, j = 0;

	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY4_02CNT; i++ ) {
		if ( Index == FCS_Fhyo4_02[i].IDNo )
			break;
	}

	// 現在ポジションセット
	idx = FCS_Fhyo4_02[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) {			//　下方向
		for ( j = i + 1; j < FCS_FHY4_02CNT; j++ ) {
			m_Fhyo4_02Diag02.GetAttribute ( FCS_Fhyo4_02[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo4_02[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {		//　上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo4_02Diag02.GetAttribute ( FCS_Fhyo4_02[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo4_02[j].IDNo;
				break;
			}
		}
	}

	return idx;
}

//=========================
// データの読込み
//-------------------------
// 返送値
//		0	:	正常終了
//=========================
int CShinFhyo4_02::ReadData()
{
	// 課税標準額
	memmove ( F402_1A, ( *m_pSnFhyo4_02Data )->Sn_F402_1A, MONY_BUF_SIZE );
	memmove ( F402_1B, ( *m_pSnFhyo4_02Data )->Sn_F402_1B, MONY_BUF_SIZE );
	memmove ( F402_1C, ( *m_pSnFhyo4_02Data )->Sn_F402_1C, MONY_BUF_SIZE );
	memmove ( F402_1X, ( *m_pSnFhyo4_02Data )->Sn_F402_1X, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
	memmove ( F402_1_1A, ( *m_pSnFhyo4_02Data )->Sn_F402_1_1A, MONY_BUF_SIZE );
	memmove ( F402_1_1B, ( *m_pSnFhyo4_02Data )->Sn_F402_1_1B, MONY_BUF_SIZE );
	memmove ( F402_1_1C, ( *m_pSnFhyo4_02Data )->Sn_F402_1_1C, MONY_BUF_SIZE );
	memmove ( F402_1_1X, ( *m_pSnFhyo4_02Data )->Sn_F402_1_1X, MONY_BUF_SIZE );

	// 消費税額
	memmove ( F402_2A, ( *m_pSnFhyo4_02Data )->Sn_F402_2A, MONY_BUF_SIZE );
	memmove ( F402_2B, ( *m_pSnFhyo4_02Data )->Sn_F402_2B, MONY_BUF_SIZE );
	memmove ( F402_2C, ( *m_pSnFhyo4_02Data )->Sn_F402_2C, MONY_BUF_SIZE );
	memmove ( F402_2X, ( *m_pSnFhyo4_02Data )->Sn_F402_2X, MONY_BUF_SIZE );
	F402 = ( *m_pSnFhyo4_02Data )->Sn_F402_2sw;

	// 貸倒回収に係る消費税額
	memmove ( F402_3A, ( *m_pSnFhyo4_02Data )->Sn_F402_3A, MONY_BUF_SIZE );
	memmove ( F402_3B, ( *m_pSnFhyo4_02Data )->Sn_F402_3B, MONY_BUF_SIZE );
	memmove ( F402_3C, ( *m_pSnFhyo4_02Data )->Sn_F402_3C, MONY_BUF_SIZE );
	memmove ( F402_3X, ( *m_pSnFhyo4_02Data )->Sn_F402_3X, MONY_BUF_SIZE );

	// 控除対象仕入税額
	memmove ( F402_4A, ( *m_pSnFhyo4_02Data )->Sn_F402_4A, MONY_BUF_SIZE );
	memmove ( F402_4B, ( *m_pSnFhyo4_02Data )->Sn_F402_4B, MONY_BUF_SIZE );
	memmove ( F402_4C, ( *m_pSnFhyo4_02Data )->Sn_F402_4C, MONY_BUF_SIZE );
	memmove ( F402_4X, ( *m_pSnFhyo4_02Data )->Sn_F402_4X, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memmove ( F402_5A, ( *m_pSnFhyo4_02Data )->Sn_F402_5A, MONY_BUF_SIZE );
	memmove ( F402_5B, ( *m_pSnFhyo4_02Data )->Sn_F402_5B, MONY_BUF_SIZE );
	memmove ( F402_5C, ( *m_pSnFhyo4_02Data )->Sn_F402_5C, MONY_BUF_SIZE );
	memmove ( F402_5X, ( *m_pSnFhyo4_02Data )->Sn_F402_5X, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	memmove ( F402_6A, ( *m_pSnFhyo4_02Data )->Sn_F402_6A, MONY_BUF_SIZE );
	memmove ( F402_6B, ( *m_pSnFhyo4_02Data )->Sn_F402_6B, MONY_BUF_SIZE );
	memmove ( F402_6C, ( *m_pSnFhyo4_02Data )->Sn_F402_6C, MONY_BUF_SIZE );
	memmove ( F402_6X, ( *m_pSnFhyo4_02Data )->Sn_F402_6X, MONY_BUF_SIZE );

	// 控除税額小計
	memmove ( F402_7A, ( *m_pSnFhyo4_02Data )->Sn_F402_7A, MONY_BUF_SIZE );
	memmove ( F402_7B, ( *m_pSnFhyo4_02Data )->Sn_F402_7B, MONY_BUF_SIZE );
	memmove ( F402_7C, ( *m_pSnFhyo4_02Data )->Sn_F402_7C, MONY_BUF_SIZE );
	memmove ( F402_7X, ( *m_pSnFhyo4_02Data )->Sn_F402_7X, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( F402_8A, ( *m_pSnFhyo4_02Data )->Sn_F402_8A, MONY_BUF_SIZE );
	memmove ( F402_8B, ( *m_pSnFhyo4_02Data )->Sn_F402_8B, MONY_BUF_SIZE );
	memmove ( F402_8C, ( *m_pSnFhyo4_02Data )->Sn_F402_8C, MONY_BUF_SIZE );
	memmove ( F402_8X, ( *m_pSnFhyo4_02Data )->Sn_F402_8X, MONY_BUF_SIZE );

	// 差引税額
	memmove ( F402_9A, ( *m_pSnFhyo4_02Data )->Sn_F402_9A, MONY_BUF_SIZE );
	memmove ( F402_9B, ( *m_pSnFhyo4_02Data )->Sn_F402_9B, MONY_BUF_SIZE );
	memmove ( F402_9C, ( *m_pSnFhyo4_02Data )->Sn_F402_9C, MONY_BUF_SIZE );
	memmove ( F402_9X, ( *m_pSnFhyo4_02Data )->Sn_F402_9X, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( F402_11B, ( *m_pSnFhyo4_02Data )->Sn_F402_11B, MONY_BUF_SIZE );
	memmove ( F402_11C, ( *m_pSnFhyo4_02Data )->Sn_F402_11C, MONY_BUF_SIZE );
	memmove ( F402_11X, ( *m_pSnFhyo4_02Data )->Sn_F402_11X, MONY_BUF_SIZE );

	// 差引税額
	memmove ( F402_12B, ( *m_pSnFhyo4_02Data )->Sn_F402_12B, MONY_BUF_SIZE );
	memmove ( F402_12C, ( *m_pSnFhyo4_02Data )->Sn_F402_12C, MONY_BUF_SIZE );
	memmove ( F402_12X, ( *m_pSnFhyo4_02Data )->Sn_F402_12X, MONY_BUF_SIZE );

	// 合計差引地方消費税の課税標準となる消費税額
	memmove ( F402_13B, ( *m_pSnFhyo4_02Data )->Sn_F402_13B, MONY_BUF_SIZE );
	memmove ( F402_13C, ( *m_pSnFhyo4_02Data )->Sn_F402_13C, MONY_BUF_SIZE );
	memmove ( F402_13X, ( *m_pSnFhyo4_02Data )->Sn_F402_13X, MONY_BUF_SIZE );

	// 還付額
	memmove ( F402_14B, ( *m_pSnFhyo4_02Data )->Sn_F402_14B, MONY_BUF_SIZE );
	memmove ( F402_14C, ( *m_pSnFhyo4_02Data )->Sn_F402_14C, MONY_BUF_SIZE );
	memmove ( F402_14X, ( *m_pSnFhyo4_02Data )->Sn_F402_14X, MONY_BUF_SIZE );

	// 納税額
	memmove ( F402_15B, ( *m_pSnFhyo4_02Data )->Sn_F402_15B, MONY_BUF_SIZE );
	memmove ( F402_15C, ( *m_pSnFhyo4_02Data )->Sn_F402_15C, MONY_BUF_SIZE );
	memmove ( F402_15X, ( *m_pSnFhyo4_02Data )->Sn_F402_15X, MONY_BUF_SIZE );

	return 0;
}

//=============================
// データの書込み
//-----------------------------
// 返送値
//		0	:	正常終了
//=============================
int CShinFhyo4_02::WriteData()
{
	// 課税標準額
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_1A, F402_1A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_1B, F402_1B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_1C, F402_1C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_1X, F402_1X, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_1_1A, F402_1_1A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_1_1B, F402_1_1B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_1_1C, F402_1_1C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_1_1X, F402_1_1X, MONY_BUF_SIZE );

	// 消費税額
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_2A, F402_2A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_2B, F402_2B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_2C, F402_2C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_2X, F402_2X, MONY_BUF_SIZE );
	(*m_pSnFhyo4_02Data)->Sn_F402_2sw = F402;

	// 貸倒回収に係る消費税額
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_3A, F402_3A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_3B, F402_3B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_3C, F402_3C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_3X, F402_3X, MONY_BUF_SIZE );

	// 控除対象仕入税額
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_4A, F402_4A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_4B, F402_4B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_4C, F402_4C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_4X, F402_4X, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_5A, F402_5A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_5B, F402_5B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_5C, F402_5C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_5X, F402_5X, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_6A, F402_6A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_6B, F402_6B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_6C, F402_6C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_6X, F402_6X, MONY_BUF_SIZE );

	// 控除税額小計
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_7A, F402_7A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_7B, F402_7B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_7C, F402_7C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_7X, F402_7X, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_8A, F402_8A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_8B, F402_8B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_8C, F402_8C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_8X, F402_8X, MONY_BUF_SIZE );

	// 差引税額
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_9A, F402_9A, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_9B, F402_9B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_9C, F402_9C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_9X, F402_9X, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_11B, F402_11B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_11C, F402_11C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_11X, F402_11X, MONY_BUF_SIZE );

	// 差引税額
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_12B, F402_12B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_12C, F402_12C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_12X, F402_12X, MONY_BUF_SIZE );

	// 差引地方消費税の課税標準となる消費税額
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_13B, F402_13B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_13C, F402_13C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_13X, F402_13X, MONY_BUF_SIZE );

	// 還付額
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_14B, F402_14B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_14C, F402_14C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_14X, F402_14X, MONY_BUF_SIZE );

	// 納税額
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_15B, F402_15B, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_15C, F402_15C, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_02Data )->Sn_F402_15X, F402_15X, MONY_BUF_SIZE );


	return 0;
}

//==========================
// 画面出力
//==========================
void CShinFhyo4_02::DispFh402()
{
	DIAGRAM_ATTRIBUTE DA;
	DIAGRAM_DATA      DiagData;

	DiagData.data_imgdata = NULL;
	
	// 画面の初期化
	for ( int i = 3; i <= 5; i++ ) {
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}

	for ( int i = ID402_1A; i <= ID402_15X; i++ ) {
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}


	// 手入力サイン初期化
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		F402 = 0;
	}

	// 期間・氏名の表示
	char buf[512] = { 0 };
	char yy, mm, dd;
	yy = mm = dd = 0;

	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s ( buf, sizeof ( buf ), _T( "%02x.%02x.%02x" ), ( yy & 0xff ), ( mm & 0xff ), ( dd & 0xff ) );
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL2, 4, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 自

	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	sprintf_s ( buf, sizeof ( buf ), _T( "%02x.%02x.%02x" ), ( yy & 0xff ), ( mm & 0xff ), ( dd & 0xff ) );
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL2, 5, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 至

	memset ( buf, '\0', sizeof ( buf ) );
	if ( ( *m_pSnHeadData )->IsSoleProprietor() ) {
		memmove ( buf, ( *m_pSnHeadData )->Sn_DIHYO, 40 );
	}
	else {
		memmove ( buf, ( *m_pSnHeadData )->Sn_CONAM, 40 );
	}
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL2, 3, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 氏名又は名称

	// 全データ表示
	Disp_f402data ( 0 );
}

//===============================
//　データ表示
//-------------------------------
// 引数
//		idx	:	0 = 全データ
//===============================
void CShinFhyo4_02::Disp_f402data(int idx)
{
	switch ( idx ) {
		case 0:
		case 1:
			f402_1data();
			f402_1_1data();
		case 2:
			f402_2data();
		case 3:
			f402_3data();
		case 4:
		case 5://yoshida190706　控除対象仕入税額も更新。付表５から計算かかった値を表示する為
			f402_4data();
			f402_5data();
		case 6:
			f402_6data();
		case 7:
			f402_7data();
		case 8:
			f402_8data();
		case 9:
			f402_9data();
		case 11:
			f402_11data();
		case 12:
			f402_12data();
		case 13:
			f402_13data();
		case 14:
			f402_14data();
		case 15:
			f402_15data();
			break;
	}
}

//========================================
// 課税標準額
//========================================
void CShinFhyo4_02::f402_1data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_1A, F402_1A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_1B, F402_1B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_1C, F402_1C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_1X, F402_1X );
}

//========================================
// 課税資産の譲渡等の対価の額
//========================================
void CShinFhyo4_02::f402_1_1data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_1_1A, F402_1_1A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_1_1B, F402_1_1B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_1_1C, F402_1_1C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_1_1X, F402_1_1X );
}

//========================================
// 消費税額
//========================================
void CShinFhyo4_02::f402_2data()
{
	if( ( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01 ) == 0x00 ) ){
		ChangeColor(IDC_ICSDIAG8CTRL2, ID402_2A, ( F402 & 0x08 ) ? 3 : 0);
		ChangeColor(IDC_ICSDIAG8CTRL2, ID402_2B, ( F402 & 0x80 ) ? 3 : 0);
		ChangeColor(IDC_ICSDIAG8CTRL2, ID402_2C, ( F402 & 0x800 ) ? 3 : 0);
	}
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_2A, F402_2A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_2B, F402_2B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_2C, F402_2C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_2X, F402_2X );
}

//=======================================
// 貸倒回収に係る消費税額
//=======================================
void CShinFhyo4_02::f402_3data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_3A, F402_3A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_3B, F402_3B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_3C, F402_3C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_3X, F402_3X );
}

//=======================================
// 控除対象仕入税額
//=======================================
void CShinFhyo4_02::f402_4data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_4A, F402_4A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_4B, F402_4B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_4C, F402_4C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_4X, F402_4X );
}

//=======================================
// 返還等対価に係る税額
//=======================================
void CShinFhyo4_02::f402_5data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_5A, F402_5A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_5B, F402_5B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_5C, F402_5C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_5X, F402_5X );
}

//=======================================
// 貸倒れに係る税額
//=======================================
void CShinFhyo4_02::f402_6data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_6A, F402_6A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_6B, F402_6B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_6C, F402_6C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_6X, F402_6X );
}

//========================================
// 控除税額小計
//========================================
void CShinFhyo4_02::f402_7data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_7A, F402_7A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_7B, F402_7B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_7C, F402_7C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_7X, F402_7X );
}

//========================================
// 控除不足還付税額
//========================================
void CShinFhyo4_02::f402_8data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_8A, F402_8A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_8B, F402_8B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_8C, F402_8C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_8X, F402_8X );
}

//========================================
// 差引税額
//========================================
void CShinFhyo4_02::f402_9data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_9A, F402_9A );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_9B, F402_9B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_9C, F402_9C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_9X, F402_9X );
}

//========================================
// 控除不足還付税額
//========================================
void CShinFhyo4_02::f402_11data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_11B, F402_11B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_11C, F402_11C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_11X, F402_11X );
}

//========================================
// 差引税額
//========================================
void CShinFhyo4_02::f402_12data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_12B, F402_12B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_12C, F402_12C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_12X, F402_12X );
}

//========================================
// 差引地方消費税の課税標準となる消費税額
//========================================
void CShinFhyo4_02::f402_13data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_13B, F402_13B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_13C, F402_13C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_13X, F402_13X );
}

//========================================
// 還付額
//========================================
void CShinFhyo4_02::f402_14data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_14B, F402_14B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_14C, F402_14C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_14X, F402_14X );
}

//========================================
// 納税額
//========================================
void CShinFhyo4_02::f402_15data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_15B, F402_15B );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_15C, F402_15C );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID402_15X, F402_15X );
}

//==================================================
// 金額表示クリア
//--------------------------------------------------
// 引数
//		Id		:	ダイアグラムID
//		Index	:	ダイアグラム内インデックス
//==================================================
void CShinFhyo4_02::dsp_cls(unsigned short Id, short Index)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	switch ( Index ) {
		case 4:		// 課税期間 自
		case 5:		// 課税期間 至
		case 3:		// 氏名又は名称
			diag_clear ( Id, Index, TRUE, CIcsdiagctrl );
			break;
		default:
			memset ( DiagData.data_val, '\0', MONY_BUF_SIZE );
			diag_setdata ( Id, Index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			break;
	}
}

//====================================================
// 金額表示
//----------------------------------------------------
// 引数
//		Id		:	ダイアグラムID
//		Index	:	ダイアグラム内インデックス
//		Mony	:	表示金額
//====================================================
void CShinFhyo4_02::dsp_prs(unsigned short Id, short Index, char *Mony)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	memmove ( DiagData.data_val, Mony, MONY_BUF_SIZE );
	diag_setdata ( Id, Index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
}

//==================================
// 各入力データ抽出
//==================================
void CShinFhyo4_02::GetAllDiagData()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	// 1. 課税標準額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_1A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_1A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_1B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_1B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_1C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_1C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_1X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_1X, DiagData.data_val, MONY_BUF_SIZE );

	// 1-1. 課税資産の譲渡等の対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_1_1A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_1_1A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_1_1B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_1_1B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_1_1C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_1_1C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_1_1X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_1_1X, DiagData.data_val, MONY_BUF_SIZE );

	// 2. 消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_2A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_2A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_2B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_2B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_2C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_2C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_2X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_2X, DiagData.data_val, MONY_BUF_SIZE );

	// 3. 貸倒回収に係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_3A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_3A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_3B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_3B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_3C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_3C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_3X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_3X, DiagData.data_val, MONY_BUF_SIZE );

	// 4. 控除対象仕入税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_4A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_4A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_4B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_4B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_4C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_4C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_4X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_4X, DiagData.data_val, MONY_BUF_SIZE );

	// 5. 返還等対価に係る税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_5A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_5A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_5B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_5B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_5C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_5C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_5X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_5X, DiagData.data_val, MONY_BUF_SIZE );

	// 6. 貸倒れに係る税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_6A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_6A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_6B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_6B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_6C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_6C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_6X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_6X, DiagData.data_val, MONY_BUF_SIZE );

	// 7. 控除税額小計
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_7A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_7A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_7B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_7B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_7C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_7C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_7X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_7X, DiagData.data_val, MONY_BUF_SIZE );

	// 8. 控除不足還付税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_8A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_8A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_8B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_8B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_8C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_8C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_8X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_8X, DiagData.data_val, MONY_BUF_SIZE );

	// 9. 差引税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_9A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_9A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_9B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_9B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_9C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_9C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_9X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_9X, DiagData.data_val, MONY_BUF_SIZE );

	// 11. 控除不足還付税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_11B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_11B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_11C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_11C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_11X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_11X, DiagData.data_val, MONY_BUF_SIZE );

	// 12. 差引税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_12B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_12B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_12C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_12C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_12X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_12X, DiagData.data_val, MONY_BUF_SIZE );

	// 13. 差引地方消費税の課税標準となる消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_13B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_13B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_13C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_13C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_13X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_13X, DiagData.data_val, MONY_BUF_SIZE );

	// 14. 還付額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_14B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_14B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_14C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_14C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_14X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_14X, DiagData.data_val, MONY_BUF_SIZE );

	// 15. 納税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_15B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_15B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_15C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_15C, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID402_15X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F402_15X, DiagData.data_val, MONY_BUF_SIZE );

}

BEGIN_EVENTSINK_MAP(CShinFhyo4_02, CSyzBaseDlg)
ON_EVENT(CShinFhyo4_02, IDC_ICSDIAG8CTRL2, 1, CShinFhyo4_02::EditONIcsdiag8ctrl2, VTS_I2)
ON_EVENT(CShinFhyo4_02, IDC_ICSDIAG8CTRL2, 2, CShinFhyo4_02::EditOFFIcsdiag8ctrl2, VTS_I2)
ON_EVENT(CShinFhyo4_02, IDC_ICSDIAG8CTRL2, 3, CShinFhyo4_02::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo4_02::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;
}

void CShinFhyo4_02::EditOFFIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char *pMony = NULL;
	char CkMy[MONY_BUF_SIZE] = {0};

	switch ( index ) {
		case ID402_1A:		// 1. 課税標準額
		case ID402_1B:
		case ID402_1C:
			SyukeiDataSet(index);
			Disp_f402data(1);
			break;
		case ID402_1_1A:	// 1-1. 課税資産の譲渡等の対価の額
		case ID402_1_1B:
		case ID402_1_1C:
			SyukeiDataSet(index);
			Disp_f402data(1);
			break;
		case ID402_2A:		// 2. 消費税額
		case ID402_2B:
		case ID402_2C:
			SyukeiDataSet(index);
			Disp_f402data(2);
			break;
		case ID402_3A:		// 3. 貸倒回収に係る消費税額
		case ID402_3B:
		case ID402_3C:
			SyukeiDataSet(index);
			Disp_f402data(3);
			break;
		case ID402_4A:		// 4. 控除対象仕入税額
		case ID402_4B:
		case ID402_4C:
			SyukeiDataSet(index);
			Disp_f402data(4);
			break;
		case ID402_5A:		// 5. 返還等対価に係る税額
		case ID402_5B:
		case ID402_5C:
			SyukeiDataSet(index);
			Disp_f402data(4);//yoshida190706
//			Disp_f402data(5);
			break;
		case ID402_6A:		// 6. 貸倒れに係る税額
		case ID402_6B:
		case ID402_6C:
			SyukeiDataSet(index);
			Disp_f402data(6);
			break;
	}
}

void CShinFhyo4_02::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	if ( nChar == 0 ) {
		return;
	}

	// Shift+TABをTABに、TABをENTERに変換
	char VK_FLG = 0x00;
	VK_FLG = ( char )::GetKeyState ( VK_SHIFT );
	short wTerm = 0;
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
	if ( wTerm == VK_UP ) {
		pos = UpDownFocusMoveCheck ( index, 1 );
		m_Fhyo4_02Diag02.SetPosition ( pos );
	}
	if ( wTerm == VK_DOWN ) {
		pos = UpDownFocusMoveCheck ( index, 0 );
		m_Fhyo4_02Diag02.SetPosition ( pos );
	}

	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_F3 ) ) {
		m_Fhyo4_02Diag02.SetNextPosition();
	}
	else if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) ) {
		m_Fhyo4_02Diag02.SetPrevPosition();
	}

	// 変更サイン
	if ( wTerm == VK_DELETE ) {
		switch ( index ) {
			case ID402_2A:			// 消費税額　税率3%
				F402 &= ~0x08;

				(*m_pSyzSyukei)->ResetShinkokuData( _T("340301") );
//				memset(F402_2A, '\0', MONY_BUF_SIZE);
				Disp_f402data(2);
				m_Fhyo4_02Diag02.SetPosition(ID402_2A);
				break;
			case ID402_2B:			// 消費税額　税率4%
				F402 &= ~0x80;

				(*m_pSyzSyukei)->ResetShinkokuData(_T("340302"));
//				memset(F402_2B, '\0', MONY_BUF_SIZE);
				Disp_f402data(2);
				m_Fhyo4_02Diag02.SetPosition(ID402_2B);
				break;
			case ID402_2C:			// 消費税額　税率6.3%
				F402 &= ~0x800;

				(*m_pSyzSyukei)->ResetShinkokuData(_T("340303"));
//				memset(F402_2C, '\0', MONY_BUF_SIZE);
				Disp_f402data(2);
				m_Fhyo4_02Diag02.SetPosition(ID402_2C);
				break;
			default:
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;
		}

//		dsp_cls(IDC_ICSDIAG8CTRL2, index);
	}


	int nowpos = m_Fhyo4_02Diag02.GetPosition();

	RECT rectA, rectB;
	GetWindowRect ( &rectA );
	diag_getrect ( IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl );
	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectA.top ) / 2 ) );
	}


}

BOOL CShinFhyo4_02::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ) {
			// 付表4-2更新
			GetAllDiagData();
			WriteData();

			if ( m_pParent ) {
				m_pParent->PostMessageA ( WM_KEYDOWN, VK_END );
			}

			return TRUE;
		}

		if ( pMsg->wParam == VK_F2 ) {
			DIAGRAM_DATA dd;
			if ( m_curwnd != -1 ) {
				short result = ( ( CIcsdiagctrl* )GetDlgItem ( m_curwnd ) )->GetPosition();
				diag_getdata ( m_curwnd, result, &dd, CIcsdiagctrl );

				if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
					TerminationIcsdiag8ctrl2 ( result, ( short )pMsg->wParam, 0, ( LPUNKNOWN )&dd );
				}
				return TRUE;
			}
		}
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

int CShinFhyo4_02::SyukeiDataSet(int cID)
{
	char			val[MONY_BUF_SIZE] = {0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	diag_getdata(IDC_ICSDIAG8CTRL2, cID, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove( val, DiagData.data_val, MONY_BUF_SIZE );

	switch (cID) {
	// 課税標準額
	case ID402_1A:
		memmove(F402_1A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310101"), F402_1A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340101"), F402_1A);
		break;
	case ID402_1B:
		memmove(F402_1B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310102"), F402_1B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340102"), F402_1B);
		break;
	case ID402_1C:
		memmove(F402_1C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310103"), F402_1C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340103"), F402_1C);
		break;

	// 課税資産の譲渡等の対価の額
	case ID402_1_1A:
		memmove(F402_1_1A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310201"), F402_1_1A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340201"), F402_1_1A);
		break;
	case ID402_1_1B:
		memmove(F402_1_1B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310202"), F402_1_1B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340202"), F402_1_1B);
		break;
	case ID402_1_1C:
		memmove(F402_1_1C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310203"), F402_1_1C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340203"), F402_1_1C);
		break;

	// 消費税額
	case ID402_2A:
		if ( m_pArith->l_cmp ( F402_2A, val ) ) {
			F402 |= 0x08;
			memmove(F402_2A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("340301"), F402_2A);
		}
		break;
	case ID402_2B:
		if ( m_pArith->l_cmp ( F402_2B, val ) ) {
			F402 |= 0x80;
			memmove(F402_2B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("340302"), F402_2B);
		}
		break;
	case ID402_2C:
		if ( m_pArith->l_cmp ( F402_2C, val ) ) {
			F402 |= 0x800;
			memmove(F402_2C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("340303"), F402_2C);
		}
		break;

	// 貸倒回収に係る消費税額
	case ID402_3A:
		memmove(F402_3A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322501"), F402_3A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340401"), F402_3A);
		break;
	case ID402_3B:
		memmove(F402_3B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322502"), F402_3B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340402"), F402_3B);
		break;
	case ID402_3C:
		memmove(F402_3C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322503"), F402_3C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340403"), F402_3C);
		break;

	// 控除対象仕入税額
	case ID402_4A:
		memmove(F402_4A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340501"), F402_4A);
		break;
	case ID402_4B:
		memmove(F402_4B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340502"), F402_4B);
		break;
	case ID402_4C:
		memmove(F402_4C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340503"), F402_4C);
		break;

	// 返還等対価に係る税額
	case ID402_5A:
		memmove(F402_5A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310701"), F402_5A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340601"), F402_5A);
		break;
	case ID402_5B:
		memmove(F402_5B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310702"), F402_5B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340602"), F402_5B);
		break;
	case ID402_5C:
		memmove(F402_5C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310703"), F402_5C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340603"), F402_5C);
		break;

	// 貸倒れに係る税額
	case ID402_6A:
		memmove(F402_6A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("311001"), F402_6A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340701"), F402_6A);
		break;
	case ID402_6B:
		memmove(F402_6B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("311002"), F402_6B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340702"), F402_6B);
		break;
	case ID402_6C:
		memmove(F402_6C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("311003"), F402_6C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340703"), F402_6C);
		break;
	}

	return MemoryReget();
}

int CShinFhyo4_02::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	MoneyBasejagArray	money;

	money = (*m_pSyzSyukei)->GetShinkokuData( _T("34") );

	int cnt = money.GetCount();
	int incnt = 0;

	// 計算項目を全て取得
	for (int i = 0; i < cnt; i++ ) {
		incnt = money[i].GetCount();
		for (int j = 0; j < incnt; j++ ) {
			// 課税標準額
			if (money[i][j].code == _T("340100")) {
				memmove(F402_1X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("340101")) {
				memmove(F402_1A, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("340102")) {
				memmove(F402_1B, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("340103")) {
				memmove(F402_1C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 課税資産の譲渡等の対価の額
			else if (money[i][j].code == _T("340200")) {
				memmove(F402_1_1X, money[i][j].arith, MONY_BUF_SIZE);
			}

			//---->yoshida191216
			else if (money[i][j].code == _T("340201")) {
				memmove(F402_1_1A, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340202")) {
				memmove(F402_1_1B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340203")) {
				memmove(F402_1_1C, money[i][j].arith, MONY_BUF_SIZE);
			}
			//<-------------------


			// 消費税額
			else if (money[i][j].code == _T("340300")) {
				memmove(F402_2X, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340301")) {
				memmove(F402_2A, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340302")) {
				memmove(F402_2B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340303")) {
				memmove(F402_2C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 貸倒回収に係る消費税額
			else if (money[i][j].code == _T("340400")) {
				memmove(F402_3X, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340401")) {
				memmove(F402_3A, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340402")) {
				memmove(F402_3B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340403")) {
				memmove(F402_3C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 控除対象仕入税額
			else if (money[i][j].code == _T("340500")) {
				memmove(F402_4X, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340501")) {
				memmove(F402_4A, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340502")) {
				memmove(F402_4B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340503")) {
				memmove(F402_4C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 返還等対価に係る税額
			else if (money[i][j].code == _T("340600")) {
				memmove(F402_5X, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340601")) {
				memmove(F402_5A, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340602")) {
				memmove(F402_5B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340603")) {
				memmove(F402_5C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 貸倒れに係る消費税額
			else if (money[i][j].code == _T("340700")) {
				memmove(F402_6X, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340701")) {
				memmove(F402_6A, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340702")) {
				memmove(F402_6B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340703")) {
				memmove(F402_6C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 控除税額小計
			else if (money[i][j].code == _T("340800")) {
				memmove(F402_7X, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340801")) {
				memmove(F402_7A, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340802")) {
				memmove(F402_7B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340803")) {
				memmove(F402_7C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 控除不足還付税額
			else if (money[i][j].code == _T("340900")) {
				memmove(F402_8X, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340901")) {
				memmove(F402_8A, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340902")) {
				memmove(F402_8B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("340903")) {
				memmove(F402_8C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 差引税額
			else if (money[i][j].code == _T("341000")) {
				memmove(F402_9X, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("341001")) {
				memmove(F402_9A, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("341002")) {
				memmove(F402_9B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("341003")) {
				memmove(F402_9C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 控除不足還付税額
			else if (money[i][j].code == _T("341200")) {
				memmove(F402_11X, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("341202")) {
				memmove(F402_11B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("341203")) {
				memmove(F402_11C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 差引税額
			else if (money[i][j].code == _T("341300")) {
				memmove(F402_12X, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("341302")) {
				memmove(F402_12B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("341303")) {
				memmove(F402_12C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 合計差引地方消費税の課税標準額となる消費税額
			else if (money[i][j].code == _T("341400")) {
				memmove(F402_13X, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("341402")) {
				memmove(F402_13B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("341403")) {
				memmove(F402_13C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 還付額
			else if (money[i][j].code == _T("341500")) {
				memmove(F402_14X, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("341502")) {
				memmove(F402_14B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("341503")) {
				memmove(F402_14C, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 納付額
			else if (money[i][j].code == _T("341600")) {
				memmove(F402_15X, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("341602")) {
				memmove(F402_15B, money[i][j].arith, MONY_BUF_SIZE);
			}
			else if (money[i][j].code == _T("341603")) {
				memmove(F402_15C, money[i][j].arith, MONY_BUF_SIZE);
			}
		}
	}

	return 0;
}

void CShinFhyo4_02::ReadData2()
{

	//if ( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ) {	// 非連動のとき
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {	// 非連動、入力ロックの場合
		// 課税標準額
		//------>yoshida190621
		(*m_pSyzSyukei)->ResetShinkokuData(_T("310101"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("310102"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("310103"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("340101"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("340102"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("340103"));
		//--------------------
//		(*m_pSyzSyukei)->SetShinkokuData(_T("310101"), F402_1A);
//		(*m_pSyzSyukei)->SetShinkokuData(_T("310102"), F402_1B);
//		(*m_pSyzSyukei)->SetShinkokuData(_T("310103"), F402_1C);
//		(*m_pSyzSyukei)->SetShinkokuData(_T("340101"), F402_1A);
//		(*m_pSyzSyukei)->SetShinkokuData(_T("340102"), F402_1B);
//		(*m_pSyzSyukei)->SetShinkokuData(_T("340103"), F402_1C);
		//<-------------------

		// 課税資産の譲渡等の対価の額
		(*m_pSyzSyukei)->SetShinkokuData(_T("310201"), F402_1_1A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310202"), F402_1_1B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310203"), F402_1_1C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340201"), F402_1_1A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340202"), F402_1_1B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340203"), F402_1_1C);

		// 消費税額
		if ( F402 & 0x08 ) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("340301"), F402_2A);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("340301"));
		}
		if ( F402 & 0x80 ) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("340302"), F402_2B);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("340302"));
		}
		if ( F402 & 0x800 ) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("340303"), F402_2C);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("340303"));
		}

		// 貸倒回収に係る消費税額
		(*m_pSyzSyukei)->SetShinkokuData(_T("322501"), F402_3A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322502"), F402_3B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322503"), F402_3C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340401"), F402_3A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340402"), F402_3B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340403"), F402_3C);

		// 返還等対価に係る税額
		(*m_pSyzSyukei)->SetShinkokuData(_T("310701"), F402_5A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310702"), F402_5B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("310703"), F402_5C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340601"), F402_5A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340602"), F402_5B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340603"), F402_5C);

		// 貸倒れに係る税額
		(*m_pSyzSyukei)->SetShinkokuData(_T("311001"), F402_6A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("311002"), F402_6B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("311003"), F402_6C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340701"), F402_6A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340702"), F402_6B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("340703"), F402_6C);

		//--->yoshida191118
		//経過措置あり→無し→再度ありに変更した際、差引税額より下が再集計かかっていなかったのを修正
		//--->191205
		(*m_pSyzSyukei)->ResetShinkokuData(_T("340901"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("340902"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("340903"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("341001"));
		//<---191205
		(*m_pSyzSyukei)->ResetShinkokuData(_T("341002"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("341003"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("341502"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("341503"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("341602"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("341603"));
		//<------

	}
	else {
	
	}
}


//=========================================================
// 手入力項目あり？
//---------------------------------------------------------
// 返送値
//	TRUE		:		あり
//	FALSE		:		なし
//=========================================================
BOOL CShinFhyo4_02::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( F402 & 0x888 ) {
			ret = TRUE;
		}
	}

	return ret;
}