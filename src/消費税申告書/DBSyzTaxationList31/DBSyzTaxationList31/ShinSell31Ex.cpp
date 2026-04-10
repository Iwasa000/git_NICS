// ShinSell31.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSell31Ex.h"
#include "afxdialogex.h"
#include "ShinSell31ExIdx.h"


// CShinSell31Ex ダイアログ

IMPLEMENT_DYNAMIC(CShinSell31Ex, CSyzBaseDlg)

CShinSell31Ex::CShinSell31Ex(CWnd* pParent /*=nullptr*/)
	: CSyzBaseDlg(IDD_DIALOG_SELL31_ex, pParent)
	, m_pParent ( pParent )
	, m_Idx (0)
	, m_Lin (0)
{
	m_curwnd = -1;

	m_FcsUpEnd = -1;
	m_FcsLeftEnd = -1;
	m_FcsDownEnd = -1;
	m_FcsRightEnd = -1;
}

CShinSell31Ex::~CShinSell31Ex()
{
}

void CShinSell31Ex::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Sell31);
}


BEGIN_MESSAGE_MAP(CShinSell31Ex, CSyzBaseDlg)
END_MESSAGE_MAP()


BOOL CShinSell31Ex::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Sell31;
	//m_BottomIdx = 121;//コントロールの一番下のIDX指定するとスクロールがそこまでいく
	m_BottomIdx = 90;
	   	 
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


int CShinSell31Ex::Init()
{
	// ダイアグ初期化
	InitDiagAttr();

	// 画面表示
	DispList(0);	// ここの処理で落ちへんか確認しとく。

	// 初期ポジション
	SetItemPosition();

	return 0;
}


int CShinSell31Ex::InitDiagAttr()
{
	DIAGRAM_ATTRIBUTE data;
	long dg1[] = { SEL31NAMEex, -1 };

	for( int i=0;; i++ ){
		if( dg1[i] == -1 ){
			break;
		}
		// ank対応
		m_Sell31.GetAttribute( (short)dg1[i], (LPUNKNOWN)&data );
		data.attr_editattr |= DIAG_EATTR_ZENHAN;//DIAG_EATTR_ZENHAN ank対応
		m_Sell31.SetAttribute( (short)dg1[i], (LPUNKNOWN)&data, 1 );
	}

	int cnt = 0;
	while (1)
	{
		if (Sell31Ex[cnt].Index == 0) {
			break;
		}
		int Flg;
		if ((*m_pSnHeadData)->Sn_Sign4 & 0x01) {	// 確定時は入力不可
			Flg = Sell31Ex[cnt].AllUnConnect;
		}
		else {
			if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {	// 非連動
				// 経過措置ありのときは6.3％入力不可
				//if (!((*m_pSnHeadData)->SVmzsw) && (Sell31Ex[cnt].UnConnect == 2)) {
				//	Flg = 0;
				//}
				//else
				//{
					Flg = Sell31Ex[cnt].UnConnect;
				//}
			}
			else {
				// 経過措置ありのときは6.3％入力不可
				/*if (!((*m_pSnHeadData)->SVmzsw) && (Sell31Ex[cnt].Connect == 2)) {
					Flg = 0;
				}
				else*/
					Flg = Sell31Ex[cnt].Connect;
			}
		}
		ATRIB_MOD(IDC_ICSDIAGCTRL1, Sell31Ex[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Sell31Ex[cnt].Connect);
		cnt++;
	}

// 22-0167,22-0361 del -->
	//InitAddText();
// 22-0167,22-0361 del <--

	SetFocusRange();

	m_Sell31.EnableDelete(0);


	return 0;
}

// 22-0167,22-0361 del -->
////=====================================================
//// 追加文言の表示/非表示
////=====================================================
//void CShinSell31Ex::InitAddText()
//{
//	long flg = 0; 
//	if ( ( *m_pSnHeadData )->m_DispTabSgn & 0x1000 ) {
//		flg = DIAG_MDFY_OPAQUE;
//	}
//	else {
//		flg = DIAG_MDFY_TRANSPARENT;
//	}
//
//	for ( short index = SADDTEXTSex; index <= SADDTEXTEex; index++ ) {
//		m_Sell31.ModifyItem ( index, flg );
//	}
//}
// 22-0167,22-0361 del <--

void CShinSell31Ex::SetItemPosition()
{

	int index = 0;

	// 前回ポジションの復元
	if (m_curwnd == IDC_ICSDIAGCTRL1) {
		if ((index = m_Sell31.GetPosition()) != -1) {
			m_Sell31.SetPosition(index);
			m_Sell31.SetFocus();
			return;
		}
	}

	this->SetFocus();
// 22-0167,22-0361 cor -->
	//if ((*m_pSnHeadData)->Sn_Sign4 & 0x80) {	// 非連動
	//	m_Sell31.SetPosition(SEL3101ex);	// とりあえず仮
	//}
	//else {	// 連動
	//	m_Sell31.SetPosition(SEL3104ex);	// とりあえず仮
	//}
// -----------------------
	DIAGRAM_ATTRIBUTE	da;
	// 非連動
	if((*m_pSnHeadData)->Sn_Sign4 & 0x80)	{
		m_Sell31.GetAttribute(SEL3101ex,(LPUNKNOWN)&da);
		if(da.attr_bcolor == BC_CREAM || da.attr_bcolor == BC_BLUE){
			m_Sell31.SetPosition(SEL3101ex);
		}
		else	{
			m_Sell31.SetPosition(SEL3101Aex);
		}
	}
	else {	// 連動
		m_Sell31.GetAttribute(SEL3104ex,(LPUNKNOWN)&da);
		if(da.attr_bcolor == BC_CREAM || da.attr_bcolor == BC_BLUE){
			m_Sell31.SetPosition(SEL3104ex);
		}
		else	{
			m_Sell31.SetPosition(SEL3104Aex);
		}
	}
// 22-0167,22-0361 cor <--
	SetRedrawScroll(0);

}

void CShinSell31Ex::SetItemPositionEx()
{
	this->SetFocus();
	DIAGRAM_ATTRIBUTE	da;
	// 非連動
	if((*m_pSnHeadData)->Sn_Sign4 & 0x80)	{
		m_Sell31.GetAttribute(SEL3101ex,(LPUNKNOWN)&da);
		if(da.attr_bcolor == BC_CREAM || da.attr_bcolor == BC_BLUE){
			m_Sell31.SetPosition(SEL3101ex);
		}
		else	{
			m_Sell31.SetPosition(SEL3101Aex);
		}
	}
	else {	// 連動
		m_Sell31.GetAttribute(SEL3104ex,(LPUNKNOWN)&da);
		if(da.attr_bcolor == BC_CREAM || da.attr_bcolor == BC_BLUE){
			m_Sell31.SetPosition(SEL3104ex);
		}
		else	{
			m_Sell31.SetPosition(SEL3104Aex);
		}
	}
	SetRedrawScroll(0);
}

int CShinSell31Ex::ChangeColor( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, index, &DA, CIcsdiagctrl );
	switch( sign ){
		case 1:	// 入力不可	（白色）
			DA.attr_bcolor = BC_WHITE;
			break;
		case 2:	// ロック	（灰色）
			DA.attr_bcolor = BC_GRAY;
			break;
		case 3:	// 特殊色①	（青色）
			DA.attr_bcolor = BC_BLUE;
			break;
		case 4:	// 特殊色②	（緑色）	
			DA.attr_bcolor = BC_GREEN;
			break;
		case 5:	// 特殊色③	（薄緑色）	 
			DA.attr_bcolor = BC_GREEN_L;
			break;
		default:	// 実額入力	（クリーム色） 
			DA.attr_bcolor = BC_CREAM;
			break;
	}
	diag_setattr( id, index, &DA, 1, CIcsdiagctrl );

	return 0;
}

//---------------------------------------------------------
// 画面表示
//---------------------------------------------------------
// 引数　	index	：	ダイアグインデックス
//			move	：	0：下　1：上　2：右　3：左
//---------------------------------------------------------
// 返送値	idx		：	次のフォーカスインデックス
//---------------------------------------------------------
int CShinSell31Ex::FocusMoveCheck(int index, int move)
{
	int idx = 0;
	int i = 0 , j = 0;
	BOOL end_flg = FALSE;

	DIAGRAM_ATTRIBUTE	DA;

	//ポジションの端到達判定
	if(move == 0){//下方向
		if( index == m_FcsDownEnd ){
			idx = m_FcsUpEnd;
			end_flg = TRUE;
		}
	}
	else if(move == 1){//上方向
		if( index == m_FcsUpEnd ){
			idx = m_FcsDownEnd;
			end_flg = TRUE;
		}
	}
	else if(move == 2){//右方向
		if( index == m_FcsRightEnd ){
			idx = m_FcsLeftEnd;
			end_flg = TRUE;
		}
	}
	else if(move == 3){//左方向
		if( index == m_FcsLeftEnd ){
			idx = m_FcsRightEnd;
			end_flg = TRUE;
		}
	}

	if( end_flg == TRUE ){
		return idx;
	}

	//現在ポジション検索
	if(( move == 0 )||( move == 1 )){
		for(i = 0 ; i < FCS_SEL31CNTex ; i++){
			if(index == FCS_Sell31Ex[i].IDNo){
				break;
			}
		}
		idx = FCS_Sell31Ex[i].IDNo;//現在ポジションセット
	}else if(( move == 2 )||( move == 3 )){
		for(i = 0 ; i < FCS_SEL31CNTex ; i++){
			if(index == FCS_Sell31Ex_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_Sell31Ex_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(move == 0){//下方向
		for(j = i + 1 ; j < FCS_SEL31CNTex ; j++){
			m_Sell31.GetAttribute(FCS_Sell31Ex[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Sell31Ex[j].IDNo;
				break;
			}
		}
	}
	else if(move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Sell31.GetAttribute(FCS_Sell31Ex[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Sell31Ex[j].IDNo;
				break;
			}
		}
	}
	else if(move == 2){//右方向
		for(j = i + 1 ; j < FCS_SEL31CNTex ; j++){
			m_Sell31.GetAttribute(FCS_Sell31Ex_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Sell31Ex_LR[j].IDNo;
				break;
			}
		}
	}
	else if(move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Sell31.GetAttribute(FCS_Sell31Ex_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Sell31Ex_LR[j].IDNo;
				break;
			}
		}
	}


	return idx;
}

//---------------------------------------------------------
// 画面表示
//---------------------------------------------------------
// 引数　mode		：		0　？
//---------------------------------------------------------
void CShinSell31Ex::DispList( int mode )
{
	m_Sell31.OffControl( TRUE );

	DispSell31DiagData();

	DIAGRAM_DATA	DiagData;

	if( !mode ){
		m_Util.DiagOcxIniz( &DiagData );
		diag_clear( IDC_ICSDIAGCTRL1, SKYEARIDex, 1, CIcsdiagctrl );
		DiagData.data_disp.Format( _T("（令和%2x年分）"), (*m_pTaxListData)->YEAR );	// 令和固定で大丈夫やんな？？
		diag_setdata( IDC_ICSDIAGCTRL1, SKYEARIDex, &DiagData, CIcsdiagctrl );

		PostMessage( WM_VSCROLL, SB_TOP, 0 );
	}

	m_Util.DiagOcxIniz( &DiagData );
	diag_clear( IDC_ICSDIAGCTRL1, SKZKBNIDex, TRUE, CIcsdiagctrl );
	DiagData.data_disp = _T("経理方式：");
	if( (*m_pTaxListData)->Head.KHstax&0x01 ){
		DiagData.data_disp += _T("税抜き");
	}
	else{
		DiagData.data_disp += _T("税込み");
	}
	diag_setdata( IDC_ICSDIAGCTRL1, SKZKBNIDex, &DiagData, CIcsdiagctrl );

	if( mode ){
		/*
		if( (m_Idx==KSELVAL14K) && !(*m_pTaxListData)->Head.KHstax ){
			m_Idx = KSELVAL14C;
		}*/

		/*if( (m_Idx==SEL3111A) && !(*m_pTaxListData)->Head.KHstax ){	// あってる？？
			m_Idx = SEL3111B;
		}*/
	}
	//diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );


	//SetRedrawScrollToIdx(m_Sell31.GetPosition());
// 22-0167,22-0361 add -->
	// (pK+3)について、3と4以外は連動時にクリアされるので3を使用する
	// ※事業・農業・不動産については連動時は自動計算チェックを含む
	// 　すべてがクリアされるので問題なし
	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;
	int		idx=0;
	for(idx=SELChk1ex; idx<=SELChk3ex; idx++)	{
		diag_getdata(IDC_ICSDIAGCTRL1,idx,&DiagData,CIcsdiagctrl);
		DiagData.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1,idx,&DiagData,CIcsdiagctrl);
	}
	idx=0;
	if((*m_pSnHeadData)->Sn_GenericSgn & 0x800)		idx=SELChk3ex;
	//else if((pK+3)->KKauto & 0x40)	idx=SELChk2ex;
	//else if((pK+3)->KKauto & 0x80)	idx=SELChk3ex;
	if(idx != 0)	{
		diag_getdata(IDC_ICSDIAGCTRL1,idx,&DiagData,CIcsdiagctrl);
		DiagData.data_check = 1;
		diag_setdata(IDC_ICSDIAGCTRL1,idx,&DiagData,CIcsdiagctrl);
	}
	//// 確定時は選択不可
	//if((*m_pSnHeadData)->Sn_Sign4 & 0x01)	{
	//	for(idx=SELChk1ex; idx<=SELChk3ex; idx++)	{
	//		ATRIB_MOD(IDC_ICSDIAGCTRL1,idx,0,(*m_pSnHeadData)->Sn_Sign4,0);
	//	}
	//}
	// 金額、6.24%適用分は非表示にする(今後要望があったときのために残しておく)
	diag_modify(IDC_ICSDIAGCTRL1,SELChk1ex,DIAG_MDFY_TRANSPARENT,CIcsdiagctrl);
	diag_modify(IDC_ICSDIAGCTRL1,SELChk2ex,DIAG_MDFY_TRANSPARENT,CIcsdiagctrl);
	// 確定時は7.8%適用分を選択不可にする
	if((*m_pSnHeadData)->Sn_Sign4 & 0x01)	{
		//ATRIB_MOD(IDC_ICSDIAGCTRL1,SELChk3ex,0,(*m_pSnHeadData)->Sn_Sign4,0);
		DIAGRAM_ATTRIBUTE	da;
		diag_modify(IDC_ICSDIAGCTRL1,SELChk3ex,DIAG_MDFY_READONLY,CIcsdiagctrl);
		diag_getattr(IDC_ICSDIAGCTRL1,SELChk3ex,&da,CIcsdiagctrl);
		da.attr_bcolor = BC_GRAY;
		diag_setattr(IDC_ICSDIAGCTRL1,SELChk3ex,&da,FALSE,CIcsdiagctrl);
	}
	else	{
		DIAGRAM_ATTRIBUTE	da,da2;
		diag_getattr(IDC_ICSDIAGCTRL1,0,&da2,CIcsdiagctrl);
		diag_getattr(IDC_ICSDIAGCTRL1,SELChk3ex,&da,CIcsdiagctrl);
		da.attr_bcolor = da2.attr_bcolor;
		diag_setattr(IDC_ICSDIAGCTRL1,SELChk3ex,&da,FALSE,CIcsdiagctrl);
		diag_modify(IDC_ICSDIAGCTRL1,SELChk3ex,DIAG_MDFY_EDIT,CIcsdiagctrl);
	}

// 22-0167,22-0361 add <--

}

//-----------------------------------------------
// ダイアグデータ表示
//-----------------------------------------------
void CShinSell31Ex::DispSell31DiagData()
{
	short			idx, i;
	KKVAL*			pK = (*m_pTaxListData)->NRec.KUval;
	DIAGRAM_DATA	data;
	int		st = 0;
	short	j = 0;
	int		skip = 0;

	for( i = 0 ; i < SKLINCNTex+1 ; i++ ){

		////農業の部分スキップ
		//if(i == 3 || i == 4 || i == 5){
		//	continue;
		//}

		for(j = 0 ; j < 3 ; j++){

			st = 0;
			skip = 0;

			/*if(j > 0){
				if( i == KSELLINE1 || i == KSELLINE2 || i == KSELLINE4  || i == KSELLINE5  || i == KSELLINE7  || i == KSELLINE8 ||
					i == KSELLINE10 || i == KSELLINE11 || i == KSELLINE16)
				{
					continue;
				}
			}*/

			m_Util.DiagOcxIniz( &data );

			switch(i){

				case KSELLINE1:
					idx = SEL3101ex + j;//インデックスをセット
					break;
				case KSELLINE2:
					idx = SEL3102ex + j;//インデックスをセット
					break;
				case KSELLINE3:
					idx = SEL3103ex + j;//インデックスをセット
					break;
				case KSELLINE4:
					idx = SEL3104ex + j;//インデックスをセット
					break;
				case KSELLINE5:
					idx = SEL3105ex + j;//インデックスをセット
					break;
				case KSELLINE6:
					idx = SEL3106ex + j;//インデックスをセット
					break;
				case KSELLINE7:
					idx = SEL3107ex + j;//インデックスをセット
					break;
				case KSELLINE8:
					idx = SEL3108ex + j;//インデックスをセット
					break;
				case KSELLINE9:
					idx = SEL3109ex + j;//インデックスをセット
					break;
				case KSELLINE10:
					idx = SEL3110ex + j;//インデックスをセット
					break;
				case KSELLINE11+1:	// calcの関係で+1する
					switch( j ){
						case 0:
							idx = SEL3111Cex;
							break;
						case 1:
							idx = SEL3111Bex;
							break;
						case 2:
							idx = SEL3111Aex;
							break;
					}
					break;
				case KSELLINE12+1:	// calcの関係で+1する
					switch( j ){
						case 0:
							idx = SEL3112Cex;
							break;
						case 1:
							idx = SEL3112Bex;
							break;
						case 2:
							idx = SEL3112Aex;
							break;
					}
					break;
				default:
					//idx = (i * 3) + j + SKVMIN;//インデックスをセット
					skip = 1;
					break;
			}

			if( skip ){
				continue;
			}

			diag_clear( IDC_ICSDIAGCTRL1, idx, TRUE, CIcsdiagctrl );

			st = DispJudge( j, i );

			if( st ){
				/*if(j == 0)		memcpy( data.data_val, (pK + i)->KKval1, 6 );
				else if(j == 1)	memcpy( data.data_val, (pK + i)->KKval2, 6 );
				else if(j == 2)	memcpy( data.data_val, (pK + i)->KKval3, 6 );
				else if(j == 3)	memcpy( data.data_val, (pK + i)->KKval4, 6 );*/

				switch( i ){
					case KSELLINE11+1:	// calcの関係で+1する
					case KSELLINE12+1:
						if(j == 0)		memcpy( data.data_val, (pK + i)->KKval1, 6 );
						else if(j == 1)	memcpy( data.data_val, (pK + i)->KKval2, 6 );
						else if(j == 2)	memcpy( data.data_val, (pK + i)->KKval3, 6 );
						break;
					default:
						if(j == 0)		memcpy( data.data_val, (pK + i)->KKval1, 6 );
						else if(j == 1)	memcpy( data.data_val, (pK + i)->KKval3, 6 );
						else if(j == 2)	memcpy( data.data_val, (pK + i)->KKval4, 6 );
						break;
				}

				diag_setdata( IDC_ICSDIAGCTRL1, idx, &data, CIcsdiagctrl );
			}
		}
	}

	// 項目名
	char	buf[128];
	::ZeroMemory( buf, sizeof(buf) );
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, SEL31NAMEex, TRUE, CIcsdiagctrl );
	data.data_edit = m_Util.CutHankaku( (*m_pTaxListData)->Head.KHkunm, sizeof((*m_pTaxListData)->Head.KHkunm) );
	diag_setdata( IDC_ICSDIAGCTRL1, SEL31NAMEex, &data, CIcsdiagctrl );

	// 11	仮受消費税
	st = 0;
	m_Util.DiagOcxIniz(&data);
	diag_clear(IDC_ICSDIAGCTRL1, SEL3111Aex, TRUE, CIcsdiagctrl);
	if ((*m_pTaxListData)->Head.KHstax) {
		if (m_pArith->l_test((pK + KSELLINE10)->KKval3)) {
			st = 1;
		}
		if ((pK + KSELLINE11+1)->KKauto & 0x04) {	// calcの関係で+1する
			st = 1;
		}
	}
	if (st) {
		memcpy(data.data_val, (pK + KSELLINE11+1)->KKval3, 6);
		diag_setdata(IDC_ICSDIAGCTRL1, SEL3111Aex, &data, CIcsdiagctrl);
	}

	// 11	計算部
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, SEL3111Bex, TRUE, CIcsdiagctrl );
	if( m_pArith->l_test((pK + KSELLINE10)->KKval3) ){
		st = 1;
	}
	if( (pK + KSELLINE11+1)->KKauto&0x02 ){
		st = 1;
	}
	if( st ){
		memcpy( data.data_val, (pK + KSELLINE11+1)->KKval2, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, SEL3111Bex, &data, CIcsdiagctrl );
	}

	// 12	仮受消費税
	st = 0;
	m_Util.DiagOcxIniz(&data);
	diag_clear(IDC_ICSDIAGCTRL1, SEL3112Aex, TRUE, CIcsdiagctrl);
	if ((*m_pTaxListData)->Head.KHstax) {
		if (m_pArith->l_test((pK + KSELLINE10)->KKval4)) {
			st = 1;
		}
		if ((pK + KSELLINE12+1)->KKauto & 0x04) {	// calcの関係で+1する
			st = 1;
		}
	}
	if (st) {
		memcpy(data.data_val, (pK + KSELLINE12+1)->KKval3, 6);
		diag_setdata(IDC_ICSDIAGCTRL1, SEL3112Aex, &data, CIcsdiagctrl);
	}

	// 12	計算部
	st = 0;
	m_Util.DiagOcxIniz( &data );
	diag_clear( IDC_ICSDIAGCTRL1, SEL3112Bex, TRUE, CIcsdiagctrl );
	if( m_pArith->l_test((pK + KSELLINE10)->KKval4) ){
		st = 1;
	}
	if( (pK + KSELLINE12+1)->KKauto&0x02 ){
		st = 1;
	}
	if( st ){
		memcpy( data.data_val, (pK + KSELLINE12+1)->KKval2, 6 );
		diag_setdata( IDC_ICSDIAGCTRL1, SEL3112Bex, &data, CIcsdiagctrl );
	}


///////////背景色変更
	int cnt = 0;
	int Flg = 0;
	int line = -1;
	int colm = -1;
	unsigned char sgn = 0x00;
	bool bKniinp = false;
	if( m_Util.IsUnConnectMst( m_pZmsub->zvol ) && ( m_Util.GetKazeihoushiki ( m_pZmsub->zvol ) == ID_ICSSH_KANNI ) ) {
		// 事業別売上高
		bKniinp = TRUE;
	}

	while( Sell31Ex[cnt].Index != 0 ){
		
		line = GetLineNum( Sell31Ex[cnt].Index );
		colm = GetColumNum( Sell31Ex[cnt].Index );
		sgn = 0x00;
		switch(colm){
			case 0:
				sgn = 0x01;
				break;
			case 1:
				sgn = 0x04;
				break;
			case 2:
				sgn = 0x08;
				break;
			default:
				break;
		}


// 22-0167,22-0361 cor -->
		//if(( line == KSELLINE1 )||( line == KSELLINE2 )||( line == KSELLINE3 )||
		//	( line == KSELLINE7 )||( line == KSELLINE8 )||//yoshida191003
		//	///*( line == KSELLINE4 )||( line == KSELLINE5 )||( line == KSELLINE6 )||*/
		//   ( line == KSELLINE10 )||( line == KSELLINE11+1 )||( line == KSELLINE12+1 )){	// calcの関係で+1する
// -----------------------
		if(( line == KSELLINE1 )||( line == KSELLINE2 )||( line == KSELLINE3 )||
			( line == KSELLINE4 )||( line == KSELLINE5 )||
			( line == KSELLINE7 )||( line == KSELLINE8 )||//yoshida191003
		   ( line == KSELLINE10 )||( line == KSELLINE11+1 )||( line == KSELLINE12+1 )){	// calcの関係で+1する
// 22-0167,22-0361 cor <--

			// 確定時は全入力不可
			if( (*m_pSnHeadData)->Sn_Sign4&0x01){
				Flg = Sell31Ex[cnt].AllUnConnect;
			}
			else{
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					// 経過措置ありのときは6.3％入力不可
					/*if (!((*m_pSnHeadData)->SVmzsw) && (Sell31Ex[cnt].UnConnect == 2)) {
						Flg = 0;
					}
					else {
						Flg = Sell31Ex[cnt].UnConnect;
					}*/
					Flg = Sell31Ex[cnt].UnConnect;
				}
				else{
					// 経過措置ありのときは6.3％入力不可
					/*if (!((*m_pSnHeadData)->SVmzsw) && (Sell31Ex[cnt].Connect == 2)) {
						Flg = 0;
					}
					else
						Flg = Sell31Ex[cnt].Connect;*/
					Flg = Sell31Ex[cnt].Connect;
				}
			}

			if( Flg ){
				DIAGRAM_ATTRIBUTE attr;
				diag_getattr( IDC_ICSDIAGCTRL1, Sell31Ex[cnt].Index, &attr, CIcsdiagctrl );

				switch( Sell31Ex[cnt].Index ){	
					case SEL3101ex:
					case SEL3101Aex:
					case SEL3101Bex:
					case SEL3102ex:
					case SEL3102Aex:
					case SEL3102Bex:
					case SEL3103ex:
					case SEL3103Aex:
					case SEL3103Bex:
						if( (*m_pTaxListData)->NRec.KUval[line].KKauto&sgn ){
							attr.attr_bcolor = BC_BLUE;
						}else{
							attr.attr_bcolor = BC_CREAM;
						}
						break;

// 22-0167,22-0361 add -->
					// 自動計算のチェックを外した時、背景色が白のままになるので
					case SEL3104ex:
					case SEL3104Aex:
					case SEL3104Bex:
					case SEL3105ex:
					case SEL3105Aex:
					case SEL3105Bex:
						attr.attr_bcolor = BC_CREAM;
						break;
// 22-0167,22-0361 add <--

					// 事業別売上高を使用していない場合は、7, 8欄の色を変更しない
					case SEL3107ex:
					case SEL3107Aex:
					case SEL3107Bex:
					case SEL3108ex:
					case SEL3108Aex:
					case SEL3108Bex:
						if( bKniinp ){
							if( (*m_pTaxListData)->NRec.KUval[line].KKauto&sgn ){
								attr.attr_bcolor = BC_BLUE;
							}else{
								attr.attr_bcolor = BC_CREAM;
							}
						}
// 22-0167,22-0361 add -->
						// 自動計算のチェックを外した時、背景色が白のままになるので
						else	{
							attr.attr_bcolor = BC_CREAM;
						}
// 22-0167,22-0361 add <--
						break;

					case SEL3111Aex:
					case SEL3112Aex:
						if ((*m_pTaxListData)->NRec.KUval[line].KKauto& 0x04) {
							attr.attr_bcolor = BC_BLUE;
						}
						else {
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					case SEL3111Bex:
					case SEL3112Bex:
						if ((*m_pTaxListData)->NRec.KUval[line].KKauto& 0x02) {
							attr.attr_bcolor = BC_BLUE;
						}
						else {
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					case SEL3111Cex:
					case SEL3112Cex:
						if ((*m_pTaxListData)->NRec.KUval[line].KKauto& 0x01) {
							attr.attr_bcolor = BC_BLUE;
						}
						else {
							attr.attr_bcolor = BC_CREAM;
						}
						break;
					default:
						break;
				}
// 22-0167,22-0361 cor -->
				//diag_setattr( IDC_ICSDIAGCTRL1, Sell31Ex[cnt].Index, &attr, FALSE, CIcsdiagctrl );
// -----------------------
				int	flg2=-1;	// 0:列が自動計算　1:列が自動計算ではない
				//if(colm == 0 || colm == 1 || colm == 2)	{
				//	if(colm == 0)	{
				//		if((pK+3)->KKauto & 0x10)	flg2=0;
				//		else						flg2=1;
				//	}
				//	else if(colm == 1)	{
				//		if((pK+3)->KKauto & 0x40)	flg2=0;
				//		else						flg2=1;
				//	}
				//	else if(colm == 2)	{
				//		if((pK+3)->KKauto & 0x80)	flg2=0;
				//		else						flg2=1;
				//	}
				//}
				if(GetColumNum2(Sell31Ex[cnt].Index) == 2)	{
					if((*m_pSnHeadData)->Sn_GenericSgn & 0x800)	flg2=0;
					else										flg2=1;
				}
				// 金額列であれば入力可／不可を切り替える
				if(flg2 != -1)	{
					ATRIB_MOD(IDC_ICSDIAGCTRL1,Sell31Ex[cnt].Index,flg2,(*m_pSnHeadData)->Sn_Sign4,Sell31Ex[cnt].Connect);
				}
				// 列が自動計算でなければ背景色を付ける
				if(flg2 != 0)	{
					diag_setattr( IDC_ICSDIAGCTRL1, Sell31Ex[cnt].Index, &attr, FALSE, CIcsdiagctrl );
				}
// 22-0167,22-0361 cor <--
			}
		}
		cnt++;
	}
////////////

}

//-----------------------------------------------------------------------------
// ダイアグデータを取得
//-----------------------------------------------------------------------------
// 引数	index		：	ダイアグのインデックス
//		leng		：	
//		pData		：	ダイアグデータ
//		mode		：	
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CShinSell31Ex::DispJudge( int clm, int line )
{
	int st = 0;
	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;

	/*if( clm > 0 ){
		if( line == KSELLINE1 || line == KSELLINE2 || line == KSELLINE4  || line == KSELLINE5  || line == KSELLINE7  || line == KSELLINE8 ||
			line == KSELLINE10 || line == KSELLINE11 || line == KSELLINE16 )
		{
			return st;
		}
	}*/

	// 経過措置のチェックがない場合、6.3%は表示しない
	/*if ( ( *m_pSnHeadData )->SVmzsw == 0 ) {
		if ( ( clm == 1 ) || ( line == KSELLINE11 ) ) {
			return 0;
		}
	}*/


	switch( line ){
		case KSELLINE6://【 [   ]所得に係る課税売上高】　差引課税売上高（④－⑤）

			if( clm == 0 ){
				if( (pK + KSELLINE4)->KKauto & 0x01 || (pK + KSELLINE5)->KKauto & 0x01 ){
					st = 1;
				}
				if( m_pArith->l_test((pK + KSELLINE4)->KKval1) ||  m_pArith->l_test((pK + KSELLINE5)->KKval1)){
					st = 1;
				}
			}else if( clm == 1 ){
				if( (pK + KSELLINE4)->KKauto & 0x04 || (pK + KSELLINE5)->KKauto & 0x04){
					st = 1;
				}
				if( m_pArith->l_test((pK + KSELLINE4)->KKval3) || m_pArith->l_test((pK + KSELLINE5)->KKval3)){
					st = 1;
				}
			}
			else if (clm == 2 ) {
				if ((pK + KSELLINE4)->KKauto & 0x08 || (pK + KSELLINE5)->KKauto & 0x08) {
					st = 1;
				}
				if (m_pArith->l_test((pK + KSELLINE4)->KKval4) || m_pArith->l_test((pK + KSELLINE5)->KKval4)) {
					st = 1;
				}
			}
			break;

		case KSELLINE9://【 業務用資産の譲渡所得に係る課税売上高】差引課税売上高（⑦－⑧）
			if( clm == 0 ){
				if( m_pArith->l_test((pK + KSELLINE7)->KKval1) || m_pArith->l_test((pK + KSELLINE8)->KKval1) ){
					st = 1;
				}
				if( (pK + KSELLINE7)->KKauto & 0x01 || (pK + KSELLINE8)->KKauto & 0x01 ){
					st = 1;
				}
			}else if( clm == 1 ){
				if( m_pArith->l_test((pK + KSELLINE7)->KKval3) || m_pArith->l_test((pK + KSELLINE8)->KKval3)){
					st = 1;
				}
				if( (pK + KSELLINE7)->KKauto & 0x04 || (pK + KSELLINE8)->KKauto & 0x04){
					st = 1;
				}
			}else if (clm == 2 ) {
				if (m_pArith->l_test((pK + KSELLINE7)->KKval4) || m_pArith->l_test((pK + KSELLINE8)->KKval4)) {
					st = 1;
				}
				if ((pK + KSELLINE7)->KKauto & 0x08 || (pK + KSELLINE8)->KKauto & 0x08) {
					st = 1;
				}
			}
			break;

		case KSELLINE10://課税売上高の合計額（①＋②＋③＋⑥＋⑨）

			//計部分は所得の方から金額転記されることがあり、手入力立たないので金額もチェック'14.10.08
			if( clm == 0 ){
				if( (pK + KSELLINE1)->KKauto&0x01 || (pK + KSELLINE2)->KKauto&0x01 ){
					st = 1;
				}
				if ((pK + KSELLINE3)->KKauto & 0x01) {
					st = 1;
				}
				if( (pK + KSELLINE4)->KKauto&0x01 || (pK + KSELLINE5)->KKauto&0x01 ){
					st = 1;
				}
				if( (pK + KSELLINE7)->KKauto&0x01 || (pK + KSELLINE8)->KKauto&0x01 ){
					st = 1;
				}

				if( m_pArith->l_test((pK + KSELLINE1)->KKval1) ||  m_pArith->l_test((pK + KSELLINE2)->KKval1) ||
					m_pArith->l_test((pK + KSELLINE3)->KKval1) ||
					m_pArith->l_test((pK + KSELLINE4)->KKval1) ||  m_pArith->l_test((pK + KSELLINE5)->KKval1) ||
					m_pArith->l_test((pK + KSELLINE7)->KKval1) ||  m_pArith->l_test((pK + KSELLINE8)->KKval1) ){
					st = 1;
				}

			}else if( clm == 1 ){
				if( (pK + KSELLINE1)->KKauto & 0x04 || (pK + KSELLINE2)->KKauto & 0x04 || (pK + KSELLINE3)->KKauto & 0x04 || (pK + KSELLINE4)->KKauto & 0x04 ||
					(pK + KSELLINE5)->KKauto & 0x04 || (pK + KSELLINE7)->KKauto & 0x04 || (pK + KSELLINE8)->KKauto & 0x04){
					st = 1;
				}
				if( m_pArith->l_test((pK + KSELLINE1)->KKval3) ||  m_pArith->l_test((pK + KSELLINE2)->KKval3)  || m_pArith->l_test((pK + KSELLINE3)->KKval3) ||
					m_pArith->l_test((pK + KSELLINE4)->KKval3) ||  m_pArith->l_test((pK + KSELLINE5)->KKval3)  || m_pArith->l_test((pK + KSELLINE7)->KKval3) ||
					m_pArith->l_test((pK + KSELLINE8)->KKval3)){
					st = 1;
				}
			}else if (clm == 2) {
				if ((pK + KSELLINE1)->KKauto & 0x08 || (pK + KSELLINE2)->KKauto & 0x08 || (pK + KSELLINE3)->KKauto & 0x08 || (pK + KSELLINE4)->KKauto & 0x08 ||
					(pK + KSELLINE5)->KKauto & 0x08 || (pK + KSELLINE7)->KKauto & 0x08 || (pK + KSELLINE8)->KKauto & 0x08) {
					st = 1;
				}
				if ( m_pArith->l_test((pK + KSELLINE1)->KKval4) || m_pArith->l_test((pK + KSELLINE2)->KKval4) || m_pArith->l_test((pK + KSELLINE3)->KKval4) ||
					 m_pArith->l_test((pK + KSELLINE4)->KKval4) || m_pArith->l_test((pK + KSELLINE5)->KKval4) || m_pArith->l_test((pK + KSELLINE7)->KKval4) ||
					 m_pArith->l_test((pK + KSELLINE8)->KKval4)) {
					st = 1;
				}
			}

			break;

		/*case KSELLINE11://課税標準額の計算(⑬B欄の金額)
			if( m_pArith->l_test((pK + KSELLINE10)->KKval2) ){
				st = 1;
			}
			if( (pK + KSELLINE11)->KKauto&0x03 ){
				st = 1;
			}
			break;

		case KSELLINE12://課税標準額の計算(⑬C欄の金額)
			if( m_pArith->l_test((pK + KSELLINE10)->KKval3) ){
				st = 1;
			}
			if( (pK + KSELLINE12)->KKauto&0x03 ){
				st = 1;
			}
			break;

		case KSELLINE13://課税標準額の計算(⑬C欄の金額)
			if (m_pArith->l_test((pK + KSELLINE10)->KKval4)) {
				st = 1;
			}
			if ((pK + KSELLINE13)->KKauto & 0x03) {
				st = 1;
			}
			break;*/

		case KSELLINE11+1://課税標準額の計算(⑬C欄の金額)		// calcの関係で+1する
			if( m_pArith->l_test((pK + KSELLINE10)->KKval3) ){
				st = 1;
			}
			if( (pK + KSELLINE11+1)->KKauto&0x03 ){
				st = 1;
			}
			break;

		case KSELLINE12+1://課税標準額の計算(⑬C欄の金額)		// calcの関係で+1する
			if (m_pArith->l_test((pK + KSELLINE10)->KKval4)) {
				st = 1;
			}
			if ((pK + KSELLINE12+1)->KKauto & 0x03) {
				st = 1;
			}
			break;

		default:
			/*if( clm == 0 ){
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

			}else if (clm == 3 ) {
				if (m_pArith->l_test((pK + line)->KKval4) ) {
					st = 1;
				}
				if ((pK + line)->KKauto & 0x08) {
					st = 1;
				}
			}*/
			if( clm == 0 ){
				if( m_pArith->l_test((pK + line)->KKval1) ){
					st = 1;
				}
				if( (pK + line)->KKauto&0x01 ){
					st = 1;
				}
			}else if( clm == 1 ){
				if( m_pArith->l_test((pK + line)->KKval3) ){
					st = 1;
				}
				if( (pK + line)->KKauto&0x04 ){
					st = 1;
				}
			}else if (clm == 2 ) {
				if (m_pArith->l_test((pK + line)->KKval4) ) {
					st = 1;
				}
				if ((pK + line)->KKauto & 0x08) {
					st = 1;
				}
			}
			break;
	}

	return st;
}

//-----------------------------------------------------------------------------
// 行番号を取得
//-----------------------------------------------------------------------------
// 引数	index		：	インデックス
//-----------------------------------------------------------------------------
// 返送値	rt		：	行番号
//-----------------------------------------------------------------------------
int CShinSell31Ex::GetLineNum( int index )
{
	int rt = -1;

	switch( index ){
		case SEL3101ex:
		case SEL3101Aex:
		case SEL3101Bex:
			rt = KSELLINE1;
			break;
		case SEL3102ex:
		case SEL3102Aex:
		case SEL3102Bex:
			rt = KSELLINE2;
			break;
		case SEL3103ex:
		case SEL3103Aex:
		case SEL3103Bex:
			rt = KSELLINE3;
			break;
		case SEL3104ex:
		case SEL3104Aex:
		case SEL3104Bex:
			rt = KSELLINE4;
			break;
		case SEL3105ex:
		case SEL3105Aex:
		case SEL3105Bex:
			rt = KSELLINE5;
			break;
		case SEL3106ex:
		case SEL3106Aex:
		case SEL3106Bex:
			rt = KSELLINE6;
			break;
		case SEL3107ex:
		case SEL3107Aex:
		case SEL3107Bex:
			rt = KSELLINE7;
			break;
		case SEL3108ex:
		case SEL3108Aex:
		case SEL3108Bex:
			rt = KSELLINE8;
			break;
		case SEL3109ex:
		case SEL3109Aex:
		case SEL3109Bex:
			rt = KSELLINE9;
			break;
		case SEL3110ex:
		case SEL3110Aex:
		case SEL3110Bex:
			rt = KSELLINE10;
			break;
		case SEL3111Aex:
		case SEL3111Bex:
		case SEL3111Cex:
			rt = KSELLINE11+1;	// calcの関係で+1する
			break;
		case SEL3112Aex:
		case SEL3112Bex:
		case SEL3112Cex:
			rt = KSELLINE12+1;
			break;
		default:
			break;
	}

	return rt;
}

//-----------------------------------------------------------------------------
// カラム番号を取得
//-----------------------------------------------------------------------------
// 引数	index		：	インデックス
//-----------------------------------------------------------------------------
// 返送値	rt		：	カラム番号
//-----------------------------------------------------------------------------
int CShinSell31Ex::GetColumNum( int index )
{
	int rt = -1;

	switch( index ){
		case SEL3101ex:
		case SEL3102ex:
		case SEL3103ex:
		case SEL3104ex:
		case SEL3105ex:
		case SEL3106ex:
		case SEL3107ex:
		case SEL3108ex:
		case SEL3109ex:
		case SEL3110ex:
		/*case SEL3111ex:
		case SEL3112ex:
		case SEL3113ex:*/
			rt = 0;
			break;
		case SEL3101Aex:
		case SEL3102Aex:
		case SEL3103Aex:
		case SEL3104Aex:
		case SEL3105Aex:
		case SEL3106Aex:
		case SEL3107Aex:
		case SEL3108Aex:
		case SEL3109Aex:
		case SEL3110Aex:
		case SEL3111Aex:
		case SEL3112Aex:
			rt = 1;
			break;
		case SEL3101Bex:
		case SEL3102Bex:
		case SEL3103Bex:
		case SEL3104Bex:
		case SEL3105Bex:
		case SEL3106Bex:
		case SEL3107Bex:
		case SEL3108Bex:
		case SEL3109Bex:
		case SEL3110Bex:
		case SEL3111Bex:
		case SEL3112Bex:
			 rt = 2;
			break;
		default:
			break;
	}

	return rt;
}

//-----------------------------------------------------------------------------
// カラム番号を取得（入力不可、(6)課税資産の譲渡等の対価の額の計算を含まない
//-----------------------------------------------------------------------------
// 引数	index		：	インデックス
//-----------------------------------------------------------------------------
// 返送値	rt		：	カラム番号
//-----------------------------------------------------------------------------
int CShinSell31Ex::GetColumNum2( int index )
{
	int rt = -1;

	switch( index ){
		case SEL3101ex:
		case SEL3102ex:
		case SEL3103ex:
		case SEL3104ex:
		case SEL3105ex:
		case SEL3107ex:
		case SEL3108ex:
			rt = 0;
			break;
		case SEL3101Aex:
		case SEL3102Aex:
		case SEL3103Aex:
		case SEL3104Aex:
		case SEL3105Aex:
		case SEL3107Aex:
		case SEL3108Aex:
			rt = 1;
			break;
		case SEL3101Bex:
		case SEL3102Bex:
		case SEL3103Bex:
		case SEL3104Bex:
		case SEL3105Bex:
		case SEL3107Bex:
		case SEL3108Bex:
			 rt = 2;
			break;
		default:
			break;
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 各入力データを抽出
//-----------------------------------------------------------------------------
// 引数	index		：	ダイアグインデックス
//		leng		：	
//		pData		：	ダイアグデータ
//		mode		：	
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CShinSell31Ex::GetDiagData( long index, short leng, DIAGRAM_DATA *pData, int mode )
{
// 22-0167,22-0361 cor -->
	//if ( index < 0 ) {
// -----------------------
	if(index < 0 || index == SELChk1ex || index == SELChk2ex || index == SELChk3ex) {
// 22-0167,22-0361 cor <--
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

	
	if(( m_Idx == SEL3111Aex ) || ( m_Idx == SEL3112Aex )){	// 仮受消費税？
		if( !(*m_pTaxListData)->Head.KHstax&0x01 ){
			DispSell31DiagData();
			return 0;
		}
	}

	int clm_pos = 0;
	if(( m_Idx == SEL3111Aex )||( m_Idx == SEL3112Aex )){
		clm_pos = 2;
	}else if(( m_Idx == SEL3111Bex )||( m_Idx == SEL3112Bex )){
		clm_pos = 1;
	}else if(( m_Idx == SEL3111Cex )||( m_Idx == SEL3112Cex )){
		clm_pos = 0;
	}else{
		clm_pos = (m_Idx - SKVMINex) % 3;
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
	else if(m_Lin == 10)	Lin_tmp = KSELLINE11+1;	// calcの関係で+1する
	else if(m_Lin == 11)	Lin_tmp = KSELLINE12+1;	// calcの関係で+1する

	if( !mode ){
		if( m_Idx == SEL31NAMEex ){
			::ZeroMemory( buf, sizeof(buf));
			m_Util.LineDataGet( dbuf, buf, sizeof(dbuf) );
			memcpy( (*m_pTaxListData)->Head.KHkunm, buf, sizeof((*m_pTaxListData)->Head.KHkunm));
		}
		else{
			pK = &(*m_pTaxListData)->NRec.KUval[Lin_tmp];
			if( leng ){
					switch( Lin_tmp ){
					case KSELLINE1:
					case KSELLINE2:
					case KSELLINE3:
					/*case KSELLINE4:
					case KSELLINE5:
					case KSELLINE6:*/
						if(clm_pos == 0){
							pK->KKauto |= 0x01;
						}else if(clm_pos == 1){
							pK->KKauto |= 0x04;
						}else if (clm_pos == 2) {
							pK->KKauto |= 0x08;
						}
						break;
					case KSELLINE11+1:	// calcの関係で+1する
					case KSELLINE12+1:
						if( clm_pos == 0 ){
							pK->KKauto |= 0x01;
						}else if( clm_pos == 1 ){
							pK->KKauto |= 0x02;
						}else if( clm_pos == 2 ){
							pK->KKauto |= 0x04;
						}
						break;
					default:
						if( m_pArith->l_test(pData->data_val) ){
							if(clm_pos == 0){
								pK->KKauto |= 0x01;
							}else
							if(clm_pos == 1){
								pK->KKauto |= 0x04;
							}else
							if(clm_pos == 2){
								pK->KKauto |= 0x08;
							}
						}else{
							if(clm_pos == 0){
								pK->KKauto &= 0xfe;
							}else
							if(clm_pos == 1){
								pK->KKauto &= 0xfb;
							}else
							if(clm_pos == 2){
								pK->KKauto &= 0xf7;
							}
						}
						break;
					}

					if( !( m_Util.IsUnConnectMst(m_pZmsub->zvol) && (m_Util.GetKazeihoushiki(m_pZmsub->zvol)==ID_ICSSH_KANNI) ) ){// 非連動ｄｂで簡易課税じゃ無い場合
						;
					}else{
						switch( Lin_tmp ){
							case KSELLINE7:
							case KSELLINE8:
							case KSELLINE9:
								if(clm_pos == 0){
									pK->KKauto |= 0x01;
								}else
								if(clm_pos == 1){
									pK->KKauto |= 0x04;
								}else
								if(clm_pos == 2){
									pK->KKauto |= 0x08;
								}
								break;
							default:
								break;
						}
					}

					/*if(clm_pos == 0)		memcpy( pK->KKval1, pData->data_val, 6 );
					else if(clm_pos == 1)	memcpy( pK->KKval2, pData->data_val, 6 );
					else if(clm_pos == 2)	memcpy( pK->KKval3, pData->data_val, 6 );
					else if(clm_pos == 3)	memcpy( pK->KKval4, pData->data_val, 6 );*/

					switch( Lin_tmp ){
						case KSELLINE11+1:	// calcの関係で+1する
						case KSELLINE12+1:
							if(clm_pos == 0)		memcpy( pK->KKval1, pData->data_val, 6 );
							else if(clm_pos == 1)	memcpy( pK->KKval2, pData->data_val, 6 );
							else if(clm_pos == 2)	memcpy( pK->KKval3, pData->data_val, 6 );
							break;
						default:
							if(clm_pos == 0)		memcpy( pK->KKval1, pData->data_val, 6 );
							else if(clm_pos == 1)	memcpy( pK->KKval3, pData->data_val, 6 );
							else if(clm_pos == 2)	memcpy( pK->KKval4, pData->data_val, 6 );
							break;
					}
			}
		}
	}
	else{
		if( m_Idx == SEL31NAMEex ){
			::ZeroMemory( (*m_pTaxListData)->Head.KHkunm, 6 );
		}
		else{
			pK = &(*m_pTaxListData)->NRec.KUval[Lin_tmp];
			if (m_Idx == SEL3111Aex) {
				pK->KKauto &= 0xfb;
				::ZeroMemory(pK->KKval3, 6);
			}
			else if (m_Idx == SEL3111Bex) {
				pK->KKauto &= 0xfd;
				::ZeroMemory( pK->KKval2, 6 );
			}
			else if (m_Idx == SEL3112Aex) {
				pK->KKauto &= 0xfb;
				::ZeroMemory(pK->KKval3, 6);
			}
			else if( m_Idx == SEL3112Bex ){
				pK->KKauto &= 0xfd;
				::ZeroMemory( pK->KKval2, 6 );
			}
			else{
				/*if(clm_pos == 0){
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
				}else
				if (clm_pos == 3) {
					pK->KKauto &= 0xf7;
					::ZeroMemory( pK->KKval4, 6 );
				}*/
				if(clm_pos == 0){
					pK->KKauto &= 0xfe;
					::ZeroMemory( pK->KKval1, 6 );
				}else
				if(clm_pos == 1){
					pK->KKauto &= 0xfb;
					::ZeroMemory( pK->KKval3, 6 );
				}else
				if (clm_pos == 2) {
					pK->KKauto &= 0xf7;
					::ZeroMemory( pK->KKval4, 6 );
				}
			}
		}
	}

	// 集計
	(*m_Scalc)->Shz_Sale31Post();
	DispSell31DiagData();


	return 0;
}

//-----------------------------------------------------------------------------
// フォーカス範囲？
//-----------------------------------------------------------------------------
void CShinSell31Ex::SetFocusRange()
{
	
	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SEL31CNTex; idx++ ){
		m_Sell31.GetAttribute(FCS_Sell31Ex[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_Sell31Ex[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SEL31CNTex; idx++ ){
		m_Sell31.GetAttribute(FCS_Sell31Ex_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_Sell31Ex_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SEL31CNTex-1; idx > -1 ; idx-- ){
		m_Sell31.GetAttribute(FCS_Sell31Ex[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd =FCS_Sell31Ex[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SEL31CNTex-1; idx > -1 ; idx-- ){
		m_Sell31.GetAttribute(FCS_Sell31Ex_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd =FCS_Sell31Ex_LR[idx].IDNo;
			break;
		}
	}
}



// CShinSell31Ex メッセージ ハンドラー
BEGIN_EVENTSINK_MAP(CShinSell31Ex, CSyzBaseDlg)
	ON_EVENT(CShinSell31Ex, IDC_ICSDIAGCTRL1, 1, CShinSell31Ex::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinSell31Ex, IDC_ICSDIAGCTRL1, 2, CShinSell31Ex::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinSell31Ex, IDC_ICSDIAGCTRL1, 3, CShinSell31Ex::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
// 22-0167,22-0361 add -->
	ON_EVENT(CShinSell31Ex, IDC_ICSDIAGCTRL1, 9, CShinSell31Ex::CheckButtonIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
// 22-0167,22-0361 add <--
END_EVENTSINK_MAP()


void CShinSell31Ex::EditONIcsdiagctrl1(short index)
{
	m_Sell31.OffControl( FALSE );

	m_Idx = index;
	if( (m_Idx >= SKVMINex) && (m_Idx <= SKVMAXex) ){
		/*if( m_Idx == SKVMAX ){
			m_Lin = ( (m_Idx - SKVMIN) / 3 ) - 1;
		}*/
		if(m_Idx == SEL3111Cex){	// インデックスの数字を変更するとき注意
			m_Lin = 10;
		}
		else if (m_Idx == SEL3112Cex) {
			m_Lin = 11;
		}
		else{
			m_Lin = (m_Idx - SKVMINex) / 3;
		}
	}
	else{
		if ( (m_Idx == SEL3111Aex) || (m_Idx == SEL3111Bex)) {
			m_Lin = 10;
		}
		else if ((m_Idx == SEL3112Aex) || (m_Idx == SEL3112Bex)) {
			m_Lin = 11;
		}
		else {
			m_Lin = -1;
		}
		
	}

	m_curwnd = IDC_ICSDIAGCTRL1;
}


void CShinSell31Ex::EditOFFIcsdiagctrl1(short index)
{
	DIAGRAM_DATA	data;
	diag_getdata( IDC_ICSDIAGCTRL1, index, &data, CIcsdiagctrl );
	int	st = GetDiagData( index, data.data_check, &data, 0 );
	diag_deledit( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
	if( st < 0 ){
		diag_setposition( IDC_ICSDIAGCTRL1, m_Idx, CIcsdiagctrl );
	}
}


void CShinSell31Ex::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
{

	if( nChar == 0 ){
		return;
	}

	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;

	UINT	flg = 0;

	idata = (DIAGRAM_DATA *)data;
	m_Util.DiagOcxIniz( &xdata );
	m_Util.MoveDiagData( &xdata, idata );


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
		m_Sell31.SetPosition(pos);	
	}
	if( wTerm==VK_DOWN ){
		pos = FocusMoveCheck(index , 0);
		m_Sell31.SetPosition(pos);
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
		pos = FocusMoveCheck(index , 2);
		m_Sell31.SetPosition(pos);	
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		pos = FocusMoveCheck(index , 3);
		m_Sell31.SetPosition(pos);	
	}

	if( wTerm == VK_DELETE){
		GetDiagData( index, length, &xdata, -1 );
	}

	SetRedrawScrollToIdx(m_Sell31.GetPosition(), IDC_ICSDIAGCTRL1);

/*	int nowpos = m_SellKei.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL1, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}*/
}


BOOL CShinSell31Ex::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}

			return TRUE;
		}
		else if( pMsg->wParam == VK_F2 ){
			DIAGRAM_DATA	DD;
			if( m_curwnd != -1 ){
				short result = ((CIcsdiagctrl*)GetDlgItem(m_curwnd))->GetPosition();
				diag_getdata( m_curwnd, result, &DD, CIcsdiagctrl );
				if( m_curwnd == IDC_ICSDIAGCTRL1 ){
					TerminationIcsdiagctrl1( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&DD );
				}
				return TRUE;
			}
		}
	}


	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// 印刷情報のセット
//-----------------------------------------------------------------------------
// 引数	pVprn			：	印刷モジュール
//		pPset			：	印刷情報セット
//		pPrintCmInfo	：	印刷情報
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
void CShinSell31Ex::SetPrintInfo( CVPrint *pVprn, NPRT_PAR *pPset , SH_PRINT_CMINFO *pPrintCmInfo )
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
int CShinSell31Ex::PrintHyo1( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j;
	char	cnm[128], buf[128], CVAL[6];
	int		len;
	KKVAL*	pS = (*m_pTaxListData)->NRec.KUval;

	// 初期化
	memset( PF, '\0', sizeof(PF) );


	i = 0;
	memset( pBp, '\0', Msizex );
	char	*pBpMv = pBp;
	::ZeroMemory( CVAL, sizeof(CVAL) );

	// 会社名
	::ZeroMemory( buf, sizeof(buf) );
	::ZeroMemory( cnm, sizeof(cnm) );

	if( m_pPset->psSigCoprn&0x2 ){
		// 顧問先
		if( m_PrintCmInfo->OPL ){	// なんでどっちも閉じてるのか？
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
	/*
	if( Is30PlusPrint() ){	// <--Is30PlusPrintをBaseDlgに追加する
		sprintf_s(buf, sizeof(buf), _T("%x"), (*m_pTaxListData)->YEAR+R_TO_H_ADNEN);
	}
	else{
		sprintf_s(buf, sizeof(buf), _T("%x"), (*m_pTaxListData)->YEAR);
	}*/
	sprintf_s(buf, sizeof(buf), _T("%x"), (*m_pTaxListData)->YEAR);
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
int CShinSell31Ex::PrintHyo2( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, j, lin, max;
	char	buf[128], CVAL[6];
	int		len, st;
	KKVAL*	pK;
	KKVAL*	pS = (*m_pTaxListData)->NRec.KUval;

	// 初期化
	memset( PF, '\0', sizeof(PF) );


	i = 0;
	memset( pBp, '\0', Msizex );
	char	*pBpMv = pBp;
	::ZeroMemory( CVAL, sizeof(CVAL) );

	// 会社名
	::ZeroMemory( buf, sizeof(buf) );


	// 事業所得に係る課税仕入高
	// 金額
	lin = 0;	max = 2;
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );

		if( DispJudge( 0, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

	//うち税率6.3％適用分
	/*for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );

		if( DispJudge( 1, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;*/

	//うち税率6.24％適用分
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );

		//if( DispJudge( 2, l_idx+lin ) == 1 ){
		if( DispJudge( 1, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

	//うち税率7.8％適用分
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		//if (DispJudge(3, l_idx + lin) == 1) {
		if (DispJudge(2, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval4, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;

	// 不動産所得に係る課税売上高
	// 金額
	lin = 2;	max = 3;
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf));

		if( DispJudge( 0, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

	//うち税率6.3％適用分
	/*for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf));

		if( DispJudge( 1, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;*/

	//うち税率6.24％適用分
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf));

		//if( DispJudge( 2, l_idx+lin ) == 1 ){
		if( DispJudge( 1, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

	//うち税率7.8％適用分
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		//if (DispJudge(3, l_idx + lin) == 1) {
		if (DispJudge(2, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval4, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;


	// (     )に係る課税売上高
	// 金額
	lin = 3;	max = 6;
	for( j=0; (j+lin) < max; j++ ){
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );

		if( DispJudge( 0, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

	//うち税率6.3％適用分
	/*for( j=0; (j+lin) < max; j++ ){
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );

		if( DispJudge( 1, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;*/

	//うち税率6.24％適用分
	for( j=0; (j+lin) < max; j++ ){
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );

		//if( DispJudge( 2, l_idx+lin ) == 1 ){
		if( DispJudge( 1, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

	//うち税率7.8％適用分
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		//if (DispJudge(3, l_idx + lin) == 1) {
		if (DispJudge(2, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval4, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;


	// 業務用資産の譲渡所得に係る課税売上高
	// 金額
	lin = 6;	max = 9;
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );

		if( DispJudge( 0, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval1, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

	//うち税率6.3％適用分
	/*for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );

		if( DispJudge( 1, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval2, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;*/

	//うち税率6.24％適用分
	for( j=0; (j+lin)<max; j++ ){
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx+lin];
		st = 0;

		::ZeroMemory( buf, sizeof(buf) );

		//if( DispJudge( 2, l_idx+lin ) == 1 ){
		if( DispJudge( 1, l_idx+lin ) == 1 ){
			m_pArith->l_print( buf, pK->KKval3, FMTX130 );
		}
		len = (int)strlen( buf );
		memmove( pBpMv, buf, len );
		PF[i].PITM[j] = pBpMv;
		pBpMv += ( len+1 );
	}
	i++;

	//うち税率7.8％適用分
	for (j = 0; (j + lin) < max; j++) {
		int l_idx = -1;
		l_idx = j;

		pK = &(*m_pTaxListData)->NRec.KUval[l_idx + lin];
		st = 0;

		::ZeroMemory(buf, sizeof(buf));

		//if (DispJudge(3, l_idx + lin) == 1) {
		if (DispJudge(2, l_idx + lin) == 1) {
			m_pArith->l_print(buf, pK->KKval4, FMTX130);
		}
		len = (int)strlen(buf);
		memmove(pBpMv, buf, len);
		PF[i].PITM[j] = pBpMv;
		pBpMv += (len + 1);
	}
	i++;


	// 課税売上高の合計
	// 金額
	pK = &(*m_pTaxListData)->NRec.KUval[KSELLINE10];
	::ZeroMemory( buf, sizeof( buf ));

	if( DispJudge( 0, KSELLINE10 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );


	//うち税率6.3％適用分
	/*pK = &(*m_pTaxListData)->NRec.KUval[KSELLINE10];
	::ZeroMemory( buf, sizeof( buf ));

	if( DispJudge( 1, KSELLINE10 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval2, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );*/

	//うち税率6.24％適用分
	pK = &(*m_pTaxListData)->NRec.KUval[KSELLINE10];
	::ZeroMemory( buf, sizeof( buf ));

	//if( DispJudge( 2, KSELLINE10 ) == 1 ){
	if( DispJudge( 1, KSELLINE10 ) == 1 ){
		m_pArith->l_print( buf, pK->KKval3, FMTX130 );
	}

	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

	//うち税率7.8％適用分
	pK = &(*m_pTaxListData)->NRec.KUval[KSELLINE10];
	::ZeroMemory(buf, sizeof(buf));

	//if (DispJudge(3, KSELLINE10) == 1) {
	if (DispJudge(2, KSELLINE10) == 1) {
		m_pArith->l_print(buf, pK->KKval4, FMTX130);
	}

	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (len + 1);


	// 課税標準額の計算
	// B欄の金額
	/*st = 0;
	pK = &(*m_pTaxListData)->NRec.KUval[KSELLINE11];

	::ZeroMemory( buf, sizeof(buf) );

	if( DispJudge( 0, KSELLINE11 ) == 1 ){ //列？
		m_pArith->l_print( buf, pK->KKval2, FMTX130 );
	}

	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

	::ZeroMemory( buf, sizeof( buf ));

	if( DispJudge( 0, KSELLINE11 ) == 1 ){ //列？
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}

	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );*/

	// C欄の金額
	st = 0;
	pK = &(*m_pTaxListData)->NRec.KUval[KSELLINE11+1];	// calcの関係で+1する

	::ZeroMemory( buf, sizeof(buf) );

	if( DispJudge( 0, KSELLINE11+1 ) == 1 ){ //列？
		m_pArith->l_print( buf, pK->KKval2, FMTX130 );
	}
	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

	::ZeroMemory( buf, sizeof( buf ));

	if( DispJudge( 0, KSELLINE11+1 ) == 1 ){ //列？
		m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	}

	len = (int)strlen( buf );
	memmove( pBpMv, buf, len );
	PF[i++].PITM[0] = pBpMv;
	pBpMv += ( len+1 );

	// D欄の金額
	st = 0;
	pK = &(*m_pTaxListData)->NRec.KUval[KSELLINE12+1];	// calcの関係で+1する

	::ZeroMemory(buf, sizeof(buf));

	if (DispJudge(0, KSELLINE12+1) == 1) { //列？
		m_pArith->l_print(buf, pK->KKval2, FMTX130);
	}
	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (len + 1);

	::ZeroMemory(buf, sizeof(buf));

	if (DispJudge(0, KSELLINE12+1) == 1) { //列？
		m_pArith->l_print(buf, pK->KKval1, FMTX130);
	}

	len = (int)strlen(buf);
	memmove(pBpMv, buf, len);
	PF[i++].PITM[0] = pBpMv;
	pBpMv += (len + 1);


	//// 合計
	//pK = &(*m_pTaxListData)->NRec.KUval[KSELLINE16];
	//::ZeroMemory( buf, sizeof( buf ));

	//if( DispJudge( 0, KSELLINE16 ) == 1 ){ //列？
	//	m_pArith->l_print( buf, pK->KKval1, FMTX130 );
	//}

	//len = (int)strlen( buf );
	//memmove( pBpMv, buf, len );
	//PF[i].PITM[0] = pBpMv;
	//pBpMv += ( len+1 );


	int fontsize = 0;
	char val[5][128];
	CString str;
	for(int l_cnt = KSELLINE1; l_cnt <= KSELLINE12+1; l_cnt++){
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
			case KSELLINE11+1:	// calcの関係で+1する
			case KSELLINE12+1:
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
		return _xprn( pno, fno1_sex, PF, 0, pDC, 0, 0 );
	}

	return _xprn( pno, fno, PF, 0, pDC, 0, 0 );

}



//-----------------------------------------------------------------------------
// 印刷３
//-----------------------------------------------------------------------------
// 引数	pno		：	出力指定表No.
//		fno		：	フィールドNo.
//		pDC		：	デバイスコンテキスト
//		pBp		：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinSell31Ex::PrintHyo3( int pno, int fno, CDC *pDC, char *pBp )
{
	int		i, len;
	char	buf[64], dbuf[64];

	i = 0;
	memset( pBp, '\0', Msizex );

	char	*pBpMv = pBp;

	memset( PF, '\0', sizeof(PF) );

	// 項目名
	::ZeroMemory( buf,sizeof(buf) );
	::ZeroMemory( dbuf,sizeof(dbuf) );
	memmove( buf, (*m_pTaxListData)->Head.KHkunm, sizeof((*m_pTaxListData)->Head.KHkunm) );
	len = (int)strlen(buf);
	if (len > 8) {
		memmove(pBpMv, buf, len);
		PF[i++].PITM[0] = pBpMv;
		pBpMv += (len + 1);

		memmove(pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHkunm));
		PF[i].PITM[0] = pBpMv;

		pBpMv += (sizeof((*m_pTaxListData)->Head.KHkunm) + 1);

		memmove(pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHkunm));
		PF[i++].PITM[0] = pBpMv;

		pBpMv += (sizeof((*m_pTaxListData)->Head.KHkunm) + 1);
	}
	else if( len > 6 ){
		memmove(pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHkunm));
		PF[i++].PITM[0] = pBpMv;

		pBpMv += (sizeof((*m_pTaxListData)->Head.KHkunm) + 1);

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

		memmove(pBpMv, dbuf, sizeof((*m_pTaxListData)->Head.KHkunm));
		PF[i++].PITM[0] = pBpMv;

		pBpMv += (sizeof((*m_pTaxListData)->Head.KHkunm) + 1);
		memmove( pBpMv, buf, len);
		PF[i].PITM[0] = pBpMv;	
		pBpMv += (len+1);
	}


	return( _xprn( pno, 4, PF, 0, pDC, 0 ));
}

//-------------------------------------------------------------
// 手入力サインのクリア
//-------------------------------------------------------------
int CShinSell31Ex::ClearManualInputSign()
{
	int rt = 0;

	for( int idx = 0; idx < 40; idx++ ){
		(*m_pTaxListData)->NRec.KUval[idx].KKauto = 0x00;
	}

	//事業所得の金額をクリアする
	//金額クリア後に金額を連動してくる為
	for( int i=0; i<3; i++ ){
		memset( (*m_pTaxListData)->NRec.KUval[i].KKval1, '\0', sizeof((*m_pTaxListData)->NRec.KUval[i].KKval1) );
		memset((*m_pTaxListData)->NRec.KUval[i].KKval2, '\0', sizeof((*m_pTaxListData)->NRec.KUval[i].KKval2));
		memset((*m_pTaxListData)->NRec.KUval[i].KKval3, '\0', sizeof((*m_pTaxListData)->NRec.KUval[i].KKval3));
		memset((*m_pTaxListData)->NRec.KUval[i].KKval4, '\0', sizeof((*m_pTaxListData)->NRec.KUval[i].KKval4));
	}

	return rt;
}

// 22-0167,22-0361 add -->
void CShinSell31Ex::CheckButtonIcsdiagctrl1(short index, LPUNKNOWN data)
{
	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;
	int				idx=0;
	//int				mode=0;
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
		//short	pos = m_Sell31.GetPosition();
		m_Sell31.DeleteInput();
		diag_getdata(IDC_ICSDIAGCTRL1,index,&xdata,CIcsdiagctrl);
		if(!idata->data_check)	xdata.data_check = 1;
		else					xdata.data_check = 0;
		diag_setdata(IDC_ICSDIAGCTRL1,index,&xdata,CIcsdiagctrl);
		//m_Sell31.SetPosition(pos);
		SetItemPositionEx();
		return;
	}
	//KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;
	//(*m_pTaxListData)->NRec.KUval[3].KKauto &= 0x2f;	// フラグの上位を一旦クリア
	(*m_pSnHeadData)->Sn_GenericSgn &= 0xfffff7ff;

	// チェックON
	if(idata->data_check)	{
		//// 自身以外のチェックをOFFにする
		//if(index == SELChk1ex)	{
		//	for(idx=SELChk2ex; idx<=SELChk3ex; idx++)	{
		//		diag_getdata(IDC_ICSDIAGCTRL1,idx,&xdata,CIcsdiagctrl);
		//		xdata.data_check = 0;
		//		diag_setdata(IDC_ICSDIAGCTRL1,idx,&xdata,CIcsdiagctrl);
		//	}
		//	(*m_pTaxListData)->NRec.KUval[3].KKauto |= 0x10;
		//}
		//else if(index == SELChk2ex)	{
		//	for(idx=SELChk1ex; idx<=SELChk3ex; idx+=2)	{
		//		diag_getdata(IDC_ICSDIAGCTRL1,idx,&xdata,CIcsdiagctrl);
		//		xdata.data_check = 0;
		//		diag_setdata(IDC_ICSDIAGCTRL1,idx,&xdata,CIcsdiagctrl);
		//	}
		//	(*m_pTaxListData)->NRec.KUval[3].KKauto |= 0x40;
		//}
		//else if(index == SELChk3ex)	{
		//	for(idx=SELChk1ex; idx<=SELChk2ex; idx++)	{
		//		diag_getdata(IDC_ICSDIAGCTRL1,idx,&xdata,CIcsdiagctrl);
		//		xdata.data_check = 0;
		//		diag_setdata(IDC_ICSDIAGCTRL1,idx,&xdata,CIcsdiagctrl);
		//	}
		//	(*m_pTaxListData)->NRec.KUval[3].KKauto |= 0x80;
		//}
		(*m_pSnHeadData)->Sn_GenericSgn |= 0x800;
	}
	else	{
		//mode=1;
		// 修正依頼No.161181対応
		if(m_Util.IsUnConnectMst(m_pZmsub->zvol) && (m_Util.GetKazeihoushiki(m_pZmsub->zvol) == ID_ICSSH_KANNI))	{
			KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;
			m_pArith->l_clear((pK+KSELLINE8)->KKval4);
			(pK+KSELLINE8)->KKauto &= 0xf7;
		}
	}
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
	m_Sell31.OffControl(TRUE);
	DispSell31DiagData();
	// カーソルの終点を決める
	SetFocusRange();
	// カーソルセット
	SetItemPositionEx();
}
// 22-0167,22-0361 add <--

// 修正No.168690 add -->
void CShinSell31Ex::DiagReSetPosition(void)
{
	//this->SetFocus();
}
// 修正No.168690 add <--
