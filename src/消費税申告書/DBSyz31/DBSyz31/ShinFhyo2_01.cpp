// ShinFhyo2_01.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo2_01.h"
#include "ShinFhyo2_01Idx.h"
#include "H31HyoView.h"

#include "ChangeTax2.h"
#include "ChangeTaxEx2.h"

// CShinFhyo2_01 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo2_01, CSyzBaseDlg)

extern	HINSTANCE g_hInstance;

CShinFhyo2_01::CShinFhyo2_01(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo2_01::IDD, pParent)
	, m_pSnFhyo2_01Data(NULL)
	, m_pSnHeadData(NULL)
	, m_pShinInfo(NULL)
	, m_pSnTanaData(NULL)
	, m_pSyzSyukei(NULL)
	, m_pParent(pParent)
	, m_FrSw(0)
	, m_DpSw(0)
	, m_DlSw(0)
	, m_SpcSgn ( FALSE )
	, m_UwSgn ( TRUE )
{

	memset( F201_1X, '\0', sizeof(F201_1X) );
	memset( F201_1D, '\0', sizeof(F201_1D) );
	memset( F201_1E, '\0', sizeof(F201_1E) );
	memset( F201_1F, '\0', sizeof(F201_1F) );

	memset( F201_2F, '\0', sizeof(F201_2F) );

	memset( F201_3F, '\0', sizeof(F201_3F) );

	memset( F201_4F, '\0', sizeof(F201_4F) );

	memset( F201_5F, '\0', sizeof(F201_5F) );

	memset( F201_6F, '\0', sizeof(F201_6F) );

	memset( F201_7F, '\0', sizeof(F201_7F) );

	//memset( F201_8F, '\0', sizeof(F201_8F) );

	memset( F201_9X, '\0', sizeof(F201_9X) );
	memset( F201_9D, '\0', sizeof(F201_9D) );
	memset( F201_9E, '\0', sizeof(F201_9E) );
	memset( F201_9F, '\0', sizeof(F201_9F) );

	memset( F201_10X, '\0', sizeof(F201_10X) );
	memset( F201_10D, '\0', sizeof(F201_10D) );
	memset( F201_10E, '\0', sizeof(F201_10E) );
	memset( F201_10F, '\0', sizeof(F201_10F) );

	memset( F201_11X, '\0', sizeof(F201_11X) );
	memset( F201_11E, '\0', sizeof(F201_11E) );
	memset( F201_11F, '\0', sizeof(F201_11F) );

	memset( F201_12X, '\0', sizeof(F201_12X) );
	memset( F201_12E, '\0', sizeof(F201_12E) );
	memset( F201_12F, '\0', sizeof(F201_12F) );

	memset( F201_13X, '\0', sizeof(F201_13X) );
	memset( F201_13D, '\0', sizeof(F201_13D) );
	memset( F201_13E, '\0', sizeof(F201_13E) );
	memset( F201_13F, '\0', sizeof(F201_13F) );

	memset( F201_14X, '\0', sizeof(F201_14X) );
	memset( F201_14D, '\0', sizeof(F201_14D) );
	memset( F201_14E, '\0', sizeof(F201_14E) );
	memset( F201_14F, '\0', sizeof(F201_14F) );

	memset( F201_15X, '\0', sizeof(F201_15X) );
	memset( F201_15D, '\0', sizeof(F201_15D) );
	memset( F201_15E, '\0', sizeof(F201_15E) );
	memset( F201_15F, '\0', sizeof(F201_15F) );

	memset( F201_16X, '\0', sizeof(F201_16X) );
	memset( F201_16D, '\0', sizeof(F201_16D) );
	memset( F201_16E, '\0', sizeof(F201_16E) );
	memset( F201_16F, '\0', sizeof(F201_16F) );

	memset( F201_17X, '\0', sizeof(F201_17X) );
	memset( F201_17D, '\0', sizeof(F201_17D) );
	memset( F201_17E, '\0', sizeof(F201_17E) );
	memset( F201_17F, '\0', sizeof(F201_17F) );

	memset( F201_18X, '\0', sizeof(F201_18X) );
	memset( F201_18D, '\0', sizeof(F201_18D) );
	memset( F201_18E, '\0', sizeof(F201_18E) );
	memset( F201_18F, '\0', sizeof(F201_18F) );

	memset( F201_19X, '\0', sizeof(F201_19X) );
	memset( F201_19D, '\0', sizeof(F201_19D) );
	memset( F201_19E, '\0', sizeof(F201_19E) );
	memset( F201_19F, '\0', sizeof(F201_19F) );

	memset( F201_20X, '\0', sizeof(F201_20X) );
	memset( F201_20D, '\0', sizeof(F201_20D) );
	memset( F201_20E, '\0', sizeof(F201_20E) );
	memset( F201_20F, '\0', sizeof(F201_20F) );

	memset( F201_21X, '\0', sizeof(F201_21X) );
	memset( F201_21D, '\0', sizeof(F201_21D) );
	memset( F201_21E, '\0', sizeof(F201_21E) );
	memset( F201_21F, '\0', sizeof(F201_21F) );

	memset( F201_22X, '\0', sizeof(F201_22X) );
	memset( F201_22D, '\0', sizeof(F201_22D) );
	memset( F201_22E, '\0', sizeof(F201_22E) );
	memset( F201_22F, '\0', sizeof(F201_22F) );

	memset( F201_23X, '\0', sizeof(F201_23X) );
	memset( F201_23D, '\0', sizeof(F201_23D) );
	memset( F201_23E, '\0', sizeof(F201_23E) );
	memset( F201_23F, '\0', sizeof(F201_23F) );

	memset( F201_24X, '\0', sizeof(F201_24X) );
	memset( F201_24D, '\0', sizeof(F201_24D) );
	memset( F201_24E, '\0', sizeof(F201_24E) );
	memset( F201_24F, '\0', sizeof(F201_24F) );

	memset( F201_25X, '\0', sizeof(F201_25X) );
	memset( F201_25D, '\0', sizeof(F201_25D) );
	memset( F201_25E, '\0', sizeof(F201_25E) );
	memset( F201_25F, '\0', sizeof(F201_25F) );

	F201_8F = 0;
	F201_8EX = 0;

	F201_10c = F201_12c = F201_19c = F201_20c = F201_23c = 0;

	m_curwnd = -1;

}

CShinFhyo2_01::~CShinFhyo2_01()
{
}

void CShinFhyo2_01::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo2_01Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo2_01Diag02);
	DDX_Control(pDX, IDC_BUTTON1, m_tanaoroshi);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
}


BEGIN_MESSAGE_MAP(CShinFhyo2_01, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CShinFhyo2_01::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinFhyo2_01::OnBnClickedButton2)
END_MESSAGE_MAP()


// CShinFhyo2_01 メッセージ ハンドラ

BOOL CShinFhyo2_01::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2);
	m_BottomIdx = 275;//コントロール一番最下のDiagのID指定。これで縦スクロール出る

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinFhyo2_01::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo2_01::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo2_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
int CShinFhyo2_01::Init()
{

	if((*m_pSnFhyo2_01Data) == NULL){
		return -1;
	}

	ReadData( m_FrSw );
	m_FrSw = 1;

	// 特定収入連動メッセージ非表示
	m_Fhyo2_01Diag02.ModifyItem ( ID201_TS, DIAG_MDFY_TRANSPARENT );

	// 読み込みデータを集計モジュールにセット
	ReadData2();
	MemoryReget();

	// ダイアグラム初期属性セット
	InitDiagAttr();

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		m_Fhyo2_01Diag01.ModifyItem ( 5, DIAG_MDFY_TRANSPARENT );
	}

	// 令和2年4月1日改正様式を使用するのチェックがONの場合のみ
	// 「経過措置対象・・・」の文言を表示
	if ( !( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) ) {
		m_Fhyo2_01Diag01.ModifyItem ( 4, DIAG_MDFY_TRANSPARENT );
	}

	// 画面表示
	DispFh2_01();

	ChangeBtnDisp();

	SetItemPosition();

	return 0;
}

//-----------------------------------------------------------------------------
// ボタン表示変更
//-----------------------------------------------------------------------------
void CShinFhyo2_01::ChangeBtnDisp()
{

	if( ((*m_pSnHeadData)->m_apno&0xf0) >= 0x50 ){
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			m_tanaoroshi.EnableWindow( FALSE );

			if ( ( *m_pSnHeadData )->Sn_Sign2 & 0x02 ) {
				// 入力ロック時には簡易計算式のボタンを非表示
				m_Button2.EnableWindow( FALSE );
				m_Button2.ShowWindow( SW_HIDE );
			}
		}
		else{
			m_tanaoroshi.EnableWindow( TRUE );
			if( *m_pShinInfo->pSgSpc && !((*m_pSnHeadData)->Sn_Sign2&0x02) ){
				m_Button2.ShowWindow( SW_SHOW );	// 「特定収入の簡易計算式の表示」ボタン
			}
			else{
				m_Button2.ShowWindow( SW_HIDE );
			}
		}
	}
	else{
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) || ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) ) ) {
			m_tanaoroshi.EnableWindow( FALSE );
		}
		else{
			m_tanaoroshi.EnableWindow( TRUE );
		}
		m_Button2.EnableWindow( FALSE );
		m_Button2.ShowWindow( SW_HIDE );
	}

}


//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数					：	
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//		pSnHonpyoData	：	本表データ読込みクラス
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo2_01::InitInfo(CH31SnFhyo2_01Data **pSnFhyo2_01Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CH31SnTanaData **pSnTanaData, CSyzSyukei **pSyzSyukei)
{

	ASSERT( pSnFhyo2_01Data );
	if( pSnFhyo2_01Data == NULL ){
		return -1;
	}
	m_pSnFhyo2_01Data = pSnFhyo2_01Data;
	
	
	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;


	ASSERT( pArith );
	if( pArith == NULL ){
		return -1;
	}
	m_pArith = pArith;


	ASSERT( pShinInfo );
	if( pShinInfo == NULL ){
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT( pSnTanaData );
	if ( pSnTanaData == NULL ) {
		return -1;
	}
	m_pSnTanaData = pSnTanaData;

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
int CShinFhyo2_01::InitDiagAttr()
{
	int cnt = 0;

	while ( 1 ) {
		if ( Fhyo2_01[cnt].Index == 0 )
			break;

		// 確定時は全入力不可
		int Flg = 0;
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Fhyo2_01[cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Fhyo2_01[cnt].UnConnect;

				//yoshida190912
				//非連動の場合には、付表２－１には特例より転記しないので処理カット
//				if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) || ( ( *m_pSnHeadData )->m_DispTabSgn & 0x20 ) ) 
//					&& ( ( Fhyo2_01[cnt].Index == ID201_1D ) || ( Fhyo2_01[cnt].Index == ID201_1E ) ) ) {
//					Flg = 0;
//				}
//				else {
					int st = ChkTokureiIdx ( Fhyo2_01[cnt].Index );
					if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) && ( st == 1 ) ) {
						Flg = 0;
					}
					else {
						st = ChkTkkzIdx ( Fhyo2_01[cnt].Index );
						if ( st == 1 ) {
							if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) {
								Flg = 1;
							}
							else {
								Flg = 0;
							}
						}
					}
//				}
			}
			else {
				Flg = Fhyo2_01[cnt].Connect;
			}
		}
		if( ((*m_pSnHeadData)->Sn_EXP100&0x01) && *m_pShinInfo->pSgSpc && (Fhyo2_01[cnt].Index==ID201_8EX) ){
			Flg = 0;
		}

		if ( m_SpcSgn ) {
			if ( ( Fhyo2_01[cnt].Index == ID201_23D ) || ( Fhyo2_01[cnt].Index == ID201_23E ) ) {
				Flg = 0;
			}
		}
		
		ATRIB_MOD ( IDC_ICSDIAG8CTRL2, Fhyo2_01[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Fhyo2_01[cnt].Connect );
		cnt++;

	}

	m_Fhyo2_01Diag02.EnableDelete ( 0 );

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
//============================================================
int CShinFhyo2_01::ChkTkkzIdx(short idx)
{
	int rSt = 0;

	if ( ( ID201_11X <= idx ) && ( idx <= ID201_12F ) )
		rSt = 1;

	return rSt;
}

//============================================================
// 特例計算に関連するインデックス
//------------------------------------------------------------
// 引数
//		idx		:	チェック対象
//------------------------------------------------------------
// 返送値
//		0		:	特例計算に関連しない
//		1		:	特例計算で入力可能になるインデックス
//============================================================
int CShinFhyo2_01::ChkTokureiIdx(short idx)
{
	int rSt = 0;

	if ( ( ID201_9D <= idx ) && ( idx <= ID201_13F ) )
		rSt = 1;

	return rSt;
}

//===================================================
// 初期ポジションセット
//===================================================
void CShinFhyo2_01::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		//index = m_Fhyo2_01Diag02.GetPosition();
		if( (index = m_Fhyo2_01Diag02.GetPosition()) != -1) {
			m_Fhyo2_01Diag02.SetPosition(index);
			m_Fhyo2_01Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo2_01Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット
	m_Fhyo2_01Diag02.SetFocus();

	if ( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ) {
		if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) || ( ( *m_pSnHeadData )->m_DispTabSgn & 0x20 ) ) ) {
			// 非課税売上額
			m_Fhyo2_01Diag02.SetPosition( ID201_2F );
		}
		else {
			// 課税売上額
			m_Fhyo2_01Diag02.SetPosition( ID201_1D );
		}
	}


	//SetRedrawScroll( 0 );
}

//----------------------------------------------
// データの書込み
//----------------------------------------------
// 引数		pas	：
//----------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//----------------------------------------------
int CShinFhyo2_01::WriteData( int pas )
{
	int st = 0;

	// 1.課税売上額(税抜き)
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_1X, F201_1X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_1D, F201_1D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_1E, F201_1E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_1F, F201_1F, MONY_BUF_SIZE );

	// 2.免税売上額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_2F, F201_2F, MONY_BUF_SIZE );

	// 3.非課税資産の輸出等の金額…
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_3F, F201_3F, MONY_BUF_SIZE );

	// 4.課税資産の譲渡等の対価の額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_4F, F201_4F, MONY_BUF_SIZE );

	// 5.課税資産の譲渡等の対価の額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_5F, F201_5F, MONY_BUF_SIZE );

	// 6.非課税売上額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_6F, F201_6F, MONY_BUF_SIZE );

	// 7.課税資産の譲渡等の対価の額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_7F, F201_7F, MONY_BUF_SIZE );

	// 8.課税売上割合
	( *m_pSnFhyo2_01Data )->Sn_F201_8EX = F201_8EX;
	( *m_pSnFhyo2_01Data )->Sn_F201_8F_UW = F201_8F;
	(*m_pSnHeadData)->Sn_UPERS &= 0xfe;
	if (((*m_pSnFhyo2_01Data)->Sn_F201_8EX) < (*m_pSnHeadData)->m_UriRatioBorder)
		(*m_pSnHeadData)->Sn_UPERS |= 0x01;

	// 9.課税仕入れに係る支払い対価の額(税込み)
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_9X, F201_9X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_9D, F201_9D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_9E, F201_9E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_9F, F201_9F, MONY_BUF_SIZE );

	// 10.課税仕入れに係る消費税額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_10X, F201_10X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_10D, F201_10D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_10E, F201_10E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_10F, F201_10F, MONY_BUF_SIZE );
	(*m_pSnFhyo2_01Data)->Sn_F201_10sw = F201_10c;

	// 11.特定課税仕入れに係る支払対価の額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_11X, F201_11X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_11E, F201_11E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_11F, F201_11F, MONY_BUF_SIZE );

	// 12.特定課税仕入れに係る消費税額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_12X, F201_12X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_12E, F201_12E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_12F, F201_12F, MONY_BUF_SIZE );
	(*m_pSnFhyo2_01Data)->Sn_F201_12sw = F201_12c;

	// 13.課税貨物に係る消費税額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_13X, F201_13X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_13D, F201_13D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_13E, F201_13E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_13F, F201_13F, MONY_BUF_SIZE );

	// 14.納税義務の免除を受けない(受ける)こととなった場合の…
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_14X, F201_14X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_14D, F201_14D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_14E, F201_14E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_14F, F201_14F, MONY_BUF_SIZE );

	// 15.課税仕入れ等の税額の合計額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_15X, F201_15X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_15D, F201_15D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_15E, F201_15E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_15F, F201_15F, MONY_BUF_SIZE );

	// 16.課税売上高が5億円以下、課税売上割合が95%以上の場合
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_16X, F201_16X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_16D, F201_16D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_16E, F201_16E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_16F, F201_16F, MONY_BUF_SIZE );

	// 17.課税売上にのみ要するもの
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_17X, F201_17X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_17D, F201_17D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_17E, F201_17E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_17F, F201_17F, MONY_BUF_SIZE );

	// 18.課税売上と非課税売上に共通して要するもの
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_18X, F201_18X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_18D, F201_18D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_18E, F201_18E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_18F, F201_18F, MONY_BUF_SIZE );

	// 19.個別対応方式により控除する課税仕入れ等の税額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_19X, F201_19X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_19D, F201_19D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_19E, F201_19E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_19F, F201_19F, MONY_BUF_SIZE );
	(*m_pSnFhyo2_01Data)->Sn_F201_19sw = F201_19c;

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_20X, F201_20X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_20D, F201_20D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_20E, F201_20E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_20F, F201_20F, MONY_BUF_SIZE );
	(*m_pSnFhyo2_01Data)->Sn_F201_20sw = F201_20c;

	// 21.課税売上割合変動時の調整対象固定資産に係る消費税額の調整額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_21X, F201_21X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_21D, F201_21D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_21E, F201_21E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_21F, F201_21F, MONY_BUF_SIZE );

	// 22.調整対象固定資産を課税業務用に転用した場合の調整額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_22X, F201_22X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_22D, F201_22D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_22E, F201_22E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_22F, F201_22F, MONY_BUF_SIZE );

	// 23.控除対象仕入税額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_23X, F201_23X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_23D, F201_23D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_23E, F201_23E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_23F, F201_23F, MONY_BUF_SIZE );
	 (*m_pSnFhyo2_01Data)->Sn_F201_23sw = F201_23c;

	// 24.控除過大調整税額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_24X, F201_24X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_24D, F201_24D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_24E, F201_24E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_24F, F201_24F, MONY_BUF_SIZE );

	// 25.貸倒回収に係る消費税額
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_25X, F201_25X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_25D, F201_25D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_25E, F201_25E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo2_01Data)->Sn_F201_25F, F201_25F, MONY_BUF_SIZE );

	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_26X, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_26D, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_26E, '\0', MONY_BUF_SIZE );
	memset ( ( *m_pSnFhyo2_01Data )->Sn_F201_26F, '\0', MONY_BUF_SIZE );


	return st;
}

//----------------------------------------------
// データの読込み
//----------------------------------------------
// 引数		pas	：
//----------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//----------------------------------------------
int CShinFhyo2_01::ReadData( int pas )
{

	// 1.課税売上額(税抜き)
	memmove( F201_1X, (*m_pSnFhyo2_01Data)->Sn_F201_1X, MONY_BUF_SIZE );
	memmove( F201_1D, (*m_pSnFhyo2_01Data)->Sn_F201_1D, MONY_BUF_SIZE );
	memmove( F201_1E, (*m_pSnFhyo2_01Data)->Sn_F201_1E, MONY_BUF_SIZE );
	memmove( F201_1F, (*m_pSnFhyo2_01Data)->Sn_F201_1F, MONY_BUF_SIZE );

	// 2.免税売上額
	memmove( F201_2F, (*m_pSnFhyo2_01Data)->Sn_F201_2F, MONY_BUF_SIZE );

	// 3.非課税資産の輸出等の金額…
	memmove( F201_3F, (*m_pSnFhyo2_01Data)->Sn_F201_3F, MONY_BUF_SIZE );

	// 4.課税資産の譲渡等の対価の額
	memmove( F201_4F, (*m_pSnFhyo2_01Data)->Sn_F201_4F, MONY_BUF_SIZE );

	// 5.課税資産の譲渡等の対価の額
	memmove( F201_5F, (*m_pSnFhyo2_01Data)->Sn_F201_5F, MONY_BUF_SIZE );

	// 6.非課税売上額
	memmove( F201_6F, (*m_pSnFhyo2_01Data)->Sn_F201_6F, MONY_BUF_SIZE );

	// 7.資産の譲渡等の対価の額
	memmove( F201_7F, (*m_pSnFhyo2_01Data)->Sn_F201_7F, MONY_BUF_SIZE );

	// 8.課税売上割合
	//memmove( F201_8F, (*m_pSnFhyo2_01Data)->Sn_F201_8F, MONY_BUF_SIZE );
	F201_8EX = (unsigned short)(*m_pSnFhyo2_01Data)->Sn_F201_8EX;
	F201_8F = (*m_pSnFhyo2_01Data)->Sn_F201_8F_UW;

	// 9.課税仕入れに係る支払い対価の額(税込み)
	memmove( F201_9X, (*m_pSnFhyo2_01Data)->Sn_F201_9X, MONY_BUF_SIZE );
	memmove( F201_9D, (*m_pSnFhyo2_01Data)->Sn_F201_9D, MONY_BUF_SIZE );
	memmove( F201_9E, (*m_pSnFhyo2_01Data)->Sn_F201_9E, MONY_BUF_SIZE );
	memmove( F201_9F, (*m_pSnFhyo2_01Data)->Sn_F201_9F, MONY_BUF_SIZE );

	// 10.課税仕入れに係る消費税額
	memmove( F201_10X, (*m_pSnFhyo2_01Data)->Sn_F201_10X, MONY_BUF_SIZE );
	memmove( F201_10D, (*m_pSnFhyo2_01Data)->Sn_F201_10D, MONY_BUF_SIZE );
	memmove( F201_10E, (*m_pSnFhyo2_01Data)->Sn_F201_10E, MONY_BUF_SIZE );
	memmove( F201_10F, (*m_pSnFhyo2_01Data)->Sn_F201_10F, MONY_BUF_SIZE );
	F201_10c = (*m_pSnFhyo2_01Data)->Sn_F201_10sw;

	// 11.特定課税仕入れに係る支払対価の額
	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		memmove( F201_11X, (*m_pSnFhyo2_01Data)->Sn_F201_11X, MONY_BUF_SIZE );
		memmove( F201_11E, (*m_pSnFhyo2_01Data)->Sn_F201_11E, MONY_BUF_SIZE );
		memmove( F201_11F, (*m_pSnFhyo2_01Data)->Sn_F201_11F, MONY_BUF_SIZE );
	}
	else{
		memset( F201_11X, '\0', MONY_BUF_SIZE );
		memset( F201_11E, '\0', MONY_BUF_SIZE );
		memset( F201_11F, '\0', MONY_BUF_SIZE );
	}

	// 12.特定課税仕入れに係る消費税額
	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		memmove( F201_12X, (*m_pSnFhyo2_01Data)->Sn_F201_12X, MONY_BUF_SIZE );
		memmove( F201_12E, (*m_pSnFhyo2_01Data)->Sn_F201_12E, MONY_BUF_SIZE );
		memmove( F201_12F, (*m_pSnFhyo2_01Data)->Sn_F201_12F, MONY_BUF_SIZE );
		F201_12c = (*m_pSnFhyo2_01Data)->Sn_F201_12sw;
	}
	else{
		memset( F201_12X, '\0', MONY_BUF_SIZE );
		memset( F201_12E, '\0', MONY_BUF_SIZE );
		memset( F201_12F, '\0', MONY_BUF_SIZE );
		F201_12c = 0;
	}

	// 13.課税貨物に係る消費税額
	memmove( F201_13X, (*m_pSnFhyo2_01Data)->Sn_F201_13X, MONY_BUF_SIZE );
	memmove( F201_13D, (*m_pSnFhyo2_01Data)->Sn_F201_13D, MONY_BUF_SIZE );
	memmove( F201_13E, (*m_pSnFhyo2_01Data)->Sn_F201_13E, MONY_BUF_SIZE );
	memmove( F201_13F, (*m_pSnFhyo2_01Data)->Sn_F201_13F, MONY_BUF_SIZE );

	// 14.納税義務の免除を受けない(受ける)こととなった場合の…
/*
	if( (*m_pSnTanaData)->Sn_MENZsw&0x10 ){
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){
			//?
		}
		else{

			memmove( F201_14X, (*m_pSnTanaData)->Sn_MENZZ, MONY_BUF_SIZE );
			memmove( F201_14D, (*m_pSnTanaData)->Sn_MNZ40, MONY_BUF_SIZE );
			memmove( F201_14E, (*m_pSnTanaData)->Sn_MNZ63, MONY_BUF_SIZE );
			memmove( F201_14F, (*m_pSnTanaData)->Sn_MNZ63, MONY_BUF_SIZE );

		}


		if( (*m_pSnTanaData)->Sn_MENZsw&0x01 ){
			m_pArith->l_neg( F201_14X );
			m_pArith->l_neg( F201_14D );
			m_pArith->l_neg( F201_14E );
			m_pArith->l_neg( F201_14F );
		}

	}
	else{
		memset( F201_14X, '\0', MONY_BUF_SIZE );
		memset( F201_14D, '\0', MONY_BUF_SIZE );
		memset( F201_14E, '\0', MONY_BUF_SIZE );
		memset( F201_14F, '\0', MONY_BUF_SIZE );

	}
*/
	memmove( F201_14X, (*m_pSnFhyo2_01Data)->Sn_F201_14X, MONY_BUF_SIZE );
	memmove( F201_14D, (*m_pSnFhyo2_01Data)->Sn_F201_14D, MONY_BUF_SIZE );
	memmove( F201_14E, (*m_pSnFhyo2_01Data)->Sn_F201_14E, MONY_BUF_SIZE );
	memmove( F201_14F, (*m_pSnFhyo2_01Data)->Sn_F201_14F, MONY_BUF_SIZE );

	// 15.課税仕入れ等の税額の合計額
	memmove( F201_15X, (*m_pSnFhyo2_01Data)->Sn_F201_15X, MONY_BUF_SIZE );
	memmove( F201_15D, (*m_pSnFhyo2_01Data)->Sn_F201_15D, MONY_BUF_SIZE );
	memmove( F201_15E, (*m_pSnFhyo2_01Data)->Sn_F201_15E, MONY_BUF_SIZE );
	memmove( F201_15F, (*m_pSnFhyo2_01Data)->Sn_F201_15F, MONY_BUF_SIZE );

	// 16.課税売上高が5億円以下、課税売上割合が95%以上の場合
	memmove( F201_16X, (*m_pSnFhyo2_01Data)->Sn_F201_16X, MONY_BUF_SIZE );
	memmove( F201_16D, (*m_pSnFhyo2_01Data)->Sn_F201_16D, MONY_BUF_SIZE );
	memmove( F201_16E, (*m_pSnFhyo2_01Data)->Sn_F201_16E, MONY_BUF_SIZE );
	memmove( F201_16F, (*m_pSnFhyo2_01Data)->Sn_F201_16F, MONY_BUF_SIZE );

	// 17.課税売上にのみ要するもの
	memmove( F201_17X, (*m_pSnFhyo2_01Data)->Sn_F201_17X, MONY_BUF_SIZE );
	memmove( F201_17D, (*m_pSnFhyo2_01Data)->Sn_F201_17D, MONY_BUF_SIZE );
	memmove( F201_17E, (*m_pSnFhyo2_01Data)->Sn_F201_17E, MONY_BUF_SIZE );
	memmove( F201_17F, (*m_pSnFhyo2_01Data)->Sn_F201_17F, MONY_BUF_SIZE );

	// 18.課税売上と非課税売上に共通して要するもの
	memmove( F201_18X, (*m_pSnFhyo2_01Data)->Sn_F201_18X, MONY_BUF_SIZE );
	memmove( F201_18D, (*m_pSnFhyo2_01Data)->Sn_F201_18D, MONY_BUF_SIZE );
	memmove( F201_18E, (*m_pSnFhyo2_01Data)->Sn_F201_18E, MONY_BUF_SIZE );
	memmove( F201_18F, (*m_pSnFhyo2_01Data)->Sn_F201_18F, MONY_BUF_SIZE );


	F201_19c = F201_20c = 0x00;
	// 19.個別対応方式により控除する課税仕入れ等の税額
	memmove( F201_19X, (*m_pSnFhyo2_01Data)->Sn_F201_19X, MONY_BUF_SIZE );
	if( m_pArith->l_test((*m_pSnFhyo2_01Data)->Sn_F201_19D) ){
		memmove( F201_19D, (*m_pSnFhyo2_01Data)->Sn_F201_19D, MONY_BUF_SIZE );
		//F201_19c |= 0x01;
	}
	if( m_pArith->l_test((*m_pSnFhyo2_01Data)->Sn_F201_19E) ){
		memmove( F201_19E, (*m_pSnFhyo2_01Data)->Sn_F201_19E, MONY_BUF_SIZE );
		//F201_19c |= 0x10;
	}
	memmove( F201_19F, (*m_pSnFhyo2_01Data)->Sn_F201_19F, MONY_BUF_SIZE );
	F201_19c = (*m_pSnFhyo2_01Data)->Sn_F201_19sw;

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	memmove( F201_20X, (*m_pSnFhyo2_01Data)->Sn_F201_20X, MONY_BUF_SIZE );
	if( m_pArith->l_test((*m_pSnFhyo2_01Data)->Sn_F201_20D) ){
		memmove( F201_20D, (*m_pSnFhyo2_01Data)->Sn_F201_20D, MONY_BUF_SIZE );
		//F201_20c |= 0x01;
	}
	if( m_pArith->l_test((*m_pSnFhyo2_01Data)->Sn_F201_20E) ){
		memmove( F201_20E, (*m_pSnFhyo2_01Data)->Sn_F201_20E, MONY_BUF_SIZE );
		//F201_20c = 0x10;
	}
	memmove( F201_20F, (*m_pSnFhyo2_01Data)->Sn_F201_20F, MONY_BUF_SIZE );
	F201_20c = (*m_pSnFhyo2_01Data)->Sn_F201_20sw;

	// 21.課税売上割合変動時の調整対象固定資産に係る消費税額の調整額
	memmove( F201_21X, (*m_pSnFhyo2_01Data)->Sn_F201_21X, MONY_BUF_SIZE );
	memmove( F201_21D, (*m_pSnFhyo2_01Data)->Sn_F201_21D, MONY_BUF_SIZE );
	memmove( F201_21E, (*m_pSnFhyo2_01Data)->Sn_F201_21E, MONY_BUF_SIZE );
	memmove( F201_21F, (*m_pSnFhyo2_01Data)->Sn_F201_21F, MONY_BUF_SIZE );

	// 22.調整対象固定資産を課税業務用に転用した場合の調整額
	memmove( F201_22X, (*m_pSnFhyo2_01Data)->Sn_F201_22X, MONY_BUF_SIZE );
	memmove( F201_22D, (*m_pSnFhyo2_01Data)->Sn_F201_22D, MONY_BUF_SIZE );
	memmove( F201_22E, (*m_pSnFhyo2_01Data)->Sn_F201_22E, MONY_BUF_SIZE );
	memmove( F201_22F, (*m_pSnFhyo2_01Data)->Sn_F201_22F, MONY_BUF_SIZE );

	// 23.控除対象仕入税額
	F201_23c = (*m_pSnFhyo2_01Data)->Sn_F201_23sw;
	memmove( F201_23X, (*m_pSnFhyo2_01Data)->Sn_F201_23X, MONY_BUF_SIZE );
	memmove( F201_23D, (*m_pSnFhyo2_01Data)->Sn_F201_23D, MONY_BUF_SIZE );
	memmove( F201_23E, (*m_pSnFhyo2_01Data)->Sn_F201_23E, MONY_BUF_SIZE );
	memmove( F201_23F, (*m_pSnFhyo2_01Data)->Sn_F201_23F, MONY_BUF_SIZE );

	// 24.控除過大調整税額
	memmove( F201_24X, (*m_pSnFhyo2_01Data)->Sn_F201_24X, MONY_BUF_SIZE );
	memmove( F201_24D, (*m_pSnFhyo2_01Data)->Sn_F201_24D, MONY_BUF_SIZE );
	memmove( F201_24E, (*m_pSnFhyo2_01Data)->Sn_F201_24E, MONY_BUF_SIZE );
	memmove( F201_24F, (*m_pSnFhyo2_01Data)->Sn_F201_24F, MONY_BUF_SIZE );

	// 25.貸倒回収に係る消費税額
	memmove( F201_25X, (*m_pSnFhyo2_01Data)->Sn_F201_25X, MONY_BUF_SIZE );
	memmove( F201_25D, (*m_pSnFhyo2_01Data)->Sn_F201_25D, MONY_BUF_SIZE );
	memmove( F201_25E, (*m_pSnFhyo2_01Data)->Sn_F201_25E, MONY_BUF_SIZE );
	memmove( F201_25F, (*m_pSnFhyo2_01Data)->Sn_F201_25F, MONY_BUF_SIZE );



	return 0;
}


//-----------------------------------------------------------------------------
// 付表２-０１ 読み込みデータを集計モジュールにセット
//-----------------------------------------------------------------------------
void CShinFhyo2_01::ReadData2()
{
	// 居住用・・・の項目は令和2年4月1日以降のためリセットしておく
	( *m_pSyzSyukei )->ResetShinkokuData ( _T( "327101" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T( "327102" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T( "327103" ) );


	if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {//非連動の場合
		// 特例計算適用時には特例帳票でセットされた金額を取得するため、ここで金額をセットしない
		// 仕様変更で、連動項目ではなくなったため削除
		//if ( ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) == 0x00 ) || ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x30 ) == 0x00 ) ) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("911104"), F201_1D);
			(*m_pSyzSyukei)->SetShinkokuData(_T("911105"), F201_1E);
			(*m_pSyzSyukei)->SetShinkokuData(_T("322602"), F201_1D);
			(*m_pSyzSyukei)->SetShinkokuData(_T("322603"), F201_1E);
		//}

		(*m_pSyzSyukei)->SetShinkokuData(_T("322700"), F201_2F);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322800"), F201_3F);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323100"), F201_6F);

		(*m_pSyzSyukei)->ResetShinkokuData(_T("323300"));//19.12.13


		// 特例計算を行う場合は、9～14の6.24%と7.8%と合計は0とする
		if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) ) {
			char mony[MONY_BUF_SIZE] = { 0 };
			memset( mony, '\0', MONY_BUF_SIZE);

			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323400" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323402" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323403" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323500" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323502" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323503" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323600" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323603" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323700" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323703" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323800" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323802" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323803" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323900" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323902" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323903" ), mony );
		}
		else {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323400" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323402" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323403" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323500" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323502" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323503" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323600" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323603" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323700" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323703" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323800" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323802" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323803" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323900" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323902" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323903" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324002" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324003" ) );

			(*m_pSyzSyukei)->SetShinkokuData(_T("323402"), F201_9D);
			(*m_pSyzSyukei)->SetShinkokuData(_T("323403"), F201_9E);
			if( F201_10c&0x80 )
				(*m_pSyzSyukei)->SetShinkokuData(_T("323502"), F201_10D);
			else
				(*m_pSyzSyukei)->ResetShinkokuData(_T("323502"));
			if( F201_10c&0x800 )
				(*m_pSyzSyukei)->SetShinkokuData(_T("323503"), F201_10E);
			else
				(*m_pSyzSyukei)->ResetShinkokuData(_T("323503"));
			(*m_pSyzSyukei)->SetShinkokuData(_T("323603"), F201_11E);
			if( F201_12c&0x800 )
				(*m_pSyzSyukei)->SetShinkokuData(_T("323703"), F201_12E);
			else
				(*m_pSyzSyukei)->ResetShinkokuData(_T("323703"));
			(*m_pSyzSyukei)->SetShinkokuData(_T("323802"), F201_13D);
			(*m_pSyzSyukei)->SetShinkokuData(_T("323803"), F201_13E);

			(*m_pSyzSyukei)->SetShinkokuData(_T("323902"), F201_14D);
			(*m_pSyzSyukei)->SetShinkokuData(_T("323903"), F201_14E);
		}

		(*m_pSyzSyukei)->SetShinkokuData(_T("324202"), F201_17D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324203"), F201_17E);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324302"), F201_18D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324303"), F201_18E);
		if( F201_19c&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("324402"), F201_19D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324402"));
		if( F201_19c&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("324403"), F201_19E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324403"));

		if( F201_20c&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("324502"), F201_20D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324502"));
		if( F201_20c&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("324503"), F201_20E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324503"));
		(*m_pSyzSyukei)->SetShinkokuData(_T("324602"), F201_21D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324603"), F201_21E);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324702"), F201_22D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324703"), F201_22E);
		if( F201_23c&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("324802"), F201_23D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324802"));
		if( F201_23c&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("324803"), F201_23E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324803"));
		if( F201_23c&0x80 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("324902"), F201_24D);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324902"));
		if( F201_23c&0x800 )
			(*m_pSyzSyukei)->SetShinkokuData(_T("324903"), F201_24E);
		else
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324903"));
		(*m_pSyzSyukei)->SetShinkokuData(_T("325002"), F201_25D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("325003"), F201_25E);

		if ((*m_pSnHeadData)->SVmzsw == 1) {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("322601"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("323401"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("323501"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("323601"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("323701"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("323801"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("323901"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324001"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324101"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324201"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324301"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324401"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324501"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324601"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324701"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324801"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("324901"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("325001"));
		}
		else {
			// 経過措置なし　旧税率合計を０クリア
			char mony[MONY_BUF_SIZE] = { 0 };
			memset(mony, '\0', MONY_BUF_SIZE);

			(*m_pSyzSyukei)->SetShinkokuData(_T("322601"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("323401"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("323501"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("323601"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("323701"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("323801"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("323901"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324001"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324101"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324201"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324301"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324401"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324501"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324601"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324701"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324801"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324901"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("325001"), mony);
		}

		char flg[MONY_BUF_SIZE] = { 0 };
		if ( (*m_pSnHeadData)->Sn_UPERS&0x02 ) m_pArith->l_input(flg, "1");
		(*m_pSyzSyukei)->SetShinkokuData(_T("FF3002"), flg);
	}
	else {
		/*(*m_pSyzSyukei)->SetShinkokuData(_T("323902"), F201_14D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323903"), F201_14E);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324202"), F201_17D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324203"), F201_17E);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324302"), F201_18D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324303"), F201_18E);*/

		// 特例計算を行う場合は、9～14の6.24%と7.8%と合計は0とする
		if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) ) {
			char mony[MONY_BUF_SIZE] = { 0 };
			memset( mony, '\0', MONY_BUF_SIZE);

			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323400" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323402" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323403" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323500" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323502" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323503" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323600" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323603" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323700" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323703" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323800" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323802" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323803" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323900" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323902" ), mony );
			( *m_pSyzSyukei )->SetShinkokuData ( _T ( "323903" ), mony );
		}
		else {
			//yoshida190808
			//Resetすると課税貨物の部分とかが再集計かからない。処理カット。修正依頼できた件
			/*
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323400" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323402" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323403" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323500" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323502" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323503" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323600" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323603" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323700" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323703" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323800" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323802" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323803" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323900" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323902" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323903" ) );
			*/
		}

		(*m_pSyzSyukei)->SetShinkokuData(_T("324602"), F201_21D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324603"), F201_21E);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324702"), F201_22D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324703"), F201_22E);


	}
}

//-----------------------------------------------------------------------------
// 付表２-０１ 画面出力
//-----------------------------------------------------------------------------
void CShinFhyo2_01::DispFh2_01()
{

	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 画面の初期化
	for( int i=4; i<=6; i++ ){
		dsp_cls( IDC_ICSDIAG8CTRL2, (short )i );
	}
	for( int i=ID201_1X; i<=ID201_25F; i++ ){
		dsp_cls( IDC_ICSDIAG8CTRL2, (short )i );
	}


	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){//連動の場合手入力サイン初期化
		//手入力サイン初期化
		F201_10c = 0;
		F201_19c = 0;
		F201_20c = 0;
		F201_23c = 0;
	}

	
	// 期間の表示
	char	buf[512] = {0};
	char	yy, mm, dd;
	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s( buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy&0xff),(mm&0xff),(dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAG8CTRL2, 4, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	sprintf_s( buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy&0xff), (mm&0xff), (dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAG8CTRL2, 5, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 氏名の表示
	memset( buf, '\0', sizeof(buf) );
	if( (*m_pSnHeadData)->IsSoleProprietor() ){
		memmove( buf, (*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO) );
	}
	else{
		memmove( buf, (*m_pSnHeadData)->Sn_CONAM, sizeof((*m_pSnHeadData)->Sn_CONAM) );
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAG8CTRL2, 6, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 課税売上高の5億円超えチェック
	ChangeColor( IDC_ICSDIAG8CTRL2, MILLION, 1 );
	ChangeColor( IDC_ICSDIAG8CTRL2, MILLION + 10, 1 );
	diag_clear( IDC_ICSDIAG8CTRL1, 3, TRUE, CIcsdiagctrl );
	CheckOver500Million();

	// 課税売上額
	f201_1data();
	// 免税売上額
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_2F, F201_2F );
	// 非課税資産の輸出
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_3F, F201_3F );
	// 課税資産の譲渡等の対価の額
	f201_45data();	
	// 非課税売上額
	f201_6data();
	// 資産の譲渡等の対価の額
	f201_7data();
	//課税売上割合
	f201_8data();
	// 課税仕入に係る支払対価の額
	f201_9data();
	// 課税仕入に係る消費税額
	f201_10data();
	//特定課税仕入れに係る支払対価の額
	f201_11data();
	//特定課税仕入れに係る消費税額
	f201_12data();
	// 課税貨物に係る消費税額
	f201_13data();
	// 課税事業⇔免税事業 調整税額
	f201_14data();
	// 課税仕入等の税額の合計額
	f201_15data();
	if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
		// 課税売上割合が９５％未満の場合
		f201_ko();
	}
	else{
		// 課税売上割合が９５％以上の場合
		if( F201_8EX >= (*m_pSnHeadData)->m_UriRatioBorder ){
			f201_16data();
		}
		else{
			// 課税売上割合が９５％未満の場合
			f201_ko();
		}

	}

	// 控除税額の調整(21)
	f201_21data();
	// 控除税額の調整(22)
	f201_22data();
	//	差引控除仕入税額の計算
	f201_2324data();
	// 回収に係る消費税額
	f201_25data();	

	//m_Fhyo2_01Diag02.Refresh();

}


//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//		Mony	：	表示金額
//-----------------------------------------------------------------------------
void CShinFhyo2_01::dsp_prs( unsigned short Id, short Index, char *Mony )
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
void CShinFhyo2_01::dsp_cls( unsigned short Id, short Index )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch( Index ) {
		case 4 :	// 課税期間　自
		case 5 :	// 課税期間　至
		case 6 :	// 氏名又は､名称
		case ID201_8F:
			diag_clear( Id, Index, TRUE, CIcsdiagctrl );
			break;
		default :	// 金額
			memset( DiagData.data_val, '\0', MONY_BUF_SIZE );
			diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			break;
	}
}

// 各入力データ抽出
void CShinFhyo2_01::GetAllDiagData()
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;


	// 1.課税売上額(税抜き)
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_1X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_1X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_1D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_1D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_1E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_1E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_1F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_1F, DiagData.data_val, MONY_BUF_SIZE );

	// 2.免税売上額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_2F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_2F, DiagData.data_val, MONY_BUF_SIZE );

	// 3.非課税資産の輸出等の金額、海外支店等へ移送した資産の価額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_3F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_3F, DiagData.data_val, MONY_BUF_SIZE );

	// 4.課税資産の譲渡等の対価の額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_4F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_4F, DiagData.data_val, MONY_BUF_SIZE );

	// 5.課税資産の譲渡等の対価の額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_5F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_5F, DiagData.data_val, MONY_BUF_SIZE );

	// 6.非課税売上額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_6F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_6F, DiagData.data_val, MONY_BUF_SIZE );

	// 7.資産譲渡等の対価の額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_7F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_7F, DiagData.data_val, MONY_BUF_SIZE );

	// 8.課税売上割合

	// 9.課税仕入れに係る支払い対価の額(税込み)
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_9X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_9X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_9D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_9D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_9E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_9E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_9F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_9F, DiagData.data_val, MONY_BUF_SIZE );

	// 10.課税仕入れに係る消費税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_10X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_10X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_10D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_10D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_10E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_10E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_10F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_10F, DiagData.data_val, MONY_BUF_SIZE );

	// 11.特定課税仕入れに係る支払対価の額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_11X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_11X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_11E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_11E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_11F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_11F, DiagData.data_val, MONY_BUF_SIZE );

	// 12.特定課税仕入れに係る消費税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_12X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_12X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_12E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_12E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_12F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_12F, DiagData.data_val, MONY_BUF_SIZE );

	// 13.課税貨物に係る消費税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_13X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_13X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_13D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_13D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_13E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_13E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_13F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_13F, DiagData.data_val, MONY_BUF_SIZE );

	// 14.納税義務の免除を受ける(受けない)こととなった場合における消費税額の調整額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_14X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_14X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_14D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_14D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_14E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_14E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_14F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_14F, DiagData.data_val, MONY_BUF_SIZE );

	// 15.課税仕入れ等の税額の合計額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_15X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_15X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_15D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_15D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_15E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_15E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_15F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_15F, DiagData.data_val, MONY_BUF_SIZE );

	// 16.課税売上高が5億円以下、割合が95％以上の場合
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_16X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_16X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_16D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_16D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_16E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_16E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_16F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_16F, DiagData.data_val, MONY_BUF_SIZE );

	// 17.15のうち、課税売上げにのみ要するもの
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_17X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_17X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_17D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_17D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_17E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_17E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_17F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_17F, DiagData.data_val, MONY_BUF_SIZE );

	// 18.15のうち、共通して要するもの
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_18X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_18X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_18D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_18D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_18E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_18E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_18F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_18F, DiagData.data_val, MONY_BUF_SIZE );

	// 19.個別対応方式により控除する課税仕入れ等の税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_19X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_19X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_19D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_19D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_19E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_19E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_19F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_19F, DiagData.data_val, MONY_BUF_SIZE );

	// 20.一括比例配分方式により控除する課税仕入れ等の税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_20X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_20X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_20D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_20D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_20E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_20E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_20F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_20F, DiagData.data_val, MONY_BUF_SIZE );

	// 21.課税売上割合変動時の…消費税額の調整額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_21X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_21X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_21D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_21D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_21E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_21E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_21F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_21F, DiagData.data_val, MONY_BUF_SIZE );

	// 22.調整対象固定資産を…調整額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_22X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_22X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_22D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_22D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_22E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_22E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_22F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_22F, DiagData.data_val, MONY_BUF_SIZE );

	// 23.控除対象仕入税額がプラスの時
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_23X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_23X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_23D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_23D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_23E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_23E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_23F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_23F, DiagData.data_val, MONY_BUF_SIZE );

	// 24.控除過大調整額がマイナスの時
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_24X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_24X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_24D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_24D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_24E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_24E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_24F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_24F, DiagData.data_val, MONY_BUF_SIZE );

	// 25.貸倒回収に係る消費税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_25X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_25X, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_25D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_25D, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_25E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_25E, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL2, ID201_25F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F201_25F, DiagData.data_val, MONY_BUF_SIZE );


}

// 課税売上額
void CShinFhyo2_01::f201_1data()
{

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_1X, F201_1X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_1D, F201_1D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_1E, F201_1E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_1F, F201_1F );

}

// 課税資産の譲渡等の対価の額
void CShinFhyo2_01::f201_45data()
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_4F, F201_4F );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_5F, F201_5F );
}

// 非課税売上額
void CShinFhyo2_01::f201_6data()
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_6F, F201_6F );

}

// 資産の譲渡等の対価の額
void CShinFhyo2_01::f201_7data()
{
	char	WORK0[MONY_BUF_SIZE] = {0};

	if( m_pArith->l_test( F201_7F ) < 0 ){
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_7F, FZERO );
	}else{
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_7F, F201_7F );
	}


}

//課税売上割合
void CShinFhyo2_01::f201_8data()
{

	char	WORK0[MONY_BUF_SIZE] = {0};
	char	PW6[MONY_BUF_SIZE] = {0};
	char	buf[512]={0};
	char	PER100BUF[MONY_BUF_SIZE] = {0};
	m_pArith->l_input( PER100BUF, _T("10000") );

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	if( !m_DpSw ){	// 再集計のみ（表示無し）
		// 課税売上高割合
		DiagData.data_check = 0;
		diag_setdata( IDC_ICSDIAG8CTRL2, ID201_8EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		ChangeColor( IDC_ICSDIAG8CTRL2, ID201_8F, 1 );
		diag_modify( IDC_ICSDIAG8CTRL2, ID201_8F, DIAG_MDFY_READONLY, CIcsdiagctrl );
		memset( buf, '\0', sizeof( buf ) );
		memset( WORK0, '\0', MONY_BUF_SIZE );
		if( !((*m_pSnHeadData)->Sn_EXP100&0x01) ){

			/*if( *m_pShinInfo->pSgSpc ){
				// 特定収入有り
				m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F201_4F, (unsigned char *)F201_7F, 2, 0 );

				// 売上割合がマイナスの場合は０％で計算する
				memset( PW6, '\0', sizeof( PW6 ) );
				if( m_pArith->l_cmp(F201_4F, PW6) < 0 ){
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				if( m_pArith->l_cmp(F201_7F, PW6) < 0 ){
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
					memmove( WORK0, PER100BUF, sizeof(WORK0) );
				}

				ChangeColor( IDC_ICSDIAG8CTRL2, ID201_8EX, 1 );
				diag_modify( IDC_ICSDIAG8CTRL2, ID201_8EX, DIAG_MDFY_READONLY, CIcsdiagctrl );
			}*/
			//else{
				if( (*m_pSnHeadData)->Sn_UPERS&0x10 ){
					// 課税売上割合　手入力
					DiagData.data_check = 1;
					diag_setdata( IDC_ICSDIAG8CTRL2, ID201_8EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					sprintf_s( buf, sizeof(buf), _T("%d"), (*m_pSnFhyo2_01Data)->Sn_F201_8F_UW);
					m_pArith->l_input( WORK0, buf );

					//yoshida190710
					(*m_pSyzSyukei)->SetShinkokuData(_T("323300"), WORK0);


					if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
						memset( buf, '\0', sizeof(buf) );
						strcpy_s( buf, sizeof(buf), _T("課　税　売　上　割　合　　(4/7)\n【課税売上割合に準ずる割合を適用】") );
						DiagData.data_disp = buf;
						diag_setdata( IDC_ICSDIAG8CTRL2, 78, (struct IUnknown *)&DiagData, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAG8CTRL2, ID201_8F, 1 );
					}
					else{
						memset( buf, '\0', sizeof(buf) );
						strcpy_s( buf, sizeof(buf), _T("課　税　売　上　割　合　　(4/7)") );
						DiagData.data_disp = buf;
						diag_setdata( IDC_ICSDIAG8CTRL2, 78, (struct IUnknown *)&DiagData, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAG8CTRL2, ID201_8F, 1 );
					}

					if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){
						ChangeColor( IDC_ICSDIAG8CTRL2, ID201_8F, 0 );
						diag_modify( IDC_ICSDIAG8CTRL2, ID201_8F, DIAG_MDFY_EDIT, CIcsdiagctrl );
					}
					else{
						ChangeColor( IDC_ICSDIAG8CTRL2, ID201_8F, 2 );
					}
				}
				else{
					m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F201_4F, (unsigned char *)F201_7F, 2, 0 );
					
					// 売上割合がマイナスの場合は０％で計算する
					memset( PW6, '\0', sizeof(PW6) );
					if( m_pArith->l_cmp(F201_4F, PW6) < 0 ){
						memset( WORK0, '\0', MONY_BUF_SIZE );
					}
					if( m_pArith->l_cmp(F201_7F, PW6) < 0 ){
						memset( WORK0, '\0', MONY_BUF_SIZE );
					}
					if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
						memmove( WORK0, PER100BUF, sizeof(WORK0) );
					}

					memset( buf, '\0', sizeof(buf) );
					strcpy_s( buf, sizeof(buf), _T("課　税　売　上　割　合　　(4/7)") );
					DiagData.data_disp = buf;
					diag_setdata( IDC_ICSDIAG8CTRL2, 78, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAG8CTRL2, ID201_8F, 1 );

					

				}
			//}
		}
		else{
			m_pArith->l_input( WORK0, _T("10000") );

			//yoshida190710
			(*m_pSyzSyukei)->SetShinkokuData(_T("323300"), WORK0);

			ChangeColor( IDC_ICSDIAG8CTRL2, ID201_8EX, 1 );
			diag_modify( IDC_ICSDIAG8CTRL2, ID201_8EX, DIAG_MDFY_READONLY, CIcsdiagctrl );

		}

		memset(buf, '\0', sizeof(buf));
		m_pArith->l_print( buf, WORK0, _T("SSSSS") );
		F201_8EX = atoi( buf );
		m_pArith->l_print( buf, WORK0, _T("SS9.99") );
		DiagData.data_edit = buf;
		diag_setdata( IDC_ICSDIAG8CTRL2, ID201_8F, (struct IUnknown *)&DiagData, CIcsdiagctrl );


		//--->ここの処理原因で、課税売上額打っても即時反映の動きしなかった。yoshida190710
		//ただこの処理を消すと付表２－２に全く反映されなくなってしまう。再度考える

//		if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {//非連動の場合（連動時にセットしてしまうと小数点３位以下が消える）
//			wsprintf(buf, "%d", F201_8EX);
//			m_pArith->l_input(WORK0, buf);
//			(*m_pSyzSyukei)->SetShinkokuData(_T("323300"), WORK0);
//		}

		//------------------
//		if ((*m_pSnHeadData)->Sn_UPERS & 0x10) {
//			wsprintf(buf, "%d", F201_8EX);
//			m_pArith->l_input(WORK0, buf);
//			(*m_pSyzSyukei)->SetShinkokuData(_T("323300"), WORK0);
//		}
		//<----------------
		}
	else{
		// 計算のみ
		memset( buf, '\0', sizeof( buf ) );
		memset( WORK0, '\0', MONY_BUF_SIZE );
		if( !((*m_pSnHeadData)->Sn_EXP100&0x01) ){
			// 課税売上割合
			/*if( *m_pShinInfo->pSgSpc ){
				// 特定収入有り　[08'11.13]
				m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F201_4F, (unsigned char *)F201_7F, 2, 0 );
				// 売上割合がマイナスの場合は０％で計算する。
				memset( PW6, '\0', sizeof( PW6 ) );
				if( m_pArith->l_cmp(F201_4F, PW6) < 0 ){
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				if( m_pArith->l_cmp(F201_7F, PW6) < 0 ){
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
					memmove( WORK0, PER100BUF, sizeof(WORK0) );
				}
			}*/
			//else{
				if( (*m_pSnHeadData)->Sn_UPERS&0x10 ){
					sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo2_01Data)->Sn_F201_8F_UW );
					m_pArith->l_input( WORK0, buf );
				}
				else{
					// 課税売上割合
					m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F201_4F, (unsigned char *)F201_7F, 2, 0 );
					// 売上割合がマイナスの場合は０％で計算する。
					memset( PW6, '\0', sizeof(PW6) );
					if( m_pArith->l_cmp(F201_4F, PW6) < 0 ){
						memset( WORK0, '\0', MONY_BUF_SIZE );
					}
					if( m_pArith->l_cmp(F201_7F, PW6) < 0 ){
						memset( WORK0, '\0', MONY_BUF_SIZE );
					}
					if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
						memmove( WORK0, PER100BUF, sizeof(WORK0) );
					}
				}
			//}
		}
		else{
			m_pArith->l_input( WORK0, _T("10000") );
		}
		m_pArith->l_print( buf, WORK0, _T("SSSSS") );
		F201_8EX = atoi( buf );
		m_pArith->l_print( buf, WORK0, "SS9.99" );
		DiagData.data_edit = buf;
		diag_setdata( IDC_ICSDIAG8CTRL2, ID201_8F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}

}

// 課税仕入に係る支払対価の額
void CShinFhyo2_01::f201_9data()
{

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_9X, F201_9X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_9D, F201_9D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_9E, F201_9E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_9F, F201_9F );

}

// 課税仕入に係る消費税額
void CShinFhyo2_01::f201_10data()
{
	//if ( ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) == 0x00 ) && ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) == 0x00 ) ) {
	if ( ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) == 0x00 ) || ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) == 0x00 ) ) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			ChangeColor( IDC_ICSDIAG8CTRL2, ID201_10D, (F201_10c & 0x80) ? 3 : 0 );
			ChangeColor( IDC_ICSDIAG8CTRL2, ID201_10E, (F201_10c & 0x800) ? 3 : 0 );
		}
	}

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_10X, F201_10X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_10D, F201_10D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_10E, F201_10E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_10F, F201_10F );

}


//特定課税仕入れに係る支払対価の額
void CShinFhyo2_01::f201_11data()
{

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_11X, F201_11X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_11E, F201_11E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_11F, F201_11F );	

}

//特定課税仕入れに係る消費税額
void CShinFhyo2_01::f201_12data()
{
	//if ((((*m_pSnHeadData)->m_DispTabSgn & 0x80) == 0x00) && (((*m_pSnHeadData)->m_DispTabSgn & 0x40) == 0x00)) {
	if ((((*m_pSnHeadData)->m_DispTabSgn & 0x80) == 0x00) || (((*m_pSnHeadData)->m_DispTabSgn & 0x40) == 0x00)) {
		if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
			if ((*m_pSnHeadData)->IsNeedTkkzDisp())
				ChangeColor(IDC_ICSDIAG8CTRL2, ID201_12E, (F201_12c & 0x800) ? 3 : 0);
			else
				ChangeColor(IDC_ICSDIAG8CTRL2, ID201_12E, 1);
		}
	}
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_12X, F201_12X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_12E, F201_12E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_12F, F201_12F );

}

// 課税貨物に係る消費税額
void CShinFhyo2_01::f201_13data()
{

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_13X, F201_13X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_13D, F201_13D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_13E, F201_13E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_13F, F201_13F );
}

// 課税事業⇔免税事業 調整税額
void CShinFhyo2_01::f201_14data()
{

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_14X, F201_14X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_14D, F201_14D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_14E, F201_14E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_14F, F201_14F );

}

// 課税仕入等の税額の合計額
void CShinFhyo2_01::f201_15data()
{

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_15X, F201_15X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_15D, F201_15D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_15E, F201_15E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_15F, F201_15F );

}

// 課税売上割合が９５％以上の場合
void CShinFhyo2_01::f201_16data()
{
	char	WORK0[MONY_BUF_SIZE] = {0};

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_16X, F201_16X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_16D, F201_16D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_16E, F201_16E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_16F, F201_16F );

	memset( F201_17X, '\0', MONY_BUF_SIZE );
	memset( F201_17D, '\0', MONY_BUF_SIZE );
	memset( F201_17E, '\0', MONY_BUF_SIZE );
	memset( F201_17F, '\0', MONY_BUF_SIZE );

	memset( F201_18X, '\0', MONY_BUF_SIZE );
	memset( F201_18D, '\0', MONY_BUF_SIZE );
	memset( F201_18E, '\0', MONY_BUF_SIZE );
	memset( F201_18F, '\0', MONY_BUF_SIZE );

	memset( F201_19X, '\0', MONY_BUF_SIZE );
	memset( F201_19D, '\0', MONY_BUF_SIZE );
	memset( F201_19E, '\0', MONY_BUF_SIZE );
	memset( F201_19F, '\0', MONY_BUF_SIZE );

	memset( F201_20X, '\0', MONY_BUF_SIZE );
	memset( F201_20D, '\0', MONY_BUF_SIZE );
	memset( F201_20E, '\0', MONY_BUF_SIZE );
	memset( F201_20F, '\0', MONY_BUF_SIZE );
	F201_19c = 0;
	F201_20c = 0;

	char mony[MONY_BUF_SIZE] = { 0 };
	memset( mony, '\0', MONY_BUF_SIZE);

	( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321701" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321702" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321703" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321801" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321802" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T ( "321803" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T ( "324202" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T ( "324203" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T ( "324302" ), mony );
	( *m_pSyzSyukei )->SetShinkokuData ( _T ( "324303" ), mony );

	( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "321901" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "321902" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "321903" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "324402" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "324403" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "322001" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "322002" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "322003" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "324502" ) );
	( *m_pSyzSyukei )->ResetShinkokuData ( _T ( "324503" ) );

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_17X, F201_17X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_17D, F201_17D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_17E, F201_17E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_17F, F201_17F );

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_18X, F201_18X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_18D, F201_18D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_18E, F201_18E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_18F, F201_18F );

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_19X, F201_19X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_19D, F201_19D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_19E, F201_19E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_19F, F201_19F );

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_20X, F201_20X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_20D, F201_20D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_20E, F201_20E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_20F, F201_20F );

	// 該当項目入力不可
	// 課税売上割合が95％以上

	ChangeColor( IDC_ICSDIAG8CTRL2, ID201_17D, 1 );
	ChangeColor( IDC_ICSDIAG8CTRL2, ID201_17E, 1 );

	ChangeColor( IDC_ICSDIAG8CTRL2, ID201_18D, 1 );
	ChangeColor( IDC_ICSDIAG8CTRL2, ID201_18E, 1 );

	ChangeColor( IDC_ICSDIAG8CTRL2, ID201_19D, 1 );
	ChangeColor( IDC_ICSDIAG8CTRL2, ID201_19E, 1 );

	ChangeColor( IDC_ICSDIAG8CTRL2, ID201_20D, 1 );
	ChangeColor( IDC_ICSDIAG8CTRL2, ID201_20E, 1 );

	ModifyDiagInput( ID201_17D, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID201_17E, DIAG_MDFY_READONLY );

	ModifyDiagInput( ID201_18D, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID201_18E, DIAG_MDFY_READONLY );

	ModifyDiagInput( ID201_19D, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID201_19E, DIAG_MDFY_READONLY );

	ModifyDiagInput( ID201_20D, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID201_20E, DIAG_MDFY_READONLY );

}

// 課税売上にのみ要する仕入税額
void CShinFhyo2_01::f201_17data()
{

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_17X, F201_17X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_17D, F201_17D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_17E, F201_17E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_17F, F201_17F );
}

// 共通の売上に要する仕入税額
void CShinFhyo2_01::f201_18data()
{

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_18X, F201_18X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_18D, F201_18D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_18E, F201_18E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_18F, F201_18F );
}

// 個別対応方式控除仕入税額
void CShinFhyo2_01::f201_19data()
{

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_19X, F201_19X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_19D, F201_19D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_19E, F201_19E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_19F, F201_19F );

}

// 比例配分方式控除仕入税額
void CShinFhyo2_01::f201_20data()
{

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_20X, F201_20X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_20D, F201_20D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_20E, F201_20E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_20F, F201_20F );


}

// 課税売上割合が９５％未満の場合
void CShinFhyo2_01::f201_ko()
{
	if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){
		dsp_cls( IDC_ICSDIAG8CTRL2, ID201_16X );
		dsp_cls( IDC_ICSDIAG8CTRL2, ID201_16D );
		dsp_cls( IDC_ICSDIAG8CTRL2, ID201_16E );
		dsp_cls( IDC_ICSDIAG8CTRL2, ID201_16F );

		f201_17data();
		f201_18data();
		f201_19data();

		memset( F201_20X, '\0', MONY_BUF_SIZE );
		memset( F201_20D, '\0', MONY_BUF_SIZE );
		memset( F201_20E, '\0', MONY_BUF_SIZE );
		memset( F201_20F, '\0', MONY_BUF_SIZE );
		F201_20c = 0;

		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_20X, F201_20X );
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_20D, F201_20D );
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_20E, F201_20E );
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_20F, F201_20F );

		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ModifyDiagInput( ID201_17D, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID201_17E, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID201_18D, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID201_18E, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID201_19D, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID201_19E, DIAG_MDFY_EDIT );

			ChangeColor( IDC_ICSDIAG8CTRL2, ID201_17D, 0 );
			ChangeColor( IDC_ICSDIAG8CTRL2, ID201_17E, 0 );
			ChangeColor( IDC_ICSDIAG8CTRL2, ID201_18D, 0 );
			ChangeColor( IDC_ICSDIAG8CTRL2, ID201_18E, 0 );
			ChangeColor( IDC_ICSDIAG8CTRL2, ID201_19D, 0 );
			ChangeColor( IDC_ICSDIAG8CTRL2, ID201_19E, 0 );


			if(F201_19c & 0x80){//手入力
				ChangeColor( IDC_ICSDIAG8CTRL2, ID201_19D, 3 );
			}
			if(F201_19c & 0x800){//手入力
				ChangeColor( IDC_ICSDIAG8CTRL2, ID201_19E, 3 );
			}

		}
		ChangeColor( IDC_ICSDIAG8CTRL2, ID201_20D, 1 );
		ChangeColor( IDC_ICSDIAG8CTRL2, ID201_20E, 1 );

		ModifyDiagInput( ID201_20D, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID201_20E, DIAG_MDFY_READONLY );
	}
	else{	// 比例配分

		dsp_cls( IDC_ICSDIAG8CTRL2, ID201_16X );
		dsp_cls( IDC_ICSDIAG8CTRL2, ID201_16D );
		dsp_cls( IDC_ICSDIAG8CTRL2, ID201_16E );
		dsp_cls( IDC_ICSDIAG8CTRL2, ID201_16F );

		f201_20data();

		memset( F201_17X, '\0', MONY_BUF_SIZE );
		memset( F201_17D, '\0', MONY_BUF_SIZE );
		memset( F201_17E, '\0', MONY_BUF_SIZE );
		memset( F201_17F, '\0', MONY_BUF_SIZE );
		memset( F201_18X, '\0', MONY_BUF_SIZE );
		memset( F201_18D, '\0', MONY_BUF_SIZE );
		memset( F201_18E, '\0', MONY_BUF_SIZE );
		memset( F201_18F, '\0', MONY_BUF_SIZE );
		memset( F201_19X, '\0', MONY_BUF_SIZE );
		memset( F201_19D, '\0', MONY_BUF_SIZE );
		memset( F201_19E, '\0', MONY_BUF_SIZE );
		memset( F201_19F, '\0', MONY_BUF_SIZE );
		F201_19c = 0;

		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_17X, F201_17X );
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_17D, F201_17D );
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_17E, F201_17E );
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_17F, F201_17F );
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_18X, F201_18X );
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_18D, F201_18D );
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_18E, F201_18E );
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_18F, F201_18F );
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_19X, F201_19X );
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_19D, F201_19D );
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_19E, F201_19E );
		dsp_prs( IDC_ICSDIAG8CTRL2, ID201_19F, F201_19F );

		ChangeColor( IDC_ICSDIAG8CTRL2, ID201_17D, 1 );
		ChangeColor( IDC_ICSDIAG8CTRL2, ID201_17E, 1 );
		ChangeColor( IDC_ICSDIAG8CTRL2, ID201_18D, 1 );
		ChangeColor( IDC_ICSDIAG8CTRL2, ID201_18E, 1 );
		ChangeColor( IDC_ICSDIAG8CTRL2, ID201_19D, 1 );
		ChangeColor( IDC_ICSDIAG8CTRL2, ID201_19E, 1 );

		ModifyDiagInput( ID201_17D, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID201_17E, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID201_18D, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID201_18E, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID201_19D, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID201_19E, DIAG_MDFY_READONLY );

		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ModifyDiagInput( ID201_20D, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID201_20E, DIAG_MDFY_EDIT );

			ChangeColor( IDC_ICSDIAG8CTRL2, ID201_20D, 0 );
			ChangeColor( IDC_ICSDIAG8CTRL2, ID201_20E, 0 );


			if(F201_20c & 0x80){//手入力
				ChangeColor( IDC_ICSDIAG8CTRL2, ID201_20D, 3 );
			}
			if(F201_20c & 0x800){//手入力
				ChangeColor( IDC_ICSDIAG8CTRL2, ID201_20E, 3 );
			}

		}

	}


}

// 控除税額の調整(21)
void CShinFhyo2_01::f201_21data()
{

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_21X, F201_21X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_21D, F201_21D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_21E, F201_21E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_21F, F201_21F );
}

// 控除税額の調整(22)
void CShinFhyo2_01::f201_22data()
{

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_22X, F201_22X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_22D, F201_22D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_22E, F201_22E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_22F, F201_22F );

}

// 差引控除仕入税額の計算
void CShinFhyo2_01::f201_2324data()
{
	//if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00)) {
	if (((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) && ( m_SpcSgn == FALSE )) {
		ChangeColor( IDC_ICSDIAG8CTRL2, ID201_23D, (F201_23c & 0x80) ? 3 : 0 );
		ChangeColor( IDC_ICSDIAG8CTRL2, ID201_23E, (F201_23c & 0x800) ? 3 : 0 );
	}

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_23X, F201_23X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_23D, F201_23D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_23E, F201_23E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_23F, F201_23F );


	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_24X, F201_24X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_24D, F201_24D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_24E, F201_24E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_24F, F201_24F );
}

// 回収に係る消費税額
void CShinFhyo2_01::f201_25data()
{

	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_25X, F201_25X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_25D, F201_25D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_25E, F201_25E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID201_25F, F201_25F );
}

// 他表からの計算
void CShinFhyo2_01::calq_by_OtherChange()
{
	m_DpSw = -1;

	ReadData( 1 );
	f201data_all( 1 );
	WriteData( 1 );

	m_DpSw = 0;
}

// 特定収入を加味した控除対象仕入税額の計算
void f201_spc()
{

}


//-------------------------------------------------
// 各入力のデータ表示
//-------------------------------------------------
// 引数	atno	：	0：全て
//		sw		：	
//-------------------------------------------------
void CShinFhyo2_01::f201data_all( int atno )
{
	while( 1 ){
		switch( atno ){
			case	0:
			case	1:
				f201_1data();
			case	2:
			case	3:
				f201_45data();
			case	4:
				f201_7data();
				f201_8data();
			case	5:
				f201_9data();
			case	6:
				f201_10data();
			case	15:
				f201_11data();
			case	16:
				f201_12data();
			case	7:
				f201_13data();
				f201_14data();
				f201_15data();
			case	8:
			case	9:		
			case	10:	
			case	11:
				if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
					// 課税売上割合に準ずる割合を有効
					// 課税売上割合が９５％未満
					f201_ko();
				}
				else{
					if( F201_8EX >= (*m_pSnHeadData)->m_UriRatioBorder ){
						// 課税売上割合が９５％以上
						f201_16data();
					}
					else{
						// 課税売上割合が９５％未満
						f201_ko();
					}
				}
			case	12:
				f201_21data();
			case	13:
				f201_22data();
				f201_2324data();
			case	14:
				f201_25data();
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
void CShinFhyo2_01::ModifyDiagInput( short index, int sign )
{
	if( !m_DpSw ){	// 再集計のみ（表示無し）
		diag_modify( IDC_ICSDIAG8CTRL2, index, sign, CIcsdiagctrl );
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
void CShinFhyo2_01::ChangeColor(unsigned short id, short index, int sign)
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
int CShinFhyo2_01::UpDownFocusMoveCheck(int Index, int Move)
{
	int idx = 0;
	int i = 0, j = 0;

	DIAGRAM_ATTRIBUTE DA;

	// 現在ポジション検索
	for ( i = 0; i < FCS_FHY2_01CNT; i++ ) {
		if ( Index == FCS_Fhyo2_01[i].IDNo )
			break;
	}

	// 現在ポジションセット
	idx = FCS_Fhyo2_01[i].IDNo;

	// 次ポジション検索
	if ( Move == 0 ) {			//　下方向
		for ( j = i + 1; j < FCS_FHY2_01CNT; j++ ) {
			m_Fhyo2_01Diag02.GetAttribute ( FCS_Fhyo2_01[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo2_01[j].IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {		//　上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_Fhyo2_01Diag02.GetAttribute ( FCS_Fhyo2_01[j].IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = FCS_Fhyo2_01[j].IDNo;
				break;
			}
		}
	}

	return idx;
}

BEGIN_EVENTSINK_MAP(CShinFhyo2_01, CSyzBaseDlg)
	ON_EVENT(CShinFhyo2_01, IDC_ICSDIAG8CTRL2, 1, CShinFhyo2_01::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo2_01, IDC_ICSDIAG8CTRL2, 2, CShinFhyo2_01::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CShinFhyo2_01, IDC_ICSDIAG8CTRL2, 3, CShinFhyo2_01::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinFhyo2_01, IDC_ICSDIAG8CTRL2, 9, CShinFhyo2_01::CheckButtonIcsdiag8ctrl2, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo2_01::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;

/* 必要？
	switch( index ){
		case ID201_14D:
		case ID201_14E:

			if (m_DlSw) {
				if ((*m_pSnHeadData)->IsUseSemiUriRatio()) {
					// 課税売上割合に準ずる割合を有効
					diag_setposition(IDC_ICSDIAG8CTRL2, ID201_17D, CIcsdiagctrl);
				}
				else {
					if (F201_8EX >= (*m_pSnHeadData)->m_UriRatioBorder) {		// 課税売上高割合が95%以上の場合
						diag_setposition(IDC_ICSDIAG8CTRL2, ID201_21D, CIcsdiagctrl);
					}
					else {				// 課税売上高割合が95%未満の場合	
						if ((*m_pSnHeadData)->IsKobetuSiireAnbun()) {	// 個別対応
							diag_setposition(IDC_ICSDIAG8CTRL2, ID201_17D, CIcsdiagctrl);
						}
						else {										// 比例配分方式
							diag_setposition(IDC_ICSDIAG8CTRL2, ID201_20D, CIcsdiagctrl);
						}
					}
				}
			}
			else {
				diag_setposition(IDC_ICSDIAG8CTRL2, ID201_13E, CIcsdiagctrl);
			}

			break;

		default:
			break;
	}
*/
}

void CShinFhyo2_01::EditOFFIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char	CkMy[MONY_BUF_SIZE];
	int		length;
	int		tmpUWexp = 0;

	char	VAL[MONY_BUF_SIZE] = {0};
	char	ValCnv[MONY_BUF_SIZE] = {0};

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	char	buf[512]={0};

	// 調整額 ???
	if( (index!=ID201_14D) && (index!=ID201_14E) ){
		if( index > ID201_14E ){
			m_DlSw = 1;
		}
		else{
			m_DlSw = 0;
		}
	}

	switch( index ){

		case ID201_1X:			// 課税売上額(税抜き)　旧税率分小計 X
		case ID201_1D:			// 課税売上額(税抜き)　税率6.24％適用分　D
		case ID201_1E:			// 課税売上額(税抜き)　税率7.8％適用分　E

			SyukeiDataSet(index);

			// 課税売上高の５億超えチェック
			CheckOver500Million();

			// 各入力の表示
			f201data_all(1);

			break;

		case ID201_2F:			// 免税売上額　合計 F

			SyukeiDataSet(index);

			// 課税売上高の５億超えチェック
			CheckOver500Million();

			// 各入力の表示
			f201data_all( 2 );
			break;

		case ID201_3F:			// 非課税資産の輸出等の金額  合計 F
			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 3 );
			break;

		case ID201_6F:			// 非課税売上高  合計F
			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 4 );
			break;

		case ID201_9D:			// 課税仕入れに係る支払対価の額(税込み)　税率6.24％適用分　D
		case ID201_9E:			// 課税仕入れに係る支払対価の額(税込み)  税率7.8％適用分　E
			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 5 );
			break;

		case ID201_10D:			// 課税仕入れに係る消費税額　税率6.24％適用分　D
		case ID201_10E:			// 課税仕入れに係る消費税額  税率7.8％適用分　E
			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 6 );
			break;

		case ID201_11E:			// 特定課税仕入れに係る支払対価の額  税率7.8％適用分　E
			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 15 );
			break;

		case ID201_12E:			// 特定課税仕入れに係る消費税額  税率7.8％適用分　E
			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 16 );
			break;

		case ID201_13D:			// 課税貨物に係る消費税額　税率6.24％適用分　D
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F201_13D, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAG8CTRL2, ID201_13D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F201_13D, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(F201_13D, CkMy) ){
				if( (((*m_pSnHeadData)->IsKobetuSiireAnbun()==TRUE)&&(F201_8EX<(*m_pSnHeadData)->m_UriRatioBorder)) || ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) ){		// 個別対応で95％未満の場合
					CString		Msg;
					Msg.Empty();
					Msg =  _T("課税貨物に係る消費税額が変更されました。\n");
					Msg += _T("個別対応方式欄の⑰～⑲の金額を見直してください。");
					ICSMessageBox( Msg, MB_OK, 0 );	// IDOK IDCANCEL
				}
			}

			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 7 );
			break;

		case ID201_13E:			// 課税貨物に係る消費税額  税率7.8％適用分　E
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F201_13E, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAG8CTRL2, ID201_13E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F201_13E, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(F201_13E, CkMy) ){
				if( (((*m_pSnHeadData)->IsKobetuSiireAnbun()==TRUE)&&(F201_8EX<(*m_pSnHeadData)->m_UriRatioBorder)) || ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) ){		// 個別対応で95％未満の場合
					CString		Msg;
					Msg.Empty();
					Msg =  _T("課税貨物に係る消費税額が変更されました。\n");
					Msg += _T("個別対応方式欄の⑰～⑲の金額を見直してください。");
					ICSMessageBox( Msg, MB_OK, 0 );	// IDOK IDCANCEL
				}
			}

			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 7 );
			break;

		case ID201_14D:
		case ID201_14E:
			SyukeiDataSet(index);

			break;

		case ID201_17D:			// 課税売上げにのみ要するもの　税率6.24％適用分　D
		case ID201_17E:			// 課税売上げにのみ要するもの  税率7.8％適用分　E
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F201_8EX>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}

			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 8 );
			break;

		case ID201_18D:			// 課税売上げと非課税売上げに共通して要するもの　税率6.24％適用分　D
		case ID201_18E:			// 課税売上げと非課税売上げに共通して要するもの  税率7.8％適用分　E
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F201_8EX>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}

			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 9 );
			break;

		case ID201_19D:			// 個別対応方式により控除する課税仕入れ等の税額　税率6.24％適用分　D
		case ID201_19E:			// 個別対応方式により控除する課税仕入れ等の税額  税率7.8％適用分　E
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F201_8EX>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}

			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 10 );
			break;

		case ID201_20D:			// 一括比例配分方式により控除する課税仕入れ等の税額　税率6.24％適用分　D
		case ID201_20E:			// 一括比例配分方式により控除する課税仕入れ等の税額  税率7.8％適用分　E
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F201_8EX>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==TRUE) ){
					break;
				}
			}

			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 11 );
			break;

		case ID201_21D:			// 調整対象固定資産に係る調整税額　税率6.24％適用分　D
		case ID201_21E:			// 調整対象固定資産に係る調整税額  税率7.8％適用分　E
			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 12 );
			break;

		case ID201_22D:			// 調整対象固定資産を課税業務用に転用した調整額　税率6.24％適用分　D
		case ID201_22E:			// 調整対象固定資産を課税業務用に転用した調整額  税率7.8％適用分　E
			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 13 );
			break;

		case ID201_23D:			// 控除対象仕入税額　税率6.24％適用分　D
		case ID201_23E:			// 控除対象仕入税額  税率7.8％適用分　E
			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 13 );		
			break;

		case ID201_25D:			// 貸倒回収に係る消費税額　税率6.24％適用分　D
		case ID201_25E:			// 貸倒回収に係る消費税額  税率7.8％適用分　E
			SyukeiDataSet(index);

			// 各入力の表示
			f201data_all( 14 );
			break;

		case ID201_8EX:			// 課税売上割合の変更チェック
			if ( m_UwSgn ) {
				(*m_pSnHeadData)->Sn_UPERS &= 0xef;
				diag_getdata( IDC_ICSDIAG8CTRL2, ID201_8EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				if( DiagData.data_check ){
					(*m_pSnHeadData)->Sn_UPERS |= 0x10;
				}
				//yoshida190710
				else {
					(*m_pSyzSyukei)->ResetShinkokuData(_T("323300"));
				}

				//--->yoshida190710
				f201_8data();
				SyukeiDataSet(index);
				//<----

				// 各入力の表示
				f201data_all( 4 );
			}
			break;

		case ID201_8F:			// 課税売上割合　合計 F
			diag_getdata( IDC_ICSDIAG8CTRL2, ID201_8F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			length = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) ); 
			tmpUWexp = 0;
			if( length ){
				memmove( buf, DiagData.data_edit, length );
			}
			m_Util.perinp( 3, 2, buf, (short *)&tmpUWexp );
			if( tmpUWexp <= 10000 ){
				(*m_pSnFhyo2_01Data)->Sn_F201_8F_UW = tmpUWexp;
			}
			//F201_8F = (*m_pSnFhyo2_01Data)->Sn_F201_8F_UW;
			// 個別対応で95％未満の場合
			if( ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) && (F201_8F!=(*m_pSnFhyo2_01Data)->Sn_F201_8F_UW) ){
				CString		Msg;
				Msg.Empty();
				Msg =  _T("課税売上割合が変更されました。\n");
				//Msg += _T("個別対応方式欄の⑮～⑰の金額を見直してください。");
				Msg += _T("個別対応方式欄の⑰～⑲の金額を見直してください。");
				ICSMessageBox( Msg, MB_OK, 0 );
				F201_8F = (*m_pSnFhyo2_01Data)->Sn_F201_8F_UW;
				f201_8data();
				MemoryReget();
				// 各入力の計算表示
				f201data_all( 4 );
				break;
			}
			else{
				// 個別対応で95％未満の場合
				if( ((*m_pSnHeadData)->IsKobetuSiireAnbun()==TRUE) && (F201_8F!=(*m_pSnFhyo2_01Data)->Sn_F201_8F_UW) && ((*m_pSnFhyo2_01Data)->Sn_F201_8F_UW<(*m_pSnHeadData)->m_UriRatioBorder) ){
					CString		Msg;
					Msg.Empty();
					Msg =  _T("課税売上割合が変更されました。\n");
					//Msg += _T("個別対応方式欄の⑮～⑰の金額を見直してください。");
					Msg += _T("個別対応方式欄の⑰～⑲の金額を見直してください。");
					ICSMessageBox( Msg, MB_OK, 0 );
				}
				F201_8F = (*m_pSnFhyo2_01Data)->Sn_F201_8F_UW;
			}

			//--->yoshida190710
			f201_8data();
			SyukeiDataSet(index);
			//<----

			// 各入力の計算表示
			f201data_all( 4 );

			break;

		default:
			break;

	}
}

void CShinFhyo2_01::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
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
	if( (index!=ID201_14D) && (index!=ID201_14E) ){
		if( index > ID201_14E ){
			m_DlSw = 1;
		}
		else{
			m_DlSw = 0;
		}
	}

	int pos = 0;
	if( wTerm==VK_UP ){
		pos = UpDownFocusMoveCheck(index , 1);
		m_Fhyo2_01Diag02.SetPosition(pos);
	}
	if( wTerm==VK_DOWN ){
		pos = UpDownFocusMoveCheck(index , 0);
		m_Fhyo2_01Diag02.SetPosition(pos);
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
		if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){//非連動の場合
			if( index == ID201_13E ){
				c_set(IDC_BUTTON1);
			}
			else{
				m_Fhyo2_01Diag02.SetNextPosition();
			}
		}
		else{
			if( index == ID201_20E ){
				c_set(IDC_BUTTON1);
			}
			else{
				m_Fhyo2_01Diag02.SetNextPosition();
			}

		}
		
	}
	// 前項目へ
	if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		BOOL IsPrevPosition = TRUE;
		if( *m_pShinInfo->pOver500MillionSw != 1 &&  F201_8EX >= (*m_pSnHeadData)->m_UriRatioBorder ){	//5億円以下かつ 課税売上割合が９５％以上の場合
			if(index == ID201_19D){
				c_set(IDC_BUTTON1);
				IsPrevPosition = FALSE;
			}
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){//非連動の場合
				if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){	// 個別対応
					if( index == ID201_17D ){
						c_set(IDC_BUTTON1);
						IsPrevPosition = FALSE;
					}
				}
				else{
					if( index == ID201_20D ){
						c_set(IDC_BUTTON1);
						IsPrevPosition = FALSE;
					}
				}
			}
			else{
				if(index == ID201_21D){
					c_set(IDC_BUTTON1);
					IsPrevPosition = FALSE;
				}
			}
		}
		if( IsPrevPosition ){
			m_Fhyo2_01Diag02.SetPrevPosition();
		}

	}
	if( wTerm == VK_DELETE ){
		switch( index ){
			case ID201_23D:			//  控除対象仕入税額　税率6.24％適用分　D
				F201_23c &= ~0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("324802"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("324902"));
				// 各入力の表示
				f201data_all( 13 );
				m_Fhyo2_01Diag02.SetPosition( ID201_23D );
				break;

			case ID201_23E:			//  控除対象仕入税額　税率7.8％適用分　E
				F201_23c &= ~0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("324803"));
				(*m_pSyzSyukei)->ResetShinkokuData(_T("324903"));
				// 各入力の表示
				f201data_all( 13 );
				m_Fhyo2_01Diag02.SetPosition( ID201_23E );
				break;

			case ID201_10D:			//	課税仕入れに係る消費税額 税率6.24％適用分　D
				F201_10c &= ~0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("323502"));
				// 各入力の表示
				f201data_all( 6 );
				m_Fhyo2_01Diag02.SetPosition( ID201_10D );
				break;

			case ID201_10E:			//  課税仕入れに係る消費税額 税率7.8％適用分　E
				F201_10c &= ~0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("323503"));
				// 各入力の表示
				f201data_all( 6 );
				m_Fhyo2_01Diag02.SetPosition( ID201_10E );
				break;

			case ID201_12E:			//  特定課税仕入れに係る消費税額 税率7.8％適用分　E
				F201_12c &= ~0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("323703"));
				// 各入力の表示
				f201data_all( 16 );
				m_Fhyo2_01Diag02.SetPosition( ID201_12E );
				break;

			case ID201_19D:			//	個別対応方式により控除する課税仕入れ等の税額 税率6.24％適用分　D
				F201_19c &= ~0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("324402"));
				// 各入力の表示
				f201data_all( 10 );
				m_Fhyo2_01Diag02.SetPosition( ID201_19D );
				break;

			case ID201_19E:			//  個別対応方式により控除する課税仕入れ等の税額 税率7.8％適用分　E
				F201_19c &= ~0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("324403"));
				// 各入力の表示
				f201data_all( 10 );
				m_Fhyo2_01Diag02.SetPosition( ID201_19E );
				break;

			case ID201_20D:			//  一括比例配分方式により控除する課税仕入れ等の税額 税率6.24％適用分　D
				F201_20c &= ~0x80;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("324502"));
				// 各入力の表示
				f201data_all( 11 );
				m_Fhyo2_01Diag02.SetPosition( ID201_20D );
				break;
			case ID201_20E:			//  一括比例配分方式により控除する課税仕入れ等の税額 税率7.8％適用分　E
				F201_20c &= ~0x800;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("324503"));
				// 各入力の表示
				f201data_all( 11 );
				m_Fhyo2_01Diag02.SetPosition( ID201_20E );
				break;
			case ID201_8EX:		// 課税売上割合変更チェック
				break;
			default:
				dsp_cls(IDC_ICSDIAG8CTRL2 , index);
				break;
		}

	}

	int nowpos = 0;
	CWnd* pWnd = this->GetFocus();
	if ( pWnd->GetDlgCtrlID() == IDC_BUTTON1 ) {
		nowpos = ID201_14X;	// 14Xのポジションをセット
	}
	else {
		nowpos = m_Fhyo2_01Diag02.GetPosition();	// 移動先または自身のポジション
	}

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}


}

BOOL CShinFhyo2_01::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	CWnd *pWnd = GetFocus();

	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			// 付表２－１更新
			GetAllDiagData();
			f201data_all( 1 );
			WriteData( 1 );

			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}

			return TRUE;
		}
		if( pMsg->wParam == VK_F2 ){
			DIAGRAM_DATA   dd;
			if( m_curwnd != -1 && (pWnd != GetDlgItem(IDC_BUTTON1))){
				short result = ((CIcsdiagctrl*)GetDlgItem( m_curwnd ))->GetPosition();
				diag_getdata( m_curwnd, result, &dd, CIcsdiagctrl );
				if( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
					TerminationIcsdiag8ctrl2( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}

		if(pWnd == GetDlgItem(IDC_BUTTON1)){
			char VK_FLG = 0x00;
			VK_FLG = (char)::GetKeyState( VK_SHIFT );

			if( (pMsg->wParam==VK_F3) || (pMsg->wParam==VK_RIGHT) || (pMsg->wParam==VK_DOWN) || ((pMsg->wParam==VK_TAB)&&!(VK_FLG&0x80)) ){
				if( *m_pShinInfo->pOver500MillionSw != 1 && F201_8EX >= (*m_pSnHeadData)->m_UriRatioBorder ){	//5億円かつ課税割合が95％以上の場合
					diag_setposition( IDC_ICSDIAG8CTRL2, ID201_21D, CIcsdiagctrl );
				}
				else{
					if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){	// 非連動の場合
						if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){	// 個別対応
							diag_setposition( IDC_ICSDIAG8CTRL2, ID201_17D, CIcsdiagctrl );
						}
						else{
							diag_setposition( IDC_ICSDIAG8CTRL2, ID201_20D, CIcsdiagctrl );
						}
					}
					else{
						diag_setposition( IDC_ICSDIAG8CTRL2, ID201_21D, CIcsdiagctrl );
					}
				}
				return TRUE;
			}
			// 前項目へ
			if( (pMsg->wParam==VK_LEFT) || (pMsg->wParam==VK_F2) || (pMsg->wParam==VK_UP) || ((pMsg->wParam==VK_TAB)&&(VK_FLG&0x80)) ){
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){	// 非連動の場合
					diag_setposition( IDC_ICSDIAG8CTRL2, ID201_13E, CIcsdiagctrl );
				}
				else{
					diag_setposition( IDC_ICSDIAG8CTRL2, ID201_22E, CIcsdiagctrl );
				}
				return TRUE;
			}
		}

	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}


// 棚卸調整額入力
void CShinFhyo2_01::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if ((*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE) {

		// 前回の情報をセーブ
		char			SV_mtype = (*m_pSnTanaData)->Sn_MENZsw;
		unsigned short	SV_F201_8E = F201_8EX;
		char			SV_mnk624[3][MONY_BUF_SIZE];
		char			SV_mnz624[3][MONY_BUF_SIZE];
		char			SV_mnk78[3][MONY_BUF_SIZE];
		char			SV_mnz78[3][MONY_BUF_SIZE];
		char			zerobuf[MONY_BUF_SIZE];

		char			SV_mnk40[3][MONY_BUF_SIZE];
		char			SV_mnz40[3][MONY_BUF_SIZE];
		char			SV_mnk63[3][MONY_BUF_SIZE];
		char			SV_mnz63[3][MONY_BUF_SIZE];
		char			SV_menzk[3][MONY_BUF_SIZE];
		char			SV_menzz[3][MONY_BUF_SIZE];
		char			VAL_14A[MONY_BUF_SIZE];
		char			VAL_14B[MONY_BUF_SIZE];
		char			VAL_14C[MONY_BUF_SIZE];
		char			VAL_17A[MONY_BUF_SIZE];
		char			VAL_17B[MONY_BUF_SIZE];
		char			VAL_17C[MONY_BUF_SIZE];
		char			VAL_18A[MONY_BUF_SIZE];
		char			VAL_18B[MONY_BUF_SIZE];
		char			VAL_18C[MONY_BUF_SIZE];

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

		if ((*m_pSnHeadData)->SVmzsw == 1) {
			memmove(SV_mnk63[0], (*m_pSnTanaData)->Sn_mnk63[0], MONY_BUF_SIZE);	// 棚卸資産額 (新税率分)　課税に係る
			memmove(SV_mnz63[0], (*m_pSnTanaData)->Sn_mnz63[0], MONY_BUF_SIZE);	// 消費税額   (〃      )　〃
			memmove(SV_mnk63[1], (*m_pSnTanaData)->Sn_mnk63[1], MONY_BUF_SIZE);	// 棚卸資産額 (新税率分)　非課税に係る
			memmove(SV_mnz63[1], (*m_pSnTanaData)->Sn_mnz63[1], MONY_BUF_SIZE);	// 消費税額   (〃      )　〃
			memmove(SV_mnk63[2], (*m_pSnTanaData)->Sn_mnk63[2], MONY_BUF_SIZE);	// 棚卸資産額 (新税率分)　共通に係る
			memmove(SV_mnz63[2], (*m_pSnTanaData)->Sn_mnz63[2], MONY_BUF_SIZE);	// 消費税額   (〃      )　〃

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


		// 個別対応
		CChangeTaxEx2		CTDiag;
		CTDiag.InitInfo ((*m_pSnFhyo2_01Data),(*m_pSnHeadData),(*m_pSnTanaData), m_pArith);

		HINSTANCE	svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle(g_hInstance);
		int st = (int)CTDiag.DoModal();
		AfxSetResourceHandle(svhInstResource);

		if (st == IDOK) {
			// 手入力
			//if (!((*m_pSnTanaData)->Sn_tansw & 0x01) || ((*m_pSnHeadData)->Sn_Sign4 & 0x80)) {//非連動の場合は常に更新させる
			if((*m_pSnHeadData)->Sn_Sign4 & 0x80) {	// 非連動
				if ((*m_pSnHeadData)->SVmzsw == 1) {
					MoneyBasejagArray	money;

					// 付表２－２のデータを取得
					money = (*m_pSyzSyukei)->GetShinkokuData(_T("321701"));
					memmove(VAL_17A, money[0][0].arith, MONY_BUF_SIZE);
					money = (*m_pSyzSyukei)->GetShinkokuData(_T("321702"));
					memmove(VAL_17B, money[0][0].arith, MONY_BUF_SIZE);
					money = (*m_pSyzSyukei)->GetShinkokuData(_T("321703"));
					memmove(VAL_17C, money[0][0].arith, MONY_BUF_SIZE);
					money = (*m_pSyzSyukei)->GetShinkokuData(_T("321801"));
					memmove(VAL_18A, money[0][0].arith, MONY_BUF_SIZE);
					money = (*m_pSyzSyukei)->GetShinkokuData(_T("321802"));
					memmove(VAL_18B, money[0][0].arith, MONY_BUF_SIZE);
					money = (*m_pSyzSyukei)->GetShinkokuData(_T("321803"));
					memmove(VAL_18C, money[0][0].arith, MONY_BUF_SIZE);
				}

				// 前回が個別対応で95％未満の場合、前回分を削除
				if ((SV_mtype & 0x10) || ((*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE)) {
					if (SV_mtype & 0x01) {
						m_pArith->l_add(F201_17E, F201_17E, SV_mnz78[0]);		// 課税売上に係る仕入税額
						m_pArith->l_add(F201_18E, F201_18E, SV_mnz78[2]);		// 共通の売上に係る仕入税額
						m_pArith->l_add(F201_17D, F201_17D, SV_mnz624[0]);		// 課税売上に係る仕入税額
						m_pArith->l_add(F201_18D, F201_18D, SV_mnz624[2]);		// 共通の売上に係る仕入税額
						if ((*m_pSnHeadData)->SVmzsw == 1) {
							m_pArith->l_add(VAL_17C, VAL_17C, SV_mnz63[0]);		// 課税売上に係る仕入税額
							m_pArith->l_add(VAL_18C, VAL_18C, SV_mnz63[2]);		// 共通の売上に係る仕入税額
							m_pArith->l_add(VAL_17B, VAL_17B, SV_mnz40[0]);		// 課税売上に係る仕入税額
							m_pArith->l_add(VAL_18B, VAL_18B, SV_mnz40[2]);		// 共通の売上に係る仕入税額
							m_pArith->l_add(VAL_17A, VAL_17A, SV_menzz[0]);		// 課税売上に係る仕入税額
							m_pArith->l_add(VAL_18A, VAL_18A, SV_menzz[2]);		// 共通の売上に係る仕入税額
						}
					}
					else {
						m_pArith->l_sub(F201_17E, F201_17E, SV_mnz78[0]);		// 15. 課税売上に係る仕入税額
						m_pArith->l_sub(F201_18E, F201_18E, SV_mnz78[2]);		// 16. 共通の売上に係る仕入税額
						m_pArith->l_sub(F201_17D, F201_17D, SV_mnz624[0]);		// 15. 課税売上に係る仕入税額
						m_pArith->l_sub(F201_18D, F201_18D, SV_mnz624[2]);		// 16. 共通の売上に係る仕入税額
						if ((*m_pSnHeadData)->SVmzsw == 1) {
							m_pArith->l_sub(VAL_17C, VAL_17C, SV_mnz63[0]);		// 課税売上に係る仕入税額
							m_pArith->l_sub(VAL_18C, VAL_18C, SV_mnz63[2]);		// 共通の売上に係る仕入税額
							m_pArith->l_sub(VAL_17B, VAL_17B, SV_mnz40[0]);		// 課税売上に係る仕入税額
							m_pArith->l_sub(VAL_18B, VAL_18B, SV_mnz40[2]);		// 共通の売上に係る仕入税額
							m_pArith->l_sub(VAL_17A, VAL_17A, SV_menzz[0]);		// 課税売上に係る仕入税額
							m_pArith->l_sub(VAL_18A, VAL_18A, SV_menzz[2]);		// 共通の売上に係る仕入税額
						}
					}
				}

				if ((*m_pSnTanaData)->Sn_MENZsw & 0x10) {
					memset(F201_14E, '\0', MONY_BUF_SIZE);
					m_pArith->l_add(F201_14E, F201_14E, (*m_pSnTanaData)->Sn_mnz78[0]);
					m_pArith->l_add(F201_14E, F201_14E, (*m_pSnTanaData)->Sn_mnz78[1]);
					m_pArith->l_add(F201_14E, F201_14E, (*m_pSnTanaData)->Sn_mnz78[2]);
					memset(F201_14D, '\0', MONY_BUF_SIZE);
					m_pArith->l_add(F201_14D, F201_14D, (*m_pSnTanaData)->Sn_mnz624[0]);
					m_pArith->l_add(F201_14D, F201_14D, (*m_pSnTanaData)->Sn_mnz624[1]);
					m_pArith->l_add(F201_14D, F201_14D, (*m_pSnTanaData)->Sn_mnz624[2]);

					memset(VAL_14C, '\0', MONY_BUF_SIZE);
					memset(VAL_14B, '\0', MONY_BUF_SIZE);
					memset(VAL_14A, '\0', MONY_BUF_SIZE);
					if ((*m_pSnHeadData)->SVmzsw == 1) {
						m_pArith->l_add(VAL_14C, VAL_14C, (*m_pSnTanaData)->Sn_mnz63[0]);
						m_pArith->l_add(VAL_14C, VAL_14C, (*m_pSnTanaData)->Sn_mnz63[1]);
						m_pArith->l_add(VAL_14C, VAL_14C, (*m_pSnTanaData)->Sn_mnz63[2]);
						m_pArith->l_add(VAL_14B, VAL_14B, (*m_pSnTanaData)->Sn_mnz40[0]);
						m_pArith->l_add(VAL_14B, VAL_14B, (*m_pSnTanaData)->Sn_mnz40[1]);
						m_pArith->l_add(VAL_14B, VAL_14B, (*m_pSnTanaData)->Sn_mnz40[2]);
						m_pArith->l_add(VAL_14A, VAL_14A, (*m_pSnTanaData)->Sn_menzz[0]);
						m_pArith->l_add(VAL_14A, VAL_14A, (*m_pSnTanaData)->Sn_menzz[1]);
						m_pArith->l_add(VAL_14A, VAL_14A, (*m_pSnTanaData)->Sn_menzz[2]);
					}

					// 旧格納場所へ格納
					memmove((*m_pSnTanaData)->Sn_MNZ78, F201_14E, MONY_BUF_SIZE);
					memmove((*m_pSnTanaData)->Sn_MNZ624, F201_14D, MONY_BUF_SIZE);
					if ((*m_pSnHeadData)->SVmzsw == 1) {
						memmove((*m_pSnTanaData)->Sn_MNZ63, VAL_14C, MONY_BUF_SIZE);
						memmove((*m_pSnTanaData)->Sn_MNZ40, VAL_14B, MONY_BUF_SIZE);
						memmove((*m_pSnTanaData)->Sn_MENZZ, VAL_14A, MONY_BUF_SIZE);
					}

					if ((*m_pSnTanaData)->Sn_MENZsw & 0x01) {
						m_pArith->l_neg(F201_14X);
						m_pArith->l_neg(F201_14D);
						m_pArith->l_neg(F201_14E);
						if ((*m_pSnHeadData)->SVmzsw == 1) {
							m_pArith->l_neg(VAL_14A);
							m_pArith->l_neg(VAL_14B);
							m_pArith->l_neg(VAL_14C);
						}
					}
					// 個別対応で95％未満の場合
					if ((F201_8EX < (*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE)) {
						if ((*m_pSnTanaData)->Sn_MENZsw & 0x01) {
							m_pArith->l_sub(F201_17E, F201_17E, (*m_pSnTanaData)->Sn_mnz78[0]);
							m_pArith->l_sub(F201_18E, F201_18E, (*m_pSnTanaData)->Sn_mnz78[2]);
							m_pArith->l_sub(F201_17D, F201_17D, (*m_pSnTanaData)->Sn_mnz624[0]);
							m_pArith->l_sub(F201_18D, F201_18D, (*m_pSnTanaData)->Sn_mnz624[2]);
							if ((*m_pSnHeadData)->SVmzsw == 1) {
								m_pArith->l_sub(VAL_17C, VAL_17C, (*m_pSnTanaData)->Sn_mnz63[0]);		// 課税売上に係る仕入税額
								m_pArith->l_sub(VAL_18C, VAL_18C, (*m_pSnTanaData)->Sn_mnz63[2]);		// 共通の売上に係る仕入税額
								m_pArith->l_sub(VAL_17B, VAL_17B, (*m_pSnTanaData)->Sn_mnz40[0]);		// 課税売上に係る仕入税額
								m_pArith->l_sub(VAL_18B, VAL_18B, (*m_pSnTanaData)->Sn_mnz40[2]);		// 共通の売上に係る仕入税額
								m_pArith->l_sub(VAL_17A, VAL_17A, (*m_pSnTanaData)->Sn_menzz[0]);		// 課税売上に係る仕入税額
								m_pArith->l_sub(VAL_18A, VAL_18A, (*m_pSnTanaData)->Sn_menzz[2]);		// 共通の売上に係る仕入税額
							}
						}
						else {
							m_pArith->l_add(F201_17E, F201_17E, (*m_pSnTanaData)->Sn_mnz78[0]);
							m_pArith->l_add(F201_18E, F201_18E, (*m_pSnTanaData)->Sn_mnz78[2]);
							m_pArith->l_add(F201_17D, F201_17D, (*m_pSnTanaData)->Sn_mnz624[0]);
							m_pArith->l_add(F201_18D, F201_18D, (*m_pSnTanaData)->Sn_mnz624[2]);
							if ((*m_pSnHeadData)->SVmzsw == 1) {
								m_pArith->l_add(VAL_17C, VAL_17C, (*m_pSnTanaData)->Sn_mnz63[0]);		// 課税売上に係る仕入税額
								m_pArith->l_add(VAL_18C, VAL_18C, (*m_pSnTanaData)->Sn_mnz63[2]);		// 共通の売上に係る仕入税額
								m_pArith->l_add(VAL_17B, VAL_17B, (*m_pSnTanaData)->Sn_mnz40[0]);		// 課税売上に係る仕入税額
								m_pArith->l_add(VAL_18B, VAL_18B, (*m_pSnTanaData)->Sn_mnz40[2]);		// 共通の売上に係る仕入税額
								m_pArith->l_add(VAL_17A, VAL_17A, (*m_pSnTanaData)->Sn_menzz[0]);		// 課税売上に係る仕入税額
								m_pArith->l_add(VAL_18A, VAL_18A, (*m_pSnTanaData)->Sn_menzz[2]);		// 共通の売上に係る仕入税額
							}
						}
					}

					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), F201_14D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), F201_14E );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324202" ), F201_17D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324203" ), F201_17E );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324302" ), F201_18D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324303" ), F201_18E );

					if ((*m_pSnHeadData)->SVmzsw == 1) {
						// 付表２－２の金額を更新
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), VAL_14A );
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), VAL_14B );
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), VAL_14C );
						if ((F201_8EX < (*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE)) {
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321701" ), VAL_17A );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321702" ), VAL_17B );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321703" ), VAL_17C );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321801" ), VAL_18A );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321802" ), VAL_18B );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321803" ), VAL_18C );
						}
					}
				}
				else {
					memset(F201_14X, '\0', MONY_BUF_SIZE);
					memset(F201_14D, '\0', MONY_BUF_SIZE);
					memset(F201_14E, '\0', MONY_BUF_SIZE);

					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), F201_14D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), F201_14E );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324202" ), F201_17D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324203" ), F201_17E );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324302" ), F201_18D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "324303" ), F201_18E );

					if ((*m_pSnHeadData)->SVmzsw == 1) {
						// 付表２－２の金額を更新
						memset(zerobuf, '\0', MONY_BUF_SIZE);
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "321401" ), zerobuf );
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "321402" ), zerobuf );
						( *m_pSyzSyukei )->SetShinkokuData ( _T( "321403" ), zerobuf );
						if ((F201_8EX < (*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE)) {
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321701" ), VAL_17A );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321702" ), VAL_17B );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321703" ), VAL_17C );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321801" ), VAL_18A );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321802" ), VAL_18B );
							( *m_pSyzSyukei )->SetShinkokuData ( _T( "321803" ), VAL_18C );
						}
					}

				}
	
				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_01();
				SetItemPosition();
			}
			else if (!((*m_pSnTanaData)->Sn_tansw & 0x01)) {
				// 連動時は、TanaDataAdd（MemoryReget）で行う

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_01();
				SetItemPosition();
			}
		}

	}
	else {
		CChangeTax2	CTDiag;
		CTDiag.InitInfo((*m_pSnHeadData), (*m_pSnTanaData), m_pArith);

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
					memmove(F201_14D, (*m_pSnTanaData)->Sn_MNZ624, MONY_BUF_SIZE);
					memmove(F201_14E, (*m_pSnTanaData)->Sn_MNZ78, MONY_BUF_SIZE);
					if ((*m_pSnTanaData)->Sn_MENZsw & 0x01) {
						m_pArith->l_neg(F201_14D);
						m_pArith->l_neg(F201_14E);
					}

					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323902" ), F201_14D );
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "323903" ), F201_14E );
				}
				else {
					memset(F201_14X, '\0', MONY_BUF_SIZE);
					memset(F201_14D, '\0', MONY_BUF_SIZE);
					memset(F201_14E, '\0', MONY_BUF_SIZE);

					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323902" ) );
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "323903" ) );
					// 付表２－２もリセット
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321401" ) );
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321402" ) );
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "321403" ) );
				}

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_01();
				SetItemPosition();	// ('14.03.13)
			}
			else if (!((*m_pSnTanaData)->Sn_tansw & 0x01)) {
				// 連動時は、TanaDataAdd（MemoryReget）で行う

				// 再集計
				MemoryReget();

				// 再表示
				DispFh2_01();
				SetItemPosition();	// ('14.03.13)

			}
		}
	}
}


void CShinFhyo2_01::CheckButtonIcsdiag8ctrl2(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	char flg[MONY_BUF_SIZE] = { 0 };

	// ICSMessageBoxによってEditOffへはしるのを防ぐ
	m_UwSgn = FALSE;

	// 課税売上割合の変更チェック
	if (index == ID201_8EX) {
		diag_getdata(IDC_ICSDIAG8CTRL2, ID201_8EX, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		if ((*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE) {
			CString	Msg;
			Msg.Empty();
			if (DiagData.data_check) {
				Msg = _T("課税売上割合に準ずる割合を適用する為に、\n");
				Msg += _T("課税売上割合を変更される場合は・・・[はい]を、\n");
				Msg += _T("それ以外の理由で変更される場合は・・・[いいえ]を、\n");
				Msg += _T("選択してください。");
				
				(*m_pSnHeadData)->Sn_UPERS &= 0xfd;
				int st = ICSMessageBox(Msg, MB_YESNO, 0);
				if (st == IDYES) {
					if ((*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE) {
					}
					else {
					}

					(*m_pSnHeadData)->Sn_UPERS |= 0x02;//
					m_pArith->l_input(flg, "1");
					(*m_pSyzSyukei)->SetShinkokuData(_T("FF3002"), flg);
				}
			}
			else {
				if ((*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE) {
					Msg = "課税売上割合に準ずる割合を無効にします。";
					ICSMessageBox(Msg, MB_OK, 0);
					(*m_pSnHeadData)->Sn_UPERS &= 0xfd;
					m_pArith->l_input(flg, "0");
					(*m_pSyzSyukei)->SetShinkokuData(_T("FF3002"), flg);
					(*m_pSyzSyukei)->ResetShinkokuData(_T("323300"));
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

		m_UwSgn = TRUE;

		//19.11.05
		m_Fhyo2_01Diag02.SetPosition(index);


		(*m_pSnHeadData)->Sn_UPERS &= 0xef;
		if( DiagData.data_check ){
			(*m_pSnHeadData)->Sn_UPERS |= 0x10;
		}

		// 各入力の計算表示
		f201data_all( 4 );
	}

}

int CShinFhyo2_01::SyukeiDataSet(int cID)
{
	DIAGRAM_DATA	DiagData;
	char			val[MONY_BUF_SIZE];

	diag_getdata(IDC_ICSDIAG8CTRL2, cID, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(val, DiagData.data_val, MONY_BUF_SIZE);

	switch (cID) {
	case ID201_1X:
		memmove(F201_1X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322601"), F201_1X);
		break;
	case ID201_1D:
		memmove(F201_1D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322602"), F201_1D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("911104"), F201_1D);
		break;
	case ID201_1E:
		memmove(F201_1E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322603"), F201_1E);
		(*m_pSyzSyukei)->SetShinkokuData(_T("911105"), F201_1E);
		break;
	case ID201_2F:
		memmove(F201_2F, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322700"), F201_2F);
		break;
	case ID201_3F:
		memmove(F201_3F, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("322800"), F201_3F);
		break;
	case ID201_6F:
		memmove(F201_6F, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323100"), F201_6F);
		break;
	case ID201_9X:
		memmove(F201_9X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323401"), F201_9X);
		break;
	case ID201_9D:
		memmove(F201_9D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323402"), F201_9D);
		break;
	case ID201_9E:
		memmove(F201_9E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323403"), F201_9E);
		break;
	case ID201_10X:	// 課税仕入れに係る消費税額 旧税率分小計
		memmove(F201_10X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323501"), F201_10X);
		break;
	case ID201_10D:	// 課税仕入れに係る消費税額 税率6.24％適用分
		if (m_pArith->l_cmp(F201_10D, val)) {
			F201_10c |= 0x80;
			memmove(F201_10D, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("323502"), F201_10D);
		}
		break;
	case ID201_10E:	// 課税仕入れに係る消費税額 税率7.8％適用分
		if (m_pArith->l_cmp(F201_10E, val)) {
			F201_10c |= 0x800;
			memmove(F201_10E, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("323503"), F201_10E);
		}
		break;
	case ID201_11X:
		memmove(F201_11X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323601"), F201_11X);
		break;
	case ID201_11E:
		memmove(F201_11E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323603"), F201_11E);
		break;
	case ID201_12X:
		memmove(F201_12X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323701"), F201_12X);
		break;
	case ID201_12E:
		if (m_pArith->l_cmp(F201_12E, val)) {
			F201_12c |= 0x800;
			memmove(F201_12E, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("323703"), F201_12E);
		}
		break;
	case ID201_13X:
		memmove(F201_13X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323801"), F201_13X);
		break;
	case ID201_13D:
		memmove(F201_13D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323802"), F201_13D);
		break;
	case ID201_13E:
		memmove(F201_13E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323803"), F201_13E);
		break;
	case ID201_14X:
		memmove(F201_14X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323901"), F201_14X);
		break;
	case ID201_14D:
		memmove(F201_14D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323902"), F201_14D);
		break;
	case ID201_14E:
		memmove(F201_14E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323903"), F201_14E);
		break;
	case ID201_17X:
		memmove(F201_17X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324201"), F201_17X);
		break;
	case ID201_17D:
		memmove(F201_17D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324202"), F201_17D);
		break;
	case ID201_17E:
		memmove(F201_17E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324203"), F201_17E);
		break;
	case ID201_18X:
		memmove(F201_18X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324301"), F201_18X);
		break;
	case ID201_18D:
		memmove(F201_18D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324302"), F201_18D);
		break;
	case ID201_18E:
		memmove(F201_18E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324303"), F201_18E);
		break;
	case ID201_19X:	// 個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 旧税率分小計
		memmove(F201_19X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324401"), F201_19X);
		break;
	case ID201_19D:	// 個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 税率6.24％適用分
		if (m_pArith->l_cmp(F201_19D, val)) {
			F201_19c |= 0x80;
			memmove(F201_19D, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324402"), F201_19D);
		}
		break;
	case ID201_19E:	// 個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 税率7.8％適用分
		if (m_pArith->l_cmp(F201_19E, val)) {
			F201_19c |= 0x800;
			memmove(F201_19E, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324403"), F201_19E);
		}
		break;
	case ID201_19F:	// 個別対応方式により控除する課税仕入れ等の税額〔17+(18*4/7)〕 合計
		memmove(F201_19F, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324400"), F201_19F);
		break;
	case ID201_20X:	// 一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 旧税率分小計
		memmove(F201_20X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324501"), F201_20X);
		break;
	case ID201_20D:	// 一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 税率6.24％適用分
		if (m_pArith->l_cmp(F201_20D, val)) {
			F201_20c |= 0x80;
			memmove(F201_20D, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324502"), F201_20D);
		}
		break;
	case ID201_20E:	// 一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 税率7.8％適用分
		if (m_pArith->l_cmp(F201_20E, val)) {
			F201_20c |= 0x800;
			memmove(F201_20E, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324503"), F201_20E);
		}
		break;
	case ID201_20F:	// 一括比例配分方式により控除する課税仕入れ等の税額〔15*4/7〕 合計
		memmove(F201_20F, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324500"), F201_20F);
		break;
	case ID201_21X:
		memmove(F201_21X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324601"), F201_21X);
		break;
	case ID201_21D:
		memmove(F201_21D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324602"), F201_21D);
		break;
	case ID201_21E:
		memmove(F201_21E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324603"), F201_21E);
		break;
	case ID201_22X:
		memmove(F201_22X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324701"), F201_22X);
		break;
	case ID201_22D:
		memmove(F201_22D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324702"), F201_22D);
		break;
	case ID201_22E:
		memmove(F201_22E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324703"), F201_22E);
		break;
	case ID201_23X:	// 控除対象仕入税額がプラスの時 旧税率分小計
		memmove(F201_23X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324801"), F201_23X);
		break;
	case ID201_23D:	// 控除対象仕入税額がプラスの時 税率6.24％適用分
		if (m_pArith->l_cmp(F201_23D, val)) {
			F201_23c |= 0x80;
			if (m_pArith->l_test(val) < 0) {
				m_pArith->l_neg( val );
				memmove(F201_24D, val, MONY_BUF_SIZE);
				memset(F201_23D, '\0', MONY_BUF_SIZE);
			}
			else {
				memmove(F201_23D, val, MONY_BUF_SIZE);
				memset(F201_24D, '\0', MONY_BUF_SIZE);
			}
			(*m_pSyzSyukei)->SetShinkokuData(_T("324802"), F201_23D);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324902"), F201_24D);
		}
		break;
	case ID201_23E:	// 控除対象仕入税額がプラスの時 税率7.8％適用分
		if (m_pArith->l_cmp(F201_23E, val)) {
			F201_23c |= 0x800;
			if (m_pArith->l_test(val) < 0) {
				m_pArith->l_neg( val );
				memmove(F201_24E, val, MONY_BUF_SIZE);
				memset(F201_23E, '\0', MONY_BUF_SIZE);
			}
			else {
				memmove(F201_23E, val, MONY_BUF_SIZE);
				memset(F201_24E, '\0', MONY_BUF_SIZE);
			}
			(*m_pSyzSyukei)->SetShinkokuData(_T("324803"), F201_23E);
			(*m_pSyzSyukei)->SetShinkokuData(_T("324903"), F201_24E);
		}
		break;
	case ID201_24X:	// 控除対象仕入税額がマイナスの時 旧税率分小計
		memmove(F201_24X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324901"), F201_24X);
		break;
	case ID201_24D:	// 控除対象仕入税額がマイナスの時 税率6.24％適用分
		memmove(F201_24D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324902"), F201_24D);
		break;
	case ID201_24E:	// 控除対象仕入税額がマイナスの時 税率7.8％適用分
		memmove(F201_24E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("324903"), F201_24E);
		break;
	case ID201_25X:
		memmove(F201_25X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("325001"), F201_25X);
		break;
	case ID201_25D:
		memmove(F201_25D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("325002"), F201_25D);
		break;
	case ID201_25E:
		memmove(F201_25E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("325003"), F201_25E);
		break;
	}

	return MemoryReget();
}

int CShinFhyo2_01::MemoryReget()
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
	for (int l = 0; l < cnt; l++) {
		incnt = (int)money[l].GetCount();
		for (int m = 0; m < incnt; m++) {
			if (money[l][m].code == _T("322601"))
				memmove(F201_1X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322602"))
				memmove(F201_1D, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322603"))
				memmove(F201_1E, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322600"))
				memmove(F201_1F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322700"))
				memmove(F201_2F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("322800"))
				memmove(F201_3F, money[l][m].arith, MONY_BUF_SIZE );
			if (money[l][m].code == _T("322900"))
				memmove(F201_4F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323000"))
				memmove(F201_5F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323100"))
				memmove(F201_6F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323200"))
				memmove(F201_7F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323300")) {
				char wariai[32];
				m_pArith->l_print( wariai,  money[l][m].arith, _T("SSSSSSSSSSSSS9") );
				F201_8EX = atoi(wariai);
			}
			if (money[l][m].code == _T("323401"))
				memmove(F201_9X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323402"))
				memmove(F201_9D, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323403"))
				memmove(F201_9E, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323400"))
				memmove(F201_9F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323501"))
				memmove(F201_10X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323502"))
				memmove(F201_10D, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323503"))
				memmove(F201_10E, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323500"))
				memmove(F201_10F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323601"))
				memmove(F201_11X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323603"))
				memmove(F201_11E, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323600"))
				memmove(F201_11F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323701"))
				memmove(F201_12X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323703"))
				memmove(F201_12E, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323700"))
				memmove(F201_12F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323801"))
				memmove(F201_13X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323802"))
				memmove(F201_13D, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323803"))
				memmove(F201_13E, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323800"))
				memmove(F201_13F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323901"))
				memmove(F201_14X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323902"))
				memmove(F201_14D, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323903"))
				memmove(F201_14E, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("323900"))
				memmove(F201_14F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324001"))
				memmove(F201_15X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324002"))
				memmove(F201_15D, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324003"))
				memmove(F201_15E, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324000"))
				memmove(F201_15F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324101"))
				memmove(F201_16X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324102"))
				memmove(F201_16D, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324103"))
				memmove(F201_16E, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324100"))
				memmove(F201_16F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324201")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F201_17X, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_17X, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324202")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F201_17D, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_17D, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324203")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F201_17E, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_17E, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324200")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F201_17F, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_17F, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324301")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F201_18X, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_18X, '\0', MONY_BUF_SIZE);
				}
			}			
			if (money[l][m].code == _T("324302")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F201_18D, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_18D, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324303")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F201_18E, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_18E, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324300")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F201_18F, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_18F, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324401")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F201_19X, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_19X, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324402")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F201_19D, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_19D, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324403")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F201_19E, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_19E, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324400")) {
				if ( Kobetsu && (Flg || semiuri) ) {
					memmove(F201_19F, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_19F, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324501")) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove(F201_20X, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_20X, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324502")) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove(F201_20D, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_20D, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324503")) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove(F201_20E, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_20E, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324500")) {
				if ( ( Kobetsu == FALSE ) && Flg ) {
					memmove(F201_20F, money[l][m].arith, MONY_BUF_SIZE);
				}
				else {
					memset(F201_20F, '\0', MONY_BUF_SIZE);
				}
			}
			if (money[l][m].code == _T("324601"))
				memmove(F201_21X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324600"))
				memmove(F201_21F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324701"))
				memmove(F201_22X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324700"))
				memmove(F201_22F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324801"))
				memmove(F201_23X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324802"))
				memmove(F201_23D, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324803"))
				memmove(F201_23E, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324800"))
				memmove(F201_23F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324901"))
				memmove(F201_24X, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324902"))
				memmove(F201_24D, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324903"))
				memmove(F201_24E, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("324900"))
				memmove(F201_24F, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("325001"))
				memmove(F201_25X, money[l][m].arith, MONY_BUF_SIZE);

			//--->yoshida190919
			//なぜ記述なかったのかを調べる
			if (money[l][m].code == _T("325002"))
				memmove(F201_25D, money[l][m].arith, MONY_BUF_SIZE);
			if (money[l][m].code == _T("325003"))
				memmove(F201_25E, money[l][m].arith, MONY_BUF_SIZE);
			//<------

			if (money[l][m].code == _T("325000"))
				memmove(F201_25F, money[l][m].arith, MONY_BUF_SIZE);
			
			// 特定収入計算表対応
			if ( ( *( m_pShinInfo )->pSgSpc == 1 ) && ( ( *m_pSnHeadData )->SVmzsw == 1 ) ) {
				// 付表2-2の金額を取得

				//---->yoshida191121
				//--> 191127　f201calq_spc()の中で取得するように変更
				//MoneyBasejagArray	money2;

				//money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("310202"));
				//memmove(SyzToSpc.SpKzur4, money2[0][0].arith, MONY_BUF_SIZE);

				//money2 = (*m_pSyzSyukei)->GetShinkokuData(_T("310203"));
				//memmove(SyzToSpc.SpKzur63, money2[0][0].arith, MONY_BUF_SIZE);
				//<-- 191127
				//-----------
//				if ( money[l][m].code == _T( "320102" ) ) {
//					memmove ( SyzToSpc.SpKzur4, money[l][m].arith, sizeof ( SyzToSpc.SpKzur4 ) );
//				}
//				else if ( money[l][m].code == _T( "320103" ) ) {
//					memmove ( SyzToSpc.SpKzur63, money[l][m].arith, sizeof ( SyzToSpc.SpKzur63 ) );
//				}
				//<-------------

				// ( !Flg ) {
				if ( !Flg && !semiuri ) {
					if ( money[l][m].code == _T( "321602" ) ) {
						memmove ( SyzToSpc.SpKgz4, money[l][m].arith, sizeof ( SyzToSpc.SpKgz4 ) );
					}
					else if ( money[l][m].code == _T( "321603" ) ) {
						memmove ( SyzToSpc.SpKgz63, money[l][m].arith, sizeof ( SyzToSpc.SpKgz63 ) );
					}
				}
				else {
					if ( Kobetsu ) {
						if ( Flg || semiuri ) {
							if ( money[l][m].code == _T( "321902" ) ) {
								memmove( SyzToSpc.SpKgz4, money[l][m].arith, sizeof ( SyzToSpc.SpKgz4 ) );
							}
							else if ( money[l][m].code == _T( "321903" ) ) {
								memmove( SyzToSpc.SpKgz63, money[l][m].arith, sizeof ( SyzToSpc.SpKgz78 ) );
							}
						}
					}
					else {
						if ( Flg ) {
							if ( money[l][m].code == _T( "322002" ) ) {
								memmove ( SyzToSpc.SpKgz4, money[l][m].arith, sizeof ( SyzToSpc.SpKgz4 ) );
							}
							else if ( money[l][m].code == _T( "322003" ) ) {
								memmove ( SyzToSpc.SpKgz63, money[l][m].arith, sizeof ( SyzToSpc.SpKgz78 ) );
							}
						}
					}
				}
			}
			
		}
	}


	// 特定収入計算表対応
	if ( *( m_pShinInfo )->pSgSpc == 1 ) {
		f201calq_spc ( SyzToSpc );
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
int CShinFhyo2_01::CheckOver500MillionSub( int &MilSw, char *pVAL, char *pValCnv )
{
	int		nflg = 0;
	char	flg[MONY_BUF_SIZE] ={ 0 };
	char	tmpVAL[MONY_BUF_SIZE]={ 0 }, tmpValCnv[MONY_BUF_SIZE]={ 0 };

	// 入力ロック時は集計しない
	if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) == 0x00 ) {
		MoneyBasejagArray	money;
		money = (*m_pSyzSyukei)->GetShinkokuData(_T("32"));
		int cnt = (int)money.GetCount();
		int	incnt = 0;

		for (int l = 0; l < cnt; l++) {
			incnt = (int)money[l].GetCount();
			for (int m = 0; m < incnt; m++) {
				// 付表２-１データ
				// 旧税率分小計も取得する
				if (money[l][m].code == _T("322601"))
					memmove(F201_1X, money[l][m].arith, MONY_BUF_SIZE);
				if (money[l][m].code == _T("322602"))
					memmove(F201_1D, money[l][m].arith, MONY_BUF_SIZE);
				if (money[l][m].code == _T("322603"))
					memmove(F201_1E, money[l][m].arith, MONY_BUF_SIZE);
				if (money[l][m].code == _T("322700"))
					memmove(F201_2F, money[l][m].arith, MONY_BUF_SIZE);

				if (money[l][m].code == _T("323300")) {
					char wariai[32] = { 0 };
					m_pArith->l_print(wariai, money[l][m].arith, _T("SSSSSSSSSSSSS9"));
					F201_8EX = atoi(wariai);
				}
			}
		}
	}

	m_pArith->l_add(tmpVAL, F201_1X, F201_1D);
	m_pArith->l_add(tmpVAL, tmpVAL, F201_1E);
	m_pArith->l_add(tmpVAL, tmpVAL, F201_2F);

	int sw = (*m_pSnHeadData)->CheckTaxationSales(tmpVAL, tmpValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno);

	if( sw == 1 ){
		m_pArith->l_input( flg, _T("0") );
		nflg = 0;
	}
	else{
		if( F201_8EX >= 9500 ){
			m_pArith->l_input( flg, _T("1") );
			nflg = 1;
		}
		else {
			m_pArith->l_input( flg, _T("0") );
			nflg = 0;
		}
	}
	(*m_pSyzSyukei)->SetShinkokuData( _T("FF3000"), flg );
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
void CShinFhyo2_01::CheckOver500Million()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char	VAL[6] = {0};
	char	ValCnv[6] = {0};
	char	buf[256] = { 0 };

	memset( VAL, '\0', 6 );
	memset( ValCnv, '\0', 6 );
	memset ( buf, '\0', sizeof ( buf ) );
	//19.11.05
	m_pArith->l_add(VAL, F201_1X, F201_1D);
	m_pArith->l_add(VAL, VAL, F201_1E);
	m_pArith->l_add(VAL, VAL, F201_2F);
//	m_pArith->l_add(VAL, F201_1F, F201_2F);
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
			ChangeColor( IDC_ICSDIAG8CTRL2, MILLION, 5 );
			ChangeColor( IDC_ICSDIAG8CTRL2, MILLION + 10, 5 );
			memset( buf, '\0', sizeof( buf ) );

			if( m_pArith->l_cmp( VAL, ValCnv ) == 0 ){
				strcpy_s( buf, sizeof( buf ), "課税売上高が、５億円を超えています。\n" );
			}
			else{
				strcpy_s( buf, sizeof( buf ), "課税売上高が、年換算で５億円を超えています。\n" );
			}
			strcat_s( buf, sizeof( buf ), "比例配分方式の選択も含めて注意してください。" );
			DiagData.data_disp = (char *)buf;
			diag_setdata( IDC_ICSDIAG8CTRL1, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}		

	}
	else{			// ４億５千万以上で５億以下又は４億５千万未満

		//19.11.05
		char flg[MONY_BUF_SIZE] = { 0 };

		if (F201_8EX >= (*m_pSnHeadData)->m_UriRatioBorder) {
			m_pArith->l_input(flg, "1");
		}
		else {
			m_pArith->l_input(flg, "0");
		}
		(*m_pSyzSyukei)->SetShinkokuData(_T("FF3000"), flg);


		(*m_pSnHeadData)->m_UriRatioBorder = 9500;
		*m_pShinInfo->pOver500MillionSw = 0;
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
			ChangeColor( IDC_ICSDIAG8CTRL2, MILLION, 1 );
			ChangeColor( IDC_ICSDIAG8CTRL2, MILLION + 10, 1 );
			diag_clear( IDC_ICSDIAG8CTRL1, 3, TRUE, CIcsdiagctrl );
		}
		
	}

	m_Fhyo2_01Diag01.Refresh();
	m_Fhyo2_01Diag02.Refresh();
}
#else
//-----------------------------------------------------------------------------
// 課税売上高5億円越えチェック
//-----------------------------------------------------------------------------
void CShinFhyo2_01::CheckOver500Million()
{
	DIAGRAM_ATTRIBUTE DA;
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	COLORREF color;

	char	VAL[MONY_BUF_SIZE] ={ 0 };
	char	ValCnv[MONY_BUF_SIZE] ={ 0 };
	(*m_pSnHeadData)->Sn_Sign4 &= 0xef;
	CheckOver500MillionSub(*(m_pShinInfo->pOver500MillionSw), VAL, ValCnv);//19.11.05

	char	buf[256] ={ 0 };
	if( *m_pShinInfo->pOver500MillionSw == 1 ){	// ON:９５％ルール改正対応期間で５億円超え

		(*m_pSnHeadData)->m_UriRatioBorder = 11000;
		(*m_pSnHeadData)->Sn_Sign4 |= 0x10;
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
			//ChangeColor(IDC_ICSDIAG8CTRL2, MILLION, 5);
			//ChangeColor(IDC_ICSDIAG8CTRL2, MILLION + 10, 5);
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
			diag_setdata(IDC_ICSDIAG8CTRL1, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl);
		}
	}
	else{			// ４億５千万以上で５億以下又は４億５千万未満

		(*m_pSnHeadData)->m_UriRatioBorder = 9500;
		*m_pShinInfo->pOver500MillionSw = 0;
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
			//ChangeColor(IDC_ICSDIAG8CTRL2, MILLION, 1);
			//ChangeColor(IDC_ICSDIAG8CTRL2, MILLION + 10, 1);
			color = BC_WHITE;
			diag_clear(IDC_ICSDIAG8CTRL1, 3, TRUE, CIcsdiagctrl);
		}
	}

	//m_Fhyo2_01Diag01.Refresh();
	//m_Fhyo2_01Diag02.Refresh();

	// m_Fhyo2_01Diag02.Refresh()を呼ぶと解像度が1024×768のときに正常に表示されなくなる
	if ( ( *m_pSnHeadData )->IsKobetuSiireAnbun() == FALSE ) {
		diag_getattr ( IDC_ICSDIAG8CTRL2, MILLION, &DA, CIcsdiagctrl );
		DA.attr_bcolor = color;
		diag_setattr ( IDC_ICSDIAG8CTRL2, MILLION, &DA, TRUE, CIcsdiagctrl );

		diag_getattr ( IDC_ICSDIAG8CTRL2, MILLION + 10, &DA, CIcsdiagctrl );
		DA.attr_bcolor = color;
		diag_setattr ( IDC_ICSDIAG8CTRL2, MILLION + 10, &DA, TRUE, CIcsdiagctrl );
	}
}
#endif

//=====================================
// 棚卸データ（ダイアログ）の加減算を行う
//=====================================
void CShinFhyo2_01::TanaDataAdd()
{
	char	VAL_14D[6] = { 0 }, VAL_14E[6] = { 0 }, VAL_17D[6] = { 0 }, VAL_17E[6] = { 0 }, VAL_18D[6] = { 0 }, VAL_18E[6] = { 0 };

	char	VAL_14A[6] = { 0 }, VAL_14B[6] = { 0 }, VAL_14C[6] = { 0 };
	char	VAL_17A[6] = { 0 }, VAL_17B[6] = { 0 }, VAL_17C[6] = { 0 };
	char	VAL_18A[6] = { 0 }, VAL_18B[6] = { 0 }, VAL_18C[6] = { 0 };

	if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {	// 非連動は保存データで使用するので、再計算不要
		return;
	}
	else {
		// 連動時でも特例計算(5-(3)号様式)を摘要する場合は再計算しない
		if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x40 ) ) {
			return;
		}
	}

	if ( !((*m_pSnTanaData)->Sn_tansw & 0x01) ) {
		if ((*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE) {
			MoneyBasejagArray	money;
			char wariai[32];

			// 連動データを取得
			money = (*m_pSyzSyukei)->GetShinkokuData(_T("323300"));
			m_pArith->l_print(wariai, money[0][0].arith, _T("SSSSSSSSSSSSS9"));
			F201_8EX = atoi(wariai);
			CheckOver500Million();

			if ((*m_pSnTanaData)->Sn_MENZsw & 0x10) {
				m_pArith->l_add(VAL_14E, (*m_pSnTanaData)->Sn_mnz78[0], (*m_pSnTanaData)->Sn_mnz78[1]);
				m_pArith->l_add(VAL_14E, VAL_14E, (*m_pSnTanaData)->Sn_mnz78[2]);
				m_pArith->l_add(VAL_14D, (*m_pSnTanaData)->Sn_mnz624[0], (*m_pSnTanaData)->Sn_mnz624[1]);
				m_pArith->l_add(VAL_14D, VAL_14D, (*m_pSnTanaData)->Sn_mnz624[2]);

				m_pArith->l_add(VAL_14C, (*m_pSnTanaData)->Sn_mnz63[0], (*m_pSnTanaData)->Sn_mnz63[1]);
				m_pArith->l_add(VAL_14C, VAL_14C, (*m_pSnTanaData)->Sn_mnz63[2]);
				m_pArith->l_add(VAL_14B, (*m_pSnTanaData)->Sn_mnz40[0], (*m_pSnTanaData)->Sn_mnz40[1]);
				m_pArith->l_add(VAL_14B, VAL_14B, (*m_pSnTanaData)->Sn_mnz40[2]);
				m_pArith->l_add(VAL_14A, (*m_pSnTanaData)->Sn_menzz[0], (*m_pSnTanaData)->Sn_menzz[1]);
				m_pArith->l_add(VAL_14A, VAL_14A, (*m_pSnTanaData)->Sn_menzz[2]);

				// 旧格納場所へ格納
				memmove((*m_pSnTanaData)->Sn_MNZ78, VAL_14E, MONY_BUF_SIZE);
				memmove((*m_pSnTanaData)->Sn_MNZ624, VAL_14D, MONY_BUF_SIZE);

				memmove((*m_pSnTanaData)->Sn_MNZ63, VAL_14C, MONY_BUF_SIZE);
				memmove((*m_pSnTanaData)->Sn_MNZ40, VAL_14B, MONY_BUF_SIZE);
				memmove((*m_pSnTanaData)->Sn_MENZZ, VAL_14A, MONY_BUF_SIZE);

				if ((*m_pSnTanaData)->Sn_MENZsw & 0x01) {
					m_pArith->l_neg(VAL_14D);
					m_pArith->l_neg(VAL_14E);

					m_pArith->l_neg(VAL_14A);
					m_pArith->l_neg(VAL_14B);
					m_pArith->l_neg(VAL_14C);
				}

				// 個別対応で95％未満の場合
				if ((F201_8EX < (*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE)) {
					memmove(VAL_17E, (*m_pSnTanaData)->Sn_mnz78[0], MONY_BUF_SIZE);
					memmove(VAL_18E, (*m_pSnTanaData)->Sn_mnz78[2], MONY_BUF_SIZE);
					memmove(VAL_17D, (*m_pSnTanaData)->Sn_mnz624[0], MONY_BUF_SIZE);
					memmove(VAL_18D, (*m_pSnTanaData)->Sn_mnz624[2], MONY_BUF_SIZE);

					memmove(VAL_17C, (*m_pSnTanaData)->Sn_mnz63[0], MONY_BUF_SIZE);
					memmove(VAL_18C, (*m_pSnTanaData)->Sn_mnz63[2], MONY_BUF_SIZE);
					memmove(VAL_17B, (*m_pSnTanaData)->Sn_mnz40[0], MONY_BUF_SIZE);
					memmove(VAL_18B, (*m_pSnTanaData)->Sn_mnz40[2], MONY_BUF_SIZE);
					memmove(VAL_17A, (*m_pSnTanaData)->Sn_menzz[0], MONY_BUF_SIZE);
					memmove(VAL_18A, (*m_pSnTanaData)->Sn_menzz[2], MONY_BUF_SIZE);

					if ((*m_pSnTanaData)->Sn_MENZsw & 0x01) {
						m_pArith->l_neg(VAL_17E);
						m_pArith->l_neg(VAL_18E);
						m_pArith->l_neg(VAL_17D);
						m_pArith->l_neg(VAL_18D);

						m_pArith->l_neg(VAL_17C);
						m_pArith->l_neg(VAL_18C);
						m_pArith->l_neg(VAL_17B);
						m_pArith->l_neg(VAL_18B);
						m_pArith->l_neg(VAL_17A);
						m_pArith->l_neg(VAL_18A);
					}
				}
			}
		}
		else {
			if ((*m_pSnTanaData)->Sn_MENZsw & 0x10) {
				memmove(VAL_14D, (*m_pSnTanaData)->Sn_MNZ624, MONY_BUF_SIZE);
				memmove(VAL_14E, (*m_pSnTanaData)->Sn_MNZ78, MONY_BUF_SIZE);

				memmove(VAL_14A, (*m_pSnTanaData)->Sn_MENZZ, MONY_BUF_SIZE);
				memmove(VAL_14B, (*m_pSnTanaData)->Sn_MNZ40, MONY_BUF_SIZE);
				memmove(VAL_14C, (*m_pSnTanaData)->Sn_MNZ63, MONY_BUF_SIZE);

				if ((*m_pSnTanaData)->Sn_MENZsw & 0x01) {
					m_pArith->l_neg(VAL_14D);
					m_pArith->l_neg(VAL_14E);

					m_pArith->l_neg(VAL_14A);
					m_pArith->l_neg(VAL_14B);
					m_pArith->l_neg(VAL_14C);
				}
			}
		}

		(*m_pSyzSyukei)->SetShinkokuData(_T("323902"), VAL_14D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("323903"), VAL_14E);

		if ((*m_pSnHeadData)->SVmzsw & 0x01) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("321401"), VAL_14A);
			(*m_pSyzSyukei)->SetShinkokuData(_T("321402"), VAL_14B);
			(*m_pSyzSyukei)->SetShinkokuData(_T("321403"), VAL_14C);
		}
	}

	(*m_pSyzSyukei)->SetShinkokuData(_T("32A602"), VAL_17D);
	(*m_pSyzSyukei)->SetShinkokuData(_T("32A603"), VAL_17E);
	(*m_pSyzSyukei)->SetShinkokuData(_T("32A702"), VAL_18D);
	(*m_pSyzSyukei)->SetShinkokuData(_T("32A703"), VAL_18E);

	if ((*m_pSnHeadData)->SVmzsw & 0x01) {
		(*m_pSyzSyukei)->SetShinkokuData(_T("32A201"), VAL_17A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("32A202"), VAL_17B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("32A203"), VAL_17C);
		(*m_pSyzSyukei)->SetShinkokuData(_T("32A301"), VAL_18A);
		(*m_pSyzSyukei)->SetShinkokuData(_T("32A302"), VAL_18B);
		(*m_pSyzSyukei)->SetShinkokuData(_T("32A303"), VAL_18C);
	}
}

//-----------------------------------------------------------------------------
// 課税売上割合の取得
//-----------------------------------------------------------------------------
// 返送値	計算した課税売上割合
//-----------------------------------------------------------------------------
unsigned short CShinFhyo2_01::GetCalqedKzwari()
{
	unsigned short	rtKzwari = F201_8EX;

	if( (*m_pSnHeadData)->Sn_UPERS&0x10 ){		// 課税売上割合を変更

		if( (*m_pSnHeadData)->Sn_UPERS&0x02 ){	// 課税売上割合に準ずる割合を使用
												// 計算
			char	WORK0[MONY_BUF_SIZE] = {0};
			char	PW6[MONY_BUF_SIZE] = {0};
			char	PER100BUF[MONY_BUF_SIZE] = {0};
			m_pArith->l_input( PER100BUF, _T("10000") );
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F201_4F, (unsigned char *)F201_7F, 2, 0 );
			if( m_pArith->l_cmp(F201_4F, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F201_7F, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
				memmove( WORK0, PER100BUF, sizeof(WORK0) );
			}
			char	buf[64]={0};
			m_pArith->l_print( buf, WORK0, "SSSSS" );
			rtKzwari = atoi( buf );
		}
	}

	return rtKzwari;
}


void CShinFhyo2_01::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	// 経過措置対象の課税資産の譲渡等あり
	( ( CH31HyoView* )m_pParent )->SpecPassDlg ( 1 );
}



//===================================================
// 特定収入計算表を加味した控除対象仕入税額の計算
//===================================================
void CShinFhyo2_01::f201calq_spc ( SYZTOSPC_DATA SpcData )
{
	if ( *( m_pShinInfo->pSgSpc ) == 1 ) {
		unsigned char sign = ( *m_pSnHeadData )->Sn_Sign2;
		MoneyBasejagArray	money;
		int cnt = 0;
		int incnt = 0;
		unsigned short uwri = 0;

		if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
			uwri = F201_8F;
		}
		else {
			uwri = F201_8EX;
		}

		SYZTOSPC_DATA SyzToSpc = { 0 };
		memmove ( &SyzToSpc, &SpcData, sizeof ( SYZTOSPC_DATA ) );

		BOOL IsRatioOver95 = FALSE;
		BOOL Kobetsu = FALSE;

		//if ( F201_8EX >= ( *m_pSnHeadData )->m_UriRatioBorder ) {
		if ( uwri >= ( *m_pSnHeadData )->m_UriRatioBorder ) {
			IsRatioOver95 = TRUE;
			SyzToSpc.IsRatioOver95 |= 0x01;
		}

		Kobetsu = ( *m_pSnHeadData )->IsKobetuSiireAnbun();

		// 課税売上セット

		//---->yoshida191121
		//--> 191127
		money = (*m_pSyzSyukei)->GetShinkokuData(_T("310202"));
		memmove(SyzToSpc.SpKzur4, money[0][0].arith, MONY_BUF_SIZE);
		money = (*m_pSyzSyukei)->GetShinkokuData(_T("310203"));
		memmove(SyzToSpc.SpKzur63, money[0][0].arith, MONY_BUF_SIZE);
		//<-- 191127

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("312200"));
		memmove(SyzToSpc.SpKzur, money[0][0].arith, MONY_BUF_SIZE);

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("312202"));
		memmove(SyzToSpc.SpKzur624, money[0][0].arith, MONY_BUF_SIZE);

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("312203"));
		memmove(SyzToSpc.SpKzur78, money[0][0].arith, MONY_BUF_SIZE);

		m_pArith->l_add(SyzToSpc.SpMzur, F201_2F, F201_3F);
		memmove(SyzToSpc.SpHkur, F201_6F, sizeof(SyzToSpc.SpHkur));
		//-----
		/*
		memmove ( SyzToSpc.SpKzur, F201_1F, sizeof ( SyzToSpc.SpKzur ) );
		m_pArith->l_add ( SyzToSpc.SpMzur, F201_2F, F201_3F );
		memmove ( SyzToSpc.SpHkur, F201_6F, sizeof ( SyzToSpc.SpHkur ) );
		memmove ( SyzToSpc.SpKzur624, F201_1D, sizeof ( SyzToSpc.SpKzur624 ) );
		memmove ( SyzToSpc.SpKzur78, F201_1E, sizeof ( SyzToSpc.SpKzur78 ) );
		*/
		//<-----

		memmove ( SyzToSpc.SpKuws, F201_4F, MONY_BUF_SIZE );
		memmove ( SyzToSpc.SpKuwb, F201_7F, MONY_BUF_SIZE );

		// 課税売上割合	
		money = ( *m_pSyzSyukei )->GetShinkokuData ( _T( "323300" ) );
		memmove ( SyzToSpc.SpUwri, money[0][0].arith, MONY_BUF_SIZE );


		// 控除対象仕入税額セット
		//if ( IsRatioOver95 ) {
		// 課税売上割合95%以上かつ、課税売上割合に準ずる割合を使用していない
		if ( IsRatioOver95 && ( ( *m_pSnHeadData )->IsUseSemiUriRatio() == FALSE ) ) {
			// 計算表５（１）
			memmove( SyzToSpc.SpKgz624, F201_16D, sizeof ( SyzToSpc.SpKgz624 ) );
			memmove( SyzToSpc.SpKgz78, F201_16E, sizeof ( SyzToSpc.SpKgz78 ) );
		}
		else {
			if ( Kobetsu ) {
				// 計算表５（２）
				memmove( SyzToSpc.SpKgz624, F201_19D, sizeof ( SyzToSpc.SpKgz624 ) );
				memmove( SyzToSpc.SpKgz78, F201_19E, sizeof ( SyzToSpc.SpKgz78 ) );
			}
			else {
				// 計算表５（３）
				memmove( SyzToSpc.SpKgz624, F201_20D, sizeof ( SyzToSpc.SpKgz624 ) );
				memmove( SyzToSpc.SpKgz78, F201_20E, sizeof ( SyzToSpc.SpKgz78 ) );
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
			memmove ( mony, SyzToSpc.SpSiz624, sizeof ( mony ) );

			if ( m_pArith->l_test ( mony ) < 0 ) {
				// マイナスの場合符号反転
				m_pArith->l_neg ( mony );

				// 24D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324902" ), mony );
				// 23D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324802" ), zero );
				// 付表1-1 4D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312602" ), zero );
			}
			else {
				// 24D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324902" ), zero );
				// 23D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324802" ), mony );
				// 付表1-1 4D欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312602" ), mony );
			}
		
			memmove ( mony, SyzToSpc.SpSiz78, sizeof ( mony ) );
			if ( m_pArith->l_test ( mony ) < 0 ) {
				// マイナスの場合符号反転
				m_pArith->l_neg ( mony );

				// 24E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), mony );
				// 23E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324803" ), zero );
				// 付表1-1 4E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312603" ), zero );
			}
			else {
				// 24E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324903" ), zero );
				// 23E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "324803" ), mony );
				// 付表1-1 4E欄へ
				( *m_pSyzSyukei )->SetShinkokuData ( _T( "312603" ), mony );
			}
		
			if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
				memmove ( mony, SyzToSpc.SpSiz4, sizeof ( mony ) );
				if ( m_pArith->l_test ( mony ) < 0 ) {
					// マイナスの場合符号反転
					m_pArith->l_neg ( mony );

					// 付表2-2 24B欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322402" ), mony );
					// 付表2-2 23B欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322302" ), zero );
					// 付表1-2 4B欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "310602" ), zero );
				}
				else {
					// 付表2-2 24B欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322402" ), zero );
					// 付表2-2 23B欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322302" ), mony );
					// 付表1-2 4B欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "310602" ), mony );
				}

				memmove ( mony, SyzToSpc.SpSiz63, sizeof ( mony ) );
				if ( m_pArith->l_test ( mony ) < 0 ) {
					// マイナスの場合符号反転
					m_pArith->l_neg ( mony );

					// 付表2-2 24C欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322403" ), mony );
					// 付表2-2 23C欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322303" ), zero );
					// 付表1-2 4C欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "310603" ), zero );
				}
				else {
					// 付表2-2 24C欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322403" ), zero );
					// 付表2-2 23C欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "322303" ), mony );
					// 付表1-2 4C欄へ
					( *m_pSyzSyukei )->SetShinkokuData ( _T( "310603" ), mony );
				}
			}

			// 特定収入連動メッセージ表示
			m_Fhyo2_01Diag02.ModifyItem ( ID201_TS, DIAG_MDFY_OPAQUE );

			// 簡易計算式ボタン表示
			m_Button2.ShowWindow ( SW_SHOW );

			for( int index = ID201_23D; index <= ID201_23E; index++ ){
				ATRIB_MOD( IDC_ICSDIAG8CTRL2, index, 0, ( *m_pSnHeadData )->Sn_Sign4, 0 );
			}
		}
		else {
			// 特定収入割合5%未満
			if ( m_SpcSgn ) {
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312602" ) );	// 付表1-1 4D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312603" ) );	// 付表1-1 4E欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324802" ) );	// 23D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324803" ) );	// 23E欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324902" ) );	// 24D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "324903" ) );	// 24E欄

				if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310602" ) );	// 付表1-2 4B欄
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310603" ) );	// 付表1-2 4C欄
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322302" ) );	// 付表2-2 23B欄
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322303" ) );	// 付表2-2 23C欄
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322402" ) );	// 付表2-2 24B欄
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "322403" ) );	// 付表2-2 24C欄
				}

				F201_23c &= ~0x80;
				F201_23c &= ~0x800;

				// 特定収入連動メッセージ非表示
				m_Fhyo2_01Diag02.ModifyItem ( ID201_TS, DIAG_MDFY_TRANSPARENT );

				// 簡易計算式ボタン非表示
				m_Button2.ShowWindow ( SW_HIDE );

				m_SpcSgn = FALSE;

				for( int index = ID201_23D; index <= ID201_23E; index++ ){
					ATRIB_MOD( IDC_ICSDIAG8CTRL2, index, 1, ( *m_pSnHeadData )->Sn_Sign4, 0 );
				}
			}
			else{
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312602" ) );	// 付表1-1 4D欄
				( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312603" ) );	// 付表1-1 4E欄

				if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310602" ) );	// 付表1-2 4B欄
					( *m_pSyzSyukei )->ResetShinkokuData ( _T( "310603" ) );	// 付表1-2 4C欄
				}
			}
		}

		money = (*m_pSyzSyukei)->GetShinkokuData(_T("32"));
		cnt = ( int )money.GetCount();
		incnt = 0;

		for ( int l = 0; l < cnt; l++ ) {
			incnt = ( int )money[l].GetCount();
			for ( int m = 0; m < incnt; m++ ) {
				if ( money[l][m].code == _T( "324801" ) ) {
					memmove ( F201_23X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324802" ) ) {
					memmove ( F201_23D, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324803" ) ) {
					memmove ( F201_23E, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324800" ) ) {
					memmove ( F201_23F, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324901" ) ) {
					memmove ( F201_24X, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324902" ) ) {
					memmove ( F201_24D, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324903" ) ) {
					memmove ( F201_24E, money[l][m].arith, MONY_BUF_SIZE );
				}
				else if ( money[l][m].code == _T( "324900" ) ) {
					memmove ( F201_24F, money[l][m].arith, MONY_BUF_SIZE );
				}
			}
		}
	}
}

//=========================================================
// 手入力項目あり？
//---------------------------------------------------------
// 返送値
//	TRUE		:		あり
//	FALSE		:		なし
//=========================================================
BOOL CShinFhyo2_01::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( ( F201_10c & 0x880 ) || ( F201_12c & 0x800 ) || ( F201_19c & 0x880 ) ||
			 ( F201_20c & 0x880 ) || ( F201_23c & 0x880 ) ) {
			ret = TRUE;
		}
	}

	return ret;
}