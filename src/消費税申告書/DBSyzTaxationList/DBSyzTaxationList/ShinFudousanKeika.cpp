// ShinFudousanKeika.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFudousanKeika.h"
#include "ShinFudousanKeikaIdx.h"


// CShinFudousanKeika ダイアログ

IMPLEMENT_DYNAMIC(CShinFudousanKeika, CSyzBaseDlg)

CShinFudousanKeika::CShinFudousanKeika(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFudousanKeika::IDD, pParent)
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

CShinFudousanKeika::~CShinFudousanKeika()
{
}

void CShinFudousanKeika::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_FudousanKei);
}


BEGIN_MESSAGE_MAP(CShinFudousanKeika, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinFudousanKeika メッセージ ハンドラ

BOOL CShinFudousanKeika::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_FudousanKei;
	m_BottomIdx = 33;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFudousanKeika::Init()
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
void CShinFudousanKeika::InitDiagAttr()
{
	
	DIAGRAM_ATTRIBUTE data;
	long dg1[] = { KFUDKNAME03, KFUDKNAME12, -1 };

	for( int i=0;; i++ ){
		if( dg1[i] == -1 ){
			break;
		}
		// ank対応
		m_FudousanKei.GetAttribute( (short)dg1[i], (LPUNKNOWN)&data );
		data.attr_editattr |= DIAG_EATTR_ZENHAN;//DIAG_EATTR_ZENHAN ank対応
		m_FudousanKei.SetAttribute( (short)dg1[i], (LPUNKNOWN)&data, 1 );
	}

	int	cnt = 0;
	while( 1 ){
		if( FudousanKeika[cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = FudousanKeika[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = FudousanKeika[cnt].UnConnect;
			}
			else{
				Flg = FudousanKeika[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL1, FudousanKeika[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, FudousanKeika[cnt].Connect );
		cnt++;
	}

	SetFocusRange();

	m_FudousanKei.EnableDelete(0);//DELETEキー１回で削除モード設定'14.08.06

	
}

//-----------------------------------------------------------------------------
// 画面表示
//-----------------------------------------------------------------------------
// 引数	mode	：	
//-----------------------------------------------------------------------------
void CShinFudousanKeika::DispList( int mode )
{
	m_FudousanKei.OffControl( TRUE );
//2016.06.22 INSERT START
	(*m_Scalc)->Shz_KeiAllCalc(2);
//2016.06.22 INSERT END
	short	i;
	for( i = 0; i < FKLINCNT; i++ ){
		DispFudousanKeikaDiagData( i );
	}

	DIAGRAM_DATA	data;
	if( !mode ){
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, FKYEARID, TRUE, CIcsdiagctrl );
		data.data_disp.Format( _T("（平成%2x年分）"), (*m_pTaxListData)->YEAR );
		diag_setdata( IDC_ICSDIAGCTRL1, FKYEARID, &data, CIcsdiagctrl );

		PostMessage(WM_VSCROLL, SB_TOP, 0 );
		m_Idx = FKVMIN;
	}

	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, FKZKBNID, TRUE, CIcsdiagctrl );
	data.data_disp = _T("経理方式：");
	if( (*m_pTaxListData)->Head.KHstax&0x01 ){
		data.data_disp += _T("税抜き");
	}
	else{
		data.data_disp += _T("税込み");
	}
	diag_setdata( IDC_ICSDIAGCTRL1, FKZKBNID, &data, CIcsdiagctrl );

//2016.06.22 DELETE START
//	diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );
//2016.06.22 DELETE END
//2016.06.22 INSERT START
	int edit[] = {KFUDKNAME03,KFUDKNAME12};
	BOOL edit_focus = FALSE;

	for( int cnt = 0; cnt < ARRAYCOUNT(edit); cnt++ ){
		Switch_KamokuEdit( IDC_ICSDIAGCTRL1, edit[cnt] );

		if( m_FudousanKei.GetPosition() == edit[cnt] ){
			edit_focus = TRUE;
		}
	}

	if(( (*m_pTaxListData)->Head.KHkoke == 1 ) && ( edit_focus == TRUE )){
		this->SetFocus();
	}
//2016.06.22 INSERT END

	//SetRedrawScrollToIdx(m_FudousanKei.GetPosition());
}

//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
// 引数	dlin	：	
//-----------------------------------------------------------------------------
void CShinFudousanKeika::DispFudousanKeikaDiagData( long dlin )
{
	//'14.08.21
	short	idx, i/*, j*/;
	long	lin;

	//データ保存ポジションセット
	static long KFudDataPos[] = {0 , 1 , 2 , 3 , 15 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 };

	lin = KFudDataPos[dlin];

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KFval[lin];
	char	val[6];
	DIAGRAM_DATA	data;
	int		st;

	idx = (short)(dlin * FKWIDECNT) + FKVMIN;
	for( i = 0 ; i < FKWIDECNT ; i++ ){
		st = 0;
		::ZeroMemory( val, sizeof(val) );
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, idx+i, TRUE, CIcsdiagctrl );
		switch( i ){
			case 0:	// 決算額 A
/*				switch( lin ){
					case KFUDLINE15:	//差引金額
					case KFUDLINE4:		//収入金額　計					
						for( j = KFUDLINE1 ; j < KFUDLINE4 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
								st = 1;
								break;
							}
						}
//						break;

						if( lin != KFUDLINE15 ){
							break;
						}

//					case KFUDLINE15:	//差引金額
//						st = 1;
//						break;

					case KFUDLINE14:	//必要経費　計
						for( j = KFUDLINE5; j < KFUDLINE14; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
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
					case KFUDLINE7:		//修繕費
					case KFUDLINE15:	//差引金額
						//入力可所無し
						break;

					case KFUDLINE4:		//収入金額　計
						for( j = KFUDLINE1; j < KFUDLINE4 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval2) ){
								st = 1;
								break;
							}
						}
						break;

					case KFUDLINE14:	//必要経費　計
						for( j = KFUDLINE5 ; j < KFUDLINE14 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval2) ){
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

			case 2:	// 課税取引金額　C（A－B）
/*				switch( lin ){
					case KFUDLINE6:		//損害保険料
					case KFUDLINE8:		//減価償却費
					case KFUDLINE9:		//借入金利子
					case KFUDLINE15:	//差引金額
						break;

					case KFUDLINE4:		//収入金額　計
						for( j = KFUDLINE1 ; j < KFUDLINE4 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
								st = 1;
								break;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval2) ){
								st = 1;
								break;
							}
						}
						break;

					case KFUDLINE14:	//必要経費　計
						for( j = KFUDLINE5; j < KFUDLINE14; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
								st = 1;
								break;
							}
							if( j == KFUDLINE7 ){ //修繕費はスキップ
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval2) ){
								st = 1;
								break;
							}
						}
						break;

					default:
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[lin].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KFval[lin].KKval2) ){
							st = 1;
						}
						break;
				}*/

				memcpy( val, pK->KKval3, 6 );
				break;

			case 3:	// うち税率４％適用分　D
/*				switch( lin ){
					case KFUDLINE6:		//損害保険料
					case KFUDLINE8:		//減価償却費
					case KFUDLINE9:		//借入金利子
					case KFUDLINE15:	//差引金額
						//入力可所無し
						break;

					case KFUDLINE4:		//収入金額　計
						for( j = KFUDLINE1; j < KFUDLINE4 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval4) ){
								st = 1;
								break;
							}
						}
						break;

					case KFUDLINE14:	//必要経費　計
						for( j = KFUDLINE5 ; j < KFUDLINE14 ; j++ ){
							if(j == KFUDLINE6 || j == KFUDLINE8 || j == KFUDLINE9){//損害保険料、減価償却費、借入金利子　は入力不可なのでチェックしない
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval4) ){
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


			case 4:	// うち税率６．３％適用分　E
/*				switch( lin ){
					case KFUDLINE6:		//損害保険料
					case KFUDLINE8:		//減価償却費
					case KFUDLINE9:		//借入金利子
					case KFUDLINE15:	//差引金額
						//入力可所無し
						break;

					case KFUDLINE4:		//収入金額　計
						for( j = KFUDLINE1; j < KFUDLINE4 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval5) ){
								st = 1;
								break;
							}
						}
						break;

					case KFUDLINE14:	//必要経費　計
						for( j = KFUDLINE5 ; j < KFUDLINE14 ; j++ ){
							if(j == KFUDLINE6 || j == KFUDLINE8 || j == KFUDLINE9){//損害保険料、減価償却費、借入金利子　は入力不可なのでチェックしない
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval5) ){
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

		if( DispJudge( i, lin ) ){
			memcpy( data.data_val, val, 6 );
			diag_setdata( IDC_ICSDIAGCTRL1, idx+i, &data, CIcsdiagctrl );
		}
	}


	// 項目名
	char	buf[128];

	if(dlin == KFUDLINE3 || dlin == KFUDLINE12){//項目名入力可所　③　⑫　の場合
		if( dlin == KFUDLINE12 ){
			lin = KFudDataPos[dlin + 1];
			pK = &(*m_pTaxListData)->NRec.KFval[lin];
		}
		if( !(pK->KKsign&0x80) ){
			m_Util.DiagOcxIniz( &data );
			::ZeroMemory( buf, sizeof(buf) );
//2016.06.22 UPDATE START
			//switch( dlin ){
			//	case KFUDLINE3:	//③
			//		idx = KFUDKNAME03;	
			//		break;
			//	case KFUDLINE12://⑫
			//		idx = KFUDKNAME12;
			//		break;
			//}
			//memcpy( buf, pK->KKnam, sizeof(pK->KKnam));

			CString tmp;
			int prf_no = 0;
			switch( dlin ){
				case KFUDLINE3:	//③
					idx = KFUDKNAME03;
					prf_no = 3;
					break;
				case KFUDLINE12://⑫
					idx = KFUDKNAME12;
					prf_no = 12;
					break;
			}

//-- '16.12.07 --
//			if(((*m_pTaxListData)->Head.KHkoke == 1 )&&( prf_no != 0 )){
//---------------
			if( (*m_pTaxListData)->IsLinkKjprfname() && (prf_no!=0) ){
//---------------
				if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)||(m_pArith->l_test(pK->KKval4)!=0)||(m_pArith->l_test(pK->KKval5)!=0)){ //金額が全て０なら表示しない
					(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmSub, ID_ICSSH_KJ_FUDOUSAN, prf_no, tmp );
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
	if( dlin < 4 ){
		lin = dlin;
	}
	else if( dlin == 4 ){
		lin = 15;
	}
	else{
		lin = dlin-1;
	}

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KFval[lin];
	char	val[6];
	DIAGRAM_DATA	data;
	int		st;

	idx = (short)(dlin*WIDECNT)+VMIN;
	for( i=0; i<WIDECNT; i++ ){
		st = 0;
		::ZeroMemory( val, sizeof(val) );
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, idx+i, TRUE, CIcsdiagctrl );
		switch( i ){
			case 0:	// 決算額
				switch( lin ){
					case 14:
					case 3:	
						for( j=0; j<3; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
								st = 1;
								break;
							}
						}
						if( lin !=14 ){
							break;
						}
					case 13:
						for( j=4; j<13; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
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
					case 6:
					case 14:
						break;
					case 3:	
						for( j=0; j<3; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval2) ){
								st = 1;
								break;
							}
						}
						break;
					case 13:
						for( j=4; j<13; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval2) ){
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
					case 5:
					case 7:
					case 8: 
					case 14:
						break;
					default:
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[lin].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KFval[lin].KKval2) ){
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
	if( ((lin+1)==3) || ((lin+1)==12) ){
		if( !(pK->KKsign&0x80) ){
			m_Util.DiagOcxIniz( &data );
			::ZeroMemory( buf, sizeof(buf) );
			switch(lin+1){
				case 3:	
					idx = KFUDKNAME03;	
					break;
				case 12:
					idx = KFUDKNAME12;
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
//		mode	：
//-----------------------------------------------------------------------------
// 返送値	0	：	
//-----------------------------------------------------------------------------
int CShinFudousanKeika::GetDiagData( long index, short leng, DIAGRAM_DATA *pData, int mode )
{

	//'14.08.21
	if( index < 0 ){
		return 0;
	}

	char	dbuf[128]={0};
	char	*buff;
	int		len;

	len = pData->data_edit.GetLength();
	buff = (char *)((LPCTSTR)(pData->data_edit));
	memmove( dbuf, buff, len );


	//データ保存ポジションセット
	long	lin;
	static long KFudDataPos[] = {0 , 1 , 2 , 3 , 15 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 };

	lin = KFudDataPos[m_Lin];

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KFval[lin];

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
	(*m_Scalc)->Shz_KeiAllCalc(2);//'14.08.06
	for( int i = 0 ; i < FKLINCNT ; i++ ){
		DispFudousanKeikaDiagData( i );
	}
	return 0;


	/*

	if( index < 0 ){
		return 0;
	}

	char	dbuf[128]={0};
	char	*buff;
	int		len;

	len = pData->data_edit.GetLength();
	buff = (char *)((LPCTSTR)(pData->data_edit));
	memmove( dbuf, buff, len );

	long	lin;
	if( m_Lin < 4 ){
		lin = m_Lin;
	}
	else if( m_Lin == 4 ){
		lin = 15;
	}
	else{
		lin = m_Lin-1;
	}

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KFval[lin];
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
	(*m_Scalc)->Shz_AllCalc(2);//'14.08.06
	for( int i=0; i<LINCNT; i++ ){
		DispFudousanKeikaDiagData( i );
	}
	return 0;

	*/

}


BEGIN_EVENTSINK_MAP(CShinFudousanKeika, CSyzBaseDlg)
ON_EVENT(CShinFudousanKeika, IDC_ICSDIAGCTRL1, 1, CShinFudousanKeika::EditONIcsdiagctrl1, VTS_I2)
ON_EVENT(CShinFudousanKeika, IDC_ICSDIAGCTRL1, 2, CShinFudousanKeika::EditOFFIcsdiagctrl1, VTS_I2)
ON_EVENT(CShinFudousanKeika, IDC_ICSDIAGCTRL1, 3, CShinFudousanKeika::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFudousanKeika::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_FudousanKei.OffControl( FALSE );

	m_Idx = index;
	if( (m_Idx >= FKVMIN) && (m_Idx <= FKVMAX) ){
		m_Lin = (index - FKVMIN ) / FKWIDECNT;
		m_Clm = (index - FKVMIN ) % FKWIDECNT;
	}
	else if( m_Idx == KFUDKNAME03 ){
		m_Lin = KFUDLINE3;
		m_Clm = -1;
	}
	else if( m_Idx == KFUDKNAME12 ){
		m_Lin = KFUDLINE12 + 1;//変換等対価の行分＋１する
//		m_Lin = KFUDLINE12;
		m_Clm = -1;
	}
	m_curwnd = IDC_ICSDIAGCTRL1;//'14.08.01

}

void CShinFudousanKeika::EditOFFIcsdiagctrl1(short index)
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

void CShinFudousanKeika::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
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
	//	if(index == KFUDVAL15E){
	//		m_FudousanKei.SetPrevPosition();
	//	}
	//	else{
//			pos = UpDownFocusMoveCheck(index , 1);
			pos = FocusMoveCheck(index , 1);
			m_FudousanKei.SetPosition(pos);	
	//	}
	}
	if( wTerm==VK_DOWN ){
	//	if(index == KFUDVAL15E){
	//		m_FudousanKei.SetNextPosition();
	//	}
	//	else{
//			pos = UpDownFocusMoveCheck(index , 0);
			pos = FocusMoveCheck(index , 0);
			m_FudousanKei.SetPosition(pos);
	//	}
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
//		m_FudousanKei.SetNextPosition();
		pos = FocusMoveCheck(index , 2);
		m_FudousanKei.SetPosition(pos);	
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
//		m_FudousanKei.SetPrevPosition();
			pos = FocusMoveCheck(index , 3);
			m_FudousanKei.SetPosition(pos);	

	}

	//------->DELETEキー対応'14.08.06
	if( wTerm == VK_DELETE){
		GetDiagData( index, length, &xdata, -1 );
	}

	SetRedrawScrollToIdx(m_FudousanKei.GetPosition());

/*	int nowpos = m_FudousanKei.GetPosition();	// 移動先または自身のポジション

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
//int CShinFudousanKeika::UpDownFocusMoveCheck(int Index , int Move)
int CShinFudousanKeika::FocusMoveCheck(int Index , int Move)
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
		for(i = 0 ; i < FCS_SHINFUDOUSANKEIKACNT ; i++){
			if(Index == FCS_ShinFudousanKeika[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinFudousanKeika[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SHINFUDOUSANKEIKACNT ; i++){
			if(Index == FCS_ShinFudousanKeika_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinFudousanKeika_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SHINFUDOUSANKEIKACNT ; j++){
			m_FudousanKei.GetAttribute(FCS_ShinFudousanKeika[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinFudousanKeika[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_FudousanKei.GetAttribute(FCS_ShinFudousanKeika[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinFudousanKeika[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SHINFUDOUSANKEIKACNT ; j++){
			m_FudousanKei.GetAttribute(FCS_ShinFudousanKeika_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinFudousanKeika_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_FudousanKei.GetAttribute(FCS_ShinFudousanKeika_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinFudousanKeika_LR[j].IDNo;
				break;
			}
		}
	}
	return idx;
}
//<-----------------------------------------

void CShinFudousanKeika::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SHINFUDOUSANKEIKACNT; idx++ ){
		m_FudousanKei.GetAttribute(FCS_ShinFudousanKeika[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_ShinFudousanKeika[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SHINFUDOUSANKEIKACNT; idx++ ){
		m_FudousanKei.GetAttribute(FCS_ShinFudousanKeika_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_ShinFudousanKeika_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINFUDOUSANKEIKACNT-1; idx > -1 ; idx-- ){
		m_FudousanKei.GetAttribute(FCS_ShinFudousanKeika[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_ShinFudousanKeika[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINFUDOUSANKEIKACNT-1; idx > -1 ; idx-- ){
		m_FudousanKei.GetAttribute(FCS_ShinFudousanKeika_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_ShinFudousanKeika_LR[idx].IDNo;
			break;
		}
	}
}

BOOL CShinFudousanKeika::PreTranslateMessage(MSG* pMsg)
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

#define		Msiz	512*20
//-----------------------------------------------------------------------------
// 印刷情報のセット
//-----------------------------------------------------------------------------
// 引数	pVprn	：	出力クラス
//		pPset	：	印刷情報
//-----------------------------------------------------------------------------
void CShinFudousanKeika::SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo )
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
int CShinFudousanKeika::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j;
	char	cnm[128], buf[128], CVAL[6];
	int		len;
	int		st = 0;
	KKVAL*	pJ = (*m_pTaxListData)->NRec.KFval;

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
//int CShinFudousanKeika::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
int CShinFudousanKeika::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j, lin, max;
	char	buf[128], CVAL[6];
	int		len;
	int		st = 0;
	KKVAL*	pK;
	KKVAL*	pJ = (*m_pTaxListData)->NRec.KFval;

	// 初期化
	memset( PF, '\0', sizeof(PF) );


	i = 0;
	memset( pBp, '\0', Msiz );
	char	*pBpMv = pBp;
//	pBpMv = BP;
	::ZeroMemory( CVAL, sizeof(CVAL) );

// 会社名
	::ZeroMemory( buf, sizeof(buf) );
/*	::ZeroMemory( cnm, sizeof(cnm) );

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
	sprintf_s( buf, sizeof(buf), _T("%x"), (*m_pTaxListData)->YEAR );
	memmove( pBpMv, buf, 2 );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( 2+1 );*/

// 収入金額
// 決算額 A
	lin = 0;	max = 4;
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
/*		if( j == 3 ){ // 収入金額　計
			for( int k=0; k<3; k++ ){
				if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval1)){
					st = 1;
					break;
				}
			}
		}*/
		if( DispJudge( 0, j ) == 1 ){
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
		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
/*		if( j == 3 ){ // 収入金額　計
			for( int k=0; k<3; k++ ){
				if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval2)){
					st = 1;
					break;
				}
			}
		}*/
		if( DispJudge( 1, j ) == 1 ){
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
		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
/*		if( j == 3 ){ // 収入金額　計
			for( int k=0; k<3; k++ ){
				if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval1)){
					st = 1;
					break;
				}
				if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval2)){
					st = 1;
					break;
				}
			}
		}else{
			if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j+lin].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j+lin].KKval2)){
				st = 1;
			}
		}*/
		if( DispJudge( 2, j ) == 1 ){
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
		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
/*		if( j == 3 ){ // 収入金額　計
			for( int k=0; k<3; k++ ){
				if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval4)){
					st = 1;
					break;
				}
			}
		}*/
		if( DispJudge( 3, j ) == 1 ){
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
		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
/*		if( j == 3 ){ // 収入金額　計
			for( int k=0; k<3; k++ ){
				if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval5)){
					st = 1;
					break;
				}
			}
		}*/
		if( DispJudge( 4, j ) == 1 ){
			m_pArith->l_print( buf, pK->KKval5, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

/*
// 収入金額 計
// 決算額 A
	pK = &(*m_pTaxListData)->NRec.KFval[3];
	::ZeroMemory( buf, sizeof(buf) );
	m_pArith->l_print( buf, pK->KKval1, FMTX13 );
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[0] = pBpMv;
	pBpMv += ( len+1 );
	i++;

// Aのうち課税取引にならないもの B
	pK = &(*m_pTaxListData)->NRec.KFval[3];
	::ZeroMemory( buf, sizeof(buf) );
	m_pArith->l_print( buf, pK->KKval2, FMTX13 );
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[0] = pBpMv;
	pBpMv += ( len+1 );
	i++;

// 課税取引額 C
	pK = &(*m_pTaxListData)->NRec.KFval[3];
	::ZeroMemory( buf, sizeof(buf) );
	m_pArith->l_print( buf, pK->KKval3, FMTX13 );
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[0] = pBpMv;
	pBpMv += ( len+1 );
	i++;

// 収入金額 返還等対価
// 決算額 A
	pK = &(*m_pTaxListData)->NRec.KFval[4];
	::ZeroMemory( buf, sizeof(buf) );
	m_pArith->l_print( buf, pK->KKval1, FMTX13 );
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[0] = pBpMv;
	pBpMv += ( len+1 );
	i++;

// Aのうち課税取引にならないもの B
	pK = &(*m_pTaxListData)->NRec.KFval[4];
	::ZeroMemory( buf, sizeof(buf) );
	m_pArith->l_print( buf, pK->KKval2, FMTX13 );
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[0] = pBpMv;
	pBpMv += ( len+1 );
	i++;

// 課税取引額 C
	pK = &(*m_pTaxListData)->NRec.KFval[4];
	::ZeroMemory( buf, sizeof(buf) );
	m_pArith->l_print( buf, pK->KKval3, FMTX13 );
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[0] = pBpMv;
	pBpMv += ( len+1 );
	i++;
*/

//--> '16.12.07 INS START
	CString	tmpStr;
	bool isLinkKjprf = (*m_pTaxListData)->IsLinkKjprfname();
//<-- '16.12.07 INS END

// 項目名 3,12
	pK = &(*m_pTaxListData)->NRec.KFval[KFUDLINE3];
	::ZeroMemory( buf, sizeof(buf) );
//-- '16.12.07 --
//	len = (int)strlen( pK->KKnam );
//	memmove( pBpMv, pK->KKnam , len );
//---------------
	if( isLinkKjprf ){
		tmpStr.Empty();
		if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)||(m_pArith->l_test(pK->KKval4)!=0)||(m_pArith->l_test(pK->KKval5)!=0)){ //金額が全て０なら表示しない
			(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmSub, ID_ICSSH_KJ_FUDOUSAN, (KFUDLINE3+1), tmpStr );
		}
		len = tmpStr.GetLength();
		memmove( pBpMv, tmpStr, len );
	}
	else{
		len = (int)strlen( pK->KKnam );
		memmove( pBpMv, pK->KKnam , len );
	}
//---------------
	PF[i].PITM[0] = pBpMv;
	pBpMv += ( len+1 );
	i++;

	pK = &(*m_pTaxListData)->NRec.KFval[KFUDLINE12];
	::ZeroMemory( buf, sizeof(buf) );
//-- '16.12.07 --
//	len = (int)strlen( pK->KKnam );
//	memmove( pBpMv, pK->KKnam , len );
//---------------
	if( isLinkKjprf ){
		tmpStr.Empty();
		if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)||(m_pArith->l_test(pK->KKval4)!=0)||(m_pArith->l_test(pK->KKval5)!=0)){ //金額が全て０なら表示しない
			(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmSub, ID_ICSSH_KJ_FUDOUSAN, (KFUDLINE12+1), tmpStr );
		}
		len = tmpStr.GetLength();
		memmove( pBpMv, tmpStr, len );
	}
	else{
		len = (int)strlen( pK->KKnam );
		memmove( pBpMv, pK->KKnam , len );
	}
//---------------
	PF[i].PITM[0] = pBpMv;
	pBpMv += ( len+1 );
	i++;

		int fontsize = 0;
		char val[5][128];
		CString str;
		for(int l_cnt = KFUDLINE1; l_cnt <= KFUDLINE4TA; l_cnt++){
			switch(l_cnt){
				case KFUDLINE1:
				case KFUDLINE2:
				case KFUDLINE3:
				case KFUDLINE4:
				case KFUDLINE4TA:
				case KFUDLINE5:
				case KFUDLINE6:
				case KFUDLINE7:
				case KFUDLINE8:
				case KFUDLINE9:
				case KFUDLINE10:
				case KFUDLINE11:
				case KFUDLINE12:
				case KFUDLINE13:
				case KFUDLINE14:
				case KFUDLINE15:
					for(int idx = 0;idx<5;idx++){
						::ZeroMemory( val[idx], sizeof( val[idx] ));
					}
					m_pArith->l_print( val[0],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval1, FMTX130 );
					m_pArith->l_print( val[1],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval2, FMTX130 );
					m_pArith->l_print( val[2],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval3, FMTX130 );
					m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval4, FMTX130 );
					m_pArith->l_print( val[4],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval5, FMTX130 );
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
//int CShinFudousanKeika::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
int CShinFudousanKeika::PrintHyo3( int pno, int fno, CDC *pDC, char *pBp )
{

	int		i, j , len , lin , max;
	char	buf[64];
	KKVAL*	pK;
	int st = 0;

	i = 0;
	memset( pBp, '\0', Msiz );
//	BP1 = BP;	
	char	*pBpMv = pBp;

// 決算額 A
	lin = 4;	max = 14;
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
/*		st = 0;
		switch( j+lin ){
			case 13:	//必要経費　計
				for( int k=4;k<13;k++ ){
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval1)){
						st = 1;
						break;
					}
				}
				break;
			default:
				st = 0;
		}
		if( st == 0 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX13 );
		}else{
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}*/
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
		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
/*		st = 0;
		switch( j+lin ){
			case 13:	//必要経費　計
				for( int k=4;k<13;k++ ){
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval1)){
						st = 1;
						break;
					}
				}
				break;
			default:
				st = 0;
				break;
		}
		if( st == 0 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX13 );
		}else{
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
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
		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
/*		st = 0;
		switch( j+lin ){
			case 13:
				for( int k=4;k<13;k++ ){
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval1)){
						st = 1;
						break;
					}
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval2)){
						st = 1;
						break;
					}
				}
				break;
			default:
				if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j+lin].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j+lin].KKval2)){
					st = 1;
				}
				break;
		}
		if( st == 0 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX13 );
		}else{
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
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
		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
/*		st = 0;
		switch( j+lin ){
			case 13:	//必要経費　計
				for( int k=4;k<13;k++ ){
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval4)){
						st = 1;
						break;
					}
				}
				break;
			default:
				st = 0;
				break;
		}
		if( st == 0 ){
			m_pArith->l_print( buf, pK->KKval4, FMTX13 );
		}else{
			m_pArith->l_print( buf, pK->KKval4, FMTX130 );
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
		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
/*		st = 0;
		switch( j+lin ){
			case 13:	//必要経費　計
				for( int k=4;k<13;k++ ){
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval5)){
						st = 1;
						break;
					}
				}
				break;
			default:
				st = 0;
				break;
		}
		if( st == 0 ){
			m_pArith->l_print( buf, pK->KKval5, FMTX13 );
		}else{
			m_pArith->l_print( buf, pK->KKval5, FMTX130 );
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

//差引金額
// 決算額 A
		pK = &(*m_pTaxListData)->NRec.KFval[14];
		::ZeroMemory( buf, sizeof(buf) );
/*		st = 0;
		for( int k=0;k<13;k++ ){
			if( k == 3 ) continue;
			if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval1)){
				st = 1;
				break;
			}
		}
		if( st == 0 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX13 );
		}else{
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}*/
		if( DispJudge( 0, KFUDLINE15 ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[0] = pBpMv;
		pBpMv += ( len+1 );
		i++;

// Aのうち課税取引にならないもの B
		pK = &(*m_pTaxListData)->NRec.KFval[14];
		::ZeroMemory( buf, sizeof(buf) );
/*		st = 0;
		for( int k=0;k<13;k++ ){
			if( k == 3 ) continue;
			if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval2)){
				st = 1;
				break;
			}
		}
		if( st == 0 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX13 );
		}else{
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
		}*/
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[0] = pBpMv;
		pBpMv += ( len+1 );
		i++;

// 課税取引額 C
		pK = &(*m_pTaxListData)->NRec.KFval[14];
		::ZeroMemory( buf, sizeof(buf) );
/*		st = 0;
		for( int k=0;k<13;k++ ){
			if( k == 3 ) continue;
			if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval1)){
				st = 1;
				break;
			}
			if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval2)){
				st = 1;
				break;
			}
		}
		if( st == 0 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX13 );
		}else{
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}*/
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[0] = pBpMv;
		pBpMv += ( len+1 );
		i++;

// うち税率４％適用分 D
		pK = &(*m_pTaxListData)->NRec.KFval[14];
		::ZeroMemory( buf, sizeof(buf) );
/*		st = 0;
		for( int k=0;k<13;k++ ){
			if( k == 3 ) continue;
			if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval4)){
				st = 1;
				break;
			}
		}
		if( st == 0 ){
			m_pArith->l_print( buf, pK->KKval4, FMTX13 );
		}else{
			m_pArith->l_print( buf, pK->KKval4, FMTX130 );
		}*/
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[0] = pBpMv;
		pBpMv += ( len+1 );
		i++;

// うち税率６．３％適用分 E
		pK = &(*m_pTaxListData)->NRec.KFval[14];
		::ZeroMemory( buf, sizeof(buf) );
/*		st = 0;
		for( int k=0;k<13;k++ ){
			if( k == 3 ) continue;
			if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval5)){
				st = 1;
				break;
			}
		}
		if( st == 0 ){
			m_pArith->l_print( buf, pK->KKval5, FMTX13 );
		}else{
			m_pArith->l_print( buf, pK->KKval5, FMTX130 );
		}*/
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[0] = pBpMv;
		pBpMv += ( len+1 );
		i++;

		int fontsize = 0;
		char val[5][128];
		CString str;
		for(int l_cnt = KFUDLINE1; l_cnt <= KFUDLINE15; l_cnt++){
			switch(l_cnt){
				case KFUDLINE1:
				case KFUDLINE2:
				case KFUDLINE3:
				case KFUDLINE4:
				case KFUDLINE4TA:
				case KFUDLINE5:
				case KFUDLINE6:
				case KFUDLINE7:
				case KFUDLINE8:
				case KFUDLINE9:
				case KFUDLINE10:
				case KFUDLINE11:
				case KFUDLINE12:
				case KFUDLINE13:
				case KFUDLINE14:
				case KFUDLINE15:
					for(int idx = 0;idx<5;idx++){
						::ZeroMemory( val[idx], sizeof( val[idx] ));
					}
					m_pArith->l_print( val[0],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval1, FMTX130 );
					m_pArith->l_print( val[1],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval2, FMTX130 );
					m_pArith->l_print( val[2],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval3, FMTX130 );
					m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval4, FMTX130 );
					m_pArith->l_print( val[4],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval5, FMTX130 );
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
			return( _xprn( pno, 5, PF, 0, pDC, 0 ));
		}

	return( _xprn( pno, 4, PF, 0, pDC, 0 ));
}


//-----------------------------------------------------------------------------
// 初期ポジションセット ('14.08.08)
//-----------------------------------------------------------------------------
void CShinFudousanKeika::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
		if( (index = m_FudousanKei.GetPosition()) != -1) {
			m_FudousanKei.SetPosition(index);
			m_FudousanKei.SetFocus();
			return;
		}
	}

	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_FudousanKei.SetPosition(KFUDVAL01A);
	}
	else{
		m_FudousanKei.SetPosition(KFUDKNAME03);
	}
	SetRedrawScroll( 0 );

}
//-----------------------------------------------------------------------------
// 表示判定
// 返送値：0…表示なし　1…表示あり
// 引数　：clm…列　line…行
//-----------------------------------------------------------------------------
int CShinFudousanKeika::DispJudge( int clm, int line )
{
	int st = 0;
	int j = 0;

	switch( clm ){
		case 0:	// 決算額 A
			switch( line ){
				case KFUDLINE15:	//差引金額
				case KFUDLINE4:		//収入金額　計					
					for( j = KFUDLINE1 ; j < KFUDLINE4 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
							st = 1;
							break;
						}
					}

					if( line != KFUDLINE15 ){
						break;
					}

				case KFUDLINE14:	//必要経費　計
					for( j = KFUDLINE5; j < KFUDLINE14; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval1) ){
						st = 1;
					}
					break;
			}
			break;

		case 1:	// Aのうち課税取引にならないもの　B
			switch( line ){
				case KFUDLINE7:		//修繕費
				case KFUDLINE15:	//差引金額
					//入力可所無し
					break;

				case KFUDLINE4:		//収入金額　計
					for( j = KFUDLINE1; j < KFUDLINE4 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval2) ){
							st = 1;
							break;
						}
					}
					break;

				case KFUDLINE14:	//必要経費　計
					for( j = KFUDLINE5 ; j < KFUDLINE14 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval2) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval2) ){
						st = 1;
					}
					break;
			}
			break;

		case 2:	// 課税取引金額　C（A－B）
			switch( line ){
				case KFUDLINE6:		//損害保険料
				case KFUDLINE8:		//減価償却費
				case KFUDLINE9:		//借入金利子
				case KFUDLINE15:	//差引金額
					break;

				case KFUDLINE4:		//収入金額　計
					for( j = KFUDLINE1 ; j < KFUDLINE4 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
							st = 1;
							break;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval2) ){
							st = 1;
							break;
						}
					}
					break;

				case KFUDLINE14:	//必要経費　計
					for( j = KFUDLINE5; j < KFUDLINE14; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
							st = 1;
							break;
						}
						if( j == KFUDLINE7 ){ //修繕費はスキップ
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval2) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval2) ){
						st = 1;
					}
					break;
			}
			break;

		case 3:	// うち税率４％適用分　D
			switch( line ){
				case KFUDLINE6:		//損害保険料
				case KFUDLINE8:		//減価償却費
				case KFUDLINE9:		//借入金利子
				case KFUDLINE15:	//差引金額
					//入力可所無し
					break;

				case KFUDLINE4:		//収入金額　計
					for( j = KFUDLINE1; j < KFUDLINE4 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval4) ){
							st = 1;
							break;
						}
					}
					break;

				case KFUDLINE14:	//必要経費　計
					for( j = KFUDLINE5 ; j < KFUDLINE14 ; j++ ){
						if(j == KFUDLINE6 || j == KFUDLINE8 || j == KFUDLINE9){//損害保険料、減価償却費、借入金利子　は入力不可なのでチェックしない
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval4) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval4) ){
						st = 1;
					}
					break;
			}
			break;


		case 4:	// うち税率６．３％適用分　E
			switch( line ){
				case KFUDLINE6:		//損害保険料
				case KFUDLINE8:		//減価償却費
				case KFUDLINE9:		//借入金利子
				case KFUDLINE15:	//差引金額
					//入力可所無し
					break;

				case KFUDLINE4:		//収入金額　計
					for( j = KFUDLINE1; j < KFUDLINE4 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval5) ){
							st = 1;
							break;
						}
					}
					break;

				case KFUDLINE14:	//必要経費　計
					for( j = KFUDLINE5 ; j < KFUDLINE14 ; j++ ){
						if(j == KFUDLINE6 || j == KFUDLINE8 || j == KFUDLINE9){//損害保険料、減価償却費、借入金利子　は入力不可なのでチェックしない
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval5) ){
							st = 1;
							break;
						}
					}
					break;

				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval5) ){
						st = 1;	
					}
					break;
			}
		default:
			break;
	}
	return st;
}

// 手入力サインのクリア
int CShinFudousanKeika::ClearManualInputSign()
{
	int rt = 0;

	for( int idx = 0; idx < 50; idx++ ){
		(*m_pTaxListData)->NRec.KFval[idx].KKauto = 0x00;
	}

	return rt;
}