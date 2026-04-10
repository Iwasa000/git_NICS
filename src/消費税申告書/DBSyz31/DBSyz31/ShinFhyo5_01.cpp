// ShinFhyo5_01.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo5_01.h"
#include "ShinFhyo5_01idx.h"


// CShinFhyo5_01 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo5_01, CSyzBaseDlg)

CShinFhyo5_01::CShinFhyo5_01(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo5_01::IDD, pParent)
	, m_pParent ( pParent )
	, m_pSnHeadData ( NULL )
	, m_pSnFhyo5_01Data( NULL )
	, m_pSyzSyukei ( NULL )
{

	memset ( F501_1X, '\0', sizeof ( F501_1X ) );
	memset ( F501_1D, '\0', sizeof ( F501_1D ) );
	memset ( F501_1E, '\0', sizeof ( F501_1E ) );
	memset ( F501_1F, '\0', sizeof ( F501_1F ) );

	memset ( F501_2X, '\0', sizeof ( F501_2X ) );
	memset ( F501_2D, '\0', sizeof ( F501_2D ) );
	memset ( F501_2E, '\0', sizeof ( F501_2E ) );
	memset ( F501_2F, '\0', sizeof ( F501_2F ) );

	memset ( F501_3X, '\0', sizeof ( F501_3X ) );
	memset ( F501_3D, '\0', sizeof ( F501_3D ) );
	memset ( F501_3E, '\0', sizeof ( F501_3E ) );
	memset ( F501_3F, '\0', sizeof ( F501_3F ) );

	memset ( F501_4X, '\0', sizeof ( F501_4X ) );
	memset ( F501_4D, '\0', sizeof ( F501_4D ) );
	memset ( F501_4E, '\0', sizeof ( F501_4E ) );
	memset ( F501_4F, '\0', sizeof ( F501_4F ) );

	memset ( F501_5X, '\0', sizeof ( F501_5X ) );
	memset ( F501_5D, '\0', sizeof ( F501_5D ) );
	memset ( F501_5E, '\0', sizeof ( F501_5E ) );
	memset ( F501_5F, '\0', sizeof ( F501_5F ) );

	memset ( F501_6X, '\0', sizeof ( F501_6X ) );
	memset ( F501_6D, '\0', sizeof ( F501_6D ) );
	memset ( F501_6E, '\0', sizeof ( F501_6E ) );
	memset ( F501_6F, '\0', sizeof ( F501_6F ) );

	memset ( F501_7X, '\0', sizeof ( F501_7X ) );
	memset ( F501_7D, '\0', sizeof ( F501_7D ) );
	memset ( F501_7E, '\0', sizeof ( F501_7E ) );
	memset ( F501_7F, '\0', sizeof ( F501_7F ) );

	memset ( F501_8X, '\0', sizeof ( F501_8X ) );
	memset ( F501_8D, '\0', sizeof ( F501_8D ) );
	memset ( F501_8E, '\0', sizeof ( F501_8E ) );
	memset ( F501_8F, '\0', sizeof ( F501_8F ) );

	memset ( F501_9X, '\0', sizeof ( F501_9X ) );
	memset ( F501_9D, '\0', sizeof ( F501_9D ) );
	memset ( F501_9E, '\0', sizeof ( F501_9E ) );
	memset ( F501_9F, '\0', sizeof ( F501_9F ) );

	memset ( F501_10X, '\0', sizeof ( F501_10X ) );
	memset ( F501_10D, '\0', sizeof ( F501_10D ) );
	memset ( F501_10E, '\0', sizeof ( F501_10E ) );
	memset ( F501_10F, '\0', sizeof ( F501_10F ) );

	memset ( F501_11X, '\0', sizeof ( F501_11X ) );
	memset ( F501_11D, '\0', sizeof ( F501_11D ) );
	memset ( F501_11E, '\0', sizeof ( F501_11E ) );
	memset ( F501_11F, '\0', sizeof ( F501_11F ) );

	memset ( F501_12X, '\0', sizeof ( F501_12X ) );
	memset ( F501_12D, '\0', sizeof ( F501_12D ) );
	memset ( F501_12E, '\0', sizeof ( F501_12E ) );
	memset ( F501_12F, '\0', sizeof ( F501_12F ) );

	memset ( F501_13X, '\0', sizeof ( F501_13X ) );
	memset ( F501_13D, '\0', sizeof ( F501_13D ) );
	memset ( F501_13E, '\0', sizeof ( F501_13E ) );
	memset ( F501_13F, '\0', sizeof ( F501_13F ) );

	memset ( F501_14X, '\0', sizeof ( F501_14X ) );
	memset ( F501_14D, '\0', sizeof ( F501_14D ) );
	memset ( F501_14E, '\0', sizeof ( F501_14E ) );
	memset ( F501_14F, '\0', sizeof ( F501_14F ) );

	memset ( F501_15X, '\0', sizeof ( F501_15X ) );
	memset ( F501_15D, '\0', sizeof ( F501_15D ) );
	memset ( F501_15E, '\0', sizeof ( F501_15E ) );
	memset ( F501_15F, '\0', sizeof ( F501_15F ) );

	memset ( F501_16X, '\0', sizeof ( F501_16X ) );
	memset ( F501_16D, '\0', sizeof ( F501_16D ) );
	memset ( F501_16E, '\0', sizeof ( F501_16E ) );
	memset ( F501_16F, '\0', sizeof ( F501_16F ) );

	memset ( F501_17X, '\0', sizeof ( F501_17X ) );
	memset ( F501_17D, '\0', sizeof ( F501_17D ) );
	memset ( F501_17E, '\0', sizeof ( F501_17E ) );
	memset ( F501_17F, '\0', sizeof ( F501_17F ) );

	memset ( F501_18X, '\0', sizeof ( F501_18X ) );
	memset ( F501_18D, '\0', sizeof ( F501_18D ) );
	memset ( F501_18E, '\0', sizeof ( F501_18E ) );
	memset ( F501_18F, '\0', sizeof ( F501_18F ) );

	memset ( F501_19X, '\0', sizeof ( F501_19X ) );
	memset ( F501_19D, '\0', sizeof ( F501_19D ) );
	memset ( F501_19E, '\0', sizeof ( F501_19E ) );
	memset ( F501_19F, '\0', sizeof ( F501_19F ) );


	F501_7UP = F501_8UP = F501_9UP = F501_10UP = F501_11UP = F501_12UP = 0;
	F501_5kbn = 0;
	typeCnt = 0;

	// 手入力サイン
	F501_6c = F501_14c = F501_15c = F501_16c = F501_17c = F501_18c = F501_19c = 0;
	F501_7c = F501_8c = F501_9c = F501_10c = F501_11c = F501_12c = 0;

	m_curwnd = -1;
	m_TokureiMode = TOKUREIMODE_RENDO;
}

CShinFhyo5_01::~CShinFhyo5_01()
{
}

void CShinFhyo5_01::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo5_01Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo5_01Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo5_01, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CShinFhyo5_01 メッセージ ハンドラ

BOOL CShinFhyo5_01::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2);
	m_BottomIdx = 263;//コントロール一番最下のDiagのID指定。これで縦スクロール出る



	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo5_01::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo5_01::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo5_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
int CShinFhyo5_01::Init()
{
	// データ読込み
	ReadData();

	// データを集計モジュールにセット
	ReadData2();
	MemoryReget();

	// ダイアグラム初期属性セット
	InitDiagAttr();

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		m_Fhyo5_01Diag01.ModifyItem ( 11, DIAG_MDFY_TRANSPARENT );
	}

	// 令和2年4月1日改正様式を使用するのチェックがONの場合のみ
	// 「経過措置対象・・・」の文言を表示
	if ( !( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) ) {
		m_Fhyo5_01Diag01.ModifyItem ( 2, DIAG_MDFY_TRANSPARENT );
	}

	// 画面表示
	DispFh501();

	// 初期ポジション
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
int CShinFhyo5_01::InitInfo(CH31SnFhyo5_01Data **pSnFhyo5_01Data, CSnHeadData **pSnHeadData, CArithEx *pArith, CSyzSyukei **pSyzSyukei)
{
	ASSERT ( pSnFhyo5_01Data );
	if ( pSnFhyo5_01Data == NULL ) {
		return -1;
	}

	ASSERT ( pSnHeadData );
	if ( pSnHeadData == NULL )
		return -1;

	ASSERT ( pArith );
	if (pArith == NULL) {
		return -1;
	}

	ASSERT ( pSyzSyukei );
	if ( pSyzSyukei == NULL ) {
		return -1;
	}

	m_pSnFhyo5_01Data = pSnFhyo5_01Data;
	m_pSnHeadData = pSnHeadData;
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
int CShinFhyo5_01::InitDiagAttr()
{
	int cnt = 0;

	while ( 1 ) {
		if ( Fhyo5_01[cnt].Index == 0 )
			break;

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo5_01[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				//----> 20230616
				if( (*m_pSnHeadData)->m_DispTabSgn & 0x2000 && !CheckInputIndex2wari( Fhyo5_01[cnt].Index ) ){
					Flg = 0;
				}
				//<---- 20230616
				else{
					Flg = Fhyo5_01[cnt].UnConnect;
				}
			}
			else if ( m_TokureiMode == TOKUREIMODE_HANRENDO ) {
				Flg = Fhyo5_01[cnt].UnConnect;
			}
			else {
				Flg = Fhyo5_01[cnt].Connect;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL2, Fhyo5_01[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo5_01[cnt].Connect );
		cnt++;
	}

	m_Fhyo5_01Diag02.EnableDelete ( 0 );

	return 0;
}

//======================================
// 初期ポジションセット
//======================================
void CShinFhyo5_01::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		//index = m_Fhyo5_01Diag02.GetPosition();
		if ( ( index = m_Fhyo5_01Diag02.GetPosition() ) != -1 ) {
			m_Fhyo5_01Diag02.SetFocus();	// ダミーでセット
			m_Fhyo5_01Diag02.SetPosition ( index );
			m_Fhyo5_01Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo5_01Diag01.SetFocus();	// ダミーでセット
	m_Fhyo5_01Diag02.SetFocus();

	//----> 20230616
	if( (*m_pSnHeadData)->m_DispTabSgn & 0x2000 ){
		m_Fhyo5_01Diag02.SetPosition ( ID501_8D );
	}
	//<---- 20230616
	else{
		//　事業区分別の課税売上高D欄へ
		m_Fhyo5_01Diag02.SetPosition ( ID501_6D );
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
void CShinFhyo5_01::ChangeColor(unsigned short id, short index, int sign)
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
int CShinFhyo5_01::UpDownFocusMoveCheck(int Index, int Move)
{
	int idx = 0;
	int i = 0, j = 0;

	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY5_01CNT; i++ ) {
		if ( Index == FCS_Fhyo5_01[i].IDNo )
			break;
	}

	// 現在ポジションセット
	idx = FCS_Fhyo5_01[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) {			//　下方向
		for ( j = i + 1; j < FCS_FHY5_01CNT; j++ ) {
			m_Fhyo5_01Diag02.GetAttribute ( FCS_Fhyo5_01[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo5_01[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {		//　上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo5_01Diag02.GetAttribute ( FCS_Fhyo5_01[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo5_01[j].IDNo;
				break;
			}
		}
	}

	return idx;
}

//======================================
// データの読込み
//--------------------------------------
// 返送値
//		0	:	正常終了
//======================================
int CShinFhyo5_01::ReadData()
{
	// 課税標準額に対する消費税額
	memmove ( F501_1X, ( *m_pSnFhyo5_01Data )->Sn_F501_1X, MONY_BUF_SIZE );
	memmove ( F501_1D, ( *m_pSnFhyo5_01Data )->Sn_F501_1D, MONY_BUF_SIZE );
	memmove ( F501_1E, ( *m_pSnFhyo5_01Data )->Sn_F501_1E, MONY_BUF_SIZE );
	memmove ( F501_1F, ( *m_pSnFhyo5_01Data )->Sn_F501_1F, MONY_BUF_SIZE );

	// 貸倒回収に係る消費税額
	memmove ( F501_2X, ( *m_pSnFhyo5_01Data )->Sn_F501_2X, MONY_BUF_SIZE );
	memmove ( F501_2D, ( *m_pSnFhyo5_01Data )->Sn_F501_2D, MONY_BUF_SIZE );
	memmove ( F501_2E, ( *m_pSnFhyo5_01Data )->Sn_F501_2E, MONY_BUF_SIZE );
	memmove ( F501_2F, ( *m_pSnFhyo5_01Data )->Sn_F501_2F, MONY_BUF_SIZE );

	// 売上対価の返還等に係る消費税額
	memmove ( F501_3X, ( *m_pSnFhyo5_01Data )->Sn_F501_3X, MONY_BUF_SIZE );
	memmove ( F501_3D, ( *m_pSnFhyo5_01Data )->Sn_F501_3D, MONY_BUF_SIZE );
	memmove ( F501_3E, ( *m_pSnFhyo5_01Data )->Sn_F501_3E, MONY_BUF_SIZE );
	memmove ( F501_3F, ( *m_pSnFhyo5_01Data )->Sn_F501_3F, MONY_BUF_SIZE );

	// 控除対象仕入税額の計算の基礎となる消費税額
	memmove ( F501_4X, ( *m_pSnFhyo5_01Data )->Sn_F501_4X, MONY_BUF_SIZE );
	memmove ( F501_4D, ( *m_pSnFhyo5_01Data )->Sn_F501_4D, MONY_BUF_SIZE );
	memmove ( F501_4E, ( *m_pSnFhyo5_01Data )->Sn_F501_4E, MONY_BUF_SIZE );
	memmove ( F501_4F, ( *m_pSnFhyo5_01Data )->Sn_F501_4F, MONY_BUF_SIZE );

	// 1種類の事業の専業者の場合の控除対象仕入税額
	memmove ( F501_5X, ( *m_pSnFhyo5_01Data )->Sn_F501_5X, MONY_BUF_SIZE );
	memmove ( F501_5D, ( *m_pSnFhyo5_01Data )->Sn_F501_5D, MONY_BUF_SIZE );
	memmove ( F501_5E, ( *m_pSnFhyo5_01Data )->Sn_F501_5E, MONY_BUF_SIZE );
	memmove ( F501_5F, ( *m_pSnFhyo5_01Data )->Sn_F501_5F, MONY_BUF_SIZE );

	// 事業区分別の課税売上高(税抜き)の明細
	// 事業区分別の合計額
	memmove ( F501_6X, ( *m_pSnFhyo5_01Data )->Sn_F501_6X, MONY_BUF_SIZE );
	memmove ( F501_6D, ( *m_pSnFhyo5_01Data )->Sn_F501_6D, MONY_BUF_SIZE );
	memmove ( F501_6E, ( *m_pSnFhyo5_01Data )->Sn_F501_6E, MONY_BUF_SIZE );
	memmove ( F501_6F, ( *m_pSnFhyo5_01Data )->Sn_F501_6F, MONY_BUF_SIZE );
	F501_6c = (*m_pSnFhyo5_01Data)->Sn_F501_6sw;	// 手入力サイン

	// 第一種事業(卸売業)
	memmove ( F501_7X, ( *m_pSnFhyo5_01Data )->Sn_F501_7X, MONY_BUF_SIZE );
	memmove ( F501_7D, ( *m_pSnFhyo5_01Data )->Sn_F501_7D, MONY_BUF_SIZE );
	memmove ( F501_7E, ( *m_pSnFhyo5_01Data )->Sn_F501_7E, MONY_BUF_SIZE );
	memmove ( F501_7F, ( *m_pSnFhyo5_01Data )->Sn_F501_7F, MONY_BUF_SIZE );

	// 第二種事業(小売業)
	memmove ( F501_8X, ( *m_pSnFhyo5_01Data )->Sn_F501_8X, MONY_BUF_SIZE );
	memmove ( F501_8D, ( *m_pSnFhyo5_01Data )->Sn_F501_8D, MONY_BUF_SIZE );
	memmove ( F501_8E, ( *m_pSnFhyo5_01Data )->Sn_F501_8E, MONY_BUF_SIZE );
	memmove ( F501_8F, ( *m_pSnFhyo5_01Data )->Sn_F501_8F, MONY_BUF_SIZE );

	// 第三種事業(製造業等)
	memmove ( F501_9X, ( *m_pSnFhyo5_01Data )->Sn_F501_9X, MONY_BUF_SIZE );
	memmove ( F501_9D, ( *m_pSnFhyo5_01Data )->Sn_F501_9D, MONY_BUF_SIZE );
	memmove ( F501_9E, ( *m_pSnFhyo5_01Data )->Sn_F501_9E, MONY_BUF_SIZE );
	memmove ( F501_9F, ( *m_pSnFhyo5_01Data )->Sn_F501_9F, MONY_BUF_SIZE );

	// 第四種事業(その他)
	memmove ( F501_10X, ( *m_pSnFhyo5_01Data )->Sn_F501_10X, MONY_BUF_SIZE );
	memmove ( F501_10D, ( *m_pSnFhyo5_01Data )->Sn_F501_10D, MONY_BUF_SIZE );
	memmove ( F501_10E, ( *m_pSnFhyo5_01Data )->Sn_F501_10E, MONY_BUF_SIZE );
	memmove ( F501_10F, ( *m_pSnFhyo5_01Data )->Sn_F501_10F, MONY_BUF_SIZE );

	// 第五種事業(サービス業)
	memmove ( F501_11X, ( *m_pSnFhyo5_01Data )->Sn_F501_11X, MONY_BUF_SIZE );
	memmove ( F501_11D, ( *m_pSnFhyo5_01Data )->Sn_F501_11D, MONY_BUF_SIZE );
	memmove ( F501_11E, ( *m_pSnFhyo5_01Data )->Sn_F501_11E, MONY_BUF_SIZE );
	memmove ( F501_11F, ( *m_pSnFhyo5_01Data )->Sn_F501_11F, MONY_BUF_SIZE );

	// 第六種事業(不動産業)
	memmove ( F501_12X, ( *m_pSnFhyo5_01Data )->Sn_F501_12X, MONY_BUF_SIZE );
	memmove ( F501_12D, ( *m_pSnFhyo5_01Data )->Sn_F501_12D, MONY_BUF_SIZE );
	memmove ( F501_12E, ( *m_pSnFhyo5_01Data )->Sn_F501_12E, MONY_BUF_SIZE );
	memmove ( F501_12F, ( *m_pSnFhyo5_01Data )->Sn_F501_12F, MONY_BUF_SIZE );

	// 事業区分別の課税売上高に係る消費税額の明細
	// 事業区分別の合計額
	memmove ( F501_13X, ( *m_pSnFhyo5_01Data )->Sn_F501_13X, MONY_BUF_SIZE );
	memmove ( F501_13D, ( *m_pSnFhyo5_01Data )->Sn_F501_13D, MONY_BUF_SIZE );
	memmove ( F501_13E, ( *m_pSnFhyo5_01Data )->Sn_F501_13E, MONY_BUF_SIZE );
	memmove ( F501_13F, ( *m_pSnFhyo5_01Data )->Sn_F501_13F, MONY_BUF_SIZE );

	// 第一種事業(卸売業)
	memmove ( F501_14X, ( *m_pSnFhyo5_01Data )->Sn_F501_14X, MONY_BUF_SIZE );
	memmove ( F501_14D, ( *m_pSnFhyo5_01Data )->Sn_F501_14D, MONY_BUF_SIZE );
	memmove ( F501_14E, ( *m_pSnFhyo5_01Data )->Sn_F501_14E, MONY_BUF_SIZE );
	memmove ( F501_14F, ( *m_pSnFhyo5_01Data )->Sn_F501_14F, MONY_BUF_SIZE );
	F501_14c = (*m_pSnFhyo5_01Data)->Sn_F501_14sw;	// 手入力サイン

	// 第二種事業(小売業)
	memmove ( F501_15X, ( *m_pSnFhyo5_01Data )->Sn_F501_15X, MONY_BUF_SIZE );
	memmove ( F501_15D, ( *m_pSnFhyo5_01Data )->Sn_F501_15D, MONY_BUF_SIZE );
	memmove ( F501_15E, ( *m_pSnFhyo5_01Data )->Sn_F501_15E, MONY_BUF_SIZE );
	memmove ( F501_15F, ( *m_pSnFhyo5_01Data )->Sn_F501_15F, MONY_BUF_SIZE );
	F501_15c = (*m_pSnFhyo5_01Data)->Sn_F501_15sw;	// 手入力サイン

	// 第三種事業(製造業等)
	memmove ( F501_16X, ( *m_pSnFhyo5_01Data )->Sn_F501_16X, MONY_BUF_SIZE );
	memmove ( F501_16D, ( *m_pSnFhyo5_01Data )->Sn_F501_16D, MONY_BUF_SIZE );
	memmove ( F501_16E, ( *m_pSnFhyo5_01Data )->Sn_F501_16E, MONY_BUF_SIZE );
	memmove ( F501_16F, ( *m_pSnFhyo5_01Data )->Sn_F501_16F, MONY_BUF_SIZE );
	F501_16c = (*m_pSnFhyo5_01Data)->Sn_F501_16sw;	// 手入力サイン

	// 第四種事業(その他)
	memmove ( F501_17X, ( *m_pSnFhyo5_01Data )->Sn_F501_17X, MONY_BUF_SIZE );
	memmove ( F501_17D, ( *m_pSnFhyo5_01Data )->Sn_F501_17D, MONY_BUF_SIZE );
	memmove ( F501_17E, ( *m_pSnFhyo5_01Data )->Sn_F501_17E, MONY_BUF_SIZE );
	memmove ( F501_17F, ( *m_pSnFhyo5_01Data )->Sn_F501_17F, MONY_BUF_SIZE );
	F501_17c = (*m_pSnFhyo5_01Data)->Sn_F501_17sw;	// 手入力サイン

	// 第五種事業(サービス業)
	memmove ( F501_18X, ( *m_pSnFhyo5_01Data )->Sn_F501_18X, MONY_BUF_SIZE );
	memmove ( F501_18D, ( *m_pSnFhyo5_01Data )->Sn_F501_18D, MONY_BUF_SIZE );
	memmove ( F501_18E, ( *m_pSnFhyo5_01Data )->Sn_F501_18E, MONY_BUF_SIZE );
	memmove ( F501_18F, ( *m_pSnFhyo5_01Data )->Sn_F501_18F, MONY_BUF_SIZE );
	F501_18c = (*m_pSnFhyo5_01Data)->Sn_F501_18sw;	// 手入力サイン

	// 第六種事業(不動産業)
	memmove ( F501_19X, ( *m_pSnFhyo5_01Data )->Sn_F501_19X, MONY_BUF_SIZE );
	memmove ( F501_19D, ( *m_pSnFhyo5_01Data )->Sn_F501_19D, MONY_BUF_SIZE );
	memmove ( F501_19E, ( *m_pSnFhyo5_01Data )->Sn_F501_19E, MONY_BUF_SIZE );
	memmove ( F501_19F, ( *m_pSnFhyo5_01Data )->Sn_F501_19F, MONY_BUF_SIZE );
	F501_19c = (*m_pSnFhyo5_01Data)->Sn_F501_19sw;	// 手入力サイン

	// 売上割合
	F501_7UP = (*m_pSnFhyo5_01Data)->Sn_F501_UP1;
	F501_8UP = (*m_pSnFhyo5_01Data)->Sn_F501_UP2;
	F501_9UP = (*m_pSnFhyo5_01Data)->Sn_F501_UP3;
	F501_10UP = (*m_pSnFhyo5_01Data)->Sn_F501_UP4;
	F501_11UP = (*m_pSnFhyo5_01Data)->Sn_F501_UP5;
	F501_12UP = (*m_pSnFhyo5_01Data)->Sn_F501_UP6;

	return 0;
}

//===================================
// データを集計モジュールにセット
//===================================
void CShinFhyo5_01::ReadData2()
{
	//----> 20230616
	if( (*m_pSnHeadData)->m_DispTabSgn & 0x2000 ){

		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {	// 非連動、入力ロックの場合

			// 事業区分別の合計と第二種のみ手入力を残し、他は全てリセットする
			F501_7c  = F501_9c  = F501_10c = F501_11c = F501_12c = 0;
			F501_14c = F501_16c = F501_17c = F501_18c = F501_19c = 0;

			// 事業区分別の合計額
			if( F501_6c&0x80 ){
				( *m_pSyzSyukei )->SetShinkokuData( _T( "357602" ), F501_6D );
			}
			else{
				( *m_pSyzSyukei )->ResetShinkokuData( _T( "357602" ) );
			}
			if( F501_6c&0x800 ){
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "357603" ), F501_6E );
			}
			else{
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "357603" ));
			}

			// 第二種売上高
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357802" ), F501_8D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357803" ), F501_8E );

			// 第二種以外の売上高はリセット
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "357702" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "357703" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "357902" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "357903" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358002" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358003" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358102" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358103" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358202" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358203" ) );

			// 第二種消費税
			if( F501_15c&0x80 ){
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358502" ), F501_15D );
			}
			else{
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358502" ));
			}
			if( F501_15c&0x800 ){
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358503" ), F501_15E );
			}
			else{
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358503" ));
			}

			// 第二種以外の消費税はリセット
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358402" ));
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358403" ));
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358602" ));
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358603" ));
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358702" ));
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358703" ));
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358802" ));
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358803" ));
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358902" ));
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358903" ));
		}
	}
	//<---- 20230616
	else{

		m_TokureiMode = TOKUREIMODE_RENDO;
		//if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {	// 非連動の場合
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {	// 非連動、入力ロックの場合
			m_TokureiMode = TOKUREIMODE_HIRENDO;
			if( F501_6c&0x80 )
				(*m_pSyzSyukei)->SetShinkokuData(_T("357602"), F501_6D);
			else
				(*m_pSyzSyukei)->ResetShinkokuData(_T("357602"));
			if( F501_6c&0x800 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "357603" ), F501_6E );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "357603" ));
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357702" ), F501_7D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357703" ), F501_7E );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357802" ), F501_8D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357803" ), F501_8E );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357902" ), F501_9D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357903" ), F501_9E );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358002" ), F501_10D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358003" ), F501_10E );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358102" ), F501_11D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358103" ), F501_11E );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358202" ), F501_12D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358203" ), F501_12E );
			if( F501_14c&0x80 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358402" ), F501_14D );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358402" ));
			if( F501_14c&0x800 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358403" ), F501_14E );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358403" ));
			if( F501_15c&0x80 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358502" ), F501_15D );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358502" ));
			if( F501_15c&0x800 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358503" ), F501_15E );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358503" ));
			if( F501_16c&0x80 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358602" ), F501_16D );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358602" ));
			if( F501_16c&0x800 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358603" ), F501_16E );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358603" ));
			if( F501_17c&0x80 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358702" ), F501_17D );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358702" ));
			if( F501_17c&0x800 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358703" ), F501_17E );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358703" ));
			if( F501_18c&0x80 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358802" ), F501_18D );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358802" ));
			if( F501_18c&0x800 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358803" ), F501_18E );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358803" ));
			if( F501_19c&0x80 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358902" ), F501_19D );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358902" ));
			if( F501_19c&0x800 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358903" ), F501_19E );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358903" ));

			if ((*m_pSnHeadData)->SVmzsw == 1) {
				(*m_pSyzSyukei)->ResetShinkokuData(_T("357101"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("357201"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("357301"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("357401"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("357501"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("357601"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("357701"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("357801"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("357901"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358001"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358101"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358201"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358301"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358401"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358501"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358601"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358701"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358801"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358901"));
			}
			else {
				// 経過措置なし　旧税率合計を０クリア
				char mony[MONY_BUF_SIZE] = { 0 };
				memset(mony, '\0', MONY_BUF_SIZE);

				(*m_pSyzSyukei)->SetShinkokuData(_T("357101"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("357201"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("357301"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("357401"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("357501"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("357601"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("357701"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("357801"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("357901"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("358001"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("358101"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("358201"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("358301"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("358401"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("358501"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("358601"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("358701"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("358801"), mony);
				(*m_pSyzSyukei)->SetShinkokuData(_T("358901"), mony);
			}
		}
		else if ( ((*m_pSnHeadData)->m_DispTabSgn & 0x10) && ((*m_pSnHeadData)->m_DispTabSgn & 0x80) && ((*m_pSnHeadData)->m_DispTabSgn & 0x100) ){
			// 特例５－１　半連動
			m_TokureiMode = TOKUREIMODE_HANRENDO;
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357702" ), F501_7D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357703" ), F501_7E );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357802" ), F501_8D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357803" ), F501_8E );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357902" ), F501_9D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "357903" ), F501_9E );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358002" ), F501_10D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358003" ), F501_10E );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358102" ), F501_11D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358103" ), F501_11E );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358202" ), F501_12D );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "358203" ), F501_12E );
			if( F501_14c&0x80 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358402" ), F501_14D );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358402" ));
			if( F501_14c&0x800 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358403" ), F501_14E );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358403" ));
			if( F501_15c&0x80 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358502" ), F501_15D );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358502" ));
			if( F501_15c&0x800 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358503" ), F501_15E );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358503" ));
			if( F501_16c&0x80 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358602" ), F501_16D );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358602" ));
			if( F501_16c&0x800 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358603" ), F501_16E );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358603" ));
			if( F501_17c&0x80 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358702" ), F501_17D );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358702" ));
			if( F501_17c&0x800 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358703" ), F501_17E );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358703" ));
			if( F501_18c&0x80 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358802" ), F501_18D );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358802" ));
			if( F501_18c&0x800 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358803" ), F501_18E );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358803" ));
			if( F501_19c&0x80 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358902" ), F501_19D );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358902" ));
			if( F501_19c&0x800 )
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "358903" ), F501_19E );
			else
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "358903" ));
		}
	}
}

//===================================
// データの書込み
//-----------------------------------
// 返送値
//		0	:	正常終了
//===================================
int CShinFhyo5_01::WriteData()
{
	// 課税標準額に対する消費税額
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_1X, F501_1X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_1D, F501_1D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_1E, F501_1E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_1F, F501_1F, MONY_BUF_SIZE );

	// 貸倒回収に係る消費税額
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_2X, F501_2X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_2D, F501_2D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_2E, F501_2E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_2F, F501_2F, MONY_BUF_SIZE );

	// 売上対価の返還等対価に係る消費税額
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_3X, F501_3X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_3D, F501_3D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_3E, F501_3E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_3F, F501_3F, MONY_BUF_SIZE );

	// 控除対象仕入税額の計算の基礎となる消費税額
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_4X, F501_4X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_4D, F501_4D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_4E, F501_4E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_4F, F501_4F, MONY_BUF_SIZE );

	// 1種類の事業の専業者の場合の控除対象仕入税額
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_5X, F501_5X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_5D, F501_5D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_5E, F501_5E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_5F, F501_5F, MONY_BUF_SIZE );

	// 事業区分別の課税売上高(税抜き)の明細
	// 事業区分別の合計額
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_6X, F501_6X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_6D, F501_6D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_6E, F501_6E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_6F, F501_6F, MONY_BUF_SIZE );
	(*m_pSnFhyo5_01Data)->Sn_F501_6sw = F501_6c;

	// 第一種事業(卸売業)
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_7X, F501_7X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_7D, F501_7D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_7E, F501_7E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_7F, F501_7F, MONY_BUF_SIZE );

	// 第二種事業(小売業)
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_8X, F501_8X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_8D, F501_8D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_8E, F501_8E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_8F, F501_8F, MONY_BUF_SIZE );

	// 第三種事業(製造業等)
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_9X, F501_9X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_9D, F501_9D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_9E, F501_9E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_9F, F501_9F, MONY_BUF_SIZE );

	// 第四種事業(その他)
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_10X, F501_10X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_10D, F501_10D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_10E, F501_10E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_10F, F501_10F, MONY_BUF_SIZE );

	// 第五種事業(サービス業)
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_11X, F501_11X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_11D, F501_11D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_11E, F501_11E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_11F, F501_11F, MONY_BUF_SIZE );

	// 第六種事業(不動産業)
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_12X, F501_12X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_12D, F501_12D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_12E, F501_12E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_12F, F501_12F, MONY_BUF_SIZE );

	// 事業区分別の課税売上高に係る消費税額の明細
	// 事業区分別の合計額
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_13X, F501_13X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_13D, F501_13D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_13E, F501_13E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_13F, F501_13F, MONY_BUF_SIZE );

	// 第一種事業(卸売業)
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_14X, F501_14X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_14D, F501_14D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_14E, F501_14E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_14F, F501_14F, MONY_BUF_SIZE );
	(*m_pSnFhyo5_01Data)->Sn_F501_14sw = F501_14c;
	if(m_pArith->l_test(F501_14D) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_14DP, F501_14D, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_14DP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F501_14E) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_14EP, F501_14E, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_14EP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F501_14X) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_14XP, F501_14X, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_14XP, '\0', MONY_BUF_SIZE );

	// 第二種事業(小売業)
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_15X, F501_15X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_15D, F501_15D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_15E, F501_15E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_15F, F501_15F, MONY_BUF_SIZE );
	(*m_pSnFhyo5_01Data)->Sn_F501_15sw = F501_15c;
	if(m_pArith->l_test(F501_15D) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_15DP, F501_15D, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_15DP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F501_15E) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_15EP, F501_15E, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_15EP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F501_15X) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_15XP, F501_15X, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_15XP, '\0', MONY_BUF_SIZE );

	// 第三種事業(製造業等)
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_16X, F501_16X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_16D, F501_16D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_16E, F501_16E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_16F, F501_16F, MONY_BUF_SIZE );
	(*m_pSnFhyo5_01Data)->Sn_F501_16sw = F501_16c;
	if(m_pArith->l_test(F501_16D) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_16DP, F501_16D, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_16DP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F501_16E) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_16EP, F501_16E, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_16EP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F501_16X) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_16XP, F501_16X, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_16XP, '\0', MONY_BUF_SIZE );

	// 第四種事業(その他)
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_17X, F501_17X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_17D, F501_17D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_17E, F501_17E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_17F, F501_17F, MONY_BUF_SIZE );
	(*m_pSnFhyo5_01Data)->Sn_F501_17sw = F501_17c;
	if(m_pArith->l_test(F501_17D) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_17DP, F501_17D, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_17DP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F501_17E) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_17EP, F501_17E, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_17EP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F501_17X) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_17XP, F501_17X, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_17XP, '\0', MONY_BUF_SIZE );

	// 第五種事業(サービス業)
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_18X, F501_18X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_18D, F501_18D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_18E, F501_18E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_18F, F501_18F, MONY_BUF_SIZE );
	(*m_pSnFhyo5_01Data)->Sn_F501_18sw = F501_18c;
	if(m_pArith->l_test(F501_18D) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_18DP, F501_18D, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_18DP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F501_18E) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_18EP, F501_18E, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_18EP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F501_18X) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_18XP, F501_18X, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_18XP, '\0', MONY_BUF_SIZE );

	// 第六種事業(不動産業)
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_19X, F501_19X, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_19D, F501_19D, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_19E, F501_19E, MONY_BUF_SIZE );
	memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_19F, F501_19F, MONY_BUF_SIZE );
	(*m_pSnFhyo5_01Data)->Sn_F501_19sw = F501_19c;
	if(m_pArith->l_test(F501_19D) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_19DP, F501_19D, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_19DP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F501_19E) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_19EP, F501_19E, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_19EP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F501_19X) >= 0)
		memmove ( ( *m_pSnFhyo5_01Data )->Sn_F501_19XP, F501_19X, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_01Data )->Sn_F501_19XP, '\0', MONY_BUF_SIZE );

	// 売上割合
	(*m_pSnFhyo5_01Data)->Sn_F501_UP1 = F501_7UP;
	(*m_pSnFhyo5_01Data)->Sn_F501_UP2 = F501_8UP;
	(*m_pSnFhyo5_01Data)->Sn_F501_UP3 = F501_9UP;
	(*m_pSnFhyo5_01Data)->Sn_F501_UP4 = F501_10UP;
	(*m_pSnFhyo5_01Data)->Sn_F501_UP5 = F501_11UP;
	(*m_pSnFhyo5_01Data)->Sn_F501_UP6 = F501_12UP;

	(*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 0;
	if (typeCnt == 1) {


		//---->yoshida191223
		if (m_pArith->l_test(F501_7D)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 1;
		else if (m_pArith->l_test(F501_7E)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 1;

		else if (m_pArith->l_test(F501_8D)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 2;
		else if (m_pArith->l_test(F501_8E)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 2;

		else if (m_pArith->l_test(F501_9D)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 3;
		else if (m_pArith->l_test(F501_9E)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 3;

		else if (m_pArith->l_test(F501_10D)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 4;
		else if (m_pArith->l_test(F501_10E)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 4;

		else if (m_pArith->l_test(F501_11D)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 5;
		else if (m_pArith->l_test(F501_11E)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 5;

		else if (m_pArith->l_test(F501_12D)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 6;
		else if (m_pArith->l_test(F501_12E)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 6;

		MoneyBasejagArray money;
		//---一種旧税率チェック
		money = (*m_pSyzSyukei)->GetShinkokuData(_T("350701"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 1;

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("350702"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 1;

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("350703"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 1;

		//---二種旧税率チェック
		money = (*m_pSyzSyukei)->GetShinkokuData(_T("350801"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 2;

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("350802"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 2;

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("350803"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 2;

		//---三種旧税率チェック
		money = (*m_pSyzSyukei)->GetShinkokuData(_T("350901"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 3;

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("350902"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 3;

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("350903"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 3;

		//---四種旧税率チェック
		money = (*m_pSyzSyukei)->GetShinkokuData(_T("351001"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 4;

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("351002"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 4;

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("351003"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 4;

		//---五種旧税率チェック
		money = (*m_pSyzSyukei)->GetShinkokuData(_T("351101"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 5;

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("351102"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 5;

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("351103"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 5;

		//---六種旧税率チェック
		money = (*m_pSyzSyukei)->GetShinkokuData(_T("351201"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 6;

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("351202"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 6;

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("351203"));
		if (m_pArith->l_test(money[0][0].arith)) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 6;


		//------------------
//		if (F501_7UP) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 1;
//		else if (F501_8UP) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 2;
//		else if (F501_9UP) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 3;
//		else if (F501_10UP) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 4;
//		else if (F501_11UP) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 5;
//		else if (F501_12UP) (*m_pSnFhyo5_01Data)->Sn_F501_5kbn = 6;
		//<-----------------
	}


	return 0;
}

//=========================================
// 画面出力
//=========================================
void CShinFhyo5_01::DispFh501()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	char buf[512] = { 0 };

	// 画面の初期化
	//for (int i = 6; i <= 8; i++ ) {
	for (int i = 8; i <= 10; i++ ) {					// 課税期間、氏名欄
		dsp_cls(IDC_ICSDIAG8CTRL1, (short)i);
	}
	for (int i = ID501_1X; i <= ID501_19F; i++ ) {	// 金額欄
		//dsp_cls(IDC_ICSDIAG8CTRL1, (short)i);
		dsp_cls(IDC_ICSDIAG8CTRL2, (short)i);
	}

	// 手入力サイン初期化
	if( !((*m_pSnHeadData)->Sn_Sign4 & 0x80) ){	// 連動の場合は手入力サイン初期化
		F501_6c = F501_14c = F501_15c = F501_16c = F501_17c = F501_18c = F501_19c = 0;
		F501_7c = F501_8c = F501_9c = F501_10c = F501_11c = F501_12c = 0;
	}

	// 期間・氏名の表示
	char yy, mm, dd;
	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s ( buf, sizeof ( buf ), _T( "%02x.%02x.%02x" ), ( yy & 0xff ), ( mm & 0xff ), ( dd & 0xff ) );
	DiagData.data_disp = ( char* )buf;
	//diag_setdata ( IDC_ICSDIAG8CTRL1, 6, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	diag_setdata ( IDC_ICSDIAG8CTRL1, 8, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	yy = mm = dd = 0;
	( *m_pSnHeadData )->GetYmdDataGen ( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	sprintf_s ( buf, sizeof ( buf ), _T( "%02x.%02x.%02x" ), ( yy & 0xff ), ( mm & 0xff ), ( dd & 0xff ) );
	DiagData.data_disp = ( char* )buf;
	//diag_setdata ( IDC_ICSDIAG8CTRL1, 7, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	diag_setdata ( IDC_ICSDIAG8CTRL1, 9, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	memset ( buf, '\0', sizeof ( buf ) );
	if ( ( *m_pSnHeadData )->IsSoleProprietor() ) {
		memmove ( buf, ( *m_pSnHeadData )->Sn_DIHYO, 40 );
	}
	else {
		memmove ( buf, ( *m_pSnHeadData )->Sn_CONAM, 40 );
	}
	DiagData.data_disp = ( char* )buf;
	//diag_setdata ( IDC_ICSDIAG8CTRL1, 8, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	diag_setdata ( IDC_ICSDIAG8CTRL1, 10, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 連動時には「●単一事業を営む～」を非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		diag_clear ( IDC_ICSDIAG8CTRL1, 4, TRUE, CIcsdiagctrl );
		diag_getattr ( IDC_ICSDIAG8CTRL1, 4, &DA, CIcsdiagctrl );
		DA.attr_bcolor = SIROIROCOLOR;
		diag_setattr ( IDC_ICSDIAG8CTRL1, 4, &DA, FALSE, CIcsdiagctrl );
	}

	// データ表示
	Disp_f501data ( 0 );

	// 売上割合表示
	f501_uws();
}

//=================================================
// 金額表示
//-------------------------------------------------
// 引数
//		Id		:	ダイアグラムID
//		Index	:	ダイアグラム内インデックス
//		Mony	:	金額
//=================================================
void CShinFhyo5_01::dsp_prs ( unsigned short Id, short Index, char* Mony ) 
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	memmove ( DiagData.data_val, Mony, MONY_BUF_SIZE );
	diag_setdata ( Id, Index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
}

//=================================================
// 金額表示クリア
//-------------------------------------------------
// 引数
//		Id		:	ダイアグラムID
//		Index	:	ダイアグラム内インデックス
//=================================================
void CShinFhyo5_01::dsp_cls ( unsigned short Id, short Index )
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	switch ( Index ) {
		case 6:		// 課税期間　自
		case 7:		// 課税期間　至
		case 8:		// 氏名又は、名称
		case ID501_7W:		//  7. 売上割合
		case ID501_8W:		//  8. 売上割合
		case ID501_9W:		//  9. 売上割合
		case ID501_10W:		// 10. 売上割合
		case ID501_11W:		// 11. 売上割合	
		case ID501_12W:		// 12. 売上割合	
			diag_clear ( Id, Index, TRUE, CIcsdiagctrl );
			break;
		default:
			memset ( DiagData.data_val, '\0', MONY_BUF_SIZE );
			diag_setdata ( Id, Index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			break;
	}
}

//=================================================
// 売上割合表示
//-------------------------------------------------
// 引数
//		Id		:	ダイアグラムID
//		Index	:	ダイアグラム内インデックス
//		pers	:	金額
//=================================================
void CShinFhyo5_01::dsp_uws ( unsigned short Id, short Index, unsigned short pers )
{
	if ( pers ) {
		char buf[128] = { 0 };
		DIAGRAM_DATA DiagData;
		DiagData.data_imgdata = NULL;

		//sprintf_s ( buf, sizeof ( buf ), _T( "%3d.%01d" ), ( pers / 100 ), ( pers % 10 ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%3d.%01d" ), ( pers / 10 ), ( pers % 10 ) );	// 小数１桁
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( Id, Index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}
	else {
		dsp_cls ( Id, Index );
	}
}


//=================================
// データ表示
//---------------------------------
// 引数
//		idx		:	0 = 全データ
//=================================
void CShinFhyo5_01::Disp_f501data ( int idx )
{
	switch ( idx ) {
	
		case 0:
		case 1:
			f501_1data();
		case 2:
			f501_2data();
		case 3:
			f501_3data();
		case 4:
			f501_4data();
		case 5:
			f501_5data();
		case 6:
			f501_6data();
		case 7:
			f501_7data();
		case 8:
			f501_8data();
		case 9:
			f501_9data();
		case 10:
			f501_10data();
		case 11:
			f501_11data();
		case 12:
			f501_12data();
		case 13:
			f501_13data();
		case 14:
			f501_14data();
		case 15:
			f501_15data();
		case 16:
			f501_16data();
		case 17:
			f501_17data();
		case 18:
			f501_18data();
		case 19:
			f501_19data();
			break;
		default:
			break;
	}
}

//=====================================
// 課税標準額に対する消費税額
//=====================================
void CShinFhyo5_01::f501_1data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_1X, F501_1X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_1D, F501_1D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_1E, F501_1E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_1F, F501_1F );
}

//=====================================
// 貸倒回収に係る消費税額
//=====================================
void CShinFhyo5_01::f501_2data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_2X, F501_2X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_2D, F501_2D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_2E, F501_2E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_2F, F501_2F );
}

//=====================================
// 売上対価の返還等対価に係る税額
//=====================================
void CShinFhyo5_01::f501_3data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_3X, F501_3X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_3D, F501_3D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_3E, F501_3E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_3F, F501_3F );
}

//===============================================
// 控除対象仕入税額の計算の基礎となる消費税額
//===============================================
void CShinFhyo5_01::f501_4data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_4X, F501_4X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_4D, F501_4D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_4E, F501_4E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_4F, F501_4F );
}

//===============================================
// 1種類の事業の専業者の場合の控除対象仕入税額
//===============================================
void CShinFhyo5_01::f501_5data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_5X, F501_5X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_5D, F501_5D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_5E, F501_5E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_5F, F501_5F );
}

//=========================================================
//  2種類以上の事業を営む事業者の場合の控除対象仕入税額
	// 事業区分別の合計額
//=========================================================
void CShinFhyo5_01::f501_6data()
{
	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
		ChangeColor(IDC_ICSDIAG8CTRL2, ID501_6D, (F501_6c & 0x80) ? 3 : 0);
		ChangeColor(IDC_ICSDIAG8CTRL2, ID501_6E, (F501_6c & 0x800) ? 3 : 0);
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_6X, F501_6X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_6D, F501_6D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_6E, F501_6E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_6F, F501_6F );
	f501_uws();
}

//=====================================
// 第一種事業(卸売業)
//=====================================
void CShinFhyo5_01::f501_7data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_7X, F501_7X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_7D, F501_7D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_7E, F501_7E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_7F, F501_7F );
	f501_uws();
}

//=====================================
// 第二種事業(小売業)
//=====================================
void CShinFhyo5_01::f501_8data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_8X, F501_8X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_8D, F501_8D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_8E, F501_8E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_8F, F501_8F );
	f501_uws();
}

//=====================================
// 第三種事業(製造業等)
//=====================================
void CShinFhyo5_01::f501_9data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_9X, F501_9X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_9D, F501_9D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_9E, F501_9E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_9F, F501_9F );
	f501_uws();
}

//=====================================
// 第四種事業(その他)
//=====================================
void CShinFhyo5_01::f501_10data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_10X, F501_10X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_10D, F501_10D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_10E, F501_10E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_10F, F501_10F );
	f501_uws();
}

//=====================================
// 第五種事業(サービス業)
//=====================================
void CShinFhyo5_01::f501_11data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_11X, F501_11X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_11D, F501_11D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_11E, F501_11E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_11F, F501_11F );
	f501_uws();
}

//=====================================
// 第六種事業(不動産業)
//=====================================
void CShinFhyo5_01::f501_12data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_12X, F501_12X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_12D, F501_12D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_12E, F501_12E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_12F, F501_12F );
	f501_uws();
}

//===============================================
// 事業区分別の課税売上高に係る消費税額の明細
// 事業区分別の合計額
//===============================================
void CShinFhyo5_01::f501_13data()
{
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_13X, F501_13X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_13D, F501_13D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_13E, F501_13E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_13F, F501_13F );
}

//=====================================
// 第一種事業(卸売業)
//=====================================
void CShinFhyo5_01::f501_14data()
{
//	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) && !((*m_pSnHeadData)->m_DispTabSgn & 0x2000)) {	// 20230616
		
		// 手入力サインなし
		if( !(F501_14c & 0x80) ){
			ChangeColor(IDC_ICSDIAG8CTRL2, ID501_14D, 0);
			if( (F501_7c & 0x10) || (m_pArith->l_test(F501_7D) && m_pArith->l_test(F501_14D) == 0)  ){
				memset( F501_14D, '\0', MONY_BUF_SIZE );
				// 売上割合の再計算
				F501_7c &= ~0xf0;
			}
		}
		if( !(F501_14c & 0x800) ){
			ChangeColor(IDC_ICSDIAG8CTRL2, ID501_14E, 0);
			if ((F501_7c & 0x100) || (m_pArith->l_test(F501_7E) && m_pArith->l_test(F501_14E) == 0)) {
				memset(F501_14E, '\0', MONY_BUF_SIZE);
				// 売上割合の再計算
				F501_7c &= ~0xf00;
			}
		}

		// 手入力サインあり
		if (F501_14c & 0x80) {
			ChangeColor(IDC_ICSDIAG8CTRL2, ID501_14D, 3);
		}
		if (F501_14c & 0x800) {
			ChangeColor(IDC_ICSDIAG8CTRL2, ID501_14E, 3);
		}
	}

	dsp_prs(IDC_ICSDIAG8CTRL2, ID501_14X, F501_14X);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID501_14D, F501_14D);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID501_14E, F501_14E);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID501_14F, F501_14F);
}

//=====================================
// 第二種事業(小売業)
//=====================================
void CShinFhyo5_01::f501_15data()
{
	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
		ChangeColor(IDC_ICSDIAG8CTRL2, ID501_15D, (F501_15c & 0x80) ? 3 : 0);
		ChangeColor(IDC_ICSDIAG8CTRL2, ID501_15E, (F501_15c & 0x800) ? 3 : 0);
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_15X, F501_15X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_15D, F501_15D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_15E, F501_15E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_15F, F501_15F );
}

//=====================================
// 第三種事業(製造業等)
//=====================================
void CShinFhyo5_01::f501_16data()
{
//	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) && !((*m_pSnHeadData)->m_DispTabSgn & 0x2000)) {	// 20230616
		ChangeColor(IDC_ICSDIAG8CTRL2, ID501_16D, (F501_16c & 0x80) ? 3 : 0);
		ChangeColor(IDC_ICSDIAG8CTRL2, ID501_16E, (F501_16c & 0x800) ? 3 : 0);
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_16X, F501_16X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_16D, F501_16D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_16E, F501_16E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_16F, F501_16F );
}



//=====================================
// 第四種事業(その他)
//=====================================
void CShinFhyo5_01::f501_17data()
{
//	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) && !((*m_pSnHeadData)->m_DispTabSgn & 0x2000)) {	// 20230616
		ChangeColor(IDC_ICSDIAG8CTRL2, ID501_17D, (F501_17c & 0x80) ? 3 : 0);
		ChangeColor(IDC_ICSDIAG8CTRL2, ID501_17E, (F501_17c & 0x800) ? 3 : 0);
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_17X, F501_17X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_17D, F501_17D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_17E, F501_17E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_17F, F501_17F );
}

//=====================================
// 第五種事業(サービス業)
//=====================================
void CShinFhyo5_01::f501_18data()
{
//	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) && !((*m_pSnHeadData)->m_DispTabSgn & 0x2000)) {	// 20230616
		ChangeColor(IDC_ICSDIAG8CTRL2, ID501_18D, (F501_18c & 0x80) ? 3 : 0);
		ChangeColor(IDC_ICSDIAG8CTRL2, ID501_18E, (F501_18c & 0x800) ? 3 : 0);
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_18X, F501_18X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_18D, F501_18D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_18E, F501_18E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_18F, F501_18F );
}

//=====================================
// 第六種事業(不動産業)
//=====================================
void CShinFhyo5_01::f501_19data()
{
//	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) && !((*m_pSnHeadData)->m_DispTabSgn & 0x2000)) {	// 20230616	
		ChangeColor(IDC_ICSDIAG8CTRL2, ID501_19D, (F501_19c & 0x80) ? 3 : 0);
		ChangeColor(IDC_ICSDIAG8CTRL2, ID501_19E, (F501_19c & 0x800) ? 3 : 0);
	}

	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_19X, F501_19X );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_19D, F501_19D );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_19E, F501_19E );
	dsp_prs ( IDC_ICSDIAG8CTRL2, ID501_19F, F501_19F );
}

//====================================
// 売上割合表示
//====================================
void CShinFhyo5_01::f501_uws()
{
	dsp_uws ( IDC_ICSDIAG8CTRL2, ID501_7W, F501_7UP );
	dsp_uws ( IDC_ICSDIAG8CTRL2, ID501_8W, F501_8UP );
	dsp_uws ( IDC_ICSDIAG8CTRL2, ID501_9W, F501_9UP );
	dsp_uws ( IDC_ICSDIAG8CTRL2, ID501_10W, F501_10UP );
	dsp_uws ( IDC_ICSDIAG8CTRL2, ID501_11W, F501_11UP );
	dsp_uws ( IDC_ICSDIAG8CTRL2, ID501_12W, F501_12UP );
}

BEGIN_EVENTSINK_MAP(CShinFhyo5_01, CSyzBaseDlg)
ON_EVENT(CShinFhyo5_01, IDC_ICSDIAG8CTRL2, 1, CShinFhyo5_01::EditONIcsdiag8ctrl2, VTS_I2)
ON_EVENT(CShinFhyo5_01, IDC_ICSDIAG8CTRL2, 2, CShinFhyo5_01::EditOFFIcsdiag8ctrl2, VTS_I2)
ON_EVENT(CShinFhyo5_01, IDC_ICSDIAG8CTRL2, 3, CShinFhyo5_01::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo5_01::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;
}

void CShinFhyo5_01::EditOFFIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char* ptmpMony = NULL;
	char  CkMy[MONY_BUF_SIZE] = { 0 };
	int	  tmpidx = 0;

	switch ( index ) {
		// 2種類以上の事業を営む事業者の場合の控除対象仕入税額
		// （１）事業区分別の課税売上高(税抜)の明細 
		// 事業区分別の合計額
		case ID501_6D:
			// 7D欄～12D欄に金額がある場合集計する
			if ( ( m_pArith->l_test ( F501_7D ) ) || ( m_pArith->l_test ( F501_8D ) ) || ( m_pArith->l_test ( F501_9D ) ) ||
				( m_pArith->l_test ( F501_10D ) ) || ( m_pArith->l_test ( F501_11D ) ) || ( m_pArith->l_test ( F501_12D ) ) ) {
				SyukeiDataSet ( index );
			}
			Disp_f501data ( 5 );
			break;
		case ID501_6E:
			// 7E欄～12E欄に金額がある場合集計する
			if ( ( m_pArith->l_test ( F501_7E ) ) || ( m_pArith->l_test ( F501_8E ) ) || ( m_pArith->l_test ( F501_9E ) ) ||
				( m_pArith->l_test ( F501_10E ) ) || ( m_pArith->l_test ( F501_11E ) ) || ( m_pArith->l_test ( F501_12E ) ) ) {
				SyukeiDataSet ( index );
			}
			Disp_f501data ( 5 );
			break;
/*		case ID501_6D:
		case ID501_6E:
			// 6F欄が0の時またはマイナス値の場合、6D欄と6E欄を手入力しても集計されないため処理変更
			//------------------->2019.12.20.yokono
			//⑥事業区分別の合計が、⑦～⑫に金額が入っていなくても入力できるようになっていた点を修正。
			memset( CkMy, '\0', MONY_BUF_SIZE );
			m_Fhyo5_01Diag02.GetData( ID501_6F, ( struct IUnknown* )&DiagData );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( m_pArith->l_test( CkMy ) ) {
				SyukeiDataSet( index );
			}
			Disp_f501data( 5 );
*/
			break;
			//<-------------------------------------

		// 第一種事業
		case ID501_7D:
		case ID501_7E:
		// 第二種事業
		case ID501_8D:
		case ID501_8E:
		// 第三種事業
		case ID501_9D:
		case ID501_9E:
		// 第四種事業
		case ID501_10D:
		case ID501_10E:
		// 第五種事業
		case ID501_11D:
		case ID501_11E:
		// 第六種事業
		case ID501_12D:
		case ID501_12E:
			// TISW修正 2026/01/21 START
			m_Fhyo5_01Diag02.GetData(index, (struct IUnknown*)&DiagData);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
			if (!m_pArith->l_test(CkMy)) {
				ResetTaxCellForAmountCell(index);
			}
			// TISW修正 2026/01/21 END
			SyukeiDataSet ( index );
			Disp_f501data ( 5 );
			break;

		case ID501_14D:
		case ID501_14E:
			if ( index == ID501_14D ) {
				tmpidx = ID501_7D;
			}
			else if ( index == ID501_14E ) {
				tmpidx = ID501_7E;
			}
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			m_Fhyo5_01Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( m_pArith->l_test ( CkMy ) ) {
				SyukeiDataSet ( index );
			}
			Disp_f501data ( 5 );
			break;

		case ID501_15D:
		case ID501_15E:
			if ( index == ID501_15D ) {
				tmpidx = ID501_8D;
			}
			else if ( index == ID501_15E ) {
				tmpidx = ID501_8E;
			}
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			m_Fhyo5_01Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( m_pArith->l_test ( CkMy ) ) {
				SyukeiDataSet ( index );
			}
			Disp_f501data ( 5 );
			break;
			
		case ID501_16D:
		case ID501_16E:
			if ( index == ID501_16D ) {
				tmpidx = ID501_9D;
			}
			else if ( index == ID501_16E ) {
				tmpidx = ID501_9E;
			}
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			m_Fhyo5_01Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( m_pArith->l_test ( CkMy ) ) {
				SyukeiDataSet ( index );
			}
			Disp_f501data ( 5 );
			break;

		case ID501_17D:
		case ID501_17E:
			if ( index == ID501_17D ) {
				tmpidx = ID501_10D;
			}
			else if ( index == ID501_17E ) {
				tmpidx = ID501_10E;
			}
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			m_Fhyo5_01Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( m_pArith->l_test ( CkMy ) ) {
				SyukeiDataSet ( index );
			}
			Disp_f501data ( 5 );
			break;

		case ID501_18D:
		case ID501_18E:
			if ( index == ID501_18D ) {
				tmpidx = ID501_11D;
			}
			else if ( index == ID501_18E ) {
				tmpidx = ID501_11E;
			}
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			m_Fhyo5_01Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( m_pArith->l_test ( CkMy ) ) {
				SyukeiDataSet ( index );
			}
			Disp_f501data ( 5 );
			break;

		case ID501_19D:
		case ID501_19E:
			if ( index == ID501_19D ) {
				tmpidx = ID501_12D;
			}
			else if ( index == ID501_19E ) {
				tmpidx = ID501_12E;
			}
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			m_Fhyo5_01Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( m_pArith->l_test ( CkMy ) ) {
				SyukeiDataSet ( index );
			}
			Disp_f501data ( 5 );
			break;
		default:
			break;
	}
}

void CShinFhyo5_01::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if ( nChar == 0 ) {
		return;
	}

	// Shift+TABをTABに、TABをENTERに変換
	short wTerm = 0;
	char VK_FLG = 0x00;
	VK_FLG = ( char )::GetKeyState ( VK_SHIFT );
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
		m_Fhyo5_01Diag02.SetPosition ( pos );
	}
	if ( wTerm == VK_DOWN ) {
		pos = UpDownFocusMoveCheck ( index, 0 );
		m_Fhyo5_01Diag02.SetPosition ( pos );
	}

	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_RIGHT ) || ( wTerm == VK_F3 ) ) {
		m_Fhyo5_01Diag02.SetNextPosition();
	}
	if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) ) {
		m_Fhyo5_01Diag02.SetPrevPosition();
	}

	if ( wTerm == VK_DELETE ) {
		switch ( index ) {
		case ID501_6D:
		case ID501_6E:
			if( index == ID501_6D ){
				F501_6c &=~ 0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("357602"));
			}
			else if( index == ID501_6E ){
				F501_6c &=~ 0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("357603"));
			}
			Disp_f501data(6);
			m_Fhyo5_01Diag02.SetPosition( index );
			break;

			// TISW修正 2026/01/21 START
		//case ID501_7D:
		//case ID501_7E:
		//	if( index == ID501_7D ){
		//		F501_14c &= ~0x80;
		//		(*m_pSyzSyukei)->ResetShinkokuData( "358402" );
		//	}
		//	else if( index == ID501_7E ){
		//		F501_14c &= ~0x800;
		//		(*m_pSyzSyukei)->ResetShinkokuData( "358403" );
		//	}
		//	dsp_cls ( IDC_ICSDIAG8CTRL2, index );
		//	break;

		//case ID501_8D:
		//case ID501_8E:
		//	if( index == ID501_8D ){
		//		F501_15c &= ~0x80;
		//		(*m_pSyzSyukei)->ResetShinkokuData( "358502" );
		//	}
		//	else if( index == ID501_8E ){
		//		F501_15c &= ~0x800;
		//		(*m_pSyzSyukei)->ResetShinkokuData( "358503" );
		//	}
		//	dsp_cls ( IDC_ICSDIAG8CTRL2, index );
		//	break;

		//case ID501_9D:
		//case ID501_9E:
		//	if( index == ID501_9D ){
		//		F501_16c &= ~0x80;
		//		(*m_pSyzSyukei)->ResetShinkokuData( "358602" );
		//	}
		//	else if( index == ID501_9E ){
		//		F501_16c &= ~0x800;
		//		(*m_pSyzSyukei)->ResetShinkokuData( "358603" );
		//	}
		//	dsp_cls ( IDC_ICSDIAG8CTRL2, index );
		//	break;

		//case ID501_10D:
		//case ID501_10E:
		//	if( index == ID501_10D ){
		//		F501_17c &= ~0x80;
		//		(*m_pSyzSyukei)->ResetShinkokuData( "358702" );
		//	}
		//	else if( index == ID501_10E ){
		//		F501_17c &= ~0x800;
		//		(*m_pSyzSyukei)->ResetShinkokuData( "358703" );
		//	}
		//	dsp_cls ( IDC_ICSDIAG8CTRL2, index );
		//	break;

		//case ID501_11D:
		//case ID501_11E:
		//	if( index == ID501_11D ){
		//		F501_18c &= ~0x80;
		//		(*m_pSyzSyukei)->ResetShinkokuData( "358802" );
		//	}
		//	else if( index == ID501_11E ){
		//		F501_18c &= ~0x800;
		//		(*m_pSyzSyukei)->ResetShinkokuData( "358803" );
		//	}
		//	dsp_cls ( IDC_ICSDIAG8CTRL2, index );
		//	break;

		//case ID501_12D:
		//case ID501_12E:
		//	if( index == ID501_12D ){
		//		F501_19c &= ~0x80;
		//		(*m_pSyzSyukei)->ResetShinkokuData( "358902" );
		//	}
		//	else if( index == ID501_12E ){
		//		F501_19c &= ~0x800;
		//		(*m_pSyzSyukei)->ResetShinkokuData( "358903" );
		//	}
		//	dsp_cls ( IDC_ICSDIAG8CTRL2, index );
		//	break;
			// TISW修正 2026/01/21 END

		case ID501_14D:
		case ID501_14E:
			if (index == ID501_14D) {
				F501_14c &= ~0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358402"));
			}
			else if (index == ID501_14E) {
				F501_14c &= ~0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358403"));
			}
			Disp_f501data(13);
			m_Fhyo5_01Diag02.SetPosition( index );
			break;
		case ID501_15D:
		case ID501_15E:
			if (index == ID501_15D) {
				F501_15c &= ~0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358502"));
			}
			else if (index == ID501_15E) {
				F501_15c &= ~0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358503"));
			}
			Disp_f501data(13);
			m_Fhyo5_01Diag02.SetPosition( index );
			break;
		case ID501_16D:
		case ID501_16E:
			if (index == ID501_16D) {
				F501_16c &= ~0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358602"));
			}
			else if (index == ID501_16E) {
				F501_16c &= ~0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358603"));
			}
			Disp_f501data(13);
			m_Fhyo5_01Diag02.SetPosition( index );
			break;
		case ID501_17D:
		case ID501_17E:
			if (index == ID501_17D) {
				F501_17c &= ~0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358702"));
			}
			else if (index == ID501_17E) {
				F501_17c &= ~0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358703"));
			}
			Disp_f501data(13);
			m_Fhyo5_01Diag02.SetPosition( index );
			break;
		case ID501_18D:
		case ID501_18E:
			if (index == ID501_18D) {
				F501_18c &= ~0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358802"));
			}
			else if (index == ID501_18E) {
				F501_18c &= ~0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358803"));
			}
			Disp_f501data(13);
			m_Fhyo5_01Diag02.SetPosition( index );
			break;
		case ID501_19D:
		case ID501_19E:
			if (index == ID501_19D) {
				F501_19c &= ~0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358902"));
			}
			else if (index == ID501_19E) {
				F501_19c &= ~0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("358903"));
			}
			Disp_f501data(13);
			m_Fhyo5_01Diag02.SetPosition( index );
			break;
		default:
			dsp_cls( IDC_ICSDIAG8CTRL2, index );
			break;
		}
	}

	int nowpos = m_Fhyo5_01Diag02.GetPosition();

	RECT rectA, rectB;
	GetWindowRect ( &rectA );
	diag_getrect ( IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl );
	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectA.top ) / 2 ) );
	}
}

// TISW修正 2026/01/21 START
/// <summary>
/// 税抜セルに対応する消費税額セルの状態をリセットする
/// </summary>
/// <param name="index">税抜セルのインデックス</param>
void CShinFhyo5_01::ResetTaxCellForAmountCell(int index) {
	switch (index)
	{
	case ID501_7D:
	case ID501_7E:
		if (index == ID501_7D) {
			F501_14c &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData("358402");
		}
		else if (index == ID501_7E) {
			F501_14c &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData("358403");
		}
		break;

	case ID501_8D:
	case ID501_8E:
		if (index == ID501_8D) {
			F501_15c &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData("358502");
		}
		else if (index == ID501_8E) {
			F501_15c &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData("358503");
		}
		break;

	case ID501_9D:
	case ID501_9E:
		if (index == ID501_9D) {
			F501_16c &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData("358602");
		}
		else if (index == ID501_9E) {
			F501_16c &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData("358603");
		}
		break;

	case ID501_10D:
	case ID501_10E:
		if (index == ID501_10D) {
			F501_17c &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData("358702");
		}
		else if (index == ID501_10E) {
			F501_17c &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData("358703");
		}
		break;

	case ID501_11D:
	case ID501_11E:
		if (index == ID501_11D) {
			F501_18c &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData("358802");
		}
		else if (index == ID501_11E) {
			F501_18c &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData("358803");
		}
		break;

	case ID501_12D:
	case ID501_12E:
		if (index == ID501_12D) {
			F501_19c &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData("358902");
		}
		else if (index == ID501_12E) {
			F501_19c &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData("358903");
		}
		break;
	}
}
// TISW修正 2026/01/21 END


BOOL CShinFhyo5_01::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ) {
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

//========================================
// 各入力データ抽出
//========================================

void CShinFhyo5_01::GetAllDiagData()
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// 課税標準額に対する消費税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_1X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_1X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_1D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_1D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_1E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_1E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_1F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_1F, DiagData.data_val, MONY_BUF_SIZE);

	// 貸倒回収に係る消費税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_2X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_2X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_2D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_2D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_2E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_2E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_2F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_2F, DiagData.data_val, MONY_BUF_SIZE);

	// 売上対価の返還等に係る消費税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_3X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_3X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_3D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_3D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_3E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_3E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_3F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_3F, DiagData.data_val, MONY_BUF_SIZE);

	// 控除対象仕入税額の計算の基礎となる消費税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_4X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_4X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_4D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_4D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_4E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_4E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_4F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_4F, DiagData.data_val, MONY_BUF_SIZE);

	// ④×みなし仕入率
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_5X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_5X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_5D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_5D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_5E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_5E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_5F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_5F, DiagData.data_val, MONY_BUF_SIZE);

	// 事業区分別の合計額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_6X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_6X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_6D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_6D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_6E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_6E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_6F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_6F, DiagData.data_val, MONY_BUF_SIZE);

	// 第一種事業(卸売業)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_7X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_7X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_7D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_7D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_7E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_7E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_7F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_7F, DiagData.data_val, MONY_BUF_SIZE);

	// 第二種事業(小売業)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_8X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_8X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_8D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_8D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_8E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_8E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_8F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_8F, DiagData.data_val, MONY_BUF_SIZE);

	// 第三種事業(製造業等)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_9X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_9X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_9D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_9D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_9E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_9E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_9F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_9F, DiagData.data_val, MONY_BUF_SIZE);


	// 第四種事業(その他)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_10X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_10X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_10D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_10D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_10E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_10E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_10F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_10F, DiagData.data_val, MONY_BUF_SIZE);

	// 第五種事業(サービス業等)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_11X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_11X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_11D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_11D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_11E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_11E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_11F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_11F, DiagData.data_val, MONY_BUF_SIZE);

	// 第六種事業(不動産業)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_12X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_12X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_12D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_12D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_12E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_12E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_12F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_12F, DiagData.data_val, MONY_BUF_SIZE);

	// 事業区分別の合計額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_13X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_13X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_13D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_13D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_13E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_13E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_13F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_13F, DiagData.data_val, MONY_BUF_SIZE);

	// 第一種事業(卸売業)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_14X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_14X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_14D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_14D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_14E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_14E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_14F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_14F, DiagData.data_val, MONY_BUF_SIZE);

	// 第二種事業(小売業)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_15X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_15X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_15D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_15D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_15E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_15E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_15F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_15F, DiagData.data_val, MONY_BUF_SIZE);

	// 第三種事業(製造業等)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_16X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_16X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_16D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_16D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_16E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_16E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_16F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_16F, DiagData.data_val, MONY_BUF_SIZE);

	// 第四種事業(その他)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_17X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_17X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_17D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_17D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_17E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_17E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_17F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_17F, DiagData.data_val, MONY_BUF_SIZE);

	// 第五種事業(サービス業等)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_18X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_18X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_18D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_18D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_18E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_18E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_18F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_18F, DiagData.data_val, MONY_BUF_SIZE);

	// 第六種事業(不動産業)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_19X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_19X, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_19D, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_19D, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_19E, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_19E, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID501_19F, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F501_19F, DiagData.data_val, MONY_BUF_SIZE);
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
int CShinFhyo5_01::SyukeiDataSet ( int cID )
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char val[MONY_BUF_SIZE] = { 0 };

	diag_getdata ( IDC_ICSDIAG8CTRL2, cID, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( val, DiagData.data_val, MONY_BUF_SIZE );

	switch ( cID ) {
	case ID501_1X:
		memmove ( F501_1X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357101" ), F501_1X );
		break;
	case ID501_1D:
		memmove ( F501_1D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357102" ), F501_1D );
		break;
	case ID501_1E:
		memmove ( F501_1E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357103" ), F501_1E );
		break;
	case ID501_2X:
		memmove ( F501_2X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357201" ), F501_2X );
		break;
	case ID501_2D:
		memmove ( F501_2D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357202" ), F501_2D );
		break;
	case ID501_2E:
		memmove ( F501_2E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357203" ), F501_2E );
		break;
	case ID501_3X:
		memmove ( F501_3X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357301" ), F501_3X );
		break;
	case ID501_3D:
		memmove ( F501_3D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357302" ), F501_3D );
		break;
	case ID501_3E:
		memmove ( F501_3E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357303" ), F501_3E );
		break;
	case ID501_4X:
		memmove ( F501_4X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357401" ), F501_4X );
		break;
	case ID501_4D:
		memmove ( F501_4D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357402" ), F501_4D );
		break;
	case ID501_4E:
		memmove ( F501_4E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357403" ), F501_4E );
		break;
	case ID501_5X:
		memmove ( F501_5X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357501" ), F501_5X );
		break;
	case ID501_5D:
		memmove ( F501_5D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357502" ), F501_5D );
		break;
	case ID501_5E:
		memmove ( F501_5E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357503" ), F501_5E );
		break;	
	case ID501_6X:
		memmove ( F501_6X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357601" ), F501_6X );
		break;
	case ID501_6D:
		if (m_pArith->l_cmp(F501_6D, val)) {
			F501_6c |= 0x80;
			memmove(F501_6D, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("357602"), F501_6D);
		}
		break;
	case ID501_6E:
		if (m_pArith->l_cmp(F501_6E, val)) {
			F501_6c |= 0x800;
			memmove(F501_6E, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("357603"), F501_6E);
		}
		break;
	case ID501_7X:
		memmove ( F501_7X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357701" ), F501_7X );
		break;
	case ID501_7D:
		memmove ( F501_7D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357702" ), F501_7D );
		break;
	case ID501_7E:
		memmove ( F501_7E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357703" ), F501_7E );
		break;
	case ID501_8X:
		memmove ( F501_8X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357801" ), F501_8X );
		break;
	case ID501_8D:
		memmove ( F501_8D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357802" ), F501_8D );
		break;
	case ID501_8E:
		memmove ( F501_8E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357803" ), F501_8E );
		break;
	case ID501_9X:
		memmove ( F501_9X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357901" ), F501_9X );
		break;
	case ID501_9D:
		memmove ( F501_9D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357902" ), F501_9D );
		break;
	case ID501_9E:
		memmove ( F501_9E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "357903" ), F501_9E );
		break;
	case ID501_10X:
		memmove ( F501_10X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358001" ), F501_10X );
		break;
	case ID501_10D:
		memmove ( F501_10D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358002" ), F501_10D );
		break;
	case ID501_10E:
		memmove ( F501_10E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358003" ), F501_10E );
		break;
	case ID501_11X:
		memmove ( F501_11X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358101" ), F501_11X );
		break;
	case ID501_11D:
		memmove ( F501_11D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358102" ), F501_11D );
		break;
	case ID501_11E:
		memmove ( F501_11E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358103" ), F501_11E );
		break;
	case ID501_12X:
		memmove ( F501_12X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358201" ), F501_12X );
		break;
	case ID501_12D:
		memmove ( F501_12D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358202" ), F501_12D );
		break;
	case ID501_12E:
		memmove ( F501_12E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358203" ), F501_12E );
		break;
	case ID501_13X:
		//memmove ( F501_12X, val, MONY_BUF_SIZE );
		memmove ( F501_13X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358301" ), F501_13X );
		break;
	case ID501_13D:
		memmove ( F501_13D, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358302" ), F501_13D );
		break;
	case ID501_13E:
		memmove ( F501_13E, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358303" ), F501_13E );
		break;
	case ID501_14X:
		//memmove(F501_13X, val, MONY_BUF_SIZE);
		memmove(F501_14X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("358401"), F501_14X);
		break;
	case ID501_14D:
		if (m_pArith->l_cmp(F501_14D, val)) {
			F501_14c |= 0x80;
			memmove(F501_14D, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358402"), F501_14D);
		}
		break;
	case ID501_14E:
		if (m_pArith->l_cmp(F501_14E, val)) {
			F501_14c |= 0x800;
			memmove(F501_14E, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358403"), F501_14E);
		}
		break;
	case ID501_15X:
		//memmove ( F501_14X, val, MONY_BUF_SIZE );
		memmove ( F501_15X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358501" ), F501_15X );
		break;
	case ID501_15D:
		if (m_pArith->l_cmp(F501_15D, val)) {
			F501_15c |= 0x80;
			memmove(F501_15D, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358502"), F501_15D);
		}
		break;
	case ID501_15E:
		if (m_pArith->l_cmp(F501_15E, val)) {
			F501_15c |= 0x800;
			memmove(F501_15E, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358503"), F501_15E);
		}
		break;
	case ID501_16X:
		memmove ( F501_16X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358601" ), F501_16X );
		break;
	case ID501_16D:
		if (m_pArith->l_cmp(F501_16D, val)) {
			F501_16c |= 0x80;
			memmove(F501_16D, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358602"), F501_16D);
		}
		break;
	case ID501_16E:
		if (m_pArith->l_cmp(F501_16E, val)) {
			F501_16c |= 0x800;
			memmove(F501_16E, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358603"), F501_16E);
		}
		break;
	case ID501_17X:
		memmove ( F501_17X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358701" ), F501_17X );
		break;
	case ID501_17D:
		if (m_pArith->l_cmp(F501_17D, val)) {
			F501_17c |= 0x80;
			memmove(F501_17D, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358702"), F501_17D);
		}
		break;
	case ID501_17E:
		if (m_pArith->l_cmp(F501_17E, val)) {
			F501_17c |= 0x800;
			memmove(F501_17E, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358703"), F501_17E);
		}
		break;
	case ID501_18X:
		memmove ( F501_18X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358801" ), F501_18X );
		break;
	case ID501_18D:
		if (m_pArith->l_cmp(F501_18D, val)) {
			F501_18c |= 0x80;
			memmove(F501_18D, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358802"), F501_18D);
		}
		break;
	case ID501_18E:
		if (m_pArith->l_cmp(F501_18E, val)) {
			F501_18c |= 0x800;
			memmove(F501_18E, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358803"), F501_18E);
		}
		break;
	case ID501_19X:
		memmove ( F501_19X, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "358901" ), F501_19X );
		break;
	case ID501_19D:
		if (m_pArith->l_cmp(F501_19D, val)) {
			F501_19c |= 0x80;
			memmove(F501_19D, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358902"), F501_19D);
		}
		break;
	case ID501_19E:
		if (m_pArith->l_cmp(F501_19E, val)) {
			F501_19c |= 0x800;
			memmove(F501_19E, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("358903"), F501_19E);
		}
		break;
	default:
		break;
	}

	return MemoryReget();
}

//============================================
// 集計モジュールからデータ取得
//--------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//============================================
int CShinFhyo5_01::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {

		//yoshida191216
		//ロックの時もF502_5kbnとかtypeCntとかセットする。
		char buff2[256] = { 0 };
		char wariai[32] = { 0 };

		MoneyBasejagArray money2;
		money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("35F000"));
		m_pArith->l_print(buff2, money2[0][0].arith, _T("SSS"));
		typeCnt = atoi(buff2);


		money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("35B100"));
		::ZeroMemory(wariai, sizeof(wariai));
		m_pArith->l_print(wariai, money2[0][0].arith, _T("sssssssssssss9"));
		F501_7UP = atoi(wariai);

		money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("35B200"));
		::ZeroMemory(wariai, sizeof(wariai));
		m_pArith->l_print(wariai, money2[0][0].arith, _T("sssssssssssss9"));
		F501_8UP = atoi(wariai);

		money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("35B300"));
		::ZeroMemory(wariai, sizeof(wariai));
		m_pArith->l_print(wariai, money2[0][0].arith, _T("sssssssssssss9"));
		F501_9UP = atoi(wariai);

		money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("35B400"));
		::ZeroMemory(wariai, sizeof(wariai));
		m_pArith->l_print(wariai, money2[0][0].arith, _T("sssssssssssss9"));
		F501_10UP = atoi(wariai);

		money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("35B500"));
		::ZeroMemory(wariai, sizeof(wariai));
		m_pArith->l_print(wariai, money2[0][0].arith, _T("sssssssssssss9"));
		F501_11UP = atoi(wariai);

		money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("35B600"));
		::ZeroMemory(wariai, sizeof(wariai));
		m_pArith->l_print(wariai, money2[0][0].arith, _T("sssssssssssss9"));
		F501_12UP = atoi(wariai);


		return 0;
	}

	// 計算項目をすべて再取得
	MoneyBasejagArray money;
	money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35" ) );

	int cnt = ( int )money.GetCount();
	int incnt = 0;
	char buff[256];

	for ( int l = 0; l < cnt; l++ ) {
		incnt = ( int )money[l].GetCount();
		for ( int m = 0; m < incnt; m++ ) {
			// 業種数
			if ( money[l][m].code == _T("35F000") ) {
				m_pArith->l_print( buff, money[l][m].arith, _T("SSS") );
				typeCnt = atoi( buff );
			}
			else if ( money[l][m].code == _T( "357100" ) ) {
				memmove ( F501_1F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357101" ) ) {
				memmove ( F501_1X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357102" ) ) {
				memmove ( F501_1D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357103" ) ) {
				memmove ( F501_1E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357200" ) ) {
				memmove ( F501_2F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357201" ) ) {
				memmove ( F501_2X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357202" ) ) {
				memmove ( F501_2D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357203" ) ) {
				memmove ( F501_2E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357300" ) ) {
				memmove ( F501_3F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357301" ) ) {
				memmove ( F501_3X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357302" ) ) {
				memmove ( F501_3D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357303" ) ) {
				memmove ( F501_3E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357400" ) ) {
				memmove ( F501_4F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357401" ) ) {
				memmove ( F501_4X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357402" ) ) {
				memmove ( F501_4D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357403" ) ) {
				memmove ( F501_4E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357500" ) ) {
				memmove ( F501_5F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357501" ) ) {
				memmove ( F501_5X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357502" ) ) {
				memmove ( F501_5D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357503" ) ) {
				memmove ( F501_5E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357600" ) ) {
				memmove ( F501_6F, money[l][m].arith, MONY_BUF_SIZE );
				memmove ( ( *m_pSnHeadData )->Sn_KURIA, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357601" ) ) {
				memmove ( F501_6X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357602" ) ) {
				memmove ( F501_6D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357603" ) ) {
				memmove ( F501_6E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357700" ) ) {
				memmove ( F501_7F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357701" ) ) {
				memmove ( F501_7X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357702" ) ) {
				memmove ( F501_7D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357703" ) ) {
				memmove ( F501_7E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "35B100" ) ) {
				char wariai[32];
				m_pArith->l_print( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
				F501_7UP = atoi( wariai );
			}
			else if ( money[l][m].code == _T( "357800" ) ) {
				memmove ( F501_8F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357801" ) ) {
				memmove ( F501_8X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357802" ) ) {
				memmove ( F501_8D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357803" ) ) {
				memmove ( F501_8E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "35B200" ) ) {
				char wariai[32];
				m_pArith->l_print( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
				F501_8UP = atoi( wariai );
			}
			else if ( money[l][m].code == _T( "357900" ) ) {
				memmove ( F501_9F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357901" ) ) {
				memmove ( F501_9X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357902" ) ) {
				memmove ( F501_9D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "357903" ) ) {
				memmove ( F501_9E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "35B300" ) ) {
				char wariai[32];
				m_pArith->l_print( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
				F501_9UP = atoi( wariai );
			}
			else if ( money[l][m].code == _T( "358000" ) ) {
				memmove ( F501_10F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358001" ) ) {
				memmove ( F501_10X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358002" ) ) {
				memmove ( F501_10D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358003" ) ) {
				memmove ( F501_10E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "35B400" ) ) {
				char wariai[32];
				m_pArith->l_print( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
				F501_10UP = atoi( wariai );
			}
			else if ( money[l][m].code == _T( "358100" ) ) {
				memmove ( F501_11F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358101" ) ) {
				memmove ( F501_11X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358102" ) ) {
				memmove ( F501_11D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358103" ) ) {
				memmove ( F501_11E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "35B500" ) ) {
				char wariai[32];
				m_pArith->l_print( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
				F501_11UP = atoi( wariai );
			}
			else if ( money[l][m].code == _T( "358200" ) ) {
				memmove ( F501_12F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358201" ) ) {
				memmove ( F501_12X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358202" ) ) {
				memmove ( F501_12D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358203" ) ) {
				memmove ( F501_12E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "35B600" ) ) {
				char wariai[32];
				m_pArith->l_print( wariai, money[l][m].arith, _T( "sssssssssssss9" ) );
				F501_12UP = atoi( wariai );
			}
			else if ( money[l][m].code == _T( "358300" ) ) {
				memmove ( F501_13F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358301" ) ) {
				memmove ( F501_13X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358302" ) ) {
				memmove ( F501_13D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358303" ) ) {
				memmove ( F501_13E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358400" ) ) {
				memmove ( F501_14F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358401" ) ) {
				memmove ( F501_14X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358402" ) ) {
				memmove ( F501_14D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358403" ) ) {
				memmove ( F501_14E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358500" ) ) {
				memmove ( F501_15F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358501" ) ) {
				memmove ( F501_15X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358502" ) ) {
				memmove ( F501_15D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358503" ) ) {
				memmove ( F501_15E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358600" ) ) {
				memmove ( F501_16F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358601" ) ) {
				memmove ( F501_16X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358602" ) ) {
				memmove ( F501_16D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358603" ) ) {
				memmove ( F501_16E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358700" ) ) {
				memmove ( F501_17F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358701" ) ) {
				memmove ( F501_17X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358702" ) ) {
				memmove ( F501_17D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358703" ) ) {
				memmove ( F501_17E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358800" ) ) {
				memmove ( F501_18F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358801" ) ) {
				memmove ( F501_18X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358802" ) ) {
				memmove ( F501_18D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358803" ) ) {
				memmove ( F501_18E, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358900" ) ) {
				memmove ( F501_19F, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358901" ) ) {
				memmove ( F501_19X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358902" ) ) {
				memmove ( F501_19D, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "358903" ) ) {
				memmove ( F501_19E, money[l][m].arith, MONY_BUF_SIZE );
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
BOOL CShinFhyo5_01::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( ( F501_6c  & 0x880 ) || ( F501_14c & 0x880 ) || ( F501_15c & 0x880 ) || ( F501_16c & 0x880 ) ||
			 ( F501_17c & 0x880 ) || ( F501_18c & 0x880 ) || ( F501_19c & 0x880 ) ) {
			ret = TRUE;
		}
	}

	return ret;
}

//====================================================================
// 課税売上割合取得
//--------------------------------------------------------------------
// 引数
//		id			:			項目ID
//		pWari		:			売上割合
//		pArith		:			合計金額
//--------------------------------------------------------------------
// 返送値
//		0			:			正常終了
//		-1
//====================================================================
int CShinFhyo5_01::GetPercentage( int id, unsigned short *pWari, char *pArith )
{
	ASSERT( pWari );
	ASSERT( pArith );
	if( ( pWari == NULL ) || ( pArith == NULL ) ) {
		return -1;
	}

	unsigned short tmpWari = 0;
	char tmpArith[MONY_BUF_SIZE] = { 0 };
	bool notFlg = false;

	*pWari = 0;
	memset( pArith, '\0', MONY_BUF_SIZE );

	switch ( id ) {
	case DAIICHISHU:
		tmpWari = F501_7UP;
		memmove( tmpArith, F501_7F, MONY_BUF_SIZE );
		break;
	case DAINISHU:
		tmpWari = F501_8UP;
		memmove( tmpArith, F501_8F, MONY_BUF_SIZE );
		break;
	case DAISANSHU:
		tmpWari = F501_9UP;
		memmove( tmpArith, F501_9F, MONY_BUF_SIZE );
		break;
	case DAIYONSHU:
		tmpWari = F501_10UP;
		memmove( tmpArith, F501_10F, MONY_BUF_SIZE );
		break;
	case DAIGOSHU:
		tmpWari = F501_11UP;
		memmove( tmpArith, F501_11F, MONY_BUF_SIZE );
		break;
	case DAIROKUSHU:
		tmpWari = F501_12UP;
		memmove( tmpArith, F501_12F, MONY_BUF_SIZE );
		break;
	default:
		notFlg = true;
		break;
	}

	if( !notFlg ) {
		*pWari = tmpWari;
		memmove( pArith, tmpArith, MONY_BUF_SIZE );
	}
	else {
		return -1;
	}

	return 0;
}

//=========================================================
// 2割特例時に手入力可能な項目　20230616
//---------------------------------------------------------
// 返送値
//	TRUE		:		手入力可能項目
//	FALSE		:		それ以外
//=========================================================
BOOL CShinFhyo5_01::CheckInputIndex2wari( int index )
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		switch( index ){
			case ID501_6D:
			case ID501_6E:
			case ID501_8D:
			case ID501_8E:
			case ID501_15D:
			case ID501_15E:
				ret = TRUE;
				break;
		}
	}

	return ret;
}