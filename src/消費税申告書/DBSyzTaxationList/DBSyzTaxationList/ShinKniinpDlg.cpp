// ShinKniinpDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinKniinpDlg.h"
#include "ShinKniinpDlgIdx.h"


// CShinKniinpDlg ダイアログ

IMPLEMENT_DYNAMIC(CShinKniinpDlg, CSyzBaseDlg)

CShinKniinpDlg::CShinKniinpDlg(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinKniinpDlg::IDD, pParent)
	, m_pParent(pParent)
	, m_Idx(0)
	, m_Lin(0)
	, m_Clm(0)
{
	m_curwnd = -1;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;
}

CShinKniinpDlg::~CShinKniinpDlg()
{
}

void CShinKniinpDlg::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Kniinp);
}


BEGIN_MESSAGE_MAP(CShinKniinpDlg, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinKniinpDlg メッセージ ハンドラ

BOOL CShinKniinpDlg::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Kniinp;
	m_BottomIdx = 9;//コントロールの一番下のIDX指定するとスクロールがそこまでいく

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinKniinpDlg::Init()
{
	InitDiagAttr();

	DispList(0);//'14.09.01

	SetItemPosition();	// フォーカスセット'14.09.01

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
void CShinKniinpDlg::InitDiagAttr()
{
	int	cnt = 0;
	int	Flg;
	while( 1 ){
		if( ShinKniinp[cnt].Index == 0 ){
			break;
		}
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01){
			Flg = ShinKniinp[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = ShinKniinp[cnt].UnConnect;
			}
			else{
				Flg = ShinKniinp[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAG8CTRL1, ShinKniinp[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, ShinKniinp[cnt].Connect );
		cnt++;
	}

	SetFocusRange();

	m_Kniinp.EnableDelete(0);//DELETEキー１回で削除モード設定'14.08.01

}



BOOL CShinKniinpDlg::PreTranslateMessage(MSG* pMsg)
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
void CShinKniinpDlg::DispList( int mode )
{
	m_Kniinp.OffControl( TRUE );
	for( int i = 0 ; i < JSLINCNT ; i++ ){
		DispKniinpDiagData(i);
	}
	
	m_Kniinp.UpdateWindow();

	DIAGRAM_DATA	data;
/*	if( !mode ){
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAG8CTRL1, BKYEARID, TRUE, CIcsdiagctrl );
		data.data_disp.Format("（平成%2x年分）", (*m_pTaxListData)->YEAR);
		diag_setdata( IDC_ICSDIAG8CTRL1, BKYEARID, &data, CIcsdiagctrl );

		PostMessage( WM_VSCROLL, SB_TOP, 0 );
		if( (*m_pTaxListData)->GKBN & 0x04 ){//農業
//			m_Idx = VAL04;
		}
		else{
			if( (*m_pTaxListData)->GKBN & 0x02 ){//不動産
				m_Idx = KBUYVAL07A;
			}
			else{
				m_Idx = BKVMIN;
			}
		}
	}
	*/

	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAG8CTRL1, JSZKBNID, TRUE, CIcsdiagctrl );
	data.data_disp = _T("経理方式：");
	if( (*m_pTaxListData)->Head.KHstax&0x01 ){
		data.data_disp += _T("税抜き");
	}
	else{
		data.data_disp += _T("税込み");
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, JSZKBNID, &data, CIcsdiagctrl );

//	if( mode ){
//		if( m_Idx == KBUYVAL14K && !(*m_pTaxListData)->Head.KHstax ){
//			m_Idx = KBUYVAL14C;
//		}
//		diag_setposition( IDC_ICSDIAG8CTRL1, m_Idx, CIcsdiagctrl );
//	}

	if( !mode ){
		m_Idx = JSVMIN;
	}

	diag_setposition( IDC_ICSDIAG8CTRL1, m_Idx, CIcsdiagctrl );
	
	//SetRedrawScrollToIdx8(m_Kniinp.GetPosition());
}


//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
void CShinKniinpDlg::DispKniinpDiagData( long dlin )
{

	//'14.08.25
	short	idx, i;
	long	lin;

	//データ保存ポジションセット
	static long KniinpDataPos[] = {0 , 1 , 2 , 3 , 4 , 8 , 5 , 6 , 7};

	lin = KniinpDataPos[dlin];

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KUval[40 + lin];

	char	val[6];
	DIAGRAM_DATA	data;
	int		st;

	idx = (short)( dlin * JSWIDECNT ) + JSVMIN;

	for( i = 0 ; i < JSWIDECNT ; i++ ){
		st = 0;
		::ZeroMemory( val, sizeof(val));
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAG8CTRL1, idx+i, TRUE, CIcsdiagctrl );
/*		switch( i ){
			case 0:	// 決算額
				switch( lin ){
					case KNIINPLINE6:		//合計
						for( j = KNIINPLINE1 ; j < JSLINCNT ; j++ ){
							if(j == KNIINPLINE6){//合計行スキップ
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
					case KNIINPLINE6:		//合計
						for( j = KNIINPLINE1 ; j < JSLINCNT ; j++ ){
							if(j == KNIINPLINE6){//合計行スキップ
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
					case KNIINPLINE6:		//合計
						for( j = KNIINPLINE1 ; j < JSLINCNT ; j++ ){
							if(j == KNIINPLINE6){//合計行スキップ
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
		}*/

		st = DispJudge( i, lin );
		switch( i ){
			case 0:	// 決算額
				memcpy( val, pK->KKval1, 6 );
				break;
			case 1:	// 課税取引外
				memcpy( val, pK->KKval2, 6 );
				break;
			case 2:	// 課税取引額
				memcpy( val, pK->KKval3, 6 );
				break;
			case 3:	// 返還等課税取引
				memcpy( val, pK->KKval4, 6 );
				break;
		}

		if( st ){
			memcpy( data.data_val, val, 6 );
			diag_setdata( IDC_ICSDIAG8CTRL1, idx+i, &data, CIcsdiagctrl );
		}

	}

	/*
	
	//'14.08.25
	short	idx, i, j;
	long	lin;

	//データ保存ポジションセット

	static long KNouDataPos[] = {0 , 1 , 2 , 3 , 4 , 5 , 38 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 , 17 , 18 , 19 , 
		                         20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 , 28 , 29 , 30 , 31 , 32 , 33 , 34 , 35 , 36 , 37};

	lin = KNouDataPos[dlin];

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KNval[lin];
	char	val[6];
	DIAGRAM_DATA	data;
	int		st;

	idx = (short)( dlin * NKWIDECNT ) + NKVMIN;
	for( i = 0 ; i < NKWIDECNT ; i++ ){
		st = 0;
		::ZeroMemory( val, sizeof(val));
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAG8CTRL1, idx+i, TRUE, CIcsdiagctrl );
		switch( i ){
			case 0:	// 決算額 A
				switch( lin ){
					case KNOULINE3M:	//未成熟果樹収入
						break;

					case KNOULINE36:	//差引金額
						st = 1;
						break;

					case KNOULINE8:		//計
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE6].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE7].KKval1) ){
							st = 1;
							break;
						}
						break;
					case KNOULINE4:		//小計
						for( j = KNOULINE1 ; j < KNOULINE4 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
								st = 1;
								break;
							}
						}
						break;
			//			if( lin != 37 ){
			//				break;
			//			}
					case KNOULINE35:	//計
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE32].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE33].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE34].KKval1) ){
							st = 1;
							break;
						}
						break;

					case KNOULINE31:	//小計
						for( j = KNOULINE8 ; j < KNOULINE31 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
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

			case 1:	// Aのうち課税取引にならないもの　B
				switch( lin ){
					case KNOULINE2K:	//家事消費
					case KNOULINE3M:	//未成熟果樹収入
					case KNOULINE5:		//農産物の棚卸高　期首
					case KNOULINE6:		//農産物の棚卸高　期末
					case KNOULINE7:		//計
					case KNOULINE13:	//農具費
					case KNOULINE14:	//農薬・衛生費
					case KNOULINE15:	//諸材料費
					case KNOULINE16:	//修繕費
					case KNOULINE18:	//作業用衣料費
					case KNOULINE32:	//農産物以外の棚卸高　期首
					case KNOULINE33:	//農産物以外の棚卸高　期末
					case KNOULINE34:	//経費から差し引く果樹牛馬等の育成費用
					case KNOULINE35:	//計
					case KNOULINE36:	//差引金額
						//入力可所無し
						break;
							
					case KNOULINE4:		//収入金額　小計	
						for( j = KNOULINE1; j < KNOULINE4; j++ ){
							if(j == KNOULINE2K || i == KNOULINE3M){//家事消費,未成熟果樹収入はスキップ（入力不可の為）
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval2) ){
								st = 1;
								break;
							}
						}
						break;

					case KNOULINE31:	//経費　小計
						for( j = KNOULINE8; j < KNOULINE31; j++ ){
							if(j == KNOULINE13 || i == KNOULINE14 || i == KNOULINE15 || i == KNOULINE16 || i == KNOULINE18){//農具費、農薬・衛生費、諸材料費、修繕費、作業用衣料費はスキップ（入力不可の為）
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval2) ){
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

			case 2:		// 課税取引金額　C（A－B）
				switch( lin ){
					case KNOULINE3M:	//未成熟果樹収入
						st = 1;		
						break;

					case KNOULINE5:		//農産物の棚卸高　期首
					case KNOULINE6:		//農産物の棚卸高　期末
					case KNOULINE7:		//計
					case KNOULINE19:	//農業共済掛金
					case KNOULINE20:	//減価償却費
					case KNOULINE23:	//利子割引料
					case KNOULINE26:	//貸倒金
					case KNOULINE32:	//農産物以外の棚卸高　期首
					case KNOULINE33:	//農産物以外の棚卸高　期末
					case KNOULINE34:	//経費から差し引く果樹牛馬等の育成費用
					case KNOULINE35:	//計
					case KNOULINE36:	//差引金額
						//入力可所無し
						break;

					case KNOULINE4:		//収入金額　小計
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[lin].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[lin].KKval2) ){
							st = 1;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE3M].KKval3) ){
							st = 1;
						}
						break;
					default:
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[lin].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[lin].KKval2) ){
							st = 1;
						}
						break;
				}
				memcpy( val, pK->KKval3, 6 );
				break;

			case 3:	// うち税率４％適用分　D
				switch( lin ){
					case KNOULINE5:		//農産物の棚卸高　期首
					case KNOULINE6:		//農産物の棚卸高　期末
					case KNOULINE7:		//計
					case KNOULINE19:	//農業共済掛金
					case KNOULINE20:	//減価償却費
					case KNOULINE23:	//利子割引料
					case KNOULINE26:	//貸倒金
					case KNOULINE32:	//農産物以外の棚卸高　期首
					case KNOULINE33:	//農産物以外の棚卸高　期末
					case KNOULINE34:	//経費から差し引く果樹牛馬等の育成費用
					case KNOULINE35:	//計
					case KNOULINE36:	//差引金額
						//入力可所無し
						break;

					case KNOULINE4:		//収入金額　小計	
						for( j = KNOULINE1; j < KNOULINE4; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval4) ){
								st = 1;
								break;
							}
						}
						break;

					case KNOULINE31:	//経費　小計
						for( j = KNOULINE8; j < KNOULINE31; j++ ){
							if(j == KNOULINE19 || i == KNOULINE20 || i == KNOULINE23 || i == KNOULINE26){//農業共済掛金、減価償却費、利子割引料、貸倒金、はスキップ（入力不可の為）
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval4) ){
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


			case 4:	// うち税率６．３％適用分　E
				switch( lin ){
					case KNOULINE5:		//農産物の棚卸高　期首
					case KNOULINE6:		//農産物の棚卸高　期末
					case KNOULINE7:		//計
					case KNOULINE19:	//農業共済掛金
					case KNOULINE20:	//減価償却費
					case KNOULINE23:	//利子割引料
					case KNOULINE26:	//貸倒金
					case KNOULINE32:	//農産物以外の棚卸高　期首
					case KNOULINE33:	//農産物以外の棚卸高　期末
					case KNOULINE34:	//経費から差し引く果樹牛馬等の育成費用
					case KNOULINE35:	//計
					case KNOULINE36:	//差引金額
						//入力可所無し
						break;

					case KNOULINE4:		//収入金額　小計	
						for( j = KNOULINE1; j < KNOULINE4; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval5) ){
								st = 1;
								break;
							}
						}
						break;

					case KNOULINE31:	//経費　小計
						for( j = KNOULINE8; j < KNOULINE31; j++ ){
							if(j == KNOULINE19 || i == KNOULINE20 || i == KNOULINE23 || i == KNOULINE26){//農業共済掛金、減価償却費、利子割引料、貸倒金、はスキップ（入力不可の為）
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval5) ){
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

		}
		if( st ){
			memcpy( data.data_val, val, 6 );
			diag_setdata( IDC_ICSDIAG8CTRL1, idx+i, &data, CIcsdiagctrl );
		}
	}

	// 項目名
	char	buf[128];
	if( (dlin >= KNOULINE27) && (dlin <= KNOULINE29) ){
//	if( (lin >= KNOULINE27) && (lin <= KNOULINE29) ){
		lin = KNouDataPos[dlin + 1];
		pK = &(*m_pTaxListData)->NRec.KNval[lin];

		if( !(pK->KKsign&0x80) ){
			m_Util.DiagOcxIniz( &data );
			::ZeroMemory( buf, sizeof(buf));
			switch(dlin){
				case KNOULINE27:
					idx = KNOUKNAME27;
					break;

				case KNOULINE28:	
					idx = KNOUKNAME28;
					break;

				case KNOULINE29:	
					idx = KNOUKNAME29;
					break;
			}

			memcpy( buf, pK->KKnam, sizeof(pK->KKnam));
			diag_clear( IDC_ICSDIAG8CTRL1, idx, TRUE, CIcsdiagctrl );
			data.data_edit = buf;
			diag_setdata( IDC_ICSDIAG8CTRL1, idx, &data, CIcsdiagctrl );
		}
	}

	*/

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
int CShinKniinpDlg::GetDiagData( long index, short leng, DIAGRAM_DATA *data, int mode )
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
	clm_pos = (m_Idx - JSVMIN) % JSWIDECNT;

	//データ保存ポジションセット
	long	lin;
		//データ保存ポジションセット
	static long KniinpDataPos[] = {0 , 1 , 2 , 3 , 4 , 8 , 5 , 6 , 7};

	lin = KniinpDataPos[m_Lin];

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
			default:
				break;
		}


	}

	// 集計
	(*m_Scalc)->Shz_AllCalc();
	for( int i = 0 ; i < JSLINCNT ; i++ ){
		DispKniinpDiagData( i );
	}
	return 0;

	
}


BEGIN_EVENTSINK_MAP(CShinKniinpDlg, CSyzBaseDlg)
	ON_EVENT(CShinKniinpDlg, IDC_ICSDIAG8CTRL1, 1, CShinKniinpDlg::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinKniinpDlg, IDC_ICSDIAG8CTRL1, 2, CShinKniinpDlg::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinKniinpDlg, IDC_ICSDIAG8CTRL1, 3, CShinKniinpDlg::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinKniinpDlg::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_Kniinp.OffControl( FALSE );

	m_Idx = index;
	
	if( (m_Idx >= JSVMIN) && (m_Idx <= JSVMAX) ){
		m_Lin = (index - JSVMIN) / JSWIDECNT;
		m_Clm = (index - JSVMIN) % JSWIDECNT;
	}
//	else{
//		m_Lin = -1;
//		m_Clm = -1;
//	}

	m_curwnd = IDC_ICSDIAG8CTRL1;//'14.08.01
}

void CShinKniinpDlg::EditOFFIcsdiag8ctrl1(short index)
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

void CShinKniinpDlg::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
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
			m_Kniinp.SetPosition(pos);	
	//	}
	}
	if( wTerm==VK_DOWN ){
	//	if(index == KBUYVAL14){
	//		m_BuyKei.SetNextPosition();
	//	}
	//	else{
//			pos = UpDownFocusMoveCheck(index , 0);
			pos = FocusMoveCheck(index , 0);
			m_Kniinp.SetPosition(pos);
	//	}
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
//		m_Kniinp.SetNextPosition();
		pos = FocusMoveCheck(index , 2);
		m_Kniinp.SetPosition(pos);	
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
//		m_Kniinp.SetPrevPosition();
		pos = FocusMoveCheck(index , 3);
		m_Kniinp.SetPosition(pos);	
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

	SetRedrawScrollToIdx8(m_Kniinp.GetPosition());

/*	int nowpos = m_Kniinp.GetPosition();	// 移動先または自身のポジション

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
//int CShinKniinpDlg::UpDownFocusMoveCheck(int Index , int Move)
int CShinKniinpDlg::FocusMoveCheck(int Index , int Move)
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
		for(i = 0 ; i < FCS_SHINKNIINPCNT ; i++){
			if(Index == FCS_ShinKniinp[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinKniinp[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SHINKNIINPCNT ; i++){
			if(Index == FCS_ShinKniinp_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinKniinp_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SHINKNIINPCNT ; j++){
			m_Kniinp.GetAttribute(FCS_ShinKniinp[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinKniinp[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Kniinp.GetAttribute(FCS_ShinKniinp[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinKniinp[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SHINKNIINPCNT ; j++){
			m_Kniinp.GetAttribute(FCS_ShinKniinp_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinKniinp_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Kniinp.GetAttribute(FCS_ShinKniinp_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinKniinp_LR[j].IDNo;
				break;
			}
		}
	}

	return idx;
}
//<-----------------------------------------

void CShinKniinpDlg::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SHINKNIINPCNT; idx++ ){
		m_Kniinp.GetAttribute(FCS_ShinKniinp[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_ShinKniinp[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SHINKNIINPCNT; idx++ ){
		m_Kniinp.GetAttribute(FCS_ShinKniinp_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_ShinKniinp_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINKNIINPCNT-1; idx > -1 ; idx-- ){
		m_Kniinp.GetAttribute(FCS_ShinKniinp[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_ShinKniinp[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINKNIINPCNT-1; idx > -1 ; idx-- ){
		m_Kniinp.GetAttribute(FCS_ShinKniinp_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_ShinKniinp_LR[idx].IDNo;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// 初期ポジションセット'14.09.01
//-----------------------------------------------------------------------------
void CShinKniinpDlg::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if( (index = m_Kniinp.GetPosition()) != -1) {
			m_Kniinp.SetPosition(index);
			m_Kniinp.SetFocus();
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

	m_Kniinp.SetPosition(JSVAL01A);

	SetRedrawScroll( 0 );

}

int CShinKniinpDlg::DispJudge( int clm, int line )
{
	int st = 0;
	int j = 0;

	switch( clm ){
			case 0:	// 決算額
				switch( line ){
					case KNIINPLINE6:		//合計
						for( j = KNIINPLINE1 ; j < JSLINCNT ; j++ ){
							if(j == KNIINPLINE6){//合計行スキップ
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
					case KNIINPLINE6:		//合計
						for( j = KNIINPLINE1 ; j < JSLINCNT ; j++ ){
							if(j == KNIINPLINE6){//合計行スキップ
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
					case KNIINPLINE6:		//合計
						for( j = KNIINPLINE1 ; j < JSLINCNT ; j++ ){
							if(j == KNIINPLINE6){//合計行スキップ
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
						if(( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + line].KKval1) )||( m_pArith->l_test((*m_pTaxListData)->NRec.KUval[40 + line].KKval2))){
							st = 1;
						}
						break;
				}
				break;

			case 3:	// 返還等課税取引
				switch( line ){
					case KNIINPLINE6:		//合計
						for( j = KNIINPLINE1 ; j < JSLINCNT ; j++ ){
							if(j == KNIINPLINE6){//合計行スキップ
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
		}

	return st;
}


// 手入力サインのクリア
int CShinKniinpDlg::ClearManualInputSign()
{
	int rt = 0;

	for( int idx = 40; idx < 50; idx++ ){
		(*m_pTaxListData)->NRec.KUval[idx].KKauto = 0x00;
	}

	return rt;
}