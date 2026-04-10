// ShinFhyo2_02.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo2_02.h"
#include "ShinFhyo2_02idx.h"
#include "H31HyoView.h"

#include "ChangeTax.h"
#include "ChangeTaxEx.h"


// CShinFhyo2_02 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo2_02, CSyzBaseDlg)

extern	HINSTANCE g_hInstance;

CShinFhyo2_02::CShinFhyo2_02(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo2_02::IDD, pParent)
	, m_pSnHeadData ( NULL )
	, m_pSnFhyo2_02Data( NULL )
	, m_pShinInfo(NULL)
	, m_pSyzSyukei(NULL)
	, m_pParent(pParent)
	, m_FrSw(0)
	, m_DpSw(0)
	, m_DlSw(0)
	, m_SpcSgn ( FALSE )
{
	memset ( F202_1A, '\0', sizeof ( F202_1A ) );
	memset ( F202_1B, '\0', sizeof ( F202_1B ) );
	memset ( F202_1C, '\0', sizeof ( F202_1C ) );
	memset ( F202_1X, '\0', sizeof ( F202_1X ) );

	memset ( F202_4X, '\0', sizeof ( F202_4X ) );

	memset ( F202_7X, '\0', sizeof ( F202_7X ) );

	//memset ( F202_8X, '\0', sizeof ( F202_8X ) );

	memset ( F202_9A, '\0', sizeof ( F202_9A ) );
	memset ( F202_9B, '\0', sizeof ( F202_9B ) );
	memset ( F202_9C, '\0', sizeof ( F202_9C ) );
	memset ( F202_9X, '\0', sizeof ( F202_9X ) );

	memset ( F202_10A, '\0', sizeof ( F202_10A ) );
	memset ( F202_10B, '\0', sizeof ( F202_10B ) );
	memset ( F202_10C, '\0', sizeof ( F202_10C ) );
	memset ( F202_10X, '\0', sizeof ( F202_10X ) );

	memset ( F202_11C, '\0', sizeof ( F202_11C ) );
	memset ( F202_11X, '\0', sizeof ( F202_11X ) );

	memset ( F202_12C, '\0', sizeof ( F202_12C ) );
	memset ( F202_12X, '\0', sizeof ( F202_12X ) );

//	memset ( F202_13A, '\0', sizeof ( F202_13A ) );
//	memset ( F202_13B, '\0', sizeof ( F202_13B ) );
	memset ( F202_13C, '\0', sizeof ( F202_13C ) );
	memset ( F202_13X, '\0', sizeof ( F202_13X ) );

	memset ( F202_14A, '\0', sizeof ( F202_14A ) );
	memset ( F202_14B, '\0', sizeof ( F202_14B ) );
	memset ( F202_14C, '\0', sizeof ( F202_14C ) );
	memset ( F202_14X, '\0', sizeof ( F202_14X ) );

	memset ( F202_15A, '\0', sizeof ( F202_15A ) );
	memset ( F202_15B, '\0', sizeof ( F202_15B ) );
	memset ( F202_15C, '\0', sizeof ( F202_15C ) );
	memset ( F202_15X, '\0', sizeof ( F202_15X ) );

	memset ( F202_16A, '\0', sizeof ( F202_16A ) );
	memset ( F202_16B, '\0', sizeof ( F202_16B ) );
	memset ( F202_16C, '\0', sizeof ( F202_16C ) );
	memset ( F202_16X, '\0', sizeof ( F202_16X ) );

	memset ( F202_17A, '\0', sizeof ( F202_17A ) );
	memset ( F202_17B, '\0', sizeof ( F202_17B ) );
	memset ( F202_17C, '\0', sizeof ( F202_17C ) );
	memset ( F202_17X, '\0', sizeof ( F202_17X ) );

	memset ( F202_18A, '\0', sizeof ( F202_18A ) );
	memset ( F202_18B, '\0', sizeof ( F202_18B ) );
	memset ( F202_18C, '\0', sizeof ( F202_18C ) );
	memset ( F202_18X, '\0', sizeof ( F202_18X ) );

	memset ( F202_19A, '\0', sizeof ( F202_19A ) );
	memset ( F202_19B, '\0', sizeof ( F202_19B ) );
	memset ( F202_19C, '\0', sizeof ( F202_19C ) );
	memset ( F202_19X, '\0', sizeof ( F202_19X ) );

	memset ( F202_20A, '\0', sizeof ( F202_20A ) );
	memset ( F202_20B, '\0', sizeof ( F202_20B ) );
	memset ( F202_20C, '\0', sizeof ( F202_20C ) );
	memset ( F202_20X, '\0', sizeof ( F202_20X ) );

	memset ( F202_21A, '\0', sizeof ( F202_21A ) );
	memset ( F202_21B, '\0', sizeof ( F202_21B ) );
	memset ( F202_21C, '\0', sizeof ( F202_21C ) );
	memset ( F202_21X, '\0', sizeof ( F202_21X ) );

	memset ( F202_22A, '\0', sizeof ( F202_22A ) );
	memset ( F202_22B, '\0', sizeof ( F202_22B ) );
	memset ( F202_22C, '\0', sizeof ( F202_22C ) );
	memset ( F202_22X, '\0', sizeof ( F202_23X ) );

	memset ( F202_23A, '\0', sizeof ( F202_23A ) );
	memset ( F202_23B, '\0', sizeof ( F202_23B ) );
	memset ( F202_23C, '\0', sizeof ( F202_23C ) );
	memset ( F202_23X, '\0', sizeof ( F202_23X ) );

	memset ( F202_24A, '\0', sizeof ( F202_24A ) );
	memset ( F202_24B, '\0', sizeof ( F202_24B ) );
	memset ( F202_24C, '\0', sizeof ( F202_24C ) );

	memset ( F202_25A, '\0', sizeof ( F202_25A ) );
	memset ( F202_25B, '\0', sizeof ( F202_25B ) );
	memset ( F202_25C, '\0', sizeof ( F202_25C ) );
	memset ( F202_25X, '\0', sizeof ( F202_25X ) );

	memset ( F201_1D, '\0', sizeof ( F201_1D ) );
	memset ( F201_1E, '\0', sizeof ( F201_1E ) );
	memset ( F201_2F, '\0', sizeof ( F201_2F ) );

	F202_8X = 0;
	F202_8EX = 0;

	F202_10c = F202_12c = F202_19c = F202_20c = F202_23c = 0;

	m_curwnd = -1;

}

CShinFhyo2_02::~CShinFhyo2_02()
{
}

void CShinFhyo2_02::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo2_02Diag02);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo2_02Diag01);
	DDX_Control(pDX, IDC_BUTTON1, m_tanaoroshi);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
}


BEGIN_MESSAGE_MAP(CShinFhyo2_02, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CShinFhyo2_02::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinFhyo2_02::OnBnClickedButton2)
END_MESSAGE_MAP()


// CShinFhyo2_02 メッセージ ハンドラ

BOOL CShinFhyo2_02::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL1);
//	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2);
	m_BottomIdx = 276;//コントロール一番最下のDiagのID指定。これで縦スクロール出る

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo2_02::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo2_02::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo2_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
int CShinFhyo2_02::Init()
{
	if ((*m_pSnFhyo2_02Data) == NULL) {
		return -1;
	}

	ReadData(m_FrSw);
	m_FrSw = 1;

	// 特定収入連動メッセージ非表示
	m_Fhyo2_02Diag02.ModifyItem ( ID202_TS, DIAG_MDFY_TRANSPARENT );

	// 読み込みデータを集計モジュールにセット
	ReadData2();

	MemoryReget();

	// ダイアグラム初期属性セット
	InitDiagAttr();

	// 画面表示
	DispFh2_02();
	diag_modify( IDC_ICSDIAG8CTRL1, ID202_8EX, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );
	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		m_Fhyo2_02Diag01.ModifyItem ( 5, DIAG_MDFY_TRANSPARENT );
	}

	ChangeBtnDisp();

	SetItemPosition();

	return 0;
}

//-----------------------------------------------------------------------------
// ボタン表示変更
//-----------------------------------------------------------------------------
void CShinFhyo2_02::ChangeBtnDisp()
{

	if (((*m_pSnHeadData)->m_apno & 0xf0) >= 0x50) {
		if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
			m_tanaoroshi.EnableWindow(FALSE);

			if ( ( *m_pSnHeadData )->Sn_Sign2 & 0x02 ) {
				// 入力ロック時には簡易計算式のボタンを非表示
				m_Button2.EnableWindow( FALSE );
				m_Button2.ShowWindow( SW_HIDE );
			}
		}
		else {
			m_tanaoroshi.EnableWindow(TRUE);
			if (*m_pShinInfo->pSgSpc && !((*m_pSnHeadData)->Sn_Sign2 & 0x02)) {
				m_Button2.ShowWindow( SW_SHOW );	// 「特定収入の簡易計算式の表示」ボタン
			}
			else {
				m_Button2.ShowWindow( SW_HIDE );
			}
		}
	}
	else {
		if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
			m_tanaoroshi.EnableWindow(FALSE);
		}
		else {
			m_tanaoroshi.EnableWindow(TRUE);
		}
		m_Button2.EnableWindow( FALSE );
		m_Button2.ShowWindow( SW_HIDE );
	}

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
int CShinFhyo2_02::InitInfo(CH31SnFhyo2_02Data **pSnFhyo2_02Data, CSnHeadData **pSnHeadData, CH31SnTanaData **pSnTanaData , CArithEx *pArith, SYC_SHININFO *pShinInfo, CSyzSyukei **pSyzSyukei)
{
	ASSERT(pSnFhyo2_02Data);
	if (pSnFhyo2_02Data == NULL) {
		return -1;
	}
	m_pSnFhyo2_02Data = pSnFhyo2_02Data;

	ASSERT(pSnHeadData);
	if (pSnHeadData == NULL) {
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT(pSnTanaData);
	if (pSnTanaData == NULL) {
		return -1;
	}
	m_pSnTanaData = pSnTanaData;


	ASSERT(pArith);
	if (pArith == NULL) {
		return -1;
	}
	m_pArith = pArith;

	ASSERT( pShinInfo );
	if( pShinInfo == NULL ){
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT(pSyzSyukei);
	if (pSyzSyukei == NULL) {
		return -1;
	}
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
int CShinFhyo2_02::InitDiagAttr()
{
	int cnt = 0;

	while ( 1 ) {
		if ( Fhyo2_02[cnt].Index == 0 )
			break;

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo2_02[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Fhyo2_02[cnt].UnConnect;

				int st = ChkTkkzIdx ( Fhyo2_02[cnt].Index );
				if ( st == 1 ) {
					if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
						Flg = 1;
					}
					else {
						Flg = 0;
					}
				}
			}
			else {
				Flg = Fhyo2_02[cnt].Connect;
			}
		}
		if (((*m_pSnHeadData)->Sn_EXP100 & 0x01) && *m_pShinInfo->pSgSpc && (Fhyo2_02[cnt].Index == ID202_8EX)) {
			Flg = 0;
		}

		if ( m_SpcSgn ) {
			if ( ( Fhyo2_02[cnt].Index == ID202_23B ) || ( Fhyo2_02[cnt].Index == ID202_23C ) ) {
				Flg = 0;
			}
		}
		
		ATRIB_MOD ( IDC_ICSDIAG8CTRL1, Fhyo2_02[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo2_02[cnt].Connect );
		cnt++;

	}

	m_Fhyo2_02Diag02.EnableDelete ( 0 );

	return 0;
}

//============================================================
// 特定課税仕入に関連するインデックス
//------------------------------------------------------------
// 引数
//		idx		:	チェック対象
//------------------------------------------------------------
// 返送値
//		0		:	特定課税仕入に関連しない
//		1		:	特定課税仕入で入力可能になるインデックス
//		2		:	特定課税仕入で入力不可になるインデックス
//============================================================
int CShinFhyo2_02::ChkTkkzIdx(short idx)
{
	int rSt = 0;

	if ( ( ID202_11C <= idx ) && ( idx <= ID202_12X ) )
		rSt = 1;

	return rSt;
}

//===================================================
// 初期ポジションセット
//===================================================
void CShinFhyo2_02::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if (m_curwnd == IDC_ICSDIAG8CTRL1) {
		//index = m_Fhyo2_02Diag02.GetPosition();
		if ((index = m_Fhyo2_02Diag02.GetPosition()) != -1) {
			m_Fhyo2_02Diag02.SetPosition(index);
			m_Fhyo2_02Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo2_02Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット
	m_Fhyo2_02Diag02.SetFocus();

	m_Fhyo2_02Diag02.SetPosition(ID202_1A);
	//SetRedrawScroll(0);
}

// データの読込み
int CShinFhyo2_02::ReadData( int pas )
{
	// 1.課税売上額(税抜き)
	memmove(F202_1A, (*m_pSnFhyo2_02Data)->Sn_F202_1A, MONY_BUF_SIZE);
	memmove(F202_1B, (*m_pSnFhyo2_02Data)->Sn_F202_1B, MONY_BUF_SIZE);
	memmove(F202_1C, (*m_pSnFhyo2_02Data)->Sn_F202_1C, MONY_BUF_SIZE);
	memmove(F202_1X, (*m_pSnFhyo2_02Data)->Sn_F202_1X, MONY_BUF_SIZE);

	// 4.課税資産の譲渡等の対価の額
	memmove(F202_4X, (*m_pSnFhyo2_02Data)->Sn_F202_4X, MONY_BUF_SIZE);

	// 7.資産の譲渡等の対価の額
	memmove(F202_7X, (*m_pSnFhyo2_02Data)->Sn_F202_7X, MONY_BUF_SIZE);

	// 8.課税売上割合
	F202_8EX = (unsigned short)(*m_pSnFhyo2_02Data)->Sn_F202_8EX;
	F202_8X = (*m_pSnFhyo2_02Data)->Sn_F202_8X_UW;

	// 9.課税仕入れに係る支払い対価の額(税込み)
	memmove(F202_9A, (*m_pSnFhyo2_02Data)->Sn_F202_9A, MONY_BUF_SIZE);
	memmove(F202_9B, (*m_pSnFhyo2_02Data)->Sn_F202_9B, MONY_BUF_SIZE);
	memmove(F202_9C, (*m_pSnFhyo2_02Data)->Sn_F202_9C, MONY_BUF_SIZE);
	memmove(F202_9X, (*m_pSnFhyo2_02Data)->Sn_F202_9X, MONY_BUF_SIZE);

	// 10.課税仕入れに係る消費税額
	memmove(F202_10A, (*m_pSnFhyo2_02Data)->Sn_F202_10A, MONY_BUF_SIZE);
	memmove(F202_10B, (*m_pSnFhyo2_02Data)->Sn_F202_10B, MONY_BUF_SIZE);
	memmove(F202_10C, (*m_pSnFhyo2_02Data)->Sn_F202_10C, MONY_BUF_SIZE);
	memmove(F202_10X, (*m_pSnFhyo2_02Data)->Sn_F202_10X, MONY_BUF_SIZE);
	F202_10c = (*m_pSnFhyo2_02Data)->Sn_F202_10sw;

	// 11.特定課税仕入れに係る支払対価の額
	if ((*m_pSnHeadData)->IsNeedTkkzDisp()) {
		memmove(F202_11C, (*m_pSnFhyo2_02Data)->Sn_F202_11C, MONY_BUF_SIZE);
		memmove(F202_11X, (*m_pSnFhyo2_02Data)->Sn_F202_11X, MONY_BUF_SIZE);
	}
	else {
		memset(F202_11C, '\0', MONY_BUF_SIZE);
		memset(F202_11X, '\0', MONY_BUF_SIZE);
	}

	// 12.特定課税仕入れに係る消費税額
	if ((*m_pSnHeadData)->IsNeedTkkzDisp()) {
		memmove(F202_12C, (*m_pSnFhyo2_02Data)->Sn_F202_12C, MONY_BUF_SIZE);
		memmove(F202_12X, (*m_pSnFhyo2_02Data)->Sn_F202_12X, MONY_BUF_SIZE);
		F202_12c = (*m_pSnFhyo2_02Data)->Sn_F202_12sw;
	}
	else {
		memset(F202_12C, '\0', MONY_BUF_SIZE);
		memset(F202_12X, '\0', MONY_BUF_SIZE);
		F202_12c = 0;
	}

	// 13.課税貨物に係る消費税額
//	memmove(F202_13A, (*m_pSnFhyo2_02Data)->Sn_F202_13A, MONY_BUF_SIZE);
//	memmove(F202_13B, (*m_pSnFhyo2_02Data)->Sn_F202_13B, MONY_BUF_SIZE);
	memmove(F202_13C, (*m_pSnFhyo2_02Data)->Sn_F202_13C, MONY_BUF_SIZE);
	memmove(F202_13X, (*m_pSnFhyo2_02Data)->Sn_F202_13X, MONY_BUF_SIZE);

	// 14.納税義務の免除を受けない(受ける)こととなった場合の…
	memmove(F202_14A, (*m_pSnFhyo2_02Data)->Sn_F202_14A, MONY_BUF_SIZE);
	memmove(F202_14B, (*m_pSnFhyo2_02Data)->Sn_F202_14B, MONY_BUF_SIZE);
	memmove(F202_14C, (*m_pSnFhyo2_02Data)->Sn_F202_14C, MONY_BUF_SIZE);
	memmove(F202_14X, (*m_pSnFhyo2_02Data)->Sn_F202_14X, MONY_BUF_SIZE);

	// 15.課税仕入れ等の税額の合計額
	memmove(F202_15A, (*m_pSnFhyo2_02Data)->Sn_F202_15A, MONY_BUF_SIZE);
	memmove(F202_15B, (*m_pSnFhyo2_02Data)->Sn_F202_15B, MONY_BUF_SIZE);
	memmove(F202_15C, (*m_pSnFhyo2_02Data)->Sn_F202_15C, MONY_BUF_SIZE);
	memmove(F202_15X, (*m_pSnFhyo2_02Data)->Sn_F202_15X, MONY_BUF_SIZE);

	// 16.課税売上高が5億円以下、課税売上割合が95%以上の場合
	memmove(F202_16A, (*m_pSnFhyo2_02Data)->Sn_F202_16A, MONY_BUF_SIZE);
	memmove(F202_16B, (*m_pSnFhyo2_02Data)->Sn_F202_16B, MONY_BUF_SIZE);
	memmove(F202_16C, (*m_pSnFhyo2_02Data)->Sn_F202_16C, MONY_BUF_SIZE);
	memmove(F202_16X, (*m_pSnFhyo2_02Data)->Sn_F202_16X, MONY_BUF_SIZE);

	// 17.課税売上にのみ要するもの
	memmove(F202_17A, (*m_pSnFhyo2_02Data)->Sn_F202_17A, MONY_BUF_SIZE);
	memmove(F202_17B, (*m_pSnFhyo2_02Data)->Sn_F202_17B, MONY_BUF_SIZE);
	memmove(F202_17C, (*m_pSnFhyo2_02Data)->Sn_F202_17C, MONY_BUF_SIZE);
	memmove(F202_17X, (*m_pSnFhyo2_02Data)->Sn_F202_17X, MONY_BUF_SIZE);

	// 18.課税売上と非課税売上に共通して要するもの
	memmove(F202_18A, (*m_pSnFhyo2_02Data)->Sn_F202_18A, MONY_BUF_SIZE);
	memmove(F202_18B, (*m_pSnFhyo2_02Data)->Sn_F202_18B, MONY_BUF_SIZE);
	memmove(F202_18C, (*m_pSnFhyo2_02Data)->Sn_F202_18C, MONY_BUF_SIZE);
	memmove(F202_18X, (*m_pSnFhyo2_02Data)->Sn_F202_18X, MONY_BUF_SIZE);

	F202_19c = F202_20c = 0x00;
	// 19.個別対応方式により控除する課税仕入れ等の税額
	if(m_pArith->l_test((*m_pSnFhyo2_02Data)->Sn_F202_19A) ) {
		memmove(F202_19A, (*m_pSnFhyo2_02Data)->Sn_F202_19A, MONY_BUF_SIZE);
	}
	if (m_pArith->l_test((*m_pSnFhyo2_02Data)->Sn_F202_19B)) {
		memmove(F202_19B, (*m_pSnFhyo2_02Data)->Sn_F202_19B, MONY_BUF_SIZE);
	}
	if (m_pArith->l_test((*m_pSnFhyo2_02Data)->Sn_F202_19C)) {
		memmove(F202_19C, (*m_pSnFhyo2_02Data)->Sn_F202_19C, MONY_BUF_SIZE);
	}
	if (m_pArith->l_test((*m_pSnFhyo2_02Data)->Sn_F202_19X)) {
		memmove(F202_19X, (*m_pSnFhyo2_02Data)->Sn_F202_19X, MONY_BUF_SIZE);
	}
	F202_19c = (*m_pSnFhyo2_02Data)->Sn_F202_19sw;

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	if (m_pArith->l_test((*m_pSnFhyo2_02Data)->Sn_F202_20A)) {
		memmove(F202_20A, (*m_pSnFhyo2_02Data)->Sn_F202_20A, MONY_BUF_SIZE);
	}
	if (m_pArith->l_test((*m_pSnFhyo2_02Data)->Sn_F202_20B)) {
		memmove(F202_20B, (*m_pSnFhyo2_02Data)->Sn_F202_20B, MONY_BUF_SIZE);
	}
	if (m_pArith->l_test((*m_pSnFhyo2_02Data)->Sn_F202_20C)) {
		memmove(F202_20C, (*m_pSnFhyo2_02Data)->Sn_F202_20C, MONY_BUF_SIZE);
	}
	if (m_pArith->l_test((*m_pSnFhyo2_02Data)->Sn_F202_20X)) {
		memmove(F202_20X, (*m_pSnFhyo2_02Data)->Sn_F202_20X, MONY_BUF_SIZE);
	}
	F202_20c = (*m_pSnFhyo2_02Data)->Sn_F202_20sw;

	// 21.課税売上割合変動時の調整対象固定資産に係る消費税額の調整額
	memmove(F202_21A, (*m_pSnFhyo2_02Data)->Sn_F202_21A, MONY_BUF_SIZE);
	memmove(F202_21B, (*m_pSnFhyo2_02Data)->Sn_F202_21B, MONY_BUF_SIZE);
	memmove(F202_21C, (*m_pSnFhyo2_02Data)->Sn_F202_21C, MONY_BUF_SIZE);
	memmove(F202_21X, (*m_pSnFhyo2_02Data)->Sn_F202_21X, MONY_BUF_SIZE);

	// 22.調整対象固定資産を課税業務用に転用した場合の調整額
	memmove(F202_22A, (*m_pSnFhyo2_02Data)->Sn_F202_22A, MONY_BUF_SIZE);
	memmove(F202_22B, (*m_pSnFhyo2_02Data)->Sn_F202_22B, MONY_BUF_SIZE);
	memmove(F202_22C, (*m_pSnFhyo2_02Data)->Sn_F202_22C, MONY_BUF_SIZE);
	memmove(F202_22X, (*m_pSnFhyo2_02Data)->Sn_F202_22X, MONY_BUF_SIZE);

	// 23.控除対象仕入税額
	F202_23c = (*m_pSnFhyo2_02Data)->Sn_F202_23sw;
	memmove(F202_23A, (*m_pSnFhyo2_02Data)->Sn_F202_23A, MONY_BUF_SIZE);
	memmove(F202_23B, (*m_pSnFhyo2_02Data)->Sn_F202_23B, MONY_BUF_SIZE);
	memmove(F202_23C, (*m_pSnFhyo2_02Data)->Sn_F202_23C, MONY_BUF_SIZE);
	memmove(F202_23X, (*m_pSnFhyo2_02Data)->Sn_F202_23X, MONY_BUF_SIZE);

	// 24.控除過大調整税額
	memmove(F202_24A, (*m_pSnFhyo2_02Data)->Sn_F202_24A, MONY_BUF_SIZE);
	memmove(F202_24B, (*m_pSnFhyo2_02Data)->Sn_F202_24B, MONY_BUF_SIZE);
	memmove(F202_24C, (*m_pSnFhyo2_02Data)->Sn_F202_24C, MONY_BUF_SIZE);
	memmove(F202_24X, (*m_pSnFhyo2_02Data)->Sn_F202_24X, MONY_BUF_SIZE);

	// 25.貸倒回収に係る消費税額
	memmove(F202_25A, (*m_pSnFhyo2_02Data)->Sn_F202_25A, MONY_BUF_SIZE);
	memmove(F202_25B, (*m_pSnFhyo2_02Data)->Sn_F202_25B, MONY_BUF_SIZE);
	memmove(F202_25C, (*m_pSnFhyo2_02Data)->Sn_F202_25C, MONY_BUF_SIZE);
	memmove(F202_25X, (*m_pSnFhyo2_02Data)->Sn_F202_25X, MONY_BUF_SIZE);


	return 0;
}

//-----------------------------------------------------------------------------
// 付表２-０２ 読み込みデータを集計モジュールにセット
//-----------------------------------------------------------------------------
void CShinFhyo2_02::ReadData2()
{
	// 居住用・・・の項目は令和2年4月以降のためリセットしておく
	( *m_pSyzSyukei )->ResetShinkokuData ( _T( "326101" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T( "326102" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T( "326103" ) );

	if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {//非連動の場合
		(*m_pSyzSyukei)->SetShinkokuData(_T("911101"), F202_1A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("911102"), F202_1B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("911103"), F202_1C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("320101"), F202_1A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("320102"), F202_1B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("320103"), F202_1C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("320901"), F202_9A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("320902"), F202_9B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("320903"), F202_9C);
		if( F202_10c&0x08 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("321001"), F202_10A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("321001"));
		if( F202_10c&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("321002"), F202_10B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("321002"));
		if( F202_10c&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("321003"), F202_10C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("321003"));
		(*m_pSyzSyukei)->SetShinkokuData(_T("321103"), F202_11C);
		if( F202_12c&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("321203"), F202_12C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("321203"));

		//--->yoshida190607
//		(*m_pSyzSyukei)->SetShinkokuData(_T("321301"), F202_13A);
//		(*m_pSyzSyukei)->SetShinkokuData(_T("321302"), F202_13B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321303"), F202_13C);

		(*m_pSyzSyukei)->SetShinkokuData(_T("321401"), F202_14A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321402"), F202_14B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321403"), F202_14C);

		(*m_pSyzSyukei)->SetShinkokuData(_T("321701"), F202_17A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321702"), F202_17B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321703"), F202_17C);

		(*m_pSyzSyukei)->SetShinkokuData(_T("321801"), F202_18A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321802"), F202_18B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321803"), F202_18C);

		if (F202_19c & 0x08)
			(*m_pSyzSyukei)->SetShinkokuData(_T("321901"), F202_19A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("321901"));

		if (F202_19c & 0x80)
			(*m_pSyzSyukei)->SetShinkokuData(_T("321902"), F202_19B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("321902"));

		if (F202_19c & 0x800)
			(*m_pSyzSyukei)->SetShinkokuData(_T("321903"), F202_19C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("321903"));

		if (F202_20c & 0x08)
			(*m_pSyzSyukei)->SetShinkokuData(_T("322001"), F202_20A);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322001"));

		if (F202_20c & 0x80)
			(*m_pSyzSyukei)->SetShinkokuData(_T("322002"), F202_20B);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322002"));

		if (F202_20c & 0x800)
			(*m_pSyzSyukei)->SetShinkokuData(_T("322003"), F202_20C);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322003"));
		//<-------------------

		(*m_pSyzSyukei)->SetShinkokuData(_T("322101"), F202_21A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322102"), F202_21B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322103"), F202_21C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322201"), F202_22A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322202"), F202_22B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322203"), F202_22C);

		// 申告区分を切り替えた時等に付表2-2の控除対象仕入税額に対して0をセットしているため、
		// 手入力サインが立っていない時はリセットする
		if( F202_23c&0x08 ) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("322301"), F202_23A);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322301"));
		}

		if( F202_23c&0x80 ) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("322302"), F202_23B);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322302"));
		}

		if( F202_23c&0x800 ) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("322303"), F202_23C);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322303"));
		}

		if( F202_23c&0x08 ) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("322401"), F202_24A);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322401"));
		}

		if( F202_23c&0x80 ) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("322402"), F202_24B);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322402"));
		}

		if( F202_23c&0x800 ) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("322403"), F202_24C);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322403"));
		}

		(*m_pSyzSyukei)->SetShinkokuData(_T("322501"), F202_25A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322502"), F202_25B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322503"), F202_25C);
	}
	else {
		/*(*m_pSyzSyukei)->SetShinkokuData(_T("321401"), F202_14A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321402"), F202_14B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321403"), F202_14C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321701"), F202_17A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321702"), F202_17B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321703"), F202_17C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321801"), F202_18A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321802"), F202_18B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321803"), F202_18C);*/
		(*m_pSyzSyukei)->SetShinkokuData(_T("322101"), F202_21A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322102"), F202_21B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322103"), F202_21C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322201"), F202_22A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322202"), F202_22B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322203"), F202_22C);
	}
}

// データの書込み
int CShinFhyo2_02::WriteData( int pas )
{
	int st = 0;

	// 1.課税売上額(税抜き)
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_1A, F202_1A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_1B, F202_1B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_1C, F202_1C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_1X, F202_1X, MONY_BUF_SIZE);

	// 4.課税資産の譲渡等の対価の額
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_4X, F202_4X, MONY_BUF_SIZE);

	// 7.課税資産の譲渡等の対価の額
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_7X, F202_7X, MONY_BUF_SIZE);

	// 8.課税売上割合
	(unsigned short)(*m_pSnFhyo2_02Data)->Sn_F202_8EX = F202_8EX;
	(*m_pSnHeadData)->Sn_UPERS &= 0xfe;
	if (((*m_pSnFhyo2_02Data)->Sn_F202_8EX) < (*m_pSnHeadData)->m_UriRatioBorder)
		(*m_pSnHeadData)->Sn_UPERS |= 0x01;

	// 9.課税仕入れに係る支払い対価の額(税込み)
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_9A, F202_9A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_9B, F202_9B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_9C, F202_9C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_9X, F202_9X, MONY_BUF_SIZE);

	// 10.課税仕入れに係る消費税額
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_10A, F202_10A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_10B, F202_10B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_10C, F202_10C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_10X, F202_10X, MONY_BUF_SIZE);
	(*m_pSnFhyo2_02Data)->Sn_F202_10sw = F202_10c;

	// 11.特定課税仕入れに係る支払対価の額
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_11C, F202_11C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_11X, F202_11X, MONY_BUF_SIZE);

	// 12.特定課税仕入れに係る消費税額
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_12C, F202_12C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_12X, F202_12X, MONY_BUF_SIZE);
	(*m_pSnFhyo2_02Data)->Sn_F202_12sw = F202_12c;

	// 13.課税貨物に係る消費税額
//	memmove((*m_pSnFhyo2_02Data)->Sn_F202_13A, F202_13A, MONY_BUF_SIZE);
//	memmove((*m_pSnFhyo2_02Data)->Sn_F202_13B, F202_13B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_13C, F202_13C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_13X, F202_13X, MONY_BUF_SIZE);

	// 14.納税義務の免除を受けない(受ける)こととなった場合の…
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_14A, F202_14A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_14B, F202_14B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_14C, F202_14C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_14X, F202_14X, MONY_BUF_SIZE);

	// 15.課税仕入れ等の税額の合計額
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_15A, F202_15A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_15B, F202_15B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_15C, F202_15C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_15X, F202_15X, MONY_BUF_SIZE);

	// 16.課税売上高が5億円以下、課税売上割合が95%以上の場合
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_16A, F202_16A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_16B, F202_16B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_16C, F202_16C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_16X, F202_16X, MONY_BUF_SIZE);

	// 17.課税売上にのみ要するもの
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_17A, F202_17A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_17B, F202_17B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_17C, F202_17C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_17X, F202_17X, MONY_BUF_SIZE);

	// 18.課税売上と非課税売上に共通して要するもの
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_18A, F202_18A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_18B, F202_18B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_18C, F202_18C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_18X, F202_18X, MONY_BUF_SIZE);

	// 19.個別対応方式により控除する課税仕入れ等の税額
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_19X, F202_19X, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_19A, F202_19A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_19B, F202_19B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_19C, F202_19C, MONY_BUF_SIZE);
	(*m_pSnFhyo2_02Data)->Sn_F202_19sw = F202_19c;

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_20X, F202_20X, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_20A, F202_20A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_20B, F202_20B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_20C, F202_20C, MONY_BUF_SIZE);
	(*m_pSnFhyo2_02Data)->Sn_F202_20sw = F202_20c;

	// 21.課税売上割合変動時の調整対象固定資産に係る消費税額の調整額
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_21A, F202_21A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_21B, F202_21B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_21C, F202_21C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_21X, F202_21X, MONY_BUF_SIZE);

	// 22.調整対象固定資産を課税業務用に転用した場合の調整額
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_22A, F202_22A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_22B, F202_22B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_22C, F202_22C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_22X, F202_22X, MONY_BUF_SIZE);

	// 23.控除対象仕入税額
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_23A, F202_23A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_23B, F202_23B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_23C, F202_23C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_23X, F202_23X, MONY_BUF_SIZE);
	(*m_pSnFhyo2_02Data)->Sn_F202_23sw = F202_23c;

	// 24.控除過大調整税額
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_24A, F202_24A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_24B, F202_24B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_24C, F202_24C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_24X, F202_24X, MONY_BUF_SIZE);

	// 25.貸倒回収に係る消費税額
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_25A, F202_25A, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_25B, F202_25B, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_25C, F202_25C, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo2_02Data)->Sn_F202_25X, F202_25X, MONY_BUF_SIZE);

	memset ( ( *m_pSnFhyo2_02Data )->Sn_F202_26A, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo2_02Data )->Sn_F202_26B, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo2_02Data )->Sn_F202_26C, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo2_02Data )->Sn_F202_26X, '\0', MONY_BUF_SIZE );

	return st;
}


//-----------------------------------------------------------------------------
// 付表２-０２ 画面出力
//-----------------------------------------------------------------------------
void CShinFhyo2_02::DispFh2_02()
{

	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 画面の初期化
	for( int i=4; i<=6; i++ ){
		dsp_cls( IDC_ICSDIAG8CTRL1, (short )i );
	}
	for( int i=ID202_1A; i<=ID202_25X; i++ ){
		dsp_cls( IDC_ICSDIAG8CTRL1, (short )i );
	}


	if (!((*m_pSnHeadData)->Sn_Sign4 & 0x80)) {//連動の場合手入力サイン初期化
		//手入力サイン初期化
		F202_10c = 0;
		F202_19c = 0;
		F202_20c = 0;
		//if( ((CH26HyoView*)m_pParent)->IsSpcDataLinked() == FALSE ){
		//	F2223c = 0;
		//}
		F202_23c = 0;
	}

	
	// 期間の表示
	char	buf[512] = {0};
	char	yy, mm, dd;
	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s( buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy&0xff),(mm&0xff),(dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAG8CTRL1, 4, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	sprintf_s( buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy&0xff), (mm&0xff), (dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAG8CTRL1, 5, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 氏名の表示
	memset( buf, '\0', sizeof(buf) );
	if( (*m_pSnHeadData)->IsSoleProprietor() ){
		memmove( buf, (*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO) );
	}
	else{
		memmove( buf, (*m_pSnHeadData)->Sn_CONAM, sizeof((*m_pSnHeadData)->Sn_CONAM) );
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAG8CTRL1, 6, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	
	// 課税売上高の5億円超えチェック
	ChangeColor( IDC_ICSDIAG8CTRL1, MILLION, 1 );
	ChangeColor( IDC_ICSDIAG8CTRL1, MILLION + 1, 1 );
	diag_clear( IDC_ICSDIAG8CTRL2, 4, TRUE, CIcsdiagctrl );
	CheckOver500Million();

	// 課税売上額
	f202_1data();
	// 課税資産の譲渡等の対価の額
	f202_45data();
	// 資産の譲渡等の対価の額
	f202_7data();
	//課税売上割合
	f202_8data();
	// 課税仕入に係る支払対価の額
	f202_9data();
	// 課税仕入に係る消費税額
	f202_10data();
	//特定課税仕入れに係る支払対価の額
	f202_11data();
	//特定課税仕入れに係る消費税額
	f202_12data();
	// 課税貨物に係る消費税額
	f202_13data();
	// 課税事業⇔免税事業 調整税額
	f202_14data();
	// 課税仕入等の税額の合計額
	f202_15data();
	if ((*m_pSnHeadData)->IsUseSemiUriRatio()) {
		// 課税売上割合が９５％未満の場合
		f202_ko();
	}
	else {
		// 課税売上割合が９５％以上の場合
		if (F202_8EX >= (*m_pSnHeadData)->m_UriRatioBorder) {
			f202_16data();
		}
		else {
			// 課税売上割合が９５％未満の場合
			f202_ko();
		}

	}

	// 控除税額の調整(21)
	f202_21data();
	// 控除税額の調整(22)
	f202_22data();
	//	差引控除仕入税額の計算
	f202_2324data();
	// 回収に係る消費税額
	f202_25data();

	//m_Fhyo2_02Diag02.Refresh();

}

//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//		Mony	：	表示金額
//-----------------------------------------------------------------------------
void CShinFhyo2_02::dsp_prs( unsigned short Id, short Index, char *Mony )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
	diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
}

//-----------------------------------------------------------------------------
// 金額表示クリア
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//-----------------------------------------------------------------------------
void CShinFhyo2_02::dsp_cls( unsigned short Id, short Index )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch( Index ) {
		case 4 :	// 課税期間　自
		case 5 :	// 課税期間　至
		case 6 :	// 氏名又は､名称
		case ID202_8X:
			diag_clear( Id, Index, TRUE, CIcsdiagctrl );
			break;
		default :	// 金額
			memset( DiagData.data_val, '\0', MONY_BUF_SIZE );
			diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			break;
	}
}


// 各入力データ抽出
void CShinFhyo2_02::GetAllDiagData()
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// 1.課税売上額(税抜き)
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_1A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_1B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_1C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_1C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_1X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_1X, DiagData.data_val, MONY_BUF_SIZE);


	// 4.課税資産の譲渡等の対価の額
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_4X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_4X, DiagData.data_val, MONY_BUF_SIZE);

	// 7.資産譲渡等の対価の額
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_7X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_7X, DiagData.data_val, MONY_BUF_SIZE);

	// 8.課税売上割合

	// 9.課税仕入れに係る支払い対価の額(税込み)
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_9A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_9A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_9B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_9B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_9C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_9C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_9X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_9X, DiagData.data_val, MONY_BUF_SIZE);

	// 10.課税仕入れに係る消費税額
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_10A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_10A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_10B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_10B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_10C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_10C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_10X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_10X, DiagData.data_val, MONY_BUF_SIZE);

	// 11.特定課税仕入れに係る支払対価の額
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_11C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_11C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_11X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_11X, DiagData.data_val, MONY_BUF_SIZE);

	// 12.特定課税仕入れに係る消費税額
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_12C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_12C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_12X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_12X, DiagData.data_val, MONY_BUF_SIZE);

	// 13.課税貨物に係る消費税額
//	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_13A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
//	memmove(F202_13A, DiagData.data_val, MONY_BUF_SIZE);
//	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_13B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
//	memmove(F202_13B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_13C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_13C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_13X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_13X, DiagData.data_val, MONY_BUF_SIZE);

	// 14.納税義務の免除を受ける(受けない)こととなった場合における消費税額の調整額
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_14A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_14A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_14B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_14B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_14C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_14C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_14X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_14X, DiagData.data_val, MONY_BUF_SIZE);

	// 15.課税仕入れ等の税額の合計額
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_15A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_15A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_15B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_15B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_15C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_15C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_15X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_15X, DiagData.data_val, MONY_BUF_SIZE);

	// 16.課税売上高が5億円以下、割合が95％以上の場合
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_16A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_16A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_16B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_16B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_16C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_16C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_16X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_16X, DiagData.data_val, MONY_BUF_SIZE);

	// 17.15のうち、課税売上げにのみ要するもの
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_17A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_17A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_17B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_17B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_17C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_17C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_17X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_17X, DiagData.data_val, MONY_BUF_SIZE);

	// 18.15のうち、共通して要するもの
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_18A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_18A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_18B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_18B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_18C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_18C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_18X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_18X, DiagData.data_val, MONY_BUF_SIZE);

	// 19.個別対応方式により控除する課税仕入れ等の税額
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_19A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_19A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_19B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_19B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_19C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_19C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_19X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_19X, DiagData.data_val, MONY_BUF_SIZE);

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_20A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_20A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_20B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_20B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_20C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_20C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_20X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_20X, DiagData.data_val, MONY_BUF_SIZE);

	// 21.課税売上割合変動時の…消費税額の調整額
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_21A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_21A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_21B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_21B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_21C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_21C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_21X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_21X, DiagData.data_val, MONY_BUF_SIZE);

	// 22.調整対象固定資産を…調整額
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_22A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_22A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_22B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_22B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_22C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_22C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_22X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_22X, DiagData.data_val, MONY_BUF_SIZE);

	// 23.控除対象仕入税額がプラスの時
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_23A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_23A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_23B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_23B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_23C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_23C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_23X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_23X, DiagData.data_val, MONY_BUF_SIZE);

	// 24.控除過大調整額がマイナスの時
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_24A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_24A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_24B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_24B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_24C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_24C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_24X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_24X, DiagData.data_val, MONY_BUF_SIZE);

	// 25.貸倒回収に係る消費税額
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_25A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_25A, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_25B, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_25B, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_25C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_25C, DiagData.data_val, MONY_BUF_SIZE);
	diag_getdata(IDC_ICSDIAG8CTRL1, ID202_25X, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(F202_25X, DiagData.data_val, MONY_BUF_SIZE);

}

// 課税売上額
void CShinFhyo2_02::f202_1data()
{

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_1A, F202_1A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_1B, F202_1B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_1C, F202_1C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_1X, F202_1X);

}

// 課税資産の譲渡等の対価の額
void CShinFhyo2_02::f202_45data()
{

	if (m_pArith->l_test(F202_4X) < 0) {
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_4X, FZERO);
	}
	else {
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_4X, F202_4X);
	}



}

// 非課税売上額
void CShinFhyo2_02::f202_6data()
{
	//dsp_prs(IDC_ICSDIAG8CTRL1, ID202_6F, F202_6F);

}

// 資産の譲渡等の対価の額
void CShinFhyo2_02::f202_7data()
{
	char	WORK0[MONY_BUF_SIZE] = { 0 };

	if (m_pArith->l_test(F202_7X) < 0) {
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_7X, FZERO);
	}
	else {
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_7X, F202_7X);
	}


}

//課税売上割合
void CShinFhyo2_02::f202_8data()
{

	char	WORK0[MONY_BUF_SIZE] = { 0 };
	char	PW6[MONY_BUF_SIZE] = { 0 };
	char	buf[512] = { 0 };
	char	PER100BUF[MONY_BUF_SIZE] = { 0 };
	m_pArith->l_input( PER100BUF, _T("10000") );

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 付表２－１からの転記のみとする
		if( !m_DpSw ){	// 再集計のみ（表示無し）
			// 課税売上高割合
			DiagData.data_check = 0;
			diag_setdata( IDC_ICSDIAG8CTRL1, ID202_8EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			ChangeColor( IDC_ICSDIAG8CTRL1, ID202_8X, 1 );
			diag_modify( IDC_ICSDIAG8CTRL1, ID202_8X, DIAG_MDFY_READONLY, CIcsdiagctrl );
			memset( buf, '\0', sizeof( buf ) );
			memset( WORK0, '\0', MONY_BUF_SIZE );
			if( !((*m_pSnHeadData)->Sn_EXP100&0x01) ){
				if( (*m_pSnHeadData)->Sn_UPERS&0x10 ){
					// 課税売上割合　手入力
					if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
						memset( buf, '\0', sizeof(buf) );
						strcpy_s( buf, sizeof(buf), _T("課　税　売　上　割　合　　(4/7)\n【課税売上割合に準ずる割合を適用】") );
						DiagData.data_disp = buf;
						diag_setdata( IDC_ICSDIAG8CTRL1, 24, (struct IUnknown *)&DiagData, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAG8CTRL2, ID202_8X, 1 );
					}
					else{
						memset( buf, '\0', sizeof(buf) );
						strcpy_s( buf, sizeof(buf), _T("課　税　売　上　割　合　　(4/7)") );
						DiagData.data_disp = buf;
						diag_setdata( IDC_ICSDIAG8CTRL1, 24, (struct IUnknown *)&DiagData, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAG8CTRL1, ID202_8X, 1 );
					}

					if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){
						ChangeColor( IDC_ICSDIAG8CTRL1, ID202_8X, 1 );
					}
					else{
						ChangeColor( IDC_ICSDIAG8CTRL1, ID202_8X, 1 );
					}
				}
				else{
					strcpy_s( buf, sizeof(buf), _T("課　税　売　上　割　合　　(4/7)") );
					DiagData.data_disp = buf;
					diag_setdata( IDC_ICSDIAG8CTRL1, 24, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAG8CTRL1, ID202_8X, 1 );
				}
			}
			wsprintf(buf, "%d.%02d", F202_8EX/100, F202_8EX%100);
			DiagData.data_edit = buf;
			diag_setdata( IDC_ICSDIAG8CTRL1, ID202_8X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
}

// 課税仕入に係る支払対価の額
void CShinFhyo2_02::f202_9data()
{

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_9A, F202_9A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_9B, F202_9B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_9C, F202_9C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_9X, F202_9X);

}

// 課税仕入に係る消費税額
void CShinFhyo2_02::f202_10data()
{

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_10A, F202_10A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_10B, F202_10B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_10C, F202_10C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_10X, F202_10X);
	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_10A, (F202_10c & 0x08) ? 3 : 0);
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_10B, (F202_10c & 0x80) ? 3 : 0);
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_10C, (F202_10c & 0x800) ? 3 : 0);
	}
}


//特定課税仕入れに係る支払対価の額
void CShinFhyo2_02::f202_11data()
{

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_11C, F202_11C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_11X, F202_11X);

}

//特定課税仕入れに係る消費税額
void CShinFhyo2_02::f202_12data()
{
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_12C, F202_12C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_12X, F202_12X);

	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
		if( (*m_pSnHeadData)->IsNeedTkkzDisp() )
			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_12C, (F202_12c & 0x800) ? 3 : 0);
		else
			ChangeColor( IDC_ICSDIAG8CTRL1, ID202_12C, 1 );
	}
}

// 課税貨物に係る消費税額
void CShinFhyo2_02::f202_13data()
{
//	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_13A, F202_13A);
//	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_13B, F202_13B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_13C, F202_13C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_13X, F202_13X);
}

// 課税事業⇔免税事業 調整税額
void CShinFhyo2_02::f202_14data()
{

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_14A, F202_14A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_14B, F202_14B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_14C, F202_14C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_14X, F202_14X);

}

// 課税仕入等の税額の合計額
void CShinFhyo2_02::f202_15data()
{

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_15A, F202_15A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_15B, F202_15B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_15C, F202_15C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_15X, F202_15X);

}

// 課税売上割合が９５％以上の場合
void CShinFhyo2_02::f202_16data()
{
	char	WORK0[MONY_BUF_SIZE] = { 0 };

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_16A, F202_16A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_16B, F202_16B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_16C, F202_16C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_16X, F202_16X);

	memset(F202_17A, '\0', MONY_BUF_SIZE);
	memset(F202_17B, '\0', MONY_BUF_SIZE);
	memset(F202_17C, '\0', MONY_BUF_SIZE);
	memset(F202_17X, '\0', MONY_BUF_SIZE);

	memset(F202_18A, '\0', MONY_BUF_SIZE);
	memset(F202_18B, '\0', MONY_BUF_SIZE);
	memset(F202_18C, '\0', MONY_BUF_SIZE);
	memset(F202_18X, '\0', MONY_BUF_SIZE);

	memset(F202_19A, '\0', MONY_BUF_SIZE);
	memset(F202_19B, '\0', MONY_BUF_SIZE);
	memset(F202_19C, '\0', MONY_BUF_SIZE);
	memset(F202_19X, '\0', MONY_BUF_SIZE);

	memset(F202_20A, '\0', MONY_BUF_SIZE);
	memset(F202_20B, '\0', MONY_BUF_SIZE);
	memset(F202_20C, '\0', MONY_BUF_SIZE);
	memset(F202_20X, '\0', MONY_BUF_SIZE);
	F202_20c = 0;

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_17A, F202_17A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_17B, F202_17B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_17C, F202_17C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_17X, F202_17X);

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_18A, F202_18A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_18B, F202_18B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_18C, F202_18C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_18X, F202_18X);

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_19A, F202_19A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_19B, F202_19B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_19C, F202_19C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_19X, F202_19X);

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_20A, F202_20A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_20B, F202_20B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_20C, F202_20C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_20X, F202_20X);

	// 該当項目入力不可
	// 課税売上割合が95％以上

	ChangeColor(IDC_ICSDIAG8CTRL1, ID202_17A, 1);
	ChangeColor(IDC_ICSDIAG8CTRL1, ID202_17B, 1);
	ChangeColor(IDC_ICSDIAG8CTRL1, ID202_17C, 1);

	ChangeColor(IDC_ICSDIAG8CTRL1, ID202_18A, 1);
	ChangeColor(IDC_ICSDIAG8CTRL1, ID202_18B, 1);
	ChangeColor(IDC_ICSDIAG8CTRL1, ID202_18C, 1);

	ChangeColor(IDC_ICSDIAG8CTRL1, ID202_19A, 1);
	ChangeColor(IDC_ICSDIAG8CTRL1, ID202_19B, 1);
	ChangeColor(IDC_ICSDIAG8CTRL1, ID202_19C, 1);

	ChangeColor(IDC_ICSDIAG8CTRL1, ID202_20A, 1);
	ChangeColor(IDC_ICSDIAG8CTRL1, ID202_20B, 1);
	ChangeColor(IDC_ICSDIAG8CTRL1, ID202_20C, 1);

	ModifyDiagInput(ID202_17A, DIAG_MDFY_READONLY);
	ModifyDiagInput(ID202_17B, DIAG_MDFY_READONLY);
	ModifyDiagInput(ID202_17C, DIAG_MDFY_READONLY);

	ModifyDiagInput(ID202_18A, DIAG_MDFY_READONLY);
	ModifyDiagInput(ID202_18B, DIAG_MDFY_READONLY);
	ModifyDiagInput(ID202_18C, DIAG_MDFY_READONLY);

	ModifyDiagInput(ID202_19A, DIAG_MDFY_READONLY);
	ModifyDiagInput(ID202_19B, DIAG_MDFY_READONLY);
	ModifyDiagInput(ID202_19C, DIAG_MDFY_READONLY);

	ModifyDiagInput(ID202_20A, DIAG_MDFY_READONLY);
	ModifyDiagInput(ID202_20B, DIAG_MDFY_READONLY);
	ModifyDiagInput(ID202_20C, DIAG_MDFY_READONLY);

}

// 課税売上にのみ要する仕入税額
void CShinFhyo2_02::f202_17data()
{

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_17A, F202_17A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_17B, F202_17B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_17C, F202_17C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_17X, F202_17X);
}

// 共通の売上に要する仕入税額
void CShinFhyo2_02::f202_18data()
{

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_18A, F202_18A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_18B, F202_18B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_18C, F202_18C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_18X, F202_18X);
}

// 個別対応方式控除仕入税額
void CShinFhyo2_02::f202_19data()
{

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_19A, F202_19A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_19B, F202_19B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_19C, F202_19C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_19X, F202_19X);

}

// 比例配分方式控除仕入税額
void CShinFhyo2_02::f202_20data()
{

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_20A, F202_20A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_20B, F202_20B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_20C, F202_20C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_20X, F202_20X);


}

// 課税売上割合が９５％未満の場合
void CShinFhyo2_02::f202_ko()
{
	if ((*m_pSnHeadData)->IsKobetuSiireAnbun()) {
		dsp_cls(IDC_ICSDIAG8CTRL1, ID202_16A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID202_16B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID202_16C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID202_16X);

		f202_17data();
		f202_18data();
		f202_19data();

		memset(F202_20A, '\0', MONY_BUF_SIZE);
		memset(F202_20B, '\0', MONY_BUF_SIZE);
		memset(F202_20C, '\0', MONY_BUF_SIZE);
		memset(F202_20X, '\0', MONY_BUF_SIZE);
		F202_20c = 0;

		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_20A, F202_20A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_20B, F202_20B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_20C, F202_20C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_20X, F202_20X);

		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ModifyDiagInput(ID202_17A, DIAG_MDFY_EDIT);
			ModifyDiagInput(ID202_17B, DIAG_MDFY_EDIT);
			ModifyDiagInput(ID202_17C, DIAG_MDFY_EDIT);
			ModifyDiagInput(ID202_18A, DIAG_MDFY_EDIT);
			ModifyDiagInput(ID202_18B, DIAG_MDFY_EDIT);
			ModifyDiagInput(ID202_18C, DIAG_MDFY_EDIT);
			ModifyDiagInput(ID202_19A, DIAG_MDFY_EDIT);
			ModifyDiagInput(ID202_19B, DIAG_MDFY_EDIT);
			ModifyDiagInput(ID202_19C, DIAG_MDFY_EDIT);

			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_17A, 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_17B, 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_17C, 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_18A, 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_18B, 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_18C, 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_19A, 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_19B, 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_19C, 0);


			if (F202_19c & 0x08) {//手入力
				ChangeColor(IDC_ICSDIAG8CTRL1, ID202_19A, 3);
			}
			if (F202_19c & 0x80) {//手入力
				ChangeColor(IDC_ICSDIAG8CTRL1, ID202_19B, 3);
			}
			if (F202_19c & 0x800) {//手入力
				ChangeColor(IDC_ICSDIAG8CTRL1, ID202_19C, 3);
			}

		}
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_20A, 1);
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_20B, 1);
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_20C, 1);

		ModifyDiagInput(ID202_20A, DIAG_MDFY_READONLY);
		ModifyDiagInput(ID202_20B, DIAG_MDFY_READONLY);
		ModifyDiagInput(ID202_20C, DIAG_MDFY_READONLY);
	}
	else {	// 比例配分

		dsp_cls(IDC_ICSDIAG8CTRL1, ID202_16A);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID202_16B);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID202_16C);
		dsp_cls(IDC_ICSDIAG8CTRL1, ID202_16X);

		f202_20data();

		memset(F202_17A, '\0', MONY_BUF_SIZE);
		memset(F202_17B, '\0', MONY_BUF_SIZE);
		memset(F202_17C, '\0', MONY_BUF_SIZE);
		memset(F202_17X, '\0', MONY_BUF_SIZE);
		memset(F202_18A, '\0', MONY_BUF_SIZE);
		memset(F202_18B, '\0', MONY_BUF_SIZE);
		memset(F202_18C, '\0', MONY_BUF_SIZE);
		memset(F202_18X, '\0', MONY_BUF_SIZE);
		memset(F202_19A, '\0', MONY_BUF_SIZE);
		memset(F202_19B, '\0', MONY_BUF_SIZE);
		memset(F202_19C, '\0', MONY_BUF_SIZE);
		memset(F202_19X, '\0', MONY_BUF_SIZE);
		F202_19c = 0;

		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_17A, F202_17A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_17B, F202_17B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_17C, F202_17C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_17X, F202_17X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_18A, F202_18A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_18B, F202_18B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_18C, F202_18C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_18X, F202_18X);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_19A, F202_19A);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_19B, F202_19B);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_19C, F202_19C);
		dsp_prs(IDC_ICSDIAG8CTRL1, ID202_19X, F202_19X);

		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_17A, 1);
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_17B, 1);
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_17C, 1);
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_18A, 1);
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_18B, 1);
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_18C, 1);
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_19A, 1);
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_19B, 1);
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_19C, 1);

		ModifyDiagInput(ID202_17A, DIAG_MDFY_READONLY);
		ModifyDiagInput(ID202_17B, DIAG_MDFY_READONLY);
		ModifyDiagInput(ID202_17C, DIAG_MDFY_READONLY);
		ModifyDiagInput(ID202_18A, DIAG_MDFY_READONLY);
		ModifyDiagInput(ID202_18B, DIAG_MDFY_READONLY);
		ModifyDiagInput(ID202_18C, DIAG_MDFY_READONLY);
		ModifyDiagInput(ID202_19A, DIAG_MDFY_READONLY);
		ModifyDiagInput(ID202_19B, DIAG_MDFY_READONLY);
		ModifyDiagInput(ID202_19C, DIAG_MDFY_READONLY);

		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ModifyDiagInput(ID202_20A, DIAG_MDFY_EDIT);
			ModifyDiagInput(ID202_20B, DIAG_MDFY_EDIT);
			ModifyDiagInput(ID202_20C, DIAG_MDFY_EDIT);

			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_20A, 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_20B, 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_20C, 0);


			if (F202_20c & 0x08) {//手入力
				ChangeColor(IDC_ICSDIAG8CTRL1, ID202_20A, 3);
			}
			if (F202_20c & 0x80) {//手入力
				ChangeColor(IDC_ICSDIAG8CTRL1, ID202_20B, 3);
			}
			if (F202_20c & 0x800) {//手入力
				ChangeColor(IDC_ICSDIAG8CTRL1, ID202_20C, 3);
			}

		}

	}


}

// 控除税額の調整(21)
void CShinFhyo2_02::f202_21data()
{

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_21A, F202_21A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_21B, F202_21B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_21C, F202_21C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_21X, F202_21X);
}

// 控除税額の調整(22)
void CShinFhyo2_02::f202_22data()
{

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_22A, F202_22A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_22B, F202_22B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_22C, F202_22C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_22X, F202_22X);

}

// 差引控除仕入税額の計算
void CShinFhyo2_02::f202_2324data()
{
	//if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) && ( m_SpcSgn == FALSE )) {
	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
		ChangeColor(IDC_ICSDIAG8CTRL1, ID202_23A, (F202_23c & 0x08) ? 3 : 0);
		if( m_SpcSgn == FALSE ) {
			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_23B, (F202_23c & 0x80) ? 3 : 0);
			ChangeColor(IDC_ICSDIAG8CTRL1, ID202_23C, (F202_23c & 0x800) ? 3 : 0);
		}
	}

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_23A, F202_23A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_23B, F202_23B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_23C, F202_23C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_23X, F202_23X);


	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_24A, F202_24A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_24B, F202_24B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_24C, F202_24C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_24X, F202_24X);
}

// 回収に係る消費税額
void CShinFhyo2_02::f202_25data()
{

	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_25A, F202_25A);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_25B, F202_25B);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_25C, F202_25C);
	dsp_prs(IDC_ICSDIAG8CTRL1, ID202_25X, F202_25X);
}

// 他表からの計算
void CShinFhyo2_02::calq_by_OtherChange()
{
	m_DpSw = -1;

	
	(1);
	f202data_all(1);
	WriteData(1);

	m_DpSw = 0;
}

//-------------------------------------------------
// 各入力のデータ表示
//-------------------------------------------------
// 引数	atno	：	0：全て
//		sw		：	
//-------------------------------------------------
void CShinFhyo2_02::f202data_all(int atno)
{
	while (1) {
		switch (atno) {
		case	0:
		case	1:
			f202_1data();
		case	2:
		case	3:
			f202_45data();
		case	4:
			f202_7data();
			f202_8data();
		case	5:
			f202_9data();
		case	6:
			f202_10data();
		case	15:
			f202_11data();
		case	16:
			f202_12data();
		case	7:
			f202_13data();
			f202_14data();
			f202_15data();
		case	8:
		case	9:
		case	10:
		case	11:
			if ((*m_pSnHeadData)->IsUseSemiUriRatio()) {
				// 課税売上割合に準ずる割合を有効
				// 課税売上割合が９５％未満
				f202_ko();
			}
			else {
				if (F202_8EX >= (*m_pSnHeadData)->m_UriRatioBorder) {
					// 課税売上割合が９５％以上
					f202_16data();
				}
				else {
					// 課税売上割合が９５％未満
					f202_ko();
				}
			}
		case	12:
			f202_21data();
		case	13:
			f202_22data();
			f202_2324data();
		case	14:
			f202_25data();
		default:
			break;
		}
		break;
	}


}

//-----------------------------------------------------------------------------
// 入力許可・不許可 切替
//-----------------------------------------------------------------------------
// 
// 
//-----------------------------------------------------------------------------
void CShinFhyo2_02::ModifyDiagInput(short index, int sign)
{
	if( !m_DpSw ){	// 再集計のみ（表示無し）
	diag_modify(IDC_ICSDIAG8CTRL1, index, sign, CIcsdiagctrl);
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
void CShinFhyo2_02::ChangeColor(unsigned short id, short index, int sign)
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
int CShinFhyo2_02::UpDownFocusMoveCheck(int Index, int Move)
{
	int idx = 0;
	int i = 0, j = 0;

	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY2_02CNT; i++ ) {
		if ( Index == FCS_Fhyo2_02[i].IDNo )
			break;
	}

	// 現在ポジションセット
	idx = FCS_Fhyo2_02[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) {			//　下方向
		for ( j = i + 1; j < FCS_FHY2_02CNT; j++ ) {
			m_Fhyo2_02Diag02.GetAttribute ( FCS_Fhyo2_02[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo2_02[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {		//　上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo2_02Diag02.GetAttribute ( FCS_Fhyo2_02[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo2_02[j].IDNo;
				break;
			}
		}
	}

	return idx;
}
BEGIN_EVENTSINK_MAP(CShinFhyo2_02, CSyzBaseDlg)
ON_EVENT(CShinFhyo2_02, IDC_ICSDIAG8CTRL1, 1, CShinFhyo2_02::EditONIcsdiag8ctrl1, VTS_I2)
ON_EVENT(CShinFhyo2_02, IDC_ICSDIAG8CTRL1, 2, CShinFhyo2_02::EditOFFIcsdiag8ctrl1, VTS_I2)
ON_EVENT(CShinFhyo2_02, IDC_ICSDIAG8CTRL1, 3, CShinFhyo2_02::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
ON_EVENT(CShinFhyo2_02, IDC_ICSDIAG8CTRL1, 9, CShinFhyo2_02::CheckButtonIcsdiag8ctrl1, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo2_02::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL1;
}

void CShinFhyo2_02::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char	CkMy[MONY_BUF_SIZE];
	int		length;
	int		tmpUWexp = 0;

	char	VAL[MONY_BUF_SIZE] = { 0 };
	char	ValCnv[MONY_BUF_SIZE] = { 0 };

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	char	buf[512] = { 0 };

	// 調整額 ???
	if ((index != ID202_14A) && (index != ID202_14B)) {
		if (index > ID202_14B) {
			m_DlSw = 1;
		}
		else {
			m_DlSw = 0;
		}
	}

	switch (index) {

	case ID202_1A:			// 課税売上額(税抜き)　税率3％適用分　A
	case ID202_1B:			// 課税売上額(税抜き)　税率4％適用分　B
	case ID202_1C:			// 課税売上額(税抜き)　税率6.3％適用分　C
		SyukeiDataSet(index);

		// 課税売上高の5億円超えチェック
		CheckOver500Million();

		// 各入力の表示
		f202data_all(1);
		break;

	case ID202_9A:			// 課税仕入れに係る支払対価の額(税込み)　税率3％適用分　A
	case ID202_9B:			// 課税仕入れに係る支払対価の額(税込み)  税率4％適用分　B
	case ID202_9C:			// 課税仕入れに係る支払対価の額(税込み)  税率6.3％適用分　C
		SyukeiDataSet(index);

		// 各入力の表示
		f202data_all(5);
		break;

	case ID202_10A:			// 課税仕入れに係る消費税額　税率3％適用分　A
	case ID202_10B:			// 課税仕入れに係る消費税額  税率4％適用分　B
	case ID202_10C:			// 課税仕入れに係る消費税額  税率6.3％適用分　C
		SyukeiDataSet(index);

		// 各入力の表示
		f202data_all(6);//
		break;

	case ID202_11C:			// 特定課税仕入れに係る支払対価の額  税率6.3％適用分　E
		SyukeiDataSet(index);

		// 各入力の表示
		f202data_all(15);//
		break;

	case ID202_12C:			// 特定課税仕入れに係る消費税額  税率6.3％適用分　C
		SyukeiDataSet(index);

		// 各入力の表示
		f202data_all(16);//
		break;

/*	case ID202_13A:			// 課税貨物に係る消費税額 税率3％適用分 A
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		memmove ( CkMy, F202_13A, MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAG8CTRL1, ID202_13A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( F202_13A, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_cmp ( F202_13A, CkMy ) ) {
			if ( ( ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) && ( F202_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {	// 個別対応で95％未満の場合
				CString Msg;
				Msg.Empty();
				Msg = _T( "課税貨物に係る消費税額が変更されました。\n" );
				Msg += _T( "個別対応方式欄の⑰～⑲の金額を見直してください。" );
				ICSMessageBox ( Msg, MB_OK, 0 );
			}
		}

		SyukeiDataSet ( index );
		// 各入力の表示
		f202data_all ( 7 );
		break;

	case ID202_13B:			// 課税貨物に係る消費税額 税率4％適用分 B
		memset ( CkMy, '\0', MONY_BUF_SIZE );
		memmove ( CkMy, F202_13B, MONY_BUF_SIZE );
		diag_getdata ( IDC_ICSDIAG8CTRL1, ID202_13B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		memmove ( F202_13B, DiagData.data_val, MONY_BUF_SIZE );
		if ( m_pArith->l_cmp ( F202_13B, CkMy ) ) {
			if ( ( ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == TRUE ) && ( F202_8EX < ( *m_pSnHeadData )->m_UriRatioBorder ) ) || ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == TRUE ) ) {	// 個別対応で95％未満の場合
				CString Msg;
				Msg.Empty();
				Msg = _T( "課税貨物に係る消費税額が変更されました。\n" );
				Msg += _T( "個別対応方式欄の⑰～⑲の金額を見直してください。" );
				ICSMessageBox ( Msg, MB_OK, 0 );
			}
		}

		SyukeiDataSet ( index );
		// 各入力の表示
		f202data_all ( 7 );
		break;
*/
	case ID202_13C:			// 課税貨物に係る消費税額　税率6.3％適用分　C
		memset(CkMy, '\0', MONY_BUF_SIZE);
		memmove(CkMy, F202_13C, MONY_BUF_SIZE);
		diag_getdata(IDC_ICSDIAG8CTRL1, ID202_13C, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(F202_13C, DiagData.data_val, MONY_BUF_SIZE);
		if (m_pArith->l_cmp(F202_13C, CkMy)) {
			if ((((*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE) && (F202_8EX < (*m_pSnHeadData)->m_UriRatioBorder)) || ((*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE)) {		// 個別対応で95％未満の場合
				CString		Msg;
				Msg.Empty();
				Msg = _T("課税貨物に係る消費税額が変更されました。\n");
				Msg += _T("個別対応方式欄の⑰～⑲の金額を見直してください。");
				ICSMessageBox(Msg, MB_OK, 0);	// IDOK IDCANCEL
			}
		}

		SyukeiDataSet(index);

		// 各入力の表示
		f202data_all(7);
		break;

	case ID202_14A:
	case ID202_14B:
	case ID202_14C:

		break;

	case ID202_17A:			// 課税売上げにのみ要するもの　税率3％適用分　A
	case ID202_17B:			// 課税売上げにのみ要するもの  税率4％適用分　B
	case ID202_17C:			// 課税売上げにのみ要するもの  税率6.3％適用分　C
		if ((*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE) {
			if ((F202_8EX >= (*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE)) {
				break;
			}
		}

		SyukeiDataSet(index);

		// 各入力の表示
		f202data_all(8);
		break;

	case ID202_18A:			// 課税売上げと非課税売上げに共通して要するもの　税率3％適用分　A
	case ID202_18B:			// 課税売上げと非課税売上げに共通して要するもの  税率4％適用分　B
	case ID202_18C:			// 課税売上げと非課税売上げに共通して要するもの  税率6.3％適用分　C
		if ((*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE) {
			if ((F202_8EX >= (*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE)) {
				break;
			}
		}
		diag_getdata(IDC_ICSDIAG8CTRL1, ID202_18A, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		memmove(F202_18A, DiagData.data_val, MONY_BUF_SIZE);

		SyukeiDataSet(index);

		// 各入力の表示
		f202data_all(9);
		break;

	case ID202_19A:			// 個別対応方式により控除する課税仕入れ等の税額　税率3％適用分　A
	case ID202_19B:			// 個別対応方式により控除する課税仕入れ等の税額  税率4％適用分　B
	case ID202_19C:			// 個別対応方式により控除する課税仕入れ等の税額  税率6.3％適用分　C
		if ((*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE) {
			if ((F202_8EX >= (*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE)) {
				break;
			}
		}

		SyukeiDataSet(index);

		// 各入力の表示
		f202data_all(10);
		break;

	case ID202_20A:			// 一括比例配分方式により控除する課税仕入れ等の税額　税率3％適用分　A
	case ID202_20B:			// 一括比例配分方式により控除する課税仕入れ等の税額  税率4％適用分　B
	case ID202_20C:			// 一括比例配分方式により控除する課税仕入れ等の税額  税率6.3％適用分　C
		if ((*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE) {
			if ((F202_8EX >= (*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE)) {
				break;
			}
		}

		SyukeiDataSet(index);

		// 各入力の表示
		f202data_all(11);
		break;

	case ID202_21A:			// 調整対象固定資産に係る調整税額　税率3％適用分　A
	case ID202_21B:			// 調整対象固定資産に係る調整税額  税率4％適用分　B
	case ID202_21C:			// 調整対象固定資産に係る調整税額  税率6.3％適用分　C
		SyukeiDataSet(index);

		// 各入力の表示
		f202data_all(12);
		break;

	case ID202_22A:			// 調整対象固定資産を課税業務用に転用した調整額　税率3％適用分　A
	case ID202_22B:			// 調整対象固定資産を課税業務用に転用した調整額  税率4％適用分　B
	case ID202_22C:			// 調整対象固定資産を課税業務用に転用した調整額  税率6.3％適用分　C
		SyukeiDataSet(index);

		// 各入力の表示
		f202data_all(13);
		break;

	case ID202_23A:			// 控除対象仕入税額　税率3％適用分　A
	case ID202_23B:			// 控除対象仕入税額  税率4％適用分　B
	case ID202_23C:			// 控除対象仕入税額  税率6.3％適用分　C
		SyukeiDataSet(index);

		// 各入力の表示
		f202data_all(13);
		break;

	case ID202_25A:			// 貸倒回収に係る消費税額　税率3％適用分　A
	case ID202_25B:			// 貸倒回収に係る消費税額  税率4％適用分　B
	case ID202_25C:			// 貸倒回収に係る消費税額  税率6.3％適用分　C
		SyukeiDataSet(index);

		// 各入力の表示
		f202data_all(14);
		break;

	case ID202_8EX:			// 課税売上割合の変更チェック
		(*m_pSnHeadData)->Sn_UPERS &= 0xef;
		diag_getdata(IDC_ICSDIAG8CTRL1, ID202_8EX, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		if (DiagData.data_check) {
			(*m_pSnHeadData)->Sn_UPERS |= 0x10;
		}

		SyukeiDataSet(index);

		// 各入力の表示
		f202data_all(4);
		break;

	case ID202_8X:			// 課税売上割合　旧税率分小計 X
			F202_8X = (*m_pSnFhyo2_02Data)->Sn_F202_8X_UW;
			diag_getdata( IDC_ICSDIAG8CTRL1, ID202_8X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			length = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) ); 
			tmpUWexp = 0;
			if( length ){
				memmove( buf, DiagData.data_edit, length );
			}
			m_Util.perinp( 3, 2, buf, (short *)&tmpUWexp );
			if( tmpUWexp <= 10000 ){
				(*m_pSnFhyo2_02Data)->Sn_F202_8X_UW = tmpUWexp;
			}
			// 個別対応で95％未満の場合
			if( ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) && (F202_8X!=(*m_pSnFhyo2_02Data)->Sn_F202_8X_UW) ){
				CString		Msg;
				Msg.Empty();
				Msg =  _T("課税売上割合が変更されました。\n");
				//Msg += _T("個別対応方式欄の⑮～⑰の金額を見直してください。");
				Msg += _T("個別対応方式欄の⑰～⑲の金額を見直してください。");
				ICSMessageBox( Msg, MB_OK, 0 );
				// 各入力の計算表示
				f202data_all( 4 );
				break;
			}
			else{
				// 個別対応で95％未満の場合
				if( ((*m_pSnHeadData)->IsKobetuSiireAnbun()==TRUE) && (F202_8X!=(*m_pSnFhyo2_02Data)->Sn_F202_8X_UW) && ((*m_pSnFhyo2_02Data)->Sn_F202_8X_UW<(*m_pSnHeadData)->m_UriRatioBorder) ){
					CString		Msg;
					Msg.Empty();
					Msg =  _T("課税売上割合が変更されました。\n");
					//Msg += _T("個別対応方式欄の⑮～⑰の金額を見直してください。");
					Msg += _T("個別対応方式欄の⑰～⑲の金額を見直してください。");
					ICSMessageBox( Msg, MB_OK, 0 );
				}
			}

			SyukeiDataSet(index);

			// 各入力の計算表示
			f202data_all( 4 );

		break;

	default:
		break;

	}


}

void CShinFhyo2_02::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}

	// Shift+TABをTABに、TABをENTERに返還
	short	wTerm;
	char	VK_FLG = 0x00;
	VK_FLG = (char )::GetKeyState( VK_SHIFT );
	if( nChar == VK_TAB ){
		if( VK_FLG & 0x80 ){
			wTerm = VK_TAB;
		}
		else{
			wTerm = VK_RETURN;
		}
	}
	else{
		wTerm = nChar;
	}

	// 調整額 ???
	if ((index != ID202_14A) && (index != ID202_14B)) {
		if (index > ID202_14B) {
			m_DlSw = 1;
		}
		else {
			m_DlSw = 0;
		}
	}

	int pos = 0;
	if( wTerm==VK_UP ){
		pos = UpDownFocusMoveCheck(index , 1);
		m_Fhyo2_02Diag02.SetPosition(pos);
	}
	if( wTerm==VK_DOWN ){
		pos = UpDownFocusMoveCheck(index , 0);
		m_Fhyo2_02Diag02.SetPosition(pos);
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
		if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) { // 非連動の場合
			if (index == ID202_13C) {
				c_set(IDC_BUTTON1);
			}
			else {
				m_Fhyo2_02Diag02.SetNextPosition();
			}
		}
		else {
			if (index == ID202_20C) {
				c_set(IDC_BUTTON1);
			}
			else {
				m_Fhyo2_02Diag02.SetNextPosition();
			}
		}
	}
	if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		BOOL IsPrevPosition = TRUE;
		if (*m_pShinInfo->pOver500MillionSw != 1 && F202_8EX >= (*m_pSnHeadData)->m_UriRatioBorder) {	//5億円以下かつ 課税売上割合が９５％以上の場合
			if (index == ID202_19A) {
				c_set(IDC_BUTTON1);
				IsPrevPosition = FALSE;
			}
		}
		else {
			if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {//非連動の場合
				if ((*m_pSnHeadData)->IsKobetuSiireAnbun()) {	// 個別対応
					if (index == ID202_17A) {
						c_set(IDC_BUTTON1);
						IsPrevPosition = FALSE;
					}
				}
				else {
					if (index == ID202_20A) {
						c_set(IDC_BUTTON1);
						IsPrevPosition = FALSE;
					}
				}
			}
			else {
				if (index == ID202_21A) {
					c_set(IDC_BUTTON1);
					IsPrevPosition = FALSE;
				}
			}
		}
		if (IsPrevPosition) {
			m_Fhyo2_02Diag02.SetPrevPosition();
		}
	}

	if( wTerm == VK_DELETE ){
		switch( index ){
		case ID202_23A:			//  控除対象仕入税額　税率3％適用分　A
			F202_23c &= ~0x08;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322301"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322401"));
			// 各入力の表示
			f202data_all(13);
			m_Fhyo2_02Diag02.SetPosition(ID202_23A);
			break;

		case ID202_23B:			//  控除対象仕入税額　税率4％適用分　B
			F202_23c &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322302"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322402"));
			// 各入力の表示
			f202data_all(13);
			m_Fhyo2_02Diag02.SetPosition(ID202_23B);
			break;

		case ID202_23C:			//  控除対象仕入税額　税率6.3％適用分　C
			F202_23c &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322303"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322403"));
			// 各入力の表示
			f202data_all(13);
			m_Fhyo2_02Diag02.SetPosition(ID202_23C);
			break;

		case ID202_10A:			//	課税仕入れに係る消費税額 税率3％適用分　A
			F202_10c &= ~0x08;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("321001"));
			// 各入力の表示
			f202data_all(6);
			m_Fhyo2_02Diag02.SetPosition(ID202_10A);
			break;

		case ID202_10B:			//  課税仕入れに係る消費税額 税率4％適用分　B
			F202_10c &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("321002"));
			// 各入力の表示
			f202data_all(6);
			m_Fhyo2_02Diag02.SetPosition(ID202_10B);
			break;

		case ID202_10C:			//  課税仕入れに係る消費税額 税率6.3％適用分　C
			F202_10c &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("321003"));
			// 各入力の表示
			f202data_all(6);
			m_Fhyo2_02Diag02.SetPosition(ID202_10C);
			break;

		case ID202_12C:			//  特定課税仕入れに係る消費税額 税率6.3％適用分　C
			F202_12c &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("321203"));
			// 各入力の表示
			f202data_all(16);
			m_Fhyo2_02Diag02.SetPosition(ID202_12C);
			break;

		case ID202_19A:			//	個別対応方式により控除する課税仕入れ等の税額 税率3％適用分　A
			F202_19c &= ~0x08;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("321901"));
			// 各入力の表示
			f202data_all(10);
			m_Fhyo2_02Diag02.SetPosition(ID202_19A);
			break;

		case ID202_19B:			//  個別対応方式により控除する課税仕入れ等の税額 税率4％適用分　B
			F202_19c &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("321902"));
			// 各入力の表示
			f202data_all(10);
			m_Fhyo2_02Diag02.SetPosition(ID202_19B);
			break;

		case ID202_19C:			//  個別対応方式により控除する課税仕入れ等の税額 税率6.3％適用分　C
			F202_19c &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("321903"));
			// 各入力の表示
			f202data_all(10);
			m_Fhyo2_02Diag02.SetPosition(ID202_19C);
			break;

		case ID202_20A:			//  一括比例配分方式により控除する課税仕入れ等の税額 税率3％適用分　A
			F202_20c &= ~0x08;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322001"));
			// 各入力の表示
			f202data_all(11);
			m_Fhyo2_02Diag02.SetPosition(ID202_20A);
			break;
		case ID202_20B:			//  一括比例配分方式により控除する課税仕入れ等の税額 税率4％適用分　B
			F202_20c &= ~0x80;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322002"));
			// 各入力の表示
			f202data_all(11);
			m_Fhyo2_02Diag02.SetPosition(ID202_20B);
			break;

		case ID202_20C:			//  一括比例配分方式により控除する課税仕入れ等の税額 税率6.3％適用分　C
			F202_20c &= ~0x800;
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322003"));
			// 各入力の表示
			f202data_all(11);
			m_Fhyo2_02Diag02.SetPosition(ID202_20C);
			break;

		default:
			dsp_cls(IDC_ICSDIAG8CTRL1, index);
			break;
		}

	}

	CWnd* pWnd = this->GetFocus();
	int nowpos = 0;
	if ( pWnd->GetDlgCtrlID() == IDC_BUTTON1 ) {
		nowpos = ID202_14X;	// 14Xのポジションをセット
	}
	else {
		nowpos = m_Fhyo2_02Diag02.GetPosition();	// 移動先または自身のポジション
	}

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAG8CTRL1, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}

}

BOOL CShinFhyo2_02::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	CWnd *pWnd = GetFocus();

	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_END) {
			// 付表２－２更新
			GetAllDiagData();
			f202data_all(1);
			WriteData(1);

			if (m_pParent) {
				m_pParent->PostMessageA(WM_KEYDOWN, VK_END);
			}

			return TRUE;
		}
		if (pMsg->wParam == VK_F2) {
			DIAGRAM_DATA   dd;
			if (m_curwnd != -1 && (pWnd != GetDlgItem(IDC_BUTTON1))) {
				short result = ((CIcsdiagctrl*)GetDlgItem(m_curwnd))->GetPosition();
				diag_getdata(m_curwnd, result, &dd, CIcsdiagctrl);
				if (m_curwnd == IDC_ICSDIAG8CTRL1) {
					TerminationIcsdiag8ctrl1(result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd);
				}
				return TRUE;
			}
		}

		if (pWnd == GetDlgItem(IDC_BUTTON1)) {
			char VK_FLG = 0x00;
			VK_FLG = (char)::GetKeyState(VK_SHIFT);

			if ((pMsg->wParam == VK_F3) || (pMsg->wParam == VK_RIGHT) || (pMsg->wParam == VK_DOWN) || ((pMsg->wParam == VK_TAB) && !(VK_FLG & 0x80))) {
				if (*m_pShinInfo->pOver500MillionSw != 1 && F202_8EX >= (*m_pSnHeadData)->m_UriRatioBorder) {	//5億円かつ課税割合が95％以上の場合
					diag_setposition(IDC_ICSDIAG8CTRL1, ID202_21A, CIcsdiagctrl);
				}
				else {
					if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {	// 非連動の場合
						if ((*m_pSnHeadData)->IsKobetuSiireAnbun()) {	// 個別対応
							diag_setposition(IDC_ICSDIAG8CTRL1, ID202_17A, CIcsdiagctrl);
						}
						else {
							diag_setposition(IDC_ICSDIAG8CTRL1, ID202_20A, CIcsdiagctrl);
						}
					}
					else {
						diag_setposition(IDC_ICSDIAG8CTRL1, ID202_21A, CIcsdiagctrl);
					}
				}
				return TRUE;
			}
			// 前項目へ
			if ((pMsg->wParam == VK_LEFT) || (pMsg->wParam == VK_F2) || (pMsg->wParam == VK_UP) || ((pMsg->wParam == VK_TAB) && (VK_FLG & 0x80))) {
				if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {	// 非連動の場合
					diag_setposition(IDC_ICSDIAG8CTRL1, ID202_13C, CIcsdiagctrl);
				}
				else {
					diag_setposition(IDC_ICSDIAG8CTRL1, ID202_22C, CIcsdiagctrl);
				}
				return TRUE;
			}
		}

	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}


// 棚卸調整額入力
void CShinFhyo2_02::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if ((*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE) {

		// 前回の情報をセーブ
		char			SV_mtype = (*m_pSnTanaData)->Sn_MENZsw;
		unsigned short	SV_F202_8EX = F202_8EX;
		char			SV_mnk40[3][MONY_BUF_SIZE];
		char			SV_mnz40[3][MONY_BUF_SIZE];
		char			SV_mnk63[3][MONY_BUF_SIZE];
		char			SV_mnz63[3][MONY_BUF_SIZE];
		char			SV_menzk[3][MONY_BUF_SIZE];
		char			SV_menzz[3][MONY_BUF_SIZE];
		char			zerobuf[MONY_BUF_SIZE];

		char			SV_mnk624[3][MONY_BUF_SIZE];
		char			SV_mnz624[3][MONY_BUF_SIZE];
		char			SV_mnk78[3][MONY_BUF_SIZE];
		char			SV_mnz78[3][MONY_BUF_SIZE];
		char			VAL_14D[MONY_BUF_SIZE];
		char			VAL_14E[MONY_BUF_SIZE];
		char			VAL_17D[MONY_BUF_SIZE];
		char			VAL_17E[MONY_BUF_SIZE];
		char			VAL_18D[MONY_BUF_SIZE];
		char			VAL_18E[MONY_BUF_SIZE];

		memmove(SV_mnk63[0], (*m_pSnTanaData)->Sn_mnk63[0], MONY_BUF_SIZE);	// 棚卸資産額 (新税率分)　課税に係る
		memmove(SV_mnz63[0], (*m_pSnTanaData)->Sn_mnz63[0], MONY_BUF_SIZE);	// 消費税額   (〃      )　〃
		memmove(SV_mnk63[1], (*m_pSnTanaData)->Sn_mnk63[1], MONY_BUF_SIZE);	// 棚卸資産額 (新税率分)　非課税に係る
		memmove(SV_mnz63[1], (*m_pSnTanaData)->Sn_mnz63[1], MONY_BUF_SIZE);	// 消費税額   (〃      )　〃
		memmove(SV_mnk63[2], (*m_pSnTanaData)->Sn_mnk63[2], MONY_BUF_SIZE);	// 棚卸資産額 (新税率分)　共通に係る
		memmove(SV_mnz63[2], (*m_pSnTanaData)->Sn_mnz63[2], MONY_BUF_SIZE);	// 消費税額   (〃      )　〃

		if ((*m_pSnHeadData)->SVmzsw == 1) {
			memmove(SV_mnk40[0], (*m_pSnTanaData)->Sn_mnk40[0], MONY_BUF_SIZE);	// 棚卸資産額 (新税率分)　課税に係る
			memmove(SV_mnz40[0], (*m_pSnTanaData)->Sn_mnz40[0], MONY_BUF_SIZE);	// 消費税額   (〃      )　〃
			memmove(SV_mnk40[1], (*m_pSnTanaData)->Sn_mnk40[1], MONY_BUF_SIZE);	// 棚卸資産額 (新税率分)　非課税に係る
			memmove(SV_mnz40[1], (*m_pSnTanaData)->Sn_mnz40[1], MONY_BUF_SIZE);	// 消費税額   (〃      )　〃
			memmove(SV_mnk40[2], (*m_pSnTanaData)->Sn_mnk40[2], MONY_BUF_SIZE);	// 棚卸資産額 (新税率分)　共通に係る
			memmove(SV_mnz40[2], (*m_pSnTanaData)->Sn_mnz40[2], MONY_BUF_SIZE);	// 消費税額   (〃      )　〃

			memmove(SV_menzk[0], (*m_pSnTanaData)->Sn_menzk[0], MONY_BUF_SIZE);	// 棚卸資産額 (新税率分)　課税に係る
			memmove(SV_menzz[0], (*m_pSnTanaData)->Sn_menzz[0], MONY_BUF_SIZE);	// 消費税額   (〃      )　〃
			memmove(SV_menzk[1], (*m_pSnTanaData)->Sn_menzk[1], MONY_BUF_SIZE);	// 棚卸資産額 (新税率分)　非課税に係る
			memmove(SV_menzz[1], (*m_pSnTanaData)->Sn_menzz[1], MONY_BUF_SIZE);	//消費税額   (〃      )　〃
			memmove(SV_menzk[2], (*m_pSnTanaData)->Sn_menzk[2], MONY_BUF_SIZE);	// 棚卸資産額 (新税率分)　共通に係る
			memmove(SV_menzz[2], (*m_pSnTanaData)->Sn_menzz[2], MONY_BUF_SIZE);	// 消費税額   (〃      )　〃
		}

		memmove(SV_mnk78[0], (*m_pSnTanaData)->Sn_mnk78[0], MONY_BUF_SIZE);
		memmove(SV_mnz78[0], (*m_pSnTanaData)->Sn_mnz78[0], MONY_BUF_SIZE);
		memmove(SV_mnk78[1], (*m_pSnTanaData)->Sn_mnk78[1], MONY_BUF_SIZE);
		memmove(SV_mnz78[1], (*m_pSnTanaData)->Sn_mnz78[1], MONY_BUF_SIZE);
		memmove(SV_mnk78[2], (*m_pSnTanaData)->Sn_mnk78[2], MONY_BUF_SIZE);
		memmove(SV_mnz78[2], (*m_pSnTanaData)->Sn_mnz78[2], MONY_BUF_SIZE);

		memmove(SV_mnk624[0], (*m_pSnTanaData)->Sn_mnk624[0], MONY_BUF_SIZE);
		memmove(SV_mnz624[0], (*m_pSnTanaData)->Sn_mnz624[0], MONY_BUF_SIZE);
		memmove(SV_mnk624[1], (*m_pSnTanaData)->Sn_mnk624[1], MONY_BUF_SIZE);
		memmove(SV_mnz624[1], (*m_pSnTanaData)->Sn_mnz624[1], MONY_BUF_SIZE);
		memmove(SV_mnk624[2], (*m_pSnTanaData)->Sn_mnk624[2], MONY_BUF_SIZE);
		memmove(SV_mnz624[2], (*m_pSnTanaData)->Sn_mnz624[2], MONY_BUF_SIZE);

		// 個別対応
		CChangeTaxEx	CTDiagEx;
		CTDiagEx.InitInfo((*m_pSnFhyo2_02Data), (*m_pSnHeadData), (*m_pSnTanaData), m_pArith);


		HINSTANCE svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle(g_hInstance);
		int st = (int)CTDiagEx.DoModal();
		AfxSetResourceHandle(svhInstResource);

		if (st == IDOK) {

			// 手入力
			//if (!((*m_pSnTanaData)->Sn_tansw & 0x01) || ((*m_pSnHeadData)->Sn_Sign4 & 0x80)) {//非連動の場合は常に更新させる
			if((*m_pSnHeadData)->Sn_Sign4 & 0x80) {	// 非連動
				if ((*m_pSnHeadData)->SVmzsw == 1) {
					MoneyBasejagArray	money;

					// 付表２－２のデータを取得
					money = (*m_pSyzSyukei)->GetShinkokuData(_T("324202"));
					memmove(VAL_17D, money[0][0].arith, MONY_BUF_SIZE);
					money = (*m_pSyzSyukei)->GetShinkokuData(_T("324203"));
					memmove(VAL_17E, money[0][0].arith, MONY_BUF_SIZE);
					money = (*m_pSyzSyukei)->GetShinkokuData(_T("324302"));
					memmove(VAL_18D, money[0][0].arith, MONY_BUF_SIZE);
					money = (*m_pSyzSyukei)->GetShinkokuData(_T("324303"));
					memmove(VAL_18E, money[0][0].arith, MONY_BUF_SIZE);
				}

				// 前回が個別対応で95％未満の場合、前回分を削除
				if ((SV_mtype & 0x10) || ((*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE)) {
					if (SV_mtype & 0x01) {
						m_pArith->l_add(F202_17C, F202_17C, SV_mnz63[0]);			// 課税売上に係る仕入税額
						m_pArith->l_add(F202_18C, F202_18C, SV_mnz63[2]);			// 共通の売上に係る仕入税額

						if ((*m_pSnHeadData)->SVmzsw == 1) {
							m_pArith->l_add(F202_17B, F202_17B, SV_mnz40[0]);		// 課税売上に係る仕入税額
							m_pArith->l_add(F202_18B, F202_18B, SV_mnz40[2]);		// 共通の売上に係る仕入税額
							m_pArith->l_add(F202_17A, F202_17A, SV_menzz[0]);		// 課税売上に係る仕入税額
							m_pArith->l_add(F202_18A, F202_18A, SV_menzz[2]);		// 共通の売上に係る仕入税額
						}

						m_pArith->l_add(VAL_17E, VAL_17E, SV_mnz78[0]);			// 課税売上に係る仕入税額
						m_pArith->l_add(VAL_18E, VAL_18E, SV_mnz78[2]);			// 共通の売上に係る仕入税額
						m_pArith->l_add(VAL_17D, VAL_17D, SV_mnz624[0]);		// 課税売上に係る仕入税額
						m_pArith->l_add(VAL_18D, VAL_18D, SV_mnz624[2]);		// 共通の売上に係る仕入税額
					}
					else {
						m_pArith->l_sub(F202_17C, F202_17C, SV_mnz63[0]);				// 15. 課税売上に係る仕入税額
						m_pArith->l_sub(F202_18C, F202_18C, SV_mnz63[2]);				// 16. 共通の売上に係る仕入税額
						if ((*m_pSnHeadData)->SVmzsw == 1) {
							m_pArith->l_sub(F202_17B, F202_17B, SV_mnz40[0]);			// 15. 課税売上に係る仕入税額
							m_pArith->l_sub(F202_18B, F202_18B, SV_mnz40[2]);			// 16. 共通の売上に係る仕入税額
							m_pArith->l_sub(F202_17A, F202_17A, SV_menzz[0]);			// 15. 課税売上に係る仕入税額
							m_pArith->l_sub(F202_18A, F202_18A, SV_menzz[2]);			// 16. 共通の売上に係る仕入税額
						}

						m_pArith->l_sub(VAL_17E, VAL_17E, SV_mnz78[0]);			// 課税売上に係る仕入税額
						m_pArith->l_sub(VAL_18E, VAL_18E, SV_mnz78[2]);			// 共通の売上に係る仕入税額
						m_pArith->l_sub(VAL_17D, VAL_17D, SV_mnz624[0]);		// 課税売上に係る仕入税額
						m_pArith->l_sub(VAL_18D, VAL_18D, SV_mnz624[2]);		// 共通の売上に係る仕入税額
					}
				}

				if ((*m_pSnTanaData)->Sn_MENZsw & 0x10) {
					memset(F202_14C, '\0', MONY_BUF_SIZE);
					m_pArith->l_add(F202_14C, F202_14C, (*m_pSnTanaData)->Sn_mnz63[0]);
					m_pArith->l_add(F202_14C, F202_14C, (*m_pSnTanaData)->Sn_mnz63[1]);
					m_pArith->l_add(F202_14C, F202_14C, (*m_pSnTanaData)->Sn_mnz63[2]);
					if ((*m_pSnHeadData)->SVmzsw == 1) {
						memset(F202_14B, '\0', MONY_BUF_SIZE);
						m_pArith->l_add(F202_14B, F202_14B, (*m_pSnTanaData)->Sn_mnz40[0]);
						m_pArith->l_add(F202_14B, F202_14B, (*m_pSnTanaData)->Sn_mnz40[1]);
						m_pArith->l_add(F202_14B, F202_14B, (*m_pSnTanaData)->Sn_mnz40[2]);

						memset(F202_14A, '\0', MONY_BUF_SIZE);
						m_pArith->l_add(F202_14A, F202_14A, (*m_pSnTanaData)->Sn_menzz[0]);	//				 〃	   消費税額   (〃      )　〃
						m_pArith->l_add(F202_14A, F202_14A, (*m_pSnTanaData)->Sn_menzz[1]);	//				 〃	   消費税額   (〃      )　〃
						m_pArith->l_add(F202_14A, F202_14A, (*m_pSnTanaData)->Sn_menzz[2]);	//				 〃	   消費税額   (〃      )　〃
					}

					memset(VAL_14E, '\0', MONY_BUF_SIZE);
					memset(VAL_14D, '\0', MONY_BUF_SIZE);
					m_pArith->l_add(VAL_14E, VAL_14E, (*m_pSnTanaData)->Sn_mnz78[0]);
					m_pArith->l_add(VAL_14E, VAL_14E, (*m_pSnTanaData)->Sn_mnz78[1]);
					m_pArith->l_add(VAL_14E, VAL_14E, (*m_pSnTanaData)->Sn_mnz78[2]);
					m_pArith->l_add(VAL_14D, VAL_14D, (*m_pSnTanaData)->Sn_mnz624[0]);
					m_pArith->l_add(VAL_14D, VAL_14D, (*m_pSnTanaData)->Sn_mnz624[1]);
					m_pArith->l_add(VAL_14D, VAL_14D, (*m_pSnTanaData)->Sn_mnz624[2]);

					// 旧格納場所へ格納
					memmove((*m_pSnTanaData)->Sn_MNZ63, F202_14C, MONY_BUF_SIZE);
					if ((*m_pSnHeadData)->SVmzsw == 1) {
						memmove((*m_pSnTanaData)->Sn_MNZ40, F202_14B, MONY_BUF_SIZE);
						memmove((*m_pSnTanaData)->Sn_MENZZ, F202_14A, MONY_BUF_SIZE);
					}
					memmove((*m_pSnTanaData)->Sn_MNZ78, VAL_14E, MONY_BUF_SIZE);
					memmove((*m_pSnTanaData)->Sn_MNZ624, VAL_14D, MONY_BUF_SIZE);
					if ((*m_pSnTanaData)->Sn_MENZsw & 0x01) {
						m_pArith->l_neg(F202_14A);
						m_pArith->l_neg(F202_14B);
						m_pArith->l_neg(F202_14C);
						m_pArith->l_neg(VAL_14D);
						m_pArith->l_neg(VAL_14E);
					}
					// 個別対応で95％未満の場合
					if ((F202_8EX < (*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE)) {
						if ((*m_pSnTanaData)->Sn_MENZsw & 0x01) {
							m_pArith->l_sub(F202_17C, F202_17C, (*m_pSnTanaData)->Sn_mnz63[0]);
							m_pArith->l_sub(F202_18C, F202_18C, (*m_pSnTanaData)->Sn_mnz63[2]);
							if ((*m_pSnHeadData)->SVmzsw == 1) {
								m_pArith->l_sub(F202_17B, F202_17B, (*m_pSnTanaData)->Sn_mnz40[0]);
								m_pArith->l_sub(F202_18B, F202_18B, (*m_pSnTanaData)->Sn_mnz40[2]);
								m_pArith->l_sub(F202_17A, F202_17A, (*m_pSnTanaData)->Sn_menzz[0]);
								m_pArith->l_sub(F202_18A, F202_18A, (*m_pSnTanaData)->Sn_menzz[2]);
							}
							m_pArith->l_sub(VAL_17E, VAL_17E, (*m_pSnTanaData)->Sn_mnz78[0]);
							m_pArith->l_sub(VAL_18E, VAL_18E, (*m_pSnTanaData)->Sn_mnz78[2]);
							m_pArith->l_sub(VAL_17D, VAL_17D, (*m_pSnTanaData)->Sn_mnz624[0]);
							m_pArith->l_sub(VAL_18D, VAL_18D, (*m_pSnTanaData)->Sn_mnz624[2]);
						}
						else {
							m_pArith->l_add(F202_17C, F202_17C, (*m_pSnTanaData)->Sn_mnz63[0]);
							m_pArith->l_add(F202_18C, F202_18C, (*m_pSnTanaData)->Sn_mnz63[2]);
							if ((*m_pSnHeadData)->SVmzsw == 1) {
								m_pArith->l_add(F202_17B, F202_17B, (*m_pSnTanaData)->Sn_mnz40[0]);
								m_pArith->l_add(F202_18B, F202_18B, (*m_pSnTanaData)->Sn_mnz40[2]);
								m_pArith->l_add(F202_17A, F202_17A, (*m_pSnTanaData)->Sn_menzz[0]);
								m_pArith->l_add(F202_18A, F202_18A, (*m_pSnTanaData)->Sn_menzz[2]);
							}
							m_pArith->l_add(VAL_17E, VAL_17E, (*m_pSnTanaData)->Sn_mnz78[0]);
							m_pArith->l_add(VAL_18E, VAL_18E, (*m_pSnTanaData)->Sn_mnz78[2]);
							m_pArith->l_add(VAL_17D, VAL_17D, (*m_pSnTanaData)->Sn_mnz624[0]);
							m_pArith->l_add(VAL_18D, VAL_18D, (*m_pSnTanaData)->Sn_mnz624[2]);
						}
					}

					(*m_pSyzSyukei)->SetShinkokuData(_T("321401"), F202_14A);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321402"), F202_14B);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321403"), F202_14C);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321701"), F202_17A);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321702"), F202_17B);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321703"), F202_17C);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321801"), F202_18A);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321802"), F202_18B);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321803"), F202_18C);

					(*m_pSyzSyukei)->SetShinkokuData(_T("323902"), VAL_14D);
					(*m_pSyzSyukei)->SetShinkokuData(_T("323903"), VAL_14E);
					if ((F202_8EX < (*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE)) {
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "324202" ), VAL_17D );
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "324203" ), VAL_17E );
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "324302" ), VAL_18D );
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "324303" ), VAL_18E );
					}

				}
				else {
					memset(F202_14A, '\0', MONY_BUF_SIZE);
					memset(F202_14B, '\0', MONY_BUF_SIZE);
					memset(F202_14C, '\0', MONY_BUF_SIZE);

					(*m_pSyzSyukei)->SetShinkokuData(_T("321401"), F202_14A);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321402"), F202_14B);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321403"), F202_14C);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321701"), F202_17A);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321702"), F202_17B);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321703"), F202_17C);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321801"), F202_18A);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321802"), F202_18B);
					(*m_pSyzSyukei)->SetShinkokuData(_T("321803"), F202_18C);

					// 付表２－１の金額を更新
					memset(zerobuf, '\0', MONY_BUF_SIZE);
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), zerobuf );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), zerobuf );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324202" ), VAL_17D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324203" ), VAL_17E );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324302" ), VAL_18D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324303" ), VAL_18E );
				}

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_02();
				SetItemPosition();
			}
			else if (!((*m_pSnTanaData)->Sn_tansw & 0x01)) {
				// 連動時は、TanaDataAdd（MemoryReget）で行う

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_02();
				SetItemPosition();
			}
		}
	}
	else {
		CChangeTax	CTDiag;
		CTDiag.InitInfo ( ( *m_pSnHeadData ), ( *m_pSnTanaData ), m_pArith );

		// 比例配分
		HINSTANCE svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle(g_hInstance);
		int st = (int)CTDiag.DoModal();
		AfxSetResourceHandle(svhInstResource);

		if (st == IDOK) {

			//if (!((*m_pSnTanaData)->Sn_tansw & 0x01) || ((*m_pSnHeadData)->Sn_Sign4 & 0x80)) {//非連動の場合は常に更新させる
			if((*m_pSnHeadData)->Sn_Sign4 & 0x80) {	// 非連動
				// (免税)控除対象調整税額
				if ((*m_pSnTanaData)->Sn_MENZsw & 0x10) {
					memmove(F202_14A, (*m_pSnTanaData)->Sn_MENZZ, MONY_BUF_SIZE);
					memmove(F202_14B, (*m_pSnTanaData)->Sn_MNZ40, MONY_BUF_SIZE);
					memmove(F202_14C, (*m_pSnTanaData)->Sn_MNZ63, MONY_BUF_SIZE);
					if ((*m_pSnTanaData)->Sn_MENZsw & 0x01) {
						m_pArith->l_neg(F202_14A);
						m_pArith->l_neg(F202_14B);
						m_pArith->l_neg(F202_14C);
					}

					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), F202_14A );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), F202_14B );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), F202_14C );
				}
				else {
					memset(F202_14A, '\0', MONY_BUF_SIZE);
					memset(F202_14B, '\0', MONY_BUF_SIZE);
					memset(F202_14C, '\0', MONY_BUF_SIZE);
					
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321401" ) );
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321402" ) );
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321403" ) );
					// 付表２－１もリセット
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323902" ) );
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323903" ) );
				}


				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_02();
				SetItemPosition();	// ('14.03.13)
			}
			else if (!((*m_pSnTanaData)->Sn_tansw & 0x01)) {
				// 連動時は、TanaDataAdd（MemoryReget）で行う

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_02();
				SetItemPosition();	// ('14.03.13)
			}
		}
	}
}


void CShinFhyo2_02::CheckButtonIcsdiag8ctrl1(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 課税売上割合の変更チェック
	if (index == ID202_8EX) {
		diag_getdata(IDC_ICSDIAG8CTRL1, ID202_8EX, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		if ((*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE) {
			CString	Msg;
			Msg.Empty();
			if (DiagData.data_check) {
				Msg = _T("課税売上割合に準ずる割合を適用する為に、\n");
				Msg += _T("課税売上割合を変更される場合は・・・[はい]を、\n");
				Msg += _T("それ以外の理由で変更される場合は・・・[いいえ]を、\n");
				Msg += _T("選択してください。");
				int st = ICSMessageBox(Msg, MB_YESNO, 0);
				if (st == IDYES) {
					if ((*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE) {
					}
					else {
					}

					(*m_pSnHeadData)->Sn_UPERS |= 0x02;//
				}
			}
			else {
				if ((*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE) {
					Msg = "課税売上割合に準ずる割合を無効にします。";
					ICSMessageBox(Msg, MB_OK, 0);
					(*m_pSnHeadData)->Sn_UPERS &= 0xfd;
				}
			}
			if ((DiagData.data_check && (((*m_pSnHeadData)->Sn_UPERS & 0x10) == 0x00)) ||
				((DiagData.data_check == 0) && ((*m_pSnHeadData)->Sn_UPERS & 0x10))) {
				CString		Msg;
				Msg.Empty();
				Msg = _T("課税売上割合を変更する場合は、\n");
				//Msg += _T("個別対応方式欄の⑮～⑰の金額を見直してください。");
				Msg += _T("個別対応方式欄の⑰～⑲の金額を見直してください。");
				ICSMessageBox(Msg, MB_OK, 0);
			}
		}

	}
}


int CShinFhyo2_02::SyukeiDataSet(int cID)
{
	DIAGRAM_DATA	DiagData;
	char			val[MONY_BUF_SIZE];

	diag_getdata(IDC_ICSDIAG8CTRL1, cID, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(val, DiagData.data_val, MONY_BUF_SIZE);

	switch (cID) {
	case ID202_1A:
		memmove(F202_1A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("320101"), F202_1A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("911101"), F202_1A);
		break;
	case ID202_1B:
		memmove(F202_1B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("320102"), F202_1B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("911102"), F202_1B);
		break;
	case ID202_1C:
		memmove(F202_1C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("320103"), F202_1C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("911103"), F202_1C);
		break;
	case ID202_9A:
		memmove(F202_9A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("320901"), F202_9A);
		break;
	case ID202_9B:
		memmove(F202_9B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("320902"), F202_9B);
		break;
	case ID202_9C:
		memmove(F202_9C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("320903"), F202_9C);
		break;
	case ID202_10A:	// 課税仕入れに係る消費税額 税率3%適用分
		if (m_pArith->l_cmp(F202_10A, val)) {
			F202_10c |= 0x08;
			memmove(F202_10A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("321001"), F202_10A);
		}
		break;
	case ID202_10B:	// 課税仕入れに係る消費税額 税率4%適用分
		if (m_pArith->l_cmp(F202_10B, val)) {
			F202_10c |= 0x80;
			memmove(F202_10B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("321002"), F202_10B);
		}
		break;
	case ID202_10C:	// 課税仕入れに係る消費税額 税率6.3%適用分
		if (m_pArith->l_cmp(F202_10C, val)) {
			F202_10c |= 0x800;
			memmove(F202_10C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("321003"), F202_10C);
		}
		break;
	case ID202_11C:
		memmove(F202_11C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321103"), F202_11C);
		break;
	case ID202_12C:	// 特定課税仕入れに係る消費税額 税率6.3%適用分
		if (m_pArith->l_cmp(F202_12C, val)) {
			F202_12c |= 0x800;
			memmove(F202_12C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("321203"), F202_12C);
		}
		break;
/*	case ID202_13A:	// 課税貨物に係る消費税額 税率3%適用分
		memmove(F202_13A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321301"), F202_13A);
		break;
	case ID202_13B:	// 課税貨物に係る消費税額 税率4%適用分
		memmove(F202_13B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321302"), F202_13B);
		break;
*/
	case ID202_13C:	// 課税貨物に係る消費税額 税率6.3%適用分
		memmove(F202_13C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321303"), F202_13C);
		break;

	case ID202_17A:
		memmove(F202_17A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321701"), F202_17A);
		break;
	case ID202_17B:
		memmove(F202_17B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321702"), F202_17B);
		break;
	case ID202_17C:
		memmove(F202_17C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321703"), F202_17C);
		break;
	case ID202_18A:
		memmove(F202_18A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321801"), F202_18A);
		break;
	case ID202_18B:
		memmove(F202_18B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321802"), F202_18B);
		break;
	case ID202_18C:
		memmove(F202_18C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321803"), F202_18C);
		break;
	case ID202_19A:	// 個別対応方式により控除する課税仕入れ等の税額 税率3%適用分
		if (m_pArith->l_cmp(F202_19A, val)) {
			F202_19c |= 0x08;
			memmove(F202_19A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("321901"), F202_19A);
		}
		break;
	case ID202_19B:	// 個別対応方式により控除する課税仕入れ等の税額 税率4%適用分
		if (m_pArith->l_cmp(F202_19B, val)) {
			F202_19c |= 0x80;
			memmove(F202_19B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("321902"), F202_19B);
		}
		break;
	case ID202_19C:	// 個別対応方式により控除する課税仕入れ等の税額 税率6.3%適用分
		if (m_pArith->l_cmp(F202_19C, val)) {
			F202_19c |= 0x800;
			memmove(F202_19C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("321903"), F202_19C);
		}
		break;
	case ID202_20A:	// 一括比例配分方式により控除する課税仕入れ等の税額 税率3%適用分
		if (m_pArith->l_cmp(F202_20A, val)) {
			F202_20c |= 0x08;
			memmove(F202_20A, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("322001"), F202_20A);
		}
		break;
	case ID202_20B:	// 一括比例配分方式により控除する課税仕入れ等の税額 税率4%適用分
		if (m_pArith->l_cmp(F202_20B, val)) {
			F202_20c |= 0x80;
			memmove(F202_20B, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("322002"), F202_20B);
		}
		break;
	case ID202_20C:	// 一括比例配分方式により控除する課税仕入れ等の税額 税率6.3%適用分
		if (m_pArith->l_cmp(F202_20C, val)) {
			F202_20c |= 0x800;
			memmove(F202_20C, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("322003"), F202_20C);
		}
		break;
	case ID202_21A:
		memmove(F202_21A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322101"), F202_21A);
		break;
	case ID202_21B:
		memmove(F202_21B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322102"), F202_21B);
		break;
	case ID202_21C:
		memmove(F202_21C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322103"), F202_21C);
		break;
	case ID202_22A:
		memmove(F202_22A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322201"), F202_22A);
		break;
	case ID202_22B:
		memmove(F202_22B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322202"), F202_22B);
		break;
	case ID202_22C:
		memmove(F202_22C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322203"), F202_22C);
		break;
	case ID202_23A:	// 控除対象仕入税額 税率3%適用分
		if (m_pArith->l_cmp(F202_23A, val)) {
			F202_23c |= 0x08;
			if (m_pArith->l_test(val) < 0) {
				m_pArith->l_neg( val );
				memmove(F202_24A, val, MONY_BUF_SIZE);
				memset(F202_23A, '\0', MONY_BUF_SIZE);
			}
			else {
				memmove(F202_23A, val, MONY_BUF_SIZE);
				memset(F202_24A, '\0', MONY_BUF_SIZE);
			}
			(*m_pSyzSyukei)->SetShinkokuData(_T("322301"), F202_23A);
			(*m_pSyzSyukei)->SetShinkokuData(_T("322401"), F202_24A);
		}
		break;
	case ID202_23B:	// 控除対象仕入税額 税率4%適用分
		if (m_pArith->l_cmp(F202_23B, val)) {
			F202_23c |= 0x80;
			if (m_pArith->l_test(val) < 0) {
				m_pArith->l_neg( val );
				memmove(F202_24B, val, MONY_BUF_SIZE);
				memset(F202_23B, '\0', MONY_BUF_SIZE);
			}
			else {
				memmove(F202_23B, val, MONY_BUF_SIZE);
				memset(F202_24B, '\0', MONY_BUF_SIZE);
			}
			(*m_pSyzSyukei)->SetShinkokuData(_T("322302"), F202_23B);
			(*m_pSyzSyukei)->SetShinkokuData(_T("322402"), F202_24B);
		}
		break;
	case ID202_23C:	// 控除対象仕入税額 税率6.3%適用分
		if (m_pArith->l_cmp(F202_23C, val)) {
			F202_23c |= 0x800;
			if (m_pArith->l_test(val) < 0) {
				m_pArith->l_neg( val );
				memmove(F202_24C, val, MONY_BUF_SIZE);
				memset(F202_23C, '\0', MONY_BUF_SIZE);
			}
			else {
				memmove(F202_23C, val, MONY_BUF_SIZE);
				memset(F202_24C, '\0', MONY_BUF_SIZE);
			}
			(*m_pSyzSyukei)->SetShinkokuData(_T("322303"), F202_23C);
			(*m_pSyzSyukei)->SetShinkokuData(_T("322403"), F202_24C);
		}
		break;
	case ID202_24A:	// 控除過大調整税額 税率3%適用分
		memmove(F202_24A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322401"), F202_24A);
		break;
	case ID202_24B:	// 控除過大調整税額 税率4%適用分
		memmove(F202_24B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322402"), F202_24B);
		break;
	case ID202_24C:	// 控除過大調整税額 税率6.3%適用分
		memmove(F202_24C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322403"), F202_24C);
		break;
	case ID202_25A:
		memmove(F202_25A, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322501"), F202_25A);
		break;
	case ID202_25B:
		memmove(F202_25B, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322502"), F202_25B);
		break;
	case ID202_25C:
		memmove(F202_25C, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322503"), F202_25C);
		break;
	}
	
	return MemoryReget();
}

int CShinFhyo2_02::MemoryReget()
{
	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	// 計算項目を全て再取得
	MoneyBasejagArray	money;

	SYZTOSPC_DATA SyzToSpc = { 0 };

	// 個別対応方式か
	BOOL Kobetsu = ( *m_pSnHeadData )->IsKobetuSiireAnbun();

	int Flg = 0;

	// 課税売上割合に準ずる割合が有効か
	int semiuri = (*m_pSnHeadData)->IsUseSemiUriRatio();

	TanaDataAdd();

	//-->19.11.05
	int		dmysw = 0;
	int		tmpFlg = 0;
	char	VAL[MONY_BUF_SIZE] = { 0 };
	char	ValCnv[MONY_BUF_SIZE] = { 0 };
	tmpFlg = CheckOver500MillionSub(dmysw, VAL, ValCnv);
	if (tmpFlg == 0) {
		Flg = 1;
	}
	//<---


	money = (*m_pSyzSyukei)->GetShinkokuData(_T("32"));
	int cnt = (int)money.GetCount();
	int	incnt = 0;

	//先に５億円判定に必要な部分だけ取得する
//	for (int l = 0; l < cnt; l++) {
//		incnt = (int)money[l].GetCount();
//		for (int m = 0; m < incnt; m++) {
//			// 付表２-１データ
//			if (money[l][m].code == _T("322602"))
//				memmove(F201_1D, money[l][m].arith, MONY_BUF_SIZE);
//			if (money[l][m].code == _T("322603"))
//				memmove(F201_1E, money[l][m].arith, MONY_BUF_SIZE);
//			if (money[l][m].code == _T("322700"))
//				memmove(F201_2F, money[l][m].arith, MONY_BUF_SIZE);
//		}
//	}

	for (int l = 0; l < cnt; l++) {
		incnt = (int)money[l].GetCount();
		for (int m = 0; m < incnt; m++) {
			if (money[l][m].code == _T("320100"))
				memmove(F202_1X, money[l][m].arith, MONY_BUF_SIZE);

			// 5億円判定時に取得
			//---->yoshida191216
			//if (money[l][m].code == _T("320101"))
			//	memmove(F202_1A, money[l][m].arith, MONY_BUF_SIZE);
			//if (money[l][m].code == _T("320102"))
			//	memmove(F202_1B, money[l][m].arith, MONY_BUF_SIZE);
			//if (money[l][m].code == _T("320103"))
			//	memmove(F202_1C, money[l][m].arith, MONY_BUF_SIZE);
			//<-----------------

			if (money[l][m].code == _T("320400"))
				memmove(F202_4X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("320700"))
				memmove(F202_7X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("320800")) {
				char wariai[32];
				m_pArith->l_print( wariai,  money[l][m].arith, _T("SSSSSSSSSSSSS9") );
				F202_8EX = atoi(wariai);
				F202_8X = atoi(wariai);
			}
			if (money[l][m].code == _T("320900"))
				memmove(F202_9X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("320901"))
				memmove(F202_9A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("320902"))
				memmove(F202_9B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("320903"))
				memmove(F202_9C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321001"))
				memmove(F202_10A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321002"))
				memmove(F202_10B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321003"))
				memmove(F202_10C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321000"))
				memmove(F202_10X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321103"))
				memmove(F202_11C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321100"))
				memmove(F202_11X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321203"))
				memmove(F202_12C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321200"))
				memmove(F202_12X, money[l][m].arith, MONY_BUF_SIZE);
			//if (money[l][m].code == _T("321301"))
			//	memmove(F202_13A, money[l][m].arith, MONY_BUF_SIZE);
			//if (money[l][m].code == _T("321302"))
			//	memmove(F202_13B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321303"))
				memmove(F202_13C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321300"))
				memmove(F202_13X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321401"))
				memmove(F202_14A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321402"))
				memmove(F202_14B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321403"))
				memmove(F202_14C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321400"))
				memmove(F202_14X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321501"))
				memmove(F202_15A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321502"))
				memmove(F202_15B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321503"))
				memmove(F202_15C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321500"))
				memmove(F202_15X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321601"))
				memmove(F202_16A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321602"))
				memmove(F202_16B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321603"))
				memmove(F202_16C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321600"))
				memmove(F202_16X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("321701")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F202_17A, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_17A, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("321702")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F202_17B, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_17B, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("321703")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F202_17C, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_17C, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("321700")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F202_17X, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_17X, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("321801")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F202_18A, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_18A, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("321802")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F202_18B, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_18B, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("321803")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F202_18C, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_18C, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("321800")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F202_18X, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_18X, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("321901")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F202_19A, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_19A, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("321902")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F202_19B, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_19B, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("321903")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F202_19C, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_19C, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("321900")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F202_19X, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_19X, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("322001")) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove(F202_20A, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_20A, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("322002")) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove(F202_20B, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_20B, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("322003")) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove(F202_20C, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_20C, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("322000")) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove(F202_20X, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F202_20X, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("322101"))
				memmove(F202_21A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322102"))
				memmove(F202_21B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322103"))
				memmove(F202_21C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322100"))
				memmove(F202_21X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322201"))
				memmove(F202_22A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322202"))
				memmove(F202_22B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322203"))
				memmove(F202_22C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322200"))
				memmove(F202_22X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322301"))
				memmove(F202_23A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322302"))
				memmove(F202_23B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322303"))
				memmove(F202_23C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322300"))
				memmove(F202_23X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322401"))
				memmove(F202_24A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322402"))
				memmove(F202_24B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322403"))
				memmove(F202_24C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322400"))
				memmove(F202_24X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322501"))
				memmove(F202_25A, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322502"))
				memmove(F202_25B, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322503"))
				memmove(F202_25C, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322500"))
				memmove(F202_25X, money[l][m].arith, MONY_BUF_SIZE);
			// 付表２-１データ
//			if (money[l][m].code == _T("322602"))
//				memmove(F201_1D, money[l][m].arith, MONY_BUF_SIZE);
//			if (money[l][m].code == _T("322603"))
//				memmove(F201_1E, money[l][m].arith, MONY_BUF_SIZE);
//			if (money[l][m].code == _T("322700"))
//				memmove(F201_2F, money[l][m].arith, MONY_BUF_SIZE);

			// 特定収入計算表対応
			if ( *( m_pShinInfo )->pSgSpc == 1 ) {
				// 付表2-1の金額を取得

				// -->191127　f202_spc()の中で取得するように変更
				//if ( money[l][m].code == _T( "322600" ) ) {
				//	memmove ( SyzToSpc.SpKzur, money[l][m].arith, MONY_BUF_SIZE );
				//}
				// <--191127

				if ( money[l][m].code == _T( "322700" ) ) {
					m_pArith->l_add ( SyzToSpc.SpMzur, SyzToSpc.SpMzur, money[l][m].arith );
				}
				else if ( money[l][m].code == _T( "322800" ) ) {
					m_pArith->l_add ( SyzToSpc.SpMzur, SyzToSpc.SpMzur, money[l][m].arith );
				}				
				else if ( money[l][m].code == _T( "323100" ) ) {
					memmove ( SyzToSpc.SpHkur, money[l][m].arith, MONY_BUF_SIZE );
				}
				// --> 191127　f202_spc()の中で取得するように変更
				//else if ( money[l][m].code == _T( "322602" ) ) {
				//	memmove ( SyzToSpc.SpKzur624, money[l][m].arith, MONY_BUF_SIZE );
				//}
				//else if ( money[l][m].code == _T( "322603" ) ) {
				//	memmove ( SyzToSpc.SpKzur78, money[l][m].arith, MONY_BUF_SIZE );
				//}
				// <-- 191127

				if ( money[l][m].code == _T( "322900" ) ) {
					memmove ( SyzToSpc.SpKuws, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "323200" ) ) {
					memmove ( SyzToSpc.SpKuwb, money[l][m].arith, MONY_BUF_SIZE );
				}

				//if ( !Flg ) {
				if ( !Flg && !semiuri ) {
					if ( money[l][m].code == _T( "324102" ) ) {
						memmove ( SyzToSpc.SpKgz624, money[l][m].arith, MONY_BUF_SIZE );
					}
					else if ( money[l][m].code == _T( "324103" ) ) {
						memmove ( SyzToSpc.SpKgz78, money[l][m].arith, MONY_BUF_SIZE );
					}
				}
				else {
					if ( Kobetsu ) {
						if ( Flg || semiuri ) {
							if ( money[l][m].code == _T( "324402" ) ) {
								memmove ( SyzToSpc.SpKgz624, money[l][m].arith, MONY_BUF_SIZE );
							}
							else if ( money[l][m].code == _T( "324403" ) ) {
								memmove ( SyzToSpc.SpKgz78, money[l][m].arith, MONY_BUF_SIZE );
							}
						}
					}
					else {
						if ( Flg ) {
							if ( money[l][m].code == _T( "324502" ) ) {
								memmove ( SyzToSpc.SpKgz624, money[l][m].arith, MONY_BUF_SIZE );
							}
							else if ( money[l][m].code == _T( "324503" ) ) {
								memmove ( SyzToSpc.SpKgz78, money[l][m].arith, MONY_BUF_SIZE );
							}
						}
					}
				}
			}
		}
	}

	// 特定収入計算表対応
	if ( *( m_pShinInfo )->pSgSpc == 1 ) {
		f202_spc ( SyzToSpc );
	}

	return 0;
}

//19.11.05
//-----------------------------------------------------------------------------
// ５億円チェック（金額チェック部分）
//-----------------------------------------------------------------------------
// 引数	MilSw	：	５億円チェックサインの保存先
//		pVAL	：	元金額
//		pValCnv	：	変換後金額
//-----------------------------------------------------------------------------
// 返送値		：	FF3000　にセットした値
//-----------------------------------------------------------------------------
int CShinFhyo2_02::CheckOver500MillionSub(int &MilSw, char *pVAL, char *pValCnv)
{
	int		nflg = 0;
	char	flg[MONY_BUF_SIZE] ={ 0 };
	char	tmpVAL[MONY_BUF_SIZE]={ 0 }, tmpValCnv[MONY_BUF_SIZE]={ 0 };


	// 入力ロック時は集計しない
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) {
		//先に５億円判定に必要な部分だけ取得する
		MoneyBasejagArray	money;
		money = (*m_pSyzSyukei)->GetShinkokuData(_T("32"));
		int cnt = (int)money.GetCount();
		int	incnt = 0;

		for (int l = 0; l < cnt; l++) {
			incnt = (int)money[l].GetCount();
			for (int m = 0; m < incnt; m++) {
				// 付表２-１データ
				if (money[l][m].code == _T("322602"))
					memmove(F201_1D, money[l][m].arith, MONY_BUF_SIZE);
				if (money[l][m].code == _T("322603"))
					memmove(F201_1E, money[l][m].arith, MONY_BUF_SIZE);
				if (money[l][m].code == _T("322700"))
					memmove(F201_2F, money[l][m].arith, MONY_BUF_SIZE);

				if (money[l][m].code == _T("320101"))
					memmove(F202_1A, money[l][m].arith, MONY_BUF_SIZE);
				if (money[l][m].code == _T("320102"))
					memmove(F202_1B, money[l][m].arith, MONY_BUF_SIZE);
				if (money[l][m].code == _T("320103"))
					memmove(F202_1C, money[l][m].arith, MONY_BUF_SIZE);

				if (money[l][m].code == _T("320800")) {
					char wariai[32] = {0};
					m_pArith->l_print(wariai, money[l][m].arith, _T("SSSSSSSSSSSSS9"));
					F202_8EX = atoi(wariai);
					F202_8X = atoi(wariai);
				}
			}
		}
	}
	else {
		// 入力ロック時は付表2-1のデータクラスから金額を取得する
		CString tag, chtag;
		tag = _T( "CRB00030" );
		( ( CH31HyoView* )m_pParent )->GetMony ( tag, chtag, F201_1D );

		tag = _T( "CRB00040" );
		( ( CH31HyoView* )m_pParent )->GetMony ( tag, chtag, F201_1E );

		tag = _T( "CRB00060" );
		( ( CH31HyoView* )m_pParent )->GetMony ( tag, chtag, F201_2F );
	}


	m_pArith->l_add(tmpVAL, F202_1A, F202_1B);
	m_pArith->l_add(tmpVAL, tmpVAL, F202_1C);
	m_pArith->l_add(tmpVAL, tmpVAL, F201_1D);	// 8％分軽減加算
	m_pArith->l_add(tmpVAL, tmpVAL, F201_1E);	// 10％分加算
	m_pArith->l_add(tmpVAL, tmpVAL, F201_2F);	// 免税分加算

	int sw = (*m_pSnHeadData)->CheckTaxationSales(tmpVAL, tmpValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno);

	if( sw == 1 ){
		m_pArith->l_input(flg, _T("0"));
		nflg = 0;
	}
	else{
		if( F202_8EX >= 9500){
			m_pArith->l_input(flg, _T("1"));
			nflg = 1;
		}
		else {
			m_pArith->l_input(flg, _T("0"));
			nflg = 0;
		}
	}
	(*m_pSyzSyukei)->SetShinkokuData(_T("FF3000"), flg);
	memmove( pVAL, tmpVAL, MONY_BUF_SIZE );
	memmove( pValCnv, tmpValCnv, MONY_BUF_SIZE );

	MilSw = sw;

	(*m_pSyzSyukei)->GetShinkokuData(_T("FF"));


	return nflg;
}

//処理変更の為カット19.11.05
#ifdef _RSV_
//=====================================
// 課税売上高5億円越えチェック
//=====================================
void CShinFhyo2_02::CheckOver500Million()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char	VAL[6] = {0};
	char	ValCnv[6] = {0};
	char	buf[256] = { 0 };

	memset( VAL, '\0', 6 );
	memset( ValCnv, '\0', 6 );
	memset ( buf, '\0', sizeof ( buf ) );

	m_pArith->l_add(VAL, F202_1A, F202_1B);
	m_pArith->l_add(VAL, VAL, F202_1C);
//	memmove ( VAL, F202_1X, MONY_BUF_SIZE );
	m_pArith->l_add(VAL, VAL, F201_1D);	// 8％分軽減加算
	m_pArith->l_add(VAL, VAL, F201_1E);	// 10％分加算
	m_pArith->l_add(VAL, VAL, F201_2F);	// 免税分加算

	(*m_pSnHeadData)->Sn_Sign4 &= 0xef;

	*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales(VAL, ValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno);

	if( *m_pShinInfo->pOver500MillionSw == 1 ){	// ON:９５％ルール改正対応期間で５億円超え

		//19.11.05
		char flg[MONY_BUF_SIZE] = { 0 };
		m_pArith->l_input(flg, "0");
		(*m_pSyzSyukei)->SetShinkokuData(_T("FF3000"), flg);


		(*m_pSnHeadData)->m_UriRatioBorder = 11000;
		(*m_pSnHeadData)->Sn_Sign4 |= 0x10;
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
			ChangeColor( IDC_ICSDIAG8CTRL1, MILLION, 5 );
			ChangeColor( IDC_ICSDIAG8CTRL1, MILLION + 1, 5 );
			memset( buf, '\0', sizeof( buf ) );

			if( m_pArith->l_cmp( VAL, ValCnv ) == 0 ){
				strcpy_s( buf, sizeof( buf ), "課税売上高が、５億円を超えています。\n" );
			}
			else{
				strcpy_s( buf, sizeof( buf ), "課税売上高が、年換算で５億円を超えています。\n" );
			}
			strcat_s( buf, sizeof( buf ), "比例配分方式の選択も含めて注意してください。" );
			DiagData.data_disp = (char *)buf;
			diag_setdata( IDC_ICSDIAG8CTRL2, 4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
	}
	else{			// ４億５千万以上で５億以下又は４億５千万未満

		//19.11.05
		char flg[MONY_BUF_SIZE] = { 0 };

		if (F202_8EX >= (*m_pSnHeadData)->m_UriRatioBorder) {
			m_pArith->l_input(flg, "1");
		}
		else {
			m_pArith->l_input(flg, "0");
		}
		(*m_pSyzSyukei)->SetShinkokuData(_T("FF3000"), flg);

		(*m_pSnHeadData)->m_UriRatioBorder = 9500;
		*m_pShinInfo->pOver500MillionSw = 0;
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
			ChangeColor( IDC_ICSDIAG8CTRL1, MILLION, 1 );
			ChangeColor( IDC_ICSDIAG8CTRL1, MILLION + 1, 1 );
			diag_clear( IDC_ICSDIAG8CTRL2, 4, TRUE, CIcsdiagctrl );
		}
	}

	m_Fhyo2_02Diag01.Refresh();
	m_Fhyo2_02Diag02.Refresh();
}
#else
//-----------------------------------------------------------------------------
// 課税売上高5億円越えチェック
//-----------------------------------------------------------------------------
void CShinFhyo2_02::CheckOver500Million()
{
	DIAGRAM_ATTRIBUTE DA;
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	COLORREF color;

	char	VAL[MONY_BUF_SIZE] ={ 0 };
	char	ValCnv[MONY_BUF_SIZE] ={ 0 };
	(*m_pSnHeadData)->Sn_Sign4 &= 0xef;
	CheckOver500MillionSub( *(m_pShinInfo->pOver500MillionSw), VAL, ValCnv );

	char	buf[256] ={ 0 };
	if( *m_pShinInfo->pOver500MillionSw == 1 ){	// ON:９５％ルール改正対応期間で５億円超え

		(*m_pSnHeadData)->m_UriRatioBorder = 11000;
		(*m_pSnHeadData)->Sn_Sign4 |= 0x10;
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
			//ChangeColor(IDC_ICSDIAG8CTRL1, MILLION, 5);
			//ChangeColor(IDC_ICSDIAG8CTRL1, MILLION + 1, 5);
			color = BC_GREEN_L;
			memset(buf, '\0', sizeof(buf));

			if( m_pArith->l_cmp(VAL, ValCnv) == 0 ){
				strcpy_s(buf, sizeof(buf), "課税売上高が、５億円を超えています。\n");
			}
			else{
				strcpy_s(buf, sizeof(buf), "課税売上高が、年換算で５億円を超えています。\n");
			}
			strcat_s(buf, sizeof(buf), "比例配分方式の選択も含めて注意してください。");
			DiagData.data_disp = (char *)buf;
			diag_setdata(IDC_ICSDIAG8CTRL2, 4, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
	}
	else{			// ４億５千万以上で５億以下又は４億５千万未満

		(*m_pSnHeadData)->m_UriRatioBorder = 9500;
		*m_pShinInfo->pOver500MillionSw = 0;
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
			//ChangeColor(IDC_ICSDIAG8CTRL1, MILLION, 1);
			//ChangeColor(IDC_ICSDIAG8CTRL1, MILLION + 1, 1);
			color = BC_WHITE;
			diag_clear(IDC_ICSDIAG8CTRL2, 4, TRUE, CIcsdiagctrl);
		}
	}

	//m_Fhyo2_02Diag01.Refresh();
	//m_Fhyo2_02Diag02.Refresh();

	// m_Fhyo2_02Diag02.Refresh()を呼ぶと解像度が1024×768のときに正常に表示されなくなる
	if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) {
		diag_getattr ( IDC_ICSDIAG8CTRL1, MILLION, &DA, CIcsdiagctrl );
		DA.attr_bcolor = color;
		diag_setattr ( IDC_ICSDIAG8CTRL1, MILLION, &DA, TRUE, CIcsdiagctrl );

		diag_getattr ( IDC_ICSDIAG8CTRL1, MILLION + 1, &DA, CIcsdiagctrl );
		DA.attr_bcolor = color;
		diag_setattr ( IDC_ICSDIAG8CTRL1, MILLION + 1, &DA, TRUE, CIcsdiagctrl );
	}
}
#endif

//=====================================
// 棚卸データ（ダイアログ）の加減算を行う
//=====================================
void CShinFhyo2_02::TanaDataAdd()
{
	char	VAL_14A[6] = { 0 }, VAL_14B[6] = { 0 }, VAL_14C[6] = { 0 }, VAL_14D[6] = { 0 }, VAL_14E[6] = { 0 };	// 全体
	char	VAL_17A[6] = { 0 }, VAL_17B[6] = { 0 }, VAL_17C[6] = { 0 }, VAL_17D[6] = { 0 }, VAL_17E[6] = { 0 };	// 課税売上に要するもの
	char	VAL_18A[6] = { 0 }, VAL_18B[6] = { 0 }, VAL_18C[6] = { 0 }, VAL_18D[6] = { 0 }, VAL_18E[6] = { 0 };	// 課税売上と非課税売上に共通して要するもの

	if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {	// 非連動は保存データで使用するので、再計算不要

		if ((*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE) {
			if ((*m_pSnTanaData)->Sn_MENZsw & 0x10) {
				memset(F202_14C, '\0', MONY_BUF_SIZE);
				m_pArith->l_add(F202_14C, F202_14C, (*m_pSnTanaData)->Sn_mnz63[0]);
				m_pArith->l_add(F202_14C, F202_14C, (*m_pSnTanaData)->Sn_mnz63[1]);
				m_pArith->l_add(F202_14C, F202_14C, (*m_pSnTanaData)->Sn_mnz63[2]);
				memset(F202_14B, '\0', MONY_BUF_SIZE);
				m_pArith->l_add(F202_14B, F202_14B, (*m_pSnTanaData)->Sn_mnz40[0]);
				m_pArith->l_add(F202_14B, F202_14B, (*m_pSnTanaData)->Sn_mnz40[1]);
				m_pArith->l_add(F202_14B, F202_14B, (*m_pSnTanaData)->Sn_mnz40[2]);
				memset(F202_14A, '\0', MONY_BUF_SIZE);
				m_pArith->l_add(F202_14A, F202_14A, (*m_pSnTanaData)->Sn_menzz[0]);
				m_pArith->l_add(F202_14A, F202_14A, (*m_pSnTanaData)->Sn_menzz[1]);
				m_pArith->l_add(F202_14A, F202_14A, (*m_pSnTanaData)->Sn_menzz[2]);

				if ((*m_pSnTanaData)->Sn_MENZsw & 0x01) {
					m_pArith->l_neg(F202_14A);
					m_pArith->l_neg(F202_14B);
					m_pArith->l_neg(F202_14C);
				}

				(*m_pSyzSyukei)->SetShinkokuData(_T("321401"), F202_14A);
				(*m_pSyzSyukei)->SetShinkokuData(_T("321402"), F202_14B);
				(*m_pSyzSyukei)->SetShinkokuData(_T("321403"), F202_14C);
			}
			else {
				memset(F202_14A, '\0', MONY_BUF_SIZE);
				memset(F202_14B, '\0', MONY_BUF_SIZE);
				memset(F202_14C, '\0', MONY_BUF_SIZE);

				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321401" ) );
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321402" ) );
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321403" ) );
			}
		}
		else {
			if ((*m_pSnTanaData)->Sn_MENZsw & 0x10) {
				memmove(F202_14A, (*m_pSnTanaData)->Sn_MENZZ, MONY_BUF_SIZE);
				memmove(F202_14B, (*m_pSnTanaData)->Sn_MNZ40, MONY_BUF_SIZE);
				memmove(F202_14C, (*m_pSnTanaData)->Sn_MNZ63, MONY_BUF_SIZE);

				if ((*m_pSnTanaData)->Sn_MENZsw & 0x01) {
					m_pArith->l_neg(F202_14A);
					m_pArith->l_neg(F202_14B);
					m_pArith->l_neg(F202_14C);
				}

				( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), F202_14A );
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), F202_14B );
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), F202_14C );
			}
			else {
				memset(F202_14A, '\0', MONY_BUF_SIZE);
				memset(F202_14B, '\0', MONY_BUF_SIZE);
				memset(F202_14C, '\0', MONY_BUF_SIZE);

				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321401" ) );
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321402" ) );
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321403" ) );
			}
		}
		return;
	}

	//if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
	if( !((*m_pSnTanaData)->Sn_tansw&0x01) && ((*m_pSnHeadData)->IsKobetuSiireAnbun()==TRUE) ){//'20.01.08

		MoneyBasejagArray	money;
		char wariai[32];

		// 連動データを取得
		money = (*m_pSyzSyukei)->GetShinkokuData(_T("323300"));
		m_pArith->l_print(wariai, money[0][0].arith, _T("SSSSSSSSSSSSS9"));
		F202_8EX = atoi(wariai);
		CheckOver500Million();

		if( (*m_pSnTanaData)->Sn_MENZsw & 0x10 ) {
			m_pArith->l_add(VAL_14C, (*m_pSnTanaData)->Sn_mnz63[0], (*m_pSnTanaData)->Sn_mnz63[1]);
			m_pArith->l_add(VAL_14C, VAL_14C, (*m_pSnTanaData)->Sn_mnz63[2]);
			m_pArith->l_add(VAL_14B, (*m_pSnTanaData)->Sn_mnz40[0], (*m_pSnTanaData)->Sn_mnz40[1]);
			m_pArith->l_add(VAL_14B, VAL_14B, (*m_pSnTanaData)->Sn_mnz40[2]);
			m_pArith->l_add(VAL_14A, (*m_pSnTanaData)->Sn_menzz[0], (*m_pSnTanaData)->Sn_menzz[1]);
			m_pArith->l_add(VAL_14A, VAL_14A, (*m_pSnTanaData)->Sn_menzz[2]);

			m_pArith->l_add(VAL_14E, (*m_pSnTanaData)->Sn_mnz78[0], (*m_pSnTanaData)->Sn_mnz78[1]);
			m_pArith->l_add(VAL_14E, VAL_14E, (*m_pSnTanaData)->Sn_mnz78[2]);
			m_pArith->l_add(VAL_14D, (*m_pSnTanaData)->Sn_mnz624[0], (*m_pSnTanaData)->Sn_mnz624[1]);
			m_pArith->l_add(VAL_14D, VAL_14D, (*m_pSnTanaData)->Sn_mnz624[2]);

			// 比例配分用格納場所へ格納
			memmove((*m_pSnTanaData)->Sn_MNZ63, VAL_14C, MONY_BUF_SIZE);
			memmove((*m_pSnTanaData)->Sn_MNZ40, VAL_14B, MONY_BUF_SIZE);
			memmove((*m_pSnTanaData)->Sn_MENZZ, VAL_14A, MONY_BUF_SIZE);

			memmove((*m_pSnTanaData)->Sn_MNZ78, VAL_14E, MONY_BUF_SIZE);
			memmove((*m_pSnTanaData)->Sn_MNZ624, VAL_14D, MONY_BUF_SIZE);

			if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ) {
				m_pArith->l_neg(VAL_14A);
				m_pArith->l_neg(VAL_14B);
				m_pArith->l_neg(VAL_14C);

				m_pArith->l_neg(VAL_14D);
				m_pArith->l_neg(VAL_14E);
			}

			// 個別対応で95％未満の場合
			if( (F202_8EX < (*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE) ) {
				memmove(VAL_17C, (*m_pSnTanaData)->Sn_mnz63[0], MONY_BUF_SIZE);
				memmove(VAL_18C, (*m_pSnTanaData)->Sn_mnz63[2], MONY_BUF_SIZE);
				memmove(VAL_17B, (*m_pSnTanaData)->Sn_mnz40[0], MONY_BUF_SIZE);
				memmove(VAL_18B, (*m_pSnTanaData)->Sn_mnz40[2], MONY_BUF_SIZE);
				memmove(VAL_17A, (*m_pSnTanaData)->Sn_menzz[0], MONY_BUF_SIZE);
				memmove(VAL_18A, (*m_pSnTanaData)->Sn_menzz[2], MONY_BUF_SIZE);

				memmove(VAL_17E, (*m_pSnTanaData)->Sn_mnz78[0], MONY_BUF_SIZE);
				memmove(VAL_18E, (*m_pSnTanaData)->Sn_mnz78[2], MONY_BUF_SIZE);
				memmove(VAL_17D, (*m_pSnTanaData)->Sn_mnz624[0], MONY_BUF_SIZE);
				memmove(VAL_18D, (*m_pSnTanaData)->Sn_mnz624[2], MONY_BUF_SIZE);

				if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ) {
					m_pArith->l_neg(VAL_17C);
					m_pArith->l_neg(VAL_18C);
					m_pArith->l_neg(VAL_17B);
					m_pArith->l_neg(VAL_18B);
					m_pArith->l_neg(VAL_17A);
					m_pArith->l_neg(VAL_18A);

					m_pArith->l_neg(VAL_17E);
					m_pArith->l_neg(VAL_18E);
					m_pArith->l_neg(VAL_17D);
					m_pArith->l_neg(VAL_18D);
				}
			}
		}
		(*m_pSyzSyukei)->SetShinkokuData(_T("321401"), VAL_14A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321402"), VAL_14B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321403"), VAL_14C);

		(*m_pSyzSyukei)->SetShinkokuData(_T("323902"), VAL_14D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323903"), VAL_14E);
	}
	else if( !((*m_pSnTanaData)->Sn_tansw&0x01) && ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
		if( (*m_pSnTanaData)->Sn_MENZsw & 0x10 ) {
			memmove(VAL_14A, (*m_pSnTanaData)->Sn_MENZZ, MONY_BUF_SIZE);
			memmove(VAL_14B, (*m_pSnTanaData)->Sn_MNZ40, MONY_BUF_SIZE);
			memmove(VAL_14C, (*m_pSnTanaData)->Sn_MNZ63, MONY_BUF_SIZE);

			memmove(VAL_14D, (*m_pSnTanaData)->Sn_MNZ624, MONY_BUF_SIZE);
			memmove(VAL_14E, (*m_pSnTanaData)->Sn_MNZ78, MONY_BUF_SIZE);

			if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ) {
				m_pArith->l_neg(VAL_14A);
				m_pArith->l_neg(VAL_14B);
				m_pArith->l_neg(VAL_14C);

				m_pArith->l_neg(VAL_14D);
				m_pArith->l_neg(VAL_14E);
			}
		}
		(*m_pSyzSyukei)->SetShinkokuData(_T("321401"), VAL_14A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321402"), VAL_14B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("321403"), VAL_14C);

		(*m_pSyzSyukei)->SetShinkokuData(_T("323902"), VAL_14D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323903"), VAL_14E);
	}

	(*m_pSyzSyukei)->SetShinkokuData(_T("32A201"), VAL_17A);
	(*m_pSyzSyukei)->SetShinkokuData(_T("32A202"), VAL_17B);
	(*m_pSyzSyukei)->SetShinkokuData(_T("32A203"), VAL_17C);
	(*m_pSyzSyukei)->SetShinkokuData(_T("32A301"), VAL_18A);
	(*m_pSyzSyukei)->SetShinkokuData(_T("32A302"), VAL_18B);
	(*m_pSyzSyukei)->SetShinkokuData(_T("32A303"), VAL_18C);

	(*m_pSyzSyukei)->SetShinkokuData(_T("32A602"), VAL_17D);
	(*m_pSyzSyukei)->SetShinkokuData(_T("32A603"), VAL_17E);
	(*m_pSyzSyukei)->SetShinkokuData(_T("32A702"), VAL_18D);
	(*m_pSyzSyukei)->SetShinkokuData(_T("32A703"), VAL_18E);

}

//===================================================
// 特定収入計算表を加味した控除対象仕入税額の計算
//===================================================
void CShinFhyo2_02::f202_spc ( SYZTOSPC_DATA SpcData )
{
	if ( *( m_pShinInfo->pSgSpc ) == 1 ) {
		unsigned char sign = ( *m_pSnHeadData )->Sn_Sign2;
		MoneyBasejagArray	money;
		int cnt = 0;
		int incnt = 0;

		SYZTOSPC_DATA SyzToSpc = { 0 };
		memmove ( &SyzToSpc, &SpcData, sizeof ( SYZTOSPC_DATA ) );

		BOOL IsRatioOver95 = FALSE;
		BOOL Kobetsu = FALSE;

		if ( F202_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder ) {
			IsRatioOver95 = TRUE;
			SyzToSpc.IsRatioOver95 |= 0x01;
		}

		Kobetsu = ( *m_pSnHeadData )->IsKobetuSiireAnbun();

		//---->yoshida191121
		//--> 191127
		money = (*m_pSyzSyukei)->GetShinkokuData(_T("312200"));
		memmove(SyzToSpc.SpKzur, money[0][0].arith, MONY_BUF_SIZE);

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("312202"));
		memmove(SyzToSpc.SpKzur624, money[0][0].arith, MONY_BUF_SIZE);

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("312203"));
		memmove(SyzToSpc.SpKzur78, money[0][0].arith, MONY_BUF_SIZE);
		//<-- 191127

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("310202"));
		memmove(SyzToSpc.SpKzur4, money[0][0].arith, MONY_BUF_SIZE);

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("310203"));
		memmove(SyzToSpc.SpKzur63, money[0][0].arith, MONY_BUF_SIZE);
		//-----
//		memmove ( SyzToSpc.SpKzur4, F202_1B, MONY_BUF_SIZE );
//		memmove ( SyzToSpc.SpKzur63, F202_1C, MONY_BUF_SIZE );
		//<-----

		// 課税売上割合	
		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "320800" ) );
		memmove ( SyzToSpc.SpUwri, money[0][0].arith, MONY_BUF_SIZE );

		//if ( IsRatioOver95 ) {
		// 課税売上割合95%以上かつ、課税売上割合に準ずる割合を使用していない
		if ( IsRatioOver95 && ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) ) {
			memmove ( SyzToSpc.SpKgz4, F202_16B, MONY_BUF_SIZE );
			memmove ( SyzToSpc.SpKgz63, F202_16C, MONY_BUF_SIZE );
		}
		else {
			if ( Kobetsu ) {
				memmove ( SyzToSpc.SpKgz4, F202_19B, MONY_BUF_SIZE );
				memmove ( SyzToSpc.SpKgz63, F202_19C, MONY_BUF_SIZE );
			}
			else {
				memmove ( SyzToSpc.SpKgz4, F202_20B, MONY_BUF_SIZE );
				memmove ( SyzToSpc.SpKgz63, F202_20C, MONY_BUF_SIZE );
			}
		}

		// 再計算
		( ( CH31HyoView* )m_pParent )->RecalqSkj ( &SyzToSpc );

		// 金額再セット
		char mony[MONY_BUF_SIZE] = { 0 };
		char zero[MONY_BUF_SIZE] = { 0 };

		if ( ( ( *m_pSnHeadData )->Sn_Sign2 & 0x02 ) == 0x00 ) {
			m_SpcSgn = TRUE;

			memset ( zero, '\0', MONY_BUF_SIZE );
			memmove ( mony, SyzToSpc.SpSiz4, sizeof ( mony ) );
			if ( m_pArith->l_test ( mony ) < 0 ) {
				// マイナスの場合符号反転
				m_pArith->l_neg ( mony );

				// 24B欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322402" ), mony );
				// 23B欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322302" ), zero );
				// 付表1-2 4B欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "310602" ), zero );
			}
			else {
				// 24B欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322402" ), zero );
				// 23B欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322302" ), mony );
				// 付表1-2 4B欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "310602" ), mony );
			}
			
			memmove ( mony, SyzToSpc.SpSiz63, sizeof ( mony ) );
			if ( m_pArith->l_test ( mony ) < 0 ) {
				// マイナスの場合符号反転
				m_pArith->l_neg ( mony );

				// 24C欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322403" ), mony );
				// 23C欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322303" ), zero );
				// 付表1-2 4C欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "310603" ), zero );
			}
			else {
				// 24C欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322403" ), zero );
				// 23C欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "322303" ), mony );
				// 付表1-2 4C欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "310603" ), mony );
			}

			memmove ( mony, SyzToSpc.SpSiz624, sizeof ( mony ) );
			if ( m_pArith->l_test ( mony ) < 0 ) {
				// マイナスの場合符号反転
				m_pArith->l_neg ( mony );

				// 付表2-1 24D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324902" ), mony );
				// 付表2-1 23D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324802" ), zero );
				// 付表1-1 4D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312602" ), zero );
			}
			else {
				// 付表2-1 24D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324902" ), zero );
				// 付表2-1 23D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324802" ), mony );
				// 付表1-1 4D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312602" ), mony );
			}

			memmove ( mony, SyzToSpc.SpSiz78, sizeof ( mony ) );
			if ( m_pArith->l_test ( mony ) < 0 ) {
				// マイナスの場合符号反転
				m_pArith->l_neg ( mony );

				// 付表2-1 24E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), mony );
				// 付表2-1 23E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324803" ), zero );
				// 付表1-1 4Ｅ欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312603" ), zero );
			}
			else {
				// 24E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), zero );
				// 23E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324803" ), mony );
				// 付表1-1 4Ｅ欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312603" ), mony );
			}
	
			// 特定収入連動メッセージ表示
			m_Fhyo2_02Diag02.ModifyItem ( ID202_TS, DIAG_MDFY_OPAQUE );

			// 簡易計算式ボタン表示
			m_Button2.ShowWindow ( SW_SHOW );

			for( int index = ID202_23B; index <= ID202_23C; index++ ){
				ATRIB_MOD( IDC_ICSDIAG8CTRL1, index, 0, ( *m_pSnHeadData )->Sn_Sign4, 0 );
			}

		}
		else {
			// 特定収入割合5%未満
			if ( m_SpcSgn ) {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310602" ) );	// 付表1-2 4B欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310603" ) );	// 付表1-2 4C欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322302" ) );	// 23B欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322303" ) );	// 23C欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322402" ) );	// 24B欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322403" ) );	// 24C欄

				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312602" ) );	// 付表1-1 4D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312603" ) );	// 付表1-1 4E欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324802" ) );	// 付表2-1 23D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324803" ) );	// 付表2-1 23E欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324902" ) );	// 付表2-1 24D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324903" ) );	// 付表2-1 24E欄

				F202_23c &= ~0x80;
				F202_23c &= ~0x800;

				// 特定収入連動メッセージ非表示
				m_Fhyo2_02Diag02.ModifyItem ( ID202_TS, DIAG_MDFY_TRANSPARENT );

				// 簡易計算式ボタン非表示
				m_Button2.ShowWindow ( SW_HIDE );

				m_SpcSgn = FALSE;

				for( int index = ID202_23B; index <= ID202_23C; index++ ){
					ATRIB_MOD( IDC_ICSDIAG8CTRL1, index, 1, ( *m_pSnHeadData )->Sn_Sign4, 0 );
				}
			}
			else{
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310602" ) );	// 付表1-2 4B欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310603" ) );	// 付表1-2 4C欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312602" ) );	// 付表1-1 4D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312603" ) );	// 付表1-1 4E欄
			}
		}


		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "32" ) );
		cnt = ( int )money.GetCount();
		incnt = 0;

		for ( int l = 0; l < cnt; l++ ) {
			incnt = ( int )money[l].GetCount();
			for ( int m = 0; m < incnt; m++ ) {
				if ( money[l][m].code == _T( "322302" ) ) {
					memmove ( F202_23B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322303" ) ) {
					memmove ( F202_23C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322300" ) ) {
					memmove ( F202_23X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322402" ) ) {
					memmove ( F202_24B, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322403" ) ) {
					memmove ( F202_24C, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "322400" ) ) {
					memmove ( F202_24X, money[l][m].arith, MONY_BUF_SIZE );
				}
			}
		}
	}
}

void CShinFhyo2_02::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	( ( CH31HyoView* )m_pParent )->SpecPassDlg ( 1 );

}


//=========================================================
// 手入力項目あり？
//---------------------------------------------------------
// 返送値
//	TRUE		:		あり
//	FALSE		:		なし
//=========================================================
BOOL CShinFhyo2_02::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( ( F202_10c & 0x888 ) || ( F202_12c & 0x800 ) || ( F202_19c & 0x888 ) ||
			 ( F202_20c & 0x888 ) || ( F202_23c & 0x888 ) ) {
			ret = TRUE;
		}
	}

	return ret;
}