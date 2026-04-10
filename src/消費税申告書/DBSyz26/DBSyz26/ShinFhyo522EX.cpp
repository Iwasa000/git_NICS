// ShinFhyo522.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo52EX.h"
#include "ShinFhyo522EX.h"

#include "ShinFhyo522EXIdx.h"

// CShinFhyo522EX ダイアログ

//#define	_20141017_EXTEND_	1

IMPLEMENT_DYNAMIC(CShinFhyo522EX, CSyzBaseDlg)

CShinFhyo522EX::CShinFhyo522EX(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo522EX::IDD, pParent)
	, m_pSnFhyo52Data(NULL)
	, m_pSnFhyo40Data(NULL)
	, m_pSnHonpyoData(NULL)
	, m_pSnHeadData(NULL)
	, m_pArith(NULL)
	, m_pShinInfo(NULL)
	, m_pShinFhyo52(NULL)
	, m_pParent(pParent)
	, m_FrSw(0)
	, m_ptypeCnt(NULL)
	, m_pCQsw(NULL)
	, m_DpSw(0)
	, m_pZmSub(NULL)
	, m_pdspSkjSw(NULL)
	, m_IsDeleteKeyIn(FALSE)
{
	pF5220T = NULL;
	pF5220A = NULL;
	pF5220B = NULL;
	pF5220C = NULL;

	pF5221T = NULL;
	pF5221A = NULL;
	pF5221B = NULL;
	pF5221C = NULL;

	pF5222T = NULL;
	pF5222A = NULL;
	pF5222B = NULL;
	pF5222C = NULL;

	pF5223T = NULL;
	pF5223A = NULL;
	pF5223B = NULL;
	pF5223C = NULL;

	pF5224T = NULL;
	pF5224A = NULL;
	pF5224B = NULL;
	pF5224C = NULL;

	pF5225T = NULL;
	pF5225A = NULL;
	pF5225B = NULL;
	pF5225C = NULL;

	pF5226T = NULL;
	pF5226A = NULL;
	pF5226B = NULL;
	pF5226C = NULL;

	pF5227T = NULL;
	pF5227A = NULL;
	pF5227B = NULL;
	pF5227C = NULL;

	pF5228T = NULL;
	pF5228A = NULL;
	pF5228B = NULL;
	pF5228C = NULL;

	pF5229T = NULL;
	pF5229A = NULL;
	pF5229B = NULL;
	pF5229C = NULL;

	pF5230T = NULL;
	pF5230A = NULL;
	pF5230B = NULL;
	pF5230C = NULL;

	pF5231T = NULL;
	pF5231A = NULL;
	pF5231B = NULL;
	pF5231C = NULL;

	pF5232T = NULL;
	pF5232A = NULL;
	pF5232B = NULL;
	pF5232C = NULL;

	pF5233T = NULL;
	pF5233A = NULL;
	pF5233B = NULL;
	pF5233C = NULL;

	pF5234T = NULL;
	pF5234A = NULL;
	pF5234B = NULL;
	pF5234C = NULL;

	pF5235T = NULL;
	pF5235A = NULL;
	pF5235B = NULL;
	pF5235C = NULL;

	pF5236T = NULL;
	pF5236A = NULL;
	pF5236B = NULL;
	pF5236C = NULL;

	pF5237T = NULL;
	pF5237A = NULL;
	pF5237B = NULL;
	pF5237C = NULL;

	pF5220c = NULL;
	pF5221c = NULL;
	pF5222c = NULL;
	pF5223c = NULL;
	pF5224c = NULL;
	pF5225c = NULL;
	pF5226c = NULL;
	pF5227c = NULL;
	pF5228c = NULL;
	pF5229c = NULL;
	pF5230c = NULL;
	pF5231c = NULL;
	pF5231c = NULL;
	pF5232c = NULL;
	pF5233c = NULL;
	pF5234c = NULL;
	pF5235c = NULL;
	pF5236c = NULL;

	m_curwnd = -1;
}

CShinFhyo522EX::~CShinFhyo522EX()
{
}

void CShinFhyo522EX::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Fhyo522Diag01);
	DDX_Control(pDX, IDC_ICSDIAGCTRL2, m_Fhyo522Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo522EX, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinFhyo522EX メッセージ ハンドラ

BOOL CShinFhyo522EX::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Fhyo522Diag01;
	m_BottomIdx = 225;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSnFhyo52Data	：	付表５－２データ読込みクラス
//		pSnFhyo40Data	：	付表４データ読込みクラス
//		pSnHonpyoData	：	本表データ読込みクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//		pShinFhyo52		：	付表５－２（１）ウィンドウ
//		pZmSub			：	財務クラス
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo522EX::InitInfo( CH27SnFhyo50Data **pSnFhyo52Data, CH27SnFhyo10Data **pSnFhyo40Data, CH27SnHonpyoData **pSnHonpyoData, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CShinFhyo52EX *pShinFhyo52, CDBNpSub *pZmSub )
//int CShinFhyo522EX::InitInfo( CH26SnFhyo50Data **pSnFhyo52Data, CH26SnFhyo10Data **pSnFhyo40Data, CH26SnHonpyoData **pSnHonpyoData, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo, CShinFhyo52EX *pShinFhyo52, CDBNpSub *pZmSub )
{
	ASSERT( pSnFhyo52Data );
	if( pSnFhyo52Data == NULL ){
		return -1;
	}
	m_pSnFhyo52Data = pSnFhyo52Data;

	ASSERT( pSnFhyo40Data );
	if( pSnFhyo40Data == NULL ){
		return -1;
	}
	m_pSnFhyo40Data = pSnFhyo40Data;

	ASSERT( pSnHonpyoData );
	if( pSnHonpyoData == NULL ){
		return -1;
	}
	m_pSnHonpyoData = pSnHonpyoData;

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

	ASSERT( pShinFhyo52 );
	if( pShinFhyo52 == NULL ){
		return -1;
	}
	m_pShinFhyo52 = pShinFhyo52;

	ASSERT( pZmSub );
	if( pZmSub == NULL ){
		return -1;
	}
	m_pZmSub = pZmSub;

	SH_52EX_LINKED_INFO	LinkedInfo = {0};
	LinkedInfo.ptypeCnt = &m_ptypeCnt;
	LinkedInfo.pCQsw = &m_pCQsw;
	LinkedInfo.pSzsw = &m_pSzsw;
	LinkedInfo.pSBsw = &m_pSBsw;
	LinkedInfo.ptkSw = &m_ptkSw;
	LinkedInfo.pdspSkjSw = &m_pdspSkjSw;	// '14.06.11
	LinkedInfo.pF5220T = &pF5220T;
	LinkedInfo.pF5220A = &pF5220A;
	LinkedInfo.pF5220B = &pF5220B;
	LinkedInfo.pF5220C = &pF5220C;
	LinkedInfo.pF5220c = &pF5220c;

	LinkedInfo.pF5221T = &pF5221T;
	LinkedInfo.pF5221A = &pF5221A;
	LinkedInfo.pF5221B = &pF5221B;
	LinkedInfo.pF5221C = &pF5221C;
	LinkedInfo.pF5221c = &pF5221c;

	LinkedInfo.pF5222T = &pF5222T;
	LinkedInfo.pF5222A = &pF5222A;
	LinkedInfo.pF5222B = &pF5222B;
	LinkedInfo.pF5222C = &pF5222C;
	LinkedInfo.pF5222c = &pF5222c;

	LinkedInfo.pF5223T = &pF5223T;
	LinkedInfo.pF5223A = &pF5223A;
	LinkedInfo.pF5223B = &pF5223B;
	LinkedInfo.pF5223C = &pF5223C;
	LinkedInfo.pF5223c = &pF5223c;

	LinkedInfo.pF5224T = &pF5224T;
	LinkedInfo.pF5224A = &pF5224A;
	LinkedInfo.pF5224B = &pF5224B;
	LinkedInfo.pF5224C = &pF5224C;
	LinkedInfo.pF5224c = &pF5224c;

	LinkedInfo.pF5225T = &pF5225T;
	LinkedInfo.pF5225A = &pF5225A;
	LinkedInfo.pF5225B = &pF5225B;
	LinkedInfo.pF5225C = &pF5225C;
	LinkedInfo.pF5225c = &pF5225c;

	LinkedInfo.pF5226T = &pF5226T;
	LinkedInfo.pF5226A = &pF5226A;
	LinkedInfo.pF5226B = &pF5226B;
	LinkedInfo.pF5226C = &pF5226C;
	LinkedInfo.pF5226c = &pF5226c;

	LinkedInfo.pF5227T = &pF5227T;
	LinkedInfo.pF5227A = &pF5227A;
	LinkedInfo.pF5227B = &pF5227B;
	LinkedInfo.pF5227C = &pF5227C;
	LinkedInfo.pF5227c = &pF5227c;

	LinkedInfo.pF5228T = &pF5228T;
	LinkedInfo.pF5228A = &pF5228A;
	LinkedInfo.pF5228B = &pF5228B;
	LinkedInfo.pF5228C = &pF5228C;
	LinkedInfo.pF5228c = &pF5228c;

	LinkedInfo.pF5229T = &pF5229T;
	LinkedInfo.pF5229A = &pF5229A;
	LinkedInfo.pF5229B = &pF5229B;
	LinkedInfo.pF5229C = &pF5229C;
	LinkedInfo.pF5229c = &pF5229c;

	LinkedInfo.pF5230T = &pF5230T;
	LinkedInfo.pF5230A = &pF5230A;
	LinkedInfo.pF5230B = &pF5230B;
	LinkedInfo.pF5230C = &pF5230C;
	LinkedInfo.pF5230c = &pF5230c;

	LinkedInfo.pF5231T = &pF5231T;
	LinkedInfo.pF5231A = &pF5231A;
	LinkedInfo.pF5231B = &pF5231B;
	LinkedInfo.pF5231C = &pF5231C;
	LinkedInfo.pF5231c = &pF5231c;

	LinkedInfo.pF5232T = &pF5232T;
	LinkedInfo.pF5232A = &pF5232A;
	LinkedInfo.pF5232B = &pF5232B;
	LinkedInfo.pF5232C = &pF5232C;
	LinkedInfo.pF5232c = &pF5232c;

	LinkedInfo.pF5233T = &pF5233T;
	LinkedInfo.pF5233A = &pF5233A;
	LinkedInfo.pF5233B = &pF5233B;
	LinkedInfo.pF5233C = &pF5233C;
	LinkedInfo.pF5233c = &pF5233c;

	LinkedInfo.pF5234T = &pF5234T;
	LinkedInfo.pF5234A = &pF5234A;
	LinkedInfo.pF5234B = &pF5234B;
	LinkedInfo.pF5234C = &pF5234C;
	LinkedInfo.pF5234c = &pF5234c;

	LinkedInfo.pF5235T = &pF5235T;
	LinkedInfo.pF5235A = &pF5235A;
	LinkedInfo.pF5235B = &pF5235B;
	LinkedInfo.pF5235C = &pF5235C;
	LinkedInfo.pF5235c = &pF5235c;

	LinkedInfo.pF5236T = &pF5236T;
	LinkedInfo.pF5236A = &pF5236A;
	LinkedInfo.pF5236B = &pF5236B;
	LinkedInfo.pF5236C = &pF5236C;
	LinkedInfo.pF5236c = &pF5236c;

	LinkedInfo.pF5237T = &pF5237T;
	LinkedInfo.pF5237A = &pF5237A;
	LinkedInfo.pF5237B = &pF5237B;
	LinkedInfo.pF5237C = &pF5237C;

	m_pShinFhyo52->GetLinkedData( &LinkedInfo );

	return 0;
}

//-----------------------------------------------------------------------------
// データの読込み
//-----------------------------------------------------------------------------
// 引数	upck	：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFhyo522EX::ReadData( int upck )
{
#ifdef _20140118_CUT_
	// 原則計算を適用する場合の控除対象仕入税額
	memmove( F5218T, m_pSnFhyo52Data->Sn_5FSZTG, MONY_BUF_SIZE );
	memmove( F5218A, m_pSnFhyo52Data->Sn_5FSZKGA, MONY_BUF_SIZE );
	memmove( F5218B, m_pSnFhyo52Data->Sn_5FSZKGB, MONY_BUF_SIZE );
	memmove( F5218C, m_pSnFhyo52Data->Sn_5FSZKGC, MONY_BUF_SIZE );

	// 特例計算を適用する場合の控除対象仕入税額
//	if( m_pSnFhyo52Data->Sn_5FSZsw ){
	if( 0 ){
		memmove( F5219T, m_pSnFhyo52Data->Sn_5FSZTT, MONY_BUF_SIZE );
	}
	else{
		memset( F5219T, '\0', MONY_BUF_SIZE );
	}
//	if( m_pSnFhyo52Data->Sn_5FSAsw ){
	if( 0 ){
		memmove( F5219A, m_pSnFhyo52Data->Sn_5FSZKTA, MONY_BUF_SIZE );
	}
	else{
		memset( F5219A, '\0', MONY_BUF_SIZE );
	}
//	if( m_pSnFhyo52Data->Sn_5FSBsw ){
	if( 0 ){
		memmove( F5219B, m_pSnFhyo52Data->Sn_5FSZKTB, MONY_BUF_SIZE );
	}
	else{
		memset( F5219B, '\0', MONY_BUF_SIZE );
	}
//	if( m_pSnFhyo52Data->Sn_5FSCsw ){
	if( 0 ){
		memmove( F5219C, m_pSnFhyo52Data->Sn_5FSZKTC, MONY_BUF_SIZE );
	}
	else{
		memset( F5219C, '\0', MONY_BUF_SIZE );
	}
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// データの書込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo522EX::WriteData()
{
#ifdef _20140118_CUT_
	// 原則計算を適用する場合の控除対象仕入税額
	memmove( m_pSnFhyo52Data->F5218T, MONY_BUF_SIZE );
	memmove( m_pSnFhyo52Data->Sn_5FSZKGA, F5218A, MONY_BUF_SIZE );
	memmove( m_pSnFhyo52Data->Sn_5FSZKGB, F5218B, MONY_BUF_SIZE );
	memmove( m_pSnFhyo52Data->Sn_5FSZKGC, F5218C, MONY_BUF_SIZE );

	// 特例計算を適用する場合の控除対象仕入税額
	memmove( m_pSnFhyo52Data->Sn_5FSZTT, F5219T, MONY_BUF_SIZE );
	memmove( m_pSnFhyo52Data->Sn_5FSZKTA, F5219A, MONY_BUF_SIZE );
	memmove( m_pSnFhyo52Data->Sn_5FSZKTB, F5219B, MONY_BUF_SIZE );
	memmove( m_pSnFhyo52Data->Sn_5FSZKTC, F5219C, MONY_BUF_SIZE );

	// １種類の事業の専業者の場合
//	memset( m_pSnFhyo40Data->Sn_1F4A, '\0', MONY_BUF_SIZE );
//	memset( m_pSnFhyo40Data->Sn_1F4B, '\0', MONY_BUF_SIZE );
//	memset( m_pSnFhyo40Data->Sn_1F4C, '\0', MONY_BUF_SIZE );

	if( (*m_pCQsw) & 0x0f ){
//		memmove( m_pSnFhyo40Data->Sn_1F8B, F525B, MONY_BUF_SIZE );
//		memmove( m_pSnFhyo40Data->Sn_1F8C, F525T, MONY_BUF_SIZE );
//		m_pArith->l_sub( pSyzShin->Snd.Sn_1F8A, F525T, F525B );
	}
	else {
		// 3%分
		if( m_pArith->l_cmp(F5218A, F5219A) > 0 ){
			memmove( m_pSnFhyo40Data->Sn_1F4A, F5218A, MONY_BUF_SIZE );
		}
		else{
			memmove( m_pSnFhyo40Data->Sn_1F4A, F5219A, MONY_BUF_SIZE );
		}
		// 4%分
		if( m_pArith->l_cmp(F5218B, F5219B) > 0 ){
			memmove( m_pSnFhyo40Data->Sn_1F4B, F5218B, MONY_BUF_SIZE );
		}
		else{
			memmove( m_pSnFhyo40Data->Sn_1F4B, F5219B, MONY_BUF_SIZE );
		}
		// 6.3%分
		if( m_pArith->l_cmp(F5218C, F5219C) > 0 ){
			memmove( m_pSnFhyo40Data->Sn_1F4C, F5218C, MONY_BUF_SIZE );
		}
		else{
			memmove( m_pSnFhyo40Data->Sn_1F4C, F5219C, MONY_BUF_SIZE );
		}

// 必要？？？
//		if( !pSyzShin->Snh.Sn_TOKUR ){
//			memmove( pSyzShin->Snd.Sn_1F8C, F5218T, MONY_BUF_SIZE );
//		}
//		else{
//			memmove( pSyzShin->Snd.Sn_1F8C, F5219T, MONY_BUF_SIZE );
//		}
	}
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo522EX::Init()
{
//--> '14.05.16 INS START
	if( ((*m_pSnFhyo52Data)==NULL) || ((*m_pSnFhyo40Data)==NULL) || ((*m_pSnHonpyoData)==NULL) ){
		return -1;
	}
//<-- '14.05.16 INS END

	ReadData( 1 );
	m_FrSw = 1;

/*	m_Tmp = 0;
	f52calq_1();				// 課税標準額消費税額
	f52calq_2();				// 貸倒回収消費税額
	f52calq_3();				// 売上対価の返還等消費税額
	f52calq_4();				// （１）＋（２）－（３）
	f52calq_5();*/
	if( *m_ptypeCnt > 1 ){
//		f52calq_ku();	// 事業別課税売上高の計算
//		f52calq_gks();		// 課税売上に係る税額計
	}
//	else{
//		m_Tmp = 1;
//	}

	// 画面表示
	DispFh522();

	InitDiagAttr();

	return 0;
}

//-----------------------------------------------------------------------------
// 画面出力
//-----------------------------------------------------------------------------
void CShinFhyo522EX::DispFh522()
{
//	f52dsp_tk();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		DispFh522_Unconnect();
	}
	else{
		DispFh522_Unconnect();
	}
}

//-----------------------------------------------------------------------------
// 画面出力（非連動）
//-----------------------------------------------------------------------------
void CShinFhyo522EX::DispFh522_Unconnect()
{
	if( (*m_ptypeCnt) < 2 ){
		*m_pSzsw = 0;
	}
#ifdef _DEBUG
	char	moji[128] = {0};
	CArithEx	Arith(0x16);

	Arith.l_print( moji, pF5222A, _T("sss,sss,sss,sss,ss9") );
	Arith.l_print( moji, pF5222B, _T("sss,sss,sss,sss,ss9") );
	Arith.l_print( moji, pF5222C, _T("sss,sss,sss,sss,ss9") );
	Arith.l_print( moji, pF5222T, _T("sss,sss,sss,sss,ss9") );
#endif

	// イ 原則計算
	dsp_prs( IDC_ICSDIAGCTRL1, ID5220A, pF5220A );
	dsp_prs( IDC_ICSDIAGCTRL1, ID5220B, pF5220B );
	dsp_prs( IDC_ICSDIAGCTRL1, ID5220C, pF5220C );
	dsp_prs( IDC_ICSDIAGCTRL1, ID5220T, pF5220T );

	// ロ-イ 特例計算
	for( int i=ID5221A; i<=ID5221T; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL1, i );
	}
/*- '14.06.11 -*/
//	if( (*m_ptkSw)&0xff ){
//		dsp_prs( IDC_ICSDIAGCTRL1, ID5219A, pF5219A );
//		dsp_prs( IDC_ICSDIAGCTRL1, ID5219B, pF5219B );
//		dsp_prs( IDC_ICSDIAGCTRL1, ID5219C, pF5219C );
//		dsp_prs( IDC_ICSDIAGCTRL1, ID5219T, pF5219T );
//	}
/*-------------*/
	if( (*m_pdspSkjSw)&0x02 ){
		dsp_prs( IDC_ICSDIAGCTRL1, ID5221A, pF5221A );
		dsp_prs( IDC_ICSDIAGCTRL1, ID5221B, pF5221B );
		dsp_prs( IDC_ICSDIAGCTRL1, ID5221C, pF5221C );
		dsp_prs( IDC_ICSDIAGCTRL1, ID5221T, pF5221T );
	}
/*-------------*/

	// ロ-ロ 特例計算
	for( int i=ID5222A; i<=ID5237T; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL1, i );
	}
/*- '14.06.11 -*/
/*	if( (*m_ptkSw)&0xffff00 ){
//		if( (*m_ptkSw)&0x100 ){
		if( *m_pSzsw == 2 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5220A, pF5220A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5220B, pF5220B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5220C, pF5220C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5220T, pF5220T );
		}
//		if( (*m_ptkSw)&0x200 ){
		else if( *m_pSzsw == 3 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5221A, pF5221A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5221B, pF5221B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5221C, pF5221C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5221T, pF5221T );
		}
//		if( (*m_ptkSw)&0x400 ){
		else if( *m_pSzsw == 4 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5222A, pF5222A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5222B, pF5222B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5222C, pF5222C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5222T, pF5222T );
		}
//		if( (*m_ptkSw)&0x800 ){
		else if( *m_pSzsw == 5 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5223A, pF5223A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5223B, pF5223B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5223C, pF5223C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5223T, pF5223T );
		}
//		if( (*m_ptkSw)&0x1000 ){
		else if( *m_pSzsw == 6 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5224A, pF5224A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5224B, pF5224B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5224C, pF5224C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5224T, pF5224T );
		}
//		if( (*m_ptkSw)&0x2000 ){
		else if( *m_pSzsw == 7 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5225A, pF5225A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5225B, pF5225B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5225C, pF5225C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5225T, pF5225T );
		}
//		if( (*m_ptkSw)&0x4000 ){
		else if( *m_pSzsw == 8 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5226A, pF5226A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5226B, pF5226B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5226C, pF5226C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5226T, pF5226T );
		}
//		if( (*m_ptkSw)&0x8000 ){
		else if( *m_pSzsw == 9 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5227A, pF5227A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5227B, pF5227B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5227C, pF5227C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5227T, pF5227T );
		}
//		if( (*m_ptkSw)&0x10000 ){
		else if( *m_pSzsw == 10 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5228A, pF5228A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5228B, pF5228B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5228C, pF5228C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5228T, pF5228T );
		}
//		if( (*m_ptkSw)&0x20000 ){
		else if( *m_pSzsw == 11 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5229A, pF5229A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5229B, pF5229B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5229C, pF5229C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5229T, pF5229T );
		}
	}
	else{
		// 当てはまらないフィールドは入力不可？
	}*/
/*-------------*/
	if( (*m_pdspSkjSw)&~0x03 ){
		if( (*m_pdspSkjSw)&0x04 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5222A, pF5222A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5222B, pF5222B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5222C, pF5222C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5222T, pF5222T );
		}
		else if( (*m_pdspSkjSw)&0x08 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5223A, pF5223A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5223B, pF5223B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5223C, pF5223C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5223T, pF5223T );
		}
		else if( (*m_pdspSkjSw)&0x10 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5224A, pF5224A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5224B, pF5224B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5224C, pF5224C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5224T, pF5224T );
		}
		else if( (*m_pdspSkjSw)&0x20 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5225A, pF5225A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5225B, pF5225B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5225C, pF5225C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5225T, pF5225T );
		}
		else if( (*m_pdspSkjSw)&0x40 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5226A, pF5226A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5226B, pF5226B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5226C, pF5226C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5226T, pF5226T );
		}
		else if( (*m_pdspSkjSw)&0x80 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5227A, pF5227A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5227B, pF5227B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5227C, pF5227C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5227T, pF5227T );
		}
		else if( (*m_pdspSkjSw)&0x100 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5228A, pF5228A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5228B, pF5228B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5228C, pF5228C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5228T, pF5228T );
		}
		else if( (*m_pdspSkjSw)&0x200 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5229A, pF5229A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5229B, pF5229B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5229C, pF5229C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5229T, pF5229T );
		}
		else if( (*m_pdspSkjSw)&0x400 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230A, pF5230A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230B, pF5230B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230C, pF5230C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230T, pF5230T );
		}
		else if( (*m_pdspSkjSw)&0x800 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5231A, pF5231A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5231B, pF5231B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5231C, pF5231C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5231T, pF5231T );
		}

		else if( (*m_pdspSkjSw)&0x1000 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5232A, pF5232A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5232B, pF5232B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5232C, pF5232C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5232T, pF5232T );
		}

		else if( (*m_pdspSkjSw)&0x2000 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5233A, pF5233A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5233B, pF5233B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5233C, pF5233C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5233T, pF5233T );
		}

		else if( (*m_pdspSkjSw)&0x4000 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5234A, pF5234A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5234B, pF5234B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5234C, pF5234C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5234T, pF5234T );
		}

		else if( (*m_pdspSkjSw)&0x8000 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5235A, pF5235A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5235B, pF5235B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5235C, pF5235C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5235T, pF5235T );
		}

		else if( (*m_pdspSkjSw)&0x10000 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5236A, pF5236A );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5236B, pF5236B );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5236C, pF5236C );
			dsp_prs( IDC_ICSDIAGCTRL1, ID5236T, pF5236T );
		}
	}
	else{
		// 当てはまらないフィールドは入力不可？
	}
/*-------------*/

	// 判定
	f52calq_sel();
}


//-----------------------------------------------------------------------------
// 控除対象仕入税額の計算式区分の表示
//-----------------------------------------------------------------------------
void CShinFhyo522EX::f52dsp_tk()
{
	return;

	/*

	dsp_cls( IDC_ICSDIAGCTRL1, ID5219T );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5219B );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5220T );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5220B );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5221T );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5221B );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5222T );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5222B );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5223T );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5223B );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5224T );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5224B );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5225T );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5225B );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5226T );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5226B );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5227T );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5227B );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5228T );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5228B );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5229T );
	dsp_cls( IDC_ICSDIAGCTRL1, ID5229B );

	if( !((*m_pCQsw)&0x0f) ){

		switch( *m_pSzsw ){
			case 1 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5219T, pF5219T );
				break;
			case 2 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5220T, pF5219T );
				break;
			case 3 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5221T, pF5219T );
				break;
			case 4 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5222T, pF5219T );
				break;
			case 5 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5223T, pF5219T );
				break;
			case 6 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5224T, pF5219T );
				break;
			case 7 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5225T, pF5219T );
				break;
			case 8 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5226T, pF5219T );
				break;
			case 9 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5227T, pF5219T );
				break;
			case 10:
				dsp_prs( IDC_ICSDIAGCTRL1, ID5228T, pF5219T );
				break;
			case 11:
				dsp_prs( IDC_ICSDIAGCTRL1, ID5229T, pF5219T );
				break;
			default:
				break;
		}

		if( m_pArith->l_cmp(pF5218T, pF5219T) > 0 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230T, pF5218T );
		}
		else{
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230T, pF5219T );
		}

		switch( *m_pSBsw ){
			case 1 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5219B, pF5219B );
				break;
			case 2 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5220B, pF5219B );
				break;
			case 3 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5221B, pF5219B );
				break;
			case 4 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5222B, pF5219B );
				break;
			case 5 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5223B, pF5219B );
				break;
			case 6 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5224B, pF5219B );
				break;
			case 7 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5225B, pF5219B );
				break;
			case 8 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5226B, pF5219B );
				break;
			case 9 :
				dsp_prs( IDC_ICSDIAGCTRL1, ID5227B, pF5219B );
				break;
			case 10:
				dsp_prs( IDC_ICSDIAGCTRL1, ID5228B, pF5219B );
						break;
			case 11:
				dsp_prs( IDC_ICSDIAGCTRL1, ID5229B, pF5219B );
				break;
			default:
				break;
		}
		if( m_pArith->l_cmp( pF5218B, pF5219B ) > 0 ){
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230B, pF5218B );
		}
		else{
			dsp_prs( IDC_ICSDIAGCTRL1, ID5230B, pF5219B );
		}
	}

	*/
}

//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	
//		Index	：	
//		Mony	：	
//-----------------------------------------------------------------------------
void CShinFhyo522EX::dsp_prs( unsigned short Id, short Index, char *Mony )
{

	if( !m_DpSw ){	// 再集計のみ（表示無し）
	
		DIAGRAM_DATA		DiagData;
		DiagData.data_imgdata = NULL;

		memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
		diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}
}

//-----------------------------------------------------------------------------
// 金額表示クリア
//-----------------------------------------------------------------------------
// 引数	Id		：	
//		Index	：	
//-----------------------------------------------------------------------------
void CShinFhyo522EX::dsp_cls( unsigned short Id, short Index )
{
	if( !m_DpSw ){	// 再集計のみ（表示無し）
	
		DIAGRAM_DATA		DiagData;
		DiagData.data_imgdata = NULL;

		memset( DiagData.data_val, '\0', MONY_BUF_SIZE );
		diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFhyo522EX::InitDiagAttr()
{
	int	cnt = 0;
	while( 1 ){
		if( Fhyo522[cnt].Index == 0 ){
			break;
		}
		
		int	Flg;
		
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			// 確定時は全入力不可
			Flg = Fhyo522[cnt].AllUnConnect;
		}
		else if( *m_ptypeCnt < 2 ){
			// 単一業種の時には入力不可
			Flg = Fhyo522[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				if( IsProvideEditCell(Fhyo522[cnt].Index) ){
					Flg = 0;
				}
				else{
					Flg = Fhyo522[cnt].UnConnect;
				}
			}
			else{
				Flg = Fhyo522[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL1, Fhyo522[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Fhyo522[cnt].Connect );
		cnt++;
	}
	m_Fhyo522Diag01.EnableDelete( 0 );

	// 手入力サインに合わせた色変更
	if( (*m_pSnHeadData)->IsInUnconnectInput(m_pZmSub) == FALSE ){
		return 0;
	}
	else if( *m_ptypeCnt < 2 ){
		return 0;
	}

//--> '14.10.17 INS START
	BOOL	isAddtype = TRUE;
	if( m_pShinFhyo52->judge_skj_calq() == 1 ){
		isAddtype = FALSE;
	}
//--> '14.10.17 INS END

	int	chkFlg = 0x08;
//-- '14.10.17 --
//	for( int i=0; i<3; i++, chkFlg<<=4 ){
//---------------
	for( int i=0; i<4; i++, chkFlg<<=4 ){
//---------------
/*- '14.06.28 -*/
//		if( (*pF5218c)&chkFlg ){
//			ChangeColor( IDC_ICSDIAGCTRL1, ID5218A+i, 3 );
//		}
//		else{
//			ChangeColor( IDC_ICSDIAGCTRL1, ID5218A+i, 0 );
//		}
/*-------------*/
//--- '14.10.17 ---
//		if( (*m_pdspSkjSw)&0x01 ){
//			if( (*pF5218c)&chkFlg ){
//				ChangeColor( IDC_ICSDIAGCTRL1, ID5218A+i, 3 );
//			}
//			else{
//				ChangeColor( IDC_ICSDIAGCTRL1, ID5218A+i, 0 );
//			}
//		}
//		else{
//			ChangeColor( IDC_ICSDIAGCTRL1, ID5218A+i, 1 );
//		}
//-----------------
		if( isAddtype && (i==3) ){
			ChangeColor( IDC_ICSDIAGCTRL1, ID5220A+i, 1 );
		}
		else{
			if( (*m_pdspSkjSw)&0x01 ){
				if( (*pF5220c)&chkFlg ){
					ChangeColor( IDC_ICSDIAGCTRL1, ID5220A+i, 3 );
				}
				else{
					ChangeColor( IDC_ICSDIAGCTRL1, ID5220A+i, 0 );
				}
			}
			else{
				ChangeColor( IDC_ICSDIAGCTRL1, ID5220A+i, 1 );
			}
		}
//-----------------
/*-------------*/

//--- '14.10.17 ---
///*- '14.06.11 -*/
////		if( (*m_ptkSw)&0xff ){
///*-------------*/
//		if( (*m_pdspSkjSw)&0x02 ){
///*-------------*/
//			if( (*pF5219c)&chkFlg ){
//				ChangeColor( IDC_ICSDIAGCTRL1, ID5219A+i, 3 );
//			}
//			else{
//				ChangeColor( IDC_ICSDIAGCTRL1, ID5219A+i, 0 );
//			}
//		}
//		else{
//			ChangeColor( IDC_ICSDIAGCTRL1, ID5219A+i, 1 );
//		}
//-----------------
		if( isAddtype && (i==3) ){
			ChangeColor( IDC_ICSDIAGCTRL1, ID5221A+i, 1 );
		}
		else{
			if( (*m_pdspSkjSw)&0x02 ){
				if( (*pF5221c)&chkFlg ){
					ChangeColor( IDC_ICSDIAGCTRL1, ID5221A+i, 3 );
				}
				else{
					ChangeColor( IDC_ICSDIAGCTRL1, ID5221A+i, 0 );
				}
			}
			else{
				ChangeColor( IDC_ICSDIAGCTRL1, ID5221A+i, 1 );
			}
		}
//-----------------

	}

/*- '14.06.11 -*/
//	if( (*m_ptkSw)&0xffff00 ){//特例計算の場合、手入力サインがたっていれば色変更する
/*-------------*/
	if( (*m_pdspSkjSw)&~0x03 ){//特例計算の場合、手入力サインがたっていれば色変更する
/*-------------*/
		chkFlg = 0x08;

//--> '14.10.17 INS START
		int clqmax = 3;
		if( isAddtype == FALSE ){
			clqmax = 4;
		}
//<-- '14.10.17 INS END
//-- '14.10.17 --
//		for( int i=0; i<3; i++, chkFlg<<=4 ){
//---------------
		for( int i=0; i<clqmax; i++, chkFlg<<=4 ){//ここのchkFlg<<=4のままでいい？？
//---------------
			if( (*pF5222c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5222A+i, 3 );
			}
			if( (*pF5223c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5223A+i, 3 );
			}
			if( (*pF5224c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5224A+i, 3 );
			}
			if( (*pF5225c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5225A+i, 3 );
			}
			if( (*pF5226c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5226A+i, 3 );
			}
			if( (*pF5227c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5227A+i, 3 );
			}
			if( (*pF5228c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5228A+i, 3 );
			}
			if( (*pF5229c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5229A+i, 3 );
			}
			if( (*pF5230c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5230A+i, 3 );
			}
			if( (*pF5231c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5231A+i, 3 );
			}
			if( (*pF5232c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5232A+i, 3 );
			}
			if( (*pF5233c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5233A+i, 3 );
			}
			if( (*pF5234c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5234A+i, 3 );
			}
			if( (*pF5235c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5235A+i, 3 );
			}
			if( (*pF5236c)&chkFlg ){
				ChangeColor( IDC_ICSDIAGCTRL1, ID5236A+i, 3 );
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 複数事業で強制入力不可行？
//-----------------------------------------------------------------------------
// 引数	Idx	：	チェックインデックス
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CShinFhyo522EX::IsProvideEditCell( short Idx )
{
	int	rt = 0;

//--> '14.10.17 INS START
	BOOL	isAddtype = TRUE;
	if( m_pShinFhyo52->judge_skj_calq() == 1 ){
		isAddtype = FALSE;
	}
//<-- '14.10.17 INS END

/*- '14.06.27 -*/
//	if( (*m_ptkSw)&0xff ){
//		if( (ID5220A<=Idx) && (Idx<=ID5229T) ){
//			rt = 1;
//		}
//	}
//	else{
//		if( (ID5219A<=Idx) && (Idx<=ID5219T) ){
//			rt = 1;
//		}
//
//		if( (ID5220A<=Idx) && (Idx<=ID5229T) ){
//			rt = 1;
//		}
//	}
/*-------------*/
	rt = 1;
	if( (*m_pdspSkjSw)&0x01 ){
//-- '14.10.17 --
//		if( (ID5218A<=Idx) && (Idx<=ID5218T) ){
//			rt = 0;
//		}
//---------------
		if( (ID5220A<=Idx) && (Idx<=ID5220C) ){
			rt = 0;
		}
		else if( (Idx==ID5220T) && !isAddtype ){
			rt = 0;
		}
//---------------
	}
/*-------------*/

/*- '14.06.11 -*/
//	if( (*m_ptkSw)&0xffff00 ){
////		if( ((*m_ptkSw)&0x000100) ){
//		if( *m_pSzsw == 2 ){
//			if( (ID5220A<=Idx) && (Idx<=ID5220T) ){
//				rt = 0;
//			}
//		}
////		if( ((*m_ptkSw)&0x000200) ){
//		else if( *m_pSzsw == 3 ){
//			if( (ID5221A<=Idx) && (Idx<=ID5221T) ){
//				rt = 0;
//			}
//		}
////		if( ((*m_ptkSw)&0x000400) ){
//		else if( *m_pSzsw == 4 ){
//			if( (ID5222A<=Idx) && (Idx<=ID5222T) ){
//				rt = 0;
//			}
//		}
////		if( ((*m_ptkSw)&0x000800) ){
//		else if( *m_pSzsw == 5 ){
//			if( (ID5223A<=Idx) && (Idx<=ID5223T) ){
//				rt = 0;
//			}
//		}
////		if( ((*m_ptkSw)&0x001000) ){
//		else if( *m_pSzsw == 6 ){
//			if( (ID5224A<=Idx) && (Idx<=ID5224T) ){
//				rt = 0;
//			}
//		}
////		if( ((*m_ptkSw)&0x002000) ){
//		else if( *m_pSzsw == 7 ){
//			if( (ID5225A<=Idx) && (Idx<=ID5225T) ){
//				rt = 0;
//			}
//		}
////		if( ((*m_ptkSw)&0x004000) ){
//		else if( *m_pSzsw == 8 ){
//			if( (ID5226A<=Idx) && (Idx<=ID5226T) ){
//				rt = 0;
//			}
//		}
////		if( ((*m_ptkSw)&0x008000) ){
//		else if( *m_pSzsw == 9 ){
//			if( (ID5227A<=Idx) && (Idx<=ID5227T) ){
//				rt = 0;
//			}
//		}
////		if( ((*m_ptkSw)&0x010000) ){
//		else if( *m_pSzsw == 10 ){
//			if( (ID5228A<=Idx) && (Idx<=ID5228T) ){
//				rt = 0;
//			}
//		}
////		if( ((*m_ptkSw)&0x020000) ){
//		else if( *m_pSzsw == 11 ){
//			if( (ID5229A<=Idx) && (Idx<=ID5229T) ){
//				rt = 0;
//			}
//		}
//	}
/*-------------*/
	if( (*m_pdspSkjSw)&~0x01 ){
		if( (*m_pdspSkjSw)&0x02 ){
			if( (ID5221A<=Idx) && (Idx<=ID5221C) ){
				rt = 0;
			}
			else if( (Idx==ID5221T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x04 ){
			if( (ID5222A<=Idx) && (Idx<=ID5222C) ){
				rt = 0;
			}
			else if( (Idx==ID5222T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x08 ){
			if( (ID5223A<=Idx) && (Idx<=ID5223C) ){
				rt = 0;
			}
			else if( (Idx==ID5223T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x10 ){
			if( (ID5224A<=Idx) && (Idx<=ID5224C) ){
				rt = 0;
			}
			else if( (Idx==ID5224T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x20 ){
			if( (ID5225A<=Idx) && (Idx<=ID5225C) ){
				rt = 0;
			}
			else if( (Idx==ID5225T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x40 ){
			if( (ID5226A<=Idx) && (Idx<=ID5226C) ){
				rt = 0;
			}
			else if( (Idx==ID5226T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x80 ){
			if( (ID5227A<=Idx) && (Idx<=ID5227C) ){
				rt = 0;
			}
			else if( (Idx==ID5227T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x100 ){
			if( (ID5228A<=Idx) && (Idx<=ID5228C) ){
				rt = 0;
			}
			else if( (Idx==ID5228T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x200 ){
			if( (ID5229A<=Idx) && (Idx<=ID5229C) ){
				rt = 0;
			}
			else if( (Idx==ID5229T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x400 ){
			if( (ID5230A<=Idx) && (Idx<=ID5230C) ){
				rt = 0;
			}
			else if( (Idx==ID5230T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x800 ){
			if( (ID5231A<=Idx) && (Idx<=ID5231C) ){
				rt = 0;
			}
			else if( (Idx==ID5231T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x1000 ){
			if( (ID5232A<=Idx) && (Idx<=ID5232C) ){
				rt = 0;
			}
			else if( (Idx==ID5232T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x2000 ){
			if( (ID5233A<=Idx) && (Idx<=ID5233C) ){
				rt = 0;
			}
			else if( (Idx==ID5233T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x4000 ){
			if( (ID5234A<=Idx) && (Idx<=ID5234C) ){
				rt = 0;
			}
			else if( (Idx==ID5234T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x8000 ){
			if( (ID5235A<=Idx) && (Idx<=ID5235C) ){
				rt = 0;
			}
			else if( (Idx==ID5235T) && !isAddtype ){
				rt = 0;
			}
		}
		else if( (*m_pdspSkjSw)&0x10000 ){
			if( (ID5236A<=Idx) && (Idx<=ID5236C) ){
				rt = 0;
			}
			else if( (Idx==ID5236T) && !isAddtype ){
				rt = 0;
			}
		}
	}
/*-------------*/

	return rt;
}

BEGIN_EVENTSINK_MAP(CShinFhyo522EX, CSyzBaseDlg)
	ON_EVENT(CShinFhyo522EX, IDC_ICSDIAGCTRL1, 1, CShinFhyo522EX::EditONIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinFhyo522EX, IDC_ICSDIAGCTRL1, 2, CShinFhyo522EX::EditOFFIcsdiagctrl1, VTS_I2)
	ON_EVENT(CShinFhyo522EX, IDC_ICSDIAGCTRL1, 3, CShinFhyo522EX::TerminationIcsdiagctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo522EX::EditONIcsdiagctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//-->Terminationへ移動 harino ('14.03.07)
	//RECT	rectA, rectB;
	//GetWindowRect( &rectA );
	//diag_getrect( IDC_ICSDIAGCTRL1, index, &rectB, CIcsdiagctrl);
	//if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
	//	int	cpos = GetScrollPos( SB_VERT );
	//	SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	//}
	//<--Terminationへ移動 harino ('14.03.07)
	m_curwnd = IDC_ICSDIAGCTRL1;
}

void CShinFhyo522EX::EditOFFIcsdiagctrl1( short index )
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// [03'07.15]
	diag_getdata( IDC_ICSDIAGCTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	char	*ptmpMony = NULL;
	char	CkMy[MONY_BUF_SIZE]={0}, CkVl[MONY_BUF_SIZE]={0};

	switch( index ){
/*		case ID5218T :	// 18. 原則計算を適用する場合
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memmove( CkMy, F5218T, MONY_BUF_SIZE );
				diag_getdata( IDC_ICSDIAGCTRL1, ID5218T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( F5218T, CkMy ) )	{
					F5218c |= 0x01;
					ChangeColor( IDC_ICSDIAGCTRL1, ID5218T, 3 );			//2 );
					memmove( F5218T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				}
				else	memmove( F5218T, CkMy, MONY_BUF_SIZE );
				// 各入力の計表示
				f52dsp_tk();
				CQsw |= 0x10;
				break;*/

		case ID5220A :	// 20. 原則計算を適用する場合
		case ID5220B :
		case ID5220C :
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
//--> '14.06.27 INS START
			if( m_IsDeleteKeyIn ){
				break;
			}
//<-- '14.06.27 INS END

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(ptmpMony, CkMy) ){
				// 手入力サインON
				TrunOnInputSign( index );

				ChangeColor( IDC_ICSDIAGCTRL1, index, 3 );

				memmove( ptmpMony, CkMy, MONY_BUF_SIZE );

//-- '14.10.17 --
//				// 横計
//				f52calq_sum1( index );
//---------------
				// 横計
				if( m_pShinFhyo52->judge_skj_calq() == 0 ){
					f52calq_sum1( index );
				}
//---------------
				// 最も有利な方式を選択
				f52calq_sel();
				(*m_pCQsw) |= 0x10;
			}

			break;

//--> '14.10.17 INS START
		case ID5220T :	// 20. 特例適用･合計 仕入税額(期間計)
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			if( m_IsDeleteKeyIn ){
				break;
			}

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, ptmpMony, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(ptmpMony, CkMy) ){
				// 手入力サインON
				TrunOnInputSign( index );

				ChangeColor( IDC_ICSDIAGCTRL1, index, 3 );

				memmove( ptmpMony, CkMy, MONY_BUF_SIZE );

				// 最も有利な方式を選択
				f52calq_sel();
//				(*m_pCQsw) |= 0x10;
			}
			break;
//<-- '14.10.17 INS END

/*		case ID5219T :	// 19. 特例適用･合計 仕入税額(期間計)
				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 1 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL1, ID5219T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 1 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}
				break;*/

		case ID5221A :	// 21. 特例適用･合計 仕入税額(4%分)	
		case ID5221B :
		case ID5221C :
			
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
//--> '14.06.27 INS START
			if( m_IsDeleteKeyIn ){
				break;
			}
//<-- '14.06.27 INS END
			memset( CkMy, '\0', MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 

			memset( CkVl, '\0', MONY_BUF_SIZE );
			memmove( CkVl, ptmpMony, MONY_BUF_SIZE );

			if( m_pArith->l_cmp(CkMy, CkVl) ){
//				if( m_pArith->l_test(CkMy) ){
//					*m_pSBsw = 1;
//				}
//				else{
//					*m_pSBsw = 0;
//				}
				// 手入力サインON
				TrunOnInputSign( index );

				ChangeColor( IDC_ICSDIAGCTRL1, index, 3 );

				memmove( ptmpMony, CkMy, MONY_BUF_SIZE );

//-- '14.10.17 --
//				// 横計
//				f52calq_sum2( index );
//---------------
				// 横計
				if( m_pShinFhyo52->judge_skj_calq() == 0 ){
					f52calq_sum2( index );
				}
//---------------
				// 最も有利な方式を選択
				f52calq_sel();
				(*m_pCQsw) |= 0x10;
			}
			break;

//--> '14.10.17 INS START
		case ID5221T :
			
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			if( m_IsDeleteKeyIn ){
				break;
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 

			memset( CkVl, '\0', MONY_BUF_SIZE );
			memmove( CkVl, ptmpMony, MONY_BUF_SIZE );

			if( m_pArith->l_cmp(CkMy, CkVl) ){
				// 手入力サインON
				TrunOnInputSign( index );

				ChangeColor( IDC_ICSDIAGCTRL1, index, 3 );

				memmove( ptmpMony, CkMy, MONY_BUF_SIZE );
				// 最も有利な方式を選択
				f52calq_sel();
				(*m_pCQsw) |= 0x10;
			}
			break;
//<-- '14.10.17 INS END

		case ID5222A :	// 22.
		case ID5222B :
		case ID5222C :

		case ID5223A :	// 23.
		case ID5223B :
		case ID5223C :

		case ID5224A :	// 24.
		case ID5224B :
		case ID5224C :

		case ID5225A :	// 25.
		case ID5225B :
		case ID5225C :

		case ID5226A :	// 26.
		case ID5226B :
		case ID5226C :

		case ID5227A :	// 27.
		case ID5227B :
		case ID5227C :

		case ID5228A :	// 28.
		case ID5228B :
		case ID5228C :

		case ID5229A :	// 29.
		case ID5229B :
		case ID5229C :

		case ID5230A :	// 30.
		case ID5230B :
		case ID5230C :

		case ID5231A :	// 31.
		case ID5231B :
		case ID5231C :

		case ID5232A :	// 32.
		case ID5232B :
		case ID5232C :

		case ID5233A :	// 33.
		case ID5233B :
		case ID5233C :

		case ID5234A :	// 34.
		case ID5234B :
		case ID5234C :

		case ID5235A :	// 35.
		case ID5235B :
		case ID5235C :

		case ID5236A :	// 36.
		case ID5236B :
		case ID5236C :

			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
//--> '14.06.27 INS START
			if( m_IsDeleteKeyIn ){
				break;
			}
//<-- '14.06.27 INS END
			diag_getdata( IDC_ICSDIAGCTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			memset( CkVl, '\0', MONY_BUF_SIZE );
			memmove( CkVl, ptmpMony, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(CkMy, CkVl) ){

				// 手入力サインON
				TrunOnInputSign( index );

				ChangeColor( IDC_ICSDIAGCTRL1, index, 3 );

				memmove( ptmpMony, CkMy, MONY_BUF_SIZE );
//-- '14.10.17 --
//				// 横計
//				f52calq_sum3( index );
//---------------
				// 横計
				if( m_pShinFhyo52->judge_skj_calq() == 0 ){
					f52calq_sum3( index );
				}
//---------------
				// 最も有利な方式を選択
				f52calq_sel();
				(*m_pCQsw) |= 0x10;

			}

			break;

//--> '14.10.17 INS START
		case ID5222T :	// 22.
		case ID5223T :	// 23.
		case ID5224T :	// 24.
		case ID5225T :	// 25.
		case ID5226T :	// 26.
		case ID5227T :	// 27.
		case ID5228T :	// 28.
		case ID5229T :	// 29.
		case ID5230T :	// 30.
		case ID5231T :	// 31.
		case ID5232T :	// 32.
		case ID5233T :	// 33.
		case ID5234T :	// 34.
		case ID5235T :	// 35.
		case ID5236T :	// 36.
			if( GetRelInfoByDiagIdx(index, &ptmpMony) ){
				break;
			}
			if( m_IsDeleteKeyIn ){
				break;
			}
			diag_getdata( IDC_ICSDIAGCTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			memset( CkVl, '\0', MONY_BUF_SIZE );
			memmove( CkVl, ptmpMony, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(CkMy, CkVl) ){

				// 手入力サインON
				TrunOnInputSign( index );

				ChangeColor( IDC_ICSDIAGCTRL1, index, 3 );

				memmove( ptmpMony, CkMy, MONY_BUF_SIZE );
				// 横計
//				f52calq_sum3( index );
				// 最も有利な方式を選択
				f52calq_sel();
//				(*m_pCQsw) |= 0x10;
			}

			break;
//<-- '14.10.17 INS END

//		case ID5220T :	// 20.
/*				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( ((*m_pSzsw)&0xff) == 2 )	{
					memmove( CkMy, pF5219T, MONY_BUF_SIZE );
					memmove( CkVl, pF5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL1, ID5220T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( m_pArith->l_cmp(CkMy, CkVl) ){
					(*m_pSzsw) = ( char ) ( l_test( CkMy ) ? 2 : 0 );
					memmove( pF5219T, CkMy, MONY_BUF_SIZE );
					// 入力直後の特例計算サイン
//					Tokurei52();
					// 各入力の計算表示
					f52dsp_tk();
					(*m_pCQsw) |= 0x10;
				}*/
//				break;


//		case ID5221T :	// 21.
/*				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 3 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL1, ID5221T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 3 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}*/
//				break;

//		case ID5222T :	// 22.
/*				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 4 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL1, ID5222T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 4 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}*/
//				break;

//		case ID5223T :	// 23.
/*				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 5 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL1, ID5223T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 5 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}*/
//				break;

//		case ID5224T :	// 24.
/*				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == MONY_BUF_SIZE )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL1, ID5224T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 6 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}*/
//				break;

//		case ID5225T :	// 25.
/*				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 7 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL1, ID5225T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 7 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}*/
//				break;

//		case ID5226T :	// 26.
/*				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 8 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL1, ID5226T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 8 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}*/
//				break;

//		case ID5227T :	// 27.
/*				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 9 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL1, ID5227T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 9 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}*/
//				break;

//		case ID5228T :	// 28.
/*				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 10 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL1, ID5228T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 10 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}*/
//				break;


//		case ID5229T :	// 29.
/*				memset( CkMy, '\0', MONY_BUF_SIZE );
				memset( CkVl, '\0', MONY_BUF_SIZE );
				if( (SZsw&0xff) == 11 )	{
					memmove( CkMy, F5219T, MONY_BUF_SIZE );
					memmove( CkVl, F5219T, MONY_BUF_SIZE );
				}
				diag_getdata( IDC_ICSDIAGCTRL1, ID5229T, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
				if( l_cmp( CkMy, CkVl ) ) {
					SZsw = ( char ) ( l_test( CkMy ) ? 11 : 0 );
					memmove( F5219T, CkMy, MONY_BUF_SIZE );
				// 入力直後の特例計算サイン
					Tokurei52();
				// 各入力の計算表示
					f52dsp_tk();
					CQsw |= 0x10;
				}*/
//				break;

		default :
				break;
	}
}

#ifdef _20141017_EXTEND_
//-----------------------------------------------------------------------------
// 関連項目の手入力サインをONにする
//-----------------------------------------------------------------------------
// 引数	index	：	該当アイテムインデックス
//-----------------------------------------------------------------------------
void CShinFhyo522EX::TrunOnInputSign( short index )
{
	int	*pTmpSw = NULL;
	int	leftMove = 0;
	if( (index==ID5218A) || (index==ID5218B) || (index==ID5218C) ){
		pTmpSw = pF5218c;
		leftMove = index - ID5218A;
	}
	else if( (index==ID5219A) || (index==ID5219B) || (index==ID5219C) ){
		pTmpSw = pF5219c;
		leftMove = index - ID5219A;
	}
	else if( (index==ID5220A) || (index==ID5220B) || (index==ID5220C) ){
		pTmpSw = pF5220c;
		leftMove = index - ID5220A;
	}
	else if( (index==ID5221A) || (index==ID5221B) || (index==ID5221C) ){
		pTmpSw = pF5221c;
		leftMove = index - ID5221A;
	}
	else if( (index==ID5222A) || (index==ID5222B) || (index==ID5222C) ){
		pTmpSw = pF5222c;
		leftMove = index - ID5222A;
	}
	else if( (index==ID5223A) || (index==ID5223B) || (index==ID5223C) ){
		pTmpSw = pF5223c;
		leftMove = index - ID5223A;
	}
	else if( (index==ID5224A) || (index==ID5224B) || (index==ID5224C) ){
		pTmpSw = pF5224c;
		leftMove = index - ID5224A;
	}
	else if( (index==ID5225A) || (index==ID5225B) || (index==ID5225C) ){
		pTmpSw = pF5225c;
		leftMove = index - ID5225A;
	}
	else if( (index==ID5226A) || (index==ID5226B) || (index==ID5226C) ){
		pTmpSw = pF5226c;
		leftMove = index - ID5226A;
	}
	else if( (index==ID5227A) || (index==ID5227B) || (index==ID5227C) ){
		pTmpSw = pF5227c;
		leftMove = index - ID5227A;
	}
	else if( (index==ID5228A) || (index==ID5228B) || (index==ID5228C) ){
		pTmpSw = pF5228c;
		leftMove = index - ID5228A;
	}
	else if( (index==ID5229A) || (index==ID5229B) || (index==ID5229C) ){
		pTmpSw = pF5229c;
		leftMove = index - ID5229A;
	}

	if( pTmpSw ){
		int	onFlg = 0x08 << (leftMove*4);
		(*pTmpSw) |= onFlg;
	}
}
#else
//-----------------------------------------------------------------------------
// 関連項目の手入力サインをONにする
//-----------------------------------------------------------------------------
// 引数	index	：	該当アイテムインデックス
//-----------------------------------------------------------------------------
void CShinFhyo522EX::TrunOnInputSign( short index )
{
	int	*pTmpSw = NULL;
	int	leftMove = 0;
	if( (index==ID5220A) || (index==ID5220B) || (index==ID5220C) || (index==ID5220T) ){
		pTmpSw = pF5220c;
		leftMove = index - ID5220A;
	}
	else if( (index==ID5221A) || (index==ID5221B) || (index==ID5221C) || (index==ID5221T) ){
		pTmpSw = pF5221c;
		leftMove = index - ID5221A;
	}
	else if( (index==ID5222A) || (index==ID5222B) || (index==ID5222C) || (index==ID5222T) ){
		pTmpSw = pF5222c;
		leftMove = index - ID5222A;
	}
	else if( (index==ID5223A) || (index==ID5223B) || (index==ID5223C) || (index==ID5223T) ){
		pTmpSw = pF5223c;
		leftMove = index - ID5223A;
	}
	else if( (index==ID5224A) || (index==ID5224B) || (index==ID5224C) || (index==ID5224T) ){
		pTmpSw = pF5224c;
		leftMove = index - ID5224A;
	}
	else if( (index==ID5225A) || (index==ID5225B) || (index==ID5225C) || (index==ID5225T) ){
		pTmpSw = pF5225c;
		leftMove = index - ID5225A;
	}
	else if( (index==ID5226A) || (index==ID5226B) || (index==ID5226C) || (index==ID5226T) ){
		pTmpSw = pF5226c;
		leftMove = index - ID5226A;
	}
	else if( (index==ID5227A) || (index==ID5227B) || (index==ID5227C) || (index==ID5227T) ){
		pTmpSw = pF5227c;
		leftMove = index - ID5227A;
	}
	else if( (index==ID5228A) || (index==ID5228B) || (index==ID5228C) || (index==ID5228T) ){
		pTmpSw = pF5228c;
		leftMove = index - ID5228A;
	}
	else if( (index==ID5229A) || (index==ID5229B) || (index==ID5229C) || (index==ID5229T) ){
		pTmpSw = pF5229c;
		leftMove = index - ID5229A;
	}
	else if( (index==ID5230A) || (index==ID5230B) || (index==ID5230C) || (index==ID5230T) ){
		pTmpSw = pF5230c;
		leftMove = index - ID5230A;
	}
	else if( (index==ID5231A) || (index==ID5231B) || (index==ID5231C) || (index==ID5231T) ){
		pTmpSw = pF5231c;
		leftMove = index - ID5231A;
	}
	else if( (index==ID5232A) || (index==ID5232B) || (index==ID5232C) || (index==ID5232T) ){
		pTmpSw = pF5232c;
		leftMove = index - ID5232A;
	}
	else if( (index==ID5233A) || (index==ID5233B) || (index==ID5233C) || (index==ID5233T) ){
		pTmpSw = pF5233c;
		leftMove = index - ID5233A;
	}
	else if( (index==ID5234A) || (index==ID5234B) || (index==ID5234C) || (index==ID5234T) ){
		pTmpSw = pF5234c;
		leftMove = index - ID5234A;
	}
	else if( (index==ID5235A) || (index==ID5235B) || (index==ID5235C) || (index==ID5235T) ){
		pTmpSw = pF5235c;
		leftMove = index - ID5235A;
	}
	else if( (index==ID5236A) || (index==ID5236B) || (index==ID5236C) || (index==ID5236T) ){
		pTmpSw = pF5236c;
		leftMove = index - ID5236A;
	}

	if( pTmpSw ){
		int	onFlg = 0x08 << (leftMove*4);//ここはこのままでいいか？？
		(*pTmpSw) |= onFlg;
	}
}
#endif

void CShinFhyo522EX::TerminationIcsdiagctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}
	
	// Shift+TABをTABに、TABをENTERに返還
	short	wTerm;
	char	VK_FLG = 0x00;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );
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
		m_Fhyo522Diag01.SetPosition(pos);
	}
	if( wTerm==VK_DOWN ){
		pos = UpDownFocusMoveCheck(index , 0);
		m_Fhyo522Diag01.SetPosition(pos);
	}


	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
		m_Fhyo522Diag01.SetNextPosition();
	}
	// 前項目へ
	if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		m_Fhyo522Diag01.SetPrevPosition();
	}

	if( wTerm == VK_DELETE ){
/*		if( index == ID5218T ){
			F5218c &= 0xfe;
			Color_Change( IDC_ICSDIAGCTRL2, ID5218T, 0 );
			memset( F5218T, '\0', MONY_BUF_SIZE );
			m_Fhyo52Diag01.SetPosition ( ID5218T );
			// 各入力の計表示
			f52dsp_tk();
			CQsw |= 0x10;
		}*/

//--> '14.06.10 INS START
		int	rIdx=0, cIdx=0;
//<-- '14.06.10 INS END

//--> '14.06. INS END
		m_IsDeleteKeyIn = TRUE;
//<-- '14.06.10 INS END

		if( (ID5220A<=index) && (index<=ID5236T) ){
//-- '14.10.17 --
//			if( (index==ID5218A) || (index==ID5218B) || (index==ID5218C) ){
//---------------
			if( (index==ID5220A) || (index==ID5220B) || (index==ID5220C) || (index==ID5220T) ){
//---------------
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5220A)*4);
				(*pF5220c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
//--> '14.06.10 INS START
				rIdx = 0;
				cIdx = index - ID5220A;
//<-- '14.06.10 INS END
			}
//-- '14.10.17 --
//			else if( (index==ID5219A) || (index==ID5219B) || (index==ID5219C) ){
//---------------
			else if( (index==ID5221A) || (index==ID5221B) || (index==ID5221C) || (index==ID5221T) ){
//---------------
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5221A)*4);
				(*pF5221c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
//--> '14.06.10 INS START
				rIdx = 1;
				cIdx = index - ID5221A;
//<-- '14.06.10 INS END
			}
//-- '14.10.17 --
//			else if( (index==ID5220A) || (index==ID5220B) || (index==ID5220C) ){
//---------------
			else if( (index==ID5222A) || (index==ID5222B) || (index==ID5222C) || (index==ID5222T) ){
//---------------
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5222A)*4);
				(*pF5222c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
//--> '14.06.10 INS START
				rIdx = 2;
				cIdx = index - ID5222A;
//<-- '14.06.10 INS END
			}
//-- '14.10.17 --
//			else if( (index==ID5221A) || (index==ID5221B) || (index==ID5221C) ){
//---------------
			else if( (index==ID5223A) || (index==ID5223B) || (index==ID5223C) || (index==ID5223T) ){
//---------------
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5223A)*4);
				(*pF5223c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
//--> '14.06.10 INS START
				rIdx = 3;
				cIdx = index - ID5223A;
//<-- '14.06.10 INS END
			}
//-- '14.10.17 --
//			else if( (index==ID5222A) || (index==ID5222B) || (index==ID5222C) ){
//---------------
			else if( (index==ID5224A) || (index==ID5224B) || (index==ID5224C) || (index==ID5224T) ){
//---------------
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5224A)*4);
				(*pF5224c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
//--> '14.06.10 INS START
				rIdx = 4;
				cIdx = index - ID5224A;
//<-- '14.06.10 INS END
			}
//-- '14.10.17 --
//			else if( (index==ID5223A) || (index==ID5223B) || (index==ID5223C) ){
//---------------
			else if( (index==ID5225A) || (index==ID5225B) || (index==ID5225C) || (index==ID5225T) ){
//---------------
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5225A)*4);
				(*pF5225c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
//--> '14.06.10 INS START
				rIdx = 5;
				cIdx = index - ID5225A;
//<-- '14.06.10 INS END
			}
//-- '14.10.17 --
//			else if( (index==ID5224A) || (index==ID5224B) || (index==ID5224C) ){
//---------------
			else if( (index==ID5226A) || (index==ID5226B) || (index==ID5226C) || (index==ID5226T) ){
//---------------
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5226A)*4);
				(*pF5226c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
//--> '14.06.10 INS START
				rIdx = 6;
				cIdx = index - ID5226A;
//<-- '14.06.10 INS END
			}
//-- '14.10.17 --
//			else if( (index==ID5225A) || (index==ID5225B) || (index==ID5225C) ){
//---------------
			else if( (index==ID5227A) || (index==ID5227B) || (index==ID5227C) || (index==ID5227T) ){
//---------------
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5227A)*4);
				(*pF5227c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
//--> '14.06.10 INS START
				rIdx = 7;
				cIdx = index - ID5227A;
//<-- '14.06.10 INS END
			}
//-- '14.10.17 --
//			else if( (index==ID5226A) || (index==ID5226B) || (index==ID5226C) ){
//---------------
			else if( (index==ID5228A) || (index==ID5228B) || (index==ID5228C) || (index==ID5228T) ){
//---------------
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5228A)*4);
				(*pF5228c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
//--> '14.06.10 INS START
				rIdx = 8;
				cIdx = index - ID5228A;
//<-- '14.06.10 INS END
			}
//-- '14.10.17 --
//			else if( (index==ID5227A) || (index==ID5227B) || (index==ID5227C) ){
//---------------
			else if( (index==ID5229A) || (index==ID5229B) || (index==ID5229C) || (index==ID5229T) ){
//---------------
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5229A)*4);
				(*pF5229c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
//--> '14.06.10 INS START
				rIdx = 9;
				cIdx = index - ID5229A;
//<-- '14.06.10 INS END
			}
//-- '14.10.17 --
//			else if( (index==ID5228A) || (index==ID5228B) || (index==ID5228C) ){
//---------------
			else if( (index==ID5230A) || (index==ID5230B) || (index==ID5230C) || (index==ID5230T) ){
//---------------
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5230A)*4);
				(*pF5230c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
//--> '14.06.10 INS START
				rIdx = 10;
				cIdx = index - ID5230A;
//<-- '14.06.10 INS END
			}
//-- '14.10.17 --
//			else if( (index==ID5229A) || (index==ID5229B) || (index==ID5229C) ){
//---------------
			else if( (index==ID5231A) || (index==ID5231B) || (index==ID5231C) || (index==ID5231T) ){
//---------------
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5231A)*4);
				(*pF5231c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
//--> '14.06.10 INS START
				rIdx = 11;
				cIdx = index - ID5231A;
//<-- '14.06.10 INS END
			}

			else if( (index==ID5232A) || (index==ID5232B) || (index==ID5232C) || (index==ID5232T) ){
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5232A)*4);
				(*pF5232c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
				rIdx = 12;
				cIdx = index - ID5232A;
			}

			else if( (index==ID5233A) || (index==ID5233B) || (index==ID5233C) || (index==ID5233T) ){
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5233A)*4);
				(*pF5233c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
				rIdx = 13;
				cIdx = index - ID5233A;
			}

			else if( (index==ID5234A) || (index==ID5234B) || (index==ID5234C) || (index==ID5234T) ){
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5234A)*4);
				(*pF5234c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
				rIdx = 14;
				cIdx = index - ID5234A;
			}

			else if( (index==ID5235A) || (index==ID5235B) || (index==ID5235C) || (index==ID5235T) ){
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5235A)*4);
				(*pF5235c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
				rIdx = 15;
				cIdx = index - ID5235A;
			}

			else if( (index==ID5236A) || (index==ID5236B) || (index==ID5236C) || (index==ID5236T) ){
				int	chkflg = 0x08;
				chkflg <<= ((index-ID5236A)*4);
				(*pF5236c) &= ~chkflg;
				ChangeColor( IDC_ICSDIAGCTRL1, index, 0 );
				m_Fhyo522Diag01.SetPosition( index );
				rIdx = 16;
				cIdx = index - ID5236A;
			}

			// 各入力の計算再表示
			if( m_pShinFhyo52 ){
/*- '14.06.09 -*/
//				(*m_pCQsw) &= ~0x10;
//				m_pShinFhyo52->f52calq_all(1);
/*-------------*/
//				m_pShinFhyo52->f52calq_all(1, 0);
				char	val[MONY_BUF_SIZE]={0};
				m_pShinFhyo52->f52calq_one_skj( rIdx, cIdx, val );
				f52calq_sel();
/*-------------*/
			}
			DispFh522_Unconnect();
		}
	}

//--> '14.06.27 INS START
	m_IsDeleteKeyIn = FALSE;
//<-- '14.06.27 INS END

	//-->Editonから移動 harino ('14.03.07)
	int nowpos = m_Fhyo522Diag01.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL1, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
	//-->Editonから移動 harino ('14.03.07)
}

//-----------------------------------------------------------------------------
// ダイアグラムのインデックスから関連項目を取得する
//-----------------------------------------------------------------------------
// 引数	Idx		：	ダイアグラムのインデックス
//		pRtMony	：	該当カラムの金額
//-----------------------------------------------------------------------------
// 返送値	0	：	該当あり
//			-1	：	該当なし
//-----------------------------------------------------------------------------
int CShinFhyo522EX::GetRelInfoByDiagIdx( short Idx, char **pRtMony )
{
	if( Idx == ID5220A ){
		*pRtMony = pF5220A;
	}
	else if( Idx == ID5220B ){
		*pRtMony = pF5220B;
	}
	else if( Idx == ID5220C ){
		*pRtMony = pF5220C;
	}
	else if( Idx == ID5221A ){
		*pRtMony = pF5221A;
	}
	else if( Idx == ID5221B ){
		*pRtMony = pF5221B;
	}
	else if( Idx == ID5221C ){
		*pRtMony = pF5221C;
	}
	else if( Idx == ID5222A ){
		*pRtMony = pF5222A;
	}
	else if( Idx == ID5222B ){
		*pRtMony = pF5222B;
	}
	else if( Idx == ID5222C ){
		*pRtMony = pF5222C;
	}
	else if( Idx == ID5223A ){
		*pRtMony = pF5223A;
	}
	else if( Idx == ID5223B ){
		*pRtMony = pF5223B;
	}
	else if( Idx == ID5223C ){
		*pRtMony = pF5223C;
	}
	else if( Idx == ID5224A ){
		*pRtMony = pF5224A;
	}
	else if( Idx == ID5224B ){
		*pRtMony = pF5224B;
	}
	else if( Idx == ID5224C ){
		*pRtMony = pF5224C;
	}
	else if( Idx == ID5225A ){
		*pRtMony = pF5225A;
	}
	else if( Idx == ID5225B ){
		*pRtMony = pF5225B;
	}
	else if( Idx == ID5225C ){
		*pRtMony = pF5225C;
	}
	else if( Idx == ID5226A ){
		*pRtMony = pF5226A;
	}
	else if( Idx == ID5226B ){
		*pRtMony = pF5226B;
	}
	else if( Idx == ID5226C ){
		*pRtMony = pF5226C;
	}
	else if( Idx == ID5227A ){
		*pRtMony = pF5227A;
	}
	else if( Idx == ID5227B ){
		*pRtMony = pF5227B;
	}
	else if( Idx == ID5227C ){
		*pRtMony = pF5227C;
	}
	else if( Idx == ID5228A ){
		*pRtMony = pF5228A;
	}
	else if( Idx == ID5228B ){
		*pRtMony = pF5228B;
	}
	else if( Idx == ID5228C ){
		*pRtMony = pF5228C;
	}
	else if( Idx == ID5229A ){
		*pRtMony = pF5229A;
	}
	else if( Idx == ID5229B ){
		*pRtMony = pF5229B;
	}
	else if( Idx == ID5229C ){
		*pRtMony = pF5229C;
	}
	else if( Idx == ID5230A ){
		*pRtMony = pF5230A;
	}
	else if( Idx == ID5230B ){
		*pRtMony = pF5230B;
	}
	else if( Idx == ID5230C ){
		*pRtMony = pF5230C;
	}
	else if( Idx == ID5231A ){
		*pRtMony = pF5231A;
	}
	else if( Idx == ID5231B ){
		*pRtMony = pF5231B;
	}
	else if( Idx == ID5231C ){
		*pRtMony = pF5231C;
	}
	else if( Idx == ID5232A ){
		*pRtMony = pF5232A;
	}
	else if( Idx == ID5232B ){
		*pRtMony = pF5232B;
	}
	else if( Idx == ID5232C ){
		*pRtMony = pF5232C;
	}
	else if( Idx == ID5233A ){
		*pRtMony = pF5233A;
	}
	else if( Idx == ID5233B ){
		*pRtMony = pF5233B;
	}
	else if( Idx == ID5233C ){
		*pRtMony = pF5233C;
	}
	else if( Idx == ID5234A ){
		*pRtMony = pF5234A;
	}
	else if( Idx == ID5234B ){
		*pRtMony = pF5234B;
	}
	else if( Idx == ID5234C ){
		*pRtMony = pF5234C;
	}
	else if( Idx == ID5235A ){
		*pRtMony = pF5235A;
	}
	else if( Idx == ID5235B ){
		*pRtMony = pF5235B;
	}
	else if( Idx == ID5235C ){
		*pRtMony = pF5235C;
	}
	else if( Idx == ID5236A ){
		*pRtMony = pF5236A;
	}
	else if( Idx == ID5236B ){
		*pRtMony = pF5236B;
	}
	else if( Idx == ID5236C ){
		*pRtMony = pF5236C;
	}
//--> '14.10.17 INS START
	else if( Idx == ID5220T ){
		*pRtMony = pF5220T;
	}
	else if( Idx == ID5221T ){
		*pRtMony = pF5221T;
	}
	else if( Idx == ID5222T ){
		*pRtMony = pF5222T;
	}
	else if( Idx == ID5223T ){
		*pRtMony = pF5223T;
	}
	else if( Idx == ID5224T ){
		*pRtMony = pF5224T;
	}
	else if( Idx == ID5225T ){
		*pRtMony = pF5225T;
	}
	else if( Idx == ID5226T ){
		*pRtMony = pF5226T;
	}
	else if( Idx == ID5227T ){
		*pRtMony = pF5227T;
	}
	else if( Idx == ID5228T ){
		*pRtMony = pF5228T;
	}
	else if( Idx == ID5229T ){
		*pRtMony = pF5229T;
	}
	else if( Idx == ID5230T ){
		*pRtMony = pF5230T;
	}
	else if( Idx == ID5231T ){
		*pRtMony = pF5231T;
	}
	else if( Idx == ID5232T ){
		*pRtMony = pF5232T;
	}
	else if( Idx == ID5233T ){
		*pRtMony = pF5233T;
	}
	else if( Idx == ID5234T ){
		*pRtMony = pF5234T;
	}
	else if( Idx == ID5235T ){
		*pRtMony = pF5235T;
	}
	else if( Idx == ID5236T ){
		*pRtMony = pF5236T;
	}
//<-- '14.10.17 INS START
	else{
		return -1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 入力項目　色変え
//-----------------------------------------------------------------------------
// 引数	id		：
//		index	：
//		sign	：
//-----------------------------------------------------------------------------
void CShinFhyo522EX::ChangeColor( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	if( !m_DpSw ){	// 再集計のみ（表示無し）
		diag_getattr( id, index, &DA, CIcsdiagctrl );
		switch( sign ){
			case 1 :		// 入力不可	（白色）
				DA.attr_bcolor = BC_WHITE;
				break;
			case 2 :		// ロック	（灰色）
				DA.attr_bcolor = BC_GRAY;
				break;
			case 3 :		// 特殊色①	（青色）
				DA.attr_bcolor = BC_BLUE;
				break;		
			case 4 :		// 特殊色②	（緑色）
				DA.attr_bcolor = BC_GREEN;
				break;
			case 5 : 		// 特殊色③	（薄緑色）	 
				DA.attr_bcolor = BC_GREEN_L;
				break;
			default: 		// 実額入力	（クリーム色） 
				DA.attr_bcolor = BC_CREAM;
				break;
		}
		diag_setattr( id, index, &DA, FALSE, CIcsdiagctrl );
	}
}

//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinFhyo522EX::UpDownFocusMoveCheck(int Index , int Move)
{
	int idx = 0;
	int i = 0 , j = 0;

	DIAGRAM_ATTRIBUTE	DA;

	//現在ポジション検索
	for(i = 0 ; i < FCS_FHYO522CNT ; i++){
		if(Index == FCS_Fhyo522[i].IDNo){
			break;
		}
	}

	idx = FCS_Fhyo522[i].IDNo;//現在ポジションセット

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_FHYO522CNT ; j++){
			m_Fhyo522Diag01.GetAttribute(FCS_Fhyo522[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo522[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Fhyo522Diag01.GetAttribute(FCS_Fhyo522[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo522[j].IDNo;
				break;
			}
		}
	}

	return idx;
}

//-----------------------------------------------------------------------------
// 入力可能ポジションの取得 ('14.06.28)
//-----------------------------------------------------------------------------
// 引数		index	：	チェック対象スタート位置
//-----------------------------------------------------------------------------
// 返送値	-1以外	：	最初の入力可能位置
//			-1		：	該当無し
//-----------------------------------------------------------------------------
int CShinFhyo522EX::GetAbleInputPosition( int index )
{
	//現在ポジション検索
	int	rtIdx = -1;
	int stIdx = 0;
	for( int i=0 ; i<FCS_FHYO522CNT ; i++ ){
		if( index == FCS_Fhyo522[i].IDNo ){
			stIdx = i;
			break;
		}
	}

	DIAGRAM_ATTRIBUTE	DA;
//-- '15.05.02 --
//	for( int j=stIdx+1; j<FCS_FHYO522CNT ; j++ ){
//---------------
	for( int j=stIdx; j<FCS_FHYO522CNT ; j++ ){
//---------------
		m_Fhyo522Diag01.GetAttribute( FCS_Fhyo522[j].IDNo, (LPUNKNOWN)&DA );
		if( (DA.attr_bcolor==BC_CREAM) || (DA.attr_bcolor==BC_BLUE) ){
			rtIdx = FCS_Fhyo522[j].IDNo;
			break;
		}
	}

	return rtIdx;
}

BOOL CShinFhyo522EX::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){

			m_pShinFhyo52->WriteData();

//			CWnd*	p1	=	GetParent();
//			CWnd*	p2	=	p1->GetParent();
//			((CSyzShinSheet*)p2)->Fhyo4_calq();
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
				if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
					TerminationIcsdiagctrl1( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}
		//<--追加 harino ('14.03.04)
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
void CShinFhyo522EX::SetItemPosition()
{
	int index;

	//-->追加 harino ('14.03.06)
	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL1 ) {
		index = m_Fhyo522Diag01.GetPosition();
		if( (index = m_Fhyo522Diag01.GetPosition()) != -1) {
			m_Fhyo522Diag01.SetPosition(index);
			m_Fhyo522Diag01.SetFocus();
			return;
		}
	}
	//<--追加 harino ('14.03.06)
	m_Fhyo522Diag02.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット ('14.04.14)
	m_Fhyo522Diag01.SetFocus();

	//入力可能なインデックスを探す処理が必要？
/*- '14.06.28 -*/
//	m_Fhyo522Diag01.SetPosition( ID5218A );
/*-------------*/
	int setIdx = GetAbleInputPosition( ID5220A );
	if( setIdx != -1 ){
		m_Fhyo522Diag01.SetPosition( setIdx );
	}
/*-------------*/
}