// ShinFhyo1_01.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo1_01.h"
#include "ShinFhyo1_01Idx.h"
#include "H31HyoView.h"

// CShinFhyo1_01 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo1_01, CSyzBaseDlg)

CShinFhyo1_01::CShinFhyo1_01(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo1_01::IDD, pParent)
	, m_pSnFhyo1_01Data(NULL)
	, m_pSnHeadData(NULL)
	, m_pShinInfo(NULL)
	, m_pSyzSyukei(NULL)
	, m_pParent(pParent)
	, m_FrSw(0)
{

	memset( F101_1X, '\0', sizeof(F101_1X) );
	memset( F101_1D, '\0', sizeof(F101_1D) );
	memset( F101_1E, '\0', sizeof(F101_1E) );
	memset( F101_1F, '\0', sizeof(F101_1F) );

	memset( F101_1_1X, '\0', sizeof(F101_1_1X) );
	memset( F101_1_1D, '\0', sizeof(F101_1_1D) );
	memset( F101_1_1E, '\0', sizeof(F101_1_1E) );
	memset( F101_1_1F, '\0', sizeof(F101_1_1F) );

	memset( F101_1_2X, '\0', sizeof(F101_1_2X) );
	memset( F101_1_2E, '\0', sizeof(F101_1_2E) );
	memset( F101_1_2F, '\0', sizeof(F101_1_2F) );

	memset( F101_2X, '\0', sizeof(F101_2X) );
	memset( F101_2D, '\0', sizeof(F101_2D) );
	memset( F101_2E, '\0', sizeof(F101_2E) );
	memset( F101_2F, '\0', sizeof(F101_2F) );

	memset( F101_3X, '\0', sizeof(F101_3X) );
	memset( F101_3D, '\0', sizeof(F101_3D) );
	memset( F101_3E, '\0', sizeof(F101_3E) );
	memset( F101_3F, '\0', sizeof(F101_3F) );

	memset( F101_4X, '\0', sizeof(F101_4X) );
	memset( F101_4D, '\0', sizeof(F101_4D) );
	memset( F101_4E, '\0', sizeof(F101_4E) );
	memset( F101_4F, '\0', sizeof(F101_4F) );

	memset( F101_5X, '\0', sizeof(F101_5X) );
	memset( F101_5D, '\0', sizeof(F101_5D) );
	memset( F101_5E, '\0', sizeof(F101_5E) );
	memset( F101_5F, '\0', sizeof(F101_5F) );

	memset( F101_5_1X, '\0', sizeof(F101_5_1X) );
	memset( F101_5_1D, '\0', sizeof(F101_5_1D) );
	memset( F101_5_1E, '\0', sizeof(F101_5_1E) );
	memset( F101_5_1F, '\0', sizeof(F101_5_1F) );

	memset( F101_5_2X, '\0', sizeof(F101_5_2X) );
	memset( F101_5_2E, '\0', sizeof(F101_5_2E) );
	memset( F101_5_2F, '\0', sizeof(F101_5_2F) );

	memset( F101_6X, '\0', sizeof(F101_6X) );
	memset( F101_6D, '\0', sizeof(F101_6D) );
	memset( F101_6E, '\0', sizeof(F101_6E) );
	memset( F101_6F, '\0', sizeof(F101_6F) );

	memset( F101_7X, '\0', sizeof(F101_7X) );
	memset( F101_7D, '\0', sizeof(F101_7D) );
	memset( F101_7E, '\0', sizeof(F101_7E) );
	memset( F101_7F, '\0', sizeof(F101_7F) );

	memset( F101_8X, '\0', sizeof(F101_8X) );
	memset( F101_8D, '\0', sizeof(F101_8D) );
	memset( F101_8E, '\0', sizeof(F101_8E) );
	memset( F101_8F, '\0', sizeof(F101_8F) );

	memset( F101_9X, '\0', sizeof(F101_9X) );
	memset( F101_9D, '\0', sizeof(F101_9D) );
	memset( F101_9E, '\0', sizeof(F101_9E) );
	memset( F101_9F, '\0', sizeof(F101_9F) );

	memset( F101_10F, '\0', sizeof(F101_10F) );

	memset( F101_11X, '\0', sizeof(F101_11X) );
	memset( F101_11E, '\0', sizeof(F101_11E) );
	memset( F101_11F, '\0', sizeof(F101_11F) );

	memset( F101_12X, '\0', sizeof(F101_12X) );
	memset( F101_12E, '\0', sizeof(F101_12E) );
	memset( F101_12F, '\0', sizeof(F101_12F) );

	memset( F101_13X, '\0', sizeof(F101_13X) );
	memset( F101_13E, '\0', sizeof(F101_13E) );
	memset( F101_13F, '\0', sizeof(F101_13F) );

	memset( F101_14X, '\0', sizeof(F101_14X) );
	memset( F101_14E, '\0', sizeof(F101_14E) );
	memset( F101_14F, '\0', sizeof(F101_14F) );

	memset( F101_15X, '\0', sizeof(F101_15X) );
	memset( F101_15E, '\0', sizeof(F101_15E) );
	memset( F101_15F, '\0', sizeof(F101_15F) );

	memset( F101_16F, '\0', sizeof(F101_16F) );


	m_curwnd = -1;

	F101_1c = 0;
	F101_2c = 0;
	F101_5c = 0;
	
}

CShinFhyo1_01::~CShinFhyo1_01()
{
}

void CShinFhyo1_01::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Fhyo1_01Diag01);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Fhyo1_01Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo1_01, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CShinFhyo1_01 メッセージ ハンドラ

BOOL CShinFhyo1_01::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期化
	Init();

	//コントロール一番最下のDiagのID指定。これで縦スクロール出る
	m_pBottomDiag = (CIcsdiagctrl*)GetDlgItem(IDC_ICSDIAG8CTRL2);
	m_BottomIdx = 222;


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
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo1_01::InitInfo(  CH31SnFhyo1_01Data **pSnFhyo1_01Data, CSnHeadData **pSnHeadData , SYC_SHININFO *pShinInfo, CArithEx *pArith, CSyzSyukei **pSyzSyukei)
{

	ASSERT( pSnFhyo1_01Data );
	if( pSnFhyo1_01Data == NULL ){
		return -1;
	}
	m_pSnFhyo1_01Data = pSnFhyo1_01Data;
	
	
	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;


	ASSERT( pShinInfo );
	if( pShinInfo == NULL ){
		return -1;
	}
	m_pShinInfo = pShinInfo;

	ASSERT ( pArith );
	if ( pArith == NULL ) {
		return -1;
	}
	m_pArith = pArith;

	ASSERT(pSyzSyukei);
	if (pSyzSyukei == NULL) {
		return -1;
	}
	m_pSyzSyukei = pSyzSyukei;

	return 0;
}





//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo1_01::Init()
{
	
	// データ読込み
	ReadData( m_FrSw );
	m_FrSw = 1;

	// 読み込みデータを集計モジュールにセット
	ReadData2();

	MemoryReget();

	// ダイアグラム初期属性セット
	InitDiagAttr();

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		m_Fhyo1_01Diag01.ModifyItem ( 4, DIAG_MDFY_TRANSPARENT );
	}

	// 令和2年4月1日改正様式を使用するのチェックがONの場合のみ
	// 「経過措置対象・・・」の文言を表示
	if ( !( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) ) {
		m_Fhyo1_01Diag01.ModifyItem ( 2, DIAG_MDFY_TRANSPARENT );
	}

	// 画面出力
	DispFh1_01();

	SetItemPosition();

	return 0;
}


//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
void CShinFhyo1_01::SetItemPosition()
{
	int index = 0;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
		//index = m_Fhyo1_01Diag02.GetPosition();
		if( (index = m_Fhyo1_01Diag02.GetPosition()) != -1) {
			m_Fhyo1_01Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット
			m_Fhyo1_01Diag02.SetPosition(index);
			m_Fhyo1_01Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo1_01Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット
	m_Fhyo1_01Diag02.SetFocus();

	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) {
			if ( ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) || ( ( *m_pSnHeadData )->m_DispTabSgn & 0x20 ) ) && ( ( *m_pSnHeadData )->IsNeedTkkzDisp() ) ) {
				// 特定課税仕入へ
				diag_setposition ( IDC_ICSDIAG8CTRL2, ID101_1_2E, CIcsdiagctrl );
			}
			else {
				// 消費税額へ
				diag_setposition ( IDC_ICSDIAG8CTRL2, ID101_2D, CIcsdiagctrl );
			}
		}
		else {
			//課税標準額へ
			diag_setposition(IDC_ICSDIAG8CTRL2, ID101_1_1D, CIcsdiagctrl);//yoshida190613
		}
	}
}




void CShinFhyo1_01::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo1_01::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CShinFhyo1_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
int CShinFhyo1_01::InitDiagAttr()
{
	int	cnt = 0;
	while( 1 ){
		if( Fhyo1_01[cnt].Index == 0 ){
			break;
		}

		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Fhyo1_01[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Fhyo1_01[cnt].UnConnect;
				//if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) || ( ( *m_pSnHeadData )->m_DispTabSgn & 0x20 ) ) 
				if( ( !((CH31HyoView*)m_pParent)->IsCrossOverR050930() && IsUseTokurei() )	// 20231025 令和5年9月30日をまたぐ場合は入力可
					&& ( ( Fhyo1_01[cnt].Index == ID101_1_1D ) || ( Fhyo1_01[cnt].Index == ID101_1_1E ) ) ) {
					Flg = 0;
				}
				else {
//--> '15.08.20 INS START
					int st = ChkTkkzIdx( Fhyo1_01[cnt].Index );
					if( st == 1 ){
						if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
							Flg = 1;
						}
						else{
							Flg = 0;
						}
					}
					else if( st == 2 ){
						if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
							Flg = 0;
						}
						else{
							Flg = 1;
						}
					}
				}
//<-- '15.08.20 INS END
			}
			else{
				Flg = Fhyo1_01[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAG8CTRL2, Fhyo1_01[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Fhyo1_01[cnt].Connect );
		cnt++;
	}

	m_Fhyo1_01Diag02.EnableDelete(0);//DELETEキー１回で削除モード設定

	return 0;
}

//-----------------------------------------------------------------------------
// 特定課税仕入に関連するインデックス？
//-----------------------------------------------------------------------------
// 引数		idx	：	チェック対象
//-----------------------------------------------------------------------------
// 返送値	0	：	特定課税仕入に関連しない
//			1	：	特定課税仕入で入力可能になるインデックス
//			2	：	特定課税仕入で入力不可になるインデックス
//-----------------------------------------------------------------------------
int CShinFhyo1_01::ChkTkkzIdx( short idx )
{
	int rSt = 0;

	//---->yoshida190613
	//特定課税仕入れでなくても、 課税資産の譲渡等の対価の額が入力可とならないといけない感じなので
	//課税標準額を入力不可とし、課税資産の譲渡等の対価の額は入力可とする仕様に変更
	if (((ID101_1_2X <= idx) && (idx <= ID101_1_2F)) || ((ID101_5_2X <= idx) && (idx <= ID101_5_2F))) {
		rSt = 1;
	}

	//⑤も同様につ似に入力不可に。
//	else if (((ID101_5X <= idx) && (idx <= ID101_5F))) {
//		rSt = 2;
//	}
	//-------------------
	/*
	if( ((ID101_1_1X<=idx)&&(idx<=ID101_1_2F)) || ((ID101_5_1X<=idx)&&(idx<=ID101_5_2F)) ){
		rSt = 1;
	}
	else if( ((ID101_1X<=idx)&&(idx<=ID101_1F)) || ((ID101_5X<=idx)&&(idx<=ID101_5F)) ){
		rSt = 2;
	}
	*/
	//<--------------------

	return rSt;
}


//-----------------------------------------------------------------------------
// 入力項目色変え
//-----------------------------------------------------------------------------
// 引数	id		；	ダイアグラムID
//		index	：	ダイアグラム内インデックス
//		sign	：	変更サイン
//-----------------------------------------------------------------------------
void CShinFhyo1_01::ChangeColor( unsigned short id, short Index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, Index, &DA, CIcsdiagctrl );
	switch( sign ){
		case 1 :	// 入力不可	（白色）
			DA.attr_bcolor = BC_WHITE;
			break;
		case 2 :	// ロック	（灰色）
			DA.attr_bcolor = BC_GRAY;
			break;
		case 3 :	// 特殊色①	（青色）
			DA.attr_bcolor = BC_BLUE;
			break;
		case 4 :	// 特殊色②	（緑色）	
			DA.attr_bcolor = BC_GREEN;
			break;
		case 5 :	// 特殊色③	（薄緑色）	 
			DA.attr_bcolor = BC_GREEN_L;
			break;
		default:	// 実額入力	（クリーム色） 
			DA.attr_bcolor = BC_CREAM;
			break;
	}
	diag_setattr( id, Index, &DA, FALSE, CIcsdiagctrl );
}


//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinFhyo1_01::UpDownFocusMoveCheck(int Index , int Move)
{
	int idx = 0;
	int i = 0 , j = 0;

	DIAGRAM_ATTRIBUTE	DA;

	//現在ポジション検索
	for(i = 0 ; i < FCS_FHY1_01CNT ; i++){
		if(Index == FCS_Fhyo1_01[i].IDNo){
			break;
		}
	}

	idx = FCS_Fhyo1_01[i].IDNo;//現在ポジションセット

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_FHY1_01CNT ; j++){
			m_Fhyo1_01Diag02.GetAttribute(FCS_Fhyo1_01[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo1_01[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Fhyo1_01Diag02.GetAttribute(FCS_Fhyo1_01[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo1_01[j].IDNo;
				break;
			}
		}
	}

	return idx;
}BEGIN_EVENTSINK_MAP(CShinFhyo1_01, CSyzBaseDlg)
ON_EVENT(CShinFhyo1_01, IDC_ICSDIAG8CTRL2, 1, CShinFhyo1_01::EditONIcsdiag8ctrl2, VTS_I2)
ON_EVENT(CShinFhyo1_01, IDC_ICSDIAG8CTRL2, 2, CShinFhyo1_01::EditOFFIcsdiag8ctrl2, VTS_I2)
ON_EVENT(CShinFhyo1_01, IDC_ICSDIAG8CTRL2, 3, CShinFhyo1_01::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo1_01::EditONIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL2;
}


void CShinFhyo1_01::EditOFFIcsdiag8ctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char				CkMy[MONY_BUF_SIZE];
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	//---->
	//char ksute1000[MONY_BUF_SIZE] = { 0 };
	//m_pArith->l_input(ksute1000, _T("1000"));
	//<----

	switch( index ){
		case ID101_1D:			//   課税標準額　税率6.24％適用分　D
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			diag_getdata ( IDC_ICSDIAG8CTRL2, ID101_1D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );

			//------------>
			/*
			//端数処理仮作成。最終は集計モジュールや集計テーブル計算処理にもっていく
			m_pArith->l_div(CkMy, CkMy, ksute1000);
			m_pArith->l_mul(CkMy, CkMy, ksute1000);


			dsp_prs(IDC_ICSDIAG8CTRL2, ID101_1D, CkMy);
			*/
			//<------------

			if ( m_pArith->l_cmp ( CkMy, F101_1D ) ) {
				if ( !( F101_1c & 0x80 ) ) {	// 手入力されていない場合のみサインたてる
					F101_1c |= 0x10;
				}	
			}

			// 再計算
			SyukeiDataSet(index);
			f101data_all(1);
			break;

		case ID101_1E:			//   課税標準額　税率7.8％適用分　E
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			diag_getdata ( IDC_ICSDIAG8CTRL2, ID101_1E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );

			//------------>
			/*
			//端数処理仮作成。最終は集計モジュールや集計テーブル計算処理にもっていく
			m_pArith->l_div(CkMy, CkMy, ksute1000);
			m_pArith->l_mul(CkMy, CkMy, ksute1000);

			dsp_prs(IDC_ICSDIAG8CTRL2, ID101_1E, CkMy);
			*/
			//<------------

			if ( m_pArith->l_cmp ( CkMy, F101_1E ) ) {
				if ( !( F101_1c & 0x800 ) ) {	// 手入力されていない場合のみサインたてる
					F101_1c |= 0x100;
				}
			}

			// 再計算
			SyukeiDataSet(index);
			f101data_all(1);
			break;

		case ID101_1_1D:		// 課税資産の譲渡等の対価の額 税率6.24%適用 D
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			diag_getdata ( IDC_ICSDIAG8CTRL2, ID101_1_1D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( m_pArith->l_cmp ( CkMy, F101_1_1D ) ) {
				if ( !( F101_1c & 0x80 ) ) {	// 手入力されていない場合のみサインたてる
					F101_2c |= 0x10;
				}
				if ( !( F101_2c & 0x80 ) ) {
					F101_2c |= 0x10;
				}
			}

			// 再計算
			SyukeiDataSet(index);
			f101data_all(1);
			break;

		case ID101_1_1E:		// 課税資産の譲渡等の対価の額 税率7.8%適用 E
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			diag_getdata ( IDC_ICSDIAG8CTRL2, ID101_1_1E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( m_pArith->l_cmp ( CkMy, F101_1_1E ) ) {
				if ( !( F101_1c & 0x800 ) ) {	// 手入力されていない場合のみサインたてる
					F101_1c |= 0x100;
				}
				if ( !( F101_2c & 0x800 ) ) {
					F101_2c |= 0x100;
				}
			}

			// 再計算
			SyukeiDataSet(index);
			f101data_all(1);
			break;

		case ID101_1_2E:		// 特定課税仕入れに係る支払い対価の額 合計 F
			memset( CkMy, '\0', MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAG8CTRL2, ID101_1_2E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( m_pArith->l_cmp( CkMy, F101_1_2E ) ) {
				if ( !( F101_1c & 0x800 ) ) {
					F101_1c |= 0x100;
				}
			}

			SyukeiDataSet(index);
			f101data_all(1);	
			break;

		case ID101_2D:			// 消費税額 税率6.24%適用分 D
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			diag_getdata ( IDC_ICSDIAG8CTRL2, ID101_2D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( m_pArith->l_cmp ( CkMy, F101_2D ) ) {
				F101_2c |= 0x80;
				F101_2c &= ~0x10;
			
			}

			// 再計算
			SyukeiDataSet(index);
			f101data_all(2);
			break;

		case ID101_2E:			// 消費税額 税率7.8%適用分 E
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			diag_getdata ( IDC_ICSDIAG8CTRL2, ID101_2E, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( m_pArith->l_cmp ( CkMy, F101_2E ) ) {
				F101_2c |= 0x800;
				F101_2c &= ~0x100;				
			}

			// 再計算
			SyukeiDataSet(index);
			f101data_all(2);
			break;

		case ID101_5D:		// 返還等対価に係る税額 税率6.24%適用分 D
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			diag_getdata ( IDC_ICSDIAG8CTRL2, ID101_5D, ( struct IUknown* )&DiagData, CIcsdiagctrl );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( m_pArith->l_cmp ( CkMy, F101_5D ) ) {
				F101_5c |= 0x80;
				F101_5c &= ~0x10;
			}

			// 再計算
			SyukeiDataSet(index);
			f101data_all(5);
			break;

		case ID101_5E:		//　返還等対価に係る税額 税率7.8%適用分 E
			memset ( CkMy, '\0', MONY_BUF_SIZE );
			diag_getdata ( IDC_ICSDIAG8CTRL2, ID101_5E, ( struct IUknown* )&DiagData, CIcsdiagctrl );
			memmove ( CkMy, DiagData.data_val, MONY_BUF_SIZE );
			if ( m_pArith->l_cmp ( CkMy, F101_5E ) ) {
				F101_5c |= 0x800;
				F101_5c &= ~0x100;
			}

			SyukeiDataSet(index);
			f101data_all(5);
			break;

		case ID101_5_1D:	// 売上の返還等対価に係る税額 D
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID101_5_1D, (struct IUknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
			if (m_pArith->l_cmp(CkMy, F101_5_1D)) {
				if (!(F101_5c & 0x80)) {
					F101_5c |= 0x10;
				}
			}

			SyukeiDataSet(index);
			f101data_all(5);
			break;

		case ID101_5_1E:	// 売上の返還等対価に係る税額 E
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID101_5_1E, (struct IUknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
			if (m_pArith->l_cmp(CkMy, F101_5_1E)) {
				if (!(F101_5c & 0x800)) {
					F101_5c |= 0x100;
				}
			}

			SyukeiDataSet(index);
			f101data_all(5);
			break;

		case ID101_5_2E:	// 特定課税仕入れの返還等対価に係る税額
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID101_5_2E, (struct IUknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
			if (m_pArith->l_cmp(CkMy, F101_5_2E)) {
				if (!(F101_5c & 0x800)) {
					F101_5c |= 0x100;
				}
			}

			SyukeiDataSet(index);
			f101data_all(5);
			break;

		case ID101_6D:		// 貸倒れに係る税額 D
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID101_6D, (struct IUknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
			if ( m_pArith->l_cmp ( CkMy, F101_6D ) ) {
	
			}

			SyukeiDataSet(index);
			f101data_all(6);
			break;

		case ID101_6E:		// 貸倒れに係る税額 E
			memset(CkMy, '\0', MONY_BUF_SIZE);
			diag_getdata(IDC_ICSDIAG8CTRL2, ID101_6E, (struct IUknown*)&DiagData, CIcsdiagctrl);
			memmove(CkMy, DiagData.data_val, MONY_BUF_SIZE);
			if ( m_pArith->l_cmp ( CkMy, F101_6E ) ) {
				
			}

			SyukeiDataSet(index);
			f101data_all(6);
			break;

		default:
			break;
	}
}

void CShinFhyo1_01::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
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

	int pos = 0;
	if( wTerm==VK_UP ){
		pos = UpDownFocusMoveCheck(index , 1);
		m_Fhyo1_01Diag02.SetPosition(pos);
	}
	if( wTerm==VK_DOWN ){
		pos = UpDownFocusMoveCheck(index , 0);
		m_Fhyo1_01Diag02.SetPosition(pos);
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
		m_Fhyo1_01Diag02.SetNextPosition();
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		m_Fhyo1_01Diag02.SetPrevPosition();
	}

	if(wTerm == VK_DELETE ){
		switch( index ){

			case ID101_1D:			//   課税標準額　税率6.24%適用分　D
				F101_1c &= ~0x80;
				F101_1c |= 0x10;
				dsp_cls ( IDC_ICSDIAG8CTRL2, index );
				break;
			case ID101_1E:			//   課税標準額　税率7.8%適用分　E
				F101_1c &= ~0x800;
				F101_1c |= 0x100;
				dsp_cls ( IDC_ICSDIAG8CTRL2, index );
				break;

			case ID101_1_1D:		// 課税資産の譲渡等の対価の額　D
				F101_1c &= ~0x80;
				F101_1c |= 0x10;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;
			case ID101_1_1E:		// 課税資産の譲渡等の対価の額　E
				F101_1c &= ~0x800;
				F101_1c |= 0x100;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;

			case ID101_1_2E:		// 特定課税仕入れに係る支払い対価の額　E
				F101_1c &= ~0x800;
				F101_1c |= 0x100;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;


			case ID101_2D:			//   消費税額　税率6.24%適用分　D
				F101_2c &= ~0x80;
				F101_2c |= 0x10;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("312402"));
				f101data_all(2);
				m_Fhyo1_01Diag02.SetPosition(index);
				break;
			case ID101_2E:			//   消費税額　税率7.8%適用分　E
				F101_2c &= ~0x800;
				F101_2c |= 0x100;
				(*m_pSyzSyukei)->ResetShinkokuData(_T("312403"));
				f101data_all(2);
				m_Fhyo1_01Diag02.SetPosition(index);
				break;


			case ID101_5D:			//   返還等対価に係る税額　税率6.24%適用分　D
				F101_5c &= ~0x80;
				F101_5c |= 0x100;
				dsp_cls ( IDC_ICSDIAG8CTRL2, index );
				break;
			case ID101_5E:			//   返還等対価に係る税額　税率7.8%適用分　E
				F101_5c &= ~0x800;
				F101_5c |= 0x100;
				dsp_cls ( IDC_ICSDIAG8CTRL2, index );
				break;
			case ID101_5_1D:			//   売上の返還等対価に係る税額　税率6.24%適用分　D
				F101_5c &= ~0x80;
				F101_5c |= 0x10;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;
			case ID101_5_1E:			//   売上の返還等対価に係る税額　税率7.8%適用分　E
				F101_5c &= ~0x800;
				F101_5c |= 0x100;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;
			case ID101_5_2E:			//   特定課税仕入れの返還等対価に係る税額　税率7.8%適用分　E
				F101_5c &= ~0x800;
				F101_5c |= 0x100;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;
			case ID101_6D:			//   貸倒れに係る税額　税率6.24%適用分　D
				F101_5c &= ~0x80;
				F101_5c |= 0x10;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;
			case ID101_6E:			//   貸倒れに係る税額　税率7.8%適用分　E
				F101_5c &= ~0x800;
				F101_5c |= 0x100;
				dsp_cls(IDC_ICSDIAG8CTRL2, index);
				break;
			default:
				dsp_cls ( IDC_ICSDIAG8CTRL2, index );
				break;
		}

	}

	int nowpos = m_Fhyo1_01Diag02.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAG8CTRL2, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}


}

BOOL CShinFhyo1_01::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			// 付表１更新
			GetAllDiagData();
//			f101data_all(0);
			WriteData();

			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}

			return TRUE;
		}
		if( pMsg->wParam == VK_F2 ){
			DIAGRAM_DATA   dd;
			if( m_curwnd != -1) {
				short result = ((CIcsdiagctrl*)GetDlgItem( m_curwnd ))->GetPosition();
				diag_getdata( m_curwnd, result, &dd, CIcsdiagctrl );
				if( m_curwnd == IDC_ICSDIAG8CTRL2 ) {
					TerminationIcsdiag8ctrl2( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}

	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------
// データの読込み
//-----------------------------------------------------------------------------
// 引数		pas	：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFhyo1_01::ReadData( int pas )
{

	// 課税標準額
	memmove( F101_1X, (*m_pSnFhyo1_01Data)->Sn_F101_1X, MONY_BUF_SIZE );
	memmove( F101_1D, (*m_pSnFhyo1_01Data)->Sn_F101_1D, MONY_BUF_SIZE );
	memmove( F101_1E, (*m_pSnFhyo1_01Data)->Sn_F101_1E, MONY_BUF_SIZE );
	memmove( F101_1F, (*m_pSnFhyo1_01Data)->Sn_F101_1F, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
//	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		memmove( F101_1_1X, (*m_pSnFhyo1_01Data)->Sn_F101_1_1X, MONY_BUF_SIZE );
		memmove( F101_1_1D, (*m_pSnFhyo1_01Data)->Sn_F101_1_1D, MONY_BUF_SIZE );
		memmove( F101_1_1E, (*m_pSnFhyo1_01Data)->Sn_F101_1_1E, MONY_BUF_SIZE );
		memmove( F101_1_1F, (*m_pSnFhyo1_01Data)->Sn_F101_1_1F, MONY_BUF_SIZE );
//	}
//	else{
//		memset( F101_1_1X, '\0', MONY_BUF_SIZE );
//		memset( F101_1_1D, '\0', MONY_BUF_SIZE );
//		memset( F101_1_1E, '\0', MONY_BUF_SIZE );
//		memset( F101_1_1F, '\0', MONY_BUF_SIZE );
//	}

	// 特定課税仕入れに係る支払対価の額
	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		memmove( F101_1_2X, (*m_pSnFhyo1_01Data)->Sn_F101_1_2X, MONY_BUF_SIZE );
		memmove( F101_1_2E, (*m_pSnFhyo1_01Data)->Sn_F101_1_2E, MONY_BUF_SIZE );
		memmove( F101_1_2F, (*m_pSnFhyo1_01Data)->Sn_F101_1_2F, MONY_BUF_SIZE );
	}
	else{
		memset( F101_1_2X, '\0', MONY_BUF_SIZE );
		memset( F101_1_2E, '\0', MONY_BUF_SIZE );
		memset( F101_1_2F, '\0', MONY_BUF_SIZE );
	}

	// 消費税額
	memmove( F101_2X, (*m_pSnFhyo1_01Data)->Sn_F101_2X, MONY_BUF_SIZE );
	memmove( F101_2D, (*m_pSnFhyo1_01Data)->Sn_F101_2D, MONY_BUF_SIZE );
	memmove( F101_2E, (*m_pSnFhyo1_01Data)->Sn_F101_2E, MONY_BUF_SIZE );
	memmove( F101_2F, (*m_pSnFhyo1_01Data)->Sn_F101_2F, MONY_BUF_SIZE );
	F101_2c = ( *m_pSnFhyo1_01Data )->Sn_F101_2c;


	// 控除過大調整税額
	memmove( F101_3X, (*m_pSnFhyo1_01Data)->Sn_F101_3X, MONY_BUF_SIZE );
	memmove( F101_3D, (*m_pSnFhyo1_01Data)->Sn_F101_3D, MONY_BUF_SIZE );
	memmove( F101_3E, (*m_pSnFhyo1_01Data)->Sn_F101_3E, MONY_BUF_SIZE );
	memmove( F101_3F, (*m_pSnFhyo1_01Data)->Sn_F101_3F, MONY_BUF_SIZE );
	

	// 控除対象仕入税額
	memmove( F101_4X, (*m_pSnFhyo1_01Data)->Sn_F101_4X, MONY_BUF_SIZE );
	memmove( F101_4D, (*m_pSnFhyo1_01Data)->Sn_F101_4D, MONY_BUF_SIZE );
	memmove( F101_4E, (*m_pSnFhyo1_01Data)->Sn_F101_4E, MONY_BUF_SIZE );
	memmove( F101_4F, (*m_pSnFhyo1_01Data)->Sn_F101_4F, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memmove( F101_5X, (*m_pSnFhyo1_01Data)->Sn_F101_5X, MONY_BUF_SIZE );
	memmove( F101_5D, (*m_pSnFhyo1_01Data)->Sn_F101_5D, MONY_BUF_SIZE );
	memmove( F101_5E, (*m_pSnFhyo1_01Data)->Sn_F101_5E, MONY_BUF_SIZE );
	memmove( F101_5F, (*m_pSnFhyo1_01Data)->Sn_F101_5F, MONY_BUF_SIZE );

	

	// 売上げの返還等対価に係る税額
//	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		memmove( F101_5_1X, (*m_pSnFhyo1_01Data)->Sn_F101_5_1X, MONY_BUF_SIZE );
		memmove( F101_5_1D, (*m_pSnFhyo1_01Data)->Sn_F101_5_1D, MONY_BUF_SIZE );
		memmove( F101_5_1E, (*m_pSnFhyo1_01Data)->Sn_F101_5_1E, MONY_BUF_SIZE );
		memmove( F101_5_1F, (*m_pSnFhyo1_01Data)->Sn_F101_5_1F, MONY_BUF_SIZE );
//	}
//	else{
//		memset( F101_5_1X, '\0', MONY_BUF_SIZE );
//		memset( F101_5_1D, '\0', MONY_BUF_SIZE );
//		memset( F101_5_1E, '\0', MONY_BUF_SIZE );
//		memset( F101_5_1F, '\0', MONY_BUF_SIZE );
//	}

	// 特定課税仕入れの返還等対価に係る税額
	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		memmove( F101_5_2X, (*m_pSnFhyo1_01Data)->Sn_F101_5_2X, MONY_BUF_SIZE );
		memmove( F101_5_2E, (*m_pSnFhyo1_01Data)->Sn_F101_5_2E, MONY_BUF_SIZE );
		memmove( F101_5_2F, (*m_pSnFhyo1_01Data)->Sn_F101_5_2F, MONY_BUF_SIZE );
	}
	else{
		memset( F101_5_2X, '\0', MONY_BUF_SIZE );
		memset( F101_5_2E, '\0', MONY_BUF_SIZE );
		memset( F101_5_2F, '\0', MONY_BUF_SIZE );
	}

	// 貸倒に係る税額
	memmove( F101_6X, (*m_pSnFhyo1_01Data)->Sn_F101_6X, MONY_BUF_SIZE );
	memmove( F101_6D, (*m_pSnFhyo1_01Data)->Sn_F101_6D, MONY_BUF_SIZE );
	memmove( F101_6E, (*m_pSnFhyo1_01Data)->Sn_F101_6E, MONY_BUF_SIZE );
	memmove( F101_6F, (*m_pSnFhyo1_01Data)->Sn_F101_6F, MONY_BUF_SIZE );

	// 控除税額小計
	memmove( F101_7X, (*m_pSnFhyo1_01Data)->Sn_F101_7X, MONY_BUF_SIZE );
	memmove( F101_7D, (*m_pSnFhyo1_01Data)->Sn_F101_7D, MONY_BUF_SIZE );
	memmove( F101_7E, (*m_pSnFhyo1_01Data)->Sn_F101_7E, MONY_BUF_SIZE );
	memmove( F101_7F, (*m_pSnFhyo1_01Data)->Sn_F101_7F, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove( F101_8X, (*m_pSnFhyo1_01Data)->Sn_F101_8X, MONY_BUF_SIZE );
	memmove( F101_8D, (*m_pSnFhyo1_01Data)->Sn_F101_8D, MONY_BUF_SIZE );
	memmove( F101_8E, (*m_pSnFhyo1_01Data)->Sn_F101_8E, MONY_BUF_SIZE );
	memmove( F101_8F, (*m_pSnFhyo1_01Data)->Sn_F101_8F, MONY_BUF_SIZE );

	// 差引税額
	memmove( F101_9X, (*m_pSnFhyo1_01Data)->Sn_F101_9X, MONY_BUF_SIZE );
	memmove( F101_9D, (*m_pSnFhyo1_01Data)->Sn_F101_9D, MONY_BUF_SIZE );
	memmove( F101_9E, (*m_pSnFhyo1_01Data)->Sn_F101_9E, MONY_BUF_SIZE );
	memmove( F101_9F, (*m_pSnFhyo1_01Data)->Sn_F101_9F, MONY_BUF_SIZE );

	// 合計差引税額
	memmove( F101_10F, (*m_pSnFhyo1_01Data)->Sn_F101_10F, MONY_BUF_SIZE );

	// 準となる消費税額　控除不足還付税額
	memmove( F101_11X, (*m_pSnFhyo1_01Data)->Sn_F101_11X, MONY_BUF_SIZE );
	memmove( F101_11E, (*m_pSnFhyo1_01Data)->Sn_F101_11E, MONY_BUF_SIZE );
	memmove( F101_11F, (*m_pSnFhyo1_01Data)->Sn_F101_11F, MONY_BUF_SIZE );

	// 準となる消費税額　差引税額
	memmove( F101_12X, (*m_pSnFhyo1_01Data)->Sn_F101_12X, MONY_BUF_SIZE );
	memmove( F101_12E, (*m_pSnFhyo1_01Data)->Sn_F101_12E, MONY_BUF_SIZE );
	memmove( F101_12F, (*m_pSnFhyo1_01Data)->Sn_F101_12F, MONY_BUF_SIZE );

	// 差引地方消費税の課税標準となる消費税
	memmove( F101_13X, (*m_pSnFhyo1_01Data)->Sn_F101_13X, MONY_BUF_SIZE );
	memmove( F101_13E, (*m_pSnFhyo1_01Data)->Sn_F101_13E, MONY_BUF_SIZE );
	memmove( F101_13F, (*m_pSnFhyo1_01Data)->Sn_F101_13F, MONY_BUF_SIZE );

	// 譲渡割額　還付額
	memmove( F101_14X, (*m_pSnFhyo1_01Data)->Sn_F101_14X, MONY_BUF_SIZE );
	memmove( F101_14E, (*m_pSnFhyo1_01Data)->Sn_F101_14E, MONY_BUF_SIZE );
	memmove( F101_14F, (*m_pSnFhyo1_01Data)->Sn_F101_14F, MONY_BUF_SIZE );

	// 譲渡割額　納税額
	memmove( F101_15X, (*m_pSnFhyo1_01Data)->Sn_F101_15X, MONY_BUF_SIZE );
	memmove( F101_15E, (*m_pSnFhyo1_01Data)->Sn_F101_15E, MONY_BUF_SIZE );
	memmove( F101_15F, (*m_pSnFhyo1_01Data)->Sn_F101_15F, MONY_BUF_SIZE );

	// 合計差引譲渡割額
	memmove( F101_16F, (*m_pSnFhyo1_01Data)->Sn_F101_16F, MONY_BUF_SIZE );

	return 0;
}

//-----------------------------------------------------------------------------
// 付表1-01 読み込みデータを集計モジュールにセット
//-----------------------------------------------------------------------------
void CShinFhyo1_01::ReadData2() {

	if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) { // 非連動の場合

		//------------>	
		/*
		char ksute1000[MONY_BUF_SIZE] = { 0 };
		m_pArith->l_input(ksute1000, _T("1000"));

		//端数処理仮作成。最終は集計モジュールや集計テーブル計算処理にもっていく
		m_pArith->l_div(F101_1D, F101_1D, ksute1000);
		m_pArith->l_mul(F101_1D, F101_1D, ksute1000);

		m_pArith->l_div(F101_1E, F101_1E, ksute1000);
		m_pArith->l_mul(F101_1E, F101_1E, ksute1000);
		*/
		//<------------

		//---->yoshida190613
		(*m_pSyzSyukei)->ResetShinkokuData(_T("312102"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("312103"));
		//------------------
//		(*m_pSyzSyukei)->SetShinkokuData(_T("312102"), F101_1D);
//		(*m_pSyzSyukei)->SetShinkokuData(_T("312103"), F101_1E);
		//<-----------------

		// 特例計算適用時には特例帳票でセットされた金額を取得するため、ここで金額をセットしない
		if ((((*m_pSnHeadData)->m_DispTabSgn & 0x80) == 0x00) || (((*m_pSnHeadData)->m_DispTabSgn & 0x30) == 0x00)) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("312202"), F101_1_1D);
			(*m_pSyzSyukei)->SetShinkokuData(_T("312203"), F101_1_1E);
			//(*m_pSyzSyukei)->SetShinkokuData(_T("312303"), F101_1_2E);
		}
		//else {
			// 特定課税仕入れのチェックがOFFの場合、金額をリセットする
			//if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() == FALSE ) {
			//	(*m_pSyzSyukei)->ResetShinkokuData(_T("312303"));
			//}
			//else {
			//	( *m_pSyzSyukei )->SetShinkokuData ( _T( "312303" ), F101_1_2E );
			//}
		//}
		//----> 20231025
		else{
			if( ((CH31HyoView*)m_pParent)->IsCrossOverR050930() && IsUseTokurei() ){
				(*m_pSyzSyukei)->SetShinkokuData(_T("312202"), F101_1_1D);
				(*m_pSyzSyukei)->SetShinkokuData(_T("312203"), F101_1_1E);
			}
		}
		//<---- 20231025

		// 特定課税仕入れのチェックがOFFの場合、金額をリセットする
		if ( ( *m_pSnHeadData )->IsNeedTkkzDisp() == FALSE ) {
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312303" ) );
		}
		else {
			( *m_pSyzSyukei )->SetShinkokuData ( _T( "312303" ), F101_1_2E );
		}

		if (F101_2c & 0x80) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("312402"), F101_2D);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("312402"));
		}
		if (F101_2c & 0x800) {
			(*m_pSyzSyukei)->SetShinkokuData(_T("312403"), F101_2E);
		}
		else {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("312403"));
		}

		//---->yoshida190613
		(*m_pSyzSyukei)->ResetShinkokuData(_T("312702"));
		(*m_pSyzSyukei)->ResetShinkokuData(_T("312703"));
		//------------------
//		(*m_pSyzSyukei)->SetShinkokuData(_T("312702"), F101_5D);
//		(*m_pSyzSyukei)->SetShinkokuData(_T("312703"), F101_5E);
		//<------------------
		(*m_pSyzSyukei)->SetShinkokuData(_T("312802"), F101_5_1D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312803"), F101_5_1E);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312903"), F101_5_2E);
		(*m_pSyzSyukei)->SetShinkokuData(_T("313002"), F101_6D);
		(*m_pSyzSyukei)->SetShinkokuData(_T("313003"), F101_6E);


		if ((*m_pSnHeadData)->SVmzsw == 1) {
			(*m_pSyzSyukei)->ResetShinkokuData(_T("312101"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("312201"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("312301"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("312401"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("312501"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("312601"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("312701"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("312801"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("312901"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("313001"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("313101"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("313201"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("313301"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("313401"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("313501"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("313601"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("313701"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("313801"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("313901"));
			(*m_pSyzSyukei)->ResetShinkokuData(_T("314001"));
		}
		else {
			// 経過措置なし　旧税率合計を０クリア
			char mony[MONY_BUF_SIZE] = { 0 };
			memset(mony, '\0', MONY_BUF_SIZE);

			(*m_pSyzSyukei)->SetShinkokuData(_T("312101"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("312201"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("312301"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("312401"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("312501"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("312601"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("312701"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("312801"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("312901"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("313001"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("313101"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("313201"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("313301"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("313401"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("313501"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("313601"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("313701"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("313801"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("313901"), mony);
			(*m_pSyzSyukei)->SetShinkokuData(_T("314001"), mony);
		}
	}
	else {
		// 特例計算適用時
		//if ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x80 ) && ( ( ( *m_pSnHeadData )->m_DispTabSgn & 0x10 ) || ( ( *m_pSnHeadData )->m_DispTabSgn & 0x20 ) ) ) {
		if( IsUseTokurei() ){
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312102" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312103" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312402" ) );
			( *m_pSyzSyukei )->ResetShinkokuData ( _T( "312403" ) );
		}
	}

}

//-----------------------------------------------------------------------------
// データの書込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo1_01::WriteData()
{
	// 課税標準額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_1X, F101_1X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_1D, F101_1D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_1E, F101_1E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_1F, F101_1F, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
	memmove((*m_pSnFhyo1_01Data)->Sn_F101_1_1X, F101_1_1X, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_01Data)->Sn_F101_1_1D, F101_1_1D, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_01Data)->Sn_F101_1_1E, F101_1_1E, MONY_BUF_SIZE);
	memmove((*m_pSnFhyo1_01Data)->Sn_F101_1_1F, F101_1_1F, MONY_BUF_SIZE);

	// 特定課税仕入れに係る支払対価の額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_1_2X, F101_1_2X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_1_2E, F101_1_2E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_1_2F, F101_1_2F, MONY_BUF_SIZE );

	// 消費税額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_2X, F101_2X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_2D, F101_2D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_2E, F101_2E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_2F, F101_2F, MONY_BUF_SIZE );
	(*m_pSnFhyo1_01Data)->Sn_F101_2c = F101_2c;

	// 控除過大調整税額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_3X, F101_3X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_3D, F101_3D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_3E, F101_3E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_3F, F101_3F, MONY_BUF_SIZE );

	// 控除対象仕入税額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_4X, F101_4X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_4D, F101_4D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_4E, F101_4E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_4F, F101_4F, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_5X, F101_5X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_5D, F101_5D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_5E, F101_5E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_5F, F101_5F, MONY_BUF_SIZE );


	// 売上げの返還等対価に係る税額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_5_1X, F101_5_1X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_5_1D, F101_5_1D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_5_1E, F101_5_1E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_5_1F, F101_5_1F, MONY_BUF_SIZE );
	
	// 特定課税仕入れの返還等対価に係る税額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_5_2X, F101_5_2X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_5_2E, F101_5_2E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_5_2F, F101_5_2F, MONY_BUF_SIZE );
	

	// 貸倒に係る税額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_6X, F101_6X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_6D, F101_6D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_6E, F101_6E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_6F, F101_6F, MONY_BUF_SIZE );

	// 控除税額小計
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_7X, F101_7X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_7D, F101_7D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_7E, F101_7E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_7F, F101_7F, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_8X, F101_8X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_8D, F101_8D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_8E, F101_8E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_8F, F101_8F, MONY_BUF_SIZE );

	// 差引税額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_9X, F101_9X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_9D, F101_9D, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_9E, F101_9E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_9F, F101_9F, MONY_BUF_SIZE );

	// 合計差引税額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_10F, F101_10F, MONY_BUF_SIZE );

	// 準となる消費税額　控除不足還付税額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_11X, F101_11X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_11E, F101_11E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_11F, F101_11F, MONY_BUF_SIZE );

	// 準となる消費税額　差引税額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_12X, F101_12X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_12E, F101_12E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_12F, F101_12F, MONY_BUF_SIZE );

	// 差引地方消費税の課税標準となる消費税
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_13X, F101_13X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_13E, F101_13E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_13F, F101_13F, MONY_BUF_SIZE );

	// 譲渡割額　還付額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_14X, F101_14X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_14E, F101_14E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_14F, F101_14F, MONY_BUF_SIZE );

	// 譲渡割額　納税額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_15X, F101_15X, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_15E, F101_15E, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_15F, F101_15F, MONY_BUF_SIZE );

	// 合計差引譲渡割額
	memmove( (*m_pSnFhyo1_01Data)->Sn_F101_16F, F101_16F, MONY_BUF_SIZE );


	return 0;
}



//-----------------------------------------------------------------------------
// 付表１-０１ 画面出力
//-----------------------------------------------------------------------------
void CShinFhyo1_01::DispFh1_01()
{

	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 画面の初期化
	for( int i=3; i<=5; i++ ){
		dsp_cls( IDC_ICSDIAG8CTRL2, (short )i );
	}
	for( int i=ID101_1X; i<=ID101_16F; i++ ){
		dsp_cls( IDC_ICSDIAG8CTRL2, (short )i );
	}


	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){//連動の場合手入力サイン初期化
		F101_1c = 0;//手入力サイン初期化
		F101_2c = 0;//手入力サイン初期化
		F101_5c = 0;//手入力サイン初期化
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
	diag_setdata( IDC_ICSDIAG8CTRL2, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	if ( ( ( ( CH31HyoView* )m_pParent )->GetShRevType() >= ID_ICSSH_KYOJU_REVTYPR ) && ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) ) {
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-1の25・26Ｄ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 46, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-1の25・26Ｅ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 47, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-1の24Ｄ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 53, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-1の24Ｅ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 54, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}
	else {
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-1の24・25Ｄ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 46, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-1の24・25Ｅ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 47, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-1の23Ｄ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 53, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-1の23Ｅ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 54, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}

	//---->20230120
	long smd = 0, emd = 0;
	( ( CH31HyoView* )m_pParent )->GetKazeiKikan( smd, emd );
	if( emd >= ICS_SH_INVOICE_DAY ){
		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-1の27・28Ｄ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 46, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-1の27・28Ｅ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 47, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-1の26Ｄ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 53, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		memset ( buf, '\0', sizeof ( buf ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "（付表2-1の26Ｅ欄の合計金額）" ) );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL2, 54, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}
	//<----20230120

	// 各入力のデータ全て表示
	f101data_all(0);

}


//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//		Mony	：	表示金額
//-----------------------------------------------------------------------------
void CShinFhyo1_01::dsp_prs( unsigned short Id, short Index, char *Mony )
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
void CShinFhyo1_01::dsp_cls( unsigned short Id, short Index )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch( Index ) {
		case 4 :	// 課税期間　自
		case 5 :	// 課税期間　至
		case 3 :	// 氏名又は､名称
			diag_clear( Id, Index, TRUE, CIcsdiagctrl );
			break;
		default :	// 金額
			memset( DiagData.data_val, '\0', MONY_BUF_SIZE );
			diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			break;
	}
}


//-----------------------------------------------------------------------------
// 各入力データ抽出
//-----------------------------------------------------------------------------
void CShinFhyo1_01::GetAllDiagData()
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	//  1. 課税標準額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_1X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_1X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_1D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_1D, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_1E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_1E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_1F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_1F, DiagData.data_val, MONY_BUF_SIZE ); 

	//	1_1. 課税資産の譲渡等の対価の額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_1_1X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_1_1X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_1_1D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_1_1D, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_1_1E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_1_1E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_1_1F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_1_1F, DiagData.data_val, MONY_BUF_SIZE ); 

	//  1_2. 特定課税仕入れに係る支払対価の額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_1_2X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_1_2X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_1_2E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_1_2E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_1_2F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_1_2F, DiagData.data_val, MONY_BUF_SIZE ); 

	//  2. 消費税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_2X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_2X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_2D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_2D, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_2E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_2E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_2F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_2F, DiagData.data_val, MONY_BUF_SIZE ); 

	//  3. 控除過大調整税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_3X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_3X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_3D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_3D, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_3E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_3E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_3F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_3F, DiagData.data_val, MONY_BUF_SIZE ); 

	//  4. 控除対象仕入税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_4X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_4X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_4D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_4D, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_4E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_4E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_4F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_4F, DiagData.data_val, MONY_BUF_SIZE ); 

	//  5. 返還等対価に係る税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_5X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_5X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_5D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_5D, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_5E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_5E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_5F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_5F, DiagData.data_val, MONY_BUF_SIZE ); 

	//  5_1. 返還等対価に係る税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_5_1X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_5_1X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_5_1D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_5_1D, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_5_1E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_5_1E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_5_1F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_5_1F, DiagData.data_val, MONY_BUF_SIZE );

	//  5_2. 特定課税仕入れの返還等対価に係る税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_5_2X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_5_2X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_5_2E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_5_2E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_5_2F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_5_2F, DiagData.data_val, MONY_BUF_SIZE ); 

	//  6. 貸倒に係る税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_6X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_6X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_6D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_6D, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_6E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_6E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_6F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_6F, DiagData.data_val, MONY_BUF_SIZE );

	//  7. 控除税額小計
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_7X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_7X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_7D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_7D, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_7E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_7E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_7F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_7F, DiagData.data_val, MONY_BUF_SIZE );

	//  7. 控除税額小計
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_7X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_7X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_7D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_7D, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_7E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_7E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_7F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_7F, DiagData.data_val, MONY_BUF_SIZE );

	//  8. 控除不足還付税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_8X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_8X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_8D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_8D, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_8E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_8E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_8F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_8F, DiagData.data_val, MONY_BUF_SIZE );

	//  9. 差引税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_9X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_9X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_9D, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_9D, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_9E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_9E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_9F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_9F, DiagData.data_val, MONY_BUF_SIZE );

	//  10. 合計差引税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_10F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_10F, DiagData.data_val, MONY_BUF_SIZE );

	//  11. 準となる消費税額　控除不足還付税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_11X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_11X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_11E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_11E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_11F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_11F, DiagData.data_val, MONY_BUF_SIZE );

	//  12. 準となる消費税額　差引税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_12X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_12X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_12E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_12E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_12F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_12F, DiagData.data_val, MONY_BUF_SIZE );

	//  13. 差引地方消費税の課税標準となる消費税
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_13X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_13X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_13E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_13E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_13F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_13F, DiagData.data_val, MONY_BUF_SIZE );

	//  14. 譲渡割額　還付額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_14X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_14X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_14E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_14E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_14F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_14F, DiagData.data_val, MONY_BUF_SIZE );

	//  15. 譲渡割額　納税額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_15X, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_15X, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_15E, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_15E, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_15F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_15F, DiagData.data_val, MONY_BUF_SIZE );

	//  16. 合計差引譲渡割額
	diag_getdata( IDC_ICSDIAG8CTRL2, ID101_16F, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F101_16F, DiagData.data_val, MONY_BUF_SIZE );
}



//-----------------------------------------------------------------------------
// 課税標準額
//-----------------------------------------------------------------------------
// 引数		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_1data()
{
	if(!((*m_pSnHeadData)->Sn_Sign4&0x80) ){	//連動の場合集計しない。自動集計値を使用する為

	}else{

		/*---->yoshida190613
		//常に入力不可
		if( !(F101_1c&0x80) ){
			if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
				if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
					ChangeColor( IDC_ICSDIAG8CTRL2, ID101_1D, 1 );
				}
				else{
					ChangeColor( IDC_ICSDIAG8CTRL2, ID101_1D, 0 );
				}
			}
		}

		if( !(F101_1c&0x800) ){
			if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
				if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
					ChangeColor( IDC_ICSDIAG8CTRL2, ID101_1E, 1 );
				}
				else{
					ChangeColor( IDC_ICSDIAG8CTRL2, ID101_1E, 0 );
				}
			}
		}
		<---------*/
	}

	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_1X, F101_1X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_1D, F101_1D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_1E, F101_1E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_1F, F101_1F );

	//----> 20231025 del
	// 現在は青背景にする仕様ではないので閉じる。
	/*if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		if((F101_1c&0x80) ){
			ChangeColor( IDC_ICSDIAG8CTRL2, ID101_1D, 3 );	
		}
		if((F101_1c&0x800) ){
			ChangeColor( IDC_ICSDIAG8CTRL2, ID101_1E, 3 );	
		}
	}*/
	//<---- 20231025 del

	F101_1c &= ~0x10;
	F101_1c &= ~0x100;
}

//-----------------------------------------------------------------------------
// 課税資産の譲渡等の対価の額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_1_1data( )
{
	// 色変更
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		ChangeColor( IDC_ICSDIAG8CTRL2, ID101_1_1D, (F101_1c & 0x80) ? 3 : 0 );
		ChangeColor( IDC_ICSDIAG8CTRL2, ID101_1_1E, (F101_1c & 0x800) ? 3 : 0 );
	}
	
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_1_1X, F101_1_1X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_1_1D, F101_1_1D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_1_1E, F101_1_1E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_1_1F, F101_1_1F );

}

//-----------------------------------------------------------------------------
// 特定課税仕入れに係る支払対価の額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_1_2data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_1_2X, F101_1_2X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_1_2E, F101_1_2E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_1_2F, F101_1_2F );
}

//-----------------------------------------------------------------------------
// 消費税額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_2data( )
{
	/*
	if ( !( F101_2c & 0x80 ) ) {
		if ( ( F101_2c & 0x10 ) || ( m_pArith->l_test ( F101_2D ) == 0 ) ) {
			memset ( F101_2D, '\0', MONY_BUF_SIZE );
		}
	}

	if ( !( F101_2c & 0x800 ) ) {
		if ( ( F101_2c & 0x100 ) || ( m_pArith->l_test ( F101_2E ) == 0 ) ) {
			memset ( F101_2E, '\0', MONY_BUF_SIZE );
		}
	}
	*/

	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_2X, F101_2X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_2D, F101_2D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_2E, F101_2E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_2F, F101_2F );

	if ( ((*m_pSnHeadData)->Sn_Sign4 & 0x80) && (((*m_pSnHeadData)->Sn_Sign4 & 0x01) == 0x00) ) {
		ChangeColor( IDC_ICSDIAG8CTRL2, ID101_2D, (F101_2c & 0x80) ? 3 : 0 );
		ChangeColor( IDC_ICSDIAG8CTRL2, ID101_2E, (F101_2c & 0x800) ? 3 : 0 );
	}


	F101_2c &= ~0x10;
	F101_2c &= ~0x100;
}

//-----------------------------------------------------------------------------
// 控除過大調整税額	
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_3data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_3X, F101_3X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_3D, F101_3D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_3E, F101_3E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_3F, F101_3F );
	
}


//-----------------------------------------------------------------------------
// 控除対象仕入税額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_4data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_4X, F101_4X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_4D, F101_4D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_4E, F101_4E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_4F, F101_4F );
	
}

//-----------------------------------------------------------------------------
// 返還等対価に係る税額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_5data( )
{

	if(!((*m_pSnHeadData)->Sn_Sign4&0x80) ){	//連動の場合集計しない。自動集計値を使用する為

	}
	else{

		/*---->yoshida190622
		//常に入力不可
		if( !(F101_5c&0x80) ){
			if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
				if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
					ChangeColor( IDC_ICSDIAG8CTRL2, ID101_5D, 1 );
				}
				else{
					ChangeColor( IDC_ICSDIAG8CTRL2, ID101_5D, 0 );
				}
			}
		}

		if( !(F101_5c&0x800) ){
			if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
				if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
					ChangeColor( IDC_ICSDIAG8CTRL2, ID101_5E, 1 );
				}
				else{
					ChangeColor( IDC_ICSDIAG8CTRL2, ID101_5E, 0 );
				}
			}
		}
		*/
		//<----------------------
	}

	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_5X, F101_5X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_5D, F101_5D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_5E, F101_5E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_5F, F101_5F );

	/*
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		if((F101_5c&0x80) ){
			ChangeColor( IDC_ICSDIAG8CTRL2, ID101_5D, 3 );	// 2
		}
		if((F101_5c&0x800) ){
			ChangeColor( IDC_ICSDIAG8CTRL2, ID101_5E, 3 );	// 2
		}
	}*/

	F101_5c &= ~0x10;
	F101_5c &= ~0x100;
}

//-----------------------------------------------------------------------------
// 売上げの返還等対価に係る税額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_5_1data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_5_1X, F101_5_1X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_5_1D, F101_5_1D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_5_1E, F101_5_1E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_5_1F, F101_5_1F );
}

//-----------------------------------------------------------------------------
// 特定課税仕入れの返還等対価に係る税額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_5_2data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_5_2X, F101_5_2X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_5_2E, F101_5_2E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_5_2F, F101_5_2F );
}


//-----------------------------------------------------------------------------
// 貸倒れに係る税額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_6data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_6X, F101_6X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_6D, F101_6D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_6E, F101_6E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_6F, F101_6F );
}


//-----------------------------------------------------------------------------
// 控除税額小計
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_7data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_7X, F101_7X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_7D, F101_7D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_7E, F101_7E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_7F, F101_7F );
}

//-----------------------------------------------------------------------------
// 控除不足還付税額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_8data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_8X, F101_8X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_8D, F101_8D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_8E, F101_8E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_8F, F101_8F );
}

//-----------------------------------------------------------------------------
// 差引税額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_9data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_9X, F101_9X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_9D, F101_9D );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_9E, F101_9E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_9F, F101_9F );
}

//-----------------------------------------------------------------------------
// 合計差引税額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_10data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_10F, F101_10F );
}

//-----------------------------------------------------------------------------
// 準となる消費税額　控除不足還付税額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_11data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_11X, F101_11X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_11E, F101_11E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_11F, F101_11F );
}

//-----------------------------------------------------------------------------
// 準となる消費税額　差引税額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_12data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_12X, F101_12X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_12E, F101_12E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_12F, F101_12F );
}

//-----------------------------------------------------------------------------
// 合計差引地方消費税の課税標準となる消費税額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_13data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_13X, F101_13X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_13E, F101_13E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_13F, F101_13F );
}


//-----------------------------------------------------------------------------
// 譲渡割額　還付額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_14data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_14X, F101_14X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_14E, F101_14E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_14F, F101_14F );
}

//-----------------------------------------------------------------------------
// 譲渡割額　納税額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_15data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_15X, F101_15X );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_15E, F101_15E );
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_15F, F101_15F );
}

//-----------------------------------------------------------------------------
// 合計差引譲渡割額
//-----------------------------------------------------------------------------
//		：
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101_16data( )
{
	dsp_prs( IDC_ICSDIAG8CTRL2, ID101_16F, F101_16F );
}

//-----------------------------------------------------------------------------
// 各入力のデータ表示
//-----------------------------------------------------------------------------
// 引数	atno	：	0：全て
//		sw		：	
//-----------------------------------------------------------------------------
void CShinFhyo1_01::f101data_all( int atno )
{
	while( 1 ){
		switch( atno ){
			case	0 :
			case	1 :
				f101_1data();
				f101_1_1data();
				f101_1_2data();

			case	2 :
				f101_2data();

			case	3 :
				f101_3data();

			case	4 :
				f101_4data();
				
			case	5 :
				f101_5data();
				f101_5_1data();
				f101_5_2data();

			case	6 :	
				f101_6data();

			case	7 :
				f101_7data();

			case	8 :
				f101_8data();

			case	9 :
				f101_9data();

			case	10 :
				f101_10data();

			case	11 :
				f101_11data();
				
			case	12 :
				f101_12data();

			case	13 :
				f101_13data();

			case	14 :
				f101_14data();

			case	15 :
				f101_15data();

			case	16 :
				f101_16data();


			default	:
				break;
		}
		break;
	}
}


//-----------------------------------------------------------------------------
// 特定課税仕入に関する項目の金額クリア
//-----------------------------------------------------------------------------
void CShinFhyo1_01::ClearTkkzMony()
{
	m_pArith->l_clear( F101_1_2X );
	m_pArith->l_clear( F101_1_2E );
	m_pArith->l_clear( F101_1_2F );
//	memmove( F101_1X, F101_1_1X, MONY_BUF_SIZE );
//	memmove( F101_1D, F101_1_1D, MONY_BUF_SIZE );
//	memmove( F101_1E, F101_1_1E, MONY_BUF_SIZE );
//	memmove( F101_1F, F101_1_1F, MONY_BUF_SIZE );

	m_pArith->l_clear( F101_5_2X );
	m_pArith->l_clear( F101_5_2E );
	m_pArith->l_clear( F101_5_2F );
//	memmove( F101_5X, F101_5_1X, MONY_BUF_SIZE );
//	memmove( F101_5D, F101_5_1D, MONY_BUF_SIZE );
//	memmove( F101_5E, F101_5_1E, MONY_BUF_SIZE );
//	memmove( F101_5F, F101_5_1F, MONY_BUF_SIZE );

	DispFh1_01();

	WriteData();

}


int CShinFhyo1_01::SyukeiDataSet(int cID) {

	DIAGRAM_DATA	DiagData;
	char			val[MONY_BUF_SIZE];

	diag_getdata(IDC_ICSDIAG8CTRL2, cID, (struct IUnknown *)&DiagData, CIcsdiagctrl);
	memmove(val, DiagData.data_val, MONY_BUF_SIZE);

	switch (cID){
	case ID101_1X:
		memmove(F101_1X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312101"), F101_1X);
		break;
	case ID101_1D:
		memmove(F101_1D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312102"), F101_1D);
		break;
	case ID101_1E:
		memmove(F101_1E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312103"), F101_1E);
		break;
	case ID101_1_1X:
		memmove(F101_1_1X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312201"), F101_1_1X);
		break;
	case ID101_1_1D:
		memmove(F101_1_1D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312202"), F101_1_1D);
		break;
	case ID101_1_1E:
		memmove(F101_1_1E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312203"), F101_1_1E);
		break;
	case ID101_1_2X:
		memmove(F101_1_2X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312301"), F101_1_2X);
		break;
	case ID101_1_2E:
		memmove(F101_1_2E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312303"), F101_1_2E);
		break;
	case ID101_2X:
		memmove(F101_2X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312401"), F101_2X);
		break;
	case ID101_2D:
		if ( m_pArith->l_cmp( F101_2D, val ) ) {
			F101_2c |= 0x80;
			memmove(F101_2D, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("312402"), F101_2D);
		}
		break;
	case ID101_2E:
		if ( m_pArith->l_cmp( F101_2E, val ) ) {
			F101_2c |= 0x800;
			memmove(F101_2E, val, MONY_BUF_SIZE);
			(*m_pSyzSyukei)->SetShinkokuData(_T("312403"), F101_2E);
		}
		break;
	case ID101_5X:
		memmove(F101_5X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312701"), F101_5X);
		break;
	case ID101_5D:
		memmove(F101_5D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312702"), F101_5D);
		break;
	case ID101_5E:
		memmove(F101_5E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312703"), F101_5E);
		break;
	case ID101_5_1X:
		memmove(F101_5_1X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312801"), F101_5_1X);
		break;
	case ID101_5_1D:
		memmove(F101_5_1D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312802"), F101_5_1D);
		break;
	case ID101_5_1E:
		memmove(F101_5_1E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312803"), F101_5_1E);
		break;
	case ID101_5_2X:
		memmove(F101_5_2X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312901"), F101_5_2X);
		break;
	case ID101_5_2E:
		memmove(F101_5_2E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("312903"), F101_5_2E);
		break;
	case ID101_6X:
		memmove(F101_6X, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("313001"), F101_6X);
		break;
	case ID101_6D:
		memmove(F101_6D, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("313002"), F101_6D);
		break;
	case ID101_6E:
		memmove(F101_6E, val, MONY_BUF_SIZE);
		(*m_pSyzSyukei)->SetShinkokuData(_T("313003"), F101_6E);
		break;
	default:
		break;
	}


	return MemoryReget();

}

int CShinFhyo1_01::MemoryReget() {

	// 入力ロック時は集計しない
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return 0;
	}

	// 計算項目を全て再取得
	MoneyBasejagArray	money;

	money = (*m_pSyzSyukei)->GetShinkokuData(_T("31"));
	int cnt = (int)money.GetCount();
	int	incnt = 0;
	for (int l = 0; l < cnt; l++) {
		incnt = (int)money[l].GetCount();
		for (int m = 0; m < incnt; m++) {
			// 課税標準額
			if (money[l][m].code == _T("312101"))
				memmove(F101_1X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312100"))
				memmove(F101_1F, money[l][m].arith, MONY_BUF_SIZE);

			else if (money[l][m].code == _T("312102"))
				memmove(F101_1D, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312103"))
				memmove(F101_1E, money[l][m].arith, MONY_BUF_SIZE);

			// 課税資産の譲渡等の対価の額
			else if (money[l][m].code == _T("312201"))
				memmove(F101_1_1X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312202"))
				memmove(F101_1_1D, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312203"))
				memmove(F101_1_1E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312200"))
				memmove(F101_1_1F, money[l][m].arith, MONY_BUF_SIZE);

			// 特定仕入に係る支払対価の額
			else if (money[l][m].code == _T("312301"))
				memmove(F101_1_2X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312303"))
				memmove(F101_1_2E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312300"))
				memmove(F101_1_2F, money[l][m].arith, MONY_BUF_SIZE);

			// 消費税額
			else if (money[l][m].code == _T("312400"))
				memmove(F101_2F, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312401"))
				memmove(F101_2X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312402"))
				memmove(F101_2D, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312403"))
				memmove(F101_2E, money[l][m].arith, MONY_BUF_SIZE);

			// 控除過大調整税額
			else if (money[l][m].code == _T("312501"))
				memmove(F101_3X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312502"))
				memmove(F101_3D, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312503"))
				memmove(F101_3E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312500"))
				memmove(F101_3F, money[l][m].arith, MONY_BUF_SIZE);

			// 控除対象仕入税額
			else if (money[l][m].code == _T("312601"))
				memmove(F101_4X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312602"))
				memmove(F101_4D, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312603"))
				memmove(F101_4E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312600"))
				memmove(F101_4F, money[l][m].arith, MONY_BUF_SIZE);

			// 返還等対価に係る税額
			else if (money[l][m].code == _T("312701"))
				memmove(F101_5X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312702"))
				memmove(F101_5D, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312703"))
				memmove(F101_5E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312700"))
				memmove(F101_5F, money[l][m].arith, MONY_BUF_SIZE);

			// 売上の返還等に係る税額
			else if (money[l][m].code == _T("312801"))
				memmove(F101_5_1X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312802"))
				memmove(F101_5_1D, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312803"))
				memmove(F101_5_1E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312800"))
				memmove(F101_5_1F, money[l][m].arith, MONY_BUF_SIZE);

			// 特定課税仕入の返還等対価に係る税額
			else if (money[l][m].code == _T("312901"))
				memmove(F101_5_2X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312903"))
				memmove(F101_5_2E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("312900"))
				memmove(F101_5_2F, money[l][m].arith, MONY_BUF_SIZE);

			// 貸倒に係る税額
			else if (money[l][m].code == _T("313001"))
				memmove(F101_6X, money[l][m].arith, MONY_BUF_SIZE);

			//---->yoshida190919
			else if (money[l][m].code == _T("313002"))
				memmove(F101_6D, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313003"))
				memmove(F101_6E, money[l][m].arith, MONY_BUF_SIZE);
			//<-------

			else if (money[l][m].code == _T("313000"))
				memmove(F101_6F, money[l][m].arith, MONY_BUF_SIZE);

			// 控除税額小計
			else if (money[l][m].code == _T("313101"))
				memmove(F101_7X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313102"))
				memmove(F101_7D, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313103"))
				memmove(F101_7E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313100"))
				memmove(F101_7F, money[l][m].arith, MONY_BUF_SIZE);

			// 控除不足還付税額
			else if (money[l][m].code == _T("313201"))
				memmove(F101_8X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313202"))
				memmove(F101_8D, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313203"))
				memmove(F101_8E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313200"))
				memmove(F101_8F, money[l][m].arith, MONY_BUF_SIZE);

			// 差引税額
			else if (money[l][m].code == _T("313301"))
				memmove(F101_9X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313302"))
				memmove(F101_9D, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313303"))
				memmove(F101_9E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313300"))
				memmove(F101_9F, money[l][m].arith, MONY_BUF_SIZE);

			// 合計差引税額
			else if (money[l][m].code == _T("313400"))
				memmove(F101_10F, money[l][m].arith, MONY_BUF_SIZE);

			// 控除不足還付税額
			else if (money[l][m].code == _T("313501"))
				memmove(F101_11X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313503"))
				memmove(F101_11E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313500"))
				memmove(F101_11F, money[l][m].arith, MONY_BUF_SIZE);

			// 差引税額
			else if (money[l][m].code == _T("313601"))
				memmove(F101_12X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313603"))
				memmove(F101_12E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313600"))
				memmove(F101_12F, money[l][m].arith, MONY_BUF_SIZE);

			// 合計差引地方消費税の課税標準となる消費税額
			else if (money[l][m].code == _T("313701"))
				memmove(F101_13X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313703"))
				memmove(F101_13E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313700"))
				memmove(F101_13F, money[l][m].arith, MONY_BUF_SIZE);

			// 還付額
			else if (money[l][m].code == _T("313801"))
				memmove(F101_14X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313803"))
				memmove(F101_14E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313800"))
				memmove(F101_14F, money[l][m].arith, MONY_BUF_SIZE);

			// 納税額
			else if (money[l][m].code == _T("313901"))
				memmove(F101_15X, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313903"))
				memmove(F101_15E, money[l][m].arith, MONY_BUF_SIZE);
			else if (money[l][m].code == _T("313900"))
				memmove(F101_15F, money[l][m].arith, MONY_BUF_SIZE);

			// 合計差引譲渡割額
			else if (money[l][m].code == _T("314000"))
				memmove(F101_16F, money[l][m].arith, MONY_BUF_SIZE);

		}
	}

	return 0;

}

//====================================
// 手入力項目あり？
//------------------------------------
// 返送値
//	TRUE		:		あり
//	FALSE		:		なし
//====================================
BOOL CShinFhyo1_01::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		if ( F101_2c & 0x880 ) {
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
bool CShinFhyo1_01::IsUseTokurei()
{
	bool rt = false;

	if( (*m_pSnHeadData)->m_DispTabSgn&0x80 && (*m_pSnHeadData)->m_DispTabSgn&0x30 ){
		// 特例5-1または5-2使用
		rt = true;
	}

	return rt;
}