// ShinFhyo40.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo40.h"
#include "ShinFhyo40Idx.h"


// CShinFhyo40 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo40, CSyzBaseDlg)

CShinFhyo40::CShinFhyo40(CWnd* pParent /*=NULL*/)	
	: CSyzBaseDlg(CShinFhyo40::IDD, pParent)
	, m_pParent(pParent)
	, m_pSnHeadData(NULL)
	, m_pArith(NULL)
	, m_pShinInfo(NULL)
	, m_pSnHonpyoData(NULL)
	, m_FrSw(0)
{
	memset( F41A, '\0', sizeof(F41A) );
	memset( F41B, '\0', sizeof(F41B) );
	memset( F41C, '\0', sizeof(F41C) );
	memset( F41T, '\0', sizeof(F41T) );

	memset( F42A, '\0', sizeof(F42A) );
	memset( F42B, '\0', sizeof(F42B) );
	memset( F42C, '\0', sizeof(F42C) );
	memset( F42T, '\0', sizeof(F42T) );

	memset( F43A, '\0', sizeof(F43A) );
	memset( F43B, '\0', sizeof(F43B) );
	memset( F43C, '\0', sizeof(F43C) );
	memset( F43T, '\0', sizeof(F43T) );

	memset( F44A, '\0', sizeof(F44A) );
	memset( F44B, '\0', sizeof(F44B) );
	memset( F44C, '\0', sizeof(F44C) );
	memset( F44T, '\0', sizeof(F44T) );

	memset( F45A, '\0', sizeof(F45A) );
	memset( F45B, '\0', sizeof(F45B) );
	memset( F45C, '\0', sizeof(F45C) );
	memset( F45T, '\0', sizeof(F45T) );

	memset( F46A, '\0', sizeof(F46A) );
	memset( F46B, '\0', sizeof(F46B) );
	memset( F46C, '\0', sizeof(F46C) );
	memset( F46T, '\0', sizeof(F46T) );

	memset( F47A, '\0', sizeof(F47A) );
	memset( F47B, '\0', sizeof(F47B) );
	memset( F47C, '\0', sizeof(F47C) );
	memset( F47T, '\0', sizeof(F47T) );

	memset( F48A, '\0', sizeof(F48A) );
	memset( F48B, '\0', sizeof(F48B) );
	memset( F48C, '\0', sizeof(F48C) );
	memset( F48T, '\0', sizeof(F48T) );

	memset( F49A, '\0', sizeof(F49A) );
	memset( F49B, '\0', sizeof(F49B) );
	memset( F49C, '\0', sizeof(F49C) );
	memset( F49T, '\0', sizeof(F49T) );

	memset( F410T, '\0', sizeof(F410T) );

	memset( F411B, '\0', sizeof(F411B) );
	memset( F411C, '\0', sizeof(F411C) );
	memset( F411T, '\0', sizeof(F411T) );

	memset( F412B, '\0', sizeof(F412B) );
	memset( F412C, '\0', sizeof(F412C) );
	memset( F412T, '\0', sizeof(F412T) );

	memset( F413T, '\0', sizeof(F413T) );

	memset( F414B, '\0', sizeof(F414B) );
	memset( F414C, '\0', sizeof(F414C) );
	memset( F414T, '\0', sizeof(F414T) );

	memset( F415B, '\0', sizeof(F415B) );
	memset( F415C, '\0', sizeof(F415C) );
	memset( F415T, '\0', sizeof(F415T) );

	memset( F416T, '\0', sizeof(F416T) );

	F4c = 0;

	m_curwnd = -1;
}

CShinFhyo40::~CShinFhyo40()
{
}

void CShinFhyo40::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Fhyo40Diag01);
	DDX_Control(pDX, IDC_ICSDIAGCTRL2, m_Fhyo40Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo40, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinFhyo40 メッセージ ハンドラ

BOOL CShinFhyo40::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期化
	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Fhyo40Diag02;
	m_BottomIdx = 119;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSnFhyo40Data	：	付表４データ読込みクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//		pSnHonpyoData	：	本表データ読込みクラス
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo40::InitInfo( CH26SnFhyo10Data **pSnFhyo40Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo , CH26SnHonpyoData **pSnHonpyoData )
{
	ASSERT( pSnFhyo40Data );
	if( pSnFhyo40Data == NULL ){
		return -1;
	}
	m_pSnFhyo40Data = pSnFhyo40Data;

	//[Y]
	m_pSnHonpyoData = pSnHonpyoData;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pArith );
	if( pArith == NULL ){
		return -1;
	}
	m_pArith = pArith;

	ASSERT( pShinInfo );
	if( pShinInfo == NULL ){
		return -1;
	}
	m_pShinInfo = pShinInfo;

	return 0;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo40::Init()
{
	// データの読込み
	ReadData( m_FrSw );
	m_FrSw = 1;

	// ダイアグラム初期属性セット
	InitDiagAttr();

	// 画面出力
	DispFh4();

	//-->削除 harino ('14.03.06)
	// 課税標準率A欄へ
	//if( m_pSnHeadData->Sn_Sign4&0x80 ){
	//	diag_setposition( IDC_ICSDIAGCTRL2, ID41A, CIcsdiagctrl );
	//}
	//<--削除 harino ('14.03.06)

	return 0;
}

//-----------------------------------------------------------------------------
// 画面出力
//-----------------------------------------------------------------------------
void CShinFhyo40::DispFh4()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 画面の初期化
	for( int i=3; i<=5; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL2, (short )i );
	}
	for( int i=ID41A; i<=ID416D; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL2, (short )i );
	}

	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){//[Y]連動の場合手入力サイン初期化
		//手入力サイン初期化
		F4c = 0;
	}

	// 期間・氏名の表示
	char	buf[512] = {0};
	char	yy, mm, dd;
	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s( buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy&0xff),(mm&0xff),(dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 4, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	yy = mm = dd = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd );
	sprintf_s( buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy&0xff),(mm&0xff),(dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 5, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	memset( buf, '\0', sizeof(buf) );
	if( (*m_pSnHeadData)->IsSoleProprietor() ){
		memmove( buf, (*m_pSnHeadData)->Sn_DIHYO, 40 );
	}
	else{
		memmove( buf, (*m_pSnHeadData)->Sn_CONAM, 40 );
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 課税標準額A
	char	WORK0[MONY_BUF_SIZE]={0};
	m_Util.l_calq( WORK0, F41A, 0 );
	dsp_prs( IDC_ICSDIAGCTRL2, ID41A, WORK0 );
	// 課税標準額B
	m_Util.l_calq( WORK0, F41B, 0 );
	dsp_prs( IDC_ICSDIAGCTRL2, ID41B, WORK0 );
	// 課税標準額C
	m_Util.l_calq( WORK0, F41C, 0 );
	dsp_prs( IDC_ICSDIAGCTRL2, ID41C, WORK0 );
	// 課税標準額計
	f4calq_3( 0 );
	//[Y]
	f4calq_4( 0 );

	// 消費税額A
	dsp_prs( IDC_ICSDIAGCTRL2, ID42A, F42A );
	// 消費税額B
	dsp_prs( IDC_ICSDIAGCTRL2, ID42B, F42B );
	// 消費税額C
	dsp_prs( IDC_ICSDIAGCTRL2, ID42C, F42C );
	// 消費税額計
	f4calq_6( 0 );

	// 控除過大調整税額 or 貸倒回収に係る税額 ・控除対象仕入税額
	f4calq_78( 0 );

	// 返還等対価に係る税額
	f4calq_9( 0 );

	// 貸倒に係る税額
	f4calq_10( 0 );

	// 控除税額小計
	f4calq_11( 0 );

	// 控除不足還付税額・差引税額
	f4calq_1213( 0 );

	// 合計差引税額
	f4calq_14( 0 );

	// 譲渡割額(14,15)
	f4calq_15( 0 );
}

//-----------------------------------------------------------------------------
// データの読込み
//-----------------------------------------------------------------------------
// 引数		pas	：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFhyo40::ReadData( int pas )
{
	// 課税標準額
	memmove( F41A, (*m_pSnFhyo40Data)->Sn_1F1A, MONY_BUF_SIZE );
	memmove( F41B, (*m_pSnFhyo40Data)->Sn_1F1B, MONY_BUF_SIZE );
	memmove( F41C, (*m_pSnFhyo40Data)->Sn_1F1C, MONY_BUF_SIZE );
	
	// 消費税額
	memmove( F42A, (*m_pSnFhyo40Data)->Sn_1F2A, MONY_BUF_SIZE );
	memmove( F42B, (*m_pSnFhyo40Data)->Sn_1F2B, MONY_BUF_SIZE );
	memmove( F42C, (*m_pSnFhyo40Data)->Sn_1F2C, MONY_BUF_SIZE );

	F4c = (*m_pSnFhyo40Data)->Sn_1F2sw;//[Y]

	// 貸倒回収に係る消費税額
	memmove( F43A, (*m_pSnFhyo40Data)->Sn_1F3A, MONY_BUF_SIZE );
	memmove( F43B, (*m_pSnFhyo40Data)->Sn_1F3B, MONY_BUF_SIZE );
	memmove( F43C, (*m_pSnFhyo40Data)->Sn_1F3C, MONY_BUF_SIZE );
	m_pArith->l_add( F43T, F43A, F43B );
	m_pArith->l_add( F43T, F43T, F43C );
	if( pas == 1 ){
		memmove( F43S, F43T, MONY_BUF_SIZE );
	}

	// 控除対象仕入税額
	memmove( F44A, (*m_pSnFhyo40Data)->Sn_1F4A, MONY_BUF_SIZE );
	memmove( F44B, (*m_pSnFhyo40Data)->Sn_1F4B, MONY_BUF_SIZE );
	memmove( F44C, (*m_pSnFhyo40Data)->Sn_1F4C, MONY_BUF_SIZE );
//--> '14.10.17 INS START
	memmove( F44T, (*m_pSnFhyo40Data)->Sn_1F4T, MONY_BUF_SIZE );
//<-- '14.10.17 INS END

	// 返還等対価に係る税額
	memmove( F45A, (*m_pSnFhyo40Data)->Sn_1F5A, MONY_BUF_SIZE );
	memmove( F45B, (*m_pSnFhyo40Data)->Sn_1F5B, MONY_BUF_SIZE );
	memmove( F45C, (*m_pSnFhyo40Data)->Sn_1F5C, MONY_BUF_SIZE );

	// 貸倒に係る税額
	memmove( F46A, (*m_pSnFhyo40Data)->Sn_1F6A, MONY_BUF_SIZE );
	memmove( F46B, (*m_pSnFhyo40Data)->Sn_1F6B, MONY_BUF_SIZE );
	memmove( F46C, (*m_pSnFhyo40Data)->Sn_1F6C, MONY_BUF_SIZE );

	return 0;
}

//-----------------------------------------------------------------------------
// データの書込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo40::WriteData()
{
	// 課税標準額
	memmove( (*m_pSnFhyo40Data)->Sn_1F1A, F41A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F1B, F41B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F1C, F41C, MONY_BUF_SIZE );

	// 消費税額
	memmove( (*m_pSnFhyo40Data)->Sn_1F2A, F42A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F2B, F42B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F2C, F42C, MONY_BUF_SIZE );

	(*m_pSnFhyo40Data)->Sn_1F2sw = F4c;//[Y]

	// 貸倒回収に係る消費税額
	memmove( (*m_pSnFhyo40Data)->Sn_1F3A, F43A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F3B, F43B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F3C, F43C, MONY_BUF_SIZE );

	// 控除対象仕入額
	memmove( (*m_pSnFhyo40Data)->Sn_1F4A, F44A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F4B, F44B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F4C, F44C, MONY_BUF_SIZE );
// 合計？？？
//	memmove( pSyzShin->Snd.Sn_1F8C, F48C, 6 );

	// 返還等対価に係る税額
	memmove( (*m_pSnFhyo40Data)->Sn_1F5A, F45A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F5B, F45B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F5C, F45C, MONY_BUF_SIZE );
// 合計？？？
//	memmove( pSyzShin->Snd.Sn_1F9C, F49C, 6 );

	// 貸倒れに係る税額
	memmove( (*m_pSnFhyo40Data)->Sn_1F6A, F46A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F6B, F46B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F6C, F46C, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove( (*m_pSnFhyo40Data)->Sn_1F8A, F48A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F8B, F48B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F8C, F48C, MONY_BUF_SIZE );

	// 差引税額
	memmove( (*m_pSnFhyo40Data)->Sn_1F9A, F49A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F9B, F49B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo40Data)->Sn_1F9C, F49C, MONY_BUF_SIZE );

	// 合計差引税額
	memmove( (*m_pSnFhyo40Data)->Sn_1F10T, F410T, MONY_BUF_SIZE );

	return 0;
}

//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//		Mony	：	表示金額
//-----------------------------------------------------------------------------
void CShinFhyo40::dsp_prs( unsigned short Id, short Index, char *Mony )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch( Index ){
		case ID41A	:	//  1. 課税標準額
		case ID41B	:
		case ID41C	:
		case ID42A	:	//  2. 消費税額
		case ID42B	:
		case ID42C	:
		case ID43A	:	//  3. 貸倒回収に係る消費税額
		case ID43B	:
		case ID43C	:
		case ID45A	:	//  5.変換等対価に係る税額
		case ID45B	:
		case ID45C	:
		case ID46A	:	//  6. 貸倒れに係る税額
		case ID46B	:
		case ID46C	:
			memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
			diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			break;
		default		:
			memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
			diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			break;
	}
}

//-----------------------------------------------------------------------------
// 金額表示クリア
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//-----------------------------------------------------------------------------
void CShinFhyo40::dsp_cls( unsigned short Id, short Index )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch( Index ){
		case 4 :	// 課税期間　自
		case 5 :	// 課税期間　至
		case 3 :	// 氏名又は､名称
			diag_clear( Id, Index, TRUE, CIcsdiagctrl );
			break;
		default :
			memset( DiagData.data_val, '\0', MONY_BUF_SIZE );
			diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			break;
	}
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFhyo40::InitDiagAttr()
{
	int	cnt = 0;
	while( 1 ){
		if( Fhyo4[cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Fhyo4[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Fhyo4[cnt].UnConnect;
			}
			else{
				Flg = Fhyo4[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL2, Fhyo4[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Fhyo4[cnt].Connect );
		cnt++;
	}

	m_Fhyo40Diag02.EnableDelete(0);//[Y]DELETEキー１回で削除モード設定

	return 0;
}

BOOL CShinFhyo40::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			// 付表４更新
			GetAllDiagData();
			WriteData();
//			CWnd*	p1	=	GetParent();
//			CWnd*	p2	=	p1->GetParent();
//			((CSyzShinSheet*)p2)->Kanik_calq( 1 );

//			((CMainFrame *)AfxGetMainWnd())->PostMessageA( WM_CLOSE, 0 );

			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}

			return TRUE;
		}
		//-->追加 harino ('14.03.04)
		if( pMsg->wParam == VK_F2 ){
			DIAGRAM_DATA   dd;
			if( m_curwnd != -1) {
				short result = ((CIcsdiagctrl*)GetDlgItem( m_curwnd ))->GetPosition();
				diag_getdata( m_curwnd, result, &dd, CIcsdiagctrl );
				if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
					TerminationIcsdiagctrl2( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}
		//<--追加 harino ('14.03.04)
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}
BEGIN_EVENTSINK_MAP(CShinFhyo40, CSyzBaseDlg)
	ON_EVENT(CShinFhyo40, IDC_ICSDIAGCTRL2, 1, CShinFhyo40::EditONIcsdiagctrl2, VTS_I2)
	ON_EVENT(CShinFhyo40, IDC_ICSDIAGCTRL2, 2, CShinFhyo40::EditOFFIcsdiagctrl2, VTS_I2)
	ON_EVENT(CShinFhyo40, IDC_ICSDIAGCTRL2, 3, CShinFhyo40::TerminationIcsdiagctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo40::EditONIcsdiagctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//-->Terminationへ移動 harino ('14.03.07)
	//RECT	rectA, rectB;
	//GetWindowRect( &rectA );
	//diag_getrect( IDC_ICSDIAGCTRL2, index, &rectB, CIcsdiagctrl);
	//if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
	//	int	cpos = GetScrollPos( SB_VERT );
	//	SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	//}
	//<--Terminationへ移動 harino ('14.03.07)
	m_curwnd = IDC_ICSDIAGCTRL2;
}

void CShinFhyo40::EditOFFIcsdiagctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	char	*pMony;
	int		tmpIdx;
	char	CkMy[MONY_BUF_SIZE] = {0};

	switch( index ){

		case ID41A	:	//  1.課税標準額
		case ID41B	:	//  1.課税標準額
		case ID41C	:	//  1.課税標準額
			if( index == ID41A ){
				pMony = F41A;
				tmpIdx = ID41A;
			}
			else if( index == ID41B ){
				pMony = F41B;
				tmpIdx = ID41B;
			}
			else if( index == ID41C ){
				pMony = F41C;
				tmpIdx = ID41C;
			}
			else{
				break;
			}

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, pMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, tmpIdx, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( index == ID41A ){
				if( m_pArith->l_cmp(pMony, CkMy) ){
					if(!(F4c&0x08)){//[Y]手入力されていない場合のみサインたてる
						F4c |= 0x01;
					}
				}
			}
			else if( index == ID41B ){
				if( m_pArith->l_cmp(pMony, CkMy) ){
					if(!(F4c&0x80)){//[Y]手入力されていない場合のみサインたてる
						F4c |= 0x10;
					}
				}
			}
			else if( index == ID41C ){
				if( m_pArith->l_cmp(pMony, CkMy) ){
					if(!(F4c&0x800)){//[Y]手入力されていない場合のみサインたてる
						F4c |= 0x100;
					}
				}
			}

			// 1000円未満切り捨て
			m_Util.l_calq( pMony, CkMy, 0 );
			memmove( DiagData.data_val, pMony, MONY_BUF_SIZE ); 
			diag_setdata( IDC_ICSDIAGCTRL2, tmpIdx, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			// 各入力の計算表示
			f4calq_all( 1, 0 );
			WriteData();
			calqx_otherlists();
			disp_by_recalq();
			break;

		case ID42A	:	//  2. 消費税
		case ID42B	:	//  2. 消費税
		case ID42C	:	//  2. 消費税
			if( index == ID42A ){
				pMony = F42A;
				tmpIdx = ID42A;
			}
			else if( index == ID42B ){
				pMony = F42B;
				tmpIdx = ID42B;
			}
			else if( index == ID42C ){
				pMony = F42C;
				tmpIdx = ID42C;
			}
			else{
				break;
			}

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, pMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, tmpIdx, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( index == ID42A ){
				if( m_pArith->l_cmp(pMony, CkMy) ){
					F4c |= 0x08;
					F4c &= ~0x01;
				}
			}
			else if( index == ID42B ){
				if( m_pArith->l_cmp(pMony, CkMy) ){
					F4c |= 0x80;
					F4c &= ~0x10;
				}
			}
			else if( index == ID42C ){
				if( m_pArith->l_cmp(pMony, CkMy) ){
					F4c |= 0x800;
					F4c &= ~0x100;
				}
			}

			diag_getdata( IDC_ICSDIAGCTRL2, tmpIdx, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( pMony, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f4calq_all( 2, 0 );//[Y]
			WriteData();
			calqx_otherlists();
			disp_by_recalq();

			break;

		case ID43A	:	//  3. 貸倒回収に係る税額
		case ID43B	:	//  3. 貸倒回収に係る税額
		case ID43C	:	//  3. 貸倒回収に係る税額
			if( index == ID43A ){
				pMony = F43A;
				tmpIdx = ID43A;
			}
			else if( index == ID43B ){
				pMony = F43B;
				tmpIdx = ID43B;
			}
			else if( index == ID43C ){
				pMony = F43C;
				tmpIdx = ID43C;
			}
			else{
				break;
			}

			diag_getdata( IDC_ICSDIAGCTRL2, tmpIdx, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( pMony, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f4calq_all( 5, 0 );
			WriteData();
			calqx_otherlists();
			disp_by_recalq();
			break;

		case ID45A	:	//  5. 返還等対価に係る税額
		case ID45B	:	//  5. 返還等対価に係る税額
		case ID45C	:	//  5. 返還等対価に係る税額
			if( index == ID45A ){
				pMony = F45A;
				tmpIdx = ID45A;
			}
			else if( index == ID45B ){
				pMony = F45B;
				tmpIdx = ID45B;
			}
			else if( index == ID45C ){
				pMony = F45C;
				tmpIdx = ID45C;
			}
			else{
				break;
			}

			diag_getdata( IDC_ICSDIAGCTRL2, tmpIdx, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( pMony, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f4calq_all( 7, 0 );
			WriteData();
			calqx_otherlists();
			disp_by_recalq();
			break;

		case ID46A	:	// 6. 貸倒に係る税額
		case ID46B	:	// 6. 貸倒に係る税額
		case ID46C	:	// 6. 貸倒に係る税額
			if( index == ID46A ){
				pMony = F46A;
				tmpIdx = ID46A;
			}
			else if( index == ID46B ){
				pMony = F46B;
				tmpIdx = ID46B;
			}
			else if( index == ID46C ){
				pMony = F46C;
				tmpIdx = ID46C;
			}
			else{
				break;
			}
			diag_getdata( IDC_ICSDIAGCTRL2, tmpIdx, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( pMony, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f4calq_all( 9, 0 );
			WriteData();
			calqx_otherlists();
			disp_by_recalq();
			break;
			
		default		:
			break;
	}
}

void CShinFhyo40::TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}

	// Shift+TABをTABに、TABをENTERに返還
	char	VK_FLG = 0x00;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );
	short	wTerm;
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
		pos = UpDownFocusMoveCheck(index , 1);
		m_Fhyo40Diag02.SetPosition(pos);
	}
	if( wTerm==VK_DOWN ){
		pos = UpDownFocusMoveCheck(index , 0);
		m_Fhyo40Diag02.SetPosition(pos);
	}


	if( (wTerm==VK_RETURN) || (wTerm==VK_RIGHT) || (wTerm==VK_F3) ){
		m_Fhyo40Diag02.SetNextPosition();
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		m_Fhyo40Diag02.SetPrevPosition();
	}

	//[Y]
	if( wTerm == VK_DELETE ){
		switch( index ){
			case ID42A	:	//  2.A 消費税(税率3%分)
				F4c &= ~0x08;
				F4c |= 0x01;
				// 各入力の計算表示
				f4calq_all( 2, 0 );
				m_Fhyo40Diag02.SetPosition( ID42A );
				break;

			case ID42B	:	//  2.B 消費税(税率4%分)
				F4c &= ~0x80;
				F4c |= 0x10;
				// 各入力の計算表示
				f4calq_all( 2, 0 );
				m_Fhyo40Diag02.SetPosition( ID42B );
				break;

			case ID42C	:	//  2.B 消費税(税率6.3%分)
				F4c &= ~0x800;
				F4c |= 0x100;
				// 各入力の計算表示
				f4calq_all( 2, 0 );
				m_Fhyo40Diag02.SetPosition( ID42C );
				break;

			default :
				dsp_cls(IDC_ICSDIAGCTRL2 , index);//[Y]
				break;
		}
	}

	//-->Editonから移動 harino ('14.03.07)
	int nowpos = m_Fhyo40Diag02.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL2, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
	//-->Editonから移動 harino ('14.03.07)
}

//-----------------------------------------------------------------------------
// 各入力データ抽出
//-----------------------------------------------------------------------------
void CShinFhyo40::GetAllDiagData()
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	//  1. 課税標準額
	diag_getdata( IDC_ICSDIAGCTRL2, ID41A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F41A, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID41B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F41B, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID41C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F41C, DiagData.data_val, MONY_BUF_SIZE );

	//  2. 消費税
	diag_getdata( IDC_ICSDIAGCTRL2, ID42A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F42A, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID42B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F42B, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID42C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F42C, DiagData.data_val, MONY_BUF_SIZE ); 

	//  3. 貸倒回収に係る税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID43A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F43A, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID43B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F43B, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID43C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F43C, DiagData.data_val, MONY_BUF_SIZE ); 
	
	//  5. 返還等対価に係る税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID45A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F45A, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID45B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F45B, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID45C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F45C, DiagData.data_val, MONY_BUF_SIZE ); 
	
	// 6. 貸倒に係る税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID46A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F46A, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID46B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F46B, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID46C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F46C, DiagData.data_val, MONY_BUF_SIZE ); 
}


//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinFhyo40::UpDownFocusMoveCheck(int Index , int Move)
{
	int idx = 0;
	int i = 0 , j = 0;

	DIAGRAM_ATTRIBUTE	DA;

	//現在ポジション検索
	for(i = 0 ; i < FCS_FHYO4CNT ; i++){
		if(Index == FCS_Fhyo4[i].IDNo){
			break;
		}
	}

	idx = FCS_Fhyo4[i].IDNo;//現在ポジションセット

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_FHYO4CNT ; j++){
			m_Fhyo40Diag02.GetAttribute(FCS_Fhyo4[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo4[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Fhyo40Diag02.GetAttribute(FCS_Fhyo4[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo4[j].IDNo;
				break;
			}
		}
	}

	return idx;
}


//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
void CShinFhyo40::SetItemPosition()
{
	int index;

	//-->追加 harino ('14.03.06)
	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
		index = m_Fhyo40Diag02.GetPosition();
		if( (index = m_Fhyo40Diag02.GetPosition()) != -1) {
			m_Fhyo40Diag02.SetPosition(index);
			m_Fhyo40Diag02.SetFocus();
			return;
		}
	}
	//<--追加 harino ('14.03.06)

	m_Fhyo40Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット ('14.04.14)
	m_Fhyo40Diag02.SetFocus();

	//入力可能なインデックスを探す処理が必要？
	m_Fhyo40Diag02.SetPosition( ID41A );

}


//-----------------------------------------------------------------------------
//[Y] 入力項目色変え
//-----------------------------------------------------------------------------
// 引数	id		；	ダイアグラムID
//		index	：	ダイアグラム内インデックス
//		sign	：	変更サイン
//-----------------------------------------------------------------------------
void CShinFhyo40::ChangeColor( unsigned short id, short Index, int sign )
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
//<-------------------------------

//-----------------------------------------------------------------------------
// 他表の再集計による再表示
//-----------------------------------------------------------------------------
void CShinFhyo40::disp_by_recalq()
{
	// 控除対象仕入税額
	memmove( F44A, (*m_pSnFhyo40Data)->Sn_1F4A, MONY_BUF_SIZE );
	memmove( F44B, (*m_pSnFhyo40Data)->Sn_1F4B, MONY_BUF_SIZE );
	memmove( F44C, (*m_pSnFhyo40Data)->Sn_1F4C, MONY_BUF_SIZE );
//--> '14.10.17 INS START
	memmove( F44T, (*m_pSnFhyo40Data)->Sn_1F4T, MONY_BUF_SIZE );
//--> '14.10.17 INS END

	// 再表示
	DispFh4();
}
