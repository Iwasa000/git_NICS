// ShinSellKeika.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSellKeika.h"
#include "ShinSellKeikaIdx.h"

#define	fno1_s 3	//フィールド１の小さいフォントフィールド番号


// CShinSellKeika ダイアログ

IMPLEMENT_DYNAMIC(CShinSellKeika, CSyzBaseDlg)

CShinSellKeika::CShinSellKeika(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinSellKeika::IDD, pParent)
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

CShinSellKeika::~CShinSellKeika()
{
}

void CShinSellKeika::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_SellKei);
}


BEGIN_MESSAGE_MAP(CShinSellKeika, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinSellKeika メッセージ ハンドラ

BOOL CShinSellKeika::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください


	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_SellKei;
	m_BottomIdx = 21;//コントロールの一番下のIDX指定するとスクロールがそこまでいく
//	m_BottomIdx = 122;//コントロールの一番下のIDX指定するとスクロールがそこまでいく

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinSellKeika::Init()
{
	InitDiagAttr();

	DispList(0);//'14.09.01

	SetItemPosition();	// フォーカスセット'14.09.01

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
void CShinSellKeika::InitDiagAttr()
{
	int	cnt = 0;
	while( 1 ) {
		if( SellKeika[cnt].Index == 0 )	break;
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = SellKeika[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = SellKeika[cnt].UnConnect;
			}
			else{
				Flg = SellKeika[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL1, SellKeika[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, SellKeika[cnt].Connect );
		cnt++;
	}

	SetFocusRange();

	m_SellKei.EnableDelete(0);//DELETEキー１回で削除モード設定'14.08.04

}
BOOL CShinSellKeika::PreTranslateMessage(MSG* pMsg)
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
void CShinSellKeika::DispList( int mode )
{
	m_SellKei.OffControl( TRUE );

	DispSellKeikaDiagData();

	DIAGRAM_DATA	data;
	if( !mode ){
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, SKYEARID, TRUE, CIcsdiagctrl );
		data.data_disp.Format( _T("（平成%2x年分）"), (*m_pTaxListData)->YEAR );
		diag_setdata( IDC_ICSDIAGCTRL1, SKYEARID, &data, CIcsdiagctrl );

		PostMessage( WM_VSCROLL, SB_TOP, 0 );

		//'14.09.01
		m_Idx = SKVMIN;

		//この処理の必要性がわからない？？理解後に処理を復活させる
		/*
		if( (*m_pTaxListData)->GKBN & 0x04 ){
			m_Idx = KSELVAL04A;
		}
		else{
			if( (*m_pTaxListData)->GKBN & 0x02 ){
				m_Idx = KSELVAL07A;
			}
			else{
				m_Idx = SKVMIN;
			}
		}
		*/

	}

	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, SKZKBNID, TRUE, CIcsdiagctrl );
	data.data_disp = _T("経理方式：");
	if( (*m_pTaxListData)->Head.KHstax&0x01 ){
		data.data_disp += _T("税抜き");
	}
	else{
		data.data_disp += _T("税込み");
	}
	diag_setdata( IDC_ICSDIAGCTRL1, SKZKBNID, &data, CIcsdiagctrl );

	if( mode ){
		if( (m_Idx==KSELVAL14K) && !(*m_pTaxListData)->Head.KHstax ){
			m_Idx = KSELVAL14C;
		}
	}
	diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );

	//SetRedrawScrollToIdx(m_SellKei.GetPosition());
}

//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
void CShinSellKeika::DispSellKeikaDiagData()
{
	short			idx, i/*, ii*/;
	KKVAL*			pK = (*m_pTaxListData)->NRec.KUval;
	DIAGRAM_DATA	data;
	int		st = 0;
	short	j = 0;

	//'14.08.20
	for( i = 0 ; i < SKLINCNT ; i++ ){

		//農業の部分スキップ
		if(i == 3 || i == 4 || i == 5){
			continue;
		}

		for(j = 0 ; j < 3 ; j++){

			st = 0;

			if(j > 0){
				if( i == KSELLINE1 || i == KSELLINE2 || i == KSELLINE4  || i == KSELLINE5  || i == KSELLINE7  || i == KSELLINE8 ||
					i == KSELLINE10 || i == KSELLINE11 || i == KSELLINE16)
				{
					continue;
				}
			}

			m_Util.DiagOcxIniz( &data );

			switch(i){

				case KSELLINE1:
					idx = KSELVAL01A + j;//インデックスをセット
					break;
				case KSELLINE2:
					idx = KSELVAL02A + j;//インデックスをセット
					break;
				case KSELLINE3:
					idx = KSELVAL03A + j;//インデックスをセット
					break;
				case KSELLINE4:
					idx = KSELVAL04A + j;//インデックスをセット
					break;
				case KSELLINE5:
					idx = KSELVAL05A + j;//インデックスをセット
					break;
				case KSELLINE6:
					idx = KSELVAL06A + j;//インデックスをセット
					break;
				case KSELLINE7:
					idx = KSELVAL07A + j;//インデックスをセット
					break;
				case KSELLINE8:
					idx = KSELVAL08A + j;//インデックスをセット
					break;
				case KSELLINE9:
					idx = KSELVAL09A + j;//インデックスをセット
					break;
				case KSELLINE10:
					idx = KSELVAL10A + j;//インデックスをセット
					break;
				case KSELLINE11:
					idx = KSELVAL11A + j;//インデックスをセット
					break;
				case KSELLINE12:
					idx = KSELVAL12A + j;//インデックスをセット
					break;
				case KSELLINE13:
					idx = KSELVAL13A + j;//インデックスをセット
					break;
				case KSELLINE16:
					idx = KSELVAL16 + j;//インデックスをセット
					break;

				case KSELLINE14:
					switch(j){
						case 0:
							idx = KSELVAL14;
							break;
						case 1:
							idx = KSELVAL14C;
							break;
						case 2:
							idx = KSELVAL14K;
							break;
					}
					break;
				case KSELLINE15:
					switch(j){
						case 0:
							idx = KSELVAL15;
							break;
						case 1:
							idx = KSELVAL15C;
							break;
						case 2:
							idx = KSELVAL15K;
							break;
					}
					break;
				default:
					idx = (i * 3) + j + SKVMIN;//インデックスをセット
					break;
			}
			diag_clear( IDC_ICSDIAGCTRL1, idx, TRUE, CIcsdiagctrl );

/*			switch(i){
				case KSELLINE3://【事業所得に係る課税売上高】差引課税売上高（①－②）
					if( j == 0 ){
						if( (pK + KSELLINE1)->KKauto & 0x01 || (pK + KSELLINE2)->KKauto & 0x01 ){
							st = 1;
						}
					}else if( j == 1 ){
						if( (pK + KSELLINE3)->KKauto & 0x02 ){
							st = 1;
						}
					}else if( j == 2 ){
						if( (pK + KSELLINE3)->KKauto & 0x04 ){
							st = 1;
						}
					}
					break;

				case KSELLINE6://【不動産所得に係る課税売上高】農業　差引課税売上高（④－⑤）
					if( j == 0 ){
						if( (pK + KSELLINE4)->KKauto & 0x01 || (pK + KSELLINE5)->KKauto & 0x01 ){
							st = 1;
						}
					}else if( j == 1 ){
						if( (pK + KSELLINE6)->KKauto & 0x02 ){
							st = 1;
						}
					}else if( j == 2 ){
						if( (pK + KSELLINE6)->KKauto & 0x04 ){
							st = 1;
						}
					}
					break;

				case KSELLINE9://【 [   ]所得に係る課税売上高】差引課税売上高（⑦－⑧）
					if( j == 0 ){
						if( (pK + KSELLINE7)->KKauto & 0x01 || (pK + KSELLINE8)->KKauto & 0x01 ){
							st = 1;
						}
					}else if( j == 1 ){
						if( (pK + KSELLINE9)->KKauto & 0x02 ){
							st = 1;
						}
					}else if( j == 2 ){
						if( (pK + KSELLINE9)->KKauto & 0x04 ){
							st = 1;
						}
					}
					break;

				case KSELLINE12://【業務用資産の取得に係る課税売上高】差引課税売上高（⑩－⑪）
					if( j == 0 ){
						if( (pK + KSELLINE10)->KKauto & 0x01 || (pK + KSELLINE11)->KKauto & 0x01 ){
							st = 1;
						}
					}else if( j == 1 ){
						if( (pK + KSELLINE12)->KKauto & 0x02 ){
							st = 1;
						}
					}else if( j == 2 ){
						if( (pK + KSELLINE12)->KKauto & 0x04 ){
							st = 1;
						}
					}
					break;

				case KSELLINE13://課税売上高の合計額（③＋⑥＋⑨＋⑫）
					if( j == 0 ){
						if( (pK + KSELLINE1)->KKauto&0x01 || (pK + KSELLINE2)->KKauto&0x01 ){
							st = 1;
						}
						if( (pK + KSELLINE4)->KKauto&0x01 || (pK + KSELLINE5)->KKauto&0x01 ){
							st = 1;
						}
						if( (pK + KSELLINE7)->KKauto&0x01 || (pK + KSELLINE8)->KKauto&0x01 ){
							st = 1;
						}
						if( (pK + KSELLINE10)->KKauto&0x01 || (pK + KSELLINE11)->KKauto&0x01 ){
							st = 1;
						}
					}else if( j == 1 ){
						if( (pK + KSELLINE3)->KKauto&0x02 || (pK + KSELLINE6)->KKauto&0x02  || (pK + KSELLINE9)->KKauto&0x02 || (pK + KSELLINE12)->KKauto&0x02 ){
							st = 1;
						}
					}else if( j == 2 ){
						if( (pK + KSELLINE3)->KKauto&0x04 || (pK + KSELLINE6)->KKauto&0x04  || (pK + KSELLINE9)->KKauto&0x04 || (pK + KSELLINE12)->KKauto&0x04 ){
							st = 1;
						}
					}
					break;

				case KSELLINE14://課税標準額の計算(⑬B欄の金額)
					if( m_pArith->l_test((pK + KSELLINE13)->KKval1) ){
						st = 1;
					}
					if( (pK + KSELLINE14)->KKauto&0x03 ){
						st = 1;
					}
					break;

				case KSELLINE15://課税標準額の計算(⑬C欄の金額)
					if( m_pArith->l_test((pK +KSELLINE13)->KKval1) ){
						st = 1;
					}
					if( (pK + KSELLINE15)->KKauto&0x03 ){
						st = 1;
					}
					break;

				case KSELLINE16://合計（⑭＋⑮）
					if( m_pArith->l_test((pK + KSELLINE14)->KKval1) || m_pArith->l_test((pK + KSELLINE15)->KKval1) ){
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

			st = DispJudge( j, i );

			if( st ){
				if(j == 0)		memcpy( data.data_val, (pK + i)->KKval1, 6 );
				else if(j == 1)	memcpy( data.data_val, (pK + i)->KKval2, 6 );
				else if(j == 2)	memcpy( data.data_val, (pK + i)->KKval3, 6 );

				diag_setdata( IDC_ICSDIAGCTRL1, idx, &data, CIcsdiagctrl );
			}
		}
	}

	// 項目名
	char	buf[128];
	::ZeroMemory( buf, sizeof(buf) );
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KSELKNAME, TRUE, CIcsdiagctrl );

	CString KHkunmstr = m_Util.CutHankaku( (*m_pTaxListData)->Head.KHkunm, sizeof((*m_pTaxListData)->Head.KHkunm) );
	m_Util.LineDataSet( (LPSTR)(LPCSTR)KHkunmstr, buf, KHkunmstr.GetLength() );

	data.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, KSELKNAME, &data, CIcsdiagctrl );



	// 14	仮受消費税
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KSELVAL14K, TRUE, CIcsdiagctrl );
	if( (*m_pTaxListData)->Head.KHstax ){
		if( m_pArith->l_test((pK + KSELLINE13)->KKval2) ){
			st = 1;
		}
		if( (pK + KSELLINE14)->KKauto&0x04 ){
			st = 1;
		}
	}
	if( st ){
		memcpy( data.data_val, (pK + KSELLINE14)->KKval3, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, KSELVAL14K, &data, CIcsdiagctrl );
	}

	// 14	計算部
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KSELVAL14C, TRUE, CIcsdiagctrl );
	if( m_pArith->l_test((pK + KSELLINE13)->KKval2) ){
		st = 1;
	}
	if( (pK + KSELLINE14)->KKauto&0x02 ){
		st = 1;
	}
	if( st ){
		memcpy( data.data_val, (pK + KSELLINE14)->KKval2, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, KSELVAL14C, &data, CIcsdiagctrl );
	}

	// 15	仮受消費税
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KSELVAL15K, TRUE, CIcsdiagctrl );
	if( (*m_pTaxListData)->Head.KHstax ){
		if( m_pArith->l_test((pK + KSELLINE13)->KKval3) ){
			st = 1;
		}
		if( (pK + KSELLINE15)->KKauto&0x04 ){
			st = 1;
		}
	}
	if( st ){
		memcpy( data.data_val, (pK + KSELLINE15)->KKval3, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, KSELVAL15K, &data, CIcsdiagctrl );
	}

	// 15	計算部
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KSELVAL15C, TRUE, CIcsdiagctrl );
	if( m_pArith->l_test((pK + KSELLINE13)->KKval3) ){
		st = 1;
	}
	if( (pK + KSELLINE15)->KKauto&0x02 ){
		st = 1;
	}
	if( st ){
		memcpy( data.data_val, (pK + KSELLINE15)->KKval2, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, KSELVAL15C, &data, CIcsdiagctrl );
	}

///////////背景色変更
	int cnt = 0;
	int Flg = 0;
	int line = -1;
	int colm = -1;
	unsigned char sgn = 0x00;

	while( SellKeika[cnt].Index != 0 ){
		
		line = GetLineNum( SellKeika[cnt].Index );
		colm = GetColumNum( SellKeika[cnt].Index );
		sgn = 0x00;
		switch(colm){
			case 0:
				sgn = 0x01;
				break;
			case 1:
				sgn = 0x02;
				break;
			case 2:
				sgn = 0x04;
				break;
			default:
				break;
		}


		if(( line == KSELLINE1 )||( line == KSELLINE2 )||( line == KSELLINE3 )||
		   ( line == KSELLINE4 )||( line == KSELLINE5 )||( line == KSELLINE6 )||
		   ( line == KSELLINE10 )||( line == KSELLINE11 )||( line == KSELLINE12 )||
		   ( line == KSELLINE14 )||( line == KSELLINE15 )){

			// 確定時は全入力不可
			if( (*m_pSnHeadData)->Sn_Sign4&0x01){
				Flg = SellKeika[cnt].AllUnConnect;
			}
			else{
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					Flg = SellKeika[cnt].UnConnect;
				}
				else{
					Flg = SellKeika[cnt].Connect;
				}
			}

			if( Flg ){
				DIAGRAM_ATTRIBUTE attr;
				diag_getattr( IDC_ICSDIAGCTRL1, SellKeika[cnt].Index, &attr, CIcsdiagctrl );

				switch( SellKeika[cnt].Index ){
					case KSELVAL01A:
					case KSELVAL01B:
					case KSELVAL01C:
					case KSELVAL02A:
					case KSELVAL02B:
					case KSELVAL02C:
					case KSELVAL03A:
					case KSELVAL03B:
					case KSELVAL03C:
					case KSELVAL04A:
					case KSELVAL04B:
					case KSELVAL04C:
					case KSELVAL05A:
					case KSELVAL05B:
					case KSELVAL05C:
					case KSELVAL06A:
					case KSELVAL06B:
					case KSELVAL06C:
					case KSELVAL14C:
					case KSELVAL14:
					case KSELVAL15C:
					case KSELVAL15:
						if( (*m_pTaxListData)->NRec.KUval[line].KKauto&sgn ){
							attr.attr_bcolor = BC_BLUE;
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					case KSELVAL10A:
					case KSELVAL10B:
					case KSELVAL10C:
					case KSELVAL11A:
					case KSELVAL11B:
					case KSELVAL11C:
					case KSELVAL12A:
					case KSELVAL12B:
					case KSELVAL12C:
						if( !( m_Util.IsUnConnectMst(m_pZmSub->zvol) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ) ){// 非連動ｄｂで簡易課税じゃ無い場合
							;
						}else{
							if( (*m_pTaxListData)->NRec.KUval[line].KKauto&sgn ){
								attr.attr_bcolor = BC_BLUE;
							}else{
								attr.attr_bcolor = BC_CREAM;
							}
						}
						break;
					case KSELVAL14K:
					case KSELVAL15K:
						if( (*m_pTaxListData)->Head.KHstax ){
							if( (*m_pTaxListData)->NRec.KUval[line].KKauto&sgn ){
								attr.attr_bcolor = BC_BLUE;
							}else{
								attr.attr_bcolor = BC_CREAM;
							}
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					default:
						break;
				}

				diag_setattr( IDC_ICSDIAGCTRL1, SellKeika[cnt].Index, &attr, FALSE, CIcsdiagctrl );
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

	// 項目名
	char	buf[128];
	::ZeroMemory( buf, sizeof(buf));
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KSELKNAME, TRUE, CIcsdiagctrl );

	CString KHkunmstr = m_Util.CutHankaku( (*m_pTaxListData)->Head.KHkunm, sizeof((*m_pTaxListData)->Head.KHkunm) );
	m_Util.LineDataSet( (LPSTR)(LPCSTR)KHkunmstr, buf, KHkunmstr.GetLength() );

	data.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, KSELKNAME, &data, CIcsdiagctrl );


	// 17	仮受消費税
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KSELVAL14K, TRUE, CIcsdiagctrl );
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
		diag_setdata( IDC_ICSDIAGCTRL1, KSELVAL14K, &data, CIcsdiagctrl );
	}

	// 17	計算部
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KSELVAL14C, TRUE, CIcsdiagctrl );
	if( m_pArith->l_test((pK+15)->KKval1) ){
		st = 1;
	}
	if( (pK+16)->KKauto&0x02 ){
		st = 1;
	}
	if( st ){
		memcpy( data.data_val, (pK+16)->KKval2, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, KSELVAL14C, &data, CIcsdiagctrl );
	}
	*/
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
int CShinSellKeika::GetDiagData( long index, short leng, DIAGRAM_DATA *pData, int mode )
{
	if( index < 0 ){
		return 0;
	}

	char	dbuf[128], buf[128];
	char	*buff;
	int		len;
	KKVAL*	pK;
	int		Lin_tmp = -1;

	len = pData->data_edit.GetLength();
	memset( dbuf, '\0', sizeof(dbuf) );
	buff = (char *)((LPCTSTR)(pData->data_edit));
	memmove( dbuf, buff, len );

	if(( m_Idx == KSELVAL14K )||( m_Idx == KSELVAL15K )){
		if( !(*m_pTaxListData)->Head.KHstax&0x01 ){
			DispSellKeikaDiagData();
			return 0;
		}
	}

	//'14.08.20
	int clm_pos = 0;
	if(( m_Idx == KSELVAL14K )||( m_Idx == KSELVAL15K )){
		clm_pos = 2;
	}else if(( m_Idx == KSELVAL14C )||( m_Idx == KSELVAL15C )){
		clm_pos = 1;
	}else if(( m_Idx == KSELVAL14 )||( m_Idx == KSELVAL15 )){
		clm_pos = 0;
	}else{
		clm_pos = (m_Idx - SKVMIN) % 3;
	}

	if(m_Lin == 0)			Lin_tmp = KSELLINE1;
	else if(m_Lin == 1)		Lin_tmp = KSELLINE2;
	else if(m_Lin == 2)		Lin_tmp = KSELLINE3;
	else if(m_Lin == 3)		Lin_tmp = KSELLINE4;
	else if(m_Lin == 4)		Lin_tmp = KSELLINE5;
	else if(m_Lin == 5)		Lin_tmp = KSELLINE6;
	else if(m_Lin == 6)		Lin_tmp = KSELLINE7;
	else if(m_Lin == 7)		Lin_tmp = KSELLINE8;
	else if(m_Lin == 8)		Lin_tmp = KSELLINE9;
	else if(m_Lin == 9)		Lin_tmp = KSELLINE10;
	else if(m_Lin == 10)	Lin_tmp = KSELLINE11;
	else if(m_Lin == 11)	Lin_tmp = KSELLINE12;
	else if(m_Lin == 12)	Lin_tmp = KSELLINE13;
	else if(m_Lin == 13)	Lin_tmp = KSELLINE14;
	else if(m_Lin == 14)	Lin_tmp = KSELLINE15;
	else if(m_Lin == 15)	Lin_tmp = KSELLINE16;

	if( !mode ){
		if( m_Idx == KSELKNAME ){
			::ZeroMemory( buf, sizeof(buf));
			m_Util.LineDataGet( dbuf, buf, sizeof(dbuf) );
			memcpy( (*m_pTaxListData)->Head.KHkunm, buf, sizeof((*m_pTaxListData)->Head.KHkunm));
		}
		else{
//			pK = &(*m_pTaxListData)->NRec.KUval[m_Lin];
			pK = &(*m_pTaxListData)->NRec.KUval[Lin_tmp];
			if( leng ){
/*				if( m_Idx == KSELVAL14K ){
					if( m_pArith->l_test(pData->data_val) ){
						pK->KKauto |= 0x04;
					}else{
						pK->KKauto &= 0xfb;
					}
					memcpy( pK->KKval3, pData->data_val, 6 );
				}
				else if( m_Idx == KSELVAL14C ){
					if( m_pArith->l_test(pData->data_val) ){
						pK->KKauto |= 0x02;
					}else{
						pK->KKauto &= 0xfd;
					}
					memcpy( pK->KKval2, pData->data_val, 6 );
				}
				else if( m_Idx == KSELVAL15K ){
					if( m_pArith->l_test(pData->data_val) ){
						pK->KKauto |= 0x04;
					}else{
						pK->KKauto &= 0xfb;
					}
					memcpy( pK->KKval3, pData->data_val, 6 );
				}
				else if( m_Idx == KSELVAL15C ){
					if( m_pArith->l_test(pData->data_val) ){
						pK->KKauto |= 0x02;
					*else{
						pK->KKauto &= 0xfd;
					}
					memcpy( pK->KKval2, pData->data_val, 6 );
				}
				else{*/
//					switch( m_Lin ){
					switch( Lin_tmp ){
					case KSELLINE1:
					case KSELLINE2:
					case KSELLINE3:
					case KSELLINE4:
					case KSELLINE5:
					case KSELLINE6:
					case KSELLINE14:
					case KSELLINE15:
						if(clm_pos == 0){
							pK->KKauto |= 0x01;
						}else
						if(clm_pos == 1){
							pK->KKauto |= 0x02;
						}else
						if(clm_pos == 2){
							pK->KKauto |= 0x04;
						}
						break;

					default:
						if( m_pArith->l_test(pData->data_val) ){
							if(clm_pos == 0){
								pK->KKauto |= 0x01;
							}else
							if(clm_pos == 1){
								pK->KKauto |= 0x02;
							}else
							if(clm_pos == 2){
								pK->KKauto |= 0x04;
							}
						}else{
							if(clm_pos == 0){
								pK->KKauto &= 0xfe;
							}else
							if(clm_pos == 1){
								pK->KKauto &= 0xfd;
							}else
							if(clm_pos == 2){
								pK->KKauto &= 0xfb;
							}
						}
						break;
					}

					if( !( m_Util.IsUnConnectMst(m_pZmSub->zvol) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ) ){// 非連動ｄｂで簡易課税じゃ無い場合
						;
					}else{
						switch( Lin_tmp ){
							case KSELLINE10:
							case KSELLINE11:
							case KSELLINE12:
								if(clm_pos == 0){
									pK->KKauto |= 0x01;
								}else
								if(clm_pos == 1){
									pK->KKauto |= 0x02;
								}else
								if(clm_pos == 2){
									pK->KKauto |= 0x04;
								}
								break;
							default:
								break;
						}
					}

					if(clm_pos == 0)		memcpy( pK->KKval1, pData->data_val, 6 );
					else if(clm_pos == 1)	memcpy( pK->KKval2, pData->data_val, 6 );
					else if(clm_pos == 2)	memcpy( pK->KKval3, pData->data_val, 6 );

//					memcpy( pK->KKval1, pData->data_val, 6 );
//				}
			}
		}
	}
	else{
		if( m_Idx == KSELKNAME ){
			::ZeroMemory( (*m_pTaxListData)->Head.KHkunm, 6 );
		}
		else{
//			pK = &(*m_pTaxListData)->NRec.KUval[m_Lin];
			pK = &(*m_pTaxListData)->NRec.KUval[Lin_tmp];
			if( m_Idx == KSELVAL14K ){
				pK->KKauto &= 0xfb;
				::ZeroMemory( pK->KKval3, 6 );
			}
			else if( m_Idx == KSELVAL14C ){
				pK->KKauto &= 0xfd;
				::ZeroMemory( pK->KKval2, 6 );
			}
			else if( m_Idx == KSELVAL15K ){
				pK->KKauto &= 0xfb;
				::ZeroMemory( pK->KKval3, 6 );
			}
			else if( m_Idx == KSELVAL15C ){
				pK->KKauto &= 0xfd;
				::ZeroMemory( pK->KKval2, 6 );
			}
			else{
				if(clm_pos == 0){
					pK->KKauto &= 0xfe;
					::ZeroMemory( pK->KKval1, 6 );
				}else
				if(clm_pos == 1){
					pK->KKauto &= 0xfd;
					::ZeroMemory( pK->KKval2, 6 );
				}else
				if(clm_pos == 2){
					pK->KKauto &= 0xfb;
					::ZeroMemory( pK->KKval3, 6 );
				}
			}
		}
	}

	// 集計
	(*m_Scalc)->Shz_SaleKeiPost();//'14.08.20
//	(*m_Scalc)->Shz_SalePost();//'14.08.06
	DispSellKeikaDiagData();
	return 0;
}


BEGIN_EVENTSINK_MAP(CShinSellKeika, CSyzBaseDlg)
	ON_EVENT(CShinSellKeika, IDC_ICSDIAGCTRL1, 1, CShinSellKeika::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinSellKeika, IDC_ICSDIAGCTRL1, 2, CShinSellKeika::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinSellKeika, IDC_ICSDIAGCTRL1, 3, CShinSellKeika::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSellKeika::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_SellKei.OffControl( FALSE );

	m_Idx = index;
	if( (m_Idx >= SKVMIN) && (m_Idx <= SKVMAX) ){
		if( m_Idx == SKVMAX ){
			m_Lin = ( (m_Idx - SKVMIN) / 3 ) - 1;
//			m_Lin = m_Idx-VMIN-1;
		}
		else{
			m_Lin = (m_Idx - SKVMIN) / 3;
//			m_Lin = m_Idx-VMIN;
		}
	}
	else{
		m_Lin = -1;
	}

	m_curwnd = IDC_ICSDIAGCTRL1;//'14.08.01
}

void CShinSellKeika::EditOFFIcsdiagctrl1(short index)
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

void CShinSellKeika::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
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
	//	if(index == KSELVAL16){
	//		m_SellKei.SetPrevPosition();
	//	}
	//	else{
//			pos = UpDownFocusMoveCheck(index , 1);
			pos = FocusMoveCheck(index , 1);
			m_SellKei.SetPosition(pos);	
	//	}
	}
	if( wTerm==VK_DOWN ){
	//	if(index == KSELVAL16){
	//		m_SellKei.SetNextPosition();
	//	}
	//	else{
//			pos = UpDownFocusMoveCheck(index , 0);
			pos = FocusMoveCheck(index , 0);
			m_SellKei.SetPosition(pos);
	//	}
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
//		m_SellKei.SetNextPosition();
		pos = FocusMoveCheck(index , 2);
		m_SellKei.SetPosition(pos);	
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
//		m_SellKei.SetPrevPosition();
		pos = FocusMoveCheck(index , 3);
		m_SellKei.SetPosition(pos);	
	}

	//------->DELETEキー対応'14.08.06
	if( wTerm == VK_DELETE){
		GetDiagData( index, length, &xdata, -1 );
	}

	SetRedrawScrollToIdx(m_SellKei.GetPosition());

/*	int nowpos = m_SellKei.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL1, nowpos, &rectB, CIcsdiagctrl );
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
//int CShinSellKeika::UpDownFocusMoveCheck(int Index , int Move)
int CShinSellKeika::FocusMoveCheck(int Index , int Move)
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
		for(i = 0 ; i < FCS_SHINSELLKEIKACNT ; i++){
			if(Index == FCS_ShinSellKeika[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinSellKeika[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SHINSELLKEIKACNT ; i++){
			if(Index == FCS_ShinSellKeika_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinSellKeika_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SHINSELLKEIKACNT ; j++){
			m_SellKei.GetAttribute(FCS_ShinSellKeika[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinSellKeika[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_SellKei.GetAttribute(FCS_ShinSellKeika[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinSellKeika[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SHINSELLKEIKACNT ; j++){
			m_SellKei.GetAttribute(FCS_ShinSellKeika_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinSellKeika_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_SellKei.GetAttribute(FCS_ShinSellKeika_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinSellKeika_LR[j].IDNo;
				break;
			}
		}
	}
	return idx;
}
//<-----------------------------------------

void CShinSellKeika::SetFocusRange()
{
	
	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SHINSELLKEIKACNT; idx++ ){
		m_SellKei.GetAttribute(FCS_ShinSellKeika[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_ShinSellKeika[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SHINSELLKEIKACNT; idx++ ){
		m_SellKei.GetAttribute(FCS_ShinSellKeika_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_ShinSellKeika_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINSELLKEIKACNT-1; idx > -1 ; idx-- ){
		m_SellKei.GetAttribute(FCS_ShinSellKeika[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd =FCS_ShinSellKeika[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINSELLKEIKACNT-1; idx > -1 ; idx-- ){
		m_SellKei.GetAttribute(FCS_ShinSellKeika_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd =FCS_ShinSellKeika_LR[idx].IDNo;
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
void CShinSellKeika::SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo )
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
int CShinSellKeika::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j;
	char	cnm[128], buf[128], CVAL[6];
	int		len;
	KKVAL*	pS = (*m_pTaxListData)->NRec.KUval;

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
//int CShinSellKeika::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
int CShinSellKeika::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j, lin, max;
	char	/*cnm[128], */buf[128], CVAL[6];
	int		len, st;
	KKVAL*	pK;
	KKVAL*	pS = (*m_pTaxListData)->NRec.KUval;

	// 初期化
	memset( PF, '\0', sizeof(PF) );


	i = 0;
	memset( pBp, '\0', Msiz );
	char	*pBpMv = pBp;
//	pBpMv = BP;
	::ZeroMemory( CVAL, sizeof(CVAL) );

// 会社名
	::ZeroMemory( buf, sizeof(buf) );
//	::ZeroMemory( cnm, sizeof(cnm) );

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
	pBpMv += ( 2+1 );
	*/

// 事業所得に係る課税仕入高
// 金額
	lin = 0;	max = 3;
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j;

//		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
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
//		if( DispJudge( 0, j+lin ) == 1 ){
		if( DispJudge( 0, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

//うち税率４％適用分
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j;

//		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
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
		}
		m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );
*/
//		if( DispJudge( 1, j+lin ) == 1 ){
		if( DispJudge( 1, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

//うち税率６．３％適用分
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j;

//		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
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
		}
		m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );
*/
//		if( DispJudge( 2, j+lin ) == 1 ){
		if( DispJudge( 2, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

// 不動産所得に係る課税売上高
// 金額
	lin = 3;	max = 6;
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j+3;

//		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
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
//		if( DispJudge( 0, j+lin ) == 1 ){
		if( DispJudge( 0, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

//うち税率４％適用分
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j+3;

//		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
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
		}
		m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );
*/
//		if( DispJudge( 1, j+lin ) == 1 ){
		if( DispJudge( 1, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

//うち税率６．３％適用分
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j+3;

//		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
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
		}
		m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );
*/
//		if( DispJudge( 2, j+lin ) == 1 ){
		if( DispJudge( 2, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;


// (     )に係る課税売上高
// 金額
	lin = 6;	max = 9;
	for( j=0; (j+lin) < max; j++ ){
		int l_idx = -1;
		l_idx = j+3;

//		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 11 ){
			if( m_pArith->l_test((pS+9)->KKval1) || m_pArith->l_test((pS+10)->KKval1) ){
				st = 1;
			}
		}*/
//		if( DispJudge( 0, j+lin ) == 1 ){
		if( DispJudge( 0, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

//うち税率４％適用分
	for( j=0; (j+lin) < max; j++ ){
		int l_idx = -1;
		l_idx = j+3;

//		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 11 ){
			if( m_pArith->l_test((pS+9)->KKval1) || m_pArith->l_test((pS+10)->KKval1) ){
				st = 1;
			}
		}
		m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );
*/
//		if( DispJudge( 1, j+lin ) == 1 ){
		if( DispJudge( 1, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

//うち税率６．３％適用分
	for( j=0; (j+lin) < max; j++ ){
		int l_idx = -1;
		l_idx = j+3;

//		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 11 ){
			if( m_pArith->l_test((pS+9)->KKval1) || m_pArith->l_test((pS+10)->KKval1) ){
				st = 1;
			}
		}
		m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );
*/
//		if( DispJudge( 2, j+lin ) == 1 ){
		if( DispJudge( 2, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;


// 業務用資産の譲渡所得に係る課税売上高
// 金額
	lin = 9;	max = 12;
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j+3;

//		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 14 ){
			if( m_pArith->l_test((pS+12)->KKval1) || m_pArith->l_test((pS+13)->KKval1) ){
				st = 1;
			}
		}*/
//		if( DispJudge( 0, j+lin ) == 1 ){
		if( DispJudge( 0, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

//うち税率４％適用分
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j+3;

//		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 14 ){
			if( m_pArith->l_test((pS+12)->KKval1) || m_pArith->l_test((pS+13)->KKval1) ){
				st = 1;
			}
		}
		m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );
*/
///		if( DispJudge( 1, j+lin ) == 1 ){
		if( DispJudge( 1, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

//うち税率６．３％適用分
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j+3;

//		pK = &(*m_pTaxListData)->NRec.KUval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 14 ){
			if( m_pArith->l_test((pS+12)->KKval1) || m_pArith->l_test((pS+13)->KKval1) ){
				st = 1;
			}
		}
		m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );
*/
//		if( DispJudge( 2, j+lin ) == 1 ){
		if( DispJudge( 2, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

// 課税売上高の合計
// 金額
//	pK = &(*m_pTaxListData)->NRec.KUval[12];
	pK = &(*m_pTaxListData)->NRec.KUval[KSELLINE13];
	::ZeroMemory( buf, sizeof( buf ));
/*	st = 0;
	if( m_pArith->l_test((pS+2)->KKval1) || m_pArith->l_test((pS+5)->KKval1) || m_pArith->l_test((pS+8)->KKval1) ||
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
	if( DispJudge( 0, KSELLINE13 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

//うち税率４％適用分
//	pK = &(*m_pTaxListData)->NRec.KUval[12];
	pK = &(*m_pTaxListData)->NRec.KUval[KSELLINE13];
	::ZeroMemory( buf, sizeof( buf ));
/*	st = 0;
	if( m_pArith->l_test((pS+2)->KKval1) || m_pArith->l_test((pS+5)->KKval1) || m_pArith->l_test((pS+8)->KKval1) ||
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
	}
	m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );
*/
	if( DispJudge( 1, KSELLINE13 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval2, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

//うち税率６．３％適用分
//	pK = &(*m_pTaxListData)->NRec.KUval[12];
	pK = &(*m_pTaxListData)->NRec.KUval[KSELLINE13];
	::ZeroMemory( buf, sizeof( buf ));
/*	st = 0;
	if( m_pArith->l_test((pS+2)->KKval1) || m_pArith->l_test((pS+5)->KKval1) || m_pArith->l_test((pS+8)->KKval1) ||
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
	}
	m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );
*/
	if( DispJudge( 2, KSELLINE13 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval3, FMTX130 );
	}

	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );


// 課税標準額の計算
// B欄の金額
	st = 0;
//	pK = &(*m_pTaxListData)->NRec.KUval[13];
	pK = &(*m_pTaxListData)->NRec.KUval[KSELLINE14];
/*	if( m_pArith->l_test((pS+15)->KKval1)){
		st = 1;
	}
	if( pK->KKauto&0x02 ){
		st = 1;
	}*/
	::ZeroMemory( buf, sizeof(buf) );
//	m_pArith->l_print( buf, pK->KKval2, st?FMTX130:FMTX13 );
	if( DispJudge( 0, KSELLINE14 ) == 1 ){ //列？
		m_pArith->l_print( buf, pK->KKval2, FMTX130 );
	}

	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

/*	st = 0;
	if( m_pArith->l_test((pS+15)->KKval1)){
		st = 1;
	}
	if( pK->KKauto&0x03 ){
		st = 1;
	}*/
	::ZeroMemory( buf, sizeof( buf ));
//	m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );
	if( DispJudge( 0, KSELLINE14 ) == 1 ){ //列？
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}

	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

// C欄の金額
	st = 0;
//	pK = &(*m_pTaxListData)->NRec.KUval[14];
	pK = &(*m_pTaxListData)->NRec.KUval[KSELLINE15];
/*	if( m_pArith->l_test((pS+15)->KKval1)){
		st = 1;
	}
	if( pK->KKauto&0x02 ){
		st = 1;
	}*/
	::ZeroMemory( buf, sizeof(buf) );
//	m_pArith->l_print( buf, pK->KKval2, st?FMTX130:FMTX13 );
	if( DispJudge( 0, KSELLINE15 ) == 1 ){ //列？
		m_pArith->l_print( buf, pK->KKval2, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

/*	st = 0;
	if( m_pArith->l_test((pS+15)->KKval1)){
		st = 1;
	}
	if( pK->KKauto&0x03 ){
		st = 1;
	}*/
	::ZeroMemory( buf, sizeof( buf ));
//	m_pArith->l_print( buf, pK->KKval3, st?FMTX130:FMTX13 );
	if( DispJudge( 0, KSELLINE15 ) == 1 ){ //列？
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}

	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

// 合計
//	pK = &(*m_pTaxListData)->NRec.KUval[15];
	pK = &(*m_pTaxListData)->NRec.KUval[KSELLINE16];
	::ZeroMemory( buf, sizeof( buf ));
/*	st = 0;
	if( m_pArith->l_test((pS+2)->KKval1) || m_pArith->l_test((pS+5)->KKval1) || m_pArith->l_test((pS+8)->KKval1) ||
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
	}
	m_pArith->l_print( buf, pK->KKval1, st?FMTX130:FMTX13 );*/
	if( DispJudge( 0, KSELLINE16 ) == 1 ){ //列？
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}

	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[0] = pBpMv;
	pBpMv += ( len+1 );


	int fontsize = 0;
	char val[5][128];
	CString str;
	for(int l_cnt = KSELLINE1; l_cnt <= KSELLINE16; l_cnt++){
		switch(l_cnt){
			case KSELLINE1:
			case KSELLINE2:
			case KSELLINE3:
			case KSELLINE4:
			case KSELLINE5:
			case KSELLINE6:
			case KSELLINE7:
			case KSELLINE8:
			case KSELLINE9:
			case KSELLINE10:
			case KSELLINE11:
			case KSELLINE12:
			case KSELLINE13:
//			case KSELLINE14:
//			case KSELLINE15:
//			case KSELLINE16:
				for(int idx = 0;idx<5;idx++){
					::ZeroMemory( val[idx], sizeof( val[idx] ));
				}
				m_pArith->l_print( val[0],(*m_pTaxListData)->NRec.KUval[l_cnt].KKval1, FMTX130 );
				m_pArith->l_print( val[1],(*m_pTaxListData)->NRec.KUval[l_cnt].KKval2, FMTX130 );
				m_pArith->l_print( val[2],(*m_pTaxListData)->NRec.KUval[l_cnt].KKval3, FMTX130 );
				m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KUval[l_cnt].KKval4, FMTX130 );
				m_pArith->l_print( val[4],(*m_pTaxListData)->NRec.KUval[l_cnt].KKval5, FMTX130 );
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
		return _xprn( pno, fno1_s, PF, 0, pDC, 0, 0 );
//		return _xprn( pno, 3  , PF, 0, pDC, 0, 1 );
	}

	return _xprn( pno, fno, PF, 0, pDC, 0, 0 );
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
//int CShinSellKeika::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
int CShinSellKeika::PrintHyo3( int pno, int fno, CDC *pDC, char *pBp )
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

//	return( _xprn( pno, fno, PF, 0, pDC, 0 ));
	return( _xprn( pno, 4, PF, 0, pDC, 0 ));
}


//-----------------------------------------------------------------------------
// 初期ポジションセット ('14.08.08)
//-----------------------------------------------------------------------------
void CShinSellKeika::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
		if( (index = m_SellKei.GetPosition()) != -1) {
			m_SellKei.SetPosition(index);
			m_SellKei.SetFocus();
			return;
		}
	}

	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_SellKei.SetPosition(KSELVAL01A);
	}
	else{
		m_SellKei.SetPosition(KSELVAL07A);
	}
	SetRedrawScroll( 0 );

}

int CShinSellKeika::DispJudge( int clm, int line )
{

	int st = 0;
	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;

	if( clm > 0 ){
		if( line == KSELLINE1 || line == KSELLINE2 || line == KSELLINE4  || line == KSELLINE5  || line == KSELLINE7  || line == KSELLINE8 ||
			line == KSELLINE10 || line == KSELLINE11 || line == KSELLINE16 )
		{
			return st;
		}
	}


	switch( line ){
		case KSELLINE3://【事業所得に係る課税売上高】差引課税売上高（①－②）

			//【事業所得に係る課税売上高】計部分は所得の方から金額転記されることがあり、手入力立たないので金額もチェック'14.10.08
			if( clm == 0 ){
				if( (pK + KSELLINE1)->KKauto & 0x01 || (pK + KSELLINE2)->KKauto & 0x01 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KSELLINE1)->KKval1) ||  m_pArith->l_test((pK + KSELLINE2)->KKval1)){
					st = 1;
				}
			}else if( clm == 1 ){
				if( (pK + KSELLINE3)->KKauto & 0x02 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KSELLINE3)->KKval2) ){
					st = 1;
				}
			}else if( clm == 2 ){
				if( (pK + KSELLINE3)->KKauto & 0x04 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KSELLINE3)->KKval3) ){
					st = 1;
				}
			}
			
			break;
					
		case KSELLINE6://【不動産所得に係る課税売上高】農業　差引課税売上高（④－⑤）

			//【不動産所得に係る課税売上高】計部分は所得の方から金額転記されることがあり、手入力立たないので金額もチェック'14.10.08
			if( clm == 0 ){
				if( (pK + KSELLINE4)->KKauto & 0x01 || (pK + KSELLINE5)->KKauto & 0x01 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KSELLINE4)->KKval1) ||  m_pArith->l_test((pK + KSELLINE5)->KKval1)){
					st = 1;
				}
			}else if( clm == 1 ){
				if( (pK + KSELLINE6)->KKauto & 0x02 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KSELLINE6)->KKval2) ){
					st = 1;
				}
			}else if( clm == 2 ){
				if( (pK + KSELLINE6)->KKauto & 0x04 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KSELLINE6)->KKval3) ){
					st = 1;
				}
			}
			break;

		case KSELLINE9://【 [   ]所得に係る課税売上高】差引課税売上高（⑦－⑧）
			if( clm == 0 ){
				if( m_pArith->l_test((pK + KSELLINE7)->KKval1) || m_pArith->l_test((pK + KSELLINE8)->KKval1) ){
					st = 1;
				}
				if( (pK + KSELLINE7)->KKauto & 0x01 || (pK + KSELLINE8)->KKauto & 0x01 ){
					st = 1;
				}
			}else if( clm == 1 ){
				if( m_pArith->l_test((pK + KSELLINE9)->KKval2) ){
					st = 1;
				}
				if( (pK + KSELLINE9)->KKauto & 0x02 ){
					st = 1;
				}
			}else if( clm == 2 ){
				if( m_pArith->l_test((pK + KSELLINE9)->KKval3) ){
					st = 1;
				}
				if( (pK + KSELLINE9)->KKauto & 0x04 ){
					st = 1;
				}
			}
			break;

		case KSELLINE12://【業務用資産の取得に係る課税売上高】差引課税売上高（⑩－⑪）
			if( clm == 0 ){
				if( (pK + KSELLINE10)->KKauto & 0x01 || (pK + KSELLINE11)->KKauto & 0x01 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KSELLINE10)->KKval1) ||  m_pArith->l_test((pK + KSELLINE11)->KKval1)){
					st = 1;
				}
			}else if( clm == 1 ){
				if( (pK + KSELLINE12)->KKauto & 0x02 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KSELLINE12)->KKval2) ){
					st = 1;
				}
			}else if( clm == 2 ){
				if( (pK + KSELLINE12)->KKauto & 0x04 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KSELLINE12)->KKval3) ){
					st = 1;
				}
			}
			break;

		case KSELLINE13://課税売上高の合計額（③＋⑥＋⑨＋⑫）

			//計部分は所得の方から金額転記されることがあり、手入力立たないので金額もチェック'14.10.08
			if( clm == 0 ){
				if( (pK + KSELLINE1)->KKauto&0x01 || (pK + KSELLINE2)->KKauto&0x01 ){
					st = 1;
				}
				if( (pK + KSELLINE4)->KKauto&0x01 || (pK + KSELLINE5)->KKauto&0x01 ){
					st = 1;
				}
				if( (pK + KSELLINE7)->KKauto&0x01 || (pK + KSELLINE8)->KKauto&0x01 ){
					st = 1;
				}
				if( (pK + KSELLINE10)->KKauto&0x01 || (pK + KSELLINE11)->KKauto&0x01 ){
					st = 1;
				}

				if( m_pArith->l_test((pK + KSELLINE1)->KKval1) ||  m_pArith->l_test((pK + KSELLINE2)->KKval1) ||
					m_pArith->l_test((pK + KSELLINE4)->KKval1) ||  m_pArith->l_test((pK + KSELLINE5)->KKval1) ||
					m_pArith->l_test((pK + KSELLINE7)->KKval1) ||  m_pArith->l_test((pK + KSELLINE8)->KKval1) ||
					m_pArith->l_test((pK + KSELLINE10)->KKval1) ||  m_pArith->l_test((pK + KSELLINE11)->KKval1) ){
					st = 1;
				}

			}else if( clm == 1 ){
				if( (pK + KSELLINE3)->KKauto & 0x02 || (pK + KSELLINE6)->KKauto & 0x02 || (pK + KSELLINE9)->KKauto & 0x02 || (pK + KSELLINE12)->KKauto & 0x02){
					st = 1;
				}
				if( m_pArith->l_test((pK + KSELLINE3)->KKval2) ||  m_pArith->l_test((pK + KSELLINE6)->KKval2)
					 ||  m_pArith->l_test((pK + KSELLINE9)->KKval2) ||  m_pArith->l_test((pK + KSELLINE12)->KKval2)){
					st = 1;
				}
			}else if( clm == 2 ){
				if( (pK + KSELLINE3)->KKauto & 0x04 || (pK + KSELLINE6)->KKauto & 0x04 || (pK + KSELLINE9)->KKauto & 0x04 || (pK + KSELLINE12)->KKauto & 0x04){
					st = 1;
				}
				if( m_pArith->l_test((pK + KSELLINE3)->KKval3) ||  m_pArith->l_test((pK + KSELLINE6)->KKval3)
					 ||  m_pArith->l_test((pK + KSELLINE9)->KKval3) ||  m_pArith->l_test((pK + KSELLINE12)->KKval3)){
					st = 1;
				}
			}

			break;

		case KSELLINE14://課税標準額の計算(⑬B欄の金額)
			if( m_pArith->l_test((pK + KSELLINE13)->KKval2) ){
				st = 1;
			}
			if( (pK + KSELLINE14)->KKauto&0x03 ){
				st = 1;
			}
			break;

		case KSELLINE15://課税標準額の計算(⑬C欄の金額)
			if( m_pArith->l_test((pK + KSELLINE13)->KKval3) ){
				st = 1;
			}
			if( (pK + KSELLINE15)->KKauto&0x03 ){
				st = 1;
			}
			break;

		case KSELLINE16://合計（⑭＋⑮）
			if( m_pArith->l_test((pK + KSELLINE13)->KKval2)|| m_pArith->l_test((pK + KSELLINE13)->KKval3)){
				if( m_pArith->l_test((pK + KSELLINE14)->KKval1) || m_pArith->l_test((pK + KSELLINE15)->KKval1) ){
					st = 1;
				}
			}
			if(((pK + KSELLINE14)->KKauto&0x03 )||((pK + KSELLINE15)->KKauto&0x03 )){
				st = 1;
			}
			break;

		default:
			if( clm == 0 ){
				if( m_pArith->l_test((pK + line)->KKval1) ){
					st = 1;
				}
				if( (pK + line)->KKauto&0x01 ){
					st = 1;
				}
			}else if( clm == 1 ){
				if( m_pArith->l_test((pK + line)->KKval2) ){
					st = 1;
				}
				if( (pK + line)->KKauto&0x02 ){
					st = 1;
				}
			}else if( clm == 2 ){
				if( m_pArith->l_test((pK + line)->KKval3) ){
					st = 1;
				}
				if( (pK + line)->KKauto&0x04 ){
					st = 1;
				}

			}
			break;
	}

	return st;

}

int CShinSellKeika::GetLineNum( int index )
{
	int rt = -1;

	switch( index ){
		case KSELVAL01A:
		case KSELVAL01B:
		case KSELVAL01C:
			rt = KSELLINE1;
			break;
		case KSELVAL02A:
		case KSELVAL02B:
		case KSELVAL02C:
			rt = KSELLINE2;
			break;
		case KSELVAL03A:
		case KSELVAL03B:
		case KSELVAL03C:
			rt = KSELLINE3;
			break;
		case KSELVAL04A:
		case KSELVAL04B:
		case KSELVAL04C:
			rt = KSELLINE4;
			break;
		case KSELVAL05A:
		case KSELVAL05B:
		case KSELVAL05C:
			rt = KSELLINE5;
			break;
		case KSELVAL06A:
		case KSELVAL06B:
		case KSELVAL06C:
			rt = KSELLINE6;
			break;
		case KSELVAL07A:
		case KSELVAL07B:
		case KSELVAL07C:
			rt = KSELLINE7;
			break;
		case KSELVAL08A:
		case KSELVAL08B:
		case KSELVAL08C:
			rt = KSELLINE8;
			break;
		case KSELVAL09A:
		case KSELVAL09B:
		case KSELVAL09C:
			rt = KSELLINE9;
			break;
		case KSELVAL10A:
		case KSELVAL10B:
		case KSELVAL10C:
			rt = KSELLINE10;
			break;
		case KSELVAL11A:
		case KSELVAL11B:
		case KSELVAL11C:
			rt = KSELLINE11;
			break;
		case KSELVAL12A:
		case KSELVAL12B:
		case KSELVAL12C:
			rt = KSELLINE12;
			break;
		case KSELVAL13A:
		case KSELVAL13B:
		case KSELVAL13C:
			rt = KSELLINE13;
			break;
		case KSELVAL14K:
		case KSELVAL14C:
		case KSELVAL14:	
			rt = KSELLINE14;
			break;
		case KSELVAL15K:
		case KSELVAL15C:
		case KSELVAL15:	
			rt = KSELLINE15;
			break;
		case KSELVAL16:	
			rt = KSELLINE16;
			break;
		default:
			break;
	}

	return rt;
}

int CShinSellKeika::GetColumNum( int index )
{
	int rt = -1;

	switch( index ){
		case KSELVAL01A:
		case KSELVAL02A:
		case KSELVAL03A:
		case KSELVAL04A:
		case KSELVAL05A:
		case KSELVAL06A:
		case KSELVAL07A:
		case KSELVAL08A:
		case KSELVAL09A:
		case KSELVAL10A:
		case KSELVAL11A:
		case KSELVAL12A:
		case KSELVAL13A:
		case KSELVAL14:
		case KSELVAL15:
		case KSELVAL16:
			rt = 0;
			break;
		case KSELVAL01B:
		case KSELVAL02B:
		case KSELVAL03B:
		case KSELVAL04B:
		case KSELVAL05B:
		case KSELVAL06B:
		case KSELVAL07B:
		case KSELVAL08B:
		case KSELVAL09B:
		case KSELVAL10B:
		case KSELVAL11B:
		case KSELVAL12B:
		case KSELVAL13B:
		case KSELVAL14C:
		case KSELVAL15C:
			 rt = 1;
			break;
		case KSELVAL01C:
		case KSELVAL02C:
		case KSELVAL03C:
		case KSELVAL04C:
		case KSELVAL05C:
		case KSELVAL06C:
		case KSELVAL07C:
		case KSELVAL08C:
		case KSELVAL09C:
		case KSELVAL10C:
		case KSELVAL11C:
		case KSELVAL12C:
		case KSELVAL13C:
		case KSELVAL14K:
		case KSELVAL15K:
			rt = 2;
			break;
		default:
			break;
	}

	return rt;
}

// 手入力サインのクリア
int CShinSellKeika::ClearManualInputSign()
{
	int rt = 0;

	for( int idx = 0; idx < 40; idx++ ){
		(*m_pTaxListData)->NRec.KUval[idx].KKauto = 0x00;
	}

	//事業所得の金額をクリアする
	//金額クリア後に金額を連動してくる為
	for( int i=0; i<6; i++ ){
		memset( (*m_pTaxListData)->NRec.KUval[i].KKval1, '\0', sizeof((*m_pTaxListData)->NRec.KUval[i].KKval1) );
	}

	return rt;
}