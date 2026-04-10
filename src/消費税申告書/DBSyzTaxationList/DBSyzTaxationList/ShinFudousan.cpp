// ShinFudousan.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFudousan.h"
#include "ShinFudousanIdx.h"


// CShinFudousan ダイアログ

IMPLEMENT_DYNAMIC(CShinFudousan, CSyzBaseDlg)

CShinFudousan::CShinFudousan(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFudousan::IDD, pParent)
	, m_pParent(pParent)//'14.08.06
	, m_Idx(0)
	, m_Lin(0)
	, m_Clm(0)
{
	m_curwnd = -1;//'14.08.06

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;
}

CShinFudousan::~CShinFudousan()
{
}

void CShinFudousan::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Fudousan);
}


BEGIN_MESSAGE_MAP(CShinFudousan, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinFudousan メッセージ ハンドラ

BOOL CShinFudousan::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Fudousan;
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
int CShinFudousan::Init()
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
void CShinFudousan::InitDiagAttr()
{
	DIAGRAM_ATTRIBUTE data;
	long dg1[] = { FUDKNAME03, FUDKNAME12, -1 };

	for( int i=0;; i++ ){
		if( dg1[i] == -1 ){
			break;
		}
		// ank対応
		m_Fudousan.GetAttribute( (short)dg1[i], (LPUNKNOWN)&data );
		data.attr_editattr |= DIAG_EATTR_ZENHAN;//DIAG_EATTR_ZENHAN ank対応
		m_Fudousan.SetAttribute( (short)dg1[i], (LPUNKNOWN)&data, 1 );
	}

	int	cnt = 0;
	while( 1 ){
		if( Fudousan[cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Fudousan[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Fudousan[cnt].UnConnect;
			}
			else{
				Flg = Fudousan[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL1, Fudousan[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Fudousan[cnt].Connect );
		cnt++;
	}

	SetFocusRange();

	m_Fudousan.EnableDelete(0);//DELETEキー１回で削除モード設定'14.08.06

}

//-----------------------------------------------------------------------------
// 画面表示
//-----------------------------------------------------------------------------
// 引数	mode	：	
//-----------------------------------------------------------------------------
void CShinFudousan::DispList( int mode )
{
	m_Fudousan.OffControl( TRUE );
//2016.06.22 INSERT START
	(*m_Scalc)->Shz_AllCalc(2);
//2016.06.22 INSERT END
	short	i;
	for( i = 0; i < VLIN; i++ ){
		DispFudousanDiagData( i );
	}

	DIAGRAM_DATA	data;
	if( !mode ){
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, YEARID, TRUE, CIcsdiagctrl );
		data.data_disp.Format( _T("（平成%2x年分）"), (*m_pTaxListData)->YEAR );
		diag_setdata( IDC_ICSDIAGCTRL1, YEARID, &data, CIcsdiagctrl );

		PostMessage(WM_VSCROLL, SB_TOP, 0 );
		m_Idx = VMIN;
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

	diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );

//2016.06.22 INSERT START
	int edit[] = {FUDKNAME03,FUDKNAME12};
	BOOL edit_focus = FALSE;

	for( int cnt = 0; cnt < ARRAYCOUNT(edit); cnt++ ){
		Switch_KamokuEdit( IDC_ICSDIAGCTRL1, edit[cnt] );

		if( m_Fudousan.GetPosition() == edit[cnt] ){
			edit_focus = TRUE;
		}
	}

	if(( (*m_pTaxListData)->Head.KHkoke == 1 ) && ( edit_focus == TRUE )){
		this->SetFocus();
	}
//2016.06.22 INSERT END

	//SetRedrawScrollToIdx(m_Fudousan.GetPosition());
}

//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
// 引数	dlin	：	
//-----------------------------------------------------------------------------
void CShinFudousan::DispFudousanDiagData( long dlin )
{
	short	idx, i;
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

	idx = (short)(dlin*VCNT)+VMIN;
	for( i=0; i<VCNT; i++ ){
		st = 0;
		::ZeroMemory( val, sizeof(val) );
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, idx+i, TRUE, CIcsdiagctrl );
/*		switch( i ){
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
		}*/

//		st = DispJudge( i, lin );
		if( DispJudge( i, lin ) == 1 ){
			st = 1;
			switch(i){
				case 0:
					memcpy( val, pK->KKval1, 6 );
					break;
				case 1:
					memcpy( val, pK->KKval2, 6 );
					break;
				case 2:
					memcpy( val, pK->KKval3, 6 );
					break;
				default:
					break;
			}
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
//2016.06.22 UPDATE START
			//switch(lin+1){
			//	case 3:	
			//		idx = FUDKNAME03;	
			//		break;
			//	case 12:
			//		idx = FUDKNAME12;
			//		break;
			//}
			//memcpy( buf, pK->KKnam, sizeof(pK->KKnam));

			CString tmp;
			int prf_no = 0;
			switch(lin+1){
				case 3:	
					idx = FUDKNAME03;
					prf_no = 3;
					break;
				case 12:
					idx = FUDKNAME12;
					prf_no = 12;
					break;
			}

//-- '16.12.07 --
//			if(((*m_pTaxListData)->Head.KHkoke == 1 )&&( prf_no != 0 )){
//---------------
			if( (*m_pTaxListData)->IsLinkKjprfname() && (prf_no!=0) ){
//---------------
				if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)){ //金額が全て０なら表示しない
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
int CShinFudousan::GetDiagData( long index, short leng, DIAGRAM_DATA *pData, int mode )
{
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
	for( int i=0; i<VLIN; i++ ){
		DispFudousanDiagData( i );
	}
	return 0;
}

//-----------------------------------------------------------------------------
// カーソル移動
//-----------------------------------------------------------------------------
// 引数	index	：
//		flg		：
//		nChar	：
//-----------------------------------------------------------------------------
void CShinFudousan::Diag1_Edit( short index, UINT flg, short nChar )
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
			if( m_Clm == 1 ){
				flg = REPLY_STAY;
				switch( m_Lin+1 ){	// 項目名へ移動
					case 2:	
						xpos = FUDKNAME03;
						break;
					case 12:
						xpos = FUDKNAME12;
						break;
					case 14:
						if(nChar == VK_RIGHT){
							xpos = VMIN;
						}
						break;
					default:
						flg = REPLY_NEXT;
						break;
				}
			}
			else if( m_Clm == -1 ){	// 項目名から移動
				flg = REPLY_STAY;
				xpos = (m_Lin*VCNT)+VMIN;
			}
			else{
				flg = REPLY_NEXT;
			}
			break;
		case VK_LEFT:
			if( !m_Clm ){
				flg = REPLY_STAY;
				switch( m_Lin+1 ){	// 項目名へ移動
					case 1:
						xpos = VLST;
						break;
					case 3:	
						xpos = FUDKNAME03;
						break;
					case 13:
						xpos = FUDKNAME12;	
						break;
					default:	
						flg = REPLY_BACK;
						break;
				}
			}
			else if( m_Clm == -1 ){
				flg = REPLY_STAY;
				xpos = ((m_Lin-1)*VCNT)+VMIN+1;
			}
			else{
				flg = REPLY_BACK;
			}
			break;
		case VK_UP:
			flg = REPLY_STAY;
			if( (index>=FUDKNAME03) && (index<=FUDKNAME12) ){
				;	// 項目名
			}
			else{
				if( !m_Clm ){	// A
					switch( m_Lin+1 ){
						case 1:	
							break;
						case 5: 
							xpos -= 6;	
							break;
						default:
							xpos -= 3;	
							break;
					}
				}
				else{			// B
					switch( m_Lin+1 ){
						case 1:	
							break;
						case 5:	
							xpos -= 6;
							break;
						case 11:
							xpos -= 15;
							break;
						default:
							xpos -= 3;
							break;
					}
				}
			}
			break;
		case VK_DOWN:
			flg = REPLY_STAY;
			if( (index>=FUDKNAME03) && (index<=FUDKNAME12) ){
				;	// 項目名
			}
			else{
				if( !m_Clm ){	// A
					switch( m_Lin+1 ){
						case 3:
							xpos += 6;	
							break;
						case 14:	
							break;
						default:	
							xpos += 3;	
							break;
					}
				}
				else{			// B
					switch( m_Lin+1 ){
						case 3:
							xpos += 6;	
							break;
						case 6:	
							xpos += 15;
							break;
						case 14:
							break;
						default:	
							xpos += 3;
							break;
					}
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
BEGIN_EVENTSINK_MAP(CShinFudousan, CSyzBaseDlg)
	ON_EVENT(CShinFudousan, IDC_ICSDIAGCTRL1, 1, CShinFudousan::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinFudousan, IDC_ICSDIAGCTRL1, 2, CShinFudousan::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinFudousan, IDC_ICSDIAGCTRL1, 3, CShinFudousan::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFudousan::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//'14.08.06
	/*
	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL1, index, &rectB, CIcsdiagctrl);
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
	*/

	m_Fudousan.OffControl( FALSE );

	m_Idx = index;
	if( (m_Idx>=VMIN) && (m_Idx<=VMAX) ){
		m_Lin = (index-VMIN)/VCNT;
		m_Clm = (index-VMIN)%VCNT;
	}
	else if( m_Idx == FUDKNAME03 ){
		m_Lin = 2;	m_Clm = -1;
	}
	else if( m_Idx <= FUDKNAME12 ){
		m_Lin = 12;	m_Clm = -1;
	}
	m_curwnd = IDC_ICSDIAGCTRL1;//'14.08.01

}

void CShinFudousan::EditOFFIcsdiagctrl1(short index)
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

void CShinFudousan::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
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
//		if(index == FUDVAL15C){
//			m_Fudousan.SetPrevPosition();
//		}
//		else{
////			pos = UpDownFocusMoveCheck(index , 1);
			pos = FocusMoveCheck(index , 1);
			m_Fudousan.SetPosition(pos);	
//		}
	}
	if( wTerm==VK_DOWN ){
//		if(index == FUDVAL15C){
//			m_Fudousan.SetNextPosition();
//		}
//		else{
////			pos = UpDownFocusMoveCheck(index , 0);
			pos = FocusMoveCheck(index , 0);
			m_Fudousan.SetPosition(pos);
//		}
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
//		m_Fudousan.SetNextPosition();
		pos = FocusMoveCheck(index , 2);
		m_Fudousan.SetPosition(pos);
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
//		m_Fudousan.SetPrevPosition();
		pos = FocusMoveCheck(index , 3);
		m_Fudousan.SetPosition(pos);
	}

	//------->DELETEキー対応'14.08.06
	if( wTerm == VK_DELETE){
		GetDiagData( index, length, &xdata, -1 );
	}


	/*
	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;

	UINT	flg = 0;

	idata = (DIAGRAM_DATA *)data;
	m_Util.MoveDiagData( &xdata, idata );
	xdata.data_imgdata = NULL;
	*/

	/*
	switch( nChar ){
		case VK_RETURN:
			m_Fudousan.OffControl( TRUE );
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
			m_Fudousan.SetNextPosition();
		}
		// 前項目へ
		if( (nChar==VK_TAB) || (nChar==VK_LEFT) || (nChar==VK_F2) ){
			m_Fudousan.SetPrevPosition();
		}
	}

	//------->DELETEキー対応'14.08.06
//	if( wTerm == VK_DELETE){
//		GetDiagData( index, length, &xdata, -1 );
//	}
*/
	SetRedrawScrollToIdx(m_Fudousan.GetPosition());

/*	int nowpos = m_Fudousan.GetPosition();	// 移動先または自身のポジション

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
//int CShinFudousan::UpDownFocusMoveCheck(int Index , int Move)
int CShinFudousan::FocusMoveCheck(int Index , int Move)
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
		for(i = 0 ; i < FCS_SHINFUDOUSANCNT ; i++){
			if(Index == FCS_ShinFudousan[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinFudousan[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SHINFUDOUSANCNT ; i++){
			if(Index == FCS_ShinFudousan_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_ShinFudousan_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SHINFUDOUSANCNT ; j++){
			m_Fudousan.GetAttribute(FCS_ShinFudousan[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinFudousan[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Fudousan.GetAttribute(FCS_ShinFudousan[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinFudousan[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SHINFUDOUSANCNT ; j++){
			m_Fudousan.GetAttribute(FCS_ShinFudousan_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinFudousan_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Fudousan.GetAttribute(FCS_ShinFudousan_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_ShinFudousan_LR[j].IDNo;
				break;
			}
		}
	}

	return idx;
}
//<-----------------------------------------

void CShinFudousan::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SHINFUDOUSANCNT; idx++ ){
		m_Fudousan.GetAttribute(FCS_ShinFudousan[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_ShinFudousan[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SHINFUDOUSANCNT; idx++ ){
		m_Fudousan.GetAttribute(FCS_ShinFudousan_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_ShinFudousan_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINFUDOUSANCNT-1; idx > -1 ; idx-- ){
		m_Fudousan.GetAttribute(FCS_ShinFudousan[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_ShinFudousan[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SHINFUDOUSANCNT-1; idx > -1 ; idx-- ){
		m_Fudousan.GetAttribute(FCS_ShinFudousan_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_ShinFudousan_LR[idx].IDNo;
			break;
		}
	}
}

//'14.08.06
BOOL CShinFudousan::PreTranslateMessage(MSG* pMsg)
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
void CShinFudousan::SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo )
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
int CShinFudousan::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j, lin, max;
	char	cnm[128], buf[128], CVAL[6];
	int		len;
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

// 収入金額
// 決算額 A
	lin = 0;	max = 4;
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 0, j+lin) == 1 ){
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
		if( DispJudge( 1, j+lin) == 1 ){
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
		if( DispJudge( 2, j+lin) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
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
//	pK = &(*m_pTaxListData)->NRec.KFval[2];
	pK = &(*m_pTaxListData)->NRec.KFval[KFUDLINE3];
	::ZeroMemory( buf, sizeof(buf) );
//-- '16.12.07 --
//	len = (int)strlen( pK->KKnam );
//	memmove( pBpMv, pK->KKnam , len );
//---------------
	if( isLinkKjprf ){
		tmpStr.Empty();
		if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)){ //金額が全て０なら表示しない
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

//	pK = &(*m_pTaxListData)->NRec.KFval[11];
	pK = &(*m_pTaxListData)->NRec.KFval[KFUDLINE12];
	::ZeroMemory( buf, sizeof(buf) );
//-- '16.12.07 --
//	len = (int)strlen( pK->KKnam );
//	memmove( pBpMv, pK->KKnam , len );
//---------------
	if( isLinkKjprf ){
		tmpStr.Empty();
		if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)){ //金額が全て０なら表示しない
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
int CShinFudousan::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
{

	int		i, j , len , lin , max;
	char	buf[64];
	KKVAL*	pK;

	i = 0;
	memset( pBp, '\0', Msiz );
//	BP1 = BP;	
	char	*pBpMv = pBp;

// 決算額 A
	lin = 4;	max = 14;
	for( j=0; (j+lin)<max; j++ ){
		pK = &(*m_pTaxListData)->NRec.KFval[j+lin];
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 0, j+lin) == 1 ){
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
		if( DispJudge( 1, j+lin) == 1 ){
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
		if( DispJudge( 2, j+lin) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

//差引金額
// 決算額 A
//		pK = &(*m_pTaxListData)->NRec.KFval[14];
		pK = &(*m_pTaxListData)->NRec.KFval[KFUDLINE15];
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 0, KFUDLINE15) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[0] = pBpMv;
		pBpMv += ( len+1 );
		i++;

// Aのうち課税取引にならないもの B
//		pK = &(*m_pTaxListData)->NRec.KFval[14];
		pK = &(*m_pTaxListData)->NRec.KFval[KFUDLINE15];
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 1, KFUDLINE15) == 1 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[0] = pBpMv;
		pBpMv += ( len+1 );
		i++;

// 課税取引額 C
//		pK = &(*m_pTaxListData)->NRec.KFval[14];
		pK = &(*m_pTaxListData)->NRec.KFval[KFUDLINE15];
		::ZeroMemory( buf, sizeof(buf) );
		if( DispJudge( 2, KFUDLINE15) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[0] = pBpMv;
		pBpMv += ( len+1 );
		i++;


	return( _xprn( pno, fno, PF, 0, pDC, 0 ));
}



//-----------------------------------------------------------------------------
// 初期ポジションセット '14.09.01
//-----------------------------------------------------------------------------
void CShinFudousan::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
		if( (index = m_Fudousan.GetPosition()) != -1) {
			m_Fudousan.SetPosition(index);
			m_Fudousan.SetFocus();
			return;
		}
	}

	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Fudousan.SetPosition(FUDVAL01A);
	}
	else{
		m_Fudousan.SetPosition(FUDKNAME03);
	}
	SetRedrawScroll( 0 );

}

int CShinFudousan::DispJudge( int clm, int line )
{
	int st = 0;
	int j = 0;
	
	switch( clm ){
		case 0:	// 決算額
			switch( line ){
				case 14:
				case 3:	
					for( j=0; j<3; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
							st = 1;
							break;
						}
					}
					if( line !=14 ){
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
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval1) ){
						st = 1;
					}
					break;
			}
			break;
		case 1:	// 課税取引にならないもの
			switch( line ){
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
					if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval2) ){
						st = 1;
					}
					break;
			}
			break;
		case 2:	// 課税取引金額
			switch( line ){
				case 5:
				case 7:
				case 8: 
				case 14:
					break;
				case 3:	
					for( j=0; j<3; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
							st = 1;
							break;
						}
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval2) ){
							st = 1;
							break;
						}
					}
				case 13:
					for( j=4; j<13; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
							st = 1;
							break;
						}
						if( j == 6 ){
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
	}

	return st;
}

// 手入力サインのクリア
int CShinFudousan::ClearManualInputSign()
{
	int rt = 0;

	for( int idx = 0; idx < 50; idx++ ){
		(*m_pTaxListData)->NRec.KFval[idx].KKauto = 0x00;
	}

	return rt;
}