// ShinNougyou.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinNougyou.h"
#include "ShinNougyouIdx.h"


// CShinNougyou ダイアログ

IMPLEMENT_DYNAMIC(CShinNougyou, CSyzBaseDlg)

CShinNougyou::CShinNougyou(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinNougyou::IDD, pParent)
	, m_pParent(pParent)//'14.08.06
	, m_Idx(0)
	, m_Lin(0)
	, m_Clm(0)
{
	m_curwnd = -1;//'14.08.06

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;
}

CShinNougyou::~CShinNougyou()
{
}

void CShinNougyou::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Nougyou);
}


BEGIN_MESSAGE_MAP(CShinNougyou, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinNougyou メッセージ ハンドラ

BOOL CShinNougyou::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Nougyou;
	m_BottomIdx = 52;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinNougyou::Init()
{
	InitDiagAttr();

	DispList(0);//'14.09.01

//2016.06.22 INSERT START
	SetFocusRange();
//2016.06.22 INSERT END

	SetItemPosition();	// フォーカスセット'14.09.01

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
void CShinNougyou::InitDiagAttr()
{
	DIAGRAM_ATTRIBUTE data;
	long dg1[] = { NOUKNAME27, NOUKNAME28, NOUKNAME29, -1 };

	for( int i=0;; i++ ){
		if( dg1[i] == -1 ){
			break;
		}
		// ank対応
		m_Nougyou.GetAttribute( (short)dg1[i], (LPUNKNOWN)&data );
		data.attr_editattr |= DIAG_EATTR_ZENHAN;//DIAG_EATTR_ZENHAN ank対応
		m_Nougyou.SetAttribute( (short)dg1[i], (LPUNKNOWN)&data, 1 );
	}

	int	cnt = 0;
	while( 1 ){
		if( Nougyou[cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Nougyou[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Nougyou[cnt].UnConnect;
			}
			else{
				Flg = Nougyou[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL1, Nougyou[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Nougyou[cnt].Connect );
		cnt++;
	}

	SetFocusRange();

	m_Nougyou.EnableDelete(0);//DELETEキー１回で削除モード設定
}

//'14.08.06
BOOL CShinNougyou::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			// 付表１更新
//			GetAllDiagData();
//			f1calq_all( 1, 0 );
//			WriteData();

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
					TerminationIcsdiagctrl1( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
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
void CShinNougyou::DispList( int mode )
{
	m_Nougyou.OffControl( TRUE );
//2016.06.22 INSERT START
	(*m_Scalc)->Shz_AllCalc(3);
//2016.06.22 INSERT END
	for( int i=0; i<VLIN; i++ ){
		DispNougyouDiagData( i );
	}

	DIAGRAM_DATA	data;
	if( !mode ){
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, YEARID, TRUE, CIcsdiagctrl );
		data.data_disp.Format( _T("（平成%2x年分）"), (*m_pTaxListData)->YEAR);
		diag_setdata( IDC_ICSDIAGCTRL1, YEARID, &data, CIcsdiagctrl );

		PostMessage(WM_VSCROLL, SB_TOP, 0 );
		m_Idx = VMIN;
	}

	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, ZKBNID, TRUE, CIcsdiagctrl );
	data.data_disp = _T("経理方式：");
	if( (*m_pTaxListData)->Head.KHstax&0x01 ){
		data.data_disp += _T("税抜き");
	}
	else{
		data.data_disp += _T("税込み");
	}
	diag_setdata( IDC_ICSDIAGCTRL1, ZKBNID, &data, CIcsdiagctrl );

	diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );

//2016.06.22 INSERT START
	int edit[] = {NOUKNAME27,NOUKNAME28,NOUKNAME29};
	BOOL edit_focus = FALSE;

	for( int cnt = 0; cnt < ARRAYCOUNT(edit); cnt++ ){
		Switch_KamokuEdit( IDC_ICSDIAGCTRL1, edit[cnt] );

		if( m_Nougyou.GetPosition() == edit[cnt] ){
			edit_focus = TRUE;
		}
	}

	if(( (*m_pTaxListData)->Head.KHkoke == 1 ) && ( edit_focus == TRUE )){
		this->SetFocus();
	}
//2016.06.22 INSERT END

	//SetRedrawScrollToIdx(m_Nougyou.GetPosition());
}

//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
// 引数	dlin	：	
//-----------------------------------------------------------------------------
void CShinNougyou::DispNougyouDiagData( long dlin )
{
	short	idx, i;
	long	lin;
	if( dlin < 6 ){
		lin = dlin;
	}
	else if( dlin == 6 ){
		lin = 38;
	}
	else{
		lin = dlin-1;
	}

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KNval[lin];
	char	val[6];
	DIAGRAM_DATA	data;
	int		st;

	idx = (short)(dlin*VCNT)+VMIN;
	for( i=0; i<VCNT; i++ ){
		st = 0;
		::ZeroMemory( val, sizeof(val));
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, idx+i, TRUE, CIcsdiagctrl );
/*		switch( i ){
			case 0:	// 決算額
				switch( lin ){
					case 4:
						break;
					case 37:
					case 8:
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[6].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[7].KKval1) ){
							st = 1;
						}
					case 5:	
						for( j=0; j<4; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
								st = 1;
								break;
							}
						}
						if( lin != 37 ){
							break;
						}
					case 36:
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[33].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[34].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[35].KKval1) ){
							st = 1;
						}
					case 32:
						for( j=9; j<32; j++ ){
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
			case 1:	// 課税取引にならないもの
				switch( lin ){
					case 1: case 4: case 6: case 7: case 8: case 14: case 15: case 16: case 17: case 19:
					case 33: case 34: case 35: case 36: case 37:
						break;
					case 5:	
						for( j=0; j<4; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval2) ){
								st = 1;
								break;
							}
						}
						break;
					case 32:
						for( j=9; j<32; j++ ){
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
			case 2:	// 課税取引金額
				switch( lin ){
					case 4:
						st = 1;		
						break;
					case 6: case 7: case 8: case 20: case 21: case 24: case 27:
					case 33: case 34: case 35: case 36: case 37:
						break;
					case 5:
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[lin].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[lin].KKval2) ){
							st = 1;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[4].KKval3) ){
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
		}*/
		if( DispJudge( i, lin ) == 1 ){
			st = 1;
			switch(i){
				case 0:
					memcpy( val, pK->KKval1, 6 );
					break;
				case 1:
					memcpy( val, pK->KKval2, 6 );
					break;
				case 2:
					memcpy( val, pK->KKval3, 6 );
					break;
				default:
					break;
			}
		}
		if( st ){
			memcpy( data.data_val, val, 6 );
			diag_setdata( IDC_ICSDIAGCTRL1, idx+i, &data, CIcsdiagctrl );
		}
	}

	// 項目名
	char	buf[128];
	if( (lin>=28) && (lin<=30) ){
		if( !(pK->KKsign&0x80) ){
			m_Util.DiagOcxIniz( &data );
			::ZeroMemory( buf, sizeof(buf));
//2016.06.22 UPDATE START
			//switch(lin){
			//	case 28:
			//		idx = NOUKNAME27;
			//		break;
			//	case 29:	
			//		idx = NOUKNAME28;
			//		break;
			//	case 30:	
			//		idx = NOUKNAME29;
			//		break;
			//}

			//memcpy( buf, pK->KKnam, sizeof(pK->KKnam));

			CString tmp;
			int prf_no = 0;
			switch(lin){
				case 28:
					idx = NOUKNAME27;
					//prf_no = 27;
					prf_no = 26;
					break;
				case 29:	
					idx = NOUKNAME28;
					prf_no = 28;
					break;
				case 30:	
					idx = NOUKNAME29;
					prf_no = 29;
					break;
			}

//-- '16.12.07 --
//			if(((*m_pTaxListData)->Head.KHkoke == 1 )&&( prf_no != 0 )){
//---------------
			if( (*m_pTaxListData)->IsLinkKjprfname() && (prf_no!=0) ){
//---------------
				if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)){ //金額が全て０なら表示しない
					(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmSub, ID_ICSSH_KJ_NOUGYOU, prf_no, tmp );
					memcpy( buf, tmp, tmp.GetLength() );
				}
			}else{
				memcpy( buf, pK->KKnam, sizeof(pK->KKnam));
			}

//2016.06.22 UPDATE END
			diag_clear( IDC_ICSDIAGCTRL1, idx, TRUE, CIcsdiagctrl );
			data.data_edit = buf;
			diag_setdata( IDC_ICSDIAGCTRL1, idx, &data, CIcsdiagctrl );
		}
	}
}

//-----------------------------------------------------------------------------
// 各入力データ抽出
//-----------------------------------------------------------------------------
// 引数
// 
// 
// 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CShinNougyou::GetDiagData( long index, short leng, DIAGRAM_DATA *data, int mode )
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

	long	lin;
	if( m_Lin < 6 ){
		lin = m_Lin;
	}
	else if( m_Lin == 6 ){
		lin = 38;
	}
	else{
		lin = m_Lin-1;
	}

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KNval[lin];
	if( !mode ){
		switch( m_Clm ){
			case 0:		// 決算額
				if( leng ){
					memcpy( pK->KKval1, data->data_val, 6 );
				}
				break;
			case 1:		// 課税取引にならないもの
				if( leng ){
					memcpy( pK->KKval2, data->data_val, 6 );
				}
				break;
			case 2:		// 課税取引金額
				if( leng ){
					memcpy( pK->KKval3, data->data_val, 6 );
				}
				break;
			default:	// 項目名
				memcpy( pK->KKnam, dbuf, sizeof(pK->KKnam));
				break;
		}
	}
	else{
		switch( m_Clm ){
			case 0:		// 決算額
				::ZeroMemory( pK->KKval1, 6 );
				break;
			case 1:		// 課税取引にならないもの
				::ZeroMemory( pK->KKval2, 6 );
				break;
			case 2:		// 課税取引金額
				::ZeroMemory( pK->KKval3, 6 );	
				break;
			default:	// 項目名
				::ZeroMemory( pK->KKnam, sizeof(pK->KKnam));
				break;
		}
	}

	// 集計
	(*m_Scalc)->Shz_AllCalc(3);//'14.08.06
	for( int i=0; i<VLIN; i++ ){
		DispNougyouDiagData( i );
	}
	return 0;
}

//-----------------------------------------------------------------------------
// カーソル移動
//-----------------------------------------------------------------------------
// 引数	index	：
//		flg		：
//		nChar	：
//-----------------------------------------------------------------------------
void CShinNougyou::Diag1_Edit( short index, UINT flg, short nChar )
{
	short	xpos = index;
	char	VK_FLG = 0x00;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );

	if( flg&REPLY_STAY ){
		diag_setposition( IDC_ICSDIAGCTRL1, xpos, CIcsdiagctrl );
		return;
	}

	if( nChar == VK_TAB ){
		if( VK_FLG & 0x80 ){
			nChar = VK_LEFT;
		}
		else{
			nChar = VK_RIGHT;
		}
	}

	switch( nChar ){
		case VK_RETURN:	case VK_RIGHT:
			if( m_Clm == 1 ){
				flg = REPLY_STAY;
				switch( m_Lin ){	// 項目名へ移動
					case 28:	
						xpos = NOUKNAME27;
						break;
					case 29:	
						xpos = NOUKNAME28;	
						break;
					case 30:
						xpos = NOUKNAME29;
						break;
					default:	
						flg = REPLY_NEXT;
						break;
				}
			}
			else if( m_Clm == -1 ){	// 項目名から移動
				flg = REPLY_STAY;
				xpos = (m_Lin*VCNT)+VMIN;
			}
			else{
				flg = REPLY_NEXT;
				if( m_Lin == 36 ){
					flg = REPLY_STAY;
					if( nChar == VK_RIGHT ){
						xpos = VMIN;
					}
				}
				else if( m_Lin == 28 ){
					flg = REPLY_STAY;	
					xpos = NOUKNAME27;
				}
			}
			break;
		case VK_LEFT:
			if( !m_Clm ){
				flg = REPLY_STAY;
				switch( m_Lin ){	// 項目名へ移動
					case 0:	
						xpos = VLST;	
						break;
					case 29:	
						xpos = NOUKNAME27;
						break;
					case 30:
						xpos = NOUKNAME28;
						break;
					case 31:	
						xpos = NOUKNAME29;
						break;
					default:	
						flg = REPLY_BACK;
						break;
				}
			}
			else if( m_Clm == -1 ){
				flg = REPLY_STAY;
				if( index == NOUKNAME27 ){
					xpos = ((m_Lin-1)*VCNT)+VMIN;
				}
				else{
					xpos = ((m_Lin-1)*VCNT)+VMIN+1;
				}
			}
			else{
				flg = REPLY_BACK;
			}
			break;
		case VK_UP:
			flg = REPLY_STAY;
			if( (index>=NOUKNAME27) && (index<=NOUKNAME29) ){	// 項目名
				if( index != NOUKNAME27 ){
					xpos -= 1;
				}
			}
			else{
				if( !m_Clm ){	// A
					switch( m_Lin ){
						case 0:	
							break;
						case 6: 	
							xpos -= 9;	
							break;
						case 10: 
						case 34:
							xpos -= 6;
							break;
						default:
							xpos -= 3;
							break;
					}
				}
				else if( m_Clm == 1 ){	// B
					switch( m_Lin ){
						case 0:
							break;
						case 2: case 26: case 29:	
							xpos -= 6;	
							break;
						case 6: 
							xpos -= 9;	
							break;
						case 10: 	
							xpos -= 12;	
							break;
						case 19: 	
							xpos -= 15;
							break;
						case 23:
							xpos -= 12;	
							break;
						default:	
							xpos -= 3;
							break;
					}
				}
			}
			break;
		case VK_DOWN:
			flg = REPLY_STAY;
			if( (index>=NOUKNAME27) && (index<=NOUKNAME29) ){	// 項目名
				if( index != NOUKNAME29 ){
					xpos += 1;
				}
			}
			else{
				if( !m_Clm ){	// A
					switch( m_Lin ){
						case 3: 	
							xpos += 9;	
							break;
						case 8: 
						case 32:
							xpos += 6;
							break;
						case 36:
							break;
						default:	
							xpos += 3;	
							break;
					}
				}
				else if( m_Clm == 1 ){	// B
					switch( m_Lin ){
						case 0: 
						case 24: 
						case 27:	
							xpos += 6;	
							break;
						case 3: 
							xpos += 9;
							break;
						case 6: 
							xpos += 12;
							break;
						case 14: 	
							xpos += 15;
							break;
						case 19:	
							xpos += 12;	
							break;
						case 32:
							break;
						default:
							xpos += 3;	
							break;
					}
				}
			}
			break;
	}
	if( flg & REPLY_NEXT ){
		diag_setnext( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
	}
	else if( flg & REPLY_BACK ){
		diag_setprev( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
	}
	else{
		diag_setposition( IDC_ICSDIAGCTRL1, xpos, CIcsdiagctrl );
	}
}
BEGIN_EVENTSINK_MAP(CShinNougyou, CSyzBaseDlg)
	ON_EVENT(CShinNougyou, IDC_ICSDIAGCTRL1, 1, CShinNougyou::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinNougyou, IDC_ICSDIAGCTRL1, 2, CShinNougyou::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinNougyou, IDC_ICSDIAGCTRL1, 3, CShinNougyou::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinNougyou::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//'14.08.06
	/*
	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL1, index, &rectB, CIcsdiagctrl);
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
	*/

	m_Nougyou.OffControl( FALSE );

	m_Idx = index;
	if( (m_Idx>=VMIN) && (m_Idx<=VMAX) ){
		m_Lin = (index-VMIN)/VCNT;
		m_Clm = (index-VMIN)%VCNT;
	}
	else if( (m_Idx>=NOUKNAME27) && (m_Idx<=NOUKNAME29) ){
		m_Lin = 29+(m_Idx-NOUKNAME27);
		m_Clm = -1;
	}

	m_curwnd = IDC_ICSDIAGCTRL1;//'14.08.01
}

void CShinNougyou::EditOFFIcsdiagctrl1(short index)
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

void CShinNougyou::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
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

	/*
	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;

	UINT	flg = 0;

	idata = (DIAGRAM_DATA *)data;
	m_Util.MoveDiagData( &xdata, idata );
	xdata.data_imgdata = NULL;
	*/

	//フォーカス移動処理対応'14.08.18
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
//		if(index == NOUVAL36C){
//			m_Nougyou.SetPrevPosition();
//		}
//		else{
//			pos = UpDownFocusMoveCheck(index , 1);
			pos = FocusMoveCheck(index , 1);
			m_Nougyou.SetPosition(pos);	
//		}
	}
	if( wTerm==VK_DOWN ){
//		if(index == NOUVAL36C){
//			m_Nougyou.SetNextPosition();
//		}
//		else{
//			pos = UpDownFocusMoveCheck(index , 0);
			pos = FocusMoveCheck(index , 0);
			m_Nougyou.SetPosition(pos);
//		}
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
//		m_Nougyou.SetNextPosition();
		pos = FocusMoveCheck(index , 2);
		m_Nougyou.SetPosition(pos);	
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
//		m_Nougyou.SetPrevPosition();
		pos = FocusMoveCheck(index , 3);
		m_Nougyou.SetPosition(pos);	
	}

	//------->DELETEキー対応'14.08.06
	if( wTerm == VK_DELETE){
		GetDiagData( index, length, &xdata, -1 );
	}


	/*
	switch( nChar ){
		case VK_RETURN:
			m_Nougyou.OffControl( TRUE );
			GetDiagData( index, length, &xdata, 0 );
			break;
		case VK_DELETE:
			GetDiagData( index, length, &xdata, -1 );
			flg = REPLY_STAY;
			break;
		case VK_F2	:
			Diag1_Edit( m_Idx, 0, VK_LEFT );
			return;
		case VK_F3  :
			Diag1_Edit( m_Idx, 0, VK_RIGHT );
			return;
		case VK_PRIOR:
			PostMessage( WM_VSCROLL, SB_TOP, 0 );
			return;
		case VK_NEXT:
			PostMessage( WM_VSCROLL, SB_BOTTOM, 0 );	
			return;
	}
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		Diag1_Edit( index, flg, nChar );
	}
	else{
		if( (nChar==VK_RETURN) || (nChar==VK_F3) ){
			m_Nougyou.SetNextPosition();
		}
		// 前項目へ
		if( (nChar==VK_TAB) || (nChar==VK_LEFT) || (nChar==VK_F2) ){
			m_Nougyou.SetPrevPosition();
		}
	}

	//------->DELETEキー対応'14.08.06
//	if( wTerm == VK_DELETE){
//		GetDiagData( index, length, &xdata, -1 );
//	}
*/

	SetRedrawScrollToIdx(m_Nougyou.GetPosition());

/*	int nowpos = m_Nougyou.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL1, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}*/

}

//'14.08.18------------------>
//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
//int CShinNougyou::UpDownFocusMoveCheck(int Index , int Move)
int CShinNougyou::FocusMoveCheck(int Index , int Move)
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
		for(i = 0 ; i < FCS_SHINNOUGYOUCNT ; i++){
			if(Index == FCS_ShinNougyou[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinNougyou[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SHINNOUGYOUCNT ; i++){
			if(Index == FCS_ShinNougyou_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinNougyou_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SHINNOUGYOUCNT ; j++){
			m_Nougyou.GetAttribute(FCS_ShinNougyou[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinNougyou[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Nougyou.GetAttribute(FCS_ShinNougyou[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinNougyou[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SHINNOUGYOUCNT ; j++){
			m_Nougyou.GetAttribute(FCS_ShinNougyou_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinNougyou_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Nougyou.GetAttribute(FCS_ShinNougyou_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinNougyou_LR[j].IDNo;
				break;
			}
		}
	}

	return idx;
}
//<-----------------------------------------

void CShinNougyou::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SHINNOUGYOUCNT; idx++ ){
		m_Nougyou.GetAttribute(FCS_ShinNougyou[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_ShinNougyou[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SHINNOUGYOUCNT; idx++ ){
		m_Nougyou.GetAttribute(FCS_ShinNougyou_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_ShinNougyou_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINNOUGYOUCNT-1; idx > -1 ; idx-- ){
		m_Nougyou.GetAttribute(FCS_ShinNougyou[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_ShinNougyou[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINNOUGYOUCNT-1; idx > -1 ; idx-- ){
		m_Nougyou.GetAttribute(FCS_ShinNougyou_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_ShinNougyou_LR[idx].IDNo;
			break;
		}
	}
}

#define		Msiz	512*20
//-----------------------------------------------------------------------------
// 印刷情報のセット
//-----------------------------------------------------------------------------
// 引数	pVprn	：	出力クラス
//		pPset	：	印刷情報
//-----------------------------------------------------------------------------
void CShinNougyou::SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo )
{
	ASSERT( pVprn );
	ASSERT( pPset );

	m_pVprn = pVprn;
	m_pPset = pPset;
	m_PrintCmInfo = pPrintCmInfo;

}

//-----------------------------------------------------------------------------
// 印刷１
//-----------------------------------------------------------------------------
// 引数	pno		：	出力指定表No.
//		fno		：	フィールドNo.
//		pDC		：	デバイスコンテキスト
//		pBp		：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinNougyou::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j, lin, max;
	char	cnm[128], buf[128], CVAL[6];
	int		len, st;
	KKVAL*	pK;
	KKVAL*	pJ = (*m_pTaxListData)->NRec.KNval;

	// 初期化
	memset( PF, '\0', sizeof(PF) );


	i = 0;
	memset( pBp, '\0', Msiz );
	char	*pBpMv = pBp;
//	pBpMv = BP;
	::ZeroMemory( CVAL, sizeof(CVAL) );

// 会社名
	::ZeroMemory( buf, sizeof(buf) );
	::ZeroMemory( cnm, sizeof(cnm) );

	if( m_pPset->psSigCoprn&0x2 ){
		// 顧問先
//		if( m_PrtSet.psSigCoprn&0x200 ) 
//		if( OPL ){
//		if( 0 ){
		if( m_PrintCmInfo->OPL ){
//			sprintf_s( buf, sizeof( buf ), _T(" %s"), CopNam );
		}
		else{
//			sprintf_s( buf, sizeof( buf ), _T("[%08d] %s"), CopNum, CopNam );
		}
	}
	else{
		// マスター
		if( (*m_pSnHeadData)->IsSoleProprietor() ){
			j = sizeof( (*m_pSnHeadData)->Sn_DIHYO );
			memmove( cnm, (*m_pSnHeadData)->Sn_DIHYO, j );
		}
		else {			// 法人名
			j = sizeof( (*m_pSnHeadData)->Sn_CONAM );
			memmove( cnm, (*m_pSnHeadData)->Sn_CONAM, j );
		}
//		if( m_PrtSet.psSigCoprn&0x200 ) 
//		if( OPL ){
//		if( 1 ){
		if( m_PrintCmInfo->OPL ){
			sprintf_s( buf, sizeof(buf), _T(" %s"),  cnm );
		}
		else{
			sprintf_s( buf, sizeof(buf), _T("[%08d] %s"), m_pZmSub->zvol->v_cod, cnm );
		}
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

// 年分
	::ZeroMemory( buf, sizeof(buf) );
	if( Is30PlusPrint() ){
		sprintf_s(buf, sizeof(buf), _T("%x"), (*m_pTaxListData)->YEAR+R_TO_H_ADNEN);
	}
	else{
		sprintf_s(buf, sizeof(buf), _T("%x"), (*m_pTaxListData)->YEAR);
	}
	memmove( pBpMv, buf, 2 );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( 2+1 );

// 決算額 A
// Aのうち課税取引にならないもの B
// 課税取引額 C
	lin = 0;	max = 3;
	pK = &(*m_pTaxListData)->NRec.KNval[0];
	for( j=0; j<max; j++ ){
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		switch( j ){
			case 0:
				m_pArith->l_print( buf, pK->KKval1, FMTX13 );
				break;
			case 1:
				m_pArith->l_print( buf, pK->KKval2, FMTX13 );
				break;
			case 2:
				if( l_test(pK->KKval1) || l_test(pK->KKval2) )
					m_pArith->l_print( buf, pK->KKval3, FMTX13 );
				else
					m_pArith->l_print( buf, pK->KKval3, FMTX130 );
				break;
		}*/
		if( DispJudge( j, 0 ) == 1 ){
			switch(j){
				case 0:
					m_pArith->l_print( buf, pK->KKval1, FMTX130 );
					break;
				case 1:
					m_pArith->l_print( buf, pK->KKval2, FMTX130 );
					break;
				case 2:
					m_pArith->l_print( buf, pK->KKval3, FMTX130 );
					break;
				default:
					break;
			}
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[0] = pBpMv;
		pBpMv += ( len+1 );
		i++;
	}

// 決算額 A
	lin = 1;	max = 5;
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
		if( (j+lin) != 4 ){
			if( DispJudge( 0, j+lin ) == 1 ){
				m_pArith->l_print( buf, pK->KKval1, FMTX130 );
			}
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	// 小計4
	pK = &(*m_pTaxListData)->NRec.KNval[5];
	::ZeroMemory( buf, sizeof( buf ));
	if( DispJudge( 0, KNOULINE4 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}

	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );

	i++;

// Aのうち課税取引にならないもの B
	for( j=0; (j+lin) < max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( (j+lin) != 1 || (j+lin) != 4 ){
			if( DispJudge( 1, j+lin ) == 1 ){
				m_pArith->l_print( buf, pK->KKval2, FMTX130 );
			}
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	// 小計4
	pK = &(*m_pTaxListData)->NRec.KNval[5];
	::ZeroMemory( buf, sizeof( buf ));
	if( DispJudge( 1, KNOULINE4 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval2, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );

	i++;

// 課税取引額 C
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( (j+lin) == 4 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX13 );
		}
		else{
			if( l_test(pK->KKval1) || l_test(pK->KKval2) ){
				m_pArith->l_print( buf, pK->KKval3, FMTX130 );
			}
		}*/
		if( DispJudge( 2, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
//	i++;

	// 小計4
	pK = &(*m_pTaxListData)->NRec.KNval[5];
	::ZeroMemory( buf, sizeof( buf ));
//	if( l_test(pK->KKval1) || l_test(pK->KKval2) || l_test((*m_pTaxListData)->NRec.KNval[4].KKval3) ){
//		l_print( buf, pK->KKval3, FMTX13 );
//	}
	if( DispJudge( 2, KNOULINE4 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval3, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );
	i++;
// 収入金額5-7
// 決算額 A
	lin = 6;	max = 8;
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 0, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
//計
	pK = &(*m_pTaxListData)->NRec.KNval[8];
	st = 0;
/*	for( int ii = 5; ii < 8; ii++ ){
		if( l_test( (*m_pTaxListData)->NRec.KNval[ii].KKval1 )){
			st = 1;		break;
		}
	}
	m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );
	*/
	if( DispJudge( 0, KNOULINE7 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );

	return _xprn( pno, fno, PF, 0, pDC, 0, 1 );
}

//-----------------------------------------------------------------------------
// 印刷２
//-----------------------------------------------------------------------------
// 引数
// 
// 
// 
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int CShinNougyou::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j , len , lin , max;
	char	buf[64];
	KKVAL*	pK;

	i = 0;
	memset( pBp, '\0', Msiz );
//	BP1 = BP;	
	char	*pBpMv = pBp;

// 経費(8-17)
	lin = 9;	max = 19;
// 決算額 A
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 0, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

// Aのうち課税取引にならないもの B
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( (j+lin) != 14 || (j+lin) != 15 || (j+lin) != 16 || (j+lin) != 17 ){
			if( DispJudge( 1, j+lin ) == 1 ){
				m_pArith->l_print( buf, pK->KKval2, FMTX130 );
			}
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

// 課税取引額 C
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
/*		if( l_test(pK->KKval1) || l_test(pK->KKval2) ){
			m_pArith->l_print( buf, pK->KKval3, FMTX13 );
		}*/
		if( DispJudge( 2, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

// 経費(18-26)
	lin = 19;	max = 28;
// 決算額 A
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 0, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;
// Aのうち課税取引にならないもの B
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( (j+lin) != 19 ){
			if( DispJudge( 1, j+lin ) == 1 ){
				m_pArith->l_print( buf, pK->KKval2, FMTX130 );
			}
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;
// 課税取引額 C
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( (j+lin) != 20 || (j+lin) != 21 || (j+lin) != 24 || (j+lin) != 27 ){
//			if( l_test(pK->KKval1) || l_test(pK->KKval2) ){
//				m_pArith->l_print( buf, pK->KKval3, FMTX13 );
//			}
			if( DispJudge( 2, j+lin ) == 1 ){
				m_pArith->l_print( buf, pK->KKval3, FMTX130 );
			}
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}

	return( _xprn( pno, fno, PF, 0, pDC, 0 ));
}

//-----------------------------------------------------------------------------
// 印刷３
//-----------------------------------------------------------------------------
// 引数
// 
// 
// 
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int CShinNougyou::PrintHyo3( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j , len , lin , max;
	char	buf[64];
	KKVAL*	pK;

	i = 0;
	memset( pBp, '\0', Msiz );
//	BP1 = BP;	
	char	*pBpMv = pBp;

// 経費(27-31)
	lin = 28;	max = 32;
// 決算額 A
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 0, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	// 計31
//	pK = &(*m_pTaxListData)->NRec.KJval[32];
	pK = &(*m_pTaxListData)->NRec.KNval[KNOULINE31];
	::ZeroMemory( buf, sizeof(buf) );
	if( DispJudge( 0, KNOULINE31 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );
	i++;

// Aのうち課税取引にならないもの B
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 1, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	// 計31
//	pK = &(*m_pTaxListData)->NRec.KJval[32];
	pK = &(*m_pTaxListData)->NRec.KNval[KNOULINE31];
	::ZeroMemory( buf, sizeof(buf) );
	if( DispJudge( 1, KNOULINE31 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval2, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );
	i++;

// 課税取引額 C
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
//		if( l_test(pK->KKval1) || l_test(pK->KKval2) ){
//			m_pArith->l_print( buf, pK->KKval3, FMTX13 );
//		}
		if( DispJudge( 2, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	// 計31
//	pK = &(*m_pTaxListData)->NRec.KJval[32];
	pK = &(*m_pTaxListData)->NRec.KNval[KNOULINE31];
	::ZeroMemory( buf, sizeof(buf) );
//	if( l_test(pK->KKval1) || l_test(pK->KKval2) ){
//		m_pArith->l_print( buf, pK->KKval3, FMTX13 );
//	}
	if( DispJudge( 2, KNOULINE31 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval3, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );
	i++;


// 経費 32-33
	lin = 33;	max = 35;
// 決算額 A
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 0, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;
// 経費 34
//	pK = &(*m_pTaxListData)->NRec.KJval[35];
//	pK = &(*m_pTaxListData)->NRec.KNval[35];
	pK = &(*m_pTaxListData)->NRec.KNval[KNOULINE34];
	::ZeroMemory( buf, sizeof(buf) );
	if( DispJudge( 0, KNOULINE34 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[0] = pBpMv;
	pBpMv += ( len+1 );
	i++;

// 経費 計35
//	pK = &(*m_pTaxListData)->NRec.KNval[36];
	pK = &(*m_pTaxListData)->NRec.KNval[KNOULINE35];
	::ZeroMemory( buf, sizeof(buf) );
	int st = 0;
//	for( j = 9; j < 36; j++ ){
//		if( l_test( (*m_pTaxListData)->NRec.KNval )){
//			st = 1;		break;
//		}
//	}
//	m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );

	if( DispJudge( 0, KNOULINE35 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );		
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

// 経費 計36
//	pK = &(*m_pTaxListData)->NRec.KNval[36];
	pK = &(*m_pTaxListData)->NRec.KNval[KNOULINE36];
	::ZeroMemory( buf, sizeof(buf) );
//	for( j = 0, st = 0; j < 36; j++ ){
//		if( l_test( (*m_pTaxListData)->NRec.KNval )){
//			st = 1;		break;
//		}
//	}
//	m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );
	if( DispJudge( 0, KNOULINE36 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );		
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[1] = pBpMv;
	pBpMv += ( len+1 );	
	i++;

//--> '16.12.07 INS START
	CString	tmpStr;
	bool isLinkKjprf = (*m_pTaxListData)->IsLinkKjprfname();
//<-- '16.12.07 INS END

// 経費 項目名 27-29
	lin = 28;	max = 31;
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
//-- '16.12.07 --
//		len = (int)strlen( pK->KKnam );
//		memmove( pBpMv, pK->KKnam , len );
//---------------
		if( isLinkKjprf ){
			tmpStr.Empty();
			if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)){ //金額が全て０なら表示しない
//2016.06.22 UPDATE START
//				(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmSub, ID_ICSSH_KJ_NOUGYOU, ((j+lin)+1), tmpStr );
				if( j == 0 ){
					(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmSub, ID_ICSSH_KJ_NOUGYOU, (((j-3)+lin)+1), tmpStr ); //既に記憶されてる名称がずれるため応急処置
				}else{
					(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmSub, ID_ICSSH_KJ_NOUGYOU, (((j-2)+lin)+1), tmpStr ); //既に記憶されてる名称がずれるため応急処置
				}
//2016.06.22 UPDATE END
			}
			len = tmpStr.GetLength();
			memmove( pBpMv, tmpStr, len );
		}
		else{
			len = (int)strlen( pK->KKnam );
			memmove( pBpMv, pK->KKnam , len );
		}
//---------------
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}

	return( _xprn( pno, fno, PF, 0, pDC, 0 ));
}


//-----------------------------------------------------------------------------
// 初期ポジションセット '14.09.01
//-----------------------------------------------------------------------------
void CShinNougyou::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
		if( (index = m_Nougyou.GetPosition()) != -1) {
			m_Nougyou.SetPosition(index);
			m_Nougyou.SetFocus();
			return;
		}
	}

	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Nougyou.SetPosition(NOUVAL01A);
	}
	else{
		m_Nougyou.SetPosition(NOUKNAME27);
	}
	SetRedrawScroll( 0 );

}

int CShinNougyou::DispJudge( int clm, int line )
{
	int st = 0;
	int j = 0;

	switch( clm ){
		case 0:	// 決算額
			switch( line ){
				case 4:
					break;
				case 37:
				case 8:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[6].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[7].KKval1) ){
						st = 1;
					}
				case 5:	
					for( j=0; j<4; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
							st = 1;
							break;
						}
					}
					if( line != 37 ){
						break;
					}
				case 36:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[33].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[34].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[35].KKval1) ){
						st = 1;
					}
				case 32:
					for( j=9; j<32; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
							st = 1;
							break;
						}
					}
					break;
				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[line].KKval1) ){
						st = 1;
					}
					break;
			}
			break;
		case 1:	// 課税取引にならないもの
			switch( line ){
				case 1: case 4: case 6: case 7: case 8: case 14: case 15: case 16: case 17: case 19:
				case 33: case 34: case 35: case 36: case 37:
					break;
				case 5:	
					for( j=0; j<4; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval2) ){
							st = 1;
							break;
						}
					}
					break;
				case 32:
					for( j=9; j<32; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval2) ){
							st = 1;
							break;
						}
					}
					break;
				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[line].KKval2)){
						st = 1;
					}
					break;
			}
			break;
		case 2:	// 課税取引金額
			switch( line ){
				case 4:
//					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[line].KKval1)){
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[line].KKval3)){
						st = 1;
					}
					break;
				case 6: case 7: case 8: case 20: case 21: case 24: case 27:
				case 33: case 34: case 35: case 36: case 37:
					break;
				case 5:
//					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[line].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[line].KKval2) ){
//						st = 1;
//					}
					for( j=0; j<4; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
							st = 1;
							break;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval2) ){
							st = 1;
							break;
						}
					}
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[4].KKval3) ){
						st = 1;
					}
					break;
				case 32:
					for( j=9; j<32; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
							st = 1;
							break;
						}
						if( st != 0 ){
							break;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval2) ){
							st = 1;
							break;
						}
					}

					break;
				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[line].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[line].KKval2) ){
						st = 1;
					}
					break;
			}
			break;
	}	

	return st;
}

// 手入力サインのクリア
int CShinNougyou::ClearManualInputSign()
{
	int rt = 0;

	for( int idx = 0; idx < 50; idx++ ){
		(*m_pTaxListData)->NRec.KNval[idx].KKauto = 0x00;
	}

	return rt;
}