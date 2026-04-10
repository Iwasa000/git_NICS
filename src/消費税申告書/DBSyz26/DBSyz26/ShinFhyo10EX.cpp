// ShinFhyo10EX.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo10EX.h"
#include "ShinFhyo10EXIdx.h"


// CShinFhyo10EXEX ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo10EX, CSyzBaseDlg)

CShinFhyo10EX::CShinFhyo10EX(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo10EX::IDD, pParent)
	, m_pSnFhyo10Data(NULL)
	, m_pSnHeadData(NULL)
	, m_pArith(NULL)
	, m_pShinInfo(NULL)
	, m_pSnHonpyoData(NULL)
	, m_pParent(pParent)
	, m_FrSw(0)
{
	memset( F11A, '\0', sizeof(F11A) );
	memset( F11B, '\0', sizeof(F11B) );
	memset( F11C, '\0', sizeof(F11C) );
	memset( F11T, '\0', sizeof(F11T) );

	memset( F11_1A, '\0', sizeof(F11_1A) );
	memset( F11_1B, '\0', sizeof(F11_1B) );
	memset( F11_1C, '\0', sizeof(F11_1C) );
	memset( F11_1T, '\0', sizeof(F11_1T) );
									
	memset( F11_2A, '\0', sizeof(F11_2A) );
	memset( F11_2B, '\0', sizeof(F11_2B) );
	memset( F11_2C, '\0', sizeof(F11_2C) );
	memset( F11_2T, '\0', sizeof(F11_2T) );

	memset( F12A, '\0', sizeof(F12A) );
	memset( F12B, '\0', sizeof(F12B) );
	memset( F12C, '\0', sizeof(F12C) );
	memset( F12T, '\0', sizeof(F12T) );

	memset( F13A, '\0', sizeof(F13A) );
	memset( F13B, '\0', sizeof(F13B) );
	memset( F13C, '\0', sizeof(F13C) );
	memset( F13T, '\0', sizeof(F13T) );

	memset( F14A, '\0', sizeof(F14A) );
	memset( F14B, '\0', sizeof(F14B) );
	memset( F14C, '\0', sizeof(F14C) );
	memset( F14T, '\0', sizeof(F14T) );

	memset( F15A, '\0', sizeof(F15A) );
	memset( F15B, '\0', sizeof(F15B) );
	memset( F15C, '\0', sizeof(F15C) );
	memset( F15T, '\0', sizeof(F15T) );

	memset( F15_1A, '\0', sizeof(F15_1A) );
	memset( F15_1B, '\0', sizeof(F15_1B) );
	memset( F15_1C, '\0', sizeof(F15_1C) );
	memset( F15_1T, '\0', sizeof(F15_1T) );
									
	memset( F15_2A, '\0', sizeof(F15_2A) );
	memset( F15_2B, '\0', sizeof(F15_2B) );
	memset( F15_2C, '\0', sizeof(F15_2C) );
	memset( F15_2T, '\0', sizeof(F15_2T) );

	memset( F16A, '\0', sizeof(F16A) );
	memset( F16B, '\0', sizeof(F16B) );
	memset( F16C, '\0', sizeof(F16C) );
	memset( F16T, '\0', sizeof(F16T) );

	memset( F17A, '\0', sizeof(F17A) );
	memset( F17B, '\0', sizeof(F17B) );
	memset( F17C, '\0', sizeof(F17C) );
	memset( F17T, '\0', sizeof(F17T) );

	memset( F18A, '\0', sizeof(F18A) );
	memset( F18B, '\0', sizeof(F18B) );
	memset( F18C, '\0', sizeof(F18C) );
	memset( F18T, '\0', sizeof(F18T) );

	memset( F19A, '\0', sizeof(F19A) );
	memset( F19B, '\0', sizeof(F19B) );
	memset( F19C, '\0', sizeof(F19C) );
	memset( F19T, '\0', sizeof(F19T) );

	memset( F110T, '\0', sizeof(F110T) );

	memset( F111B, '\0', sizeof(F111B) );
	memset( F111C, '\0', sizeof(F111C) );
	memset( F111T, '\0', sizeof(F111T) );

	memset( F112B, '\0', sizeof(F112B) );
	memset( F112C, '\0', sizeof(F112C) );
	memset( F112T, '\0', sizeof(F112T) );

	memset( F113T, '\0', sizeof(F113T) );

	memset( F114B, '\0', sizeof(F114B) );
	memset( F114C, '\0', sizeof(F114C) );
	memset( F114T, '\0', sizeof(F114T) );

	memset( F115B, '\0', sizeof(F115B) );
	memset( F115C, '\0', sizeof(F115C) );
	memset( F115T, '\0', sizeof(F115T) );

	memset( F116T, '\0', sizeof(F116T) );

	F1c = 0;
	F0c = 0;
	F2c = 0;

	m_curwnd = -1;
}

CShinFhyo10EX::~CShinFhyo10EX()
{
}

void CShinFhyo10EX::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Fhyo10Diag01);
	DDX_Control(pDX, IDC_ICSDIAGCTRL2, m_Fhyo10Diag02);
}


BEGIN_MESSAGE_MAP(CShinFhyo10EX, CSyzBaseDlg)
END_MESSAGE_MAP()


// CShinFhyo10EX メッセージ ハンドラ

BOOL CShinFhyo10EX::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期化
	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Fhyo10Diag02;
	m_BottomIdx = 149;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSnFhyo40Data	：	付表１データ読込みクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//		pSnHonpyoData	：	本表データ読込みクラス
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo10EX::InitInfo( CH28SnFhyo10Data **pSnFhyo10Data, CSnHeadData **pSnHeadData, CArithEx *pArith, SYC_SHININFO *pShinInfo , CH28SnHonpyoData **pSnHonpyoData )
{
	ASSERT( pSnFhyo10Data );
	if( pSnFhyo10Data == NULL ){
		return -1;
	}
	m_pSnFhyo10Data = pSnFhyo10Data;

	//[Y]
	m_pSnHonpyoData = pSnHonpyoData;
	ASSERT( m_pSnHonpyoData );
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
int CShinFhyo10EX::Init()
{
	//int index;

	// データ読込み
	ReadData( m_FrSw );
	m_FrSw = 1;

	// ダイアグラム初期属性セット
	InitDiagAttr();
	// 画面出力
	DispFh1();

	SetItemPosition();	// フォーカスセットを関数化 harino ('14.03.12)

	return 0;
}


//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
void CShinFhyo10EX::SetItemPosition()
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
		index = m_Fhyo10Diag02.GetPosition();
		if( (index = m_Fhyo10Diag02.GetPosition()) != -1) {
			m_Fhyo10Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット ('14.04.14)
			m_Fhyo10Diag02.SetPosition(index);
			m_Fhyo10Diag02.SetFocus();
			return;
		}
	}

	m_Fhyo10Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット ('14.04.14)
	m_Fhyo10Diag02.SetFocus();

	// 課税標準率A欄へ
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
			diag_setposition( IDC_ICSDIAGCTRL2, ID11_1A, CIcsdiagctrl );
		}
		else{
			diag_setposition( IDC_ICSDIAGCTRL2, ID11A, CIcsdiagctrl );
		}
	}
}

//-----------------------------------------------------------------------------
// データの読込み
//-----------------------------------------------------------------------------
// 引数		pas	：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFhyo10EX::ReadData( int pas )
{
	// 課税標準額
	memmove( F11A, (*m_pSnFhyo10Data)->Sn_1F1A, MONY_BUF_SIZE );
	memmove( F11B, (*m_pSnFhyo10Data)->Sn_1F1B, MONY_BUF_SIZE );
	memmove( F11C, (*m_pSnFhyo10Data)->Sn_1F1C, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		memmove( F11_1A, (*m_pSnFhyo10Data)->Sn_1F1_1A, MONY_BUF_SIZE );
		memmove( F11_1B, (*m_pSnFhyo10Data)->Sn_1F1_1B, MONY_BUF_SIZE );
		memmove( F11_1C, (*m_pSnFhyo10Data)->Sn_1F1_1C, MONY_BUF_SIZE );
	}
	else{
		memset( F11_1A, '\0', MONY_BUF_SIZE );
		memset( F11_1B, '\0', MONY_BUF_SIZE );
		memset( F11_1C, '\0', MONY_BUF_SIZE );
	}

	// 特定課税仕入れに係る支払対価の額
	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		memmove( F11_2C, (*m_pSnFhyo10Data)->Sn_1F1_2C, MONY_BUF_SIZE );
	}
	else{
		memset( F11_2C, '\0', MONY_BUF_SIZE );
	}

	// 消費税額
	memmove( F12A, (*m_pSnFhyo10Data)->Sn_1F2A, MONY_BUF_SIZE );
	memmove( F12B, (*m_pSnFhyo10Data)->Sn_1F2B, MONY_BUF_SIZE );
	memmove( F12C, (*m_pSnFhyo10Data)->Sn_1F2C, MONY_BUF_SIZE );

	F1c = (*m_pSnFhyo10Data)->Sn_1F2sw;//[Y]

	// 控除過大調整税額
	memmove( F13A, (*m_pSnFhyo10Data)->Sn_1F3A, MONY_BUF_SIZE );
	memmove( F13B, (*m_pSnFhyo10Data)->Sn_1F3B, MONY_BUF_SIZE );
	memmove( F13C, (*m_pSnFhyo10Data)->Sn_1F3C, MONY_BUF_SIZE );
	m_pArith->l_add( F13T, F13A, F13B );
	m_pArith->l_add( F13T, F13T, F13C );
	if( pas == 1 ){
		memmove( F13S, F13T, MONY_BUF_SIZE );
	}

	// 控除対象仕入税額
	memmove( F14A, (*m_pSnFhyo10Data)->Sn_1F4A, MONY_BUF_SIZE );
	memmove( F14B, (*m_pSnFhyo10Data)->Sn_1F4B, MONY_BUF_SIZE );
	memmove( F14C, (*m_pSnFhyo10Data)->Sn_1F4C, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memmove( F15A, (*m_pSnFhyo10Data)->Sn_1F5A, MONY_BUF_SIZE );
	memmove( F15B, (*m_pSnFhyo10Data)->Sn_1F5B, MONY_BUF_SIZE );
	memmove( F15C, (*m_pSnFhyo10Data)->Sn_1F5C, MONY_BUF_SIZE );

	// 売上げの返還等対価に係る税額
	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		memmove( F15_1A, (*m_pSnFhyo10Data)->Sn_1F5_1A, MONY_BUF_SIZE );
		memmove( F15_1B, (*m_pSnFhyo10Data)->Sn_1F5_1B, MONY_BUF_SIZE );
		memmove( F15_1C, (*m_pSnFhyo10Data)->Sn_1F5_1C, MONY_BUF_SIZE );
	}
	else{
		memset( F15_1A, '\0', MONY_BUF_SIZE );
		memset( F15_1B, '\0', MONY_BUF_SIZE );
		memset( F15_1C, '\0', MONY_BUF_SIZE );
	}

	// 特定課税仕入れの返還等対価に係る税額
	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		memmove( F15_2C, (*m_pSnFhyo10Data)->Sn_1F5_2C, MONY_BUF_SIZE );
	}
	else{
		memset( F15_2C, '\0', MONY_BUF_SIZE );
	}

	// 貸倒に係る税額
	memmove( F16A, (*m_pSnFhyo10Data)->Sn_1F6A, MONY_BUF_SIZE );
	memmove( F16B, (*m_pSnFhyo10Data)->Sn_1F6B, MONY_BUF_SIZE );
	memmove( F16C, (*m_pSnFhyo10Data)->Sn_1F6C, MONY_BUF_SIZE );

	return 0;
}

//-----------------------------------------------------------------------------
// データの書込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo10EX::WriteData()
{
	// 課税標準額
	memmove( (*m_pSnFhyo10Data)->Sn_1F1A, F11A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F1B, F11B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F1C, F11C, MONY_BUF_SIZE );

	// 課税資産の譲渡等の対価の額
	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		memmove( (*m_pSnFhyo10Data)->Sn_1F1_1A, F11_1A, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo10Data)->Sn_1F1_1B, F11_1B, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo10Data)->Sn_1F1_1C, F11_1C, MONY_BUF_SIZE );
	}
	else{
		memmove( (*m_pSnFhyo10Data)->Sn_1F1_1A, F11A, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo10Data)->Sn_1F1_1B, F11B, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo10Data)->Sn_1F1_1C, F11C, MONY_BUF_SIZE );
	}

	// 特定課税仕入れに係る支払対価の額
	memmove( (*m_pSnFhyo10Data)->Sn_1F1_2C, F11_2C, MONY_BUF_SIZE );

	// 消費税額
	memmove( (*m_pSnFhyo10Data)->Sn_1F2A, F12A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F2B, F12B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F2C, F12C, MONY_BUF_SIZE );

	(*m_pSnFhyo10Data)->Sn_1F2sw = F1c;//[Y]

	// 控除過大調整税額
	memmove( (*m_pSnFhyo10Data)->Sn_1F3A, F13A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F3B, F13B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F3C, F13C, MONY_BUF_SIZE );

	// 控除対象仕入額
	memmove( (*m_pSnFhyo10Data)->Sn_1F4A, F14A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F4B, F14B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F4C, F14C, MONY_BUF_SIZE );

	// 返還等対価に係る税額
	memmove( (*m_pSnFhyo10Data)->Sn_1F5A, F15A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F5B, F15B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F5C, F15C, MONY_BUF_SIZE );

	// 売上げの返還等対価に係る税額
	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		memmove( (*m_pSnFhyo10Data)->Sn_1F5_1A, F15_1A, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo10Data)->Sn_1F5_1B, F15_1B, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo10Data)->Sn_1F5_1C, F15_1C, MONY_BUF_SIZE );
	}
	else{
		memmove( (*m_pSnFhyo10Data)->Sn_1F5_1A, F15A, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo10Data)->Sn_1F5_1B, F15B, MONY_BUF_SIZE );
		memmove( (*m_pSnFhyo10Data)->Sn_1F5_1C, F15C, MONY_BUF_SIZE );
	}

	// 特定課税仕入れの返還等対価に係る税額
	memmove( (*m_pSnFhyo10Data)->Sn_1F5_2C, F15_2C, MONY_BUF_SIZE );

	// 貸倒れに係る税額
	memmove( (*m_pSnFhyo10Data)->Sn_1F6A, F16A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F6B, F16B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F6C, F16C, MONY_BUF_SIZE );

	// 控除不足還付税額
	memmove( (*m_pSnFhyo10Data)->Sn_1F8A, F18A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F8B, F18B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F8C, F18C, MONY_BUF_SIZE );

	// 差引税額　　 Ｂ
	memmove( (*m_pSnFhyo10Data)->Sn_1F9A, F19A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F9B, F19B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F9C, F19C, MONY_BUF_SIZE );

	// 合計差引税額 Ｃ
	memmove( (*m_pSnFhyo10Data)->Sn_1F10T, F110T, MONY_BUF_SIZE );

	return 0;
}

//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 引数	Id		：	ダイアグラムID
//		Index	：	ダイアグラム内インデックス
//		Mony	：	表示金額
//-----------------------------------------------------------------------------
void CShinFhyo10EX::dsp_prs( unsigned short Id, short Index, char *Mony )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch( Index ){
		case ID11A	:	//  1. 課税標準額
		case ID11B	:
		case ID11C	:
		case ID11_1A:	//  1_1. 課税資産の譲渡等の対価の額
		case ID11_1B:
		case ID11_1C:
		case ID11_2C:	//	1_2.特定課税仕入れに係る支払対価の額
		case ID12A	:	//  2. 消費税額
		case ID12B	:
		case ID12C	:
		case ID13A	:	//  3. 控除過大調整税額
		case ID13B	:
		case ID13C	:
		case ID15A	:	//  5.変換等対価に係る税額
		case ID15B	:
		case ID15C	:
		case ID15_1A:	//  5_1.売上の返還等対価に係る税額
		case ID15_1B:
		case ID15_1C:
		case ID15_2C:	//	5_2.返還等対価に係る税額
		case ID16A	:	//  6. 貸倒れに係る税額
		case ID16B	:
		case ID16C	:
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
void CShinFhyo10EX::dsp_cls( unsigned short Id, short Index )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch( Index ) {
		case 4 :	// 課税期間　自
		case 5 :	// 課税期間　至
		case 3 :	// 氏名又は､名称
			diag_clear( Id, Index, TRUE, CIcsdiagctrl );
			break;
		default :	// 金額
			memset( DiagData.data_val, '\0', MONY_BUF_SIZE );
			diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			break;
	}
}

//-----------------------------------------------------------------------------
// 各入力データ抽出
//-----------------------------------------------------------------------------
void CShinFhyo10EX::GetAllDiagData()
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	//  1. 課税標準額
	diag_getdata( IDC_ICSDIAGCTRL2, ID11A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F11A, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID11B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F11B, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID11C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F11C, DiagData.data_val, MONY_BUF_SIZE ); 

	//  1_1. 課税資産の譲渡等の対価の額
	diag_getdata( IDC_ICSDIAGCTRL2, ID11_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F11_1A, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID11_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F11_1B, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID11_1C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F11_1C, DiagData.data_val, MONY_BUF_SIZE ); 

	//  1_2. 特定課税仕入れに係る支払対価の額
	diag_getdata( IDC_ICSDIAGCTRL2, ID11_2C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F11_2C, DiagData.data_val, MONY_BUF_SIZE ); 

	//  2. 消費税(税率4%分)
	diag_getdata( IDC_ICSDIAGCTRL2, ID12A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F12A, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID12B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F12B, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID12C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F12C, DiagData.data_val, MONY_BUF_SIZE ); 
	
	//  5. 返還等対価に係る税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID15A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F15A, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID15B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F15B, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID15C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F15C, DiagData.data_val, MONY_BUF_SIZE ); 

	//  5. 売上の返還等対価に係る税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID15_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F15_1A, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID15_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F15_1B, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID15_1C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F15_1C, DiagData.data_val, MONY_BUF_SIZE ); 

	//  5. 返還等対価に係る税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID15_2C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F15_2C, DiagData.data_val, MONY_BUF_SIZE ); 

	// 6. 貸倒に係る税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID16A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F16A, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID16B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F16B, DiagData.data_val, MONY_BUF_SIZE ); 
	diag_getdata( IDC_ICSDIAGCTRL2, ID16C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F16C, DiagData.data_val, MONY_BUF_SIZE ); 
}

//-----------------------------------------------------------------------------
// 付表１ 画面出力 (拡張バージョン)
//-----------------------------------------------------------------------------
void CShinFhyo10EX::DispFh1()
{
	DIAGRAM_ATTRIBUTE	DA;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 画面の初期化
	for( int i=3; i<=5; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL2, (short )i );
	}
	for( int i=ID11A; i<=ID115D; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL2, (short )i );
	}
	dsp_cls( IDC_ICSDIAGCTRL2, (short )ID116D );

	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){//連動の場合手入力サイン初期化//[Y]
		F1c = 0;//手入力サイン初期化
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
	sprintf_s( buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy&0xff), (mm&0xff), (dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 5, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	memset( buf, '\0', sizeof(buf) );
	if( (*m_pSnHeadData)->IsSoleProprietor() ){
		memmove( buf, (*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO) );
	}
	else{
		memmove( buf, (*m_pSnHeadData)->Sn_CONAM, sizeof((*m_pSnHeadData)->Sn_CONAM) );
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 課税標準額
	//char	WORK0[MONY_BUF_SIZE]={0};
	//m_Util.l_calq( WORK0, F11A, 0 );
	//dsp_prs( IDC_ICSDIAGCTRL2, ID11A, WORK0 );
	//m_Util.l_calq( WORK0, F11B, 0 );
	//dsp_prs( IDC_ICSDIAGCTRL2, ID11B, WORK0 );
	//m_Util.l_calq( WORK0, F11C, 0 );
	//dsp_prs( IDC_ICSDIAGCTRL2, ID11C, WORK0 );
	// 課税標準額計
	f1calq_3( 0 );
	f1calq_3_1( 0 );
	f1calq_3_2( 0 );

	//[Y] 消費税額
	//  ４．消費税額（4%税率分）
	f1calq_4( 0 );
	//	５．消費税額（3%税率分）
	f1calq_5( 0 );
	//	５．消費税額（6.3%税率分）
	f1calq_5_2( 0 );

	dsp_prs( IDC_ICSDIAGCTRL2, ID12A, F12A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID12B, F12B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID12C, F12C );
	// 消費税額計
	f1calq_6( 0 );

	// 控除過大調整税額 or 貸倒回収に係る税額 ・控除対象仕入税額
	f1calq_78( 0 );

	// 返還等対価に係る税額
	f1calq_9( 0 );
	f1calq_9_1( 0 );
	f1calq_9_2( 0 );

	// 貸倒に係る税額
	f1calq_10( 0 );

	// 控除税額小計
	f1calq_11( 0 );

	// 控除不足還付税額・差引税額
	f1calq_1213( 0 );

	// 合計差引税額
	f1calq_14( 0 );

	// 譲渡割額
	f1calq_15( 0 );
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFhyo10EX::InitDiagAttr()
{
	int	cnt = 0;
	while( 1 ){
		if( Fhyo1[cnt].Index == 0 ){
			break;
		}

		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Fhyo1[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Fhyo1[cnt].UnConnect;
//--> '15.08.20 INS START
				int st = ChkTkkzIdx( Fhyo1[cnt].Index );
				if( st == 1 ){
					if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
						Flg = 1;
					}
					else{
						Flg = 0;
					}
				}
				else if( st == 2 ){
					if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
						Flg = 0;
					}
					else{
						Flg = 1;
					}
				}
//<-- '15.08.20 INS END
			}
			else{
				Flg = Fhyo1[cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL2, Fhyo1[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Fhyo1[cnt].Connect );
		cnt++;
	}

	m_Fhyo10Diag02.EnableDelete(0);//DELETEキー１回で削除モード設定

	return 0;
}

BOOL CShinFhyo10EX::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			// 付表１更新
			GetAllDiagData();
			f1calq_all( 1, 0 );
			WriteData();

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
				if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
					TerminationIcsdiagctrl2( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}

	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

BEGIN_EVENTSINK_MAP(CShinFhyo10EX, CSyzBaseDlg)
	ON_EVENT(CShinFhyo10EX, IDC_ICSDIAGCTRL2, 1, CShinFhyo10EX::EditONIcsdiagctrl2, VTS_I2)
	ON_EVENT(CShinFhyo10EX, IDC_ICSDIAGCTRL2, 2, CShinFhyo10EX::EditOFFIcsdiagctrl2, VTS_I2)
	ON_EVENT(CShinFhyo10EX, IDC_ICSDIAGCTRL2, 3, CShinFhyo10EX::TerminationIcsdiagctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo10EX::EditONIcsdiagctrl2(short index)
{
	m_curwnd = IDC_ICSDIAGCTRL2;
}

void CShinFhyo10EX::EditOFFIcsdiagctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char				CkMy[MONY_BUF_SIZE];
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	switch( index ){

		case ID11A	:	//  1.A 課税標準額(税率3%分)

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F11A, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID11A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F11A, CkMy) ){

//--> '15.08.24 CUT START
//				F0c |= 0x08;
//				F0c &= ~0x01;
//<-- '15.08.24 CUT END

				if(!(F1c&0x08)){//[Y]手入力されていない場合のみサインたてる
					F1c |= 0x01;
				}
			}
			memmove( F11A, CkMy, MONY_BUF_SIZE );

			// 1000円未満切り捨て
			m_Util.l_calq( F11A, CkMy, 0 );
			memmove( DiagData.data_val, F11A, MONY_BUF_SIZE ); 
			diag_setdata( IDC_ICSDIAGCTRL2, ID11A, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			// 各入力の計算表示
			f1calq_all( 1, 0 );

			break;

		case ID11B	:	//  1.B 課税標準額(税率4%分)

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F11B, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID11B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F11B, CkMy) ){

//--> '15.08.24 CUT START
//				F0c |= 0x80;
//				F0c &= ~0x10;
//<-- '15.08.24 CUT END

				if(!(F1c&0x80)){//[Y]手入力されていない場合のみサインたてる
					F1c |= 0x10;
				}
			}
			memmove( F11B, CkMy, MONY_BUF_SIZE );

			// 1000円未満切り捨て
			m_Util.l_calq( F11B, CkMy, 0 );
			memmove( DiagData.data_val, F11B, MONY_BUF_SIZE ); 
			diag_setdata( IDC_ICSDIAGCTRL2, ID11B, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			// 各入力の計算表示
			f1calq_all( 1, 0 );

			break;

		case ID11C	:	//  1.C 課税標準額(税率6.3%分)

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F11C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID11C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F11C, CkMy) ){

//--> '15.08.24 CUT START
//				F0c |= 0x800;
//				F0c &= ~0x100;
//<-- '15.08.24 CUT END

				if(!(F1c&0x800)){//[Y]手入力されていない場合のみサインたてる
					F1c |= 0x100;
				}
			}
			memmove( F11C, CkMy, MONY_BUF_SIZE );

			// 1000円未満切り捨て
			m_Util.l_calq( F11C, CkMy, 0 );
			memmove( DiagData.data_val, F11C, MONY_BUF_SIZE ); 
			diag_setdata( IDC_ICSDIAGCTRL2, ID11C, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			// 各入力の計算表示
			f1calq_all( 1, 0 );

			break;

		case ID11_1A	:	//  1_1.A 課税資産の譲渡等の対価の額(税率3%分)

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F11_1A, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID11_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F11_1A, CkMy) ){
				if(!(F0c&0x08)){//[Y]手入力されていない場合のみサインたてる
					F0c |= 0x01;
				}
				if(!(F1c&0x08)){
					F1c |= 0x01;
				}
			}
			memmove( F11_1A, CkMy, MONY_BUF_SIZE );

			memmove( DiagData.data_val, F11_1A, MONY_BUF_SIZE ); 
			diag_setdata( IDC_ICSDIAGCTRL2, ID11_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			// 各入力の計算表示
			f1calq_all( 1, 0 );

			break;

		case ID11_1B	:	//  1_1.B 課税資産の譲渡等の対価の額(税率5%分)

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F11_1B, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID11_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F11_1B, CkMy) ){
				if(!(F0c&0x80)){//[Y]手入力されていない場合のみサインたてる
					F0c |= 0x10;
				}
				if(!(F1c&0x80)){
					F1c |= 0x10;
				}
			}
			memmove( F11_1B, CkMy, MONY_BUF_SIZE );

			memmove( DiagData.data_val, F11_1B, MONY_BUF_SIZE ); 
			diag_setdata( IDC_ICSDIAGCTRL2, ID11_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			// 各入力の計算表示
			f1calq_all( 1, 0 );

			break;

		case ID11_1C	:	//  1_1.A 課税資産の譲渡等の対価の額(税率8%分)

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F11_1C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID11_1C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F11_1C, CkMy) ){
				if(!(F0c&0x800)){//[Y]手入力されていない場合のみサインたてる
					F0c |= 0x100;
				}
				if(!(F1c&0x800)){
					F1c |= 0x100;
				}
			}
			memmove( F11_1C, CkMy, MONY_BUF_SIZE );

			memmove( DiagData.data_val, F11_1C, MONY_BUF_SIZE ); 
			diag_setdata( IDC_ICSDIAGCTRL2, ID11_1C, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			// 各入力の計算表示
			f1calq_all( 1, 0 );

			break;

		case ID11_2C	:	//  1_2.C 特定課税仕入れに係る支払対価の額(税率6.3%分)

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F11_2C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID11_2C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F11_2C, CkMy) ){
				if(!(F0c&0x800)){//[Y]手入力されていない場合のみサインたてる
					F0c |= 0x100;
				}
				if(!(F1c&0x800)){
					F1c |= 0x100;
				}
			}
			memmove( F11_2C, CkMy, MONY_BUF_SIZE );

			memmove( DiagData.data_val, F11_2C, MONY_BUF_SIZE ); 
			diag_setdata( IDC_ICSDIAGCTRL2, ID11_2C, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			// 各入力の計算表示
			f1calq_all( 1, 0 );

			break;

		case ID12A	:	//  2.A 消費税(税率3%分)

			//[Y]
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F12A, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID12A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F12A, CkMy) ){
				F1c |= 0x08;
				F1c &= ~0x01;
			}
			memmove( F12A, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f1calq_all( 2, 0 );
			break;

		case ID12B	:	//  2.B 消費税(税率4%分)

			//[Y]
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F12B, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID12B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F12B, CkMy) ){
				F1c |= 0x80;
				F1c &= ~0x10;
			}
			memmove( F12B, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f1calq_all( 2, 0 );

			break;

		case ID12C	:	//  2.B 消費税(税率6.3%分)

			//[Y]
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F12C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID12C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F12C, CkMy) ){
				F1c |= 0x800;
				F1c &= ~0x100;
			}
			memmove( F12C, DiagData.data_val, MONY_BUF_SIZE ); 
			
			// 各入力の計算表示
			f1calq_all( 2, 0 );

			break;

		case ID15A	:	//  5.A 返還等対価に係る税額

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F15A, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID15A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F15A, DiagData.data_val, MONY_BUF_SIZE ); 

			if( m_pArith->l_cmp(F15A, CkMy) ){
				F2c |= 0x08;
				F2c &= ~0x01;
			}

			// 各入力の計算表示
			f1calq_all( 5, 0 );
			break;

		case ID15B	:	//  5.B 返還等対価に係る税額

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F15B, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID15B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F15B, DiagData.data_val, MONY_BUF_SIZE ); 

			if( m_pArith->l_cmp(F15B, CkMy) ){
				F2c |= 0x80;
				F2c &= ~0x10;
			}

			// 各入力の計算表示
			f1calq_all( 6, 0 );//[Y]
			break;

		case ID15C	:	//  5.C 返還等対価に係る税額

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F15C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID15C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F15C, DiagData.data_val, MONY_BUF_SIZE ); 

			if( m_pArith->l_cmp(F15C, CkMy) ){
				F2c |= 0x800;
				F2c &= ~0x100;
			}

			// 各入力の計算表示
			f1calq_all( 6, 0 );//[Y]
			break;

		case ID15_1A	:	//  15_1.A 売上の返還等対価に係る税額

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F15_1A, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID15_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F15_1A, CkMy) ){
				if(!(F2c&0x08)){//[Y]手入力されていない場合のみサインたてる
					F2c |= 0x01;
				}
			}
			memmove( F15_1A, CkMy, MONY_BUF_SIZE );

			memmove( DiagData.data_val, F15_1A, MONY_BUF_SIZE ); 
			diag_setdata( IDC_ICSDIAGCTRL2, ID15_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			// 各入力の計算表示
			f1calq_all( 6, 0 );

			break;

		case ID15_1B	:	//  15_1.B 売上の返還等対価に係る税額

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F15_1B, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID15_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F15_1B, CkMy) ){
				if(!(F2c&0x80)){//[Y]手入力されていない場合のみサインたてる
					F2c |= 0x10;
				}
			}
			memmove( F15_1B, CkMy, MONY_BUF_SIZE );

			memmove( DiagData.data_val, F15_1B, MONY_BUF_SIZE ); 
			diag_setdata( IDC_ICSDIAGCTRL2, ID15_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			// 各入力の計算表示
			f1calq_all( 6, 0 );

			break;

		case ID15_1C	:	//  15_1.C 売上の返還等対価に係る税額

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F15_1C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID15_1C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F15_1C, CkMy) ){
				if(!(F2c&0x800)){//[Y]手入力されていない場合のみサインたてる
					F2c |= 0x100;
				}
			}
			memmove( F15_1C, CkMy, MONY_BUF_SIZE );

			memmove( DiagData.data_val, F15_1C, MONY_BUF_SIZE ); 
			diag_setdata( IDC_ICSDIAGCTRL2, ID15_1C, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			// 各入力の計算表示
			f1calq_all( 6, 0 );

			break;

		case ID15_2C	:	//  15_1.A 売上の返還等対価に係る税額

			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F15_2C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID15_2C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F15_2C, CkMy) ){
				if(!(F2c&0x800)){//[Y]手入力されていない場合のみサインたてる
					F2c |= 0x100;
				}
			}
			memmove( F15_2C, CkMy, MONY_BUF_SIZE );

			memmove( DiagData.data_val, F15_2C, MONY_BUF_SIZE ); 
			diag_setdata( IDC_ICSDIAGCTRL2, ID15_2C, (struct IUnknown *)&DiagData, CIcsdiagctrl );

			// 各入力の計算表示
			f1calq_all( 6, 0 );

			break;

		case ID16A	:	// 6.A 貸倒に係る税額

			diag_getdata( IDC_ICSDIAGCTRL2, ID16A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F16A, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f1calq_all( 7, 0 );
			break;

		case ID16B	:	// 6.B 貸倒に係る税額

			diag_getdata( IDC_ICSDIAGCTRL2, ID16B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F16B, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f1calq_all( 8, 0 );
			break;
			
		case ID16C	:	// 6.C 貸倒に係る税額

			diag_getdata( IDC_ICSDIAGCTRL2, ID16C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F16C, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f1calq_all( 8, 0 );
			break;

		default		:
			break;
	}
}

void CShinFhyo10EX::TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}

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
		pos = UpDownFocusMoveCheck(index , 1);
		m_Fhyo10Diag02.SetPosition(pos);
	}
	if( wTerm==VK_DOWN ){
		pos = UpDownFocusMoveCheck(index , 0);
		m_Fhyo10Diag02.SetPosition(pos);
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
		m_Fhyo10Diag02.SetNextPosition();
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		m_Fhyo10Diag02.SetPrevPosition();
	}

	//[Y]
	if( wTerm == VK_DELETE ){
		switch( index ){
//--> '15.08.24 CUT START
//			case ID11A	:	//  1.A 課税標準額(税率3%分)
//				F0c &= ~0x08;
//				F0c |= 0x01;
//				// 各入力の計算表示
//				f1calq_all( 1, 0 );
//				m_Fhyo10Diag02.SetPosition( ID11A );
//				break;
//
//			case ID11B	:	//  1.B 課税標準額(税率4%分)
//				F0c &= ~0x80;
//				F0c |= 0x10;
//				// 各入力の計算表示
//				f1calq_all( 1, 0 );
//				m_Fhyo10Diag02.SetPosition( ID11B );
//				break;
//
//			case ID11C	:	//  1.C 課税標準額(税率6.3%分)
//				F0c &= ~0x800;
//				F0c |= 0x100;
//				// 各入力の計算表示
//				f1calq_all( 1, 0 );
//				m_Fhyo10Diag02.SetPosition( ID11C );
//				break;
//<-- '15.08.24 CUT END

			case ID12A	:	//  2.A 消費税(税率3%分)
				F1c &= ~0x08;
				F1c |= 0x01;
				// 各入力の計算表示
				f1calq_all( 2, 0 );
				m_Fhyo10Diag02.SetPosition( ID12A );
				break;

			case ID12B	:	//  2.B 消費税(税率4%分)
				F1c &= ~0x80;
				F1c |= 0x10;
				// 各入力の計算表示
				f1calq_all( 2, 0 );
				m_Fhyo10Diag02.SetPosition( ID12B );
				break;

			case ID12C	:	//  2.B 消費税(税率6.3%分)
				F1c &= ~0x800;
				F1c |= 0x100;
				// 各入力の計算表示
				f1calq_all( 2, 0 );
				m_Fhyo10Diag02.SetPosition( ID12C );
				break;

			case ID15A	:	//  5.A 返還等対価に係る税額(税率3%分)
				F2c &= ~0x08;
				F2c |= 0x01;
				// 各入力の計算表示
				f1calq_all( 6, 0 );
				m_Fhyo10Diag02.SetPosition( ID15A );
				break;

			case ID15B	:	//  5.B 返還等対価に係る税額(税率4%分)
				F2c &= ~0x80;
				F2c |= 0x10;
				// 各入力の計算表示
				f1calq_all( 6, 0 );
				m_Fhyo10Diag02.SetPosition( ID15B );
				break;

			case ID15C	:	//  5.B 返還等対価に係る税額(税率6.3%分)
				F2c &= ~0x800;
				F2c |= 0x100;
				// 各入力の計算表示
				f1calq_all( 6, 0 );
				m_Fhyo10Diag02.SetPosition( ID15C );
				break;

			default :
				dsp_cls(IDC_ICSDIAGCTRL2 , index);//[Y]
				break;
		}
	}

	int nowpos = m_Fhyo10Diag02.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL2, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
}

//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinFhyo10EX::UpDownFocusMoveCheck(int Index , int Move)
{
	int idx = 0;
	int i = 0 , j = 0;

	DIAGRAM_ATTRIBUTE	DA;

	//現在ポジション検索
	for(i = 0 ; i < FCS_FHYO10CNT ; i++){
		if(Index == FCS_Fhyo10[i].IDNo){
			break;
		}
	}

	idx = FCS_Fhyo10[i].IDNo;//現在ポジションセット

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_FHYO10CNT ; j++){
			m_Fhyo10Diag02.GetAttribute(FCS_Fhyo10[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo10[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Fhyo10Diag02.GetAttribute(FCS_Fhyo10[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo10[j].IDNo;
				break;
			}
		}
	}

	return idx;
}

//-----------------------------------------------------------------------------
//[Y] 入力項目色変え
//-----------------------------------------------------------------------------
// 引数	id		；	ダイアグラムID
//		index	：	ダイアグラム内インデックス
//		sign	：	変更サイン
//-----------------------------------------------------------------------------
void CShinFhyo10EX::ChangeColor( unsigned short id, short Index, int sign )
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

//-----------------------------------------------------------------------------
// 特定課税仕入に関連するインデックス？
//-----------------------------------------------------------------------------
// 引数		idx	：	チェック対象
//-----------------------------------------------------------------------------
// 返送値	0	：	特定課税仕入に関連しない
//			1	：	特定課税仕入で入力可能になるインデックス
//			2	：	特定課税仕入で入力不可になるインデックス
//-----------------------------------------------------------------------------
int CShinFhyo10EX::ChkTkkzIdx( short idx )
{
	int rSt = 0;

	if( ((ID11_1A<=idx)&&(idx<=ID11_2D)) || ((ID15_1A<=idx)&&(idx<=ID15_2D)) ){
		rSt = 1;
	}
	else if( ((ID11A<=idx)&&(idx<=ID11D)) || ((ID15A<=idx)&&(idx<=ID15D)) ){
		rSt = 2;
	}

	return rSt;
}

void CShinFhyo10EX::SetEditFlg()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char				CkMy[MONY_BUF_SIZE];
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	//  1.A 課税標準額(税率3%分)
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID11A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F11A, CkMy) ){
		if(!(F1c&0x08)){//[Y]手入力されていない場合のみサインたてる
			F1c |= 0x01;
		}
	}

	//  1.B 課税標準額(税率4%分)
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID11B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F11B, CkMy) ){
		if(!(F1c&0x80)){//[Y]手入力されていない場合のみサインたてる
			F1c |= 0x10;
		}
	}

	//  1.C 課税標準額(税率6.3%分)
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID11C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F11C, CkMy) ){
		if(!(F1c&0x800)){//[Y]手入力されていない場合のみサインたてる
			F1c |= 0x100;
		}
	}

	//  1_1.A 課税資産の譲渡等の対価の額(税率3%分)
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID11_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F11_1A, CkMy) ){
		if(!(F0c&0x08)){//[Y]手入力されていない場合のみサインたてる
			F0c |= 0x01;
		}
		if(!(F1c&0x08)){
			F1c |= 0x01;
		}
	}

	//  1_1.B 課税資産の譲渡等の対価の額(税率5%分)
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID11_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F11_1B, CkMy) ){
		if(!(F0c&0x80)){//[Y]手入力されていない場合のみサインたてる
			F0c |= 0x10;
		}
		if(!(F1c&0x80)){
			F1c |= 0x10;
		}
	}

	//  1_1.A 課税資産の譲渡等の対価の額(税率8%分)
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID11_1C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F11_1C, CkMy) ){
		if(!(F0c&0x800)){//[Y]手入力されていない場合のみサインたてる
			F0c |= 0x100;
		}
		if(!(F1c&0x800)){
			F1c |= 0x100;
		}
	}

	//  1_2.C 特定課税仕入れに係る支払対価の額(税率6.3%分)
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID11_2C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F11_2C, CkMy) ){
		if(!(F0c&0x800)){//[Y]手入力されていない場合のみサインたてる
			F0c |= 0x100;
		}
		if(!(F1c&0x800)){
			F1c |= 0x100;
		}
	}

	//  2.A 消費税(税率3%分)
	//[Y]
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID12A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F12A, CkMy) ){
		F1c |= 0x08;
		F1c &= ~0x01;
	}

	//  2.B 消費税(税率4%分)
	//[Y]
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID12B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F12B, CkMy) ){
		F1c |= 0x80;
		F1c &= ~0x10;
	}

	//  2.B 消費税(税率6.3%分)
	//[Y]
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID12C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F12C, CkMy) ){
		F1c |= 0x800;
		F1c &= ~0x100;
	}

	//  5.A 返還等対価に係る税額
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID15A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F15A, CkMy) ){
		F2c |= 0x08;
		F2c &= ~0x01;
	}

	//  5.B 返還等対価に係る税額
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID15B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F15B, CkMy) ){
		F2c |= 0x80;
		F2c &= ~0x10;
	}

	//  5.C 返還等対価に係る税額
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID15C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F15C, CkMy) ){
		F2c |= 0x800;
		F2c &= ~0x100;
	}

	//  15_1.A 売上の返還等対価に係る税額
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID15_1A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F15_1A, CkMy) ){
		if(!(F2c&0x08)){//[Y]手入力されていない場合のみサインたてる
			F2c |= 0x01;
		}
	}

	//  15_1.B 売上の返還等対価に係る税額
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID15_1B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F15_1B, CkMy) ){
		if(!(F2c&0x80)){//[Y]手入力されていない場合のみサインたてる
			F2c |= 0x10;
		}
	}

	//  15_1.C 売上の返還等対価に係る税額
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID15_1C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F15_1C, CkMy) ){
		if(!(F2c&0x800)){//[Y]手入力されていない場合のみサインたてる
			F2c |= 0x100;
		}
	}

	//  15_1.A 売上の返還等対価に係る税額
	memset( CkMy, '\0', MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAGCTRL2, ID15_2C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
	if( m_pArith->l_cmp(F15_2C, CkMy) ){
		if(!(F2c&0x800)){//[Y]手入力されていない場合のみサインたてる
			F2c |= 0x100;
		}
	}
}

//-----------------------------------------------------------------------------
// 特定課税仕入に関する項目の金額クリア('15.11.13)
//-----------------------------------------------------------------------------
void CShinFhyo10EX::ClearTkkzMony()
{
	m_pArith->l_clear( F11_2A );
	m_pArith->l_clear( F11_2B );
	m_pArith->l_clear( F11_2C );
	m_pArith->l_clear( F11_2T );
	memmove( F11A, F11_1A, MONY_BUF_SIZE );
	memmove( F11B, F11_1B, MONY_BUF_SIZE );
	memmove( F11C, F11_1C, MONY_BUF_SIZE );
	memmove( F11T, F11_1T, MONY_BUF_SIZE );
	F1c |= 0x111;	// 消費税額 自動計算

	m_pArith->l_clear( F15_2A );
	m_pArith->l_clear( F15_2B );
	m_pArith->l_clear( F15_2C );
	m_pArith->l_clear( F15_2T );
	memmove( F15A, F15_1A, MONY_BUF_SIZE );
	memmove( F15B, F15_1B, MONY_BUF_SIZE );
	memmove( F15C, F15_1C, MONY_BUF_SIZE );
	memmove( F15T, F15_1T, MONY_BUF_SIZE );

	DispFh1();

	WriteData();
}
