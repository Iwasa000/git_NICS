// ShinFhyo60.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo6.h"
#include "ShinFhyo6Idx.h"

#include "H31HyoView.h"

// CShinFhyo6 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo6, CSyzBaseDlg)

//----------------------------------------------------------------------------------------------------
// マイナンバー入力モジュールコールバック関数 マイナンバー対応(高木正)('16.01.29)
//----------------------------------------------------------------------------------------------------
int MNCallback(void* wnd, int index, int mes_id, MSG* pMsg)
{
	if(mes_id == ID_MN_MESSAGE){	// ICSMNEditのPretranslateMessageでコールされた場合
		if( wnd && IsWindow(((CShinFhyo6*)wnd)->m_hWnd) && pMsg ){
			TRACE("MNCallback ID_MN_MESSAGE\n");
			return (((CShinFhyo6*)wnd)->OnTerminationCallBack( index, (short)pMsg->wParam, 0, NULL ));
		}
	}
	else if(mes_id == ID_MN_EDIT_ON){ // ICSMNEditのEditOnでコールされた場合
		if( wnd && IsWindow(((CShinFhyo6*)wnd)->m_hWnd) ){
			TRACE("MNCallback ID_MN_EDIT_ON\n");
			return ((CShinFhyo6*)wnd)->OnEditONCallBack( index );
		}
	}
	else if(mes_id == ID_MN_EDIT_OFF){ // ICSMNEditのEditOffでコールされた場合
		if( wnd && IsWindow(((CShinFhyo6*)wnd)->m_hWnd) ){
			TRACE("MNCallback ID_MN_EDIT_OFF\n");
	 		return ((CShinFhyo6*)wnd)->OnEditOFFCallBack( index );
		}
	}
	return FALSE;
}

CShinFhyo6::CShinFhyo6(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo6::IDD, pParent)
	, m_pSnHeadData(NULL)
	, m_pSnHonpyoData(NULL)
	, m_pSnFhyo1_01Data(NULL)
	, m_pSnFhyo4_01Data(NULL)
	, m_pSnFhyo6Data(NULL)
	, m_pParent(pParent)
	, m_pZmSub(NULL)
	, m_EditOff_sw(0)
	, m_Lin(0)
	, m_Focus(0)
	, m_MyNum_Haiki_idx(0)
	, m_MyNum_Delete_idx(0)
	, mynum_cursel(0)
	, Sn_F6_USE ( 0 )
	, m_SnSeq(0)
	, m_pDBSyzAcc(NULL)
{
	memset( XM8, '\0', sizeof(XM8) );
	memset( XM11, '\0', sizeof(XM11) );
	memset( XM12, '\0', sizeof(XM12) );
	memset( XM13, '\0', sizeof(XM13) );
	memset( XM19, '\0', sizeof(XM19) );
	memset( XM22, '\0', sizeof(XM22) );
	memset( XM23, '\0', sizeof(XM23) );
	memset( XM24, '\0', sizeof(XM24) );
	memset( XM26, '\0', sizeof(XM26) );

	memset( F61, '\0', sizeof(F61) );
	memset( F62, '\0', sizeof(F62) );
	memset( F63, '\0', sizeof(F63) );
	memset( F64, '\0', sizeof(F64) );
	memset( F65, '\0', sizeof(F65) );
	memset( F66, '\0', sizeof(F66) );
	for( int cnt=0; cnt < SOZOKU_MAX; cnt++ ){
		memset( F69[cnt], '\0', sizeof(F69[cnt]) );
		memset( F610[cnt], '\0', sizeof(F610[cnt]) );
		memset( F611[cnt], '\0', sizeof(F611[cnt]) );
		//memset( F612[cnt], '\0', sizeof(F612[cnt]) );
		//memset( F613[cnt], '\0', sizeof(F613[cnt]) );
		//memset( F614[cnt], '\0', sizeof(F614[cnt]) );
	}

	memset ( Sn_F6_ADR, '\0', sizeof ( Sn_F6_ADR ) );
	memset ( Sn_F6_SONOTA, '\0', sizeof ( Sn_F6_SONOTA ) );

	MyNumber_DelHaiki_Clear();
}

CShinFhyo6::~CShinFhyo6()
{
	if( m_pDBSyzAcc ){
		if( m_pDBSyzAcc->m_pShinPayment ){
			m_pDBSyzAcc->ShinPayMentClose();
		}
		delete m_pDBSyzAcc;
		m_pDBSyzAcc = NULL;
	}
}

void CShinFhyo6::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Fhyo6Diag);
	DDX_Control(pDX, IDC_EDIT_MYNUM1, m_edit_mynum1);
	DDX_Control(pDX, IDC_EDIT_MYNUM2, m_edit_mynum2);
	DDX_Control(pDX, IDC_EDIT_MYNUM3, m_edit_mynum3);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_bar);
}


BEGIN_MESSAGE_MAP(CShinFhyo6, CSyzBaseDlg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_MYNUM_DEL1, &CShinFhyo6::OnBnClickedMynumDel1)
	ON_BN_CLICKED(IDC_MYNUM_DEL2, &CShinFhyo6::OnBnClickedMynumDel2)
	ON_BN_CLICKED(IDC_MYNUM_DEL3, &CShinFhyo6::OnBnClickedMynumDel3)
	ON_BN_CLICKED(IDC_MYNUM_DEL4, &CShinFhyo6::OnBnClickedMynumDel4)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SOZOKU_DEL, &CShinFhyo6::OnBnClickedSozokuDel)
	ON_BN_CLICKED(IDC_SOZOKU_INS, &CShinFhyo6::OnBnClickedSozokuIns)
END_MESSAGE_MAP()


// CShinFhyo6 メッセージ ハンドラ

BOOL CShinFhyo6::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期化
	Init();

	m_pBottomDiag = &m_Fhyo6Diag;
	m_BottomIdx = ID_BOTTOM;

	mnedit_brush.CreateSolidBrush( BC_CREAM );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

int CShinFhyo6::InitInfo( CH31SnFhyo6Data **pSnFhyo6Data, CH31SnFhyo1_01Data **pSnFhyo1_01Data, CH31SnFhyo4_01Data **pSnFhyo4_01Data, CSnHeadData **pSnHeadData, CH31SnHonpyoData **pSnHonpyoData, CArithEx *pArith, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, int SnSeq )
//int CShinFhyo6::InitInfo( CH31SnFhyo6Data **pSnFhyo6Data, CH31SnFhyo1_01Data **pSnFhyo1_01Data, CH31SnFhyo4_01Data **pSnFhyo4_01Data, CSnHeadData **pSnHeadData, CArithEx *pArith, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo )
{
	ASSERT( pSnFhyo6Data );
	if( pSnFhyo6Data == NULL ){
		return -1;
	}
	m_pSnFhyo6Data = pSnFhyo6Data;	

	ASSERT( pSnFhyo1_01Data );
	if( pSnFhyo1_01Data == NULL ){
		return -1;
	}
	m_pSnFhyo1_01Data = pSnFhyo1_01Data;

	ASSERT( pSnFhyo4_01Data );
	if( pSnFhyo4_01Data == NULL ){
		return -1;
	}
	m_pSnFhyo4_01Data = pSnFhyo4_01Data;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pSnHonpyoData );
	if( pSnHonpyoData == NULL ){
		return -1;
	}
	m_pSnHonpyoData = pSnHonpyoData;

	ASSERT( pArith );
	if( pArith == NULL ){
		return -1;
	}
	m_pArith = pArith;
//Arith.l_defn(0x16);
//m_pArith = &Arith;

	ASSERT( pZmSub );
	if( pZmSub == NULL ){
		return -1;
	}
	m_pZmSub = pZmSub;

	ASSERT( pShinInfo );
	if( pShinInfo == NULL ){
		return -1;
	}
	m_pShinInfo = pShinInfo;

	m_SnSeq = SnSeq;

	m_pDBSyzAcc = new CDBSyzAccess;
	if( m_pDBSyzAcc == NULL ){
		return -1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo6::Init()
{

	if( ((*m_pSnFhyo6Data)==NULL) || (((*m_pSnFhyo1_01Data)==NULL)&&((*m_pSnFhyo4_01Data)==NULL)) || ((*m_pSnHonpyoData)==NULL) ){
//	if( ((*m_pSnFhyo6Data)==NULL) || (((*m_pSnFhyo1_01Data)==NULL)&&((*m_pSnFhyo4_01Data)==NULL))){
		return -1;
	}

	InitMyNumberData();
	
	memset( Sn_F6_DEATH, '\0', sizeof(Sn_F6_DEATH));
	Sn_F6_DAIHYO.Empty();
	Sn_F6_LIMIT = 0;
	for( int cnt=0; cnt < SOZOKU_MAX; cnt++ ){
		Init_sozoku( cnt );
	}

	ReadData();

	SCROLLINFO SCI;
	SCI.cbSize	= 0;	SCI.fMask	= SIF_ALL;
	SCI.nMin	= 0;	SCI.nMax	= SOZOKU_MAX-1;
	SCI.nPage	= 4;	SCI.nPos	= 0;
	m_bar.SetScrollInfo( &SCI );
	m_bar.SetScrollPos(m_Lin);

	// 納める消費税～
	f6calq_1to6();

	if( m_pArith->l_test(XM26) > 0 ){
		for( int idx = 0; idx < SOZOKU_MAX; idx++ ){
			f6calq_9to14(idx);
			if( !(sozoku[idx].Sn_F6_NOFUKANPUsw&0x01)){
				memmove( sozoku[idx].Sn_F6_NOFUZEI, F69[idx], MONY_BUF_SIZE );
			}
			if( !(sozoku[idx].Sn_F6_NOFUKANPUsw&0x02)){
				memmove( sozoku[idx].Sn_F6_NOFUCHIHO, F610[idx], MONY_BUF_SIZE );
			}
		}
	}

	// ダイアグラム初期属性セット
	InitDiagAttr();
	// 付表６
	DispFh6();

//2018.08.21 INSERT START
	int line = GetLineNumber_FromIndex(m_Focus);
	if( line >= 0 ){
		m_ctrl_mynum.SetData( sozoku[line].Sn_F6_MYNUM1+sozoku[line].Sn_F6_MYNUM2+sozoku[line].Sn_F6_MYNUM3);
	}
//2018.08.21 INSERT END
	return 0;
}

void CShinFhyo6::InitMyNumberData()
{
	NTBMOD8_BeginProc( 0, NULL );

	HWND edit[10] = {NULL};

	edit[0] = m_edit_mynum1.GetSafeHwnd();
	edit[1] = m_edit_mynum2.GetSafeHwnd();
	edit[2] = m_edit_mynum3.GetSafeHwnd();

	int diag_index[5][10] = {{-1},{-1},{-1},{-1},-1};

	diag_index[0][0] = MYNUM1_1;
	diag_index[0][1] = MYNUM1_2;
	diag_index[0][2] = MYNUM1_3;
	int cnt = 0;
	for(cnt=3;cnt<10;cnt++){
		diag_index[0][cnt] = -1;
	}
	diag_index[1][0] = MYNUM2_1;
	diag_index[1][1] = MYNUM2_2;
	diag_index[1][2] = MYNUM2_3;
	for(cnt=3;cnt<10;cnt++){
		diag_index[1][cnt] = -1;
	}
	diag_index[2][0] = MYNUM3_1;
	diag_index[2][1] = MYNUM3_2;
	diag_index[2][2] = MYNUM3_3;
	for(cnt=3;cnt<10;cnt++){
		diag_index[2][cnt] = -1;
	}
	diag_index[3][0] = MYNUM4_1;
	diag_index[3][1] = MYNUM4_2;
	diag_index[3][2] = MYNUM4_3;
	for(cnt=3;cnt<10;cnt++){
		diag_index[3][cnt] = -1;
	}

	//マイナンバーコントロールを初期化
	m_ctrl_mynum.Init(this, MN_KOJIN, (*m_pSnHeadData)->m_apno, TRUE, edit, m_Fhyo6Diag.GetSafeHwnd(), diag_index);

	m_ctrl_mynum.SetDisableColor(BC_GRAY);//disableの背景色セット
	m_ctrl_mynum.SetHeightMargin(MN_HEIGHT_BOTTOM);
	m_ctrl_mynum.SetCallbackFunction(MNCallback);
	//m_ctrl_mynum.SetFrameColor(BC_GRAY2);//枠色セット
//	m_ctrl_mynum.ShowWindow(SW_SHOW);

	NTBMOD8_EndProc();

	return;
}

int CShinFhyo6::MyNumberCheck( int sgn )
{
	int accesPrm = MynoAccessPerm();										//マイナンバー権限
	int isMNAble = NTBMOD8_IsAbleToMyNumber( (*m_pSnHeadData)->m_apno, 0 );	//マイナンバー有償・無償
	if( !accesPrm||!isMNAble ){
		return 0;
	}

	int ret = 0;

	//　マイナンバー重複チェック
	for( int idx1 = 0; idx1 < SOZOKU_MAX-1; idx1++ ){
		if( sozoku[idx1].Sn_F6_MYNUM1.IsEmpty()){
			continue;
		}
		if( sozoku[idx1].Sn_F6_MYNUM1.Compare("****") == 0 ){
			continue;
		}
		for( int idx2 = idx1+1; idx2 < SOZOKU_MAX; idx2++ ){
			BOOL flg = TRUE;
			if( sozoku[idx1].Sn_F6_MYNUM1.Compare( sozoku[idx2].Sn_F6_MYNUM1 ) != 0 ){
				flg = FALSE;
			}
			if( sozoku[idx1].Sn_F6_MYNUM2.Compare( sozoku[idx2].Sn_F6_MYNUM2 ) != 0 ){
				flg = FALSE;
			}
			if( sozoku[idx1].Sn_F6_MYNUM3.Compare( sozoku[idx2].Sn_F6_MYNUM3 ) != 0 ){
				flg = FALSE;
			}
			if( flg == TRUE ){
				ret = -1;
				break;
			}
		}
	}

	if( ret == -1 ){
		if( sgn == 0 ){
			// インボイス施工日以降は準確定の付表６を付表７へ名称変更
			long symd = 0, eymd = 0;
			((CH31HyoView*)m_pParent)->GetKazeiKikan( symd, eymd );
			if( eymd >= ICS_SH_INVOICE_DAY ){
				ICSMessageBox( "付表７　相続人等に関する事項\n個人番号が重複しています。", MB_ICONSTOP );
			}
			else{
				ICSMessageBox( "付表６　相続人等に関する事項\n個人番号が重複しています。", MB_ICONSTOP );
			}
		}
	}

	return ret;
}

int CShinFhyo6::SozokubunCheck( int sgn )
{
	int num1=0, num2=0;
	int bunshi_t=0, bunbo_t=0,bunbo=0;
	int gcd=0,lcm=0;
	int ret = 0;

	for( int index = 0; index < SOZOKU_MAX; index++ ){

		BOOL bunbo_zero = FALSE;
		if(( sozoku[index].Sn_F6_BUNBO.IsEmpty() )||(atoi(sozoku[index].Sn_F6_BUNBO)==0)){
			bunbo_zero = TRUE;
		}
		BOOL bunshi_zero = FALSE;
		if(( sozoku[index].Sn_F6_BUNSHI.IsEmpty() )||(atoi(sozoku[index].Sn_F6_BUNSHI)==0)){
			bunshi_zero = TRUE;
		}


		//分子のみ、分母のみが１つでもあれば合計不一致として返す
		if( bunbo_zero&&!bunshi_zero ){
			//分子のみ
			ret =  -1;
			break;
		}
		if( !bunbo_zero&&bunshi_zero ){
			//分母のみ
			ret =  -1;
			break;
		}

		if( bunbo_zero&&bunshi_zero ){
			continue;
		}
		
		if(( bunshi_t == 0 )||( bunbo_t == 0 )){
			bunshi_t = atoi( sozoku[index].Sn_F6_BUNSHI );
			bunbo_t  = atoi( sozoku[index].Sn_F6_BUNBO );
		}else{
			bunbo = atoi( sozoku[index].Sn_F6_BUNBO );
			gcd = get_gcd( bunbo_t, bunbo );

			num1 = ((bunbo_t*bunbo)/gcd)/bunbo;
			num2 = ((bunbo_t*bunbo)/gcd)/bunbo_t;

			bunbo_t *= num2;
			bunshi_t *= num2;
			//bunbo*=num1;
			bunshi_t += atoi( sozoku[index].Sn_F6_BUNSHI ) * num1;

			gcd = get_gcd( bunshi_t, bunbo_t );
			bunshi_t /= gcd;
			bunbo_t /= gcd;
		}
	}

	if( bunshi_t != bunbo_t ){
		//ICSMessageBox( "５　相続人等に関する事項\n承継割合の合計が『 1 』ではありません。", MB_ICONSTOP );
		ret =  -1;
	}

	if( ret == -1 ){
		if( sgn == 0 ){
			// インボイス施工日以降は準確定の付表６を付表７へ名称変更
			long symd = 0, eymd = 0;
			((CH31HyoView*)m_pParent)->GetKazeiKikan( symd, eymd );
			if( eymd >= ICS_SH_INVOICE_DAY ){
				ICSMessageBox( "付表７　相続人等に関する事項\n相続割合の合計が『 1 』ではありません。", MB_ICONSTOP );
			}
			else{
				ICSMessageBox( "付表６　相続人等に関する事項\n相続割合の合計が『 1 』ではありません。", MB_ICONSTOP );
			}
		}
	}

	return ret;

}

int CShinFhyo6::NofuKanpuGokeiCheck()
{
	int ret = 0;
	char zei_total[MONY_BUF_SIZE] = {0};
	char chiho_total[MONY_BUF_SIZE] = {0};
	int sw = 0;

	f6calq_1to6();

	if( m_pArith->l_test(F61)!=0 ){
		//納付
		for( int idx = 0; idx < SOZOKU_MAX; idx++ ){
			if( sozoku[idx].Sn_NO != -1 ){

				f6calq_9to14(idx);

				sw |= sozoku[idx].Sn_F6_NOFUKANPUsw;

				if( sozoku[idx].Sn_F6_NOFUKANPUsw&0x01 ){
					m_pArith->l_add( zei_total, zei_total, sozoku[idx].Sn_F6_NOFUZEI );
				}else{
					m_pArith->l_add( zei_total, zei_total, F69[idx] );
				}
				if( sozoku[idx].Sn_F6_NOFUKANPUsw&0x02 ){
					m_pArith->l_add( chiho_total, chiho_total, sozoku[idx].Sn_F6_NOFUCHIHO );
				}else{
					m_pArith->l_add( chiho_total, chiho_total, F610[idx] );
				}
			}
		}

		if(( m_pArith->l_cmp( zei_total, F62 ) != 0 )&&( sw&0x01 )){
			ret |= 0x01;
		}
		if(( m_pArith->l_cmp( chiho_total, F63 ) != 0)&&( sw&0x02 )){
			ret |= 0x02;
		}
	}else if( m_pArith->l_test(F64)!=0 ){
		//還付
		for( int idx = 0; idx < SOZOKU_MAX; idx++ ){
			if( sozoku[idx].Sn_NO != -1 ){
				m_pArith->l_add( zei_total, zei_total, sozoku[idx].Sn_F6_KANPUZEI );
				m_pArith->l_add( chiho_total, chiho_total, sozoku[idx].Sn_F6_KANPUCHIHO );

				if( m_pArith->l_test( sozoku[idx].Sn_F6_KANPUZEI ) != 0 ){
					sw |= 0x04;
				}
				if( m_pArith->l_test( sozoku[idx].Sn_F6_KANPUCHIHO ) != 0 ){
					sw |= 0x08;
				}
			}
		}

		if(( m_pArith->l_cmp( zei_total, F65 ) != 0 )&&( sw&0x04 )){
			ret |= 0x04;
		}
		if(( m_pArith->l_cmp( chiho_total, F66 ) != 0 )&&( sw&0x08 )){
			ret |= 0x08;
		}
	}

	return ret;
}

int CShinFhyo6::get_gcd( int num1, int num2 )
{
	int tmp1=0,tmp2=0;
	int joyo=0,gcd=0;

	if(	num1 < num2 ){
		tmp1 = num1;
		tmp2 = num2;
	}else if( num1 > num2){
		tmp1 = num2;
		tmp2 = num1;
	}
	else if( num1 == num2 ){
		gcd = num1;
		return gcd;
	}

	joyo = -1;
	while(joyo!=0){
		joyo = tmp1%tmp2;
		gcd = tmp2;
		tmp1 = tmp2;
		tmp2 = joyo;
	}

	return gcd;
}

void CShinFhyo6::MyNumber_ButtomDisp()
{

	int btn[SOZOKU_PER_PAGE] = {IDC_MYNUM_DEL1,IDC_MYNUM_DEL2,IDC_MYNUM_DEL3,IDC_MYNUM_DEL4};

	char hide = 0x00;
	int sw = SW_HIDE;

	// 上手くん環境なら非表示
	if( GET_EntrySign() != 0 ){
		hide |= 0x01;
	}
	// 個人番号アクセス権限がなかったら非表示
	if( MynoAccessPerm() == FALSE ){
		hide |= 0x01;
	}
	// 入力ロック時は非表示
	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
		hide |= 0x01;
	}
	// 個人番号項目の処理が可能か
	if( NTBMOD8_IsAbleToMyNumber( (*m_pSnHeadData)->m_apno, 0 ) != 1 ){
		hide |= 0x01;
	}

	for( int btn_cnt = 0; btn_cnt < SOZOKU_PER_PAGE; btn_cnt++ ){
		if( hide ){
			GetDlgItem(btn[btn_cnt])->ShowWindow(SW_HIDE);
		}else{
			// ハッシュが空なら非表示
			int size = sizeof(sozoku[m_Lin + btn_cnt].Sn_F6_HASH);

			if( MyNumber_IsHashEmpty( sozoku[m_Lin + btn_cnt].Sn_F6_HASH, size ) == FALSE ){
				GetDlgItem(btn[btn_cnt])->ShowWindow(SW_SHOW);
			}else{
				GetDlgItem(btn[btn_cnt])->ShowWindow(SW_HIDE);
			}
		}
	}

}

BOOL CShinFhyo6::MyNumber_IsHashEmpty( char *hash, int size )
{
	BOOL ret = TRUE;

	for( int idx = 0; idx < size; idx++ ){
		if( hash[idx] == '\0' ){
			break;
		}
		if( hash[idx] != 0 ){
			ret = FALSE;
			break;
		}
	}
	
	return ret;
}

//2018.08.21 INSERT START
// マイナンバー　空白判定(ハッシュ値のみも空白とする)
BOOL CShinFhyo6::MyNumber_IsAllEmpty()
{
	BOOL ret = TRUE;

	for( int idx = 0; idx < SOZOKU_MAX; idx++){
		if( sozoku[idx].Sn_F6_MYNUM1.IsEmpty() && sozoku[idx].Sn_F6_MYNUM2.IsEmpty() && sozoku[idx].Sn_F6_MYNUM3.IsEmpty() ){
			;
		}else{
			ret = FALSE;
			break;
		}
	}

	return ret;
}
//2018.08.21 INSERT END

// マイナンバーコントロールのリセット
// (列データが移動、削除された際に使用。コントロールが一時的に消え、内部的に個人番号を更新する) 
void CShinFhyo6::ResetMyNumberControl()
{
	int line = GetLineNumber_FromIndex( m_Focus, 1 );
	int pos = 0;
	switch( line ){
		case 0:
			pos = ID5041_1;
			break;
		case 1:
			pos = ID5042_1;
			break;
		case 2:
			pos = ID5043_1;
			break;
		case 3:
			pos = ID5044_1;
			break;
		default:
			pos = ID5041_1;
			break;
	}

	int index = GetLineNumber_FromIndex( m_Focus );
	CString str;
	if( index != -1 ){
		str = sozoku[index].Sn_F6_MYNUM1+sozoku[index].Sn_F6_MYNUM2+sozoku[index].Sn_F6_MYNUM3;
	}

	this->LockWindowUpdate();

	m_ctrl_mynum.MoveDiagMNEdit( pos, FALSE);	//マイナンバーコントロールを現在の位置に移動
	m_ctrl_mynum.SetData( str );	//マイナンバーコントロールに現在の位置の個人番号を再セットする
	m_ctrl_mynum.SetVisibleMode(0);						//スクロール後マイナンバーコントロール非表示

	this->UnlockWindowUpdate();
}

int CShinFhyo6::MyNumber_CheckFromDisp( int sgn )
{
	int accesPrm = MynoAccessPerm();										//マイナンバー権限
	int isMNAble = NTBMOD8_IsAbleToMyNumber( (*m_pSnHeadData)->m_apno, 0 );	//マイナンバー有償・無償
	if( !accesPrm||!isMNAble ){
		return 0;
	}

	int ret = 0;

	CStringArray	tmpMNAry;
	int rt1=0,rt2=0;
	rt1 = m_ctrl_mynum.GetData(tmpMNAry, 1);
	if( rt1 == 0 ){
		rt2 = m_ctrl_mynum.CheckNumber( tmpMNAry.GetAt(0)+tmpMNAry.GetAt(1)+tmpMNAry.GetAt(2) );
		if( rt2 == 0 ){ //不正
			ret = -1;
		}else
		if( rt2 == 1 ){ //正常
			ret = 0;
		}else
		if( rt2 == 2 ){ //空白
			ret = 1;
		}else{
			ret = -1;
		}
	}else{
		ret = -1; //不正・失敗
	}

	if( ret == -1 ){
		if( sgn == 0 ){
			//ICSMessageBox( "付表６　相続人等に関する事項\n個人番号に不備があります。", MB_ICONSTOP );
		}
	}		

	return ret;
}


BOOL CShinFhyo6::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){

		if( pMsg->wParam == VK_END ){
			if( MyNumber_CheckFromDisp() < 0 ){
				return FALSE;
			}
			if( MyNumberCheck() == -1 ){
				return FALSE;
			}
			OnEditOFFCallBack( m_Focus );
			if( SozokubunCheck() == -1 ){
				return FALSE;
			}

			WriteData();

			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}
			return TRUE;
		}
		//if( pMsg->wParam == VK_F2 ){
		//	DIAGRAM_DATA   dd;
		//	if( m_curwnd != -1) {
		//		short result = ((CIcsdiagctrl*)GetDlgItem( m_curwnd ))->GetPosition();
		//		diag_getdata( m_curwnd, result, &dd, CIcsdiagctrl );
		//		if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
		//			TerminationIcsdiagctrl2( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
		//		}
		//		return TRUE;
		//	}
		//}

		if( pMsg->wParam == VK_RETURN ){
			short result = m_Fhyo6Diag.GetPosition();
			DIAGRAM_DATA   DiagData;

			//改行のみの行の場合は次の項目へ、そうでないなら改行としたいが、
			//GetCursorPos関数が改行コードを判定しないため、断念

			if(( result == ID5051 )||( result == ID5052 )||( result == ID5053 )||( result == ID5054 )
			 ||( result == ID5061 )||( result == ID5062 )||( result == ID5063 )||( result == ID5064 )){
				m_Util.DiagOcxIniz(&DiagData);
				diag_getdata( IDC_ICSDIAGCTRL1, result, &DiagData, CIcsdiagctrl );
				TerminationIcsdiagctrl1( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&DiagData );
				return TRUE;
			}
		}

		if( pMsg->wParam == VK_BACK ){
			short result = m_Fhyo6Diag.GetPosition();
			DIAGRAM_DATA   DiagData;

			switch(result){
				case ID5071:
				case ID5072:
				case ID5073:
				case ID5074:
					TerminationIcsdiagctrl1( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&DiagData );
					return TRUE;
				default:
					break;
			}
		}

		if(( pMsg->wParam == VK_LEFT )||( pMsg->wParam == VK_RIGHT )||( pMsg->wParam == VK_F2 )||( pMsg->wParam == VK_F3 )){
			DIAGRAM_ATTRIBUTE        DA;
			short result = m_Fhyo6Diag.GetPosition();
			diag_getattr( IDC_ICSDIAGCTRL1, result, &DA, CIcsdiagctrl );

			if( DA.attr_inpkind == 1 ) {  //コンボボックスにカーソルがある場合はリストを閉じる
				DIAGRAM_DATA   DiagData;
				m_Util.DiagOcxIniz(&DiagData);
				diag_getdata( IDC_ICSDIAGCTRL1, result, &DiagData, CIcsdiagctrl );

				diag_cbdrop( IDC_ICSDIAGCTRL1, result, FALSE, CIcsdiagctrl );
				TerminationIcsdiagctrl1( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&DiagData );
				return TRUE;
			}

			DIAGRAM_DATA	DiagData;
			DIAGRAM_DATA_clear( &DiagData );
			if(( result == ID5071 )||( result == ID5072 )||( result == ID5073 )||( result == ID5074 )){
				TerminationIcsdiagctrl1( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&DiagData );
				return TRUE;
			}
		}
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

int CShinFhyo6::ReadData()
{

	//memmove( Sn_F6_DEATH, (*m_pSnFhyo6Data)->Sn_F6_DEATH, sizeof(Sn_F6_DEATH));
	//memmove( Sn_F6_DEATH, (*m_pSnFhyo6Data)->Sn_F6_DEATH, sizeof(Sn_F6_DEATH)-1);
//	char dummy;
//	m_Util.ADtoJC( &dummy, &Sn_F6_DEATH[0], &Sn_F6_DEATH[1], &Sn_F6_DEATH[2], (*m_pSnFhyo6Data)->Sn_F6_DEATH ); //年号固定(平成)
	m_Util.ADtoJC( &Sn_F6_DEATH[0], &Sn_F6_DEATH[1], &Sn_F6_DEATH[2], &Sn_F6_DEATH[3] , (*m_pSnFhyo6Data)->Sn_F6_DEATH );
	Sn_F6_DAIHYO	= (*m_pSnFhyo6Data)->Sn_F6_DAIHYO;
	Sn_F6_LIMIT		= (*m_pSnFhyo6Data)->Sn_F6_LIMIT;

	// 事業承継の有無
	Sn_F6_USE = ( *m_pSnFhyo6Data )->Sn_6BS_USE;

	// 住所等
	memmove ( Sn_F6_ADR, ( *m_pSnFhyo6Data )->Sn_6BS_ADR, 80 );

	// 電話番号
	m_Util.char_to_cstring ( &Sn_F6_TELNO1, ( *m_pSnFhyo6Data )->Sn_6BS_TELNO1, 6 );
	m_Util.char_to_cstring ( &Sn_F6_TELNO2, ( *m_pSnFhyo6Data )->Sn_6BS_TELNO2, 4 );
	m_Util.char_to_cstring ( &Sn_F6_TELNO3, ( *m_pSnFhyo6Data )->Sn_6BS_TELNO3, 4 );

	// フリガナ
	Sn_F6_NMFRI = ( *m_pSnFhyo6Data )->Sn_6BS_NMFRI;

	// 氏名
	Sn_F6_NAME = ( *m_pSnFhyo6Data )->Sn_6BS_NAME;

	// その他参考事項
	m_Util.cstring_to_char ( Sn_F6_SONOTA, ( *m_pSnFhyo6Data )->Sn_6BS_SONOTA, 80 );

	for( int cnt = 0; cnt < SOZOKU_MAX; cnt++ ){

		if( (*m_pSnFhyo6Data)->Sn_NO[cnt] == -1 ){
			continue;
		}

		sozoku[cnt].Sn_NO				= (*m_pSnFhyo6Data)->Sn_NO[cnt];
		sozoku[cnt].Sn_F6_JUSYO		= (*m_pSnFhyo6Data)->Sn_F6_JUSYO[cnt];
		sozoku[cnt].Sn_F6_FURI			= (*m_pSnFhyo6Data)->Sn_F6_FURI[cnt];
		sozoku[cnt].Sn_F6_NAME			= (*m_pSnFhyo6Data)->Sn_F6_NAME[cnt];
		memmove( sozoku[cnt].Sn_F6_HASH, (*m_pSnFhyo6Data)->Sn_F6_HASH[cnt], sizeof(sozoku[cnt].Sn_F6_HASH));
		sozoku[cnt].Sn_F6_SYOKU		= (*m_pSnFhyo6Data)->Sn_F6_SYOKU[cnt];
		sozoku[cnt].Sn_F6_TSUDUKI		= (*m_pSnFhyo6Data)->Sn_F6_TSUDUKI[cnt];
		//memmove( sozoku[cnt].Sn_F6_BIRTH, (*m_pSnFhyo6Data)->Sn_F6_BIRTH[cnt], sizeof(sozoku[cnt].Sn_F6_BIRTH) );
		m_Util.ADtoJC( &sozoku[cnt].Sn_F6_BIRTH[0], &sozoku[cnt].Sn_F6_BIRTH[1], &sozoku[cnt].Sn_F6_BIRTH[2], &sozoku[cnt].Sn_F6_BIRTH[3], (*m_pSnFhyo6Data)->Sn_F6_BIRTH[cnt] );
		sozoku[cnt].Sn_F6_TEL1			= (*m_pSnFhyo6Data)->Sn_F6_TEL1[cnt];
		sozoku[cnt].Sn_F6_TEL2			= (*m_pSnFhyo6Data)->Sn_F6_TEL2[cnt];
		sozoku[cnt].Sn_F6_TEL3			= (*m_pSnFhyo6Data)->Sn_F6_TEL3[cnt];
		sozoku[cnt].Sn_F6_HOTEI		= (*m_pSnFhyo6Data)->Sn_F6_HOTEI[cnt];
		sozoku[cnt].Sn_F6_BUNSHI		= (*m_pSnFhyo6Data)->Sn_F6_BUNSHI[cnt];
		sozoku[cnt].Sn_F6_BUNBO		= (*m_pSnFhyo6Data)->Sn_F6_BUNBO[cnt];
		memmove( sozoku[cnt].Sn_F6_KAGAKU, (*m_pSnFhyo6Data)->Sn_F6_KAGAKU[cnt], sizeof(sozoku[cnt].Sn_F6_KAGAKU) );
		memmove( sozoku[cnt].Sn_F6_NOFUZEI, (*m_pSnFhyo6Data)->Sn_F6_NOFUZEI[cnt], sizeof(sozoku[cnt].Sn_F6_NOFUZEI) );
		memmove( sozoku[cnt].Sn_F6_NOFUCHIHO, (*m_pSnFhyo6Data)->Sn_F6_NOFUCHIHO[cnt], sizeof(sozoku[cnt].Sn_F6_NOFUCHIHO) );
		memmove( sozoku[cnt].Sn_F6_KANPUZEI, (*m_pSnFhyo6Data)->Sn_F6_KANPUZEI[cnt], sizeof(sozoku[cnt].Sn_F6_KANPUZEI) );
		memmove( sozoku[cnt].Sn_F6_KANPUCHIHO, (*m_pSnFhyo6Data)->Sn_F6_KANPUCHIHO[cnt], sizeof(sozoku[cnt].Sn_F6_KANPUCHIHO) );
		sozoku[cnt].Sn_F6_NOFUKANPUsw	= (*m_pSnFhyo6Data)->Sn_F6_NOFUKANPUsw[cnt];
		sozoku[cnt].Sn_F6_GNAME		= (*m_pSnFhyo6Data)->Sn_F6_GNAME[cnt];
		sozoku[cnt].Sn_F6_GSEL			= (*m_pSnFhyo6Data)->Sn_F6_GSEL[cnt];
		sozoku[cnt].Sn_F6_SNAME		= (*m_pSnFhyo6Data)->Sn_F6_SNAME[cnt];
		sozoku[cnt].Sn_F6_SSEL			= (*m_pSnFhyo6Data)->Sn_F6_SSEL[cnt];
		sozoku[cnt].Sn_F6_YOKIN		= (*m_pSnFhyo6Data)->Sn_F6_YOKIN[cnt];
		sozoku[cnt].Sn_F6_KOUZA		= (*m_pSnFhyo6Data)->Sn_F6_KOUZA[cnt];
		sozoku[cnt].Sn_F6_KIGO1		= (*m_pSnFhyo6Data)->Sn_F6_KIGO1[cnt];
		sozoku[cnt].Sn_F6_KIGO2		= (*m_pSnFhyo6Data)->Sn_F6_KIGO2[cnt];
		sozoku[cnt].Sn_F6_YUBIN		= (*m_pSnFhyo6Data)->Sn_F6_YUBIN[cnt];
	}

	MyNumber_Change();

	return 0;
}

int CShinFhyo6::WriteData()
{

	MyNumber_Update();

	//syz_sozokuテーブルはReadData()でできている前提

	//memmove( (*m_pSnFhyo6Data)->Sn_F6_DEATH, Sn_F6_DEATH, sizeof((*m_pSnFhyo6Data)->Sn_F6_DEATH));
	//m_Util.JCtoAD( &(*m_pSnFhyo6Data)->Sn_F6_DEATH, Sn_F6_DEATH[0], Sn_F6_DEATH[1], Sn_F6_DEATH[2], Sn_F6_DEATH[3] ); 
//	m_Util.JCtoAD( &(*m_pSnFhyo6Data)->Sn_F6_DEATH, 0x04, Sn_F6_DEATH[0], Sn_F6_DEATH[1], Sn_F6_DEATH[2] ); //年号固定(平成)
	m_Util.JCtoAD( &(*m_pSnFhyo6Data)->Sn_F6_DEATH, Sn_F6_DEATH[0], Sn_F6_DEATH[1], Sn_F6_DEATH[2] ,Sn_F6_DEATH[3] );
	(*m_pSnFhyo6Data)->Sn_F6_DAIHYO = Sn_F6_DAIHYO;
	(*m_pSnFhyo6Data)->Sn_F6_LIMIT = Sn_F6_LIMIT;


	// 事業承継の有無
	( *m_pSnFhyo6Data )->Sn_6BS_USE = Sn_F6_USE;

	// 住所等
	memmove ( ( *m_pSnFhyo6Data )->Sn_6BS_ADR, Sn_F6_ADR, 80 );

	// 電話番号
	m_Util.cstring_to_char ( ( *m_pSnFhyo6Data )->Sn_6BS_TELNO1, ( LPCTSTR )Sn_F6_TELNO1, 6 );
	m_Util.cstring_to_char ( ( *m_pSnFhyo6Data )->Sn_6BS_TELNO2, ( LPCTSTR )Sn_F6_TELNO2, 4 );
	m_Util.cstring_to_char ( ( *m_pSnFhyo6Data )->Sn_6BS_TELNO3, ( LPCTSTR )Sn_F6_TELNO3, 4 );

	// フリガナ
	( *m_pSnFhyo6Data )->Sn_6BS_NMFRI = Sn_F6_NMFRI;

	// 氏名
	( *m_pSnFhyo6Data )->Sn_6BS_NAME = Sn_F6_NAME;

	// その他参考事項
	char buf[128] = { 0 };
	memset ( buf, '\0', sizeof ( buf ) );
	memset ( buf, 0x20, 40 );
	int kjln = kjlen ( Sn_F6_SONOTA, 40 );
	memmove ( buf, Sn_F6_SONOTA, kjln );
	kjln = kjlen ( &Sn_F6_SONOTA[40], 40 );
	memmove ( &buf[40], &Sn_F6_SONOTA[40], kjln );
	m_Util.char_to_cstring ( &( *m_pSnFhyo6Data )->Sn_6BS_SONOTA, ( unsigned char* )buf, 80 );

	f6calq_1to6();

	char sgn = 0x00;
	if( m_pArith->l_test(XM26) > 0 ){
		sgn = 0x02;
	}else
	if( m_pArith->l_test(XM26) < 0 ){
		sgn = 0x01;
	}else{
		sgn = 0x03;
	}

	for( int cnt = 0; cnt < SOZOKU_MAX; cnt++ ){

		if( IsNULL_Column(cnt,sgn) == TRUE ){
			UpdateShinPayment( 1, cnt );
			(*m_pSnFhyo6Data)->Sn_NO[cnt]			= -1;
		}else{
			(*m_pSnFhyo6Data)->Sn_NO[cnt]			= sozoku[cnt].Sn_NO;
		}

		(*m_pSnFhyo6Data)->Sn_F6_JUSYO[cnt]		= sozoku[cnt].Sn_F6_JUSYO;
		(*m_pSnFhyo6Data)->Sn_F6_FURI[cnt]		= sozoku[cnt].Sn_F6_FURI;
		(*m_pSnFhyo6Data)->Sn_F6_NAME[cnt]		= sozoku[cnt].Sn_F6_NAME;
		memmove( (*m_pSnFhyo6Data)->Sn_F6_HASH[cnt], sozoku[cnt].Sn_F6_HASH, sizeof((*m_pSnFhyo6Data)->Sn_F6_HASH[cnt]));
		(*m_pSnFhyo6Data)->Sn_F6_SYOKU[cnt]		= sozoku[cnt].Sn_F6_SYOKU;
		(*m_pSnFhyo6Data)->Sn_F6_TSUDUKI[cnt]		= sozoku[cnt].Sn_F6_TSUDUKI;
		//memmove( (*m_pSnFhyo6Data)->Sn_F6_BIRTH[cnt], sozoku[cnt].Sn_F6_BIRTH, sizeof((*m_pSnFhyo6Data)->Sn_F6_BIRTH[cnt]) );
		m_Util.JCtoAD( &(*m_pSnFhyo6Data)->Sn_F6_BIRTH[cnt], sozoku[cnt].Sn_F6_BIRTH[0], sozoku[cnt].Sn_F6_BIRTH[1], sozoku[cnt].Sn_F6_BIRTH[2], sozoku[cnt].Sn_F6_BIRTH[3] );
		(*m_pSnFhyo6Data)->Sn_F6_TEL1[cnt]		= sozoku[cnt].Sn_F6_TEL1;
		(*m_pSnFhyo6Data)->Sn_F6_TEL2[cnt]		= sozoku[cnt].Sn_F6_TEL2;
		(*m_pSnFhyo6Data)->Sn_F6_TEL3[cnt]		= sozoku[cnt].Sn_F6_TEL3;
		(*m_pSnFhyo6Data)->Sn_F6_HOTEI[cnt]		= sozoku[cnt].Sn_F6_HOTEI;
		(*m_pSnFhyo6Data)->Sn_F6_BUNSHI[cnt]		= sozoku[cnt].Sn_F6_BUNSHI;
		(*m_pSnFhyo6Data)->Sn_F6_BUNBO[cnt]		= sozoku[cnt].Sn_F6_BUNBO;
		memmove( (*m_pSnFhyo6Data)->Sn_F6_KAGAKU[cnt],	sozoku[cnt].Sn_F6_KAGAKU, sizeof((*m_pSnFhyo6Data)->Sn_F6_KAGAKU[cnt])) ;
		memmove( (*m_pSnFhyo6Data)->Sn_F6_NOFUZEI[cnt], sozoku[cnt].Sn_F6_NOFUZEI, sizeof((*m_pSnFhyo6Data)->Sn_F6_NOFUZEI[cnt])) ;
		memmove( (*m_pSnFhyo6Data)->Sn_F6_NOFUCHIHO[cnt], sozoku[cnt].Sn_F6_NOFUCHIHO, sizeof((*m_pSnFhyo6Data)->Sn_F6_NOFUCHIHO[cnt])) ;
		memmove( (*m_pSnFhyo6Data)->Sn_F6_KANPUZEI[cnt], sozoku[cnt].Sn_F6_KANPUZEI, sizeof((*m_pSnFhyo6Data)->Sn_F6_KANPUZEI[cnt])) ;
		memmove( (*m_pSnFhyo6Data)->Sn_F6_KANPUCHIHO[cnt], sozoku[cnt].Sn_F6_KANPUCHIHO, sizeof((*m_pSnFhyo6Data)->Sn_F6_KANPUCHIHO[cnt])) ;
		(*m_pSnFhyo6Data)->Sn_F6_NOFUKANPUsw[cnt]	= sozoku[cnt].Sn_F6_NOFUKANPUsw;
		(*m_pSnFhyo6Data)->Sn_F6_GNAME[cnt]		= sozoku[cnt].Sn_F6_GNAME;
		(*m_pSnFhyo6Data)->Sn_F6_GSEL[cnt]		= sozoku[cnt].Sn_F6_GSEL;
		(*m_pSnFhyo6Data)->Sn_F6_SNAME[cnt]		= sozoku[cnt].Sn_F6_SNAME;
		(*m_pSnFhyo6Data)->Sn_F6_SSEL[cnt]		= sozoku[cnt].Sn_F6_SSEL;
		(*m_pSnFhyo6Data)->Sn_F6_YOKIN[cnt]		= sozoku[cnt].Sn_F6_YOKIN;
		(*m_pSnFhyo6Data)->Sn_F6_KOUZA[cnt]		= sozoku[cnt].Sn_F6_KOUZA;
		(*m_pSnFhyo6Data)->Sn_F6_KIGO1[cnt]		= sozoku[cnt].Sn_F6_KIGO1;
		(*m_pSnFhyo6Data)->Sn_F6_KIGO2[cnt]		= sozoku[cnt].Sn_F6_KIGO2;
		(*m_pSnFhyo6Data)->Sn_F6_YUBIN[cnt]		= sozoku[cnt].Sn_F6_YUBIN;		
	}

	return 0;
}

void CShinFhyo6::DispFh6(int mode)
{

	DIAGRAM_ATTRIBUTE	DA;
	COLORREF			bc;

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	char	buf[512] = {0};
	CString tmp_str1,tmp_str2,tmp_str3;
	char	tmp_char1[100] = {0},tmp_char2[100] = {0},tmp_char3[100] = {0};
	int		flg;
	int len = 0;

	if(!mode)
		ResetMyNumberControl();//マイナンバーコントロールリセット

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		m_Fhyo6Diag.ModifyItem ( 376, DIAG_MDFY_TRANSPARENT );
	}

	// 画面の初期化
	for( int i = ID0010;i <= ID5244; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL1, (short )i );
	}

	long eymd = 0;
	if ( !( ( *m_pSnHeadData )->Sn_SKKBN % 2 ) ) {
		eymd = ( *m_pSnHeadData )->Sn_MDAYE;
	}
	else {
		eymd = ( *m_pSnHeadData )->Sn_KDAYE;
	}
	if (eymd >= ICS_SH_INVOICE_DAY) {	//インボイス施工日以降は付表６→付表７へ文言変更
		tmp_str1.Format("付表７　死亡した事業者の消費税及び地方消費税の確定申告明細書");
		dsp_disp( 0, tmp_str1);
	}

	tmp_str1.Format( "%d", m_Lin+1 );
	dsp_disp( ID_NUMBER1, tmp_str1 );
	tmp_str1.Format( "%d", m_Lin+2 );
	dsp_disp( ID_NUMBER2, tmp_str1 );
	tmp_str1.Format( "%d", m_Lin+3 );
	dsp_disp( ID_NUMBER3, tmp_str1 );
	tmp_str1.Format( "%d", m_Lin+4 );
	dsp_disp( ID_NUMBER4, tmp_str1 );

	::ZeroMemory( tmp_char1, sizeof(tmp_char1));
	::ZeroMemory( tmp_char2, sizeof(tmp_char2));
	tmp_str1.Empty();
	tmp_str2.Empty();
	memmove( tmp_char1, &(*m_pSnHeadData)->Sn_NOZEI[0], 40 );
	memmove( tmp_char2, &(*m_pSnHeadData)->Sn_NOZEI[40], 40 );
//2018.08.21 UPDATE START
	//tmp_str1.Format( tmp_char1 );
	//tmp_str2.Format( tmp_char2 );
	tmp_str1 = tmp_char1;
	tmp_str2 = tmp_char2;
//2018.08.21 UPDATE END
	dsp_edit( ID1010, tmp_str1 + '\n' + tmp_str2 );

	//課税期間
	char	yy=0, mm=0, dd=0;
	int gengo = 0;
	CString gengoStr;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd, &gengo );
	if( (yy&0xff) && (mm&0xff) && (dd&0xff) ){
		tmp_str1.Format( ("%02x"), yy );
		dsp_disp( ID0020, tmp_str1 );
		tmp_str1.Format( ("%02x"), mm );
		dsp_disp( ID0030, tmp_str1 );
		tmp_str1.Format( ("%02x"), dd );
		dsp_disp( ID0040, tmp_str1 );
	}

	if ( gengo == ID_ICSSH_REIWA_GENGO ) {
		gengoStr = SYZ31_REIWA_G_STR;
	}
	else {
		gengoStr = SYZ31_HEISEI_G_STR;
	}
	dsp_disp(ID0010, gengoStr);

	yy=0, mm=0, dd=0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd, &gengo );
	if( (yy&0xff) && (mm&0xff) && (dd&0xff) ){ 
		tmp_str1.Format( ("%02x"), yy );
		dsp_disp( ID0060, tmp_str1 );
		tmp_str1.Format( ("%02x"), mm );
		dsp_disp( ID0070, tmp_str1 );
		tmp_str1.Format( ("%02x"), dd );
		dsp_disp( ID0080, tmp_str1 );
	}
	if ( gengo == ID_ICSSH_REIWA_GENGO ) {
		gengoStr = SYZ31_REIWA_G_STR;
	}
	else {
		gengoStr = SYZ31_HEISEI_G_STR;
	}
	dsp_disp(ID0050, gengoStr);

	// 整理番号
	if( ((*m_pSnHeadData)->Sn_SEIRI[0]&0xff) != 0xff ){
		memset( buf, '\0', sizeof( buf ) );
		m_pArith->l_unpac( buf, (*m_pSnHeadData)->Sn_SEIRI, 8 );
		tmp_str1.Format( buf );
		dsp_edit( ID0090, tmp_str1 );
	}

	// 代表者（振りがな）
	tmp_str1.Empty();
	::ZeroMemory( tmp_char1, sizeof(tmp_char1));
	memmove( tmp_char1, (*m_pSnHeadData)->Sn_DIFRI, sizeof((*m_pSnHeadData)->Sn_DIFRI));
	//tmp_str1.Format((char)(*m_pSnHeadData)->Sn_DIFRI );
//2018.08.21 UPDATE START
//	tmp_str1.Format(tmp_char1);
	tmp_str1 = tmp_char1;
//2018.08.21 UPDATE END
	tmp_str1 = tmp_str1.Left(50);
	dsp_edit( ID1020, tmp_str1 );

	// 代表者名
	tmp_str1.Empty();
	::ZeroMemory( tmp_char1, sizeof(tmp_char1));
	memmove( tmp_char1, (*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO));
//2018.08.21 UPDATE START
//	tmp_str1.Format(tmp_char1);
	tmp_str1 = tmp_char1;
//2018.08.21 UPDATE END
	//tmp_str1.Format((char)(*m_pSnHeadData)->Sn_DIHYO );
	dsp_edit( ID1030, tmp_str1 );
	
	dsp_day( ID1040, Sn_F6_DEATH );

	// 事業承継
	if ( Sn_F6_USE ) {
		dsp_check ( ID6010, 1 );
		dsp_check ( ID6011, 0 );
	}
	else {
		dsp_check ( ID6010, 0 );
		dsp_check ( ID6011, 1 );
	}
	ModifyDiagInput();

	if ( Sn_F6_USE ) {
		// 住所等
		memset ( buf, '\0', sizeof ( buf ) );
		len = m_Util.MixSpaceCutLength ( ( char* )Sn_F6_ADR, 40 );
		memmove ( buf, Sn_F6_ADR, len );
		tmp_str1 = buf;
		dsp_edit ( ID6020, tmp_str1 );

		memset ( buf, '\0', sizeof ( buf ) );
		len = m_Util.MixSpaceCutLength ( ( char* )&Sn_F6_ADR[40], 40 );
		memmove ( buf, &Sn_F6_ADR[40], len );
		tmp_str1 = buf;
		dsp_edit ( ID6021, tmp_str1 );

		// 電話番号
		dsp_edit ( ID6030, Sn_F6_TELNO1 );
		dsp_edit ( ID6031, Sn_F6_TELNO2 );
		dsp_edit ( ID6032, Sn_F6_TELNO3 );

		// フリガナ
		dsp_edit ( ID6040, Sn_F6_NMFRI );
		dsp_edit ( ID6041, Sn_F6_NAME );

		// その他参考事項
		memset ( buf, '\0', sizeof ( buf ) );
		len = m_Util.MixSpaceCutLength ( ( char* )Sn_F6_SONOTA, 40 );
		memmove ( buf, Sn_F6_SONOTA, len );
		tmp_str1 = buf;
		dsp_edit ( ID6050, tmp_str1 );

		memset ( buf, '\0', sizeof ( buf ) );
		len = m_Util.MixSpaceCutLength ( ( char* )&Sn_F6_SONOTA[40], 40 );
		memmove ( buf, &Sn_F6_SONOTA[40], len );
		tmp_str1 = buf;
		dsp_edit ( ID6051, tmp_str1 );
	}

	dsp_edit( ID2010, Sn_F6_DAIHYO );
	dsp_check( ID3010, Sn_F6_LIMIT );


	// 納める消費税～
	f6calq_1to6();

	dsp_val( ID4010, F61 );
	dsp_val( ID4020, F62 );
	dsp_val( ID4030, F63 );
	dsp_val( ID4040, F64 );
	dsp_val( ID4050, F65 );
	dsp_val( ID4060, F66 );

	int accesPrm = MynoAccessPerm();										//マイナンバー権限
	int isMNAble = NTBMOD8_IsAbleToMyNumber( (*m_pSnHeadData)->m_apno, 0 );	//マイナンバー有償・無償


	for( int cnt = 0; cnt < SOZOKU_PER_PAGE; cnt++ ){
		
		memset( buf, '\0', sizeof(buf) );
		int size = 0;
		BOOL empty = FALSE;

		int idx = GetIndex_FromLine(m_Lin+cnt);


		if( idx != -1 ){

			f6calq_9to14(idx);

			dsp_edit(	JUSYO_ID[cnt],		sozoku[idx].Sn_F6_JUSYO );
			dsp_edit(	FURI_ID[cnt],		sozoku[idx].Sn_F6_FURI );
			dsp_edit(	NAME_ID[cnt],		sozoku[idx].Sn_F6_NAME );

			dsp_edit(	MYNUM1_ID[cnt],		sozoku[idx].Sn_F6_MYNUM1 );	
			dsp_edit(	MYNUM2_ID[cnt],		sozoku[idx].Sn_F6_MYNUM2 );	
			dsp_edit(	MYNUM3_ID[cnt],		sozoku[idx].Sn_F6_MYNUM3 );

			dsp_edit(	SYOKU_ID[cnt],		sozoku[idx].Sn_F6_SYOKU );
			dsp_edit(	TSUDUKI_ID[cnt],	sozoku[idx].Sn_F6_TSUDUKI );
			dsp_day( 	BIRTH_ID[cnt],		sozoku[idx].Sn_F6_BIRTH );
			dsp_edit(	TEL1_ID[cnt],		sozoku[idx].Sn_F6_TEL1 );
			dsp_edit(	TEL2_ID[cnt],		sozoku[idx].Sn_F6_TEL2 );
			dsp_edit(	TEL3_ID[cnt],		sozoku[idx].Sn_F6_TEL3 );
			dsp_combo(	HOTEI_ID[cnt],		sozoku[idx].Sn_F6_HOTEI );
			dsp_edit(	BUNSHI_ID[cnt],		sozoku[idx].Sn_F6_BUNSHI );
			dsp_edit(	BUNBO_ID[cnt],		sozoku[idx].Sn_F6_BUNBO );
			dsp_val(	KAGAKU_ID[cnt],		sozoku[idx].Sn_F6_KAGAKU );

			char zei[MONY_BUF_SIZE]={0},chiho[MONY_BUF_SIZE]={0},gokei[MONY_BUF_SIZE]={0};
			if( m_pArith->l_test(XM26) > 0 ){
				if( sozoku[idx].Sn_F6_NOFUKANPUsw&0x01){
					memmove( zei, sozoku[idx].Sn_F6_NOFUZEI, MONY_BUF_SIZE );
					if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){
						ChangeColor( IDC_ICSDIAGCTRL1, NOFUZEI_ID[cnt], 3 );
					}
				}else{
					memmove( zei, F69[idx], MONY_BUF_SIZE );
					if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){
						ChangeColor( IDC_ICSDIAGCTRL1, NOFUZEI_ID[cnt], 0 );
					}
				}
				if( sozoku[idx].Sn_F6_NOFUKANPUsw&0x02){
					memmove( chiho, sozoku[idx].Sn_F6_NOFUCHIHO, MONY_BUF_SIZE );
					if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){
						ChangeColor( IDC_ICSDIAGCTRL1, NOFUCHIHO_ID[cnt], 3 );
					}
				}else{
					memmove( chiho, F610[idx], MONY_BUF_SIZE );
					if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){
						ChangeColor( IDC_ICSDIAGCTRL1, NOFUCHIHO_ID[cnt], 0 );
					}
				}

				dsp_val( NOFUZEI_ID[cnt],	zei );
				dsp_val( NOFUCHIHO_ID[cnt],	chiho );
				m_pArith->l_add( gokei, zei, chiho );
				dsp_val( NOFUGOKEI_ID[cnt],	gokei );
			}else if( m_pArith->l_test(XM26) < 0 ){
				dsp_val( KANPUZEI_ID[cnt],	sozoku[idx].Sn_F6_KANPUZEI );
				dsp_val( KANPUCHIHO_ID[cnt],	sozoku[idx].Sn_F6_KANPUCHIHO );
				m_pArith->l_add( gokei, sozoku[idx].Sn_F6_KANPUZEI, sozoku[idx].Sn_F6_KANPUCHIHO );
				if( m_pArith->l_test(gokei) < 0 ){
					m_pArith->l_neg(gokei);
				}
				dsp_val( KANPUGOKEI_ID[cnt],	gokei );
			}

			dsp_edit(	GNAME_ID[cnt],		sozoku[idx].Sn_F6_GNAME );
			dsp_combo(	GSEL_ID[cnt],		sozoku[idx].Sn_F6_GSEL );
			dsp_edit(	SNAME_ID[cnt],		sozoku[idx].Sn_F6_SNAME );
			dsp_combo(	SSEL_ID[cnt],		sozoku[idx].Sn_F6_SSEL );
			dsp_edit(	YOKIN_ID[cnt],		sozoku[idx].Sn_F6_YOKIN );
			dsp_edit(	KOUZA_ID[cnt],		sozoku[idx].Sn_F6_KOUZA );
			dsp_edit(	KIGO1_ID[cnt],		sozoku[idx].Sn_F6_KIGO1 );
			dsp_edit(	KIGO2_ID[cnt],		sozoku[idx].Sn_F6_KIGO2 );
			dsp_edit(	YUBIN_ID[cnt],		sozoku[idx].Sn_F6_YUBIN );
		}else{
			if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){
				if( m_pArith->l_test(XM26) > 0 ){
					ChangeColor( IDC_ICSDIAGCTRL1, NOFUZEI_ID[cnt], 0 );
					ChangeColor( IDC_ICSDIAGCTRL1, NOFUCHIHO_ID[cnt], 0 );
				}
			}
		}

		if( idx != -1 ){
			size = sizeof(sozoku[idx].Sn_F6_HASH);
			empty =  MyNumber_IsHashEmpty( sozoku[idx].Sn_F6_HASH, size );
		}else{
			empty = TRUE;
		}

		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			flg = DIAG_MDFY_READONLY|DIAG_MODIFY_NOCOPY;//個人番号のコピー禁止
			bc = BC_GRAY;
		}else
		if( empty && accesPrm && isMNAble ){
			flg = DIAG_MDFY_EDIT;
			bc = BC_CREAM;
		}else{
			flg = DIAG_MDFY_READONLY|DIAG_MODIFY_NOCOPY;//個人番号のコピー禁止
			bc = BC_GRAY;
		}
		diag_modify( IDC_ICSDIAGCTRL1, MYNUM1_ID[cnt], flg, CIcsdiagctrl );
		diag_getattr( IDC_ICSDIAGCTRL1, MYNUM1_ID[cnt], &DA, CIcsdiagctrl );
		DA.attr_bcolor = bc;
		diag_setattr( IDC_ICSDIAGCTRL1, MYNUM1_ID[cnt], &DA, TRUE, CIcsdiagctrl );

		diag_modify( IDC_ICSDIAGCTRL1, MYNUM2_ID[cnt], flg, CIcsdiagctrl );
		diag_getattr( IDC_ICSDIAGCTRL1, MYNUM2_ID[cnt], &DA, CIcsdiagctrl );
		DA.attr_bcolor = bc;
		diag_setattr( IDC_ICSDIAGCTRL1, MYNUM2_ID[cnt], &DA, TRUE, CIcsdiagctrl );

		diag_modify( IDC_ICSDIAGCTRL1, MYNUM3_ID[cnt], flg, CIcsdiagctrl );
		diag_getattr( IDC_ICSDIAGCTRL1, MYNUM3_ID[cnt], &DA, CIcsdiagctrl );
		DA.attr_bcolor = bc;
		diag_setattr( IDC_ICSDIAGCTRL1, MYNUM3_ID[cnt], &DA, TRUE, CIcsdiagctrl );
	}

	MyNumber_ButtomDisp();

	diag_redraw( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
}

//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinFhyo6::FocusMoveCheck(int Index , int Move)
{
	int idx = 0;
	int i = 0 , j = 0;

	DIAGRAM_ATTRIBUTE	DA;

	//現在ポジション検索
	if(( Move == 0 )||( Move == 1 )){
		for(i = 0 ; i < FCS_FHYO6CNT ; i++){
			if(Index == FCS_Fhyo60[i].IDNo){
				break;
			}
		}
		idx = FCS_Fhyo60[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_FHYO6CNT ; i++){
			if(Index == FCS_Fhyo60_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_Fhyo60_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_FHYO6CNT ; j++){
			m_Fhyo6Diag.GetAttribute(FCS_Fhyo60[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo60[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Fhyo6Diag.GetAttribute(FCS_Fhyo60[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo60[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_FHYO6CNT ; j++){
			m_Fhyo6Diag.GetAttribute(FCS_Fhyo60_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo60_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Fhyo6Diag.GetAttribute(FCS_Fhyo60_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo60_LR[j].IDNo;
				break;
			}
		}
	}
	return idx;
}

//-----------------------------------------------------------------------------
// 入力項目色変え
//-----------------------------------------------------------------------------
// 引数	id		；	ダイアグラムID
//		index	：	ダイアグラム内インデックス
//		sign	：	変更サイン
//-----------------------------------------------------------------------------
void CShinFhyo6::ChangeColor( unsigned short id, short Index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, Index, &DA, CIcsdiagctrl );
	switch( sign ){
		case 1 :	// 入力不可	（白色）
			DA.attr_bcolor = BC_WHITE;
			break;
		case 2 :	// ロック	（灰色）
			DA.attr_bcolor = BC_GRAY;
			break;
		case 3 :	// 特殊色①	（青色）
			DA.attr_bcolor = BC_BLUE;
			break;
		case 4 :	// 特殊色②	（緑色）	
			DA.attr_bcolor = BC_GREEN;
			break;
		case 5 :	// 特殊色③	（薄緑色）	 
			DA.attr_bcolor = BC_GREEN_L;
			break;
		default:	// 実額入力	（クリーム色） 
			DA.attr_bcolor = BC_CREAM;
			break;
	}
	diag_setattr( id, Index, &DA, FALSE, CIcsdiagctrl );
}

// 初期ポジションセット
void CShinFhyo6::SetItemPosition()
{
	if( mynum_cursel ){//mainframe：OnActivateから飛んでくるため、個人番号にフォーカス状態は何もしない。清水
		return;
	}
	this->SetFocus();
	int index = m_Fhyo6Diag.GetPosition();
	if( index != -1 ){
		m_Fhyo6Diag.SetPosition(index);
		m_Fhyo6Diag.SetFocus();
		return;
	}

	m_Fhyo6Diag.SetFocus();
	//m_Fhyo6Diag.SetPosition(ID2010);//ID1040から変更

	if ( Sn_F6_USE ) {
		m_Fhyo6Diag.SetPosition ( ID6020 );//ID2010から変更
	}
	else {
		m_Fhyo6Diag.SetPosition ( ID2010 );
	}
}

int CShinFhyo6::InitDiagAttr()
{

	int	cnt = 0;
	while( 1 ){
		if( Fhyo6[cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Fhyo6[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Fhyo6[cnt].UnConnect;
			}
			else{
				Flg = Fhyo6[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL1, Fhyo6[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Fhyo6[cnt].Connect );

		cnt++;
	}

	if( MynoAccessPerm() == FALSE ){

	}

	DIAGRAM_ATTRIBUTE	DA;

	// ANK交じり対応
	int edit_id[] =	{ID2010,								//相続人等の代表者の氏名
					ID6020, ID6021, ID6041, ID6050, ID6051,	// 事業承継追加 2020/05/14
					ID5011,ID5012,ID5013,ID5014,			//住所又は居所
					ID5031,ID5032,ID5033,ID5034,			//氏名
					ID5051,ID5052,ID5053,ID5054,			//職業
					ID5061,ID5062,ID5063,ID5064,			//続柄
					ID5191_1,ID5192_1,ID5193_1,ID5194_1,	//銀行名
					ID5201_1,ID5202_1,ID5203_1,ID5204_1,	//支店名
					ID5241,ID5242,ID5243,ID5244};			//郵便局名

	for( int cnt = 0; cnt < (sizeof(edit_id)/sizeof(edit_id[0])); cnt++ ){
		m_Fhyo6Diag.GetAttribute( edit_id[cnt], (LPUNKNOWN)&DA );
		DA.attr_editattr |= DIAG_EATTR_ZENHAN;
		m_Fhyo6Diag.SetAttribute( edit_id[cnt], (LPUNKNOWN)&DA, TRUE );
	}

	if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){

		ChangeColor( IDC_ICSDIAGCTRL1, ID1040, 0 );
		ChangeColor( IDC_ICSDIAGCTRL1, ID2010, 0 );
		ChangeColor( IDC_ICSDIAGCTRL1, ID3010, 0 );
		
		for( int idx = ID5011; idx <= ID5124; idx++ ){
			ChangeColor( IDC_ICSDIAGCTRL1, idx, 0 );	
		}
		for( int idx = ID5191_1; idx <= ID5244; idx++ ){
			ChangeColor( IDC_ICSDIAGCTRL1, idx, 0 );	
		}

		m_ctrl_mynum.ShowWindow(SW_SHOW);
	}else{
		m_ctrl_mynum.ShowWindow(SW_HIDE);
	}

	if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){
		for( int idx = 0; idx < SOZOKU_PER_PAGE; idx++ ){
			int test = m_pArith->l_test(XM26);
			if( m_pArith->l_test(XM26) > 0 ){
				diag_modify( IDC_ICSDIAGCTRL1, NOFUZEI_ID[idx], DIAG_MDFY_EDIT, CIcsdiagctrl );
				diag_modify( IDC_ICSDIAGCTRL1, NOFUCHIHO_ID[idx], DIAG_MDFY_EDIT, CIcsdiagctrl );
				diag_modify( IDC_ICSDIAGCTRL1, KANPUZEI_ID[idx], DIAG_MDFY_READONLY, CIcsdiagctrl );
				diag_modify( IDC_ICSDIAGCTRL1, KANPUCHIHO_ID[idx], DIAG_MDFY_READONLY, CIcsdiagctrl );

				ChangeColor( IDC_ICSDIAGCTRL1, NOFUZEI_ID[idx], 0 );
				ChangeColor( IDC_ICSDIAGCTRL1, NOFUCHIHO_ID[idx], 0 );
				ChangeColor( IDC_ICSDIAGCTRL1, KANPUZEI_ID[idx], 1 );
				ChangeColor( IDC_ICSDIAGCTRL1, KANPUCHIHO_ID[idx], 1 );
			}else if( m_pArith->l_test(XM26) < 0 ){
				diag_modify( IDC_ICSDIAGCTRL1, NOFUZEI_ID[idx], DIAG_MDFY_READONLY, CIcsdiagctrl );
				diag_modify( IDC_ICSDIAGCTRL1, NOFUCHIHO_ID[idx], DIAG_MDFY_READONLY, CIcsdiagctrl );
				diag_modify( IDC_ICSDIAGCTRL1, KANPUZEI_ID[idx],  DIAG_MDFY_EDIT, CIcsdiagctrl );
				diag_modify( IDC_ICSDIAGCTRL1, KANPUCHIHO_ID[idx], DIAG_MDFY_EDIT, CIcsdiagctrl );

				ChangeColor( IDC_ICSDIAGCTRL1, NOFUZEI_ID[idx], 1 );
				ChangeColor( IDC_ICSDIAGCTRL1, NOFUCHIHO_ID[idx], 1 );
				ChangeColor( IDC_ICSDIAGCTRL1, KANPUZEI_ID[idx], 0 );
				ChangeColor( IDC_ICSDIAGCTRL1, KANPUCHIHO_ID[idx], 0 );
			}else{
				diag_modify( IDC_ICSDIAGCTRL1, NOFUZEI_ID[idx], DIAG_MDFY_READONLY, CIcsdiagctrl );
				diag_modify( IDC_ICSDIAGCTRL1, NOFUCHIHO_ID[idx], DIAG_MDFY_READONLY, CIcsdiagctrl );
				diag_modify( IDC_ICSDIAGCTRL1, KANPUZEI_ID[idx], DIAG_MDFY_READONLY, CIcsdiagctrl );
				diag_modify( IDC_ICSDIAGCTRL1, KANPUCHIHO_ID[idx], DIAG_MDFY_READONLY, CIcsdiagctrl );

				ChangeColor( IDC_ICSDIAGCTRL1, NOFUZEI_ID[idx], 1 );
				ChangeColor( IDC_ICSDIAGCTRL1, NOFUCHIHO_ID[idx], 1 );
				ChangeColor( IDC_ICSDIAGCTRL1, KANPUZEI_ID[idx], 1 );
				ChangeColor( IDC_ICSDIAGCTRL1, KANPUCHIHO_ID[idx], 1 );
			}
		}
	}

	if( (*m_pSnHeadData)->Sn_Sign4&0x01){
		GetDlgItem(IDC_SOZOKU_INS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SOZOKU_DEL)->ShowWindow(SW_HIDE);
	}else{
		GetDlgItem(IDC_SOZOKU_INS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SOZOKU_DEL)->ShowWindow(SW_SHOW);
	}	

	m_Fhyo6Diag.EnableDelete(0);

	return 0;
}

void CShinFhyo6::dsp_disp( short Index, CString disp )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	
	DiagData.data_disp = disp;
	diag_setdata( IDC_ICSDIAGCTRL1, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
}

void CShinFhyo6::dsp_edit( short Index, CString edit )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	
	DiagData.data_edit = edit;
	diag_setdata( IDC_ICSDIAGCTRL1, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
}

void CShinFhyo6::dsp_combo( short Index, short combo )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	
	DiagData.data_combo = combo;
	diag_setdata( IDC_ICSDIAGCTRL1, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
}

void CShinFhyo6::dsp_check( short Index, short check )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	
	DiagData.data_check = check;
	diag_setdata( IDC_ICSDIAGCTRL1, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
}

void CShinFhyo6::dsp_val( short Index, char *val )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAGCTRL1, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
}

void CShinFhyo6::dsp_day( short Index, char *day )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	
	diag_getdata( IDC_ICSDIAGCTRL1, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( DiagData.data_day, day, sizeof(DiagData.data_day) );
	diag_setdata( IDC_ICSDIAGCTRL1, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
}

//-----------------------------------------------------------------------------
// 金額表示クリア
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//-----------------------------------------------------------------------------
void CShinFhyo6::dsp_cls( unsigned short Id, short Index )
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	DIAGRAM_ATTRIBUTE        DA;
	diag_getattr( Id, Index, &DA, CIcsdiagctrl );

	if( DA.attr_inpkind == 0 ){ //エディット
		if( DA.attr_editkind == 3 ){ //日付
			memset( DiagData.data_day, '\0', sizeof( DiagData.data_day ));
		}else
		if( DA.attr_editkind == 4 ){ //金額
			memset( DiagData.data_val, '\0', sizeof(DiagData.data_val) );
		}else{
			diag_clear( Id, Index, TRUE, CIcsdiagctrl );
		}
	}else 
	if( DA.attr_inpkind == 1 ){ //コンボボックス
		DiagData.data_combo = 0;
	}else 
	if( DA.attr_inpkind == 2 ){ //チェック
		DiagData.data_check = 0;		
	}

	diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
}

	// 各入力データ抽出
void CShinFhyo6::GetAllDiagData()
{

	DIAGRAM_DATA	DiagData;

	char			tmp[MONY_BUF_SIZE] = {0};
	char			PW1[6];

	memset( PW1, '\0', 6 );
	m_pArith->l_input( PW1, _T("100") );

	char nofu_kanpu_sgn = 0x00;
	if (m_pArith->l_test(XM26) > 0) {
		nofu_kanpu_sgn = 0x02;
	}
	else
		if (m_pArith->l_test(XM26) < 0) {
			nofu_kanpu_sgn = 0x01;
		}
		else {
			nofu_kanpu_sgn = 0x03;
		}


	DIAGRAM_DATA_clear( &DiagData );
	diag_getdata( IDC_ICSDIAGCTRL1, ID1040, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( Sn_F6_DEATH, DiagData.data_day, sizeof(Sn_F6_DEATH));

	// 事業承継の有無
	DIAGRAM_DATA_clear( &DiagData );
	diag_getdata ( IDC_ICSDIAGCTRL1, ID6010, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	Sn_F6_USE = DiagData.data_check;

	if ( Sn_F6_USE ) {
		int len = 0;
		char buf[128] = { 0 };

		// 住所等
		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata ( IDC_ICSDIAGCTRL1, ID6020, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		len = DiagData.data_edit.GetLength();
		memset ( buf, '\0', sizeof ( buf ) );
		memmove ( buf, DiagData.data_edit, len );
		memmove ( Sn_F6_ADR, buf, 40 );

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata ( IDC_ICSDIAGCTRL1, ID6021, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		len = DiagData.data_edit.GetLength();
		memset ( buf, '\0', sizeof ( buf ) );
		memmove ( buf, DiagData.data_edit, len );
		memmove ( &Sn_F6_ADR[40], buf, 40 );

		// 電話番号
		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata ( IDC_ICSDIAGCTRL1, ID6030, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		Sn_F6_TELNO1 = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata ( IDC_ICSDIAGCTRL1, ID6031, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		Sn_F6_TELNO2 = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata ( IDC_ICSDIAGCTRL1, ID6032, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		Sn_F6_TELNO3 = DiagData.data_edit;

		// フリガナ
		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata ( IDC_ICSDIAGCTRL1, ID6040, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		Sn_F6_NMFRI = DiagData.data_edit;

		// 氏名
		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata ( IDC_ICSDIAGCTRL1, ID6041, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		Sn_F6_NAME = DiagData.data_edit;

		// その他参考事項
		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata ( IDC_ICSDIAGCTRL1, ID6050, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		len = DiagData.data_edit.GetLength();
		memset ( buf, '\0', sizeof ( buf ) );
		memmove ( buf, DiagData.data_edit, len );
		memmove ( Sn_F6_SONOTA, buf, 40 );

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata ( IDC_ICSDIAGCTRL1, ID6051, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		len = DiagData.data_edit.GetLength();
		memset ( buf, '\0', sizeof ( buf ) );
		memmove ( buf, DiagData.data_edit, len );
		memmove ( &Sn_F6_SONOTA[40], buf, 40 );
	}

	DIAGRAM_DATA_clear( &DiagData );
	diag_getdata( IDC_ICSDIAGCTRL1, ID2010, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	Sn_F6_DAIHYO = DiagData.data_edit;
	DIAGRAM_DATA_clear( &DiagData );
	diag_getdata( IDC_ICSDIAGCTRL1, ID3010, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	Sn_F6_LIMIT = DiagData.data_check;


	for( int idx = 0; idx < SOZOKU_PER_PAGE; idx++ ){

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, JUSYO_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_JUSYO = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, FURI_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_FURI = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, NAME_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_NAME = DiagData.data_edit;

		if(( m_Fhyo6Diag.GetPosition() == MYNUM1_ID[idx] )||
			( m_Fhyo6Diag.GetPosition() == MYNUM2_ID[idx] )||
			( m_Fhyo6Diag.GetPosition() == MYNUM3_ID[idx] )){

			//diag_getdata( IDC_ICSDIAGCTRL1, MYNUM1_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			//diag_getdata( IDC_ICSDIAGCTRL1, MYNUM2_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			//diag_getdata( IDC_ICSDIAGCTRL1, MYNUM3_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );

				CStringArray	MNAry;
				if( m_ctrl_mynum.GetData( MNAry, 0 ) == 0 ){
					sozoku[idx+m_Lin].Sn_F6_MYNUM1 = MNAry.GetAt(0);
					sozoku[idx+m_Lin].Sn_F6_MYNUM2 = MNAry.GetAt(1);
					sozoku[idx+m_Lin].Sn_F6_MYNUM3 = MNAry.GetAt(2);
				}
		}

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, SYOKU_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_SYOKU = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, TSUDUKI_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_TSUDUKI = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		memset( DiagData.data_day, '\0', sizeof( DiagData.data_day ));
		diag_getdata( IDC_ICSDIAGCTRL1, BIRTH_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		memmove( sozoku[idx+m_Lin].Sn_F6_BIRTH, DiagData.data_day, sizeof(sozoku[idx+m_Lin].Sn_F6_BIRTH) );

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, TEL1_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_TEL1 = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, TEL2_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_TEL2 = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, TEL3_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_TEL3 = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, HOTEI_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_HOTEI = DiagData.data_combo;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, BUNSHI_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		if( atoi( DiagData.data_edit ) == 0 ){
			sozoku[idx+m_Lin].Sn_F6_BUNSHI.Empty();
		}else{
			sozoku[idx+m_Lin].Sn_F6_BUNSHI = DiagData.data_edit;
		}

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, BUNBO_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		if( atoi( DiagData.data_edit ) == 0 ){
			sozoku[idx+m_Lin].Sn_F6_BUNBO.Empty();
		}else{
			sozoku[idx+m_Lin].Sn_F6_BUNBO = DiagData.data_edit;
		}

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, KAGAKU_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		memmove( sozoku[idx+m_Lin].Sn_F6_KAGAKU, DiagData.data_val, sizeof(sozoku[idx+m_Lin].Sn_F6_KAGAKU) );

		if( m_pArith->l_test(XM26) > 0 ){
			int cmp = 0;
			DIAGRAM_DATA_clear( &DiagData );
			diag_getdata( IDC_ICSDIAGCTRL1, NOFUZEI_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		
			if( sozoku[idx+m_Lin].Sn_F6_NOFUKANPUsw&0x01 ){
				cmp = m_pArith->l_cmp( sozoku[idx+m_Lin].Sn_F6_NOFUZEI, DiagData.data_val );
			}else{
				cmp = m_pArith->l_cmp( F69[idx+m_Lin], DiagData.data_val );
			}
			if( cmp != 0 ){
				sozoku[idx+m_Lin].Sn_F6_NOFUKANPUsw |= 0x01;
				memmove( tmp, DiagData.data_val, sizeof(tmp) );
				m_pArith->l_div( tmp, tmp, PW1 );
				m_pArith->l_mul( tmp, tmp, PW1 );
				memmove( sozoku[idx+m_Lin].Sn_F6_NOFUZEI, tmp, sizeof(sozoku[idx+m_Lin].Sn_F6_NOFUZEI) );
			}

			cmp = 0;
			DIAGRAM_DATA_clear( &DiagData );
			diag_getdata( IDC_ICSDIAGCTRL1, NOFUCHIHO_ID[idx]	, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			if( sozoku[idx+m_Lin].Sn_F6_NOFUKANPUsw&0x02 ){
				cmp = m_pArith->l_cmp( sozoku[idx+m_Lin].Sn_F6_NOFUCHIHO, DiagData.data_val );
			}else{
				cmp = m_pArith->l_cmp( F610[idx+m_Lin], DiagData.data_val );
			}
			if( cmp != 0 ){
				sozoku[idx+m_Lin].Sn_F6_NOFUKANPUsw |= 0x02;
				memmove( tmp, DiagData.data_val, sizeof(tmp) );
				m_pArith->l_div( tmp, tmp, PW1 );
				m_pArith->l_mul( tmp, tmp, PW1 );
				memmove( sozoku[idx+m_Lin].Sn_F6_NOFUCHIHO, tmp, sizeof(sozoku[idx+m_Lin].Sn_F6_NOFUCHIHO) );
			}
		}

		if( m_pArith->l_test(XM26) < 0 ){
			DIAGRAM_DATA_clear( &DiagData );
			diag_getdata( IDC_ICSDIAGCTRL1, KANPUZEI_ID[idx]	, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( sozoku[idx+m_Lin].Sn_F6_KANPUZEI, DiagData.data_val, sizeof(sozoku[idx+m_Lin].Sn_F6_KANPUZEI) );

			DIAGRAM_DATA_clear( &DiagData );
			diag_getdata( IDC_ICSDIAGCTRL1, KANPUCHIHO_ID[idx]	, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( sozoku[idx+m_Lin].Sn_F6_KANPUCHIHO, DiagData.data_val, sizeof(sozoku[idx+m_Lin].Sn_F6_KANPUCHIHO) );
		}

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, GNAME_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_GNAME = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, GSEL_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_GSEL = DiagData.data_combo;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, SNAME_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_SNAME = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, SSEL_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_SSEL = DiagData.data_combo;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, YOKIN_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_YOKIN = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, KOUZA_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_KOUZA = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, KIGO1_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_KIGO1 = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, KIGO2_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_KIGO2 = DiagData.data_edit;

		DIAGRAM_DATA_clear( &DiagData );
		diag_getdata( IDC_ICSDIAGCTRL1, YUBIN_ID[idx]		, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		sozoku[idx+m_Lin].Sn_F6_YUBIN = DiagData.data_edit;

		if((IsNULL_Column(idx + m_Lin, nofu_kanpu_sgn) == FALSE)&&(sozoku[idx + m_Lin].Sn_NO ==-1)) {
			sozoku[idx + m_Lin].Sn_NO = idx + m_Lin;
		}
	}

	return;

}

//フォーカスが個人番号入力上にあり、個人番号が確定された場合にフォーカスをずらす
void CShinFhyo6::MoveFocus_FromMyNum()
{
	for(int idx = 0; idx < SOZOKU_PER_PAGE; idx++ ){
		if(( m_Fhyo6Diag.GetPosition() == MYNUM1_ID[idx] )||
			( m_Fhyo6Diag.GetPosition() == MYNUM2_ID[idx] )||
			( m_Fhyo6Diag.GetPosition() == MYNUM3_ID[idx] )){

				if( m_ctrl_mynum.CheckNumber( sozoku[idx+m_Lin].Sn_F6_MYNUM1+sozoku[idx+m_Lin].Sn_F6_MYNUM2+sozoku[idx+m_Lin].Sn_F6_MYNUM3) == 1 ){
					DispFh6();
					m_Fhyo6Diag.SetPosition(FocusMoveCheck(MYNUM3_ID[idx] , 0));
				}
		}
	}
}

// 相続人欄のスクロールバーを画面最下部に移動
void CShinFhyo6::MoveWindow_SozokuBar()
{
	if( !m_bar ){
		return;
	}

	CRect crect_bar,crect_window;
	long hight;
	//this->GetWindowRect( &crect_window );
	m_pParent->GetWindowRect( &crect_window );
	m_bar.GetWindowRect( &crect_bar );
	if(( crect_window.bottom < 0 )||(crect_window.top < 0)
		||(crect_window.left < 0 )||(crect_window.right < 0 )){
			return;
	}
	ScreenToClient( &crect_window );
	ScreenToClient( &crect_bar );
	hight = crect_bar.bottom - crect_bar.top;
	crect_bar.bottom = crect_window.bottom;
	crect_bar.top = crect_bar.bottom - hight;
	m_bar.MoveWindow( &crect_bar );
}

/////////////////////////////////////////////////
//処理概要：指定の相続人列が空か判定
//
//返送値　：TRUE…空 FALSE…データ有
//引数　　：line_idx…指定のindex
//　　　　：sgn…除外項目の指定
//　　　　：　　 D0…各人の納付税額　D1…各人の還付税額
/////////////////////////////////////////////////
BOOL CShinFhyo6::IsNULL_Column( int line_idx, char sgn )
{
	BOOL ret = TRUE;

	CString str;

	str = sozoku[line_idx].Sn_F6_JUSYO;
	str.Replace(_T("　"),"");
	if( str.Trim().GetLength() != 0 ){
		return FALSE;
	}
	str = sozoku[line_idx].Sn_F6_FURI;
	str.Replace(_T("　"),"");
	if( str.Trim().GetLength() != 0 ){
		return FALSE;
	}
	str = sozoku[line_idx].Sn_F6_NAME;
	str.Replace(_T("　"),"");
	if( str.Trim().GetLength() != 0 ){
		return FALSE;
	}
	for( int idx = 0; idx < sizeof(sozoku[line_idx].Sn_F6_HASH); idx++ ){
		if( sozoku[line_idx].Sn_F6_HASH[idx] != '\0' ){
			return FALSE;	
		}
	}
	str = sozoku[line_idx].Sn_F6_SYOKU;
	str.Replace(_T("　"),"");
	if( str.Trim().GetLength() != 0 ){
		return FALSE;
	}
	str = sozoku[line_idx].Sn_F6_TSUDUKI;
	str.Replace(_T("　"),"");
	if( str.Trim().GetLength() != 0 ){
		return FALSE;
	}
	if( sozoku[line_idx].Sn_F6_BIRTH[0] != '\0' ){
		return FALSE;
	}
	if( sozoku[line_idx].Sn_F6_BIRTH[1] != '\0' ){
		return FALSE;
	}
	if( sozoku[line_idx].Sn_F6_BIRTH[2] != '\0' ){
		return FALSE;
	}
	if( sozoku[line_idx].Sn_F6_BIRTH[3] != '\0' ){
		return FALSE;
	}
	if( sozoku[line_idx].Sn_F6_TEL1.IsEmpty() == FALSE ){
		return FALSE;	
	}
	if( sozoku[line_idx].Sn_F6_TEL2.IsEmpty() == FALSE ){
		return FALSE;	
	}
	if( sozoku[line_idx].Sn_F6_TEL3.IsEmpty() == FALSE ){
		return FALSE;	
	}
	if( sozoku[line_idx].Sn_F6_HOTEI != 0 ){
		return FALSE;
	}
	if( sozoku[line_idx].Sn_F6_BUNSHI.IsEmpty() == FALSE ){
		return FALSE;
	}
	if( sozoku[line_idx].Sn_F6_BUNBO.IsEmpty() == FALSE ){
		return FALSE;
	}
	if( m_pArith->l_test( sozoku[line_idx].Sn_F6_KAGAKU ) != 0 ){
		return FALSE;
	}
	if( !(sgn&0x01) ){
		if( m_pArith->l_test( sozoku[line_idx].Sn_F6_NOFUZEI ) != 0 ){
			return FALSE;
		}
		if( m_pArith->l_test( sozoku[line_idx].Sn_F6_NOFUCHIHO ) != 0 ){
			return FALSE;
		}
	}
	if( !(sgn&0x02) ){
		if( m_pArith->l_test( sozoku[line_idx].Sn_F6_KANPUZEI ) != 0 ){
			return FALSE;
		}
		if( m_pArith->l_test( sozoku[line_idx].Sn_F6_KANPUCHIHO ) != 0 ){
			return FALSE;
		}
	}
	str = sozoku[line_idx].Sn_F6_GNAME;
	str.Replace(_T("　"),"");
	if( str.Trim().GetLength() != 0 ){
		return FALSE;
	}
	if( sozoku[line_idx].Sn_F6_GSEL != 0 ){
		return FALSE;
	}
	str = sozoku[line_idx].Sn_F6_SNAME;
	str.Replace(_T("　"),"");
	if( str.Trim().GetLength() != 0 ){
		return FALSE;
	}
	if( sozoku[line_idx].Sn_F6_SSEL != 0 ){
		return FALSE;
	}
	str = sozoku[line_idx].Sn_F6_YOKIN;
	str.Replace(_T("　"),"");
	if( str.Trim().GetLength() != 0 ){
		return FALSE;
	}
	if( sozoku[line_idx].Sn_F6_KOUZA.IsEmpty() == FALSE ){
		return FALSE;	
	}
	if( sozoku[line_idx].Sn_F6_KIGO1.IsEmpty() == FALSE ){
		return FALSE;	
	}
	if( sozoku[line_idx].Sn_F6_KIGO2.IsEmpty() == FALSE ){
		return FALSE;	
	}
	str = sozoku[line_idx].Sn_F6_YUBIN;
	str.Replace(_T("　"),"");
	if( str.Trim().GetLength() != 0 ){
		return FALSE;
	}
	
	return TRUE;
}

BEGIN_EVENTSINK_MAP(CShinFhyo6, CSyzBaseDlg)
ON_EVENT(CShinFhyo6, IDC_ICSDIAGCTRL1, 1, CShinFhyo6::EditONIcsdiagctrl1, VTS_I2)
ON_EVENT(CShinFhyo6, IDC_ICSDIAGCTRL1, 2, CShinFhyo6::EditOFFIcsdiagctrl1, VTS_I2)
ON_EVENT(CShinFhyo6, IDC_ICSDIAGCTRL1, 3, CShinFhyo6::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
ON_EVENT(CShinFhyo6, IDC_ICSDIAGCTRL1, 8, CShinFhyo6::ComboSelIcsdiagctrl1, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo6::EditOFFIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	OnEditOFFCallBack(index);

//	m_EditOff_sw = 1;
//	DiagOcxIniz( &DiagData );
//	diag_getdata( IDC_ICSDIAGCTRL1, index, &DiagData,  CIcsdiagctrl );
//	//if( m_Clm1 == SDSKBN || m_Clm1 == SDRKBN );
//	//else	Dig1DataSet( index, data.data_check, &data );
//	else	Dig1DataSet( index, data.data_check, &data );
//	m_EditOff_sw = 0;
////	diag_deledit( IDC_ICSDIAGCTRL1, CICSDiagEx );

	//sozoku[m_Lin+line_idx].Sn_SEQ = 0;
	//sozoku[m_Lin+line_idx].Sn_NO = line_idx;
}

int CShinFhyo6::OnTerminationCallBack(short index, short nChar, short length, LPUNKNOWN data)
{

	if( nChar == 0 ){
		return FALSE;
	}
	
	//if( ((CH31HyoView*)m_pParent)->IsEndStart() ){
	//	return 0;
	//}	

	int ret = TerminationMyNumber( index, nChar, length, data );
	if( ret != -1){
		return ret;
	}

	char	buf[512] = {0} , buff[64] = {0};
	int		len = 0;
	CString	ZipBf, AddBf;
	CString str;
	short	wTerm;
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;


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
	BOOL scroll = FALSE;
	if( wTerm==VK_TAB ){
		pos = FocusMoveCheck(index , 1);
		if( IsLeftEnd(index)&&(index==ID_UPPER_LEFT)&&(m_Lin!=0) ){ //左上端andNOT最初ページ
			F6DataSet(index);
			m_Fhyo6Diag.SetPosition(ID_LOWER_LEFT);
			m_Lin--;
			m_bar.SetScrollPos(m_Lin);			//スクロールバーの移動
			scroll = TRUE;

			DispFh6();
		}else{
			if ( index == ID6020 ) {
				if ( Sn_F6_USE ) {
					m_Fhyo6Diag.SetPosition ( ID6010 );
				}
				else {
					m_Fhyo6Diag.SetPosition ( ID6011 );
				}
			}
			else if ( index == ID2010 ) {
				if ( Sn_F6_USE ) {
					m_Fhyo6Diag.SetPosition ( ID6051 );
				}
				else {
					m_Fhyo6Diag.SetPosition ( ID6011 );
				}		}
			else if ( ( index == ID6010 ) || ( index == ID6011 ) ) {
				m_Fhyo6Diag.SetPosition ( ID1040 );
			}
			else {
				m_Fhyo6Diag.SetPosition(pos);
			}
		}
	}
	if((wTerm==VK_UP)||(wTerm==VK_F2)){
		if( IsComboBox(index)&&(wTerm!=VK_F2)){
			m_Fhyo6Diag.ComboDropDown( (short)index, TRUE );
			return 0;
		}
		pos = FocusMoveCheck(index , 1);
		if( IsLeftEnd(index)&&(index==ID_UPPER_LEFT)&&(m_Lin!=0) ){ //左上端andNOT最初ページ
			F6DataSet(index);
			m_Fhyo6Diag.SetPosition(ID_LOWER_LEFT);
			m_Lin--;
			m_bar.SetScrollPos(m_Lin);			//スクロールバーの移動
			scroll = TRUE;

			DispFh6();
		}else{
			if ( ( index == ID6020 ) || ( index == ID6050 ) ) {
				if ( Sn_F6_USE ) {
					m_Fhyo6Diag.SetPosition ( ID6010 );
				}
				else {
					m_Fhyo6Diag.SetPosition ( ID6011 );
				}
			}
			else if ( index == ID2010 ) {
				if ( Sn_F6_USE ) {
					m_Fhyo6Diag.SetPosition ( ID6051 );
				}
				else {
					m_Fhyo6Diag.SetPosition ( ID6011 );
				}
			}
			else if ( ( index == ID6010 ) || ( index == ID6011 ) ) {
				m_Fhyo6Diag.SetPosition ( ID1040 );
			}
			else {
				m_Fhyo6Diag.SetPosition(pos);
			}
		}
	}
	if(wTerm==VK_RETURN){
		pos = FocusMoveCheck(index , 0);
		if( IsRightEnd(index)&&(pos==index)&&(m_Lin!=SOZOKU_MAX-SOZOKU_PER_PAGE) ){ //右下端andNOT最終ページ
			F6DataSet(index);
			m_Fhyo6Diag.SetPosition(ID_UPPER_RIGHT);
			m_Lin++;
			m_bar.SetScrollPos(m_Lin);			//スクロールバーの移動
			scroll = TRUE;

			DispFh6();
		}else{
			if ( index == ID1040 ) {
				if ( Sn_F6_USE ) {
					m_Fhyo6Diag.SetPosition ( ID6010 );
				}
				else {
					m_Fhyo6Diag.SetPosition ( ID6011 );
				}
			}
			else if ( ( index == ID6010 ) || ( index == ID6011 ) ) {
				if ( Sn_F6_USE ) {
					m_Fhyo6Diag.SetPosition ( ID6020 );
				}
				else {
					m_Fhyo6Diag.SetPosition ( ID2010 );
				}
			}
			else {
				m_Fhyo6Diag.SetPosition(pos);
			}
		}
	}
	if((wTerm==VK_DOWN)||(wTerm==VK_F3)){
		if( IsComboBox(index)&&(wTerm!=VK_F3)){
			m_Fhyo6Diag.ComboDropDown( (short)index, TRUE );
			return 0;
		}
		pos = FocusMoveCheck(index , 0);
		if( IsRightEnd(index)&&(pos==index)&&(m_Lin!=SOZOKU_MAX-SOZOKU_PER_PAGE) ){ //右下端andNOT最終ページ
			F6DataSet(index);
			m_Fhyo6Diag.SetPosition(ID_UPPER_RIGHT);
			m_Lin++;
			m_bar.SetScrollPos(m_Lin);			//スクロールバーの移動
			scroll = TRUE;

			DispFh6();
		}else{
			if ( ( index == ID6010 ) || ( index == ID6011 ) ) {
				if ( Sn_F6_USE ) {
					m_Fhyo6Diag.SetPosition ( ID6050 );
				}
				else {
					m_Fhyo6Diag.SetPosition ( ID2010 );
				}
			}
			else {
				m_Fhyo6Diag.SetPosition(pos);
			}
		}
	}
	if(wTerm==VK_RIGHT){
		if( IsRightEnd(index) ){
			int line = GetLineNumber_FromIndex( index );
			if( line < SOZOKU_MAX - 1){
				BOOL empty = MyNumber_IsHashEmpty( sozoku[line+1].Sn_F6_HASH, sizeof(sozoku[line+1].Sn_F6_HASH));
				if( IsMyNumberEdit(index)&&!empty ){
					pos = FocusMoveCheck(index , 0);
					m_Fhyo6Diag.SetPosition(pos);
				}
				F6DataSet(index);
				m_Lin++;
				m_bar.SetScrollPos(m_Lin);			//スクロールバーの移動
				scroll = TRUE;
			}else{
				F6DataSet(index);
			}

			DispFh6();

			int pos;
			switch( index ){
				case ID5064: //続柄
					pos = ID5054;
					break;
				case ID5084_3: //電話番号
					pos = ID5084_1;
					break;
				case ID5114: //相続分
					pos = ID5094;
					break;
				case ID5234_2: //記号番号
					pos = ID5234_1;
					break;
				default:
					pos = index;
			}
			m_Fhyo6Diag.SetPosition(pos);
		}else{
			if ( ( index == ID6010 ) || ( index == ID6011 ) ) {
				if ( !Sn_F6_USE ) {
					Sn_F6_USE = 1;
					dsp_check ( ID6010, 1 );
					dsp_check ( ID6011, 0 );
					ModifyDiagInput();
					m_Fhyo6Diag.SetPosition ( ID6010 );
				}
				else {
					Sn_F6_USE = 0;
					dsp_check ( ID6010, 0 );
					dsp_check ( ID6011, 1 );
					ModifyDiagInput();
					m_Fhyo6Diag.SetPosition ( ID6011 );
				}
			}
			else {
				pos = FocusMoveCheck(index , 2);
				m_Fhyo6Diag.SetPosition(pos);
			}
		}
	}
	//else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
	else if(wTerm==VK_LEFT){
		if( IsLeftEnd(index) ){
			int line = GetLineNumber_FromIndex( index );
			if( line > 0){
				BOOL empty = MyNumber_IsHashEmpty( sozoku[line-1].Sn_F6_HASH, sizeof(sozoku[line-1].Sn_F6_HASH));
				if( IsMyNumberEdit(index)&&!empty ){
					pos = FocusMoveCheck(index , 1);
					m_Fhyo6Diag.SetPosition(pos);
				}
				F6DataSet(index);
				m_Lin--;
				m_bar.SetScrollPos(m_Lin);			//スクロールバーの移動
				scroll = TRUE;
			}else{
				F6DataSet(index);
			}

			DispFh6();
			m_Fhyo6Diag.SetPosition(index);
		}else{
			if ( ( index == ID6010 ) || ( index == ID6011 ) ) {
				if ( !Sn_F6_USE ) {
					Sn_F6_USE = 1;
					dsp_check ( ID6010, 1 );
					dsp_check ( ID6011, 0 );
					ModifyDiagInput();
					m_Fhyo6Diag.SetPosition ( ID6010 );
				}
				else {
					Sn_F6_USE = 0;
					dsp_check ( ID6010, 0 );
					dsp_check ( ID6011, 1 );
					ModifyDiagInput();
					m_Fhyo6Diag.SetPosition ( ID6011 );
				}
			}
			else if ( index == ID6020 ) {
				if ( Sn_F6_USE ) {
					m_Fhyo6Diag.SetPosition ( ID6010 );
				}
				else {
					m_Fhyo6Diag.SetPosition ( ID6011 );
				}
			}
			else {
				pos = FocusMoveCheck(index , 3);
				m_Fhyo6Diag.SetPosition(pos);
			}
		}
	}

	if( wTerm == VK_DELETE ){
		int line = GetLineNumber_FromIndex( index );

		switch(index){
			case ID5131:
			case ID5132:
			case ID5133:
			case ID5134:
				sozoku[line].Sn_F6_NOFUKANPUsw &= ~0x01;
				memmove( sozoku[line].Sn_F6_NOFUZEI, F69[line], MONY_BUF_SIZE );
				DispFh6();
				break;
			case ID5141:
			case ID5142:
			case ID5143:
			case ID5144:
				sozoku[line].Sn_F6_NOFUKANPUsw &= ~0x02;
				memmove( sozoku[line].Sn_F6_NOFUCHIHO, F610[line], MONY_BUF_SIZE );
				DispFh6();
				break;
			case ID5071:
			case ID5072:
			case ID5073:
			case ID5074:
			case ID1040:
				dsp_cls( IDC_ICSDIAGCTRL1, (short )index );
				m_Fhyo6Diag.SetPosition(index);
				break;
			default:
				dsp_cls( IDC_ICSDIAGCTRL1, (short )index );
				break;
		}
	}

	if( wTerm == VK_BACK ){
		int line = GetLineNumber_FromIndex( index );

		switch(index){
			case ID5071:
			case ID5072:
			case ID5073:
			case ID5074:
				dsp_cls( IDC_ICSDIAGCTRL1, (short )index );
				m_Fhyo6Diag.SetPosition(index);
				break;
			default:
				break;
		}
	}

	if( wTerm == VK_HOME ){
		int line = GetLineNumber_FromIndex( index );
		CString zip,add;
		switch(index){
			case ID5011:
			case ID5012:
			case ID5013:
			case ID5014:
			case ID6020:
			case ID6021:
				if( ZipToAddress(zip, add) == TRUE ){
					if ( index == ID6020 ) {
						memset ( buf, '\0', sizeof ( buf ) );
						len = add.GetLength();
						memmove ( buf, add, len );
						memmove ( Sn_F6_ADR, buf, 40 );
					}
					else if ( index == ID6021 ) {
						memset ( buf, '\0', sizeof ( buf ) );
						len = add.GetLength();
						memmove ( buf, add, len );
						memmove ( &Sn_F6_ADR[40], buf, 40 );
					}
					else {
						Insert_kaigyo( 20, 4, &add );
						sozoku[line].Sn_F6_JUSYO = add;
						if( add.IsEmpty() == FALSE ){
							sozoku[line].Sn_NO = line;
						}
					}
					DispFh6();
					m_Fhyo6Diag.SetPosition(index);
				}
			default:
				break;
		}
	}

	if( wTerm == VK_INSERT ){
		int line = GetLineNumber_FromIndex( index );
		char	tmp_char1[100] = {0},tmp_char2[100] = {0};
		CString tmp_str1,tmp_str2,tmp_str3;
		if(( index == ID5011 )||( index == ID5012 )||( index == ID5013 )||( index == ID5014 )){
			memmove( tmp_char1, &(*m_pSnHeadData)->Sn_NOZEI[0], 40 );
//2018.08.21 UPDATE START
//			tmp_str1.Format(tmp_char1);
			tmp_str1 = tmp_char1;
//2018.08.21 UPDATE END
			tmp_str1.TrimRight();
			int len1 =  tmp_str1.GetLength();
			Insert_kaigyo( 20, 2, &tmp_str1 );
			memmove( tmp_char2, &(*m_pSnHeadData)->Sn_NOZEI[40], 40 );
//2018.08.21 UPDATE START
//			tmp_str2.Format(tmp_char2);
			tmp_str2 = tmp_char2;
//2018.08.21 UPDATE END
			tmp_str2.TrimRight();
			Insert_kaigyo( 20, 2, &tmp_str2 );
			if( len1 == 20 ){ //この時だけ改行コードが付加されている
				tmp_str3 = tmp_str1 + tmp_str2;
			}else{
				tmp_str3 = tmp_str1 + "\r\n" + tmp_str2;
			}

			//Insert_kaigyo( 20, 4, &tmp_str3 );
			sozoku[line].Sn_F6_JUSYO = tmp_str3;
			if( tmp_str3.IsEmpty() == FALSE ){
				sozoku[line].Sn_NO = line;
			}
			DispFh6();
			m_Fhyo6Diag.SetPosition(index);
		}
	}

	if( IsNeedAllSel(pos) ){
		m_Fhyo6Diag.SetPosition(pos);
		m_Fhyo6Diag.SetCursorPos( pos, -1 );
	}

	int nowpos = m_Fhyo6Diag.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL1, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}

	return 0;
}

////////////////////////////////////////////////////////////////////
//処理概要：改行コードを挿入
//
//引数　　：length…一行のバイト数
//　　　　　line_max…最大行数
//　　　　　str…編集対象文字列
////////////////////////////////////////////////////////////////////
void CShinFhyo6::Insert_kaigyo( int length, int line_max, CString *str )
{
	CString conv_str,tmp_str;
	int len = 0;
	int line = 0;

	tmp_str = *str;

	for( int idx = 0; idx < tmp_str.GetLength(); idx++ ){
		if(tmp_str.Mid(idx,1) == '\0'){
			break;
		}

		if( len == length-1 ){
			line++;
			if( IsKindOfLetter(tmp_str, idx) == 2 ){

				if( line == line_max ){
					break;
				}

				conv_str += "\r\n";
				conv_str += tmp_str.Mid(idx,1);
				len = 1;
			}else{
				conv_str += tmp_str.Mid(idx,1);

				if( line == line_max ){
					break;
				}

				conv_str += "\r\n";
				len = 0;
			}
		}else{
			conv_str += tmp_str.Mid(idx,1);
			len++;
		}
	}
	
	*str = conv_str;
}

//-----------------------------------------------------------------------------
// 全選択が必要なコントロール？（相続分の分子・分母）
//-----------------------------------------------------------------------------
// 引数		nID		：	該当のコントロール
//-----------------------------------------------------------------------------
// 返送値	true	：	全選択が必要
//			false	：	全選択は不要
//-----------------------------------------------------------------------------
bool CShinFhyo6::IsNeedAllSel( int nID )
{
	bool bRt = false;

	if( (nID==ID5101) || (nID==ID5102) || (nID==ID5103) || (nID==ID5104) ){
		bRt = true;
	}

	return bRt;
}

int CShinFhyo6::OnEditONCallBack(short index)
{
	//if( ((CH31HyoView*)m_pParent)->IsEndStart() ){
	//	return 0;
	//}

	if( IsMyNumberEdit(index) ){
		m_ctrl_mynum.ShowWindow( SW_SHOW );
		m_ctrl_mynum.MoveDiagMNEdit(index, TRUE);
		mynum_cursel = 1;
	}else{
		mynum_cursel = 0;

		CString str;
		if( m_ctrl_mynum.GetData(str,0) == 0 ){
			m_ctrl_mynum.SetVisibleMode(0);
		}
	}
	
	//横スクロールバーにフォーカスが隠れるため、移動させる
	CRect crect_diag,crect_window,crect_bar;
	long hight_bar,hight_diag;
	BOOL birth_focus = FALSE;

	if((index==ID5071)||(index==ID5072)||(index==ID5073)||(index==ID5074)){
		birth_focus = TRUE;
	}

	m_Fhyo6Diag.GetRect( index, (LPUNKNOWN)&crect_diag );
	this->GetWindowRect( &crect_window );
	m_bar.GetWindowRect( &crect_bar );
	if(( crect_window.bottom < 0 )||(crect_window.top < 0)
		||(crect_window.left < 0 )||(crect_window.right < 0 )){
			return 0;
	}
	ScreenToClient( &crect_diag );
	ScreenToClient( &crect_window );
	ScreenToClient( &crect_bar );
	hight_bar = crect_bar.bottom - crect_bar.top;
	int move =  crect_diag.bottom - ( crect_window.bottom - hight_bar );
	if( birth_focus ){
		hight_diag = crect_diag.bottom - crect_diag.top;
		move +=  hight_diag;
	}
	if( move <= 0 ){
		return 0;
	}

	SCROLLINFO ScrollInfo;

	if( GetScrollInfo( SB_VERT, &ScrollInfo ) == FALSE ){
		return 0;
	}

	//CSyzBaseDlg側で微調整しているためにSetScrollInfoやScrollWindowだとずれが生じるため
	int line = 0;
	this->LockWindowUpdate();
	if( birth_focus ){
		line = 6;
	}else{
		line = 4;
	}
	for(int cnt = 0; cnt < line; cnt++){
		OnVScrollCallBack(SB_LINEDOWN, ScrollInfo.nPos, 0 );
		if( GetScrollInfo( SB_VERT, &ScrollInfo ) == FALSE ){
			return 0;
		}
	}
	this->UnlockWindowUpdate();

	MoveWindow_SozokuBar();

	return 0;
}

int CShinFhyo6::OnEditOFFCallBack(short index)
{
	mynum_cursel = 0;

	if( m_EditOff_sw ){
		return 0;
	}

	//if( ((CH31HyoView*)m_pParent)->IsEndStart() ){
	//	return 0;
	//}

	if( m_Focus <= 0 ){
		return 0;
	}

	F6DataSet(index);

	return 0;
}

int CShinFhyo6::F6DataSet( int index )
{

	int				ret = 0;
	CStringArray	tmpMNAry;
	DIAGRAM_DATA	DiagData;
	char			buf[256] = {0};
	int				len = 0;
	char			tmpData = 0;
	char			tmp[MONY_BUF_SIZE] = {0};
	char			PW1[6];
	BOOL			redisp = FALSE;
	int				cmp = 0;

	memset( PW1, '\0', 6 );
	m_pArith->l_input( PW1, _T("100") );

	int line_idx = GetLineNumber_FromIndex( index );

	//if(( index == ID1040 )||( index == ID2010 )||( index == ID3010 )){
	if ( ( index == ID1040 ) || ( index == ID2010 ) || ( index == ID3010 )
	  || ( index == ID6010 ) || ( index == ID6011 ) || ( index == ID6020 ) || ( index == ID6021 ) || ( index == ID6030 ) || ( index == ID6031 )
	  || ( index == ID6032 ) || ( index == ID6040 ) || ( index == ID6041 ) || ( index == ID6050 ) || ( index == ID6051 ) ) {

	}else{
		if( line_idx == -1 ){
			return 0;
		}
	}

	DIAGRAM_DATA_clear( &DiagData );
	m_Fhyo6Diag.GetData( (int)index, (struct IUnknown *)&DiagData );


	switch( index ){
		case ID1040:
			if ( DiagData.data_day[0] == 0 ) {
				memset ( Sn_F6_DEATH, '\0', sizeof ( Sn_F6_DEATH ) );
			}
			else if ( DiagData.data_day[0] != -1 ) {
				memmove( Sn_F6_DEATH, DiagData.data_day, sizeof(Sn_F6_DEATH));
				redisp = TRUE;
			}
			break;
		case ID6020:
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( Sn_F6_ADR, buf, 40 );
			break;
		case ID6021:
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( &Sn_F6_ADR[40], buf, 40 );
			break;
		case ID6030:
			Sn_F6_TELNO1 = DiagData.data_edit;
			break;
		case ID6031:
			Sn_F6_TELNO2 = DiagData.data_edit;
			break;
		case ID6032:
			Sn_F6_TELNO3 = DiagData.data_edit;
			break;
		case ID6040:
			Sn_F6_NMFRI = DiagData.data_edit;
			break;
		case ID6041:
			Sn_F6_NAME = DiagData.data_edit;
			break;
		case ID6050:
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( Sn_F6_SONOTA, buf, 40 );
			break;
		case ID6051:
			len = DiagData.data_edit.GetLength();
			memset ( buf, '\0', sizeof ( buf ) );
			memmove ( buf, DiagData.data_edit, len );
			memmove ( &Sn_F6_SONOTA[40], buf, 40 );
			break;
		case 	ID2010:
			Sn_F6_DAIHYO = DiagData.data_edit;
			break;
		case 	ID3010:
			Sn_F6_LIMIT = DiagData.data_check;
			break;
		case 	ID5011:
		case 	ID5012:
		case 	ID5013:
		case 	ID5014:
			sozoku[line_idx].Sn_F6_JUSYO = DiagData.data_edit;
			break;
		case	ID5021:
		case	ID5022:
		case	ID5023:
		case	ID5024:
			sozoku[line_idx].Sn_F6_FURI = DiagData.data_edit;
			break;
		case	ID5031:
		case	ID5032:
		case	ID5033:
		case	ID5034:
			sozoku[line_idx].Sn_F6_NAME = DiagData.data_edit;
			break;
		case	ID5041_1:
		case	ID5041_2:
		case	ID5041_3:
		case	ID5042_1:
		case	ID5042_2:
		case	ID5042_3:
		case	ID5043_1:
		case	ID5043_2:
		case	ID5043_3:
		case	ID5044_1:
		case	ID5044_2:
		case	ID5044_3:
			ret = m_ctrl_mynum.GetData(tmpMNAry, 0);
			if( ret == 0 ){
				if( !tmpMNAry.GetAt(0).IsEmpty() && !tmpMNAry.GetAt(1).IsEmpty() && !tmpMNAry.GetAt(2).IsEmpty() ){
					sozoku[line_idx].Sn_NO = line_idx;
				}
				sozoku[line_idx].Sn_F6_MYNUM1.Format(tmpMNAry.GetAt(0));
				sozoku[line_idx].Sn_F6_MYNUM2.Format(tmpMNAry.GetAt(1));
				sozoku[line_idx].Sn_F6_MYNUM3.Format(tmpMNAry.GetAt(2));

				DispFh6(1);
			}
			break;

		case	ID5051:	
		case	ID5052:	
		case	ID5053:	
		case	ID5054:
			sozoku[line_idx].Sn_F6_SYOKU = DiagData.data_edit;
			break;
		case	ID5061:	
		case	ID5062:	
		case	ID5063:	
		case	ID5064:	
			sozoku[line_idx].Sn_F6_TSUDUKI = DiagData.data_edit;
			break;		
		case	ID5071:	
		case	ID5072:	
		case	ID5073:	
		case	ID5074:	
			if ( DiagData.data_day[0] == 0 ) {
				memset ( sozoku[line_idx].Sn_F6_BIRTH, '\0', sizeof ( sozoku[line_idx].Sn_F6_BIRTH ) );
			}
			else if ( DiagData.data_day[0] != -1 ) {
				memmove( sozoku[line_idx].Sn_F6_BIRTH, DiagData.data_day, sizeof(sozoku[line_idx].Sn_F6_BIRTH) );
				redisp = TRUE;
			}
			break;
		case	ID5081_1:
		case	ID5082_1:
		case	ID5083_1:
		case	ID5084_1:
			sozoku[line_idx].Sn_F6_TEL1 = DiagData.data_edit;
			break;		
		case	ID5081_2:
		case	ID5082_2:
		case	ID5083_2:
		case	ID5084_2:
			sozoku[line_idx].Sn_F6_TEL2 = DiagData.data_edit;
			break;		
		case	ID5081_3:
		case	ID5082_3:
		case	ID5083_3:
		case	ID5084_3:
			sozoku[line_idx].Sn_F6_TEL3 = DiagData.data_edit;
			break;		
		case	ID5091:
		case	ID5092:
		case	ID5093:
		case	ID5094:
			sozoku[line_idx].Sn_F6_HOTEI = DiagData.data_combo;
			break;
		case	ID5101:
		case	ID5102:
		case	ID5103:
		case	ID5104:
			if( atoi( DiagData.data_edit ) == 0 ){
				sozoku[line_idx].Sn_F6_BUNSHI.Empty();
			}else{
				// 頭の0をカット
				//sozoku[line_idx].Sn_F6_BUNSHI = DiagData.data_edit;
				sozoku[line_idx].Sn_F6_BUNSHI.Format ( _T( "%d" ), atoi ( DiagData.data_edit ) );
			}
			redisp = TRUE;
			break;
		case	ID5111:
		case	ID5112:
		case	ID5113:
		case	ID5114:
			if( atoi( DiagData.data_edit ) == 0 ){
				sozoku[line_idx].Sn_F6_BUNBO.Empty();
			}else{
				// 頭の0をカット
				//sozoku[line_idx].Sn_F6_BUNBO = DiagData.data_edit;
				sozoku[line_idx].Sn_F6_BUNBO.Format ( _T( "%d" ), atoi ( DiagData.data_edit ) );
			}
			redisp = TRUE;
			break;
		case	ID5121:
		case	ID5122:
		case	ID5123:
		case	ID5124:
			memmove( sozoku[line_idx].Sn_F6_KAGAKU, DiagData.data_val, sizeof(sozoku[line_idx].Sn_F6_KAGAKU) );
			break;

		case	ID5131:
		case	ID5132:
		case	ID5133:
		case	ID5134:
			memmove( tmp, DiagData.data_val, sizeof(tmp) );
			m_pArith->l_div( tmp, tmp, PW1 );
			m_pArith->l_mul( tmp, tmp, PW1 );

			if( sozoku[line_idx].Sn_F6_NOFUKANPUsw&0x01 ){
				cmp = m_pArith->l_cmp( sozoku[line_idx].Sn_F6_NOFUZEI, tmp );
			}else{
				cmp = m_pArith->l_cmp( F69[line_idx], tmp );
			}
			if( cmp != 0 ){
				sozoku[line_idx].Sn_F6_NOFUKANPUsw |= 0x01;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 3 );
				memmove( sozoku[line_idx].Sn_F6_NOFUZEI, tmp, sizeof(sozoku[line_idx].Sn_F6_NOFUZEI) );
			}
			redisp = TRUE;

			break;

		case	ID5141:
		case	ID5142:
		case	ID5143:
		case	ID5144:
			memmove( tmp, DiagData.data_val, sizeof(tmp) );
			m_pArith->l_div( tmp, tmp, PW1 );
			m_pArith->l_mul( tmp, tmp, PW1 );
			if( sozoku[line_idx].Sn_F6_NOFUKANPUsw&0x02){
				cmp = m_pArith->l_cmp( sozoku[line_idx].Sn_F6_NOFUCHIHO, tmp );
			}else{
				cmp = m_pArith->l_cmp( F610[line_idx], tmp );
			}
			if( cmp != 0 ){
				sozoku[line_idx].Sn_F6_NOFUKANPUsw |= 0x02;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 3 );
				memmove( sozoku[line_idx].Sn_F6_NOFUCHIHO, tmp, sizeof(sozoku[line_idx].Sn_F6_NOFUCHIHO) );
			}
			redisp = TRUE;

			break;

		case	ID5161:
		case	ID5162:
		case	ID5163:
		case	ID5164:
			memmove( sozoku[line_idx].Sn_F6_KANPUZEI, DiagData.data_val, sizeof(sozoku[line_idx].Sn_F6_KANPUZEI) );
			redisp = TRUE;
			break;

		case	ID5171:
		case	ID5172:
		case	ID5173:
		case	ID5174:
			memmove( sozoku[line_idx].Sn_F6_KANPUCHIHO, DiagData.data_val, sizeof(sozoku[line_idx].Sn_F6_KANPUCHIHO) );
			redisp = TRUE;
			break;

		case	ID5191_1:
		case	ID5192_1:
		case	ID5193_1:
		case	ID5194_1:
			sozoku[line_idx].Sn_F6_GNAME = DiagData.data_edit;
			break;
		case	ID5191_2:
		case	ID5192_2:
		case	ID5193_2:
		case	ID5194_2:
			sozoku[line_idx].Sn_F6_GSEL = DiagData.data_combo;
			break;
		case	ID5201_1:
		case	ID5202_1:
		case	ID5203_1:
		case	ID5204_1:
			sozoku[line_idx].Sn_F6_SNAME = DiagData.data_edit;
			break;
		case	ID5201_2:
		case	ID5202_2:
		case	ID5203_2:
		case	ID5204_2:
			sozoku[line_idx].Sn_F6_SSEL = DiagData.data_combo;
			break;
		case	ID5211:
		case	ID5212:
		case	ID5213:
		case	ID5214:
			sozoku[line_idx].Sn_F6_YOKIN = DiagData.data_edit;
			break;
		case	ID5221:
		case	ID5222:
		case	ID5223:
		case	ID5224:
			sozoku[line_idx].Sn_F6_KOUZA = DiagData.data_edit;
			break;
		case	ID5231_1:
		case	ID5232_1:
		case	ID5233_1:
		case	ID5234_1:
			sozoku[line_idx].Sn_F6_KIGO1 = DiagData.data_edit;
			break;
		case	ID5231_2:
		case	ID5232_2:
		case	ID5233_2:
		case	ID5234_2:
			sozoku[line_idx].Sn_F6_KIGO2 = DiagData.data_edit;
			break;
		case	ID5241:
		case	ID5242:
		case	ID5243:
		case	ID5244:
			sozoku[line_idx].Sn_F6_YUBIN = DiagData.data_edit;
			break;
		default:
			break;
	}

	if( line_idx >= 0 ){
		if( IsNULL_Column( line_idx ) == FALSE ){
			sozoku[line_idx].Sn_NO = line_idx;
		}
	}

	CString dummy;
	if( m_ctrl_mynum.GetData( dummy, 0 ) == -1 ){
		redisp = FALSE;
	}

	if( redisp ){
		DispFh6();
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//処理概要：index(diag)から行番号取得
//
//返送値　：行番号
//引数　　：sgn 0…相続人全体としての行番号　1…画面上の行番号
////////////////////////////////////////////////////////////////////////////////////
int CShinFhyo6::GetLineNumber_FromIndex( int index, int sgn )
{
	int ret = -1;

	int column = 0;
	for( column = 0; column < SOZOKU_PER_PAGE; column++ ){
		if( JUSYO_ID[column]		== index ) break;
		if( FURI_ID[column]			== index ) break;
		if( NAME_ID[column]			== index ) break;
		if( MYNUM1_ID[column]		== index ) break;
		if( MYNUM2_ID[column]		== index ) break;
		if( MYNUM3_ID[column]		== index ) break;
		if( SYOKU_ID[column]		== index ) break;
		if( TSUDUKI_ID[column]		== index ) break;
		if( BIRTH_ID[column]		== index ) break;
		if( TEL1_ID[column]			== index ) break;
		if( TEL2_ID[column]			== index ) break;
		if( TEL3_ID[column]			== index ) break;
		if( HOTEI_ID[column]		== index ) break;
		if( BUNSHI_ID[column]		== index ) break;
		if( BUNBO_ID[column]		== index ) break;
		if( KAGAKU_ID[column]		== index ) break;
		if( NOFUZEI_ID[column]		== index ) break;
		if( NOFUCHIHO_ID[column]	== index ) break;
		if( NOFUGOKEI_ID[column]	== index ) break;
		if( KANPUZEI_ID[column]		== index ) break;
		if( KANPUCHIHO_ID[column]	== index ) break;
		if( KANPUGOKEI_ID[column]	== index ) break;
		if( GNAME_ID[column]		== index ) break;
		if( GSEL_ID[column]			== index ) break;
		if( SNAME_ID[column]		== index ) break;
		if( SSEL_ID[column]			== index ) break;
		if( YOKIN_ID[column]		== index ) break;
		if( KOUZA_ID[column]		== index ) break;
		if( KIGO1_ID[column]		== index ) break;
		if( KIGO2_ID[column]		== index ) break;
		if( YUBIN_ID[column]		== index ) break;
	}								
									
	if( column < SOZOKU_PER_PAGE ){
		ret = column;
		if( sgn == 0 ){
			ret += m_Lin;
		}
	}

	return ret;
}

BOOL CShinFhyo6::IsLeftEnd(int index)
{
	BOOL ret = FALSE;

	switch( index ){
		case ID5011:
		case ID5021:
		case ID5031:
		case ID5041_1:
		case ID5051:
		case ID5071:
		case ID5081_1:
		case ID5091:
		case ID5121:
		case ID5131:
		case ID5141:
		case ID5161: //還付
		case ID5171:
		case ID5181:
		case ID5191_1:
		case ID5201_1:
		case ID5211:
		case ID5221:
		case ID5231_1:
		case ID5241:
			ret = TRUE;
			break;
	}

	return ret;
}

BOOL CShinFhyo6::IsRightEnd(int index)
{
	BOOL ret = FALSE;

	switch( index ){
		case ID5014:
		case ID5024:
		case ID5034:
		case ID5044_3:
		case ID5064:
		case ID5074:
		case ID5084_3:
		case ID5114:
		case ID5124:
		case ID5134:
		case ID5144:
		case ID5164:
		case ID5174:
		case ID5184:
		case ID5194_2:
		case ID5204_2:
		case ID5214:
		case ID5224:
		case ID5234_2:
		case ID5244:
			ret = TRUE;
			break;
	}

	return ret;
}

BOOL CShinFhyo6::IsComboBox(int index)
{
	BOOL ret = FALSE;

	switch( index ){
		case ID5091:
		case ID5092:
		case ID5093:
		case ID5094:
		case ID5191_2:
		case ID5192_2:
		case ID5193_2:
		case ID5194_2:
		case ID5201_2:
		case ID5202_2:
		case ID5203_2:
		case ID5204_2:
			ret = TRUE;
			break;
	}

	return ret;
}

BOOL CShinFhyo6::IsMyNumberEdit(int index)
{
	BOOL ret = FALSE;

	switch(index){
		case ID5041_1:
		case ID5041_2:
		case ID5041_3:
		case ID5042_1:
		case ID5042_2:
		case ID5042_3:
		case ID5043_1:
		case ID5043_2:
		case ID5043_3:
		case ID5044_1:
		case ID5044_2:
		case ID5044_3:
			ret = TRUE;
			break;
	}

	return ret;
}


// 行番号からindex(メモリ上の配列)取得
int CShinFhyo6::GetIndex_FromLine( int line_num )
{
	int ret = -1;

	for( int cnt = 0; cnt < SOZOKU_MAX; cnt++ ){
		if( sozoku[cnt].Sn_NO == line_num ){
			ret = cnt;
			break;
		}
	}

	return ret;
}

void CShinFhyo6::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CSyzBaseDlg::OnShowWindow(bShow, nStatus);

	// TODO: ここにメッセージ ハンドラ コードを追加します。

	m_ctrl_mynum.ShowWindow(SW_SHOW);
	m_ctrl_mynum.MoveDiagMNEdit(MYNUM1_1,FALSE);
}

void CShinFhyo6::OnSize(UINT nType, int cx, int cy)
{
	CSyzBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_Focus == 0 ){
		m_ctrl_mynum.MoveDiagMNEdit(MYNUM1_1,FALSE);
	}else{
		m_ctrl_mynum.MoveDiagMNEdit(m_Focus,FALSE);
	}

	//個人番号削除ボタンのフォント設定
	CClientDC dc(AfxGetMainWnd());
	int btn[4] = {IDC_MYNUM_DEL1,IDC_MYNUM_DEL2,IDC_MYNUM_DEL3,IDC_MYNUM_DEL4};

	for( int cnt = 0; cnt < SOZOKU_PER_PAGE; cnt++ ){
		CWnd *item;
		item = GetDlgItem(btn[cnt]);

		if( item == NULL ){
			continue;
		}

		// ポイントをフォントの高さに変換
		int Height = ::MulDiv(9, dc.GetDeviceCaps(LOGPIXELSY), 72);

		LOGFONT log_font;

		item->GetFont()->GetLogFont(&log_font);
		log_font.lfHeight = Height;

		m_font_mndel[cnt].DeleteObject();
		m_font_mndel[cnt].CreateFontIndirectA( &log_font );

		item->SetFont( &m_font_mndel[cnt], FALSE );
	}

	MoveWindow_SozokuBar();
}

void CShinFhyo6::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	OnVScrollCallBack( nSBCode, nPos, pScrollBar);
}

int CShinFhyo6::OnVScrollCallBack(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int focus = 0;
	if( m_Focus == 0 ){
		m_ctrl_mynum.MoveDiagMNEdit(MYNUM1_1,FALSE);
	}else{
		m_ctrl_mynum.MoveDiagMNEdit(m_Focus,FALSE);
	}

	MoveFocus_FromMyNum();

	m_bar.ShowWindow(SW_HIDE);

	CSyzBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);

	MoveWindow_SozokuBar();
	m_bar.ShowWindow(SW_SHOW);

	return 0;
}


void CShinFhyo6::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == &m_bar ) {	//横スクロールバー
		BOOL	DrawSw = TRUE;
		int		oldLeft = m_Lin;

		DelEdit();
		switch(nSBCode) {
		case SB_LINELEFT:		m_Lin--;					break;
		case SB_LINERIGHT:		m_Lin++;					break;
		case SB_PAGELEFT:		m_Lin -= SOZOKU_PER_PAGE;	break;
		case SB_PAGERIGHT:		m_Lin += SOZOKU_PER_PAGE;	break;
		case SB_LEFT:			m_Lin = 0;					break;
		case SB_RIGHT:			m_Lin = SOZOKU_MAX-1;		break;
		case SB_THUMBTRACK:		m_Lin = nPos;				break;	//Drag
		default:	//SB_ENDSCROLL
			DrawSw = FALSE;		//描画されないときに、セット
			break;
		}
		if( m_Lin > SOZOKU_MAX - SOZOKU_PER_PAGE )	m_Lin = SOZOKU_MAX-SOZOKU_PER_PAGE;
		if( SOZOKU_MAX <= SOZOKU_PER_PAGE )			m_Lin = oldLeft;
		if( m_Lin < 0 )								m_Lin = 0;
		if( oldLeft == m_Lin )						DrawSw = FALSE;

		if( DrawSw ) {
			m_bar.SetScrollPos(m_Lin);			//スクロールバーの移動
			DispFh6();							//表示更新
		}
		Edit_Set();

		MoveFocus_FromMyNum();
	}
	CSyzBaseDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}
void CShinFhyo6::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	OnTerminationCallBack(index, nChar, length, data);
}

int CShinFhyo6::TerminationMyNumber(short index, short nChar, short length, LPUNKNOWN data)
{

	if( IsMyNumberEdit(index) == TRUE ){
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
		CString str;
		switch(index){
			case ID5041_1:
			case ID5042_1:
			case ID5043_1:
			case ID5044_1:
				if(( wTerm == VK_TAB )||( wTerm == VK_UP )||( wTerm == VK_F2 )){
					pos = FocusMoveCheck(index , 1);
					m_Fhyo6Diag.SetPosition(pos);
					return 1;
				}
				else if(( wTerm == VK_DOWN )||( wTerm == VK_RIGHT )||( wTerm == VK_F3 )){
					pos = FocusMoveCheck(index , 2);
					m_Fhyo6Diag.SetPosition(pos);
					return 1;
				}
				else if( wTerm == VK_LEFT ){
					if( IsLeftEnd(index)&&(m_Lin!=0) ){
						if( m_ctrl_mynum.GetData(str, 0) ){
							m_Fhyo6Diag.SetPosition(index);
							return 1;
						}
						m_Lin--;
						m_bar.SetScrollPos(m_Lin);			//スクロールバーの移動
						DispFh6();

						int line = GetLineNumber_FromIndex(index);
						if( MyNumber_IsHashEmpty(sozoku[line].Sn_F6_HASH, sizeof(sozoku[line].Sn_F6_HASH)) ){
							pos = index;
						}else{
							//画面左端で、次の列が個人番号入力済みの時はフォーカスを上方へずらす
							pos = FocusMoveCheck(index , 1);
						}
					}else{
						pos = FocusMoveCheck(index , 3);
					}
					m_Fhyo6Diag.SetPosition(pos);

					return 1;
				}
				break;
			case ID5041_2:
			case ID5042_2:
			case ID5043_2:
			case ID5044_2:
				if(( wTerm == VK_UP )||( wTerm == VK_F2 )){
					pos = FocusMoveCheck(index , 1);
					m_Fhyo6Diag.SetPosition(pos);
					return 1;
				}
				else if(( wTerm == VK_DOWN )||( wTerm == VK_RIGHT )||( wTerm == VK_F3 )){
					pos = FocusMoveCheck(index , 2);
					m_Fhyo6Diag.SetPosition(pos);
					return 1;
				}
				break;
			case ID5041_3:
			case ID5042_3:
			case ID5043_3:
			case ID5044_3:
				if( wTerm == VK_UP ){
					pos = FocusMoveCheck(index , 1);
					m_Fhyo6Diag.SetPosition(pos);
					return 1;
				}
				else if(( wTerm == VK_RETURN )||( wTerm == VK_DOWN )||( wTerm == VK_F3 )){
					CString str;
					if( m_ctrl_mynum.GetData(str,1) == 0 ){ //Enterで抜ける時だけメッセージが2回出る(原因不明)
						pos = FocusMoveCheck(index , 0);
						m_Fhyo6Diag.SetPosition(pos);
					}
					return 1;
				}
				else if( wTerm == VK_RIGHT ){
					if( IsRightEnd(index)&&(m_Lin+SOZOKU_PER_PAGE<=SOZOKU_MAX-1) ){
						if( m_ctrl_mynum.GetData(str, 0) ){
							m_Fhyo6Diag.SetPosition(index);
							return 1;
						}

						//右端からさらに右へ移動すると入力した個人番号が消える(原因不明)
						int line = GetLineNumber_FromIndex(index);
						sozoku[line].Sn_F6_MYNUM1.Format("%s", str.Left(4) );
						sozoku[line].Sn_F6_MYNUM2.Format("%s", str.Mid(4, 4) );
						sozoku[line].Sn_F6_MYNUM3.Format("%s", str.Right(4) );
						if( str.IsEmpty() == FALSE ){
							sozoku[line].Sn_NO = line;
						}

						m_Lin++;
						m_bar.SetScrollPos(m_Lin);			//スクロールバーの移動
						DispFh6(1);

						line = GetLineNumber_FromIndex(index);
						m_ctrl_mynum.SetData( sozoku[line].Sn_F6_MYNUM1+sozoku[line].Sn_F6_MYNUM2+sozoku[line].Sn_F6_MYNUM3);
						if( MyNumber_IsHashEmpty(sozoku[line].Sn_F6_HASH, sizeof(sozoku[line].Sn_F6_HASH)) ){
							//pos = index;
							pos = MYNUM1_ID[SOZOKU_PER_PAGE-1];
						}else{
							//画面右端で、次の列が個人番号入力済みの時はフォーカスを上方へずらす
							pos = FocusMoveCheck(index , 1);
						}
					}else{
						pos = FocusMoveCheck(index , 2);
					}
					m_Fhyo6Diag.SetPosition(pos);

					return 1;
				}
				break;
			default:
				break;
		}
		
		if( wTerm == VK_END ){
			CString str;
			//マイナンバーコントロール上でENDを押されると、予想外の動作をするため
			if( m_ctrl_mynum.GetData(str,0) == 0 ){
				if((index == ID5041_1)||(index == ID5041_2)||(index == ID5041_3)){
					pos = FocusMoveCheck(ID5041_3 , 0);
				}else
				if((index == ID5042_1)||(index == ID5042_2)||(index == ID5042_3)){
					pos = FocusMoveCheck(ID5042_3 , 0);
				}else
				if((index == ID5043_1)||(index == ID5043_2)||(index == ID5043_3)){
					pos = FocusMoveCheck(ID5043_3 , 0);
				}else
				if((index == ID5044_1)||(index == ID5044_2)||(index == ID5044_3)){
					pos = FocusMoveCheck(ID5044_3 , 0);
				}
				m_Fhyo6Diag.SetPosition(pos);
			}
		}
		return 0;
	}

	return -1;
}

void CShinFhyo6::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	CString str;

	int column_old = GetLineNumber_FromIndex(m_Focus, 1);
	BOOL isMynumEdit_old = IsMyNumberEdit(m_Focus);

	m_Focus = index;
	if( IsMyNumberEdit(index) ){
		//同じ行のマイナンバーエディット間の移動は処理しない
		mynum_cursel = 1;
		m_ctrl_mynum.ShowWindow(SW_SHOW);

		if(( GetLineNumber_FromIndex(index,1) != column_old )||(!isMynumEdit_old)){
			int line = GetLineNumber_FromIndex(index);
			if( m_ctrl_mynum.GetData(str,0) == 0 ){
				if(line>-1){
					//移動先に移動元の番号を移動してしまうため(原因突き止められず・・・)
					m_ctrl_mynum.SetData( sozoku[line].Sn_F6_MYNUM1+sozoku[line].Sn_F6_MYNUM2+sozoku[line].Sn_F6_MYNUM3 );
				}
			}
		}
		m_ctrl_mynum.MoveDiagMNEdit(index, TRUE);
	}else{
		//mynum_cursel = 0;
		OnEditONCallBack(index);

		switch ( index ) {
		case ID6010:
			if ( !Sn_F6_USE )  {
				Sn_F6_USE = 1;
				m_Fhyo6Diag.DeleteInput();
				dsp_check ( ID6010, 1 );
				dsp_check ( ID6011, 0 );
				m_Fhyo6Diag.SetPosition ( ID6010 );
				ModifyDiagInput();
			}
			break;
		case ID6011:
			if ( Sn_F6_USE ) {
				Sn_F6_USE = 0;
				m_Fhyo6Diag.DeleteInput();
				dsp_check ( ID6010, 0 );
				dsp_check ( ID6011, 1 );
				m_Fhyo6Diag.SetPosition ( ID6011 );
				ModifyDiagInput();
			}
			break;
		default:
			break;
		}
	}

	//MyNumber_ButtomDisp();
}

// 終了処理
void CShinFhyo6::EndJob()
{
	NTBMOD8_EndProc();
}

void CShinFhyo6::ComboSelIcsdiagctrl1(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo6::Init_sozoku( int index )
{
//	sozoku[index].Sn_SEQ = -1;
	sozoku[index].Sn_NO = -1;
	//sozoku[index].Sn_NO = index;
	sozoku[index].Sn_F6_JUSYO.Empty();
	sozoku[index].Sn_F6_FURI.Empty();
	sozoku[index].Sn_F6_NAME.Empty();
	//sozoku[index].Sn_F6_HASH.Empty();
	memset( sozoku[index].Sn_F6_HASH, '\0', sizeof( sozoku[index].Sn_F6_HASH ));
	sozoku[index].Sn_F6_MYNUM1.Empty();
	sozoku[index].Sn_F6_MYNUM2.Empty();
	sozoku[index].Sn_F6_MYNUM3.Empty();
	sozoku[index].Sn_F6_SYOKU.Empty();
	sozoku[index].Sn_F6_TSUDUKI.Empty();
	memset( sozoku[index].Sn_F6_BIRTH, '\0', sizeof( sozoku[index].Sn_F6_BIRTH ));
	sozoku[index].Sn_F6_TEL1.Empty();
	sozoku[index].Sn_F6_TEL2.Empty();
	sozoku[index].Sn_F6_TEL3.Empty();
	sozoku[index].Sn_F6_HOTEI = 0;
	sozoku[index].Sn_F6_BUNSHI.Empty();
	sozoku[index].Sn_F6_BUNBO.Empty();
	//m_pArith->l_clear( sozoku[index].Sn_F6_KAGAKU );
	memset( sozoku[index].Sn_F6_KAGAKU, '\0', sizeof( sozoku[index].Sn_F6_KAGAKU ));
	memset( sozoku[index].Sn_F6_NOFUZEI, '\0', sizeof( sozoku[index].Sn_F6_NOFUZEI ));
	memset( sozoku[index].Sn_F6_NOFUCHIHO, '\0', sizeof( sozoku[index].Sn_F6_NOFUCHIHO ));
	memset( sozoku[index].Sn_F6_KANPUZEI, '\0', sizeof( sozoku[index].Sn_F6_KANPUZEI ));
	memset( sozoku[index].Sn_F6_KANPUCHIHO, '\0', sizeof( sozoku[index].Sn_F6_KANPUCHIHO ));
	sozoku[index].Sn_F6_NOFUKANPUsw = 0;
	sozoku[index].Sn_F6_GNAME.Empty();
	sozoku[index].Sn_F6_GSEL = 0;
	sozoku[index].Sn_F6_SNAME.Empty();
	sozoku[index].Sn_F6_SSEL = 0;
	sozoku[index].Sn_F6_YOKIN.Empty();
	sozoku[index].Sn_F6_KOUZA.Empty();
	sozoku[index].Sn_F6_KIGO1.Empty();
	sozoku[index].Sn_F6_KIGO2.Empty();
	sozoku[index].Sn_F6_YUBIN.Empty();
	
	return;
}

// マイナンバー更新(廃棄・削除・追加)を一括で
int CShinFhyo6::MyNumber_Update()
{
	//「削除」を選択された個人番号(履歴出力のみ)
	MyNumber_DeleteHistory();

	//「廃棄」を選択された個人番号
	MyNumber_Haiki();

	//追加されたマイナンバー
	MyNumber_Write();

	//削除・廃棄を選択されたマイナンバーの記録領域のクリア
	MyNumber_DelHaiki_Clear();

	return 0;
}

// 削除履歴書き込み
int CShinFhyo6::MyNumber_DeleteHistory()
{
	if( m_MyNum_Delete_idx == 0 ){
		return 0;
	}

	NTBMOD8_BeginProc( 0, NULL );

	CNoHistoryTblArray nhtblary;

//	if( m_MyNum_Delete_idx == 0 ){
//		return 0;
//	}

	for( int idx = 0; idx < m_MyNum_Delete_idx; idx++ ){
		NODATATBL ndtbl = {0};
		memmove( ndtbl.arrow, m_MyNum_Delete[idx], sizeof(ndtbl.arrow));

		int ret = NTBMOD8_GetNoDataTbl( 0, &ndtbl );
		if(( ret == 0 )||( ret == 1 )){
			//ハッシュ値のみ(個人番号なし) or 個人番号あり
			CString mynum;
			BOOL ReEnter = FALSE;
			mynum.Format( ndtbl.no );
			if( ret == 1 ){
				for( int idx2 = 0; idx2 < SOZOKU_MAX; idx2++ ){
					if( mynum.Compare(sozoku[idx2].Sn_F6_MYNUM1+sozoku[idx2].Sn_F6_MYNUM2+sozoku[idx2].Sn_F6_MYNUM3) == 0 ){
						//削除後再度入力→履歴は残さない
						ReEnter = TRUE;
						break;
					}
				}
			}
			if( ReEnter == FALSE ){ //削除
				NOHISTTBL	nhtbl;
				NTBMOD8_InitNoHistoryTbl( 0, &nhtbl );
				wsprintf( nhtbl.tgt_char, "%s", m_MyNum_Delete_Shimei.GetAt(idx) );
				nhtbl.user_play = NTBMOD_USER_PLAY_APPLI|NTBMOD_USER_PLAY_DELETE;

				nhtbl.mst_code = m_pZmSub->zvol->v_cod;
				nhtbl.mst_apno = m_pZmSub->zvol->apno;


//2018.08.21 UPDATE START
				//int len =  m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO) );
				//if( len ){
				//	memmove( nhtbl.mst_nam, (*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO) );
				//}
				//else{
				//	memmove( nhtbl.mst_nam, m_pZmSub->zvol->c_nam, min(sizeof(nhtbl.mst_nam), m_pZmSub->zvol->c_nam.GetLength()) );
				//}

				memmove( nhtbl.mst_nam, m_pZmSub->zvol->c_nam, min(sizeof(nhtbl.mst_nam), m_pZmSub->zvol->c_nam.GetLength()) );
//2018.08.21 UPDATE END

//2018.08.21 INSERT START
				CString mst_nam_str,kessan_str;
				
				mst_nam_str = nhtbl.mst_nam;
				mst_nam_str = mst_nam_str.TrimRight();
				(*m_pSnHeadData)->GetString_KessanKikan( &kessan_str );
				strcpy_s( nhtbl.mst_nam, sizeof(nhtbl.mst_nam), mst_nam_str + kessan_str );
//2018.08.21 INSERT END

				strcpy_s( nhtbl.gy_nam, sizeof(nhtbl.gy_nam), _T("消費税申告書") );

				nhtblary.Add( nhtbl );
			}			
		}else if( ret == -1 ){
			//エラー
			continue;
		}
	}

	// 履歴テーブルにセット
	NTBMOD8_SetNoHistoryTbl( 0, &nhtblary );
	NTBMOD8_EndProc();

	return 0;
}
// マイナンバー廃棄
int CShinFhyo6::MyNumber_Haiki()
{

	if( m_MyNum_Haiki_idx == 0 ){
		return 0;
	}

	NTBMOD8_BeginProc( 0, NULL );

	//CNoHistoryTblArray nhtblary;

//	if( m_MyNum_Haiki_idx == 0 ){
//		return 0;
//	}
	
	for( int idx = 0; idx < m_MyNum_Haiki_idx; idx++ ){
		NODATATBL ndtbl = {0};
		memmove( ndtbl.arrow, m_MyNum_Haiki[idx], sizeof(ndtbl.arrow));

		int ret = NTBMOD8_GetNoDataTbl( 0, &ndtbl );
		if(( ret == 0 )||( ret == 1 )){
			//ハッシュ値のみ(個人番号なし) or 個人番号あり
			CString mynum;
			BOOL ReEnter = FALSE;
			mynum.Format( ndtbl.no );
			if( ret == 1 ){
				for( int idx2 = 0; idx2 < SOZOKU_MAX; idx2++ ){
					if( mynum.Compare(sozoku[idx2].Sn_F6_MYNUM1+sozoku[idx2].Sn_F6_MYNUM2+sozoku[idx2].Sn_F6_MYNUM3) == 0 ){
						//削除後再度入力→履歴は残さない
						ReEnter = TRUE;
						break;
					}
				}
			}
			if( ReEnter == FALSE ){ //廃棄

				NOHISTTBL	nhtbl;
				NTBMOD8_InitNoHistoryTbl( 0, &nhtbl );
				wsprintf( nhtbl.tgt_char, "%s", m_MyNum_Haiki_Shimei.GetAt(idx) );
				nhtbl.user_play = NTBMOD_USER_PLAY_APPLI|NTBMOD_USER_PLAY_DISCARD;

				nhtbl.mst_code = m_pZmSub->zvol->v_cod;
				nhtbl.mst_apno = m_pZmSub->zvol->apno;

//2018.08.21 UPDATE START
				//int len =  m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO) );
				//if( len ){
				//	memmove( nhtbl.mst_nam, (*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO) );
				//}
				//else{
				//	memmove( nhtbl.mst_nam, m_pZmSub->zvol->c_nam, min(sizeof(nhtbl.mst_nam), m_pZmSub->zvol->c_nam.GetLength()) );
				//}

				memmove( nhtbl.mst_nam, m_pZmSub->zvol->c_nam, min(sizeof(nhtbl.mst_nam), m_pZmSub->zvol->c_nam.GetLength()) );
//2018.08.21 UPDATE END

//2018.08.21 INSERT START
				CString mst_nam_str,kessan_str;
				
				mst_nam_str = nhtbl.mst_nam;
				mst_nam_str = mst_nam_str.TrimRight();
				(*m_pSnHeadData)->GetString_KessanKikan( &kessan_str );
				strcpy_s( nhtbl.mst_nam, sizeof(nhtbl.mst_nam), mst_nam_str + kessan_str );
//2018.08.21 INSERT END

				strcpy_s( nhtbl.gy_nam, sizeof(nhtbl.gy_nam), _T("消費税申告書") );

				NTBMOD8_DeleteNoDataTbl( 0, &ndtbl, &nhtbl );

				//nhtblary.Add( nhtbl );
			}
		}else if( ret == -1 ){
			//エラー
			continue;
		}
	}

	// 履歴テーブルにセット
	//NTBMOD8_SetNoHistoryTbl( 0, &nhtblary );
	NTBMOD8_EndProc();

	return 0;
}
// マイナンバー更新
int CShinFhyo6::MyNumber_Write()
{

	for( int idx = 0; idx < SOZOKU_MAX; idx++ ){
		CString my_num;
		my_num = sozoku[idx].Sn_F6_MYNUM1+sozoku[idx].Sn_F6_MYNUM2+sozoku[idx].Sn_F6_MYNUM3;
		if( m_ctrl_mynum.CheckNumber( my_num, MN_KOJIN ) != 1 ){
			continue;
		}

		NODATATBL ndtbl = {0};
		wsprintf( ndtbl.no, "%s", my_num );


		NOHISTTBL	nhtbl;
		NTBMOD8_InitNoHistoryTbl( 0, &nhtbl );
		wsprintf( nhtbl.tgt_char, "%s", sozoku[idx].Sn_F6_NAME );

		nhtbl.mst_code = m_pZmSub->zvol->v_cod;
		nhtbl.mst_apno = m_pZmSub->zvol->apno;

//2018.08.21 UPDATE START
		//int len =  m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO) );
		//if( len ){
		//	memmove( nhtbl.mst_nam, (*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO) );
		//}
		//else{
		//	memmove( nhtbl.mst_nam, m_pZmSub->zvol->c_nam, min(sizeof(nhtbl.mst_nam), m_pZmSub->zvol->c_nam.GetLength()) );
		//}

		memmove( nhtbl.mst_nam, m_pZmSub->zvol->c_nam, min(sizeof(nhtbl.mst_nam), m_pZmSub->zvol->c_nam.GetLength()) );
		//memmove( nhtbl.tgt_char, (*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO) );
//2018.08.21 UPDATE END

//2018.08.21 INSERT START
		CString mst_nam_str,kessan_str;
		
		mst_nam_str = nhtbl.mst_nam;
		mst_nam_str = mst_nam_str.TrimRight();
		(*m_pSnHeadData)->GetString_KessanKikan( &kessan_str );
		strcpy_s( nhtbl.mst_nam, sizeof(nhtbl.mst_nam), mst_nam_str + kessan_str );
//2018.08.21 INSERT END

		strcpy_s( nhtbl.gy_nam, sizeof(nhtbl.gy_nam), _T("消費税申告書") );


		int ret = NTBMOD8_GetNoDataTbl( 1, &ndtbl );
		if( ret == 0 ){ //該当の個人番号あり
			memmove( sozoku[idx].Sn_F6_HASH, ndtbl.arrow, sizeof(sozoku[idx].Sn_F6_HASH) );

			if( strlen( ndtbl.nam2 ) == 0 ){
				nhtbl.user_play = NTBMOD_USER_PLAY_UPDATE|NTBMOD_USER_PLAY_APPLI;
				wsprintf( ndtbl.nam2, "%s", sozoku[idx].Sn_F6_NAME );
				if( NTBMOD8_SetNoDataTbl( 1, &ndtbl, &nhtbl ) == -1 ){ //情報部更新
					return -1;
				}
			}

			continue;
		}
		if( ret == -1 ){ 
			continue; //エラー
		}


		NTBMOD8_InitNoDataTbl( 0, &ndtbl );

		wsprintf( ndtbl.no, "%s", my_num );						//個人番号
		ndtbl.gyousyu_flg = NTBMOD_GYFLG_SYOHI;					//業種フラグ
		wsprintf( ndtbl.nam2, "%s", sozoku[idx].Sn_F6_NAME );	//氏名
		
		char	sqlsvr[128]={0}, apno[32]={0}, cocode[32]={0}, kikan[32]={0};
		PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan );
		ndtbl.cocode = atoi(cocode);


		nhtbl.user_play = NTBMOD_USER_PLAY_APPLI|NTBMOD_USER_PLAY_INSERT;
		if( NTBMOD8_SetNoDataTbl( 0, &ndtbl, &nhtbl ) == -1 ){
			return -1;
		}

		if( NTBMOD8_GetNoDataTbl( 1, &ndtbl ) == -1 ){
			return -1;
		}

		memmove( sozoku[idx].Sn_F6_HASH, ndtbl.arrow, sizeof(sozoku[idx].Sn_F6_HASH) );
	}

	return 0;
}

void CShinFhyo6::MyNumber_DelHaiki_Clear()
{
	for(int cnt = 0; cnt < SOZOKU_MAX; cnt++){
		memset( m_MyNum_Haiki[cnt], '\0', sizeof(m_MyNum_Haiki[cnt]));
		memset( m_MyNum_Delete[cnt], '\0', sizeof(m_MyNum_Delete[cnt]));
	}
	m_MyNum_Haiki_Shimei.RemoveAll();
	m_MyNum_Delete_Shimei.RemoveAll();

	m_MyNum_Haiki_idx = 0;
	m_MyNum_Delete_idx = 0;
}

void CShinFhyo6::MyNumber_Change()
{

	NODATATBL ndtbl = {0};

	for( int idx = 0; idx < SOZOKU_MAX; idx++ ){

		NTBMOD8_InitNoDataTbl( 0, &ndtbl );

		memmove( ndtbl.arrow, sozoku[idx].Sn_F6_HASH, sizeof( ndtbl.arrow ));

		if( NTBMOD8_GetNoDataTbl( 0, &ndtbl ) == 0 ){

			CString mynum_tmp;
			mynum_tmp.Format(ndtbl.no);

			if( MynoAccessPerm() == TRUE ){
				sozoku[idx].Sn_F6_MYNUM1.Format("%s", mynum_tmp.Left(4) );
				sozoku[idx].Sn_F6_MYNUM2.Format("%s", mynum_tmp.Mid(4, 4) );
				sozoku[idx].Sn_F6_MYNUM3.Format("%s", mynum_tmp.Right(4) );
			}else{
				sozoku[idx].Sn_F6_MYNUM1.Format("****");
				sozoku[idx].Sn_F6_MYNUM2.Format("****");
				sozoku[idx].Sn_F6_MYNUM3.Format("****");
			}
		}
	}
}
void CShinFhyo6::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CSyzBaseDlg::OnActivate(nState, pWndOther, bMinimized);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinFhyo6::DelEdit()
{
	if( m_Focus >= -1 ){
		OnEditOFFCallBack(m_Focus);
	}
	diag_deledit( IDC_ICSDIAGCTRL1, CIcsdiagctrl );
}

void CShinFhyo6::Edit_Set()
{
	diag_setposition( IDC_ICSDIAGCTRL1, m_Focus, CIcsdiagctrl );
}

////////////////////////////////////////////////////////////////
//処理概要：個人番号削除
//
//引数　　：line…画面上の列番号
//返送値　：-1…エラー　0…削除・廃棄　1…キャンセル
////////////////////////////////////////////////////////////////
int CShinFhyo6::MynumDel( int line )
{
	// 上手くん環境なら不可
	if( GET_EntrySign() != 0 ){
		return -1;
	}
	//// 個人番号アクセス権限がなかったら不可
	//if( MynoAccessPerm() == FALSE ){
	//	return -1;
	//}
	// 入力ロック時は不可
	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
		return -1;
	}
	// 個人番号項目の処理が可能か
	//if( NTBMOD8_IsAbleToMyNumber( (*m_pSnHeadData)->m_apno, 0 ) != 1 ){
	//	return -1;
	//}

	if( MyNumber_IsHashEmpty( sozoku[m_Lin+line].Sn_F6_HASH, sizeof(sozoku[m_Lin+line].Sn_F6_HASH))){
		return -1;
	}

	NODATATBL ndtbl = {0};
	memmove( ndtbl.arrow, sozoku[m_Lin+line].Sn_F6_HASH, sizeof(ndtbl.arrow));
	
	int flg = MEG_KIND_ONE_DEL;
	if( NTBMOD8_GetNoDataTbl( 0, &ndtbl ) == 1 ){
		//該当データなし
		flg |= MEG_KIND_NODB_KBN;
	}

	CICSMNSub8 mnsub;

	int sgn = mnsub.NoMessageBox_Dsp( this, flg );

	// 個人番号項目の処理が不可能(ライセンスなし)でも個人番号を削除する(列削除のため)
	if( NTBMOD8_IsAbleToMyNumber( (*m_pSnHeadData)->m_apno, 0 ) != 1 ){
		if( sgn == RET_NO_CANCEL ){ //キャンセル
			sgn = RET_NODEL_H;
		}
	}

	if( sgn == RET_NO_CANCEL ){ //キャンセル
		return 1;
	}else
	if( sgn == RET_NODEL_H ){ //削除　ハッシュのみ削除
		memmove( m_MyNum_Delete[m_MyNum_Delete_idx], sozoku[m_Lin+line].Sn_F6_HASH, sizeof(m_MyNum_Delete[m_MyNum_Haiki_idx]) );
		m_MyNum_Delete_idx++;
		m_MyNum_Delete_Shimei.Add( sozoku[m_Lin+line].Sn_F6_NAME );
	}else
	if( sgn == RET_NODEL_DB ){ //廃棄　個人番号削除
		memmove( m_MyNum_Haiki[m_MyNum_Haiki_idx], sozoku[m_Lin+line].Sn_F6_HASH, sizeof(m_MyNum_Delete[m_MyNum_Haiki_idx]) );
		m_MyNum_Haiki_idx++;
		m_MyNum_Haiki_Shimei.Add( sozoku[m_Lin+line].Sn_F6_NAME );
	}
	memset( sozoku[m_Lin+line].Sn_F6_HASH, '\0', sizeof(sozoku[m_Lin+line].Sn_F6_HASH));
	sozoku[m_Lin+line].Sn_F6_MYNUM1.Empty();
	sozoku[m_Lin+line].Sn_F6_MYNUM2.Empty();
	sozoku[m_Lin+line].Sn_F6_MYNUM3.Empty();

	DispFh6();

	return 0;

}
void CShinFhyo6::OnBnClickedMynumDel1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	MynumDel(0);
}
void CShinFhyo6::OnBnClickedMynumDel2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	MynumDel(1);
}

void CShinFhyo6::OnBnClickedMynumDel3()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	MynumDel(2);
}

void CShinFhyo6::OnBnClickedMynumDel4()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	MynumDel(3);
}

HBRUSH CShinFhyo6::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSyzBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	//switch(nCtlColor){
	//	case CTLCOLOR_EDIT:
	//		if( mynum_cursel == 0 ){
	//			hbr = mnedit_brush;
	//			pDC->SetBkColor(BC_CREAM);
	//		}

	//		break;
	//}
	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinFhyo6::DIAGRAM_DATA_clear( DIAGRAM_DATA *DiagData )
{
	DiagData->data_disp.Empty();
	DiagData->data_edit.Empty();
	DiagData->data_combo = 0;
	DiagData->data_check = 0;
	memset( DiagData->data_val, '\0', sizeof( DiagData->data_val ));
	memset( DiagData->data_day, '\0', sizeof( DiagData->data_day ));
	DiagData->data_imgdata = NULL;
	DiagData->data_imgwidth = 0;
	DiagData->data_imgheight = 0;

	DiagData->text_combo.Empty();
	DiagData->text_check.Empty();
	DiagData->movepos  = 0;
}

void CShinFhyo6::OnBnClickedSozokuDel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int index = GetLineNumber_FromIndex( m_Focus );
	if( index == -1 ){
		return;
	}
	CString msg;
	msg.Format("番号[%d]の相続人情報を削除しますか？",index+1);
	if( ICSMessageBox(msg, MB_YESNO|MB_DEFBUTTON2, 0)!=IDYES ){
		return;
	}

	BOOL empty = MyNumber_IsHashEmpty( sozoku[index].Sn_F6_HASH, sizeof(sozoku[index].Sn_F6_HASH) );

	if( empty == FALSE ){
		int index2 = GetLineNumber_FromIndex( m_Focus, 1 );
		if( MynumDel(index2) != 0 ){
			return;
		}
	}

	for( int idx = index; idx < SOZOKU_MAX-1; idx++ ){
		sozoku[idx] = sozoku[idx+1];
		memmove( F69[idx], F69[idx+1], MONY_BUF_SIZE );
		memmove( F610[idx], F610[idx+1], MONY_BUF_SIZE );
		memmove( F611[idx], F611[idx+1], MONY_BUF_SIZE );
		if( sozoku[idx].Sn_NO != -1 ){
			sozoku[idx].Sn_NO--;
		}
	}

	Init_sozoku(SOZOKU_MAX-1);
	m_pArith->l_clear(F69[SOZOKU_MAX-1]);
	m_pArith->l_clear(F610[SOZOKU_MAX-1]);
	m_pArith->l_clear(F611[SOZOKU_MAX-1]);

	UpdateShinPayment( 1, index, 1 );

	DispFh6();

	m_Fhyo6Diag.SetPosition(m_Focus);

}

void CShinFhyo6::OnBnClickedSozokuIns()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int index = GetLineNumber_FromIndex( m_Focus );
	if( index == -1 ){
		return;
	}

	if( IsNULL_Column( SOZOKU_MAX-1 ) == FALSE ){
		m_Fhyo6Diag.SetPosition(m_Focus);
		return;
	}

	for( int idx = SOZOKU_MAX-1; idx > index; idx-- ){
		sozoku[idx] = sozoku[idx-1];
		memmove( F69[idx], F69[idx-1], MONY_BUF_SIZE );
		memmove( F610[idx], F610[idx-1], MONY_BUF_SIZE );
		memmove( F611[idx], F611[idx-1], MONY_BUF_SIZE );
		if( sozoku[idx].Sn_NO != -1 ){
			sozoku[idx].Sn_NO++;
		}
	}

	Init_sozoku(index);

	m_pArith->l_clear(F69[index]);
	m_pArith->l_clear(F610[index]);
	m_pArith->l_clear(F611[index]);

	UpdateShinPayment( 0, index );

	DispFh6();

	m_Fhyo6Diag.SetPosition(m_Focus);

}

//====================================================
// 入力許可・不許可切り替え
//====================================================
void CShinFhyo6::ModifyDiagInput()
{
	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
		return;
	}

	if ( Sn_F6_USE ) {
		int len = 0;
		char buf[128] = { 0 };
		CString str;

		memset ( buf, '\0', sizeof ( buf ) );
		len = m_Util.MixSpaceCutLength ( ( char* )Sn_F6_ADR, 40 );
		memmove ( buf, Sn_F6_ADR, len );
		str = buf;
		dsp_edit ( ID6020, str );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6020, DIAG_MDFY_EDIT, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6020, 0 );

		memset ( buf, '\0', sizeof ( buf ) );
		len = m_Util.MixSpaceCutLength ( ( char* )&Sn_F6_ADR[40], 40 );
		memmove ( buf, &Sn_F6_ADR[40], len );
		str = buf;
		dsp_edit ( ID6021, str );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6021, DIAG_MDFY_EDIT, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6021, 0 );

		dsp_edit ( ID6030, Sn_F6_TELNO1 );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6030, DIAG_MDFY_EDIT, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6030, 0 );

		dsp_edit ( ID6031, Sn_F6_TELNO2 );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6031, DIAG_MDFY_EDIT, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6031, 0 );

		dsp_edit ( ID6032, Sn_F6_TELNO3 );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6032, DIAG_MDFY_EDIT, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6032, 0 );

		dsp_edit ( ID6040, Sn_F6_NMFRI );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6040, DIAG_MDFY_EDIT, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6040, 0 );

		dsp_edit ( ID6041, Sn_F6_NAME );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6041, DIAG_MDFY_EDIT, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6041, 0 );


		memset ( buf, '\0', sizeof ( buf ) );
		len = m_Util.MixSpaceCutLength ( ( char* )Sn_F6_SONOTA, 40 );
		memmove ( buf, Sn_F6_SONOTA, len );
		str = buf;
		dsp_edit ( ID6050, str );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6050, DIAG_MDFY_EDIT, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6050, 0 );

		memset ( buf, '\0', sizeof ( buf ) );
		len = m_Util.MixSpaceCutLength ( ( char* )&Sn_F6_SONOTA[40], 40 );
		memmove ( buf, &Sn_F6_SONOTA[40], len );
		str = buf;
		dsp_edit ( ID6051, str );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6051, DIAG_MDFY_EDIT, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6051, 0 );
	}
	else {
		dsp_cls ( IDC_ICSDIAGCTRL1, ID6020 );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6020, DIAG_MDFY_READONLY, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6020, 1 );

		dsp_cls ( IDC_ICSDIAGCTRL1, ID6021 );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6021, DIAG_MDFY_READONLY, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6021, 1 );
	
		dsp_cls ( IDC_ICSDIAGCTRL1, ID6030 );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6030, DIAG_MDFY_READONLY, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6030, 1 );
		
		dsp_cls ( IDC_ICSDIAGCTRL1, ID6031 );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6031, DIAG_MDFY_READONLY, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6031, 1 );
		
		dsp_cls ( IDC_ICSDIAGCTRL1, ID6032 );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6032, DIAG_MDFY_READONLY, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6032, 1 );
		
		dsp_cls ( IDC_ICSDIAGCTRL1, ID6040 );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6040, DIAG_MDFY_READONLY, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6040, 1 );
		
		dsp_cls ( IDC_ICSDIAGCTRL1, ID6041 );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6041, DIAG_MDFY_READONLY, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6041, 1 );
		
		dsp_cls ( IDC_ICSDIAGCTRL1, ID6050 );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6050, DIAG_MDFY_READONLY, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6050, 1 );

		dsp_cls ( IDC_ICSDIAGCTRL1, ID6051 );
		diag_modify ( IDC_ICSDIAGCTRL1, ID6051, DIAG_MDFY_READONLY, CIcsdiagctrl );
		ChangeColor ( IDC_ICSDIAGCTRL1, ID6051, 1 );
	}

	//m_Fhyo6Diag.Refresh();
}

//==========================================
// 手入力項目あり？
//------------------------------------------
// 返送値
//		TRUE		:		あり
//		FALSE		:		なし
//==========================================
BOOL CShinFhyo6::IsManualInputSign()
{
	BOOL ret = FALSE;
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		// 連動時、入力ロック中はチェックしない
		return ret;
	}
	else {
		// 納付の場合のみチェック
		if ( m_pArith->l_test ( F61 ) != 0 ) {
			for ( int idx = 0; idx  < SOZOKU_MAX; idx ++ ) {
				if ( sozoku[idx].Sn_F6_NOFUKANPUsw & 0x03 ) {
					ret = TRUE;
					break;
				}	
			}
		}
	}

	return ret;
}

//----> 納付書・相続人対応
//==========================================
// 相続人情報の取得
//------------------------------------------
// 引数
//		nohfuinfo	:	納付書情報(配列40で来る)
//		nohfudata	:	納付書データ(配列40で来る)
//		SozokuNoAry	:	相続人番号(Sn_NO)
//------------------------------------------
// 返送値
//		data_cnt	:	データ数(相続人数)
//==========================================
int CShinFhyo6::GetSozokuInfo( NOHFUSHO_INFO *nohfuinfo, NOHFUSHO_DATA *nohfudata, CArray<int> *SozokuNoAry )
{
	int data_cnt = 0;


	// 基本情報タブから開いたときのため読み込んでおく
	ReadData();

	// 入力データチェック
	if( ((CH31HyoView*)m_pParent)->Message_MyNumber_error() == -1 ){
		return -1;
	}
	if( ((CH31HyoView*)m_pParent)->Message_MyNumber_overlap() == -1 ){
		return -1;
	}
	if( ((CH31HyoView*)m_pParent)->Message_SozokuWariai() == -1 ){
		return -1;
	}
	if( ((CH31HyoView*)m_pParent)->Message_NofuKanpuGokei() == IDYES ){ // 集計込み
		return -1;
	}

	// IsNull_Columnチェック用フラグ
	char sgn = 0x00;
	if( m_pArith->l_test(XM26) > 0 ){
		sgn = 0x02;
	}else
	if( m_pArith->l_test(XM26) < 0 ){
		sgn = 0x01;
	}else{
		sgn = 0x03;
	}


	for( int cnt = 0; cnt < SOZOKU_MAX; cnt++ ){

		if( sozoku[cnt].Sn_NO == -1 ){
			continue;
		}

		if( IsNULL_Column( cnt, sgn ) == TRUE ){
			continue;
		}
		else{
			// データが何か入力されていたらカウント
			SozokuNoAry->Add( sozoku[cnt].Sn_NO );
		}

		/** 相続人情報 **/
		// 住所
		if( sozoku[cnt].Sn_F6_JUSYO.GetLength() > 0 ){
			// 改行文字入る場合があるため除く
			CString str;
			str.Format( sozoku[cnt].Sn_F6_JUSYO );
			str.Replace( "\r", "" );
			str.Replace( "\n", "" );

			int cutlen = 40;
			int kind = IsKindOfLetter( str, cutlen-1 );
			if( kind == 2 ){
				cutlen--;
			}
			CString jodan = str.Left( cutlen );
			memmove( nohfuinfo[data_cnt].NINFADDR2_1, jodan.GetBuffer(), sizeof(nohfuinfo[data_cnt].NINFADDR2_1) );
			
			int totallen = str.GetLength();
			CString gedan = str.Mid( cutlen, totallen );
			memmove( nohfuinfo[data_cnt].NINFADDR2_2, gedan.GetBuffer(), sizeof(nohfuinfo[data_cnt].NINFADDR2_2) );
		}
		// 氏名
		if( sozoku[cnt].Sn_F6_NAME.GetLength() > 0 ){
			memmove( nohfuinfo[data_cnt].NINFNAME2, sozoku[cnt].Sn_F6_NAME.GetBuffer(), sizeof(nohfuinfo[data_cnt].NINFNAME2) );
			memmove( nohfuinfo[data_cnt].NINFNAME_TITLE, "相続人", sizeof(nohfuinfo[data_cnt].NINFNAME_TITLE) );
		}
		// カナ
		if( sozoku[cnt].Sn_F6_FURI.GetLength() > 0 ){
			memmove( nohfuinfo[data_cnt].NINFKANA, sozoku[cnt].Sn_F6_FURI.GetBuffer(), sizeof(nohfuinfo[data_cnt].NINFKANA) );
		}
		// 電話番号
		if( (sozoku[cnt].Sn_F6_TEL1.GetLength() > 0) || (sozoku[cnt].Sn_F6_TEL2.GetLength() > 0) || (sozoku[cnt].Sn_F6_TEL3.GetLength() > 0) ){
			CString tel;
			char	TelNo1[10] = {0};
			char	TelNo2[10] = {0};
			char	TelNo3[10] = {0};
			char	stel[64] = {0};
			char	ptel[64] = {0};
			int		len1, len2, len3;
			memmove( TelNo1, sozoku[cnt].Sn_F6_TEL1, 6 );
			memmove( TelNo2, sozoku[cnt].Sn_F6_TEL2, 4 );
			memmove( TelNo3, sozoku[cnt].Sn_F6_TEL3, 4 );
			m_Util.AnkSpaceCut( TelNo1, 6 );
			m_Util.AnkSpaceCut( TelNo2, 4 );
			m_Util.AnkSpaceCut( TelNo3, 4 );
			len1 = (int)strlen( TelNo1 );
			len2 = (int)strlen( TelNo2 );
			len3 = (int)strlen( TelNo3 );
			int i = 0;
			if( len1 || len2 || len3 ){
				if( !len1 ){
					strcat_s( ptel, sizeof(ptel), _T("    -") );
				}
				else{
					strcat_s( ptel, sizeof(ptel), TelNo1 );
					i = (int)strlen( ptel );
					ptel[i] = '-';
					ptel[i+1] = 0x00;
				}
				if( !len2 ){
					strcat_s( ptel, sizeof( ptel ), _T("    -") );
				}
				else{
					strcat_s( ptel, sizeof(ptel), TelNo2 );
					i = i = (int)strlen( ptel );
					ptel[i] = '-';
					ptel[i+1] = 0x00;
				}
				if( !len3 ){
					strcat_s( ptel, sizeof( ptel ), _T("    ") );
				}
				else{
					strcat_s( ptel, sizeof( ptel ), TelNo3 );
				}
				memmove( nohfuinfo[data_cnt].NINFTELNO, ptel, strlen( ptel ) );
			}
		}
		// 本税
		if( m_pArith->l_test( XM26 ) > 0 ){	// 本表の26にプラス金額

			// 納付税
			char nofu[6] = {0};
			if( (*m_pSnFhyo6Data)->Sn_F6_NOFUKANPUsw[cnt]&0x01 ){
				memmove( nofu, sozoku[cnt].Sn_F6_NOFUZEI, sizeof(nofu) );
			}
			else{
				memmove( nofu, F69[cnt], sizeof(nofu) );
			}

			// 地方税
			char chiho[6] = {0};
			if( (*m_pSnFhyo6Data)->Sn_F6_NOFUKANPUsw[cnt]&0x02 ){
				memmove( chiho, sozoku[cnt].Sn_F6_NOFUCHIHO, sizeof(chiho) );
			}
			else{
				memmove( chiho, F610[cnt], sizeof(chiho) );
			}

			m_pArith->l_add( nohfudata[data_cnt].HONZEI, nofu, chiho );
		}

		// 還付の場合、納付書の作成は必要ないため取得しない（所得税dbでも還付金額の転記はされてなかった）
		//if( m_pArith->l_test( XM26 ) < 0 ){	// 本表の26にマイナス金額

		//	// 還付
		//	if( (m_pArith->l_test( sozoku[cnt].Sn_F6_KANPUZEI ) != 0) || (m_pArith->l_test( sozoku[cnt].Sn_F6_KANPUCHIHO ) != 0) ){
		//		m_pArith->l_add( nohfudata[data_cnt].HONZEI, sozoku[cnt].Sn_F6_KANPUZEI, sozoku[cnt].Sn_F6_KANPUCHIHO );
		//	}
		//}


		/** 被相続人情報 **/
		// 住所
		if( strlen( (char*)(*m_pSnHeadData)->Sn_NOZEI ) > 0 ){
			int len = m_Util.MixSpaceCutLength( (char *)&(*m_pSnHeadData)->Sn_NOZEI, 40 );
			memmove( nohfuinfo[data_cnt].NINFADDR1, &(*m_pSnHeadData)->Sn_NOZEI, len );
			len = m_Util.MixSpaceCutLength( (char *)&(*m_pSnHeadData)->Sn_NOZEI[40], 40 );
			memmove( nohfuinfo[data_cnt].NINFADDR2, &(*m_pSnHeadData)->Sn_NOZEI[40], len );
		}
		// 氏名
		if( strlen( (char*)(*m_pSnHeadData)->Sn_DIHYO ) > 0 ){
			memmove( nohfuinfo[data_cnt].NINFNAME, (*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO) );
		}

		data_cnt++;
	}

	return data_cnt;
}

//==============================================================
// 納付書情報の更新
//--------------------------------------------------------------
// 引数		update_type	:	0:挿入 / 1:削除
//			index		:	sozoku[]のオフセット
//			flg			:	0:削除時にNFLineずらさない
//						:	1:削除時にNFLineずらす
//--------------------------------------------------------------
// 返送値	-1			:	エラー
//			0			:	正常終了
//==============================================================
int CShinFhyo6::UpdateShinPayment( int update_type, int index, int flg/*=0*/ )
{
	CString filter;
	filter.Format( _T("NFSequ = %d"), m_SnSeq );
	if( m_pDBSyzAcc->m_pShinPayment == NULL ){
		if( m_pDBSyzAcc->ShinPayMentOpen( m_pZmSub->m_database, filter ) == -1 ){
			return -1;
		}
	}
	else{
		if( m_pDBSyzAcc->m_pShinPayment->Requery( filter ) == -1 ){
			return -1;
		}
	}

	int max = m_pDBSyzAcc->m_pShinPayment->GetRecordCount();

	if( max > 0 ){

		CString sql;
		int sozoku_no = index+2;	// 配列順に+2したものを相続人の番号として扱う

		// 削除
		if( update_type ){

			// 該当データの削除
			filter.Format( "NFSequ = %d AND NFLine = %d", m_SnSeq, sozoku_no );
			if( m_pDBSyzAcc->m_pShinPayment->Requery( filter ) != -1 ){
				m_pDBSyzAcc->m_pShinPayment->Delete();
			}

			if( flg ){
				// 該当データ以降のNFLineをずらす
				sql.Format( "UPDATE ShinPayment SET NFLine = NFLine - 1 WHERE NFSequ = %d AND NFLine > %d", m_SnSeq, sozoku_no );
				m_pZmSub->m_database->BeginTrans();
				try{
					m_pZmSub->m_database->ExecuteSQL( sql );
				}
				catch( CDBException *pdbe ){
					m_pZmSub->m_database->Rollback();
					pdbe->Delete();
					return -1;
				}
				m_pZmSub->m_database->CommitTrans();
			}
		}
		// 挿入
		else{

			// 該当データ以降のNFLineをずらす
			sql.Format( "UPDATE ShinPayment SET NFLine = NFLine + 1 WHERE NFSequ = %d AND NFLine >= %d", m_SnSeq, sozoku_no );
			m_pZmSub->m_database->BeginTrans();
			try{
				m_pZmSub->m_database->ExecuteSQL( sql );
			}
			catch( CDBException *pdbe ){
				m_pZmSub->m_database->Rollback();
				pdbe->Delete();
				return -1;
			}
			m_pZmSub->m_database->CommitTrans();

			// 該当データの追加
			m_pDBSyzAcc->m_pShinPayment->AddNew();
			m_pDBSyzAcc->m_pShinPayment->NFItem = 1;
			m_pDBSyzAcc->m_pShinPayment->NFLine = sozoku_no;	// データ自体は納付書を開いたときに書き込みする。一旦相続人番号をセットしたレコードを追加。
			m_pDBSyzAcc->m_pShinPayment->NFSequ = m_SnSeq;
			m_pDBSyzAcc->m_pShinPayment->Update();
		}
	}

	return 0;
}
//<---- 納付書・相続人対応