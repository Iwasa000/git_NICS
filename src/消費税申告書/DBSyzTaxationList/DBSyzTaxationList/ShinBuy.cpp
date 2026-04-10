// ShinBuy.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinBuy.h"
#include "ShinBuyIdx.h"

// CShinBuy ダイアログ

IMPLEMENT_DYNAMIC(CShinBuy, CSyzBaseDlg)

CShinBuy::CShinBuy(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinBuy::IDD, pParent)
	, m_pParent(pParent)
	, m_Idx(0)
	, m_Lin(0)
{
	m_curwnd = -1;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;
}

CShinBuy::~CShinBuy()
{
}

void CShinBuy::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Buy);
}


BEGIN_MESSAGE_MAP(CShinBuy, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinBuy メッセージ ハンドラ

BOOL CShinBuy::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Buy;
	m_BottomIdx = 26;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinBuy::Init()
{
	InitDiagAttr();

	DispList(0);//'14.09.01

	SetItemPosition();	// フォーカスセット'14.09.01


	return 0;
}

BOOL CShinBuy::PreTranslateMessage(MSG* pMsg)
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
void CShinBuy::DispList( int mode )
{
	m_Buy.OffControl( TRUE );
	DispBuyDiagData();
//	m_Buy.UpdateWindow();

	DIAGRAM_DATA	data;
	if( !mode ){
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, YEARID, TRUE, CIcsdiagctrl );
		data.data_disp.Format("（平成%2x年分）", (*m_pTaxListData)->YEAR);
		diag_setdata( IDC_ICSDIAGCTRL1, YEARID, &data, CIcsdiagctrl );

		PostMessage( WM_VSCROLL, SB_TOP, 0 );
		
		m_Idx = VMIN;

		//この処理の必要性がわからない？？理解後に処理を復活させる
	/*	if( (*m_pTaxListData)->GKBN & 0x04 ){//農業
			m_Idx = BUYVAL04;
		}
		else{
			if( (*m_pTaxListData)->GKBN & 0x02 ){//不動産
				m_Idx = BUYVAL07;
			}
			else{
				m_Idx = VMIN;
			}
		}
		*/
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

	if( mode ){
		if( m_Idx ==BUYVAL17K && !(*m_pTaxListData)->Head.KHstax ){
			m_Idx = BUYVAL17C;
		}
	}
	diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );

	//SetRedrawScrollToIdx(m_Buy.GetPosition());

}

//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
void CShinBuy::DispBuyDiagData()
{
	short	idx, i/*, ii*/;
	KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;
	DIAGRAM_DATA	data;
	int		st = 0;


	//'14.08.19
//	for( i = 0 ; i < LINCNT ; i++ ){//行数分回す
	for( i = 0 ; i < LINCNT - 1 ; i++ ){//行数分回す
	//	if( i == BUYLINE14 || i == BUYLINE15 || i == BUYLINE16){//課税仕入れに係る消費税額の計算の３行はスキップ
	//		continue;
	//	}

		st = 0;

		m_Util.DiagOcxIniz( &data );

		idx = i + VMIN;//インデックスをセット
		diag_clear( IDC_ICSDIAGCTRL1, idx, TRUE, CIcsdiagctrl );

/*		switch(i){
			case BUYLINE3://【事業所得に係る課税仕入高】営業等　差引課税仕入高（①－②）
				if( m_pArith->l_test((pK + BUYLINE1)->KKval1) || m_pArith->l_test((pK + BUYLINE2)->KKval1) ){
					st = 1;
				}
				if( (pK + BUYLINE1)->KKauto & 0x01 || (pK + BUYLINE2)->KKauto & 0x01 ){
					st = 1;
				}
				break;

			case BUYLINE6://【事業所得に係る課税仕入高】農業　差引課税仕入高（④－⑤）
				if( m_pArith->l_test((pK + BUYLINE4)->KKval1) || m_pArith->l_test((pK + BUYLINE5)->KKval1) ){
					st = 1;
				}
				if( (pK + BUYLINE4)->KKauto & 0x01 || (pK + BUYLINE5)->KKauto & 0x01 ){
					st = 1;
				}
				break;
		
			case BUYLINE9://【不動産所得に係る課税仕入高】差引課税仕入高（⑦－⑧）
				if( m_pArith->l_test((pK + BUYLINE7)->KKval1) || m_pArith->l_test((pK + BUYLINE8)->KKval1) ){
					st = 1;
				}
				if( (pK + BUYLINE7)->KKauto & 0x01 || (pK + BUYLINE8)->KKauto & 0x01 ){
					st = 1;
				}
				break;

			case BUYLINE12://【 [   ]所得に係る課税仕入高】差引課税仕入高（⑩－⑪）
				if( m_pArith->l_test((pK + BUYLINE10)->KKval1) || m_pArith->l_test((pK + BUYLINE11)->KKval1) ){
					st = 1;
				}
				break;

			case BUYLINE15://【業務用資産の取得に係る課税仕入高】差引課税仕入高（⑬－⑭）
				if( m_pArith->l_test((pK + BUYLINE13)->KKval1) || m_pArith->l_test((pK + BUYLINE14)->KKval1) ){
					st = 1;
				}
				break;

			case BUYLINE16://課税仕入高の合計額（③＋⑥＋⑨＋⑫＋⑮）
				if( m_pArith->l_test((pK + BUYLINE3)->KKval1) || m_pArith->l_test((pK + BUYLINE6)->KKval1) || m_pArith->l_test((pK + BUYLINE9)->KKval1) || 
					m_pArith->l_test((pK + BUYLINE12)->KKval1) || m_pArith->l_test((pK + BUYLINE15)->KKval1)  ){
						st = 1;
				}
				if( (pK + BUYLINE1)->KKauto&0x01 || (pK + BUYLINE2)->KKauto&0x01 ){
					st = 1;
				}
				if( (pK + BUYLINE4)->KKauto&0x01 || (pK + BUYLINE5)->KKauto&0x01 ){
					st = 1;
				}
				if( (pK + BUYLINE7)->KKauto&0x01 || (pK + BUYLINE8)->KKauto&0x01 ){
					st = 1;
				}
				break;

			case BUYLINE17://課税仕入れに係る消費税額の計算
				if( m_pArith->l_test((pK + BUYLINE16)->KKval1) ){
					st = 1;
				}
				if( (pK + BUYLINE17)->KKauto&0x03 ){
					st = 1;
				}
				break;

			default:
				if( m_pArith->l_test((pK + i)->KKval1) ){
					st = 1;
				}
				if( (pK + i)->KKauto&0x01 ){
					st = 1;
				}
				break;

		}*/

		st = DispJudge( 0, i );

		if( st ){
			memcpy( data.data_val, (pK + i)->KKval1, 6 );
			diag_setdata( IDC_ICSDIAGCTRL1, idx, &data, CIcsdiagctrl );
		}

	}

	// 項目名
	char	buf[128];
	::ZeroMemory( buf, sizeof(buf) );
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, BUYKNAME, TRUE, CIcsdiagctrl );

	CString KHksnmstr = m_Util.CutHankaku( (*m_pTaxListData)->Head.KHksnm, sizeof((*m_pTaxListData)->Head.KHksnm) );
	m_Util.LineDataSet( (LPSTR)(LPCSTR)KHksnmstr, buf, KHksnmstr.GetLength() );

	data.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, BUYKNAME, &data, CIcsdiagctrl );

	// 17	仮受消費税
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, BUYVAL17K, TRUE, CIcsdiagctrl );
	if( (*m_pTaxListData)->Head.KHstax ){
		if( m_pArith->l_test((pK + BUYLINE16)->KKval1) ){
			st = 1;
		}
		if( (pK + BUYLINE17)->KKauto&0x04 ){
			st = 1;
		}
	}
	if( st ){
		memcpy( data.data_val, (pK + BUYLINE17)->KKval3, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, BUYVAL17K, &data, CIcsdiagctrl );
	}

	// 17	計算部
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, BUYVAL17C, TRUE, CIcsdiagctrl );
/*	if( l_test((pK + BUYLINE16)->KKval1) ){
		st = 1;
	}*/
	st = DispJudge( 1, BUYLINE17 );
//	if( (pK + BUYLINE17)->KKauto&0x02 ){
//		st = 1;
//	}
	if( st ){
		memcpy( data.data_val, (pK + BUYLINE17)->KKval2, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, BUYVAL17C, &data, CIcsdiagctrl );
	}

	// 17　金額
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, BUYVAL17, TRUE, CIcsdiagctrl );
/*	if( l_test((pK + BUYLINE16)->KKval1) ){
		st = 1;
	}*/
	st = DispJudge( 0, BUYLINE17 );
	if( st ){
		memcpy( data.data_val, (pK + BUYLINE17)->KKval1, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, BUYVAL17, &data, CIcsdiagctrl );
	}


///////////背景色変更
	int cnt = 0;
	int Flg = 0;
	int line = -1;

	while( Buy[cnt].Index != 0 ){
		if(( Buy[cnt].Index == BUYVAL01  )||( Buy[cnt].Index == BUYVAL02  )||( Buy[cnt].Index == BUYVAL03 )||
		   ( Buy[cnt].Index == BUYVAL04  )||( Buy[cnt].Index == BUYVAL05  )||( Buy[cnt].Index == BUYVAL06 )||
	       ( Buy[cnt].Index == BUYVAL07  )||( Buy[cnt].Index == BUYVAL08  )||( Buy[cnt].Index == BUYVAL09 )||
		   ( Buy[cnt].Index == BUYVAL17K )||( Buy[cnt].Index == BUYVAL17C )||( Buy[cnt].Index == BUYVAL17 )){
			// 確定時は全入力不可
			if( (*m_pSnHeadData)->Sn_Sign4&0x01){
				Flg = Buy[cnt].AllUnConnect;
			}
			else{
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					Flg = Buy[cnt].UnConnect;
				}
				else{
					Flg = Buy[cnt].Connect;
				}
			}

			if( Flg ){
				DIAGRAM_ATTRIBUTE attr;
				diag_getattr( IDC_ICSDIAGCTRL1, Buy[cnt].Index, &attr, CIcsdiagctrl );
				
				switch( Buy[cnt].Index ){
					case BUYVAL01:
					case BUYVAL02:
					case BUYVAL03:
					case BUYVAL04:
					case BUYVAL05:
					case BUYVAL06:
					case BUYVAL07:
					case BUYVAL08:
					case BUYVAL09:
						switch( Buy[cnt].Index ){
							case BUYVAL01:
								line = BUYLINE1;
								break;
							case BUYVAL02:
								line = BUYLINE2;
								break;
							case BUYVAL03:
								line = BUYLINE3;
								break;
							case BUYVAL04:
								line = BUYLINE4;
								break;
							case BUYVAL05:
								line = BUYLINE5;
								break;
							case BUYVAL06:
								line = BUYLINE6;
								break;
							case BUYVAL07:
								line = BUYLINE7;
								break;
							case BUYVAL08:
								line = BUYLINE8;
								break;
							case BUYVAL09:
								line = BUYLINE9;
								break;
						}
						if( (*m_pTaxListData)->NRec.KSval[line].KKauto&0x01 ){
							attr.attr_bcolor = BC_BLUE;
						}else{
							attr.attr_bcolor = BC_CREAM;
						}

						break;
					case BUYVAL17K:
						if( (*m_pTaxListData)->Head.KHstax ){
							if( (*m_pTaxListData)->NRec.KSval[BUYLINE17].KKauto&0x04 ){
								attr.attr_bcolor = BC_BLUE;
							}else{
								attr.attr_bcolor = BC_CREAM;
							}
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					case BUYVAL17C:
						if( (*m_pTaxListData)->NRec.KSval[BUYLINE17].KKauto&0x02 ){
							attr.attr_bcolor = BC_BLUE;
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					case BUYVAL17:
						if( (*m_pTaxListData)->NRec.KSval[BUYLINE17].KKauto&0x01 ){
							attr.attr_bcolor = BC_BLUE;
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					default:
						break;
				}

				diag_setattr( IDC_ICSDIAGCTRL1, Buy[cnt].Index, &attr, FALSE, CIcsdiagctrl );
			}
		}
		cnt++;
	}
////////////



	/*
	for( i=0; i<VLIN; i++ ){
		if( (i==16) || (i==17) ){
			continue;
		}
		idx = i+VMIN;
		m_Util.DiagOcxIniz( &data );
		st = 0;
		ii = i;
		if( i == 18 ){
			ii = 16;
		}
		
		diag_clear( IDC_ICSDIAGCTRL1, idx, TRUE, CIcsdiagctrl );
		switch( ii ){
		case 2:
			if( m_pArith->l_test((pK+0)->KKval1) || m_pArith->l_test((pK+1)->KKval1) ){
				st = 1;
			}
			if( (pK+0)->KKauto&0x01 || (pK+1)->KKauto&0x01 ){
				st = 1;
			}
			break;
		case 5:
			if( m_pArith->l_test((pK+3)->KKval1) || m_pArith->l_test((pK+4)->KKval1) ){
				st = 1;
			}
			if( (pK+3)->KKauto&0x01 || (pK+4)->KKauto&0x01 ){
				st = 1;
			}
			break;
		case 8:
			if( m_pArith->l_test((pK+6)->KKval1) || m_pArith->l_test((pK+7)->KKval1) ){
				st = 1;
			}
			if( (pK+6)->KKauto&0x01 || (pK+7)->KKauto&0x01 ){
				st = 1;
			}
			break;
		case 11:
			if( m_pArith->l_test((pK+9)->KKval1) || m_pArith->l_test((pK+10)->KKval1) ){
				st = 1;
			}
			break;
		case 14:
			if( m_pArith->l_test((pK+12)->KKval1) || m_pArith->l_test((pK+13)->KKval1) ){
				st = 1;
			}
			break;
		case 15:
			if( m_pArith->l_test((pK+2)->KKval1) || m_pArith->l_test((pK+5)->KKval1) || m_pArith->l_test((pK+8)->KKval1) ||
				m_pArith->l_test((pK+11)->KKval1) || m_pArith->l_test((pK+14)->KKval1) ){
				st = 1;
			}
			if( (pK+0)->KKauto&0x01 || (pK+1)->KKauto&0x01 ){
				st = 1;
			}
			if( (pK+3)->KKauto&0x01 || (pK+4)->KKauto&0x01 ){
				st = 1;
			}
			if( (pK+6)->KKauto&0x01 || (pK+7)->KKauto&0x01 ){
				st = 1;
			}
			break;
		case 16:
			if( m_pArith->l_test((pK+15)->KKval1) ){
				st = 1;
			}
			if( (pK+ii)->KKauto&0x03 ){
				st = 1;
			}
			break;
		default:
			if( m_pArith->l_test((pK+ii)->KKval1) ){
				st = 1;
			}
			if( (pK+ii)->KKauto&0x01 ){
				st = 1;
			}
			break;
		}
		if( st ){
			memcpy( data.data_val, (pK+ii)->KKval1, 6 );
			diag_setdata( IDC_ICSDIAGCTRL1, idx, &data, CIcsdiagctrl );
		}
	}
	*/

	
/*

	// 項目名
	char	buf[128];
	::ZeroMemory( buf, sizeof(buf) );
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, BUYKNAME, TRUE, CIcsdiagctrl );

	CString KHksnmstr = m_Util.CutHankaku( (*m_pTaxListData)->Head.KHksnm, sizeof((*m_pTaxListData)->Head.KHksnm) );
	m_Util.LineDataSet( (LPSTR)(LPCSTR)KHksnmstr, buf, KHksnmstr.GetLength() );

	data.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, BUYKNAME, &data, CIcsdiagctrl );

	// 17	仮受消費税
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, BUYVAL17K, TRUE, CIcsdiagctrl );
	if( (*m_pTaxListData)->Head.KHstax ){
		if( m_pArith->l_test((pK+15)->KKval1) ){
			st = 1;
		}
		if( (pK+16)->KKauto&0x04 ){
			st = 1;
		}
	}
	if( st ){
		memcpy( data.data_val, (pK+16)->KKval3, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, BUYVAL17K, &data, CIcsdiagctrl );
	}

	// 17	計算部
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, BUYVAL17C, TRUE, CIcsdiagctrl );
	if( l_test((pK+15)->KKval1) ){
		st = 1;
	}
	if( (pK+16)->KKauto&0x02 ){
		st = 1;
	}
	if( st ){
		memcpy( data.data_val, (pK+16)->KKval2, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, BUYVAL17C, &data, CIcsdiagctrl );
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
int CShinBuy::GetDiagData( long index, short leng, DIAGRAM_DATA *pData, int mode )
{
	if( index < 0 ){
		return 0;
	}

	char	dbuf[128], buf[128];
	char	*buff;
	int		len;
	KKVAL*	pK;

	len = pData->data_edit.GetLength();
	memset( dbuf, '\0', sizeof(dbuf) );
	buff = (char *)((LPCTSTR)(pData->data_edit));
	memmove( dbuf, buff, len );

	if( m_Idx == BUYVAL17K ){
		if( !(*m_pTaxListData)->Head.KHstax&0x01 ){
			DispBuyDiagData();
			return 0;
		}
	}

	if( !mode ){
		if( m_Idx == BUYKNAME ){
			::ZeroMemory( buf, sizeof(buf));
			m_Util.LineDataGet( dbuf, buf, sizeof(dbuf) );
			memcpy( (*m_pTaxListData)->Head.KHksnm, buf, sizeof((*m_pTaxListData)->Head.KHksnm));
		}
		else{
			pK = &(*m_pTaxListData)->NRec.KSval[m_Lin];
			if( leng ){
				if( m_Idx == BUYVAL17K ){
					pK->KKauto |= 0x04;
					memcpy( pK->KKval3, pData->data_val, 6 );
				}
				else if( m_Idx == BUYVAL17C ){
					pK->KKauto |= 0x02;
					memcpy( pK->KKval2, pData->data_val, 6 );
				}
				else{
					switch( m_Lin ){
						case BUYLINE10:
						case BUYLINE11:
						case BUYLINE13:
						case BUYLINE14:
							if( m_pArith->l_test(pData->data_val) ){
								pK->KKauto |= 0x01;
							}else{
								pK->KKauto &= 0xfe;
							}
						break;
					default:
						pK->KKauto |= 0x01;
						break;
					}
					memcpy( pK->KKval1, pData->data_val, 6 );
				}
			}
		}
	}
	else{
		if( m_Idx == BUYKNAME ){
			::ZeroMemory( (*m_pTaxListData)->Head.KHksnm, 6 );
		}
		else{
			pK = &(*m_pTaxListData)->NRec.KSval[m_Lin];
			if( m_Idx == BUYVAL17K ){
				pK->KKauto &= 0xfb;
				::ZeroMemory( pK->KKval3, 6 );
			}
			else if( m_Idx == BUYVAL17C ){
				pK->KKauto &= 0xfd;
				::ZeroMemory( pK->KKval2, 6 );
			}
			else{
				pK->KKauto &= 0xfe;
				::ZeroMemory( pK->KKval1, 6 );
			}
		}
	}

	// 集計
	(*m_Scalc)->Shz_BuyPost();//'14.08.06
	DispBuyDiagData();

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
void CShinBuy::InitDiagAttr()
{
	int	cnt = 0;
	int	Flg;
	while( 1 ){
		if( Buy[cnt].Index == 0 ){
			break;
		}
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01){
			Flg = Buy[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Buy[cnt].UnConnect;
			}
			else{
				Flg = Buy[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL1, Buy[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Buy[cnt].Connect );
		cnt++;
	}

	SetFocusRange();

	m_Buy.EnableDelete(0);//DELETEキー１回で削除モード設定'14.08.01

}
BEGIN_EVENTSINK_MAP(CShinBuy, CSyzBaseDlg)
	ON_EVENT(CShinBuy, IDC_ICSDIAGCTRL1, 1, CShinBuy::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinBuy, IDC_ICSDIAGCTRL1, 2, CShinBuy::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinBuy, IDC_ICSDIAGCTRL1, 3, CShinBuy::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinBuy, IDC_ICSDIAGCTRL1, 9, CShinBuy::CheckButtonIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinBuy::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	
	//'14.08.01
	/*RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL1, index, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
	*/

	m_Buy.OffControl( FALSE );

	m_Idx = index;
	if( (m_Idx>=VMIN) && (m_Idx<=VMAX) ){
//		if( m_Idx == VMAX ){
//			m_Lin = m_Idx-VMIN-1;
//		}
		if(( m_Idx == BUYVAL17K )||( m_Idx == BUYVAL17C )||( m_Idx == BUYVAL17 )){
			m_Lin = BUYLINE17;
		}
		else{
			m_Lin = m_Idx-VMIN;
		}
	}
	else{
		m_Lin = -1;
	}

	m_curwnd = IDC_ICSDIAGCTRL1;//'14.08.01

}

void CShinBuy::EditOFFIcsdiagctrl1(short index)
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

void CShinBuy::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
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
//		if(index == BUYVAL17){
//		if(index == BUYVAL01){
//			m_Buy.SetPrevPosition();
//		}
//		else{
//			pos = UpDownFocusMoveCheck(index , 1);
			pos = FocusMoveCheck(index , 1);
			m_Buy.SetPosition(pos);	
//		}
	}
	if( wTerm==VK_DOWN ){
//		if(index == BUYVAL17){
//			m_Buy.SetNextPosition();
//		}
//		else{
//			pos = UpDownFocusMoveCheck(index , 0);
			pos = FocusMoveCheck(index , 0);
			m_Buy.SetPosition(pos);
//		}
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
//		m_Buy.SetNextPosition();
		pos = FocusMoveCheck(index , 2);
		m_Buy.SetPosition(pos);
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
//		m_Buy.SetPrevPosition();
		pos = FocusMoveCheck(index , 3);
		m_Buy.SetPosition(pos);
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

	SetRedrawScrollToIdx(m_Buy.GetPosition());

/*	int nowpos = m_Buy.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL1, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}*/
	
}

//'14.08.01------------------>
//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
//int CShinBuy::UpDownFocusMoveCheck(int Index , int Move)
int CShinBuy::FocusMoveCheck(int Index , int Move)
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
		for(i = 0 ; i < FCS_SHINBUYCNT ; i++){
			if(Index == FCS_ShinBuy[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinBuy[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SHINBUYCNT ; i++){
			if(Index == FCS_ShinBuy_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinBuy_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SHINBUYCNT ; j++){
			m_Buy.GetAttribute(FCS_ShinBuy[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinBuy[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Buy.GetAttribute(FCS_ShinBuy[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinBuy[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SHINBUYCNT ; j++){
			m_Buy.GetAttribute(FCS_ShinBuy_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinBuy_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Buy.GetAttribute(FCS_ShinBuy_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinBuy_LR[j].IDNo;
				break;
			}
		}
	}

	return idx;
}
//<-----------------------------------------

void CShinBuy::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SHINBUYCNT; idx++ ){
		m_Buy.GetAttribute(FCS_ShinBuy[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_ShinBuy[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SHINBUYCNT; idx++ ){
		m_Buy.GetAttribute(FCS_ShinBuy_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_ShinBuy_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINBUYCNT-1; idx > -1 ; idx-- ){
		m_Buy.GetAttribute(FCS_ShinBuy[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_ShinBuy[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINBUYCNT-1; idx > -1 ; idx-- ){
		m_Buy.GetAttribute(FCS_ShinBuy_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_ShinBuy_LR[idx].IDNo;
			break;
		}
	}
}

void CShinBuy::CheckButtonIcsdiagctrl1(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

//-----------------------------------------------------------------------------
// カーソル移動
//-----------------------------------------------------------------------------
// 引数	index	：	
//		flg		：	
//		nChar	：	
//-----------------------------------------------------------------------------
void CShinBuy::Diag1_Edit( short index, UINT flg, short nChar )
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
		case VK_RETURN:
		case VK_RIGHT:
			flg = REPLY_NEXT;
			switch( m_Idx ){
				case VMAX:
					flg = REPLY_STAY;
					if( nChar != VK_RETURN ){
						if( (*m_pTaxListData)->GKBN == 0x02 ){
							xpos = BUYVAL07;
						}
						else if( (*m_pTaxListData)->GKBN == 0x04 ){
							xpos = BUYVAL04;
						}
						else{
							xpos = VMIN;
						}
					}
					break;
				case BUYVAL02:
					if( (*m_pTaxListData)->GKBN == 0x01 ){
						flg = REPLY_STAY;
						xpos = BUYKNAME;
					}
					break;
				case BUYVAL05:
					if( !((*m_pTaxListData)->GKBN&0x02) ){
						flg = REPLY_STAY;
						xpos = BUYKNAME;
					}
					break;
				case BUYVAL08:
					flg = REPLY_STAY;
					xpos = BUYKNAME;
					break;
				case BUYKNAME:
					flg = REPLY_STAY;
					xpos = BUYVAL10;
					break;
				default:
					break;
			}
			break;
		case VK_LEFT:
			flg = REPLY_BACK;
			switch( m_Idx ){
				case VMIN:
					flg = REPLY_STAY;
					xpos = VMAX;
					break;
				case BUYVAL04:
					if( (*m_pTaxListData)->GKBN == 0x04 ){
						flg = REPLY_STAY;
						xpos = VMAX;
					}
					break;
				case BUYVAL07:
					if( (*m_pTaxListData)->GKBN == 0x02 ){
						flg = REPLY_STAY;
						xpos = VMAX;
					}
					break;
				case BUYVAL10:
					flg = REPLY_STAY;
					xpos = BUYKNAME;
					break;
				case BUYKNAME:
					flg = REPLY_STAY;
					if( (*m_pTaxListData)->GKBN == 0x01 ){
						xpos = BUYVAL02;
					}
					else if( (*m_pTaxListData)->GKBN == 0x04 ){
						xpos = BUYVAL05;
					}
					else{
						xpos = BUYVAL08;
					}
					break;
				default:
					break;
			}
			break;
		case VK_UP:
			flg = REPLY_BACK;
			switch( m_Idx ){
				case BUYKNAME:
				case BUYVAL17K:
				case BUYVAL17C:
				case VMIN:
					flg = REPLY_STAY;
					break;
				case BUYVAL04:
					if( (*m_pTaxListData)->GKBN == 0x04 ){
						flg = REPLY_STAY;
					}
					break;
				case BUYVAL07:
					if( (*m_pTaxListData)->GKBN == 0x02 ){
						flg = REPLY_STAY;	
					}
					break;
				case VMAX:
					flg = REPLY_STAY;
					xpos = BUYVAL14;
					break;
				default:
					break;
			}
			break;
		case VK_DOWN:
			flg = REPLY_STAY;
			if( (m_Idx==BUYKNAME) || (m_Idx==BUYVAL17K) || (m_Idx==BUYVAL17C) || (m_Idx==VMAX) ){
				;
			}
			else{
				if( m_Idx == BUYVAL14 ){
					xpos = VMAX;
				}
				else{
					flg = REPLY_NEXT;
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
#define		Msiz	512*20
//-----------------------------------------------------------------------------
// 印刷情報のセット
//-----------------------------------------------------------------------------
// 引数	pVprn	：	出力クラス
//		pPset	：	印刷情報
//-----------------------------------------------------------------------------
void CShinBuy::SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo )
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
int CShinBuy::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j, lin, max;
	char	cnm[128], buf[128], CVAL[6];
	int		len, st;
	KKVAL*	pK;
	KKVAL*	pS = (*m_pTaxListData)->NRec.KSval;

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

// 事業所得に係る課税売上高
	lin = 0;	max = 6;
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		switch( j+lin ){
			case 2:
				if( m_pArith->l_test((pS+0)->KKval1) || m_pArith->l_test((pS+1)->KKval1) ){
					st = 1;
				}
				if( (pS+0)->KKauto&0x01 || (pS+1)->KKauto&0x01 ){
					st = 1;
				}
				break;
			case 5:
				if( m_pArith->l_test((pS+3)->KKval1) || m_pArith->l_test((pS+4)->KKval1) ){
					st = 1;
				}
				if( (pS+3)->KKauto&0x01 || (pS+4)->KKauto&0x01 ){
					st = 1;
				}
				break;
			default:
				if( pK->KKauto&0x01 ){
					st = 1;
				}
				break;
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

// 不動産所得に係る課税売上高
	lin = 6;	max = 9;
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf));
/*		if( j+lin == 8 ){
			if( m_pArith->l_test((pS+6)->KKval1) || m_pArith->l_test((pS+7)->KKval1) ){
				st = 1;
			}
			if( (pS+6)->KKauto&0x01 || (pS+7)->KKauto&0x01 ){
				st = 1;
			}
		}
		else{
			if( pK->KKauto&0x01 ){
				st = 1;
			}
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

// (     )に係る課税売上高
	lin = 9;	max = 12;
	for( j=0; (j+lin) < max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 11 ){
			if( m_pArith->l_test((pS+9)->KKval1) || m_pArith->l_test((pS+10)->KKval1) ){
				st = 1;
			}
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

// 業務用資産の譲渡所得に係る課税売上高
	lin = 12;	max = 15;
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 14 ){
			if( m_pArith->l_test((pS+12)->KKval1) || m_pArith->l_test((pS+13)->KKval1) ){
				st = 1;
			}
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

// 課税売上高の合計
//	pK = &(*m_pTaxListData)->NRec.KSval[15];
	pK = &(*m_pTaxListData)->NRec.KSval[BUYLINE16];
	::ZeroMemory( buf, sizeof( buf ));
	st = 0;
/*	if( m_pArith->l_test((pS+2)->KKval1) || m_pArith->l_test((pS+5)->KKval1) || m_pArith->l_test((pS+8)->KKval1) ||
		m_pArith->l_test((pS+11)->KKval1) || m_pArith->l_test((pS+14)->KKval1) ){
		st = 1;
	}
	if( (pS+0)->KKauto&0x01 || (pS+1)->KKauto&0x01 ){
		st = 1;
	}
	if( (pS+3)->KKauto&0x01 || (pS+4)->KKauto&0x01 ){
		st = 1;
	}
	if( (pS+6)->KKauto&0x01 || (pS+7)->KKauto&0x01 ){
		st = 1;
	}*/
	if( DispJudge( 0, BUYLINE16 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

// 課税標準額の計算
	st = 0;
//	pK = &(*m_pTaxListData)->NRec.KSval[16];
	pK = &(*m_pTaxListData)->NRec.KSval[BUYLINE17];
/*	if( m_pArith->l_test((pS+15)->KKval1)){
		st = 1;
	}
	if( pK->KKauto&0x02 ){
		st = 1;
	}*/
	::ZeroMemory( buf, sizeof(buf) );
	if( DispJudge( 1, BUYLINE17 )){
		m_pArith->l_print( buf, pK->KKval2, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

	st = 0;
/*	if( m_pArith->l_test((pS+15)->KKval1)){
		st = 1;
	}
	if( pK->KKauto&0x03 ){
		st = 1;
	}*/
	::ZeroMemory( buf, sizeof( buf ));
	if( DispJudge( 0, BUYLINE17 )){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[0] = pBpMv;
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
int CShinBuy::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, len;
	char	buf[64], dbuf[64];

	i = 0;
	memset( pBp, '\0', Msiz );
//	BP1 = BP;	
	char	*pBpMv = pBp;

// 項目名
	::ZeroMemory( buf,sizeof(buf) );
	::ZeroMemory( dbuf,sizeof(dbuf) );
//	memmove( buf, (*m_pTaxListData)->Head.KHkunm, sizeof((*m_pTaxListData)->Head.KHkunm) );
	memmove( buf, (*m_pTaxListData)->Head.KHksnm, sizeof((*m_pTaxListData)->Head.KHksnm) );
	len = (int)strlen(buf);
	if( len > 6 ){
		memmove( pBpMv, buf, len );
		PF[i++].PITM[0] = pBpMv;
		pBpMv += (len+1);
//		memmove( pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHkunm));
		memmove( pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHksnm));
		PF[i].PITM[0] = pBpMv;
//		pBpMv += (sizeof((*m_pTaxListData)->Head.KHkunm)+1);
		pBpMv += (sizeof((*m_pTaxListData)->Head.KHksnm)+1);
	}
	else{
//		memmove( pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHkunm));
		memmove( pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHksnm));
		PF[i++].PITM[0] = pBpMv;
//		pBpMv += (sizeof((*m_pTaxListData)->Head.KHkunm)+1);
		pBpMv += (sizeof((*m_pTaxListData)->Head.KHksnm)+1);
		memmove( pBpMv, buf, len);
		PF[i].PITM[0] = pBpMv;	
		pBpMv += (len+1);
	}

	return( _xprn( pno, fno, PF, 0, pDC, 0 ));
}


//-----------------------------------------------------------------------------
// 初期ポジションセット ('14.08.08)
//-----------------------------------------------------------------------------
void CShinBuy::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
		if( (index = m_Buy.GetPosition()) != -1) {
			m_Buy.SetPosition(index);
			m_Buy.SetFocus();
			return;
		}
	}

	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Buy.SetPosition(BUYVAL01);
	}
	else{
		m_Buy.SetPosition(BUYVAL10);
	}
	SetRedrawScroll( 0 );

}

int CShinBuy::DispJudge( int clm, int line )
{

	int st = 0;
	KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;

	switch( line ){
		case BUYLINE1:
		case BUYLINE2:
		case BUYLINE4:
		case BUYLINE5:
		case BUYLINE7:
		case BUYLINE8:
			if( m_pArith->l_test((pK + line)->KKval1) ){
				st = 1;
			}
			if((pK + line)->KKauto & 0x01 ){
				st = 1;
			}
			break;

		case BUYLINE3://【事業所得に係る課税仕入高】営業等　差引課税仕入高（①－②）
			if( m_pArith->l_test((pK + BUYLINE1)->KKval1) || m_pArith->l_test((pK + BUYLINE2)->KKval1) ){
				st = 1;
			}
			if( (pK + BUYLINE1)->KKauto & 0x01 || (pK + BUYLINE2)->KKauto & 0x01 ){
				st = 1;
			}
			break;

		case BUYLINE6://【事業所得に係る課税仕入高】農業　差引課税仕入高（④－⑤）
			if( m_pArith->l_test((pK + BUYLINE4)->KKval1) || m_pArith->l_test((pK + BUYLINE5)->KKval1) ){
				st = 1;
			}
			if( (pK + BUYLINE4)->KKauto & 0x01 || (pK + BUYLINE5)->KKauto & 0x01 ){
				st = 1;
			}
			break;
	
		case BUYLINE9://【不動産所得に係る課税仕入高】差引課税仕入高（⑦－⑧）
			if( m_pArith->l_test((pK + BUYLINE7)->KKval1) || m_pArith->l_test((pK + BUYLINE8)->KKval1) ){
				st = 1;
			}
			if( (pK + BUYLINE7)->KKauto & 0x01 || (pK + BUYLINE8)->KKauto & 0x01 ){
				st = 1;
			}
			break;

		case BUYLINE12://【 [   ]所得に係る課税仕入高】差引課税仕入高（⑩－⑪）
			if( m_pArith->l_test((pK + BUYLINE10)->KKval1) || m_pArith->l_test((pK + BUYLINE11)->KKval1) ){
				st = 1;
			}
			break;

		case BUYLINE15://【業務用資産の取得に係る課税仕入高】差引課税仕入高（⑬－⑭）
			if( m_pArith->l_test((pK + BUYLINE13)->KKval1) || m_pArith->l_test((pK + BUYLINE14)->KKval1) ){
				st = 1;
			}
			break;

		case BUYLINE16://課税仕入高の合計額（③＋⑥＋⑨＋⑫＋⑮）
//			if( m_pArith->l_test((pK + BUYLINE3)->KKval1) || m_pArith->l_test((pK + BUYLINE6)->KKval1) || m_pArith->l_test((pK + BUYLINE9)->KKval1) || 
//				m_pArith->l_test((pK + BUYLINE12)->KKval1) || m_pArith->l_test((pK + BUYLINE15)->KKval1)  ){
			if( m_pArith->l_test((pK + BUYLINE1)->KKval1) || m_pArith->l_test((pK + BUYLINE2)->KKval1) ||
				m_pArith->l_test((pK + BUYLINE4)->KKval1) || m_pArith->l_test((pK + BUYLINE5)->KKval1) || 
				m_pArith->l_test((pK + BUYLINE7)->KKval1) || m_pArith->l_test((pK + BUYLINE8)->KKval1) ||
				m_pArith->l_test((pK + BUYLINE10)->KKval1) || m_pArith->l_test((pK + BUYLINE11)->KKval1) || 
				m_pArith->l_test((pK + BUYLINE13)->KKval1) || m_pArith->l_test((pK + BUYLINE14)->KKval1) ){
				st = 1;
			}
			if( (pK + BUYLINE1)->KKauto&0x01 || (pK + BUYLINE2)->KKauto&0x01 ){
				st = 1;
			}
			if( (pK + BUYLINE4)->KKauto&0x01 || (pK + BUYLINE5)->KKauto&0x01 ){
				st = 1;
			}
			if( (pK + BUYLINE7)->KKauto&0x01 || (pK + BUYLINE8)->KKauto&0x01 ){
				st = 1;
			}
			break;

		case BUYLINE17://課税仕入れに係る消費税額の計算
			if( clm == 0 ){ //金額
				if( m_pArith->l_test((pK + BUYLINE16)->KKval1) ){
					st = 1;
				}
//				if( (pK + BUYLINE16)->KKauto&0x01 ){
				if( (pK + BUYLINE17)->KKauto&0x03 ){
					st = 1;
				}
			}else if( clm == 1 ){ //計算部
				if( m_pArith->l_test((pK + BUYLINE16)->KKval1)){
					st = 1;
				}
//				if( (pK + BUYLINE16)->KKauto&0x02 ){
				if( (pK + BUYLINE17)->KKauto&0x02 ){
					st = 1;
				}
			}else if( clm == 2 ){ //仮払消費税等
				//画面表示のみなので保留
			}

//			if( (pK + BUYLINE17)->KKauto&0x03 ){
//				st = 1;
//			}
			break;

		default:
			if( m_pArith->l_test((pK + line)->KKval1) ){
				st = 1;
			}
			if( (pK + line)->KKauto&0x01 ){
				st = 1;
			}
			break;
	}

	return st;

}

// 手入力サインのクリア
int CShinBuy::ClearManualInputSign()
{
	int rt = 0;

	for( int idx = 0; idx < 50; idx++ ){
		(*m_pTaxListData)->NRec.KSval[idx].KKauto = 0x00;
	}

	//事業所得の金額をクリアする
	//金額クリア後に金額を連動してくる為
	for( int i=0; i<6; i++ ){
		memset( (*m_pTaxListData)->NRec.KSval[i].KKval1 , '\0', sizeof((*m_pTaxListData)->NRec.KSval[i].KKval1) );
	}

	return rt;
}