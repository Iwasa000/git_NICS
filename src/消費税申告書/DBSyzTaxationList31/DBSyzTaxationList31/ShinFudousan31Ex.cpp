// ShinFudousan31Ex.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFudousan31Ex.h"
#include "ShinFudousan31ExIdx.h"


// CShinFudousan31Ex ダイアログ

IMPLEMENT_DYNAMIC(CShinFudousan31Ex, CSyzBaseDlg)

CShinFudousan31Ex::CShinFudousan31Ex(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFudousan31Ex::IDD, pParent)
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

CShinFudousan31Ex::~CShinFudousan31Ex()
{
}

void CShinFudousan31Ex::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Fudousan31);
}


BEGIN_MESSAGE_MAP(CShinFudousan31Ex, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinFudousan31Ex メッセージ ハンドラ

BOOL CShinFudousan31Ex::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Fudousan31;
//	m_BottomIdx = 36;
	m_BottomIdx = 31;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFudousan31Ex::Init()
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
void CShinFudousan31Ex::InitDiagAttr()
{
	DIAGRAM_ATTRIBUTE data;
	long dg1[] = { KFUDKNAME03ex, KFUDKNAME12ex, -1 };

	for( int i=0;; i++ ){
		if( dg1[i] == -1 ){
			break;
		}
		// ank対応
		m_Fudousan31.GetAttribute( (short)dg1[i], (LPUNKNOWN)&data );
		data.attr_editattr |= DIAG_EATTR_ZENHAN;//DIAG_EATTR_ZENHAN ank対応
		m_Fudousan31.SetAttribute( (short)dg1[i], (LPUNKNOWN)&data, 1 );
	}

	SH_ITEMINDEX *pIndex = NULL;
	if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) {
		pIndex = R02_Fudousan31Ex;
	}
	else {
		pIndex = Fudousan31Ex;
	}

	int	cnt = 0;
	while( 1 ){
		//if( Fudousan31Ex[cnt].Index == 0 ){
		if ( ( pIndex + cnt )->Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			//Flg = Fudousan31Ex[cnt].AllUnConnect;
			Flg = ( pIndex + cnt )->AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				//Flg = Fudousan31Ex[cnt].UnConnect;
				Flg = ( pIndex + cnt )->UnConnect;
				//if ( ( *m_pSnHeadData )->SVmzsw != 1 ) {
				//	//if( ( Fudousan31Ex[cnt].Index % 6 ) == 1 ){

				//	// 簡易課税の時は経費の欄を入力可能
				//	//if ( ( ( pIndex + cnt )->Index % 6 ) == 1 ){
				//	//	Flg = 0;
				//	//}
				//	if ( CheckSVmzsw ( ( pIndex + cnt )->Index ) == 1 ) {
				//		Flg = 0;
				//	}
				//}
			}
			else{
				//Flg = Fudousan31Ex[cnt].Connect;
				Flg = ( pIndex + cnt )->Connect;
			}
		}
		//ATRIB_MOD( IDC_ICSDIAGCTRL1, Fudousan31Ex[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Fudousan31Ex[cnt].Connect );
		ATRIB_MOD( IDC_ICSDIAGCTRL1, ( pIndex + cnt )->Index, Flg, (*m_pSnHeadData)->Sn_Sign4, ( pIndex + cnt )->Connect );
		cnt++;
	}

	InitDiagonalLines();

	SetFocusRange();

	m_Fudousan31.EnableDelete(0);//DELETEキー１回で削除モード設定'14.08.06

	
}

//====================================================
// 斜線の表示/非表示
//====================================================
void CShinFudousan31Ex::InitDiagonalLines()
{
	long flg = 0; 
	if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) {
		flg = DIAG_MDFY_OPAQUE;
	}
	else {
		flg = DIAG_MDFY_TRANSPARENT;
	}

	for ( short index = FDIAGONALLINE_Sex; index <= FDIAGONALLINE_Eex; index++ ) {
		m_Fudousan31.ModifyItem ( index, flg );
	}
}

//-----------------------------------------------------------------------------
// 画面表示
//-----------------------------------------------------------------------------
// 引数	mode	：	
//-----------------------------------------------------------------------------
void CShinFudousan31Ex::DispList( int mode )
{
// 22-0167,22-0361 add -->
	// 入力可／不可を切り替える
	ColumnControl();
// 22-0167,22-0361 add <--
	m_Fudousan31.OffControl( TRUE );
//2016.06.22 INSERT START
	(*m_Scalc)->Shz_31AllCalc(2);
//2016.06.22 INSERT END
	short	i;
	for( i = 0; i < FKLINCNT; i++ ){
		DispFudousan31DiagData( i );
	}

	DIAGRAM_DATA	data;
	if( !mode ){
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, FKYEARIDex, TRUE, CIcsdiagctrl );
		data.data_disp.Format( _T("（令和%2x年分）"), (*m_pTaxListData)->YEAR );
		diag_setdata( IDC_ICSDIAGCTRL1, FKYEARIDex, &data, CIcsdiagctrl );

		PostMessage(WM_VSCROLL, SB_TOP, 0 );
		m_Idx = FKVMINex;
	}

	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, FKZKBNIDex, TRUE, CIcsdiagctrl );
	data.data_disp = _T("経理方式：");
	if( (*m_pTaxListData)->Head.KHstax&0x01 ){
		data.data_disp += _T("税抜き");
	}
	else{
		data.data_disp += _T("税込み");
	}
	diag_setdata( IDC_ICSDIAGCTRL1, FKZKBNIDex, &data, CIcsdiagctrl );

	int edit[] = {KFUDKNAME03ex,KFUDKNAME12ex};
	BOOL edit_focus = FALSE;

	for( int cnt = 0; cnt < ARRAYCOUNT(edit); cnt++ ){
		Switch_KamokuEdit( IDC_ICSDIAGCTRL1, edit[cnt] );

		if( m_Fudousan31.GetPosition() == edit[cnt] ){
			edit_focus = TRUE;
		}
	}

	if(( (*m_pTaxListData)->Head.KHkoke == 1 ) && ( edit_focus == TRUE )){
		this->SetFocus();
	}
// 22-0167,22-0361 add -->
	KKVAL*	pK = &( *m_pTaxListData )->NRec.KFval[0];
	int	idx=0;
	for(idx=FUSELChk2ex; idx<=FUSELChk3ex; idx++)	{
		diag_getdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
		data.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
	}
	idx=0;
	if((*m_pSnHeadData)->Sn_GenericSgn & 0x800)		idx=FUSELChk3ex;
	//if(pK->KKauto & 0x40)		idx=FUSELChk2ex;
	//else if(pK->KKauto & 0x80)	idx=FUSELChk3ex;
	if(idx != 0)	{
		diag_getdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
		data.data_check = 1;
		diag_setdata(IDC_ICSDIAGCTRL1,idx,&data,CIcsdiagctrl);
	}
	//// 確定時または連動時は選択不可
	//if(((*m_pSnHeadData)->Sn_Sign4 & 0x01) || (!((*m_pSnHeadData)->Sn_Sign4 & 0x80)))	{
	//	for(idx=FUSELChk2ex; idx<=FUSELChk3ex; idx++)	{
	//		ATRIB_MOD(IDC_ICSDIAGCTRL1,idx,0,(*m_pSnHeadData)->Sn_Sign4,0);
	//	}
	//}
	// 6.24%適用分は非表示にする(今後要望があったときのために残しておく)
	diag_modify(IDC_ICSDIAGCTRL1,FUSELChk2ex,DIAG_MDFY_TRANSPARENT,CIcsdiagctrl);
	// 連動時は7.8%適用分も非表示にする
	if(!((*m_pSnHeadData)->Sn_Sign4 & 0x80))	{
		diag_modify(IDC_ICSDIAGCTRL1,FUSELChk3ex,DIAG_MDFY_TRANSPARENT,CIcsdiagctrl);
	}
	// 確定時は7.8%適用分を選択不可にする
	else if((*m_pSnHeadData)->Sn_Sign4 & 0x01)	{
		//ATRIB_MOD(IDC_ICSDIAGCTRL1,FUSELChk3ex,0,(*m_pSnHeadData)->Sn_Sign4,0);
		DIAGRAM_ATTRIBUTE	da;
		diag_modify(IDC_ICSDIAGCTRL1,FUSELChk3ex,DIAG_MDFY_READONLY,CIcsdiagctrl);
		diag_getattr(IDC_ICSDIAGCTRL1,FUSELChk3ex,&da,CIcsdiagctrl);
		da.attr_bcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAGCTRL1,FUSELChk3ex,&da,FALSE,CIcsdiagctrl);
	}
	else	{
		DIAGRAM_ATTRIBUTE	da,da2;
		diag_getattr(IDC_ICSDIAGCTRL1,0,&da2,CIcsdiagctrl);
		diag_getattr(IDC_ICSDIAGCTRL1,FUSELChk3ex,&da,CIcsdiagctrl);
		da.attr_bcolor = da2.attr_bcolor;
		diag_setattr(IDC_ICSDIAGCTRL1,FUSELChk3ex,&da,FALSE,CIcsdiagctrl);
		diag_modify(IDC_ICSDIAGCTRL1,FUSELChk3ex,DIAG_MDFY_EDIT,CIcsdiagctrl);
	}
// 22-0167,22-0361 add <--
}

//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
// 引数	dlin	：	
//-----------------------------------------------------------------------------
void CShinFudousan31Ex::DispFudousan31DiagData( long dlin )
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

	idx = (short)(dlin * FKWIDECNTex) + FKVMINex;
	for( i = 0 ; i < FKWIDECNTex ; i++ ){
		st = 0;
		::ZeroMemory( val, sizeof(val) );
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, idx+i, TRUE, CIcsdiagctrl );
		switch( i ){
			case 0:	// 決算額 A
				memcpy( val, pK->KKval1, 6 );
				break;

			case 1:	// Aのうち課税取引にならないもの　B
				memcpy( val, pK->KKval2, 6 );
				break;

			case 2:	// 課税取引金額　C（A－B）
				memcpy( val, pK->KKval3, 6 );
				break;

			//case 3:	// うち税率６．３％適用分　D
			//	memcpy( val, pK->KKval4, 6 );
			//	break;

			//case 4:	// うち税率６．２４％適用分　E
			case 3:	// うち税率６．２４％適用分　D
				memcpy( val, pK->KKval5, 6 );
				break;
			//case 5:	// うち税率７．８％適用分　F
			case 4:	// うち税率７．８％適用分　E
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

	if(dlin == KFUDLINE3 || dlin == KFUDLINE12){//項目名入力可所　③　⑫　の場合
		if( dlin == KFUDLINE12 ){
			lin = KFudDataPos[dlin + 1];
			pK = &(*m_pTaxListData)->NRec.KFval[lin];
		}
		if( !(pK->KKsign&0x80) ){
			m_Util.DiagOcxIniz( &data );
			::ZeroMemory( buf, sizeof(buf) );

			CString tmp;
			int prf_no = 0;
			switch( dlin ){
				case KFUDLINE3:	//③
					idx = KFUDKNAME03ex;
					prf_no = 3;
					break;
				case KFUDLINE12://⑫
					idx = KFUDKNAME12ex;
					prf_no = 12;
					break;
			}

			if( (*m_pTaxListData)->IsLinkKjprfname() && (prf_no!=0) ){
				if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)||(m_pArith->l_test(pK->KKval4)!=0)||(m_pArith->l_test(pK->KKval5)!=0)|| (m_pArith->l_test(pK->KKval6) != 0)){ //金額が全て０なら表示しない
					(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmsub, ID_ICSSH_KJ_FUDOUSAN, prf_no, tmp );
					memcpy( buf, tmp, tmp.GetLength() );
				}
			}else{
				memcpy( buf, pK->KKnam, sizeof(pK->KKnam));
			}

			diag_clear( IDC_ICSDIAGCTRL1, idx, TRUE, CIcsdiagctrl );
			data.data_edit = buf;
			diag_setdata( IDC_ICSDIAGCTRL1, idx, &data, CIcsdiagctrl );
		}

	}
	
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
int CShinFudousan31Ex::GetDiagData( long index, short leng, DIAGRAM_DATA *pData, int mode )
{
	//'14.08.21
// 22-0167,22-0361 cor -->
	//if( index < 0 ){
// -----------------------
	if(index < 0 || index == FUSELChk2ex || index == FUSELChk3ex) {
// 22-0167,22-0361 cor <--
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

			//case 3:		// うち税率６．３％適用分　D
			//	if( leng ){
			//		memcpy( pK->KKval4, pData->data_val, 6 );
			//	}
			//	break;

			//case 4:		// うち税率６．２４％適用分　E
			case 3:		// うち税率６．２４％適用分　D
				if( leng ){
					memcpy( pK->KKval5, pData->data_val, 6 );
				}
				break;
			//case 5:		// うち税率７．８％適用分　F
			case 4:		// うち税率７．８％適用分　E
				if (leng) {
					memcpy(pK->KKval6, pData->data_val, 6);
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
			case 3:		// うち税率６．２４％適用分　D
				::ZeroMemory( pK->KKval5, 6 );	
				break;
			//case 5:		// うち税率７．８％適用分　F
			case 4:		// うち税率７．８％適用分　E
				::ZeroMemory(pK->KKval6, 6);
			break;			default:	// 項目名
				::ZeroMemory( pK->KKnam, sizeof(pK->KKnam));
				break;
		}
	}

	// 集計
	(*m_Scalc)->Shz_31AllCalc(2);//'14.08.06
	for( int i = 0 ; i < FKLINCNT ; i++ ){
		DispFudousan31DiagData( i );
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
		DispFudousan31DiagData( i );
	}
	return 0;

	*/

}


BEGIN_EVENTSINK_MAP(CShinFudousan31Ex, CSyzBaseDlg)
ON_EVENT(CShinFudousan31Ex, IDC_ICSDIAGCTRL1, 1, CShinFudousan31Ex::EditONIcsdiagctrl1, VTS_I2)
ON_EVENT(CShinFudousan31Ex, IDC_ICSDIAGCTRL1, 2, CShinFudousan31Ex::EditOFFIcsdiagctrl1, VTS_I2)
ON_EVENT(CShinFudousan31Ex, IDC_ICSDIAGCTRL1, 3, CShinFudousan31Ex::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
// 22-0167,22-0361 add -->
ON_EVENT(CShinFudousan31Ex, IDC_ICSDIAGCTRL1, 9, CShinFudousan31Ex::CheckButtonIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
// 22-0167,22-0361 add <--
END_EVENTSINK_MAP()

void CShinFudousan31Ex::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_Fudousan31.OffControl( FALSE );

	m_Idx = index;
	if( (m_Idx >= FKVMINex) && (m_Idx <= FKVMAXex) ){
		m_Lin = (index - FKVMINex ) / FKWIDECNTex;
		m_Clm = (index - FKVMINex ) % FKWIDECNTex;
	}
	else if( m_Idx == KFUDKNAME03ex ){
		m_Lin = KFUDLINE3;
		m_Clm = -1;
	}
	else if( m_Idx == KFUDKNAME12ex ){
		m_Lin = KFUDLINE12 + 1;//変換等対価の行分＋１する
//		m_Lin = KFUDLINE12;
		m_Clm = -1;
	}
	m_curwnd = IDC_ICSDIAGCTRL1;//'14.08.01

}

void CShinFudousan31Ex::EditOFFIcsdiagctrl1(short index)
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

void CShinFudousan31Ex::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
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
	//		m_Fudousan31.SetPrevPosition();
	//	}
	//	else{
//			pos = UpDownFocusMoveCheck(index , 1);
			pos = FocusMoveCheck(index , 1);
			m_Fudousan31.SetPosition(pos);	
	//	}
	}
	if( wTerm==VK_DOWN ){
	//	if(index == KFUDVAL15E){
	//		m_Fudousan31.SetNextPosition();
	//	}
	//	else{
//			pos = UpDownFocusMoveCheck(index , 0);
			pos = FocusMoveCheck(index , 0);
			m_Fudousan31.SetPosition(pos);
	//	}
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
//		m_Fudousan31.SetNextPosition();
		pos = FocusMoveCheck(index , 2);
		m_Fudousan31.SetPosition(pos);	
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
//		m_Fudousan31.SetPrevPosition();
			pos = FocusMoveCheck(index , 3);
			m_Fudousan31.SetPosition(pos);	

	}

	//------->DELETEキー対応'14.08.06
	if( wTerm == VK_DELETE){
		GetDiagData( index, length, &xdata, -1 );
	}

	SetRedrawScrollToIdx(m_Fudousan31.GetPosition(), IDC_ICSDIAGCTRL1);

/*	int nowpos = m_Fudousan31.GetPosition();	// 移動先または自身のポジション

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
//int CShinFudousan31Ex::UpDownFocusMoveCheck(int Index , int Move)
int CShinFudousan31Ex::FocusMoveCheck(int Index , int Move)
{
	int idx = 0;
	int i = 0 , j = 0;
	BOOL end_flg = FALSE;

	DIAGRAM_ATTRIBUTE	DA;

	int cnt = 0;
	FCSUPDOWN_INFO *pInfo = NULL,
				   *pInfo_LR = NULL;

	if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) {
		cnt = R02_FCS_SHINFUDOUSAN31CNTex;
		pInfo = R02_FCS_ShinFudousan31Ex;
		pInfo_LR = R02_FCS_ShinFudousan31Ex_LR;
	}
	else {
		cnt = FCS_SHINFUDOUSAN31CNTex;
		pInfo = FCS_ShinFudousan31Ex;
		pInfo_LR = FCS_ShinFudousan31Ex_LR;
	}

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
		//for(i = 0 ; i < FCS_SHINFUDOUSAN31CNT; i++){
		//	if(Index == FCS_ShinFudousan31Ex[i].IDNo){
		//		break;
		//	}
		//}
		//idx = FCS_ShinFudousan31Ex[i].IDNo;//現在ポジションセット
		for ( i = 0; i < cnt; i++ ) {
			if ( Index == ( pInfo + i )->IDNo ) {
				break;
			}
		}
		idx = ( pInfo + i )->IDNo;
	}else if(( Move == 2 )||( Move == 3 )){
		//for(i = 0 ; i < FCS_SHINFUDOUSAN31CNT; i++){
		//	if(Index == FCS_ShinFudousan31Ex_LR[i].IDNo){
		//		break;
		//	}
		//}
		//idx = FCS_ShinFudousan31Ex_LR[i].IDNo;//現在ポジションセット
		for ( i = 0; i < cnt; i++ ) {
			if ( Index == ( pInfo_LR + i )->IDNo ) {
				break;
			}
		}
		idx = ( pInfo_LR + i )->IDNo;
	}

	//次ポジション検索
	if(Move == 0){//下方向
		//for(j = i + 1 ; j < FCS_SHINFUDOUSAN31CNT; j++){
		//	m_Fudousan31.GetAttribute(FCS_ShinFudousan31Ex[j].IDNo ,  (LPUNKNOWN)&DA);
		//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
		//		idx = FCS_ShinFudousan31Ex[j].IDNo;
		//		break;
		//	}
		//}
		for ( j = i + 1; j < cnt; j++ ) {
			m_Fudousan31.GetAttribute ( ( pInfo + i )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo + j )->IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		//for(j = i - 1 ; j > -1 ; j--){
		//	m_Fudousan31.GetAttribute(FCS_ShinFudousan31Ex[j].IDNo ,  (LPUNKNOWN)&DA);
		//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
		//		idx = FCS_ShinFudousan31Ex[j].IDNo;
		//		break;
		//	}
		//}
		for ( j = i - 1; j > -1; j-- ) {
			m_Fudousan31.GetAttribute ( ( pInfo + i )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo + j )->IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		//for(j = i + 1 ; j < FCS_SHINFUDOUSAN31CNT; j++){
		//	m_Fudousan31.GetAttribute(FCS_ShinFudousan31Ex_LR[j].IDNo ,  (LPUNKNOWN)&DA);
		//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
		//		idx = FCS_ShinFudousan31Ex_LR[j].IDNo;
		//		break;
		//	}
		//}
		for ( j = i + 1; j < cnt; j++ ) {
			m_Fudousan31.GetAttribute ( ( pInfo_LR + j )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo_LR + j )->IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		//for(j = i - 1 ; j > -1 ; j--){
		//	m_Fudousan31.GetAttribute(FCS_ShinFudousan31Ex_LR[j].IDNo ,  (LPUNKNOWN)&DA);
		//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
		//		idx = FCS_ShinFudousan31Ex_LR[j].IDNo;
		//		break;
		//	}
		//}
		for ( j = i - 1; j > -1; j-- ) {
			m_Fudousan31.GetAttribute ( ( pInfo_LR + j )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo_LR + j )->IDNo;
				break;
			}
		}
	}
	return idx;
}
//<-----------------------------------------

void CShinFudousan31Ex::SetFocusRange()
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
		cnt = R02_FCS_SHINFUDOUSAN31CNTex;
		pInfo = R02_FCS_ShinFudousan31Ex;
		pInfo_LR = R02_FCS_ShinFudousan31Ex_LR;
	}
	else {
		cnt = FCS_SHINFUDOUSAN31CNTex;
		pInfo = FCS_ShinFudousan31Ex_LR;
		pInfo_LR = FCS_ShinFudousan31Ex_LR;
	}

	//for( int idx = 0; idx < FCS_SHINFUDOUSAN31CNT; idx++ ){
	//	m_Fudousan31.GetAttribute(FCS_ShinFudousan31Ex[idx].IDNo ,  (LPUNKNOWN)&DA);
	//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
	//		m_FcsUpEnd = FCS_ShinFudousan31Ex[idx].IDNo;
	//		break;
	//	}
	//}
	for ( int idx = 0; idx < cnt; idx++ ) {
		m_Fudousan31.GetAttribute ( ( pInfo + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsUpEnd = ( pInfo + idx )->IDNo;
			break;
		}
	}

	//for( int idx = 0; idx < FCS_SHINFUDOUSAN31CNT; idx++ ){
	//	m_Fudousan31.GetAttribute(FCS_ShinFudousan31Ex_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
	//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
	//		m_FcsLeftEnd = FCS_ShinFudousan31Ex_LR[idx].IDNo;
	//		break;
	//	}
	//}
	for ( int idx = 0; idx < cnt; idx++ ) {
		m_Fudousan31.GetAttribute ( ( pInfo_LR + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsLeftEnd = ( pInfo_LR + idx )->IDNo;
			break;
		}
	}

	//for( int idx = FCS_SHINFUDOUSAN31CNT -1; idx > -1 ; idx-- ){
	//	m_Fudousan31.GetAttribute(FCS_ShinFudousan31Ex[idx].IDNo ,  (LPUNKNOWN)&DA);
	//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
	//		m_FcsDownEnd = FCS_ShinFudousan31Ex[idx].IDNo;
	//		break;
	//	}
	//}
	for ( int idx = cnt -1; idx > -1; idx-- ) {
		m_Fudousan31.GetAttribute ( ( pInfo + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsDownEnd = ( pInfo + idx )->IDNo;
			break;
		}
	}

	//for( int idx = FCS_SHINFUDOUSAN31CNT -1; idx > -1 ; idx-- ){
	//	m_Fudousan31.GetAttribute(FCS_ShinFudousan31Ex_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
	//	if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
	//		m_FcsRightEnd = FCS_ShinFudousan31Ex_LR[idx].IDNo;
	//		break;
	//	}
	//}
	for ( int idx = cnt -1; idx > -1; idx-- ) {
		m_Fudousan31.GetAttribute ( ( pInfo_LR + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsRightEnd = ( pInfo_LR + idx )->IDNo;
			break;
		}
	}
}

BOOL CShinFudousan31Ex::PreTranslateMessage(MSG* pMsg)
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
void CShinFudousan31Ex::SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo )
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
int CShinFudousan31Ex::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
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
//int CShinFudousan31Ex::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
int CShinFudousan31Ex::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
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
		if( DispJudge( 2, j ) == 1 ){
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
//		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
//		::ZeroMemory( buf, sizeof(buf) );
//		if( DispJudge( 3, j ) == 1 ){
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
		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 3, j ) == 1 ){
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
		pK = &(*m_pTaxListData)->NRec.KFval[j + lin];
		::ZeroMemory(buf, sizeof(buf));
		if (DispJudge(4, j) == 1) {
			m_pArith->l_print(buf, pK->KKval6, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
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
		if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)||(m_pArith->l_test(pK->KKval4)!=0)||(m_pArith->l_test(pK->KKval5)!=0)|| (m_pArith->l_test(pK->KKval6) != 0)){ //金額が全て０なら表示しない
			(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmsub, ID_ICSSH_KJ_FUDOUSAN, (KFUDLINE3+1), tmpStr );
		}
		len = tmpStr.GetLength();
		memmove( pBpMv, tmpStr, len );
	}
	else{
		len = (int)strlen( pK->KKnam );
		memmove( pBpMv, pK->KKnam , len );
	}
//---------------
	if( len > 14 ){	// 文字数が8文字以上のとき
		pbclr(0, 1, &PF[i]);
		i++;

		PF[i].PITM[0] = pBpMv;
		pBpMv += ( len+1 );
		i++;
	}
	else{
		PF[i].PITM[0] = pBpMv;
		pBpMv += ( len+1 );
		i++;

		pbclr(0, 1, &PF[i]);
		i++;
	}

	pK = &(*m_pTaxListData)->NRec.KFval[KFUDLINE12];
	::ZeroMemory( buf, sizeof(buf) );
//-- '16.12.07 --
//	len = (int)strlen( pK->KKnam );
//	memmove( pBpMv, pK->KKnam , len );
//---------------
	if( isLinkKjprf ){
		tmpStr.Empty();
		if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)||(m_pArith->l_test(pK->KKval4)!=0)||(m_pArith->l_test(pK->KKval5)!=0)|| (m_pArith->l_test(pK->KKval6) != 0)){ //金額が全て０なら表示しない
			(*m_pTaxListData)->GetKoketsuMeisyo( m_pZmsub, ID_ICSSH_KJ_FUDOUSAN, (KFUDLINE12+1), tmpStr );
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
		char val[6][128];
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
					//for(int idx = 0;idx<6;idx++){
					for(int idx = 0;idx<5;idx++){
						::ZeroMemory( val[idx], sizeof( val[idx] ));
					}
					m_pArith->l_print( val[0],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval1, FMTX130 );
					m_pArith->l_print( val[1],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval2, FMTX130 );
					m_pArith->l_print( val[2],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval3, FMTX130 );
					//m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval4, FMTX130 );
					//m_pArith->l_print( val[4],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval5, FMTX130 );
					m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval5, FMTX130 );
					//m_pArith->l_print( val[5],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval6, FMTX130 );
					m_pArith->l_print( val[4],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval6, FMTX130 );
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
//int CShinFudousan31Ex::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
int CShinFudousan31Ex::PrintHyo3( int pno, int fno, CDC *pDC, char *pBp )
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

//// うち税率６．３％適用分 D
//	for( j=0; (j+lin)<max; j++ ){
//		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
//		::ZeroMemory( buf, sizeof(buf) );
///*		st = 0;
//		switch( j+lin ){
//			case 13:	//必要経費　計
//				for( int k=4;k<13;k++ ){
//					if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval4)){
//						st = 1;
//						break;
//					}
//				}
//				break;
//			default:
//				st = 0;
//				break;
//		}
//		if( st == 0 ){
//			m_pArith->l_print( buf, pK->KKval4, FMTX13 );
//		}else{
//			m_pArith->l_print( buf, pK->KKval4, FMTX130 );
//		}*/
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
		pK = &(*m_pTaxListData)->NRec.KFval[j + lin];
		::ZeroMemory(buf, sizeof(buf));
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
		if (DispJudge(4, j + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval6, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
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

//// うち税率６．３％適用分 D
//		pK = &(*m_pTaxListData)->NRec.KFval[14];
//		::ZeroMemory( buf, sizeof(buf) );
///*		st = 0;
//		for( int k=0;k<13;k++ ){
//			if( k == 3 ) continue;
//			if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[k].KKval4)){
//				st = 1;
//				break;
//			}
//		}
//		if( st == 0 ){
//			m_pArith->l_print( buf, pK->KKval4, FMTX13 );
//		}else{
//			m_pArith->l_print( buf, pK->KKval4, FMTX130 );
//		}*/
//		len = (int)strlen( buf );
//		memmove( pBpMv, buf, len );
//		PF[i].PITM[0] = pBpMv;
//		pBpMv += ( len+1 );
//		i++;

// うち税率６．２４％適用分 E
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

// うち税率７．８％適用分 F
		pK = &(*m_pTaxListData)->NRec.KFval[14];
		::ZeroMemory(buf, sizeof(buf));
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
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[0] = pBpMv;
		pBpMv += (len + 1);
		i++;

		int fontsize = 0;
		char val[6][128];
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
					//for(int idx = 0;idx<6;idx++){
					for(int idx = 0;idx<5;idx++){
						::ZeroMemory( val[idx], sizeof( val[idx] ));
					}
					m_pArith->l_print( val[0],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval1, FMTX130 );
					m_pArith->l_print( val[1],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval2, FMTX130 );
					m_pArith->l_print( val[2],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval3, FMTX130 );
					//m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval4, FMTX130 );
					//m_pArith->l_print( val[4],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval5, FMTX130 );
					m_pArith->l_print( val[3],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval5, FMTX130 );
					//m_pArith->l_print( val[5],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval6, FMTX130 );
					m_pArith->l_print( val[4],(*m_pTaxListData)->NRec.KFval[l_cnt].KKval6, FMTX130 );
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
// 初期ポジションセット ('14.08.08)
//-----------------------------------------------------------------------------
void CShinFudousan31Ex::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
		if( (index = m_Fudousan31.GetPosition()) != -1) {
			m_Fudousan31.SetPosition(index);
			m_Fudousan31.SetFocus();
			return;
		}
	}

	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Fudousan31.SetPosition(KFUDVAL01Aex);
	}
	else{
		m_Fudousan31.SetPosition(KFUDKNAME03ex);
	}
	SetRedrawScroll( 0 );

}

void CShinFudousan31Ex::SetItemPositionEx()
{
	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Fudousan31.SetPosition(KFUDVAL01Aex);
	}
	else{
		m_Fudousan31.SetPosition(KFUDKNAME03ex);
	}
	SetRedrawScroll( 0 );
}

//-----------------------------------------------------------------------------
// 表示判定
// 返送値：0…表示なし　1…表示あり
// 引数　：clm…列　line…行
//-----------------------------------------------------------------------------
int CShinFudousan31Ex::DispJudge( int clm, int line )
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
		//		case KFUDLINE6:		//損害保険料
		//		case KFUDLINE8:		//減価償却費
		//		case KFUDLINE9:		//借入金利子
		//		case KFUDLINE15:	//差引金額
		//			//入力可所無し
		//			break;

		//		case KFUDLINE4:		//収入金額　計
		//			for( j = KFUDLINE1; j < KFUDLINE4 ; j++ ){
		//				if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval4) ){
		//					st = 1;
		//					break;
		//				}
		//			}
		//			break;

		//		case KFUDLINE14:	//必要経費　計
		//			for( j = KFUDLINE5 ; j < KFUDLINE14 ; j++ ){
		//				if(j == KFUDLINE6 || j == KFUDLINE8 || j == KFUDLINE9){//損害保険料、減価償却費、借入金利子　は入力不可なのでチェックしない
		//					continue;
		//				}
		//				if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval4) ){
		//					st = 1;
		//					break;
		//				}
		//			}
		//			break;

		//		default:
		//			if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval4) ){
		//				st = 1;
		//			}
		//			break;
		//	}
		//	break;


		//case 4:	// うち税率６．２４％適用分　E
		case 3:	// うち税率６．２４％適用分　D
			switch( line ){
				case KFUDLINE6:		//損害保険料
				case KFUDLINE8:		//減価償却費
				case KFUDLINE9:		//借入金利子
				case KFUDLINE15:	//差引金額
					//入力可所無し
					break;

				//--->新様式チェック時は下記項目は斜線部分なので非表示yoshida201216
				case KFUDLINE1:		//賃貸料
				case KFUDLINE2:		//礼金・権利金更新料
				case KFUDLINE5:		//租税公課
				case KFUDLINE7:		//修繕費
				case KFUDLINE10:	//地代家賃
				case KFUDLINE11:	//給料賃金
					if ((*m_pSnHeadData)->m_DispTabSgn & 0x1000) {//新様式チェックあり
					}
					else {//新様式チェック無し
						if (m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval5)) {
							st = 1;
						}
					}
					break;
				//<----

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
			break;

		//case 5:	// うち税率７．８％適用分　F
		case 4:	// うち税率７．８％適用分　E
			switch (line) {
			case KFUDLINE6:		//損害保険料
			case KFUDLINE8:		//減価償却費
			case KFUDLINE9:		//借入金利子
			case KFUDLINE15:	//差引金額
				//入力可所無し
				break;

			case KFUDLINE4:		//収入金額　計
				for (j = KFUDLINE1; j < KFUDLINE4; j++) {
					if (m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval6)) {
						st = 1;
						break;
					}
				}
				break;

			case KFUDLINE14:	//必要経費　計
				for (j = KFUDLINE5; j < KFUDLINE14; j++) {
					if (j == KFUDLINE6 || j == KFUDLINE8 || j == KFUDLINE9) {//損害保険料、減価償却費、借入金利子　は入力不可なのでチェックしない
						continue;
					}
					if (m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval6)) {
						st = 1;
						break;
					}
				}
				break;

			default:
				if (m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval6)) {
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
int CShinFudousan31Ex::ClearManualInputSign()
{
	int rt = 0;

	for( int idx = 0; idx < 50; idx++ ){
		(*m_pTaxListData)->NRec.KFval[idx].KKauto = 0x00;
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
//int CShinFudousan31Ex::CheckSVmzsw ( int index )
//{
//	int ret = 0;
//
//	if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
//		return ret;
//	}
//
//	switch ( index ) {
//	case KFUDVAL01D:
//	case KFUDVAL02D:
//	case KFUDVAL03D:
//	case KFUDVAL04D:
//	case KFUDVAL04TD:
//		ret = 1;
//		break;
//	case KFUDVAL05D:
//	case KFUDVAL06D:
//	case KFUDVAL07D:
//	case KFUDVAL08D:
//	case KFUDVAL09D:
//	case KFUDVAL10D:
//	case KFUDVAL11D:
//	case KFUDVAL12D:
//	case KFUDVAL13D:
//	case KFUDVAL14D:
//	case KFUDVAL15D:
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
void CShinFudousan31Ex::CheckButtonIcsdiagctrl1(short index, LPUNKNOWN data)
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
		//short	pos = m_Fudousan31.GetPosition();
		m_Fudousan31.DeleteInput();
		diag_getdata(IDC_ICSDIAGCTRL1,index,&xdata,CIcsdiagctrl);
		if(!idata->data_check)	xdata.data_check = 1;
		else					xdata.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1,index,&xdata,CIcsdiagctrl);
		//m_Fudousan31.SetPosition(pos);
		SetItemPositionEx();
		return;
	}

	// 現在は画面に自動計算チェックを１つしか設けていないので
	// 先頭レコードにのみ保存でもよいが
	// 今後のこと（ブロックごとに自動計算サインが必要という要望がきたとき）と
	// Shz_LinCalc関数内で[0]番目が参照できないことから
	// 全行に同じ状態の自動計算サインを書き込むことにする
	//for(ii=0; ii<50; ii++)	{
	//	(*m_pTaxListData)->NRec.KFval[ii].KKauto &= 0x2f;	// フラグの上位を一旦クリア
	//}
//----->('23.10.02 tanaka cor)インボイス対応で追加されたD12,D13のサインも落ちていた為、修正
//	(*m_pSnHeadData)->Sn_GenericSgn &= 0x7ff;
//------
	(*m_pSnHeadData)->Sn_GenericSgn &= 0xfffff7ff;
//<-----
	// チェックON
	if(idata->data_check)	{
		//// 自身以外のチェックをOFFにする
		//if(index == FUSELChk2ex)	{
		//	diag_getdata(IDC_ICSDIAGCTRL1,FUSELChk3ex,&xdata,CIcsdiagctrl);
		//	xdata.data_check = 0;
		//	diag_setdata(IDC_ICSDIAGCTRL1,FUSELChk3ex,&xdata,CIcsdiagctrl);
		//	for(ii=0; ii<50; ii++)	{
		//		(*m_pTaxListData)->NRec.KFval[ii].KKauto |= 0x40;
		//	}
		//}
		//else if(index == FUSELChk3ex)	{
		//	diag_getdata(IDC_ICSDIAGCTRL1,FUSELChk2ex,&xdata,CIcsdiagctrl);
		//	xdata.data_check = 0;
		//	diag_setdata(IDC_ICSDIAGCTRL1,FUSELChk2ex,&xdata,CIcsdiagctrl);
		//	for(ii=0; ii<50; ii++)	{
		//		(*m_pTaxListData)->NRec.KFval[ii].KKauto |= 0x80;
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
	m_Fudousan31.OffControl(TRUE);
	for(int idx=0; idx<FKLINCNT; idx++)	{
		DispFudousan31DiagData(idx);
	}
	SetItemPositionEx();
}

// ----------------------------------------------------------------------------------
// 自動計算チェックON/OFFによる、6.24%・7.8%の入力可／不可を切り替える
// ----------------------------------------------------------------------------------
// 【引　数】	なし
// 【返送値】	なし
// ----------------------------------------------------------------------------------
void CShinFudousan31Ex::ColumnControl( void )
{
	SH_ITEMINDEX	*pIndex=NULL;
	int				cnt=0;
	int				Flg=0;
	KKVAL*			pK = &(*m_pTaxListData)->NRec.KFval[0];

	if((*m_pSnHeadData )->m_DispTabSgn & 0x1000)	pIndex = R02_Fudousan31Ex;
	else											pIndex = Fudousan31Ex;
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
			//if(colm == 1 || colm == 2)	{
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
int CShinFudousan31Ex::GetColumnKind( int pInd )
{
	int		rv=0;

	switch(pInd)	{
		case	KFUDVAL01Eex:
		case	KFUDVAL02Eex:
		case	KFUDVAL03Eex:
		case	KFUDVAL04Eex:
		case	KFUDVAL04TEex:
		case	KFUDVAL05Eex:
		case	KFUDVAL06Eex:
		case	KFUDVAL07Eex:
		case	KFUDVAL08Eex:
		case	KFUDVAL09Eex:
		case	KFUDVAL10Eex:
		case	KFUDVAL11Eex:
		case	KFUDVAL12Eex:
		case	KFUDVAL13Eex:
		case	KFUDVAL14Eex:
		case	KFUDVAL15Eex:
			rv=1;
			break;
		case	KFUDVAL01Fex:
		case	KFUDVAL02Fex:
		case	KFUDVAL03Fex:
		case	KFUDVAL04Fex:
		case	KFUDVAL04TFex:
		case	KFUDVAL05Fex:
		case	KFUDVAL06Fex:
		case	KFUDVAL07Fex:
		case	KFUDVAL08Fex:
		case	KFUDVAL09Fex:
		case	KFUDVAL10Fex:
		case	KFUDVAL11Fex:
		case	KFUDVAL12Fex:
		case	KFUDVAL13Fex:
		case	KFUDVAL14Fex:
		case	KFUDVAL15Fex:
			rv=2;
			break;
	}
	return(rv);
}
// 22-0167,22-0361 add <--

// 修正No.168690 add -->
void CShinFudousan31Ex::DiagReSetPosition(void)
{
	this->SetFocus();
}
// 修正No.168690 add <--
