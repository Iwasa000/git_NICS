#include "stdafx.h"
#include "ShinJigyo31Ex2.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CShinJigyo31Ex2, CSyzBaseDlg)

CShinJigyo31Ex2::CShinJigyo31Ex2(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(CShinJigyo31Ex2::IDD, pParent)
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
	setFCSArray();
}

CShinJigyo31Ex2::~CShinJigyo31Ex2()
{
}

void CShinJigyo31Ex2::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_JigyoDiag);
}


BEGIN_MESSAGE_MAP(CShinJigyo31Ex2, CSyzBaseDlg)
END_MESSAGE_MAP()

// CShinJigyo31Ex2 メッセージ ハンドラー


BOOL CShinJigyo31Ex2::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_JigyoDiag;
    //(項目名:3+32)
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
int CShinJigyo31Ex2::Init()
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
void CShinJigyo31Ex2::InitDiagAttr()
{
	DIAGRAM_ATTRIBUTE data = {};
	long dg1[] = { jigName25, jigName26, jigName27, jigName28, jigName29, jigName30, -1 };
	

	for ( int idx = 0; ; idx++ ) {
		if ( dg1[idx] == -1 ) {
			break;
		}

		// ank対応
		m_JigyoDiag.GetAttribute ( ( short )dg1[idx], ( LPUNKNOWN )&data );
		data.attr_editattr |= DIAG_EATTR_ZENHAN;
		m_JigyoDiag.SetAttribute ( ( short )dg1[idx], ( LPUNKNOWN )&data, TRUE );
	}

	int KE = jigVal01F;
	
	const SH_ITEMINDEX *pIndex = NULL;
	//pIndex = (*m_pSnHeadData)->m_DispTabSgn & 0x1000 ? R02_JigyoEx : JigyoEx; //令和2年4月1日改正様式との切り替え処理
	pIndex = idxAttr;

	int cnt = 0;
	while ( 1 ) {
		
		if ( ( pIndex + cnt )->Index == 0 ) {
			break;
		}

		int Flg = 0;

		// 確定時は全入力不可
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = ( pIndex + cnt )->AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				//Flg = ( CheckSVmzsw ( ( pIndex + cnt )->Index ) == 0 ) ? ( pIndex + cnt )->UnConnect : 0; 
				Flg = ( pIndex + cnt )->UnConnect;
			}
			else {
				Flg = ( pIndex + cnt )->Connect;
			}
		}
		ATRIB_MOD ( IDC_ICSDIAGCTRL1, ( pIndex + cnt )->Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, ( pIndex + cnt )->Connect );
		cnt++;
	}

	// 斜線
	//InitDiagonalLines();

	SetFocusRange();


	m_JigyoDiag.EnableDelete ( 0 );
}


//=====================================
// ポジションの末端設定
//=====================================
void CShinJigyo31Ex2::SetFocusRange()
{
	int cnt = 0;
	FCSUPDOWN_INFO *pInfo    = NULL,
		           *pInfo_LR = NULL; 
	/*
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
	*/
	cnt      = FCSitemMax;
	pInfo    = FCS_UD;
	pInfo_LR = FCS_LR;

	DIAGRAM_ATTRIBUTE DA = {};

	m_FcsUpEnd = -1;
	m_FcsLeftEnd = -1;
	m_FcsDownEnd = -1;
	m_FcsRightEnd = -1;

	// 上
	
	for ( int idx = 0; idx < cnt; idx++ ) {
		m_JigyoDiag.GetAttribute ( ( pInfo + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsUpEnd = ( pInfo + idx )->IDNo;
			break;
		}
	}

	// 右
	for ( int idx = 0; idx < cnt; idx++ ) {
		m_JigyoDiag.GetAttribute ( ( pInfo_LR + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsLeftEnd = ( pInfo_LR + idx )->IDNo;
			break;
		}
	}

	// 下
	for ( int idx = cnt - 1; idx > -1; idx-- ) {
		m_JigyoDiag.GetAttribute ( ( pInfo + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsDownEnd = ( pInfo + idx )->IDNo;
			break;
		}
	}

	// 左
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
void CShinJigyo31Ex2::DispList(int mode)
{
	
	//課税仕入高が4列となり、自動計算が不可能となったため削除(2023.10インボイス改正)	
	//ColumnControl(); // 入力可／不可を切り替える(22-0167,22-0361 add)
	
	m_JigyoDiag.OffControl ( TRUE );
	( *m_Scalc )->Shz_31AllCalc ( 1 );

	short idx = 0;
	for ( idx = 0; idx < rowSize; idx++ ) {
		DispJigyoDiagData ( idx );
	}

	DIAGRAM_DATA data = {};

	if ( !mode ) {
		m_Util.DiagOcxIniz ( &data );
		diag_clear ( IDC_ICSDIAGCTRL1, idxYear, TRUE, CIcsdiagctrl );
		data.data_disp.Format (_T( "（令和%2x年分）" ), ( *m_pTaxListData )->YEAR );
		diag_setdata ( IDC_ICSDIAGCTRL1, idxYear, &data, CIcsdiagctrl );

		PostMessage ( WM_VSCROLL, SB_TOP, 0 );
		m_Idx = idxEditMin;
	}

	m_Util.DiagOcxIniz ( &data );
	diag_clear ( IDC_ICSDIAGCTRL1, idxZkbn, TRUE, CIcsdiagctrl );
	data.data_disp = _T( "経理方式：" );
	if ( ( *m_pTaxListData )->Head.KHstax & 0x01 ) {
		data.data_disp += _T( "税抜き" );
	}
	else {
		data.data_disp += _T( "税込み" );
	}

	diag_setdata ( IDC_ICSDIAGCTRL1, idxZkbn, &data, CIcsdiagctrl );
	//diag_setposition ( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );

	int edit[] = { jigName25, jigName26, jigName27, jigName28, jigName29, jigName30 };
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

	// 2023/07/11 自動計算機能を削除
    // 2023/07/11 add -->
	diag_modify(IDC_ICSDIAGCTRL1, autoCalcChk2, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl);
	diag_modify(IDC_ICSDIAGCTRL1, autoCalcChk3, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl);
	// 2023/07/11 add <--
	// 2023/07/11 del -->
	/* 
	// 22-0167,22-0361 add -->
	KKVAL*	pK = &( *m_pTaxListData )->NRec.KJval[0];
	for(idx=autoCalcChk2; idx<=autoCalcChk3; idx++)  {
		diag_getdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
		data.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
	}
	idx=0;
	if((*m_pSnHeadData)->Sn_GenericSgn & 0x800)		idx=autoCalcChk3;
	//if(pK->KKauto & 0x40)		idx=autoCalcChk2;
	//else if(pK->KKauto & 0x80)	idx=autoCalcChk3;
	if(idx != 0)  {
		diag_getdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
		data.data_check = 1;
		diag_setdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
	}
	//// 確定時または連動時は選択不可
	//if(((*m_pSnHeadData)->Sn_Sign4 & 0x01) || (!((*m_pSnHeadData)->Sn_Sign4 & 0x80)))	{
	//	for(idx=autoCalcChk2; idx<=autoCalcChk3; idx++)	{
	//		ATRIB_MOD(IDC_ICSDIAGCTRL1,idx,0,(*m_pSnHeadData)->Sn_Sign4,0);
	//	}
	//}
	// 6.24%適用分は非表示にする(今後要望があったときのために残しておく)
	diag_modify(IDC_ICSDIAGCTRL1, autoCalcChk2, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl);
	// 連動時は7.8%適用分も非表示にする
	if(!((*m_pSnHeadData)->Sn_Sign4 & 0x80))  {
		diag_modify(IDC_ICSDIAGCTRL1, autoCalcChk3, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl);
	}
	// 確定時は7.8%適用分を選択不可にする
	else if((*m_pSnHeadData)->Sn_Sign4 & 0x01) {
		//ATRIB_MOD(IDC_ICSDIAGCTRL1,autoCalcChk3,0,(*m_pSnHeadData)->Sn_Sign4,0);
		DIAGRAM_ATTRIBUTE	da;
		diag_modify(IDC_ICSDIAGCTRL1,autoCalcChk3,DIAG_MDFY_READONLY,CIcsdiagctrl);
		diag_getattr(IDC_ICSDIAGCTRL1,autoCalcChk3,&da, CIcsdiagctrl);
		da.attr_bcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAGCTRL1,autoCalcChk3,&da,FALSE,CIcsdiagctrl);
	}
	else {
		//ATRIB_MOD(IDC_ICSDIAGCTRL1,autoCalcChk3,1,(*m_pSnHeadData)->Sn_Sign4,0);
		DIAGRAM_ATTRIBUTE	da, da2;
		diag_getattr(IDC_ICSDIAGCTRL1,0,&da2,CIcsdiagctrl);
		diag_getattr(IDC_ICSDIAGCTRL1,autoCalcChk3,&da,CIcsdiagctrl);
		da.attr_bcolor = da2.attr_bcolor;
		diag_setattr(IDC_ICSDIAGCTRL1,autoCalcChk3,&da, FALSE,CIcsdiagctrl);
		diag_modify(IDC_ICSDIAGCTRL1,autoCalcChk3,DIAG_MDFY_EDIT,CIcsdiagctrl);
	}
	// 22-0167,22-0361 add <--
	*/
	// 2023/07/11 del <--
}


//=====================================
// ダイアグラムデータ表示
//-------------------------------------
// 引数
//		dlin	: ダイアグラム上の行数
//=====================================
void CShinJigyo31Ex2::DispJigyoDiagData(long dlin)
{
	short idx = 0,
	      i = 0;
	long  lin = 0;

	//データ保存ポジションセット
	lin = dataPos[dlin];

	KKVAL*	pK = &( *m_pTaxListData )->NRec.KJval[lin];

	char val[6] = { 0 };
	DIAGRAM_DATA data = {};
	int st = 0;

	//idx = ( short )( dlin * JIGWIDECNTex ) + JIGMINex;
// インボイス_23/07/24_higuchi del -->
	//idx = ( short )idxMap[dlin][clmA]; //2023/07/11
// インボイス_23/07/24_higuchi del <--

	for ( i = 0; i < clmSize; i++ ) {
// インボイス_23/07/24_higuchi add -->
		idx = ( short )idxMap[dlin][i];
		if(idx < 0)	continue;
// インボイス_23/07/24_higuchi add <--
		st = 0;
		::ZeroMemory ( val, sizeof ( val ) );
// インボイス_23/07/24_higuchi cor -->
		//diag_clear ( IDC_ICSDIAGCTRL1, idx + i, TRUE, CIcsdiagctrl );
// -----------------------------------
		diag_clear ( IDC_ICSDIAGCTRL1, idx, TRUE, CIcsdiagctrl );
// インボイス_23/07/24_higuchi cor <--
		m_Util.DiagOcxIniz ( &data );

		switch ( i ) {
		case clmA:		// 決算額 A
			memcpy ( val, pK->KKval1, 6 );
			break;
		case clmB:		// Aのうち課税取引にならないもの B
			memcpy ( val, pK->KKval2, 6 );
			break;
		case clmC:		// 課税取引金額 C ( A - B )
			memcpy ( val, pK->KKval3, 6 );
			break;
		case clmD:		// うち税率6.24%適用分 D
			memcpy ( val, pK->KKval5, 6 );
			break;
		case clmF:		// うち税率7.8%適用分 F
			memcpy ( val, pK->KKval6, 6 );
			break;
		case clmE: 
// インボイス_23/07/21_higuchi add -->
			memcpy ( val, pK->KKval7, 6 );
			break;
// インボイス_23/07/21_higuchi add <--
		case clmG:
// インボイス_23/07/21_higuchi add -->
			memcpy ( val, pK->KKval8, 6 );
			break;
// インボイス_23/07/21_higuchi add <--
		default:
			break;
		}
		if ( DispJudge ( i, lin ) == 1 ) {
			memcpy ( data.data_val, val, 6 );
// インボイス_23/07/24_higuchi cor -->
			//diag_setdata ( IDC_ICSDIAGCTRL1, idx + i, &data, CIcsdiagctrl );
// -----------------------------------
			diag_setdata ( IDC_ICSDIAGCTRL1, idx, &data, CIcsdiagctrl );
// インボイス_23/07/24_higuchi cor <--
		}
	}

	// 項目名
	char buf[128] = { 0 };
	if ( ( dlin >= jigDiagLine25 ) && ( dlin <= jigDiagLine30 ) ) {
		//lin = KJigyoDataPos[dlin + 1]; 
		lin = dataPos[dlin];
		pK = &( *m_pTaxListData )->NRec.KJval[lin];

		if ( !( pK->KKsign & 0x80 ) ){
			m_Util.DiagOcxIniz ( &data );
			::ZeroMemory ( buf, sizeof ( buf ) );

			CString tmp = {};
			int prf_no = 0;

			switch ( dlin ) {
			case jigDiagLine25:
				idx = jigName25;
				prf_no = 25;
				break;
			case jigDiagLine26:
				idx = jigName26;
				prf_no = 26;
				break;
			case jigDiagLine27:
				idx = jigName27;
				prf_no = 27;
				break;
			case jigDiagLine28:
				idx = jigName28;
				prf_no = 28;
				break;
			case jigDiagLine29:
				idx = jigName29;
				prf_no = 29;
				break;
			case jigDiagLine30:
				idx = jigName30;
				prf_no = 30;
				break;
			}
			if ( ( *m_pTaxListData)->IsLinkKjprfname() && ( prf_no != 0 ) ) {
// インボイス_23/07/21_higuchi cor -->
				//if ( ( m_pArith->l_test ( pK->KKval1 ) != 0 ) || ( m_pArith->l_test ( pK->KKval2 ) != 0 ) || ( m_pArith->l_test ( pK->KKval3 ) != 0 ) || ( m_pArith->l_test ( pK->KKval5 ) != 0 ) || ( m_pArith->l_test ( pK->KKval6 ) != 0 ) ) {
// -----------------------------------
				if ( ( m_pArith->l_test ( pK->KKval1 ) != 0 ) || ( m_pArith->l_test ( pK->KKval2 ) != 0 ) || 
					 ( m_pArith->l_test ( pK->KKval3 ) != 0 ) || ( m_pArith->l_test ( pK->KKval5 ) != 0 ) || 
					 ( m_pArith->l_test ( pK->KKval6 ) != 0 ) || 
					 ( m_pArith->l_test ( pK->KKval7 ) != 0 ) || ( m_pArith->l_test ( pK->KKval8 ) != 0 ) ) {
// インボイス_23/07/21_higuchi cor <--
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
//		dataId	:	表示対象とするデータ項目の保存位置
// 戻り値
//      st      :   1=表示可能、0=表示不可
//=====================================
int CShinJigyo31Ex2::DispJudge ( int clm, int dataId )
{
	int st = 0;
	int j = 0;

	switch ( clm  ){
	case clmA:		// 決算額　A
		switch (dataId) {
		case jigDataLine33:	//差引税額 ( 下 )
		case jigDataLine7:	//差引税額 ( 上 )
			if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[jigDataLine1].KKval1 ) ) {
				st = 1;
			}
		case jigDataLine6:		//売上原価 差引原価
			if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[jigDataLine5].KKval1 ) ) {
				st = 1;
			}
		case jigDataLine4:		//売上原価 小計
			if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[jigDataLine2].KKval1 ) || m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[jigDataLine3].KKval1 ) ) {
				st = 1;
			}
			if (dataId != jigDataLine33) {
				break;
			}
		case jigDataLine32:	//経費 計
			for ( j = jigDataLine8; j < jigDataLine32; j++ ) {
				if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[j].KKval1 ) ) {
					st = 1;
					break;
				}
			}
			break;
		default:
			if  ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[dataId].KKval1 ) ) {
				st = 1;
			}
			break;
		}
		break;
	case clmB:	// Aのうち課税取引にならないもの　B
		switch ( dataId ) {
		case jigDataLine2:		 //期首商品棚卸高
		case jigDataLine4:		 //小計
		case jigDataLine5:		 //期末商品棚卸高
		case jigDataLine6:		 //差引原価
		case jigDataLine7:		 //差引金額
		case jigDataLine10:	 //水道光熱費
		case jigDataLine16:	 //修繕費
		case jigDataLine17:	 //消耗品費
		case jigDataLine33:	 //差引金額
							 //入力不可の為処理しない
			break;
		case jigDataLine32:	//経費　計
			for ( j = jigDataLine8; j < jigDataLine32; j++ ) {
				if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[j].KKval2 ) ) {
					st = 1; 
					break;
				}
			}
			break;
		default:
			if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[dataId].KKval2 ) ) {
				st = 1;
			}
			break;
		}
		break;
	case clmC:		// 課税取引金額 C ( A - B )
		switch ( dataId ) {
		case jigDataLine2:		 //期首商品棚卸高
		case jigDataLine4:		 //小計
		case jigDataLine5:		 //期末商品棚卸高
		case jigDataLine6:		 //差引原価
		case jigDataLine7:		 //差引金額
		case jigDataLine15:	 //損害保険料
		case jigDataLine18:	 //減価償却費
		case jigDataLine22:	 //利子割引料
		case jigDataLine24:	 //貸倒金
		case jigDataLine33:	 //差引金額
							 //入力不可の為処理しない
			break;
		case jigDataLine32:	//経費 計
			for ( j = jigDataLine8; j < jigDataLine32; j++ ) {
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
			if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[dataId].KKval1 ) || m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[dataId].KKval2 ) ) {
				st = 1;
			}
			break;
		}
		break;
	case clmD:		// うち税率6.24%適用分 D
		switch (dataId) {
		case jigDataLine2:		 //期首商品棚卸高
		case jigDataLine4:		 //小計
		case jigDataLine5:		 //期末商品棚卸高
		case jigDataLine6:		 //差引原価
		case jigDataLine7:		 //差引金額
		case jigDataLine8:		 //租税公課
		case jigDataLine9:		 //荷造運賃
		case jigDataLine10:	 //水道光熱費
		case jigDataLine11:	 //旅費交通費
		case jigDataLine15:	 //損害保険料
		case jigDataLine18:	 //減価償却費
		case jigDataLine16:	 //修繕費
		case jigDataLine20:	 //給料賃金
		case jigDataLine21:	 //外注工賃
		case jigDataLine22:	 //利子割引料
		case jigDataLine23:	 //地代家賃
		case jigDataLine24:	 //貸倒金
		case jigDataLine33:	 //差引金額
							 //入力不可の為処理しない
		break;
		case jigDataLine32:	 //経費 計
			for (j = jigDataLine8; j < jigDataLine32; j++) {
				if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[j].KKval5 ) ) {
					st = 1;
					break;
				}
			}
			break;
		default:
			if ( m_pArith->l_test ( ( *m_pTaxListData)->NRec.KJval[dataId].KKval5 ) ) {
				st = 1;
			}
			break;
		}
		break;

// インボイス_23/07/21_higuchi add -->
	case clmE:		// うち税率6.24%適用分 E
		switch (dataId) {
		case jigDataLine2:		 //期首商品棚卸高
		case jigDataLine4:		 //小計
		case jigDataLine5:		 //期末商品棚卸高
		case jigDataLine6:		 //差引原価
		case jigDataLine7:		 //差引金額
		case jigDataLine8:		 //租税公課
		case jigDataLine9:		 //荷造運賃
		case jigDataLine10:	 //水道光熱費
		case jigDataLine11:	 //旅費交通費
		case jigDataLine15:	 //損害保険料
		case jigDataLine18:	 //減価償却費
		case jigDataLine16:	 //修繕費
		case jigDataLine20:	 //給料賃金
		case jigDataLine21:	 //外注工賃
		case jigDataLine22:	 //利子割引料
		case jigDataLine23:	 //地代家賃
		case jigDataLine24:	 //貸倒金
		case jigDataLine33:	 //差引金額
							 //入力不可の為処理しない
		break;
		case jigDataLine32:	 //経費 計
			for (j = jigDataLine8; j < jigDataLine32; j++) {
				if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[j].KKval7 ) ) {
					st = 1;
					break;
				}
			}
			break;
		default:
			if ( m_pArith->l_test ( ( *m_pTaxListData)->NRec.KJval[dataId].KKval7 ) ) {
				st = 1;
			}
			break;
		}
		break;
// インボイス_23/07/21_higuchi add <--

	case clmF:		// うち税率7.8%適用分 F
		switch ( dataId ) {
		case jigDataLine2:		 //期首商品棚卸高
		case jigDataLine4:		 //小計
		case jigDataLine5:		 //期末商品棚卸高
		case jigDataLine6:		 //差引原価
		case jigDataLine7:		 //差引金額
		case jigDataLine15:	 //損害保険料
		case jigDataLine18:	 //減価償却費
		case jigDataLine22:	 //利子割引料
		case jigDataLine24:	 //貸倒金
		case jigDataLine33:	 //差引金額
							 //入力不可の為処理しない
			break;
		case jigDataLine32:	 //経費 計
			for ( j = jigDataLine8; j < jigDataLine32; j++ ) {
				if (m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[j].KKval6 ) ){
					st = 1;
					break;
				}
			}
			break;
		default:
			if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[dataId].KKval6 ) ){
				st = 1;
			}
			break;
		}
		break;

// インボイス_23/07/21_higuchi add -->
	case clmG:		// うち税率7.8%適用分 G
		switch ( dataId ) {
		case jigDataLine2:		 //期首商品棚卸高
		case jigDataLine4:		 //小計
		case jigDataLine5:		 //期末商品棚卸高
		case jigDataLine6:		 //差引原価
		case jigDataLine7:		 //差引金額
		case jigDataLine15:	 //損害保険料
		case jigDataLine18:	 //減価償却費
		case jigDataLine22:	 //利子割引料
		case jigDataLine24:	 //貸倒金
		case jigDataLine33:	 //差引金額
							 //入力不可の為処理しない
			break;
		case jigDataLine32:	 //経費 計
			for ( j = jigDataLine8; j < jigDataLine32; j++ ) {
				if (m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[j].KKval8 ) ){
					st = 1;
					break;
				}
			}
			break;
		default:
			if ( m_pArith->l_test ( ( *m_pTaxListData )->NRec.KJval[dataId].KKval8 ) ){
				st = 1;
			}
			break;
		}
		break;
// インボイス_23/07/21_higuchi add <--
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
int CShinJigyo31Ex2::GetDiagData(long index, short leng, DIAGRAM_DATA *pData, int mode)
{
	
	if ( index < 0 || index == autoCalcChk2 || index == autoCalcChk3) { // 22-0167,22-0361
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

	lin = dataPos[m_Lin];

	KKVAL*	pK = &( *m_pTaxListData )->NRec.KJval[lin];

	if ( !mode ) {
		switch ( m_Clm ) {
		case clmA:		// 決算額	A
			if ( leng ){
				memcpy ( pK->KKval1, pData->data_val, 6 );
			}
			break;
		case clmB:		// Aのうち課税取引にならないもの B
			if ( leng ) {
				memcpy ( pK->KKval2, pData->data_val, 6 );
			}
			break;
		case clmC:		// 課税取引金額 C ( A - B )
			if ( leng ) {
				memcpy ( pK->KKval3, pData->data_val, 6 );
			}
			break;
		case clmD:		// うち税率6.24%適用分 D
			if ( leng ) {
				memcpy ( pK->KKval5, pData->data_val, 6 );
			}
			break;
		case clmF:		// うち税率7.8%適用分 F
			if ( leng ) {
				memcpy ( pK->KKval6, pData->data_val, 6 );
			}
			break;
		case clmE:
// インボイス_23/07/21_higuchi add -->
			if ( leng ) {
				memcpy ( pK->KKval7, pData->data_val, 6 );
			}
			break;
// インボイス_23/07/21_higuchi add <--
		case clmG:
// インボイス_23/07/21_higuchi add -->
			if ( leng ) {
				memcpy ( pK->KKval8, pData->data_val, 6 );
			}
// インボイス_23/07/21_higuchi add <--
			break;
		default:	// 項目名
			memcpy ( pK->KKnam, dbuf, sizeof ( pK->KKnam ) );
			break;
		}
	}
	else {
		switch ( m_Clm ) {
		case clmA:		// 決算額 A
			::ZeroMemory ( pK->KKval1, 6 );
			break;
		case clmB:		// Aのうち課税取引にならないもの B
			::ZeroMemory ( pK->KKval2, 6 );
			break;
		case clmC:		// 課税取引金額 C（ A － B ）
			::ZeroMemory ( pK->KKval3, 6 ); 
			break;
		case clmD:		// うち税率6.24%適用分 D
			::ZeroMemory ( pK->KKval5, 6 );  
			break;
		case clmF:		// うち税率7.8%適用分 F
			::ZeroMemory ( pK->KKval6, 6 );  
			break;
		case clmE:
// インボイス_23/07/21_higuchi add -->
			::ZeroMemory ( pK->KKval7, 6 );  
			break;
// インボイス_23/07/21_higuchi add <--
		case clmG:
// インボイス_23/07/21_higuchi add -->
			::ZeroMemory ( pK->KKval8, 6 );  
// インボイス_23/07/21_higuchi add <--
			break;
		default:	// 項目名
			::ZeroMemory ( pK->KKnam, sizeof ( pK->KKnam ) );
			break;
		}
	}

	// 集計
// インボイス_23/07/24_higuchi cor -->
	//( *m_Scalc )->Shz_R5AllCalc ( 1 );
// -----------------------------------
	( *m_Scalc )->Shz_31AllCalc( 1 );
// インボイス_23/07/24_higuchi cor <--

	for ( int idx = 0; idx < rowSize; idx++ ) {
		DispJigyoDiagData ( idx );
	}

	return 0;
}

//=====================================
// 手入力サインのクリア
//=====================================
int CShinJigyo31Ex2::ClearManualInputSign()
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
void CShinJigyo31Ex2::SetPrintInfo(CVPrint *pVprn, NPRT_PAR *pPset, SH_PRINT_CMINFO *pPrintCmInfo)
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
int CShinJigyo31Ex2::PrintHyo1(int pno, int fno, CDC *pDC, char *pBp)
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
int CShinJigyo31Ex2::PrintHyo2(int pno, int fno, CDC *pDC, char *pBp)
{
	int	i = 0,
		j = 0;

	int lin = 0,
		max = 0;
	    
	int len = 0, 
		st  = 0;

	char buf[128] = { 0 };

	KKVAL* pK = NULL;
	KKVAL* pJ = ( *m_pTaxListData )->NRec.KJval;

	// 初期化
	memset( PF, '\0', sizeof ( PF ) );

	i = 0;
	memset( pBp, '\0', Msiz );
	char *pBpMv = pBp;

	::ZeroMemory ( buf, sizeof ( buf ) ); 

	//売上(収入)金額
	lin = 0;
	max = clmSize;
	pK = &( *m_pTaxListData )->NRec.KJval[0];

	for ( j = 0; j < max; j++ ) {
		st = 0;
	//----->('23.08.04)tanaka Del
		//if (j == clmE) {
		//	st = DispJudge(j, jigDataLine1);
		//}
		//else if (j == clmG) {
		//	st = DispJudge(j, jigDataLine1);
		//}
		//else {
		//	st = DispJudge(j, jigDataLine1);
		//}
	//<-----
		st = DispJudge(j, jigDataLine1);	//('23.08.04)tanaka Del

		::ZeroMemory(buf, sizeof(buf));
		if (st == TRUE) {
			switch ( j ) {
			case clmA:  // 決算額 A
				m_pArith->l_print ( buf, pK->KKval1, FMTX130 );
				break;
			case clmB:  // Aのうち課税取引にならないもの B
				m_pArith->l_print ( buf, pK->KKval2, FMTX130 );
				break;
			case clmC:  // 課税取引額 C
				m_pArith->l_print ( buf, pK->KKval3, FMTX130 );
				break;
			case clmD:  // うち軽減税率6.24%適用分 D
				m_pArith->l_print ( buf, pK->KKval5, FMTX130 );
				break;
			case clmF:  // うち標準税率7.8%適用分  F
				m_pArith->l_print ( buf, pK->KKval6, FMTX130 );
				break;
			case clmE:
// インボイス_23/07/21_higuchi add -->
				m_pArith->l_print(buf, pK->KKval7, FMTX130);
				break;
// インボイス_23/07/21_higuchi add <--
			case clmG:
// インボイス_23/07/21_higuchi add -->
				m_pArith->l_print(buf, pK->KKval8, FMTX130);
// インボイス_23/07/21_higuchi add <--
				break;
			}
		}

		len = ( int )strlen ( buf );
		memmove ( pBpMv, buf, len );
		PF[i].PITM[0] = pBpMv;
		pBpMv += ( len + 1 );
		i++;
	}

	// 売上原価+差引金額
	lin  = jigDataLine2;
	max  = jigDataLine7 + 1;
	for (int clm = clmA; clm < clmSize; clm++) {
		for (j = 0; j + lin  < max; j++) {
			pK = &(*m_pTaxListData)->NRec.KJval[j + lin];
			st = 0;
            ::ZeroMemory(buf, sizeof(buf));
	//----->('23.08.04)tanaka Del
		//if (j == clmE) {
		//	st = DispJudge(j, jigDataLine1);
		//}
		//else if (j == clmG) {
		//	st = DispJudge(j, jigDataLine1);
		//}
		//else {
		//	st = DispJudge(j, jigDataLine1);
		//}
	//<-----
			st = DispJudge(clm, j + lin);	//('23.08.04)tanaka Del

			if (st == TRUE) {
				switch (clm) {
				case clmA:  // 決算額 A
					m_pArith->l_print(buf, pK->KKval1, FMTX130);
					break;
				case clmB:  // Aのうち課税取引にならないもの B
					m_pArith->l_print(buf, pK->KKval2, FMTX130);
					break;
				case clmC:  // 課税取引額 C
					m_pArith->l_print(buf, pK->KKval3, FMTX130);
					break;
				case clmD:  // うち軽減税率6.24%適用分 D
					m_pArith->l_print(buf, pK->KKval5, FMTX130);
					break;
				case clmE:
// インボイス_23/07/21_higuchi add -->
					m_pArith->l_print(buf, pK->KKval7, FMTX130);
					break;
// インボイス_23/07/21_higuchi add <--
				case clmG:
// インボイス_23/07/21_higuchi add -->
					m_pArith->l_print(buf, pK->KKval8, FMTX130);
					break;
// インボイス_23/07/21_higuchi add <--
				case clmF:  // うち標準税率7.8%適用分  F
					m_pArith->l_print(buf, pK->KKval6, FMTX130);
					break;
				default:
					break;
				}
			}
			len = (int)strlen(buf);
			memmove(pBpMv, buf, len);
			PF[i].PITM[j] = pBpMv;
			pBpMv += (len + 1);
		}
		i++;
	}

	/*
	max = 7;
	//決算額 A
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
	*/

	CString	tmpStr = {};
	bool isLinkKjprf = ( *m_pTaxListData )->IsLinkKjprfname();

	// 経費 項目名 25 - 30
	lin = jigDataLine25;
	max = jigDataLine30 + 1;
	for ( j = 0; ( j + lin ) < max; j++ ) {
		pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
		::ZeroMemory ( buf, sizeof ( buf ) );
		if ( isLinkKjprf ) {
			tmpStr.Empty();
// インボイス_23/07/21_higuchi cor -->
			//if ( ( m_pArith->l_test ( pK->KKval1 ) != 0 ) || ( m_pArith->l_test ( pK->KKval2 ) != 0 ) || ( m_pArith->l_test ( pK->KKval3 ) != 0 ) || ( m_pArith->l_test ( pK->KKval5 ) != 0 ) || ( m_pArith->l_test ( pK->KKval6 ) != 0 ) ) { //金額が全て０なら表示しない
// -----------------------------------
			if ( ( m_pArith->l_test ( pK->KKval1 ) != 0 ) || ( m_pArith->l_test ( pK->KKval2 ) != 0 ) || 
				 ( m_pArith->l_test ( pK->KKval3 ) != 0 ) || ( m_pArith->l_test ( pK->KKval5 ) != 0 ) || 
				 ( m_pArith->l_test ( pK->KKval6 ) != 0 ) || 
				 ( m_pArith->l_test ( pK->KKval7 ) != 0 ) || ( m_pArith->l_test ( pK->KKval8 ) != 0 ) ) {
				//金額が全て０なら表示しない
// インボイス_23/07/21_higuchi cor <--
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

	int fieldNo = (useSmallFontSize()) ? 3 : 2;
	return _xprn ( pno, fieldNo, PF, 0, pDC, 0, 1 );
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
int CShinJigyo31Ex2::PrintHyo3(int pno, int fno, CDC *pDC, char *pBp)
{
	int i = 0,
		j = 0,
		k = 0;

	int len = 0,
		lin = 0,
		max = 0;

	int st = 0;
	char buf[64] = { 0 };
	KKVAL* pK = NULL;
	KKVAL* pJ = ( *m_pTaxListData )->NRec.KJval;

	i = 0;
	memset( pBp, '\0', Msiz );
	char *pBpMv = pBp;

	//経費(租税公課～貸倒金)
	// 決算額 A
	lin = jigDataLine8;
	max = jigDataLine24+1;

	for (int clm = clmA; clm < clmSize; clm++) {
		for ( j = 0; j + lin < max; j++ ) {
			pK = &( *m_pTaxListData )->NRec.KJval[j + lin];
			st = 0;
			::ZeroMemory( buf, sizeof ( buf ) );

	//----->('23.08.04)tanaka Del
		//if (j == clmE) {
		//	st = DispJudge(j, jigDataLine1);
		//}
		//else if (j == clmG) {
		//	st = DispJudge(j, jigDataLine1);
		//}
		//else {
		//	st = DispJudge(j, jigDataLine1);
		//}
	//<-----
			st = DispJudge(clm, j + lin);	//('23.08.04)tanaka Del

			if ( st == TRUE ) {
				switch ( clm ) {
				case clmA:  // 決算額 A
					m_pArith->l_print(buf, pK->KKval1, FMTX130);
					break;
				case clmB:  // Aのうち課税取引にならないもの B
					m_pArith->l_print(buf, pK->KKval2, FMTX130);
					break;
				case clmC:  // 課税取引額 C
					m_pArith->l_print(buf, pK->KKval3, FMTX130);
					break;
				case clmD:  // うち軽減税率6.24%適用分 D
					m_pArith->l_print(buf, pK->KKval5, FMTX130);
					break;
				case clmE:
// インボイス_23/07/21_higuchi add -->
					m_pArith->l_print(buf, pK->KKval7, FMTX130);
					break;
// インボイス_23/07/21_higuchi add <--
				case clmG:
// インボイス_23/07/21_higuchi add -->
					m_pArith->l_print(buf, pK->KKval8, FMTX130);
					break;
// インボイス_23/07/21_higuchi add <--
				case clmF:  // うち標準税率7.8%適用分  F
					m_pArith->l_print(buf, pK->KKval6, FMTX130);
					break;

				default:
					break;
				}
			}
			len = ( int )strlen ( buf );
			memmove ( pBpMv, buf, len );
			PF[i].PITM[j] = pBpMv;
			pBpMv += ( len + 1 );
		}
		i++;
	}


	/*
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
	*/

	// 経費(自由入力欄～計)

	lin = jigDataLine25;
	max = jigDataLine31+1;

	for (int clm = clmA; clm < clmSize; clm++) {
		//自由入力欄、雑費
		for (j = 0; j + lin < max; j++) {
			pK = &(*m_pTaxListData)->NRec.KJval[j + lin];
			st = 0;
			::ZeroMemory(buf, sizeof(buf));

	//----->('23.08.04)tanaka Del
		//if (j == clmE) {
		//	st = DispJudge(j, jigDataLine1);
		//}
		//else if (j == clmG) {
		//	st = DispJudge(j, jigDataLine1);
		//}
		//else {
		//	st = DispJudge(j, jigDataLine1);
		//}
	//<-----
			st = DispJudge(clm, j + lin);	//('23.08.04)tanaka Del

			if (st == TRUE) {
				switch (clm) {
				case clmA:  // 決算額 A
					m_pArith->l_print(buf, pK->KKval1, FMTX130);
					break;
				case clmB:  // Aのうち課税取引にならないもの B
					m_pArith->l_print(buf, pK->KKval2, FMTX130);
					break;
				case clmC:  // 課税取引額 C
					m_pArith->l_print(buf, pK->KKval3, FMTX130);
					break;
				case clmD:  // うち軽減税率6.24%適用分 D
					m_pArith->l_print(buf, pK->KKval5, FMTX130);
					break;
				case clmE:
// インボイス_23/07/21_higuchi add -->
					m_pArith->l_print(buf, pK->KKval7, FMTX130);
					break;
// インボイス_23/07/21_higuchi add <--
				case clmG:
// インボイス_23/07/21_higuchi add -->
					m_pArith->l_print(buf, pK->KKval8, FMTX130);
					break;
// インボイス_23/07/21_higuchi add <--
				case clmF:  // うち標準税率7.8%適用分  F
					m_pArith->l_print(buf, pK->KKval6, FMTX130);
					break;

				default:
					break;
				}
			}
			len = (int)strlen(buf);
			memmove(pBpMv, buf, len);
			PF[i].PITM[j] = pBpMv;
			pBpMv += (len + 1);
		}

		// 計
		pK = &(*m_pTaxListData)->NRec.KJval[jigDataLine32];
		st = 0;
		::ZeroMemory(buf, sizeof(buf));
		st = DispJudge(clm, jigDataLine32);

		if (st == TRUE) {
			switch (clm) {
			case clmA:  // 決算額 A
				m_pArith->l_print(buf, pK->KKval1, FMTX130);
				break;
			case clmB:  // Aのうち課税取引にならないもの B
				m_pArith->l_print(buf, pK->KKval2, FMTX130);
				break;
			case clmC:  // 課税取引額 C
				m_pArith->l_print(buf, pK->KKval3, FMTX130);
				break;
			case clmD:  // うち軽減税率6.24%適用分 D
				m_pArith->l_print(buf, pK->KKval5, FMTX130);
				break;
			case clmE:
// インボイス_23/07/21_higuchi add -->
				m_pArith->l_print(buf, pK->KKval7, FMTX130);
				break;
// インボイス_23/07/21_higuchi add <--
			case clmG:
// インボイス_23/07/21_higuchi add -->
				m_pArith->l_print(buf, pK->KKval8, FMTX130);
				break;
// インボイス_23/07/21_higuchi add <--
			case clmF:  // うち標準税率7.8%適用分  F
				m_pArith->l_print(buf, pK->KKval6, FMTX130);
				break;

			default:
				break;
			}
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);

		i++;
	}



	/*

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
	*/

	// 差引金額33(決算額 A)
	pK = &(*m_pTaxListData)->NRec.KJval[jigDataLine33];
	::ZeroMemory(buf, sizeof(buf));
	if (DispJudge(clmA, jigDataLine33) == 1) {
		m_pArith->l_print(buf, pK->KKval1, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i].PITM[0] = pBpMv;
	pBpMv += (len + 1);
	i++;

	// 34. 3 + 32(決算額 A)
	lin = jigDataLine34;
	pK = &(*m_pTaxListData)->NRec.KJval[lin];
	for (j = 0; j < clmSize; j++) {
		::ZeroMemory(buf, sizeof(buf));
		if (DispJudge(j, jigDataLine34) == 1) {
			switch (j) {
			case clmA:  // 決算額 A
				m_pArith->l_print(buf, pK->KKval1, FMTX130);
				break;
			case clmB:  // Aのうち課税取引にならないもの B
				m_pArith->l_print(buf, pK->KKval2, FMTX130);
				break;
			case clmC:  // 課税取引額 C
				m_pArith->l_print(buf, pK->KKval3, FMTX130);
				break;
			case clmD:  // うち軽減税率6.24%適用分 D
				m_pArith->l_print(buf, pK->KKval5, FMTX130);
				break;
			case clmF:  // うち標準税率7.8%適用分  F
				m_pArith->l_print(buf, pK->KKval6, FMTX130);
				break;
			case clmE:
// インボイス_23/07/21_higuchi add -->
				m_pArith->l_print(buf, pK->KKval7, FMTX130);
				break;
// インボイス_23/07/21_higuchi add <--
			case clmG:
// インボイス_23/07/21_higuchi add -->
				m_pArith->l_print(buf, pK->KKval8, FMTX130);
				break;
// インボイス_23/07/21_higuchi add <--
			default:
				break;
			}
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;



	if (useSmallFontSize()) {
		return _xprn(pno, 5, PF, 0, pDC, 0, 0);
	}
	else {
		return _xprn(pno, 4, PF, 0, pDC, 0);
	}
}

bool CShinJigyo31Ex2::useSmallFontSize() {

// インボイス_23/07/21_higuchi cor -->
	//char val[5][128] = { 0, 0 };
// -----------------------------------
	char val[7][128] = { 0, 0 };
// インボイス_23/07/21_higuchi cor <--
	CString str;

	for ( int l_cnt = jigDataLine1; l_cnt <= jigDataLine34; l_cnt++ ) {
		switch ( l_cnt ) {
		case jigDataLine1:
		case jigDataLine2:
		case jigDataLine3:
		case jigDataLine4:
		case jigDataLine5:
		case jigDataLine6:
		case jigDataLine7:
		case jigDataLine8:
		case jigDataLine9:
		case jigDataLine10:
		case jigDataLine11:
		case jigDataLine12:
		case jigDataLine13:
		case jigDataLine14:
		case jigDataLine15:
		case jigDataLine16:
		case jigDataLine17:
		case jigDataLine18:
		case jigDataLine19:
		case jigDataLine20:
		case jigDataLine21:
		case jigDataLine22:
		case jigDataLine23:
		case jigDataLine24:
		case jigDataLine25:
		case jigDataLine26:
		case jigDataLine27:
		case jigDataLine28:
		case jigDataLine29:
		case jigDataLine30:
		case jigDataLine31:
		case jigDataLine32:
		case jigDataLine33:
// インボイス_23/07/21_higuchi cor -->
			//for (int idx = 0; idx < 5; idx++) {
// -----------------------------------
			for (int idx = 0; idx < 7; idx++) {
// インボイス_23/07/21_higuchi cor <--
				::ZeroMemory(val[idx], sizeof(val[idx]));
			}

			m_pArith->l_print(val[0], (*m_pTaxListData)->NRec.KJval[l_cnt].KKval1, FMTX130);
			m_pArith->l_print(val[1], (*m_pTaxListData)->NRec.KJval[l_cnt].KKval2, FMTX130);
			m_pArith->l_print(val[2], (*m_pTaxListData)->NRec.KJval[l_cnt].KKval3, FMTX130);
			m_pArith->l_print(val[3], (*m_pTaxListData)->NRec.KJval[l_cnt].KKval5, FMTX130);
			m_pArith->l_print(val[4], (*m_pTaxListData)->NRec.KJval[l_cnt].KKval6, FMTX130);
// インボイス_23/07/21_higuchi add -->
			m_pArith->l_print(val[5], (*m_pTaxListData)->NRec.KJval[l_cnt].KKval7, FMTX130);
			m_pArith->l_print(val[6], (*m_pTaxListData)->NRec.KJval[l_cnt].KKval8, FMTX130);
// インボイス_23/07/21_higuchi add <--

// インボイス_23/07/21_higuchi cor -->
			//for (int idx = 0; idx < 5; idx++) {
// -----------------------------------
			for (int idx = 0; idx < 7; idx++) {
// インボイス_23/07/21_higuchi cor <--
				str.Format(val[idx]);
				str.Trim();
				// 10億円 ( カンマ含む )以上ならフォントサイズを下げる
				//if ( str.GetLength() > 13 ) {
				if (str.GetLength() > 12) {
					return true;
				}
			}
			break;
		default:
			break;
		}
	}
	return false;
}


//=====================================
// 初期ポジションセット
//=====================================
void CShinJigyo31Ex2::SetItemPosition()
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
	SetItemPositionEx();
}

void CShinJigyo31Ex2::SetItemPositionEx()
{
	this->SetFocus();
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
		m_JigyoDiag.SetPosition (jigVal01A);
	}
	else {
		m_JigyoDiag.SetPosition (jigName25);
	}

	SetRedrawScroll( 0 );
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
int CShinJigyo31Ex2::FocusMoveCheck(int Index, int Move)
{
	int idx = 0;
	int i = 0, 
		j = 0;
	BOOL end_flg = FALSE;

	int cnt = 0;
	FCSUPDOWN_INFO *pInfo    = NULL,
	               *pInfo_LR = NULL;
	cnt      = FCSitemMax;
	pInfo    = FCS_UD;
	pInfo_LR = FCS_LR;


	DIAGRAM_ATTRIBUTE DA = {};

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
		for ( i = 0; i < cnt; i++ ) {
			if ( Index == ( pInfo + i )->IDNo ) {
				break;
			}
		}
		idx = ( pInfo + i )->IDNo;
	}
	else if ( ( Move == 2 ) || ( Move == 3 ) ) {
		for ( i = 0; i < cnt; i++ ) {
			if ( Index == ( pInfo_LR + i )->IDNo ) {
				break;
			}
			idx = ( pInfo_LR + i )->IDNo;
		}
	}

	// 次ポジション検索
	if ( Move == 0 ) {	//下方向
		for ( j = i + 1; j < cnt; j++ ) {
			m_JigyoDiag.GetAttribute( ( pInfo + j )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo + j )->IDNo;
				break;
			}
		}
	}
	else if ( Move == 1 ) {	//上方向
		for ( j = i - 1; j > -1; j-- ) {
			m_JigyoDiag.GetAttribute( ( pInfo + j )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo + j )->IDNo;
				break;
			}
		}
	}
	else if ( Move == 2 ) {	//右方向
		for ( j = i + 1; j < cnt; j++ ) {
			m_JigyoDiag.GetAttribute( ( pInfo_LR + j )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo_LR + j )->IDNo;
				break;
			}
		}
	}
	else if ( Move == 3 ) {	//左方向
		for ( j = i - 1; j > -1; j-- ) {
			m_JigyoDiag.GetAttribute( ( pInfo_LR + j )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo_LR + j )->IDNo;
				break;
			}
		}
	}

	return idx;
}


BEGIN_EVENTSINK_MAP(CShinJigyo31Ex2, CSyzBaseDlg)
	ON_EVENT(CShinJigyo31Ex2, IDC_ICSDIAGCTRL1, 1, CShinJigyo31Ex2::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinJigyo31Ex2, IDC_ICSDIAGCTRL1, 2, CShinJigyo31Ex2::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinJigyo31Ex2, IDC_ICSDIAGCTRL1, 3, CShinJigyo31Ex2::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	// 22-0167,22-0361 add -->
	//ON_EVENT(CShinJigyo31Ex2, IDC_ICSDIAGCTRL1, 9, CShinJigyo31Ex2::CheckButtonIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
	// 22-0167,22-0361 add <--
END_EVENTSINK_MAP()


void CShinJigyo31Ex2::EditONIcsdiagctrl1(short index)
{
	
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_JigyoDiag.OffControl(FALSE);

	m_Idx = index;

	// 2023/07/11 add -->
	if ((m_Idx >= idxEditMin) && (m_Idx <= idxEditMax)) {
		for (int i = 0; i < rowSize; i++) {
			for (int j = 0; j < clmSize; j++) {
				if (idxMap[i][j] < 0) { continue; }
				if (idxMap[i][j] == index) {
					m_Lin = i;
					m_Clm = j;
				}
			}
		}
	}
	// 2023/07/11 add <--
	// 2023/07/11 del --> 斜線部分へのコントロールを持たないのでindexを単純計算できない 
	/*
	if ((m_Idx >= JIGMINex) && (m_Idx <= JIGMAXex)) {
		m_Lin = (index - JIGMINex) / JIGWIDECNTex;
		m_Clm = (index - JIGMINex) % JIGWIDECNTex;
	}
	*/
	// 2023/07/11 del <--
	else if (m_Idx == jigName25) {
		m_Lin = jigDiagLine25; 
		m_Clm = -1;
	}
	else if (m_Idx == jigName26) {
		m_Lin = jigDiagLine26;	
		m_Clm = -1;
	}
	else if (m_Idx == jigName27) {
		m_Lin = jigDiagLine27;	
		m_Clm = -1;
	}
	else if (m_Idx == jigName28) {
		m_Lin = jigDiagLine28;	
		m_Clm = -1;
	}
	else if (m_Idx == jigName29) {
		m_Lin = jigDiagLine29;	
		m_Clm = -1;
	}
	else if (m_Idx == jigName30) {
		m_Lin = jigDiagLine30;	
		m_Clm = -1;
	}
	m_curwnd = IDC_ICSDIAGCTRL1;
}

void CShinJigyo31Ex2::EditOFFIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA	data = {};
	diag_getdata ( IDC_ICSDIAGCTRL1, index, &data, CIcsdiagctrl );
	int	st = GetDiagData ( index, data.data_check, &data, 0 );
	diag_deledit ( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
	if ( st < 0 ) {
		diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );
	}
}

void CShinJigyo31Ex2::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if ( nChar == 0 ) {
		return;
	}

	DIAGRAM_DATA *idata = NULL;
	DIAGRAM_DATA xdata = {};

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

	SetRedrawScrollToIdx ( m_JigyoDiag.GetPosition(), IDC_ICSDIAGCTRL1);

}

BOOL CShinJigyo31Ex2::PreTranslateMessage(MSG* pMsg)
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
			DIAGRAM_DATA dd = {};
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

 FCSUPDOWN_INFO CShinJigyo31Ex2::FCS_LR[] = {};
 FCSUPDOWN_INFO CShinJigyo31Ex2::FCS_UD[] = {};
void CShinJigyo31Ex2::setFCSArray()
{
	//水平方向フォーカス移動用
	if (FCS_LR[0].IDNo == NULL) {
		int cnt = 0;
		for (int i = 0; i < rowSize; i++) {
			if (i >= jigDiagLine32) { break; }               //計、差引金額、3+32はフォーカス対象外
			if ((i >= jigDiagLine25) && ( i <= jigDiagLine30)) { //項目名欄の挿入処理
				int tmp = (i == jigDiagLine25) ? jigName25
					    : (i == jigDiagLine26) ? jigName26
					    : (i == jigDiagLine27) ? jigName27
					    : (i == jigDiagLine28) ? jigName28
					    : (i == jigDiagLine29) ? jigName29
					    : jigName30;
				FCS_LR[cnt] = { (short)tmp };
				cnt++;
			}
			for (int j = 0; j < clmSize; j++) {
				if(j == clmC) { continue; }        //C列
				if(idxMap[i][j] < 0) { continue; } //斜線部分
				FCS_LR[cnt] = { (short)idxMap[i][j] };
				cnt++;
			}
		}
		FCS_LR[cnt] = { 0 };
	}

	//垂直方向フォーカス移動用
	if (FCS_UD[0].IDNo == NULL) {
		int cnt = 0;
		//項目名欄の挿入処理
		FCS_UD[0] = { (short)jigName25 };
		FCS_UD[1] = { (short)jigName26 };
		FCS_UD[2] = { (short)jigName27 };
		FCS_UD[3] = { (short)jigName28 };
		FCS_UD[4] = { (short)jigName29 };
		FCS_UD[5] = { (short)jigName30 };
		cnt = 6;

		for (int j = 0; j < clmSize; j++) {
			if(j == clmC) { continue; } //C列
			for (int i = 0; i < rowSize; i++) {
				if (i >= jigDiagLine32)   { break; }    //計、差引金額、3+32はフォーカス対象外
				if (idxMap[i][j] < 0) { continue; } //斜線部分
				FCS_UD[cnt] = { (short)idxMap[i][j] };
				cnt++;
			}
		}
		FCS_UD[cnt] = { 0 };
	}

}

// 修正No.168690 add -->
void CShinJigyo31Ex2::DiagReSetPosition(void)
{
	this->SetFocus();
}
// 修正No.168690 add <--

