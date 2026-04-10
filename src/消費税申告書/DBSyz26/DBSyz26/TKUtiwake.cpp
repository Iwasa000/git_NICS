// TKUtiwake.cpp : 実装ファイル
//

#include "stdafx.h"
#include "TKUtiwake.h"
#include "TKUtiwakeIdx.h"


// CTKUtiwake ダイアログ

IMPLEMENT_DYNAMIC(CTKUtiwake, CSyzBaseDlg)

CTKUtiwake::CTKUtiwake(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CTKUtiwake::IDD, pParent)
	, m_pSnHonpyoData(NULL)
	, m_pSnHeadData(NULL)
	, m_pArith(NULL)
	, m_pShinInfo(NULL)
	, m_pParent(pParent)
	, m_DpSw(0)
	, m_curwnd(-1)
{
	memset( XM1, '\0', sizeof(XM1) );
	memset( XM2, '\0', sizeof(XM2) );
	memset( XM3, '\0', sizeof(XM3) );
	memset( XM4, '\0', sizeof(XM4) );
	memset( XM5, '\0', sizeof(XM5) );
	memset( XM6, '\0', sizeof(XM6) );

	memset( UTI11, '\0', sizeof(UTI11) );
	memset( UTI12, '\0', sizeof(UTI12) );
	memset( UTI13, '\0', sizeof(UTI13) );
	memset( UTI23, '\0', sizeof(UTI23) );
	memset( UTI31, '\0', sizeof(UTI31) );
	memset( UTI32, '\0', sizeof(UTI32) );
	memset( UTI33, '\0', sizeof(UTI33) );
}

CTKUtiwake::~CTKUtiwake()
{
}

void CTKUtiwake::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_TkUtiwakeDiag01);
	DDX_Control(pDX, IDC_ICSDIAGCTRL2, m_TkUtiwakeDiag02);
}


BEGIN_MESSAGE_MAP(CTKUtiwake, CSyzBaseDlg)
END_MESSAGE_MAP()


// CTKUtiwake メッセージ ハンドラ

BOOL CTKUtiwake::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期化
	Init();

	// ダイアグラム初期設定
	ChgDiagAttr();

	// 基底クラスの初期化
	m_pBottomDiag = &m_TkUtiwakeDiag02;
	m_BottomIdx = 55;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CTKUtiwake::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( GetCombineKeyDown(VK_SHIFT) && (VK_F1<=pMsg->wParam) && (pMsg->wParam<=VK_F12) ){
			return TRUE;
		}
		if( pMsg->wParam == VK_END ){
//			// フォーカス移動
//			MoveFocusInEnd();
//			// 本表 一般 更新
			GetDiagData();
//			calqx_all( 6, 1 );

			WriteData();

			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}

			return TRUE;
		}
	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSnHonpyoData	：	本表データ読込みクラス
//		pSnFhyo10Data	：	付表１データ読込みクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CTKUtiwake::InitInfo( CH28SnHonpyoData **pSnHonpyoData, CH28SnFhyo10Data **pSnFhyo10Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo )
{
	ASSERT( pSnHonpyoData );
	if( pSnHonpyoData == NULL ){
		return -1;
	}
	m_pSnHonpyoData = pSnHonpyoData;

	ASSERT( pSnFhyo10Data );
	if( pSnFhyo10Data == NULL ){
		return -1;
	}
	m_pSnFhyo10Data = pSnFhyo10Data;

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

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	return 0;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTKUtiwake::Init()
{
	InitDiagAttr();

	ReadData();

	// 初期データ表示
	m_DpSw = 0;
	DispTKUtiwake();

	return 0;
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTKUtiwake::InitDiagAttr()
{
	DIAGRAM_ATTRIBUTE	DA;

	// ANK混じり設定
	// 納税地　住所(上)
	m_TkUtiwakeDiag01.GetAttribute( ID1030, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_TkUtiwakeDiag01.SetAttribute( ID1030, (LPUNKNOWN)&DA, TRUE );
	// 納税地　住所(下)
	m_TkUtiwakeDiag01.GetAttribute( ID1031, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_TkUtiwakeDiag01.SetAttribute( ID1031, (LPUNKNOWN)&DA, TRUE );
	// 氏名・屋号
	m_TkUtiwakeDiag01.GetAttribute( ID1051, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_TkUtiwakeDiag01.SetAttribute( ID1051, (LPUNKNOWN)&DA, TRUE );
	// 被合併名
	m_TkUtiwakeDiag01.GetAttribute( ID1052, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_TkUtiwakeDiag01.SetAttribute( ID1052, (LPUNKNOWN)&DA, TRUE );
	// 代表者氏名
	m_TkUtiwakeDiag01.GetAttribute( ID1061, (LPUNKNOWN)&DA );
	DA.attr_editattr |= DIAG_EATTR_ZENHAN;
	m_TkUtiwakeDiag01.SetAttribute( ID1061, (LPUNKNOWN)&DA, TRUE );

	m_TkUtiwakeDiag01.EnableDelete(0);
	m_TkUtiwakeDiag02.EnableDelete(0);

	return 0;
}

#ifdef _20150824_EXTEND_
//-----------------------------------------------------------------------------
// データの読込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTKUtiwake::ReadData()
{
	char	WORK0[MONY_BUF_SIZE]={0},
			WORK1[MONY_BUF_SIZE]={0},
			WORK2[MONY_BUF_SIZE]={0};

	m_Util.l_calq( WORK0, (char *)(*m_pSnFhyo10Data)->Sn_1F1A, 0 );
	m_Util.l_calq( WORK1, (char *)(*m_pSnFhyo10Data)->Sn_1F1B, 0 );
	m_Util.l_calq( WORK2, (char *)(*m_pSnFhyo10Data)->Sn_1F1C, 0 );

	if( (*m_pSnHeadData)->SVmzsw ){
		// 課税標準額
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KSTD, WORK0, WORK1 );
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KSTD, (*m_pSnHonpyoData)->Sn_KSTD, WORK2 );
		// 消費税額
		if( !((*m_pSnHonpyoData)->Sn_SYTsw&0x08) ){//消費税額手入力されていない場合のみ[Y]
			m_pArith->l_add( (*m_pSnHonpyoData)->Sn_SYTX, (*m_pSnFhyo10Data)->Sn_1F2A, (*m_pSnFhyo10Data)->Sn_1F2B );
			m_pArith->l_add( (*m_pSnHonpyoData)->Sn_SYTX, (*m_pSnHonpyoData)->Sn_SYTX, (*m_pSnFhyo10Data)->Sn_1F2C );
		}
		// 貸倒回収
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KJADJZ, (*m_pSnFhyo10Data)->Sn_1F3A, (*m_pSnFhyo10Data)->Sn_1F3B );
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KJADJZ, (*m_pSnHonpyoData)->Sn_KJADJZ, (*m_pSnFhyo10Data)->Sn_1F3C );
		// 控除対象仕入税額
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_SIREZ, (*m_pSnFhyo10Data)->Sn_1F4A, (*m_pSnFhyo10Data)->Sn_1F4B );
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_SIREZ, (*m_pSnHonpyoData)->Sn_SIREZ, (*m_pSnFhyo10Data)->Sn_1F4C );
		// 返還等対価にかかる税額
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_HKANZ, (*m_pSnFhyo10Data)->Sn_1F5A, (*m_pSnFhyo10Data)->Sn_1F5B );
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_HKANZ, (*m_pSnHonpyoData)->Sn_HKANZ, (*m_pSnFhyo10Data)->Sn_1F5C );
		// 貸倒に係る税額
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KSITZ, (*m_pSnFhyo10Data)->Sn_1F6A, (*m_pSnFhyo10Data)->Sn_1F6B );
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KSITZ, (*m_pSnHonpyoData)->Sn_KSITZ, (*m_pSnFhyo10Data)->Sn_1F6C );
	}
	else{
		memmove( (*m_pSnHonpyoData)->Sn_KSTD, WORK2, MONY_BUF_SIZE );

		if( !((*m_pSnHonpyoData)->Sn_SYTsw&0x08) ){
			memmove( (*m_pSnHonpyoData)->Sn_SYTX, (*m_pSnFhyo10Data)->Sn_1F2C, MONY_BUF_SIZE );
		}
		memmove( (*m_pSnHonpyoData)->Sn_KJADJZ, (*m_pSnFhyo10Data)->Sn_1F3C, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_SIREZ, (*m_pSnFhyo10Data)->Sn_1F4C, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_HKANZ, (*m_pSnFhyo10Data)->Sn_1F5C, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_KSITZ, (*m_pSnFhyo10Data)->Sn_1F6C, MONY_BUF_SIZE );
	}

	memmove( XM1, (*m_pSnHonpyoData)->Sn_KSTD, MONY_BUF_SIZE );

	// 返還等対価にかかる税額
	if( (*m_pSnHeadData)->SVmzsw ){
		memmove( XM4, (*m_pSnHonpyoData)->Sn_HKANZ, MONY_BUF_SIZE );

		m_pArith->l_add( XM5, (*m_pSnFhyo10Data)->Sn_1F5_1A, (*m_pSnFhyo10Data)->Sn_1F5_1B );
		m_pArith->l_add( XM5, XM5, (*m_pSnFhyo10Data)->Sn_1F5_1C );
		
		m_pArith->l_clear( XM6 );
		m_pArith->l_add( XM6, XM6, (*m_pSnFhyo10Data)->Sn_1F5_2C );
	}
	else{
		memmove( XM4, (*m_pSnHonpyoData)->Sn_HKANZ, MONY_BUF_SIZE );
		memmove( XM5, (*m_pSnHonpyoData)->Sn_URHKNZ, MONY_BUF_SIZE );
		memmove( XM6, (*m_pSnHonpyoData)->Sn_TKHKNZ, MONY_BUF_SIZE );
	}

	// 課税標準額の内訳
	if( (*m_pSnHeadData)->SVmzsw ){
		memmove( UTI11, (*m_pSnFhyo10Data)->Sn_1F1_1A, MONY_BUF_SIZE );
		memmove( UTI12, (*m_pSnFhyo10Data)->Sn_1F1_1B, MONY_BUF_SIZE );
		memmove( UTI13, (*m_pSnFhyo10Data)->Sn_1F1_1C, MONY_BUF_SIZE );

		memmove( UTI23, (*m_pSnFhyo10Data)->Sn_1F1_2C, MONY_BUF_SIZE );

		memmove( UTI31, (*m_pSnFhyo10Data)->Sn_1F1A, MONY_BUF_SIZE );
		memmove( UTI32, (*m_pSnFhyo10Data)->Sn_1F1B, MONY_BUF_SIZE );
		memmove( UTI33, (*m_pSnFhyo10Data)->Sn_1F1C, MONY_BUF_SIZE );
	}
	else{
		memmove( XM2, (*m_pSnHonpyoData)->Sn_KSJTKEI, MONY_BUF_SIZE );
		memmove( UTI13, XM2, MONY_BUF_SIZE );

		memmove( XM3, (*m_pSnHonpyoData)->Sn_TKSTKEI, MONY_BUF_SIZE );
		memmove( UTI23, XM3, MONY_BUF_SIZE );
	}

	return 0;
}
#else
//-----------------------------------------------------------------------------
// データの読込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTKUtiwake::ReadData()
{
	char	WORK0[MONY_BUF_SIZE]={0},
			WORK1[MONY_BUF_SIZE]={0},
			WORK2[MONY_BUF_SIZE]={0};

	// 課税標準額の内訳
	if( (*m_pSnHeadData)->SVmzsw ){
		memmove( UTI11, (*m_pSnFhyo10Data)->Sn_1F1_1A, MONY_BUF_SIZE );
		memmove( UTI12, (*m_pSnFhyo10Data)->Sn_1F1_1B, MONY_BUF_SIZE );
		memmove( UTI13, (*m_pSnFhyo10Data)->Sn_1F1_1C, MONY_BUF_SIZE );

		memmove( UTI23, (*m_pSnFhyo10Data)->Sn_1F1_2C, MONY_BUF_SIZE );

		memmove( UTI31, (*m_pSnFhyo10Data)->Sn_1F1A, MONY_BUF_SIZE );
		memmove( UTI32, (*m_pSnFhyo10Data)->Sn_1F1B, MONY_BUF_SIZE );
		memmove( UTI33, (*m_pSnFhyo10Data)->Sn_1F1C, MONY_BUF_SIZE );
	}
	else{
		memset( UTI11, '\0', MONY_BUF_SIZE );
		memset( UTI12, '\0', MONY_BUF_SIZE );
		memmove( UTI13, (*m_pSnFhyo10Data)->Sn_1F1_1C, MONY_BUF_SIZE );

		memmove( UTI23, (*m_pSnFhyo10Data)->Sn_1F1_2C, MONY_BUF_SIZE );

		memset( UTI31, '\0', MONY_BUF_SIZE );
		memset( UTI32, '\0', MONY_BUF_SIZE );
		memmove( UTI33, (*m_pSnFhyo10Data)->Sn_1F1C, MONY_BUF_SIZE );
	}

	// 課税標準額
	m_Util.l_calq( WORK0, (char *)(*m_pSnFhyo10Data)->Sn_1F1A, 0 );
	m_Util.l_calq( WORK1, (char *)(*m_pSnFhyo10Data)->Sn_1F1B, 0 );
	m_Util.l_calq( WORK2, (char *)(*m_pSnFhyo10Data)->Sn_1F1C, 0 );
	if( (*m_pSnHeadData)->SVmzsw ){
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KSTD, WORK0, WORK1 );
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KSTD, (*m_pSnHonpyoData)->Sn_KSTD, WORK2 );

		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KSJTKEI, UTI11, UTI12 );
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_KSJTKEI, (*m_pSnHonpyoData)->Sn_KSJTKEI, UTI13 );

		memmove( (*m_pSnHonpyoData)->Sn_TKSTKEI, UTI23, MONY_BUF_SIZE );
	}
	else{
		memmove( (*m_pSnHonpyoData)->Sn_KSTD, WORK2, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_KSJTKEI, UTI13, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_TKSTKEI, UTI23, MONY_BUF_SIZE );
	}
	memmove( XM1, (*m_pSnHonpyoData)->Sn_KSTD, MONY_BUF_SIZE );
	memmove( XM2, (*m_pSnHonpyoData)->Sn_KSJTKEI, MONY_BUF_SIZE );
	memmove( XM3, (*m_pSnHonpyoData)->Sn_TKSTKEI, MONY_BUF_SIZE );

	// 返還等対価にかかる税額
	if( (*m_pSnHeadData)->SVmzsw ){
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_HKANZ, (*m_pSnFhyo10Data)->Sn_1F5A, (*m_pSnFhyo10Data)->Sn_1F5B );
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_HKANZ, (*m_pSnHonpyoData)->Sn_HKANZ, (*m_pSnFhyo10Data)->Sn_1F5C );

		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_URHKNZ, (*m_pSnFhyo10Data)->Sn_1F5_1A, (*m_pSnFhyo10Data)->Sn_1F5_1B );
		m_pArith->l_add( (*m_pSnHonpyoData)->Sn_URHKNZ, (*m_pSnHonpyoData)->Sn_URHKNZ, (*m_pSnFhyo10Data)->Sn_1F5_1C );

		memmove( (*m_pSnHonpyoData)->Sn_TKHKNZ, (*m_pSnFhyo10Data)->Sn_1F5_2C, MONY_BUF_SIZE );
	}
	else{
		memmove( (*m_pSnHonpyoData)->Sn_HKANZ, (*m_pSnFhyo10Data)->Sn_1F5C, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_URHKNZ, (*m_pSnFhyo10Data)->Sn_1F5_1C, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_TKHKNZ, (*m_pSnFhyo10Data)->Sn_1F5_2C, MONY_BUF_SIZE );
	}
	memmove( XM4, (*m_pSnHonpyoData)->Sn_HKANZ, MONY_BUF_SIZE );
	memmove( XM5, (*m_pSnHonpyoData)->Sn_URHKNZ, MONY_BUF_SIZE );
	memmove( XM6, (*m_pSnHonpyoData)->Sn_TKHKNZ, MONY_BUF_SIZE );

	return 0;
}
#endif

//-----------------------------------------------------------------------------
// データの書込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CTKUtiwake::WriteData()
{
	if( (*m_pSnHeadData)->SVmzsw ){
		// 課税資産の譲渡等の対価の額
		memmove( (*m_pSnHonpyoData)->Sn_KSJT3, (*m_pSnFhyo10Data)->Sn_1F1_1A, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_KSJT4, (*m_pSnFhyo10Data)->Sn_1F1_1B, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_KSJT63, (*m_pSnFhyo10Data)->Sn_1F1_1C, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_KSJTKEI, XM2, MONY_BUF_SIZE );

		// 特定課税仕入に係る支払対価の額
		memmove( (*m_pSnHonpyoData)->Sn_TKST63, (*m_pSnFhyo10Data)->Sn_1F1_2C, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_TKSTKEI, XM3, MONY_BUF_SIZE );

		// 売上げの返還等対価に係る税額
		memmove( (*m_pSnHonpyoData)->Sn_URHKNZ, XM5, MONY_BUF_SIZE );

		// 特定課税仕入の返還等対価に係る税額
		memmove( (*m_pSnHonpyoData)->Sn_TKHKNZ, XM6, MONY_BUF_SIZE );
	}
	else{
		// 課税標準額
		memmove( (*m_pSnFhyo10Data)->Sn_1F1C, XM1, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_KSTD, (*m_pSnFhyo10Data)->Sn_1F1C, MONY_BUF_SIZE );

		// 課税資産の譲渡等の対価の額
		memset( (*m_pSnFhyo10Data)->Sn_1F1_1A, '\0', MONY_BUF_SIZE );
		memset( (*m_pSnFhyo10Data)->Sn_1F1_1B, '\0', MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo10Data)->Sn_1F1_1C, XM2, MONY_BUF_SIZE );

		memmove( (*m_pSnHonpyoData)->Sn_KSJT3, (*m_pSnFhyo10Data)->Sn_1F1_1A, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_KSJT4, (*m_pSnFhyo10Data)->Sn_1F1_1B, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_KSJT63, (*m_pSnFhyo10Data)->Sn_1F1_1C, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_KSJTKEI, (*m_pSnHonpyoData)->Sn_KSJT63, MONY_BUF_SIZE );

		// 特定課税仕入に係る支払対価の額
		memmove( (*m_pSnFhyo10Data)->Sn_1F1_2C, XM3, MONY_BUF_SIZE );

		memmove( (*m_pSnHonpyoData)->Sn_TKST63, (*m_pSnFhyo10Data)->Sn_1F1_2C, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_TKSTKEI, (*m_pSnFhyo10Data)->Sn_1F1_2C, MONY_BUF_SIZE );

		// 返還等対価に係る税額
		memmove( (*m_pSnFhyo10Data)->Sn_1F5C, XM4, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_HKANZ, (*m_pSnFhyo10Data)->Sn_1F5C, MONY_BUF_SIZE );

		// 売上げの返還等対価に係る税額
		memset( (*m_pSnFhyo10Data)->Sn_1F5_1A, '\0', MONY_BUF_SIZE );
		memset( (*m_pSnFhyo10Data)->Sn_1F5_1B, '\0', MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo10Data)->Sn_1F5_1C, XM5, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_URHKNZ, (*m_pSnFhyo10Data)->Sn_1F5_1C, MONY_BUF_SIZE );

		// 特定課税仕入の返還等対価に係る税額
		memmove( (*m_pSnFhyo10Data)->Sn_1F5_2C, XM6, MONY_BUF_SIZE );
		memmove( (*m_pSnHonpyoData)->Sn_TKHKNZ, (*m_pSnFhyo10Data)->Sn_1F5_2C, MONY_BUF_SIZE );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 金額欄表示
//-----------------------------------------------------------------------------
void CTKUtiwake::DispTKUtiwake()
{
	char				buf[512] = {0};
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

//	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){//[Y]連動の場合手入力サイン初期化
//		//手入力サイン初期化
//		m_M1c = 0;
//	}

	// 納税地　郵便番号
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_ZIP, 8 );
	DiagData.data_edit = buf;
	m_TkUtiwakeDiag01.SetData( ID1020, (struct IUnknown *)&DiagData );
	// 納税地　住所(上)
	memset( buf, '\0', sizeof( buf ) );
	int len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_NOZEI, 40 );
	memmove( buf, (*m_pSnHeadData)->Sn_NOZEI, len );
	DiagData.data_edit = buf;
	m_TkUtiwakeDiag01.SetData( ID1030, (struct IUnknown *)&DiagData );
	// 納税地　住所(下)
	memset( buf, '\0', sizeof( buf ) );
	len = m_Util.MixSpaceCutLength( (char *)&(*m_pSnHeadData)->Sn_NOZEI[40], 40 );
	memmove( buf, &(*m_pSnHeadData)->Sn_NOZEI[40], len );
	DiagData.data_edit = buf;
	m_TkUtiwakeDiag01.SetData( ID1031, (struct IUnknown *)&DiagData );

	// 納税地　電話番号
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_TELNO1, sizeof((*m_pSnHeadData)->Sn_TELNO1) );
	DiagData.data_edit = buf;
	m_TkUtiwakeDiag01.SetData( ID1040, (struct IUnknown *)&DiagData );
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_TELNO2, sizeof((*m_pSnHeadData)->Sn_TELNO2) );
	DiagData.data_edit = buf;
	m_TkUtiwakeDiag01.SetData( ID1041, (struct IUnknown *)&DiagData );
	memset( buf, '\0', sizeof(buf) );
	memmove( buf, (*m_pSnHeadData)->Sn_TELNO3, sizeof((*m_pSnHeadData)->Sn_TELNO3) );
	DiagData.data_edit = buf;
	m_TkUtiwakeDiag01.SetData( ID1042, (struct IUnknown *)&DiagData );
	// 氏名・屋号　フリガナ
	memset( buf, '\0', sizeof(buf) );
//2016.10.20 UPDATE START
//	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_COFRI, 68 );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_COFRI, 80 );
//2016.10.20 UPDATE END
	memmove( buf, (*m_pSnHeadData)->Sn_COFRI, len );
	DiagData.data_edit = buf;
	m_TkUtiwakeDiag01.SetData( ID1050, (struct IUnknown *)&DiagData );
	// 氏名・屋号
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_CONAM, sizeof((*m_pSnHeadData)->Sn_CONAM) );
	memmove( buf, (*m_pSnHeadData)->Sn_CONAM, len );
	DiagData.data_edit = buf;
	m_TkUtiwakeDiag01.SetData( ID1051, (struct IUnknown *)&DiagData );

	// 被合併名
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_GPNAM, 40 );
	memmove( buf, (*m_pSnHeadData)->Sn_GPNAM, len );
	DiagData.data_edit = buf;
	m_TkUtiwakeDiag01.SetData( ID1052, (struct IUnknown *)&DiagData );
	// 代表者氏名　フリガナ
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_DIFRI, 68 );
	memmove( buf, (*m_pSnHeadData)->Sn_DIFRI, len );
	DiagData.data_edit = buf;
	m_TkUtiwakeDiag01.SetData( ID1060, (struct IUnknown *)&DiagData );
	// 代表者氏名
	memset( buf, '\0', sizeof(buf) );
	len = m_Util.MixSpaceCutLength( (char *)(*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO) );
	memmove( buf, (*m_pSnHeadData)->Sn_DIHYO, len );
	DiagData.data_edit = buf;
	m_TkUtiwakeDiag01.SetData( ID1061, (struct IUnknown *)&DiagData );

	// 整理番号
	if( ((*m_pSnHeadData)->Sn_SEIRI[0]&0xff) != 0xff ){
		memset( buf, '\0', sizeof( buf ) );
		m_pArith->l_unpac( buf, (*m_pSnHeadData)->Sn_SEIRI, 8 );
		DiagData.data_edit = buf;
		m_TkUtiwakeDiag01.SetData( ID1071, (struct IUnknown *)&DiagData );
	}

	// 課税期間
	int		gengo=0;
	char	yy=0, mm=0, dd=0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd, &gengo );
	if( (yy&0xff) && (mm&0xff) && (dd&0xff) ){ 
		memset(buf, '\0', sizeof(buf));
		if( gengo == 5 ){
			sprintf_s(buf, sizeof(buf), _T("自 %s"), SYZ26_REIWA_G_STR);
		}
		else{
			sprintf_s(buf, sizeof(buf), _T("自 %s"), SYZ26_HEISEI_G_STR);
		}
		DiagData.data_disp = buf;
		m_TkUtiwakeDiag01.SetData(ID1083, (struct IUnknown *)&DiagData);

		memset( buf, '\0', sizeof( buf ) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), yy );
		DiagData.data_disp = buf;
		m_TkUtiwakeDiag01.SetData( ID1080, (struct IUnknown *)&DiagData );
		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), mm );
		DiagData.data_disp = buf;
		m_TkUtiwakeDiag01.SetData( ID1081, (struct IUnknown *)&DiagData );
		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), dd );
		DiagData.data_disp = buf;
		m_TkUtiwakeDiag01.SetData( ID1082, (struct IUnknown *)&DiagData );

	}
	yy = mm = dd = gengo = 0;
	(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yy, &mm, &dd, &gengo );
	if( (yy&0xff) && (mm&0xff) && (dd&0xff) ){
		memset(buf, '\0', sizeof(buf));
		if( gengo == 5 ){
			sprintf_s(buf, sizeof(buf), _T("至 %s"), SYZ26_REIWA_G_STR);
		}
		else{
			sprintf_s(buf, sizeof(buf), _T("至 %s"), SYZ26_HEISEI_G_STR);
		}
		DiagData.data_disp = buf;
		m_TkUtiwakeDiag01.SetData(ID1093, (struct IUnknown *)&DiagData);

		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), yy );
		DiagData.data_disp = buf;
		m_TkUtiwakeDiag01.SetData( ID1090, (struct IUnknown *)&DiagData );
		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), mm );
		DiagData.data_disp = buf;
		m_TkUtiwakeDiag01.SetData( ID1091, (struct IUnknown *)&DiagData );
		memset( buf, '\0', sizeof(buf) );
		sprintf_s( buf, sizeof(buf), _T("%02x"), dd );
		DiagData.data_disp = buf;
		m_TkUtiwakeDiag01.SetData( ID1092, (struct IUnknown *)&DiagData );
	}
	// 申告区分
	memset( buf, '\0', sizeof(buf) );
	switch( (*m_pSnHeadData)->Sn_SKKBN ){
		case 1 :
			memmove( buf, _T("　確定　"), 8 );
			break;
		case 2 :
			memmove( buf, _T("　中間　"), 8 );
			break;
		case 3 :
			memmove( buf, _T("修正確定"), 8 );
			break;
		default:
			memmove( buf, _T("修正中間"), 8 );
			break;
	}
	DiagData.data_disp = buf;
	m_TkUtiwakeDiag01.SetData( ID1100, (struct IUnknown *)&DiagData );

	// 中間期間
	if( !(((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) ){
		yy = mm = dd = gengo = 0;
		(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, &yy, &mm, &dd, &gengo );
		if( (yy&0xff) && (mm&0xff) && (dd&0xff) ){ 
			memset(buf, '\0', sizeof(buf));
			if( gengo == 5 ){
				sprintf_s(buf, sizeof(buf), _T("自 %s"), SYZ26_REIWA_G_STR);
			}
			else{
				sprintf_s(buf, sizeof(buf), _T("自 %s"), SYZ26_HEISEI_G_STR);
			}
			DiagData.data_disp = buf;
			m_TkUtiwakeDiag01.SetData(ID1113, (struct IUnknown *)&DiagData);

			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), yy );
			DiagData.data_disp = buf;
			m_TkUtiwakeDiag01.SetData( ID1110, (struct IUnknown *)&DiagData );
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), mm );
			DiagData.data_disp = buf;
			m_TkUtiwakeDiag01.SetData( ID1111, (struct IUnknown *)&DiagData );
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), dd );
			DiagData.data_disp = buf;
			m_TkUtiwakeDiag01.SetData( ID1112, (struct IUnknown *)&DiagData );
		}
		yy = mm = dd = gengo = 0;
		(*m_pSnHeadData)->GetYmdDataGen( ID_ICSSH_CHUUKAN_TO, &yy, &mm, &dd, &gengo );
		if( (yy&0xff) && (mm&0xff) && (dd&0xff) ){ 
			memset(buf, '\0', sizeof(buf));
			if( gengo == 5 ){
				sprintf_s(buf, sizeof(buf), _T("至 %s"), SYZ26_REIWA_G_STR);
			}
			else{
				sprintf_s(buf, sizeof(buf), _T("至 %s"), SYZ26_HEISEI_G_STR);
			}
			DiagData.data_disp = buf;
			m_TkUtiwakeDiag01.SetData(ID1123, (struct IUnknown *)&DiagData);

			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), yy );
			DiagData.data_disp = buf;
			m_TkUtiwakeDiag01.SetData( ID1120, (struct IUnknown *)&DiagData );
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), mm );
			DiagData.data_disp = buf;
			m_TkUtiwakeDiag01.SetData( ID1121, (struct IUnknown *)&DiagData );
			memset( buf, '\0', sizeof(buf) );
			sprintf_s( buf, sizeof(buf), _T("%02x"), dd );
			DiagData.data_disp = buf;
			m_TkUtiwakeDiag01.SetData( ID1122, (struct IUnknown *)&DiagData );
		}
	}

	// 表示（金額欄）
	DispTKUtiwakeMony();
}

//-----------------------------------------------------------------------------
// 表示（金額欄）
//-----------------------------------------------------------------------------
void CTKUtiwake::DispTKUtiwakeMony()
{
	// 画面の初期化
	for( int i=ID2XM1; i<=ID2UTI33; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL2, i );
	}
	// 計算&表示
	calqx_all( 1, 0 );
}

//-----------------------------------------------------------------------------
// 表示クリア
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//-----------------------------------------------------------------------------
void CTKUtiwake::dsp_cls( unsigned short Id, short Index )
{
	if( m_DpSw ){
		return;
	}
	if( Id == IDC_ICSDIAGCTRL1 ){		// 基本情報
		m_TkUtiwakeDiag01.DataClear( Index, TRUE );
	}
	else if( Id == IDC_ICSDIAGCTRL2 ){	// 計算項目
		DIAGRAM_DATA	DiagData;
		DiagData.data_imgdata = NULL;

		switch( Index ){
			case ID2XM1		:
			case ID2XM2		:
			case ID2XM3		:
			case ID2XM4		:
			case ID2XM5		:
			case ID2XM6		:
			case ID2UTI11	:
			case ID2UTI12	:
			case ID2UTI13	:
			case ID2UTI23	:
			case ID2UTI31	:
			case ID2UTI32	:
			case ID2UTI33	:
				memset( DiagData.data_val, '\0', MONY_BUF_SIZE );
				m_TkUtiwakeDiag02.SetData( Index, (struct IUnknown *)&DiagData );
				break;
			default		:
				m_TkUtiwakeDiag02.DataClear( Index, TRUE );
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//		Mony	：	表示金額
//-----------------------------------------------------------------------------
void CTKUtiwake::dsp_prs( unsigned short Id, short Index, char *Mony )
{
	if( m_DpSw ){
		return;
	}
	if( Id == IDC_ICSDIAGCTRL2 ){	// 計算項目
		DIAGRAM_DATA	DiagData;
		DiagData.data_imgdata = NULL;
		char			M1000[MONY_BUF_SIZE] = {0};
		char			MNY[MONY_BUF_SIZE] = {0};

		switch( Index ){
			case ID2XM1		:
			case ID2XM2		:
			case ID2XM3		:
			case ID2XM4		:
			case ID2XM5		:
			case ID2XM6		:
			case ID2UTI11	:
			case ID2UTI12	:
			case ID2UTI13	:
			case ID2UTI23	:
				memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
				m_TkUtiwakeDiag02.SetData( Index, (struct IUnknown *)&DiagData );
				break;
			case ID2UTI31	:
			case ID2UTI32	:
			case ID2UTI33	:
				m_pArith->l_input( M1000, "1000" );
				m_pArith->l_div( MNY, Mony, M1000 );
				memmove( DiagData.data_val, MNY, MONY_BUF_SIZE );
				m_TkUtiwakeDiag02.SetData( Index, (struct IUnknown *)&DiagData );
				break;
			default		:
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// ダイアグラム設定変更
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CTKUtiwake::ChgDiagAttr()
{
	int cnt = 0;
	while( 1 ){
		if( TkUtiwakeIdx2[cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = TkUtiwakeIdx2[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = TkUtiwakeIdx2[cnt].UnConnect;
			}
			else{
				Flg = TkUtiwakeIdx2[cnt].Connect;
			}
//			if( !(((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) ){
//				if( (TkUtiwakeIdx2[cnt].Index==ID3XM10) || (TkUtiwakeIdx2[cnt].Index==ID3XM21) ){
//					Flg = 0;
//				}
//			}
//			if( (TkUtiwakeIdx2[cnt].Index==ID3XM13) || (TkUtiwakeIdx2[cnt].Index==ID3XM24) ){
//				if( Flg == 1 ){
//					// 修正確定・修正中間
//					if( ((*m_pSnHeadData)->Sn_SKKBN==0x03) || ((*m_pSnHeadData)->Sn_SKKBN==0x04) ){
//						;
//					}
//					else{
//						Flg = 0;
//					}
//				}
//				else{
//					Flg = 0;
//				}
//			}
		}

		int flg_ex = 0;

		if( Flg == 2 ){
			if( (*m_pSnHeadData)->SVmzsw == 0 ){	// 経過措置なし
				flg_ex = TRUE;
			}
			else{								// 経過措置あり
				flg_ex = FALSE;
			}
		}
		else{
			if( Flg ){
				flg_ex = TRUE;
			}
			else{
				flg_ex = FALSE;
			}
		}

		ATRIB_MOD( IDC_ICSDIAGCTRL2, TkUtiwakeIdx2[cnt].Index, flg_ex, (*m_pSnHeadData)->Sn_Sign4, TkUtiwakeIdx2[cnt].Connect );
		cnt++;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CTKUtiwake::EndJob()
{
}

//-----------------------------------------------------------------------------
// 金額の最終データ読み込み
//-----------------------------------------------------------------------------
void CTKUtiwake::GetDiagData()
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	char			M1000[MONY_BUF_SIZE]={0};

	// 1.課税標準額
	m_TkUtiwakeDiag02.GetData( ID2XM1, (struct IUnknown *)&DiagData );
	memmove( XM1, DiagData.data_val, MONY_BUF_SIZE ); 
	// 2.課税資産の譲渡等の対価の額
	m_TkUtiwakeDiag02.GetData( ID2XM2, (struct IUnknown *)&DiagData );
	memmove( XM2, DiagData.data_val, MONY_BUF_SIZE ); 
	// 3.特定課税仕入に係る支払対価の額
	m_TkUtiwakeDiag02.GetData( ID2XM3, (struct IUnknown *)&DiagData );
	memmove( XM3, DiagData.data_val, MONY_BUF_SIZE ); 
	// 4.返還等対価に係る税額
	m_TkUtiwakeDiag02.GetData( ID2XM4, (struct IUnknown *)&DiagData );
	memmove( XM4, DiagData.data_val, MONY_BUF_SIZE ); 
	// 5.売上げの返還等対価に係る税額
	m_TkUtiwakeDiag02.GetData( ID2XM5, (struct IUnknown *)&DiagData );
	memmove( XM5, DiagData.data_val, MONY_BUF_SIZE ); 
	// 6.特定課税仕入の返還等対価に係る税額
	m_TkUtiwakeDiag02.GetData( ID2XM6, (struct IUnknown *)&DiagData );
	memmove( XM6, DiagData.data_val, MONY_BUF_SIZE ); 
}

BEGIN_EVENTSINK_MAP(CTKUtiwake, CSyzBaseDlg)
	ON_EVENT(CTKUtiwake, IDC_ICSDIAGCTRL2, 2, CTKUtiwake::EditOFFIcsdiagctrl2, VTS_I2)
	ON_EVENT(CTKUtiwake, IDC_ICSDIAGCTRL2, 3, CTKUtiwake::TerminationIcsdiagctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CTKUtiwake, IDC_ICSDIAGCTRL2, 1, CTKUtiwake::EditONIcsdiagctrl2, VTS_I2)
END_EVENTSINK_MAP()

void CTKUtiwake::EditOFFIcsdiagctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	DIAGRAM_DATA	DiagData;
	char			CkMy[MONY_BUF_SIZE] = {0};

	m_TkUtiwakeDiag02.GetData( index, (struct IUnknown *)&DiagData );
	switch( index ){
		case ID2XM1	:
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, XM1, MONY_BUF_SIZE );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE );
//			if( m_pArith->l_cmp(XM1, CkMy) ){
//				m_M1c |= 0x01;
//			}
			memmove( XM1, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			calqx_all( 1, 0 );
			break;
		case ID2XM2	:
			memmove( XM2, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 2, 0 );
			break;
		case ID2XM3	:
			memmove( XM3, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 3, 0 );
			break;
		case ID2XM4	:
			memmove( XM4, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 4, 0 );
			break;
		case ID2XM5	:
			memmove( XM5, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 5, 0 );
			break;
		case ID2XM6	:
			memmove( XM6, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			calqx_all( 6, 0 );
			break;
		default:
			break;
	}
}

void CTKUtiwake::TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}

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

	// 次項目
	int nowdiag = 2;
	if( (wTerm==VK_RETURN) || (wTerm==VK_RIGHT) || (wTerm==VK_F3) || (wTerm==VK_DOWN) ){
		m_TkUtiwakeDiag02.SetNextPosition();
	}

	// 前項目
	if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) || (wTerm==VK_UP) ){
		m_TkUtiwakeDiag02.SetPrevPosition();			// 戻り
	}

	if( wTerm == VK_DELETE ){
		dsp_cls( IDC_ICSDIAGCTRL2 , index );
	}

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	int nowpos;	// 移動先または自身のポジション
	nowpos = m_TkUtiwakeDiag02.GetPosition();
	m_TkUtiwakeDiag02.GetRect( nowpos, (LPUNKNOWN)&rectB );

	if ( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
}

//-----------------------------------------------------------------------------
// ポジションセット
//-----------------------------------------------------------------------------
void CTKUtiwake::SetItemPosition()
{
	int index;
	if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
		index = m_TkUtiwakeDiag02.GetPosition();
		if( (index = m_TkUtiwakeDiag02.GetPosition()) != -1) {
			m_TkUtiwakeDiag02.SetPosition(index);
			m_TkUtiwakeDiag02.SetFocus();
			return;
		}
	}

	m_TkUtiwakeDiag01.SetFocus();
	m_TkUtiwakeDiag02.SetFocus();

	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		diag_setposition( IDC_ICSDIAGCTRL2, ID2XM2, CIcsdiagctrl );
	}
}

void CTKUtiwake::EditONIcsdiagctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAGCTRL2;
}

//-----------------------------------------------------------------------------
// 特定課税仕入に関する項目の金額クリア('15.12.02)
//-----------------------------------------------------------------------------
void CTKUtiwake::ClearTkkzMony()
{
	m_pArith->l_clear( UTI23 );
	calq_utiwake();

	m_pArith->l_clear( XM3 );
	calqx_1to3();
	m_pArith->l_clear( XM6 );
	calqx_4to6();

	DispTKUtiwake();

	WriteData();
}
