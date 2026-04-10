// ShinNougyouKeika.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinNougyou31Ex.h"
#include "ShinNougyou31ExIdx.h"


// CShinNougyou31Ex ダイアログ

IMPLEMENT_DYNAMIC(CShinNougyou31Ex, CSyzBaseDlg)

CShinNougyou31Ex::CShinNougyou31Ex(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinNougyou31Ex::IDD, pParent)
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

CShinNougyou31Ex::~CShinNougyou31Ex()
{
}

void CShinNougyou31Ex::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Nougyou31);
}


BEGIN_MESSAGE_MAP(CShinNougyou31Ex, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinNougyou31Ex メッセージ ハンドラ

BOOL CShinNougyou31Ex::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Nougyou31;
//	m_BottomIdx = 63;
	m_BottomIdx = 58;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinNougyou31Ex::Init()
{
	InitDiagAttr();

	DispList(0);

	SetFocusRange();

	SetItemPosition();	// フォーカスセット'14.09.01

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
void CShinNougyou31Ex::InitDiagAttr()
{

	DIAGRAM_ATTRIBUTE data;
	long dg1[] = { KNOUKNAME27ex, KNOUKNAME28ex, KNOUKNAME29ex, -1 };

	for( int i=0;; i++ ){
		if( dg1[i] == -1 ){
			break;
		}
		// ank対応
		m_Nougyou31.GetAttribute( (short)dg1[i], (LPUNKNOWN)&data );
		data.attr_editattr |= DIAG_EATTR_ZENHAN;//DIAG_EATTR_ZENHAN ank対応
		m_Nougyou31.SetAttribute( (short)dg1[i], (LPUNKNOWN)&data, 1 );
	}

	int	cnt = 0;

	SH_ITEMINDEX *pIndex = NULL;
	if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) {
		pIndex = R02_NougyouEx;
	}
	else {
		pIndex = NougyouEx;
	}

	while( 1 ){
		//if( Nougyou[cnt].Index == 0 ){
		if ( ( pIndex + cnt )->Index == 0 ) {
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			//Flg = Nougyou[cnt].AllUnConnect;
			Flg = ( pIndex + cnt )->AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				//Flg = Nougyou[cnt].UnConnect;
				Flg = ( pIndex + cnt )->UnConnect;
				//if ( ( *m_pSnHeadData )->SVmzsw != 1 ) {
				//	//if( ( Nougyou[cnt].Index % 6 ) == 0 ){

				//	// 簡易課税の時は経費の欄を入力可能
				//	//if ( ( ( pIndex + cnt )->Index % 6 ) == 0 ) {
				//	//	Flg = 0;
				//	//}
				//	if ( CheckSVmzsw ( ( pIndex + cnt )->Index ) == 1 ) {
				//		Flg = 0;
				//	}
				//}
			}
			else{
				//Flg = Nougyou[cnt].Connect;
				Flg = ( pIndex + cnt )->Connect;
			}
		}
		//ATRIB_MOD( IDC_ICSDIAGCTRL1, Nougyou[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Nougyou[cnt].Connect );
		ATRIB_MOD( IDC_ICSDIAGCTRL1, ( pIndex + cnt )->Index, Flg, (*m_pSnHeadData)->Sn_Sign4, ( pIndex + cnt )->Connect );
		cnt++;
	}

	InitDiagonalLines();

	SetFocusRange();

	m_Nougyou31.EnableDelete(0);//DELETEキー１回で削除モード設定

	
}


//====================================================
// 斜線の表示/非表示
//====================================================
void CShinNougyou31Ex::InitDiagonalLines()
{
	long flg = 0; 
	if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) {
		flg = DIAG_MDFY_OPAQUE;
	}
	else {
		flg = DIAG_MDFY_TRANSPARENT;
	}

	for ( short index = NDIAGONALLINE_Sex; index <= NDIAGONALLINE_Eex; index++ ) {
		m_Nougyou31.ModifyItem ( index, flg );
	}
}

BOOL CShinNougyou31Ex::PreTranslateMessage(MSG* pMsg)
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
void CShinNougyou31Ex::DispList( int mode )
{
// 22-0167,22-0361 add -->
	// 入力可／不可を切り替える
	ColumnControl();
// 22-0167,22-0361 add <--
	m_Nougyou31.OffControl( TRUE );
//2016.06.22 INSERT START
	(*m_Scalc)->Shz_31AllCalc(3);
//2016.06.22 INSERT END
	for( int i = 0 ; i < NKLINCNTex ; i++ ){
		DispNougyou31DiagData( i );
	}

	DIAGRAM_DATA	data;
	if( !mode ){
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, NKYEARIDex, TRUE, CIcsdiagctrl );
		data.data_disp.Format( _T("（令和%2x年分）"), (*m_pTaxListData)->YEAR);
		diag_setdata( IDC_ICSDIAGCTRL1, NKYEARIDex, &data, CIcsdiagctrl );

		PostMessage(WM_VSCROLL, SB_TOP, 0 );
		m_Idx = NKVMINex;
	}

	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, NKZKBNIDex, TRUE, CIcsdiagctrl );
	data.data_disp = _T("経理方式：");
	if( (*m_pTaxListData)->Head.KHstax&0x01 ){
		data.data_disp += _T("税抜き");
	}
	else{
		data.data_disp += _T("税込み");
	}
	diag_setdata( IDC_ICSDIAGCTRL1, NKZKBNIDex, &data, CIcsdiagctrl );

	//diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );

//2016.06.22 INSERT START
	int edit[] = {KNOUKNAME27ex,KNOUKNAME28ex,KNOUKNAME29ex};
	BOOL edit_focus = FALSE;

	for( int cnt = 0; cnt < ARRAYCOUNT(edit); cnt++ ){
		Switch_KamokuEdit( IDC_ICSDIAGCTRL1, edit[cnt] );

		if( m_Nougyou31.GetPosition() == edit[cnt] ){
			edit_focus = TRUE;
		}
	}

	if(( (*m_pTaxListData)->Head.KHkoke == 1 ) && ( edit_focus == TRUE )){
		this->SetFocus();
	}
//2016.06.22 INSERT END

	//SetRedrawScrollToIdx(m_Nougyou31.GetPosition());

// 22-0167,22-0361 add -->
	KKVAL*	pK = &(*m_pTaxListData)->NRec.KNval[0];
	int	idx=0;
	for(idx=NOSELChk2ex; idx<=NOSELChk3ex; idx++)	{
		diag_getdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
		data.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
	}
	idx=0;
	if((*m_pSnHeadData)->Sn_GenericSgn & 0x800)		idx=NOSELChk3ex;
	//if(pK->KKauto & 0x40)		idx=NOSELChk2ex;
	//else if(pK->KKauto & 0x80)	idx=NOSELChk3ex;
	if(idx != 0)	{
		diag_getdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
		data.data_check = 1;
		diag_setdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
	}
	//// 確定時または連動時は選択不可
	//if(((*m_pSnHeadData)->Sn_Sign4 & 0x01) || (!((*m_pSnHeadData)->Sn_Sign4 & 0x80)))	{
	//	for(idx=NOSELChk2ex; idx<=NOSELChk3ex; idx++)	{
	//		ATRIB_MOD(IDC_ICSDIAGCTRL1,idx,0,(*m_pSnHeadData)->Sn_Sign4,0);
	//	}
	//}
	// 6.24%適用分は非表示にする(今後要望があったときのために残しておく)
	diag_modify(IDC_ICSDIAGCTRL1,NOSELChk2ex,DIAG_MDFY_TRANSPARENT,CIcsdiagctrl);
	// 連動時は7.8%適用分も非表示にする
	if(!((*m_pSnHeadData)->Sn_Sign4 & 0x80))	{
		diag_modify(IDC_ICSDIAGCTRL1,NOSELChk3ex,DIAG_MDFY_TRANSPARENT,CIcsdiagctrl);
	}
	// 確定時は7.8%適用分を選択不可にする
	else if((*m_pSnHeadData)->Sn_Sign4 & 0x01)	{
		//ATRIB_MOD(IDC_ICSDIAGCTRL1,NOSELChk3ex,0,(*m_pSnHeadData)->Sn_Sign4,0);
		DIAGRAM_ATTRIBUTE	da;
		diag_modify(IDC_ICSDIAGCTRL1,NOSELChk3ex,DIAG_MDFY_READONLY,CIcsdiagctrl);
		diag_getattr(IDC_ICSDIAGCTRL1,NOSELChk3ex,&da,CIcsdiagctrl);
		da.attr_bcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAGCTRL1,NOSELChk3ex,&da,FALSE,CIcsdiagctrl);
	}
	else	{
		DIAGRAM_ATTRIBUTE	da,da2;
		diag_getattr(IDC_ICSDIAGCTRL1,0,&da2,CIcsdiagctrl);
		diag_getattr(IDC_ICSDIAGCTRL1,NOSELChk3ex,&da,CIcsdiagctrl);
		da.attr_bcolor = da2.attr_bcolor;
		diag_setattr(IDC_ICSDIAGCTRL1,NOSELChk3ex,&da,FALSE,CIcsdiagctrl);
		diag_modify(IDC_ICSDIAGCTRL1,NOSELChk3ex,DIAG_MDFY_EDIT,CIcsdiagctrl);
	}
// 22-0167,22-0361 add <--
}

//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
// 引数	dlin	：	
//-----------------------------------------------------------------------------
void CShinNougyou31Ex::DispNougyou31DiagData( long dlin )
{

	//'14.08.25
	short	idx, i;
	long	lin;

	//データ保存ポジションセット

	static long KNouDataPos[] = {0 , 1 , 2 , 3 , 4 , 5 , 38 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 , 17 , 18 , 19 , 
		                         20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 , 28 , 29 , 30 , 31 , 32 , 33 , 34 , 35 , 36 , 37};

	lin = KNouDataPos[dlin];

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KNval[lin];
	char	val[6];
	DIAGRAM_DATA	data;
	int		st;

	idx = (short)( dlin * NKWIDECNTex ) + NKVMINex;
	for( i = 0 ; i < NKWIDECNTex ; i++ ){
		st = 0;
		::ZeroMemory( val, sizeof(val));
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, idx+i, TRUE, CIcsdiagctrl );
		switch( i ){
			case 0:	// 決算額 A
/*				switch( lin ){
					case KNOULINE3M:	//未成熟果樹収入
						break;

					case KNOULINE4:		//小計
						for( j = KNOULINE1 ; j < KNOULINE4 ; j++ ){
							if( j == KNOULINE3M ){//未成熟果樹収入はスキップ（入力不可の為）
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
								st = 1;
								break;
							}
						}
						break;
//					case KNOULINE8:		//計
//						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE6].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE7].KKval1) ){
//							st = 1;
//							break;
//						}
//						break;
					case KNOULINE7:		//計
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE5].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE6].KKval1) ){
							st = 1;
//							break;
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

					case KNOULINE35:	//計
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE32].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE33].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE34].KKval1) ){
							st = 1;
//							break;
						}
						break;

					case KNOULINE36:	//差引金額
						for( j = KNOULINE1 ; j < KNOULINE4 ; j++ ){
							if( i == KNOULINE3M ){//未成熟果樹収入はスキップ（入力不可の為）
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
								st = 1;
								break;
							}
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE5].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE6].KKval1) ){
							st = 1;
							break;
						}
						for( j = KNOULINE8 ; j < KNOULINE31 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
								st = 1;
								break;
							}
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE32].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE33].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE34].KKval1) ){
							st = 1;
//							break;
						}

//						st = 1;
						break;

					default:
						st = 1;	
						break;
				}*/
				memcpy( val, pK->KKval1, 6 );
				break;

			case 1:	// Aのうち課税取引にならないもの　B
/*				switch( lin ){
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
							if(j == KNOULINE13 || j == KNOULINE14 || j == KNOULINE15 || j == KNOULINE16 || j == KNOULINE18){//農具費、農薬・衛生費、諸材料費、修繕費、作業用衣料費はスキップ（入力不可の為）
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
				}*/
				memcpy( val, pK->KKval2, 6 );
				break;

			case 2:		// 課税取引金額　C（A－B）
/*				switch( lin ){
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
//						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[lin].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[lin].KKval2) ){
//							st = 1;
//						}
						for( j = KNOULINE1 ; j < KNOULINE4 ; j++ ){
							if( j != KNOULINE3M ){//家事消費,未成熟果樹収入はスキップ（入力不可の為）
								if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
									st = 1;
									break;
								}
							}
							if( j != KNOULINE2K && j != KNOULINE3M ){//家事消費,未成熟果樹収入はスキップ（入力不可の為）
								if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval2) ){
									st = 1;
									break;
								}
							}
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE3M].KKval3) ){
							st = 1;
						}
						break;

					case KNOULINE31:	//経費　小計
						for( j = KNOULINE8 ; j < KNOULINE31 ; j++ ){
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
								st = 1;
								break;
							}
							if( j == KNOULINE13 || i == KNOULINE14 || i == KNOULINE15 || i == KNOULINE16 || i == KNOULINE18 ){//農具費、農薬・衛生費、諸材料費、修繕費、作業用衣料費はスキップ（入力不可の為）
								continue;
							}
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval2) ){
								st = 1;
								break;
							}
						}
						break;

					default:
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[lin].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[lin].KKval2) ){
							st = 1;
						}
						break;
				}*/
				memcpy( val, pK->KKval3, 6 );
				break;

//			case 3:	// うち税率６．３％適用分　D
///*				switch( lin ){
//					case KNOULINE5:		//農産物の棚卸高　期首
//					case KNOULINE6:		//農産物の棚卸高　期末
//					case KNOULINE7:		//計
//					case KNOULINE19:	//農業共済掛金
//					case KNOULINE20:	//減価償却費
//					case KNOULINE23:	//利子割引料
//					case KNOULINE26:	//貸倒金
//					case KNOULINE32:	//農産物以外の棚卸高　期首
//					case KNOULINE33:	//農産物以外の棚卸高　期末
//					case KNOULINE34:	//経費から差し引く果樹牛馬等の育成費用
//					case KNOULINE35:	//計
//					case KNOULINE36:	//差引金額
//						//入力可所無し
//						break;
//
//					case KNOULINE4:		//収入金額　小計	
//						for( j = KNOULINE1; j < KNOULINE4; j++ ){
//							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval4) ){
//								st = 1;
//								break;
//							}
//						}
//						break;
//
//					case KNOULINE31:	//経費　小計
//						for( j = KNOULINE8; j < KNOULINE31; j++ ){
//							if(j == KNOULINE19 || i == KNOULINE20 || i == KNOULINE23 || i == KNOULINE26){//農業共済掛金、減価償却費、利子割引料、貸倒金、はスキップ（入力不可の為）
//								continue;
//							}
//							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval4) ){
//								st = 1;
//								break;
//							}
//						}
//						break;
//					default:
//						st = 1;		
//						break;
//				}*/
//				memcpy( val, pK->KKval4, 6 );
//				break;


			//case 4:	// うち税率６．２４％適用分　E
			case 3:	// うち税率６．２４％適用分　E
/*				switch( lin ){
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
				}*/
				memcpy( val, pK->KKval5, 6 );
				break;

			//case 5:	// うち税率７．８％適用分　F	//2019/07/02追加
			case 4:	// うち税率７．８％適用分　F	//2019/07/02追加
/*				switch( lin ){
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
				}*/
				memcpy(val, pK->KKval6, 6);
				break;


		}
		if( DispJudge( i, lin ) ){
			memcpy( data.data_val, val, 6 );
			diag_setdata( IDC_ICSDIAGCTRL1, idx+i, &data, CIcsdiagctrl );
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
//2016.06.22 UPDATE START
			//switch(dlin){
			//	case KNOULINE27:
			//		idx = KNOUKNAME27ex;
			//		break;

			//	case KNOULINE28:	
			//		idx = KNOUKNAME28ex;
			//		break;

			//	case KNOULINE29:	
			//		idx = KNOUKNAME29ex;
			//		break;
			//}

			//memcpy( buf, pK->KKnam, sizeof(pK->KKnam));

			CString tmp;
			int prf_no = 0;
			// 経費１と経費２をまとめるのではなく、経費３と経費４を合算
			switch(dlin){
				case KNOULINE27:
					idx = KNOUKNAME27ex;
					//prf_no = 27;
					prf_no = 26;
					break;

				case KNOULINE28:	
					idx = KNOUKNAME28ex;
					//prf_no = 28;
					prf_no = 27;
					break;

				case KNOULINE29:	
					idx = KNOUKNAME29ex;
					//prf_no = 29;
					prf_no = 28;
					break;
			}

//-- '16.12.07 --
//			if(((*m_pTaxListData)->Head.KHkoke == 1 )&&( prf_no != 0 )){
//---------------
			if( (*m_pTaxListData)->IsLinkKjprfname() && (prf_no!=0) ){
//---------------
				if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)||(m_pArith->l_test(pK->KKval4)!=0)||(m_pArith->l_test(pK->KKval5)!=0)|| (m_pArith->l_test(pK->KKval6) != 0)){ //金額が全て０なら表示しない
					(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmsub, ID_ICSSH_KJ_NOUGYOU, prf_no, tmp );
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

	idx = (short)( dlin * NKWIDECNTex ) + NKVMINex;
	for( i = 0 ; i < NKWIDECNTex ; i++ ){
		st = 0;
		::ZeroMemory( val, sizeof(val));
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, idx+i, TRUE, CIcsdiagctrl );
		switch( i ){
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
			switch(lin){
				case 28:
					idx = KNOUKNAME27ex;
					break;
				case 29:	
					idx = KNOUKNAME28ex;
					break;
				case 30:	
					idx = KNOUKNAME29ex;
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
// 引数
// 
// 
// 
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CShinNougyou31Ex::GetDiagData( long index, short leng, DIAGRAM_DATA *data, int mode )
{
	//'14.08.25
// 22-0167,22-0361 cor -->
	//if( index < 0 ){
// -----------------------
	if(index < 0 || index == NOSELChk2ex || index == NOSELChk3ex) {
// 22-0167,22-0361 cor <--
		return 0;
	}

	char	dbuf[128];
	char	*buff;
	int		len;

	len = data->data_edit.GetLength();
	memset( dbuf, '\0', sizeof(dbuf) );
	buff = (char *)((LPCTSTR)(data->data_edit));
	memmove( dbuf, buff, len );

	//データ保存ポジションセット
	long	lin;
		//データ保存ポジションセット
	static long KNouDataPos[] = {0 , 1 , 2 , 3 , 4 , 5 , 38 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 , 17 , 18 , 19 , 
		                         20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 , 28 , 29 , 30 , 31 , 32 , 33 , 34 , 35 , 36 , 37};

	lin = KNouDataPos[m_Lin];

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KNval[lin];

	if( !mode ){
		switch( m_Clm ){
			case 0:		// 決算額	A
				if( leng ){
					memcpy( pK->KKval1, data->data_val, 6 );
				}
				break;
			case 1:		// Aのうち課税取引にならないもの	B
				if( leng ){
					memcpy( pK->KKval2, data->data_val, 6 );
				}
				break;
			case 2:		// 課税取引金額　C（A－B）
				if( leng ){
					memcpy( pK->KKval3, data->data_val, 6 );
				}
				break;

			//case 3:		// うち税率６．３％適用分　D
			//	if( leng ){
			//		memcpy( pK->KKval4, data->data_val, 6 );
			//	}
			//	break;

			//case 4:		// うち税率６．２４％適用分　E
			case 3:		// うち税率６．２４％適用分　E
				if( leng ){
					memcpy( pK->KKval5, data->data_val, 6 );
				}
				break;

			//case 5:		// うち税率７．８％適用分　F
			case 4:		// うち税率７．８％適用分　F
				if (leng) {
					memcpy(pK->KKval6, data->data_val, 6);
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
			//case 3:		// うち税率６．３％適用分　D
			//	::ZeroMemory( pK->KKval4, 6 );	
			//	break;
			//case 4:		// うち税率６．２４％適用分　E
			case 3:		// うち税率６．２４％適用分　E
				::ZeroMemory( pK->KKval5, 6 );	
				break;
			//case 5:		// うち税率７．８％適用分　E
			case 4:		// うち税率７．８％適用分　E
				::ZeroMemory( pK->KKval6, 6 );
				break;
			default:	// 項目名
				::ZeroMemory( pK->KKnam, sizeof(pK->KKnam));
				break;
		}
	}

	// 集計
	(*m_Scalc)->Shz_31AllCalc(3);//'14.08.25
//	m_Scalc->Shz_AllCalc(3);//'14.08.06
	for( int i = 0 ; i < NKLINCNTex ; i++ ){
		DispNougyou31DiagData( i );
	}

	return 0;

	/*
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
	m_Scalc->Shz_AllCalc(3);//'14.08.06
	for( int i = 0 ; i < NKLINCNTex ; i++ ){
		DispNougyouKeikaDiagData( i );
	}
	return 0;
	*/
}



BEGIN_EVENTSINK_MAP(CShinNougyou31Ex, CSyzBaseDlg)
ON_EVENT(CShinNougyou31Ex, IDC_ICSDIAGCTRL1, 1, CShinNougyou31Ex::EditONIcsdiagctrl1, VTS_I2)
ON_EVENT(CShinNougyou31Ex, IDC_ICSDIAGCTRL1, 2, CShinNougyou31Ex::EditOFFIcsdiagctrl1, VTS_I2)
ON_EVENT(CShinNougyou31Ex, IDC_ICSDIAGCTRL1, 3, CShinNougyou31Ex::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
// 22-0167,22-0361 add -->
ON_EVENT(CShinNougyou31Ex, IDC_ICSDIAGCTRL1, 9, CShinNougyou31Ex::CheckButtonIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
// 22-0167,22-0361 add <--
END_EVENTSINK_MAP()

void CShinNougyou31Ex::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	
	//'14.08.25
	m_Nougyou31.OffControl( FALSE );

	m_Idx = index;
	if( (m_Idx >= NKVMINex) && (m_Idx <= NKVMAXex) ){
		m_Lin = (index - NKVMINex) / NKWIDECNTex;
		m_Clm = (index - NKVMINex) % NKWIDECNTex;
	}
	
	else if( m_Idx == KNOUKNAME27ex){
		m_Lin = KNOULINE27 + 1;//変換等対価の行分＋１する
//		m_Lin = KNOULINE27;
		m_Clm = -1;
	}
	else if( m_Idx == KNOUKNAME28ex){
		m_Lin = KNOULINE28 + 1;//変換等対価の行分＋１する
//		m_Lin = KNOULINE28;
		m_Clm = -1;
	}
	else if( m_Idx == KNOUKNAME29ex){
		m_Lin = KNOULINE29 + 1;//変換等対価の行分＋１する
//		m_Lin = KNOULINE29;
		m_Clm = -1;
	}


	m_curwnd = IDC_ICSDIAGCTRL1;//'14.08.01
}

void CShinNougyou31Ex::EditOFFIcsdiagctrl1(short index)
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

void CShinNougyou31Ex::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
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
	//	if(index == KNOUVAL36E){
	//		m_Nougyou31.SetPrevPosition();
	//	}
	//	else{
//			pos = UpDownFocusMoveCheck(index , 1);
			pos = FocusMoveCheck(index , 1);
			m_Nougyou31.SetPosition(pos);	
	//	}
	}
	if( wTerm==VK_DOWN ){
	//	if(index == KNOUVAL36E){
	//		m_Nougyou31.SetNextPosition();
	//	}
	//	else{
//			pos = UpDownFocusMoveCheck(index , 0);
			pos = FocusMoveCheck(index , 0);
			m_Nougyou31.SetPosition(pos);
	//	}
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
//		m_Nougyou31.SetNextPosition();
		pos = FocusMoveCheck(index , 2);
		m_Nougyou31.SetPosition(pos);	
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
//		m_Nougyou31.SetPrevPosition();
		pos = FocusMoveCheck(index , 3);
		m_Nougyou31.SetPosition(pos);	
	}

	//------->DELETEキー対応'14.08.06
	if( wTerm == VK_DELETE){
		GetDiagData( index, length, &xdata, -1 );
	}

	SetRedrawScrollToIdx(m_Nougyou31.GetPosition(), IDC_ICSDIAGCTRL1);

/*	int nowpos = m_Nougyou31.GetPosition();	// 移動先または自身のポジション

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
//int CShinNougyou31Ex::UpDownFocusMoveCheck(int Index , int Move)
int CShinNougyou31Ex::FocusMoveCheck(int Index , int Move)
{
	int idx = 0;
	int i = 0 , j = 0;
	BOOL end_flg = FALSE;

	int cnt = 0;
	FCSUPDOWN_INFO *pInfo = NULL,
				   *pInfo_LR = NULL;

	if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) {
		cnt = R02_FCS_SHINNOUGYOU31CNTex;
		pInfo = R02_FCS_ShinNougyou31Ex;
		pInfo_LR = R02_FCS_ShinNougyou31Ex_LR;
	}
	else {
		cnt = FCS_SHINNOUGYOU31CNTex;
		pInfo = FCS_ShinNougyou31Ex;
		pInfo_LR = FCS_ShinNougyou31Ex_LR;
	}

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
		//for(i = 0 ; i < FCS_SHINNOUGYOU31CNT ; i++){
		//	if(Index == FCS_ShinNougyou31[i].IDNo){
		//		break;
		//	}
		//}
		//idx = FCS_ShinNougyou31[i].IDNo;//現在ポジションセット
		for ( i = 0; i < cnt; i++ ) {
			if ( Index == ( pInfo + i )->IDNo ) {
				break;
			}
		}
		idx = ( pInfo + i )->IDNo;
	}else if(( Move == 2 )||( Move == 3 )){
		//for(i = 0 ; i < FCS_SHINNOUGYOU31CNT ; i++){
		//	if(Index == FCS_ShinNougyou31_LR[i].IDNo){
		//		break;
		//	}
		//}
		//idx = FCS_ShinNougyou31_LR[i].IDNo;//現在ポジションセット
		for ( i = 0; i < cnt; i++ ) {
			if ( Index == ( pInfo_LR + i )->IDNo ) {
				break;
			}
		}
		idx = ( pInfo + i )->IDNo;
	}
	//次ポジション検索
	if(Move == 0){//下方向
		//for(j = i + 1 ; j < FCS_SHINNOUGYOU31CNT ; j++){
		//	m_Nougyou31.GetAttribute(FCS_ShinNougyou31[j].IDNo ,  (LPUNKNOWN)&DA);
		//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
		//		idx = FCS_ShinNougyou31[j].IDNo;
		//		break;
		//	}
		//}
		for ( j = i + 1; j < cnt; j++ ) {
			m_Nougyou31.GetAttribute ( ( pInfo + j )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo + j )->IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		//for(j = i - 1 ; j > -1 ; j--){
			//m_Nougyou31.GetAttribute(FCS_ShinNougyou31[j].IDNo ,  (LPUNKNOWN)&DA);
			//if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			//	idx = FCS_ShinNougyou31[j].IDNo;
			//	break;
			//}
		//}
		for( j = i - 1; j > -1; j--) {
			m_Nougyou31.GetAttribute ( ( pInfo + j )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo + j )->IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		//for(j = i + 1 ; j < FCS_SHINNOUGYOU31CNT ; j++){
		//	m_Nougyou31.GetAttribute(FCS_ShinNougyou31_LR[j].IDNo ,  (LPUNKNOWN)&DA);
		//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
		//		idx = FCS_ShinNougyou31_LR[j].IDNo;
		//		break;
		//	}
		//}
		for ( j = i + 1; j < cnt; j++ ) {
			m_Nougyou31.GetAttribute ( ( pInfo_LR + j )->IDNo, (LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo_LR + j )->IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		//for(j = i - 1 ; j > -1 ; j--){
		//	m_Nougyou31.GetAttribute(FCS_ShinNougyou31_LR[j].IDNo ,  (LPUNKNOWN)&DA);
		//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
		//		idx = FCS_ShinNougyou31_LR[j].IDNo;
		//		break;
		//	}
		//}
		for( j = i - 1; j > -1; j-- ) {
			m_Nougyou31.GetAttribute ( ( pInfo_LR + j )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo_LR + j )->IDNo;
				break;
			}
		}
	}
	return idx;
}
//<-----------------------------------------

void CShinNougyou31Ex::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	int cnt = 0;
	FCSUPDOWN_INFO *pInfo = NULL,
				   *pInfo_LR = NULL;

	if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) {
		cnt = R02_FCS_SHINNOUGYOU31CNTex;
		pInfo = R02_FCS_ShinNougyou31Ex;
		pInfo_LR = R02_FCS_ShinNougyou31Ex_LR;
	}
	else {
		cnt = FCS_SHINNOUGYOU31CNTex;
		pInfo = FCS_ShinNougyou31Ex;
		pInfo_LR = FCS_ShinNougyou31Ex_LR;
	}

	//for( int idx = 0; idx < FCS_SHINNOUGYOU31CNT; idx++ ){
	//	m_Nougyou31.GetAttribute(FCS_ShinNougyou31[idx].IDNo ,  (LPUNKNOWN)&DA);
	//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
	//		m_FcsUpEnd = FCS_ShinNougyou31[idx].IDNo;
	//		break;
	//	}
	//}
	for ( int idx = 0; idx < cnt; idx++ ) {
		m_Nougyou31.GetAttribute ( ( pInfo + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsUpEnd = ( pInfo + idx )->IDNo;
			break;
		}
	}

	//for( int idx = 0; idx < FCS_SHINNOUGYOU31CNT; idx++ ){
	//	m_Nougyou31.GetAttribute(FCS_ShinNougyou31_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
	//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
	//		m_FcsLeftEnd = FCS_ShinNougyou31_LR[idx].IDNo;
	//		break;
	//	}
	//}
	for ( int idx = 0; idx < cnt; idx++ ) {
		m_Nougyou31.GetAttribute ( ( pInfo_LR + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsLeftEnd = ( pInfo_LR + idx )->IDNo;
			break;
		}
	}

	//for( int idx = FCS_SHINNOUGYOU31CNT-1; idx > -1 ; idx-- ){
	//	m_Nougyou31.GetAttribute(FCS_ShinNougyou31[idx].IDNo ,  (LPUNKNOWN)&DA);
	//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
	//		m_FcsDownEnd = FCS_ShinNougyou31[idx].IDNo;
	//		break;
	//	}
	//}
	for ( int idx = cnt - 1; idx > -1; idx-- ) {
		m_Nougyou31.GetAttribute ( ( pInfo + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsDownEnd = ( pInfo + idx )->IDNo;
			break;
		}
	}

	//for( int idx = FCS_SHINNOUGYOU31CNT-1; idx > -1 ; idx-- ){
	//	m_Nougyou31.GetAttribute(FCS_ShinNougyou31_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
	//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
	//		m_FcsRightEnd = FCS_ShinNougyou31_LR[idx].IDNo;
	//		break;
	//	}
	//}
	for ( int idx = cnt - 1; idx > -1; idx-- ) {
		m_Nougyou31.GetAttribute ( ( pInfo_LR + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsRightEnd = ( pInfo_LR + idx )->IDNo;
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
void CShinNougyou31Ex::SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo )
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
int CShinNougyou31Ex::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j;
	char	cnm[128], buf[128], CVAL[6];
	int		len;
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
			sprintf_s( buf, sizeof( buf ), _T(" %s"), m_PrintCmInfo->CopNam );
		}
		else{
//			sprintf_s( buf, sizeof( buf ), _T("[%08d] %s"), CopNum, CopNam );
			sprintf_s( buf, sizeof( buf ), _T("[%08d] %s"), m_PrintCmInfo->CopNum, m_PrintCmInfo->CopNam );
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
			sprintf_s( buf, sizeof(buf), _T("[%08d] %s"), m_pZmsub->zvol->v_cod, cnm );
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

	return( _xprn( pno, fno, PF, 0, pDC, 0 ));
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
//int CShinNougyou31Ex::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
int CShinNougyou31Ex::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
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
			sprintf_s( buf, sizeof(buf), _T("[%08d] %s"), m_pZmsub->zvol->v_cod, cnm );
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
//	lin = 0;	max = 6;
	lin = 0;	max = 5;
	pK = &(*m_pTaxListData)->NRec.KNval[0];
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
				if( st == 1 ){
					m_pArith->l_print( buf, pK->KKval3, FMTX130 );
				}
				break;
			//case 3:
			//	if( st == 1 ){
			//		m_pArith->l_print( buf, pK->KKval4, FMTX130 );
			//	}
			//	break;
			//case 4:
			case 3:
				if( st == 1 ){
					m_pArith->l_print( buf, pK->KKval5, FMTX130 );
				}
				break;
			//case 5:
			case 4:
				if (st == 1) {
					m_pArith->l_print(buf, pK->KKval6, FMTX130);
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
	lin = 1;	max = 5;
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
		if( DispJudge( 1, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
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
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 2, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}

	// 小計4
	pK = &(*m_pTaxListData)->NRec.KNval[5];
	::ZeroMemory( buf, sizeof( buf ));
	if( DispJudge( 2, KNOULINE4 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval3, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );
	i++;

//// うち税率６．３％適用分 D
//	for( j=0; (j+lin)<max; j++ ){
//		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
//		::ZeroMemory( buf, sizeof(buf) );
//		if( DispJudge( 3, j+lin ) == 1 ){
//			m_pArith->l_print( buf, pK->KKval4, FMTX130 );
//		}
//		len = (int)strlen( buf );
//		memmove( pBpMv, buf, len );
//		PF[i].PITM[j] = pBpMv;
//		pBpMv += ( len+1 );
//	}
//
//	// 小計4
//	pK = &(*m_pTaxListData)->NRec.KNval[5];
//	::ZeroMemory( buf, sizeof( buf ));
//	if( DispJudge( 3, KNOULINE4 ) == 1 ){
//		m_pArith->l_print( buf, pK->KKval4, FMTX130 );
//	}
//	len = (int)strlen( buf );
//	memmove( pBpMv, buf, len );
//	PF[i].PITM[j] = pBpMv;
//	pBpMv += ( len+1 );
//	i++;

// うち税率６．２４％適用分 E
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 3, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval5, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}

	// 小計4
	pK = &(*m_pTaxListData)->NRec.KNval[5];
	::ZeroMemory( buf, sizeof( buf ));
	if( DispJudge( 3, KNOULINE4 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval5, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );
	i++;

	// うち税率７．８％適用分 F
	for (j = 0; (j + lin) < max; j++) {
		pK = &(*m_pTaxListData)->NRec.KNval[j + lin];
		::ZeroMemory(buf, sizeof(buf));
		if (DispJudge(4, j + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval6, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}

	// 小計4
	pK = &(*m_pTaxListData)->NRec.KNval[5];
	::ZeroMemory(buf, sizeof(buf));
	if (DispJudge(4, KNOULINE4) == 1) {
		m_pArith->l_print(buf, pK->KKval6, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i].PITM[j] = pBpMv;
	pBpMv += (len + 1);
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
	if( DispJudge( 0, KNOULINE7 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );

	int fontsize = 0;
	char val[6][128];
	CString str;
	for(int l_cnt = KNOULINE1; l_cnt <= KNOULINE36; l_cnt++){
		switch(l_cnt){
			case KNOULINE1:
			case KNOULINE2K:	
			case KNOULINE2J:
			case KNOULINE3:	
			case KNOULINE3M:	
			case KNOULINE4:	
			case KNOULINE5:	
			case KNOULINE6:	
			case KNOULINE7:
			case KNOULINE8:
			case KNOULINE9:
			case KNOULINE10:
			case KNOULINE11:
			case KNOULINE12:
			case KNOULINE13:
			case KNOULINE14:
			case KNOULINE15:
			case KNOULINE16:
			case KNOULINE17:
			case KNOULINE18:
			case KNOULINE19:
			case KNOULINE20:
			case KNOULINE21:
			case KNOULINE22:
			case KNOULINE23:
			case KNOULINE24:
			case KNOULINE25:
			case KNOULINE26:
			case KNOULINE27:
			case KNOULINE28:
			case KNOULINE29:
			case KNOULINE30:
			case KNOULINE31:
			case KNOULINE32:
			case KNOULINE33:
			case KNOULINE34:
			case KNOULINE35:
			case KNOULINE36:
				for(int idx = 0;idx<6;idx++){
					::ZeroMemory( val[idx], sizeof( val[idx] ));
				}
				m_pArith->l_print( val[0],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval1, FMTX130 );
				m_pArith->l_print( val[1],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval2, FMTX130 );
				m_pArith->l_print( val[2],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval3, FMTX130 );
				//m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval4, FMTX130 );
				//m_pArith->l_print( val[4],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval5, FMTX130 );
				m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval5, FMTX130 );
				//m_pArith->l_print( val[5], (*m_pTaxListData)->NRec.KNval[l_cnt].KKval6, FMTX130);
				m_pArith->l_print( val[4], (*m_pTaxListData)->NRec.KNval[l_cnt].KKval6, FMTX130);
				//for(int idx = 0;idx<6;idx++){
				for(int idx = 0;idx<5;idx++){
					str.Format(val[idx]);
					str.Trim();
					// 10億円 ( カンマ含む )以上ならフォントサイズを下げる
					//if( str.GetLength() > 13 ){
					if ( str.GetLength() > 12 ) {
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
//int CShinNougyou31Ex::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
int CShinNougyou31Ex::PrintHyo3( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j , len , lin , max;
	char	buf[64];
	KKVAL*	pK;
	int st = 0;

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
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
//		st = 0;
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 2, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

//// うち税率６．３％適用分 D
//	for( j=0; (j+lin)<max; j++ ){
//		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
////		st = 0;
//		::ZeroMemory( buf, sizeof(buf) );
//		if( DispJudge( 3, j+lin ) == 1 ){
//			m_pArith->l_print( buf, pK->KKval4, FMTX130 );
//		}
//		len = (int)strlen( buf );
//		memmove( pBpMv, buf, len );
//		PF[i].PITM[j] = pBpMv;
//		pBpMv += ( len+1 );
//	}
//	i++;

// うち税率６．２４％適用分 E
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
//		st = 0;
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 3, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval5, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

	// うち税率７．８％適用分 F
	for (j = 0; (j + lin) < max; j++) {
		pK = &(*m_pTaxListData)->NRec.KNval[j + lin];
		//		st = 0;
		::ZeroMemory(buf, sizeof(buf));
		if (DispJudge(4, j + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval6, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;

	// 経費(18-26)
	lin = 19;	max = 28;
// 決算額 A
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
//		st = 0;
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
		st = 0;
		::ZeroMemory( buf, sizeof(buf) );
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
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		st = 0;
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 2, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;
//// うち税率６．３％適用分 D
//	for( j=0; (j+lin)<max; j++ ){
//		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
//		st = 0;
//		::ZeroMemory( buf, sizeof(buf) );
//		if( DispJudge( 3, j+lin ) == 1 ){
//			m_pArith->l_print( buf, pK->KKval4, FMTX130 );
//		}
//		len = (int)strlen( buf );
//		memmove( pBpMv, buf, len );
//		PF[i].PITM[j] = pBpMv;
//		pBpMv += ( len+1 );
//	}
//	i++;
// うち税率６．２４％適用分 E
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		st = 0;
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 3, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval5, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

	// うち税率７．８％適用分 F
	for (j = 0; (j + lin) < max; j++) {
		pK = &(*m_pTaxListData)->NRec.KNval[j + lin];
		st = 0;
		::ZeroMemory(buf, sizeof(buf));
		if (DispJudge(4, j + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval6, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;

	int fontsize = 0;
	char val[6][128];
	CString str;
	for(int l_cnt = KNOULINE1; l_cnt <= KNOULINE36; l_cnt++){
		switch(l_cnt){
			case KNOULINE1:
			case KNOULINE2K:	
			case KNOULINE2J:
			case KNOULINE3:	
			case KNOULINE3M:	
			case KNOULINE4:	
			case KNOULINE5:	
			case KNOULINE6:	
			case KNOULINE7:
			case KNOULINE8:
			case KNOULINE9:
			case KNOULINE10:
			case KNOULINE11:
			case KNOULINE12:
			case KNOULINE13:
			case KNOULINE14:
			case KNOULINE15:
			case KNOULINE16:
			case KNOULINE17:
			case KNOULINE18:
			case KNOULINE19:
			case KNOULINE20:
			case KNOULINE21:
			case KNOULINE22:
			case KNOULINE23:
			case KNOULINE24:
			case KNOULINE25:
			case KNOULINE26:
			case KNOULINE27:
			case KNOULINE28:
			case KNOULINE29:
			case KNOULINE30:
			case KNOULINE31:
			case KNOULINE32:
			case KNOULINE33:
			case KNOULINE34:
			case KNOULINE35:
			case KNOULINE36:
				//for(int idx = 0;idx<6;idx++){
				for(int idx = 0;idx<5;idx++){
					::ZeroMemory( val[idx], sizeof( val[idx] ));
				}
				m_pArith->l_print( val[0],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval1, FMTX130 );
				m_pArith->l_print( val[1],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval2, FMTX130 );
				m_pArith->l_print( val[2],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval3, FMTX130 );
				//m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval4, FMTX130 );
				//m_pArith->l_print( val[4],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval5, FMTX130 );
				m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval5, FMTX130 );
				//m_pArith->l_print( val[5], (*m_pTaxListData)->NRec.KNval[l_cnt].KKval6, FMTX130);
				m_pArith->l_print( val[4], (*m_pTaxListData)->NRec.KNval[l_cnt].KKval6, FMTX130);
				//for(int idx = 0;idx<6;idx++){
				for(int idx = 0;idx<5;idx++){
					str.Format(val[idx]);
					str.Trim();
					// 10億円 ( カンマ含む )以上ならフォントサイズを下げる
					//if( str.GetLength() > 13 ){
					if ( str.GetLength() > 12 ) {
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
// 印刷３
//-----------------------------------------------------------------------------
// 引数
// 
// 
// 
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
//int CShinNougyou31Ex::PrintHyo3( int pno, int fno, CDC *pDC, char *pBp )
int CShinNougyou31Ex::PrintHyo4( int pno, int fno, CDC *pDC, char *pBp )
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
	pK = &(*m_pTaxListData)->NRec.KNval[32];
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
	pK = &(*m_pTaxListData)->NRec.KNval[32];
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
	pK = &(*m_pTaxListData)->NRec.KNval[32];
	::ZeroMemory( buf, sizeof(buf) );
	if( DispJudge( 2, KNOULINE31 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval3, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );
	i++;

//// うち税率６．３％適用分 D
//	for( j=0; (j+lin)<max; j++ ){
//		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
//		::ZeroMemory( buf, sizeof(buf) );
//		if( DispJudge( 3, j+lin ) == 1 ){
//			m_pArith->l_print( buf, pK->KKval4, FMTX130 );
//		}
//		len = (int)strlen( buf );
//		memmove( pBpMv, buf, len );
//		PF[i].PITM[j] = pBpMv;
//		pBpMv += ( len+1 );
//	}
//	// 計31
////	pK = &(*m_pTaxListData)->NRec.KJval[32];
//	pK = &(*m_pTaxListData)->NRec.KNval[32];
//	::ZeroMemory( buf, sizeof(buf) );
//	if( DispJudge( 3, KNOULINE31 ) == 1 ){
//		m_pArith->l_print( buf, pK->KKval4, FMTX130 );
//	}
//	len = (int)strlen( buf );
//	memmove( pBpMv, buf, len );
//	PF[i].PITM[j] = pBpMv;
//	pBpMv += ( len+1 );
//	i++;

// うち税率６．２４％適用分 E
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KNval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 3, j+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval5, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	// 計31
//	pK = &(*m_pTaxListData)->NRec.KJval[32];
	pK = &(*m_pTaxListData)->NRec.KNval[32];
	::ZeroMemory( buf, sizeof(buf) );
	if( DispJudge( 3, KNOULINE31 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval5, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len+1 );
	i++;

	// うち税率７．８％適用分 E
	for (j = 0; (j + lin) < max; j++) {
		pK = &(*m_pTaxListData)->NRec.KNval[j + lin];
		::ZeroMemory(buf, sizeof(buf));
		if (DispJudge(4, j + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval6, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	// 計31
//	pK = &(*m_pTaxListData)->NRec.KJval[32];
	pK = &(*m_pTaxListData)->NRec.KNval[32];
	::ZeroMemory(buf, sizeof(buf));
	if (DispJudge(4, KNOULINE31) == 1) {
		m_pArith->l_print(buf, pK->KKval6, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i].PITM[j] = pBpMv;
	pBpMv += (len + 1);
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
	pK = &(*m_pTaxListData)->NRec.KNval[35];
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
	pK = &(*m_pTaxListData)->NRec.KNval[36];
	::ZeroMemory( buf, sizeof(buf) );
//	int st = 0;
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
	pK = &(*m_pTaxListData)->NRec.KNval[37];
	::ZeroMemory( buf, sizeof(buf) );
//	for( j = 0, st = 0; j < 36; j++ ){
//		if( l_test( (*m_pTaxListData)->NRec.KNval )){
//			st = 1;		break;
//		}
//	}
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
			if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)||(m_pArith->l_test(pK->KKval4)!=0)||(m_pArith->l_test(pK->KKval5)!=0)|| (m_pArith->l_test(pK->KKval6) != 0)){ //金額が全て０なら表示しない
//2016.06.22 UPDATE START
//				(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmsub, ID_ICSSH_KJ_NOUGYOU, ((j+lin)+1), tmpStr );
				CString no;
				int tmp = 0;
				//if( j == 0 ){
					(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmsub, ID_ICSSH_KJ_NOUGYOU, (((j-3)+lin)+1), tmpStr ); //既に記憶されてる名称がずれるため応急処置
				//}else{
				//	(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmsub, ID_ICSSH_KJ_NOUGYOU, (((j-2)+lin)+1), tmpStr ); //既に記憶されてる名称がずれるため応急処置
				//}
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

	int fontsize = 0;
	char val[6][128];
	CString str;
	for(int l_cnt = KNOULINE1; l_cnt <= KNOULINE36; l_cnt++){
		switch(l_cnt){
			case KNOULINE1:
			case KNOULINE2K:	
			case KNOULINE2J:
			case KNOULINE3:	
			case KNOULINE3M:	
			case KNOULINE4:	
			case KNOULINE5:	
			case KNOULINE6:	
			case KNOULINE7:
			case KNOULINE8:
			case KNOULINE9:
			case KNOULINE10:
			case KNOULINE11:
			case KNOULINE12:
			case KNOULINE13:
			case KNOULINE14:
			case KNOULINE15:
			case KNOULINE16:
			case KNOULINE17:
			case KNOULINE18:
			case KNOULINE19:
			case KNOULINE20:
			case KNOULINE21:
			case KNOULINE22:
			case KNOULINE23:
			case KNOULINE24:
			case KNOULINE25:
			case KNOULINE26:
			case KNOULINE27:
			case KNOULINE28:
			case KNOULINE29:
			case KNOULINE30:
			case KNOULINE31:
			case KNOULINE32:
			case KNOULINE33:
			case KNOULINE34:
			case KNOULINE35:
			case KNOULINE36:
				//for(int idx = 0;idx<6;idx++){
				for(int idx = 0;idx<5;idx++){
					::ZeroMemory( val[idx], sizeof( val[idx] ));
				}
				m_pArith->l_print( val[0],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval1, FMTX130 );
				m_pArith->l_print( val[1],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval2, FMTX130 );
				m_pArith->l_print( val[2],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval3, FMTX130 );
				//m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval4, FMTX130 );
				//m_pArith->l_print( val[4],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval5, FMTX130 );
				m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval5, FMTX130 );
				//m_pArith->l_print( val[5],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval6, FMTX130);
				m_pArith->l_print( val[4],(*m_pTaxListData)->NRec.KNval[l_cnt].KKval6, FMTX130);
				//for(int idx = 0;idx<6;idx++){
				for(int idx = 0;idx<5;idx++){
					str.Format(val[idx]);
					str.Trim();
					// 10億円 ( カンマ含む )以上ならフォントサイズを下げる
					//if( str.GetLength() > 13 ){
					if ( str.GetLength() > 12 ) {
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
		return( _xprn( pno, 7, PF, 0, pDC, 0 ));
	}

	return( _xprn( pno, 6, PF, 0, pDC, 0 ));
}


//-----------------------------------------------------------------------------
// 初期ポジションセット ('14.08.08)
//-----------------------------------------------------------------------------
void CShinNougyou31Ex::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
		if( (index = m_Nougyou31.GetPosition()) != -1) {
			m_Nougyou31.SetPosition(index);
			m_Nougyou31.SetFocus();
			return;
		}
	}

	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Nougyou31.SetPosition(KNOUVAL01Aex);
	}
	else{
		m_Nougyou31.SetPosition(KNOUKNAME27ex);
	}
	SetRedrawScroll( 0 );

}

void CShinNougyou31Ex::SetItemPositionEx()
{
	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Nougyou31.SetPosition(KNOUVAL01Aex);
	}
	else{
		m_Nougyou31.SetPosition(KNOUKNAME27ex);
	}
	SetRedrawScroll( 0 );
}

//-----------------------------------------------------------------------------
// 表示判定
// 返送値：0…表示なし　1…表示あり
// 引数　：clm…列　line…行
//-----------------------------------------------------------------------------
int  CShinNougyou31Ex::DispJudge( int clm, int line )
{
	int st = 0;

	switch( clm ){
		case 0:	// 決算額 A
			switch( line ){
				case KNOULINE3M:	//未成熟果樹収入
					break;

				case KNOULINE4:		//小計
					for( int j = KNOULINE1 ; j < KNOULINE4 ; j++ ){
						if( j == KNOULINE3M ){//未成熟果樹収入はスキップ（入力不可の為）
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
							st = 1;
							break;
						}
					}
					break;
				case KNOULINE7:		//計
//--> '15.01.09 INS START
					for( int j = KNOULINE1 ; j < KNOULINE4 ; j++ ){
						if( j == KNOULINE3M ){//未成熟果樹収入はスキップ（入力不可の為）
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
							st = 1;
							break;
						}
					}
//<-- '15.01.09 INS END
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE5].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE6].KKval1) ){
						st = 1;
					}
					break;
				case KNOULINE31:	//小計
					for( int j = KNOULINE8 ; j < KNOULINE31 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
							st = 1;
							break;
						}
					}
					break;

				case KNOULINE35:	//計
					for( int j = KNOULINE8 ; j < KNOULINE31 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
							st = 1;
							break;
						}
					}
					for( int j = KNOULINE32 ; j < KNOULINE36 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
							st = 1;
							break;
						}
					}
//					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE32].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE33].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE34].KKval1) ){
//						st = 1;
//					}
					break;

				case KNOULINE36:	//差引金額
					for( int j = KNOULINE1 ; j < KNOULINE4 ; j++ ){
						if( j == KNOULINE3M ){//未成熟果樹収入はスキップ（入力不可の為）
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
							st = 1;
							break;
						}
					}
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE5].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE6].KKval1) ){
						st = 1;
						break;
					}
					for( int j = KNOULINE8 ; j < KNOULINE31 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
							st = 1;
							break;
						}
					}
//					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE32].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE33].KKval1) || m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE34].KKval1) ){
//						st = 1;
//					}
					for( int j = KNOULINE32 ; j < KNOULINE36 ; j++ ){
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
		case 1:	// Aのうち課税取引にならないもの　B
			switch( line ){
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
					for( int j = KNOULINE1; j < KNOULINE4; j++ ){
						if( j == KNOULINE2K || j == KNOULINE3M){//家事消費,未成熟果樹収入はスキップ（入力不可の為）
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval2) ){
							st = 1;
							break;
						}
					}
					break;

				case KNOULINE31:	//経費　小計
					for( int j = KNOULINE8; j < KNOULINE31; j++ ){
						if(j == KNOULINE13 || j == KNOULINE14 || j == KNOULINE15 || j == KNOULINE16 || j == KNOULINE18){//農具費、農薬・衛生費、諸材料費、修繕費、作業用衣料費はスキップ（入力不可の為）
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval2) ){
							st = 1;
							break;
						}
					}
					break;
				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[line].KKval2) ){
						st = 1;
					}
					break;
			}
			break;
		case 2:	// 課税取引金額　C（A－B）
			switch( line ){
				case KNOULINE3M:	//未成熟果樹収入
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[line].KKval3 )){
						st = 1;
					}
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
					for( int j = KNOULINE1 ; j < KNOULINE4 ; j++ ){
						if( j != KNOULINE3M ){//家事消費,未成熟果樹収入はスキップ（入力不可の為）
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
								st = 1;
								break;
							}
						}
						if( j != KNOULINE2K && j != KNOULINE3M ){//家事消費,未成熟果樹収入はスキップ（入力不可の為）
							if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval2) ){
								st = 1;
								break;
							}
						}
					}
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[KNOULINE3M].KKval3) ){
						st = 1;
					}
					break;

				case KNOULINE31:	//経費　小計
					for( int j = KNOULINE8 ; j < KNOULINE31 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval1) ){
							st = 1;
							break;
						}
						if( j == KNOULINE13 || j == KNOULINE14 || j == KNOULINE15 || j == KNOULINE16 || j == KNOULINE18 ){//農具費、農薬・衛生費、諸材料費、修繕費、作業用衣料費はスキップ（入力不可の為）
							continue;
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

		//case 3:	// うち税率６．３％適用分　D
		//	// 簡易課税の時は6.3%も表示
		//	//if ( ( *m_pSnHeadData )->SVmzsw == 0 ) {
		//	//	break;
		//	//}
		//	if ( m_Util.GetKazeihoushiki ( m_pZmsub->zvol ) == ID_ICSSH_KANNI ) {
		//		// 簡易課税は常に表示
		//		;
		//	}
		//	else {
		//		// 原則課税は旧税率が存在すれば表示
		//		if ( ( *m_pSnHeadData )->SVmzsw == 0 ) {
		//			break;
		//		}
		//	}
		//	switch( line ){
		//		case KNOULINE5:		//農産物の棚卸高　期首
		//		case KNOULINE6:		//農産物の棚卸高　期末
		//		case KNOULINE7:		//計
		//		case KNOULINE19:	//農業共済掛金
		//		case KNOULINE20:	//減価償却費
		//		case KNOULINE23:	//利子割引料
		//		case KNOULINE26:	//貸倒金
		//		case KNOULINE32:	//農産物以外の棚卸高　期首
		//		case KNOULINE33:	//農産物以外の棚卸高　期末
		//		case KNOULINE34:	//経費から差し引く果樹牛馬等の育成費用
		//		case KNOULINE35:	//計
		//		case KNOULINE36:	//差引金額
		//			//入力可所無し
		//			break;

		//		case KNOULINE4:		//収入金額　小計	
		//			for( int j = KNOULINE1; j < KNOULINE4; j++ ){
		//				if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval4) ){
		//					st = 1;
		//					break;
		//				}
		//			}
		//			break;

		//		case KNOULINE31:	//経費　小計
		//			for( int j = KNOULINE8; j < KNOULINE31; j++ ){
		//				if(j == KNOULINE19 || j == KNOULINE20 || j == KNOULINE23 || j == KNOULINE26){//農業共済掛金、減価償却費、利子割引料、貸倒金、はスキップ（入力不可の為）
		//					continue;
		//				}
		//				if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval4) ){
		//					st = 1;
		//					break;
		//				}
		//			}
		//			break;
		//		default:
		//			if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[line].KKval4) ){
		//				st = 1;
		//			}
		//			break;
		//	}
		//	break;

		//case 4:	// うち税率６．２４％適用分　E
		case 3:	// うち税率６．２４％適用分　D
			switch( line ){
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

				//--->新様式チェック時は下記項目は斜線部分なので非表示yoshida201216
				case KNOULINE8:		//租税公課
				case KNOULINE10:	//素畜費
				case KNOULINE13:	//農具費
				case KNOULINE14:	//農薬・衛生費
				case KNOULINE15:	//諸材料費
				case KNOULINE16:	//修繕費
				case KNOULINE17:	//動力光熱費
				case KNOULINE18:	//作業用衣料費
				case KNOULINE21:	//荷造運賃手数料
				case KNOULINE22:	//雇人費
				case KNOULINE24:	//地代・賃貸料
				case KNOULINE25:	//土地改良費
					if ((*m_pSnHeadData)->m_DispTabSgn & 0x1000) {//新様式チェックあり
					}
					else {//新様式チェック無し
						if (m_pArith->l_test((*m_pTaxListData)->NRec.KNval[line].KKval5)) {
							st = 1;
						}
					}
					break;
				//<------

				case KNOULINE4:		//収入金額　小計	
					for( int j = KNOULINE1; j < KNOULINE4; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval5) ){
							st = 1;
							break;
						}
					}
					break;

				case KNOULINE31:	//経費　小計
					for( int j = KNOULINE8; j < KNOULINE31; j++ ){
						if(j == KNOULINE19 || j == KNOULINE20 || j == KNOULINE23 || j == KNOULINE26){//農業共済掛金、減価償却費、利子割引料、貸倒金、はスキップ（入力不可の為）
							continue;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval5) ){
							st = 1;
							break;
						}
					}
					break;
				default:
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KNval[line].KKval5 )){
						st = 1;
					}
					break;
			}
			break;

		//case 5:	// うち税率７．８％適用分　E
		case 4:	// うち税率７．８％適用分　E
			switch (line) {
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
				for (int j = KNOULINE1; j < KNOULINE4; j++) {
					if (m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval6)) {
						st = 1;
						break;
					}
				}
				break;

			case KNOULINE31:	//経費　小計
				for (int j = KNOULINE8; j < KNOULINE31; j++) {
					if (j == KNOULINE19 || j == KNOULINE20 || j == KNOULINE23 || j == KNOULINE26) {//農業共済掛金、減価償却費、利子割引料、貸倒金、はスキップ（入力不可の為）
						continue;
					}
					if (m_pArith->l_test((*m_pTaxListData)->NRec.KNval[j].KKval6)) {
						st = 1;
						break;
					}
				}
				break;
			default:
				if (m_pArith->l_test((*m_pTaxListData)->NRec.KNval[line].KKval6)) {
					st = 1;
				}
				break;
			}
			break;
		default:
			break;
	}
	return st;
}

// 手入力サインのクリア
int CShinNougyou31Ex::ClearManualInputSign()
{
	int rt = 0;

	for( int idx = 0; idx < 50; idx++ ){
		(*m_pTaxListData)->NRec.KNval[idx].KKauto = 0x00;
	}

	return rt;
}

////=====================================================
//// 旧税率欄のチェック
////-----------------------------------------------------
//// 引数
////		index	:		ダイアグラム内インデックス
////-----------------------------------------------------
//// 返送値
////		1		:		旧税率 ( 6.3% )欄
////=====================================================
//int CShinNougyou31Ex::CheckSVmzsw ( int index )
//{
//	int ret = 0;
//
//	if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
//		return ret;
//	}
//
//	switch ( index ) {
//	case KNOUVAL01D:
//	case KNOUVAL02KD:
//	case KNOUVAL02JD:
//	case KNOUVAL03D:
//	case KNOUVAL03MD:
//	case KNOUVAL04D:
//	case KNOUVAL04TD:
//	case KNOUVAL05D:
//	case KNOUVAL06D:
//	case KNOUVAL07D:
//		ret = 1;
//		break;
//	case KNOUVAL08D:
//	case KNOUVAL09D:
//	case KNOUVAL10D:
//	case KNOUVAL11D:
//	case KNOUVAL12D:
//	case KNOUVAL13D:
//	case KNOUVAL14D:
//	case KNOUVAL15D:
//	case KNOUVAL16D:
//	case KNOUVAL17D:
//	case KNOUVAL18D:
//	case KNOUVAL19D:
//	case KNOUVAL20D:
//	case KNOUVAL21D:
//	case KNOUVAL22D:
//	case KNOUVAL23D:
//	case KNOUVAL24D:
//	case KNOUVAL25D:
//	case KNOUVAL26D:
//	case KNOUVAL27D:
//	case KNOUVAL28D:
//	case KNOUVAL29D:
//	case KNOUVAL30D:
//	case KNOUVAL31D:
//	case KNOUVAL32D:
//	case KNOUVAL33D:
//	case KNOUVAL34D:
//	case KNOUVAL35D:
//	case KNOUVAL36D:
//		if ( m_Util.GetKazeihoushiki ( m_pZmsub->zvol ) == ID_ICSSH_KANNI ) {
//			// 簡易課税の時は経費の欄を入力可能
//			;
//		}
//		else {
//			ret = 1;
//		}
//		break;
//	default:
//		break;
//	}
//
//	return ret;
//}

// 22-0167,22-0361 add -->
void CShinNougyou31Ex::CheckButtonIcsdiagctrl1(short index, LPUNKNOWN data)
{
	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;
	int				ii=0;
	CString			msg=_T("");

	idata = (DIAGRAM_DATA *)data;

	if(idata->data_check)	{
		msg = _T("うち標準税率7.8%適用分について\n");
		msg+= _T("うち軽減税率6.24%適用分を差し引いて自動計算を行いますか？\n");
		msg+= _T("※入力済の金額は自動計算の金額に上書きされるため、元には戻せません。");
	}
	else	{
		msg = _T("うち標準税率7.8%適用分の自動計算を解除しますか？");
	}
	if(ICSMessageBox(msg,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)	{
		//short	pos = m_Nougyou31.GetPosition();
		m_Nougyou31.DeleteInput();
		diag_getdata(IDC_ICSDIAGCTRL1,index,&xdata,CIcsdiagctrl);
		if(!idata->data_check)	xdata.data_check = 1;
		else					xdata.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1,index,&xdata,CIcsdiagctrl);
		//m_Nougyou31.SetPosition(pos);
		SetItemPositionEx();
		return;
	}

	// 現在は画面に自動計算チェックを１つしか設けていないので
	// 先頭レコードにのみ保存でもよいが
	// 今後のこと（ブロックごとに自動計算サインが必要という要望がきたとき）と
	// Shz_LinCalc関数内で[0]番目が参照できないことから
	// 全行に同じ状態の自動計算サインを書き込むことにする
	//for(ii=0; ii<50; ii++)	{
	//	(*m_pTaxListData)->NRec.KNval[ii].KKauto &= 0x2f;	// フラグの上位を一旦クリア
	//}
//----->('23.10.02 tanaka cor)インボイス対応で追加されたD12,D13のサインも落ちていた為、修正
//	(*m_pSnHeadData)->Sn_GenericSgn &= 0x7ff;
//------
	(*m_pSnHeadData)->Sn_GenericSgn &= 0xfffff7ff;
//<-----
	// チェックON
	if(idata->data_check)	{
		//// 自身以外のチェックをOFFにする
		//if(index == NOSELChk2ex)	{
		//	diag_getdata(IDC_ICSDIAGCTRL1,NOSELChk3ex,&xdata,CIcsdiagctrl);
		//	xdata.data_check = 0;
		//	diag_setdata(IDC_ICSDIAGCTRL1,NOSELChk3ex,&xdata,CIcsdiagctrl);
		//	for(ii=0; ii<50; ii++)	{
		//		(*m_pTaxListData)->NRec.KNval[ii].KKauto |= 0x40;
		//	}
		//}
		//else if(index == NOSELChk3ex)	{
		//	diag_getdata(IDC_ICSDIAGCTRL1,NOSELChk2ex,&xdata,CIcsdiagctrl);
		//	xdata.data_check = 0;
		//	diag_setdata(IDC_ICSDIAGCTRL1,NOSELChk2ex,&xdata,CIcsdiagctrl);
		//	for(ii=0; ii<50; ii++)	{
		//		(*m_pTaxListData)->NRec.KNval[ii].KKauto |= 0x80;
		//	}
		//}
		(*m_pSnHeadData)->Sn_GenericSgn |= 0x800;
	}
	// 入力可／不可を切り替える
	ColumnControl();
	// カーソルの終点を決める
	SetFocusRange();
	// 集計（自動計算チェックは全画面で共通のため、関係する画面の計算も行う）
	if(m_Util.IsUnConnectMst(m_pZmsub->zvol) && (m_Util.GetKazeihoushiki(m_pZmsub->zvol) == ID_ICSSH_KANNI))	{
		(*m_Scalc)->Shz_31AllCalc();		// 事業別売上高
		(*m_Scalc)->Shz_Sale31Post();		// 課税売上高
	}
	else	{
		if(m_pZmsub->zvol->ind_type&0x01)	{
			(*m_Scalc)->Shz_31AllCalc(1);	// 事業所得用
		}
		if(m_pZmsub->zvol->ind_type&0x04)	{
			(*m_Scalc)->Shz_31AllCalc(3);	// 農業所得用
		}
		if(m_pZmsub->zvol->ind_type&0x02)	{
			(*m_Scalc)->Shz_31AllCalc(2);	// 不動産所得用
		}
		(*m_Scalc)->Shz_Sale31Post();		// 課税売上高
		if(m_Util.GetKazeihoushiki(m_pZmsub->zvol) != ID_ICSSH_KANNI)	{
			(*m_Scalc)->Shz_Buy31Post();	// 課税仕入高
		}
	}
	// 再表示
	m_Nougyou31.OffControl(TRUE);
	for(int idx=0; idx<NKLINCNTex; idx++)	{
		DispNougyou31DiagData(idx);
	}
	SetItemPositionEx();
}

// ----------------------------------------------------------------------------------
// 自動計算チェックON/OFFによる、6.24%・7.8%の入力可／不可を切り替える
// ----------------------------------------------------------------------------------
// 【引　数】	なし
// 【返送値】	なし
// ----------------------------------------------------------------------------------
void CShinNougyou31Ex::ColumnControl( void )
{
	SH_ITEMINDEX	*pIndex=NULL;
	int				cnt=0;
	int				Flg=0;
	KKVAL*			pK = &( *m_pTaxListData )->NRec.KNval[0];

	if((*m_pSnHeadData )->m_DispTabSgn & 0x1000)	pIndex = R02_NougyouEx;
	else											pIndex = NougyouEx;
	while((pIndex+cnt)->Index != 0)	{
		// 確定時は全入力不可
		if((*m_pSnHeadData )->Sn_Sign4 & 0x01)	{
			Flg = (pIndex+cnt)->AllUnConnect;
		}
		else {
			if((*m_pSnHeadData)->Sn_Sign4 & 0x80)	{
				Flg = (pIndex+cnt)->UnConnect;
			}
			else	{
				Flg = (pIndex+cnt)->Connect;
			}
		}
		if(Flg)	{
			int		colm=GetColumnKind((pIndex+cnt)->Index);
			if(colm == 2)	{
				int	flg2=-1;	// 0:列が自動計算　1:列が自動計算ではない
				if(colm == 2)	{
					if((*m_pSnHeadData)->Sn_GenericSgn & 0x800)	flg2=0;
					else										flg2=1;
				}
				ATRIB_MOD(IDC_ICSDIAGCTRL1,(pIndex+cnt)->Index,flg2,(*m_pSnHeadData)->Sn_Sign4,(pIndex+cnt)->Connect);
				// 列が自動計算でなければ背景色を付ける
				if(flg2 != 0)	{
					DIAGRAM_ATTRIBUTE attr;
					diag_getattr(IDC_ICSDIAGCTRL1,(pIndex+cnt)->Index,&attr,CIcsdiagctrl);
					attr.attr_bcolor = BC_CREAM;
					diag_setattr(IDC_ICSDIAGCTRL1,(pIndex+cnt)->Index,&attr,FALSE,CIcsdiagctrl);
				}
			}
		}
		cnt++;
	}
}

// ----------------------------------------------------------------------------------
// 与えられたインデックス番号から列の種類を返す
// ----------------------------------------------------------------------------------
// 【引　数】	int		インデックス番号
// 【返送値】	int		1:うち軽減税率6.24%適用分(D)	2:うち標準税率7.8%適用分(E)
// ----------------------------------------------------------------------------------
int CShinNougyou31Ex::GetColumnKind( int pInd )
{
	int		rv=0;

	switch(pInd)	{
		case	KNOUVAL01Eex:
		case	KNOUVAL02KEex:
		case	KNOUVAL02JEex:
		case	KNOUVAL03Eex:
		case	KNOUVAL03MEex:
		case	KNOUVAL04Eex:
		case	KNOUVAL04TEex:
		case	KNOUVAL05Eex:
		case	KNOUVAL06Eex:
		case	KNOUVAL07Eex:
		case	KNOUVAL08Eex:
		case	KNOUVAL09Eex:
		case	KNOUVAL10Eex:
		case	KNOUVAL11Eex:
		case	KNOUVAL12Eex:
		case	KNOUVAL13Eex:
		case	KNOUVAL14Eex:
		case	KNOUVAL15Eex:
		case	KNOUVAL16Eex:
		case	KNOUVAL17Eex:
		case	KNOUVAL18Eex:
		case	KNOUVAL19Eex:
		case	KNOUVAL20Eex:
		case	KNOUVAL21Eex:
		case	KNOUVAL22Eex:
		case	KNOUVAL23Eex:
		case	KNOUVAL24Eex:
		case	KNOUVAL25Eex:
		case	KNOUVAL26Eex:
		case	KNOUVAL27Eex:
		case	KNOUVAL28Eex:
		case	KNOUVAL29Eex:
		case	KNOUVAL30Eex:
		case	KNOUVAL31Eex:
		case	KNOUVAL32Eex:
		case	KNOUVAL33Eex:
		case	KNOUVAL34Eex:
		case	KNOUVAL35Eex:
		case	KNOUVAL36Eex:
			rv=1;
			break;
		case	KNOUVAL01Fex:
		case	KNOUVAL02KFex:
		case	KNOUVAL02JFex:
		case	KNOUVAL03Fex:
		case	KNOUVAL03MFex:
		case	KNOUVAL04Fex:
		case	KNOUVAL04TFex:
		case	KNOUVAL05Fex:
		case	KNOUVAL06Fex:
		case	KNOUVAL07Fex:
		case	KNOUVAL08Fex:
		case	KNOUVAL09Fex:
		case	KNOUVAL10Fex:
		case	KNOUVAL11Fex:
		case	KNOUVAL12Fex:
		case	KNOUVAL13Fex:
		case	KNOUVAL14Fex:
		case	KNOUVAL15Fex:
		case	KNOUVAL16Fex:
		case	KNOUVAL17Fex:
		case	KNOUVAL18Fex:
		case	KNOUVAL19Fex:
		case	KNOUVAL20Fex:
		case	KNOUVAL21Fex:
		case	KNOUVAL22Fex:
		case	KNOUVAL23Fex:
		case	KNOUVAL24Fex:
		case	KNOUVAL25Fex:
		case	KNOUVAL26Fex:
		case	KNOUVAL27Fex:
		case	KNOUVAL28Fex:
		case	KNOUVAL29Fex:
		case	KNOUVAL30Fex:
		case	KNOUVAL31Fex:
		case	KNOUVAL32Fex:
		case	KNOUVAL33Fex:
		case	KNOUVAL34Fex:
		case	KNOUVAL35Fex:
		case	KNOUVAL36Fex:
			rv=2;
			break;
	}
	return(rv);
}
// 22-0167,22-0361 add <--

// 修正No.168690 add -->
void CShinNougyou31Ex::DiagReSetPosition(void)
{
	this->SetFocus();
}
// 修正No.168690 add <--
