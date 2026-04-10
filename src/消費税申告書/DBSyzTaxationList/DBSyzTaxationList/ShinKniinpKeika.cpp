// ShinKniinpKeika.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinKniinpKeika.h"
#include "ShinKniinpKeikaIdx.h"

// CShinKniinpKeika ダイアログ

IMPLEMENT_DYNAMIC(CShinKniinpKeika, CSyzBaseDlg)

CShinKniinpKeika::CShinKniinpKeika(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinKniinpKeika::IDD, pParent)
	, m_pParent(pParent)
	, m_Idx(0)
	, m_Lin(0)
	, m_Clm(0)
{
	m_curwnd = -1;
}

CShinKniinpKeika::~CShinKniinpKeika()
{
}

void CShinKniinpKeika::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_KniinpKei);
}


BEGIN_MESSAGE_MAP(CShinKniinpKeika, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinKniinpKeika メッセージ ハンドラ

BOOL CShinKniinpKeika::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_KniinpKei;
//2016.10.20 UPDATE START
//	m_BottomIdx = 29;//コントロールの一番下のIDX指定するとスクロー9ルがそこまでいく
	m_BottomIdx = 35;//コントロールの一番下のIDX指定するとスクロー9ルがそこまでいく
//2016.10.20 UPDATE END

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinKniinpKeika::Init()
{
	InitDiagAttr();

	DispList(0);//'14.09.01

	SetItemPosition();	// フォーカスセット'14.09.01

	return 0;
}


//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
void CShinKniinpKeika::InitDiagAttr()
{
	int	cnt = 0;
	int	Flg;
	while( 1 ){
		if( ShinKniinpKeika[cnt].Index == 0 ){
			break;
		}
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01){
			Flg = ShinKniinpKeika[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = ShinKniinpKeika[cnt].UnConnect;
			}
			else{
				Flg = ShinKniinpKeika[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAG8CTRL1, ShinKniinpKeika[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, ShinKniinpKeika[cnt].Connect );
		cnt++;
	}

	SetFocusRange();

	m_KniinpKei.EnableDelete(0);//DELETEキー１回で削除モード設定'14.08.01

}
BOOL CShinKniinpKeika::PreTranslateMessage(MSG* pMsg)
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
				if( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
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
void CShinKniinpKeika::DispList( int mode )
{
	m_KniinpKei.OffControl( TRUE );
	for( int i = 0 ; i < JSKLINCNT ; i++ ){
		DispKniinpKeikaDiagData(i);
	}
	m_KniinpKei.UpdateWindow();

	DIAGRAM_DATA	data;

	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAG8CTRL1, JSKZKBNID, TRUE, CIcsdiagctrl );
	data.data_disp = _T("経理方式：");
	if( (*m_pTaxListData)->Head.KHstax&0x01 ){
		data.data_disp += _T("税抜き");
	}
	else{
		data.data_disp += _T("税込み");
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, JSKZKBNID, &data, CIcsdiagctrl );

	if( !mode ){
		m_Idx = JSKVMIN;
	}
	diag_setposition( IDC_ICSDIAG8CTRL1, m_Idx, CIcsdiagctrl );

	//SetRedrawScrollToIdx8(m_KniinpKei.GetPosition());
}


//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
void CShinKniinpKeika::DispKniinpKeikaDiagData( long dlin )
{

	//'14.08.25
	short	idx, i;
	long	lin;

	int pos = 0;

	//データ保存ポジションセット
//2016.10.20 UPDATE START
//	static long KniinpKeiDataPos[] = {0 , 1 , 2 , 4 , 8 , 5 , 6 , 7};
	static long KniinpKeiDataPos[] = {0 , 1 , 2 , 3 , 4 , 8 , 5 , 6 , 7};
//2016.10.20 UPDATE END

	lin = KniinpKeiDataPos[dlin];

	//if(lin == 4){//経過措置に農業事業者ないので処理スキップ
	//	return ;
	//}

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KUval[40 + lin];

	char	val[6];
	DIAGRAM_DATA	data;
	int		st;

	idx = (short)( dlin * JSKWIDECNT ) + JSKVMIN;

	for( i = 0 ; i < JSKWIDECNT ; i++ ){


		pos = idx + i;
//		if(i >= 4){
//			pos += 2;
//		}
//		if(i >= 6){
////		if(i >= 8){
//			pos += 2;
//		}
		
	
		st = 0;
		::ZeroMemory( val, sizeof(val));
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAG8CTRL1, pos , TRUE, CIcsdiagctrl );
//		diag_clear( IDC_ICSDIAG8CTRL1, idx+i, TRUE, CIcsdiagctrl );
/*		switch( i ){
			case 0:	// 決算額
				switch( lin ){
					case KKNIINPLINE9:		//合計
						for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
							if(j == KKNIINPLINE9){//合計行スキップ
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval1) ){
								st = 1;
								break;
							}
						}
						break;

					default:
						st = 1;	
						break;
				}

				memcpy( val, pK->KKval1, 6 );
				break;

			case 1:	// 課税取引外
				switch( lin ){
					case KKNIINPLINE9:		//合計
						for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
							if(j == KKNIINPLINE9){//合計行スキップ
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval2) ){
								st = 1;
								break;
							}
						}
						break;

					default:
						st = 1;	
						break;
				}

				memcpy( val, pK->KKval2, 6 );
				break;

			case 2:	// 課税取引額
			//	switch( lin ){		
			//		default:
			//			st = 1;	
			//			break;
			//	}
				st = 1;
				memcpy( val, pK->KKval3, 6 );
				break;

			case 3:	// 返還等課税取引
				switch( lin ){
					case KKNIINPLINE9:		//合計
						for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
							if(j == KKNIINPLINE9){//合計行スキップ
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval4) ){
								st = 1;
								break;
							}
						}
						break;

					default:
						st = 1;	
						break;
				}

				memcpy( val, pK->KKval4, 6 );
				break;

			case 4:		// 課税取引額 うち税率6.3%適用分
				switch( lin ){
					case KKNIINPLINE9:		//合計
						for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
							if(j == KKNIINPLINE9){//合計行スキップ
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval5) ){
								st = 1;
								break;
							}
						}
						break;

					default:
						st = 1;	
						break;
				}

				memcpy( val, pK->KKval5, 6 );
				break;

			case 5:		// 返還等課税取引 うち税率6.3%適用分
				switch( lin ){
					case KKNIINPLINE9:		//合計
						for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
							if(j == KKNIINPLINE9){//合計行スキップ
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval6) ){
								st = 1;
								break;
							}
						}
						break;

					default:
						st = 1;	
						break;
				}

				memcpy( val, pK->KKval6, 6 );
				break;

			case 6:		// 課税取引額 うち税率4%適用分
				switch( lin ){
					case KKNIINPLINE9:		//合計
						for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
							if(j == KKNIINPLINE9){//合計行スキップ
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval7) ){
								st = 1;
								break;
							}
						}
						break;

					default:
						st = 1;	
						break;
				}

				memcpy( val, pK->KKval7, 6 );
				break;

			case 7:	// 返還等課税取引 うち税率4%適用分
				switch( lin ){
					case KKNIINPLINE9:		//合計
						for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
							if(j == KKNIINPLINE9){//合計行スキップ
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + j].KKval8) ){
								st = 1;
								break;
							}
						}
						break;

					default:
						st = 1;	
						break;
				}

				memcpy( val, pK->KKval8, 6 );
				break;
		}*/

		st = DispJudge( i, lin );

		switch( i ){
			case 0:	// 決算額
				memcpy( val, pK->KKval1, 6 );
				break;

//			case 1:	// 課税取引外
//				memcpy( val, pK->KKval3, 6 );
//				break;

			case 1:	// 課税取引外
				memcpy( val, pK->KKval2, 6 );
				break;

			case 2:	// 課税取引額
				memcpy( val, pK->KKval3, 6 );
				break;

			case 3:	// 返還等課税取引
				memcpy( val, pK->KKval4, 6 );
				break;

			case 4: // 決算額 うち税率6.3%適用分
				continue;
				break;

			case 5: // 課税取引外 うち税率6.3%適用分
				continue;
				break;

			case 6:	// 課税取引額 うち税率6.3%適用分
				memcpy( val, pK->KKval5, 6 );
				break;

			case 7:	// 返還等課税取引 うち税率6.3%適用分
				memcpy( val, pK->KKval6, 6 );
				break;

			case 8: // 決算額 うち税率4%適用分
				continue;
				break;

			case 9: // 課税取引外 うち税率4%適用分
				continue;
				break;

			case 10:	// 課税取引額 うち税率4%適用分
				memcpy( val, pK->KKval7, 6 );
				break;

			case 11:	// 返還等課税取引 うち税率4%適用分
				memcpy( val, pK->KKval8, 6 );
				break;
		}

		if( st ){
			memcpy( data.data_val, val, 6 );
			diag_setdata( IDC_ICSDIAG8CTRL1, pos , &data, CIcsdiagctrl );
//			diag_setdata( IDC_ICSDIAG8CTRL1, idx+i, &data, CIcsdiagctrl );
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
int CShinKniinpKeika::GetDiagData( long index, short leng, DIAGRAM_DATA *data, int mode )
{

	if( index < 0 ){
		return 0;
	}

	char	dbuf[128]/*, buf[128]*/;
	char	*buff;
	int		len;

	
	len = data->data_edit.GetLength();
	memset( dbuf, '\0', sizeof(dbuf) );
	buff = (char *)((LPCTSTR)(data->data_edit));
	memmove( dbuf, buff, len );

	//'14.08.20
	int clm_pos = 0;
	clm_pos = (m_Idx - JSKVMIN) % JSKWIDECNT;


	//データ保存ポジションセット
	long	lin;
		//データ保存ポジションセット
//2016.10.20 UPDATE START
//	static long KniinpDataPos[] = {0 , 1 , 2 , 4 , 8 , 5 , 6 , 7};
	static long KniinpDataPos[] = {0 , 1 , 2 , 3 , 4 , 8 , 5 , 6 , 7};
//2016.10.20 UPDATE END

	lin = KniinpDataPos[m_Lin];

	//if(lin == 4){//経過措置に農業事業者ないので処理しない
	//	return 0;
	//}


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
			case 4:		// 課税取引額 うち６．３％
				if( leng ){
					memcpy( pK->KKval5, data->data_val, 6 );
				}
				break;
			case 5:		// 変換等課税取引　うち６．３％
				if( leng ){
					memcpy( pK->KKval6, data->data_val, 6 );
				}
				break;
			case 6:		// 課税取引額 うち４％
				if( leng ){
					memcpy( pK->KKval7, data->data_val, 6 );
				}
				break;

			case 7:		// 変換等課税取引　うち４％
				if( leng ){
					memcpy( pK->KKval8, data->data_val, 6 );
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
			case 4:		// 課税取引額 うち６．３％
				::ZeroMemory( pK->KKval5, 6 );	
				break;
			case 5:		// 変換等課税取引　うち６．３％
				::ZeroMemory( pK->KKval6, 6 );	
				break;
			case 6:		// 課税取引額 うち４％
				::ZeroMemory( pK->KKval7, 6 );	
				break;
			case 7:		// 変換等課税取引　うち４％
				::ZeroMemory( pK->KKval8, 6 );	
				break;

			default:
				break;
		}

	}

	// 集計
	(*m_Scalc)->Shz_KeiAllCalc();
	for( int i = 0 ; i < JSKLINCNT ; i++ ){
		DispKniinpKeikaDiagData( i );
	}
	return 0;

}


BEGIN_EVENTSINK_MAP(CShinKniinpKeika, CSyzBaseDlg)
ON_EVENT(CShinKniinpKeika, IDC_ICSDIAG8CTRL1, 1, CShinKniinpKeika::EditONIcsdiag8ctrl1, VTS_I2)
ON_EVENT(CShinKniinpKeika, IDC_ICSDIAG8CTRL1, 2, CShinKniinpKeika::EditOFFIcsdiag8ctrl1, VTS_I2)
ON_EVENT(CShinKniinpKeika, IDC_ICSDIAG8CTRL1, 3, CShinKniinpKeika::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinKniinpKeika::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_KniinpKei.OffControl( FALSE );

	m_Idx = index;
	if( (m_Idx>=JSKVMIN) && (m_Idx <= JSKVMAX) ){
		m_Lin = (index - JSKVMIN) / JSKWIDECNT;
		m_Clm = (index - JSKVMIN) % JSKWIDECNT;

		if(m_Clm >= 8){
			m_Clm += -2;
		}
		if(m_Clm >= 4){
			m_Clm += -2;
		}
		


	}
//	else{
//		m_Lin = -1;
//		m_Clm = -1;
//	}

	m_curwnd = IDC_ICSDIAG8CTRL1;//'14.08.01
}

void CShinKniinpKeika::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA	data;
	diag_getdata( IDC_ICSDIAG8CTRL1, index, &data, CIcsdiagctrl );
	int	st = GetDiagData( index, data.data_check, &data, 0 );
	diag_deledit( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
	if( st < 0 ){
		diag_setposition( IDC_ICSDIAG8CTRL1, m_Idx, CIcsdiagctrl );
	}
}

void CShinKniinpKeika::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}

	//-------->'14.08.06
	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;

	UINT	flg = 0;

	idata = (DIAGRAM_DATA *)data;
	m_Util.DiagOcxIniz( &xdata );
	m_Util.MoveDiagData( &xdata, idata );
	//<---------------

	//フォーカス移動処理対応'14.08.01
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
	//	if(index == KBUYVAL14){
	//		m_BuyKei.SetPrevPosition();
	//	}
	//	else{
//			pos = UpDownFocusMoveCheck(index , 1);
			pos = FocusMoveCheck(index , 1);
			m_KniinpKei.SetPosition(pos);	
	//	}
	}
	if( wTerm==VK_DOWN ){
	//	if(index == KBUYVAL14){
	//		m_BuyKei.SetNextPosition();
	//	}
	//	else{
//			pos = UpDownFocusMoveCheck(index , 0);
			pos = FocusMoveCheck(index , 0);
			m_KniinpKei.SetPosition(pos);
	//	}
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
		m_KniinpKei.SetNextPosition();
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		m_KniinpKei.SetPrevPosition();
	}

	//------->DELETEキー対応'14.08.06
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


	//<--------------

	SetRedrawScrollToIdx8(m_KniinpKei.GetPosition());

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
//int CShinKniinpKeika::UpDownFocusMoveCheck(int Index , int Move)
int CShinKniinpKeika::FocusMoveCheck(int Index , int Move)
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
		for(i = 0 ; i < FCS_SHINKNIINPKEIKACNT ; i++){
			if(Index == FCS_ShinKniinpKeika[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinKniinpKeika[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SHINKNIINPKEIKACNT ; i++){
			if(Index == FCS_ShinKniinpKeika_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinKniinpKeika_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SHINKNIINPKEIKACNT ; j++){
			m_KniinpKei.GetAttribute(FCS_ShinKniinpKeika[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinKniinpKeika[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_KniinpKei.GetAttribute(FCS_ShinKniinpKeika[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinKniinpKeika[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SHINKNIINPKEIKACNT ; j++){
			m_KniinpKei.GetAttribute(FCS_ShinKniinpKeika_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinKniinpKeika_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_KniinpKei.GetAttribute(FCS_ShinKniinpKeika_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinKniinpKeika_LR[j].IDNo;
				break;
			}
		}
	}
	return idx;
}
//<-----------------------------------------

void CShinKniinpKeika::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SHINKNIINPKEIKACNT; idx++ ){
		m_KniinpKei.GetAttribute(FCS_ShinKniinpKeika[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_ShinKniinpKeika[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SHINKNIINPKEIKACNT; idx++ ){
		m_KniinpKei.GetAttribute(FCS_ShinKniinpKeika_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_ShinKniinpKeika_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINKNIINPKEIKACNT-1; idx > -1 ; idx-- ){
		m_KniinpKei.GetAttribute(FCS_ShinKniinpKeika[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_ShinKniinpKeika[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINKNIINPKEIKACNT-1; idx > -1 ; idx-- ){
		m_KniinpKei.GetAttribute(FCS_ShinKniinpKeika_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_ShinKniinpKeika_LR[idx].IDNo;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 初期ポジションセット'14.09.01
//-----------------------------------------------------------------------------
void CShinKniinpKeika::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if( (index = m_KniinpKei.GetPosition()) != -1) {
			m_KniinpKei.SetPosition(index);
			m_KniinpKei.SetFocus();
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

	m_KniinpKei.SetPosition(JSKVAL01A);

	SetRedrawScroll( 0 );

}

int CShinKniinpKeika::DispJudge( int clm, int line )
{
	int st = 0;
	int j = 0;

	switch( clm ){
		case 0:	// 決算額
			switch( line ){
				case KKNIINPLINE9:		//合計
//					for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
//						if(j == KKNIINPLINE9){//合計行スキップ
						if((j == KKNIINPLINE4)||(j == KKNIINPLINE9)){//農業事業者・合計行スキップ
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
				case KKNIINPLINE9:		//合計
//					for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
//						if(j == KKNIINPLINE9){//合計行スキップ
						if((j == KKNIINPLINE4)||(j == KKNIINPLINE9)){//農業事業者・合計行スキップ
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
				case KKNIINPLINE9:		//合計
//					for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
//						if(j == KKNIINPLINE9){//合計行スキップ
						if((j == KKNIINPLINE4)||(j == KKNIINPLINE9)){//農業事業者・合計行スキップ
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
				case KKNIINPLINE9:		//合計
//					for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
//						if(j == KKNIINPLINE9){//合計行スキップ
						if((j == KKNIINPLINE4)||(j == KKNIINPLINE9)){//農業事業者・合計行スキップ
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

		case 4: // 決算額 うち税率6.3%適用分
			break;
		case 5: // 課税取引外 うち税率6.3%適用分
			break;
		case 6:	// 課税取引額 うち税率6.3%適用分
			switch( line ){
				case KKNIINPLINE9:		//合計
//					for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
//						if(j == KKNIINPLINE9){//合計行スキップ
						if((j == KKNIINPLINE4)||(j == KKNIINPLINE9)){//農業事業者・合計行スキップ
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

		case 7:	// 返還等課税取引 うち税率6.3%適用分
			switch( line ){
				case KKNIINPLINE9:		//合計
//					for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
//						if(j == KKNIINPLINE9){//合計行スキップ
						if((j == KKNIINPLINE4)||(j == KKNIINPLINE9)){//農業事業者・合計行スキップ
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

		case 8: // 決算額 うち税率4%適用分
			break;
		case 9: // 課税取引外 うち税率4%適用分
			break;

		case 10:// 課税取引額 うち税率4%適用分
			switch( line ){
				case KKNIINPLINE9:		//合計
//					for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
//						if(j == KKNIINPLINE9){//合計行スキップ
						if((j == KKNIINPLINE4)||(j == KKNIINPLINE9)){//農業事業者・合計行スキップ
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

		case 11:// 返還等課税取引 うち税率4%適用分
			switch( line ){
				case KKNIINPLINE9:		//合計
//					for( j = KKNIINPLINE1 ; j < JSKLINCNT ; j++ ){
					for( j = KKNIINPLINE1 ; j < JSKLINCNT+1 ; j++ ){
//						if(j == KKNIINPLINE9){//合計行スキップ
						if((j == KKNIINPLINE4)||(j == KKNIINPLINE9)){//農業事業者・合計行スキップ
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
	}

	return st;
}


// 手入力サインのクリア
int CShinKniinpKeika::ClearManualInputSign()
{
	int rt = 0;

	for( int idx = 40; idx < 50; idx++ ){
		(*m_pTaxListData)->NRec.KUval[idx].KKauto = 0x00;
	}

	return rt;
}