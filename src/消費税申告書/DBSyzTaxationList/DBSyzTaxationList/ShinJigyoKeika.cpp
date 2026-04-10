// ShinJigyoKeika.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinJigyoKeika.h"
#include "ShinJigyoKeikaIdx.h"


// CShinJigyoKeika ダイアログ

IMPLEMENT_DYNAMIC(CShinJigyoKeika, CSyzBaseDlg)

CShinJigyoKeika::CShinJigyoKeika(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinJigyoKeika::IDD, pParent)
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

CShinJigyoKeika::~CShinJigyoKeika()
{
}

void CShinJigyoKeika::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_JigyoKei);
}


BEGIN_MESSAGE_MAP(CShinJigyoKeika, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinJigyoKeika メッセージ ハンドラ

BOOL CShinJigyoKeika::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_JigyoKei;
	m_BottomIdx = 51;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinJigyoKeika::Init()
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
void CShinJigyoKeika::InitDiagAttr()
{

	
	DIAGRAM_ATTRIBUTE data;
	long dg1[] = { KJIGKNAME25, KJIGKNAME26, KJIGKNAME27, KJIGKNAME28, KJIGKNAME29, KJIGKNAME30, -1 };

	for( int i=0;; i++ ){
		if( dg1[i] == -1 ){
			break;
		}
		// ank対応
		m_JigyoKei.GetAttribute( (short)dg1[i], (LPUNKNOWN)&data );
		data.attr_editattr |= DIAG_EATTR_ZENHAN;			//DIAG_EATTR_ZENHAN ank対応
		m_JigyoKei.SetAttribute( (short)dg1[i], (LPUNKNOWN)&data, 1 );
	}

	int	cnt = 0;
	while( 1 ){
		if( JigyouKeika[cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = JigyouKeika[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = JigyouKeika[cnt].UnConnect;
			}
			else{
				Flg = JigyouKeika[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL1, JigyouKeika[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, JigyouKeika[cnt].Connect );
		cnt++;
	}

	SetFocusRange();

	m_JigyoKei.EnableDelete(0);//DELETEキー１回で削除モード設定'14.08.06

	
}


BOOL CShinJigyoKeika::PreTranslateMessage(MSG* pMsg)
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
void CShinJigyoKeika::DispList( int mode )
{
	m_JigyoKei.OffControl( TRUE );
//2016.06.22 INSERT START
	(*m_Scalc)->Shz_KeiAllCalc(1);
//2016.06.22 INSERT END
	short	i;
	for( i = 0; i < JKLINCNT; i++ ){
		DispJigyoKeikaDiagData( i );
	}


	DIAGRAM_DATA	data;
	if( !mode ){
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, JKYEARID, TRUE, CIcsdiagctrl );
		data.data_disp.Format( _T("（平成%2x年分）"), (*m_pTaxListData)->YEAR );
		diag_setdata( IDC_ICSDIAGCTRL1, JKYEARID, &data, CIcsdiagctrl );

		PostMessage(WM_VSCROLL, SB_TOP, 0 );
		m_Idx = JKVMIN;
	}

	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, JKZKBNID, TRUE, CIcsdiagctrl );
	data.data_disp = _T("経理方式：");
	if( (*m_pTaxListData)->Head.KHstax&0x01 ){
		data.data_disp += _T("税抜き");
	}
	else{
		data.data_disp += _T("税込み");
	}
	diag_setdata( IDC_ICSDIAGCTRL1, JKZKBNID, &data, CIcsdiagctrl );

	diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );

//2016.06.22 INSERT START
	int edit[] = {KJIGKNAME25,KJIGKNAME26,KJIGKNAME27,KJIGKNAME28,KJIGKNAME29,KJIGKNAME30};
	BOOL edit_focus = FALSE;

	for( int cnt = 0; cnt < ARRAYCOUNT(edit); cnt++ ){
		Switch_KamokuEdit( IDC_ICSDIAGCTRL1, edit[cnt] );

		if( m_JigyoKei.GetPosition() == edit[cnt] ){
			edit_focus = TRUE;
		}
	}

	if(( (*m_pTaxListData)->Head.KHkoke == 1 ) && ( edit_focus == TRUE )){
		this->SetFocus();
	}
//2016.06.22 INSERT END

	//SetRedrawScrollToIdx(m_JigyoKei.GetPosition());
}

//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
// 引数	dlin	：
//-----------------------------------------------------------------------------
void CShinJigyoKeika::DispJigyoKeikaDiagData( long dlin )
{
	//'14.08.21
	short	idx, i;
	long	lin;

	//データ保存ポジションセット
	static long KJigyoDataPos[] = {0 , 33 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 , 
								 17 , 18 , 19 , 20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 , 28 , 29 , 30 , 31 , 32};

	lin = KJigyoDataPos[dlin];

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KJval[lin];
	char	val[6];
	DIAGRAM_DATA	data;
	int		st;

	idx = (short)(dlin * JKWIDECNT) + JKVMIN;

	for( i = 0 ; i < JKWIDECNT ; i++ ){
		st = 0;
		::ZeroMemory( val, sizeof(val));
		diag_clear( IDC_ICSDIAGCTRL1, idx+i, TRUE, CIcsdiagctrl );
		m_Util.DiagOcxIniz( &data );
		switch( i ){
			case 0:	// 決算額　A
/*				switch( lin ){
					case KJIGLINE33:	//差引税額（下）
//						break;

					case KJIGLINE7:		//差引税額（上）
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[KJIGLINE1].KKval1) ){
							st = 1;
						}

					case KJIGLINE6:		//売上原価　差引原価
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[KJIGLINE5].KKval1) ){
							st = 1;
						}

					case KJIGLINE4:		//売上原価　小計
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[KJIGLINE2].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval1) ){
							st = 1;
						}
						if( lin != KJIGLINE33 ){
							break;
						}

					case KJIGLINE32:	//経費	計
						for( j = KJIGLINE8 ; j < KJIGLINE32 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[j].KKval1) ){
								st = 1;
								break;
							}
						}
						break;

					default:
						st = 1;
						break;
				}*/
				memcpy( val, pK->KKval1, 6 );
				break;

			case 1:	// Aのうち課税取引にならないもの　B
/*				switch( lin ){
					case KJIGLINE2:	 //期首商品棚卸高
					case KJIGLINE4:	 //小計
					case KJIGLINE5:	 //期末商品棚卸高
					case KJIGLINE6:	 //差引原価
					case KJIGLINE7:	 //差引金額
					case KJIGLINE10: //水道光熱費
					case KJIGLINE16: //修繕費
					case KJIGLINE17: //消耗品費
					case KJIGLINE33: //差引金額
						//入力不可の為処理しない
						break;

					case KJIGLINE32: //経費　計
						for( j = KJIGLINE8 ; j < KJIGLINE32 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[j].KKval2) ){
								st = 1;	
								break;
							}
						}
						break;

					default:
						st = 1;
						break;
				}*/
				memcpy( val, pK->KKval2, 6 );
				break;

			case 2:	// 課税取引金額 C(AーB)
/*				switch( lin ){
					case KJIGLINE2:	 //期首商品棚卸高
					case KJIGLINE4:	 //小計
					case KJIGLINE5:	 //期末商品棚卸高
					case KJIGLINE6:	 //差引原価
					case KJIGLINE7:	 //差引金額
					case KJIGLINE15: //損害保険料
					case KJIGLINE18: //減価償却費
					case KJIGLINE22: //利子割引料
					case KJIGLINE24: //貸倒金
					case KJIGLINE33: //差引金額
						//入力不可の為処理しない
						break;
					case KJIGLINE32: //経費	計
						for( j = KJIGLINE8 ; j < KJIGLINE32 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[j].KKval1) ){
								st = 1;
								break;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[j].KKval2) ){
								st = 1;
								break;
							}
						}

						break;

					default:
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[lin].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KJval[lin].KKval2) ){
							st = 1;
						}
						break;
				}*/
				memcpy( val, pK->KKval3, 6 );
				break;

			case 3: // うち税率４％適用分　D
/*				switch( lin ){
					case KJIGLINE2:	 //期首商品棚卸高
					case KJIGLINE4:	 //小計
					case KJIGLINE5:	 //期末商品棚卸高
					case KJIGLINE6:	 //差引原価
					case KJIGLINE7:	 //差引金額
					case KJIGLINE15: //損害保険料
					case KJIGLINE18: //減価償却費
					case KJIGLINE22: //利子割引料
					case KJIGLINE24: //貸倒金
					case KJIGLINE33: //差引金額
						//入力不可の為処理しない
						break;

					case KJIGLINE32:	//経費	計
						for( j = KJIGLINE8 ; j < KJIGLINE32 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[j].KKval4) ){
								st = 1;
								break;
							}
						}
						break;

					default:
						st = 1;
						break;
				}*/
				memcpy( val, pK->KKval4, 6 );
				break;

			case 4: // うち税率６．３％適用分　D
/*				switch( lin ){
					case KJIGLINE2:	 //期首商品棚卸高
					case KJIGLINE4:	 //小計
					case KJIGLINE5:	 //期末商品棚卸高
					case KJIGLINE6:	 //差引原価
					case KJIGLINE7:	 //差引金額
					case KJIGLINE15: //損害保険料
					case KJIGLINE18: //減価償却費
					case KJIGLINE22: //利子割引料
					case KJIGLINE24: //貸倒金
					case KJIGLINE33: //差引金額
						//入力不可の為処理しない
						break;

					case KJIGLINE32:	//経費	計
						for( j = KJIGLINE8 ; j < KJIGLINE32 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[j].KKval5) ){
								st = 1;
								break;
							}
						}
						break;

					default:
						st = 1;
						break;
				}*/
				memcpy( val, pK->KKval5, 6 );
				break;

		}
//		if( st ){
		if( DispJudge( i, lin ) == 1 ){
			memcpy( data.data_val, val, 6 );
			diag_setdata( IDC_ICSDIAGCTRL1, idx+i, &data, CIcsdiagctrl );
		}
	}

	// 項目名
	char	buf[128];
	if(dlin >= KJIGLINE25 && dlin <= KJIGLINE30){//項目名入力可所　25 26 27 28 29　の場合
		lin = KJigyoDataPos[dlin + 1];
		pK = &(*m_pTaxListData)->NRec.KJval[lin];

		if( !(pK->KKsign&0x80) ){
			m_Util.DiagOcxIniz( &data );
			::ZeroMemory( buf, sizeof(buf));
//2016.06.22 UPDATE START
			//switch(dlin){
			//	case KJIGLINE25:
			//		idx = KJIGKNAME25;
			//		break;
			//	case KJIGLINE26:
			//		idx = KJIGKNAME26;
			//		break;
			//	case KJIGLINE27:
			//		idx = KJIGKNAME27;
			//		break;
			//	case KJIGLINE28:
			//		idx = KJIGKNAME28;
			//		break;
			//	case KJIGLINE29:
			//		idx = KJIGKNAME29;	
			//		break;
			//	case KJIGLINE30:
			//		idx = KJIGKNAME30;
			//		break;
			//}
			//memcpy( buf, pK->KKnam, sizeof(pK->KKnam));

			CString tmp;
			int prf_no = 0;
			switch(dlin){
				case KJIGLINE25:
					idx = KJIGKNAME25;
					prf_no = 25;
					break;
				case KJIGLINE26:
					idx = KJIGKNAME26;
					prf_no = 26;
					break;
				case KJIGLINE27:
					idx = KJIGKNAME27;
					prf_no = 27;
					break;
				case KJIGLINE28:
					idx = KJIGKNAME28;
					prf_no = 28;
					break;
				case KJIGLINE29:
					idx = KJIGKNAME29;	
					prf_no = 29;
					break;
				case KJIGLINE30:
					idx = KJIGKNAME30;
					prf_no = 30;
					break;
			}

//-- '16.12.07 --
//			if(((*m_pTaxListData)->Head.KHkoke == 1 )&&( prf_no != 0 )){
//---------------
			if( (*m_pTaxListData)->IsLinkKjprfname() && (prf_no!=0) ){
//---------------
				if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)||(m_pArith->l_test(pK->KKval4)!=0)||(m_pArith->l_test(pK->KKval5)!=0)){ //金額が全て０なら表示しない
					(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmSub, ID_ICSSH_KJ_IPPAN, prf_no, tmp );
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

	/*
	short	idx, i, j;
	long	lin;
	if( !dlin ){
		lin = dlin;
	}
	else if( dlin == 1 ){
		lin = 33;
	}
	else{
		lin = dlin-1;
	}

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KJval[lin];
	char	val[6];
	DIAGRAM_DATA	data;
	int		st;


	idx = (short)(dlin * JKVCNT) + JKVMIN;
	for( i = 0 ; i < JKVCNT ; i++ ){
		st = 0;
		::ZeroMemory( val, sizeof(val));
		diag_clear( IDC_ICSDIAGCTRL1, idx+i, TRUE, CIcsdiagctrl );
		m_Util.DiagOcxIniz( &data );
		switch( i ){
			case 0:	// 決算額
				switch( lin ){
					case 32:
					case 6:	
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[0].KKval1) ){
							st = 1;
						}
					case 5:
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[4].KKval1) ){
							st = 1;
						}
					case 3:
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[1].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KJval[2].KKval1) ){
							st = 1;
						}
						if( lin !=32 ){
							break;
						}
					case 31:
						for( j=7; j<31; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[j].KKval1) ){
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
					case 1: case 3: case 4: case 5: case 6: case 9: case 15: case 16: case 32:
						break;
					case 31:
						for( j=7; j<31; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[j].KKval2) ){
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
					case 1: case 3: case 4: case 5: case 6: case 14: case 17: case 21: case 23: case 32:
						break;
					default:
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[lin].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KJval[lin].KKval2) ){
							st = 1;
						}
						break;
				}
				memcpy( val, pK->KKval3, 6 );
				break;
		}
		if( st ){
			memcpy( data.data_val, val, 6 );
			diag_setdata( IDC_ICSDIAGCTRL1, idx+i, &data, CIcsdiagctrl );
		}
	}

	// 項目名
	char	buf[128];
	if( ((lin+1)>=25) && ((lin+1)<=30) ){
		if( !(pK->KKsign&0x80) ){
			m_Util.DiagOcxIniz( &data );
			::ZeroMemory( buf, sizeof(buf));
			switch(lin+1){
				case 25:
					idx = KJIGKNAME25;
					break;
				case 26:
					idx = KJIGKNAME26;
					break;
				case 27:
					idx = KJIGKNAME27;
					break;
				case 28:
					idx = KJIGKNAME28;
					break;
				case 29:
					idx = KJIGKNAME29;	
					break;
				case 30:
					idx = KJIGKNAME30;
					break;
			}
			memcpy( buf, pK->KKnam, sizeof(pK->KKnam));
			diag_clear( IDC_ICSDIAGCTRL1, idx, TRUE, CIcsdiagctrl );
			data.data_edit = buf;
			diag_setdata( IDC_ICSDIAGCTRL1, idx, &data, CIcsdiagctrl );
		}
	}

	*/

}

//-----------------------------------------------------------------------------
// 各入力データ抽出
//-----------------------------------------------------------------------------
// 引数	index	：
//		leng	：
//		pData	：
//		modde	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CShinJigyoKeika::GetDiagData( long index, short leng, DIAGRAM_DATA *pData, int mode )
{

	//'14.08.21
	if( index < 0 ){
		return 0;
	}

	char	dbuf[128];
	char	*buff;
	int		len;

	
	len = pData->data_edit.GetLength();
	memset( dbuf, '\0', sizeof(dbuf) );
	buff = (char *)((LPCTSTR)(pData->data_edit));
	memmove( dbuf, buff, len );	


	//データ保存ポジションセット
	long	lin;
		//データ保存ポジションセット
	static long KJigyoDataPos[] = {0 , 33 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 , 
								  17 , 18 , 19 , 20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 , 28 , 29 , 30 , 31 , 32};

	lin = KJigyoDataPos[m_Lin];

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KJval[lin];

	if( !mode ){
		switch( m_Clm ){
			case 0:		// 決算額	A
				if( leng ){
					memcpy( pK->KKval1, pData->data_val, 6 );
				}
				break;
			case 1:		// Aのうち課税取引にならないもの	B
				if( leng ){
					memcpy( pK->KKval2, pData->data_val, 6 );
				}
				break;
			case 2:		// 課税取引金額　C（A－B）
				if( leng ){
					memcpy( pK->KKval3, pData->data_val, 6 );
				}
				break;

			case 3:		// うち税率４％適用分　D
				if( leng ){
					memcpy( pK->KKval4, pData->data_val, 6 );
				}
				break;

			case 4:		// うち税率６．３％適用分　E
				if( leng ){
					memcpy( pK->KKval5, pData->data_val, 6 );
				}
				break;

			default:	// 項目名
				memcpy( pK->KKnam, dbuf, sizeof(pK->KKnam));
				break;
		}
	}
	else{
		switch( m_Clm ){
			case 0:		// 決算額	A
				::ZeroMemory( pK->KKval1, 6 );
				break;
			case 1:		// Aのうち課税取引にならないもの	B
				::ZeroMemory( pK->KKval2, 6 );
				break;
			case 2:		// 課税取引金額　C（A－B）
				::ZeroMemory( pK->KKval3, 6 );	
				break;
			case 3:		// うち税率４％適用分　D
				::ZeroMemory( pK->KKval4, 6 );	
				break;
			case 4:		// うち税率６．３％適用分　E
				::ZeroMemory( pK->KKval5, 6 );	
				break;
			default:	// 項目名
				::ZeroMemory( pK->KKnam, sizeof(pK->KKnam));
				break;
		}
	}

	// 集計
	(*m_Scalc)->Shz_KeiAllCalc(1);//'14.08.21
	//m_Scalc->Shz_AllCalc(1);//'14.08.06
	for( int i = 0; i < JKLINCNT; i++ ){
		DispJigyoKeikaDiagData( i );
	}

	return 0;

	

	/*
	if( index < 0 ){
		return 0;
	}

	char	dbuf[128];
	char	*buff;
	int		len;

	len = pData->data_edit.GetLength();
	memset( dbuf, '\0', sizeof(dbuf) );
	buff = (char *)((LPCTSTR)(pData->data_edit));
	memmove( dbuf, buff, len );

	long	lin;
	if( !m_Lin ){
		lin = m_Lin;
	}
	else if( m_Lin == 1 ){
		lin = 33;
	}
	else{
		lin = m_Lin-1;
	}

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KJval[lin];
	if( !mode ){
		switch( m_Clm ){
			case 0:		// 決算額
				if( leng ){
					memcpy( pK->KKval1, pData->data_val, 6 );
				}
				break;
			case 1:		// 課税取引にならないもの
				if( leng ){
					memcpy( pK->KKval2, pData->data_val, 6 );
				}
				break;
			case 2:		// 課税取引金額
				if( leng ){
					memcpy( pK->KKval3, pData->data_val, 6 );
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
	m_Scalc->Shz_AllCalc(1);//'14.08.06
	for( int i = 0; i < JKLINCNT; i++ ){
		DispJigyoKeikaDiagData( i );
	}
	return 0;
	*/
}



BEGIN_EVENTSINK_MAP(CShinJigyoKeika, CSyzBaseDlg)
	ON_EVENT(CShinJigyoKeika, IDC_ICSDIAGCTRL1, 1, CShinJigyoKeika::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinJigyoKeika, IDC_ICSDIAGCTRL1, 2, CShinJigyoKeika::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinJigyoKeika, IDC_ICSDIAGCTRL1, 3, CShinJigyoKeika::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinJigyoKeika::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_JigyoKei.OffControl( FALSE );

	m_Idx = index;
	if( (m_Idx >= JKVMIN) && (m_Idx <= JKVMAX) ){
		m_Lin = (index - JKVMIN) / JKWIDECNT;
		m_Clm = (index - JKVMIN) % JKWIDECNT;
	}
	else if( m_Idx == KJIGKNAME25 ){
		m_Lin = KJIGLINE25 + 1;//変換等対価の行分＋１する
//		m_Lin = KJIGLINE25;
		m_Clm = -1;
	}
	else if( m_Idx == KJIGKNAME26 ){
		m_Lin = KJIGLINE26 + 1;//変換等対価の行分＋１する
//		m_Lin = KJIGLINE26;
		m_Clm = -1;
	}
	else if( m_Idx == KJIGKNAME27 ){
		m_Lin = KJIGLINE27 + 1;//変換等対価の行分＋１する
//		m_Lin = KJIGLINE27;
		m_Clm = -1;
	}
	else if( m_Idx == KJIGKNAME28 ){
		m_Lin = KJIGLINE28 + 1;//変換等対価の行分＋１する
//		m_Lin = KJIGLINE28;
		m_Clm = -1;
	}
	else if( m_Idx == KJIGKNAME29 ){
		m_Lin = KJIGLINE29 + 1;//変換等対価の行分＋１する
//		m_Lin = KJIGLINE29;
		m_Clm = -1;
	}
	else if( m_Idx == KJIGKNAME30 ){
		m_Lin = KJIGLINE30 + 1;//変換等対価の行分＋１する
//		m_Lin = KJIGLINE30;
		m_Clm = -1;
	}

	m_curwnd = IDC_ICSDIAGCTRL1;
}

void CShinJigyoKeika::EditOFFIcsdiagctrl1(short index)
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

void CShinJigyoKeika::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
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
	//	if(index == KJIGVAL33E){
	//		m_JigyoKei.SetPrevPosition();
	//	}
	//	else{
//			pos = UpDownFocusMoveCheck(index , 1);
			pos = FocusMoveCheck(index , 1);
			m_JigyoKei.SetPosition(pos);	
	//	}
	}
	if( wTerm==VK_DOWN ){
	//	if(index == KJIGVAL33E){
	//		m_JigyoKei.SetNextPosition();
	//	}
	//	else{
//			pos = UpDownFocusMoveCheck(index , 0);
			pos = FocusMoveCheck(index , 0);
			m_JigyoKei.SetPosition(pos);
	//	}
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
//		m_JigyoKei.SetNextPosition();
		pos = FocusMoveCheck(index , 2);
		m_JigyoKei.SetPosition(pos);
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
//		m_JigyoKei.SetPrevPosition();
		pos = FocusMoveCheck(index , 3);
		m_JigyoKei.SetPosition(pos);
	}

	//------->DELETEキー対応'14.08.06
	if( wTerm == VK_DELETE){
		GetDiagData( index, length, &xdata, -1 );
	}

	SetRedrawScrollToIdx(m_JigyoKei.GetPosition());

/*	int nowpos = m_JigyoKei.GetPosition();	// 移動先または自身のポジション

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
//int CShinJigyoKeika::UpDownFocusMoveCheck(int Index , int Move)
int CShinJigyoKeika::FocusMoveCheck(int Index , int Move)
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
		for(i = 0 ; i < FCS_SHINJIGYOUKEIKACNT ; i++){
			if(Index == FCS_ShinJigyouKeika[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinJigyouKeika[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SHINJIGYOUKEIKACNT ; i++){
			if(Index == FCS_ShinJigyouKeika_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinJigyouKeika_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SHINJIGYOUKEIKACNT ; j++){
			m_JigyoKei.GetAttribute(FCS_ShinJigyouKeika[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinJigyouKeika[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_JigyoKei.GetAttribute(FCS_ShinJigyouKeika[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinJigyouKeika[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SHINJIGYOUKEIKACNT ; j++){
			m_JigyoKei.GetAttribute(FCS_ShinJigyouKeika_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinJigyouKeika_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_JigyoKei.GetAttribute(FCS_ShinJigyouKeika_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinJigyouKeika_LR[j].IDNo;
				break;
			}
		}
	}
	return idx;
}
//<-----------------------------------------

void CShinJigyoKeika::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SHINJIGYOUKEIKACNT; idx++ ){
		m_JigyoKei.GetAttribute(FCS_ShinJigyouKeika[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_ShinJigyouKeika[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SHINJIGYOUKEIKACNT; idx++ ){
		m_JigyoKei.GetAttribute(FCS_ShinJigyouKeika_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_ShinJigyouKeika_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINJIGYOUKEIKACNT-1; idx > -1 ; idx-- ){
		m_JigyoKei.GetAttribute(FCS_ShinJigyouKeika[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_ShinJigyouKeika[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINJIGYOUKEIKACNT-1; idx > -1 ; idx-- ){
		m_JigyoKei.GetAttribute(FCS_ShinJigyouKeika_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_ShinJigyouKeika_LR[idx].IDNo;
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
void CShinJigyoKeika::SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo )
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
int CShinJigyoKeika::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j;
	char	cnm[128], buf[128], CVAL[6];
	int		len;
//	KKVAL*	pK;
	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;

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

	return _xprn( pno, fno, PF, 0, pDC, 0, 1 );
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
//int CShinJigyoKeika::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
int CShinJigyoKeika::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j, lin, max;
	char	cnm[128], buf[128], CVAL[6];
	int		len, st;
	KKVAL*	pK;
	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;

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

/*	if( m_pPset->psSigCoprn&0x2 ){
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
	sprintf_s( buf, sizeof(buf), _T("%x"), (*m_pTaxListData)->YEAR );
	memmove( pBpMv, buf, 2 );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( 2+1 );*/

// 決算額 A
// Aのうち課税取引にならないもの B
// 課税取引額 C
	lin = 0;	max = 5;
	pK = &(*m_pTaxListData)->NRec.KJval[0];
	for( j=0; j<max; j++ ){
		st = 0;
		st = DispJudge( j, 0 );

		::ZeroMemory( buf, sizeof(buf) );
		switch( j ){
			case 0:
				if( st == 1 ){
					m_pArith->l_print( buf, pK->KKval1, FMTX130 );
				}
				break;
			case 1:
				if( st == 1 ){
					m_pArith->l_print( buf, pK->KKval2, FMTX130 );
				}
				break;
			case 2:
/*				if( l_test(pK->KKval1) || l_test(pK->KKval2) )
//					m_pArith->l_print( buf, pK->KKval3, FMTX13 );
					m_pArith->l_print( buf, pK->KKval3, FMTX130 );
				else
//					m_pArith->l_print( buf, pK->KKval3, FMTX130 );
					m_pArith->l_print( buf, pK->KKval3, FMTX13 );*/
				if( st == 1 ){
					m_pArith->l_print( buf, pK->KKval3, FMTX130 );
				}
				break;
			case 3:
				if( st == 1 ){
					m_pArith->l_print( buf, pK->KKval4, FMTX130 );
				}
				break;
			case 4:
				if( st == 1 ){
					m_pArith->l_print( buf, pK->KKval5, FMTX130 );
				}
				break;
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[0] = pBpMv;
		pBpMv += ( len+1 );
		i++;
	}

// 決算額 A
	lin = 1;	max = 7;
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		switch( j+lin ){
			case 6:
				if( m_pArith->l_test((pJ+0)->KKval1)){
					st = 1;
				}
//				break;
			case 5:
				if( m_pArith->l_test((pJ+4)->KKval1) ){
					st = 1;
				}
//				break;
			case 3:
				if( m_pArith->l_test((pJ+1)->KKval1) || m_pArith->l_test((pJ+2)->KKval1) ){
					st = 1;
				}
//				break;
		}
		m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );*/
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
	for( j=0; (j+lin) < max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 2 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX13 );
		}*/
		if( DispJudge( 1, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

// 課税取引額 C
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 2 ){
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

// うち税率４％適用分 D
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 2 ){
			m_pArith->l_print( buf, pK->KKval4, FMTX13 );
		}*/
		if( DispJudge( 3, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval4, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

// うち税率６．３％適用分 E
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 2 ){
			m_pArith->l_print( buf, pK->KKval5, FMTX13 );
		}*/
		if( DispJudge( 4, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval5, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

//--> '16.12.07 INS START
	CString	tmpStr;
	bool isLinkKjprf = (*m_pTaxListData)->IsLinkKjprfname();
//<-- '16.12.07 INS END

// 経費 項目名 25-30
	lin = 24;	max = 30;
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
//-- '16.12.07 --
//		len = (int)strlen( pK->KKnam );
//		memmove( pBpMv, pK->KKnam , len );
//---------------
		if( isLinkKjprf ){
			tmpStr.Empty();
			if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)||(m_pArith->l_test(pK->KKval4)!=0)||(m_pArith->l_test(pK->KKval5)!=0)){ //金額が全て０なら表示しない
				(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmSub, ID_ICSSH_KJ_IPPAN, ((j+lin)+1), tmpStr );
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

	int fontsize = 0;
	char val[5][128];
	CString str;
	for(int l_cnt = KJIGLINE1; l_cnt <= KJIGLINE33; l_cnt++){
		switch(l_cnt){
			case KJIGLINE1:
			case KJIGLINE2:
			case KJIGLINE3:
			case KJIGLINE4:
			case KJIGLINE5:
			case KJIGLINE6:
			case KJIGLINE7:
			case KJIGLINE8:
			case KJIGLINE9:
			case KJIGLINE10:
			case KJIGLINE11:
			case KJIGLINE12:
			case KJIGLINE13:
			case KJIGLINE14:
			case KJIGLINE15:
			case KJIGLINE16:
			case KJIGLINE17:
			case KJIGLINE18:
			case KJIGLINE19:
			case KJIGLINE20:
			case KJIGLINE21:
			case KJIGLINE22:
			case KJIGLINE23:
			case KJIGLINE24:
			case KJIGLINE25:
			case KJIGLINE26:
			case KJIGLINE27:
			case KJIGLINE28:
			case KJIGLINE29:
			case KJIGLINE30:
			case KJIGLINE31:
			case KJIGLINE32:
			case KJIGLINE33:
				for(int idx = 0;idx<5;idx++){
					::ZeroMemory( val[idx], sizeof( val[idx] ));
				}
				m_pArith->l_print( val[0],(*m_pTaxListData)->NRec.KJval[l_cnt].KKval1, FMTX130 );
				m_pArith->l_print( val[1],(*m_pTaxListData)->NRec.KJval[l_cnt].KKval2, FMTX130 );
				m_pArith->l_print( val[2],(*m_pTaxListData)->NRec.KJval[l_cnt].KKval3, FMTX130 );
				m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KJval[l_cnt].KKval4, FMTX130 );
				m_pArith->l_print( val[4],(*m_pTaxListData)->NRec.KJval[l_cnt].KKval5, FMTX130 );
				for(int idx = 0;idx<5;idx++){
					str.Format(val[idx]);
					str.Trim();
					if( str.GetLength() > 13 ){
						fontsize = 1;
					}
				}
				break;
			default:
				break;
		}
		if( fontsize != 0 ){
			break;
		}
	}

	if( fontsize == 1 ){
		return _xprn( pno, 3, PF, 0, pDC, 0, 1 );
	}

	return _xprn( pno, 2, PF, 0, pDC, 0, 1 );
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
//int CShinJigyoKeika::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
int CShinJigyoKeika::PrintHyo3( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j , len , lin , max;
	char	buf[64];
	KKVAL*	pK;
	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;
	int		k = 0;

	i = 0;
	memset( pBp, '\0', Msiz );
//	BP1 = BP;	
	char	*pBpMv = pBp;

// 決算額 A
	lin = 7;	max = 24;
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		//m_pArith->l_print( buf, pK->KKval1, FMTX13 );
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
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
/*		if( (j+lin) != 9 || (j+lin) != 15 || (j+lin) != 16 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX13 );
		}*/
		if( DispJudge( 1, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

// 課税取引額 C
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
/*		if( (j+lin) != 14 || (j+lin) != 17 || (j+lin) != 21 || (j+lin) != 23 ){
			if( l_test(pK->KKval1) || l_test(pK->KKval2) ){
				m_pArith->l_print( buf, pK->KKval3, FMTX13 );
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
	i++;

// うち税率４％適用分 D
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
//		if( (j+lin) != 14 || (j+lin) != 17 || (j+lin) != 21 || (j+lin) != 23 ){
//			if( l_test(pK->KKval1) || l_test(pK->KKval2) ){
//				m_pArith->l_print( buf, pK->KKval4, FMTX13 );
//			}
//		}
		if( DispJudge( 3, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval4, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

// うち税率６．３％適用分 E
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
//		if( (j+lin) != 14 || (j+lin) != 17 || (j+lin) != 21 || (j+lin) != 23 ){
//			if( l_test(pK->KKval1) || l_test(pK->KKval2) ){
//				m_pArith->l_print( buf, pK->KKval5, FMTX13 );
//			}
//		}
		if( DispJudge( 4, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval5, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

// 経費(25-32)
	int st = 0;

	lin = 24;	max = 31;
// 決算額 A
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
//		m_pArith->l_print( buf, pK->KKval1, FMTX13 );
		if( DispJudge( 0, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	// 計32
	pK = &(*m_pTaxListData)->NRec.KJval[31];
	::ZeroMemory( buf, sizeof(buf) );
/*	st = 0;
	for( k=7; k< 31; k++ ){
		if( m_pArith->l_test((pJ+k)->KKval1 )){
			st = 1;
			break;
		}
	}
	if( st == 0 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX13 );
	}else{
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}*/
	if( DispJudge( 0, KJIGLINE32 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );
	i++;

// Aのうち課税取引にならないもの B
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
//		m_pArith->l_print( buf, pK->KKval2, FMTX13 );
		if( DispJudge( 1, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	// 計32
	pK = &(*m_pTaxListData)->NRec.KJval[31];
	::ZeroMemory( buf, sizeof(buf) );
/*	st = 0;
	for( k=7; k< 31; k++ ){
		if( m_pArith->l_test((pJ+k)->KKval2 )){
			st = 1;
			break;
		}
	}
	if( st == 0 ){
		m_pArith->l_print( buf, pK->KKval2, FMTX13 );
	}else{
		m_pArith->l_print( buf, pK->KKval2, FMTX130 );
	}
//	m_pArith->l_print( buf, pK->KKval2, FMTX13 );*/
	if( DispJudge( 1, KJIGLINE32 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval2, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );
	i++;

// 課税取引額 C
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
//		m_pArith->l_print( buf, pK->KKval3, FMTX13 );
		if( DispJudge( 2, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	// 計32
	pK = &(*m_pTaxListData)->NRec.KJval[31];
	::ZeroMemory( buf, sizeof(buf) );
//	m_pArith->l_print( buf, pK->KKval3, FMTX13 );
/*	st = 0;
	for( k=7; k< 31; k++ ){
		if( m_pArith->l_test((pJ+k)->KKval3 )){
			st = 1;
			break;
		}
	}
	if( st == 0 ){
		m_pArith->l_print( buf, pK->KKval3, FMTX13 );
	}else{
		m_pArith->l_print( buf, pK->KKval3, FMTX130 );
	}*/
	if( DispJudge( 2, KJIGLINE32 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval3, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );
	i++;

// うち税率４％適用分 D
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
//		m_pArith->l_print( buf, pK->KKval4, FMTX13 );
		if( DispJudge( 3, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval4, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	// 計32
	pK = &(*m_pTaxListData)->NRec.KJval[31];
	::ZeroMemory( buf, sizeof(buf) );
//	m_pArith->l_print( buf, pK->KKval4, FMTX13 );
/*	st = 0;
	for( k=7; k< 31; k++ ){
		if( m_pArith->l_test((pJ+k)->KKval4 )){
			st = 1;
			break;
		}
	}
	if( st == 0 ){
		m_pArith->l_print( buf, pK->KKval4, FMTX13 );
	}else{
		m_pArith->l_print( buf, pK->KKval4, FMTX130 );
	}*/
	if( DispJudge( 3, KJIGLINE32 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval4, FMTX130 );
	}

	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );
	i++;

// うち税率６．３％適用分 E
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KJval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
//		m_pArith->l_print( buf, pK->KKval5, FMTX13 );
		if( DispJudge( 4, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval5, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	// 計32
	pK = &(*m_pTaxListData)->NRec.KJval[31];
	::ZeroMemory( buf, sizeof(buf) );
//	m_pArith->l_print( buf, pK->KKval5, FMTX13 );
/*	st = 0;
	for( k=7; k< 31; k++ ){
		if( m_pArith->l_test((pJ+k)->KKval5 )){
			st = 1;
			break;
		}
	}
	if( st == 0 ){
		m_pArith->l_print( buf, pK->KKval5, FMTX13 );
	}else{
		m_pArith->l_print( buf, pK->KKval5, FMTX130 );
	}*/
	if( DispJudge( 4, KJIGLINE32 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval5, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );
	i++;

// 差引金額33
// 決算額 A
	pK = &(*m_pTaxListData)->NRec.KJval[32];
	::ZeroMemory( buf, sizeof(buf) );
//	int st = 0;
/*	st = 0;
	for( j = 0; j < 31; j++ ){
		if( l_test( (*m_pTaxListData)->NRec.KJval[j].KKval1 )){
			st = 1;
			break;
		}
	}
	if( st ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}else{
		m_pArith->l_print( buf, pK->KKval1, FMTX13 );
	}*/
	if( DispJudge( 0, KJIGLINE33 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[0] = pBpMv;
	pBpMv += ( len+1 );
	i++;

	int fontsize = 0;
	char val[5][128];
	CString str;
	for(int l_cnt = KJIGLINE1; l_cnt <= KJIGLINE33; l_cnt++){
		switch(l_cnt){
			case KJIGLINE1:
			case KJIGLINE2:
			case KJIGLINE3:
			case KJIGLINE4:
			case KJIGLINE5:
			case KJIGLINE6:
			case KJIGLINE7:
			case KJIGLINE8:
			case KJIGLINE9:
			case KJIGLINE10:
			case KJIGLINE11:
			case KJIGLINE12:
			case KJIGLINE13:
			case KJIGLINE14:
			case KJIGLINE15:
			case KJIGLINE16:
			case KJIGLINE17:
			case KJIGLINE18:
			case KJIGLINE19:
			case KJIGLINE20:
			case KJIGLINE21:
			case KJIGLINE22:
			case KJIGLINE23:
			case KJIGLINE24:
			case KJIGLINE25:
			case KJIGLINE26:
			case KJIGLINE27:
			case KJIGLINE28:
			case KJIGLINE29:
			case KJIGLINE30:
			case KJIGLINE31:
			case KJIGLINE32:
			case KJIGLINE33:
				for(int idx = 0;idx<5;idx++){
					::ZeroMemory( val[idx], sizeof( val[idx] ));
				}
				m_pArith->l_print( val[0],(*m_pTaxListData)->NRec.KJval[l_cnt].KKval1, FMTX130 );
				m_pArith->l_print( val[1],(*m_pTaxListData)->NRec.KJval[l_cnt].KKval2, FMTX130 );
				m_pArith->l_print( val[2],(*m_pTaxListData)->NRec.KJval[l_cnt].KKval3, FMTX130 );
				m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KJval[l_cnt].KKval4, FMTX130 );
				m_pArith->l_print( val[4],(*m_pTaxListData)->NRec.KJval[l_cnt].KKval5, FMTX130 );
				for(int idx = 0;idx<5;idx++){
					str.Format(val[idx]);
					str.Trim();
					if( str.GetLength() > 13 ){
						fontsize = 1;
					}
				}
				break;
			default:
				break;
		}
		if( fontsize != 0 ){
			break;
		}
	}

	if( fontsize == 1 ){
		return _xprn( pno, 5, PF, 0, pDC, 0, 0 );
	}

	return( _xprn( pno, 4, PF, 0, pDC, 0 ));
}


//-----------------------------------------------------------------------------
// 初期ポジションセット ('14.08.08)
//-----------------------------------------------------------------------------
void CShinJigyoKeika::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
		if( (index = m_JigyoKei.GetPosition()) != -1) {
			m_JigyoKei.SetPosition(index);
			m_JigyoKei.SetFocus();
			return;
		}
	}

	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_JigyoKei.SetPosition(KJIGVAL01A);
	}
	else{
		m_JigyoKei.SetPosition(KJIGKNAME25);
	}
	SetRedrawScroll( 0 );

}

//-----------------------------------------------------------------------------
// 表示判定
// 返送値：0…表示なし　1…表示あり
// 引数　：clm…列　line…行
//-----------------------------------------------------------------------------
int CShinJigyoKeika::DispJudge( int clm, int line )
{

	int st = 0;
	int j = 0;

	switch( clm ){
		case 0:	// 決算額　A
			switch( line ){
				case KJIGLINE33:	//差引税額（下）
				case KJIGLINE7:		//差引税額（上）
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[KJIGLINE1].KKval1) ){
						st = 1;
					}

				case KJIGLINE6:		//売上原価　差引原価
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[KJIGLINE5].KKval1) ){
						st = 1;
					}

				case KJIGLINE4:		//売上原価　小計
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[KJIGLINE2].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval1) ){
						st = 1;
					}
					if( line != KJIGLINE33 ){
						break;
					}

				case KJIGLINE32:	//経費	計
					for( j = KJIGLINE8 ; j < KJIGLINE32 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[j].KKval1) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[line].KKval1) ){
						st = 1;
					}
					break;
			}
			break;

		case 1:	// Aのうち課税取引にならないもの　B
			switch( line ){
				case KJIGLINE2:	 //期首商品棚卸高
				case KJIGLINE4:	 //小計
				case KJIGLINE5:	 //期末商品棚卸高
				case KJIGLINE6:	 //差引原価
				case KJIGLINE7:	 //差引金額
				case KJIGLINE10: //水道光熱費
				case KJIGLINE16: //修繕費
				case KJIGLINE17: //消耗品費
				case KJIGLINE33: //差引金額
					//入力不可の為処理しない
					break;

				case KJIGLINE32: //経費　計
					for( j = KJIGLINE8 ; j < KJIGLINE32 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[j].KKval2) ){
							st = 1;	
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[line].KKval2) ){
						st = 1;
					}
					break;
			}
			break;

		case 2:	// 課税取引金額 C(AーB)
			switch( line ){
				case KJIGLINE2:	 //期首商品棚卸高
				case KJIGLINE4:	 //小計
				case KJIGLINE5:	 //期末商品棚卸高
				case KJIGLINE6:	 //差引原価
				case KJIGLINE7:	 //差引金額
				case KJIGLINE15: //損害保険料
				case KJIGLINE18: //減価償却費
				case KJIGLINE22: //利子割引料
				case KJIGLINE24: //貸倒金
				case KJIGLINE33: //差引金額
					//入力不可の為処理しない
					break;
				case KJIGLINE32: //経費	計
					for( j = KJIGLINE8 ; j < KJIGLINE32 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[j].KKval1) ){
							st = 1;
							break;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[j].KKval2) ){
							st = 1;
							break;
						}
					}

					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[line].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KJval[line].KKval2) ){
						st = 1;
					}
					break;
			}
			break;

		case 3: // うち税率４％適用分　D
			switch( line ){
				case KJIGLINE2:	 //期首商品棚卸高
				case KJIGLINE4:	 //小計
				case KJIGLINE5:	 //期末商品棚卸高
				case KJIGLINE6:	 //差引原価
				case KJIGLINE7:	 //差引金額
				case KJIGLINE15: //損害保険料
				case KJIGLINE18: //減価償却費
				case KJIGLINE22: //利子割引料
				case KJIGLINE24: //貸倒金
				case KJIGLINE33: //差引金額
					//入力不可の為処理しない
					break;

				case KJIGLINE32:	//経費	計
					for( j = KJIGLINE8 ; j < KJIGLINE32 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[j].KKval4) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[line].KKval4) ){
						st = 1;
					}
					break;
			}
			break;

		case 4: // うち税率６．３％適用分　D
			switch( line ){
				case KJIGLINE2:	 //期首商品棚卸高
				case KJIGLINE4:	 //小計
				case KJIGLINE5:	 //期末商品棚卸高
				case KJIGLINE6:	 //差引原価
				case KJIGLINE7:	 //差引金額
				case KJIGLINE15: //損害保険料
				case KJIGLINE18: //減価償却費
				case KJIGLINE22: //利子割引料
				case KJIGLINE24: //貸倒金
				case KJIGLINE33: //差引金額
					//入力不可の為処理しない
					break;

				case KJIGLINE32:	//経費	計
					for( j = KJIGLINE8 ; j < KJIGLINE32 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[j].KKval5) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KJval[line].KKval5) ){
						st = 1;
					}
					break;
			}
			break;

	}

	return st;
}

// 手入力サインのクリア
int CShinJigyoKeika::ClearManualInputSign()
{
	int rt = 0;

	for( int idx = 0; idx < 50; idx++ ){
		(*m_pTaxListData)->NRec.KJval[idx].KKauto = 0x00;
	}

	return rt;
}