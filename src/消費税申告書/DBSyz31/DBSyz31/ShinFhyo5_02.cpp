// ShinFhyo5_02.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo5_02.h"
#include "ShinFhyo5_02idx.h"

//#define	SIROIROCOLOR	PALETTERGB( 255, 251, 240 )

// CShinFhyo5_02 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo5_02, CSyzBaseDlg)

CShinFhyo5_02::CShinFhyo5_02(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo5_02::IDD, pParent)
	, m_pParent(pParent)
	, m_pSnFhyo5_02Data(NULL)
	, m_pSnHeadData( NULL )
	, m_pSyzSyukei ( NULL )
{
	memset ( F502_1A, '\0', sizeof ( F502_1A ) );
	memset ( F502_1B, '\0', sizeof ( F502_1B ) );
	memset ( F502_1C, '\0', sizeof ( F502_1C ) );
	memset ( F502_1X, '\0', sizeof ( F502_1X ) );

	memset ( F502_2A, '\0', sizeof ( F502_2A ) );
	memset ( F502_2B, '\0', sizeof ( F502_2B ) );
	memset ( F502_2C, '\0', sizeof ( F502_2C ) );
	memset ( F502_2X, '\0', sizeof ( F502_2X ) );

	memset ( F502_3A, '\0', sizeof ( F502_3A ) );
	memset ( F502_3B, '\0', sizeof ( F502_3B ) );
	memset ( F502_3C, '\0', sizeof ( F502_3C ) );
	memset ( F502_3X, '\0', sizeof ( F502_3X ) );

	memset ( F502_4A, '\0', sizeof ( F502_4A ) );
	memset ( F502_4B, '\0', sizeof ( F502_4B ) );
	memset ( F502_4C, '\0', sizeof ( F502_4C ) );
	memset ( F502_4X, '\0', sizeof ( F502_4X ) );

	memset ( F502_5A, '\0', sizeof ( F502_5A ) );
	memset ( F502_5B, '\0', sizeof ( F502_5B ) );
	memset ( F502_5C, '\0', sizeof ( F502_5C ) );
	memset ( F502_5X, '\0', sizeof ( F502_5X ) );

	memset ( F502_6A, '\0', sizeof ( F502_6A ) );
	memset ( F502_6B, '\0', sizeof ( F502_6B ) );
	memset ( F502_6C, '\0', sizeof ( F502_6C ) );
	memset ( F502_6X, '\0', sizeof ( F502_6X ) );

	memset ( F502_7A, '\0', sizeof ( F502_7A ) );
	memset ( F502_7B, '\0', sizeof ( F502_7B ) );
	memset ( F502_7C, '\0', sizeof ( F502_7C ) );
	memset ( F502_7X, '\0', sizeof ( F502_7X ) );

	memset ( F502_8A, '\0', sizeof ( F502_8A ) );
	memset ( F502_8B, '\0', sizeof ( F502_8B ) );
	memset ( F502_8C, '\0', sizeof ( F502_8C ) );
	memset ( F502_8X, '\0', sizeof ( F502_8X ) );

	memset ( F502_9A, '\0', sizeof ( F502_9A ) );
	memset ( F502_9B, '\0', sizeof ( F502_9B ) );
	memset ( F502_9C, '\0', sizeof ( F502_9C ) );
	memset ( F502_9X, '\0', sizeof ( F502_9X ) );

	memset ( F502_10A, '\0', sizeof ( F502_10A ) );
	memset ( F502_10B, '\0', sizeof ( F502_10B ) );
	memset ( F502_10C, '\0', sizeof ( F502_10C ) );
	memset ( F502_10X, '\0', sizeof ( F502_10X ) );

	memset ( F502_11A, '\0', sizeof ( F502_11A ) );
	memset ( F502_11B, '\0', sizeof ( F502_11B ) );
	memset ( F502_11C, '\0', sizeof ( F502_11C ) );
	memset ( F502_11X, '\0', sizeof ( F502_11X ) );

	memset ( F502_12A, '\0', sizeof ( F502_12A ) );
	memset ( F502_12B, '\0', sizeof ( F502_12B ) );
	memset ( F502_12C, '\0', sizeof ( F502_12C ) );
	memset ( F502_12X, '\0', sizeof ( F502_12X ) );

	memset ( F502_13A, '\0', sizeof ( F502_13A ) );
	memset ( F502_13B, '\0', sizeof ( F502_13B ) );
	memset ( F502_13C, '\0', sizeof ( F502_13C ) );
	memset ( F502_13X, '\0', sizeof ( F502_13X ) );

	memset ( F502_14A, '\0', sizeof ( F502_14A ) );
	memset ( F502_14B, '\0', sizeof ( F502_14B ) );
	memset ( F502_14C, '\0', sizeof ( F502_14C ) );
	memset ( F502_14X, '\0', sizeof ( F502_14X ) );

	memset ( F502_15A, '\0', sizeof ( F502_15A ) );
	memset ( F502_15B, '\0', sizeof ( F502_15B ) );
	memset ( F502_15C, '\0', sizeof ( F502_15C ) );
	memset ( F502_15X, '\0', sizeof ( F502_15X ) );

	memset ( F502_16A, '\0', sizeof ( F502_16A ) );
	memset ( F502_16B, '\0', sizeof ( F502_16B ) );
	memset ( F502_16C, '\0', sizeof ( F502_16C ) );
	memset ( F502_16X, '\0', sizeof ( F502_16X ) );

	memset ( F502_17A, '\0', sizeof ( F502_17A ) );
	memset ( F502_17B, '\0', sizeof ( F502_17B ) );
	memset ( F502_17C, '\0', sizeof ( F502_17C ) );
	memset ( F502_17X, '\0', sizeof ( F502_17X ) );

	memset ( F502_18A, '\0', sizeof ( F502_18A ) );
	memset ( F502_18B, '\0', sizeof ( F502_18B ) );
	memset ( F502_18C, '\0', sizeof ( F502_18C ) );
	memset ( F502_18X, '\0', sizeof ( F502_18X ) );

	memset ( F502_19A, '\0', sizeof ( F502_19A ) );
	memset ( F502_19B, '\0', sizeof ( F502_19B ) );
	memset ( F502_19C, '\0', sizeof ( F502_19C ) );
	memset ( F502_19X, '\0', sizeof ( F502_19X ) );

	F502_6s = 0, F502_7s = 0, F502_8s = 0, F502_9s = 0, F502_10s = 0, F502_11s = 0, F502_12s = 0;
	F502_13s = 0, F502_14s = 0, F502_15s = 0, F502_16s = 0, F502_17s = 0, F502_18s = 0, F502_19s = 0;
	F502_5kbn = 0;
	typeCnt = 0;

	m_curwnd = -1;
}

CShinFhyo5_02::~CShinFhyo5_02()
{
}

void CShinFhyo5_02::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo5_02Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo5_02Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo5_02, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CShinFhyo5_02 メッセージ ハンドラ

BOOL CShinFhyo5_02::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2);
	m_BottomIdx = 249;//コントロール一番最下のDiagのID指定。これで縦スクロール出る

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo5_02::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo5_02::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo5_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
int CShinFhyo5_02::Init()
{
	// データ読み込み
	ReadData();

	// 集計モジュールにデータセット
	ReadData2();
	MemoryReget();
		 
	// ダイアグラム初期属性セット
	InitDiagAttr();

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		m_Fhyo5_02Diag01.ModifyItem ( 11, DIAG_MDFY_TRANSPARENT );
	}

	// 画面表示
	DispFh5_02();

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
int CShinFhyo5_02::InitInfo(CH31SnFhyo5_02Data **pSnFhyo5_02Data, CSnHeadData **pSnHeadData, CArithEx *pArith, CDBNpSub *pZmSub, CSyzSyukei **pSyzSyukei)
{
	ASSERT ( pSnHeadData );
	if ( pSnHeadData == NULL ) {
		return -1;
	}

	ASSERT ( pSnFhyo5_02Data );
	if (pSnFhyo5_02Data == NULL) {
		return -1;
	}

	ASSERT ( pArith );
	if (pArith == NULL) {
		return -1;
	}

	ASSERT ( pZmSub );
	if ( pZmSub == NULL ) {
		return -1;
	}

	ASSERT ( pSyzSyukei );
	if ( pSyzSyukei == NULL ) {
		return -1;
	}

	m_pSnHeadData = pSnHeadData;
	m_pSnFhyo5_02Data = pSnFhyo5_02Data;
	m_pArith = pArith;
	m_pZmSub = pZmSub;
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
int CShinFhyo5_02::InitDiagAttr()
{
	int cnt = 0;

	while ( 1 ) {
		if ( Fhyo5_02[cnt].Index == 0 )
			break;

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo5_02[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				//----> 20230616
				if( (*m_pSnHeadData)->m_DispTabSgn & 0x2000 && !CheckInputIndex2wari( Fhyo5_02[cnt].Index ) ){
					Flg = 0;
				}
				//<---- 20230616
				else{
					Flg = Fhyo5_02[cnt].UnConnect;
				}
			}
			else {
				Flg = Fhyo5_02[cnt].Connect;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL2, Fhyo5_02[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo5_02[cnt].Connect );
		cnt++;

	}

	m_Fhyo5_02Diag02.EnableDelete ( 0 );

	return 0;
}

//======================================
// 初期ポジションセット
//======================================
void CShinFhyo5_02::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		//index = m_Fhyo5_02Diag02.GetPosition();
		if ( ( index = m_Fhyo5_02Diag02.GetPosition() ) != -1 ) {
			m_Fhyo5_02Diag02.SetFocus();	// ダミーでセット
			m_Fhyo5_02Diag02.SetPosition ( index );
			m_Fhyo5_02Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo5_02Diag01.SetFocus();	// ダミーでセット
	m_Fhyo5_02Diag02.SetFocus();

	//----> 20230616
	if( (*m_pSnHeadData)->m_DispTabSgn & 0x2000 ){
		m_Fhyo5_02Diag02.SetPosition(ID502_8A);
	}
	//<---- 20230616
	else{
		//　第一種事業　旧税率分小計欄
		m_Fhyo5_02Diag02.SetPosition(ID502_6A);
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
void CShinFhyo5_02::ChangeColor(unsigned short id, short index, int sign)
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
int CShinFhyo5_02::UpDownFocusMoveCheck(int Index, int Move)
{
	int idx = 0;
	int i = 0, j = 0;

	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY5_02CNT; i++ ) {
		if ( Index == FCS_Fhyo5_02[i].IDNo )
			break;
	}

	// 現在ポジションセット
	idx = FCS_Fhyo5_02[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) {			//　下方向
		for ( j = i + 1; j < FCS_FHY5_02CNT; j++ ) {
			m_Fhyo5_02Diag02.GetAttribute ( FCS_Fhyo5_02[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo5_02[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {		//　上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo5_02Diag02.GetAttribute ( FCS_Fhyo5_02[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo5_02[j].IDNo;
				break;
			}
		}
	}

	return idx;
}


BEGIN_EVENTSINK_MAP(CShinFhyo5_02, CSyzBaseDlg)
	ON_EVENT(CShinFhyo5_02, IDC_ICSDIAG8CTRL2, 1, CShinFhyo5_02::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo5_02, IDC_ICSDIAG8CTRL2, 2, CShinFhyo5_02::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo5_02, IDC_ICSDIAG8CTRL2, 3, CShinFhyo5_02::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo5_02::EditONIcsdiag8ctrl2(short index)
{
	m_curwnd = IDC_ICSDIAG8CTRL2;
}

void CShinFhyo5_02::EditOFFIcsdiag8ctrl2(short index)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char  CkMy[MONY_BUF_SIZE] = { 0 };
	int tmpidx = 0;

	switch (index) {
		// 2種類以上の事業を営む事業者の場合の控除対象仕入税額
		// 事業区分別の合計額
		case ID502_6A:
			// 7A～12Aに金額がある場合集計する			
			if ( ( m_pArith->l_test ( F502_7A ) ) || ( m_pArith->l_test ( F502_8A ) ) || ( m_pArith->l_test ( F502_9A ) ) ||
				 ( m_pArith->l_test ( F502_10A ) ) || ( m_pArith->l_test ( F502_11A ) ) || ( m_pArith->l_test ( F502_12A ) ) ) {
				SyukeiDataSet ( index );
			}
			Disp_f502data ( 5 );
			break;
		case ID502_6B:
			// 7B～12Bに金額がある場合集計する			
			if ( ( m_pArith->l_test ( F502_7B ) ) || ( m_pArith->l_test ( F502_8B ) ) || ( m_pArith->l_test ( F502_9B ) ) ||
				( m_pArith->l_test ( F502_10B ) ) || ( m_pArith->l_test ( F502_11B ) ) || ( m_pArith->l_test ( F502_12B ) ) ) {
				SyukeiDataSet ( index );
			}
			Disp_f502data ( 5 );
			break;
		case ID502_6C:
			// 7C～12Cに金額がある場合集計する			
			if ( ( m_pArith->l_test ( F502_7C ) ) || ( m_pArith->l_test ( F502_8C ) ) || ( m_pArith->l_test ( F502_9C ) ) ||
				( m_pArith->l_test ( F502_10C ) ) || ( m_pArith->l_test ( F502_11C ) ) || ( m_pArith->l_test ( F502_12C ) ) ) {
				SyukeiDataSet ( index );
			}
			Disp_f502data ( 5 );
			break;
	// 2種類以上の事業を営む事業者の場合の控除対象仕入税額
	// 事業区分別の合計額

	// 6X欄が0の時、6A～6Cを手入力しても集計されないため処理変更
/*	case ID502_6A:
	case ID502_6B:
	case ID502_6C:
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		m_Fhyo5_02Diag02.GetData ( ID502_6X, ( struct IUnknown* )&DiagData );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_test ( CkMy ) ) {
			SyukeiDataSet ( index );
		}
		Disp_f502data ( 5 );
		break;
*/
	// 第一種事業
	case ID502_7A:
	case ID502_7B:
	case ID502_7C:
	// 第二種事業
	case ID502_8A:
	case ID502_8B:
	case ID502_8C:
	// 第三種事業
	case ID502_9A:
	case ID502_9B:
	case ID502_9C:
	// 第四種事業
	case ID502_10A:
	case ID502_10B:
	case ID502_10C:
	// 第五種事業
	case ID502_11A:
	case ID502_11B:
	case ID502_11C:
	// 第六種事業
	case ID502_12A:
	case ID502_12B:
	case ID502_12C:
	// （１）の事業区分別の課税売上高に係る消費税額の明細
	// 事業区分別の合計額
	case ID502_13A:
	case ID502_13B:
	case ID502_13C:
		SyukeiDataSet ( index );
		Disp_f502data ( 5 );
		break;

	case ID502_14A:
	case ID502_14B:
	case ID502_14C:
		if ( index == ID502_14A ) {
			tmpidx = ID502_7A;
		}
		else if ( index == ID502_14B ) {
			tmpidx = ID502_7B;
		}
		else if ( index == ID502_14C ) {
			tmpidx = ID502_7C;
		}

		memset ( CkMy, '\0', MONY_BUF_SIZE );
		m_Fhyo5_02Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_test ( CkMy ) ) {
			SyukeiDataSet ( index );
		}
		Disp_f502data ( 5 );
		break;

	case ID502_15A:
	case ID502_15B:
	case ID502_15C:
		if ( index == ID502_15A ) {
			tmpidx = ID502_8A;
		}
		else if ( index == ID502_15B ) {
			tmpidx = ID502_8B;
		}
		else if ( index == ID502_15C ) {
			tmpidx = ID502_8C;
		}

		memset ( CkMy, '\0', MONY_BUF_SIZE );
		m_Fhyo5_02Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_test ( CkMy ) ) {
			SyukeiDataSet ( index );
		}
		Disp_f502data ( 5 );
		break;

	case ID502_16A:
	case ID502_16B:
	case ID502_16C:
		if ( index == ID502_16A ) {
			tmpidx = ID502_9A;
		}
		else if ( index == ID502_16B ) {
			tmpidx = ID502_9B;
		}
		else if ( index == ID502_16C ) {
			tmpidx = ID502_9C;
		}

		memset ( CkMy, '\0', MONY_BUF_SIZE );
		m_Fhyo5_02Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_test ( CkMy ) ) {
			SyukeiDataSet ( index );
		}
		Disp_f502data ( 5 );
		break;
	case ID502_17A:
	case ID502_17B:
	case ID502_17C:
		if ( index == ID502_17A ) {
			tmpidx = ID502_10A;
		}
		else if ( index == ID502_17B ) {
			tmpidx = ID502_10B;
		}
		else if ( index == ID502_17C ) {
			tmpidx = ID502_10C;
		}

		memset ( CkMy, '\0', MONY_BUF_SIZE );
		m_Fhyo5_02Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_test ( CkMy ) ) {
			SyukeiDataSet ( index );
		}
		Disp_f502data ( 5 );
		break;

	case ID502_18A:
	case ID502_18B:
	case ID502_18C:
		if ( index == ID502_18A ) {
			tmpidx = ID502_11A;
		}
		else if ( index == ID502_18B ) {
			tmpidx = ID502_11B;
		}
		else if ( index == ID502_18C ) {
			tmpidx = ID502_11C;
		}

		memset ( CkMy, '\0', MONY_BUF_SIZE );
		m_Fhyo5_02Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_test ( CkMy ) ) {
			SyukeiDataSet ( index );
		}
		Disp_f502data ( 5 );
		break;

	case ID502_19A:
	case ID502_19B:
	case ID502_19C:
		if ( index == ID502_19A ) {
			tmpidx = ID502_12A;
		}
		else if ( index == ID502_19B ) {
			tmpidx = ID502_12B;
		}
		else if ( index == ID502_19C ) {
			tmpidx = ID502_12C;
		}

		memset ( CkMy, '\0', MONY_BUF_SIZE );
		m_Fhyo5_02Diag02.GetData ( tmpidx, ( struct IUnknown* )&DiagData );
		memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_test ( CkMy ) ) {
			SyukeiDataSet ( index );
		}
		Disp_f502data ( 5 );
		break;
	default:
		break;
	}
}

void CShinFhyo5_02::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	if (nChar == 0) {
		return;
	}

	// Shift+TABをTABに、TABをENTERに変換
	short wTerm = 0;
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
	if (wTerm == VK_UP) {
		pos = UpDownFocusMoveCheck(index, 1);
		m_Fhyo5_02Diag02.SetPosition(pos);
	}
	if (wTerm == VK_DOWN) {
		pos = UpDownFocusMoveCheck(index, 0);
		m_Fhyo5_02Diag02.SetPosition(pos);
	}

	if ((wTerm == VK_RETURN) || (wTerm == VK_RIGHT) || (wTerm == VK_F3)) {
		m_Fhyo5_02Diag02.SetNextPosition();
	}
	if ((wTerm == VK_TAB) || (wTerm == VK_LEFT) || (wTerm == VK_F2)) {
		m_Fhyo5_02Diag02.SetPrevPosition();
	}

	if (wTerm == VK_DELETE) {
		switch (index) {
		case ID502_6A:
			F502_6s &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350601" ) );
			Disp_f502data ( 6 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_6A );
			break;
		case ID502_6B:
			F502_6s &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350602" ) );
			Disp_f502data ( 6 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_6B );
			break;
		case ID502_6C:
			F502_6s &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350603" ) );
			Disp_f502data ( 6 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_6C );
			break;
		case ID502_7A:
			F502_14s &= ~0x08;
			(*m_pSyzSyukei)->ResetShinkokuData( "351401" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_7B:
			F502_14s &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData( "351402" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_7C:
			F502_14s &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData( "351403" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_8A:
			F502_15s &= ~0x08;
			(*m_pSyzSyukei)->ResetShinkokuData( "351501" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_8B:
			F502_15s &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData( "351502" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_8C:
			F502_15s &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData( "351503" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_9A:
			F502_16s &= ~0x08;
			(*m_pSyzSyukei)->ResetShinkokuData( "351601" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_9B:
			F502_16s &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData( "351602" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_9C:
			F502_16s &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData( "351603" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_10A:
			F502_17s &= ~0x08;
			(*m_pSyzSyukei)->ResetShinkokuData( "351701" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_10B:
			F502_17s &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData( "351702" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_10C:
			F502_17s &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData( "351703" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_11A:
			F502_18s &= ~0x08;
			(*m_pSyzSyukei)->ResetShinkokuData( "351801" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_11B:
			F502_18s &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData( "351802" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_11C:
			F502_18s &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData( "351803" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_12A:
			F502_19s &= ~0x08;
			(*m_pSyzSyukei)->ResetShinkokuData( "351901" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_12B:
			F502_19s &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData( "351902" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		case ID502_12C:
			F502_19s &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData( "351903" );
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;

		case ID502_14A:
			F502_14s &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351401" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_14A );
			break;
		case ID502_14B:
			F502_14s &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351402" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_14B );
			break;
		case ID502_14C:
			F502_14s &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351403" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_14C );
			break;
		case ID502_15A:
			F502_15s &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351501" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_15A );
			break;
		case ID502_15B:
			F502_15s &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351502" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_15B );
			break;
		case ID502_15C:
			F502_15s &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351503" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_15C );
			break;
		case ID502_16A:
			F502_16s &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351601" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_16A );
			break;
		case ID502_16B:
			F502_16s &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351602" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_16B );
			break;
		case ID502_16C:
			F502_16s &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351603" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_16C );
			break;
		case ID502_17A:
			F502_17s &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351701" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_17A );
			break;
		case ID502_17B:
			F502_17s &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351702" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_17B );
			break;
		case ID502_17C:
			F502_17s &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351703" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_17C );
			break;
		case ID502_18A:
			F502_18s &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351801" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_18A );
			break;
		case ID502_18B:
			F502_18s &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351802" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_18B );
			break;
		case ID502_18C:
			F502_18s &= ~0x800;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351803" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_18C );
			break;
		case ID502_19A:
			F502_19s &= ~0x08;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351901" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_19A );
			break;
		case ID502_19B:
			F502_19s &= ~0x80;
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351902" ) );
			Disp_f502data ( 13 );
			m_Fhyo5_02Diag02.SetPosition ( ID502_19B );
			break;
		case ID502_19C:
			F502_19s &= ~0x800;
			Disp_f502data ( 13 );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351903" ) );
			m_Fhyo5_02Diag02.SetPosition ( ID502_19C );
			break;
		default:
			dsp_cls ( IDC_ICSDIAG8CTRL2, index );
			break;
		}
	}

	int nowpos = m_Fhyo5_02Diag02.GetPosition();

	RECT rectA, rectB;
	GetWindowRect(&rectA);
	diag_getrect(IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl);
	if ((rectA.top > rectB.top) || (rectA.bottom < rectB.bottom)) {
		int cpos = GetScrollPos(SB_VERT);
		SetRedrawScroll((cpos + rectB.top - rectA.top) - ((rectA.bottom - rectA.top) / 2));
	}
}

//===================================
// データ読み込み
//===================================
void CShinFhyo5_02::ReadData()
{
	// 課税標準額に対する消費税額
	memmove(F502_1A, (*m_pSnFhyo5_02Data)->Sn_F502_1A, MONY_BUF_SIZE);
	memmove(F502_1B, (*m_pSnFhyo5_02Data)->Sn_F502_1B, MONY_BUF_SIZE);
	memmove(F502_1C, (*m_pSnFhyo5_02Data)->Sn_F502_1C, MONY_BUF_SIZE);
	memmove(F502_1X, (*m_pSnFhyo5_02Data)->Sn_F502_1X, MONY_BUF_SIZE);

	// 貸倒回収に係る消費税額
	memmove(F502_2A, (*m_pSnFhyo5_02Data)->Sn_F502_2A, MONY_BUF_SIZE);
	memmove(F502_2B, (*m_pSnFhyo5_02Data)->Sn_F502_2B, MONY_BUF_SIZE);
	memmove(F502_2C, (*m_pSnFhyo5_02Data)->Sn_F502_2C, MONY_BUF_SIZE);
	memmove(F502_2X, (*m_pSnFhyo5_02Data)->Sn_F502_2X, MONY_BUF_SIZE);

	// 売上対価の返還等に係る消費税額
	memmove(F502_3A, (*m_pSnFhyo5_02Data)->Sn_F502_3A, MONY_BUF_SIZE);
	memmove(F502_3B, (*m_pSnFhyo5_02Data)->Sn_F502_3B, MONY_BUF_SIZE);
	memmove(F502_3C, (*m_pSnFhyo5_02Data)->Sn_F502_3C, MONY_BUF_SIZE);
	memmove(F502_3X, (*m_pSnFhyo5_02Data)->Sn_F502_3X, MONY_BUF_SIZE);

	// 控除対象仕入税額の計算の基礎となる消費税額
	memmove(F502_4A, (*m_pSnFhyo5_02Data)->Sn_F502_4A, MONY_BUF_SIZE);
	memmove(F502_4B, (*m_pSnFhyo5_02Data)->Sn_F502_4B, MONY_BUF_SIZE);
	memmove(F502_4C, (*m_pSnFhyo5_02Data)->Sn_F502_4C, MONY_BUF_SIZE);
	memmove(F502_4X, (*m_pSnFhyo5_02Data)->Sn_F502_4X, MONY_BUF_SIZE);

	// 1種類の事業の専業者の場合の控除対象仕入税額
	memmove(F502_5A, (*m_pSnFhyo5_02Data)->Sn_F502_5A, MONY_BUF_SIZE);
	memmove(F502_5B, (*m_pSnFhyo5_02Data)->Sn_F502_5B, MONY_BUF_SIZE);
	memmove(F502_5C, (*m_pSnFhyo5_02Data)->Sn_F502_5C, MONY_BUF_SIZE);
	memmove(F502_5X, (*m_pSnFhyo5_02Data)->Sn_F502_5X, MONY_BUF_SIZE);

	// 事業区分別の課税売上高(税抜き)の明細
	// 事業区分別の合計額
	memmove(F502_6A, (*m_pSnFhyo5_02Data)->Sn_F502_6A, MONY_BUF_SIZE);
	memmove(F502_6B, (*m_pSnFhyo5_02Data)->Sn_F502_6B, MONY_BUF_SIZE);
	memmove(F502_6C, (*m_pSnFhyo5_02Data)->Sn_F502_6C, MONY_BUF_SIZE);
	memmove(F502_6X, (*m_pSnFhyo5_02Data)->Sn_F502_6X, MONY_BUF_SIZE);
	F502_6s = ( *m_pSnFhyo5_02Data )->Sn_F502_6sw;

	// 第一種事業(卸売業)
	memmove(F502_7A, (*m_pSnFhyo5_02Data)->Sn_F502_7A, MONY_BUF_SIZE);
	memmove(F502_7B, (*m_pSnFhyo5_02Data)->Sn_F502_7B, MONY_BUF_SIZE);
	memmove(F502_7C, (*m_pSnFhyo5_02Data)->Sn_F502_7C, MONY_BUF_SIZE);
	memmove(F502_7X, (*m_pSnFhyo5_02Data)->Sn_F502_7X, MONY_BUF_SIZE);

	// 第二種事業(小売業)
	memmove(F502_8A, (*m_pSnFhyo5_02Data)->Sn_F502_8A, MONY_BUF_SIZE);
	memmove(F502_8B, (*m_pSnFhyo5_02Data)->Sn_F502_8B, MONY_BUF_SIZE);
	memmove(F502_8C, (*m_pSnFhyo5_02Data)->Sn_F502_8C, MONY_BUF_SIZE);
	memmove(F502_8X, (*m_pSnFhyo5_02Data)->Sn_F502_8X, MONY_BUF_SIZE);

	// 第三種事業(製造業等)
	memmove(F502_9A, (*m_pSnFhyo5_02Data)->Sn_F502_9A, MONY_BUF_SIZE);
	memmove(F502_9B, (*m_pSnFhyo5_02Data)->Sn_F502_9B, MONY_BUF_SIZE);
	memmove(F502_9C, (*m_pSnFhyo5_02Data)->Sn_F502_9C, MONY_BUF_SIZE);
	memmove(F502_9X, (*m_pSnFhyo5_02Data)->Sn_F502_9X, MONY_BUF_SIZE);

	// 第四種事業(その他)
	memmove(F502_10A, (*m_pSnFhyo5_02Data)->Sn_F502_10A, MONY_BUF_SIZE);
	memmove(F502_10B, (*m_pSnFhyo5_02Data)->Sn_F502_10B, MONY_BUF_SIZE);
	memmove(F502_10C, (*m_pSnFhyo5_02Data)->Sn_F502_10C, MONY_BUF_SIZE);
	memmove(F502_10X, (*m_pSnFhyo5_02Data)->Sn_F502_10X, MONY_BUF_SIZE);

	// 第五種事業(サービス業)
	memmove(F502_11A, (*m_pSnFhyo5_02Data)->Sn_F502_11A, MONY_BUF_SIZE);
	memmove(F502_11B, (*m_pSnFhyo5_02Data)->Sn_F502_11B, MONY_BUF_SIZE);
	memmove(F502_11C, (*m_pSnFhyo5_02Data)->Sn_F502_11C, MONY_BUF_SIZE);
	memmove(F502_11X, (*m_pSnFhyo5_02Data)->Sn_F502_11X, MONY_BUF_SIZE);

	// 第六種事業(不動産業)
	memmove(F502_12A, (*m_pSnFhyo5_02Data)->Sn_F502_12A, MONY_BUF_SIZE);
	memmove(F502_12B, (*m_pSnFhyo5_02Data)->Sn_F502_12B, MONY_BUF_SIZE);
	memmove(F502_12C, (*m_pSnFhyo5_02Data)->Sn_F502_12C, MONY_BUF_SIZE);
	memmove(F502_12X, (*m_pSnFhyo5_02Data)->Sn_F502_12X, MONY_BUF_SIZE);

	// 事業区分別の課税売上高に係る消費税額の明細
	// 事業区分別の合計額
	memmove(F502_13A, (*m_pSnFhyo5_02Data)->Sn_F502_13A, MONY_BUF_SIZE);
	memmove(F502_13B, (*m_pSnFhyo5_02Data)->Sn_F502_13B, MONY_BUF_SIZE);
	memmove(F502_13C, (*m_pSnFhyo5_02Data)->Sn_F502_13C, MONY_BUF_SIZE);
	memmove(F502_13X, (*m_pSnFhyo5_02Data)->Sn_F502_13X, MONY_BUF_SIZE);

	// 第一種事業(卸売業)
	memmove(F502_14A, (*m_pSnFhyo5_02Data)->Sn_F502_14A, MONY_BUF_SIZE);
	memmove(F502_14B, (*m_pSnFhyo5_02Data)->Sn_F502_14B, MONY_BUF_SIZE);
	memmove(F502_14C, (*m_pSnFhyo5_02Data)->Sn_F502_14C, MONY_BUF_SIZE);
	memmove(F502_14X, (*m_pSnFhyo5_02Data)->Sn_F502_14X, MONY_BUF_SIZE);
	F502_14s = (*m_pSnFhyo5_02Data)->Sn_F502_14sw;	// 手入力サイン

	// 第二種事業(小売業)
	memmove(F502_15A, (*m_pSnFhyo5_02Data)->Sn_F502_15A, MONY_BUF_SIZE);
	memmove(F502_15B, (*m_pSnFhyo5_02Data)->Sn_F502_15B, MONY_BUF_SIZE);
	memmove(F502_15C, (*m_pSnFhyo5_02Data)->Sn_F502_15C, MONY_BUF_SIZE);
	memmove(F502_15X, (*m_pSnFhyo5_02Data)->Sn_F502_15X, MONY_BUF_SIZE);
	F502_15s = (*m_pSnFhyo5_02Data)->Sn_F502_15sw;	// 手入力サイン

	// 第三種事業(製造業等)
	memmove(F502_16A, (*m_pSnFhyo5_02Data)->Sn_F502_16A, MONY_BUF_SIZE);
	memmove(F502_16B, (*m_pSnFhyo5_02Data)->Sn_F502_16B, MONY_BUF_SIZE);
	memmove(F502_16C, (*m_pSnFhyo5_02Data)->Sn_F502_16C, MONY_BUF_SIZE);
	memmove(F502_16X, (*m_pSnFhyo5_02Data)->Sn_F502_16X, MONY_BUF_SIZE);
	F502_16s = (*m_pSnFhyo5_02Data)->Sn_F502_16sw;	// 手入力サイン

	// 第四種事業(その他)
	memmove(F502_17A, (*m_pSnFhyo5_02Data)->Sn_F502_17A, MONY_BUF_SIZE);
	memmove(F502_17B, (*m_pSnFhyo5_02Data)->Sn_F502_17B, MONY_BUF_SIZE);
	memmove(F502_17C, (*m_pSnFhyo5_02Data)->Sn_F502_17C, MONY_BUF_SIZE);
	memmove(F502_17X, (*m_pSnFhyo5_02Data)->Sn_F502_17X, MONY_BUF_SIZE);
	F502_17s = (*m_pSnFhyo5_02Data)->Sn_F502_17sw;	// 手入力サイン

	// 第五種事業(サービス業)
	memmove(F502_18A, (*m_pSnFhyo5_02Data)->Sn_F502_18A, MONY_BUF_SIZE);
	memmove(F502_18B, (*m_pSnFhyo5_02Data)->Sn_F502_18B, MONY_BUF_SIZE);
	memmove(F502_18C, (*m_pSnFhyo5_02Data)->Sn_F502_18C, MONY_BUF_SIZE);
	memmove(F502_18X, (*m_pSnFhyo5_02Data)->Sn_F502_18X, MONY_BUF_SIZE);
	F502_18s = (*m_pSnFhyo5_02Data)->Sn_F502_18sw;	// 手入力サイン

	// 第六種事業(不動産業)
	memmove(F502_19A, (*m_pSnFhyo5_02Data)->Sn_F502_19A, MONY_BUF_SIZE);
	memmove(F502_19B, (*m_pSnFhyo5_02Data)->Sn_F502_19B, MONY_BUF_SIZE);
	memmove(F502_19C, (*m_pSnFhyo5_02Data)->Sn_F502_19C, MONY_BUF_SIZE);
	memmove(F502_19X, (*m_pSnFhyo5_02Data)->Sn_F502_19X, MONY_BUF_SIZE);
	F502_19s = (*m_pSnFhyo5_02Data)->Sn_F502_19sw;	// 手入力サイン

}

//===================================
// データを集計モジュールにセット
//===================================
void CShinFhyo5_02::ReadData2()
{
	//----> 20230616
	if( (*m_pSnHeadData)->m_DispTabSgn & 0x2000 ){

		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {	// 非連動、入力ロックの場合

			// 事業区分別の合計と第二種のみ手入力を残し、他は全てリセットする
			F502_7s  = F502_9s  = F502_10s = F502_11s = F502_12s = 0;
			F502_14s = F502_16s = F502_17s = F502_18s = F502_19s = 0;

			// 事業区分別の合計額
			if ( F502_6s & 0x08 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "350601" ), F502_6A );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350601" ) );
			}
			if ( F502_6s & 0x80 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "350602" ), F502_6B );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350602" ) );
			}
			if ( F502_6s & 0x800 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "350603" ), F502_6C );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350603" ) );
			}

			// 第二種売上高
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350801" ), F502_8A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350802" ), F502_8B );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350803" ), F502_8C );

			// 第二種以外の売上高はリセット
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350701" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350702" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350703" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350901" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350902" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350903" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351001" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351002" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351003" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351101" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351102" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351103" ) );		
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351201" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351202" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351203" ) );

			// 第二種消費税
			if ( F502_15s & 0x08 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351501" ), F502_15A );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351501" ) );
			}
			if ( F502_15s & 0x80 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351502" ), F502_15B );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351502" ) );
			}
			if ( F502_15s & 0x800 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351503" ), F502_15C );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351503" ) );
			}

			// 第二種以外の消費税はリセット
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351401" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351402" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351403" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351601" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351602" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351603" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351701" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351702" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351703" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351801" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351802" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351803" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351901" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351902" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351903" ) );
		}
	}
	//<---- 20230616
	else{

		//if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {	// 非連動の場合
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {	// 非連動、入力ロックの場合
			if ( F502_6s & 0x08 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "350601" ), F502_6A );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350601" ) );
			}
			if ( F502_6s & 0x80 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "350602" ), F502_6B );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350602" ) );
			}
			if ( F502_6s & 0x800 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "350603" ), F502_6C );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "350603" ) );
			}

			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350701" ), F502_7A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350702" ), F502_7B );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350703" ), F502_7C );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350801" ), F502_8A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350802" ), F502_8B );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350803" ), F502_8C );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350901" ), F502_9A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350902" ), F502_9B );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350903" ), F502_9C );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351001" ), F502_10A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351002" ), F502_10B );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351003" ), F502_10C );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351101" ), F502_11A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351102" ), F502_11B );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351103" ), F502_11C );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351201" ), F502_12A );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351202" ), F502_12B );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351203" ), F502_12C );
			if ( F502_14s & 0x08 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351401" ), F502_14A );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351401" ) );
			}
			if ( F502_14s & 0x80 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351402" ), F502_14B );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351402" ) );
			}
			if ( F502_14s & 0x800 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351403" ), F502_14C );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351403" ) );
			}

			if ( F502_15s & 0x08 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351501" ), F502_15A );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351501" ) );
			}
			if ( F502_15s & 0x80 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351502" ), F502_15B );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351502" ) );
			}
			if ( F502_15s & 0x800 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351503" ), F502_15C );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351503" ) );
			}

			if ( F502_16s & 0x08 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351601" ), F502_16A );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351601" ) );
			}
			if ( F502_16s & 0x80 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351602" ), F502_16B );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351602" ) );
			}
			if ( F502_16s & 0x800 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351603" ), F502_16C );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351603" ) );
			}

			if ( F502_17s & 0x08 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351701" ), F502_17A );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351701" ) );
			}
			if ( F502_17s & 0x80 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351702" ), F502_17B );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351702" ) );
			}
			if ( F502_17s & 0x800 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351703" ), F502_17C );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351703" ) );
			}

			if ( F502_18s & 0x08 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351801" ), F502_18A );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351801" ) );
			}
			if ( F502_18s & 0x80 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351802" ), F502_18B );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351802" ) );
			}
			if ( F502_18s & 0x800 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351803" ), F502_18C );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351803" ) );
			}

			if ( F502_19s & 0x08 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351901" ), F502_19A );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351901" ) );
			}
			if ( F502_19s & 0x80 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351902" ), F502_19B );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351902" ) );
			}
			if ( F502_19s & 0x800 ) {
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "351903" ), F502_19C );
			}
			else {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "351903" ) );
			}
		}
		else {
		}
	}
}

//===================================
// データ書き込み
//===================================
void CShinFhyo5_02::WriteData()
{
	// 課税標準額に対する消費税額
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_1A, F502_1A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_1B, F502_1B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_1C, F502_1C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_1X, F502_1X, MONY_BUF_SIZE);

	// 貸倒回収に係る消費税額
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_2A, F502_2A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_2B, F502_2B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_2C, F502_2C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_2X, F502_2X, MONY_BUF_SIZE);

	// 売上対価の返還等に係る消費税額
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_3A, F502_3A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_3B, F502_3B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_3C, F502_3C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_3X, F502_3X, MONY_BUF_SIZE);

	// 控除対象仕入税額の計算の基礎となる消費税額
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_4A, F502_4A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_4B, F502_4B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_4C, F502_4C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_4X, F502_4X, MONY_BUF_SIZE);

	// 1種類の事業の専業者の場合の控除対象仕入税額
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_5A, F502_5A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_5B, F502_5B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_5C, F502_5C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_5X, F502_5X, MONY_BUF_SIZE);

	// 事業区分別の課税売上高(税抜き)の明細
	// 事業区分別の合計額
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_6A, F502_6A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_6B, F502_6B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_6C, F502_6C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_6X, F502_6X, MONY_BUF_SIZE);
	(*m_pSnFhyo5_02Data)->Sn_F502_6sw = F502_6s;

	// 第一種事業(卸売業)
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_7A, F502_7A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_7B, F502_7B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_7C, F502_7C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_7X, F502_7X, MONY_BUF_SIZE);

	// 第二種事業(小売業)
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_8A, F502_8A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_8B, F502_8B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_8C, F502_8C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_8X, F502_8X, MONY_BUF_SIZE);

	// 第三種事業(製造業等)
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_9A, F502_9A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_9B, F502_9B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_9C, F502_9C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_9X, F502_9X, MONY_BUF_SIZE);

	// 第四種事業(その他)
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_10A, F502_10A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_10B, F502_10B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_10C, F502_10C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_10X, F502_10X, MONY_BUF_SIZE);

	// 第五種事業(サービス業)
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_11A, F502_11A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_11B, F502_11B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_11C, F502_11C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_11X, F502_11X, MONY_BUF_SIZE);

	// 第六種事業(不動産業)
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_12A, F502_12A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_12B, F502_12B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_12C, F502_12C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_12X, F502_12X, MONY_BUF_SIZE);

	// 事業区分別の課税売上高に係る消費税額の明細
	// 事業区分別の合計額
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_13A, F502_13A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_13B, F502_13B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_13C, F502_13C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_13X, F502_13X, MONY_BUF_SIZE);

	// 第一種事業(卸売業)
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_14A, F502_14A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_14B, F502_14B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_14C, F502_14C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_14X, F502_14X, MONY_BUF_SIZE);
	(*m_pSnFhyo5_02Data)->Sn_F502_14sw = F502_14s;
	if(m_pArith->l_test(F502_14A) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_14AP, F502_14A, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_14AP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_14B) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_14BP, F502_14B, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_14BP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_14C) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_14CP, F502_14C, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_14CP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_14X) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_14XP, F502_14X, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_14XP, '\0', MONY_BUF_SIZE );

	// 第二種事業(小売業)
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_15A, F502_15A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_15B, F502_15B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_15C, F502_15C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_15X, F502_15X, MONY_BUF_SIZE);
	(*m_pSnFhyo5_02Data)->Sn_F502_15sw = F502_15s;
	if(m_pArith->l_test(F502_15A) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_15AP, F502_15A, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_15AP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_15B) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_15BP, F502_15B, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_15BP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_15C) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_15CP, F502_15C, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_15CP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_15X) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_15XP, F502_15X, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_15XP, '\0', MONY_BUF_SIZE );

	// 第三種事業(製造業等)
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_16A, F502_16A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_16B, F502_16B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_16C, F502_16C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_16X, F502_16X, MONY_BUF_SIZE);
	(*m_pSnFhyo5_02Data)->Sn_F502_16sw = F502_16s;
	if(m_pArith->l_test(F502_16A) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_16AP, F502_16A, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_16AP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_16B) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_16BP, F502_16B, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_16BP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_16C) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_16CP, F502_16C, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_16CP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_16X) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_16XP, F502_16X, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_16XP, '\0', MONY_BUF_SIZE );

	// 第四種事業(その他)
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_17A, F502_17A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_17B, F502_17B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_17C, F502_17C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_17X, F502_17X, MONY_BUF_SIZE);
	(*m_pSnFhyo5_02Data)->Sn_F502_17sw = F502_17s;
	if(m_pArith->l_test(F502_17A) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_17AP, F502_17A, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_17AP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_17B) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_17BP, F502_17B, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_17BP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_17C) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_17CP, F502_17C, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_17CP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_17X) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_17XP, F502_17X, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_17XP, '\0', MONY_BUF_SIZE );

	// 第五種事業(サービス業)
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_18A, F502_18A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_18B, F502_18B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_18C, F502_18C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_18X, F502_18X, MONY_BUF_SIZE);
	(*m_pSnFhyo5_02Data)->Sn_F502_18sw = F502_18s;
	if(m_pArith->l_test(F502_18A) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_18AP, F502_18A, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_18AP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_18B) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_18BP, F502_18B, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_18BP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_18C) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_18CP, F502_18C, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_18CP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_18X) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_18XP, F502_18X, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_18XP, '\0', MONY_BUF_SIZE );

	// 第六種事業(不動産業)
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_19A, F502_19A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_19B, F502_19B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_19C, F502_19C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo5_02Data)->Sn_F502_19X, F502_19X, MONY_BUF_SIZE);
	(*m_pSnFhyo5_02Data)->Sn_F502_19sw = F502_19s;
	if(m_pArith->l_test(F502_19A) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_19AP, F502_19A, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_19AP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_19B) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_19BP, F502_19B, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_19BP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_19C) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_19CP, F502_19C, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_19CP, '\0', MONY_BUF_SIZE );
	if(m_pArith->l_test(F502_19X) >= 0)
		memmove ( ( *m_pSnFhyo5_02Data )->Sn_F502_19XP, F502_19X, MONY_BUF_SIZE );
	else
		memset ( ( *m_pSnFhyo5_02Data )->Sn_F502_19XP, '\0', MONY_BUF_SIZE );

	(*m_pSnFhyo5_02Data)->Sn_F502_5kbn = F502_5kbn;
}

//=================================================
// 金額表示
//-------------------------------------------------
// 引数
//		Id		:	ダイアグラムID
//		Index	:	ダイアグラム内インデックス
//		Mony	:	金額
//=================================================
void CShinFhyo5_02::dsp_prs(unsigned short Id, short Index, char* Mony)
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
void CShinFhyo5_02::dsp_cls(unsigned short Id, short Index)
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	switch (Index) {
	case 8:		// 課税期間　自
	case 9:		// 課税期間　至
	case 10:	// 氏名又は、名称
		diag_clear(Id, Index, TRUE, CIcsdiagctrl);
		break;
	default:
		memset(DiagData.data_val, '\0', MONY_BUF_SIZE);
		diag_setdata(Id, Index, (struct IUnknown*)&DiagData, CIcsdiagctrl);
		break;
	}
}

//=====================================
// 課税標準額に対する消費税額
//=====================================
void CShinFhyo5_02::f502_1data()
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_1A, F502_1A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_1B, F502_1B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_1C, F502_1C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_1X, F502_1X);
}

//=====================================
// 貸倒回収に係る消費税額
//=====================================
void CShinFhyo5_02::f502_2data()
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_2A, F502_2A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_2B, F502_2B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_2C, F502_2C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_2X, F502_2X);
}

//=====================================
// 売上対価の返還等対価に係る税額
//=====================================
void CShinFhyo5_02::f502_3data()
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_3A, F502_3A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_3B, F502_3B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_3C, F502_3C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_3X, F502_3X);
}

//===============================================
// 控除対象仕入税額の計算の基礎となる消費税額
//===============================================
void CShinFhyo5_02::f502_4data()
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_4A, F502_4A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_4B, F502_4B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_4C, F502_4C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_4X, F502_4X);
}

//===============================================
// 1種類の事業の専業者の場合の控除対象仕入税額
//===============================================
void CShinFhyo5_02::f502_5data()
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_5A, F502_5A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_5B, F502_5B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_5C, F502_5C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_5X, F502_5X);
}

//=========================================================
//  2種類以上の事業を営む事業者の場合の控除対象仕入税額
//  事業区分別の合計額
//=========================================================
void CShinFhyo5_02::f502_6data()
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_6A, F502_6A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_6B, F502_6B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_6C, F502_6C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_6X, F502_6X);

	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_6A, ( ( F502_6s & 0x08 ) ? 3 : 0 ) );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_6B, ( ( F502_6s & 0x80 ) ? 3 : 0 ) );
		ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_6C, ( ( F502_6s & 0x800 ) ? 3 : 0 ) );
	}
}

//=====================================
// 第一種事業(卸売業)
//=====================================
void CShinFhyo5_02::f502_7data()
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_7A, F502_7A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_7B, F502_7B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_7C, F502_7C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_7X, F502_7X);
}

//=====================================
// 第二種事業(小売業)
//=====================================
void CShinFhyo5_02::f502_8data()
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_8A, F502_8A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_8B, F502_8B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_8C, F502_8C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_8X, F502_8X);
}

//=====================================
// 第三種事業(製造業等)
//=====================================
void CShinFhyo5_02::f502_9data()
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_9A, F502_9A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_9B, F502_9B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_9C, F502_9C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_9X, F502_9X);
}

//=====================================
// 第四種事業(その他)
//=====================================
void CShinFhyo5_02::f502_10data()
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_10A, F502_10A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_10B, F502_10B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_10C, F502_10C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_10X, F502_10X);
}

//=====================================
// 第五種事業(サービス業)
//=====================================
void CShinFhyo5_02::f502_11data()
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_11A, F502_11A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_11B, F502_11B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_11C, F502_11C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_11X, F502_11X);
}

//=====================================
// 第六種事業(不動産業)
//=====================================
void CShinFhyo5_02::f502_12data()
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_12A, F502_12A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_12B, F502_12B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_12C, F502_12C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_12X, F502_12X);
}

//===============================================
// 事業区分別の課税売上高に係る消費税額の明細
// 事業区分別の合計額
//===============================================
void CShinFhyo5_02::f502_13data()
{
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_13A, F502_13A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_13B, F502_13B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_13C, F502_13C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_13X, F502_13X);
}

//=====================================
// 第一種事業(卸売業)
//=====================================
void CShinFhyo5_02::f502_14data()
{
//	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) && !( ( *m_pSnHeadData )->m_DispTabSgn & 0x2000 ) ) {	// 20230616

		// 手入力サインなし
		if ( !( F502_14s & 0x08 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_14A, 0 );
			if ( ( F502_7s & 0x01 ) || ( ( m_pArith->l_test ( F502_7A ) ) && ( m_pArith->l_test ( F502_14A ) == 0 ) ) ) {
				memset ( F502_14A, '\0', MONY_BUF_SIZE );
				F502_7s &= ~0x0f;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_14A, 3 );
		}

		// 手入力サインなし
		if ( !( F502_14s & 0x80 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_14B, 0 );
			if ( ( F502_7s & 0x10 ) || ( ( m_pArith->l_test ( F502_7B ) ) && ( m_pArith->l_test ( F502_14B ) == 0 ) ) ) {
				memset ( F502_14B, '\0', MONY_BUF_SIZE );
				F502_7s &= ~0xf0;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_14B, 3 );
		}

		// 手入力サインなし
		if ( !( F502_14s & 0x800 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_14C, 0 );
			if ( ( F502_7s & 0x100 ) || ( ( m_pArith->l_test ( F502_7C ) ) && ( m_pArith->l_test ( F502_14C ) == 0 ) ) ) {
				memset ( F502_14C, '\0', MONY_BUF_SIZE );
				F502_7s &= ~0xf00;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_14C, 3 );
		}
	}

	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_14A, F502_14A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_14B, F502_14B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_14C, F502_14C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_14X, F502_14X);
}

//=====================================
// 第二種事業(小売業)
//=====================================
void CShinFhyo5_02::f502_15data()
{
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {

		// 手入力サインなし
		if ( !( F502_15s & 0x08 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_15A, 0 );
			if ( ( F502_8s & 0x01 ) || ( ( m_pArith->l_test ( F502_8A ) ) && ( m_pArith->l_test ( F502_15A ) == 0 ) ) ) {
				memset ( F502_15A, '\0', MONY_BUF_SIZE );
				F502_8s &= ~0x0f;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_15A, 3 );
		}

		// 手入力サインなし
		if ( !( F502_15s & 0x80 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_15B, 0 );
			if ( ( F502_8s & 0x10 ) || ( ( m_pArith->l_test ( F502_8B ) ) && ( m_pArith->l_test ( F502_15B ) == 0 ) ) ) {
				memset ( F502_15B, '\0', MONY_BUF_SIZE );
				F502_8s &= ~0xf0;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_15B, 3 );
		}

		// 手入力サインなし
		if ( !( F502_15s & 0x800 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_15C, 0 );
			if ( ( F502_8s & 0x100 ) || ( ( m_pArith->l_test ( F502_8C ) ) && ( m_pArith->l_test ( F502_15C ) == 0 ) ) ) {
				memset ( F502_15C, '\0', MONY_BUF_SIZE );
				F502_8s &= ~0xf00;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_15C, 3 );
		}
	}

	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_15A, F502_15A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_15B, F502_15B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_15C, F502_15C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_15X, F502_15X);
}

//=====================================
// 第三種事業(製造業等)
//=====================================
void CShinFhyo5_02::f502_16data()
{
//	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) && !( ( *m_pSnHeadData )->m_DispTabSgn & 0x2000 ) ) {	// 20230616

		// 手入力サインなし
		if ( !( F502_16s & 0x08 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_16A, 0 );
			if ( ( F502_9s & 0x01 ) || ( ( m_pArith->l_test ( F502_9A ) ) && ( m_pArith->l_test ( F502_16A ) == 0 ) ) ) {
				memset ( F502_16A, '\0', MONY_BUF_SIZE );
				F502_9s &= ~0x0f;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_16A, 3 );
		}

		// 手入力サインなし
		if ( !( F502_16s & 0x80 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_16B, 0 );
			if ( ( F502_9s & 0x10 ) || ( ( m_pArith->l_test ( F502_9B ) ) && ( m_pArith->l_test ( F502_16B ) == 0 ) ) ) {
				memset ( F502_16B, '\0', MONY_BUF_SIZE );
				F502_9s &= ~0xf0;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_16B, 3 );
		}

		// 手入力サインなし
		if ( !( F502_16s & 0x800 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_16C, 0 );
			if ( ( F502_9s & 0x100 ) || ( ( m_pArith->l_test ( F502_9C ) ) && ( m_pArith->l_test ( F502_16C ) == 0 ) ) ) {
				memset ( F502_16C, '\0', MONY_BUF_SIZE );
				F502_9s &= ~0xf00;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_16C, 3 );
		}
	}

	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_16A, F502_16A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_16B, F502_16B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_16C, F502_16C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_16X, F502_16X);
}

//=====================================
// 第四種事業(その他)
//=====================================
void CShinFhyo5_02::f502_17data()
{
//	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) && !( ( *m_pSnHeadData )->m_DispTabSgn & 0x2000 ) ) {	// 20230616

		// 手入力サインなし
		if ( !( F502_17s & 0x08 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_17A, 0 );
			if ( ( F502_10s & 0x01 ) || ( ( m_pArith->l_test ( F502_10A ) ) && ( m_pArith->l_test ( F502_17A ) == 0 ) ) ) {
				memset ( F502_17A, '\0', MONY_BUF_SIZE );
				F502_10s &= ~0x0f;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_17A, 3 );
		}

		// 手入力サインなし
		if ( !( F502_17s & 0x80 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_17B, 0 );
			if ( ( F502_10s & 0x10 ) || ( ( m_pArith->l_test ( F502_10B ) ) && ( m_pArith->l_test ( F502_17B ) == 0 ) ) ) {
				memset ( F502_17B, '\0', MONY_BUF_SIZE );
				F502_10s &= ~0xf0;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_17B, 3 );
		}

		// 手入力サインなし
		if ( !( F502_17s & 0x800 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_17C, 0 );
			if ( ( F502_10s & 0x100 ) || ( ( m_pArith->l_test ( F502_10C ) ) && ( m_pArith->l_test ( F502_17C ) == 0 ) ) ) {
				memset ( F502_17C, '\0', MONY_BUF_SIZE );
				F502_10s &= ~0xf00;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_17C, 3 );
		}
	}

	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_17A, F502_17A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_17B, F502_17B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_17C, F502_17C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_17X, F502_17X);
}

//=====================================
// 第五種事業(サービス業)
//=====================================
void CShinFhyo5_02::f502_18data()
{
//	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) && !( ( *m_pSnHeadData )->m_DispTabSgn & 0x2000 ) ) {	// 20230616

		// 手入力サインなし
		if ( !( F502_18s & 0x08 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_18A, 0 );
			if ( ( F502_11s & 0x01 ) || ( ( m_pArith->l_test ( F502_11A ) ) && ( m_pArith->l_test ( F502_18A ) == 0 ) ) ) {
				memset ( F502_18A, '\0', MONY_BUF_SIZE );
				F502_11s &= ~0x0f;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_18A, 3 );
		}

		// 手入力サインなし
		if ( !( F502_18s & 0x80 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_18B, 0 );
			if ( ( F502_11s & 0x10 ) || ( ( m_pArith->l_test ( F502_11B ) ) && ( m_pArith->l_test ( F502_18B ) == 0 ) ) ) {
				memset ( F502_18B, '\0', MONY_BUF_SIZE );
				F502_11s &= ~0xf0;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_18B, 3 );
		}

		// 手入力サインなし
		if ( !( F502_18s & 0x800 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_18C, 0 );
			if ( ( F502_11s & 0x100 ) || ( ( m_pArith->l_test ( F502_11C ) ) && ( m_pArith->l_test ( F502_18C ) == 0 ) ) ) {
				memset ( F502_18C, '\0', MONY_BUF_SIZE );
				F502_11s &= ~0xf00;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_18C, 3 );
		}
	}

	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_18A, F502_18A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_18B, F502_18B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_18C, F502_18C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_18X, F502_18X);
}

//=====================================
// 第六種事業(不動産業)
//=====================================
void CShinFhyo5_02::f502_19data()
{
//	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) ) {
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) && !( ( *m_pSnHeadData )->m_DispTabSgn & 0x2000 ) ) {	// 20230616

		// 手入力サインなし
		if ( !( F502_19s & 0x08 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_19A, 0 );
			if ( ( F502_12s & 0x01 ) || ( ( m_pArith->l_test ( F502_12A ) ) && ( m_pArith->l_test ( F502_19A ) == 0 ) ) ) {
				memset ( F502_19A, '\0', MONY_BUF_SIZE );
				F502_12s &= ~0x0f;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_19A, 3 );
		}

		// 手入力サインなし
		if ( !( F502_19s & 0x80 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_19B, 0 );
			//if ( ( F502_12s & 0x10 ) || ( ( m_pArith->l_test ( F502_12B ) ) && ( m_pArith->l_test ( F502_12B ) == 0 ) ) ) {
			if ( ( F502_12s & 0x10 ) || ( ( m_pArith->l_test ( F502_12B ) ) && ( m_pArith->l_test ( F502_19B ) == 0 ) ) ) {
				memset ( F502_19B, '\0', MONY_BUF_SIZE );
				F502_12s &= ~0xf0;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_19B, 3 );
		}

		// 手入力サインなし
		if ( !( F502_19s & 0x800 ) ) {
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_19C, 0 );
			if ( ( F502_12s & 0x100 ) || ( ( m_pArith->l_test ( F502_12C ) ) && ( m_pArith->l_test ( F502_19C ) == 0 ) ) ) {
				memset ( F502_19C, '\0', MONY_BUF_SIZE );
				F502_12s &= ~0xf00;
			}
		}
		else {	// あり
			ChangeColor ( IDC_ICSDIAG8CTRL2, ID502_19C, 3 );
		}
	}

	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_19A, F502_19A);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_19B, F502_19B);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_19C, F502_19C);
	dsp_prs(IDC_ICSDIAG8CTRL2, ID502_19X, F502_19X);
}


//===================================
// 各入力データ表示
//-----------------------------------
// 引数
//		index	：　
//===================================
void CShinFhyo5_02::Disp_f502data( int index )
{
	
	switch ( index ) {
		case 0:
		case 1:
			f502_1data();
		case 2:
			f502_2data();
		case 3:
			f502_3data();
		case 4:
			f502_4data();
		case 5:
			f502_5data();
		case 6:
			f502_6data();
		case 7:
			f502_7data();
		case 8:
			f502_8data();
		case 9:
			f502_9data();
		case 10:
			f502_10data();
		case 11:
			f502_11data();
		case 12:
			f502_12data();
		case 13:
			f502_13data();
		case 14:
			f502_14data();
		case 15:
			f502_15data();
		case 16:
			f502_16data();
		case 17:
			f502_17data();
		case 18:
			f502_18data();
		case 19:
			f502_19data();
		default :
			break;
	}
	
}

//===================================
// 画面表示
//===================================
void CShinFhyo5_02::DispFh5_02()
{
	DIAGRAM_DATA	DiagData;
	DIAGRAM_ATTRIBUTE	DA;
	DiagData.data_imgdata = NULL;
	char			buf[512] = {0};


	// 画面初期化
	for (int i = 8; i <= 10; i++ ) {					// 課税期間、氏名又は名称
		dsp_cls(IDC_ICSDIAG8CTRL1, (short)i);
	}
	for (int i = ID502_1A; i <= ID502_19X; i++ ) {	// 金額欄
		dsp_cls(IDC_ICSDIAG8CTRL2, (short)i);
	}

	// 手入力サイン初期化
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		F502_6s = 0, F502_7s = 0, F502_8s = 0, F502_9s = 0, F502_10s = 0, F502_11s = 0, F502_12s = 0;
		F502_13s = 0, F502_14s = 0, F502_15s = 0, F502_16s = 0, F502_17s = 0, F502_18s = 0, F502_19s = 0;
	}

	// 課税期間の表示
	char yy, mm, dd;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s( buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy & 0xff), (mm & 0xff), (dd & 0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata(IDC_ICSDIAG8CTRL1, 8, (struct IUnknown*)&DiagData, CIcsdiagctrl);

	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd);
	sprintf_s(buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy & 0xff), (mm & 0xff), (dd & 0xff));
	DiagData.data_disp = (char*)buf;
	diag_setdata(IDC_ICSDIAG8CTRL1, 9, (struct IUnknown*)&DiagData, CIcsdiagctrl);


	// 氏名又は名称の表示
	memset(buf, '\0', sizeof(buf));
	if ((*m_pSnHeadData)->IsSoleProprietor()) {
		memmove(buf, (*m_pSnHeadData)->Sn_DIHYO, 40);
	}
	else {
		memmove(buf, (*m_pSnHeadData)->Sn_CONAM, 40);
	}
	DiagData.data_disp = (char*)buf;
	diag_setdata(IDC_ICSDIAG8CTRL1, 10, (struct IUnknown*)&DiagData, CIcsdiagctrl);

	// 連動マスター
	//if ( m_Util.IsUnConnectMst ( m_pZmSub->zvol ) == FALSE ){
	// 連動時には「●単一事業を営む～」を非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) ) {
		diag_clear ( IDC_ICSDIAG8CTRL1, 4, TRUE, CIcsdiagctrl );
		diag_getattr ( IDC_ICSDIAG8CTRL1, 4, &DA, CIcsdiagctrl );
		DA.attr_bcolor = SIROIROCOLOR;
		diag_setattr ( IDC_ICSDIAG8CTRL1, 4, &DA, FALSE, CIcsdiagctrl );
	}

	// 各入力データ表示
	Disp_f502data(0);
}

BOOL CShinFhyo5_02::PreTranslateMessage(MSG* pMsg)
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
				if (m_curwnd == IDC_ICSDIAG8CTRL2) {
					TerminationIcsdiag8ctrl2(result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd);
				}

				return TRUE;
			}
		}
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

//======================================
// 各入力データ抽出
//======================================
void CShinFhyo5_02::GetAllDiagData()
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// 課税標準額に対する消費税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_1A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_1A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_1B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_1B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_1C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_1C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_1X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_1X, DiagData.data_val, MONY_BUF_SIZE);

	// 貸倒回収に係る消費税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_2A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_2A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_2B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_2B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_2C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_2C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_2X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_2X, DiagData.data_val, MONY_BUF_SIZE);

	// 売上対価の返還等に係る消費税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_3A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_3A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_3B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_3B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_3C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_3C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_3X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_3X, DiagData.data_val, MONY_BUF_SIZE);

	// 控除対象仕入税額の計算の基礎となる消費税額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_4A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_4A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_4B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_4B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_4C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_4C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_4X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_4X, DiagData.data_val, MONY_BUF_SIZE);

	// ④×みなし仕入率
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_5A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_5A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_5B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_5B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_5C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_5C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_5X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_5X, DiagData.data_val, MONY_BUF_SIZE);

	// 事業区分別の合計額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_6A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_6A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_6B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_6B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_6C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_6C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_6X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_6X, DiagData.data_val, MONY_BUF_SIZE);

	// 第一種事業(卸売業)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_7A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_7A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_7B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_7B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_7C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_7C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_7X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_7X, DiagData.data_val, MONY_BUF_SIZE);

	// 第二種事業(小売業)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_8A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_8A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_8B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_8B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_8C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_8C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_8X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_8X, DiagData.data_val, MONY_BUF_SIZE);

	// 第三種事業(製造業等)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_9A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_9A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_9B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_9B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_9C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_9C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_9X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_9X, DiagData.data_val, MONY_BUF_SIZE);


	// 第四種事業(その他)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_10A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_10A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_10B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_10B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_10C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_10C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_10X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_10X, DiagData.data_val, MONY_BUF_SIZE);

	// 第五種事業(サービス業等)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_11A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_11A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_11B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_11B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_11C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_11C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_11X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_11X, DiagData.data_val, MONY_BUF_SIZE);

	// 第六種事業(不動産業)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_12A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_12A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_12B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_12B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_12C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_12C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_12X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_12X, DiagData.data_val, MONY_BUF_SIZE);

	// 事業区分別の合計額
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_13A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_13A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_13B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_13B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_13C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_13C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_13X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_13X, DiagData.data_val, MONY_BUF_SIZE);

	// 第一種事業(卸売業)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_14A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_14A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_14B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_14B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_14C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_14C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_14X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_14X, DiagData.data_val, MONY_BUF_SIZE);

	// 第二種事業(小売業)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_15A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_15A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_15B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_15B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_15C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_15C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_15X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_15X, DiagData.data_val, MONY_BUF_SIZE);

	// 第三種事業(製造業等)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_16A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_16A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_16B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_16B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_16C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_16C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_16X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_16X, DiagData.data_val, MONY_BUF_SIZE);

	// 第四種事業(その他)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_17A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_17A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_17B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_17B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_17C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_17C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_17X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_17X, DiagData.data_val, MONY_BUF_SIZE);

	// 第五種事業(サービス業等)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_18A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_18A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_18B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_18B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_18C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_18C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_18X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_18X, DiagData.data_val, MONY_BUF_SIZE);

	// 第六種事業(不動産業)
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_19A, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_19A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_19B, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_19B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_19C, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_19C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL2, ID502_19X, (struct IUnknown*)&DiagData, CIcsdiagctrl);
	memmove(F502_19X, DiagData.data_val, MONY_BUF_SIZE);

}


//=====================================================
// 集計モジュール更新
//-----------------------------------------------------
// 引数
//		cID		:		ダイアグラム内インデックス
//-----------------------------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//=====================================================
int CShinFhyo5_02::SyukeiDataSet ( int cID )
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char val[MONY_BUF_SIZE] = { 0 };

	diag_getdata ( IDC_ICSDIAG8CTRL2, cID, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	memmove ( val, DiagData.data_val, MONY_BUF_SIZE );

	switch ( cID ) {
	case ID502_6A:
		if ( m_pArith->l_cmp ( F502_6A, val ) ) {
			F502_6s |= 0x08;
			memmove ( F502_6A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350601" ), F502_6A );
		}
		break;
	case ID502_6B:
		if ( m_pArith->l_cmp ( F502_6B, val ) ) {
			F502_6s |= 0x80;
			memmove ( F502_6B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350602" ), F502_6B );
		}
		break;
	case ID502_6C:
		if ( m_pArith->l_cmp ( F502_6C, val ) ) {
			F502_6s |= 0x800;
			memmove ( F502_6C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "350603" ), F502_6C );
		}
		break;
	case ID502_7A:
		memmove ( F502_7A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "350701" ), F502_7A );
		break;
	case ID502_7B:
		memmove ( F502_7B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "350702" ), F502_7B );
		break;
	case ID502_7C:
		memmove ( F502_7C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "350703" ), F502_7C );
		break;
	case ID502_8A:
		memmove ( F502_8A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "350801" ), F502_8A );
		break;
	case ID502_8B:
		memmove ( F502_8B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "350802" ), F502_8B );
		break;
	case ID502_8C:
		memmove ( F502_8C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "350803" ), F502_8C );
		break;
	case ID502_9A:
		memmove ( F502_9A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "350901" ), F502_9A );
		break;
	case ID502_9B:
		memmove ( F502_9B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "350902" ), F502_9B );
		break;
	case ID502_9C:
		memmove ( F502_9C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "350903" ), F502_9C );
		break;
	case ID502_10A:
		memmove ( F502_10A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "351001" ), F502_10A );
		break;
	case ID502_10B:
		memmove ( F502_10B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "351002" ), F502_10B );
		break;
	case ID502_10C:
		memmove ( F502_10C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "351003" ), F502_10C );
		break;
	case ID502_11A:
		memmove ( F502_11A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "351101" ), F502_11A );
		break;
	case ID502_11B:
		memmove ( F502_11B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "351102" ), F502_11B );
		break;
	case ID502_11C:
		memmove ( F502_11C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "351103" ), F502_11C );
		break;
	case ID502_12A:
		memmove ( F502_12A, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "351201" ), F502_12A );
		break;
	case ID502_12B:
		memmove ( F502_12B, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "351202" ), F502_12B );
		break;
	case ID502_12C:
		memmove ( F502_12C, val, MONY_BUF_SIZE );
		( *m_pSyzSyukei )->SetShinkokuData ( _T( "351203" ), F502_12C );
		break;
	case ID502_14A:
		if ( m_pArith->l_cmp ( F502_14A, val ) ) {
			F502_14s |= 0x08;
			memmove ( F502_14A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351401" ), F502_14A );
		}
		break;
	case ID502_14B:
		if ( m_pArith->l_cmp ( F502_14B, val ) ) {
			F502_14s |= 0x80;
			memmove ( F502_14B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351402" ), F502_14B );
		}
		break;
	case ID502_14C:
		if ( m_pArith->l_cmp ( F502_14C, val ) ) {
			F502_14s |= 0x800;
			memmove ( F502_14C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351403" ), F502_14C );
		}
		break;
	case ID502_15A:
		if ( m_pArith->l_cmp ( F502_15A, val ) ) {
			F502_15s |= 0x08;
			memmove ( F502_15A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351501" ), F502_15A );
		}
		break;
	case ID502_15B:
		if ( m_pArith->l_cmp ( F502_15B, val ) ) {
			F502_15s |= 0x80;
			memmove ( F502_15B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351502" ), F502_15B );
		}
		break;
	case ID502_15C:
		if ( m_pArith->l_cmp ( F502_15C, val ) ) {
			F502_15s |= 0x800;
			memmove ( F502_15C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351503" ), F502_15C );
		}
		break;
	case ID502_16A:
		if ( m_pArith->l_cmp ( F502_16A, val ) ) {
			F502_16s |= 0x08;
			memmove ( F502_16A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351601" ), F502_16A );
		}
		break;
	case ID502_16B:
		if ( m_pArith->l_cmp ( F502_16B, val ) ) {
			F502_16s |= 0x80;
			memmove ( F502_16B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351602" ), F502_16B );
		}
		break;
	case ID502_16C:
		if ( m_pArith->l_cmp ( F502_16C, val ) ) {
			F502_16s |= 0x800;
			memmove ( F502_16C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351603" ), F502_16C );
		}
		break;
	case ID502_17A:
		if ( m_pArith->l_cmp ( F502_17A, val ) ) {
			F502_17s |= 0x08;
			memmove ( F502_17A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351701" ), F502_17A );
		}
		break;
	case ID502_17B:
		if ( m_pArith->l_cmp ( F502_17B, val ) ) {
			F502_17s |= 0x80;
			memmove ( F502_17B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351702" ), F502_17B );
		}
		break;
	case ID502_17C:
		if ( m_pArith->l_cmp ( F502_17C, val ) ) {
			F502_17s |= 0x800;
			memmove ( F502_17C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351703" ), F502_17C );
		}
		break;
	case ID502_18A:
		if ( m_pArith->l_cmp ( F502_18A, val ) ) {
			F502_18s |= 0x08;
			memmove ( F502_18A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351801" ), F502_18A );
		}
		break;
	case ID502_18B:
		if ( m_pArith->l_cmp ( F502_18B, val ) ) {
			F502_18s |= 0x80;
			memmove ( F502_18B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351802" ), F502_18B );
		}
		break;
	case ID502_18C:
		if ( m_pArith->l_cmp ( F502_18C, val ) ) {
			F502_18s |= 0x800;
			memmove ( F502_18C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351803" ), F502_18C );
		}
		break;
	case ID502_19A:
		if ( m_pArith->l_cmp ( F502_19A, val ) ) {
			F502_19s |= 0x08;
			memmove ( F502_19A, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351901" ), F502_19A );
		}
		break;
	case ID502_19B:
		if ( m_pArith->l_cmp ( F502_19B, val ) ) {
			F502_19s |= 0x80;
			memmove ( F502_19B, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351902" ), F502_19B );
		}
		break;
	case ID502_19C:
		if ( m_pArith->l_cmp ( F502_19C, val ) ) {
			F502_19s |= 0x800;
			memmove ( F502_19C, val, MONY_BUF_SIZE );
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "351903" ), F502_19C );
		}
		break;
	default:
		break;
	}

	return MemoryReget();
}

//=================================
// 集計モジュールからデータ取得
//---------------------------------
// 返送値
//		0		:		正常終了
//		-1		:		エラー
//=================================
int CShinFhyo5_02::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {

		//yoshida191216
		//ロックの時もF502_5kbnとかtypeCntとかセットする。
		char buff2[256] = {0};
		F502_5kbn = 0;

		MoneyBasejagArray money2;
		money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("35F000"));
		m_pArith->l_print(buff2, money2[0][0].arith, _T("SSS"));
		typeCnt = atoi(buff2);
		
		if ((typeCnt == 1) && (F502_5kbn == 0)) {
			if (m_pArith->l_test(F502_7A)) F502_5kbn = 1;
			else if (m_pArith->l_test(F502_7B)) F502_5kbn = 1;
			else if (m_pArith->l_test(F502_7C)) F502_5kbn = 1;

			else if (m_pArith->l_test(F502_8A)) F502_5kbn = 2;
			else if (m_pArith->l_test(F502_8B)) F502_5kbn = 2;
			else if (m_pArith->l_test(F502_8C)) F502_5kbn = 2;

			else if (m_pArith->l_test(F502_9A)) F502_5kbn = 3;
			else if (m_pArith->l_test(F502_9B)) F502_5kbn = 3;
			else if (m_pArith->l_test(F502_9C)) F502_5kbn = 3;

			else if (m_pArith->l_test(F502_10A)) F502_5kbn = 4;
			else if (m_pArith->l_test(F502_10B)) F502_5kbn = 4;
			else if (m_pArith->l_test(F502_10C)) F502_5kbn = 4;

			else if (m_pArith->l_test(F502_11A)) F502_5kbn = 5;
			else if (m_pArith->l_test(F502_11B)) F502_5kbn = 5;
			else if (m_pArith->l_test(F502_11C)) F502_5kbn = 5;

			else if (m_pArith->l_test(F502_12A)) F502_5kbn = 6;
			else if (m_pArith->l_test(F502_12B)) F502_5kbn = 6;
			else if (m_pArith->l_test(F502_12C)) F502_5kbn = 6;

			
			money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("357702"));
			if (m_pArith->l_test(money2[0][0].arith)) F502_5kbn = 1;

			money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("357703"));
			if (m_pArith->l_test(money2[0][0].arith)) F502_5kbn = 1;

			money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("357802"));
			if (m_pArith->l_test(money2[0][0].arith)) F502_5kbn = 2;

			money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("357803"));
			if (m_pArith->l_test(money2[0][0].arith)) F502_5kbn = 2;

			money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("357902"));
			if (m_pArith->l_test(money2[0][0].arith)) F502_5kbn = 3;

			money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("357903"));
			if (m_pArith->l_test(money2[0][0].arith)) F502_5kbn = 3;

			money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("358002"));
			if (m_pArith->l_test(money2[0][0].arith)) F502_5kbn = 4;

			money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("358003"));
			if (m_pArith->l_test(money2[0][0].arith)) F502_5kbn = 4;

			money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("358102"));
			if (m_pArith->l_test(money2[0][0].arith)) F502_5kbn = 5;

			money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("358103"));
			if (m_pArith->l_test(money2[0][0].arith)) F502_5kbn = 5;

			money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("358202"));
			if (m_pArith->l_test(money2[0][0].arith)) F502_5kbn = 6;

			money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("358203"));
			if (m_pArith->l_test(money2[0][0].arith)) F502_5kbn = 6;
		
		}

		return 0;
	}

	// 計算項目をすべて再取得
	MoneyBasejagArray money;
	money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "35" ) );

	int cnt = ( int )money.GetCount();
	int incnt = 0;
	char buff[256] = {0};

	F502_5kbn = 0;

	for ( int l = 0; l < cnt; l++ ) {
		incnt = ( int )money[l].GetCount();
		for ( int m = 0; m < incnt; m++ ) {
			// 業種数
			if ( money[l][m].code == _T("35F000") ) {
				m_pArith->l_print( buff, money[l][m].arith, _T("SSS") );
				typeCnt = atoi( buff );
			}
			else if ( money[l][m].code == _T( "350100" ) ) {
				memmove ( F502_1X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350101" ) ) {
				memmove ( F502_1A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350102" ) ) {
				memmove ( F502_1B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350103" ) ) {
				memmove ( F502_1C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350200" ) ) {
				memmove ( F502_2X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350201" ) ) {
				memmove ( F502_2A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350202" ) ) {
				memmove ( F502_2B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350203" ) ) {
				memmove ( F502_2C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350300" ) ) {
				memmove ( F502_3X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350301" ) ) {
				memmove ( F502_3A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350302" ) ) {
				memmove ( F502_3B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350303" ) ) {
				memmove ( F502_3C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350400" ) ) {
				memmove ( F502_4X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350401" ) ) {
				memmove ( F502_4A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350402" ) ) {
				memmove ( F502_4B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350403" ) ) {
				memmove ( F502_4C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350500" ) ) {
				memmove ( F502_5X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350501" ) ) {
				memmove ( F502_5A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350502" ) ) {
				memmove ( F502_5B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350503" ) ) {
				memmove ( F502_5C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350600" ) ) {
				memmove ( F502_6X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350601" ) ) {
				memmove ( F502_6A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350602" ) ) {
				memmove ( F502_6B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350603" ) ) {
				memmove ( F502_6C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350700" ) ) {
				memmove ( F502_7X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350701" ) ) {
				memmove ( F502_7A, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 1;
				}
			}
			else if ( money[l][m].code == _T( "350702" ) ) {
				memmove ( F502_7B, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 1;
				}
			}
			else if ( money[l][m].code == _T( "350703" ) ) {
				memmove ( F502_7C, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 1;
				}
			}
			else if ( money[l][m].code == _T( "350800" ) ) {
				memmove ( F502_8X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350801" ) ) {
				memmove ( F502_8A, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 2;
				}
			}
			else if ( money[l][m].code == _T( "350802" ) ) {
				memmove ( F502_8B, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 2;
				}
			}
			else if ( money[l][m].code == _T( "350803" ) ) {
				memmove ( F502_8C, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 2;
				}
			}
			else if ( money[l][m].code == _T( "350900" ) ) {
				memmove ( F502_9X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "350901" ) ) {
				memmove ( F502_9A, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 3;
				}
			}
			else if ( money[l][m].code == _T( "350902" ) ) {
				memmove ( F502_9B, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 3;
				}
			}
			else if ( money[l][m].code == _T( "350903" ) ) {
				memmove ( F502_9C, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 3;
				}
			}
			else if ( money[l][m].code == _T( "351000" ) ) {
				memmove ( F502_10X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351001" ) ) {
				memmove ( F502_10A, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 4;
				}
			}
			else if ( money[l][m].code == _T( "351002" ) ) {
				memmove ( F502_10B, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 4;
				}
			}
			else if ( money[l][m].code == _T( "351003" ) ) {
				memmove ( F502_10C, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 4;
				}
			}
			else if ( money[l][m].code == _T( "351100" ) ) {
				memmove ( F502_11X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351101" ) ) {
				memmove ( F502_11A, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 5;
				}
			}
			else if ( money[l][m].code == _T( "351102" ) ) {
				memmove ( F502_11B, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 5;
				}
			}
			else if ( money[l][m].code == _T( "351103" ) ) {
				memmove ( F502_11C, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 5;
				}
			}
			else if ( money[l][m].code == _T( "351200" ) ) {
				memmove ( F502_12X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351201" ) ) {
				memmove ( F502_12A, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 6;
				}
			}
			else if ( money[l][m].code == _T( "351202" ) ) {
				memmove ( F502_12B, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 6;
				}
			}
			else if ( money[l][m].code == _T( "351203" ) ) {
				memmove ( F502_12C, money[l][m].arith, MONY_BUF_SIZE );
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 6;
				}
			}
			else if ( money[l][m].code == _T( "351300" ) ) {
				memmove ( F502_13X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351301" ) ) {
				memmove ( F502_13A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351302" ) ) {
				memmove ( F502_13B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351303" ) ) {
				memmove ( F502_13C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351400" ) ) {
				memmove ( F502_14X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351401" ) ) {
				memmove ( F502_14A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351402" ) ) {
				memmove ( F502_14B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351403" ) ) {
				memmove ( F502_14C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351500" ) ) {
				memmove ( F502_15X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351501" ) ) {
				memmove ( F502_15A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351502" ) ) {
				memmove ( F502_15B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351503" ) ) {
				memmove ( F502_15C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351600" ) ) {
				memmove ( F502_16X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351601" ) ) {
				memmove ( F502_16A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351602" ) ) {
				memmove ( F502_16B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351603" ) ) {
				memmove ( F502_16C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351700" ) ) {
				memmove ( F502_17X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351701" ) ) {
				memmove ( F502_17A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351702" ) ) {
				memmove ( F502_17B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351703" ) ) {
				memmove ( F502_17C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351800" ) ) {
				memmove ( F502_18X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351801" ) ) {
				memmove ( F502_18A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351802" ) ) {
				memmove ( F502_18B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351803" ) ) {
				memmove ( F502_18C, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351900" ) ) {
				memmove ( F502_19X, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351901" ) ) {
				memmove ( F502_19A, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351902" ) ) {
				memmove ( F502_19B, money[l][m].arith, MONY_BUF_SIZE );
			}
			else if ( money[l][m].code == _T( "351903" ) ) {
				memmove ( F502_19C, money[l][m].arith, MONY_BUF_SIZE );
			}

			else if ( money[l][m].code == _T( "357702" ) ) {
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 1;
				}
			}
			else if ( money[l][m].code == _T( "357703" ) ) {
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 1;
				}
			}
			else if ( money[l][m].code == _T( "357802" ) ) {
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 2;
				}
			}
			else if ( money[l][m].code == _T( "357803" ) ) {
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 2;
				}
			}
			else if ( money[l][m].code == _T( "357902" ) ) {
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 3;
				}
			}
			else if ( money[l][m].code == _T( "357903" ) ) {
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 3;
				}
			}
			else if ( money[l][m].code == _T( "358002" ) ) {
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 4;
				}
			}
			else if ( money[l][m].code == _T( "358003" ) ) {
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 4;
				}
			}
			else if ( money[l][m].code == _T( "358102" ) ) {
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 5;
				}
			}
			else if ( money[l][m].code == _T( "358103" ) ) {
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 5;
				}
			}
			else if ( money[l][m].code == _T( "358202" ) ) {
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 6;
				}
			}
			else if ( money[l][m].code == _T( "358203" ) ) {
				if ((typeCnt == 1) && (F502_5kbn == 0)) {
					if (m_pArith->l_test(money[l][m].arith)) F502_5kbn = 6;
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
BOOL CShinFhyo5_02::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( ( F502_6s  & 0x888 ) || ( F502_14s & 0x888 ) || ( F502_15s & 0x888 ) || ( F502_16s & 0x888 ) ||
			 ( F502_17s & 0x888 ) || ( F502_18s & 0x888 ) || ( F502_19s & 0x888 ) ) {
			ret = TRUE;
		}
	}

	return ret;
}

//=========================================================
// 2割特例時に手入力可能な項目　20230616
//---------------------------------------------------------
// 返送値
//	TRUE		:		手入力可能項目
//	FALSE		:		それ以外
//=========================================================
BOOL CShinFhyo5_02::CheckInputIndex2wari( int index )
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		switch( index ){
			case ID502_6A:
			case ID502_6B:
			case ID502_6C:
			case ID502_8A:
			case ID502_8B:
			case ID502_8C:
			case ID502_15A:
			case ID502_15B:
			case ID502_15C:
				ret = TRUE;
				break;
		}
	}

	return ret;
}