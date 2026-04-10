// ShinFudousan31Ex.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFudousan31Ex2.h"
//	#include "ShinFudousan31Ex2Idx.h"
#include "afxdialogex.h"


//2023.7.7～　戎居

// CShinFudousan31Ex2 ダイアログ

IMPLEMENT_DYNAMIC(CShinFudousan31Ex2, CSyzBaseDlg)

CShinFudousan31Ex2::CShinFudousan31Ex2(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFudousan31Ex2::IDD, pParent)
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
	setFCSArray();

}

CShinFudousan31Ex2::~CShinFudousan31Ex2()
{
}

void CShinFudousan31Ex2::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Fudousan31);
}


BEGIN_MESSAGE_MAP(CShinFudousan31Ex2, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinFudousan31Ex2 メッセージ ハンドラ

BOOL CShinFudousan31Ex2::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Fudousan31;
	//底を合わせるためのインデックス
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
int CShinFudousan31Ex2::Init()
{
	InitDiagAttr();

	DispList(0);

	SetFocusRange();
	SetItemPosition();	// フォーカスセット

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
void CShinFudousan31Ex2::InitDiagAttr()
{
	DIAGRAM_ATTRIBUTE data;
	long dg1[] = { FudName03, FudName12, -1 };

	for( int i=0;; i++ ){
		if( dg1[i] == -1 ){
			break;
		}
		// ank対応
		m_Fudousan31.GetAttribute( (short)dg1[i], (LPUNKNOWN)&data );
		data.attr_editattr |= DIAG_EATTR_ZENHAN;
		m_Fudousan31.SetAttribute( (short)dg1[i], (LPUNKNOWN)&data, 1 );
	}

	const SH_ITEMINDEX *pIndex = NULL;
	//if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) { R2との切り替え
		pIndex = idxAttr;

		//int KE = jigVal01F; 和田さんの　どういう意味？
	int	cnt = 0;
	while( 1 ){
		if ( ( pIndex + cnt )->Index == 0 ){
			break;
		}
		int	Flg = 0;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4 & 0x01 ){
			Flg = ( pIndex + cnt )->AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ){
				Flg = ( pIndex + cnt )->UnConnect;
			}
			else{
				Flg = ( pIndex + cnt )->Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL1, ( pIndex + cnt )->Index, Flg, (*m_pSnHeadData)->Sn_Sign4, ( pIndex + cnt )->Connect );
		cnt++;
	}

	//InitDiagonalLines();

	SetFocusRange();

	m_Fudousan31.EnableDelete(0);//DELETEキー１回で削除モード設定

	
}

//-----------------------------------------------------------------------------
// 画面表示
//-----------------------------------------------------------------------------
// 引数	mode	：	
//-----------------------------------------------------------------------------
void CShinFudousan31Ex2::DispList( int mode )
{
	m_Fudousan31.OffControl( TRUE );

	(*m_Scalc)->Shz_31AllCalc(2);

	short idx = 0;
	for (idx = 0; idx < rowSize; idx++) {
		DispFudousan31DiagData(idx);
	}

	DIAGRAM_DATA data = {};

	if( !mode ){
		m_Util.DiagOcxIniz( &data );
		diag_clear( IDC_ICSDIAGCTRL1, idxYear, TRUE, CIcsdiagctrl );
		data.data_disp.Format( _T("（令和%2x年分）"), (*m_pTaxListData)->YEAR );
		diag_setdata( IDC_ICSDIAGCTRL1, idxYear, &data, CIcsdiagctrl );

		PostMessage(WM_VSCROLL, SB_TOP, 0 );
		m_Idx = idxEditMin;
	}

	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, idxZkbn, TRUE, CIcsdiagctrl );
	data.data_disp = _T("経理方式：");
	if( (*m_pTaxListData)->Head.KHstax&0x01 ){
		data.data_disp += _T("税抜き");
	}
	else{
		data.data_disp += _T("税込み");
	}

	diag_setdata( IDC_ICSDIAGCTRL1, idxZkbn, &data, CIcsdiagctrl );

	int edit[] = { FudName03, FudName12 };
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

	//2023.7 add
	diag_modify(IDC_ICSDIAGCTRL1, autoCalcChk2, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl);
	diag_modify(IDC_ICSDIAGCTRL1, autoCalcChk3, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl);

}

//-----------------------------------------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------------------------------------
// 引数	dlin	：	: ダイアグラム上の行数
//-----------------------------------------------------------------------------
void CShinFudousan31Ex2::DispFudousan31DiagData( long dlin )
{
	short idx = 0,
		i = 0;
	long  lin = 0;

	//データ保存ポジションセット
	lin = dataPos[dlin];

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KFval[lin];
	char	val[6] = { 0 };
	DIAGRAM_DATA	data;
	int		st = 0;

	//idx = (short)(dlin * FKWIDECNTex) + FKVMINex;
// インボイス_23/07/24_higuchi del -->
	//idx = (short)idxMap[dlin][clmA]; //2023/07/11
// インボイス_23/07/24_higuchi del <--
	

	for (i = 0; i < clmSize; i++) {
// インボイス_23/07/24_higuchi add -->
		idx = ( short )idxMap[dlin][i];
		if(idx < 0)	continue;
// インボイス_23/07/24_higuchi add <--
		st = 0;
		::ZeroMemory(val, sizeof(val));
// インボイス_23/07/24_higuchi cor -->
		//diag_clear(IDC_ICSDIAGCTRL1, idx + i, TRUE, CIcsdiagctrl );
// -----------------------------------
		diag_clear(IDC_ICSDIAGCTRL1, idx, TRUE, CIcsdiagctrl );
// インボイス_23/07/24_higuchi cor <--
		m_Util.DiagOcxIniz(&data);

		switch (i) {
		case clmA:		// 決算額 A
			memcpy(val, pK->KKval1, 6);
			break;
		case clmB:		// Aのうち課税取引にならないもの B
			memcpy(val, pK->KKval2, 6);
			break;
		case clmC:		// 課税取引金額 C ( A - B )
			memcpy(val, pK->KKval3, 6);
			break;
		case clmD:		// うち税率6.24%適用分 D
			memcpy(val, pK->KKval5, 6);
			break;
		case clmE:		// うち税率７．８％適用分　E
// インボイス_23/07/21_higuchi add -->
			memcpy(val, pK->KKval7, 6);
// インボイス_23/07/21_higuchi add <--
			break;
		case clmF:		// うち税率7.8%適用分 F
			memcpy(val, pK->KKval6, 6);
			break;
		case clmG:		// うち税率７．８％適用分　G
// インボイス_23/07/21_higuchi add -->
			memcpy(val, pK->KKval8, 6);
// インボイス_23/07/21_higuchi add <--
			break;
		default:
			break;
		}

		if (DispJudge(i, lin) == 1) {
			memcpy(data.data_val, val, 6);
// インボイス_23/07/24_higuchi cor -->
			//diag_setdata(IDC_ICSDIAGCTRL1, idx + i, &data, CIcsdiagctrl);
// -----------------------------------
			diag_setdata(IDC_ICSDIAGCTRL1, idx, &data, CIcsdiagctrl);
// インボイス_23/07/24_higuchi cor <--
		}
	}

	// 項目名
	char	buf[128] = { 0 };

	if(dlin == FudDiagLine3 || dlin == FudDiagLine12){//項目名入力可所　③　⑫　の場合

		lin = dataPos[dlin];
		//pK = &(*m_pTaxListData)->NRec.KJval[lin];	// 230725Aw del
		pK = &(*m_pTaxListData)->NRec.KFval[lin];	// 230725Aw add
		
		if( !(pK->KKsign&0x80) ){
			m_Util.DiagOcxIniz( &data );
			::ZeroMemory( buf, sizeof(buf) );

			CString tmp;
			int prf_no = 0;
			switch( dlin ){
				case FudDiagLine3:	//③
					idx = FudName03;
					prf_no = 3;
					break;
				case FudDiagLine12://⑫
					//idx = FudName03;	// 230727Aw del
					idx = FudName12;	// 230727Aw add
					prf_no = 12;
					break;
			}

			if( (*m_pTaxListData)->IsLinkKjprfname() && (prf_no!=0) ){
// インボイス_23/07/21_higuchi cor -->
				//if((m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||(m_pArith->l_test(pK->KKval3)!=0)||(m_pArith->l_test(pK->KKval4)!=0)||(m_pArith->l_test(pK->KKval5)!=0)|| (m_pArith->l_test(pK->KKval6) != 0)){ //金額が全て０なら表示しない
// -----------------------------------
				if( (m_pArith->l_test(pK->KKval1)!=0)||(m_pArith->l_test(pK->KKval2)!=0)||
					(m_pArith->l_test(pK->KKval3)!=0)||(m_pArith->l_test(pK->KKval4)!=0)||
					(m_pArith->l_test(pK->KKval5)!=0)||(m_pArith->l_test(pK->KKval6)!=0)||
					(m_pArith->l_test(pK->KKval7)!=0)||(m_pArith->l_test(pK->KKval8)!=0) ){
					//金額が全て０なら表示しない
// インボイス_23/07/21_higuchi cor <--
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
int CShinFudousan31Ex2::GetDiagData( long index, short leng, DIAGRAM_DATA *pData, int mode )
{
	if(index < 0 || index == autoCalcChk2 || index == autoCalcChk3) {
		return 0;
	}

	char dbuf[128] = { 0 };
	char *buff = NULL;
	int	len = 0;

	len = pData->data_edit.GetLength();
	memset(dbuf, '\0', sizeof(dbuf));
	buff = (char *)((LPCTSTR)(pData->data_edit));
	memmove( dbuf, buff, len );


	//データ保存ポジションセット
	long	lin = 0;
	lin = dataPos[m_Lin];

	KKVAL*	pK = &(*m_pTaxListData)->NRec.KFval[lin];

	if( !mode ){
		switch( m_Clm ){
			case clmA:		// 決算額	A
				if( leng ){
					memcpy( pK->KKval1, pData->data_val, 6 );
				}
				break;

			case clmB:		// Aのうち課税取引にならないもの	B
				if( leng ){
					memcpy( pK->KKval2, pData->data_val, 6 );
				}
				break;

			case clmC:		// 課税取引金額　C（A－B）
				if( leng ){
					memcpy( pK->KKval3, pData->data_val, 6 );
				}
				break;
			case clmD:		// うち税率6.24%適用分 D
				if (leng) {
					memcpy(pK->KKval5, pData->data_val, 6);
				}
				break;
			case clmE:
// インボイス_23/07/21_higuchi add -->
				if (leng) {
					memcpy(pK->KKval7, pData->data_val, 6);
				}
// インボイス_23/07/21_higuchi add <--
				break;
			case clmF:		// うち税率7.8%適用分 F
				if (leng) {
					memcpy(pK->KKval6, pData->data_val, 6);
				}
				break;
			case clmG:
// インボイス_23/07/21_higuchi add -->
				if (leng) {
					memcpy(pK->KKval8, pData->data_val, 6);
				}
// インボイス_23/07/21_higuchi add <--
				break;
			default:	// 項目名
				memcpy(pK->KKnam, dbuf, sizeof(pK->KKnam));
				break;
		}
	}
	else {
		switch (m_Clm) {
		case clmA:		// 決算額 A
			::ZeroMemory(pK->KKval1, 6);
			break;
		case clmB:		// Aのうち課税取引にならないもの B
			::ZeroMemory(pK->KKval2, 6);
			break;
		case clmC:		// 課税取引金額 C（ A － B ）
			::ZeroMemory(pK->KKval3, 6);
			break;
		case clmD:		// うち税率6.24%適用分 D
			::ZeroMemory(pK->KKval5, 6);
			break;
		case clmF:		// うち税率7.8%適用分 F
			::ZeroMemory(pK->KKval6, 6);
			break;
		case clmE:
// インボイス_23/07/21_higuchi add -->
			::ZeroMemory(pK->KKval7, 6);
			break;
// インボイス_23/07/21_higuchi add <--
		case clmG:
// インボイス_23/07/21_higuchi add -->
			::ZeroMemory(pK->KKval8, 6);
// インボイス_23/07/21_higuchi add <--
			break;
		default:	// 項目名
			::ZeroMemory(pK->KKnam, sizeof(pK->KKnam));
			break;
		}
	}

	// 集計
	(*m_Scalc)->Shz_31AllCalc(2);

	for (int idx = 0; idx < rowSize; idx++) {
		DispFudousan31DiagData( idx );
	}
	return 0;

}


BEGIN_EVENTSINK_MAP(CShinFudousan31Ex2, CSyzBaseDlg)
ON_EVENT(CShinFudousan31Ex2, IDC_ICSDIAGCTRL1, 1, CShinFudousan31Ex2::EditONIcsdiagctrl1, VTS_I2)
ON_EVENT(CShinFudousan31Ex2, IDC_ICSDIAGCTRL1, 2, CShinFudousan31Ex2::EditOFFIcsdiagctrl1, VTS_I2)
ON_EVENT(CShinFudousan31Ex2, IDC_ICSDIAGCTRL1, 3, CShinFudousan31Ex2::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
//ON_EVENT(CShinFudousan31Ex2, IDC_ICSDIAGCTRL1, 9, CShinFudousan31Ex2::CheckButtonIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFudousan31Ex2::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	//--->2023.7del
	//m_Fudousan31.OffControl( FALSE );
	//m_Idx = index;
	//if( (m_Idx >= FKVMINex) && (m_Idx <= FKVMAXex) ){
	//	m_Lin = (index - FKVMINex ) / FKWIDECNTex;
	//	m_Clm = (index - FKVMINex ) % FKWIDECNTex;
	//}
	//else if( m_Idx == KFUDKNAME_03 ){
	//	m_Lin = FudDiagLine3;
	//	m_Clm = -1;
	//}
	//else if( m_Idx == KFUDKNAME_12 ){
	//	m_Lin = FudDiagLine12 + 1;//変換等対価の行分＋１する
	//	m_Clm = -1;
	//}
	//m_curwnd = IDC_ICSDIAGCTRL1;
	//<---del

	m_Fudousan31.OffControl(FALSE);

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

	else if( m_Idx == FudName03){
		m_Lin = FudDiagLine3;
		m_Clm = -1;
	}
	else if( m_Idx == FudName12){
		//m_Lin = FudDiagLine12 + 1;//変換等対価の行分＋１する
		m_Lin = FudDiagLine12;

		m_Clm = -1;
	}

	m_curwnd = IDC_ICSDIAGCTRL1;

}

void CShinFudousan31Ex2::EditOFFIcsdiagctrl1(short index)
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

void CShinFudousan31Ex2::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}
	DIAGRAM_DATA	*idata = NULL;
	DIAGRAM_DATA	xdata;

	UINT	flg = 0;

	idata = (DIAGRAM_DATA *)data;
	m_Util.DiagOcxIniz( &xdata );
	m_Util.MoveDiagData( &xdata, idata );

	//フォーカス移動処理対応
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
		pos = FocusMoveCheck(index , 1);
		m_Fudousan31.SetPosition(pos);	
	}
	if( wTerm==VK_DOWN ){
		pos = FocusMoveCheck(index , 0);
		m_Fudousan31.SetPosition(pos);
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
		pos = FocusMoveCheck(index , 2);
		m_Fudousan31.SetPosition(pos);	
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		pos = FocusMoveCheck(index , 3);
		m_Fudousan31.SetPosition(pos);	
	}

	//	DELETEキー対応
	if( wTerm == VK_DELETE){
		GetDiagData( index, length, &xdata, -1 );
	}

	SetRedrawScrollToIdx(m_Fudousan31.GetPosition(), IDC_ICSDIAGCTRL1);
}



//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
// 引数
//		Index	:		ダイアグラム内インデックス
//		Move	:		移動方向
//----------------------------------------------------
// 返送値
//		次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinFudousan31Ex2::FocusMoveCheck(int Index , int Move)
{
	int idx = 0;
	int i = 0 , j = 0;
	BOOL end_flg = FALSE;

	DIAGRAM_ATTRIBUTE	DA;

	int cnt = 0;
	FCSUPDOWN_INFO *pInfo = NULL,
				   *pInfo_LR = NULL;

	cnt = FCSitemMax;
	pInfo = FCS_UD;
	pInfo_LR = FCS_LR;

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
		for ( i = 0; i < cnt; i++ ) {
			if ( Index == ( pInfo + i )->IDNo ) {
				break;
			}
		}
		idx = ( pInfo + i )->IDNo;
	}else if(( Move == 2 )||( Move == 3 )){
		for ( i = 0; i < cnt; i++ ) {
			if ( Index == ( pInfo_LR + i )->IDNo ) {
				break;
			}
		}
		idx = ( pInfo_LR + i )->IDNo;
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for ( j = i + 1; j < cnt; j++ ) {
			//m_Fudousan31.GetAttribute ( ( pInfo + i )->IDNo, ( LPUNKNOWN )&DA );	// 230801Aw del
			m_Fudousan31.GetAttribute ( ( pInfo + j )->IDNo, ( LPUNKNOWN )&DA );	// 230801Aw add
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo + j )->IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for ( j = i - 1; j > -1; j-- ) {
			//m_Fudousan31.GetAttribute ( ( pInfo + i )->IDNo, ( LPUNKNOWN )&DA );	// 230801Aw del
			m_Fudousan31.GetAttribute ( ( pInfo + j )->IDNo, ( LPUNKNOWN )&DA );	// 230801Aw add
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo + j )->IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for ( j = i + 1; j < cnt; j++ ) {
			m_Fudousan31.GetAttribute ( ( pInfo_LR + j )->IDNo, ( LPUNKNOWN )&DA );
			if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
				idx = ( pInfo_LR + j )->IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
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

//=====================================
// ポジションの末端設定
//=====================================
void CShinFudousan31Ex2::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA = {};

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	int cnt = 0;
	FCSUPDOWN_INFO *pInfo = NULL,
				   *pInfo_LR = NULL;

	cnt = FCSitemMax;
	pInfo = FCS_UD;
	pInfo_LR = FCS_LR;

	//上
	for ( int idx = 0; idx < cnt; idx++ ) {
		m_Fudousan31.GetAttribute ( ( pInfo + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsUpEnd = ( pInfo + idx )->IDNo;
			break;
		}
	}
	//左
	for ( int idx = 0; idx < cnt; idx++ ) {
		m_Fudousan31.GetAttribute ( ( pInfo_LR + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsLeftEnd = ( pInfo_LR + idx )->IDNo;
			break;
		}
	}
	//下
	for ( int idx = cnt -1; idx > -1; idx-- ) {
		m_Fudousan31.GetAttribute ( ( pInfo + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsDownEnd = ( pInfo + idx )->IDNo;
			break;
		}
	}
	//右
	for ( int idx = cnt -1; idx > -1; idx-- ) {
		m_Fudousan31.GetAttribute ( ( pInfo_LR + idx )->IDNo, ( LPUNKNOWN )&DA );
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsRightEnd = ( pInfo_LR + idx )->IDNo;
			break;
		}
	}
}

BOOL CShinFudousan31Ex2::PreTranslateMessage(MSG* pMsg)
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

#define		Msiz	512*20
//-----------------------------------------------------------------------------
// 印刷情報のセット
//-----------------------------------------------------------------------------
// 引数	pVprn	：	出力クラス
//		pPset	：	印刷情報
//-----------------------------------------------------------------------------
void CShinFudousan31Ex2::SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo )
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
int CShinFudousan31Ex2::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
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
	::ZeroMemory( CVAL, sizeof(CVAL) );

// 会社名
	::ZeroMemory( buf, sizeof(buf) );
	::ZeroMemory( cnm, sizeof(cnm) );

	if( m_pPset->psSigCoprn&0x2 ){
		// 顧問先
		if( m_PrintCmInfo->OPL ){
			sprintf_s( buf, sizeof( buf ), _T(" %s"), m_PrintCmInfo->CopNam );
		}
		else{
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
// 印刷2
//-----------------------------------------------------------------------------
// 引数	pno		：	出力指定表No.
//		fno		：	フィールドNo.
//		pDC		：	デバイスコンテキスト
//		pBp		：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFudousan31Ex2::PrintHyo2(int pno, int fno, CDC *pDC, char *pBp)
{

	int	i = 0,
		j = 0;

	int lin = 0,
		max = 0;

	int len = 0,
		st = 0;

	char buf[128] = { 0 };

	KKVAL*	pK;
	KKVAL*	pJ = (*m_pTaxListData)->NRec.KFval;

	// 初期化
	memset(PF, '\0', sizeof(PF));


	i = 0;
	memset(pBp, '\0', Msiz);
	char	*pBpMv = pBp;
	::ZeroMemory(buf, sizeof(buf));

	// ①～④　収入金額
	lin = FudDataLine1;
	max = FudDataLine4 + 1;

	for (int clm = clmA; clm < clmSize; clm++) {
		for (j = 0; j + lin < max; j++) {
			pK = &(*m_pTaxListData)->NRec.KFval[j + lin];
			st = 0;
			::ZeroMemory(buf, sizeof(buf));

			st = DispJudge(clm, j + lin);

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
				case clmF:  // うち標準税率7.8%適用分  F
					m_pArith->l_print(buf, pK->KKval6, FMTX130);
					break;
// インボイス_23/07/21_higuchi cor -->
				//case clmE: 入力可能部分無し
				//case clmG: 入力可能部分無し
// -----------------------------------
				case clmE:  // うち軽減税率6.24%適用分 E
					m_pArith->l_print(buf, pK->KKval7, FMTX130);
					break;
				case clmG:  // うち標準税率7.8%適用分  G
					m_pArith->l_print(buf, pK->KKval8, FMTX130);
					break;
// インボイス_23/07/21_higuchi cor <--
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


	// 項目名 3,12
	CString	tmpStr;
	bool isLinkKjprf = (*m_pTaxListData)->IsLinkKjprfname();

	pK = &(*m_pTaxListData)->NRec.KFval[FudDataLine3];
	::ZeroMemory(buf, sizeof(buf));

	if (isLinkKjprf) {
		tmpStr.Empty();
// インボイス_23/07/21_higuchi cor -->
		//if ((m_pArith->l_test(pK->KKval1) != 0) || (m_pArith->l_test(pK->KKval2) != 0) || (m_pArith->l_test(pK->KKval3) != 0) || (m_pArith->l_test(pK->KKval4) != 0) || (m_pArith->l_test(pK->KKval5) != 0) || (m_pArith->l_test(pK->KKval6) != 0)) { //金額が全て０なら表示しない
// -----------------------------------
		if ( (m_pArith->l_test(pK->KKval1) != 0) || (m_pArith->l_test(pK->KKval2) != 0) || 
			 (m_pArith->l_test(pK->KKval3) != 0) || (m_pArith->l_test(pK->KKval4) != 0) || 
			 (m_pArith->l_test(pK->KKval5) != 0) || (m_pArith->l_test(pK->KKval6) != 0) || 
			 (m_pArith->l_test(pK->KKval7) != 0) || (m_pArith->l_test(pK->KKval8) != 0) ) {
			//金額が全て０なら表示しない
// インボイス_23/07/21_higuchi cor <--
			//(*m_pTaxListData)->GetKoketsuMeisyo(m_pZmsub, ID_ICSSH_KJ_FUDOUSAN, (FudDataLine1), tmpStr);	// 230725Aw del
			(*m_pTaxListData)->GetKoketsuMeisyo(m_pZmsub, ID_ICSSH_KJ_FUDOUSAN, (FudDataLine3+1), tmpStr);	// 230725Aw add
		}
		len = tmpStr.GetLength();
		memmove(pBpMv, tmpStr, len);
	}
	else {
		len = (int)strlen(pK->KKnam);
		memmove(pBpMv, pK->KKnam, len);
	}

	PF[i].PITM[0] = pBpMv;
	pBpMv += (len + 1);
	i++;

	pK = &(*m_pTaxListData)->NRec.KFval[FudDataLine12];
	::ZeroMemory(buf, sizeof(buf));

	if (isLinkKjprf) {
		tmpStr.Empty();
// インボイス_23/07/21_higuchi cor -->
		//if ((m_pArith->l_test(pK->KKval1) != 0) || (m_pArith->l_test(pK->KKval2) != 0) || (m_pArith->l_test(pK->KKval3) != 0) || (m_pArith->l_test(pK->KKval4) != 0) || (m_pArith->l_test(pK->KKval5) != 0) || (m_pArith->l_test(pK->KKval6) != 0)) { //金額が全て０なら表示しない
// -----------------------------------
		if ((m_pArith->l_test(pK->KKval1) != 0) || (m_pArith->l_test(pK->KKval2) != 0) || 
			(m_pArith->l_test(pK->KKval3) != 0) || (m_pArith->l_test(pK->KKval4) != 0) || 
			(m_pArith->l_test(pK->KKval5) != 0) || (m_pArith->l_test(pK->KKval6) != 0) || 
			(m_pArith->l_test(pK->KKval7) != 0) || (m_pArith->l_test(pK->KKval8) != 0)) {
			//金額が全て０なら表示しない
// インボイス_23/07/21_higuchi cor <--
			(*m_pTaxListData)->GetKoketsuMeisyo(m_pZmsub, ID_ICSSH_KJ_FUDOUSAN, (FudDataLine12 + 1), tmpStr);
		}
		len = tmpStr.GetLength();
		memmove(pBpMv, tmpStr, len);
	}
	else {
		len = (int)strlen(pK->KKnam);
		memmove(pBpMv, pK->KKnam, len);
	}

	PF[i].PITM[0] = pBpMv;
	pBpMv += (len + 1);
	i++;


	int fieldNo = (useSmallFontSize()) ? 3 : 2;
	return _xprn(pno, fieldNo, PF, 0, pDC, 0, 1);

}


//-----------------------------------------------------------------------------
// 印刷3
//-----------------------------------------------------------------------------
// 引数
//		pno		:
//		fno		:
//		*pDC	:
//		*pBp	:
//-------------------------------------
// 返送値
//		0		:		正常終了
//-----------------------------------------------------------------------------
int CShinFudousan31Ex2::PrintHyo3( int pno, int fno, CDC *pDC, char *pBp )
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
	KKVAL* pJ = (*m_pTaxListData)->NRec.KFval;

	i = 0;
	memset(pBp, '\0', Msiz);
	char *pBpMv = pBp;

	//　⑤～⑪、⑫、⑬　経費　自由入力　その他の経費
	lin = FudDataLine5;
	max = FudDataLine13 + 1;

	for (int clm = clmA; clm < clmSize; clm++) {
		for (j = 0; j + lin < max; j++) {
			pK = &(*m_pTaxListData)->NRec.KFval[j + lin];
			st = 0;
			::ZeroMemory(buf, sizeof(buf));

			st = DispJudge(clm, j + lin);

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

	// ⑭　計
		pK = &(*m_pTaxListData)->NRec.KFval[FudDataLine14];
		st = 0;
		::ZeroMemory(buf, sizeof(buf));
		st = DispJudge(clm, FudDataLine14);

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


		i++;
	}


	// ⑮　差引金額(決算額 A)
	pK = &(*m_pTaxListData)->NRec.KFval[FudDataLine15];
	::ZeroMemory(buf, sizeof(buf));
	if (DispJudge(clmA, FudDataLine15) == 1) {
		m_pArith->l_print(buf, pK->KKval1, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i].PITM[0] = pBpMv;
	pBpMv += (len + 1);
	i++;



if (useSmallFontSize()) {
	return _xprn(pno, 5, PF, 0, pDC, 0, 0);
}
else {
	return _xprn(pno, 4, PF, 0, pDC, 0);
}

}


//-----------------------------------------------------------------------------
// 初期ポジションセット ('14.08.08)
//-----------------------------------------------------------------------------
void CShinFudousan31Ex2::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
		if( (index = m_Fudousan31.GetPosition()) != -1) {
			m_Fudousan31.SetPosition(index);
			return;
		}
	}
	SetItemPositionEx();
}

void CShinFudousan31Ex2::SetItemPositionEx()
{
	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Fudousan31.SetPosition(FudVal01A);
	}
	else{
		m_Fudousan31.SetPosition(FudName03);
	}
	SetRedrawScroll( 0 );
}


//-----------------------------------------------------------------------------
// 表示判定
// 引数
//		clm		:	列
//		dataId	:	表示対象とするデータ項目の保存位置
// 戻り値
//      st      :   1=表示可能、0=表示不可
//-----------------------------------------------------------------------------
int CShinFudousan31Ex2::DispJudge( int clm, int line )
{
	int st = 0;
	int j = 0;
	
	switch( clm ){
		case clmA:	// 決算額 A
			switch( line ){
				case FudDataLine15:	//差引金額
				case FudDataLine4:		//収入金額　計					
					for( j = FudDataLine1 ; j < FudDataLine4 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
							st = 1;
							break;
						}
					}

					if( line != FudDataLine15 ){
						break;
					}

				case FudDataLine14:	//必要経費　計
					for( j = FudDataLine5; j < FudDataLine14; j++ ){
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

		case clmB:	// Aのうち課税取引にならないもの　B
			switch( line ){
				case FudDataLine7:		//修繕費
				case FudDataLine15:	//差引金額
					//入力可所無し
					break;

				case FudDataLine4:		//収入金額　計
					for( j = FudDataLine1; j < FudDataLine4 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval2) ){
							st = 1;
							break;
						}
					}
					break;

				case FudDataLine14:	//必要経費　計
					for( j = FudDataLine5 ; j < FudDataLine14 ; j++ ){
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

		case clmC:	// 課税取引金額　C（A－B）
			switch( line ){
				case FudDataLine6:		//損害保険料
				case FudDataLine8:		//減価償却費
				case FudDataLine9:		//借入金利子
				case FudDataLine15:	//差引金額
					break;

				case FudDataLine4:		//収入金額　計
					for( j = FudDataLine1 ; j < FudDataLine4 ; j++ ){
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

				case FudDataLine14:	//必要経費　計
					for( j = FudDataLine5; j < FudDataLine14; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval1) ){
							st = 1;
							break;
						}
						if( j == FudDataLine7 ){ //修繕費はスキップ
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

		case clmD:	// うち税率６．２４％適用分　D
			switch( line ){
				case FudDataLine1:		//賃貸料
				case FudDataLine2:		//礼金・権利金更新料
				case FudDataLine5:		//租税公課
				case FudDataLine6:		//損害保険料
				case FudDataLine7:		//修繕費
				case FudDataLine8:		//減価償却費
				case FudDataLine9:		//借入金利子
				case FudDataLine10:	//地代家賃
				case FudDataLine11:	//給料賃金
				case FudDataLine15:	//差引金額
					//入力可所無し
					break;


				case FudDataLine4:		//収入金額　計
					for( j = FudDataLine1; j < FudDataLine4 ; j++ ){
						if( m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval5) ){
							st = 1;
							break;
						}
					}
					break;

				case FudDataLine14:	//必要経費　計
					for( j = FudDataLine5 ; j < FudDataLine14 ; j++ ){
						if(j == FudDataLine6 || j == FudDataLine8 || j == FudDataLine9){//損害保険料、減価償却費、借入金利子　は入力不可なのでチェックしない
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

			//戎居　変更
		case clmE:	// うち税率７．８％適用分　E
			switch (line) {
			case FudDataLine1:		//賃貸料
			case FudDataLine2:		//礼金・権利金更新料
			case FudDataLine3:		//
			case FudDataLine4:		//
			case FudDataLine5:		//租税公課
			case FudDataLine6:		//損害保険料
			case FudDataLine7:		//
			case FudDataLine8:		//減価償却費
			case FudDataLine9:		//借入金利子
			case FudDataLine10:		//地代家賃
			case FudDataLine11:		//給料賃金
			case FudDataLine15:		//差引金額
				//入力可所無し
				break;

			case FudDataLine14:	//必要経費　計
				for (j = FudDataLine5; j < FudDataLine14; j++) {
					if (j == FudDataLine6 || j == FudDataLine8 || j == FudDataLine9) {//損害保険料、減価償却費、借入金利子　は入力不可なのでチェックしない
						continue;
					}
// インボイス_23/07/21_higuchi cor -->
					//if (m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval6)) {
// -----------------------------------
					if (m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval7)) {
// インボイス_23/07/21_higuchi cor <--
						st = 1;
						break;
					}
				}
				break;

			default:
// インボイス_23/07/21_higuchi cor -->
				//if (m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval6)) {
// -----------------------------------
				if (m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval7)) {
// インボイス_23/07/21_higuchi cor <--
					st = 1;
				}
				break;
			}
			break;

		case clmF:	// うち税率７．８％適用分　F
			switch (line) {
			case FudDataLine6:		//損害保険料
			case FudDataLine8:		//減価償却費
			case FudDataLine9:		//借入金利子
			case FudDataLine15:	//差引金額
				//入力可所無し
				break;

			case FudDataLine4:		//収入金額　計
				for (j = FudDataLine1; j < FudDataLine4; j++) {
					if (m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval6)) {
						st = 1;
						break;
					}
				}
				break;

			case FudDataLine14:	//必要経費　計
				for (j = FudDataLine5; j < FudDataLine14; j++) {
					if (j == FudDataLine6 || j == FudDataLine8 || j == FudDataLine9) {//損害保険料、減価償却費、借入金利子　は入力不可なのでチェックしない
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
		case clmG:	// うち税率７．８％適用分　G
			switch (line) {
			case FudDataLine1:		//賃貸料
			case FudDataLine2:		//礼金・権利金更新料
			case FudDataLine3:		//
			case FudDataLine4:		//
			case FudDataLine6:		//損害保険料
			case FudDataLine8:		//減価償却費
			case FudDataLine9:		//借入金利子
			case FudDataLine15:	//差引金額
				//入力可所無し
				break;

			case FudDataLine14:	//必要経費　計
				for (j = FudDataLine5; j < FudDataLine14; j++) {
					if (j == FudDataLine6 || j == FudDataLine8 || j == FudDataLine9) {//損害保険料、減価償却費、借入金利子　は入力不可なのでチェックしない
						continue;
					}
// インボイス_23/07/21_higuchi cor -->
					//if (m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval6)) {
// -----------------------------------
					if (m_pArith->l_test((*m_pTaxListData)->NRec.KFval[j].KKval8)) {
// インボイス_23/07/21_higuchi cor <--
						st = 1;
						break;
					}
				}
				break;

			default:
// インボイス_23/07/21_higuchi cor -->
				//if (m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval6)) {
// -----------------------------------
				if (m_pArith->l_test((*m_pTaxListData)->NRec.KFval[line].KKval8)) {
// インボイス_23/07/21_higuchi cor <--
					st = 1;
				}
				break;
			}
			break;
// 追加ここまで

		default:
			break;
	}
	return st;
}

// 手入力サインのクリア
int CShinFudousan31Ex2::ClearManualInputSign()
{
	for( int idx = 0; idx < 50; idx++ ){
		(*m_pTaxListData)->NRec.KFval[idx].KKauto = 0x00;
	}

	return 0;
}


FCSUPDOWN_INFO CShinFudousan31Ex2::FCS_LR[] = {};
FCSUPDOWN_INFO CShinFudousan31Ex2::FCS_UD[] = {};

bool CShinFudousan31Ex2::useSmallFontSize() {

// インボイス_23/07/21_higuchi cor -->
	//char val[5][128] = { 0, 0 };
// -----------------------------------
	char val[7][128] = { 0, 0 };
// インボイス_23/07/21_higuchi cor <--
	CString str;

	for (int l_cnt = FudDataLine1; l_cnt <= FudDataLine4TA; l_cnt++) {
		switch (l_cnt) {
		case FudDataLine1:
		case FudDataLine2:
		case FudDataLine3:
		case FudDataLine4:
		case FudDataLine4TA:
		case FudDataLine5:
		case FudDataLine6:
		case FudDataLine7:
		case FudDataLine8:
		case FudDataLine9:
		case FudDataLine10:
		case FudDataLine11:
		case FudDataLine12:
		case FudDataLine13:
		case FudDataLine14:
		case FudDataLine15:

// インボイス_23/07/21_higuchi cor -->
			//for (int idx = 0; idx < 5; idx++) {
// -----------------------------------
			for (int idx = 0; idx < 7; idx++) {
// インボイス_23/07/21_higuchi cor <--
				::ZeroMemory(val[idx], sizeof(val[idx]));
			}
			m_pArith->l_print(val[0], (*m_pTaxListData)->NRec.KFval[l_cnt].KKval1, FMTX130);
			m_pArith->l_print(val[1], (*m_pTaxListData)->NRec.KFval[l_cnt].KKval2, FMTX130);
			m_pArith->l_print(val[2], (*m_pTaxListData)->NRec.KFval[l_cnt].KKval3, FMTX130);
			m_pArith->l_print(val[3], (*m_pTaxListData)->NRec.KFval[l_cnt].KKval5, FMTX130);
			m_pArith->l_print(val[4], (*m_pTaxListData)->NRec.KFval[l_cnt].KKval6, FMTX130);
// インボイス_23/07/21_higuchi add -->
			m_pArith->l_print(val[5], (*m_pTaxListData)->NRec.KFval[l_cnt].KKval7, FMTX130);
			m_pArith->l_print(val[6], (*m_pTaxListData)->NRec.KFval[l_cnt].KKval8, FMTX130);
// インボイス_23/07/21_higuchi add <--

// インボイス_23/07/21_higuchi cor -->
			//for (int idx = 0; idx < 5; idx++) {
// -----------------------------------
			for (int idx = 0; idx < 7; idx++) {
// インボイス_23/07/21_higuchi cor <--
				str.Format(val[idx]);
				str.Trim();
				// 10億円 ( カンマ含む )以上ならフォントサイズを下げる
				//if( str.GetLength() > 13 ){
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
void CShinFudousan31Ex2::setFCSArray()
{
	//水平方向フォーカス移動用
	if (FCS_LR[0].IDNo == NULL) {
		int cnt = 0;
		for (int i = 0; i < rowSize; i++) {
			if (i >= FudDiagLine14) { break; }               //計、差引金額はフォーカス対象外
			if (i == FudDiagLine3 || i == FudDiagLine12) { //項目名欄の挿入処理

				int tmp = (i == FudDiagLine3) ? FudName03
					: FudName12;
				FCS_LR[cnt] = { (short)tmp };
				cnt++;
			}
			for (int j = 0; j < clmSize; j++) {
				if (j == clmC) { continue; }        //C列
				if (idxMap[i][j] < 0) { continue; } //斜線部分
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
		FCS_UD[0] = { (short)FudName03 };
		FCS_UD[1] = { (short)FudName12 };
		cnt = 2;

		for (int j = 0; j < clmSize; j++) {
			if (j == clmC) { continue; } //C列
			for (int i = 0; i < rowSize; i++) {
				if (i >= FudDiagLine14) { break; }    //計、差引金額はフォーカス対象外
				if (idxMap[i][j] < 0) { continue; } //斜線部分
				FCS_UD[cnt] = { (short)idxMap[i][j] };
				cnt++;
			}
		}
		FCS_UD[cnt] = { 0 };
	}
}

// 修正No.168690 add -->
void CShinFudousan31Ex2::DiagReSetPosition(void)
{
	this->SetFocus();
}
// 修正No.168690 add <--
