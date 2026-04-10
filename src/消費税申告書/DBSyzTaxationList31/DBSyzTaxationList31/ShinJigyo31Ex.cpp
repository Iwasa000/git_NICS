// ShinJigyo31.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinJigyo31Ex.h"
#include "ShinJigyo31ExIdx.h"
#include "afxdialogex.h"

// CShinJigyo31Ex ダイアログ

IMPLEMENT_DYNAMIC(CShinJigyo31Ex, CSyzBaseDlg)

CShinJigyo31Ex::CShinJigyo31Ex(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(CShinJigyo31Ex::IDD, pParent)
	, m_pParent ( pParent )
	, m_Idx ( 0 )
	, m_Lin ( 0 )
	, m_Clm ( 0 )
{
	m_curwnd = -1;

	m_FcsUpEnd = -1;
	m_FcsLeftEnd = -1;
	m_FcsDownEnd = -1;
	m_FcsRightEnd = -1;
}

CShinJigyo31Ex::~CShinJigyo31Ex()
{
}

void CShinJigyo31Ex::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_JigyoDiag);
}


BEGIN_MESSAGE_MAP(CShinJigyo31Ex, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinJigyo31Ex メッセージ ハンドラー


BOOL CShinJigyo31Ex::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_JigyoDiag;
	//m_BottomIdx = 61;
	m_BottomIdx = 55;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//=====================================
// 初期化
//-------------------------------------
// 返送値
//		0		:		正常終了
//=====================================
int CShinJigyo31Ex::Init()
{
	InitDiagAttr();

	DispList ( 0 );

	SetFocusRange();
	SetItemPosition();

	return 0;
}

//=====================================
// ダイアグラム初期属性セット
//=====================================
void CShinJigyo31Ex::InitDiagAttr()
{
	DIAGRAM_ATTRIBUTE data;
	long dg1[] = { JIGNAME25ex, JIGNAME26ex, JIGNAME27ex, JIGNAME28ex, JIGNAME29ex, JIGNAME30ex, -1 };
	int KE = JIGVAL01Eex;

	for ( int idx = 0; ; idx++ ) {
		if ( dg1[idx] == -1 ) {
			break;
		}

		// ank対応
		m_JigyoDiag.GetAttribute ( ( short )dg1[idx], ( LPUNKNOWN )&data );
		data.attr_editattr |= DIAG_EATTR_ZENHAN;
		m_JigyoDiag.SetAttribute ( ( short )dg1[idx], ( LPUNKNOWN )&data, 1 );
	}


	SH_ITEMINDEX *pIndex = NULL;
	if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) {
		pIndex = R02_JigyoEx;
	}
	else {
		pIndex = JigyoEx;
	}

	int cnt = 0;
	while ( 1 ) {
		//if ( Jigyo[cnt].Index == 0 ) {
		if ( ( pIndex + cnt )->Index == 0 ) {
			break;
		}

		int Flg = 0;

		// 確定時は全入力不可
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			//Flg = Jigyo[cnt].AllUnConnect;
			Flg = ( pIndex + cnt )->AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				//if ( CheckSVmzsw ( Jigyo[cnt].Index ) == 0 ) {
					//Flg = Jigyo[cnt].UnConnect;
				/*if ( CheckSVmzsw ( ( pIndex + cnt )->Index ) == 0 ) {
					Flg = ( pIndex + cnt )->UnConnect;
				}
				else {
					Flg = 0;
				}*/
				Flg = ( pIndex + cnt )->UnConnect;
			}
			else {
				//Flg = Jigyo[cnt].Connect;
				Flg = ( pIndex + cnt )->Connect;
			}
		}

		//ATRIB_MOD ( IDC_ICSDIAGCTRL1, Jigyo[cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Jigyo[cnt].Connect );
		ATRIB_MOD ( IDC_ICSDIAGCTRL1, ( pIndex + cnt )->Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, ( pIndex + cnt )->Connect );
		cnt++;
	}

	// 斜線
	InitDiagonalLines();

	SetFocusRange();

	m_JigyoDiag.EnableDelete ( 0 );
}

//===============================================
// 斜線の表示/非表示
//===============================================
void CShinJigyo31Ex::InitDiagonalLines()
{
	long flg = 0; 
	if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) {
		flg = DIAG_MDFY_OPAQUE;
	}
	else {
		flg = DIAG_MDFY_TRANSPARENT;
	}

	for ( short index = JDIAGONALLINE_Sex; index <= JDIAGONALLINE_Eex; index++ ) {
		m_JigyoDiag.ModifyItem ( index, flg );
	}
}

//===============================================
// 経過措置のチェック
//-----------------------------------------------
// 引数
//		index	:	ダイアグラム内インデックス
//===============================================
int CShinJigyo31Ex::CheckSVmzsw ( int index )
{
	if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
		return 0;
	}

	int ret = 0;
	switch ( index ) {
	case JIGVAL01Dex:
	case JIGVAL012Dex:
		ret = 1;
		break;
	case JIGVAL02Dex:
	case JIGVAL03Dex:
	case JIGVAL04Dex:
	case JIGVAL05Dex:
	case JIGVAL06Dex:
	case JIGVAL07Dex:
	case JIGVAL08Dex:
	case JIGVAL09Dex:
	case JIGVAL10Dex:
	case JIGVAL11Dex:
	case JIGVAL12Dex:
	case JIGVAL13Dex:
	case JIGVAL14Dex:
	case JIGVAL15Dex:
	case JIGVAL16Dex:
	case JIGVAL17Dex:
	case JIGVAL18Dex:
	case JIGVAL19Dex:
	case JIGVAL20Dex:
	case JIGVAL21Dex:
	case JIGVAL22Dex:
	case JIGVAL23Dex:
	case JIGVAL24Dex:
	case JIGVAL25Dex:
	case JIGVAL26Dex:
	case JIGVAL27Dex:
	case JIGVAL28Dex:
	case JIGVAL29Dex:
	case JIGVAL30Dex:
	case JIGVAL31Dex:
	case JIGVAL32Dex:
	case JIGVAL33Dex:
	case JIGVAL34Dex:
		// 簡易課税の時は仕入、経費の欄を入力可能
		//ret = 1;
		if ( m_Util.GetKazeihoushiki ( m_pZmsub->zvol ) == ID_ICSSH_KANNI ) {
			;
		}
		else {
			ret = 1;
		}
		break;
	default:
		break;
	}

	return ret;
}

//=====================================
// ポジションの末端設定
//=====================================
void CShinJigyo31Ex::SetFocusRange()
{
	int cnt = 0;
	FCSUPDOWN_INFO *pInfo = NULL,
				   *pInfo_LR = NULL;

	if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) {
		cnt = R02_FCS_JIGYOCNTex;
		pInfo = R02_FCS_JigyoEx;
		pInfo_LR = R02_FCS_JigyoEx_LR;
	}
	else {
		cnt = FCS_JIGYOCNTex;
		pInfo = FCS_JigyoEx;
		pInfo_LR = FCS_JigyoEx_LR;
	}

	DIAGRAM_ATTRIBUTE DA;

	m_FcsUpEnd = -1;
	m_FcsLeftEnd = -1;
	m_FcsDownEnd = -1;
	m_FcsRightEnd = -1;

	// 上
	//for ( int idx = 0; idx < FCS_JIGYOCNT; idx++ ) {
	//	m_JigyoDiag.GetAttribute ( FCS_Jigyo[idx].IDNo, ( LPUNKNOWN )&DA );
	//	if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
	//		m_FcsUpEnd = FCS_Jigyo[idx].IDNo;
	//		break;
	//	}
	//}
	for ( int idx = 0; idx < cnt; idx++ ) {
		m_JigyoDiag.GetAttribute ( ( pInfo + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsUpEnd = ( pInfo + idx )->IDNo;
			break;
		}
	}

	// 左
	//for ( int idx = 0; idx < FCS_JIGYOCNT; idx++ ) {
	//	m_JigyoDiag.GetAttribute ( FCS_Jigyo_LR[idx].IDNo, ( LPUNKNOWN )&DA );
	//	if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
	//		m_FcsLeftEnd = FCS_Jigyo_LR[idx].IDNo;
	//		break;
	//	}
	//}
	for ( int idx = 0; idx < cnt; idx++ ) {
		m_JigyoDiag.GetAttribute ( ( pInfo_LR + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsLeftEnd = ( pInfo_LR + idx )->IDNo;
			break;
		}
	}

	// 下
	//for ( int idx = FCS_JIGYOCNT - 1; idx > -1; idx-- ) {
	//	m_JigyoDiag.GetAttribute ( FCS_Jigyo[idx].IDNo, ( LPUNKNOWN )&DA );
	//	if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
	//		m_FcsDownEnd = FCS_Jigyo[idx].IDNo;
	//		break;
	//	}
	//}
	for ( int idx = cnt - 1; idx > -1; idx-- ) {
		m_JigyoDiag.GetAttribute ( ( pInfo + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsDownEnd = ( pInfo + idx )->IDNo;
			break;
		}
	}

	// 左
	//for ( int idx = FCS_JIGYOCNT - 1; idx > -1; idx-- ) {
	//	m_JigyoDiag.GetAttribute ( FCS_Jigyo_LR[idx].IDNo, ( LPUNKNOWN )&DA );
	//	if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
	//		m_FcsRightEnd = FCS_Jigyo_LR[idx].IDNo;
	//		break;
	//	}
	//}
	for ( int idx = cnt - 1; idx > -1; idx-- ) {
		m_JigyoDiag.GetAttribute ( ( pInfo_LR + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsRightEnd = ( pInfo_LR + idx )->IDNo;
			break;
		}
	}

}


//=====================================
// 画面表示
//-------------------------------------
// 引数
//		mode	:
//-------------------------------------
// 返送値
//		0		:		正常終了
//=====================================
void CShinJigyo31Ex::DispList ( int mode )
{
// 22-0167,22-0361 add -->
	// 入力可／不可を切り替える
	ColumnControl();
// 22-0167,22-0361 add <--
	m_JigyoDiag.OffControl ( TRUE );
	( *m_Scalc )->Shz_31AllCalc ( 1 );

	short idx = 0;
	for ( idx = 0; idx < JIGLINCNTex; idx++ ) {
		DispJigyoDiagData ( idx );
	}

	DIAGRAM_DATA data;

	if ( !mode ) {
		m_Util.DiagOcxIniz ( &data );
		diag_clear ( IDC_ICSDIAGCTRL1, JIGYEARIDex, TRUE, CIcsdiagctrl );
		data.data_disp.Format (_T( "（令和%2x年分）" ), ( *m_pTaxListData )->YEAR );
		diag_setdata ( IDC_ICSDIAGCTRL1, JIGYEARIDex, &data, CIcsdiagctrl );

		PostMessage ( WM_VSCROLL, SB_TOP, 0 );
		m_Idx = JIGMINex;
	}

	m_Util.DiagOcxIniz ( &data );
	diag_clear ( IDC_ICSDIAGCTRL1, JIGZKBNIDex, TRUE, CIcsdiagctrl );
	data.data_disp = _T( "経理方式：" );
	if ( ( *m_pTaxListData )->Head.KHstax & 0x01 ) {
		data.data_disp += _T( "税抜き" );
	}
	else {
		data.data_disp += _T( "税込み" );
	}

	diag_setdata ( IDC_ICSDIAGCTRL1, JIGZKBNIDex, &data, CIcsdiagctrl );
	//diag_setposition ( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );

	int edit[] = { JIGNAME25ex, JIGNAME26ex, JIGNAME27ex, JIGNAME28ex, JIGNAME29ex, JIGNAME30ex };
	BOOL edit_focus = FALSE;

	for ( int cnt = 0; cnt < ARRAYCOUNT ( edit ); cnt++ ) {
		Switch_KamokuEdit ( IDC_ICSDIAGCTRL1, edit[cnt] );

		if (m_JigyoDiag.GetPosition() == edit[cnt] ) {
			edit_focus = TRUE;
		}
	}

	if ( ( ( *m_pTaxListData )->Head.KHkoke == 1 ) && ( edit_focus == TRUE ) ) {
		this->SetFocus();
	}
// 22-0167,22-0361 add -->
	KKVAL*	pK = &( *m_pTaxListData )->NRec.KJval[0];
	for(idx=JISELChk2ex; idx<=JISELChk3ex; idx++)	{
		diag_getdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
		data.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
	}
	idx=0;
	if((*m_pSnHeadData)->Sn_GenericSgn & 0x800)		idx=JISELChk3ex;
	//if(pK->KKauto & 0x40)		idx=JISELChk2ex;
	//else if(pK->KKauto & 0x80)	idx=JISELChk3ex;
	if(idx != 0)	{
		diag_getdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
		data.data_check = 1;
		diag_setdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
	}
	//// 確定時または連動時は選択不可
	//if(((*m_pSnHeadData)->Sn_Sign4 & 0x01) || (!((*m_pSnHeadData)->Sn_Sign4 & 0x80)))	{
	//	for(idx=JISELChk2ex; idx<=JISELChk3ex; idx++)	{
	//		ATRIB_MOD(IDC_ICSDIAGCTRL1,idx,0,(*m_pSnHeadData)->Sn_Sign4,0);
	//	}
	//}
	// 6.24%適用分は非表示にする(今後要望があったときのために残しておく)
	diag_modify(IDC_ICSDIAGCTRL1,JISELChk2ex,DIAG_MDFY_TRANSPARENT,CIcsdiagctrl);
	// 連動時は7.8%適用分も非表示にする
	if(!((*m_pSnHeadData)->Sn_Sign4 & 0x80))	{
		diag_modify(IDC_ICSDIAGCTRL1,JISELChk3ex,DIAG_MDFY_TRANSPARENT,CIcsdiagctrl);
	}
	// 確定時は7.8%適用分を選択不可にする
	else if((*m_pSnHeadData)->Sn_Sign4 & 0x01)	{
		//ATRIB_MOD(IDC_ICSDIAGCTRL1,JISELChk3ex,0,(*m_pSnHeadData)->Sn_Sign4,0);
		DIAGRAM_ATTRIBUTE	da;
		diag_modify(IDC_ICSDIAGCTRL1,JISELChk3ex,DIAG_MDFY_READONLY,CIcsdiagctrl);
		diag_getattr(IDC_ICSDIAGCTRL1,JISELChk3ex,&da,CIcsdiagctrl);
		da.attr_bcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAGCTRL1,JISELChk3ex,&da,FALSE,CIcsdiagctrl);
	}
	else	{
		//ATRIB_MOD(IDC_ICSDIAGCTRL1,JISELChk3ex,1,(*m_pSnHeadData)->Sn_Sign4,0);
		DIAGRAM_ATTRIBUTE	da,da2;
		diag_getattr(IDC_ICSDIAGCTRL1,0,&da2,CIcsdiagctrl);
		diag_getattr(IDC_ICSDIAGCTRL1,JISELChk3ex,&da,CIcsdiagctrl);
		da.attr_bcolor = da2.attr_bcolor;
		diag_setattr(IDC_ICSDIAGCTRL1,JISELChk3ex,&da,FALSE,CIcsdiagctrl);
		diag_modify(IDC_ICSDIAGCTRL1,JISELChk3ex,DIAG_MDFY_EDIT,CIcsdiagctrl);
	}
// 22-0167,22-0361 add <--
}

//=====================================
// ダイアグラムデータ表示
//-------------------------------------
// 引数
//		dlin	:
//=====================================
void CShinJigyo31Ex::DispJigyoDiagData ( long dlin )
{
	short idx = 0,
		  i   = 0;
	long  lin = 0;

	//データ保存ポジションセット
	static long KJigyoDataPos[] = { 0, 34, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
								   18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33 };

	lin = KJigyoDataPos[dlin];

	KKVAL*	pK = &( *m_pTaxListData )->NRec.KJval[lin];

	char val[6] = { 0 };
	DIAGRAM_DATA data;
	int st = 0;

	idx = ( short )( dlin * JIGWIDECNTex ) + JIGMINex;

	for ( i = 0; i < JIGWIDECNTex; i++ ) {
		st = 0;
		::ZeroMemory ( val, sizeof ( val ) );
		diag_clear ( IDC_ICSDIAGCTRL1, idx + i, TRUE, CIcsdiagctrl );
		m_Util.DiagOcxIniz ( &data );

		switch ( i ) {
		case 0:		// 決算額 A
			memcpy ( val, pK->KKval1, 6 );
			break;
		case 1:		// Aのうち課税取引にならないもの B
			memcpy ( val, pK->KKval2, 6 );
			break;
		case 2:		// 課税取引金額 C ( A - B )
			memcpy ( val, pK->KKval3, 6 );
			break;
		case 3:		// うち税率6.24%適用分 D
			memcpy ( val, pK->KKval5, 6 );
			break;
		case 4:		// うち税率7.8%適用分 D
			memcpy ( val, pK->KKval6, 6 );
			break;
		default:
			break;
		}

		if ( DispJudge ( i, lin ) == 1 ) {
			memcpy ( data.data_val, val, 6 );
			diag_setdata ( IDC_ICSDIAGCTRL1, idx + i, &data, CIcsdiagctrl );
		}
	}

	// 項目名
	char buf[128] = { 0 };
	if ( ( dlin >= KJIGLINE25 ) && ( dlin <= KJIGLINE30 ) ) {
		lin = KJigyoDataPos[dlin + 1];
		pK = &( *m_pTaxListData )->NRec.KJval[lin];

		if ( !( pK->KKsign & 0x80 ) ){
			m_Util.DiagOcxIniz ( &data );
			::ZeroMemory ( buf, sizeof ( buf ) );

			CString tmp;
			int prf_no = 0;

			switch ( dlin ) {
			case KJIGLINE25:
				idx = JIGNAME25ex;
				prf_no = 25;
				break;
			case KJIGLINE26:
				idx = JIGNAME26ex;
				prf_no = 26;
				break;
			case KJIGLINE27:
				idx = JIGNAME27ex;
				prf_no = 27;
				break;
			case KJIGLINE28:
				idx = JIGNAME28ex;
				prf_no = 28;
				break;
			case KJIGLINE29:
				idx = JIGNAME29ex;
				prf_no = 29;
				break;
			case KJIGLINE30:
				idx = JIGNAME30ex;
				prf_no = 30;
				break;
			}
			if ( ( *m_pTaxListData)->IsLinkKjprfname() && ( prf_no != 0 ) ) {
				if ( ( m_pArith->l_test ( pK->KKval1 ) != 0 ) || ( m_pArith->l_test ( pK->KKval2 ) != 0 ) || ( m_pArith->l_test ( pK->KKval3 ) != 0 ) || ( m_pArith->l_test ( pK->KKval5 ) != 0 ) || ( m_pArith->l_test ( pK->KKval6 ) != 0 ) ) {
					( *m_pTaxListData )->GetKoketsuMeisyo ( m_pZmsub, ID_ICSSH_KJ_IPPAN, prf_no, tmp );
					memcpy ( buf, tmp, tmp.GetLength() );
				}
			}
			else {
				memcpy ( buf, pK->KKnam, sizeof ( pK->KKnam ) );
			}

			diag_clear ( IDC_ICSDIAGCTRL1, idx, TRUE, CIcsdiagctrl );
			data.data_edit = buf;
			diag_setdata ( IDC_ICSDIAGCTRL1, idx, &data, CIcsdiagctrl );

		}
	}
}

//=====================================
// 表示判定
//-------------------------------------
// 引数
//		clm		:	列
//		line	:	行
//=====================================
int CShinJigyo31Ex::DispJudge ( int clm, int line )
{
	int st = 0;
	int j = 0;

	switch ( clm  ){
	case 0:		// 決算額　A
		switch( line ){
		case KJIGLINE33:	//差引税額 ( 下 )
		case KJIGLINE7:		//差引税額 ( 上 )
			if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[KJIGLINE1].KKval1 ) ) {
				st = 1;
			}
		case KJIGLINE6:		//売上原価 差引原価
			if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[KJIGLINE5].KKval1 ) ) {
				st = 1;
			}
		case KJIGLINE4:		//売上原価 小計
			if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[KJIGLINE2].KKval1 ) || m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[KJIGLINE3].KKval1 ) ) {
				st = 1;
			}
			if ( line != KJIGLINE33 ) {
				break;
			}
		case KJIGLINE32:	//経費 計
			for ( j = KJIGLINE8; j < KJIGLINE32 ; j++ ) {
				if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[j].KKval1 ) ) {
					st = 1;
					break;
				}
			}
			break;
		default:
			if  ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[line].KKval1 ) ) {
				st = 1;
			}
			break;
		}
		break;
	case 1:	// Aのうち課税取引にならないもの　B
		switch ( line ) {
		case KJIGLINE2:		 //期首商品棚卸高
		case KJIGLINE4:		 //小計
		case KJIGLINE5:		 //期末商品棚卸高
		case KJIGLINE6:		 //差引原価
		case KJIGLINE7:		 //差引金額
		case KJIGLINE10:	 //水道光熱費
		case KJIGLINE16:	 //修繕費
		case KJIGLINE17:	 //消耗品費
		case KJIGLINE33:	 //差引金額
							 //入力不可の為処理しない
			break;
		case KJIGLINE32:	//経費　計
			for ( j = KJIGLINE8; j < KJIGLINE32; j++ ) {
				if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[j].KKval2 ) ) {
					st = 1;	
					break;
				}
			}
			break;
		default:
			if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[line].KKval2 ) ) {
				st = 1;
			}
			break;
		}
		break;
	case 2:		// 課税取引金額 C ( A - B )
		switch( line ){
		case KJIGLINE2:		 //期首商品棚卸高
		case KJIGLINE4:		 //小計
		case KJIGLINE5:		 //期末商品棚卸高
		case KJIGLINE6:		 //差引原価
		case KJIGLINE7:		 //差引金額
		case KJIGLINE15:	 //損害保険料
		case KJIGLINE18:	 //減価償却費
		case KJIGLINE22:	 //利子割引料
		case KJIGLINE24:	 //貸倒金
		case KJIGLINE33:	 //差引金額
							 //入力不可の為処理しない
			break;
		case KJIGLINE32:	//経費 計
			for ( j = KJIGLINE8; j < KJIGLINE32; j++ ) {
				if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[j].KKval1 ) ) {
					st = 1;
					break;
				}
				if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[j].KKval2 ) ) {
					st = 1;
					break;
				}
			}
			break;
		default:
			if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[line].KKval1 ) || m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[line].KKval2 ) ) {
				st = 1;
			}
			break;
		}
		break;
	case 3:		// うち税率6.24%適用分 D
		switch( line ){
		case KJIGLINE2:		 //期首商品棚卸高
		case KJIGLINE4:		 //小計
		case KJIGLINE5:		 //期末商品棚卸高
		case KJIGLINE6:		 //差引原価
		case KJIGLINE7:		 //差引金額
		case KJIGLINE15:	 //損害保険料
		case KJIGLINE18:	 //減価償却費
		case KJIGLINE22:	 //利子割引料
		case KJIGLINE24:	 //貸倒金
		case KJIGLINE33:	 //差引金額
							 //入力不可の為処理しない
			break;

		//--->新様式チェック時は下記項目は斜線部分なので非表示yoshida201216
		case KJIGLINE8:		 //租税公課
		case KJIGLINE9:		 //荷造運賃
		case KJIGLINE10:	 //水道光熱費
		case KJIGLINE11:	 //旅費交通費
		case KJIGLINE16:	 //修繕費
		case KJIGLINE20:	 //給料賃金
		case KJIGLINE21:	 //外注工賃
		case KJIGLINE23:	 //地代家賃
			if ((*m_pSnHeadData)->m_DispTabSgn & 0x1000) {//新様式チェックあり
			}
			else {//新様式チェック無し
				if (m_pArith->l_test((*m_pTaxListData)->NRec.KJval[line].KKval5)) {
					st = 1;
				}
			}
			break;
		//<------

		case KJIGLINE32:	 //経費 計
			for ( j = KJIGLINE8; j < KJIGLINE32; j++ ) {
				if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[j].KKval5 ) ) {
					st = 1;
					break;
				}
			}
			break;
		default:
			if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[line].KKval5 ) ){
				st = 1;
			}
			break;
		}
		break;
	case 4:		// うち税率7.8%適用分 E
		switch( line ){
		case KJIGLINE2:		 //期首商品棚卸高
		case KJIGLINE4:		 //小計
		case KJIGLINE5:		 //期末商品棚卸高
		case KJIGLINE6:		 //差引原価
		case KJIGLINE7:		 //差引金額
		case KJIGLINE15:	 //損害保険料
		case KJIGLINE18:	 //減価償却費
		case KJIGLINE22:	 //利子割引料
		case KJIGLINE24:	 //貸倒金
		case KJIGLINE33:	 //差引金額
							 //入力不可の為処理しない
			break;
		case KJIGLINE32:	 //経費 計
			for ( j = KJIGLINE8; j < KJIGLINE32; j++ ) {
				if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[j].KKval6 ) ) {
					st = 1;
					break;
				}
			}
			break;
		default:
			if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[line].KKval6 ) ){
				st = 1;
			}
			break;
		}
		break;
	}

	return st;
}

//=====================================
// 各入力データ取得
//-------------------------------------
// 引数
//		index	:
//		leng	:
//		*pData	:
//		mode	:
//=====================================
int CShinJigyo31Ex::GetDiagData ( long index, short leng, DIAGRAM_DATA *pData, int mode )
{
// 22-0167,22-0361 cor -->
	//if ( index < 0 ) {
// -----------------------
	if(index < 0 || index == JISELChk2ex || index == JISELChk3ex) {
// 22-0167,22-0361 cor <--
		return 0;
	}

	char dbuf[128] = { 0 };
	char *buff = NULL;
	int	len = 0;

	len = pData->data_edit.GetLength();
	memset ( dbuf, '\0', sizeof ( dbuf ) );
	buff = ( char* )( ( LPCTSTR )( pData->data_edit ) );
	memmove ( dbuf, buff, len );	

	long lin = 0;

	//データ保存ポジションセット
	static long JigyoDataPos[] = { 0, 34, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
								  18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33 };

	lin = JigyoDataPos[m_Lin];

	KKVAL*	pK = &( *m_pTaxListData )->NRec.KJval[lin];

	if ( !mode ) {
		switch ( m_Clm ) {
		case 0:		// 決算額	A
			if ( leng ){
				memcpy ( pK->KKval1, pData->data_val, 6 );
			}
			break;
		case 1:		// Aのうち課税取引にならないもの B
			if ( leng ) {
				memcpy ( pK->KKval2, pData->data_val, 6 );
			}
			break;
		case 2:		// 課税取引金額 C ( A - B )
			if ( leng ) {
				memcpy ( pK->KKval3, pData->data_val, 6 );
			}
			break;
		case 3:		// うち税率6.24%適用分 E
			if ( leng ) {
				memcpy ( pK->KKval5, pData->data_val, 6 );
			}
			break;
		case 4:		// うち税率7.8%適用分 F
			if ( leng ) {
				memcpy ( pK->KKval6, pData->data_val, 6 );
			}
			break;
		default:	// 項目名
			memcpy ( pK->KKnam, dbuf, sizeof ( pK->KKnam ) );
			break;
		}
	}
	else {
		switch ( m_Clm ) {
		case 0:		// 決算額 A
			::ZeroMemory ( pK->KKval1, 6 );
			break;
		case 1:		// Aのうち課税取引にならないもの B
			::ZeroMemory ( pK->KKval2, 6 );
			break;
		case 2:		// 課税取引金額 C（ A － B ）
			::ZeroMemory ( pK->KKval3, 6 );	
			break;
		case 3:		// うち税率6.24%適用分 E
			::ZeroMemory ( pK->KKval5, 6 );	
			break;
		case 4:		// うち税率7.8%適用分 F
			::ZeroMemory ( pK->KKval6, 6 );	
			break;
		default:	// 項目名
			::ZeroMemory ( pK->KKnam, sizeof ( pK->KKnam ) );
			break;
		}
	}

	// 集計
	( *m_Scalc )->Shz_31AllCalc ( 1 );

	for ( int idx = 0; idx < JIGLINCNTex; idx++ ) {
		DispJigyoDiagData ( idx );
	}

	return 0;
}

//=====================================
// 手入力サインのクリア
//=====================================
int CShinJigyo31Ex::ClearManualInputSign()
{
	for ( int idx = 0; idx < 50; idx++ ) {
		( *m_pTaxListData )->NRec.KJval[idx].KKauto = 0x00;
	}

	return 0;
}

#define		Msiz	512 * 20
//=====================================
// 印刷情報のセット
//-------------------------------------
// 引数
//		*pVprn			:
//		*pPset			:
//		*pPrintCmInfo	:
//-------------------------------------
// 返送値
//		0		:		正常終了
//=====================================
void CShinJigyo31Ex::SetPrintInfo ( CVPrint *pVprn, NPRT_PAR *pPset, SH_PRINT_CMINFO *pPrintCmInfo )
{
	ASSERT ( pVprn );
	ASSERT ( pPset );

	m_pVprn = pVprn;
	m_pPset = pPset;
	m_PrintCmInfo = pPrintCmInfo;
}

//=====================================
// 印刷１
//-------------------------------------
// 引数
//		pno		:
//		fno		:
//		*pDC	:
//		*pBp	:
//-------------------------------------
// 返送値
//		0		:		正常終了
//=====================================
int CShinJigyo31Ex::PrintHyo1 ( int pno, int fno, CDC *pDC, char *pBp )
{
	int i = 0,
		j = 0;
	int len = 0;

	char cnm[128] = { 0 },
		 buf[128] = { 0 },
		 CVAL[6]  = { 0 };

	KKVAL* pJ = ( *m_pTaxListData )->NRec.KJval;

	// 初期化
	memset ( PF, '\0', sizeof ( PF ) );

	i = 0;
	memset ( pBp, '\0', Msiz );
	char *pBpMv = pBp;

	::ZeroMemory ( CVAL, sizeof ( CVAL ) );

	// 会社名
	::ZeroMemory ( buf, sizeof ( buf ) );
	::ZeroMemory ( cnm, sizeof ( cnm ) );

	if (m_pPset->psSigCoprn & 0x2) {
		// 顧問先
		if (m_PrintCmInfo->OPL) {
			sprintf_s(buf, sizeof(buf), _T(" %s"), m_PrintCmInfo->CopNam);
		}
		else {
			sprintf_s(buf, sizeof(buf), _T("[%08d] %s"), m_PrintCmInfo->CopNum, m_PrintCmInfo->CopNam);
		}
	}
	else {
		// マスター
		if ((*m_pSnHeadData)->IsSoleProprietor()) {
			j = sizeof((*m_pSnHeadData)->Sn_DIHYO);
			memmove(cnm, (*m_pSnHeadData)->Sn_DIHYO, j);
		}
		else {	// 法人名
			j = sizeof((*m_pSnHeadData)->Sn_CONAM);
			memmove(cnm, (*m_pSnHeadData)->Sn_CONAM, j);
		}

		if (m_PrintCmInfo->OPL) {
			sprintf_s(buf, sizeof(buf), _T(" %s"), cnm);
		}
		else {
			sprintf_s(buf, sizeof(buf), _T("[%08d] %s"), m_pZmsub->zvol->v_cod, cnm);
		}
	}

	len = ( int )strlen ( buf );
	memmove ( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len + 1 );

	// 年分
	::ZeroMemory ( buf, sizeof ( buf ) );
	sprintf_s ( buf, sizeof ( buf ), _T( "%x" ), ( *m_pTaxListData )->YEAR );

	memmove ( pBpMv, buf, 2 );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( 2 + 1 );

	return _xprn ( pno, fno, PF, 0, pDC, 0, 1 );
}

//=====================================
// 印刷２
//-------------------------------------
// 引数
//		pno		:
//		fno		:
//		*pDC	:
//		*pBp	:
//-------------------------------------
// 返送値
//		0		:		正常終了
//=====================================
int CShinJigyo31Ex::PrintHyo2 ( int pno, int fno, CDC *pDC, char *pBp )
{
	int	i = 0,
		j = 0;

	int lin = 0,
		max = 0;
	int len = 0, 
		st  = 0;

	char cnm[128] = { 0 },
		 buf[128] = { 0 },
		 CVAL[6]  = { 0 };

	KKVAL* pK = NULL;
	KKVAL* pJ = ( *m_pTaxListData )->NRec.KJval;

	// 初期化
	memset ( PF, '\0', sizeof ( PF ) );

	i = 0;
	memset ( pBp, '\0', Msiz );
	char *pBpMv = pBp;

	::ZeroMemory ( CVAL, sizeof ( CVAL ) );

	// 会社名
	::ZeroMemory ( buf, sizeof ( buf ) );
	::ZeroMemory ( cnm, sizeof ( cnm ) );

	// 決算額 A
	// Aのうち課税取引にならないもの B
	// 課税取引額 C
	lin = 0;
	max = 5;
	pK = &( *m_pTaxListData )->NRec.KJval[0];

	for ( j = 0; j < max; j++ ) {
		st = 0;
		st = DispJudge ( j, 0 );

		::ZeroMemory ( buf, sizeof ( buf ) );
		switch ( j ){
		case 0:
			if ( st == 1 ) {
				m_pArith->l_print ( buf, pK->KKval1, FMTX130 );
			}
			break;
		case 1:
			if ( st == 1 ) {
				m_pArith->l_print ( buf, pK->KKval2, FMTX130 );
			}
			break;
		case 2:
			if ( st == 1 ) {
				m_pArith->l_print ( buf, pK->KKval3, FMTX130 );
			}
			break;
		case 3:
			if ( st == 1 ){
				m_pArith->l_print ( buf, pK->KKval5, FMTX130 );
			}
			break;
		case 4:
			if ( st == 1 ){
				m_pArith->l_print ( buf, pK->KKval6, FMTX130 );
			}
			break;
		}

		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[0] = pBpMv;
		pBpMv += ( len + 1 );
		i++;
	}

	// 決算額 A
	lin = 1;
	max = 7;

	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		st = 0;

		::ZeroMemory ( buf, sizeof ( buf ) );
		if ( DispJudge ( 0, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval1, FMTX130 );
		}
		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}
	i++;

	// Aのうち課税取引にならないもの B
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		::ZeroMemory( buf, sizeof(buf) );

		if ( DispJudge( 1, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval2, FMTX130 );
		}
		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}
	i++;

	// 課税取引額 C
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		st = 0;

		::ZeroMemory ( buf, sizeof ( buf ) );
		if ( DispJudge( 2, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval3, FMTX130 );
		}
		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}
	i++;

	// うち税率6.24%適用分 E
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
		if ( DispJudge ( 3, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval5, FMTX130 );
		}
		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}
	i++;

	// うち税率7.8%適用分 F
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );
		if ( DispJudge ( 4, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval6, FMTX130 );
		}
		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}
	i++;

	CString	tmpStr;
	bool isLinkKjprf = ( *m_pTaxListData )->IsLinkKjprfname();

	// 経費 項目名 25 - 30
	lin = 24;
	max = 30;
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		::ZeroMemory ( buf, sizeof ( buf ) );
		if ( isLinkKjprf ) {
			tmpStr.Empty();
			if ( ( m_pArith->l_test ( pK->KKval1 ) != 0 ) || ( m_pArith->l_test ( pK->KKval2 ) != 0 ) || ( m_pArith->l_test ( pK->KKval3 ) != 0 ) || ( m_pArith->l_test ( pK->KKval5 ) != 0 ) || ( m_pArith->l_test ( pK->KKval6 ) != 0 ) ) { //金額が全て０なら表示しない
				( *m_pTaxListData )->GetKoketsuMeisyo ( m_pZmsub, ID_ICSSH_KJ_IPPAN, ( ( j + lin ) + 1 ), tmpStr );
			}
			len = tmpStr.GetLength();
			memmove ( pBpMv, tmpStr, len );
		}
		else {
			len = ( int )strlen ( pK->KKnam );
			memmove ( pBpMv, pK->KKnam , len );
		}

		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}

	int fontsize = 0;
	char val[5][128] = { 0, 0 };
	CString str;

	for ( int l_cnt = KJIGLINE1; l_cnt <= KJIGLINE33; l_cnt++ ) {
		switch ( l_cnt ) {
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
			for ( int idx = 0; idx < 5; idx++ ) {
				::ZeroMemory ( val[idx], sizeof ( val[idx] ) );
			}

			m_pArith->l_print ( val[0], ( *m_pTaxListData )->NRec.KJval[l_cnt].KKval1, FMTX130 );
			m_pArith->l_print ( val[1], ( *m_pTaxListData )->NRec.KJval[l_cnt].KKval2, FMTX130 );
			m_pArith->l_print ( val[2], ( *m_pTaxListData )->NRec.KJval[l_cnt].KKval3, FMTX130 );
			m_pArith->l_print ( val[3], ( *m_pTaxListData )->NRec.KJval[l_cnt].KKval5, FMTX130 );
			m_pArith->l_print ( val[4], ( *m_pTaxListData )->NRec.KJval[l_cnt].KKval6, FMTX130 );
			for ( int idx = 0; idx < 5; idx++ ) {
				str.Format ( val[idx] );
				str.Trim();
				// 10億円 ( カンマ含む )以上ならフォントサイズを下げる
				//if ( str.GetLength() > 13 ) {
				if ( str.GetLength() > 12 ) {
					fontsize = 1;
				}
			}
			break;
		default:
			break;
		}

		if ( fontsize != 0 ) {
			break;
		}
	}

	if ( fontsize == 1 ) {
		return _xprn ( pno, 3, PF, 0, pDC, 0, 1 );
	}

	return _xprn ( pno, 2, PF, 0, pDC, 0, 1 );
}

//=====================================
// 印刷３
//-------------------------------------
// 引数
//		pno		:
//		fno		:
//		*pDC	:
//		*pBp	:
//-------------------------------------
// 返送値
//		0		:		正常終了
//=====================================
int CShinJigyo31Ex::PrintHyo3 ( int pno, int fno, CDC *pDC, char *pBp )
{
	int i = 0,
		j = 0,
		k = 0;

	int len = 0,
		lin = 0,
		max = 0;

	char buf[64] = { 0 };
	KKVAL* pK = NULL;
	KKVAL* pJ = ( *m_pTaxListData )->NRec.KJval;

	i = 0;
	memset( pBp, '\0', Msiz );
	char *pBpMv = pBp;

	// 決算額 A
	lin = 7;
	max = 24;
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		::ZeroMemory ( buf, sizeof ( buf ) );
		if ( DispJudge ( 0, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval1, FMTX130 );
		}
		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}
	i++;

	// Aのうち課税取引にならないもの B
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		::ZeroMemory ( buf, sizeof ( buf ) );
		if ( DispJudge( 1, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval2, FMTX130 );
		}
		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}
	i++;

	// 課税取引額 C
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		::ZeroMemory ( buf, sizeof ( buf ) );
		if ( DispJudge ( 2, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval3, FMTX130 );
		}
		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}
	i++;

	// うち税率6.24%適用分 E
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		::ZeroMemory ( buf, sizeof ( buf ) );
		if ( DispJudge ( 3, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval5, FMTX130 );
		}
		len = ( int) strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}
	i++;

	// うち税率7.8%適用分 E
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		::ZeroMemory ( buf, sizeof ( buf ) );
		if ( DispJudge ( 4, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval6, FMTX130 );
		}
		len = ( int) strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}
	i++;
	
	// 経費 ( 25 - 32 )
	int st = 0;

	lin = 24;
	max = 31;
	// 決算額 A
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		::ZeroMemory ( buf, sizeof ( buf) );
		if ( DispJudge ( 0, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval1, FMTX130 );
		}
		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}

	// 計32
	pK = &( *m_pTaxListData )->NRec.KJval[31];
	::ZeroMemory ( buf, sizeof( buf ) );
	if ( DispJudge ( 0, KJIGLINE32 ) == 1 ) {
		m_pArith->l_print ( buf, pK->KKval1, FMTX130 );
	}
	len = ( int )strlen ( buf );
	memmove ( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len + 1 );
	i++;

	// Aのうち課税取引にならないもの B
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		::ZeroMemory ( buf, sizeof ( buf ) );
		if ( DispJudge ( 1, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval2, FMTX130 );
		}
		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}

	// 計32
	pK = &( *m_pTaxListData )->NRec.KJval[31];
	::ZeroMemory ( buf, sizeof ( buf ) );
	if ( DispJudge ( 1, KJIGLINE32 ) == 1 ) {
		m_pArith->l_print ( buf, pK->KKval2, FMTX130 );
	}
	len = ( int )strlen ( buf );
	memmove ( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len + 1 );
	i++;

	// 課税取引額 C
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		::ZeroMemory ( buf, sizeof ( buf) );
		if ( DispJudge ( 2, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval3, FMTX130 );
		}
		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}

	// 計32
	pK = &( *m_pTaxListData )->NRec.KJval[31];
	::ZeroMemory ( buf, sizeof ( buf ) );
	if ( DispJudge ( 2, KJIGLINE32 ) == 1 ) {
		m_pArith->l_print ( buf, pK->KKval3, FMTX130 );
	}
	len = ( int ) strlen ( buf );
	memmove ( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len + 1 );
	i++;

	// うち税率6.24%適用分 D
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		::ZeroMemory ( buf, sizeof ( buf ) );
		if ( DispJudge ( 3, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval5, FMTX130 );
		}
		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}

	// 計32
	pK = &( *m_pTaxListData )->NRec.KJval[31];
	::ZeroMemory ( buf, sizeof ( buf ) );
	if( DispJudge ( 3, KJIGLINE32 ) == 1 ) {
		m_pArith->l_print ( buf, pK->KKval5, FMTX130 );
	}
	len = ( int )strlen ( buf );
	memmove ( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len + 1 );
	i++;

	// うち税率7.8%適用分 E
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		::ZeroMemory ( buf, sizeof ( buf ) );
		if ( DispJudge ( 4, j + lin ) == 1 ) {
			m_pArith->l_print ( buf, pK->KKval6, FMTX130 );
		}
		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}	

	// 計32
	pK = &( *m_pTaxListData )->NRec.KJval[31];
	::ZeroMemory ( buf, sizeof ( buf ) );
	if( DispJudge ( 4, KJIGLINE32 ) == 1 ) {
		m_pArith->l_print ( buf, pK->KKval6, FMTX130 );
	}
	len = ( int )strlen ( buf );
	memmove ( pBpMv, buf, len );
	PF[i].PITM[j] = pBpMv;
	pBpMv += ( len + 1 );
	i++;

	// 差引金額33
	// 決算額 A
	pK = &( *m_pTaxListData )->NRec.KJval[32];
	::ZeroMemory ( buf, sizeof ( buf ) );
	if ( DispJudge ( 0, KJIGLINE33 ) == 1 ) {
		m_pArith->l_print ( buf, pK->KKval1, FMTX130 );
	}
	len = ( int )strlen ( buf );
	memmove ( pBpMv, buf, len );
	PF[i].PITM[0] = pBpMv;
	pBpMv += ( len + 1 );
	i++;

	// 34. 3 + 32
	lin = 33;
	pK = &( *m_pTaxListData )->NRec.KJval[lin];
	for ( j = 0; j < 5; j++ ) {
		::ZeroMemory ( buf, sizeof ( buf ) );
		if ( DispJudge ( j, KJIGLINE34 ) == 1 ) {
			switch ( j ) {
			case 0:
				m_pArith->l_print ( buf, pK->KKval1, FMTX130 );
				break;
			case 1:
				m_pArith->l_print ( buf, pK->KKval2, FMTX130 );
				break;
			case 2:
				m_pArith->l_print ( buf, pK->KKval3, FMTX130 );
				break;
			case 3:
				m_pArith->l_print ( buf, pK->KKval5, FMTX130 );
				break;
			case 4:
				m_pArith->l_print ( buf, pK->KKval6, FMTX130 );
				break;
			}
		}
		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len + 1 );
	}
	i++;

	int fontsize = 0;
	char val[5][128] = { 0, 0 };
	CString str;

	for ( int l_cnt = KJIGLINE1; l_cnt <= KJIGLINE34; l_cnt++ ) {
		switch ( l_cnt ) {
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
		case KJIGLINE34:
			for ( int idx = 0; idx < 5; idx++ ) {
				::ZeroMemory ( val[idx], sizeof ( val[idx] ) );
			}
			m_pArith->l_print ( val[0], ( *m_pTaxListData )->NRec.KJval[l_cnt].KKval1, FMTX130 );
			m_pArith->l_print ( val[1], ( *m_pTaxListData )->NRec.KJval[l_cnt].KKval2, FMTX130 );
			m_pArith->l_print ( val[2], ( *m_pTaxListData )->NRec.KJval[l_cnt].KKval3, FMTX130 );
			m_pArith->l_print ( val[3], ( *m_pTaxListData )->NRec.KJval[l_cnt].KKval5, FMTX130 );
			m_pArith->l_print ( val[4], ( *m_pTaxListData )->NRec.KJval[l_cnt].KKval6, FMTX130 );

			for ( int idx = 0; idx < 5; idx++ ) {
				str.Format ( val[idx] );
				str.Trim();
				// 10億円 ( カンマ含む )以上ならフォントサイズを下げる
				//if ( str.GetLength() > 13 ) {
				if ( str.GetLength() > 12 ) {
					fontsize = 1;
				}
			}
			break;
		default:
			break;
		}

		if ( fontsize != 0 ) {
			break;
		}
	}

	if ( fontsize == 1 ) {
		return _xprn ( pno, 5, PF, 0, pDC, 0, 0 );
	}

	return _xprn( pno, 4, PF, 0, pDC, 0 );
}

//=====================================
// 初期ポジションセット
//=====================================
void CShinJigyo31Ex::SetItemPosition()
{
	int index = 0;

	// 前回ポジション
	if ( m_curwnd == IDC_ICSDIAGCTRL1 ) {
		if ( ( index = m_JigyoDiag.GetPosition() ) != -1 ) {
			m_JigyoDiag.SetPosition ( index );
			//m_JigyoDiag.SetFocus();
			return;
		}
	}

	this->SetFocus();
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
		m_JigyoDiag.SetPosition ( JIGVAL01Aex );
	}
	else {
		m_JigyoDiag.SetPosition ( JIGNAME25ex );
	}

	SetRedrawScroll ( 0 );
}

void CShinJigyo31Ex::SetItemPositionEx()
{
	this->SetFocus();
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
		m_JigyoDiag.SetPosition ( JIGVAL01Aex );
	}
	else {
		m_JigyoDiag.SetPosition ( JIGNAME25ex );
	}

	SetRedrawScroll ( 0 );
}

//====================================================
// 次フォーカスの場所検索
//----------------------------------------------------
// 引数
//		Index	:		ダイアグラム内インデックス
//		Move	:		移動方向
//----------------------------------------------------
// 返送値
//		次のフォーカスインデックス
//====================================================
int CShinJigyo31Ex::FocusMoveCheck ( int Index, int Move )
{
	int idx = 0;
	int i = 0, 
		j = 0;
	BOOL end_flg = FALSE;

	int cnt = 0;
	FCSUPDOWN_INFO *pInfo = NULL,
				   *pInfo_LR = NULL;

	if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) {
		cnt = R02_FCS_JIGYOCNTex;
		pInfo = R02_FCS_JigyoEx;
		pInfo_LR = R02_FCS_JigyoEx_LR;
	}
	else {
		cnt = FCS_JIGYOCNTex;
		pInfo = FCS_JigyoEx;
		pInfo_LR = FCS_JigyoEx_LR;
	}

	DIAGRAM_ATTRIBUTE DA;

	if ( Move == 0 ) {	// 下方向
		if ( Index == m_FcsDownEnd ) {
			idx = m_FcsUpEnd;
			end_flg = TRUE;
		}
	}
	else if ( Move == 1 ) {	// 上方向
		if ( Index == m_FcsUpEnd ) {
			idx = m_FcsDownEnd;
			end_flg = TRUE;
		}
	}
	else if ( Move == 2 ) {	// 右方向
		if ( Index == m_FcsRightEnd ) {
			idx = m_FcsLeftEnd;
			end_flg = TRUE;
		}
	}
	else if ( Move == 3 ) {	// 左方向
		if ( Index == m_FcsLeftEnd ) {
			idx = m_FcsRightEnd;
			end_flg = TRUE;
		}
	}

	if ( end_flg == TRUE ) {
		return idx;
	}

	// 現在ポジション検索
	if ( ( Move == 0 ) || ( Move == 1 ) ) {
		//for ( i = 0; i < FCS_JIGYOCNT; i++ ) {
		//	if ( Index == FCS_Jigyo[i].IDNo ) {
		//		break;
		//	}
		//}
		//idx = FCS_Jigyo[i].IDNo;
		for ( i = 0; i < cnt; i++ ) {
			if ( Index == ( pInfo + i )->IDNo ) {
				break;
			}
		}
		idx = ( pInfo + i )->IDNo;
	}
	else if ( ( Move == 2 ) || ( Move == 3 ) ) {
		//for ( i = 0; i < FCS_JIGYOCNT; i++ ) {
		//	if ( Index == FCS_Jigyo_LR[i].IDNo ) {
		//		break;
		//	}
		//}
		//idx = FCS_Jigyo_LR[i].IDNo;
		for ( i = 0; i < cnt; i++ ) {
			if ( Index == ( pInfo_LR + i )->IDNo ) {
				break;
			}
			idx = ( pInfo_LR + i )->IDNo;
		}
	}

	// 次ポジション検索
	if ( Move == 0 ) {	//下方向
		//for ( j = i + 1; j < FCS_JIGYOCNT; j++ ) {
		//	m_JigyoDiag.GetAttribute ( FCS_Jigyo[j].IDNo, ( LPUNKNOWN )&DA );
		//	if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
		//		idx = FCS_Jigyo[j].IDNo;
		//		break;
		//	}
		//}
		for ( j = i + 1; j < cnt; j++ ) {
			m_JigyoDiag.GetAttribute ( ( pInfo + j )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo + j )->IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {	//上方向
		for ( j = i - 1; j > -1; j-- ) {
			//m_JigyoDiag.GetAttribute ( FCS_Jigyo[j].IDNo, ( LPUNKNOWN )&DA );
			//if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			//	idx = FCS_Jigyo[j].IDNo;
			//	break;
			//}
			m_JigyoDiag.GetAttribute ( ( pInfo + j )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo + j )->IDNo;
				break;
			}
		}
	}
	else if ( Move == 2 ) {	//右方向
		//for ( j = i + 1; j < FCS_JIGYOCNT; j++ ) {
		//	m_JigyoDiag.GetAttribute ( FCS_Jigyo_LR[j].IDNo, ( LPUNKNOWN )&DA );
		//	if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
		//		idx = FCS_Jigyo_LR[j].IDNo;
		//		break;
		//	}
		//}
		for ( j = i + 1; j < cnt; j++ ) {
			m_JigyoDiag.GetAttribute ( ( pInfo_LR + j )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo_LR + j )->IDNo;
				break;
			}
		}
	}
	else if ( Move == 3 ) {	//左方向
		for ( j = i - 1; j > -1; j-- ) {
			//m_JigyoDiag.GetAttribute ( FCS_Jigyo_LR[j].IDNo, ( LPUNKNOWN )&DA );
			//if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			//	idx = FCS_Jigyo_LR[j].IDNo;
			//	break;
			//}
			m_JigyoDiag.GetAttribute ( ( pInfo_LR + j )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo_LR + j )->IDNo;
				break;
			}
		}
	}

	return idx;
}


BEGIN_EVENTSINK_MAP(CShinJigyo31Ex, CSyzBaseDlg)
	ON_EVENT(CShinJigyo31Ex, IDC_ICSDIAGCTRL1, 1, CShinJigyo31Ex::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinJigyo31Ex, IDC_ICSDIAGCTRL1, 2, CShinJigyo31Ex::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinJigyo31Ex, IDC_ICSDIAGCTRL1, 3, CShinJigyo31Ex::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
// 22-0167,22-0361 add -->
	ON_EVENT(CShinJigyo31Ex, IDC_ICSDIAGCTRL1, 9, CShinJigyo31Ex::CheckButtonIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
// 22-0167,22-0361 add <--
END_EVENTSINK_MAP()

void CShinJigyo31Ex::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_JigyoDiag.OffControl ( FALSE );

	m_Idx = index;
	if ( ( m_Idx >= JIGMINex ) && ( m_Idx <= JIGMAXex ) ) {
		m_Lin = ( index - JIGMINex ) / JIGWIDECNTex;
		m_Clm = ( index - JIGMINex ) % JIGWIDECNTex;
	}
	else if ( m_Idx == JIGNAME25ex ) {
		m_Lin = KJIGLINE25 + 1; //変換等対価の行分＋１する
		m_Clm = -1;
	}
	else if ( m_Idx == JIGNAME26ex ) {
		m_Lin = KJIGLINE26 + 1;	//変換等対価の行分＋１する
		m_Clm = -1;
	}
	else if ( m_Idx == JIGNAME27ex ) {
		m_Lin = KJIGLINE27 + 1;	//変換等対価の行分＋１する
		m_Clm = -1;
	}
	else if ( m_Idx == JIGNAME28ex ) {
		m_Lin = KJIGLINE28 + 1;	//変換等対価の行分＋１する
		m_Clm = -1;
	}
	else if ( m_Idx == JIGNAME29ex ) {
		m_Lin = KJIGLINE29 + 1;	//変換等対価の行分＋１する
		m_Clm = -1;
	}
	else if ( m_Idx == JIGNAME30ex ) {
		m_Lin = KJIGLINE30 + 1;	//変換等対価の行分＋１する
		m_Clm = -1;
	}


	m_curwnd = IDC_ICSDIAGCTRL1;
}

void CShinJigyo31Ex::EditOFFIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA	data;
	diag_getdata ( IDC_ICSDIAGCTRL1, index, &data, CIcsdiagctrl );
	int	st = GetDiagData ( index, data.data_check, &data, 0 );
	diag_deledit ( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
	if ( st < 0 ) {
		diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );
	}
}

void CShinJigyo31Ex::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if ( nChar == 0 ) {
		return;
	}

	DIAGRAM_DATA *idata = NULL;
	DIAGRAM_DATA xdata;

	UINT Flg = 0;

	idata = ( DIAGRAM_DATA* )data;
	m_Util.DiagOcxIniz ( &xdata );
	m_Util.MoveDiagData ( &xdata, idata );

	short wTerm = 0;
	char VK_FLG = 0x00;

	VK_FLG = ( char )::GetKeyState ( VK_SHIFT );
	if ( nChar == VK_TAB ) {
		if ( VK_FLG & 0x80 ) {
			wTerm = VK_TAB;
		}
		else {
			wTerm = VK_RETURN;
		}
	}
	else {
		wTerm = nChar;
	}

	int pos = 0;
	if ( wTerm == VK_UP ) {
		pos = FocusMoveCheck ( index, 1 );
		m_JigyoDiag.SetPosition ( pos );
	}

	if ( wTerm == VK_DOWN ) {
		pos = FocusMoveCheck ( index, 0 );
		m_JigyoDiag.SetPosition ( pos );
	}

	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) ) {
		pos = FocusMoveCheck ( index, 2 );
		m_JigyoDiag.SetPosition ( pos );
	}
	else if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) ) {
		pos = FocusMoveCheck ( index, 3 );
		m_JigyoDiag.SetPosition ( pos );
	}

	if ( wTerm == VK_DELETE ) {
		GetDiagData( index, length, &xdata, -1 );
	}

	SetRedrawScrollToIdx ( m_JigyoDiag.GetPosition(), IDC_ICSDIAGCTRL1 );

}

BOOL CShinJigyo31Ex::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ) {
			if ( m_pParent ) {
				m_pParent->PostMessageA ( WM_KEYDOWN, VK_END );
			}

			return TRUE;
		}

		if ( pMsg->wParam == VK_F2 ) {
			DIAGRAM_DATA dd;
			if ( m_curwnd != -1 ) {
				short result = ( ( CIcsdiagctrl* )GetDlgItem ( m_curwnd ) )->GetPosition();
				diag_getdata ( m_curwnd, result, &dd, CIcsdiagctrl );
				if ( m_curwnd == IDC_ICSDIAGCTRL1 ) {
					TerminationIcsdiagctrl1 ( result, ( short )pMsg->wParam, 0, ( LPUNKNOWN )&dd );
				}

				return TRUE;
			}

		}
	}
	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

// 22-0167,22-0361 add -->
void CShinJigyo31Ex::CheckButtonIcsdiagctrl1(short index, LPUNKNOWN data)
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
		//short	pos = m_JigyoDiag.GetPosition();
		m_JigyoDiag.DeleteInput();
		diag_getdata(IDC_ICSDIAGCTRL1,index,&xdata,CIcsdiagctrl);
		if(!idata->data_check)	xdata.data_check = 1;
		else					xdata.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1,index,&xdata,CIcsdiagctrl);
		//m_JigyoDiag.SetPosition(pos);
		SetItemPositionEx();
		return;
	}

	// 現在は画面に自動計算チェックを１つしか設けていないので
	// 先頭レコードにのみ保存でもよいが
	// 今後のこと（ブロックごとに自動計算サインが必要という要望がきたとき）と
	// Shz_LinCalc関数内で[0]番目が参照できないことから
	// 全行に同じ状態の自動計算サインを書き込むことにする
	//for(ii=0; ii<50; ii++)	{
	//	(*m_pTaxListData)->NRec.KJval[ii].KKauto &= 0x2f;	// フラグの上位を一旦クリア
	//}
	(*m_pSnHeadData)->Sn_GenericSgn &= 0x7ff;
	// チェックON
	if(idata->data_check)	{
		//// 自身以外のチェックをOFFにする
		//if(index == JISELChk2ex)	{
		//	diag_getdata(IDC_ICSDIAGCTRL1,JISELChk3ex,&xdata,CIcsdiagctrl);
		//	xdata.data_check = 0;
		//	diag_setdata(IDC_ICSDIAGCTRL1,JISELChk3ex,&xdata,CIcsdiagctrl);
		//	for(ii=0; ii<50; ii++)	{
		//		(*m_pTaxListData)->NRec.KJval[ii].KKauto |= 0x40;
		//	}
		//}
		//else if(index == JISELChk3ex)	{
		//	diag_getdata(IDC_ICSDIAGCTRL1,JISELChk2ex,&xdata,CIcsdiagctrl);
		//	xdata.data_check = 0;
		//	diag_setdata(IDC_ICSDIAGCTRL1,JISELChk2ex,&xdata,CIcsdiagctrl);
		//	for(ii=0; ii<50; ii++)	{
		//		(*m_pTaxListData)->NRec.KJval[ii].KKauto |= 0x80;
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
	m_JigyoDiag.OffControl(TRUE);
	for(int idx=0; idx<JIGLINCNTex; idx++)	{
		DispJigyoDiagData(idx);
	}
	SetItemPositionEx();
}

// ----------------------------------------------------------------------------------
// 自動計算チェックON/OFFによる、6.24%・7.8%の入力可／不可を切り替える
// ----------------------------------------------------------------------------------
// 【引　数】	なし
// 【返送値】	なし
// ----------------------------------------------------------------------------------
void CShinJigyo31Ex::ColumnControl( void )
{
	SH_ITEMINDEX	*pIndex=NULL;
	int				cnt=0;
	int				Flg=0;
	KKVAL*			pK = &( *m_pTaxListData )->NRec.KJval[0];

	if((*m_pSnHeadData )->m_DispTabSgn & 0x1000)	pIndex = R02_JigyoEx;
	else											pIndex = JigyoEx;

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
int CShinJigyo31Ex::GetColumnKind( int pInd )
{
	int		rv=0;

	switch(pInd)	{
		case	JIGVAL01Dex:
		case	JIGVAL012Dex:
		case	JIGVAL03Dex:
		case	JIGVAL08Dex:
		case	JIGVAL09Dex:
		case	JIGVAL10Dex:
		case	JIGVAL11Dex:
		case	JIGVAL12Dex:
		case	JIGVAL13Dex:
		case	JIGVAL14Dex:
		case	JIGVAL16Dex:
		case	JIGVAL17Dex:
		case	JIGVAL19Dex:
		case	JIGVAL20Dex:
		case	JIGVAL21Dex:
		case	JIGVAL23Dex:
		case	JIGVAL25Dex:
		case	JIGVAL26Dex:
		case	JIGVAL27Dex:
		case	JIGVAL28Dex:
		case	JIGVAL29Dex:
		case	JIGVAL30Dex:
		case	JIGVAL31Dex:
			rv=1;
			break;
		case	JIGVAL01Eex:
		case	JIGVAL012Eex:
		case	JIGVAL03Eex:
		case	JIGVAL08Eex:
		case	JIGVAL09Eex:
		case	JIGVAL10Eex:
		case	JIGVAL11Eex:
		case	JIGVAL12Eex:
		case	JIGVAL13Eex:
		case	JIGVAL14Eex:
		case	JIGVAL16Eex:
		case	JIGVAL17Eex:
		case	JIGVAL19Eex:
		case	JIGVAL20Eex:
		case	JIGVAL21Eex:
		case	JIGVAL23Eex:
		case	JIGVAL25Eex:
		case	JIGVAL26Eex:
		case	JIGVAL27Eex:
		case	JIGVAL28Eex:
		case	JIGVAL29Eex:
		case	JIGVAL30Eex:
		case	JIGVAL31Eex:
			rv=2;
			break;
	}
	return(rv);
}
// 22-0167,22-0361 add <--

// 修正No.168690 add -->
void CShinJigyo31Ex::DiagReSetPosition(void)
{
	this->SetFocus();
}
// 修正No.168690 add <--
