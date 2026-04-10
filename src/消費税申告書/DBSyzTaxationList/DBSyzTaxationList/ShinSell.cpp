// ShinSell.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSell.h"
#include "ShinSellIdx.h"

// CShinSell ダイアログ

IMPLEMENT_DYNAMIC(CShinSell, CSyzBaseDlg)

CShinSell::CShinSell(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinSell::IDD, pParent)
	, m_pParent(pParent)//'14.08.06
	, m_Idx(0)
	, m_Lin(0)
{
	m_curwnd = -1;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;
}

CShinSell::~CShinSell()
{
}

void CShinSell::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Sell);
}


BEGIN_MESSAGE_MAP(CShinSell, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinSell メッセージ ハンドラ

BOOL CShinSell::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Sell;
	m_BottomIdx = 91;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinSell::Init()
{
	InitDiagAttr();

	DispList(0);//'14.09.01

	SetItemPosition();	// フォーカスセット'14.09.01

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
void CShinSell::InitDiagAttr()
{
	int	cnt = 0;
	while( 1 ) {
		if( Sell[cnt].Index == 0 )	break;
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Sell[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Sell[cnt].UnConnect;
			}
			else{
				Flg = Sell[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL1, Sell[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Sell[cnt].Connect );
		cnt++;
	}

	SetFocusRange();

	m_Sell.EnableDelete(0);//DELETEキー１回で削除モード設定'14.08.04

}

BOOL CShinSell::PreTranslateMessage(MSG* pMsg)
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
void CShinSell::DispList( int mode )
{
	m_Sell.OffControl( TRUE );
	DispSellDiagData();

	DIAGRAM_DATA	data;
	if( !mode ){
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, YEARID, TRUE, CIcsdiagctrl );
		data.data_disp.Format( _T("（平成%2x年分）"), (*m_pTaxListData)->YEAR );
		diag_setdata( IDC_ICSDIAGCTRL1, YEARID, &data, CIcsdiagctrl );

		PostMessage( WM_VSCROLL, SB_TOP, 0 );

		m_Idx = VMIN;

		//この処理の必要性がわからない？？理解後に処理を復活させる
		/*
		if( (*m_pTaxListData)->GKBN & 0x04 ){
			m_Idx = SELVAL04;
		}
		else{
			if( (*m_pTaxListData)->GKBN & 0x02 ){
				m_Idx = SELVAL07;
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
		if( (m_Idx==SELVAL17K) && !(*m_pTaxListData)->Head.KHstax ){
			m_Idx = SELVAL17C;
		}
	}
	diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );

	//SetRedrawScrollToIdx(m_Sell.GetPosition());
}

//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
void CShinSell::DispSellDiagData()
{
	short			idx, i, ii;
	KKVAL*			pK = (*m_pTaxListData)->NRec.KUval;
	DIAGRAM_DATA	data;
	int		st;

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
/*		switch( ii ){
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
		}*/
		st = DispJudge( 0, i );
		if( st ){
			memcpy( data.data_val, (pK+ii)->KKval1, 6 );
			diag_setdata( IDC_ICSDIAGCTRL1, idx, &data, CIcsdiagctrl );
		}
	}

	// 項目名
	char	buf[128];
	::ZeroMemory( buf, sizeof(buf));
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, SELKNAME, TRUE, CIcsdiagctrl );

	CString KHkunmstr = m_Util.CutHankaku( (*m_pTaxListData)->Head.KHkunm, sizeof((*m_pTaxListData)->Head.KHkunm) );
	m_Util.LineDataSet( (LPSTR)(LPCSTR)KHkunmstr, buf, KHkunmstr.GetLength() );

	data.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, SELKNAME, &data, CIcsdiagctrl );


	// 17	仮受消費税
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, SELVAL17K, TRUE, CIcsdiagctrl );
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
		diag_setdata( IDC_ICSDIAGCTRL1, SELVAL17K, &data, CIcsdiagctrl );
	}

	// 17	計算部
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, SELVAL17C, TRUE, CIcsdiagctrl );
/*	if( m_pArith->l_test((pK+15)->KKval1) ){
		st = 1;
	}
	if( (pK+16)->KKauto&0x02 ){
		st = 1;
	}*/
	st = DispJudge( 1, SELLINE17 );
	if( st ){
		memcpy( data.data_val, (pK+16)->KKval2, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, SELVAL17C, &data, CIcsdiagctrl );
	}

	// 17　金額
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, SELVAL17, TRUE, CIcsdiagctrl );
/*	if( l_test((pK + BUYLINE16)->KKval1) ){
		st = 1;
	}*/
	st = DispJudge( 0, SELLINE17 );
	if( st ){
		memcpy( data.data_val, (pK+16)->KKval1, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, SELVAL17, &data, CIcsdiagctrl );
	}

///////////背景色変更
	int cnt = 0;
	int Flg = 0;
	int line = -1;

	while( Sell[cnt].Index != 0 ){
		if(( Sell[cnt].Index == SELVAL01  )||( Sell[cnt].Index == SELVAL02  )||( Sell[cnt].Index == SELVAL03 )||
		   ( Sell[cnt].Index == SELVAL04  )||( Sell[cnt].Index == SELVAL05  )||( Sell[cnt].Index == SELVAL06 )||
	       ( Sell[cnt].Index == SELVAL07  )||( Sell[cnt].Index == SELVAL08  )||( Sell[cnt].Index == SELVAL09 )||
		   ( Sell[cnt].Index == SELVAL13  )||( Sell[cnt].Index == SELVAL14  )||( Sell[cnt].Index == SELVAL15 )||
		   ( Sell[cnt].Index == SELVAL17K )||( Sell[cnt].Index == SELVAL17C )||( Sell[cnt].Index == SELVAL17 )){
			// 確定時は全入力不可
			if( (*m_pSnHeadData)->Sn_Sign4&0x01){
				Flg = Sell[cnt].AllUnConnect;
			}
			else{
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					Flg = Sell[cnt].UnConnect;
				}
				else{
					Flg = Sell[cnt].Connect;
				}
			}

			if( Flg ){
				DIAGRAM_ATTRIBUTE attr;
				diag_getattr( IDC_ICSDIAGCTRL1, Sell[cnt].Index, &attr, CIcsdiagctrl );
				
				switch( Sell[cnt].Index ){
					case SELVAL01:
					case SELVAL02:
					case SELVAL03:
					case SELVAL04:
					case SELVAL05:
					case SELVAL06:
					case SELVAL07:
					case SELVAL08:
					case SELVAL09:
						switch( Sell[cnt].Index ){
							case SELVAL01:
								line = SELLINE1;
								break;
							case SELVAL02:
								line = SELLINE2;
								break;
							case SELVAL03:
								line = SELLINE3;
								break;
							case SELVAL04:
								line = SELLINE4;
								break;
							case SELVAL05:
								line = SELLINE5;
								break;
							case SELVAL06:
								line = SELLINE6;
								break;
							case SELVAL07:
								line = SELLINE7;
								break;
							case SELVAL08:
								line = SELLINE8;
								break;
							case SELVAL09:
								line = SELLINE9;
								break;
						}
						if( (*m_pTaxListData)->NRec.KUval[line].KKauto&0x01 ){
							attr.attr_bcolor = BC_BLUE;
						}else{
							attr.attr_bcolor = BC_CREAM;
						}

						break;
					case SELVAL13:
					case SELVAL14:
					case SELVAL15:
						if( !( m_Util.IsUnConnectMst(m_pZmSub->zvol) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ) ){// 非連動ｄｂで簡易課税じゃ無い場合
							;
						}else{
							switch( Sell[cnt].Index ){
								case SELVAL13:
									line = SELLINE13;
									break;
								case SELVAL14:
									line = SELLINE14;
									break;
								case SELVAL15:
									line = SELLINE15;
									break;
								default:
									break;
							}
							if( (*m_pTaxListData)->NRec.KUval[line].KKauto&0x01 ){
								attr.attr_bcolor = BC_BLUE;
							}else{
								attr.attr_bcolor = BC_CREAM;
							}
						}

						break;
					case SELVAL17K:
						if( (*m_pTaxListData)->Head.KHstax ){
							if( (*m_pTaxListData)->NRec.KUval[SELLINE17].KKauto&0x04 ){
								attr.attr_bcolor = BC_BLUE;
							}else{
								attr.attr_bcolor = BC_CREAM;
							}
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					case SELVAL17C:
						if( (*m_pTaxListData)->NRec.KUval[SELLINE17].KKauto&0x02 ){
							attr.attr_bcolor = BC_BLUE;
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					case SELVAL17:
						if( (*m_pTaxListData)->NRec.KUval[SELLINE17].KKauto&0x01 ){
							attr.attr_bcolor = BC_BLUE;
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					default:
						break;
				}

				diag_setattr( IDC_ICSDIAGCTRL1, Sell[cnt].Index, &attr, FALSE, CIcsdiagctrl );
			}
		}
		cnt++;
	}
////////////
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
int CShinSell::GetDiagData( long index, short leng, DIAGRAM_DATA *pData, int mode )
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

	if( m_Idx == SELVAL17K ){
		if( !(*m_pTaxListData)->Head.KHstax&0x01 ){
			DispSellDiagData();
			return 0;
		}
	}

	if( !mode ){
		if( m_Idx == SELKNAME ){
			::ZeroMemory( buf, sizeof(buf));
			m_Util.LineDataGet( dbuf, buf, sizeof(dbuf) );
			memcpy( (*m_pTaxListData)->Head.KHkunm, buf, sizeof((*m_pTaxListData)->Head.KHkunm));
		}
		else{
			pK = &(*m_pTaxListData)->NRec.KUval[m_Lin];
			if( leng ){
				if( m_Idx == SELVAL17K ){
					pK->KKauto |= 0x04;
					memcpy( pK->KKval3, pData->data_val, 6 );
				}
				else if( m_Idx == SELVAL17C ){
					pK->KKauto |= 0x02;
					memcpy( pK->KKval2, pData->data_val, 6 );
				}
				else{
					switch( m_Lin ){
						case SELLINE10:
						case SELLINE11:
						case SELLINE13:
						case SELLINE14:
							if( m_pArith->l_test(pData->data_val) ){
								pK->KKauto |= 0x01;
							}else{
								pK->KKauto &= 0xfe;
							}

							if(( m_Lin == SELLINE13)||( m_Lin == SELLINE14)){
								if( !( m_Util.IsUnConnectMst(m_pZmSub->zvol) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ) ){// 非連動ｄｂで簡易課税じゃ無い場合
									;
								}else{
									pK->KKauto |= 0x01;
								}	
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
		if( m_Idx == SELKNAME )
			::ZeroMemory( (*m_pTaxListData)->Head.KHkunm, 6 );
		else{
			pK = &(*m_pTaxListData)->NRec.KUval[m_Lin];
			if( m_Idx == SELVAL17K ){
				pK->KKauto &= 0xfb;
				::ZeroMemory( pK->KKval3, 6 );
			}
			else if( m_Idx == SELVAL17C ){
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
	(*m_Scalc)->Shz_SalePost();//'14.08.06
	DispSellDiagData();
	return 0;
}

//-----------------------------------------------------------------------------
// カーソル移動
//-----------------------------------------------------------------------------
// 引数	index	：
//		flg		：
//		nChar	：
//-----------------------------------------------------------------------------
void CShinSell::Diag1_Edit( short index, UINT flg, short nChar )
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
					if(nChar != VK_RETURN){
						if( (*m_pTaxListData)->GKBN == 0x02 ){
							xpos = SELVAL07;
						}
						else if( (*m_pTaxListData)->GKBN == 0x04 ){
							xpos = SELVAL04;
						}
						else{
							xpos = VMIN;
						}
					}
					break;
				case SELVAL02:
					if( (*m_pTaxListData)->GKBN == 0x01 ){
						flg = REPLY_STAY;
						xpos = SELKNAME;
					}
					break;
				case SELVAL05:
					if( !((*m_pTaxListData)->GKBN & 0x02) ){
						flg = REPLY_STAY;
						xpos = SELKNAME;
					}
					break;
				case SELVAL08:
					flg = REPLY_STAY;
					xpos = SELKNAME;
					break;
				case SELKNAME:
					flg = REPLY_STAY;
					xpos = SELVAL10;
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
				case SELVAL04:
					if( (*m_pTaxListData)->GKBN == 0x04 ){
						flg = REPLY_STAY;
						xpos = VMAX;
					}
					break;
				case SELVAL07:
					if( (*m_pTaxListData)->GKBN == 0x02 ){
						flg = REPLY_STAY;
						xpos = VMAX;
					}
					break;
				case SELVAL10:
					flg = REPLY_STAY;
					xpos = SELKNAME;
					break;
				case SELKNAME:
					flg = REPLY_STAY;
					if( (*m_pTaxListData)->GKBN == 0x01 ){
						xpos = SELVAL02;
					}
					else if( (*m_pTaxListData)->GKBN == 0x04 ){
						xpos = SELVAL05;
					}
					else{
						xpos = SELVAL08;
					}
					break;
				default:
					break;
			}
			break;
		case VK_UP:
			flg = REPLY_BACK;
			switch( m_Idx ){
				case SELKNAME:
				case SELVAL17K: 
				case SELVAL17C: 
				case VMIN:
					flg = REPLY_STAY;
					break;
				case SELVAL04:
					if( (*m_pTaxListData)->GKBN == 0x04 ){
						flg = REPLY_STAY;	
					}
					break;
				case SELVAL07:
					if( (*m_pTaxListData)->GKBN == 0x02 ){
						flg = REPLY_STAY;	
					}
					break;
				case VMAX:
					flg = REPLY_STAY;	
					xpos = SELVAL14;
					break;
				default:	
					break;
			}
			break;
		case VK_DOWN:
			flg = REPLY_STAY;
			if( (m_Idx==SELKNAME) || (m_Idx==SELVAL17K) || (m_Idx==SELVAL17C) || (m_Idx==VMAX) ){
				;
			}
			else{
				if( m_Idx == SELVAL14 ){
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
}BEGIN_EVENTSINK_MAP(CShinSell, CSyzBaseDlg)
ON_EVENT(CShinSell, IDC_ICSDIAGCTRL1, 1, CShinSell::EditONIcsdiagctrl1, VTS_I2)
ON_EVENT(CShinSell, IDC_ICSDIAGCTRL1, 2, CShinSell::EditOFFIcsdiagctrl1, VTS_I2)
ON_EVENT(CShinSell, IDC_ICSDIAGCTRL1, 3, CShinSell::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
ON_EVENT(CShinSell, IDC_ICSDIAGCTRL1, 9, CShinSell::CheckButtonIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSell::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。


	/*'14.08.04
	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL1, index, &rectB, CIcsdiagctrl);
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
	*/

	m_Sell.OffControl( FALSE );//'14.08.06

	m_Idx = index;
	if( (m_Idx>=VMIN) && (m_Idx<=VMAX) ){
//		if( m_Idx == VMAX ){
//			m_Lin = m_Idx-VMIN-1;
//		}
		if(( m_Idx == SELVAL17K )||( m_Idx == SELVAL17C )||( m_Idx == SELVAL17 )){
			m_Lin = SELLINE17;
		}
		else{
			m_Lin = m_Idx-VMIN;
		}
	}
	else{
		m_Lin = -1;
	}
	
	m_curwnd = IDC_ICSDIAGCTRL1;//'14.08.04

}

void CShinSell::EditOFFIcsdiagctrl1(short index)
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

void CShinSell::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
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

	//フォーカス移動処理対応'14.08.04
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
//		if(index == SELVAL17){
//			m_Sell.SetPrevPosition();
//		}
//		else{
//			pos = UpDownFocusMoveCheck(index , 1);
			pos = FocusMoveCheck(index , 1);
			m_Sell.SetPosition(pos);	
//		}
	}
	if( wTerm==VK_DOWN ){
//		if(index == SELVAL17){
//			m_Sell.SetNextPosition();
//		}
//		else{
//			pos = UpDownFocusMoveCheck(index , 0);
			pos = FocusMoveCheck(index , 0);
			m_Sell.SetPosition(pos);
//		}
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
//		m_Sell.SetNextPosition();
		pos = FocusMoveCheck(index , 2);
		m_Sell.SetPosition(pos);	
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
//		m_Sell.SetPrevPosition();
		pos = FocusMoveCheck(index , 3);
		m_Sell.SetPosition(pos);	
	}

	//------->DELETEキー対応'14.08.06
	if( wTerm == VK_DELETE){
		GetDiagData( index, length, &xdata, -1 );
	}



	//-->Editonから移動

	SetRedrawScrollToIdx(m_Sell.GetPosition());

/*	int nowpos = m_Sell.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL1, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}*/
	//<----------------





	/*
	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;

	UINT	flg = 0;

	idata = (DIAGRAM_DATA *)data;
	m_Util.MoveDiagData( &xdata, idata );
	xdata.data_imgdata = NULL;

	switch( nChar ){
		case VK_RETURN:
			m_Sell.OffControl( TRUE );
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
			PostMessage(WM_VSCROLL, SB_TOP, 0 );
			return;
		case VK_NEXT:
			PostMessage(WM_VSCROLL, SB_BOTTOM, 0 );
			return;
	}
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		Diag1_Edit( index, flg, nChar );
	}
	else{
		if( (nChar==VK_RETURN) || (nChar==VK_F3) ){
			m_Sell.SetNextPosition();
		}
		// 前項目へ
		if( (nChar==VK_TAB) || (nChar==VK_LEFT) || (nChar==VK_F2) ){
			m_Sell.SetPrevPosition();
		}
	}

	*/
}

//'14.08.04------------------>
//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
//int CShinSell::UpDownFocusMoveCheck(int Index , int Move)
int CShinSell::FocusMoveCheck(int Index , int Move)
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
		for(i = 0 ; i < FCS_SHINSELLCNT ; i++){
			if(Index == FCS_ShinSell[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinSell[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SHINSELLCNT ; i++){
			if(Index == FCS_ShinSell_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinSell_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SHINSELLCNT ; j++){
			m_Sell.GetAttribute(FCS_ShinSell[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinSell[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Sell.GetAttribute(FCS_ShinSell[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinSell[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SHINSELLCNT ; j++){
			m_Sell.GetAttribute(FCS_ShinSell_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinSell_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Sell.GetAttribute(FCS_ShinSell_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinSell_LR[j].IDNo;
				break;
			}
		}
	}

	return idx;
}
//<-----------------------------------------

void CShinSell::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SHINSELLCNT; idx++ ){
		m_Sell.GetAttribute(FCS_ShinSell[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_ShinSell[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SHINSELLCNT; idx++ ){
		m_Sell.GetAttribute(FCS_ShinSell_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_ShinSell_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINSELLCNT-1; idx > -1 ; idx-- ){
		m_Sell.GetAttribute(FCS_ShinSell[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_ShinSell[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINSELLCNT-1; idx > -1 ; idx-- ){
		m_Sell.GetAttribute(FCS_ShinSell_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_ShinSell_LR[idx].IDNo;
			break;
		}
	}
}

void CShinSell::CheckButtonIcsdiagctrl1(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

#define		Msiz	512*20

//-----------------------------------------------------------------------------
// 印刷情報のセット
//-----------------------------------------------------------------------------
// 引数	pVprn	：	出力クラス
//		pPset	：	印刷情報
//-----------------------------------------------------------------------------
void CShinSell::SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo )
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
int CShinSell::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j, lin, max;
	char	cnm[128], buf[128], CVAL[6];
	int		len, st;
	KKVAL*	pK;
	KKVAL*	pU = (*m_pTaxListData)->NRec.KUval;

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
		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		switch( j+lin ){
			case 2:
				if( m_pArith->l_test((pU+0)->KKval1) || m_pArith->l_test((pU+1)->KKval1) ){
					st = 1;
				}
				if( (pU+0)->KKauto&0x01 || (pU+1)->KKauto&0x01 ){
					st = 1;
				}
				break;
			case 5:
				if( m_pArith->l_test((pU+3)->KKval1) || m_pArith->l_test((pU+4)->KKval1) ){
					st = 1;
				}
				if( (pU+3)->KKauto&0x01 || (pU+4)->KKauto&0x01 ){
					st = 1;
				}
				break;
			default:
				if( pK->KKauto&0x01 ){
					st = 1;
				}
				break;
		}*/
		if( DispJudge( 0 , j+lin ) == 1 ){
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
		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf));
/*		if( j+lin == 8 ){
			if( m_pArith->l_test((pU+6)->KKval1) || m_pArith->l_test((pU+7)->KKval1) ){
				st = 1;
			}
			if( (pU+6)->KKauto&0x01 || (pU+7)->KKauto&0x01 ){
				st = 1;
			}
		}
		else{
			if( pK->KKauto&0x01 ){
				st = 1;
			}
		}*/
		if( DispJudge( 0 , j+lin ) == 1 ){
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
		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 11 ){
			if( m_pArith->l_test((pU+9)->KKval1) || m_pArith->l_test((pU+10)->KKval1) ){
				st = 1;
			}
		}*/
		if( DispJudge( 0 , j+lin ) == 1 ){
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
		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 14 ){
			if( m_pArith->l_test((pU+12)->KKval1) || m_pArith->l_test((pU+13)->KKval1) ){
				st = 1;
			}
		}*/
		if( DispJudge( 0 , j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

// 課税売上高の合計
//	pK = &(*m_pTaxListData)->NRec.KUval[15];
	pK = &(*m_pTaxListData)->NRec.KUval[SELLINE16];
	::ZeroMemory( buf, sizeof( buf ));
	st = 0;
/*	if( m_pArith->l_test((pU+2)->KKval1) || m_pArith->l_test((pU+5)->KKval1) || m_pArith->l_test((pU+8)->KKval1) ||
		m_pArith->l_test((pU+11)->KKval1) || m_pArith->l_test((pU+14)->KKval1) ){
		st = 1;
	}
	if( (pU+0)->KKauto&0x01 || (pU+1)->KKauto&0x01 ){
		st = 1;
	}
	if( (pU+3)->KKauto&0x01 || (pU+4)->KKauto&0x01 ){
		st = 1;
	}
	if( (pU+6)->KKauto&0x01 || (pU+7)->KKauto&0x01 ){
		st = 1;
	}*/
	if( DispJudge( 0 , SELLINE16 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

// 課税標準額の計算
	st = 0;
//	pK = &(*m_pTaxListData)->NRec.KUval[16];
	pK = &(*m_pTaxListData)->NRec.KUval[SELLINE17];
/*	if( m_pArith->l_test((pU+15)->KKval1)){
		st = 1;
	}
	if( pK->KKauto&0x02 ){
		st = 1;
	}*/
	::ZeroMemory( buf, sizeof(buf) );
	if( DispJudge( 1 , SELLINE17 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval2, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

	st = 0;
//	if( m_pArith->l_test((pU+15)->KKval1)){
	if( m_pArith->l_test((pU+SELLINE16)->KKval1)){
		st = 1;
	}
	if( pK->KKauto&0x03 ){
		st = 1;
	}
	::ZeroMemory( buf, sizeof( buf ));
	if( DispJudge( 0 , SELLINE17 ) == 1 ){
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
int CShinSell::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
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
	memmove( buf, (*m_pTaxListData)->Head.KHkunm, sizeof((*m_pTaxListData)->Head.KHkunm) );
	len = (int)strlen(buf);
	if( len > 6 ){
		memmove( pBpMv, buf, len );
		PF[i++].PITM[0] = pBpMv;
		pBpMv += (len+1);
		memmove( pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHkunm));
		PF[i].PITM[0] = pBpMv;
		pBpMv += (sizeof((*m_pTaxListData)->Head.KHkunm)+1);
	}
	else{
		memmove( pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHkunm));
		PF[i++].PITM[0] = pBpMv;
		pBpMv += (sizeof((*m_pTaxListData)->Head.KHkunm)+1);
		memmove( pBpMv, buf, len);
		PF[i].PITM[0] = pBpMv;	
		pBpMv += (len+1);
	}

	return( _xprn( pno, fno, PF, 0, pDC, 0 ));
}


//-----------------------------------------------------------------------------
// 初期ポジションセット ('14.08.08)
//-----------------------------------------------------------------------------
void CShinSell::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
		if( (index = m_Sell.GetPosition()) != -1) {
			m_Sell.SetPosition(index);
			m_Sell.SetFocus();
			return;
		}
	}
	
	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Sell.SetPosition(SELVAL01);
	}
	else{
		m_Sell.SetPosition(SELVAL10);
	}
	SetRedrawScroll( 0 );

}

int CShinSell::DispJudge( int clm, int line )
{

	int st = 0;
	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;

	switch( line ){
		case SELLINE1:
		case SELLINE2:
		case SELLINE4:
		case SELLINE5:
		case SELLINE7:
		case SELLINE8:
			if( m_pArith->l_test((pK + line)->KKval1) ){
				st = 1;
			}
			if((pK + line)->KKauto & 0x01 ){
				st = 1;
			}
			break;

		case SELLINE3://【事業所得に係る課税仕入高】営業等　差引課税仕入高（①－②）
			if( m_pArith->l_test((pK + SELLINE1)->KKval1) || m_pArith->l_test((pK + SELLINE2)->KKval1) ){
				st = 1;
			}
			if( (pK + SELLINE1)->KKauto & 0x01 || (pK + SELLINE2)->KKauto & 0x01 ){
				st = 1;
			}
			break;

		case SELLINE6://【事業所得に係る課税仕入高】農業　差引課税仕入高（④－⑤）
			if( m_pArith->l_test((pK + SELLINE4)->KKval1) || m_pArith->l_test((pK + SELLINE5)->KKval1) ){
				st = 1;
			}
			if( (pK + SELLINE4)->KKauto & 0x01 || (pK + SELLINE5)->KKauto & 0x01 ){
				st = 1;
			}
			break;
	
		case SELLINE9://【不動産所得に係る課税仕入高】差引課税仕入高（⑦－⑧）
			if( m_pArith->l_test((pK + SELLINE7)->KKval1) || m_pArith->l_test((pK + SELLINE8)->KKval1) ){
				st = 1;
			}
			if( (pK + SELLINE7)->KKauto & 0x01 || (pK + SELLINE8)->KKauto & 0x01 ){
				st = 1;
			}
			break;

		case SELLINE12://【 [   ]所得に係る課税仕入高】差引課税仕入高（⑩－⑪）
			if( m_pArith->l_test((pK + SELLINE10)->KKval1) || m_pArith->l_test((pK + SELLINE11)->KKval1) ){
				st = 1;
			}
			break;

		case SELLINE15://【業務用資産の取得に係る課税仕入高】差引課税仕入高（⑬－⑭）
			if( m_pArith->l_test((pK + SELLINE13)->KKval1) || m_pArith->l_test((pK + SELLINE14)->KKval1) ){
				st = 1;
			}
			break;

		case SELLINE16://課税仕入高の合計額（③＋⑥＋⑨＋⑫＋⑮）
//			if( m_pArith->l_test((pK + SELLINE3)->KKval1) || m_pArith->l_test((pK + SELLINE6)->KKval1) || m_pArith->l_test((pK + SELLINE9)->KKval1) || 
//				m_pArith->l_test((pK + SELLINE12)->KKval1) || m_pArith->l_test((pK + SELLINE15)->KKval1)  ){
			if( m_pArith->l_test((pK + SELLINE1)->KKval1) || m_pArith->l_test((pK + SELLINE2)->KKval1) ||
				m_pArith->l_test((pK + SELLINE4)->KKval1) || m_pArith->l_test((pK + SELLINE5)->KKval1) ||
				m_pArith->l_test((pK + SELLINE7)->KKval1) || m_pArith->l_test((pK + SELLINE8)->KKval1) ||
				m_pArith->l_test((pK + SELLINE10)->KKval1) || m_pArith->l_test((pK + SELLINE11)->KKval1) ||
				m_pArith->l_test((pK + SELLINE13)->KKval1) || m_pArith->l_test((pK + SELLINE14)->KKval1) ){
				st = 1;
			}
			if( (pK + SELLINE1)->KKauto&0x01 || (pK + SELLINE2)->KKauto&0x01 ){
				st = 1;
			}
			if( (pK + SELLINE4)->KKauto&0x01 || (pK + SELLINE5)->KKauto&0x01 ){
				st = 1;
			}
			if( (pK + SELLINE7)->KKauto&0x01 || (pK + SELLINE8)->KKauto&0x01 ){
				st = 1;
			}
			break;

		case SELLINE17://課税仕入れに係る消費税額の計算
			if( clm == 0 ){ //金額
				if( m_pArith->l_test((pK + SELLINE16)->KKval1) ){
					st = 1;
				}
//				if( (pK + SELLINE17)->KKauto&0x01 ){
				if( (pK + SELLINE17)->KKauto&0x03 ){
					st = 1;
				}
			}else if( clm == 1 ){ //計算部
//				if( m_pArith->l_test((pK + SELLINE16)->KKval2)){
				if( m_pArith->l_test((pK + SELLINE16)->KKval1)){
					st = 1;
				}
				if( (pK + SELLINE17)->KKauto&0x02 ){
					st = 1;
				}
			}else if( clm == 2 ){ //仮払消費税等
				if( (*m_pTaxListData)->Head.KHstax ){
					if( m_pArith->l_test((pK + SELLINE16)->KKval1) ){
						st = 1;
					}
					if( (pK + SELLINE17)->KKauto&0x04 ){
						st = 1;
					}
				}
			}

//			if( (pK + SELLINE17)->KKauto&0x03 ){
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
int CShinSell::ClearManualInputSign()
{
	int rt = 0;

	for( int idx = 0; idx < 40; idx++ ){
		(*m_pTaxListData)->NRec.KUval[idx].KKauto = 0x00;
	}

	//事業所得の金額をクリアする
	//金額クリア後に金額を連動してくる為
	for( int i=0; i<6; i++ ){	// 何基準？
		memset( (*m_pTaxListData)->NRec.KUval[i].KKval1 , '\0', sizeof((*m_pTaxListData)->NRec.KUval[i].KKval1) );
	}

	return rt;
}