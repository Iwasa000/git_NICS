// ShinBuy8.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinBuyKeika.h"
#include "ShinBuyKeikaIdx.h"


// CShinBuyKeika ダイアログ

IMPLEMENT_DYNAMIC(CShinBuyKeika, CSyzBaseDlg)

CShinBuyKeika::CShinBuyKeika(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinBuyKeika::IDD, pParent)
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

CShinBuyKeika::~CShinBuyKeika()
{
}

void CShinBuyKeika::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_BuyKei);
}


BEGIN_MESSAGE_MAP(CShinBuyKeika, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinBuyKeika メッセージ ハンドラ

BOOL CShinBuyKeika::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_BuyKei;
	m_BottomIdx = 21;//コントロールの一番下のIDX指定するとスクロールがそこまでいく


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinBuyKeika::Init()
{
	InitDiagAttr();

	DispList(0);//'14.09.01

	SetItemPosition();	// フォーカスセット'14.09.01

	return 0;
}
BOOL CShinBuyKeika::PreTranslateMessage(MSG* pMsg)
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
void CShinBuyKeika::DispList( int mode )
{
	m_BuyKei.OffControl( TRUE );
	DispBuyKeiDiagData();
	m_BuyKei.UpdateWindow();

	DIAGRAM_DATA	data;
	if( !mode ){
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, BKYEARID, TRUE, CIcsdiagctrl );
		data.data_disp.Format("（平成%2x年分）", (*m_pTaxListData)->YEAR);
		diag_setdata( IDC_ICSDIAGCTRL1, BKYEARID, &data, CIcsdiagctrl );

		PostMessage( WM_VSCROLL, SB_TOP, 0 );
		m_Idx = BKVMIN;

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

	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, BKZKBNID, TRUE, CIcsdiagctrl );
	data.data_disp = _T("経理方式：");
	if( (*m_pTaxListData)->Head.KHstax&0x01 ){
		data.data_disp += _T("税抜き");
	}
	else{
		data.data_disp += _T("税込み");
	}
	diag_setdata( IDC_ICSDIAGCTRL1, BKZKBNID, &data, CIcsdiagctrl );

	if( mode ){
		if( m_Idx == KBUYVAL14K && !(*m_pTaxListData)->Head.KHstax ){
			m_Idx = KBUYVAL14C;
		}
		diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );
	}

	diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );

	//SetRedrawScrollToIdx(m_BuyKei.GetPosition());

}

//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
void CShinBuyKeika::DispBuyKeiDiagData()
{
	short	idx, i/*, ii*/;
	KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;
	DIAGRAM_DATA	data;
	int		st = 0;
	short	j = 0;

	//'14.08.20
	for( i = 0 ; i < BKLINCNT ; i++ ){

		//------>'14.10.08
		//農業の部分スキップ
		if(i == 3 || i == 4 || i == 5){
			continue;
		}
		//<-------------------

		for(j = 0 ; j < 3 ; j++){

			st = 0;

			if(j > 0){
				if( i == KBUYLINE1 || i == KBUYLINE2 || i == KBUYLINE4  || i == KBUYLINE5  || i == KBUYLINE7  || i == KBUYLINE8 ||
					i == KBUYLINE10 || i == KBUYLINE11 || i == KBUYLINE16 )
				{
					continue;
				}
			}

			m_Util.DiagOcxIniz( &data );

			switch(i){

				//--------->'14.10.08
				case KBUYLINE1:
					idx = KBUYVAL01A + j;//インデックスをセット
					break;
				case KBUYLINE2:
					idx = KBUYVAL02A + j;//インデックスをセット
					break;
				case KBUYLINE3:
					idx = KBUYVAL03A + j;//インデックスをセット
					break;
				case KBUYLINE4:
					idx = KBUYVAL04A + j;//インデックスをセット
					break;
				case KBUYLINE5:
					idx = KBUYVAL05A + j;//インデックスをセット
					break;
				case KBUYLINE6:
					idx = KBUYVAL06A + j;//インデックスをセット
					break;
				case KBUYLINE7:
					idx = KBUYVAL07A + j;//インデックスをセット
					break;
				case KBUYLINE8:
					idx = KBUYVAL08A + j;//インデックスをセット
					break;
				case KBUYLINE9:
					idx = KBUYVAL09A + j;//インデックスをセット
					break;
				case KBUYLINE10:
					idx = KBUYVAL10A + j;//インデックスをセット
					break;
				case KBUYLINE11:
					idx = KBUYVAL11A + j;//インデックスをセット
					break;
				case KBUYLINE12:
					idx = KBUYVAL12A + j;//インデックスをセット
					break;
				case KBUYLINE13:
					idx = KBUYVAL13A + j;//インデックスをセット
					break;
				case KBUYLINE16:
					idx = KBUYVAL16 + j;//インデックスをセット
					break;
				//<---------------------

				case KBUYLINE14:
					switch(j){
						case 0:
							idx = KBUYVAL14;
							break;
						case 1:
							idx = KBUYVAL14C;
							break;
						case 2:
							idx = KBUYVAL14K;
							break;
					}
					break;
				case KBUYLINE15:
					switch(j){
						case 0:
							idx = KBUYVAL15;
							break;
						case 1:
							idx = KBUYVAL15C;
							break;
						case 2:
							idx = KBUYVAL15K;
							break;
					}
					break;
				default:
//					idx = (i * 3) + j + BKVMIN;//インデックスをセット
					break;
			}

			diag_clear( IDC_ICSDIAGCTRL1, idx, TRUE, CIcsdiagctrl );

/*			switch(i){
				case KBUYLINE3://【事業所得に係る課税仕入高】差引課税仕入高（①－②）
					if( j == 0 ){
						if( (pK + KBUYLINE1)->KKauto & 0x01 || (pK + KBUYLINE2)->KKauto & 0x01 ){
							st = 1;
						}
					}else if( j == 1 ){
						if( (pK + KBUYLINE3)->KKauto & 0x02 ){
							st = 1;
						}
					}else if( j == 2 ){
						if( (pK + KBUYLINE3)->KKauto & 0x04 ){
							st = 1;
						}
					}
					break;

				case KBUYLINE6://【不動産所得に係る課税仕入高】農業　差引課税仕入高（④－⑤）
					if( j == 0 ){
						if( (pK + KBUYLINE4)->KKauto & 0x01 || (pK + KBUYLINE5)->KKauto & 0x01 ){
							st = 1;
						}
					}else if( j == 1 ){
						if( (pK + KBUYLINE6)->KKauto & 0x02 ){
							st = 1;
						}
					}else if( j == 2 ){
						if( (pK + KBUYLINE6)->KKauto & 0x04 ){
							st = 1;
						}
					}
					break;

				case KBUYLINE9://【 [   ]所得に係る課税仕入高】差引課税仕入高（⑩－⑪）
					if( j == 0 ){
						if( (pK + KBUYLINE7)->KKauto & 0x01 || (pK + KBUYLINE8)->KKauto & 0x01 ){
							st = 1;
						}
					}else if( j == 1 ){
						if( (pK + KBUYLINE9)->KKauto & 0x02 ){
							st = 1;
						}
					}else if( j == 2 ){
						if( (pK + KBUYLINE9)->KKauto & 0x04 ){
							st = 1;
						}
					}
					break;

				case KBUYLINE12://【業務用資産の取得に係る課税仕入高】差引課税仕入高（⑬－⑭）
					if( j == 0 ){
						if( (pK + KBUYLINE10)->KKauto & 0x01 || (pK + KBUYLINE10)->KKauto & 0x01 ){
							st = 1;
						}
					}else if( j == 1 ){
						if( (pK + KBUYLINE12)->KKauto & 0x02 ){
							st = 1;
						}
					}else if( j == 2 ){
						if( (pK + KBUYLINE12)->KKauto & 0x04 ){
							st = 1;
						}
					}
					break;

				case KBUYLINE13://課税仕入高の合計額（③＋⑥＋⑨＋⑫）
					if( j == 0 ){
						if( (pK + KBUYLINE1)->KKauto&0x01 || (pK + KBUYLINE2)->KKauto&0x01 ){
							st = 1;
						}
						if( (pK + KBUYLINE4)->KKauto&0x01 || (pK + KBUYLINE5)->KKauto&0x01 ){
							st = 1;
						}
						if( (pK + KBUYLINE7)->KKauto&0x01 || (pK + KBUYLINE8)->KKauto&0x01 ){
							st = 1;
						}
						if( (pK + KBUYLINE10)->KKauto&0x01 || (pK + KBUYLINE11)->KKauto&0x01 ){
							st = 1;
						}
					}else if( j == 1 ){
						if( (pK + KBUYLINE3)->KKauto & 0x02 || (pK + KBUYLINE6)->KKauto & 0x02 || (pK + KBUYLINE9)->KKauto & 0x02 || (pK + KBUYLINE12)->KKauto & 0x02){
							st = 1;
						}
					}else if( j == 2 ){
						if( (pK + KBUYLINE3)->KKauto & 0x04 || (pK + KBUYLINE6)->KKauto & 0x04 || (pK + KBUYLINE9)->KKauto & 0x04 || (pK + KBUYLINE12)->KKauto & 0x04){
							st = 1;
						}
					}
					break;

				case KBUYLINE14://課税仕入れに係る消費税額の計算(⑬B欄の金額)
					if( m_pArith->l_test((pK + KBUYLINE13)->KKval1) ){
						st = 1;
					}
					if( (pK + KBUYLINE14)->KKauto&0x03 ){
						st = 1;
					}
					break;

				case KBUYLINE15://課税仕入れに係る消費税額の計算(⑬C欄の金額)
					if( m_pArith->l_test((pK + KBUYLINE13)->KKval1) ){
						st = 1;
					}
					if( (pK + KBUYLINE15)->KKauto&0x03 ){
						st = 1;
					}
					break;

				case KBUYLINE16://合計（⑭－⑮）
					if( m_pArith->l_test((pK + KBUYLINE14)->KKval1) || m_pArith->l_test((pK + KBUYLINE15)->KKval1) ){
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
	diag_clear( IDC_ICSDIAGCTRL1, KBUYKNAME, TRUE, CIcsdiagctrl );

	CString KHksnmstr = m_Util.CutHankaku( (*m_pTaxListData)->Head.KHksnm, sizeof((*m_pTaxListData)->Head.KHksnm) );
	m_Util.LineDataSet( (LPSTR)(LPCSTR)KHksnmstr, buf, KHksnmstr.GetLength() );

	data.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, KBUYKNAME, &data, CIcsdiagctrl );

	// 14	仮受消費税
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KBUYVAL14K, TRUE, CIcsdiagctrl );
	if( (*m_pTaxListData)->Head.KHstax ){
		if( m_pArith->l_test((pK + KBUYLINE13)->KKval2) ){
			st = 1;
		}
		if( (pK + KBUYLINE14)->KKauto&0x04 ){
			st = 1;
		}
	}
	if( st ){
		memcpy( data.data_val, (pK + KBUYLINE14)->KKval3, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, KBUYVAL14K, &data, CIcsdiagctrl );
	}

	// 14	計算部
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KBUYVAL14C, TRUE, CIcsdiagctrl );
	if( m_pArith->l_test((pK + KBUYLINE13)->KKval2) ){
		st = 1;
	}
	if( (pK + KBUYLINE14)->KKauto&0x02 ){
		st = 1;
	}
	if( st ){
		memcpy( data.data_val, (pK + KBUYLINE14)->KKval2, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, KBUYVAL14C, &data, CIcsdiagctrl );
	}

	// 15	仮受消費税
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KBUYVAL15K, TRUE, CIcsdiagctrl );
	if( (*m_pTaxListData)->Head.KHstax ){
		if( m_pArith->l_test((pK + KBUYLINE13)->KKval3) ){
			st = 1;
		}
		if( (pK + KBUYLINE15)->KKauto&0x04 ){
			st = 1;
		}
	}
	if( st ){
		memcpy( data.data_val, (pK + KBUYLINE15)->KKval3, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, KBUYVAL15K, &data, CIcsdiagctrl );
	}

	// 15	計算部
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KBUYVAL15C, TRUE, CIcsdiagctrl );
	if( m_pArith->l_test((pK + KBUYLINE13)->KKval3) ){
		st = 1;
	}
	if( (pK + KBUYLINE15)->KKauto&0x02 ){
		st = 1;
	}
	if( st ){
		memcpy( data.data_val, (pK + KBUYLINE15)->KKval2, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, KBUYVAL15C, &data, CIcsdiagctrl );
	}

///////////背景色変更
	int cnt = 0;
	int Flg = 0;
	int line = -1;
	unsigned char sgn = 0x00;

	while( BuyKeika[cnt].Index != 0 ){
		if(( BuyKeika[cnt].Index == KBUYVAL01A )||( BuyKeika[cnt].Index == KBUYVAL01B )||( BuyKeika[cnt].Index == KBUYVAL01C )||
		   ( BuyKeika[cnt].Index == KBUYVAL02A )||( BuyKeika[cnt].Index == KBUYVAL02B )||( BuyKeika[cnt].Index == KBUYVAL02C )||
		   ( BuyKeika[cnt].Index == KBUYVAL03A )||( BuyKeika[cnt].Index == KBUYVAL03B )||( BuyKeika[cnt].Index == KBUYVAL03C )||
		   ( BuyKeika[cnt].Index == KBUYVAL04A )||( BuyKeika[cnt].Index == KBUYVAL04B )||( BuyKeika[cnt].Index == KBUYVAL04C )||
		   ( BuyKeika[cnt].Index == KBUYVAL05A )||( BuyKeika[cnt].Index == KBUYVAL05B )||( BuyKeika[cnt].Index == KBUYVAL05C )||
		   ( BuyKeika[cnt].Index == KBUYVAL06A )||( BuyKeika[cnt].Index == KBUYVAL06B )||( BuyKeika[cnt].Index == KBUYVAL06C )||
		   ( BuyKeika[cnt].Index == KBUYVAL14K )||( BuyKeika[cnt].Index == KBUYVAL14C )||( BuyKeika[cnt].Index == KBUYVAL14 )||
		   ( BuyKeika[cnt].Index == KBUYVAL15K )||( BuyKeika[cnt].Index == KBUYVAL15C )||( BuyKeika[cnt].Index == KBUYVAL15 )){
			// 確定時は全入力不可
			if( (*m_pSnHeadData)->Sn_Sign4&0x01){
				Flg = BuyKeika[cnt].AllUnConnect;
			}
			else{
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					Flg = BuyKeika[cnt].UnConnect;
				}
				else{
					Flg = BuyKeika[cnt].Connect;
				}
			}

			if( Flg ){
				DIAGRAM_ATTRIBUTE attr;
				diag_getattr( IDC_ICSDIAGCTRL1, BuyKeika[cnt].Index, &attr, CIcsdiagctrl );
				
				switch( BuyKeika[cnt].Index ){
					case KBUYVAL01A:
					case KBUYVAL01B:
					case KBUYVAL01C:
					case KBUYVAL02A:
					case KBUYVAL02B:
					case KBUYVAL02C:
					case KBUYVAL03A:
					case KBUYVAL03B:
					case KBUYVAL03C:
					case KBUYVAL04A:
					case KBUYVAL04B:
					case KBUYVAL04C:
					case KBUYVAL05A:
					case KBUYVAL05B:
					case KBUYVAL05C:
					case KBUYVAL06A:
					case KBUYVAL06B:
					case KBUYVAL06C:

						line = -1;
						switch( BuyKeika[cnt].Index ){
							case KBUYVAL01A:
							case KBUYVAL01B:
							case KBUYVAL01C:
								line = KBUYLINE1;
									break;
							case KBUYVAL02A:
							case KBUYVAL02B:
							case KBUYVAL02C:
								line = KBUYLINE2;
									break;
							case KBUYVAL03A:
							case KBUYVAL03B:
							case KBUYVAL03C:
								line = KBUYLINE3;
									break;
							case KBUYVAL04A:
							case KBUYVAL04B:
							case KBUYVAL04C:
								line = KBUYLINE4;
									break;
							case KBUYVAL05A:
							case KBUYVAL05B:
							case KBUYVAL05C:
								line = KBUYLINE5;
									break;
							case KBUYVAL06A:
							case KBUYVAL06B:
							case KBUYVAL06C:
								line = KBUYLINE6;
									break;
						}

						sgn = 0x00;
						switch( BuyKeika[cnt].Index ){
							case KBUYVAL01A:
							case KBUYVAL02A:
							case KBUYVAL03A:
							case KBUYVAL04A:
							case KBUYVAL05A:
							case KBUYVAL06A:
								sgn = 0x01;
								break;
							case KBUYVAL01B:
							case KBUYVAL02B:
							case KBUYVAL03B:
							case KBUYVAL04B:
							case KBUYVAL05B:
							case KBUYVAL06B:
								sgn = 0x02;
								break;
							case KBUYVAL01C:
							case KBUYVAL02C:
							case KBUYVAL03C:
							case KBUYVAL04C:
							case KBUYVAL05C:
							case KBUYVAL06C:
								sgn = 0x04;
								break;
						}

						if( (*m_pTaxListData)->NRec.KSval[line].KKauto&sgn ){
							attr.attr_bcolor = BC_BLUE;
						}else{
							attr.attr_bcolor = BC_CREAM;
						}

						break;
					case KBUYVAL14K:
						if( (*m_pTaxListData)->Head.KHstax ){
							if( (*m_pTaxListData)->NRec.KSval[KBUYLINE14].KKauto&0x04 ){
								attr.attr_bcolor = BC_BLUE;
							}else{
								attr.attr_bcolor = BC_CREAM;
							}
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					case KBUYVAL14C:
						if( (*m_pTaxListData)->NRec.KSval[KBUYLINE14].KKauto&0x02 ){
							attr.attr_bcolor = BC_BLUE;
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					case KBUYVAL14:
						if( (*m_pTaxListData)->NRec.KSval[KBUYLINE14].KKauto&0x01 ){
							attr.attr_bcolor = BC_BLUE;
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					case KBUYVAL15K:
						if( (*m_pTaxListData)->Head.KHstax ){
							if( (*m_pTaxListData)->NRec.KSval[KBUYLINE15].KKauto&0x04 ){
								attr.attr_bcolor = BC_BLUE;
							}else{
								attr.attr_bcolor = BC_CREAM;
							}
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					case KBUYVAL15C:
						if( (*m_pTaxListData)->NRec.KSval[KBUYLINE15].KKauto&0x02 ){
							attr.attr_bcolor = BC_BLUE;
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					case KBUYVAL15:
						if( (*m_pTaxListData)->NRec.KSval[KBUYLINE15].KKauto&0x01 ){
							attr.attr_bcolor = BC_BLUE;
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					default:
						break;
				}

				diag_setattr( IDC_ICSDIAGCTRL1, BuyKeika[cnt].Index, &attr, FALSE, CIcsdiagctrl );
			}
		}
		cnt++;
	}
/*	

	for( i=0; i<LINCNT; i++ ){
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
	::ZeroMemory( buf, sizeof(buf) );
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KBUYKNAME, TRUE, CIcsdiagctrl );

	CString KHksnmstr = m_Util.CutHankaku( (*m_pTaxListData)->Head.KHksnm, sizeof((*m_pTaxListData)->Head.KHksnm) );
	m_Util.LineDataSet( (LPSTR)(LPCSTR)KHksnmstr, buf, KHksnmstr.GetLength() );

	data.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL1, KBUYKNAME, &data, CIcsdiagctrl );

	// 17	仮受消費税
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KBUYVAL14K, TRUE, CIcsdiagctrl );
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
		diag_setdata( IDC_ICSDIAGCTRL1, KBUYVAL14K, &data, CIcsdiagctrl );
	}

	// 17	計算部
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, KBUYVAL14C, TRUE, CIcsdiagctrl );
	if( l_test((pK+15)->KKval1) ){
		st = 1;
	}
	if( (pK+16)->KKauto&0x02 ){
		st = 1;
	}
	if( st ){
		memcpy( data.data_val, (pK+16)->KKval2, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, KBUYVAL14C, &data, CIcsdiagctrl );
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
int CShinBuyKeika::GetDiagData( long index, short leng, DIAGRAM_DATA *pData, int mode )
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

	if(( m_Idx == KBUYVAL14K )||( m_Idx == KBUYVAL15K )){
		if( !(*m_pTaxListData)->Head.KHstax&0x01 ){
			DispBuyKeiDiagData();
			return 0;
		}
	}

	//'14.08.20
	int clm_pos = 0;
	if(( m_Idx == KBUYVAL14K )||( m_Idx == KBUYVAL15K )){
		clm_pos = 2;
	}else if(( m_Idx == KBUYVAL14C )||( m_Idx == KBUYVAL15C )){
		clm_pos = 1;
	}else if(( m_Idx == KBUYVAL14 )||( m_Idx == KBUYVAL15 )){
		clm_pos = 0;
	}else{
		clm_pos = (m_Idx - BKVMIN) % 3;
	}

	//------->'14.10.08
	if(m_Lin == 0)			Lin_tmp = KBUYLINE1;
	else if(m_Lin == 1)		Lin_tmp = KBUYLINE2;
	else if(m_Lin == 2)		Lin_tmp = KBUYLINE3;
	else if(m_Lin == 3)		Lin_tmp = KBUYLINE4;
	else if(m_Lin == 4)		Lin_tmp = KBUYLINE5;
	else if(m_Lin == 5)		Lin_tmp = KBUYLINE6;
	else if(m_Lin == 6)		Lin_tmp = KBUYLINE7;
	else if(m_Lin == 7)		Lin_tmp = KBUYLINE8;
	else if(m_Lin == 8)		Lin_tmp = KBUYLINE9;
	else if(m_Lin == 9)		Lin_tmp = KBUYLINE10;
	else if(m_Lin == 10)	Lin_tmp = KBUYLINE11;
	else if(m_Lin == 11)	Lin_tmp = KBUYLINE12;
	else if(m_Lin == 12)	Lin_tmp = KBUYLINE13;
	else if(m_Lin == 13)	Lin_tmp = KBUYLINE14;
	else if(m_Lin == 14)	Lin_tmp = KBUYLINE15;
	else if(m_Lin == 15)	Lin_tmp = KBUYLINE16;
	//<--------------------



	if( !mode ){
		if( m_Idx == KBUYKNAME ){
			::ZeroMemory( buf, sizeof(buf));
			m_Util.LineDataGet( dbuf, buf, sizeof(dbuf) );
			memcpy( (*m_pTaxListData)->Head.KHksnm, buf, sizeof((*m_pTaxListData)->Head.KHksnm));
		}
		else{
//			pK = &(*m_pTaxListData)->NRec.KSval[m_Lin];
			pK = &(*m_pTaxListData)->NRec.KSval[Lin_tmp];
			if( leng ){
/*				if( m_Idx == KBUYVAL14K ){
					if( m_pArith->l_test(pData->data_val) ){
						pK->KKauto |= 0x04;
					}else{
						pK->KKauto &= 0xfb;
					}
					memcpy( pK->KKval3, pData->data_val, 6 );
				}
				else if( m_Idx == KBUYVAL14C ){
					if( m_pArith->l_test(pData->data_val) ){
						pK->KKauto |= 0x02;
					}else{
						pK->KKauto &= 0xfd;
					}
					memcpy( pK->KKval2, pData->data_val, 6 );
				}
				else if( m_Idx == KBUYVAL15K ){
					if( m_pArith->l_test(pData->data_val) ){
						pK->KKauto |= 0x04;
					}else{
						pK->KKauto &= 0xfb;
					}
					memcpy( pK->KKval3, pData->data_val, 6 );
				}
				else if( m_Idx == KBUYVAL15C ){
					if( m_pArith->l_test(pData->data_val) ){
						pK->KKauto |= 0x02;
					}else{
						pK->KKauto &= 0xfd;
					}
					memcpy( pK->KKval2, pData->data_val, 6 );
				}
				else{*/
//					switch( m_Lin ){
					switch( Lin_tmp ){
					case KBUYLINE1:
					case KBUYLINE2:
					case KBUYLINE3:
					case KBUYLINE4:
					case KBUYLINE5:
					case KBUYLINE6:
					case KBUYLINE14:
					case KBUYLINE15:
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

					if(clm_pos == 0)		memcpy( pK->KKval1, pData->data_val, 6 );
					else if(clm_pos == 1)	memcpy( pK->KKval2, pData->data_val, 6 );
					else if(clm_pos == 2)	memcpy( pK->KKval3, pData->data_val, 6 );

//					memcpy( pK->KKval1, pData->data_val, 6 );
//				}
			}
		}
	}
	else{
		if( m_Idx == KBUYKNAME ){
			::ZeroMemory( (*m_pTaxListData)->Head.KHksnm, 6 );
		}
		else{
//			pK = &(*m_pTaxListData)->NRec.KSval[m_Lin];
			pK = &(*m_pTaxListData)->NRec.KSval[Lin_tmp];
			if( m_Idx == KBUYVAL14K ){
				pK->KKauto &= 0xfb;
				::ZeroMemory( pK->KKval3, 6 );
			}
			else if( m_Idx == KBUYVAL14C ){
				pK->KKauto &= 0xfd;
				::ZeroMemory( pK->KKval2, 6 );
			}
			else if( m_Idx == KBUYVAL15K ){
				pK->KKauto &= 0xfb;
				::ZeroMemory( pK->KKval3, 6 );
			}
			else if( m_Idx == KBUYVAL15C ){
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
	(*m_Scalc)->Shz_BuyKeiPost();//'14.08.20
//	m_Scalc->Shz_BuyPost();//'14.08.06
	DispBuyKeiDiagData();
	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
void CShinBuyKeika::InitDiagAttr()
{
	int	cnt = 0;
	int	Flg;
	while( 1 ){
		if( BuyKeika[cnt].Index == 0 ){
			break;
		}
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01){
			Flg = BuyKeika[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = BuyKeika[cnt].UnConnect;
			}
			else{
				Flg = BuyKeika[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL1, BuyKeika[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, BuyKeika[cnt].Connect );
		cnt++;
	}

	SetFocusRange();

	m_BuyKei.EnableDelete(0);//DELETEキー１回で削除モード設定'14.08.01

}

BEGIN_EVENTSINK_MAP(CShinBuyKeika, CSyzBaseDlg)
	ON_EVENT(CShinBuyKeika, IDC_ICSDIAGCTRL1, 1, CShinBuyKeika::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinBuyKeika, IDC_ICSDIAGCTRL1, 2, CShinBuyKeika::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinBuyKeika, IDC_ICSDIAGCTRL1, 3, CShinBuyKeika::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinBuyKeika::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_BuyKei.OffControl( FALSE );

	m_Idx = index;
	if( (m_Idx>=BKVMIN) && (m_Idx <= BKVMAX) ){
		if( m_Idx == BKVMAX ){
			m_Lin = ( (m_Idx- BKVMIN) / 3 ) - 1;
		}
//		if(( m_Idx == KBUYVAL14K )||( m_Idx == KBUYVAL14C )||( m_Idx == KBUYVAL14 )){
//			m_Lin = KBUYLINE14;
//		}
//		if(( m_Idx == KBUYVAL15K )||( m_Idx == KBUYVAL15C )||( m_Idx == KBUYVAL15 )){
//			m_Lin = KBUYLINE15;
//		}
		else{
			m_Lin = (m_Idx- BKVMIN) / 3;
		}
	}
	else{
		m_Lin = -1;
	}

	m_curwnd = IDC_ICSDIAGCTRL1;//'14.08.01

}

void CShinBuyKeika::EditOFFIcsdiagctrl1(short index)
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

void CShinBuyKeika::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
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
			m_BuyKei.SetPosition(pos);	
	//	}
	}
	if( wTerm==VK_DOWN ){
	//	if(index == KBUYVAL14){
	//		m_BuyKei.SetNextPosition();
	//	}
	//	else{
//			pos = UpDownFocusMoveCheck(index , 0);
			pos = FocusMoveCheck(index , 0);
			m_BuyKei.SetPosition(pos);
	//	}
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
//		m_BuyKei.SetNextPosition();
		pos = FocusMoveCheck(index , 2);
		m_BuyKei.SetPosition(pos);
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
//		m_BuyKei.SetPrevPosition();
		pos = FocusMoveCheck(index , 3);
		m_BuyKei.SetPosition(pos);
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

	SetRedrawScrollToIdx(m_BuyKei.GetPosition());

/*	int nowpos = m_BuyKei.GetPosition();	// 移動先または自身のポジション

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
//int CShinBuyKeika::UpDownFocusMoveCheck(int Index , int Move)
int CShinBuyKeika::FocusMoveCheck(int Index , int Move)
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
		for(i = 0 ; i < FCS_SHINBUYKEIKACNT ; i++){
			if(Index == FCS_ShinBuyKeika[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinBuyKeika[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SHINBUYKEIKACNT ; i++){
			if(Index == FCS_ShinBuyKeika_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinBuyKeika_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SHINBUYKEIKACNT ; j++){
			m_BuyKei.GetAttribute(FCS_ShinBuyKeika[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinBuyKeika[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_BuyKei.GetAttribute(FCS_ShinBuyKeika[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinBuyKeika[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SHINBUYKEIKACNT ; j++){
			m_BuyKei.GetAttribute(FCS_ShinBuyKeika_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinBuyKeika_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_BuyKei.GetAttribute(FCS_ShinBuyKeika_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinBuyKeika_LR[j].IDNo;
				break;
			}
		}
	}
	return idx;
}
//<-----------------------------------------

void CShinBuyKeika::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SHINBUYKEIKACNT; idx++ ){
		m_BuyKei.GetAttribute(FCS_ShinBuyKeika[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_ShinBuyKeika[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SHINBUYKEIKACNT; idx++ ){
		m_BuyKei.GetAttribute(FCS_ShinBuyKeika_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_ShinBuyKeika_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINBUYKEIKACNT-1; idx > -1 ; idx-- ){
		m_BuyKei.GetAttribute(FCS_ShinBuyKeika[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_ShinBuyKeika[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINBUYKEIKACNT-1; idx > -1 ; idx-- ){
		m_BuyKei.GetAttribute(FCS_ShinBuyKeika_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_ShinBuyKeika_LR[idx].IDNo;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// カーソル移動
//-----------------------------------------------------------------------------
// 引数	index	：	
//		flg		：	
//		nChar	：	
//-----------------------------------------------------------------------------
void CShinBuyKeika::Diag1_Edit( short index, UINT flg, short nChar )
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
				case BKVMAX:
					flg = REPLY_STAY;
					if( nChar != VK_RETURN ){
						if( (*m_pTaxListData)->GKBN == 0x02 ){
							xpos = KBUYVAL07A;
						}
						else if( (*m_pTaxListData)->GKBN == 0x04 ){
//							xpos = VAL04;
						}
						else{
							xpos = BKVMIN;
						}
					}
					break;
				case KBUYVAL02A:
					if( (*m_pTaxListData)->GKBN == 0x01 ){
						flg = REPLY_STAY;
						xpos = KBUYKNAME;
					}
					break;
//				case VAL05:
//					if( !((*m_pTaxListData)->GKBN&0x02) ){
//						flg = REPLY_STAY;
//						xpos = KZNAME;
//					}
//					break;
				case KBUYVAL08A:
					flg = REPLY_STAY;
					xpos = KBUYKNAME;
					break;
				case KBUYKNAME:
					flg = REPLY_STAY;
					xpos = KBUYVAL10A;
					break;
				default:
					break;
			}
			break;
		case VK_LEFT:
			flg = REPLY_BACK;
			switch( m_Idx ){
				case BKVMIN:
					flg = REPLY_STAY;
					xpos = BKVMAX;
					break;
//				case VAL04:
//					if( (*m_pTaxListData)->GKBN == 0x04 ){
//						flg = REPLY_STAY;
//						xpos = VMAX;
//					}
					break;
				case KBUYVAL07A:
					if( (*m_pTaxListData)->GKBN == 0x02 ){
						flg = REPLY_STAY;
						xpos = BKVMAX;
					}
					break;
				case KBUYVAL10A:
					flg = REPLY_STAY;
					xpos = KBUYKNAME;
					break;
				case KBUYKNAME:
					flg = REPLY_STAY;
					if( (*m_pTaxListData)->GKBN == 0x01 ){
						xpos = KBUYVAL02A;
					}
					else if( (*m_pTaxListData)->GKBN == 0x04 ){
//						xpos = VAL05;
					}
					else{
						xpos = KBUYVAL08A;
					}
					break;
				default:
					break;
			}
			break;
		case VK_UP:
			flg = REPLY_BACK;
			switch( m_Idx ){
				case KBUYKNAME:
				case KBUYVAL14K:
				case KBUYVAL14C:
				case BKVMIN:
					flg = REPLY_STAY;
					break;
//				case VAL04:
//					if( (*m_pTaxListData)->GKBN == 0x04 ){
//						flg = REPLY_STAY;
//					}
//					break;
				case KBUYVAL07A:
					if( (*m_pTaxListData)->GKBN == 0x02 ){
						flg = REPLY_STAY;	
					}
					break;
				case BKVMAX:
					flg = REPLY_STAY;
					xpos = KBUYVAL14;
					break;
				default:
					break;
			}
			break;
		case VK_DOWN:
			flg = REPLY_STAY;
			if( (m_Idx == KBUYKNAME) || (m_Idx == KBUYVAL14K) || (m_Idx == KBUYVAL14C) || (m_Idx == BKVMAX) ){
				;
			}
			else{
				if( m_Idx == KBUYVAL14 ){
					xpos = BKVMAX;
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
void CShinBuyKeika::SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo )
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
int CShinBuyKeika::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j;
	char	cnm[128], buf[128], CVAL[6];
	int		len;
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

	return _xprn( pno, fno, PF, 0, pDC, 0, 1 );
}
//-----------------------------------------------------------------------------
// 印刷２
//-----------------------------------------------------------------------------
// 引数	pno		：	出力指定表No.
//		fno		：	フィールドNo.
//		pDC		：	デバイスコンテキスト
//		pBp		：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
//int CShinBuyKeika::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
int CShinBuyKeika::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j, lin, max;
	char	/*cnm[128], */buf[128], CVAL[6];
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
	pBpMv += ( 2+1 );*/

// 事業所得に係る課税仕入高
// 金額
	lin = 0;	max = 3;
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j;

//		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KSval[l_idx+lin];
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

//		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KSval[l_idx+lin];
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

//		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KSval[l_idx+lin];
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

//		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KSval[l_idx+lin];
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

//		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KSval[l_idx+lin];
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

//		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KSval[l_idx+lin];
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

//		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KSval[l_idx+lin];
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

//		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KSval[l_idx+lin];
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

//		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KSval[l_idx+lin];
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

//		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KSval[l_idx+lin];
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

//		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KSval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
/*		if( j+lin == 14 ){
			if( m_pArith->l_test((pS+12)->KKval1) || m_pArith->l_test((pS+13)->KKval1) ){
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

//		pK = &(*m_pTaxListData)->NRec.KSval[j+lin];
		pK = &(*m_pTaxListData)->NRec.KSval[l_idx+lin];
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
//	pK = &(*m_pTaxListData)->NRec.KSval[12];
	pK = &(*m_pTaxListData)->NRec.KSval[KBUYLINE13];
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
	if( DispJudge( 0, KBUYLINE13 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

//うち税率４％適用分
//	pK = &(*m_pTaxListData)->NRec.KSval[12];
	pK = &(*m_pTaxListData)->NRec.KSval[KBUYLINE13];
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
	if( DispJudge( 1, KBUYLINE13 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval2, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

//うち税率６．３％適用分
//	pK = &(*m_pTaxListData)->NRec.KSval[12];
	pK = &(*m_pTaxListData)->NRec.KSval[KBUYLINE13];
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
	if( DispJudge( 2, KBUYLINE13 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval3, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );


// 課税標準額の計算
// B欄の金額
	st = 0;
//	pK = &(*m_pTaxListData)->NRec.KSval[13];
	pK = &(*m_pTaxListData)->NRec.KSval[KBUYLINE14];
/*	if( m_pArith->l_test((pS+15)->KKval1)){
		st = 1;
	}
	if( pK->KKauto&0x02 ){
		st = 1;
	}*/
	::ZeroMemory( buf, sizeof(buf) );
//	m_pArith->l_print( buf, pK->KKval2, st?FMTX130:FMTX13 );
	if( DispJudge( 0, KBUYLINE14 ) == 1 ){ //列？
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
	if( DispJudge( 0, KBUYLINE14 ) == 1 ){ //列？
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

// C欄の金額
	st = 0;
//	pK = &(*m_pTaxListData)->NRec.KSval[14];
	pK = &(*m_pTaxListData)->NRec.KSval[KBUYLINE15];
/*	if( m_pArith->l_test((pS+15)->KKval1)){
		st = 1;
	}
	if( pK->KKauto&0x02 ){
		st = 1;
	}*/
	::ZeroMemory( buf, sizeof(buf) );
//	m_pArith->l_print( buf, pK->KKval2, st?FMTX130:FMTX13 );
	if( DispJudge( 0, KBUYLINE15 ) == 1 ){ //列？
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
	if( DispJudge( 0, KBUYLINE15 ) == 1 ){ //列？
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

// 合計
	pK = &(*m_pTaxListData)->NRec.KSval[ KBUYLINE16];
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
	if( DispJudge( 0, KBUYLINE16 ) == 1 ){ //列？
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );


	int fontsize = 0;
	char val[5][128];
	CString str;
	for(int l_cnt = KBUYLINE1; l_cnt <= KBUYLINE16; l_cnt++){
		switch(l_cnt){
			case KBUYLINE1:
			case KBUYLINE2:
			case KBUYLINE3:
			case KBUYLINE4:
			case KBUYLINE5:
			case KBUYLINE6:
			case KBUYLINE7:
			case KBUYLINE8:
			case KBUYLINE9:
			case KBUYLINE10:
			case KBUYLINE11:
			case KBUYLINE12:
			case KBUYLINE13:
//			case KBUYLINE14:
//			case KBUYLINE15:
//			case KBUYLINE16:
				for(int idx = 0;idx<5;idx++){
					::ZeroMemory( val[idx], sizeof( val[idx] ));
				}
				m_pArith->l_print( val[0],(*m_pTaxListData)->NRec.KSval[l_cnt].KKval1, FMTX130 );
				m_pArith->l_print( val[1],(*m_pTaxListData)->NRec.KSval[l_cnt].KKval2, FMTX130 );
				m_pArith->l_print( val[2],(*m_pTaxListData)->NRec.KSval[l_cnt].KKval3, FMTX130 );
				m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KSval[l_cnt].KKval4, FMTX130 );
				m_pArith->l_print( val[4],(*m_pTaxListData)->NRec.KSval[l_cnt].KKval5, FMTX130 );
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
//int CShinBuyKeika::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
int CShinBuyKeika::PrintHyo3( int pno, int fno, CDC *pDC, char *pBp )
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

//	return( _xprn( pno, fno, PF, 0, pDC, 0 ));
	return( _xprn( pno, 4, PF, 0, pDC, 0 ));
}


//-----------------------------------------------------------------------------
// 初期ポジションセット'14.09.01
//-----------------------------------------------------------------------------
void CShinBuyKeika::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
		if( (index = m_BuyKei.GetPosition()) != -1) {
			m_BuyKei.SetPosition(index);
			m_BuyKei.SetFocus();
			return;
		}
	}

	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_BuyKei.SetPosition(KBUYVAL01A);
	}
	else{
		m_BuyKei.SetPosition(KBUYVAL07A);
	}
	SetRedrawScroll( 0 );

}

int CShinBuyKeika::DispJudge( int clm, int line )
{

	int st = 0;
	KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;

	if( clm > 0 ){
		if( line == KBUYLINE1 || line == KBUYLINE2 || line == KBUYLINE4  || line == KBUYLINE5  || line == KBUYLINE7  || line == KBUYLINE8 ||
			line == KBUYLINE10 || line == KBUYLINE11 || line == KBUYLINE16 )
		{
			return st;
		}
	}


	switch( line ){
		case KBUYLINE3://【事業所得に係る課税仕入高】差引課税仕入高（①－②）
			if( clm == 0 ){
				//【事業所得に係る課税売上高】計部分は所得の方から金額転記されることがあり、手入力立たないので金額もチェック'14.10.08
				if( (pK + KBUYLINE1)->KKauto & 0x01 || (pK + KBUYLINE2)->KKauto & 0x01 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KBUYLINE1)->KKval1) ||  m_pArith->l_test((pK + KBUYLINE2)->KKval1)){//'14.10.08
					st = 1;
				}
			}else if( clm == 1 ){
				if( (pK + KBUYLINE3)->KKauto & 0x02 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KBUYLINE3)->KKval2) ){//'14.10.08
					st = 1;
				}
			}else if( clm == 2 ){
				if( (pK + KBUYLINE3)->KKauto & 0x04 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KBUYLINE3)->KKval3) ){//'14.10.08
					st = 1;
				}
			}
			break;
					
		case KBUYLINE6://【不動産所得に係る課税仕入高】農業　差引課税仕入高（④－⑤）
			//【不動産所得に係る課税売上高】計部分は所得の方から金額転記されることがあり、手入力立たないので常に表示'14.10.08
			if( clm == 0 ){
				if( (pK + KBUYLINE4)->KKauto & 0x01 || (pK + KBUYLINE5)->KKauto & 0x01 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KBUYLINE4)->KKval1) ||  m_pArith->l_test((pK + KBUYLINE5)->KKval1)){//'14.10.08
					st = 1;
				}
			}else if( clm == 1 ){
				if( (pK + KBUYLINE6)->KKauto & 0x02 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KBUYLINE6)->KKval2) ){//'14.10.08
					st = 1;
				}
			}else if( clm == 2 ){
				if( (pK + KBUYLINE6)->KKauto & 0x04 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KBUYLINE6)->KKval3) ){//'14.10.08
					st = 1;
				}
			}
			break;

		case KBUYLINE9://【 [   ]所得に係る課税仕入高】差引課税仕入高（⑩－⑪）
			if( clm == 0 ){
				if( m_pArith->l_test((pK + KBUYLINE7)->KKval1)||(m_pArith->l_test((pK + KBUYLINE8)->KKval1))){
					st = 1;
				}
				if( (pK + KBUYLINE7)->KKauto & 0x01 || (pK + KBUYLINE8)->KKauto & 0x01 ){
					st = 1;
				}
			}else if( clm == 1 ){
				if( m_pArith->l_test((pK + KBUYLINE9 )->KKval2 ) ){
					st = 1;
				}
				if( (pK + KBUYLINE9)->KKauto & 0x02 ){
					st = 1;
				}
			}else if( clm == 2 ){
				if( m_pArith->l_test((pK + KBUYLINE9 )->KKval3 ) ){
					st = 1;
				}
				if( (pK + KBUYLINE9)->KKauto & 0x04 ){
					st = 1;
				}
			}
			break;

		case KBUYLINE12://【業務用資産の取得に係る課税仕入高】差引課税仕入高（⑬－⑭）
			if( clm == 0 ){
				if( m_pArith->l_test((pK + KBUYLINE10)->KKval1)||(m_pArith->l_test((pK + KBUYLINE11)->KKval1))){
					st = 1;
				}
				if( (pK + KBUYLINE10)->KKauto & 0x01 || (pK + KBUYLINE11)->KKauto & 0x01 ){
					st = 1;
				}
			}else if( clm == 1 ){
				if( m_pArith->l_test((pK + KBUYLINE12 )->KKval2 ) ){
					st = 1;
				}
				if( (pK + KBUYLINE12)->KKauto & 0x02 ){
					st = 1;
				}
			}else if( clm == 2 ){
				if( m_pArith->l_test((pK + KBUYLINE12 )->KKval3 ) ){
					st = 1;
				}
				if( (pK + KBUYLINE12)->KKauto & 0x04 ){
					st = 1;
				}
			}
			break;

		case KBUYLINE13://課税仕入高の合計額（③＋⑥＋⑨＋⑫）
			//計部分は所得の方から金額転記されることがあり、手入力立たないので金額もチェック//'14.10.08
			if( clm == 0 ){
				if( (pK + KBUYLINE1)->KKauto&0x01 || (pK + KBUYLINE2)->KKauto&0x01 ){
					st = 1;
				}
				if( (pK + KBUYLINE4)->KKauto&0x01 || (pK + KBUYLINE5)->KKauto&0x01 ){
					st = 1;
				}
				if( (pK + KBUYLINE7)->KKauto&0x01 || (pK + KBUYLINE8)->KKauto&0x01 ){
					st = 1;
				}
				if( (pK + KBUYLINE10)->KKauto&0x01 || (pK + KBUYLINE11)->KKauto&0x01 ){
					st = 1;
				}

				if( m_pArith->l_test((pK + KBUYLINE1)->KKval1) ||  m_pArith->l_test((pK + KBUYLINE2)->KKval1) ||
					m_pArith->l_test((pK + KBUYLINE4)->KKval1) ||  m_pArith->l_test((pK + KBUYLINE5)->KKval1) ||
					m_pArith->l_test((pK + KBUYLINE7)->KKval1) ||  m_pArith->l_test((pK + KBUYLINE8)->KKval1) ||
					m_pArith->l_test((pK + KBUYLINE10)->KKval1) ||  m_pArith->l_test((pK + KBUYLINE11)->KKval1)){
					st = 1;
				}

			}else if( clm == 1 ){
				if( (pK + KBUYLINE3)->KKauto & 0x02 || (pK + KBUYLINE6)->KKauto & 0x02 || (pK + KBUYLINE9)->KKauto & 0x02 || (pK + KBUYLINE12)->KKauto & 0x02){
					st = 1;
				}
				if( m_pArith->l_test((pK + KBUYLINE3)->KKval2) ||  m_pArith->l_test((pK + KBUYLINE6)->KKval2)
					 ||  m_pArith->l_test((pK + KBUYLINE9)->KKval2) ||  m_pArith->l_test((pK + KBUYLINE12)->KKval2)){
					st = 1;
				}
			}else if( clm == 2 ){
				if( (pK + KBUYLINE3)->KKauto & 0x04 || (pK + KBUYLINE6)->KKauto & 0x04 || (pK + KBUYLINE9)->KKauto & 0x04 || (pK + KBUYLINE12)->KKauto & 0x04){
					st = 1;
				}
				if( m_pArith->l_test((pK + KBUYLINE3)->KKval3) ||  m_pArith->l_test((pK + KBUYLINE6)->KKval3)
					 ||  m_pArith->l_test((pK + KBUYLINE9)->KKval3) ||  m_pArith->l_test((pK + KBUYLINE12)->KKval3)){
					st = 1;
				}
			}
			break;

		case KBUYLINE14://課税仕入れに係る消費税額の計算(⑬B欄の金額)
//			if( m_pArith->l_test((pK + KBUYLINE13)->KKval1) ){
			if( m_pArith->l_test((pK + KBUYLINE13)->KKval2) ){
				st = 1;
			}
			if( (pK + KBUYLINE14)->KKauto&0x03 ){
				st = 1;
			}
			break;

		case KBUYLINE15://課税仕入れに係る消費税額の計算(⑬C欄の金額)
//			if( m_pArith->l_test((pK + KBUYLINE13)->KKval1) ){
			if( m_pArith->l_test((pK + KBUYLINE13)->KKval3) ){
				st = 1;
			}
			if( (pK + KBUYLINE15)->KKauto&0x03 ){
				st = 1;
			}
			break;

		case KBUYLINE16://合計（⑭－⑮）
			if( m_pArith->l_test((pK + KBUYLINE13)->KKval2)|| m_pArith->l_test((pK + KBUYLINE13)->KKval3)){
				if( m_pArith->l_test((pK + KBUYLINE14)->KKval1) || m_pArith->l_test((pK + KBUYLINE15)->KKval1) ){
					st = 1;
				}
			}
			if(((pK + KBUYLINE14)->KKauto&0x03 )||( (pK + KBUYLINE15)->KKauto&0x03 )){
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


// 手入力サインのクリア
int CShinBuyKeika::ClearManualInputSign()
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