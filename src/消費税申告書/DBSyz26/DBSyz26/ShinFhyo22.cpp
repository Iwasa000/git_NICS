// ShinFhyo22.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinFhyo22.h"
#include "ShinFhyo22Idx.h"
#include "H26HyoView.h"	// '15.02.16

#include "ChangeTax.h"
#include "ChangeTaxEx.h"

// CShinFhyo22 ダイアログ

IMPLEMENT_DYNAMIC(CShinFhyo22, CSyzBaseDlg)

extern	HINSTANCE g_hInstance;

CShinFhyo22::CShinFhyo22(CWnd* pParent /*=NULL*/)
	: CSyzBaseDlg(CShinFhyo22::IDD, pParent)
	, m_pSnFhyo22Data(NULL)
	, m_pSnHeadData(NULL)
	, m_pSnTanaData(NULL)
	, m_pParent(pParent)
	, m_FrSw(0)
	, m_DpSw(0)
	, m_DlSw(0)
	, m_pSnHonpyoData(NULL)
{
	memset( F221A, '\0', sizeof(F221A) );
	memset( F221B, '\0', sizeof(F221B) );
	memset( F221C, '\0', sizeof(F221C) );
	memset( F222A, '\0', sizeof(F222A) );
	memset( F222B, '\0', sizeof(F222B) );
	memset( F222C, '\0', sizeof(F222C) );
	memset( F223A, '\0', sizeof(F223A) );
	memset( F223B, '\0', sizeof(F223B) );
	memset( F223C, '\0', sizeof(F223C) );
	memset( F224C, '\0', sizeof(F224C) );
	memset( F226A, '\0', sizeof(F226A) );
	memset( F226B, '\0', sizeof(F226B) );
	memset( F226C, '\0', sizeof(F226C) );
	memset( F227C, '\0', sizeof(F227C) );
	memset( F228A, '\0', sizeof(F228A) );
	memset( F228B, '\0', sizeof(F228B) );
	memset( F228C, '\0', sizeof(F228C) );
	memset( F229A, '\0', sizeof(F229A) );
	memset( F229B, '\0', sizeof(F229B) );
	memset( F229C, '\0', sizeof(F229C) );
	memset( F2210A, '\0', sizeof(F2210A) );
	memset( F2210B, '\0', sizeof(F2210B) );
	memset( F2210C, '\0', sizeof(F2210C) );
	memset( F2211A, '\0', sizeof(F2211A) );
	memset( F2211B, '\0', sizeof(F2211B) );
	memset( F2211C, '\0', sizeof(F2211C) );
	memset( F2212A, '\0', sizeof(F2212A) );
	memset( F2212B, '\0', sizeof(F2212B) );
	memset( F2212C, '\0', sizeof(F2212C) );
	memset( F2213A, '\0', sizeof(F2213A) );
	memset( F2213B, '\0', sizeof(F2213B) );
	memset( F2213C, '\0', sizeof(F2213C) );
	memset( F2215A, '\0', sizeof(F2215A) );
	memset( F2215B, '\0', sizeof(F2215B) );
	memset( F2215C, '\0', sizeof(F2215C) );
	memset( F2216A, '\0', sizeof(F2216A) );
	memset( F2216B, '\0', sizeof(F2216B) );
	memset( F2216C, '\0', sizeof(F2216C) );
	memset( F2215AEX, '\0', sizeof(F2215AEX) );
	memset( F2215BEX, '\0', sizeof(F2215BEX) );
	memset( F2215CEX, '\0', sizeof(F2215CEX) );
	memset( F2216AEX, '\0', sizeof(F2216AEX) );
	memset( F2216BEX, '\0', sizeof(F2216BEX) );
	memset( F2216CEX, '\0', sizeof(F2216CEX) );
	memset( F2217A, '\0', sizeof(F2217A) );
	memset( F2217B, '\0', sizeof(F2217B) );
	memset( F2217C, '\0', sizeof(F2217C) );
	memset( F2218A, '\0', sizeof(F2218A) );
	memset( F2218B, '\0', sizeof(F2218B) );
	memset( F2218C, '\0', sizeof(F2218C) );
	memset( F2219A, '\0', sizeof(F2219A) );
	memset( F2219B, '\0', sizeof(F2219B) );
	memset( F2219C, '\0', sizeof(F2219C) );
	memset( F2220A, '\0', sizeof(F2220A) );
	memset( F2220B, '\0', sizeof(F2220B) );
	memset( F2220C, '\0', sizeof(F2220C) );
	memset( F2221A, '\0', sizeof(F2221A) );
	memset( F2221B, '\0', sizeof(F2221B) );
	memset( F2221C, '\0', sizeof(F2221C) );
	memset( F2222A, '\0', sizeof(F2222A) );
	memset( F2222B, '\0', sizeof(F2222B) );
	memset( F2222C, '\0', sizeof(F2222C) );
	memset( F2223A, '\0', sizeof(F2223A) );
	memset( F2223B, '\0', sizeof(F2223B) );
	memset( F2223C, '\0', sizeof(F2223C) );

	memset( FS23A, '\0', sizeof(FS23A) );
	memset( FS23B, '\0', sizeof(FS23B) );
	memset( FS23C, '\0', sizeof(FS23C) );


	F22UWS = 0;
	F22UWSEX = 0;

	F229c = F2211c = F2217c = F2218c = F2221c = 0;

	m_curwnd = -1;
}

CShinFhyo22::~CShinFhyo22()
{
}

void CShinFhyo22::DoDataExchange(CDataExchange* pDX)
{
	CSyzBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAGCTRL1, m_Fhyo22Diag01);
	DDX_Control(pDX, IDC_ICSDIAGCTRL2, m_Fhyo22Diag02);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
}


BEGIN_MESSAGE_MAP(CShinFhyo22, CSyzBaseDlg)
	ON_BN_CLICKED(IDC_BUTTON1, &CShinFhyo22::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinFhyo22::OnBnClickedButton2)
END_MESSAGE_MAP()


// CShinFhyo22 メッセージ ハンドラ

BOOL CShinFhyo22::OnInitDialog()
{
	CSyzBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期化
	Init();

	// 基底クラスの初期化
	m_pBottomDiag = &m_Fhyo22Diag02;
	m_BottomIdx = 60;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSnFhyo22Data	：	付表２－２データ読込みクラス
//		pSnHeadData		：	本表ヘッダデータ読込みクラス
//		pSnTanaData		：	棚卸調整データ読込みクラス
//		pSnFhyo10Data	：	付表１データ読込みクラス
//		pSnHonpyoData	：	本表データ読込みクラス
//		pArith			：	倍長演算クラス
//		pShinInfo		：	共通情報
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo22::InitInfo( CH26SnFhyo20Data **pSnFhyo22Data, CSnHeadData **pSnHeadData, CH26SnTanaData **pSnTanaData, CH26SnFhyo10Data **pSnFhyo10Data, CH26SnHonpyoData **pSnHonpyoData, CArithEx *pArith, SYC_SHININFO *pShinInfo )
{
	ASSERT( pSnFhyo22Data );
	if( pSnFhyo22Data == NULL ){
		return -1;
	}
	m_pSnFhyo22Data = pSnFhyo22Data;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pSnTanaData );
	if( pSnTanaData == NULL ){
		return -1;
	}
	m_pSnTanaData = pSnTanaData;

	ASSERT( pSnFhyo10Data );
	if( pSnFhyo10Data == NULL ){
		return -1;
	}
	m_pSnFhyo10Data = pSnFhyo10Data;

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
int CShinFhyo22::Init()
{
//--> '14.05.16 INS START
	if( ((*m_pSnFhyo22Data)==NULL) || ((*m_pSnTanaData)==NULL) || ((*m_pSnFhyo10Data)==NULL) ){
		return -1;
	}
//<-- '14.05.16 INS END

	ReadData( m_FrSw );
	m_FrSw = 1;

	InitDiagAttr();

	DispFh22();

	ChangeBtnDisp();

	//-->削除 harino ('14.03.06)
	// 課税標準率Ａ欄へ
	//if( m_pSnHeadData->Sn_Sign4&0x80 ){
	//	diag_setposition( IDC_ICSDIAGCTRL2, ID221A, CIcsdiagctrl );
	//}
	//<--削除 harino ('14.03.06)

	return 0;
}

//-----------------------------------------------------------------------------
// ボタン表示変更
//-----------------------------------------------------------------------------
void CShinFhyo22::ChangeBtnDisp()
{
	if( ((*m_pSnHeadData)->m_apno&0xf0) >= 0x50 ){
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			m_Button1.EnableWindow( FALSE );
//--> '15.04.25 CUT START
//			m_Button2.EnableWindow( FALSE );
//<-- '15.04.25 CUT END
		}
		else{
			m_Button1.EnableWindow( TRUE );
//-- '15.04.04 --
//			if( m_pShinInfo->sgSpc && !((*m_pSnHeadData)->Sn_Sign2&0x02) ){
//---------------
			if( *m_pShinInfo->pSgSpc && !((*m_pSnHeadData)->Sn_Sign2&0x02) ){
//---------------
				m_Button2.ShowWindow( SW_SHOW );
			}
			else{
				m_Button2.ShowWindow( SW_HIDE );
			}
		}
	}
	else{
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			m_Button1.EnableWindow( FALSE );
		}
		else{
			m_Button1.EnableWindow( TRUE );
		}
//--> '15.04.25 INS START
		m_Button2.EnableWindow( FALSE );
//<-- '15.04.25 INS END
		m_Button2.ShowWindow( SW_HIDE );
	}
}

//-----------------------------------------------------------------------------
// ダイアグラム初期属性セット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CShinFhyo22::InitDiagAttr()
{
	int	cnt = 0;
	while( 1 ){
		if( Fhyo22[cnt].Index == 0 ){
			break;
		}

		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Fhyo22[cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Fhyo22[cnt].UnConnect;
			}
			else{
				Flg = Fhyo22[cnt].Connect;
			}
		}
//-- '15.04.04 --
//		if( ((*m_pSnHeadData)->Sn_EXP100&0x01) && m_pShinInfo->sgSpc && (Fhyo22[cnt].Index==ID227EX) ){
//---------------
		if( ((*m_pSnHeadData)->Sn_EXP100&0x01) && *m_pShinInfo->pSgSpc && (Fhyo22[cnt].Index==ID227EX) ){
//---------------
			Flg = 0;
		}
		ATRIB_MOD( IDC_ICSDIAGCTRL2, Fhyo22[cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Fhyo22[cnt].Connect );
		cnt++;
	}

	m_Fhyo22Diag02.EnableDelete(0);//[Y]DELETEキー１回で削除モード設定

	return 0;
}


//-----------------------------------------------------------------------------
// データの読込み
//-----------------------------------------------------------------------------
// 引数		pas	：
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo22::ReadData( int pas )
{
	// 課税標準額
	memmove( F221A, (*m_pSnFhyo22Data)->Sn_2F1A, MONY_BUF_SIZE );
	memmove( F221B, (*m_pSnFhyo22Data)->Sn_2F1B, MONY_BUF_SIZE );
	memmove( F221C, (*m_pSnFhyo22Data)->Sn_2F1C, MONY_BUF_SIZE );

	// 免税売上額
	memmove( F222C, (*m_pSnFhyo22Data)->Sn_2F2C, MONY_BUF_SIZE );
	// 非課税資産の輸出
	memmove( F223C, (*m_pSnFhyo22Data)->Sn_2F3C, MONY_BUF_SIZE );
	// 非課税売上額
	memmove( F226C, (*m_pSnFhyo22Data)->Sn_2F6C, MONY_BUF_SIZE );
	// 課税売上割合
//-- '15.04.04 --
//	F22UWS = (*m_pSnFhyo22Data)->Sn_2FUWS;
//---------------
	F22UWS = (unsigned short)(*m_pSnFhyo22Data)->Sn_2FUWS;
//---------------
	// 課税仕入に係る支払対価の額
	memmove( F228A, (*m_pSnFhyo22Data)->Sn_2F8A, MONY_BUF_SIZE );
	memmove( F228B, (*m_pSnFhyo22Data)->Sn_2F8B, MONY_BUF_SIZE );
	memmove( F228C, (*m_pSnFhyo22Data)->Sn_2F8C, MONY_BUF_SIZE );
	// 課税仕入に係る消費税額
	memmove( F229A, (*m_pSnFhyo22Data)->Sn_2F9A, MONY_BUF_SIZE );
	memmove( F229B, (*m_pSnFhyo22Data)->Sn_2F9B, MONY_BUF_SIZE );
	memmove( F229C, (*m_pSnFhyo22Data)->Sn_2F9C, MONY_BUF_SIZE );

	F229c = (*m_pSnFhyo22Data)->Sn_2F9sw;//[Y]

	// 課税貨物に係る消費税額
	memmove( F2211A, (*m_pSnFhyo22Data)->Sn_2F10A, MONY_BUF_SIZE );
	memmove( F2211B, (*m_pSnFhyo22Data)->Sn_2F10B, MONY_BUF_SIZE );
	memmove( F2211C, (*m_pSnFhyo22Data)->Sn_2F10C, MONY_BUF_SIZE );
	// (免税)控除対象調整税額
	if( (*m_pSnTanaData)->Sn_MENZsw & 0x10 ){

		//[Y]
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){//個別対応時
			char	WORK0[MONY_BUF_SIZE] = {0};
			m_pArith->l_add( F2212A , (*m_pSnTanaData)->Sn_menzz[0] , (*m_pSnTanaData)->Sn_menzz[1] );
			m_pArith->l_add( F2212A , F2212A , (*m_pSnTanaData)->Sn_menzz[2] );
			m_pArith->l_add( F2212B , (*m_pSnTanaData)->Sn_mnz40[0] , (*m_pSnTanaData)->Sn_mnz40[1] );
			m_pArith->l_add( F2212B , F2212B , (*m_pSnTanaData)->Sn_mnz40[2] );
			m_pArith->l_add( F2212C , (*m_pSnTanaData)->Sn_mnz63[0] , (*m_pSnTanaData)->Sn_mnz63[1] );
			m_pArith->l_add( F2212C , F2212C , (*m_pSnTanaData)->Sn_mnz63[2] );

		}
		else{//比例配分
			memmove( F2212A, (*m_pSnTanaData)->Sn_MENZZ, MONY_BUF_SIZE );
			memmove( F2212B, (*m_pSnTanaData)->Sn_MNZ40, MONY_BUF_SIZE );
			memmove( F2212C, (*m_pSnTanaData)->Sn_MNZ63, MONY_BUF_SIZE );
		}
		
		if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ){
			m_pArith->l_neg( F2212A );
			m_pArith->l_neg( F2212B );
			m_pArith->l_neg( F2212C );
		}
	}
	else{
		memset( F2212A, '\0', MONY_BUF_SIZE );
		memset( F2212B, '\0', MONY_BUF_SIZE );
		memset( F2212C, '\0', MONY_BUF_SIZE );
	}
	// 課税売上に係る仕入税額
	memmove( F2215A, (*m_pSnFhyo22Data)->Sn_2F15A, MONY_BUF_SIZE );
	memmove( F2215B, (*m_pSnFhyo22Data)->Sn_2F15B, MONY_BUF_SIZE );
	memmove( F2215C, (*m_pSnFhyo22Data)->Sn_2F15C, MONY_BUF_SIZE );
	// 共通の売上に係る仕入税額
	memmove( F2216A, (*m_pSnFhyo22Data)->Sn_2F16A, MONY_BUF_SIZE );
	memmove( F2216B, (*m_pSnFhyo22Data)->Sn_2F16B, MONY_BUF_SIZE );
	memmove( F2216C, (*m_pSnFhyo22Data)->Sn_2F16C, MONY_BUF_SIZE );

	// 課税売上に係る仕入税額
	memmove( F2215AEX, (*m_pSnFhyo22Data)->Sn_2F15A, MONY_BUF_SIZE );
	memmove( F2215BEX, (*m_pSnFhyo22Data)->Sn_2F15B, MONY_BUF_SIZE );
	memmove( F2215CEX, (*m_pSnFhyo22Data)->Sn_2F15C, MONY_BUF_SIZE );
	// 共通の売上に係る仕入税額
	memmove( F2216AEX, (*m_pSnFhyo22Data)->Sn_2F16A, MONY_BUF_SIZE );
	memmove( F2216BEX, (*m_pSnFhyo22Data)->Sn_2F16B, MONY_BUF_SIZE );
	memmove( F2216CEX, (*m_pSnFhyo22Data)->Sn_2F16C, MONY_BUF_SIZE );

	F2217c = F2218c = 0x00;
	// 個別対応方式控除仕入税額
	if( m_pArith->l_test((*m_pSnFhyo22Data)->Sn_2F17A) ){
		memmove( F2217A, (*m_pSnFhyo22Data)->Sn_2F17A, MONY_BUF_SIZE );
		F2217c |= 0x10;
	}
	if( m_pArith->l_test((*m_pSnFhyo22Data)->Sn_2F17B) ){
		memmove( F2217B, (*m_pSnFhyo22Data)->Sn_2F17B, MONY_BUF_SIZE );
		F2217c |= 0x01;
	}
	if( m_pArith->l_test((*m_pSnFhyo22Data)->Sn_2F17C) ){
		memmove( F2217C, (*m_pSnFhyo22Data)->Sn_2F17C, MONY_BUF_SIZE );
		F2217c |= 0x100;
	}
	// 比例配分方式控除仕入税額
	if( m_pArith->l_test((*m_pSnFhyo22Data)->Sn_2F18A) ){
		memmove( F2218A, (*m_pSnFhyo22Data)->Sn_2F18A, MONY_BUF_SIZE );
		F2218c |= 0x10;
	}
	if( m_pArith->l_test((*m_pSnFhyo22Data)->Sn_2F18B) ){
		memmove( F2218B, (*m_pSnFhyo22Data)->Sn_2F18B, MONY_BUF_SIZE );
		F2218c |= 0x01;
	}
	if( m_pArith->l_test((*m_pSnFhyo22Data)->Sn_2F18C) ){
		memmove( F2218C, (*m_pSnFhyo22Data)->Sn_2F18C, MONY_BUF_SIZE );
		F2218c |= 0x100;
	}
	// 調整対象固定資産に係る調整税額
	memmove( F2219A, (*m_pSnFhyo22Data)->Sn_2F19A, MONY_BUF_SIZE );
	memmove( F2219B, (*m_pSnFhyo22Data)->Sn_2F19B, MONY_BUF_SIZE );
	memmove( F2219C, (*m_pSnFhyo22Data)->Sn_2F19C, MONY_BUF_SIZE );
	// 調整対象固定資産を課税業務用に転用した調整額
	memmove( F2220A, (*m_pSnFhyo22Data)->Sn_2F20A, MONY_BUF_SIZE );
	memmove( F2220B, (*m_pSnFhyo22Data)->Sn_2F20B, MONY_BUF_SIZE );
	memmove( F2220C, (*m_pSnFhyo22Data)->Sn_2F20C, MONY_BUF_SIZE );

	// 控除対象仕入税額
	F2221c = (*m_pSnFhyo22Data)->Sn_2F21sw;
	if( F2221c & 0x01 ){
		memmove( F2221A, (*m_pSnFhyo22Data)->Sn_2F21A, MONY_BUF_SIZE );
	}
	else{
//		memmove( F2221A, pSyzShin->Snd.Sn_1F8A, MONY_BUF_SIZE );
		memmove( F2221A, (*m_pSnFhyo10Data)->Sn_1F4A, MONY_BUF_SIZE );
	}
	if( F2221c & 0x10 ){
		memmove( F2221B, (*m_pSnFhyo22Data)->Sn_2F21B, MONY_BUF_SIZE );
	}
	else{
//		memmove( F2221B, pSyzShin->Snd.Sn_1F8B, MONY_BUF_SIZE );
		memmove( F2221B, (*m_pSnFhyo10Data)->Sn_1F4B, MONY_BUF_SIZE );
	}
	if( F2221c & 0x100 ){
		memmove( F2221C, (*m_pSnFhyo22Data)->Sn_2F21C, MONY_BUF_SIZE );
	}
	else{
//		memmove( F2221B, pSyzShin->Snd.Sn_1F8B, MONY_BUF_SIZE );
		memmove( F2221C, (*m_pSnFhyo10Data)->Sn_1F4C, MONY_BUF_SIZE );
	}

	// 貸倒回収に係る税額
	if( pas == 1 ){
		memmove( FS23A, (*m_pSnFhyo22Data)->Sn_2F23A, MONY_BUF_SIZE );
		memmove( FS23B, (*m_pSnFhyo22Data)->Sn_2F23B, MONY_BUF_SIZE );
		memmove( FS23C, (*m_pSnFhyo22Data)->Sn_2F23C, MONY_BUF_SIZE );
	}
	memmove( F2223A, (*m_pSnFhyo22Data)->Sn_2F23A, MONY_BUF_SIZE );
	memmove( F2223B, (*m_pSnFhyo22Data)->Sn_2F23B, MONY_BUF_SIZE );
	memmove( F2223C, (*m_pSnFhyo22Data)->Sn_2F23C, MONY_BUF_SIZE );
	// 控除過大調整税額	･ 貸倒回収に係る消費税額
//	m_pArith->l_sub( F2222A, pSyzShin->Snd.Sn_1F7A, F2223A );
	m_pArith->l_sub( F2222A, (*m_pSnFhyo10Data)->Sn_1F3A, F2223A );
//	m_pArith->l_sub( F2222B, pSyzShin->Snd.Sn_1F7B, F2223B );
	m_pArith->l_sub( F2222B, (*m_pSnFhyo10Data)->Sn_1F3B, F2223B );

	return 0;
}

//-----------------------------------------------------------------------------
// データの書込み
//-----------------------------------------------------------------------------
// 引数		pas	：
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo22::WriteData( int pas )
{
	int		st = 0;

	// 課税標準額
	memmove( (*m_pSnFhyo22Data)->Sn_2F1A, F221A,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F1B, F221B,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F1C, F221C,  MONY_BUF_SIZE );
	// 免税売上額
	memmove( (*m_pSnFhyo22Data)->Sn_2F2A, F222A,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F2B, F222B,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F2C, F222C,  MONY_BUF_SIZE );
	// 非課税資産の輸出
	memmove( (*m_pSnFhyo22Data)->Sn_2F3A, F223A,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F3B, F223B,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F3C, F223C,  MONY_BUF_SIZE );
	// 課税資産の譲渡等の対価の額
	memmove( (*m_pSnHonpyoData)->Sn_KZURI, F224C, MONY_BUF_SIZE );
	// 非課税売上額
	memmove( (*m_pSnFhyo22Data)->Sn_2F6A, F226A,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F6B, F226B,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F6C, F226C,  MONY_BUF_SIZE );
	// 資産の譲渡等の対価の額
//	memmove( pSyzShin->Snd.Sn_SOURI, F227C, MONY_BUF_SIZE );
	memmove( (*m_pSnHonpyoData)->Sn_SOURI, F227C, MONY_BUF_SIZE );
	// 課税売上割合
	(*m_pSnHeadData)->Sn_UPERS &= 0xfe;
	if( ((*m_pSnFhyo22Data)->Sn_2FUWS=F22UWS) < (*m_pSnHeadData)->m_UriRatioBorder ){
		(*m_pSnHeadData)->Sn_UPERS |= 0x01;
	}

	// 課税仕入に係る支払対価の額
	memmove( (*m_pSnFhyo22Data)->Sn_2F8A, F228A,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F8B, F228B,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F8C, F228C,  MONY_BUF_SIZE );
	// 課税仕入に係る消費税額
	memmove( (*m_pSnFhyo22Data)->Sn_2F9A, F229A,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F9B, F229B,  MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F9C, F229C,  MONY_BUF_SIZE );

	(*m_pSnFhyo22Data)->Sn_2F9sw = F229c;//[Y]

	// 課税貨物に係る支払対価の額
//	memmove( (*m_pSnFhyo22Data)->Sn_2F10A, F2210A, MONY_BUF_SIZE );
//	memmove( (*m_pSnFhyo22Data)->Sn_2F10B, F2210B, MONY_BUF_SIZE );
//	memmove( (*m_pSnFhyo22Data)->Sn_2F10C, F2210C, MONY_BUF_SIZE );
	// 課税貨物に係る消費税額
	memmove( (*m_pSnFhyo22Data)->Sn_2F10A, F2211A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F10B, F2211B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F10C, F2211C, MONY_BUF_SIZE );
	// 課税売上に係る仕入税額
	memmove( (*m_pSnFhyo22Data)->Sn_2F15A, F2215A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F15B, F2215B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F15C, F2215C, MONY_BUF_SIZE );
	// 共通の売上に係る仕入税額
	memmove( (*m_pSnFhyo22Data)->Sn_2F16A, F2216A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F16B, F2216B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F16C, F2216C, MONY_BUF_SIZE );
	// 個別対応方式控除仕入税額
	memset( (*m_pSnFhyo22Data)->Sn_2F17A, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo22Data)->Sn_2F17B, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo22Data)->Sn_2F17C, '\0', MONY_BUF_SIZE );
	if( F2217c & 0x10 ){
		memmove( (*m_pSnFhyo22Data)->Sn_2F17A, F2217A, MONY_BUF_SIZE );
	}
	if( F2217c & 0x01 ){
		memmove( (*m_pSnFhyo22Data)->Sn_2F17B, F2217B, MONY_BUF_SIZE );
	}
	if( F2217c & 0x100 ){
		memmove( (*m_pSnFhyo22Data)->Sn_2F17C, F2217C, MONY_BUF_SIZE );
	}
	// 比例配分方式控除仕入税額
	memset( (*m_pSnFhyo22Data)->Sn_2F18A, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo22Data)->Sn_2F18B, '\0', MONY_BUF_SIZE );
	memset( (*m_pSnFhyo22Data)->Sn_2F18C, '\0', MONY_BUF_SIZE );
	if( F2218c & 0x10 ){
		memmove( (*m_pSnFhyo22Data)->Sn_2F18A, F2218A, MONY_BUF_SIZE );
	}
	if( F2218c & 0x01 ){
		memmove( (*m_pSnFhyo22Data)->Sn_2F18B, F2218B, MONY_BUF_SIZE );
	}
	if( F2218c & 0x100 ){
		memmove( (*m_pSnFhyo22Data)->Sn_2F18C, F2218C, MONY_BUF_SIZE );
	}
	// 調整対象固定資産に係る調整税額
	memmove( (*m_pSnFhyo22Data)->Sn_2F19A, F2219A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F19B, F2219B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F19C, F2219C, MONY_BUF_SIZE );
	// 調整対象固定資産を課税業務用に転用した調整額
	memmove( (*m_pSnFhyo22Data)->Sn_2F20A, F2220A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F20B, F2220B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F20C, F2220C, MONY_BUF_SIZE );
	// 控除対象仕入税額
	if( F2221c & 0x01 ){
		if( m_pArith->l_test(F2221A) ){
			memmove( (*m_pSnFhyo22Data)->Sn_2F21A, F2221A, MONY_BUF_SIZE );
		}
	}
	else{
		memset( (*m_pSnFhyo22Data)->Sn_2F21A, '\0', MONY_BUF_SIZE );
	}
	if( F2221c & 0x10 ){
		if( m_pArith->l_test(F2221B) ){
			memmove( (*m_pSnFhyo22Data)->Sn_2F21B, F2221B, MONY_BUF_SIZE );
		}
	}
	else{
		memset( (*m_pSnFhyo22Data)->Sn_2F21B, '\0', MONY_BUF_SIZE );
	}
	if( F2221c & 0x100 ){
		if( m_pArith->l_test(F2221C) ){
			memmove( (*m_pSnFhyo22Data)->Sn_2F21C, F2221C, MONY_BUF_SIZE );
		}
	}
	else{
		memset( (*m_pSnFhyo22Data)->Sn_2F21C, '\0', MONY_BUF_SIZE );
	}
	(*m_pSnFhyo22Data)->Sn_2F21sw = F2221c;

//	memmove( pSyzShin->Snd.Sn_1F8A, F2221A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F4A, F2221A, MONY_BUF_SIZE );
//	memmove( pSyzShin->Snd.Sn_1F8B, F2221B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo10Data)->Sn_1F4B, F2221B, MONY_BUF_SIZE );	
	memmove( (*m_pSnFhyo10Data)->Sn_1F4C, F2221C, MONY_BUF_SIZE );

//	m_pArith->l_add( pSyzShin->Snd.Sn_1F8C, F2221A, F2221B );

	

	
	// 合計額を持つべき？？？？？
	//m_pArith->l_add( (*m_pSnFhyo10Data)->Sn_1F8C, F2221A, F2221B );

	// 控除過大調整税額	･ 貸倒回収に係る消費税額
	if( (pas==1) && (m_pArith->l_cmp(FS23A, F2223A)||m_pArith->l_cmp(FS23B, F2223B)||m_pArith->l_cmp(FS23C, F2223C)) ){
		st = 1;
	}

//	m_pArith->l_add( pSyzShin->Snd.Sn_1F7A, F2222A, F2223A );
	m_pArith->l_add( (*m_pSnFhyo10Data)->Sn_1F3A, F2222A, F2223A );
//	m_pArith->l_add( pSyzShin->Snd.Sn_1F7B, F2222B, F2223B );
	m_pArith->l_add( (*m_pSnFhyo10Data)->Sn_1F3B, F2222B, F2223B );
	m_pArith->l_add( (*m_pSnFhyo10Data)->Sn_1F3C, F2222C, F2223C );

	memmove( (*m_pSnFhyo22Data)->Sn_2F23A, F2223A, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F23B, F2223B, MONY_BUF_SIZE );
	memmove( (*m_pSnFhyo22Data)->Sn_2F23C, F2223C, MONY_BUF_SIZE );

	return st;
}

BOOL CShinFhyo22::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	CWnd *pWnd = GetFocus();

	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			// 付表２－２更新
			GetAllDiagData();
			f22calq_all( 1 );
			WriteData( 1 );
//			CWnd*	p1	=	GetParent();
//			CWnd*	p2	=	p1->GetParent();
//			((CSyzShinSheet*)p2)->Fhyo1_calq( 1 );
//			((CSyzShinSheet*)p2)->Ippan_calq( 0 );
//			((CMainFrame *)AfxGetMainWnd())->PostMessageA( WM_CLOSE, 0 );

			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}

			return TRUE;
		}
		//-->追加 harino ('14.03.04)
		if( pMsg->wParam == VK_F2 ){
			DIAGRAM_DATA   dd;
			if( m_curwnd != -1 && (pWnd != GetDlgItem(IDC_BUTTON1)) && (pWnd != GetDlgItem(IDC_BUTTON2))) {
				short result = ((CIcsdiagctrl*)GetDlgItem( m_curwnd ))->GetPosition();
				diag_getdata( m_curwnd, result, &dd, CIcsdiagctrl );
				if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
					TerminationIcsdiagctrl2( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}
		//<--追加 harino ('14.03.04)

		if(pWnd == GetDlgItem(IDC_BUTTON1)){
			if( (pMsg->wParam==VK_F3) || (pMsg->wParam==VK_RIGHT) || (pMsg->wParam==VK_DOWN) || (pMsg->wParam==VK_TAB)){//[Y]
				if( *m_pShinInfo->pOver500MillionSw != 1 &&  F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){	//[Y]5億円以下かつ 課税売上割合が９５％以上の場合
					diag_setposition( IDC_ICSDIAGCTRL2, ID2219A, CIcsdiagctrl );
				}
				else{
					if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){//非連動の場合
						if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){	// 個別対応
							diag_setposition( IDC_ICSDIAGCTRL2, ID2215A, CIcsdiagctrl );
						}
						else{
							diag_setposition( IDC_ICSDIAGCTRL2, ID2218A, CIcsdiagctrl );
						}
					}
					else{
						diag_setposition( IDC_ICSDIAGCTRL2, ID2219A, CIcsdiagctrl );
					}
				}
				return TRUE;

			}
			// 前項目へ
			if( (pMsg->wParam==VK_LEFT) || (pMsg->wParam==VK_F2) || (pMsg->wParam==VK_UP) ){//[Y]
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){//非連動の場合
					diag_setposition( IDC_ICSDIAGCTRL2, ID2211C, CIcsdiagctrl );
				}
				else{
					diag_setposition( IDC_ICSDIAGCTRL2, ID2220C, CIcsdiagctrl );
				}
				return TRUE;
			}
		}


	}

	return CSyzBaseDlg::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// 金額表示
//-----------------------------------------------------------------------------
// 
// 
// 
//-----------------------------------------------------------------------------
void CShinFhyo22::dsp_prs( unsigned short Id, short Index, char *Mony )
{
	if( !m_DpSw ){	// 再集計のみ（表示無し）[04'10.26]
	
		DIAGRAM_DATA		DiagData;
		DiagData.data_imgdata = NULL;

		switch( Index ) {
			case ID221A :		//  1. 課税売上額
			case ID221B :		//  1. 課税売上額
			case ID221C :		//  1. 課税売上額

			case ID222C :		//  2. 免税売上額
			case ID223C :		//  3. 非課税資産の輸出金額	
			case ID226C :		//	6. 非課税売上額

			case ID228A :		//	8. 課税仕入に係る支払対価の額
			case ID228B :		//	8. 課税仕入に係る支払対価の額
			case ID228C :		//	8. 課税仕入に係る支払対価の額

			case ID229A :		//	9. 課税仕入に係る消費税額
			case ID229B :		//	9. 課税仕入に係る消費税額
			case ID229C :		//	9. 課税仕入に係る消費税額

			case ID2211A:		// 11. 課税貨物に係る消費税額
			case ID2211B:		// 11. 課税貨物に係る消費税額
			case ID2211C:		// 11. 課税貨物に係る消費税額

			case ID2212A:		// 12. 調整税額
			case ID2212B:		// 12. 調整税額
			case ID2212C:		// 12. 調整税額

			case ID2215A:		// 15. 課税売上に係る仕入税額
			case ID2215B:		// 15. 課税売上に係る仕入税額
			case ID2215C:		// 15. 課税売上に係る仕入税額

			case ID2216A:		// 16. 共通の売上に係る仕入税額
			case ID2216B:		// 16. 共通の売上に係る仕入税額
			case ID2216C:		// 16. 共通の売上に係る仕入税額

			case ID2217A:		// 17. 個別対応方式控除仕入税額
			case ID2217B:		// 17. 個別対応方式控除仕入税額
			case ID2217C:		// 17. 個別対応方式控除仕入税額

			case ID2218A:		// 18. 比例配分方式控除仕入税額
			case ID2218B:		// 18. 比例配分方式控除仕入税額
			case ID2218C:		// 18. 比例配分方式控除仕入税額

			case ID2219A:		// 19. 調整対象固定資産に係る調整税額
			case ID2219B:		// 19. 調整対象固定資産に係る調整税額
			case ID2219C:		// 19. 調整対象固定資産に係る調整税額

			case ID2220A:		// 20. 調整対象固定資産を課税業務用に転用した調整額
			case ID2220B:		// 20. 調整対象固定資産を課税業務用に転用した調整額
			case ID2220C:		// 20. 調整対象固定資産を課税業務用に転用した調整額

			case ID2221A: 		// 21. 差引控除対象仕入税額
			case ID2221B:		// 21. 差引控除対象仕入税額
			case ID2221C:		// 21. 差引控除対象仕入税額

			case ID2223A:		// 23. 貸倒回収に係る消費税額
			case ID2223B:		// 23. 貸倒回収に係る消費税額
			case ID2223C:		// 23. 貸倒回収に係る消費税額
				memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
				diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				break;
			default		:
				memmove( DiagData.data_val, Mony, MONY_BUF_SIZE );
				diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				break;
		}

	}
}

//-----------------------------------------------------------------------------
// 金額表示クリア
//-----------------------------------------------------------------------------
//
//
//-----------------------------------------------------------------------------
void CShinFhyo22::dsp_cls( unsigned short Id, short Index )
{
	if( !m_DpSw ){	// 再集計のみ（表示無し）[04'10.26]

		DIAGRAM_DATA		DiagData;
		DiagData.data_imgdata = NULL;

		switch( Index ){
			case 147 :		// 課税期間　自
			case 148 :		// 課税期間　至
			case 149 :		// 氏名又は､名称
			case ID22EX :	// 課税売上割合
				diag_clear( Id, Index, TRUE, CIcsdiagctrl );
				break;
			default		:
				memset( DiagData.data_val, '\0', MONY_BUF_SIZE );
				diag_setdata( Id, Index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				break;
		}

	}
}

//-----------------------------------------------------------------------------
// 入力項目　色変え
//-----------------------------------------------------------------------------
// 
// 
// 
//-----------------------------------------------------------------------------
void CShinFhyo22::ChangeColor( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, index, &DA, CIcsdiagctrl );
	switch( sign ){
		case 1 :		// 入力不可	（白色）
			DA.attr_bcolor = BC_WHITE;	
			break;
		case 2 : 		// ロック	（灰色）
			DA.attr_bcolor = BC_GRAY;	
			break;
		case 3 : 		// 特殊色①	（青色）
			DA.attr_bcolor = BC_BLUE;
			break;
		case 4 :		// 特殊色②	（緑色）	
			DA.attr_bcolor = BC_GREEN;
			break;
		case 5 : 		// 特殊色③	（薄緑色）	 
			DA.attr_bcolor = BC_GREEN_L;
			break;
		default:		// 実額入力	（クリーム色） 
			DA.attr_bcolor = BC_CREAM;	
			break;
	}
	diag_setattr( id, index, &DA, FALSE, CIcsdiagctrl );
}

//-----------------------------------------------------------------------------
// 入力許可・不許可 切替
//-----------------------------------------------------------------------------
// 
// 
//-----------------------------------------------------------------------------
void CShinFhyo22::ModifyDiagInput( short index, int sign )
{
	if( !m_DpSw ){	// 再集計のみ（表示無し）
		diag_modify( IDC_ICSDIAGCTRL2, index, sign, CIcsdiagctrl );
	}
}

//-----------------------------------------------------------------------------
// 付表２-(2)-1 画面出力 (拡張バージョン)
//-----------------------------------------------------------------------------
void CShinFhyo22::DispFh22()
{
	DIAGRAM_ATTRIBUTE	DA;

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 画面の初期化
	for( int i=4; i<=6; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL2, (short )i );
	}
	for( int i=ID221A; i<=ID2223D; i++ ){
		dsp_cls( IDC_ICSDIAGCTRL2, (short )i );
	}

	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){//[Y]連動の場合手入力サイン初期化
		//手入力サイン初期化
		F229c = 0;
		F2217c = 0;
		F2218c = 0;
//-- '15.03.09 --
//		F2221c = 0;
//---------------
		if( ((CH26HyoView*)m_pParent)->IsSpcDataLinked() == FALSE ){
			F2221c = 0;
		}
//---------------
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
//	(*m_pSnHeadData)->GetYmdData( ID_ICSSH_KAZEIKIKAN_FROM, &yy, &mm, &dd );
	sprintf_s( buf, sizeof(buf), _T("%02x.%02x.%02x"), (yy&0xff), (mm&0xff), (dd&0xff) );
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 5, (struct IUnknown *)&DiagData, CIcsdiagctrl );

//	KOBET = pSyzShin->Anbun;
	memset( buf, '\0', sizeof( buf ) );
	if( (*m_pSnHeadData)->IsSoleProprietor() ){
		memmove( buf, (*m_pSnHeadData)->Sn_DIHYO, sizeof((*m_pSnHeadData)->Sn_DIHYO) );
	}
	else{
		memmove( buf, (*m_pSnHeadData)->Sn_CONAM, sizeof((*m_pSnHeadData)->Sn_CONAM) );
	}
	DiagData.data_disp = (char *)buf;
	diag_setdata( IDC_ICSDIAGCTRL2, 6, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	
	// 課税売上高の５億超えチェック
	char	VAL[6] = {0};
	char	ValCnv[6] = {0};
	memset( VAL, '\0', 6 );
	memset( ValCnv, '\0', 6 );
	m_pArith->l_add( VAL, F221A, F221B );
	m_pArith->l_add( VAL, VAL, F221C );//[Y]
	m_pArith->l_add( VAL, VAL, F222C );//[Y]
	(*m_pSnHeadData)->Sn_Sign4 &= 0xef;
	ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 1 );			//0 );
	ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 1 );			//0 );
	diag_clear( IDC_ICSDIAGCTRL1, 4, TRUE, CIcsdiagctrl );
/*- '14.06.10 -*/
//	*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( VAL, ValCnv, m_pShinInfo->s_tno, m_pShinInfo->d_tno );
/*-------------*/
	*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( VAL, ValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );
/*-------------*/
	if( *m_pShinInfo->pOver500MillionSw == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
//		memset( pSyzShin->RatioAsc, '\0', 6 );
//		strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), "11000" );
		(*m_pSnHeadData)->m_UriRatioBorder = 11000;
		(*m_pSnHeadData)->Sn_Sign4 |= 0x10;
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
			ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 5 );		//4 );
			ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 5 );		//4 );
			memset( buf, '\0', sizeof( buf ) );
			if( m_pArith->l_cmp( VAL, ValCnv ) == 0 ){
				strcpy_s( buf, sizeof( buf ), "課税売上高が、５億円を超えています。\n" );
			}
			else{
				strcpy_s( buf, sizeof( buf ), "課税売上高が、年換算で５億円を超えています。\n" );
			}
			strcat_s( buf, sizeof( buf ), "比例配分方式の選択も含めて注意してください。" );
			DiagData.data_disp = (char *)buf;
			diag_setdata( IDC_ICSDIAGCTRL1, 4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
	}
	else{			// ４億５千万以上で５億以下又は４億５千万未満
//		memset( pSyzShin->RatioAsc, '\0', 6 );
//		strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), "9500" );
		(*m_pSnHeadData)->m_UriRatioBorder = 9500;
		*m_pShinInfo->pOver500MillionSw = 0;
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
			ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 1 );			//0 );
			ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 1 );			//0 );
			diag_clear( IDC_ICSDIAGCTRL1, 4, TRUE, CIcsdiagctrl );
		}
	}
	m_Fhyo22Diag01.Refresh();
	m_Fhyo22Diag02.Refresh();

	// 課税標準額
	f22calq_1();
	// 免税売上額
	dsp_prs( IDC_ICSDIAGCTRL2, ID222C, F222C );
	// 非課税資産の輸出
	dsp_prs( IDC_ICSDIAGCTRL2, ID223C, F223C );
	// 課税資産の譲渡等の対価の額
	f22calq_45();
	// 非課税売上額
	f22calq_6();
	// 資産の譲渡等の対価の額 ･ 課税売上割合
	f22calq_7();
	// 課税仕入に係る支払対価の額
	f22calq_8();
	// 課税仕入に係る消費税額
	f22calq_9();
	// 課税貨物に係る消費税額
	f22calq_11();
	// (免税)控除対象調整税額
	f22calq_12();
	// 課税仕入等の税額の合計額
	f22calq_13();
	if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
		f22calq_ko();		// 課税売上割合が９５％未満
	}
	else{
		// 課税売上割合が９５％以上
		if( F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
			f22calq_14();
		}
		// 課税売上割合が９５％未満
		else{
			f22calq_ko();
		}
	}
//--> '15.04.04 INS START
	// 特定収入を加味した控除対象仕入税額の計算
	f22calq_spc();
//<-- '15.04.04 INS END
	// 調整対象固定資産に係る調整税額
	f22calq_19();
	// 調整対象固定資産を課税業務用に転用した調整額
	f22calq_20();
	// 差引控除税額
	f22calq_2122();
	// 貸倒回収に係る消費税額
	f22calq_23();

	m_Fhyo22Diag02.Refresh();//[Y]

}

BEGIN_EVENTSINK_MAP(CShinFhyo22, CSyzBaseDlg)
ON_EVENT(CShinFhyo22, IDC_ICSDIAGCTRL2, 1, CShinFhyo22::EditONIcsdiagctrl2, VTS_I2)
ON_EVENT(CShinFhyo22, IDC_ICSDIAGCTRL2, 2, CShinFhyo22::EditOFFIcsdiagctrl2, VTS_I2)
ON_EVENT(CShinFhyo22, IDC_ICSDIAGCTRL2, 3, CShinFhyo22::TerminationIcsdiagctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
ON_EVENT(CShinFhyo22, IDC_ICSDIAGCTRL2, 9, CShinFhyo22::CheckButtonIcsdiagctrl2, VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinFhyo22::EditONIcsdiagctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	//-->Terminationへ移動 harino ('14.03.07)
	//RECT	rectA, rectB;
	//GetWindowRect( &rectA );
	//diag_getrect( IDC_ICSDIAGCTRL2, index, &rectB, CIcsdiagctrl);
	//if ( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
	//	int	cpos = GetScrollPos( SB_VERT );
	//	SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	//}
	//<--Terminationへ移動 harino ('14.03.07)
	m_curwnd = IDC_ICSDIAGCTRL2;

	switch( index ){

		case ID2212A :
		case ID2212B :

				if( m_DlSw ){
					if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
						// 課税売上割合に準ずる割合を有効
						diag_setposition( IDC_ICSDIAGCTRL2, ID2215A, CIcsdiagctrl );
					}
					else{
						if( F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){		// 課税売上高割合が95%以上の場合
							diag_setposition( IDC_ICSDIAGCTRL2, ID2219A, CIcsdiagctrl );
						}
						else{				// 課税売上高割合が95%未満の場合	
							if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){	// 個別対応
								diag_setposition( IDC_ICSDIAGCTRL2, ID2215A, CIcsdiagctrl );
							}
							else{										// 比例配分方式
								diag_setposition( IDC_ICSDIAGCTRL2, ID2218A, CIcsdiagctrl );
							}
						}
					}
				}
				else{
					diag_setposition( IDC_ICSDIAGCTRL2, ID2211B, CIcsdiagctrl );
				}

				// 次入力項目　設定
				if( !m_DlSw ){
					if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
						diag_setposition( IDC_ICSDIAGCTRL2, ID2215A, CIcsdiagctrl );
					}
					else{
						if( F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){// 課税売上高割合が95%以上の場合
							diag_setposition( IDC_ICSDIAGCTRL2, ID2219A, CIcsdiagctrl );
						}
						else{				// 課税売上高割合が95%未満の場合	
							if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){	// 個別対応
								diag_setposition( IDC_ICSDIAGCTRL2, ID2215A, CIcsdiagctrl );
							}
							else{										// 比例配分方式
								diag_setposition( IDC_ICSDIAGCTRL2, ID2218A, CIcsdiagctrl );
							}
						}
					}
				}
				else{
					diag_setposition( IDC_ICSDIAGCTRL2, ID2211B, CIcsdiagctrl );
				}
				break;

		default      :
				break;
	}
}

void CShinFhyo22::EditOFFIcsdiagctrl2(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char	CkMy[MONY_BUF_SIZE];
	int		length;
	int		tmpUWexp = 0;	// 元 m_UWexp;

	char	VAL[MONY_BUF_SIZE] = {0};
	char	ValCnv[MONY_BUF_SIZE] = {0};

	//m_curwnd = -1;	// 削除 harino ('14.03.07)

	// 12. 調整税額
	if( (index!=ID2212A) && (index!=ID2212B) ){
		if( index > ID2212B ){
			m_DlSw = 1;
		}
		else{
			m_DlSw = 0;
		}
	}
	
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	char				buf[512]={0};

	switch( index ){

		case ID221A :		//  1. 課税売上額
			diag_getdata( IDC_ICSDIAGCTRL2, ID221A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, F221A, MONY_BUF_SIZE );
			memmove( F221A, DiagData.data_val, MONY_BUF_SIZE ); 
			//if( m_pArith->l_cmp( CkMy, F221A ) ) {	// End時に正しく判定できないため削除 harino ('14.03.12)
				memset( VAL, '\0', MONY_BUF_SIZE );
				memset( ValCnv, '\0', MONY_BUF_SIZE );
				m_pArith->l_add( VAL, F221A, F221B );
				m_pArith->l_add( VAL, VAL, F221C );
//				m_pArith->l_add( VAL, VAL, F222B );
				m_pArith->l_add( VAL, VAL, F222C );
				// 課税売上高の５億超えチェック
				(*m_pSnHeadData)->Sn_Sign4 &= 0xef;
/*- '14.06.10 -*/
//				*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( VAL, ValCnv, m_pShinInfo->s_tno, m_pShinInfo->d_tno );
/*-------------*/
				*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( VAL, ValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );
/*-------------*/
//				if( pSyzShin->m_Million == 1 && pSyzShin->m_TaxReforms == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
				if( *m_pShinInfo->pOver500MillionSw == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
//					memset( pSyzShin->RatioAsc, '\0', MONY_BUF_SIZE );
//					strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), "11000" );
					(*m_pSnHeadData)->m_UriRatioBorder = 11000;
					if( !((*m_pSnHeadData)->Sn_Sign4&0x10) && ((*m_pSnHeadData)->m_s_sgn4&0x80) ){
						// 課税売上に係る仕入税額
						memmove( F2215A, F2215AEX, MONY_BUF_SIZE );
						memmove( F2215B, F2215BEX, MONY_BUF_SIZE );
						memmove( F2215C, F2215CEX, MONY_BUF_SIZE );

						// 共通の売上に係る仕入税額
						memmove( F2216A, F2216AEX, MONY_BUF_SIZE );
						memmove( F2216B, F2216BEX, MONY_BUF_SIZE );
						memmove( F2216C, F2216CEX, MONY_BUF_SIZE );
					}
					(*m_pSnHeadData)->Sn_Sign4 |= 0x10;
					if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 5 );
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 5 );
						memset( buf, '\0', sizeof( buf ) );
						if( m_pArith->l_cmp( VAL, ValCnv ) == 0 ){
							strcpy_s( buf, sizeof( buf ), _T("課税売上高が、５億円を超えています。\n") );
						}
						else{
							strcpy_s( buf, sizeof( buf ), _T("課税売上高が、年換算で５億円を超えています。\n") );
						}
						strcat_s( buf, sizeof( buf ), _T("比例配分方式の選択も含めて注意してください。") );
						DiagData.data_disp = (char *)buf;
						diag_setdata( IDC_ICSDIAGCTRL1, 4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					}
				}
				else{			// ４億５千万以上で５億以下又は４億５千万未満
//					memset( pSyzShin->RatioAsc, '\0', MONY_BUF_SIZE );
//					strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), "9500" );
					(*m_pSnHeadData)->m_UriRatioBorder = 9500;
					*m_pShinInfo->pOver500MillionSw = 0;
					if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 1 );			//0 );
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 1 );			//0 );
						diag_clear( IDC_ICSDIAGCTRL1, 4, TRUE, CIcsdiagctrl );
					}
				}
				m_Fhyo22Diag01.Refresh();
				m_Fhyo22Diag02.Refresh();
			//}
			// 各入力の計算表示
			f22calq_all( 1 );
			break;

		case ID221B :		//  1. 課税売上額
			diag_getdata( IDC_ICSDIAGCTRL2, ID221B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, F221B, MONY_BUF_SIZE );
			memmove( F221B, DiagData.data_val, MONY_BUF_SIZE ); 
			//if( m_pArith->l_cmp(CkMy, F221B) ) {	// End時に正しく判定できないため削除 harino ('14.03.12)
				memset( VAL, '\0', MONY_BUF_SIZE );
				memset( ValCnv, '\0', MONY_BUF_SIZE );
				m_pArith->l_add( VAL, F221A, F221B );
				m_pArith->l_add( VAL, VAL, F221C );
				m_pArith->l_add( VAL, VAL, F222C );

				// 課税売上高の５億超えチェック
				(*m_pSnHeadData)->Sn_Sign4 &= 0xef;
/*- '14.06.10 -*/
//				*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( VAL, ValCnv, m_pShinInfo->s_tno, m_pShinInfo->d_tno );
/*-------------*/
				*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( VAL, ValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );
/*-------------*/
//				if( pSyzShin->m_Million == 1 && pSyzShin->m_TaxReforms == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
				if( *m_pShinInfo->pOver500MillionSw == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
//					memset( pSyzShin->RatioAsc, '\0', MONY_BUF_SIZE );
//					strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), _T("11000") );
					(*m_pSnHeadData)->m_UriRatioBorder = 11000;
					if( !((*m_pSnHeadData)->Sn_Sign4&0x10) && ((*m_pSnHeadData)->m_s_sgn4&0x80) ){
						// 課税売上に係る仕入税額
						memmove( F2215A, F2215AEX, MONY_BUF_SIZE );
						memmove( F2215B, F2215BEX, MONY_BUF_SIZE );
						memmove( F2215C, F2215CEX, MONY_BUF_SIZE );

						// 共通の売上に係る仕入税額
						memmove( F2216A, F2216AEX, MONY_BUF_SIZE );
						memmove( F2216B, F2216BEX, MONY_BUF_SIZE );
						memmove( F2216C, F2216CEX, MONY_BUF_SIZE );

					}
					(*m_pSnHeadData)->Sn_Sign4 |= 0x10;
					if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 5 );			//4 );
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 5 );			//4 );
						memset( buf, '\0', sizeof( buf ) );
						if( m_pArith->l_cmp(VAL, ValCnv) == 0 ){ 
							strcpy_s( buf, sizeof(buf), _T("課税売上高が、５億円を超えています。\n") );
						}
						else{
							strcpy_s( buf, sizeof(buf), _T("課税売上高が、年換算で５億円を超えています。\n") );
						}
						strcat_s( buf, sizeof(buf), _T("比例配分方式の選択も含めて注意してください。") );
						DiagData.data_disp = (char *)buf;
						diag_setdata( IDC_ICSDIAGCTRL1, 4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					}
				}
				else{			// ４億５千万以上で５億以下又は４億５千万未満
//					memset( pSyzShin->RatioAsc, '\0', MONY_BUF_SIZE );
//					strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), "9500" );
					(*m_pSnHeadData)->m_UriRatioBorder = 9500;
					*m_pShinInfo->pOver500MillionSw = 0;
					if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 1 );			//0 );
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 1 );			//0 );
						diag_clear( IDC_ICSDIAGCTRL1, 4, TRUE, CIcsdiagctrl );
					}
				}
				m_Fhyo22Diag01.Refresh();
				m_Fhyo22Diag02.Refresh();
			//}
			// 各入力の計算表示
			f22calq_all( 1 );
			break;

		case ID221C :		//  1. 課税売上額
			diag_getdata( IDC_ICSDIAGCTRL2, ID221C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, F221C, MONY_BUF_SIZE );
			memmove( F221C, DiagData.data_val, MONY_BUF_SIZE ); 
			//if( m_pArith->l_cmp(CkMy, F221C) ) {	// End時に正しく判定できないため削除 harino ('14.03.12)
				memset( VAL, '\0', MONY_BUF_SIZE );
				memset( ValCnv, '\0', MONY_BUF_SIZE );
				m_pArith->l_add( VAL, F221A, F221B );
				m_pArith->l_add( VAL, VAL, F221C );
				m_pArith->l_add( VAL, VAL, F222C );
				// 課税売上高の５億超えチェック
				(*m_pSnHeadData)->Sn_Sign4 &= 0xef;
/*- '14.06.10 -*/
//				*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( VAL, ValCnv, m_pShinInfo->s_tno, m_pShinInfo->d_tno );
/*-------------*/
				*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( VAL, ValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );
/*-------------*/
//				if( pSyzShin->m_Million == 1 && pSyzShin->m_TaxReforms == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
				if( *m_pShinInfo->pOver500MillionSw == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
//					memset( pSyzShin->RatioAsc, '\0', MONY_BUF_SIZE );
//					strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), _T("11000") );
					(*m_pSnHeadData)->m_UriRatioBorder = 11000;
					if( !((*m_pSnHeadData)->Sn_Sign4&0x10) && ((*m_pSnHeadData)->m_s_sgn4&0x80) ){
						// 課税売上に係る仕入税額
						memmove( F2215A, F2215AEX, MONY_BUF_SIZE );
						memmove( F2215B, F2215BEX, MONY_BUF_SIZE );
						memmove( F2215C, F2215CEX, MONY_BUF_SIZE );
						// 共通の売上に係る仕入税額
						memmove( F2216A, F2216AEX, MONY_BUF_SIZE );
						memmove( F2216B, F2216BEX, MONY_BUF_SIZE );
						memmove( F2216C, F2216CEX, MONY_BUF_SIZE );
					}
					(*m_pSnHeadData)->Sn_Sign4 |= 0x10;
					if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 5 );			//4 );
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 5 );			//4 );
						memset( buf, '\0', sizeof( buf ) );
						if( m_pArith->l_cmp(VAL, ValCnv) == 0 ){ 
							strcpy_s( buf, sizeof(buf), _T("課税売上高が、５億円を超えています。\n") );
						}
						else{
							strcpy_s( buf, sizeof(buf), _T("課税売上高が、年換算で５億円を超えています。\n") );
						}
						strcat_s( buf, sizeof(buf), _T("比例配分方式の選択も含めて注意してください。") );
						DiagData.data_disp = (char *)buf;
						diag_setdata( IDC_ICSDIAGCTRL1, 4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					}
				}
				else{			// ４億５千万以上で５億以下又は４億５千万未満
//					memset( pSyzShin->RatioAsc, '\0', MONY_BUF_SIZE );
//					strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), "9500" );
					(*m_pSnHeadData)->m_UriRatioBorder = 9500;
					*m_pShinInfo->pOver500MillionSw = 0;
					if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 1 );			//0 );
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 1 );			//0 );
						diag_clear( IDC_ICSDIAGCTRL1, 4, TRUE, CIcsdiagctrl );
					}
				}
				m_Fhyo22Diag01.Refresh();
				m_Fhyo22Diag02.Refresh();
			//}
			// 各入力の計算表示
			f22calq_all( 1 );
			break;
		


		case ID222C :		//  2. 免税売上額
			diag_getdata( IDC_ICSDIAGCTRL2, ID222C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, F222C, MONY_BUF_SIZE );
			memmove( F222C, DiagData.data_val, MONY_BUF_SIZE ); 
			//if( m_pArith->l_cmp(CkMy, F222C) ){	// End時に正しく判定できないため削除 harino ('14.03.12)
				memset( VAL, '\0', MONY_BUF_SIZE );
				memset( ValCnv, '\0', MONY_BUF_SIZE );
				m_pArith->l_add( VAL, F221A, F221B );
				m_pArith->l_add( VAL, VAL, F221C );
				m_pArith->l_add( VAL, VAL, F222C );
				// 課税売上高の５億超えチェック
				(*m_pSnHeadData)->Sn_Sign4 &= 0xef;
/*- '14.06.10 -*/
//				*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( VAL, ValCnv, m_pShinInfo->s_tno, m_pShinInfo->d_tno );
/*-------------*/
				*m_pShinInfo->pOver500MillionSw = (*m_pSnHeadData)->CheckTaxationSales( VAL, ValCnv, m_pShinInfo->bil_stno, m_pShinInfo->bil_dtno );
/*-------------*/
//				if( pSyzShin->m_Million == 1 && pSyzShin->m_TaxReforms == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
				if( *m_pShinInfo->pOver500MillionSw == 1 ){	// ON:９５％ルール改正対応期間で５億円超え
//					memset( pSyzShin->RatioAsc, '\0', MONY_BUF_SIZE );
//					strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), "11000" );
					(*m_pSnHeadData)->m_UriRatioBorder = 11000;
					if( !((*m_pSnHeadData)->Sn_Sign4&0x10) && ((*m_pSnHeadData)->m_s_sgn4&0x80) ){
						// 課税売上に係る仕入税額
						memmove( F2215A, F2215AEX, MONY_BUF_SIZE );
						memmove( F2215B, F2215BEX, MONY_BUF_SIZE );
						memmove( F2215C, F2215CEX, MONY_BUF_SIZE );

						// 共通の売上に係る仕入税額
						memmove( F2216A, F2216AEX, MONY_BUF_SIZE );
						memmove( F2216B, F2216BEX, MONY_BUF_SIZE );
						memmove( F2216C, F2216CEX, MONY_BUF_SIZE );
					}
					(*m_pSnHeadData)->Sn_Sign4 |= 0x10;
					if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 5 );			//4 );
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 5 );			//4 );
						memset( buf, '\0', sizeof(buf) );
						if( m_pArith->l_cmp(VAL, ValCnv) == 0 ){
							strcpy_s( buf, sizeof( buf ), _T("課税売上高が、５億円を超えています。\n") );
						}
						else{
							strcpy_s( buf, sizeof( buf ), _T("課税売上高が、年換算で５億円を超えています。\n") );
						}
						strcat_s( buf, sizeof( buf ), _T("比例配分方式の選択も含めて注意してください。") );
						DiagData.data_disp = (char *)buf;
						diag_setdata( IDC_ICSDIAGCTRL1, 4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					}
				}
				else{			// ４億５千万以上で５億以下又は４億５千万未満
//					memset( pSyzShin->RatioAsc, '\0', MONY_BUF_SIZE );
//					strcpy_s( pSyzShin->RatioAsc, sizeof( pSyzShin->RatioAsc ), _T("9500") );
					(*m_pSnHeadData)->m_UriRatioBorder = 9500;
					*m_pShinInfo->pOver500MillionSw = 0;
					if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION, 1 );			//0 );
						ChangeColor( IDC_ICSDIAGCTRL2, MILLION+1, 1 );			//0 );
						diag_clear( IDC_ICSDIAGCTRL1, 4, TRUE, CIcsdiagctrl );
					}
				}
				m_Fhyo22Diag01.Refresh();
				m_Fhyo22Diag02.Refresh();
			//}
			// 各入力の計算表示
			f22calq_all( 2 );
			break;

		case ID223C :		//  3. 非課税資産の輸出金額	
			diag_getdata( IDC_ICSDIAGCTRL2, ID223C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F223C, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 3 );
			break;

		case ID226C :		//	MONY_BUF_SIZE. 非課税売上額
			diag_getdata( IDC_ICSDIAGCTRL2, ID226C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F226C, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 4 );
			break;

		case ID228A :		//	8. 課税仕入に係る支払対価の額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F228A, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID228A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F228A, CkMy) ){
				if(!(F229c&0x80)){//[Y]手入力されていない場合のみサインたてる
					F229c |= 0x10;
				}
			}
			memmove( F228A, CkMy, MONY_BUF_SIZE );

			// 各入力の計算表示
			f22calq_all( 5 );
			break;

		case ID228B :		//	8. 課税仕入に係る支払対価の額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F228B, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID228B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F228B, CkMy) ){
				if(!(F229c&0x08)){//[Y]手入力されていない場合のみサインたてる
					F229c |= 0x01;
				}
			}
			memmove( F228B, CkMy, MONY_BUF_SIZE );

			// 各入力の計算表示
			f22calq_all( 5 );
			break;

		case ID228C :		//	8. 課税仕入に係る支払対価の額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F228C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID228C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F228C, CkMy) ){
				if(!(F229c&0x800)){//[Y]手入力されていない場合のみサインたてる
					F229c |= 0x100;
				}
			}
			memmove( F228C, CkMy, MONY_BUF_SIZE );

			// 各入力の計算表示
			f22calq_all( 5 );
			break;


		case ID229A :		//	9. 課税仕入に係る消費税額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F229A, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID229A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F229A, CkMy) ){
				F229c |= 0x80;
				F229c &= ~0x10;
			}
			memmove( F229A, CkMy, MONY_BUF_SIZE );

			// 各入力の計算表示
			f22calq_all( 6 );//[Y]
			break;

		case ID229B :		//	9. 課税仕入に係る消費税額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F229B, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID229B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F229B, CkMy) ){
				F229c |= 0x08;
				F229c &= ~0x01;
			}
			memmove( F229B, CkMy, MONY_BUF_SIZE );

			// 各入力の計算表示
			f22calq_all( 6 );//[Y]
			break;

		case ID229C :		//	9. 課税仕入に係る消費税額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F229C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID229C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F229C, CkMy) ){
				F229c |= 0x800;
				F229c &= ~0x100;
			}
			memmove( F229C, CkMy, MONY_BUF_SIZE );

			// 各入力の計算表示
			f22calq_all( 6 );//[Y]
			break;

		case ID2211A:		// 10. 課税貨物に係る消費税額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F2211A, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID2211A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2211A, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(F2211A, CkMy) ){
				if( (((*m_pSnHeadData)->IsKobetuSiireAnbun()==TRUE)&&(F22UWS<(*m_pSnHeadData)->m_UriRatioBorder)) || ((*m_pSnHeadData)->IsUseSemiUriRatio()) ){		// 個別対応で95％未満の場合
					CString		Msg;
					Msg.Empty();
					Msg =  _T("課税貨物に係る消費税額が変更されました。\n");
					Msg += _T("個別対応方式欄の⑭～⑯の金額を見直してください。");
					ICSMessageBox( Msg, MB_OK, 0 );	// IDOK IDCANCEL
				}
			}

			// 各入力の計算表示
			f22calq_all( 7 );
			break;

		case ID2211B:		// 10. 課税貨物に係る消費税額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F2211B, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID2211B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2211B, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(F2211B, CkMy) ){
				if( (((*m_pSnHeadData)->IsKobetuSiireAnbun()==TRUE)&&(F22UWS<(*m_pSnHeadData)->m_UriRatioBorder)) || ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) ){		// 個別対応で95％未満の場合
					CString		Msg;
					Msg.Empty();
					Msg = _T("課税貨物に係る消費税額が変更されました。\n");
					Msg += _T("個別対応方式欄の⑭～⑯の金額を見直してください。");
					ICSMessageBox( Msg, MB_OK, 0 );	// IDOK IDCANCEL
				}
			}

			// 各入力の計算表示
			f22calq_all( 7 );
			break;

		case ID2211C:		// 10. 課税貨物に係る消費税額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F2211C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID2211C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2211C, DiagData.data_val, MONY_BUF_SIZE );
			if( m_pArith->l_cmp(F2211C, CkMy) ){
				if( (((*m_pSnHeadData)->IsKobetuSiireAnbun()==TRUE)&&(F22UWS<(*m_pSnHeadData)->m_UriRatioBorder)) || ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) ){		// 個別対応で95％未満の場合
					CString		Msg;
					Msg.Empty();
					Msg = _T("課税貨物に係る消費税額が変更されました。\n");
					Msg += _T("個別対応方式欄の⑭～⑯の金額を見直してください。");
					ICSMessageBox( Msg, MB_OK, 0 );	// IDOK IDCANCEL
				}
			}

			// 各入力の計算表示
			f22calq_all( 7 );
			break;


		case ID2212A:		// 11. 調整税額
		case ID2212B:		// 11. 調整税額
		case ID2212C:		// 11. 調整税額

			break;

		case ID2215A:		// 14. 課税売上に係る仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F22UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID2215A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2215A, DiagData.data_val, MONY_BUF_SIZE ); 
			memmove( F2215AEX, DiagData.data_val, MONY_BUF_SIZE ); 
			// 各入力の計算表示
			f22calq_all( 8 );
			break;

		case ID2215B:		// 14. 課税売上に係る仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F22UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID2215B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2215B, DiagData.data_val, MONY_BUF_SIZE ); 
			memmove( F2215BEX, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 8 );
			break;

		case ID2215C:		// 14. 課税売上に係る仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F22UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID2215C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2215C, DiagData.data_val, MONY_BUF_SIZE ); 
			memmove( F2215CEX, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 8 );
			break;


		case ID2216A:		// 15. 共通の売上に係る仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F22UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID2216A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2216A, DiagData.data_val, MONY_BUF_SIZE ); 
			memmove( F2216AEX, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 9 );
			break;

		case ID2216B:		// 15. 共通の売上に係る仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F22UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID2216B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2216B, DiagData.data_val, MONY_BUF_SIZE ); 
			memmove( F2216BEX, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 9 );
			break;

		case ID2216C:		// 15. 共通の売上に係る仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F22UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			diag_getdata( IDC_ICSDIAGCTRL2, ID2216C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2216C, DiagData.data_val, MONY_BUF_SIZE ); 
			memmove( F2216CEX, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 9 );
			break;

		case ID2217A:		// 16. 個別対応方式控除仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F22UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F2217A, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID2217A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F2217A, CkMy) ){
				F2217c |= 0x10;
			}
			memmove( F2217A, CkMy, MONY_BUF_SIZE );

			// 各入力の計算表示
			f22calq_all( 10 );
			break;

		case ID2217B:		// 16. 個別対応方式控除仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F22UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F2217B, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID2217B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F2217B, CkMy) ){
				F2217c |= 0x01;
			}
			memmove( F2217B, CkMy, MONY_BUF_SIZE );

			// 各入力の計算表示
			f22calq_all( 10 );
			break;

		case ID2217C:		// 16. 個別対応方式控除仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F22UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) ){
					break;
				}
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F2217C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID2217C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F2217C, CkMy) ){
				F2217c |= 0x100;
			}
			memmove( F2217C, CkMy, MONY_BUF_SIZE );

			// 各入力の計算表示
			f22calq_all( 10 );
			break;

		case ID2218A:		// 17. 比例配分方式控除仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F22UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==TRUE) ){
					break;
				}
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F2218A, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID2218A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F2218A, CkMy) ){
				F2218c |= 0x10;
			}
			memmove( F2218A, CkMy, MONY_BUF_SIZE );

			// 各入力の計算表示
			f22calq_all( 11 );
			break;

		case ID2218B:		// 17. 比例配分方式控除仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F22UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==TRUE) ){
					break;
				}
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F2218B, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID2218B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F2218B, CkMy) ){
				F2218c |= 0x01;
			}
			memmove( F2218B, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f22calq_all( 11 );
			break;

		case ID2218C:		// 17. 比例配分方式控除仕入税額
			if( (*m_pSnHeadData)->IsUseSemiUriRatio() == FALSE ){
				if( (F22UWS>=(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsKobetuSiireAnbun()==TRUE) ){
					break;
				}
			}
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F2218C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID2218C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F2218C, CkMy) ){
				F2218c |= 0x100;
			}
			memmove( F2218C, CkMy, MONY_BUF_SIZE );
			// 各入力の計算表示
			f22calq_all( 11 );
			break;

		case ID2219A:		// 18. 調整対象固定資産に係る調整税額
			diag_getdata( IDC_ICSDIAGCTRL2, ID2219A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2219A, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 12 );
			break;

		case ID2219B:		// 18. 調整対象固定資産に係る調整税額
			diag_getdata( IDC_ICSDIAGCTRL2, ID2219B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2219B, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 12 );
			break;

		case ID2219C:		// 18. 調整対象固定資産に係る調整税額
			diag_getdata( IDC_ICSDIAGCTRL2, ID2219C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2219C, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 12 );
			break;

		case ID2220A:		// 20. 調整対象固定資産を課税業務用に転用した調整額
			diag_getdata( IDC_ICSDIAGCTRL2, ID2220A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2220A, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 13 );
			break;

		case ID2220B:		// 19. 調整対象固定資産を課税業務用に転用した調整額
			diag_getdata( IDC_ICSDIAGCTRL2, ID2220B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2220B, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 13 );
			break;

		case ID2220C:		// 19. 調整対象固定資産を課税業務用に転用した調整額
			diag_getdata( IDC_ICSDIAGCTRL2, ID2220C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2220C, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 13 );
			break;

		case ID2221A:		// 20. 差引控除対象仕入税額
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F2221A, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID2221A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F2221A, CkMy) ){
				// 金額変更
				F2221c |= 0x01;
				memmove( F2221A, CkMy, MONY_BUF_SIZE );
				memmove( (*m_pSnFhyo22Data)->Sn_2F21A, F2221A, MONY_BUF_SIZE );
				if( m_pArith->l_test(F2221A) < 0 ){
					// 控除過大調整額
					m_pArith->l_neg( F2221A );
					memmove( F2222A, F2221A, MONY_BUF_SIZE );
					memset( F2221A, '\0', MONY_BUF_SIZE );	// 控除対象仕入税額０円クリア
				}
				else{
					// 控除対象仕入税額
					memset( F2222A, '\0', MONY_BUF_SIZE );	// 控除過大調整額０円クリア
				}
			}
			// 各入力の計算表示
			f22calq_all( 13 );
			break;

		case ID2221B:		// 20. 差引控除対象仕入税額
			// 各入力の計算表示
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F2221B, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID2221B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F2221B, CkMy) ){
				F2221c |= 0x10;
				memmove( F2221B, CkMy, MONY_BUF_SIZE );
				memmove( (*m_pSnFhyo22Data)->Sn_2F21B, F2221B, MONY_BUF_SIZE );		//★[13'11.21]
				if( m_pArith->l_test(F2221B) < 0 ){
					// 控除過大調整額
					m_pArith->l_neg( F2221B );
					memmove( F2222B, F2221B, MONY_BUF_SIZE );
					memset( F2221B, '\0', MONY_BUF_SIZE );	// 控除対象仕入税額０円クリア
				}
				else{
					// 控除対象仕入税額
					memset( F2222B, '\0', MONY_BUF_SIZE );	// 控除過大調整額０円クリア
				}
			}
			// 各入力の計算表示
			f22calq_all( 13 );
			break;

		case ID2221C:		// 20. 差引控除対象仕入税額
			// 各入力の計算表示
			memset( CkMy, '\0', MONY_BUF_SIZE );
			memmove( CkMy, F2221C, MONY_BUF_SIZE );
			diag_getdata( IDC_ICSDIAGCTRL2, ID2221C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( CkMy, DiagData.data_val, MONY_BUF_SIZE ); 
			if( m_pArith->l_cmp(F2221C, CkMy) ){
				F2221c |= 0x100;
				memmove( F2221C, CkMy, MONY_BUF_SIZE );
				memmove( (*m_pSnFhyo22Data)->Sn_2F21C, F2221C, MONY_BUF_SIZE );		//★[13'11.21]
				if( m_pArith->l_test(F2221C) < 0 ){
					// 控除過大調整額
					m_pArith->l_neg( F2221C );
					memmove( F2222C, F2221C, MONY_BUF_SIZE );
					memset( F2221C, '\0', MONY_BUF_SIZE );	// 控除対象仕入税額０円クリア
				}
				else{
					// 控除対象仕入税額
					memset( F2222C, '\0', MONY_BUF_SIZE );	// 控除過大調整額０円クリア
				}
			}
			// 各入力の計算表示
			f22calq_all( 13 );
			break;

		case ID2223A:		//  22. 貸倒回収に係る消費税額
			diag_getdata( IDC_ICSDIAGCTRL2, ID2223A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2223A, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 14 );
			break;

		case ID2223B:		//  22. 貸倒回収に係る消費税額
			diag_getdata( IDC_ICSDIAGCTRL2, ID2223B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2223B, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 14 );
			break;

		case ID2223C:		//  22. 貸倒回収に係る消費税額
			diag_getdata( IDC_ICSDIAGCTRL2, ID2223C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( F2223C, DiagData.data_val, MONY_BUF_SIZE ); 

			// 各入力の計算表示
			f22calq_all( 14 );
			break;

		case ID227EX :	// 課税売上割合の変更チェック
			(*m_pSnHeadData)->Sn_UPERS &= 0xef;
			diag_getdata( IDC_ICSDIAGCTRL2, ID227EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			if( DiagData.data_check ){
				(*m_pSnHeadData)->Sn_UPERS |= 0x10;
			}
			// 各入力の計算表示
			f22calq_all( 4 );
			break;

		case ID22EX :	// 課税売上割合
			F22UWSEX = (*m_pSnFhyo22Data)->Sn_2FUWSEX;
			diag_getdata( IDC_ICSDIAGCTRL2, ID22EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			length = DiagData.data_edit.GetLength();
			memset( buf, '\0', sizeof(buf) ); 
			tmpUWexp = 0;
			if( length ){
				memmove( buf, DiagData.data_edit, length );
			}
			m_Util.perinp( 3, 2, buf, (short *)&tmpUWexp );
			if( tmpUWexp <= 10000 ){
				(*m_pSnFhyo22Data)->Sn_2FUWSEX = tmpUWexp;
			}
			// 個別対応で95％未満の場合
			if( ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) && (F22UWSEX!=(*m_pSnFhyo22Data)->Sn_2FUWSEX) ){
				CString		Msg;
				Msg.Empty();
				Msg =  _T("課税売上割合が変更されました。\n");
				Msg += _T("個別対応方式欄の⑭～⑯の金額を見直してください。");
				ICSMessageBox( Msg, MB_OK, 0 );
				// 各入力の計算表示
				f22calq_all( 4 );
				break;
			}
			else{
				// 個別対応で95％未満の場合
				if( ((*m_pSnHeadData)->IsKobetuSiireAnbun()==TRUE) && (F22UWS!=(*m_pSnFhyo22Data)->Sn_2FUWSEX) && ((*m_pSnFhyo22Data)->Sn_2FUWSEX<(*m_pSnHeadData)->m_UriRatioBorder) ){
					CString		Msg;
					Msg.Empty();
					Msg =  _T("課税売上割合が変更されました。\n");
					Msg += _T("個別対応方式欄の⑭～⑯の金額を見直してください。");
					ICSMessageBox( Msg, MB_OK, 0 );
				}
			}
			// 各入力の計算表示
			f22calq_all( 4 );
			if( tmpUWexp > 10000 ){
//				F22DiagPosition2( ID22EX );
			}
			break;

		default     :
			break;
	}
}

void CShinFhyo22::TerminationIcsdiagctrl2(short index, short nChar, short length, LPUNKNOWN data)
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

	// 12. 調整税額
	if( (index!=ID2212A) && (index!=ID2212B) ){
		if( index > ID2212B ){
			m_DlSw = 1;
		}
		else{
			m_DlSw = 0;
		}
	}

	int pos = 0;
	if( wTerm==VK_UP ){
		pos = UpDownFocusMoveCheck(index , 1);
		m_Fhyo22Diag02.SetPosition(pos);
	}
	if( wTerm==VK_DOWN ){
		pos = UpDownFocusMoveCheck(index , 0);
		m_Fhyo22Diag02.SetPosition(pos);
	}

	
	if( (wTerm==VK_RETURN) || (wTerm==VK_RIGHT) || (wTerm==VK_F3) ){
		m_Fhyo22Diag02.SetNextPosition();
		if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){//[Y]非連動の場合
			if(index == ID2211C){
				c_set(IDC_BUTTON1);
			}
		}
		else{
			if(index == ID2220C){
				c_set(IDC_BUTTON1);
			}
		}
	}
	// 前項目へ
	if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		m_Fhyo22Diag02.SetPrevPosition();

		if( *m_pShinInfo->pOver500MillionSw != 1 &&  F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){	//[Y]5億円以下かつ 課税売上割合が９５％以上の場合
			if(index == ID2219A){
				c_set(IDC_BUTTON1);
			}
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){//[Y]非連動の場合
				if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){	//[Y] 個別対応
					if(index == ID2215A){
						c_set(IDC_BUTTON1);
					}
				}
				else{
					if(index == ID2218A){
						c_set(IDC_BUTTON1);
					}
				}
			}
			else{
				if(index == ID2219A){
					c_set(IDC_BUTTON1);
				}
			}
		}
	}
	if( wTerm == VK_DELETE ){	
		switch( index ){
			case ID2221A:		// 21. 控除対象仕入税額
				F2221c &= ~0x001;
				// 各入力の計算表示
				f22calq_all( 13 );
				m_Fhyo22Diag02.SetPosition( ID2221A );
				break;
			case ID2221B:		// 21. 控除対象仕入税額
				F2221c &= ~0x010;
				// 各入力の計算表示
				f22calq_all( 13 );
				m_Fhyo22Diag02.SetPosition( ID2221B );
				break;
			case ID2221C:		// 21. 控除対象仕入税額
				F2221c &= ~0x100;
				// 各入力の計算表示
				f22calq_all( 13 );
				m_Fhyo22Diag02.SetPosition( ID2221C );
				break;

			case ID229A:		//	9. 課税仕入に係る消費税額
				F229c &= ~0x080;
				F229c |= 0x010;
				// 各入力の計算表示
				f22calq_all( 6 );
				m_Fhyo22Diag02.SetPosition( ID229A );
				break;

			case ID229B:		//	9. 課税仕入に係る消費税額
				F229c &= ~0x008;
				F229c |= 0x001;
				// 各入力の計算表示
				f22calq_all( 6 );
				m_Fhyo22Diag02.SetPosition( ID229B );
				break;

			case ID229C:		//	9. 課税仕入に係る消費税額
				F229c &= ~0x800;
				F229c |= 0x100;
				// 各入力の計算表示
				f22calq_all( 6 );
				m_Fhyo22Diag02.SetPosition( ID229C );
				break;

			case ID2217A:		// 16. 個別対応方式控除仕入税額
				F2217c &= ~0x10;
				// 各入力の計算表示
				f22calq_all( 10 );
				m_Fhyo22Diag02.SetPosition( ID2217A );
				break;

			case ID2217B:		// 16. 個別対応方式控除仕入税額
				F2217c &= ~0x01;
				// 各入力の計算表示
				f22calq_all( 10 );
				m_Fhyo22Diag02.SetPosition( ID2217B );
				break;

			case ID2217C:		// 16. 個別対応方式控除仕入税額
				F2217c &= ~0x100;
				// 各入力の計算表示
				f22calq_all( 10 );
				m_Fhyo22Diag02.SetPosition( ID2217C );
				break;

			case ID2218A:		// 17. 比例配分方式控除仕入税額
				F2218c &= ~0x10;
				// 各入力の計算表示
				f22calq_all( 11 );
				m_Fhyo22Diag02.SetPosition( ID2218A );
				break;

			case ID2218B:		// 17. 比例配分方式控除仕入税額
				F2218c &= ~0x01;
				// 各入力の計算表示
				f22calq_all( 11 );
				m_Fhyo22Diag02.SetPosition( ID2218B );
				break;

			case ID2218C:		// 17. 比例配分方式控除仕入税額
				F2218c &= ~0x100;
				// 各入力の計算表示
				f22calq_all( 11 );
				m_Fhyo22Diag02.SetPosition( ID2218C );
				break;

			default :
				dsp_cls(IDC_ICSDIAGCTRL2 , index);
				break;
		}
	}

	//-->Editonから移動 harino ('14.03.07)
	int nowpos = m_Fhyo22Diag02.GetPosition();	// 移動先または自身のポジション

	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL2, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
	//-->Editonから移動 harino ('14.03.07)
}

void CShinFhyo22::CheckButtonIcsdiagctrl2(short index, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	// 課税売上割合の変更チェック
	if( index == ID227EX ){
		diag_getdata( IDC_ICSDIAGCTRL2, ID227EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
//			pSyzShin->RATIOX = pSyzShin->RATIO;
			CString	Msg;
			Msg.Empty();
			if( DiagData.data_check ){
				Msg  = _T("課税売上割合に準ずる割合を適用する為に、\n");
				Msg += _T("課税売上割合を変更される場合は・・・[はい]を、\n");
				Msg += _T("それ以外の理由で変更される場合は・・・[いいえ]を、\n");
				Msg += _T("選択してください。" );
//				Msg += _T("選択してください。\n" );
//				Msg += _T("※　申告書集計を行うと、全ての変更は無効になります。");
//				Msg += _T("※　いずれの場合も⑭～⑯の金額を再計算します。");
				int st = ICSMessageBox( Msg, MB_YESNO, 0 );
				if( st == IDYES ){
					if( (*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE ){
//						pSyzShin->RATIO &= 0xfd;
					}
					else{
//						pSyzShin->RATIO |= 0x02;
					}

					(*m_pSnHeadData)->Sn_UPERS |= 0x02;//[Y]
				}
			}
			else{
				if( (*m_pSnHeadData)->IsUseSemiUriRatio() == TRUE ){
					Msg  = "課税売上割合に準ずる割合を無効にします。";
					ICSMessageBox( Msg, MB_OK, 0 );
//					pSyzShin->RATIO &= 0xfd;
					(*m_pSnHeadData)->Sn_UPERS &= 0xfd;
				}
			}
			if( (DiagData.data_check && (((*m_pSnHeadData)->Sn_UPERS&0x10)==0x00)) ||
				((DiagData.data_check==0) && ((*m_pSnHeadData)->Sn_UPERS&0x10)) ){
				CString		Msg;
				Msg.Empty();
				Msg =  _T("課税売上割合を変更する場合は、\n");
				Msg += _T("個別対応方式欄の⑭～⑯の金額を見直してください。");
				ICSMessageBox( Msg, MB_OK, 0 );
			}
		}
		(*m_pSnFhyo22Data)->Sn_2FUWSEX = F22UWS;	
		if( (*m_pSnFhyo22Data)->Sn_2FUWS < (*m_pSnHeadData)->m_UriRatioBorder ){
			// 課税売上に係る仕入税額
			memmove( F2215A, F2215AEX, 6 );
			memmove( F2215B, F2215BEX, 6 );
			memmove( F2215C, F2215CEX, 6 );
			// 共通の売上に係る仕入税額
			memmove( F2216A, F2216AEX, 6 );
			memmove( F2216B, F2216BEX, 6 );
			memmove( F2216C, F2216CEX, 6 );
		}
		(*m_pSnHeadData)->Sn_UPERS &= 0xef;
		if( DiagData.data_check ){
			(*m_pSnHeadData)->Sn_UPERS |= 0x10;
		}
		// 各入力の計算表示
		f22calq_all( 4 );
	}
}

void CShinFhyo22::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){

		// 前回の情報をセーブ
		char			SV_mtype = (*m_pSnTanaData)->Sn_MENZsw;
		unsigned short	SV_F22UWS = F22UWS;
		char			SV_mnk40[3][MONY_BUF_SIZE];
		char			SV_mnz40[3][MONY_BUF_SIZE];
		char			SV_mnk63[3][MONY_BUF_SIZE];
		char			SV_mnz63[3][MONY_BUF_SIZE];
		char			SV_menzk[3][MONY_BUF_SIZE];
		char			SV_menzz[3][MONY_BUF_SIZE];

		memmove( SV_mnk63[0], (*m_pSnTanaData)->Sn_mnk63[0], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　課税に係る
		memmove( SV_mnz63[0], (*m_pSnTanaData)->Sn_mnz63[0], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃
		memmove( SV_mnk63[1], (*m_pSnTanaData)->Sn_mnk63[1], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　非課税に係る
		memmove( SV_mnz63[1], (*m_pSnTanaData)->Sn_mnz63[1], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃
		memmove( SV_mnk63[2], (*m_pSnTanaData)->Sn_mnk63[2], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　共通に係る
		memmove( SV_mnz63[2], (*m_pSnTanaData)->Sn_mnz63[2], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃

		if( (*m_pSnHeadData)->SVmzsw == 1 ){
			memmove( SV_mnk40[0], (*m_pSnTanaData)->Sn_mnk40[0], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　課税に係る
			memmove( SV_mnz40[0], (*m_pSnTanaData)->Sn_mnz40[0], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃
			memmove( SV_mnk40[1], (*m_pSnTanaData)->Sn_mnk40[1], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　非課税に係る
			memmove( SV_mnz40[1], (*m_pSnTanaData)->Sn_mnz40[1], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃
			memmove( SV_mnk40[2], (*m_pSnTanaData)->Sn_mnk40[2], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　共通に係る
			memmove( SV_mnz40[2], (*m_pSnTanaData)->Sn_mnz40[2], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃

			memmove( SV_menzk[0], (*m_pSnTanaData)->Sn_menzk[0], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　課税に係る
			memmove( SV_menzz[0], (*m_pSnTanaData)->Sn_menzz[0], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃
			memmove( SV_menzk[1], (*m_pSnTanaData)->Sn_menzk[1], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　非課税に係る
			memmove( SV_menzz[1], (*m_pSnTanaData)->Sn_menzz[1], MONY_BUF_SIZE );	//消費税額   (〃      )　〃
			memmove( SV_menzk[2], (*m_pSnTanaData)->Sn_menzk[2], MONY_BUF_SIZE );	// 棚卸資産額 (新税率分)　共通に係る
			memmove( SV_menzz[2], (*m_pSnTanaData)->Sn_menzz[2], MONY_BUF_SIZE );	// 消費税額   (〃      )　〃
		}

		// 個別対応
		CChangeTaxEx	CTDiagEx;
		CTDiagEx.InitInfo( (*m_pSnFhyo22Data), (*m_pSnHeadData), (*m_pSnTanaData), m_pArith );

//		ItSw = 0;

		HINSTANCE svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle( g_hInstance );
		int st = (int)CTDiagEx.DoModal();
		AfxSetResourceHandle( svhInstResource );

//		ItSw = 1;
		if( st == IDOK ){

			// 手入力
			if( !((*m_pSnTanaData)->Sn_tansw & 0x01) || ((*m_pSnHeadData)->Sn_Sign4&0x80) ){//[Y]非連動の場合は常に更新させる
				// 前回が個別対応で95％未満の場合、前回分を削除
				if( (SV_mtype&0x10) || ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) ){
					if( SV_mtype&0x01 ){
						m_pArith->l_add( F2215C, F2215C, SV_mnz63[0] );		// 15. 課税売上に係る仕入税額
						m_pArith->l_add( F2216C, F2216C, SV_mnz63[2] );		// 16. 共通の売上に係る仕入税額

						if( (*m_pSnHeadData)->SVmzsw == 1 ){
							m_pArith->l_add( F2215B, F2215B, SV_mnz40[0] );		// 15. 課税売上に係る仕入税額
							m_pArith->l_add( F2216B, F2216B, SV_mnz40[2] );		// 16. 共通の売上に係る仕入税額
							m_pArith->l_add( F2215A, F2215A, SV_menzz[0] );	// 15. 課税売上に係る仕入税額
							m_pArith->l_add( F2216A, F2216A, SV_menzz[2] );	// 16. 共通の売上に係る仕入税額
						}
					}
					else{
						m_pArith->l_sub( F2215C, F2215C, SV_mnz63[0] );		// 15. 課税売上に係る仕入税額
						m_pArith->l_sub( F2216C, F2216C, SV_mnz63[2] );		// 16. 共通の売上に係る仕入税額
						if( (*m_pSnHeadData)->SVmzsw == 1 ){
							m_pArith->l_sub( F2215B, F2215B, SV_mnz40[0] );		// 15. 課税売上に係る仕入税額
							m_pArith->l_sub( F2216B, F2216B, SV_mnz40[2] );		// 16. 共通の売上に係る仕入税額
							m_pArith->l_sub( F2215A, F2215A, SV_menzz[0] );	// 15. 課税売上に係る仕入税額
							m_pArith->l_sub( F2216A, F2216A, SV_menzz[2] );	// 16. 共通の売上に係る仕入税額
						}
					}
				}

				// 今回分を変更
				// [03'11.27]
				char	CkMyA[MONY_BUF_SIZE]={0}, CkMyB[MONY_BUF_SIZE]={0} , CkMyC[MONY_BUF_SIZE]={0};
				memmove( CkMyA, F2212A, MONY_BUF_SIZE );
				memmove( CkMyB, F2212B, MONY_BUF_SIZE );
				memmove( CkMyC, F2212B, MONY_BUF_SIZE );

				if( (*m_pSnTanaData)->Sn_MENZsw & 0x10 ){
					memset( F2212C, '\0', MONY_BUF_SIZE );
					m_pArith->l_add( F2212C, F2212C, (*m_pSnTanaData)->Sn_mnz63[0] );
					m_pArith->l_add( F2212C, F2212C, (*m_pSnTanaData)->Sn_mnz63[1] );
					m_pArith->l_add( F2212C, F2212C, (*m_pSnTanaData)->Sn_mnz63[2] );
					if( (*m_pSnHeadData)->SVmzsw == 1 ){
						memset( F2212B, '\0', MONY_BUF_SIZE );
						m_pArith->l_add( F2212B, F2212B, (*m_pSnTanaData)->Sn_mnz40[0] );
						m_pArith->l_add( F2212B, F2212B, (*m_pSnTanaData)->Sn_mnz40[1] );
						m_pArith->l_add( F2212B, F2212B, (*m_pSnTanaData)->Sn_mnz40[2] );

						memset( F2212A, '\0', MONY_BUF_SIZE );
						m_pArith->l_add( F2212A, F2212A, (*m_pSnTanaData)->Sn_menzz[0] );	//				 〃	   消費税額   (〃      )　〃
						m_pArith->l_add( F2212A, F2212A, (*m_pSnTanaData)->Sn_menzz[1] );	//				 〃	   消費税額   (〃      )　〃
						m_pArith->l_add( F2212A, F2212A, (*m_pSnTanaData)->Sn_menzz[2] );	//				 〃	   消費税額   (〃      )　〃
					}
					// 旧格納場所へ格納
					memmove( (*m_pSnTanaData)->Sn_MNZ63, F2212C, MONY_BUF_SIZE );
					if( (*m_pSnHeadData)->SVmzsw == 1 ){
						memmove( (*m_pSnTanaData)->Sn_MNZ40, F2212B, MONY_BUF_SIZE );
						memmove( (*m_pSnTanaData)->Sn_MENZZ, F2212A, MONY_BUF_SIZE );
					}
					if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ){
						m_pArith->l_neg( F2212A );
						m_pArith->l_neg( F2212B );
						m_pArith->l_neg( F2212C );
					}
					// 個別対応で95％未満の場合
					if( (F22UWS<(*m_pSnHeadData)->m_UriRatioBorder) || ((*m_pSnHeadData)->IsUseSemiUriRatio()==TRUE) ){
						if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ){
							m_pArith->l_sub( F2215C, F2215C, (*m_pSnTanaData)->Sn_mnz63[0] );
							m_pArith->l_sub( F2216C, F2216C, (*m_pSnTanaData)->Sn_mnz63[2] );
							if( (*m_pSnHeadData)->SVmzsw == 1 ){
								m_pArith->l_sub( F2215B, F2215B, (*m_pSnTanaData)->Sn_mnz40[0] );
								m_pArith->l_sub( F2216B, F2216B, (*m_pSnTanaData)->Sn_mnz40[2] );
								m_pArith->l_sub( F2215A, F2215A, (*m_pSnTanaData)->Sn_menzz[0] );
								m_pArith->l_sub( F2216A, F2216A, (*m_pSnTanaData)->Sn_menzz[2] );
							}
						}
						else{
							m_pArith->l_add( F2215C, F2215C, (*m_pSnTanaData)->Sn_mnz63[0] );
							m_pArith->l_add( F2216C, F2216C, (*m_pSnTanaData)->Sn_mnz63[2] );
							if( (*m_pSnHeadData)->SVmzsw == 1 ){
								m_pArith->l_add( F2215B, F2215B, (*m_pSnTanaData)->Sn_mnz40[0] );
								m_pArith->l_add( F2216B, F2216B, (*m_pSnTanaData)->Sn_mnz40[2] );
								m_pArith->l_add( F2215A, F2215A, (*m_pSnTanaData)->Sn_menzz[0] );
								m_pArith->l_add( F2216A, F2216A, (*m_pSnTanaData)->Sn_menzz[2] );
							}
						}
//--> '17.11.20 INS START
						if( (*m_pSnHeadData)->SVmzsw == 1 ){
							memmove( F2215AEX, F2215A, MONY_BUF_SIZE );
							memmove( F2215BEX, F2215B, MONY_BUF_SIZE );
							memmove( F2216AEX, F2216A, MONY_BUF_SIZE );
							memmove( F2216BEX, F2216B, MONY_BUF_SIZE );
						}
						memmove( F2215CEX, F2215C, MONY_BUF_SIZE );
						memmove( F2216CEX, F2216C, MONY_BUF_SIZE );
//<-- '17.11.20 INS END
					}
				}
				else{
					memset( F2212C, '\0', MONY_BUF_SIZE );
					memset( F2212B, '\0', MONY_BUF_SIZE );
					memset( F2212A, '\0', MONY_BUF_SIZE );
				}
				// 再表示
				DispFh22();
				SetItemPosition();	// ('14.03.13)
			}
		}
	}
	else{
		CChangeTax		CTDiag;
		CTDiag.InitInfo( (*m_pSnFhyo22Data), (*m_pSnHeadData), (*m_pSnTanaData), m_pArith );

		// 比例配分
//		ItSw = 0;

		HINSTANCE svhInstResource = AfxGetResourceHandle();
		AfxSetResourceHandle( g_hInstance );
		int st = (int)CTDiag.DoModal();
		AfxSetResourceHandle( svhInstResource );

//		ItSw = 1;
		if( st == IDOK ){

			if( !((*m_pSnTanaData)->Sn_tansw & 0x01) || ((*m_pSnHeadData)->Sn_Sign4&0x80) ){//[Y]非連動の場合は常に更新させる

				// [03'11.27]
				char	CkMyA[MONY_BUF_SIZE]={0}, CkMyB[MONY_BUF_SIZE]={0} , CkMyC[MONY_BUF_SIZE]={0};
				memmove( CkMyA, F2212A, MONY_BUF_SIZE );
				memmove( CkMyB, F2212B, MONY_BUF_SIZE );
				memmove( CkMyC, F2212C, MONY_BUF_SIZE );
				// (免税)控除対象調整税額
				if( (*m_pSnTanaData)->Sn_MENZsw & 0x10 ){
					memmove( F2212A, (*m_pSnTanaData)->Sn_MENZZ, MONY_BUF_SIZE );
					memmove( F2212B, (*m_pSnTanaData)->Sn_MNZ40, MONY_BUF_SIZE );
					memmove( F2212C, (*m_pSnTanaData)->Sn_MNZ63, MONY_BUF_SIZE );
					if( (*m_pSnTanaData)->Sn_MENZsw & 0x01 ){
						m_pArith->l_neg( F2212A );
						m_pArith->l_neg( F2212B );
						m_pArith->l_neg( F2212C );
					}
				}
				else{
					memset( F2212B, '\0', MONY_BUF_SIZE );
					memset( F2212A, '\0', MONY_BUF_SIZE );
					memset( F2212C, '\0', MONY_BUF_SIZE );
				}
				// 再表示
				DispFh22();
				SetItemPosition();	// ('14.03.13)
			}
		}
	}
}

void CShinFhyo22::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	// 経過措置対象の課税資産の譲渡等あり
	((CH26HyoView*)m_pParent)->SpecPassDlg( 1 );

	// 画面出力
	DispFh22();
//2015.03.31 INSERT START
	SetItemPosition();
//2015.03.31 INSERT END
}

//-----------------------------------------------------------------------------
// 各入力データ抽出
//-----------------------------------------------------------------------------
void CShinFhyo22::GetAllDiagData()
{
	if( (m_Fhyo22Diag02.GetPosition()==ID228A) || (m_Fhyo22Diag02.GetPosition()==ID228B) || (m_Fhyo22Diag02.GetPosition()==ID228C) ){
		m_Fhyo22Diag01.SetFocus();
	}
	
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	//  1. 課税売上額
	diag_getdata( IDC_ICSDIAGCTRL2, ID221A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F221A, DiagData.data_val, MONY_BUF_SIZE ); 
	//  1. 課税売上額
	diag_getdata( IDC_ICSDIAGCTRL2, ID221B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F221B, DiagData.data_val, MONY_BUF_SIZE ); 
	//  1. 課税売上額
	diag_getdata( IDC_ICSDIAGCTRL2, ID221C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F221C, DiagData.data_val, MONY_BUF_SIZE ); 

	//  2. 免税売上額
	diag_getdata( IDC_ICSDIAGCTRL2, ID222C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F222C, DiagData.data_val, MONY_BUF_SIZE ); 
	//  3. 非課税資産の輸出金額	
	diag_getdata( IDC_ICSDIAGCTRL2, ID223C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F223C, DiagData.data_val, MONY_BUF_SIZE ); 
	//	6. 非課税売上額
	diag_getdata( IDC_ICSDIAGCTRL2, ID226C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F226C, DiagData.data_val, MONY_BUF_SIZE ); 

	//	8. 課税仕入に係る支払対価の額
	diag_getdata( IDC_ICSDIAGCTRL2, ID228A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F228A, DiagData.data_val, MONY_BUF_SIZE ); 
	//	8. 課税仕入に係る支払対価の額
	diag_getdata( IDC_ICSDIAGCTRL2, ID228B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F228B, DiagData.data_val, MONY_BUF_SIZE ); 
	//	8. 課税仕入に係る支払対価の額
	diag_getdata( IDC_ICSDIAGCTRL2, ID228C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F228C, DiagData.data_val, MONY_BUF_SIZE ); 
	
	//	9. 課税仕入に係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID229A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F229A, DiagData.data_val, MONY_BUF_SIZE ); 
	//	9. 課税仕入に係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID229B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F229B, DiagData.data_val, MONY_BUF_SIZE ); 
	//	9. 課税仕入に係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID229C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F229C, DiagData.data_val, MONY_BUF_SIZE ); 

	// 11. 課税貨物に係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2211A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2211A, DiagData.data_val, MONY_BUF_SIZE ); 
	// 11. 課税貨物に係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2211B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2211B, DiagData.data_val, MONY_BUF_SIZE ); 
	// 11. 課税貨物に係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2211C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2211C, DiagData.data_val, MONY_BUF_SIZE ); 

	// 15. 課税売上に係る仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2215A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2215A, DiagData.data_val, MONY_BUF_SIZE ); 
	// 15. 課税売上に係る仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2215B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2215B, DiagData.data_val, MONY_BUF_SIZE ); 
	// 15. 課税売上に係る仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2215C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2215C, DiagData.data_val, MONY_BUF_SIZE ); 

	// 16. 共通の売上に係る仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2216A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2216A, DiagData.data_val, MONY_BUF_SIZE ); 
	// 16. 共通の売上に係る仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2216B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2216B, DiagData.data_val, MONY_BUF_SIZE ); 
	// 16. 共通の売上に係る仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2216C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2216C, DiagData.data_val, MONY_BUF_SIZE ); 

	// 17. 個別対応方式控除仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2217A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2217A, DiagData.data_val, MONY_BUF_SIZE ); 
	// 17. 個別対応方式控除仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2217B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2217B, DiagData.data_val, MONY_BUF_SIZE ); 
	// 17. 個別対応方式控除仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2217C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2217C, DiagData.data_val, MONY_BUF_SIZE ); 

	// 18. 比例配分方式控除仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2218A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2218A, DiagData.data_val, MONY_BUF_SIZE ); 
	// 18. 比例配分方式控除仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2218B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2218B, DiagData.data_val, MONY_BUF_SIZE ); 
	// 18. 比例配分方式控除仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2218C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2218C, DiagData.data_val, MONY_BUF_SIZE ); 

	// 19. 調整対象固定資産に係る調整税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2219A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2219A, DiagData.data_val, MONY_BUF_SIZE ); 
	// 19. 調整対象固定資産に係る調整税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2219B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2219B, DiagData.data_val, MONY_BUF_SIZE ); 
	// 19. 調整対象固定資産に係る調整税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2219C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2219C, DiagData.data_val, MONY_BUF_SIZE ); 

	// 20. 調整対象固定資産を課税業務用に転用した調整額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2220A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2220A, DiagData.data_val, MONY_BUF_SIZE ); 
	// 20. 調整対象固定資産を課税業務用に転用した調整額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2220B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2220B, DiagData.data_val, MONY_BUF_SIZE ); 
	// 20. 調整対象固定資産を課税業務用に転用した調整額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2220C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2220C, DiagData.data_val, MONY_BUF_SIZE ); 

	// 21. 控除対象仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2221A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2221A, DiagData.data_val, MONY_BUF_SIZE ); 
	// 21. 控除対象仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2221B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2221B, DiagData.data_val, MONY_BUF_SIZE ); 	
	// 21. 控除対象仕入税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2221C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2221C, DiagData.data_val, MONY_BUF_SIZE ); 	

	// 23. 貸倒回収に係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2223A, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2223A, DiagData.data_val, MONY_BUF_SIZE ); 
	// 23. 貸倒回収に係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2223B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2223B, DiagData.data_val, MONY_BUF_SIZE ); 
	// 23. 貸倒回収に係る消費税額
	diag_getdata( IDC_ICSDIAGCTRL2, ID2223C, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( F2223C, DiagData.data_val, MONY_BUF_SIZE ); 
}


//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinFhyo22::UpDownFocusMoveCheck(int Index , int Move)
{
	int idx = 0;
	int i = 0 , j = 0;

	DIAGRAM_ATTRIBUTE	DA;

	//現在ポジション検索
	for(i = 0 ; i < FCS_FHYO22CNT ; i++){
		if(Index == FCS_Fhyo22[i].IDNo){
			break;
		}
	}

	idx = FCS_Fhyo22[i].IDNo;//現在ポジションセット

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_FHYO22CNT ; j++){
			m_Fhyo22Diag02.GetAttribute(FCS_Fhyo22[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo22[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Fhyo22Diag02.GetAttribute(FCS_Fhyo22[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Fhyo22[j].IDNo;
				break;
			}
		}
	}

	return idx;
}


//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
void CShinFhyo22::SetItemPosition()
{
	int index;

	//-->追加 harino ('14.03.06)
	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAGCTRL2 ) {
		index = m_Fhyo22Diag02.GetPosition();
		if( (index = m_Fhyo22Diag02.GetPosition()) != -1) {
			m_Fhyo22Diag02.SetPosition(index);
			m_Fhyo22Diag02.SetFocus();
			return;
		}
	}
	//<--追加 harino ('14.03.06)

	m_Fhyo22Diag01.SetFocus();	// ↓のSetFocusを有効にするためにダミーでセット ('14.04.14)
	m_Fhyo22Diag02.SetFocus();

	//入力可能なインデックスを探す処理が必要？
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){//[Y]非連動の場合
		m_Fhyo22Diag02.SetPosition( ID221A );
	}
	else{
		m_Fhyo22Diag02.SetPosition( ID2219A );
	}
	SetRedrawScroll( 0 );
}

//-----------------------------------------------------------------------------
// 他表からの計算 ('14.05.20)
//-----------------------------------------------------------------------------
void CShinFhyo22::calq_by_OtherChange()
{
	m_DpSw = -1;

	ReadData( 1 );
	f22calq_all( 1 );
	WriteData( 1 );

	m_DpSw = 0;
}

//-----------------------------------------------------------------------------
// 特定収入関連の手入力サイン連動('15.05.20)
//-----------------------------------------------------------------------------
// 引数		sw	：	セット対象サイン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CShinFhyo22::SetCalqSw( int sw )
{
	F2221c = sw;

	return 0;
}
