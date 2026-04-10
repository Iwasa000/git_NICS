// ShinFhyo4_01.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo4_01.h"
#include "ShinFhyo4_01idx.h"
#include "H31HyoView.h"

// CShinFhyo4_01 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo4_01, CSyzBaseDlg)

CShinFhyo4_01::CShinFhyo4_01(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo4_01::IDD, pParent)
	, m_pParent ( pParent )
	, m_pSnFhyo4_01Data ( NULL )
	, m_pSnHeadData ( NULL )
	, m_pSyzSyukei( NULL )
{
	memset ( F401_1X, '\0', sizeof ( F401_1X ) );
	memset ( F401_1D, '\0', sizeof ( F401_1D ) );
	memset ( F401_1E, '\0', sizeof ( F401_1E ) );
	memset ( F401_1F, '\0', sizeof ( F401_1F ) );

	memset ( F401_1_1X, '\0', sizeof ( F401_1_1X ) );
	memset ( F401_1_1D, '\0', sizeof ( F401_1_1D ) );
	memset ( F401_1_1E, '\0', sizeof ( F401_1_1E ) );
	memset ( F401_1_1F, '\0', sizeof ( F401_1_1F ) );

	memset ( F401_2X, '\0', sizeof ( F401_2X ) );
	memset ( F401_2D, '\0', sizeof ( F401_2D ) );
	memset ( F401_2E, '\0', sizeof ( F401_2E ) );
	memset ( F401_2F, '\0', sizeof ( F401_2F ) );

	memset ( F401_3X, '\0', sizeof ( F401_3X ) );
	memset ( F401_3D, '\0', sizeof ( F401_3D ) );
	memset ( F401_3E, '\0', sizeof ( F401_3E ) );
	memset ( F401_3F, '\0', sizeof ( F401_3F ) );

	memset ( F401_4X, '\0', sizeof ( F401_4X ) );
	memset ( F401_4D, '\0', sizeof ( F401_4D ) );
	memset ( F401_4E, '\0', sizeof ( F401_4E ) );
	memset ( F401_4F, '\0', sizeof ( F401_4F ) );

	memset ( F401_5X, '\0', sizeof ( F401_5X ) );
	memset ( F401_5D, '\0', sizeof ( F401_5D ) );
	memset ( F401_5E, '\0', sizeof ( F401_5E ) );
	memset ( F401_5F, '\0', sizeof ( F401_5F ) );

	memset ( F401_6X, '\0', sizeof ( F401_6X ) );
	memset ( F401_6D, '\0', sizeof ( F401_6D ) );
	memset ( F401_6E, '\0', sizeof ( F401_6E ) );
	memset ( F401_6F, '\0', sizeof ( F401_6F ) );

	memset ( F401_7X, '\0', sizeof ( F401_7X ) );
	memset ( F401_7D, '\0', sizeof ( F401_7D ) );
	memset ( F401_7E, '\0', sizeof ( F401_7E ) );
	memset ( F401_7F, '\0', sizeof ( F401_7F ) );

	memset ( F401_8X, '\0', sizeof ( F401_8X ) );
	memset ( F401_8D, '\0', sizeof ( F401_8D ) );
	memset ( F401_8E, '\0', sizeof ( F401_8E ) );
	memset ( F401_8F, '\0', sizeof ( F401_8F ) );

	memset ( F401_9X, '\0', sizeof ( F401_9X ) );
	memset ( F401_9D, '\0', sizeof ( F401_9D ) );
	memset ( F401_9E, '\0', sizeof ( F401_9E ) );
	memset ( F401_9F, '\0', sizeof ( F401_9F ) );

	memset ( F401_10F, '\0', sizeof ( F401_10F ) );

	memset ( F401_11X, '\0', sizeof ( F401_11X ) );
	memset ( F401_11E, '\0', sizeof ( F401_11E ) );
	memset ( F401_11F, '\0', sizeof ( F401_11F ) );

	memset ( F401_12X, '\0', sizeof ( F401_12X ) );
	memset ( F401_12E, '\0', sizeof ( F401_12E ) );
	memset ( F401_12F, '\0', sizeof ( F401_12F ) );

	memset ( F401_13X, '\0', sizeof ( F401_13X ) );
	memset ( F401_13E, '\0', sizeof ( F401_13E ) );
	memset ( F401_13F, '\0', sizeof ( F401_13F ) );

	memset ( F401_14X, '\0', sizeof ( F401_14X ) );
	memset ( F401_14E, '\0', sizeof ( F401_14E ) );
	memset ( F401_14F, '\0', sizeof ( F401_14F ) );

	memset ( F401_15X, '\0', sizeof ( F401_15X ) );
	memset ( F401_15E, '\0', sizeof ( F401_15E ) );
	memset ( F401_15F, '\0', sizeof ( F401_15F ) );

	memset ( F401_16F, '\0', sizeof ( F401_16F ) );

	F401 = 0;
	F401_1c = 0;

	m_curwnd = -1;
}

CShinFhyo4_01::~CShinFhyo4_01()
{
}

void CShinFhyo4_01::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo4_01Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo4_01Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo4_01, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CShinFhyo4_01 メッセージ ハンドラ

BOOL CShinFhyo4_01::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2);
	m_BottomIdx = 187;//コントロール一番最下のDiagのID指定。これで縦スクロール出る

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo4_01::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo4_01::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo4_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
int CShinFhyo4_01::Init()
{
	if ( (*m_pSnFhyo4_01Data) == NULL ) {
		return -1;
	}

	// データ読込み
	ReadData();

	// 読み込みデータを集計モジュールにセット
	ReadData2();

	MemoryReget();

	// ダイアグラム初期属性セット
	InitDiagAttr();

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		m_Fhyo4_01Diag01.ModifyItem ( 4, DIAG_MDFY_TRANSPARENT );
	}

	// 令和2年4月1日改正様式を使用するのチェックがONの場合のみ
	// 「経過措置対象・・・」の文言を表示
	if ( !( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) ) {
		m_Fhyo4_01Diag01.ModifyItem ( 2, DIAG_MDFY_TRANSPARENT );
	}
	
	// 画面出力
	DispFh4_01();

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
int CShinFhyo4_01::InitInfo(CH31SnFhyo4_01Data **pSnFhyo4_01Data, CSnHeadData **pSnHeadData, CSyzSyukei **pSyzSyukei, CArithEx *pArith)
{
	ASSERT ( pSnFhyo4_01Data );
	if ( pSnFhyo4_01Data == NULL )
		return -1;

	ASSERT ( pSnHeadData );
	if ( pSnHeadData == NULL )
		return -1;

	ASSERT(pSyzSyukei);
	if (pSyzSyukei == NULL)
		return -1;

	ASSERT( pArith );
	if ( pArith == NULL ) {
		return -1;
	}

	m_pSnHeadData = pSnHeadData;

	m_pSnFhyo4_01Data = pSnFhyo4_01Data;

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
int CShinFhyo4_01::InitDiagAttr()
{
	int cnt = 0;

	while ( 1 ) {
		if ( Fhyo4_01[cnt].Index == 0 )
			break;

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo4_01[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				//if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) )
				if( ( !((CH31HyoView*)m_pParent)->IsCrossOverR050930() && IsUseTokurei() )	// 20231025 令和5年9月30日をまたぐ場合は入力可
					&& ( ( Fhyo4_01[cnt].Index == ID401_1_1D ) || ( Fhyo4_01[cnt].Index == ID401_1_1E ) ) ) {
					Flg = 0;
				}
				else {
					Flg = Fhyo4_01[cnt].UnConnect;
				}
			}
			else {
				Flg = Fhyo4_01[cnt].Connect;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL2, Fhyo4_01[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo4_01[cnt].Connect );
		cnt++;

	}

	m_Fhyo4_01Diag02.EnableDelete ( 0 );

	return 0;
}

//======================================
// 初期ポジションセット
//======================================
void CShinFhyo4_01::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		//index = m_Fhyo4_01Diag02.GetPosition();
		if ( ( index = m_Fhyo4_01Diag02.GetPosition() ) != -1 ) {
			m_Fhyo4_01Diag01.SetFocus();	// ダミーでセット
			m_Fhyo4_01Diag02.SetPosition ( index );
			m_Fhyo4_01Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo4_01Diag01.SetFocus();	// ダミーでセット
	m_Fhyo4_01Diag02.SetFocus();


	//if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) ) {
	if( !((CH31HyoView*)m_pParent)->IsCrossOverR050930() && IsUseTokurei() ){	// 20231025　令和5年9月30日をまたぐ場合は入力可
		// 消費税額D欄
		m_Fhyo4_01Diag02.SetPosition ( ID401_2D );
	}
	else {
		// 課税標準額D欄
		m_Fhyo4_01Diag02.SetPosition ( ID401_1_1D );
	}

}

//=================================================
// 入力項目色変え
//-------------------------------------------------
// 引数
//		id		:	ダイアグラムID
//		index	:	ダイアグラム内インデックス
//		sign	:	変更サイン
//=================================================
void CShinFhyo4_01::ChangeColor(unsigned short id, short Index, int sign)
{
	DIAGRAM_ATTRIBUTE DA;

	diag_getattr ( id, Index, &DA, CIcsdiagctrl );
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
		default:	// 実額入力（クリーム色）
			DA.attr_bcolor = BC_CREAM;
	}

	diag_setattr ( id, Index, &DA, FALSE, CIcsdiagctrl );
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
int CShinFhyo4_01::UpDownFocusMoveCheck(int Index, int Move)
{
	int idx = 0;
	int i = 0, j = 0;

	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY4_01CNT; i++ ) {
		if ( Index == FCS_Fhyo4_01[i].IDNo )
			break;
	}

	// 現在ポジションセット
	idx = FCS_Fhyo4_01[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) {			//　下方向
		for ( j = i + 1; j < FCS_FHY4_01CNT; j++ ) {
			m_Fhyo4_01Diag02.GetAttribute ( FCS_Fhyo4_01[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo4_01[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {		//　上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo4_01Diag02.GetAttribute ( FCS_Fhyo4_01[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo4_01[j].IDNo;
				break;
			}
		}
	}

	return idx;
}

//================================
// データの読込み
//--------------------------------
// 返送値
//		0		:	正常終了
//================================
int CShinFhyo4_01::ReadData()
{
	// 課税標準額
	memmove ( F401_1X, ( *m_pSnFhyo4_01Data )->Sn_F401_1X, MONY_BUF_SIZE );
	memmove ( F401_1D, ( *m_pSnFhyo4_01Data )->Sn_F401_1D, MONY_BUF_SIZE );
	memmove ( F401_1E, ( *m_pSnFhyo4_01Data )->Sn_F401_1E, MONY_BUF_SIZE );
	memmove ( F401_1F, ( *m_pSnFhyo4_01Data )->Sn_F401_1F, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
	memmove ( F401_1_1X, ( *m_pSnFhyo4_01Data )->Sn_F401_1_1X, MONY_BUF_SIZE );
	memmove ( F401_1_1D, ( *m_pSnFhyo4_01Data )->Sn_F401_1_1D, MONY_BUF_SIZE );
	memmove ( F401_1_1E, ( *m_pSnFhyo4_01Data )->Sn_F401_1_1E, MONY_BUF_SIZE );
	memmove ( F401_1_1F, ( *m_pSnFhyo4_01Data )->Sn_F401_1_1F, MONY_BUF_SIZE );

	// 消費税額
	memmove ( F401_2X, ( *m_pSnFhyo4_01Data )->Sn_F401_2X, MONY_BUF_SIZE );
	memmove ( F401_2D, ( *m_pSnFhyo4_01Data )->Sn_F401_2D, MONY_BUF_SIZE );
	memmove ( F401_2E, ( *m_pSnFhyo4_01Data )->Sn_F401_2E, MONY_BUF_SIZE );
	memmove ( F401_2F, ( *m_pSnFhyo4_01Data )->Sn_F401_2F, MONY_BUF_SIZE );
	F401 = (*m_pSnFhyo4_01Data)->Sn_F401_2sw;	// 手入力サイン

	// 貸倒回収に係る消費税額
	memmove ( F401_3X, ( *m_pSnFhyo4_01Data )->Sn_F401_3X, MONY_BUF_SIZE );
	memmove ( F401_3D, ( *m_pSnFhyo4_01Data )->Sn_F401_3D, MONY_BUF_SIZE );
	memmove ( F401_3E, ( *m_pSnFhyo4_01Data )->Sn_F401_3E, MONY_BUF_SIZE );
	memmove ( F401_3F, ( *m_pSnFhyo4_01Data )->Sn_F401_3F, MONY_BUF_SIZE );

	// 控除対象仕入税額
	memmove ( F401_4X, ( *m_pSnFhyo4_01Data )->Sn_F401_4X, MONY_BUF_SIZE );
	memmove ( F401_4D, ( *m_pSnFhyo4_01Data )->Sn_F401_4D, MONY_BUF_SIZE );
	memmove ( F401_4E, ( *m_pSnFhyo4_01Data )->Sn_F401_4E, MONY_BUF_SIZE );
	memmove ( F401_4F, ( *m_pSnFhyo4_01Data )->Sn_F401_4F, MONY_BUF_SIZE );

	// 返還等対価に係る消費税額
	memmove ( F401_5X, ( *m_pSnFhyo4_01Data )->Sn_F401_5X, MONY_BUF_SIZE );
	memmove ( F401_5D, ( *m_pSnFhyo4_01Data )->Sn_F401_5D, MONY_BUF_SIZE );
	memmove ( F401_5E, ( *m_pSnFhyo4_01Data )->Sn_F401_5E, MONY_BUF_SIZE );
	memmove ( F401_5F, ( *m_pSnFhyo4_01Data )->Sn_F401_5F, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	memmove ( F401_6X, ( *m_pSnFhyo4_01Data )->Sn_F401_6X, MONY_BUF_SIZE );
	memmove ( F401_6D, ( *m_pSnFhyo4_01Data )->Sn_F401_6D, MONY_BUF_SIZE );
	memmove ( F401_6E, ( *m_pSnFhyo4_01Data )->Sn_F401_6E, MONY_BUF_SIZE );
	memmove ( F401_6F, ( *m_pSnFhyo4_01Data )->Sn_F401_6F, MONY_BUF_SIZE );

	// 控除税額小計
	memmove ( F401_7X, ( *m_pSnFhyo4_01Data )->Sn_F401_7X, MONY_BUF_SIZE );
	memmove ( F401_7D, ( *m_pSnFhyo4_01Data )->Sn_F401_7D, MONY_BUF_SIZE );
	memmove ( F401_7E, ( *m_pSnFhyo4_01Data )->Sn_F401_7E, MONY_BUF_SIZE );
	memmove ( F401_7F, ( *m_pSnFhyo4_01Data )->Sn_F401_7F, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( F401_8X, ( *m_pSnFhyo4_01Data )->Sn_F401_8X, MONY_BUF_SIZE );
	memmove ( F401_8D, ( *m_pSnFhyo4_01Data )->Sn_F401_8D, MONY_BUF_SIZE );
	memmove ( F401_8E, ( *m_pSnFhyo4_01Data )->Sn_F401_8E, MONY_BUF_SIZE );
	memmove ( F401_8F, ( *m_pSnFhyo4_01Data )->Sn_F401_8F, MONY_BUF_SIZE );

	// 差引税額
	memmove ( F401_9X, ( *m_pSnFhyo4_01Data )->Sn_F401_9X, MONY_BUF_SIZE );
	memmove ( F401_9D, ( *m_pSnFhyo4_01Data )->Sn_F401_9D, MONY_BUF_SIZE );
	memmove ( F401_9E, ( *m_pSnFhyo4_01Data )->Sn_F401_9E, MONY_BUF_SIZE );
	memmove ( F401_9F, ( *m_pSnFhyo4_01Data )->Sn_F401_9F, MONY_BUF_SIZE );

	// 合計差引税額
	memmove ( F401_10F, ( *m_pSnFhyo4_01Data )->Sn_F401_10F, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( F401_11X, ( *m_pSnFhyo4_01Data )->Sn_F401_11X, MONY_BUF_SIZE );
	memmove ( F401_11E, ( *m_pSnFhyo4_01Data )->Sn_F401_11E, MONY_BUF_SIZE );
	memmove ( F401_11F, ( *m_pSnFhyo4_01Data )->Sn_F401_11F, MONY_BUF_SIZE );

	// 差引税額
	memmove ( F401_12X, ( *m_pSnFhyo4_01Data )->Sn_F401_12X, MONY_BUF_SIZE );
	memmove ( F401_12E, ( *m_pSnFhyo4_01Data )->Sn_F401_12E, MONY_BUF_SIZE );
	memmove ( F401_12F, ( *m_pSnFhyo4_01Data )->Sn_F401_12F, MONY_BUF_SIZE );

	// 合計差引地方消費税の課税標準となる消費税額
	memmove ( F401_13X, ( *m_pSnFhyo4_01Data )->Sn_F401_13X, MONY_BUF_SIZE );
	memmove ( F401_13E, ( *m_pSnFhyo4_01Data )->Sn_F401_13E, MONY_BUF_SIZE );
	memmove ( F401_13F, ( *m_pSnFhyo4_01Data )->Sn_F401_13F, MONY_BUF_SIZE );

	// 還付額
	memmove ( F401_14X, ( *m_pSnFhyo4_01Data )->Sn_F401_14X, MONY_BUF_SIZE );
	memmove ( F401_14E, ( *m_pSnFhyo4_01Data )->Sn_F401_14E, MONY_BUF_SIZE );
	memmove ( F401_14F, ( *m_pSnFhyo4_01Data )->Sn_F401_14F, MONY_BUF_SIZE );

	// 納付額
	memmove ( F401_15X, ( *m_pSnFhyo4_01Data )->Sn_F401_15X, MONY_BUF_SIZE );
	memmove ( F401_15E, ( *m_pSnFhyo4_01Data )->Sn_F401_15E, MONY_BUF_SIZE );
	memmove ( F401_15F, ( *m_pSnFhyo4_01Data )->Sn_F401_15F, MONY_BUF_SIZE );

	// 合計差引譲渡割額
	memmove ( F401_16F, ( *m_pSnFhyo4_01Data )->Sn_F401_16F, MONY_BUF_SIZE );

	return 0;
}

//=======================================
// データの書込み
//---------------------------------------
// 返送値
//		0	:	正常終了
//=======================================
int CShinFhyo4_01::WriteData()
{
	// 課税標準額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_1X, F401_1X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_1D, F401_1D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_1E, F401_1E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_1F, F401_1F, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_1_1X, F401_1_1X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_1_1D, F401_1_1D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_1_1E, F401_1_1E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_1_1F, F401_1_1F, MONY_BUF_SIZE );

	// 消費税額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_2X, F401_2X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_2D, F401_2D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_2E, F401_2E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_2F, F401_2F, MONY_BUF_SIZE );
	(*m_pSnFhyo4_01Data)->Sn_F401_2sw = F401;

	// 貸倒回収に係る消費税額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_3X, F401_3X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_3D, F401_3D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_3E, F401_3E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_3F, F401_3F, MONY_BUF_SIZE );

	// 控除対象仕入税額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_4X, F401_4X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_4D, F401_4D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_4E, F401_4E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_4F, F401_4F, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_5X, F401_5X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_5D, F401_5D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_5E, F401_5E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_5F, F401_5F, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_6X, F401_6X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_6D, F401_6D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_6E, F401_6E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_6F, F401_6F, MONY_BUF_SIZE );

	// 控除税額小計
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_7X, F401_7X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_7D, F401_7D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_7E, F401_7E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_7F, F401_7F, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_8X, F401_8X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_8D, F401_8D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_8E, F401_8E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_8F, F401_8F, MONY_BUF_SIZE );

	// 差引税額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_9X, F401_9X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_9D, F401_9D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_9E, F401_9E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_9F, F401_9F, MONY_BUF_SIZE );

	// 合計差引税額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_10F, F401_10F, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_11X, F401_11X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_11E, F401_11E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_11F, F401_11F, MONY_BUF_SIZE );

	// 差引税額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_12X, F401_12X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_12E, F401_12E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_12F, F401_12F, MONY_BUF_SIZE );

	// 合計差引地方消費税の課税標準となる消費税額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_13X, F401_13X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_13E, F401_13E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_13F, F401_13F, MONY_BUF_SIZE );

	// 還付額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_14X, F401_14X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_14E, F401_14E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_14F, F401_14F, MONY_BUF_SIZE );

	// 納税額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_15X, F401_15X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_15E, F401_15E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_15F, F401_15F, MONY_BUF_SIZE );

	// 合計差引譲渡割額
	memmove ( ( *m_pSnFhyo4_01Data )->Sn_F401_16F, F401_16F, MONY_BUF_SIZE );

	return 0;
}


//==========================
// 画面出力
//==========================
void CShinFhyo4_01::DispFh4_01()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 画面の初期化
	for ( int i = 3; i <=5; i++ ) {
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}
	for ( int i = ID401_1X; i <= ID401_16F; i++ ) {
		dsp_cls ( IDC_ICSDIAG8CTRL2, ( short )i );
	}

	// 手入力サイン初期化
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		F401 = 0;
		F401_1c = 0;
	}

	// 期間・氏名の表示
	char buf[512] = { 0 };
	char yy, mm, dd;
	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s ( buf, sizeof ( buf ), _T( "%02x.%02x.%02x" ), ( yy & 0xff ), ( mm & 0xff ), ( dd & 0xff ) );
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL2, 4, ( struct IUnknow* )&DiagData, CIcsdiagctrl );	// 自

	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	sprintf_s ( buf, sizeof ( buf ), _T( "%02x.%02x.%02x" ), ( yy & 0xff ), ( mm & 0xff ), ( dd & 0xff ) );
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL2, 5, ( struct IUnknow* )&DiagData, CIcsdiagctrl );	// 至

	memset ( buf, '\0', sizeof ( buf ) );
	if ( ( *m_pSnHeadData )->IsSoleProprietor() ) {
		memmove ( buf, ( *m_pSnHeadData )->Sn_DIHYO, 40 );
	}
	else {
		memmove ( buf, ( *m_pSnHeadData )->Sn_CONAM, 40 );
	}
	DiagData.data_disp = ( char* )buf;
	diag_setdata ( IDC_ICSDIAG8CTRL2, 3, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 氏名

	// 全データ表示
	Disp_f401data ( 0 );
}

//================================================
// 金額表示
//------------------------------------------------
// 引数
//		Id		:	ダイアグラムID
//		Index	:	ダイアグラム内インデックス
//		Mony	:	表示金額
//================================================
void CShinFhyo4_01::dsp_prs(unsigned short Id, short Index, char *Mony)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	memmove ( DiagData.data_val, Mony, MONY_BUF_SIZE );
	diag_setdata ( Id, Index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
}

//================================================
// 金額表示クリア
//------------------------------------------------
// 引数
//		Id		:	ダイアグラムID
//		Index	:	ダイアグラム内インデックス
//================================================
void CShinFhyo4_01::dsp_cls(unsigned short Id, short Index )
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	switch ( Index ) {
		case 4:		// 課税期間 自
		case 5:		// 課税期間 至
		case 3:		// 氏名又は、名称
			diag_clear ( Id, Index, TRUE, CIcsdiagctrl );
			break;
		default:
			memset ( DiagData.data_val, '\0', MONY_BUF_SIZE );
			diag_setdata ( Id, Index, ( struct IUnknown *)&DiagData, CIcsdiagctrl );
			break;
	}
}

//===============================
// データ表示
//-------------------------------
// 引数
//		idx		:	0 = 全データ
//===============================
void CShinFhyo4_01::Disp_f401data ( int idx )
{
	switch ( idx ) {
		case 0:
		case 1:
			f401_1data();
			f401_1_1data();
		case 2:
			f401_2data();
		case 3:
			f401_3data();
		case 4:
		case 5://yoshida190706　控除対象仕入税額も更新。付表５から計算かかった値を表示する為
			f401_4data();
			f401_5data();
		case 6:
			f401_6data();
		case 7:
			f401_7data();
		case 8:
			f401_8data();
		case 9:
			f401_9data();
		case 10:
			f401_10data();
		case 11:
			f401_11data();
		case 12:
			f401_12data();
		case 13:
			f401_13data();
		case 14:
			f401_14data();
		case 15:
			f401_15data();
		case 16:
			f401_16data();
		default:
			break;
	}

}

//===============================
// 課税標準額表示
//===============================
void CShinFhyo4_01::f401_1data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_1X, F401_1X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_1D, F401_1D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_1E, F401_1E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_1F, F401_1F );
}

//===============================
// 課税資産の譲渡等の対価の額
//===============================
void CShinFhyo4_01::f401_1_1data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_1_1X, F401_1_1X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_1_1D, F401_1_1D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_1_1E, F401_1_1E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_1_1F, F401_1_1F );
}

//===============================
// 消費税額
//===============================
void CShinFhyo4_01::f401_2data()
{
	if( ((*m_pSnHeadData)->Sn_Sign4 & 0x80 ) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) ){
		ChangeColor(IDC_ICSDIAG8CTRL2, ID401_2D, ( F401 & 0x80 ) ? 3 : 0);
		ChangeColor(IDC_ICSDIAG8CTRL2, ID401_2E, ( F401 & 0x800 ) ? 3 : 0);
	}
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_2X, F401_2X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_2D, F401_2D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_2E, F401_2E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_2F, F401_2F );
}

//===============================
// 貸倒回収に係る消費税額
//===============================
void CShinFhyo4_01::f401_3data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_3X, F401_3X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_3D, F401_3D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_3E, F401_3E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_3F, F401_3F );
}

//===============================
// 控除対象仕入税額
//===============================
void CShinFhyo4_01::f401_4data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_4X, F401_4X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_4D, F401_4D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_4E, F401_4E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_4F, F401_4F );
}

//===============================
// 返還等対価に係る税額
//===============================
void CShinFhyo4_01::f401_5data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_5X, F401_5X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_5D, F401_5D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_5E, F401_5E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_5F, F401_5F );
}

//===============================
// 貸倒れに係る税額
//===============================
void CShinFhyo4_01::f401_6data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_6X, F401_6X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_6D, F401_6D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_6E, F401_6E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_6F, F401_6F );
}

//===============================
// 控除税額小計
//===============================
void CShinFhyo4_01::f401_7data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_7X, F401_7X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_7D, F401_7D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_7E, F401_7E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_7F, F401_7F );
}

//===============================
// 控除不足還付税額
//===============================
void CShinFhyo4_01::f401_8data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_8X, F401_8X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_8D, F401_8D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_8E, F401_8E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_8F, F401_8F );
}

//===============================
// 差引税額
//===============================
void CShinFhyo4_01::f401_9data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_9X, F401_9X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_9D, F401_9D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_9E, F401_9E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_9F, F401_9F );
}

//===============================
// 合計差引税額
//===============================
void CShinFhyo4_01::f401_10data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_10F, F401_10F );
}

//===============================
// 控除不足還付税額
//===============================
void CShinFhyo4_01::f401_11data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_11X, F401_11X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_11E, F401_11E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_11F, F401_11F );
}

//===============================
// 差引税額
//===============================
void CShinFhyo4_01::f401_12data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_12X, F401_12X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_12E, F401_12E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_12F, F401_12F );
}

//=============================================
// 合計差引地方消費税の課税標準となる消費税額
//=============================================
void CShinFhyo4_01::f401_13data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_13X, F401_13X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_13E, F401_13E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_13F, F401_13F );
}

//===============================
// 還付額
//===============================
void CShinFhyo4_01::f401_14data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_14X, F401_14X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_14E, F401_14E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_14F, F401_14F );
}

//===============================
// 納税額
//===============================
void CShinFhyo4_01::f401_15data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_15X, F401_15X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_15E, F401_15E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_15F, F401_15F );
}

//===============================
// 合計差引譲渡割額
//===============================
void CShinFhyo4_01::f401_16data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID401_16F, F401_16F );
}

//===============================
// 各入力データ抽出
//===============================
void CShinFhyo4_01::GetAllDiagData()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	// 1. 課税標準額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_1X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_1X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_1D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_1D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_1E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_1E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_1F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_1F, DiagData.data_val, MONY_BUF_SIZE );

	// 1-1. 課税資産の譲渡等の対価の額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_1_1X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_1_1X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_1_1D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_1_1D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_1_1E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_1_1E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_1_1F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_1_1F, DiagData.data_val, MONY_BUF_SIZE );

	// 2. 消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_2X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_2X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_2D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_2D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_2E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_2E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_2F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_2F, DiagData.data_val, MONY_BUF_SIZE );

	// 3. 貸倒回収に係る消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_3X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_3X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_3D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_3D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_3E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_3E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_3F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_3F, DiagData.data_val, MONY_BUF_SIZE );

	// 4. 控除対象仕入税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_4X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_4X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_4D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_4D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_4E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_4E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_4F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_4F, DiagData.data_val, MONY_BUF_SIZE );

	// 5. 返還等対価に係る税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_5X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_5X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_5D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_5D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_5E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_5E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_5F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_5F, DiagData.data_val, MONY_BUF_SIZE );

	// 6. 貸倒れに係る税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_6X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_6X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_6D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_6D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_6E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_6E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_6F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_6F, DiagData.data_val, MONY_BUF_SIZE );

	// 7. 控除税額小計
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_7X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_7X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_7D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_7D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_7E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_7E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_7F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_7F, DiagData.data_val, MONY_BUF_SIZE );

	// 8. 控除不足還付税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_8X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_8X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_8D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_8D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_8E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_8E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_8F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_8F, DiagData.data_val, MONY_BUF_SIZE );

	// 9. 差引税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_9X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_9X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_9D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_9D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_9E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_9E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_9F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_9F, DiagData.data_val, MONY_BUF_SIZE );

	// 10. 合計差引税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_10F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_10F, DiagData.data_val, MONY_BUF_SIZE );

	// 11. 控除不足還付税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_11X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_11X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_11E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_11E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_11F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_11F, DiagData.data_val, MONY_BUF_SIZE );

	// 12. 差引税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_12X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_12X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_12E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_12E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_12F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_12F, DiagData.data_val, MONY_BUF_SIZE );

	// 13. 合計差引地方消費税の課税標準となる消費税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_13X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_13X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_13E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_13E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_13F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_13F, DiagData.data_val, MONY_BUF_SIZE );

	// 14. 還付額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_14X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_14X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_14E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_14E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_14F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_14F, DiagData.data_val, MONY_BUF_SIZE );

	// 15. 納税額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_15X, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_15X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_15E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_15E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_15F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_15F, DiagData.data_val, MONY_BUF_SIZE );

	// 16. 合計差引譲渡割額
	diag_getdata ( IDC_ICSDIAG8CTRL2, ID401_16F, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( F401_16F, DiagData.data_val, MONY_BUF_SIZE );
}


BEGIN_EVENTSINK_MAP(CShinFhyo4_01, CSyzBaseDlg)
ON_EVENT(CShinFhyo4_01, IDC_ICSDIAG8CTRL2, 1, CShinFhyo4_01::EditONIcsdiag8ctrl2, VTS_I2)
ON_EVENT(CShinFhyo4_01, IDC_ICSDIAG8CTRL2, 2, CShinFhyo4_01::EditOFFIcsdiag8ctrl2, VTS_I2)
ON_EVENT(CShinFhyo4_01, IDC_ICSDIAG8CTRL2, 3, CShinFhyo4_01::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo4_01::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;
}

void CShinFhyo4_01::EditOFFIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char CkMy[MONY_BUF_SIZE] = {0};

	switch ( index ) {
		case ID401_1D:
		case ID401_1E:
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			diag_getdata ( IDC_ICSDIAG8CTRL2, index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( index == ID401_1D ) {
				if ( m_pArith->l_cmp ( F401_1D, CkMy ) ) {
					if ( !( F401 & 0x80 ) ) {
						F401 |= 0x10;
					}
				}
			}
			else if ( index == ID401_1E ) {
				if ( m_pArith->l_cmp ( F401_1E, CkMy ) ) {
					if ( !( F401 & 0x800 ) ) {
						F401 |= 0x100;
					}
				}
			}
			SyukeiDataSet(index);
			Disp_f401data(1);
			break;
		case ID401_1_1D:	// 1-1. 課税資産の譲渡等の対価の額
		case ID401_1_1E:
			SyukeiDataSet(index);
			Disp_f401data(1);
			break;
		case ID401_2D:		// 2. 消費税額
		case ID401_2E:
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			diag_getdata ( IDC_ICSDIAG8CTRL2, index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( index == ID401_2D ) {
				if ( m_pArith->l_cmp ( F401_2D, CkMy ) ) {
					F401 |= 0x80;
					F401 &= ~0x10;
				}
			}
			else if ( index == ID401_2E ) {
				if ( m_pArith->l_cmp ( F401_2E, CkMy ) ) {
					F401 |= 0x800;
					F401 &= ~0x100;
				}
			}
			SyukeiDataSet(index);
			Disp_f401data(2);
			break;
		case ID401_3D:	// 3. 貸倒回収に係る消費税額
		case ID401_3E:
			SyukeiDataSet(index);
			Disp_f401data(3);
			break;
		case ID401_4D:	// 4. 控除対象仕入れ税額
		case ID401_4E:
			SyukeiDataSet(index);
			Disp_f401data(4);
			break;
		case ID401_5D:	// 5. 返還等対価に係る税額
		case ID401_5E:
			SyukeiDataSet(index);
			Disp_f401data(5);
			break;
		case ID401_6D:	// 6. 貸倒れに係る税額
		case ID401_6E:
			SyukeiDataSet(index);
			Disp_f401data(6);
			break;
		default:
			break;

	}
}

void CShinFhyo4_01::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if ( nChar == 0 ) {
		return;
	}

	// Shift+TABをTABに、TABをEnterに変換
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
		m_Fhyo4_01Diag02.SetPosition ( pos );
	}
	if ( wTerm == VK_DOWN ) {
		pos = UpDownFocusMoveCheck ( index, 0 );
		m_Fhyo4_01Diag02.SetPosition ( pos );
	}

	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_F3 ) ) {
		m_Fhyo4_01Diag02.SetNextPosition();
	}
	else if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) ) {
		m_Fhyo4_01Diag02.SetPrevPosition();
	}

	if ( wTerm == VK_DELETE ) {
		switch ( index ) {
			case ID401_2D:
				F401 &= ~0x80;

				(*m_pSyzSyukei)->ResetShinkokuData( _T("342002") );
				Disp_f401data(0);
				m_Fhyo4_01Diag02.SetPosition ( ID401_2D );
				break;

			case ID401_2E:
				F401 &= ~0x800;

				(*m_pSyzSyukei)->ResetShinkokuData( _T("342003") );
				Disp_f401data(0);
				m_Fhyo4_01Diag02.SetPosition ( ID401_2E );
				break;

			default:
				dsp_cls ( IDC_ICSDIAG8CTRL2, index );
				break;
		}
		
//		dsp_cls ( IDC_ICSDIAG8CTRL2, index );
	}

	int nowpos = m_Fhyo4_01Diag02.GetPosition();
	RECT rectA, rectB;
	GetWindowRect ( &rectA );
	diag_getrect ( IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl );

	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectB.top ) / 2 ) );
	}
}

BOOL CShinFhyo4_01::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ) {
			// 付表4-1更新
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

int CShinFhyo4_01::SyukeiDataSet( int cID )
{
	char			val[MONY_BUF_SIZE] = {0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	diag_getdata(IDC_ICSDIAG8CTRL2, cID, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove( val, DiagData.data_val, MONY_BUF_SIZE );

	switch ( cID ) {
	// 課税標準額
	case ID401_1X:
		memmove(F401_1X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData( _T("312101"), F401_1X);
		(*m_pSyzSyukei)->SetShinkokuData( _T("341801"), F401_1X);
		break;
	case ID401_1D:
		memmove(F401_1D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312102"), F401_1D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("341802"), F401_1D);
		break;
	case ID401_1E:
		memmove(F401_1E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312103"), F401_1E);
		(*m_pSyzSyukei)->SetShinkokuData(_T("341803"), F401_1E);
		break;

	// 課税資産の譲渡等の対価の額
	case ID401_1_1X:
		memmove(F401_1_1X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312201"), F401_1_1X);
		(*m_pSyzSyukei)->SetShinkokuData(_T("341901"), F401_1_1X);
		break;
	case ID401_1_1D:
		memmove(F401_1_1D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312202"), F401_1_1D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("341902"), F401_1_1D);
		break;
	case ID401_1_1E:
		memmove(F401_1_1E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312203"), F401_1_1E);
		(*m_pSyzSyukei)->SetShinkokuData(_T("341903"), F401_1_1E);
		break;

	// 消費税額
	case ID401_2X:
		if( m_pArith->l_cmp( F401_2X, val ) ){
//			F401 |= 0x08;
			memmove(F401_2X, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342001"), F401_2X);
		}
		break;
	case ID401_2D:
		if( m_pArith->l_cmp( F401_2D, val ) ){
			F401 |= 0x80;
			memmove(F401_2D, val, MONY_BUF_SIZE); 
			(*m_pSyzSyukei)->SetShinkokuData(_T("342002"), F401_2D);
		}
		break;
	case ID401_2E:
		if( m_pArith->l_cmp( F401_2E, val ) ){
			F401 |= 0x800;
			memmove(F401_2E, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342003"), F401_2E);
		}
		break;

	// 貸倒回収に係る消費税額
	case ID401_3X:
		memmove(F401_3X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("325001"), F401_3X);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342101"), F401_3X);
		break;
	case ID401_3D:
		memmove(F401_3D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("325002"), F401_3D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342102"), F401_3D);
		break;
	case ID401_3E:
		memmove(F401_3E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("325003"), F401_3E);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342103"), F401_3E);
		break;

	// 控除対象仕入税額
	case ID401_4X:
		memmove(F401_4X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342201"), F401_4X);
		break;
	case ID401_4D:
		memmove(F401_4D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342202"), F401_4D);
		break;
	case ID401_4E:
		memmove(F401_4E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342203"), F401_4E);
		break;

	// 返還等対価に係る税額
	case ID401_5X:
		memmove(F401_5X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312701"), F401_5X);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342301"), F401_5X);
		break;
	case ID401_5D:
		memmove(F401_5D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312702"), F401_5D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342302"), F401_5D);
		break;
	case ID401_5E:
		memmove(F401_5E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312703"), F401_5E);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342303"), F401_5E);
		break;

	// 貸倒れに係る税額
	case ID401_6X:
		memmove(F401_6X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("313001"), F401_6X);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342401"), F401_6X);
		break;
	case ID401_6D:
		memmove(F401_6D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("313002"), F401_6D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342402"), F401_6D);
		break;
	case ID401_6E:
		memmove(F401_6E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("313003"), F401_6E);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342403"), F401_6E);
		break;

	// 控除税額小計
	case ID401_7X:
		memmove(F401_7X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342501"), F401_7X);
		break;
	case ID401_7D:
		memmove(F401_7D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342502"), F401_7D);
		break;
	case ID401_7E:
		memmove(F401_7E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342503"), F401_7E);
		break;

	// 控除不足還付税額
	case ID401_8X:
		memmove(F401_8X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342601"), F401_8X);
		break;
	case ID401_8D:
		memmove(F401_8D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342602"), F401_8D);
		break;
	case ID401_8E:
		memmove(F401_8E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342603"), F401_8E);
		break;

	// 差引税額
	case ID401_9X:
		memmove(F401_9X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342701"), F401_9X);
		break;
	case ID401_9D:
		memmove(F401_9D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342702"), F401_9D);
		break;
	case ID401_9E:
		memmove(F401_9E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342703"), F401_9E);
		break;

	// 合計差引税額
	case ID401_10F:
		memmove(F401_10F, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342800"), F401_10F);
		break;

	// 控除不足還付税額
	case ID401_11X:
		memmove(F401_11X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342901"), F401_11X);
		break;
	case ID401_11E:
		memmove(F401_11E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342903"), F401_11E);
		break;

	// 差引税額
	case ID401_12X:
		memmove(F401_12X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("343001"), F401_12X);
		break;
	case ID401_12E:
		memmove(F401_12E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("343003"), F401_12E);
		break;

	// 合計差引地方消費税の課税標準額となる消費税額
	case ID401_13X:
		memmove(F401_13X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("343101"), F401_13X);
		break;
	case ID401_13E:
		memmove(F401_13E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("343103"), F401_13E);
		break;

	// 還付額
	case ID401_14X:
		memmove(F401_14X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("343201"), F401_14X);
		break;
	case ID401_14E:
		memmove(F401_14E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("343203"), F401_14E);
		break;

	// 納税額
	case ID401_15X:
		memmove(F401_15X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("343301"), F401_15X);
		break;
	case ID401_15E:
		memmove(F401_15E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("343303"), F401_15E);
		break;

	// 合計差引譲渡割額
	case ID401_16F:
		memmove(F401_16F, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("343400"), F401_16F);
		break;
	}

	return MemoryReget();
}

int CShinFhyo4_01::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	MoneyBasejagArray	money;

	money = (*m_pSyzSyukei)->GetShinkokuData(_T("34"));
	int cnt = (int)money.GetCount();
	int incnt = 0;

	// 計算項目を全て再取得
	for (int i = 0; i < cnt; i++ ) {
		incnt = (int)money[i].GetCount();
		for (int j = 0; j < incnt; j++ ) {
			// 課税標準額
			if ( money[i][j].code == _T("341801") ) {
				memmove(F401_1X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("341802")) {
				memmove(F401_1D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("341803")) {
				memmove(F401_1E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("341800")) {
				memmove(F401_1F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 課税資産の譲渡等の対価の額
			if (money[i][j].code == _T("341901")) {
				memmove(F401_1_1X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("341902")) {
				memmove(F401_1_1D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("341903")) {
				memmove(F401_1_1E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("341900")) {
				memmove(F401_1_1F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 消費税額
			if (money[i][j].code == _T("342001")) {
				memmove(F401_2X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342002")) {
				memmove(F401_2D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342003")) {
				memmove(F401_2E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342000")) {
				memmove(F401_2F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 貸倒回収に係る消費税額
			if (money[i][j].code == _T("342101")) {
				memmove(F401_3X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342102")) {
				memmove(F401_3D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342103")) {
				memmove(F401_3E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342100")) {
				memmove(F401_3F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 控除対象仕入税額
			if (money[i][j].code == _T("342201")) {
				memmove(F401_4X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342202")) {
				memmove(F401_4D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342203")) {
				memmove(F401_4E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342200")) {
				memmove(F401_4F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 返還等対価に係る税額
			if (money[i][j].code == _T("342301")) {
				memmove(F401_5X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342302")) {
				memmove(F401_5D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342303")) {
				memmove(F401_5E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342300")) {
				memmove(F401_5F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 貸倒れに係る税額
			if (money[i][j].code == _T("342403")) {
				memmove(F401_6E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342402")) {
				memmove(F401_6D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342401")) {
				memmove(F401_6X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342400")) {
				memmove(F401_6F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 控除税額小計
			if (money[i][j].code == _T("342501")) {
				memmove(F401_7X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342502")) {
				memmove(F401_7D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342503")) {
				memmove(F401_7E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342500")) {
				memmove(F401_7F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 控除不足還付税額
			if (money[i][j].code == _T("342601")) {
				memmove(F401_8X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342602")) {
				memmove(F401_8D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342603")) {
				memmove(F401_8E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342600")) {
				memmove(F401_8F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 差引税額
			if (money[i][j].code == _T("342701")) {
				memmove(F401_9X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342702")) {
				memmove(F401_9D, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342703")) {
				memmove(F401_9E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342700")) {
				memmove(F401_9F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 合計差引税額
			if (money[i][j].code == _T("342800")) {
				memmove(F401_10F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 控除不足還付税額
			if (money[i][j].code == _T("342901")) {
				memmove(F401_11X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342903")) {
				memmove(F401_11E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("342900")) {
				memmove(F401_11F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 差引税額
			if (money[i][j].code == _T("343001")) {
				memmove(F401_12X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("343003")) {
				memmove(F401_12E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("343000")) {
				memmove(F401_12F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 合計差引地方消費税の課税標準額となる消費税額
			if (money[i][j].code == _T("343101")) {
				memmove(F401_13X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("343103")) {
				memmove(F401_13E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("343100")) {
				memmove(F401_13F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 還付額
			if (money[i][j].code == _T("343201")) {
				memmove(F401_14X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("343203")) {
				memmove(F401_14E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("343200")) {
				memmove(F401_14F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 納付額
			if (money[i][j].code == _T("343301")) {
				memmove(F401_15X, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("343303")) {
				memmove(F401_15E, money[i][j].arith, MONY_BUF_SIZE);
			}
			if (money[i][j].code == _T("343300")) {
				memmove(F401_15F, money[i][j].arith, MONY_BUF_SIZE);
			}

			// 合計差引譲渡割額
			if (money[i][j].code == _T("343400")) {
				memmove(F401_16F, money[i][j].arith, MONY_BUF_SIZE);
			}
		}
	}

	return 0;
}

//------------------------------------------
// 読み込みデータを集計モジュールにセット
//------------------------------------------
void CShinFhyo4_01::ReadData2()
{
	// ７・８・９・１０・１１・１２・１３・１４・１５・１６は計算結果が来る箇所なので省く

	//if ( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ) {	// 非連動の場合
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {	// 非連動、入力ロックの場合

		//(*m_pSyzSyukei)->SetShinkokuData(_T("312101"), F401_1X);		
		//(*m_pSyzSyukei)->SetShinkokuData(_T("312102"), F401_1D);
		//(*m_pSyzSyukei)->SetShinkokuData(_T("312103"), F401_1E);
		

		//(*m_pSyzSyukei)->SetShinkokuData(_T("341801"), F401_1X);

		//---->yoshida190621
		(*m_pSyzSyukei)->ResetShinkokuData(_T("312102"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("312103"));

		(*m_pSyzSyukei)->ResetShinkokuData(_T("341802"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("341803"));
		//------------------
//		(*m_pSyzSyukei)->SetShinkokuData(_T("341802"), F401_1D);		// 課税標準額
//		(*m_pSyzSyukei)->SetShinkokuData(_T("341803"), F401_1E);
		//<------------------

		//(*m_pSyzSyukei)->SetShinkokuData(_T("312201"), F401_1_1X);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312202"), F401_1_1D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312203"), F401_1_1E);
		//(*m_pSyzSyukei)->SetShinkokuData(_T("341901"), F401_1_1X);	// 課税資産の譲渡等の対価の額
		(*m_pSyzSyukei)->SetShinkokuData(_T("341902"), F401_1_1D);		// 課税資産の譲渡等の対価の額
		(*m_pSyzSyukei)->SetShinkokuData(_T("341903"), F401_1_1E);

		if ( F401 & 0x80 ) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("312402"), F401_2D);	// 消費税額
			(*m_pSyzSyukei)->SetShinkokuData(_T("342002"), F401_2D);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("312402"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("342002"));
		}
		if ( F401 & 0x800 ) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("312403"), F401_2E);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342003"), F401_2E);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("312403"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("342003"));
		}

		//(*m_pSyzSyukei)->SetShinkokuData(_T("325001"), F401_3X);
		(*m_pSyzSyukei)->SetShinkokuData(_T("325002"), F401_3D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("325003"), F401_3E);
		//(*m_pSyzSyukei)->SetShinkokuData(_T("342101"), F401_3X);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342102"), F401_3D);		// 貸倒回収に係る消費税額
		(*m_pSyzSyukei)->SetShinkokuData(_T("342103"), F401_3E);

		//(*m_pSyzSyukei)->SetShinkokuData(_T("312701"), F401_5X);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312702"), F401_5D);		
		(*m_pSyzSyukei)->SetShinkokuData(_T("312703"), F401_5E);
		//(*m_pSyzSyukei)->SetShinkokuData(_T("342301"), F401_5X);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342302"), F401_5D);		// 返還等対価に係る税額
		(*m_pSyzSyukei)->SetShinkokuData(_T("342303"), F401_5E);

		//(*m_pSyzSyukei)->SetShinkokuData(_T("313001"), F401_6X);
		(*m_pSyzSyukei)->SetShinkokuData(_T("313002"), F401_6D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("313003"), F401_6E);
		//(*m_pSyzSyukei)->SetShinkokuData(_T("342401"), F401_6X);
		(*m_pSyzSyukei)->SetShinkokuData(_T("342402"), F401_6D);		// 貸倒れに係る税額
		(*m_pSyzSyukei)->SetShinkokuData(_T("342403"), F401_6E);

		if ((*m_pSnHeadData)->SVmzsw == 1) {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("341801"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("341901"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("342001"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("342101"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("342201"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("342301"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("342401"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("342501"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("342601"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("342701"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("342801"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("342901"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("343001"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("343101"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("343201"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("343301"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("343401"));
		}
		else {
			// 経過措置なし　旧税率合計を０クリア
			char mony[MONY_BUF_SIZE] = { 0 };
			memset(mony, '\0', MONY_BUF_SIZE);

			(*m_pSyzSyukei)->SetShinkokuData(_T("341801"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("341901"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342001"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342101"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342201"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342301"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342401"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342501"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342601"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342701"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342801"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("342901"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("343001"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("343101"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("343201"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("343301"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("343401"), mony);
		}
	}
	else {
		//if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) ) {
		if( IsUseTokurei() ){
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312102" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312103" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312402" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312403" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "341802" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "341803" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "342002" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "342003" ) );
		}
	}
}

//-----------------------------------------------------------------------------
// 課税取引金額計算書から転記時のフォーカス移動
//-----------------------------------------------------------------------------
void CShinFhyo4_01::MoveFocusInTaxTenki()
{
	m_Fhyo4_01Diag02.SetPosition ( ID401_1_1E );

}


//=========================================================
// 手入力項目あり？
//---------------------------------------------------------
// 返送値
//	TRUE		:		あり
//	FALSE		:		なし
//=========================================================
BOOL CShinFhyo4_01::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( F401 & 0x880 ) {
			ret = TRUE;
		}
	}

	return ret;
}

//=========================================================
// 特例帳票を使用しているか  20231025
//---------------------------------------------------------
// 返送値
//	true		:		使用している
//	false		:		使用していない
//=========================================================
bool CShinFhyo4_01::IsUseTokurei()
{
	bool rt = false;

	if( (*m_pSnHeadData)->m_DispTabSgn&0x80 && (*m_pSnHeadData)->m_DispTabSgn&0x10 ){
		// 特例5-1使用
		rt = true;
	}

	return rt;
}