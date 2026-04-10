// ShinKniinpKeika6syu.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinKniinp31.h"
#include "ShinKniinp31Idx.h"

// CShinKniinp31 ダイアログ

IMPLEMENT_DYNAMIC(CShinKniinp31, CSyzBaseDlg)

CShinKniinp31::CShinKniinp31(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinKniinp31::IDD, pParent)
	, m_pParent(pParent)
	, m_Idx(0)
	, m_Lin(0)
	, m_Clm(0)
{
	m_curwnd = -1;
}

CShinKniinp31::~CShinKniinp31()
{
}

void CShinKniinp31::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Kniinp31);
}


BEGIN_MESSAGE_MAP(CShinKniinp31, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinKniinp31 メッセージ ハンドラ

BOOL CShinKniinp31::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Kniinp31;
	m_BottomIdx = 346;	//コントロールの一番下のIDX指定するとスクロー9ルがそこまでいく
//	m_BottomIdx = 337;	//コントロールの一番下のIDX指定するとスクロー9ルがそこまでいく


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinKniinp31::Init()
{
	InitDiagAttr();

	DispList(0);

	SetItemPosition();

	return 0;
}


//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
void CShinKniinp31::InitDiagAttr()
{
	int	cnt = 0;
	int	Flg;

	while( 1 ){
		if( ShinKniinp31[cnt].Index == 0 ){
			break;
		}
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01){
			Flg = ShinKniinp31[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				// 経過措置なしのときは6.3％入力不可
				if( ( !(*m_pSnHeadData)->SVmzsw ) && ( ShinKniinp31[cnt].UnConnect == 3 ) ){
					Flg = 0;
				}
				Flg = ShinKniinp31[cnt].UnConnect;
			}
			else{
				Flg = ShinKniinp31[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL1, ShinKniinp31[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, ShinKniinp31[cnt].Connect );

		// 経過措置なしの時は6.3％入力不可
		if( ( !(*m_pSnHeadData)->SVmzsw ) && ( ShinKniinp31[cnt].UnConnect == 3 ) ){
			m_Kniinp31.ModifyItem( ShinKniinp31[cnt].Index, DIAG_MDFY_READONLY );	// 入力不可
			ChangeColor( IDC_ICSDIAGCTRL1, ShinKniinp31[cnt].Index, 1 );			// 背景色変更
		}
		cnt++;
	}

	SetFocusRange();

	m_Kniinp31.EnableDelete(0);//DELETEキー１回で削除モード設定'14.08.01

}

BOOL CShinKniinp31::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
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
				if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
					TerminationIcsdiag8ctrl1( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------
// 画面表示
//-----------------------------------------------------------------------------
// 引数	mode	：	
//-----------------------------------------------------------------------------
void CShinKniinp31::DispList( int mode )
{
	m_Kniinp31.OffControl( TRUE );
	for( int i = 0 ; i < JSKLINCNT ; i++ ){
		DispKniinpKeikaDiagData(i);
	}
	m_Kniinp31.UpdateWindow();

	DIAGRAM_DATA	data;

	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, JSKZKBNID, TRUE, CIcsdiagctrl );
	data.data_disp = _T("経理方式：");
	if( (*m_pTaxListData)->Head.KHstax&0x01 ){
		data.data_disp += _T("税抜き");
	}
	else{
		data.data_disp += _T("税込み");
	}
	diag_setdata( IDC_ICSDIAGCTRL1, JSKZKBNID, &data, CIcsdiagctrl );

	if( !mode ){
		m_Idx = JSKVMIN;
	}
	//diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );

	SetRedrawScrollToIdx(m_Kniinp31.GetPosition(), IDC_ICSDIAGCTRL1);
}


//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
void CShinKniinp31::DispKniinpKeikaDiagData( long dlin )
{
	short	idx, i;
	long	lin;

	int idxpos = 0;

	//データ保存ポジションセット
	static long KniinpKeiDataPos[] = {0 , 1 , 9 , 2 , 3 , 4 , 8 , 5 , 6 , 7};

	lin = KniinpKeiDataPos[dlin];

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KUval[40 + lin];

	char	val[6];
	DIAGRAM_DATA	data;
	int		st;

	int WideCntSub = 0;//農業などの場合、この数値に少ない分をセットし、idxを合わせる
	if (dlin > 2) {//第２種農業
		WideCntSub = 8;
	}
	if (dlin > 4) {//第３種農業
		WideCntSub += 4;
	}

	idx = (short)( (dlin * JSKWIDECNT) - WideCntSub) + JSKVMIN;


	int loopcnt = JSKWIDECNT;//デフォルト

	if (dlin == 2) { //第二種　農業事業者
		loopcnt = 8;
	}
	else if (dlin == 4) { //第三種　農業事業者
		loopcnt = 12;
	}

	static long Dispjudgepos1[] = { 0 , 1 , 2 , 3 , 8 , 9 , 10 , 11 };
	static long Dispjudgepos2[] = { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 12 , 13 , 14 , 15 };
	static long Dispjudgeposdef[] = { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 };

	int clmpos = 0;

	for (i = 0; i < loopcnt; i++) {

		idxpos = idx + i;

		st = 0;
		::ZeroMemory(val, sizeof(val));
		m_Util.DiagOcxIniz(&data);
		diag_clear(IDC_ICSDIAGCTRL1, idxpos, TRUE, CIcsdiagctrl);

		if (dlin == 2) { //第二種　農業事業者
			clmpos = Dispjudgepos1[i];
		}
		else if (dlin == 4) { //第三種　農業事業者
			clmpos = Dispjudgepos2[i];
		}
		else {
			clmpos = Dispjudgeposdef[i];
		}

		st = DispJudge(clmpos, lin);

		switch (clmpos) {
		case 0:	// 決算額
			memcpy(val, pK->KKval1, 6);
			break;
		case 1:	// 課税取引外
			memcpy(val, pK->KKval2, 6);
			break;
		case 2:	// 課税取引額
			memcpy(val, pK->KKval3, 6);
			break;
		case 3:	// 返還等課税取引
			memcpy(val, pK->KKval4, 6);
			break;

		case 4:	// 決算　うち7.8％
		case 5:	// 課税取引外　うち7.8％
			continue;
		case 6:	// 課税取引金額　うち7.8％
			memcpy(val, pK->KKval5, 6);
			break;
		case 7:	// 返還等課税取引　うち7.8％
			memcpy(val, pK->KKval6, 6);
			break;

		case 8:	// 決算　うち6.24％
		case 9:	// 課税取引額　うち6.24％
			continue;
		case 10:	// 課税取引金額　うち6.24％
			memcpy(val, pK->KKval7, 6);
			break;
		case 11:	// 返還等課税取引　うち6.24％
			memcpy(val, pK->KKval8, 6);
			break;

		case 12:	// 決算　うち6.3％
		case 13:	// 課税取引額　うち6.3％
			continue;
		case 14:	// 課税取引金額　うち6.3％
			memcpy(val, pK->KKval9, 6);
			break;
		case 15:	// 返還等課税取引　うち6.3％
			memcpy(val, pK->KKval10, 6);
			break;
		}

		if (st) {
			memcpy(data.data_val, val, 6);
			diag_setdata(IDC_ICSDIAGCTRL1, idxpos, &data, CIcsdiagctrl);
		}
	}

}

//-----------------------------------------------------------------------------
// 各入力データ抽出
//-----------------------------------------------------------------------------
// 引数	index	：
//		leng	：
//		data	：
//		mode	：
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CShinKniinp31::GetDiagData( long index, short leng, DIAGRAM_DATA *data, int mode )
{

	if( index < 0 ){
		return 0;
	}

	char	dbuf[128];
	char	*buff;
	int		len;

	
	len = data->data_edit.GetLength();
	memset( dbuf, '\0', sizeof(dbuf) );
	buff = (char *)((LPCTSTR)(data->data_edit));
	memmove( dbuf, buff, len );


	int clm_pos = 0;
	clm_pos = GetIndexToClmPosition(index);

	//データ保存ポジションセット
	long	lin;

	lin = m_Lin;

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KUval[40 + lin];

	if( !mode ){
		switch( m_Clm ){
			case 0:		// 決算額
				if( leng ){
					memcpy( pK->KKval1, data->data_val, 6 );
				}
				break;
			case 1:		// 課税取引外
				if( leng ){
					memcpy( pK->KKval2, data->data_val, 6 );
				}
				break;
			case 2:		// 課税取引額
				if( leng ){
					memcpy( pK->KKval3, data->data_val, 6 );
				}
				break;
			case 3:		// 変換等課税取引
				if( leng ){
					memcpy( pK->KKval4, data->data_val, 6 );
				}
				break;

			case 4:		// 決算額 7.8%
			case 5:		// 課税取引外 7.8%
				break;
			case 6:		// 課税取引額  うち7.8％
				if( leng ){
					memcpy( pK->KKval5, data->data_val, 6 );
				}
				break;
			case 7:		// 返還等課税取引  うち7.8％
				if( leng ){
					memcpy( pK->KKval6, data->data_val, 6 );
				}
				break;

			case 8:		// 決算額 6.24
			case 9:		// 課税取引外 6.24
				break;
			case 10:		// 課税取引額  うち6.24％
				if( leng ){
					memcpy( pK->KKval7, data->data_val, 6 );
				}
				break;
			case 11:		// 変換等課税取引  うち6.24％
				if( leng ){
					memcpy( pK->KKval8, data->data_val, 6 );
				}
				break;

			case 12:		// 決算額 6.3%
			case 13:		// 課税取引外 6.3%
				break;
			case 14:		// 課税取引額  うち6.3％
				if( leng ){
					memcpy( pK->KKval9, data->data_val, 6 );
				}
				break;
			case 15:		// 変換等課税取引  うち6.3％
				if( leng ){
					memcpy( pK->KKval10, data->data_val, 6 );
				}
				break;

			default:
				break;
		}
	}
	else{
		switch( m_Clm ){
			case 0:		// 決算額
				::ZeroMemory( pK->KKval1, 6 );
				break;
			case 1:		// 課税取引外
				::ZeroMemory( pK->KKval2, 6 );
				break;
			case 2:		// 課税取引額
				::ZeroMemory( pK->KKval3, 6 );	
				break;
			case 3:		// 変換等課税取引
				::ZeroMemory( pK->KKval4, 6 );	
				break;
			case 4:		// 決算額 7.8%
			case 5:		// 課税取引外 7.8%
				break;
			case 6:		// 課税取引額　うち7.8%
				::ZeroMemory( pK->KKval5, 6 );	
				break;
			case 7:		// 変換等課税取引　うち7.8％
				::ZeroMemory( pK->KKval6, 6 );	
				break;
			case 8:		// 決算額 6.24
			case 9:		// 課税取引外 6.24
				break;
			case 10:	// 課税取引額  うち6.24％
				::ZeroMemory( pK->KKval7, 6 );
				break;
			case 11:	// 変換等課税取引  うち6.24％
				::ZeroMemory( pK->KKval8, 6 );
				break;
			case 12:	// 決算額 6.3%
			case 13:	// 課税取引外 6.3%
				break;
			case 14:	// 課税取引額  うち6.3％
				::ZeroMemory( pK->KKval9, 6 );
				break;
			case 15:	// 変換等課税取引  うち6.3％
				::ZeroMemory( pK->KKval10, 6 );
				break;
			default:
				break;
		}

	}

	// 集計
	(*m_Scalc)->Shz_31AllCalc();
	for( int i = 0; i < JSKLINCNT; i++ ){
		DispKniinpKeikaDiagData( i );
	}
	return 0;

}


//-----------------------------------------------------------------------------
// 背景色変更
//-----------------------------------------------------------------------------
// 引数	id		；	ダイアグラムID
//		index	：	ダイアグラム内インデックス
//		sign	：	変更サイン
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
void CShinKniinp31::ChangeColor( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, index, &DA, CIcsdiagctrl );
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
	diag_setattr( id, index, &DA, FALSE, CIcsdiagctrl );
}


BEGIN_EVENTSINK_MAP(CShinKniinp31, CSyzBaseDlg)
ON_EVENT(CShinKniinp31, IDC_ICSDIAGCTRL1, 1, CShinKniinp31::EditONIcsdiag8ctrl1, VTS_I2)
ON_EVENT(CShinKniinp31, IDC_ICSDIAGCTRL1, 2, CShinKniinp31::EditOFFIcsdiag8ctrl1, VTS_I2)
ON_EVENT(CShinKniinp31, IDC_ICSDIAGCTRL1, 3, CShinKniinp31::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinKniinp31::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_Kniinp31.OffControl( FALSE );

	static long DataLinePos[] = { 0 , 1 , 9 , 2 , 3 , 4 , 8 , 5 , 6 , 7 };
	
	m_Idx = index;

	if ((m_Idx >= JSKVMIN) && (m_Idx <= JSKVMAX)) {
		if ((m_Idx >= JSKVAL01A) && (m_Idx <= JSKVAL04D)) {//第１種
			m_Lin = DataLinePos[0];
		}
		else if ((m_Idx >= JSKVAL05A) && (m_Idx <= JSKVAL08D)) {//第２種
			m_Lin = DataLinePos[1];
		}
		else if ((m_Idx >= JSKVAL09A) && (m_Idx <= JSKVAL10D)) {//第２種(農業)
			m_Lin = DataLinePos[2];
		}
		//これより以下、2種農業の少ない分を計算の為idxプラス
		else if ((m_Idx >= JSKVAL11A) && (m_Idx <= JSKVAL14D)) {//第３種
			m_Lin = DataLinePos[3];
		}
		else if ((m_Idx >= JSKVAL15A) && (m_Idx <= JSKVAL17D)) {//第３種(農業)
			m_Lin = DataLinePos[4];
		}
		//これより以下、2種農業 + 3種農業の少ない分を計算の為idxプラス
		else if ((m_Idx >= JSKVAL18A) && (m_Idx <= JSKVAL21D)) {//第４種
			m_Lin = DataLinePos[5];
		}
		else if ((m_Idx >= JSKVAL22A) && (m_Idx <= JSKVAL25D)) {//第４種（固定資産等）
			m_Lin = DataLinePos[6];
		}
		else if ((m_Idx >= JSKVAL26A) && (m_Idx <= JSKVAL29D)) {//第５種
			m_Lin = DataLinePos[7];
		}
		else if ((m_Idx >= JSKVAL30A) && (m_Idx <= JSKVAL33D)) {//第６種
			m_Lin = DataLinePos[8];
		}
		else if ((m_Idx >= JSKVAL34A) && (m_Idx <= JSKVAL37D)) {//合計
			m_Lin = DataLinePos[9];
		}

		m_Clm = GetIndexToClmPosition(index);

	}

	//m_Idx = index;
	//if( (m_Idx>=JSKVMIN) && (m_Idx <= JSKVMAX) ){
	//	m_Lin = (index - JSKVMIN) / JSKWIDECNT;
	//	m_Clm = (index - JSKVMIN) % JSKWIDECNT;

	//	/*
	//	// これいらん気がするんで省く
	//	if(m_Clm >= 8){
	//		m_Clm += -2;
	//	}
	//	if(m_Clm >= 4){
	//		m_Clm += -2;
	//	}*/
	//}

	m_curwnd = IDC_ICSDIAGCTRL1;
}

void CShinKniinp31::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA	data;
	diag_getdata( IDC_ICSDIAGCTRL1, index, &data, CIcsdiagctrl );
	int	st = GetDiagData( index, data.data_check, &data, 0 );
	diag_deledit( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
	if( st < 0 ){
		diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );
	}
}

void CShinKniinp31::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}

	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;

	UINT	flg = 0;

	idata = (DIAGRAM_DATA *)data;
	m_Util.DiagOcxIniz( &xdata );
	m_Util.MoveDiagData( &xdata, idata );


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
		pos = FocusMoveCheck(index , 1);
		m_Kniinp31.SetPosition(pos);	
	}
	if( wTerm==VK_DOWN ){
		pos = FocusMoveCheck(index , 0);
		m_Kniinp31.SetPosition(pos);
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
		pos = FocusMoveCheck( index, 2 );
		m_Kniinp31.SetPosition(pos);
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		pos = FocusMoveCheck( index, 3 );
		m_Kniinp31.SetPosition(pos);
	}

	if( wTerm == VK_DELETE){
		GetDiagData( index, length, &xdata, -1 );
	}

	/*
	if( wTerm == VK_DELETE ){
		switch( index ){
			case ID12A	:	//  2.A 消費税(税率3%分)
				F1c &= ~0x08;
				F1c |= 0x01;
				// 各入力の計算表示
				f1calq_all( 2, 0 );
				m_Fhyo10Diag02.SetPosition( ID12A );
				break;

			case ID12B	:	//  2.B 消費税(税率4%分)
				F1c &= ~0x80;
				F1c |= 0x10;
				// 各入力の計算表示
				f1calq_all( 2, 0 );
				m_Fhyo10Diag02.SetPosition( ID12B );
				break;

			case ID12C	:	//  2.B 消費税(税率6.3%分)
				F1c &= ~0x800;
				F1c |= 0x100;
				// 各入力の計算表示
				f1calq_all( 2, 0 );
				m_Fhyo10Diag02.SetPosition( ID12C );
				break;

			default :
				dsp_cls(IDC_ICSDIAGCTRL2 , index);//[Y]
				break;
		}
	}
	*/

	SetRedrawScrollToIdx(m_Kniinp31.GetPosition(), IDC_ICSDIAGCTRL1);

/*	int nowpos = m_KniinpKei.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAG8CTRL1, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}*/

}



//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinKniinp31::FocusMoveCheck(int Index , int Move)
{
	int idx = 0;
	int i = 0 , j = 0;
	BOOL end_flg = FALSE;

	DIAGRAM_ATTRIBUTE	DA;

	//ポジションの端到達判定
	if(Move == 0){//下方向
		if( Index == m_FcsDownEnd ){
			idx = m_FcsUpEnd;
			end_flg = TRUE;
		}
	}
	else if(Move == 1){//上方向
		if( Index == m_FcsUpEnd ){
			idx = m_FcsDownEnd;
			end_flg = TRUE;
		}
	}
	else if(Move == 2){//右方向
		if( Index == m_FcsRightEnd ){
			idx = m_FcsLeftEnd;
			end_flg = TRUE;
		}
	}
	else if(Move == 3){//左方向
		if( Index == m_FcsLeftEnd ){
			idx = m_FcsRightEnd;
			end_flg = TRUE;
		}
	}

	if( end_flg == TRUE ){
		return idx;
	}

	//現在ポジション検索
	if(( Move == 0 )||( Move == 1 )){
		for(i = 0 ; i < FCS_SHINKNIINP31CNT ; i++){
			if(Index == FCS_ShinKniinp31[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinKniinp31[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SHINKNIINP31CNT ; i++){
			if(Index == FCS_ShinKniinp31_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinKniinp31_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SHINKNIINP31CNT ; j++){
			m_Kniinp31.GetAttribute(FCS_ShinKniinp31[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinKniinp31[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Kniinp31.GetAttribute(FCS_ShinKniinp31[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinKniinp31[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SHINKNIINP31CNT ; j++){
			m_Kniinp31.GetAttribute(FCS_ShinKniinp31_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinKniinp31_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Kniinp31.GetAttribute(FCS_ShinKniinp31_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinKniinp31_LR[j].IDNo;
				break;
			}
		}
	}
	return idx;
}
//<-----------------------------------------

void CShinKniinp31::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SHINKNIINP31CNT; idx++ ){
		m_Kniinp31.GetAttribute(FCS_ShinKniinp31[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_ShinKniinp31[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SHINKNIINP31CNT; idx++ ){
		m_Kniinp31.GetAttribute(FCS_ShinKniinp31_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_ShinKniinp31_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINKNIINP31CNT-1; idx > -1 ; idx-- ){
		m_Kniinp31.GetAttribute(FCS_ShinKniinp31[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_ShinKniinp31[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINKNIINP31CNT-1; idx > -1 ; idx-- ){
		m_Kniinp31.GetAttribute(FCS_ShinKniinp31_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_ShinKniinp31_LR[idx].IDNo;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 初期ポジションセット'14.09.01
//-----------------------------------------------------------------------------
void CShinKniinp31::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
		if( (index = m_Kniinp31.GetPosition()) != -1) {
			m_Kniinp31.SetPosition(index);
			m_Kniinp31.SetFocus();
			return;
		}
	}

	this->SetFocus();
	/*
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Kniinp.SetPosition(JSVAL01A);
	}
	else{
		m_Kniinp.SetPosition(KBUYVAL07A);
	}
	*/

	m_Kniinp31.SetPosition(JSKVAL01A);	

	SetRedrawScroll( 0 );

}

int CShinKniinp31::DispJudge( int clm, int line )
{
	int st = 0;
	int j = 0;

	switch( clm ){
		case 0:	// 決算額
			switch( line ){
				case KKNIINPLINE_KEI:		//合計
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
						//if((j == KKNIINPLINE2_NOUGYO) || (j == KKNIINPLINE3_NOUGYO)||(j == KKNIINPLINE_KEI)){//農業事業者・合計行スキップ
						if( j == KKNIINPLINE_KEI ){	// '20.02.06 合計行スキップ
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval1) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + line].KKval1) ){
						st = 1;	
					}
					break;
			}

			break;

		case 1:	// 課税取引外
			switch( line ){
				case KKNIINPLINE_KEI:		//合計
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
						//if ((j == KKNIINPLINE2_NOUGYO) || (j == KKNIINPLINE3_NOUGYO) || (j == KKNIINPLINE_KEI)) {//農業事業者・合計行スキップ
						if( j == KKNIINPLINE_KEI ){	// '20.02.06 合計行スキップ
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval2) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + line].KKval2) ){
						st = 1;	
					}
					break;
			}

			break;

		case 2:	// 課税取引額
			switch( line ){
				case KKNIINPLINE_KEI:		//合計
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
						//if ((j == KKNIINPLINE2_NOUGYO) || (j == KKNIINPLINE3_NOUGYO) || (j == KKNIINPLINE_KEI)) {//農業事業者・合計行スキップ
						if( j == KKNIINPLINE_KEI ){	// '20.02.06 合計行スキップ
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval1) ){
							st = 1;
							break;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval2) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if(( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + line].KKval1))||( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + line].KKval2))){
						st = 1;	
					}
					break;
			}

			break;

		case 3:	// 返還等課税取引
			switch( line ){
				case KKNIINPLINE_KEI:		//合計
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
						//if ((j == KKNIINPLINE2_NOUGYO) || (j == KKNIINPLINE3_NOUGYO) || (j == KKNIINPLINE_KEI)) {//農業事業者・合計行スキップ
						if( j == KKNIINPLINE_KEI ){	// '20.02.06 合計行スキップ
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval4) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + line].KKval4) ){
						st = 1;
					}
					break;
			}

			break;

		case 4: // 決算額 うち税率7.8%適用分
			break;
		case 5: // 課税取引外 うち税率7.8%適用分
			break;
		case 6:	// 課税取引額 うち税率7.8%適用分
			switch( line ){
				case KKNIINPLINE_KEI:		//合計
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
						//if ((j == KKNIINPLINE2_NOUGYO) || (j == KKNIINPLINE3_NOUGYO) || (j == KKNIINPLINE_KEI)) {//農業事業者・合計行スキップ
						if( j == KKNIINPLINE_KEI ){	// '20.02.06 合計行スキップ
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval5) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + line].KKval5) ){
						st = 1;	
					}
					break;
			}

			break;

		case 7:	// 返還等課税取引 うち税率7.8%適用分
			switch( line ){
				case KKNIINPLINE_KEI:		//合計
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
						//if ((j == KKNIINPLINE2_NOUGYO) || (j == KKNIINPLINE3_NOUGYO) || (j == KKNIINPLINE_KEI)) {//農業事業者・合計行スキップ
						if( j == KKNIINPLINE_KEI ){	// '20.02.06 合計行スキップ
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval6) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + line].KKval6) ){
						st = 1;
					}
					break;
			}

			break;

		case 8: // 決算額 うち税率6.24%適用分
			break;
		case 9: // 課税取引外 うち税率6.24%適用分
			break;

		case 10:// 課税取引額 うち税率6.24%適用分
			switch( line ){
				case KKNIINPLINE_KEI:		//合計
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
						//if ((j == KKNIINPLINE2_NOUGYO) || (j == KKNIINPLINE3_NOUGYO) || (j == KKNIINPLINE_KEI)) {//農業事業者・合計行スキップ
						if( j == KKNIINPLINE_KEI ){	// '20.02.06 合計行スキップ
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval7) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + line].KKval7) ){
						st = 1;
					}
					break;
			}

			break;

		case 11:// 返還等課税取引 うち税率6.24%適用分
			switch( line ){
				case KKNIINPLINE_KEI:		//合計
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
						//if ((j == KKNIINPLINE2_NOUGYO) || (j == KKNIINPLINE3_NOUGYO) || (j == KKNIINPLINE_KEI)) {//農業事業者・合計行スキップ
						if( j == KKNIINPLINE_KEI ){	// '20.02.06 合計行スキップ
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval8) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + line].KKval8) ){
						st = 1;
					}
					break;
			}

			break;

		case 12:	// 決算額　うち税率6.3％
			break;
		case 13:	// 課税取引外　うち税率6.3％
			break;
		case 14:	// 課税取引金額　うち税率6.3％
			if ( ( *m_pSnHeadData )->SVmzsw == 0 ) {
				break;
			}
			switch( line ){
				case KKNIINPLINE_KEI:	// 合計行
					for( j = KKNIINPLINE1; j < JSKLINCNT+1; j++ ){
						//if ((j == KKNIINPLINE2_NOUGYO) || (j == KKNIINPLINE3_NOUGYO) || (j == KKNIINPLINE_KEI)) {//農業事業者・合計行スキップ
						if( j == KKNIINPLINE_KEI ){	// '20.02.06 合計行スキップ
							continue;
						}
						if( m_pArith->l_test( (*m_pTaxListData)->NRec.KUval[40 + j].KKval9 ) ){
							st = 1;
							break;
						}
					}
					break;
				default:
					if( m_pArith->l_test( (*m_pTaxListData)->NRec.KUval[40 + line].KKval9 ) ){
						st = 1;
						break;
					}
					break;
			}
			break;
		case 15:	// 返還等課税取引　うち税率6.3％
			if ( ( *m_pSnHeadData )->SVmzsw == 0 ) {
				break;
			}
			switch( line ){
				case KKNIINPLINE_KEI:
					for( j = KKNIINPLINE1; j < JSKLINCNT+1; j++ ){
						//if ((j == KKNIINPLINE2_NOUGYO) || (j == KKNIINPLINE3_NOUGYO) || (j == KKNIINPLINE_KEI)) {//農業事業者・合計行スキップ
						if( j == KKNIINPLINE_KEI ){	// '20.02.06 合計行スキップ
							continue;
						}
						if( m_pArith->l_test( (*m_pTaxListData)->NRec.KUval[40 + j].KKval10 ) ){
							st = 1;
							break;
						}
					}
					break;
				default:
					if( m_pArith->l_test( (*m_pTaxListData)->NRec.KUval[40 + line].KKval10 ) ){
						st = 1;
						break;
					}
					break;
			}
			break;
	}

	return st;
}


// 手入力サインのクリア
int CShinKniinp31::ClearManualInputSign()
{
	int rt = 0;

	for( int idx = 40; idx < 50; idx++ ){
		(*m_pTaxListData)->NRec.KUval[idx].KKauto = 0x00;
	}

	return rt;
}

//-----------------------------------------------------------------------------
// IDXよりカラムポジションを返送
//-----------------------------------------------------------------------------
// 返送値	カラムポジション
//-----------------------------------------------------------------------------
int CShinKniinp31::GetIndexToClmPosition(int index)
{
	int clmpos = 0;

	if ((index >= JSKVAL01A) && (index <= JSKVAL04D)) {//第１種
		clmpos = (index - JSKVMIN) % JSKWIDECNT;
	}
	else if ((m_Idx >= JSKVAL05A) && (m_Idx <= JSKVAL08D)) {//第２種
		clmpos = (index - JSKVMIN) % JSKWIDECNT;
	}
	else if ((m_Idx >= JSKVAL09A) && (m_Idx <= JSKVAL09D)) {//第２種(農業　計)
		clmpos = (index - JSKVMIN) % JSKWIDECNT;
	}
	else if ((m_Idx >= JSKVAL10A) && (m_Idx <= JSKVAL10D)) {//第２種(農業　６．２４％)
		clmpos = (index - JSKVMIN + 4) % JSKWIDECNT;
	}

	//これより以下、2種農業の少ない分を計算の為idxプラス
	else if ((m_Idx >= JSKVAL11A) && (m_Idx <= JSKVAL14D)) {//第３種
		clmpos = (index - JSKVMIN + 8) % JSKWIDECNT;
	}
	else if ((m_Idx >= JSKVAL15A) && (m_Idx <= JSKVAL16D)) {//第３種(農業　計～７．８％)
		clmpos = (index - JSKVMIN + 8) % JSKWIDECNT;
	}
	else if ((m_Idx >= JSKVAL17A) && (m_Idx <= JSKVAL17D)) {//第３種(農業６．３％)
		clmpos = (index - JSKVMIN + 12) % JSKWIDECNT;
	}
	//これより以下、2種農業 + 3種農業の少ない分を計算の為idxプラス
	else if ((m_Idx >= JSKVAL18A) && (m_Idx <= JSKVAL21D)) {//第４種
		clmpos = (index - JSKVMIN + 12) % JSKWIDECNT;
	}
	else if ((m_Idx >= JSKVAL22A) && (m_Idx <= JSKVAL25D)) {//第４種（固定資産等）
		clmpos = (index - JSKVMIN + 12) % JSKWIDECNT;
	}
	else if ((m_Idx >= JSKVAL26A) && (m_Idx <= JSKVAL29D)) {//第５種
		clmpos = (index - JSKVMIN + 12) % JSKWIDECNT;
	}
	else if ((m_Idx >= JSKVAL30A) && (m_Idx <= JSKVAL33D)) {//第６種
		clmpos = (index - JSKVMIN + 12) % JSKWIDECNT;
	}
	else if ((m_Idx >= JSKVAL34A) && (m_Idx <= JSKVAL37D)) {//合計
		clmpos = (index - JSKVMIN + 12) % JSKWIDECNT;
	}
	else {

	}

	return clmpos;
}

// 修正No.168690 add -->
void CShinKniinp31::DiagReSetPosition(void)
{
	//this->SetFocus();
}
// 修正No.168690 add <--
